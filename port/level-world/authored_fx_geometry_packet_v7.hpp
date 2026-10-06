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
struct AuthoredFxGeometryChangeV34 {bool vertices{},indices{};};
struct AuthoredFxGeometryCountersV34 {std::uint64_t updates{},topology_rebuilds{},vertex_changes{},unchanged_vertices{};};
// Renderer cache only. Every live stream is still consumed and compared
// exactly; no source animation, particle, UV or color update is skipped.
class AuthoredFxGeometryCacheV34 {
 AuthoredFxGeometryPacketV7 packet_;
 std::vector<objects::Vertex> scratch_vertices_;
 std::vector<std::uint16_t> scratch_indices_;
 std::vector<std::uint32_t> source_indices_;
 std::size_t validated_vertex_count_{};
 AuthoredFxGeometryCountersV34 counters_;
public:
 bool update(const skinning::VisualDrawPartV6&,std::uint32_t,AuthoredFxGeometryChangeV34&,std::string&);
 const AuthoredFxGeometryPacketV7& packet()const noexcept{return packet_;}
 const AuthoredFxGeometryCountersV34& counters()const noexcept{return counters_;}
};
}
