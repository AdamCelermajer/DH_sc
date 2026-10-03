"""Bundle original class data; only build-time preparation uses Python."""
import argparse,hashlib,json,zipfile
from pathlib import Path
from inventory import EXPECTED
TABLES=('character_classes_pyarray.bin','character_classes_pyarraynames.bin','character_classes_pystructnames.bin','character_classes_pycst.bin')
def main():
 p=argparse.ArgumentParser();p.add_argument('cache',type=Path);p.add_argument('--project',type=Path,default=Path(__file__).resolve().parents[2]/'android-native');a=p.parse_args()
 with a.cache.open('rb') as f:assert hashlib.file_digest(f,'sha256').hexdigest()==EXPECTED
 assets=a.project/'app/src/main/assets';inputs=[]
 with zipfile.ZipFile(a.cache) as archive:
  for name in TABLES:
   entries=[e for e in archive.infolist() if Path(e.filename).name==name];assert len(entries)==1;raw=archive.read(entries[0]);(assets/'data'/name).write_bytes(raw);inputs.append({'asset':'data/'+name,'entry':entries[0].filename,'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()})
 report={'cache_sha256':EXPECTED,'inputs':inputs,'runtime_scope':'Native complete class reader and cached base-class snapshots. Dynamic gear/buff/property resolution, level selection and combat remain pending. Constants file retained as enum evidence.'}
 (assets/'class-provenance.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'original_class_files':len(inputs)}))
if __name__=='__main__':main()
