"""Exact source cache inputs for the retained player session audit, no APK writes."""
import hashlib,json,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
ZIP=Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
def sha(b):return hashlib.sha256(b).hexdigest()
def main():
 destination=ROOT/'.local-inputs/character-skill-session-v2/cache';rows=[]
 with zipfile.ZipFile(ZIP)as z:
  for item in z.infolist():
   if '/files/data/'not in item.filename:continue
   relative='data/'+item.filename.split('/files/data/',1)[1]
   if not(relative.startswith('data/scripts/skills/')or relative=='data/scripts/ai/_commons.luac'or relative in['data/pydata/faeries_'+s+'.bin'for s in('pyarray','pyarraynames','pystructnames')]):continue
   if item.is_dir():continue
   target=(destination/relative).resolve();assert target.is_relative_to(destination.resolve())
   b=z.read(item)
   if target.exists()and target.read_bytes()!=b:raise RuntimeError('Existing source input differs: '+relative)
   target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(b);rows.append(dict(member=item.filename,path=target.relative_to(ROOT).as_posix(),bytes=len(b),sha256=sha(b)))
 ref=ROOT/'port/level-world/reference/character-skill-session-v2';ref.mkdir(parents=True,exist_ok=True)
 (ref/'cache-inputs.json').write_text(json.dumps(dict(validation='PASS',source_zip=str(ZIP),expected_source_zip_sha256='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679',inputs=rows),indent=2)+'\n');print(json.dumps(dict(validation='PASS',inputs=len(rows))))
if __name__=='__main__':main()
