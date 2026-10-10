"""Original dqhud_droid HUD panels: PC HUD action button, cutscene SKIP, caption box and quest banners.

Source: port/android-native/app/src/main/assets/original-cache/data/menus/dqhud_droid.swf (uncompressed or CWS).
Each panel is resolved through the authored display list from a named root placement, so every batch and text
slot lands in the original 480x320 stage (twips / 20). Bitmap fills keep the exact MenusGraphics_droid atlas UVs
(bitmap id 1); solid fills keep their source colour. A missing or ambiguous placement fails the export.

Output: hud_panels_art_v1.cpp (generated, do not hand-edit). Shape flattening reuses the button exporter's
shape_batches (same triangulation and fill rules).
"""
from __future__ import annotations

import hashlib
import json
import runpy
import struct
import sys
import zlib
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = Path(__file__).resolve().parents[4]
SOURCE = ROOT / "port/android-native/app/src/main/assets/original-cache/data/menus/dqhud_droid.swf"
OUTPUT = HERE / "hud_panels_art_v1.cpp"
DECODER = ROOT / "port/windows-foundation/tools/export_hud_geometry.py"
sys.path.insert(0, str(HERE.parent / "generic_skills"))
import export_pc_gameplay_button_art_v1 as button  # noqa: E402  (shape triangulation and fill rules)

# Authored placement names and labels (verified against the source by the decoder; see HUDART-report).
ACTION_ROOT = "menu_HUD_0"          # PC HUD variant whose controls row carries btn_interact (sprite 374)
ACTION_SPRITE = 374                 # btn_interact: labels idle / pressed / released / disabled / activated
ACTION_ICON_SPRITE = 373            # btn_interact.btimg: one frame per action icon (labels Chest..Inspect)
SKIP_ROOT = "menu_skipcutscene"     # root placement of the SKIP control (sprite 737, label Idle)
SKIP_NAME = "btn_MENU_SKIP"         # the placed button (sprite 736: labels idle / pressed / release)
DIALOG_ROOT = "DialogBox"           # root placement of the dialog sprite; its labels select the variant
DIALOG_VARIANTS = {"quest_new": "QuestMsgDialog", "quest_done": "QuestCompletedMsgDialog", "caption": "ChestTuto"}
# Children of a dialog variant that are not part of the panel art (navigation arrow, flush text, tutorial icon).
DIALOG_EXCLUDED = ("btn_next", "flush_text", "TutoIcon")
IDENTITY = [1., 0., 0., 1., 0., 0.]


def cpp_num(x: float) -> str:
    text = f"{x:.7g}"
    if "." not in text and "e" not in text:
        text += ".0"
    return text + "f"


class Swf:
    def __init__(self, path: Path):
        self.dec = runpy.run_path(str(DECODER))
        raw = path.read_bytes()
        if raw[:3] == b"CWS":
            data = raw[:8] + zlib.decompress(raw[8:])
        elif raw[:3] == b"FWS":
            data = raw
        else:
            raise ValueError("Unsupported dqhud SWF header")
        if len(data) != struct.unpack_from("<I", data, 4)[0]:
            raise ValueError("dqhud SWF declared length mismatch")
        self.raw_sha256 = hashlib.sha256(raw).hexdigest()
        _, at = self.dec["rect"](data, 8)
        at += 4
        self.sprites, self.labels, self.shapes, self.edits = {}, {}, {}, {}
        for code, tag in self.dec["tags"](data, at, len(data)):
            if code == 39:
                ident, frames = struct.unpack_from("<HH", tag)
                labels, cur = {}, 0
                for c, t in self.dec["tags"](tag, 4, len(tag)):
                    if c == 1:
                        cur += 1
                    elif c == 43:
                        labels.setdefault(t.split(b"\0")[0].decode("utf8", "replace"), cur)
                self.sprites[ident] = self.dec["parse_timeline"](tag, 4)
                if len(self.sprites[ident]) != frames:
                    raise ValueError(f"dqhud sprite {ident} frame count mismatch")
                self.labels[ident] = labels
            elif code in (2, 22, 32, 83):
                self.shapes[struct.unpack_from("<H", tag)[0]] = (code, tag)
            elif code == 37:
                edit = self.dec["parse_edit_text"](tag)
                self.edits[edit["character"]] = edit
        self.root = self.dec["parse_timeline"](data, at)[0]
        self.cache: dict = {}

    def frame_of(self, sprite: int, label: str) -> int:
        if label not in self.labels.get(sprite, {}):
            raise ValueError(f"sprite {sprite} has no frame label {label!r}")
        return self.labels[sprite][label]

    def named(self, frame: dict, name: str) -> dict:
        found = [p for p in frame.values() if p.get("name") == name]
        if len(found) != 1:
            raise ValueError(f"expected one placement named {name!r}, found {len(found)}")
        return found[0]


def compose(a, b):
    return button.compose(a, b)


def chain_to(swf: Swf, start_char: int, start_matrix, target: int, depth: int = 0):
    """First depth-first path from a placed sprite (frame 0) to a target sprite. Returns (matrix, hops)."""
    if depth > 16:
        raise ValueError("chain search exceeds nesting bound")
    if start_char == target:
        return start_matrix, []
    for _, p in sorted(swf.sprites.get(start_char, [{}])[0].items()):
        child = p.get("character")
        if child is None or (child not in swf.sprites):
            continue
        hit = chain_to(swf, child, compose(start_matrix, p["matrix"]), target, depth + 1)
        if hit is not None:
            return hit[0], [(p.get("name", ""), child)] + hit[1]
    return None


def leaves(swf: Swf, char: int, frame: int, matrix, mult, path: str, out: list, depth: int = 0) -> None:
    """Shape leaves of a sprite frame. EDIT text is not art and is skipped; source features that need
    clipping, filters or colour-add are refused, as in the button exporter."""
    if depth > 24:
        raise ValueError("panel nesting bound")
    if char in swf.shapes:
        out.append({"shape": char, "matrix": matrix, "mult": mult, "path": path})
        return
    if char not in swf.sprites or not 0 <= frame < len(swf.sprites[char]):
        raise ValueError(f"panel references missing character/frame {char}/{frame}")
    for depth_key, p in sorted(swf.sprites[char][frame].items()):
        color = p.get("color", [[1.] * 4, [0.] * 4])
        if color[0][3] == 0 and color[1][3] == 0:
            continue
        child = p.get("character")
        if child is None or child in swf.edits:
            continue
        child_mult = [m * c for m, c in zip(mult, color[0])]
        sub: list = []
        # Nested sprites are walked at frame 0 (their idle pose); dialog children are placed at their rest frame by the caller.
        leaves(swf, child, 0, compose(matrix, p["matrix"]), child_mult, path + "/" + p.get("name", ""), sub, depth + 1)
        if not sub:
            continue  # text-only or empty placements carry no art, so their flags do not matter here
        if p.get("clip_depth"):
            raise ValueError(f"panel clip mask at {char}/{depth_key}")
        if p.get("placeobject3_extra_flags"):
            raise ValueError(f"panel filter/blend at {char}/{depth_key}")
        if any(v != 0 for v in color[1]):
            raise ValueError(f"panel colour add term at {char}/{depth_key}")
        out.extend(sub)


def batches(swf: Swf, char: int, frame: int, matrix, path: str, exclude=()) -> list:
    """Batches of a sprite frame (or of a single shape) with the stage matrix (twips), excluding named children."""
    found: list = []
    if char in swf.shapes:
        leaves(swf, char, 0, matrix, [1.] * 4, path, found)
    else:
        for _, p in sorted(swf.sprites[char][frame].items()):
            if p.get("name", "") in exclude:
                continue
            color = p.get("color", [[1.] * 4, [0.] * 4])
            if color[0][3] == 0 and color[1][3] == 0:
                continue
            child = p.get("character")
            if child is None or child in swf.edits:
                continue
            child_mult = list(color[0])
            leaves(swf, child, rest_frame(swf, child), compose(matrix, p["matrix"]), child_mult,
                   path + "/" + p.get("name", ""), found, 1)
    out = []
    cache = swf.cache
    for leaf in found:
        out.extend(b for b in button.shape_batches(swf.dec, swf.shapes, cache, leaf) if visible(b))
    return out


def visible(batch: dict) -> bool:
    """Bitmap batches are always art; solid batches with zero alpha are invisible in the source."""
    return batch["bitmap"] or batch["rgba"][3] > 0


def rest_frame(swf: Swf, sprite: int) -> int:
    """Rest pose of an animated sprite: the frame before its 'hide' label (after 'show' has finished)."""
    labels = swf.labels.get(sprite, {})
    return labels["hide"] - 1 if "hide" in labels else 0


def edit_slots(swf: Swf, char: int, frame: int, matrix, out: list, container: str = "", name: str = "",
               depth: int = 0) -> None:
    """Dynamic text fields reached from a sprite frame. container = the outermost named placement on the way
    (for example TextBox or NameBox), name = the placement of the text field itself (its authored role).
    Bounds are in stage px with the authored format."""
    if depth > 24:
        raise ValueError("text search exceeds nesting bound")
    if char in swf.edits:
        e = swf.edits[char]
        x0, x1, y0, y1 = e["bounds_twips"]
        pts = [(matrix[0] * x + matrix[2] * y + matrix[4], matrix[1] * x + matrix[3] * y + matrix[5])
               for x, y in ((x0, y0), (x1, y0), (x1, y1), (x0, y1))]
        xs, ys = [p[0] / 20. for p in pts], [p[1] / 20. for p in pts]
        if max(xs) < 0 or min(xs) > 480 or max(ys) < 0 or min(ys) > 320:
            return  # off-stage helper text (never shown in the source frame)
        out.append({"container": container, "name": name, "variable": e["variable"],
                    "rect": [min(xs), min(ys), max(xs), max(ys)], "height": e["height_twips"] / 20.,
                    "rgba": e["rgba"], "align": e["layout"].get("align", 0), "font": e["font"],
                    "leading": e["layout"].get("leading", 0)})
        return
    if char not in swf.sprites or not 0 <= frame < len(swf.sprites[char]):
        return
    for _, p in sorted(swf.sprites[char][frame].items()):
        color = p.get("color", [[1.] * 4, [0.] * 4])
        if color[0][3] == 0 and color[1][3] == 0:
            continue
        child = p.get("character")
        if child is None:
            continue
        placed = p.get("name", "")
        edit_slots(swf, child, rest_frame(swf, child) if child in swf.sprites else 0,
                   compose(matrix, p["matrix"]), out, container or placed, placed, depth + 1)


def build_panels(swf: Swf) -> dict:
    out: dict = {}
    # 1. Action button btn_interact, placed by the PC HUD controls row (root menu_HUD_0 -> ... -> sprite 374).
    root_place = swf.named(swf.root, ACTION_ROOT)
    hit = chain_to(swf, root_place["character"], compose(IDENTITY, root_place["matrix"]), ACTION_SPRITE)
    if hit is None:
        raise ValueError("btn_interact is not reachable from the PC HUD root placement")
    action_matrix, hops = hit
    idle = swf.frame_of(ACTION_SPRITE, "idle")
    pressed = swf.frame_of(ACTION_SPRITE, "pressed")
    btimg = swf.named(swf.sprites[ACTION_SPRITE][idle], "btimg")
    icon_matrix = compose(action_matrix, btimg["matrix"])
    icons = []
    labels = swf.labels[ACTION_ICON_SPRITE]
    for frame in range(len(swf.sprites[ACTION_ICON_SPRITE])):
        label = next((k for k, v in labels.items() if v == frame), f"frame{frame}")
        icons.append({"label": label, "batches": batches(swf, ACTION_ICON_SPRITE, frame, icon_matrix, "action_icon")})
    out["action"] = {
        "hops": [name for name, _ in hops],
        "base": batches(swf, ACTION_SPRITE, idle, action_matrix, "action", exclude=("btimg",)),
        "pressed": batches(swf, ACTION_SPRITE, pressed, action_matrix, "action", exclude=("btimg",)),
        "icons": icons,
    }

    # 2. Cutscene SKIP: root menu_skipcutscene (sprite 737, Idle) -> btn_MENU_SKIP (sprite 736, idle).
    skip_root = swf.named(swf.root, SKIP_ROOT)
    skip_sprite = skip_root["character"]
    skip_idle = swf.frame_of(skip_sprite, "Idle")
    skip_btn = swf.named(swf.sprites[skip_sprite][skip_idle], SKIP_NAME)
    skip_matrix = compose(compose(IDENTITY, skip_root["matrix"]), skip_btn["matrix"])
    skip_child = skip_btn["character"]
    skip_idle_frame = swf.frame_of(skip_child, "idle")
    texts: list = []
    edit_slots(swf, skip_child, skip_idle_frame, compose(compose(IDENTITY, skip_root["matrix"]), skip_btn["matrix"]), texts)
    out["skip"] = {"batches": batches(swf, skip_child, skip_idle_frame, compose(compose(IDENTITY, skip_root["matrix"]), skip_btn["matrix"]), "skip", exclude=("text",)),
                   "texts": texts, "matrix": skip_matrix}

    # 3. Dialog variants of the DialogBox sprite: quest banners and the caption box.
    dialog_root = swf.named(swf.root, DIALOG_ROOT)
    dialog_sprite = dialog_root["character"]
    dialog_matrix = compose(IDENTITY, dialog_root["matrix"])
    for key, label in DIALOG_VARIANTS.items():
        variant = swf.frame_of(dialog_sprite, label)
        panel_batches: list = []
        panel_texts: list = []
        for _, p in sorted(swf.sprites[dialog_sprite][variant].items()):
            name = p.get("name", "")
            child = p.get("character")
            color = p.get("color", [[1.] * 4, [0.] * 4])
            if child is None or name in DIALOG_EXCLUDED or (color[0][3] == 0 and color[1][3] == 0):
                continue
            child_matrix = compose(dialog_matrix, p["matrix"])
            frame = rest_frame(swf, child) if child in swf.sprites else 0
            found: list = []
            leaves(swf, child, frame, child_matrix, list(color[0]), f"dialog_{key}/{name}", found)
            for leaf in found:
                panel_batches.extend(b for b in button.shape_batches(swf.dec, swf.shapes, swf.cache, leaf) if visible(b))
            if child in swf.sprites:
                edit_slots(swf, child, frame, child_matrix, panel_texts)
        out[key] = {"batches": panel_batches, "texts": panel_texts, "label": label, "frame": variant}
    return out


def cpp_batches(items) -> str:
    rows = []
    for b in items:
        tris = ",".join("{" + ",".join(cpp_num(c) for c in v) + "}" for v in b["triangles"])
        rgba = ",".join(cpp_num(c) for c in b["rgba"])
        rows.append("{" + str(b["shape"]) + "," + ("true" if b["bitmap"] else "false") + ",{" + rgba + "},{" + tris + "}}")
    return ",".join(rows)


def cpp_texts(items) -> str:
    rows = []
    for t in items:
        rect = ",".join(cpp_num(c) for c in t["rect"])
        rgba = ",".join(str(int(c)) for c in t["rgba"])
        rows.append('{"' + t["container"] + '","' + t["name"] + '",{' + rect + "},{" + rgba + "}," +
                    cpp_num(t["height"]) + "," + str(int(t["align"])) + "}")
    return ",".join(rows)


def write(panels: dict, swf: Swf) -> dict:
    lines = [
        "// Generated from exact dqhud_droid.swf display lists by features/hud_panels/export_hud_panels_v1.py;",
        "// do not hand-edit. Batches are in the original 480x320 stage (twips / 20).",
        '#include "hud_panels_art_v1.hpp"',
        "namespace dh::foundation::hud_panels {",
        "namespace {",
    ]
    action = panels["action"]
    lines.append("const std::vector<PcGameplayHudArtBatchV1> action_base{" + cpp_batches(action["base"]) + "};")
    lines.append("const std::vector<PcGameplayHudArtBatchV1> action_pressed{" + cpp_batches(action["pressed"]) + "};")
    rows = []
    for i, icon in enumerate(action["icons"]):
        lines.append(f"const std::vector<PcGameplayHudArtBatchV1> action_icon_{i}{{" + cpp_batches(icon["batches"]) + "};")
        rows.append(f"&action_icon_{i}")
    lines.append("const std::vector<const std::vector<PcGameplayHudArtBatchV1>*> action_icons{" + ",".join(rows) + "};")
    lines.append("const std::vector<PcGameplayHudArtBatchV1> skip_batches{" + cpp_batches(panels["skip"]["batches"]) + "};")
    lines.append("const std::vector<HudPanelTextSlotV1> skip_texts{" + cpp_texts(panels["skip"]["texts"]) + "};")
    for key in DIALOG_VARIANTS:
        lines.append(f"const std::vector<PcGameplayHudArtBatchV1> {key}_batches{{" + cpp_batches(panels[key]["batches"]) + "};")
        lines.append(f"const std::vector<HudPanelTextSlotV1> {key}_texts{{" + cpp_texts(panels[key]["texts"]) + "};")
    lines += [
        "} // namespace",
        "const std::vector<PcGameplayHudArtBatchV1>& action_base_v1() noexcept { return action_base; }",
        "const std::vector<PcGameplayHudArtBatchV1>& action_pressed_v1() noexcept { return action_pressed; }",
        "const std::vector<PcGameplayHudArtBatchV1>& action_icon_v1(std::size_t frame) noexcept { return *action_icons[frame]; }",
        "std::size_t action_icon_count_v1() noexcept { return action_icons.size(); }",
        "const std::vector<PcGameplayHudArtBatchV1>& skip_batches_v1() noexcept { return skip_batches; }",
        "const std::vector<HudPanelTextSlotV1>& skip_texts_v1() noexcept { return skip_texts; }",
        "const std::vector<PcGameplayHudArtBatchV1>& caption_batches_v1() noexcept { return caption_batches; }",
        "const std::vector<HudPanelTextSlotV1>& caption_texts_v1() noexcept { return caption_texts; }",
        "const std::vector<PcGameplayHudArtBatchV1>& quest_new_batches_v1() noexcept { return quest_new_batches; }",
        "const std::vector<HudPanelTextSlotV1>& quest_new_texts_v1() noexcept { return quest_new_texts; }",
        "const std::vector<PcGameplayHudArtBatchV1>& quest_done_batches_v1() noexcept { return quest_done_batches; }",
        "const std::vector<HudPanelTextSlotV1>& quest_done_texts_v1() noexcept { return quest_done_texts; }",
        "} // namespace dh::foundation::hud_panels",
    ]
    OUTPUT.write_text("\n".join(lines) + "\n", encoding="utf-8")
    return {
        "source_sha256": swf.raw_sha256,
        "output": str(OUTPUT),
        "action_hops": action["hops"],
        "action_base_batches": len(action["base"]),
        "action_pressed_batches": len(action["pressed"]),
        "action_icons": [{"label": i["label"], "batches": len(i["batches"])} for i in action["icons"]],
        "skip_batches": len(panels["skip"]["batches"]),
        "skip_texts": [t["name"] + ":" + t["variable"] for t in panels["skip"]["texts"]],
        "dialog": {k: {"frame": panels[k]["frame"], "batches": len(panels[k]["batches"]),
                       "texts": [t["name"] + ":" + t["variable"] for t in panels[k]["texts"]]} for k in DIALOG_VARIANTS},
    }


def main() -> None:
    swf = Swf(SOURCE)
    panels = build_panels(swf)
    print(json.dumps(write(panels, swf), indent=2))


if __name__ == "__main__":
    main()
