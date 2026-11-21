// SPDX-License-Identifier: BSD-3-Clause
// AY compatibility mode adapted from MAME, copyright Couriersud. See NOTICE.
#include "machine.h"

void psg_write(psg_t *p, unsigned reg, uint8_t data)
{
    static const uint8_t mask[] = {255, 15, 255, 15,  255, 15, 31,  255,
                                   31,  31, 31,  255, 255, 15, 255, 255};
    p->reg[reg] = data & mask[reg];
    if (reg == 13) {
        p->attack = (data & 4) ? 15 : 0;
        p->hold = (data & 8) ? data & 1 : 1;
        p->alternate = (data & 8) ? (data >> 1) & 1 : !!p->attack;
        p->holding = 0;
        p->step = 15;
    }
}

void psg_tick(psg_t *p)
{
    static const int16_t level[2][16] = {
        {-4096, -3873, -3806, -3718, -3581, -3382, -3138, -2536, -2172, -944, 267, 1698, 3646, 5712,
         8888, 12288},
        {-4096, -3948, -3881, -3792, -3654, -3454, -3209, -2605, -2239, -1005, 212, 1650, 3606,
         5682, 8872, 12288},
    };
    int volume[3];
    if (++p->noise_count >= p->reg[6]) {
        p->noise_count = 0;
        p->noise_phase ^= 1;
        if (!p->noise_phase)
            p->rng = (p->rng >> 1) | (((p->rng ^ (p->rng >> 3)) & 1) << 16);
    }
    unsigned period = 2u * (p->reg[11] + 256u * p->reg[12]);
    if (!p->holding && ++p->env_count >= period) {
        p->env_count = 0;
        if (!p->step) {
            if (p->alternate)
                p->attack ^= 15;
            p->holding = p->hold;
            p->step = p->hold ? 0 : 15;
        } else
            --p->step;
    }
    for (unsigned c = 0; c < 3; ++c) {
        unsigned freq = p->reg[c * 2] + p->reg[c * 2 + 1] * 256;
        if (!freq)
            freq = 1;
        if (++p->count[c] >= freq) {
            p->tone[c] ^= (p->count[c] / freq) & 1;
            p->count[c] %= freq;
        }
        unsigned mixer = p->reg[7] >> c;
        unsigned gate = (p->tone[c] | mixer) & (p->rng | (mixer >> 3)) & 1;
        unsigned env = p->reg[c + 8] >> 4;
        unsigned amp = env ? p->step ^ p->attack : p->reg[c + 8] & 15;
        volume[c] = level[env][gate ? amp : 0];
    }
    p->output[0] = (int16_t)((2 * volume[0] + volume[2]) / 4);
    p->output[1] = (int16_t)((2 * volume[1] + volume[2]) / 4);
}
