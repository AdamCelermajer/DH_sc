#include "source_level_lifecycle_gate_v1.hpp"

namespace dh2::source_level_lifecycle_gate_v1 {

Descriptor inspect(const Snapshot& source) noexcept {
    Descriptor out{};
    out.gslevel_owner = source.gslevel_owner;
    out.level = source.application_current_level;
    out.level_savegame = source.level_savegame_ec;
    out.level_state = source.level_state_130;
    out.transition_flag = source.transition_flag_144;
    if (!source.gslevel_owner) {
        out.status = Status::missing_owner;
        return out;
    }
    if (!source.application_current_level) {
        out.status = Status::missing_level;
        return out;
    }
    if (!source.gslevel_level_34) {
        out.status = Status::unpublished_level;
        return out;
    }
    if (source.application_current_level != source.gslevel_level_34) {
        out.status = Status::owner_mismatch;
        return out;
    }
    if (source.teardown_started) {
        out.status = Status::tearing_down;
        return out;
    }
    if (source.gslevel_phase != GsLevelPhase::active) {
        out.status = Status::loading;
        return out;
    }
    if (source.level_state_130 != 38) {
        out.status = Status::not_loaded;
        return out;
    }
    if (source.level_savegame_ec &&
        source.savegame_parent_level_08 != source.application_current_level) {
        out.status = Status::savegame_parent_mismatch;
        return out;
    }
    out.status = Status::ready;
    out.quick_save_available = source.level_savegame_ec ? 1u : 0u;
    return out;
}

} // namespace dh2::source_level_lifecycle_gate_v1
