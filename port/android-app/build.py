#!/usr/bin/env python3
"""Build the source renderer APK with the installed Android SDK and NDK."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import xml.etree.ElementTree as ET
import zipfile

HERE = Path(__file__).resolve().parent
REPO = HERE.parent.parent
BOX2D_ROOT = REPO / 'port/physics-backend/box2d-2.0.1'
SOURCES = [
    HERE / 'native.cpp',
    HERE / 'world_renderer.cpp',
    HERE / 'gameplay.cpp',
    HERE / 'scene_buffers.cpp',
    HERE / 'swamp_preview.cpp',
    HERE / 'infected_village_scene.cpp',
    HERE / 'infected_village_preview.cpp',
    HERE / 'infected_actor_preview.cpp',
    HERE / 'swamp_script_jni.cpp',
    REPO / 'port/navigation/navigation.cpp',
    REPO / 'port/floor-types/floor_types.cpp',
    REPO / 'port/swamp-movement/movement.cpp',
    REPO / 'port/world-data/world.cpp',
    REPO / 'port/world-data/world_scene.cpp',
    HERE / 'scripts.cpp',
    HERE / 'pydata_jni.cpp',
    REPO / 'port/pydata-scripts/native/pydata_scripts.cpp',
    REPO / 'port/script-runtime/script_runtime.cpp',
    REPO / 'port/skin-payloads/skin.cpp',
    REPO / 'port/animation-pose/pose.cpp',
    REPO / 'port/animation-values/values.cpp',
    REPO / 'port/animation-timeline/timeline.cpp',
    REPO / 'port/animation-mixing/mixing.cpp',
    REPO / 'port/animation-layers/layers.cpp',
    REPO / 'port/scene-draw/draw.cpp',
    REPO / 'port/scene-payloads/scene.cpp',
    REPO / 'port/asset-payloads/payloads.cpp',
    REPO / 'port/engine-resources/resources.cpp',
    REPO / 'port/engine-math/math.cpp',
    REPO / 'port/material-bindings/bindings.cpp',
    REPO / 'port/texture-assets/texture.cpp',
    REPO / 'port/texture-assets/decode.cpp',
]


def run(*argv: object) -> None:
    subprocess.run([str(a) for a in argv], check=True)


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


NATIVE_APP_GLUE_NOTICE = REPO / 'port/irrlicht-android/notices/native_app_glue-NOTICE.txt'
APACHE2_LICENSE = REPO / 'port/irrlicht-android/notices/Apache-2.0.txt'
NATIVE_APP_GLUE_NOTICE_SHA256 = '396e2d75715e8b4d1029ded55ae92832c8ab9c58148969653d34931248739b95'
APACHE2_LICENSE_SHA256 = 'bb28c48e3e078166e91cfc2b6db7ffebb8a0973b9e23b3df060561292d8d69ec'


IRRLICHT_CACHE_SCENE_ASSETS = {
    'data/3d/modules/void_maze/void_maze.bdae': {
        'apk_path': 'dh2/void_maze.bdae',
        'bytes': 199300,
        'sha256': '7b67b90b65b41de96a5bd5a9a9d9d8f40cdd4425e806057a28ee5299ed2840ba',
        'role': 'local-only BRES source-scene diagnostic',
    },
    'data/3d/textures/env_voidmaze.tga': {
        'apk_path': 'dh2/env_voidmaze.tga',
        'bytes': 32828,
        'sha256': 'aac2c1923d2b9add49d6d8c32211d0af9d1f0a9e022530d551d1dc65d2703c20',
        'role': 'local-only PVRTC source texture diagnostic',
    },
}


def irrlicht_cache_scene_assets(cache_root: Path, assets_root: Path) -> dict:
    """Stage only the pinned local BRES scene and texture in ignored build output."""
    root = cache_root.resolve(strict=True)
    if not root.is_dir():
        raise NotADirectoryError(root)
    if REPO == root or REPO in root.parents or root in REPO.parents:
        raise ValueError('--irrlicht-cache-scene requires an external --cache root outside the repository')
    records = []
    for relative, expected in IRRLICHT_CACHE_SCENE_ASSETS.items():
        source = (root / Path(relative)).resolve(strict=True)
        if root not in source.parents:
            raise ValueError(f'cache scene input resolves outside --cache: {relative}')
        actual = {'bytes': source.stat().st_size, 'sha256': sha(source)}
        if actual != {'bytes': expected['bytes'], 'sha256': expected['sha256']}:
            raise ValueError(f'cache scene input hash/size differs for {relative}: {actual}')
        target = assets_root / expected['apk_path']
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)
        copied = {'bytes': target.stat().st_size, 'sha256': sha(target)}
        if copied != actual:
            raise ValueError(f'cache scene input changed while staging: {relative}')
        records.append({'source': relative, 'apk_asset': expected['apk_path'],
                        'role': expected['role'], **actual})
    manifest_path = assets_root / 'dh2' / 'local-irrlicht-cache-scene-manifest.json'
    manifest_path.write_text(json.dumps({
        'scope': 'local-only cache scene render diagnostic; not a playable level or release asset',
        'source_cache_root': 'external --cache argument (absolute path omitted)',
        'assets': records,
    }, indent=2) + '\n', encoding='utf-8')
    return {'local_only': True, 'release_eligible': False,
            'source_cache_root': 'external --cache argument (absolute path omitted)',
            'assets': records,
            'apk_manifest_asset': 'dh2/local-irrlicht-cache-scene-manifest.json'}


IRRLICHT_SWAMP_ASSETS = {
    'data/3d/modules/swamp/swamp.bdae': 'dh2/swamp.bdae',
    'data/scene/001_swamp.mlx': 'dh2/swamp.mlx',
    'data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp':
        'dh2/swamp-entry-mgp.mgp',
    'data/3d/textures/env_swamp.tga': 'dh2/swamp-diffuse.tga',
    'data/3d/textures/pvr2_env_swamp_alpha.tga': 'dh2/swamp-alpha.tga',
    'data/3d/characters/prince/prince_modular.bdae':
        'dh2/prince-modular.bdae',
    'data/3d/textures/atlas_modular_warrior.tga':
        'dh2/prince-atlas.tga',
}

IRRLICHT_PRINCE_ANIMATION_DATA = (
    'animations_pyarray.bin',
    'animations_pyarraynames.bin',
    'animations_pystructnames.bin',
    'animations_dictionary_pyarraynames.bin',
    'animations_dictionary_pyarray.bin',
    'prince-animation-bank.bin',
    'character_properties_pyarray.bin',
    'character_properties_pyarraynames.bin',
    'character_properties_pystructnames.bin',
    'character_classes_pyarray.bin',
    'character_classes_pyarraynames.bin',
    'character_classes_pystructnames.bin',
)
IRRLICHT_PRINCE_ANIMATION_DATA_ROOT = (
    REPO / 'port/android-native/app/src/main/assets/data')
IRRLICHT_PRINCE_ANIMATION_APK_ROOT = 'dh2/prince-animation'


def irrlicht_swamp_assets(cache_root: Path, assets_root: Path) -> dict:
    """Stage local SWAMP inputs and the checked authored Prince animation bank."""
    root = cache_root.resolve(strict=True)
    if not root.is_dir():
        raise NotADirectoryError(root)
    if REPO == root or REPO in root.parents or root in REPO.parents:
        raise ValueError('--irrlicht-swamp requires an external --cache root outside the repository')
    records = []
    for relative, apk_asset in IRRLICHT_SWAMP_ASSETS.items():
        source = (root / Path(relative)).resolve(strict=True)
        if root not in source.parents:
            raise ValueError(f'SWAMP input resolves outside --cache: {relative}')
        target = assets_root / apk_asset
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)
        identity = {'bytes': source.stat().st_size, 'sha256': sha(source)}
        if {'bytes': target.stat().st_size, 'sha256': sha(target)} != identity:
            raise ValueError(f'SWAMP input changed while staging: {relative}')
        records.append({'source': relative, 'apk_asset': apk_asset,
                        'role': 'local-only original game/cache input', **identity})

    # The PAB1 bank carries authored resource IDs, identities and registration
    # order. Its resource bytes remain a local-cache-only input, but its binary
    # and parser tables are checked source assets already used by the native
    # app. Preserve the bank asset paths below a diagnostic-only APK prefix.
    bank_json_path = IRRLICHT_PRINCE_ANIMATION_DATA_ROOT / 'prince-animation-bank.json'
    bank = json.loads(bank_json_path.read_text(encoding='utf-8'))
    if (bank.get('character') != 'KnightPlayerBase' or
            bank.get('animation_table') != 48 or
            bank.get('animation_set_id') != 12302 or
            bank.get('template_clip_id') != 1111 or
            len(bank.get('resources', [])) != 116 or
            len(bank.get('registration_requests', [])) != 158):
        raise ValueError('checked Prince animation bank metadata identity/counts differ')

    table_records = []
    for name in IRRLICHT_PRINCE_ANIMATION_DATA:
        source = IRRLICHT_PRINCE_ANIMATION_DATA_ROOT / name
        if not source.is_file():
            raise FileNotFoundError(f'checked Prince animation input missing: {source}')
        apk_asset = f'{IRRLICHT_PRINCE_ANIMATION_APK_ROOT}/data/{name}'
        target = assets_root / Path(apk_asset)
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)
        identity = {'bytes': source.stat().st_size, 'sha256': sha(source)}
        if {'bytes': target.stat().st_size, 'sha256': sha(target)} != identity:
            raise ValueError(f'Prince animation input changed while staging: {name}')
        table_records.append({
            'source': source.relative_to(REPO).as_posix(),
            'apk_asset': apk_asset,
            'role': 'checked source animation table or bank metadata', **identity})

    cache_prefix = 'com.gameloft.android.GAND.GloftD2SS/files/'
    resource_records = []
    for resource in bank['resources']:
        cache_entry = resource['cache_entry']
        if not cache_entry.startswith(cache_prefix):
            raise ValueError(f'Prince clip cache entry has an unknown root: {cache_entry}')
        relative = Path(cache_entry[len(cache_prefix):])
        if relative.is_absolute() or '..' in relative.parts:
            raise ValueError(f'Prince clip cache path escapes input root: {cache_entry}')
        source = (root / relative).resolve(strict=True)
        if root not in source.parents:
            raise ValueError(f'Prince clip input resolves outside --cache: {cache_entry}')
        identity = {'bytes': source.stat().st_size, 'sha256': sha(source)}
        if identity != {'bytes': resource['bytes'], 'sha256': resource['sha256']}:
            raise ValueError(f'Prince clip size/hash differs for source id {resource["clip_id"]}')
        asset_relative = Path(resource['asset'])
        if (asset_relative.is_absolute() or '..' in asset_relative.parts or
                not resource['asset'].startswith('animations/')):
            raise ValueError(f'Prince bank asset path is outside animation namespace: {resource["asset"]}')
        apk_asset = f'{IRRLICHT_PRINCE_ANIMATION_APK_ROOT}/{asset_relative.as_posix()}'
        target = assets_root / Path(apk_asset)
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)
        if {'bytes': target.stat().st_size, 'sha256': sha(target)} != identity:
            raise ValueError(f'Prince clip changed while staging: {resource["asset"]}')
        resource_records.append({
            'clip_id': resource['clip_id'],
            'source': relative.as_posix(),
            'bank_asset': resource['asset'],
            'apk_asset': apk_asset,
            'role': 'local-only registered Prince animation resource', **identity})
    if len(resource_records) != len(bank['resources']):
        raise ValueError('Prince bank did not stage every unique registered resource')
    records.extend(resource_records)

    manifest_path = assets_root / 'dh2/local-irrlicht-swamp-manifest.json'
    manifest_path.parent.mkdir(parents=True, exist_ok=True)
    manifest_path.write_text(json.dumps({
        'scope': 'development diagnostic; source-driven SWAMP module-zero rendering and movement plus four Prince warrior skins and authored Character/BlendedPlayback source bank; not full-game parity',
        'source_cache_root': 'external --cache argument (absolute path omitted)',
        'assets': records + table_records,
        'animation_bank': {
            'source': bank_json_path.relative_to(REPO).as_posix(),
            'character': bank['character'],
            'animation_table': bank['animation_table'],
            'animation_set_id': bank['animation_set_id'],
            'template_clip_id': bank['template_clip_id'],
            'unique_resources': len(resource_records),
            'registration_requests': len(bank['registration_requests']),
            'resources': resource_records,
            'checked_inputs': table_records,
        },
    }, indent=2) + '\n', encoding='utf-8')
    asset_notice = (Path(__file__).resolve().parent.parent /
                    'irrlicht-android/swamp-smoke/ASSET-NOTICE.txt')
    target_notice = assets_root / 'dh2/LOCAL-ASSET-NOTICE.txt'
    shutil.copy2(asset_notice, target_notice)
    return {
        'local_only': True,
        'release_eligible': False,
        'source_cache_root': 'external --cache argument (absolute path omitted)',
        'assets': records,
        'animation_bank': {
            'character': bank['character'],
            'animation_table': bank['animation_table'],
            'animation_set_id': bank['animation_set_id'],
            'template_clip_id': bank['template_clip_id'],
            'unique_resources': len(resource_records),
            'registration_requests': len(bank['registration_requests']),
            'resources': resource_records,
            'checked_inputs': table_records,
        },
        'apk_assets': sorted(row['apk_asset'] for row in records + table_records),
        'cache_apk_assets': sorted(row['apk_asset'] for row in records),
        'apk_manifest_asset': 'dh2/local-irrlicht-swamp-manifest.json',
        'apk_asset_notice': 'dh2/LOCAL-ASSET-NOTICE.txt',
    }


ANDROID_XML_NS = '{http://schemas.android.com/apk/res/android}'


def validate_irrlicht_swamp_in_app_manifest(path: Path) -> dict[str, str]:
    """Guard package identity, launcher, and NativeActivity wiring for the opt-in app flavor."""
    baseline = ET.parse(HERE / 'AndroidManifest.xml').getroot()
    variant = ET.parse(path).getroot()
    package_name = baseline.get('package')
    if package_name != 'local.dh2.sourceviewer' or variant.get('package') != package_name:
        raise ValueError('Integrated Irrlicht SWAMP manifest must keep the standard app package')

    def launcher(root: ET.Element) -> str | None:
        application = root.find('application')
        if application is None:
            return None
        for activity in application.findall('activity'):
            actions = {node.get(ANDROID_XML_NS + 'name')
                       for intent in activity.findall('intent-filter')
                       for node in intent.findall('action')}
            categories = {node.get(ANDROID_XML_NS + 'name')
                          for intent in activity.findall('intent-filter')
                          for node in intent.findall('category')}
            if ('android.intent.action.MAIN' in actions and
                    'android.intent.category.LAUNCHER' in categories):
                return activity.get(ANDROID_XML_NS + 'name')
        return None

    expected_launcher = 'local.dh2.sourceviewer.GameplayActivity'
    if launcher(baseline) != expected_launcher or launcher(variant) != expected_launcher:
        raise ValueError('Integrated Irrlicht SWAMP manifest must retain GameplayActivity as launcher')
    application = variant.find('application')
    if application is None or application.get(ANDROID_XML_NS + 'hasCode') != 'true':
        raise ValueError('Integrated Irrlicht SWAMP manifest must retain the Java app code')
    baseline_application = baseline.find('application')
    baseline_activities = {
        activity.get(ANDROID_XML_NS + 'name')
        for activity in baseline_application.findall('activity')
    } if baseline_application is not None else set()
    variant_activities = {
        activity.get(ANDROID_XML_NS + 'name')
        for activity in application.findall('activity')
    }
    if not baseline_activities.issubset(variant_activities):
        raise ValueError('Integrated Irrlicht SWAMP manifest must retain all standard app activities')
    native = next((activity for activity in application.findall('activity')
                   if activity.get(ANDROID_XML_NS + 'name') == 'android.app.NativeActivity'), None)
    if native is None or native.get(ANDROID_XML_NS + 'exported') != 'false':
        raise ValueError('Integrated Irrlicht SWAMP manifest must declare a non-exported NativeActivity')
    native_library = next((metadata.get(ANDROID_XML_NS + 'value')
                           for metadata in native.findall('meta-data')
                           if metadata.get(ANDROID_XML_NS + 'name') == 'android.app.lib_name'), None)
    if native_library != 'dh2irrlicht':
        raise ValueError('Integrated Irrlicht SWAMP NativeActivity must load libdh2irrlicht.so')
    metadata = {item.get(ANDROID_XML_NS + 'name'): item.get(ANDROID_XML_NS + 'value')
                for item in application.findall('meta-data')}
    if not metadata.get('local.dh2.sourceviewer.irrlicht_diagnostic_title') or not metadata.get(
            'local.dh2.sourceviewer.irrlicht_diagnostic_description'):
        raise ValueError('Integrated Irrlicht SWAMP manifest must label the diagnostics launch button')
    return {'package_name': package_name, 'launcher_activity': expected_launcher,
            'native_activity': 'android.app.NativeActivity',
            'native_library': native_library,
            'diagnostic_title': metadata['local.dh2.sourceviewer.irrlicht_diagnostic_title'],
            'diagnostic_description': metadata['local.dh2.sourceviewer.irrlicht_diagnostic_description']}


INFECTED_VILLAGE_MANIFEST_SHA256 = (
    '88e0c556c9c763bccb5e548c63c2a648f1e7da3b5671b84d53ce59c227156fd0')
INFECTED_VILLAGE_BDAE = 'data/3d/modules/infectedvillage/infectedvillage.bdae'
INFECTED_VILLAGE_SOURCE_PATHS = {
    'data/scene/005_infectedvillage.mlx',
    INFECTED_VILLAGE_BDAE,
    'data/3d/modules/infectedvillage/mgp/infected01.mgp',
    'data/3d/modules/infectedvillage/mgp/infected02.mgp',
    'data/3d/modules/infectedvillage/mvp/infected01.mvp',
    'data/3d/modules/infectedvillage/mvp/infected02.mvp',
}
INFECTED_VILLAGE_TEXTURE_PATHS = {
    'data/3d/textures/env_infectedvillage.tga',
    'data/3d/textures/env_infectedvillage_spec.tga',
    'data/3d/textures/pvr2_env_infectedvillage_alpha.tga',
}

INFECTED_ACTOR_MANIFEST = REPO / 'port/level-runtime/infected-village-actor-assets.json'
INFECTED_ACTOR_COMPATIBILITY = REPO / 'port/infected-actor-compatibility/validation.json'
INFECTED_ACTOR_MANIFEST_SHA256 = (
    'd322afdd1f052e771b3fcf697cc5fa4bbd9d930ebc0e8084a82ebc9899136a19')
INFECTED_ACTOR_COMPATIBILITY_SHA256 = (
    'f87698aa63d2fd481b3cd20009a2f55291ea4c48cc218cd1c917270d7ebd52d7')
INFECTED_ACTOR_TEXTURE_PATHS = (
    'data/3d/textures/atlas_dh2_game_objects_001.tga',
    'data/3d/textures/atlas_dh2_game_objects_001_alpha.tga',
    'data/3d/textures/atlas_dh2_game_objects_001_specular.tga',
    'data/3d/textures/atlas_skinned_characters_animdecor_gameobjects_002.tga',
)
INFECTED_ACTOR_CLIPS = (
    {'label': 'Idle A', 'state': 'Idle', 'anim_tpl_id': 321,
     'path': 'data/3d/characters/infected/animations/infected_idle.bdae'},
    {'label': 'Idle B', 'state': 'Idle', 'anim_tpl_id': 321,
     'path': 'data/3d/characters/infected/animations/infected_idle_02.bdae'},
    {'label': 'Walk', 'state': 'Walk', 'anim_tpl_id': 328,
     'path': 'data/3d/characters/infected/animations/infected_walk.bdae'},
)


def infected_actor_preview_bundle(cache: Path, build: Path) -> tuple[dict[str, Path], dict]:
    """Package only hash-pinned infected actor models, compatible clips and textures."""
    cache = cache.resolve(strict=True)
    manifest_raw = INFECTED_ACTOR_MANIFEST.read_bytes()
    manifest_sha = hashlib.sha256(manifest_raw).hexdigest()
    if manifest_sha != INFECTED_ACTOR_MANIFEST_SHA256:
        raise ValueError('Pinned Infected Village actor asset manifest changed')
    compatibility_raw = INFECTED_ACTOR_COMPATIBILITY.read_bytes()
    compatibility_sha = hashlib.sha256(compatibility_raw).hexdigest()
    if compatibility_sha != INFECTED_ACTOR_COMPATIBILITY_SHA256:
        raise ValueError('Pinned Infected actor compatibility evidence changed')
    source = json.loads(manifest_raw)
    compatibility = json.loads(compatibility_raw)
    if (compatibility.get('source_manifest_sha256') != manifest_sha or
            compatibility.get('original_animator_equivalence') is not False or
            compatibility.get('complete_game') is not False):
        raise ValueError('Actor compatibility evidence does not match the source asset manifest')
    models = source.get('template_choices', {}).get('models')
    clips = source.get('asset_files')
    if not isinstance(models, list) or len(models) != 6 or not isinstance(clips, list):
        raise ValueError('Actor source manifest does not have the expected six-model closure')
    model_rows = {row.get('bdae_path'): row for row in models}
    if len(model_rows) != 6:
        raise ValueError('Actor source manifest has duplicate model paths')
    compatibility_models = {row.get('model_key'): row for row in compatibility.get('models', [])}
    compatibility_pairs = {
        (row.get('model_key'), row.get('path')): row
        for row in compatibility.get('model_clip_results', [])
    }
    pinned_files = {row.get('path'): row for row in clips}
    if len(pinned_files) != len(clips):
        raise ValueError('Actor asset closure contains duplicate file paths')
    selected_paths = set(INFECTED_ACTOR_TEXTURE_PATHS)
    actor_models = []
    for model in models:
        key, path = model.get('model_key'), model.get('bdae_path')
        check = compatibility_models.get(key)
        if (not check or check.get('path') != path or check.get('status') != 'ok' or
                check.get('controller_count') != 1 or check.get('controller_match_count') != 1 or
                check.get('expected_controller_id') != model.get('skin_controller_id') or
                check.get('skin_joint_scope_missing') or check.get('skin_joint_scope_duplicated')):
            raise ValueError(f'Actor model/controller compatibility gate failed: {key}')
        selected_paths.add(path)
        actor_models.append({
            'model_key': key,
            'character_name': model['character_name'],
            'character_table_id': model['character_table_id'],
            'path': path,
            'skin_controller_id': model['skin_controller_id'],
        })
    for clip in INFECTED_ACTOR_CLIPS:
        selected_paths.add(clip['path'])
        clip_rows = [row for row in compatibility.get('clip_file_summary', [])
                     if row.get('path') == clip['path']]
        if len(clip_rows) != 1:
            raise ValueError(f'Actor clip compatibility evidence missing or duplicated: {clip["path"]}')
        aggregate = clip_rows[0]
        if (aggregate.get('state_table_name') != 'Infected_' + clip['state'] or
                aggregate.get('anim_tpl_id') != clip['anim_tpl_id'] or
                aggregate.get('model_pairs_tested') != 6 or
                aggregate.get('compatible_model_pairs') != 6 or
                aggregate.get('unresolved_target_occurrences') != 0):
            raise ValueError(f'Actor clip is not proven against all six models: {clip["path"]}')
        for model in actor_models:
            pair = compatibility_pairs.get((model['model_key'], clip['path']))
            if not pair or pair.get('status') != 'compatible' or not pair.get('segments'):
                raise ValueError(f'Actor model/clip pair is not compatible: {model["model_key"]} {clip["path"]}')
            if any(segment.get('fully_compatible') is not True or
                   segment.get('unresolved_targets') or segment.get('multiply_matched_targets') or
                   segment.get('skin_joint_scope_missing') or segment.get('skin_joint_scope_duplicated') or
                   segment.get('palette_sample_pass_count') != len(segment.get('sample_times_ms', []))
                   for segment in pair['segments']):
                raise ValueError(f'Actor model/clip pose samples are incomplete: {model["model_key"]} {clip["path"]}')
    if set(INFECTED_ACTOR_TEXTURE_PATHS) != set(source.get('model_textures', {}).get('paths', [])):
        raise ValueError('Actor texture set no longer matches the six-model source closure')
    if not selected_paths.issubset(set(pinned_files)):
        raise ValueError('Actor preview selection includes a file outside the manifest-pinned closure')

    assets: dict[str, Path] = {}
    file_rows = []
    for relative in sorted(selected_paths):
        row = pinned_files[relative]
        source_path = cache / Path(relative)
        if not source_path.is_file():
            raise FileNotFoundError(f'Missing manifest-pinned actor preview cache file: {relative}')
        source_path = source_path.resolve(strict=True)
        if cache not in source_path.parents:
            raise ValueError(f'Actor preview cache path resolves outside the supplied cache root: {relative}')
        if source_path.stat().st_size != row.get('size_bytes') or sha(source_path) != row.get('sha256'):
            raise ValueError(f'Actor preview cache file differs from pinned source hash: {relative}')
        assets['assets/dh2/infectedactors/' + relative] = source_path
        file_rows.append({'path': relative, 'role': row['role'],
                          'size_bytes': row['size_bytes'], 'sha256': row['sha256']})
    manifest = {
        'format': 'dh2.infected-actor-preview-bundle.v1',
        'scope': 'Render-only diagnostic of six source model choices and source-compatible Idle/Walk clips; not gameplay.',
        'source_asset_manifest_sha256': manifest_sha,
        'compatibility_evidence_sha256': compatibility_sha,
        'compatibility_evidence_source': 'port/infected-actor-compatibility/validation.json',
        'models': actor_models,
        'clips': [{**row, 'segment': 0} for row in INFECTED_ACTOR_CLIPS],
        'textures': list(INFECTED_ACTOR_TEXTURE_PATHS),
        'files': file_rows,
    }
    manifest_file = build / 'infected-actor-preview-manifest.json'
    manifest_file.parent.mkdir(parents=True, exist_ok=True)
    manifest_file.write_text(json.dumps(manifest, indent=2) + '\n')
    assets['assets/dh2/infectedactors/actor-preview-manifest.json'] = manifest_file
    return assets, manifest


def infected_village_bundle(cache: Path, build: Path) -> tuple[dict[str, Path], dict]:
    """Select exact static-preview inputs from cache using the finalized lock."""
    lock = HERE / 'infected-village-asset-lock.json'
    raw = lock.read_bytes()
    actual_lock_sha = hashlib.sha256(raw).hexdigest()
    if actual_lock_sha != INFECTED_VILLAGE_MANIFEST_SHA256:
        raise ValueError('Finalized Infected Village asset manifest lock hash changed')
    manifest = json.loads(raw)
    if manifest.get('level_name') != 'INFECTED_VILLAGE_01' or manifest.get('module_count') != 2:
        raise ValueError('Finalized Infected Village manifest has unexpected level identity')
    rows = manifest.get('files')
    if not isinstance(rows, list):
        raise ValueError('Finalized Infected Village manifest has no file list')
    by_path = {row.get('path'): row for row in rows}
    if len(by_path) != len(rows) or None in by_path:
        raise ValueError('Finalized Infected Village manifest has duplicate or missing paths')
    selected = set(INFECTED_VILLAGE_SOURCE_PATHS)
    for row in rows:
        if 'bdae-material-sampler-texture' not in row.get('roles', []):
            continue
        if any(ref.get('source_bdae') == INFECTED_VILLAGE_BDAE
               for ref in row.get('references', [])):
            selected.add(row['path'])
    selected_textures = selected - INFECTED_VILLAGE_SOURCE_PATHS
    if selected_textures != INFECTED_VILLAGE_TEXTURE_PATHS:
        raise ValueError('Finalized manifest no longer selects the exact three module sampler textures')
    if selected - set(by_path):
        raise ValueError('Finalized Infected Village manifest lacks required source entries')

    assets: dict[str, Path] = {}
    verified = {}
    for relative in sorted(selected):
        row = by_path[relative]
        source = cache / Path(relative)
        if not source.is_file():
            raise FileNotFoundError(f'Original cache lacks manifest-listed preview asset: {relative}')
        if source.stat().st_size != row.get('size_bytes') or sha(source) != row.get('sha256'):
            raise ValueError(f'Original cache asset differs from finalized manifest: {relative}')
        assets['assets/dh2/infectedvillage/' + relative] = source
        verified[relative] = {'sha256': row['sha256'], 'bytes': row['size_bytes']}
    manifest_copy = build / 'infected-village-asset-manifest.json'
    manifest_copy.parent.mkdir(parents=True, exist_ok=True)
    manifest_copy.write_bytes(raw)
    assets['assets/dh2/infectedvillage/asset-manifest.json'] = manifest_copy
    return assets, {
        'level': 'INFECTED_VILLAGE_01',
        'manifest_sha256': actual_lock_sha,
        'manifest_source': lock.relative_to(HERE).as_posix(),
        'packaged_source_asset_count': len(verified),
        'source_assets': verified,
        'sampler_textures': sorted(selected_textures),
        'unresolved_module_sampler_count': sum(
            gap.get('source_bdae') == INFECTED_VILLAGE_BDAE
            for gap in manifest.get('material_texture_resolution', {}).get(
                'unresolved_sampler_references', [])),
    }


def check_elf(path: Path, expected_machine: int) -> dict:
    data = path.read_bytes()
    if data[:6] != b'\x7fELF\x02\x01':
        raise ValueError(f'{path}: expected little-endian ELF64')
    kind, machine = struct.unpack_from('<HH', data, 16)
    if kind != 3 or machine != expected_machine:
        raise ValueError(f'{path}: unexpected ELF type/machine {(kind, machine)}')
    phoff = struct.unpack_from('<Q', data, 32)[0]
    phsize, phcount = struct.unpack_from('<HH', data, 54)
    segments = []
    for i in range(phcount):
        ptype, _, offset, vaddr, _, filesz, _, align = struct.unpack_from(
            '<IIQQQQQQ', data, phoff + i * phsize)
        if ptype == 1:
            if align < 16384 or offset % 16384 != vaddr % 16384:
                raise ValueError(f'{path}: PT_LOAD {i} is not 16 KiB aligned')
            segments.append({'alignment': align, 'offset': offset})
    if not segments:
        raise ValueError(f'{path}: no PT_LOAD segments')
    return {'machine': machine, 'sha256': sha(path), 'bytes': len(data), 'pt_load': segments}


def build_irrlicht_host(ndk: Path, build: Path, *,
                        cache_scene: bool = False,
                        swamp: bool = False,
                        cache_root: Path | None = None,
                        irrlicht_static_root: Path | None = None) -> tuple[dict[str, Path], dict[str, dict]]:
    """Build a separate NativeActivity renderer against the pinned Irrlicht import."""
    engine = REPO / 'port/irrlicht-android'
    upstream = engine / 'upstream'
    static_root = (irrlicht_static_root.resolve() if irrlicht_static_root else
                   engine / 'build/static')
    if sum((bool(cache_scene), bool(swamp))) > 1:
        raise ValueError('Irrlicht build modes are mutually exclusive')
    variant = ('irrlicht-cache-scene' if cache_scene else
               'irrlicht-swamp' if swamp else 'irrlicht-host')
    manifest_path = engine / 'upstream-source-manifest.json'
    manifest = json.loads(manifest_path.read_text(encoding='utf-8'))
    records = {row['path']: row for row in manifest['files']}
    glue = ndk / 'sources/android/native_app_glue'
    host_bin = ndk / 'toolchains/llvm/prebuilt/windows-x86_64/bin'
    clang = host_bin / 'clang++.exe'
    clang_c = host_bin / 'clang.exe'
    readelf = host_bin / 'llvm-readelf.exe'
    sysroot = host_bin.parent / 'sysroot'
    if not all(path.is_file() for path in (clang, clang_c, readelf,
                                           glue / 'android_native_app_glue.c')):
        raise FileNotFoundError('Android NDK clang, llvm-readelf, or native app glue is missing')

    sources = {
        'main': engine / ('cache-scene-smoke/main.cpp' if cache_scene else
                          'swamp-smoke/main.cpp' if swamp else 'game-smoke/main.cpp'),
        'adapter': engine / 'game/scene_mesh_adapter.cpp',
        'glue': glue / 'android_native_app_glue.c',
    }
    extra_sources = []
    if cache_scene:
        if cache_root is None:
            raise ValueError('cache_root is required for the local cache-scene variant')
        extra_sources = [
            HERE / 'scene_buffers.cpp',
            REPO / 'port/skin-payloads/skin.cpp',
            REPO / 'port/animation-pose/pose.cpp',
            REPO / 'port/animation-values/values.cpp',
            REPO / 'port/animation-timeline/timeline.cpp',
            REPO / 'port/animation-mixing/mixing.cpp',
            REPO / 'port/animation-layers/layers.cpp',
            REPO / 'port/scene-draw/draw.cpp',
            REPO / 'port/scene-payloads/scene.cpp',
            REPO / 'port/asset-payloads/payloads.cpp',
            REPO / 'port/engine-resources/resources.cpp',
            REPO / 'port/engine-math/math.cpp',
            REPO / 'port/material-bindings/bindings.cpp',
            REPO / 'port/texture-assets/texture.cpp',
            REPO / 'port/texture-assets/decode.cpp',
        ]
    elif swamp:
        if cache_root is None:
            raise ValueError('cache_root is required for the local SWAMP variant')
        extra_sources = [
            HERE / 'scene_buffers.cpp',
            REPO / 'port/skin-payloads/skin.cpp',
            REPO / 'port/animation-pose/pose.cpp',
            REPO / 'port/animation-values/values.cpp',
            REPO / 'port/animation-mixing/mixing.cpp',
            REPO / 'port/animation-layers/layers.cpp',
            REPO / 'port/navigation/navigation.cpp',
            REPO / 'port/floor-types/floor_types.cpp',
            REPO / 'port/swamp-movement/movement.cpp',
            REPO / 'port/world-data/world.cpp',
            REPO / 'port/world-data/world_scene.cpp',
            REPO / 'port/scene-draw/draw.cpp',
            REPO / 'port/scene-payloads/scene.cpp',
            REPO / 'port/asset-payloads/payloads.cpp',
            REPO / 'port/engine-resources/resources.cpp',
            REPO / 'port/engine-math/math.cpp',
            REPO / 'port/material-bindings/bindings.cpp',
            REPO / 'port/texture-assets/texture.cpp',
            REPO / 'port/texture-assets/decode.cpp',
            REPO / 'port/scene-materials/technique_selector.cpp',
            engine / 'game/prince_actor.cpp',
            engine / 'game/prince_character_runtime.cpp',
            engine / 'game/prince_mesh_adapter.cpp',
            REPO / 'port/level-world/visual_motion.cpp',
            REPO / 'port/level-world/physical_controls.cpp',
            REPO / 'port/level-world/actor_playback.cpp',
            REPO / 'port/level-world/actor_blended_playback.cpp',
            REPO / 'port/level-world/animation_blender.cpp',
            REPO / 'port/level-world/visual_timeline.cpp',
            REPO / 'port/level-world/character_coordinator.cpp',
            REPO / 'port/level-world/character_state.cpp',
            REPO / 'port/level-world/character_timers.cpp',
            REPO / 'port/level-world/character_stance.cpp',
            REPO / 'port/level-world/character_scene.cpp',
            REPO / 'port/level-world/move_state.cpp',
            REPO / 'port/level-world/decor_scene.cpp',
            REPO / 'port/level-world/decor_body_config.cpp',
            REPO / 'port/level-world/floors.cpp',
            REPO / 'port/level-world/floor_source.cpp',
            REPO / 'port/level-world/octree.cpp',
            REPO / 'port/level-world/selector.cpp',
            REPO / 'port/level-world/collision.cpp',
            REPO / 'port/level-world/navigation.cpp',
            REPO / 'port/level-world/navigation_world.cpp',
            REPO / 'port/level-world/navigation_motion.cpp',
            REPO / 'port/level-world/navigation_search.cpp',
            REPO / 'port/level-world/navigation_path.cpp',
            REPO / 'port/level-world/navigation_controller.cpp',
            REPO / 'port/level-world/navigation_heading.cpp',
            REPO / 'port/level-world/navigation_objects.cpp',
            REPO / 'port/level-world/navigation_producers.cpp',
            REPO / 'port/level-world/navigation_avoidance.cpp',
            REPO / 'port/level-world/physical_world.cpp',
            REPO / 'port/level-world/native_body.cpp',
            REPO / 'port/level-world/physical_controls.cpp',
            REPO / 'port/level-world/body_transform.cpp',
            REPO / 'port/level-world/subobjects_update.cpp',
            REPO / 'port/level-world/actor_runtime.cpp',
            REPO / 'port/level-world/actor_rotation.cpp',
            REPO / 'port/level-world/character_body_config.cpp',
            engine / 'game/swamp_actor_floor_bridge.cpp',
            engine / 'game/swamp_actor_session.cpp',
            REPO / 'port/engine-skinning/skinning.cpp',
            REPO / 'port/scene-materials/scene.cpp',
            REPO / 'port/game-data/data.cpp',
            REPO / 'port/game-data/animation_bank.cpp',
            REPO / 'port/game-data/animation_tables.cpp',
            REPO / 'port/game-data/animation_scheduler.cpp',
            REPO / 'port/game-data/animation_selection.cpp',
            REPO / 'port/game-data/class_tables.cpp',
            REPO / 'port/game-data/properties.cpp',
            REPO / 'port/engine-animation/animation_registration.cpp',
            REPO / 'port/engine-animation/animation.cpp',
            REPO / 'port/engine-animation/animation_blend.cpp',
            REPO / 'port/engine-animation/component_applicator.cpp',
            REPO / 'port/engine-animation/angle_interpreter.cpp',
            REPO / 'port/engine-animation/events.cpp',
            REPO / 'port/engine-animation/event_track.cpp',
        ]
        box2d_sources = sorted((BOX2D_ROOT / 'Source').rglob('*.cpp'))
        if len(box2d_sources) != 31 or not (BOX2D_ROOT / 'License.txt').is_file():
            raise FileNotFoundError('Pinned Box2D 2.0.1 source/license inputs are incomplete')
        extra_sources.extend(box2d_sources)
    # Several shared source files are also needed by the Character runtime.
    # Keep compilation deterministic when the source lists overlap.
    extra_sources = list(dict.fromkeys(extra_sources))
    outputs: dict[str, Path] = {}
    abi_reports: dict[str, dict] = {}
    for abi, target, machine in (
        ('arm64-v8a', 'aarch64-linux-android35', 183),
        ('x86_64', 'x86_64-linux-android35', 62),
    ):
        static_library = static_root / abi / 'libIrrlicht.a'
        if not static_library.is_file():
            raise FileNotFoundError('Pinned Irrlicht archive is missing; run port/irrlicht-android/build.py --no-apk first')
        output_dir = build / variant / 'obj' / abi
        output_dir.mkdir(parents=True, exist_ok=True)
        flags = [f'--target={target}', f'--sysroot={sysroot}', '-std=c++17',
                 '-fPIC', '-fno-exceptions', '-fno-rtti', '-O2',
                 '-fno-fast-math', '-ffp-contract=off', '-include', 'cstring',
                 '-Wall', '-Wextra',
                 '-Werror', '-Wno-unused-parameter',
                 '-Wno-inconsistent-missing-override',
                 '-Wno-deprecated-copy-with-user-provided-copy',
                 '-I', upstream / 'include', '-I', engine / 'game',
                 '-I', REPO / 'port/level-world',
                 '-isystem', BOX2D_ROOT / 'Include',
                 '-I', HERE, '-I', glue]
        objects = []
        compile_sources = [('main', sources['main']), ('adapter', sources['adapter'])]
        compile_sources.extend((source.stem + '-' + str(index), source)
                               for index, source in enumerate(extra_sources))
        for name, source in compile_sources:
            obj = output_dir / (name + '.o')
            source_flags = flags
            if source.is_relative_to(BOX2D_ROOT):
                # Pinned Box2D 2.0.1 predates the repository's strict warning
                # gate; match its host build policy without weakening project
                # translation units.
                source_flags = [flag for flag in flags if flag != '-Werror']
                source_flags.append('-w')
            if source in {
                REPO / 'port/engine-skinning/skinning.cpp',
                engine / 'game/prince_character_runtime.cpp',
                REPO / 'port/level-world/actor_playback.cpp',
                REPO / 'port/level-world/actor_blended_playback.cpp',
                REPO / 'port/scene-materials/scene.cpp',
                REPO / 'port/level-world/character_coordinator.cpp',
                REPO / 'port/level-world/character_scene.cpp',
                REPO / 'port/level-world/decor_scene.cpp',
                REPO / 'port/level-world/floors.cpp',
                REPO / 'port/level-world/physical_world.cpp',
                engine / 'game/swamp_actor_floor_bridge.cpp',
                engine / 'game/swamp_actor_session.cpp',
                REPO / 'port/game-data/data.cpp',
                REPO / 'port/game-data/animation_bank.cpp',
                REPO / 'port/game-data/animation_tables.cpp',
                REPO / 'port/game-data/animation_scheduler.cpp',
                REPO / 'port/game-data/animation_selection.cpp',
                REPO / 'port/game-data/class_tables.cpp',
                REPO / 'port/game-data/properties.cpp',
                REPO / 'port/engine-animation/animation_registration.cpp',
                REPO / 'port/engine-animation/animation.cpp',
                REPO / 'port/engine-animation/angle_interpreter.cpp',
                REPO / 'port/engine-animation/events.cpp',
                REPO / 'port/engine-animation/event_track.cpp',
            }:
                source_flags = [flag for flag in flags if flag != '-fno-exceptions']
                source_flags.append('-fexceptions')
            if source == REPO / 'port/level-world/physical_controls.cpp':
                source_flags.append('-Wno-misleading-indentation')
            run(clang, *source_flags, '-c', source, '-o', obj)
            objects.append(obj)
        glue_obj = output_dir / 'android_native_app_glue.o'
        run(clang_c, f'--target={target}', f'--sysroot={sysroot}', '-fPIC',
            '-Wall', '-Wextra', '-I', glue, '-c', sources['glue'], '-o', glue_obj)
        objects.append(glue_obj)
        library = build / variant / 'lib' / abi / 'libdh2irrlicht.so'
        library.parent.mkdir(parents=True, exist_ok=True)
        run(clang, f'--target={target}', f'--sysroot={sysroot}', '-shared',
            '-Wl,--no-undefined', '-Wl,-z,max-page-size=16384',
            '-Wl,-z,common-page-size=16384', *objects, static_library,
            '-landroid', '-llog', '-lEGL', '-lGLESv1_CM', '-lGLESv2', '-lz',
            '-static-libstdc++', '-o', library)
        abi_report = check_elf(library, machine)
        run(readelf, '-h', library)
        abi_report['path'] = library.relative_to(build).as_posix()
        abi_report['irrlicht_archive_sha256'] = sha(static_library)
        abi_reports[abi] = abi_report
        outputs[abi] = library

    asset_root = build / variant / 'assets'
    shaders_root = upstream / 'media/Shaders'
    shader_reports = []
    for shader in sorted(shaders_root.glob('*')):
        if not shader.is_file():
            continue
        relative = shader.relative_to(upstream).as_posix()
        record = records.get(relative)
        if not record or shader.stat().st_size != record['bytes'] or sha(shader) != record['sha256']:
            raise ValueError(f'Irrlicht shader differs from pinned upstream manifest: {relative}')
        target = asset_root / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(shader, target)
        shader_reports.append({'path': relative, 'bytes': record['bytes'],
                               'sha256': record['sha256']})
    for required in ('media/Shaders/COGLES2Solid.vsh', 'media/Shaders/COGLES2Solid.fsh'):
        if not any(row['path'] == required for row in shader_reports):
            raise ValueError(f'Pinned Irrlicht shader missing: {required}')
    # The diagnostic links Irrlicht built with its bundled AES, IJG JPEG,
    # libpng, and zlib sources. Package the exact upstream notice-bearing
    # files (and an exact license comment excerpt from zlib.h) beside the APK
    # assets, without adding or editing anything under the pinned source tree.
    notice_sources = {
        'Irrlicht-LICENSE.txt': 'doc/irrlicht-license.txt',
        'IJG-LICENSE.txt': 'doc/jpglib-license.txt',
        'IJG-README.txt': 'source/Irrlicht/jpeglib/README',
        'libpng-LICENSE.txt': 'doc/libpng-license.txt',
        'libpng-UPSTREAM-LICENSE.txt': 'source/Irrlicht/libpng/LICENSE',
        'aesGladman-LICENSE.txt': 'doc/aesGladman.txt',
        'aesGladman-README.txt': 'source/Irrlicht/aesGladman/Readme.txt',
        'bzip2-LICENSE.txt': 'doc/bzip2-license.txt',
        'bzip2-UPSTREAM-LICENSE.txt': 'source/Irrlicht/bzip2/LICENSE',
        'zlib-README.txt': 'source/Irrlicht/zlib/README',
    }
    notice_reports = []
    notices_root = asset_root / 'third-party-notices'
    notices_root.mkdir(parents=True, exist_ok=True)
    for name, relative in notice_sources.items():
        source = upstream / relative
        record = records.get(relative)
        if (not record or not source.is_file() or
                source.stat().st_size != record['bytes'] or sha(source) != record['sha256']):
            raise ValueError(f'Pinned Irrlicht license source differs from manifest: {relative}')
        target = notices_root / name
        shutil.copy2(source, target)
        notice_reports.append({'path': target.relative_to(asset_root).as_posix(),
                               'source': relative, 'bytes': target.stat().st_size,
                               'sha256': sha(target), 'source_sha256': record['sha256']})
    if not swamp:
        lua_relative = 'vendor/lua-5.1.4/COPYRIGHT'
        lua_source = REPO / 'port/lua-runtime' / lua_relative
        lua_manifest = json.loads((REPO / 'port/lua-runtime/vendor-manifest.json').read_text())
        lua_record = next((row for row in lua_manifest['files']
                           if row['path'] == lua_relative), None)
        if (not lua_record or not lua_source.is_file() or
                lua_source.stat().st_size != lua_record['bytes'] or
                sha(lua_source) != lua_record['sha256']):
            raise ValueError(f'Pinned Lua license source differs from vendor manifest: {lua_relative}')
        lua_target = notices_root / 'Lua-COPYRIGHT.txt'
        shutil.copy2(lua_source, lua_target)
        notice_reports.append({'path': lua_target.relative_to(asset_root).as_posix(),
                               'source': 'port/lua-runtime/' + lua_relative,
                               'bytes': lua_target.stat().st_size,
                               'sha256': sha(lua_target),
                               'source_sha256': lua_record['sha256']})
    zlib_relative = 'source/Irrlicht/zlib/zlib.h'
    zlib_source = upstream / zlib_relative
    zlib_record = records.get(zlib_relative)
    if (not zlib_record or not zlib_source.is_file() or
            zlib_source.stat().st_size != zlib_record['bytes'] or
            sha(zlib_source) != zlib_record['sha256']):
        raise ValueError(f'Pinned Irrlicht license source differs from manifest: {zlib_relative}')
    zlib_bytes = zlib_source.read_bytes()
    if not zlib_bytes.startswith(b'/*') or b'*/' not in zlib_bytes:
        raise ValueError('Expected zlib.h opening notice block')
    zlib_notice = zlib_bytes[:zlib_bytes.index(b'*/') + 2]
    zlib_target = notices_root / 'zlib-LICENSE.txt'
    zlib_target.write_bytes(zlib_notice)
    notice_reports.append({'path': zlib_target.relative_to(asset_root).as_posix(),
                           'source': zlib_relative + ' (opening license comment)',
                           'bytes': len(zlib_notice), 'sha256': sha(zlib_target),
                           'source_sha256': zlib_record['sha256']})
    if swamp:
        box2d_license = BOX2D_ROOT / 'License.txt'
        if not box2d_license.is_file() or box2d_license.stat().st_size != 886:
            raise ValueError(f'Pinned Box2D 2.0.1 license source is missing or changed: {box2d_license}')
        box2d_target = notices_root / 'Box2D-2.0.1-License.txt'
        shutil.copy2(box2d_license, box2d_target)
        notice_reports.append({'path': box2d_target.relative_to(asset_root).as_posix(),
                               'source': 'port/physics-backend/box2d-2.0.1/License.txt',
                               'bytes': box2d_target.stat().st_size,
                               'sha256': sha(box2d_target),
                               'source_sha256': sha(box2d_license)})
    for source, target_name, expected_hash, source_label in (
            (NATIVE_APP_GLUE_NOTICE, 'native_app_glue-NOTICE.txt',
             NATIVE_APP_GLUE_NOTICE_SHA256,
             'Android NDK r29 native_app_glue/NOTICE'),
            (APACHE2_LICENSE, 'Apache-2.0-LICENSE.txt', APACHE2_LICENSE_SHA256,
             'Apache License 2.0 text from Android NDK r29 NOTICE')):
        if not source.is_file() or sha(source) != expected_hash:
            raise ValueError(f'Checked Android native_app_glue legal source is missing or changed: {source}')
        if target_name == 'native_app_glue-NOTICE.txt':
            contents = source.read_text(encoding='utf-8')
            if 'Copyright (C) 2010 The Android Open Source Project' not in contents or \
                    'Apache License, Version 2.0' not in contents:
                raise ValueError('native_app_glue NOTICE lacks its Android attribution or Apache license reference')
        target = notices_root / target_name
        shutil.copy2(source, target)
        notice_reports.append({'path': target.relative_to(asset_root).as_posix(),
                               'source': source_label, 'bytes': target.stat().st_size,
                               'sha256': sha(target), 'source_sha256': expected_hash})
    notice_scope = ('local cache-scene diagnostic APK' if cache_scene else
                    'local SWAMP Irrlicht source diagnostic APK' if swamp else
                    'optional DH2 Irrlicht host diagnostic')
    notice_text = (
        f'Third-party notices for the {notice_scope}.\n\n'
        'This APK includes the Irrlicht Engine OGL-ES source tree (SVN r6038), '
        'built into its native library, and shaders from that tree.\n'
        + ('This APK includes Lua 5.1.4 in its DH2 Lua native library.\n'
           if not swamp else '')
        + 'This software is based in part on the work of the Independent JPEG Group.\n'
        + 'Irrlicht also includes code from zlib, libpng, and aesGladman.\n\n'
        + ('This APK also includes Box2D 2.0.1 source under its included license.\n\n'
           if swamp else '')
        + 'The Android NativeActivity glue is Apache License 2.0; its attribution notice and full license are included.\n\n'
        + 'The complete applicable notice and license texts are in this directory.\n'
    ).encode('utf-8')
    notice_index = notices_root / 'NOTICE.txt'
    notice_index.write_bytes(notice_text)
    notice_reports.append({'path': notice_index.relative_to(asset_root).as_posix(),
                           'source': 'authored package index',
                           'bytes': len(notice_text), 'sha256': sha(notice_index)})
    cache_asset_report = None
    if cache_scene:
        if cache_root is None:
            raise ValueError('cache_root is required for cache asset staging')
        cache_asset_report = irrlicht_cache_scene_assets(cache_root, asset_root)
    elif swamp:
        if cache_root is None:
            raise ValueError('cache_root is required for SWAMP asset staging')
        cache_asset_report = irrlicht_swamp_assets(cache_root, asset_root)
    result = {'abi': abi_reports, 'shaders': shader_reports,
                     'third_party_notices': notice_reports,
                     'native_compile_inputs': [
                         {'path': (path.relative_to(REPO).as_posix()
                                   if REPO in path.parents else str(path)),
                          'sha256': sha(path)}
                         for path in [sources['main'], sources['adapter'],
                                      sources['glue'], *extra_sources]],
                     'upstream_svn_revision': manifest['svn_revision'],
                     'upstream_tree_manifest_sha256': manifest['tree_manifest_sha256'],
                     'scope': ('Local-only cache-backed void_maze texture-subset render diagnostic; not a playable level or release asset.'
                               if cache_scene else
                               'Development diagnostic: SWAMP module-zero source render and path-mask movement with the authored Prince Character coordinator, animation bank, actor runtime, and one NativeWorld step per frame; not full-game parity.'
                               if swamp else
                               'Synthetic SceneMesh adapter render diagnostic hosted beside the DH2 source activities; not a DH2 asset or a reconstructed game level.')}
    if cache_asset_report:
        result['local_cache_assets'] = cache_asset_report
    return outputs, result


def build_irrlicht_swamp_package(sdk: Path, ndk: Path, cache: Path,
                                build: Path,
                                irrlicht_static_root: Path | None = None) -> Path:
    """Build a cache-minimal, standalone, local-only Irrlicht SWAMP APK."""
    default_output = (HERE / 'build').resolve()
    build = build.resolve()
    if build == default_output:
        raise ValueError('--irrlicht-swamp output must be separate from the default APK build directory')
    build.mkdir(parents=True, exist_ok=True)
    tools = sdk / 'build-tools/37.0.0'
    jar = sdk / 'platforms/android-37.2/android.jar'
    if not jar.is_file():
        jar = sdk / 'platforms/android-37.0/android.jar'
    required = (tools / 'aapt2.exe', tools / 'zipalign.exe', tools / 'apksigner.bat', jar)
    missing = [path for path in required if not path.is_file()]
    if missing:
        raise FileNotFoundError('Android SDK files missing: ' + ', '.join(map(str, missing)))

    libraries, native_report = build_irrlicht_host(
        ndk, build, swamp=True, cache_root=cache,
        irrlicht_static_root=irrlicht_static_root)
    cache_asset_report = native_report.get('local_cache_assets')
    if not cache_asset_report:
        raise RuntimeError('SWAMP host build did not report its staged local assets')
    base = build / 'base-swamp.apk'
    manifest = HERE / 'AndroidManifest.irrlicht-swamp.xml'
    assets = build / 'irrlicht-swamp/assets'
    run(tools / 'aapt2.exe', 'link', '--manifest', manifest,
        '-I', jar, '--min-sdk-version', '26', '--target-sdk-version', '37',
        '-A', assets, '-o', base)
    with zipfile.ZipFile(base, 'a') as apk:
        for abi, library in libraries.items():
            apk.write(library, f'lib/{abi}/libdh2irrlicht.so',
                      compress_type=zipfile.ZIP_STORED)

    aligned = build / 'aligned-swamp.apk'
    run(tools / 'zipalign.exe', '-f', '-P', '16', '4', base, aligned)
    java_home = Path(os.environ.get('JAVA_HOME', 'C:/Program Files/Java/jdk-23'))
    keytool = java_home / 'bin/keytool.exe'
    if not keytool.is_file():
        keytool = Path(shutil.which('keytool') or '')
    if not keytool.is_file():
        raise FileNotFoundError('JDK keytool required to sign the local debug APK; set JAVA_HOME')
    key = build / 'local-irrlicht-swamp-debug.jks'
    if not key.is_file():
        run(keytool, '-genkeypair', '-keystore', key, '-storepass', 'android',
            '-keypass', 'android', '-alias', 'debug', '-keyalg', 'RSA',
            '-keysize', '2048', '-validity', '3650', '-dname',
            'CN=DH2 SWAMP Local Debug')
    apk_path = build / 'dh2-swamp-irrlicht-source-local-debug.apk'
    run(tools / 'apksigner.bat', 'sign', '--ks', key, '--ks-key-alias', 'debug',
        '--ks-pass', 'pass:android', '--key-pass', 'pass:android',
        '--out', apk_path, aligned)
    run(tools / 'apksigner.bat', 'verify', '--verbose', apk_path)
    run(tools / 'zipalign.exe', '-c', '-P', '16', '4', apk_path)

    with zipfile.ZipFile(apk_path, 'r') as apk:
        names = set(apk.namelist())
    expected_cache_assets = {
        'assets/' + value for value in cache_asset_report['cache_apk_assets']}
    included_cache_assets = {name for name in names if name in expected_cache_assets}
    if included_cache_assets != expected_cache_assets:
        raise RuntimeError('SWAMP local APK does not contain the exact selected source asset set')
    if any(name.startswith('assets/dh2/encounter/') or
           name.startswith('assets/dh2/infected') for name in names):
        raise RuntimeError('SWAMP APK unexpectedly contains unrelated source/cache bundles')
    for name in ('assets/dh2/LOCAL-ASSET-NOTICE.txt',
                 'assets/dh2/local-irrlicht-swamp-manifest.json',
                 'assets/third-party-notices/Irrlicht-LICENSE.txt',
                 'assets/third-party-notices/IJG-LICENSE.txt',
                 'assets/third-party-notices/IJG-README.txt',
                 'assets/third-party-notices/libpng-UPSTREAM-LICENSE.txt',
                 'assets/third-party-notices/aesGladman-README.txt',
                 'assets/third-party-notices/bzip2-UPSTREAM-LICENSE.txt',
                 'assets/third-party-notices/zlib-README.txt',
                 'assets/third-party-notices/zlib-LICENSE.txt',
                 'assets/third-party-notices/native_app_glue-NOTICE.txt',
                 'assets/third-party-notices/Apache-2.0-LICENSE.txt',
                 'assets/third-party-notices/Box2D-2.0.1-License.txt'):
        if name not in names:
            raise RuntimeError(f'Local SWAMP APK missing required notice/manifest: {name}')

    report = {
        'scope': ('Development diagnostic: SWAMP module-zero Irrlicht source scene plus four Prince warrior skins driven by the authored Character Coordinator, animation bank, actor_runtime and one NativeWorld step per accepted frame; not full-game parity'),
        'package_name': 'local.dh2.sourceviewer.irrlichtswamp',
        'launcher_activity': 'android.app.NativeActivity',
        'min_sdk': 26,
        'target_sdk': 37,
        'build_tools': tools.name,
        'apk': {'path': apk_path.relative_to(build).as_posix(),
                'sha256': sha(apk_path), 'bytes': apk_path.stat().st_size},
        'local_cache_only': True,
        'release_eligible': False,
        'included_cache_assets': sorted(included_cache_assets),
        'prince_animation_bank': cache_asset_report['animation_bank'],
        'apk_entry_count': len(names),
        'apk_asset_count': sum(name.startswith('assets/') for name in names),
        'apk_notice_assets': sorted(name for name in names
                                    if 'NOTICE' in name.upper() or 'LICENSE' in name.upper()),
        'irrlicht_swamp_diagnostic': native_report,
        'control_policy': {
            'fixed_step_ms': 20,
            'max_catch_up_steps': 5,
            'max_frame_delta_ms': 100,
            'movement_timebase': 'developer-selected 20 ms actor logical tick; actor speed/flags come from source Character properties',
            'source_player_path_mask': 2,
            'source_module_index': 0,
            'world_step': 'one pinned Box2D 2.0.1 NativeWorld Step per successful actor frame',
            'collision': 'module-zero PF floor graph/path checks and source Character body pin/unpin; no environment bodies, walls, swept volume, or other actors',
        },
    }
    # Keep a complete hash list for every translation unit in the native
    # link, plus the public headers that define the newly composed source
    # actor/session and its borrowed navigation/physics interfaces. This is
    # intentionally nested in the isolated SWAMP variant report.
    source_hashes = {
        row['path']: row['sha256']
        for row in native_report['native_compile_inputs']
    }
    source_headers = [
        REPO / 'port/irrlicht-android/swamp-smoke/main.cpp',
        REPO / 'port/irrlicht-android/swamp-smoke/alpha_map_policy.hpp',
        REPO / 'port/scene-materials/technique_selector.hpp',
        REPO / 'port/irrlicht-android/game/scene_mesh_adapter.hpp',
        REPO / 'port/irrlicht-android/game/prince_actor.hpp',
        REPO / 'port/irrlicht-android/game/prince_character_runtime.hpp',
        REPO / 'port/irrlicht-android/game/swamp_source_navigation_view.hpp',
        REPO / 'port/irrlicht-android/game/swamp_actor_floor_bridge.hpp',
        REPO / 'port/irrlicht-android/game/swamp_actor_session.hpp',
        REPO / 'port/level-world/actor_runtime.hpp',
        REPO / 'port/level-world/character_coordinator.hpp',
        REPO / 'port/level-world/character_scene.hpp',
        REPO / 'port/level-world/character_state.hpp',
        REPO / 'port/level-world/character_timers.hpp',
        REPO / 'port/level-world/floors.hpp',
        REPO / 'port/level-world/native_body.hpp',
        REPO / 'port/level-world/navigation_producers.hpp',
        REPO / 'port/level-world/physical_world.hpp',
        REPO / 'port/level-world/visual_motion.hpp',
        REPO / 'port/navigation/navigation.hpp',
        REPO / 'port/physics-backend/box2d-2.0.1/Include/Box2D.h',
        HERE / 'build.py',
        REPO / 'port/irrlicht-android/upstream-source-manifest.json',
    ]
    for path in source_headers:
        if not path.is_file():
            raise FileNotFoundError(f'SWAMP source manifest input is missing: {path}')
        key_name = (path.relative_to(REPO).as_posix()
                    if REPO in path.parents else str(path))
        source_hashes[key_name] = sha(path)
    native_report['source_sha256'] = source_hashes
    report['source_sha256'] = source_hashes
    report_path = build / 'irrlicht-swamp-build-validation.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(apk_path)
    print('SHA-256:', report['apk']['sha256'])
    print('Build report:', report_path)
    return apk_path


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sdk', required=True, type=Path)
    parser.add_argument('--ndk', required=True, type=Path)
    parser.add_argument('--cache', type=Path,
                        help='Unpacked original cache; only assets used by the selected local diagnostic are bundled')
    parser.add_argument('--irrlicht-host', action='store_true',
                        help='Build a separate APK variant with an opt-in synthetic Irrlicht adapter NativeActivity')
    parser.add_argument('--irrlicht-cache-scene', action='store_true',
                        help='Build a local-only APK variant that renders pinned void_maze cache assets with Irrlicht')
    parser.add_argument('--irrlicht-swamp', action='store_true',
                        help='Build a separate local-only SWAMP module-zero Irrlicht NativeActivity APK')
    parser.add_argument('--irrlicht-swamp-in-app', action='store_true',
                        help='Build a local-only standard-app APK with an opt-in Irrlicht SWAMP NativeActivity')
    parser.add_argument('--build-dir', type=Path,
                        help='Isolated output directory (only valid with --irrlicht-swamp)')
    parser.add_argument('--irrlicht-static-dir', type=Path,
                        help='Directory holding the pinned Irrlicht static libraries (variant builds only)')
    args = parser.parse_args()
    selected_irrlicht_modes = sum((args.irrlicht_host, args.irrlicht_cache_scene,
                                   args.irrlicht_swamp, args.irrlicht_swamp_in_app))
    if selected_irrlicht_modes > 1:
        parser.error('choose only one Irrlicht variant: --irrlicht-host, --irrlicht-cache-scene, --irrlicht-swamp, or --irrlicht-swamp-in-app')
    if args.build_dir is not None and not args.irrlicht_swamp:
        parser.error('--build-dir is currently supported only with --irrlicht-swamp')
    if args.irrlicht_static_dir is not None and not (args.irrlicht_swamp or args.irrlicht_swamp_in_app):
        parser.error('--irrlicht-static-dir is currently supported only with a SWAMP Irrlicht variant')
    if (args.irrlicht_cache_scene or args.irrlicht_swamp or
            args.irrlicht_swamp_in_app) and args.cache is None:
        parser.error('cache-scene and SWAMP Irrlicht variants require an explicit external --cache root')
    sdk, ndk = args.sdk.resolve(), args.ndk.resolve()
    if args.irrlicht_swamp:
        cache = args.cache.resolve(strict=True)
        build = (args.build_dir if args.build_dir is not None else
                 HERE / 'build/irrlicht-swamp').resolve()
        try:
            build.relative_to((HERE / 'build').resolve())
        except ValueError:
            parser.error('--irrlicht-swamp build output must stay under port/android-app/build')
        if build == (HERE / 'build').resolve():
            parser.error('--irrlicht-swamp build output must not replace the default APK directory')
        build_irrlicht_swamp_package(sdk, ndk, cache, build,
                                     irrlicht_static_root=args.irrlicht_static_dir)
        return
    build = (HERE / 'build/irrlicht-swamp-in-app' if args.irrlicht_swamp_in_app
             else HERE / 'build')
    classes, dex, lib = build / 'classes', build / 'dex', build / 'lib'
    for path in (classes, dex, lib):
        path.mkdir(parents=True, exist_ok=True)
    cache = (args.cache if args.cache is not None else REPO.parent / 'cache/files').resolve()
    encounter_paths = {
        'room.bdae': 'data/3d/modules/void_maze/void_maze.bdae',
        'floor.tga': 'data/3d/textures/env_voidmaze.tga',
        'hero.bdae': 'data/3d/characters/prince/prince_low_poly_warrior.bdae',
        'hero.tga': 'data/3d/textures/prince-warrior.tga',
        'walk.bdae': 'data/3d/characters/prince/animations/prince_walk_1hand.bdae',
        'idle.bdae': 'data/3d/characters/prince/animations/prince_idle_shield.bdae',
        'attack.bdae': 'data/3d/characters/prince/animations/prince_1hand_combo_01.bdae',
        'properties.bin': 'data/pydata/character_properties_pyarray.bin',
        'classes.bin': 'data/pydata/character_classes_pyarray.bin',
        'loot.bin': 'data/pydata/loot_table_pyarray.bin',
        'powers.bin': 'data/pydata/item_powers_pyarray.bin',
        'quests.bin': 'data/pydata/v2quests_pyarray.bin',
        # Original first SWAMP module, bundled only in this explicitly supplied-cache build.
        # The Android runtime receives byte arrays from APK assets; it does not need cache access.
        'swamp.bdae': 'data/3d/modules/swamp/swamp.bdae',
        'swamp.mlx': 'data/scene/001_swamp.mlx',
        'swamp-entry-mgp.mgp': 'data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp',
        'swamp-lizard-intro.mgp': 'data/3d/modules/swamp/mgp/obj_3of4_brdwalk_sw_00.mgp',
        'swamp-diffuse.tga': 'data/3d/textures/env_swamp.tga',
        'swamp-alpha.tga': 'data/3d/textures/pvr2_env_swamp_alpha.tga',
        'common-script-names.bin': 'data/pydata/scripts_pyscriptnames.bin',
        'common-script-programs.bin': 'data/pydata/scripts_pyscripts.bin',
        'swamp-script-names.bin': 'data/pydata/scripts/001_swamp_pyscriptnames.bin',
        'swamp-script-programs.bin': 'data/pydata/scripts/001_swamp_pyscripts.bin',
    }
    encounter_assets = {}
    encounter_manifest = {}
    for name, relative in encounter_paths.items():
        source = cache / relative
        if not source.is_file():
            raise FileNotFoundError(f'Supply --cache with the unpacked original cache: missing {relative}')
        encounter_assets['assets/dh2/encounter/' + name] = source
        encounter_manifest[name] = {'source': relative, 'sha256': sha(source),
                                    'bytes': source.stat().st_size}
    constant_sources = [cache / 'data/pydata' / name for name in
                        ('ai_pycst.bin', 'design_pycst.bin', 'v2quests_pycst.bin')]
    constant_payloads = [path.read_bytes() for path in constant_sources]
    if any(len(data) < 4 for data in constant_payloads):
        raise ValueError('Original encounter constants are incomplete')
    group_count = sum(struct.unpack_from('<I', data)[0] for data in constant_payloads)
    merged = build / 'encounter-constants.bin'
    merged.write_bytes(struct.pack('<I', group_count) + b''.join(data[4:] for data in constant_payloads))
    encounter_assets['assets/dh2/encounter/constants.bin'] = merged
    encounter_manifest['constants.bin'] = {'sha256': sha(merged), 'bytes': merged.stat().st_size,
        'derivation': 'sum little-endian group counts, concatenate complete original group payloads',
        'sources': [{'source': path.relative_to(cache).as_posix(), 'sha256': sha(path)}
                    for path in constant_sources]}
    bundle_manifest = build / 'encounter-assets.json'
    bundle_manifest.write_text(json.dumps(encounter_manifest, indent=2) + '\n')
    encounter_assets['assets/dh2/encounter/manifest.json'] = bundle_manifest
    infected_village_assets, infected_village_manifest = infected_village_bundle(cache, build)
    infected_actor_assets, infected_actor_manifest = infected_actor_preview_bundle(cache, build)
    jar = sdk / 'platforms/android-37.2/android.jar'
    if not jar.is_file():
        jar = sdk / 'platforms/android-37.0/android.jar'
    tools = sdk / 'build-tools/37.0.0'
    if not tools.is_dir():
        raise FileNotFoundError('Android SDK Build Tools 37.0.0 required; install with sdkmanager "build-tools;37.0.0"')
    clang = ndk / 'toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
    java_home = Path(os.environ.get('JAVA_HOME', 'C:/Program Files/Java/jdk-23'))
    javac = java_home / 'bin/javac.exe'
    keytool = java_home / 'bin/keytool.exe'
    if not javac.is_file() or not keytool.is_file():
        javac, keytool = shutil.which('javac'), shutil.which('keytool')
    if not javac or not keytool:
        raise FileNotFoundError('JDK javac/keytool required; set JAVA_HOME')
    result = {}
    lua_root = REPO / 'port/lua-runtime'
    run(sys.executable, lua_root / 'build.py', '--ndk', ndk,
        '--report', build / 'lua-build-validation.json')
    manifest = json.loads((REPO / 'recovered/scripts/manifest.json').read_text())
    known = {row['path']: row for row in manifest['files']}
    script_assets = {}
    for asset, source in [('ai-commons.lua', 'ai/_commons.luac'),
                          ('skills-commons.lua', 'skills/_commons.luac'),
                          ('combat-formulas.lua', 'level/combat_formulas.luac')]:
        path = REPO / 'recovered/scripts/original/data/scripts' / source
        row = known[path.relative_to(REPO).as_posix()]
        if sha(path) != row['sha256']: raise ValueError('Changed original script: ' + source)
        script_assets['assets/dh2/scripts/' + asset] = path
    for abi, target, machine in (
        ('arm64-v8a', 'aarch64-linux-android35', 183),
        ('x86_64', 'x86_64-linux-android35', 62),
    ):
        directory = lib / abi
        directory.mkdir(exist_ok=True)
        output = directory / 'libdh2source.so'
        lua_library = directory / 'libdh2lua.so'
        variant = 'arm64' if abi == 'arm64-v8a' else 'x86_64'
        shutil.copyfile(lua_root / ('build/lua-' + variant + '.so'), lua_library)
        run(clang, f'--target={target}', '-std=c++17', '-O2', '-Wall', '-Wextra',
            '-Werror', '-fPIC', '-shared', '-fno-exceptions', '-fno-rtti',
            '-fno-fast-math', '-ffp-contract=off',
            '-nostdlib++', '-Wl,-z,max-page-size=16384', '-Wl,--no-undefined',
            *SOURCES, '-L', directory, '-ldh2lua', '-llog', '-lGLESv2', '-lEGL', '-landroid', '-o', output)
        result[abi] = check_elf(output, machine)
        result[abi]['lua_library'] = check_elf(lua_library, machine)
    irrlicht_libraries = {}
    irrlicht_report = None
    if args.irrlicht_host or args.irrlicht_cache_scene or args.irrlicht_swamp_in_app:
        irrlicht_libraries, irrlicht_report = build_irrlicht_host(
            ndk, build, cache_scene=args.irrlicht_cache_scene,
            swamp=args.irrlicht_swamp_in_app,
            cache_root=cache if (args.irrlicht_cache_scene or args.irrlicht_swamp_in_app) else None,
            irrlicht_static_root=args.irrlicht_static_dir)
    java_sources = sorted((HERE / 'src/local/dh2/sourceviewer').glob('*.java'))
    run(javac, '-Xlint:-options', '-source', '8', '-target', '8', '-classpath', jar,
        '-d', classes, *java_sources)
    for source in java_sources:
        if (classes / 'local/dh2/sourceviewer' / (source.stem + '.class')).stat().st_mtime_ns < source.stat().st_mtime_ns:
            raise RuntimeError('javac did not update ' + source.stem + '.class')
    run(tools / 'd8.bat', '--min-api', '26', '--output', dex,
        *sorted((classes / 'local/dh2/sourceviewer').glob('*.class')))
    base = build / 'base.apk'
    if args.irrlicht_swamp_in_app:
        manifest_path = HERE / 'AndroidManifest.irrlicht-swamp-in-app.xml'
        integrated_manifest = validate_irrlicht_swamp_in_app_manifest(manifest_path)
    else:
        manifest_path = HERE / ('AndroidManifest.irrlicht.xml'
                                if (args.irrlicht_host or args.irrlicht_cache_scene)
                                else 'AndroidManifest.xml')
    aapt_args = [tools / 'aapt2.exe', 'link', '--manifest', manifest_path,
        '-I', jar, '--min-sdk-version', '26', '--target-sdk-version', '37',
        '-o', base]
    if args.irrlicht_host or args.irrlicht_cache_scene or args.irrlicht_swamp_in_app:
        variant_dir = ('irrlicht-cache-scene' if args.irrlicht_cache_scene else
                       'irrlicht-swamp' if args.irrlicht_swamp_in_app else
                       'irrlicht-host')
        aapt_args += ['-A', build / variant_dir / 'assets']
    run(*aapt_args)
    with zipfile.ZipFile(base, 'a') as apk:
        apk.write(dex / 'classes.dex', 'classes.dex', compress_type=zipfile.ZIP_DEFLATED)
        for abi in result:
            for name in ('libdh2source.so', 'libdh2lua.so'):
                apk.write(lib / abi / name, f'lib/{abi}/{name}', compress_type=zipfile.ZIP_STORED)
        for abi, library in irrlicht_libraries.items():
            apk.write(library, f'lib/{abi}/libdh2irrlicht.so', compress_type=zipfile.ZIP_STORED)
        for name, path in {**script_assets, **encounter_assets,
                           **infected_village_assets, **infected_actor_assets}.items():
            apk.write(path, name, compress_type=zipfile.ZIP_DEFLATED)
    aligned = build / 'aligned.apk'
    run(tools / 'zipalign.exe', '-f', '-P', '16', '4', base, aligned)
    # The integrated variant is a same-package upgrade of the standard app;
    # sign it with the standard APK's key even though outputs are isolated.
    key = (HERE / 'build/debug.jks' if args.irrlicht_swamp_in_app
           else build / 'debug.jks')
    if not key.is_file():
        key.parent.mkdir(parents=True, exist_ok=True)
        run(keytool, '-genkeypair', '-keystore', key, '-storepass', 'android',
            '-keypass', 'android', '-alias', 'debug', '-keyalg', 'RSA', '-keysize',
            '2048', '-validity', '3650', '-dname', 'CN=DH2 Local Debug')
    if args.irrlicht_swamp_in_app:
        signed = build / 'dh2-source-renderer-irrlicht-swamp-in-app-local-debug.apk'
    elif args.irrlicht_cache_scene:
        signed = build / 'dh2-source-renderer-irrlicht-cache-scene-local-debug.apk'
    elif args.irrlicht_host:
        signed = build / 'dh2-source-renderer-irrlicht-host-debug.apk'
    else:
        signed = build / 'dh2-source-renderer-debug.apk'
    run(tools / 'apksigner.bat', 'sign', '--ks', key, '--ks-key-alias', 'debug',
        '--ks-pass', 'pass:android', '--key-pass', 'pass:android',
        '--out', signed, aligned)
    run(tools / 'apksigner.bat', 'verify', '--verbose', signed)
    run(tools / 'zipalign.exe', '-c', '-P', '16', '4', signed)
    report = {'scope': 'source-built authored development encounter, static SWAMP and INFECTED_VILLAGE module previews, path-mask-gated endpoint movement, and manually started bounded LizardMan_Intro logical trace; not the complete original game',
              'target_sdk': 37, 'min_sdk': 26, 'build_tools': tools.name, 'abi': result,
              'apk': {'sha256': sha(signed), 'bytes': signed.stat().st_size},
              'lua_build_sha256': sha(build / 'lua-build-validation.json'),
              'lua_build': json.loads((build / 'lua-build-validation.json').read_text()),
              'script_assets': {name: {'sha256': sha(path), 'bytes': path.stat().st_size}
                                for name, path in script_assets.items()},
              'java_source_sha256': {p.relative_to(HERE).as_posix(): sha(p)
                                     for p in java_sources},
              'encounter_assets': encounter_manifest,
              'infected_village_static_preview': infected_village_manifest,
              'infected_actor_diagnostic_preview': infected_actor_manifest,
              'source_sha256': {str(p.relative_to(REPO)).replace('\\', '/'): sha(p)
                                for p in SOURCES}}
    if args.irrlicht_swamp_in_app:
        report_path = build / 'irrlicht-swamp-in-app-build-validation.json'
        report['scope'] = (
            'LOCAL-ONLY standard Java app plus opt-in Irrlicht SWAMP module-zero source-render and movement diagnostic; '
            'not a complete game or public release asset')
        report['cache_bearing_local_test'] = True
        report['local_only'] = True
        report['release_eligible'] = False
        report['integrated_native_activity'] = integrated_manifest
        report['signing_key_shared_with_standard_app'] = 'port/android-app/build/debug.jks'
        report['irrlicht_swamp_local_cache_assets'] = (
            irrlicht_report.get('local_cache_assets') if irrlicht_report else None)
    elif args.irrlicht_cache_scene:
        report_path = build / 'irrlicht-cache-scene-build-validation.json'
        report['scope'] = (
            'LOCAL-ONLY CACHE-BEARING TEST APK: pinned void_maze BRES and env_voidmaze PVRTC texture draws '
            'through Irrlicht; texture-bearing subset diagnostic, not a playable level or public release')
        report['cache_bearing_local_test'] = True
        report['release_eligible'] = False
    elif args.irrlicht_host:
        report_path = HERE / 'irrlicht-host-build-validation.json'
    else:
        report_path = HERE / 'build-validation.json'
    if irrlicht_report:
        if args.irrlicht_swamp_in_app:
            swamp_app_sources = [
                REPO / 'port/irrlicht-android/swamp-smoke/main.cpp',
                REPO / 'port/irrlicht-android/game/scene_mesh_adapter.cpp',
                REPO / 'port/irrlicht-android/game/scene_mesh_adapter.hpp',
                HERE / 'AndroidManifest.irrlicht-swamp-in-app.xml',
                HERE / 'src/local/dh2/sourceviewer/MainActivity.java',
                HERE / 'build.py',
            ]
            irrlicht_report['source_sha256'] = {
                path.relative_to(REPO).as_posix(): sha(path)
                for path in swamp_app_sources}
            report['irrlicht_swamp_in_app_diagnostic'] = irrlicht_report
        elif args.irrlicht_cache_scene:
            cache_scene_sources = [
                REPO / 'port/irrlicht-android/cache-scene-smoke/main.cpp',
                REPO / 'port/irrlicht-android/game/scene_mesh_adapter.cpp',
                REPO / 'port/irrlicht-android/game/scene_mesh_adapter.hpp',
                HERE / 'scene_buffers.cpp', HERE / 'scene_buffers.hpp',
                REPO / 'port/skin-payloads/skin.cpp',
                REPO / 'port/animation-pose/pose.cpp',
                REPO / 'port/animation-values/values.cpp',
                REPO / 'port/animation-timeline/timeline.cpp',
                REPO / 'port/animation-mixing/mixing.cpp',
                REPO / 'port/animation-layers/layers.cpp',
                REPO / 'port/scene-draw/draw.cpp',
                REPO / 'port/scene-payloads/scene.cpp',
                REPO / 'port/asset-payloads/payloads.cpp',
                REPO / 'port/engine-resources/resources.cpp',
                REPO / 'port/engine-math/math.cpp',
                REPO / 'port/material-bindings/bindings.cpp',
                REPO / 'port/texture-assets/texture.cpp',
                REPO / 'port/texture-assets/decode.cpp',
                HERE / 'AndroidManifest.irrlicht.xml',
            ]
            irrlicht_report['source_sha256'] = {
                path.relative_to(REPO).as_posix(): sha(path)
                for path in cache_scene_sources}
            report['irrlicht_cache_scene_local_diagnostic'] = irrlicht_report
        else:
            irrlicht_report['source_sha256'] = {
                path.relative_to(REPO).as_posix(): sha(path)
                for path in (REPO / 'port/irrlicht-android/game-smoke/main.cpp',
                             REPO / 'port/irrlicht-android/game/scene_mesh_adapter.cpp',
                             REPO / 'port/irrlicht-android/game/scene_mesh_adapter.hpp',
                             HERE / 'AndroidManifest.irrlicht.xml')}
            report['irrlicht_host_diagnostic'] = irrlicht_report
    (report_path).write_text(json.dumps(report, indent=2) + '\n')
    print(signed)


if __name__ == '__main__':
    main()
