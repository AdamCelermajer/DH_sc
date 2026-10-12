"""Focused original Level/Idle/PreSpawn versus frozen O2 owner selection.
Actual embedded machine+3c and Character+538 are proven identical addresses.
Behavior methods remain explicit dispatch fixtures; no production edits.
"""
from pathlib import Path
R=Path(__file__).resolve().parent
source=(R/'character_state_owner_differential.py').read_text()
def replace(old,new):
 global source
 assert old in source,old
 source=source.replace(old,new)
replace('from unicorn import UC_HOOK_CODE','from unicorn import UC_HOOK_CODE,UC_HOOK_MEM_WRITE')
replace('calls=[[],[]];mutation=0;', 'alias_writes=[]\n def alias_write(uc,access,address,size,value,unused):\n  if address<=om+0x3c<address+size:alias_writes.append(dict(pc=hex(uc.reg_read(old.pc)),address=address,size=size,value=value))\n old.uc.hook_add(UC_HOOK_MEM_WRITE,alias_write)\n assert om==ob+0x4fc and om+0x3c==ob+0x538\n calls=[[],[]];mutation=0;')
replace('chosen=-1,body=1):','chosen=-1,body=1,suppression=1):')
replace('bytes([1]));old.pointer(ob+0x2dc','bytes([suppression]));old.pointer(ob+0x2dc')
replace('0xfffffff0,0,1,0,0,0,0,body','0xfffffff0,0,suppression,0,0,0,0,body')
replace("calls[0].clear();calls[1].clear()", "calls[0].clear();calls[1].clear();alias_writes.clear()")
replace('expect=[old_id(),', 'assert old.uc.mem_read(om+0x3c,1)==old.uc.mem_read(ob+0x538,1)\n  assert len(alias_writes)==int(preset not in (0,17)),(preset,alias_writes)\n  if alias_writes:assert alias_writes==[dict(pc=hex(0x3c1a00),address=ob+0x538,size=1,value=0)],alias_writes\n  expect=[old_id(),')
replace('input=[operation,id,next,event,preset,change,predicate,chosen,body]', 'dual_address_write_trace=alias_writes.copy(),input=[operation,id,next,event,preset,change,predicate,chosen,body,suppression]')
start=source.index(' for id,next in itertools.product(');end=source.index(' # Actual original registration order',start)
source=source[:start]+" for suppression,preset,id in itertools.product(range(256),(-1,0,3,4,17,19),(-1,0,3,5,17)):compare(2,id,0,-1,preset,suppression=suppression)\n"+source[end:]
replace("bytearray(b'SBO1'", "bytearray(b'SAI1'")
replace("struct.pack('<9I'", "struct.pack('<10I'")
replace("scope=__doc__,", "scope="+repr(__doc__)+",embedded_machine_offset=0x4fc,machine_idle_offset=0x3c,character_idle_offset=0x538,identical_source_addresses=om+0x3c==ob+0x538,original_idle_byte_writes=sum(len(x['dual_address_write_trace']) for x in records),all256_initial_byte_values=True,production_correction_required=False,")
replace("'tests/character_state_owner_differential.py']", "'tests/character_state_owner_differential.py','tests/character_state_owner_initialization_alias.py']")
# Frozen native oracle must match existing source-bound binary proof. It is not
# rebound to changed source or an inferred current central DSO.
replace("old=Cpu(a.engine", "frozen=json.loads((R/'reports/character-state-owner-arm64-differential.json').read_text());assert sha(a.library)==frozen['arm64_library_sha256'];assert all(sha(R.parents[1]/x)==h for x,h in frozen['source_sha256'].items())\n old=Cpu(a.engine")
exec(compile(source,str(R/'character_state_owner_differential.py'),'exec'),{'__name__':'__main__','__file__':str(R/'character_state_owner_differential.py')})
