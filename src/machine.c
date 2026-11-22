// SPDX-License-Identifier: GPL-2.0-or-later
// Adapted from MAME's Bit Corp Gamate driver and devices. See NOTICE.

#include "machine.h"
#include <string.h>

static const uint8_t *rom, *firmware;
static size_t rom_mask;
static enum board board_type;
enum { F_C = 1, F_Z = 2, F_I = 4, F_D = 8, F_B = 16, F_E = 32, F_V = 64, F_N = 128 };
static uint8_t read_bus(machine_t *m, uint16_t addr);
static void write_bus(machine_t *m, uint16_t addr, uint8_t data);
static uint16_t set_h(uint16_t base, uint8_t val) { return (base & 255) | ((unsigned)val << 8); }
static uint16_t set_l(uint16_t base, uint8_t val) { return (base & 0xff00) | val; }
static bool page_changing(uint16_t base, int delta) { return ((base + delta) ^ base) & 0xff00; }
#include "cpu_alu.inc"
#include "cpu_ops.inc"
#include "resample.inc"
bool machine_cpu_valid(const cpu_t *c) { return c->micro < CPU_MICRO_COUNT; }

uint32_t gamate_crc32(const void *bytes, size_t size)
{
    const uint8_t *p = (const uint8_t *)bytes;
    uint32_t crc = ~0u;
    while (size--) {
        crc ^= *p++;
        for (unsigned b = 0; b < 8; ++b)
            crc = (crc >> 1) ^ ((0u - (crc & 1u)) & 0xedb88320u);
    }
    return ~crc;
}

bool machine_cart_size_supported(size_t size)
{
    return size >= 0x4000 && size <= 0x80000 && (size & (size - 1)) == 0;
}

enum board machine_board(uint32_t crc, size_t size, bool use_multicart)
{
    static const struct {
        uint32_t crc, size;
        enum board type;
    } records[] = {
#include "boards.inc"
    };
    for (size_t i = 0; i < sizeof(records) / sizeof(records[0]); ++i)
        if (records[i].crc == crc && records[i].size == size)
            return records[i].type;
    if (size == 0x20000 && use_multicart)
        return MULTI;
    return size <= 0x8000 ? PLAIN : BANKED;
}

void machine_cart(const uint8_t *data, size_t size, const uint8_t *bios, enum board pcb)
{
    rom = data;
    rom_mask = size ? size - 1 : 0;
    firmware = bios;
    board_type = pcb;
}

void machine_reset(machine_t *m)
{
    m->available = m->bank = m->multibank = 0;
    m->prot_bit = m->prot_byte = m->prot_seq = m->failed = m->replying = m->unlocked = 0;
    m->lcd = m->cursor = m->scroll_x = m->plane = 0;
    m->scroll_y = 10;
    memset(&m->psg, 0, sizeof(m->psg));
    m->psg.rng = 1;
    psg_write(&m->psg, 13, 0);
    m->cpu.micro = CPU_RESET;
    m->cpu.irq_taken = m->cpu.irq_sampled = 0;
}

void machine_power(machine_t *m)
{
    memset(m, 0, sizeof(*m));
    memset(m->ram, 255, sizeof(m->ram));
    filter_init();
    // MAME renders every source sample whose start precedes a register write.
    m->sound_phase = CPU_HZ - 1;
    m->cpu.P = 0x36;
    m->cpu.X = 0x80;
    m->cpu.SP = 0x100;
    m->joy = 255;
    m->irq_timer = 1001;
    machine_reset(m);
}

static uint8_t cartridge_read(machine_t *m, uint16_t addr)
{
    if (!m->unlocked) {
        if (!m->replying)
            return 0;
        unsigned bit = (0x47 >> (7 - m->prot_bit)) & 1;
        if (++m->prot_bit == 8) {
            m->unlocked = 1;
            m->prot_bit = 0;
        }
        return (uint8_t)(bit << 1);
    }
    unsigned offset = addr - 0x6000;
    if (board_type != PLAIN) {
        unsigned bank = offset >= 0x4000 ? m->bank : m->multibank;
        offset = bank * 0x4000 + (offset & 0x3fff);
    }
    return rom[offset & rom_mask];
}

static void cartridge_write(machine_t *m, uint16_t addr, uint8_t data)
{
    if (m->unlocked) {
        if (board_type != PLAIN && addr == 0xc000)
            m->bank = data;
        if (board_type == MULTI && addr == 0x8000)
            m->multibank = data;
    } else if (!m->replying && !m->failed) {
        static const char key[] = "BIT CORPORATION";
        if (m->prot_bit < 8) {
            m->prot_byte |= ((data >> 2) & 1) << (7 - m->prot_bit++);
        } else {
            m->failed = m->prot_byte != key[m->prot_seq++];
            m->prot_bit = m->prot_byte = 0;
            m->replying = !m->failed && m->prot_seq == sizeof(key) - 1;
        }
    }
}

static unsigned vram_port(machine_t *m)
{
    unsigned offset = 2u * m->cursor + m->plane;
    m->cursor = (m->cursor + ((m->lcd & 64) ? 32 : 1)) & 8191;
    return offset;
}

static uint8_t read_bus(machine_t *m, uint16_t addr)
{
    if (addr < 0x2000)
        return m->ram[addr & 1023];
    if (addr >= 0xe000)
        return firmware[addr & 4095];
    if (addr >= 0x6000)
        return cartridge_read(m, addr);
    switch (addr >> 10) {
    case 0x10:
        return m->psg.reg[addr & 15];
    case 0x11:
        return m->joy;
    case 0x12:
        return 0;
    case 0x14:
        if ((addr & 7) == 6)
            return m->vram[vram_port(m)];
        break;
    }
    if (addr == 0x5800) {
        m->available = 1;
        return 0;
    }
    if (addr == 0x5a00)
        return m->available ? 3 : 1;
    return 0;
}

static void write_bus(machine_t *m, uint16_t addr, uint8_t data)
{
    if (addr < 0x2000)
        m->ram[addr & 1023] = data;
    else if (addr >= 0x6000 && addr < 0xe000)
        cartridge_write(m, addr, data);
    else if ((addr >> 10) == 0x10) {
#ifdef GAMATE_TRACE_PSG
        GAMATE_TRACE_PSG(addr & 15, data);
#endif
        psg_write(&m->psg, addr & 15, data);
    } else if ((addr >> 10) == 0x14)
        switch (addr & 7) {
        case 1:
            m->lcd = data;
            break;
        case 2:
            m->scroll_x = data;
            break;
        case 3:
            m->scroll_y = data;
            break;
        case 4:
            m->plane = data >> 7;
            m->cursor = (m->cursor & 0x1fe0) | (data & 31);
            break;
        case 5:
            m->cursor = (m->cursor & 31) | (data << 5);
            break;
        case 7:
            m->vram[vram_port(m)] = data;
            break;
        }
}

static void scanline(const machine_t *m, unsigned line, uint8_t *dst)
{
    unsigned x = m->scroll_x, y = line;
    if (m->lcd & 128) {
        memset(dst, 0, WIDTH);
        return;
    }
    if (m->scroll_y < 200) {
        y = (line + m->scroll_y) % 200;
        if ((m->lcd & 32) && line < 16) {
            x = 0;
            y = 208 + line;
        }
    } else if (m->scroll_y & 8) {
        y = 0;
    } else if (line <= (m->scroll_y & 7u)) {
        x = 0;
        y = 255 + line - (m->scroll_y & 7u);
    }
    for (unsigned i = 0; i < WIDTH; ++i, ++x) {
        unsigned offset = y * 64 + ((x & 255) >> 3) * 2;
        unsigned shift = 7 - (x & 7);
        unsigned a = (m->vram[offset] >> shift) & 1;
        unsigned b = (m->vram[offset + 1] >> shift) & 1;
        dst[i] = (uint8_t)((m->lcd & 16) ? b + 2 * a : a + 2 * b);
    }
}

size_t machine_frame(machine_t *m, uint8_t *pixels, int16_t *pcm)
{
    size_t samples = 0;
    for (unsigned line = 0; line < HEIGHT; ++line) {
        scanline(m, line, pixels + line * WIDTH);
        // Distribute fractional clocks across scanlines at MAME's 60.8093 Hz.
        uint64_t budget = m->line_phase + (uint64_t)CPU_HZ * 10000;
        const unsigned divisor = 608093u * HEIGHT;
        unsigned cycles = (unsigned)(budget / divisor);
        m->line_phase = (uint32_t)(budget % divisor);
        while (cycles--) {
#ifdef GAMATE_TRACE_CLOCK
            GAMATE_TRACE_CLOCK();
#endif
            if (!--m->irq_timer) {
                m->irq_timer = 16384;
                m->irq_width = 10;
            }
            cpu_tick(m, m->irq_width != 0);
            if (m->irq_width)
                --m->irq_width;
            m->sound_phase += AY_RATE;
            if (m->sound_phase >= CPU_HZ) {
                m->sound_phase -= CPU_HZ;
                source_sample(m);
            }
            m->audio_phase += SAMPLE_RATE;
            if (m->audio_phase >= CPU_HZ) {
                m->audio_phase -= CPU_HZ;
                output_sample(m, pcm + 2 * samples);
                ++samples;
            }
        }
    }
    return samples;
}
