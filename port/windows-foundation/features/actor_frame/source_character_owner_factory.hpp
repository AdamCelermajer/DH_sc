#pragma once
#include "../../../level-world/canonical_character_candidate_v60.hpp"
#include <map>

namespace dh::foundation::features {
// A loan of the canonical native owner graph. This deliberately has no
// ActorState storage: host presentation must borrow/project these owners.
struct SourceCharacterOwnerAliases {
    std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> lifetime;
    std::uintptr_t identity{};
    dh2::character::RetainedCharacterActorV1* character{};
    dh2::data::PropertyState* properties{};
    dh2::data::PropertyView* property_view{};
    dh2::data::CombatActorState* life{};
    dh2::data::FreshInventoryOwnedV4* inventory37c{};
    dh2::character::NativeFsm24* fsm{};
    dh2::character::RetainedCharacterFamilyVisualV6* visual_owner{};
    const std::uintptr_t* visual2d8{};
    const float* position160{};
    dh2::character::skills::CharacterPlayerSkillsV6* player_skills{};
    dh2::data::PlayerSavegameV1* save14e8{};
    // Camera target identity is the canonical Character, not its visual.
    std::uintptr_t camera_target()const noexcept{return identity;}
    bool validate(std::string&)const;
};

// Bind the EXISTING world factory. This adapter never creates a second
// factory beside the live graph. Canonical class bindings still own native
// Load/property/template/default/override/Add/InitPost ordering.
class SourceCharacterOwnerFactory {
    std::shared_ptr<dh2::world::CanonicalCharacterCandidateFactoryV60> factory_;
    // A successful source virtual58 delivery receipt, never a source field.
    using FinalDeliveries=std::map<std::uintptr_t,std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>>;
    std::shared_ptr<FinalDeliveries> final_deliveries_=std::make_shared<FinalDeliveries>();
    bool aliases(std::uintptr_t,SourceCharacterOwnerAliases&,std::string&)const;
public:
    explicit SourceCharacterOwnerFactory(std::shared_ptr<dh2::world::CanonicalCharacterCandidateFactoryV60>);
    // Whole native C1 prefix only. Wraps the returned original virtual58 so
    // final delivery can be proved without guessing from once1395 (which is
    // written BEFORE a failing InitFinal tail).
    bool construct(const dh2::world::CanonicalFactoryEntryV1&,
        const dh2::world::CanonicalSourceObjectRequestV1&,
        dh2::world::CanonicalClassReceiverV1&,std::string&);
    bool borrow_constructed_prefix(std::uintptr_t,SourceCharacterOwnerAliases&,std::string&)const;
    // Requires actual complete native InitPost + our actual virtual58 receipt.
    // Root must additionally validate PM/PlayerInfo, quests, camera, PF and
    // host publication consumers before its single graph commit.
    bool borrow_completed_character(std::uintptr_t,SourceCharacterOwnerAliases&,std::string&)const;
};
}
