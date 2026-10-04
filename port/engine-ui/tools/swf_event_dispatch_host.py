"""Isolated actual-UI-DSO sanitizer mutable event/method proof (run in WSL)."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[3]
MODULE = ROOT/'port/engine-ui'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--ui-library', type=Path, required=True)
    p.add_argument('--build-directory', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    assert not a.report.exists(), 'Preserve existing host proof'
    assert a.build_directory.resolve().is_relative_to(ROOT), 'Scratch must stay in workspace'
    private = a.build_directory/'private'
    private.mkdir(parents=True, exist_ok=True)
    library = private/'libdh2_engine_ui.so'
    if a.ui_library.resolve() != library.resolve():
        before = sha(a.ui_library)
        shutil.copyfile(a.ui_library, library)
        assert before == sha(a.ui_library) == sha(library), 'UI DSO changed during snapshot'
    binary = a.build_directory/'audit'
    command = ['c++', '-std=c++17', '-O1', '-g', '-fsanitize=address,undefined',
               '-fno-omit-frame-pointer', '-fno-fast-math', '-ffp-contract=off']
    command += ['-D'+key+'=0' for key in ('TU_CONFIG_LINK_TO_JPEGLIB','TU_CONFIG_LINK_TO_LIBPNG',
                                       'TU_CONFIG_LINK_TO_FREETYPE','TU_CONFIG_LINK_TO_THREAD')]
    command += ['-I'+str(MODULE), '-isystem', str(MODULE/'vendor/gameswf1714'),
                str(MODULE/'swf_event_dispatch.cpp'), str(MODULE/'swf_event_core.cpp'), str(MODULE/'tests/swf_event_dispatch.cpp'),
                '-L'+str(private), '-ldh2_engine_ui', '-Wl,-rpath,'+str(private.resolve()), '-o', str(binary)]
    compiled = subprocess.run(command, capture_output=True, text=True)
    if compiled.returncode:
        print(compiled.stderr)
        compiled.check_returncode()
    gold = MODULE/'reference/swf-input-connection/dispatch-gold.bin'
    env = dict(os.environ, ASAN_OPTIONS='detect_leaks=1:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1')
    result = subprocess.run([str(binary), str(gold)], check=True, capture_output=True, text=True, env=env)
    audit = json.loads(result.stdout)
    assert audit['validation'] == 'PASS' and not result.stderr
    report = dict(validation='PASS', scope=__doc__, host_audit=audit,
                  source_sha256={str(path.relative_to(ROOT)):sha(path) for path in
                                 (MODULE/'swf_event_dispatch.hpp',MODULE/'swf_event_dispatch.cpp',MODULE/'swf_event_core.hpp',MODULE/'swf_event_core.cpp',
                                  MODULE/'tests/swf_event_dispatch.cpp',Path(__file__).resolve())},
                  original_instruction_evidence={str(path.relative_to(ROOT)):sha(path) for path in
                    (gold,MODULE/'reference/swf-input-connection/dispatch/original-functions.json',
                     MODULE/'reference/swf-input-connection/callbacks/original-functions.json',
                     MODULE/'reports/swf-event-dispatch-arm64-differential.json')},
                  ui_library=dict(path=str(library.resolve()),sha256=sha(library),actual_library_executed=True),
                  executable_sha256=sha(binary), compiler=subprocess.run(['c++','--version'],capture_output=True,text=True,check=True).stdout,
                  command=command, sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],
                  sanitizer_findings=0, whole_original_frame_parity=False, live_Android=False)
    a.report.parent.mkdir(parents=True, exist_ok=True)
    a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(audit))


if __name__ == '__main__':
    main()
