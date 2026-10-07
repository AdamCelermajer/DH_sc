#pragma once
#include "../scene-materials/scene.hpp"
#include "floors.hpp"
#include <array>
#include <vector>
namespace dh2::world {
class CanonicalLevelConfigV1;
using Point=std::array<float,3>;
struct Triangle{Point a,b,c;unsigned room;};
struct Level{scene::Scene scene;std::vector<Triangle> floor;Point spawn{};unsigned rooms=0;std::unique_ptr<floors::World> native_floor;
 // The existing development Level retains its actual authored config here.
 // Canonical GSLevel publication/LevelConfig38 forwarding remains separate.
 std::shared_ptr<CanonicalLevelConfigV1> native_camera_config_v20;
};
bool load(const resources::BresView&,const std::uint8_t* descriptor,std::size_t,Level&,std::string&);
// Authored levels use the source-built floor selector/collision kernel. The
// radius/step/substep movement policy remains a new adapter; PF route search,
// obstacles and gameplay movement scheduling need further reconstruction.
bool height(const Level&,const Point&,float& result);
bool supported(const Level&,const Point&,float radius,float& floor_height);
bool move(const Level&,Point&,float dx,float dy,float radius);
}
