"""Frozen original alias corpus versus optimized ARM64, with explicit string/allocator services.

Actual ARM64 std::map search/insert/balance/erase/destroy instructions execute.
No host alias/hash algorithm substitutes for either instruction kernel.
"""
import argparse,hashlib,json,struct,sys,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];REF=ROOT/'port/script-runtime/reference/function-alias'
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from visual_timeline_differential import TimelineCpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def w32(*v):return struct.pack('<'+'I'*len(v),*(n&0xffffffff for n in v))
def q64(*v):return struct.pack('<'+'Q'*len(v),*v)
class Native(TimelineCpu):
 def __init__(self,library):
  super().__init__(library,True,{'functions':[]});self.heap=self.data+0x100000
  self.owner=0;self.allocations={};self.frees=0;self.assignments=[];self.long_assignments=0;self.tree_checks=0
  self.input=self.data+0x1000;self.values=self.data+0x8000
  self.results=self.data+0x9000;self.returned=self.data+0xa000;self.error=self.data+0xb000
  self.vm=self.data+0xc000;self.bindings={};self.binding_calls=[];self.vm_calls=[];self.callback_calls=0
 def number(self,p):return struct.unpack('<Q',self.uc.mem_read(p,8))[0]
 def word(self,p):return struct.unpack('<I',self.uc.mem_read(p,4))[0]
 def text(self,p):
  raw=bytearray()
  while True:
   v=self.uc.mem_read(p+len(raw),1)[0]
   if not v:return bytes(raw)
   raw.append(v);assert len(raw)<65536
 def allocate(self,size):
  p=self.heap;self.heap+=(size+15)&~15;assert self.heap<self.data+0x1000000
  self.allocations[p]=size;self.uc.mem_write(p,bytes(size));return p
 def string(self,p):
  tag=self.uc.mem_read(p,1)[0]
  if tag&1:return bytes(self.uc.mem_read(self.number(p+16),self.number(p+8)))
  return bytes(self.uc.mem_read(p+1,tag>>1))
 def string_assignment(self,p,value):
  node=p-40;parent=self.number(node+16)
  visited=set()
  while parent not in (self.owner+8,self.owner+32):
   assert parent and parent not in visited;visited.add(parent);parent=self.number(parent+16)
  offset=0x34 if parent==self.owner+8 else 0x4c
  self.assignments.append(['copy',offset,self.word(node+32),value.hex()])
  if self.uc.mem_read(p,1)[0]&1:
   old=self.number(p+16);assert old in self.allocations;del self.allocations[old];self.frees+=1
  self.uc.mem_write(p,bytes(24))
  if len(value)<=22:self.uc.mem_write(p,bytes((len(value)<<1,))+value+b'\0')
  else:
   storage=self.allocate(len(value)+1);self.uc.mem_write(storage,value+b'\0')
   self.uc.mem_write(p,q64((len(value)+2)&~1|1,len(value),storage));self.long_assignments+=1
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='_Znwm':self.put(0,self.allocate(self.reg(0)))
  elif name=='_ZdlPvm':
   p,n=self.reg(0),self.reg(1);assert p in self.allocations,(hex(p),n)
   # Long string capacity is an explicit ABI fixture, rounded to an even word.
   assert self.allocations[p]==n or (self.allocations[p]+1)&~1==n,(self.allocations[p],n)
   del self.allocations[p];self.frees+=1
  elif name=='_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEaSERKS5_':
   p=self.reg(0);self.string_assignment(p,self.string(self.reg(1)));self.put(0,p)
  elif name=='_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignEPKc':
   p=self.reg(0);self.string_assignment(p,self.text(self.reg(1)));self.put(0,p)
  elif name=='dh2_script_vm_bind_source_values':
   assert self.reg(0)==self.vm and self.reg(3)==self.owner
   binding=self.text(self.reg(1)).decode();self.bindings[binding]=self.reg(2);self.binding_calls.append(binding);self.put(0,0)
  elif name=='dh2_script_vm_call_discard_source':
   assert self.reg(0)==self.vm and self.reg(3)==0 and self.reg(2)==0
   self.vm_calls.append({'name_hex':self.text(self.reg(1)).hex(),'requested_identity':self.reg(1)==self.input});self.put(0,0)
  else:return super().external(uc,address,size,unused)
  self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
 def state(self):
  def table(offset):
   seen=set();entries=[];head=self.owner+offset+8;root=self.number(head)
   def visit(node,parent):
    if not node:return 1
    assert node not in seen;seen.add(node);assert self.number(node+16)==parent
    color=self.uc.mem_read(node+24,1)[0];assert color in (0,1)
    left,right=self.number(node),self.number(node+8)
    if not color:
     assert (not left or self.uc.mem_read(left+24,1)[0]) and (not right or self.uc.mem_read(right+24,1)[0])
    lb=visit(left,node);entries.append((self.word(node+32),self.string(node+40)));rb=visit(right,node)
    assert lb==rb;return lb+color
   if root:assert self.uc.mem_read(root+24,1)[0]==1
   visit(root,head);assert len(entries)==self.number(self.owner+offset+16)
   assert [x[0] for x in entries]==sorted(x[0] for x in entries)
   assert self.number(self.owner+offset)==(min(seen,key=lambda n:self.word(n+32)) if seen else head)
   return entries
  result=(table(0),table(24),self.uc.mem_read(self.owner+48,1)[0]);self.tree_checks+=2;return result
 def execute(self,op,name=b'',replacement=b'',kind0=4,kind1=4,count=2):
  self.assignments=[];self.uc.mem_write(self.input,name+b'\0'+replacement+b'\0');other=self.input+len(name)+1
  if op==6:
   if self.owner:self.invoke('dh2_script_alias_destroy',[self.owner]);assert not self.allocations
   self.owner=self.invoke('dh2_script_alias_create',[]);assert self.owner>0xffffffff
   start=len(self.binding_calls);assert self.invoke('dh2_script_alias_bind',[self.vm,self.owner])==0
   assert self.binding_calls[start:]==['AddToVFTable','PushVFTable','PopVFTable'];return b''
  if op==0:return w32(self.invoke('dh2_script_alias_hash',[self.input]))
  if op==1:
   p=self.invoke('dh2_script_alias_resolve',[self.owner,self.input]);value=self.text(p)
   return w32(p==self.input,len(value))+value
  if op==2:return w32(self.invoke('dh2_script_alias_contains',[self.owner,self.input]))
  if op==3:
   raw=bytearray(40*max(2,count));struct.pack_into('<II',raw,0,kind0,0);struct.pack_into('<QQ',raw,16,self.input,len(name))
   struct.pack_into('<II',raw,40,kind1,0);struct.pack_into('<QQ',raw,56,other,len(replacement))
   self.uc.mem_write(self.values,bytes(raw));binding='AddToVFTable'
  else:binding='PushVFTable' if op==4 else 'PopVFTable'
  self.uc.mem_write(self.returned,w32(0xdeadbeef))
  assert self.invoke(self.bindings[binding],[self.owner,self.values,count,self.results,16,self.returned,self.error,256])==0
  assert self.word(self.returned)==0;self.callback_calls+=1;return b''

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,default=ROOT/'.local-inputs/script-function-alias/oracle.so')
 ap.add_argument('--output',type=Path,default=ROOT/'port/script-runtime/reports/script-function-alias-arm64-differential.json');a=ap.parse_args();started=time.monotonic()
 build_path=Path(str(a.library)+'.build.json');build=json.loads(build_path.read_text(encoding='utf-8-sig'))
 assert build['library_sha256']==sha(a.library)
 for source,digest in build['source_sha256'].items():assert sha(ROOT/source)==digest
 # Execute only preserved source-oracle definitions, not its corpus/report writer.
 # This reuses its explicit ARM32 string/tree allocation fixture unchanged.
 original_script=REF/'probe_original.py';code=original_script.read_text();prefix=code.split('records=[];rows=[]')[0]
 assert len(prefix)<len(code);old={'__file__':str(original_script)};exec(compile(prefix,str(original_script),'exec'),old)
 native=Native(a.library);raw=(REF/'alias-reference.bin').read_bytes();assert raw[:4]==b'FAL1'
 old_report=json.loads((REF/'original-probe.json').read_text());assert sha(REF/'alias-reference.bin')==old_report['gold_sha256']
 def old_state():
  c=old['c'];owner=old['owner']
  return tuple([(key,old['string'](old['word'](node+0x28))) for key,node in sorted(old['tables'][owner+off].items())] for off in (0x34,0x4c))+(c.uc.mem_read(owner+0x64,1)[0],)
 cases=0;copies=0;counts=[0]*7;states=0
 def compare(inp,expected=None):
  nonlocal cases,copies,states
  op,name,replacement,k0,k1,count=inp;before=len(old['trace']);result=old['action'](*inp)
  old_output=w32(result) if op in (0,2) else w32(result[0],len(result[1]))+result[1] if op==1 else b''
  actual=native.execute(*inp)
  assert actual==old_output,(cases,op,name,actual.hex(),old_output.hex())
  if expected is not None:assert old_output==expected,('preserved corpus',cases)
  original_copies=[t for t in old['trace'][before:] if t[0]=='copy']
  assert native.assignments==original_copies,(cases,'ordered string copies',original_copies,native.assignments)
  if op!=0:assert native.state()==old_state(),(cases,'full main/backup state',native.state(),old_state());states+=1
  cases+=1;copies+=len(original_copies);counts[op]+=1;return old_output
 p=8;count=struct.unpack_from('<I',raw,4)[0]
 for _ in range(count):
  ni,no=struct.unpack_from('<II',raw,p);p+=8;start=p
  op,n,m,k0,k1,argc=struct.unpack_from('<6I',raw,p);p+=24;name=raw[p:p+n];p+=n;replacement=raw[p:p+m];p+=m
  assert p==start+ni;expected=raw[p:p+no];p+=no;compare((op,name,replacement,k0,k1,argc),expected)
 assert p==len(raw) and count==4209
 # Add new long-value/short-to-long/long-to-short cases without rewriting frozen gold.
 extension=[]
 def extra(op,name=b'',replacement=b'',k0=4,k1=4,argc=2):
  inp=(op,name,replacement,k0,k1,argc);out=compare(inp);packed=w32(op,len(name),len(replacement),k0,k1,argc)+name+replacement
  extension.append(w32(len(packed),len(out))+packed+out)
 extra(6)
 for length in (0,1,15,16,22,23,24,127,128,129,1024):
  value=b'\xff'+b'z'*(length-1) if length else b''
  extra(3,b'Long',value);extra(1,b'Long');extra(4);extra(3,b'Long',b'new');extra(3,b'Added',value);extra(5);extra(1,b'Long');extra(2,b'Long');extra(1,b'Added')
 for i in range(20):extra(3,('Sort'+str(i)).encode(),b'v'* (24+i))
 extra(4)
 for i in reversed(range(20)):extra(3,('Sort'+str(i)).encode(),b'replacement')
 extra(5)
 for i in range(20):extra(1,('Sort'+str(i)).encode())
 # Compare the original resolved-call contract with the actual ARM64 wrapper.
 old['action'](6);native.execute(6)
 for name,value in ((b'OnTimer',b'Renamed'),(b'Renamed',b'Other')):old['action'](3,name,value);native.execute(3,name,value)
 c=old['c'];c.pointer(old['owner']+8,old['L']);c.pointer(old['returns']+0x24,old['return_vector']);c.uc.mem_write(old['return_vector'],w32(0,0,0))
 for name in (b'OnTimer',b'Unmapped',b'Renamed'):
  c.uc.mem_write(old['text'],name+b'\0');c.invoke(0x37c390,[old['owner'],old['text'],old['args'],old['returns']])
  native.uc.mem_write(native.input,name+b'\0');assert native.invoke('dh2_script_alias_call_discard_source',[native.vm,native.owner,native.input,0,0])==0
 assert native.vm_calls==old['live_calls'] and len(native.vm_calls)==3
 native.invoke('dh2_script_alias_destroy',[native.owner]);assert not native.allocations
 extended=REF/'arm64-extension-reference.bin';extended.write_bytes(b'FAL1'+w32(len(extension))+b''.join(extension))
 source_files=[ROOT/'port/script-runtime/script_function_alias.cpp',ROOT/'port/script-runtime/script_function_alias.h',ROOT/'port/script-runtime/script_runtime.h',Path(__file__),REF/'build_arm64_oracle.ps1',original_script]
 for module in tuple(sys.modules.values()):
  path=Path(getattr(module,'__file__','') or '.')
  if path.is_file() and path.name.endswith('_differential.py') and path.is_relative_to(ROOT):source_files.append(path)
 source_files=sorted(set(source_files))
 report={'validation':'PASS','original_sha256':old_report['original_sha256'],'arm64_library_sha256':sha(a.library),
   'build':build,'build_manifest_sha256':sha(build_path),
   'optimized_arm64':True,'native_pointers_above_4GiB':True,'preserved_original_cases':count,'extension_cases':len(extension),'comparisons':cases,'mismatches':0,
   'preserved_gold_sha256':sha(REF/'alias-reference.bin'),'extension_gold_sha256':sha(extended),'original_probe_sha256':sha(REF/'original-probe.json'),
   'preserved_host_report_sha256':sha(ROOT/'port/script-runtime/reports/script-function-alias-host-audit.json'),
   'original_manifest_sha256':sha(REF/'original-functions.json'),'original_assembly_sha256':sha(REF/'reference/original-functions.asm'),
   'source_sha256':{p.relative_to(ROOT).as_posix():sha(p) for p in source_files},'cases_by_operation':counts,
   'ordered_string_copy_services_compared':copies,'complete_main_and_backup_state_comparisons':states,
   'native_RB_integrity_checks':native.tree_checks,'native_long_string_assignments':native.long_assignments,
   'actual_native_bound_producer_callback_calls':native.callback_calls,'native_binding_registration_names':native.binding_calls,
   'resolved_call_contract_cases':len(native.vm_calls),'resolved_call_contract':native.vm_calls,
   'native_tree_insertion_balance_erase_destroy_instructions_execute':True,'native_import_calls':native.import_calls,
   'explicit_services':{'original':old_report['explicit_services']+['Lua globals primitive and pCall execution for three resolved-call cases'],'arm64':['operator new/delete fixture storage','libc++ string assignment/copy with actual short/long object ABI','VM binding registration/call observation; no Lua argument projection or complete VM execution']},
   'packaged_apk':False,'full_VM_or_allocator_parity':False,'elapsed_seconds':round(time.monotonic()-started,2),
   'scope':'Actual original hashing/unsigned lookup/contains/VFTable producer guards, backup, restore, erase and copy order versus O2 ARM64 source. ARM32 container allocation/shape is a service; native std::map tree instructions execute. String allocation/copy primitives are explicit on both sides, byte results and complete main/backup states exact.'}
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('source_sha256','native_import_calls')}))
if __name__=='__main__':main()
