"""Evidence and contract checks for the source Map page projection leaf."""
from pathlib import Path
import hashlib
import json
import os
import struct
import unittest
import zlib

ROOT = Path(__file__).resolve().parents[4]
UI = Path(os.environ.get("DH2_AUTHORED_UI_DIR", ROOT / ".local-inputs/ui-layout-discovery"))
SOURCE = ROOT / "port/windows-foundation/features/map"
SWF_SHA256 = "43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0"


class RuntimeSourceMapPageTest(unittest.TestCase):
    def test_map_page_uses_actual_original_sheet_and_projection_regions(self):
        raw = (UI / "dqcharmenu_droid.swf").read_bytes()
        self.assertEqual(hashlib.sha256(raw).hexdigest(), SWF_SHA256)
        rows = json.loads((UI / "dqcharmenu_droid-tree.json").read_text())
        sprites = {}
        def visit(items):
            for item in items:
                if item["tag"] == 39:
                    sprites[item["id"]] = item
                    visit(item["children"])
        visit(rows)
        self.assertIn(655, sprites)
        raw_uncompressed = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
        self.assertEqual(len(raw_uncompressed), struct.unpack_from("<I", raw, 4)[0])
        sheet = sprites[655]
        names = {item.get("instance_name"): item for item in sheet["children"]
                 if item["tag"] in (26, 70) and item.get("instance_name")}
        self.assertEqual(names["RenderMap"]["character_id"], 601)
        self.assertEqual(names["RenderMap"]["matrix_twips"],
                         [1.26568603515625, 0.0, 0.0, 1.182342529296875, 281, 982])
        self.assertEqual(names["MapIconsDynamic"]["character_id"], 614)
        self.assertEqual(names["btn_Legend"]["character_id"], 623)
        self.assertEqual(names["btn_ResetZoom"]["character_id"], 623)

    def test_page_routes_only_authored_actions_and_live_source_markers(self):
        header = (SOURCE / "runtime_source_map_page_v1.hpp").read_text()
        source = (SOURCE / "runtime_source_map_page_v1.cpp").read_text()
        authored = (ROOT / "port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt").read_text()
        self.assertIn("PageInputV1 { none, legend, reset_zoom }", header)
        self.assertIn("collect_markers", header)
        self.assertIn("source_map_project_runtime_frame_v1", source)
        self.assertIn("same_level(owner, after)", source)
        self.assertIn("same_retained_resource", source)
        self.assertIn('"btn_Legend"', source)
        self.assertIn('"btn_ResetZoom"', source)
        self.assertNotIn("marker_hit", source)
        map_action_start = authored.index('0002a00d decl_dict')
        map_action_end = authored.index('0002a668 push_data', map_action_start)
        map_action = authored[map_action_start:map_action_end]
        self.assertIn("NativeShowMinimapLegend", map_action)
        self.assertIn("NativeResetMapZoom", map_action)
        # All 18 vectors are native producers; these five indices have no
        # Show producer and must stay absent in the page adapter.
        self.assertIn("default:", source)
        self.assertIn("return false;", source)


if __name__ == "__main__":
    unittest.main()
