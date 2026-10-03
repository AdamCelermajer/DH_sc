"""Host DSO integration/memory proof; this is not a full-bank pose differential."""
import argparse
from collections import Counter
import hashlib
import json
import shlex
import struct
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
ASSETS = ROOT / 'port/android-native/app/src/main/assets'
MANIFEST = ASSETS / 'data/prince-animation-bank.json'
PROBE = ROOT / 'port/engine-animation/reference/prince-registration/probe.json'
LOCAL = ROOT / '.local-inputs/prince-bank-integration'
FIXTURE = ROOT / 'port/engine-animation/reference/prince-bank-integration/bank-fixture.bin'
TEST = ROOT / 'port/engine-animation/tests/prince_bank_integration.cpp'

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def rel(path):
    return path.relative_to(ROOT).as_posix()

def wslpath(path):
    text = str(path.resolve()).replace('\\', '/')
    return '/mnt/' + text[0].lower() + text[2:]

def sources():
    result = {}
    for directory in ('engine-animation', 'scene-materials', 'engine-math', 'engine-resources', 'asset-payloads'):
        base = ROOT / 'port' / directory
        for pattern in ('*.cpp', '*.hpp', 'CMakeLists.txt'):
            for path in base.glob(pattern):
                result[rel(path)] = sha(path)
    for path in (TEST, Path(__file__)):
        result[rel(path)] = sha(path)
    return result

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-dir', default='/home/adampalace/dh2-world-build')
    parser.add_argument('--output', type=Path, default=ROOT / 'port/engine-animation/reports/prince-bank-integration-host.json')
    args = parser.parse_args()
    manifest = json.loads(MANIFEST.read_text())
    probe = json.loads(PROBE.read_text())
    assert sha(MANIFEST) == '76633bb4f0ab5f645f4516e407e1f926df61641ea66faa805f68da57f043eb81'
    assert probe['validation'] == 'PASS'
    assert manifest['original_sha256'] == probe['original_sha256'] == sha(ROOT / '.local-inputs/libDungeonHunter2.so')
    assert manifest['cache_sha256'] == probe['cache_sha256']
    requests = manifest['registration_requests']
    assert requests == [call['clip_id'] for call in probe['registration_calls']]
    assert len(requests) == 158 and len(manifest['resources']) == len(set(requests)) == 116
    assert manifest['template_clip_id'] == probe['template']['clip_id'] == 1111
    projections = probe['current17_projection']
    assert len(projections) == 17
    expected = {resource['clip_id']: resource for resource in probe['resources']}
    bound = {}
    def input_file(path, expected_hash, expected_size):
        assert path.is_relative_to(ASSETS) and path.stat().st_size == expected_size
        assert sha(path) == expected_hash, str(path)
        bound[rel(path)] = {'sha256': expected_hash, 'bytes': expected_size}
    model = ASSETS / 'models/prince_modular.bdae'
    input_file(model, probe['model_property']['sha256'], probe['model_property']['bytes'])
    data = bytearray(struct.pack('<5I', 0x314b4250, 116, 158, 17, 1111))
    def string(value):
        encoded = value.encode('utf-8')
        data.extend(struct.pack('<I', len(encoded))); data.extend(encoded)
    string('models/prince_modular.bdae')
    for resource in manifest['resources']:
        record = expected[resource['clip_id']]
        assert record['sha256'] == resource['sha256'] and record['bytes'] == resource['bytes']
        input_file(ASSETS / resource['asset'], resource['sha256'], resource['bytes'])
        # The zero-track dome-warning clip retains INT_MIN end as unsigned
        # 0x80000000 in the original probe. Preserve its exact 32-bit words.
        data.extend(struct.pack('<III', resource['clip_id'], record['start'] & 0xffffffff, record['end'] & 0xffffffff))
        string(resource['asset'])
    data.extend(struct.pack('<158i', *requests))
    for projection in projections:
        assert requests.index(projection['clip_id']) == projection['first_library_index']
        data.extend(struct.pack('<ii', projection['clip_id'], projection['first_library_index']))
    FIXTURE.parent.mkdir(parents=True, exist_ok=True)
    FIXTURE.write_bytes(data)
    LOCAL.mkdir(parents=True, exist_ok=True)
    before = sources()
    commands = []
    def run(command):
        result = subprocess.run(['wsl', 'bash', '-lc', command], capture_output=True, text=True)
        commands.append({'command': command, 'returncode': result.returncode, 'stdout': result.stdout, 'stderr': result.stderr})
        if result.returncode:
            raise RuntimeError(json.dumps(commands[-1], indent=2))
        return result.stdout
    q = shlex.quote
    build = args.build_dir.rstrip('/')
    animation_dir = build + '/engine-skinning/engine-animation'
    scene_dir = animation_dir + '/scene-materials'
    animation = animation_dir + '/libdh2_engine_animation.so'
    scene = scene_dir + '/libdh2_scene_materials.so'
    executable = wslpath(LOCAL / 'host_audit')
    run('cmake --build ' + q(build) + ' --target dh2_engine_animation dh2_scene_materials -j2')
    run('g++ --version')
    configuration = run('cmake -LA -N ' + q(build))
    assert '-fsanitize=address,undefined' in configuration
    run('g++ -std=c++17 -O1 -g -fno-fast-math -ffp-contract=off -fsanitize=address,undefined -fno-omit-frame-pointer -Wall -Wextra -Werror ' +
        q(wslpath(TEST)) + ' -L' + q(animation_dir) + ' -L' + q(scene_dir) +
        ' -ldh2_engine_animation -ldh2_scene_materials -Wl,-rpath,' + q(animation_dir) + ' -Wl,-rpath,' + q(scene_dir) + ' -o ' + q(executable))
    dependencies = run('ldd ' + q(executable))
    assert animation in dependencies and scene in dependencies and 'libasan.so' in dependencies and 'libubsan.so' in dependencies
    run('readelf -d ' + q(animation) + ' ' + q(scene))
    output = run('ASAN_OPTIONS=detect_leaks=1:abort_on_error=1 UBSAN_OPTIONS=halt_on_error=1 ' + q(executable) + ' ' + q(wslpath(FIXTURE)) + ' ' + q(wslpath(ASSETS)))
    checks = json.loads(output)
    assert checks['validation'] == 'PASS' and checks['resources'] == 116 and checks['original_current17_mappings'] == 17
    binaries = {}
    for path in (executable, animation, scene):
        binaries[path] = run('sha256sum ' + q(path)).split()[0]
    after = sources()
    assert before == after, 'Source changed during build/replay'
    for name, binding in bound.items():
        assert sha(ROOT / name) == binding['sha256'], 'Asset changed during replay'
    report = {
        'validation': 'PASS', 'checks': checks, 'original_sha256': manifest['original_sha256'],
        'cache_sha256': manifest['cache_sha256'], 'manifest_sha256': sha(MANIFEST),
        'original_registration_probe': {'path': rel(PROBE), 'sha256': sha(PROBE)},
        'fixture': {'path': rel(FIXTURE), 'sha256': sha(FIXTURE), 'bytes': len(data)},
        'input_sha256': bound, 'source_sha256': before, 'binary_sha256': binaries,
        'commands': commands, 'linked_dependencies': dependencies, 'sources_unchanged_through_replay': True,
        'original_compiled_bounds_checked': 158,
        'original_distinct_resource_raw_track_types': dict(sorted(Counter({key: sum(resource['channel_type_counts'].get(key, 0) for resource in probe['resources']) for key in {key for resource in probe['resources'] for key in resource['channel_type_counts']}}).items())),
        'scope': 'Actual staged 116-resource bank and 158 source registration requests; canonical identity equals clip ID. Production shared animation/scene libraries execute registration, retain1 dynamic compilation and all clip/target start/end samples after borrowed Players are destroyed. The 17 first indices and clip bounds are checked against the original-instruction registration probe. This verifies integration, ownership and sanitizer safety; it is not full-bank original pose, GPU, actor event/clock, equipment, factory ownership or APK parity.'
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({'report': str(args.output), 'sha256': sha(args.output), 'checks': checks}, indent=2))

if __name__ == '__main__':
    main()
