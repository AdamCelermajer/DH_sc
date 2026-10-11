#include "merchant_shop_v1.hpp"
#include <cstdlib>
#include <iostream>

using namespace dh::foundation;
using namespace dh::foundation::merchant;
static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::cerr << "FAIL line " << __LINE__ << ": " #c "\n"; ++failures; } } while (0)

static dh2::data::ItemTable table() {
    dh2::data::ItemTable t;
    t.identifiers = {"Sword", "Potion0", "Helm"};
    t.rows.resize(3);
    t.rows[0].record.words[27] = 10; t.rows[0].record.words[28] = 5;           // value 50
    t.rows[1].record.words[7] = 1; t.rows[1].record.words[22] = 14;            // stackable potion
    t.rows[1].record.words[27] = 1; t.rows[1].record.words[28] = 3;            // value 3
    t.rows[2].record.words[27] = 1; t.rows[2].record.words[28] = 1;            // value 1
    return t;
}

int main() {
    // MerchantTable tail: count, then (buy, sell, n, n*(condition, merchandise)).
    {
        std::vector<std::uint8_t> b(4, 0xEE);   // pretend six loot tables occupy the first 4 bytes
        const auto w = [&](std::int32_t v) { for (int i = 0; i < 4; ++i) b.push_back(std::uint8_t(std::uint32_t(v) >> (8 * i))); };
        w(2); w(40); w(120); w(2); w(0); w(7); w(0); w(9); w(25); w(150); w(0);
        std::vector<MerchantRow> rows; std::string e;
        CHECK(parse_merchant_rows(b.data(), b.size(), 4, rows, e));
        CHECK(rows.size() == 2 && rows[0].buy_percent == 40 && rows[0].sell_percent == 120);
        CHECK(rows[0].merchandise == std::vector<std::int32_t>({7, 9}));
        CHECK(rows[1].merchandise.empty());
        CHECK(!parse_merchant_rows(b.data(), b.size() - 1, 4, rows, e));
    }
    const auto items = table();
    CHECK(item_value(items.rows[0]) == 50);
    CHECK(trade_price(50, 40) == 20);
    CHECK(trade_price(1, 40) == 1);          // never below 1
    CHECK(trade_price(3, 100) == 3);
    Shop shop({40, 120, {}});
    shop.add_stock(items, {{0, 1}, {1, 5}});
    CHECK(shop.stocked() && shop.inventory().inventory.size() == 2);
    shop.add_stock(items, {{2, 1}});          // GetNumItems gate: a stocked merchant is not refilled
    CHECK(shop.inventory().inventory.size() == 2);
    CharacterState player; player.gold = 100;
    const auto sword = shop.inventory().inventory[0].instance_id, potion = shop.inventory().inventory[1].instance_id;
    CHECK(shop.buy_price(sword, items) == 60);   // stock uses the Sell multiplier 120%
    std::int32_t price = 0;
    TradeRules rules;
    CHECK(shop.buy(sword, player, items, rules, price) == TradeResult::ok && price == 60);
    CHECK(player.gold == 40 && player.inventory.size() == 1 && player.inventory[0].definition_id == "Sword");
    CHECK(shop.inventory().inventory.size() == 1);
    CHECK(shop.buy(potion, player, items, rules, price) == TradeResult::ok);
    CHECK(shop.buy(potion, player, items, rules, price) == TradeResult::ok);
    CHECK(player.inventory.size() == 2 && player.inventory[1].quantity == 2);  // potions stack
    CHECK(shop.inventory().inventory[0].quantity == 3);
    CHECK(player.gold == 40 - 3 - 3);
    // Failure outcomes change nothing.
    CharacterState poor; poor.gold = 1;
    CHECK(shop.buy(potion, poor, items, rules, price) == TradeResult::insufficient_gold && poor.inventory.empty() && poor.gold == 1);
    rules.potion_capacity = 2;
    CHECK(shop.buy(potion, player, items, rules, price) == TradeResult::potion_full && player.inventory[1].quantity == 2);
    rules.potion_capacity = 0;
    CharacterState full; full.gold = 1000;
    for (std::size_t i = 0; i < kInventoryCapacity; ++i) full.inventory.push_back({"x" + std::to_string(i), "Helm", 1});
    CHECK(shop.buy(potion, full, items, rules, price) == TradeResult::ok);   // a potion may still be added when the bag is full
    shop.add_stock(items, {});
    CHECK(shop.buy("nope", player, items, rules, price) == TradeResult::not_found);
    // Sell: Buy multiplier 40% of value; the item becomes a buyback entry charged at the same multiplier.
    const auto sword_id = player.inventory[0].instance_id;
    player.equipment.push_back({"slot1", sword_id, 0, 1});
    CHECK(shop.sell(sword_id, player, items, price) == TradeResult::equipped && player.inventory.size() == 2);
    player.equipment.clear();
    const auto gold = player.gold;
    CHECK(shop.sell(sword_id, player, items, price) == TradeResult::ok && price == 20 && player.gold == gold + 20);
    CHECK(player.inventory.size() == 1);
    std::string buyback;
    for (const auto& i : shop.inventory().inventory) if (shop.is_buyback(i.instance_id)) buyback = i.instance_id;
    CHECK(!buyback.empty() && shop.buy_price(buyback, items) == 20);
    CHECK(shop.buy(buyback, player, items, rules, price) == TradeResult::ok && price == 20 && player.gold == gold);
    CHECK(shop.sell("missing", player, items, price) == TradeResult::not_found);
    // Selling one potion of a stack keeps the rest.
    std::size_t pi = 0; for (std::size_t i = 0; i < player.inventory.size(); ++i) if (player.inventory[i].definition_id == "Potion0") pi = i;
    const auto before = player.inventory[pi].quantity;
    CHECK(shop.sell(player.inventory[pi].instance_id, player, items, price) == TradeResult::ok && price == 1);
    CHECK(player.inventory[pi].quantity == before - 1);
    if (failures) { std::cerr << failures << " failures\n"; return 1; }
    std::cout << "merchant_shop_v1_tests passed\n";
    return 0;
}
