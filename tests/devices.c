/* Redistributable diagnostic BIOS and program are generated here in RAM. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/machine.c"
#define CHECK(x)                                                                                   \
    do {                                                                                           \
        if (!(x)) {                                                                                \
            fprintf(stderr, "line %d: %s\n", __LINE__, #x);                                        \
            exit(1);                                                                               \
        }                                                                                          \
    } while (0)
int main(void)
{
    static uint8_t firmware_image[4096], image[65536], pixels[WIDTH * HEIGHT];
    static int16_t samples[1600];
    static machine_t m;
    /* LDA #$42; STA $1c00; INC $00; JMP $e007 */
    const uint8_t program[] = {0xa9, 0x42, 0x8d, 0, 0x1c, 0xe6, 0, 0x4c, 7, 0xe0};
    memcpy(firmware_image, program, sizeof(program));
    firmware_image[0xffd] = 0xe0;
    machine_cart(image, sizeof(image), firmware_image, MULTI);
    machine_power(&m);
    CHECK(machine_frame(&m, pixels, samples) == 789 && read_bus(&m, 0) == 0x43);
    CHECK(read_bus(&m, 0x5a00) == 1 && read_bus(&m, 0x5800) == 0 && read_bus(&m, 0x5a00) == 3);
    for (const char *p = "BIT CORPORATION"; *p; ++p) {
        for (int bit = 7; bit >= 0; --bit)
            write_bus(&m, 0x6000, ((*p >> bit) & 1) << 2);
        write_bus(&m, 0x6000, 0);
    }
    unsigned reply = 0;
    for (unsigned i = 0; i < 8; ++i)
        reply = (reply << 1) | (read_bus(&m, 0x6000) >> 1);
    CHECK(reply == 0x47 && m.unlocked);
    image[0x4000] = 81;
    image[0x8000] = 82;
    write_bus(&m, 0x8000, 1);
    write_bus(&m, 0xc000, 2);
    CHECK(read_bus(&m, 0x6000) == 81 && read_bus(&m, 0xa000) == 82);
    write_bus(&m, 0x53fb, 0);
    write_bus(&m, 0x5004, 0);
    write_bus(&m, 0x5005, 0);
    write_bus(&m, 0x5007, 128);
    scanline(&m, 0, pixels);
    CHECK(pixels[0] == 1 && !pixels[1]);
    write_bus(&m, 0x5001, 16);
    scanline(&m, 0, pixels);
    CHECK(pixels[0] == 2);
    write_bus(&m, 0x5001, 128);
    scanline(&m, 0, pixels);
    CHECK(!pixels[0]);
    write_bus(&m, 0x5001, 64);
    write_bus(&m, 0x5004, 0);
    write_bus(&m, 0x5005, 0);
    CHECK(read_bus(&m, 0x5006) == 128 && m.cursor == 32);
    write_bus(&m, 0x4007, 63);
    write_bus(&m, 0x4008, 15);
    psg_tick(&m.psg);
    CHECK(m.psg.output[0] > m.psg.output[1]);
    write_bus(&m, 0x4008, 0);
    write_bus(&m, 0x4009, 15);
    psg_tick(&m.psg);
    CHECK(m.psg.output[1] > m.psg.output[0]);
    machine_reset(&m);
    CHECK(!m.unlocked && m.ram[0] == 0x43 && m.cursor == 0);
    puts("Device diagnostics passed");
    return 0;
}
