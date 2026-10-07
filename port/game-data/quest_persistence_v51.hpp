#pragma once
#include "quest_savegame_v1.hpp"
#include <memory>
#include <functional>
#include <optional>
namespace dh2::data {
// Native projections of the actual v2Quests PyArray. No legacy vtable word is
// installed or used as a native dispatch address.
struct QuestObjectiveDefinitionV51 {
 std::int32_t type{},description{},on_complete{},oid1{},oid2{},value{};
 std::string str1,str2;
};
struct QuestRewardDefinitionV51 {std::int32_t type{},parameter1{},parameter2{};};
struct QuestConditionDefinitionV51 {std::int32_t type{},parameter1{},parameter2{};};
struct QuestDefinitionV51 {
 std::string name;
 std::array<std::int32_t,4> text_fields{};
 std::vector<QuestConditionDefinitionV51> prerequisites;
 std::vector<QuestObjectiveDefinitionV51> objectives;
 std::array<std::vector<QuestRewardDefinitionV51>,3> rewards;
 QuestObjectiveDefinitionV51 accept,end;
 std::int32_t target_level{},state{},priority{},act{};
 std::uint8_t repeatable{};
 std::array<std::string,14> scripts;
};
class QuestTablesPersistenceV51 {
 std::vector<QuestDefinitionV51> rows_;
 bool ready_{};
public:
 // Source field order: Structs::v2Quest::read504a94 and children. Native
 // allocation guards are bounded by the actual supplied input span.
 bool decode(Bytes array,Bytes names,std::string&);
 const auto& rows()const noexcept{return rows_;}
 bool ready()const noexcept{return ready_;}
};
struct QuestObjectivePersistenceV51 {
 const QuestObjectiveDefinitionV51* definition{};
 std::uintptr_t character_owner{};
 std::uint8_t completed14{};
 std::int32_t quantity20{};
 //Missing native runtime cells from the SAME Objective C1. The saved14/20
 //prefix above remains the persistence authority; these are never copies.
 std::int32_t source_type4{-1};
 std::uint8_t compiled8{};
 std::optional<std::uint8_t> receiver1c;
 std::array<std::optional<std::uint32_t>,3> words24_2c;
 std::uintptr_t marker28{}; //native pointer-width TalkToNPC source model
 bool runtime_ctor_produced_v76{};
};
struct QuestPersistenceStateV51 {
 const QuestDefinitionV51* definition{};
 std::uintptr_t character_owner{};
 std::int32_t difficulty{},index{},state{-1};
 std::uint8_t volatile64{};
 QuestObjectivePersistenceV51 accept,end;
 std::vector<QuestObjectivePersistenceV51> objectives;
 std::uint8_t rewards_enabled5c{1}; //Quest C1 48092c, distinct volatile64.
 std::uintptr_t prerequisites20{}; //SAME native ConditionList C1/AssignPyData.
 std::uint32_t state_date4{};std::uint8_t completed5d{}; //actual Quest C1 480900/934
};
// Owns the source persistence cells, with real native identity addresses, for
// ONE QuestSavegame. Gameplay Register/Unregister/evaluation is outside this
// stage and is never reported accepted by its wire methods.
class QuestPersistenceOwnerV51 {
 std::shared_ptr<const QuestTablesPersistenceV51> tables_;
 std::array<std::vector<std::unique_ptr<QuestPersistenceStateV51>>,3> rows_;
 bool attempted_{},ready_{};
 static bool load_callback(void*,std::uintptr_t,Bytes,bool,std::size_t&,std::string&);
public:
 bool construct(std::shared_ptr<const QuestTablesPersistenceV51>,std::uintptr_t actual_character_owner,
                QuestSavegameV1& same_collection,std::string&);
 QuestLoadServicesV1 load_services(){return {this,&load_callback};}
 bool load_quest(std::uintptr_t,Bytes,std::size_t&,std::string&);
 bool save_quest(std::uintptr_t,std::vector<std::uint8_t>&,std::string&)const;
 // Source ReInit old-state branches2/3/5/6/8/9 require genuine objective
 // registration/marker services. Other states need no such callback.
 bool reinitialize(const std::function<bool(QuestPersistenceStateV51&,std::int32_t,std::string&)>&,
                   std::string&);
 bool destroy_source_v108(QuestSavegameV1&,const std::function<bool(std::uintptr_t,std::string&)>&,std::string&);
 QuestPersistenceStateV51* resolve(std::uintptr_t)const noexcept;
 bool ready()const noexcept{return ready_;}
};
// Original menu metadata owns temporary Quest receivers with Character owner0.
// Preserves incoming LNAM acts until the reached QEST stores replace them.
bool load_quest_metadata_acts_v51(std::shared_ptr<const QuestTablesPersistenceV51>,Bytes,
 std::array<std::int32_t,3>& regular,std::array<std::int32_t,3>& volatile_acts,std::string&);
}
