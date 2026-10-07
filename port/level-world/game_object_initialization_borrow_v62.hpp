#pragma once
#include "game_object_initialization_owner_v1.hpp"
#include "condition_data_init_v3.hpp"
namespace dh2::world {
// Borrowed source fields, not another ObjectBase/GameObject receiver. Every
// callback returns storage on the same retained class instance. In particular
// physical2dc and visual2d8 are reread after callback delivery.
struct GameObjectInitializationFieldsV62 {
 std::shared_ptr<void> receiver;
 std::uintptr_t source_identity{};
 std::function<std::uint8_t*(std::uint32_t)> byte;
 std::function<std::int32_t*(std::uint32_t)> integer;
 std::function<std::uintptr_t*(std::uint32_t)> pointer;
 std::function<std::string*(std::uint32_t)> string;
 std::function<float*(std::uint32_t)> vector3,scalar;
 std::function<float*()> relative_aabb144;
 std::function<void()> update_absolute_aabb;
 std::uintptr_t identity()const noexcept{return source_identity;}
};
bool condition_data_init_borrow_v62(const GameObjectInitializationFieldsV62&,std::uint32_t,const ConditionDataInitServicesV3&,std::string&);
bool condition_data_clear_borrow_v62(const GameObjectInitializationFieldsV62&,std::uint32_t,const ConditionDataInitServicesV3&,std::string&);
// Whole original inherited InitPost/InitFinal, using the same control and store
// order as V1. The field borrower allows Character's actual embedded ownership
// graph without constructing a parallel CanonicalGameObjectBaseOwnerV1.
class GameObjectInitializationBorrowV62 {
 GameObjectInitializationFieldsV62 fields_;
 GameObjectInitializationServicesV1 services_;
 bool missing(const char*,std::string&)const;
 bool spawn(std::int32_t&,std::string&);
public:
 GameObjectInitializationBorrowV62(GameObjectInitializationFieldsV62 f,GameObjectInitializationServicesV1 s):fields_(std::move(f)),services_(std::move(s)){}
 bool object_base_init_post(std::string&);
 bool check_spawn_probability(std::int32_t& roll,std::string& e){return spawn(roll,e);}
 bool init_post(bool&,std::string&);
 bool init_final(bool&,std::string&);
};
}
