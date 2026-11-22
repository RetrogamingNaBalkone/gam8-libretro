// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <stdint.h>
#include <stddef.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif
typedef struct {
    uint16_t PC, SP, TMP, micro;
    uint8_t A, X, Y, P, TMP2, IR, irq_sampled, irq_taken, nmi_pending;
} cpu_t;

enum { WIDTH = 160, HEIGHT = 150, CPU_HZ = 2216500, SAMPLE_RATE = 48000 };
enum board { PLAIN, BANKED, MULTI };
typedef struct {
    uint8_t reg[16], tone[3], noise_phase, attack, hold, alternate, holding, step;
    uint16_t count[3], noise_count;
    uint32_t rng, env_count;
    int16_t output[2];
} psg_t;
typedef struct {
    cpu_t cpu;
    uint8_t ram[1024], vram[16384];
    uint8_t lcd, scroll_x, scroll_y, plane, bank, multibank;
    uint8_t available, prot_bit, prot_byte, prot_seq, failed, replying, unlocked;
    uint16_t cursor, irq_timer;
    uint8_t irq_width, joy, sound_head, filter_base;
    int16_t sound_ring[32][2];
    uint32_t sound_phase, filter_phase;
    uint32_t audio_phase, line_phase;
    psg_t psg;
} machine_t;

void machine_power(machine_t *m);
void machine_reset(machine_t *m);
void machine_cart(const uint8_t *data, size_t size, const uint8_t *bios, enum board pcb);
size_t machine_frame(machine_t *m, uint8_t *pixels, int16_t *pcm);
bool machine_cart_size_supported(size_t size);
enum board machine_board(uint32_t crc, size_t size, bool use_multicart);
uint32_t gamate_crc32(const void *data, size_t size);
void psg_write(psg_t *p, unsigned reg, uint8_t data);
void psg_tick(psg_t *p);

bool machine_cpu_valid(const cpu_t *c);
