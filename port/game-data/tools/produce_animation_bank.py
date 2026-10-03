"""Produce PAB1 metadata from the exact source-derived Prince JSON manifest."""
import argparse,hashlib,json,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
EXPECTED='76633bb4f0ab5f645f4516e407e1f926df61641ea66faa805f68da57f043eb81'
def sha(raw):return hashlib.sha256(raw).hexdigest()
def path(s):
 assert isinstance(s,str) and s and len(s)<=4096 and all(32<=ord(c)<=126 for c in s),s
 assert not s.startswith('/') and not s.endswith('/') and '\\' not in s and ':' not in s and not any(p in ('','.','..') for p in s.split('/')),s
 return s
def text(s):
 raw=s.encode('ascii');assert 0<len(raw)<=4096 and all(32<=b<=126 for b in raw)
 return struct.pack('<I',len(raw))+raw
def word(v):
 assert type(v)==int and 0<=v<=0x7fffffff,v
 return struct.pack('<I',v)
def digest(s):
 assert isinstance(s,str) and len(s)==64 and s==s.lower() and any(c!='0' for c in s)
 raw=bytes.fromhex(s);assert len(raw)==32;return raw
def produce(manifest,assets):
 raw=manifest.read_bytes();assert sha(raw)==EXPECTED,'Source JSON SHA differs; review source provenance before producing'
 m=json.loads(raw);resources=m['resources'];order=m['registration_requests'];unique=m['first_unique_resource_order']
 assert len(resources)==116 and len(order)==158 and unique==list(dict.fromkeys(order)) and [r['clip_id'] for r in resources]==unique
 assert order[0]==m['template_clip_id']==1111 and m['animation_table']==48 and m['animation_set_id']==12302
 records=b'';input_files=[]
 for field in ('asset','authored_path','cache_entry'):assert len({r[field] for r in resources})==len(resources),field
 for r in resources:
  asset=path(r['asset']);authored=path(r['authored_path']);cache=path(r['cache_entry']);file=assets/asset;data=file.read_bytes();assert len(data)==r['bytes'] and sha(data)==r['sha256'],asset
  records+=word(r['clip_id'])+word(r['bytes'])+digest(r['sha256'])+text(authored)+text(asset)+text(cache)
  input_files.append({'asset':asset,'bytes':len(data),'sha256':sha(data)})
 body=word(1)+word(m['animation_table'])+word(m['animation_set_id'])+word(m['template_clip_id'])+word(len(resources))+word(len(order))+digest(EXPECTED)+digest(m['cache_sha256'])+digest(m['original_sha256'])+digest(m['producer_sha256'])+text(m['character'])+records+b''.join(word(id) for id in order)
 binary=b'PAB1'+word(1)+word(len(body)+12)+body
 report={'validation':'PASS','format':'PAB1-v1','manifest_sha256':EXPECTED,'binary_sha256':sha(binary),'binary_bytes':len(binary),'resources':len(resources),'registration_requests':len(order),'template_clip_id':m['template_clip_id'],'identity_policy':'one port cache token per exact unique asset path; token=first-unique resource index+1','cache_sha256':m['cache_sha256'],'original_sha256':m['original_sha256'],'source_producer_sha256':m['producer_sha256'],'resource_inputs':input_files,'scope':'Exact JSON metadata transformation and actual116 asset-byte preflight. Not an original binary file format or CCDB pointer reconstruction.'}
 return binary,report
def main():
 p=argparse.ArgumentParser();p.add_argument('--manifest',type=Path,default=REPO/'port/android-native/app/src/main/assets/data/prince-animation-bank.json');p.add_argument('--assets',type=Path,default=REPO/'port/android-native/app/src/main/assets');p.add_argument('--output',type=Path,default=REPO/'port/android-native/app/src/main/assets/data/prince-animation-bank.bin');p.add_argument('--report',type=Path,default=ROOT/'reports/animation-bank-producer.json');p.add_argument('--verify-only',action='store_true');a=p.parse_args();binary,report=produce(a.manifest,a.assets)
 if a.output.exists():assert a.output.read_bytes()==binary,'Refusing to overwrite a mismatching animation bank binary'
 else:
  assert not a.verify_only,'Binary asset missing';a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_bytes(binary)
 report['script_sha256']=sha(Path(__file__).read_bytes());a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='resource_inputs'}))
if __name__=='__main__':main()
