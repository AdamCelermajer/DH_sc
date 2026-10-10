#!/usr/bin/env python3
"""Static regression for EnterName -> process MenuCharacterSelect::Show."""
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]


def body(path: str, start: str, end: str) -> str:
    text = (ROOT / path).read_text(encoding="utf-8")
    a = text.index(start)
    b = text.index(end, a + len(start))
    return text[a:b]


def ordered(text: str, *needles: str) -> None:
    cursor = -1
    for needle in needles:
        next_cursor = text.find(needle, cursor + 1)
        assert next_cursor >= 0, f"missing/out-of-order source step: {needle}"
        cursor = next_cursor


def main() -> None:
    startup = body(
        "port/android-native/app/src/main/cpp/native_process_startup_v119.inc",
        "bool model_renderer::native_process_derived_menu_show_v119(",
        "bool model_renderer::native_process_derived_menu_focus_v119(",
    )
    ordered(
        startup,
        "menu->render->identity!=render||owner->fields.render!=render",
        'owner->kind==MenuSingletonKindV67::enter_name',
        "if(!owner->fields.valid7c)",
        "return p->base_show(id,e)",
        "owner->kind==MenuSingletonKindV67::select_class",
        "native_menu_preview_select_show_v121(app,saved_slot,e)",
        "owner->select160=saved_slot",
        "if(!owner->fields.valid7c)",
        "if(!p->base_show(id,e))return false",
        "front_ui.process_class_select_show_v87(render,e)",
    )
    assert "MenuCharacterSelect::Show 0x428f38" in startup
    assert "EnterName42aef0" in startup

    hide = body(
        "port/android-native/app/src/main/cpp/native_selected_menu_hide_v119.inc",
        "case MenuSingletonKindV67::select_class:",
        "case MenuSingletonKindV67::merchant:",
    )
    assert "native_menu_preview_select_hide_v121(app,selected->select160,0,e)" in hide
    class_scene = body(
        "port/android-native/app/src/main/cpp/renderer_front_exports_v87.inc",
        "std::string load_class_scene(",
        "bool select_class_scene(",
    )
    ordered(class_scene,
        "class_scene=true", "register_class_selection_scene_root_v121(root_error)")
    native_preview = (ROOT / "port/android-native/app/src/main/cpp/renderer_native_menu_preview_v121.inc").read_text(encoding="utf-8")
    first = native_preview.index("bool register_class_selection_scene_root_v121(")
    last = native_preview.index("bool build_native_menu_preview_renderer_services_v121(", first)
    class_root = native_preview[first:last]
    ordered(class_root,
        "scene->add_child(root,e)", "class_selection_scene_node_v121=node", "node->drop(e)")
    first = native_preview.index("out.selected_scene_root=[native]")
    last = native_preview.index("out.remove_selected_root=[native]", first)
    selection_root = native_preview[first:last]
    assert "class_selection_scene_node_v121.lock()" in selection_root
    assert "menu_scene_node_v121.lock()" not in selection_root
    assert "borrow_registered_root_v110" in selection_root
    assert "Actual CLASS_SELECTION scene lacks its retained native root loan" in selection_root

    ida_hide = (ROOT / ".local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0042/004283c0.c").read_text(encoding="utf-8")
    ordered(ida_hide,
        "if ( result )", "MenuBase::Hide(this)", "if ( v3 )", "ObjectManager::Flush", "AnimSetManager::Flush",
        "MenuMainMenu::CreateAvatarCamera", "MenuMainMenu::SetupScene", "MenuMainMenu::SetupCharacter")
    ida_show = (ROOT / ".local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0042/00428f38.c").read_text(encoding="utf-8")
    ordered(ida_show,
        '"data/3D/OptimizedMaxFiles/CLASS_SELECTION.bdae"',
        "CColladaDatabase::constructScene", "*((_DWORD *)v1 + 49) = v10",
        "getSceneNodeFromType(*((_DWORD *)v1 + 49)")
    assert "if ( v3 )" in ida_hide

    front = body(
        "port/android-native/app/src/main/cpp/front_ui_session_v87.cpp",
        "bool FrontUiSessionV87::process_class_select_show_v87(",
        "bool FrontUiSessionV87::process_class_select_update_v87(",
    )
    ordered(
        front,
        "movie_slot_v93(2,actual,error)||actual.identity!=render",
        "self->process_class_select_active_v87=false",
        "self->movie->menu_action_script",
        "Impl::update_class(&q.self,graph,e)",
        'menu_display_callback("_root.menu_SelectClass.class_select"',
        "q.self.process_class_select_active_v87=true",
    )
    assert "MenuCharacterSelect.Show requires the same live primary2 RenderFX" in front

    bootstrap = (ROOT / "port/android-native/app/src/main/cpp/native_menu_process_bootstrap_v104.inc").read_text(encoding="utf-8")
    assert "request.menu->render==request.render" in bootstrap
    assert "model_renderer::native_process_derived_menu_show_v119(request.menu->identity,request.render->identity,e)" in bootstrap

    event_cpp = (ROOT / "port/level-world/event_manager_owner_v12.cpp").read_text(encoding="utf-8")
    assert 'EventManager cannot replay a failed delivery prefix' in event_cpp
    assert 'first failure: '+ '"+failed_reason_' in event_cpp
    assert "latch_failure(e,\"Actual EventManager handler rejected required delivery\")" in event_cpp

    print("PASS EnterName/SelectClass Show ordering, same RenderFX identity, and EventManager first-failure reporting")


if __name__ == "__main__":
    main()
