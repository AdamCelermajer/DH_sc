#!/usr/bin/env python3
"""Dump checked source material/effect records for the SWAMP appearance audit."""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys
import ctypes as c


ROOT = Path(__file__).resolve().parents[4]
AUDIT_CACHE = ROOT / 'port/material-bindings/tools/audit_cache.py'
spec = importlib.util.spec_from_file_location('audit_cache', AUDIT_CACHE)
audit_cache = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = audit_cache
spec.loader.exec_module(audit_cache)


def bounded_bytes(view, pointer, maximum=64):
    if not pointer:
        return ''
    base = int(view.bytes)
    address = int(pointer)
    if address < base or address - base >= view.size:
        raise ValueError('serialized-value pointer escaped its BRES image')
    length = min(maximum, view.size - (address - base))
    return c.string_at(pointer, length).hex()


def source_string(view, offset):
    if not offset or offset >= view.size:
        return None
    pointer = int(view.bytes) + offset
    maximum = min(4096, view.size - offset)
    raw = c.string_at(pointer, maximum).split(b'\0', 1)[0]
    try:
        return raw.decode('utf-8')
    except UnicodeDecodeError:
        return None


def describe_named_records(view, pointer, count):
    if not pointer:
        return []
    base = int(view.bytes)
    address = int(pointer)
    if address < base or address - base + count * 12 > view.size:
        raise ValueError('named-record pointer escaped its BRES image')
    entries = []
    for index in range(count):
        fields = struct.unpack('<3I', c.string_at(address + index * 12, 12))
        entries.append({'name': source_string(view, fields[0]),
                        'raw_words': list(fields),
                        'value_prefix_hex': bounded_bytes(view, base + fields[2])
                        if fields[2] else ''})
    return entries


def describe_effect(dll, view, index):
    effect = audit_cache.Effect()
    if dll.dh2_effect_record(c.byref(effect), c.byref(view), index) != 0:
        raise ValueError(f'effect record {index} did not validate')
    result = {'id': audit_cache.decode(effect.id), 'name': audit_cache.decode(effect.name),
              'groups': []}
    for group_index in range(2):
        group = audit_cache.EffectGroup()
        if dll.dh2_effect_group(c.byref(group), c.byref(effect), group_index) != 0:
            raise ValueError(f'effect group {group_index} did not validate')
        entry = {'named_record_count': group.named_count,
                 'named_records_raw': bounded_bytes(view, group.named_records,
                                                     group.named_count * 12),
                 'named_records': describe_named_records(view, group.named_records,
                                                         group.named_count),
                 'parameters': [], 'images': []}
        for parameter_index in range(group.parameter_count):
            parameter = audit_cache.EffectParameter()
            if dll.dh2_effect_parameter(c.byref(parameter), c.byref(group),
                                        parameter_index) != 0:
                raise ValueError('effect parameter did not validate')
            raw = bounded_bytes(view, parameter.raw_value)
            record = {'id': audit_cache.decode(parameter.id),
                      'type_code': parameter.type_code,
                      'raw_word_8': parameter.raw_word_8,
                      'value_count': parameter.value_count,
                      'raw_prefix_hex': raw}
            if parameter.type_code == 16 and parameter.value_count == 1 and len(raw) >= 32:
                record['first_four_f32'] = list(struct.unpack('<4f', bytes.fromhex(raw[:32])))
            entry['parameters'].append(record)
        for image_index in range(group.image_count):
            reference = audit_cache.ImageRef()
            if dll.dh2_effect_group_image(c.byref(reference), c.byref(group),
                                          image_index) != 0:
                raise ValueError('effect image did not validate')
            entry['images'].append({
                'index': reference.index,
                'id': audit_cache.decode(reference.id),
                'name': audit_cache.decode(reference.name),
                'source_path': audit_cache.decode(reference.source_path),
            })
        result['groups'].append(entry)
    return result


def parse_bres(dll, path, selected_materials=()):
    raw = path.read_bytes()
    backing = c.create_string_buffer(raw)
    view = audit_cache.Bres()
    if dll.dh2_bres_open(c.byref(view), backing, len(raw)) != 0:
        raise ValueError(f'BRES rejected: {path}')
    materials = []
    wanted = set(selected_materials)
    count = dll.dh2_bres_library_count(c.byref(view), 6)
    for index in range(count):
        material = audit_cache.Material()
        if dll.dh2_material_record(c.byref(material), c.byref(view), index) != 0:
            raise ValueError(f'material {index} did not validate: {path}')
        material_id = audit_cache.decode(material.id)
        name = audit_cache.decode(material.name)
        if wanted and material_id not in wanted and name not in wanted:
            continue
        record = {'index': index, 'id': material_id, 'name': name,
                  'external_effect_file': audit_cache.decode(material.external_effect_file),
                  'effect_url': audit_cache.decode(material.effect_url), 'parameters': []}
        for parameter_index in range(material.parameter_count):
            parameter = audit_cache.Parameter()
            if dll.dh2_material_parameter(c.byref(parameter), c.byref(material),
                                          parameter_index) != 0:
                raise ValueError(f'material parameter {parameter_index} did not validate')
            item = {'id': audit_cache.decode(parameter.id),
                    'semantic': audit_cache.decode(parameter.semantic),
                    'type_code': parameter.type_code,
                    'value_count': parameter.value_count,
                    'raw_prefix_hex': bounded_bytes(view, parameter.raw_value)}
            if parameter.type_code == 11:
                reference = audit_cache.ImageRef()
                if dll.dh2_material_sampler_image(c.byref(reference), c.byref(material),
                                                  parameter_index) != 0:
                    raise ValueError('material sampler did not validate')
                item['image'] = {'index': reference.index,
                                 'id': audit_cache.decode(reference.id),
                                 'name': audit_cache.decode(reference.name),
                                 'source_path': audit_cache.decode(reference.source_path)}
            elif parameter.type_code == 7 and parameter.value_count == 1:
                raw_value = bytes.fromhex(item['raw_prefix_hex'])
                if len(raw_value) >= 16:
                    item['first_four_f32'] = list(struct.unpack('<4f', raw_value[:16]))
            record['parameters'].append(item)
        materials.append(record)
    effects = []
    for index in range(dll.dh2_bres_library_count(c.byref(view), 5)):
        effects.append(describe_effect(dll, view, index))
    return {'path': path.as_posix(), 'bytes': len(raw),
            'sha256': hashlib.sha256(raw).hexdigest(),
            'library_counts': {str(index): dll.dh2_bres_library_count(c.byref(view), index)
                               for index in range(13)},
            'effects': effects, 'selected_materials': materials}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', type=Path, required=True)
    parser.add_argument('--library', type=Path, required=True,
                        help='compiled port/material-bindings library')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    dll = audit_cache.bind(args.library)
    scene = args.cache / 'data/3d/modules/swamp/swamp.bdae'
    local_effect = parse_bres(dll, scene, ('Standard_7', 'Standard_19',
                                           'Material__11598', 'Material__11610',
                                           'Material__11611'))
    external_path = args.cache / 'data/gfx/effects/gl_diffuse_l1_vc_iphone.bdae'
    external_effect = parse_bres(dll, external_path)
    output = {'scope': 'checked serialized BRES material/effect records; no shader execution',
              'source_scene': local_effect, 'external_effect': external_effect}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(output, indent=2) + '\n', encoding='utf-8')
    print(args.output.resolve())


if __name__ == '__main__':
    main()
