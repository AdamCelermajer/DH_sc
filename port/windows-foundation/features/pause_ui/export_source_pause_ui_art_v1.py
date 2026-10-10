"""Export authored pause/HUD triangles, text styles, and button hit contours.

This is intentionally a narrow SWF graph walk over the original dqhud movie.
It uses the repository's checked SWF tag/edge parser and emits the same flat
ScreenArt payload consumed by the frontend renderer. No runtime timeline,
menu, clock, or pause owner is created here.
"""
from __future__ import annotations

import hashlib
import ast
import importlib.util
import inspect
import json
import struct
import types
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent
SWF_PATH = ROOT / "port/android-native/app/src/main/assets/original-cache/data/menus/dqhud_droid.swf"
spec = importlib.util.spec_from_file_location("checked_swf", ROOT / "port/windows-foundation/tools/export_hud_geometry.py")
swf = importlib.util.module_from_spec(spec)
spec.loader.exec_module(swf)

def load_checked_shape_parser(bitmap_dimensions):
    # The quest exporter already contains the repository's dependency-free
    # contour-with-holes triangulator. Reuse that checked implementation while
    # widening its source bitmap guard for this movie's embedded IDs 1..3.
    quest_path = ROOT / "port/windows-foundation/features/quests/export_runtime_quest_menu_art_v1.py"
    tree = ast.parse(quest_path.read_text(encoding="utf-8"))
    node = next(n for n in tree.body if isinstance(n, ast.FunctionDef) and n.name == "parse_shape_full")
    source = ast.get_source_segment(quest_path.read_text(encoding="utf-8"), node)
    source = source.replace("if bitmap != 1:", "if bitmap not in bitmap_dimensions:")
    source = source.replace('{"kind": "bitmap", "matrix": matrix}',
                            '{"kind": "bitmap", "bitmap_id": bitmap, "matrix": matrix}')
    source = source.replace("bitmap_vertices, solid_vertices = [], []",
                            "bitmap_vertices, solid_vertices = [], []; bitmap_groups, solid_groups = [], []")
    source = source.replace(
        "        if style[\"kind\"] == \"solid\":\n            solid_vertices.extend([[x, y, 0., 0.] for x, y in points])",
        "        if style[\"kind\"] == \"solid\":\n            fill_vertices = [[x, y, 0., 0.] for x, y in points]\n            solid_vertices.extend(fill_vertices)\n            solid_groups.append({\"rgba\": style[\"rgba\"], \"vertices\": fill_vertices})")
    source = source.replace(
        "                bitmap_vertices.append([x, y, u, v])",
        "                bitmap_vertices.append([x, y, u, v])\n            bitmap_groups.append({\"bitmap_id\": style[\"bitmap_id\"], \"vertices\": [[x, y, (d * (x - tx) - c * (y - ty)) / determinant / 1024, (-b * (x - tx) + a * (y - ty)) / determinant / 1024] for x, y in points]})")
    source = source.replace(
        '"solid_rgba": next((style["rgba"] for style in styles if style["kind"] == "solid"), None)}',
        '"solid_rgba": next((style["rgba"] for style in styles if style["kind"] == "solid"), None), "bitmap_groups": bitmap_groups, "solid_groups": solid_groups}')
    if "bitmap_groups" not in source or "solid_groups" not in source:
        raise RuntimeError("Failed to adapt checked quest shape parser for grouped pause fills")
    namespace = {"swf": swf, "struct": struct, "bitmap_dimensions": bitmap_dimensions}
    exec(source, namespace)
    return namespace["parse_shape_full"]


raw = SWF_PATH.read_bytes()
data = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
if data[:3] != b"FWS" or len(data) != struct.unpack_from("<I", data, 4)[0]:
    raise ValueError("Unsupported or truncated original pause SWF")
stage, at = swf.rect(data, 8)
if stage != [0, 9600, 0, 6400]:
    raise ValueError(f"Unexpected original pause SWF stage {stage}")
at += 4
bitmap_dimensions = {struct.unpack_from("<H", tag)[0]: struct.unpack_from("<HH", tag, 3)
                    for code, tag in swf.tags(data, at, len(data)) if code in (20, 36)}
shapes, shape_codes, shape_failures, sprites, sprite_labels, sprite_stops, edits, fonts = {}, {}, {}, {}, {}, {}, {}, {}
parse_shape_full = None

for code, tag in swf.tags(data, at, len(data)):
    if code in (2, 22, 32):
        ident = struct.unpack_from("<H", tag)[0]
        if parse_shape_full is None:
            parse_shape_full = load_checked_shape_parser(bitmap_dimensions)
        swf.ROLES[ident] = f"shape_{ident}"
        shape_codes[ident] = code
        try:
            shapes[ident] = parse_shape_full(code, tag)
            for group in shapes[ident]["bitmap_groups"]:
                width, height = bitmap_dimensions[group["bitmap_id"]]
                for vertex in group["vertices"]:
                    vertex[2] *= 1024 / width
                    vertex[3] *= 1024 / height
        except ValueError as exc:
            shape_failures[ident] = str(exc)
    elif code == 39:
        ident, count = struct.unpack_from("<HH", tag)
        frames = swf.parse_timeline(tag, 4)
        if len(frames) != count:
            raise ValueError(f"Sprite {ident}: timeline frame count mismatch")
        labels, stops, frame = {}, [], 0
        for subcode, subtag in swf.tags(tag, 4, len(tag)):
            if subcode == 43:
                labels[subtag.split(b"\0")[0].decode("utf8")] = frame
            elif subcode == 12 and subtag == b"\x07\x00":
                stops.append(frame)
            elif subcode == 1:
                frame += 1
        sprites[ident] = frames
        sprite_labels[ident] = labels
        sprite_stops[ident] = stops
    elif code == 37:
        rec = swf.parse_edit_text(tag)
        strings = tag.split(b"\0")
        rec["initial_text"] = strings[-2].decode("utf8", errors="replace") if rec["flags"] & 0x8000 else ""
        edits[rec["character"]] = rec
    elif code in (48, 75):
        ident = struct.unpack_from("<H", tag)[0]
        length = tag[4]
        name = tag[5:5 + length].decode("utf8", errors="replace").rstrip("\0")
        metrics = [0, 0, 0]
        if tag[2] & 0x80:
            offsets = 7 + length
            size = 4 if tag[2] & 8 else 2
            codes = int.from_bytes(tag[offsets + struct.unpack_from("<H", tag, 5 + length)[0] * size:
                                      offsets + (struct.unpack_from("<H", tag, 5 + length)[0] + 1) * size], "little")
            layout = offsets + codes + struct.unpack_from("<H", tag, 5 + length)[0] * (2 if tag[2] & 4 else 1)
            metrics = list(struct.unpack_from("<hhh", tag, layout))
            if code == 75:
                metrics = [v / 20 for v in metrics]
        fonts[ident] = {"name": name, "metrics": metrics}

root = swf.parse_timeline(data, at)


def multiply(a, b):
    return [a[0]*b[0]+a[2]*b[1], a[1]*b[0]+a[3]*b[1],
            a[0]*b[2]+a[2]*b[3], a[1]*b[2]+a[3]*b[3],
            a[0]*b[4]+a[2]*b[5]+a[4], a[1]*b[4]+a[3]*b[5]+a[5]]


def idle_frame(character):
    labels = sprite_labels.get(character, {})
    idle = next((frame for label, frame in labels.items() if label.lower() == "idle"), None)
    if idle is None:
        return 0
    next_label = min((frame for frame in labels.values() if frame > idle), default=len(sprites[character]))
    stop = min((frame for frame in sprite_stops.get(character, []) if idle <= frame < next_label), default=idle)
    return stop


def walk(character, matrix, path, art, fields, hits, button="", input_only=False,
         color_mult=(1., 1., 1., 1.), color_add=(0., 0., 0., 0.),
         frame_override=None, multiplayer=False, hidden_paths=(), depth=0):
    if depth > 40:
        raise ValueError("Pause SWF nesting bound")
    if any(path == hidden or path.startswith(hidden + "/") for hidden in hidden_paths):
        return
    if character in shapes:
        record = shapes[character]
        # Shape IDs remain grouped by original fill; no fill color/atlas
        # assignment is reconstructed from bounds or painted replacements.
        for group in record["bitmap_groups"] + record["solid_groups"]:
            bitmap = group.get("bitmap_id", 0)
            source_color = [c / 255 for c in group.get("rgba", [255, 255, 255, 255])]
            color = [max(0., min(1., source_color[i] * color_mult[i] + color_add[i])) for i in range(4)]
            transformed = [[(matrix[0]*x+matrix[2]*y+matrix[4])/20,
                            (matrix[1]*x+matrix[3]*y+matrix[5])/20, u, v]
                           for x, y, u, v in group["vertices"]]
            if button and color[3] > 0:
                # The original authored clips do not contain a named hitzone
                # for these MovieClip controls. This contour is the union of
                # actual currently-rendered source fill triangles, never a
                # synthetic bounding rectangle. Explicit hitzone children, if
                # present, are included through the same contour walk.
                hits.setdefault(button, []).extend(transformed)
            if not input_only and color[3] > 0:
                art.append((path, character, bitmap, color, transformed))
        return
    if character in shape_failures:
        raise ValueError(f"Unsupported original pause shape {character} at {path}: {shape_failures[character]}")
    if character in edits:
        rec = edits[character]
        b = rec["bounds_twips"]
        points = [(matrix[0]*x+matrix[2]*y+matrix[4], matrix[1]*x+matrix[3]*y+matrix[5])
                  for x, y in ((b[0], b[2]), (b[1], b[2]), (b[1], b[3]), (b[0], b[3]))]
        bounds = [min(x for x, _ in points)/20, max(x for x, _ in points)/20,
                  min(y for _, y in points)/20, max(y for _, y in points)/20]
        fields.append((path, character, rec, bounds, matrix))
        return
    frames = sprites.get(character)
    if not frames:
        raise ValueError(f"Missing pause artwork definition {character} at {path}")
    if frame_override is not None:
        selected_frame = frame_override
    elif character == 755:
        selected_label = "Multi" if multiplayer else "Single"
        selected_frame = next((frame for label, frame in sprite_labels.get(character, {}).items()
                               if label.lower() == selected_label.lower()), 0)
    elif character == 77 and path.endswith("/btimg"):
        selected_frame = next((frame for label, frame in sprite_labels.get(character, {}).items()
                               if label.lower() == "hudpause"), 0)
    else:
        selected_frame = idle_frame(character) if button else 0
    if selected_frame < 0 or selected_frame >= len(frames):
        raise ValueError(f"Selected source frame {selected_frame} outside sprite {character}")
    for _, placement in sorted(frames[selected_frame].items()):
        name = placement.get("name", str(_))
        child_path = path + "/" + name
        child_button = child_path if name.startswith("btn") else button
        only_hit = input_only or name == "hitzone"
        placement_mult, placement_add = placement["color"]
        if placement_mult[3] == 0 and placement_add[3] == 0:
            continue
        child_mult = tuple(color_mult[i] * placement_mult[i] for i in range(4))
        child_add = tuple(color_add[i] + color_mult[i] * placement_add[i] for i in range(4))
        walk(placement["character"], multiply(matrix, placement["matrix"]), child_path,
             art, fields, hits, child_button, only_hit, child_mult, child_add,
             multiplayer=multiplayer, hidden_paths=hidden_paths, depth=depth + 1)


def find_placement(frames, name):
    matches = [p for p in frames[0].values() if p.get("name") == name]
    if len(matches) != 1:
        raise ValueError(f"Expected one source placement named {name}; got {len(matches)}")
    return matches[0]


def collect_surface(name, multiplayer=False):
    if name == "hud_pause_button":
        hud = find_placement(root, "menu_HUD_0")
        menu = find_placement(sprites[hud["character"]], "HUDelements")
        button = find_placement(sprites[menu["character"]], "btn_mainmenu")
        placement = {"character": button["character"],
                     "matrix": multiply(multiply(multiply(hud["matrix"], menu["matrix"]), button["matrix"]), [1,0,0,1,0,0])}
        root_name = "_root.menu_HUD_0.HUDelements.btn_mainmenu"
    else:
        source_name = "menu_Ingame" if name == "pause_page" else "menu_hud_confirm"
        placement = find_placement(root, source_name)
        root_name = "_root." + source_name
    art, fields, hits = [], [], {}
    # `parse_timeline` and source shapes retain twips. The only world-to-stage
    # conversion occurs once, at the final vertex divide-by-20 below.
    stage_matrix = [1., 0, 0, 1., 0, 0]
    frame_override = {"pause_page": 14, "confirmation": 13}.get(name)
    hidden_paths = (("_root.menu_Ingame/buttons/btn_Debug",)
                    if name == "pause_page" else ())
    walk(placement["character"], multiply(stage_matrix, placement["matrix"]), root_name,
         art, fields, hits, root_name if name == "hud_pause_button" else "",
         frame_override=frame_override, multiplayer=multiplayer, hidden_paths=hidden_paths)
    if name == "hud_pause_button":
        # The HUD hitzone sits on the selected button's frame-zero source tree.
        # Walk its exact sprite with the same accumulated matrix, preserving the
        # authored hit path and avoiding a visual-bounds fallback.
        button_path = root_name
    required = {
        "hud_pause_button": [root_name],
        "pause_page": ["_root.menu_Ingame/buttons/btn_MENU_CONTINUE",
                       "_root.menu_Ingame/buttons/btn_MENU_MAIN_MENU"],
        "confirmation": ["_root.menu_hud_confirm/WarningBox/btn_yes",
                         "_root.menu_hud_confirm/WarningBox/btn_no"],
    }[name]
    missing = [p for p in required if not hits.get(p)]
    if missing:
        raise ValueError(f"Authored source hit contours missing for {name}: {missing}")
    return art, fields, hits


def cpp_num(n):
    return swf.cpp_number(float(n))


def cpp_array(values):
    return "{" + ",".join(cpp_num(v) for v in values) + "}"


def cpp_vertices(vertices):
    return "{" + ",".join("{" + ",".join(cpp_num(v) for v in item) + "}" for item in vertices) + "}"


def surface_cpp(name, value):
    art, fields, hits = value
    lines = [f"static const frontend::art::ScreenArt art_{name} = []{{",
             "frontend::art::ScreenArt x;"]
    for path, ident, bitmap, color, vertices in art:
        lines.append(f"x.batches.push_back({{{json.dumps(path)},{ident},{cpp_vertices(vertices)}}});")
        lines.append(f"x.bitmap_ids.push_back({bitmap});")
        lines.append("x.batch_colors.push_back(" + cpp_array(color) + ");")
    for path, ident, rec, bounds, matrix in fields:
        local = [v / 20 for v in rec["bounds_twips"]]
        margins = [rec["layout"].get(k, 0) / 20 for k in ("left_margin", "right_margin", "indent")]
        metrics = fonts.get(rec["font"], {}).get("metrics", [0,0,0])
        fields_cpp = (f"frontend::art::TextField f;f.path={json.dumps(path)};f.character_id={ident};"
                      f"f.font_id={rec['font']};f.source_height={cpp_num(rec['height_twips']/20)};"
                      f"f.bounds={cpp_array(bounds)};f.rgba={{{','.join(str(c) for c in rec['rgba'])}}};"
                      f"f.align={rec['layout'].get('align',0)};f.matrix={cpp_array(matrix[:4]+[matrix[4]/20,matrix[5]/20])};"
                      f"f.local_bounds={cpp_array(local)};f.margins={cpp_array(margins)};"
                      f"f.leading={cpp_num(rec['layout'].get('leading',0)/20)};"
                      f"f.initial_text={json.dumps(rec.get('initial_text',''))};f.font_metrics={cpp_array(metrics)};"
                      f"f.word_wrap={str(bool(rec['flags'] & 0x4000)).lower()};f.multiline={str(bool(rec['flags'] & 0x2000)).lower()};"
                      "x.text_fields.push_back(std::move(f));")
        # Keep the local name short/readable while allowing multiple authored
        # text fields to coexist in the same source-frame lambda.
        lines.append("{" + fields_cpp + "}")
    for path, vertices in hits.items():
        lines.append(f"x.hit_regions.push_back({{{json.dumps(path)},{cpp_vertices(vertices)}}});")
    lines.extend(["return x;}();"])
    return lines


surfaces = {name: collect_surface(name) for name in ("hud_pause_button", "pause_page", "confirmation")}
surfaces["pause_page_multiplayer"] = collect_surface("pause_page", multiplayer=True)
lines = ["// Generated from original dqhud_droid.swf by export_source_pause_ui_art_v1.py.",
         '#include "source_pause_ui_render_v1.hpp"',
         "namespace dh::foundation::pause_ui {"]
for name, value in surfaces.items():
    lines.extend(surface_cpp(name, value))
lines += [
    "const frontend::art::ScreenArt& source_pause_ui_art_v1(SourcePauseSurfaceV1 surface,bool multiplayer) noexcept {",
    "switch(surface){case SourcePauseSurfaceV1::hud_pause_button:return art_hud_pause_button;",
    "case SourcePauseSurfaceV1::confirmation:return art_confirmation;",
    "case SourcePauseSurfaceV1::pause_page:return multiplayer?art_pause_page_multiplayer:art_pause_page;",
    "default:return art_pause_page;}}",
    "}"]
(HERE / "source_pause_ui_art_v1.cpp").write_text("\n".join(lines) + "\n", encoding="utf-8")

report = {
    "schema": "source-pause-ui-art-v1",
    "source": SWF_PATH.relative_to(ROOT).as_posix(),
    "source_bytes": len(raw),
    "source_sha256": hashlib.sha256(raw).hexdigest(),
    "generated_cpp": "source_pause_ui_art_v1.cpp",
    "generated_cpp_sha256": hashlib.sha256((HERE / "source_pause_ui_art_v1.cpp").read_bytes()).hexdigest(),
    "verification": {
        "test": "source_pause_ui_render_v1_source_test.py",
        "command": "py -3 port/windows-foundation/features/pause_ui/source_pause_ui_render_v1_source_test.py",
        "result": "Source-backed parser, source frame selection, exact triangle/text/hit data, contour hit probes, and route contract.",
        "native_compile": "Not run: no C++ compiler is available in this workspace PATH.",
    },
    "coordinate_system": "authored 480x320 stage pixels; source SWF twips transformed through root placement then divided by 20 once",
    "atlas": "data/3d/textures/MenusGraphics_droid.tga",
    "bitmap_definitions": bitmap_dimensions,
    "font_definitions": fonts,
    "used_font_assets": {"7": "data/Fontin SmallCaps.ttf", "512": "data/Fontin SmallCaps.ttf"},
    "surfaces": {},
    "exporter": Path(__file__).name,
    "triangulation": "checked source edge contours with the dependency-free source-hole bridge and ear-clipping routine; original fills remain separate bitmap/color batches",
    "timeline_policy": {
        "hud_pause_button": "source btn_mainmenu idle label frame0 settled at its authored stop before pressed frame5; nested btimg selected from onLoad HUDPause label frame1",
    "pause_page": "source menu_Ingame show label frame10 settled at authored stop frame14; page button sprites selected at their authored Idle labels/stops; btn_Multiplayer selects Single or Multi source label",
        "confirmation": "source menu_hud_confirm show label frame1 settled at its authored stop frame13 before hide frame14; Yes/No button sprites selected at authored idle labels/stops",
    },
    "source_timeline_labels": {str(ident): {"labels": sprite_labels.get(ident, {}),
                                                "stop_frames": sprite_stops.get(ident, [])}
                                for ident in (85, 77, 756, 755, 699, 697)},
    "selected_source_frames": {
        "hud_pause_button": {"button_85_idle_stop": 4, "hud_icon_77_HUDPause": 1},
        "pause_page": {"menu_Ingame_show_stop": 14, "button_531_idle_stop": 4,
                       "buttons_755_Single": 0, "buttons_755_Multi": 1},
        "confirmation": {"menu_hud_confirm_show_stop": 13, "button_697_idle_stop": 1},
    },
    "hit_policy": "No named hitzone exists for these source MovieClip controls. Hit contours are triangles from the exact visible source fill art under each authored btn_* clip at the selected frame; no synthetic rectangle is added.",
    "source_visibility_policy": {
        "hidden_controls": [{"path": "_root.menu_Ingame/buttons/btn_Debug",
                             "source_assignment": "this.buttons.btn_Debug._visible = false",
                             "action_block": "root/sprite756 FRAME 0 OFFSET 00028743",
                             "action_offset": "00028979..0002898c",
                             "reason": "The authored menu onShow handler hides this developer/debug button; its BtnText placeholder and source hit contour are omitted.",
                             "policy": "Do not render or hit-test this source branch."}],
        "other_pause_buttons": "The visible source Help/Options/Main Menu/Continue buttons and conditional source Multiplayer state remain as authored.",
        "transparent_cheat_controls": "Cheat1..4 placements are not selected for art/input because their source fill alpha composes to zero; source ActionScript exposes their hidden-code behavior but this feature creates no substitute controls.",
    },
    "action_script_evidence": {
        "dump": ".local-inputs/swf-layout-trigger/dqhud_droid-actions.txt",
        "sha256": hashlib.sha256((ROOT / ".local-inputs/swf-layout-trigger/dqhud_droid-actions.txt").read_bytes()).hexdigest(),
        "menu_ingame_on_show": "BLOCK root/sprite756 FRAME 0 OFFSET 00028743; ActionScript localizes five buttons and gotoAndPlay(\"Idle\") on source button clips.",
        "pause_icon_on_load": "BLOCK root FRAME 0 OFFSET 00030407; source HUD onLoad at 00031db1 calls btimg.gotoAndStop(\"HUDPause\") for HUDelements.btn_mainmenu.",
        "confirmation_on_push": "BLOCK root/sprite699 FRAME 0 OFFSET 00023726; localizes Yes/No and prompt; Yes routes NativeGoToMainMenu; No routes NativePopMenu.",
    },
}
for name, (art, fields, hits) in surfaces.items():
    if any(len(vertices) == 0 or len(vertices) % 3 for _, _, _, _, vertices in art):
        raise ValueError(f"{name}: generated source draw batch is not a triangle list")
    if any(not (0 <= vertex[2] <= 1 and 0 <= vertex[3] <= 1)
           for _, _, _, _, vertices in art for vertex in vertices):
        raise ValueError(f"{name}: original SWF UV lies outside its source bitmap")
    if any(not vertices or len(vertices) % 3 for vertices in hits.values()):
        raise ValueError(f"{name}: source hit contour is not a triangle list")
    report["surfaces"][name] = {
        "batch_count": len(art),
        "triangle_count": sum(len(x[4]) // 3 for x in art),
        "text_fields": [{"path": p, "character": ident, "font": rec["font"],
                         "height_twips": rec["height_twips"], "rgba": rec["rgba"],
                         "align": rec["layout"].get("align", 0), "bounds_twips": rec["bounds_twips"]}
                        for p, ident, rec, _, _ in fields],
        "hit_regions": {path: {"triangle_count": len(verts) // 3,
                               "bounds": [min(v[0] for v in verts), max(v[0] for v in verts),
                                          min(v[1] for v in verts), max(v[1] for v in verts)]}
                        for path, verts in hits.items()},
        "bitmap_ids": sorted({bitmap for _, _, bitmap, _, _ in art if bitmap}),
    }
(HERE / "source-pause-ui-art-v1.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
print("exported pause SWF surfaces:", {k: (len(v[0]), sum(len(x[4]) // 3 for x in v[0]), len(v[1]), len(v[2])) for k, v in surfaces.items()})
