"""Actual selected timer->Arguments->alias->global lookup; VM execution is a service."""
import hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[4];HERE=Path(__file__).resolve().parent
ALIAS=ROOT/'port/script-runtime/reference/function-alias'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
original_script=ALIAS/'probe_original.py';prefix=original_script.read_text().split('records=[];rows=[]')[0]
prefix=prefix.replace('c.uc.hook_add(UC_HOOK_CODE,hook)','original_hook_handle=c.uc.hook_add(UC_HOOK_CODE,hook)')
ns={'__file__':str(original_script)};exec(compile(prefix,str(original_script),'exec'),ns)
c=ns['c'];c.uc.hook_del(ns['original_hook_handle']);d=c.data
ai=d+0xc000;vtable=d+0xd000;vector=d+0xe000;value=d+0xf000;retvec=d+0x11000
observed=[]
def hook(uc,address,size,unused):
 if address==0x3192b4:
  c.pointer(c.reg(0)+4,vector);uc.mem_write(vector,words(value,value,value+112));ns['ret']()
 elif address==0x3195c0:
  assert c.reg(0)==vector;raw=bytes(uc.mem_read(c.reg(1),112));uc.mem_write(value,raw);c.pointer(vector+4,value+112);ns['ret']()
 elif address==0x31b434:
  c.pointer(c.reg(0)+0x24,retvec);uc.mem_write(retvec,words(0,0,0));ns['ret']()
 elif address==0x31ab4c:
  assert c.reg(0)==ns['owner']+4
  p=ns['word'](c.reg(1)+4);assert ns['word'](p)==value and ns['word'](p+4)==value+112
  observed.append({'name_hex':ns['live_calls'][-1]['name_hex'],'type':ns['word'](value+4),'number_bits':ns['word'](value+8)})
  ns['ret']()
 elif address in (0x319228,0x31b398,0x3193b0,0x37be44,0x708f00,0x310440):ns['ret']()
 else:ns['hook'](uc,address,size,unused)
c.uc.hook_add(UC_HOOK_CODE,hook)
ids=[0,1,2,0x7fffff,0xffffff,0x1000000,0x1000001,0x1000003,0x7fffffff,0x80000000,0x80000001,0xffffffff]
rng=random.Random(0x3dcc80);ids += [rng.getrandbits(32) for _ in range(84)]
rows=[];raw=[]
for mode in range(3):
 ns['action'](6)
 if mode:ns['action'](3,b'OnTimer',b'Renamed')
 if mode==2:ns['action'](3,b'Renamed',b'Other')
 c.pointer(ns['owner']+8,ns['L']);c.pointer(ns['owner'],vtable);c.pointer(vtable+0x90,0x3dcc80)
 for active in (0,1):
  c.pointer(ai+0x1c,ns['owner'] if active else 0)
  for ident in ids:
   begin=len(observed);c.invoke(0x3d0ca0,[ai,ident]);calls=observed[begin:]
   assert len(calls)==active
   if active:
    assert calls[0]['type']==3 and bytes.fromhex(calls[0]['name_hex'])==(b'Renamed' if mode else b'OnTimer')
    bits=calls[0]['number_bits']
   else:bits=0
   rows.append({'id':ident,'active':active,'alias_mode':mode,'calls':calls});raw.append(words(ident,active,mode,bits))
gold=HERE/'timer-call-reference.bin';gold.write_bytes(b'STC1'+words(len(rows))+b''.join(raw))
manifest=HERE/'original-functions.json';report={'validation':'PASS','cases':len(rows),'actual_selected_calls':len(observed),
 'original_sha256':sha(ROOT/'.local-inputs/libDungeonHunter2.so'),'gold_sha256':sha(gold),'manifest_sha256':sha(manifest),
 'assembly_sha256':sha(HERE/'reference/original-functions.asm'),'alias_source_probe_sha256':sha(original_script),'script_sha256':sha(Path(__file__)),
 'explicit_services':['Arguments/ReturnValues/vector allocation, append and destruction','Value string reserve/vector cleanup','original alias fixture tree allocation','Lua globals primitive and complete pCall execution'],
 'actual_instructions':['CharAI.OnScriptTimer active gate','AISDefault.OnScriptTimer','Arguments.pushInteger','Value integer constructor and number setter','LuaScript.Call discarded wrapper and resolver','Instance.pCall global lookup'],
 'arithmetic_service':'existing identical AEABI signed32 to float32 import contract','rows':rows,'full_vm_or_allocator_parity':False}
(HERE/'original-probe.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('validation','cases','actual_selected_calls','gold_sha256')}))
