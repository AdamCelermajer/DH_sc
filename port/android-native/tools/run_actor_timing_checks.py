"""Verify recovered movement, decor definitions and timelines in both APKs.

These kernels do not by themselves replace the development movement adapter.
The original fixtures retain explicit scene, selection and callback boundaries.
"""
import argparse
import hashlib
import json
import subprocess
import sys
import zipfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
LOCAL = REPO / '.local-inputs'
TESTS = REPO / 'port/level-world/tests'
REPORTS = REPO / 'port/level-world/reports'
SPECS = {
    'move-state': ('move_state', LOCAL / 'move-state-reference.bin', 4352),
    'decor-body-config': ('decor_body_config', LOCAL / 'decor-body-discovery/reference.bin', 4304),
    'visual-timeline': ('visual_timeline', REPO / 'port/level-world/reference/visual-timeline/kernel-fixtures.bin', 5970),
    'visual-timeline-replay': ('visual_timeline_replay', REPO / 'port/level-world/reference/visual-timeline/replay-fixtures.bin', 640),
}


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def wsl(path):
    path = path.resolve()
    return '/mnt/' + path.drive[0].lower() + path.as_posix()[2:]


def run(command):
    result = subprocess.run(command, cwd=REPO, text=True, capture_output=True)
    if result.returncode:
        raise RuntimeError(f'{command!r}\n{result.stdout}\n{result.stderr}')
    return result.stdout


def comparisons(report):
    for key in ('comparisons', 'original_derived_replay'):
        if key in report:
            return report[key]
    return report['mesh_comparisons'] + report['config_comparisons']


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--studio', type=Path, required=True)
    parser.add_argument('--host-build', default='/home/adampalace/dh2-world-build')
    parser.add_argument('--skip-host', action='store_true')
    parser.add_argument('--skip-packaged-module', action='append', default=[])
    args = parser.parse_args()
    if not args.skip_host:
        cache = run(['wsl', 'cat', args.host_build + '/CMakeCache.txt'])
        for prefix in ('CMAKE_CXX_FLAGS:STRING=', 'CMAKE_EXE_LINKER_FLAGS:STRING=', 'CMAKE_SHARED_LINKER_FLAGS:STRING='):
            assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
        for module, (stem, gold, count) in SPECS.items():
            report = json.loads(run(['wsl', args.host_build + '/' + stem + '_audit', wsl(gold)]))
            assert comparisons(report) == count and report['mismatches'] == 0
            report.update(reference_sha256=sha(gold), sanitizers=['address', 'undefined'])
            (REPORTS / f'actor-timing-{module}-host-audit.json').write_text(json.dumps(report, indent=2) + '\n')
            print(f'Host {module}: {count} pass', flush=True)
    apks = [('packaged', REPO / 'port/android-native/app/build/outputs/apk/debug/app-debug.apk'),
            ('studio', args.studio / 'app/build/outputs/apk/debug/app-debug.apk')]
    for tag, apk in apks:
        library = LOCAL / f'actor-timing-{tag}-world-arm64.so'
        with zipfile.ZipFile(apk) as archive:
            library.write_bytes(archive.read('lib/arm64-v8a/libdh2_level_world.so'))
        for module, (stem, gold, count) in SPECS.items():
            if tag + ':' + module in args.skip_packaged_module:
                continue
            output = LOCAL / f'actor-timing-{tag}-{module}-reference.bin'
            report_path = REPORTS / f'actor-timing-{tag}-{module}-arm64-differential.json'
            run([sys.executable, str(TESTS / (stem + '_differential.py')),
                 '--engine', str(LOCAL / 'libDungeonHunter2.so'), '--library', str(library),
                 '--reference-output', str(output), '--report', str(report_path)])
            report = json.loads(report_path.read_text())
            assert comparisons(report) == count and report['mismatches'] == 0
            assert sha(output) == sha(gold), (module, tag, 'Original corpus changed')
            print(f'{tag} {module}: {count} pass', flush=True)


if __name__ == '__main__':
    main()
