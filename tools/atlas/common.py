"""Shared atlas I/O; paths always point at read-only original sources."""
import hashlib
import json
from pathlib import Path
from types import SimpleNamespace

ROOT = Path(__file__).resolve().parents[2]
READ_ONLY_INPUT_ROOTS = {ROOT / '.local-inputs', ROOT.parents[2] / '.local-inputs', ROOT / 'port'}

def context(inputs=None):
    if inputs:
        source = Path(inputs).resolve()
    else:
        candidates = [ROOT / '.local-inputs', ROOT.parents[2] / '.local-inputs']
        source = next((p for p in candidates if (p / 'assets-extra/android/data').is_dir()), None)
        if source is None:
            raise RuntimeError('Original data unavailable; use --inputs <read-only .local-inputs directory>.')
    READ_ONLY_INPUT_ROOTS.add(source)
    return SimpleNamespace(root=ROOT, inputs=source, out=ROOT / 'coordination/atlas')

def write_json(path, value):
    path = Path(path)
    resolved = path.resolve()
    if any(resolved.is_relative_to(source.resolve()) for source in READ_ONLY_INPUT_ROOTS):
        raise ValueError('Original source inputs are read-only: ' + str(path))
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2, sort_keys=True) + '\n', encoding='utf-8', newline='\n')

def read_json(path, default=None):
    path = Path(path)
    return json.loads(path.read_text(encoding='utf-8')) if path.exists() else default

def stable_id(*parts):
    return hashlib.sha256('|'.join(map(str, parts)).encode('utf-8')).hexdigest()[:20]

def source_roots(ctx):
    candidates = [('android',ctx.inputs / 'assets-extra/android/data'),
                  ('ios',ctx.inputs / 'assets-extra/ios/data'),
                  ('original-cache',ctx.inputs / 'assets-extra/android/original-cache/data'),
                  ('android-extra',ctx.inputs / 'assets-extra/android/3d'),
                  ('audio-cache',ctx.inputs / 'audio-v34/cache'),
                  ('ios-packed',ctx.inputs / 'claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/data'),
                  ('ios-glive',ctx.inputs / 'claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/GLIVE_RES'),
                  ('ios-app-resources',ctx.inputs / 'claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app'),
                  ('port-runtime-original-cache',ctx.root / 'port/android-native/app/src/main/assets/original-cache/data')]
    return [(label,p) for label,p in candidates if p.is_dir()]

def source_files(ctx):
    seen=set()
    for label,folder in source_roots(ctx):
        if label == 'ios-app-resources':
            # Only named game/app resources; do not recursively admit binaries,
            # signing files, third-party package branding or copied data packs.
            paths = [p for pattern in ('Default*.png', 'defaulteffects.bdae', 'DH2_*logo_iPad.mov',
                     'DQ_TRAILER_SUB-*.srt', 'response_*.txt', '*.lproj/InfoPlist.strings')
                     for p in folder.glob(pattern)]
        else:
            paths = folder.rglob('*')
        for p in sorted(paths):
            if p.is_file() and p not in seen:
                seen.add(p)
                yield label,p
    # Owner clarified that both partial caches are authoritative inputs and
    # assets present in either may resolve. Keep the explicit SDD authority.
    for label,p in [('audio-catalog',ctx.inputs/'audio-v34/cache/sounds.xml'),
                    ('sound-table',ctx.inputs/'items-discovery/sdd_dungeon_hunter_2_iphone_pyarray.bin'),
                    ('sound-table',ctx.inputs/'items-discovery/sdd_dungeon_hunter_2_iphone_pyarraynames.bin')]:
        if p.is_file() and p not in seen:yield label,p

def source_site(ctx, path):
    try:
        return '.local-inputs/' + Path(path).relative_to(ctx.inputs).as_posix()
    except ValueError:
        return '.repo/' + Path(path).relative_to(ctx.root).as_posix()


def physical_path(ctx, path):
    if path.startswith('.local-inputs/'):
        return ctx.inputs / path[len('.local-inputs/'):]
    if path.startswith('.repo/'):
        return ctx.root / path[len('.repo/'):]
    return ctx.root / path

def markdown_table(headers, rows):
    def cell(v):
        if isinstance(v, (list, dict)): v=json.dumps(v,ensure_ascii=False)
        return str(v).replace('|','\\|').replace('\n','<br>')
    return '\n'.join(['| ' + ' | '.join(headers) + ' |', '| ' + ' | '.join(['---']*len(headers)) + ' |'] +
                     ['| ' + ' | '.join(cell(v) for v in row) + ' |' for row in rows]) + '\n'
