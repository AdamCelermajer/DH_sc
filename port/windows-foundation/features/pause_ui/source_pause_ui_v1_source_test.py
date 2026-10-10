"""Check pause projection paths, labels and actions against original SWF evidence."""
from pathlib import Path
import hashlib
import json
import struct

ROOT = Path(__file__).resolve().parents[4]
ASSETS = ROOT / "port/android-native/app/src/main/assets/original-cache"
HUD_SWF = ASSETS / "data/menus/dqhud_droid.swf"
MENU_TEXT = ASSETS / "data/text/menu.english"
MENU_SYMBOLS = ASSETS / "data/text/menu.symbols"
GLOBAL_TEXT = ASSETS / "data/text/global.english"
GLOBAL_SYMBOLS = ASSETS / "data/text/global.symbols"
GAMEPLAY_TEXT = ASSETS / "data/text/gameplaymenus.english"
GAMEPLAY_SYMBOLS = ASSETS / "data/text/gameplaymenus.symbols"
ACTION_DUMP = ROOT / ".local-inputs/swf-layout-trigger/dqhud_droid-actions.txt"
FIRST_FRAME = ROOT / ".local-inputs/ui-layout-discovery/first-frame-paths.json"
LIVE_TREE = ROOT / ".local-inputs/authored-hud-live-tree.txt"
VIDEO_FRAME = ROOT / ".local-inputs/referenceframes/dh2-act1/first-combat/at-0300s.png"
API = ROOT / "port/windows-foundation/features/pause_ui/source_pause_ui_v1.cpp"
API_HEADER = ROOT / "port/windows-foundation/features/pause_ui/source_pause_ui_v1.hpp"


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def read_table(path: Path) -> list[str]:
    data = path.read_bytes()
    count, = struct.unpack_from("<H", data)
    offset = 2
    result = []
    for _ in range(count):
        size, = struct.unpack_from("<H", data, offset)
        offset += 2
        result.append(data[offset:offset + size].rstrip(b"\0").decode("utf-8"))
        offset += size
    assert offset == len(data)
    return result


assert sha(HUD_SWF) == "a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238"
assert sha(MENU_TEXT) == "50e01954a9104b7b3c08bcbab0985c1e516cc873645854cc6fbdaa65a9fad101"
assert sha(GLOBAL_TEXT) == "020a1779505fda2387cd4b2506aa0ac2ae04d7467405edb10eaefdff3049e568"
assert sha(GAMEPLAY_TEXT) == "ce07a456aaf8a8d914b210e62e40a5606bcc9a3a40348c958063bcc10e3c4000"
assert sha(ACTION_DUMP) == "fdef41abfcedf0225faccada8e2ea0ed6f589e26dcf799a6493072e83d0b4664"
assert sha(FIRST_FRAME) == "7381fe4299881a8331819b2afc3cab129e9cc11901e902483fb661cba3c4bf2b"
assert sha(LIVE_TREE) == "f005cf16dd52ead9d2fd0b1ad1919b6a525a6322bdf0b610615b8d7ed59bdbe2"
assert sha(VIDEO_FRAME) == "2dc4d26541fc19f2511b035f10aab7977997f3cd73a71db496dabbbd236baf19"

for path in (MENU_TEXT, MENU_SYMBOLS, GLOBAL_TEXT, GLOBAL_SYMBOLS, GAMEPLAY_TEXT, GAMEPLAY_SYMBOLS):
    assert path.is_file()
menu_pairs = dict(zip(read_table(MENU_SYMBOLS), read_table(MENU_TEXT)))
global_pairs = dict(zip(read_table(GLOBAL_SYMBOLS), read_table(GLOBAL_TEXT)))
gameplay_pairs = dict(zip(read_table(GAMEPLAY_SYMBOLS), read_table(GAMEPLAY_TEXT)))
assert all(key in menu_pairs for key in ("MENU_CONTINUE", "MENU_HELP", "MENU_OPTIONS", "MENU_MAIN_MENU", "MENU_MULTIPLAYER"))
assert global_pairs.get("GLOBAL_ASK_FOR_MAINMENU") == "Return to main menu?"
assert "GAMEPLAYMENUS_ACCEPT" in gameplay_pairs and "GAMEPLAYMENUS_REFUSE" in gameplay_pairs

actions = ACTION_DUMP.read_text(encoding="utf-8")
first = json.loads(FIRST_FRAME.read_text(encoding="utf-8"))["dqhud_droid"]
api = API.read_text(encoding="utf-8")
api_header = API_HEADER.read_text(encoding="utf-8")
live = LIVE_TREE.read_text(encoding="utf-8")

def character(path: str) -> int:
    return next(row["character_id"] for row in first if row["path"] == path)


assert character("_root.menu_HUD_0") == 470
assert character("_root.menu_HUD_0.HUDelements.btn_mainmenu") == 85
assert character("_root.menu_HUD_0.HUDelements.btn_mainmenu.btimg") == 77
assert character("_root.menu_HUD_0.HUDelements.btn_mainmenu.btimg.HudChar") == 41
assert character("_root.menu_Ingame") == 756
assert character("_root.menu_Ingame.buttons") == 755
assert character("_root.menu_Ingame.buttons.btn_MENU_CONTINUE") == 531
assert character("_root.menu_Ingame.buttons.btn_MENU_HELP") == 531
assert character("_root.menu_Ingame.buttons.btn_MENU_OPTIONS") == 531
assert character("_root.menu_Ingame.buttons.btn_MENU_MAIN_MENU") == 531
assert character("_root.menu_Ingame.buttons.btn_Multiplayer") == 754
assert character("_root.menu_hud_confirm") == 699
assert character("_root.menu_hud_confirm.WarningBox.btn_yes") == 697
assert character("_root.menu_hud_confirm.WarningBox.btn_no") == 697

def placement(path: str) -> tuple[int, int, int]:
    row = next(item for item in first if item["path"] == path)
    return row["depth"], int(row["matrix_twips"][4]), int(row["matrix_twips"][5])


assert placement("_root.menu_Ingame.buttons.btn_MENU_CONTINUE") == (13, 90, -67)
assert placement("_root.menu_Ingame.buttons.btn_MENU_HELP") == (7, 90, 819)
assert placement("_root.menu_Ingame.buttons.btn_Multiplayer") == (31, -7289, 1114)
assert placement("_root.menu_Ingame.buttons.btn_MENU_OPTIONS") == (1, 90, 1767)
assert placement("_root.menu_Ingame.buttons.btn_MENU_MAIN_MENU") == (19, 90, 2653)
assert placement("_root.menu_hud_confirm.WarningBox.btn_yes") == (10, 270, 1185)
assert placement("_root.menu_hud_confirm.WarningBox.btn_no") == (16, 2782, 1185)
assert placement("_root.menu_HUD_0.HUDelements.btn_mainmenu") == (11, 670, 1878)

for token in ("NativePushState", "menu_Ingame", "NativePauseAllSounds", "MenuBack"):
    assert token in actions
assert "NatvieResumeAllSounds" in actions  # preserve source's authored misspelling
assert "NativePopAllMenus" in actions and "NativeBackToHud" in actions
assert "menu_Options" in actions and "menu_HelpButtons" in actions
assert "NativePushMenu" in actions and "GLOBAL_ASK_FOR_MAINMENU" in actions
assert "NativeGoToMainMenu" in actions and "GAMEPLAYMENUS_ACCEPT" in actions
assert "GAMEPLAYMENUS_REFUSE" in actions and "NativePopMenu" in actions
assert "NativeIsMultiplayerGame" in actions and '"Multi"' in actions and '"Single"' in actions
assert "btn_mainmenu" in live and "menu_HUD_0.HUDelements.btn_mainmenu" in live

# The v1.0.3 reference screenshot at t=300s visibly shows the usual circular
# pause mark at the upper-left of the active combat HUD. It is visual corroboration
# for the source HUD control location/icon, not evidence of pixel parity with v1.0.2.
assert VIDEO_FRAME.is_file() and sha(VIDEO_FRAME)
assert 'gameplay_pause_button_path' in api_header and "_root.menu_HUD_0.HUDelements.btn_mainmenu" in api
for action_name in ("resume_game", "open_help", "open_multiplayer", "open_options",
                    "request_main_menu_confirmation", "confirm_return_to_main_menu",
                    "cancel_confirmation"):
    assert action_name in api

print("SourcePauseUiV1 PASS: exact HUD/menu/confirmation SWF clips, button labels/routes, conditional multiplayer state, and 300s reference HUD control evidence")
