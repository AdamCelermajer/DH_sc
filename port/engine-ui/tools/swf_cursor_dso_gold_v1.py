"""Execute frozen cursor gold through an actual UI DSO, with source trig imports."""
import argparse, hashlib, json, os, subprocess
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
UI = ROOT / 'port/engine-ui'
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--library', type=Path, required=True)
    p.add_argument('--dependencies', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    library = a.library.resolve(); out = a.output.resolve()
    assert out.is_relative_to(ROOT) and not a.report.exists()
    assert library.name == 'libdh2_engine_ui.so'
    out.mkdir(parents=True, exist_ok=True)
    before = sha(library)
    sources = [UI/'tests/swf_cursor_input_gold.cpp', UI/'tests/swf_cursor_gold_libm_v1.cpp']
    source_hashes = {str(x.relative_to(ROOT)): sha(x) for x in sources+[Path(__file__).resolve()]}
    gold = UI/'reference/swf-native-input-frame-v1/input-gold.bin'
    exe = out/'cursor_dso_gold_v1'
    command = ['g++', '-std=c++17', '-O1', '-g', '-fsanitize=address,undefined',
               '-fno-omit-frame-pointer', '-fno-fast-math', '-ffp-contract=off',
               '-fno-sanitize=vptr', '-I'+str(UI)] + list(map(str, sources)) + [
               '-L'+str(library.parent), '-Wl,--no-as-needed', '-ldh2_engine_ui',
               '-Wl,--export-dynamic-symbol=sinf', '-Wl,--export-dynamic-symbol=cosf',
               '-Wl,--export-dynamic-symbol=sincosf', '-o', str(exe)]
    subprocess.run(command, check=True)
    env = dict(os.environ, LD_LIBRARY_PATH=str(library.parent)+':'+str(a.dependencies.resolve()),
               ASAN_OPTIONS='detect_leaks=1:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1')
    ldd = subprocess.check_output(['ldd', str(exe)], env=env, text=True)
    assert str(library) in ldd and 'not found' not in ldd
    run = subprocess.run([str(exe), str(gold)], env=dict(env, LD_DEBUG='bindings'),
                         text=True, capture_output=True)
    (out/'bindings.log').write_text(run.stderr)
    if run.returncode: print(run.stdout+run.stderr); run.check_returncode()
    assert all(x not in run.stderr for x in ['ERROR: AddressSanitizer', 'runtime error:', 'LeakSanitizer'])
    bindings = [x.strip() for x in run.stderr.splitlines() if str(library) in x and
                ' to '+str(exe)+' ' in x and any('`'+n+"'" in x for n in ['sinf','cosf','sincosf'])]
    assert bindings, 'Actual UI trig import did not bind to the executable source fixture'
    result = json.loads(run.stdout); assert result['validation']=='PASS' and result['mismatches']==0
    assert sha(library)==before
    assert all(sha(ROOT/x)==h for x,h in source_hashes.items())
    evidence = UI/'reference/swf-native-input-frame-v1/freeze-manifest.json'
    a.report.parent.mkdir(parents=True, exist_ok=True)
    a.report.write_text(json.dumps(dict(validation='PASS', actual_ui_library_executed=True,
        library=dict(path=str(library), sha256=before), executable_sha256=sha(exe),
        source_sha256=source_hashes, original_gold_sha256=sha(gold),
        original_evidence=dict(path=str(evidence.relative_to(ROOT)), sha256=sha(evidence)),
        original_instructions_executed_this_run=False, libm_source_fixture=True,
        native_libm_parity=False, compile_command=command, ldd=ldd, actual_trig_bindings=bindings,
        binding_log_sha256=sha(out/'bindings.log'), host_audit=result,
        sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],
        sanitizer_findings=0), indent=2)+'\n')
    print(run.stdout, end='')
if __name__ == '__main__': main()
