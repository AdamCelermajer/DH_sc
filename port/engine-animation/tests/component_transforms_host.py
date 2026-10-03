"""Source and executable bound sanitizer replay for extended transforms and regressions."""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
REPO=ROOT.parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report',type=Path,required=True)
    args=parser.parse_args()
    if args.report.exists():
        raise RuntimeError('Refusing to overwrite sanitizer proof')
    paths=[p for module in ('engine-animation','scene-materials','engine-resources','asset-payloads','engine-math')
           for p in (REPO/'port'/module).glob('*') if p.is_file() and (p.suffix in ('.hpp','.cpp') or p.name=='CMakeLists.txt')]
    paths += [Path(__file__)]+[ROOT/'tests'/name for name in ('component_transforms.cpp','compiled_transforms.cpp','dynamic_compiled_transforms.cpp')]
    snapshot=lambda:{str(p.relative_to(REPO)).replace('\\','/'):sha(p) for p in sorted(paths)}
    before=snapshot()
    commands=[]

    def run(command):
        result=subprocess.run(['wsl.exe','--','bash','-lc',command],capture_output=True,text=True)
        commands.append(dict(command=command,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr))
        if result.returncode or 'AddressSanitizer:' in result.stderr or 'runtime error:' in result.stderr:
            raise RuntimeError(result.stdout+result.stderr)
        return result.stdout

    build='/home/adampalace/dh2-world-build'
    run('cmake --build '+build+' --target component_transforms_audit compiled_transforms_audit dynamic_compiled_transforms_audit -j8')
    cases=[('component_transforms_audit','component-transforms/actual-raw-factories/original-corpus.bin','samples',9376),
           ('compiled_transforms_audit','compiled-transforms/original-corpus.bin','original_raw_samples',11656),
           ('dynamic_compiled_transforms_audit','dynamic-compiled-transforms/original-corpus.bin','samples',117030)]
    checks,binaries,golds={}, {}, {}
    for target,gold,key,count in cases:
        exe=build+'/engine-skinning/engine-animation/'+target
        output=run('ASAN_OPTIONS=detect_leaks=1:abort_on_error=1 UBSAN_OPTIONS=halt_on_error=1 '+exe+
                   ' /mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-animation/reference/'+gold)
        result=json.loads(output.strip())
        assert result['validation']=='PASS' and result[key]==count and result['sanitizer_findings']==0
        checks[target]=result
        binaries[exe]=run('sha256sum '+exe).split()[0]
        dynamic=run('readelf -d '+exe)
        assert 'libasan.so' in dynamic and 'libubsan.so' in dynamic
        golds[gold]=sha(ROOT/'reference'/gold)
    assert before==snapshot(), 'Source changed during provenance replay'
    report=dict(validation='PASS',checks=checks,source_sha256=before,binary_sha256=binaries,gold_sha256=golds,
                original_capture_sha256=sha(ROOT/'reference/component-transforms/actual-raw-factories/original-capture.json'),
                original_sha256='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80',commands=commands,
                scope='Direct-source CMake sanitizer replay of actual original typed transforms. Imported libm results are original-gold fixtures; '
                      'no genuine DSO/GPU/live scene/renderer/APK/device claim. Static/dynamic regressions preserved.')
    args.report.parent.mkdir(parents=True,exist_ok=True)
    args.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(checks))


if __name__=='__main__':
    main()
