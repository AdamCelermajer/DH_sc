#!/usr/bin/env python3
"""Independent XML comparison, placement checks and bounded-input failures."""
import argparse
from collections import Counter
import ctypes as c
import hashlib
import importlib.util
import json
import math
from pathlib import Path
import sys
import xml.etree.ElementTree as ET

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[1]
U = c.c_uint32
P = c.c_void_p


class Transform(c.Structure):
    _fields_ = [(s, c.c_float*3) for s in ['position', 'rotation_degrees', 'scale']]


class Field(c.Structure):
    _fields_ = [('name', c.c_char_p), ('value', c.c_char_p)]


class Object(c.Structure):
    _fields_ = [('source_path', c.c_char_p)] + [(s, U) for s in
        ['source_record', 'source_begin', 'source_end', 'module_index', 'kind']] + [
        ('fields', c.POINTER(Field)), ('field_count', U), ('name', c.c_char_p),
        ('gametype', c.c_char_p), ('local', Transform), ('world_position', c.c_float*3)]


class Module(c.Structure):
    _fields_ = [('record', Object)] + [(s, c.c_char_p) for s in
        ['cache_dae', 'cache_mgp', 'cache_mvp', 'catalogue_node_id']] + [
        ('mgp_loaded', c.c_bool), ('mvp_loaded', c.c_bool)]


class Level(c.Structure):
    _fields_ = [('name', c.c_char_p), ('source_path', c.c_char_p), ('config', Object),
                ('modules', c.POINTER(Module)), ('module_count', U),
                ('entities', c.POINTER(Object)), ('entity_count', U)]


class Diagnostic(c.Structure):
    _fields_ = [(s, U) for s in ['error', 'byte_offset', 'line', 'column']] + [('message', c.c_char*160)]


class Binding(c.Structure):
    _fields_ = [('visual_index', U), ('node_record', U),
               ('catalogue_origin', c.c_float*3), ('placement_delta', c.c_float*3)]


spec = importlib.util.spec_from_file_location('checked_scene', ROOT/'../scene-payloads/tests/audit_cache.py')
scene = importlib.util.module_from_spec(spec)
spec.loader.exec_module(scene)


def bind(path):
    dll = scene.bind(path)
    definitions = {
        'dh2_world_import_level': (U, [c.POINTER(Level), c.c_char_p, c.c_char_p, P, c.c_size_t, c.POINTER(Diagnostic)]),
        'dh2_world_import_module_objects': (U, [c.POINTER(Level), U, U, c.c_char_p, P, c.c_size_t, c.POINTER(Diagnostic)]),
        'dh2_world_cache_path': (U, [P, c.c_size_t, c.c_char_p, c.POINTER(Diagnostic)]),
        'dh2_world_field': (c.c_char_p, [c.POINTER(Object), c.c_char_p]),
        'dh2_world_free': (None, [c.POINTER(Level)]),
        'dh2_world_bind_module': (U, [c.POINTER(Binding), c.POINTER(Module), c.POINTER(scene.Scene), c.POINTER(Diagnostic)]),
        'dh2_world_module_records': (U, [c.POINTER(U), U, c.POINTER(U), c.POINTER(Binding), c.POINTER(scene.Scene), c.POINTER(Diagnostic)]),
        'dh2_world_placement_matrix': (U, [c.POINTER(scene.Matrix), c.POINTER(Binding), c.POINTER(Diagnostic)]),
        'dh2_world_place_matrix': (U, [c.POINTER(scene.Matrix), c.POINTER(Binding), c.POINTER(scene.Matrix), c.POINTER(Diagnostic)]),
    }
    for name, (result, args) in definitions.items():
        getattr(dll, name).restype = result
        getattr(dll, name).argtypes = args
    return dll


def decode(value):
    return value.decode('utf-8')


def fields(obj):
    return {decode(obj.fields[i].name): decode(obj.fields[i].value) for i in range(obj.field_count)}


def normalize(path):
    value = path.replace('\\', '/').lower()
    return value.replace('data/iphone/', 'data/', 1) if value.startswith('data/iphone/') else value


def close(a, b):
    assert len(a) == len(b)
    assert all(math.isclose(x, y, rel_tol=1e-6, abs_tol=0.002) for x, y in zip(a, b)), (list(a), list(b))


def compare(obj, attributes, source, index, raw, module=None):
    assert fields(obj) == attributes
    assert decode(obj.source_path) == source
    assert obj.source_record == index
    assert ET.fromstring(raw[obj.source_begin:obj.source_end]).attrib == attributes
    for xml, member in [('position', 'position'), ('rotation', 'rotation_degrees'), ('scale', 'scale')]:
        close(getattr(obj.local, member), [float(x) for x in attributes[xml].split(',')])
    origin = module.record.local.position if module else [0, 0, 0]
    close(obj.world_position, [obj.local.position[i] + origin[i] for i in range(3)])


SYNTHETIC = b'''<?xml version="1.0" encoding="utf-8"?>
<Level><GameObject name="config" gametype="LevelConfig" position="0,0,0" rotation="0,0,0" scale="1,1,1"/>
<GameObject name="room" gametype="Module" position="-6000,0,0" rotation="0,0,0" scale="1,1,1"
dae="data/3d/a.bdae" mgp="data/iphone/3d/a.mgp" mvp="data/3d/a.mvp" xrefobject="_module_test"/></Level>'''
MGP = b'''<Module><GameObject name="one &amp; two" gametype="Dummy" position="1,2,3" rotation="0,0,0" scale="1,1,1" note="&#x1f30d; &quot;quoted&quot;"/></Module>'''


def synthetic_checks(dll):
    d = Diagnostic()
    count = 0
    for original, expected in [(b'data/iphone/3D/Modules/A.mgp', b'data/3d/modules/a.mgp'),
                               (b'data\\3D\\a.bdae', b'data/3d/a.bdae')]:
        output = c.create_string_buffer(1025)
        assert dll.dh2_world_cache_path(output, len(output), original, c.byref(d)) == 0
        assert output.value == expected
        count += 1
    for original in [b'', b'/data/x', b'C:/data/x', b'data/../x', b'data//x', b'data/./x', b'data/x/', b'data/x#id']:
        output = c.create_string_buffer(1025)
        assert dll.dh2_world_cache_path(output, len(output), original, c.byref(d)) != 0
        assert output.value == b''
        count += 1
    output = c.create_string_buffer(4)
    assert dll.dh2_world_cache_path(output, 4, b'data/a', c.byref(d)) != 0
    count += 1
    output = c.create_string_buffer(b'data/iphone/3D/A.mgp', 1025)
    assert dll.dh2_world_cache_path(output, len(output), c.cast(output, c.c_char_p), c.byref(d)) == 0
    assert output.value == b'data/3d/a.mgp'
    count += 1
    level = Level()
    assert dll.dh2_world_import_level(c.byref(level), b'SYNTHETIC', b'data/scene/test.mlx', SYNTHETIC, len(SYNTHETIC), c.byref(d)) == 0, d.message
    try:
        # Failed replacements keep the successfully imported level intact.
        before = c.string_at(c.byref(level), c.sizeof(level))
        invalid = [SYNTHETIC[:i] for i in range(len(SYNTHETIC))]
        invalid.extend([SYNTHETIC+b'x', SYNTHETIC.replace(b'<Level>', b'<!DOCTYPE Level><Level>'),
                        SYNTHETIC.replace(b'name="room"', b'name="room" name="duplicate"')])
        for value in [b'nan,0,0', b'inf,0,0', b'1e99,0,0', b'0x1p2,0,0', b'1,2', b'1,2,3,4']:
            invalid.append(SYNTHETIC.replace(b'-6000,0,0', value))
        invalid.append(SYNTHETIC.replace(b'room', b'room&unknown;'))
        invalid.append(SYNTHETIC.replace(b'room', b'room\xc0\xaf'))
        for raw in invalid:
            assert dll.dh2_world_import_level(c.byref(level), b'BAD', b'data/scene/test.mlx', raw, len(raw), c.byref(d)) != 0
            assert c.string_at(c.byref(level), c.sizeof(level)) == before
            count += 1
        assert dll.dh2_world_import_module_objects(c.byref(level), 0, 1, b'data/3d/wrong.mgp', MGP, len(MGP), c.byref(d)) != 0
        assert level.entity_count == 0
        assert dll.dh2_world_import_module_objects(c.byref(level), 0, 1, b'data/3d/a.mgp', MGP, len(MGP), c.byref(d)) == 0, d.message
        assert level.entities[0].name == b'one & two'
        assert dll.dh2_world_field(c.byref(level.entities[0]), b'note') == '\U0001f30d "quoted"'.encode()
        close(level.entities[0].world_position, [-5999, 2, 3])
        assert dll.dh2_world_import_module_objects(c.byref(level), 0, 1, b'data/3d/a.mgp', MGP, len(MGP), c.byref(d)) == 11
        assert level.entity_count == 1
        level.modules[0].record.local.rotation_degrees[2] = 90
        assert dll.dh2_world_import_module_objects(c.byref(level), 0, 2, b'data/3d/a.mvp', MGP, len(MGP), c.byref(d)) == 12
        assert level.entity_count == 1 and not level.modules[0].mvp_loaded
        count += 5
        correction = scene.Matrix()
        binding = Binding(0, 0, (c.c_float*3)(0, 0, 0), (c.c_float*3)(-6000, 2, 3))
        assert dll.dh2_world_placement_matrix(c.byref(correction), c.byref(binding), c.byref(d)) == 0
        close(correction.m[12:15], [-6000, 2, 3])
        correction.m[3] = 1
        result = scene.Matrix()
        assert dll.dh2_world_place_matrix(c.byref(result), c.byref(binding), c.byref(correction), c.byref(d)) == 12
        count += 2
    finally:
        dll.dh2_world_free(c.byref(level))
    assert level.module_count == level.entity_count == 0
    return count


def cache_checks(dll, cache):
    source = 'data/scene/001_swamp.mlx'
    raw = (cache/source).read_bytes()
    original = ET.fromstring(raw).findall('GameObject')
    level = Level()
    d = Diagnostic()
    assert dll.dh2_world_import_level(c.byref(level), b'SWAMP', source.encode(), raw, len(raw), c.byref(d)) == 0, d.message
    hashes = {source: hashlib.sha256(raw).hexdigest()}
    try:
        assert level.module_count == 9
        compare(level.config, original[0].attrib, source, 0, raw)
        assert fields(level.config)['scriptFile'] == 'data\\PyData\\scripts\\001_SWAMP.pyscript'
        totals = Counter()
        for i in range(level.module_count):
            module = level.modules[i]
            compare(module.record, original[i+1].attrib, source, i+1, raw)
            assert module.catalogue_node_id == (original[i+1].attrib['xrefobject']+'-node').encode()
            for kind, field in [(1, 'mgp'), (2, 'mvp')]:
                filename = normalize(original[i+1].attrib[field])
                content = (cache/filename).read_bytes()
                hashes[filename] = hashlib.sha256(content).hexdigest()
                expected = ET.fromstring(content).findall('GameObject')
                before = level.entity_count
                assert dll.dh2_world_import_module_objects(c.byref(level), i, kind, filename.encode(), content, len(content), c.byref(d)) == 0, (filename, d.message)
                assert level.entity_count == before + len(expected)
                for j, element in enumerate(expected):
                    obj = level.entities[before+j]
                    compare(obj, element.attrib, filename, j, content, module)
                    assert obj.kind == kind and obj.module_index == i
                    totals[decode(obj.gametype)] += 1
        assert totals['Character'] == 50 and totals['SpawnPoint'] == 11
        assert totals['AnimatedDecor'] == 37 and totals['Decor'] == 10
        entries = [level.entities[i] for i in range(level.entity_count)
                   if level.entities[i].name == b'_prim_EntryPoint' and level.entities[i].module_index == 0]
        assert len(entries) == 1
        close(entries[0].world_position, [1090.75, -212.202, 258])
        assert fields(entries[0])['entrypointID'] == '0'
        intro = [level.entities[i] for i in range(level.entity_count)
                 if level.entities[i].name == b'_prim_Monster_LizManIntro1']
        assert len(intro) == 1
        close(intro[0].world_position, [-3725.75, 252.509, 250])
        assert fields(intro[0])['ai_state'] == 'Limbus'
        assert fields(intro[0])['auto_spawn'] == '0'
        catalogue_path = decode(level.modules[0].cache_dae)
        catalogue = (cache/catalogue_path).read_bytes()
        hashes[catalogue_path] = hashlib.sha256(catalogue).hexdigest()
        storage = c.create_string_buffer(catalogue)
        bres, model = scene.Bres(), scene.Scene()
        assert dll.dh2_bres_open(c.byref(bres), storage, len(catalogue)) == 0
        assert dll.dh2_scene_open(c.byref(model), c.byref(bres)) == 0
        modules = []
        for i in range(level.module_count):
            module = level.modules[i]
            binding = Binding()
            assert dll.dh2_world_bind_module(c.byref(binding), c.byref(module), c.byref(model), c.byref(d)) == 0, d.message
            records = (U*4096)()
            count = U()
            assert dll.dh2_world_module_records(records, len(records), c.byref(count), c.byref(binding), c.byref(model), c.byref(d)) == 0, d.message
            assert count.value > 1 and records[0] == binding.node_record
            selected = set(records[:count.value])
            assert len(selected) == count.value
            # Independent reference traversal: root and all child records.
            visual, node = scene.Visual(), scene.Node()
            assert dll.dh2_scene_visual(c.byref(model), binding.visual_index, c.byref(visual)) == 0
            root = None
            for r in range(visual.roots):
                assert dll.dh2_scene_root_node(c.byref(visual), r, c.byref(node)) == 0
                if node.record == binding.node_record:
                    root = scene.Node.from_buffer_copy(node)
            assert root is not None
            reference = []
            def visit(n):
                reference.append(n.record)
                for child in range(n.children):
                    entry = scene.Node()
                    assert dll.dh2_scene_child_node(c.byref(n), child, c.byref(entry)) == 0
                    visit(entry)
            visit(root)
            assert reference == list(records[:count.value])
            matrix, placed = scene.Matrix(), scene.Matrix()
            assert dll.dh2_scene_local_matrix(c.byref(root), c.byref(matrix)) == 0
            assert dll.dh2_world_place_matrix(c.byref(placed), c.byref(binding), c.byref(matrix), c.byref(d)) == 0
            close(placed.m[12:15], module.record.local.position)
            assert list(placed.m[:12]) == list(matrix.m[:12])
            short_count = U(99)
            assert dll.dh2_world_module_records(records, 1, c.byref(short_count), c.byref(binding), c.byref(model), c.byref(d)) == 3
            assert short_count.value == 0
            modules.append({'name': decode(module.record.name),
                            'world_origin': list(module.record.local.position),
                            'catalogue_origin': list(binding.catalogue_origin),
                            'node_record': binding.node_record, 'subtree_nodes': count.value})
        return {'level': 'SWAMP', 'modules': modules, 'entities': level.entity_count,
                'entity_types': dict(sorted(totals.items())), 'source_sha256': hashes,
                'original_entry0': list(entries[0].world_position),
                'original_intro_enemy1': list(intro[0].world_position)}
    finally:
        dll.dh2_world_free(c.byref(level))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--library', type=Path, required=True)
    parser.add_argument('--cache', type=Path)
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    dll = bind(args.library)
    report = {'synthetic_checks': synthetic_checks(dll), 'pass': True,
              'complete_game': False, 'level_playable': False, 'scripts_executed': False}
    if args.cache:
        report['original_cache'] = cache_checks(dll, args.cache)
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
