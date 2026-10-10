#pragma once
#include "source_quest_service_binding.hpp"
#include "../../../level-world/canonical_character_candidate_v60.hpp"
namespace dh::foundation {
using CanonicalQuestCharacterResolver=std::function<bool(std::uintptr_t,
    std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>&,std::string&)>;
// Fills only Character leaves from actual canonical records; App/PM/local0,
// Level194/event dispatch and rewards remain their genuine source providers.
bool bind_canonical_quest_character_services(SourceQuestServices&,
    CanonicalQuestCharacterResolver,std::string&);
// Source order: PM InitializePlayerSavegame already produced SAME Save14e8,
// SaveLoad and sync owner; profile C1/read masks create collections through
// CharacterMenuProfileLoad. Never call constructor helper before Load2 again.
bool prepare_canonical_quest_profile(
    const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>&,
    const std::shared_ptr<const dh2::data::QuestTablesPersistenceV51>&,
    dh2::character::CharacterProfileBootstrapInputsV59,
    std::shared_ptr<dh2::character::CharacterMenuQuestsV51>&,std::string&);
struct CanonicalQuestNativeGraph {
    std::shared_ptr<void> world_owner,level_owner;
    std::shared_ptr<dh2::world::NativeConditionRuntimeV69> conditions;
    std::shared_ptr<dh2::loader::GameEventRuntimeV75> objectives;
    std::shared_ptr<dh2::loader::GameEventManagerV50> game_events194;
    const std::uintptr_t* source_game_events194{};
    dh2::events::EventManagerOwnerV12* level_events{};
    std::uintptr_t level_identity{};
    // Existing native marker transport, not new marker/effect state.
    dh2::world::NativeQuestRuntimeServicesV76 markers;
};
// Called after original profile masks/InitPost initialized collections and
// Level's original GameEventManager load finished. Borrows arenas, no copies.
bool publish_canonical_quest_runtime(
    const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>&,
    const std::shared_ptr<SourceQuestServiceBinding>&,
    const CanonicalQuestNativeGraph&,std::string&);
}
