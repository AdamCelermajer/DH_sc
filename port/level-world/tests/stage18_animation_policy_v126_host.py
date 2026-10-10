"""Focused source Stage18 policy/PreSpawn prefix regression; host only."""
import argparse, hashlib, json, subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
SOURCE = [
    'port/level-world/tests/stage18_animation_policy_v126.cpp',
    'port/game-data/animation_scheduler.cpp',
    'port/game-data/animation_tables.cpp',
    'port/game-data/animation_selection.cpp',
    'port/game-data/data.cpp',
    'port/level-world/character_world_npc_state_owner_v1.cpp',
    'port/level-world/character_state_owner.cpp',
    'port/level-world/character_state_owner_behavior.cpp',
    'port/level-world/character_state_owner_extensions.cpp',
    'port/level-world/character_state.cpp',
    'port/level-world/character_pre_spawn.cpp',
    'port/level-world/character_state_empty.cpp',
    'port/level-world/character_idle_events.cpp',
]
DEPENDENCIES = [
    'port/level-world/character_animation_selection_debug_v126.hpp',
    'port/level-world/character_design_services.cpp',
    'port/game-data/animation_scheduler.hpp',
    'port/level-world/actor_blended_playback.cpp',
    'port/level-world/actor_blended_playback.hpp',
    'port/android-native/app/src/main/cpp/source_campaign_character_fsm_v101.cpp',
    'port/level-world/tests/stage18_animation_policy_v126_host.py',
]

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def posix(path):
    return '/mnt/' + path.drive[0].lower() + str(path.resolve())[2:].replace('\\', '/')

def run(command):
    result = subprocess.run(command, capture_output=True, text=True, timeout=180)
    if result.returncode:
        raise RuntimeError(json.dumps(dict(command=command, code=result.returncode,
                                          stdout=result.stdout, stderr=result.stderr)))
    return result

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--output', type=Path, default=ROOT/'port/level-world/reports/stage18-animation-policy-v126.json')
    args = parser.parse_args()
    scratch = ROOT/'.local-inputs/stage18-policy-v126'
    scratch.mkdir(parents=True, exist_ok=True)
    executable = scratch/'stage18-animation-policy'
    snapshot = {path: sha(ROOT/path) for path in SOURCE + DEPENDENCIES}
    flags = ['-std=c++17', '-O2', '-g', '-fno-omit-frame-pointer',
             '-fsanitize=address,undefined', '-fno-fast-math', '-ffp-contract=off',
             '-ffunction-sections', '-fdata-sections', '-Wl,--gc-sections',
             '-Wall', '-Wextra', '-Wno-misleading-indentation', '-Wno-missing-field-initializers']
    command = ['wsl', '-e', 'g++', *flags, *[posix(ROOT/path) for path in SOURCE], '-o', posix(executable)]
    compiled = run(command)
    assert not compiled.stderr, compiled.stderr
    data = ROOT/'port/android-native/app/src/main/assets/data'
    result = run(['wsl', '-e', 'env', 'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1',
                  'UBSAN_OPTIONS=halt_on_error=1', posix(executable), posix(data)])
    assert not result.stderr, result.stderr
    receipt = json.loads(result.stdout)
    assert receipt['validation'] == 'PASS'
    assert all(sha(ROOT/path) == value for path, value in snapshot.items()), 'Compiler inputs changed during verification'
    assets = ['animations_pyarray.bin', 'animations_pyarraynames.bin', 'animations_pystructnames.bin',
              'animations_dictionary_pyarray.bin', 'animations_dictionary_pyarraynames.bin']
    report = dict(validation='PASS', host_audit=receipt, source_sha256=snapshot,
                  authored_input_sha256={path: sha(data/path) for path in assets},
                  executable_sha256=sha(executable), compiler_command=command,
                  sanitizers=dict(address=True, undefined=True, leak_detection=True, diagnostics=0),
                  source_order='metadata -> tracing -> event36 -> type2 live policy -> RNG/step -> event38',
                  debug_true_fixture='Already source-loaded actual Debug map; no save-file decoder claim',
                  full_APK=False, device_launch=False)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(dict(validation='PASS', output=str(args.output), host_audit=receipt)))

if __name__ == '__main__':
    main()
