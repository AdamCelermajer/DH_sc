#pragma once

#include "character_coordinator.hpp"
#include "../game-data/fresh_inventory_owned_v4.hpp"
#include "../game-data/item_presentation_v5.hpp"
#include "../game-data/player_savegame_v1.hpp"
#include <cstdint>
#include <string>

namespace dh2::character {

// Native 64-bit Character object for the rebuilt runtime. This deliberately
// does not reproduce the original ARM object layout: it owns the existing
// Coordinator and resolves the one live Save/property/inventory state by its
// stable native identity.
class NativePlayerCharacterOwnerV1 final : public Coordinator {
    std::uintptr_t* player_character_660_ = nullptr;
    data::PlayerSavegameV1* save_ = nullptr;
    data::PropertyState* properties_ = nullptr;
    data::FreshInventoryOwnedV4* inventory_ = nullptr;
    // Character::ChangeFaery writes Player Character+0x420 with the live
    // Faery Character selected by Level::PlaceFaeryAndFollowers. This is one
    // semantic pointer owner on the canonical Player, not a second faery map.
    std::uintptr_t faery_character_420_ = 0;
    // Reward_Gold::Give and Reward_XP::Give store their last applied amount
    // in Character+0x1500/+0x1504. The source initializes both to -1 and
    // Character::SG_Update consumes those same fields; keep them on this one
    // canonical Character owner rather than in quest/reward mirrors.
    std::int32_t reward_gold_1500_ = -1;
    std::int32_t reward_xp_1504_ = -1;
    // Typed projection of GameObject+0x1b4. GameObject::UpdatePath owns its
    // transitions; this is not a byte overlay on the ARM32 Character object.
    std::uint8_t game_object_moving_ = 0;
    // Character+0x412: OOI interaction intent. UseOOI/ForceUseOOI set it;
    // skill/cast focus and target-range callbacks clear it. It is separate
    // from both GameObject movement and Coordinator flags.
    std::uint8_t ooi_intent_412_ = 0;
    // Character ctor 0x3a9594 initializes signed OOI type +0x14a8 to -1.
    // MenuManager reads this same byte for the HUD action artwork.
    std::int8_t ooi_type_14a8_ = -1;
    // Character::UpdateObjectOfInterest retains the selected GameObject at
    // +0x14a4. Keep that identity beside its type on the same Player owner;
    // it is the target consumed by Ctrl_UseOOI, not CharAI's combat target.
    std::uintptr_t ooi_identity_14a4_ = 0;
    // UpdateObjectOfInterest's signed16 refresh countdown and +0x14ac/+0x14ad
    // per-update marker/changed bytes stay with this same Character owner.
    std::int16_t ooi_refresh_timer_14aa_ = 0;
    std::uint8_t ooi_marker_14ac_ = 0;
    std::uint8_t ooi_changed_14ad_ = 0;
    // Source GameObject visibility/enabled bytes are modeled as semantic state,
    // not ARM32 offsets in this native 64-bit owner.
    // ObjectBase starts +0x28 at zero. The first successful fresh Player bind
    // is the native projection's receipt that Character construction completed;
    // that source constructor writes the LevelSave gate to one.
    std::uint8_t save_eligibility_28_ = 0;
    // PlayerManager::AddCharacter asks the source factory for "Character";
    // the retained source factory table maps both Character and Player to
    // GetNewInstance<Character>(GO_IDS=0). Publish this adoption receipt only
    // with the successful same-identity PlayerInfo/Save session bind.
    std::uint32_t go_ids_0f4_ = 0;
    std::uint8_t go_ids_0f4_ready_ = 0;
    std::uint8_t visibility_80_ = 1;
    std::uint8_t enabled_8a_ = 1;
    // Source ObjectBase::setUpdating(+0x85) and Delete(+0x82) writes stay on
    // this same retained Player Character. +0x82 is only meaningful after
    // Delete; its source constructor does not establish an initial value.
    std::uint8_t updating_85_ = 0;
    std::uint8_t deleted_state_82_ = 0;
    // GameObject::CheckSpawnProbability initializes +0x270 to -1. Its
    // Character::IsPlayer branch writes -2 without consulting shared RNG.
    // Keep that result with this canonical Player GameObject.
    std::int32_t spawn_probability_result_270_ = -1;
    // GameObject constructor 0x38c130 zeros +0x373. GameObject::Disabled
    // writes one at the end of its source tail; retain that byte on the same
    // canonical Player GameObject rather than in a removal-side mirror.
    std::uint8_t disabled_373_ = 0;
    // ObjectBase::ObjectBase initializes +0x81 to zero. The player owner is
    // retained for the process lifetime, so only an explicit source Delete
    // producer may change this; renderer/session teardown is not deletion.
    std::uint8_t deleted_81_ = 0;
    // Character C2 initializes +0x14fc to the source binary32 -1.0f. Status
    // reactions read and replace this same Character field.
    float reaction_gate_14fc_ = -1.0f;
    // Character::QuickSave copies the current Character position words into
    // the serialized Character +0x1468 Point3D. Keep these as raw words so the
    // source memcpy is exact and does not change float representations.
    std::uint32_t checkpoint_position_1468_[3]{};
    // Level::CheckpointSave optionally records Character::Revive's destination
    // in serialized Character +0x1474. Preserve its three raw source words on
    // the same canonical Player owner used by Character serialization.
    std::uint32_t checkpoint_revive_position_1474_[3]{};
    // Character::SM_SetInteractState writes the selected sequence at +0x28,
    // its active/suppression byte at +0x4c, and the interacting GameObject at
    // +0x50. Character::Interact also consults +0x2fa before raising event 5.
    // These are the Player's canonical source facts, stored on this existing
    // Character/Coordinator owner rather than in a renderer callback mirror.
    std::int32_t interaction_sequence_28_ = -1;
    std::uint8_t interaction_active_4c_ = 0;
    std::uintptr_t interacting_object_50_ = 0;
    std::uint8_t talk_objective_marker_2fa_ = 0;

public:
    NativePlayerCharacterOwnerV1();
    NativePlayerCharacterOwnerV1(const NativePlayerCharacterOwnerV1&) = delete;
    NativePlayerCharacterOwnerV1& operator=(const NativePlayerCharacterOwnerV1&) = delete;

    std::uintptr_t identity() const noexcept {
        return reinterpret_cast<std::uintptr_t>(this);
    }

    std::uint8_t* game_object_moving_storage() noexcept {
        return &game_object_moving_;
    }
    std::uint8_t game_object_moving() const noexcept {
        return game_object_moving_;
    }
    // Called after the synchronous DropPath and destination copy in Stop.
    void stop_game_object_moving() noexcept { game_object_moving_ = 0; }
    std::uint8_t* ooi_intent_storage() noexcept { return &ooi_intent_412_; }
    std::uint8_t ooi_intent() const noexcept { return ooi_intent_412_; }
    std::int8_t ooi_type() const noexcept { return ooi_type_14a8_; }
    std::int8_t* ooi_type_storage() noexcept { return &ooi_type_14a8_; }
    std::uintptr_t ooi_identity() const noexcept { return ooi_identity_14a4_; }
    std::int16_t ooi_refresh_timer() const noexcept { return ooi_refresh_timer_14aa_; }
    void set_ooi_refresh_timer(std::int16_t value) noexcept { ooi_refresh_timer_14aa_ = value; }
    std::uint8_t ooi_marker() const noexcept { return ooi_marker_14ac_; }
    std::uint8_t ooi_changed() const noexcept { return ooi_changed_14ad_; }
    void clear_ooi_changed() noexcept { ooi_changed_14ad_ = 0; }
    // UpdateObjectOfInterest clears only +0x14a4 when a retained Character is
    // no longer interactive. Type and marker remain until the next 500ms scan.
    void clear_ooi_target() noexcept { ooi_identity_14a4_ = 0; }
    bool set_ooi(std::uintptr_t identity, std::int8_t type) noexcept {
        if (!identity) { ooi_identity_14a4_ = 0; ooi_type_14a8_ = -1; ooi_marker_14ac_ = 0; return true; }
        ooi_identity_14a4_ = identity; ooi_type_14a8_ = type; return true;
    }
    void set_ooi_selected(std::uintptr_t identity, std::int8_t type,
                          std::uint8_t marker, bool changed) noexcept {
        ooi_identity_14a4_ = identity;
        ooi_type_14a8_ = identity ? type : std::int8_t{-1};
        ooi_marker_14ac_ = identity ? marker : 0;
        ooi_changed_14ad_ = identity && changed ? 1 : 0;
    }
    void clear_ooi() noexcept {
        ooi_identity_14a4_ = 0; ooi_type_14a8_ = -1;
        ooi_marker_14ac_ = 0; ooi_changed_14ad_ = 0;
    }
    std::uint8_t visibility_80() const noexcept { return visibility_80_; }
    std::uint8_t enabled_8a() const noexcept { return enabled_8a_; }
    std::uint8_t* visibility_80_storage() noexcept { return &visibility_80_; }
    std::uint8_t* enabled_8a_storage() noexcept { return &enabled_8a_; }
    bool restore_object_base_flags_v1(std::uintptr_t character,
                                      std::uint8_t visible,
                                      std::uint8_t enabled) noexcept;
    std::uint8_t* updating_85_storage() noexcept { return &updating_85_; }
    std::uint8_t* deleted_state_82_storage() noexcept { return &deleted_state_82_; }
    std::uint8_t save_eligibility_28() const noexcept {
        return save_eligibility_28_;
    }
    std::uint8_t* save_eligibility_28_storage() noexcept {
        return &save_eligibility_28_;
    }
    std::uint8_t deleted_81() const noexcept { return deleted_81_; }
    std::uint8_t* deleted_81_storage() noexcept { return &deleted_81_; }
    std::int32_t spawn_probability_result_270() const noexcept {
        return spawn_probability_result_270_;
    }
    bool restore_spawn_probability_result_270_v1(
        std::uintptr_t character, std::int32_t value) noexcept;
    std::uint8_t disabled_373() const noexcept { return disabled_373_; }
    bool set_disabled_373(std::uintptr_t character,
                          std::uint8_t value) noexcept;
    // Exact CheckSpawnProbability Player branch: only a bound canonical
    // Character can take the IsPlayer fast path, which stores -2.
    bool check_spawn_probability_player_branch(std::uintptr_t character) noexcept;
    bool read_reaction_gate_14fc(std::uintptr_t character,
                                 float* value) const noexcept;
    bool write_reaction_gate_14fc(std::uintptr_t character,
                                  float value) noexcept;
    bool render_batch_visible(bool environment_batch) const noexcept {
        return environment_batch || visibility_80_ != 0;
    }
    void set_enabled_8a(std::uint8_t value) noexcept { enabled_8a_ = value; }
    void set_visible(std::uint32_t raw_argument) noexcept {
        visibility_80_ = raw_argument ? enabled_8a_ : 0;
    }

    // Publication occurs only after these existing owners are ready. Inventory
    // is optional while its production Android owner is still disconnected.
    bool bind_session(std::uintptr_t* player_character_660,
                      data::PlayerSavegameV1& save,
                      data::PropertyState& properties,
                      data::FreshInventoryOwnedV4* inventory,
                      std::string& error);
    bool object_base_go_ids_0f4_for(std::uintptr_t character,
                                    std::uint32_t* value) const noexcept;
    // Attach the already-created authoritative inventory after source Save/
    // property setup, without clearing and republishing Character660. Exactly
    // one inventory may be adopted, and it must borrow these same owners.
    bool bind_inventory(data::FreshInventoryOwnedV4& inventory,
                        std::string& error);
    bool unbind_session(std::string& error);
    bool matches_session(const std::uintptr_t* player_character_660,
                         const data::PlayerSavegameV1& save,
                         const data::PropertyState& properties,
                         const data::FreshInventoryOwnedV4* inventory) const noexcept;

    data::PlayerSavegameV1* save_for(std::uintptr_t character) const noexcept;
    // Character::SG_Block(bool) writes its already-bound PlayerSavegame +0x0c
    // byte. This mutates the same Save owner; it does not create a gate mirror.
    bool sg_block(std::uintptr_t character, std::uint32_t blocked) noexcept;
    bool save_checkpoint_position(std::uintptr_t character,
                                  const std::uint32_t source_xyz[3]) noexcept;
    const std::uint32_t* checkpoint_position_1468(
        std::uintptr_t character) const noexcept;
    bool save_checkpoint_revive_position(
        std::uintptr_t character, const std::uint32_t source_xyz[3]) noexcept;
    const std::uint32_t* checkpoint_revive_position_1474(
        std::uintptr_t character) const noexcept;
    bool set_interaction_state_v1(std::uintptr_t character,
                                  std::int32_t sequence,
                                  std::uint8_t active,
                                  std::uintptr_t interactor) noexcept;
    bool interaction_state_v1(std::uintptr_t character,
                              std::int32_t* sequence,
                              std::uint8_t* active,
                              std::uintptr_t* interactor) const noexcept;
    bool consume_interaction_sequence_v1(std::uintptr_t character,
                                         std::int32_t* sequence) noexcept;
    std::uint8_t talk_objective_marker_2fa_for(
        std::uintptr_t character) const noexcept;
    bool set_talk_objective_marker_2fa(std::uintptr_t character,
                                       std::uint8_t value) noexcept;
    data::PropertyState* properties_for(std::uintptr_t character) const noexcept;
    data::FreshInventoryOwnedV4* inventory_for(std::uintptr_t character) const noexcept;
    std::uintptr_t faery_character_420_for(std::uintptr_t character) const noexcept;
    bool set_faery_character_420(std::uintptr_t character,
                                 std::uintptr_t faery_character,
                                 std::string& error) noexcept;
    std::int32_t* reward_gold_1500_for(std::uintptr_t character) noexcept;
    std::int32_t* reward_xp_1504_for(std::uintptr_t character) noexcept;
    // SavegameManager::SetLanguage's Character::UpdateInventoryLocalization
    // player branch. Resolves only this published Character's one V4 inventory
    // and refreshes its same V5 presentation records; it is not a Level walk.
    bool update_inventory_localization(
        std::uintptr_t character,
        data::ItemPresentationOwnerV5& presentation,
        const data::ItemTextServicesV5& text,
        std::string& error) const;
};

static_assert(sizeof(std::uintptr_t) == 8,
              "Native player Character owner requires a 64-bit runtime");

} // namespace dh2::character
