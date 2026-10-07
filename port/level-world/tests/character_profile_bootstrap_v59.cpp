#include "../character_profile_bootstrap_v59.hpp"
#include "../private_save_file_transport_v45.hpp"
#include "../../game-data/fresh_player_profile_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
using namespace dh2;
namespace {
unsigned checks{};
void check(bool v,const std::string& message){++checks;if(!v)throw std::runtime_error(message);}
std::vector<std::uint8_t> file(const std::string& name){std::ifstream f(name,std::ios::binary);if(!f)throw std::runtime_error("Required actual cache "+name);return {std::istreambuf_iterator<char>(f),{}};}
data::Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
}
int main(){try{
 std::string e;const std::string assets="port/android-native/app/src/main/assets/data/";
 auto a=file(assets+"character_properties_pyarray.bin"),b=file(assets+"character_properties_pyarraynames.bin"),c=file(assets+"character_properties_pystructnames.bin");
 data::CharacterTable characters;check(data::load_characters(bytes(a),bytes(b),bytes(c),characters,e),e);
 a=file(assets+"skills_pyarray.bin");b=file(assets+"skills_pyarraynames.bin");c=file(assets+"skills_pystructnames.bin");
 data::SkillTables skills;check(skills.load(bytes(a),bytes(b),bytes(c),e),e);
 const std::string quest_cache="port/level-world/reference/character-menu-profile-v51/cache/";
 a=file(quest_cache+"v2quests_pyarray.bin");b=file(quest_cache+"v2quests_pyarraynames.bin");
 auto quest_tables=std::make_shared<data::QuestTablesPersistenceV51>();check(quest_tables->decode(bytes(a),bytes(b),e),e);
 auto selected=std::make_shared<data::FreshPlayerProfileV1>();
 check(data::fresh_player_profile_v1(characters,characters.names.at(0).c_str(),"Source V59",123,456,*selected,e),e);
 char folder[]="/tmp/dh2-profile-v59-XXXXXX";check(::mkdtemp(folder)!=nullptr,"actual private fixture directory");
 auto transport=std::make_shared<level::PrivateSaveFileTransportV45>(folder,1024*1024);
 auto jobs=std::make_shared<level::SavegameJobsOwnerV2>(transport->services());
 auto stream=std::make_unique<level::SavegameStreamV2>(bytes(selected->bytes));
 check(jobs->add_write("dh2_000.savegame",std::move(stream),e)&&jobs->flush(nullptr,e),e);
 auto profile=std::make_shared<level::CampaignSaveProfileV45>("dh2_000.savegame",transport->services(),jobs);
 check(profile->construct(e),e);
 auto save=std::make_shared<data::PlayerSavegameV1>();save->set_character(0x1234);
 auto load=std::make_shared<data::PlayerSaveLoadOwnerV1>(save);auto lease=std::make_shared<int>(1);
 std::uintptr_t slot14e8=reinterpret_cast<std::uintptr_t>(save.get());std::int32_t slot664=0,difficulty=-1;
 unsigned slot_stores{};
 std::vector<std::int32_t> levels,maps; // Explicit empty collection fixture; not campaign Level initialization.
 character::CharacterProfileBootstrapInputsV59 input;input.save=save;input.load=load;input.profile=profile;
 input.selected_file_lease=selected;input.selected_file_bytes=bytes(selected->bytes);input.selected_slot=0;
 input.character=save->character();input.actual_source_cells_lease=lease;input.source_save14e8=&slot14e8;
 input.source_player_info_slot664=&slot664;input.source_set_slot=[&](auto value,auto&){++slot_stores;save->set_slot(value);return true;};
 input.reads.tables=lease;input.reads.characters=&characters;input.reads.skills=skills.borrow();
 input.reads.level_defaults28=&levels;input.reads.map_defaults8=&maps;
 input.reads.quests=std::make_shared<character::CharacterMenuQuestsV51>(save,quest_tables);
 input.reads.character_skill_list=[&](auto id,const auto*& out,auto& error){if(id!=save->character()){error="Wrong actual fixture Character";return false;}out=&skills.borrow().lists().at(0);return true;};
 input.reads.store_current_difficulty=[&](auto value,auto&){difficulty=value;return true;}; // Explicit global-store fixture.
 // Wrong source14e8 must fail before source slot/field writes.
 auto wrong=input;std::uintptr_t unrelated=0;wrong.source_save14e8=&unrelated;
 auto rejected=std::make_shared<character::CharacterProfileBootstrapV59>(std::move(wrong));
 check(!rejected->prepare(e)&&slot_stores==0&&save->slot()==-1&&!load->profile().identity,"same-source-slot preflight");
 auto owner=std::make_shared<character::CharacterProfileBootstrapV59>(input);
 check(owner->prepare(e),e);check(slot_stores==1&&save->slot()==0&&save->name()=="Source V59"&&save->level()==1,"actual seven-section fields");
 check(save->skills_initialized()&&save->regular_quests_v45().initialized()&&save->volatile_quests_v45().initialized(),"same source initialized persistence cells");
 check(load->profile().identity==reinterpret_cast<std::uintptr_t>(profile.get())&&&load->save()==save.get(),"one Save/profile authority");
 check(!owner->prepare(e)&&slot_stores==1,"no source prefix retry");
 player::PlayerEquipmentRenderInputsV1 equipment;equipment.character=save->character();
 check(owner->configure_equipment(equipment,e)&&bool(equipment.source_profile_load_v59)&&!equipment.saved_gear_v50,"single actual source Gear transport");
 equipment.saved_gear_v50=[](auto&,auto&,auto&,auto&){return true;};
 check(!owner->configure_equipment(equipment,e),"cannot bind both Gear loaders");
 check(!owner->gear_delivered()&&!owner->finished(),"pre-Gear stage does not claim equipment/whole player ready");
 check(difficulty==0,"actual PDFL source store fixture");
 check(jobs->release(e),e);::unlink((std::string(folder)+"/dh2_000.savegame").c_str());check(::rmdir(folder)==0,"fixture cleanup");
 std::cout<<"PASS "<<checks<<" actual-profile prepare guards; source fixtures only for slot/global/skill-list selectors; Gear test is separate\n";return 0;
 }catch(const std::exception& x){std::cerr<<x.what()<<'\n';return 1;}}
