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

std::string actor_clip_name(std::int32_t dictionary_id) {
    return "cs:" + std::to_string(dictionary_id);
}

} // namespace dh::foundation::campaign_host
