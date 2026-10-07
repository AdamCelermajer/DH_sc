#pragma once
#include "savegame_stream_v2.hpp"
#include "canonical_gameobject_base_owner_v1.hpp"
namespace dh2::world {
using level::SavegameStreamV2;
struct ObjectSaveRestoreBorrowV3 {
 std::uintptr_t identity{};
 std::uint8_t* visible80{};std::uint8_t* enabled8a{};
 std::uint8_t* tested_ac{};std::uint8_t* tested_d0{};
 const std::int32_t* archetype270{};const std::uintptr_t* visual2d8{};
 bool* visible_produced{}; // observation of actual byte80 write; no stream bytes
};
struct ObjectSaveRestoreRequestV3 {
 std::uint32_t entry{},argument0{},argument1{};
 std::uintptr_t subject{},payload{};
};
struct ObjectSaveRestoreServicesV3 {
 void* context{};
 bool(*invoke)(void*,const ObjectSaveRestoreRequestV3&,std::string&){};
};
bool canonical_gameobject_save_borrow_v3(CanonicalGameObjectBaseOwnerV1&,ObjectSaveRestoreBorrowV3&,std::string&);
bool object_base_serialize_v3(SavegameStreamV2&,const ObjectSaveRestoreBorrowV3&,std::string&);
bool object_base_deserialize_v3(SavegameStreamV2&,const ObjectSaveRestoreBorrowV3&,std::string&);
bool gameobject_serialize_v3(SavegameStreamV2&,const ObjectSaveRestoreBorrowV3&,std::string&);
bool gameobject_deserialize_v3(SavegameStreamV2&,const ObjectSaveRestoreBorrowV3&,ObjectSaveRestoreServicesV3,std::string&);
}
