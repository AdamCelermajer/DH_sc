#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>

namespace dh::foundation {
using OriginalTargetPosition = std::array<float,3>;
using OriginalAbsoluteNodePosition = std::function<bool(std::uintptr_t,OriginalTargetPosition&,std::string&)>;
using OriginalNamedNodeLookup = std::function<bool(std::uintptr_t,const char*,std::uintptr_t&,std::string&)>;
struct OriginalTargetPositionUpdateResult { bool node_queried=false,cache_written=false; };
// Explicit GameObject constructor prefix, not a fallback at frame time.
void original_target_position_ctor_prefix(std::optional<std::uintptr_t>& own_node180,
                                         std::optional<OriginalTargetPosition>& own_cache184);
// Reached InitFinal named-node assignment only. Caller owns preceding Sync and
// actual visual/root lookup. A known null scene root skips this source branch.
bool original_target_position_named_node_prefix(std::optional<std::uintptr_t>& own_node180,
    std::uintptr_t actual_scene_root,const OriginalNamedNodeLookup&,std::string& error);
// UpdateTargetPosition0x393d74: own node, NOT combat target_id. Enabled80 is
// deliberately absent: even a disabled owner updates its non-null node cache.
// Unknown node fails; known NULL retains cache. Borrowed service supplies the
// node's actual absolute position after UpdateSubObjects. No finite clamp.
bool original_update_target_position(const std::optional<std::uintptr_t>& own_node180,
    std::optional<OriginalTargetPosition>& own_cache184,const OriginalAbsoluteNodePosition&,
    OriginalTargetPositionUpdateResult&,std::string& error);
// GetTargetPosition0x3935dc exact pointer selection. Enabled getter is called
// only for non-null own node. Failed selection retains caller output pointer.
bool original_get_target_position(const std::optional<std::uintptr_t>& own_node180,
    const std::optional<OriginalTargetPosition>& own_cache184,const OriginalTargetPosition& same_position160,
    const std::function<bool(std::uint8_t&,std::string&)>& actual_enabled80,
    const OriginalTargetPosition*& selected,std::string& error);
}
