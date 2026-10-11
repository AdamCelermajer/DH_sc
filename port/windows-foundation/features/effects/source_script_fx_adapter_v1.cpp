#include "source_script_fx_adapter_v1.hpp"

namespace dh::foundation::effects {

SourceScriptFxResultV1 dispatch_source_play_effect_v1(
    bool source_skip, std::int32_t set, const float position[3],
    std::uintptr_t resolved_anchor, const SourceScriptFxPlaySetV1& play_set,
    std::string& error) {
    error.clear();
    if (source_skip) return SourceScriptFxResultV1::skipped;
    if (!position) {
        error = "Script_PlayEffect requires the decoded source position";
        return SourceScriptFxResultV1::failed;
    }
    if (!play_set) {
        error = "Script_PlayEffect requires the active-session FX play adapter";
        return SourceScriptFxResultV1::failed;
    }
    if (!play_set(set, position, resolved_anchor, error)) {
        if (error.empty()) error = "source FX set play failed";
        return SourceScriptFxResultV1::failed;
    }
    return SourceScriptFxResultV1::played;
}

} // namespace dh::foundation::effects
