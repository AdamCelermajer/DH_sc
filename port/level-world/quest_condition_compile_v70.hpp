#pragma once
#include "../game-data/quest_savegame_v1.hpp"
#include <functional>
#include <memory>
namespace dh2::world {
//Actual QuestSavegame fields only: byte28/29/2a C1 zeros and Character5c
//published by its real initializer. No side cache or synthesized flags.
struct QuestConditionCompileBorrowV70 {
 std::shared_ptr<void> receiver;
 data::QuestSavegameV1* collection{};
 const std::uintptr_t* character5c{};
 std::array<std::uint8_t*,3> compiled28{};
};
struct QuestConditionCompileServicesV70 {
 std::shared_ptr<void> transport;
 std::function<bool(std::uintptr_t,std::int32_t&,std::string&)> character_difficulty3bb8e4;
 std::function<bool(std::uintptr_t,std::string&)> quest_compile480178;
};
//Whole CompileQuests46b824: false flag cache branch, repeated SAME Character
//difficulty queries, source flag store BEFORE actual ordered Quest.Compile.
bool quest_condition_compile_v70(const QuestConditionCompileBorrowV70&,bool force,
 const QuestConditionCompileServicesV70&,std::string&);
}
