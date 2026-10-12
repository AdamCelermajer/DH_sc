"""Hash-bound read-only inspection; never claims unimplemented integration."""
import hashlib,json,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[4]
HERE=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    paths=[
      'port/level-world/character_script_owner.hpp',
      'port/level-world/character_script_owner.cpp',
      'port/level-world/character_script_owner_bindings.inc',
      'port/level-world/tests/character_script_owner.cpp',
      'port/level-world/reference/character-script-ownership/original-functions.json',
      'port/level-world/reference/character-script-ownership/reference/original-functions.asm',
      'port/script-runtime/reference/game-bindings/registration-names.json',
      'port/script-runtime/reference/int-bindings/teardown/original-functions.json',
      'port/script-runtime/reference/int-bindings/teardown/reference/original-functions.asm',
      'port/script-runtime/reports/script-int-arm64-differential.json',
      'port/script-runtime/reports/script-int-host-audit.json',
      'port/script-runtime/script_int_bindings.hpp',
      'port/script-runtime/script_int_bindings.cpp',
      'port/script-runtime/script_runtime.h',
      'port/script-runtime/script_runtime.c',
      'port/level-world/reference/character-script-owner-integers/PROPOSAL.md',
      'port/level-world/reference/character-script-owner-integers/integration-cases.json',
      'port/level-world/reference/character-script-owner-integers/inspect_proposal.py',
    ]
    before={p:sha(ROOT/p) for p in paths}
    owner=(ROOT/paths[1]).read_text();catalog=(ROOT/paths[2]).read_text();historical=(ROOT/paths[3]).read_text()
    stage1=catalog.split('constexpr ScriptBinding24 bindings1[]{',1)[1].split('};',1)[0]
    rows=re.findall(r'\{"([^"]+)",0x([0-9a-f]+)u,([01])u,([01])u,0u\}',stage1)
    assert len(rows)==35 and rows[2]==('SetInt','37de5c','0','0') and rows[3]==('GetInt','37ec14','0','0')
    registrations=json.loads((ROOT/paths[6]).read_text());records=registrations['registrations'] if 'registrations' in registrations else registrations['bindings']
    exact=[r for r in records if r['caller']=='0x37b5a0' and r['name'] in ('SetInt','GetInt')]
    assert [(r['instruction'],r['callback']) for r in exact]==[('0x37b628','0x37de5c'),('0x37b644','0x37ec14')]
    assert 'dh2_script_int' not in owner and 'integer_bindings' not in owner
    assert 'get(a.vm,"SetInt").type==0' in historical
    assert owner.index('if(aliases)dh2_script_alias_clear_contents(aliases)')<owner.index('if(vm)dh2_script_vm_destroy(vm)')<owner.index('if(aliases)dh2_script_alias_destroy(aliases)')
    assert 'if(descriptor.method)continue;' in owner and 't.sessions.erase(r->subject)' in owner
    asm=(ROOT/paths[5]).read_text()
    for address in ('0037c6c4:','0037c6cc:','0037c6d0:','0037c6d4:','0037c6d8:'):assert address in asm
    arm=json.loads((ROOT/paths[9]).read_text());host=json.loads((ROOT/paths[10]).read_text())
    assert arm['validation']=='PASS' and arm['comparisons']==3163 and arm['mismatches']==0 and arm['actual_original_erased_nodes']==199
    assert host['validation']=='PASS' and host['host_audit']['actual_VM_checks']==35 and host['sanitizers']['errors']==0
    source=ROOT/'.local-inputs/libDungeonHunter2.so';assert sha(source)==arm['original_sha256']==host['original_sha256']
    assert before=={p:sha(ROOT/p) for p in paths},'inputs changed during inspection'
    result=dict(status='READ_ONLY_PROPOSAL_READY',integration_executed=False,production_files_written=False,
      original_sha256=sha(source),inputs_sha256=before,source_registrations=exact,
      current_owner=dict(integer_map_owned=False,integer_binding_accessor=False,stable_heap_session=True,
        all_source_global_deliveries=170,stage1_global_deliveries=35,historical_SetInt_nil_is_fixture_boundary=True),
      source_owned_map=dict(offset='0x1c',initialization=['0x37c6c4','0x37c6d8'],
        clearing=['0x37c0bc','0x37c0e8'],owned_VM_close='0x37c100',clear_after_aliases=True),
      proposed_test_plan='port/level-world/reference/character-script-owner-integers/integration-cases.json',
      proposed_test_cases=len(json.loads((HERE/'integration-cases.json').read_text())['cases']),
      production_API='Add constructor persistent services + stable borrowed integer_bindings accessor; keep old views/constructor and all registration deliveries.',
      outstanding='Parent approval/production implementation and NEW actual central-DSO owner integration proof; no current package/device/full-namespace claim.')
    (HERE/'inspection.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:result[k] for k in ['status','integration_executed','production_files_written','proposed_test_cases']}))
if __name__=='__main__':main()
