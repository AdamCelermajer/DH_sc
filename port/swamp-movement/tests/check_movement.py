#!/usr/bin/env python3
"""Exercise the module-zero movement slice against source SWAMP geometry."""
import argparse
import ctypes as c
import importlib.util
import json
import math
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
U = c.c_uint32


class Input(c.Structure):
    _fields_ = [('position', c.c_float*3), ('heading_radians', c.c_float),
                ('stick_x', c.c_float), ('stick_y', c.c_float),
                ('dt_seconds', c.c_float)]


class Output(c.Structure):
    _fields_ = [('position', c.c_float*3), ('heading_radians', c.c_float),
                ('dt_used_seconds', c.c_float), ('floor_height', c.c_float),
                ('floor_surface_index', U), ('moved', c.c_bool),
                ('floor_sample_valid', c.c_bool)]


def load_navigation_bindings(library):
    path = ROOT.parent/'navigation/tests/check_navigation.py'
    spec = importlib.util.spec_from_file_location('movement_navigation_checks', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module, module.load_world_bindings(library)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--library', type=Path, required=True)
    parser.add_argument('--cache', type=Path, required=True)
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    navigation_checks, (world, dll) = load_navigation_bindings(args.library)
    dll.dh2_nav_build_swamp.restype = U
    dll.dh2_nav_build_swamp.argtypes = [
        c.POINTER(navigation_checks.Navigation), c.POINTER(world.SourceLevel),
        c.POINTER(world.scene.Scene), c.POINTER(navigation_checks.Diagnostic)]
    dll.dh2_nav_free.restype = None
    dll.dh2_nav_free.argtypes = [c.POINTER(navigation_checks.Navigation)]
    dll.dh2_nav_query_actor_floor.restype = U
    dll.dh2_nav_query_actor_floor.argtypes = [
        c.POINTER(navigation_checks.Navigation), c.c_float, c.c_float,
        c.c_float, c.c_float, c.c_float, U,
        c.POINTER(navigation_checks.Hit), c.POINTER(c.c_bool)]
    dll.dh2_swamp_movement_step.restype = U
    dll.dh2_swamp_movement_step.argtypes = [
        c.POINTER(navigation_checks.Navigation), c.POINTER(Input), c.c_float,
        c.POINTER(Output)]

    level_path = args.cache/'data/scene/001_swamp.mlx'
    catalogue_path = args.cache/'data/3d/modules/swamp/swamp.bdae'
    level_bytes = level_path.read_bytes()
    catalogue_bytes = catalogue_path.read_bytes()
    level = world.SourceLevel()
    world_diagnostic = world.Diagnostic()
    assert dll.dh2_world_import_level(c.byref(level), b'SWAMP',
        b'data/scene/001_swamp.mlx', level_bytes, len(level_bytes),
        c.byref(world_diagnostic)) == 0, world_diagnostic.message
    catalogue_storage = c.create_string_buffer(catalogue_bytes)
    bres, scene = world.scene.Bres(), world.scene.Scene()
    assert dll.dh2_bres_open(c.byref(bres), catalogue_storage,
                              len(catalogue_bytes)) == 0
    assert dll.dh2_scene_open(c.byref(scene), c.byref(bres)) == 0
    nav = navigation_checks.Navigation()
    diagnostic = navigation_checks.Diagnostic()
    assert dll.dh2_nav_build_swamp(c.byref(nav), c.byref(level), c.byref(scene),
        c.byref(diagnostic)) == 0, diagnostic.message

    speed = 10.0
    def step(position, heading, sx, sy, dt, walk_speed=speed):
        source = Input((c.c_float*3)(*position), heading, sx, sy, dt)
        result = Output()
        code = dll.dh2_swamp_movement_step(c.byref(nav), c.byref(source),
                                            walk_speed, c.byref(result))
        return code, result

    def assert_position(actual, expected):
        assert all(math.isclose(float(a), float(e), rel_tol=0, abs_tol=1.0e-5)
                   for a, e in zip(actual, expected))

    def actor_floor(x, y, z, path_mask, max_distance=100.0):
        hit = navigation_checks.Hit()
        found = c.c_bool()
        code = dll.dh2_nav_query_actor_floor(c.byref(nav), x, y, z,
            max_distance, 1e-6, path_mask, c.byref(hit), c.byref(found))
        assert code == 0
        return hit, found.value

    def triangle_centroid(triangle):
        return tuple((float(triangle.a[i]) + float(triangle.b[i]) +
                      float(triangle.c[i])) / 3.0 for i in range(3))

    start = (1090.75, -212.202, 258.0)
    right_status, right = step(start, 0.3, 1, 0, 0.1)
    forward_status, forward = step(start, 0.3, 0, 1, 0.1)
    assert right_status == 0 and right.moved and right.floor_sample_valid
    assert forward_status == 0 and forward.moved and forward.floor_sample_valid
    assert math.isclose(right.position[0], start[0]+1.0, abs_tol=1e-4)
    assert math.isclose(right.position[1], start[1], abs_tol=1e-5)
    assert math.isclose(forward.position[0], start[0], abs_tol=1e-5)
    assert math.isclose(forward.position[1], start[1]+1.0, abs_tol=1e-4)
    assert math.isclose(right.position[2], right.floor_height, abs_tol=1e-5)
    assert math.isclose(forward.position[2], forward.floor_height, abs_tol=1e-5)
    assert math.isclose(right.position[2], 255.0, abs_tol=1e-4)
    assert math.isclose(forward.position[2], 255.0, abs_tol=1e-4)
    assert math.isclose(right.heading_radians, -math.pi/2, abs_tol=1e-6)
    assert math.isclose(forward.heading_radians, 0, abs_tol=1e-6)
    assert nav.surfaces[right.floor_surface_index].module_index == 0
    assert nav.surfaces[forward.floor_surface_index].module_index == 0

    # The constructor baseline path mask (2) can path on a real module-zero
    # water surface. Exercise the same capability query through movement.
    water_sample = None
    for surface in nav.surfaces[:nav.surface_count]:
        if surface.module_index != 0 or surface.floor_type_flags != 2:
            continue
        for triangle_index in range(surface.first_triangle,
                                    surface.first_triangle+surface.triangle_count):
            candidate = triangle_centroid(nav.triangles[triangle_index])
            hit, found = actor_floor(*candidate, 2, 0.01)
            if found and hit.floor_type_flags == 2:
                water_sample = candidate
                break
        if water_sample:
            break
    assert water_sample is not None, 'module-zero source water must pass baseline mask 2'
    water_step_status, water_step = step(water_sample, 0, 1, 0, 0.1,
                                         walk_speed=0.01)
    assert water_step_status == 0 and water_step.floor_sample_valid
    assert nav.surfaces[water_step.floor_surface_index].floor_type_flags == 2

    # A boss-room hole requires path mask bit 1. Mask 2 must reject it; mask 1
    # is the positive control proving the sampled source floor is a hole.
    hole_sample = None
    for surface in nav.surfaces[:nav.surface_count]:
        if surface.module_index != 7 or surface.floor_type_flags != 1:
            continue
        for triangle_index in range(surface.first_triangle,
                                    surface.first_triangle+surface.triangle_count):
            candidate = triangle_centroid(nav.triangles[triangle_index])
            pass_hit, pass_found = actor_floor(*candidate, 1, 0.01)
            if not pass_found or pass_hit.floor_type_flags != 1:
                continue
            reject_hit, reject_found = actor_floor(*candidate, 2, 0.01)
            assert not reject_found or reject_hit.floor_type_flags != 1
            hole_sample = candidate
            break
        if hole_sample:
            break
    assert hole_sample is not None, 'source boss-room hole must be excluded by baseline mask 2'

    # The cache's holes are in module 7, while this movement experiment is
    # restricted to module 0. Use a local adjacent-floor fixture to verify the
    # movement-level reject/rollback path without claiming those modules join.
    fixture_surfaces = (navigation_checks.Surface*4)()
    fixture_triangles = (navigation_checks.SurfaceTriangle*4)()
    fixture_vertices = (
        ((0, 0, 0), (1, 0, 0), (1, 1, 0)),
        ((0, 0, 0), (1, 1, 0), (0, 1, 0)),
        ((1, 0, 0), (2, 0, 0), (2, 1, 0)),
        ((1, 0, 0), (2, 1, 0), (1, 1, 0)),
    )
    for index in range(4):
        fixture_surfaces[index].module_index = 0
        fixture_surfaces[index].floor_type_flags_known = True
        fixture_surfaces[index].floor_type_flags = 0 if index < 2 else 1
        fixture_surfaces[index].first_triangle = index
        fixture_surfaces[index].triangle_count = 1
        triangle = fixture_triangles[index]
        triangle.a[:] = fixture_vertices[index][0]
        triangle.b[:] = fixture_vertices[index][1]
        triangle.c[:] = fixture_vertices[index][2]
        triangle.surface_index = index
    fixture_nav = navigation_checks.Navigation(
        fixture_surfaces, 4, 4, fixture_triangles, 4, 4)
    fixture_start = (0.5, 0.5, 0.0)
    fixture_input = Input((c.c_float*3)(*fixture_start), 0.25, 1, 0, 0.1)
    fixture_output = Output()
    fixture_status = dll.dh2_swamp_movement_step(
        c.byref(fixture_nav), c.byref(fixture_input), 10.0,
        c.byref(fixture_output))
    assert fixture_status == 3 and not fixture_output.moved
    assert_position(fixture_output.position, fixture_start)
    assert math.isclose(fixture_output.heading_radians, 0.25, abs_tol=1e-6)

    # Record the current endpoint-only limitation with separated valid floor
    # patches: both endpoints are walkable but the accepted 8-unit step crosses
    # a 6-unit gap. This is deliberately an observation, not desired parity.
    gap_surfaces = (navigation_checks.Surface*2)()
    gap_triangles = (navigation_checks.SurfaceTriangle*4)()
    gap_vertices = (
        ((0, 0, 0), (2, 0, 0), (2, 2, 0)),
        ((0, 0, 0), (2, 2, 0), (0, 2, 0)),
        ((8, 0, 0), (10, 0, 0), (10, 2, 0)),
        ((8, 0, 0), (10, 2, 0), (8, 2, 0)),
    )
    for surface_index, surface in enumerate(gap_surfaces):
        surface.module_index = 0
        surface.floor_type_flags_known = True
        surface.floor_type_flags = 0
        surface.first_triangle = surface_index*2
        surface.triangle_count = 2
    for triangle_index, triangle in enumerate(gap_triangles):
        triangle.a[:] = gap_vertices[triangle_index][0]
        triangle.b[:] = gap_vertices[triangle_index][1]
        triangle.c[:] = gap_vertices[triangle_index][2]
        triangle.surface_index = triangle_index//2
    gap_nav = navigation_checks.Navigation(
        gap_surfaces, 2, 2, gap_triangles, 4, 4)
    gap_start = (1.0, 1.0, 0.0)
    gap_input = Input((c.c_float*3)(*gap_start), 0.0, 1.0, 0.0, 0.1)
    gap_output = Output()
    gap_status = dll.dh2_swamp_movement_step(
        c.byref(gap_nav), c.byref(gap_input), 80.0, c.byref(gap_output))
    assert gap_status == 0 and gap_output.moved and gap_output.floor_sample_valid
    assert_position(gap_output.position, (9.0, 1.0, 0.0))

    diagonal_status, diagonal = step(start, 0, 1, 1, 0.1)
    assert diagonal_status == 0 and diagonal.moved
    assert math.isclose(math.hypot(diagonal.position[0]-start[0],
                                   diagonal.position[1]-start[1]), 1.0,
                        abs_tol=1e-4)

    clamped_status, clamped = step(start, 0, 0, 1, 1.0)
    fixed_status, fixed = step(start, 0, 0, 1, 0.1)
    repeat_status, repeated = step(start, 0, 0, 1, 0.1)
    assert clamped_status == fixed_status == repeat_status == 0
    assert math.isclose(clamped.dt_used_seconds, 0.1, abs_tol=1e-7)
    assert list(clamped.position) == list(fixed.position) == list(repeated.position)

    zero_status, zero = step(start, 0.7, 0, 0, 0.1)
    assert zero_status == 0 and not zero.moved and not zero.floor_sample_valid
    assert_position(zero.position, start)
    assert math.isclose(zero.heading_radians, 0.7, abs_tol=1e-6)
    assert math.isclose(zero.dt_used_seconds, 0.1, abs_tol=1e-7)

    invalid_status, invalid = step(start, 0, 0, 1, -0.01)
    invalid_axis_status, invalid_axis = step(start, 0, 1.01, 0, 0.1)
    assert invalid_status == 1 and invalid_axis_status == 1
    assert_position(invalid.position, start)
    assert_position(invalid_axis.position, start)

    # Source module-zero edge: y=-199 is supported; stepping to y=-198 is not.
    boundary_start = (1090.75, -199.0, 258.0)
    inward_status, inward = step(boundary_start, 0, 0, -1, 0.1)
    assert inward_status == 0 and math.isclose(inward.position[1], -200.0,
                                               abs_tol=1e-4)
    edge_status, edge = step(boundary_start, 0, 0, 1, 0.1)
    assert edge_status == 3 and not edge.moved
    assert list(edge.position) == list(boundary_start)
    unsupported_status, unsupported = step((5000, 0, 258), 0, 1, 0, 0.1)
    assert unsupported_status == 3 and not unsupported.moved
    assert list(unsupported.position) == [5000, 0, 258]

    # PFWorld::ValidatePosition's default tolerance is strict
    # abs(candidateZ-floorZ) < 100. The source entry floor is at Z=255.
    below_limit_status, below_limit = step((start[0], start[1], 354.9),
                                           0, 1, 0, 0.1)
    at_limit_status, at_limit = step((start[0], start[1], 355.0),
                                     0, 1, 0, 0.1)
    assert below_limit_status == 0 and below_limit.moved
    assert at_limit_status == 3 and not at_limit.moved
    assert_position(at_limit.position, (start[0], start[1], 355.0))

    report = {
        'pass': True,
        'complete_movement': False,
        'scope': 'bounded module-zero endpoint point stepping with baseline path mask 2 and strict native floor-height tolerance; no sweep or full controller parity',
        'checks': {
            'source_entry_point': list(start),
            'positive_x_step': list(right.position),
            'positive_y_step': list(forward.position),
            'accepted_position_z_matches_sampled_floor': True,
            'clamped_dt_seconds': clamped.dt_used_seconds,
            'zero_input_preserves_position_and_heading': True,
            'invalid_input_preserves_position': True,
            'module_zero_boundary_rejects_uncovered_candidate': True,
            'unsupported_start_rejected': True,
            'baseline_mask_2_passes_source_water': list(water_sample),
            'baseline_mask_2_rejects_source_hole': list(hole_sample),
            'hole_endpoint_failure_rolls_back_fixture_position_and_heading': True,
            'known_gap_endpoint_only_step_is_accepted': {
                'start': list(gap_start),
                'end': list(gap_output.position),
                'gap_x': [2.0, 8.0],
                'purpose': 'locks down the known endpoint-only limitation; not a movement-parity claim',
            },
            'strict_100_unit_floor_height_tolerance': True,
            'position_rollback_on_rejected_endpoint': list(edge.position),
        },
        'source_mesh': {'surfaces': nav.surface_count,
                        'triangles': nav.triangle_count},
    }
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))
    dll.dh2_nav_free(c.byref(nav))
    dll.dh2_world_free(c.byref(level))


if __name__ == '__main__':
    main()
