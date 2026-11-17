/* Open Invaders for GFOS: every picture and sound in data/ read through
 * gfopenallegro's own code (host build, fake acgame.library) at the
 * game's 16-bit colour depth, the way init.cc and intro.cc load them, and
 * the two PCX fonts as glyph-grid fonts.
 *   test_data GAME_DIR
 * Copyright (c) 2026 AmiCradle Team. GPL-2 or later, as Open Invaders. */
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <allegro.h>
#ifdef OI_DUMB
#include <aldumb.h>
#endif

static int fails, checks;
#define CHECK(c) do { checks++; if (!(c)) { fails++; printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); } } while (0)

static int ends_with(const char *s, const char *e)
{
    size_t a = strlen(s), b = strlen(e);
    return a >= b && strcmp(s + a - b, e) == 0;
}

int main(int argc, char **argv)
{
    char path[1024];
    DIR *dir;
    struct dirent *de;
    int pcx = 0, wav = 0, mods = 0;

    if (argc < 2) return 2;
    CHECK(allegro_init() == 0);
#ifdef OI_DUMB
    dumb_register_stdfiles();
#endif
    set_color_depth(16);               /* graphics.cc */
    snprintf(path, sizeof(path), "%s/data", argv[1]);
    dir = opendir(path);
    CHECK(dir != NULL);
    while (dir && (de = readdir(dir)) != NULL) {
        snprintf(path, sizeof(path), "%s/data/%s", argv[1], de->d_name);
        if (ends_with(de->d_name, ".pcx")) {
            BITMAP *b = load_bitmap(path, NULL);
            CHECK(b != NULL);
            if (!b) { printf("  cannot load %s\n", de->d_name); continue; }
            CHECK(b->w > 0 && b->h > 0);
            CHECK(bitmap_color_depth(b) == 16);
            destroy_bitmap(b);
            pcx++;
        } else if (ends_with(de->d_name, ".wav")) {
            SAMPLE *s = load_wav(path);
            CHECK(s != NULL);
            if (!s) { printf("  cannot load %s\n", de->d_name); continue; }
            CHECK(s->len > 0);
            destroy_sample(s);
            wav++;
        } else if (ends_with(de->d_name, ".mod")) {
            /* ProTracker: 31 instruments, "M.K." at 1080 */
            FILE *f = fopen(path, "rb");
            char tag[4] = {0};
            CHECK(f != NULL);
            if (f) { fseek(f, 1080, SEEK_SET); CHECK(fread(tag, 1, 4, f) == 4); fclose(f); }
            CHECK(memcmp(tag, "M.K.", 4) == 0 || memcmp(tag, "M!K!", 4) == 0 ||
                  memcmp(tag + 1, "CHN", 3) == 0 || memcmp(tag + 2, "CH", 2) == 0);
#ifdef OI_DUMB
            {   /* init.cc, intro.cc, ending.cc: dumb_load_mod_quick */
                DUH *d = dumb_load_mod_quick(path);
                CHECK(d != NULL);
                if (d) unload_duh(d);
            }
#endif
            mods++;
        }
    }
    if (dir) closedir(dir);
    CHECK(pcx >= 30 && wav >= 15 && mods == 4);
#ifdef OI_FONTS
    {   /* the fonts: load_font on a PCX glyph grid, as intro.cc does */
        FONT *f;
        snprintf(path, sizeof(path), "%s/data/arcade_font.pcx", argv[1]);
        f = load_font(path, NULL, NULL);
        CHECK(f != NULL);
        if (f) { CHECK(text_height(f) > 0); CHECK(text_length(f, "SCORE") > 0); destroy_font(f); }
        snprintf(path, sizeof(path), "%s/data/arcade_font_big.pcx", argv[1]);
        f = load_font(path, NULL, NULL);
        CHECK(f != NULL);
        if (f) { CHECK(text_height(f) > 0); destroy_font(f); }
        f = load_bitmap_font(path, NULL, NULL);    /* init.cc */
        CHECK(f != NULL);
        if (f) destroy_font(f);
    }
#else
    printf("fonts skipped: this gfopenallegro has no load_bitmap_font yet\n");
#endif
    printf("%d pictures, %d sounds, %d songs; %d checks, %d failed\n", pcx, wav, mods, checks, fails);
    return fails ? 1 : 0;
}
