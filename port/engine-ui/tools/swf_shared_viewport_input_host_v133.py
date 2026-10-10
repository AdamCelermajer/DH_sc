"""Focused retained input regression, with the actual source GameSWF core."""
import argparse
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[3]
UI = ROOT / 'port/engine-ui'

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-directory', type=Path, required=True)
    args = parser.parse_args()
    build = args.build_directory.resolve()
    assert build.is_relative_to(ROOT)
    build.mkdir(parents=True, exist_ok=True)
    sources = ['swf_cursor_input', 'swf_input_geometry', 'swf_input_policy',
               'swf_input_history', 'swf_input_connection', 'swf_controller_storage_v91',
               'swf_event_dispatch', 'swf_event_core', 'swf_viewport_connection',
               'viewport', 'swf_frame_schedule', 'swf_frame_connection', 'swf_drag_values']
    text = '''cmake_minimum_required(VERSION 3.22)
project(shared_viewport_input LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-fast-math -ffp-contract=off)
add_link_options(-fsanitize=address,undefined)
'''
    for recipe in ['gameswf_sources.cmake', 'gameswf_input_overlay_v1.cmake', 'gameswf_frame_overlay_v1.cmake']:
        text += f'include("{UI / recipe}")\n'
    text += 'add_library(input_core SHARED ${DH2_GAMESWF_SOURCES}\n'
    text += ''.join(f' "{UI / (source + ".cpp")}"\n' for source in sources) + ')\n'
    text += f'target_include_directories(input_core PUBLIC "{UI}" "${{DH2_GAMESWF_ROOT}}")\n'
    text += '''target_compile_definitions(input_core PUBLIC TU_CONFIG_LINK_TO_JPEGLIB=0 TU_CONFIG_LINK_TO_LIBPNG=0 TU_CONFIG_LINK_TO_FREETYPE=0 TU_CONFIG_LINK_TO_THREAD=0)
target_compile_options(input_core PRIVATE -w -fpermissive -fno-sanitize=vptr)
target_link_libraries(input_core PUBLIC z ${CMAKE_DL_LIBS})
'''
    text += f'add_executable(audit "{UI / "tests/swf_cursor_input.cpp"}")\n'
    text += 'target_link_libraries(audit PRIVATE input_core)\ntarget_compile_options(audit PRIVATE -fno-sanitize=vptr)\n'
    text += f'add_executable(snapshot_repro "{UI / "tests/swf_cursor_input.cpp"}")\n'
    text += 'target_link_libraries(snapshot_repro PRIVATE input_core)\ntarget_compile_options(snapshot_repro PRIVATE -fno-sanitize=vptr)\ntarget_compile_definitions(snapshot_repro PRIVATE DH2_INPUT_VIEWPORT_SNAPSHOT_REPRO=1)\n'
    (build / 'CMakeLists.txt').write_text(text)
    subprocess.run(['cmake', '-S', str(build), '-B', str(build / 'build')], check=True)
    subprocess.run(['cmake', '--build', str(build / 'build'), '-j', '4', '--target', 'audit', 'snapshot_repro'], check=True)
    env = dict(os.environ, ASAN_OPTIONS='detect_leaks=1:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1')
    subprocess.run([str(build / 'build/audit')], check=True, env=env)
    baseline = subprocess.run([str(build / 'build/snapshot_repro')], text=True, capture_output=True, env=env)
    assert baseline.returncode == 1 and 'Wide source button missed after render viewport update' in baseline.stderr, baseline
    assert 'AddressSanitizer' not in baseline.stderr and 'runtime error:' not in baseline.stderr, baseline.stderr
    print('Former connection-time viewport copy: reproduced missed wide source button')

if __name__ == '__main__':
    main()
