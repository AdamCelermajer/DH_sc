#pragma once

#include "data.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace dh2::data {

// One serialized Structs::MerchandiseListEntry. The original table reader
// supplies these two signed words; the Merchandise identifier is retained
// verbatim because its target domain is not yet established.
struct MerchantMerchandiseEntryV1 {
    std::int32_t condition_id{};
    std::int32_t merchandise_id{};
};

// The source Merchant row has two integer price multipliers and its authored
// merchandise list. This resource is descriptive data only: it does not
// instantiate stock or own a Character/ItemInventory.
struct MerchantRowV1 {
    std::int32_t buy_multiplier{};
    std::int32_t sell_multiplier{};
    std::vector<MerchantMerchandiseEntryV1> merchandise;
};

enum class MerchantQuoteV1 : std::uint8_t {
    player_buys_stock,
    player_buys_back,
    player_sells,
};

// Immutable MerchantTable projection decoded from the source loot cache.
// loot_table_end must be LootTablesV2::Borrow::consumed() for the same
// records span; this selects the real table boundary rather than a fixed
// cache offset.
class MerchantTablesV1 {
    struct Snapshot;
    std::shared_ptr<const Snapshot> snapshot_;

public:
    class Borrow {
        friend class MerchantTablesV1;
        std::shared_ptr<const Snapshot> snapshot_;
        explicit Borrow(std::shared_ptr<const Snapshot> snapshot)
            : snapshot_(std::move(snapshot)) {}

    public:
        Borrow() = default;
        explicit operator bool() const noexcept { return bool(snapshot_); }
        const std::vector<MerchantRowV1>& rows() const;
        const std::vector<std::string>& names() const;
        std::size_t consumed() const;
        const MerchantRowV1* find(std::int32_t id) const noexcept;
        const MerchantRowV1* find(std::string_view name) const noexcept;

        // Mirrors the source NativeMerchant callbacks' multiplier selection:
        // ordinary stock purchase uses SellMultiplier, buyback and player
        // sale use BuyMultiplier. Computes max(1, int(value * multiplier /
        // 100)) using the source float path and truncation toward zero.
        bool quote(std::int32_t merchant_id, MerchantQuoteV1 kind,
                   std::int32_t source_item_value,
                   std::int32_t* price) const noexcept;
    };

    bool load(Bytes records, Bytes names, Bytes schema,
              std::size_t loot_table_end, std::string& error);
    Borrow borrow() const { return Borrow(snapshot_); }
};

} // namespace dh2::data
