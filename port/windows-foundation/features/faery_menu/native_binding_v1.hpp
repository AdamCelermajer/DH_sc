#pragma once

#include "faery_menu.hpp"
#include "../../../engine-ui/character_menu_queries_owner_v1.hpp"
#include "../../../level-world/canonical_character_candidate_v60.hpp"
#include "../character_menu/source_composition.hpp"

namespace dh::foundation::faery_menu {

// Root passes only owners from one completed canonical Player record. This
// adapter composes those public owners into the page callbacks; it creates no
// Save, Faery state, actor, localization cache, or query/action owner.
struct NativeOwnerInputV1 {
    std::shared_ptr<void> selected_character_owner;
    std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> record;
    dh2::ui::CharacterMenuActionsOwnerV1* actions{};
    dh2::ui::CharacterMenuQueriesOwnerV1* queries{};
    // The same graph value used to construct `queries`; its callbacks retain
    // fresh Player/Difficulty and genuine HudText/localization producers.
    dh2::ui::CharacterMenuQueriesGraphV1* query_graph{};
    dh2::ui::CharacterMenuFaeryActionsV1* faery_actions{};
    dh2::data::FaeryTables::Borrow faery_tables;
};

bool bind_native_owners_v1(const NativeOwnerInputV1&, Bindings&,
                           std::string& error);
bool bind_source_page_provider_v1(Bindings,
                                  dh::foundation::character_menu::SourcePageProviderV1&,
                                  std::string& error);

// Source ActionScript uses menu slots 0..4; only localized text maps through
// FaeryList symbols [1,4,2,3,5]. The upgraded suffix is selected from the
// same Save level field, not a UI toggle.
bool source_text_symbol_v1(const std::string& field, std::int32_t slot,
                           std::int32_t saved_level, std::string& symbol,
                           std::string& error);

} // namespace dh::foundation::faery_menu
