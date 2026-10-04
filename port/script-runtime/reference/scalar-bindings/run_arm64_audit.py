"""Build the independent optimized Android ARM64 scalar oracle and execute original gold."""
import hashlib,json,os,subprocess,sys
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
NDK=Path(os.environ['LOCALAPPDATA'])/'Android/Sdk/ndk/29.0.14206865'
LLVM=NDK/'toolchains/llvm/prebuilt/windows-x86_64'
OUT=ROOT/'.local-inputs/lua514-source/scalar/libscript_scalar_oracle.so'
OUT.parent.mkdir(parents=True,exist_ok=True)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
compiler=LLVM/'bin/clang.exe'
args=[str(compiler),'--target=aarch64-linux-android24','--sysroot='+str(LLVM/'sysroot'),'-shared','-fPIC','-O2','-fno-fast-math','-ffp-contract=off','-Wl,-z,max-page-size=16384',str(ROOT/'port/script-runtime/script_scalar_bindings.c'),'-o',str(OUT)]
r=subprocess.run(args,capture_output=True,text=True,check=True);assert not r.stderr.strip(),r.stderr
test=ROOT/'port/script-runtime/tests/script_scalar_differential.py'
replay=subprocess.run([sys.executable,str(test),'--library',str(OUT)],capture_output=True,text=True,check=True)
assert not replay.stderr.strip(),replay.stderr;result=json.loads(replay.stdout);assert result['validation']=='PASS'
report=dict(validation='PASS',compiler=dict(path=str(compiler),sha256=sha(compiler)),ndk_version=(NDK/'source.properties').read_text(),arguments=args,library_sha256=sha(OUT),script_sha256=sha(Path(__file__)),test_script_sha256=sha(test),replay=result)
(ROOT/'port/script-runtime/reports/script-scalar-arm64-build.json').write_text(json.dumps(report,indent=2)+'\n');print(replay.stdout.strip())
