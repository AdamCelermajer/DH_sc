"""Deterministic isolated PAB1 banks from executed original Crypt registrations.
No source APK assets are written. Real cache bytes are copied without edits.
"""
import argparse,hashlib,json,struct,zipfile
from pathlib import Path
HERE=Path(__file__).resolve().parent;REPO=HERE.parents[3]
ORIGINAL='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
CACHE='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
KINDS=[('skeleton','Crypt_Skeleton',62,15875,1203,44,23),('slime','CryptSlime',64,16387,1267,37,23),('slime-red','CryptSlime_RE',64,16387,1267,37,23),('ghost','Crypt_Ghost',24,6147,716,28,18)]
def sha(raw):return hashlib.sha256(raw).hexdigest()
def file_sha(p):return sha(p.read_bytes())
def word(v):assert type(v)==int and 0<=v<=0x7fffffff;return struct.pack('<I',v)
def text(s):
 b=s.encode('ascii');assert b and len(b)<=4096 and all(32<=v<=126 for v in b);return word(len(b))+b
def digest(s):assert len(s)==64 and s==s.lower() and any(c!='0' for c in s);return bytes.fromhex(s)
def valid_path(s):assert s and not s.startswith('/') and not s.endswith('/') and '\\' not in s and ':' not in s and not any(p in ('','.','..')for p in s.split('/'));return s
def pab(raw,m):
 resources=m['resources'];order=m['registration_requests'];assert [r['clip_id']for r in resources]==list(dict.fromkeys(order));assert order[0]==m['template_clip_id'];records=b''
 for key in ['asset','authored_path','cache_entry']:assert len({r[key]for r in resources})==len(resources)
 for r in resources:records+=word(r['clip_id'])+word(r['bytes'])+digest(r['sha256'])+text(valid_path(r['authored_path']))+text(valid_path(r['asset']))+text(valid_path(r['cache_entry']))
 body=word(1)+word(m['animation_table'])+word(m['animation_set_id'])+word(m['template_clip_id'])+word(len(resources))+word(len(order))+digest(sha(raw))+digest(CACHE)+digest(ORIGINAL)+digest(m['producer_sha256'])+text(m['character'])+records+b''.join(word(i)for i in order)
 return b'PAB1'+word(1)+word(len(body)+12)+body
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--cache',type=Path,default=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'));p.add_argument('--output',type=Path,default=REPO/'.local-inputs/character-monster-bank-assets');p.add_argument('--verify-only',action='store_true');a=p.parse_args()
 with a.cache.open('rb')as f:assert hashlib.file_digest(f,'sha256').hexdigest()==CACHE
 assert file_sha(REPO/'.local-inputs/libDungeonHunter2.so')==ORIGINAL
 # Writes are bounded to the workspace's isolated staging tree.
 out=a.output.resolve();out.relative_to((REPO/'.local-inputs').resolve());assets=out/'assets';outputs={};source={};unique={}
 def write(path,data):
  path.relative_to(out)
  if path.exists():assert path.read_bytes()==data,('Refusing mismatching staging overwrite',path)
  else:assert not a.verify_only,('Missing verify-only output',path);path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(data)
  outputs[path.relative_to(out).as_posix()]={'bytes':len(data),'sha256':sha(data)}
 placements=json.loads((REPO/'port/level-world/reports/object-input-provenance.json').read_text())
 with zipfile.ZipFile(a.cache)as archive:
  index={}
  for info in archive.infolist():index.setdefault(info.filename.lower(),[]).append(info)
  def resource(r,asset):
   found=index[r['entry'].lower()];assert len(found)==1 and found[0].filename==r['entry'];data=archive.read(found[0]);assert len(data)==r['bytes'] and sha(data)==r['sha256'];write(assets/asset,data);prior=unique.setdefault(asset,{'entry':r['entry'],'bytes':len(data),'sha256':sha(data)});assert prior=={'entry':r['entry'],'bytes':len(data),'sha256':sha(data)};return data
  for tag,character,table,set_id,template,requests,count in KINDS:
   probe_path=HERE/(tag+'-probe.json');probe=json.loads(probe_path.read_text());assert probe['validation']=='PASS' and probe['original_sha256']==ORIGINAL and probe['cache_sha256']==CACHE;assert probe['manifest_sha256']==file_sha(HERE/'original-functions.json') and probe['script_sha256']==file_sha(HERE/'probe.py');assert (probe['character'],probe['animation_table'],probe['animation_set_id'],probe['template']['clip_id'],probe['registration_calls_count'],probe['unique_clip_count'])==(character,table,set_id,template,requests,count)
   assert probe['authored_skill_list']==-1 and probe['skill_list']==3 and probe['skill_sequences']==[];assert all(not r['unsupported_domain_tracks']for r in probe['resources']);source[probe_path.relative_to(REPO).as_posix()]=file_sha(probe_path);resources=[]
   for r in probe['resources']:
    asset='animations/'+Path(r['path']).name;resource(r,asset);resources.append({'clip_id':r['clip_id'],'bytes':r['bytes'],'sha256':r['sha256'],'authored_path':r['path'],'asset':asset,'cache_entry':r['entry']})
   model=probe['model_property'];model_asset='actors/'+Path(model['path']).name;resource(model,model_asset)
   m={'character':character,'character_row':probe['character_row'],'animation_table':table,'animation_set_id':set_id,'template_clip_id':template,'identity_policy':1,'registration_requests':[r['clip_id']for r in probe['registration_calls']],'first_unique_resource_order':probe['first_unique_order'],'resources':resources,'cache_sha256':CACHE,'original_sha256':ORIGINAL,'producer_sha256':file_sha(probe_path),'registration_producer_sha256':file_sha(HERE/'probe.py'),'producer_manifest_sha256':file_sha(HERE/'original-functions.json'),'model_asset':model_asset,'model_property':model,'native_row160':probe['row160_projection'],'native_row_fields':probe['native_character_table_fields'],'idle_sequence':probe['source_idle_sequence'],'authored_skill_list':-1,'selected_skill_list':3,'source_placements':[r['name']for r in placements['records']if r['kind']==1 and r['character']==character],'scope':'Executed source non-player registration requests; genuine cache resources/explicit manager services. PAB1 is a port metadata format, not original serialization/cache ownership.'}
   raw=(json.dumps(m,indent=2)+'\n').encode();write(assets/'data'/('monster-'+tag+'-animation-bank.json'),raw);write(assets/'data'/('monster-'+tag+'-animation-bank.bin'),pab(raw,m))
  assert len([r for r in unique if r.startswith('animations/')])==64
 source[HERE.relative_to(REPO).as_posix()+'/produce.py']=file_sha(Path(__file__));source[HERE.relative_to(REPO).as_posix()+'/original-functions.json']=file_sha(HERE/'original-functions.json');source['port/level-world/reports/object-input-provenance.json']=file_sha(REPO/'port/level-world/reports/object-input-provenance.json')
 report={'validation':'PASS','format':'PAB1-v1','original_sha256':ORIGINAL,'cache_sha256':CACHE,'source_sha256':source,'output_sha256':outputs,'unique_resources':unique,'unique_animation_files':64,'bank_count':4,'ordered_occurrences_by_kind':{k[0]:k[5]for k in KINDS},'source_APK_assets_changed':False,'scope':__doc__}
 write(out/'producer-report.json',(json.dumps(report,indent=2)+'\n').encode());print(json.dumps({'validation':'PASS','output':str(out),'bank_count':4,'unique_animation_files':64,'ordered_occurrences':sum(k[5]for k in KINDS)}))
if __name__=='__main__':main()
