"""Open Invaders for GFOS: every data file the code opens is in data/
under that name, no path starts with "./" (on AmigaDOS "." is not the
current drawer) and the drawer the Makefile packs holds them all.
Copyright (c) 2026 AmiCradle Team. GPL-2 or later, as Open Invaders."""
import glob
import os
import re
import sys
import unittest

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "amiga", "tools"))
import amiga_paths  # noqa: E402


def sources():
    out = {}
    for p in sorted(glob.glob(os.path.join(ROOT, "src", "*.cc"))):
        with open(p, encoding="latin-1") as f:
            out[os.path.basename(p)] = f.read()
    return out


class Paths(unittest.TestCase):
    def test_no_dot_slash(self):
        for name, text in sources().items():
            self.assertNotIn('"./', text, name)

    def test_literal_files_exist(self):
        seen = 0
        for name, text in sources().items():
            for lit in re.findall(r'"data/([^"]+\.(?:pcx|wav|mod))"', text):
                seen += 1
                self.assertTrue(os.path.isfile(os.path.join(ROOT, "data", lit)), lit)
        self.assertGreaterEqual(seen, 10)

    def test_loader_names_exist(self):
        # oi_load_graphic("x.pcx", ...) and oi_load_sfx("x.wav", ...) add data/
        seen = 0
        for name, text in sources().items():
            for lit in re.findall(r'oi_load_(?:graphic|sfx)\("([^"]+)"', text):
                seen += 1
                self.assertTrue(os.path.isfile(os.path.join(ROOT, "data", lit)), lit)
        self.assertGreaterEqual(seen, 20)

    def test_patch_is_applied_and_stable(self):
        for name, text in sources().items():
            self.assertEqual(amiga_paths.patch(text), text, name)

    def test_patch_on_upstream_text(self):
        up = ('#ifdef ALLEGRO_LINUX\n#include "config.h"\n#endif\n'
              '#ifndef ALLEGRO_LINUX\n#include "../include/pmask.h"\n#endif\n'
              'x=load_pcx("./data/a.pcx",NULL);\n'
              "\t\tstring filepath='.';\n")
        out = amiga_paths.patch(up)
        self.assertIn("#if defined(ALLEGRO_LINUX) || defined(__amigaos__)\n#include \"config.h\"", out)
        self.assertIn("#if !defined(ALLEGRO_LINUX) && !defined(__amigaos__)\n#include \"../include/", out)
        self.assertIn('"data/a.pcx"', out)
        self.assertIn('string filepath="data";', out)
        self.assertNotIn("'.'", out)


if __name__ == "__main__":
    unittest.main()
