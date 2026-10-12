"""Execute actual additional AIS factories, empty-container destruction and VCB producers."""
import json,hashlib,struct
from pathlib import Path
from unicorn import UC_HOOK_CODE
HERE=Path(__file__).resolve().parent
# Reuse the frozen probe's explicitly declared allocation/VM/string services;
# execute only its setup, never its cases or output writers.
setup=(HERE.parent/'character-script-ownership/probe.py').read_text().split('cases=[]')[0]
setup=setup.replace('uc.mem_write(result,bytes(0x1000))','uc.mem_write(result,bytes([0xa5])*0x1000)')
exec(compile(setup,str(HERE/'probe.py'),'exec'))
mask=0;query=0
def additional(uc,a,size,unused):
 global query
 if a==0x37c2a0:
  value=(mask>>query)&1;trace.append(dict(service='alias_membership',name=string(cpu,cpu.reg(1)).decode(),result=value,flags_before=hex(w(cpu.reg(0)+0xb8))));query+=1;ret(value)
 elif a==0x37c514:
  trace.append(dict(service='lua_call_no_arguments',receiver=hex(cpu.reg(0)),name=string(cpu,cpu.reg(1)).decode()));ret()
cpu.uc.hook_add(UC_HOOK_CODE,additional)
cases=[]
def invoke(label,fn,args):
 trace.clear();cpu.invoke(fn,args);row=dict(case=label,function=hex(fn),trace=trace.copy());cases.append(row);return row
for kind,factory in [('AISDefault',0x3cce14),('AISMonster',0x3ccbe4),('AISFaery',0x3cccf8),('AISExternal',0x3ccaf4)]:
 cpu.uc.mem_write(AI,bytes(0x1000));cpu.pointer(AI+4,CHAR)
 row=invoke(kind+'-factory',factory,[AI]);pending=w(AI+0x20);size=next(x['size'] for x in row['trace'] if x['service']=='allocate')
 assert size==(0xc8 if kind=='AISFaery' else 0xc4)
 assert w(pending)==cpu.symbols['_ZTV'+str(len(kind))+kind]+8
 assert cpu.uc.mem_read(pending+12,1)==b'\1' and w(pending+8)!=0
 assert not any(x['service']=='register_function' for x in row['trace'])
 row.update(allocation_bytes=size,pending=hex(pending),initial_words={hex(x):hex(w(pending+x)) for x in [8,0x14,0x18,0x98,0xb4,0xb8,0xbc,0xc0]+([0xc4] if kind=='AISFaery' else [])})
 assert w(pending+0x18)==0 and all(w(pending+x)==0 for x in [0x98,0xb4,0xb8,0xbc,0xc0])
 if kind=='AISFaery':assert w(pending+0xc4)==0xffffffff
 for offset in [8,12,16,20]:
  method=w(w(pending)+offset);row=invoke(kind+'-virtual-'+hex(offset),method,[pending]);row['slot']=hex(offset)
  effects=[x for x in row['trace'] if x['service']!='actual_empty_selected_on_init']
  assert effects==([dict(service='lua_call_no_arguments',receiver=hex(pending),name={8:'OnInit',12:'OnInitPost',16:'OnInitFinal',20:'OnTerminate'}[offset])] if kind=='AISExternal' else []),(kind,offset,effects)
 row=invoke(kind+'-deleting-destructor',w(w(pending)+4),[pending]);assert [x['service'] for x in row['trace']]==['lua_close','free']
vcb=[]
for fn,total in [(0x3dc7d8,2),(0x3dcec8,12)]:
 obj=cpu.data+0x15000
 for mask in range(1<<total):
  query=0;cpu.uc.mem_write(obj,bytes(0x1000));cpu.pointer(obj+0xb8,0xf0f0f0f0)
  trace.clear();cpu.invoke(fn,[obj]);assert query==total
  expected=((mask&1)<<11)|(((mask>>1)&1)<<12)
  if total==12:expected|=(mask>>2)&1023
  assert w(obj+0xb8)==expected
  vcb.append(dict(function=hex(fn),mask=mask,result=expected,queries=trace.copy()))
report=dict(validation='PASS',scope=__doc__,original_sha256=sha(ENGINE),probe_sha256=sha(Path(__file__)),setup_sha256=sha(HERE.parent/'character-script-ownership/probe.py'),manifest_sha256=sha(HERE/'original-functions.json'),vtable_sha256=sha(HERE/'vtables.json'),original_instructions_executed=True,factory_and_lifecycle_cases=cases,vcb_cases=vcb,vcb_comparisons=len(vcb),explicit_services=['allocator/free','private VM creation/panic/close','string reserve','LuaScript alias-membership result','LuaScript no-argument Call result'])
(HERE/'kind-probe.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(dict(validation='PASS',factory_lifecycle_cases=len(cases),vcb_cases=len(vcb),vcb_query_order=[x['name'] for x in vcb[-1]['queries']])))
