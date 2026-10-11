"""Export exact solid-vector geometry used by dqhud_droid sprite742."""
from pathlib import Path
import importlib.util
import inspect
import struct
import subprocess
import zlib

ROOT = Path(__file__).resolve().parents[4]
SWF = ROOT / ".local-inputs/ui-layout-discovery/dqhud_droid.swf"
spec = importlib.util.spec_from_file_location(
    "swf_geometry", ROOT / "port/windows-foundation/tools/export_hud_geometry.py")
swf = importlib.util.module_from_spec(spec)
spec.loader.exec_module(swf)

raw = SWF.read_bytes()
data = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
_, at = swf.rect(data, 8)
at += 4

def solid_styles(tag, offset, ident, code):
    count = tag[offset]
    offset += 1
    if count == 255:
        count = struct.unpack_from("<H", tag, offset)[0]
        offset += 2
    styles = []
    for _ in range(count):
        kind = tag[offset]
        offset += 1
        if kind != 0:
            raise ValueError(f"Shape {ident}: unsupported non-solid panel fill {kind}")
        color_size = 4 if code == 32 else 3
        rgba = list(tag[offset:offset + color_size])
        offset += color_size
        if color_size == 3:
            rgba.append(255)
        styles.append({"kind": 0, "rgba": rgba,
                       "matrix_twips": [1, 0, 0, 1, 0, 0]})
    lines = tag[offset]
    offset += 1
    if lines:
        raise ValueError(f"Shape {ident}: unexpected panel outline")
    return styles, offset

swf.read_bitmap_styles = solid_styles
parser_source = inspect.getsource(swf.parse_shape).replace(
    '    solid_triangles = []', '    solid_triangles = []; fill_groups = []').replace(
    '        contours.extend(fill_contours)', '        contours.extend(fill_contours); fill_groups.append((fill, fill_contours))').replace(
    '"solid_triangles":solid_triangles}', '"solid_triangles":solid_triangles,"fill_contours":fill_groups}').replace(
    '            if any(contour_inside(inner, outer) for inner in fill_contours for outer in fill_contours if inner is not outer):\n'
    '                raise ValueError(f"Shape {shape_id}: multiple contours need a hole-aware tessellator")\n'
    '            vertices = [v for contour in fill_contours for v in triangulate(contour)]',
    '            nested = any(contour_inside(inner, outer) for inner in fill_contours for outer in fill_contours if inner is not outer)\n'
    '            print("fill-contours", shape_id, len(fill_contours), "nested", nested)\n'
    '            vertices = [v for contour in fill_contours for v in triangulate(contour)]')
exec(parser_source, swf.parse_shape.__globals__)
swf.parse_shape = swf.parse_shape.__globals__["parse_shape"]
found = {}
for code, tag in swf.tags(data, at, len(data)):
    if code not in (2, 22, 32):
        continue
    ident = struct.unpack_from("<H", tag)[0]
    if ident in (496, 739):
        shape = swf.parse_shape(code, tag)
        found[ident] = shape
if set(found) != {496, 739}:
    raise SystemExit(f"Expected source shapes 496 and 739, got {sorted(found)}")
for ident, shape in found.items():
    print(ident, shape["bounds_twips"],
          [(fill["rgba"], fill["kind"])
           for fill in shape["fill_records"]],
          len(shape["solid_triangles"]))
    for fill_id, fill_contours in shape.get("fill_contours", []):
        areas = [sum(contour[i][0]*contour[(i+1)%len(contour)][1] -
                     contour[(i+1)%len(contour)][0]*contour[i][1]
                     for i in range(len(contour))) / 2 for contour in fill_contours]
        parents = []
        for i, contour in enumerate(fill_contours):
            candidates = [j for j, other in enumerate(fill_contours) if j != i and
                          swf.contour_inside(contour, other)]
            parents.append(min(candidates, key=lambda j: abs(areas[j])) if candidates else None)
        print("  fill", fill_id, "contours", len(fill_contours),
              "areas", [round(a, 1) for a in areas], "parents", parents)

# Preserve source paths in a temporary C++ include so the vendored GameSWF
# hole/island-aware tessellator can generate static runtime triangles.
HERE = Path(__file__).resolve().parent
path_header = HERE
parts = ["#pragma once", "#include <array>", "#include <cstddef>",
         "namespace death_screen_source_paths_v1 {"]
groups = found[739]["fill_contours"]
for group_index, (fill_id, contours) in enumerate(groups):
    parts.append(f"inline constexpr std::size_t kFill{fill_id}Count = {len(contours)};")
    for contour_index, contour in enumerate(contours):
        parts.append(f"inline constexpr std::array<std::array<float,2>,{len(contour)}> kFill{fill_id}Contour{contour_index}{{{{")
        def cpp_float(value):
            text = f"{value:.9g}"
            return text if any(c in text for c in ".eE") else text + ".0"
        parts.extend(f"    {{{cpp_float(point[0])}f, {cpp_float(point[1])}f}}," for point in contour)
        parts.append("}};")
    parts.append(f"inline constexpr std::array<const float*,kFill{fill_id}Count> kFill{fill_id}Data{{{{")
    parts.extend(f"    kFill{fill_id}Contour{i}[0].data()," for i in range(len(contours)))
    parts.append("}};")
    parts.append(f"inline constexpr std::array<std::size_t,kFill{fill_id}Count> kFill{fill_id}Sizes{{{{")
    parts.extend(f"    kFill{fill_id}Contour{i}.size()," for i in range(len(contours)))
    parts.append("}};")
parts.append("}")
(path_header / "death_screen_source_paths_v1.hpp").write_text("\n".join(parts) + "\n")

compiler = ROOT / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe"
binary = ROOT / ".local-inputs/death-screen-triangulator.exe"
command = [str(compiler), "-std=c++17", "-O0", "-DNDEBUG", "-static",
           "-I", str(ROOT / "port/engine-ui/vendor/gameswf1714"),
           str(HERE / "triangulate_death_screen_art_v1.cpp"),
           str(ROOT / "port/engine-ui/vendor/gameswf1714/base/ear_clip_triangulate_float.cpp"),
           str(ROOT / "port/engine-ui/vendor/gameswf1714/base/container.cpp"),
           str(ROOT / "port/engine-ui/vendor/gameswf1714/base/tu_random.cpp"),
           str(ROOT / "port/engine-ui/vendor/gameswf1714/base/tu_timer.cpp"),
           str(ROOT / "port/engine-ui/vendor/gameswf1714/base/utf8.cpp"),
           "-o", str(binary)]
subprocess.run(command, cwd=ROOT, check=True, capture_output=True, text=True)
result = subprocess.run([str(binary)], cwd=ROOT, check=True, capture_output=True, text=True)
records = result.stdout.splitlines()
generated = ["#pragma once", "#include <array>", "#include <cstddef>",
             "namespace dh::foundation::persistence::death_flow::art_data {",
             "struct Vertex { float x; float y; };"]
cursor = 0
areas = {}

def cpp_float(value):
    text = f"{value:.9g}"
    return text if any(c in text for c in ".eE") else text + ".0"

for _ in range(2):
    fields = records[cursor].split()
    cursor += 1
    if len(fields) != 7 or fields[0] != "FILL":
        raise SystemExit("Tessellator output header is malformed")
    fill_id, count = int(fields[1]), int(fields[2])
    rgba = [int(value) for value in fields[3:7]]
    points = [tuple(map(float, records[cursor + i].split())) for i in range(count)]
    cursor += count
    if count % 3:
        raise SystemExit(f"Fill {fill_id}: output is not complete triangles")
    area = sum(abs((points[i+1][0]-points[i][0])*(points[i+2][1]-points[i][1]) -
                   (points[i+1][1]-points[i][1])*(points[i+2][0]-points[i][0])) / 2
                 for i in range(0, count, 3))
    expected = sum(sum(contour[i][0]*contour[(i+1)%len(contour)][1] -
                       contour[(i+1)%len(contour)][0]*contour[i][1]
                       for i in range(len(contour))) / 2
                   for source_fill, contours in groups if source_fill == fill_id
                   for contour in contours)
    if abs(area - abs(expected)) > 1.0:
        raise SystemExit(f"Fill {fill_id}: triangle area {area} differs from source {abs(expected)}")
    areas[fill_id] = (area, abs(expected))
    generated.append(f"inline constexpr std::size_t kPanelFill{fill_id}Count = {count};")
    generated.append(f"inline constexpr std::array<Vertex,kPanelFill{fill_id}Count> kPanelFill{fill_id}{{{{")
    generated.extend(f"    {{{cpp_float(x)}f, {cpp_float(y)}f}}," for x, y in points)
    generated.append("}};")
    generated.append(f"inline constexpr std::array<float,4> kPanelFill{fill_id}Color{{{{"
                     + ",".join(f"{cpp_float(v / 255)}f" for v in rgba) + "}};")

fade = found[496]["solid_triangles"]
generated.append(f"inline constexpr std::size_t kFadeCount = {len(fade)};")
generated.append("inline constexpr std::array<Vertex,kFadeCount> kFade{{")
generated.extend(f"    {{{cpp_float(x)}f, {cpp_float(y)}f}}," for x, y, *_ in fade)
generated.append("}};")
generated.append("}")
(HERE / "death_screen_art_geometry_v1.hpp").write_text("\n".join(generated) + "\n")
(HERE / "death_screen_source_paths_v1.hpp").unlink()
print("PASS original panel tessellation and source-area checks:", areas)
