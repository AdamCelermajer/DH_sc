#pragma once
// B110: the original MenuMerchant screen. The item list, slot rail, arrows, item details, equipped-item column, total gold and
// the paper-doll pane are the shared Details panel (menu_Merchant "reuses the equipment page layout", sprite 251 vs 456),
// presented over two owners: the player's CharacterState (SELL tab) and the merchant's stock (BUY tab). The merchant-only
// chrome (title, BUY/SELL tabs, action button with VALUE box, back arrow, error popup) is the exported menu_Merchant art.
#include "merchant_art.hpp"
#include "merchant_shop_v1.hpp"
#include "../equipment/equipment_adapter.hpp"
#include "../equipment/equipment_menu.hpp"
#include "../inventory/inventory_details.hpp"
#include <memory>

namespace dh::foundation::merchant {

class ShopUi {
public:
    enum class Mode { buy, sell };
    struct Command {
        enum class Kind { none, close, buy, sell, equip, unequip, buy_potions } kind{Kind::none};
        std::string instance_id;
        unsigned slot{};
    };
    ShopUi(CharacterState& player, Shop& shop, const dh2::data::ItemTable& items, const ActorState& actor,
           const OriginalCombatProperties& properties, const OriginalPropertyDatabase& database,
           equipment_menu::Options options, inventory::DetailBindings bindings);
    ~ShopUi();
    bool open(std::string& error);
    Mode mode() const noexcept { return mode_; }
    bool set_mode(Mode mode, std::string& error);
    // Authored 480x320 pointer position (already inverse-transformed). Back/tabs/pane navigation act here; a Buy/Sell
    // button press returns the command for the caller to execute (it owns equipment/world side effects).
    Command release(float authored_x, float authored_y, std::string& error);
    // Instance selected in the active list; empty when the category has no rows.
    std::string selected_instance(std::string& error) const;
    // Price shown in the VALUE box for the selected row (0 when none).
    std::int32_t selected_price() const;
    // After a transaction: keep the nearest row selected in both lists.
    bool refresh(std::string& error);
    void show_error(TradeResult result) { error_ = result; }
    bool error_visible() const noexcept { return error_ != TradeResult::ok; }
    void dismiss_error() noexcept { error_=TradeResult::ok; }
    bool confirmation_visible() const noexcept { return pending_.kind != Command::Kind::none; }
    void cancel_confirmation() noexcept { pending_ = {}; }
    void potion_status(std::uint32_t held, std::uint32_t capacity) { potions_ = held; potion_capacity_ = capacity; }
    void update_properties(const ActorState& actor, const OriginalCombatProperties& properties) { actor_=actor;properties_=properties; }
    using ParsedText = std::function<bool(const std::string&,const std::vector<std::string>&,std::string&,std::string&)>;
    void parsed_text(ParsedText callback) { parsed_=std::move(callback); }
    bool frame(int width, int height, character_menu::Frame& output, std::string& error) const;
private:
    struct Pane;
    CharacterState& player_;
    Shop& shop_;
    const dh2::data::ItemTable& items_;
    ActorState actor_;
    OriginalCombatProperties properties_;
    inventory::DetailBindings bindings_;
    ParsedText parsed_;
    std::unique_ptr<Pane> buy_, sell_;
    Mode mode_{Mode::buy};
    unsigned slot_{0};
    TradeResult error_{TradeResult::ok};
    Command pending_;
    std::uint32_t potions_{},potion_capacity_{};
    Pane& pane() const { return mode_ == Mode::buy ? *buy_ : *sell_; }
};

}
