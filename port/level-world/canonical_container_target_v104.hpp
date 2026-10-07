#pragma once
#include "canonical_gameobject_graph_v68.hpp"
#include "character_world_runtime_v1.hpp"
namespace dh2::world {
struct CanonicalContainerTargetServicesV104 {
 std::shared_ptr<void> provider;
 CanonicalBaseBorrowV68 base;
 std::function<bool(std::shared_ptr<RetainedGameObjectVisualV1>&,std::string&)> visual;
 //Exact original selected Container.state394; all three classes inherit
 //Container.IsDead39f364 and IsInteractive39f384. No parallel state copy.
 std::function<bool(const std::int32_t*&,std::shared_ptr<void>&,std::string&)> state394;
 std::int32_t selected_interaction_type{}; //source Openable0 / Destructible8
 std::function<bool(std::uintptr_t,std::string&)> interact;
};
//Index adapter only on the SAME canonical object and existing target runtime.
//Owns no GameObject, Character, scene, life, FSM, source map or registry.
class CanonicalContainerTargetV104 final {
 CanonicalObjectManagerV1& manager_;
 character::skills::CharacterWorldRuntimeV1& targets_;
 CanonicalContainerTargetServicesV104 services_;
 target_search::Object48 search_{};
 std::shared_ptr<void> receiver_inflight_,state_inflight_;
 std::shared_ptr<RetainedGameObjectVisualV1> visual_inflight_;
 std::uintptr_t identity_{};std::int32_t published_key_{};
 bool registered_{};std::string error_;
 bool actual(CanonicalGameObjectBaseOwnerV1*&,std::string&);
 static int refresh(void*,character::skills::WorldTargetActorBorrowV1*);
 static int query(void*,std::uint32_t,std::uintptr_t,std::int32_t*);
public:
 CanonicalContainerTargetV104(CanonicalObjectManagerV1&,character::skills::CharacterWorldRuntimeV1&,CanonicalContainerTargetServicesV104);
 bool register_after_init_post(std::string&);
 //Actual factory/journal calls only AFTER canonical ObjectManager erase.
 //A still-published source object cannot be retired from this index early.
 bool retire_after_unpublication(std::string&);
 bool registered()const noexcept{return registered_;}
 bool source_interact_v114(std::uintptr_t character,std::string& e){struct Scope{CanonicalContainerTargetV104& self;~Scope(){self.receiver_inflight_.reset();self.state_inflight_.reset();self.visual_inflight_.reset();}} scope{*this};CanonicalGameObjectBaseOwnerV1* base{};if(!actual(base,e)||!services_.interact){if(e.empty())e="Actual selected Container.Interact provider required";return false;}return services_.interact(character,e);}
};
}
