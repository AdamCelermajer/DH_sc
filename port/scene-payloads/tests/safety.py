#!/usr/bin/env python3
"""Check malformed nested scene records without keeping any cache bytes."""
import argparse
import ctypes as c
import hashlib
import json
import struct
from pathlib import Path

from audit_cache import Bres, Scene, Visual, Node, Matrix, VISIT, bind


def opened(dll, data):
    storage = c.create_string_buffer(bytes(data))
    bres = Bres()
    assert dll.dh2_bres_open(c.byref(bres), storage, len(data)) == 0
    return storage, bres


def main():
    p = argparse.ArgumentParser()
    p.add_argument('--sample', type=Path, required=True)
    p.add_argument('--library', type=Path, required=True)
    p.add_argument('--report', type=Path)
    args = p.parse_args()
    dll = bind(args.library)
    original = args.sample.read_bytes()
    u = lambda b, o: struct.unpack_from('<I', b, o)[0]
    root = u(original, 32)
    visual_offset = u(original, root+0x9c)
    first_node = u(original, visual_offset+12)
    cases = 0

    storage, bres = opened(dll, original)
    scene = Scene()
    assert dll.dh2_scene_open(c.byref(scene), c.byref(bres)) == 0
    assert dll.dh2_scene_visual_index(c.byref(scene), b'external#nope') == -1
    assert dll.dh2_scene_visual_index(c.byref(scene), b'#missing') == -1
    cases += 3

    damaged = bytearray(original)
    struct.pack_into('<I', damaged, root+0xb8, 0xffffffff)
    storage, bres = opened(dll, damaged)
    scene = Scene()
    assert dll.dh2_scene_open(c.byref(scene), c.byref(bres)) == 2
    assert scene.references == 0
    cases += 1

    damaged = bytearray(original)
    struct.pack_into('<I', damaged, visual_offset+8, 0xffffffff)
    storage, bres = opened(dll, damaged)
    scene = Scene()
    assert dll.dh2_scene_open(c.byref(scene), c.byref(bres)) == 0
    visual = Visual()
    assert dll.dh2_scene_visual(c.byref(scene), 0, c.byref(visual)) == 2
    assert visual.roots == 0
    cases += 1

    damaged = bytearray(original)
    struct.pack_into('<I', damaged, first_node+0x38, 0xffffffff)
    storage, bres = opened(dll, damaged)
    scene = Scene()
    assert dll.dh2_scene_open(c.byref(scene), c.byref(bres)) == 0
    visual = Visual()
    assert dll.dh2_scene_visual(c.byref(scene), 0, c.byref(visual)) == 0
    node = Node()
    assert dll.dh2_scene_root_node(c.byref(visual), 0, c.byref(node)) == 2
    assert node.record == 0
    cases += 1

    damaged = bytearray(original)
    struct.pack_into('<I', damaged, first_node+0x0c, 0x7fc00000)
    storage, bres = opened(dll, damaged)
    scene = Scene()
    assert dll.dh2_scene_open(c.byref(scene), c.byref(bres)) == 0
    visual = Visual()
    assert dll.dh2_scene_visual(c.byref(scene), 0, c.byref(visual)) == 0
    node = Node()
    assert dll.dh2_scene_root_node(c.byref(visual), 0, c.byref(node)) == 4
    assert node.record == 0
    cases += 1

    # A noncommutative transform fixture: +90-degree Z quaternion uses the
    # original transposed matrix, then unequal column scales and translation.
    parent_node = Node()
    parent_node.position[:] = (10.0, 20.0, 30.0)
    parent_node.rotation[:] = (0.0, 0.0, 0.7071067811865476, 0.7071067811865476)
    parent_node.scale[:] = (2.0, 3.0, 4.0)
    child_node = Node()
    child_node.position[:] = (1.0, 2.0, 3.0)
    child_node.rotation[:] = (0.0, 0.0, 0.0, 1.0)
    child_node.scale[:] = (1.0, 1.0, 1.0)
    local, world = Matrix(), Matrix()
    assert dll.dh2_scene_local_matrix(c.byref(parent_node), c.byref(local)) == 0
    assert dll.dh2_scene_world_matrix(c.byref(child_node), c.byref(local), c.byref(world)) == 0
    assert all(abs(world.m[12+i]-v) < 1e-5 for i, v in enumerate((16.0, 18.0, 42.0)))
    cases += 3

    # Input/output aliasing is intentional and the parent is validated.
    alias = Matrix.from_buffer_copy(bytes(local))
    assert dll.dh2_scene_world_matrix(c.byref(child_node), c.byref(alias), c.byref(alias)) == 0
    assert list(alias.m) == list(world.m)
    bad_parent = Matrix.from_buffer_copy(bytes(local))
    bad_parent.m[15] = 0.0
    assert dll.dh2_scene_world_matrix(c.byref(child_node), c.byref(bad_parent), c.byref(alias)) == 6
    child_node.position[0] = float('nan')
    assert dll.dh2_scene_local_matrix(c.byref(child_node), c.byref(alias)) == 4
    cases += 3

    # Deliberately form a self-child edge in the otherwise valid BRES image.
    damaged = bytearray(original)
    struct.pack_into('<II', damaged, first_node+0x38, 1, first_node)
    storage, bres = opened(dll, damaged)
    scene = Scene()
    assert dll.dh2_scene_open(c.byref(scene), c.byref(bres)) == 0
    visual = Visual()
    assert dll.dh2_scene_visual(c.byref(scene), 0, c.byref(visual)) == 0

    @VISIT
    def keep_going(_node, _world, _depth, _user):
        return True

    assert dll.dh2_scene_walk_visual(c.byref(visual), keep_going, None, 100) == 8
    cases += 1

    storage, bres = opened(dll, original)
    scene = Scene()
    assert dll.dh2_scene_open(c.byref(scene), c.byref(bres)) == 0
    visual = Visual()
    assert dll.dh2_scene_visual(c.byref(scene), 0, c.byref(visual)) == 0
    assert dll.dh2_scene_walk_visual(c.byref(visual), keep_going, None, 1) == 7

    @VISIT
    def stop(_node, _world, _depth, _user):
        return False

    assert dll.dh2_scene_walk_visual(c.byref(visual), stop, None, 100) == 9
    cases += 2

    report = {'sample_sha256': hashlib.sha256(original).hexdigest(),
              'checks': cases, 'passed': True, 'complete_engine': False}
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
