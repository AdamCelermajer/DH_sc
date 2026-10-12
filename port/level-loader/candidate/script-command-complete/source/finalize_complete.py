from pathlib import Path
import json,difflib,hashlib
p=Path(r'C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/candidate/script-command-complete')
base=p.parent/'command-kind7-v65'
j=json.loads((p/'source/constructor-data-runtime.json').read_text())
sc=j['bounded_named_method_scopes']
def scope(address):return sc[str(int(address,16))]
for name,first in [
 ('command_c1_descriptors_v59.inc','// Source C1 stores: retained46 executed MEM_WRITE evidence plus bounded ARM source for remaining34.'),
 ('script_data_schemas_v52.inc','// Data schemas: retained46 executed C1/reader evidence plus bounded ARM source for remaining34.')]:
 f=p/name;b=f.read_text();f.write_text(first+'\n'+b.split('\n',1)[1])
f=p/'script_command_receivers_v59.cpp';b=f.read_text().replace('Original source pointer C1 must produceNULL','Original source pointer C1 must produce NULL').replace('Required live SAME actual Menu alias owner";return false;}out=value','Required live SAME actual source target alias owner";return false;}out=value');f.write_text(b)
f=p/'source/recover_complete.py';b=f.read_text()
notice='# Source recovery generator originally creates scalar extra cells. The final working tree additionally uses proven native-width weak-alias cells for56..59; regenerating source outputs must preserve that reviewed API extension.\n'
if not b.startswith(notice):f.write_text(notice+b)
rows=['# Actual Main runtime boundaries for all80 table slots','',
'Source inspection only. The original ELF SHA256 is36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80. Each retained named symbol was decoded in <=256-byte read chunks; no original code ran. Symbol sizes may include literal pools. Literal short leaves below are identified only by their exact instruction sequence.','',
'All Execute invocation and IsBlocking dispatch belong to Main. A bridge callback returning true only reports successful actual dispatch; a literal BX LR Execute does not define a source boolean result or justify a gameplay effect. V59 has no competing scheduler or IsBlocking policy. Main may use checked_data_borrow for its actual class/query receiver and must fail closed for an absent required body.','',
'| Kind | Actual class | Init | Execute address / symbol bytes | IsBlocking address / symbol bytes / proven literal |',
'|---:|---|---|---|---|']
for v in j['runtime_matrix']:
 def method(m):
  d=v[m];s=scope(d['address']);size=max(t['size'] for t in d['symbols']);ins=s['instructions']
  exact=[(x['mnemonic'],x['operands']) for x in ins]
  desc=''
  if exact==[('bx','lr')]:desc='; BX LR only'
  elif len(exact)==2 and exact[0][0]=='mov' and exact[0][1] in ('r0, #0','r0, #1') and exact[1]==('bx','lr'):desc='; literal'+exact[0][1].split('#')[1]
  return d['address']+' /'+str(size)+desc
 rows.append('|'+str(v['kind'])+'|'+v['class']+'|'+('base BX LR' if v['Init']['address']=='0x455628' else v['Init']['address'])+'|'+method('Execute')+'|'+method('IsBlocking')+'|')
rows.extend(['','## Direct reached engine and runtime leaves','',
'The exact named method source, bytes, symbol metadata, and reached direct branch/call addresses are retained in source/constructor-data-runtime.json and matching source/<address>.asm. Direct-call lists include runtime helpers and do not constitute an implemented engine contract; indirect vtable dispatch remains inside Main. The decoded .asm includes literal pool words, which must not be interpreted as instructions reachable from the method.',''])
for v in j['runtime_matrix']:
 rows.append('### '+str(v['kind'])+' '+v['class'])
 rows.append('')
 for m in ('Execute','IsBlocking'):
  calls=v[m]['direct_external_calls']
  rows.append(m+': '+('; '.join(x['source']+' ->'+x['target']+' '+('/'.join(t['symbol'] for t in x['symbols']) or 'no retained dynamic symbol') for x in calls) if calls else 'no direct external branch/call in retained named scope')+'.')
 rows.append('')
(p/'MAIN-RUNTIME-BOUNDARIES.md').write_text('\n'.join(rows)+'\n')
h='''# Complete command storage integration

This single source extension contains80 distinct actual named receiver classes/factory cases and80 original Data schemas. It extends the immutable kind7/v65 baseline48 with32 remaining table slots. It implements storage construction and reading only; it does not make an all-map runtime claim.

## Integration

Apply integration.patch to the four active private loader files only:
script_command_receivers_v59.hpp, script_command_receivers_v59.cpp,
command_c1_descriptors_v59.inc, script_data_schemas_v52.inc.
The patch baseline is candidate/command-kind7-v65, which already contains kind4/kind7. Frozen V59/V62/V63/kind4/kind7 candidates are preserved.

The working tree copies script_command_init_v62.hpp and script_manager_bind_v63.hpp unchanged. Their quoted dependency chain is V63 -> V62 -> V59 -> script_manager_owner_v52.hpp. Use the existing loader include directory for that owner header. The copied script_manager_owner_v52.cpp is unchanged from the actual manager and selects this local schema include; it is an isolated milestone compile input and must not be linked alongside another owner implementation.

Main constructs its ONE App-owned actual V52 manager from V63 composed services. Cache/stream services.owner must have independent ownership; direct and alias App owners and replacement of an existing real factory are rejected. Main callbacks and their hidden captures must retain App weakly. No new manager, VM, global registry, command executable record, or alternate cache was added.

## Actual source storage

Original ARM offsets describe provenance, not a modern host byte layout. All command C1s produce skip4 byte0, kind8=-1, data_c=NULL. Only source-produced extra cells are available. No allocation padding or unproduced words are invented.

Kinds56..59 source C1 +0x10 NULL is now an actual native-width uintptr_t member with a weak actual target alias. Actual Execute stores the object pointer at45d480/45d490,45d384/45d394,45d28c/45d29c,45d190/45d1a0 respectively. Main supplies the real resolution/type checks/target mutation and writes through write_operand_pointer with an alias owner whose get() equals the pointer. checked_pointer_borrow pins that SAME weak target and exposes the SAME native cell; expiry/mismatch fails. The scoped pin preserves lifetime during a real body without retaining App between calls. Existing Show/Hide menu +0x10 uses the same checked native cell route and existing menu-specific borrow. Other unproved old extras remain opaque until an actual typed producer writes them.

All32 new Data readers use retained flat scalar/byte/CString helper calls. Raw words and float IEEE bits remain exact; there is no numeric reinterpretation or generated zero fill. CString reads follow their actual produced length fields. Explicit C1 scalar defaults are included only where bytes were source-written. The original46 executed evidence remains unchanged; kind4/kind7 and remaining32 use bounded static source evidence.

## Actual Init contracts

75 Init table slots are exact source base455628 BX LR. The same existing five nontrivial V62 adapters remain required and contain no permissive engine leaves:

*10 StartDialog459034: cached Data_c, actual App.constants2c, Data+c scalar read BEFORE required DialogStyles/EnterLocationDialog getConstant; actual comparison writes SAME skip4 exactly0/1.
*20 PlayEffect4598b4: cached Data+8 scalar read BEFORE required current FX singleton projection; Main RegisterFXSetToLoad4967e8 is a real functional registration leaf.
*22 ShowFlash45983c and23 HideFlash4597e8: required MenuManager GetInstance BEFORE checked CURRENT Data_c read. Read CString+10 from current data, case-sensitive strstr(name,"HUD"); match stores NULL, otherwise real GetMenuByName using original name produces checked actual menu alias.
*45 PlayActorAnim45a2ec: cache SAME Data before actual App.objects38; required GetObjectByName and handle GetObject(false), actual Character conversion; genuine returned null is original early return. SAME Character-owned Animator49c executes two real AddAnimDictToSet3c9b14 leaves in source order. Read cached Data+8 before first and re-read cached SAME Data+c after first callback; reuse cached SAME Animator. Opaque receiver extras are not speculatively interpreted.

Functional FX registration and animation dictionary registration remain required Main leaf calls. Header replacement after an Init callback retains the original V59 failure prefix rather than silently completing/replaying. These callbacks can mutate source values; V62 preserves exact cached/live load points already captured.

## Runtime boundary and lifetime

MAIN-RUNTIME-BOUNDARIES.md lists all80 actual Init/Execute/IsBlocking addresses, exact symbol sizes and proven short literal leaves, with reached direct Main/helper addresses. Execute/dialogue/cinematic/quest/sound/animation/object/level/tutorial effects and all blocking dispatch belong to Main.

V52 publishes the actual command C1 before Data reading, assigns SAME kind/Data/data_c, calls Init, then folds actual skip4. Failed reads/Init retain the reached prefix. Unload resets actual Data/pendingData then data_c before qualified base-storage release; command cleanup does not dispatch Execute/Finish. It then releases commands/names and resets original current/common fields. Extra original manager fields survive as in the retained source.

## Milestone verification boundary

This80-slot candidate has not been compiled, linked, executed or checked against new authored maps. Earlier frozen V59 production syntax and V62 template syntax passes do not cover this extension. No per-kind tests/jobs were run. Existing useful fixtures remain frozen; one comprehensive milestone suite must update coverage expectations to80, exercise all constructor/Data schemas plus weak native pointer expiry/high-bit aliases, preserve prior meaningful lifecycle/failure-prefix and callback-mutation cases, and invoke only genuine Main runtime leaves where admitted.

Retained original whole-parser evidence covers common+SWAMP70scripts/870records/3495values and46 reached kinds.51-level metadata contains37 other unique local script pairs without command execution receipts; these maps remain unsampled. Static80-slot storage coverage does not establish authored reach, behavior completion, mob/chest rendering or real saved-game integration.
'''
h=h.replace('contains80','contains 80').replace('baseline48','baseline 48').replace('with32','with 32').replace('All32','All 32').replace('original46','original 46').replace('remaining32','remaining 32').replace('all80','all 80').replace('This80','This 80').replace('to80','to 80').replace('SWAMP70','SWAMP 70').replace('and46','and 46').replace('Static80','Static 80').replace('75 Init','75 Init')
(p/'HANDOFF.md').write_text(h)
changed=['script_command_receivers_v59.hpp','script_command_receivers_v59.cpp','command_c1_descriptors_v59.inc','script_data_schemas_v52.inc']
diff=''.join(''.join(difflib.unified_diff((base/n).read_text().splitlines(True),(p/n).read_text().splitlines(True),fromfile='a/'+n,tofile='b/'+n)) for n in changed)
(p/'integration.patch').write_text(diff)
files=changed+['script_command_init_v62.hpp','script_manager_bind_v63.hpp','script_manager_owner_v52.cpp','HANDOFF.md','MAIN-RUNTIME-BOUNDARIES.md','integration.patch','source/constructor-data-runtime.json','source/recover_complete.py']
manifest={'status':'SOURCE_COMPLETE_MILESTONE_UNCOMPILED_UNRUN','baseline':'candidate/command-kind7-v65','classes_and_factories':80,'Data_schemas':80,'new_kinds':j['added_kinds'],'base_Init_slots':75,'required_Main_Init_kinds':[10,20,22,23,45],'Main_Execute_and_IsBlocking_required':True,'other_maps_sampled':False,'jobs_run':0,'files':{n:hashlib.sha256((p/n).read_bytes()).hexdigest() for n in files}}
(p/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
print(json.dumps({'candidate':str(p),'integration_files':changed,'runtime_rows':len(j['runtime_matrix']),'status':manifest['status'],'integration_patch_sha256':manifest['files']['integration.patch']}))
