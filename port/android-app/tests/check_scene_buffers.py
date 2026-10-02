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
                ('first_diffuse_texture', c.c_char * 96)]


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--library', type=Path, required=True)
    parser.add_argument('--sample', type=Path, required=True)
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
    vertices = [result.vertices[i] for i in range(result.vertex_count * 4)]
    indices = [result.indices[i] for i in range(result.index_count)]
    assert all(math.isfinite(x) for x in vertices)
    assert all(-0.76 <= vertices[4 * i + axis] <= 0.76
               for i in range(result.vertex_count) for axis in (0, 1))
    assert all(0 <= index < result.vertex_count for index in indices)
    assert len(set(indices)) == 8  # Both scene draw commands were included.
    assert hashlib.sha256(source.raw[:len(raw)]).digest() == before
    dll.dh2_viewer_scene_mesh_free(c.byref(result))
    assert not result.vertices and not result.indices and result.draw_commands == 0
    assert dll.dh2_viewer_scene_mesh(c.byref(result), None) == 1
    print('scene buffers: 2 commands, 8 vertices, 12 indices, '
          'world-projected and bounded; private BRES unchanged')


if __name__ == '__main__':
    main()
