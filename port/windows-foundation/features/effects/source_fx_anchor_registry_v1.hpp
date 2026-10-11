#pragma once

#include "../../actor_state.hpp"
#include "../../../level-world/character_mesh_fx_owner_v1.hpp"

#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <utility>

namespace dh::foundation::effects {

enum class SourceFxAnchorKindV1 { actor, level_object };

// A generation-scoped identity for the exact object named by a source script.
// Actor keys carry the real CombatSession ActorId. Level-object identities are
// tagged opaque registry handles and never alias ActorIds.
struct SourceFxAnchorKeyV1 {
    SourceFxAnchorKindV1 kind = SourceFxAnchorKindV1::actor;
    std::uint64_t generation{};
    std::uint64_t source_object_identity{}; // exact active SourceWorldObjects/ObjectId scope
    std::uintptr_t manager_identity{};
    std::int32_t module{};
    std::string source_name;
};

struct SourceFxAnchorPoseV1 {
    std::array<float, 3> position{};
    std::array<float, 3> rotation{}; // source radians, same as ActorState transform
    std::array<float, 3> scale{1.f, 1.f, 1.f};
    bool alive = true;
    bool disabled = false;
    bool stationary = false;
};

using SourceFxAnchorProviderV1 =
    std::function<bool(SourceFxAnchorPoseV1&, std::string& error)>;

// Scoped lookup for source GameObjects which are not CombatSession actors,
// such as MGP Waypoint Dummies. Providers must query the current source object
// transform (or an immutable authored transform for a guaranteed-static object)
// and must remain valid only for the active level generation.
class SourceFxAnchorRegistryV1 final {
public:
    bool register_actor(const std::string& source_name, std::int32_t module,
                        ActorId actor, std::string& error);
    bool register_object(const std::string& source_name, std::int32_t module,
                         std::uint64_t source_object_identity,
                         SourceFxAnchorProviderV1 provider,
                         SourceFxAnchorKeyV1& key, std::string& error);
    bool resolve(const std::string& source_name, std::int32_t module,
                 SourceFxAnchorKeyV1& key, bool& found, std::string& error) const;
    bool current(const SourceFxAnchorKeyV1& key) const noexcept;

    // Receives CharacterMeshFxOwner anchor requests from the active factory.
    // `handled=false` means identity belongs to the built-in ActorId provider.
    // Tagged but expired level-object IDs are handled failures, never ActorIds.
    bool query(dh2::fx::MeshFxRequestV1& request, bool& handled,
               std::string& error) const;

    // Call before registering objects from a replacement level/session. IDs are
    // monotonic and never reused; stale manager records fail rather than alias.
    void clear_for_new_world() noexcept;
    std::uint64_t generation() const noexcept { return generation_; }

    static bool is_level_object_identity(std::uintptr_t identity) noexcept;

private:
    struct Entry {
        SourceFxAnchorKeyV1 key;
        SourceFxAnchorProviderV1 provider;
    };
    using NameKey = std::pair<std::int32_t, std::string>;
    static constexpr std::uintptr_t kLevelObjectTag =
        std::uintptr_t{1} << (sizeof(std::uintptr_t) * 8 - 1);

    std::map<NameKey, SourceFxAnchorKeyV1> names_;
    std::map<std::uintptr_t, Entry> objects_;
    std::uintptr_t next_object_token_ = 1;
    std::uint64_t generation_ = 1;
    bool generation_exhausted_ = false;
};

} // namespace dh::foundation::effects
