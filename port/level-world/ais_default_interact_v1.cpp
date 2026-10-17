#include "ais_default_interact_v1.hpp"

namespace dh2::ais_default_interact_v1 {

Status on_interact(const Request& request, const Services& services,
                   Result& result, std::string& error) noexcept {
    result = {};
    error.clear();
    if (!request.ai_identity || !request.owner_character ||
        !request.target_game_object ||
        request.ai_identity == request.owner_character) {
        error = "AISDefault::OnInteract request has invalid same-Level identities";
        return Status::invalid_argument;
    }
    if (!services.is_character || !services.is_monster ||
        !services.interact || !services.is_merchant ||
        !services.clear_target || !services.sync_last_target) {
        error = "AISDefault::OnInteract requires typed target and same-CharAI providers";
        return Status::provider_unavailable;
    }

    bool character = false;
    if (!services.is_character(services.context, request.target_game_object,
                               character, error)) {
        if (error.empty()) error = "GameObject::IsCharacter provider failed";
        return Status::provider_failed;
    }
    if (character) {
        bool monster = false;
        if (!services.is_monster(services.context, request.target_game_object,
                                 monster, error)) {
            if (error.empty()) error = "Character::IsMonster provider failed";
            return Status::provider_failed;
        }
        if (monster) return Status::complete;
    }

    std::uint8_t retain = 0;
    if (!services.interact(services.context, request.target_game_object,
                           request.owner_character, retain, error)) {
        if (error.empty()) error = "target GameObject::Interact provider failed";
        return Status::provider_failed;
    }
    result.interaction_dispatched = true;
    if (retain) return Status::complete;

    // The source performs this second virtual IsCharacter test after Interact,
    // because the target call may have changed its source state.
    character = false;
    if (!services.is_character(services.context, request.target_game_object,
                               character, error)) {
        if (error.empty()) error = "post-Interact GameObject::IsCharacter provider failed";
        return Status::provider_failed;
    }
    if (character) {
        bool merchant = false;
        if (!services.is_merchant(services.context, request.target_game_object,
                                  merchant, error)) {
            if (error.empty()) error = "Character::IsMerchant provider failed";
            return Status::provider_failed;
        }
        if (merchant) return Status::complete;
    }

    if (!services.clear_target(services.context, request.ai_identity, error)) {
        if (error.empty()) error = "CharAI::AI_SetTarget(nullptr,false) failed";
        return Status::provider_failed;
    }
    result.target_cleared = true;
    if (!services.sync_last_target(services.context, request.ai_identity, error)) {
        if (error.empty()) error = "CharAI::AI_SyncLastTarget failed";
        return Status::provider_failed;
    }
    result.last_target_synchronized = true;
    error.clear();
    return Status::complete;
}

} // namespace dh2::ais_default_interact_v1
