/* Test the public frame callback with a synthetic machine and a memory VFS. */
#include "../src/core.c"
#define CHECK(test)                                                                                \
    do {                                                                                           \
        if (!(test)) {                                                                             \
            fprintf(stderr, "Failed at line %d: %s\n", __LINE__, #test);                           \
            exit(1);                                                                               \
        }                                                                                          \
    } while (0)
static const char *ini, *selected = "game", *strength = "off";
static uint32_t first;
static bool dirty = true;
static struct retro_vfs_file_handle *open_ini(const char *p, unsigned mode, unsigned hints)
{
    (void)mode;
    (void)hints;
    return ini && !strcmp(p, "test.ini") ? (struct retro_vfs_file_handle *)&first : NULL;
}
static int close_ini(struct retro_vfs_file_handle *f)
{
    (void)f;
    return 0;
}
static int64_t read_ini(struct retro_vfs_file_handle *f, void *buf, uint64_t n)
{
    (void)f;
    size_t len = strlen(ini);
    if (len > n)
        len = (size_t)n;
    memcpy(buf, ini, len);
    return (int64_t)len;
}
static bool environment(unsigned cmd, void *data)
{
    if (cmd == RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE) {
        *(bool *)data = dirty;
        dirty = false;
        return true;
    }
    if (cmd == RETRO_ENVIRONMENT_GET_VARIABLE) {
        struct retro_variable *v = (struct retro_variable *)data;
        v->value = !strcmp(v->key, "gamate_palette") ? selected : strength;
        return true;
    }
    return false;
}
static void present(const void *p, unsigned w, unsigned h, size_t pitch)
{
    CHECK(w == 160 && h == 150);
    if (rgb565) {
        CHECK(pitch == 320);
        first = *(const uint16_t *)p;
    } else {
        CHECK(pitch == 640);
        first = *(const uint32_t *)p;
    }
}
static void poll_input(void) {}
static int16_t press(unsigned p, unsigned d, unsigned i, unsigned id)
{
    (void)p;
    (void)d;
    (void)i;
    return id == RETRO_DEVICE_ID_JOYPAD_B;
}
int main(void)
{
    static struct retro_vfs_interface fs;
    fs.open = open_ini;
    fs.close = close_ini;
    fs.read = read_ini;
    vfs = &fs;
    env = environment;
    video = present;
    poll = poll_input;
    input = press;
    strcpy(preset, "test.ini");
    cart = (uint8_t *)calloc(1, 16384);
    CHECK(cart);
    // Jump to self at $e000.
    bios[0] = 0x4c;
    bios[2] = 0xe0;
    bios[0xffd] = 0xe0;
    machine_cart(cart, 16384, bios, PLAIN);
    machine_power(&machine);
    ini = "[palette]\r\nrgb0=000000\r\nrgb1=FFFFFF\r\nrgb2=101112\r\nrgb3=667788\r\n";
    retro_run();
    CHECK(first == 0 && machine.joy == 0xdf);
    strength = "medium";
    dirty = true;
    for (unsigned i = 0; i < 16384; i += 2)
        machine.vram[i] = 255;
    retro_run();
    CHECK(first == 0x7f7f7f);
    size_t size = retro_serialize_size();
    void *state = malloc(size);
    CHECK(state && retro_serialize(state, size));
    retro_run();
    CHECK(first == 0xbfbfbf);
    CHECK(retro_unserialize(state, size));
    retro_run();
    CHECK(first == 0xbfbfbf);
    strength = "off";
    dirty = true;
    rgb565 = true;
    retro_run();
    CHECK(first == 0xffff);
    rgb565 = false;
    ini = "[palette]\nrgb0=000001\nrgb1=123456\nrgb2=000003\nrgb3=000004\n";
    dirty = true;
    retro_run();
    CHECK(first == 0x123456);
    ini = "[palette]\nrgb0=000001\nrgb1=bad!\n";
    dirty = true;
    retro_run();
    CHECK(first == 0x437a63);
    retro_set_controller_port_device(0, RETRO_DEVICE_NONE);
    retro_run();
    CHECK(machine.joy == 255);
    ++bios_id;
    CHECK(!retro_unserialize(state, size));
    --bios_id;
    ++cart_id;
    CHECK(!retro_unserialize(state, size));
    --cart_id;
    free(state);
    retro_deinit();
    puts("Presentation, input and state tests passed");
    return 0;
}
