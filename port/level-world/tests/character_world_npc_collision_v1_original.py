"""Whole original POCharacter collision routing, with explicit synchronous Debug,
CString, Handle/Character conversion and RaiseEvent service fixtures."""
from pathlib import Path
import sys,struct,itertools,json,hashlib
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
root=Path(__file__).resolve().parents[3];sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
w=lambda a:struct.unpack('<I',c.uc.mem_read(a,4))[0]
put=lambda a,v:c.uc.mem_write(a,struct.pack('<I',v&0xffffffff))
physical=c.data+0x1000;peer=c.data+0x2000;owner=c.data+0x3000;other=c.data+0x5000;vtable=c.data+0x7000
events=[];tracing=convert=0;handle_owner=0
def hook(uc,pc,size,user):
 global handle_owner
 if pc==0x337888:events.append(['debug_load'])
 elif pc==0x46fcd8:
  end=c.reg(1);name=bytes(c.uc.mem_read(end-25,25)).rstrip(b'\0').decode()
  assert name=='isTracingPlayersCollision',name
  events.append(['string_construct',name])
 elif pc==0x337a88:events.append(['debug_query']);c.put(0,tracing)
 elif pc==0x3139ac:events.append(['string_destroy'])
 elif pc==0x33dd2c:handle_owner=c.reg(1);events.append(['handle',handle_owner])
 elif pc==0x33ff54:events.append(['character']);c.put(0,handle_owner if convert else 0)
 elif pc==0x3a4d5c:events.append(['raise',c.reg(0),c.reg(1),c.reg(2)])
 elif pc in (0x46fe10,0x46ff50,0x470058):
  events.append(['player']);c.put(0,0);uc.reg_write(c.pc,pc+4);return
 else:return
 uc.reg_write(c.pc,uc.reg_read(c.lr))
c.uc.hook_add(UC_HOOK_CODE,hook)
records=[]
for source,event in [(0x46fe68,0x37),(0x46ff6c,0x39),(0x46fd28,0x3b)]:
 for persist,own_present,other_present,tracing,convert in itertools.product((0,1),repeat=5):
  events.clear();put(physical+8,owner if own_present else 0);put(peer+8,other if other_present else 0);put(owner,vtable)
  c.invoke(source,[physical,peer,0,persist])
  names=[e[0] for e in events]
  if not own_present or not other_present:assert not events
  else:
   assert names.count('debug_load')==names.count('debug_query')==names.count('string_construct')==names.count('string_destroy')==1
   assert names.count('handle')==names.count('character')==1
   assert names.index('handle')<names.index('debug_load') if event==0x39 else names.index('handle')>names.index('debug_load')
   assert names.count('player')==tracing
   raised=[e for e in events if e[0]=='raise'];assert len(raised)==convert
   if convert:assert raised[0]==['raise',owner,event+(0 if persist else 1),other]
  records.append([source,persist,own_present,other_present,tracing,convert,events.copy()])
c.invoke(0x46fb14,[physical,peer,0,1]);assert len(events)==len(records[-1][-1])
# AISExternal constructor stores: execute genuine constructor with only its
# CharAIScript base ctor isolated as a declared dependency.
from unicorn.arm_const import UC_ARM_REG_R4
def ctor_hook(uc,pc,size,user):
 if pc==0x3d8fb0:uc.reg_write(c.pc,uc.reg_read(c.lr))
c.uc.hook_add(UC_HOOK_CODE,ctor_hook)
ctor_cases=0
for entry in (0x3dd0e4,0x3dd128):
 for poison in (0,0x33,0xaa,0xff):
  c.uc.mem_write(owner,bytes([poison])*0x100);c.invoke(entry,[owner,0])
  assert w(owner+0xb8)==w(owner+0xbc)==w(owner+0xc0)==0;ctor_cases+=1
report={'validation':'PASS','whole_original_POCharacter_callback_cases':len(records),'whole_AISExternal_ctor_with_declared_base_service':ctor_cases,'same_other_owner_RaiseEvent_payload':True,'begin_end_Debug_before_handle':True,'persist_handle_before_Debug':True,'source_collision_debug_key':'isTracingPlayersCollision','source_AIS_fields':'collision_ms+bc,last_collision_frame+c0 both zero; paused borrowed Character+3e0=CharAI+18','POCharacter_Result':'complete bx lr body, no AIS forwarding','native_differential':False,'original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest()}
(root/'port/level-world/reports/character-world-npc-collision-v1-original-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report))

