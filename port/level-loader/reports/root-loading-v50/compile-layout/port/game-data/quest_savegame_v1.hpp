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
 bool initialized_{};
public:
 // Caller must supply identities produced by the authored quest constructor.
 // Owns the arrays, while Quest objects remain with their actual world owner.
 bool attach_initialized_quests(const std::array<std::vector<std::uintptr_t>,3>&,std::string&);
 bool initialized()const noexcept{return initialized_;}
 const SavedQuestProgressV1& progress()const noexcept{return progress_;}
 const auto& source_quests_v45()const noexcept{return quests_;}
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
