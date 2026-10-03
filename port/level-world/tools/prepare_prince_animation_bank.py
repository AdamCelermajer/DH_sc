"""Stage every exact cache resource requested by the original Prince setup.

Registration requests and first-unique resource order are retained independently;
library deduplication and live loading are not inferred by this asset preparer.
"""
import argparse
import hashlib
import json
import re
import zipfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
CACHE_SHA = '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
ORIGINAL_SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', type=Path, default=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'))
    parser.add_argument('--producer', type=Path, default=REPO / 'port/engine-animation/reference/prince-registration/probe.json')
    parser.add_argument('--project', type=Path, default=REPO / 'port/android-native')
    parser.add_argument('--report', type=Path, required=True)
    parser.add_argument('--producer-snapshot', type=Path, required=True)
    args = parser.parse_args()
    if args.report.exists() or args.producer_snapshot.exists():
        raise RuntimeError('Refusing to overwrite staging evidence or its producer snapshot')
    producer_bytes = args.producer.read_bytes()
    producer = json.loads(producer_bytes)
    assert producer['validation'] == 'PASS'
    assert producer['original_sha256'] == ORIGINAL_SHA and producer['cache_sha256'] == CACHE_SHA
    with args.cache.open('rb') as stream:
        assert hashlib.file_digest(stream, 'sha256').hexdigest() == CACHE_SHA
    resources = producer['resources']
    calls = producer['registration_calls']
    assert len(resources) == producer['unique_clip_count'] == 116
    assert len(calls) == producer['registration_calls_count'] == 158
    assert calls[0]['kind'] == 'template' and calls[0]['clip_id'] == 1111
    assert len({row['clip_id'] for row in resources}) == len(resources)
    ids = [row['clip_id'] for row in resources]
    assert ids == list(dict.fromkeys(call['clip_id'] for call in calls)) == producer['first_unique_order']
    assets = args.project.resolve() / 'app/src/main/assets'
    payloads = []
    destinations = set()
    # Validate the entire input set and all existing destinations before writing.
    with zipfile.ZipFile(args.cache) as cache:
        for row in resources:
            filename = row['path'].replace('\\', '/').rsplit('/', 1)[-1]
            assert re.fullmatch(r'[a-zA-Z0-9_-]+\.bdae', filename), filename
            destination = assets / 'animations' / filename
            assert destination not in destinations, ('Flat filename collision', filename)
            destinations.add(destination)
            raw = cache.read(row['entry'])
            assert len(raw) == row['bytes'] and digest(raw) == row['sha256']
            reused = destination.exists()
            if reused:
                assert destination.read_bytes() == raw, ('Conflicting existing animation', filename)
            payloads.append((destination, raw, reused, dict(clip_id=row['clip_id'],
                              authored_path=row['path'], asset='animations/' + filename,
                              cache_entry=row['entry'], bytes=len(raw), sha256=digest(raw))))
    assert args.producer.read_bytes() == producer_bytes, 'Producer changed during preflight'
    manifest = dict(cache_sha256=CACHE_SHA, original_sha256=ORIGINAL_SHA,
                    character=producer['character'], animation_table=producer['animation_table'],
                    animation_set_id=producer['animation_set_id'], producer_sha256=digest(producer_bytes),
                    template_clip_id=1111, registration_requests=[call['clip_id'] for call in calls],
                    first_unique_resource_order=ids, resources=[row for _, _, _, row in payloads],
                    scope='Original source registration request inputs for asset loading. '
                          'This does not assert library deduplication/order, current renderer loading '
                          'or full game asset coverage.')
    for destination, raw, reused, _ in payloads:
        if not reused:
            destination.parent.mkdir(parents=True, exist_ok=True)
            destination.write_bytes(raw)
        assert destination.read_bytes() == raw
    manifest_path = assets / 'data/prince-animation-bank.json'
    manifest_path.parent.mkdir(parents=True, exist_ok=True)
    manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')
    args.producer_snapshot.parent.mkdir(parents=True, exist_ok=True)
    args.producer_snapshot.write_bytes(producer_bytes)
    report = dict(validation='PASS', resources=len(payloads), registration_requests=len(calls),
                  added_resources=sum(not reused for _, _, reused, _ in payloads),
                  reused_resources=sum(reused for _, _, reused, _ in payloads),
                  bytes=sum(len(raw) for _, raw, _, _ in payloads), cache_sha256=CACHE_SHA,
                  producer_snapshot=str(args.producer_snapshot), producer_sha256=digest(producer_bytes),
                  asset_manifest=str(manifest_path), asset_manifest_sha256=digest(manifest_path.read_bytes()),
                  preparer_sha256=digest(Path(__file__).read_bytes()),
                  scope='Exact original-cache resources staged in source APK assets only. '
                        'No new APK, live full-bank loader or Studio project synchronization claim.')
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
