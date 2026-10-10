#include "../../../android-native/app/src/main/cpp/source_campaign_character_fsm_v101.hpp"
#include "source_campaign_backend_v1.hpp"
#include "source_current_level_backend_v1.hpp"

// Link proof only: retain the genuine provider/binder/current-Level definitions
// without constructing a synthetic World, Level, Character, or callback owner.
int main() {
  auto volatile binder = &model_renderer::bind_backend_character_fsm_v1;
  auto volatile register_context =
      &dh::foundation::actor_frame::SourceCampaignBackendContextV1::register_current;
  auto volatile unregister_context =
      &dh::foundation::actor_frame::SourceCampaignBackendContextV1::unregister_current;
  auto volatile level_current =
      &dh::foundation::actor_frame::SourceCurrentLevelBackendV1::current;
  return binder && register_context && unregister_context && level_current
             ? 0
             : 1;
}
