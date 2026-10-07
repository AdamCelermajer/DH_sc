#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
namespace dh2::world {class RetainedGameObjectVisualV1;}
namespace model_renderer {
struct LootVisualDrawSourceV27 {
 std::uintptr_t object{};
 std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;
};
// GL-thread adapter. Receivers/poses come from the actual canonical item graph
// and SAME SceneManager membership after its source world update.
bool sync_loot_visual_draws_v27(const std::vector<LootVisualDrawSourceV27>&,std::string&);
}
