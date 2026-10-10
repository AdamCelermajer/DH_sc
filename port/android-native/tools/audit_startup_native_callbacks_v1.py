#!/usr/bin/env python3
"""Guard the two callbacks reported missing by the original main-menu onPush."""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[3]
SESSION = ROOT / "port/android-native/app/src/main/cpp/front_ui_session_v87.cpp"
SOURCE_MOVIE = ROOT / "port/engine-ui/swf_source_movie_v1.cpp"
MOVIE = ROOT / "port/engine-ui/swf_movie.cpp"
ACTION_GRAPH = ROOT / "port/engine-ui/swf_actionscript_connection.cpp"

session = SESSION.read_text(encoding="utf-8")
source_movie = SOURCE_MOVIE.read_text(encoding="utf-8")
movie = MOVIE.read_text(encoding="utf-8")
action_graph = ACTION_GRAPH.read_text(encoding="utf-8")

required = ("NativeHasPushNotification", "NativeOnlineSanityCheck", "NativeStartFromGCInvite")
main_registration = session.split('if(front_screen=="main"){', 1)[1].split("}\n        if(front_screen==\"main\")for", 1)[0]

for name in required:
    assert f'services.native_actions.emplace_back("{name}")' in main_registration, f"{name} missing from main-menu callback registration"
    assert f'if(!std::strcmp(name,"{name}"))' in session, f"{name} missing from the native dispatcher"

# The source and text-platform wrappers must retain the native callback vector
# and typed callback while loading the actual SWF player.
assert "auto out=original" in source_movie and "out.native_action=SourceOwner::native_as" in source_movie
assert "as->install_native(s.native_actions,e)" in movie
assert "set_member(n.c_str(),gameswf::as_value(fn.get_ptr()))" in action_graph

print("startup native callback route PASS: both onPush callbacks are registered, dispatched, and installed on the retained SWF global")
