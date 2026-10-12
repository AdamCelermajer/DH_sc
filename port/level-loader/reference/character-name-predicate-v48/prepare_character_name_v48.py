from pathlib import Path
import json,hashlib,difflib
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');v=b/'vendor/character-rng-integration-v5-loading/port/level-world';ref=b/'reference/character-name-predicate-v48';ref.mkdir(exist_ok=True)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
names=['retained_character_actor_v1.hpp','retained_character_actor_v1.cpp','character_is_player_v41.hpp','character_is_player_v41.cpp','canonical_character_family_v4.hpp','canonical_character_family_v4.cpp','character_model_route_v45.hpp','character_model_route_v45.cpp']
before={n:sha(v/n) for n in names}
for n in names:(ref/n).write_bytes((v/n).read_bytes())
p=ref/'retained_character_actor_v1.cpp';s=p.read_text();old='out={std::move(lease),identity_,&source_archetype_};error.clear();return true;';new='out={std::move(lease),identity_,&source_name_};error.clear();return true;';assert s.count(old)==1;p.write_text(s.replace(old,new))
for n in ['character_is_player_v41.hpp','character_is_player_v41.cpp']:
 p=ref/n;s=p.read_text().replace('live_archetype','live_name').replace('Archetype is read only','Name CString30/buffer44 is read only').replace('Character archetype CString','Character name CString30');p.write_text(s)
patch=''
for n in ['retained_character_actor_v1.cpp','character_is_player_v41.hpp','character_is_player_v41.cpp']:
 patch+=''.join(difflib.unified_diff((v/n).read_text().splitlines(True),(ref/n).read_text().splitlines(True),fromfile='a/port/level-world/'+n,tofile='b/port/level-world/'+n))
(ref/'character-name-predicate-v48-adoption.patch').write_text(patch)
proof=json.loads((b/'reference/character-parent-init-v48/character-parent-init-original-v48.json').read_text())
out={'schema':'dh2-character-name-predicate-original-v48','original_elf_sha256':proof['original_elf_sha256'],'validation':'PASS whole original source name identity','ctor_scope':proof['scope'],'constructor_name_identity':[{'constructor':x['constructor'],'GO_id':x['source_GO_id'],'poison':x['poison'],'name_CString30_begin44':x['name_CString30_begin44'],'archetype_CString48_begin5c':x['archetype_CString48_begin5c']} for x in proof['whole_ObjectBase_C1_C2_cases']],'cases':proof['whole_IsPlayer_name_vs_archetype_cases'],'correction':proof['correction'],'earlier_receipts':'V41 original bitwise predicate results remain valid for artificial buffer44 inputs; archetype classification and V42 actor actual field binding are retracted. Nonzero-type branch unchanged. V45 tested positive types/names follow the same branch, but rerun with corrected actual borrow is required.'}
(ref/'character-name-predicate-original-v48.json').write_text(json.dumps(out,indent=2)+'\n')
(ref/'character_name_predicate_v48_setup.cpp').write_bytes((b/'reference/character-model-route-v45/character_model_route_v45_setup.cpp').read_bytes())
(ref/'prepare_character_name_v48.py').write_bytes(Path(__file__).read_bytes())
(b/'reports/character-name-predicate-v48-prepared.json').write_text(json.dumps({'status':'Original proof PASS; actual-factory20 test pending coherent parent READY','baseline_selected':before,'candidate':{n:sha(ref/n) for n in names},'patch_sha256':sha(ref/'character-name-predicate-v48-adoption.patch'),'selected_writes':False,'missing7field_candidate_bundled':False},indent=2)+'\n')
print(json.dumps({'prepared':str(ref),'patch_sha256':sha(ref/'character-name-predicate-v48-adoption.patch'),'changed_files':3,'model_route_source_preserved':True}))
