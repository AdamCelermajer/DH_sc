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

static dh2::data::ItemTable navigation_table() {
    dh2::data::ItemTable t;
    for (unsigned i = 0; i < 8; ++i) {
        t.identifiers.push_back("Row" + std::to_string(i));
        t.rows.emplace_back();
        auto& record = t.rows.back().record;
        record.words[17] = 17;
        record.words[22] = 4;
        record.words[26] = 1; // all eight rows belong to the same weapon category
        record.words[27] = 1;
        record.words[28] = i + 1;
    }
    return t;
}

static std::pair<float,float> first_row_center() {
    const auto& rows = inventory::original_inventory_details().rows;
    float x0=0,y0=0,x1=0,y1=0;bool any=false;
    const auto include=[&](const character_menu::MenuArt& art) {
        for(const auto& batch:art.batches)for(const auto& v:batch.triangles) {
            if(!any){x0=x1=v.x;y0=y1=v.y;any=true;}
            else {x0=std::min(x0,v.x);x1=std::max(x1,v.x);y0=std::min(y0,v.y);y1=std::max(y1,v.y);}
        }
    };
    if(!rows.empty()){include(rows.front().unselected);include(rows.front().selected);}
    return any?std::make_pair((x0+x1)*.5f,(y0+y1)*.5f):std::make_pair(-1.0f,-1.0f);
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
    {
        Shop generated({40,120,{}});
        const std::vector<InventoryItem> occurrences{{"roll-a","Potion0",1},{"roll-b","Potion0",1}};
        generated.add_generated_stock(items,occurrences);
        CHECK(generated.inventory().inventory.size()==2&&generated.inventory().inventory[0].quantity==1&&generated.inventory().inventory[1].quantity==1);
        CharacterState buyer;buyer.gold=100;buyer.inventory.push_back({"existing-potion","Potion0",2});
        TradeRules generatedRules;std::int32_t generatedPrice=0;
        const auto first=generated.inventory().inventory.front().instance_id;
        CHECK(generated.buy(first,buyer,items,generatedRules,generatedPrice)==TradeResult::ok);
        CHECK(buyer.inventory.size()==1&&buyer.inventory.front().quantity==3&&generated.inventory().inventory.size()==1);
    }
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
        CHECK(ui.release(270,305,e).kind==ShopUi::Command::Kind::none&&ui.error_visible()&&!ui.confirmation_visible());
        CHECK(ui.release(235,208,e).kind==ShopUi::Command::Kind::none&&!ui.error_visible()); // visible source OK centre, independent of exported contour
        character_menu::Frame frame;CHECK(ui.frame(960,640,frame,e));
        CHECK(std::any_of(frame.text.begin(),frame.text.end(),[](const auto& t){return t.field.path.find("/player_gold/")!=std::string::npos&&t.value=="1000";}));
        const auto sell_tab=ui.release(410,15,e);
        CHECK(sell_tab.kind==ShopUi::Command::Kind::select_mode&&ui.mode()==ShopUi::Mode::buy);
        CHECK(ui.set_mode(sell_tab.mode,e)&&ui.mode()==ShopUi::Mode::sell);
        CHECK(ui.release(20,20,e).kind==ShopUi::Command::Kind::close);
    }
    // B124: the PC adapter steps the source-owned row selection. The existing
    // presenter remains the only scroll/focus state and clamps at list bounds.
    {
        auto t=navigation_table();Shop stock({40,120,{}});
        std::vector<std::pair<std::int32_t,std::uint32_t>> merchandise;
        for(unsigned i=0;i<8;++i)merchandise.emplace_back(i,1);
        stock.add_stock(t,merchandise);CharacterState owned;owned.gold=1000;
        ActorState actor;OriginalCombatProperties props;OriginalPropertyDatabase db;std::string e;
        inventory::DetailBindings bindings;
        bindings.symbol=[](const std::string& s,std::string& out,std::string& e){out=s;e.clear();return true;};
        bindings.item_name=[](const InventoryItem& i,const auto&,std::string& out,std::string& e){out=i.definition_id;e.clear();return true;};
        equipment_menu::Options options;options.item_name=bindings.item_name;
        ShopUi ui(owned,stock,t,actor,props,db,options,bindings);CHECK(ui.open(e));
        inventory::DetailRailBox box;CHECK(inventory::details_rail_box(inventory::original_inventory_details(),1,box));
        ui.release((box.x0+box.x1)/2,(box.y0+box.y1)/2,e);
        const auto id=[&](unsigned i){return stock.inventory().inventory[i].instance_id;};
        CHECK(ui.selected_instance(e)==id(0));
        const auto [cx,cy]=first_row_center();CHECK(cx>=0&&cy>=0);
        CHECK(ui.list_contains(cx,cy));CHECK(!ui.list_contains(20,20));
        CHECK(ui.step_rows(-1,e)&&ui.selected_instance(e)==id(0)); // first-row boundary
        CHECK(ui.step_rows(1,e)&&ui.selected_instance(e)==id(1));
        character_menu::Frame frame;CHECK(ui.frame(1774,1091,frame,e));
        CHECK(std::any_of(frame.text.begin(),frame.text.end(),[](const auto& value){return value.value=="Row1";}));
        // Rapid, distinct row steps stay tied to the presented instance and
        // the details frame follows the same selection immediately.
        for(unsigned i=2;i<=5;++i)CHECK(ui.step_rows(1,e)&&ui.selected_instance(e)==id(i));
        CHECK(ui.frame(960,640,frame,e));
        CHECK(std::any_of(frame.text.begin(),frame.text.end(),[](const auto& value){return value.value=="Row5";}));
        CHECK(ui.step_rows(0,e)&&ui.selected_instance(e)==id(5)); // duplicate/no-op transport event
        CHECK(ui.step_rows(100,e)&&ui.selected_instance(e)==id(7)); // large/page movement clamps at last row
        CHECK(ui.step_rows(1,e)&&ui.selected_instance(e)==id(7));
        CHECK(ui.step_rows(-100,e)&&ui.selected_instance(e)==id(0));
        CHECK(ui.release(270,305,e).kind==ShopUi::Command::Kind::none&&ui.confirmation_visible());
        CHECK(!ui.list_contains(cx,cy));CHECK(ui.step_rows(1,e)&&ui.selected_instance(e)==id(0)); // modal owns input
        ui.cancel_confirmation();ui.show_error(TradeResult::insufficient_gold);
        CHECK(!ui.list_contains(cx,cy));CHECK(ui.step_rows(1,e)&&ui.selected_instance(e)==id(0));
        ui.dismiss_error();

        CHECK(inventory::details_rail_box(inventory::original_inventory_details(),9,box));
        ui.release((box.x0+box.x1)/2,(box.y0+box.y1)/2,e);
        CHECK(ui.selected_instance(e).empty()); // empty category
        CHECK(ui.step_rows(1,e)&&ui.selected_instance(e).empty());
        CHECK(!ui.list_contains(-100,-100));
        // A category switch reopens the source list at its authored initial
        // selection, rather than carrying a stale row index across lists.
        CHECK(inventory::details_rail_box(inventory::original_inventory_details(),1,box));
        ui.release((box.x0+box.x1)/2,(box.y0+box.y1)/2,e);
        CHECK(ui.selected_instance(e)==id(0));
        CHECK(ui.step_rows(3,e)&&ui.selected_instance(e)==id(3));
        CHECK(ui.release(20,20,e).kind==ShopUi::Command::Kind::close);
        CHECK(ui.open(e)&&ui.selected_instance(e)==id(0)); // reopen resets to first source row

        Shop one_stock({40,120,{}});one_stock.add_stock(t,{{0,1}});ShopUi one(owned,one_stock,t,actor,props,db,options,bindings);
        CHECK(one.open(e));CHECK(inventory::details_rail_box(inventory::original_inventory_details(),1,box));
        one.release((box.x0+box.x1)/2,(box.y0+box.y1)/2,e);const auto only=one.selected_instance(e);
        CHECK(!only.empty()&&one.step_rows(5,e)&&one.selected_instance(e)==only); // single-row category
    }
    // Authored admission is distinct from native defensive transfer behavior.
    {
        inventory::DetailBindings bindings;
        bindings.symbol=[](const std::string& s,std::string& out,std::string& e){out=s;e.clear();return true;};
        bindings.item_name=[](const InventoryItem& i,const auto&,std::string& out,std::string& e){out=i.definition_id;e.clear();return true;};
        equipment_menu::Options options;options.item_name=bindings.item_name;
        ActorState actor;OriginalCombatProperties props;OriginalPropertyDatabase db;std::string e;
        const auto category=[&](ShopUi& ui,unsigned slot){
            inventory::DetailRailBox box;CHECK(inventory::details_rail_box(inventory::original_inventory_details(),slot,box));
            ui.release((box.x0+box.x1)/2,(box.y0+box.y1)/2,e);
        };
        const auto action_disabled=[&](ShopUi& ui){character_menu::Frame f;CHECK(ui.frame(960,640,f,e));
            return std::any_of(f.art.batches.begin(),f.art.batches.end(),[](const auto& b){return b.role=="menu_Merchant/btn_Action/1"&&b.shape_id==198;});};
        struct Case{unsigned slot,bound;int set,slotting;bool blocked;};
        for(const auto c:{Case{0,0,0,0,true},Case{1,2,0,-3,true},Case{5,6,0,-2,true},Case{1,2,1,-3,false},Case{0,0,1,0,false}}){
            auto t=table();t.rows[0].record.words[26]=c.slotting;
            Shop stock({40,120,{}});stock.add_stock(t,{{0,1}});CharacterState owned;owned.gold=1000;
            owned.inventory.push_back({"owned","Sword",1});owned.equipment.push_back({"slot"+std::to_string(c.bound),"owned",c.set,int(c.bound)});
            ShopUi ui(owned,stock,t,actor,props,db,options,bindings);CHECK(ui.open(e));CHECK(ui.set_mode(ShopUi::Mode::sell,e));category(ui,c.slot);
            CHECK(ui.selected_instance(e)=="owned");
            CHECK(action_disabled(ui)==c.blocked);
            const auto command=ui.release(270,305,e);
            CHECK(command.kind==(c.blocked?ShopUi::Command::Kind::none:ShopUi::Command::Kind::sell));
            CHECK(!ui.confirmation_visible()&&!ui.error_visible()&&owned.gold==1000&&owned.inventory.size()==1&&stock.inventory().inventory.size()==1);
            if(c.slot==0&&c.set==0){
                CHECK(ui.release(400,45,e).kind==ShopUi::Command::Kind::unequip);
                owned.equipment.clear();CHECK(ui.refresh(e));CHECK(!action_disabled(ui));
                CHECK(ui.release(270,305,e).kind==ShopUi::Command::Kind::sell);
            }
        }
        auto t=table();t.rows[1].record.words[26]=-1;
        Shop stock({40,120,{}});stock.add_stock(t,{{1,20}});CharacterState owned;owned.gold=1000;owned.inventory.push_back({"held","Potion0",12});
        ShopUi ui(owned,stock,t,actor,props,db,options,bindings);CHECK(ui.open(e));category(ui,9);ui.potion_status(12,12);
        CHECK(action_disabled(ui));
        for(unsigned i=0;i<2;++i){CHECK(ui.release(270,305,e).kind==ShopUi::Command::Kind::none);CHECK(ui.release(410,305,e).kind==ShopUi::Command::Kind::none);}
        CHECK(!ui.confirmation_visible()&&!ui.error_visible()&&owned.gold==1000&&owned.inventory[0].quantity==12&&stock.inventory().inventory[0].quantity==20);
        ui.potion_status(11,12);CHECK(!action_disabled(ui));
        CHECK(ui.release(270,305,e).kind==ShopUi::Command::Kind::none&&ui.confirmation_visible());ui.cancel_confirmation();
        CHECK(ui.release(410,305,e).kind==ShopUi::Command::Kind::none&&ui.confirmation_visible());
        CHECK(ui.release(180,190,e).kind==ShopUi::Command::Kind::buy_potions);
    }
    if (failures) { std::cerr << failures << " failures\n"; return 1; }
    std::cout << "merchant_shop_v1_tests passed\n";
    return 0;
}
