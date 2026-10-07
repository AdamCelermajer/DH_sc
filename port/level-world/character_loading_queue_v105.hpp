#pragma once
#include "character_deferred_queue.hpp"
#include <memory>
namespace dh2::character {
// One process static s_concurrentAI9a2944 map, C1-empty. Campaign retirement
// unloads its actual entries; changing Worlds never constructs another map.
std::shared_ptr<CharacterDeferredQueue> character_loading_queue_v105();
}
