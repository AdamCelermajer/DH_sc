#include "player_manager_remove_character_v1.hpp"

#include <limits>
#include <utility>

namespace dh2::player_manager_remove_character_v1 {
namespace {
Status fail(Result& result, Status status, Stage stage, const char* message,
            std::string& error) {
    result.status = status;
    result.reached = stage;
    error = message;
    return status;
}

template<class F, class... Args>
bool call(Services const& services, Result& result, Stage stage, F fn,
          const char* message, std::string& error, Args&&... args) {
    result.reached = stage;
    if (!fn) {
        result.status = Status::service_unavailable;
        error = message;
        return false;
    }
    ++result.callbacks;
    try {
        if (fn(services.context, std::forward<Args>(args)...) == 0) return true;
    } catch (...) {
        // Renderer providers report ordinary source failures by return value;
        // contain an unexpected exception at this native lifetime boundary.
    }
    result.status = Status::service_failed;
    if (error.empty()) error = message;
    return false;
}
}

bool services_ready(const Services& s) noexcept {
    return s.set_position && s.disable_and_hide && s.init_camera &&
           s.clear_ai_ooi_aggro && s.logical_delete_and_rename &&
           s.current_level_effects && s.player_info_tail && s.hud_refresh;
}

Status remove_character(player_manager_update_v1::State& manager,
                        player_info_record_v1::Record& player,
                        const Services& services,
                        std::uintptr_t expected_manager,
                        std::uintptr_t character_identity,
                        const std::string& original_object_name,
                        bool current_level_present, bool local_controlled,
                        std::int32_t player_class, Result* output,
                        std::string& error) {
    Result local{};
    Result& result = output ? *output : local;
    result = {};
    if (!expected_manager || !character_identity || original_object_name.empty() ||
        original_object_name.size() > 400)
        return fail(result, Status::invalid_argument, Stage::none,
                    "PlayerManager::RemoveCharacter arguments are incomplete", error);
    if (manager.manager_identity != expected_manager ||
        !manager.created_character_count_known || !manager.remove_flag_6c9_known ||
        manager.manager_remove_flag_6c9 != 0)
        return fail(result, Status::invalid_manager, Stage::none,
                    "PlayerManager::RemoveCharacter is outside the known offline manager branch", error);
    if (!manager.busy)
        return fail(result, Status::invalid_manager, Stage::none,
                    "PlayerManager::RemoveCharacter must run inside the canonical PlayerManager::Update", error);
    if (manager.local_death_count_6c4 != 1 ||
        manager.counted_player_character != character_identity)
        return fail(result, Status::unsupported_count, Stage::none,
                    "PlayerManager::RemoveCharacter only has a proven single-Player count owner", error);
    if (!player.constructed || player.busy || player.character_660 != character_identity)
        return fail(result, Status::wrong_character, Stage::none,
                    "PlayerManager::RemoveCharacter PlayerInfo +0x660 is not the same canonical Character", error);
    if (!services_ready(services))
        return fail(result, Status::service_unavailable, Stage::none,
                    "PlayerManager::RemoveCharacter source effect providers are incomplete", error);
    result.has_current_level = current_level_present;
    result.local_controlled = local_controlled;
    result.player_class = player_class;
    const std::int32_t remaining_count = manager.local_death_count_6c4 - 1;
    // PlayerManager::RemoveCharacter decrements +0x6c4 before any Character
    // side effects. Publish that reached source prefix now, so a provider
    // failure cannot leave the canonical manager claiming the Character is
    // still counted after the source would already have removed it.
    if (!player_manager_update_v1::player_character_removed(
            manager, expected_manager, character_identity))
        return fail(result, Status::wrong_character, Stage::none,
                    "PlayerManager::RemoveCharacter could not publish its leading count decrement", error);
    if (!call(services, result, Stage::set_position, services.set_position,
              "PlayerManager::RemoveCharacter GameObject::SetPosition failed",
              error, character_identity, error)) return result.status;
    if (!call(services, result, Stage::disable_and_hide, services.disable_and_hide,
              "PlayerManager::RemoveCharacter Disabled/SetVisible(false) failed",
              error, character_identity, error)) return result.status;
    if (!call(services, result, Stage::init_camera, services.init_camera,
              "PlayerManager::RemoveCharacter Character::InitCam failed",
              error, character_identity, error)) return result.status;
    if (!call(services, result, Stage::clear_ai_ooi_aggro,
              services.clear_ai_ooi_aggro,
              "PlayerManager::RemoveCharacter Character/CharAI cleanup failed",
              error, character_identity, error)) return result.status;

    if (manager.deleted_character_suffix_counter >
        static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max()))
        return fail(result, Status::invalid_argument,
                    Stage::logical_delete_and_rename,
                    "PlayerManager deleted-character suffix exceeds the source signed range", error);
    const auto suffix = manager.deleted_character_suffix_counter++;
    const auto deleted_name = original_object_name + "_Deleted_" + std::to_string(suffix);
    if (!call(services, result, Stage::logical_delete_and_rename,
              services.logical_delete_and_rename,
              "PlayerManager::RemoveCharacter ObjectBase::Delete/SetName failed",
              error, character_identity, original_object_name, deleted_name, error))
        return result.status;

    // CurrentLevel::UpdateLightSet(bool,int) at 0x3ef280 is an empty source
    // stub. The callback still verifies the exact active Level and is called
    // only under the original class/local-control condition. With the single
    // Player count, remaining_count is zero and the replacement-camera branch
    // is not reached.
    if (current_level_present && player_class != -1 && local_controlled) {
        if (!call(services, result, Stage::current_level_effects,
                  services.current_level_effects,
                  "PlayerManager::RemoveCharacter CurrentLevel branch failed",
                  error, remaining_count, local_controlled, player_class, error))
            return result.status;
    }

    if (!call(services, result, Stage::player_info_tail,
              services.player_info_tail,
              "PlayerManager::RemoveCharacter PlayerInfo tail failed",
              error, player, character_identity, manager.manager_remove_flag_6c9,
              error))
        return result.status;
    if (!call(services, result, Stage::hud_refresh, services.hud_refresh,
              "PlayerManager::RemoveCharacter HUD completeRefresh failed",
              error, error)) return result.status;

    result.status = Status::complete;
    result.reached = Stage::complete;
    error.clear();
    return result.status;
}

} // namespace dh2::player_manager_remove_character_v1
