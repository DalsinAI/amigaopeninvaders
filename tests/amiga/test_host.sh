#!/bin/sh
# Open Invaders for GFOS, host tests: the path checks, then every picture,
# sound, song and font in data/ read back by gfopenallegro's own code at
# 16 bits (fake acgame.library and music library).
#   tests/amiga/test_host.sh A4_DIR     (gfopenallegro/allegro4)
# Copyright (c) 2026 AmiCradle Team. GPL-2 or later, as Open Invaders.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
A4=${1:?gfopenallegro/allegro4 directory}
T=${TMPDIR:-/tmp}/oi-host-$$
mkdir -p "$T"
trap 'rm -rf "$T"' EXIT HUP INT TERM
python3 -m unittest discover -s "$ROOT/tests/amiga" -p 'test_paths.py'
CC=${HOSTCC:-cc}
# the host parts of the library (a4_dumb_gm.c is its Amiga music glue)
SRCS=$(ls "$A4"/src/a4_*.c | grep -v -e a4_os_amiga -e a4_amiga -e a4_dumb_gm)
FAKES="$A4/tests/fake_acgame.c"
if [ -f "$A4/tests/fake_music.c" ]; then FAKES="$FAKES $A4/tests/fake_music.c"; fi
# fonts need load_bitmap_font, songs aldumb.h (both in gfopenallegro #3)
DEFS=
if grep -q 'load_bitmap_font' "$A4/include/allegro.h"; then DEFS="$DEFS -DOI_FONTS"; fi
if [ -f "$A4/include/aldumb.h" ]; then DEFS="$DEFS -DOI_DUMB"; fi
"$CC" -std=gnu99 $DEFS -D_DEFAULT_SOURCE -w -O1 -I"$A4/include" -I"$A4/src" -I"$A4/tests" \
  $SRCS $FAKES "$ROOT/tests/amiga/test_data.c" -o "$T/test_data" -lm
"$T/test_data" "$ROOT"
echo "host tests: PASS"
