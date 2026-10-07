#include "../character_skill_save_load_connection_v1.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh2::data;using namespace dh2::character::skills;
void check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
int main(){try{
 auto save=std::make_shared<PlayerSavegameV1>();save->set_character(0x100000009ULL);
 std::string error;check(save->initialize_skills({11,12},error),"initial source rows");
 check(save->set_skill_level(0,7,error),"existing development level");
 PlayerSaveLoadOwnerV1 load(save);auto list=std::make_shared<std::vector<std::int32_t>>(std::initializer_list<std::int32_t>{21,22,23});
 CharacterSkillSaveLoadConnectionV1 connection(load,list,[&](std::uintptr_t actor,const std::vector<std::int32_t>*& out,std::string&){check(actor==save->character(),"same actor");out=list.get();return true;});
 auto services=connection.services();SkillSaveReloadOutputV6 result{};
 check(dh2_character_skill_save_reload_v6(&result,save.get(),&services)==0,"composed source reload");
 check(result.phase==6&&result.deleted==1&&result.old_count==2&&result.new_count==3,"full reached native prefix");
 check(load.delivered_calls()==0&&!load.profile().identity,"genuine null profile no file callbacks");
 for(unsigned i=0;i<3;++i)check(save->skill_id(i)==(*list)[i]&&save->skill_level(i)==0,"actual selected rows reset");
 check(save->skill_slots()[0].empty()&&save->skill_slots()[1].empty(),"both source maps cleared");
 check(connection.load_mask(0x20,error)&&load.delivered_calls()==0,"outer source property load null guard");
 PlayerSavegameV1 other;check(services.load(services.context,&other,8)!=0,"different Save rejected");
 save->set_slot(0);check(!connection.load_mask(8,error)&&load.reached_phase()==1,"nonnegative slot requires actual filename provider");
 check(load.publish_profile({reinterpret_cast<std::uintptr_t>(list.get()),list},error),"explicit profile fixture");
 check(!connection.load_mask(8,error)&&load.reached_phase()==3,"existing profile requires real SKIL named section");
 std::cout<<"PASS same Save SG_ReloadSkills + real null-profile Load8/20; required filename and SKIL rejected\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
