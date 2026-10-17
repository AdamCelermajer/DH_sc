#!/usr/bin/env python3
"""Deterministic closure, staging, and guard tests for asset_bundle.py."""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import sys
import tempfile
from unittest import mock
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location("dh2_asset_bundle_under_test", ROOT / "asset_bundle.py")
bundle = importlib.util.module_from_spec(spec)
spec.loader.exec_module(bundle)
_parser = argparse.ArgumentParser(add_help=False)
_parser.add_argument("--library", type=Path, required=True)
_parser.add_argument("--cache-root", type=Path, required=True)
_parser.add_argument("--scene-library", type=Path,
                     default=bundle.SCENE_DRAW_LIBRARY)
_args, _unknown = _parser.parse_known_args()
LIBRARY = _args.library.resolve()
CACHE = _args.cache_root.resolve()
SCENE_LIBRARY = _args.scene_library.resolve()
sys.argv = [sys.argv[0], *_unknown]


class AssetBundleTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = bundle.runtime.load_static_level(
            CACHE, "INFECTED_VILLAGE_01", LIBRARY)
        cls.material_assets, cls.material_report = bundle.collect_material_assets(
            CACHE, cls.report, SCENE_LIBRARY)
        cls.closure = bundle.collect_closure(cls.report, extra_assets=cls.material_assets)

    def test_bdae_sampler_paths_resolve_to_exact_texture_cache_files(self):
        paths = {item["path"] for item in self.material_assets}
        self.assertEqual(paths, {
            "data/3d/textures/env_infectedvillage.tga",
            "data/3d/textures/env_infectedvillage_spec.tga",
            "data/3d/textures/pvr2_env_infectedvillage_alpha.tga",
            "data/3d/textures/fx_smoke_03.tga",
            "data/3d/textures/atlas_fx_particles_001.tga",
            "data/3d/textures/fx_magic_lenz_flares_008.tga",
            "data/3d/textures/fx_spark_01.tga",
        })
        for relative in paths:
            self.assertTrue(bundle._source_file(CACHE, relative).is_file())
        by_scene = {item["source_bdae"]: item
                    for item in self.material_report["scenes"]}
        module = by_scene["data/3d/modules/infectedvillage/infectedvillage.bdae"]
        self.assertEqual(module["draw_commands_in_scope"], 20)
        self.assertEqual(module["module_subtree_node_count"], 25)
        fog = by_scene["data/3d/animateddecors/fog/fog_infected_village.bdae"]
        self.assertEqual(fog["draw_commands_in_scope"], 0)
        self.assertEqual(fog["material_only_sampler_count"], 1)
        self.assertEqual(fog["unique_resolved_textures"],
                         ["data/3d/textures/fx_smoke_03.tga"])

    def test_material_texture_provenance_distinguishes_draw_bound_and_material_only(self):
        by_path = {item["path"]: item for item in self.material_assets}
        module_ref = by_path["data/3d/textures/env_infectedvillage.tga"]["references"][0]
        self.assertEqual(module_ref["source_path"],
                         "q:/data/iphone/3d/textures/env_infectedvillage.tga")
        self.assertEqual(module_ref["node_id"], "_mesh_alpha_0sfv-node")
        self.assertEqual([item["module_index"] for item in module_ref["module_context"]], [0])
        self.assertEqual(module_ref["primitive_material_id"], module_ref["material_id"])
        fog_ref = by_path["data/3d/textures/fx_smoke_03.tga"]["references"][0]
        self.assertIsNone(fog_ref["node_id"])
        self.assertIsNone(fog_ref["geometry_id"])
        self.assertEqual(fog_ref["source_bdae"],
                         "data/3d/animateddecors/fog/fog_infected_village.bdae")
        self.assertEqual(self.material_report["source_sampler_count"], 12)
        self.assertEqual(len(self.material_report["unresolved_sampler_references"]), 3)
        dust = by_path["data/3d/textures/fx_magic_lenz_flares_008.tga"]["references"][0]
        self.assertIsNone(dust["node_id"])
        self.assertEqual(dust["material_id"], "wiimaterial_shader")

    def test_bres_source_root_mapping_is_exact_and_traversal_safe(self):
        self.assertEqual(bundle._bres_texture_path(
            "q:/data/iphone/3d/textures/FX_smoke_03.tga"),
            "data/3d/textures/fx_smoke_03.tga")
        with self.assertRaisesRegex(bundle.AssetBundleError, "unrecognized BRES image source root"):
            bundle._bres_texture_path("c:/Windows/system.ini")
        with self.assertRaisesRegex(bundle.AssetBundleError, "unsafe cache asset path"):
            bundle._bres_texture_path("q:/data/iphone/../../outside.tga")

    def test_bdae_draw_and_sampler_caps_fail_closed(self):
        with mock.patch.object(bundle, "MAX_DRAW_COMMANDS", 1):
            with self.assertRaisesRegex(bundle.AssetBundleError, "static draw traversal failed"):
                bundle.collect_material_assets(CACHE, self.report, SCENE_LIBRARY)
        with mock.patch.object(bundle, "MAX_MATERIAL_SAMPLERS", 1):
            with self.assertRaisesRegex(bundle.AssetBundleError, "sampler count exceeds bound"):
                bundle.collect_material_assets(CACHE, self.report, SCENE_LIBRARY)

    def test_lightsets_and_script_ids_have_explicit_resolution_limits(self):
        entries = bundle.collect_closure(self.report)
        lightsets = bundle.audit_lightsets(CACHE, entries)
        self.assertEqual(lightsets["observed_nested_path_count"], 0)
        self.assertEqual({item["path"] for item in lightsets["files"]}, {
            "data/3d/light/infected.lightset_xml",
            "data/3d/light/infected_es1_1.lightset_xml",
        })
        scripts = bundle.audit_script_resources(CACHE, entries, self.report)
        names = {item["resolved_name"] for item in scripts["resolved_exec_script_calls"]}
        self.assertEqual(names, {"BeginScriptedCutScene", "EndScriptedCutScene"})
        self.assertTrue(scripts["unresolved_numeric_resources"])

    def test_real_closure_contains_direct_modules_visuals_scripts_and_lightsets(self):
        paths = {item["path"] for item in self.closure}
        expected = {
            "data/pydata/levels_pyarray.bin",
            "data/pydata/levels_pyarraynames.bin",
            "data/scene/005_infectedvillage.mlx",
            "data/3d/modules/infectedvillage/mgp/infected01.mgp",
            "data/3d/modules/infectedvillage/mgp/infected02.mgp",
            "data/3d/modules/infectedvillage/mvp/infected01.mvp",
            "data/3d/modules/infectedvillage/mvp/infected02.mvp",
            "data/3d/modules/infectedvillage/infectedvillage.bdae",
            "data/3d/animateddecors/fog/fog_infected_village.bdae",
            "data/3d/animateddecors/swamp_caveentrance_effect.bdae",
            "data/pydata/scripts/005_infectedvillage_pyscriptnames.bin",
            "data/pydata/scripts/005_infectedvillage_pyscripts.bin",
            "data/pydata/scripts_pyscriptnames.bin",
            "data/pydata/scripts_pyscripts.bin",
            "data/3d/light/infected.lightset_xml",
            "data/3d/light/infected_es1_1.lightset_xml",
        }
        self.assertTrue(expected.issubset(paths), expected - paths)
        self.assertNotIn("data/iphone/3d/modules/infectedvillage/infectedvillage.max", paths)
        roles = {item["path"]: set(item["roles"]) for item in self.closure}
        self.assertIn("visual-scene", roles["data/3d/animateddecors/fog/fog_infected_village.bdae"])
        self.assertIn("level-script-name-table",
                      roles["data/pydata/scripts/005_infectedvillage_pyscriptnames.bin"])
        self.assertIn("common-script-program-table", roles["data/pydata/scripts_pyscripts.bin"])
        self.assertIn("fixed-lightset", roles["data/3d/light/infected_es1_1.lightset_xml"])
        self.assertTrue(all(item["references"] for item in self.closure))

    def test_copy_manifest_is_reproducible_and_contents_match(self):
        with tempfile.TemporaryDirectory(prefix="dh2-asset-bundle-test-") as temporary:
            root = Path(temporary)
            first = root / "bundle-one"
            second = root / "bundle-two"
            manifest_one = bundle.build_bundle(CACHE, "INFECTED_VILLAGE_01", LIBRARY, first)
            manifest_two = bundle.build_bundle(CACHE, "INFECTED_VILLAGE_01", LIBRARY, second)
            bytes_one = (first / "asset-manifest.json").read_bytes()
            bytes_two = (second / "asset-manifest.json").read_bytes()
            self.assertEqual(bytes_one, bytes_two)
            self.assertEqual(manifest_one, manifest_two)
            self.assertGreater(manifest_one["file_count"], 10)
            self.assertTrue(manifest_one["unresolved_dependencies"])
            self.assertEqual(manifest_one["script_table_validation"], [
                {"scope": "common", "names_path": "data/pydata/scripts_pyscriptnames.bin",
                 "programs_path": "data/pydata/scripts_pyscripts.bin", "script_count": 15,
                 "command_count": 81,
                 "validated_by": "port/pydata-scripts/pydata_scripts.py"},
                {"scope": "level", "names_path": "data/pydata/scripts/005_infectedvillage_pyscriptnames.bin",
                 "programs_path": "data/pydata/scripts/005_infectedvillage_pyscripts.bin",
                 "script_count": 6, "command_count": 36,
                 "validated_by": "port/pydata-scripts/pydata_scripts.py"},
            ])
            self.assertEqual(manifest_one["material_texture_resolution"]["unique_texture_count"], 7)
            self.assertEqual(manifest_one["lightset_audit"]["observed_nested_path_count"], 0)
            self.assertEqual(manifest_one["script_resource_audit"]["common_script_count"], 15)
            for entry in manifest_one["files"]:
                copied = first.joinpath(*Path(entry["path"]).parts)
                data = copied.read_bytes()
                self.assertEqual(len(data), entry["size_bytes"])
                self.assertEqual(hashlib.sha256(data).hexdigest(), entry["sha256"])
            loaded = json.loads(bytes_one)
            self.assertEqual(loaded["format"], "dh2-source-asset-bundle-v1")

    def test_traversal_is_rejected_before_copy(self):
        malformed = {
            "source_sha256": {"data/scene/005_infectedvillage.mlx": "0" * 64},
            "source_records": [{"source_path": "data/scene/a.mlx", "source_record": 0,
                "name": "bad", "fields": {"scriptFile": "data/../../escape.pyscript"}}],
        }
        with self.assertRaisesRegex(bundle.AssetBundleError, "unsafe cache asset path"):
            bundle.collect_closure(malformed)

    def test_count_and_total_byte_guards_do_not_publish_partial_bundle(self):
        with tempfile.TemporaryDirectory(prefix="dh2-asset-bundle-guard-") as temporary:
            root = Path(temporary)
            stage_count = root / "too-many"
            with self.assertRaisesRegex(bundle.AssetBundleError, "closure has .* files; limit"):
                bundle.build_bundle(CACHE, "INFECTED_VILLAGE_01", LIBRARY, stage_count,
                                   max_files=1)
            self.assertFalse(stage_count.exists())

            stage_bytes = root / "too-large-total"
            with self.assertRaisesRegex(bundle.AssetBundleError, "exceed.*bundle byte bound|exceeded total bundle bound"):
                bundle.build_bundle(CACHE, "INFECTED_VILLAGE_01", LIBRARY, stage_bytes,
                                   max_total_bytes=1)
            self.assertFalse(stage_bytes.exists())

    def test_existing_destination_is_never_overwritten(self):
        with tempfile.TemporaryDirectory(prefix="dh2-asset-bundle-existing-") as temporary:
            stage = Path(temporary) / "existing"
            stage.mkdir()
            marker = stage / "keep.txt"
            marker.write_text("keep", encoding="utf-8")
            with self.assertRaisesRegex(bundle.AssetBundleError, "already exists"):
                bundle.build_bundle(CACHE, "INFECTED_VILLAGE_01", LIBRARY, stage)
            self.assertEqual(marker.read_text(encoding="utf-8"), "keep")

    def test_per_file_bound_is_checked_before_staging(self):
        with tempfile.TemporaryDirectory(prefix="dh2-asset-bundle-file-limit-") as temporary:
            stage = Path(temporary) / "file-limit"
            with self.assertRaisesRegex(bundle.AssetBundleError, "per-file bound"):
                bundle.build_bundle(CACHE, "INFECTED_VILLAGE_01", LIBRARY, stage,
                                   max_file_bytes=1)
            self.assertFalse(stage.exists())

    def test_cache_symlink_is_rejected(self):
        with tempfile.TemporaryDirectory(prefix="dh2-asset-bundle-link-") as temporary:
            root = Path(temporary) / "cache"
            root.mkdir()
            data = root / "data"
            data.mkdir()
            link = data / "linked.bin"
            with mock.patch.object(Path, "is_symlink", autospec=True,
                                   side_effect=lambda path: path == link):
                with self.assertRaisesRegex(bundle.AssetBundleError, "symbolic link"):
                    bundle._source_file(root.resolve(), "data/linked.bin")

    def test_failed_copy_cleans_private_temporary_directory(self):
        with tempfile.TemporaryDirectory(prefix="dh2-asset-bundle-cleanup-") as temporary:
            root = Path(temporary)
            stage = root / "too-large-total"
            with self.assertRaises(bundle.AssetBundleError):
                bundle.build_bundle(CACHE, "INFECTED_VILLAGE_01", LIBRARY, stage,
                                   max_total_bytes=1)
            self.assertFalse(stage.exists())
            self.assertEqual(list(root.glob(".too-large-total.bundle-*")), [])


if __name__ == "__main__":
    unittest.main()
