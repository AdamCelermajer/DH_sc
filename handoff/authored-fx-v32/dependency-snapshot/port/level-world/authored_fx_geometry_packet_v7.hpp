#pragma once
#include "objects.hpp"
#include "../engine-skinning/visual_skin_owner_v6.hpp"
namespace dh2::fx {
// Native upload packet from a retained source primitive. Engine primitive6
// is triangle-list; serialized COLLADA type0 maps to6, not GL enum or3.
struct AuthoredFxGeometryPacketV7 {
 std::vector<objects::Vertex> vertices;
 std::vector<std::uint16_t> indices;
 bool source_color_missing{};
};
bool authored_fx_geometry_packet_v7(const skinning::VisualDrawPartV6&,
 std::uint32_t material,AuthoredFxGeometryPacketV7&,std::string&);
}
