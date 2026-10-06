#pragma once
#include <cstdint>
#include <cstddef>
namespace dh2::skinning {
struct VisualModuleV6 {const char* uri;std::uintptr_t descriptor;};
struct VisualCategoryV6 {const char* name;const char* default_uri;const VisualModuleV6* modules;std::uint32_t count,reserved;};
struct VisualCellV6 {std::int32_t id;std::uint32_t reserved;std::uintptr_t resource;};
struct VisualSelectionV6 {const VisualCategoryV6* categories;VisualCellV6* cells;std::uint32_t count,buffer_flags;std::uintptr_t weapons[2];std::uintptr_t root;};
enum class VisualOperationV6:std::uint32_t {construct_module=0x61ace8,retain=0x649080,release=0x31d584,update_buffers=0x6483c8,visibility=0x5890a8,construct_weapon=0x61bbd4,detach=0x473dc0,search=0x35a0e4,attach=0x473e14};
struct VisualRequestV6 {VisualOperationV6 operation;std::int32_t category,module,slot,mode;std::uintptr_t resource,descriptor;const char* text;};
struct VisualServicesV6 {void* context;bool(*invoke)(void*,VisualSelectionV6&,const VisualRequestV6&,std::uintptr_t*);};
static_assert(sizeof(void*)==8 && sizeof(VisualModuleV6)==16 && sizeof(VisualCategoryV6)==32 && sizeof(VisualCellV6)==16 && sizeof(VisualSelectionV6)==48 && sizeof(VisualRequestV6)==48 && sizeof(VisualServicesV6)==16);
// Service receiver/cells must survive synchronous callbacks. Source updateBuffer
// and the controller/root factories are explicit services, not fake successes.
}
extern "C" {
std::int32_t dh2_visual_category_v6(const dh2::skinning::VisualSelectionV6*,const char*);
std::int32_t dh2_visual_module_uri_v6(const dh2::skinning::VisualSelectionV6*,const char*);
// module_name is the source VisualObject input: '#' + name + '-mesh-skin'.
std::int32_t dh2_visual_module_v6(const dh2::skinning::VisualSelectionV6*,std::int32_t ignored_category,const char* module_name);
std::int32_t dh2_visual_set_modular_v6(dh2::skinning::VisualSelectionV6*,std::int32_t category,std::int32_t module,const dh2::skinning::VisualServicesV6*);
std::int32_t dh2_visual_set_category_v6(dh2::skinning::VisualSelectionV6*,std::int32_t category,std::int32_t module,std::uint32_t update,const dh2::skinning::VisualServicesV6*);
std::int32_t dh2_visual_set_weapon_v6(dh2::skinning::VisualSelectionV6*,const char* nullable_name,std::int32_t slot,std::int32_t mode,const dh2::skinning::VisualServicesV6*);
}
