from pathlib import Path
import sys,json,struct,hashlib,zipfile
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
sys.path.insert(0,'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');sys.path.insert(0,str(b/'tests'))
from xml_original_probe import Parser
from unicorn import UC_HOOK_CODE
r=b/'reference/level-source-filename-route-v52';e=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so')
summary=b/'reports/chapter-coverage-v36-summary.json';doc=json.loads(summary.read_text());levels=doc['levels']
class Probe(Parser):
 def __init__(self):
  super().__init__(e,{'functions':[]});self.attempts=[];self.mode_queries=[];self.level=self.data+0x30000;self.application=self.data+0x32000;system=self.data+0x33000;files=self.data+0x34000;vt=self.data+0x35000;self.stub=self.data+0x50000
  self.uc.mem_write(self.stub,struct.pack('<I',0xe12fff1e));self.pointer(0x994a98+0x37f4,self.application);self.pointer(self.application+0x10,system);self.pointer(system+0x34,files);self.pointer(files,vt);self.pointer(vt+0x88,self.stub);self.uc.hook_add(UC_HOOK_CODE,self.allocation);self.uc.hook_add(UC_HOOK_CODE,self.boundary)
 def boundary(self,uc,a,size,u):
  if a==0x320678:self.mode_queries.append(self.text(self.reg(1))) # actual original body runs
  elif a==self.stub:self.attempts.append(self.text(self.reg(1)));self.returned(0)
 def external(self,uc,a,size,u):
  if self.imports.get(a)=='strstr':
   value,needle=self.text(self.reg(0)),self.text(self.reg(1));at=value.find(needle);self.returned(self.reg(0)+at if at>=0 else 0)
  else:super().external(uc,a,size,u)
 def run(self,name,uncompiled_range=False):
  self.uc.mem_write(self.level,bytes(0x200));self.attempts=[];self.mode_queries=[];self.pointer(self.application+0xcc,0);self.pointer(self.application+0xd0,4 if uncompiled_range else 0)
  s=self.data+0x38000;t=self.data+0x38100;inp=self.data+0x39000;tag=self.data+0x3a000;self.uc.mem_write(inp,name.encode()+b'\0');self.uc.mem_write(tag,b'Level\0');self.invoke(0x3140ec,[s,inp,0]);self.invoke(0x3140ec,[t,tag,0]);result=self.invoke(0x3f3b40,[self.level,s,t],budget=10000000)
  return {'input':name,'nonempty_source_cc_d0_range':uncompiled_range,'open_resource_attempts':self.attempts.copy(),'actual_original_mode_queries':self.mode_queries.copy(),'source_return_when_all_missing':result}
def projected(name):
 out=[]
 for prefix in ['', 'data/','data/scene/','data/3d/modules/']:
  candidate=prefix+name
  for token in ['old/','debug/','ps3/','iphone/']:
   if token in candidate:candidate=candidate.replace(token,'',1);break
  out.append(candidate)
 return out
p=Probe();rows=[]
with zipfile.ZipFile('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip') as z:
 entries={x.lower()[len('com.gameloft.android.gand.gloftd2ss/files/'):]:x for x in z.namelist() if x.lower().startswith('com.gameloft.android.gand.gloftd2ss/files/') and not x.endswith('/')}
 for level in levels:
  name=level['design_row']['file'];assert '/' not in name and '\\' not in name
  row=p.run(name);assert row['open_resource_attempts']==projected(name)
  row['original_level_id']=level.get('name',level.get('id',level.get('identifier')));row['actual_cache_hits']=[candidate for candidate in row['open_resource_attempts'] if candidate.lower() in entries];rows.append(row)
 witnesses=[p.run('worlds/001_swamp.mlx'),p.run('old/debug/ps3/iphone/001_swamp.mlx'),p.run('data/iphone/001_swamp.mlx')]
 for row in witnesses:assert row['open_resource_attempts']==projected(row['input'])
 for row in witnesses:row['actual_cache_hits']=[x for x in row['open_resource_attempts'] if x.lower() in entries]
 mode_cases=[p.run('old/001_swamp.mlx',True),p.run('debug/001_swamp.mlx',True),p.run('iphone/001_swamp.mlx',True),p.run('001_swamp.mlx',True)]
report={'validation':'PASS','original_sha256':hashlib.sha256(e.read_bytes()).hexdigest(),'summary_sha256':hashlib.sha256(summary.read_bytes()).hexdigest(),'source_LevelList_rows':len(rows),'original_LoadFile_and_Application_mode_bodies_executed':True,'filesystem_open_endpoint':'explicit missing-resource virtual; no XML/load success supplied','prefixes':['','data/','data/scene/','data/3d/modules/'],'first_token_removal':['old/','debug/','ps3/','iphone/'],'source_rows':rows,'fixture_witnesses':witnesses,'nonempty_range_mode_cases':mode_cases,'compiled_rows_with_exact_cache_hit':sum(bool(x['actual_cache_hits']) for x in rows),'full_Level_or_generated_stream_verified':False}
(r/'original-level-file-routes-v52.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ['source_rows','fixture_witnesses','nonempty_range_mode_cases']},indent=2));print(json.dumps(witnesses[:1],indent=2));print(json.dumps(mode_cases,indent=2)[:5500])
