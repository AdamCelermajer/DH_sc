#!/usr/bin/env python3
"""Host-only checks for the pinned Android infected-actor preview bundle."""
from __future__ import annotations

import importlib.util
import json
from pathlib import Path
import shutil
import tempfile
import unittest


REPO = Path(__file__).resolve().parents[3]
BUILD_SCRIPT = REPO / "port/android-app/build.py"
CACHE = REPO.parent / "cache/files"
SPEC = importlib.util.spec_from_file_location("dh2_android_build", BUILD_SCRIPT)
assert SPEC and SPEC.loader
BUILD = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(BUILD)


class ActorPreviewBundleTests(unittest.TestCase):
    def test_build_bundle_includes_only_pinned_models_compatible_clips_and_textures(self) -> None:
        with tempfile.TemporaryDirectory(prefix="dh2-actor-bundle-") as temporary:
            root = Path(temporary)
            assets, manifest = BUILD.infected_actor_preview_bundle(CACHE, root / "build")
            self.assertEqual(len(assets), 14)  # 13 source files and the generated manifest.
            self.assertEqual(len(manifest["models"]), 6)
            self.assertEqual(len(manifest["clips"]), 3)
            self.assertEqual(len(manifest["textures"]), 4)
            self.assertEqual(len(manifest["files"]), 13)
            self.assertEqual({row["state"] for row in manifest["clips"]}, {"Idle", "Walk"})
            self.assertEqual({row["path"] for row in manifest["files"] if row["role"] == "model"},
                             {row["path"] for row in manifest["models"]})
            self.assertEqual({row["path"] for row in manifest["files"] if row["role"] == "animation"},
                             {row["path"] for row in manifest["clips"]})
            for packaged, original in assets.items():
                if packaged.endswith("actor-preview-manifest.json"):
                    continue
                self.assertEqual(original.stat().st_size,
                                 next(row["size_bytes"] for row in manifest["files"]
                                      if packaged.endswith(row["path"])))

    def test_missing_cache_asset_fails_with_its_source_path(self) -> None:
        with tempfile.TemporaryDirectory(prefix="dh2-actor-missing-") as temporary:
            with self.assertRaisesRegex(FileNotFoundError, "Missing manifest-pinned actor preview cache file"):
                BUILD.infected_actor_preview_bundle(Path(temporary), Path(temporary) / "build")

    def test_modified_cache_asset_fails_its_manifest_hash(self) -> None:
        source_manifest = json.loads(BUILD.INFECTED_ACTOR_MANIFEST.read_text(encoding="utf-8"))
        selected = {row["bdae_path"] for row in source_manifest["template_choices"]["models"]}
        selected.update(row["path"] for row in BUILD.INFECTED_ACTOR_CLIPS)
        selected.update(BUILD.INFECTED_ACTOR_TEXTURE_PATHS)
        with tempfile.TemporaryDirectory(prefix="dh2-actor-tamper-") as temporary:
            cache = Path(temporary) / "cache"
            for relative in selected:
                destination = cache / Path(relative)
                destination.parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(CACHE / Path(relative), destination)
            damaged = cache / Path(BUILD.INFECTED_ACTOR_CLIPS[0]["path"])
            data = bytearray(damaged.read_bytes())
            data[-1] ^= 1
            damaged.write_bytes(data)
            with self.assertRaisesRegex(ValueError, "Actor preview cache file differs from pinned source hash"):
                BUILD.infected_actor_preview_bundle(cache, Path(temporary) / "build")


if __name__ == "__main__":
    unittest.main(verbosity=2)
