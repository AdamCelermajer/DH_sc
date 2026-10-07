"""Actual Add/Set aggression map instructions and ordered virtual boundaries.
Allocator, actor virtual policy and target OnAggro are explicit observers;
OnAggro snapshots both maps before insertion. No full AIS backend claim.
"""
from pathlib import Path
import sys,json,hashlib,struct,random
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from aggro_differential import Cpu,float_bits
from combat_result_differential import floating,bits
LIB=ROOT/'.local-inputs/libcharacter_skill_application_v6_oracle.so';ELF=ROOT/'.local-inputs/libDungeonHunter2.so'
manifest=json.loads((ROOT/'port/game-data/reference/aggro/original-functions.json').read_text())
old=Cpu(ELF,False,manifest);new=Cpu(LIB,True,{'functions':[]})
chars=[old.data+0x1000+i*0x2000 for i in range(2)];keys=[0x100000000+c for c in chars]
vt=old.data+0x20000;avt=vt+0x100;heap=old.data+0x100000;old_callback=old.data+0x50000
tables=[new.data+0x1000+i*0x20 for i in range(2)];stores=[new.data+0x2000+i*0x100 for i in range(2)]
owner=new.data+0x4000;target=owner+0x20;svc=owner+0x40;callback=owner+0x60;out=owner+0x80;req=owner+0x100;changed=owner+0x140
config={};traces={'old':[],'new':[]};record=[];initializing=False
def word(c,p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def returned(c,v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def entries(c,native):
 result=[]
 for i in range(2):
  values=[]
  if native:
   ptr,count,cap=struct.unpack('<QII',c.uc.mem_read(tables[i],16));assert cap==8
   for j in range(count):values.append(list(struct.unpack('<QII',c.uc.mem_read(ptr+j*16,16))))
  else:
   head=chars[i]+(0x444 if i==0 else 0x45c)
   def walk(node):
    if not node:return
    walk(word(c,node+8));values.append([0x100000000+word(c,node+16),word(c,node+20),0]);walk(word(c,node+12))
   walk(word(c,head+4));assert len(values)==word(c,head+16)
  result.append(values)
 return result
def event(native,op):
 traces['new'if native else 'old'].append([op,entries(new if native else old,native)if op==4 else None])
 return config['player']if op==1 else config['owner_dead']if op==2 else config['target_dead']if op==3 else 0
def hook_old(uc,at,size,user):
 global heap
 if at==0x708ec0:assert word(old,old.reg(0))==24;returned(old,heap);heap+=32
 elif at==0x3a49f0:
  if not initializing:event(False,1)
  returned(old,config.get('player',0))
 elif at==0x3a2ed4:
  if not initializing:event(False,2 if old.reg(0)==chars[0]else 3)
 elif at==old_callback:
  assert old.reg(0)==chars[1]+0x3c8 and old.reg(1)==chars[0]
  if not initializing:event(False,4)
  returned(old)
def hook_new(uc,at,size,user):
 if at!=callback:return
 op,reserved,subject,other=struct.unpack('<IIQQ',uc.mem_read(new.reg(1),24));assert not reserved
 assert subject==keys[0 if op in(1,2)else 1] and other==(keys[0]if op==4 else 0)
 uc.mem_write(new.reg(2),struct.pack('<I',event(True,op)));returned(new)
old.uc.hook_add(UC_HOOK_CODE,hook_old);new.uc.hook_add(UC_HOOK_CODE,hook_new)
old.pointer(vt+0x28,0x3a49f0);old.pointer(vt+0x34,0x3a2ed4);old.pointer(avt+0x38,old_callback);old.uc.mem_write(old_callback,struct.pack('<I',0xe12fff1e))
new.uc.mem_write(svc,struct.pack('<QQ',0,callback));new.uc.mem_write(callback,bytes.fromhex('c0035fd6'))
new.uc.mem_write(owner,struct.pack('<QQ',keys[0],tables[0]));new.uc.mem_write(target,struct.pack('<QQ',keys[1],tables[1]))
rng=random.Random(2026100577)
for i in range(4096):
 initializing=True;heap=old.data+0x100000;config=dict(player=0,owner_dead=0,target_dead=0)
 for j,c in enumerate(chars):
  old.uc.mem_write(c,bytes(0x1800));old.pointer(c,vt);old.pointer(c+0x3c8,avt);old.pointer(c+0x3cc,c)
  for offset in(0x444,0x45c):head=c+offset;old.uc.mem_write(head,struct.pack('<5I',0,0,head,head,0))
  new.uc.mem_write(tables[j],struct.pack('<QII',stores[j],0,8));new.uc.mem_write(stores[j],bytes(128))
 exists=i%2;previous=bits(rng.choice([0.,-20.,2.5,7.,1000.]));supplied=bits(rng.choice([0.,-5.,1.,3.75,100.]));add=(i//2)%2
 if exists:
  old.invoke(0x3d79ec,[chars[0]+0x3c8,chars[1],previous]);new.uc.mem_write(req,struct.pack('<QQQQII',tables[0],tables[1],keys[0],keys[1],previous,0));assert new.invoke('dh2_aggro_apply',[changed,req,0])==0
 config=dict(player=bool(i&4),owner_dead=bool(i&8),target_dead=bool(i&16));initializing=False
 old.uc.mem_write(chars[0]+0x1449,bytes([config['owner_dead']]));old.uc.mem_write(chars[1]+0x1449,bytes([config['target_dead']]));traces={'old':[],'new':[]}
 expected=old.invoke(0x3d7c68 if add else 0x3d79ec,[chars[0]+0x3c8,chars[1],supplied]);status=new.invoke('dh2_character_skill_aggro_v6',[out,owner,target,supplied,add,svc]);assert status==0,(i,status)
 actual=word(new,out);assert actual==expected,(i,actual,expected)
 assert traces['old']==traces['new'],(i,traces)
 assert entries(old,False)==entries(new,True),(i,entries(old,False),entries(new,True))
 record.append(dict(existing=exists,add=add,previous=previous,supplied=supplied,policy=config,returned=expected,trace=traces['old'],tables=entries(old,False)))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
gold=ROOT/'port/level-world/reference/character-skill-combat-v6/aggro-gold-v6.json';gold.write_text(json.dumps(record,indent=2)+'\n')
report=dict(validation='PASS',cases=len(record),ordered_services=sum(len(x['trace'])for x in record),mismatches=0,original_sha256=sha(ELF),optimized_sha256=sha(LIB),gold_sha256=sha(gold),scope=__doc__)
(ROOT/'port/level-world/reports/character-skill-aggro-v6-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
