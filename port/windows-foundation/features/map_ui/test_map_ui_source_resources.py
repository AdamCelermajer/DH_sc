"""Check the actual MapSheet projection anchors in the original Droid SWF."""
import hashlib
import json
import os
from pathlib import Path
import struct
import unittest
import zlib


ROOT = Path(__file__).resolve().parents[4]
ASSETS = Path(os.environ.get("DH2_AUTHORED_UI_DIR", ROOT / ".local-inputs/ui-layout-discovery"))
SWF_SHA256 = "43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0"
EXPECTED_RENDER_MATRIX = [1.26568603515625, 0.0, 0.0, 1.182342529296875, 281, 982]


class SourceMapResourceTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        raw = (ASSETS / "dqcharmenu_droid.swf").read_bytes()
        if hashlib.sha256(raw).hexdigest() != SWF_SHA256:
            raise AssertionError("Map projection check requires the original authored Droid movie")
        cls.swf = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
        if raw[:3] not in (b"FWS", b"CWS") or len(cls.swf) != struct.unpack_from("<I", raw, 4)[0]:
            raise AssertionError("Invalid authored SWF header/declared length")
        cls.rows = json.loads((ASSETS / "dqcharmenu_droid-tree.json").read_text())
        cls.sprites = {}

        def collect(rows):
            for row in rows:
                if row["tag"] == 39:
                    if struct.unpack_from("<H", cls.swf, row["offset"])[0] != row["id"]:
                        raise AssertionError("Tree sprite definition differs from actual SWF")
                    cls.sprites[row["id"]] = row
                    collect(row["children"])

        collect(cls.rows)

    def test_original_map_page_camera_canvas_placement(self):
        sheet = self.sprites[655]
        placements = [row for row in sheet["children"] if row["tag"] in (26, 70)]
        by_name = {row.get("instance_name"): row for row in placements if row.get("instance_name")}
        self.assertEqual(by_name["RenderMap"]["character_id"], 601)
        self.assertEqual(by_name["RenderMap"]["matrix_twips"], EXPECTED_RENDER_MATRIX)
        # The source SWF stores this placement at this exact tag offset; the
        # tree contributes names/matrices only, never a drawn rectangle.
        placement = by_name["RenderMap"]
        raw_tag = self.swf[placement["offset"]:placement["offset"] + placement["length"]]
        self.assertIn(b"RenderMap\0", raw_tag)

    def test_source_dynamic_targets_and_controls_are_authored(self):
        sheet = self.sprites[655]
        placements = [row for row in sheet["children"] if row["tag"] in (26, 70)]
        by_name = {row.get("instance_name"): row for row in placements if row.get("instance_name")}
        self.assertEqual(by_name["MapName"]["character_id"], 617)
        self.assertEqual(by_name["iconsContainer"]["character_id"], 615)
        self.assertEqual(by_name["MapIconsDynamic"]["character_id"], 614)
        self.assertEqual(by_name["btn_Legend"]["character_id"], 623)
        self.assertEqual(by_name["btn_ResetZoom"]["character_id"], 623)
        for name in ("MapName", "iconsContainer", "MapIconsDynamic", "btn_Legend", "btn_ResetZoom"):
            row = by_name[name]
            raw_tag = self.swf[row["offset"]:row["offset"] + row["length"]]
            self.assertIn(name.encode() + b"\0", raw_tag)


if __name__ == "__main__":
    unittest.main()
