"""Owned frame directly executed as optimized ARM64; original complete Update
1536-case trace. External effect mutations and unbounded bodies are explicit
fixtures. Bounded bodies really execute with no-effect body services; their
exact requests/state are separately proved by the 906-case bounded audit.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
R=Path(__file__).resolve().parents[1];REPO=R.parents[1];sys.path.insert(0,str(R/'tests'))
from character_state_differential import Cpu as Base
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
class Cpu(Base):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name in ('frame_outer','frame_other','frame_body'):return self.service(name,uc)
  return super().external(uc,address,size,unused)
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 probe=R/'reference/character-native-fsm/native-fsm-probe.json';gold=json.loads(probe.read_text());assert gold['validation']=='PASS' and gold['original_instructions_executed']
 plan=R/'reference/character-monster-state-ownership/registration-plan.json';data=json.loads(plan.read_text())
 c=Cpu(a.library,True,{'functions':[]});state=c.data+4096;fsm=c.data+8192;machine=c.data+12288;infos=c.data+16384;facts=c.data+20480;bodies=c.data+24576;context=c.data+28672;character=0xa123456789abcdef
 # Canonical original vtable dispatch metadata from the executed source factory
 # capture, not invented modern state numbering or ARM object overlays.
 rows=data['states']
 trace=[];current_row=None;other_count=0;body_count=0
 def w(x):return struct.unpack('<I',c.uc.mem_read(x,4))[0]
 def current():return w(state) if w(fsm+16) else 0xffffffff
 def service(name,uc):
  nonlocal other_count,body_count
  if name=='frame_body':body_count+=1;uc.reg_write(c.pc,uc.reg_read(c.lr));return
  if name=='frame_other':
   fn,id_,subject,elapsed,zero=struct.unpack('<IiQII',uc.mem_read(c.reg(2),24));assert not zero and subject==character and id_==current() and elapsed==w(state+16);assert fn==rows[id_]['on_update'];other_count+=1
  else:
   kind,arg0,arg1,force,subject,payload=struct.unpack('<4IQQ',uc.mem_read(c.reg(2),32));assert force==payload==0
   if kind in (0,5):assert not(arg0 or arg1 or subject);trace.append(dict(service='profile_begin' if kind==0 else 'profile_end'))
   elif kind==1:assert not(arg0 or arg1 or subject);trace.append(dict(service='raw_engine_dt',elapsed_before=w(state+16),result=current_row['dt']));uc.mem_write(c.reg(3),struct.pack('<I',current_row['dt']))
   else:
    assert kind in (2,3) and subject==character;trace.append(dict(service='set_stun' if kind==2 else 'set_scare',duration=arg0,mode=arg1,payload=payload,force=force,elapsed=w(state+16),current=current()))
    next_=current_row['stun_result' if kind==2 else 'scare_result']
    if next_>=0:uc.mem_write(state,struct.pack('<i',next_));uc.mem_write(fsm+16,struct.pack('<I',1));uc.mem_write(machine+20,struct.pack('<i',next_))
  c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
 c.service=service
 for i,name in enumerate(('frame_outer','frame_other','frame_body')):c.imports[c.callback+32+i*16]=name
 for row in rows:
  id_=row['id'];raw=struct.pack('<i5IQII',id_,row['singleton'],row['on_focus'],row['on_blur'],row['on_update'],row['on_event'],0,0,0);assert len(raw)==40;c.uc.mem_write(infos+id_*40,raw)
 c.uc.mem_write(facts,bytes(96));c.uc.mem_write(bodies,struct.pack('<QQ',0,c.callback+64));c.uc.mem_write(context,struct.pack('<7Q',machine,facts,bodies,0,c.callback+32,0,c.callback+48))
 for current_row in gold['updates']:
  row=current_row;trace.clear();c.uc.mem_write(state,bytes(56));c.uc.mem_write(state,struct.pack('<i4I',row['state'],row['policy'],row['mask'],0,row['elapsed']));c.uc.mem_write(state+48,struct.pack('<ii',-1,-1));c.uc.mem_write(fsm,struct.pack('<QQII',state,character,int(row['state']!=-1),0));c.uc.mem_write(machine,struct.pack('<QQIiQQ',fsm,infos,20,row['state'],0,0))
  assert c.invoke('dh2_character_state_owner_frame',[context])==1
  expected=[x for x in row['trace'] if x['service']!='actual_current_virtual_update'];assert trace==expected and current()==row['final_state'] and w(state+16)==row['final_elapsed'],(row,trace)
 paths=('character_state_owner_frame.cpp','character_state_owner_frame.hpp','character_native_fsm.cpp','character_native_fsm.hpp','character_state.cpp','character_state.hpp','character_state_owner.hpp','tests/character_state_owner_frame_outer_differential.py')
 report=dict(validation='PASS',scope=__doc__,comparisons=len(gold['updates']),mismatches=0,original_sha256=gold['original_sha256'],original_probe=dict(path=str(probe.relative_to(REPO)),sha256=sha(probe)),source_registry=dict(path=str(plan.relative_to(REPO)),sha256=sha(plan)),library_sha256=sha(a.library),source_sha256={'port/level-world/'+x:sha(R/x) for x in paths},other_update_metadata_deliveries=other_count,bounded_body_fixture_deliveries=body_count,virtual_dispatch_marker_not_part_of_public_adapter_trace=True,full_effect_backends=False,packaged_APK=False)
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
