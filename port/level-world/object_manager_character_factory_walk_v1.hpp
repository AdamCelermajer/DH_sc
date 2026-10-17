#pragma once

#include "character_runtime_factory_v1.hpp"

#include <cstddef>
#include <string>

namespace dh2::object_manager_character_factory_walk_v1 {

namespace factory = character_runtime_factory_v1;
namespace manager = object_manager_runtime_owner_v1;

enum class Status : unsigned char {
    complete,
    invalid_argument,
    manager_failed,
    identity_mismatch,
    callback_failed,
};

struct Services {
    void* context{};
    int (*visit)(void*, const factory::Record&, std::string&){};
};

struct Result {
    std::size_t manager_rows{};
    std::size_t characters{};
};

// Walks the canonical ObjectManager map in signed source-handle order and
// resolves Character rows through the existing Factory owner. Non-Character
// GameObjects are skipped. No parallel Character registry is created. The
// two-phase handle snapshot permits a visitor to mutate ObjectManager; every
// retained handle and identity is revalidated immediately before delivery.
// This supplies the real Character sequence for language refresh, but does
// not supply IsPlayer/IsMerchant or inventory-refresh implementations.
Status walk(manager::Owner&, const factory::Owner&, const Services&,
            Result*, std::string& error);

} // namespace dh2::object_manager_character_factory_walk_v1
