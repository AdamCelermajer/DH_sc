"""Isolated spatial DSO replay against the genuine, frozen central Lua runtime.

No CMake or central DSO rebuild. Object-manager/cast and userdata identities are
explicit caller services, rather than a reconstructed complete world backend.
"""
import argparse, hashlib, json, subprocess
from pathlib import Path
WORLD = Path(__file__).resolve().parents[1]
ROOT = WORLD.parents[1]
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p): return '/mnt/' + p.drive[0].lower() + p.as_posix()[2:]
def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--runtime-build', default='/home/adampalace/dh2-world-build/script-runtime')
    a = ap.parse_args()
    scratch = ROOT / '.local-inputs/character-spatial-bindings'
    scratch.mkdir(parents=True, exist_ok=True)
    inputs = [WORLD / p for p in [
        'character_spatial_bindings.hpp', 'character_spatial_bindings.cpp',
        'tests/character_spatial_bindings.cpp', 'tests/character_spatial_bindings_host.py',
        'tests/character_spatial_bindings_differential.py',
        'reference/character-spatial-bindings/spatial-original-gold.bin',
        'reference/character-spatial-bindings/spatial-original-gold.json',
        'reference/character-spatial-bindings/original-functions.json',
        'reference/character-spatial-bindings/reference/original-functions.asm',
        'reference/character-spatial-bindings/value-helpers/original-functions.json',
        'reference/character-spatial-bindings/value-helpers/reference/original-functions.asm',
        'reports/character-spatial-bindings-arm64-differential.json']]
    inputs += [ROOT / 'port/script-runtime' / p for p in ['script_runtime.h', 'script_runtime.c']]
    hashes = {p.relative_to(ROOT).as_posix(): sha(p) for p in inputs}
    commands = []
    def run(*args):
        r = subprocess.run(['wsl', '--cd', linux(ROOT), *args], capture_output=True, text=True, timeout=120)
        commands.append(dict(arguments=list(args), returncode=r.returncode, stdout=r.stdout, stderr=r.stderr))
        assert r.returncode == 0 and not r.stderr.strip(), commands[-1]
        return r.stdout.strip()
    runtime = a.runtime_build + '/libdh2_script_runtime.so'
    before = run('sha256sum', runtime).split()[0]
    lib = linux(scratch / 'libcharacter_spatial_bindings.so')
    exe = linux(scratch / 'spatial_audit')
    flags = ['-O1', '-g', '-std=c++17', '-fno-fast-math', '-ffp-contract=off',
             '-fsanitize=address,undefined', '-fno-omit-frame-pointer',
             '-Wall', '-Wextra', '-Werror']
    links = ['-L' + a.runtime_build, '-ldh2_script_runtime', '-Wl,-rpath,' + a.runtime_build]
    run('g++', '-shared', '-fPIC', *flags, linux(WORLD / 'character_spatial_bindings.cpp'), *links, '-o', lib)
    run('g++', *flags, '-Wno-misleading-indentation', linux(WORLD / 'tests/character_spatial_bindings.cpp'),
        '-L' + linux(scratch), '-lcharacter_spatial_bindings', '-Wl,-rpath,' + linux(scratch),
        *links, '-ldl', '-o', exe)
    audit = json.loads(run('env', 'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1',
                          'UBSAN_OPTIONS=halt_on_error=1', exe,
                          linux(WORLD / 'reference/character-spatial-bindings/spatial-original-gold.bin')))
    assert audit['validation'] == 'PASS' and audit['original_gold_cases'] == 2746
    assert audit['ordered_logical_lookup_cast_services'] == 3861
    assert audit['kernel_library'] == lib and audit['runtime_library'] == runtime
    deps = run('ldd', exe)
    assert lib in deps and runtime in deps and 'libasan.so' in deps and 'libubsan.so' in deps
    binaries = {p: run('sha256sum', p).split()[0] for p in [lib, exe, runtime]}
    assert binaries[runtime] == before
    assert hashes == {p.relative_to(ROOT).as_posix(): sha(p) for p in inputs}
    report = dict(validation='PASS', host_audit=audit, source_sha256=hashes,
                  binary_sha256=binaries, linked_dependencies=deps, commands=commands,
                  sanitizers=['AddressSanitizer', 'UndefinedBehaviorSanitizer', 'LeakSanitizer'],
                  sanitizer_findings=0,
                  original_sha256='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80',
                  scope=__doc__, actual_central_VM_DSO=True, isolated_spatial_DSO=True,
                  source_wrapper_original_instruction_proof='port/level-world/reports/character-spatial-bindings-arm64-differential.json',
                  whole_VM_differential=False, full_object_manager_or_world=False,
                  source_SetPosition=False, packaged_APK=False,
                  NaN_policy='Copied coordinates exact; arithmetic NaNs class-equivalent; finite/infinity/signed-zero words exact.')
    (WORLD / 'reports/character-spatial-bindings-host-audit.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(audit))
if __name__ == '__main__': main()
