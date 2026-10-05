"""Use the same differential-proven coordinator in the V6 retained owner."""
from pathlib import Path
root=Path(__file__).resolve().parents[3];base=root/'port/level-world'
payload='''int CharacterPlayerSkillsV6::native_reload_skills(const SkillSaveReloadServicesV6* services){
 if(!ready())return -2;
 struct Reload {
  Impl& t;const SkillSaveReloadServicesV6* save_services;
  static int invoke(void* p,State40* state,const SkillReloadRequestV6* q){
   auto& r=*static_cast<Reload*>(p);auto& t=r.t;
   if(state!=&t.owner->state()||q->character!=t.session->timers().owner)return -1;
   if(q->service==skill_reload_delete_v6)return t.owner->native_delete_skill(q->index,q->instance)==1?0:-1;
   if(q->service==skill_reload_reset_end_v6)return t.owner->native_reset_skill_end()==1?0:-1;
   if(q->service==skill_reload_save_v6){
    if(!t.saved)return 0; // Original Character.SG_ReloadSkills null-Save guard.
    SkillSaveReloadOutputV6 reloaded{};
    if(dh2_character_skill_save_reload_v6(&reloaded,t.saved.get(),r.save_services)){
     t.error="Required SAME Save SG_ReloadSkills failed at phase "+std::to_string(reloaded.phase);return -1;}return 0;
   }
   const auto status=q->service==skill_reload_configure_v6?t.owner->configure():q->service==skill_reload_update_v6?t.owner->update():-1;
   if(status!=1){t.error=t.owner->error()+t.service->error();return -1;}return 0;
  }
 }reload{*impl_,services};
 SkillReloadServicesV6 providers{&reload,Reload::invoke};SkillReloadOutputV6 result{};
 const auto code=dh2_character_skill_reload_v6(&result,const_cast<State40*>(&impl_->owner->state()),&providers);
 if(code!=1){if(impl_->error.empty())impl_->error="Required native AI_ReloadSkills failed at phase "+std::to_string(result.phase);return -2;}
 impl_->error.clear();return 1;
}
'''
for name in ['character_player_skills_v6.cpp','tools/prepare_character_player_skills_v6.py']:
 path=base/name;text=path.read_text();start=text.index('int CharacterPlayerSkillsV6::native_reload_skills(');end=text.index('\nint CharacterPlayerSkillsV6::native_delete_buff',start)
 text=text[:start]+payload.rstrip()+text[end:]
 if '#include "character_skill_reload_v6.hpp"' not in text:text=text.replace('#include "character_skills_owner_v6.hpp"','#include "character_skills_owner_v6.hpp"\n#include "character_skill_reload_v6.hpp"',1)
 path.write_text(text)
