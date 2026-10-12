"""Bind the actual HUD facade audit; fixture boundaries are part of its report."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def wslpath(p):
 p=Path(p).resolve().as_posix();return '/mnt/'+p[0].lower()+p[2:] if len(p)>2 and p[1]==':' else p
def main():
 p=argparse.ArgumentParser();p.add_argument('--executable',required=True);p.add_argument('--core-library',required=True);p.add_argument('--input-directory',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--sanitized',action='store_true');p.add_argument('--ndk-report',type=Path);a=p.parse_args()
 sources=[ROOT/'swf_movie.hpp',ROOT/'swf_movie.cpp',ROOT/'tests/swf_movie.cpp',ROOT/'gameswf_sources.cmake']
 manifest=ROOT/'reference/gameswf-core/vendor-manifest.json'
 sources += [ROOT/'vendor/gameswf1714'/x['path'] for x in json.loads(manifest.read_text())['files']]
 before={str(s.relative_to(REPO)):sha(s)for s in sources}
 env=['env','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=print_stacktrace=1'] if a.sanitized else []
 args=['wsl.exe','-e',*env,a.executable,wslpath(a.input_directory)]
 r=subprocess.run(args,capture_output=True,text=True);a.output.parent.mkdir(parents=True,exist_ok=True)
 (a.output.parent/(a.output.stem+'-stderr.txt')).write_text(r.stderr)
 values=json.loads(r.stdout) if r.stdout.strip() else {}
 binary=subprocess.check_output(['wsl.exe','-e','sha256sum',a.executable,a.core_library],text=True).splitlines()
 after={str(s.relative_to(REPO)):sha(s)for s in sources};assert before==after,'Source changed during proof'
 ndk=json.loads(a.ndk_report.read_text()) if a.ndk_report else None
 status=r.returncode==0 and values.get('validation')=='PASS' and (not a.sanitized or not r.stderr.strip())
 result=dict(validation='PASS' if status else 'FAIL',scope='Native upstream1714 HUD load/AS execution/display facade, not complete original fork parity',command=args,exit=r.returncode,observations=values,
  original_elf_sha256='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80',source_sha256=before,
  binaries=[dict(path=l.split(None,1)[1],sha256=l.split()[0])for l in binary],inputs=[dict(path=str(a.input_directory/n),sha256=sha(a.input_directory/n))for n in ('dqshared_droid.swf','dqhud_droid.swf')],
  sanitizer=dict(enabled=a.sanitized,address=a.sanitized,undefined=a.sanitized,vptr=False,leak_detection=a.sanitized,findings=0 if status and a.sanitized else None),
  ndk_compile=dict(sources=len(ndk),failures=sum(bool(x['exit'])for x in ndk)) if ndk else None,
  limitations=['Texture/image upload and localization providers are explicit host fixtures.','Core AS package lookup diagnostics remain; full ActionScript parity is unproved.','No original ARM32 runtime is linked.','No GL pixels, input, full UI layout or provider-font parity is inferred.','Original source bitmap-export/matrix captures are provenance, not a complete original differential corpus.'])
 a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(validation=result['validation'],observations=values)));raise SystemExit(not status)
if __name__=='__main__':main()
