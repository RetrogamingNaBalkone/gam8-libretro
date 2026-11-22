/* Standalone libretro frontend used by the differential test. */
#include "../third_party/libretro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(expr)                                                                                \
    do {                                                                                           \
        if (!(expr)) {                                                                             \
            fprintf(stderr, "Failed at line %d: %s\n", __LINE__, #expr);                           \
            exit(1);                                                                               \
        }                                                                                          \
    } while (0)
static const char *system_dir, *bios, *palette = "mame", *ghost = "off";
static const char *mapper = "auto";
static FILE *capture, *wave;
FILE *psg_trace;
static unsigned frame, limit, test_mode;
static size_t sample_count;
static uint32_t last_video, last_audio;
static bool updated;
static unsigned legacy_opts, modern_opts;
static uint32_t hash(const void *data, size_t len)
{
    uint32_t h = 2166136261u;
    const uint8_t *p = (const uint8_t *)data;
    while (len--)
        h = (h ^ *p++) * 16777619u;
    return h;
}
static bool environment(unsigned cmd, void *data)
{
    switch (cmd) {
    case RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION:
        *(unsigned *)data = test_mode ? 0 : 2;
        return true;
    case RETRO_ENVIRONMENT_SET_CORE_OPTIONS_V2:
        ++modern_opts;
        return true;
    case RETRO_ENVIRONMENT_SET_VARIABLES:
        ++legacy_opts;
        return true;
    case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
        *(const char **)data = system_dir;
        return true;
    case RETRO_ENVIRONMENT_GET_VARIABLE: {
        struct retro_variable *v = (struct retro_variable *)data;
        if (!strcmp(v->key, "gamate_bios"))
            v->value = bios;
        else if (!strcmp(v->key, "gamate_ghost"))
            v->value = ghost;
        else if (!strcmp(v->key, "gamate_palette"))
            v->value = palette;
        else if (!strcmp(v->key, "gamate_mapper"))
            v->value = mapper;
        else
            return false;
        return true;
    }
    case RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE:
        *(bool *)data = updated;
        updated = false;
        return true;
    case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT:
        return *(enum retro_pixel_format *)data == RETRO_PIXEL_FORMAT_XRGB8888;
    case RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS:
        return true;
    default:
        return false;
    }
}
static void present(const void *buffer, unsigned width, unsigned height, size_t pitch)
{
    CHECK(width == 160 && height == 150 && pitch == 640);
    if (test_mode)
        last_video = hash(buffer, height * pitch);
    if (capture && (frame == 60 || frame == 180 || frame == 300 || frame == 360 || frame == 600 ||
                    frame == 900 || frame == 1200 || frame == limit))
        CHECK(fwrite(buffer, height * pitch, 1, capture) == 1);
}
static size_t sound(const int16_t *samples, size_t n)
{
    CHECK(n == 789 || n == 790);
    sample_count += n;
    if (wave)
        CHECK(fwrite(samples, 4, n, wave) == n);
    if (test_mode)
        last_audio = hash(samples, n * 4);
    return n;
}
static void poll(void) {}
static int16_t input(unsigned port, unsigned device, unsigned index, unsigned id)
{
    CHECK(!port && device == RETRO_DEVICE_JOYPAD && !index);
    return (id == RETRO_DEVICE_ID_JOYPAD_START && frame > 780 && frame <= 786) ||
           (id == RETRO_DEVICE_ID_JOYPAD_A && frame > 810 && frame <= 816);
}
static void state_test(void)
{
    size_t size = retro_serialize_size();
    uint8_t *state = (uint8_t *)malloc(size);
    CHECK(state && !retro_serialize(state, size - 1) && retro_serialize(state, size));
    retro_run();
    uint32_t v = last_video, a = last_audio;
    CHECK(retro_unserialize(state, size));
    retro_run();
    CHECK(v == last_video && a == last_audio);
    state[size - 10] ^= 1;
    CHECK(!retro_unserialize(state, size));
    free(state);
}
static void cartridge_test(void)
{
    static uint8_t image[131072];
    struct retro_game_info game = {"patched.bin", image, sizeof(image) - 1, NULL};
    CHECK(!retro_load_game(&game));
    game.size = sizeof(image);
    CHECK(retro_load_game(&game));
    size_t size = retro_serialize_size();
    uint8_t *state = (uint8_t *)malloc(size);
    CHECK(state && retro_serialize(state, size));
    CHECK(state[4] == 1);
    retro_unload_game();
    mapper = "4in1";
    CHECK(retro_load_game(&game) && !retro_unserialize(state, size));
    CHECK(retro_serialize(state, size) && retro_unserialize(state, size));
    retro_unload_game();
    mapper = "auto";
    CHECK(retro_load_game(&game) && !retro_unserialize(state, size));
    free(state);
    retro_unload_game();
}

int main(int argc, char **argv)
{
    if (argc != 7) {
        fprintf(stderr, "frontend SYSTEM BIOS ROM FRAMES OUTPUT TEST\\n");
        return 2;
    }
    system_dir = argv[1];
    bios = argv[2];
    limit = (unsigned)atoi(argv[4]);
    test_mode = (unsigned)atoi(argv[6]);
    FILE *f = fopen(argv[3], "rb");
    CHECK(f);
    CHECK(!fseek(f, 0, SEEK_END));
    long len = ftell(f);
    rewind(f);
    void *rom = malloc(len);
    CHECK(rom && fread(rom, len, 1, f) == 1);
    fclose(f);
    struct retro_game_info game = {argv[3], rom, (size_t)len, NULL};
    retro_set_environment(environment);
    retro_set_video_refresh(present);
    retro_set_audio_sample_batch(sound);
    retro_set_input_poll(poll);
    retro_set_input_state(input);
    retro_init();
    CHECK(test_mode ? legacy_opts : modern_opts);
    CHECK(retro_api_version() == RETRO_API_VERSION && retro_load_game(&game));
    CHECK(retro_get_memory_size(RETRO_MEMORY_SYSTEM_RAM) == 1024);
    if (getenv("GAMATE_PSG")) {
        psg_trace = fopen(getenv("GAMATE_PSG"), "w");
        CHECK(psg_trace);
    }
    if (getenv("GAMATE_PCM")) {
        wave = fopen(getenv("GAMATE_PCM"), "wb");
        CHECK(wave);
    }
    capture = fopen(argv[5], "wb");
    CHECK(capture);
    for (frame = 1; frame <= limit; ++frame)
        retro_run();
    fclose(capture);
    capture = NULL;
    if (psg_trace) {
        fclose(psg_trace);
        psg_trace = NULL;
    }
    if (wave) {
        fclose(wave);
        wave = NULL;
    }
    CHECK(sample_count == (size_t)(limit * 48000.0 / 60.8093) ||
          sample_count + 1 == (size_t)(limit * 48000.0 / 60.8093));
    if (test_mode) {
        ghost = "medium";
        palette = "game";
        updated = true;
        retro_run();
        state_test();
        ghost = "off";
        palette = "gray";
        updated = true;
        retro_run();
        state_test();
        retro_reset();
        retro_run();
        state_test();
        retro_unload_game();
        const char *valid = system_dir;
        system_dir = "__missing_firmware__";
        CHECK(!retro_load_game(&game));
        system_dir = valid;
        CHECK(retro_load_game(&game));
        retro_run();
    }
    retro_unload_game();
    if (test_mode)
        cartridge_test();
    retro_deinit();
    free(rom);
    return 0;
}
