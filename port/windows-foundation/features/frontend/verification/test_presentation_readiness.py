#!/usr/bin/env python3
"""Focused source/asset checks for the MainMenu-vs-intro readiness receipt."""
from hashlib import sha256
import json
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[5]
FRONT = ROOT / "port/windows-foundation/features/frontend"


class PresentationReadinessTest(unittest.TestCase):
    def test_main_menu_and_title_intro_are_distinct_original_assets(self):
        evidence = json.loads((FRONT / "verification/evidence.json").read_text())
        receipt = evidence["presentation_readiness"]
        main = ROOT / "port/android-native/app/src/main/assets/models/main_menu_charactere_swamp.bdae"
        intro = ROOT / "port/android-native/app/src/main/assets/original-media/intro.mp4"
        self.assertEqual(sha256(main.read_bytes()).hexdigest(), receipt["main_menu_scene"]["sha256"])
        self.assertEqual(sha256(intro.read_bytes()).hexdigest(), receipt["title_intro_movie"]["sha256"])
        cinematic = ROOT / ".local-inputs/referenceframes/dh2-act1/at-0120s.png"
        self.assertEqual(sha256(cinematic.read_bytes()).hexdigest(), receipt["act1_opening_swamp_cinematic"]["frame_sha256"])
        setup = (ROOT / ".local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0042/0042c510.c").read_text()
        self.assertIn('"data/3d/menu/main_menu_charactere_swamp.bdae"', setup)
        self.assertIn("constructScene", setup)
        self.assertIn('"original-media/intro.mp4"', (ROOT / "port/android-native/app/src/main/java/com/example/dh2/CinematicActivity.java").read_text())

    def test_preview_stays_fail_closed_at_real_owner_boundaries(self):
        interactive = (FRONT / "interactive.cpp").read_text()
        self.assertIn('menu=="menu_MainMenu"||menu=="menu_StartGame")renderer.draw(mainScene.mesh)', interactive)
        self.assertNotIn("set_idle_transition_provider", interactive)
        self.assertNotIn("SourceLightPointOwnerV1", interactive)
        self.assertIn("source-showcase-end-held-idle-owner-required", (FRONT / "preview/creation_preview.cpp").read_text())
        self.assertIn("Required SAME authored scene light", (FRONT / "preview/class_light_source.cpp").read_text())

    def test_class_selection_camera_aspect_is_authored_four_thirds(self):
        scene = (FRONT / "preview/class_preview_scene.cpp").read_text()
        tests = (FRONT / "preview/creation_preview_tests.cpp").read_text()
        header = (FRONT / "preview/creation_preview.hpp").read_text()
        self.assertIn("const std::uint32_t aspectBits=0x3faaaaab", scene)
        self.assertIn("aspectRatio==float(4.0/3.0)", tests)
        self.assertNotIn("aspectRatio=0", header)
        self.assertIn("fixed at 4:3", header)


if __name__ == "__main__":
    unittest.main()
