"""Stage original menu/HUD/font bytes inside APK assets without rewriting them.

The original-cache namespace preserves original resource URIs separately from
the prototype's generated tables. Its index supplies exact case resolution.
This is a UI asset stage, not a claim that the full game cache is bundled.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import zipfile

ROOT = Path(__file__).resolve().parents[3]
CACHE_SHA = '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
PREFIX = 'com.gameloft.android.GAND.GloftD2SS/files/'


def digest(path):
    h = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(8*1024*1024), b''):
            h.update(block)
    return h.hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    group=parser.add_mutually_exclusive_group()
    group.add_argument('--texts', action='store_true', help='Stage all original text sheets and their metadata instead of movies/fonts')
    group.add_argument('--textures', action='store_true', help='Stage the six source-bound external UI textures')
    args = parser.parse_args()
    assert not args.output.exists(), 'Preserve the existing asset stage receipt'
    assert digest(args.cache) == CACHE_SHA, 'Unexpected canonical cache bytes'
    assets = ROOT/'port/android-native/app/src/main/assets'
    before = {p.relative_to(assets).as_posix(): digest(p) for p in assets.rglob('*') if p.is_file()}
    selected = {}
    texture_bindings = {}
    if args.textures:
        proof = json.loads((ROOT/'port/scene-materials/reference/swf-render-connection/connection-probe.json').read_text())
        assert proof['validation']=='PASS' and proof['canonical_cache_sha256']==CACHE_SHA
        texture_bindings = {row['canonical_cache_file']:row for row in proof['canonical_cache_bindings']}
        assert len(texture_bindings)==6
    with zipfile.ZipFile(args.cache) as cache:
        for item in cache.infolist():
            if item.is_dir():
                continue
            assert item.filename.startswith(PREFIX), item.filename
            uri = item.filename[len(PREFIX):]
            path = PurePosixPath(uri)
            assert not path.is_absolute() and '..' not in path.parts and '\\' not in uri
            key = uri.casefold()
            wanted = (key.startswith('data/menus/') or path.suffix.casefold() == '.ttf'
                      or 'menusgraphics' in key or key == 'gameswf_effects.bdae')
            if args.texts:
                wanted = ((key.startswith('data/text/') and path.name != '.nomedia')
                          or key.startswith('data/pydata/common_text_'))
            if args.textures:
                wanted = item.filename in texture_bindings
            if not wanted:
                continue
            assert key not in selected, ('Ambiguous original resource case', uri)
            raw = cache.read(item)
            if args.textures:
                assert len(raw)==texture_bindings[item.filename]['size']
                assert hashlib.sha256(raw).hexdigest()==texture_bindings[item.filename]['sha256']
            target = assets/'original-cache'/uri
            if target.exists():
                assert target.read_bytes() == raw, ('Conflicting original asset', uri)
            selected[key] = dict(uri=uri, asset='original-cache/'+uri,
                                 bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest(),
                                 cache_member=item.filename)
        # Validate the complete selection before creating any asset files.
        movies = [row for row in selected.values() if row['uri'].casefold().endswith('.swf')]
        fonts = [row for row in selected.values() if row['uri'].casefold().endswith('.ttf')]
        if args.textures:
            assert len(selected)==6
        elif args.texts:
            assert len([key for key in selected if key.startswith('data/text/')]) == 333
            assert len([key for key in selected if key.startswith('data/pydata/')]) == 4
        else:
            assert len(movies) == 30 and len(fonts) == 7
            assert 'data/menus/sct_font_3.fnt' in selected and 'gameswf_effects.bdae' in selected
        for row in selected.values():
            target = assets/row['asset']
            target.parent.mkdir(parents=True, exist_ok=True)
            if not target.exists():
                target.write_bytes(cache.read(row['cache_member']))
    index = dict(version=1, cache_sha256=CACHE_SHA, resources=selected,
                 matching='Exact URI first, then unique casefold key after original filename projection')
    index_path = assets/'original-cache'/('texture-index.json' if args.textures else 'text-index.json' if args.texts else 'index.json')
    encoded = (json.dumps(index, indent=2)+'\n').encode()
    if index_path.exists():
        assert index_path.read_bytes() == encoded, 'Conflicting original asset index'
    else:
        index_path.write_bytes(encoded)
    for row in selected.values():
        assert digest(assets/row['asset']) == row['sha256']
    assert all(digest(assets/key) == value for key, value in before.items())
    report = dict(validation='PASS', cache_sha256=CACHE_SHA,
                  resource_count=len(selected), movies=len(movies), ttf_fonts=len(fonts),
                  resources=selected, index_sha256=hashlib.sha256(encoded).hexdigest(),
                  text_stage=args.texts,
                  texture_stage=args.textures,
                  unmodified_original_bytes=True, prior_assets_preserved=len(before),
                  assets_per_project=sum(p.is_file() for p in assets.rglob('*')),
                  studio_staged=False, full_game_assets=False, packaged_apk=False,
                  live_validation='NOT_RUN')
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps({key: report[key] for key in ('validation','resource_count','movies','ttf_fonts','assets_per_project')}))


if __name__ == '__main__':
    main()
