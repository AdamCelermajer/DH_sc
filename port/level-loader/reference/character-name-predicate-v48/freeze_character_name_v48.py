from pathlib import Path
import json,hashlib,zipfile
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/character-name-predicate-v48';v=b/'vendor/character-rng-integration-v5-loading/port/level-world';reports=b/'reports';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
parent_file=reports/'coherent-loader-v49-integrated-verified.json';parent=json.loads(parent_file.read_text());assert parent['validation']=='PASS' and parent['coherent_files_verified']==749
semantic=['retained_character_actor_v1.cpp','character_is_player_v41.hpp','character_is_player_v41.cpp']
line_endings={}
for n in semantic:
 assert (r/n).read_text()==(v/n).read_text(),n
 line_endings[n]={'candidate_before_pin_sha256':sha(r/n),'selected_sha256':sha(v/n),'text_equal':True};(r/n).write_bytes((v/n).read_bytes());assert sha(r/n)==sha(v/n)
for n in ['character_name_predicate_v48_test.cpp','character_spawn_name_v48_regression.cpp']:
 selected=b/'tests/name-selected-v48'/n;assert (r/n).read_text()==selected.read_text(),n;(r/n).write_bytes(selected.read_bytes());assert sha(r/n)==parent['source_sha256']['tests/name-selected-v48/'+n],n
executions={n:parent['executions'][n] for n in ['character_name','character_spawn','character_model']}
for n,run in executions.items():
 assert run['returncode']==0 and run['stdout'].startswith('PASS'),n
 binary=Path('C:/'+run['command'][4][7:]);assert sha(binary)==run['binary_sha256'],n
receipt={'schema':'dh2-character-name-predicate-v48-verified','validation':'PASS selected coherent749 actual factory/kernel','original_name30_buffer44':True,'archetype48_buffer5c_excluded':True,'native_actual_factory_cases':20,'native_live_name_changes':20,'native_spawn_records':8,'native_model_regression':True,'executions':executions,'parent_receipt_sha256':sha(parent_file),'coherent_manifest_sha256':parent['manifest_sha256'],'semantic_source_sha256':{n:sha(v/n) for n in semantic},'line_ending_normalization_pin':line_endings,'native_test_sha256':{n:sha(r/n) for n in ['character_name_predicate_v48_test.cpp','character_spawn_name_v48_regression.cpp','character_model_name_v48_regression.cpp']},'validation_provenance':'Parent-confirmed coherent selected source was read back and native binary/test/source hashes checked. Candidate text matched exactly; CRLF copies pinned to the selected LF bytes. No redundant candidate rebuild.','retraction':'Historical V41/V42 original predicate bit results over artificial44 buffers remain source control evidence; archetype label and actual actor archetype binding superseded.','remaining_fixtures':'actual AI row choice in predicate resolved-sheet field; spawn offline/lifecycle observers; model Debug/difficulty1/DEFAULT/master-save association. No whole engine/player equipment/activation claim.','missing_parent_cell_changes_bundled':False,'whole_InitPost_success':False,'rendered':False,'playable':False,'emulator_launched':False}
(reports/'character-name-predicate-v48-verified.json').write_text(json.dumps(receipt,indent=2)+'\n')
doc='''# Character IsPlayer V48: original NAME field corrected and native verified

Original IsPlayer3a49f0 reads the live NAME CString30 buffer44 for type0. ObjectBase constructor independently creates archetype/gametype CString48 buffer5c. The earlier V41/V42 classification and actual archetype binding were wrong. This packet supersedes those semantic labels and bindings; their archived files remain historical evidence.

| Subtask | Status |
|---|---|
| Original constructor name30/buffer44 versus gametype48/buffer5c identity | Finished: whole ObjectBase C1/C2, GO0, three poisons each |
| Whole original IsPlayer name/gametype discriminators | Finished: 20 original cases |
| Actual checked actor alias borrows source_name_ | Finished: selected3file candidate equality verified |
| Real factory20 cases +20 live Name stores | Finished: coherent749 native PASS |
| Corrected same-actor spawn8 regression | Finished: coherent749 native PASS |
| Existing ordered NPC/Faery model regression | Finished: coherent749 native PASS; required38be5c remains |
| Qualified parent GameObject/whole Character activation | Pending separate V48 inherited-field/provider work |

The correction changes only three semantic files: actor is_player_fields_v41 lends its existing source_name_ cell; the pure predicate field is renamed live_name and its source comment/error identifies NAME30. Type!=0 retains type==1 and does not require a name. No new source storage, class/AI policy, model/template selection or save object is introduced.

The real factory test loads actual cache/design tables and selects existing AI rows as explicit predicate inputs in the same resolved sheet. It applies Name and gametype values through the existing PropertyMap, proves the returned pointer is the same source_name()/source_string(0x30), and observes later live Name stores through the original borrow. Gametype prefix cannot substitute for NAME. Missing type0 NAME preserves output; nonzero type bypasses NAME. RNG counts remain0.

The updated spawn regression corrects the historical fixture's0x48 archetype writes to original0x30 NAME stores. Its shared application/lifetime/failure cases still pass. The ordered model regression's NPC/nullFaery/ownedFaery responses remain unchanged and still stop at required whole GameObject.InitPost38be5c. Offline/lifecycle observers and model Debug/difficulty1/DEFAULT/master-save association are explicit fixtures.

Native proof is the parent's current coherent749 selected graph. Exact selected semantic source/test equality and native binary hashes were read back against coherent-loader-v49-integrated-verified.json. The serial isolated candidate runner is available for future changed-input investigations; a redundant compile was unnecessary after these same-source native checks passed. No emulator was launched.

This packet deliberately contains the NAME correction independently. The eight missing inherited scalar cells (seven ObjectBase plus idle-sound halfword) and ConditionList bridge remain separate work and are not adopted here.
'''
(reports/'character-name-predicate-v48-handoff.md').write_text(doc)
(r/'freeze_character_name_v48.py').write_bytes(Path(__file__).read_bytes())
files=[p for p in r.rglob('*') if p.is_file() and '__pycache__' not in p.parts]
for n in ['character-name-predicate-v48-verified.json','character-name-predicate-v48-handoff.md','character-name-predicate-v48-syntax.log','coherent-loader-v49-integrated-verified.json']:files.append(reports/n)
for n in ['0x33f15c.asm','0x33f310.asm']:files.append(b/'reference/character-parent-init-v48'/n)
packet=reports/'character-name-predicate-v48-corrected.zip';manifest={str(p.relative_to(b)).replace('\\','/'):sha(p) for p in files}
with zipfile.ZipFile(packet,'w',zipfile.ZIP_DEFLATED) as z:
 for p in files:z.write(p,str(p.relative_to(b)).replace('\\','/'))
 z.writestr('SHA256SUMS.json',json.dumps(manifest,indent=2)+'\n')
with zipfile.ZipFile(packet) as z:
 for n,h in manifest.items():assert hashlib.sha256(z.read(n)).hexdigest()==h,n
out={'schema':'dh2-character-name-predicate-v48-frozen','packet':str(packet),'packet_sha256':sha(packet),'members_verified':len(manifest),'native_proof':'selected coherent749 actual factory20/liveName20/spawn8/model','missing_parent_fields_bundled':False,'files':manifest}
(reports/'character-name-predicate-v48-frozen.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps({k:v for k,v in out.items() if k!='files'},indent=2))
