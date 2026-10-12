#pragma once
#include "canonical_property_map_v1.hpp"
#include "character_world_npc_object_v1.hpp"
#include "actor_runtime.hpp"
namespace dh2::world {
// Same base-field authority for actual fresh GameObject class receivers.
// Runtime is borrowed, never copied: caller retains its one PF/pose owner.
class CanonicalGameObjectBaseOwnerV1 {
 std::uintptr_t identity_{};std::shared_ptr<void> world_pin_;
 actor::RuntimeState& runtime_;
 std::uint32_t type_f4_{};target_providers::Handle16 handle_{};
 const char* class_name20_{};std::int32_t room64_{-1};
 std::string template_name8_;
 character::WorldNpcObjectFieldsV1 lifecycle_;
 std::map<std::uint32_t,std::uint8_t> bytes_;
 std::map<std::uint32_t,std::string> strings_;
 std::map<std::uint32_t,std::int32_t> integers_;
 std::map<std::uint32_t,std::uintptr_t> pointers_;
 std::array<float,3> scale120_{};
 bool across_rooms_produced_{};
 static bool set_name(void*,const char*,std::string&);
 static bool set_archetype(void*,const char*,std::string&);
 static bool as_character(void*,std::uintptr_t&,std::string&);
 static bool read_across_rooms(void*,std::uint8_t&,std::string&);
 static bool read_bool(void*,std::uint32_t,std::uint8_t&,std::string&);
 static bool write_bool(void*,std::uint32_t,std::uint8_t,std::string&);
 static bool write_int(void*,std::uint32_t,std::int32_t,std::string&);
 static bool write_float(void*,std::uint32_t,float,std::string&);
 static bool write_string(void*,std::uint32_t,const std::string&,std::string&);
 static bool write_vector3(void*,std::uint32_t,const std::array<float,3>&,std::string&);
 static bool write_point2(void*,std::uint32_t,const std::array<std::int32_t,2>&,std::string&);
public:
 // Only base GameObject constructor projection. Class-specific ctor is owned
 // by OpenableContainer/AnimatedDecor receiver and must continue it in order.
 CanonicalGameObjectBaseOwnerV1(std::uintptr_t,std::uint32_t source_go_id,
  std::shared_ptr<void> world_pin,actor::RuntimeState& fresh_runtime);
 CanonicalGameObjectBaseOwnerV1(const CanonicalGameObjectBaseOwnerV1&)=delete;
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> receiver_lease);
 CanonicalPropertyActorV1 properties()noexcept;
 actor::RuntimeState& runtime()noexcept{return runtime_;}
 std::uintptr_t identity()const noexcept{return identity_;}
 character::WorldNpcObjectFieldsV1& lifecycle()noexcept{return lifecycle_;}
 target_providers::Handle16& shared_handle()noexcept{return handle_;}
 // Stable source-field borrows; null means no recovered storage/producer.
 std::uint8_t* byte(std::uint32_t)noexcept;
 std::int32_t* integer(std::uint32_t)noexcept;
 std::uintptr_t* pointer(std::uint32_t)noexcept;
 std::string* string(std::uint32_t)noexcept;
 float* vector3(std::uint32_t)noexcept;
 float* scalar(std::uint32_t)noexcept;
 float* relative_aabb144()noexcept{return runtime_.subobjects.local_bounds;}
 float* absolute_aabb12c()noexcept{return runtime_.subobjects.absolute_bounds;}
 void update_absolute_aabb()noexcept;
 bool store_byte(std::uint32_t,std::uint8_t,std::string&);
 std::int32_t& room64()noexcept{return room64_;}
 std::uint32_t& type_f4()noexcept{return type_f4_;}
 const char*& class_name20()noexcept{return class_name20_;}
 // Genuine derived AnimatedDecor factory ctor stores static84=1.
 void animated_decor_constructor_static()noexcept{lifecycle_.static84=1;}
};
}
