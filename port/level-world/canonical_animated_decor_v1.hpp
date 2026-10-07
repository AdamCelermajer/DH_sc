#pragma once
#include "canonical_class_receiver_bindings_v1.hpp"
#include "game_object_initialization_owner_v1.hpp"
namespace dh2::world {
struct AnimatedDecorServicesV1 {
 std::shared_ptr<void> owner;
 std::function<bool(std::uintptr_t,std::string&)> visual_sync;
 std::function<bool(std::uintptr_t,bool&,std::string&)> visual_physical28;
 // Exact PODecor clone388a2c over the same canonical base/PhysicalWorld.
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::uintptr_t&,std::string&)> construct_podecor;
 // Actual GameObject SetPhysicalObject394bf8(body,false), including old release.
 std::function<bool(std::uintptr_t,bool,std::string&)> set_physical;
 // Timeline calls use visual+38, never a detached animation bank.
 std::function<bool(std::uintptr_t,std::int32_t&,std::string&)> animation_count;
 std::function<bool(std::uintptr_t,const std::string&,bool&,std::string&)> animation_exists;
 // Source play index/name: remaining priority/start/callback args are zero.
 std::function<bool(std::uintptr_t,std::int32_t,bool,bool&,std::string&)> play_index;
 std::function<bool(std::uintptr_t,const std::string&,bool,bool&,std::string&)> play_name;
 // Borrow the genuine shared Random state; max is count-1, including zero.
 std::function<bool(std::int32_t,std::int32_t&,std::string&)> random;
 std::function<bool(std::uintptr_t,std::function<bool(std::string&)>,std::string&)> install_random_completion;
 // Source CallbackRandomAll assertion after Play returned false.
 std::function<bool(std::string&)> random_play_assertion;
 std::function<bool(std::string&)> update;
 std::function<bool(std::string&)> destroy_base;
};
// Genuine factory342600 GO_ID20 and AnimatedDecor InitPost389128 control.
// The caller retains its sole RuntimeState before this owner and its real
// visual/timeline/PhysicalWorld providers until destroy() completes.
class CanonicalAnimatedDecorV1 {
 CanonicalGameObjectBaseOwnerV1 base_;
 GameObjectInitializationOwnerV1 initialization_;
 AnimatedDecorServicesV1 services_;
 std::uint8_t load_floor375_{},solid376_{1};
 std::string startanim37c_;
 CanonicalPropertyFieldServicesV1 inherited_;
 static bool read_bool(void*,std::uint32_t,std::uint8_t&,std::string&);
 static bool write_bool(void*,std::uint32_t,std::uint8_t,std::string&);
 static bool write_int(void*,std::uint32_t,std::int32_t,std::string&);
 static bool write_float(void*,std::uint32_t,float,std::string&);
 static bool write_string(void*,std::uint32_t,const std::string&,std::string&);
 static bool write_vector3(void*,std::uint32_t,const std::array<float,3>&,std::string&);
 static bool write_point2(void*,std::uint32_t,const std::array<std::int32_t,2>&,std::string&);
 bool missing(const char*,std::string&)const;
 bool sync_and_physical(std::string&);
 bool random_animation(bool install_completion,std::string&);
public:
 CanonicalAnimatedDecorV1(std::shared_ptr<void>,actor::RuntimeState&,
                         GameObjectInitializationServicesV1,AnimatedDecorServicesV1);
 CanonicalAnimatedDecorV1(const CanonicalAnimatedDecorV1&)=delete;
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> lease){return base_.canonical(std::move(lease));}
 CanonicalPropertyActorV1 properties()noexcept;
 const std::string& start_animation()const noexcept{return startanim37c_;}
 std::uint8_t solid()const noexcept{return solid376_;}
 std::uint8_t load_floor()const noexcept{return load_floor375_;}
 // Same source GameObject MeetCondition38ab60: constant true.
 static constexpr bool meet_condition()noexcept{return true;}
 static constexpr bool is_animated()noexcept{return true;}
 static constexpr bool is_updatable()noexcept{return false;}
 bool init_post(std::string&);
 bool init_final(bool& eligible,std::string& e){return initialization_.init_final(eligible,e);}
 bool animation_finished(std::string&);
 bool destroy(std::string&);
};
}
