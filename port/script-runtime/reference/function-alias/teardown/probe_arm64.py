"""Original LuaScript destructor alias phase vs actual optimized native clear."""
import hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[5];HERE=Path(__file__).resolve().parent;ALIAS=HERE.parent
sys.path.insert(0,str(ROOT/'port/script-runtime/tests'))
from script_function_alias_differential import Native
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
prefix=(ALIAS/'probe_original.py').read_text().split('records=[];rows=[]')[0]
ns={'__file__':str(ALIAS/'probe_original.py')};exec(compile(prefix,str(ALIAS/'probe_original.py'),'exec'),ns)
c=ns['c'];closing=[]
def old_state():
 return [len(ns['tables'][ns['owner']+offset]) for offset in (0x34,0x4c)]+[c.uc.mem_read(ns['owner']+0x64,1)[0]]
def close_hook(uc,address,size,unused):
 if address==0x31b180:
  assert c.reg(0)==ns['owner']+4
  closing.append(old_state());ns['ret']()
c.uc.hook_add(UC_HOOK_CODE,close_hook)
class TeardownNative(Native):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='_ZdlPvm' and self.reg(0) in self.node_groups:self.deleted_groups.append(self.node_groups[self.reg(0)])
  return super().external(uc,address,size,unused)
 def nodes(self):
  self.node_groups={};self.deleted_groups=[]
  def visit(node,group):
   if node:
    assert node not in self.node_groups;self.node_groups[node]=group;visit(self.number(node),group);visit(self.number(node+8),group)
  visit(self.number(self.owner+8),0x34);visit(self.number(self.owner+32),0x4c)
library=ROOT/'.local-inputs/script-function-alias/oracle-teardown.so';new=TeardownNative(library);new.node_groups={};new.deleted_groups=[]
rows=[]
for recording in (0,1):
 for count in (0,1,8,32):
  ns['action'](6);new.execute(6)
  for i in range(count):
   key=('key'+str(i)).encode();ns['action'](3,key,b'before');new.execute(3,key,b'before')
  if recording:
   ns['action'](4);new.execute(4)
   for i in range(max(1,count)):
    key=('key'+str(i)).encode();ns['action'](3,key,b'changed');new.execute(3,key,b'changed')
  for repeat in range(2):
   begin=len(ns['trace']);atclose=len(closing);c.invoke(0x37c004,[ns['owner']]);original_order=[x[1] for x in ns['trace'][begin:] if x[0]=='clear']
   assert closing[atclose:]==[[0,0,recording]],closing[atclose:]
   new.nodes();assert new.invoke('dh2_script_alias_clear_contents',[new.owner])==0
   state=new.state();assert state==([],[],recording),state
   native_order=[]
   for group in new.deleted_groups:
    if not native_order or native_order[-1]!=group:native_order.append(group)
   assert native_order==original_order,(native_order,original_order)
   assert set(new.allocations)=={new.owner},new.allocations
   rows.append({'tracking_before':recording,'main_setup_keys':count,'second_clear':bool(repeat),'original_clear_order':original_order,'native_node_delete_group_order':native_order,'original_state_at_instance_close':closing[-1],'native_cleared_state':[0,0,state[2]]})
new.invoke('dh2_script_alias_destroy',[new.owner]);assert not new.allocations
gold=HERE/'teardown-reference.json';gold.write_text(json.dumps({'rows':rows},indent=2)+'\n')
regression=ROOT/'port/script-runtime/reports/script-function-alias-teardown-arm64-regression.json';prior=json.loads(regression.read_text())
assert prior['validation']=='PASS' and prior['comparisons']==4371 and prior['arm64_library_sha256']==sha(library)
source=[ROOT/'port/script-runtime/script_function_alias.cpp',ROOT/'port/script-runtime/script_function_alias.h',Path(__file__),ROOT/'port/script-runtime/tests/script_function_alias_differential.py',ALIAS/'probe_original.py']
report={'validation':'PASS','comparisons':len(rows),'mismatches':0,'original_sha256':sha(ROOT/'.local-inputs/libDungeonHunter2.so'),'manifest_sha256':sha(HERE/'original-functions.json'),'assembly_sha256':sha(HERE/'reference/original-functions.asm'),'library_sha256':sha(library),'build_manifest_sha256':sha(Path(str(library)+'.build.json')),'regression_report_sha256':sha(regression),'regression_comparisons':prior['comparisons'],'teardown_reference_sha256':sha(gold),'source_sha256':{p.relative_to(ROOT).as_posix():sha(p) for p in source},'actual_original_destructor_to_instance_close':True,'tracking_preserved':True,'native_backup_before_main_delete_order':True,'original_instance_close_observations':len(closing),'explicit_services':['Original loaded-set/path/Arguments are empty fixture inputs','Original map allocation/tree shape and complete Instance destructor/VM close are services','Native operator new/delete and imported libc++ string primitives are services; native map clear/tree node destruction instructions execute'],'original_complete_lua_close_executed':False,'packaged_apk':False,'scope':'Exact original LuaScriptD1 alias-clear phase/order/empty headers/tracking byte observed at Instance destruction versus optimized native clear_contents. Wrapper remains alive. Genuine Lua __gc callbacks are independently covered by teardown host report.'}
out=ROOT/'port/script-runtime/reports/script-function-alias-teardown-arm64-differential.json';out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','comparisons':len(rows),'regression_comparisons':prior['comparisons'],'library_sha256':sha(library),'report_sha256':sha(out)}))
