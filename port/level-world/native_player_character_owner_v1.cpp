#include "native_player_character_owner_v1.hpp"
#include <stdexcept>

namespace dh2::character {

NativePlayerCharacterOwnerV1::NativePlayerCharacterOwnerV1()
    : Coordinator(reinterpret_cast<std::uintptr_t>(this)) {
    if (Coordinator::owner() != identity())
        throw std::logic_error("Native player Character and Coordinator identities differ");
}

bool NativePlayerCharacterOwnerV1::bind_session(
        std::uintptr_t* player_character_660,
        data::PlayerSavegameV1& save,
        data::PropertyState& properties,
        data::FreshInventoryOwnedV4* inventory,
        std::string& error) {
    const auto id = identity();
    if (!player_character_660 ||
        reinterpret_cast<std::uintptr_t>(player_character_660) % alignof(std::uintptr_t) ||
        Coordinator::owner() != id || save.character() != id) {
        error = "Native Character publication requires its canonical PlayerInfo and Save identities";
        return false;
    }
    if (inventory && (inventory->character() != id || inventory->properties() != &properties)) {
        error = "Native Character inventory does not borrow its canonical identity and properties";
        return false;
    }
    if (player_character_660_ && !matches_session(player_character_660_, *save_, *properties_, inventory_)) {
        error = "Native Character already has a different live session";
        return false;
    }
    if (player_character_660_ &&
        !matches_session(player_character_660, save, properties, inventory)) {
        error = "Native Character rebind changed an authoritative owner";
        return false;
    }
    if (*player_character_660 && *player_character_660 != id) {
        error = "PlayerInfo Character660 already belongs to another Character";
        return false;
    }
    if (!player_character_660_) {
        // Native Player publication follows the completed Character
        // constructor. Character C1/C2 overwrite ObjectBase's initial zero
        // gate with one; do not expose that gate before this fresh bind.
        save_eligibility_28_ = 1;
        // A source PlayerManager::RemoveCharacter receipt belongs to the
        // retired Level's Character. The next fresh Character C1 writes
        // ObjectBase+0x81=0 and +0x8a=1; _AddCharacter then calls
        // GameObject::SetVisible(1) for the local Player before QuickSave.
        // Recreate those active-generation facts here instead of carrying the
        // prior Level's delete/hide writes through this retained native owner.
        deleted_81_ = 0;
        deleted_state_82_ = 0; // Native neutral state; source C1 does not write +0x82.
        enabled_8a_ = 1;
        visibility_80_ = enabled_8a_;
        ooi_intent_412_ = 0;
        clear_ooi();
        // Character ctor 0x3a9684 initializes +0x14aa to zero.
        ooi_refresh_timer_14aa_ = 0;
        faery_character_420_ = 0;
        reward_gold_1500_ = -1;
        reward_xp_1504_ = -1;
        spawn_probability_result_270_ = -1;
        disabled_373_ = 0;
        // Character C1/C2 owns the fresh reaction gate at +0x14fc and writes
        // binary32 -1.0 before the first Character::Update can decay it.
        reaction_gate_14fc_ = -1.0f;
        checkpoint_position_1468_[0] = 0;
        checkpoint_position_1468_[1] = 0;
        checkpoint_position_1468_[2] = 0;
        checkpoint_revive_position_1474_[0] = 0;
        checkpoint_revive_position_1474_[1] = 0;
        checkpoint_revive_position_1474_[2] = 0;
        interaction_sequence_28_ = -1;
        interaction_active_4c_ = 0;
        interacting_object_50_ = 0;
        talk_objective_marker_2fa_ = 0;
    }
    player_character_660_ = player_character_660;
    save_ = &save;
    properties_ = &properties;
    inventory_ = inventory;
    *player_character_660_ = id;
    // This is the native Player component-adoption boundary for the source
    // PlayerManager::AddCharacter factory route. IDA: both source class names
    // resolve to GetNewInstance<Character>(0x340800), whose C1 argument at
    // 0x340818 is literal GO_IDS=0. The getter remains identity/session bound.
    go_ids_0f4_ = 0;
    go_ids_0f4_ready_ = 1;
    error.clear();
    return true;
}

bool NativePlayerCharacterOwnerV1::object_base_go_ids_0f4_for(
        std::uintptr_t character, std::uint32_t* value) const noexcept {
    if (!value || character != identity() || !go_ids_0f4_ready_ ||
        !player_character_660_ || *player_character_660_ != character ||
        !save_ || save_->character() != character)
        return false;
    *value = go_ids_0f4_;
    return true;
}

bool NativePlayerCharacterOwnerV1::bind_inventory(
        data::FreshInventoryOwnedV4& inventory,
        std::string& error) {
    const auto id = identity();
    if (!player_character_660_ || !save_ || !properties_ ||
        Coordinator::owner() != id || *player_character_660_ != id ||
        save_->character() != id) {
        error = "Native inventory attachment requires the published canonical Character, Save and properties";
        return false;
    }
    if (inventory.character() != id || inventory.properties() != properties_) {
        error = "Native inventory does not borrow the Character's canonical identity and properties";
        return false;
    }
    if (inventory_ == &inventory) {
        error.clear();
        return true;
    }
    if (inventory_) {
        error = "Native Character already owns a different live inventory";
        return false;
    }
    inventory_ = &inventory;
    error.clear();
    return true;
}

bool NativePlayerCharacterOwnerV1::unbind_session(std::string& error) {
    ooi_intent_412_ = 0;
    clear_ooi();
    ooi_refresh_timer_14aa_ = 0;
    if (!player_character_660_) {
        error.clear();
        return true;
    }
    // PlayerManager::RemoveCharacter clears PlayerInfo+0x660 before the
    // Character is deleted; only a different live Character is a mismatch.
    const bool association_matches = *player_character_660_ == identity() ||
                                     *player_character_660_ == 0;
    if (association_matches) *player_character_660_ = 0;
    player_character_660_ = nullptr;
    save_ = nullptr;
    properties_ = nullptr;
    inventory_ = nullptr;
    go_ids_0f4_ = 0;
    go_ids_0f4_ready_ = 0;
    faery_character_420_ = 0;
    spawn_probability_result_270_ = -1;
    disabled_373_ = 0;
    reaction_gate_14fc_ = -1.0f;
    checkpoint_position_1468_[0] = 0;
    checkpoint_position_1468_[1] = 0;
    checkpoint_position_1468_[2] = 0;
    checkpoint_revive_position_1474_[0] = 0;
    checkpoint_revive_position_1474_[1] = 0;
    checkpoint_revive_position_1474_[2] = 0;
    interaction_sequence_28_ = -1;
    interaction_active_4c_ = 0;
    interacting_object_50_ = 0;
    talk_objective_marker_2fa_ = 0;
    if (!association_matches) {
        error = "PlayerInfo Character660 changed before native Character retirement";
        return false;
    }
    error.clear();
    return true;
}

bool NativePlayerCharacterOwnerV1::matches_session(
        const std::uintptr_t* player_character_660,
        const data::PlayerSavegameV1& save,
        const data::PropertyState& properties,
        const data::FreshInventoryOwnedV4* inventory) const noexcept {
    return player_character_660_ == player_character_660 && save_ == &save &&
           properties_ == &properties && inventory_ == inventory &&
           player_character_660 && *player_character_660 == identity() &&
           save.character() == identity() &&
           (!inventory || (inventory->character() == identity() &&
                           inventory->properties() == &properties));
}

data::PlayerSavegameV1* NativePlayerCharacterOwnerV1::save_for(
        std::uintptr_t character) const noexcept {
    return character == identity() ? save_ : nullptr;
}

bool NativePlayerCharacterOwnerV1::check_spawn_probability_player_branch(
        std::uintptr_t character) noexcept {
    const auto id = identity();
    if (character != id || !player_character_660_ ||
        *player_character_660_ != id || !save_ || save_->character() != id)
        return false;
    spawn_probability_result_270_ = -2;
    return true;
}

bool NativePlayerCharacterOwnerV1::restore_object_base_flags_v1(
        std::uintptr_t character, std::uint8_t visible,
        std::uint8_t enabled) noexcept {
    if (visible > 1 || enabled > 1 || character != identity() ||
        !player_character_660_ || *player_character_660_ != identity() ||
        !save_ || save_->character() != identity()) return false;
    visibility_80_ = visible;
    enabled_8a_ = enabled;
    return true;
}

bool NativePlayerCharacterOwnerV1::restore_spawn_probability_result_270_v1(
        std::uintptr_t character, std::int32_t value) noexcept {
    if (character != identity() || !player_character_660_ ||
        *player_character_660_ != identity() || !save_ ||
        save_->character() != identity()) return false;
    spawn_probability_result_270_ = value;
    return true;
}

bool NativePlayerCharacterOwnerV1::set_disabled_373(
        std::uintptr_t character, std::uint8_t value) noexcept {
    const auto id = identity();
    if (value > 1 || character != id || !player_character_660_ ||
        *player_character_660_ != id || !save_ || save_->character() != id)
        return false;
    disabled_373_ = value;
    return true;
}

bool NativePlayerCharacterOwnerV1::sg_block(
        std::uintptr_t character, std::uint32_t blocked) noexcept {
    if (blocked > 1 || character != identity() || !player_character_660_ ||
        *player_character_660_ != identity() || !save_ ||
        save_->character() != identity()) return false;
    save_->set_source_save_blocked(blocked != 0);
    return true;
}

bool NativePlayerCharacterOwnerV1::save_checkpoint_position(
        std::uintptr_t character, const std::uint32_t source_xyz[3]) noexcept {
    if (!source_xyz || character != identity() || !player_character_660_ ||
        *player_character_660_ != identity() || !save_ ||
        save_->character() != identity()) return false;
    checkpoint_position_1468_[0] = source_xyz[0];
    checkpoint_position_1468_[1] = source_xyz[1];
    checkpoint_position_1468_[2] = source_xyz[2];
    return true;
}

const std::uint32_t* NativePlayerCharacterOwnerV1::checkpoint_position_1468(
        std::uintptr_t character) const noexcept {
    return character == identity() && player_character_660_ &&
           *player_character_660_ == identity() && save_ &&
           save_->character() == identity() ? checkpoint_position_1468_ : nullptr;
}

bool NativePlayerCharacterOwnerV1::save_checkpoint_revive_position(
        std::uintptr_t character, const std::uint32_t source_xyz[3]) noexcept {
    if (!source_xyz || character != identity() || !player_character_660_ ||
        *player_character_660_ != identity() || !save_ ||
        save_->character() != identity()) return false;
    checkpoint_revive_position_1474_[0] = source_xyz[0];
    checkpoint_revive_position_1474_[1] = source_xyz[1];
    checkpoint_revive_position_1474_[2] = source_xyz[2];
    return true;
}

const std::uint32_t* NativePlayerCharacterOwnerV1::checkpoint_revive_position_1474(
        std::uintptr_t character) const noexcept {
    return character == identity() && player_character_660_ &&
           *player_character_660_ == identity() && save_ &&
           save_->character() == identity() ? checkpoint_revive_position_1474_ : nullptr;
}

bool NativePlayerCharacterOwnerV1::set_interaction_state_v1(
        std::uintptr_t character, std::int32_t sequence,
        std::uint8_t active, std::uintptr_t interactor) noexcept {
    const auto id = identity();
    if (character != id || sequence < 0 || active > 1 || !interactor ||
        interactor == id || !player_character_660_ ||
        *player_character_660_ != id || !save_ || save_->character() != id)
        return false;
    interaction_sequence_28_ = sequence;
    interaction_active_4c_ = active;
    interacting_object_50_ = interactor;
    return true;
}

bool NativePlayerCharacterOwnerV1::interaction_state_v1(
        std::uintptr_t character, std::int32_t* sequence,
        std::uint8_t* active, std::uintptr_t* interactor) const noexcept {
    const auto id = identity();
    if (character != id || !sequence || !active || !interactor ||
        !player_character_660_ || *player_character_660_ != id ||
        !save_ || save_->character() != id)
        return false;
    *sequence = interaction_sequence_28_;
    *active = interaction_active_4c_;
    *interactor = interacting_object_50_;
    return true;
}

bool NativePlayerCharacterOwnerV1::consume_interaction_sequence_v1(
        std::uintptr_t character, std::int32_t* sequence) noexcept {
    const auto id = identity();
    if (character != id || !sequence || interaction_sequence_28_ < 0 ||
        !player_character_660_ || *player_character_660_ != id ||
        !save_ || save_->character() != id)
        return false;
    *sequence = interaction_sequence_28_;
    interaction_sequence_28_ = -1;
    return true;
}

std::uint8_t NativePlayerCharacterOwnerV1::talk_objective_marker_2fa_for(
        std::uintptr_t character) const noexcept {
    return character == identity() && player_character_660_ &&
           *player_character_660_ == identity() && save_ &&
           save_->character() == identity() ? talk_objective_marker_2fa_ : 0;
}

bool NativePlayerCharacterOwnerV1::set_talk_objective_marker_2fa(
        std::uintptr_t character, std::uint8_t value) noexcept {
    const auto id = identity();
    if (character != id || value > 1 || !player_character_660_ ||
        *player_character_660_ != id || !save_ || save_->character() != id)
        return false;
    talk_objective_marker_2fa_ = value;
    return true;
}

data::PropertyState* NativePlayerCharacterOwnerV1::properties_for(
        std::uintptr_t character) const noexcept {
    return character == identity() ? properties_ : nullptr;
}

data::FreshInventoryOwnedV4* NativePlayerCharacterOwnerV1::inventory_for(
        std::uintptr_t character) const noexcept {
    return character == identity() ? inventory_ : nullptr;
}

std::uintptr_t NativePlayerCharacterOwnerV1::faery_character_420_for(
        std::uintptr_t character) const noexcept {
    return character == identity() && player_character_660_ &&
           *player_character_660_ == identity() ? faery_character_420_ : 0;
}

bool NativePlayerCharacterOwnerV1::set_faery_character_420(
        std::uintptr_t character, std::uintptr_t faery_character,
        std::string& error) noexcept {
    const auto id = identity();
    if (character != id || !player_character_660_ ||
        *player_character_660_ != id || !save_ || save_->character() != id ||
        (faery_character && faery_character == id)) {
        error = "Character+0x420 write requires the published Player and a distinct Faery Character";
        return false;
    }
    faery_character_420_ = faery_character;
    error.clear();
    return true;
}

bool NativePlayerCharacterOwnerV1::read_reaction_gate_14fc(
        std::uintptr_t character, float* value) const noexcept {
    const auto id = identity();
    if (!value || character != id || !player_character_660_ ||
        *player_character_660_ != id || !save_ || save_->character() != id ||
        Coordinator::owner() != id) {
        return false;
    }
    *value = reaction_gate_14fc_;
    return true;
}

bool NativePlayerCharacterOwnerV1::write_reaction_gate_14fc(
        std::uintptr_t character, float value) noexcept {
    const auto id = identity();
    if (character != id || !player_character_660_ ||
        *player_character_660_ != id || !save_ || save_->character() != id ||
        Coordinator::owner() != id) {
        return false;
    }
    reaction_gate_14fc_ = value;
    return true;
}

std::int32_t* NativePlayerCharacterOwnerV1::reward_gold_1500_for(
        std::uintptr_t character) noexcept {
    return character == identity() && player_character_660_ &&
           *player_character_660_ == identity() ? &reward_gold_1500_ : nullptr;
}

std::int32_t* NativePlayerCharacterOwnerV1::reward_xp_1504_for(
        std::uintptr_t character) noexcept {
    return character == identity() && player_character_660_ &&
           *player_character_660_ == identity() ? &reward_xp_1504_ : nullptr;
}

bool NativePlayerCharacterOwnerV1::update_inventory_localization(
        std::uintptr_t character,
        data::ItemPresentationOwnerV5& presentation,
        const data::ItemTextServicesV5& text,
        std::string& error) const {
    const auto id = identity();
    if (character != id || Coordinator::owner() != id || !player_character_660_ ||
        *player_character_660_ != id || !save_ || save_->character() != id ||
        !properties_ || !inventory_ || inventory_->character() != id ||
        inventory_->properties() != properties_) {
        error = "SetLanguage player refresh requires the published Character and its canonical V4 inventory";
        return false;
    }
    return inventory_->update_localization(presentation, text, error);
}

} // namespace dh2::character
