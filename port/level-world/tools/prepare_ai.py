"""Bundle original AI/faction tables with cache identity and input provenance."""
import argparse,hashlib,json,zipfile
from pathlib import Path
from inventory import EXPECTED

def main():
 p=argparse.ArgumentParser();p.add_argument('cache',type=Path);p.add_argument('--project',type=Path,default=Path(__file__).resolve().parents[2]/'android-native');a=p.parse_args()
 with a.cache.open('rb') as f:assert hashlib.file_digest(f,'sha256').hexdigest()==EXPECTED
 assets=a.project/'app/src/main/assets';dest=assets/'data';dest.mkdir(parents=True,exist_ok=True);inputs=[]
 with zipfile.ZipFile(a.cache) as z:
  for name in ('ai_pyarray.bin','ai_pyarraynames.bin','ai_pystructnames.bin','ai_factions_pyarray.bin','ai_factions_pyarraynames.bin','ai_factions_pystructnames.bin'):
   matches=[n for n in z.namelist() if n.endswith('/'+name)];assert len(matches)==1;raw=z.read(matches[0]);(dest/name).write_bytes(raw);inputs.append({'asset':'data/'+name,'entry':matches[0],'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()})
 report={'cache_sha256':EXPECTED,'inputs':inputs,'scope':'Original AIProps and faction tables; native reader and character-branch geometry/target events. Spatial query, pursuit and full FSM remain separate.'}
 (assets/'ai-provenance.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
