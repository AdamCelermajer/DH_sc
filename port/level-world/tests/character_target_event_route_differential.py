"""Execute O2 ARM64 against complete Character/CharAI target routing observations.

Original-only probe executes both routers; typed handlers and FSM are explicit
synchronous providers. This binds generic gates/forwarding and live owner reload,
not the full selected handler implementation, FSM or world/event ownership.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/character-target-event-route'
sys.path.insert(0,str(REPO/'port/game-data/tests'))
from items_differential import words
from navigation_differential import Cpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();source=json.loads((REF/'endpoints-original-probe.json').read_text());assert source['validation']=='PASS' and source['native_comparisons']==0 and source['original_sha256']==sha(REPO/'.local-inputs/libDungeonHunter2.so') and source['manifest_sha256']==sha(REF/'original-functions.json') and source['script_sha256']==sha(ROOT/'tests/character_target_endpoints_original.py')
 c=Cpu(a.library,True,{'functions':[]});state=c.data+0x1000;owners=[c.data+0x1100,c.data+0x1200];facts=c.data+0x1300;services=c.data+0x1400;callback=c.data+0x1500;result=c.data+0x1600;vtable=c.data+0x2000;c.uc.mem_write(callback,bytes.fromhex('c0035fd6'));c.uc.mem_write(services,struct.pack('<QQII',0,callback,3,0));c.uc.mem_write(vtable,struct.pack('<51Q',*(0x300000000+4*i for i in range(51))));trace=[];row=None;payload=0
 def hook(uc,address,size,_):
  if address!=callback:return
  service,operation,event,argument,subject,callee,actual_payload=struct.unpack('<4I3Q',uc.mem_read(c.reg(2),40));assert event==row['event'] and argument==0
  if service==1:
   slot=0x34 if event==9 else 0x40+4*(event-10);assert operation==slot and callee==0x300000000+slot and actual_payload==(payload if event==9 else 0)
   assert subject==0x20000000b;trace.append(['handler',event,row['payload']])
   if row['replace_owner']:uc.mem_write(state+8,struct.pack('<Q',owners[1]))
  else:
   assert service==0 and operation==0 and callee==0 and actual_payload==payload and subject in (0x100000101,0x100000102);trace.append(['fsm',subject-0x100000100,event,row['payload']])
  uc.mem_write(c.reg(3),words(row['handler_return']));c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
 c.uc.hook_add(UC_HOOK_CODE,hook);gold=b'CTR1'+words(len(source['routing_cases']));calls=0
 for row in source['routing_cases']:
  payload=(0x100000000+row['payload']) if row['payload'] else 0
  for i,owner in enumerate(owners):c.uc.mem_write(owner,struct.pack('<4Q4I',0x100000001+i,0x100000201+i,0x100000101+i,0x100000301+i,row['forced'],row['locked'],0,0))
  c.uc.mem_write(state,struct.pack('<5Q6I',0x20000000b,owners[0],vtable,0,0,0,0,row['blocked'],0,0,0));c.uc.mem_write(facts,struct.pack('<QQII',payload,0,0,0));trace.clear();assert c.invoke('dh2_character_ai_event',[result,state,row['event'],facts,services])==0;assert struct.unpack('<4I',c.uc.mem_read(result,16))[0]==7;assert trace==row['trace'],(row,trace)
  entries=[]
  for entry in row['trace']:entries.append([0,row['event'],11,row['payload']] if entry[0]=='handler' else [1,row['event'],entry[1],row['payload']])
  gold+=words(row['event'],row['blocked'],row['locked'],row['forced'],row['handler_return'],row['payload'],row['replace_owner'],len(entries),*(x for entry in entries for x in entry));calls+=len(entries)
 (REF/'target-route-fixtures.bin').write_bytes(gold)
 result={'validation':'PASS','production_export':'dh2_character_ai_event','comparisons':len(source['routing_cases']),'ordered_services':calls,'mismatches':0,'payload_and_receiver_above_4gib':True,'original_sha256':source['original_sha256'],'original_probe_sha256':sha(REF/'endpoints-original-probe.json'),'manifest_sha256':source['manifest_sha256'],'corpus_sha256':hashlib.sha256(gold).hexdigest(),'arm64_sha256':sha(a.library),'source_sha256':{str(x.relative_to(REPO)):sha(x) for x in (ROOT/'character_ai_events.hpp',ROOT/'character_ai_events.cpp',Path(__file__))},'scope':__doc__};(ROOT/'reports/character-target-event-route-arm64-differential.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
