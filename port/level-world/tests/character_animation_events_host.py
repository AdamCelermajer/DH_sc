"""Run and bind the source-linked sanitized animation routing replay."""
import argparse, hashlib, json, subprocess
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()
def run(*args):
    result = subprocess.run(['wsl', *args], capture_output=True, text=True)
    assert result.returncode == 0, (result.returncode, result.stdout, result.stderr)
    assert not result.stderr.strip(), result.stderr
    return result.stdout.strip()
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--executable', default='/home/adampalace/dh2-world-build/character_animation_events_audit')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.output.exists():
        raise RuntimeError('Refusing to overwrite host routing evidence')
    arm = ROOT / 'reports/character-animation-events-arm64-differential.json'
    kernel = json.loads(arm.read_bytes())
    assert kernel['validation'] == 'PASS' and kernel['mismatches'] == 0
    sources = {name: sha(ROOT / name) for name in kernel['source_sha256']}
    assert sources == kernel['source_sha256']
    executable_hash = run('sha256sum', args.executable).split()[0]
    reference = ROOT / 'reference/animation-event-routing/native-reference/routing-reference.bin'
    assert sha(reference) == kernel['reference_sha256']
    audit = json.loads(run('env', 'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1',
                          'UBSAN_OPTIONS=halt_on_error=1', args.executable,
                          '/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/level-world/' + str(reference.relative_to(ROOT)).replace('\\', '/')))
    assert audit['validation'] == 'PASS' and audit['mismatches'] == 0
    assert audit['original_routing_cases'] == kernel['comparisons'] == 1344
    assert audit['ordered_callbacks'] == kernel['ordered_callbacks'] == 2422
    assert audit['atomic_rejections'] == 8 and audit['payload_above_4gib']
    assert executable_hash == run('sha256sum', args.executable).split()[0]
    dependencies = run('readelf', '-d', args.executable)
    assert 'libasan.so' in dependencies and 'libubsan.so' in dependencies
    assert 'libdh2_' not in dependencies  # This audit directly compiles the kernel.
    for name, expected in sources.items():
        assert sha(ROOT / name) == expected
    sources['CMakeLists.txt'] = sha(ROOT / 'CMakeLists.txt')
    sources['tests/character_animation_events_host.py'] = sha(Path(__file__))
    report = dict(validation='PASS', host_audit=audit, source_sha256=sources,
                  executable=args.executable, executable_sha256=executable_hash,
                  sanitizers=['AddressSanitizer', 'UndefinedBehaviorSanitizer'],
                  sanitizer_findings=0, reference_sha256=sha(reference),
                  arm64_report_sha256=sha(arm), original_sha256=kernel['original_sha256'],
                  scope='Source-linked animation event routing only. Actual AI consumers and FSM are fixtures; no live APK or full AI behavior claim.')
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))
if __name__ == '__main__':
    main()
