#pragma once

#include "character_level_runtime.hpp"

#include <cstdint>
#include <string>

namespace dh2::character_runtime_factory_v1 { class Record; }

namespace dh2::character_faery_init_hp_mp_v1 {

enum class Status : std::uint8_t {
    complete,
    already_complete,
    invalid_record,
    invalid_storage,
    owner_mismatch,
    busy,
    prior_failure,
    provider_failed,
    allocation_failed,
};

// Retained by the canonical Faery Character Record. Runtime owns only
// lifecycle/provider state; all property sheets and debug services are
// borrowed from the existing per-Faery source owners for each call.
class Owner final {
public:
    Status initialize(character_runtime_factory_v1::Record&,
                      const character_level_runtime::Storage&,
                      character_level_runtime::Result*, std::string& error);

    bool complete() const noexcept { return complete_; }
    const character_level_runtime::Result& last_result() const noexcept {
        return last_result_;
    }

private:
    character_level_runtime::Runtime runtime_;
    character_level_runtime::Result last_result_{};
    std::uintptr_t character_{};
    std::uintptr_t properties_{};
    std::int32_t* base_{};
    data::PropertyView* view_{};
    bool bound_{};
    bool failure_latched_{};
    bool complete_{};
};

// Source Character::_InitHpMp: invoke the recovered _InitHpMp composition on
// the same canonical Record after Revive/InitPhysicalObject. The subowner is
// allocated once on that Record; it never allocates another property owner.
Status initialize(character_runtime_factory_v1::Record&,
                  const character_level_runtime::Storage&,
                  character_level_runtime::Result*, std::string& error);

} // namespace dh2::character_faery_init_hp_mp_v1
