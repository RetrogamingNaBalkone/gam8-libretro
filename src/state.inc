// Canonical little-endian wire format. Never copy host pointers or struct padding.
typedef struct {
    uint8_t *out;
    const uint8_t *in;
    size_t pos;
} stream_t;

static uint64_t scalar(stream_t *s, uint64_t value, unsigned width)
{
    if (s->in)
        value = 0;
    for (unsigned i = 0; i < width; ++i) {
        if (s->out)
            s->out[s->pos] = (uint8_t)(value >> (8 * i));
        if (s->in)
            value |= (uint64_t)s->in[s->pos] << (8 * i);
        ++s->pos;
    }
    return value;
}
static void bytes(stream_t *s, uint8_t *data, size_t size)
{
    if (s->in)
        memcpy(data, s->in + s->pos, size);
    if (s->out)
        memcpy(s->out + s->pos, data, size);
    s->pos += size;
}
#define FIELD(x)                                                                                   \
    do {                                                                                           \
        uint64_t v = scalar(s, (uint64_t)(x), sizeof(x));                                          \
        if (s->in)                                                                                 \
            (x) = v;                                                                               \
    } while (0)
static void machine_state(stream_t *s, machine_t *m)
{
    cpu_t *c = &m->cpu;
    FIELD(c->micro);
    FIELD(c->PC);
    FIELD(c->SP);
    FIELD(c->TMP);
    FIELD(c->TMP2);
    FIELD(c->A);
    FIELD(c->X);
    FIELD(c->Y);
    FIELD(c->P);
    FIELD(c->IR);
    FIELD(c->irq_sampled);
    FIELD(c->irq_taken);
    FIELD(c->nmi_pending);
    bytes(s, m->ram, sizeof(m->ram));
    bytes(s, m->vram, sizeof(m->vram));
    FIELD(m->lcd);
    FIELD(m->scroll_x);
    FIELD(m->scroll_y);
    FIELD(m->plane);
    FIELD(m->bank);
    FIELD(m->multibank);
    FIELD(m->available);
    FIELD(m->prot_bit);
    FIELD(m->prot_byte);
    FIELD(m->prot_seq);
    FIELD(m->failed);
    FIELD(m->replying);
    FIELD(m->unlocked);
    FIELD(m->cursor);
    FIELD(m->irq_timer);
    FIELD(m->irq_width);
    FIELD(m->sound_phase);
    FIELD(m->filter_phase);
    FIELD(m->filter_base);
    FIELD(m->sound_head);
    for (unsigned i = 0; i < 32; ++i) {
        FIELD(m->sound_ring[i][0]);
        FIELD(m->sound_ring[i][1]);
    }
    FIELD(m->joy);
    FIELD(m->audio_phase);
    FIELD(m->line_phase);
    psg_t *p = &m->psg;
    bytes(s, p->reg, sizeof(p->reg));
    bytes(s, p->tone, sizeof(p->tone));
    FIELD(p->noise_phase);
    FIELD(p->attack);
    FIELD(p->hold);
    FIELD(p->alternate);
    FIELD(p->holding);
    FIELD(p->step);
    for (unsigned i = 0; i < 3; ++i) {
        FIELD(p->count[i]);
    }
    FIELD(p->noise_count);
    FIELD(p->rng);
    FIELD(p->env_count);
    FIELD(p->output[0]);
    FIELD(p->output[1]);
}
#undef FIELD
enum { STATE_HEADER = 20, STATE_DISPLAY = 18 };
size_t retro_serialize_size(void)
{
    stream_t count = {0};
    machine_state(&count, &machine);
    return STATE_HEADER + count.pos + STATE_DISPLAY + WIDTH * HEIGHT * 4;
}
bool retro_serialize(void *data, size_t size)
{
    size_t needed = retro_serialize_size();
    if (!cart || !data || size < needed)
        return false;
    stream_t s = {(uint8_t *)data, NULL, 0};
    scalar(&s, 0x31534d47, 4);
    scalar(&s, 1, 4);
    scalar(&s, cart_id, 4);
    scalar(&s, bios_id, 4);
    scalar(&s, 0, 4);
    machine_state(&s, &machine);
    scalar(&s, valid_history, 1);
    scalar(&s, ghost, 1);
    for (unsigned i = 0; i < 4; ++i)
        scalar(&s, colors[i], 4);
    for (unsigned i = 0; i < WIDTH * HEIGHT; ++i)
        scalar(&s, valid_history ? history[i] : 0, 4);
    uint32_t crc = gamate_crc32(s.out + STATE_HEADER, needed - STATE_HEADER);
    s.pos = 16;
    scalar(&s, crc, 4);
    return true;
}

static bool sane(const machine_t *m)
{
    const psg_t *p = &m->psg;
    return machine_cpu_valid(&m->cpu) && m->cursor < 8192 && m->plane < 2 && m->prot_bit <= 8 &&
           (!m->replying || m->prot_bit < 8) && m->prot_seq <= 15 &&
           (m->prot_seq < 15 || m->replying || m->failed) && m->sound_phase < CPU_HZ &&
           m->filter_phase < 3 * SAMPLE_RATE && m->filter_base < 32 && m->sound_head < 32 &&
           m->line_phase < 608093u * HEIGHT && m->audio_phase < CPU_HZ && p->reg[8] < 32 &&
           p->reg[9] < 32 && p->reg[10] < 32 && p->step < 16 && (p->attack == 0 || p->attack == 15);
}
bool retro_unserialize(const void *data, size_t size)
{
    size_t needed = retro_serialize_size();
    if (!cart || !data || size < needed)
        return false;
    stream_t s = {NULL, (const uint8_t *)data, 0};
    if (scalar(&s, 0, 4) != 0x31534d47 || scalar(&s, 0, 4) != 1 || scalar(&s, 0, 4) != cart_id ||
        scalar(&s, 0, 4) != bios_id ||
        scalar(&s, 0, 4) != gamate_crc32(s.in + STATE_HEADER, needed - STATE_HEADER))
        return false;
    // Static scratch keeps console port stacks small and failure atomic.
    static machine_t next;
    machine_state(&s, &next);
    if (!sane(&next))
        return false;
    machine = next;
    bool restore = scalar(&s, 0, 1) != 0;
    if (scalar(&s, 0, 1) != ghost)
        restore = false;
    for (unsigned i = 0; i < 4; ++i)
        if (scalar(&s, 0, 4) != colors[i])
            restore = false;
    for (unsigned i = 0; i < WIDTH * HEIGHT; ++i)
        history[i] = (uint32_t)scalar(&s, 0, 4);
    valid_history = restore;
    return true;
}
