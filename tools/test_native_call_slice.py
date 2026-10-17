#!/usr/bin/env python3
"""Small synthetic graph test for native_call_slice.py."""

import json
import tempfile
import unittest
from pathlib import Path

from native_call_slice import analyze_slice


class NativeCallSliceTest(unittest.TestCase):
    def test_shortest_path_callers_filter_and_snippet(self):
        with tempfile.TemporaryDirectory() as temporary:
            decomp = Path(temporary)
            source = (
                "void _ZA() {\n  _ZB();\n  _ZC();\n}\n"
                "void _ZB() {\n  _ZD();\n}\n"
                "void _ZC() {\n  _ZD();\n}\n"
                "void _ZD() {\n  return;\n}\n"
            ).encode("utf-8")
            (decomp / "functions-000.pseudo.c").write_bytes(source)
            rows = []
            cursor = 0
            for name, address, body in (
                ("_ZA", "1000", b"void _ZA() {\n  _ZB();\n  _ZC();\n}\n"),
                ("_ZB", "1010", b"void _ZB() {\n  _ZD();\n}\n"),
                ("_ZC", "1020", b"void _ZC() {\n  _ZD();\n}\n"),
                ("_ZD", "1030", b"void _ZD() {\n  return;\n}\n"),
            ):
                offset = source.index(body, cursor)
                cursor = offset + len(body)
                rows.append({
                    "elf_address": address,
                    "name": name,
                    "success": True,
                    "has_warning": False,
                    "file": "functions-000.pseudo.c",
                    "byte_offset": offset,
                    "byte_length": len(body),
                })
            (decomp / "function-index.jsonl").write_text(
                "\n".join(json.dumps(row) for row in rows) + "\n",
                encoding="utf-8",
            )

            full = analyze_slice(decomp, ["_ZA"], max_depth=2)
            self.assertEqual(full["reachable_unique_count"], 4)
            by_name = {row["name"]: row for row in full["functions"]}
            self.assertEqual(by_name["_ZD"]["depth"], 2)
            self.assertEqual([step["name"] for step in by_name["_ZD"]["shortest_path"]["functions"]],
                             ["_ZA", "_ZB", "_ZD"])
            self.assertEqual({item["name"] for item in by_name["_ZD"]["direct_callers"]},
                             {"_ZB", "_ZC"})
            self.assertIn("_ZB();", by_name["_ZA"]["decompiler"]["snippet"])

            filtered = analyze_slice(decomp, ["_ZA"], max_depth=2,
                                     name_prefixes=["_ZB"])
            self.assertEqual(filtered["reachable_unique_count"], 4)
            self.assertEqual({row["name"] for row in filtered["functions"]}, {"_ZA", "_ZB"})

            limited = analyze_slice(decomp, ["_ZA"], max_depth=2, edge_limit=1)
            d_row = next(row for row in limited["functions"] if row["name"] == "_ZD")
            self.assertEqual(d_row["direct_callers_total"], 2)
            self.assertEqual(len(d_row["direct_callers"]), 1)
            self.assertTrue(d_row["direct_callers_truncated"])


if __name__ == "__main__":
    unittest.main()
