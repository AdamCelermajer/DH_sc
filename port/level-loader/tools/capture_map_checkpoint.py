"""Bind the internal geometry/navigation milestone to sources and four builds."""
import hashlib
import json
import pathlib

ROOT = pathlib.Path(__file__).resolve().parents[3]
LOADER = ROOT / 'port/level-loader'
BUILD = ROOT.parent / 'build'

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

sources = {p for p in LOADER.rglob('*') if p.is_file() and
           'reports' not in p.relative_to(LOADER).parts and '__pycache__' not in p.parts}
for folder in ('asset-payloads', 'engine-resources', 'engine-math', 'scene-materials', 'level-world'):
    sources.update(p for p in (ROOT / 'port' / folder).iterdir()
                   if p.is_file() and p.suffix in ('.cpp', '.hpp', '.h', '.cmake'))
binaries = {}
for platform in ('host-xml', 'host-sanitizers', 'android-arm64', 'android-x86_64'):
    directory = BUILD / platform
    paths = sorted(directory.glob('libdh2_loader_*.a'))
    paths += [directory / 'libdh2_level_loader.a']
    paths += sorted(p for p in directory.glob('dh2_loader_*') if p.is_file())
    for path in paths:
        if not path.is_file():
            raise RuntimeError(f'Missing binary {path}')
        binaries[path.relative_to(BUILD).as_posix()] = sha(path)
    for required in ('dh2_loader_fixed_map_probe', 'dh2_loader_visual_transform_probe',
                     'dh2_loader_user_properties_probe'):
        if not (directory / required).is_file():
            raise RuntimeError(f'Missing required probe {platform}/{required}')
receipts = {}
names = ('xml-original-comparison.json', 'resource-path-differential.json',
         'fixed-sources-level-coverage.json', 'swamp-fixed-map-host.json',
         'swamp-fixed-map-sanitizers.json', 'fixed-map-level-coverage.json',
         'visual-transform-differential.json', 'user-properties-differential.json')
for name in names:
    path = LOADER / 'reports' / name
    row = json.loads(path.read_text())
    if name in ('swamp-fixed-map-host.json', 'swamp-fixed-map-sanitizers.json'):
        for relative, expected in row['sources_sha256'].items():
            if sha(LOADER / relative) != expected:
                raise RuntimeError(f'Stale receipt {name}: {relative}')
    receipts[name] = sha(path)
coverage = json.loads((LOADER / 'reports/fixed-map-level-coverage.json').read_text())
report = {
    'scope': 'Internal native map assembly checkpoint; not a finished loader or renderer.',
    'source_sha256': {p.relative_to(ROOT).as_posix(): sha(p) for p in sorted(sources)},
    'binary_sha256': binaries, 'receipt_sha256': receipts,
    'level_coverage': coverage['summary'], 'map_render_verified': False,
    'runtime_factory_abi_agreed': False, 'integrated_APK_built': False,
    'full_loader_verified': False,
}
(LOADER / 'reports/map-assembly-checkpoint.json').write_text(json.dumps(report, indent=2) + '\n')
print(json.dumps({'sources': len(sources), 'binaries': len(binaries),
                  'receipts': len(receipts), 'coverage': coverage['summary'],
                  'map_render_verified': False, 'full_loader_verified': False}))
