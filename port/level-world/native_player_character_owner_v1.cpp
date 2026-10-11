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
        ooi_intent_412_ = 0;
        clear_ooi();
        // Character ctor 0x3a9684 initializes +0x14aa to zero.
        ooi_refresh_timer_14aa_ = 0;
        faery_character_420_ = 0;
        reward_gold_1500_ = -1;
        reward_xp_1504_ = -1;
        checkpoint_position_1468_[0] = 0;
        checkpoint_position_1468_[1] = 0;
        checkpoint_position_1468_[2] = 0;
    }
    player_character_660_ = player_character_660;
    save_ = &save;
    properties_ = &properties;
    inventory_ = inventory;
    *player_character_660_ = id;
    error.clear();
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
    const bool association_matches = *player_character_660_ == identity();
    if (association_matches) *player_character_660_ = 0;
    player_character_660_ = nullptr;
    save_ = nullptr;
    properties_ = nullptr;
    inventory_ = nullptr;
    faery_character_420_ = 0;
    checkpoint_position_1468_[0] = 0;
    checkpoint_position_1468_[1] = 0;
    checkpoint_position_1468_[2] = 0;
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
