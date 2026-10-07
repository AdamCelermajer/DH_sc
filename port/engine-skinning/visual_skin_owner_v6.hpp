#pragma once
#include "visual_skin_selection_v6.hpp"
#include "skinning.hpp"
#include "skin_pose_cache_v32.hpp"
#include "../game-data/player_gear_effects_v5.hpp"
#include <memory>
#include <functional>
namespace dh2::skinning {
class VisualSkinOwnerV6;
struct VisualAttributeV6 {std::uint32_t type{},components{};std::vector<float> values;};
struct VisualPrimitiveV6 {std::string material_symbol;std::uint32_t collada_type{},engine_type{};std::array<std::int32_t,18> attributes{};std::vector<std::uint32_t> indices;};
struct VisualGeometryV6 {std::string id;std::vector<std::array<float,3>> positions;std::vector<VisualAttributeV6> attributes;std::vector<VisualPrimitiveV6> primitives;float minimum[3]{},maximum[3]{};};
struct VisualSkinPartV6 {VisualGeometryV6 geometry;Skin skin;std::vector<std::uint32_t> materials;};
struct VisualModuleResourceV6 {std::string uri;std::uint32_t descriptor_offset{};VisualSkinPartV6 part;};
struct VisualCategoryResourceV6 {std::string name,default_uri;std::vector<VisualModuleResourceV6> modules;};
class VisualSkinResourcesV6 {
 struct Storage;std::shared_ptr<const Storage> storage_;
public:
 class Borrow {friend class VisualSkinResourcesV6;friend class VisualSkinOwnerV6;std::shared_ptr<const Storage> storage_;explicit Borrow(std::shared_ptr<const Storage> s):storage_(std::move(s)){};
 public:Borrow()=default;explicit operator bool()const noexcept{return bool(storage_);}const scene::Scene& factory_scene()const;const std::vector<VisualCategoryResourceV6>& categories()const;const std::vector<std::uint8_t>& bytes()const;std::uint32_t modular_node()const;};
 // Actual serialized tag13. Atomic, immutable backing; rejects unproved extra
 // module array/nonlocal material/controller forms rather than guessing.
 bool load(const std::vector<std::uint8_t>& complete_bres,std::string& error);
 static bool source_has_modular_v114(const resources::BresView&,bool&,std::string&);
 Borrow borrow()const{return Borrow(storage_);}
};
//Source VisualObject2c is borrowed from SceneManager SearchByType. Keep the
//same mesh receiver; weak root lifetime prevents a native self-owning cycle.
struct SourceModularSkinBorrowV114 {std::weak_ptr<void> root;std::shared_ptr<VisualSkinOwnerV6> mesh;};
enum class VisualAssetResultV6 {found,missing,failed};
struct VisualAssetServicesV6 {void* context{};VisualAssetResultV6(*read)(void*,const char* requested_uri,std::vector<std::uint8_t>& owned_bytes,std::string& error){};};
struct VisualDrawPartV6 {std::shared_ptr<const void> retention;const VisualGeometryV6* geometry{};const std::vector<scene::Material>* material_table{};const std::vector<std::uint32_t>* materials{};std::vector<std::array<float,3>> positions;std::array<float,16> world{};bool skinned{};std::int32_t category{-1},module{-1},weapon_slot{};};
struct VisualDrawViewV32 {
 std::shared_ptr<const void> retention;
 const VisualGeometryV6* geometry{};const std::vector<scene::Material>* material_table{};
 const std::vector<std::uint32_t>* materials{};
 const std::vector<std::array<float,3>>* positions{};
 std::array<float,16> world{};bool skinned{},positions_changed{};
 std::int32_t category{-1},module{-1},weapon_slot{};
 std::uint64_t pose_revision{};
};
class VisualSkinOwnerV6 {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 // The live scene is the SAME stable CharacterAnimationInstance.scene(); it
 // must outlive this owner. The owner never appends nodes/recompiles its graph.
 VisualSkinOwnerV6(VisualSkinResourcesV6::Borrow,const scene::Scene& live_scene,VisualAssetServicesV6);
 ~VisualSkinOwnerV6();VisualSkinOwnerV6(const VisualSkinOwnerV6&)=delete;VisualSkinOwnerV6& operator=(const VisualSkinOwnerV6&)=delete;VisualSkinOwnerV6(VisualSkinOwnerV6&&)=delete;
 bool initialize(std::string&); // exact default-category order, genuine Skin resources
 std::int32_t category_id(const char*)const;std::int32_t module_id(std::int32_t,const char*)const;
 bool set_modular(std::int32_t,std::int32_t,std::string&);bool set_weapon(const char* nullable_name,std::int32_t slot,std::int32_t mode,std::string&);
 bool draw_parts(std::vector<VisualDrawPartV6>&,std::string&)const;
 // Synchronous render borrow; views/positions remain valid until the next
 // draw_views, selection mutation or owner destruction. retention separately
 // pins geometry/materials and may be stored for GPU topology identity. Never
 // store positions for deferred use. All clocks/events still advance normally.
 bool draw_views(const std::vector<VisualDrawViewV32>*&,std::string&)const;
 SkinPoseCountersV32 pose_counters()const noexcept;
 std::int32_t current_module(std::uint32_t)const;bool visibility_dirty()const noexcept;void clear_visibility_dirty()noexcept;
 std::string weapon_uri(std::int32_t slot)const;
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 // V5 Debug Load/GetSwitch remain mandatory genuine caller services. The
 // factory adapter validates the freshly re-read visual identity each call.
 data::GearSkinServicesV5 gear_services(data::GearSkinServicesV5 debug);
 // Canonical facade over SAME actual VisualObject2d8 and borrowed live Scene.
 // Slot is reread per delivery; no scene/visual or native pointer is cloned.
 data::GearSkinServicesV5 gear_services_for_source_visual_v62(data::GearSkinServicesV5 debug,const std::uintptr_t*);
};
}
