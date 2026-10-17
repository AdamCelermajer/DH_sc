#!/usr/bin/env python3
"""Synthetic regression tests; no original game assets are required."""
import io
from pathlib import Path
import struct
import tempfile
import unittest
import zipfile

from recover_cache import assemble, inspect_archive, ordered_parts


def make_zip(records, method=zipfile.ZIP_STORED):
    stream = io.BytesIO()
    with zipfile.ZipFile(stream, "w", compression=method) as archive:
        for name, content in records:
            archive.writestr(name, content)
    return stream.getvalue()


class CacheRecoveryTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)

    def tearDown(self):
        self.temp.cleanup()

    def archive(self, content):
        path = self.root / "input.zip"
        path.write_bytes(content)
        return path

    def test_parts_are_sorted_numerically_even_without_padding(self):
        parts = []
        for number in range(1, 12):
            part = self.root / f"sample.zip.part{number}"
            part.write_bytes(bytes([number]))
            parts.append(part)
        result = assemble(list(reversed(parts)), self.root / "assembled.zip")
        self.assertEqual((self.root / "assembled.zip").read_bytes(), bytes(range(1, 12)))
        self.assertEqual(result["size"], 11)

    def test_missing_duplicate_and_mixed_parts_are_rejected(self):
        for names in (("x.zip.part1", "x.zip.part3"),
                      ("x.zip.part1", "x.zip.part01"),
                      ("x.zip.part1", "y.zip.part2")):
            paths = [self.root / name for name in names]
            for path in paths:
                path.write_bytes(b"x")
            with self.assertRaises(ValueError):
                ordered_parts(paths)

    def test_complete_archive_is_crc_verified_and_extracted(self):
        data = make_zip([("dir/", b""), ("dir/data.txt", b"hello\n")], zipfile.ZIP_DEFLATED)
        result = inspect_archive(self.archive(data), self.root / "output")
        self.assertTrue(result["archive_complete"])
        self.assertEqual(result["verified_file_count"], 1)
        self.assertEqual((self.root / "output/dir/data.txt").read_bytes(), b"hello\n")

    def test_truncated_archive_only_writes_intact_prefix(self):
        data = make_zip([("first.txt", b"first"), ("second.wav", b"second" * 20)])
        with zipfile.ZipFile(io.BytesIO(data)) as archive:
            offset = archive.infolist()[1].header_offset
        payload_offset = offset + 30 + len("second.wav")
        result = inspect_archive(self.archive(data[:payload_offset + 7]), self.root / "output")
        self.assertFalse(result["archive_complete"])
        self.assertEqual(result["stop"]["reason"], "truncated_entry")
        self.assertEqual(result["verified_file_count"], 1)
        self.assertEqual((self.root / "output/first.txt").read_bytes(), b"first")
        self.assertFalse((self.root / "output/second.wav").exists())

    def test_wrong_crc_is_rejected_before_write(self):
        data = bytearray(make_zip([("file.txt", b"checked content")]))
        data[30 + len("file.txt")] ^= 1
        result = inspect_archive(self.archive(data), self.root / "output")
        self.assertEqual(result["stop"]["reason"], "crc_mismatch")
        self.assertEqual(result["verified_file_count"], 0)
        self.assertFalse((self.root / "output/file.txt").exists())

    def test_traversal_absolute_and_windows_paths_are_rejected(self):
        for path in ("../escaped.txt", "/escaped.txt", "a/../../escaped.txt", "a\\escaped.txt", "C:/escaped.txt"):
            data = make_zip([(path, b"x")])
            if "\\" in path:
                # zipfile normalizes backslashes on Windows before writing.
                data = data.replace(path.replace("\\", "/").encode(), path.encode())
            with self.subTest(path=path), self.assertRaises(ValueError):
                inspect_archive(self.archive(data), self.root / "output")
        self.assertFalse((self.root / "escaped.txt").exists())

    def test_preexisting_symlink_is_not_followed(self):
        output = self.root / "output"
        outside = self.root / "outside"
        output.mkdir()
        outside.mkdir()
        try:
            (output / "dir").symlink_to(outside, target_is_directory=True)
        except OSError as error:
            if getattr(error, "winerror", None) == 1314:
                self.skipTest("Windows account has no symlink privilege")
            raise
        with self.assertRaises(ValueError):
            inspect_archive(self.archive(make_zip([("dir/file.txt", b"x")])), output)
        self.assertFalse((outside / "file.txt").exists())

    def test_duplicate_paths_are_rejected(self):
        # zipfile warns about duplicate names but permits construction for this test.
        import warnings
        with warnings.catch_warnings():
            warnings.simplefilter("ignore", UserWarning)
            data = make_zip([("x.txt", b"one"), ("x.txt", b"two")])
        with self.assertRaises(ValueError):
            inspect_archive(self.archive(data))

    def test_size_limit_stops_before_decompression(self):
        data = make_zip([("large.txt", b"a" * 10000)], zipfile.ZIP_DEFLATED)
        result = inspect_archive(self.archive(data), max_entry_bytes=500)
        self.assertEqual(result["stop"]["reason"], "entry_size_limit")
        self.assertEqual(result["verified_file_count"], 0)

    def test_data_descriptor_flag_is_reported_without_guessing(self):
        data = bytearray(make_zip([("x.txt", b"x")]))
        struct.pack_into("<H", data, 6, 8)
        result = inspect_archive(self.archive(data))
        self.assertEqual(result["stop"]["reason"], "data_descriptor_unsupported")

    def test_text_and_nested_shader_export_have_provenance(self):
        shader = make_zip([("pass.glsl", b"void main() {}\n"), ("glsl.config", b"test\n")])
        source_root = self.root / "source"
        data = make_zip([("files/data/scene.mgp", b'<?xml version="1.0"?><Module/>'),
                         ("files/shaders.pak", shader), ("files/data/art.bdae", b"BRES\0\0")])
        result = inspect_archive(self.archive(data), source_root=source_root)
        self.assertEqual(len(result["source_records"]), 3)
        self.assertEqual((source_root / "files/shaders.pak.contents/pass.glsl").read_bytes(), b"void main() {}\n")
        self.assertEqual(result["source_records"][1]["nested_archive_path"], "pass.glsl")
        self.assertFalse((source_root / "files/data/art.bdae").exists())

    def test_nested_shader_path_traversal_is_rejected(self):
        data = make_zip([("files/shaders.pak", make_zip([("../escaped.glsl", b"void main() {}")]))])
        with self.assertRaises(ValueError):
            inspect_archive(self.archive(data), source_root=self.root / "source")
        self.assertFalse((self.root / "escaped.glsl").exists())


if __name__ == "__main__":
    unittest.main()
