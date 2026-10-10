"""Regression for authored main-menu pointer input and native action routing."""
from pathlib import Path
import hashlib


ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"


def section(path: str, start: str, end: str) -> str:
    source = (ROOT / path).read_text(encoding="utf-8")
    a = source.index(start)
    b = source.index(end, a + len(start))
    return source[a:b]


def main() -> None:
    # This is the exact authored Android menu movie whose ActionScript was
    # inspected for btn_MENU_SINGLE_PLAYER, btn_MENU_OPTIONS, and btn_MENU_INFO.
    swf = ROOT / "port/android-native/app/src/main/assets/original-cache/data/menus/dqmenus_droid.swf"
    data = swf.read_bytes()
    assert hashlib.sha256(data).hexdigest() == "c114c7d39b7fe0351a93a515c78f9457dda715d78aed6e3bcc3f4895120de421"
    for action in (
        "onRelease", "btn_MENU_SINGLE_PLAYER", "NativeAssignSaveSlotToPlayer",
        "NativeStartGame", "NativePushMenu", "menu_EnterName", "menu_StartGame",
        "menu_Options", "menu_info",
    ):
        assert action.encode("ascii") in data, f"authored menu action missing from pinned SWF: {action}"

    # Android pointer phases become genuine App EventManager input events;
    # releases carry pressed=0 and the source cursor slot through unchanged.
    transport = section(
        "port/android-native/app/src/main/cpp/native_menu_application_event_v120.inc",
        "bool dispatch_source_app_touch_v121(",
        "}\n}",
    )
    for expected in (
        "packet->type=move?5:4",
        "packet->fields.index_c=static_cast<std::int32_t>(slot)",
        "packet->fields.pressed10=down?1:0",
        "event.input_v120=",
        "app->events14()->raise(event,e)",
        "if(phase==2){active[slot]=0;t.physical[slot]=-1;}",
    ):
        assert expected in transport, f"Android-to-App input leg missing: {expected}"

    dispatch = section(
        "port/android-native/app/src/main/cpp/native_menu_application_event_v120.inc",
        "bool source_menu_application_event_v119(",
        "// Android frontend pointer identities",
    )
    for expected in (
        "cursor=payload.index_c;pressed=payload.pressed10",
        "if(!source_main_menu_input_enabled_v120)continue",
        "input_cursor_slot_v120({x,y,0.f,pressed},static_cast<std::uint32_t>(cursor),e)",
    ):
        assert expected in dispatch, f"App-to-authored-SWF input leg missing: {expected}"
    assert "source_animation_scaling_enabled_v99()" not in dispatch, (
        "main-menu input must use MenuMainMenu::Show admission, not campaign animation preference"
    )

    startup = section(
        "port/android-native/app/src/main/cpp/native_process_startup_v119.inc",
        "case 26:{",
        "default:",
    )
    for expected in (
        "source_menu_application_event_v119(self.app.lock(),*n,event,result,e)",
        "constexpr std::int32_t types[]{5,4,7}",
        "events->attach(types[attach_index],receiver,10,inserted,e)",
    ):
        assert expected in startup, f"actual Application event receiver missing: {expected}"

    front = (CPP / "front_ui_session_v87.cpp").read_text(encoding="utf-8")
    for action in (
        "NativePushMenu", "NativePopMenu", "NativeAssignSaveSlotToPlayer",
        "NativeStartGame",
    ):
        assert f'"{action}"' in front, f"main-menu native action is not registered/handled: {action}"
    assert 'requested=="menu_Options"||requested=="menu_info"' in front
    push = section(
        "port/android-native/app/src/main/cpp/front_ui_session_v87.cpp",
        "static bool push_menu(void* context,const char* name,std::string& error){",
        "static bool pop_top_menu(void* context,std::string& error){",
    )
    assert 'std::strcmp(name,"menu_info")' in push
    assert 'std::strcmp(name,"menu_EnterName")' in push
    assert 'std::strcmp(name,"menu_StartGame")' in push
    assert 'shared_state(name)' in push  # Includes menu_Options.
    assert "transition_menu(previous,name,true,error)" in push
    transition = section(
        "port/android-native/app/src/main/cpp/front_ui_session_v87.cpp",
        "bool transition_menu(const std::string& previous,const std::string& next,bool pushed,std::string& error){",
        "static bool push_menu(void* context,const char* name,std::string& error){",
    )
    for expected in (
        "menu_action_script(&hide,change_menu,error)",
        "menu_action_script(&show,change_menu,error)",
    ):
        assert expected in transition, f"authored menu transition callback missing: {expected}"
    callbacks = section(
        "port/android-native/app/src/main/cpp/front_ui_session_v87.cpp",
        "static bool change_menu(void* context,ui::SwfAsGraph& graph,std::string& error){",
        "bool transition_menu(const std::string& previous,const std::string& next,bool pushed,std::string& error){",
    )
    assert '"onHide"' in callbacks and '"onShow"' in callbacks

    print("PASS pinned authored SWF actions, App pointer press/release delivery, main-menu admission, and Options/Info transition routing")


if __name__ == "__main__":
    main()
