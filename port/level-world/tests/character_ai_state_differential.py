"""Original CanAttack flow vs ARM64. Hostility/inventory/range remain explicit query services."""
import argparse,hashlib,itertools,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../engine-resources/tests'))
from cpu import Cpu
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);a=p.parse_args()
 manifest=json.loads((ROOT/'reference/prince-live-ai/attack-gate/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256']
 old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]})
 ai=old.data+0x1000;owners=[old.data+0x2000,old.data+0x4000];target=old.data+0x6000;vt=old.data+0x8000;virtual=old.data+0xa000
 ns=new.data+0x1000;nsvc=new.data+0x2000;nout=new.data+0x3000;ni=[0xabcdef0100002000,0xabcdef0100004000];nt=0xbcdef01200006000
 old.uc.mem_write(virtual,bytes.fromhex('1eff2fe1'))
 for owner in owners:old.pointer(owner,vt)
 old.pointer(vt+0x124,virtual)
 cb=new.data+0x4000;new.uc.mem_write(cb,bytes.fromhex('c0035fd6'));new.uc.mem_write(nsvc,struct.pack('<QQ',0x123456789abcdef0,cb))
 facts=();mask=0;trace=[[],[]]
 def oldword(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 def oldret(v):old.put(0,v);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def hook_old(uc,address,size,unused):
  nonlocal trace
  addresses=(0x3d574c,0x3ffd38,0x3d6188,virtual,0x3d6604)
  if address not in addresses:return
  kind=addresses.index(address);oi=owners.index(oldword(ai+4))
  assert old.reg(0)==(owners[oi]+0x37c if kind==1 else owners[oi] if kind==3 else ai)
  if kind in (0,2,4):assert old.reg(1)==target
  trace[0].append((kind,oi,2))
  if mask&(1<<kind):old.pointer(ai+4,owners[oi^1]);old.pointer(ai+0x40,owners[1])
  oldret(facts[kind])
 def hook_new(uc,address,size,unused):
  if address!=cb:return
  assert new.reg(0)==0x123456789abcdef0 and new.reg(1)==ns
  kind,reserved,owner,held=struct.unpack('<IIQQ',uc.mem_read(new.reg(2),24));assert reserved==0 and held==nt
  oi=ni.index(owner);trace[1].append((kind,oi,2))
  if mask&(1<<kind):uc.mem_write(ns,struct.pack('<QQ',ni[oi^1],ni[1]))
  new.put(0,facts[kind]);uc.reg_write(new.pc,uc.reg_read(new.lr))
 old.uc.hook_add(UC_HOOK_CODE,hook_old);new.uc.hook_add(UC_HOOK_CODE,hook_new)
 records=[];fixtures=list(itertools.product((0,1),(0,1),itertools.product((0,1),repeat=5),(0,1,10,31)))
 rng=random.Random(20261003)
 fixtures += [(rng.randrange(2),rng.randrange(2),tuple(rng.choice((0,1,2,255,0xffffffff)) for _ in range(5)),rng.randrange(32)) for _ in range(512)]
 for explicit,present,facts,mask in fixtures:
  old.uc.mem_write(ai,bytes(0x100));old.pointer(ai+4,owners[0]);old.pointer(ai+0x40,target if present else 0)
  new.uc.mem_write(ns,struct.pack('<QQ',ni[0],nt if present else 0));new.uc.mem_write(nout,struct.pack('<I',0xdeadbeef));trace[0].clear();trace[1].clear()
  expected=old.invoke(0x3d67f4,[ai,target if explicit else 0]);status=new.invoke('dh2_character_ai_can_attack',[ns,nt if explicit else 0,nsvc,nout]);actual=struct.unpack('<I',new.uc.mem_read(nout,4))[0]
  assert status==0 and expected==actual and trace[0]==trace[1],(explicit,present,facts,mask,expected,actual,trace)
  oo=owners.index(oldword(ai+4));ot=oldword(ai+0x40);ti=0 if not ot else 2 if ot==target else 3
  no,nT=struct.unpack('<QQ',new.uc.mem_read(ns,16));assert ni.index(no)==oo and (0 if not nT else 2 if nT==nt else 3)==ti
  values=[explicit,present,*facts,mask,expected,oo,ti,len(trace[0])]
  values += [word for request in trace[0] for word in request]+[0]*((5-len(trace[0]))*3)
  records.append(struct.pack('<27I',*values))
 gold=struct.pack('<III',0x31535141,len(records),108)+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(gold)
 report=dict(validation='PASS',comparisons=len(records),ordered_queries=sum(struct.unpack_from('<I',r,44)[0] for r in records),mismatches=0,original_sha256=sha(a.engine),arm64_library_sha256=sha(a.library),reference_sha256=hashlib.sha256(gold).hexdigest(),script_sha256=sha(Path(__file__)),source_sha256={str(p.relative_to(ROOT)):sha(p) for p in (ROOT/'character_ai_state.hpp',ROOT/'character_ai_state.cpp')},scope=__doc__,owner_reload_and_retained_target_reentry=True,identities_above_4gib=True)
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
