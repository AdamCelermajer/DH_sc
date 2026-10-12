"""Original native FSM getters/preset/complete Update prelude versus optimized ARM64; native state/effect/clock services explicit."""
import argparse,hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();ref=ROOT/'reference/character-native-fsm';original=ref/'native-fsm-probe.json';e=json.loads(original.read_text());assert e['validation']=='PASS' and e['original_instructions_executed']
 c=Cpu(a.library,True,{'functions':[]});view=c.data+4096;state=c.data+8192;services=c.data+12288;out=c.data+16384;text=c.data+20480;character=0xabcdef0123456789;trace=[];row={};gold=bytearray(b'NFM1'+struct.pack('<I',len(e['updates'])));comparisons=0
 def w(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
 def current():return w(state) if w(view+16) else 0xffffffff
 def callback(uc,address,size,unused):
  request=c.reg(2);kind,arg0,arg1,force,subject,payload=struct.unpack('<4IQQ',uc.mem_read(request,32));assert force==payload==0
  if kind in [0,5]:assert arg0==arg1==subject==0;trace.append(dict(service='profile_begin' if kind==0 else 'profile_end'))
  elif kind==1:assert arg0==arg1==subject==0;trace.append(dict(service='raw_engine_dt',elapsed_before=w(state+16),result=row['dt']));c.uc.mem_write(c.reg(3),struct.pack('<I',row['dt']))
  elif kind in [2,3]:
   assert subject==character;trace.append(dict(service='set_stun' if kind==2 else 'set_scare',duration=arg0,mode=arg1,payload=payload,force=force,elapsed=w(state+16),current=current()))
   result=row['stun_result' if kind==2 else 'scare_result']
   if result>=0:c.uc.mem_write(state,struct.pack('<I',result));c.uc.mem_write(view+16,struct.pack('<I',1))
  else:assert kind==4 and arg1==0;trace.append(dict(service='actual_current_virtual_update',id=arg0,character=subject==character,machine=c.reg(1)==view,elapsed=w(state+16)))
  c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
 c.imports[c.callback+32]='body_callback';c.body_callback=callback;c.uc.mem_write(services,struct.pack('<QQ',0,c.callback+32))
 for row in e['updates']:
  c.uc.mem_write(state,bytes(56));c.uc.mem_write(state,struct.pack('<iIII',row['state'],row['policy'],row['mask'],0));c.uc.mem_write(state+16,struct.pack('<I',row['elapsed']));c.uc.mem_write(view,struct.pack('<QQII',state,character,int(row['state']!=-1),0));trace.clear();assert c.invoke('dh2_character_native_fsm_update',[view,services])==1
  assert trace==row['trace'] and current()==row['final_state'] and w(state+16)==row['final_elapsed'],(row,trace)
  gold+=struct.pack('<iIIIIIiiIII',row['state'],row['mask'],row['policy'],row['elapsed'],row['dt'],0,row['stun_result'],row['scare_result'],row['final_state'],row['final_elapsed'],len(trace))
  for event in trace:
   kind=['profile_begin','raw_engine_dt','set_stun','set_scare','actual_current_virtual_update','profile_end'].index(event['service']);gold+=struct.pack('<6I',kind,event.get('duration',event.get('id',0)),event.get('mode',0),event.get('elapsed',event.get('elapsed_before',0)),event.get('current',0),event.get('result',0))
  comparisons+=1
 for row in e['getters']:
  c.uc.mem_write(state,struct.pack('<i',row['state']));c.uc.mem_write(state+16,struct.pack('<I',row['elapsed_bits']));c.uc.mem_write(view,struct.pack('<QQII',state,character,int(row['state']!=-1),0))
  for selector in range(2):
   assert c.invoke('dh2_character_native_fsm_get_integer',[out,view,selector])==1;assert w(out)==row['trace'][selector]['raw'];comparisons+=1
 for row in e['presets']:
  c.uc.mem_write(text,row['name'].encode()+b'\0');assert c.invoke('dh2_character_native_fsm_preset_state',[out,text])==1 and w(out)==row['state'];comparisons+=1
 guards=0;c.uc.mem_write(out,struct.pack('<I',0xa5a5a5a5));c.uc.mem_write(view,struct.pack('<QQII',state,character,0,0));before=bytes(c.uc.mem_read(state,56));trace.clear()
 for name,args in [('dh2_character_native_fsm_get_integer',[0,view,0]),('dh2_character_native_fsm_get_integer',[out,0,1]),('dh2_character_native_fsm_get_integer',[out,view,2]),('dh2_character_native_fsm_preset_state',[out,0]),('dh2_character_native_fsm_update',[0,services]),('dh2_character_native_fsm_update',[view,0])]:
  assert c.invoke(name,args)&0xffffffff==0xffffffff and w(out)==0xa5a5a5a5 and bytes(c.uc.mem_read(state,56))==before and not trace;guards+=1
 target=ref/'native-update-fixtures.bin';target.write_bytes(gold);sources=['character_native_fsm.hpp','character_native_fsm.cpp','tests/character_native_fsm_differential.py','tools/build_character_native_fsm_oracle.ps1'];report=dict(validation='PASS',scope=__doc__,original_sha256=e['original_sha256'],original_probe_sha256=sha(original),optimized_arm64_library_sha256=sha(a.library),gold_sha256=sha(target),comparisons=comparisons,update_cases=len(e['updates']),getter_cases=len(e['getters'])*2,preset_cases=len(e['presets']),ordered_services=sum(len(r['trace']) for r in e['updates']),native_atomic_rejections=guards,mismatches=0,source_sha256={str((ROOT/x).relative_to(REPO)):sha(ROOT/x) for x in sources},full_stun_scare_backend=False,full_current_state_body=False,packaged_APK=False)
 (ROOT/'reports/character-native-fsm-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',comparisons=comparisons,guards=guards)))
if __name__=='__main__':main()
