"""Audit the actual host Lua DSO's deferred ownership policy and regressions.

Source stack/order observations come from the separate original instruction
discovery. This audit runs real native Lua; it does not prove full AIS/manager
registration, Android packaged instructions, or physical device behavior.
"""
import argparse
import hashlib
import json
import re
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(*arguments):
    result = subprocess.run(['wsl.exe', '--', *arguments], capture_output=True,
                            text=True, timeout=90)
    assert result.returncode == 0, (arguments, result.returncode, result.stdout, result.stderr)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', default='/home/adampalace/dh2-world-build')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    assert not args.output.exists(), 'Refusing to replace an earlier audit'
    proof = REPO/'port/level-world/reports/character-script-ownership-discovery.json'
    original = json.loads(proof.read_text())
    assert original['validation'] == 'PASS'
    common = REPO/'port/level-world/reference/character-script-update/lua-inputs/ai-commons.luac'
    assert sha(common) == '20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c'
    inputs = [p for p in ROOT.rglob('*') if p.is_file() and p.suffix in ('.c', '.h', '.cpp')]
    inputs += [ROOT/'CMakeLists.txt', Path(__file__), proof, common]
    sources = {p.relative_to(REPO).as_posix(): sha(p) for p in inputs}
    configure_command = ['cmake', '-S', '/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/level-world',
                         '-B', args.build, '-DDH2_SCRIPT_SANITIZERS=ON',
                         '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON']
    configure = run(*configure_command)
    build_command = ['cmake', '--build', args.build, '--target',
                     'script_vm_ownership_audit', 'lua514_audit',
                     'script_game_bindings_audit', '-j', '4']
    build = run(*build_command)
    executables = [args.build+'/script-runtime/'+name for name in (
        'script_vm_ownership_audit', 'lua514_audit', 'script_game_bindings_audit')]
    binaries = set(executables)
    linked = {}
    for executable in executables:
        result = run('ldd', executable)
        assert 'not found' not in result.stdout
        linked[executable] = result.stdout
        for line in result.stdout.splitlines():
            match = re.search(r'=>\s+(/\S+)|^\s*(/\S+)', line)
            if match:
                binaries.add(match.group(1) or match.group(2))
    assert args.build+'/script-runtime/libdh2_script_runtime.so' in binaries
    assert any('libasan.so' in path for path in binaries)
    assert any('libubsan.so' in path for path in binaries)

    def hashes():
        return {line.split(maxsplit=1)[1].strip(): line.split()[0]
                for line in run('sha256sum', *sorted(binaries)).stdout.splitlines()}

    before = hashes()
    prefix = ['env', 'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1', 'UBSAN_OPTIONS=halt_on_error=1']
    common_wsl = '/mnt/c/Users/adamc/Desktop/workspace/DH_sc/'+common.relative_to(REPO).as_posix()
    commands = [prefix+[executables[0]],
                prefix+[executables[1], common_wsl,
                        '/mnt/c/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/deferred-vm-ai-commons-native.luac'],
                prefix+[executables[2], common_wsl]]
    observations = []
    for command in commands:
        result = run(*command)
        assert not result.stderr, result.stderr
        audit = json.loads(result.stdout)
        assert audit['validation'] == 'PASS'
        observations.append(audit)
    ownership = observations[0]
    assert ownership['source_library_stack_steps'] == [2, 3, 4, 5]
    assert ownership['repeated_stack_top'] == 10
    assert ownership['private_vms_verified'] and ownership['eager_policy_preserved']
    assert ownership['allocation_failure_protected']
    assert before == hashes()
    assert sources == {p.relative_to(REPO).as_posix(): sha(p) for p in inputs}
    # Bind actual C and C++ instrumentation for the DSO and new audit.
    compile_path = args.build+'/compile_commands.json'
    compile_db = json.loads(run('cat', compile_path).stdout)
    compiler_rows = [row for row in compile_db if row['file'].endswith((
        '/script_runtime.c', '/lua/lvm.c', '/tests/script_vm_ownership.cpp'))]
    assert len(compiler_rows) == 3
    assert all('-fsanitize=address,undefined' in row['command'] for row in compiler_rows)
    report = {'validation': 'PASS', 'scope': __doc__, 'source_bindings': sources,
              'original_ownership_discovery': {'path': str(proof), 'sha256': sha(proof)},
              'binary_bindings': before, 'ldd': linked, 'compiler_rows': compiler_rows,
              'build_command': build_command, 'build_output': build.stdout,
              'configure_command': configure_command, 'configure_output': configure.stdout,
              'commands': commands, 'observations': observations,
              'sanitizer_findings': 0, 'full_ais_or_manager_owned': False,
              'packaged_android_verified': False, 'physical_arm64_verified': False}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps({'validation': 'PASS', 'observations': observations, 'sanitizer_findings': 0}))


if __name__ == '__main__':
    main()
