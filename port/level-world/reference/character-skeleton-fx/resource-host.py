"""Read exact FX283 scene/material/animation payload with existing native DSOs.
No original scene factory, shader/GPU or FX event loop parity is asserted.
"""
import hashlib,json,subprocess,zipfile
from pathlib import Path
HERE=Path(__file__).resolve().parent;REPO=HERE.parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
 deps=REPO/'.local-inputs/character-skeleton-fx-discovery/dependencies';snap=json.loads((deps/'snapshot.json').read_text());assert all(sha(deps/n)==v['sha256']for n,v in snap.items());probe=json.loads((HERE/'probe.json').read_text());assert probe['validation']=='PASS';resource=probe['resources']['283'];cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
 with cache.open('rb')as f:assert hashlib.file_digest(f,'sha256').hexdigest()==probe['cache_sha256']
 assets=REPO/'.local-inputs/character-skeleton-fx-assets';assets.mkdir(exist_ok=True);path=assets/'zombie_spawn_fx.bdae';textures={}
 with zipfile.ZipFile(cache)as z:
  raw=z.read(resource['entry']);assert hashlib.sha256(raw).hexdigest()==resource['sha256'];path.write_bytes(raw)
  for name in ['atlas_fx_particles_001','atlas_fx_particles_002']:
   found=[e for e in z.infolist()if '/textures/'+name+'.' in e.filename.lower()];assert len(found)==1
   blob=z.read(found[0]);textures[name]={'entry':found[0].filename,'bytes':len(blob),'sha256':hashlib.sha256(blob).hexdigest()};(assets/Path(found[0].filename).name).write_bytes(blob)
 commands=[]
 def run(*a):
  r=subprocess.run(['wsl','--cd',linux(REPO),*a],capture_output=True,text=True,timeout=120);commands.append(dict(arguments=a,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and not r.stderr.strip(),commands[-1];return r.stdout.strip()
 exe=REPO/'.local-inputs/character-skeleton-fx-discovery/resource-probe';run('g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-Wno-misleading-indentation',linux(HERE/'resource-probe.cpp'),'-L'+linux(deps),'-ldh2_engine_animation','-ldh2_scene_materials','-Wl,-rpath,'+linux(deps),'-o',linux(exe));audit=json.loads(run('env','LD_LIBRARY_PATH='+linux(deps),'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(exe),linux(path)));assert audit['validation']=='PASS';assert all(sha(deps/n)==v['sha256']for n,v in snap.items())
 report={'validation':'PASS','resource':resource,'textures':textures,'native_reader':audit,'private_snapshot':snap,'source_sha256':{str(p.relative_to(REPO)):sha(p)for p in [HERE/'resource-probe.cpp',Path(__file__)]},'executable_sha256':sha(exe),'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'scope':__doc__,'commands':commands};(HERE/'resource-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','native_reader':audit,'textures':textures}))
if __name__=='__main__':main()
