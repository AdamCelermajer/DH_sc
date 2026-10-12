#include "actor_clip_manifest.hpp"

namespace dh::foundation::campaign_host {

std::vector<ActorClipRequest> collect_actor_clip_requests(const OriginalCampaignRuntime& runtime) {
    std::vector<ActorClipRequest> requests;
    constexpr std::uint32_t none = 0xFFFFFFFFu; // source "no chained clip" value of scalar 12
    for (const auto& script : runtime.scripts()) {
        for (const auto& command : script.commands) {
            if (command.kind != 45) continue;
            const auto actor = command.strings.find(24);
            if (actor == command.strings.end()) continue;
            for (const unsigned offset : {8u, 12u}) {
                const auto value = command.scalars.find(offset);
                if (value == command.scalars.end() || value->second == none) continue;
                requests.push_back({actor->second, static_cast<std::int32_t>(value->second)});
            }
        }
    }
    return requests;
}

std::vector<SceneObjectClipRequest> collect_scene_object_clip_requests(const OriginalCampaignRuntime& runtime) {
    std::vector<SceneObjectClipRequest> requests;
    for (const auto& script : runtime.scripts()) {
        for (const auto& command : script.commands) {
            if (command.kind != 19) continue;
            const auto object = command.strings.find(24);
            const auto clip = command.strings.find(12);
            if (object == command.strings.end() || clip == command.strings.end()) continue;
            requests.push_back({object->second, clip->second});
        }
    }
    return requests;
}

std::string actor_clip_name(std::int32_t dictionary_id) {
    return "cs:" + std::to_string(dictionary_id);
}

} // namespace dh::foundation::campaign_host

namespace dh::foundation::campaign_host {

std::set<std::string> collect_script_gameplay_actors(const OriginalCampaignRuntime& runtime) {
    // B154: kind 30 SpawnCharacter, 31 PutCharacterInIdle and 44 KillActor name the actor at string 12,
    // kind 29 PutCharacterInLimbus at string 16. A by-name Idle or Kill is a gameplay lifecycle step (see header).
    std::set<std::string> spawned, retired, gameplay_named;
    for (const auto& script : runtime.scripts()) {
        for (const auto& command : script.commands) {
            if (command.kind != 30 && command.kind != 29 && command.kind != 31 && command.kind != 44) continue;
            const auto name = command.strings.find(command.kind == 29 ? 16u : 12u);
            if (name == command.strings.end() || name->second.empty()) continue;
            if (command.kind == 30) spawned.insert(name->second);
            else if (command.kind == 29) retired.insert(name->second);
            else gameplay_named.insert(name->second);
        }
    }
    for (const auto& name : retired)
        if (!gameplay_named.count(name)) spawned.erase(name);
    return spawned;
}

std::set<std::string> collect_script_idled_actors(const OriginalCampaignRuntime& runtime) {
    // B155: kind 31 PutCharacterInIdle and kind 44 KillActor name the actor at string 12.
    std::set<std::string> named;
    for (const auto& script : runtime.scripts())
        for (const auto& command : script.commands) {
            if (command.kind != 31 && command.kind != 44) continue;
            const auto name = command.strings.find(12u);
            if (name != command.strings.end() && !name->second.empty() && name->second != "All" &&
                name->second != "LocalPlayer" && name->second != "Player")
                named.insert(name->second);
        }
    return named;
}

} // namespace dh::foundation::campaign_host
