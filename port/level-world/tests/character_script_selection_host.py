"""Build and bind the sanitized source script-selector replay."""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
WSL_ROOT = '/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/level-world'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(*args):
    result = subprocess.run(['wsl.exe', *args], capture_output=True, text=True, timeout=120)
    assert result.returncode == 0, (result.returncode, result.stdout, result.stderr)
    assert not result.stderr.strip(), result.stderr
    return result.stdout.strip()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--build', default='/home/adampalace/dh2-world-build')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.output.exists():
        raise RuntimeError('Refusing to overwrite selector host evidence')
    reference_dir = ROOT / 'reference/character-script-selection'
    differential = reference_dir / 'differential.json'
    arm = json.loads(differential.read_bytes())
    assert arm['validation'] == 'PASS' and arm['mismatches'] == 0
    sources = {name: sha(ROOT / name) for name in arm['source_sha256']}
    assert sources == arm['source_sha256']
    for name in ['CMakeLists.txt', 'tests/character_script_selection.cpp',
                 'tests/character_script_selection_host.py']:
        sources[name] = sha(ROOT / name)
    reference = reference_dir / 'selection-reference.bin'
    assert sha(reference) == arm['reference_sha256']
    run('cmake', '-S', WSL_ROOT, '-B', args.build)
    run('cmake', '--build', args.build, '--target', 'character_script_selection_audit', '--parallel', '8')
    assert all(sha(ROOT / name) == expected for name, expected in sources.items())
    executable = args.build + '/character_script_selection_audit'
    executable_hash = run('sha256sum', executable).split()[0]
    dependencies = run('readelf', '-d', executable)
    assert 'libasan.so' in dependencies and 'libubsan.so' in dependencies
    assert 'libdh2_' not in dependencies
    audit = json.loads(run('env', 'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1',
                          'UBSAN_OPTIONS=halt_on_error=1', executable,
                          WSL_ROOT + '/reference/character-script-selection/selection-reference.bin'))
    assert audit['validation'] == 'PASS' and audit['mismatches'] == 0
    assert audit['original_selection_cases'] == arm['comparisons'] == 1080
    assert audit['factory_callbacks'] == arm['ordered_factory_callbacks'] == 1080
    assert audit['atomic_rejections'] == 10 and audit['filename_identity_above_4gib']
    assert executable_hash == run('sha256sum', executable).split()[0]
    assert all(sha(ROOT / name) == expected for name, expected in sources.items())
    report = dict(validation='PASS', host_audit=audit, source_sha256=sources,
                  executable=executable, executable_sha256=executable_hash,
                  sanitizers=['AddressSanitizer', 'UndefinedBehaviorSanitizer'],
                  sanitizer_findings=0, reference_sha256=sha(reference),
                  arm64_report_sha256=sha(differential), original_sha256=arm['original_sha256'],
                  scope='Actual source script selection and synchronous factory ordering. '
                        'Allocation, active/pending ownership, backend construction and script lifecycle '
                        'remain explicit services. No full AIS or live APK claim.')
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
