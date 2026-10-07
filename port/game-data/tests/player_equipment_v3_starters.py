"""Actual cached starting AddLoot items through original/native Character auto-equip.
Name/stats/requirements and Character property/Skin/HP-MP updates are explicit
fixture effects; owner flags and selected set are caller fixture projections.
Actual source item metadata, initial duplicates and original slot writes run.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from fresh_inventory_v2_original import Fresh,W
from navigation_differential import Cpu
from unicorn import UC_HOOK_CODE
OPS=(0x3e08a8,0x3a999c,0x3bd140)
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,required=True);a=ap.parse_args();old=Fresh();new=Cpu(a.library,True,{'functions':[]});trace=[];current=0;capture=False;callback=new.data+0x30000;svc=new.data+0x2000;new.uc.mem_write(svc,struct.pack('<QQ',0x123400007777,callback));oldtrace=[];newtrace=[]
 def oldhook(uc,address,size,user):
  if capture and address in OPS:oldtrace.append(W(current,address,uc.reg_read(old.lr)-4));old.returned()
 def newhook(uc,address,size,user):
  if address!=callback:return
  owner,item,op,caller,*args=struct.unpack('<QQII4i',uc.mem_read(new.reg(2),40));assert owner==0x123400005678 and item==0 and op in OPS and args==[0]*4;newtrace.append(W(current,op,caller));uc.mem_write(new.reg(3),bytes(16));new.put(0,0);uc.reg_write(new.pc,uc.reg_read(new.lr))
 old.uc.hook_add(UC_HOOK_CODE,oldhook);new.uc.hook_add(UC_HOOK_CODE,newhook);cases=[];requests=0
 for loot in (165,174,213):
  for flags in range(4):
   for selected in range(2):
    old.fresh_inventory(-1);old.loading=True;old.capture=True;old.requests=[];old.minimal=False;old.mutation=0;old.uc.mem_write(old.seed,W(1));old.uc.mem_write(old.rngcalls,W(0));old.uc.mem_write(old.uc.reg_read(old.sp),W(-1,0));old.invoke(0x40407c,[old.inv,loot,0,0],budget=20000000);old.capture=False;old.loading=False;old.pointer(old.inv+4,old.character);old.uc.mem_write(old.character+0x1320,W(flags&1,flags>>1));old.uc.mem_write(old.inv+0x2e,bytes([selected]));n=(old.word(old.inv+0xc)-old.word(old.inv+8))//4;assert n==(6 if loot==213 else 5)
    # Native metadata comes from the same actual cache reader, not synthetic
    # hardcoded weapon/slot guesses. Decoder parity is independently frozen.
    state=new.data+0x1000;itemlist=new.data+0x3000;items=[];slots=[];sets=[new.data+0x5000,new.data+0x5100];table=new.data+0x10000;rows=1322
    metadata=[]
    for i in range(rows):
     t=old.word(old.table+164*i+0x58);s=old.word(old.table+164*i+0x68);stack=old.uc.mem_read(old.table+164*i+0x1c,1)[0];new.uc.mem_write(table+12*i,W(t,s,stack))
    for i in range(n):
     source_slot=old.word(old.word(old.inv+8)+4*i);source=old.word(source_slot);id=old.word(source+4);quantity=struct.unpack('<h',old.uc.mem_read(source+0x50,2))[0];p=new.data+0x4000+i*32;q=new.data+0x4400+i*32;items.append(p);slots.append(q);new.uc.mem_write(p,struct.pack('<ihHQ',id,quantity,0,0x123400000000+i));new.uc.mem_write(q,struct.pack('<Qbb6x',p,-1,-1));new.uc.mem_write(itemlist+8*i,struct.pack('<Q',q));metadata.append({'id':id,'quantity':quantity,'type':old.word(old.table+164*id+0x58),'slotting_signed':struct.unpack('<i',old.uc.mem_read(old.table+164*id+0x68,4))[0]})
    for p in sets:new.uc.mem_write(p,bytes(9*8))
    new.uc.mem_write(state,struct.pack('<QQIi2Q4IQ2I',0x123400005678,itemlist,n,selected,*sets,9,flags&1,flags>>1,0,table,rows,0));out=new.data+0x2100;oldtrace.clear();newtrace.clear();result=[];capture=True
    for current in range(n):
     source=old.word(old.word(old.word(old.inv+8)+4*current));eq=old.invoke(0x3f9e68,[source])
     if eq:
      expected=old.invoke(0x3a9fa8,[old.character,current]);assert new.invoke('dh2_equipment_character_auto_v3',[out,state,current,svc])==0;got=struct.unpack('<I',new.uc.mem_read(out,4))[0];assert got==expected;result.append([current,expected])
    capture=False;assert oldtrace==newtrace;requests+=len(oldtrace);oldsets=[];newsets=[]
    sources=[old.word(old.word(old.inv+8)+4*i) for i in range(n)]
    for s in range(2):
     base=old.word(old.word(old.inv+0x14)+12*s)
     for j in range(9):p=old.word(base+4*j);q=struct.unpack('<Q',new.uc.mem_read(sets[s]+8*j,8))[0];oldsets.append(sources.index(p) if p else -1);newsets.append(slots.index(q) if q else -1)
    assert oldsets==newsets
    for i in range(n):assert bytes(old.uc.mem_read(sources[i]+4,2))==bytes(new.uc.mem_read(slots[i]+8,2))
    cases.append({'loot':loot,'selected':selected,'flag1320':flags&1,'flag1324':flags>>1,'items':metadata,'equipment_indices':oldsets,'results':result,'ordered_requests':[list(struct.unpack('<3I',q)) for q in oldtrace]})
 sha=lambda x:hashlib.sha256(Path(x).read_bytes()).hexdigest();report={'validation':'PASS','comparisons':len(cases),'ordered_requests':requests,'original_sha256':sha(ROOT/'.local-inputs/libDungeonHunter2.so'),'library_sha256':sha(a.library),'source_sha256':{n:sha(ROOT/n) for n in ('port/game-data/player_equipment_v3.hpp','port/game-data/player_equipment_v3.cpp')},'cache_sha256':sha(ROOT/'.local-inputs/items-discovery/loot_table_pyarray.bin'),'script_sha256':sha(__file__),'cases':cases,'mismatches':0,'scope':__doc__};path=ROOT/'port/game-data/reports/player-equipment-v3-actual-starters-differential.json';path.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='cases'}))
if __name__=='__main__':main()
