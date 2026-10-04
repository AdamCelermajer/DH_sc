"""Original loot power/quantity/value/coordinator versus O2 native production.

Actual serialized cache readers, source RNG and original Power append execute.
Debug switches and localization/parseEx use declared fixture providers. This
isolates generation/valuation; it does not claim full AddLoot or real locale.
Original code executes only in this offline proof, never in the game.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
R=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(R/'port/game-data/tests'))
from item_power_instance_v5_differential import OriginalPower
from item_presentation_v5_differential import NativePresentation,block,sha
from player_savegame_v1_original import W
from unicorn import UC_HOOK_CODE
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
from elftools.elf.elffile import ELFFile

def signed(x):return struct.unpack('<i',W(x))[0]
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def floating(x):return struct.unpack('<f',W(x))[0]

class OriginalLoot(OriginalPower):
 def __init__(self):
  super().__init__();self.looting=False;self.valuing=False;self.calls=[]
  root=R/'.local-inputs/player-loot-v7/cache'
  self.blob=(root/'item_powers_monopoly_pyarray.bin').read_bytes();self.cursor=0
  self.invoke(0x4baa30,[self.stream],budget=20000000);assert self.cursor==len(self.blob)
  self.blob=(root/'loot_table_pyarray.bin').read_bytes();self.cursor=0
  for a in (0x4ba4fc,0x4ba3c8,0x4ba27c,0x4ba12c,0x4b9fe0,0x4b9e8c,0x4b9d40,0x4b9bf4):self.invoke(a,[self.stream],budget=30000000)
  assert self.cursor==len(self.blob)
  self.table=self.word(self.word(0x994a98+self.word(0x4ba278)))
  base=0x401b10+self.word(0x401b84);self.seed=self.word(base+self.word(0x401b88));self.random_calls=self.word(base+self.word(0x401b8c))
  self.power_lists=self.word(0x9a65b8);self.power_count=self.word(0x9a65b4)
  got=0x401bac+self.word(0x401d84);self.quantities=self.word(self.word(got+self.word(0x401d8c)));self.quantity_count=self.word(self.word(got+self.word(0x401d88)))
  self.uc.hook_add(UC_HOOK_CODE,self.loot_service)
 def external(self,uc,a,z,u):
  name=self.imports.get(a)
  if name=='__aeabi_i2f':self.returned(bits(signed(self.reg(0))))
  elif name=='__aeabi_f2iz':self.returned(max(-2147483648,min(2147483647,int(floating(self.reg(0))))))
  else:super().external(uc,a,z,u)
 def loot_service(self,uc,a,z,u):
  if self.valuing and a==0x3fb754:self.returned();return
  if not self.looting:return
  caller=uc.reg_read(self.lr)-4
  if a in (0x337888,0x337a88):
   if a==0x337a88:assert self.stringvalue(self.reg(1))==b'isTracingItemInventory_Loot'
   self.calls.append(W(a,caller,0));self.returned()
  elif a==0x3fbc60:self.calls.append(W(a,caller,self.reg(1)))
 def random(self,seed,calls):self.pointer(self.seed,seed);self.pointer(self.random_calls,calls)
 def rng(self):return W(self.word(self.seed),self.word(self.random_calls))
 def state(self):
  begin,end=self.word(self.item+0x5c),self.word(self.item+0x60)
  return W((end-begin)//32)+b''.join(bytes(self.uc.mem_read(p,8))+block(self.stringvalue(p+8))for p in range(begin,end,32))
 def coordinator(self,args,initial):
  seed,calls,listid,quantity,bonus,requested,difficulty=args
  self.run_power(initial);self.random(seed,calls);self.calls=[]
  entry=self.data+0x15000;self.uc.mem_write(entry,W(0,0,listid,0,quantity,0,0,0,0))
  self.looting=self.loadingsource=True
  try:self.invoke(0x403310,[entry,self.item,bonus,requested,difficulty],budget=50000000)
  finally:self.looting=self.loadingsource=False
  return self.rng()+W(len(self.calls))+b''.join(self.calls)+self.state()
 def value(self,id,powers,bonus,seed,calls):
  self.pointer(self.item+4,id);self.random(seed,calls)
  backing=self.data+0x18000;self.uc.mem_write(backing,b''.join(W(x)+bytes(28)for x in powers))
  self.uc.mem_write(self.item+0x5c,W(backing,backing+32*len(powers),backing+32*len(powers)))
  self.valuing=True
  try:self.invoke(0x4020b4,[self.item,bonus],budget=10000000)
  finally:self.valuing=False
  return W(self.word(self.item+0x54))+self.rng()

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,required=True);a=ap.parse_args()
 old=OriginalLoot();new=NativePresentation(a.library);rng=random.Random(0x4c505637)
 root=R/'.local-inputs/player-loot-v7/cache';reference=R/'port/game-data/reference/loot-power-creation-v7';reference.mkdir(exist_ok=True,parents=True)
 names=['item_powers_pyarray.bin','item_powers_pyarraynames.bin','item_powers_pystructnames.bin','item_powers_monopoly_pyarray.bin','item_powers_monopoly_pyarraynames.bin','item_powers_monopoly_pystructnames.bin','num_prob_records_v7.bin','loot_table_pyarraynames.bin','loot_table_pystructnames.bin']
 blobs=[(root/n).read_bytes()for n in names]
 # Recover PLT targets through their real relocated GOT slots.
 md=Cs(CS_ARCH_ARM,CS_MODE_ARM);md.detail=True;imports=[]
 for address in (0x30e964,0x30eba4,0x30ec94,0x30ed6c,0x30e4cc):
  code=bytes(old.uc.mem_read(address,12));i=list(md.disasm(code,address));got=address+8+i[0].operands[2].imm+i[1].operands[2].imm+i[2].operands[1].mem.disp
  imports.append(dict(plt=hex(address),got=hex(got),name=old.imports[old.word(got)],code=code.hex()))
 assert [x['name']for x in imports]==['__aeabi_i2f','__aeabi_fadd','__aeabi_fdiv','__aeabi_fmul','__aeabi_f2iz']
 captures=[]
 with (R/'.local-inputs/libDungeonHunter2.so').open('rb')as f:
  elf=ELFFile(f);syms=elf.get_section_by_name('.symtab')
  for address in (0x401afc,0x401b90,0x401f38,0x4020b4,0x403310,0x4bacc8,0x4baa30,0x4b9bf4):
   symbols=[s for s in syms.iter_symbols()if s['st_value']==address and s['st_size']];symbol=max(symbols,key=lambda s:s['st_size']);size=symbol['st_size'];code=bytes(old.uc.mem_read(address,size));captures.append(dict(address=hex(address),symbol=symbol.name,size=size,code_sha256=hashlib.sha256(code).hexdigest()))
 (reference/'original-functions.json').write_text(json.dumps(dict(original_sha256=sha(R/'.local-inputs/libDungeonHunter2.so'),functions=captures,float_imports=imports,debug_switch='isTracingItemInventory_Loot'),indent=2)+'\n')
 out=new.data+0x1000;nrng=out+16;rows=new.data+0x3000
 records=[];counts=dict(weighted=0,quantity=0,value=0,coordinator=0,coordinator_services=0,retry_fallback_cases=0)
 power_lists=[]
 for listid in range(old.power_count):
  row=old.power_lists+12*listid;n,p=old.word(row+4),old.word(row+8);choices=[(old.word(p+12*j+4),struct.unpack('<b',old.uc.mem_read(p+12*j+8,1))[0])for j in range(n)];power_lists.append(choices)
  if not n or not sum(v for _,v in choices):continue
  packed=b''.join(struct.pack('<ib3x',signed(id),prob)for id,prob in choices)
  for k in range(8):
   seed,calls=rng.getrandbits(32),rng.getrandbits(32);old.random(seed,calls);result=old.invoke(0x401f38,[row]);expected=W(result)+old.rng()
   new.uc.mem_write(nrng,W(seed,calls));new.uc.mem_write(rows,packed);new.uc.mem_write(out,W(0xaaaa5555));status=new.invoke('dh2_loot_power_select_v7',[out,nrng,rows,n]);assert status==0,(listid,status)
   actual=bytes(new.uc.mem_read(out,4))+bytes(new.uc.mem_read(nrng,8));assert actual==expected,('power',listid,k,actual.hex(),expected.hex());records.append(block(W(0,seed,calls,n)+packed)+block(expected));counts['weighted']+=1
 for id in range(old.quantity_count):
  row=old.quantities+12*id;n,p=old.word(row+4),old.word(row+8);packed=b''.join(bytes(old.uc.mem_read(p+8*j+4,4))for j in range(n))
  if not n or not sum(struct.unpack_from('<h',packed,j*4+2)[0]for j in range(n)):continue
  for bonus in (-2147483648,-2,-1,0,1,25,100,2147483647):
   seed,calls=rng.getrandbits(32),rng.getrandbits(32);old.random(seed,calls);expected=W(old.invoke(0x401b90,[id,bonus]))+old.rng();new.uc.mem_write(nrng,W(seed,calls));new.uc.mem_write(rows,packed)
   assert new.invoke('dh2_loot_quantity_v7',[out,nrng,rows,n,bonus])==0;actual=bytes(new.uc.mem_read(out,4))+bytes(new.uc.mem_read(nrng,8));assert actual==expected,('quantity',id,bonus);records.append(block(W(1,seed,calls,n,bonus)+packed)+block(expected));counts['quantity']+=1
 # Every genuine item metadata row and actual power valuation fields.
 definitions=old.word(0x9a65c4)
 for id in range(1322):
  seed,calls=rng.getrandbits(32),rng.getrandbits(32);bonus=rng.choice((-2147483648,-25600,-256,-1,0,256,25600,2147483647));powers=[rng.randrange(937)for _ in range(id%5)]
  expected=old.value(id,powers,bonus,seed,calls);raw=bytearray(old.uc.mem_read(old.table+164*id,164))
  for off in (0,8,80):struct.pack_into('<I',raw,off,0)
  packed=b''.join(W(old.word(definitions+40*x+24),old.word(definitions+40*x+32))for x in powers)
  new.uc.mem_write(nrng,W(seed,calls));new.uc.mem_write(rows,bytes(raw));new.uc.mem_write(rows+0x100,packed)
  assert new.invoke('dh2_loot_item_value_v7',[out,nrng,rows,rows+0x100,len(powers),bonus])==0;actual=bytes(new.uc.mem_read(out,4))+bytes(new.uc.mem_read(nrng,8));assert actual==expected,('value',id,bonus,actual.hex(),expected.hex());records.append(block(W(2,seed,calls,len(powers),bonus)+raw+packed)+block(expected));counts['value']+=1
 cases=[];gold=[]
 for listid,choices in enumerate(power_lists):
  for target in sorted(set((0,1,2,len(choices),-1,-2))):
   for mode in (0,1,2):
    quantity=rng.randrange(old.quantity_count);args=(rng.getrandbits(32),rng.getrandbits(32),listid,quantity,rng.choice((-256,0,256,25600)),target,mode);initial=[]
    if target==2 and listid%7==0:initial=[(rng.randrange(935),0)]
    expected=old.coordinator(args,initial);command=W(*args,len(initial))+b''.join(W(*x)for x in initial);cases.append(command);gold.append(block(command)+block(expected));counts['coordinator_services']+=len(old.calls);counts['retry_fallback_cases']+=any(struct.unpack_from('<I',q,4)[0]in(0x40367c,0x4037b0)for q in old.calls)
 # Genuine no-power early return must not require either provider.
 args=(123,4,-1,-1,0,-1,0);expected=old.coordinator(args,[]);command=W(*args,0);cases.append(command);gold.append(block(command)+block(expected))
 expected=b''
 for row in gold:
  n=struct.unpack_from('<I',row)[0];expected+=row[4+n:]
 inp=b''.join(block(x)for x in blobs)+W(len(cases))+b''.join(cases);new.heap=new.data+0x500000;new.uc.mem_write(new.data+0x10000,inp)
 length=new.invoke('dh2_loot_power_creation_fixture_v7',[new.data+0x10000,new.data+0x200000],budget=800000000);assert length!=0xffffffff,('coordinator native required failure',hex(new.uc.reg_read(new.pc)))
 actual=bytes(new.uc.mem_read(new.data+0x200000,length));assert actual==expected,('coordinator',length,len(expected),next((i for i in range(min(length,len(expected)))if actual[i]!=expected[i]),None))
 counts['coordinator']=len(cases)
 binary=b'LPV7'+W(len(records))+b''.join(records)+W(len(gold))+b''.join(gold);ref=reference/'fixtures.bin';ref.write_bytes(binary)
 sources=['port/game-data/loot_power_resources_v7.hpp','port/game-data/loot_power_resources_v7.cpp','port/game-data/loot_power_creation_v7.hpp','port/game-data/loot_power_creation_v7.cpp','port/game-data/tests/loot_power_creation_v7_fixture.cpp',Path(__file__).relative_to(R).as_posix(),'port/game-data/tools/build_loot_power_v7_oracle.ps1']
 report=dict(validation='PASS',comparisons=sum(counts[k]for k in('weighted','quantity','value','coordinator')),counts=counts,actual_power_lists=old.power_count,actual_quantity_lists=old.quantity_count,actual_item_rows=1322,mismatches=0,original_sha256=sha(R/'.local-inputs/libDungeonHunter2.so'),library_sha256=sha(a.library),gold_sha256=sha(ref),source_sha256={x:sha(R/x)for x in sources},input_sha256={n:sha(root/n)for n in names},source_capture_sha256=sha(reference/'original-functions.json'),scope=__doc__)
 path=R/'port/game-data/reports/loot-power-creation-v7-arm64-differential.json';path.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',counts=counts,report=str(path))))
if __name__=='__main__':main()
