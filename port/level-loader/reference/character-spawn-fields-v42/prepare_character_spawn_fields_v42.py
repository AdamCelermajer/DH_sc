from pathlib import Path
import shutil,json,hashlib,difflib
base=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');source=base/'vendor/character-rng-integration-v5-loading/port/level-world';ref=base/'reference/character-spawn-fields-v42';ref.mkdir(exist_ok=True);temp=Path('C:/Users/adamc/AppData/Local/Temp');h=(source/'retained_character_actor_v1.hpp').read_text();c=(source/'retained_character_actor_v1.cpp').read_text();nh=h;nc=c
def one(s,a,b):assert s.count(a)==1,a;return s.replace(a,b)
nh=one(nh,'#include "character_model_name_v38.hpp"','#include "character_model_name_v38.hpp"\n#include "character_is_player_v41.hpp"')
nh=one(nh,'// Native retained ownership extracted from the renderer.','struct CharacterSpawnFieldsV42 {\n std::shared_ptr<const void> receiver_lease;std::uintptr_t identity{};\n std::int32_t* cached_roll270{};const std::int32_t* probability274{};\n};\n// Native retained ownership extracted from the renderer.')
nh=one(nh,' std::int32_t spawn_probability_{};bool spawn_probability_written_{};',' std::int32_t source_cached_spawn270_{-1}; // original GameObject C2 store38c4a8\n std::int32_t spawn_probability_{};bool spawn_probability_written_{};')
nh=one(nh,' bool loader_fields_v38(std::shared_ptr<const void>,CharacterLoaderFieldsV38&,std::string&);',' bool spawn_fields_v42(std::shared_ptr<const void>,CharacterSpawnFieldsV42&,std::string&);\n bool is_player_fields_v41(std::shared_ptr<const void>,CharacterIsPlayerFieldsV41&,std::string&);\n bool loader_fields_v38(std::shared_ptr<const void>,CharacterLoaderFieldsV38&,std::string&);')
methods='''bool RetainedCharacterActorV1::spawn_fields_v42(std::shared_ptr<const void> lease,CharacterSpawnFieldsV42& out,std::string& error){
 if(!lease||lease.get()!=static_cast<const void*>(this)||!loader_fields_produced_v38_||!spawn_probability_written_){error="Required SAME fresh Character cache270 alias and actual property-produced probability274";return false;}
 out={std::move(lease),identity_,&source_cached_spawn270_,&spawn_probability_};error.clear();return true;
}
bool RetainedCharacterActorV1::is_player_fields_v41(std::shared_ptr<const void> lease,CharacterIsPlayerFieldsV41& out,std::string& error){
 if(!lease||lease.get()!=static_cast<const void*>(this)||!loader_fields_produced_v38_){error="Required SAME fresh Character IsPlayer receiver alias authority";return false;}
 out={std::move(lease),identity_,&source_archetype_};error.clear();return true;
}
'''
assert nc.endswith('}\n');nc=nc[:-2]+methods+'}\n';(ref/'retained_character_actor_v1.hpp').write_text(nh);(ref/'retained_character_actor_v1.cpp').write_text(nc)
for name in ['canonical_character_family_v4.hpp','canonical_character_family_v4.cpp']:shutil.copyfile(source/name,ref/name)
family=(source/'canonical_character_family_v4.cpp').read_text();new_family=one(family,' if(q.source_entry==0x3a49f0){const auto* row=ai(r);if(!row){e="Required actual Character GetCharType";return false;}response.value=row->type==1;return true;}',''' if(q.source_entry==0x3a49f0){
  character::CharacterIsPlayerFieldsV41 fields;if(!r.actor->is_player_fields_v41(r.actor,fields,e))return false;
  character::CharacterIsPlayerServicesV41 services;services.actual_get_char_type=[&r](std::uintptr_t identity,std::int32_t& type,std::string& error){
   if(identity!=r.actor->shared_handle().cached){error="Required SAME Character GetCharType receiver";return false;}
   const auto* row=ai(r);if(!row){error="Required actual Character AI row/fallback8";return false;}type=row->type;return true;
  };
  bool player{};if(!character::character_is_player_v41(fields,services,player,e))return false;response.value=player;return true;
 }''');(ref/'canonical_character_family_v4.cpp').write_text(new_family)
for name in ['character_is_player_v41.hpp','character_is_player_v41.cpp']:shutil.copyfile(base/'reference/character-init-model-v41'/name,ref/name)
patch=''.join(difflib.unified_diff(h.splitlines(True),nh.splitlines(True),fromfile='a/port/level-world/retained_character_actor_v1.hpp',tofile='b/port/level-world/retained_character_actor_v1.hpp'))+''.join(difflib.unified_diff(c.splitlines(True),nc.splitlines(True),fromfile='a/port/level-world/retained_character_actor_v1.cpp',tofile='b/port/level-world/retained_character_actor_v1.cpp'))+''.join(difflib.unified_diff(family.splitlines(True),new_family.splitlines(True),fromfile='a/port/level-world/canonical_character_family_v4.cpp',tofile='b/port/level-world/canonical_character_family_v4.cpp'));(ref/'retained-character-spawn-v42-adoption.patch').write_text(patch)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();receipt={'schema':'dh2-character-spawn-fields-v42-proposal','status':'CANDIDATE ONLY; selected V5 unchanged','initial_hashes':{str(source/n):sha(source/n) for n in ['retained_character_actor_v1.hpp','retained_character_actor_v1.cpp','canonical_character_family_v4.hpp','canonical_character_family_v4.cpp']},'candidate_hashes':{str(p.relative_to(base)):sha(p) for p in ref.iterdir() if p.is_file()},'original_C1_cache_store':'38c4a8 cached270=-1; original unconditional producer-prefix3poisons in V41','no_base_added':True,'single_cache270':True,'probability_reuses_actual_existing_member':True,'adopted_state_unavailable_without_authority_transfer':True,'scope':'One cache field and checked actual alias/probability/live archetype borrows only. No model/template selection, stage reorder, NPC/player behavior/save overrides.'};(base/'reports/character-spawn-fields-v42-proposal.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps(receipt,indent=2))
