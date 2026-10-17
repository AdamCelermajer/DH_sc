#pragma once

#include "character_ai_initialization.hpp"
#include "character_faery_placement_v1.hpp"
#include "character_runtime_factory_v1.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace dh2::character_faery_links_v1 {

namespace factory = character_runtime_factory_v1;
namespace placement = character_faery_placement_v1;
namespace ai = character_ai_initialization;

enum class Role : std::uint8_t { player, faery };
enum class Status : std::uint8_t {
    complete, invalid_argument, source_character_incomplete,
    wrong_character_type, duplicate_identity, provider_unavailable,
};

struct Result {
    std::uint32_t registrations{};
    std::uint32_t player_faery_writes{};
    std::uint32_t ai_master_writes{};
    std::uint32_t delegated_calls{};
};

// Providers read the already-live Character graph. These mirror the three
// source queries performed by CharAI::AI_SetMaster for a non-null master.
struct MasterServices {
    void* context{};
    int (*is_host_player)(void*, std::uintptr_t character, bool*){};
    int (*target_position)(void*, std::uintptr_t character,
                           placement::Vector3*){};
    int (*owner_view_distance)(void*, std::uintptr_t faery_character,
                               float*){};
};

// `source_character_type` is the already-resolved Character/AI type from the
// same source properties used by the factory. AI state is the canonical owner
// stored in the factory's AI component slot. Registering retains neither owner.
class Owner final {
public:
    Status register_character(factory::Record&, ai::State&,
                              std::int32_t source_character_type,
                              Role, std::string& error);
    // Retire the role binding before its canonical factory Record is erased.
    // Faery retirement also clears player Records that point at that exact
    // Faery identity, preventing a stale Character+0x420 association.
    Status unregister_character(std::uintptr_t identity,
                                std::string& error) noexcept;

    // Delegates the remaining Level::PlaceFaeryAndFollowers services and
    // services its Character+0x420 and canonical CharAI+0x50 writes directly.
    placement::Services services(void* context,
        int (*delegate)(void*, const placement::Request&, placement::Reply*),
        MasterServices master = {})
        noexcept;

private:
    struct Entry { factory::Record* record{}; ai::State* ai_owner{}; Role role{}; };
    static int dispatch(void*, const placement::Request&, placement::Reply*);
    Entry* find(std::uintptr_t identity) noexcept;
    const Entry* find(std::uintptr_t identity) const noexcept;

    std::vector<Entry> entries_;
    MasterServices master_{};
    void* delegate_context_{};
    int (*delegate_)(void*, const placement::Request&, placement::Reply*){};
};

} // namespace dh2::character_faery_links_v1
