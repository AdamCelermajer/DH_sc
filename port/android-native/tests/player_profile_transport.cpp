#include "../app/src/main/cpp/native_player_profile.hpp"
#include "../app/src/main/cpp/player_inventory_save_commit_v1.hpp"
#include "../app/src/main/cpp/native_quest_owner.hpp"
#include "../../game-data/player_saved_skill_slots_v1.hpp"
#include "../../game-data/player_faery_change_v1.hpp"
#include "character_saved_class_v1.hpp"
#include "properties.hpp"
#include "level_tables.hpp"
#include "world_map_tables.hpp"
#include "skill_tables.hpp"
#include "loot_tables_v2.hpp"
#include "item_power_tables_v5.hpp"
#include "player_profile_filename_v1.hpp"
#include "../../game-data/quest_current_selection_v1.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

using namespace dh2::data;
namespace data=dh2::data;
namespace profile=dh2::native::player_profile;
namespace saved_class=dh2::character_saved_class_v1;
using Raw=std::vector<std::uint8_t>;
namespace {
unsigned checks=0;
void check(bool value,int line){if(!value)throw std::runtime_error("transport check "+std::to_string(checks+1)+" at line "+std::to_string(line));++checks;}
#define require(...) check((__VA_ARGS__),__LINE__)
Raw read(const std::filesystem::path& path){std::ifstream f(path,std::ios::binary);require(bool(f));return Raw(std::istreambuf_iterator<char>(f),{});}
Bytes bytes(const Raw& raw){return {raw.data(),raw.size()};}
void put(Raw& raw,std::uint32_t value){for(unsigned i=0;i<4;++i)raw.push_back(std::uint8_t(value>>(8*i)));}
Raw words(std::initializer_list<std::uint32_t> values){Raw out;for(auto v:values)put(out,v);return out;}
Raw text(const std::string& s){Raw out;put(out,std::uint32_t(s.size()+1));out.insert(out.end(),s.begin(),s.end());out.push_back(0);return out;}
Raw campaign(const std::string& class_name,const Raw& prop,bool include_level=true){
 std::vector<std::pair<std::string,Raw>> sections={
  {"PNAM",text("TransportFixture")},{"PLVL",words({17})},{"PCLS",text(class_name)},
  {"PDFL",words({1,2})},{"LNAM",words({0x10002,1,2,3,4,5,6,7,8,9})},
  {"LEPT",words({4,5,6})},{"LUSP",Raw{1,0,1}},{"PROP",prop}};
 if(!include_level)sections.erase(sections.begin()+1);
 Raw out;put(out,std::uint32_t(sections.size()));
 for(const auto& row:sections){put(out,std::uint32_t(row.second.size()));out.insert(out.end(),row.first.begin(),row.first.end());out.insert(out.end(),row.second.begin(),row.second.end());}
 return out;
}
void write(const std::filesystem::path& path,const Raw& raw){std::ofstream f(path,std::ios::binary|std::ios::trunc);require(bool(f));f.write(reinterpret_cast<const char*>(raw.data()),std::streamsize(raw.size()));require(bool(f));}
struct Query {unsigned calls=0;std::uintptr_t character;};
int is_player(void* raw,std::uintptr_t id,std::uint32_t* value,std::string&){auto& q=*static_cast<Query*>(raw);require(id==q.character);++q.calls;*value=1;return 0;}
struct GameplayPayload {const PlayerSavegameV1* save=nullptr;std::array<char,4> tag{};unsigned calls=0;};
bool write_gameplay_payload(void* raw,const PlayerSavegameV1& save,Raw& out,std::string& error){
 auto& provider=*static_cast<GameplayPayload*>(raw);if(provider.save!=&save){error="provider received a different canonical Save";return false;}
 ++provider.calls;out.assign(provider.tag.begin(),provider.tag.end());error.clear();return true;
}
struct GameplayAssemblyTables {
 SkillTables skills;LevelTables levels;WorldMapTables world_map;
 LootTablesV2 loot;ItemPowerTablesV5 powers;
 quest_table_bindings_v1::Owner quest_owner;
 quest_table_bindings_v1::View quests;
 dh2::native::quests::Constants constants;
 std::shared_ptr<Raw> quest_constants;
};
void load_gameplay_assembly_tables(const std::filesystem::path& cache,
                                  GameplayAssemblyTables& tables){
 std::string error;
 const auto load_three=[&](const char* prefix,auto& owner,auto load){
  const auto data=read(cache/(std::string(prefix)+"_pyarray.bin"));
  const auto names=read(cache/(std::string(prefix)+"_pyarraynames.bin"));
  const auto schema=read(cache/(std::string(prefix)+"_pystructnames.bin"));
  require(load(bytes(data),bytes(names),bytes(schema),owner,error));
 };
 load_three("skills",tables.skills,load_skill_tables);
 load_three("levels",tables.levels,load_levels);
 load_three("worldmap",tables.world_map,load_world_map);
 load_three("loot_table",tables.loot,[](Bytes a,Bytes b,Bytes c,LootTablesV2& target,std::string& e){return target.load(a,b,c,e);});
 const auto item_power_dir=cache.parent_path()/"original-cache"/"data"/"pydata";
 const auto power_records=read(item_power_dir/"item_powers_pyarray.bin");
 const auto power_names=read(item_power_dir/"item_powers_pyarraynames.bin");
 const auto power_schema=read(item_power_dir/"item_powers_pystructnames.bin");
 require(tables.powers.load(bytes(power_records),bytes(power_names),bytes(power_schema),error));
 const auto quest_records=std::make_shared<Raw>(read(cache/"v2quests_pyarray.bin"));
 const auto quest_names=std::make_shared<Raw>(read(cache/"v2quests_pyarraynames.bin"));
 quest_table_bindings_v1::Input quest_input;
 require(!dh2_quests_open(&quest_input.table,quest_records->data(),std::uint32_t(quest_records->size())));
 quest_input.packed_owner=quest_records;quest_input.names=quest_names->data();
 quest_input.names_size=quest_names->size();quest_input.names_owner=quest_names;
 require(tables.quest_owner.load(quest_input,error));tables.quests=tables.quest_owner.borrow();
 tables.quest_constants=std::make_shared<Raw>(read(cache/"v2quests_pycst.bin"));
 require(!dh2_pycst_open(&tables.constants.view,tables.quest_constants->data(),
                         std::uint32_t(tables.quest_constants->size())));
 tables.constants.owner=tables.quest_constants;
}
bool no_random(void*,std::int32_t bound,std::uint32_t,std::int32_t& value,std::string& error){
 if(bound<=0){error="positive source random bound required";return false;}value=0;return true;
}
bool unused_inventory_effect(void*,FreshInventoryOwnedV4&,
 const OwnedInventoryRequestV4&,OwnedInventoryResponseV4& response,std::string& error){
 response={};error.clear();return true;
}
void unused_inventory_observer(void*,FreshInventoryOwnedV4&,
 const OwnedInventoryRequestV4&){}
void gameplay_assembly_case(const std::filesystem::path& root,
 const std::filesystem::path& cache,const std::string& class_name,
 const CharacterTable& characters,const PropertyRules& property_rules){
 GameplayAssemblyTables tables;std::string error;
 load_gameplay_assembly_tables(cache,tables);

 const auto dir=root/"gameplay-save-assembled";std::filesystem::create_directories(dir);
 constexpr std::int32_t slot=9;const auto primary=dir/data::player_profile_filename_v1(slot,false,false);
 const auto original=campaign(class_name,Raw{});write(primary,original);
 auto save=std::make_shared<PlayerSavegameV1>();save->set_character(UINT64_C(0x77770009));save->set_slot(slot);
 PlayerSaveProfileV1 profile;std::int32_t difficulty=0;std::uint8_t online=0;
 const auto class_row=std::size_t(std::find(characters.names.begin(),characters.names.end(),class_name)-
                                  characters.names.begin());
 require(class_row<characters.rows.size());
 PropertyState properties;reset_properties(property_rules,properties,&characters.rows[class_row]);
 // This fixture selects actual SkillList row 0, whose single source entry
 // avoids conflating duplicate Skill identifiers during load-by-ID.
 properties.resolved[28]=0;
 properties.resolved[33]=0x1000;
 auto inventory=std::make_unique<FreshInventoryOwnedV4>(save->character(),
   tables.loot.borrow(),InventoryRandomServiceV4{nullptr,no_random},0,properties);
 auto effect_lease=std::make_shared<std::uint8_t>(0);
 OwnedInventoryServicesV4 inventory_services{effect_lease.get(),unused_inventory_effect,
                                               unused_inventory_observer,true};
 auto quest_owner=std::make_shared<dh2::native::quests::Owner>(save,tables.quests,tables.constants);
 profile::Transport transport(*save,profile);
 profile::TransportBindings bindings{dir,&characters,&difficulty,{},false,
   &tables.levels,&tables.world_map,quest_owner,&online};
 bindings.skill_tables=&tables.skills;bindings.skill_tree_selector=&properties.resolved[28];
 bindings.property_rules=&property_rules;bindings.properties=&properties;
 bindings.inventory=inventory.get();bindings.item_powers=tables.powers.borrow();
 bindings.inventory_services=inventory_services;bindings.inventory_services_owner=effect_lease;
 std::unique_ptr<ItemInstanceV1> incoming;bindings.inventory_incoming=&incoming;
 require(transport.bind(std::move(bindings),error));
 require(transport.loader().load(1,error));
 properties.resolved[28]=0;
 require(save->initialize_skills(tables.skills,properties.resolved[28],error));
 require(transport.loader().load(2,error));
 require(transport.loader().load(4,error));
 const auto save_identity=save.get();const auto profile_identity=profile.identity;
 auto* loader=&transport.loader();const auto old_view=profile.campaign;
 require(old_view&&old_view.bytes()==original&&quest_owner->owns_save(save.get()));
 auto* first_quest=quest_owner->resolve(save->source_quest_log_b8().quests[0][0]);
 require(first_quest&&first_quest->action_18);
 dh2::native::quests::QuestReadV1 selected_quest;
 require(quest_owner->read_quest(0,0,0,selected_quest,error));
 auto* saved_action=first_quest->action_18;first_quest->action_18=nullptr;
 require(!transport.save_gameplay(error)&&read(primary)==original&&
         profile.campaign.bytes()==original&&error.find("Native QEST")!=std::string::npos);
 first_quest->action_18=saved_action;
 require(dh2::data::quest_current_selection_v1::set_and_persist(
   *save,false,0,selected_quest.id,
   [&](PlayerSavegameV1& selected,std::string& save_error){
    if(&selected!=save.get()||selected.character()!=save->character()){
     save_error="quest selection persistence crossed canonical Save";return false;
    }
    return transport.save_gameplay(save_error);
   },error));
 const auto committed=read(primary);require(committed!=original&&read(primary.string()+".bak")==original);
 require(save.get()==save_identity&&&transport.loader()==loader&&profile.identity==profile_identity);
 {
  const auto qest=profile.campaign.payload("QEST");require(qest.size>0);
  Raw checkpoint_image;put(checkpoint_image,1);put(checkpoint_image,std::uint32_t(qest.size));
  checkpoint_image.insert(checkpoint_image.end(),{'Q','E','S','T'});
  checkpoint_image.insert(checkpoint_image.end(),qest.data,qest.data+qest.size);
  const auto campaign_bytes=profile.campaign.bytes();
  require(transport.load_checkpoint_quests(checkpoint_image,error));
  require(save.get()==save_identity&&profile.identity==profile_identity&&
          &transport.loader()==loader&&profile.campaign.bytes()==campaign_bytes&&
          transport.loader().profile().campaign.bytes()==campaign_bytes&&
          quest_owner->owns_save(save.get()));
 }
 require(profile.campaign.bytes()==committed&&transport.loader().profile().campaign.bytes()==committed);
 require(inventory->add_fixed_loot(165,inventory_services,error));
 require(!inventory->items().empty());
 std::int32_t equipped_slot=-1;
 require(inventory->character_auto_equip(0,equipped_slot,inventory_services,error));
 bool item_equipped=false;require(inventory->is_equipped(0,item_equipped,error)&&item_equipped);
 inventory->swap_equipment();
 require(inventory->current_equipment()==1);
 require(dh2::native::player_inventory_save_commit_v1::after_successful_mutation(
   [&](std::string& save_error){return transport.save_gameplay(save_error);},error));
 const auto gear=profile.campaign.payload("GEAR");
 const auto gear_item_count=gear.size>=12?std::uint32_t(gear.data[8])|
  (std::uint32_t(gear.data[9])<<8)|(std::uint32_t(gear.data[10])<<16)|
  (std::uint32_t(gear.data[11])<<24):0;
 require(gear.size>=12&&gear.data[0]==0&&gear.data[4]==1&&
         gear_item_count==inventory->items().size());
 // The same V3 SKIL writer receives talent levels and assigned slots from
 // this canonical Save; test the mutation-to-transport boundary and payload.
 require(save->skills_initialized()&&!save->skills().empty());
 require(save->set_skill_level(0,7,error));
 struct SkillUpdateContext {std::uintptr_t character;unsigned calls;};
 SkillUpdateContext skill_update{save->character(),0};
 const SavedSkillUpdateServicesV1 skill_update_services{&skill_update,
  [](void* raw,std::uintptr_t character,std::string& callback_error)->bool{
   auto& context=*static_cast<SkillUpdateContext*>(raw);
   if(character!=context.character){callback_error="skill update crossed canonical Character";return false;}
   ++context.calls;return true;
  }};
 dh2::player_saved_skill_slots_v1::BoundSlotsV1 saved_slots(*save,*inventory);
 require(saved_slots.set_skill_in_slot(2,0,skill_update_services,error)==
         dh2::player_saved_skill_slots_v1::Status::ok&&skill_update.calls==1);
 require(dh2::native::player_inventory_save_commit_v1::after_successful_mutation(
   [&](std::string& save_error){return transport.save_gameplay(save_error);},error));
 require(save->skill_level(0)==7&&save->skill_in_slot(2)==0&&
         profile.campaign.payload("SKIL").size>8);
 save->set_player_level(18);
 auto live_properties=property_view(property_rules,properties);
 require(!dh2_property_set(&live_properties,33,0x1000));
 require(!dh2_property_add(&live_properties,33,256));
 require(save->set_faery_state(1,1,0,error)&&save->set_faery_level(1,4,0,error));
 struct FaeryUpdateContext {unsigned calls;};FaeryUpdateContext faery_update{0};
 const dh2::data::player_faery_change_v1::Services faery_services{&faery_update,
  [](void* raw,std::string& callback_error)->bool{
   ++static_cast<FaeryUpdateContext*>(raw)->calls;callback_error.clear();return true;
  }};
 dh2::data::player_faery_change_v1::Result faery_result{};
 require(dh2::data::player_faery_change_v1::change(save.get(),0,1,faery_services,
          &faery_result,error)==dh2::data::player_faery_change_v1::Status::complete&&
         faery_update.calls==1&&faery_result.save_changed&&faery_result.skills_updated);
 require(dh2::native::player_inventory_save_commit_v1::after_successful_mutation(
   [&](std::string& save_error){return transport.save_gameplay(save_error);},error));
 const auto saved_xp=properties.resolved[33];
 require(save->level()==18&&saved_xp==0x1100&&save->current_faery(0)==1&&
         save->faery_level(1,0)==4&&save->faeries()[0][1].state==1&&
         !inventory->items().empty()&&item_equipped&&inventory->current_equipment()==1);
 constexpr const char* tags[]{"PNAM","PLVL","PCLS","PDFL","LNAM","LEPT","LUSP",
  "LVLS","SKIL","FAES","CFEE","QEST","PROP","GEAR","FTVL"};
 for(const auto* tag:tags)require(profile.campaign.section(tag)!=nullptr);
 require(old_view.bytes()==original&&profile.campaign.bytes()!=old_view.bytes());
 // Recreate the crash window where only the previous profile backup remains.
 // The same canonical Save writers may publish a new primary only because
 // the backup exactly matches the retained indexed profile.
 const auto committed_before_recovery=read(primary);
 write(primary.string()+".bak",committed_before_recovery);std::filesystem::remove(primary);
 require(transport.save_gameplay(error));
 require(read(primary)==profile.campaign.bytes()&&read(primary.string()+".bak")==committed_before_recovery&&
         profile.campaign.source_sections().size()==15);
 std::string close_error;require(quest_owner->close(close_error));
}
std::vector<profile::GameplaySectionPayloadProviderV1> gameplay_providers(
 const PlayerSavegameV1& save,std::vector<std::shared_ptr<GameplayPayload>>& owners){
 constexpr const char* tags[]{"PNAM","PLVL","PCLS","PDFL","LNAM","LEPT","LUSP",
  "LVLS","SKIL","FAES","CFEE","QEST","PROP","GEAR","FTVL"};
 std::vector<profile::GameplaySectionPayloadProviderV1> result;result.reserve(15);owners.clear();owners.reserve(15);
 for(const auto* tag:tags){auto state=std::make_shared<GameplayPayload>();state->save=&save;std::memcpy(state->tag.data(),tag,4);
  result.push_back({state->tag,state,state.get(),write_gameplay_payload});owners.push_back(std::move(state));}
 return result;
}
void gameplay_save_cases(const std::filesystem::path& root,const CharacterTable& table,
                         const std::string& class_name){
 const auto dir=root/"gameplay-save";std::filesystem::create_directories(dir);std::string error;
 const std::int32_t slot=7;const auto primary=dir/data::player_profile_filename_v1(slot,false,false);
 const auto original=campaign(class_name,Raw{});write(primary,original);
 PlayerSavegameV1 save;save.set_character(UINT64_C(0x77770001));save.set_slot(slot);
 PlayerSaveProfileV1 canonical;std::int32_t difficulty=0;std::uint8_t online=0;
 profile::Transport transport(save,canonical);
 require(transport.bind({dir,&table,&difficulty,{},false,nullptr,nullptr,{},&online},error));
 require(transport.loader().load(1,error));require(transport.loader().load(4,error));
 constexpr const char* tags[]{"PNAM","PLVL","PCLS","PDFL","LNAM","LEPT","LUSP",
  "LVLS","SKIL","FAES","CFEE","QEST","PROP","GEAR","FTVL"};
 std::vector<std::shared_ptr<GameplayPayload>> owners;auto providers=gameplay_providers(save,owners);
 const auto save_id=reinterpret_cast<std::uintptr_t>(&save),profile_id=canonical.identity;
 auto* loader=&transport.loader();const auto old_view=canonical.campaign;
 require(profile_id&&old_view&&old_view.bytes()==original);
 const auto original_mode=save.source_save_mode();
 require(!transport.save_gameplay(error)&&read(primary)==original&&
         save.source_save_mode()==original_mode);
 require(transport.save_gameplay(providers,error));
 const auto committed=read(primary);require(read(primary.string()+".bak")==original);
 require(reinterpret_cast<std::uintptr_t>(&save)==save_id&&&transport.loader()==loader&&canonical.identity==profile_id);
 require(loader->profile().identity==profile_id&&canonical.campaign.bytes()==committed&&
         transport.loader().profile().campaign.bytes()==committed);
 require(canonical.campaign.bytes()!=old_view.bytes()&&old_view.bytes()==original);
 for(std::size_t i=0;i<15;++i){const auto payload=canonical.campaign.payload(tags[i]);
  require(payload.size==4&&std::memcmp(payload.data,tags[i],4)==0&&owners[i]->calls==1);}

 const auto backup=read(primary.string()+".bak");
 const auto indexed=canonical.campaign.bytes();
 auto missing=providers;missing.erase(missing.begin()+11);
 require(!transport.save_gameplay(missing,error)&&read(primary)==committed&&read(primary.string()+".bak")==backup);
 auto extra=providers;profile::GameplaySectionPayloadProviderV1 extra_provider=providers.front();
 std::memcpy(extra_provider.tag.data(),"EXTR",4);extra.push_back(extra_provider);
 require(!transport.save_gameplay(extra,error)&&read(primary)==committed);
 auto duplicate=providers;duplicate.push_back(providers.front());
 require(!transport.save_gameplay(duplicate,error)&&read(primary)==committed);
 require(std::all_of(owners.begin(),owners.end(),[](const auto& p){return p->calls==1;}));

 save.set_source_save_blocked(true);
 require(!transport.save_gameplay(providers,error)&&read(primary)==committed);
 save.set_source_save_blocked(false);
 online=1;require(!transport.save_gameplay(providers,error)&&read(primary)==committed);online=0;
 const auto saved_slot=save.slot();save.set_slot(saved_slot+1);
 require(!transport.save_gameplay(providers,error)&&read(primary)==committed);save.set_slot(saved_slot);

 // A mask-1-only registration set is incomplete even with all providers.
 const auto partial_dir=root/"gameplay-save-partial";std::filesystem::create_directories(partial_dir);
 const auto partial_primary=partial_dir/data::player_profile_filename_v1(8,false,false);
 write(partial_primary,original);PlayerSavegameV1 partial_save;partial_save.set_character(UINT64_C(0x77770002));partial_save.set_slot(8);
 PlayerSaveProfileV1 partial_profile;std::uint8_t partial_online=0;profile::Transport partial_transport(partial_save,partial_profile);
 require(partial_transport.bind({partial_dir,&table,&difficulty,{},false,nullptr,nullptr,{},&partial_online},error));
 require(partial_transport.loader().load(1,error));std::vector<std::shared_ptr<GameplayPayload>> partial_owners;
 auto partial_providers=gameplay_providers(partial_save,partial_owners);
 require(!partial_transport.save_gameplay(partial_providers,error)&&read(partial_primary)==original);
 require(std::all_of(partial_owners.begin(),partial_owners.end(),[](const auto& p){return p->calls==0;}));

 // Out-of-band primary mutation is rejected without overwriting that file or
 // replacing the backup/canonical immutable profile snapshot.
 Raw external;put(external,1);put(external,1);external.insert(external.end(),{'R','A','C','E'});external.push_back(0x99);
 write(primary,external);const Raw backup_sentinel{0xa1,0xa2};write(primary.string()+".bak",backup_sentinel);
 require(!transport.save_gameplay(providers,error)&&read(primary)==external&&
         read(primary.string()+".bak")==backup_sentinel&&canonical.campaign.bytes()==indexed);
}
}
void metadata_character_session_case(const std::filesystem::path& root,
 const std::filesystem::path& cache,const CharacterTable& characters,
 const std::string& class_name){
 const auto dir=root/"metadata-character-session";std::filesystem::create_directories(dir);
 Raw prop;put(prop,224);for(unsigned i=0;i<224;++i)put(prop,0x40000000+i);prop.push_back(41);
 write(dir/"dh2_000.savegame",campaign(class_name,prop));
 profile::Metadata unloaded;profile::Metadata::CharacterSession pre_load_session;
 std::string error;require(!unloaded.create_character_session(1,pre_load_session,error));
 require(!pre_load_session&&!error.empty());
 profile::Metadata preview;std::int32_t difficulty=0;
 require(preview.load(0,dir,characters,difficulty,error));
 const auto metadata_session=preview.share_session();
 require(metadata_session&&metadata_session.save.get()==&preview.save());
 require(metadata_session.profile->identity==preview.profile_identity());
 require(&metadata_session.transport->loader().save()==metadata_session.save.get());
 profile::Metadata::SharedSession metadata_copy;metadata_copy=metadata_session;
 require(metadata_copy.save==metadata_session.save&&metadata_copy.profile==metadata_session.profile&&
         metadata_copy.transport==metadata_session.transport);
 metadata_copy.reset();require(!metadata_copy&&metadata_session);
 profile::Metadata::CharacterSession character_session;
 constexpr std::uintptr_t character_identity=UINT64_C(0x7fff000000000123);
 require(preview.create_character_session(character_identity,character_session,error));
 require(character_session&&character_session.save!=metadata_session.save&&
         character_session.profile!=metadata_session.profile&&
         character_session.transport!=metadata_session.transport);
 require(character_session.save->slot()==preview.save().slot()&&
         character_session.save->character()==character_identity);
 require(&character_session.transport->loader().save()==character_session.save.get());
 const auto original_save=character_session.save.get();
 const auto original_profile=character_session.profile.get();
 const auto original_transport=character_session.transport.get();
 require(!preview.create_character_session(character_identity,character_session,error));
 require(!error.empty()&&character_session.save.get()==original_save&&
         character_session.profile.get()==original_profile&&
         character_session.transport.get()==original_transport);
 GameplayAssemblyTables tables;load_gameplay_assembly_tables(cache,tables);
 {
  auto character_quests=std::make_shared<dh2::native::quests::Owner>(
      character_session.save,tables.quests,tables.constants);
  require(character_quests->owns_save(character_session.save.get())&&
          !character_quests->owns_save(metadata_session.save.get()));
 }
 profile::Metadata::CharacterSession invalid_session;
 require(!preview.create_character_session(0,invalid_session,error)&&!invalid_session&&!error.empty());
 profile::Metadata::CharacterSession moved_session(std::move(character_session));
 require(!character_session&&moved_session.save.get()==original_save&&
         moved_session.profile.get()==original_profile&&
         moved_session.transport.get()==original_transport);
 profile::Metadata::CharacterSession replacement;
 require(preview.create_character_session(character_identity+1,replacement,error));
 std::weak_ptr<PlayerSavegameV1> replaced_save=replacement.save;
 std::weak_ptr<PlayerSaveProfileV1> replaced_profile=replacement.profile;
 std::weak_ptr<profile::Transport> replaced_transport=replacement.transport;
 replacement=std::move(moved_session);
 require(!moved_session&&replacement.save.get()==original_save&&
         replacement.profile.get()==original_profile&&replacement.transport.get()==original_transport);
 require(replaced_save.expired()&&replaced_profile.expired()&&replaced_transport.expired());
 require(&replacement.transport->loader().save()==replacement.save.get());
 profile::Metadata::SharedSession retained_session;
 {
  profile::Metadata selected;require(selected.load(0,dir,characters,difficulty,error));
  retained_session=selected.share_session();
 }
 require(retained_session&&retained_session.transport->loader().save().slot()==0);
 require(&retained_session.transport->loader().save()==retained_session.save.get());
}
int main(int argc,char** argv){try{
 if(argc==5&&!std::strcmp(argv[1],"--cold-load")){
  const std::filesystem::path cache=argv[2],directory=argv[3];
  const auto slot=std::stoi(argv[4]);Raw data=read(cache/"character_properties_pyarray.bin");
  Raw names=read(cache/"character_properties_pyarraynames.bin");
  Raw fields=read(cache/"character_properties_pystructnames.bin");
  CharacterTable table;std::string error;
  require(load_characters(bytes(data),bytes(names),bytes(fields),table,error));
  const auto knight=std::find(table.names.begin(),table.names.end(),"KnightPlayerBase");
  require(knight!=table.names.end());
  GameplayAssemblyTables tables;load_gameplay_assembly_tables(cache,tables);
  PropertyRules property_rules;require(load_property_rules(table,property_rules,error));
  auto save=std::make_shared<PlayerSavegameV1>();
  save->set_character(UINT64_C(0x77770009));save->set_slot(slot);
  PlayerSaveProfileV1 canonical;std::int32_t difficulty=-1;
  std::uint8_t online=0;PropertyState properties;
  const auto class_row=std::size_t(knight-table.names.begin());
  require(class_row<table.rows.size());
  reset_properties(property_rules,properties,&table.rows[class_row]);
  properties.resolved[28]=0;
  auto inventory=std::make_unique<FreshInventoryOwnedV4>(save->character(),
    tables.loot.borrow(),InventoryRandomServiceV4{nullptr,no_random},0,properties);
  auto effect_lease=std::make_shared<std::uint8_t>(0);
  OwnedInventoryServicesV4 inventory_services{effect_lease.get(),unused_inventory_effect,
                                                unused_inventory_observer,true};
  auto quest_owner=std::make_shared<dh2::native::quests::Owner>(save,tables.quests,tables.constants);
  profile::Transport transport(*save,canonical);
  profile::TransportBindings bindings{directory,&table,&difficulty,{},false,
    &tables.levels,&tables.world_map,quest_owner,&online};
  bindings.skill_tables=&tables.skills;bindings.skill_tree_selector=&properties.resolved[28];
  bindings.property_rules=&property_rules;bindings.properties=&properties;
  bindings.inventory=inventory.get();bindings.item_powers=tables.powers.borrow();
  bindings.inventory_services=inventory_services;bindings.inventory_services_owner=effect_lease;
  std::unique_ptr<ItemInstanceV1> incoming;bindings.inventory_incoming=&incoming;
  require(transport.bind(std::move(bindings),error));
  require(transport.loader().load(1,error));
  properties.resolved[28]=0;
  require(save->initialize_skills(tables.skills,properties.resolved[28],error));
  require(transport.loader().load(2,error));
  require(transport.loader().load(4,error));
  PlayerSavegameV1 skill_probe;skill_probe.set_character(save->character());
  require(skill_probe.initialize_skills(tables.skills,properties.resolved[28],error));
  std::size_t skill_probe_consumed=0;
  const auto skill_probe_payload=canonical.campaign.payload("SKIL");
  const auto skill_probe_status=skill_probe.load_skills(
      skill_probe_payload,tables.skills,skill_probe_consumed,error);
  if(!(skill_probe_status==0&&skill_probe_consumed==skill_probe_payload.size&&
       skill_probe.skill_level(0)==7&&skill_probe.skill_in_slot(2)==0))
   std::cerr<<"cold SKIL probe status="<<skill_probe_status<<" consumed="
     <<skill_probe_consumed<<" size="<<skill_probe_payload.size<<" level="
     <<skill_probe.skill_level(0)<<" slot="<<skill_probe.skill_in_slot(2)
     <<" error="<<error<<"\n";
  require(skill_probe_status==0&&
          skill_probe_consumed==skill_probe_payload.size&&skill_probe.skill_level(0)==7&&
          skill_probe.skill_in_slot(2)==0);
  require(save->class_id()==std::int32_t(knight-table.names.begin())&&save->level()==18&&
          save->name()=="TransportFixture"&&difficulty==1&&save->unlocked_difficulty()==2);
  require(properties.saved[33]==0x1100);
  require(save->skills_initialized()&&save->skill_level(0)==7&&save->skill_in_slot(2)==0);
  require(save->current_faery(0)==1&&save->faery_level(1,0)==4&&save->faeries()[0][1].state==1);
  require(!inventory->items().empty()&&inventory->current_equipment()==1);
  bool item_equipped=false;require(inventory->is_equipped(0,item_equipped,error)&&item_equipped);
  auto* quest=quest_owner->resolve(save->source_quest_log_b8().quests[0][0]);
  require(quest&&quest->action_18);
  dh2::native::quests::QuestReadV1 selected_quest;
  require(quest_owner->read_quest(0,0,0,selected_quest,error));
  require(save->source_quest_log_b8().word_2c[0]==selected_quest.id);
  require(transport.receipt().file_opens==1&&canonical.campaign.source_sections().size()==15);
  std::cout<<"{\"validation\":\"PASS\",\"fresh_process_profile_open\":true,"
           "\"slot\":"<<slot<<",\"sections\":"
           <<canonical.campaign.source_sections().size()<<",\"mismatches\":0}\n";
  return 0;
 }
 require(argc==3);const std::filesystem::path cache=argv[1],dir=argv[2];std::filesystem::create_directories(dir);
 auto data=read(cache/"character_properties_pyarray.bin"),names=read(cache/"character_properties_pyarraynames.bin"),fields=read(cache/"character_properties_pystructnames.bin");
 CharacterTable table;std::string error;require(load_characters(bytes(data),bytes(names),bytes(fields),table,error));
 const auto knight=std::find(table.names.begin(),table.names.end(),"KnightPlayerBase");require(knight!=table.names.end());
 const auto id=std::int32_t(knight-table.names.begin());require(id==263);
 PropertyRules rules;require(load_property_rules(table,rules,error));
  metadata_character_session_case(dir,cache,table,*knight);
 gameplay_save_cases(dir,table,*knight);
 gameplay_assembly_case(dir,cache,*knight,table,rules);
 Raw prop;put(prop,224);for(unsigned i=0;i<224;++i)put(prop,0x40000000+i);prop.push_back(41);
 const auto primary=dir/"dh2_000.savegame";write(primary,campaign(*knight,prop));
 profile::Metadata preview;std::int32_t difficulty=0;
 require(preview.load(0,dir,table,difficulty,error));require(preview.receipt().loaded&&preview.receipt().field_reads==7&&preview.receipt().file_opens==1);
 require(preview.save().class_id()==id&&preview.save().character()==0&&difficulty==1);
 // Metadata follows indexed C1 defaults when the optional PLVL reader has
 // no payload; it does not borrow the blank gameplay Save's level0.
 write(dir/"dh2_002.savegame",campaign(*knight,prop,false));
 profile::Metadata no_level;
 require(no_level.load(2,dir,table,difficulty,error));
 require(no_level.save().level()==1&&no_level.receipt().level==1);
 require(no_level.save().class_id()==id&&no_level.save().character()==0);
 require(no_level.load(2,dir,table,difficulty,error));
 require(no_level.save().level()==1&&no_level.receipt().file_opens==1);
 // NativeStartGame's optional numeric SG_Save and mandatory LUSP SG_Save
 // reuse this same loaded Save/+8 index and retain each prior primary as .bak.
 const auto start_dir=dir/"native-start-save";std::filesystem::create_directories(start_dir);
 const auto start_primary=start_dir/"dh2_006.savegame";
 const auto start_original=campaign(*knight,prop);write(start_primary,start_original);
 profile::Metadata start_save;
 require(start_save.load(6,start_dir,table,difficulty,error));
 const auto start_save_id=start_save.save_identity(),start_profile_id=start_save.profile_identity();
 require(start_save.save().use_spawn_points()==std::array<std::uint8_t,3>{{1,0,1}});
 difficulty=2;
 const auto numeric_saved=start_save.save_numeric_request(table,difficulty,error);
 if(!numeric_saved)std::cerr<<"NativeStartGame numeric save: "<<error<<'\n';
 require(numeric_saved);
 require(start_save.save_identity()==start_save_id&&start_save.profile_identity()==start_profile_id);
 const auto after_numeric_save=read(start_primary);
 require(read(start_primary.string()+".bak")==start_original);
 const auto start_saved=start_save.clear_spawn_point_and_save(2,table,difficulty,error);
 if(!start_saved)std::cerr<<"NativeStartGame save: "<<error<<'\n';
 require(start_saved);
 require(start_save.save_identity()==start_save_id&&start_save.profile_identity()==start_profile_id);
 require(start_save.save().use_spawn_points()==std::array<std::uint8_t,3>{{1,0,0}});
 require(read(start_primary.string()+".bak")==after_numeric_save);
 PlayerProfileIndexV1 start_index;const auto start_updated=read(start_primary);
 require(start_index.load(bytes(start_updated),error));
 const auto updated_spawn=start_index.borrow().payload("LUSP");
 require(updated_spawn.size==3&&updated_spawn.data[0]==1&&updated_spawn.data[1]==0&&updated_spawn.data[2]==0);
 const auto kept_properties=start_index.borrow().payload("PROP");
 require(kept_properties.size==prop.size()&&std::equal(prop.begin(),prop.end(),kept_properties.data));
 profile::Metadata start_reload;
 require(start_reload.load(6,start_dir,table,difficulty,error));
 require(start_reload.save().use_spawn_points()==std::array<std::uint8_t,3>{{1,0,0}});
 PlayerSavegameV1 save;const std::uintptr_t character=UINT64_C(0x12345678000000a1);save.set_character(character);save.set_slot(0);
 PlayerSaveProfileV1 canonical;profile::Transport transport(save,canonical);require(transport.bind({dir,&table,&difficulty,{}},error));
 auto* live_save=&save;auto* loader=&transport.loader();std::int16_t class_cache=-1;
 Query query{0,character};saved_class::Runtime runtime({character,&class_cache,nullptr,nullptr,nullptr,&table,nullptr,&live_save,&loader,{&query,is_player}});
 saved_class::Result result;require(runtime.resolve(&result,error)==saved_class::Status::complete);
 require(result.load_calls==1&&result.class_writebacks==1&&class_cache==id&&save.class_id()==id);
 require(&transport.loader().save()==&save&&canonical.identity&&canonical.identity!=preview.profile_identity());
 require(reinterpret_cast<std::uintptr_t>(&save)!=preview.save_identity());
 require(transport.receipt().field_reads==7&&transport.receipt().file_opens==1&&transport.receipt().sections==8);
 require(save.level()==17&&save.level_name_fields().level_id==0x10002&&save.unlocked_difficulty()==2);
 require(runtime.resolve(&result,error)==saved_class::Status::complete&&result.stage==saved_class::Stage::cached&&result.load_calls==0&&query.calls==1);
 require(preview.load(0,dir,table,difficulty,error));require(preview.receipt().field_reads==14&&preview.receipt().file_opens==1);
 require(!preview.load(1,dir,table,difficulty,error));
 // A populated source +8 profile is retained even after SG_SetSlot changes +4.
 save.set_slot(99);class_cache=-1;require(runtime.resolve(&result,error)==saved_class::Status::complete);
 require(transport.receipt().file_opens==1&&transport.receipt().field_reads==14&&save.slot()==99);
 // PROP binds the same live Save profile and PlayerCombat PropertyState.
 // The transport builds a short-lived PropertyView only for this delivery;
 // its generic continuation must not become a competing property owner.
 PropertyState state;state.saved.fill(-7);state.resolved.fill(-9);
 unsigned generic_calls=0;auto lease=std::make_shared<int>(1);
 PlayerSaveLoadServicesV1 continuation{lease,[&](const PlayerSaveLoadRequestV1&,PlayerSaveLoadResponseV1&,std::string&){
  ++generic_calls;return false;
 }};
 profile::TransportBindings property_bindings{dir,&table,&difficulty,continuation};
 property_bindings.property_rules=&rules;property_bindings.properties=&state;
 require(transport.bind(std::move(property_bindings),error));require(transport.loader().load(0x20,error));
 require(generic_calls==0&&save.saved_properties_byte_194()==41);
 require(&transport.loader().save()==&save&&canonical.identity&&canonical.campaign);
 unsigned stored=0;for(unsigned i=0;i<224;++i){const auto type=rules.types[i]==-1?16u:std::uint32_t(rules.types[i]);
  const auto expected=(type&0x20)?std::int32_t(0x40000000+i):-7;stored+=(type&0x20)!=0;
  require(state.saved[i]==expected&&state.resolved[i]==-9);
 }require(stored>0);
 // This gameplay-bound Transport has the seven metadata writers plus the
 // reached PROP writer. Its metadata-only persistence API must reject the
 // gameplay profile before touching the primary; it is not NativeSaveGame.
 const auto primary_before_gameplay_save=read(primary);
 const auto gameplay_slot_before_save=save.slot();save.set_slot(0);
 require(!transport.save_existing_metadata(error));
 require(error=="mask-1 metadata writer set required");
 require(read(primary)==primary_before_gameplay_save);
 save.set_slot(gameplay_slot_before_save);
 profile::TransportBindings incomplete{dir,&table,&difficulty,{}};incomplete.property_rules=&rules;
 require(!transport.bind(std::move(incomplete),error)&&error.find("must bind together")!=std::string::npos);
 write(dir/"dh2_005.savegame",campaign(*knight,prop));
 PlayerSavegameV1 missing_save;missing_save.set_slot(5);missing_save.set_character(character);
 PlayerSaveProfileV1 missing_profile;profile::Transport missing_transport(missing_save,missing_profile);
 require(missing_transport.bind({dir,&table,&difficulty,continuation},error));
 require(!missing_transport.loader().load(0x20,error));
 require(error=="live gameplay PropertyRules/PropertyState unavailable"&&generic_calls==0);
 require(transport.bind({dir,&table,&difficulty,{}},error));require(!transport.loader().load(2,error));
 require(transport.loader().reached_phase()==std::uint32_t(PlayerSaveLoadOpV1::init_levels)+1&&!save.skills_initialized());
 require(!transport.loader().load(4,error));require(transport.loader().reached_phase()==std::uint32_t(PlayerSaveLoadOpV1::online)+1);
 require(!transport.bind({dir,&table,&difficulty,{lease,{}}},error));
 // Actual table bindings advance source SG_Load2 to the missing skill provider,
 // retaining the genuine six arrays. SG_Load4 then consumes LVLS into those
 // same arrays and stops at the still-unbound online provider.
 LevelTables levels;WorldMapTables world_map;
 auto level_data=read(cache/"levels_pyarray.bin"),level_names=read(cache/"levels_pyarraynames.bin"),level_fields=read(cache/"levels_pystructnames.bin");
 auto map_data=read(cache/"worldmap_pyarray.bin"),map_names=read(cache/"worldmap_pyarraynames.bin"),map_fields=read(cache/"worldmap_pystructnames.bin");
 require(load_levels(bytes(level_data),bytes(level_names),bytes(level_fields),levels,error));
 require(load_world_map(bytes(map_data),bytes(map_names),bytes(map_fields),world_map,error));
 require(levels.levels.size()==51&&world_map.locations.size()==13);
 require(!transport.bind({dir,&table,&difficulty,{},false,&levels,nullptr},error));
 Raw lvls;
 for(unsigned kind=0;kind<2;++kind)for(unsigned d=0;d<3;++d){
  put(lvls,2);auto name=text(kind?world_map.locations.front().name:levels.levels.front().name);
  lvls.insert(lvls.end(),name.begin(),name.end());put(lvls,kind?2u:1u);
  name=text("UnknownModdedLevel");lvls.insert(lvls.end(),name.begin(),name.end());put(lvls,UINT32_MAX);
 }
 Raw level_campaign;put(level_campaign,2);put(level_campaign,std::uint32_t(lvls.size()));
 level_campaign.insert(level_campaign.end(),{'L','V','L','S'});level_campaign.insert(level_campaign.end(),lvls.begin(),lvls.end());
 Raw travel;for(const auto& value:{std::string("1"),std::string("100000000000000000000000000000001"),std::string("101")}){
  auto field=text(value);travel.insert(travel.end(),field.begin(),field.end());
 }
 put(level_campaign,std::uint32_t(travel.size()));level_campaign.insert(level_campaign.end(),{'F','T','V','L'});
 level_campaign.insert(level_campaign.end(),travel.begin(),travel.end());
 write(dir/"dh2_003.savegame",level_campaign);
 PlayerSavegameV1 saved_levels;saved_levels.set_slot(3);PlayerSaveProfileV1 level_profile;
 profile::Transport levels_transport(saved_levels,level_profile);
 require(levels_transport.bind({dir,&table,&difficulty,{},false,&levels,&world_map},error));
 require(!levels_transport.loader().load(2,error));
 require(levels_transport.loader().reached_phase()==std::uint32_t(PlayerSaveLoadOpV1::init_skills)+1);
 require(!saved_levels.skills_initialized()&&level_profile.identity);
 std::array<const std::int32_t*,6> arrays{};unsigned defaults=0;
 for(unsigned d=0;d<3;++d){
  auto* a=saved_levels.source_level_states(d);auto* b=saved_levels.source_world_map_states(d);
  require(a->count==51&&b->count==13&&a->words&&b->words);
  arrays[d*2]=a->words;arrays[d*2+1]=b->words;
  for(unsigned row=0;row<a->count;++row){require(a->words[row]==levels.levels[row].level_state);++defaults;}
  for(unsigned row=0;row<b->count;++row){require(b->words[row]==world_map.locations[row].state);++defaults;}
 }
 require(defaults==192);
 require(!levels_transport.loader().load(4,error));
 require(levels_transport.loader().reached_phase()==std::uint32_t(PlayerSaveLoadOpV1::online)+1);
 for(unsigned d=0;d<3;++d){
  auto* a=saved_levels.source_level_states(d);auto* b=saved_levels.source_world_map_states(d);
  require(a->words==arrays[d*2]&&b->words==arrays[d*2+1]);
  require(a->words[0]==1&&b->words[0]==2);
 }
 require(levels_transport.receipt().file_opens==1);
 std::uint8_t offline_status=0;
 // Declared host-only source-global fixture: online=false. All file/section
 // readers still execute production code. This synthetic profile has no
 // inventory/quest/skill payload; successful mask4 is not a gameplay proof.
 require(levels_transport.bind({dir,&table,&difficulty,{},false,&levels,&world_map,{},&offline_status},error));
 require(levels_transport.loader().load(4,error));
 require(*saved_levels.source_fast_travel_bits(0)==std::array<std::uint32_t,2>{{1,0}});
 require(*saved_levels.source_fast_travel_bits(1)==std::array<std::uint32_t,2>{{1,1}});
 require(*saved_levels.source_fast_travel_bits(2)==std::array<std::uint32_t,2>{{5,0}});
 require(levels_transport.bind({dir,&table,&difficulty,{},false,&levels,&world_map},error));
 require(!levels_transport.loader().load(2,error));
 require(saved_levels.source_level_states(0)->words[0]==1&&saved_levels.source_world_map_states(0)->words[0]==2);
 require(levels_transport.bind({},error));
 require(!levels_transport.loader().load(4,error));
 require(levels_transport.loader().reached_phase()==std::uint32_t(PlayerSaveLoadOpV1::load_section)+1);
 // Slot -1 has the original no-file SG_Load(1) branch and fallback writeback.
 PlayerSavegameV1 blank;blank.set_character(character);PlayerSaveProfileV1 empty;profile::Transport blank_transport(blank,empty);
 auto* blank_save=&blank;auto* blank_loader=&blank_transport.loader();std::int16_t blank_cache=-1;
 saved_class::Runtime fallback({character,&blank_cache,nullptr,nullptr,nullptr,&table,nullptr,&blank_save,&blank_loader,{&query,is_player}});
 require(fallback.resolve(&result,error)==saved_class::Status::complete&&blank_cache==id&&blank.class_id()==id&&result.load_calls==1);
 require(!empty.identity&&blank_transport.receipt().file_opens==0&&blank_loader->delivered_calls()==0);
 // Cold-launch cache recovery is header-gated: absent, short, and FFFFFFFF
 // primaries may use the actual previous save, but malformed indexed sections
 // do not trigger an arbitrary retry.
 PlayerSavegameV1 bad;bad.set_slot(1);PlayerSaveProfileV1 bad_profile;profile::Transport rejected(bad,bad_profile);
 require(rejected.bind({dir,&table,&difficulty,{}},error));require(!rejected.loader().load(1,error));require(!bad_profile.identity&&bad.class_id()==-1);
 write(dir/"dh2_001.savegame",words({UINT32_MAX}));require(!rejected.loader().load(1,error));require(!bad_profile.identity&&bad.class_id()==-1);
 write(dir/"dh2_001.savegame",campaign(*knight,prop));require(rejected.loader().load(1,error));
 require(bad_profile.identity&&bad.class_id()==id&&rejected.receipt().file_opens==2);
 const auto recovery_path=dir/"dh2_008.savegame";
 const auto recovery_bytes=campaign(*knight,prop);
 write(recovery_path.string()+".bak",recovery_bytes);
 PlayerSavegameV1 cold_missing;cold_missing.set_slot(8);PlayerSaveProfileV1 cold_missing_profile;
 profile::Transport cold_missing_transport(cold_missing,cold_missing_profile);
 require(cold_missing_transport.bind({dir,&table,&difficulty,{}},error));
 require(cold_missing_transport.loader().load(1,error));
 require(cold_missing_profile.campaign.bytes()==recovery_bytes&&cold_missing.class_id()==id&&
         cold_missing_transport.receipt().file_opens==1);
 write(recovery_path,Raw{0xff,0xff,0xff,0xff});
 PlayerSavegameV1 cold_marker;cold_marker.set_slot(8);PlayerSaveProfileV1 cold_marker_profile;
 profile::Transport cold_marker_transport(cold_marker,cold_marker_profile);
 require(cold_marker_transport.bind({dir,&table,&difficulty,{}},error));
 require(cold_marker_transport.loader().load(1,error));
 require(cold_marker_profile.campaign.bytes()==recovery_bytes&&cold_marker.class_id()==id&&
         cold_marker_transport.receipt().file_opens==2);
 write(recovery_path,Raw{0x01,0x02,0x03});
 PlayerSavegameV1 cold_short;cold_short.set_slot(8);PlayerSaveProfileV1 cold_short_profile;
 profile::Transport cold_short_transport(cold_short,cold_short_profile);
 require(cold_short_transport.bind({dir,&table,&difficulty,{}},error));
 require(cold_short_transport.loader().load(1,error));
 require(cold_short_profile.campaign.bytes()==recovery_bytes&&cold_short.class_id()==id&&
         cold_short_transport.receipt().file_opens==2);
 Raw malformed_index;put(malformed_index,1);put(malformed_index,100);
 malformed_index.insert(malformed_index.end(),{'P','N','A','M'});write(recovery_path,malformed_index);
 PlayerSavegameV1 cold_malformed;cold_malformed.set_slot(8);PlayerSaveProfileV1 cold_malformed_profile;
 profile::Transport cold_malformed_transport(cold_malformed,cold_malformed_profile);
 require(cold_malformed_transport.bind({dir,&table,&difficulty,{}},error));
 require(!cold_malformed_transport.loader().load(1,error));
 require(!cold_malformed_profile.identity&&cold_malformed.class_id()==-1&&
         cold_malformed_transport.receipt().file_opens==1);
 // A failed typed PROP read keeps the words already delivered into the same
 // saved sheet. The enclosing mask load does not roll this prefix back.
 Raw truncated_prop;put(truncated_prop,224);for(unsigned i=0;i<224;++i)put(truncated_prop,0x50000000+i);
 write(dir/"dh2_004.savegame",campaign(*knight,truncated_prop));
 PlayerSavegameV1 partial_save;partial_save.set_slot(4);partial_save.set_character(character);
 PlayerSaveProfileV1 partial_profile;profile::Transport partial_transport(partial_save,partial_profile);
 PropertyState partial_state;partial_state.saved.fill(-11);partial_state.resolved.fill(-13);
 profile::TransportBindings partial_bindings{dir,&table,&difficulty,{}};
 partial_bindings.property_rules=&rules;partial_bindings.properties=&partial_state;
 require(partial_transport.bind(std::move(partial_bindings),error));
 require(!partial_transport.loader().load(0x20,error));
 require(error=="truncated source PROP byte194"&&partial_profile.identity&&partial_profile.campaign);
 require(&partial_transport.loader().save()==&partial_save&&partial_save.slot()==4&&partial_save.character()==character);
 unsigned partial_stored=0;for(unsigned i=0;i<224;++i){const auto type=rules.types[i]==-1?16u:std::uint32_t(rules.types[i]);
  const auto expected=(type&0x20)?std::int32_t(0x50000000+i):-11;partial_stored+=(type&0x20)!=0;
  require(partial_state.saved[i]==expected&&partial_state.resolved[i]==-13);
 }require(partial_stored==stored&&partial_save.saved_properties_byte_194()==0);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"character_class\":"<<id<<",\"saved_properties_written\":"<<stored<<",\"saved_level_defaults\":"<<defaults<<",\"same_six_saved_arrays\":true,\"distinct_preview_gameplay_owners\":true,\"source_class_loader_calls\":2,\"scope\":\"selected-library host composition; native startup association and complete InitPost remain unbound\"}\n";
 return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
