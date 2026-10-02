#!/usr/bin/env python3
"""Check static draw descriptors across the private complete BRES cache."""

from __future__ import annotations

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
    _fields_ = [('bytes', P), ('size', c.c_size_t)] + [(name, U) for name in (
        'fixup_count', 'fixup_offset', 'root_offset', 'tail_offset',
        'bulk_size', 'block_count', 'tail_size')]


class Matrix(c.Structure):
    _fields_ = [('m', c.c_float * 16), ('identity_hint', c.c_uint8)]


class Command(c.Structure):
    _fields_ = [('world', Matrix), ('node_id', P), ('geometry_id', P),
                ('material_id', P), ('visual_index', U), ('node_record', U),
                ('visible', U), ('vertex_count', U), ('index_count', U),
                ('index_width', U), ('geometry_index', I),
                ('primitive_index', I), ('material_index', I)]


class Stats(c.Structure):
    _fields_ = [(name, U) for name in (
        'visual_references', 'visual_scenes', 'nodes', 'instances',
        'geometry_instances', 'resolved_geometry', 'skipped_nonvisual_references',
        'skipped_unresolved_visuals', 'skipped_unresolved_geometry',
        'skipped_unsupported_geometry', 'skipped_nontriangle_primitives',
        'unresolved_materials', 'draw_commands')] + [('triangles', c.c_uint64)]


VISIT = c.CFUNCTYPE(c.c_bool, c.POINTER(Command), P)


def bind(library: Path):
    dll = c.CDLL(str(library.resolve()))
    dll.dh2_bres_open.restype = U
    dll.dh2_bres_open.argtypes = [c.POINTER(Bres), P, c.c_size_t]
    dll.dh2_static_scene_draws.restype = U
    dll.dh2_static_scene_draws.argtypes = [c.POINTER(Stats), c.POINTER(Bres),
                                          VISIT, P, U, U]
    return dll


def view(dll, raw: bytes):
    storage = c.create_string_buffer(raw)
    image = Bres()
    assert dll.dh2_bres_open(c.byref(image), storage, len(raw)) == 0
    return storage, image


def check_file(dll, raw: bytes):
    storage, image = view(dll, raw)
    stats = Stats()
    commands = 0
    triangles = 0
    translated = 0
    sampled = []

    @VISIT
    def receive(pointer, _):
        nonlocal commands, triangles, translated
        command = pointer.contents
        assert command.geometry_index >= 0 and command.primitive_index >= 0
        assert command.vertex_count > 0 and command.index_count % 3 == 0
        assert command.index_width in (2, 4)
        assert c.string_at(command.node_id) and c.string_at(command.geometry_id)
        assert c.string_at(command.material_id)
        values = tuple(command.world.m)
        assert all(math.isfinite(value) for value in values)
        assert (values[3], values[7], values[11], values[15]) == (0, 0, 0, 1)
        translated += values[12:15] != (0, 0, 0)
        commands += 1
        triangles += command.index_count // 3
        if len(sampled) < 2:
            sampled.append({'geometry': c.string_at(command.geometry_id).decode('utf-8'),
                            'material': c.string_at(command.material_id).decode('utf-8'),
                            'world_translation': list(values[12:15]),
                            'vertices': command.vertex_count,
                            'triangles': command.index_count // 3})
        return True

    status = dll.dh2_static_scene_draws(c.byref(stats), c.byref(image), receive,
                                        None, 200000, 200000)
    assert status == 0, status
    assert stats.draw_commands == commands and stats.triangles == triangles
    assert stats.geometry_instances == (stats.resolved_geometry +
                                        stats.skipped_unresolved_geometry)
    assert stats.unresolved_materials <= stats.draw_commands
    assert storage.raw[:len(raw)] == raw
    return stats, translated, sampled


def bounded(dll, raw: bytes, stats: Stats):
    storage, image = view(dll, raw)

    @VISIT
    def keep_going(_command, _user):
        return True

    @VISIT
    def stop(_command, _user):
        return False

    if stats.draw_commands > 1:
        result = dll.dh2_static_scene_draws(c.byref(Stats()), c.byref(image),
                                             keep_going, None, 200000, 1)
        assert result == 9, result  # draw_limit
    if stats.nodes > 1:
        result = dll.dh2_static_scene_draws(c.byref(Stats()), c.byref(image),
                                             keep_going, None, 1, 200000)
        assert result == 8, result  # node_limit
    if stats.draw_commands:
        result = dll.dh2_static_scene_draws(c.byref(Stats()), c.byref(image),
                                             stop, None, 200000, 200000)
        assert result == 10, result  # callback_stopped
    assert dll.dh2_static_scene_draws(c.byref(Stats()), c.byref(image),
                                      keep_going, None, 0, 1) == 1
    assert storage.raw[:len(raw)] == raw


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', required=True, type=Path)
    parser.add_argument('--library', required=True, type=Path)
    parser.add_argument('--report', required=True, type=Path)
    args = parser.parse_args()
    dll = bind(args.library)
    totals = Counter()
    samples = []
    manifest = hashlib.sha256()
    bounded_done = False
    for path in sorted(args.cache.rglob('*.bdae')):
        raw = path.read_bytes()
        relative = path.relative_to(args.cache).as_posix()
        manifest.update(relative.encode('utf-8') + b'\0')
        manifest.update(hashlib.sha256(raw).digest())
        try:
            stats, translated, sampled = check_file(dll, raw)
        except AssertionError as error:
            raise AssertionError(f'{relative}: {error}') from error
        totals['files'] += 1
        totals['translated_draw_commands'] += translated
        for name, _ in Stats._fields_:
            totals[name] += getattr(stats, name)
        if stats.draw_commands and len(samples) < 4:
            samples.append({'file': relative, 'commands': sampled})
        if not bounded_done and stats.draw_commands > 1 and stats.nodes > 1:
            bounded(dll, raw, stats)
            bounded_done = True
    assert bounded_done
    report = {
        'scope': 'all original-cache BRES files; static scene/geometry/material draw descriptors only',
        'all_checks_passed': True,
        'bounds_and_callback_checks_passed': bounded_done,
        'totals': dict(sorted(totals.items())),
        'samples': samples,
        'cache_manifest_sha256': manifest.hexdigest(),
        'library_sha256': hashlib.sha256(args.library.read_bytes()).hexdigest(),
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(report, indent=2))


if __name__ == '__main__':
    main()
