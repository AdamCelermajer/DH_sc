"""Export the original campaign loading menu (menu_Loading in dqshared.swf) as static geometry.

Source: the retained original data/menus/dqshared.swf (the same movie the Android reconstruction
renders as _root.menu_Loading, see port/android-native/tests/test_authored_loading_screen_assets_v1.py).
Output: loading_art_data.cpp (stage coordinates in 1024x768 authored pixels; textures = MenuGraphics01..05).

Layers: bg (sprite126->shape8), frame (shape127), track (131), fill (134, clipped at runtime by the
per-frame Mask rect of loading_anim), spark (135, per-frame matrix/alpha). Text fields are exported as
authored rectangles; the runtime draws the text with the authored font.
Requires: python3 + shapely. Usage: py -3 export_loading_art.py [dqshared.swf]
"""
from pathlib import Path
import importlib.util, inspect, struct, zlib, sys
HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[4]
spec = importlib.util.spec_from_file_location('s', ROOT/'port/windows-foundation/tools/export_hud_geometry.py')
swf = importlib.util.module_from_spec(spec); spec.loader.exec_module(swf)
from shapely.geometry import Polygon
from shapely import constrained_delaunay_triangles


def tessellate_all(contours):
    if len(contours) == 1:
        return swf.triangulate(contours[0])
    polygon = Polygon()
    for c in contours:
        polygon = polygon.symmetric_difference(Polygon(c))
    out = []
    for t in constrained_delaunay_triangles(polygon).geoms:
        out.extend(list(t.exterior.coords)[:3])
    return out


src = Path(sys.argv[1]) if len(sys.argv) > 1 else ROOT/'.local-inputs/windows-source-clock-v19-preview-15/assets/original-cache/data/menus/dqshared.swf'
raw = src.read_bytes()
data = raw[:8]+zlib.decompress(raw[8:]) if raw[:3] == b'CWS' else raw
_, at = swf.rect(data, 8)
at += 4
bitmap_dims = {struct.unpack_from('<H', t)[0]: struct.unpack_from('<HH', t, 3) for c, t in swf.tags(data, at, len(data)) if c in (20, 36)}
exports = {}
for c, t in swf.tags(data, at, len(data)):
    if c == 56:
        n = struct.unpack_from('<H', t)[0]
        p = 2
        for _ in range(n):
            i = struct.unpack_from('<H', t, p)[0]
            e = t.index(0, p+2)
            exports[i] = t[p+2:e].decode()
            p = e+1


def styles_reader(d, a, ident, code=2):
    n = d[a]
    a += 1
    if n == 255:
        n = struct.unpack_from('<H', d, a)[0]
        a += 2
    out = []
    for _ in range(n):
        k = d[a]
        a += 1
        if k == 0:
            w = 4 if code in (32, 83) else 3
            rgba = list(d[a:a+w]) + ([255] if w == 3 else [])
            a += w
            out.append({'kind': 0, 'rgba': [x/255 for x in rgba]})
        elif k in (0x40, 0x41, 0x42, 0x43):
            b = struct.unpack_from('<H', d, a)[0]
            a += 2
            m, a = swf.matrix(d, a)
            out.append({'kind': k, 'bitmap_id': b, 'matrix_twips': m})
        else:
            raise ValueError(f'shape {ident}: unsupported fill {k}')
    if d[a]:
        raise ValueError(f'shape {ident}: line styles')
    return out, a+1


code_src = inspect.getsource(swf.parse_shape)
code_src = code_src.replace('/determinant/1024', '/determinant')
code_src = code_src.replace('            if not (0 <= u <= 1 and 0 <= v <= 1):\n                raise ValueError("Original contour outside supplied atlas")\n', '')
code_src = code_src.replace('vertices = triangulate(fill_contours[0])', 'vertices = tessellate_all(fill_contours)')
code_src = code_src.replace('vertices = [v for contour in fill_contours for v in triangulate(contour)]', 'vertices = tessellate_all(fill_contours)')
code_src = code_src.replace('triangles.append([x,y,u,v])', 'triangles.append([x,y,u,v,styles[fill-1]["bitmap_id"]])')
g = dict(swf.parse_shape.__globals__)
g.update(read_bitmap_styles=styles_reader, tessellate_all=tessellate_all)
exec(code_src, g)
parse_shape = g['parse_shape']
shapes, sprites, edits = {}, {}, {}
for c, t in swf.tags(data, at, len(data)):
    i = struct.unpack_from('<H', t)[0] if len(t) >= 2 else -1
    if c in (2, 22, 32) and i in (8, 127, 131, 132, 134, 135):
        shapes[i] = parse_shape(c, t)
    elif c == 39 and i == 137:
        sprites[i] = swf.parse_timeline(t, 4)
    elif c == 37 and i in (128, 129):
        r = swf.parse_edit_text(t)
        edits[i] = r
tex_index = {3: 5, 4: 4, 5: 3, 6: 2, 7: 1, 2: 6}   # SWF bitmap id -> texture index (1..5 MenuGraphicsNN, 6 MenusGraphics)
for b, n in [(k, v) for k, v in tex_index.items() if v <= 5]:
    assert exports[b] == f'menus/MenuGraphics{n:02d}.tga', (b, exports[b])


def place(shape_id, m):
    sh = shapes[shape_id]
    verts, solids = [], []
    for x, y, u, v, b in sh['triangles']:
        w, h = bitmap_dims[b]
        verts.append((b, (m[0]*x+m[2]*y+m[4])/20, (m[1]*x+m[3]*y+m[5])/20, u/w, v/h))
    for t in sh['solid_triangles']:
        solids.append(((m[0]*t[0]+m[2]*t[1]+m[4])/20, (m[1]*t[0]+m[3]*t[1]+m[5])/20, t[2:6]))
    return verts, solids


def mm(*ms):
    r = [1, 0, 0, 1, 0, 0]
    for m in ms:
        r = swf.multiply(r, m)
    return r


root = swf.parse_timeline(data, at)
menu = swf.placed_path(root, 'menu_Loading')
d141 = None
for c, t in swf.tags(data, at, len(data)):
    if c == 39 and struct.unpack_from('<H', t)[0] == 141:
        d141 = swf.parse_timeline(t, 4)[0]
mroot = menu['matrix']
bg_m = mm(mroot, d141[1]['matrix'], [1, 0, 0, 1, -709, -9854])
frame_m = mm(mroot, d141[9]['matrix'])
anim = mm(mroot, d141[15]['matrix'])
frames = sprites[137]
lines = ['// Generated by tools/export_loading_art.py from original data/menus/dqshared.swf (menu_Loading). Do not edit.',
         '#include "loading_art_v1.hpp"', 'namespace dh::foundation::startup {', 'namespace {']


def emit_layer(name, verts, solids):
    groups = {}
    for b, x, y, u, v in verts:
        groups.setdefault(tex_index[b], []).append((x, y, u, v))
    parts = []
    for tex, vs in sorted(groups.items()):
        parts.append('{%d,{1,1,1,1},{%s}}' % (tex, ','.join('{%.4ff,%.4ff,%.6ff,%.6ff}' % v for v in vs)))
    sg = {}
    for x, y, rgba in solids:
        sg.setdefault(tuple(rgba), []).append((x, y))
    for rgba, vs in sg.items():
        parts.append('{0,{%s},{%s}}' % (','.join('%.4ff' % c for c in rgba), ','.join('{%.4ff,%.4ff,0,0}' % v for v in vs)))
    lines.append('const Layer k%s[]{%s};' % (name, ','.join(parts)))
    return len(parts)


counts = {}
counts['Bg'] = emit_layer('Bg', *place(8, bg_m))
counts['Frame'] = emit_layer('Frame', *place(127, frame_m))
counts['Track'] = emit_layer('Track', *place(131, mm(anim, frames[0][1]['matrix'])))
counts['Fill'] = emit_layer('Fill', *place(134, mm(anim, frames[0][4]['matrix'])))
counts['Spark'] = emit_layer('Spark', *place(135, [1, 0, 0, 1, 0, 0]))
mb = shapes[132]['bounds_twips']
rows = []
for f in frames:
    mk = mm(anim, f[2]['matrix'])
    sp = mm(anim, f[7]['matrix'])
    a = f[7]['color'][0][3]
    corners = [(mk[0]*x+mk[2]*y+mk[4], mk[1]*x+mk[3]*y+mk[5]) for x in (mb[0], mb[1]) for y in (mb[2], mb[3])]
    rows.append('{%.3ff,%.3ff,%.3ff,%.3ff,{%s},%.3ff}' % (
        min(c[0] for c in corners)/20, max(c[0] for c in corners)/20,
        min(c[1] for c in corners)/20, max(c[1] for c in corners)/20,
        ','.join('%.5ff' % (sp[i] if i < 4 else sp[i]/20) for i in range(6)), a))
lines.append('const FrameInfo kFrames[101]{%s};' % ','.join(rows))


def field(i, parent):
    r = edits[i]
    b = r['bounds_twips']
    return '{%.2ff,%.2ff,%.2ff,%.2ff,%.2ff,{%d,%d,%d,%d}}' % (
        parent[4]/20+b[0]/20, parent[5]/20+b[2]/20, (b[1]-b[0])/20, (b[3]-b[2])/20, r['height_twips']/20, *r['rgba'])


loading_text_m = mm(mroot, d141[10]['matrix'])
tip_m = mm(mroot, d141[13]['matrix'], [1, 0, 0, 1, -2339, 2897])
lines.append('}  // namespace')
lines.append('const LoadingArt& loading_art() noexcept {')
lines.append('static const LoadingArt art{')
lines.append(','.join('{k%s,%d}' % (n, counts[n]) for n in ('Bg', 'Frame', 'Track', 'Fill', 'Spark')) + ',')
lines.append('kFrames, %s, %s, 1024.0f, 768.0f};' % (field(128, loading_text_m), field(129, tip_m)))
lines.append('return art; }')
lines.append('}  // namespace dh::foundation::startup')
(HERE.parent/'loading_art_data.cpp').write_text('\n'.join(lines)+'\n', encoding='utf8')
print(counts, [(k, len(v['triangles']), len(v['solid_triangles'])) for k, v in shapes.items()])
