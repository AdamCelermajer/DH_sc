#pragma once

#include "prince_actor.hpp"
#include "../../android-app/scene_buffers.hpp"
#include "../upstream/include/IMesh.h"
#include "../upstream/include/ITexture.h"
#include "../upstream/include/SMesh.h"
#include "../upstream/include/SMeshBuffer.h"

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace dh2::irrlicht_adapter {

using irrlicht_game::PrinceActor;
using irrlicht_game::PrinceMeshPart;

enum class MeshBuildStatus : std::uint32_t {
    ok,
    argument,
    invalid_scene,
    range_error,
    no_visible_draws,
    allocation
};

// The callback chooses a texture using source material/sampler metadata. It
// returns a borrowed Irrlicht texture owned by the caller's video driver.
// Returning null leaves the mesh buffer untextured.
using TextureResolver = irr::video::ITexture* (*)(
    const viewer::SceneDrawDescriptor& draw,
    const viewer::SceneTextureReference* references,
    std::uint32_t reference_count,
    void* user_data);

// PFWorld room loading uses selected root bounds and named floor/exit helpers
// for non-scenery roles. The serialized Collada visibility bit can still be
// true for these nodes; it is not the room render policy.
enum class SourceRoomDrawRole : std::uint32_t {
    scenery,
    room_root_bounds,
    navigation_floor,
    exit_marker
};

struct SourceRoomRenderCounts {
    std::uint32_t room_root_bounds = 0;
    std::uint32_t navigation_floors = 0;
    std::uint32_t exit_markers = 0;
};

// The root is identified by the exact source-node record from ModuleBinding.
// Floor/exit roles match `_floor_`/`_exit_` at the start of a node ID or after
// a slash-delimited instance path. Plain substrings inside scenery names do
// not match.
inline SourceRoomDrawRole classify_source_room_draw(
    const viewer::SceneDrawDescriptor& draw,
    std::uint32_t room_root_node_record) noexcept {
    if (draw.node_record == room_root_node_record)
        return SourceRoomDrawRole::room_root_bounds;
    std::size_t length = 0;
    while (length < sizeof(draw.node_id) && draw.node_id[length]) ++length;
    if (length == sizeof(draw.node_id)) return SourceRoomDrawRole::scenery;

    const auto has_role_component = [&](const char* marker, std::size_t marker_size) {
        for (std::size_t i = 0; i + marker_size <= length; ++i) {
            if ((i == 0 || draw.node_id[i - 1] == '/') &&
                std::memcmp(draw.node_id + i, marker, marker_size) == 0)
                return true;
        }
        return false;
    };
    if (has_role_component("_floor_", sizeof("_floor_") - 1))
        return SourceRoomDrawRole::navigation_floor;
    if (has_role_component("_exit_", sizeof("_exit_") - 1))
        return SourceRoomDrawRole::exit_marker;
    return SourceRoomDrawRole::scenery;
}

// Converts an already flattened SceneMesh into one Irrlicht SMeshBuffer per
// visible SceneDrawDescriptor. The returned IMesh has its initial reference;
// the caller must eventually call drop().
//
// SceneMesh positions and UVs are copied without an extra transform. The
// adapter does not own or free the input SceneMesh.
irr::scene::IMesh* build_mesh(
    const viewer::SceneMesh& source,
    MeshBuildStatus* status = nullptr,
    TextureResolver texture_resolver = nullptr,
    void* texture_user_data = nullptr,
    std::uint32_t* output_buffer_count = nullptr);

// Builds only the visible scenery view of one authored source room. This
// filters the exact bound room-root mesh, source navigation floors, and exit
// markers while leaving the input SceneMesh and any navigation ownership
// untouched. Generic build_mesh retains its all-visible behavior.
irr::scene::IMesh* build_source_room_scenery_mesh(
    const viewer::SceneMesh& source,
    std::uint32_t room_root_node_record,
    MeshBuildStatus* status = nullptr,
    TextureResolver texture_resolver = nullptr,
    void* texture_user_data = nullptr,
    std::uint32_t* output_buffer_count = nullptr,
    SourceRoomRenderCounts* output_filtered_counts = nullptr);

using PrinceTextureResolver = irr::video::ITexture* (*)(
    const PrinceMeshPart& part, void* user_data);

// Owns one SMesh reference and keeps non-owning pointers to its mutable
// buffers. Adding mesh to a scene node takes a second mesh reference; the
// caller can then let this wrapper go out of scope after the node is destroyed.
struct MutablePrinceMesh {
    irr::scene::SMesh* mesh = nullptr;
    std::vector<irr::scene::SMeshBuffer*> buffers;
    std::uint32_t mapped_textures = 0;
    std::uint32_t unmapped_diffuse_textures = 0;
    std::uint32_t alpha_map_materials = 0;

    MutablePrinceMesh() = default;
    ~MutablePrinceMesh();
    MutablePrinceMesh(MutablePrinceMesh&& other) noexcept;
    MutablePrinceMesh& operator=(MutablePrinceMesh&& other) noexcept;
    MutablePrinceMesh(const MutablePrinceMesh&) = delete;
    MutablePrinceMesh& operator=(const MutablePrinceMesh&) = delete;
};

// Builds one mutable Irrlicht SMeshBuffer for each source Prince primitive.
// Only source diffuse references accepted by the resolver are bound; alpha
// maps and other unsupported source effects remain explicitly unrendered.
bool build_mutable_prince_mesh(
    const PrinceActor& source, MutablePrinceMesh* output, std::string& error,
    PrinceTextureResolver texture_resolver = nullptr,
    void* texture_user_data = nullptr);

// Copies only deformed positions, rebuilds normals/bounds and marks each
// buffer dirty for a dynamic hardware upload. UV/color/index/material streams
// stay immutable. The corresponding scene node must remain at identity.
bool update_mutable_prince_mesh(const PrinceActor& source,
                               MutablePrinceMesh& output,
                               std::string& error);

} // namespace dh2::irrlicht_adapter
