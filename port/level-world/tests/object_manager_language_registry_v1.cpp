#include "../object_manager_language_registry_v1.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::ui;using namespace dh2::world;
void check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
struct Fixture {
 std::vector<std::uintptr_t> characters,objects,items;
 static int invoke(void* p,const SettingsSceneRequest16V1* q,std::uint32_t* result){auto& f=*static_cast<Fixture*>(p);*result=0;
  if(q->operation==SettingsSceneOperationV1::is_player){f.characters.push_back(q->identity);*result=q->identity==101;}
  if(q->operation==SettingsSceneOperationV1::is_merchant)*result=q->identity==103;
  if(q->operation==SettingsSceneOperationV1::is_game_object){f.objects.push_back(q->identity);*result=1;}
  if(q->operation==SettingsSceneOperationV1::refresh_item)f.items.push_back(q->identity);
  return 0;
 }
};
int main(){try{
 ObjectManagerLanguageRegistryV1 registry;auto owner=std::make_shared<int>(1);std::string error;std::uint32_t type_character=2,type_item=3,type_text=14;std::uint8_t text_valid=1;
 check(registry.scene().characters->next==registry.scene().characters&&registry.scene().object_first==registry.scene().object_end,"source constructor empty sentinels");
 check(registry.source_added({9,101,owner,&type_character,nullptr,101},error),"actual character registration fixture");
 check(registry.source_added({-1,102,owner,&type_item,nullptr,0},error),"actual item registration fixture");
 check(registry.source_added({4,103,owner,&type_character,nullptr,103},error),"actual second character fixture");
 check(registry.source_added({12,104,owner,&type_text,&text_valid,0},error),"actual text byte borrow fixture");
 Fixture fixture;SettingsSceneServices16V1 services{&fixture,Fixture::invoke};
 check(!dh2_settings_v2_refresh_language_scene(&registry.scene(),&services),"whole nonempty source traversal");
 check(fixture.characters==std::vector<std::uintptr_t>{101,103},"AsChar insertion order");
 check(fixture.objects==std::vector<std::uintptr_t>{102,103,101,104},"signed handle-key map successor order");
 check(fixture.items==std::vector<std::uintptr_t>{102}&&!text_valid,"type3 localization and canonical type14 byte invalidated");
 check(!registry.source_added({9,999,owner,&type_character,nullptr,999},error)&&registry.object_count()==4,"source duplicate handle requires owner removal");
 check(registry.source_removed(9,error)&&registry.character_count()==1&&registry.object_count()==3,"actual removed registration");
 type_item=14;fixture={};check(dh2_settings_v2_refresh_language_scene(&registry.scene(),&services)==-2,"live type14 requires genuine valid field");
 registry.source_flush();check(registry.object_count()==0&&registry.character_count()==0&&registry.scene().object_first==registry.scene().object_end,"source flush releases registrations");
 std::cout<<"PASS retained source registration/signed order/AsChar list/removal/full nonempty language traversal; object/predicate inputs declared fixtures\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
