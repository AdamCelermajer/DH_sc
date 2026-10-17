#pragma once

#include <array>
#include <cstddef>

namespace dh2::ui {

enum class MerchantDetailsValueKindV1 : unsigned char {
    other,
    number,
    object,
    boolean,
    string,
};

enum class MerchantDetailsCallFormV1 : unsigned char {
    ignored,
    player_inventory,
    merchant_inventory,
};

struct MerchantDetailsCallShapeV1 {
    std::size_t count{};
    std::array<MerchantDetailsValueKindV1, 6> arguments{};
};

// NativeInvGetItemDetails has a three-argument Player form, a five-argument
// equipped-item comparison form (the merchant identity tail is ignored by the
// native unless nargs is exactly six), and a six-argument merchant form. This
// classifies only observed SWF/native forms; it owns no inventory.
inline MerchantDetailsCallFormV1 merchant_details_call_form_v1(
        const MerchantDetailsCallShapeV1& call) noexcept {
    using Kind = MerchantDetailsValueKindV1;
    const auto& a = call.arguments;
    if (call.count == 3 && a[0] == Kind::number && a[1] == Kind::object &&
        a[2] == Kind::number) {
        return MerchantDetailsCallFormV1::player_inventory;
    }
    if (call.count == 5 && a[0] == Kind::number && a[1] == Kind::object &&
        a[2] == Kind::number && a[3] == Kind::string &&
        a[4] == Kind::number) {
        // dqcharmenu_droid's merchant equipped-item comparison calls
        // (itemIndex, output, playerIndex, merchantName, merchantObjectId).
        // NativeInvGetItemDetails selects merchant inventory only for nargs=6;
        // this five-argument call follows its Player inventory path.
        return MerchantDetailsCallFormV1::player_inventory;
    }
    if (call.count == 6 && a[0] == Kind::number && a[1] == Kind::object &&
        a[2] == Kind::number && a[3] == Kind::boolean &&
        a[4] == Kind::string && a[5] == Kind::number) {
        return MerchantDetailsCallFormV1::merchant_inventory;
    }
    return MerchantDetailsCallFormV1::ignored;
}

struct MerchantDetailsOwnerBindingsV1 {
    bool object_manager_name_lookup{};
    bool merchant_character{};
    bool canonical_merchant_inventory{};
};

inline bool merchant_details_owner_ready_v1(
        const MerchantDetailsOwnerBindingsV1& bindings) noexcept {
    return bindings.object_manager_name_lookup && bindings.merchant_character &&
           bindings.canonical_merchant_inventory;
}

// A true merchant flag selects merchant inventory only when exact ObjectManager
// lookup resolves a live merchant Character. Source falls back to Player
// inventory when that selection is missing or resolves to another Character.
inline bool merchant_details_uses_merchant_inventory_v1(
        bool merchant_requested, bool source_selection_is_noop) noexcept {
    return merchant_requested && !source_selection_is_noop;
}

} // namespace dh2::ui
