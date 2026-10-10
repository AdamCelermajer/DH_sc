"""Run exact shipped Loading bytecode against current loading bodies and host GameSWF."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess
import zlib

ROOT = Path(__file__).resolve().parents[3]

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-directory', type=Path, required=True)
    parser.add_argument('--native-host-build', type=Path, required=True)
    args = parser.parse_args()
    build = args.build_directory.resolve()
    assert build.is_relative_to(ROOT)
    build.mkdir(parents=True, exist_ok=True)
    host = args.native_host_build.resolve()
    ui_lib = host / 'native/level-world/engine-ui/libdh2_engine_ui.so'
    assert ui_lib.is_file()
    shared = ROOT / 'port/android-native/app/src/main/assets/original-cache/data/menus/dqshared_droid.swf'
    raw = shared.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == 'f0f9f119f2fe474d21c722e361ef9907ef2b885654705073e0cb956432f46263'
    decoded = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b'CWS' else raw
    actions = decoded[43984:43984 + 1633]
    assert hashlib.sha256(actions).hexdigest() == '0911da4e726ed744b5b6d2fd1b5b836944c266fab82d6f9a0f563456937c4f3b'
    (build / 'loading-actions.bin').write_bytes(actions)
    # One empty480x320 host root; shipped Loading functions are installed on
    # controlled timeline children. No replacement ActionScript is authored.
    bits = '01111' + ''.join(f'{value:015b}' for value in (0, 9600, 0, 6400))
    bits += '0' * ((-len(bits)) % 8)
    rect = bytes(int(bits[i:i + 8], 2) for i in range(0, len(bits), 8))
    payload = rect + bytes((0, 30, 1, 0)) + struct.pack('<HH', 1 << 6, 0)
    (build / 'blank.swf').write_bytes(b'FWS\x08' + struct.pack('<I', len(payload) + 8) + payload)
    sources = [
        'port/level-loader/tests/terminal_loading_offline_v135.cpp',
        'port/engine-ui/loading_menu_v1.cpp',
        'port/engine-ui/swf_loading_menu_v1.cpp',
        'port/level-loader/menu_end_loading_source_v114.cpp',
        'port/level-loader/canonical_level_loading_v26.cpp',
        'port/level-loader/canonical_level_context_v1.cpp',
        'port/level-loader/native_gslevel_runtime_v27.cpp',
        'port/level-loader/level_constructor_v3.cpp',
        'port/level-loader/lifecycle_v36.cpp',
        'port/level-world/player_controller_attachment_v70.cpp',
        'port/level-world/event_manager_owner_v12.cpp',
        'port/engine-ui/menu_stack_owner_v1.cpp',
        'port/engine-ui/menu_stack_v1.cpp',
    ]
    includes = ['port/engine-ui/overlays/edit-text-v1', 'port/engine-ui', 'port/engine-ui/vendor/gameswf1714', 'port/level-loader',
                'port/level-world', 'port/game-data', 'port/engine-math', 'port/engine-resources',
                'port/scene-materials', 'port/asset-payloads', 'port/script-runtime',
                'port/engine-animation', 'port/engine-audio/integration-v42/application']
    text = '''cmake_minimum_required(VERSION 3.22)
project(terminal_loading_v135 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-fast-math -ffp-contract=off -fno-sanitize=vptr -ffunction-sections -fdata-sections)
add_link_options(-fsanitize=address,undefined -Wl,--gc-sections)
add_executable(audit
'''
    text += ''.join(f' "{ROOT / path}"\n' for path in sources) + ')\n'
    text += 'target_include_directories(audit PRIVATE\n' + ''.join(f' "{ROOT / path}"\n' for path in includes) + ')\n'
    text += 'target_compile_definitions(audit PRIVATE TU_CONFIG_LINK_TO_JPEGLIB=0 TU_CONFIG_LINK_TO_LIBPNG=0 TU_CONFIG_LINK_TO_FREETYPE=0 TU_CONFIG_LINK_TO_THREAD=0)\n'
    text += f'target_link_libraries(audit PRIVATE "{ui_lib}" z ${{CMAKE_DL_LIBS}})\n'
    (build / 'CMakeLists.txt').write_text(text)
    subprocess.run(['cmake', '-S', str(build), '-B', str(build / 'build')], check=True)
    subprocess.run(['cmake', '--build', str(build / 'build'), '-j', '2', '--target', 'audit'], check=True)
    env = dict(os.environ, ASAN_OPTIONS='detect_leaks=1:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1')
    command = [str(build / 'build/audit'), str(build / 'blank.swf'), str(build / 'loading-actions.bin')]
    result = subprocess.run(command, text=True, capture_output=True, env=env)
    changed = sources + ['port/engine-ui/loading_menu_v1.hpp', 'port/level-loader/canonical_level_loading_v26.hpp',
                         'port/level-loader/native_gslevel_runtime_v27.hpp', 'port/level-world/source_online_loading_menu_v135.hpp',
                         'port/android-native/app/src/main/cpp/front_ui_session_v87.cpp',
                         'port/android-native/app/src/main/cpp/front_ui_session_v87.hpp',
                         'port/android-native/app/src/main/cpp/native_gslevel_menu_v27.inc']
    receipt = {'status': 'PASS' if result.returncode == 0 else 'FAIL', 'command': command,
               'exit_code': result.returncode, 'stdout': result.stdout, 'stderr': result.stderr,
               'source_sha256': {path: hashlib.sha256((ROOT / path).read_bytes()).hexdigest() for path in changed},
               'shipped_loading_block_sha256': hashlib.sha256(actions).hexdigest(),
               'host_gameswf_backend': str(ui_lib), 'host_gameswf_backend_sha256': hashlib.sha256(ui_lib.read_bytes()).hexdigest(),
               'limits': 'Current loading/controller/menu bodies instrumented with ASan/UBSan. Existing host GameSWF backend is uninstrumented. Script/player/menu-effect leaves are explicit fixtures. No full campaign or device acceptance.'}
    (build / 'receipt.json').write_text(json.dumps(receipt, indent=2) + '\n')
    print(result.stdout, end='')
    if result.stderr:
        print(result.stderr, end='')
    result.check_returncode()
    print('Shipped Loading action block SHA256: ' + hashlib.sha256(actions).hexdigest())

if __name__ == '__main__':
    main()
