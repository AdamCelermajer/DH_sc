#pragma once

#include "../../../level-world/canonical_character_candidate_v60.hpp"

namespace dh::foundation::features {

using BindCanonicalNpcEventsV1 = bool (*)(
    dh2::world::CanonicalCharacterCandidateRecordV60&,
    dh2::character::CharacterScriptSessionInput&, std::string&);

// Actual host/level/object providers owned by the same source World. The
// context lease may retain those providers and their Application, but must not
// retain the CanonicalCharacterCandidateRecord (which owns this lease).
struct SourceCharacterNpcContextProvidersV1 {
    std::shared_ptr<void> world_lease;
    std::shared_ptr<void> context_lease;
    const dh2::character::HostContextBindings16* host{};
    const dh2::character::LevelServices16* level{};
    const dh2_script_object_services* objects{};
    BindCanonicalNpcEventsV1 bind_events{};
};

// Strong same-record lease for the interval from NPC input assembly through
// actual Session construction and later timer/update calls. `timer_expiry` is
// the CampaignFsm-owned event relay installed by bind_events; the Session
// creates its own CharTimers storage and wraps this relay for timer growth.
struct SourceCharacterNpcContextBorrowV1 {
    std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> record_lease;
    std::shared_ptr<void> world_lease, context_lease;
    std::uintptr_t identity{};
    dh2::character::NativeFsm24* machine{};
    dh2::character::TargetBindings48* target{};
    dh2::character::CharacterWorldNpcControllerV1* controller{};
    const dh2::character::TimerServices32* timer_expiry{};
};

// Called from CanonicalCharacterCandidateServicesV60::npc_script after the
// cache provider supplies bytecode/include-file spans and before
// RetainedCharacterActorV1::construct_script creates the SAME script session.
// This function never creates or replaces a World, controller, FSM, target,
// Session, or timer store.
bool bind_source_character_owner_factory_npc_context_v1(
    const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>&,
    dh2::character::CharacterScriptSessionInput&,
    const SourceCharacterNpcContextProvidersV1&,
    SourceCharacterNpcContextBorrowV1&, std::string&);

// Call only after construct_script has created actor->session. Confirms the
// newly-created Session and timer store still alias the retained record.
bool validate_source_character_owner_factory_npc_session_v1(
    const SourceCharacterNpcContextBorrowV1&, std::string&);

} // namespace dh::foundation::features
