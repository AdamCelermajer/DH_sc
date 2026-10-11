#pragma once

#include <cstdint>
#include <functional>
#include <string>

namespace dh::foundation::effects {

// The Script_PlayEffect source command's boolean is an execution-skip flag.
// The adapter deliberately does not own or deduplicate instances: source
// scripts can author repeated calls on either side of a position change.
using SourceScriptFxPlaySetV1 = std::function<bool(
    std::int32_t set, const float position[3], std::uintptr_t anchor,
    std::string& error)>;

enum class SourceScriptFxResultV1 {
    skipped,
    played,
    failed,
};

// Reusable Script_PlayEffect seam. On a non-skipped call, `play_set` must be
// bound to the active session's CharacterMeshFxOwnerV4 and preserve that
// manager's level-scoped EffectsTables/resource bank.
SourceScriptFxResultV1 dispatch_source_play_effect_v1(
    bool source_skip, std::int32_t set, const float position[3],
    std::uintptr_t resolved_anchor, const SourceScriptFxPlaySetV1& play_set,
    std::string& error);

} // namespace dh::foundation::effects
