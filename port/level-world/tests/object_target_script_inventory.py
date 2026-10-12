"""Exact cache bytes and executable colon-method demand; comments are excluded."""
import hashlib,json,re,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/object-identity-lifecycle'
CACHE=Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
def sha(raw):return hashlib.sha256(raw).hexdigest()
def main():
 assert sha(CACHE.read_bytes())=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
 rows=[];catalog=(ROOT/'gameobject_lua_catalog.inc').read_text();methods={name:int(addr,16) for name,addr in re.findall(r'\{"([^"\n]+)",0x([0-9a-f]+),0\}',catalog)}
 with zipfile.ZipFile(CACHE) as z:
  for name in ('monster','follower','rene','_commons'):
   member='com.gameloft.android.GAND.GloftD2SS/files/data/scripts/ai/'+name+'.luac';raw=z.read(member);text=raw.decode('utf-8');code=re.sub(r'--\[\[.*?\]\]','',text,flags=re.S);code=re.sub(r'--[^\n]*','',code);colon=[]
   for match in re.finditer(r'([A-Za-z_][\w.]*)\s*:\s*([A-Za-z_]\w*)\s*\(',code):colon.append({'receiver':match[1],'method':match[2],'source_address':hex(methods[match[2]])})
   target_names=['HasTarget','GetTarget','SetTarget','ClearTarget','GetID','HeadTo','ClearAggro','Stop','MoveTo','Attack','Flee','HasPath','GetState','GetStateTime']
   calls={method:len(re.findall(r'\b'+method+r'\s*\(',code)) for method in target_names}
   rows.append({'script':name,'cache_member':member,'sha256':sha(raw),'bytes':len(raw),'executable_colon_methods':colon,'target_related_call_counts':{k:v for k,v in calls.items() if v}})
 assert rows[0]['executable_colon_methods']==[{'receiver':'defender','method':'GetID','source_address':'0x38ebe4'}]
 assert all(not row['executable_colon_methods'] for row in rows[1:])
 result={'validation':'PASS','scope':__doc__,'cache_sha256':sha(CACHE.read_bytes()),'catalog_sha256':sha((ROOT/'gameobject_lua_catalog.inc').read_bytes()),'scripts':rows,'registered_method_addresses':{name:hex(methods[name]) for name in target_names if name in methods}}
 (REF/'target-script-inventory.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
