#include "source_current_player_provider_v1.hpp"
#include "../../../level-world/application_player_manager_bootstrap_v59.hpp"
#include "../../../level-world/player_manager_owner_v1.hpp"

namespace dh::foundation::inventory {
namespace {
template<class A, class B>
bool same_owner(const std::shared_ptr<A>& a, const std::shared_ptr<B>& b) {
    return a && b && a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a);
}
}

SourceFixedLootCurrentLevelProviderV1::SourceFixedLootCurrentLevelProviderV1(
    std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> application,
    std::shared_ptr<dh::foundation::actor_frame::SourceCurrentLevelBackendV1> level_backend,
    std::string& error)
    : application_(std::move(application)), level_backend_(std::move(level_backend)) {
    if (!validate(error)) return;
    player_manager_ = application_->source_player_manager_v59();
    error.clear();
}

bool SourceFixedLootCurrentLevelProviderV1::validate(std::string& error) const {
    if (!application_ || !level_backend_) {
        error = "Required retained Application and SAME source GS current-Level backend";
        return false;
    }
    const auto& graph = level_backend_->graph();
    if (!same_owner(application_, graph.application) || !graph.gs_runtime || !graph.gs_globals ||
        !graph.gs_runtime->owns_globals_v50(graph.gs_globals)) {
        error = "Current-Level backend must use this exact Application and its published GS global/runtime";
        return false;
    }
    const auto& manager = application_->source_player_manager_v59();
    if (!manager || !manager->belongs_to_application(application_) || !manager->manager() ||
        !manager->manager()->source_initialized_v59() || !manager->count_field()) {
        error = "Required initialized SAME Application-owned PlayerManager C1 count6c4";
        return false;
    }
    if (player_manager_ && !same_owner(player_manager_, manager)) {
        error = "Application PlayerManager owner changed after fixed-loot binding";
        return false;
    }
    error.clear();
    return true;
}

bool SourceFixedLootCurrentLevelProviderV1::ready() const {
    std::string ignored;
    return validate(ignored);
}

bool SourceFixedLootCurrentLevelProviderV1::query_count(
    OwnedInventoryResponseV4& out, std::string& error) const {
    if (!validate(error)) return false;
    const auto* count = player_manager_->count_field();
    if (!count) {
        error = "Required SAME PlayerManager +0x6c4 character count field";
        return false;
    }
    // AddLoot's 0x4043a8 reads Application+0x40 PlayerManager+0x6c4.
    // This is the existing source C1 word, including its real empty value.
    out = {};
    out.value = *count;
    if (out.value < 0) {
        error = "Source PlayerManager character count is negative";
        return false;
    }
    error.clear();
    return true;
}

bool SourceFixedLootCurrentLevelProviderV1::query_level(
    std::uint32_t source_caller, OwnedInventoryResponseV4& out, std::string& error) {
    if (!validate(error)) return false;
    dh::foundation::actor_frame::SourceCurrentLevelBorrowV1 current;
    if (!level_backend_->current(current, error)) return false;

    if (source_caller == 0x4043cc) {
        pending_level_ = std::move(current);
        pending_level_query_ = true;
        out = {};
        out.identity = pending_level_.identity(); // zero only for actual empty GS s_level
        error.clear();
        return true;
    }
    if (source_caller != 0x4043dc || !pending_level_query_) {
        error = "AddLoot difficulty query must follow its actual GetCurrentLevel 0x4043cc";
        return false;
    }
    if (!pending_level_) {
        error = "Source AddLoot queried Level+0x118 after GetCurrentLevel returned the real empty slot";
        return false;
    }
    if (!pending_level_.difficulty118() ||
        pending_level_.identity() != current.identity() ||
        pending_level_.level().get() != current.level().get()) {
        if (error.empty()) error = "AddLoot second GetCurrentLevel did not resolve the same live Level C1";
        return false;
    }
    out = {};
    out.identity = current.identity();
    out.value = *current.difficulty118();
    pending_level_ = {};
    pending_level_query_ = false;
    error.clear();
    return true;
}

bool SourceFixedLootCurrentLevelProviderV1::query(
    const OwnedInventoryRequestV4& request, OwnedInventoryResponseV4& out,
    std::string& error) {
    if (busy_) {
        error = "Unsupported reentrant FreshInventoryOwnedV4 AddLoot source query";
        return false;
    }
    busy_ = true;
    struct Reset { bool& busy; ~Reset() { busy = false; } } reset{busy_};
    using Operation = OwnedInventoryOperationV4;
    if (request.operation == Operation::player_count) {
        if (request.source_caller != 0x4043a8) {
            error = "PlayerManager count provider received a non-AddLoot source site";
            return false;
        }
        return query_count(out, error);
    }
    if (request.operation == Operation::current_player) {
        if (request.argument == 0 && request.source_caller == 0x4043cc)
            return query_level(request.source_caller, out, error);
        if (request.argument == 1 && request.source_caller == 0x4043dc)
            return query_level(request.source_caller, out, error);
        error = "FreshInventoryOwnedV4 current_player label requires exact AddLoot Level query site/argument";
        return false;
    }
    error = "Source fixed-loot provider received an unrelated inventory operation";
    return false;
}

bool SourceFixedLootCurrentLevelProviderV1::query_callback(
    void* raw, FreshInventoryOwnedV4&, const OwnedInventoryRequestV4& request,
    OwnedInventoryResponseV4& out, std::string& error) {
    if (!raw) {
        error = "Required retained SourceFixedLootCurrentLevelProviderV1";
        return false;
    }
    return static_cast<SourceFixedLootCurrentLevelProviderV1*>(raw)->query(request, out, error);
}

} // namespace dh::foundation::inventory
