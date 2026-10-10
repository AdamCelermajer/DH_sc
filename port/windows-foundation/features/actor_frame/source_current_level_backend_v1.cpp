#include "source_current_level_backend_v1.hpp"

#include <cctype>
#include <limits>
#include <stdexcept>

namespace dh::foundation::actor_frame {
namespace {
template<class A, class B>
bool same_receiver(const std::shared_ptr<A>& a, const std::shared_ptr<B>& b) {
    return a && b && a.get() == b.get() &&
        !a.owner_before(b) && !b.owner_before(a);
}
}

SourceCurrentLevelBackendV1::SourceCurrentLevelBackendV1(SourceCurrentLevelGraphV1 graph)
    : graph_(std::move(graph)),
      graph_lease_(std::make_shared<SourceCurrentLevelGraphV1>(graph_)) {
    std::string error;
    if (!validate_graph(error)) throw std::invalid_argument(error);
}

bool SourceCurrentLevelBackendV1::validate_graph(std::string& error) const {
    if (!graph_.root_scope || !graph_.application || !graph_.world ||
        !graph_.objects || !graph_.navigation || !graph_.floors ||
        !graph_.gs_globals || !graph_.gs_runtime || !graph_.actor_world ||
        graph_.world.owner_before(graph_.actor_world) ||
        graph_.actor_world.owner_before(graph_.world) ||
        !graph_.level_tables || !graph_.gs_runtime->owns_globals_v50(graph_.gs_globals)) {
        error = "Required same root/Application/World/ObjectManager/navigation/floors/GS source graph";
        return false;
    }
    error.clear();
    return true;
}

bool SourceCurrentLevelBackendV1::construct(
    dh2::loader::LevelSourceRequestV1 request,
    dh2::loader::GSLevelArgumentsV2 arguments,
    dh2::loader::LevelConstructorApplicationV4 application,
    dh2::loader::GSLevelServicesV2<dh2::loader::CanonicalLevelContextV1> gs_services,
    std::function<bool(std::uint32_t&, std::string&)> loading_online,
    std::string& error) {
    if (!validate_graph(error)) return false;
    if (attempted_) {
        error = "Source current-Level C1 attempt already reached; no replay";
        return false;
    }
    attempted_ = true;

    if (!application.owner || application.owner.get() != graph_.application.get() ||
        application.owner.owner_before(graph_.application) ||
        graph_.application.owner_before(application.owner) ||
        application.levels != graph_.level_tables ||
        !arguments.name18.size() || arguments.name18.find('\0') != std::string::npos ||
        request.identity != arguments.name18 || request.definition.empty() ||
        request.definition.find('\0') != std::string::npos) {
        error = "Require same root C1 Application LevelTables and unmodified source Level identity/MLX definition";
        return false;
    }

    // Resolve the source C1 row from the actual LevelTable row.filename using
    // the original first substring rule. The MLX definition remains the exact
    // loader-selected source definition; no Act/location alias is synthesized.
    std::int32_t expected_row{-1};
    const dh2::data::LevelRecord* expected_record{};
    std::string expected_filename;
    for (std::size_t i = 0; i < graph_.level_tables->levels.size(); ++i) {
        const auto& row = graph_.level_tables->levels[i];
        if (row.file.size() > 1023 || row.file.find('\0') != std::string::npos) {
            error = "Level filename outside original CString1024 domain";
            return false;
        }
        std::string lower = row.file;
        for (char& ch : lower)
            ch = char(std::tolower(static_cast<unsigned char>(ch)));
        if (arguments.name18.find(lower) == std::string::npos) continue;
        if (i > std::size_t(std::numeric_limits<std::int32_t>::max())) {
            error = "LevelList index outside original signed member ID";
            return false;
        }
        expected_row = std::int32_t(i);
        expected_record = &row;
        expected_filename = std::move(lower);
        break;
    }
    std::string definition_lower = request.definition;
    for (char& ch : definition_lower)
        ch = char(std::tolower(static_cast<unsigned char>(ch)));
    if (expected_row < 0 || !expected_record ||
        definition_lower.find(expected_filename) == std::string::npos) {
        error = "Source Level identity/MLX definition has no matching actual LevelTable filename row";
        return false;
    }

    if (!graph_.gs_runtime->construct(std::move(request), std::move(arguments),
            std::move(application), std::move(gs_services), std::move(loading_online), error))
        return false;

    dh2::loader::CanonicalCurrentLevelBorrowV1 current;
    if (!graph_.gs_runtime->current(current, error)) return false;
    if (!current || !same_receiver(current.level(), graph_.gs_globals->s_level) ||
        current.level()->constructor_fields_v3().row3c != expected_row ||
        !current.level()->constructor_owner_v3() ||
        current.level()->constructor_owner_v3()->phase() !=
            dh2::loader::LevelConstructorPhaseV3::complete ||
        current.kill_level() != current.level()->kill_level()) {
        error = "Published current Level is not the same completed native C1/LevelTable row";
        return false;
    }
    error.clear();
    return true;
}

bool SourceCurrentLevelBackendV1::current(SourceCurrentLevelBorrowV1& out,
                                           std::string& error) const {
    if (!validate_graph(error)) return false;
    dh2::loader::CanonicalCurrentLevelBorrowV1 source;
    if (!graph_.gs_runtime->current(source, error)) return false;
    if (source && !same_receiver(source.level(), graph_.gs_globals->s_level)) {
        error = "Current-Level getter did not borrow the same GS s_level slot";
        return false;
    }
    SourceCurrentLevelBorrowV1 next;
    next.graph_ = graph_lease_;
    next.current_ = std::move(source);
    out = std::move(next);
    error.clear();
    return true;
}

bool SourceCurrentLevelBackendV1::still_current(
    const SourceCurrentLevelBorrowV1& borrow, std::string& error) const {
    if (!borrow || !borrow.graph_ || borrow.graph_.get() != graph_lease_.get()) {
        error = "Required live SourceCurrentLevelBorrowV1 from this root graph";
        return false;
    }
    SourceCurrentLevelBorrowV1 now;
    if (!current(now, error)) return false;
    if (!now || now.identity() != borrow.identity() ||
        now.level().get() != borrow.level().get()) {
        error = "Source current-Level borrow is stale after GS slot change/unload";
        return false;
    }
    error.clear();
    return true;
}

bool SourceCurrentLevelBackendV1::validate_actor_manager_identity(
    std::uintptr_t actor, std::string& error) const {
    if (!validate_graph(error)) return false;
    if (!actor) {
        error = "Required non-NULL live World actor identity";
        return false;
    }
    dh2::target_providers::Handle16* world_handle{};
    dh2::target_providers::Registry24* world_registry{};
    if (graph_.actor_world->handle_borrow(actor, &world_handle, &world_registry) ||
        !world_handle || !world_registry || world_handle->cached != actor) {
        error = "Actor is absent from the same live CharacterWorldRuntime handle registry";
        return false;
    }
    dh2::character::skills::WorldTargetActorBorrowV1 world_actor{};
    if (graph_.actor_world->actor(actor, &world_actor) ||
        world_actor.identity != actor || !world_actor.receiver_lease) {
        error = "Actor World receiver lease/identity is not available from the same World runtime";
        return false;
    }
    const auto* object = graph_.objects->object(world_handle->key);
    if (!object || object->identity != actor || !object->lease ||
        object->shared_handle != world_handle ||
        object->lease.get() != world_actor.receiver_lease.get() ||
        object->lease.owner_before(world_actor.receiver_lease) ||
        world_actor.receiver_lease.owner_before(object->lease)) {
        error = "Actor World handle does not resolve to the same canonical ObjectManager receiver";
        return false;
    }
    error.clear();
    return true;
}

bool SourceCurrentLevelBackendV1::destroy(std::string& error) {
    if (!validate_graph(error)) return false;
    return graph_.gs_runtime->destroy(error);
}

bool SourceCurrentLevelBackendV1::current_callback(
    void* raw, dh2::loader::CanonicalCurrentLevelBorrowV1& out,
    std::string& error) {
    if (!raw) {
        error = "Required same SourceCurrentLevelBackendV1 provider";
        return false;
    }
    auto& self = *static_cast<SourceCurrentLevelBackendV1*>(raw);
    if (!self.validate_graph(error)) return false;
    if (!self.graph_.gs_runtime->current(out, error)) return false;
    if (out && !same_receiver(out.level(), self.graph_.gs_globals->s_level)) {
        error = "Current-Level callback escaped the same GS s_level slot";
        return false;
    }
    error.clear();
    return true;
}

} // namespace dh::foundation::actor_frame
