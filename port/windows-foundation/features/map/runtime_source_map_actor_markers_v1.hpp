#pragma once

#include "runtime_source_map_page_v1.hpp"
#include "../../combat_session.hpp"

#include <vector>

namespace dh::foundation::map_page {

// Produce the recovered local-player marker (MenuCharMenu_Map family 3) from
// the current generic gameplay session. The ActorState and its transform are
// read from the session's one PlayableActorWorld; the marker retains that
// actor-binding lease through projection. This does not classify other actors
// or manufacture positions for unsupported marker families.
bool source_map_current_player_marker_v1(
    const CombatSession& session, SourceMarkerV1& output, std::string& error);

// Composition helper for RuntimeSourceMapPageV1::ServicesV1::collect_markers.
// It appends only the current Session's source family-3 player marker to
// caller-provided markers. Existing source producers may append their own
// supported families first; unsupported facts remain absent. The caller must
// invoke it synchronously with the same current CombatSession that owns the
// visible gameplay frame. This function does not claim a legacy Level,
// RoomZone, quest, or event owner for the generic Session.
bool source_map_append_current_player_marker_v1(
    const CombatSession& session, std::vector<SourceMarkerV1>& markers,
    std::string& error);

} // namespace dh::foundation::map_page
