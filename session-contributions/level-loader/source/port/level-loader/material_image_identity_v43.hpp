#pragma once
#include <cstdint>
#include <array>
#include <functional>
#include <memory>
#include <string>
#include <vector>
namespace dh2::resources {struct BresView;}
namespace dh2::scene {struct Scene;struct Material;}
namespace dh2::loader {
enum class MaterialImageStateV43 {source_no_image, authored_image};
// Copied immutable source identity; not a texture object or GPU residency claim.
struct MaterialImageIdentityV43 {
 std::uint32_t material_catalog{},parameter_record{},parameter_order{},image_catalog{},image_record{};
 std::string material_id,parameter_name,image_id,authored_uri,source_resource_uri;
 MaterialImageStateV43 state{MaterialImageStateV43::source_no_image};
};
// Call synchronously within V37's checked consume scope for actor resources.
// Requires the SAME Scene/material and its SAME retained unrelocated BRES.
// This copies identities only. It never keeps actor/BRES pointers after return.
bool decode_retained_material_images_v43(const scene::Scene&,const scene::Material&,
 const resources::BresView&,const std::string& source_resource_uri,
 std::vector<MaterialImageIdentityV43>& out,std::string&);
struct MaterialImageServicesV43 {
 std::shared_ptr<void> owner;
 // Actual filesystem/mount policy owns full authored URI resolution. Do not
 // substitute an unrestricted filename search. Must return canonical URI.
 std::function<bool(const MaterialImageIdentityV43&,std::string&,std::string&)> canonical_uri;
 // Bind OriginalCacheAssetsV1::read through its retained owner. Allocation
 // admission/accounting stays an explicit main resource-owner service.
 std::function<bool(const std::string&,bool&,std::vector<std::uint8_t>&,std::string&)> read;
};
struct MaterialImageReadV43 {
 MaterialImageIdentityV43 identity;
 std::string canonical_uri;
 std::shared_ptr<const std::vector<std::uint8_t>> bytes;
 // Keeps the actual filesystem owner independently of actor release.
 std::shared_ptr<void> resource_owner;
};
bool read_material_image_v43(const MaterialImageIdentityV43&,const MaterialImageServicesV43&,
 MaterialImageReadV43& out,std::string&);
// Explicit supplied-cache compatibility policy, NOT recovered original mount
// routing. Only q:/data/iphone/ -> data/ and already-relative data/ accepted.
// Full hierarchy survives; legacy drive/parent paths remain unsupported.
bool supplied_cache_image_uri_v43(const MaterialImageIdentityV43&,std::string& out,std::string&);
// Original CResFactory6594a0 first tries resource_directory+'/'+SImage.path,
// then SImage.path; optional image-id cache key and embedded CResFile are main
// services. This preserves source order, not a filesystem normalization rule.
bool source_factory_image_candidates_v43(const std::string& resource_directory,
 const MaterialImageIdentityV43&,std::array<std::string,2>& out,std::string&);
}
