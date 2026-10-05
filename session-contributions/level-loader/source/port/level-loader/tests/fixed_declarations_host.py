"""Compare native declaration occurrences with independently read original XML.

Checks every assembled fixed-level source, including all attributes, nested
element structure, module context and checked float32 authored projections.
No class defaults, activation, original InitPre results or runtime objects are
established by this comparison. Procedural/source blockers remain explicit.
"""
import argparse
import collections
import hashlib
import json
import pathlib
import struct
import subprocess
import xml.etree.ElementTree as ET
import zipfile

ap = argparse.ArgumentParser()
ap.add_argument('--cache', type=pathlib.Path, required=True)
ap.add_argument('--probe', type=pathlib.Path, required=True)
ap.add_argument('--map-coverage', type=pathlib.Path, required=True)
ap.add_argument('--out', type=pathlib.Path, required=True)
ap.add_argument('--swamp-out', type=pathlib.Path)
a = ap.parse_args()
with a.cache.open('rb') as file:
    cache_sha = hashlib.file_digest(file, 'sha256').hexdigest()
coverage = json.loads(a.map_coverage.read_text())
assert cache_sha == coverage['cache_sha256'] == '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
prefix = 'com.gameloft.android.GAND.GloftD2SS/files/'
source_hashes = {}

def f32(value):
    return struct.unpack('<f', struct.pack('<f', value))[0]

def point(element, key):
    value = element.attrib.get(key)
    if value is None:
        return None
    fields = value.split(',')
    assert len(fields) == 3
    return [f32(float(v)) for v in fields]

def tree(element):
    return {'tag': element.tag, 'attributes': element.attrib,
            'children': [tree(child) for child in element]}

with zipfile.ZipFile(a.cache) as pack:
    names = {n[len(prefix):].replace('\\', '/').lower(): n
             for n in pack.namelist() if n.startswith(prefix) and not n.endswith('/')}

    def read(authored, tag):
        for folder in ('', 'data/', 'data/scene/', 'data/3d/modules/'):
            candidate = folder + authored
            for marker in ('old/', 'debug/', 'ps3/', 'iphone/'):
                if marker in candidate:
                    candidate = candidate.replace(marker, '', 1)
                    break
            key = candidate.replace('\\', '/').lower()
            if key in names:
                raw = pack.read(names[key])
                source_hashes[key] = hashlib.sha256(raw).hexdigest()
                element = ET.fromstring(raw)
                assert element.tag == tag, key
                return key, element
        raise AssertionError('Missing original XML: ' + authored)

    def expected(definition):
        uri, level = read(definition, 'Level')
        modules = []

        def discover(document, root, offset):
            for child in root:
                if child.attrib.get('gametype') != 'Module':
                    continue
                position = point(child, 'position') or [0, 0, 0]
                translation = [f32(v + o) for v, o in zip(position, offset)]
                inputs = []
                for field, origin in (('mgp', 'gameplay'), ('mvp', 'visual')):
                    authored = child.attrib.get(field)
                    if authored:
                        child_uri, child_root = read(authored, 'Module')
                        inputs.append((origin, child_uri, child_root))
                modules.append((child.attrib['name'], translation, inputs))
                for _, child_uri, child_root in inputs:
                    discover(child_uri, child_root, translation)

        discover(uri, level, [0, 0, 0])
        rows = []

        def collect(uri, root, origin, module, name, offset):
            for order, child in enumerate(root):
                position = point(child, 'position')
                rows.append({'uri': uri, 'tag': child.tag, 'source_order': order,
                    'origin': origin, 'module': module, 'module_name': name,
                    'module_offset': offset, 'attributes': child.attrib,
                    'authored_position': position,
                    'translated_position': None if position is None else
                        [f32(v + o) for v, o in zip(position, offset)],
                    'rotation_degrees': point(child, 'rotation'),
                    'scale': point(child, 'scale'), 'tree': tree(child)})

        collect(uri, level, 'level', None, None, [0, 0, 0])
        for index, (name, offset, inputs) in enumerate(modules):
            for origin, child_uri, child_root in inputs:
                collect(child_uri, child_root, origin, index, name, offset)
        return modules, rows

    rows = []
    total = 0
    for source in coverage['levels']:
        row = {k: source[k] for k in ('name', 'file', 'random', 'map_preparation')}
        if source['map_preparation'] != 'assembled':
            row.update(declarations='source_blocked', reason=source['reason'])
            rows.append(row)
            continue
        run = subprocess.run([str(a.probe.resolve()), str(a.cache.resolve()),
            source['name'], source['file']], text=True, capture_output=True)
        if run.returncode:
            raise RuntimeError(source['name'] + ': ' + run.stderr)
        native = json.loads(run.stdout)
        modules, authored = expected(source['file'])
        assert native['validation'] == 'PASS'
        assert native['module_count'] == len(modules)
        assert native['occurrences'] == len(authored)
        for index, (actual, wanted) in enumerate(zip(native['declarations'], authored)):
            for key, value in wanted.items():
                observed = actual[key]
                if key in ('module_offset', 'authored_position', 'translated_position', 'rotation_degrees', 'scale') and observed is not None:
                    observed = [f32(v) for v in observed]
                assert observed == value, (source['name'], index, key, observed, value)
        assert native['ownership_checks']
        assert not native['runtime_objects_verified'] and not native['gameplay_verified']
        row.update(declarations='source_compared', occurrences=len(authored),
            module_count=len(modules), types=native['types'],
            missing_gametype=native['missing_gametype'], missing_name=native['missing_name'])
        rows.append(row)
        total += len(authored)
        if source['name'] == 'SWAMP':
            assert native['types']['Character'] == 50 and native['types']['OpenableContainer'] == 5
            if a.swamp_out:
                a.swamp_out.write_text(json.dumps(native, indent=2) + '\n')

root = pathlib.Path(__file__).resolve().parents[1]
paths = [root/'fixed_declarations_v1.cpp', root/'fixed_declarations_v1.hpp',
         root/'CMakeLists.txt', root/'tests/fixed_declarations_probe.cpp', pathlib.Path(__file__)]
report = {'validation': 'PASS', 'scope': __doc__, 'cache_sha256': cache_sha,
    'probe_sha256': hashlib.sha256(a.probe.read_bytes()).hexdigest(),
    'map_coverage_sha256': hashlib.sha256(a.map_coverage.read_bytes()).hexdigest(),
    'sources_sha256': {str(p.relative_to(root)): hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
    'authored_xml_sha256': source_hashes, 'levels': rows,
    'summary': dict(collections.Counter(r['declarations'] for r in rows)),
    'occurrences_compared': total, 'runtime_objects_verified': False,
    'gameplay_verified': False, 'full_loader_verified': False}
a.out.write_text(json.dumps(report, indent=2) + '\n')
print(json.dumps({k: report[k] for k in ('validation', 'summary', 'occurrences_compared')}))
