"""Bundle proved ordinary integer constants without rewriting existing assets."""
import argparse,hashlib,json,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 if a.output.exists():raise RuntimeError('Refusing to replace asset provenance')
 cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
 proof=ROOT/'port/script-runtime/reference/design-constants-loader/original-loader-probe.json';report=json.loads(proof.read_text())
 assert report['validation']=='PASS' and sha(cache)==report['cache_sha256']
 records=[r for r in report['inputs'] if not r['input'].startswith('synthetic/') and not r['source_name_stop']]
 assert len(records)==26 and sum(r['assignments'] for r in records)==5608
 assets=ROOT/'port/android-native/app/src/main/assets/data';assets.mkdir(parents=True,exist_ok=True);bundled=[]
 with zipfile.ZipFile(cache) as z:
  for row in records:
   raw=z.read(row['input']);assert hashlib.sha256(raw).hexdigest()==row['sha256'] and row['consumed']==len(raw)
   dest=assets/Path(row['input']).name
   if dest.exists():assert dest.read_bytes()==raw,'Different existing asset: '+str(dest)
   else:dest.write_bytes(raw)
   bundled.append(dict(path=str(dest.relative_to(ROOT)),cache_entry=row['input'],bytes=len(raw),sha256=sha(dest)))
 result=dict(validation='PASS',scope=__doc__,cache_sha256=sha(cache),original_loader_proof_sha256=sha(proof),tool_sha256=sha(Path(__file__)),constants_files=26,integer_assignments=5608,bundled_assets=bundled,sounds_mixed_format_bundled=False,full_game_assets_bundled=False,application_startup_loader_wired=False)
 a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(validation='PASS',files=26,integer_assignments=5608,bytes=sum(r['bytes'] for r in bundled))))
if __name__=='__main__':main()
