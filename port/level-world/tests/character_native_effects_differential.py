"""Original effect producers/simple state bodies versus optimized ARM64; registered transitions and gameplay services explicit."""
import argparse,hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();ref=ROOT/'reference/character-native-effects';producer=json.loads((ref/'effect-probe.json').read_text());body=json.loads((ref/'body-probe.json').read_text());assert producer['validation']==body['validation']=='PASS'
 c=Cpu(a.library,True,{'functions':[]});state=c.data+0x1000;view=c.data+0x2000;effects=c.data+0x3000;services=c.data+0x4000;stun=c.data+0x5000;scare=c.data+0x6000;character=0xabcdef0123456789;payload=0xa0123456fedcba98;trace=[];row={};operation=0
 def w(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
 def callback(uc,address,size,unused):
  service,x,y,z,owner,value=struct.unpack('<4IQQ',uc.mem_read(c.reg(2),32));assert owner==character;response=0
  if service==0:trace.append(dict(service='boss_query',character=True));response=row['boss']*4
  elif service==1:trace.append(dict(service='animation_table_id',stored=row['index']));response=row['index']&0xffffffff
  elif service==2:
   assert not value;trace.append(dict(service='timer_start',character=True,duration=x,repeat=y,event=z,payload=0,mask_before=w(state+8)));response=0xffffffff
   if row['mutate']:uc.mem_write(state+8,struct.pack('<I',0x40000000))
  elif service==3:trace.append(dict(service='constant',group='AnimStancedAnim',key='SL__LIST_IPHONE',animation_before=struct.unpack('<i',uc.mem_read(state+48,4))[0]));response=row['constant']
  elif service==4:trace.append(dict(service='stance',character=True));response=row['stance']&0xffffffff
  elif service in [5,6]:
   assert value==(payload if row['_payload'] else 0)
   if operation:trace.append(dict(service='force_state',state=x,event=y,payload=0,idle_suppressed=w(state+24)))
   else:
    trace.append(dict(service='force_state' if service==5 else 'raise_state_event',**({'state':x,'event':y} if service==5 else {'event':x}),payload=int(bool(value)),animation=w(state+48),mask=w(state+8)))
    if row['mutate']:uc.mem_write(state+4,struct.pack('<I',0x100))
  elif service==7:assert x==y==z==value==0;trace.append(dict(service='stop_loop',animator=True,mode=0))
  elif service==8:assert x==y==z==value==0;trace.append(dict(service='pin',body=True,controller_locked=w(state+36)))
  elif service==9:
   assert x==y==z==value==0;trace.append(dict(service='heading_object',controller=True,payload=0))
   if row['mutate']:uc.mem_write(state+44,bytes(4))
  else:raise AssertionError(service)
  uc.mem_write(c.reg(3),struct.pack('<I',response));c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
 c.imports[c.callback+32]='body_callback';c.body_callback=callback;c.uc.mem_write(services,struct.pack('<QQ',0,c.callback+32));c.uc.mem_write(stun,struct.pack('<20i',*[1000+10*i for i in range(20)]));c.uc.mem_write(scare,struct.pack('<20i',*[2000+10*i for i in range(20)]));gold=bytearray(b'NEF1'+struct.pack('<I',len(producer['rows'])+len(producer['table_gates'])+len(body['rows'])));ordered=0;comparisons=0
 def encoded(event):
  name=event['service'];kind=['boss_query','animation_table_id','timer_start','constant','stance','force_state','raise_state_event','stop_loop','pin','heading_object'].index(name);a=b=d=v=f=g=0
  if kind==0:f=int(event['character'])
  elif kind==1:a=event['stored']&0xffffffff
  elif kind==2:a,b,d=event['duration'],event['repeat'],event['event'];f,g=event['mask_before'],int(event['character'])
  elif kind==3:f=event['animation_before']&0xffffffff
  elif kind==4:f=int(event['character'])
  elif kind in [5,6]:a=event['state'] if kind==5 else event['event'];b=event['event'] if kind==5 else 0;v=int(bool(event['payload']));f=event.get('animation',event.get('idle_suppressed',0));g=event.get('mask',0)
  elif kind==7:f=int(event['animator'])
  elif kind==8:f,g=event['controller_locked'],int(event['body'])
  elif kind==9:f=int(event['controller'])
  return [kind,a,b,d,v,f,g]
 records=[]
 for source in producer['rows']+producer['table_gates']:
  row=dict(source);gate='accepted' in row;operation=0
  if gate:row.update(boss=0,mask=0,constant=0,mode=0,force=0,mutate=0,stance=0,duration=123)
  flags=0x11111111 if gate else 0x12345000;animation=0x22222222 if gate else 0x87654321;expected=[dict(e,payload=int(bool(e['payload']))) if e['service'] in ['force_state','raise_state_event'] else e for e in row['trace']]
  header=[row['kind'],0,row['boss'],row['index']&0xffffffff,row['count'],row['mask'],row['constant'],row['mode'],row['force'],row['mutate'],row['stance']&0xffffffff,row['duration'],17,19,1,flags,animation,row['final_flags'],row['final_animation'],row['final_mask'],17,19,1,len(expected),int(not gate)];records.append((row,header,expected))
 for source in body['rows']:
  row=dict(source);row.update(boss=0,index=0,count=20,constant=0,mode=0,force=0,stance=0,duration=0);operation=row['operation']+1;header=[row['kind'],operation,0,0,20,row['mask'],0,0,0,row['mutate'],0,0,row['locked'],row['idle'],row['present'],0x12345000,0x87654321,0x12345000,0x87654321,row['mask'],row['final_locked'],row['final_idle'],int(row['final_body']),len(row['trace']),0];records.append((row,header,row['trace']))
 for row,header,expected in records:
  operation=header[1];row['_payload']=header[24];c.uc.mem_write(state,bytes(56));c.uc.mem_write(state,struct.pack('<iII',9 if not row['kind'] else 8,header[15],header[5]));c.uc.mem_write(state+24,struct.pack('<I',header[13]));c.uc.mem_write(state+36,struct.pack('<I',header[12]));c.uc.mem_write(state+44,struct.pack('<II',header[14],header[16]));c.uc.mem_write(view,struct.pack('<QQII',state,character,1,0));c.uc.mem_write(effects,struct.pack('<QQQII',view,stun,scare,header[4],0));trace.clear()
  if not operation:result=c.invoke('dh2_character_native_effect_set',[effects,header[0],header[11],header[7],payload if header[24] else 0,header[8],services]);assert result==int(not row['boss'] and row.get('accepted',True))
  else:assert c.invoke('dh2_character_native_effect_body',[effects,header[0],operation-1,services])==1
  assert trace==expected,(row,trace,expected)
  assert [w(state+4),w(state+48),w(state+8),w(state+36),w(state+24),w(state+44)]==header[17:23]
  gold+=struct.pack('<25I',*header)
  for event in expected:gold+=struct.pack('<7I',*encoded(event))
  ordered+=len(expected);comparisons+=1
 guards=0;before=bytes(c.uc.mem_read(state,56));trace.clear()
 for fn,args in [('dh2_character_native_effect_set',[0,0,1,0,0,0,services]),('dh2_character_native_effect_set',[effects,2,1,0,0,0,services]),('dh2_character_native_effect_set',[effects,0,1,2,0,0,services]),('dh2_character_native_effect_set',[effects,0,1,0,0,2,services]),('dh2_character_native_effect_set',[effects,0,1,0,0,0,0]),('dh2_character_native_effect_body',[effects,0,2,services])]:assert c.invoke(fn,args)&0xffffffff==0xffffffff and bytes(c.uc.mem_read(state,56))==before and not trace;guards+=1
 target=ref/'effect-fixtures.bin';target.write_bytes(gold);sources=['character_native_effects.hpp','character_native_effects.cpp','tests/character_native_effects_differential.py','tools/build_character_native_effects_oracle.ps1'];report=dict(validation='PASS',scope=__doc__,original_sha256=producer['original_sha256'],original_probe_sha256={name:sha(ref/name) for name in ['effect-probe.json','body-probe.json']},optimized_arm64_library_sha256=sha(a.library),gold_sha256=sha(target),comparisons=comparisons,producer_cases=len(producer['rows']),table_gate_cases=len(producer['table_gates']),simple_body_cases=len(body['rows']),ordered_services=ordered,native_atomic_rejections=guards,mismatches=0,source_sha256={str((ROOT/name).relative_to(REPO)):sha(ROOT/name) for name in sources},native_registered_state_owner=False,focus_random_heading_and_init_implemented=False,packaged_APK=False)
 (ROOT/'reports/character-native-effects-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',comparisons=comparisons,ordered_services=ordered,guards=guards)))
if __name__=='__main__':main()
