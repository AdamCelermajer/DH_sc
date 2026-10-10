"""Prove startup splash and level loading use distinct ORIGINAL SWF artwork.

Reads movie graphs and bitmap fills; no Android build, GPU, or emulator.
"""
from pathlib import Path
import hashlib
import json
import os
import struct
import unittest
import zipfile
import zlib

ROOT = Path(__file__).resolve().parents[3]
ASSETS = ROOT / "port/android-native/app/src/main/assets/original-cache"
ARCHIVE = Path(os.environ.get("DH2_ORIGINAL_CACHE", "C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip"))
PREFIX = "com.gameloft.android.GAND.GloftD2SS/files/"


class Bits:
    def __init__(self, data, offset):
        self.data, self.bit = data, offset * 8

    def read(self, count):
        value = 0
        for _ in range(count):
            value = value * 2 + ((self.data[self.bit // 8] >> (7 - self.bit % 8)) & 1)
            self.bit += 1
        return value

    def offset(self):
        return (self.bit + 7) // 8


def rectangle_end(data, pos):
    bits = Bits(data, pos)
    bits.read(bits.read(5) * 4)
    return bits.offset()


def matrix_end(data, pos):
    bits = Bits(data, pos)
    for _ in range(2):
        if bits.read(1):
            bits.read(bits.read(5) * 2)
    bits.read(bits.read(5) * 2)
    return bits.offset()


def color_end(data, pos):
    bits = Bits(data, pos)
    add, multiply, count = bits.read(1), bits.read(1), bits.read(4)
    bits.read(count * 4 * (add + multiply))
    return bits.offset()


def tags(data, pos):
    while pos + 2 <= len(data):
        header = struct.unpack_from("<H", data, pos)[0]
        pos += 2
        kind, size = header >> 6, header & 63
        if size == 63:
            size = struct.unpack_from("<I", data, pos)[0]
            pos += 4
        end = pos + size
        if end > len(data):
            raise AssertionError("SWF tag exceeds its parent")
        yield kind, data[pos:end]
        pos = end
        if kind == 0:
            break


def placement(data):
    flags, depth = struct.unpack_from("<BH", data)
    pos = 3
    identity = None
    if flags & 2:
        identity = struct.unpack_from("<H", data, pos)[0]
        pos += 2
    if flags & 4:
        pos = matrix_end(data, pos)
    if flags & 8:
        pos = color_end(data, pos)
    if flags & 16:
        pos += 2
    name = data[pos:data.index(0, pos)].decode() if flags & 32 else None
    return identity, depth, name


def shape_bitmaps(kind, data):
    pos = rectangle_end(data, 2)
    if kind == 83:
        pos = rectangle_end(data, pos) + 1
    count = data[pos]
    pos += 1
    if count == 255:
        count = struct.unpack_from("<H", data, pos)[0]
        pos += 2
    colors = 4 if kind in (32, 83) else 3
    refs = []
    for _ in range(count):
        style = data[pos]
        pos += 1
        if style == 0:
            pos += colors
        elif style in (16, 18, 19):
            pos = matrix_end(data, pos)
            stops = data[pos] & 15
            pos += 1 + stops * (1 + colors) + (2 if style == 19 else 0)
        elif style in (64, 65, 66, 67):
            refs.append(struct.unpack_from("<H", data, pos)[0])
            pos = matrix_end(data, pos + 2)
        else:
            raise AssertionError(f"Unsupported fill style {style}")
    return refs


def movie_artwork(raw, clip):
    data = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
    definitions, exports, clips = {}, {}, {}
    for kind, payload in tags(data, rectangle_end(data, 8) + 4):
        if kind in (2, 22, 32, 39, 83):
            definitions[struct.unpack_from("<H", payload)[0]] = kind, payload
        elif kind == 26:
            identity, _, name = placement(payload)
            if name:
                clips[name] = identity
        elif kind == 56:
            count = struct.unpack_from("<H", payload)[0]
            pos = 2
            for _ in range(count):
                identity = struct.unpack_from("<H", payload, pos)[0]
                pos += 2
                end = payload.index(0, pos)
                exports[identity] = payload[pos:end].decode()
                pos = end + 1
    visited, bitmap_shapes = set(), {}

    def visit(identity):
        if identity in visited or identity not in definitions:
            return
        visited.add(identity)
        kind, payload = definitions[identity]
        if kind == 39:
            for child_kind, child in tags(payload, 4):
                if child_kind == 26:
                    reference, _, _ = placement(child)
                    if reference is not None:
                        visit(reference)
                elif child_kind == 4:
                    visit(struct.unpack_from("<H", child)[0])
        elif kind in (2, 22, 32, 83):
            for bitmap in shape_bitmaps(kind, payload):
                bitmap_shapes.setdefault(bitmap, set()).add(identity)
    visit(clips[clip])
    return {"clip": "_root." + clip, "clip_character": clips[clip],
            "movie_sha256": hashlib.sha256(raw).hexdigest(),
            "bitmap_exports": [{"bitmap_character": bitmap, "export": exports.get(bitmap), "shape_characters": sorted(shapes)}
                               for bitmap, shapes in sorted(bitmap_shapes.items())]}


def sprite_timeline(raw, identity):
    data = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
    definitions = {}
    for kind, payload in tags(data, rectangle_end(data, 8) + 4):
        if kind == 39:
            definitions[struct.unpack_from("<H", payload)[0]] = payload
    payload = definitions[identity]
    frame_count = struct.unpack_from("<H", payload, 2)[0]
    children, frames = {}, []
    for kind, child in tags(payload, 4):
        if kind == 26:
            character, depth, name = placement(child)
            if character is not None:
                children[depth] = (character, name)
        elif kind == 28:
            children.pop(struct.unpack_from("<H", child)[0], None)
        elif kind == 1:
            frames.append(dict(children))
    return frame_count, frames


def sprite_composition_frames(raw, identity):
    """Return each frame's retained children and authored transforms.

    `sprite_timeline` intentionally projects only child IDs/names. That is
    enough to follow the menu graph, but it misses PlaceObject2 records that
    update a child's matrix without changing its character ID. Keep the full
    matrix bytes here so a background that pans/scales between frames cannot
    pass the static-backdrop assertion unnoticed.
    """
    data = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
    definitions = {}
    for kind, payload in tags(data, rectangle_end(data, 8) + 4):
        if kind == 39:
            definitions[struct.unpack_from("<H", payload)[0]] = payload
    payload = definitions[identity]
    children, frames = {}, []
    for kind, child in tags(payload, 4):
        if kind == 26:
            flags, depth = struct.unpack_from("<BH", child)
            pos = 3
            prior = children.get(depth, {})
            current = dict(prior)
            if flags & 2:
                current["character"] = struct.unpack_from("<H", child, pos)[0]
                pos += 2
            if flags & 4:
                end = matrix_end(child, pos)
                current["matrix"] = child[pos:end]
                pos = end
            if flags & 8:
                end = color_end(child, pos)
                current["color"] = child[pos:end]
                pos = end
            if flags & 16:
                current["ratio"] = struct.unpack_from("<H", child, pos)[0]
                pos += 2
            if flags & 32:
                end = child.index(0, pos)
                current["name"] = child[pos:end].decode()
            children[depth] = current
        elif kind == 28:
            children.pop(struct.unpack_from("<H", child)[0], None)
        elif kind == 1:
            frames.append(tuple((depth, tuple(sorted(values.items())))
                                for depth, values in sorted(children.items())))
    return frames


def sprite_display_placements(raw, identity):
    data = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
    for kind, payload in tags(data, rectangle_end(data, 8) + 4):
        if kind == 39 and struct.unpack_from("<H", payload)[0] == identity:
            return [child for child_kind, child in tags(payload, 4) if child_kind in (4, 26, 70)]
    raise AssertionError(f"Missing sprite definition {identity}")


def bitmap_refs(raw, identity):
    data = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
    definitions = {}
    for kind, payload in tags(data, rectangle_end(data, 8) + 4):
        if kind in (2, 22, 32, 39, 83, 37):
            definitions[struct.unpack_from("<H", payload)[0]] = kind, payload
    visited, bitmaps = set(), set()

    def visit(character):
        if character in visited or character not in definitions:
            return
        visited.add(character)
        kind, payload = definitions[character]
        if kind in (2, 22, 32, 83):
            bitmaps.update(shape_bitmaps(kind, payload))
        elif kind == 39:
            for child_kind, child in tags(payload, 4):
                if child_kind == 26:
                    nested, _, _ = placement(child)
                    if nested is not None:
                        visit(nested)
    visit(identity)
    return bitmaps


def cpp_function(source, signature):
    start = source.index(signature)
    opening = source.index("{", start)
    depth = 0
    for pos in range(opening, len(source)):
        if source[pos] == "{":
            depth += 1
        elif source[pos] == "}":
            depth -= 1
            if depth == 0:
                return source[start:pos + 1]
    raise AssertionError(f"Unclosed C++ function: {signature}")


class AuthoredLoadingScreenAssetsTest(unittest.TestCase):
    def test_original_movies_separate_startup_and_level_loading_art(self):
        evidence = []
        with zipfile.ZipFile(ARCHIVE) as archive:
            for suffix in ("", "_droid", "_i9000"):
                screens = []
                for stem, clip in (("dqmenus", "menu_splash"), ("dqshared", "menu_Loading")):
                    uri = "data/menus/" + stem + suffix + ".swf"
                    original = archive.read(PREFIX + uri)
                    self.assertEqual((ASSETS / uri).read_bytes(), original)
                    result = movie_artwork(original, clip)
                    result["uri"] = uri
                    screens.append(result)
                splash, level = screens
                self.assertEqual({item["export"] for item in splash["bitmap_exports"]},
                                 {"splash_final.tga"} if not suffix else
                                 {"menus/splash_final_droid.tga"} if suffix == "_droid" else
                                 {"menus/splash_final_I9000.tga"})
                self.assertTrue(level["bitmap_exports"])
                self.assertFalse(any("splash_final" in (item["export"] or "") for item in level["bitmap_exports"]))
                self.assertEqual({item["export"] for item in level["bitmap_exports"]},
                                 {f"menus/MenuGraphics{i:02}.tga" for i in range(1, 6)} if not suffix else
                                 {"menus/MenusGraphics_droid.tga"} if suffix == "_droid" else
                                 {"menus/MenusGraphics_I9000.tga"})
                evidence += screens
        report = os.environ.get("DH2_LOADING_SCREEN_ASSET_REPORT")
        if report:
            Path(report).write_text(json.dumps(evidence, indent=2) + "\n")
        for screen in evidence:
            print(screen["uri"], screen["clip"], screen["clip_character"],
                  [(item["bitmap_character"], item["export"], item["shape_characters"]) for item in screen["bitmap_exports"]])
        print("PASS original startup splash and level-loading movies use distinct authored bitmap resources")

    def test_campaign_touch_state_keeps_the_authored_loading_backdrop(self):
        front_source = (ROOT / "port/android-native/app/src/main/cpp/front_ui_session_v87.cpp").read_text()
        show_loading = cpp_function(front_source, "bool FrontUiSessionV87::show_game_loading(")
        refresh_loading = cpp_function(front_source, "bool FrontUiSessionV87::refresh_game_loading(")
        render_loading = cpp_function(front_source, "bool FrontUiSessionV87::render_game_loading(")
        source_renderer = cpp_function(front_source, "bool FrontUiSessionV87::render_source_movies_v93(")
        self.assertIn('Impl::push_menu(impl_.get(),"menu_Loading",error)', show_loading)
        self.assertIn("refresh_game_loading(error)", show_loading)
        self.assertIn('graph.invoke(menu,menu,"onProgress"', refresh_loading)
        self.assertIn("impl_->source_loading_render_v114=true", render_loading)
        self.assertIn("return render(width,height,error)", render_loading)
        self.assertIn("front_loading_clip_visible_v1(phase,q.slot,q.actual_clip_path)", source_renderer)
        self.assertIn("display_source_stage_clip_v5(q.actual_clip_path.c_str(),e)", source_renderer)
        self.assertIn('q.actual_clip_path=="_root.menu_splash"', source_renderer)
        with zipfile.ZipFile(ARCHIVE) as archive:
            for suffix, uri, expected in (
                ("", "data/menus/dqshared.swf", "menus/MenuGraphics"),
                ("_droid", "data/menus/dqshared_droid.swf", "menus/MenusGraphics_droid"),
                ("_i9000", "data/menus/dqshared_i9000.swf", "menus/MenusGraphics_I9000"),
            ):
                raw = archive.read(PREFIX + uri)
                artwork = movie_artwork(raw, "menu_Loading")
                self.assertEqual({item["export"] for item in artwork["bitmap_exports"]},
                                 {f"{expected}{i:02}.tga" for i in range(1, 6)} if not suffix else {expected + ".tga"})
                root_frames, states = sprite_timeline(raw, artwork["clip_character"])
                self.assertEqual(root_frames, 1)
                self.assertEqual(len(states), 1)
                named = {name: character for character, name in states[0].values() if name}
                self.assertIn("loading_anim", named)
                self.assertIn("continue_text", named)

                background_identity = states[0][min(states[0])][0]
                background_frames, background_states = sprite_timeline(raw, background_identity)
                self.assertGreater(background_frames, 1)
                self.assertEqual(len(background_states), background_frames)
                self.assertTrue(all(state == background_states[0] for state in background_states))
                self.assertEqual(len(sprite_display_placements(raw, background_identity)), 1)
                background_composition = sprite_composition_frames(raw, background_identity)
                self.assertEqual(len(background_composition), background_frames)
                self.assertTrue(all(frame == background_composition[0]
                                    for frame in background_composition))
                self.assertTrue(bitmap_refs(raw, background_identity))

                # The authored root backdrop remains in its single root frame;
                # Continue adds localized text while the existing progress sprite
                # advances to its final frame. It does not select another picture.
                progress_frames, progress_states = sprite_timeline(raw, named["loading_anim"])
                prompt_frames, _ = sprite_timeline(raw, named["continue_text"])
                self.assertEqual(progress_frames, 101)
                self.assertEqual(len(progress_states), progress_frames)
                self.assertEqual(prompt_frames, 72)
                self.assertFalse(bitmap_refs(raw, named["continue_text"]))
                self.assertTrue(bitmap_refs(raw, named["loading_anim"]))
        print("PASS campaign loading and Touch-to-Continue use the same retained menu_Loading display list and authored backdrop; only progress art/text state changes")


if __name__ == "__main__":
    unittest.main()
