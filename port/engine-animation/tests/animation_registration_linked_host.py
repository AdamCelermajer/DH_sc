"""Build and bind the original-derived registration replay to the real host DSO."""
import argparse
import hashlib
import json
import re
import subprocess
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report', type=Path, required=True)
    args = parser.parse_args()
    if args.report.exists():
        raise RuntimeError('Refusing to overwrite a captured library proof')
    started = time.monotonic()
    gold = ROOT / 'reference/animation-registration/original-corpus.bin'
    modules = ('engine-animation','scene-materials','engine-resources','asset-payloads','engine-math')
    paths = [p for module in modules for p in (REPO/'port'/module).glob('*')
             if p.is_file() and (p.suffix in ('.cpp','.hpp') or p.name == 'CMakeLists.txt')]
    paths += [ROOT/'tests/animation_registration.cpp',Path(__file__)]
    snapshot = lambda: {str(p.relative_to(REPO)).replace('\\','/'):sha(p) for p in sorted(paths)}
    before = snapshot()
    commands = []

    def run(command):
        result = subprocess.run(['wsl.exe','--','bash','-lc',command],capture_output=True,text=True)
        commands.append(dict(command=command,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr))
        if result.returncode:
            raise RuntimeError(result.stdout+result.stderr)
        if 'AddressSanitizer:' in result.stderr or 'runtime error:' in result.stderr:
            raise RuntimeError(result.stderr)
        return result.stdout

    build = '/home/adampalace/dh2-world-build'
    executable = build+'/engine-skinning/engine-animation/animation_registration_audit'
    run('cmake --build '+build+' --target animation_registration_audit -j8')
    output = run('ASAN_OPTIONS=detect_leaks=1:abort_on_error=1 UBSAN_OPTIONS=halt_on_error=1 '+executable+
                 ' /mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-animation/reference/animation-registration/original-corpus.bin')
    validation = json.loads(output.strip())
    assert validation['validation'] == 'PASS' and validation['cases'] == 42
    assert validation['appends'] == 2529 and validation['lookups'] == 2529
    assert validation['entry_checks'] == 2120 and validation['atomic_rejections'] == 211
    assert validation['sanitizer_findings'] == 0
    needed = run('readelf -d '+executable)
    assert 'libdh2_engine_animation.so' in needed and 'libasan.so' in needed
    dependencies = run('ldd '+executable)
    binaries = [executable]+re.findall(r'libdh2_\S+\s+=>\s+(\S+)',dependencies)
    assert len(binaries) == 3 and all(path.startswith(build+'/') for path in binaries)
    hashes = {}
    for path in binaries:
        hashes[path] = run('sha256sum '+path).split()[0]
    assert before == snapshot(), 'Compiler source changed during library proof'
    report = dict(validation='PASS',checks=validation,source_sha256=before,binary_sha256=hashes,
                  gold_sha256=sha(gold),original_report_sha256=sha(ROOT/'reports/animation-registration-arm64-differential.json'),
                  commands=commands,elapsed_seconds=round(time.monotonic()-started,2),
                  scope='Actual CMake shared animation library linked replay of original-derived registration. '
                        'Borrowed resource identities are explicit fixtures. No raw transform, renderer, APK or device claim.')
    args.report.parent.mkdir(parents=True,exist_ok=True)
    args.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(validation))


if __name__ == '__main__':
    main()
