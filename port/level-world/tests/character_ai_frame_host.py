"""Replay outer CharAI gold through a real DSO and the production world kernels."""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def linux(path):
    return '/mnt/c/' + str(path.resolve()).replace('\\', '/')[3:]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', default='/home/adampalace/dh2-world-build')
    parser.add_argument('--main-linked', action='store_true', help='Run an already built central character_ai_frame_audit target.')
    parser.add_argument('--output', type=Path, default=ROOT / 'reports/character-ai-frame-host-audit.json')
    args = parser.parse_args()
    scratch = REPO / '.local-inputs/character-ai-frame-discovery'
    scratch.mkdir(parents=True, exist_ok=True)
    commands = []

    def run(*arguments):
        result = subprocess.run(['wsl', '--cd', linux(REPO), *arguments], capture_output=True, text=True, timeout=120)
        command = {'arguments': list(arguments), 'returncode': result.returncode, 'stdout': result.stdout, 'stderr': result.stderr}
        commands.append(command)
        assert result.returncode == 0 and not result.stderr.strip(), command
        return result.stdout.strip()

    paths = ['character_ai_frame.hpp', 'character_ai_frame.cpp', 'tests/character_ai_frame.cpp',
             'tests/character_ai_frame_differential.py', 'tests/character_ai_frame_bridge.cpp',
             'tests/character_ai_frame_host.py', 'character_ai_update.hpp', 'character_ai_update.cpp',
             'character_script_update.hpp', 'character_script_update.cpp', 'CMakeLists.txt']
    sources = {str((ROOT / path).relative_to(REPO)).replace('\\', '/'): sha(ROOT / path) for path in paths}
    world = args.build + '/libdh2_level_world.so'
    world_before = run('sha256sum', world).split()[0]
    flags = ['-std=c++17', '-O1', '-g', '-fno-fast-math', '-ffp-contract=off', '-fsanitize=address,undefined',
             '-fno-omit-frame-pointer', '-Wall', '-Wextra', '-Werror']
    library = linux(scratch / 'libcharacter_ai_frame_audit.so')
    executable = linux(scratch / 'host_audit')
    if args.main_linked:
        library = world
        executable = args.build + '/character_ai_frame_audit'
        assert '-fsanitize=address,undefined' in run('cmake', '-LA', '-N', args.build)
        run('ninja', '-C', args.build, '-t', 'commands', 'character_ai_frame_audit')
    else:
        run('g++', *flags, '-shared', '-fPIC', linux(ROOT / 'character_ai_frame.cpp'), '-o', library)
        run('g++', *flags, linux(ROOT / 'tests/character_ai_frame.cpp'), '-L' + linux(scratch), '-L' + args.build,
            '-lcharacter_ai_frame_audit', '-ldh2_level_world', '-ldl', '-Wl,-rpath,' + linux(scratch) + ':' + args.build,
            '-o', executable)
    dependencies = run('ldd', executable)
    assert library in dependencies and world in dependencies and 'libasan.so' in dependencies and 'libubsan.so' in dependencies
    gold = ROOT / 'reference/character-ai-frame/frame-fixtures.bin'
    original_report = ROOT / 'reports/character-ai-frame-arm64-differential.json'
    evidence = json.loads(original_report.read_text())
    assert evidence['validation'] == 'PASS' and sha(gold) == evidence['corpus_sha256']
    checks = json.loads(run('env', 'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1', 'UBSAN_OPTIONS=halt_on_error=1', executable, linux(gold)))
    assert checks['validation'] == 'PASS' and checks['comparisons'] == evidence['comparisons']
    assert checks['ordered_service_requests'] == evidence['ordered_service_requests']
    assert checks['frame_library'] == library and checks['on_update_library'] == checks['selected_library'] == world
    binary_paths = {executable, library, world}
    binary_paths.update(value for value in dependencies.split() if value.startswith(args.build + '/') and value.endswith('.so'))
    binaries = {path: run('sha256sum', path).split()[0] for path in sorted(binary_paths)}
    assert binaries[world] == world_before, 'World DSO changed during audit'
    assert all(sha(REPO / path) == value for path, value in sources.items()), 'Source changed during audit'
    assert all(sha(REPO / path) == value for path, value in evidence['source_sha256'].items()), 'Original differential source is stale'
    references = ['original-functions.json', 'reference/original-functions.asm', 'debug-producer.json', 'debug_producer.py',
                  'producers/original-functions.json', 'producers/reference/original-functions.asm']
    report = {'validation': 'PASS', 'main_world_library_executed': args.main_linked,
              'on_update_and_selected_world_library_executed': True, 'host_audit': checks,
              'source_sha256': sources, 'binary_sha256': binaries, 'linked_dependencies': dependencies,
              'reference_sha256': {path: sha(ROOT / 'reference/character-ai-frame' / path) for path in references},
              'original_sha256': evidence['original_sha256'], 'corpus_sha256': sha(gold),
              'original_instruction_report_sha256': sha(original_report),
              'composed_instruction_evidence': evidence['composed_instruction_evidence'],
              'sanitizers': ['AddressSanitizer', 'UndefinedBehaviorSanitizer'], 'sanitizer_findings': 0,
              'sources_unchanged_through_replay': True, 'commands': commands,
              'scope': 'All original outer-dispatcher gold replayed through the genuine frame DSO with actual full OnUpdate and selected AISDefault kernels from libdh2_level_world.so. Target/master/aggro, zoning, timer and Stop are explicit callbacks. No Lua, original complete-frame, renderer or APK execution claim.'}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({'validation': 'PASS', 'report': str(args.output), 'checks': checks}))


if __name__ == '__main__':
    main()
