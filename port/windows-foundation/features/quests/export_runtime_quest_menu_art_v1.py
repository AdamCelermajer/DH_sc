"""Export the actual retained sprite599 page, duplicated row states and text fields."""
from __future__ import annotations

import importlib.util
import json
import struct
import sys
import types
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent
MOVIE = ROOT / "port/android-native/app/src/main/assets/original-cache/data/menus/dqcharmenu_droid.swf"
PARSER = ROOT / "port/windows-foundation/features/faery_menu/source_swf_geometry.py"
HEADER = HERE / "runtime_quest_menu_art_v1.hpp"
SOURCE = HERE / "runtime_quest_menu_art_v1.cpp"

spec = importlib.util.spec_from_file_location("quest_art_swf", PARSER)
swf = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = swf
spec.loader.exec_module(swf)


def source_solid_style_reader(data, at, shape_id):
    count = data[at]
    at += 1
    if count == 255:
        count = struct.unpack_from("<H", data, at)[0]
        at += 2
    styles = []
    for _ in range(count):
        kind = data[at]
        at += 1
        if kind != 0:
            raise ValueError(f"Shape {shape_id}: expected authored solid fill")
        width = 4 if current_shape_code == 32 else 3
        rgba = list(data[at:at + width])
        at += width
        rgba.extend([255] * (4 - len(rgba)))
        # UVs are unused by solid-color projection. This invertible matrix
        # merely lets the shared contour tessellator retain source vertices.
        styles.append({"kind": 0, "bitmap_id": 0,
                       "matrix_twips": [1000000., 0., 0., 1000000., -100000000., -100000000.],
                       "rgba": rgba})
    lines = data[at]
    at += 1
    if lines:
        raise ValueError(f"Shape {shape_id}: solid shape has unsupported source line style")
    return styles, at


def parse_shape_full(code, data):
    shape_id = struct.unpack_from("<H", data)[0]
    bounds, at = swf.rect(data, 2)
    def read_styles(at):
        count = data[at]
        at += 1
        if count == 255:
            count = struct.unpack_from("<H", data, at)[0]
            at += 2
        records = []
        for _ in range(count):
            kind = data[at]
            at += 1
            if kind == 0:
                width = 4 if code == 32 else 3
                color = list(data[at:at + width])
                at += width
                color.extend([255] * (4 - len(color)))
                records.append({"kind": "solid", "rgba": color})
            elif kind in (0x40, 0x41, 0x42, 0x43):
                bitmap = struct.unpack_from("<H", data, at)[0]
                at += 2
                matrix, at = swf.matrix(data, at)
                if bitmap != 1:
                    raise ValueError(f"Quest shape {shape_id}: unsupported atlas bitmap {bitmap}")
                records.append({"kind": "bitmap", "matrix": matrix})
            else:
                raise ValueError(f"Quest shape {shape_id}: unsupported SWF fill kind {kind}")
        line_count = data[at]
        at += 1
        if line_count:
            raise ValueError(f"Quest shape {shape_id}: source line styles unsupported")
        return records, at

    styles, at = read_styles(at)
    bits = swf.Bits(data, at)
    fill_bits, line_bits = bits.u(4), bits.u(4)
    x = y = fill0 = fill1 = fill_base = 0
    edges, commands = {}, []
    while True:
        if not bits.u(1):
            flags = bits.u(5)
            if not flags:
                break
            if flags & 1:
                width = bits.u(5)
                x, y = bits.s(width), bits.s(width)
                commands.append({"move": [x, y]})
            if flags & 2:
                fill0 = bits.u(fill_bits)
                if fill0: fill0 += fill_base
            if flags & 4:
                fill1 = bits.u(fill_bits)
                if fill1: fill1 += fill_base
            if flags & 8: bits.u(line_bits)
            if flags & 16:
                fill_base = len(styles)
                new_styles, next_at = read_styles(bits.end())
                styles.extend(new_styles)
                bits = swf.Bits(data, next_at)
                fill_bits, line_bits = bits.u(4), bits.u(4)
                commands.append({"new_styles": new_styles})
            continue
        straight, width = bits.u(1), bits.u(4) + 2
        start = (x, y)
        if straight:
            if bits.u(1): dx, dy = bits.s(width), bits.s(width)
            elif bits.u(1): dx, dy = 0, bits.s(width)
            else: dx, dy = bits.s(width), 0
            x, y = x + dx, y + dy
            segment = [start, (x, y)]
        else:
            control = (x + bits.s(width), y + bits.s(width))
            x, y = control[0] + bits.s(width), control[1] + bits.s(width)
            segment = [start] + swf.flatten_quad(start, control, (x, y))
        edges.setdefault(fill0, []) if fill0 else None
        if fill0: edges[fill0].append(list(reversed(segment)))
        if fill1: edges.setdefault(fill1, []).append(segment)
        if fill0 > len(styles) or fill1 > len(styles):
            raise ValueError(f"Quest shape {shape_id}: fill index outside styles")

    bitmap_vertices, solid_vertices = [], []

    def area(ring):
        ring = ring[:-1] if ring[0] == ring[-1] else ring
        return sum(ring[i][0] * ring[(i + 1) % len(ring)][1] -
                   ring[(i + 1) % len(ring)][0] * ring[i][1]
                   for i in range(len(ring))) / 2

    def point_inside(point, ring):
        inside = False
        ring = ring[:-1] if ring[0] == ring[-1] else ring
        x, y = point
        for a, b in zip(ring, ring[1:] + ring[:1]):
            if (a[1] > y) != (b[1] > y):
                cross_x = (b[0] - a[0]) * (y - a[1]) / (b[1] - a[1]) + a[0]
                if x < cross_x: inside = not inside
        return inside

    def segments_cross(a, b, c, d):
        def orient(p, q, r):
            return (q[0]-p[0])*(r[1]-p[1])-(q[1]-p[1])*(r[0]-p[0])
        x1, x2, x3, x4 = orient(a,b,c), orient(a,b,d), orient(c,d,a), orient(c,d,b)
        return x1*x2 < -1e-8 and x3*x4 < -1e-8

    def merge_hole(outer, hole, all_rings):
        outer = outer[:-1] if outer[0] == outer[-1] else list(outer)
        hole = hole[:-1] if hole[0] == hole[-1] else list(hole)
        if area(outer) < 0: outer.reverse()
        if area(hole) > 0: hole.reverse()
        hi = max(range(len(hole)), key=lambda i: (hole[i][0], -hole[i][1]))
        h = hole[hi]
        candidates = sorted(range(len(outer)), key=lambda i: ((outer[i][0]-h[0])**2+(outer[i][1]-h[1])**2, i))
        oi = None
        for candidate in candidates:
            o = outer[candidate]
            if not point_inside(((h[0]+o[0])/2, (h[1]+o[1])/2), outer):
                continue
            if any(point_inside(((h[0]+o[0])/2, (h[1]+o[1])/2), ring) for ring in all_rings if ring != outer and ring != hole):
                continue
            ring_edges = [ring[:-1] if ring[0] == ring[-1] else ring for ring in all_rings]
            if any(segments_cross(h,o,a,b) for ring in ring_edges for a,b in zip(ring,ring[1:]+ring[:1])
                   if a not in (h,o) and b not in (h,o)):
                continue
            oi = candidate
            break
        if oi is None:
            raise ValueError(f"Quest shape {shape_id}: no visible bridge for source hole")
        cycle = hole[hi:] + hole[:hi + 1]
        return outer[:oi + 1] + cycle + [outer[oi]] + outer[oi + 1:]

    def triangulate_contours(contours):
        rings = [ring[:-1] if ring[0] == ring[-1] else list(ring) for ring in contours]
        parent = [None] * len(rings)
        for i, ring in enumerate(rings):
            containers = [j for j, other in enumerate(rings) if j != i and
                          abs(area(other)) > abs(area(ring)) and point_inside(ring[0], other)]
            if containers: parent[i] = min(containers, key=lambda j: abs(area(rings[j])))
        result = []
        for i, outer in enumerate(rings):
            if parent[i] is not None: continue
            holes = [j for j, p in enumerate(parent) if p == i]
            merged = outer
            for hole_index in holes:
                merged = merge_hole(merged, rings[hole_index], [outer] + [rings[j] for j in holes])
            points = list(merged)
            changed = True
            while changed and len(points) > 3:
                changed = False
                for index in range(len(points)):
                    if points[index] == points[index-1] or points[index] == points[(index+1)%len(points)]:
                        continue
                    if abs(swf.cross(points[index-1],points[index],points[(index+1)%len(points)])) < 1e-8:
                        points.pop(index);changed=True;break
            winding = 1 if area(points) > 0 else -1
            remaining = list(range(len(points)))
            triangles = []
            guard = 0
            while len(remaining) > 3:
                guard += 1
                if guard > len(points)*len(points): raise ValueError(f"Quest shape {shape_id}: bridged hole ear clipping stalled")
                found = False
                for j, current in enumerate(remaining):
                    prev, nxt = remaining[j-1], remaining[(j+1)%len(remaining)]
                    a,b,c = points[prev],points[current],points[nxt]
                    if swf.cross(a,b,c)*winding <= 1e-8: continue
                    blocked = False
                    for k in remaining:
                        point=points[k]
                        if k in (prev,current,nxt) or point in (a,b,c): continue
                        if all(swf.cross(x,y,point)*winding >= -1e-8 for x,y in ((a,b),(b,c),(c,a))):
                            blocked=True;break
                    if blocked: continue
                    triangles.extend((a,b,c));remaining.pop(j);found=True;break
                if not found: raise ValueError(f"Quest shape {shape_id}: cannot triangulate source contour with holes")
            triangles.extend(points[k] for k in remaining)
            result.extend(triangles)
        expected = sum(abs(area(ring)) * (-1 if parent[i] is not None else 1)
                       for i, ring in enumerate(rings))
        measured = sum(abs(swf.cross(*result[i:i+3]))/2 for i in range(0,len(result),3))
        if abs(measured-expected)>1e-4:
            raise ValueError(f"Quest shape {shape_id}: hole tessellation area mismatch {measured} vs {expected}")
        return result
    for fill_index, segments in sorted(edges.items()):
        chains = []
        while segments:
            chain = segments.pop(0)
            while chain[-1] != chain[0]:
                matches = [i for i, segment in enumerate(segments) if segment[0] == chain[-1]]
                if len(matches) != 1:
                    raise ValueError(f"Quest shape {shape_id}: open/ambiguous source contour")
                chain.extend(segments.pop(matches[0])[1:])
            chains.append(chain)
        points = triangulate_contours(chains)
        style = styles[fill_index - 1]
        if style["kind"] == "solid":
            solid_vertices.extend([[x, y, 0., 0.] for x, y in points])
        else:
            a, b, c, d, tx, ty = style["matrix"]
            determinant = a * d - b * c
            if abs(determinant) < 1e-12:
                raise ValueError(f"Quest shape {shape_id}: singular source bitmap matrix")
            for x, y in points:
                u = (d * (x - tx) - c * (y - ty)) / determinant / 1024
                v = (-b * (x - tx) + a * (y - ty)) / determinant / 1024
                bitmap_vertices.append([x, y, u, v])
    return {"shape_id": shape_id, "bounds_twips": bounds, "source_commands": commands,
            "triangles": bitmap_vertices, "solid_triangles": solid_vertices,
            "solid_rgba": next((style["rgba"] for style in styles if style["kind"] == "solid"), None)}


def cpp(value):
    return swf.cpp_number(float(value))


def arr(values):
    return "{" + ",".join(cpp(value) for value in values) + "}"


def field_cpp(field):
    return ("{" + json.dumps(field["path"]) + "," + str(field["character"]) + "," +
            str(field["font"]) + "," + cpp(field["height_twips"] / 20) + "," +
            arr(field["bounds"]) + ",{" + ",".join(str(int(v)) for v in field["rgba"]) + "}," +
            str(field["align"]) + "," + arr(field["matrix"]) + "," +
            arr(field["local_bounds"]) + "," + arr(field["margins"]) + "," +
            cpp(field["leading"]) + "}")


def vertex_cpp(vertex):
    return "{" + ",".join(cpp(value) for value in vertex) + "}"


def batch_cpp(group):
    path, shape_id, vertices = group
    return "{" + json.dumps(path) + "," + str(shape_id) + ",{" + ",".join(
        vertex_cpp(vertex) for vertex in vertices) + "}}"


def solid_cpp(solid):
    path, shape_id, vertices, rgba, after = solid
    return ("{{" + json.dumps(path) + "," + str(shape_id) + ",{" + ",".join(
        vertex_cpp(vertex) for vertex in vertices) + "}},{" + ",".join(
        cpp(channel / 255) for channel in rgba) + "}," + json.dumps(after) + "}")


raw = MOVIE.read_bytes()
if raw[:3] not in (b"CWS", b"FWS"):
    raise ValueError("Unsupported retained Quest SWF compression")
data = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
if len(data) != struct.unpack_from("<I", data, 4)[0]:
    raise ValueError("Retained Quest SWF declared length differs")
stage, at = swf.rect(data, 8)
if stage != [0, 9600, 0, 6400]:
    raise ValueError(f"Unexpected original Quest stage: {stage}")
at += 4
sprites, edits, shapes, solids, labels = {}, {}, {}, {}, {}
shape_tags = {}
current_shape_code = 0
solid_reader_globals = dict(swf.parse_shape.__globals__)
solid_reader_globals["read_bitmap_styles"] = source_solid_style_reader
parse_solid = types.FunctionType(swf.parse_shape.__code__, solid_reader_globals)
for code, tag in swf.tags(data, at, len(data)):
    if code == 39:
        character, count = struct.unpack_from("<HH", tag)
        frames = swf.parse_timeline(tag, 4)
        sprites[character] = frames
        labels[character] = {}
        frame = 0
        for subcode, subtag in swf.tags(tag, 4, len(tag)):
            if subcode == 43:
                labels[character][subtag.split(b"\0")[0].decode("utf8")] = frame
            elif subcode == 1:
                frame += 1
        if frames and len(frames) != count:
            raise ValueError(f"Sprite {character} frame count mismatch")
    elif code in (2, 22, 32):
        shape_id = struct.unpack_from("<H", tag)[0]
        swf.ROLES[shape_id] = f"quest_shape_{shape_id}"
        shape_tags[shape_id] = (code, tag)
    elif code == 37:
        edit = swf.parse_edit_text(tag)
        edits[edit["character"]] = edit
root_frames = swf.parse_timeline(data, at)
root_page = [item for item in root_frames[0].values()
             if item.get("character") == 599 and item.get("name") == "menu_QuestLogSheetNEW"]
if len(root_page) != 1:
    raise ValueError("Original Quest page placement missing or ambiguous")
page_matrix = root_page[0]["matrix"]

reachable_shapes = set()
def collect_reachable(character, depth=0):
    if depth > 40:
        raise ValueError("Quest source character graph nesting bound")
    if character in shape_tags:
        reachable_shapes.add(character)
        return
    for timeline in sprites.get(character, []):
        for item in timeline.values():
            if item.get("character") is not None:
                collect_reachable(item["character"], depth + 1)
collect_reachable(599)
for shape_id in sorted(reachable_shapes):
    code, tag = shape_tags[shape_id]
    shapes[shape_id] = parse_shape_full(code, tag)


def xy(m, x, y):
    return [(m[0] * x + m[2] * y + m[4]) / 20,
            (m[1] * x + m[3] * y + m[5]) / 20]


def edge(a, b, point):
    return (b[0] - a[0]) * (point[1] - a[1]) - (b[1] - a[1]) * (point[0] - a[0])


def clip_triangle(poly, clip):
    direction = 1 if edge(clip[0], clip[1], clip[2]) >= 0 else -1
    for a, b in zip(clip, clip[1:] + clip[:1]):
        if not poly:
            break
        result, previous = [], poly[-1]
        previous_value = edge(a, b, previous) * direction
        for current in poly:
            current_value = edge(a, b, current) * direction
            if (current_value >= -1e-6) != (previous_value >= -1e-6):
                fraction = previous_value / (previous_value - current_value)
                result.append([previous[i] + fraction * (current[i] - previous[i]) for i in range(4)])
            if current_value >= -1e-6:
                result.append(current)
            previous, previous_value = current, current_value
        poly = result
    return poly


def clip_triangles(vertices, mask):
    out = []
    for i in range(0, len(vertices), 3):
        for j in range(0, len(mask), 3):
            polygon = clip_triangle(vertices[i:i + 3], mask[j:j + 3])
            for k in range(1, len(polygon) - 1):
                if abs(edge(polygon[0], polygon[k], polygon[k + 1])) > 1e-6:
                    out.extend(polygon[:1] + polygon[k:k + 2])
    return out


def shape_vertices(shape, matrix):
    return [[*xy(matrix, vertex[0], vertex[1]), vertex[2], vertex[3]]
            for vertex in shape["triangles"]]


def solid_shape_vertices(shape, matrix):
    return [[*xy(matrix, vertex[0], vertex[1]), vertex[2], vertex[3]]
            for vertex in shape["solid_triangles"]]


def mask_vertices(character, matrix, depth=0):
    if depth > 30:
        raise ValueError("Quest clip-mask nesting bound")
    if character in shapes:
        return shape_vertices(shapes[character], matrix)
    if character in solids:
        return shape_vertices(solids[character], matrix)
    if character not in sprites or not sprites[character]:
        raise ValueError(f"Quest clip-mask definition missing: {character}")
    out = []
    for _, item in sorted(sprites[character][0].items()):
        out.extend(mask_vertices(item["character"], swf.multiply(matrix, item["matrix"]), depth + 1))
    return out


def text_field(path, character, matrix):
    record = edits[character]
    bounds = record["bounds_twips"]
    corners = [xy(matrix, x, y) for x, y in ((bounds[0], bounds[2]),
        (bounds[1], bounds[2]), (bounds[1], bounds[3]), (bounds[0], bounds[3]))]
    return {"path": path, "character": character, "font": record["font"],
            "height_twips": record["height_twips"],
            "bounds": [min(p[0] for p in corners), max(p[0] for p in corners),
                       min(p[1] for p in corners), max(p[1] for p in corners)],
            "rgba": record["rgba"], "align": record["layout"].get("align", 0),
            "matrix": [matrix[0], matrix[1], matrix[2], matrix[3], matrix[4] / 20, matrix[5] / 20],
            "local_bounds": [v / 20 for v in bounds],
            "margins": [record["layout"].get(name, 0) / 20 for name in ("left_margin", "right_margin", "indent")],
            "leading": record["layout"].get("leading", 0) / 20}


def walk(character, matrix, path, frame_overrides=None, excluded=frozenset(), depth=0):
    if depth > 30:
        raise ValueError("Quest art nesting bound")
    if path.rsplit("/", 1)[-1] in excluded:
        return [], [], []
    if character in shapes:
        shape = shapes[character]
        bitmap = [(path, character, shape_vertices(shape, matrix))] if shape["triangles"] else []
        solid = ([(path, character, solid_shape_vertices(shape, matrix), shape["solid_rgba"], "")
                  ] if shape["solid_triangles"] else [])
        return bitmap, solid, []
    if character in edits:
        return [], [], [text_field(path, character, matrix)]
    if character not in sprites or not sprites[character]:
        return [], [], []
    frame = (frame_overrides or {}).get(character, 0)
    if frame >= len(sprites[character]):
        raise ValueError(f"Quest art requested unavailable frame {frame} from sprite {character}")
    art, solid_art, fields = [], [], []
    masks = []
    for depth_id, item in sorted(sprites[character][frame].items()):
        masks = [mask for mask in masks if depth_id <= mask[0]]
        name = item.get("name", f"d{depth_id}")
        if name in excluded:
            continue
        if item["color"][0][3] == 0 and item["color"][1][3] == 0:
            continue
        if item["color"] != [[1.0] * 4, [0.0] * 4]:
            raise ValueError(f"Quest authored placement requires unimplemented color transform: {path}/{name}")
        child_matrix = swf.multiply(matrix, item["matrix"])
        if item.get("clip_depth"):
            masks.append((item["clip_depth"], mask_vertices(item["character"], child_matrix)))
            continue
        a, s, f = walk(item["character"], child_matrix, path + "/" + name,
                       frame_overrides, excluded, depth + 1)
        first = len(art)
        art.extend(a)
        solid_art.extend(s)
        fields.extend(f)
        for _, mask in masks:
            for index in range(first, len(art)):
                old_path, shape_id, vertices = art[index]
                art[index] = (old_path, shape_id, clip_triangles(vertices, mask))
    return art, solid_art, fields


def placements(character, name):
    return swf.placed_path(sprites[character], name)


def compose_matrix(*matrices):
    result = swf.IDENTITY
    for matrix in matrices:
        result = swf.multiply(result, matrix)
    return result


def placement_to_pixels(matrix):
    return [matrix[0], matrix[1], matrix[2], matrix[3], matrix[4] / 20, matrix[5] / 20]


def flatten_groups(groups):
    return [(path, shape_id, vertices) for path, shape_id, vertices in groups if vertices]


def flatten_solids(groups):
    out = []
    for path, shape_id, vertices, rgba, _ in groups:
        if not vertices:
            continue
        after = ""
        out.append((path, shape_id, vertices, rgba, after))
    return out


exclude_page = {"btnQuests", "btn_Activate", "btn_ClickPreventer", "hitzone", "btn_down"}
page_art, page_solids, page_fields = walk(599, page_matrix, "menu_QuestLogSheetNEW",
                                          excluded=exclude_page)
page_art = flatten_groups(page_art)
page_solids = flatten_solids(page_solids)

# Dynamic duplicated row template remains local; caller composes this exact
# source matrix with its authored `_y = i * 50` row index.
row_states, row_fields = [], []
row_state_solids = []
for label in ("Unselected", "Selected"):
    frame = labels[564][label]
    art, solid_art, fields = walk(564, swf.IDENTITY, "btnQuests",
                                  {564: frame}, {"LightOn", "hitzone"})
    row_states.append(flatten_groups(art))
    row_state_solids.append(flatten_solids(solid_art))
    row_fields.append(fields)

# Source current marker is the actual LightOn child, kept as a distinct layer
# because ActionScript toggles its visibility from each row's `Current` value.
marker_child = placements(564, "LightOn")
marker_art, marker_solids, _ = walk(marker_child["character"], marker_child["matrix"],
                                     "btnQuests/LightOn")
marker_art = flatten_groups(marker_art)
marker_solids = flatten_solids(marker_solids)

# Retain the actual Activate button idle art and field; the source ActionScript
# only shows it for a selected Assigned row.
activate_place = placements(599, "btn_Activate")
activate_matrix = compose_matrix(page_matrix, activate_place["matrix"])
activate_art, activate_solids, activate_fields = walk(595, activate_matrix, "btn_Activate",
    {595: labels[595]["Idle"]}, {"hitzone"})
activate_art = flatten_groups(activate_art)
activate_solids = flatten_solids(activate_solids)

all_quests = placements(599, "AllQuests")
content = placements(all_quests["character"], "content")
category_transforms = []
for category_name in ("Assigned", "Completed"):
    category = placements(content["character"], category_name)
    row_template = placements(category["character"], "btnQuests")
    row_matrix = compose_matrix(page_matrix, all_quests["matrix"], content["matrix"],
                                category["matrix"], row_template["matrix"])
    category_transforms.append(placement_to_pixels(row_matrix))

def vector_array(groups):
    return "{" + ",".join(batch_cpp(group) for group in groups) + "}"


header = r'''#pragma once
#include "runtime_quest_menu_v1.hpp"
namespace dh::foundation {
struct RuntimeQuestMenuSourceArtV1 {
    std::vector<HudGeometryBatch> page_art;
    std::vector<character_menu::MenuSolidBatch> page_solids;
    std::vector<character_menu::MenuTextField> page_fields;
    std::array<std::vector<HudGeometryBatch>,2> row_art;
    std::array<std::vector<character_menu::MenuSolidBatch>,2> row_solids;
    std::array<std::vector<character_menu::MenuTextField>,2> row_fields;
    std::vector<HudGeometryBatch> current_marker_art;
    std::vector<character_menu::MenuSolidBatch> current_marker_solids;
    std::vector<HudGeometryBatch> activate_art;
    std::vector<character_menu::MenuSolidBatch> activate_solids;
    std::vector<character_menu::MenuTextField> activate_fields;
    std::array<std::array<float,6>,2> row_parent_matrices;
};
const RuntimeQuestMenuSourceArtV1& original_runtime_quest_menu_art_v1();
}
'''
HEADER.write_text(header, encoding="utf-8")

def menu_batch_init(group):
    path, shape_id, vertices = group
    return "{" + json.dumps(path) + "," + str(shape_id) + ",{" + ",".join(vertex_cpp(v) for v in vertices) + "}}"

def menu_solid_init(solid):
    path, shape_id, vertices, rgba, after = solid
    return "{{" + json.dumps(path) + "," + str(shape_id) + ",{" + ",".join(vertex_cpp(v) for v in vertices) + "}},{" + ",".join(cpp(c / 255) for c in rgba) + "}," + json.dumps(after) + "}"

def menu_field_init(field):
    color = "{" + ",".join(str(int(v)) for v in field["rgba"]) + "}"
    return "{" + json.dumps(field["path"]) + "," + str(field["character"]) + "," + str(field["font"]) + "," + cpp(field["height_twips"] / 20) + "," + arr(field["bounds"]) + "," + color + "," + str(field["align"]) + "," + arr(field["matrix"]) + "," + arr(field["local_bounds"]) + "," + arr(field["margins"]) + "," + cpp(field["leading"]) + "}"

def vec(name, typ, values, initializer):
    return f"static const std::vector<{typ}> {name}{{" + ",".join(initializer(x) for x in values) + "};\n"

source = ['#include "runtime_quest_menu_art_v1.hpp"\n', 'namespace dh::foundation {\n']
source.append(vec("page_art", "HudGeometryBatch", page_art, menu_batch_init))
source.append(vec("page_solids", "character_menu::MenuSolidBatch", page_solids, menu_solid_init))
source.append(vec("page_fields", "character_menu::MenuTextField", page_fields, menu_field_init))
for i, values in enumerate(row_states): source.append(vec(f"row_art_{i}", "HudGeometryBatch", values, menu_batch_init))
for i, values in enumerate(row_state_solids): source.append(vec(f"row_solids_{i}", "character_menu::MenuSolidBatch", values, menu_solid_init))
for i, values in enumerate(row_fields): source.append(vec(f"row_fields_{i}", "character_menu::MenuTextField", values, menu_field_init))
source.append(vec("marker_art", "HudGeometryBatch", marker_art, menu_batch_init))
source.append(vec("marker_solids", "character_menu::MenuSolidBatch", marker_solids, menu_solid_init))
source.append(vec("activate_art", "HudGeometryBatch", activate_art, menu_batch_init))
source.append(vec("activate_solids", "character_menu::MenuSolidBatch", activate_solids, menu_solid_init))
source.append(vec("activate_fields", "character_menu::MenuTextField", activate_fields, menu_field_init))
source.append("const RuntimeQuestMenuSourceArtV1& original_runtime_quest_menu_art_v1(){static const RuntimeQuestMenuSourceArtV1 art=[](){RuntimeQuestMenuSourceArtV1 x;x.page_art=page_art;x.page_solids=page_solids;x.page_fields=page_fields;x.row_art={row_art_0,row_art_1};x.row_solids={row_solids_0,row_solids_1};x.row_fields={row_fields_0,row_fields_1};x.current_marker_art=marker_art;x.current_marker_solids=marker_solids;x.activate_art=activate_art;x.activate_solids=activate_solids;x.activate_fields=activate_fields;" + "".join(f"x.row_parent_matrices[{i}]={arr(m)};" for i, m in enumerate(category_transforms)) + "return x;}();return art;}\n")
source.append("}\n")
SOURCE.write_text("".join(source), encoding="utf-8")
print(json.dumps({"status":"generated", "movie_sha256":__import__("hashlib").sha256(raw).hexdigest(),
                  "page_shapes":len(page_art),"page_fields":len(page_fields),
                  "solid_batches":len(page_solids),"row_shapes":[len(x) for x in row_states],
                  "row_fields":[len(x) for x in row_fields],"marker_shapes":len(marker_art),
                  "activate_shapes":len(activate_art),"row_parent_matrices":category_transforms},indent=2))
