"""Pin the fetched official source and explicit patches; never fetch at build time."""
import hashlib,json,shutil,sys
from pathlib import Path
REPO=Path(__file__).resolve().parents[3]
SCRATCH=REPO/'.local-inputs/ui-swf-discovery'
TARGET=Path(__file__).resolve().parents[1]/'vendor/gameswf1714'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 src=SCRATCH/'upstream/build-core'; original=SCRATCH/'upstream/core'
 prior=TARGET.parents[1]/'reference/gameswf-core/vendor-manifest.json'
 known={x['path']:x['sha256'] for x in json.loads(prior.read_text())['files']} if prior.exists() else {}
 files=[]
 for p in sorted(src.rglob('*')):
  if not p.is_file():continue
  rel=p.relative_to(src);q=TARGET/rel
  # Source updates require regenerating the bound manifest; do not accept unrelated edits.
  if q.exists() and q.read_bytes()!=p.read_bytes() and not('--update' in sys.argv and known.get(rel.as_posix())==digest(q)):raise RuntimeError(f'Refuse overwrite mismatch: {q}')
  q.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(p,q)
  files.append(dict(path=rel.as_posix(),original_sha256=digest(original/rel),sha256=digest(q),size=q.stat().st_size))
 ref=TARGET.parents[1]/'reference/gameswf-core';ref.mkdir(parents=True,exist_ok=True)
 for name in ('core-manifest.json','portability-edits.json','game-adapter-edits.json'):
  shutil.copyfile(SCRATCH/'upstream'/name,ref/name)
 (ref/'vendor-manifest.json').write_text(json.dumps(dict(upstream_revision=1714,upstream_url='https://svn.code.sf.net/p/tu-testbed/code/!svn/bc/1714/trunk/tu-testbed/',files=files),indent=2)+'\n')
 cmake=(SCRATCH/'build/CMakeLists.txt').read_text();lines=cmake.split('add_library(gameswf_core STATIC\n',1)[1].split('\n)',1)[0]
 (TARGET.parents[1]/'gameswf_sources.cmake').write_text('''# Pinned official GameSWF r1714, explicit portability/fork patches in reference/gameswf-core.
set(DH2_GAMESWF_ROOT "${CMAKE_CURRENT_LIST_DIR}/vendor/gameswf1714")
set(DH2_GAMESWF_SOURCES
'''+lines.replace('${CORE}','${DH2_GAMESWF_ROOT}')+'''\n)
''')
 print('Vendored',len(files),'files')
if __name__=='__main__':main()
