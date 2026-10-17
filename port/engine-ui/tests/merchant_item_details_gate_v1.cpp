#include "../merchant_item_details_gate_v1.hpp"

#include <iostream>
#include <stdexcept>

using namespace dh2::ui;

static void check(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}

int main() {
    using Kind = MerchantDetailsValueKindV1;
    const MerchantDetailsCallShapeV1 player{
        3, {Kind::number, Kind::object, Kind::number, Kind::other,
            Kind::other, Kind::other}};
    const MerchantDetailsCallShapeV1 merchant{
        6, {Kind::number, Kind::object, Kind::number, Kind::boolean,
            Kind::string, Kind::number}};
    const MerchantDetailsCallShapeV1 merchant_equipped_compare{
        5, {Kind::number, Kind::object, Kind::number, Kind::string,
            Kind::number, Kind::other}};
    check(merchant_details_call_form_v1(player) ==
          MerchantDetailsCallFormV1::player_inventory,
          "three-argument Player details ABI");
    check(merchant_details_call_form_v1(merchant) ==
          MerchantDetailsCallFormV1::merchant_inventory,
          "six-argument merchant details ABI");
    check(merchant_details_call_form_v1(merchant_equipped_compare) ==
          MerchantDetailsCallFormV1::player_inventory,
          "five-argument merchant screen equipped comparison uses player inventory");

    auto invalid = merchant;
    invalid.arguments[4] = Kind::number;
    check(merchant_details_call_form_v1(invalid) ==
          MerchantDetailsCallFormV1::ignored,
          "merchant name must retain its source string type");

    check(!merchant_details_owner_ready_v1({}),
          "no ObjectManager/merchant/inventory owners means fail closed");
    check(!merchant_details_owner_ready_v1({true, true, false}),
          "a merchant identity without its canonical inventory is insufficient");
    check(merchant_details_owner_ready_v1({true, true, true}),
          "a complete borrowed canonical merchant owner is sufficient");
    check(merchant_details_uses_merchant_inventory_v1(true, false),
          "a live exact merchant selection keeps merchant inventory details");
    check(!merchant_details_uses_merchant_inventory_v1(true, true),
          "a missing or nonmerchant selection falls back to Player details");
    check(!merchant_details_uses_merchant_inventory_v1(false, false),
          "a nonmerchant-form call always uses Player inventory details");
    std::cout << "{\"validation\":\"PASS\",\"checks\":10}\n";
}
