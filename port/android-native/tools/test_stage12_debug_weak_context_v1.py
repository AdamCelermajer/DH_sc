"""Source regression for IDA LoadMenu's live/expired debug weak-context paths.

This guard checks the actual Stage12/26 shared singleton source. It does not
claim callback execution, game runtime coverage, or a compiled host test.
"""
from pathlib import Path
import re


ROOT = Path(__file__).resolve().parents[3]
path = ROOT / "port/android-native/app/src/main/cpp/native_menu_singletons_v67.inc"
source = path.read_text(encoding="utf-8")
body = source.split("bool model_renderer::source_native_loadmenu3_singletons_v67(", 1)[1]
body = body.split("bool model_renderer::create_native_process_menu_singleton_v104(", 1)[0]

# A live existing weak context short-circuits Init and never earns the hide
# receipt. A NULL render or expired context enters the same Init path.
assert "bool debug_initialized{};" in body
initialize = re.search(r"auto initialize=\[&\]\(\)\{(.*?)\};", body, re.S)
assert initialize, "missing shared NULL/expired Init path"
assert re.search(
    r"if\(!source_singleton_init_v67\(\*provider,\*owner,e\)\)return false;"
    r"\s*if\(kind==MenuSingletonKindV67::debug\)debug_initialized=true;",
    initialize.group(1),
), "debug hide receipt must follow successful Init only"
assert "if(!owner->fields.render)return initialize();" in body
assert "return weak.live||initialize();" in body, "live weak context must bypass Init"

# Original4323c4..4323e0 uses MenuFX.State.GetCharacter, not the child text
# cache refreshed by MenuDebug.Init42a040. Its NULL result skips the store.
hide = body.split("if(auto* owner=source_singleton_v67[", 1)[1]
hide = hide.split("dh2::ui::MenuDebugHudOwnerV62* debug{};", 1)[0]
assert "debug_initialized&&owner&&owner->fields.render" in hide
assert "owner.state.weak_character_v59.borrow(graph,value,live,e)" in hide
assert "if(!live)return true;" in hide
assert "cached[0]" not in hide, "debug root hide must not target its text child"
assert "set_visible(false);owner.projection.visible=0;" in hide

print("PASS: live debug weak context bypasses Init/hide; NULL/expired context hides the registered root after successful Init")
