"""Build and replay AI consumers through the actual shared native world library."""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]
WSL_REPO = '/mnt/c/Users/adamc/Desktop/workspace/DH_sc'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(*args):
    result = subprocess.run(['wsl.exe', *args], capture_output=True, text=True, timeout=120)
    assert result.returncode == 0, (result.returncode, result.stdout, result.stderr)
    assert not result.stderr.strip(), result.stderr
    return result.stdout.strip()


def snapshot():
    result = {}
    for module in ('level-world', 'game-data', 'engine-animation', 'engine-skinning',
                   'engine-resources', 'engine-textures', 'engine-math',
                   'scene-materials', 'physics-backend'):
        for path in sorted((REPO / 'port' / module).rglob('*')):
            if path.is_file() and (path.suffix in ('.cpp', '.hpp', '.h', '.c', '.cc', '.inc',
                                                  '.inl', '.ipp', '.cmake') or path.name == 'CMakeLists.txt'):
                result[path.relative_to(REPO).as_posix()] = sha(path)
    result[Path(__file__).relative_to(REPO).as_posix()] = sha(Path(__file__))
    return result


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--build', default='/home/adampalace/dh2-world-build')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.output.exists():
        raise RuntimeError('Refusing to overwrite main-library AI evidence')
    arm_path = ROOT / 'reports/character-animation-ai-arm64-differential.json'
    arm = json.loads(arm_path.read_bytes())
    assert arm['validation'] == 'PASS' and arm['mismatches'] == 0
    assert all(sha(REPO / name) == value for name, value in arm['source_sha256'].items())
    corpus = ROOT / 'reference/character-animation-ai/consumer-fixtures.bin'
    assert sha(corpus) == arm['corpus_sha256']
    before = snapshot()
    run('cmake', '-S', WSL_REPO + '/port/level-world', '-B', args.build)
    run('cmake', '--build', args.build, '--target', 'character_animation_ai_audit', '--parallel', '8')
    assert snapshot() == before, 'Source inputs changed during build; retry after workers finish'
    executable = args.build + '/character_animation_ai_audit'
    elf = run('readelf', '-d', executable)
    assert 'libdh2_level_world.so' in elf
    paths = {executable}
    paths.update(token for token in run('ldd', executable).split()
                 if token.startswith(args.build + '/') and token.endswith('.so'))
    hashes = {path: run('sha256sum', path).split()[0] for path in sorted(paths)}
    assert any(path.endswith('/libdh2_level_world.so') for path in hashes)
    world = next(path for path in hashes if path.endswith('/libdh2_level_world.so'))
    symbols = run('nm', '-D', '--defined-only', world)
    for symbol in ('dh2_character_animation_ai', 'dh2_character_animation_has_combo',
                   'dh2_character_animation_table_id'):
        assert symbol in symbols
    world_dynamic = run('readelf', '-d', world)
    assert 'libasan.so' in world_dynamic and 'libubsan.so' in world_dynamic
    audit = json.loads(run('env', 'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1',
                          'UBSAN_OPTIONS=halt_on_error=1', executable,
                          WSL_REPO + '/port/level-world/reference/character-animation-ai/consumer-fixtures.bin'))
    assert audit['validation'] == 'PASS' and audit['mismatches'] == 0
    for field, expected in [('consumer_comparisons', 2197), ('ordered_service_requests', 4530),
                            ('scalar_comparisons', 90), ('atomic_rejection_checks', 17)]:
        assert audit[field] == arm[field] == expected
    assert snapshot() == before, 'Source inputs changed during replay'
    assert all(run('sha256sum', path).split()[0] == value for path, value in hashes.items())
    report = dict(validation='PASS', host_audit=audit, source_sha256=before,
                  binary_sha256=hashes, original_sha256=arm['original_sha256'],
                  corpus_sha256=sha(corpus), arm64_report_sha256=sha(arm_path),
                  sanitizers=['AddressSanitizer', 'UndefinedBehaviorSanitizer'], sanitizer_findings=0,
                  scope='Current CMake shared-library source integration and original-derived AI consumer replay. '
                        'Borrowed controller, target, scheduler and AIS services remain explicit fixtures. '
                        'No Android package, live observer wiring or whole-game proof.')
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(dict(validation='PASS', host_audit=audit, libraries=len(hashes)-1,
                          report=str(args.output))))


if __name__ == '__main__':
    main()
