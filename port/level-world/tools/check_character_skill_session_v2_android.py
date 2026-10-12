"""Versioned whole player Session compile readiness, not packaged/link/device proof."""
import argparse,hashlib,json,os,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--ndk',default=str(Path(os.environ['LOCALAPPDATA'])/'Android/Sdk/ndk/29.0.14206865'));a=p.parse_args();tool=Path(a.ndk)/'toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
 names=['character_script_owner_v2','character_script_session_v2','character_skills_session_v2','character_skill_info_session_v2','character_script_player_vcb_v2','character_current_skill_v2','character_player_skills_v2'];sources=['port/level-world/'+n+'.cpp'for n in names];before={x:sha(ROOT/x)for x in sources};commands=[]
 for abi,target in [('arm64-v8a','aarch64-linux-android26'),('x86_64','x86_64-linux-android26')]:
  for s in sources:
   command=[str(tool),'--target='+target,'-std=c++17','-fsyntax-only','-fno-fast-math','-ffp-contract=off',str(ROOT/s)];r=subprocess.run(command,capture_output=True,text=True)
   if r.returncode or r.stderr:raise RuntimeError(r.stdout+r.stderr)
   commands.append(dict(abi=abi,source=s,command=command))
 assert before=={x:sha(ROOT/x)for x in sources};report=dict(validation='PASS',source_sha256=before,translation_units=14,abis=['arm64-v8a','x86_64'],commands=commands,scope=__doc__);dest=ROOT/'port/level-world/reports/character-skill-session-v2-android-readiness.json';dest.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',translation_units=14,report_sha256=sha(dest))))
if __name__=='__main__':main()
