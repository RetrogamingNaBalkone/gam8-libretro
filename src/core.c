// SPDX-License-Identifier: GPL-2.0-or-later
#include "../third_party/libretro.h"
#include "machine.h"
#include <stdio.h>
#if defined(_MSC_VER) && _MSC_VER < 1900
#define snprintf _snprintf
#endif
#include <stdlib.h>
#include <string.h>

static retro_environment_t env;
static retro_video_refresh_t video;
static retro_audio_sample_t audio;
static retro_audio_sample_batch_t batch;
static retro_input_poll_t poll;
static retro_input_state_t input;
static retro_log_printf_t logger;
static const struct retro_vfs_interface *vfs;
static machine_t machine;
static uint8_t *cart, bios[4096], lcd[WIDTH * HEIGHT];
static uint32_t history[WIDTH * HEIGHT], colors[4], cart_id, bios_id;
// Version 1 identified the board from the cartridge CRC and size alone.
static unsigned state_version = 1;
static uint16_t frame565[WIDTH * HEIGHT];
static int16_t pcm[1600];
static unsigned ghost;
static bool valid_history, rgb565, pad = true;
static char preset[4096];

static struct retro_core_option_v2_definition defs[] = {
    {"gamate_palette",
     "Color palette",
     NULL,
     "Select the four LCD colors. Game reads the cartridge's Pico INI preset.",
     NULL,
     NULL,
     {{"mame", "Original (MAME)"},
      {"pico", "Pico"},
      {"gray", "Grayscale"},
      {"game", "Game (INI)"},
      {NULL, NULL}},
     "mame"},
    {"gamate_ghost",
     "LCD ghosting",
     NULL,
     "Blend each pixel with the previous frame.",
     NULL,
     NULL,
     {{"off", "Off"},
      {"light", "Light (25%)"},
      {"medium", "Medium (50%)"},
      {"strong", "Strong (75%)"},
      {NULL, NULL}},
     "off"},
    {"gamate_bios",
     "BIOS (restart)",
     NULL,
     "Auto prefers UMC when both revisions are available.",
     NULL,
     NULL,
     {{"auto", "Auto"}, {"umc", "UMC"}, {"bit", "BIT"}, {NULL, NULL}},
     "auto"},
    {"gamate_mapper",
     "Cartridge board (restart)",
     NULL,
     "Select 4-in-1 for a patched 128 KiB multicart. Known dumps always use their original board.",
     NULL,
     NULL,
     {{"auto", "Auto"}, {"4in1", "4-in-1"}, {NULL, NULL}},
     "auto"},
    {0}};

void retro_set_environment(retro_environment_t cb)
{
    env = cb;
    unsigned version = 0;
    env(RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION, &version);
    if (version >= 2) {
        static struct retro_core_option_v2_category categories[] = {{0}};
        static struct retro_core_options_v2 opts = {categories, defs};
        env(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_V2, &opts);
    } else {
        static struct retro_variable vars[] = {
            {"gamate_palette", "Color palette; mame|pico|gray|game"},
            {"gamate_ghost", "LCD ghosting; off|light|medium|strong"},
            {"gamate_bios", "BIOS (restart); auto|umc|bit"},
            {"gamate_mapper", "Cartridge board (restart); auto|4in1"},
            {NULL, NULL}};
        env(RETRO_ENVIRONMENT_SET_VARIABLES, vars);
    }
}
void retro_set_video_refresh(retro_video_refresh_t cb) { video = cb; }
void retro_set_audio_sample(retro_audio_sample_t cb) { audio = cb; }
void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { batch = cb; }
void retro_set_input_poll(retro_input_poll_t cb) { poll = cb; }
void retro_set_input_state(retro_input_state_t cb) { input = cb; }
void retro_set_controller_port_device(unsigned port, unsigned device)
{
    if (!port)
        pad = (device & RETRO_DEVICE_MASK) == RETRO_DEVICE_JOYPAD;
}

void retro_init(void)
{
    struct retro_log_callback log = {0};
    struct retro_vfs_interface_info files = {1, NULL};
    logger = env(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log) ? log.log : NULL;
    vfs = env(RETRO_ENVIRONMENT_GET_VFS_INTERFACE, &files) ? files.iface : NULL;
    pad = true;
}
unsigned retro_api_version(void) { return RETRO_API_VERSION; }
void retro_get_system_info(struct retro_system_info *info)
{
    memset(info, 0, sizeof(*info));
    info->library_name = "Gam8";
    info->library_version = "0.1";
    info->valid_extensions = "bin";
}
void retro_get_system_av_info(struct retro_system_av_info *info)
{
    info->geometry.base_width = info->geometry.max_width = WIDTH;
    info->geometry.base_height = info->geometry.max_height = HEIGHT;
    info->geometry.aspect_ratio = (float)WIDTH / HEIGHT;
    info->timing.fps = 60.8093;
    info->timing.sample_rate = SAMPLE_RATE;
}
static const char *option(const char *key, const char *fallback)
{
    struct retro_variable var = {key, NULL};
    return env(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value ? var.value : fallback;
}

// Read up to capacity bytes, including one extra byte for oversize detection.
static size_t read_file(const char *path, void *data, size_t capacity)
{
    if (vfs) {
        struct retro_vfs_file_handle *f = vfs->open(path, RETRO_VFS_FILE_ACCESS_READ, 0);
        if (!f)
            return 0;
        int64_t n = vfs->read(f, data, capacity);
        vfs->close(f);
        return n < 0 ? 0 : (size_t)n;
    }
    FILE *f = fopen(path, "rb");
    if (!f)
        return 0;
    size_t n = fread(data, 1, capacity, f);
    if (ferror(f))
        n = 0;
    fclose(f);
    return n;
}

static bool game_palette(uint32_t *out)
{
    char buf[512];
    size_t n = preset[0] ? read_file(preset, buf, sizeof(buf)) : 0;
    if (!n || n == sizeof(buf) || memchr(buf, 0, n))
        return false;
    buf[n] = 0;
    char *p = buf;
    if (strncmp(p, "[palette]", 9))
        return false;
    p += 9;
    for (unsigned i = 0; i < 4; ++i) {
        p += strspn(p, "\r\n");
        if (strncmp(p, "rgb", 3) || p[3] != '0' + (int)i || p[4] != '=')
            return false;
        p += 5;
        if (strspn(p, "0123456789abcdefABCDEF") != 6)
            return false;
        out[i] = (uint32_t)strtoul(p, NULL, 16);
        p += 6;
    }
    return p[strspn(p, "\r\n")] == 0;
}

static void options(void)
{
    static const uint32_t palettes[][4] = {{0x6ba64a, 0x437a63, 0x255955, 0x12424c},
                                           {0xccffff, 0xffb266, 0xcc0066, 0x663300},
                                           {0xffffff, 0xaaaaaa, 0x555555, 0}};
    const char *choice = option("gamate_palette", "mame");
    uint32_t next[4];
    memcpy(next,
           palettes[!strcmp(choice, "gray")   ? 2
                    : !strcmp(choice, "pico") ? 1
                                              : 0],
           sizeof(next));
    if (!strcmp(choice, "game") && !game_palette(next)) {
        memcpy(next, palettes[0], sizeof(next));
        if (logger)
            logger(RETRO_LOG_WARN, "Missing or invalid palette: %s\n", preset);
    }
    if (memcmp(colors, next, sizeof(next)))
        valid_history = false;
    memcpy(colors, next, sizeof(next));
    choice = option("gamate_ghost", "off");
    ghost = !strcmp(choice, "strong")   ? 3
            : !strcmp(choice, "medium") ? 2
            : !strcmp(choice, "light")  ? 1
                                        : 0;
}

static void preset_path(const char *system, const char *path)
{
    struct retro_game_info_ext *ext = NULL;
    const char *base = path ? path : "";
    for (const char *p = base; *p; ++p)
        if (*p == '/' || *p == '\\' || *p == '#')
            base = p + 1;
    size_t len = strlen(base);
    const char *dot = strrchr(base, '.');
    if (dot)
        len = (size_t)(dot - base);
    if (env(RETRO_ENVIRONMENT_GET_GAME_INFO_EXT, &ext) && ext && ext->name && *ext->name) {
        base = ext->name;
        len = strlen(base);
    }
    preset[0] = 0;
    if (!len || len > 1024 || strpbrk(base, "/\\") || !strcmp(base, ".."))
        return;
    int n = snprintf(preset, sizeof(preset), "%s/gamate/palettes/%.*s.ini", system, (int)len, base);
    if (n < 0 || (size_t)n >= sizeof(preset))
        preset[0] = 0;
}

static bool firmware(const char *system, const char *type, uint32_t expected)
{
    char path[4096];
    uint8_t bytes[4097];
    int n = snprintf(path, sizeof(path), "%s/gamate_bios_%s.bin", system, type);
    if (n < 0 || (size_t)n >= sizeof(path))
        return false;
    size_t size = read_file(path, bytes, sizeof(bytes));
    if (size != sizeof(bios) || gamate_crc32(bytes, size) != expected) {
        if (logger)
            logger(RETRO_LOG_WARN, "BIOS %s missing or invalid (expected 4096 bytes, CRC %08x)\n",
                   path, expected);
        return false;
    }
    memcpy(bios, bytes, sizeof(bios));
    bios_id = expected;
    if (logger)
        logger(RETRO_LOG_INFO, "Loaded %s BIOS (CRC %08x)\n",
               expected == 0x07090415 ? "UMC" : "BIT", expected);
    return true;
}
static bool error_message(const char *message)
{
    struct retro_message msg = {message, 300};
    if (logger)
        logger(RETRO_LOG_ERROR, "%s\n", message);
    env(RETRO_ENVIRONMENT_SET_MESSAGE, &msg);
    return false;
}
#define BUTTON(id, label)                                                                          \
    {                                                                                              \
        0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_##id, label                              \
    }
bool retro_load_game(const struct retro_game_info *game)
{
    static struct retro_input_descriptor buttons[] = {BUTTON(UP, "Up"),
                                                      BUTTON(DOWN, "Down"),
                                                      BUTTON(LEFT, "Left"),
                                                      BUTTON(RIGHT, "Right"),
                                                      BUTTON(A, "A"),
                                                      BUTTON(B, "B"),
                                                      BUTTON(START, "Start/Pause"),
                                                      BUTTON(SELECT, "Select"),
                                                      {0}};
    const char *system = NULL;
    if (cart || !game || !game->data)
        return error_message("Invalid Gamate cartridge.");
    if (!machine_cart_size_supported(game->size))
        return error_message("Unsupported Gamate ROM size. Expected 16, 32, 64, 128, 256 or 512 KiB.");
    if (!env(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &system) || !system)
        return error_message("Set the frontend system directory for the Gamate BIOS.");
    const char *mode = option("gamate_bios", "auto");
    if ((!strcmp(mode, "bit") || !firmware(system, "umc", 0x07090415)) &&
        (!strcmp(mode, "umc") || !firmware(system, "bit", 0x03a5f3a7)))
        return error_message("Install the selected gamate_bios_umc.bin or gamate_bios_bit.bin in "
                             "the system directory.");
    enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
    rgb565 = !env(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt);
    if (rgb565) {
        fmt = RETRO_PIXEL_FORMAT_RGB565;
        if (!env(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
            return error_message("Frontend rejected Gamate video formats.");
    }
    cart = (uint8_t *)malloc(game->size);
    if (!cart)
        return error_message("Could not allocate cartridge memory.");
    memcpy(cart, game->data, game->size);
    cart_id = gamate_crc32(cart, game->size);
    bool use_multicart = !strcmp(option("gamate_mapper", "auto"), "4in1");
    enum board pcb = machine_board(cart_id, game->size, use_multicart);
    state_version = use_multicart && pcb != machine_board(cart_id, game->size, false) ? 2 : 1;
    machine_cart(cart, game->size, bios, pcb);
    machine_power(&machine);
    valid_history = false;
    preset_path(system, game->path);
    options();
    env(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, buttons);
    return true;
}

void retro_reset(void)
{
    if (cart)
        machine_reset(&machine);
    valid_history = false;
}
void retro_unload_game(void)
{
    machine_cart(NULL, 0, NULL, PLAIN);
    free(cart);
    cart = NULL;
    valid_history = false;
}
void retro_deinit(void)
{
    retro_unload_game();
    vfs = NULL;
    logger = NULL;
}
static unsigned blend_channel(uint8_t target, uint8_t prev, unsigned weight)
{
    unsigned mixed = (target * (4 - weight) + prev * weight) >> 2;
    /* Finish the fade if integer rounding would retain the old value forever. */
    return mixed == prev ? target : mixed;
}

void retro_run(void)
{
    static const unsigned ids[] = {RETRO_DEVICE_ID_JOYPAD_UP,    RETRO_DEVICE_ID_JOYPAD_DOWN,
                                   RETRO_DEVICE_ID_JOYPAD_LEFT,  RETRO_DEVICE_ID_JOYPAD_RIGHT,
                                   RETRO_DEVICE_ID_JOYPAD_A,     RETRO_DEVICE_ID_JOYPAD_B,
                                   RETRO_DEVICE_ID_JOYPAD_START, RETRO_DEVICE_ID_JOYPAD_SELECT};
    if (!cart)
        return;
    bool update = false;
    if (env(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &update) && update)
        options();
    poll();
    machine.joy = 255;
    for (unsigned i = 0; pad && i < 8; ++i)
        if (input(0, RETRO_DEVICE_JOYPAD, 0, ids[i]))
            machine.joy &= ~(1u << i);
    size_t samples = machine_frame(&machine, lcd, pcm);
    for (size_t i = 0; i < WIDTH * HEIGHT; ++i) {
        uint32_t c = colors[lcd[i]];
        if (ghost && valid_history && c != history[i]) {
            uint32_t prev = history[i];
            c = (blend_channel(c >> 16, prev >> 16, ghost) << 16) |
                (blend_channel(c >> 8, prev >> 8, ghost) << 8) |
                blend_channel(c, prev, ghost);
        }
        history[i] = c;
        if (rgb565)
            frame565[i] = (uint16_t)(((c >> 8) & 0xf800) | ((c >> 5) & 0x7e0) | ((c >> 3) & 31));
    }
    valid_history = true;
    video(rgb565 ? (void *)frame565 : (void *)history, WIDTH, HEIGHT, WIDTH * (rgb565 ? 2 : 4));
    if (batch)
        batch(pcm, samples);
    else if (audio)
        for (size_t i = 0; i < samples; ++i)
            audio(pcm[2 * i], pcm[2 * i + 1]);
}
void *retro_get_memory_data(unsigned id)
{
    return cart && id == RETRO_MEMORY_SYSTEM_RAM ? machine.ram : NULL;
}
size_t retro_get_memory_size(unsigned id)
{
    return cart && id == RETRO_MEMORY_SYSTEM_RAM ? sizeof(machine.ram) : 0;
}
unsigned retro_get_region(void) { return RETRO_REGION_NTSC; }
void retro_cheat_reset(void) {}
void retro_cheat_set(unsigned i, bool enabled, const char *code)
{
    (void)i;
    (void)enabled;
    (void)code;
}
bool retro_load_game_special(unsigned t, const struct retro_game_info *g, size_t n)
{
    (void)t;
    (void)g;
    (void)n;
    return false;
}
#include "state.inc"
