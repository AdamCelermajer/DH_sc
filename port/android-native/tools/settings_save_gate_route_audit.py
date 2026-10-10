#!/usr/bin/env python3
"""Guard NativeSaveSettings' gate, shared front/gameplay route, and language tail."""
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"


def body(source: str, start: str, end: str) -> str:
    a = source.index(start)
    b = source.index(end, a)
    return source[a:b]


front = (CPP / "front_ui_session_v87.cpp").read_text(encoding="utf-8")
native_save = body(front, "static bool save_settings(void* context", "static bool option_string(")
assert "save_process_settings_v102(app,error)" in native_save
assert native_save.index("save_process_settings_v102(app,error)") < native_save.index("set_language(self.settings->language()")
assert "fopen(" not in native_save and "fwrite(" not in native_save and "rename(" not in native_save

worker = (CPP / "source_settings_update_job_v102.cpp").read_text(encoding="utf-8")
save = body(worker, "bool save_process_settings_v102(", "\n}\n}")
gate = save.index("source_save_settings_gate_v102()")
files = save.index("source_save_files_v61()")
assert gate < files
assert "if(!settings->source_save_settings_gate_v102()){error.clear();return true;}" in save
assert "write_settings_stream_v102" in save

bootstrap = (CPP / "native_menu_process_bootstrap_v104.inc").read_text(encoding="utf-8")
assert "settings_actions.save=[](std::string& error){return front_ui.save_process_settings_v119(error);};" in bootstrap
assert "if(q.operation==O::save_settings)return front_ui.save_process_settings_v119(e);" in bootstrap
gameplay = (CPP / "original_ui_gameplay_bridge_v1.inc").read_text(encoding="utf-8")
assert "return context.self.gameplay_settings_actions_v1.save(error);" in gameplay

settings_test = (ROOT / "port/engine-ui/tests/owned_settings_v1.cpp").read_text(encoding="utf-8")
assert "saveSettings gate must reject before Savegame+4 exists" in settings_test
assert "real missing file must create Savegame+4 and admit saveSettings" in settings_test
assert "fresh raw save must set byte37 and reject saveSettings" in settings_test
print("PASS | NativeSaveSettings gate false/true state, canonical FileManager route, shared front/gameplay callback, and unconditional language tail")
