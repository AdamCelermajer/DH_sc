"""Export original dqhud contours and placement timelines without a SWF runtime.

Uses the same checked RECT/MATRIX/tag interpretation as the repository's
inspect_gameplay_icon_shapes.py, extending it to actual shape edges. Requires
only Python's standard library. Fails rather than inventing unsupported shapes,
fills, masks, or missing placement paths. Regenerates hud_geometry.cpp and adds
the source evidence to reports/hud-source.json.
"""
from __future__ import annotations

import argparse
import copy
import hashlib
import json
import math
from pathlib import Path
import struct
import zlib

ROOT = Path(__file__).resolve().parents[3]
ROLES = {38: "portrait_warrior", 39: "portrait_rogue", 40: "portrait_mage",
         86: "hp_background", 88: "hp_fill", 143: "mp_background", 145: "mp_fill",
         148: "xp_background", 150: "xp_fill",
         153: "player_overlay_a", 155: "player_overlay_b",
         92: "target_decoration", 128: "target_level_background"}
IDENTITY = [1., 0., 0., 1., 0., 0.]
CURVE_TOLERANCE_TWIPS = .25
FRAME_OVERRIDES = {}


class Bits:
    def __init__(self, data, offset=0):
        self.data, self.i = data, offset * 8

    def u(self, n):
        if self.i + n > len(self.data) * 8:
            raise ValueError("Truncated SWF bit field")
        v = 0
        for _ in range(n):
            v = (v << 1) | ((self.data[self.i // 8] >> (7 - self.i % 8)) & 1)
            self.i += 1
        return v

    def s(self, n):
        v = self.u(n)
        return v - (1 << n) if n and v & (1 << (n - 1)) else v

    def end(self):
        return (self.i + 7) // 8


def rect(data, at):
    r = Bits(data, at)
    n = r.u(5)
    return [r.s(n) for _ in range(4)], r.end()


def matrix(data, at):
    r = Bits(data, at)
    a = d = 1.
    b = c = 0.
    if r.u(1):
        n = r.u(5)
        a, d = r.s(n) / 65536., r.s(n) / 65536.
    if r.u(1):
        n = r.u(5)
        b, c = r.s(n) / 65536., r.s(n) / 65536.
    n = r.u(5)
    return [a, b, c, d, r.s(n), r.s(n)], r.end()


def cxform(data, at):
    r = Bits(data, at)
    add, mult, n = r.u(1), r.u(1), r.u(4)
    m = [r.s(n) / 256. for _ in range(4)] if mult else [1.] * 4
    a = [r.s(n) / 255. for _ in range(4)] if add else [0.] * 4
    return [m, a], r.end()


def tags(data, at, end):
    while at + 2 <= end:
        h = struct.unpack_from("<H", data, at)[0]
        at += 2
        length, code = h & 63, h >> 6
        if length == 63:
            length = struct.unpack_from("<I", data, at)[0]
            at += 4
        if at + length > end:
            raise ValueError("Truncated SWF tag")
        yield code, data[at:at + length]
        at += length
        if code == 0:
            return


def multiply(a, b):
    return [a[0]*b[0]+a[2]*b[1], a[1]*b[0]+a[3]*b[1],
            a[0]*b[2]+a[2]*b[3], a[1]*b[2]+a[3]*b[3],
            a[0]*b[4]+a[2]*b[5]+a[4], a[1]*b[4]+a[3]*b[5]+a[5]]


def flatten_quad(start, control, end, depth=0):
    dx, dy = end[0]-start[0], end[1]-start[1]
    length = math.hypot(dx, dy)
    distance = abs(dx*(start[1]-control[1])-dy*(start[0]-control[0])) / length if length else math.dist(start, control)
    if distance <= CURVE_TOLERANCE_TWIPS:
        return [end]
    if depth >= 20:
        raise ValueError("Curve exceeds flattening bound")
    a = tuple((start[i]+control[i])/2 for i in range(2))
    b = tuple((control[i]+end[i])/2 for i in range(2))
    mid = tuple((a[i]+b[i])/2 for i in range(2))
    return flatten_quad(start, a, mid, depth+1) + flatten_quad(mid, b, end, depth+1)


def cross(a, b, c):
    return (b[0]-a[0])*(c[1]-a[1]) - (b[1]-a[1])*(c[0]-a[0])


def triangulate(contour):
    points = list(contour)
    if points[0] == points[-1]:
        points.pop()
    changed = True
    while changed and len(points) > 3:
        changed = False
        for i in range(len(points)):
            if abs(cross(points[i-1], points[i], points[(i+1) % len(points)])) < 1e-8:
                points.pop(i)
                changed = True
                break
    area = sum(points[i][0]*points[(i+1) % len(points)][1] - points[(i+1) % len(points)][0]*points[i][1] for i in range(len(points)))
    if len(points) < 3 or abs(area) < 1e-8:
        raise ValueError("Degenerate source contour")
    winding = 1 if area > 0 else -1
    remaining = list(range(len(points)))
    result = []
    while len(remaining) > 3:
        for j, current in enumerate(remaining):
            prev, nxt = remaining[j-1], remaining[(j+1) % len(remaining)]
            a, b, c = points[prev], points[current], points[nxt]
            if cross(a, b, c) * winding <= 1e-8:
                continue
            blocked = any(all(cross(x, y, points[k]) * winding >= -1e-8 for x, y in ((a,b),(b,c),(c,a)))
                          for k in remaining if k not in (prev,current,nxt))
            if not blocked:
                result.extend((a,b,c))
                remaining.pop(j)
                break
        else:
            raise ValueError("Cannot tessellate original contour (hole/self-intersection)")
    result.extend(points[k] for k in remaining)
    triangle_area = sum(abs(cross(*result[i:i+3])) for i in range(0,len(result),3))
    if abs(triangle_area - abs(area)) > 1e-5:
        raise ValueError("Tessellation area differs from source contour")
    return result


def read_bitmap_styles(data, at, shape_id):
    count = data[at]
    at += 1
    if count == 255:
        count = struct.unpack_from("<H", data, at)[0]
        at += 2
    styles = []
    for _ in range(count):
        kind = data[at]
        at += 1
        if kind not in (0x40, 0x41, 0x42, 0x43):
            raise ValueError(f"Shape {shape_id}: unsupported fill {kind}")
        bitmap = struct.unpack_from("<H", data, at)[0]
        at += 2
        fill_matrix, at = matrix(data, at)
        if bitmap != 1:
            raise ValueError(f"Shape {shape_id}: bitmap is not original HUD atlas1")
        styles.append({"kind":kind,"bitmap_id":bitmap,"matrix_twips":fill_matrix})
    lines = data[at]
    at += 1
    if lines:
        raise ValueError(f"Shape {shape_id}: original line styles require export")
    return styles, at


def parse_shape(code, data):
    shape_id = struct.unpack_from("<H", data)[0]
    bounds, at = rect(data, 2)
    styles, at = read_bitmap_styles(data,at,shape_id)
    r = Bits(data, at)
    fill_bits, line_bits = r.u(4), r.u(4)
    x = y = fill0 = fill1 = 0
    fill_base = 0
    edges, commands = {}, []
    while True:
        if not r.u(1):
            flags = r.u(5)
            if not flags:
                break
            if flags & 1:
                n = r.u(5)
                x, y = r.s(n), r.s(n)
                commands.append({"move": [x,y]})
            if flags & 2:
                fill0 = r.u(fill_bits)
                if fill0:
                    fill0 += fill_base
            if flags & 4:
                fill1 = r.u(fill_bits)
                if fill1:
                    fill1 += fill_base
            if flags & 8:
                r.u(line_bits)
            if flags & 16:
                # Same fill_base rule as original GameSWF shape.cpp1245.
                fill_base = len(styles)
                added, next_at = read_bitmap_styles(data,r.end(),shape_id)
                styles.extend(added)
                r = Bits(data,next_at)
                fill_bits,line_bits = r.u(4),r.u(4)
                commands.append({"new_styles":added})
            continue
        straight, n = r.u(1), r.u(4)+2
        start = (x,y)
        if straight:
            if r.u(1):
                dx, dy = r.s(n), r.s(n)
            elif r.u(1):
                dx, dy = 0, r.s(n)
            else:
                dx, dy = r.s(n), 0
            x, y = x+dx, y+dy
            segment = [start,(x,y)]
            commands.append({"line": [start,[x,y]], "fill0":fill0,"fill1":fill1})
        else:
            control = (x+r.s(n), y+r.s(n))
            x, y = control[0]+r.s(n), control[1]+r.s(n)
            segment = [start] + flatten_quad(start, control, (x,y))
            commands.append({"curve": [start,control,[x,y]], "fill0":fill0,"fill1":fill1})
        if fill1:
            edges.setdefault(fill1,[]).append(segment)
        if fill0:
            edges.setdefault(fill0,[]).append(list(reversed(segment)))
        if fill0 > len(styles) or fill1 > len(styles):
            raise ValueError("Unexpected fill index")
    contours = []
    triangles = []
    for fill, segments in sorted(edges.items()):
        fill_contours = []
        while segments:
            chain = segments.pop(0)
            while chain[-1] != chain[0]:
                matches = [i for i,e in enumerate(segments) if e[0] == chain[-1]]
                if len(matches) != 1:
                    raise ValueError(f"Shape {shape_id}: ambiguous/open source contour")
                chain.extend(segments.pop(matches[0])[1:])
            fill_contours.append(chain)
        if len(fill_contours) != 1:
            raise ValueError(f"Shape {shape_id}: multiple contours need a hole-aware tessellator")
        vertices = triangulate(fill_contours[0])
        contours.extend(fill_contours)
        a,b,c,d,tx,ty = styles[fill-1]["matrix_twips"]
        determinant = a*d-b*c
        if abs(determinant) < 1e-12:
            raise ValueError("Singular bitmap fill matrix")
        for x,y in vertices:
            u = (d*(x-tx)-c*(y-ty))/determinant/1024
            v = (-b*(x-tx)+a*(y-ty))/determinant/1024
            if not (0 <= u <= 1 and 0 <= v <= 1):
                raise ValueError("Original contour outside supplied atlas")
            triangles.append([x,y,u,v])
    return {"shape_id":shape_id,"role":ROLES[shape_id],"bounds_twips":bounds,
            "fill_records":styles,"source_commands":commands,
            "contours_twips":contours,"triangles":triangles}


def parse_timeline(data, at=0):
    display, frames = {}, []
    for code, tag in tags(data, at, len(data)):
        if code in (26,70):
            flags, depth = tag[0], struct.unpack_from("<H",tag,1)[0]
            q = 3
            extra_flags = 0
            if code == 70:
                extra_flags = tag[1]
                depth = struct.unpack_from("<H",tag,2)[0]
                q = 4
                if extra_flags & 8 or (extra_flags & 16 and flags & 2):
                    q = tag.index(0,q)+1
            current = copy.deepcopy(display.get(depth, {"matrix":IDENTITY,"color":[[1.]*4,[0.]*4]}))
            if not flags & 1:
                current = {"matrix":IDENTITY,"color":[[1.]*4,[0.]*4]}
            if flags & 2:
                current["character"] = struct.unpack_from("<H",tag,q)[0]
                q += 2
            if flags & 4:
                current["matrix"], q = matrix(tag,q)
            if flags & 8:
                current["color"], q = cxform(tag,q)
            if flags & 16:
                current["ratio"] = struct.unpack_from("<H",tag,q)[0]
                q += 2
            if flags & 32:
                end = tag.index(0,q)
                current["name"] = tag[q:end].decode("utf8")
                q = end+1
            if flags & 64:
                current["clip_depth"] = struct.unpack_from("<H",tag,q)[0]
                q += 2
            if extra_flags:
                current["placeobject3_extra_flags"] = extra_flags
                current["placeobject3_remaining_hex"] = tag[q:].hex()
            display[depth] = current
        elif code == 28:
            display.pop(struct.unpack_from("<H",tag)[0],None)
        elif code == 1:
            frames.append(copy.deepcopy(display))
    return frames


def placed_path(frames, name):
    found = [p for p in frames[0].values() if p.get("name") == name]
    if len(found) != 1:
        raise ValueError(f"Missing/ambiguous source placement: {name}")
    return found[0]


def parse_edit_text(data):
    char = struct.unpack_from("<H",data)[0]
    bounds, at = rect(data,2)
    flags = (data[at] << 8) | data[at+1]
    at += 2
    font, height, color = 0,0,[0,0,0,255]
    if flags & 0x100:
        font = struct.unpack_from("<H",data,at)[0]
        at += 2
    if flags & 0x80:
        at = data.index(0,at)+1
    if flags & (0x100|0x80):
        height = struct.unpack_from("<H",data,at)[0]
        at += 2
    if flags & 0x400:
        color = list(data[at:at+4])
        at += 4
    if flags & 0x200:
        at += 2
    layout = {}
    if flags & 0x20:
        layout = {"align":data[at],"left_margin":struct.unpack_from("<H",data,at+1)[0],
                  "right_margin":struct.unpack_from("<H",data,at+3)[0],
                  "indent":struct.unpack_from("<H",data,at+5)[0],
                  "leading":struct.unpack_from("<h",data,at+7)[0]}
        at += 9
    end = data.index(0,at)
    variable = data[at:end].decode("utf8")
    return {"character":char,"bounds_twips":bounds,"font":font,"height_twips":height,
            "rgba":color,"variable":variable,"layout":layout,"flags":flags}


def collect_text(sprites, edits, char, parent, role="", depth=0):
    if depth > 30:
        raise ValueError("Target text nesting bound")
    if char in edits:
        if role not in ("enemy_name","enemy_level"):
            return []
        record = copy.deepcopy(edits[char])
        record.update(role=role,matrix_twips=parent)
        x0,x1,y0,y1 = record["bounds_twips"]
        points = [(parent[0]*x+parent[2]*y+parent[4],parent[1]*x+parent[3]*y+parent[5])
                  for x,y in ((x0,y0),(x1,y0),(x1,y1),(x0,y1))]
        record["world_bounds_twips"] = [min(p[0] for p in points),max(p[0] for p in points),
                                        min(p[1] for p in points),max(p[1] for p in points)]
        return [record]
    result = []
    if char in sprites:
        for _,p in sorted(sprites[char][FRAME_OVERRIDES.get(char,0)].items()):
            next_role = role if role in ("enemy_name","enemy_level") else p.get("name",role)
            fields = collect_text(sprites,edits,p["character"],multiply(parent,p["matrix"]),next_role,depth+1)
            if p.get("placeobject3_extra_flags"):
                for field in fields:
                    field.setdefault("placeobject3_extras",[]).append({"flags":p["placeobject3_extra_flags"],
                                                                      "remaining_hex":p["placeobject3_remaining_hex"]})
            result.extend(fields)
    return result


def collect_layers(sprites, shapes, char, parent, hp, mp, portrait, depth=0, xp=0):
    if depth > 30:
        raise ValueError("Source sprite nesting bound")
    if char in shapes:
        return [(char,parent)]
    if char not in sprites:
        return []
    frame = hp if char == 90 else mp if char == 147 else xp if char == 152 else portrait if char == 41 else FRAME_OVERRIDES.get(char,0)
    if frame >= len(sprites[char]):
        raise ValueError("Source frame outside timeline")
    result = []
    for _, p in sorted(sprites[char][frame].items()):
        # Export the explicit player HUD scope. Other controls reuse the same
        # shape IDs (including hidden portrait children) and are separate UI.
        if char in (452,461,465,469) and p.get("name") not in ("HealthBars","btn_charactermenu"):
            continue
        if char == 158 and p.get("name") != "player":
            continue
        if p["color"][0][3] == 0 and p["color"][1][3] == 0:
            continue
        child = p.get("character")
        if child is None:
            raise ValueError("Source placement missing character")
        subset = collect_layers(sprites,shapes,child,multiply(parent,p["matrix"]),hp,mp,portrait,depth+1,xp)
        if subset:
            if p.get("clip_depth"):
                raise ValueError("Selected original art requires a clipping mask")
            if p["color"] != [[1.]*4,[0.]*4]:
                raise ValueError(f"Selected original art requires a color transform at {char}->{child}: {p['color']}")
            if p.get("placeobject3_extra_flags"):
                raise ValueError("Selected bitmap art requires PlaceObject3 filter/blend semantics")
        result.extend(subset)
    return result


def cpp_number(x):
    if x == int(x):
        return f"{int(x)}.0f"
    return f"{x:.10g}f"


def cpp_matrix(m):
    return "{" + ",".join(cpp_number(x) for x in m) + "}"


PORTRAIT_RUNTIME = '// Original decoded MenusGraphics_droid atlas measurements and bitmap fills.\n// Same correction as engine-ui/authored_hud_portrait_v4.cpp, align_v5:\n// alpha>=128 connected paint bounding-box centres, not bitmap shape centres.\n// Frame155 dark aperture atlas bounds[698,734]x[480,517], fill20/-15450/-9986.\nconst std::array<float,2> portrait_paint_centres[]{\n {700.f*15.10906982421875f-10293.f,687.f*15.10906982421875f-10052.f},\n {630.5f*15.10906982421875f-9234.f,688.5f*15.10906982421875f-10082.f},\n {559.f*15.10906982421875f-8160.f,689.f*15.10906982421875f-10082.f}\n};\nstd::array<float,2> source_point(const Matrix& m,float x,float y){\n return {(m[0]*x+m[2]*y+m[4])/20.f,(m[1]*x+m[3]*y+m[5])/20.f};\n}\nstd::array<float,2> portrait_aperture_centre(unsigned style){\n return source_point(static_layers[style][3].matrix,716.f*20.f-15450.f,498.5f*20.f-9986.f);\n}'
PORTRAIT_BOUNDS = 'bool original_hud_portrait_bounds(unsigned style,std::array<float,4>& out,std::string& error){\n if(style>3){error="Original HUD portrait layout outside source domain";return false;}\n const auto& m=static_layers[style][3].matrix;\n const auto a=source_point(m,698.f*20.f-15450.f,480.f*20.f-9986.f);\n const auto b=source_point(m,734.f*20.f-15450.f,517.f*20.f-9986.f);\n out={std::min(a[0],b[0]),std::max(a[0],b[0]),std::min(a[1],b[1]),std::max(a[1],b[1])};\n error.clear();return true;\n}'
PORTRAIT_CORRECTION = '   if(index==-3){\n    const auto& local=portrait_paint_centres[portrait];\n    const auto before=source_point(m,local[0],local[1]);const auto target=portrait_aperture_centre(style);\n    for(auto& vertex:batch.triangles){vertex.x+=target[0]-before[0];vertex.y+=target[1]-before[1];}\n   }'

def main():
    args = argparse.ArgumentParser()
    args.add_argument("--source",type=Path,default=ROOT/"port/android-native/app/src/main/assets/original-cache/data/menus/dqhud_droid.swf")
    opts = args.parse_args()
    raw = opts.source.read_bytes()
    if raw[:3] not in (b"CWS",b"FWS"):
        raise ValueError("Unsupported SWF compression")
    data = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
    if len(data) != struct.unpack_from("<I",data,4)[0]:
        raise ValueError("SWF length mismatch")
    stage, at = rect(data,8)
    if stage != [0,9600,0,6400]:
        raise ValueError("Unexpected original HUD stage")
    at += 4
    shapes, sprites, edits, labels, fonts = {}, {}, {}, {}, {}
    for code, tag in tags(data,at,len(data)):
        if code in (2,22,32) and struct.unpack_from("<H",tag)[0] in ROLES:
            record = parse_shape(code,tag)
            shapes[record["shape_id"]] = record
        elif code == 39:
            char, count = struct.unpack_from("<HH",tag)
            frames = parse_timeline(tag,4)
            if len(frames) != count:
                raise ValueError("Sprite frame count mismatch")
            sprites[char] = frames
            label_frame = 0
            labels[char] = {}
            for subcode, subtag in tags(tag,4,len(tag)):
                if subcode == 43:
                    labels[char][subtag.split(b'\0')[0].decode("utf8")] = label_frame
                elif subcode == 1:
                    label_frame += 1
        elif code == 37:
            record = parse_edit_text(tag)
            edits[record["character"]] = record
        elif code in (48,75):
            font_id = struct.unpack_from("<H",tag)[0]
            name_length = tag[4]
            fonts[font_id] = {"font_id":font_id,"name":tag[5:5+name_length].decode("utf8").rstrip('\0'),
                              "flags":tag[2],"glyphs":struct.unpack_from("<H",tag,5+name_length)[0],"tag":code}
            if tag[2] & 0x80:
                count = fonts[font_id]["glyphs"]
                offsets_at = 7+name_length
                offset_bytes = 4 if tag[2] & 8 else 2
                code_offset = int.from_bytes(tag[offsets_at+count*offset_bytes:offsets_at+(count+1)*offset_bytes],"little")
                layout_at = offsets_at+code_offset+count*(2 if tag[2]&4 else 1)
                ascent,descent,leading = struct.unpack_from("<hhh",tag,layout_at)
                fonts[font_id].update(ascent=ascent,descent=descent,leading=leading)
                code_at = offsets_at+code_offset
                code_bytes = 2 if tag[2]&4 else 1
                fonts[font_id]["glyph_codes"] = [int.from_bytes(tag[code_at+i*code_bytes:code_at+(i+1)*code_bytes],"little") for i in range(count)]
                fonts[font_id]["advance_table"] = list(struct.unpack_from("<"+"h"*count,tag,layout_at+6))
                bounds_at = layout_at+6+count*2
                for _ in range(count):
                    _,bounds_at = rect(tag,bounds_at)
                fonts[font_id]["kerning_count"] = struct.unpack_from("<H",tag,bounds_at)[0]
                pairs = []
                pair_at = bounds_at+2
                for _ in range(fonts[font_id]["kerning_count"]):
                    first = int.from_bytes(tag[pair_at:pair_at+code_bytes],"little")
                    second = int.from_bytes(tag[pair_at+code_bytes:pair_at+code_bytes*2],"little")
                    adjustment = struct.unpack_from("<h",tag,pair_at+code_bytes*2)[0]
                    pairs.append([first,second,adjustment])
                    pair_at += code_bytes*2+2
                fonts[font_id]["kerning_pairs"] = pairs
    if set(shapes) != set(ROLES):
        raise ValueError("Required original shapes missing")
    if "show" not in labels[142]:
        raise ValueError(f"Original target show label missing: {labels[142]}")
    FRAME_OVERRIDES[142] = labels[142]["show"]
    root_frames = parse_timeline(data,at)
    layouts = []
    for style in range(4):
        menu = placed_path(root_frames,f"menu_HUD_{style}")
        elements = placed_path(sprites[menu["character"]],"HUDelements")
        for placement in (menu,elements):
            if placement.get("clip_depth") or placement["color"] != [[1.]*4,[0.]*4]:
                raise ValueError("Selected HUD root requires source mask/color transform")
        parent = multiply(menu["matrix"],elements["matrix"])
        initial = collect_layers(sprites,shapes,elements["character"],parent,0,0,0)
        # XP background (148) is appended after the HEAD static layers, never inserted among them:
        # portrait_aperture_centre() and original_hud_portrait_bounds() read static_layers[style][3] (shape 155).
        static = [(char,m) for char,m in initial if char not in (38,39,40,88,145,148,150)]
        static += [(char,m) for char,m in initial if char == 148]
        dynamic = {}
        for role, count in (("hp_fill",100),("mp_fill",100),("xp_fill",100),("portrait",3)):
            values = []
            for frame in range(count):
                layers = collect_layers(sprites,shapes,elements["character"],parent,
                                        frame if role == "hp_fill" else 0,
                                        frame if role == "mp_fill" else 0,
                                        frame if role == "portrait" else 0,
                                        xp=frame if role == "xp_fill" else 0)
                found = [(char,m) for char,m in layers if (char in (38,39,40) if role == "portrait" else ROLES[char] == role)]
                if len(found) != 1:
                    raise ValueError("Required visible art must have one source layer")
                values.append(found[0])
            dynamic[role] = values
        layouts.append({"style":style,"menu_placement":menu,"elements_placement":elements,
                        "initial_layers":initial,"static_layers":static,"dynamic":dynamic})

    target_initial = collect_layers(sprites,shapes,142,IDENTITY,0,0,0)
    target_static = [(char,m) for char,m in target_initial if char != 88]
    target_frames = []
    for hp in range(100):
        found = [(char,m) for char,m in collect_layers(sprites,shapes,142,IDENTITY,hp,0,0) if char == 88]
        if len(found) != 1:
            raise ValueError("Original target HP layer missing")
        target_frames.append(found[0])
    target_order = [-1 if char == 88 else next(i for i,(c,_) in enumerate(target_static) if c == char)
                    for char,_ in target_initial]
    target_text = collect_text(sprites,edits,142,IDENTITY)
    if sorted(x["role"] for x in target_text) != ["enemy_level","enemy_name"]:
        raise ValueError(f"Source target name/level fields missing: {target_text}")
    target_points = []
    for char,m in target_initial:
        target_points.extend((m[0]*v[0]+m[2]*v[1]+m[4],m[1]*v[0]+m[3]*v[1]+m[5]) for v in shapes[char]["triangles"])
    for field in target_text:
        x0,x1,y0,y1 = field["world_bounds_twips"]
        target_points.extend(((x0,y0),(x1,y1)))
    target_bounds = [min(p[0] for p in target_points)/20,max(p[0] for p in target_points)/20,
                     min(p[1] for p in target_points)/20,max(p[1] for p in target_points)/20]
    marker_projection = json.loads((ROOT/"port/game-data/reference/effects-tables/original-reader-projection.json").read_text())
    marker_rows = json.loads((ROOT/"port/level-world/reports/target-art-v28/target-set-rows.json").read_text())
    base = marker_projection["names"][0].index("target_circle_00_chest")
    markers = []
    for kind in range(9):
        effect_set = base+kind
        row = marker_rows["rows"][marker_rows["ids"].index(effect_set)]
        if len(row["steps"]) != 1:
            raise ValueError("Original marker needs composite effect runtime")
        step = row["steps"][0]
        path = marker_projection["dictionary_paths"][step["file"]] if step["file"] >= 0 else ""
        markers.append({"interaction_type":kind,"effect_set":effect_set,
                        "effect_name":marker_projection["names"][0][effect_set],
                        "model_uri":path,"self_illum":bool(step["self_illum"]),
                        "scale_with_anchor":bool(step["scale_with_anchor"]),"source_step":step})

    cpp = ['// Generated by tools/export_hud_geometry.py from original SWF; do not hand-edit.',
           '#include "hud_geometry.hpp"', '#include "hud_glyphs.hpp"', '#include <algorithm>', '#include <cmath>', '#include <new>', '#include <utility>',
           'namespace dh::foundation {', 'namespace {', 'using Matrix = std::array<float,6>;']
    cpp += ['const std::vector<HudShapeGeometry> shapes{']
    for char, shape in sorted(shapes.items()):
        vertices = ",".join("{"+",".join(cpp_number(x) for x in vertex)+"}" for vertex in shape["triangles"])
        cpp.append('{'+json.dumps(shape["role"])+f',{char},'+'{'+vertices+'}},')
    cpp += ['};','const std::vector<HudTargetMarkerArt> marker_art{']
    cpp.extend('{'+str(m["interaction_type"])+','+str(m["effect_set"])+','+json.dumps(m["model_uri"])+','+
               str(m["self_illum"]).lower()+','+str(m["scale_with_anchor"]).lower()+'},' for m in markers)
    cpp += ['};', 'struct Layer {unsigned shape; Matrix matrix;};']
    cpp += ['struct SourcePair {unsigned first,second;float adjustment;};','const SourcePair font7_pairs[]{']
    cpp.extend('{'+str(a)+','+str(b)+','+cpp_number(c/20)+'},' for a,b,c in fonts[7]["kerning_pairs"])
    cpp += ['};','struct SourceAdvance {unsigned code;float advance;};','const SourceAdvance font7_advances[]{']
    cpp.extend('{'+str(code)+','+cpp_number(advance/20)+'},' for code,advance in zip(fonts[7]["glyph_codes"],fonts[7]["advance_table"]))
    cpp.append('};')
    for name, values in (("target_static",target_static),("target_frames",target_frames)):
        cpp.append(f'const Layer {name}[]'+'{')
        cpp.extend('{'+str(char)+','+cpp_matrix(m)+'},' for char,m in values)
        cpp.append('};')
    cpp.append('const int target_order[]{'+','.join(map(str,target_order))+'};')
    cpp.append('const std::array<float,4> target_bounds'+cpp_matrix(target_bounds)+';')
    cpp.append('const std::vector<HudTargetTextField> target_text_fields{')
    for field in target_text:
        b = [v/20 for v in field["world_bounds_twips"]]
        m = field["matrix_twips"][:]
        m[4] /= 20
        m[5] /= 20
        font = fonts[field["font"]]
        units_divisor = 20 if font["tag"] == 75 else 1
        font_metrics = [font.get(name,0)/units_divisor for name in ("ascent","descent","leading")]
        local_bounds = [v/20 for v in field["bounds_twips"]]
        margins = [field["layout"].get(name,0)/20 for name in ("left_margin","right_margin","indent")]
        cpp.append('{'+json.dumps(field["role"])+','+str(field["character"])+','+str(field["font"])+','+
                   cpp_number(field["height_twips"]/20)+','+cpp_matrix(b)+','+cpp_matrix(m)+',{' +
                   ','.join(map(str,field["rgba"]))+'},'+str(field["layout"].get("align",0))+','+
                   cpp_number(field["layout"].get("leading",0)/20)+','+cpp_matrix(local_bounds)+','+
                   cpp_matrix(font_metrics)+','+cpp_matrix(margins)+'},')
    cpp.append('};')
    for layout in layouts:
        style = layout["style"]
        for role, values in layout["dynamic"].items():
            cpp.append(f'const Layer {role}_{style}[]'+'{')
            cpp.extend('{'+str(char)+','+cpp_matrix(m)+'},' for char,m in values)
            cpp.append('};')
        cpp.append(f'const Layer static_{style}[]'+'{')
        cpp.extend('{'+str(char)+','+cpp_matrix(m)+'},' for char,m in layout["static_layers"])
        cpp.append('};')
        # Preserve original depth order. -1,-2,-3 select timeline-dependent layers.
        order = []
        for char,_ in layout["initial_layers"]:
            if char == 88: order.append(-1)
            elif char == 145: order.append(-2)
            elif char == 150: order.append(-4)
            elif char in (38,39,40): order.append(-3)
            else: order.append(next(i for i,(c,_) in enumerate(layout["static_layers"]) if c == char))
        cpp.append(f'const int order_{style}[]'+'{'+','.join(map(str,order))+'};')
    cpp += ['const Layer* hp_frames[]{hp_fill_0,hp_fill_1,hp_fill_2,hp_fill_3};',
            'const Layer* xp_frames[]{xp_fill_0,xp_fill_1,xp_fill_2,xp_fill_3};',
            'const Layer* mp_frames[]{mp_fill_0,mp_fill_1,mp_fill_2,mp_fill_3};',
            'const Layer* portraits[]{portrait_0,portrait_1,portrait_2,portrait_3};',
            'const Layer* static_layers[]{static_0,static_1,static_2,static_3};',
            'const int* orders[]{order_0,order_1,order_2,order_3};',
            'const unsigned order_sizes[]{'+','.join(str(len(x["initial_layers"])) for x in layouts)+'};',
            *PORTRAIT_RUNTIME.splitlines(),
            '} // namespace',
            'const std::vector<HudShapeGeometry>& original_hud_shapes(){return shapes;}',
            'const std::vector<HudTargetMarkerArt>& original_target_marker_art(){return marker_art;}',
            *PORTRAIT_BOUNDS.splitlines(),
            'namespace {',
            '// xp_art=false omits source XP shapes 148/150 (legacy 6-argument compose_original_hud).',
            'bool compose_hud(unsigned style,unsigned hp,unsigned mp,unsigned xp,unsigned portrait,bool xp_art,HudGeometry& out,std::string& error){',
            ' if(style>3||hp>99||mp>99||xp>99||portrait>2){error="Original HUD layout/frame outside source domain";return false;}',
            ' try { HudGeometry result;',
            '  for(unsigned i=0;i<order_sizes[style];++i){const int index=orders[style][i];',
            '   const Layer& layer=index==-1?hp_frames[style][hp]:index==-2?mp_frames[style][mp]:index==-3?portraits[style][portrait]:index==-4?xp_frames[style][xp]:static_layers[style][index];',
            '   if(!xp_art&&(layer.shape==148||layer.shape==150))continue;',
            '   const HudShapeGeometry* source=nullptr;for(const auto& shape:shapes)if(shape.shape_id==layer.shape){source=&shape;break;}',
            '   if(!source){error="Original HUD geometry missing";return false;}',
            '   HudGeometryBatch batch;batch.role=source->role;batch.shape_id=source->shape_id;batch.triangles.reserve(source->triangles.size());',
            '   const auto& m=layer.matrix;for(const auto& v:source->triangles)batch.triangles.push_back({(m[0]*v.x+m[2]*v.y+m[4])/20.f,(m[1]*v.x+m[3]*v.y+m[5])/20.f,v.u,v.v});',
            *PORTRAIT_CORRECTION.splitlines(),
            '   result.batches.push_back(std::move(batch));',
            '  }out=std::move(result);error.clear();return true;',
            ' }catch(const std::bad_alloc&){error="Cannot allocate original HUD geometry";return false;}',
            '}',
            '} // namespace',
            'bool compose_original_hud(unsigned style,unsigned hp,unsigned mp,unsigned portrait,HudGeometry& out,std::string& error){',
            ' return compose_hud(style,hp,mp,0,portrait,false,out,error);',
            '}',
            'bool compose_original_hud(unsigned style,unsigned hp,unsigned mp,unsigned xp,unsigned portrait,HudGeometry& out,std::string& error){',
            ' return compose_hud(style,hp,mp,xp,portrait,true,out,error);',
            '}',
            'bool compose_original_target_hud(unsigned hp,HudTargetGeometry& out,std::string& error){',
            ' if(hp>99){error="Original target HP frame outside source domain";return false;}',
            ' try{HudTargetGeometry result;result.bounds=target_bounds;result.text_fields=target_text_fields;',
            '  for(int index:target_order){const Layer& layer=index==-1?target_frames[hp]:target_static[index];',
            '   const HudShapeGeometry* source=nullptr;for(const auto& shape:shapes)if(shape.shape_id==layer.shape){source=&shape;break;}',
            '   if(!source){error="Original target HUD shape missing";return false;}',
            '   HudGeometryBatch batch;batch.role=source->role;batch.shape_id=source->shape_id;',
            '   const auto& m=layer.matrix;for(const auto& v:source->triangles)batch.triangles.push_back({(m[0]*v.x+m[2]*v.y+m[4])/20.f,(m[1]*v.x+m[3]*v.y+m[5])/20.f,v.u,v.v});',
            '   result.art.batches.push_back(std::move(batch));',
            '  }out=std::move(result);error.clear();return true;',
            ' }catch(const std::bad_alloc&){error="Cannot allocate original target HUD";return false;}',
            '}',
            'bool layout_original_target_text(const HudTargetTextField& field,float advance,std::array<float,2>& out,std::string& error){',
            ' if(!std::isfinite(advance)||advance<0||!std::isfinite(field.source_height)||field.source_height<=0||field.align>2){error="Invalid original target text metrics";return false;}',
            ' for(float x:field.local_bounds)if(!std::isfinite(x)){error="Invalid original target text bounds";return false;}',
            ' for(float x:field.font_metrics)if(!std::isfinite(x)){error="Invalid original target font metrics";return false;}',
            ' for(float x:field.margins)if(!std::isfinite(x)){error="Invalid original target text margins";return false;}',
            ' for(float x:field.matrix)if(!std::isfinite(x)){error="Invalid original target text transform";return false;}',
            ' const auto& b=field.local_bounds;const float width=b[1]-b[0];if(width<=0||b[3]<=b[2]){error="Empty original target text bounds";return false;}',
            ' float x=std::max(0.f,field.margins[0]+field.margins[2])+b[0];',
            ' const float extra=(width-field.margins[1])-(x+advance)-4.f;',
            ' if(field.align==2)x+=extra*.5f;else if(field.align==1)x+=extra;',
            ' const float y=b[2]+field.source_height+(field.font_metrics[2]-field.font_metrics[1])*field.source_height/1024.f;',
            ' const auto& m=field.matrix;std::array<float,2> baseline{m[0]*x+m[2]*y+m[4],m[1]*x+m[3]*y+m[5]};',
            ' if(!std::isfinite(baseline[0])||!std::isfinite(baseline[1])){error="Original target baseline overflow";return false;}',
            ' out=baseline;error.clear();return true;',
            '}',
            'bool apply_original_target_font_layout(std::uint32_t font,float height,HudGlyphRun& output,std::string& error){',
            ' if(font!=7||!std::isfinite(height)||height<=0){error="Original target font layout unavailable";return false;}',
            ' if(output.source_layout_font){if(output.source_layout_font!=font||output.source_layout_height!=height){error="Glyph run already uses another source font layout";return false;}error.clear();return true;}',
            ' try{HudGlyphRun run=output;float old_pen=0,new_pen=0;unsigned previous=0xffffffffu;bool have_bounds=false;',
            '  for(auto& glyph:run.glyphs){if(!std::isfinite(glyph.advance)||glyph.advance<0){error="Invalid source glyph advance";return false;}',
            '   for(const auto& pair:font7_pairs)if(pair.first==previous&&pair.second==glyph.codepoint){new_pen+=pair.adjustment*height/1024.f;break;}',
            '   glyph.x+=new_pen-old_pen;const float old_advance=glyph.advance;',
            '   for(const auto& entry:font7_advances)if(entry.code==glyph.codepoint){glyph.advance=entry.advance*height/1024.f;break;}',
            '   old_pen+=old_advance;new_pen+=glyph.advance;previous=glyph.codepoint;',
            '   if(!std::isfinite(new_pen)||!std::isfinite(glyph.x)){error="Original text layout overflow";return false;}',
            '   if(glyph.width>0&&glyph.height>0){if(!have_bounds){run.bounds={glyph.x,glyph.x+glyph.width,glyph.y,glyph.y+glyph.height};have_bounds=true;}',
            '    else{run.bounds[0]=std::min(run.bounds[0],glyph.x);run.bounds[1]=std::max(run.bounds[1],glyph.x+glyph.width);run.bounds[2]=std::min(run.bounds[2],glyph.y);run.bounds[3]=std::max(run.bounds[3],glyph.y+glyph.height);}}',
            '  }run.advance=new_pen;run.source_layout_font=font;run.source_layout_height=height;output=std::move(run);error.clear();return true;',
            ' }catch(const std::bad_alloc&){error="Cannot allocate original font layout";return false;}',
            '}', '} // namespace dh::foundation']
    cpp_path = ROOT/"port/windows-foundation/hud_geometry.cpp"
    cpp_path.write_text("\n".join(cpp)+"\n",encoding="utf8")
    report_path = ROOT/"port/windows-foundation/reports/hud-source.json"
    report = json.loads(report_path.read_text(encoding="utf-8-sig"))
    # Text-metric evidence written by the native text workflow, not by this exporter; keep it on rewrite.
    previous_layout = report.get("geometry_export", {}).get("target", {}).get("plain_text_layout", {})
    report["geometry_export"] = {
        "source_sha256":hashlib.sha256(raw).hexdigest(),
        "tool":"port/windows-foundation/tools/export_hud_geometry.py",
        "cpp_sha256":hashlib.sha256(cpp_path.read_bytes()).hexdigest(),
        "curve_tolerance_twips":CURVE_TOLERANCE_TWIPS,
        "frame_convention":{"HP_MP":"Source native producer: percent-1, clamped to0..99; frame0empty,frame99full.",
                            "cover_semantics":"Shape88/145/150 are shrinking covers; shrinking exposes original red/blue/XP bar artwork.",
                            "XP":"InfoHUDManager::FastUpdate 0x41e064: GotoFrame(bar_xp,min(99,100*resolved[33]/resolved[34])); no -1, frame0 empty. Char152 has101 frames; frame100 is never selected.",
                            "portrait":"Source frames0Warrior,1Rogue,2Mage."},
        "shape_records":list(shapes.values()), "layouts":layouts,
        "target":{"source_clip":142,"source_frame":FRAME_OVERRIDES[142],"labels":labels[142],"hp_frames":target_frames,
                  "static_layers":target_static,"depth_order":target_order,
                  "text_fields":target_text,"bounds_pixels":target_bounds,
                  "fonts":fonts,
                  "plain_text_layout":{"baseline":"rect.yMin+textHeight+(fontLeading-fontDescent)*textHeight/(1024*20) for DefineFont3; then apply original field matrix.",
                                       "alignment":"align_line extra=(fieldWidth-rightMargin)-(initialX+runAdvance)-80twips; centreShift=extra/2.",
                                       "source":"port/engine-ui/renderfx_text_connection.cpp:format_plain and vendor/gameswf1714/gameswf/gameswf_text.cpp:align_line",
                                       "level_format":"hud_manager.cpp enemy uses integer %d; boss or unresolvedlevel uses ??; no Lv prefix."},
                  "anchor_evidence":"port/engine-ui/enemy_hud_presentation_v2.cpp: bottom-centre of complete bounds projected to target head.",
                  "limits":"Name/level glyph generation and actor data remain caller-owned; source typography metadata exported, no replacement name invented."},
        "target_markers":{"source":"port/level-world/reports/target-art-v28/target-set-rows.json",
                          "bindings":markers,"source_files":"port/level-world/reports/target-art-v28/assets.json",
                          "rendering":"Original 3D BDAE model artwork on actor anchor; self-illumination and scale_with_anchor are original effect-step flags.",
                          "limits":"Effect animation/anchor scaling are runtime work; no substitute procedural circle should claim original art."},
        "validation":"Closed single source contours; ear tessellation triangle area matches contour area; original atlas UV inside0..1; source color/mask guards.",
        "limits":["This export deliberately does not render text or action controls; the XP bar (char152, frames0..99) is driven by the compose_original_hud xp argument.",
                  "Source frame0 ancestor matrices exported; original AS viewport reflow and HUD activation animation remain external.",
                  "Masks/color transforms on selected art reject explicitly rather than approximating."]}
    for key in ("native_verification", "source_font_metrics"):
        if key in previous_layout:
            report["geometry_export"]["target"]["plain_text_layout"].setdefault(key, previous_layout[key])
    report_path.write_text(json.dumps(report,indent=2)+"\n",encoding="utf8")
    print(json.dumps({"shapes":len(shapes),"source_triangles":sum(len(s["triangles"])//3 for s in shapes.values()),
                      "layouts":4,"hp_frames":100,"mp_frames":100,"portraits":3}))


if __name__ == "__main__":
    main()
