#include "../character_npc_animation_set_v6.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
static unsigned checks;
static void check(bool value,const std::string& text){++checks;if(!value)throw std::runtime_error(text);}
static std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error(path);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){try{
 check(argc==2,"cache directory");std::string directory=argv[1],error;
 auto names=read(directory+"/animations_dictionary_pyarraynames.bin"),values=read(directory+"/animations_dictionary_pyarray.bin");data::Dictionary dictionary;
 check(data::load_dictionary({names.data(),names.size()},{values.data(),values.size()},dictionary,error),error);
 auto records=read(directory+"/animations_pyarray.bin"),keys=read(directory+"/animations_pyarraynames.bin"),fields=read(directory+"/animations_pystructnames.bin");data::AnimationTables tables;
 check(data::load_animation_tables({records.data(),records.size()},{keys.data(),keys.size()},{fields.data(),fields.size()},dictionary,tables,error),error);
 std::ifstream gold(directory+"/registration-gold.txt");unsigned cases=0;int table,set,count;
 while(gold>>table>>set>>count){std::vector<int> expected(count);for(auto& id:expected)gold>>id;std::vector<int> actual;unsigned preloads=0;
  character::NpcAnimationRegistrationServicesV6 services;services.sound_manager_present=true;services.invoke=[&](const auto& q,auto&){check(q.set_id==set,"same set identity");using O=character::NpcAnimationRegistrationOperationV6;if(q.operation==O::add_animation||q.operation==O::add_template)actual.push_back(q.resource);if(q.operation==O::preload_fx)++preloads;return true;};
  check(character::character_npc_register_animation_set_v6(tables,table,set,{},services,error),error);check(actual==expected,"exact original ARM registration occurrence sequence");
  unsigned delivered=0;services.invoke=[&](const auto&,auto& e){++delivered;e="required source delivery diagnostic";return false;};check(!character::character_npc_register_animation_set_v6(tables,table,set,{},services,error)&&delivered==1&&error=="required source delivery diagnostic","mandatory failure retains first source prefix");++cases;
 }
 check(cases==4,"four original gold families");unsigned families=0;
 for(unsigned i=0;i<tables.characters.size();++i){bool template_present=false;character::NpcAnimationRegistrationServicesV6 services;services.invoke=[&](const auto& q,auto&){if(q.operation==character::NpcAnimationRegistrationOperationV6::add_template)template_present=true;return true;};check(character::character_npc_register_animation_set_v6(tables,int(i),int(i<<8),{},services,error),error);families+=template_present;}
 check(families>50,"actual general NPC animation family census");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_gold_families\":"<<cases<<",\"registered_table_families\":"<<families<<"}"<<std::endl;
 }catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
