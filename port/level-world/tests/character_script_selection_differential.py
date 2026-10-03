"""Original script-choice instructions versus optimized native ARM64.

AIS construction/ownership and logging are explicit synchronous services;
SetScriptByName/StepCreateScript selection and post-service field stores execute.
"""
import argparse, hashlib, itertools, json, struct, sys
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from body_transform_differential import Cpu as Base

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def string(cpu,address):
 out=bytearray()
 for i in range(4097):
  byte=cpu.uc.mem_read(address+i,1)[0]
  if not byte:return bytes(out)
  out.append(byte)
 raise AssertionError('Unterminated script name')
class Cpu(Base):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name in ('strcmp','strncmp'):
   a,b=string(self,self.reg(0)),string(self,self.reg(1))
   if name=='strncmp':a,b=a[:self.reg(2)],b[:self.reg(2)]
   self.put(0,(int(a>b)-int(a<b))&0xffffffff)
   self.import_calls[name]=self.import_calls.get(name,0)+1
   uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,unused)

def main():
 parser=argparse.ArgumentParser()
 for key in ('engine','library','output'):parser.add_argument('--'+key,type=Path,required=True)
 args=parser.parse_args();args.output.mkdir(parents=True,exist_ok=True)
 if (args.output/'differential.json').exists():raise RuntimeError('Refusing to overwrite script-choice evidence')
 assert sha(args.engine)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
 constructors={0x3cce14:0,0x3ccbe4:1,0x3ccfe4:2,0x3cccf8:3,0x3ccaf4:4,0x3cd11c:5}
 addresses=(0x3ceeb0,0x3cf04c,*constructors)
 functions,assembly=[],[]
 with args.engine.open('rb') as stream:
  elf=ELFFile(stream);symbols=list(elf.get_section_by_name('.symtab').iter_symbols())
  segments=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
  for address in addresses:
   symbol=next(s for s in symbols if s['st_value']==address and s['st_size'])
   segment=next(s for s in segments if s['p_vaddr']<=address<s['p_vaddr']+s['p_filesz'])
   stream.seek(segment['p_offset']+address-segment['p_vaddr']);raw=stream.read(symbol['st_size'])
   functions.append(dict(original_symbol=symbol.name,elf_address=hex(address),size=len(raw),sha256=hashlib.sha256(raw).hexdigest()))
   assembly.append('\n# '+symbol.name)
   assembly.extend(f'{i.address:08x}: {i.mnemonic:8} {i.op_str}' for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw,address))
 manifest=dict(original_sha256=sha(args.engine),functions=functions)
 (args.output/'original-functions.json').write_text(json.dumps(manifest,indent=2)+'\n')
 (args.output/'original-functions.asm').write_text('\n'.join(assembly)+'\n')
 old=Cpu(args.engine,False,manifest);new=Cpu(args.library,True,{'functions':[]})
 character=old.data+0x1000;ai=character+0x3c8;table=old.data+0x5000;table_slot=old.data+0x6000
 old_name=old.data+0x7000;old_owner=old.data+0x9000
 ns=new.data+0x1000;nf=new.data+0x2000;nv=new.data+0x3000;new_name=new.data+0x4000;new_owner=new.data+0x6000
 context=0xabcdef0123456789;old_external=0x12345678;new_external=0xa123456789abcdef
 mutation=0;calls=[[],[]]
 def word(address):return struct.unpack('<I',old.uc.mem_read(address,4))[0]
 def returned(value=0):old.put(0,value&0xffffffff);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def old_token(value):
  assert value in (0,old_external,old_name),hex(value)
  return (0,old_external,old_name).index(value)
 def new_token(value):
  assert value in (0,new_external,new_name),hex(value)
  return (0,new_external,new_name).index(value)
 def old_hook(uc,address,size,unused):
  if address in constructors:
   assert old.reg(0)==ai
   calls[0].append((constructors[address],old_token(word(ai+0x30)),uc.mem_read(ai+0x2c,1)[0]))
   if mutation:
    old.pointer(ai+0x30,old_external if mutation==1 else 0)
    uc.mem_write(ai+0x2c,bytes((1-uc.mem_read(ai+0x2c,1)[0],)))
   returned()
  elif address==0x3a2fec:returned(0)
  elif address in (0x337888,0x337a88):returned()
  elif address==0x3140ec:
   # Logger's temporary std::string has no content in this caller fixture.
   uc.mem_write(old.reg(0),bytes(24));returned()
 def native_callback(uc,address,size,unused):
  assert (new.reg(0),new.reg(1))==(context,ns)
  external,scripted,reserved=struct.unpack('<QII',uc.mem_read(ns,16));assert reserved==0
  calls[1].append((new.reg(2),new_token(external),scripted))
  if mutation:uc.mem_write(ns,struct.pack('<QII',new_external if mutation==1 else 0,1-scripted,0))
  uc.reg_write(new.pc,uc.reg_read(new.lr))
 old.uc.hook_add(UC_HOOK_CODE,old_hook)
 new.imports[new.callback+32]='body_callback';new.body_callback=native_callback
 new.uc.mem_write(nv,struct.pack('<QQ',context,new.callback+32))
 base=(0x3cf05c+8+word(0x3cf1d4))&0xffffffff
 old.pointer(base+word(0x3cf1dc),table_slot);old.pointer(table_slot,table)
 names=(b'',b'_',b'__',b'__monster__',b'__player__',b'__faery__',b'__npc__',b'__unknown__',b'__Player__',b'__player',b'__player__extra',b'player',b'Player',b'ais/player.py',b'__npc__\0ignored')
 owners=(b'Player',b'player',b'Prince',b'')
 records=[];corpus=bytearray(b'SSC1'+bytes(4))
 for operation,name,owner,prior_external,prior_scripted,mutation in itertools.product(range(3),names,owners,(0,1),(0,1),range(3)):
  if operation!=2 and owner!=b'Player':continue
  old.uc.mem_write(character,bytes(0x1000));old.pointer(ai+4,character)
  old.pointer(ai+0x30,old_external if prior_external else 0);old.uc.mem_write(ai+0x2c,bytes((prior_scripted,)))
  old.uc.mem_write(old_name,name+b'\0');old.uc.mem_write(old_owner,owner+b'\0');old.pointer(character+0x5c,old_owner)
  old.uc.mem_write(table,bytes(68));old.pointer(table+0x28,0 if operation==2 else len(name)+1);old.pointer(table+0x2c,old_name)
  old.uc.mem_write(old.stack+0xd000,bytes(0x1000))
  new.uc.mem_write(ns,struct.pack('<QII',new_external if prior_external else 0,prior_scripted,0))
  new.uc.mem_write(new_name,name+b'\0');new.uc.mem_write(new_owner,owner+b'\0')
  new.uc.mem_write(nf,struct.pack('<IIQQ',0 if operation==2 else len(name)+1,0,new_name,new_owner))
  calls[0].clear();calls[1].clear()
  if operation==0:
   old.invoke(0x3ceeb0,[ai,old_name]);actual=new.invoke('dh2_character_script_select',[ns,new_name,nv])
  else:
   old.invoke(0x3cf04c,[ai]);actual=new.invoke('dh2_character_script_create_step',[ns,nf,nv])
  final_old=(old_token(word(ai+0x30)),old.uc.mem_read(ai+0x2c,1)[0])
  external,flag,reserved=struct.unpack('<QII',new.uc.mem_read(ns,16));final_new=(new_token(external),flag)
  assert actual==1 and final_old==final_new and calls[0]==calls[1],(operation,name,owner,prior_external,prior_scripted,mutation,final_old,final_new,calls)
  records.append(dict(operation=operation,name=name.hex(),owner=owner.hex(),prior_external=prior_external,prior_scripted=prior_scripted,mutation=mutation,final=final_old,calls=calls[0].copy()))
  corpus+=struct.pack('<8I',operation,prior_external,prior_scripted,mutation,*final_old,len(name),len(owner))+name+owner
  corpus+=struct.pack('<I',len(calls[0]))+b''.join(struct.pack('<3I',*call) for call in calls[0])
 struct.pack_into('<I',corpus,4,len(records));(args.output/'selection-reference.bin').write_bytes(corpus)
 # Native guards reject before service delivery and preserve logical state.
 calls[1].clear();new.uc.mem_write(ns,struct.pack('<QII',new_external,1,0))
 rejected=0
 for symbol,arguments in [('dh2_character_script_select',[0,new_name,nv]),('dh2_character_script_select',[ns,0,nv]),('dh2_character_script_select',[ns,new_name,0]),('dh2_character_script_create_step',[ns,0,nv])]:
  assert new.invoke(symbol,arguments)&0xffffffff==0xffffffff and not calls[1];rejected+=1
 assert bytes(new.uc.mem_read(ns,16))==struct.pack('<QII',new_external,1,0)
 sources=['character_script_selection.cpp','character_script_selection.hpp','tests/character_script_selection_differential.py']
 report=dict(validation='PASS',original_sha256=sha(args.engine),original_manifest_sha256=sha(args.output/'original-functions.json'),library_sha256=sha(args.library),source_sha256={p:sha(ROOT/p) for p in sources},reference_sha256=hashlib.sha256(corpus).hexdigest(),original_instructions_executed=True,compiled_arm64_instructions_executed=True,comparisons=len(records),ordered_factory_callbacks=sum(len(r['calls']) for r in records),native_rejections=rejected,mismatches=0,pointer_identities_above_4gib=True,original_imports=old.import_calls,scope=__doc__)
 (args.output/'differential.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps(report))

if __name__=='__main__':main()
