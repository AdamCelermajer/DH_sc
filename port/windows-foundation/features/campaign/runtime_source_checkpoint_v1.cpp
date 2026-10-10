#include "runtime_source_checkpoint_v1.hpp"

namespace dh::foundation::campaign {
namespace {
bool validate_route(const SourceTransitionV1& route, const GameSave& save,
                    std::string& error) {
    if (route.source_module.empty() || route.transition_object.empty() ||
        route.destination_member.empty() || route.destination_level_uri.empty() ||
        route.entrypoint_id < 0) {
        error = "Require a complete admitted authored transition identity";
        return false;
    }
    if (save.level_uri != route.destination_level_uri) {
        error = "Canonical checkpoint is not at the admitted transition destination";
        return false;
    }
    return validate_game_save(save, error);
}

RuntimeSourceCheckpointReceiptV1 receipt_for(const SourceTransitionV1& route,
                                              const GameSave& save,
                                              bool persisted) {
    return {route.source_module, route.transition_object,
            route.destination_member, save.level_uri, save.character.id,
            route.entrypoint_id, persisted, false};
}
} // namespace

bool save_runtime_source_checkpoint_v1(
    const std::filesystem::path& path, const SourceTransitionV1& route,
    const GameSave& save, RuntimeSourceCheckpointReceiptV1& output,
    std::string& error) {
    if (!validate_route(route, save, error)) return false;
    if (!save_game(path, save, error)) return false;
    output = receipt_for(route, save, true);
    error.clear();
    return true;
}

bool load_runtime_source_checkpoint_v1(
    const std::filesystem::path& path, const SourceTransitionV1& route,
    GameSave& output, RuntimeSourceCheckpointReceiptV1& receipt,
    std::string& error) {
    GameSave candidate;
    if (!load_game(path, candidate, error)) return false;
    if (!validate_route(route, candidate, error)) return false;
    auto next_receipt = receipt_for(route, candidate, true);
    output = std::move(candidate);
    receipt = std::move(next_receipt);
    error.clear();
    return true;
}

} // namespace dh::foundation::campaign
