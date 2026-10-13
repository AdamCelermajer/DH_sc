"""Build provenance-preserving platform comparison views from asset rows.

This module is intentionally pure: callers may pass freshly inventoried rows
and decide where/how to store the result. It groups exact source paths and
explicit/candidate logical URIs while retaining each file's raw and decoded
hashes. A shared path or hash proves only path correspondence or byte identity;
it does not establish runtime use or behavioral equivalence.
"""
from __future__ import annotations

from collections import defaultdict
from pathlib import Path, PurePosixPath

from common import stable_id


ANDROID_SOURCES = frozenset({
    'android', 'original-cache', 'port-runtime-original-cache',
    # Retain the existing auxiliary comparison sets as distinct source rows.
    'android-extra', 'audio-cache',
})


def _side(source: str) -> str | None:
    if source in ANDROID_SOURCES:
        return 'ANDROID_OR_RECOVERED'
    if source.lower().startswith('ios') or source.lower() == 'ios-package':
        return 'IOS'
    return None


def _norm(value: str) -> str:
    return PurePosixPath(str(value).replace('\\', '/')).as_posix()


def _physical_key(asset: dict) -> str:
    source = str(asset.get('source', ''))
    relative = _norm(asset.get('relative') or Path(str(asset.get('path', ''))).name)
    if source == 'android-extra':
        return _norm('3d/' + relative)
    if source == 'audio-cache' and PurePosixPath(relative).suffix.lower() in ('.wav', '.vxn'):
        return _norm('sounds/' + PurePosixPath(relative).name)
    return relative


def _logical_uris(asset: dict) -> list[dict]:
    """Return URI candidates with their source and confidence evidence."""
    source = str(asset.get('source', ''))
    candidates: dict[str, dict] = {}
    for alias in asset.get('metadata_aliases') or []:
        if not isinstance(alias, dict) or not alias.get('logical_path'):
            continue
        uri = _norm(alias['logical_path'])
        candidates[uri] = {
            'logical_uri': uri,
            'confidence': alias.get('confidence', asset.get('mapping_confidence', 'UNKNOWN')),
            'provenance': alias.get('evidence', 'metadata_aliases entry'),
            'package_version': alias.get('package_version') or asset.get('package_version'),
            'kind': 'package-map-alias',
        }

    # package_index.logical_paths is a parallel compact form. Keep an explicit
    # alias's richer provenance when both representations are present.
    for uri_value in asset.get('logical_paths') or []:
        if not uri_value:
            continue
        uri = _norm(uri_value)
        candidates.setdefault(uri, {
            'logical_uri': uri,
            'confidence': asset.get('mapping_confidence', 'UNKNOWN'),
            'provenance': 'logical_paths metadata field; detailed alias evidence unavailable',
            'package_version': asset.get('package_version'),
            'kind': 'logical-path-metadata',
        })

    # Data-root relative names on staged source trees are useful lookup keys,
    # but are identified separately from embedded iOS pack-map aliases.
    relative = _norm(asset.get('relative') or '')
    if relative and not candidates:
        if source in {'android', 'original-cache', 'port-runtime-original-cache'}:
            uri = relative if relative.startswith('data/') else 'data/' + relative
            candidates[uri] = {
                'logical_uri': uri,
                'confidence': 'SOURCE_ROOT_PATH',
                'provenance': f'{source} file relative to its data/ source root; no native admission claim',
                'package_version': asset.get('package_version'),
                'kind': 'source-root-relative',
            }
        elif source == 'android-extra':
            uri = _norm('data/3d/' + relative)
            candidates[uri] = {
                'logical_uri': uri,
                'confidence': 'SOURCE_ROOT_PATH',
                'provenance': 'android-extra file relative to the retained 3d/ source root',
                'package_version': asset.get('package_version'),
                'kind': 'source-root-relative',
            }
        elif source == 'ios':
            uri = relative if relative.startswith('data/') else 'data/' + relative
            candidates[uri] = {
                'logical_uri': uri,
                'confidence': 'SOURCE_ROOT_PATH',
                'provenance': 'staged ios file relative to its data/ source root; no native admission claim',
                'package_version': asset.get('package_version'),
                'kind': 'source-root-relative',
            }
    return [candidates[key] for key in sorted(candidates)]


def _file_record(asset: dict, side: str, logical: list[dict]) -> dict:
    return {
        'asset_id': asset.get('id'),
        'path': asset.get('path'),
        'source': asset.get('source'),
        'side': side,
        'relative': _norm(asset.get('relative') or ''),
        'logical_uri_provenance': logical,
        'raw': {
            'bytes': asset.get('bytes', asset.get('size')),
            'sha256': asset.get('sha256'),
        },
        'decoded': {
            'bytes': asset.get('decoded_bytes'),
            'sha256': asset.get('decoded_sha256'),
            'format': asset.get('format'),
            'recipe': asset.get('decode_recipe'),
            'confidence': asset.get('decode_confidence'),
            'crosscheck': asset.get('decode_crosscheck'),
        } if asset.get('decoded_sha256') else None,
        'package_version': asset.get('package_version'),
    }


def _hash_evidence(files: list[dict]) -> dict:
    raw_by_hash = defaultdict(list)
    decoded_by_hash = defaultdict(list)
    for item in files:
        raw_hash = item['raw'].get('sha256')
        decoded = item.get('decoded')
        if raw_hash:
            raw_by_hash[raw_hash].append(item)
        if decoded and decoded.get('sha256'):
            decoded_by_hash[decoded['sha256']].append(item)

    exact_raw = [
        {'sha256': digest, 'paths': sorted(item.get('path') or '' for item in items)}
        for digest, items in sorted(raw_by_hash.items()) if len(items) > 1
    ]
    exact_decoded = [
        {'sha256': digest, 'paths': sorted(item.get('path') or '' for item in items)}
        for digest, items in sorted(decoded_by_hash.items()) if len(items) > 1
    ]
    decoded_to_raw = []
    for digest in sorted(set(decoded_by_hash) & set(raw_by_hash)):
        for decoded_item in decoded_by_hash[digest]:
            for raw_item in raw_by_hash[digest]:
                if decoded_item is raw_item:
                    continue
                decoded_to_raw.append({
                    'sha256': digest,
                    'decoded_path': decoded_item.get('path'),
                    'raw_path': raw_item.get('path'),
                    'note': 'Decoded bytes equal the other source file raw bytes; no behavioral equivalence inferred.',
                })
    return {
        'exact_raw_hash_groups': exact_raw,
        'exact_decoded_hash_groups': exact_decoded,
        'decoded_to_raw_hash_matches': decoded_to_raw,
        'different_raw_bytes': len({item['raw'].get('sha256') for item in files
                                    if item['raw'].get('sha256')}) > 1,
    }


def _group_records(assets: list[dict], view: str) -> list[dict]:
    groups = defaultdict(list)
    for asset in assets:
        if not isinstance(asset, dict):
            continue
        side = _side(str(asset.get('source', '')))
        if side is None:
            continue
        logical = _logical_uris(asset)
        if view == 'logical-uri':
            keys = [item['logical_uri'] for item in logical]
        else:
            keys = [_physical_key(asset)]
        for key in keys:
            groups[key.casefold()].append((key, asset, side, logical))

    records = []
    for group_key, members in sorted(groups.items()):
        display_keys = sorted({key for key, _, _, _ in members})
        files = [_file_record(asset, side, logical) for _, asset, side, logical in members]
        android = [item for item in files if item['side'] == 'ANDROID_OR_RECOVERED']
        ios = [item for item in files if item['side'] == 'IOS']
        availability = 'BOTH' if android and ios else 'ANDROID_OR_RECOVERED_ONLY' if android else 'IOS_ONLY'
        records.append({
            'id': stable_id('platform-view', view, group_key),
            'view': view,
            'key': display_keys[0],
            'case_variants': display_keys,
            'availability': availability,
            'android_or_recovered_sources': sorted({item['source'] for item in android}),
            'ios_sources': sorted({item['source'] for item in ios}),
            'files': files,
            'hash_evidence': _hash_evidence(files),
            'confidence': 'PROVEN_PATH_OR_HASH_FACTS_ONLY',
            'behavioral_equivalence': 'NOT_ESTABLISHED',
        })
    return records


def compare_assets(assets: list[dict]) -> dict:
    """Return source-relative and logical-URI platform comparison views.

    Include `assets` rows from Android data/original-cache/runtime-original-
    cache and any `ios*` source labels. The result keeps package map evidence,
    raw hashes, optional decoded hashes, source roots, and build versions. It
    does not mutate rows, write files, pick a canonical variant, or imply that
    two matching names/contents are behaviorally interchangeable.
    """
    assets = assets if isinstance(assets, list) else []
    return {
        'path_view': _group_records(assets, 'source-relative-path'),
        'logical_uri_view': _group_records(assets, 'logical-uri'),
        'policy': ('Platform availability is source-presence metadata. Exact hashes establish byte identity only; '
                   'path aliases and differing versions/content do not establish behavioral equivalence.'),
    }
