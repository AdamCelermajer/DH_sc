#!/usr/bin/env python3
"""Cache-backed static-level integration and bounded failure regressions."""
import argparse
import ctypes as c
import importlib.util
import math
from pathlib import Path
import shutil
import sys
import tempfile
import unittest
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location("dh2_level_runtime_under_test", ROOT / "runtime.py")
runtime = importlib.util.module_from_spec(spec)
spec.loader.exec_module(runtime)
_parser = argparse.ArgumentParser(add_help=False)
_parser.add_argument("--library", type=Path, required=True)
_parser.add_argument("--cache-root", type=Path, required=True)
_args, _unknown = _parser.parse_known_args()
_library = _args.library
_cache = _args.cache_root
sys.argv = [sys.argv[0], *_unknown]


def close(actual, expected, tolerance=0.003):
    assert len(actual) == len(expected)
    assert all(math.isclose(float(a), float(b), rel_tol=1e-6, abs_tol=tolerance)
               for a, b in zip(actual, expected)), (actual, expected)


def get_object_attr(element, name):
    return element.attrib[name]


class RuntimeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.library = _library.resolve()
        cls.cache = _cache.resolve()
        cls.dll = runtime.bind_library(cls.library)

    def test_real_infected_village_closure_order_and_spawn(self):
        result = runtime.load_static_level(self.cache, "INFECTED_VILLAGE_01", self.library)
        self.assertEqual(result["catalogue_level_file"], "005_infectedvillage.mlx")
        self.assertEqual(result["module_count"], 2)
        self.assertEqual(result["source_records"][0]["gametype"], "LevelConfig")
        modules = result["modules"]
        self.assertEqual([module["module_index"] for module in modules], [0, 1])
        self.assertEqual([module["source_record"] for module in modules], [1, 2])
        self.assertTrue(all(module["mgp_loaded"] and module["mvp_loaded"] for module in modules))
        self.assertEqual([module["catalogue_node_id"] for module in modules], [
            "_module_infectedvillage_01-node", "_module_infectedvillage_02-node"])
        self.assertTrue(all(len(module["subtree_node_records"]) > 1 for module in modules))

        level_path = self.cache / "data/scene/005_infectedvillage.mlx"
        level_elements = ET.fromstring(level_path.read_bytes()).findall("GameObject")
        expected_order = [("data/scene/005_infectedvillage.mlx", index)
                          for index in range(len(level_elements))]
        for module_index, module in enumerate(modules):
            for key in ("mgp", "mvp"):
                relative = module[key]
                records = ET.fromstring((self.cache / relative).read_bytes()).findall("GameObject")
                expected_order.extend((relative, index) for index in range(len(records)))
        actual_order = [(record["source_path"], record["source_record"])
                        for record in result["source_records"]]
        self.assertEqual(actual_order, expected_order)
        self.assertEqual(result["source_record_count"], len(expected_order))

        for module_index, module in enumerate(modules):
            source_module = level_elements[module_index + 1]
            close(module["level_origin"], [float(x) for x in source_module.get("position").split(",")])
            self.assertEqual(module["dae"], "data/3d/modules/infectedvillage/infectedvillage.bdae")
            self.assertTrue(module["mgp"].endswith(f"infected0{module_index + 1}.mgp"))
            self.assertTrue(module["mvp"].endswith(f"infected0{module_index + 1}.mvp"))

        starts = result["entrypoint_id_0"]
        self.assertEqual(len(starts), 1)
        entry = starts[0]
        self.assertEqual(entry["name"], "_prim_EntryPoint")
        self.assertEqual(entry["gametype"], "SpawnPoint")
        self.assertEqual(entry["entrypoint_id"], 0)
        close(entry["local_position"], [900.85, -4148.52, 1348.88])
        close(entry["world_position"], [-2547.65, -1148.52, 1348.88])
        self.assertEqual(result["objects_activated"], False)
        self.assertEqual(result["scripts_executed"], False)
        self.assertEqual(result["gameplay_ready"], False)
        self.assertIn("data/3d/modules/infectedvillage/infectedvillage.bdae",
                      result["source_sha256"])

    def test_native_import_failure_preserves_existing_level(self):
        source = "data/scene/005_infectedvillage.mlx"
        raw = (self.cache / source).read_bytes()
        buffer = c.create_string_buffer(raw)
        level = runtime.Level()
        diagnostic = runtime.Diagnostic()
        result = self.dll.dh2_world_import_level(c.byref(level), b"INFECTED_VILLAGE_01",
            source.encode("ascii"), buffer, len(raw), c.byref(diagnostic))
        self.assertEqual(result, 0, diagnostic.message.decode(errors="replace"))
        try:
            previous = c.string_at(c.byref(level), c.sizeof(level))
            invalid = c.create_string_buffer(raw[:40])
            result = self.dll.dh2_world_import_level(c.byref(level), b"BROKEN",
                b"data/scene/broken.mlx", invalid, 40, c.byref(diagnostic))
            self.assertNotEqual(result, 0)
            self.assertEqual(c.string_at(c.byref(level), c.sizeof(level)), previous)
        finally:
            self.dll.dh2_world_free(c.byref(level))
        self.assertEqual(level.module_count, 0)

    def test_missing_module_file_fails_without_returning_partial_level(self):
        with tempfile.TemporaryDirectory(prefix="dh2-level-runtime-missing-") as temp:
            root = Path(temp)
            for relative in ("data/pydata/levels_pyarray.bin",
                             "data/pydata/levels_pyarraynames.bin",
                             "data/scene/005_infectedvillage.mlx"):
                destination = root / relative
                destination.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(self.cache / relative, destination)
            with self.assertRaisesRegex(runtime.LevelRuntimeError, "missing/unreadable cache file"):
                runtime.load_static_level(root, "INFECTED_VILLAGE_01", self.library)

    def test_module_record_scene_node_and_serialized_output_bounds(self):
        cases = [
            ({"max_modules": 1}, "modules; output bound"),
            ({"max_records": 4}, "source records exceed output bound"),
            ({"max_scene_nodes": 1}, "subtree"),
            ({"max_output_bytes": 128}, "serialized level output"),
        ]
        for bounds, message in cases:
            with self.subTest(bounds=bounds):
                with self.assertRaisesRegex(runtime.LevelRuntimeError, message):
                    runtime.load_static_level(self.cache, "INFECTED_VILLAGE_01",
                                              self.library, **bounds)


if __name__ == "__main__":
    unittest.main()
