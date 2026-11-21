// BSD-3-Clause, adapted from MAME's audio_resampler_lofi by Olivier Galibert.
// For Gamate the source is 138531 Hz and three AY samples form one group.
enum { AY_RATE = 138531, GROUP_RATE_DEN = 3 * SAMPLE_RATE };
static int32_t f0[4097], f1[4097];
static void filter_init(void)
{
    for (unsigned i = 0; i <= 4096; ++i) {
        double p = i / 4096.0;
        f0[i] = (int32_t)((p - p * p * p) * (65536.0 / 6) + 0.5);
        if (i <= 2048)
            f1[i] = (int32_t)((p + (p * p - p * p * p) * 0.5) * 65536 + 0.5);
        else
            f1[i] = 65536 + f0[i] + f0[4096 - i] - f1[4096 - i];
    }
}

static void source_sample(machine_t *m)
{
    psg_tick(&m->psg);
    memcpy(m->sound_ring[m->sound_head], m->psg.output, sizeof(m->psg.output));
    m->sound_head = (m->sound_head + 1) & 31;
}

static void output_sample(machine_t *m, int16_t *out)
{
    unsigned phase = (m->filter_phase * 4096u) / GROUP_RATE_DEN;
    int32_t w[4] = {-f0[4096 - phase], f1[4096 - phase], f1[phase], -f0[phase]};
    int64_t sum[2] = {0, 0};
    for (unsigned group = 0; group < 4; ++group) {
        unsigned start = m->filter_base + 3 * group - 12;
        for (unsigned ch = 0; ch < 2; ++ch) {
            int value = m->sound_ring[start & 31][ch] + m->sound_ring[(start + 1) & 31][ch] +
                        m->sound_ring[(start + 2) & 31][ch];
            sum[ch] += (int64_t)w[group] * value;
        }
    }
    for (unsigned ch = 0; ch < 2; ++ch)
        out[ch] = (int16_t)(sum[ch] / (65536 * 3));
    m->filter_phase += AY_RATE;
    if (m->filter_phase >= GROUP_RATE_DEN) {
        m->filter_phase -= GROUP_RATE_DEN;
        m->filter_base = (m->filter_base + 3) & 31;
    }
}
