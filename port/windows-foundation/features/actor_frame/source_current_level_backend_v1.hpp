#pragma once

#include "../../../level-loader/native_gslevel_runtime_v27.hpp"
#include "../../../level-loader/area_transition_request_v114.hpp"
#include "../../../level-world/canonical_object_manager_v1.hpp"
#include "../../../level-world/character_world_runtime_v1.hpp"

namespace dh::foundation::actor_frame {

// The root lends the already published source graph. These are identity and
// lifetime witnesses for one application/world epoch; this backend creates no
// parallel World, ObjectManager, GS global, Level, or current-Level slot.
struct SourceCurrentLevelGraphV1 {
    std::shared_ptr<void> root_scope;
    std::shared_ptr<void> application;
    std::shared_ptr<void> world;
    std::shared_ptr<dh2::world::CanonicalObjectManagerV1> objects;
    std::shared_ptr<void> navigation;
    std::shared_ptr<void> floors;
    std::shared_ptr<dh2::loader::NativeGSLevelGlobalsV27> gs_globals;
    std::shared_ptr<dh2::loader::NativeGSLevelRuntimeV27> gs_runtime;
    std::shared_ptr<dh2::character::skills::CharacterWorldRuntimeV1> actor_world;
    const dh2::data::LevelTables* level_tables{};
};

// Typed call-scoped loan. current_level() is the source GS s_level receiver;
// kill_level() returns that exact C1's word+150 storage, never phase+130.
class SourceCurrentLevelBorrowV1 {
    std::shared_ptr<void> graph_;
    dh2::loader::CanonicalCurrentLevelBorrowV1 current_;
    friend class SourceCurrentLevelBackendV1;
public:
    explicit operator bool() const noexcept { return bool(current_); }
    std::uintptr_t identity() const noexcept { return current_.identity(); }
    const dh2::character::KillLevel16* kill_level() const noexcept {
        return current_.kill_level();
    }
    const std::int32_t* difficulty118() const noexcept {
        return current_ ? &current_.level()->constructor_fields_v3().mode118 : nullptr;
    }
    const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& level() const noexcept {
        return current_.level();
    }
};

// Adapter around the already existing NativeGSLevelRuntimeV27. It runs its
// original GS/Level C1 path and exposes that runtime's existing s_level slot.
// All calls are sequential on the source runtime thread.
class SourceCurrentLevelBackendV1 {
    SourceCurrentLevelGraphV1 graph_;
    std::shared_ptr<void> graph_lease_;
    bool attempted_{};

    bool validate_graph(std::string&) const;
public:
    explicit SourceCurrentLevelBackendV1(SourceCurrentLevelGraphV1);
    SourceCurrentLevelBackendV1(const SourceCurrentLevelBackendV1&) = delete;
    SourceCurrentLevelBackendV1& operator=(const SourceCurrentLevelBackendV1&) = delete;

    bool construct(dh2::loader::LevelSourceRequestV1,
        dh2::loader::GSLevelArgumentsV2,
        dh2::loader::LevelConstructorApplicationV4,
        dh2::loader::GSLevelServicesV2<dh2::loader::CanonicalLevelContextV1>,
        std::function<bool(std::uint32_t&, std::string&)>, std::string&);
    bool current(SourceCurrentLevelBorrowV1&, std::string&) const;
    bool still_current(const SourceCurrentLevelBorrowV1&, std::string&) const;
    bool validate_actor_manager_identity(std::uintptr_t actor, std::string&) const;
    bool destroy(std::string&);
    bool construction_attempted() const noexcept { return attempted_; }

    // Adapter shape used by existing CurrentLevel/Kill providers.
    static bool current_callback(void*,
        dh2::loader::CanonicalCurrentLevelBorrowV1&, std::string&);
    const SourceCurrentLevelGraphV1& graph() const noexcept { return graph_; }
};

} // namespace dh::foundation::actor_frame
