"""Probe: authored touch-HUD element placements of every dqhud_droid HUD style (menu_HUD_0..3), in the 480x320 stage.

Source: port/android-native/app/src/main/assets/original-cache/data/menus/dqhud_droid.swf (same decoder as
export_hud_panels_v1.py). For each root placement menu_HUD_<n> the frame-0 display list of the style sprite is
walked; every placement whose name is in WANTED is resolved to the stage matrix (twips/20) and the stage bounding box
of its art (frame 0 / idle pose). Output is JSON on stdout: {style: [{path, name, character, matrix_px, bbox_px,
center_px, radius_px}]}. Nothing here is inferred: every number is the product of the authored matrices.
"""
from __future__ import annotations

import json
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import export_hud_panels_v1 as hp  # noqa: E402

WANTED = {
    "btn_skill1", "btn_skill2", "btn_skill3", "btn_interact", "btn_spell", "btn_swap", "btn_potion", "btn_itunes",
    "btn_mainmenu", "btn_charactermenu", "HealthBars", "player", "Joystick", "list", "btn_0", "btn_pre0", "btn_post0",
    "btn_pre1", "btn_post1", "bar_hp", "bar_mp", "bar_xp", "itunes_controls", "dragHandler", "enemy",
    "deadzone_menus", "deadzone_mainmenus", "deadzone_skills1", "deadzone_faery", "deadzone_potion", "deadzone_igm",
    "btn_dpad", "btn_dpad_limits", "stick", "hitzone", "cnt", "value", "DistressGlow", "btimg", "btframe", "HUDelements", "controls", "Level_up", "itemname_text",
}
STYLES = {int(sys.argv[1])} if len(sys.argv) > 1 and sys.argv[1] != "all" else {0, 1, 2, 3}
SWF_PATH = Path(sys.argv[2]) if len(sys.argv) > 2 else hp.SOURCE


def bbox_of(swf, char, frame, matrix):
    pts = []
    for b in hp.batches(swf, char, frame, matrix, "probe"):
        pts.extend((v[0], v[1]) for v in b["triangles"])
    if not pts:
        return None
    xs, ys = [p[0] for p in pts], [p[1] for p in pts]
    return [min(xs), min(ys), max(xs), max(ys)]


def shape_rect_bbox(swf, char, matrix, depth=0):
    """Stage bbox from the DefineShape header RECTs (used when the shape fills are not the droid atlas, e.g. dqhud.swf)."""
    if depth > 16:
        return None
    if char in swf.shapes:
        code, tag = swf.shapes[char]
        (x0, x1, y0, y1), _ = swf.dec["rect"](tag, 2)
        pts = [(matrix[0] * x + matrix[2] * y + matrix[4], matrix[1] * x + matrix[3] * y + matrix[5])
               for x, y in ((x0, y0), (x1, y0), (x1, y1), (x0, y1))]
        return [min(p[0] for p in pts) / 20., min(p[1] for p in pts) / 20., max(p[0] for p in pts) / 20., max(p[1] for p in pts) / 20.]
    if char not in swf.sprites:
        return None
    boxes = []
    for _, p in sorted(swf.sprites[char][0].items()):
        child = p.get("character")
        if child is None or child in swf.edits:
            continue
        color = p.get("color", [[1.] * 4, [0.] * 4])
        if color[0][3] == 0 and color[1][3] == 0:
            continue
        b = shape_rect_bbox(swf, child, hp.compose(matrix, p["matrix"]), depth + 1)
        if b:
            boxes.append(b)
    if not boxes:
        return None
    return [min(b[0] for b in boxes), min(b[1] for b in boxes), max(b[2] for b in boxes), max(b[3] for b in boxes)]


def walk(swf, char, frame, matrix, path, out, depth=0):
    if depth > 12 or char not in swf.sprites:
        return
    frames = swf.sprites[char]
    if not 0 <= frame < len(frames):
        return
    for depth_key, p in sorted(frames[frame].items()):
        child = p.get("character")
        name = p.get("name", "")
        if child is None:
            continue
        m = hp.compose(matrix, p["matrix"])
        here = path + "/" + (name or f"#{child}")
        if name in WANTED:
            box = None
            try:
                if name in ("hitzone",):
                    raise ValueError("hit zones are invisible solids: use the shape header rectangle")
                box = bbox_of(swf, child, hp.rest_frame(swf, child) if child in swf.sprites else 0, m)
            except Exception:  # noqa: BLE001 (non-droid atlas: fall back to the shape header rectangles)
                box = shape_rect_bbox(swf, child, m)
            row = {"path": here, "name": name, "character": child, "matrix_px": [round(v / 20. if i >= 4 else v, 4) for i, v in enumerate(m)]}
            if isinstance(box, list):
                row["bbox_px"] = [round(v, 2) for v in box]
                row["center_px"] = [round((box[0] + box[2]) / 2, 2), round((box[1] + box[3]) / 2, 2)]
                row["size_px"] = [round(box[2] - box[0], 2), round(box[3] - box[1], 2)]
            else:
                row["bbox_px"] = box
            out.append(row)
        walk(swf, child, hp.rest_frame(swf, child) if child in swf.sprites and False else 0, m, here, out, depth + 1)


def main():
    swf = hp.Swf(SWF_PATH)
    result = {}
    for style in sorted(STYLES):
        root = swf.named(swf.root, f"menu_HUD_{style}")
        rows = []
        walk(swf, root["character"], 0, hp.compose(hp.IDENTITY, root["matrix"]), f"menu_HUD_{style}", rows)
        result[f"menu_HUD_{style}"] = {"root_character": root["character"], "elements": rows}
    print(json.dumps(result, indent=1))


if __name__ == "__main__":
    main()
