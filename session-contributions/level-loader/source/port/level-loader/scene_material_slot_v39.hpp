#pragma once
#include <cstdint>
#include <string>
namespace dh2::scene {struct Scene;struct Instance;struct Material;}
namespace dh2::loader {
// Original constructGeometry61aeb8/constructController61ace8 assign binding
// slot i through the SAME mesh setMaterial(i,...). CMesh::setMaterial644bdc
// updates mesh-buffer slot i; no primitive-symbol/id search is performed.
// scene::load already preserves that exact SInstanceMaterial sequence in
// Instance.materials. This is the recovered source mapping, never a fallback.
// Borrow only the actual retained Scene/Instance inside a checked Visual
// consume scope. Output is that Scene's actual material; it expires with it.
// A missing slot/foreign instance/invalid catalog remains explicit; no first,
// global, default or ordinal guess substitutes for absent source bindings.
bool resolve_retained_scene_material_slot_v39(const scene::Scene&,
 const scene::Instance&,std::uint32_t primitive_slot,const scene::Material*& out,
 std::string&);
}
