#pragma once
#include "menu_flow.hpp"
#include "../../../game_save.hpp"
#include <memory>
namespace dh::foundation::frontend::flow {
enum class CanonicalOperation { create_slot, assign_slot, start_game, capture_checkpoint };
// ROOT-SUPPLIED authority contract. No implementation here substitutes the
// ordinary checkpoint serializer for original indexed save-slot ownership.
class CanonicalNavigationOwner {
public:
    virtual ~CanonicalNavigationOwner()=default;
    // Loan the same live CharacterState used by gameplay/inventory/equipment.
    // A detached frontend profile or make_default_character fixture is forbidden.
    virtual CharacterState* live_character() noexcept=0;
    virtual bool available(CanonicalOperation) const noexcept=0;
    // Root must stage_creation with a real CompleteCreationService and validate
    // the complete detached native candidate before any profile persistence.
    // Root owns full fresh source initialization, indexed persistence, seeds,
    // date, difficulty/progression and authoritative publication. These must not
    // synthesize success, mutate another profile graph or replay failed prefixes.
    virtual bool create_slot(CharacterState& same_live,const std::string& name,
                             const std::string& class_token,int& actual_slot,std::string&)=0;
    virtual bool assign_slot(CharacterState& same_live,int slot,int player,std::string&)=0;
    virtual bool start_game(CharacterState& same_live,int difficulty,std::string&)=0;
    // Root must capture same character plus actual world/RNG through existing
    // capture_game_save and complete campaign registered source owners. This
    // operation is independent of indexed source CreateSaveSlot/SG_Save.
    virtual bool capture_checkpoint(const CharacterState& same_live,GameSave&,std::string&)=0;
};
// Borrow-only bridge: keeps root owner alive and forwards the exact live loan on
// every operation. It stores no CharacterState, GameSave, slot map or save path.
Services bind_canonical_navigation(std::shared_ptr<CanonicalNavigationOwner>);
bool capture_canonical_checkpoint(CanonicalNavigationOwner&,GameSave&,std::string&);
}
