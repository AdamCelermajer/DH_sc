"""Stage the reviewed Adam v69 URI closure, verifying every input digest.

Original resources come from the locally supplied cache. Adam's exact pinned
Git blobs supply his derived keyboard repair and decoded audio where needed.
Personal save files and the full cache archive are never staged.
"""
import argparse, hashlib, json, subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
COMMIT = '791e961b12233100b303038c961666834f4beb9d'

def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cache-files', type=Path, required=True)
    p.add_argument('--adam', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    manifest = ROOT/'port/engine-ui/reference/adam-menu-v69/asset-staging-manifest.json'
    m = json.loads(manifest.read_text(encoding='utf-8'))
    assert m['commit'] == COMMIT
    cache = {f.relative_to(a.cache_files).as_posix().lower(): f
             for f in a.cache_files.rglob('*') if f.is_file()}
    destinations = {}
    scene = json.loads((manifest.parent/'scene-assets.json').read_text(encoding='utf-8'))
    for row in m['entries'] + scene:
        previous = destinations.setdefault(row['asset'], row)
        assert previous['sha256'] == row['sha256']
    output = ROOT/'port/android-native/app/src/main/assets'
    receipts = []
    for asset, row in destinations.items():
        assert not Path(asset).is_absolute() and '..' not in Path(asset).parts
        destination = output/asset
        candidate = cache.get(row.get('cache_path',asset.removeprefix('original-cache/')).lower())
        raw = candidate.read_bytes() if candidate else b''
        source = 'supplied-cache'
        if hashlib.sha256(raw).hexdigest() != row['sha256']:
            raw = subprocess.check_output(['git','cat-file','blob',row['git_blob']], cwd=a.adam) if row.get('git_blob') else subprocess.check_output(['git','-c','core.longpaths=true','show',COMMIT+':'+row['source_path']], cwd=a.adam)
            source = 'reviewed-adam-blob'
        assert len(raw) == row['bytes'] and hashlib.sha256(raw).hexdigest() == row['sha256'], asset
        if destination.exists():
            assert destination.read_bytes() == raw, ('Existing asset differs', asset)
        else:
            destination.parent.mkdir(parents=True, exist_ok=True)
            destination.write_bytes(raw)
        receipts.append(dict(asset=asset, bytes=len(raw), sha256=row['sha256'], source=source))
    report = dict(validation='PASS', commit=COMMIT, assets=receipts,
                  manifest_sha256=hashlib.sha256(manifest.read_bytes()).hexdigest())
    a.report.parent.mkdir(parents=True, exist_ok=True)
    a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(dict(validation='PASS',assets=len(receipts),bytes=sum(r['bytes'] for r in receipts))))

if __name__ == '__main__':
    main()
