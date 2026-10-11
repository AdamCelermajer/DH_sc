#include "merchant_shop_ui_v1.hpp"
#include "../../save_store.hpp"
#include <cstdlib>
#include <iostream>
#include <algorithm>

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

int main(int argc,char** argv) {
    // Isolated EXE verification fixture/inspector, never touches default or user save paths.
    if(argc>1) {
        CharacterState state;std::string e;if(!load_character(argv[1],state,e)){std::cerr<<e;return 1;}
        if(argc>2){state.gold=std::stoull(argv[2]);if(!save_character(argv[1],state,e)){std::cerr<<e;return 1;}}
        std::cout<<"profile gold="<<state.gold<<" items="<<state.inventory.size()<<" equipment="<<state.equipment.size()<<'\n';
        for(const auto& i:state.inventory)std::cout<<i.instance_id<<' '<<i.definition_id<<" qty="<<i.quantity<<'\n';return 0;
    }
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
    player.equipment.push_back({"slot1", sword_id, 1, 1}); // TransferItemTo clears both sets.
    const auto gold = player.gold;
    CHECK(shop.sell(sword_id, player, items, price) == TradeResult::ok && price == 20 && player.gold == gold + 20);
    CHECK(player.inventory.size() == 1 && player.equipment.empty());
    CHECK(shop.sell(sword_id, player, items, price) == TradeResult::not_found && player.gold == gold+20); // duplicate stale release
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
    {
        Shop stock({40,120,{}});stock.add_stock(items,{{0,1}});CharacterState fullBag;fullBag.gold=1000;
        for(unsigned i=0;i<100;++i)fullBag.inventory.push_back({"full"+std::to_string(i),"Helm",1});
        const auto id=stock.inventory().inventory[0].instance_id;
        CHECK(stock.buy(id,fullBag,items,{},price)==TradeResult::inventory_full&&fullBag.gold==1000&&stock.inventory().inventory.size()==1);
        fullBag.inventory.clear();CHECK(stock.buy(id,fullBag,items,{},price)==TradeResult::ok);
        const auto paid=fullBag.gold;CHECK(stock.buy(id,fullBag,items,{},price)==TradeResult::not_found&&fullBag.gold==paid);
        stock.add_stock(items,{{0,1}});CHECK(stock.inventory().inventory.size()==1); // source EMPTY-only restock gate
    }
    {
        Shop stock({40,120,{}});stock.add_stock(items,{{0,1}});CharacterState owned;owned.gold=1000;
        ActorState actor;OriginalCombatProperties props;OriginalPropertyDatabase db;std::string e;
        inventory::DetailBindings bindings;
        bindings.symbol=[](const std::string& s,std::string& out,std::string& e){out=s;e.clear();return true;};
        bindings.item_name=[](const InventoryItem& i,const auto&,std::string& out,std::string& e){out=i.definition_id;e.clear();return true;};
        equipment_menu::Options options;options.item_name=bindings.item_name;
        ShopUi ui(owned,stock,items,actor,props,db,options,bindings);CHECK(ui.open(e));
        CHECK(ui.release(270,305,e).kind==ShopUi::Command::Kind::none&&ui.confirmation_visible());
        CHECK(ui.release(340,190,e).kind==ShopUi::Command::Kind::none&&!ui.confirmation_visible()); // cancel
        CHECK(ui.release(270,305,e).kind==ShopUi::Command::Kind::none);
        const auto cmd=ui.release(180,190,e);CHECK(cmd.kind==ShopUi::Command::Kind::buy&&!cmd.instance_id.empty());
        CHECK(ui.release(180,190,e).kind==ShopUi::Command::Kind::none); // duplicate YES cannot trade again
        ui.show_error(TradeResult::insufficient_gold);CHECK(ui.error_visible());
        CHECK(ui.release(270,305,e).kind==ShopUi::Command::Kind::none&&!ui.error_visible()&&!ui.confirmation_visible());
        character_menu::Frame frame;CHECK(ui.frame(960,640,frame,e));
        CHECK(std::any_of(frame.text.begin(),frame.text.end(),[](const auto& t){return t.field.path.find("/player_gold/")!=std::string::npos&&t.value=="1000";}));
        CHECK(ui.release(410,15,e).kind==ShopUi::Command::Kind::none&&ui.mode()==ShopUi::Mode::sell);
        CHECK(ui.release(20,20,e).kind==ShopUi::Command::Kind::close);
    }
    if (failures) { std::cerr << failures << " failures\n"; return 1; }
    std::cout << "merchant_shop_v1_tests passed\n";
    return 0;
}
