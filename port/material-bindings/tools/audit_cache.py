#!/usr/bin/env python3
"""Check BRES image/material strings and local effect links on a cache corpus."""

from __future__ import annotations

import argparse
from collections import Counter
import ctypes as c
import hashlib
import json
from pathlib import Path


U = c.c_uint32
I = c.c_int32
P = c.c_void_p


class Bres(c.Structure):
    _fields_ = [('bytes', P), ('size', c.c_size_t)] + [
        (name, U) for name in ('fixup_count', 'fixup_offset', 'root_offset',
                               'tail_offset', 'bulk_size', 'block_count', 'tail_size')
    ]


class Image(c.Structure):
    _fields_ = [('image', Bres), ('id', P), ('name', P), ('source_path', P),
                ('raw_word_12', U), ('raw_word_16', U)]


class Material(c.Structure):
    _fields_ = [('image', Bres), ('id', P), ('name', P),
                ('external_effect_file', P), ('effect_url', P), ('record', P),
                ('parameter_count', U), ('parameter_records', P)]


class Effect(c.Structure):
    _fields_ = [('image', Bres), ('id', P), ('name', P), ('record', P)]


class EffectGroup(c.Structure):
    _fields_ = [('image', Bres), ('named_count', U), ('parameter_count', U),
                ('image_count', U), ('named_records', P),
                ('parameter_records', P), ('image_indices', P)]


class Parameter(c.Structure):
    _fields_ = [('image', Bres), ('id', P), ('semantic', P),
                ('type_code', U), ('value_count', U), ('raw_value', P)]


class EffectParameter(c.Structure):
    _fields_ = [('image', Bres), ('id', P), ('type_code', U),
                ('raw_word_8', U), ('value_count', U), ('raw_value', P)]


class ImageRef(c.Structure):
    _fields_ = [('index', I), ('id', P), ('name', P), ('source_path', P)]


def decode(pointer: int | None) -> str | None:
    return c.string_at(pointer).decode('utf-8') if pointer else None


def bind(path: Path):
    dll = c.CDLL(str(path.resolve()))
    for name, result, arguments in (
        ('dh2_bres_open', U, [c.POINTER(Bres), P, c.c_size_t]),
        ('dh2_bres_library_count', U, [c.POINTER(Bres), U]),
        ('dh2_image_record', U, [c.POINTER(Image), c.POINTER(Bres), I]),
        ('dh2_effect_record', U, [c.POINTER(Effect), c.POINTER(Bres), I]),
        ('dh2_material_record', U, [c.POINTER(Material), c.POINTER(Bres), I]),
        ('dh2_material_local_effect', I, [c.POINTER(Material)]),
        ('dh2_effect_group', U, [c.POINTER(EffectGroup), c.POINTER(Effect), I]),
        ('dh2_effect_parameter', U, [c.POINTER(EffectParameter), c.POINTER(EffectGroup), I]),
        ('dh2_effect_group_image', U, [c.POINTER(ImageRef), c.POINTER(EffectGroup), I]),
        ('dh2_material_parameter', U, [c.POINTER(Parameter), c.POINTER(Material), I]),
        ('dh2_material_sampler_image', U, [c.POINTER(ImageRef), c.POINTER(Material), I]),
    ):
        function = getattr(dll, name)
        function.restype = result
        function.argtypes = arguments
    return dll


def audit(dll, cache: Path) -> dict:
    textures = {path.name.casefold() for path in
                (cache / 'data/3d/textures').iterdir() if path.is_file()}
    bres_names = {path.name.casefold() for path in cache.rglob('*.bdae')}
    totals: Counter[str] = Counter()
    missing: Counter[str] = Counter()
    digest = hashlib.sha256()
    for path in sorted(cache.rglob('*.bdae')):
        raw = path.read_bytes()
        relative = path.relative_to(cache).as_posix()
        digest.update(relative.encode('utf-8') + b'\0' + hashlib.sha256(raw).digest())
        buffer = c.create_string_buffer(raw)
        view = Bres()
        assert dll.dh2_bres_open(c.byref(view), buffer, len(raw)) == 0, relative
        totals['bres_files'] += 1
        for index in range(dll.dh2_bres_library_count(c.byref(view), 4)):
            image = Image()
            assert dll.dh2_image_record(c.byref(image), c.byref(view), index) == 0, (relative, index)
            assert decode(image.id) and decode(image.name), (relative, index)
            source = decode(image.source_path)
            assert source, (relative, index)
            assert image.raw_word_12 == 0 and image.raw_word_16 == 0, (relative, index)
            totals['image_records'] += 1
            filename = source.replace('\\', '/').split('/')[-1]
            if filename.casefold() in textures:
                totals['exact_texture_filename_matches'] += 1
            else:
                totals['unmatched_texture_references'] += 1
                missing[filename] += 1
                if ('pvr2_' + filename).casefold() in textures:
                    totals['pvr2_prefixed_filename_candidates'] += 1
        for index in range(dll.dh2_bres_library_count(c.byref(view), 5)):
            effect = Effect()
            assert dll.dh2_effect_record(c.byref(effect), c.byref(view), index) == 0, (relative, index)
            assert decode(effect.id) and decode(effect.name), (relative, index)
            totals['effect_records'] += 1
            for group_index in range(2):
                group = EffectGroup()
                assert dll.dh2_effect_group(c.byref(group), c.byref(effect), group_index) == 0, (relative, index, group_index)
                totals['effect_named_records'] += group.named_count
                totals['effect_parameter_records'] += group.parameter_count
                for j in range(group.parameter_count):
                    parameter = EffectParameter()
                    assert dll.dh2_effect_parameter(c.byref(parameter), c.byref(group), j) == 0, (relative, index, group_index, j)
                    assert decode(parameter.id) is not None
                for j in range(group.image_count):
                    reference = ImageRef()
                    assert dll.dh2_effect_group_image(c.byref(reference), c.byref(group), j) == 0, (relative, index, group_index, j)
                    totals['effect_image_references'] += 1
                    if reference.index == -1:
                        totals['effect_unbound_image_references'] += 1
                    else:
                        assert decode(reference.id) and decode(reference.source_path)
                        totals['effect_bound_image_references'] += 1
        for index in range(dll.dh2_bres_library_count(c.byref(view), 6)):
            material = Material()
            assert dll.dh2_material_record(c.byref(material), c.byref(view), index) == 0, (relative, index)
            assert decode(material.id) and decode(material.name), (relative, index)
            url = decode(material.effect_url)
            assert url and url.startswith('#'), (relative, index)
            totals['material_records'] += 1
            totals['material_parameter_records'] += material.parameter_count
            sampler_indices = set()
            for j in range(material.parameter_count):
                parameter = Parameter()
                assert dll.dh2_material_parameter(c.byref(parameter), c.byref(material), j) == 0, (relative, index, j)
                assert decode(parameter.id) is not None and decode(parameter.semantic) is not None
                if parameter.type_code == 11:
                    reference = ImageRef()
                    assert dll.dh2_material_sampler_image(c.byref(reference), c.byref(material), j) == 0, (relative, index, j)
                    sampler_indices.add(reference.index)
                    totals['material_sampler_references'] += 1
                    if reference.index == -1:
                        totals['material_unbound_sampler_references'] += 1
                    else:
                        assert decode(reference.id) and decode(reference.source_path)
                        totals['material_bound_sampler_references'] += 1
            effect_index = dll.dh2_material_local_effect(c.byref(material))
            if material.external_effect_file:
                assert effect_index == -1, (relative, index)
                totals['external_effect_references'] += 1
                if decode(material.external_effect_file).casefold() in bres_names:
                    totals['external_effect_basename_matches'] += 1
            else:
                assert effect_index >= 0, (relative, index)
                effect = Effect()
                assert dll.dh2_effect_record(c.byref(effect), c.byref(view), effect_index) == 0
                assert decode(effect.id) == url[1:], (relative, index)
                for group_index in range(2):
                    group = EffectGroup()
                    assert dll.dh2_effect_group(c.byref(group), c.byref(effect), group_index) == 0
                    group_indices = set()
                    for j in range(group.image_count):
                        reference = ImageRef()
                        assert dll.dh2_effect_group_image(c.byref(reference), c.byref(group), j) == 0
                        group_indices.add(reference.index)
                    assert group_indices == sampler_indices, (relative, index, group_index)
                    totals['local_effect_group_image_set_matches'] += 1
                totals['resolved_local_effects'] += 1
    return {
        'all_checks_passed': True,
        'scope': 'immutable BRES image/effect/material views, bounded nested parameter arrays and serialized image-index links; no shader state or GPU behavior',
        'totals': dict(totals),
        'unique_unmatched_texture_filenames': len(missing),
        'unmatched_texture_samples': missing.most_common(25),
        'bres_manifest_sha256': digest.hexdigest(),
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', type=Path, required=True)
    parser.add_argument('--library', type=Path, required=True)
    parser.add_argument('--report', type=Path, required=True)
    args = parser.parse_args()
    report = audit(bind(args.library), args.cache)
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
