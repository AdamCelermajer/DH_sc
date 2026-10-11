#pragma once
// B110: original merchant shop (Character::Interact -> openMerchantMenu 0x3a4d78, MenuMerchant,
// NativeMerchantBuyItem 0x4408d4, NativeMerchantSellItem 0x44058c, NativeMerchantGetItemsListForSlot 0x444168).
// Pure model over CharacterState + ItemTable (no GL); the Details-panel UI composition is in merchant_shop_ui_v1.
#include "../../character_state.hpp"
#include "../../../game-data/items.hpp"
#include "../../../game-data/loot_item_selection_v8.hpp"
#include "../../../game-data/loot_power_resources_v7.hpp"
#include "../../../game-data/loot_table_selection_v8.hpp"
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace dh::foundation::merchant {

// Arrays::MerchantTable row (Structs::Merchant::read 0x4ff604): BuyMultiplier (percent the merchant PAYS when it buys
// from the player, and charges for buyback items), SellMultiplier (percent charged for stock), MerchandiseList (Loot rows).
struct MerchantRow {
    std::int32_t buy_percent{}, sell_percent{};
    std::vector<std::int32_t> merchandise;   // ConditionId is not tested (Interact 0x3a4e80..b4 reads Merchandise only)
};

// The table follows the six loot tables of loot_table_pyarray.bin; `offset` is LootTablesV2::Borrow::consumed().
bool parse_merchant_rows(const std::uint8_t* data, std::size_t size, std::size_t offset,
                         std::vector<MerchantRow>& rows, std::string& error);

// ItemInstance value (+84) of a bare item: ARM32 wrapping product of Item words 27 and 28 (as source _CreateItem/SetValue).
std::int32_t item_value(const dh2::data::Item& item) noexcept;
// (int)(percent/100.0f * value), at least 1 (both Buy and Sell natives).
std::int32_t trade_price(std::int32_t value, std::int32_t percent) noexcept;

// AddLoot for every Merchandise row with the session loot RNG; every selected item is stocked (give-all).
using WithLootRandom = std::function<bool(const std::function<bool(dh2::data::LootRandom8V2&, std::string&)>&, std::string&)>;
bool roll_stock(const MerchantRow& row, const dh2::data::LootTablesV2::Borrow& tables,
                const dh2::data::LootPowerResourcesV7::Borrow& powers, const dh2::data::LootEntryServicesV8& entry,
                const WithLootRandom& with_rng, std::vector<std::pair<std::int32_t, std::uint32_t>>& item_quantities,
                std::string& error);

// NativeMerchantBuyItem return codes (the Flash menu shows MerchantErrorPopup for 1..3).
enum class TradeResult { ok = 0, insufficient_gold = 1, inventory_full = 2, potion_full = 3, not_found = 4, equipped = 5 };

struct TradeRules {
    std::int32_t potion_capacity{0};   // property 194 whole number (Character+936); 0 = not bound, no cap
    bool infinite_inventory{false};    // Debug switch InfiniteInventory
};

// Original inventory capacity (ItemInventory::IsInventoryFull 0x3fe330: more than 99 items).
inline constexpr std::size_t kInventoryCapacity = 100;

// One merchant's ItemInventory. Stock is held as a pseudo CharacterState so the shared Details list can present it.
class Shop {
public:
    explicit Shop(MerchantRow row) : row_(std::move(row)) {}
    const MerchantRow& row() const noexcept { return row_; }
    bool stocked() const noexcept { return stocked_; }
    // Source gate: stock is added only while the merchant inventory is empty (GetNumItems == 0).
    void add_stock(const dh2::data::ItemTable& items, const std::vector<std::pair<std::int32_t, std::uint32_t>>& item_quantities);
    CharacterState& inventory() noexcept { return stock_; }
    const CharacterState& inventory() const noexcept { return stock_; }
    bool is_buyback(const std::string& stock_instance) const;
    std::int32_t buy_price(const std::string& stock_instance, const dh2::data::ItemTable&) const;
    std::int32_t sell_price(const std::string& player_instance, const CharacterState& player, const dh2::data::ItemTable&) const;
    // Transfers one unit of the stock item to `player` and debits gold.
    TradeResult buy(const std::string& stock_instance, CharacterState& player, const dh2::data::ItemTable&,
                    const TradeRules&, std::int32_t& price);
    // Transfers one unit of an UNEQUIPPED player item to the shop (buyback) and credits gold.
    TradeResult sell(const std::string& player_instance, CharacterState& player, const dh2::data::ItemTable&, std::int32_t& price);
private:
    MerchantRow row_;
    CharacterState stock_;
    std::vector<std::string> buyback_;   // stock instance ids that came from the player (flag +105)
    std::uint32_t counter_{0};
    bool stocked_{false};
};

}
