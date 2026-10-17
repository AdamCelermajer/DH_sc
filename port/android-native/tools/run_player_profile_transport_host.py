"""Compose the native per-Save transport with actual selected world/data libraries."""
import argparse, hashlib, json, os, shutil, subprocess, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'port/level-world/tests'))
from run_player_skill_cleanup_session_v1_host import selected_entries, actual_dependencies

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['compiler', 'cache', 'output', 'report']:
        parser.add_argument('--' + name, required=True, type=Path)
    args = parser.parse_args()
    cxx = args.compiler.resolve()
    cc = cxx.with_name(cxx.name.replace('g++', 'gcc'))
    cmake, ninja = shutil.which('cmake'), shutil.which('ninja')
    assert cmake and ninja
    out = args.output.resolve()
    wrapper, build = out / 'wrapper', out / 'build'
    wrapper.mkdir(parents=True, exist_ok=True)
    world = ROOT / 'port/level-world'
    source = ROOT / 'port/android-native/app/src/main/cpp/native_player_profile.cpp'
    quest_owner = ROOT / 'port/android-native/app/src/main/cpp/native_quest_owner.cpp'
    quest_cursor = ROOT / 'port/android-native/app/src/main/cpp/native_quest_cursor.cpp'
    test = ROOT / 'port/android-native/tests/player_profile_transport.cpp'
    body = f'''cmake_minimum_required(VERSION 3.22)
project(profile_transport_selected LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{world.as_posix()}" selected-world)
add_executable(profile_transport_audit "{test.as_posix()}" "{source.as_posix()}"
    "{quest_owner.as_posix()}" "{quest_cursor.as_posix()}")
target_compile_features(profile_transport_audit PRIVATE cxx_std_17)
target_compile_options(profile_transport_audit PRIVATE -Wall -Wextra -Werror)
target_link_libraries(profile_transport_audit PRIVATE dh2_level_world dh2_game_data)
'''
    (wrapper / 'CMakeLists.txt').write_text(body, encoding='utf-8')
    env = os.environ.copy()
    env['PATH'] = str(cxx.parent) + os.pathsep + env.get('PATH', '')
    def run(command, custom_env=None):
        result = subprocess.run(list(map(str, command)), cwd=ROOT, env=custom_env or env,
                                text=True, capture_output=True)
        if result.returncode:
            raise RuntimeError(f'{command}\n{result.stdout}\n{result.stderr}')
        return result.stdout
    logs = [run([cmake, '-S', wrapper, '-B', build, '-G', 'Ninja',
        f'-DCMAKE_MAKE_PROGRAM={ninja}', f'-DCMAKE_CXX_COMPILER={cxx}',
        f'-DCMAKE_C_COMPILER={cc}', '-DCMAKE_BUILD_TYPE=Release',
        '-DCMAKE_CXX_FLAGS_RELEASE=-O1', '-DCMAKE_C_FLAGS_RELEASE=-O1'])]
    print('Discovering selected transport/world/data inputs', flush=True)
    logs.append(run([cmake, '--build', build, '--target', 'profile_transport_audit', '--parallel', '2']))
    commands = run([ninja, '-C', build, '-t', 'commands', 'profile_transport_audit'])
    rows = selected_entries(build, commands)
    reached = [Path(row['file']).resolve() for row in rows]
    required = [source, quest_owner, quest_cursor, world/'character_saved_class_v1.cpp', world/'character_template_random.cpp',
                ROOT/'port/game-data/player_savegame_v1.cpp', ROOT/'port/game-data/player_save_load_owner_v1.cpp',
                ROOT/'port/game-data/player_profile_index_v1.cpp', ROOT/'port/game-data/player_save_level_states_v1.cpp',
                ROOT/'port/game-data/player_saved_level_states_v1.cpp', ROOT/'port/game-data/player_saved_fast_travel_v1.cpp',
                ROOT/'port/game-data/world_map_tables.cpp', ROOT/'port/random/random.c']
    assert all(reached.count(path.resolve()) == 1 for path in required)
    inputs = actual_dependencies(build, ninja, rows) | {Path(__file__).resolve(), test}
    before = {path.relative_to(ROOT).as_posix(): sha(path) for path in sorted(inputs)}
    cache = args.cache.resolve()
    cache_hashes = {name: sha(cache/name) for name in ['character_properties_pyarray.bin',
        'character_properties_pyarraynames.bin', 'character_properties_pystructnames.bin',
        'levels_pyarray.bin','levels_pyarraynames.bin','levels_pystructnames.bin',
        'worldmap_pyarray.bin','worldmap_pyarraynames.bin','worldmap_pystructnames.bin']}
    print(f'Clean rebuild and composition from {len(before)} selected project inputs', flush=True)
    logs.append(run([cmake, '--build', build, '--target', 'profile_transport_audit', '--clean-first', '--parallel', '2']))
    dsos = sorted(build.rglob('*.dll'))
    live_env = env.copy()
    live_env['PATH'] = os.pathsep.join([*(str(path.parent) for path in dsos), env['PATH']])
    executable = build/'profile_transport_audit.exe'
    # A new named fixture directory per run avoids inheriting any prior file.
    import uuid
    fixture = out/('fixture-' + uuid.uuid4().hex)
    host = json.loads(run([executable, cache, fixture], live_env))
    assert host['validation'] == 'PASS'
    # The production assembled writer created slot 9. Reopen it in a distinct
    # OS process with fresh Save/Transport/index owners to separate durable
    # file persistence from same-process serialization receipts.
    cold_process = json.loads(run([executable, '--cold-load', cache,
        fixture/'gameplay-save-assembled', '9'], live_env))
    assert cold_process['validation'] == 'PASS' and cold_process['fresh_process_profile_open']
    host['cold_process_reopen'] = cold_process
    after_commands = run([ninja, '-C', build, '-t', 'commands', 'profile_transport_audit'])
    after_rows = selected_entries(build, after_commands)
    after_inputs = actual_dependencies(build, ninja, after_rows) | {Path(__file__).resolve(), test}
    after = {path.relative_to(ROOT).as_posix(): sha(path) for path in sorted(after_inputs)}
    changed = {name: {'before': before.get(name), 'after': after.get(name)}
        for name in before.keys() | after.keys() if before.get(name) != after.get(name)}
    (out/'raw-composition-results.json').write_text(json.dumps({'host': host,
        'source_changes': changed, 'commands_stable': commands == after_commands}, indent=2)+'\n', encoding='utf-8')
    assert before == after and commands == after_commands, 'Selected source changed during verification'
    assert cache_hashes == {name: sha(cache/name) for name in cache_hashes}, 'Authored cache changed'
    imports = run([cxx.with_name('objdump.exe'), '-p', executable])
    assert 'DLL Name: libdh2_level_world.dll' in imports and 'DLL Name: libdh2_game_data.dll' in imports
    report = {'validation': 'PASS', 'host': host, 'source_before_after_equal': True,
        'source_sha256': before, 'cache_sha256': cache_hashes,
        'source_tu_counts': {path.name: reached.count(path.resolve()) for path in required},
        'compiler_objects': {Path(row['output']).resolve().relative_to(build).as_posix():
            Path(row['file']).resolve().relative_to(ROOT).as_posix() for row in rows},
        'selected_commands': commands, 'wrapper_cmake': body, 'build_stdout': logs,
        'binary_sha256': {path.relative_to(out).as_posix(): sha(path) for path in [executable, *dsos]},
        'scope': 'Native I/O adapter + selected source Save index/mask1/class/PROP and InitLevelStates/LVLS/FTVL composition over actual 51-level/13-location table owners, 192 defaults and the same six arrays/bitset words. Synthetic mask4 uses a declared offline-global fixture and absent inventory/quest/skill payloads. Header-gated primary/backup recovery, malformed-index fail-closed behavior, and an OS-process cold reopen of a profile written by the complete same-Save mask1+mask4 writer assembly. Does not establish power-loss-at-every-instruction, full native InitPost/mask2/mask4 completion or live gameplay.'}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({'validation': 'PASS', 'host': host, 'project_inputs': len(before)}))

if __name__ == '__main__':
    main()
