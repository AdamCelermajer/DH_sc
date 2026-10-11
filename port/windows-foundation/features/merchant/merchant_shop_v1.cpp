#include "merchant_shop_v1.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>
#include <map>

namespace dh::foundation::merchant {
namespace {
struct Tail {
    const std::uint8_t* d; std::size_t n, at;
    std::uint32_t word() { if (at > n || n - at < 4) throw std::runtime_error("Truncated MerchantTable"); const auto* p = d + at; at += 4; return p[0] | std::uint32_t(p[1]) << 8 | std::uint32_t(p[2]) << 16 | std::uint32_t(p[3]) << 24; }
    std::uint32_t count() { const auto v = word(); if (v > 65536) throw std::runtime_error("MerchantTable count out of range"); return v; }
    std::int32_t integer() { const auto v = word(); std::int32_t o; std::memcpy(&o, &v, 4); return o; }
};
bool stackable(const dh2::data::Item& item) { return item.record.words[7] != 0; }
std::string new_id(const std::vector<InventoryItem>& a, const std::vector<InventoryItem>& b, const char* prefix, const std::string& def, std::uint32_t& counter) {
    for (;;) {
        const auto id = std::string(prefix) + std::to_string(++counter) + "-" + def;
        const auto used = [&](const std::vector<InventoryItem>& v) { return std::any_of(v.begin(), v.end(), [&](const auto& i) { return i.instance_id == id; }); };
        if (!used(a) && !used(b)) return id;
    }
}
}
bool parse_merchant_rows(const std::uint8_t* data, std::size_t size, std::size_t offset, std::vector<MerchantRow>& rows, std::string& error) {
    try {
        Tail t{data, size, offset};
        const auto count = t.count();
        std::vector<MerchantRow> next; next.reserve(count);
        for (unsigned i = 0; i < count; ++i) {
            MerchantRow row; row.buy_percent = t.integer(); row.sell_percent = t.integer();
            const auto n = t.count();
            for (unsigned j = 0; j < n; ++j) { t.integer(); row.merchandise.push_back(t.integer()); }
            next.push_back(std::move(row));
        }
        rows = std::move(next); error.clear(); return true;
    } catch (const std::exception& e) { error = e.what(); return false; }
}
std::int32_t item_value(const dh2::data::Item& item) noexcept {
    const auto v = std::uint32_t(item.record.words[27]) * std::uint32_t(item.record.words[28]);
    std::int32_t o; std::memcpy(&o, &v, 4); return o;
}
std::int32_t trade_price(std::int32_t value, std::int32_t percent) noexcept {
    const float f = float(percent) / 100.0f;
    const auto p = std::int32_t(f * float(value));
    return p < 1 ? 1 : p;
}
bool roll_stock(const MerchantRow& row, const dh2::data::LootTablesV2::Borrow& tables, const dh2::data::LootPowerResourcesV7::Borrow& powers,
                const dh2::data::LootEntryServicesV8& entry, const WithLootRandom& with_rng,
                std::vector<std::pair<std::int32_t, std::uint32_t>>& out, std::string& error) {
    if (!tables || !with_rng) { error = "Merchant stock requires the loot tables and the session loot RNG"; return false; }
    std::vector<std::pair<std::int32_t, std::uint32_t>> next;
    for (const auto loot : row.merchandise) {
        const std::function<bool(dh2::data::LootRandom8V2&, std::string&)> op = [&](dh2::data::LootRandom8V2& rng, std::string& e) {
            dh2::data::LootTableSelectionV8 table(tables, powers, rng, entry);
            std::vector<const dh2::data::LootEntry32V2*> entries;
            if (!table.select(loot, entries, e)) return false;
            dh2::data::LootItemSelectionV8 selection(tables, rng, entry);
            std::vector<dh2::data::LootItemInfoV8> items;
            if (!selection.expand(entries, false, items, e)) return false;
            for (const auto& i : items) if (i.item) next.emplace_back(std::int32_t(i.id), std::uint32_t(i.quantity ? i.quantity : 1));
            return true;
        };
        if (!with_rng(op, error)) return false;
    }
    out = std::move(next); error.clear(); return true;
}
void Shop::add_stock(const dh2::data::ItemTable& items, const std::vector<std::pair<std::int32_t, std::uint32_t>>& list) {
    if (!stock_.inventory.empty()) return;
    for (const auto& [id, quantity] : list) {
        const auto* item = dh2::data::item(items, id);
        if (!item || id < 0 || std::size_t(id) >= items.identifiers.size()) continue;
        const auto& def = items.identifiers[std::size_t(id)];
        auto same = stackable(*item) ? std::find_if(stock_.inventory.begin(), stock_.inventory.end(), [&](const auto& i) { return i.definition_id == def && !is_buyback(i.instance_id); }) : stock_.inventory.end();
        if (same != stock_.inventory.end()) { same->quantity += quantity; continue; }
        stock_.inventory.push_back({new_id(stock_.inventory, {}, "shop-", def, counter_), def, quantity});
    }
    stocked_ = true;
}
bool Shop::is_buyback(const std::string& id) const { return std::find(buyback_.begin(), buyback_.end(), id) != buyback_.end(); }
bool Shop::sort_stock(const dh2::data::ItemTable& items,const std::function<bool(const InventoryItem&,std::string&,std::string&)>& name,std::string& error) {
    std::map<std::string,std::string> names;
    for(const auto& i:stock_.inventory)if(!name(i,names[i.instance_id],error))return false;
    std::stable_sort(stock_.inventory.begin(),stock_.inventory.end(),[&](const auto& a,const auto& b){
        if(is_buyback(a.instance_id)!=is_buyback(b.instance_id))return !is_buyback(a.instance_id); // II_Item_sortbybuyback 0x3fd9f8
        const auto* ai=dh2::data::item(items,dh2::data::item_id(items,a.definition_id));const auto* bi=dh2::data::item(items,dh2::data::item_id(items,b.definition_id));
        const auto av=ai?item_value(*ai):0,bv=bi?item_value(*bi):0;
        return av!=bv?av>bv:names[a.instance_id]<names[b.instance_id]; // II_Item_sortbyvalue then name
    });error.clear();return true;
}
std::int32_t Shop::buy_price(const std::string& id, const dh2::data::ItemTable& items) const {
    const auto it = std::find_if(stock_.inventory.begin(), stock_.inventory.end(), [&](const auto& i) { return i.instance_id == id; });
    if (it == stock_.inventory.end()) return 0;
    const auto* item = dh2::data::item(items, dh2::data::item_id(items, it->definition_id));
    if (!item) return 0;
    // NativeMerchantBuyItem: buyback items cost the Buy multiplier (+4), stock the Sell multiplier (+8).
    return trade_price(item_value(*item), is_buyback(id) ? row_.buy_percent : row_.sell_percent);
}
std::int32_t Shop::sell_price(const std::string& id, const CharacterState& player, const dh2::data::ItemTable& items) const {
    const auto it = std::find_if(player.inventory.begin(), player.inventory.end(), [&](const auto& i) { return i.instance_id == id; });
    if (it == player.inventory.end()) return 0;
    const auto* item = dh2::data::item(items, dh2::data::item_id(items, it->definition_id));
    return item ? trade_price(item_value(*item), row_.buy_percent) : 0;
}
TradeResult Shop::buy(const std::string& id, CharacterState& player, const dh2::data::ItemTable& items, const TradeRules& rules, std::int32_t& price) {
    price = 0;
    const auto it = std::find_if(stock_.inventory.begin(), stock_.inventory.end(), [&](const auto& i) { return i.instance_id == id; });
    if (it == stock_.inventory.end()) return TradeResult::not_found;
    const auto* item = dh2::data::item(items, dh2::data::item_id(items, it->definition_id));
    if (!item) return TradeResult::not_found;
    price = buy_price(id, items);
    if (std::uint64_t(price) > player.gold) return TradeResult::insufficient_gold;
    const bool potion = dh2::data::item_type(*item) == 14;
    const bool full = !rules.infinite_inventory && player.inventory.size() >= kInventoryCapacity;
    if (full && !potion) return TradeResult::inventory_full;
    if (potion && rules.potion_capacity > 0) {
        std::uint64_t held = 0;
        for (const auto& i : player.inventory) { const auto* r = dh2::data::item(items, dh2::data::item_id(items, i.definition_id)); if (r && dh2::data::item_type(*r) == 14) held += i.quantity; }
        if (held >= std::uint64_t(rules.potion_capacity)) return TradeResult::potion_full;
    }
    const std::string def = it->definition_id;
    auto mine = stackable(*item) ? std::find_if(player.inventory.begin(), player.inventory.end(), [&](const auto& i) {
        return i.definition_id == def && std::none_of(player.equipment.begin(), player.equipment.end(), [&](const auto& b) { return b.item_instance_id == i.instance_id; }); }) : player.inventory.end();
    if (mine != player.inventory.end()) mine->quantity += 1;
    else player.inventory.push_back({new_id(player.inventory, stock_.inventory, "merchant-", def, counter_), def, 1});
    player.gold -= std::uint64_t(price);
    if (it->quantity > 1) it->quantity -= 1;
    else { buyback_.erase(std::remove(buyback_.begin(), buyback_.end(), id), buyback_.end()); stock_.inventory.erase(it); }
    return TradeResult::ok;
}
TradeResult Shop::sell(const std::string& id, CharacterState& player, const dh2::data::ItemTable& items, std::int32_t& price) {
    price = 0;
    const auto it = std::find_if(player.inventory.begin(), player.inventory.end(), [&](const auto& i) { return i.instance_id == id; });
    if (it == player.inventory.end()) return TradeResult::not_found;
    const auto* item = dh2::data::item(items, dh2::data::item_id(items, it->definition_id));
    if (!item) return TradeResult::not_found;
    price = sell_price(id, player, items);
    const std::string def = it->definition_id;
    // The merchant keeps the item as a buyback entry (Item+105); stackables merge with an existing buyback stack.
    auto mine = stackable(*item) ? std::find_if(stock_.inventory.begin(), stock_.inventory.end(), [&](const auto& i) { return i.definition_id == def && is_buyback(i.instance_id); }) : stock_.inventory.end();
    if (mine != stock_.inventory.end()) mine->quantity += 1;
    else { const auto sid = new_id(stock_.inventory, player.inventory, "shop-", def, counter_); stock_.inventory.push_back({sid, def, 1}); buyback_.push_back(sid); }
    player.gold += std::uint64_t(price);
    if (it->quantity > 1) it->quantity -= 1;
    else {
        player.equipment.erase(std::remove_if(player.equipment.begin(),player.equipment.end(),[&](const auto& b){return b.item_instance_id==id;}),player.equipment.end());
        player.inventory.erase(it);
    }
    return TradeResult::ok;
}
}
