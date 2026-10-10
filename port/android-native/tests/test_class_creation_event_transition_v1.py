"""Run current production creation delivery bodies with declared movie/GL leaves.

Extracted includes contain the exact current App input, native navigation,
selected Show, inherited focus/blur, preview slot-transfer and Front Show code.
The unused Main.Show implementation is omitted. Real EventManager and MenuStack
owners, navigation conversion order and qualified MenuBase.Show are compiled;
this host fixture does not execute authored AS bytecode or
verify an installed APK.
"""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"


def section(text, start, end):
    a = text.index(start)
    return text[a:text.index(end, a + len(start))]


class ClassCreationEventTransition(unittest.TestCase):
    def test_current_delivery_and_failed_prefix_cannot_replay(self):
        if shutil.which("g++"):
            prefix, convert = [], str
        elif os.name == "nt" and shutil.which("wsl"):
            prefix = ["wsl", "--exec"]

            def convert(path):
                return subprocess.check_output(prefix + ["wslpath", "-a", path.as_posix()], text=True).strip()
        else:
            self.fail("A C++17 host compiler is required")
        startup = (CPP / "native_process_startup_v119.inc").read_text()
        front = (CPP / "front_ui_session_v87.cpp").read_text()
        preview = (CPP / "native_menu_preview_v121.cpp").read_text()
        bootstrap = (CPP / "native_menu_process_bootstrap_v104.inc").read_text()
        extracted = {
            "class_creation_selected_show_under_test.inc": section(startup,
                "bool model_renderer::native_process_derived_menu_show_v119(",
                " if(owner->kind!=MenuSingletonKindV67::main)") +
                ' e="Unused Main.Show is outside this host fixture";return false;\n}\n',
            "class_creation_focus_blur_under_test.inc": section(startup,
                "bool model_renderer::native_process_derived_menu_focus_v119(",
                "bool model_renderer::native_process_derived_menu_callback_v119("),
            "class_creation_front_show_under_test.inc": section(front,
                "bool FrontUiSessionV87::process_class_select_show_v87(",
                "bool FrontUiSessionV87::process_class_select_update_v87("),
            "class_creation_update_class_under_test.inc": section(front,
                "    static bool update_class(", "    static bool change_menu("),
            "class_creation_native_action_under_test.inc":
                "bool FrontUiSessionV87::Impl::native_action(void* context,const char* name,const gameswf::fn_call& fn,std::string& error) {\n" +
                section(front, "        auto& self=*static_cast<Impl*>(context);\n        // Process draw/input", '        if(!std::strcmp(name,"NativeHasPushNotification"))') +
                ' error="Unused native action outside this host fixture";return false;\n}\n',
            "class_creation_preview_under_test.inc": section(preview,
                " template<class F> bool run(", "public:") + section(preview,
                " bool select_show(", " bool select_hide("),
            "class_creation_show_dispatch_under_test.inc": section(bootstrap,
                "  if(request.operation==dh2::ui::MenuStackOperationV1::menu_show",
                "  if(request.operation==dh2::ui::MenuStackOperationV1::menu_focus"),
            "class_creation_application_event_under_test.inc": "\n".join(
                line for line in (CPP / "native_menu_application_event_v120.inc").read_text().splitlines()
                if not line.startswith("#include")),
        }
        sources = [
            ROOT / "port/android-native/tests/class_creation_event_transition_v1.cpp",
            ROOT / "port/engine-ui/menu_stack_v1.cpp",
            ROOT / "port/engine-ui/menu_stack_owner_v1.cpp",
            ROOT / "port/engine-ui/menu_stack_actions_v1.cpp",
            ROOT / "port/engine-ui/authored_menu_lifecycle_v1.cpp",
            ROOT / "port/level-world/event_manager_owner_v12.cpp",
        ]
        with tempfile.TemporaryDirectory(prefix="dh2-class-event-") as temporary:
            directory = Path(temporary)
            for name, body in extracted.items():
                (directory / name).write_text(body)
            executable = directory / "class-event"
            command = prefix + ["g++", "-std=c++17", "-O1", "-Wall", "-Wextra"]
            command += ["-I" + convert(path) for path in [ROOT / "port/engine-ui", ROOT / "port/level-world", directory]]
            command += [convert(path) for path in sources]
            command += ["-o", convert(executable)]
            build = subprocess.run(command, text=True, capture_output=True, timeout=90)
            self.assertEqual(build.returncode, 0, build.stdout + build.stderr)
            result = subprocess.run(prefix + [convert(executable)], text=True, capture_output=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn("PASS initial menu_splash Continue hit-test", result.stdout)
            print(result.stdout, end="")


if __name__ == "__main__":
    unittest.main()
