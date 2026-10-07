#pragma once
#include "source_fx_node_matrix_v4.hpp"
namespace dh2::fx {
// Explicitly supplied by the actual source producer, never inferred from art.
enum class AuthoredPositionSpaceV39 { node_local,owner_local_skinned,particle_world,weapon_socket_local };
// Retained draw contract: camera is separate from owner/socket transforms.
// Source particle baker already writes world positions. Its draw matrix must
// be identity; adding outer/emitter again displaces/camera-skews the effect.
bool authored_fx_draw_world_v39(math::Matrix4f& out,AuthoredPositionSpaceV39,
 const math::Matrix4f* owner,const math::Matrix4f* node_or_socket,std::string&);
bool authored_fx_wvp_v39(math::Matrix4f& out,const math::Matrix4f& camera_projection_view,
 const math::Matrix4f& draw_world,std::string&);
}
