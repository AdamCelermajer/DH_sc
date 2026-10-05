#include "../character_skill_save_reload_v6.hpp"
using namespace dh2::character::skills;using namespace dh2::data;
// Differential-only caller inputs and observers. Neither callback is offered
// as a production Save.Load implementation or source file provider.
struct SaveFixtureV6 {
 PlayerSavegameV1 save;std::vector<std::int32_t> selected;
 void* context{};int(*observe)(void*,unsigned,PlayerSavegameV1*,unsigned){};
 static int list(void* p,PlayerSavegameV1* s,std::uintptr_t,const std::vector<std::int32_t>** out){auto& f=*static_cast<SaveFixtureV6*>(p);if(f.observe(f.context,1,s,0))return -1;*out=&f.selected;return 0;}
 static int load(void* p,PlayerSavegameV1* s,unsigned mask){auto& f=*static_cast<SaveFixtureV6*>(p);return f.observe(f.context,2,s,mask);}
};
extern "C" SaveFixtureV6* dh2_skill_save_fixture_create_v6(const std::int32_t* old_ids,unsigned old_count,const std::int32_t* selected,unsigned count){
 auto* f=new SaveFixtureV6;f->save.set_character(0x100000009ULL);std::string error;
 std::vector<std::int32_t> ids(old_ids,old_ids+old_count);if(!f->save.initialize_skills(ids,error))return nullptr;
 for(unsigned i=0;i<old_count;++i)f->save.set_skill_level(i,int(i+7),error);
 SavedSkillUpdateServicesV1 fixture{nullptr,[](void*,std::uintptr_t,std::string&){return true;}};
 if(old_count)f->save.set_skill_in_slot(2,0,fixture,error);
 f->selected.assign(selected,selected+count);return f;
}
extern "C" void dh2_skill_save_fixture_snapshot_v6(SaveFixtureV6* f,std::int32_t* out){
 out[0]=f->save.skills_initialized();out[1]=f->save.skills().size();out[2]=f->save.skill_slots()[0].size();out[3]=f->save.skill_slots()[1].size();
 for(unsigned i=0;i<f->save.skills().size();++i){auto& r=f->save.skills()[i];out[4+i*3]=r.id;out[5+i*3]=r.level;out[6+i*3]=r.flag;}
}
extern "C" int dh2_skill_save_fixture_reload_v6(SaveFixtureV6* f,void* context,int(*observe)(void*,unsigned,PlayerSavegameV1*,unsigned)){
 f->context=context;f->observe=observe;SkillSaveReloadServicesV6 services{f,SaveFixtureV6::list,SaveFixtureV6::load};SkillSaveReloadOutputV6 out{};return dh2_character_skill_save_reload_v6(&out,&f->save,&services);
}
extern "C" void dh2_skill_save_fixture_destroy_v6(SaveFixtureV6* f){delete f;}
