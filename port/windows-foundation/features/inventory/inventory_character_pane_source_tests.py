"""Verify preview callback anchors and pane geometry against the original SWF."""
import runpy
from pathlib import Path

root = Path(__file__).resolve().parents[4]
exporter = Path(__file__).with_name("export_inventory_art.py")
data = runpy.run_path(str(exporter))
swf = data["swf"]
env = data["env"]
source = data["source_panes"]
assert env["path"].as_posix().endswith("original-cache/data/menus/dqcharmenu_droid.swf")
assert data["hashlib"].sha256(env["raw"]).hexdigest() == "43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0"

expected = [
    ("menu_InventorySheetMain", 571, 1, "", "menu_InventorySheetMain/3/",
     (155.05, 87.7, 321.1005615, 282.7512604)),
    ("menu_InventorySheetDetails", 750, 235, "menu_InventorySheetDetails/btn_AutoEquip/",
     "menu_InventorySheetDetails/237/", (219.2730034, 66.2637451, 322.4895824, 260.7911862)),
]
root_frames = env["root"][0]
sprites = env["sprites"]
assert len(source) == len(expected) == 2
for pane, (owner_name, root_depth, child_depth, after_role, before_role, expected_bounds) in zip(source, expected):
    root_placements = [(d, p) for d, p in root_frames.items() if p.get("name") == owner_name]
    assert len(root_placements) == 1
    actual_root_depth, owner = root_placements[0]
    assert actual_root_depth == root_depth
    host = sprites[owner["character"]][0]
    pane_placements = [(d, p) for d, p in host.items() if p.get("name") == "avatarpane"]
    assert len(pane_placements) == 1
    actual_child_depth, avatarpane = pane_placements[0]
    assert actual_child_depth == child_depth and avatarpane.get("clip_depth") is None
    assert pane["path"] == f"_root.{owner_name}.avatarpane"
    assert pane["root_depth"] == root_depth and pane["child_depth"] == child_depth
    assert pane["after_role"] == after_role and pane["before_role"] == before_role

    sibling_depths = sorted(host)
    index = sibling_depths.index(child_depth)
    if owner_name == "menu_InventorySheetMain":
        assert index == 0 and sibling_depths[index + 1] == 3
    else:
        assert sibling_depths[index - 1:index + 2] == [234, 235, 237]
        # Child 234 is an authored shape with a fill branch not exported by the
        # bounded bitmap tessellator. The source depth remains in the record.
        assert host[234]["character"] == 453

    pane_symbol = sprites[avatarpane["character"]]
    assert len(pane_symbol) == 1 and len(pane_symbol[0]) == 1
    shape_depth, shape = next(iter(pane_symbol[0].items()))
    assert shape_depth == 1 and shape["character"] == 217 and shape.get("clip_depth") is None
    shape_bounds = data["source_shape_bounds"][shape["character"]]
    assert shape_bounds == [0, 4822, 0, 948]
    matrix = swf.multiply(swf.multiply(owner["matrix"], avatarpane["matrix"]), shape["matrix"])
    corners = [(matrix[0]*x + matrix[2]*y + matrix[4], matrix[1]*x + matrix[3]*y + matrix[5])
               for x, y in ((0, 0), (4822, 0), (4822, 948), (0, 948))]
    bounds = [min(x for x, _ in corners)/20, min(y for _, y in corners)/20,
              max(x for x, _ in corners)/20, max(y for _, y in corners)/20]
    assert all(abs(got-want) < 0.0002 for got, want in zip(bounds, expected_bounds))
    assert all(abs(got-want) < 0.0002 for got, want in zip(pane["bounds"], expected_bounds))
    matrix_px = matrix[:4] + [matrix[4]/20, matrix[5]/20]
    assert all(abs(got-want) < 0.0002 for got, want in zip(pane["matrix"], matrix_px))

# The native callback's viewport is the transformed avatarpane rectangle. The
# callback computes camera aspect from physical viewport width / height after
# the authored 480x320 menu viewport scaling has been applied.
assert source[1]["child_depth"] == 235 and source[1]["root_depth"] > source[0]["root_depth"]
print("original inventory character pane source tests PASS")
