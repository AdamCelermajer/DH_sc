#pragma once
#include "original_art.hpp"
#include <map>
namespace dh::foundation::frontend::art::source {
using Matrix = std::array<float,6>;
struct ShapePart {std::uint32_t bitmap{};std::array<float,4> color{1,1,1,1};std::vector<HudGeometryVertex> vertices;};
struct Shape { std::uint32_t id{},bitmap{};std::vector<ShapePart> parts; };
struct Placement {std::string name;std::uint32_t character{};Matrix matrix{};std::array<float,4> multiply{1,1,1,1},add{};std::uint32_t clip_depth{};};
struct Clip {std::vector<std::vector<Placement>> frames;std::map<std::string,unsigned> labels;std::vector<unsigned> stop_frames;};
struct Movie {std::map<unsigned,Shape> shapes;std::map<unsigned,Clip> clips;std::map<unsigned,TextField> fields;std::vector<Placement> roots;};
const Movie& movie();
}
