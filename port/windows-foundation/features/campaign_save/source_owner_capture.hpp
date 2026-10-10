#pragma once
#include "campaign_snapshot.hpp"
#include "../../../level-world/character_menu_quests_v51.hpp"

namespace dh::foundation::campaign_save {
// Registers complete implemented codecs on SAME source owners. This is a
// capture/validation inventory, never a second Character or live restore graph.
class SourceOwnerCapture : public std::enable_shared_from_this<SourceOwnerCapture> {
    struct Binding {
        SourceRequirement requirement;
        std::shared_ptr<void> lease;
        std::function<bool(SourceSection&,std::string&)> capture;
        std::function<bool(const SourceSection&,std::string&)> validate;
    };
    std::vector<Binding> bindings_;
    std::vector<const void*> quest_owners_;
public:
    bool add_quest_collections(const std::shared_ptr<dh2::character::CharacterMenuQuestsV51>&,
        std::shared_ptr<void> actual_character_lease,
        const std::array<std::string,2>& regular_volatile_keys,
        const std::string& definition_revision,std::string& error);
    // Unsupported REQUIRED fragments stay in the inventory, so capture fails
    // rather than silently saving only a supported subset of an owner.
    bool require_unsupported(SourceRequirement,std::string& error);
    SourceCaptureServices services();
};
} // namespace dh::foundation::campaign_save
