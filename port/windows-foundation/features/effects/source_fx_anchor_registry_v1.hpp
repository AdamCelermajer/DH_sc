#pragma once

#include "../../actor_state.hpp"
#include "../../../level-world/character_mesh_fx_owner_v1.hpp"

#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <utility>

namespace dh::foundation::effects {

enum class SourceFxAnchorKindV1 { actor, level_object };

// A generation-scoped identity for the exact object named by a source script.
// Both kinds use disjoint tagged opaque manager handles. The key separately
// retains the real CombatSession ActorId or active SourceWorldObjects identity;
// full-width ActorIds must never be repurposed as manager handles.
struct SourceFxAnchorKeyV1 {
    SourceFxAnchorKindV1 kind = SourceFxAnchorKindV1::actor;
    std::uint64_t generation{};
    ActorId actor_identity{}; // exact live CombatSession identity for actor anchors
    std::uint64_t source_object_identity{}; // exact active SourceWorldObjects/ObjectId scope
    std::uintptr_t manager_identity{};
    std::int32_t module{};
    std::string source_name;
};

struct SourceFxAnchorPoseV1 {
    std::array<float, 3> position{};
    std::array<float, 3> rotation{}; // source radians, same as ActorState transform
    std::array<float, 3> scale{1.f, 1.f, 1.f};
    std::optional<bool> alive;
    std::optional<bool> disabled;
    std::optional<bool> stationary;
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
                        ActorId actor, SourceFxAnchorProviderV1 provider,
                        SourceFxAnchorKeyV1& key, std::string& error);
    bool register_object(const std::string& source_name, std::int32_t module,
                         std::uint64_t source_object_identity,
                         SourceFxAnchorProviderV1 provider,
                         SourceFxAnchorKeyV1& key, std::string& error);
    bool resolve_actor_identity(ActorId actor, std::uintptr_t& manager_identity,
                                bool& found, std::string& error) const;
    bool resolve(const std::string& source_name, std::int32_t module,
                 SourceFxAnchorKeyV1& key, bool& found, std::string& error) const;
    bool current(const SourceFxAnchorKeyV1& key) const noexcept;

    // Receives CharacterMeshFxOwner anchor requests from the active factory.
    // Every nonzero manager identity is claimed. Unknown and expired handles
    // fail closed, preventing raw ActorIds from aliasing opaque FX identities.
    // Zero is the source's explicit unanchored call and remains unhandled.
    bool query(dh2::fx::MeshFxRequestV1& request, bool& handled,
               std::string& error) const;

    // Call before registering objects from a replacement level/session. IDs are
    // monotonic and never reused; stale manager records fail rather than alias.
    void clear_for_new_world() noexcept;
    std::uint64_t generation() const noexcept { return generation_; }

    static bool is_level_object_identity(std::uintptr_t identity) noexcept;
    static bool is_actor_identity(std::uintptr_t identity) noexcept;

private:
    struct Entry {
        SourceFxAnchorKeyV1 key;
        SourceFxAnchorProviderV1 provider;
    };
    using NameKey = std::pair<std::int32_t, std::string>;
    static constexpr unsigned kIdentityTagShift = sizeof(std::uintptr_t) * 8 - 2;
    static constexpr std::uintptr_t kIdentityKindMask = std::uintptr_t{3} << kIdentityTagShift;
    static constexpr std::uintptr_t kActorTag = std::uintptr_t{1} << kIdentityTagShift;
    static constexpr std::uintptr_t kLevelObjectTag = std::uintptr_t{2} << kIdentityTagShift;
    static constexpr std::uintptr_t kIdentityTokenMask = (std::uintptr_t{1} << kIdentityTagShift) - 1;

    std::map<NameKey, SourceFxAnchorKeyV1> names_;
    std::map<std::uintptr_t, Entry> objects_;
    std::map<ActorId, SourceFxAnchorKeyV1> actors_;
    // Retain generated handles after a world clear to prevent numeric reuse.
    std::map<std::uintptr_t, bool> managed_identities_;
    std::uintptr_t next_actor_token_ = 1;
    std::uintptr_t next_object_token_ = 1;
    std::uint64_t generation_ = 1;
    bool generation_exhausted_ = false;
};

} // namespace dh::foundation::effects
