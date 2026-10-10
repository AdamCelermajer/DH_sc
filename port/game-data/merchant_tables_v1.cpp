#include "merchant_tables_v1.hpp"

#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <utility>

namespace {
constexpr std::size_t kMaximumCacheBytes = 8u * 1024u * 1024u;
constexpr std::uint32_t kMaximumRows = 65536;

struct Reader {
    dh2::data::Bytes bytes{};
    std::size_t cursor{};

    explicit Reader(dh2::data::Bytes input) : bytes(input) {
        const auto address = reinterpret_cast<std::uintptr_t>(input.data);
        if (!input.data || input.size > kMaximumCacheBytes ||
            input.size > UINTPTR_MAX - address)
            throw std::runtime_error("Invalid MerchantTable byte span");
    }

    const std::uint8_t* take(std::size_t count) {
        if (count > bytes.size - cursor)
            throw std::runtime_error("Truncated MerchantTable cache");
        const auto* result = bytes.data + cursor;
        cursor += count;
        return result;
    }

    std::uint32_t word() {
        const auto* value = take(4);
        return std::uint32_t(value[0]) |
            (std::uint32_t(value[1]) << 8) |
            (std::uint32_t(value[2]) << 16) |
            (std::uint32_t(value[3]) << 24);
    }

    std::int32_t signed_word() {
        const auto raw = word();
        std::int32_t value{};
        std::memcpy(&value, &raw, sizeof(value));
        return value;
    }

    std::uint32_t count() {
        const auto value = word();
        if (value > kMaximumRows)
            throw std::runtime_error("MerchantTable count exceeds native budget");
        return value;
    }

    std::vector<std::string> strings() {
        const auto count_value = count();
        std::vector<std::string> result;
        result.reserve(count_value);
        for (std::uint32_t index = 0; index < count_value; ++index) {
            const auto length = count();
            const auto* value = take(length);
            result.emplace_back(reinterpret_cast<const char*>(value), length);
        }
        return result;
    }
};

void require_schema(const std::vector<std::string>& actual,
                    std::initializer_list<const char*> expected) {
    if (actual.size() != expected.size())
        throw std::runtime_error("MerchantTable schema differs");
    auto want = expected.begin();
    for (const auto& field : actual) {
        if (field != *want++)
            throw std::runtime_error("MerchantTable schema differs");
    }
}

} // namespace

namespace dh2::data {

struct MerchantTablesV1::Snapshot {
    std::vector<MerchantRowV1> rows;
    std::vector<std::string> names;
    std::size_t record_start{};
    std::size_t consumed{};
};

bool MerchantTablesV1::load(Bytes records, Bytes names, Bytes schema,
                            std::size_t loot_table_end,
                            std::string& error) {
    try {
        if (snapshot_ && snapshot_.use_count() != 1)
            throw std::runtime_error("MerchantTable snapshot has live borrowers");
        if (!records.data || loot_table_end > records.size)
            throw std::runtime_error("Invalid source MerchantTable boundary");

        Reader name_reader(names);
        std::vector<std::string> merchant_names;
        for (unsigned section = 0; section < 8; ++section) {
            auto values = name_reader.strings();
            if (section == 6) merchant_names = std::move(values);
        }
        if (name_reader.cursor != name_reader.bytes.size)
            throw std::runtime_error("Unexpected MerchantTable name trailing bytes");

        Reader schema_reader(schema);
        std::vector<std::string> merchant_entry_schema;
        std::vector<std::string> merchant_schema;
        for (unsigned section = 0; section < 26; ++section) {
            auto fields = schema_reader.strings();
            if (section == 22) merchant_entry_schema = std::move(fields);
            if (section == 23) merchant_schema = std::move(fields);
            if (section == 24)
                require_schema(fields, {"MaxItems"});
            if (section == 25)
                require_schema(fields, {"list_entries"});
        }
        if (schema_reader.cursor != schema_reader.bytes.size)
            throw std::runtime_error("Unexpected MerchantTable schema trailing bytes");
        require_schema(merchant_entry_schema, {"ConditionId", "Merchandise"});
        require_schema(merchant_schema,
                       {"BuyMultiplier", "SellMultiplier", "MerchandiseList"});

        Reader table({records.data + loot_table_end,
                      records.size - loot_table_end});
        const auto row_count = table.count();
        if (row_count == 0 || row_count != merchant_names.size())
            throw std::runtime_error("MerchantTable names/count mismatch");
        auto next = std::make_shared<Snapshot>();
        next->record_start = loot_table_end;
        next->rows.reserve(row_count);
        next->names = std::move(merchant_names);
        for (std::uint32_t index = 0; index < row_count; ++index) {
            MerchantRowV1 row;
            row.buy_multiplier = table.signed_word();
            row.sell_multiplier = table.signed_word();
            const auto entry_count = table.count();
            if (entry_count > (table.bytes.size - table.cursor) / 8)
                throw std::runtime_error("Truncated MerchantTable merchandise list");
            row.merchandise.reserve(entry_count);
            for (std::uint32_t entry = 0; entry < entry_count; ++entry)
                row.merchandise.push_back(
                    {table.signed_word(), table.signed_word()});
            next->rows.push_back(std::move(row));
        }
        next->consumed = loot_table_end + table.cursor;
        snapshot_ = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

const std::vector<MerchantRowV1>&
MerchantTablesV1::Borrow::rows() const {
    if (!snapshot_) throw std::logic_error("Unbound MerchantTable");
    return snapshot_->rows;
}

const std::vector<std::string>&
MerchantTablesV1::Borrow::names() const {
    if (!snapshot_) throw std::logic_error("Unbound MerchantTable");
    return snapshot_->names;
}

std::size_t MerchantTablesV1::Borrow::consumed() const {
    if (!snapshot_) throw std::logic_error("Unbound MerchantTable");
    return snapshot_->consumed;
}

const MerchantRowV1*
MerchantTablesV1::Borrow::find(std::int32_t id) const noexcept {
    if (!snapshot_ || id < 0 || std::size_t(id) >= snapshot_->rows.size())
        return nullptr;
    return &snapshot_->rows[std::size_t(id)];
}

const MerchantRowV1*
MerchantTablesV1::Borrow::find(std::string_view name) const noexcept {
    if (!snapshot_) return nullptr;
    for (std::size_t index = 0; index < snapshot_->names.size(); ++index)
        if (snapshot_->names[index] == name) return &snapshot_->rows[index];
    return nullptr;
}

bool MerchantTablesV1::Borrow::quote(std::int32_t merchant_id,
                                    MerchantQuoteV1 kind,
                                    std::int32_t source_item_value,
                                    std::int32_t* price) const noexcept {
    if (!price) return false;
    const auto* row = find(merchant_id);
    if (!row) return false;
    std::int32_t multiplier{};
    switch (kind) {
    case MerchantQuoteV1::player_buys_stock:
        multiplier = row->sell_multiplier;
        break;
    case MerchantQuoteV1::player_buys_back:
    case MerchantQuoteV1::player_sells:
        multiplier = row->buy_multiplier;
        break;
    default:
        return false;
    }

    // The original callbacks convert both signed integers to float, divide
    // the multiplier by 100.0f, multiply, then truncate toward zero. Keep
    // the operations in that order to preserve native rounding behavior.
    const float factor = static_cast<float>(multiplier) / 100.0f;
    const float total = static_cast<float>(source_item_value) * factor;
    if (!std::isfinite(total) ||
        static_cast<double>(total) >
            static_cast<double>(std::numeric_limits<std::int32_t>::max()) ||
        static_cast<double>(total) <
            static_cast<double>(std::numeric_limits<std::int32_t>::min()))
        return false;
    const auto truncated = static_cast<std::int32_t>(total);
    *price = truncated < 1 ? 1 : truncated;
    return true;
}

} // namespace dh2::data
