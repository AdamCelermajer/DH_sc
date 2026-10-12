"""Bundle exact original level data and required Crypt initialization scripts."""
import argparse,hashlib,json,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(raw):return hashlib.sha256(raw).hexdigest()
def main():
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--studio',type=Path);parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
 if args.output.exists():raise RuntimeError('Preserve previous asset staging proof')
 cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');cache_hash=sha(cache.read_bytes());assert cache_hash=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
 levels=json.loads((ROOT/'port/game-data/reference/level-tables/original-loader-capture.json').read_text());scripts=json.loads((ROOT/'port/level-world/reference/character-script-kinds/authored-ai-rows.json').read_text())
 wanted={name:(f'data/{name}',digest,None) for name,digest in levels['assets_sha256'].items()}
 for record in scripts['required_cache_resources']:
  if record['requested'] in ('_commons','monster'):
   name=record['requested']+'.luac';wanted[name]=(f'data/scripts/ai/{name}',record['sha256'],record['entry'])
 assert len(wanted)==5
 payloads={}
 with zipfile.ZipFile(cache) as archive:
  for name,(relative,digest,exact_entry) in wanted.items():
   entries=[exact_entry] if exact_entry else [entry for entry in archive.namelist() if entry.endswith('/data/pydata/'+name)];assert len(entries)==1
   raw=archive.read(entries[0]);assert sha(raw)==digest
   payloads[relative]=(raw,digest,entries[0])
 projects={'repository':ROOT/'port/android-native'}
 if args.studio:projects['studio']=args.studio
 for project in projects.values():
  assert (project/'app/src/main').is_dir()
  for relative,(raw,_,_) in payloads.items():
   path=project/'app/src/main/assets'/relative
   if path.exists():assert path.read_bytes()==raw,('Preserve differing asset',path)
 results={}
 for tag,project in projects.items():
  results[tag]=[]
  for relative,(raw,digest,entry) in payloads.items():
   path=project/'app/src/main/assets'/relative;created=not path.exists()
   if created:path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(raw)
   assert sha(path.read_bytes())==digest
   results[tag].append(dict(path=str(path.resolve()),sha256=digest,bytes=len(raw),cache_entry=entry,created=created))
 report=dict(validation='PASS',scope=__doc__,cache_sha256=cache_hash,projects=results,
  files=5,all_original_game_assets_bundled=False,original_scripts_executed_in_APK=False,
  source_Application_level_selection_verified=False)
 args.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',files=5,projects=list(projects))))
if __name__=='__main__':main()
