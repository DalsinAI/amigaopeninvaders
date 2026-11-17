# Open Invaders for GFOS

Open Invaders 0.3 by Darryl LeCount (GPL-2 or later), built for
AmigaOS 3.2 / GFOS on a 68040, 68060 or 68080. No FPU is needed.

A Space Invaders style shooter: hold off waves of invaders across 15
levels with their own backgrounds and music, and unlock extras as your
high scores grow.

## What is in the drawer

- `OpenInvaders`: the game.
- `data/`: the pictures, sounds and music (ProTracker MODs). The game
  writes `hiscore.dat`, `config.dat` and its screenshots (Ctrl-S) here.
  Updates leave them alone.
- `Libs/allegro4.library` and `Libs/acgame.library`: Allegro 4 for GFOS
  (gfopenallegro) on ACGame. The game opens them from its own `Libs/`.

## How the port is made

- The code is upstream's, built as C++98 (as it was written in 2007) with
  68040 soft float against gfopenallegro's Allegro 4
  (`amiga/Makefile.amigaos`).
- The game draws at 800x600 in 16-bit colour with translucency;
  allegro4.library shows that on ACGame's screen. Its music goes through
  gfopenallegro's DUMB calls (`aldumb.h`), which play the MODs through
  gfopenmusic.library when it is installed and stay silent without it.
- What the game needed from Allegro and DUMB went into gfopenallegro, not
  the game: truecolour, aldumb, load_bitmap_font, make_trans_font,
  is_windowed_mode and the 15 to 32-bit bmp_read/bmp_write calls.
- `amiga/tools/amiga_paths.py` made the few source changes: upstream's
  Linux header layout on AmigaOS too, `"./data/"` as `"data/"` (on
  AmigaDOS `.` is not the current drawer), and screenshots into `data/`.
  The game moves into its own drawer (`PROGDIR:`) when it starts.

## Tests

`tests/amiga/test_host.sh` checks that every file the code opens is in
`data/` and that the source changes are in place, then reads every
picture (at 16 bits), sound, song and both fonts through gfopenallegro's
own code. `package/build-amiga.sh` runs it before each build.
