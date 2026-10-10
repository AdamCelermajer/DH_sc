#pragma once
#include "source_combo_chain.hpp"
#include "../../../level-world/character_target_bindings.hpp"
#include "../../../level-world/character_world_ai_can_attack_v1.hpp"
#include "../../../level-world/character_path_commands.hpp"
#include "../../../level-world/character_controller_commands.hpp"
#include <memory>

namespace dh::foundation::combo {
struct OwnerFieldBindings {
    std::shared_ptr<void> lease;
    std::uintptr_t owner=0;
    const std::uint32_t* flags528=nullptr;
    const std::uint8_t* headingActive1b5=nullptr;
    const std::uintptr_t* objectOfInterest14a4=nullptr;
    const std::int8_t* objectOfInterestType14a8=nullptr;
};
// Refresh only original raw owner fields, preserving same AI continued/index/
// last/finisher. No default OOI/heading/flags are synthesized.
bool refresh_owner_fields(dh2::character::AttackState64&,const OwnerFieldBindings&,std::string& error);
struct ConsumerBindings {
    // Borrow canonical existing records. No target/controller/AI/FSM clone is
    // allocated. Lease must keep these records alive through synchronous reentry.
    std::shared_ptr<void> lease;
    dh2::character::AttackState64* attack=nullptr;
    dh2::character::TargetBindings48* targets=nullptr;
    dh2::character::ControllerCommandState32* controller=nullptr;
    const float* gamePosition=nullptr;
    float* headingAngle=nullptr;
    dh2::character::WorldAIAttackServicesV1 canAttack;
    // Actual active AIS+1c storage, reloaded AFTER AI_CanAttack callbacks.
    const std::uintptr_t* activeAIS=nullptr;
    std::function<bool(std::uintptr_t,std::int32_t,std::string&)> aisPreAttack;
    // Actual stable GetTargetPosition backing, not an invented feet point.
    std::function<bool(std::uintptr_t,const float*&,std::string&)> targetPosition;
    // Publishes source setter prefixes/last-target to existing host views only.
    // Do not create another target authority or perform speculative resets.
    std::function<bool(const dh2::character::TargetState48&,std::string&)> publishTargets;
};
class SourceComboConsumers {
public:
    bool bind(ConsumerBindings,std::string& error);
    // Handles only genuine look, preattack, and sticky-target operations. Other
    // operations remain with the existing retained cursor/state-event owner.
    bool execute(const Operation&,bool& handled,std::string& error);
private:
    bool coherent(std::string&)const;
    bool publish(std::string&);
    ConsumerBindings bindings_;
};
}
