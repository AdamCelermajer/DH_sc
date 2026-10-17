"""WorldMap two-section decoder plus actual-cache saved-default composition."""
from __future__ import annotations
import argparse,hashlib,importlib.util,json,os,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_SP,UC_ARM_REG_LR,UC_ARM_REG_PC
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put
from run_player_save_level_states_v1 import Original as SavedOriginal,SAVE,COUNTS,MEMBERS,TABLES,STRIDES,DEFAULTS,source_offset
PIN=MODULE/'reference/world-map-tables/original-functions.json';OWNER=0x10001000;STREAM=0x10002000;STOP=0x30000000
class RowOriginal:
 def __init__(self,path,wire):
  data,names,_=image(path);self.pins=json.loads(PIN.read_text());self.ranges=[]
  for pin in self.pins['functions']:
   address=int(pin['elf_address'],0);symbol=names[pin['original_symbol']];size=pin['size']
   assert (symbol['st_value'],symbol['st_size'])==(address,size)
   assert hashlib.sha256(data[address:address+size]).hexdigest()==pin['sha256'];self.ranges.append((address,size))
  self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);self.u.mem_map(0,len(data));self.u.mem_write(0,data)
  for base,size in [(0x10000000,0x40000),(0x20000000,0x10000),(STOP,0x1000)]:self.u.mem_map(base,size)
  self.words=set();self.calls={};self.wire=wire;self.cursor=0;self.heap=0x10010000;self.u.hook_add(UC_HOOK_CODE,self.hook)
 def take(self,count):
  assert 0<=count<=len(self.wire)-self.cursor
  data=self.wire[self.cursor:self.cursor+count];self.cursor+=count;return data
 def hook(self,u,address,size,context):
  if address==STOP:u.emu_stop();return
  r0=u.reg_read(UC_ARM_REG_R0);r1=u.reg_read(UC_ARM_REG_R1)
  if address in [0x459090,0x3df1a0]:
   assert r0==STREAM;u.mem_write(r1,self.take(4))
  elif address==0x31056c:
   assert r1==1 and r0<=16384;u.reg_write(UC_ARM_REG_R0,self.heap);u.mem_write(self.heap,bytes(r0 or 4));self.heap+=(r0+31)&~15
  elif address==0x310440:pass
  else:
   assert any(a<=address<a+n for a,n in self.ranges),f'unscoped WorldMap reader {address:#x}'
   self.words.add(address);return
  self.calls[hex(address)]=self.calls.get(hex(address),0)+1;u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def row(self,function,name):
  self.u.mem_write(OWNER,bytes(20));self.heap=0x10010000
  self.u.reg_write(UC_ARM_REG_R0,OWNER);self.u.reg_write(UC_ARM_REG_R1,STREAM);self.u.reg_write(UC_ARM_REG_SP,0x20008000);self.u.reg_write(UC_ARM_REG_LR,STOP)
  self.u.emu_start(function,STOP+4,count=10000);assert self.u.reg_read(UC_ARM_REG_PC)==STOP
  signed=lambda offset:struct.unpack('<i',self.u.mem_read(OWNER+offset,4))[0]
  if function==0x4eb5a8:
   count=get(self.u,OWNER+12);pointer=get(self.u,OWNER+16)
   return dict(name=name,name_id=signed(4),state=signed(8),location_levels=list(struct.unpack('<'+'i'*count,self.u.mem_read(pointer,count*4))))
  return dict(name=name,on_state=signed(4),quest_id=signed(8))
def names_sections(raw):
 cursor=0;sections=[]
 while cursor<len(raw):
  count=struct.unpack_from('<I',raw,cursor)[0];cursor+=4;rows=[]
  for _ in range(count):
   length=struct.unpack_from('<I',raw,cursor)[0];cursor+=4;rows.append(raw[cursor:cursor+length].decode('utf-8'));cursor+=length
  sections.append(rows)
 assert cursor==len(raw);return sections
def level_defaults(records,names):
 path=ROOT/'port/level-catalogue/catalogue.py';spec=importlib.util.spec_from_file_location('world_map_composition_catalogue',path);module=importlib.util.module_from_spec(spec);sys.modules[spec.name]=module;spec.loader.exec_module(module)
 return [row.level_state for row in module.decode_catalogue(records,names).levels]
def compare(original,wire,names,host,levels):
 cpu=RowOriginal(original,wire);location_names,locker_names=names_sections(names);rows={}
 for key,keys,function in [('locations',location_names,0x4eb5a8),('lockers',locker_names,0x4eb4d8)]:
  count=struct.unpack('<I',cpu.take(4))[0];assert count==len(keys)
  rows[key]=[cpu.row(function,name) for name in keys]
 assert cpu.cursor==len(wire) and rows==host['table']
 # Execute the original complete Save initializer with actual decoded cache
 # globals, then compare all six arrays in the sole native Save composition.
 saved=SavedOriginal(original);saved.execute([0,3,3,0,0,0]);saved.u.mem_write(SAVE,bytes(0x198))
 saved.events=[];saved.allocations={};saved.next=16;saved.heap=0x10018000
 defaults=[levels,[row['state'] for row in rows['locations']]]
 for table,words in enumerate(defaults):
  put(saved.u,COUNTS[table],len(words));put(saved.u,MEMBERS[table],TABLES[table])
  for index,word in enumerate(words):put(saved.u,TABLES[table]+index*STRIDES[table]+DEFAULTS[table],word)
 saved.call();arrays=[]
 for slot in range(6):
  pointer=get(saved.u,SAVE+source_offset(slot));count=saved.allocations[pointer][1]
  arrays.append(list(struct.unpack('<'+'i'*count,saved.u.mem_read(pointer,count*4))))
 assert arrays==host['saved_defaults'] and host['level_defaults']==levels
 return dict(original_arm_record_comparisons=len(location_names)+len(locker_names),original_record_words=len(cpu.words),declared_row_provider_calls=cpu.calls,actual_save_initializer_words=len(saved.words),saved_default_word_comparisons=sum(map(len,arrays)),mismatches=0)
def main():
 p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--library',type=Path);a=p.parse_args()
 a.output.mkdir(parents=True,exist_ok=True);files=[a.cache/x for x in ['worldmap_pyarray.bin','worldmap_pyarraynames.bin','worldmap_pystructnames.bin','levels_pyarray.bin','levels_pyarraynames.bin','levels_pystructnames.bin']]
 sources=['world_map_tables.cpp','level_tables.cpp','player_save_level_states_v1.cpp'];test=MODULE/'tests/world_map_tables_host.cpp';exe=a.output/'world_map_tables_host.exe'
 command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror',str(test)];command += [str(a.library)] if a.library else [str(MODULE/x) for x in sources]
 subprocess.run(command+['-o',str(exe)],check=True);env=os.environ.copy();dlls=[]
 if a.library:
  dlls=sorted(a.library.parent.parent.rglob('*.dll'));env['PATH']=os.pathsep.join([*(str(x.parent) for x in dlls),env['PATH']])
 host=json.loads(subprocess.check_output([str(exe),*map(str,files)],env=env,text=True));defaults=level_defaults(files[3].read_bytes(),files[4].read_bytes());proof=compare(a.original,files[0].read_bytes(),files[1].read_bytes(),host,defaults)
 evidence=[*sources,'world_map_tables.hpp','player_savegame_v1.hpp','player_save_level_states_v1.hpp','tests/world_map_tables_host.cpp','tests/run_world_map_tables.py','tests/run_player_save_level_states_v1.py','reference/world-map-tables/original-functions.json']
 report=dict(validation='PASS',host=host,original=proof,implementation='selected library' if a.library else 'direct translation units',cache_sha256={x.name:hashlib.sha256(x.read_bytes()).hexdigest() for x in files},source_sha256={x:hashlib.sha256((MODULE/x).read_bytes()).hexdigest() for x in evidence},scope=json.loads(PIN.read_text())['scope'])
 if a.library:
  build=next(parent for parent in a.library.parents if (parent/'compile_commands.json').is_file());commands=json.loads((build/'compile_commands.json').read_text());paths={(MODULE/x).resolve() for x in sources}
  report['selected_commands']=[c for c in commands if Path(c['file']).resolve() in paths];assert len(report['selected_commands'])==len(sources)
  report['binary_sha256']={x.name:hashlib.sha256(x.read_bytes()).hexdigest() for x in [exe,*dlls]}
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items() if k not in ['source_sha256','cache_sha256','selected_commands','binary_sha256','host']}))
if __name__=='__main__':main()
