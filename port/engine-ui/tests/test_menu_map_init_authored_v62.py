"""Map Init lookup regression against the supplied authored Droid SWF/tree.

Run directly with Python. DH2_AUTHORED_UI_DIR may point to the exported asset
directory; by default it is .local-inputs/ui-layout-discovery. This checks
authored first-frame placements and production lookup names, not game runtime.
"""
import hashlib
import json
import os
from pathlib import Path
import re
import struct
import unittest
import zlib


ROOT = Path(__file__).resolve().parents[3]
ASSETS = Path(os.environ.get("DH2_AUTHORED_UI_DIR", ROOT / ".local-inputs/ui-layout-discovery"))
EXPECTED = ("MapIconsDummy", "MapName", "RenderMap")  # Original Init 0x453c3c.
SWF_SHA256 = "43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0"


def first_frame(rows):
    placements = {}
    for row in rows:
        if row["frame"]:
            break
        if row["tag"] in (26, 70):
            depth = row["placement_depth"]
            placements[depth] = {**placements.get(depth, {}), **row}
    return [row for _, row in sorted(placements.items()) if "character_id" in row]


class MapInitAuthoredTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        raw = (ASSETS / "dqcharmenu_droid.swf").read_bytes()
        if hashlib.sha256(raw).hexdigest() != SWF_SHA256:
            raise AssertionError("Map regression requires the original authored Droid movie")
        cls.swf = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
        if raw[:3] not in (b"FWS", b"CWS") or len(cls.swf) != struct.unpack_from("<I", raw, 4)[0]:
            raise AssertionError("Invalid authored SWF header/declared length")
        cls.rows = json.loads((ASSETS / "dqcharmenu_droid-tree.json").read_text())
        cls.sprites = {}

        def collect(rows):
            for row in rows:
                if row["tag"] == 39:
                    if struct.unpack_from("<H", cls.swf, row["offset"])[0] != row["id"]:
                        raise AssertionError("Tree sprite definition differs from the actual SWF")
                    cls.sprites[row["id"]] = row
                    collect(row["children"])

        collect(cls.rows)

    def test_production_init_uses_original_child_names(self):
        source = (ROOT / "port/engine-ui/menu_postmovie_v62.cpp").read_text()
        initializer = source.split("bool MenuMapOwnerV62::source_initialize_v67", 1)[1]
        array = re.search(r"paths\[\]\s*\{([^}]+)\}", initializer)
        self.assertIsNotNone(array)
        self.assertEqual(tuple(re.findall(r'"([^"]+)"', array.group(1))), EXPECTED)

    def test_children_resolve_inside_authored_map_sheet(self):
        menus = [row for row in first_frame(self.rows) if row.get("instance_name") == "menu_MapSheet"]
        self.assertEqual(len(menus), 1, "Expected one authored root menu_MapSheet")
        menu = menus[0]
        self.assertIn(menu["character_id"], self.sprites)
        paths = []

        def walk(rows, path, chain):
            for row in first_frame(rows):
                name = row.get("instance_name", "#" + str(row["placement_depth"]))
                child = path + "." + name
                if "instance_name" in row:
                    body = self.swf[row["offset"]:row["offset"] + row["length"]]
                    self.assertIn(name.encode() + b"\0", body, "Tree placement name absent from its SWF tag")
                    paths.append(child)
                identity = row["character_id"]
                if identity in self.sprites and identity not in chain:
                    walk(self.sprites[identity]["children"], child, chain + (identity,))

        walk(self.sprites[menu["character_id"]]["children"], "_root.menu_MapSheet", (menu["character_id"],))
        for name, expected_path in zip(EXPECTED, (
            "_root.menu_MapSheet.iconsContainer.MapIconsDummy",
            "_root.menu_MapSheet.MapName",
            "_root.menu_MapSheet.RenderMap",
        )):
            with self.subTest(child=name):
                # The actual SearchIndex traverses descendants in display order
                # and resolves these single-segment queries by child name.
                matches = [path for path in paths if path.rsplit(".", 1)[-1] == name]
                self.assertTrue(matches, "Map Init child must resolve under the MapSheet context")
                self.assertEqual(matches[0], expected_path)
        self.assertFalse(any(path.rsplit(".", 1)[-1] in ("y", "Map") for path in paths))


if __name__ == "__main__":
    unittest.main()
