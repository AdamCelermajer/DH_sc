#!/usr/bin/env python3
"""Check the source renderer's bounded scene buffer assembly on a private BRES."""

from __future__ import annotations

import argparse
import ctypes as c
import hashlib
import math
from pathlib import Path


U = c.c_uint32
P = c.c_void_p


class Bres(c.Structure):
    _fields_ = [('bytes', P), ('size', c.c_size_t)] + [(name, U) for name in (
        'fixup_count', 'fixup_offset', 'root_offset', 'tail_offset',
        'bulk_size', 'block_count', 'tail_size')]


class SceneMesh(c.Structure):
    _fields_ = [('vertices', c.POINTER(c.c_float)),
                ('indices', c.POINTER(c.c_uint16)),
                ('vertex_count', U), ('index_count', U), ('draw_commands', U),
                ('skin_joints', U),
                ('first_diffuse_texture', c.c_char * 96)]


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--library', type=Path, required=True)
    parser.add_argument('--sample', type=Path, required=True)
    parser.add_argument('--complex-sample', type=Path)
    parser.add_argument('--character-sample', type=Path)
    args = parser.parse_args()
    dll = c.CDLL(str(args.library.resolve()))
    dll.dh2_bres_open.argtypes = [c.POINTER(Bres), P, c.c_size_t]
    dll.dh2_bres_open.restype = U
    dll.dh2_viewer_scene_mesh.argtypes = [c.POINTER(SceneMesh), c.POINTER(Bres)]
    dll.dh2_viewer_scene_mesh.restype = U
    dll.dh2_viewer_scene_mesh_free.argtypes = [c.POINTER(SceneMesh)]
    raw = args.sample.read_bytes()
    source = c.create_string_buffer(raw)
    before = hashlib.sha256(raw).digest()
    view = Bres()
    assert dll.dh2_bres_open(c.byref(view), source, len(raw)) == 0
    result = SceneMesh()
    assert dll.dh2_viewer_scene_mesh(c.byref(result), c.byref(view)) == 0
    assert (result.vertex_count, result.index_count, result.draw_commands) == (8, 12, 2)
    assert result.first_diffuse_texture == b'env_crypt.tga'
    assert result.skin_joints == 0
    vertices = [result.vertices[i] for i in range(result.vertex_count * 5)]
    indices = [result.indices[i] for i in range(result.index_count)]
    assert all(math.isfinite(x) for x in vertices)
    assert all(-0.51 <= vertices[5 * i + axis] <= 0.51
               for i in range(result.vertex_count) for axis in (0, 1, 2))
    assert all(0 <= index < result.vertex_count for index in indices)
    assert len(set(indices)) == 8  # Both scene draw commands were included.
    assert hashlib.sha256(source.raw[:len(raw)]).digest() == before
    dll.dh2_viewer_scene_mesh_free(c.byref(result))
    assert not result.vertices and not result.indices and result.draw_commands == 0
    assert dll.dh2_viewer_scene_mesh(c.byref(result), None) == 1
    if args.complex_sample:
        raw = args.complex_sample.read_bytes()
        source = c.create_string_buffer(raw)
        view = Bres()
        assert dll.dh2_bres_open(c.byref(view), source, len(raw)) == 0
        assert dll.dh2_viewer_scene_mesh(c.byref(result), c.byref(view)) == 0
        assert (result.draw_commands, result.vertex_count, result.index_count) == (
            77, 3140, 4695)
        assert result.first_diffuse_texture == b'env_voidmaze.tga'
        coordinates = [result.vertices[i] for i in range(result.vertex_count * 5)]
        assert all(math.isfinite(x) for x in coordinates)
        assert all(-0.51 <= coordinates[5 * i + axis] <= 0.51
                   for i in range(result.vertex_count) for axis in (0, 1, 2))
        assert max(coordinates[5 * i + 1] for i in range(result.vertex_count)) > (
            min(coordinates[5 * i + 1] for i in range(result.vertex_count)))
        assert hashlib.sha256(source.raw[:len(raw)]).digest() == hashlib.sha256(raw).digest()
        dll.dh2_viewer_scene_mesh_free(c.byref(result))
    if args.character_sample:
        raw = args.character_sample.read_bytes()
        source = c.create_string_buffer(raw)
        view = Bres()
        assert dll.dh2_bres_open(c.byref(view), source, len(raw)) == 0
        assert dll.dh2_viewer_scene_mesh(c.byref(result), c.byref(view)) == 0
        assert (result.draw_commands, result.vertex_count, result.skin_joints) == (1, 335, 18)
        assert result.first_diffuse_texture == b'prince-warrior.tga'
        assert all(math.isfinite(result.vertices[i]) for i in range(result.vertex_count * 5))
        assert all(result.indices[i] < result.vertex_count for i in range(result.index_count))
        assert hashlib.sha256(source.raw[:len(raw)]).digest() == hashlib.sha256(raw).digest()
        dll.dh2_viewer_scene_mesh_free(c.byref(result))
    print('scene buffers: candle 2 commands; optional void maze 77 commands; '
          'bounded world XYZ preserved and private BRES unchanged')


if __name__ == '__main__':
    main()
