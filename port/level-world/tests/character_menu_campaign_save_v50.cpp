#include "../character_menu_campaign_save_v50.hpp"
#include "../private_save_file_transport_v45.hpp"
#include <fstream>
#include <iostream>
#include <cassert>
#include <unistd.h>
using namespace dh2;
static std::vector<std::uint8_t> file(const char* name){std::ifstream f(std::string("port/android-native/app/src/main/assets/data/")+name,std::ios::binary);assert(f);return {std::istreambuf_iterator<char>(f),{}};}
int main(){unsigned checks{};std::string e;
 auto a=file("character_properties_pyarray.bin"),b=file("character_properties_pyarraynames.bin"),c=file("character_properties_pystructnames.bin");data::CharacterTable characters;assert(data::load_characters({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},characters,e));
 a=file("skills_pyarray.bin");b=file("skills_pyarraynames.bin");c=file("skills_pystructnames.bin");data::SkillTables skills;assert(skills.load({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},e));
 a=file("loot_table_pyarray.bin");b=file("loot_table_pyarraynames.bin");c=file("loot_table_pystructnames.bin");data::LootTablesV2 loot;assert(loot.load({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},e));
 auto state=std::make_shared<data::PropertyState>();data::PropertyRules rules;assert(data::load_property_rules(characters,rules,e));auto view=data::property_view(rules,*state);state->saved[19]=512;
 data::LootRandom8V2 random{1,0};data::FreshInventoryOwnedV4 inventory(1,loot.borrow(),random,8,state);
 data::OwnedInventoryServicesV4 effects{nullptr,[](void*,auto&,const auto& q,auto& out,std::string&){out.value=0;if(q.operation==data::OwnedInventoryOperationV4::current_player)out.identity=1;return true;}}; // explicit item construction-effect fixture
 std::unique_ptr<data::ItemInstanceV1> potion;assert(inventory.create_item(data::item_id(inventory.table(),"Potion0"),3,potion,effects,e));int index{};assert(inventory.add_item(potion,true,false,index,effects,e));
 char folder[]="/tmp/dh2-menu-save-v50-XXXXXX";assert(::mkdtemp(folder));auto transport=std::make_shared<level::PrivateSaveFileTransportV45>(folder,1024*1024);auto files=transport->services();auto jobs=std::make_shared<level::SavegameJobsOwnerV2>(files);
 auto profile=std::make_shared<level::CampaignSaveProfileV45>("dh2_000.savegame",files,jobs);assert(profile->construct(e));
 auto save=std::make_shared<data::PlayerSavegameV1>();save->set_character(1);save->set_player_class(0);save->set_player_level(2);save->initialize_faeries();assert(save->initialize_skills(skills.borrow().lists().at(0),e));
 auto authority=std::make_shared<data::PlayerSaveLoadOwnerV1>(save);auto lease=std::make_shared<int>(1);std::vector<std::string> powers,levels,maps; // empty Level/Map collections explicitly fixture scope, not campaign initialization
 character::CharacterMenuCampaignSaveServicesV50 s;s.tables=lease;s.characters=&characters;s.skills=skills.borrow();s.power_names=&powers;s.level_names=&levels;s.map_names=&maps;
 s.current_difficulty=[](int& out,std::string&){out=0;return true;}; // actual-global selector fixture
 s.actor=[&](auto& out,auto&){out={lease,&view,&inventory};return true;};
 s.remaining={lease,[](const auto& q,auto& out,auto& e){if(q.operation==data::PlayerSaveWriteOpV1::online){out.flag=false;return true;}e="Required fixture online branch";return false;}};
 auto unbound=std::make_shared<character::CharacterMenuCampaignSaveV50>(authority,profile,s);assert(!unbound->bind(e));++checks;
 assert(authority->publish_profile(profile->receiver(),e));auto owner=std::make_shared<character::CharacterMenuCampaignSaveV50>(authority,profile,s);assert(owner->bind(e)&&owner->ready());++checks;assert(!owner->bind(e));++checks;
 data::PlayerSaveWriteOwnerV1 writer(authority,owner->write_services());assert(writer.save(e)&&jobs->pending()==2);++checks;assert(jobs->flush(nullptr,e)&&jobs->pending()==0);++checks;
 bool found{};std::vector<std::uint8_t> bytes;assert(files.read_file(files.context,"dh2_000.savegame",found,bytes,e)&&found);data::PlayerProfileIndexV1 restored;assert(restored.load({bytes.data(),bytes.size()},e));
 {auto cache=restored.borrow();assert(cache.source_sections().size()==15);++checks;for(const char* tag:{"PNAM","PLVL","PCLS","PDFL","LNAM","LEPT","LUSP","LVLS","SKIL","FAES","CFEE","QEST","PROP","GEAR","FTVL"}){assert(cache.payload(tag).data);++checks;}
  data::PlayerSavegameV1 other;other.set_character(1);assert(other.initialize_skills(skills.borrow().lists().at(0),e));std::size_t used{};assert(other.load_class(cache.payload("PCLS"),characters.names,used,e)&&other.class_id()==0);assert(other.load_level(cache.payload("PLVL"),used,e)&&other.level()==2);assert(other.load_skills(cache.payload("SKIL"),skills.borrow(),used,e)==0&&other.skills().size()==save->skills().size());checks+=3;
  data::ItemInventoryV1 items(inventory.table());items.set_character(1);items.project_potion_capacity(8);data::InventoryLoadReceiptV1 receipt;data::InventoryServicesV1 load{nullptr,[](void*,auto&,const auto&,auto&,std::string&){return true;}};assert(items.load_section(cache.payload("GEAR"),powers,load,receipt,e)&&receipt.completed&&items.items().size()==1&&items.items()[0]->item->quantity==3);++checks;
 }
 state->saved[19]=999;data::PlayerSaveLoadRequestV1 q{data::PlayerSaveLoadOpV1::load_section};q.save=save.get();q.profile=profile->receiver();q.section="PROP";data::PlayerSaveLoadResponseV1 out;assert(owner->load_property_section(q,out,e)&&state->saved[19]==512);++checks;
 save->set_player_level(3);assert(writer.save(e)&&jobs->flush(nullptr,e));assert(files.read_file(files.context,"dh2_000.savegame.bak",found,bytes,e)&&found);data::PlayerProfileIndexV1 backup;assert(backup.load({bytes.data(),bytes.size()},e));{auto cache=backup.borrow();data::PlayerSavegameV1 old;std::size_t used{};assert(old.load_level(cache.payload("PLVL"),used,e)&&old.level()==2);}++checks;
 q.save=nullptr;assert(!owner->load_property_section(q,out,e));++checks;
 // Attach the genuine menu PROP reader on SAME authority; no C1/default replay.
 auto identity=authority.get();auto before=save->location();
 auto actual_reader=owner->load_services_v50();
 data::PlayerSaveLoadServicesV1 reader{lease,[&,actual_reader](const auto& request,auto& response,auto& error){
  data::PlayerSaveLoadServicesV1 replacement{lease,[](const auto&,auto&,auto&){return true;}};
  std::string rejected;assert(!authority->bind_services_v50(replacement,rejected));++checks;
  return actual_reader.invoke(request,response,error);
 }};
 assert(authority->bind_services_v50(reader,e));state->saved[19]=777;assert(authority->load(0x20,e)&&state->saved[19]==512);assert(authority.get()==identity&&authority->profile().identity==profile->receiver().identity&&save->location().current_acts==before.current_acts);checks+=2;
 assert(!authority->bind_services_v50({},e));++checks;
 // Required actor removal is a failed save prefix, never an empty successful PROP/GEAR.
 s.actor=[](auto&,auto& e){e="Missing actual actor";return false;};auto removed=std::make_shared<character::CharacterMenuCampaignSaveV50>(authority,profile,s);assert(removed->bind(e));data::PlayerSaveWriteOwnerV1 failure(authority,removed->write_services());assert(!failure.save(e)&&e=="Missing actual actor");++checks;
 assert(jobs->release(e));for(const char* name:{"dh2_000.savegame","dh2_000.savegame.bak"})::unlink((std::string(folder)+"/"+name).c_str());assert(!::rmdir(folder));
 std::cout<<"Character menu whole15 sections/current same PROP reload/private-file backup PASS "<<checks<<"; actual cached Character/Skills/Item, source fields; offline/global and item effect fixtures; campaign Level/Quest initialization not claimed\n";
}
