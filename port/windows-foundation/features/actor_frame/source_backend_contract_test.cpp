#include "source_campaign_backend_v1.hpp"
#include <type_traits>
using namespace dh::foundation::actor_frame;
static_assert(
    std::is_same_v<
        decltype(SourceCampaignBackendGraphV1::characters),
        std::shared_ptr<dh2::world::CanonicalCharacterCandidateFactoryV60>>);
static_assert(std::is_same_v<decltype(SourceCampaignBackendServicesV1::fx),
                             std::shared_ptr<dh2::fx::CharacterMeshFxOwnerV4>>);
static_assert(
    std::is_same_v<decltype(SourceCampaignBackendServicesV1::level),
                   std::shared_ptr<dh2::loader::CanonicalLevelContextV1>>);
static_assert(
    std::is_same_v<decltype(SourceCampaignBackendServicesV1::audio_bridge),
                   std::shared_ptr<dh2::audio::AudioCampaignBridgeV46>>);
// Compile contract: root retention and scoped registration use actual graph.
bool register_existing_backend(
    SourceCampaignBackendGraphV1 graph,
    std::shared_ptr<SourceCampaignBackendContextV1> &root_retained,
    std::string &error) {
  auto context =
      std::make_shared<SourceCampaignBackendContextV1>(std::move(graph));
  if (!SourceCampaignBackendContextV1::register_current(context, error))
    return false;
  root_retained = std::move(context);
  return true;
}
