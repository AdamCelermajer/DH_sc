#include "source_campaign_backend_v1.hpp"

#include <type_traits>

using namespace dh::foundation::actor_frame;

static_assert(std::is_same_v<decltype(SourceCampaignBackendGraphV1::mode),
    SourceCampaignBackendModeV1>);
static_assert(SourceCampaignBackendModeV1::menu != SourceCampaignBackendModeV1::campaign);
static_assert(std::is_same_v<decltype(std::declval<const SourceCurrentLevelBackendV1&>().construction_attempted()), bool>);

int main() { return 0; }
