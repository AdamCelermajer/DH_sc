"""Actual Lua ClearAggro wrapper plus original map/ownership instructions.

Original SetAggro constructs the RB maps; erase/rebalance and Value.getUserData
execute. Allocation, player classification and notification/SetTarget/controller
backends are explicit services. This is source discovery, not native parity.
"""
import hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
WORLD=Path(__file__).resolve().parents[1];ROOT=WORLD.parents[1];REF=WORLD/'reference/character-clear-aggro'
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from aggro_differential import Cpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 manifest=json.loads((REF/'original-functions.json').read_text());engine=ROOT/'.local-inputs/libDungeonHunter2.so';assert sha(engine)==manifest['original_sha256'];c=Cpu(engine,False,manifest);c.events=[];d=c.data
 chars=[d+0x1000+i*0x2000 for i in range(4)];vt=d+0x20000;avt=vt+0x100;stub=vt+0x200;args=d+0x22000;vector=args+0x100;values=args+0x200;out=d+0x24000
 state=dict(heap=d+0x100000,trace=[],mode=0,phase=False,mutated=False,freed=0);records=[]
 def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
 def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def idx(p):return chars.index(p) if p else -1
 def entries(character,offset):
  head=character+offset;result=[]
  def walk(p):
   if p:walk(word(p+8));result.append([idx(word(p+16)),word(p+20)]);walk(word(p+12))
  walk(word(head+4));assert len(result)==word(head+16);return result
 def snap():return dict(owner=idx(word(chars[0]+0x3cc)),target=idx(word(chars[1]+0x408)),outgoing=entries(chars[0],0x444),incoming=entries(chars[1],0x45c))
 def hook(uc,address,size,_):
  if address==0x708ec0:assert word(c.reg(0))==24;ret(state['heap']);state['heap']+=32
  elif address==0x708f00:state['freed']+=1;ret()
  elif address==0x3a49f0:ret(0)
  elif address in (stub,stub+16):
   if state['phase']:
    state['trace'].append(dict(op='OnDeAggro' if address==stub+16 else 'OnAggro',receiver=idx(c.reg(0)-0x3c8),owner=idx(c.reg(1)),state=snap()))
    if address==stub+16 and not state['mutated']:
     state['mutated']=True
     if state['mode']==1:c.pointer(chars[1]+0x408,0)
     elif state['mode']==2:c.pointer(chars[0]+0x3cc,chars[2])
     elif state['mode']==3:c.pointer(chars[1]+0x378,chars[3]+0x1700)
   ret()
  elif address==0x3d6890:
   assert c.reg(1)==c.reg(2)==0
   if state['phase']:state['trace'].append(dict(op='SetTargetNull',receiver=idx(c.reg(0)-0x3c8),state=snap()))
   c.pointer(c.reg(0)+0x40,0);ret()
  elif address==0x40559c:
   if state['phase']:state['trace'].append(dict(op='CmdStop',controller=idx(c.reg(0)-0x1700),state=snap()))
   ret()
 c.uc.hook_add(UC_HOOK_CODE,hook);c.pointer(vt+0x28,0x3a49f0);c.pointer(vt+0x34,0x3a2ed4);c.pointer(avt+0x38,stub);c.pointer(avt+0x3c,stub+16);c.uc.mem_write(stub,bytes.fromhex('1eff2fe1')+bytes(12)+bytes.fromhex('1eff2fe1'));c.pointer(args+4,vector)
 def reset():
  state.update(heap=d+0x100000,trace=[],phase=False,mutated=False,freed=0)
  for char in chars:
   c.uc.mem_write(char,bytes(0x1800));c.pointer(char,vt);c.pointer(char+0x3c8,avt);c.pointer(char+0x3cc,char);c.pointer(char+0x378,char+0x1700)
   for off in (0x444,0x45c):head=char+off;c.uc.mem_write(head,struct.pack('<5I',0,0,head,head,0))
 def populate(forward,reverse):
  # Source insertion includes unrelated keys, testing true lower_bound/erase.
  for target in (3,2):c.invoke(0x3d79ec,[chars[0]+0x3c8,chars[target],0x40000000])
  if forward or reverse:c.invoke(0x3d79ec,[chars[0]+0x3c8,chars[1],0x41200000])
  if reverse and not forward:
   # A partial ownership fixture: remove outgoing via actual erase while
   # retaining reciprocal. ClearAggro must leave that reciprocal untouched.
   head=chars[0]+0x444;node=word(head+4)
   while word(node+16)!=chars[1]:node=word(node+(8 if word(node+16)>chars[1] else 12))
   c.pointer(out,node);c.invoke(0x3d5d9c,[head,out])
  if forward and not reverse:
   head=chars[1]+0x45c;node=word(head+4);c.pointer(out,node);c.invoke(0x3d5d9c,[head,out])
 def run(kind,count,identity,forward,reverse,targets,mutation,wrapper):
  reset();populate(forward,reverse);state['mode']=mutation;c.pointer(chars[1]+0x408,chars[0] if targets else 0)
  c.pointer(vector,values);c.pointer(vector+4,values+count*144)
  for i in range(max(1,count)):c.uc.mem_write(values+144*i,bytes(144));c.pointer(values+144*i+4,kind);c.pointer(values+144*i+0x6c,chars[1] if identity else 0)
  sentinel=bytes([0xa5])*144;c.uc.mem_write(out,sentinel);before=snap();state['phase']=True
  if wrapper:c.invoke(0x3b8648,[args,out,chars[0]])
  else:c.invoke(0x3d6d68,[chars[0]+0x3c8,chars[1] if identity else 0])
  state['phase']=False;assert bytes(c.uc.mem_read(out,144))==sentinel
  accepted=identity and (not wrapper or count>0 and kind in (2,7));removed=accepted and forward
  expected=[]
  if removed:expected.append('OnDeAggro')
  clear=accepted and targets and not(removed and mutation in (1,2))
  if clear:expected+=['SetTargetNull','CmdStop']
  assert [r['op'] for r in state['trace']]==expected,(kind,count,identity,forward,reverse,targets,mutation,wrapper,state['trace'],expected)
  after=snap();assert (any(x[0]==1 for x in after['outgoing']))==bool(forward and not accepted)
  assert (any(x[0]==0 for x in after['incoming']))==bool(reverse and not(forward and accepted))
  if removed:assert not any(x[0]==1 for x in state['trace'][0]['state']['outgoing']) and not state['trace'][0]['state']['incoming']
  if clear:assert state['trace'][-1]['controller']==(3 if removed and mutation==3 else 1)
  records.append(dict(input=[kind,count,identity,forward,reverse,targets,mutation,wrapper],before=before,after=after,trace=state['trace'],freed=state['freed']))
 for row in itertools.product(range(9),(0,1,3),(0,1),(0,1),(0,1),(0,1),(0,1,2,3)):
  run(*row,True)
 for row in itertools.product((0,1),(0,1),(0,1),(0,1),(0,1,2,3)):
  identity,forward,reverse,targets,mutation=row
  run(7,1,identity,forward,reverse,targets,mutation,False)
 proof=dict(validation='PASS',original_sha256=sha(engine),manifest_sha256=sha(REF/'original-functions.json'),cases=len(records),ordered_calls=sum(len(r['trace']) for r in records),records=records,
  source_executed=['Lua wrapper3b8648','Value.getUserData31b5a0','ClearAggro3d6d68','SetAggro/map insertion/rebalancing','erase3d5d9c/RB rebalance336004'],services=['allocator/deallocator','IsPlayer','OnAggro/OnDeAggro callback bodies','SetTarget null body','CmdStop body'],
  ReturnValues_unchanged=True,full_world_ownership=False,native_comparison=False)
 (REF/'source-probes.json').write_text(json.dumps(proof,indent=2)+'\n');print(json.dumps({k:v for k,v in proof.items() if k!='records'}))
if __name__=='__main__':main()
