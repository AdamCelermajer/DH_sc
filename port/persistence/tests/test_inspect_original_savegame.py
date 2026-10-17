from __future__ import annotations

import importlib.util
from pathlib import Path
import struct
import tempfile
import unittest


REPO_ROOT = Path(__file__).resolve().parents[3]
TOOL_PATH = REPO_ROOT / "port" / "persistence" / "inspect_original_savegame.py"
SPEC = importlib.util.spec_from_file_location("inspect_original_savegame", TOOL_PATH)
assert SPEC is not None and SPEC.loader is not None
inspector = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(inspector)

CACHE_FILES = REPO_ROOT.parent / "cache" / "files"
MANIFEST = REPO_ROOT / "recovered" / "assets" / "cache-manifest.json"
PROFILE_NAME = "dh2_000.savegame"
LEVEL_NAME = "dh2_000_0_000_041_level.savegame"


class OriginalSaveInspectorTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        if not CACHE_FILES.is_dir() or not MANIFEST.is_file():
            raise unittest.SkipTest(
                "external recovered cache fixtures are unavailable; provide work/cache/files"
            )
        cls.temp = tempfile.TemporaryDirectory(prefix="dh2-save-fixtures-")
        cls.fixture_root = Path(cls.temp.name)
        cls.fixture_paths = inspector.copy_verified_fixtures(
            CACHE_FILES, MANIFEST, cls.fixture_root
        )
        cls.manifest_entries = inspector.load_manifest(MANIFEST)

    @classmethod
    def tearDownClass(cls) -> None:
        if hasattr(cls, "temp"):
            cls.temp.cleanup()

    def _entry(self, basename: str):
        matches = [
            (logical, entry)
            for logical, entry in self.manifest_entries.items()
            if inspector._relative_cache_path(logical).name == basename
        ]
        self.assertEqual(len(matches), 1, basename)
        return matches[0]

    def _fixture_copy(self, basename: str) -> Path:
        logical, _ = self._entry(basename)
        return self.fixture_root / inspector._relative_cache_path(logical)

    def test_all_manifest_save_fixtures_are_verified_copies(self) -> None:
        report = inspector.inspect_cache(self.fixture_root, MANIFEST)
        self.assertTrue(report["success"], report["errors"])
        self.assertEqual(report["errors"], [])
        self.assertEqual(len(report["files"]), 5)
        self.assertTrue(all(record["manifest_verified"] for record in report["files"]))
        names = {record["local_name"] for record in report["files"]}
        self.assertEqual(
            names,
            {
                "DebugSwitches.savegame",
                "dh2_settings.savegame",
                PROFILE_NAME,
                PROFILE_NAME + ".bak",
                LEVEL_NAME + ".bak",
            },
        )

    def test_verified_level_backup_directory_prefix(self) -> None:
        name = LEVEL_NAME + ".bak"
        logical, entry = self._entry(name)
        record = inspector.inspect_entry(logical, self._fixture_copy(name), entry)
        envelope = record["envelope"]
        self.assertEqual(envelope["format"], "observed-level-checkpoint-directory-prefix-v1")
        self.assertEqual(envelope["section_count"], 2)
        self.assertEqual([section["name"] for section in envelope["sections"]], ["INFO", "OBJS"])
        self.assertEqual(
            [section["opaque_descriptor_u32_le"] for section in envelope["sections"]], [41, 100]
        )
        self.assertTrue(
            all(section["descriptor_interpretation"].startswith("unclassified")
                for section in envelope["sections"])
        )
        self.assertEqual(envelope["opaque_body"]["offset"], 28)
        self.assertGreater(envelope["opaque_body"]["byte_length"], 0)
        self.assertFalse(envelope["opaque_body"]["payload_included"])

    def test_profile_saves_remain_opaque(self) -> None:
        logical, entry = self._entry(PROFILE_NAME)
        record = inspector.inspect_entry(logical, self._fixture_copy(PROFILE_NAME), entry)
        self.assertEqual(record["classification"], "opaque-original-save-file")
        self.assertEqual(record["opaque_payload"]["byte_length"], 9303)
        self.assertEqual(record["opaque_payload"]["offset"], 0)
        self.assertEqual(record["opaque_payload"]["sha256"], record["sha256"])

    def test_backup_comparison_does_not_claim_equivalence(self) -> None:
        report = inspector.inspect_cache(self.fixture_root, MANIFEST)
        comparisons = {item["backup"]: item for item in report["backup_comparisons"]}
        profile = comparisons[PROFILE_NAME + ".bak"]
        checkpoint = comparisons[LEVEL_NAME + ".bak"]
        self.assertTrue(profile["same_length"])
        self.assertFalse(profile["same_sha256"])
        self.assertGreater(profile["differing_byte_count"], 0)
        self.assertEqual(checkpoint["status"], "primary-not-in-verified-manifest")
        self.assertFalse(checkpoint["primary_read"])

    def test_unknown_section_is_reported_as_opaque(self) -> None:
        data = bytearray(self._fixture_copy(LEVEL_NAME + ".bak").read_bytes())
        # This is a test-only mutation of an authenticated temporary copy. The
        # original cache and manifest-verified fixture copy are untouched.
        data[8:12] = b"ZZZZ"
        parsed = inspector.parse_level_checkpoint_directory(bytes(data))
        unknown = parsed["sections"][0]
        self.assertEqual(unknown["name"], "ZZZZ")
        self.assertEqual(unknown["classification"], "unknown-section-label-body-opaque")
        self.assertEqual(unknown["opaque_descriptor_u32_le"], 41)
        self.assertEqual(len(unknown["opaque_descriptor_sha256"]), 64)
        self.assertGreater(parsed["opaque_body"]["byte_length"], 0)

    def test_bounds_checks_on_copied_level_fixture(self) -> None:
        original = self._fixture_copy(LEVEL_NAME + ".bak").read_bytes()
        with self.assertRaises(inspector.InspectionError):
            inspector.parse_level_checkpoint_directory(original[:27])
        with self.assertRaises(inspector.InspectionError):
            inspector.parse_level_checkpoint_directory(struct.pack("<I", 0xFFFFFFFF) + original[4:])
        overlong_count = struct.pack("<I", 129) + original[4:]
        with self.assertRaises(inspector.InspectionError):
            inspector.parse_level_checkpoint_directory(overlong_count)

    def test_hash_mismatch_fails_closed_on_temporary_copy(self) -> None:
        logical, entry = self._entry(PROFILE_NAME)
        damaged = self._fixture_copy(PROFILE_NAME).with_name("temporary-damaged-copy.savegame")
        data = bytearray(self._fixture_copy(PROFILE_NAME).read_bytes())
        data[0] ^= 1
        damaged.write_bytes(data)
        with self.assertRaisesRegex(inspector.InspectionError, "SHA-256 mismatch"):
            inspector.inspect_entry(logical, damaged, entry)


if __name__ == "__main__":
    unittest.main()
