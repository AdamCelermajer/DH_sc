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
// Details-panel controls that do not exist on the merchant screen (Drop/Transmute/Auto-equip) and the EQUIP/UNEQUIP column
// buttons (not ported: the merchant screen shows only the pane the player trades from).
bool merchant_hides(const std::string& role) {
    static const char* const hidden[] = {
        "menu_InventorySheetDetails/btn_Drop/", "menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/",
        "menu_InventorySheetDetails/btn_AutoEquip/", "menu_InventorySheetDetails/btn_EquipItem/",
        "menu_InventorySheetDetails/btn_Unequip/"};
    if (role.size() >= 17 && role.compare(role.size() - 17, 17, "/b056b_value_icon") == 0) return false; // keep the VALUE coin icon
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
    if (error_ != TradeResult::ok) { error_ = TradeResult::ok; return command; }   // MerchantErrorPopup btn_deadzone swallows the click
    const auto& art = merchant_art();
    if (inside(art.hits[4], x, y)) { command.kind = Command::Kind::close; return command; }
    if (inside(art.hits[0], x, y)) { if (mode_ != Mode::buy) set_mode(Mode::buy, error); return command; }
    if (inside(art.hits[1], x, y)) { if (mode_ != Mode::sell) set_mode(Mode::sell, error); return command; }
    if (inside(art.hits[2], x, y)) {
        const auto id = selected_instance(error);
        if (!id.empty()) { command.kind = mode_ == Mode::buy ? Command::Kind::buy : Command::Kind::sell; command.instance_id = id; }
        return command;
    }
    inventory::DetailAction action{};
    if (!pane().details->release(x, y, action, error)) return command;
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
    f.art.batches.erase(std::remove_if(f.art.batches.begin(), f.art.batches.end(), [](const auto& b) { return merchant_hides(b.role); }), f.art.batches.end());
    f.text.erase(std::remove_if(f.text.begin(), f.text.end(), [](const auto& t) { return merchant_hides(t.field.path); }), f.text.end());
    f.solids.erase(std::remove_if(f.solids.begin(), f.solids.end(), [](const auto& s) { return merchant_hides(s.geometry.role); }), f.solids.end());
    const auto& art = merchant_art();
    const bool buy = mode_ == Mode::buy;
    std::string sell_error;
    const auto symbol = [&](const char* name, std::string& out) { return bindings_.symbol && bindings_.symbol(name, out, sell_error); };
    const auto value = [&](const std::string& path, std::string& out) -> bool {
        if (path.find("/MENU_title/") != std::string::npos) return symbol("GAMEPLAYMENUS_MERCHANT_TITLE", out);
        if (path.find("/btn_Buy/") != std::string::npos) return symbol("GAMEPLAYMENUS_BUY", out);
        if (path.find("/btn_Sell/") != std::string::npos) return symbol("GAMEPLAYMENUS_SELL", out);
        if (path.find("/ButtonName/") != std::string::npos) return symbol(buy ? "GAMEPLAYMENUS_BUY" : "GAMEPLAYMENUS_SELL", out);
        if (path.find("/ValueText/") != std::string::npos) return symbol("MENU_VALUE_TITLE", out);
        if (path.find("/ValueBox/") != std::string::npos) { const auto p = selected_price(); if (p > 0) out = std::to_string(p); return true; }
        if (path.find("/confirm_msg/") != std::string::npos) {
            const char* name = error_ == TradeResult::insufficient_gold ? "GAMEPLAYMENUS_TRANSACTION_INSUFFICIENT_GOLD"
                             : error_ == TradeResult::inventory_full ? "GAMEPLAYMENUS_SHOP_INVENTORY_FULL" : "GAMEPLAYMENUS_SHOP_POTION_FULL";
            return symbol(name, out);
        }
        return false;
    };
    append(f, art.title, value);
    append(f, buy ? art.tab_buy_on : art.tab_buy_idle, value);
    append(f, buy ? art.tab_sell_idle : art.tab_sell_on, value);
    std::string e2;
    append(f, selected_instance(e2).empty() ? art.action_disabled : art.action_idle, value);
    append(f, art.back, value);
    if (error_visible()) append(f, art.error, value);
    output = std::move(f); error.clear(); return true;
}

}
