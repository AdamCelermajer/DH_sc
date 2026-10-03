"""Extract named owner inputs with whole-archive and entry validation."""
import argparse,hashlib,json,zipfile
from pathlib import Path
from inventory import EXPECTED
def main():
 p=argparse.ArgumentParser();p.add_argument('cache',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--names',nargs='*',default=[]);p.add_argument('--list',action='store_true');a=p.parse_args()
 with a.cache.open('rb') as f:assert hashlib.file_digest(f,'sha256').hexdigest()==EXPECTED
 records=[];a.output.mkdir(parents=True,exist_ok=True)
 with zipfile.ZipFile(a.cache) as z:
  if a.list:
   for entry in z.infolist():
    if any(word in entry.filename.lower() for word in a.names):print(entry.filename,entry.file_size)
   return
  for name in a.names:
   assert Path(name).name==name
   matches=[e for e in z.infolist() if Path(e.filename).name==name];assert len(matches)==1,(name,len(matches))
   entry=matches[0];raw=z.read(entry);(a.output/name).write_bytes(raw);records.append({'name':name,'entry':entry.filename,'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()})
 (a.output/'input-provenance.json').write_text(json.dumps({'cache_sha256':EXPECTED,'inputs':records},indent=2)+'\n')
 print(json.dumps(records))
if __name__=='__main__':main()
