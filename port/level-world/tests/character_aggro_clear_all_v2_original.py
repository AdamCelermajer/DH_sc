"""Whole original ClearAllAggro methods. Allocators and callbacks are named
fixtures; original map traversal/erase/clear and target SyncLastTarget execute.
Native keys normalize each original Character address to index+0x100000001.
"""
import sys,struct,json,hashlib,itertools
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
ELF=ROOT/'.local-inputs/libDungeonHunter2.so'
c=Cpu(ELF,False,json.loads((ROOT/'port/game-data/reference/aggro/original-functions.json').read_text()))
c.events=[];d=c.data;chars=[d+0x1000+i*0x2000 for i in range(6)]
vt=d+0x20000;avt=vt+0x100;stub=avt+0x100
state={'heap':d+0x100000,'phase':False,'trace':[],'reenter':False,'entered':False}
def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def index(p):return chars.index(p) if p else -1
def entries(i,j):
 head=chars[i]+(0x444 if j==0 else 0x45c);out=[]
 def walk(p):
  if p:walk(word(p+8));out.append([index(word(p+16)),word(p+20)]);walk(word(p+12))
 walk(word(head+4));assert len(out)==word(head+16);return out
def snap():
 return [{'out':entries(i,0),'in':entries(i,1),'target':index(word(p+0x408)),
          'last':index(word(p+0x40c))}for i,p in enumerate(chars)]
def hook(uc,at,size,_):
 if at==0x708ec0:
  count=word(c.reg(0));assert 0<count<=4096;ret(state['heap']);state['heap']+=(count+31)&~31
 elif at==0x31056c:
  count=c.reg(0);assert 0<count<=4096;ret(state['heap']);state['heap']+=(count+31)&~31
 elif at in(0x708f00,0x310440):ret()
 elif at==0x3a49f0:ret(0)
 elif at in(stub,stub+16):
  if state['phase']:
   state['trace'].append({'op':2,'receiver':index(c.reg(0)-0x3c8),'other':index(c.reg(1)),'state':snap()})
  ret()
 elif at==0x3d6890:
  assert c.reg(1)==c.reg(2)==0
  state['trace'].append({'op':4,'receiver':index(c.reg(0)-0x3c8),'other':-1,'state':snap()})
  c.pointer(c.reg(0)+0x40,0);ret()
c.uc.hook_add(UC_HOOK_CODE,hook)
c.pointer(vt+0x28,0x3a49f0);c.pointer(vt+0x34,0x3a2ed4)
c.pointer(avt+0x38,stub);c.pointer(avt+0x3c,stub+16)
c.uc.mem_write(stub,bytes.fromhex('1eff2fe1')+bytes(12)+bytes.fromhex('1eff2fe1'))
records=[]
for incoming,clear_targets,count,extras in itertools.product((0,1),(0,1),range(6),(0,1)):
 state.update(heap=d+0x100000,phase=False,trace=[])
 for p in chars:
  c.uc.mem_write(p,bytes(0x1800));c.pointer(p,vt);c.pointer(p+0x3c8,avt);c.pointer(p+0x3cc,p)
  for off in(0x444,0x45c):head=p+off;c.uc.mem_write(head,struct.pack('<5I',0,0,head,head,0))
 edges=[]
 for i in range(1,count+1):edges.append((i,0)if incoming else(0,i))
 if extras:edges.extend([(1,2),(2,3),(3,1)])
 for a,b in edges:c.invoke(0x3d79ec,[chars[a]+0x3c8,chars[b],0x41200000+a+b])
 for i,p in enumerate(chars):c.pointer(p+0x408,chars[0] if i%2 else chars[5]);c.pointer(p+0x40c,chars[4])
 before=snap();state['phase']=True
 c.invoke(0x3d6abc if incoming else 0x3d5fa8,[chars[0]+0x3c8,clear_targets])
 state['phase']=False
 records.append(dict(incoming=incoming,clear=clear_targets,before=before,after=snap(),trace=state['trace']))
ref=ROOT/'port/level-world/reference/character-world-death-v2';ref.mkdir(parents=True,exist_ok=True)
(ref/'clear-all-gold.json').write_text(json.dumps(records,indent=2)+'\n')
out=bytearray(struct.pack('<II',0x32414341,len(records)))
def append_snapshot(rows):
 for r in rows:
  out.extend(struct.pack('<ii',r['target'],r['last']))
  for key in('out','in'):
   out.extend(struct.pack('<I',len(r[key])))
   for i,bits in r[key]:out.extend(struct.pack('<II',i,bits))
for r in records:
 out.extend(struct.pack('<II',r['incoming'],r['clear']));append_snapshot(r['before']);append_snapshot(r['after'])
 out.extend(struct.pack('<I',len(r['trace'])))
 for e in r['trace']:
  out.extend(struct.pack('<Iii',e['op'],e['receiver'],e['other']));append_snapshot(e['state'])
(ref/'clear-all-gold.bin').write_bytes(out)
report=dict(validation='PASS',cases=len(records),source_sha256=hashlib.sha256(ELF.read_bytes()).hexdigest(),
 gold_sha256=hashlib.sha256(out).hexdigest(),scope=__doc__,full_game_death=False)
(ROOT/'port/level-world/reports/character-aggro-clear-all-v2-original.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report))
