#pragma once
#include "../../character_state.hpp"
#include "../../../game-data/items.hpp"

namespace dh::foundation::inventory {
enum class Sort { owned_order, definition, name, type };
struct Row {
    // name_key is legacy raw Item.name/ModularModule URI, not displayable text.
    // UI must use name_text_oid or the actual ItemInstance name provider.
    std::string instance_id, definition_id, name_key, icon_name;
    std::uint32_t quantity{};
    std::int32_t type{};
    bool stackable{}, equipped{}, selected{};
    // _UpdateName uses source word17 textOID; Item.name is ModularModule URI.
    std::int32_t name_text_oid{};
};
struct View { std::uint64_t gold{}; std::vector<Row> rows; };
// Borrows the single save owner and source table. Only selection/sort are local.
// Each view is rebuilt, so neither pointers nor indices survive owner mutations.
class Presenter {
    CharacterState& owner_;
    const dh2::data::ItemTable& table_;
    std::string selected_;
    Sort sort_{Sort::owned_order};
public:
    Presenter(CharacterState& owner, const dh2::data::ItemTable& table):owner_(owner),table_(table){}
    void sort(Sort value) noexcept { sort_=value; }
    bool select(const std::string& instance_id, std::string& error);
    bool present(View& output, std::string& error);
    // Bare-item foundation storage operations, not original ItemInstance creation.
    // Caller supplies genuine resolved definitions and unique instance identities.
    // Gold items require an external valuation service and are rejected here.
    bool pickup(const InventoryItem& resolved_item, std::string& retained_instance, std::string& error);
    // Returns removed contents for the host to publish through its world service.
    // Equipped drops require the equipment owner to un/equip first.
    bool drop(const std::string& instance_id, std::uint32_t quantity,
              InventoryItem& removed, std::string& error);
    // Amount is already valued by the upstream service. No inferred item price.
    bool credit_gold(std::uint64_t amount, std::string& error);
    bool debit_gold(std::uint64_t amount, std::string& error);
};
}
