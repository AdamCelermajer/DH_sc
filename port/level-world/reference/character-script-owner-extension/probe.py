"""Actual complete external selection/load/init/VCB/publication instructions; manager/VM/registration services explicit."""
import hashlib,json,struct
from pathlib import Path
from unicorn import UC_HOOK_CODE
HERE=Path(__file__).resolve().parent
setup=(HERE.parent/'character-script-ownership/probe.py').read_text().split('cases=[]')[0]
setup=setup.replace("manifest=json.loads((HERE/'original-functions.json').read_text())","manifest=json.loads((HERE.parent/'character-script-kinds/original-functions.json').read_text())")
exec(compile(setup,str(HERE/'probe.py'),'exec'))
mask=0;query=0
def additional(uc,a,size,unused):
 global query
 if a==0x37c2a0:
  value=(mask>>query)&1;trace.append(dict(service='alias_membership',name=string(cpu,cpu.reg(1)).decode(),result=value,flags_before=hex(w(cpu.reg(0)+0xb8)),progress=w(AI+0x28)));query+=1;ret(value)
 elif a==0x37c514:
  trace.append(dict(service='lua_call_no_arguments',receiver=hex(cpu.reg(0)),name=string(cpu,cpu.reg(1)).decode(),active_before=hex(w(AI+0x1c)),progress=w(AI+0x28)));ret()
cpu.uc.hook_add(UC_HOOK_CODE,additional)
cases=[]
base=(0x3cf05c+8+w(0x3cf1d4))&0xffffffff
cpu.pointer(base+w(0x3cf1dc),ROW_SLOT);cpu.pointer(ROW_SLOT,ROW)
for script in ['follower','monster','rene']:
 for load_result in [0,1]:
  for mask in [0,0xfff]:
   query=0;trace.clear();cpu.uc.mem_write(AI,bytes(0x1000));cpu.pointer(AI+4,CHAR);cpu.pointer(AI,cpu.symbols['_ZTV6CharAI']+8);cpu.pointer(AI+0x10,0xffffffff);cpu.pointer(AI+0x14,0xffffffff)
   cpu.uc.mem_write(CHAR,bytes(0x2000));cpu.pointer(CHAR,VT);cpu.pointer(VT+0xc,cpu.data+0x19000);cpu.pointer(VT+0x34,0x3a2ed4)
   cpu.uc.mem_write(ROW,bytes(76*68));cpu.uc.mem_write(NAME,script.encode()+b'\0');cpu.pointer(ROW+44*68+0x28,len(script));cpu.pointer(ROW+44*68+0x2c,NAME)
   cpu.invoke(0x3cf1f0,[AI]);pending=w(AI+0x20);assert w(AI+0x1c)==pending and w(AI+0x28)==7 and w(pending)==cpu.symbols['_ZTV11AISExternal']+8
   assert w(AI+0x30)==NAME and cpu.uc.mem_read(AI+0x2c,1)==b'\1' and query==12
   assert [x['filename'] for x in trace if x['service']=='manager_add_file']==['_commons',script]
   assert [x['event'] for x in trace if x['service']=='character_timer_start']==[0x33,0x34]
   assert [x['name'] for x in trace if x['service']=='lua_call_no_arguments']==['OnInit']
   assert all(x['active_before']=='0x0' for x in trace if 'active_before' in x)
   assert all(x['progress']==5 for x in trace if 'progress' in x)
   expected=((mask&1)<<11)|(((mask>>1)&1)<<12)|((mask>>2)&1023);assert w(pending+0xb8)==expected
   cases.append(dict(script=script,manager_result=load_result,alias_mask=mask,pending_fields={hex(x):hex(w(pending+x)) for x in [0x98,0xb8,0xbc,0xc0]},final_progress=7,final_scripted=1,active_equals_pending=True,trace=trace.copy()))
report=dict(validation='PASS',scope=__doc__,original_sha256=sha(ENGINE),probe_sha256=sha(Path(__file__)),setup_sha256=sha(HERE.parent/'character-script-ownership/probe.py'),kind_manifest_sha256=sha(HERE.parent/'character-script-kinds/original-functions.json'),original_instructions_executed=True,cases=cases,full_gameplay_namespace_executed=False)
(HERE/'external-owner-probe.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',original_external_order_cases=len(cases))))
