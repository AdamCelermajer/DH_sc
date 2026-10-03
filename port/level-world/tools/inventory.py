"""Inventory owner-supplied scene/world resources before selecting a level."""
import argparse,collections,hashlib,json,struct,zipfile
from pathlib import Path
EXPECTED='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
def main():
 p=argparse.ArgumentParser();p.add_argument('cache',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 with a.cache.open('rb') as f:assert hashlib.file_digest(f,'sha256').hexdigest()==EXPECTED
 rows=[];ext=collections.Counter()
 with zipfile.ZipFile(a.cache) as z:
  for entry in z.infolist():
   if entry.is_dir():continue
   name=Path(entry.filename).name;suffix=Path(name).suffix.lower();ext[suffix]+=1
   if suffix=='.bdae':
    raw=z.read(entry);r=struct.unpack_from('<I',raw,32)[0] if len(raw)>60 else 0
    w=lambda offset:struct.unpack_from('<I',raw,r+offset)[0]
    row={'name':name,'entry':entry.filename,'bytes':len(raw),'geometries':w(104),'controllers':w(112),'scenes':w(152),'animations':w(36),'images':w(76)};rows.append(row)
   elif suffix in ('.xml','.lua','.nav','.map','.world','.scene','.bin','.dat'):
    rows.append({'name':name,'entry':entry.filename,'bytes':entry.file_size})
 report={'cache_sha256':EXPECTED,'extensions':dict(ext),'resources':rows};a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({'extensions':dict(ext),'world_candidates':[r for r in rows if r.get('geometries',0)>200]}))
if __name__=='__main__':main()
