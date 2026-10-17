#!/usr/bin/env python3
"""Host checks for bounded SWAMP floor extraction and floor-height queries."""
import argparse
import ctypes as c
import hashlib
import importlib.util
import json
import math
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
U = c.c_uint32
P = c.c_void_p


class Surface(c.Structure):
    _fields_ = [(name, U) for name in (
        'module_index', 'module_source_record', 'node_record', 'geometry_index',
        'visible', 'first_triangle', 'triangle_count', 'vertex_count',
        'primitive_count')] + [
        ('floor_type_tag_present', c.c_bool), ('floor_type_tag', c.c_char*256),
        ('floor_type_flags', U), ('floor_type_flags_known', c.c_bool),
        ('module_name', c.c_char*128), ('source_node_id', c.c_char*128),
        ('source_node_name', c.c_char*128), ('source_geometry_id', c.c_char*128),
        ('source_geometry_name', c.c_char*128)]


class SurfaceTriangle(c.Structure):
    _fields_ = [('a', c.c_float*3), ('b', c.c_float*3), ('c', c.c_float*3)] + [
        (name, U) for name in ('surface_index', 'primitive_index', 'source_triangle_index')]


class Navigation(c.Structure):
    _fields_ = [('surfaces', c.POINTER(Surface))] + [(name, U) for name in
        ('surface_count', 'surface_capacity')] + [('triangles', c.POINTER(SurfaceTriangle))] + [
        (name, U) for name in ('triangle_count', 'triangle_capacity')]


class Hit(c.Structure):
    _fields_ = [(name, U) for name in ('surface_index', 'primitive_index', 'source_triangle_index')]
    _fields_ += [(name, c.c_float) for name in ('height', 'vertical_distance')]
    _fields_ += [('barycentric', c.c_float*3), ('floor_type_flags_known', c.c_bool),
                 ('floor_type_flags', U), ('floor_type_tag_present', c.c_bool),
                 ('floor_type_tag', c.c_char*256)]


class SegmentHit(c.Structure):
    _fields_ = [(name, U) for name in
        ('surface_index', 'primitive_index', 'source_triangle_index')]
    _fields_ += [('fraction', c.c_float), ('position', c.c_float*3),
                 ('floor_type_flags_known', c.c_bool), ('floor_type_flags', U),
                 ('floor_type_tag_present', c.c_bool), ('floor_type_tag', c.c_char*256)]


class Diagnostic(c.Structure):
    _fields_ = [('error', U), ('message', c.c_char*160)]


def load_world_bindings(library):
    path = ROOT.parent/'world-data/tests/check_world.py'
    spec = importlib.util.spec_from_file_location('navigation_world_checks', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module, module.bind(library)


def _check_synthetic(dll):
    surfaces = (Surface*1)()
    triangles = (SurfaceTriangle*1)()
    surfaces[0].floor_type_tag_present = True
    surfaces[0].floor_type_tag = b'water'
    surfaces[0].floor_type_flags = 2
    surfaces[0].floor_type_flags_known = True
    triangles[0].a[:] = (0.0, 0.0, 0.0)
    triangles[0].b[:] = (2.0, 0.0, 2.0)
    triangles[0].c[:] = (0.0, 2.0, 4.0)
    triangles[0].surface_index = 0
    nav = Navigation(surfaces, 1, 1, triangles, 1, 1)
    query = dll.dh2_nav_query_height
    query.restype = U
    query.argtypes = [c.POINTER(Navigation), c.c_float, c.c_float, c.c_float,
        c.c_float, c.c_float, c.POINTER(Hit), c.POINTER(c.c_bool)]
    result = Hit()
    found = c.c_bool()
    # Plane is z=x+2y.
    assert query(c.byref(nav), 0.5, 0.5, 1.5, 0.01, 1e-6,
                 c.byref(result), c.byref(found)) == 0 and found.value
    assert math.isclose(result.height, 1.5, abs_tol=1e-6)
    assert result.floor_type_flags_known and result.floor_type_flags == 2
    assert result.floor_type_tag_present and result.floor_type_tag == b'water'
    surfaces[0].floor_type_tag = b'void_wall'
    surfaces[0].floor_type_flags = 0x03000000
    assert query(c.byref(nav), 0.5, 0.5, 1.5, 0.01, 1e-6,
                 c.byref(result), c.byref(found)) == 0 and found.value
    assert result.floor_type_flags == 0x01000000 | 0x02000000
    assert result.floor_type_tag_present and result.floor_type_tag == b'void_wall'
    assert query(c.byref(nav), 0.5, 0.5, 1.7, 0.1, 1e-6,
                 c.byref(result), c.byref(found)) == 0 and not found.value
    # An exact edge point remains part of the triangle.
    assert query(c.byref(nav), 1.0, 0.0, 1.0, 0.01, 1e-6,
                 c.byref(result), c.byref(found)) == 0 and found.value
    assert math.isclose(result.height, 1.0, abs_tol=1e-6)
    # Outside the projected triangle does not extrapolate the plane.
    assert query(c.byref(nav), 1.5, 1.5, 4.5, 10, 1e-6,
                 c.byref(result), c.byref(found)) == 0 and not found.value
    # A point outside by more than the explicit barycentric tolerance is out.
    assert query(c.byref(nav), 1.001, 1.001, 3.003, 10, 1e-4,
                 c.byref(result), c.byref(found)) == 0 and not found.value
    # Projected-vertical triangles cannot supply a height sample.
    triangles[0].a[:] = (1.0, 0.0, 0.0)
    triangles[0].b[:] = (1.0, 1.0, 0.0)
    triangles[0].c[:] = (1.0, 0.0, 1.0)
    assert query(c.byref(nav), 1.0, 0.25, 0.0, 1, 1e-6,
                 c.byref(result), c.byref(found)) == 0 and not found.value
    assert query(c.byref(nav), 1, 1, 1, 1, 0.3,
                 c.byref(result), c.byref(found)) == 1

    # Six fully overlapping triangles exercise actor eligibility separately
    # from the unrestricted geometric height query. Only Z and metadata vary.
    actor_surfaces = (Surface*6)()
    actor_triangles = (SurfaceTriangle*6)()
    tags = (b'hole', b'water', b'hole_water', b'', b'void_water', b'wall')
    masks = (1, 2, 3, 0, 0x01000002, 0x02000000)
    heights = (0.0, 1.0, 2.0, 3.0, 0.1, 0.2)
    for i, (tag, mask, height) in enumerate(zip(tags, masks, heights)):
        actor_surfaces[i].floor_type_flags_known = True
        actor_surfaces[i].floor_type_flags = mask
        actor_surfaces[i].floor_type_tag_present = bool(tag)
        actor_surfaces[i].floor_type_tag = tag
        actor_triangles[i].a[:] = (0.0, 0.0, height)
        actor_triangles[i].b[:] = (2.0, 0.0, height)
        actor_triangles[i].c[:] = (0.0, 2.0, height)
        actor_triangles[i].surface_index = i
    actor_nav = Navigation(actor_surfaces, 6, 6, actor_triangles, 6, 6)
    actor_query = dll.dh2_nav_query_actor_floor
    actor_query.restype = U
    actor_query.argtypes = [c.POINTER(Navigation), c.c_float, c.c_float,
        c.c_float, c.c_float, c.c_float, U, c.POINTER(Hit), c.POINTER(c.c_bool)]
    x, y, edge = 0.25, 0.25, 1e-6

    # The legacy geometric API is untouched and still picks the closest hole.
    assert query(c.byref(actor_nav), x, y, 0.0, 4.0, edge,
                 c.byref(result), c.byref(found)) == 0 and found.value
    assert result.floor_type_tag == b'hole' and result.height == 0.0

    # Zero requirements pass even when the object's capability mask is zero.
    assert actor_query(c.byref(actor_nav), x, y, 0.0, 4.0, edge, 0,
                       c.byref(result), c.byref(found)) == 0 and found.value
    assert result.floor_type_flags == 0 and not result.floor_type_tag_present
    assert result.height == 3.0

    # With only water capability, the closer hole and the both-bit floor fail;
    # water is selected ahead of the unrestricted zero-requirement floor.
    assert actor_query(c.byref(actor_nav), x, y, 0.0, 4.0, edge, 2,
                       c.byref(result), c.byref(found)) == 0 and found.value
    assert result.floor_type_tag == b'water' and result.floor_type_flags == 2
    # A narrow band proves the hole is rejected rather than merely outranked.
    assert actor_query(c.byref(actor_nav), x, y, 0.0, 0.05, edge, 2,
                       c.byref(result), c.byref(found)) == 0 and not found.value

    # Both low capability bits satisfy a floor requiring both.
    assert actor_query(c.byref(actor_nav), x, y, 2.0, 0.05, edge, 3,
                       c.byref(result), c.byref(found)) == 0 and found.value
    assert result.floor_type_tag == b'hole_water' and result.floor_type_flags == 3

    # Native Void/Wall category bits reject floors even when the actor mask
    # includes those same bits and the floor's low path requirements.
    all_bits = 0x03000002
    assert actor_query(c.byref(actor_nav), x, y, 0.1, 0.01, edge, all_bits,
                       c.byref(result), c.byref(found)) == 0 and not found.value
    assert actor_query(c.byref(actor_nav), x, y, 0.2, 0.01, edge, all_bits,
                       c.byref(result), c.byref(found)) == 0 and not found.value
    # In a wider band, ordinary water can still win; high-bit surfaces cannot.
    assert actor_query(c.byref(actor_nav), x, y, 0.1, 2.0, edge, all_bits,
                       c.byref(result), c.byref(found)) == 0 and found.value
    assert result.floor_type_tag == b'water'
    checks = 22
    segment_query = dll.dh2_nav_query_segment
    segment_query.restype = U
    segment_query.argtypes = [c.POINTER(Navigation), c.POINTER(c.c_float),
        c.POINTER(c.c_float), c.c_bool, c.POINTER(SegmentHit), c.POINTER(c.c_bool)]

    def segment_call(nav, start, end, include_all=False):
        start = (c.c_float*3)(*start)
        end = (c.c_float*3)(*end)
        hit = SegmentHit()
        found = c.c_bool()
        status = segment_query(c.byref(nav), start, end, include_all,
                               c.byref(hit), c.byref(found))
        return status, found.value, hit

    # The segment is finite and includes both endpoints.
    plane_surface = (Surface*1)()
    plane_triangle = (SurfaceTriangle*1)()
    plane_surface[0].floor_type_flags_known = True
    plane_surface[0].triangle_count = 1
    plane_triangle[0].a[:] = (0, 0, 0)
    plane_triangle[0].b[:] = (2, 0, 0)
    plane_triangle[0].c[:] = (0, 2, 0)
    plane_nav = Navigation(plane_surface, 1, 1, plane_triangle, 1, 1)
    status, found_value, segment_hit = segment_call(plane_nav, (0.5, 0.5, 1),
                                                    (0.5, 0.5, 0))
    assert status == 0 and found_value and segment_hit.fraction == 1.0, (
        status, found_value, segment_hit.fraction, tuple(segment_hit.position))
    assert tuple(segment_hit.position) == (0.5, 0.5, 0.0)
    checks += 1
    status, found_value, segment_hit = segment_call(plane_nav, (0.5, 0.5, 0),
                                                    (0.5, 0.5, 1))
    assert status == 0 and found_value and segment_hit.fraction == 0.0
    checks += 1
    # Parallel/coplanar sweeps have no unique plane intersection.
    status, found_value, _ = segment_call(plane_nav, (0.25, 0.25, 1),
                                           (1.0, 0.25, 1))
    assert status == 0 and not found_value
    checks += 1
    # Projected-vertical triangles remain valid 3D collision surfaces.
    vertical_surface = (Surface*1)()
    vertical_triangle = (SurfaceTriangle*1)()
    vertical_surface[0].triangle_count = 1
    vertical_surface[0].floor_type_flags_known = True
    vertical_triangle[0].a[:] = (1, 0, 0)
    vertical_triangle[0].b[:] = (1, 2, 0)
    vertical_triangle[0].c[:] = (1, 0, 2)
    vertical_triangle[0].surface_index = 0
    vertical_nav = Navigation(vertical_surface, 1, 1, vertical_triangle, 1, 1)
    status, found_value, segment_hit = segment_call(vertical_nav, (0, 0.25, 0.25),
                                                    (2, 0.25, 0.25))
    assert status == 0 and found_value and segment_hit.fraction == 0.5
    assert tuple(segment_hit.position) == (1.0, 0.25, 0.25)
    checks += 1
    status, found_value, _ = segment_call(plane_nav, (0.25, 0.25, 0),
                                           (1.0, 0.25, 0))
    assert status == 0 and not found_value
    checks += 1
    # A zero-radius point on the triangle edge is accepted.
    status, found_value, segment_hit = segment_call(plane_nav, (1, 0, 1), (1, 0, 0))
    assert status == 0 and found_value and tuple(segment_hit.position) == (1.0, 0.0, 0.0)
    checks += 1
    status, found_value, _ = segment_call(plane_nav, (0.5, 0.5, -1), (0.5, 0.5, -2))
    assert status == 0 and not found_value  # Intersection exists only before start.
    checks += 1

    # Source surface order dominates global distance; within that first eligible
    # surface, choose its nearest intersection and keep source triangle ties.
    order_surfaces = (Surface*3)()
    order_triangles = (SurfaceTriangle*4)()
    order_surfaces[0].first_triangle = 0
    order_surfaces[0].triangle_count = 1
    order_surfaces[0].floor_type_flags_known = True
    order_surfaces[0].floor_type_flags = 0x01000000  # native void category
    order_surfaces[1].first_triangle = 1
    order_surfaces[1].triangle_count = 2
    order_surfaces[1].floor_type_flags_known = True
    order_surfaces[1].floor_type_flags = 2
    order_surfaces[1].floor_type_tag_present = True
    order_surfaces[1].floor_type_tag = b'water'
    order_surfaces[2].first_triangle = 3
    order_surfaces[2].triangle_count = 1
    order_surfaces[2].floor_type_flags_known = True
    order_surfaces[2].floor_type_flags = 0
    z_values = (1.0, -1.0, -0.5, 0.5)
    surface_values = (0, 1, 1, 2)
    for i, (z, surface_index) in enumerate(zip(z_values, surface_values)):
        order_triangles[i].a[:] = (0, 0, z)
        order_triangles[i].b[:] = (2, 0, z)
        order_triangles[i].c[:] = (0, 2, z)
        order_triangles[i].surface_index = surface_index
        order_triangles[i].source_triangle_index = i
    order_nav = Navigation(order_surfaces, 3, 3, order_triangles, 4, 4)
    status, found_value, segment_hit = segment_call(order_nav, (0.25, 0.25, 2),
                                                    (0.25, 0.25, -2))
    assert status == 0 and found_value and segment_hit.surface_index == 1
    assert segment_hit.source_triangle_index == 2
    assert math.isclose(segment_hit.fraction, 0.625, abs_tol=1e-6)
    assert segment_hit.floor_type_tag == b'water'
    checks += 1
    # include_all=false skips void; true restores native source order.
    status, found_value, segment_hit = segment_call(order_nav, (0.25, 0.25, 2),
                                                    (0.25, 0.25, -2), True)
    assert status == 0 and found_value and segment_hit.surface_index == 0
    assert segment_hit.fraction == 0.25
    checks += 1
    for category in (0x02000000, 0x03000000):
        order_surfaces[0].floor_type_flags = category
        status, found_value, segment_hit = segment_call(order_nav, (0.25, 0.25, 2),
                                                        (0.25, 0.25, -2))
        assert status == 0 and found_value and segment_hit.surface_index == 1
        checks += 1
    # A floor whose flags cannot be classified is not eligible in filtered mode.
    order_surfaces[0].floor_type_flags_known = False
    order_surfaces[0].floor_type_flags = 0
    status, found_value, segment_hit = segment_call(order_nav, (0.25, 0.25, 2),
                                                    (0.25, 0.25, -2))
    assert status == 0 and found_value and segment_hit.surface_index == 1
    checks += 1
    # Surface triangle spans are validated before access, and length is bounded.
    order_surfaces[1].first_triangle = 4
    status, found_value, _ = segment_call(order_nav, (0, 0, 0), (0, 0, 1))
    assert status == 9 and not found_value
    order_surfaces[1].first_triangle = 1
    status, found_value, _ = segment_call(order_nav, (0, 0, 0), (1000001, 0, 0))
    assert status == 1 and not found_value
    checks += 2
    return checks


def _check_cache(dll, world, cache):
    from collections import Counter
    source = cache/'data/scene/001_swamp.mlx'
    catalogue = cache/'data/3d/modules/swamp/swamp.bdae'
    level_bytes = source.read_bytes()
    catalogue_bytes = catalogue.read_bytes()
    level = world.SourceLevel()
    diagnostic = world.Diagnostic()
    assert dll.dh2_world_import_level(c.byref(level), b'SWAMP',
        b'data/scene/001_swamp.mlx', level_bytes, len(level_bytes),
        c.byref(diagnostic)) == 0, diagnostic.message
    catalogue_storage = c.create_string_buffer(catalogue_bytes)
    bres, scene = world.scene.Bres(), world.scene.Scene()
    assert dll.dh2_bres_open(c.byref(bres), catalogue_storage, len(catalogue_bytes)) == 0
    assert dll.dh2_scene_open(c.byref(scene), c.byref(bres)) == 0
    nav = Navigation()
    nav_diagnostic = Diagnostic()
    try:
        result = dll.dh2_nav_build_swamp(c.byref(nav), c.byref(level), c.byref(scene),
                                         c.byref(nav_diagnostic))
        assert result == 0, nav_diagnostic.message
        assert nav.surface_count == 16
        assert nav.triangle_count == 626
        assert nav.surface_count <= 256 and nav.triangle_count <= 100000
        modules = Counter()
        floor_nodes = set()
        for i in range(nav.surface_count):
            surface = nav.surfaces[i]
            assert surface.floor_type_flags_known is True
            assert b'floor' in surface.source_node_name.lower()
            assert surface.source_node_id and surface.source_geometry_id
            assert surface.first_triangle + surface.triangle_count <= nav.triangle_count
            modules[surface.module_index] += surface.triangle_count
            floor_nodes.add(surface.source_node_id.decode())
        seen_tags = {}
        surface_floor_types = []
        category_void = 0x01000000
        category_wall = 0x02000000
        path_hole = 1
        path_water = 2
        for surface in nav.surfaces[:nav.surface_count]:
            tag = surface.floor_type_tag.decode()
            token_source = tag if surface.floor_type_tag_present else surface.source_node_name.decode()
            expected = 0
            if 'void' in token_source: expected |= category_void
            if 'wall' in token_source: expected |= category_wall
            if 'hole' in token_source: expected |= path_hole
            if 'water' in token_source: expected |= path_water
            assert surface.floor_type_flags == expected, (token_source, surface.floor_type_flags, expected)
            if surface.floor_type_tag_present:
                seen_tags[tag] = surface.floor_type_flags
            surface_floor_types.append({
                'node': surface.source_node_id.decode(),
                'tag': tag,
                'tag_present': bool(surface.floor_type_tag_present),
                'flags': int(surface.floor_type_flags),
            })
        for tag, flags in (('hole', path_hole), ('water', path_water),
                           ('wood', 0), ('door', 0)):
            assert seen_tags.get(tag) == flags, (tag, seen_tags)
        for triangle in nav.triangles[:nav.triangle_count]:
            assert triangle.surface_index < nav.surface_count
        assert set(modules) == set(range(9))
        assert '_floor_obj_4of4_brdwalk_sw_-node' in floor_nodes
        assert '_floor_water_obj_4of4_brdwalk_sw_-node' in floor_nodes
        # Source MLX entry zero is (1090.75,-212.202,258). The actual source
        # floor mesh under module 0 gives Z=255, leaving the recorded 3-unit gap.
        hit = Hit()
        found = c.c_bool()
        query = dll.dh2_nav_query_height
        query.restype = U
        query.argtypes = [c.POINTER(Navigation), c.c_float, c.c_float,
            c.c_float, c.c_float, c.c_float, c.POINTER(Hit), c.POINTER(c.c_bool)]
        assert query(c.byref(nav), 1090.75, -212.202, 258.0, 10.0, 1e-6,
                     c.byref(hit), c.byref(found)) == 0 and found.value
        assert math.isclose(hit.height, 255.0, abs_tol=1e-4)
        assert math.isclose(hit.vertical_distance, 3.0, abs_tol=1e-4)
        entry_height = hit.height
        surface = nav.surfaces[hit.surface_index]
        assert surface.module_index == 0
        assert surface.source_node_id.decode() == '_floor_obj_4of4_brdwalk_sw_-node'
        assert surface.source_geometry_id.decode() == '_floor_obj_4of4_brdwalk_sw_-mesh'
        assert hit.floor_type_flags == surface.floor_type_flags
        assert hit.floor_type_flags_known == surface.floor_type_flags_known
        assert hit.floor_type_tag_present == surface.floor_type_tag_present
        assert hit.floor_type_tag == surface.floor_type_tag
        segment_start = (c.c_float*3)(1090.75, -212.202, 258.0)
        segment_end = (c.c_float*3)(1090.75, -212.202, 250.0)
        segment_hit = SegmentHit()
        segment_found = c.c_bool()
        segment_query = dll.dh2_nav_query_segment
        assert segment_query(c.byref(nav), segment_start, segment_end, False,
            c.byref(segment_hit), c.byref(segment_found)) == 0 and segment_found.value
        assert segment_hit.surface_index == hit.surface_index
        assert segment_hit.primitive_index == hit.primitive_index
        assert math.isclose(segment_hit.fraction, 0.375, abs_tol=1e-5)
        assert all(math.isclose(value, expected, abs_tol=1e-4)
                   for value, expected in zip(segment_hit.position,
                                               (1090.75, -212.202, 255.0)))
        # A point beyond all module floors remains unsupported.
        assert query(c.byref(nav), 5000.0, 0.0, 255.0, 1000.0, 1e-6,
                     c.byref(hit), c.byref(found)) == 0 and not found.value
        # Invalid barycentric tolerance is rejected.
        assert query(c.byref(nav), 0.0, 0.0, 0.0, 1.0, 0.3,
                     c.byref(hit), c.byref(found)) == 1
        source_hashes = {str(p.relative_to(cache)).replace('\\', '/'): hashlib.sha256(p.read_bytes()).hexdigest()
                         for p in (source, catalogue)}
        return {
            'module_count': level.module_count,
            'surface_count': nav.surface_count,
            'triangle_count': nav.triangle_count,
            'triangles_by_module': dict(sorted(modules.items())),
            'source_floor_node_ids': sorted(floor_nodes),
            'surface_floor_types': surface_floor_types,
            'entrypoint_query': {
                'point_xy': [1090.75, -212.202], 'entry_z': 258.0,
                'surface_height_z': entry_height,
                'surface_id': '_floor_obj_4of4_brdwalk_sw_-node',
                'clearance': 3.0,
            },
            'entrypoint_segment_query': {
                'start': [1090.75, -212.202, 258.0],
                'end': [1090.75, -212.202, 250.0],
                'surface_id': '_floor_obj_4of4_brdwalk_sw_-node',
                'fraction': float(segment_hit.fraction),
                'position': list(segment_hit.position),
            },
            'source_sha256': source_hashes,
        }
    finally:
        dll.dh2_nav_free(c.byref(nav))
        dll.dh2_world_free(c.byref(level))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--library', type=Path, required=True)
    parser.add_argument('--cache', type=Path, required=True)
    parser.add_argument('--original', type=Path,
        default=ROOT.parents[2] / 'standalone-build/compatibility/work/decoded-original'
                / 'lib/armeabi-v7a/libDungeonHunter2.so')
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    world, dll = load_world_bindings(args.library)
    dll.dh2_nav_build_swamp.restype = U
    dll.dh2_nav_build_swamp.argtypes = [c.POINTER(Navigation), c.POINTER(world.SourceLevel),
        c.POINTER(world.scene.Scene), c.POINTER(Diagnostic)]
    dll.dh2_nav_free.restype = None
    dll.dh2_nav_free.argtypes = [c.POINTER(Navigation)]
    dll.dh2_nav_query_height.restype = U
    dll.dh2_nav_query_height.argtypes = [c.POINTER(Navigation), c.c_float,
        c.c_float, c.c_float, c.c_float, c.c_float, c.POINTER(Hit), c.POINTER(c.c_bool)]
    dll.dh2_nav_query_actor_floor.restype = U
    dll.dh2_nav_query_actor_floor.argtypes = [c.POINTER(Navigation), c.c_float,
        c.c_float, c.c_float, c.c_float, c.c_float, U, c.POINTER(Hit),
        c.POINTER(c.c_bool)]
    dll.dh2_nav_query_segment.restype = U
    dll.dh2_nav_query_segment.argtypes = [c.POINTER(Navigation), c.POINTER(c.c_float),
        c.POINTER(c.c_float), c.c_bool, c.POINTER(SegmentHit), c.POINTER(c.c_bool)]
    synthetic = _check_synthetic(dll)
    cache_report = _check_cache(dll, world, args.cache)
    arm_script = ROOT/'tests/check_triangle_arm.py'
    arm_result = subprocess.run([sys.executable, str(arm_script),
        '--original', str(args.original), '--library', str(args.library)],
        check=True, capture_output=True, text=True)
    arm_report = json.loads(arm_result.stdout)
    report = {'pass': True, 'complete_navigation': False,
              'scope': 'source-selected SWAMP floor geometry; height sampling, floor eligibility, and bounded zero-radius segment-vs-triangle query; no actor movement integration',
              'synthetic_checks': synthetic, 'original_cache': cache_report,
              'original_arm32_triangle_differential': arm_report,
              'host_library_sha256': hashlib.sha256(args.library.read_bytes()).hexdigest()}
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
