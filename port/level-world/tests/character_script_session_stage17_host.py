"""Rebuild reached Stage17 session/owner callbacks in an isolated host audit.

World/data DSOs are recorded borrowed dependencies, not represented as
current-source builds. This runner leaves those DSOs and the historical compiler
proof unchanged. It executes real cached Lua with explicit audio, motion and
animation provider fixtures using a current isolated VM build; no APK or device
claim follows from this audit.
"""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
SOURCES = [
    'port/level-world/tests/character_script_session.cpp',
    'port/level-world/character_script_session.cpp',
    'port/level-world/character_game_design.cpp',
    'port/android-native/app/src/main/cpp/source_process_arrays_v101.cpp',
    'port/level-world/character_script_owner.cpp',
    'port/level-world/character_script_states.cpp',
    'port/level-world/character_script_lifecycle.cpp',
    'port/level-world/character_script_virtual.cpp',
    'port/level-world/character_design_services.cpp',
    'port/level-world/character_register_summon_v81.cpp',
]
REFERENCES = [
    'port/level-world/character_game_design.hpp',
    'port/android-native/app/src/main/cpp/source_process_arrays_v101.hpp',
    'port/android-native/app/src/main/cpp/source_process_array_registration_v101.hpp',
    'port/android-native/app/src/main/cpp/source_process_pydata_files_v101.hpp',
    'port/android-native/app/src/main/cpp/source_process_compiled_members_v121.hpp',
]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def linux(path):
    path = path.resolve()
    return '/mnt/' + path.drive[0].lower() + path.as_posix()[2:]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', default='/home/adampalace/dh2-world-build')
    parser.add_argument('--output', type=Path, default=ROOT/'.local-inputs/live-objective/swamp-stage17-18/stage17-host.json')
    args = parser.parse_args()
    scratch = args.output.parent/'host-build'
    scratch.mkdir(parents=True, exist_ok=True)
    executable = scratch/'stage17-character-session'
    dependencies = [args.build+'/libdh2_level_world.so', args.build+'/game-data/libdh2_game_data.so']
    runtime_sources = sorted(p for p in (ROOT/'port/script-runtime').rglob('*') if p.is_file() and (p.suffix in ('.c', '.cpp', '.h', '.hpp') or p.name == 'CMakeLists.txt') and 'tests' not in p.parts)
    source_hashes = {name: sha(ROOT/name) for name in SOURCES}
    source_hashes.update({name: sha(ROOT/name) for name in REFERENCES})
    source_hashes.update({p.relative_to(ROOT).as_posix(): sha(p) for p in runtime_sources})
    inputs = [
        ROOT/'port/level-world/reference/character-game-design/real-cache-inputs.bin',
        ROOT/'.local-inputs/character-script-owner-discovery/ai-commons-source.luac',
        ROOT/'.local-inputs/character-script-owner-extension/monster.luac',
        ROOT/'port/android-native/app/src/main/assets/worlds/crypt01.dact',
        ROOT/'.local-inputs/character-script-assets-v1/bootstrap-cache/data/scripts/ai/swampking_core.luac',
        ROOT/'port/android-native/app/src/main/assets/data/animations_dictionary_pyarray.bin',
        ROOT/'port/android-native/app/src/main/assets/data/animations_dictionary_pyarraynames.bin',
    ]
    input_hashes = {p.relative_to(ROOT).as_posix(): sha(p) for p in inputs}
    commands = []

    def run(argv, require_success=True):
        result = subprocess.run(['wsl', '-e', *argv], capture_output=True, text=True, timeout=180)
        commands.append(dict(arguments=argv, returncode=result.returncode, stdout=result.stdout, stderr=result.stderr))
        if require_success and (result.returncode or result.stderr.strip()):
            raise RuntimeError(json.dumps(commands[-1]))
        return result

    before = {p: run(['sha256sum', p]).stdout.split()[0] for p in dependencies}
    runtime_build = scratch/'runtime'
    run(['cmake', '-S', linux(ROOT/'port/script-runtime'), '-B', linux(runtime_build), '-DDH2_SCRIPT_SANITIZERS=ON', '-DCMAKE_BUILD_TYPE=RelWithDebInfo'])
    run(['cmake', '--build', linux(runtime_build), '--target', 'dh2_script_runtime', '-j2'])
    flags = ['-std=c++17', '-O1', '-g', '-fno-fast-math', '-ffp-contract=off', '-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-Wall', '-Wextra', '-Werror', '-Wno-misleading-indentation', '-ffunction-sections', '-fdata-sections', '-Wl,--gc-sections']
    links = ['-L'+args.build, '-ldh2_level_world', '-L'+args.build+'/game-data', '-ldh2_game_data', '-L'+linux(runtime_build), '-ldh2_script_runtime', '-ldl', '-Wl,-rpath,'+args.build, '-Wl,-rpath,'+args.build+'/game-data', '-Wl,-rpath,'+linux(runtime_build)]
    run(['g++', *flags, *[linux(ROOT/name) for name in SOURCES], *links, '-o', linux(executable)])
    result = run(['env', 'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1', 'UBSAN_OPTIONS=halt_on_error=1', linux(executable), *map(linux, inputs)], require_success=False)
    audit = json.loads(result.stdout) if not result.returncode else None
    assert result.returncode == 0 and not result.stderr.strip(), commands[-1]
    assert audit['validation'] == 'PASS' and audit['actual_swampking_core_oninit'] is True
    assert audit['actual_swampking_core_mark_as_flying_calls'] == 1 and audit['actual_swampking_core_register_anim_calls'] == 7
    assert audit['actual_swampking_core_virtual16'] is True and audit['stage17_required_failure_prefixes'] == 3
    assert audit['missing_AnimDict_failure_reproduced'] is True
    assert audit['process_AnimDict_owner_resolved_swampking_idle'] is True and audit['process_AnimDict_swampking_idle_id'] == 1328
    after = {p: run(['sha256sum', p]).stdout.split()[0] for p in dependencies}
    assert before == after, 'Borrowed dependency changed during focused audit'
    assert all(sha(ROOT/name) == value for name, value in source_hashes.items()), 'Source changed during focused audit'
    assert input_hashes == {p.relative_to(ROOT).as_posix(): sha(p) for p in inputs}, 'Cached source input changed during focused audit'
    args.output.write_text(json.dumps(dict(validation='PASS', scope=__doc__, host_audit=audit, source_sha256=source_hashes, input_sha256=input_hashes, borrowed_dependency_sha256=before, current_runtime_sha256=sha(runtime_build/'libdh2_script_runtime.so'), executable_sha256=sha(executable), commands=commands, current_session_owner_source_rebuilt=True, current_character_game_design_source_rebuilt=True, current_process_arrays_source_rebuilt=True, current_runtime_source_rebuilt=True, full_current_world_source_rebuilt=False, source_virtual16_called=True, full_APK=False, device_launch=False), indent=2)+'\n')
    print(json.dumps(dict(validation='PASS', output=str(args.output), host_audit=audit, diagnostic=result.stderr.strip()[:500])))


if __name__ == '__main__':
    main()
