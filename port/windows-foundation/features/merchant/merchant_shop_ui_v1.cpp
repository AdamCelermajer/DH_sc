#include "merchant_shop_ui_v1.hpp"
#include <algorithm>
#include <cmath>

namespace dh::foundation::merchant {

struct ShopUi::Pane {
    std::unique_ptr<EquipmentAdapter> adapter;   // never mutates: the merchant UI only reads selection/candidates
    std::unique_ptr<equipment_menu::Presenter> selection;
    std::unique_ptr<inventory::DetailsPresenter> details;
    Pane(CharacterState& owner, ActorState& actor, const OriginalCombatProperties& props, const dh2::data::ItemTable& items,
         const OriginalPropertyDatabase& database, const equipment_menu::Options& options)
        : adapter(std::make_unique<EquipmentAdapter>(owner, actor, props, items, database, EquipmentAdapterOptions{})),
          selection(std::make_unique<equipment_menu::Presenter>(owner, items, props.sheets, *adapter, options)),
          details(std::make_unique<inventory::DetailsPresenter>(owner, items, *selection)) {}
};

namespace {
bool starts(const std::string& s, const char* p) { return s.compare(0, std::char_traits<char>::length(p), p) == 0; }
bool inside(const std::vector<float>& pairs, float x, float y) {
    const auto edge = [](float ax, float ay, float bx, float by, float px, float py) { return (bx - ax) * (py - ay) - (by - ay) * (px - ax); };
    for (std::size_t i = 0; i + 5 < pairs.size(); i += 6) {
        const float* p = &pairs[i];
        if (std::abs(edge(p[0], p[1], p[2], p[3], p[4], p[5])) < 1e-6f) continue;
        const float a = edge(p[0], p[1], p[2], p[3], x, y), b = edge(p[2], p[3], p[4], p[5], x, y), c = edge(p[4], p[5], p[0], p[1], x, y);
        if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0)) return true;
    }
    return false;
}
// Details controls absent from the authored merchant screen.
bool merchant_hides(const std::string& role) {
    static const char* const hidden[] = {
        "menu_InventorySheetDetails/btn_Drop/", "menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/",
        "menu_InventorySheetDetails/btn_AutoEquip/", "menu_InventorySheetDetails/237/", "menu_InventorySheetDetails/category_title/"};
    return std::any_of(std::begin(hidden), std::end(hidden), [&](const char* h) { return starts(role, h); });
}
void append(character_menu::Frame& f, const character_menu::MenuArt& art, const std::function<bool(const std::string&, std::string&)>& value) {
    f.art.batches.insert(f.art.batches.end(), art.batches.begin(), art.batches.end());
    f.solids.insert(f.solids.end(), art.solids.begin(), art.solids.end());
    for (const auto& field : art.text_fields) { std::string text; if (value(field.path, text) && !text.empty()) f.text.push_back({field, std::move(text)}); }
}
}

ShopUi::ShopUi(CharacterState& player, Shop& shop, const dh2::data::ItemTable& items, const ActorState& actor,
               const OriginalCombatProperties& properties, const OriginalPropertyDatabase& database,
               equipment_menu::Options options, inventory::DetailBindings bindings)
    : player_(player), shop_(shop), items_(items), actor_(actor), properties_(properties), bindings_(std::move(bindings)) {
    buy_ = std::make_unique<Pane>(shop.inventory(), actor_, properties_, items, database, options);
    sell_ = std::make_unique<Pane>(player, actor_, properties_, items, database, options);
}
ShopUi::~ShopUi() = default;

bool ShopUi::open(std::string& error) {
    error_ = TradeResult::ok;
    pending_ = {};
    return buy_->details->open(slot_, error) && sell_->details->open(slot_, error) && (pane().details->open(slot_, error));
}
bool ShopUi::set_mode(Mode mode, std::string& error) {
    mode_ = mode; error_ = TradeResult::ok;
    return pane().details->open(slot_, error);
}
std::string ShopUi::selected_instance(std::string& error) const {
    std::vector<equipment_menu::OwnedSelection> rows;
    if (!pane().selection->view_for_selected_slot(rows, error) || rows.empty()) return {};
    return rows[std::min(pane().details->selected_index(), rows.size() - 1)].instance_id;
}
std::int32_t ShopUi::selected_price() const {
    std::string e; const auto id = selected_instance(e);
    if (id.empty()) return 0;
    return mode_ == Mode::buy ? shop_.buy_price(id, items_) : shop_.sell_price(id, player_, items_);
}
bool ShopUi::list_contains(float x,float y) const {
    if(error_visible()||confirmation_visible())return false;
    const auto& art=inventory::original_inventory_details();
    return std::any_of(art.rows.begin(),art.rows.end(),[&](const auto& row){return inventory::details_row_hit(row,x,y);});
}
bool ShopUi::step_rows(int delta,std::string& error) {
    error.clear();if(error_visible()||confirmation_visible()||delta==0)return true;
    std::vector<equipment_menu::OwnedSelection> rows;
    if(!pane().selection->view_for_selected_slot(rows,error))return false;
    if(rows.empty())return true;
    const auto current=std::min(pane().details->selected_index(),rows.size()-1);std::size_t target=current;
    if(delta<0){const auto amount=static_cast<std::uint64_t>(-static_cast<std::int64_t>(delta));target=amount>current?0:current-static_cast<std::size_t>(amount);}
    else{const auto amount=static_cast<std::uint64_t>(delta);const auto remaining=rows.size()-1-current;target=amount>remaining?rows.size()-1:current+static_cast<std::size_t>(amount);}
    if(target!=current&&!pane().selection->select_instance(rows[target].instance_id,error))return false;
    error.clear();return true;
}
bool ShopUi::action_available(std::string& error) const {
    const auto id = selected_instance(error);
    if (id.empty()) return false;
    if (mode_ == Mode::buy)
        return std::uint64_t(selected_price()) <= player_.gold && !(slot_ == 9 && potions_ == potion_capacity_);
    // NativeInvGetItemsListForSlot exposes the active selected slot and paired
    // hand/ring, rather than treating an inactive equipment set as equipped.
    const unsigned paired = slot_ == 1 ? 2 : slot_ == 2 ? 1 : slot_ == 5 ? 6 : slot_ == 6 ? 5 : slot_;
    const auto bound_to = [](const EquipmentBinding& b, unsigned slot) {
        return b.source_slot >= 0 ? b.source_slot == int(slot) : b.slot == "slot" + std::to_string(slot);
    };
    return std::none_of(player_.equipment.begin(), player_.equipment.end(), [&](const auto& b) {
        return b.item_instance_id == id && b.equipment_set <= 0 && (bound_to(b, slot_) || bound_to(b, paired));
    });
}
bool ShopUi::refresh(std::string& error) {
    for (auto* p : {buy_.get(), sell_.get()}) {
        const auto index = p->details->selected_index();
        if (!p->details->reselect_near(index, error)) return false;
    }
    return true;
}
ShopUi::Command ShopUi::release(float x, float y, std::string& error) {
    Command command;
    error.clear();
    const auto& art = merchant_art();
    if(error_visible()){if(inside(art.error_ok_hit,x,y))dismiss_error();return command;} // deadzone swallows background; only btn_Ok hides
    if (confirmation_visible()) {
        if(inside(art.confirm_hits[0],x,y)){command=pending_;pending_={};}
        else if(inside(art.confirm_hits[1],x,y))pending_={};
        return command;
    }
    if (inside(art.hits[4], x, y)) { command.kind = Command::Kind::close; return command; }
    if (inside(art.hits[0], x, y)) { command.kind=Command::Kind::select_mode; command.mode=Mode::buy; return command; }
    if (inside(art.hits[1], x, y)) { command.kind=Command::Kind::select_mode; command.mode=Mode::sell; return command; }
    if (inside(art.hits[2], x, y)) {
        if(!action_available(error))return command;
        const auto id = selected_instance(error);
        if(!id.empty()) {
            if(mode_==Mode::buy){pending_.kind=Command::Kind::buy;pending_.instance_id=id;}
            else {command.kind=Command::Kind::sell;command.instance_id=id;}
        }
        return command;
    }
    if(mode_==Mode::buy&&slot_==9&&potions_<potion_capacity_&&
       std::uint64_t(selected_price())*std::uint64_t(potion_capacity_-potions_)<=player_.gold&&inside(art.hits[3],x,y)) {
        const auto id=selected_instance(error);
        const auto at=std::find_if(shop_.inventory().inventory.begin(),shop_.inventory().inventory.end(),[&](const auto& i){return i.instance_id==id;});
        const auto* item=at==shop_.inventory().inventory.end()?nullptr:dh2::data::item(items_,dh2::data::item_id(items_,at->definition_id));
        if(item&&dh2::data::item_type(*item)==14){pending_.kind=Command::Kind::buy_potions;pending_.instance_id=id;}
        return command;
    }
    inventory::DetailAction action{};
    if (!pane().details->release(x, y, action, error)) return command;
    if(action==inventory::DetailAction::equip&&mode_==Mode::sell&&slot_<9){command.kind=Command::Kind::equip;command.instance_id=selected_instance(error);command.slot=slot_;}
    if(action==inventory::DetailAction::unequip&&slot_<9){command.kind=Command::Kind::unequip;command.slot=slot_;}
    if (action == inventory::DetailAction::slot || action == inventory::DetailAction::previous || action == inventory::DetailAction::next)
        slot_ = pane().selection->selected_slot();
    // Keep both lists on the same category (the slot rail and arrows act on the visible one).
    if (slot_ <= 9) { Pane& other = mode_ == Mode::buy ? *sell_ : *buy_; if (other.selection->selected_slot() != slot_) other.details->open(slot_, error); }
    return command;
}
bool ShopUi::frame(int width, int height, character_menu::Frame& output, std::string& error) const {
    character_menu::Frame f;
    if (!character_menu::Presenter::viewport(width, height, f.transform, error)) return false;
    if (!pane().details->frame(bindings_, f, error)) return false;
    const bool buy = mode_ == Mode::buy;
    const auto upper=[](const std::string& p){return p.find("/EquipedItem")!=std::string::npos||p.find("/EquipedSwordIcon/")!=std::string::npos||p.find("/btn_Unequip/")!=std::string::npos;};
    // BUY's comparison column is the player's equipment, never the merchant's inventory.
    if(buy){
        character_menu::Frame owned;if(!sell_->details->frame(bindings_,owned,error))return false;
        f.art.batches.erase(std::remove_if(f.art.batches.begin(),f.art.batches.end(),[&](const auto& b){return upper(b.role)||b.role.find("/btn_EquipItem/")!=std::string::npos;}),f.art.batches.end());
        f.text.erase(std::remove_if(f.text.begin(),f.text.end(),[&](const auto& t){return upper(t.field.path)||t.field.path.find("/btn_EquipItem/")!=std::string::npos;}),f.text.end());
        for(const auto& b:owned.art.batches)if(upper(b.role))f.art.batches.push_back(b);
        for(const auto& t:owned.text)if(upper(t.field.path))f.text.push_back(t);
    }
    for(auto& t:f.text)if(t.field.path.find("/player_gold/")!=std::string::npos)t.value=std::to_string(player_.gold);
    const bool has_equipped=slot_<9&&std::any_of(player_.equipment.begin(),player_.equipment.end(),[&](const auto& b){return b.source_slot==int(slot_)&&b.equipment_set<=0;});
    std::string selected_error;const bool has_selected=!selected_instance(selected_error).empty();
    const auto unavailable=[&](const std::string& p){return (!has_equipped&&p.find("/btn_Unequip/")!=std::string::npos)||((buy||slot_==9||!has_selected)&&p.find("/btn_EquipItem/")!=std::string::npos);};
    f.art.batches.erase(std::remove_if(f.art.batches.begin(),f.art.batches.end(),[&](const auto& b){return unavailable(b.role);}),f.art.batches.end());
    f.text.erase(std::remove_if(f.text.begin(),f.text.end(),[&](const auto& t){return unavailable(t.field.path);}),f.text.end());
    f.solids.erase(std::remove_if(f.solids.begin(),f.solids.end(),[&](const auto& s){return unavailable(s.geometry.role);}),f.solids.end());
    std::vector<HudGeometryBatch> value_icon;
    for(const auto& b:f.art.batches)if(b.role.find("/b056b_value_icon")!=std::string::npos)value_icon.push_back(b);
    f.art.batches.erase(std::remove_if(f.art.batches.begin(), f.art.batches.end(), [](const auto& b) { return merchant_hides(b.role); }), f.art.batches.end());
    f.text.erase(std::remove_if(f.text.begin(), f.text.end(), [](const auto& t) { return merchant_hides(t.field.path); }), f.text.end());
    f.solids.erase(std::remove_if(f.solids.begin(), f.solids.end(), [](const auto& s) { return merchant_hides(s.geometry.role); }), f.solids.end());
    const auto& art = merchant_art();
    std::string sell_error;
    const auto symbol = [&](const char* name, std::string& out) { return bindings_.symbol && bindings_.symbol(name, out, sell_error); };
    const auto value = [&](const std::string& path, std::string& out) -> bool {
        if (path.find("/MENU_title/") != std::string::npos) return symbol("GAMEPLAYMENUS_MERCHANT_TITLE", out);
        if(path.find("/category_title/")!=std::string::npos)return bindings_.symbol("GAMEPLAYMENUS_CATEGORY_"+std::to_string(slot_),out,sell_error);
        if (path.find("/btn_Buy/") != std::string::npos) return symbol("GAMEPLAYMENUS_BUY", out);
        if (path.find("/btn_Sell/") != std::string::npos) return symbol("GAMEPLAYMENUS_SELL", out);
        if(path.find("/btn_BuyPotions/")!=std::string::npos){
            if(path.find("/ButtonName/")!=std::string::npos)return symbol("GAMEPLAYMENUS_BUY_MAXIMUM",out);
            if(path.find("/ValueText/")!=std::string::npos)return symbol("GAMEPLAYMENUS_YOUR_POTIONS",out);
            if(path.find("/ValueBox/")!=std::string::npos){out=std::to_string(potions_)+" / "+std::to_string(potion_capacity_);return true;}
        }
        if (path.find("/ButtonName/") != std::string::npos) return symbol(buy ? "GAMEPLAYMENUS_BUY" : "GAMEPLAYMENUS_SELL", out);
        if (path.find("/ValueText/") != std::string::npos) return symbol("MENU_VALUE_TITLE", out);
        if (path.find("/ValueBox/") != std::string::npos) { const auto p = selected_price(); if (p > 0) out = std::to_string(p); return true; }
        if (path.find("/confirm_msg/") != std::string::npos) {
            const char* name = error_ == TradeResult::insufficient_gold ? "GAMEPLAYMENUS_TRANSACTION_INSUFFICIENT_GOLD"
                             : error_ == TradeResult::inventory_full ? "GAMEPLAYMENUS_SHOP_INVENTORY_FULL" : "GAMEPLAYMENUS_SHOP_POTION_FULL";
            if(error_==TradeResult::insufficient_gold&&parsed_){std::string e,id=selected_instance(e),title;for(const auto& i:shop_.inventory().inventory)if(i.instance_id==id){const auto* item=dh2::data::item(items_,dh2::data::item_id(items_,i.definition_id));if(item&&bindings_.item_name)bindings_.item_name(i,*item,title,e);}return parsed_(name,{title},out,sell_error);}
            return symbol(name, out);
        }
        if(path.find("/btn_Ok/")!=std::string::npos)return symbol("GAMEPLAYMENUS_TRANSMUTE_CONFIRM",out);
        return false;
    };
    append(f, art.title, value);
    append(f, art.category, value);
    append(f, buy ? art.tab_buy_on : art.tab_buy_idle, value);
    append(f, buy ? art.tab_sell_idle : art.tab_sell_on, value);
    const bool action_enabled = action_available(error);
    if (!error.empty()) return false;
    append(f, action_enabled ? art.action_idle : art.action_disabled, value);
    f.art.batches.insert(f.art.batches.end(),value_icon.begin(),value_icon.end());
    append(f, art.back, value);
    if(buy&&slot_==9)append(f,potions_>=potion_capacity_||std::uint64_t(selected_price())*std::uint64_t(potion_capacity_-std::min(potions_,potion_capacity_))>player_.gold?art.potions_disabled:art.potions,value);
    if (error_visible()) append(f, art.error, value);
    if(confirmation_visible())append(f,art.confirm,[&](const std::string& path,std::string& out){
        if(path.find("/confirm_msg/")==std::string::npos)return false;
        std::string label;symbol(pending_.kind==Command::Kind::sell?"GAMEPLAYMENUS_SELL":"GAMEPLAYMENUS_BUY",label);
        std::string e;const auto id=pending_.instance_id;const auto& inv=buy?shop_.inventory().inventory:player_.inventory;
        const auto at=std::find_if(inv.begin(),inv.end(),[&](const auto& i){return i.instance_id==id;});
        std::string name;const auto* item=at==inv.end()?nullptr:dh2::data::item(items_,dh2::data::item_id(items_,at->definition_id));
        if(item&&bindings_.item_name)bindings_.item_name(*at,*item,name,e);
        auto price=selected_price();if(pending_.kind==Command::Kind::buy_potions)price*=std::int32_t(potion_capacity_-std::min(potions_,potion_capacity_));
        if(parsed_)return parsed_("GAMEPLAYMENUS_BUY_QUESTION",{name,std::to_string(price)},out,sell_error);
        out=label+" "+name+" ("+std::to_string(price)+")?";return true;
    });
    if(!sell_error.empty()){error=sell_error;return false;}
    output = std::move(f); error.clear(); return true;
}

}
