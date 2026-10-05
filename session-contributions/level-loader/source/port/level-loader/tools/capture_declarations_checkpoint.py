"""Bind the internal declaration milestone; leave prior preview receipts intact."""
import hashlib
import json
import pathlib

root = pathlib.Path(__file__).resolve().parents[3]
loader = root/'port/level-loader'
build = root.parent/'build'

def digest(path):
    with path.open('rb') as file:
        return hashlib.file_digest(file, 'sha256').hexdigest()

receipts = {}
for name in ('fixed-declarations-host.json', 'fixed-declarations-sanitizers.json'):
    path = loader/'reports'/name
    report = json.loads(path.read_text())
    assert report['validation'] == 'PASS'
    assert report['summary'] == {'source_blocked': 35, 'source_compared': 16}
    assert report['occurrences_compared'] == 2331
    assert not report['runtime_objects_verified'] and not report['full_loader_verified']
    for source, expected in report['sources_sha256'].items():
        assert digest(loader/source) == expected, source
    assert digest(loader/'reports/fixed-map-level-coverage.json') == report['map_coverage_sha256']
    variant = 'host-xml' if name == 'fixed-declarations-host.json' else 'host-sanitizers'
    assert digest(build/variant/'dh2_loader_fixed_declarations_probe') == report['probe_sha256']
    receipts[name] = digest(path)

sources = {str(p.relative_to(root)): digest(p) for p in loader.glob('*')
           if p.suffix in ('.cpp', '.hpp') or p.name == 'CMakeLists.txt'}
for p in (loader/'tests').glob('fixed_declarations*'):
    sources[str(p.relative_to(root))] = digest(p)
# This capture records current declaration builds. The older map checkpoint
# remains a historical milestone and is not represented as current rendering.
binaries = {}
for variant in ('host-xml', 'host-sanitizers', 'android-arm64', 'android-x86_64'):
    for name in ('libdh2_level_loader.a', 'dh2_loader_fixed_declarations_probe'):
        binaries[variant+'/'+name] = digest(build/variant/name)
swamp_path = loader/'reports/swamp-fixed-declarations.json'
swamp = json.loads(swamp_path.read_text())
assert swamp['types']['Character'] == 50 and swamp['types']['OpenableContainer'] == 5
report = {'validation': 'PASS', 'scope': __doc__, 'sources_sha256': sources,
    'binaries_sha256': binaries, 'receipts_sha256': receipts,
    'swamp_declarations_sha256': digest(swamp_path), 'occurrences_compared': 2331,
    'fixed_levels_compared': 16, 'source_blocked': 35,
    'android_execution_of_declarations_verified': False,
    'runtime_factory_contract_agreed': False, 'runtime_objects_verified': False,
    'full_loader_verified': False}
(loader/'reports/declarations-checkpoint.json').write_text(json.dumps(report, indent=2)+'\n')
print(json.dumps({'validation': 'PASS', 'sources': len(sources), 'binaries': len(binaries),
                  'receipts': len(receipts), 'occurrences_compared': 2331}))
