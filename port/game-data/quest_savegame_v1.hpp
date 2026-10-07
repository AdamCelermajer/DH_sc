#pragma once
#include "data.hpp"
namespace dh2::data {
struct SavedQuestProgressV1 {
 std::array<std::int32_t,3> current_quest{{-1,-1,-1}},primary_quest{{-1,-1,-1}};
 std::array<std::int32_t,3> current_act{{1,1,1}},compatible_act{{1,1,1}};
};
// Quest::_loadQuestData owns its condition and objective objects. This boundary
// supplies the actual initialized Quest identity and the remaining source span;
// successful delivery must report its consumed bytes. No fallback item layout.
struct QuestLoadServicesV1 {
 void* context{};
 bool (*load_quest)(void*,std::uintptr_t,Bytes,bool volatile_state,std::size_t& consumed,std::string&){};
};
class QuestSavegameV1 {
 std::array<std::vector<std::uintptr_t>,3> quests_;
 SavedQuestProgressV1 progress_;
 //QuestSavegame C1 46b0a4: these source cells are zero, independently of
 //the three quest arrays and the Save synchronization byte14.
 std::array<std::uint8_t,3> compiled28_{};
 std::uintptr_t character5c_{};
 bool initialized_{};
public:
 // Caller must supply identities produced by the authored quest constructor.
 // Owns the arrays, while Quest objects remain with their actual world owner.
 bool attach_initialized_quests(const std::array<std::vector<std::uintptr_t>,3>&,std::string&);
 bool initialized()const noexcept{return initialized_;}
 //Constructor-empty QuestSavegame D1 has no positive Quest receiver calls.
 //Never use this branch to bypass owned populated Quest D0.
 bool source_destroy_empty_v108(std::string& e){
  for(const auto& list:quests_)if(!list.empty()){e="QuestSavegame D1 requires actual positive Quest destructors";return false;}
  for(auto& list:quests_)std::vector<std::uintptr_t>{}.swap(list);
  initialized_=false;e.clear();return true;
 }
 bool source_store_progress_v108(std::uint32_t field,std::int32_t value,std::int32_t difficulty,std::string& e){
  if(difficulty<0||difficulty>=3){e="Original Quest progress difficulty outside actual three cells";return false;}
  auto index=static_cast<std::size_t>(difficulty);
  if(field==0x2c)progress_.current_quest[index]=value;
  else if(field==0x38)progress_.primary_quest[index]=value;
  else if(field==0x44){if(value>progress_.current_act[index])progress_.current_act[index]=value;}
  else {e="Uncaptured Quest progress source field";return false;}
  e.clear();return true;
 }
 const SavedQuestProgressV1& progress()const noexcept{return progress_;}
 const auto& source_quests_v45()const noexcept{return quests_;}
 //Mutable SAME vector cells used only by original qualified D1/free sequence.
 auto& source_quest_arrays_d1_v108()noexcept{return quests_;}
 const std::uintptr_t* source_character5c_v70()const noexcept{return &character5c_;}
 std::array<std::uint8_t*,3> source_compiled28_v70()noexcept{return {{&compiled28_[0],&compiled28_[1],&compiled28_[2]}};}
 //Only the actual Character.SG_SetPlayer3bb754 publication writes this.
 //It does not reset a previously compiled collection or any quest state.
 void source_set_character5c_v70(std::uintptr_t character)noexcept{character5c_=character;}
 void set_current_acts(const std::array<std::int32_t,3>& acts)noexcept{progress_.current_act=acts;}
 // Original UnpackQuests: count mismatch is a successful early return after
 // its count word, without consuming entries or trailing progress words.
 // Reached state writes remain on delivery/truncation failure.
 bool unpack(std::uint32_t difficulty,Bytes,bool volatile_state,const QuestLoadServicesV1&,
             std::size_t& consumed,bool& count_mismatch,std::string&);
 // Original LoadQuests calls unpack for all three difficulties, including
 // after a count mismatch. Each item receives volatile_state=false.
 bool load(Bytes,const QuestLoadServicesV1&,std::size_t& consumed,
           std::array<bool,3>& count_mismatch,std::string&);
};
// Original PlayerSavegame::__LoadQuests reads regular then seeks back and
// reads volatile from the same QEST payload. Both states retain reached writes.
bool load_player_quests_v1(Bytes,QuestSavegameV1& regular,QuestSavegameV1& volatile_quests,
 const QuestLoadServicesV1&,std::size_t& consumed,std::string&);
}
