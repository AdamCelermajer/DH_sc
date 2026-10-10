"""Source-backed contracts for the exported pause/HUD frame and routes."""
from __future__ import annotations

import hashlib
import json
import runpy
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent
EXPORT = HERE / "export_source_pause_ui_art_v1.py"
API = HERE / "source_pause_ui_render_v1.cpp"
HEADER = HERE / "source_pause_ui_render_v1.hpp"
GENERATED = HERE / "source_pause_ui_art_v1.cpp"
REPORT = HERE / "source-pause-ui-art-v1.json"


def require(condition: bool, message: str) -> None:
    if not condition:
        raise SystemExit("SourcePauseUiRenderV1 FAIL: " + message)


# Reparse the original SWF; the published C++/receipt cannot silently drift
# away from the actual source asset without failing this focused check.
source = runpy.run_path(str(EXPORT))
surfaces = source["surfaces"]
report = json.loads(REPORT.read_text(encoding="utf-8"))
raw = (ROOT / report["source"]).read_bytes()
require(hashlib.sha256(raw).hexdigest() == report["source_sha256"], "original SWF hash")
require(report["source_sha256"] == "a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238", "audited source asset identity")
require(hashlib.sha256(GENERATED.read_bytes()).hexdigest() == report["generated_cpp_sha256"], "generated source checksum")
require(report["font_definitions"]["7"]["name"] == "Fontin SmallCaps", "pause text source font face")
require(report["used_font_assets"]["7"] == "data/Fontin SmallCaps.ttf", "pause text source font asset")
action_dump = ROOT / report["action_script_evidence"]["dump"]
actions = action_dump.read_text(encoding="utf-8")
require(hashlib.sha256(action_dump.read_bytes()).hexdigest() == report["action_script_evidence"]["sha256"], "ActionScript evidence hash")
require('00028979 push_data {"values": [{"constant": 1, "text": "buttons"}]}' in actions and
        '00028984 get_member {}' in actions and
        '00028985 push_data {"values": [{"constant": 3, "text": "_visible"}, false]}' in actions and
        '0002898c set_member {}' in actions,
        "source menu onShow unconditionally sets buttons.btn_Debug._visible=false")
hidden = report["source_visibility_policy"]["hidden_controls"]
require(hidden == [{
    "path": "_root.menu_Ingame/buttons/btn_Debug",
    "source_assignment": "this.buttons.btn_Debug._visible = false",
    "action_block": "root/sprite756 FRAME 0 OFFSET 00028743",
    "action_offset": "00028979..0002898c",
    "reason": "The authored menu onShow handler hides this developer/debug button; its BtnText placeholder and source hit contour are omitted.",
    "policy": "Do not render or hit-test this source branch.",
}], "source Debug visibility branch is recorded exactly")

expected = {
    "hud_pause_button": (2, 4, 1),
    "pause_page": (33, 66, 4),
    "confirmation": (25, 52, 2),
    "pause_page_multiplayer": (36, 72, 5),
}
for name, (batch_count, triangle_count, hit_count) in expected.items():
    art, fields, hits = surfaces[name]
    observed = (len(art), sum(len(batch[4]) // 3 for batch in art), len(hits))
    require(observed == (batch_count, triangle_count, hit_count), f"{name} original graph counts {observed}")
    require(all(len(vertices) and len(vertices) % 3 == 0 for vertices in hits.values()), f"{name} hit triangle lists")
    require(all(len(batch[4]) and len(batch[4]) % 3 == 0 for batch in art), f"{name} art triangle lists")
    require(all(0 <= v[2] <= 1 and 0 <= v[3] <= 1 for batch in art for v in batch[4]), f"{name} exact atlas UVs")
    require(not any("btn_Debug" in batch[0] for batch in art), f"{name} omits source-hidden Debug art")
    require(not any("btn_Debug" in field[0] for field in fields), f"{name} omits source-hidden Debug placeholder text")
    require(not any("btn_Debug" in path for path in hits), f"{name} omits source-hidden Debug hit contour")
    require(report["surfaces"][name]["bitmap_ids"] == [1], f"{name} dqhud atlas identity")
    for field in report["surfaces"][name]["text_fields"]:
        require(field["font"] == 7 and field["rgba"] == [255, 204, 102, 255] and field["align"] == 2,
                f"{name} source field face/color/alignment: {field['path']}")

required_hits = {
    "hud_pause_button": ["_root.menu_HUD_0.HUDelements.btn_mainmenu"],
    "pause_page": [
        "_root.menu_Ingame/buttons/btn_MENU_CONTINUE",
        "_root.menu_Ingame/buttons/btn_MENU_HELP",
        "_root.menu_Ingame/buttons/btn_MENU_OPTIONS",
        "_root.menu_Ingame/buttons/btn_MENU_MAIN_MENU",
    ],
    "confirmation": [
        "_root.menu_hud_confirm/WarningBox/btn_yes",
        "_root.menu_hud_confirm/WarningBox/btn_no",
    ],
}
for surface, paths in required_hits.items():
    for path in paths:
        require(path in surfaces[surface][2], f"actual source hit contour {path}")

def source_triangle_hit(vertices, point):
    x, y = point
    for i in range(0, len(vertices), 3):
        a, b, c = vertices[i:i + 3]
        e0 = (b[0]-a[0])*(y-a[1]) - (b[1]-a[1])*(x-a[0])
        e1 = (c[0]-b[0])*(y-b[1]) - (c[1]-b[1])*(x-b[0])
        e2 = (a[0]-c[0])*(y-c[1]) - (a[1]-c[1])*(x-c[0])
        if (e0 >= -1e-5 and e1 >= -1e-5 and e2 >= -1e-5) or (e0 <= 1e-5 and e1 <= 1e-5 and e2 <= 1e-5):
            return True
    return False


# The representative points are centroids of real source triangles, so this
# exercises the contours the C++ hit test consumes rather than hit-box bounds.
for surface, path in (("hud_pause_button", required_hits["hud_pause_button"][0]),
                      ("pause_page", required_hits["pause_page"][0]),
                      ("pause_page", required_hits["pause_page"][3]),
                      ("confirmation", required_hits["confirmation"][0]),
                      ("confirmation", required_hits["confirmation"][1])):
    vertices = surfaces[surface][2][path]
    a, b, c = vertices[:3]
    center = ((a[0]+b[0]+c[0])/3, (a[1]+b[1]+c[1])/3)
    require(source_triangle_hit(vertices, center), f"source contour centroid hit for {path}")

pause_frame = surfaces["pause_page"][0]
multi_frame = surfaces["pause_page_multiplayer"][0]
require(any("btn_Multiplayer" in batch[0] for batch in multi_frame), "authored multiplayer button on Multi frame")
require(not any("btn_Multiplayer" in batch[0] for batch in pause_frame), "Single source frame does not invent multiplayer button")
require(report["timeline_policy"]["hud_pause_button"].find("HUDPause") >= 0, "source icon onLoad frame policy")
require("No named hitzone exists" in report["hit_policy"], "no invented hitzone; source fill geometry policy")
selected = report["selected_source_frames"]
require(selected == {
    "hud_pause_button": {"button_85_idle_stop": 4, "hud_icon_77_HUDPause": 1},
    "pause_page": {"menu_Ingame_show_stop": 14, "button_531_idle_stop": 4,
                   "buttons_755_Single": 0, "buttons_755_Multi": 1},
    "confirmation": {"menu_hud_confirm_show_stop": 13, "button_697_idle_stop": 1},
}, "authored source frame selection")
labels = source["sprite_labels"]
stops = source["sprite_stops"]
require(labels[85].get("idle") == 0 and 4 in stops[85] and labels[85].get("pressed") == 5,
        "HUD pause source idle stop before pressed")
require(labels[77].get("HUDPause") == 1, "HUD icon source label")
require(labels[756].get("show") == 10 and 14 in stops[756], "pause page source show endpoint")
require(labels[755].get("Single") == 0 and labels[755].get("Multi") == 1,
        "source multiplayer placement state")
require(labels[699].get("show") == 1 and 13 in stops[699], "confirmation source show endpoint")
require(labels[697].get("idle") == 0 and 1 in stops[697], "confirmation button source idle endpoint")

api_text = API.read_text(encoding="utf-8")
header_text = HEADER.read_text(encoding="utf-8")
generated_text = GENERATED.read_text(encoding="utf-8")
for fragment in (
    "source_pause_ui_frame_v1", "source_pause_ui_hit_test_v1", "source_pause_ui_route_v1",
    "open_pause_page", "resume_game", "open_main_menu_confirmation",
    "return_to_main_menu", "cancel_confirmation", "unsupported_action",
    "existing menu owner", "not actionable on this pause surface",
):
    require(fragment in api_text, f"route API contract {fragment}")
require("if (std::abs(area) < 1e-6f) continue;" in api_text, "degenerate hit triangles are ignored")
for fragment in ("SourcePauseUiFrameV1", "source_pause_ui_hit_test_v1", "source_pause_ui_route_v1",
                 "source_pause_ui_texture_file_v1", "source_pause_ui_font_file_v1",
                 "source_pause_ui_text_bindings_v1"):
    require(fragment in header_text, f"root-callable frame API {fragment}")
for fragment in ("MENU_CONTINUE", "MENU_HELP", "MENU_MULTIPLAYER", "MENU_OPTIONS", "MENU_MAIN_MENU",
                 "GLOBAL_ASK_FOR_MAINMENU", "GAMEPLAYMENUS_ACCEPT", "GAMEPLAYMENUS_REFUSE"):
    require(fragment in api_text, f"authored text binding {fragment}")
for fragment in ("_root.menu_HUD_0.HUDelements.btn_mainmenu", "_root.menu_Ingame/buttons/btn_MENU_CONTINUE",
                 "_root.menu_Ingame/buttons/btn_MENU_MAIN_MENU", "_root.menu_hud_confirm/WarningBox/btn_yes",
                 "_root.menu_hud_confirm/WarningBox/btn_no", "font_metrics"):
    require(fragment in generated_text, f"generated source geometry/styles {fragment}")

print("SourcePauseUiRenderV1 PASS: original HUD/pause/confirmation triangles, text styles, atlas, selected source timelines, source-contour hit tests, and supported route projection")
