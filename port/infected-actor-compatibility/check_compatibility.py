#!/usr/bin/env python3
"""Check cache-backed infected model and animation skeleton compatibility.

The checker reuses the repository's BRES, scene, animation, skin and pose
readers. It reports exact node-target matches and exercises the bounded pose
and skin palette readers. It does not guess an animation for an unmapped state.
"""
import argparse
from collections import Counter
import ctypes as c
import hashlib
import json
from pathlib import Path
import struct
import sys

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[1]
POSE_TESTS = REPO / 'port/animation-pose/tests'
sys.path.insert(0, str(POSE_TESTS))
import check as pose_api

skin_api = pose_api.skin
scene_api = skin_api.scene
assets_api = pose_api.assets

U, I, P = c.c_uint32, c.c_int32, c.c_void_p
LIBRARY_CONTROLLER = 8
ERRORS = {
    0: 'ok', 1: 'argument', 2: 'range', 3: 'unsupported', 4: 'nonfinite',
    5: 'keys', 6: 'duplicate', 7: 'scene_or_track_target', 8: 'joint',
    9: 'limit',
}


def bind_extras(dll):
    funcs = {
        'dh2_bres_library_count': (U, [c.POINTER(scene_api.Bres), U]),
        'dh2_bres_library_item': (P, [c.POINTER(scene_api.Bres), U, I]),
        'dh2_animation_segments': (U, [c.POINTER(scene_api.Bres)]),
    }
    for name, (result, args) in funcs.items():
        fn = getattr(dll, name)
        fn.restype, fn.argtypes = result, args


def sha(data):
    return hashlib.sha256(data).hexdigest()


def as_assets_bres(image):
    """Rewrap the same borrowed BRES view in asset-payloads' ctypes layout."""
    return assets_api.Bres.from_buffer_copy(c.string_at(c.byref(image), c.sizeof(image)))


def cstr(address):
    return c.string_at(address).decode('utf-8', errors='replace') if address else None


def cache_file(cache, relative):
    path = cache / Path(relative)
    if not path.is_file():
        raise FileNotFoundError(f'manifest asset is missing from cache: {relative}')
    return path


def controller_inventory(dll, image, raw):
    count = dll.dh2_bres_library_count(c.byref(image), LIBRARY_CONTROLLER)
    values = []
    for index in range(count):
        record = dll.dh2_bres_library_item(c.byref(image), LIBRARY_CONTROLLER, index)
        if not record:
            continue
        # Original controller record: type at +0, ID string offset at +4,
        # skin payload offset at +8 (see dh2_skin_open).
        string_offset = struct.unpack('<I', c.string_at(record + 4, 4))[0]
        name = cstr(image.bytes + string_offset) if string_offset < len(raw) else None
        values.append({'index': index, 'id': name})
    return values


def source_file_pin(manifest_assets, relative_path, raw):
    expected = manifest_assets.get(relative_path)
    actual_sha = sha(raw)
    return {
        'manifest_size_bytes': expected.get('size_bytes') if expected else None,
        'manifest_sha256': expected.get('sha256') if expected else None,
        'manifest_pin_present': expected is not None,
        'manifest_pin_matches': bool(expected and expected.get('size_bytes') == len(raw)
                                     and expected.get('sha256') == actual_sha),
    }


def read_model(dll, path, cache_relative_path, expected_controller, manifest_assets):
    storage, image, raw = pose_api.image(dll, path)
    controllers = controller_inventory(dll, image, raw)
    matches = [v for v in controllers if v['id'] == expected_controller]
    model = {
        'path': cache_relative_path, 'sha256': sha(raw), 'bytes': len(raw),
        'controller_count': len(controllers), 'controllers': controllers,
        'expected_controller_id': expected_controller,
        'controller_match_count': len(matches),
        **source_file_pin(manifest_assets, cache_relative_path, raw),
    }
    if len(matches) != 1:
        model['status'] = 'controller_id_unresolved_or_ambiguous'
        return model, storage, image, None, None, None
    skin = skin_api.Skin()
    skin_error = dll.dh2_skin_open(c.byref(skin), c.byref(image), matches[0]['index'])
    if skin_error:
        model.update(status='skin_open_failed', skin_error=int(skin_error))
        return model, storage, image, None, None, None
    scene = scene_api.Scene()
    scene_error = dll.dh2_scene_open(c.byref(scene), c.byref(image))
    if scene_error or scene.visuals == 0:
        model.update(status='scene_open_failed', scene_error=int(scene_error), visual_count=int(scene.visuals))
        return model, storage, image, skin, None, None
    visual = scene_api.Visual()
    visual_error = dll.dh2_scene_visual(c.byref(scene), 0, c.byref(visual))
    if visual_error:
        model.update(status='visual_open_failed', visual_error=int(visual_error))
        return model, storage, image, skin, scene, None
    node_ids = Counter()
    scopes = Counter()
    node_count = 0
    callback_errors = []

    @scene_api.VISIT
    def visit(node_ptr, _matrix, _depth, _user):
        nonlocal node_count
        node = node_ptr.contents
        node_count += 1
        node_ids[cstr(node.id)] += 1
        offset = node.record + 8
        if offset + 4 > len(raw):
            callback_errors.append({'record': int(node.record), 'error': 'scope_pointer_out_of_range'})
            return False
        scope_offset = struct.unpack_from('<I', raw, offset)[0]
        if scope_offset >= len(raw):
            callback_errors.append({'record': int(node.record), 'error': 'scope_string_out_of_range'})
            return False
        scopes[cstr(image.bytes + scope_offset)] += 1
        return True

    walk_error = dll.dh2_scene_walk_visual(c.byref(visual), visit, None, 20000)
    joints = [cstr(dll.dh2_skin_joint_name(c.byref(skin), j)) for j in range(skin.joints)]
    missing_joints = [name for name in joints if scopes[name] == 0]
    duplicate_joints = [name for name in joints if scopes[name] > 1]
    model.update({
        'status': 'ok' if walk_error == 0 and not callback_errors else 'scene_walk_failed',
        'skin_error': 0, 'visual_count': int(scene.visuals), 'visual_id': cstr(visual.id),
        'node_count': node_count, 'node_ids_unique_count': len(node_ids),
        'skin_joint_count': int(skin.joints), 'skin_vertex_count': int(skin.vertices),
        'skin_joint_scope_missing': missing_joints,
        'skin_joint_scope_duplicated': duplicate_joints,
        'skin_scope_check_complete': walk_error == 0 and not callback_errors,
        'scene_walk_error': int(walk_error), 'scene_walk_detail': callback_errors,
    })
    if storage.raw[:len(raw)] != raw:
        raise AssertionError(f'reader modified model bytes: {cache_relative_path}')
    return model, storage, image, skin, visual, {'node_ids': node_ids, 'scopes': scopes}


def build_clip_specs(manifest):
    anim = manifest['animation']
    by_id = {row['id']: row for row in anim['anim_tpls']}
    states = {row['state']: row['anim_tpl_id'] for row in anim['states']}
    wanted = [('Idle', states['Idle']), ('Walk', states['Walk']),
              ('Attack', states['Attack']), ('Stunned-talk', states['Stunned'])]
    specs = []
    for label, table_id in wanted:
        row = by_id[table_id]
        paths = row['anim_dict_paths']
        if label == 'Stunned-talk':
            paths = [p for p in paths if p.endswith('/cs_madruk_intro_rene_scene07_talk.bdae')]
        if label == 'Stunned-talk' and not paths:
            raise ValueError('exact NPC-talk Stunned animation path is absent from cache-derived manifest')
        for path in paths:
            specs.append({
                'state': label, 'state_table_name': row['name'],
                'anim_tpl_id': table_id, 'path': path,
            })
    return specs


def check_clip_file(dll, assets, cache, spec, model, model_detail, manifest_assets):
    path = cache_file(cache, spec['path'])
    storage, image, raw = pose_api.image(dll, path)
    asset_image = as_assets_bres(image)
    segment_count = int(assets.dh2_animation_segments(c.byref(asset_image)))
    animation_count = int(dll.dh2_bres_library_count(c.byref(image), 0))
    output = {
        **spec, 'sha256': sha(raw), 'bytes': len(raw), 'bres_status': 'ok',
        **source_file_pin(manifest_assets, spec['path'], raw),
        'segment_count': segment_count, 'animation_record_count': animation_count,
        'segments': [],
    }
    for segment in range(segment_count):
        targets, errors = [], []
        for track_index in range(animation_count):
            animation = assets_api.Animation()
            open_error = assets.dh2_animation_open(c.byref(animation), c.byref(asset_image), track_index, segment)
            if open_error:
                errors.append({'track_index': track_index, 'stage': 'animation_open', 'error': int(open_error)})
                continue
            target_ptr = assets.dh2_animation_target(c.byref(animation))
            target = cstr(target_ptr)
            match_count = model_detail['node_ids'][target] if target is not None else 0
            targets.append({'track_index': track_index, 'target': target, 'node_match_count': match_count})
        clip = pose_api.Clip()
        clip_error = int(dll.dh2_pose_clip_open(c.byref(clip), c.byref(image), segment))
        segment_result = {
            'segment': segment, 'track_target_count': len(targets), 'targets': targets,
            'track_read_errors': errors, 'pose_clip_open': ERRORS.get(clip_error, str(clip_error)),
            'pose_clip_error_code': clip_error,
            'exactly_matched_track_count': sum(row['node_match_count'] == 1 for row in targets),
            'unresolved_targets': [row for row in targets if row['node_match_count'] == 0],
            'multiply_matched_targets': [row for row in targets if row['node_match_count'] > 1],
        }
        segment_result['skin_joint_scope_missing'] = model['skin_joint_scope_missing']
        segment_result['skin_joint_scope_duplicated'] = model['skin_joint_scope_duplicated']
        if clip_error == 0:
            times = sorted({int(clip.start), (int(clip.start) + int(clip.end)) // 2, int(clip.end)})
            segment_result['clip_start_ms'] = int(clip.start)
            segment_result['clip_end_ms'] = int(clip.end)
            segment_result['positive_duration'] = int(clip.end) > int(clip.start)
            segment_result['sample_times_ms'] = times
            palette_results = []
            for time in times:
                palette = (scene_api.Matrix * model_detail['skin'].joints)()
                error = int(dll.dh2_pose_skin_palette(
                    c.byref(clip), time, c.byref(model_detail['skin']),
                    c.byref(model_detail['visual']), palette, model_detail['skin'].joints))
                palette_results.append({'time_ms': time, 'status': ERRORS.get(error, str(error)), 'error_code': error})
            segment_result['palette_samples'] = palette_results
            segment_result['palette_sample_pass_count'] = sum(r['error_code'] == 0 for r in palette_results)
        else:
            segment_result['clip_start_ms'] = None
            segment_result['clip_end_ms'] = None
            segment_result['positive_duration'] = False
            segment_result['palette_samples'] = []
            segment_result['palette_sample_pass_count'] = 0
        segment_result['fully_compatible'] = bool(
            model['status'] == 'ok'
            and not model['skin_joint_scope_missing']
            and not model['skin_joint_scope_duplicated']
            and not errors and not segment_result['unresolved_targets']
            and not segment_result['multiply_matched_targets']
            and clip_error == 0
            and segment_result['palette_sample_pass_count'] == len(segment_result['palette_samples'])
        )
        output['segments'].append(segment_result)
    if storage.raw[:len(raw)] != raw:
        raise AssertionError(f'reader modified animation bytes: {spec["path"]}')
    output['segment_fully_compatible_count'] = sum(s['fully_compatible'] for s in output['segments'])
    output['status'] = 'compatible' if output['segment_fully_compatible_count'] == segment_count and segment_count else 'mismatch_or_reader_limit'
    return output


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', type=Path, required=True, help='Root of extracted private cache files')
    parser.add_argument('--library', type=Path, required=True, help='Host library built by port/animation-pose/build.py')
    parser.add_argument('--manifest', type=Path, default=REPO/'port/level-runtime/infected-village-actor-assets.json')
    parser.add_argument('--report', type=Path, required=True)
    args = parser.parse_args()
    cache = args.cache.resolve()
    manifest = json.loads(args.manifest.read_text(encoding='utf-8'))
    manifest_assets = {row['path']: row for row in manifest['asset_files']}
    dll = pose_api.bind(args.library)
    bind_extras(dll)
    assets = assets_api.bind(args.library)

    models = []
    model_keepalive = []
    model_details = {}
    for variant in manifest['template_choices']['models']:
        path = cache_file(cache, variant['bdae_path'])
        model, storage, image, skin, visual, detail = read_model(
            dll, path, variant['bdae_path'], variant['skin_controller_id'], manifest_assets)
        model.update({'character_table_id': variant['character_table_id'],
                      'character_name': variant['character_name'], 'model_key': variant['model_key']})
        models.append(model)
        model_keepalive.append((storage, image))
        if skin is not None and visual is not None and detail is not None:
            detail.update({'skin': skin, 'visual': visual})
            model_details[variant['model_key']] = detail

    clip_specs = build_clip_specs(manifest)
    clip_files = []
    pair_records = []
    for clip_spec in clip_specs:
        # Segment enumeration and target-node matching are clip-file facts;
        # palette sampling below binds each segment independently to each model.
        path = cache_file(cache, clip_spec['path'])
        storage, image, raw = pose_api.image(dll, path)
        asset_image = as_assets_bres(image)
        segment_count = int(assets.dh2_animation_segments(c.byref(asset_image)))
        animation_count = int(dll.dh2_bres_library_count(c.byref(image), 0))
        clip_files.append({**clip_spec, 'sha256': sha(raw), 'bytes': len(raw),
                           **source_file_pin(manifest_assets, clip_spec['path'], raw),
                           'segment_count': segment_count, 'animation_record_count': animation_count})
        for variant in manifest['template_choices']['models']:
            model = next(row for row in models if row['model_key'] == variant['model_key'])
            detail = model_details.get(variant['model_key'])
            if detail is None:
                pair_records.append({**clip_spec, 'model_key': variant['model_key'],
                                     'status': 'model_unresolved', 'segments': []})
                continue
            result = check_clip_file(dll, assets, cache, clip_spec, model, detail, manifest_assets)
            result['model_key'] = variant['model_key']
            result['character_name'] = variant['character_name']
            result['model_sha256'] = model['sha256']
            pair_records.append(result)

    summary = Counter()
    by_clip_file = {}
    for record in pair_records:
        if record.get('segments'):
            summary['model_clip_file_pairs'] += 1
            summary['compatible_model_clip_file_pairs'] += int(record.get('status') == 'compatible')
            summary['clip_segments_tested'] += len(record['segments'])
            summary['fully_compatible_segments'] += sum(s.get('fully_compatible', False) for s in record['segments'])
            summary['unresolved_target_occurrences'] += sum(len(s.get('unresolved_targets', [])) for s in record['segments'])
            summary['multiply_matched_target_occurrences'] += sum(len(s.get('multiply_matched_targets', [])) for s in record['segments'])
            summary['palette_sample_passes'] += sum(s.get('palette_sample_pass_count', 0) for s in record['segments'])
            summary['palette_sample_attempts'] += sum(len(s.get('palette_samples', [])) for s in record['segments'])
            key = (record['state'], record['path'])
            grouped = by_clip_file.setdefault(key, {
                'state_table_name': record['state_table_name'],
                'anim_tpl_id': record['anim_tpl_id'], 'path': record['path'],
                'model_pairs_tested': 0, 'compatible_model_pairs': 0,
                'segments_tested': 0, 'fully_compatible_segments': 0,
                'unresolved_target_occurrences': 0,
                'unresolved_target_names': set(),
            })
            grouped['model_pairs_tested'] += 1
            grouped['compatible_model_pairs'] += int(record.get('status') == 'compatible')
            grouped['segments_tested'] += len(record['segments'])
            grouped['fully_compatible_segments'] += sum(s.get('fully_compatible', False) for s in record['segments'])
            for segment in record['segments']:
                missing = segment.get('unresolved_targets', [])
                grouped['unresolved_target_occurrences'] += len(missing)
                grouped['unresolved_target_names'].update(row['target'] for row in missing if row.get('target'))
    grouped_rows = []
    for grouped in by_clip_file.values():
        grouped['unresolved_target_names'] = sorted(grouped['unresolved_target_names'])
        grouped_rows.append(grouped)
    hash_mismatches = [
        {'path': row['path'], 'sha256': row['sha256'], 'manifest_sha256': row['manifest_sha256']}
        for row in [*models, *clip_files] if not row.get('manifest_pin_matches', False)
    ]
    result = {
        'format': 'dh2.infected-actor-compatibility.v1',
        'scope': 'Six cache-derived InfectedVillage_CommonType1 models; exact Infected CharAnimTable Idle/Walk mappings plus its two Attack and NPC-talk Stunned mapping files.',
        'complete_game': False, 'original_animator_equivalence': False,
        'limbus_clip_inferred': False,
        'source_manifest': args.manifest.resolve().relative_to(REPO).as_posix(),
        'source_manifest_sha256': sha(args.manifest.read_bytes()),
        'cache_root_not_embedded': True,
        'library_path': args.library.resolve().relative_to(REPO).as_posix(),
        'library_sha256': sha(args.library.read_bytes()),
        'source_character_records': [
            {
                'source_record': row['source_record'],
                'name': row['name'],
                'char_template_pydata': row['attributes'].get('char_template_pydata'),
                'template_name': row['attributes'].get('char_template'),
                'template_name_field': row['attributes'].get('_templateName'),
                'ai_state': row['attributes'].get('ai_state'),
                'auto_spawn': row['attributes'].get('auto_spawn'),
            }
            for row in manifest['source_records']['records']
        ],
        'table_evidence': {
            'template_name': manifest['source_records']['shared_template']['template_name'],
            'source_level_module': manifest['source_records']['source_path'],
            'animation_table': manifest['animation']['table'],
            'animation_table_id': manifest['animation']['table_id'],
            'positive_state_mapping_count_in_source_manifest': manifest['animation']['positive_state_mapping_count'],
            'tested_states': [{'state': s, 'anim_tpl_id': i,
                               'anim_tpl_name': next(a['name'] for a in manifest['animation']['anim_tpls'] if a['id'] == i)}
                              for s, i in [('Idle', 321), ('Walk', 328), ('Attack', 315), ('Stunned-talk', 326)]],
            'unmapped_state_slots': manifest['animation']['unmapped_state_slots'],
        },
        'models': models, 'clip_files': clip_files,
        'source_asset_hash_mismatches': hash_mismatches,
        'summary': dict(summary), 'clip_file_summary': grouped_rows,
        'model_clip_results': pair_records,
        'cache_input_preservation': True,
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'report': str(args.report.resolve()), 'models': len(models),
                      'clip_files': len(clip_files), **result['summary']}, sort_keys=True))


if __name__ == '__main__':
    main()
