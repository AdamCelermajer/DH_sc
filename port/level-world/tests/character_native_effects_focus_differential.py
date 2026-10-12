"""Original stun/scare Focus and event/random-heading bodies versus optimized native ARM64; effect services explicit."""
import argparse,hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();ref=ROOT/'reference/character-native-effects';e=json.loads((ref/'focus-probe.json').read_text());assert e['validation']=='PASS';c=Cpu(a.library,True,{'functions':[]});state=c.data+0x1000;view=c.data+0x2000;effects=c.data+0x3000;services=c.data+0x4000;stun=c.data+0x5000;scare=c.data+0x6000;character=0xabcdef0123456789;trace=[];row={};draw=0
 def w(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
 def callback(uc,address,size,unused):
  nonlocal draw
  kind,x,y,z,owner,payload=struct.unpack('<4IQQ',uc.mem_read(c.reg(2),32));assert owner==character and not payload;response=0
  if kind==1:trace.append(dict(service='animation_table_id',stored=row['index'],flags=w(state+4)));response=row['index']&0xffffffff
  elif kind==3:trace.append(dict(service='constant',group='AnimStancedAnim',key='SL__LIST_IPHONE'));response=row['constant']
  elif kind==4:trace.append(dict(service='stance'));response=row['stance']&0xffffffff
  elif kind==10:trace.append(dict(service='set_animation',animator=True,animation=x))
  elif kind==11:trace.append(dict(service='is_player',character=True));response=row['player']
  elif kind==12:
   trace.append(dict(service='cancel_sneaking',character=True))
   if row['mutate']:uc.mem_write(state+44,bytes(4))
  elif kind==13:trace.append(dict(service='unpin',body=True))
  elif kind==14:response=row['draws'][draw];draw+=1;trace.append(dict(service='random',bound=x,value=response))
  elif kind==15:trace.append(dict(service='heading_point',controller=True,xyz=[x,y,z]))
  else:raise AssertionError(kind)
  uc.mem_write(c.reg(3),struct.pack('<I',response));c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
 c.imports[c.callback+32]='body_callback';c.body_callback=callback;c.uc.mem_write(services,struct.pack('<QQ',0,c.callback+32));c.uc.mem_write(stun,struct.pack('<20i',*[1000+10*i for i in range(20)]));c.uc.mem_write(scare,struct.pack('<20i',*[2000+10*i for i in range(20)]));gold=bytearray(b'NEF2'+struct.pack('<I',len(e['rows'])));ordered=0
 def encoded(event):
  name=event['service'];kind={'animation_table_id':1,'constant':3,'stance':4,'set_animation':10,'is_player':11,'cancel_sneaking':12,'unpin':13,'random':14,'heading_point':15}[name];x=y=z=f=0
  if kind==1:x=event['stored']&0xffffffff;y=event['flags']
  elif kind==10:x=event['animation'];f=int(event['animator'])
  elif kind in [11,12]:f=int(event['character'])
  elif kind==13:f=int(event['body'])
  elif kind==14:x,y=event['bound'],event['value']
  elif kind==15:x,y,z=event['xyz'];f=int(event['controller'])
  return [kind,x,y,z,f]
 for row in e['rows']:
  c.uc.mem_write(state,bytes(56));c.uc.mem_write(state+4,struct.pack('<I',0x12345678));c.uc.mem_write(state+36,struct.pack('<I',row['locked']));c.uc.mem_write(state+44,struct.pack('<I',row['present']));c.uc.mem_write(view,struct.pack('<QQII',state,character,1,0));c.uc.mem_write(effects,struct.pack('<QQQII',view,stun,scare,20,0));trace.clear();draw=0
  if not row['operation']:assert c.invoke('dh2_character_native_effect_focus',[effects,row['kind'],services])==1
  else:assert c.invoke('dh2_character_native_effect_event',[effects,row['kind'],row['event'],services])==1
  assert trace==row['trace'] and [w(state+4),w(state+36),w(state+44)]==[row['final_flags'],row['final_locked'],int(row['final_body'])],(row,trace)
  header=[row['kind'],row['operation'],row.get('event',0),row['index']&0xffffffff,row['constant'],row['stance']&0xffffffff,row['player'],row['present'],row['locked'],row['mutate'],*row['draws'],row['final_flags'],row['final_locked'],int(row['final_body']),len(trace)];gold+=struct.pack('<18I',*header)
  for event in trace:gold+=struct.pack('<5I',*encoded(event))
  ordered+=len(trace)
 before=bytes(c.uc.mem_read(state,56));trace.clear();guards=0
 for name,args in [('dh2_character_native_effect_focus',[0,0,services]),('dh2_character_native_effect_focus',[effects,2,services]),('dh2_character_native_effect_focus',[effects,0,0]),('dh2_character_native_effect_event',[0,0,0,services]),('dh2_character_native_effect_event',[effects,2,0x23,services])]:assert c.invoke(name,args)&0xffffffff==0xffffffff and bytes(c.uc.mem_read(state,56))==before and not trace;guards+=1
 target=ref/'focus-fixtures.bin';target.write_bytes(gold);paths=['character_native_effects.hpp','character_native_effects.cpp','tests/character_native_effects_focus_differential.py','tools/build_character_native_effects_oracle.ps1'];report=dict(validation='PASS',scope=__doc__,original_sha256=e['original_sha256'],original_probe_sha256=sha(ref/'focus-probe.json'),optimized_arm64_library_sha256=sha(a.library),gold_sha256=sha(target),comparisons=len(e['rows']),focus_cases=e['focus_cases'],event_cases=e['event_cases'],ordered_services=ordered,native_atomic_rejections=guards,mismatches=0,source_sha256={str((ROOT/name).relative_to(REPO)):sha(ROOT/name) for name in paths},registered_state_owner=False,RNG_provider_explicit=True,packaged_APK=False);(ROOT/'reports/character-native-effects-focus-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',comparisons=len(e['rows']),ordered_services=ordered,guards=guards)))
if __name__=='__main__':main()
