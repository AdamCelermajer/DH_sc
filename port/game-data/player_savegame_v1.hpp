#pragma once
#include "skill_tables.hpp"
#include "quest_savegame_v1.hpp"
#include <array>
#include <map>
#include <functional>
namespace dh2::data {
struct SavedSkill8V1 {std::int32_t id;std::uint16_t level;std::uint8_t flag,reserved;};
struct PlayerProfileSpan24V1 {const std::uint8_t* data;std::uint32_t size,cursor,reserved0,reserved1;};
struct SavedSkillsView16V1 {SavedSkill8V1* rows;std::uint32_t count,reserved;};
struct SavedSkillsLoadServices32V1 {
 void* context;
 std::int32_t (*skill_id)(void*,const std::uint8_t*,std::uint32_t);
 std::uint32_t* (*slot_value)(void*,std::uint32_t set,std::int32_t key);
 void (*read_observer)(void*,std::uint32_t bytes);
};
static_assert(sizeof(SavedSkill8V1)==8&&sizeof(PlayerProfileSpan24V1)==24);
static_assert(sizeof(SavedSkillsView16V1)==16&&sizeof(SavedSkillsLoadServices32V1)==32);
struct SavedSkillUpdateServicesV1 {void* context{};bool (*update_skills)(void*,std::uintptr_t character,std::string&){};};
struct SavedFaery4V1 {std::uint8_t state,reserved;std::uint16_t level;};
struct SavedLocationV1 {
 std::uint32_t save_date{};
 std::array<std::int32_t,3> levels{},seeds{};
 std::array<std::int32_t,3> current_acts{{1,1,1}},volatile_acts{{1,1,1}};
 std::array<std::int32_t,3> entry_points{};
 std::array<std::uint8_t,3> use_spawn_point{};
};
static_assert(sizeof(SavedFaery4V1)==4);
// Owns the recovered PlayerSavegame fields used by HUD. A blank constructor is
// source state, not a ready campaign player. The campaign section index, quests,
// quests and campaign creation/class/gear producers remain separate owners.
class PlayerSavegameV1 {
 friend class PlayerSaveQuestSyncOwnerV3;
 std::int32_t slot_{-1},level_{},class_{-1};std::string name_;
 std::uintptr_t character_{};bool skills_initialized_{};
 std::vector<SavedSkill8V1> skills_;std::array<std::map<std::int32_t,std::uint32_t>,2> slots_;
 std::array<std::array<SavedFaery4V1,5>,3> faeries_{};std::array<bool,3> faeries_initialized_{};
 std::array<std::int32_t,3> current_faery_{};std::int32_t unlocked_difficulty_{};
 SavedLocationV1 location_;
 std::uint8_t quest_sync_ready14_{}; // both original C1 variants store0
 // Additive original Save fields: constructor-null level/map allocations,
 // three bitsets17c..193 zero and raw PROP tail194 zero. Restore writes these
 // same cells; source initialization never replays over an existing array.
 std::array<std::vector<std::int32_t>,3> level_states68_,map_states74_;
 //Actual default C1 stores all three94 pointers NULL at465c04. No positive
 //allocation producer is reconstructed yet; this owns any admitted native
 //raw storage rather than borrowing Character properties as that allocation.
 std::array<std::vector<std::uint8_t>,3> source_arrays94_v108_;
 std::array<bool,3> level_states_present_{},map_states_present_{};
 std::array<std::uint64_t,3> fast_travel17c_{};
 std::uint8_t property_tail194_{};
 QuestSavegameV1 regular_quests_b8_,volatile_quests118_;
 bool destroyed_v108_{};
public:
 PlayerSavegameV1()=default;
 bool destroy_fields_v108(const std::function<bool(std::uint32_t,std::string&)>& quests,std::string& e){
  if(destroyed_v108_){e.clear();return true;}
  for(std::size_t i=0;i<3;++i){
   std::vector<std::int32_t>{}.swap(level_states68_[i]);level_states_present_[i]=false;
   std::vector<std::int32_t>{}.swap(map_states74_[i]);map_states_present_[i]=false;
   std::vector<std::uint8_t>{}.swap(source_arrays94_v108_[i]);
  }
  std::vector<SavedSkill8V1>{}.swap(skills_);
  if(!quests||!quests(1,e)||!quests(0,e))return false;
  for(auto& slots:slots_)std::map<std::int32_t,std::uint32_t>{}.swap(slots);
  std::string{}.swap(name_);destroyed_v108_=true;e.clear();return true;
 }
 bool destroyed_v108()const noexcept{return destroyed_v108_;}
 PlayerSavegameV1(const PlayerSavegameV1&)=delete;PlayerSavegameV1& operator=(const PlayerSavegameV1&)=delete;
 void set_character(std::uintptr_t identity)noexcept{
  //Character.SG_SetPlayer3bb754: Save174, Save10, Save114, in that order.
  volatile_quests118_.source_set_character5c_v70(identity);
  character_=identity;
  regular_quests_b8_.source_set_character5c_v70(identity);
 }
 void set_slot(std::int32_t slot)noexcept{slot_=slot;}
 const std::int32_t* source_slot_field_v122()const noexcept{return &slot_;}
 bool initialize_skills(const std::vector<std::int32_t>& actual_character_skill_list,std::string&);
 // Original named skills section. Retains reached writes on truncated input;
 // source duplicate entries overwrite, missing names consume/discard levels,
 // loaded maps are merged without clearing or validating saved index values.
 int load_skills(Bytes,SkillTables::Borrow,std::size_t& consumed,std::string&);
 // Only the actual name/level/class named sections; no new outer file format.
 bool load_name(Bytes,std::size_t&,std::string&);
 bool load_level(Bytes,std::size_t&,std::string&);
 bool load_class(Bytes,const std::vector<std::string>& genuine_class_names,std::size_t&,std::string&);
 // Original __LoadLevelName (LNAM): date then three level/seed/current-act
 // triples. Each act store reaches both regular and volatile quest owners.
 // Prefix stores remain on truncation, matching the existing bounded readers.
 bool load_location(Bytes,std::size_t&,std::string&);
 // Original LEPT/LUSP readers; spawn flags retain their raw saved bytes.
 bool load_entry_points(Bytes,std::size_t&,std::string&);
 bool load_spawn_points(Bytes,std::size_t&,std::string&);
 const SavedLocationV1& location()const noexcept{return location_;}
 //Exact PlayerSavegame.SG_SetSaveDate467744: unsigned time(NULL) at Save38.
 void source_save_date_store_v88(std::uint32_t value)noexcept{location_.save_date=value;}
 // Exact SG_SetLevelEntryPoint3bb89c target: same Save+40/+44/+48 cells.
 // Character's NULL-save/default-difficulty gates precede this store.
 bool source_store_level_entrypoint_v68(std::int32_t value,std::int32_t difficulty,std::string& error){if(difficulty<0||difficulty>=3){error="Original SG_SetLevelEntryPoint difficulty index outside retained Save cells";return false;}location_.entry_points[std::size_t(difficulty)]=value;error.clear();return true;}
 void initialize_faeries()noexcept; // Source literal five rows, three difficulties.
 // Actual SG_SetUseSpawnPoint and SG_SetLevelId location cells; no disk IO.
 bool source_store_use_spawn_point_v84(bool value,std::int32_t difficulty,std::string& error){
  if(difficulty<0||difficulty>=3){error="Original SG_SetUseSpawnPoint index outside retained Save cells";return false;}
  location_.use_spawn_point[std::size_t(difficulty)]=value?1:0;error.clear();return true;
 }
 bool source_store_level_id_v84(std::int32_t value,std::int32_t difficulty,std::string& error){
  if(difficulty<0||difficulty>=3){error="Original SG_SetLevelId index outside retained Save cells";return false;}
  location_.levels[std::size_t(difficulty)]=value;error.clear();return true;
 }
 bool load_current_faery(Bytes,std::size_t&,std::string&);
 bool load_faeries(Bytes,std::size_t&,bool& source_count_mismatch,std::string&);
 // The first difficulty word belongs to source global CurrentDifficulty;
 // caller must deliver that store before the second per-savegame word read.
 bool load_difficulty(Bytes,void* context,bool (*store_selected)(void*,std::int32_t,std::string&),std::size_t&,std::string&);
 bool set_faery_level(std::uint32_t id,std::int32_t value,std::uint32_t difficulty,std::string&);
 bool set_faery_state(std::uint32_t id,std::int32_t value,std::uint32_t difficulty,std::string&);
 std::int32_t faery_level(std::uint32_t id,std::uint32_t difficulty)const noexcept;
 std::int32_t current_faery(std::uint32_t difficulty)const noexcept;
 // Source Character::SG_SetCurrentFaerie3bb9d8 stores raw ID without unlock
 // validation. Caller resolves original difficulty=-1 via its live World.
 bool set_current_faery(std::uint32_t id,std::uint32_t difficulty,std::string&);
 const std::array<std::array<SavedFaery4V1,5>,3>& faeries()const noexcept{return faeries_;}
 const std::array<bool,3>& faeries_initialized()const noexcept{return faeries_initialized_;}
 const std::array<std::int32_t,3>& current_faeries()const noexcept{return current_faery_;}
 std::int32_t unlocked_difficulty()const noexcept{return unlocked_difficulty_;}
 // Character::SG_SetPlayerLevel3bb840 stores raw signed level at the SAME
 // Savegame+30. This is progression's retained in-memory source store;
 // original SG_Save serialization/delivery remains a separate provider.
 void set_player_level(std::int32_t value)noexcept{level_=value;}
 // Source Character::SG_SetPlayerClass3bb814 writes raw SAME Savegame+34.
 // The value is a CharacterProperties index, not a selectable UI class ID.
 void set_player_class(std::int32_t value)noexcept{class_=value;}
 bool set_skill_level(std::uint32_t row,std::int32_t level,std::string&);
 bool set_skill_in_slot(std::int32_t slot,std::uint32_t row,const SavedSkillUpdateServicesV1&,std::string&);
 std::int32_t skill_id(std::uint32_t row)const noexcept;
 std::int32_t skill_level(std::uint32_t row)const noexcept;
 std::int32_t skill_in_slot(std::int32_t slot)const noexcept;
 std::int32_t skill_slot(std::uint32_t row)const noexcept;
 bool has_skill_slots()const noexcept{return !slots_[0].empty();}
 const std::vector<SavedSkill8V1>& skills()const noexcept{return skills_;}
 const std::array<std::map<std::int32_t,std::uint32_t>,2>& skill_slots()const noexcept{return slots_;}
 bool skills_initialized()const noexcept{return skills_initialized_;}
 std::int32_t slot()const noexcept{return slot_;}std::int32_t level()const noexcept{return level_;}
 std::int32_t class_id()const noexcept{return class_;}const std::string& name()const noexcept{return name_;}
 std::uintptr_t character()const noexcept{return character_;}
 std::uint8_t source_quest_sync_ready14_v3()const noexcept{return quest_sync_ready14_;}
 bool initialize_level_states_v45(const std::vector<std::int32_t>& actual_level_defaults28,
  const std::vector<std::int32_t>& actual_map_defaults8,std::string&);
 bool load_level_states_v45(Bytes,const std::vector<std::string>& actual_level_names,
  const std::vector<std::string>& actual_map_names,std::size_t&,std::string&);
 bool load_fast_travel_v45(Bytes,std::size_t&,std::string&);
 const auto& level_states_v45()const noexcept{return level_states68_;}
 const auto& map_states_v45()const noexcept{return map_states74_;}
 bool set_level_state_v117(std::int32_t level,std::int32_t state,std::int32_t difficulty,std::string&);
 bool set_map_state_v117(std::int32_t location,std::int32_t state,std::int32_t difficulty,std::string&);
 const auto& fast_travel_v45()const noexcept{return fast_travel17c_;}
 // Whole4663f8 lookup and selected64-bit bitset store. Name misses are the
 // original completed return; invalid positive bit/difficulty is source-unsafe.
 bool source_set_fast_travel_v83(const char* name,bool unlocked,std::int32_t difficulty,
  const std::vector<std::string>& actual_names,std::string& error){
  if(!name){error="Original FastTravel NULL CString";return false;}
  std::size_t index{};for(;index<actual_names.size();++index)if(actual_names[index]==name)break;
  if(index==actual_names.size()){error.clear();return true;}
  if(difficulty<0||difficulty>=3||index>=64){error="Original FastTravel bitset/difficulty outside native source storage";return false;}
  auto& cell=fast_travel17c_[std::size_t(difficulty)];const auto bit=std::uint64_t{1}<<index;
  if(unlocked)cell|=bit;else cell&=~bit;error.clear();return true;
 }
 const std::uint8_t* source_property_tail194_v45()const noexcept{return &property_tail194_;}
 // Call only at __LoadProperties4693cc byte delivery/adopted observed store.
 void load_property_tail194_v45(std::uint8_t actual)noexcept{property_tail194_=actual;}
 QuestSavegameV1& regular_quests_v45()noexcept{return regular_quests_b8_;}
 QuestSavegameV1& volatile_quests_v45()noexcept{return volatile_quests118_;}
 const QuestSavegameV1& regular_quests_v45()const noexcept{return regular_quests_b8_;}
 const QuestSavegameV1& volatile_quests_v45()const noexcept{return volatile_quests118_;}
};
}
extern "C" {
// 0 delivered, -1 malformed native entry, -2 truncated stream, -3 required
// storage service failed. Counts retain original signed-loop semantics. No
// bounds-unsafe source Debug continuation is accepted as a valid native span.
int dh2_player_skills_v1_load(dh2::data::SavedSkillsView16V1*,dh2::data::PlayerProfileSpan24V1*,const dh2::data::SavedSkillsLoadServices32V1*) noexcept;
int dh2_saved_skill_v1_level(const dh2::data::SavedSkillsView16V1*,std::uint32_t) noexcept;
int dh2_saved_skill_v1_set_level(dh2::data::SavedSkillsView16V1*,std::uint32_t,std::int32_t) noexcept;
int dh2_inventory_v1_quantity(std::uint32_t raw16) noexcept;
int dh2_inventory_v1_current_equipment(std::uint32_t raw8,std::int32_t requested) noexcept;
std::uint32_t dh2_inventory_v1_swap_equipment(std::uint32_t raw8) noexcept;
}
