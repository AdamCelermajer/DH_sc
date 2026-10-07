#pragma once
#include "canonical_class_receiver_bindings_v1.hpp"
#include "game_object_initialization_owner_v1.hpp"
#include "module_xml_selection_v1.hpp"
namespace dh2::world {
struct LevelConfigServicesV1 {
 std::shared_ptr<void> owner;
 std::function<bool(const char*,bool&,std::string&)> debug_switch;
 // Must reach the actual SAME current Level::SetLevelConfig, not a second World.
 std::function<bool(std::uintptr_t,std::string&)> set_level_config;
};
// Source factory GO_ID4; actual ObjectBase inheritance (no GameObject fields).
class CanonicalLevelConfigV1 {
 std::shared_ptr<void> pin_;std::uintptr_t identity_;
 target_providers::Handle16 handle_;
 std::uint32_t type_{4};std::int32_t room_{-1};const char* class_name_{};
 std::string template_;
 // Original ConditionDataC1 compiled=NULL/tested=0, not GameObject fields.
 std::uintptr_t condition_a8_v95_{},condition_cc_v95_{};
 std::uint8_t tested_ac_v95_{},tested_d0_v95_{};
 std::map<std::uint32_t,std::uint8_t> bytes_;
 std::map<std::uint32_t,std::int32_t> ints_;
 std::map<std::uint32_t,float> floats_;
 std::map<std::uint32_t,std::string> strings_;
 std::map<std::uint32_t,std::array<float,3>> vectors_;
 CanonicalPoint3ListV1 dfog_;
 LevelConfigServicesV1 services_;
 static bool set_name(void*,const char*,std::string&);
 static bool set_archetype(void*,const char*,std::string&);
 static bool as_character(void*,std::uintptr_t&,std::string&);
 static bool across(void*,std::uint8_t&,std::string&);
 static bool read_bool(void*,std::uint32_t,std::uint8_t&,std::string&);
 static bool write_bool(void*,std::uint32_t,std::uint8_t,std::string&);
 static bool write_int(void*,std::uint32_t,std::int32_t,std::string&);
 static bool write_float(void*,std::uint32_t,float,std::string&);
 static bool write_string(void*,std::uint32_t,const std::string&,std::string&);
 static bool write_vector(void*,std::uint32_t,const std::array<float,3>&,std::string&);
 static bool write_point2(void*,std::uint32_t,const std::array<std::int32_t,2>&,std::string&);
 static bool write_list(void*,std::uint32_t,const CanonicalPoint3ListV1&,std::string&);
public:
 CanonicalLevelConfigV1(std::shared_ptr<void>,LevelConfigServicesV1);
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void>);
 CanonicalPropertyActorV1 properties()noexcept;
 bool init_post(std::string&);
 bool source_loading_fields_v95(std::shared_ptr<void>,CanonicalObjectLoadingFieldsV95&,std::string&);
 const std::string* string(std::uint32_t)const noexcept;
 const std::array<float,3>* vector(std::uint32_t)const noexcept;
 float* source_vector_component_v90(std::uint32_t offset,std::size_t component)noexcept{
  auto i=vectors_.find(offset);return i!=vectors_.end()&&component<3?&i->second[component]:nullptr;
 }
 const std::int32_t* integer(std::uint32_t)const noexcept;
 const float* scalar_float_v55(std::uint32_t)const noexcept;
 const std::uint8_t* byte(std::uint32_t)const noexcept;
 // Existing actual ObjectBase save cells only. Never insert a missing C1/default.
 std::uint8_t* source_save_byte_v91(std::uint32_t offset)noexcept{auto p=bytes_.find(offset);return p==bytes_.end()?nullptr:&p->second;}
 const CanonicalPoint3ListV1& dfog_colors()const noexcept{return dfog_;}
 static constexpr bool is_game_object()noexcept{return false;}
 std::uintptr_t identity()const noexcept{return identity_;}
};
// One original static Module counter and generated zone counter per retained
// runtime, shared by all actual Module receivers. No counter copied per class.
struct ModuleRuntimeGlobalsV1 {std::uint32_t next_module_id{},next_generated_zone{};};
struct ModuleInitServicesV1 {
 std::shared_ptr<void> owner;
 std::function<bool(std::uintptr_t,std::string&)> visual_apply_mesh_box;
 std::function<bool(std::uintptr_t,bool&,std::string&)> visual_physical;
 std::function<bool(std::uintptr_t&,std::string&)> construct_podecor;
 std::function<bool(std::uintptr_t,bool,std::string&)> set_physical;
 // Source PFWorld::LoadRoom + solid flag + ExtendBoundingBox. Required only
 // visual present and ctor375=1; successful source Room miss is allowed.
 std::function<bool(std::uintptr_t,std::int32_t,const std::string&,bool,const float*,std::string&)> load_floor;
 std::function<bool(std::uintptr_t,std::array<float,6>&,std::string&)> root_world_bounds;
 // Whole source Spawn RoomZone(false,true), Resolve and GO_ID11 gate. A real
 // source miss returns type !=11 or handle.cached0, never a fake Zone object.
 std::function<bool(const std::string&,target_providers::Handle16&,std::uint32_t&,std::string&)> spawn_room_zone;
 std::function<bool(target_providers::Handle16&,const std::array<float,6>&,std::uintptr_t,std::string&)> zone_init;
};
class CanonicalModuleV1 {
 CanonicalGameObjectBaseOwnerV1 base_;GameObjectInitializationOwnerV1 initialization_;
 CanonicalPropertyFieldServicesV1 inherited_;ModuleRuntimeGlobalsV1& globals_;
 ModuleInitServicesV1 services_;ModuleXmlFieldsV1 xml_;
 std::array<float,3> fog_{-1,-1,-1};std::uint8_t floor375_{1},solid376_{1};
 std::uint8_t visited3fc_{0}; // genuine ModuleC1 38a0b0
 target_providers::Handle16 zone_{0,UINT32_MAX,0};std::int32_t id_{};
 static bool read_bool(void*,std::uint32_t,std::uint8_t&,std::string&);
 static bool write_bool(void*,std::uint32_t,std::uint8_t,std::string&);
 static bool write_int(void*,std::uint32_t,std::int32_t,std::string&);
 static bool write_float(void*,std::uint32_t,float,std::string&);
 static bool write_string(void*,std::uint32_t,const std::string&,std::string&);
 static bool write_vector(void*,std::uint32_t,const std::array<float,3>&,std::string&);
 static bool write_point2(void*,std::uint32_t,const std::array<std::int32_t,2>&,std::string&);
public:
 CanonicalModuleV1(std::shared_ptr<void>,actor::RuntimeState&,ModuleRuntimeGlobalsV1&,
                   GameObjectInitializationServicesV1,ModuleInitServicesV1);
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> lease){return base_.canonical(std::move(lease));}
 CanonicalPropertyActorV1 properties()noexcept;
 bool init_post(std::string&);
 bool init_final(bool& eligible,std::string& e){return initialization_.init_final(eligible,e);}
 bool load(const ModuleXmlServicesV1& s,const ModuleLevelLoadBorrowV1& l,std::string& e){return module_load_v1(xml_,id_,base_.vector3(0x160),s,l,e);}
 const ModuleXmlFieldsV1& xml()const noexcept{return xml_;}
 const std::array<float,3>& fog_color()const noexcept{return fog_;}
 std::array<float,3>& source_fog_color_v67()noexcept{return fog_;}
 std::uint8_t& source_visited3fc_v91()noexcept{return visited3fc_;}
 target_providers::Handle16& source_zone400_v91()noexcept{return zone_;}
 std::int32_t module_id()const noexcept{return id_;}
 std::uint8_t load_floor_pending()const noexcept{return floor375_;}
};
}
