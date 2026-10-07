#include "character_skills_owner_v6.hpp"
#define CharacterSkillOwnerV3 CharacterSkillOwnerV6
#include "character_skills_owner_v3.cpp"
#undef CharacterSkillOwnerV3
namespace dh2::character::skills {
int CharacterSkillOwnerV6::native_delete_skill(std::uint32_t index,std::uintptr_t instance){
 auto& t=*impl_;if(index>=t.slots[0].size()||reinterpret_cast<std::uintptr_t>(t.slots[0][index])!=instance)return -1;
 const auto at=std::find_if(t.instances.begin(),t.instances.end(),[&](const auto& p){return reinterpret_cast<std::uintptr_t>(&p->value)==instance;});
 if(at==t.instances.end()){t.error="V6 deleting destructor received unowned Skill";return -2;}
 t.instances.erase(at);return 1;
}
int CharacterSkillOwnerV6::native_reset_skill_end(){
 auto& t=*impl_;for(auto* p:t.slots[0])if(p)return -1;t.slots[0].clear();t.sync();return 1;
}
int CharacterSkillOwnerV6::native_destroy_skills(std::uint32_t* destroyed){
 if(!destroyed)return -1;*destroyed=0;auto& t=*impl_;
 // AI_ReloadSkills captures begin/end, then destroys each nullable skill in
 // ascending order. Owned script string + numeric argument are exactly the
 // source Arguments string/number payload; no Lua reference Value is owned.
 const auto count=t.slots[0].size();
 for(std::size_t i=0;i<count;++i){
  auto* value=t.slots[0][i];if(!value)continue;
  const auto at=std::find_if(t.instances.begin(),t.instances.end(),[&](const auto& p){return &p->value==value;});
  if(at==t.instances.end()){t.error="V6 deleting destructor received unowned Skill";return -2;}
  t.instances.erase(at); // Genuine owned Arguments/string/instance release.
  t.slots[0][i]=nullptr;++*destroyed;t.sync(); // AFTER deleting destructor.
 }
 t.slots[0].clear();t.sync(); // Source end=begin; capacity remains retained.
 return 1;
}
}
