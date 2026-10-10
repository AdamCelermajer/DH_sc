#pragma once

#include "../../combat_session.hpp"
#include "../../actor_definitions.hpp"
#include "../../world.hpp"
#include "../../../level-world/game_object_spawn_probability_v1.hpp"
#include "session_container_modern_openable_v1.hpp"
#include <optional>

namespace dh::foundation::interactions {

using SessionSourceSpawnRandomConsumerV1 = std::function<bool(
    dh2::data::LootRandom8V2& channel0,
    dh2::data::LootRandom8V2* channel1,
    std::string& error)>;
using SessionSourceSpawnRandomLoanV1 = std::function<bool(
    const SessionSourceSpawnRandomConsumerV1&,std::string& error)>;

// A not-yet-published source GameObject candidate. These are the source leaves
// that CheckSpawnProbability actually reads; they are not inferred from the
// modern WorldObject's mere presence or from its gametype.
struct SessionSourceObjectAdmissionRequestV1 {
    const ActorDefinition* definition{};
    WorldObject candidate;
    std::int32_t data_id_ec{-1}; // Canonical fresh GameObject constructor default.
    std::int32_t network_id108{-1}; // Canonical fresh GameObject constructor default.
    std::int32_t probability274{100}; // Canonical fresh GameObject constructor default.
    std::optional<std::int32_t> online_owner_fc;
    std::function<bool(bool&,std::string&)> online_byte5;
    // Synchronous loan from the one canonical source-global owner. The owner
    // must preserve both channel states and prefix writes; this feature never
    // constructs/resets a stream. It can be backed by the application owner
    // or the proven equivalent current gameplay owner. The consumer must not
    // destroy/replace the Session or replace its World while the World RNG
    // loan is active. A Session rebind after loan release is detected and
    // prevents publication.
    std::shared_ptr<const void> random_owner;
    SessionSourceSpawnRandomLoanV1 with_spawn_random;
};

struct SessionSourceObjectAdmissionReceiptV1 {
    bool admitted{};
    std::int32_t roll{};
    std::int32_t cached_roll270{};
    std::int32_t probability{};
    std::int32_t data_id_ec{-1};
    std::int32_t network_id108{-1};
    bool player_handle{};
    bool online{};
    bool online_provider_evaluated{};
    bool candidate_hidden{};
    bool candidate_deleted{};
    bool marked_for_deletion{};
    SessionContainerPriorAdmissionV1 prior_admission;
};

// Runs only the source-owned MeetCondition and CheckSpawnProbability gates on
// an unpublished candidate. Success binds that exact candidate into the
// current Session WorldObject set and mints the receipt accepted by the
// admitted Openable and Destructible binders. A probability reject
// projects the source hide/Delete/byte82/Mark prefix onto the staging record
// and discards it before any retained visual or interaction callback is bound.
bool admit_session_source_object_v1(
    CombatSession&, SessionSourceObjectAdmissionRequestV1,
    SessionSourceObjectAdmissionReceiptV1&, std::string& error);

} // namespace dh::foundation::interactions
