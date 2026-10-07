#pragma once
#include "native_conditions_v69.hpp"
#include "quest_savegame_v1.hpp"
#include "quest_condition_compile_v70.hpp"
namespace dh2::character {class CharacterMenuQuestsV51;}
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
struct SourceWorldBorrowV61;
struct SourceConditionDependenciesV70 {
 //These are real process/global/Quest gameplay owners, never the containing
 //World itself. Query callbacks must keep the containing World weak.
 std::shared_ptr<void> assertion_owner;
 const std::int32_t* assertion_mode{};
 std::function<bool(const char*,std::int32_t,const char*,std::string&)> assertion;
 std::shared_ptr<void> quest_compile_owner;
 //Supplies the actual whole Quest.Compile leaf. The V70
 //native kernel owns the whole CompileQuests(false) walk/store/query ordering;
 //providers cannot replace that body with a saved-state peek or success flag.
 std::function<bool(const std::shared_ptr<dh2::character::CharacterMenuQuestsV51>&,
                    std::uintptr_t quest,std::string&)> quest_compile480178;
};
bool borrow_source_campaign_conditions_v70(const SourceCampaignCandidateBorrowV55&,
 dh2::world::ConditionDataInitServicesV3&,
 std::shared_ptr<dh2::world::NativeConditionRuntimeV69>& actual_owner,std::string&);
bool bind_source_campaign_condition_dependencies_v70(const SourceCampaignCandidateBorrowV55&,
 SourceConditionDependenciesV70,std::string&);
}
