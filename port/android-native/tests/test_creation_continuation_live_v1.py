"""Run current production creation render/Hide bodies with explicit host leaves.

No production copies are stored: methods are extracted from current source on
each run. Authored bytecode execution, save creation, GPU pixels and device
acceptance require a separate integrated run.
"""
from pathlib import Path
import hashlib
import importlib.util
import os
import shutil
import struct
import subprocess
import tempfile
import unittest
import zlib

ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"


def section(text, start, end):
    first = text.index(start)
    return text[first:text.index(end, first + len(start))]


def movie_actions(path):
    """Read the actual nested DoAction tags by sprite and frame."""
    raw = path.read_bytes()
    data = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
    spec = importlib.util.spec_from_file_location("creation_action_decoder",
        ROOT / "port/engine-ui/tools/swf_action_decoder_v1.py")
    decoder = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(decoder)
    bounds = decoder.Bits(data, 8)
    bounds.skip(bounds.u(5) * 4)
    output = {}

    def tags(start, end, sprite="root"):
        cursor, frame, action_index = start, 0, 0
        while cursor + 2 <= end:
            header = struct.unpack_from("<H", data, cursor)[0]
            cursor += 2
            kind, size = header >> 6, header & 63
            if size == 63:
                size = struct.unpack_from("<I", data, cursor)[0]
                cursor += 4
            stop = cursor + size
            if stop > end:
                raise AssertionError("Truncated source SWF tag")
            if kind == 39:
                identifier = struct.unpack_from("<H", data, cursor)[0]
                tags(cursor + 4, stop, sprite + "/sprite" + str(identifier))
            elif kind == 12:
                key = (sprite, frame, action_index)
                action_index += 1
                output[key] = (data[cursor:stop], decoder.decode(data, cursor, stop, label=sprite))
            elif kind == 1:
                frame += 1
                action_index = 0
            elif kind == 0:
                break
            cursor = stop

    tags(bounds.end() + 4, len(data))
    return raw, output


class CreationContinuationLive(unittest.TestCase):
    def test_authored_class_confirm_continuation_preserved(self):
        assets = ROOT / "port/android-native/app/src/main/assets"
        original, actions = movie_actions(assets / "original-cache/data/menus/dqmenus_droid.swf")
        self.assertEqual(hashlib.sha256(original).hexdigest(),
            "c114c7d39b7fe0351a93a515c78f9457dda715d78aed6e3bcc3f4895120de421")
        _, compatibility = movie_actions(assets / "front-compat/dqmenus_droid.swf")
        self.assertEqual(set(actions), set(compatibility))
        for key in actions:
            self.assertEqual(actions[key][0], compatibility[key][0], key)
        confirm = [body[1] for key, body in compatibility.items()
            if key[:2] == ("root/sprite428", 29)]
        self.assertEqual(len(confirm), 1)
        rows = confirm[0]
        calls = [value["text"] for row in rows for value in row.get("values", [])
            if isinstance(value, dict) and value.get("text", "").startswith("Native")]
        self.assertEqual(calls, ["NativeCreateSaveSlot", "NativeAssignSaveSlotToPlayer",
            "NativePopAllAbove", "NativeStartFromGCInvite", "NativePushMenu"])
        values = [value.get("text") for row in rows for value in row.get("values", [])
            if isinstance(value, dict)]
        self.assertIn("menu_MainMenu", values)
        self.assertIn("current_slot", values)
        self.assertIn("menu_StartGame", values)
        print("PASS original/packaged compatibility DoAction parity; class-confirm frame29 create/assign/current_slot/pop-to-Main/StartGame continuation; "
              + str(len(actions)) + " action tags")

    def test_current_production_render_and_hide_continuation(self):
        if shutil.which("g++"):
            prefix, convert = [], str
        elif os.name == "nt" and shutil.which("wsl"):
            prefix = ["wsl", "--exec"]

            def convert(path):
                return subprocess.check_output(prefix + ["wslpath", "-a", path.as_posix()], text=True).strip()
        else:
            self.fail("A C++17 host compiler is required")
        front = (CPP / "front_ui_session_v87.cpp").read_text()
        preview = (CPP / "native_menu_preview_v121.cpp").read_text()
        extracted = {
            "creation_front_render_live_v1.inc": section(front,
                "bool FrontUiSessionV87::render_source_movies_v93(",
                "bool FrontUiSessionV87::character_menu_sound_v4("),
            "creation_front_hide_live_v1.inc": section(front,
                "bool FrontUiSessionV87::process_class_select_hide_v87(",
                "bool FrontUiSessionV87::bind_main_menu_state_services_v114("),
            "creation_preview_run_live_v1.inc": section(preview,
                " template<class F> bool run(", "public:"),
            "creation_preview_show_live_v1.inc": section(preview,
                " bool select_show(", " bool select_hide("),
            "creation_preview_hide_live_v1.inc": section(preview,
                " bool select_hide(", " bool prepare_main("),
        }
        with tempfile.TemporaryDirectory(prefix="dh2-creation-continuation-") as temporary:
            directory = Path(temporary)
            for name, body in extracted.items():
                (directory / name).write_text(body)
            executable = directory / "creation-continuation"
            command = prefix + ["g++", "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror"]
            command += ["-I" + convert(path) for path in [CPP, ROOT / "port/engine-ui", directory]]
            command += [convert(ROOT / "port/android-native/tests/creation_continuation_live_v1.cpp"), "-o", convert(executable)]
            build = subprocess.run(command, text=True, capture_output=True, timeout=90)
            self.assertEqual(build.returncode, 0, build.stdout + build.stderr)
            result = subprocess.run(prefix + [convert(executable)], text=True, capture_output=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn("PASS current production creation render/Hide continuation", result.stdout)
            print(result.stdout, end="")


if __name__ == "__main__":
    unittest.main()
