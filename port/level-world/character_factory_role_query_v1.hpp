#pragma once

#include "character_ai_classification.hpp"
#include "character_runtime_factory_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::character_factory_role_query_v1 {

namespace factory = character_runtime_factory_v1;
namespace classification = character_ai_classification;

enum class Role : std::uint8_t { player, merchant };
enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    service_unavailable,
    identity_mismatch,
    source_query_failed,
};

// Captures borrowed cached Character words/name and the canonical AI table
// services for this exact Factory record. State/table backing need only survive
// the synchronous classification call; this adapter retains no second copy.
struct Services {
    void* context{};
    int (*capture)(void*, const factory::Record&,
                   classification::State*, classification::Services*,
                   std::string&){};
};

struct Result {
    std::uintptr_t character_identity{};
    bool value{};
    std::uint32_t source_calls{};
};

// Exact Character::IsPlayer / IsMerchant query over the existing bounded
// source classification bodies (ELF 0x3a49f0 / 0x3a30c4). The capture service
// must borrow facts from this Record's actual Character/AI/property owners.
Status query(const factory::Record&, Role, const Services&, Result*,
             std::string& error);

} // namespace dh2::character_factory_role_query_v1
