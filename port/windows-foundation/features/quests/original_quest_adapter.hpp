#pragma once
#include "../../../level-world/native_quest_runtime_v76.hpp"
#include "../../../level-world/quest_condition_compile_v70.hpp"
#include "../../original_campaign_runtime.hpp"

namespace dh::foundation {
// Durable source identity. Native receiver addresses are resolved on each use.
struct OriginalQuestId {
    std::uint32_t collection{}; // source regular0 / volatile1
    std::int32_t difficulty{}, row{};
};
struct OriginalQuestObjectiveView {
    dh2::data::QuestObjectiveDefinitionV51 authored;
    std::uint8_t completed{};
    std::int32_t quantity{};
};
struct OriginalQuestView {
    OriginalQuestId id;
    std::string source_name;
    std::array<std::int32_t,4> text_ids{};
    std::array<std::string,4> localized_text;
    std::int32_t state{},priority{},act{},target_level{};
    bool repeatable{};
    OriginalQuestObjectiveView accept,end;
    std::vector<OriginalQuestObjectiveView> objectives;
    std::vector<dh2::data::QuestRewardDefinitionV51> authored_rewards;
};
using QuestTextProvider = std::function<bool(std::int32_t,std::string&,std::string&)>;

// Borrows SAME CharacterMenuQuests / Save / Condition / Objective owners.
// Initialization, restoration and publication belong to the character owner.
class OriginalQuestAdapter {
    std::shared_ptr<dh2::character::CharacterMenuQuestsV51> owner_;
public:
    explicit OriginalQuestAdapter(std::shared_ptr<dh2::character::CharacterMenuQuestsV51> owner)
        :owner_(std::move(owner)){}
    const auto& owner() const noexcept { return owner_; }
    bool resolve(OriginalQuestId,dh2::data::QuestPersistenceStateV51*&,std::string&) const;
    bool list(std::uint32_t collection,std::int32_t difficulty,
              const QuestTextProvider&,std::vector<OriginalQuestView>&,std::string&) const;
    // CompileQuests preserves source cache28 store BEFORE ordered compile;
    // native interrupted prefixes are retained, never retried as successful.
    bool compile(std::uint32_t collection,bool force,
        const dh2::world::QuestConditionCompileServicesV70&,std::string&);
    bool update(std::uint32_t collection,
        const dh2::world::QuestConditionCompileServicesV70&,std::string&);
    bool prerequisites(OriginalQuestId,bool&,std::string&);
    const dh2::data::SavedQuestProgressV1* progress(std::uint32_t collection) const;
    // SAME source stores: CurrentQuest2c, PrimaryQuest38, monotonic Act44.
    bool store_progress(std::uint32_t collection,std::uint32_t source_field,
                        std::int32_t value,std::int32_t difficulty,std::string&);
};
// Only script leaves are supplied here. Gate/time/constants/dialog/rewards/
// network/local/trophy leaves must be bound to actual providers by the host.
void bind_original_quest_scripts(dh2::world::NativeQuestFrameServicesV108&,
                                 const std::shared_ptr<OriginalCampaignRuntime>&);
}
