#include "../loot_tables_v2.hpp"
#include "../merchant_tables_v1.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::data;
using Raw = std::vector<std::uint8_t>;

static unsigned checks = 0;

static void check(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
    ++checks;
}

static Raw read(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    check(bool(input), "Missing source cache: " + path);
    return {std::istreambuf_iterator<char>(input), {}};
}

static Bytes span(const Raw& bytes) { return {bytes.data(), bytes.size()}; }

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Expected canonical loot pydata directory");
        const std::string directory = argv[1];
        auto records = read(directory + "/loot_table_pyarray.bin");
        auto names = read(directory + "/loot_table_pyarraynames.bin");
        auto schema = read(directory + "/loot_table_pystructnames.bin");

        LootTablesV2 loot;
        std::string error;
        check(loot.load(span(records), span(names), span(schema), error), error);
        const auto loot_view = loot.borrow();
        check(loot_view.consumed() == 282872,
              "LootTable boundary differs from the original reader");

        MerchantTablesV1 owner;
        check(owner.load(span(records), span(names), span(schema),
                         loot_view.consumed(), error), error);
        const auto view = owner.borrow();
        check(view.rows().size() == 12 && view.names().size() == 12,
              "MerchantTable count differs from source names");
        check(view.consumed() == 283108,
              "MerchantTable end differs from source cache boundary");
        check(view.names().front() == "Merchant_Bloodsands" &&
                  view.names().back() == "Merchant_Underworld_Epic",
              "MerchantTable row name order differs");

        const auto* swamp = view.find("Merchant_Swamp");
        check(swamp == view.find(8) && swamp && swamp->buy_multiplier == 20 &&
                  swamp->sell_multiplier == 200 && swamp->merchandise.size() == 1,
              "Merchant_Swamp source row differs");
        check(swamp->merchandise[0].condition_id == -1 &&
                  swamp->merchandise[0].merchandise_id == 183,
              "Merchant_Swamp merchandise words differ");
        const auto* online = view.find("Merchant_Online");
        check(online && online->buy_multiplier == 100 &&
                  online->sell_multiplier == 100 && online->merchandise.empty(),
              "Merchant_Online source row differs");

        std::int32_t price = -1;
        check(view.quote(8, MerchantQuoteV1::player_buys_stock, 1000, &price) &&
                  price == 2000,
              "Stock purchase multiplier differs from NativeMerchantBuyItem");
        check(view.quote(8, MerchantQuoteV1::player_buys_back, 1000, &price) &&
                  price == 200,
              "Buyback multiplier differs from NativeMerchantBuyItem");
        check(view.quote(8, MerchantQuoteV1::player_sells, 1000, &price) &&
                  price == 200,
              "Player sale multiplier differs from NativeMerchantSellItem");
        check(view.quote(8, MerchantQuoteV1::player_sells, 2, &price) &&
                  price == 1,
              "Source merchant price minimum differs");
        const auto unchanged = price;
        check(!view.quote(99, MerchantQuoteV1::player_sells, 100, &price) &&
                  price == unchanged,
              "Invalid merchant changed the quote output");

        check(!owner.load(span(records), span(names), span(schema),
                          loot_view.consumed(), error) &&
                  owner.borrow().rows().size() == 12,
              "Reload with a live borrow was accepted or destroyed the snapshot");
        auto truncated_records = records;
        truncated_records.resize(282873);
        MerchantTablesV1 malformed;
        check(!malformed.load(span(truncated_records), span(names), span(schema),
                              loot_view.consumed(), error) &&
                  !malformed.borrow(),
              "Truncated MerchantTable published a snapshot");
        auto bad_schema = schema;
        bad_schema.back() ^= 1;
        check(!malformed.load(span(records), span(names), span(bad_schema),
                              loot_view.consumed(), error) &&
                  !malformed.borrow(),
              "Malformed MerchantTable schema published a snapshot");

        std::cout << "{\"validation\":\"PASS\",\"merchant_rows\":12,"
                     "\"table_end\":283108,\"source_price_quotes\":true,"
                     "\"opaque_merchandise_ids\":true,\"checks\":"
                  << checks << "}\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
