"""Frozen original ClearAggro wrapper/ownership traces vs optimized ARM64.

Original source gold executes RB operations and Value.getUserData. Native uses
real aggro storage primitive and actual target setter. Notification/Stop bodies
and original null-setter body are explicit services in the ownership proof.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
WORLD=Path(__file__).resolve().parents[1];ROOT=WORLD.parents[1];REF=WORLD/'reference/character-clear-aggro'
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from aggro_differential import Cpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def state_words(s):
 out=[s['owner'],s['target'],len(s['outgoing'])]
 for e in s['outgoing']:out+=e
 out += [0]*(6-len(s['outgoing'])*2);out += [len(s['incoming'])]
 for e in s['incoming']:out+=e
 return out+[0]*(2-len(s['incoming'])*2)
def trace_words(t):return [{'OnDeAggro':0,'SetTargetNull':1,'CmdStop':2}[t['op']],t.get('receiver',t.get('controller',-1)),t.get('owner',-1),*state_words(t['state'])]
def pack(w):return struct.pack('<'+'I'*len(w),*[x&0xffffffff for x in w])
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,default=ROOT/'.local-inputs/character-clear-aggro/libcharacter_clear_aggro.so');a=ap.parse_args();proof=json.loads((REF/'source-probes.json').read_text());assert proof['validation']=='PASS' and proof['cases']==1792
 c=Cpu(a.library,True,{'functions':[]});d=c.data;s=d+0x1000;receiver=d+0x1100;targetstate=d+0x1200;owners=[d+0x1300+i*0x100 for i in range(4)];outtable=d+0x1800;intable=d+0x1900;outentries=d+0x2000;inentries=d+0x2100;target=d+0x2200;binding=d+0x2300;services=d+0x2400;bindings=d+0x2500;values=d+0x2600;callbacks=d+0x3000
 def id(i):return 0 if i<0 else 0x100000000+i+1
 def index(i):return -1 if not i else i-0x100000001
 def ptr(p):return struct.unpack('<Q',c.uc.mem_read(p,8))[0]
 def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
 def snap():
  op=ptr(receiver+8);rows=lambda table:[(index(key),bits) for key,bits,_ in [struct.unpack('<QII',c.uc.mem_read(ptr(table)+16*i,16)) for i in range(word(table+8))]]
  return dict(owner=owners.index(op),target=index(ptr(targetstate+24)),outgoing=rows(outtable),incoming=rows(intable))
 trace=[];mutation=0
 def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def hook(uc,address,size,_):
  if address==callbacks:
   op,_,recv,other,scope=struct.unpack('<IIQQQ',c.uc.mem_read(c.reg(3),32));assert not scope
   trace.append(dict(op='OnDeAggro' if not op else 'CmdStop',**({'receiver':index(recv-0x100000000),'owner':index(other)} if not op else {'controller':index(recv)}),state=snap()))
   if not op:
    if mutation==1:c.pointer(targetstate+24,0)
    elif mutation==2:c.pointer(receiver+8,owners[2])
    elif mutation==3:c.pointer(target+24,id(3))
   ret()
  elif address==callbacks+16:
   assert index(c.reg(1))==1;c.pointer(c.reg(2),target);ret()
  elif address==callbacks+32:
   op=word(c.reg(2))
   if op==0:trace.append(dict(op='SetTargetNull',receiver=1,state=snap()))
   assert op in (0,1);c.uc.mem_write(c.reg(3),pack([0]));ret()
 c.uc.hook_add(UC_HOOK_CODE,hook)
 for off in (0,16,32):c.uc.mem_write(callbacks+off,bytes.fromhex('c0035fd6'))
 c.uc.mem_write(services,struct.pack('<QQQ',0,callbacks,callbacks+16));c.uc.mem_write(bindings,struct.pack('<QQ',s,services))
 gold=[];calls=0
 for row in proof['records']:
  kind,count,identity,forward,reverse,targets,mutation,wrapper=row['input'];trace.clear()
  for i,p in enumerate(owners):c.uc.mem_write(p,struct.pack('<QHHI',id(i),0,0,0))
  c.uc.mem_write(receiver,struct.pack('<5Q4BI',0x200000001,owners[0],0,0,0,0,0,0,0,0));c.uc.mem_write(targetstate,struct.pack('<5Q4BI',0x200000002,owners[1],0,id(0) if targets else 0,0,0,0,0,0,0))
  c.uc.mem_write(s,struct.pack('<QQ',receiver,outtable));c.uc.mem_write(target,struct.pack('<QQQQ',id(1),intable,binding,id(1)));c.uc.mem_write(binding,struct.pack('<6Q',targetstate,0,callbacks+32,0,0,0))
  for table,storage,entries in ((outtable,outentries,row['before']['outgoing']),(intable,inentries,row['before']['incoming'])):
   c.uc.mem_write(storage,b''.join(struct.pack('<QII',id(key),bits,0) for key,bits in entries)+bytes((4-len(entries))*16));c.uc.mem_write(table,struct.pack('<QII',storage,len(entries),4))
  for i in range(max(1,count)):c.uc.mem_write(values+i*40,struct.pack('<4I3Q',kind,0,0,0,0,0,id(1) if identity else 0))
  status=c.invoke('dh2_character_clear_aggro_values',[bindings,0,values,count]) if wrapper else c.invoke('dh2_character_clear_aggro',[s,target if identity else 0,services,0]);assert not status,(row['input'],status)
  actual=state_words(snap());expected=state_words(row['after']);assert actual==expected,(row['input'],actual,expected)
  assert [trace_words(t) for t in trace]==[trace_words(t) for t in row['trace']],(row['input'],trace,row['trace']);assert not ptr(binding+24)
  calls+=len(trace);gold.append(pack(row['input']+state_words(row['before'])+expected+[len(trace)])+b''.join(pack(trace_words(t)) for t in trace))
 corpus=REF/'clear-aggro-fixtures.bin';corpus.write_bytes(pack([0x31414743,len(gold)])+b''.join(gold))
 source=[WORLD/'character_clear_aggro.cpp',WORLD/'character_clear_aggro.hpp',WORLD/'character_target_bindings.cpp',ROOT/'port/game-data/aggro.cpp',Path(__file__)]
 report=dict(validation='PASS',comparisons=len(gold),ordered_calls=calls,mismatches=0,original_sha256=proof['original_sha256'],original_probe_sha256=sha(REF/'source-probes.json'),manifest_sha256=sha(REF/'original-functions.json'),arm64_sha256=sha(a.library),corpus_sha256=sha(corpus),source_sha256={p.relative_to(ROOT).as_posix():sha(p) for p in source},scope=__doc__,actual_native_target_setter=True,scope_delivery_VM_proof=False)
 (WORLD/'reports/character-clear-aggro-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
