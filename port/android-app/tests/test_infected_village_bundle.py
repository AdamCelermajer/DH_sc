#!/usr/bin/env python3
"""Verify the Android preview selects only finalized, cache-hashed inputs."""

from __future__ import annotations

import importlib.util
import json
from pathlib import Path
import tempfile
import unittest


APP = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location('dh2_android_build', APP / 'build.py')
assert SPEC and SPEC.loader
BUILD_MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(BUILD_MODULE)


class InfectedVillageBundleTest(unittest.TestCase):
    def test_cache_selection_matches_locked_manifest(self) -> None:
        cache = APP.parent.parent.parent / 'cache' / 'files'
        with tempfile.TemporaryDirectory(prefix='dh2-infected-preview-') as temp:
            output = Path(temp)
            assets, report = BUILD_MODULE.infected_village_bundle(cache.resolve(), output)
            self.assertEqual(report['manifest_sha256'],
                BUILD_MODULE.INFECTED_VILLAGE_MANIFEST_SHA256)
            self.assertEqual(report['packaged_source_asset_count'], 9)
            self.assertEqual(report['unresolved_module_sampler_count'], 3)
            self.assertEqual(set(report['sampler_textures']),
                BUILD_MODULE.INFECTED_VILLAGE_TEXTURE_PATHS)
            self.assertEqual(len(assets), 10)  # Nine source files plus one manifest.
            embedded = Path(assets['assets/dh2/infectedvillage/asset-manifest.json'])
            manifest = json.loads(embedded.read_text(encoding='utf-8'))
            self.assertEqual(manifest['level_name'], 'INFECTED_VILLAGE_01')
            self.assertEqual(manifest['module_count'], 2)
            self.assertTrue(all(path.startswith('assets/dh2/infectedvillage/')
                                for path in assets))


if __name__ == '__main__':
    unittest.main()
