#pragma once
#include "original_quest_adapter.hpp"
namespace dh::foundation {
enum class QuestLogCategoryV108 { active, closed };
struct QuestLogEntryV108 { OriginalQuestId id; std::string title; bool current{}; };
struct QuestLogDetailsV108 {
    OriginalQuestId id;
    std::string source_name;
    std::array<std::int32_t,4> text_ids{};
    std::array<std::string,4> text;
    bool primary{};
};
// Native GetQuestFunctor category selection remains an explicit original
// provider. The menu adapter only enumerates/sorts the actual same-owner rows.
using QuestLogFunctorV108 = std::function<bool(
    const dh2::data::QuestPersistenceStateV51&,QuestLogCategoryV108,bool&,std::string&)>;
using QuestLogTextV108 = QuestTextProvider;
// Composes the exact scoped QE_TalkToNPC projection produced by the source
// interaction owner ahead of any other genuine event-family projector.
bool bind_source_quest_event_projection_v108(dh2::loader::GameEventRuntimeServicesV75&,std::string&);
// Constructs only the missing actual source quest owner on an existing fresh
// PlayerSavegame; caller publishes the resulting complete Character owner graph.
bool construct_source_quest_owner(const std::shared_ptr<dh2::data::PlayerSavegameV1>&,
    const std::shared_ptr<const dh2::data::QuestTablesPersistenceV51>&,
    std::shared_ptr<dh2::character::CharacterMenuQuestsV51>&,std::string&);
// One genuine owner slot in world/level graph. No replacement or second arena.
bool construct_source_condition_runtime(
    const std::shared_ptr<const dh2::world::NativeConditionTableV69>&,
    dh2::world::NativeConditionServicesV69,
    std::shared_ptr<dh2::world::NativeConditionRuntimeV69>&,std::string&);
bool construct_source_objective_runtime(
    const std::shared_ptr<dh2::loader::GameEventManagerV50>&,
    dh2::loader::GameEventRuntimeServicesV75,
    std::shared_ptr<dh2::loader::GameEventRuntimeV75>&,std::string&);
// Every callback borrows its actual existing Character/PM/App/Level owner.
// A delivered null local character is an original branch, not absent transport.
struct SourceQuestServices {
    std::shared_ptr<void> world_owner;
    std::shared_ptr<OriginalCampaignRuntime> campaign;
    std::function<bool(std::shared_ptr<dh2::character::CharacterMenuQuestsV51>&,std::string&)> local_quests;
    std::function<bool(std::uintptr_t,std::shared_ptr<dh2::character::CharacterMenuQuestsV51>&,std::string&)> character_quests;
    std::function<bool(std::uintptr_t,std::int32_t&,std::string&)> difficulty;
    std::function<bool(bool&,std::string&)> online,local_hosting,online_script_admission;
    std::function<bool(const char*,const char*,std::int32_t&,std::string&)> constant;
    std::function<bool(std::uint32_t&,std::string&)> application_time70;
    // Actual PlayerSaveQuestSyncOwnerV3::try_sync producer, sole ready14 writer.
    std::function<bool(std::uintptr_t,std::string&)> synchronize_quests;
    std::function<bool(std::string&)> transition_save,online_activation,online_act;
    std::function<bool(const dh2::data::QuestPersistenceStateV51&,std::string&)> new_dialog;
    std::function<bool(const dh2::data::QuestPersistenceStateV51&,const std::vector<dh2::data::QuestRewardDefinitionV51>&,std::string&)> completed_dialog;
    std::function<bool(std::uintptr_t,const dh2::data::QuestRewardDefinitionV51&,bool&,std::string&)> give_reward;
    std::function<bool(std::uintptr_t,bool&,std::string&)> is_local;
    std::function<bool(const char* source_trophy_name,std::string&)> unlock_trophy;
    std::function<bool(std::int32_t&,std::string&)> assertion_mode;
    std::function<bool(const char*,std::int32_t,const char*,std::string&)> assertion;
};
// Reusable native transport; owns no Quest/Save/Character/progression copies.
// SourceQuestServices callbacks must weakly borrow the owner graph to avoid a
// Quest -> runtime -> service transport -> Quest ownership cycle.
class SourceQuestServiceBinding : public std::enable_shared_from_this<SourceQuestServiceBinding> {
    SourceQuestServices source_;
    bool selected(std::shared_ptr<dh2::character::CharacterMenuQuestsV51>,dh2::data::QuestSavegameV1*&,std::int32_t&,std::string&);
    bool progress(std::uint32_t,std::int32_t,std::int32_t,std::string&);
    bool all_quests(std::uintptr_t,std::string&);
public:
    explicit SourceQuestServiceBinding(SourceQuestServices services):source_(std::move(services)){}
    bool frame_services(dh2::world::NativeQuestFrameServicesV108&,std::string&);
    dh2::world::QuestConditionCompileServicesV70 compile_services();
    // Adopts existing SAME Condition/Objective owners; one native runtime slot.
    bool publish(const std::shared_ptr<dh2::character::CharacterMenuQuestsV51>&,
                 dh2::world::NativeQuestRuntimeServicesV76,std::string&);
    // Source Character.SG_GetQuestByID(index,requestedDiff): compile first,
    // original negative/outside row returns a genuine NULL quest state.
    bool quest_state(std::uintptr_t,std::int32_t row,std::int32_t requested_difficulty,
                     dh2::world::NativeConditionStateV69&,std::string&);
    bool update(std::uintptr_t character,bool force_compile,std::string&);
    // Quest Log page read/detail/activate surface. All IDs resolve against the
    // currently selected regular/online collection and the same Character Save.
    bool quest_log(QuestLogCategoryV108,const QuestLogFunctorV108&,
                   const QuestLogTextV108&,std::vector<QuestLogEntryV108>&,std::string&);
    bool quest_log_details(OriginalQuestId,const QuestLogTextV108&,
                           QuestLogDetailsV108&,std::string&);
    bool quest_log_activate(OriginalQuestId,const QuestLogFunctorV108&,std::string&);
};
}
