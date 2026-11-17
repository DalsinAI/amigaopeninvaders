"""Open Invaders for GFOS: the source changes the Amiga build needs, made
once and committed (kept here so they can be made again on a new upstream).
- The include blocks take upstream's Linux layout (headers/, config.h) on
  AmigaOS too; the Linux-only data paths ($HOME) stay Linux-only.
- "./data/..." becomes "data/...": on AmigaDOS "." is not the current
  drawer. The game runs in its own drawer (amiga/amiga_start.c).
- Screenshots go to data/ on AmigaOS (upstream: "." + "/name", and on
  AmigaDOS "/" is the parent drawer); upstream's '.' is also no string.
Run from the source root; prints how many places changed.
Copyright (c) 2026 AmiCradle Team. GPL-2 or later, as Open Invaders."""
import glob
import re

AMIGA = "defined(__amigaos__)"


def patch(text):
    text = re.sub(r'#ifdef ALLEGRO_LINUX(\s*\n\s*#include "(?:config\.h|headers/)[^"]*")',
                  r"#if defined(ALLEGRO_LINUX) || " + AMIGA + r"\1", text)
    text = re.sub(r'#ifndef ALLEGRO_LINUX(\s*\n\s*#include "\.\./include/)',
                  lambda m: "#if !defined(ALLEGRO_LINUX) && !" + AMIGA + m.group(1), text)
    text = text.replace('"./data/', '"data/')
    text = text.replace("\t\tstring filepath='.';",
                        '#ifdef __amigaos__\n\t\tstring filepath="data";\n#else\n'
                        '\t\tstring filepath=".";\n#endif')
    return text


def main():
    changed = 0
    for path in sorted(glob.glob("src/*.cc") + glob.glob("src/*.c") + glob.glob("headers/*.h")):
        with open(path, encoding="latin-1") as f:
            old = f.read()
        new = patch(old)
        if new != old:
            changed += 1
            with open(path, "w", encoding="latin-1") as f:
                f.write(new)
    print(changed)


if __name__ == "__main__":
    main()
