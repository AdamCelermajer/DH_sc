#include "../../../android-native/app/src/main/cpp/source_campaign_character_fsm_v101.hpp"
#include "source_campaign_backend_v1.hpp"
// Link-closure probe only: no native receiver is fabricated or dereferenced.
int main() {
  auto volatile binder = &model_renderer::bind_backend_character_fsm_v1;
  return binder ? 0 : 1;
}
