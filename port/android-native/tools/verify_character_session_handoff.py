#!/usr/bin/env python3
"""Guard the menu-start Character session and native Level ownership splice."""

from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
RENDERER = ROOT / "port/android-native/app/src/main/cpp/model_renderer.cpp"
NATIVE_APP = ROOT / "port/android-native/app/src/main/cpp/native_app.cpp"
ACTIVITY = ROOT / "port/android-native/app/src/main/java/com/example/dh2/MainActivity.java"
LEVEL_OWNER_H = ROOT / "port/level-world/source_level_owner_v1.hpp"
LEVEL_OWNER_CPP = ROOT / "port/level-world/source_level_owner_v1.cpp"


def require(condition: bool, message: str) -> None:
    if not condition:
        raise SystemExit("FAIL " + message)


renderer = RENDERER.read_text(encoding="utf-8")
native_app = NATIVE_APP.read_text(encoding="utf-8")
activity = ACTIVITY.read_text(encoding="utf-8")
level_owner_h = LEVEL_OWNER_H.read_text(encoding="utf-8")
level_owner_cpp = LEVEL_OWNER_CPP.read_text(encoding="utf-8")

factory = "create_character_session("
require(renderer.count(factory) == 1, "fresh Character session factory must have one renderer callsite")
factory_at = renderer.index(factory)
fresh_at = renderer.rfind("PlayerCombat fresh_player;", 0, factory_at)
profile_at = renderer.index("fresh_player.profile_characters", factory_at)
session = renderer[fresh_at:profile_at]

require("if(!restore){" in session and "if(native_host.metadata){" in session,
        "fresh gameplay must select a Character session only on non-restore loads")
require("create_character_session(\n           prince_character.owner(),character_session,error)" in session,
        "authored menu start must bind the selected slot to the actual live Character identity")
for destination, source in (("save_transport", "transport"),
                            ("save_profile", "profile"),
                            ("savegame", "save")):
    require(session.count(f"fresh_player.{destination}=std::move(character_session.{source})") == 1,
            f"metadata Character session must adopt its {source} owner exactly once")

require("if(menu_gameplay_slot>=0)" in session and
        "Menu Start has no loaded Native metadata owner" in session,
        "menu Start must fail closed when selected-slot metadata is missing")
fallback_at = session.index("Development fixture loads without a selected menu profile")
fallback = session[fallback_at:]
require(fallback.index("make_shared<dh2::data::PlayerSavegameV1>()") <
        fallback.index("make_shared<dh2::data::PlayerSaveProfileV1>()") <
        fallback.index("make_shared<dh2::native::player_profile::Transport>"),
        "direct fixture fallback must construct one Save→Profile→Transport bundle")
require("fresh_player.quests=std::make_shared<dh2::native::quests::Owner>(fresh_player.savegame" in session,
        "QEST owner must use the selected branch's exact Character Save")

restore_at = session.index("}else{\n     if(!prince_combat.savegame")
restore = session[restore_at:]
for member in ("savegame", "save_profile", "save_transport", "quests"):
    require(f"fresh_player.{member}=prince_combat.{member};" in restore,
            f"surface restore must reuse the retained {member} owner")
post_session = renderer[profile_at:]
require("fresh_player.save_transport->bind(" in post_session
        and "&fresh_player.save_transport->loader()" in post_session,
        "transport must bind and load through its own Save identity")
require("prince_character.bind_session(&selected_record->character_660,\n         *prince_combat.savegame" in renderer,
        "Character must bind to the same Save later moved into the retained combat owner")
require(renderer.count("prince_combat=std::move(fresh_player)") == 1,
        "fresh session bundle must be published to the live Character once")

start = renderer[renderer.index("std::string start_menu_game("):]
require("prepare_native_menu_start(" in start and "load_world(payload.dwld_v1.data()" in start,
        "deferred authored Start must resolve its plan before native Level loading")
prepare_at = renderer.index("bool prepare_native_menu_start(")
prepare_end = renderer.index("\nbool request_menu_start(", prepare_at)
prepare = renderer[prepare_at:prepare_end]
require("native_host.prepare_metadata(runtime_root,characters,native_save_difficulty)" in prepare
        and "metadata_ready_for_slot(slot)" in prepare,
        "NativeStartGame must prepare the selected PlayerInfo +680 owner before resolving its plan")
require("begin_native_start_game_save_transaction_v1(\n      save_owner,levels,request,save_transaction" in prepare
        and "save_spawn_flag_before_native_load_v1(\n       save_owner,save_transaction" in prepare,
        "NativeStartGame plan and both SG_Save edges must use one transaction owner")
require("native_host.metadata->save_identity()" in prepare
        and "identity!=context.metadata->save_identity()" in prepare
        and "Metadata temporary_save" not in prepare,
        "launch transaction must reject identity changes and must not create a temporary metadata Save")
load_world = renderer[renderer.index("std::string load_world("):]
metadata_start = load_world.index("if(native_host.metadata_slot>=0){")
metadata_end = load_world.index("const auto& receipt=native_host.metadata->receipt();", metadata_start)
managed_metadata = load_world[metadata_start:metadata_end]
require("metadata_ready_for_slot(menu_gameplay_slot)" in managed_metadata
        and "Native selected PlayerInfo +680 Save was not retained through Crypt load" in managed_metadata,
        "Crypt load must retain and verify the same prepared metadata owner rather than preparing another")
require("source_level_owner.request_level(selected_level_row,source_entry_point," in load_world
        and "source_level_owner.publish_projection(&level)" in load_world
        and "source_level_owner.bind_native_level(binding)" in load_world,
        "selected Crypt Level row must flow through request, scene publication, and native lifecycle binding")
require("Source GSLevel/Level/LevelSavegame identities are optional" in level_owner_h
        and "until a real constructor/provider is selected" in level_owner_h,
        "source owner must document that native projection does not imply original Level construction")
require("source_level_owner.bind_source_chain(" not in renderer,
        "renderer must not present the synthetic native lifecycle as an original source owner chain")
require("state_.source_gslevel = reinterpret_cast<std::uintptr_t>(this)" in level_owner_cpp
        and "&native_level_identity_" in level_owner_cpp,
        "native binding must remain visibly distinct from a source GSLevel/Level identity")
require("menu_start_commit_gate_v1(world_loaded,hud_attached)" in native_app,
        "Java must only commit a playable world after source player HUD attachment")
require("NativeBridge.consumeMenuLaunch(nativeMenuLaunchRequest)" in activity
        and "acceptGameStart(startSlot,start,numeric,difficulty)" in activity,
        "GL frame must drain the authored launch request and deliver its result")
draw_at = native_app.index("Java_com_example_dh2_NativeBridge_draw(")
draw_end = native_app.index("Java_com_example_dh2_NativeBridge_", draw_at + 1)
draw_jni = native_app[draw_at:draw_end]
require("if(model_renderer::active())" in draw_jni
        and "model_renderer::draw(surface_width,surface_height)" in draw_jni,
        "first post-commit JNI draw must enter the active native world renderer")
require(activity.index("NativeBridge.draw();") <
        activity.index("NativeBridge.consumeMenuLaunch(nativeMenuLaunchRequest)") <
        activity.index("String start=startMenuGame(startSlot,numeric,difficulty)") <
        activity.index("acceptGameStart(startSlot,start,numeric,difficulty)"),
        "start is consumed after the current menu draw; the world draw follows launch return")

print("PASS Character session handoff: menu Save distinct; one fresh bundle; restore reuses; direct fixture fallback isolated")
print("PASS native level handoff: selected row -> scene projection/native lifecycle -> HUD commit -> first active JNI draw")
print("INFO original GSLevel/Level constructor chain remains unbound; first gameplay frame is render-path evidence only")
