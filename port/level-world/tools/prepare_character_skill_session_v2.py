"""Prepare separately versioned source copies; frozen V1 bytes are never edited.
The equivalence map records reversible changes, not a claim of new behavior parity.
"""
import hashlib,json,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];WORLD=ROOT/'port/level-world'
def sha(b):return hashlib.sha256(b).hexdigest()
def main():
 records=[]
 existing=WORLD/'reference/character-skill-session-v2/versioning-map.json'
 prior={r['path']:r['versioned_sha256']for r in json.loads(existing.read_text())['files']}if existing.exists()else{}
 def write(name,original,base,changes):
  result=base
  for old,new in changes:
   assert result.count(old)==1,(name,old)
   result=result.replace(old,new)
  target=WORLD/name
  if target.exists() and target.read_text()!=result and sha(target.read_bytes())!=prior.get(target.relative_to(ROOT).as_posix()):raise RuntimeError('Refusing overwrite of independently changed versioned source '+name)
  target.write_text(result)
  restored=result
  for old,new in reversed(changes):assert restored.count(new)==1;restored=restored.replace(new,old)
  assert restored==base
  records.append(dict(path=target.relative_to(ROOT).as_posix(),frozen_path=original.relative_to(ROOT).as_posix(),frozen_sha256=sha(original.read_bytes()),normalized_base_sha256=sha(base.encode()),versioned_sha256=sha(target.read_bytes()),reversible_changes=[dict(before=o,after=n)for o,n in changes]))
 def rename(s):return re.sub(r'\bScriptOwnerServices\b','ScriptOwnerServicesV2',re.sub(r'\bScriptOwner\b','ScriptOwnerV2',s))
 owner_h=WORLD/'character_script_owner.hpp';old=owner_h.read_text();body=old[old.index('class ScriptOwner {'):old.index('\n};',old.index('class ScriptOwner {'))+4]
 prefix='#pragma once\n#include "character_script_owner.hpp"\nnamespace dh2::character {\nclass ScriptOwnerV2;\nstruct ScriptOwnerServicesV2 {void* context=nullptr;int(*invoke)(void*,ScriptOwnerV2&,const ScriptOwnerRequest&,ScriptOwnerResponse&)=nullptr;};\n'
 write('character_script_owner_v2.hpp',owner_h,prefix+rename(body)+'\n}\n',[(
 ' // actual alias-membership InitVCB and source publication. External/default\n // VCB executes locally; unrecovered player-specific VCB remains a service.',
 ' // actual alias-membership InitVCB and source publication. External/default\n // and source AISPlayer/IPhone InitVCB execute on these owned callback flags.')])
 owner_c=WORLD/'character_script_owner.cpp';old=owner_c.read_text();body=old[old.index('namespace dh2::character {\nstruct ScriptOwner::Impl'):]
 prefix='#include "character_script_owner_v2.hpp"\n#include "character_script_virtual.hpp"\n#include "character_script_player_vcb_v2.hpp"\n#include <cstring>\n#include <map>\n#include <set>\n#include <utility>\n#include <stdexcept>\nextern "C" int dh2_script_alias_clear_contents(dh2_script_aliases*);\nnamespace {struct Failure {int status;};}\n'
 old_dispatch='}else{const auto view=s->view();t.call(owner_init_vcb,0,0,r->subject,&view);}'
 new_dispatch='}else if(s->kind==script_player||s->kind==script_player_iphone){\n     if(dh2_character_script_player_vcb_v2(&s->callback_flags,s->aliases))throw Failure{-1};\n    }else{const auto view=s->view();t.call(owner_init_vcb,0,0,r->subject,&view);}'
 old_public='  struct Restore{Impl& impl;const ScriptOwnerServicesV2* old;~Restore(){impl.services=old;}} restore{t,t.services};\n  t.services=&services;const auto view=hold->view();t.call(owner_init_vcb,0,0,identity,&view);return 0;'
 new_public='  if(hold->kind==script_player||hold->kind==script_player_iphone)\n   return dh2_character_script_player_vcb_v2(&hold->callback_flags,hold->aliases);\n'+old_public
 write('character_script_owner_v2.cpp',owner_c,prefix+rename(body),[(old_dispatch,new_dispatch),(old_public,new_public)])
 session_h=WORLD/'character_script_session.hpp';old=session_h.read_text();body=old[old.index('class CharacterScriptSession {'):]
 body=re.sub(r'\bCharacterScriptSessionInput\b','CharacterScriptSessionInputV2',re.sub(r'\bCharacterScriptSession\b','CharacterScriptSessionV2',re.sub(r'\bScriptOwner\b','ScriptOwnerV2',body)))
 prefix='#pragma once\n#include "character_script_session.hpp"\n#include "character_script_owner_v2.hpp"\n#include "character_skill_properties_v1.hpp"\n#include "character_current_skill_v2.hpp"\nnamespace dh2::character {\nstruct CharacterScriptSessionInputV2:CharacterScriptSessionInput {\n void* property_context=nullptr;\n int(*external_property)(void*,std::uintptr_t,std::int32_t**)=nullptr;\n int(*recalculate_properties)(void*,data::PropertyView*)=nullptr;\n int(*normal_property_class)(void*,data::PropertyView*,std::int32_t)=nullptr;\n data::SkillTables::Borrow skill_tables;\n std::shared_ptr<data::PlayerSavegameV1> savegame;\n void* skill_number_context=nullptr;\n int(*skill_number)(void*,const dh2_script_value*,float*)=nullptr;\n};\n'
 write('character_script_session_v2.hpp',session_h,prefix+body,[])
 session_c=WORLD/'character_script_session.cpp';base=session_c.read_text().replace('"character_script_session.hpp"','"character_script_session_v2.hpp"')
 base=re.sub(r'\bCharacterScriptSessionInput\b','CharacterScriptSessionInputV2',re.sub(r'\bCharacterScriptSession\b','CharacterScriptSessionV2',rename(base)))
 changes=[(' PropertyBindings48 property_bindings{};',' PropertyBindings48 property_bindings{};\n skills::SkillPropertyBindingsV1 skill_properties{};\n data::SkillTables::Borrow skill_tables;\n std::shared_ptr<data::PlayerSavegameV1> savegame;\n skills::CurrentSkillBindingsV2 current_skill{};'),
 ('  property_bindings={{properties->resolved.data(),224,0},{temporary?temporary->data():nullptr,temporary?224u:0u,0},nullptr,nullptr};','  property_bindings={{properties->resolved.data(),224,0},{temporary?temporary->data():nullptr,temporary?224u:0u,0},nullptr,nullptr};\n  skill_properties={&property_view,temporary.get(),design.classes(),in.property_context,in.external_property,in.recalculate_properties,in.normal_property_class};\n  skill_tables=in.skill_tables;savegame=in.savegame;\n  current_skill={&property_view,&skill_tables,savegame.get(),in.skill_number_context,in.skill_number};'),
 ('  else if(b.original_callback==0x3b9d8c){function=&dh2_character_get_prop;context=&property_bindings;}','  else if(b.original_callback==0x3b82e4){function=&skills::skill_clear_properties_v1;context=&skill_properties;}\n  else if(b.original_callback==0x3b7eac){function=&skills::skill_set_property_v1;context=&skill_properties;}\n  else if(b.original_callback==0x3babdc){function=&skills::skill_apply_class_v1;context=&skill_properties;}\n  else if(b.original_callback==0x3b8f9c){function=&skills::current_skill_info_v2;context=&current_skill;}\n  else if(b.original_callback==0x3b9d8c){function=&dh2_character_get_prop;context=&property_bindings;}')]
 write('character_script_session_v2.cpp',session_c,base,changes)
 for name in ('character_skills_session','character_skill_info_session_v1'):
  out=name.replace('_v1','')+'_v2'
  for suffix in ('.hpp','.cpp'):
   original=WORLD/(name+suffix);base=original.read_text()
   base=base.replace('"'+name+'.hpp"','"'+out+'.hpp"').replace('"character_script_session.hpp"','"character_script_session_v2.hpp"')
   for a,b in [('CharacterScriptSession','CharacterScriptSessionV2'),('CharacterSkillSessionServices','CharacterSkillSessionServicesV2'),('SkillInfoSessionsV1','SkillInfoSessionsV2'),('skill_info_session_invoke_v1','skill_info_session_invoke_v2')]:base=re.sub(r'\b'+a+r'\b',b,base)
   write(out+suffix,original,base,[])
 ref=WORLD/'reference/character-skill-session-v2';ref.mkdir(parents=True,exist_ok=True)
 (ref/'versioning-map.json').write_text(json.dumps(dict(validation='PASS',files=records,scope='Normalized frozen source copies plus listed reversible V2 changes. Existing helper/catalogue exports are reused. New behavior needs separate original/native and lifetime proofs.'),indent=2)+'\n')
 print(json.dumps(dict(validation='PASS',versioned_files=len(records))))
if __name__=='__main__':main()
