#!/usr/bin/env python3
"""Exercise every scene, node and instance in the owner's recovered BRES cache."""
import argparse
from collections import Counter
import ctypes as c
import hashlib
import json
import math
from pathlib import Path

U = c.c_uint32
I = c.c_int32
P = c.c_void_p


class Bres(c.Structure):
    _fields_ = [('bytes', P), ('size', c.c_size_t)] + [(x, U) for x in
        ['fixup_count', 'fixup_offset', 'root_offset', 'tail_offset',
         'bulk_size', 'block_count', 'tail_size']]


class Scene(c.Structure):
    _fields_ = [('image', Bres)] + [(x, U) for x in
        ['references', 'reference_offset', 'visuals', 'visual_offset']]


class Reference(c.Structure):
    _fields_ = [('type', U), ('url', P)]


class Visual(c.Structure):
    _fields_ = [('image', Bres), ('id', P), ('name', P), ('roots', U), ('root_offset', U)]


class Node(c.Structure):
    _fields_ = [('image', Bres), ('id', P), ('name', P)] + [(x, U) for x in
        ['record', 'children', 'child_offset', 'instances', 'instance_offset',
         'visible', 'extension_offset']] + [('position', c.c_float*3),
        ('rotation', c.c_float*4), ('scale', c.c_float*3)]


class Instance(c.Structure):
    _fields_ = [('type', U), ('payload_offset', U), ('geometry_url', P)]


class Matrix(c.Structure):
    _fields_ = [('m', c.c_float*16), ('identity_hint', c.c_uint8)]


VISIT = c.CFUNCTYPE(c.c_bool, c.POINTER(Node), c.POINTER(Matrix), U, P)


def bind(path):
    dll = c.CDLL(str(path.resolve()))
    specs = {
        'dh2_bres_open': (U, [c.POINTER(Bres), P, c.c_size_t]),
        'dh2_scene_open': (U, [c.POINTER(Scene), c.POINTER(Bres)]),
        'dh2_scene_reference': (U, [c.POINTER(Scene), I, c.POINTER(Reference)]),
        'dh2_scene_visual': (U, [c.POINTER(Scene), I, c.POINTER(Visual)]),
        'dh2_scene_root_node': (U, [c.POINTER(Visual), I, c.POINTER(Node)]),
        'dh2_scene_child_node': (U, [c.POINTER(Node), I, c.POINTER(Node)]),
        'dh2_scene_instance': (U, [c.POINTER(Node), I, c.POINTER(Instance)]),
        'dh2_scene_visual_index': (I, [c.POINTER(Scene), c.c_char_p]),
        'dh2_scene_geometry_index': (I, [c.POINTER(Scene), c.POINTER(Instance)]),
        'dh2_scene_local_matrix': (U, [c.POINTER(Node), c.POINTER(Matrix)]),
        'dh2_scene_world_matrix': (U, [c.POINTER(Node), c.POINTER(Matrix), c.POINTER(Matrix)]),
        'dh2_scene_walk_visual': (U, [c.POINTER(Visual), VISIT, P, U]),
    }
    for name, (return_type, args) in specs.items():
        f = getattr(dll, name)
        f.restype = return_type
        f.argtypes = args
    return dll


def check_file(dll, path, relative, totals, kinds, unresolved):
    raw = path.read_bytes()
    storage = c.create_string_buffer(raw)
    bres = Bres()
    assert dll.dh2_bres_open(c.byref(bres), storage, len(raw)) == 0, path
    scene = Scene()
    assert dll.dh2_scene_open(c.byref(scene), c.byref(bres)) == 0, path
    totals['files'] += 1
    totals['references'] += scene.references
    totals['visual_scenes'] += scene.visuals
    for i in range(scene.references):
        ref = Reference()
        assert dll.dh2_scene_reference(c.byref(scene), i, c.byref(ref)) == 0, (path, i)
        assert ref.type == 6
        url = c.string_at(ref.url)
        visual_index = dll.dh2_scene_visual_index(c.byref(scene), url)
        if visual_index < 0:
            unresolved.append({'file': relative, 'kind': 'visual', 'url': url.decode(errors='replace')})
        else:
            totals['resolved_visual_references'] += 1
    for i in range(scene.visuals):
        visual = Visual()
        assert dll.dh2_scene_visual(c.byref(scene), i, c.byref(visual)) == 0, (path, i)
        assert c.string_at(visual.id) and c.string_at(visual.name)
        totals['root_nodes'] += visual.roots
        walked = {}

        @VISIT
        def visit(node_ptr, matrix_ptr, depth, _user):
            node = node_ptr.contents
            matrix = matrix_ptr.contents
            walked[node.record] = (depth, tuple(matrix.m))
            return True

        assert dll.dh2_scene_walk_visual(c.byref(visual), visit, None, 100000) == 0, (path, i)
        pending = []
        for j in range(visual.roots):
            node = Node()
            assert dll.dh2_scene_root_node(c.byref(visual), j, c.byref(node)) == 0, (path, i, j)
            pending.append((node, 0, None))
        visited = set()
        while pending:
            node, depth, parent = pending.pop()
            assert depth < 100 and node.record not in visited, (path, node.record)
            visited.add(node.record)
            world = Matrix()
            assert dll.dh2_scene_world_matrix(c.byref(node),
                c.byref(parent) if parent is not None else None, c.byref(world)) == 0, (path, node.record)
            assert node.record in walked and walked[node.record] == (depth, tuple(world.m))
            assert [world.m[k] for k in (3, 7, 11, 15)] == [0.0, 0.0, 0.0, 1.0]
            if parent is None:
                assert tuple(world.m[12:15]) == tuple(node.position)
            else:
                # Independent point-transform check for the affine translation
                # column; this catches a reversed parent/child multiplication.
                for axis in range(3):
                    expected = (parent.m[axis]*node.position[0]
                                + parent.m[4+axis]*node.position[1]
                                + parent.m[8+axis]*node.position[2]
                                + parent.m[12+axis])
                    assert math.isclose(world.m[12+axis], expected,
                                        rel_tol=3e-6, abs_tol=0.002), (path, node.record, axis)
            totals['world_matrices'] += 1
            if tuple(node.rotation) != (0.0, 0.0, 0.0, 1.0):
                totals['rotated_nodes'] += 1
            if tuple(node.scale) != (1.0, 1.0, 1.0):
                totals['scaled_nodes'] += 1
            totals['nodes'] += 1
            totals['max_depth'] = max(totals['max_depth'], depth)
            totals['child_links'] += node.children
            totals['instances'] += node.instances
            if node.extension_offset:
                totals['nodes_with_opaque_extension'] += 1
            for j in range(node.instances):
                instance = Instance()
                assert dll.dh2_scene_instance(c.byref(node), j, c.byref(instance)) == 0, (path, node.record, j)
                kinds[str(instance.type)] += 1
                if instance.type == 3:
                    assert instance.geometry_url
                    geometry_index = dll.dh2_scene_geometry_index(c.byref(scene), c.byref(instance))
                    if geometry_index < 0:
                        unresolved.append({'file': relative, 'kind': 'geometry',
                                           'url': c.string_at(instance.geometry_url).decode(errors='replace')})
                    else:
                        totals['resolved_geometry_references'] += 1
            for j in range(node.children):
                child = Node()
                assert dll.dh2_scene_child_node(c.byref(node), j, c.byref(child)) == 0, (path, node.record, j)
                pending.append((child, depth+1, world))
        assert set(walked) == visited
    return raw


def main():
    p = argparse.ArgumentParser()
    p.add_argument('--cache', type=Path, required=True)
    p.add_argument('--library', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    args = p.parse_args()
    dll = bind(args.library)
    totals, kinds, unresolved = Counter(), Counter(), []
    manifest = hashlib.sha256()
    paths = sorted(args.cache.rglob('*.bdae'))
    for path in paths:
        relative = path.relative_to(args.cache).as_posix()
        raw = check_file(dll, path, relative, totals, kinds, unresolved)
        manifest.update(relative.encode()+b'\0')
        manifest.update(hashlib.sha256(raw).digest())
    report = {
        'all_scene_records_traversed': True, 'complete_engine': False,
        'cache_scope': 'all recovered .bdae files',
        'totals': dict(totals), 'instance_types': dict(kinds),
        'unresolved_reference_samples': unresolved[:10],
        'unresolved_reference_count': len(unresolved),
        'cache_manifest_sha256': manifest.hexdigest(),
        'library_sha256': hashlib.sha256(args.library.read_bytes()).hexdigest(),
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
