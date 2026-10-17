#!/usr/bin/env python3
"""Unit and supplied-cache verification for the bounded catalogue decoder."""
import importlib.util
from pathlib import Path
import struct
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.dont_write_bytecode = True


HERE = Path(__file__).resolve()
REPO = HERE.parents[3]
CACHE = REPO.parent / "cache" / "files"
MODULE_PATH = HERE.parents[1] / "catalogue.py"
spec = importlib.util.spec_from_file_location("dh2_level_catalogue", MODULE_PATH)
catalogue_module = importlib.util.module_from_spec(spec)
assert spec.loader is not None
sys.modules[spec.name] = catalogue_module
spec.loader.exec_module(catalogue_module)


class ReaderTests(unittest.TestCase):
    def test_truncated_header_and_record_are_rejected(self):
        with self.assertRaises(catalogue_module.CatalogueError):
            catalogue_module.decode_catalogue(b"\x00", b"")

    def test_oversized_table_count_is_rejected_before_iteration(self):
        payload = struct.pack("<I", 0xFFFFFFFF)
        with self.assertRaisesRegex(catalogue_module.CatalogueError, "safety limit"):
            catalogue_module.decode_names(payload)

    def test_oversized_string_is_rejected(self):
        payload = struct.pack("<II", 33, catalogue_module.MAX_STRING_BYTES + 1)
        with self.assertRaisesRegex(catalogue_module.CatalogueError, "string length"):
            catalogue_module.decode_names(payload)

    def test_invalid_utf8_is_rejected(self):
        payload = bytearray(struct.pack("<I", 33))
        payload += struct.pack("<I", 1) + b"\xff"
        with self.assertRaisesRegex(catalogue_module.CatalogueError, "UTF-8"):
            catalogue_module.decode_names(bytes(payload))

    def test_noncanonical_boolean_is_rejected(self):
        reader = catalogue_module.Reader(b"\x02", "fixture")
        with self.assertRaisesRegex(catalogue_module.CatalogueError, "invalid bool"):
            reader.boolean()

    def test_duplicate_names_are_rejected(self):
        payload = bytearray()
        for count in (33, 51):
            payload += struct.pack("<I", count)
            names = [f"name_{count}_{index}" for index in range(count)]
            if count == 33:
                names[:2] = ["same", "same"]
            for name in names:
                encoded = name.encode()
                payload += struct.pack("<I", len(encoded)) + encoded
        with self.assertRaisesRegex(catalogue_module.CatalogueError, "duplicate"):
            catalogue_module.decode_names(bytes(payload))

    def test_trailing_data_is_rejected(self):
        payload = bytearray()
        for count in (33, 51):
            payload += struct.pack("<I", count)
            for index in range(count):
                name = f"n{count}_{index}".encode()
                payload += struct.pack("<I", len(name)) + name
        payload += b"unexpected"
        with self.assertRaisesRegex(catalogue_module.CatalogueError, "trailing bytes"):
            catalogue_module.decode_names(bytes(payload))

    def test_level_file_traversal_and_unknown_format_are_rejected(self):
        for value in ("../escape.mlx", "nested/file.mlx", "C:/escape.mlx", "file.xml"):
            with self.subTest(value=value), self.assertRaises(catalogue_module.CatalogueError):
                catalogue_module.safe_level_path(value)

    def test_cache_reference_cannot_escape_root(self):
        with self.assertRaisesRegex(catalogue_module.CatalogueError, "unsafe cache path"):
            catalogue_module._cache_path(CACHE, "data/scene/../escape.mlx")

    def test_validate_cache_rejects_oversized_binary_source(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            pydata = root / "data" / "pydata"
            pydata.mkdir(parents=True)
            (pydata / "levels_pyarray.bin").write_bytes(b"x" * 9)
            with patch.object(catalogue_module, "MAX_BINARY_BYTES", 8):
                with self.assertRaisesRegex(
                    catalogue_module.CatalogueError, "levels_pyarray.bin: file size 9 exceeds 8 bytes"
                ):
                    catalogue_module.validate_cache(root)

    def test_parse_swamp_exits_rejects_oversized_scene_xml(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            scene = root / "data" / "scene" / "001_swamp.mlx"
            scene.parent.mkdir(parents=True)
            scene.write_bytes(b"x" * 9)
            with patch.object(catalogue_module, "MAX_XML_BYTES", 8):
                with self.assertRaisesRegex(
                    catalogue_module.CatalogueError, "SWAMP scene XML: file size 9 exceeds 8 bytes"
                ):
                    catalogue_module.parse_swamp_exits(root, catalogue_module.Catalogue((), ()))

    def test_parse_swamp_exits_rejects_oversized_module_xml(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            scene = root / "data" / "scene" / "001_swamp.mlx"
            scene.parent.mkdir(parents=True)
            scene.write_text(
                '<Level><GameObject gametype="Module" mgp="data/3d/modules/module.mgp" />'
                '</Level>', encoding="utf-8"
            )
            module = root / "data" / "3d" / "modules" / "module.mgp"
            module.parent.mkdir(parents=True)
            module.write_bytes(b"x" * 129)
            with patch.object(catalogue_module, "MAX_XML_BYTES", 128):
                with self.assertRaisesRegex(
                    catalogue_module.CatalogueError,
                    "SWAMP module XML: file size 129 exceeds 128 bytes"
                ):
                    catalogue_module.parse_swamp_exits(root, catalogue_module.Catalogue((), ()))


class SuppliedCacheTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        pydata = CACHE / "data" / "pydata"
        cls.catalogue = catalogue_module.decode_catalogue(
            (pydata / "levels_pyarray.bin").read_bytes(),
            (pydata / "levels_pyarraynames.bin").read_bytes(),
        )
        cls.exits = catalogue_module.parse_swamp_exits(CACHE, cls.catalogue)

    def test_all_native_rows_and_cache_level_files_validate(self):
        self.assertEqual(len(self.catalogue.fast_travel), 33)
        self.assertEqual(len(self.catalogue.levels), 51)
        catalogue_module.validate_cache_files(self.catalogue, CACHE)
        self.assertEqual(len(self.catalogue.levels_by_name), 51)
        self.assertEqual(len(self.catalogue.fast_travel_by_name), 33)

    def test_fast_travel_camp_is_swamp_entrypoint_one(self):
        camp = self.catalogue.fast_travel_by_name["a01_SWAMP_CAMP"]
        self.assertEqual((camp.level_name, camp.entrypoint_id), ("SWAMP", 1))

    def test_all_three_authored_swamp_exit_records_resolve(self):
        catalogue_module.validate_swamp_exits(self.exits)
        self.assertEqual(len(self.exits), 3)
        actual = {
            (row.module_index, Path(row.module_path).name, row.record_index):
                (row.target.level_name, row.target.declaration.level_file,
                 row.target.entrypoint_id, row.activation_condition)
            for row in self.exits
        }
        self.assertEqual(actual, {
            (4, "merchantcamp_ruins_swe_00.mgp", 13):
                ("SWAMP_02", "022_swamp2.rule.xml", 0, "IsAfter_Gothicus2Survivors"),
            (6, "deadend_brdwalk_w_00.mgp", 1):
                ("SWAMP_CAVE_WITCH_A", "002_swamp_witchcave.rule.xml", 0, None),
            (7, "bossroom_ruins_ns_.mgp", 7):
                ("DARKWOOD", "003_darkwood.mlx", 0, "IsAfter_Swamp_Escape"),
        })

    def test_rule_xml_targets_are_procedural_rule_inputs(self):
        swamp2 = self.catalogue.levels_by_name["SWAMP_02"]
        witch = self.catalogue.levels_by_name["SWAMP_CAVE_WITCH_A"]
        darkwood = self.catalogue.levels_by_name["DARKWOOD"]
        self.assertEqual(swamp2.file_kind, "procedural_rules")
        self.assertEqual(witch.file_kind, "procedural_rules")
        self.assertEqual(darkwood.file_kind, "static_mlx")
        self.assertTrue((CACHE / "data" / "scene" / swamp2.level_file).is_file())
        self.assertTrue((CACHE / "data" / "scene" / witch.level_file).is_file())

    def test_exit_destination_is_separate_from_fast_travel_unlock(self):
        witch = next(row for row in self.exits if row.target.level_name == "SWAMP_CAVE_WITCH_A")
        self.assertEqual(witch.target.entrypoint_id, 0)
        self.assertEqual(witch.unlock.fast_travel_name, "a01_SWAMP_CAMP")
        self.assertEqual(witch.unlock.destination.level_name, "SWAMP")
        self.assertEqual(witch.unlock.destination.entrypoint_id, 1)

    def test_command_line_validation_reports_scope_and_counts(self):
        report = catalogue_module.validate_cache(CACHE)
        self.assertIn("no level loading performed", report["scope"])
        self.assertEqual(report["fast_travel_count"], 33)
        self.assertEqual(report["level_count"], 51)
        self.assertEqual(len(report["swamp_exits"]), 3)


if __name__ == "__main__":
    unittest.main()
