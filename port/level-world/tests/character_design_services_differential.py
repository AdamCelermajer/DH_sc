"""Original DebugSwitches load/GetSwitch missing-file instructions and OnInit constant keys versus O2 native owned-map backend."""
import argparse,hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];sys.path.insert(0,str(ROOT/'tests'));sys.path.insert(0,str(REPO/'port/script-runtime/tests'))
from character_script_selection_differential import Cpu,string
from script_function_alias_differential import Native as Base
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Native(Base):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='strlen':self.put(0,len(self.text(self.reg(0))))
  elif name=='memcmp':
   count=self.reg(2);a=bytes(uc.mem_read(self.reg(0),count));b=bytes(uc.mem_read(self.reg(1),count));self.put(0,(int(a>b)-int(a<b))&0xffffffff)
  elif name=='memmove':uc.mem_write(self.reg(0),bytes(uc.mem_read(self.reg(1),self.reg(2))));self.put(0,self.reg(0))
  elif name=='debug_open':self.trace.append(['open',self.text(self.reg(1)).hex()]);uc.mem_write(self.reg(2),struct.pack('<Q',0));self.put(0,0)
  elif name=='debug_close':raise AssertionError('Missing-file branch must not close')
  elif name=='debug_design':
   assert self.reg(1)==0;self.trace.append(['design',self.text(self.reg(2)).decode(),self.text(self.reg(3)).decode()]);uc.mem_write(self.reg(4),struct.pack('<I',self.tick&0xffffffff));self.put(0,0)
  else:return super().external(uc,address,size,unused)
  self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();ref=ROOT/'reference/character-design-services';engine=REPO/'.local-inputs/libDungeonHunter2.so';manifest=json.loads((ref/'original-functions.json').read_text());assert sha(engine)==manifest['original_sha256'];old=Cpu(engine,False,manifest);new=Native(a.library)
 def w(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 def ret(value=0):old.put(0,value&0xffffffff);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 got=(0x337898+8+w(0x337a5c))&0xffffffff;loaded=w(got+w(0x337a60));owner=w(got+w(0x337a70));app=w(got+w(0x337a68));core=old.data+0x1000;fs=old.data+0x2000;vt=old.data+0x3000;callback=old.data+0x1e000;old_input=old.data+0x4000;old_name=old.data+0x5000;old.pointer(app+0x10,core);old.pointer(core+0x34,fs);old.pointer(fs,vt);old.pointer(vt+0x94,callback);old.uc.mem_write(callback,struct.pack('<I',0xe12fff1e));switches={};heap=old.data+0x100000;original_trace=[];map_trace=[]
 def key(pointer):return string(old,w(pointer+0x14))
 def hook(uc,address,size,unused):
  nonlocal heap
  if address==0x3140ec:
   raw=string(old,old.reg(1));old.pointer(old.reg(0)+0x14,old.reg(1));old.pointer(old.reg(0)+0x10,old.reg(1)+len(raw));ret(old.reg(0))
  elif address==0x318254:ret()
  elif address==0x3369a8:
   assert old.reg(0)==owner;name=key(old.reg(1));map_trace.append(['find',name.hex()]);ret(switches[name]-0x28 if name in switches else owner)
  elif address==0x337288:
   assert old.reg(0)==owner;name=key(old.reg(1));map_trace.append(['access',name.hex()])
   if name not in switches:switches[name]=heap;heap+=16;uc.mem_write(switches[name],bytes(4))
   ret(switches[name])
  elif address==callback:
   assert old.reg(0)==fs;original_trace.append(['open',string(old,old.reg(1)).hex()]);ret(0)
 old.uc.hook_add(UC_HOOK_CODE,hook);services=new.data+0x11000;input_new=new.data+0x12000;out=new.data+0x13000;countout=out+8;nameout=out+16
 new.imports[new.callback+32]='debug_open';new.imports[new.callback+48]='debug_close';new.imports[new.callback+64]='debug_design';new.uc.mem_write(services,struct.pack('<QQQ',0xabcdef0123456789,new.callback+32,new.callback+48));new.trace=[];new.owner=0
 def snapshot():
  assert new.invoke('dh2_character_debug_snapshot',[new.owner,out,countout])==1;flag,count=struct.unpack('<I',new.uc.mem_read(out,4))[0],struct.unpack('<I',new.uc.mem_read(countout,4))[0];state=[]
  for i in range(count):
   assert new.invoke('dh2_character_debug_entry',[new.owner,i,nameout,out])==1;pointer=struct.unpack('<Q',new.uc.mem_read(nameout,8))[0];state.append((new.text(pointer).hex(),struct.unpack('<I',new.uc.mem_read(out,4))[0]))
  return flag,state
 names=[b'',b'isTracingChar_Stats',b'isTracingDebugSwitches',b'IsDeactivatingFlashMenus',b'ConnectToBetaServer',b'X'*128,b'z',b'A',b'\xff\x80key',b'case\0ignored']
 gold=bytearray(b'DSV1'+bytes(4));records=[];sessions=0
 for first,names_order in itertools.product(range(3),range(3)):
  sessions+=1;switches.clear();old.uc.mem_write(loaded,b'\0');old.invoke(0x335f94,[owner]);original_trace.clear();map_trace.clear()
  if new.owner:new.invoke('dh2_character_debug_destroy',[new.owner]);assert not new.allocations
  new.owner=new.invoke('dh2_character_debug_create',[]);assert new.owner>0xffffffff;new.trace.clear();operations=[(first==0,b'initial')]+[(False,name) for name in (names if names_order==0 else list(reversed(names)) if names_order==1 else names[::2]+names[1::2])]+[(True,b''),(True,b''),(False,b'isTracingChar_Stats')]
  for j,(is_load,name) in enumerate(operations):
   original_trace.clear();map_trace.clear();new.trace.clear();old.uc.mem_write(old_input,name+b'\0');old.pointer(old_name+0x14,old_input);old.pointer(old_name+0x10,old_input+len(name.split(b'\0')[0]));new.uc.mem_write(input_new,name+b'\0')
   if is_load:old.invoke(0x337888,[owner]);assert new.invoke('dh2_character_debug_load',[new.owner,services])==1;value=0xffffffff
   else:value=old.invoke(0x337a88,[owner,old_name]);assert new.invoke('dh2_character_debug_get',[out,new.owner,input_new,services])==1 and struct.unpack('<I',new.uc.mem_read(out,4))[0]==value
   old_state=sorted((name.hex(),old.uc.mem_read(pointer,1)[0]) for name,pointer in switches.items());flag,state=snapshot();assert flag==old.uc.mem_read(loaded,1)[0] and state==old_state and original_trace==new.trace,(j,name,old_state,state,original_trace,new.trace)
   record=dict(reset=j==0,load=is_load,name=name.hex(),value=value,loaded=flag,state=old_state,file_calls=original_trace.copy(),original_map_services=map_trace.copy());records.append(record);gold+=struct.pack('<6I',j==0,is_load,len(name),value,flag,len(old_state))+name
   for key_hex,byte in old_state:raw=bytes.fromhex(key_hex);gold+=struct.pack('<II',len(raw),byte)+raw
   gold+=struct.pack('<I',len(original_trace))
 if new.owner:new.invoke('dh2_character_debug_destroy',[new.owner]);new.owner=0;assert not new.allocations
 struct.pack_into('<I',gold,4,len(records));(ref/'debug-services-fixtures.bin').write_bytes(gold)
 # Execute the actual OnInit body, observing only its design/timer/virtual services.
 ai=old.data+0x9000;character=old.data+0xa000;character_vt=old.data+0xb000;dead_callback=callback+16;old.uc.mem_write(dead_callback,struct.pack('<I',0xe12fff1e));old.pointer(character,character_vt);old.pointer(character_vt+0x34,dead_callback);old.pointer(ai+4,character);old.pointer(ai+0x20,0);tick_trace=[];tick_values={};design=new.data+0x14000;new.uc.mem_write(design,struct.pack('<QQQ',0x123456789abcdef0,new.callback+64,0))
 def init_hook(uc,address,size,unused):
  if address==dead_callback:ret(0)
  elif address==0x4c4bdc:
   group=string(old,old.reg(1)).decode();name=string(old,old.reg(2)).decode();tick_trace.append(['design',group,name]);ret(tick_values[name])
  elif address==0x3dbe24:assert old.reg(0)==character+0x3b4 and old.reg(2)==0xffffffff and w(old.uc.reg_read(old.sp))==0;tick_trace.append(['timer',old.reg(3),old.reg(1)]);ret(60 if old.reg(3)==0x33 else 61)
 old.uc.hook_add(UC_HOOK_CODE,init_hook);ticks=[]
 for tick33,tick34 in itertools.product([0,3000,0xffffffff,0x80000000],[0,1000,0x7fffffff,0xffffffff]):
  old.pointer(ai+0x10,0xffffffff);old.pointer(ai+0x14,0xffffffff);tick_values={'AI_Tick':tick33,'DoT_Tick':tick34};tick_trace.clear();old.invoke(0x3d12b0,[ai]);assert tick_trace==[['design','CharacterDesign','AI_Tick'],['timer',0x33,tick33],['design','CharacterDesign','DoT_Tick'],['timer',0x34,tick34]]
  for event,value,key in [(0x33,tick33,'AI_Tick'),(0x34,tick34,'DoT_Tick')]:new.trace.clear();new.tick=value;assert new.invoke('dh2_character_design_tick',[out,design,event])==1 and struct.unpack('<I',new.uc.mem_read(out,4))[0]==value and new.trace==[['design','CharacterDesign',key]]
  ticks.append(dict(tick33=tick33,tick34=tick34,trace=tick_trace.copy()))
 guards=0;new.trace.clear()
 for arguments in ([0,design,0x33],[out,0,0x33],[out,design,0],[out,design,0x35]):assert new.invoke('dh2_character_design_tick',arguments)&0xffffffff==0xffffffff and not new.trace;guards+=1
 report=dict(validation='PASS',scope=__doc__,original_sha256=sha(engine),original_manifest_sha256=sha(ref/'original-functions.json'),original_helper_manifest_sha256=sha(ref/'helpers/original-functions.json'),optimized_arm64_library_sha256=sha(a.library),gold_sha256=sha(ref/'debug-services-fixtures.bin'),debug_operations=len(records),fresh_singleton_sessions=sessions,tick_original_OnInit_cases=len(ticks),tick_native_calls=2*len(ticks),native_guards=guards,mismatches=0,original_instructions_executed=True,native_owned_std_map_instructions_executed=True,original_STL_map_and_string_services_explicit=True,source_file_parser_supported=False,source_sha256={str((ROOT/x).relative_to(REPO)):sha(ROOT/x) for x in ['character_design_services.hpp','character_design_services.cpp','tests/character_design_services_differential.py','tools/build_character_design_services_oracle.ps1']},packaged_APK=False)
 (ROOT/'reports/character-design-services-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');(ref/'original-probe.json').write_text(json.dumps(dict(validation='PASS',original_sha256=sha(engine),rows=records,ticks=ticks),indent=2)+'\n');print(json.dumps({k:report[k] for k in ['validation','debug_operations','tick_original_OnInit_cases','tick_native_calls']}))
if __name__=='__main__':main()
