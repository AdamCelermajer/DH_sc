"""Audit the retained Quest Log SWF geometry and ActionScript row/hit routes."""
from __future__ import annotations

import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys
import zlib

ROOT = Path(__file__).resolve().parents[4]
MOVIE = ROOT / "port/android-native/app/src/main/assets/original-cache/data/menus/dqcharmenu_droid.swf"
ACTIONS = ROOT / "port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt"
PARSER = ROOT / "port/windows-foundation/features/faery_menu/source_swf_geometry.py"


def require(value: bool, message: str) -> None:
    if not value:
        raise RuntimeError(message)


def load_parser():
    spec = importlib.util.spec_from_file_location("quest_swf_geometry", PARSER)
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


def shape_rect_contour_edges(parser, code: int, tag: bytes):
    """Read filled straight edges for the two authored hit rectangles."""
    _, at = parser.rect(tag, 2)
    count = tag[at]
    at += 1
    for _ in range(count):
        fill_kind = tag[at]
        at += 1
        if fill_kind != 0 or code != 32:
            raise RuntimeError("Quest hit contour is no longer one solid DefineShape3 fill")
        at += 4  # RGBA. Alpha zero is still the source MovieClip hit fill.
    line_count = tag[at]
    at += 1
    require(line_count == 0, "Quest hit contour unexpectedly adds line-only hit geometry")
    bits = parser.Bits(tag, at)
    fill_bits, line_bits = bits.u(4), bits.u(4)
    x = y = fill0 = fill1 = 0
    edges = []
    while True:
        if not bits.u(1):
            flags = bits.u(5)
            if not flags:
                break
            if flags & 1:
                width = bits.u(5)
                x, y = bits.s(width), bits.s(width)
            if flags & 2:
                fill0 = bits.u(fill_bits)
            if flags & 4:
                fill1 = bits.u(fill_bits)
            if flags & 8:
                bits.u(line_bits)
            if flags & 16:
                raise RuntimeError("Quest hit contour unexpectedly changes source styles")
            continue
        straight, width = bits.u(1), bits.u(4) + 2
        if not straight:
            raise RuntimeError("Quest hit contour contains an unsupported curved edge")
        start = (x, y)
        if bits.u(1):
            dx, dy = bits.s(width), bits.s(width)
        elif bits.u(1):
            dx, dy = 0, bits.s(width)
        else:
            dx, dy = bits.s(width), 0
        x, y = x + dx, y + dy
        end = (x, y)
        if fill0 or fill1:
            edges.append(frozenset((start, end)))
    return edges


def main() -> None:
    source_hash = hashlib.sha256(MOVIE.read_bytes()).hexdigest()
    require(source_hash == "43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0",
            "Original Quest Log SWF differs from the audited movie")
    raw = MOVIE.read_bytes()
    data = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
    require(raw[:3] in (b"CWS", b"FWS"), "Unsupported source SWF compression")
    require(len(data) == struct.unpack_from("<I", data, 4)[0], "Source SWF length mismatch")
    parser = load_parser()
    stage, at = parser.rect(data, 8)
    require(stage == [0, 9600, 0, 6400], "Quest source stage changed")
    at += 4
    root_frames = parser.parse_timeline(data, at)
    page = [p for p in root_frames[0].values()
            if p.get("character") == 599 and p.get("name") == "menu_QuestLogSheetNEW"]
    require(len(page) == 1 and page[0]["matrix"] == [1.0, 0.0, 0.0, 1.0, 170, 2126],
            "Original page placement/identity changed")

    sprites, edits, source_shapes = {}, {}, {}
    for code, tag in parser.tags(data, at, len(data)):
        if code == 39:
            character, count = struct.unpack_from("<HH", tag)
            frames = parser.parse_timeline(tag, 4)
            if frames:
                require(len(frames) == count, f"Sprite {character} timeline frame count mismatch")
                sprites[character] = frames
        elif code == 37:
            edit = parser.parse_edit_text(tag)
            edits[edit["character"]] = edit
        elif code in (2, 22, 32, 83):
            shape_id = struct.unpack_from("<H", tag)[0]
            if shape_id in (82, 106):
                bounds, _ = parser.rect(tag, 2)
                source_shapes[shape_id] = (code, bounds, tag)

    placements = {}
    fields = {}

    def walk(character, matrix, path, depth=0):
        require(depth <= 20, "Quest page nesting bound exceeded")
        if character in edits:
            item = edits[character]
            fields[path] = {
                "bounds_twips": item["bounds_twips"],
                "matrix_twips": matrix,
            }
        if character not in sprites:
            return
        for layer, item in sorted(sprites[character][0].items()):
            name = item.get("name", f"d{layer}")
            child_path = path + "/" + name
            child_matrix = parser.multiply(matrix, item["matrix"])
            if item.get("name"):
                placements[child_path] = {
                    "character": item.get("character"),
                    "matrix_twips": item["matrix"],
                    "composed_matrix_twips": child_matrix,
                }
            if item.get("character") is not None:
                walk(item["character"], child_matrix, child_path, depth + 1)

    walk(599, parser.IDENTITY, "sprite599")
    require(placements["sprite599/AllQuests/content/Assigned"]["character"] == 569,
            "Original Assigned category art placement missing")
    require(placements["sprite599/AllQuests/content/Completed"]["character"] == 573,
            "Original Completed category art placement missing")
    require(placements["sprite599/AllQuests/content/Assigned/btnQuests"]["character"] == 564 and
            placements["sprite599/AllQuests/content/Completed/btnQuests"]["character"] == 564,
            "Original shared row-template placement missing")
    require(fields["sprite599/QuestDesc/Description/text"]["matrix_twips"] ==
            [1.0, 0.0, 0.0, 1.0, 5954.0, 431.0], "Source objective text field geometry changed")
    require(fields["sprite599/QuestDetailsText/text"]["matrix_twips"] ==
            [1.0, 0.0, 0.0, 1.0, 5884.0, -733.0], "Source details text field geometry changed")
    require("sprite599/btn_Activate/hitzone" in placements,
            "Source Activate hitzone placement missing")
    require(82 in source_shapes and source_shapes[82][0] == 32 and
            source_shapes[82][1] == [0, 2320, 0, 1039],
            "Original transparent Activate hit contour shape 82 changed")
    require(106 in source_shapes and source_shapes[106][0] == 32 and
            source_shapes[106][1] == [1002, 6344, -274, 661],
            "Original quest row hit contour shape 106 changed")
    rect_edges = lambda points: {frozenset((points[i], points[(i + 1) % 4]))
                                 for i in range(4)}
    require(set(shape_rect_contour_edges(parser, *source_shapes[82][::2])) ==
            rect_edges([(0, 0), (2320, 0), (2320, 1039), (0, 1039)]),
            "Activate shape 82 is no longer the audited filled rectangle contour")
    require(set(shape_rect_contour_edges(parser, *source_shapes[106][::2])) ==
            rect_edges([(1002, -274), (6344, -274), (6344, 661), (1002, 661)]),
            "Quest row shape 106 is no longer the audited filled rectangle contour")
    row_hit_placements = [item for item in sprites[564][0].values()
                          if item.get("character") == 106]
    require(len(row_hit_placements) == 1,
            "Original shared Quest row template no longer has one shape 106 hit contour")
    row_hit_matrix = row_hit_placements[0]["matrix"]
    require(row_hit_matrix == [1.716766357421875, 0.0, 0.0, 1.0, -1515, 0],
            "Original row hit contour transform changed")
    activate_hit_matrix = parser.multiply(
        parser.multiply(page[0]["matrix"], placements["sprite599/btn_Activate"]["matrix_twips"]),
        placements["sprite599/btn_Activate/hitzone"]["matrix_twips"])
    expected_activate_hit_matrix = [1.22930908203125, 0.0, 0.0,
                                    0.8619384765625, 6318.0, 5255.0]
    require(all(abs(a-b) < 1e-6 for a,b in zip(activate_hit_matrix, expected_activate_hit_matrix)),
            "Activate hitzone page/root transform changed")

    action_source = ACTIONS.read_text(encoding="utf-8")
    start = action_source.index("BLOCK root/sprite599 FRAME 0 OFFSET 00025b43")
    end = action_source.index("CLIP root/sprite599 FRAME 0 OFFSET 00026f37", start)
    block = action_source[start:end]
    for token in ('"onRelease"', '"NativeGetQuestDetails"', '"NativeSetCurrentQuest"',
                  '"NativeGetQuestIDsInRange"', '"Assigned"', '"Completed"',
                  '"LightOn"', '"Selected"'):
        require(token in block, f"Source Quest ActionScript route missing {token}")
    require('"_y"' in block and "50" in block and '"duplicateMovieClip"' in block,
            "Source-authored repeated list-row spacing route missing")
    require(hashlib.sha256(ACTIONS.read_bytes()).hexdigest() ==
            "d8280e6fcf7f96ea1533f8d3a60f26cddc73c0d1cbaf5e98ef35f75444775565",
            "Quest ActionScript extraction differs from its audited source")
    print(json.dumps({
        "status": "PASS",
        "movie_sha256": source_hash,
        "action_sha256": hashlib.sha256(ACTIONS.read_bytes()).hexdigest(),
        "page": {"name": "menu_QuestLogSheetNEW", "sprite": 599,
                 "matrix_twips": page[0]["matrix"], "stage_twips": stage},
        "source_geometry": {
            "assigned": placements["sprite599/AllQuests/content/Assigned"],
            "completed": placements["sprite599/AllQuests/content/Completed"],
            "shared_row_template": placements["sprite599/AllQuests/content/Assigned/btnQuests"],
            "objective_field": fields["sprite599/QuestDesc/Description/text"],
            "details_field": fields["sprite599/QuestDetailsText/text"],
            "activate_hitzone": placements["sprite599/btn_Activate/hitzone"],
            "activate_hit_shape": {"id": 82, "bounds_twips": source_shapes[82][1],
                                   "root_matrix_twips": activate_hit_matrix},
            "row_hit_shape": {"id": 106, "bounds_twips": source_shapes[106][1],
                              "row_matrix_twips": row_hit_matrix},
            "row_y_step_swf_pixels": 50,
        },
        "action_routes": ["row onRelease -> NativeGetQuestDetails",
                          "btn_Activate -> NativeSetCurrentQuest",
                          "NativeGetQuestIDsInRange Active/Closed"],
    }, indent=2))


if __name__ == "__main__":
    main()
