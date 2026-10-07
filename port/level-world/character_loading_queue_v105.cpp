#include "character_loading_queue_v105.hpp"
namespace dh2::character {
std::shared_ptr<CharacterDeferredQueue> character_loading_queue_v105(){
 static const auto owner=std::shared_ptr<CharacterDeferredQueue>(dh2_character_deferred_queue_create(),
  [](CharacterDeferredQueue* queue){if(queue)dh2_character_deferred_queue_destroy(queue);});
 return owner;
}
}
