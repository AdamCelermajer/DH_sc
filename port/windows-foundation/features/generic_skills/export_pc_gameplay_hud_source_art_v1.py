"""Export exact dqhud_droid spell and potion icon contours for the PC HUD."""
from __future__ import annotations

import json
import runpy
import struct
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
SOURCE = ROOT / "port/android-native/app/src/main/assets/original-cache/data/menus/dqhud_droid.swf"
OUTPUT = Path(__file__).with_name("pc_gameplay_hud_source_art_v1.cpp")
DECODER = ROOT / "port/windows-foundation/tools/export_hud_geometry.py"


def cpp_num(x: float) -> str:
    value = f"{x:.10g}"
    if "." not in value and "e" not in value:
        value += ".0"
    return value + "f"


def main() -> None:
    swf = runpy.run_path(str(DECODER))
    raw = SOURCE.read_bytes()
    if raw[:3] == b"CWS":
        data = raw[:8] + zlib.decompress(raw[8:])
    elif raw[:3] == b"FWS":
        data = raw
    else:
        raise ValueError("Unsupported dqhud SWF header")
    if len(data) != struct.unpack_from("<I", data, 4)[0]:
        raise ValueError("dqhud SWF declared length mismatch")
    _, at = swf["rect"](data, 8)
    at += 4
    sprites: dict[int, list[dict]] = {}
    shape_tags: dict[int, tuple[int, bytes]] = {}
    for code, tag in swf["tags"](data, at, len(data)):
        if code == 39:
            ident, frames = struct.unpack_from("<HH", tag)
            sprites[ident] = swf["parse_timeline"](tag, 4)
            if len(sprites[ident]) != frames:
                raise ValueError(f"dqhud sprite {ident} frame count mismatch")
        elif code in (2, 22, 32, 83):
            shape_tags[struct.unpack_from("<H", tag)[0]] = (code, tag)

    # Reach only the authored spell btimg frames and the potion btframe.
    reachable: set[int] = set()

    def walk(char: int, frame: int, depth: int = 0) -> None:
        if depth > 32:
            raise ValueError("dqhud icon display list exceeds nesting limit")
        if char in shape_tags:
            reachable.add(char)
            return
        if char not in sprites or not 0 <= frame < len(sprites[char]):
            raise ValueError(f"dqhud icon references missing character/frame {char}/{frame}")
        for _, placement in sorted(sprites[char][frame].items()):
            color = placement.get("color", [[1.] * 4, [0.] * 4])
            if color[0][3] == 0 and color[1][3] == 0:
                continue
            child = placement.get("character")
            if child is not None:
                walk(child, 0, depth + 1)

    for frame in range(13):
        walk(397, frame)
    walk(106, 0)
    swf["ROLES"].update({shape: f"pc_hud_icon_{shape}" for shape in reachable})
    shapes = {
        shape: swf["parse_shape"](*shape_tags[shape])
        for shape in sorted(reachable)
    }

    def flattened(sprite_id: int, frame: int) -> list[tuple[int, list[float]]]:
        swf["FRAME_OVERRIDES"][sprite_id] = frame
        return swf["collect_layers"](
            sprites, shapes, sprite_id, [1., 0., 0., 1., 0., 0.], 0, 0, 0
        )

    def transformed(shape_id: int, matrix: list[float]) -> list[tuple[float, float, float, float]]:
        shape = shapes[shape_id]
        a, b, c, d, tx, ty = matrix
        return [((a*x+c*y+tx)/20., (b*x+d*y+ty)/20., u, v)
                for x, y, u, v in shape["triangles"]]

    frame_rows = []
    for frame in range(13):
        layers = flattened(397, frame)
        triangles = [vertex for shape, matrix in layers for vertex in transformed(shape, matrix)]
        frame_rows.append((frame, layers, triangles))
    potion_layers = flattened(106, 0)
    potion_triangles = [vertex for shape, matrix in potion_layers for vertex in transformed(shape, matrix)]
    if any(shape != 105 for shape, _ in potion_layers):
        raise ValueError("Original potion btframe no longer resolves only to shape 105")

    lines = [
        "// Generated from exact dqhud_droid.swf shape/display-list frames; do not hand-edit.",
        '#include "pc_gameplay_hud_source_art_v1.hpp"',
        "namespace dh::foundation::generic_skills {",
        "namespace {",
        "const std::array<PcGameplayHudSourceFaeryIconV1, 13> faery_icons{{",
    ]
    # Source onPush passes NativeHUDGetActiveFaery()+1 to img_Faery.gotoAndStop.
    for source_frame, layers, vertices in frame_rows:
        shapes_in_frame = ",".join(str(shape) for shape, _ in layers)
        verts = ",".join("{" + ",".join(cpp_num(x) for x in v) + "}" for v in vertices)
        lines.append("{" + str(source_frame) + ",{" + shapes_in_frame + "},{" + verts + "}},")
    potion_verts = ",".join("{" + ",".join(cpp_num(x) for x in v) + "}" for v in potion_triangles)
    lines += [
        "}};",
        "const std::vector<HudGeometryVertex> potion_icon{{" + potion_verts + "}};",
        "}",
        "const std::array<PcGameplayHudSourceFaeryIconV1, 13>& original_pc_gameplay_hud_faery_icons_v1() noexcept { return faery_icons; }",
        "const std::vector<HudGeometryVertex>& original_pc_gameplay_hud_potion_icon_v1() noexcept { return potion_icon; }",
        "} // namespace dh::foundation::generic_skills",
    ]
    OUTPUT.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(json.dumps({
        "source": str(SOURCE),
        "source_sha256": __import__("hashlib").sha256(raw).hexdigest(),
        "faery_frames": len(frame_rows),
        "faery_shapes": {str(frame): [shape for shape, _ in layers]
                         for frame, layers, _ in frame_rows},
        "potion_shape_ids": [shape for shape, _ in potion_layers],
        "potion_triangle_vertices": len(potion_triangles),
        "output": str(OUTPUT),
    }, indent=2))


if __name__ == "__main__":
    main()
