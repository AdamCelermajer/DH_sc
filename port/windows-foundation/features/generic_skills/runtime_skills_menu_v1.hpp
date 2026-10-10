#pragma once

#include "generic_skills_page_v1.hpp"
#include "../character_menu/character_menu.hpp"
#include "../character_menu/source_composition.hpp"
#include "../character_menu/skill_page_text_projection_v1.hpp"

#include <memory>

namespace dh::foundation::generic_skills {

using RuntimeSkillsRendererV1 = std::function<bool(
    const ViewV1&, std::optional<int> selected_position,
    character_menu::Frame&, std::string& error)>;
using RuntimeSkillsReleaseV1 = std::function<bool(
    float authored_x, float authored_y, PageV1&, std::string& error)>;
using RuntimeSkillsActiveSetV1 = std::function<bool(
    const CharacterState&, std::optional<unsigned>& equipment_set, std::string& error)>;
// Exact original NativeSkillsTrainSkill(true, position, player) admission
// query. The provider is borrowed from the same CharacterState/source owner;
// it must not mutate the state.
using RuntimeSkillsTrainAvailableV1 = std::function<bool(
    const CharacterState&, int skill_list_id, int class_skill_position,
    int skill_table_id, bool& available, std::string& error)>;
using RuntimeSkillsSymbolTextV1 = character_menu::SkillPageSymbolTextV1;
class RuntimeSkillsMenuV1 : public std::enable_shared_from_this<RuntimeSkillsMenuV1> {
    std::shared_ptr<CharacterState> state_;
    std::shared_ptr<void> source_owner_;
    const dh2::data::CharacterTable* characters_ = nullptr;
    dh2::data::SkillTables::Borrow tables_;
    std::function<bool(const CharacterState&,
        const character_menu::SkillPageCurrentNextRequestV1&,
        character_menu::SkillPageCurrentNextTextV1&, std::string&)> skill_level_text_;
    PageV1 page_;
    RuntimeSkillsRendererV1 renderer_;
    RuntimeSkillsReleaseV1 release_;
    RuntimeSkillsActiveSetV1 active_set_;
    RuntimeSkillsSymbolTextV1 symbol_text_;
    RuntimeSkillsTrainAvailableV1 train_available_;

    bool ready(std::string&) const;
    bool append(character_menu::Frame&, std::string&);
    bool release(float, float, std::string&);
public:
    RuntimeSkillsMenuV1(std::shared_ptr<CharacterState> same_state,
                        std::shared_ptr<void> same_source_owner,
                        const dh2::data::CharacterTable&,
                        dh2::data::SkillTables::Borrow,
                        ServicesV1,
                        RuntimeSkillsRendererV1 = {},
                        RuntimeSkillsActiveSetV1 = {},
                        RuntimeSkillsReleaseV1 = {},
                        RuntimeSkillsSymbolTextV1 = {},
                        RuntimeSkillsTrainAvailableV1 = {});

    const std::shared_ptr<CharacterState>& shared_state() const noexcept { return state_; }
    PageV1& page() noexcept { return page_; }
    const PageV1& page() const noexcept { return page_; }
    bool source_class_art_available() const noexcept { return class_art_available_; }

    // Chains the current callback and projects this page only for Tab::skills.
    // The installed Binding must refer to the exact same CharacterState owner.
    bool install_content(character_menu::Bindings&, std::string& error);

    // Adapter for the existing same-owner SourceCompositionV1 provider slot.
    character_menu::SourcePageProviderV1 source_page_provider();
private:
    bool content_installed_ = false;
    bool class_art_available_ = false;
};

} // namespace dh::foundation::generic_skills
