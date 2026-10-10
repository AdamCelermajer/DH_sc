"""Reproduce class Show lookup failure using actual packaged SWF names."""
from pathlib import Path
import importlib.util
import html
import os
import re
import shutil
import struct
import subprocess
import tempfile
import unittest
import zlib

ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"
ASSETS = ROOT / "port/android-native/app/src/main/assets"


def edit_text_initial(payload):
    """Decode the DefineEditText authoring value, after its optional fields."""
    count = payload[2] >> 3
    cursor = 2 + (5 + 4 * count + 7) // 8
    flags, flags2 = payload[cursor:cursor + 2]
    cursor += 2

    def text():
        nonlocal cursor
        end = payload.index(0, cursor)
        value = payload[cursor:end].decode("utf-8")
        cursor = end + 1
        return value

    if flags & 1:
        cursor += 2  # FontID
    if flags2 & 128:
        text()  # FontClass
    if flags & 1:
        cursor += 2  # FontHeight
    if flags & 4:
        cursor += 4  # TextColor RGBA
    if flags & 2:
        cursor += 2  # MaxLength
    if flags2 & 32:
        cursor += 9  # align/margins/indent/leading
    text()  # VariableName
    return text() if flags & 128 else ""


def class_paths(path):
    raw = path.read_bytes()
    data = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
    spec = importlib.util.spec_from_file_location("class_screen_decoder", ROOT / "port/engine-ui/tools/swf_action_decoder_v1.py")
    decoder = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(decoder)
    bounds = decoder.Bits(data, 8)
    bounds.skip(bounds.u(5) * 4)
    children, edit_texts = {}, {}

    def tags(start, end, sprite=0):
        cursor, frame = start, 0
        while cursor + 2 <= end:
            header = struct.unpack_from("<H", data, cursor)[0]
            cursor += 2
            kind, size = header >> 6, header & 63
            if size == 63:
                size = struct.unpack_from("<I", data, cursor)[0]
                cursor += 4
            stop = cursor + size
            if stop > end:
                raise AssertionError("Truncated actual SWF tag")
            if kind == 39:
                identifier = struct.unpack_from("<H", data, cursor)[0]
                tags(cursor + 4, stop, identifier)
            elif kind == 37:
                identifier = struct.unpack_from("<H", data, cursor)[0]
                edit_texts[identifier] = data[cursor:stop]
            elif kind in (26, 70) and frame == 0:
                flags = data[cursor]
                flags2 = data[cursor + 1] if kind == 70 else 0
                p = cursor + (4 if kind == 70 else 3)
                if flags2 & 8 or ((flags2 & 16) and (flags & 2)):
                    _, p = decoder.string(data, p)
                identifier = None
                if flags & 2:
                    identifier = struct.unpack_from("<H", data, p)[0]
                    p += 2
                if flags & 4:
                    p = decoder.matrix_end(data, p)
                if flags & 8:
                    color = decoder.Bits(data, p)
                    add, multiply, count = color.u(1), color.u(1), color.u(4)
                    color.skip(count * 4 * (add + multiply))
                    p = color.end()
                if flags & 16:
                    p += 2
                if flags & 32 and identifier is not None:
                    name, _ = decoder.string(data, p)
                    children.setdefault(sprite, {})[name] = identifier
            elif kind == 1:
                frame += 1
            elif kind == 0:
                break
            cursor = stop

    tags(bounds.end() + 4, len(data))
    paths, text_paths = set(), {}

    def expand(sprite, prefix, ancestors):
        if sprite in ancestors:
            return
        for name, identifier in children.get(sprite, {}).items():
            current = prefix + "." + name if prefix else name
            paths.add(current)
            if identifier in edit_texts:
                text_paths[current] = edit_texts[identifier]
            expand(identifier, current, ancestors | {sprite})

    class_id = children[0]["menu_SelectClass"]
    paths.add("menu_SelectClass")
    expand(class_id, "menu_SelectClass", set())
    return paths, text_paths


class ClassScreenBindings(unittest.TestCase):
    def test_actual_packaged_fields_complete_current_class_show(self):
        if shutil.which("g++"):
            prefix, convert = [], str
        elif os.name == "nt" and shutil.which("wsl"):
            prefix = ["wsl", "--exec"]

            def convert(path):
                return subprocess.check_output(prefix + ["wslpath", "-a", path.as_posix()], text=True).strip()
        else:
            self.fail("A C++17 host compiler is required")
        front = (CPP / "front_ui_session_v87.cpp").read_text()
        singleton = (CPP / "native_menu_singletons_v67.inc").read_text()
        self.assertIn('"menu_SelectClass.class_description.text"', front)
        self.assertNotIn("menu_SelectClass.s_description.text", front)
        self.assertIn('"class_description.text"', singleton)
        self.assertNotIn('"s_description.text"', singleton)
        with tempfile.TemporaryDirectory(prefix="dh2-class-screen-bindings-") as temporary:
            directory = Path(temporary)
            start = front.index("    static bool update_class(")
            end = front.index("    static bool change_menu(", start)
            (directory / "class_screen_update_live_v1.inc").write_text(front[start:end])
            start = front.index("bool FrontUiSessionV87::process_class_select_show_v87(")
            end = front.index("bool FrontUiSessionV87::process_class_select_update_v87(", start)
            (directory / "class_screen_show_live_v1.inc").write_text(front[start:end])
            executable = directory / "class-screen-bindings"
            command = prefix + ["g++", "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-I" + convert(directory),
                convert(ROOT / "port/android-native/tests/class_screen_bindings_live_v1.cpp"), "-o", convert(executable)]
            build = subprocess.run(command, text=True, capture_output=True, timeout=90)
            self.assertEqual(build.returncode, 0, build.stdout + build.stderr)
            movies = [ASSETS / "front-compat/dqmenus_droid.swf"]
            movies += sorted((ASSETS / "original-cache/data/menus").glob("dqmenus*.swf"))
            checked = 0
            for movie in movies:
                if movie.name.startswith("dqmenus2"):
                    continue
                with self.subTest(movie=movie.name, directory=movie.parent.name):
                    paths, text_paths = class_paths(movie)
                    self.assertIn("menu_SelectClass.class_title.text", text_paths)
                    self.assertIn("menu_SelectClass.class_description.text", text_paths)
                    self.assertNotIn("menu_SelectClass.s_description.text", text_paths)
                    self.assertNotIn("menu_SelectClass.class_title", text_paths)
                    initial = edit_text_initial(text_paths["menu_SelectClass.class_title.text"])
                    self.assertEqual(html.unescape(re.sub(r"<[^>]*>", "", initial)), "TITLE_14")
                    payload = sorted(paths) + ["EDITTEXT:" + path for path in sorted(text_paths)]
                    result = subprocess.run(prefix + [convert(executable)], input="\n".join(payload) + "\n", text=True, capture_output=True, timeout=30)
                    self.assertEqual(result.returncode, 0, str(movie) + "\n" + result.stdout + result.stderr)
                    checked += 1
            print("PASS current class Show/Update against " + str(checked) + " actual original/packaged movie variants")


if __name__ == "__main__":
    unittest.main()
