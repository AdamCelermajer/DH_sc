"""Bind frozen read-only source ownership discovery and its focused probes."""
import hashlib,json
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
ownership=json.loads((HERE/'ownership-probe.json').read_text());vm=json.loads((HERE/'vm-core-probe.json').read_text())
assert ownership['validation']==vm['validation']=='PASS'
for report,name in ((ownership,'probe.py'),(vm,'vm_core_probe.py')):
 assert report['probe_sha256']==sha(HERE/name)
 for path,digest in report['manifest_sha256'].items():assert sha(HERE/path)==digest
def event_counts(label):
 events=next(x for x in ownership['cases'] if x['case']==label)['trace']
 return {s:sum(x['service']==s for x in events) for s in ('open_library','register_function','register_method')}
first=event_counts('step1-bind');second=event_counts('step2-real-character-and-gameobject-registration')
assert first=={'open_library':4,'register_function':35,'register_method':0}
assert second=={'open_library':0,'register_function':135,'register_method':130}
sources=[p for p in HERE.rglob('*') if p.is_file() and p.suffix in ('.py','.md','.asm','.json') and p.name not in ('ownership-probe.json','vm-core-probe.json')]
unique={r['elf_address']:r for p in HERE.rglob('original-functions.json') for r in json.loads(p.read_text())['functions']}
report=dict(validation='PASS',scope='Read-only original ownership and VM/library-core discovery; no native module, packaged/runtime, whole-AIS or complete registration callback parity claim.',original_sha256=ownership['original_sha256'],cache_sha256=ownership['cache_sha256'],original_ownership_cases=len(ownership['cases']),original_vm_core_cases=len(vm['rows']),original_library_order=['base','math','table','string'],original_library_stack_tops=[2,3,4,5],original_repeat_stack_tops=[7,8,9,10],heap_allocations=vm['heap_allocations'],heap_frees=vm['heap_frees'],selected_player_scripted=1,privately_owned_vm=True,constructor_defers_bindings=True,stage1=first,stage2=second,common=ownership['commons'],probes={str(p.relative_to(ROOT)):sha(p) for p in (HERE/'ownership-probe.json',HERE/'vm-core-probe.json')},source_sha256={str(p.relative_to(REPO)):sha(p) for p in sources},captured_unique_functions=len(unique),proposed_additive_api=['dh2_script_vm_create_empty(size_t)','dh2_script_vm_open_source_libraries(dh2_script_vm*)'],production_runtime_changed=False,source_failure_and_native_protection_boundary='Original ignores common-load bool and closes owned null state; native protection must remain an explicit caller contract.',remaining_services=['35Stage1 game callback implementations/registration','135global+130method Character registration services and callback implementations','manager file cache/resource I/O/per-script loaded set','active/pending lifetime from existing ScriptLifecycle'])
output=ROOT/'reports/character-script-ownership-discovery.json';output.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(dict(validation='PASS',report=str(output.relative_to(REPO)),sha256=sha(output),ownership_cases=report['original_ownership_cases'],vm_core_cases=report['original_vm_core_cases'],captured_unique_functions=len(unique))))
