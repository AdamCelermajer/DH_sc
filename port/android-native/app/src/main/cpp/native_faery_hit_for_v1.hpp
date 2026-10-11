#pragma once

#include "../../../../../game-data/health.hpp"
#include "../../../../../level-world/character_ai_classification.hpp"
#include "../../../../../level-world/character_kill_npc_provider_v1.hpp"
#include "../../../../../level-world/character_runtime_factory_v1.hpp"
#include "../../../../../level-world/object_update_culling.hpp"
#include "../../../../../level-world/character_apply_result.hpp"

#include <cstdint>
#include <string>

namespace dh2::native::faery_hit_for_v1 {

namespace factory = character_runtime_factory_v1;

struct Facts {
    // Source-owned facts used by Character::HitFor. `health_dead` and
    // `health_remote` are derived from the bound Record by this provider and
    // ignored if supplied by the app adapter.
    std::uint32_t flags = 0;
    std::int32_t session_state = 0;
};

struct Services {
    void* context = nullptr;
    // Capture GetOnline, Application/current Level, session state, the
    // one-shot option/debug gates, Character::IsPlayer/IsMonster, and the
    // main/child Character facts at their source call phase. This callback
    // supplies reads only; it must not mutate the Record or PropertyView.
    bool (*capture_facts)(void*, factory::Record&, std::uintptr_t attacker,
                          Facts&, std::string& error) = nullptr;
    // Character::HitFor's remaining source-ordered tutorial, low-health audio,
    // damage trophy, and one-hit boss trophy callees. The app adapter performs
    // only branches reached for this Faery and uses the same Record.
    bool (*after_health)(void*, factory::Record&, std::uintptr_t attacker,
                         std::uint32_t raw_amount,
                         const data::HealthChange&, std::string& error) = nullptr;
    // Real reached source Character Kill/Ctrl_Kill providers. Their contexts
    // and identities are borrowed from the existing Faery/Level owners.
    character_kill_npc_provider_v1::Services kill{};
    character_ai_classification::State* classification_state = nullptr;
    const character_ai_classification::Services* classification_services = nullptr;
};

struct Bindings {
    factory::Record* record = nullptr;
    data::PropertyView* properties = nullptr;
    const Services* services = nullptr;
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    stale_record,
    reentrant,
    facts_failed,
    health_failed,
    kill_failed,
    postcondition_failed,
    hit_for_tail_failed,
};

struct Snapshot {
    data::HealthChange health{};
    character_kill_npc_provider_v1::Snapshot kill{};
    std::uint32_t facts_captured = 0;
    std::uint32_t remote_query_completed = 0;
    std::uint32_t kill_attempted = 0;
    std::uint32_t kill_completed = 0;
    std::uint32_t lifecycle_write_attempted = 0;
    std::uint32_t lifecycle_write_completed = 0;
    std::uint32_t tail_attempted = 0;
    std::uint32_t tail_completed = 0;
};

// App-side typed adapter for the genuine HitFor health kernel and one
// same-Record Ctrl_Kill -> Kill -> RaiseEvent(2) lifecycle episode. It owns no
// Character, PropertyView, AI, VM, or Coordinator. Runtime is retained with
// the existing per-Faery owner for exactly one death episode; a revive needs
// a new episode bound to the same factory Record.
class Runtime final {
public:
    explicit Runtime(Bindings);
    Runtime(const Runtime&) = delete;
    Runtime& operator=(const Runtime&) = delete;

    int hit_for(void* context, const character_apply_result::Request*,
                character_apply_result::Reply*, std::string& error);
    const Snapshot& snapshot() const noexcept { return snapshot_; }

    // Wrappers for character_kill_npc_provider_v1::Services. Public only so
    // the small binding builder can preserve that provider's callback ABI.
    static bool kill_preflight(void*, std::uintptr_t character,
                               std::uintptr_t killer, std::uint32_t force,
                               std::string& error);
    static bool kill_player_branch(void*, std::uintptr_t character,
                                   std::uintptr_t killer, std::uint32_t force,
                                   std::string& error);
    static bool kill_event2(void*, std::uintptr_t character,
                            std::uintptr_t killer, std::string& error);

private:
    Bindings bindings_{};
    character_kill_npc_provider_v1::Runtime kill_;
    Snapshot snapshot_{};
    bool busy_ = false;
};

// Stable callback signature for the ApplyResult operation dispatcher. The
// context must be this retained Runtime; Request subject is the same bound
// Character, peer is the original attacker, and word0 is captured raw damage.
int hit_for(void*, const character_apply_result::Request*,
            character_apply_result::Reply*, std::string& error);

} // namespace dh2::native::faery_hit_for_v1
