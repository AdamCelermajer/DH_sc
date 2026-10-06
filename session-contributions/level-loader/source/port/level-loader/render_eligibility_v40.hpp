#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <functional>
namespace dh2::scene {struct Scene;}
namespace dh2::loader {
// Original58b88c gate before node onRegister. Camera culling is the actual
// main producer; no readiness, pass enrollment or material-opacity inference.
bool source_node_registration_gate_v40(std::uint32_t flags11c,bool actual_culled)noexcept;
// Original custom rigid Mesh render35a9a8; C1 initializes byte138=1,
// Visual C1 sets it0 on the actual selected helper, independently of visibility.
bool source_rigid_mesh_render_gate_v40(std::uint8_t mesh138)noexcept;


struct SourceMeshPassProjectionV40 {
 std::uint32_t primary_pass{},secondary_pass{}; //0 means no source registration.
 bool prepare_only{};
};
// Source6463c8 projection. actual_mesh_mode is the actual mesh virtual38
// result; technique_flags is the real selected material-technique pass word.
// Never infer either from primitive type, color alpha, additive or node name.
// Numeric pass values preserve original ABI; this never enrolls a backend.
SourceMeshPassProjectionV40 source_mesh_pass_projection_v40(
 std::uint32_t actual_mesh_mode,std::uint32_t technique_flags,std::uint32_t node_flags)noexcept;
struct SourceNodeRegistrationBorrowV40 {
 std::shared_ptr<void> node_owner;
 std::uintptr_t identity{};
 const std::uint32_t* flags11c{};
};
struct SourceNodeRegistrationServicesV40 {
 std::shared_ptr<void> provider_owner;
 std::function<bool(std::uintptr_t,bool& actual_culled,std::string&)> is_culled;
 std::function<bool(std::uintptr_t,bool& source_visit_children,std::string&)> on_register;
};
struct SourceNodeRegistrationResultV40 {
 bool visible{},culled{},on_register_delivered{},visit_children{};
};
// Exact sequential visibility -> real isCulled -> real onRegister prefix.
// Delivery does not imply any pass was enrolled. Main owns the actual node
// virtual/material/technique/backend services and traversal/current frame.
// Missing services fail; no camera-false/opacity/default pass is fabricated.
bool deliver_source_node_registration_v40(const SourceNodeRegistrationBorrowV40&,
 const SourceNodeRegistrationServicesV40&,SourceNodeRegistrationResultV40& out,std::string&);
// Original352b74 true branch is PREFIX strncmp, root then children in source
// depth-first order; query is exactly "_colbox_". No renderer name filter.
// Finds first original node only. Actual typed mesh cast is a separate C1
// producer; do not skip a wrong type and search for a later helper.
bool find_source_visual_helper_v40(const scene::Scene&,std::uint32_t& node,std::string&);
}
