"""Compile readiness only; original/O2 execution is in the two differential reports."""
import argparse,hashlib,json,os,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--ndk',default=str(Path(os.environ['LOCALAPPDATA'])/'Android/Sdk/ndk/29.0.14206865'));a=p.parse_args()
 tool=Path(a.ndk)/'toolchains/llvm/prebuilt/windows-x86_64/bin'
 sources=['port/engine-ui/hud_text_format_v1.cpp','port/engine-ui/hud_text_v1.cpp','port/level-world/character_skill_info_v1.cpp','port/level-world/character_skill_properties_v1.cpp','port/level-world/character_skill_info_session_v1.cpp','port/level-world/character_hud_skill_text_v1.cpp','port/script-runtime/script_runtime_return_v1.c']
 before={x:sha(ROOT/x)for x in sources};commands=[]
 for abi,target in [('arm64-v8a','aarch64-linux-android26'),('x86_64','x86_64-linux-android26')]:
  for source in sources:
   cpp=source.endswith('.cpp');command=[str(tool/('clang++.exe'if cpp else 'clang.exe')),'--target='+target,'-std=c++17'if cpp else '-std=c99','-fsyntax-only','-fno-fast-math','-ffp-contract=off','-I'+str(ROOT/'port/script-runtime/lua'),str(ROOT/source)]
   r=subprocess.run(command,text=True,capture_output=True)
   if r.returncode or r.stderr:raise RuntimeError(r.stdout+r.stderr)
   commands.append(dict(abi=abi,source=source,command=command))
 assert before=={x:sha(ROOT/x)for x in sources}
 report=dict(validation='PASS',source_sha256=before,translation_units=14,abis=['arm64-v8a','x86_64'],commands=commands,scope='NDK source syntax readiness; not packaged linkage, device execution, or source behavior evidence.')
 dest=ROOT/'port/engine-ui/reports/hud-formatting-v1-android-readiness.json';dest.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',translation_units=14,report_sha256=sha(dest))))
if __name__=='__main__':main()
