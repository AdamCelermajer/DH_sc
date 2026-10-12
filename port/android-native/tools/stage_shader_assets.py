"""Bundle the unmodified original shader pack and each checked member.

This stages source bytes, not original material selection or visual parity.
It preserves every existing prototype asset and refuses conflicting files.
"""
import argparse
import hashlib
import io
import json
from pathlib import Path, PurePosixPath
import zipfile

ROOT = Path(__file__).resolve().parents[3]
CACHE_SHA = '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
PACK_SHA = '365a4d3c432454c44208ebb484c7a472a3a4534a0c5c9e77a7a90f3b87b1b5c0'


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', type=Path, required=True)
    parser.add_argument('--studio', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    cache_raw = args.cache.read_bytes()
    assert sha(cache_raw) == CACHE_SHA, 'Unexpected original cache'
    with zipfile.ZipFile(io.BytesIO(cache_raw)) as cache:
        names = [name for name in cache.namelist() if PurePosixPath(name).name.lower() == 'shaders.pak']
        assert len(names) == 1, names
        pack = cache.read(names[0])
    assert len(pack) == 44194 and sha(pack) == PACK_SHA
    staged = {'shaders/shaders.pak': pack}
    with zipfile.ZipFile(io.BytesIO(pack)) as archive:
        assert len(archive.infolist()) == 34
        for item in archive.infolist():
            path = PurePosixPath(item.filename)
            assert len(path.parts) == 1 and not path.is_absolute() and path.name not in ('.', '..')
            assert not item.is_dir() and item.file_size <= 65536
            key = 'shaders/' + path.name
            assert key not in staged, 'Duplicate shader member'
            staged[key] = archive.read(item)
    roots = [ROOT/'port/android-native/app/src/main/assets', args.studio/'app/src/main/assets']
    prior = [{p.relative_to(root).as_posix(): sha(p.read_bytes()) for p in root.rglob('*') if p.is_file()} for root in roots]
    assert prior[0] == prior[1], 'Repository and Studio assets diverged'
    for root in roots:
        for key, raw in staged.items():
            path = root/key
            if path.exists():
                assert path.read_bytes() == raw, ('Existing shader differs', str(path))
    for root in roots:
        for key, raw in staged.items():
            path = root/key
            path.parent.mkdir(parents=True, exist_ok=True)
            if not path.exists():
                path.write_bytes(raw)
    current = [{p.relative_to(root).as_posix(): sha(p.read_bytes()) for p in root.rglob('*') if p.is_file()} for root in roots]
    assert current[0] == current[1]
    assert all(current[0][key] == digest for key, digest in prior[0].items())
    result = dict(validation='PASS', cache_sha256=CACHE_SHA, cache_member=names[0], pack_sha256=PACK_SHA,
                  shader_member_count=34, asset_sha256={key: sha(raw) for key, raw in staged.items()},
                  assets_per_project=len(current[0]), original_bytes_unmodified=True,
                  full_game_assets=False, original_material_selection=False, live_validation='NOT_RUN')
    args.output.parent.mkdir(parents=True, exist_ok=True)
    if args.output.exists():
        assert json.loads(args.output.read_text()) == result, 'Preserve earlier shader stage'
    else:
        args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps(dict(validation='PASS', shader_members=34, assets_per_project=len(current[0]))))


if __name__ == '__main__':
    main()
