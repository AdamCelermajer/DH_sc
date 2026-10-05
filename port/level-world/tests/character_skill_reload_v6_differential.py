"""Original AI_ReloadSkills instruction order through declared destructor/SG/config/update observers.
The same native coordinator is used by actual V6 player/Skill/Save ownership;
these boundary observers do not claim original pooled Value storage parity.
"""
from pathlib import Path
import json,sys,hashlib,struct
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3];sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from aggro_differential import Cpu
OLD=ROOT/'.local-inputs/libDungeonHunter2.so';LIB=ROOT/'.local-inputs/libcharacter_skill_application_v6_oracle.so'
old=Cpu(OLD,False,{'functions':[]});new=Cpu(LIB,True,{'functions':[]})
ai=old.data+0x1000;character=old.data+0x2000;slots=old.data+0x3000;vtable=old.data+0x4000;callback=old.data+0x5000;nodes=[old.data+0x10000+i*0x100 for i in range(16)]
state=new.data+0x1000;nslots=new.data+0x2000;services=new.data+0x3000;ncb=new.data+0x4000;out=new.data+0x5000;nnodes=[new.data+0x10000+i*0x100 for i in range(16)]
trace={'old':[],'new':[]};count=0
def ret(c,v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def snap(native):
 c=new if native else old
 active=struct.unpack('<I',c.uc.mem_read(state+16,4))[0]if native else (struct.unpack('<I',c.uc.mem_read(ai+0xb8,4))[0]-slots)//4
 raw=c.uc.mem_read(nslots if native else slots,count*(8 if native else 4));values=struct.unpack('<'+('Q'if native else'I')*count,raw)if count else[];lookup=nnodes if native else nodes
 return [active,[lookup.index(v)+1 if v else 0 for v in values]]
def hook_old(uc,at,size,user):
 if at==callback:
  index=nodes.index(old.reg(0));trace['old'].append([1,index,snap(False)]);ret(old)
 elif at in(0x3bbe2c,0x3ce044,0x3d8894):
  assert old.reg(0)==(character if at==0x3bbe2c else ai);trace['old'].append([{0x3bbe2c:3,0x3ce044:4,0x3d8894:5}[at],0,snap(False)]);ret(old)
def hook_new(uc,at,size,user):
 if at!=ncb:return
 op,index,owner,instance=struct.unpack('<IIQQ',uc.mem_read(new.reg(2),24));assert owner==0x100000009
 if op==2:new.uc.mem_write(state+16,struct.pack('<I',0));ret(new);return
 if op==1:assert instance==nnodes[index]
 trace['new'].append([op,index,snap(True)]);ret(new)
old.uc.hook_add(UC_HOOK_CODE,hook_old);new.uc.hook_add(UC_HOOK_CODE,hook_new);old.uc.mem_write(callback,struct.pack('<I',0xe12fff1e));old.pointer(vtable+4,callback)
new.uc.mem_write(ncb,bytes.fromhex('c0035fd6'));new.uc.mem_write(services,struct.pack('<QQ',0,ncb))
records=[]
for i in range(2048):
 count=i%17;pattern=(i//17)%16;values=[nodes[j]if((j+pattern)%5)else 0 for j in range(count)];nvalues=[nnodes[j]if values[j]else 0 for j in range(count)]
 old.uc.mem_write(ai,bytes(0x100));old.pointer(ai+4,character);old.pointer(ai+0xb4,slots);old.pointer(ai+0xb8,slots+count*4);old.pointer(ai+0xbc,slots+16*4)
 old.uc.mem_write(slots,struct.pack('<16I',*(values+[0]*(16-count))));new.uc.mem_write(nslots,struct.pack('<16Q',*(nvalues+[0]*(16-count))))
 for node in nodes:old.pointer(node,vtable)
 new.uc.mem_write(state,struct.pack('<QQIIQII',0x100000009,nslots,count,0,0,0,0));trace={'old':[],'new':[]}
 old.invoke(0x3d8cfc,[ai]);assert new.invoke('dh2_character_skill_reload_v6',[out,state,services])==1
 assert trace['old']==trace['new'],(i,trace)
 assert snap(False)==snap(True),(i,snap(False),snap(True))
 records.append(dict(count=count,pattern=pattern,trace=trace['old']))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
gold=ROOT/'port/level-world/reference/character-skill-combat-v6/reload-gold-v6.json';gold.write_text(json.dumps(records,indent=2)+'\n')
report=dict(validation='PASS',cases=len(records),ordered_services=sum(len(x['trace'])for x in records),mismatches=0,original_sha256=sha(OLD),optimized_sha256=sha(LIB),gold_sha256=sha(gold),scope=__doc__)
(ROOT/'port/level-world/reports/character-skill-reload-v6-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
