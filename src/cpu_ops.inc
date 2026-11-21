// Generated from MAME, copyright Olivier Galibert. SPDX-License-Identifier: BSD-3-Clause

enum { CPU_RESET=1253, CPU_MICRO_COUNT=1262 };
static const uint16_t decode[256]={
    0, 11, 18, 24, 33, 37, 41, 47, 53, 57, 60, 63, 66, 71, 76, 83,
    90, 96, 18, 103, 112, 117, 122, 129, 136, 139, 145, 148, 156, 162, 168, 176,
    184, 191, 18, 198, 207, 211, 215, 221, 227, 232, 235, 63, 238, 243, 248, 255,
    262, 268, 18, 275, 112, 284, 289, 296, 303, 306, 145, 312, 156, 320, 326, 334,
    342, 349, 18, 356, 33, 365, 369, 375, 381, 385, 388, 391, 394, 398, 403, 410,
    417, 423, 18, 430, 112, 439, 444, 451, 458, 461, 145, 467, 156, 475, 481, 489,
    497, 504, 18, 511, 33, 520, 524, 530, 536, 541, 544, 547, 550, 556, 561, 568,
    575, 581, 18, 588, 112, 597, 602, 609, 616, 619, 145, 625, 156, 633, 639, 647,
    655, 658, 655, 665, 672, 676, 680, 684, 688, 655, 691, 694, 697, 702, 707, 712,
    717, 723, 18, 730, 737, 742, 747, 752, 757, 760, 766, 769, 775, 781, 787, 793,
    799, 802, 809, 812, 819, 823, 827, 831, 835, 838, 841, 844, 847, 852, 857, 862,
    867, 873, 18, 880, 887, 892, 897, 902, 907, 910, 916, 919, 925, 931, 937, 943,
    949, 952, 655, 959, 968, 972, 976, 982, 988, 991, 994, 997, 1000, 1005, 1010, 1017,
    1024, 1030, 18, 1037, 112, 1046, 1051, 1058, 1065, 1068, 145, 1074, 156, 1082, 1088, 1096,
    1104, 1107, 655, 1114, 1123, 1127, 1131, 1137, 1143, 1146, 145, 1146, 1149, 1154, 1159, 1166,
    1173, 1179, 18, 1186, 112, 1195, 1200, 1207, 1214, 1217, 145, 1223, 156, 1231, 1237, 1245
};
// One invocation performs precisely one memory access.
static void cpu_tick(machine_t *m, bool irq)
{
    cpu_t *c=&m->cpu;
    for (;;) {
        switch (c->micro) {
        case 0: /* brk_imp */
            if(c->irq_taken) {
            c->irq_sampled=irq;
                read_bus(m,c->PC);
            c->micro=1; return;
        case 1:;
            } else {
            c->irq_sampled=irq;
                read_bus(m,c->PC);
            c->micro=2; return;
        case 2:;
                c->PC++;
            }
            c->irq_sampled=irq;
            write_bus(m,c->SP, c->PC >> 8);
            c->micro=3; return;
        case 3:;
            c->SP=set_l(c->SP,c->SP-1);
            c->irq_sampled=irq;
            write_bus(m,c->SP, c->PC);
            c->micro=4; return;
        case 4:;
            c->SP=set_l(c->SP,c->SP-1);
            c->TMP = c->nmi_pending;
            c->irq_sampled=irq;
            write_bus(m,c->SP, c->irq_taken ? c->P & ~F_B : c->P);
            c->micro=5; return;
        case 5:;
            c->SP=set_l(c->SP,c->SP-1);
            if(c->TMP) {
                    (void)0;
                c->PC = read_bus(m,0xfffa);
            c->micro=6; return;
        case 6:;
                c->PC = set_h(c->PC, read_bus(m,0xfffb));
            c->micro=7; return;
        case 7:;
                c->nmi_pending = false;
            } else {
                if(c->irq_taken)
                    (void)0;
                c->PC = read_bus(m,0xfffe);
            c->micro=8; return;
        case 8:;
                c->PC = set_h(c->PC, read_bus(m,0xffff));
            c->micro=9; return;
        case 9:;
            }
            c->irq_taken = false;
            c->P |= F_I; // Do *not* move after the prefetch
            c->IR=read_bus(m,c->PC);
            c->micro=10; return;
        case 10:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 11: /* ora_idx */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=12; return;
        case 12:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP2);
            c->micro=13; return;
        case 13:;
            c->TMP2 += c->X;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2 & 0xff);
            c->micro=14; return;
        case 14:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=15; return;
        case 15:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=16; return;
        case 16:;
            c->A |= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=17; return;
        case 17:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 18: /* kil_non */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=19; return;
        case 19:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,0xffff);
            c->micro=20; return;
        case 20:;
            c->irq_sampled=irq;
            read_bus(m,0xfffe);
            c->micro=21; return;
        case 21:;
            c->irq_sampled=irq;
            read_bus(m,0xfffe);
            c->micro=22; return;
        case 22:;
            for(;;) {
            c->irq_sampled=irq;
                read_bus(m,0xffff);
            c->micro=23; return;
        case 23:;
            }
            break;
        case 24: /* slo_idx */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=25; return;
        case 25:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP2);
            c->micro=26; return;
        case 26:;
            c->TMP2 += c->X;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2 & 0xff);
            c->micro=27; return;
        case 27:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=28; return;
        case 28:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=29; return;
        case 29:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=30; return;
        case 30:;
            c->TMP2 = do_asl(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=31; return;
        case 31:;
            c->A |= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=32; return;
        case 32:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 33: /* nop_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=34; return;
        case 34:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=35; return;
        case 35:;
            c->IR=read_bus(m,c->PC);
            c->micro=36; return;
        case 36:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 37: /* ora_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=38; return;
        case 38:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=39; return;
        case 39:;
            c->A |= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=40; return;
        case 40:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 41: /* asl_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=42; return;
        case 42:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=43; return;
        case 43:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=44; return;
        case 44:;
            c->TMP2 = do_asl(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=45; return;
        case 45:;
            c->IR=read_bus(m,c->PC);
            c->micro=46; return;
        case 46:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 47: /* slo_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=48; return;
        case 48:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=49; return;
        case 49:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=50; return;
        case 50:;
            c->TMP2 = do_asl(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=51; return;
        case 51:;
            c->A |= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=52; return;
        case 52:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 53: /* php_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=54; return;
        case 54:;
            c->irq_sampled=irq;
            write_bus(m,c->SP, c->P);
            c->micro=55; return;
        case 55:;
            c->SP=set_l(c->SP,c->SP-1);
            c->IR=read_bus(m,c->PC);
            c->micro=56; return;
        case 56:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 57: /* ora_imm */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=58; return;
        case 58:;
            c->PC++;
            c->A |= c->TMP;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=59; return;
        case 59:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 60: /* asl_acc */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=61; return;
        case 61:;
            c->A = do_asl(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=62; return;
        case 62:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 63: /* anc_imm */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=64; return;
        case 64:;
            c->PC++;
            c->A &= c->TMP;
            set_nz(c, c->A);
            if(c->A & 0x80)
                c->P |= F_C;
            else
                c->P &= ~F_C;
            c->IR=read_bus(m,c->PC);
            c->micro=65; return;
        case 65:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 66: /* nop_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=67; return;
        case 67:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=68; return;
        case 68:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=69; return;
        case 69:;
            c->IR=read_bus(m,c->PC);
            c->micro=70; return;
        case 70:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 71: /* ora_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=72; return;
        case 72:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=73; return;
        case 73:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=74; return;
        case 74:;
            c->A |= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=75; return;
        case 75:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 76: /* asl_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=77; return;
        case 77:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=78; return;
        case 78:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=79; return;
        case 79:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=80; return;
        case 80:;
            c->TMP2 = do_asl(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=81; return;
        case 81:;
            c->IR=read_bus(m,c->PC);
            c->micro=82; return;
        case 82:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 83: /* slo_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=84; return;
        case 84:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=85; return;
        case 85:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=86; return;
        case 86:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=87; return;
        case 87:;
            c->TMP2 = do_asl(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=88; return;
        case 88:;
            c->A |= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=89; return;
        case 89:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 90: /* bpl_rel */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=91; return;
        case 91:;
            c->PC++;
            if(!(c->P & F_N)) {
                if(page_changing(c->PC, (int8_t)(c->TMP))) {
            c->irq_sampled=irq;
                    read_bus(m,c->PC);
            c->micro=92; return;
        case 92:;
            c->irq_sampled=irq;
                    read_bus(m,set_l(c->PC, c->PC+(int8_t)(c->TMP)));
            c->micro=93; return;
        case 93:;
                } else {
                    read_bus(m,c->PC);
            c->micro=94; return;
        case 94:;
                }
                c->PC += (int8_t)(c->TMP);
            }
            c->IR=read_bus(m,c->PC);
            c->micro=95; return;
        case 95:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 96: /* ora_idy */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=97; return;
        case 97:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2);
            c->micro=98; return;
        case 98:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=99; return;
        case 99:;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=100; return;
        case 100:;
            }
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP+c->Y);
            c->micro=101; return;
        case 101:;
            c->A |= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=102; return;
        case 102:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 103: /* slo_idy */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=104; return;
        case 104:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2);
            c->micro=105; return;
        case 105:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=106; return;
        case 106:;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=107; return;
        case 107:;
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=108; return;
        case 108:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=109; return;
        case 109:;
            c->TMP2 = do_asl(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=110; return;
        case 110:;
            c->A |= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=111; return;
        case 111:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 112: /* nop_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=113; return;
        case 113:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=114; return;
        case 114:;
            c->irq_sampled=irq;
            read_bus(m,(uint8_t)(c->TMP+c->X));
            c->micro=115; return;
        case 115:;
            c->IR=read_bus(m,c->PC);
            c->micro=116; return;
        case 116:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 117: /* ora_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=118; return;
        case 118:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=119; return;
        case 119:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,(uint8_t)(c->TMP+c->X));
            c->micro=120; return;
        case 120:;
            c->A |= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=121; return;
        case 121:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 122: /* asl_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=123; return;
        case 123:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=124; return;
        case 124:;
            c->TMP = (uint8_t)(c->TMP+c->X);
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=125; return;
        case 125:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=126; return;
        case 126:;
            c->TMP2 = do_asl(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=127; return;
        case 127:;
            c->IR=read_bus(m,c->PC);
            c->micro=128; return;
        case 128:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 129: /* slo_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=130; return;
        case 130:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=131; return;
        case 131:;
            c->TMP = (uint8_t)(c->TMP+c->X);
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=132; return;
        case 132:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=133; return;
        case 133:;
            c->TMP2 = do_asl(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=134; return;
        case 134:;
            c->A |= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=135; return;
        case 135:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 136: /* clc_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=137; return;
        case 137:;
            c->P &= ~F_C;
            c->IR=read_bus(m,c->PC);
            c->micro=138; return;
        case 138:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 139: /* ora_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=140; return;
        case 140:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=141; return;
        case 141:;
            c->PC++;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=142; return;
        case 142:;
            }
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=143; return;
        case 143:;
            c->A |= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=144; return;
        case 144:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 145: /* nop_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=146; return;
        case 146:;
            c->IR=read_bus(m,c->PC);
            c->micro=147; return;
        case 147:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 148: /* slo_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=149; return;
        case 149:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=150; return;
        case 150:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=151; return;
        case 151:;
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=152; return;
        case 152:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=153; return;
        case 153:;
            c->TMP2 = do_asl(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=154; return;
        case 154:;
            c->A |= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=155; return;
        case 155:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 156: /* nop_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=157; return;
        case 157:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=158; return;
        case 158:;
            c->PC++;
            if(page_changing(c->TMP, c->X)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=159; return;
        case 159:;
            }
            c->irq_sampled=irq;
            read_bus(m,c->TMP + c->X);
            c->micro=160; return;
        case 160:;
            c->IR=read_bus(m,c->PC);
            c->micro=161; return;
        case 161:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 162: /* ora_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=163; return;
        case 163:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=164; return;
        case 164:;
            c->PC++;
            if(page_changing(c->TMP, c->X)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=165; return;
        case 165:;
            }
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=166; return;
        case 166:;
            c->A |= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=167; return;
        case 167:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 168: /* asl_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=169; return;
        case 169:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=170; return;
        case 170:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=171; return;
        case 171:;
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=172; return;
        case 172:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=173; return;
        case 173:;
            c->TMP2 = do_asl(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=174; return;
        case 174:;
            c->IR=read_bus(m,c->PC);
            c->micro=175; return;
        case 175:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 176: /* slo_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=177; return;
        case 177:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=178; return;
        case 178:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=179; return;
        case 179:;
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=180; return;
        case 180:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=181; return;
        case 181:;
            c->TMP2 = do_asl(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=182; return;
        case 182:;
            c->A |= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=183; return;
        case 183:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 184: /* jsr_adr */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=185; return;
        case 185:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->SP);
            c->micro=186; return;
        case 186:;
            c->irq_sampled=irq;
            write_bus(m,c->SP, c->PC>>8);
            c->micro=187; return;
        case 187:;
            c->SP=set_l(c->SP,c->SP-1);
            c->irq_sampled=irq;
            write_bus(m,c->SP, c->PC);
            c->micro=188; return;
        case 188:;
            c->SP=set_l(c->SP,c->SP-1);
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=189; return;
        case 189:;
            c->PC = c->TMP;
            c->IR=read_bus(m,c->PC);
            c->micro=190; return;
        case 190:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 191: /* and_idx */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=192; return;
        case 192:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP2);
            c->micro=193; return;
        case 193:;
            c->TMP2 += c->X;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2 & 0xff);
            c->micro=194; return;
        case 194:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=195; return;
        case 195:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=196; return;
        case 196:;
            c->A &= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=197; return;
        case 197:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 198: /* rla_idx */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=199; return;
        case 199:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP2);
            c->micro=200; return;
        case 200:;
            c->TMP2 += c->X;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2 & 0xff);
            c->micro=201; return;
        case 201:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=202; return;
        case 202:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=203; return;
        case 203:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=204; return;
        case 204:;
            c->TMP2 = do_rol(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=205; return;
        case 205:;
            c->A &= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=206; return;
        case 206:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 207: /* bit_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=208; return;
        case 208:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=209; return;
        case 209:;
            do_bit(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=210; return;
        case 210:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 211: /* and_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=212; return;
        case 212:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=213; return;
        case 213:;
            c->A &= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=214; return;
        case 214:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 215: /* rol_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=216; return;
        case 216:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=217; return;
        case 217:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=218; return;
        case 218:;
            c->TMP2 = do_rol(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=219; return;
        case 219:;
            c->IR=read_bus(m,c->PC);
            c->micro=220; return;
        case 220:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 221: /* rla_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=222; return;
        case 222:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=223; return;
        case 223:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=224; return;
        case 224:;
            c->TMP2 = do_rol(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=225; return;
        case 225:;
            c->A &= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=226; return;
        case 226:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 227: /* plp_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=228; return;
        case 228:;
            c->irq_sampled=irq;
            read_bus(m,c->SP);
            c->micro=229; return;
        case 229:;
            c->SP=set_l(c->SP,c->SP+1);
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->SP) | (F_B|F_E);
            c->micro=230; return;
        case 230:;
            c->IR=read_bus(m,c->PC);
            c->micro=231; return;
        case 231:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            c->P = c->TMP; // Do *not* move it before the prefetch
            break;
        case 232: /* and_imm */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=233; return;
        case 233:;
            c->PC++;
            c->A &= c->TMP;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=234; return;
        case 234:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 235: /* rol_acc */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=236; return;
        case 236:;
            c->A = do_rol(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=237; return;
        case 237:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 238: /* bit_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=239; return;
        case 239:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=240; return;
        case 240:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=241; return;
        case 241:;
            do_bit(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=242; return;
        case 242:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 243: /* and_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=244; return;
        case 244:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=245; return;
        case 245:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=246; return;
        case 246:;
            c->A &= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=247; return;
        case 247:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 248: /* rol_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=249; return;
        case 249:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=250; return;
        case 250:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=251; return;
        case 251:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=252; return;
        case 252:;
            c->TMP2 = do_rol(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=253; return;
        case 253:;
            c->IR=read_bus(m,c->PC);
            c->micro=254; return;
        case 254:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 255: /* rla_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=256; return;
        case 256:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=257; return;
        case 257:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=258; return;
        case 258:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=259; return;
        case 259:;
            c->TMP2 = do_rol(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=260; return;
        case 260:;
            c->A &= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=261; return;
        case 261:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 262: /* bmi_rel */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=263; return;
        case 263:;
            c->PC++;
            if(c->P & F_N) {
                if(page_changing(c->PC, (int8_t)(c->TMP))) {
            c->irq_sampled=irq;
                    read_bus(m,c->PC);
            c->micro=264; return;
        case 264:;
            c->irq_sampled=irq;
                    read_bus(m,set_l(c->PC, c->PC+(int8_t)(c->TMP)));
            c->micro=265; return;
        case 265:;
                } else {
                    read_bus(m,c->PC);
            c->micro=266; return;
        case 266:;
                }
                c->PC += (int8_t)(c->TMP);
            }
            c->IR=read_bus(m,c->PC);
            c->micro=267; return;
        case 267:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 268: /* and_idy */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=269; return;
        case 269:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2);
            c->micro=270; return;
        case 270:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=271; return;
        case 271:;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=272; return;
        case 272:;
            }
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP+c->Y);
            c->micro=273; return;
        case 273:;
            c->A &= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=274; return;
        case 274:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 275: /* rla_idy */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=276; return;
        case 276:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2);
            c->micro=277; return;
        case 277:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=278; return;
        case 278:;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=279; return;
        case 279:;
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=280; return;
        case 280:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=281; return;
        case 281:;
            c->TMP2 = do_rol(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=282; return;
        case 282:;
            c->A &= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=283; return;
        case 283:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 284: /* and_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=285; return;
        case 285:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=286; return;
        case 286:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,(uint8_t)(c->TMP+c->X));
            c->micro=287; return;
        case 287:;
            c->A &= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=288; return;
        case 288:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 289: /* rol_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=290; return;
        case 290:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=291; return;
        case 291:;
            c->TMP = (uint8_t)(c->TMP+c->X);
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=292; return;
        case 292:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=293; return;
        case 293:;
            c->TMP2 = do_rol(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=294; return;
        case 294:;
            c->IR=read_bus(m,c->PC);
            c->micro=295; return;
        case 295:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 296: /* rla_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=297; return;
        case 297:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=298; return;
        case 298:;
            c->TMP = (uint8_t)(c->TMP+c->X);
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=299; return;
        case 299:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=300; return;
        case 300:;
            c->TMP2 = do_rol(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=301; return;
        case 301:;
            c->A &= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=302; return;
        case 302:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 303: /* sec_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=304; return;
        case 304:;
            c->P |= F_C;
            c->IR=read_bus(m,c->PC);
            c->micro=305; return;
        case 305:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 306: /* and_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=307; return;
        case 307:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=308; return;
        case 308:;
            c->PC++;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=309; return;
        case 309:;
            }
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=310; return;
        case 310:;
            c->A &= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=311; return;
        case 311:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 312: /* rla_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=313; return;
        case 313:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=314; return;
        case 314:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=315; return;
        case 315:;
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=316; return;
        case 316:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=317; return;
        case 317:;
            c->TMP2 = do_rol(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=318; return;
        case 318:;
            c->A &= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=319; return;
        case 319:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 320: /* and_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=321; return;
        case 321:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=322; return;
        case 322:;
            c->PC++;
            if(page_changing(c->TMP, c->X)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=323; return;
        case 323:;
            }
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=324; return;
        case 324:;
            c->A &= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=325; return;
        case 325:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 326: /* rol_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=327; return;
        case 327:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=328; return;
        case 328:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=329; return;
        case 329:;
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=330; return;
        case 330:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=331; return;
        case 331:;
            c->TMP2 = do_rol(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=332; return;
        case 332:;
            c->IR=read_bus(m,c->PC);
            c->micro=333; return;
        case 333:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 334: /* rla_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=335; return;
        case 335:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=336; return;
        case 336:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=337; return;
        case 337:;
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=338; return;
        case 338:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=339; return;
        case 339:;
            c->TMP2 = do_rol(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=340; return;
        case 340:;
            c->A &= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=341; return;
        case 341:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 342: /* rti_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=343; return;
        case 343:;
            c->irq_sampled=irq;
            read_bus(m,c->SP);
            c->micro=344; return;
        case 344:;
            c->SP=set_l(c->SP,c->SP+1);
            c->irq_sampled=irq;
            c->P = read_bus(m,c->SP) | (F_B|F_E);
            c->micro=345; return;
        case 345:;
            c->SP=set_l(c->SP,c->SP+1);
            c->irq_sampled=irq;
            c->PC = read_bus(m,c->SP);
            c->micro=346; return;
        case 346:;
            c->SP=set_l(c->SP,c->SP+1);
            c->irq_sampled=irq;
            c->PC = set_h(c->PC, read_bus(m,c->SP));
            c->micro=347; return;
        case 347:;
            c->IR=read_bus(m,c->PC);
            c->micro=348; return;
        case 348:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 349: /* eor_idx */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=350; return;
        case 350:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP2);
            c->micro=351; return;
        case 351:;
            c->TMP2 += c->X;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2 & 0xff);
            c->micro=352; return;
        case 352:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=353; return;
        case 353:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=354; return;
        case 354:;
            c->A ^= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=355; return;
        case 355:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 356: /* sre_idx */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=357; return;
        case 357:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP2);
            c->micro=358; return;
        case 358:;
            c->TMP2 += c->X;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2 & 0xff);
            c->micro=359; return;
        case 359:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=360; return;
        case 360:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=361; return;
        case 361:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=362; return;
        case 362:;
            c->TMP2 = do_lsr(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=363; return;
        case 363:;
            c->A ^= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=364; return;
        case 364:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 365: /* eor_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=366; return;
        case 366:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=367; return;
        case 367:;
            c->A ^= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=368; return;
        case 368:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 369: /* lsr_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=370; return;
        case 370:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=371; return;
        case 371:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=372; return;
        case 372:;
            c->TMP2 = do_lsr(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=373; return;
        case 373:;
            c->IR=read_bus(m,c->PC);
            c->micro=374; return;
        case 374:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 375: /* sre_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=376; return;
        case 376:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=377; return;
        case 377:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=378; return;
        case 378:;
            c->TMP2 = do_lsr(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=379; return;
        case 379:;
            c->A ^= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=380; return;
        case 380:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 381: /* pha_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=382; return;
        case 382:;
            c->irq_sampled=irq;
            write_bus(m,c->SP, c->A);
            c->micro=383; return;
        case 383:;
            c->SP=set_l(c->SP,c->SP-1);
            c->IR=read_bus(m,c->PC);
            c->micro=384; return;
        case 384:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 385: /* eor_imm */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=386; return;
        case 386:;
            c->PC++;
            c->A ^= c->TMP;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=387; return;
        case 387:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 388: /* lsr_acc */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=389; return;
        case 389:;
            c->A = do_lsr(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=390; return;
        case 390:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 391: /* asr_imm */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=392; return;
        case 392:;
            c->PC++;
            c->A = do_lsr(c, c->A & c->TMP);
            c->IR=read_bus(m,c->PC);
            c->micro=393; return;
        case 393:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 394: /* jmp_adr */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=395; return;
        case 395:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=396; return;
        case 396:;
            c->PC = c->TMP;
            c->IR=read_bus(m,c->PC);
            c->micro=397; return;
        case 397:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 398: /* eor_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=399; return;
        case 399:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=400; return;
        case 400:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=401; return;
        case 401:;
            c->A ^= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=402; return;
        case 402:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 403: /* lsr_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=404; return;
        case 404:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=405; return;
        case 405:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=406; return;
        case 406:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=407; return;
        case 407:;
            c->TMP2 = do_lsr(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=408; return;
        case 408:;
            c->IR=read_bus(m,c->PC);
            c->micro=409; return;
        case 409:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 410: /* sre_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=411; return;
        case 411:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=412; return;
        case 412:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=413; return;
        case 413:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=414; return;
        case 414:;
            c->TMP2 = do_lsr(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=415; return;
        case 415:;
            c->A ^= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=416; return;
        case 416:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 417: /* bvc_rel */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=418; return;
        case 418:;
            c->PC++;
            if(!(c->P & F_V)) {
                if(page_changing(c->PC, (int8_t)(c->TMP))) {
            c->irq_sampled=irq;
                    read_bus(m,c->PC);
            c->micro=419; return;
        case 419:;
            c->irq_sampled=irq;
                    read_bus(m,set_l(c->PC, c->PC+(int8_t)(c->TMP)));
            c->micro=420; return;
        case 420:;
                } else {
                    read_bus(m,c->PC);
            c->micro=421; return;
        case 421:;
                }
                c->PC += (int8_t)(c->TMP);
            }
            c->IR=read_bus(m,c->PC);
            c->micro=422; return;
        case 422:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 423: /* eor_idy */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=424; return;
        case 424:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2);
            c->micro=425; return;
        case 425:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=426; return;
        case 426:;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=427; return;
        case 427:;
            }
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP+c->Y);
            c->micro=428; return;
        case 428:;
            c->A ^= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=429; return;
        case 429:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 430: /* sre_idy */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=431; return;
        case 431:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2);
            c->micro=432; return;
        case 432:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=433; return;
        case 433:;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=434; return;
        case 434:;
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=435; return;
        case 435:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=436; return;
        case 436:;
            c->TMP2 = do_lsr(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=437; return;
        case 437:;
            c->A ^= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=438; return;
        case 438:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 439: /* eor_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=440; return;
        case 440:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=441; return;
        case 441:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,(uint8_t)(c->TMP+c->X));
            c->micro=442; return;
        case 442:;
            c->A ^= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=443; return;
        case 443:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 444: /* lsr_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=445; return;
        case 445:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=446; return;
        case 446:;
            c->TMP = (uint8_t)(c->TMP+c->X);
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=447; return;
        case 447:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=448; return;
        case 448:;
            c->TMP2 = do_lsr(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=449; return;
        case 449:;
            c->IR=read_bus(m,c->PC);
            c->micro=450; return;
        case 450:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 451: /* sre_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=452; return;
        case 452:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=453; return;
        case 453:;
            c->TMP = (uint8_t)(c->TMP+c->X);
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=454; return;
        case 454:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=455; return;
        case 455:;
            c->TMP2 = do_lsr(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=456; return;
        case 456:;
            c->A ^= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=457; return;
        case 457:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 458: /* cli_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=459; return;
        case 459:;
            c->IR=read_bus(m,c->PC);
            c->micro=460; return;
        case 460:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            c->P &= ~F_I; // Do *not* move it before the prefetch
            break;
        case 461: /* eor_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=462; return;
        case 462:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=463; return;
        case 463:;
            c->PC++;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=464; return;
        case 464:;
            }
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=465; return;
        case 465:;
            c->A ^= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=466; return;
        case 466:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 467: /* sre_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=468; return;
        case 468:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=469; return;
        case 469:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=470; return;
        case 470:;
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=471; return;
        case 471:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=472; return;
        case 472:;
            c->TMP2 = do_lsr(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=473; return;
        case 473:;
            c->A ^= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=474; return;
        case 474:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 475: /* eor_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=476; return;
        case 476:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=477; return;
        case 477:;
            c->PC++;
            if(page_changing(c->TMP, c->X)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=478; return;
        case 478:;
            }
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=479; return;
        case 479:;
            c->A ^= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=480; return;
        case 480:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 481: /* lsr_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=482; return;
        case 482:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=483; return;
        case 483:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=484; return;
        case 484:;
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=485; return;
        case 485:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=486; return;
        case 486:;
            c->TMP2 = do_lsr(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=487; return;
        case 487:;
            c->IR=read_bus(m,c->PC);
            c->micro=488; return;
        case 488:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 489: /* sre_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=490; return;
        case 490:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=491; return;
        case 491:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=492; return;
        case 492:;
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=493; return;
        case 493:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=494; return;
        case 494:;
            c->TMP2 = do_lsr(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=495; return;
        case 495:;
            c->A ^= c->TMP2;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=496; return;
        case 496:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 497: /* rts_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=498; return;
        case 498:;
            c->irq_sampled=irq;
            read_bus(m,c->SP);
            c->micro=499; return;
        case 499:;
            c->SP=set_l(c->SP,c->SP+1);
            c->irq_sampled=irq;
            c->PC = read_bus(m,c->SP);
            c->micro=500; return;
        case 500:;
            c->SP=set_l(c->SP,c->SP+1);
            c->irq_sampled=irq;
            c->PC = set_h(c->PC, read_bus(m,c->SP));
            c->micro=501; return;
        case 501:;
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=502; return;
        case 502:;
            c->PC++;
            c->IR=read_bus(m,c->PC);
            c->micro=503; return;
        case 503:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 504: /* adc_idx */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=505; return;
        case 505:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP2);
            c->micro=506; return;
        case 506:;
            c->TMP2 += c->X;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2 & 0xff);
            c->micro=507; return;
        case 507:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=508; return;
        case 508:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=509; return;
        case 509:;
            do_adc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=510; return;
        case 510:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 511: /* rra_idx */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=512; return;
        case 512:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP2);
            c->micro=513; return;
        case 513:;
            c->TMP2 += c->X;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2 & 0xff);
            c->micro=514; return;
        case 514:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=515; return;
        case 515:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=516; return;
        case 516:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=517; return;
        case 517:;
            c->TMP2 = do_ror(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=518; return;
        case 518:;
            do_adc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=519; return;
        case 519:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 520: /* adc_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=521; return;
        case 521:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=522; return;
        case 522:;
            do_adc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=523; return;
        case 523:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 524: /* ror_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=525; return;
        case 525:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=526; return;
        case 526:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=527; return;
        case 527:;
            c->TMP2 = do_ror(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=528; return;
        case 528:;
            c->IR=read_bus(m,c->PC);
            c->micro=529; return;
        case 529:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 530: /* rra_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=531; return;
        case 531:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=532; return;
        case 532:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=533; return;
        case 533:;
            c->TMP2 = do_ror(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=534; return;
        case 534:;
            do_adc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=535; return;
        case 535:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 536: /* pla_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=537; return;
        case 537:;
            c->irq_sampled=irq;
            read_bus(m,c->SP);
            c->micro=538; return;
        case 538:;
            c->SP=set_l(c->SP,c->SP+1);
            c->irq_sampled=irq;
            c->A = read_bus(m,c->SP);
            c->micro=539; return;
        case 539:;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=540; return;
        case 540:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 541: /* adc_imm */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=542; return;
        case 542:;
            c->PC++;
            do_adc(c, c->TMP);
            c->IR=read_bus(m,c->PC);
            c->micro=543; return;
        case 543:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 544: /* ror_acc */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=545; return;
        case 545:;
            c->A = do_ror(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=546; return;
        case 546:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 547: /* arr_imm */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=548; return;
        case 548:;
            c->PC++;
            c->A &= c->TMP;
            do_arr(c);
            c->IR=read_bus(m,c->PC);
            c->micro=549; return;
        case 549:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 550: /* jmp_ind */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=551; return;
        case 551:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=552; return;
        case 552:;
            c->PC++;
            c->irq_sampled=irq;
            c->PC = read_bus(m,c->TMP);
            c->micro=553; return;
        case 553:;
            c->irq_sampled=irq;
            c->PC = set_h(c->PC, read_bus(m,set_l(c->TMP, c->TMP+1)));
            c->micro=554; return;
        case 554:;
            c->IR=read_bus(m,c->PC);
            c->micro=555; return;
        case 555:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 556: /* adc_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=557; return;
        case 557:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=558; return;
        case 558:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=559; return;
        case 559:;
            do_adc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=560; return;
        case 560:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 561: /* ror_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=562; return;
        case 562:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=563; return;
        case 563:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=564; return;
        case 564:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=565; return;
        case 565:;
            c->TMP2 = do_ror(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=566; return;
        case 566:;
            c->IR=read_bus(m,c->PC);
            c->micro=567; return;
        case 567:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 568: /* rra_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=569; return;
        case 569:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=570; return;
        case 570:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=571; return;
        case 571:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=572; return;
        case 572:;
            c->TMP2 = do_ror(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=573; return;
        case 573:;
            do_adc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=574; return;
        case 574:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 575: /* bvs_rel */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=576; return;
        case 576:;
            c->PC++;
            if(c->P & F_V) {
                if(page_changing(c->PC, (int8_t)(c->TMP))) {
            c->irq_sampled=irq;
                    read_bus(m,c->PC);
            c->micro=577; return;
        case 577:;
            c->irq_sampled=irq;
                    read_bus(m,set_l(c->PC, c->PC+(int8_t)(c->TMP)));
            c->micro=578; return;
        case 578:;
                } else {
                    read_bus(m,c->PC);
            c->micro=579; return;
        case 579:;
                }
                c->PC += (int8_t)(c->TMP);
            }
            c->IR=read_bus(m,c->PC);
            c->micro=580; return;
        case 580:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 581: /* adc_idy */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=582; return;
        case 582:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2);
            c->micro=583; return;
        case 583:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=584; return;
        case 584:;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=585; return;
        case 585:;
            }
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP+c->Y);
            c->micro=586; return;
        case 586:;
            do_adc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=587; return;
        case 587:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 588: /* rra_idy */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=589; return;
        case 589:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2);
            c->micro=590; return;
        case 590:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=591; return;
        case 591:;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=592; return;
        case 592:;
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=593; return;
        case 593:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=594; return;
        case 594:;
            c->TMP2 = do_ror(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=595; return;
        case 595:;
            do_adc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=596; return;
        case 596:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 597: /* adc_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=598; return;
        case 598:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=599; return;
        case 599:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,(uint8_t)(c->TMP+c->X));
            c->micro=600; return;
        case 600:;
            do_adc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=601; return;
        case 601:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 602: /* ror_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=603; return;
        case 603:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=604; return;
        case 604:;
            c->TMP = (uint8_t)(c->TMP+c->X);
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=605; return;
        case 605:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=606; return;
        case 606:;
            c->TMP2 = do_ror(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=607; return;
        case 607:;
            c->IR=read_bus(m,c->PC);
            c->micro=608; return;
        case 608:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 609: /* rra_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=610; return;
        case 610:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=611; return;
        case 611:;
            c->TMP = (uint8_t)(c->TMP+c->X);
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=612; return;
        case 612:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=613; return;
        case 613:;
            c->TMP2 = do_ror(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=614; return;
        case 614:;
            do_adc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=615; return;
        case 615:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 616: /* sei_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=617; return;
        case 617:;
            c->IR=read_bus(m,c->PC);
            c->micro=618; return;
        case 618:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            c->P |= F_I; // Do *not* move it before the prefetch
            break;
        case 619: /* adc_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=620; return;
        case 620:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=621; return;
        case 621:;
            c->PC++;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=622; return;
        case 622:;
            }
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=623; return;
        case 623:;
            do_adc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=624; return;
        case 624:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 625: /* rra_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=626; return;
        case 626:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=627; return;
        case 627:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=628; return;
        case 628:;
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=629; return;
        case 629:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=630; return;
        case 630:;
            c->TMP2 = do_ror(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=631; return;
        case 631:;
            do_adc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=632; return;
        case 632:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 633: /* adc_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=634; return;
        case 634:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=635; return;
        case 635:;
            c->PC++;
            if(page_changing(c->TMP, c->X)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=636; return;
        case 636:;
            }
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=637; return;
        case 637:;
            do_adc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=638; return;
        case 638:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 639: /* ror_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=640; return;
        case 640:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=641; return;
        case 641:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=642; return;
        case 642:;
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=643; return;
        case 643:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=644; return;
        case 644:;
            c->TMP2 = do_ror(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=645; return;
        case 645:;
            c->IR=read_bus(m,c->PC);
            c->micro=646; return;
        case 646:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 647: /* rra_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=648; return;
        case 648:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=649; return;
        case 649:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=650; return;
        case 650:;
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=651; return;
        case 651:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=652; return;
        case 652:;
            c->TMP2 = do_ror(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=653; return;
        case 653:;
            do_adc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=654; return;
        case 654:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 655: /* nop_imm */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=656; return;
        case 656:;
            c->PC++;
            c->IR=read_bus(m,c->PC);
            c->micro=657; return;
        case 657:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 658: /* sta_idx */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=659; return;
        case 659:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP2);
            c->micro=660; return;
        case 660:;
            c->TMP2 += c->X;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2 & 0xff);
            c->micro=661; return;
        case 661:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=662; return;
        case 662:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->A);
            c->micro=663; return;
        case 663:;
            c->IR=read_bus(m,c->PC);
            c->micro=664; return;
        case 664:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 665: /* sax_idx */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=666; return;
        case 666:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP2);
            c->micro=667; return;
        case 667:;
            c->TMP2 += c->X;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2 & 0xff);
            c->micro=668; return;
        case 668:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=669; return;
        case 669:;
            c->TMP2 = c->A & c->X;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=670; return;
        case 670:;
            c->IR=read_bus(m,c->PC);
            c->micro=671; return;
        case 671:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 672: /* sty_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=673; return;
        case 673:;
            c->PC++;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->Y);
            c->micro=674; return;
        case 674:;
            c->IR=read_bus(m,c->PC);
            c->micro=675; return;
        case 675:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 676: /* sta_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=677; return;
        case 677:;
            c->PC++;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->A);
            c->micro=678; return;
        case 678:;
            c->IR=read_bus(m,c->PC);
            c->micro=679; return;
        case 679:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 680: /* stx_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=681; return;
        case 681:;
            c->PC++;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->X);
            c->micro=682; return;
        case 682:;
            c->IR=read_bus(m,c->PC);
            c->micro=683; return;
        case 683:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 684: /* sax_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=685; return;
        case 685:;
            c->PC++;
            c->TMP2 = c->A & c->X;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=686; return;
        case 686:;
            c->IR=read_bus(m,c->PC);
            c->micro=687; return;
        case 687:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 688: /* dey_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=689; return;
        case 689:;
            c->Y--;
            set_nz(c, c->Y);
            c->IR=read_bus(m,c->PC);
            c->micro=690; return;
        case 690:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 691: /* txa_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=692; return;
        case 692:;
            c->A = c->X;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=693; return;
        case 693:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 694: /* ane_imm */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=695; return;
        case 695:;
            c->PC++;
            c->A &= c->TMP & c->X;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=696; return;
        case 696:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 697: /* sty_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=698; return;
        case 698:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=699; return;
        case 699:;
            c->PC++;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->Y);
            c->micro=700; return;
        case 700:;
            c->IR=read_bus(m,c->PC);
            c->micro=701; return;
        case 701:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 702: /* sta_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=703; return;
        case 703:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=704; return;
        case 704:;
            c->PC++;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->A);
            c->micro=705; return;
        case 705:;
            c->IR=read_bus(m,c->PC);
            c->micro=706; return;
        case 706:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 707: /* stx_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=708; return;
        case 708:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=709; return;
        case 709:;
            c->PC++;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->X);
            c->micro=710; return;
        case 710:;
            c->IR=read_bus(m,c->PC);
            c->micro=711; return;
        case 711:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 712: /* sax_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=713; return;
        case 713:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=714; return;
        case 714:;
            c->PC++;
            c->TMP2 = c->A & c->X;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=715; return;
        case 715:;
            c->IR=read_bus(m,c->PC);
            c->micro=716; return;
        case 716:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 717: /* bcc_rel */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=718; return;
        case 718:;
            c->PC++;
            if(!(c->P & F_C)) {
                if(page_changing(c->PC, (int8_t)(c->TMP))) {
            c->irq_sampled=irq;
                    read_bus(m,c->PC);
            c->micro=719; return;
        case 719:;
            c->irq_sampled=irq;
                    read_bus(m,set_l(c->PC, c->PC+(int8_t)(c->TMP)));
            c->micro=720; return;
        case 720:;
                } else {
                    read_bus(m,c->PC);
            c->micro=721; return;
        case 721:;
                }
                c->PC += (int8_t)(c->TMP);
            }
            c->IR=read_bus(m,c->PC);
            c->micro=722; return;
        case 722:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 723: /* sta_idy */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=724; return;
        case 724:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2);
            c->micro=725; return;
        case 725:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=726; return;
        case 726:;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=727; return;
        case 727:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP+c->Y, c->A);
            c->micro=728; return;
        case 728:;
            c->IR=read_bus(m,c->PC);
            c->micro=729; return;
        case 729:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 730: /* sha_idy */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=731; return;
        case 731:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2);
            c->micro=732; return;
        case 732:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=733; return;
        case 733:;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=734; return;
        case 734:;
            c->TMP2 = c->A & c->X & ((c->TMP >> 8)+1);
            if(page_changing(c->TMP, c->Y))
                c->TMP = set_h(c->TMP+c->Y, c->TMP2);
            else
                c->TMP += c->Y;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=735; return;
        case 735:;
            c->IR=read_bus(m,c->PC);
            c->micro=736; return;
        case 736:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 737: /* sty_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=738; return;
        case 738:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=739; return;
        case 739:;
            c->irq_sampled=irq;
            write_bus(m,(uint8_t)(c->TMP+c->X), c->Y);
            c->micro=740; return;
        case 740:;
            c->IR=read_bus(m,c->PC);
            c->micro=741; return;
        case 741:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 742: /* sta_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=743; return;
        case 743:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=744; return;
        case 744:;
            c->irq_sampled=irq;
            write_bus(m,(uint8_t)(c->TMP+c->X), c->A);
            c->micro=745; return;
        case 745:;
            c->IR=read_bus(m,c->PC);
            c->micro=746; return;
        case 746:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 747: /* stx_zpy */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=748; return;
        case 748:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=749; return;
        case 749:;
            c->irq_sampled=irq;
            write_bus(m,(uint8_t)(c->TMP+c->Y), c->X);
            c->micro=750; return;
        case 750:;
            c->IR=read_bus(m,c->PC);
            c->micro=751; return;
        case 751:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 752: /* sax_zpy */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=753; return;
        case 753:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=754; return;
        case 754:;
            c->TMP = (uint8_t)(c->TMP+c->Y);
            c->TMP2 = c->A & c->X;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=755; return;
        case 755:;
            c->IR=read_bus(m,c->PC);
            c->micro=756; return;
        case 756:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 757: /* tya_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=758; return;
        case 758:;
            c->A = c->Y;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=759; return;
        case 759:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 760: /* sta_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=761; return;
        case 761:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=762; return;
        case 762:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=763; return;
        case 763:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP+c->Y, c->A);
            c->micro=764; return;
        case 764:;
            c->IR=read_bus(m,c->PC);
            c->micro=765; return;
        case 765:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 766: /* txs_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=767; return;
        case 767:;
            c->SP = set_l(c->SP, c->X);
            c->IR=read_bus(m,c->PC);
            c->micro=768; return;
        case 768:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 769: /* shs_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=770; return;
        case 770:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=771; return;
        case 771:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=772; return;
        case 772:;
            c->SP = set_l(c->SP, c->A & c->X);
            c->TMP2 = c->A & c->X & ((c->TMP >> 8)+1);
            if(page_changing(c->TMP, c->Y))
                c->TMP = set_h(c->TMP+c->Y, c->TMP2);
            else
                c->TMP += c->Y;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=773; return;
        case 773:;
            c->IR=read_bus(m,c->PC);
            c->micro=774; return;
        case 774:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 775: /* shy_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=776; return;
        case 776:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=777; return;
        case 777:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=778; return;
        case 778:;
            c->TMP2 = c->Y & ((c->TMP >> 8)+1);
            if(page_changing(c->TMP, c->X))
                c->TMP = set_h(c->TMP+c->X, c->TMP2);
            else
                c->TMP += c->X;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=779; return;
        case 779:;
            c->IR=read_bus(m,c->PC);
            c->micro=780; return;
        case 780:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 781: /* sta_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=782; return;
        case 782:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=783; return;
        case 783:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=784; return;
        case 784:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP+c->X, c->A);
            c->micro=785; return;
        case 785:;
            c->IR=read_bus(m,c->PC);
            c->micro=786; return;
        case 786:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 787: /* shx_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=788; return;
        case 788:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=789; return;
        case 789:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=790; return;
        case 790:;
            c->TMP2 = c->X & ((c->TMP >> 8)+1);
            if(page_changing(c->TMP, c->Y))
                c->TMP = set_h(c->TMP+c->Y, c->TMP2);
            else
                c->TMP += c->Y;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=791; return;
        case 791:;
            c->IR=read_bus(m,c->PC);
            c->micro=792; return;
        case 792:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 793: /* sha_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=794; return;
        case 794:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=795; return;
        case 795:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=796; return;
        case 796:;
            c->TMP2 = c->A & c->X & ((c->TMP >> 8)+1);
            if(page_changing(c->TMP, c->Y))
                c->TMP = set_h(c->TMP+c->Y, c->TMP2);
            else
                c->TMP += c->Y;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=797; return;
        case 797:;
            c->IR=read_bus(m,c->PC);
            c->micro=798; return;
        case 798:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 799: /* ldy_imm */
            c->irq_sampled=irq;
            c->Y = read_bus(m,c->PC);
            c->micro=800; return;
        case 800:;
            c->PC++;
            set_nz(c, c->Y);
            c->IR=read_bus(m,c->PC);
            c->micro=801; return;
        case 801:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 802: /* lda_idx */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=803; return;
        case 803:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP2);
            c->micro=804; return;
        case 804:;
            c->TMP2 += c->X;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2 & 0xff);
            c->micro=805; return;
        case 805:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=806; return;
        case 806:;
            c->irq_sampled=irq;
            c->A = read_bus(m,c->TMP);
            c->micro=807; return;
        case 807:;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=808; return;
        case 808:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 809: /* ldx_imm */
            c->irq_sampled=irq;
            c->X = read_bus(m,c->PC);
            c->micro=810; return;
        case 810:;
            c->PC++;
            set_nz(c, c->X);
            c->IR=read_bus(m,c->PC);
            c->micro=811; return;
        case 811:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 812: /* lax_idx */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=813; return;
        case 813:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP2);
            c->micro=814; return;
        case 814:;
            c->TMP2 += c->X;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2 & 0xff);
            c->micro=815; return;
        case 815:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=816; return;
        case 816:;
            c->irq_sampled=irq;
            c->A = c->X = read_bus(m,c->TMP);
            c->micro=817; return;
        case 817:;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=818; return;
        case 818:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 819: /* ldy_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=820; return;
        case 820:;
            c->PC++;
            c->irq_sampled=irq;
            c->Y = read_bus(m,c->TMP);
            c->micro=821; return;
        case 821:;
            set_nz(c, c->Y);
            c->IR=read_bus(m,c->PC);
            c->micro=822; return;
        case 822:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 823: /* lda_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=824; return;
        case 824:;
            c->PC++;
            c->irq_sampled=irq;
            c->A = read_bus(m,c->TMP);
            c->micro=825; return;
        case 825:;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=826; return;
        case 826:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 827: /* ldx_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=828; return;
        case 828:;
            c->PC++;
            c->irq_sampled=irq;
            c->X = read_bus(m,c->TMP);
            c->micro=829; return;
        case 829:;
            set_nz(c, c->X);
            c->IR=read_bus(m,c->PC);
            c->micro=830; return;
        case 830:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 831: /* lax_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=832; return;
        case 832:;
            c->PC++;
            c->irq_sampled=irq;
            c->A = c->X = read_bus(m,c->TMP);
            c->micro=833; return;
        case 833:;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=834; return;
        case 834:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 835: /* tay_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=836; return;
        case 836:;
            c->Y = c->A;
            set_nz(c, c->Y);
            c->IR=read_bus(m,c->PC);
            c->micro=837; return;
        case 837:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 838: /* lda_imm */
            c->irq_sampled=irq;
            c->A = read_bus(m,c->PC);
            c->micro=839; return;
        case 839:;
            c->PC++;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=840; return;
        case 840:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 841: /* tax_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=842; return;
        case 842:;
            c->X = c->A;
            set_nz(c, c->X);
            c->IR=read_bus(m,c->PC);
            c->micro=843; return;
        case 843:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 844: /* lxa_imm */
            c->irq_sampled=irq;
            c->A = c->X = read_bus(m,c->PC);
            c->micro=845; return;
        case 845:;
            c->PC++;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=846; return;
        case 846:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 847: /* ldy_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=848; return;
        case 848:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=849; return;
        case 849:;
            c->PC++;
            c->irq_sampled=irq;
            c->Y = read_bus(m,c->TMP);
            c->micro=850; return;
        case 850:;
            set_nz(c, c->Y);
            c->IR=read_bus(m,c->PC);
            c->micro=851; return;
        case 851:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 852: /* lda_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=853; return;
        case 853:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=854; return;
        case 854:;
            c->PC++;
            c->irq_sampled=irq;
            c->A = read_bus(m,c->TMP);
            c->micro=855; return;
        case 855:;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=856; return;
        case 856:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 857: /* ldx_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=858; return;
        case 858:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=859; return;
        case 859:;
            c->PC++;
            c->irq_sampled=irq;
            c->X = read_bus(m,c->TMP);
            c->micro=860; return;
        case 860:;
            set_nz(c, c->X);
            c->IR=read_bus(m,c->PC);
            c->micro=861; return;
        case 861:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 862: /* lax_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=863; return;
        case 863:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=864; return;
        case 864:;
            c->PC++;
            c->irq_sampled=irq;
            c->A = c->X = read_bus(m,c->TMP);
            c->micro=865; return;
        case 865:;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=866; return;
        case 866:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 867: /* bcs_rel */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=868; return;
        case 868:;
            c->PC++;
            if(c->P & F_C) {
                if(page_changing(c->PC, (int8_t)(c->TMP))) {
            c->irq_sampled=irq;
                    read_bus(m,c->PC);
            c->micro=869; return;
        case 869:;
            c->irq_sampled=irq;
                    read_bus(m,set_l(c->PC, c->PC+(int8_t)(c->TMP)));
            c->micro=870; return;
        case 870:;
                } else {
                    read_bus(m,c->PC);
            c->micro=871; return;
        case 871:;
                }
                c->PC += (int8_t)(c->TMP);
            }
            c->IR=read_bus(m,c->PC);
            c->micro=872; return;
        case 872:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 873: /* lda_idy */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=874; return;
        case 874:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2);
            c->micro=875; return;
        case 875:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=876; return;
        case 876:;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=877; return;
        case 877:;
            }
            c->irq_sampled=irq;
            c->A = read_bus(m,c->TMP+c->Y);
            c->micro=878; return;
        case 878:;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=879; return;
        case 879:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 880: /* lax_idy */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=881; return;
        case 881:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2);
            c->micro=882; return;
        case 882:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=883; return;
        case 883:;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=884; return;
        case 884:;
            }
            c->irq_sampled=irq;
            c->A = c->X = read_bus(m,c->TMP+c->Y);
            c->micro=885; return;
        case 885:;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=886; return;
        case 886:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 887: /* ldy_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=888; return;
        case 888:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=889; return;
        case 889:;
            c->irq_sampled=irq;
            c->Y = read_bus(m,(uint8_t)(c->TMP+c->X));
            c->micro=890; return;
        case 890:;
            set_nz(c, c->Y);
            c->IR=read_bus(m,c->PC);
            c->micro=891; return;
        case 891:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 892: /* lda_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=893; return;
        case 893:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=894; return;
        case 894:;
            c->irq_sampled=irq;
            c->A = read_bus(m,(uint8_t)(c->TMP+c->X));
            c->micro=895; return;
        case 895:;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=896; return;
        case 896:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 897: /* ldx_zpy */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=898; return;
        case 898:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=899; return;
        case 899:;
            c->irq_sampled=irq;
            c->X = read_bus(m,(uint8_t)(c->TMP+c->Y));
            c->micro=900; return;
        case 900:;
            set_nz(c, c->X);
            c->IR=read_bus(m,c->PC);
            c->micro=901; return;
        case 901:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 902: /* lax_zpy */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=903; return;
        case 903:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=904; return;
        case 904:;
            c->TMP = (uint8_t)(c->TMP+c->Y);
            c->irq_sampled=irq;
            c->A = c->X = read_bus(m,c->TMP);
            c->micro=905; return;
        case 905:;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=906; return;
        case 906:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 907: /* clv_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=908; return;
        case 908:;
            c->P &= ~F_V;
            c->IR=read_bus(m,c->PC);
            c->micro=909; return;
        case 909:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 910: /* lda_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=911; return;
        case 911:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=912; return;
        case 912:;
            c->PC++;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=913; return;
        case 913:;
            }
            c->irq_sampled=irq;
            c->A = read_bus(m,c->TMP + c->Y);
            c->micro=914; return;
        case 914:;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=915; return;
        case 915:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 916: /* tsx_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=917; return;
        case 917:;
            c->X = c->SP;
            set_nz(c, c->X);
            c->IR=read_bus(m,c->PC);
            c->micro=918; return;
        case 918:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 919: /* las_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=920; return;
        case 920:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=921; return;
        case 921:;
            c->PC++;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=922; return;
        case 922:;
            }
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP+c->Y);
            c->micro=923; return;
        case 923:;
            c->A = c->TMP2 | 0x51;
            c->X = 0xff;
            set_nz(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=924; return;
        case 924:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 925: /* ldy_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=926; return;
        case 926:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=927; return;
        case 927:;
            c->PC++;
            if(page_changing(c->TMP, c->X)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=928; return;
        case 928:;
            }
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->Y = read_bus(m,c->TMP);
            c->micro=929; return;
        case 929:;
            set_nz(c, c->Y);
            c->IR=read_bus(m,c->PC);
            c->micro=930; return;
        case 930:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 931: /* lda_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=932; return;
        case 932:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=933; return;
        case 933:;
            c->PC++;
            if(page_changing(c->TMP, c->X)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=934; return;
        case 934:;
            }
            c->irq_sampled=irq;
            c->A = read_bus(m,c->TMP + c->X);
            c->micro=935; return;
        case 935:;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=936; return;
        case 936:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 937: /* ldx_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=938; return;
        case 938:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=939; return;
        case 939:;
            c->PC++;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=940; return;
        case 940:;
            }
            c->irq_sampled=irq;
            c->X = read_bus(m,c->TMP + c->Y);
            c->micro=941; return;
        case 941:;
            set_nz(c, c->X);
            c->IR=read_bus(m,c->PC);
            c->micro=942; return;
        case 942:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 943: /* lax_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=944; return;
        case 944:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=945; return;
        case 945:;
            c->PC++;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=946; return;
        case 946:;
            }
            c->irq_sampled=irq;
            c->A = c->X = read_bus(m,c->TMP+c->Y);
            c->micro=947; return;
        case 947:;
            set_nz(c, c->A);
            c->IR=read_bus(m,c->PC);
            c->micro=948; return;
        case 948:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 949: /* cpy_imm */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=950; return;
        case 950:;
            c->PC++;
            do_cmp(c, c->Y, c->TMP);
            c->IR=read_bus(m,c->PC);
            c->micro=951; return;
        case 951:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 952: /* cmp_idx */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=953; return;
        case 953:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP2);
            c->micro=954; return;
        case 954:;
            c->TMP2 += c->X;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2 & 0xff);
            c->micro=955; return;
        case 955:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=956; return;
        case 956:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=957; return;
        case 957:;
            do_cmp(c, c->A, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=958; return;
        case 958:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 959: /* dcp_idx */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=960; return;
        case 960:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP2);
            c->micro=961; return;
        case 961:;
            c->TMP2 += c->X;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2 & 0xff);
            c->micro=962; return;
        case 962:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=963; return;
        case 963:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=964; return;
        case 964:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=965; return;
        case 965:;
            c->TMP2--;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=966; return;
        case 966:;
            do_cmp(c, c->A, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=967; return;
        case 967:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 968: /* cpy_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=969; return;
        case 969:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=970; return;
        case 970:;
            do_cmp(c, c->Y, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=971; return;
        case 971:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 972: /* cmp_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=973; return;
        case 973:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=974; return;
        case 974:;
            do_cmp(c, c->A, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=975; return;
        case 975:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 976: /* dec_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=977; return;
        case 977:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=978; return;
        case 978:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=979; return;
        case 979:;
            c->TMP2--;
            set_nz(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=980; return;
        case 980:;
            c->IR=read_bus(m,c->PC);
            c->micro=981; return;
        case 981:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 982: /* dcp_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=983; return;
        case 983:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=984; return;
        case 984:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=985; return;
        case 985:;
            c->TMP2--;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=986; return;
        case 986:;
            do_cmp(c, c->A, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=987; return;
        case 987:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 988: /* iny_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=989; return;
        case 989:;
            c->Y++;
            set_nz(c, c->Y);
            c->IR=read_bus(m,c->PC);
            c->micro=990; return;
        case 990:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 991: /* cmp_imm */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=992; return;
        case 992:;
            c->PC++;
            do_cmp(c, c->A, c->TMP);
            c->IR=read_bus(m,c->PC);
            c->micro=993; return;
        case 993:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 994: /* dex_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=995; return;
        case 995:;
            c->X--;
            set_nz(c, c->X);
            c->IR=read_bus(m,c->PC);
            c->micro=996; return;
        case 996:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 997: /* sbx_imm */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=998; return;
        case 998:;
            c->PC++;
            c->X &= c->A;
            if(c->X < c->TMP2)
                c->P &= ~F_C;
            else
                c->P |= F_C;
            c->X -= c->TMP2;
            set_nz(c, c->X);
            c->IR=read_bus(m,c->PC);
            c->micro=999; return;
        case 999:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1000: /* cpy_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1001; return;
        case 1001:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1002; return;
        case 1002:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1003; return;
        case 1003:;
            do_cmp(c, c->Y, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1004; return;
        case 1004:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1005: /* cmp_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1006; return;
        case 1006:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1007; return;
        case 1007:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1008; return;
        case 1008:;
            do_cmp(c, c->A, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1009; return;
        case 1009:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1010: /* dec_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1011; return;
        case 1011:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1012; return;
        case 1012:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1013; return;
        case 1013:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1014; return;
        case 1014:;
            c->TMP2--;
            set_nz(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1015; return;
        case 1015:;
            c->IR=read_bus(m,c->PC);
            c->micro=1016; return;
        case 1016:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1017: /* dcp_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1018; return;
        case 1018:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1019; return;
        case 1019:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1020; return;
        case 1020:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1021; return;
        case 1021:;
            c->TMP2--;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1022; return;
        case 1022:;
            do_cmp(c, c->A, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1023; return;
        case 1023:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1024: /* bne_rel */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1025; return;
        case 1025:;
            c->PC++;
            if(!(c->P & F_Z)) {
                if(page_changing(c->PC, (int8_t)(c->TMP))) {
            c->irq_sampled=irq;
                    read_bus(m,c->PC);
            c->micro=1026; return;
        case 1026:;
            c->irq_sampled=irq;
                    read_bus(m,set_l(c->PC, c->PC+(int8_t)(c->TMP)));
            c->micro=1027; return;
        case 1027:;
                } else {
                    read_bus(m,c->PC);
            c->micro=1028; return;
        case 1028:;
                }
                c->PC += (int8_t)(c->TMP);
            }
            c->IR=read_bus(m,c->PC);
            c->micro=1029; return;
        case 1029:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1030: /* cmp_idy */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=1031; return;
        case 1031:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2);
            c->micro=1032; return;
        case 1032:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=1033; return;
        case 1033:;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=1034; return;
        case 1034:;
            }
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP+c->Y);
            c->micro=1035; return;
        case 1035:;
            do_cmp(c, c->A, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1036; return;
        case 1036:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1037: /* dcp_idy */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=1038; return;
        case 1038:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2);
            c->micro=1039; return;
        case 1039:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=1040; return;
        case 1040:;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=1041; return;
        case 1041:;
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1042; return;
        case 1042:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1043; return;
        case 1043:;
            c->TMP2--;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1044; return;
        case 1044:;
            do_cmp(c, c->A, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1045; return;
        case 1045:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1046: /* cmp_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1047; return;
        case 1047:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=1048; return;
        case 1048:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,(uint8_t)(c->TMP+c->X));
            c->micro=1049; return;
        case 1049:;
            do_cmp(c, c->A, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1050; return;
        case 1050:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1051: /* dec_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1052; return;
        case 1052:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=1053; return;
        case 1053:;
            c->TMP = (uint8_t)(c->TMP+c->X);
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1054; return;
        case 1054:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1055; return;
        case 1055:;
            c->TMP2--;
            set_nz(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1056; return;
        case 1056:;
            c->IR=read_bus(m,c->PC);
            c->micro=1057; return;
        case 1057:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1058: /* dcp_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1059; return;
        case 1059:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=1060; return;
        case 1060:;
            c->TMP = (uint8_t)(c->TMP+c->X);
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1061; return;
        case 1061:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1062; return;
        case 1062:;
            c->TMP2--;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1063; return;
        case 1063:;
            do_cmp(c, c->A, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1064; return;
        case 1064:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1065: /* cld_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=1066; return;
        case 1066:;
            c->P &= ~F_D;
            c->IR=read_bus(m,c->PC);
            c->micro=1067; return;
        case 1067:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1068: /* cmp_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1069; return;
        case 1069:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1070; return;
        case 1070:;
            c->PC++;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=1071; return;
        case 1071:;
            }
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1072; return;
        case 1072:;
            do_cmp(c, c->A, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1073; return;
        case 1073:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1074: /* dcp_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1075; return;
        case 1075:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1076; return;
        case 1076:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=1077; return;
        case 1077:;
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1078; return;
        case 1078:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1079; return;
        case 1079:;
            c->TMP2--;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1080; return;
        case 1080:;
            do_cmp(c, c->A, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1081; return;
        case 1081:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1082: /* cmp_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1083; return;
        case 1083:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1084; return;
        case 1084:;
            c->PC++;
            if(page_changing(c->TMP, c->X)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=1085; return;
        case 1085:;
            }
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1086; return;
        case 1086:;
            do_cmp(c, c->A, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1087; return;
        case 1087:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1088: /* dec_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1089; return;
        case 1089:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1090; return;
        case 1090:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=1091; return;
        case 1091:;
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1092; return;
        case 1092:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1093; return;
        case 1093:;
            c->TMP2--;
            set_nz(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1094; return;
        case 1094:;
            c->IR=read_bus(m,c->PC);
            c->micro=1095; return;
        case 1095:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1096: /* dcp_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1097; return;
        case 1097:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1098; return;
        case 1098:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=1099; return;
        case 1099:;
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1100; return;
        case 1100:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1101; return;
        case 1101:;
            c->TMP2--;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1102; return;
        case 1102:;
            do_cmp(c, c->A, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1103; return;
        case 1103:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1104: /* cpx_imm */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1105; return;
        case 1105:;
            c->PC++;
            do_cmp(c, c->X, c->TMP);
            c->IR=read_bus(m,c->PC);
            c->micro=1106; return;
        case 1106:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1107: /* sbc_idx */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=1108; return;
        case 1108:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP2);
            c->micro=1109; return;
        case 1109:;
            c->TMP2 += c->X;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2 & 0xff);
            c->micro=1110; return;
        case 1110:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=1111; return;
        case 1111:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1112; return;
        case 1112:;
            do_sbc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1113; return;
        case 1113:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1114: /* isb_idx */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=1115; return;
        case 1115:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP2);
            c->micro=1116; return;
        case 1116:;
            c->TMP2 += c->X;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2 & 0xff);
            c->micro=1117; return;
        case 1117:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=1118; return;
        case 1118:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1119; return;
        case 1119:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1120; return;
        case 1120:;
            c->TMP2++;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1121; return;
        case 1121:;
            do_sbc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1122; return;
        case 1122:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1123: /* cpx_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1124; return;
        case 1124:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1125; return;
        case 1125:;
            do_cmp(c, c->X, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1126; return;
        case 1126:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1127: /* sbc_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1128; return;
        case 1128:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1129; return;
        case 1129:;
            do_sbc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1130; return;
        case 1130:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1131: /* inc_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1132; return;
        case 1132:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1133; return;
        case 1133:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1134; return;
        case 1134:;
            c->TMP2++;
            set_nz(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1135; return;
        case 1135:;
            c->IR=read_bus(m,c->PC);
            c->micro=1136; return;
        case 1136:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1137: /* isb_zpg */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1138; return;
        case 1138:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1139; return;
        case 1139:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1140; return;
        case 1140:;
            c->TMP2++;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1141; return;
        case 1141:;
            do_sbc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1142; return;
        case 1142:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1143: /* inx_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=1144; return;
        case 1144:;
            c->X++;
            set_nz(c, c->X);
            c->IR=read_bus(m,c->PC);
            c->micro=1145; return;
        case 1145:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1146: /* sbc_imm */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1147; return;
        case 1147:;
            c->PC++;
            do_sbc(c, c->TMP);
            c->IR=read_bus(m,c->PC);
            c->micro=1148; return;
        case 1148:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1149: /* cpx_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1150; return;
        case 1150:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1151; return;
        case 1151:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1152; return;
        case 1152:;
            do_cmp(c, c->X, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1153; return;
        case 1153:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1154: /* sbc_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1155; return;
        case 1155:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1156; return;
        case 1156:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1157; return;
        case 1157:;
            do_sbc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1158; return;
        case 1158:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1159: /* inc_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1160; return;
        case 1160:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1161; return;
        case 1161:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1162; return;
        case 1162:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1163; return;
        case 1163:;
            c->TMP2++;
            set_nz(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1164; return;
        case 1164:;
            c->IR=read_bus(m,c->PC);
            c->micro=1165; return;
        case 1165:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1166: /* isb_aba */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1167; return;
        case 1167:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1168; return;
        case 1168:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1169; return;
        case 1169:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1170; return;
        case 1170:;
            c->TMP2++;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1171; return;
        case 1171:;
            do_sbc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1172; return;
        case 1172:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1173: /* beq_rel */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1174; return;
        case 1174:;
            c->PC++;
            if(c->P & F_Z) {
                if(page_changing(c->PC, (int8_t)(c->TMP))) {
            c->irq_sampled=irq;
                    read_bus(m,c->PC);
            c->micro=1175; return;
        case 1175:;
            c->irq_sampled=irq;
                    read_bus(m,set_l(c->PC, c->PC+(int8_t)(c->TMP)));
            c->micro=1176; return;
        case 1176:;
                } else {
                    read_bus(m,c->PC);
            c->micro=1177; return;
        case 1177:;
                }
                c->PC += (int8_t)(c->TMP);
            }
            c->IR=read_bus(m,c->PC);
            c->micro=1178; return;
        case 1178:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1179: /* sbc_idy */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=1180; return;
        case 1180:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2);
            c->micro=1181; return;
        case 1181:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=1182; return;
        case 1182:;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=1183; return;
        case 1183:;
            }
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP+c->Y);
            c->micro=1184; return;
        case 1184:;
            do_sbc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1185; return;
        case 1185:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1186: /* isb_idy */
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->PC);
            c->micro=1187; return;
        case 1187:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->TMP2);
            c->micro=1188; return;
        case 1188:;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,(c->TMP2+1) & 0xff));
            c->micro=1189; return;
        case 1189:;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=1190; return;
        case 1190:;
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1191; return;
        case 1191:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1192; return;
        case 1192:;
            c->TMP2++;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1193; return;
        case 1193:;
            do_sbc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1194; return;
        case 1194:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1195: /* sbc_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1196; return;
        case 1196:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=1197; return;
        case 1197:;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,(uint8_t)(c->TMP+c->X));
            c->micro=1198; return;
        case 1198:;
            do_sbc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1199; return;
        case 1199:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1200: /* inc_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1201; return;
        case 1201:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=1202; return;
        case 1202:;
            c->TMP = (uint8_t)(c->TMP+c->X);
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1203; return;
        case 1203:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1204; return;
        case 1204:;
            c->TMP2++;
            set_nz(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1205; return;
        case 1205:;
            c->IR=read_bus(m,c->PC);
            c->micro=1206; return;
        case 1206:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1207: /* isb_zpx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1208; return;
        case 1208:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,c->TMP);
            c->micro=1209; return;
        case 1209:;
            c->TMP = (uint8_t)(c->TMP+c->X);
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1210; return;
        case 1210:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1211; return;
        case 1211:;
            c->TMP2++;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1212; return;
        case 1212:;
            do_sbc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1213; return;
        case 1213:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1214: /* sed_imp */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=1215; return;
        case 1215:;
            c->P |= F_D;
            c->IR=read_bus(m,c->PC);
            c->micro=1216; return;
        case 1216:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1217: /* sbc_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1218; return;
        case 1218:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1219; return;
        case 1219:;
            c->PC++;
            if(page_changing(c->TMP, c->Y)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=1220; return;
        case 1220:;
            }
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1221; return;
        case 1221:;
            do_sbc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1222; return;
        case 1222:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1223: /* isb_aby */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1224; return;
        case 1224:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1225; return;
        case 1225:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->Y));
            c->micro=1226; return;
        case 1226:;
            c->TMP += c->Y;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1227; return;
        case 1227:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1228; return;
        case 1228:;
            c->TMP2++;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1229; return;
        case 1229:;
            do_sbc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1230; return;
        case 1230:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1231: /* sbc_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1232; return;
        case 1232:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1233; return;
        case 1233:;
            c->PC++;
            if(page_changing(c->TMP, c->X)) {
            c->irq_sampled=irq;
                read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=1234; return;
        case 1234:;
            }
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1235; return;
        case 1235:;
            do_sbc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1236; return;
        case 1236:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1237: /* inc_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1238; return;
        case 1238:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1239; return;
        case 1239:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=1240; return;
        case 1240:;
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1241; return;
        case 1241:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1242; return;
        case 1242:;
            c->TMP2++;
            set_nz(c, c->TMP2);
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1243; return;
        case 1243:;
            c->IR=read_bus(m,c->PC);
            c->micro=1244; return;
        case 1244:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1245: /* isb_abx */
            c->irq_sampled=irq;
            c->TMP = read_bus(m,c->PC);
            c->micro=1246; return;
        case 1246:;
            c->PC++;
            c->irq_sampled=irq;
            c->TMP = set_h(c->TMP, read_bus(m,c->PC));
            c->micro=1247; return;
        case 1247:;
            c->PC++;
            c->irq_sampled=irq;
            read_bus(m,set_l(c->TMP, c->TMP+c->X));
            c->micro=1248; return;
        case 1248:;
            c->TMP += c->X;
            c->irq_sampled=irq;
            c->TMP2 = read_bus(m,c->TMP);
            c->micro=1249; return;
        case 1249:;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1250; return;
        case 1250:;
            c->TMP2++;
            c->irq_sampled=irq;
            write_bus(m,c->TMP, c->TMP2);
            c->micro=1251; return;
        case 1251:;
            do_sbc(c, c->TMP2);
            c->IR=read_bus(m,c->PC);
            c->micro=1252; return;
        case 1252:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        case 1253: /* reset */
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=1254; return;
        case 1254:;
            c->irq_sampled=irq;
            read_bus(m,c->PC);
            c->micro=1255; return;
        case 1255:;
            c->irq_sampled=irq;
            read_bus(m,c->SP);
            c->micro=1256; return;
        case 1256:;
            c->SP=set_l(c->SP,c->SP-1);
            c->irq_sampled=irq;
            read_bus(m,c->SP);
            c->micro=1257; return;
        case 1257:;
            c->SP=set_l(c->SP,c->SP-1);
            c->irq_sampled=irq;
            read_bus(m,c->SP);
            c->micro=1258; return;
        case 1258:;
            c->SP=set_l(c->SP,c->SP-1);
            c->P |= F_I;
            c->irq_sampled=irq;
            c->PC = read_bus(m,0xfffc);
            c->micro=1259; return;
        case 1259:;
            c->irq_sampled=irq;
            c->PC = set_h(c->PC, read_bus(m,0xfffd));
            c->micro=1260; return;
        case 1260:;
            c->IR=read_bus(m,c->PC);
            c->micro=1261; return;
        case 1261:
            if (c->irq_sampled && !(c->P & F_I)) {
                c->irq_taken=true; c->IR=0;
            } else ++c->PC;
            break;
        }
        c->micro=decode[c->IR];
    }
}
