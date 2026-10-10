#include "original_loot.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh2::data;
using namespace dh::foundation::loot;
namespace {
void check(bool value, const std::string& message) { if (!value) throw std::runtime_error(message); }
std::vector<std::uint8_t> read(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    check(bool(file), "Missing original fixture: " + path);
    return {std::istreambuf_iterator<char>(file), {}};
}
Bytes bytes(const std::vector<std::uint8_t>& value) { return {value.data(), value.size()}; }
bool debug(void*, const LootEntryRequestV8& request, std::int32_t& value, std::string& error) {
    if (request.operation == LootEntryOperationV8::debug_load ||
        request.operation == LootEntryOperationV8::debug_query) { value = 0; return true; }
    error = "Test only supplies explicit disabled Debug switches";
    return false;
}
struct Destination {
    bool full = false, fail_after_add = false;
    unsigned adds = 0, quests = 0, gold_calls = 0;
    std::vector<std::unique_ptr<ItemInstanceV1>> items;
    static bool eligible(void* context, LootTemporaryInventoryV8&, bool& allowed, std::string&) {
        allowed = !static_cast<Destination*>(context)->full;
        return true;
    }
    static bool add(void* context, std::unique_ptr<ItemInstanceV1>& item, bool force,
                    bool convert, std::int32_t& index, std::string& error) {
        auto& self = *static_cast<Destination*>(context);
        if (force || !convert || !item) { error = "Incorrect original pickup transfer flags"; return false; }
        index = static_cast<std::int32_t>(self.items.size());
        self.items.push_back(std::move(item));
        ++self.adds;
        return true;
    }
    static bool after(void* context, std::int32_t id, std::string& error) {
        auto& self = *static_cast<Destination*>(context);
        check(!self.items.empty() && self.items.back()->id == id, "Quest tail lost actual item identity");
        ++self.quests;
        if (self.fail_after_add) { error = "Explicit reached quest failure"; return false; }
        return true;
    }
    static bool gold(void* context, std::int32_t amount, std::string&) {
        check(amount == 0, "Temporary loot gold tail invented currency");
        ++static_cast<Destination*>(context)->gold_calls;
        return true;
    }
    PickupTransferServices services() { return {this, eligible, add, after, gold}; }
};
void store_actual(LootTemporaryInventoryV8& inventory, std::int32_t id, std::string& error) {
    auto item = std::make_unique<ItemInstanceV1>();
    item->id = id;
    item->quantity = 1;
    check(inventory.store(item, {nullptr, debug}, nullptr, nullptr, error), error);
    check(!item, "Source store failed to consume actual item");
}
}
int main(int argc, char** argv) {
    try {
        check(argc == 2, "Pass original data/pydata directory");
        const std::string root = argv[1];
        auto records = read(root + "/loot_table_pyarray.bin");
        auto names = read(root + "/loot_table_pyarraynames.bin");
        auto schema = read(root + "/loot_table_pystructnames.bin");
        LootTablesV2 tables;
        std::string error;
        check(tables.load(bytes(records), bytes(names), bytes(schema), error), error);
        auto borrow = tables.borrow();
        check(!borrow.loots().empty() && !borrow.items().rows.empty(), "Original loot cache empty");
        LootRandom8V2 random{0x12345678u, 0};
        std::size_t authored_entries = 0;
        // Compare complete fixed-entry expansion against actual serialized lists;
        // no new item IDs, drop quantities, or balance tables are introduced.
        for (const auto& table : borrow.loots()) {
            std::vector<const LootEntry32V2*> entries;
            std::vector<const LootItemEntryV2*> expected;
            for (const auto& entry : table.fixed_entries) {
                entries.push_back(&entry);
                for (const auto& item : borrow.item_lists().at(entry.words[0])) expected.push_back(&item);
            }
            std::vector<LootItemInfoV8> actual;
            LootItemSelectionV8 expansion(borrow, random, {nullptr, debug});
            check(expansion.expand(entries, true, actual, error), error);
            check(actual.size() == expected.size(), "Original fixed item expansion count differs");
            for (std::size_t index = 0; index < actual.size(); ++index) {
                check(actual[index].id == expected[index]->item && actual[index].quantity == expected[index]->quantity,
                      "Original item identity or authored quantity changed");
                check(actual[index].item == &borrow.items().rows.at(expected[index]->item), "Original table identity copied");
                ++authored_entries;
            }
        }
        check(random.calls == 0, "Give-all fixed list expansion consumed invented RNG");
        SelectedLoot selected;
        check(select_items(borrow, {}, random, {}, -1, false, selected, error) && selected.items.empty(),
              "Source absent loot table did not remain empty");
        check(!select_items({}, {}, random, {}, 0, false, selected, error), "Missing source table accepted");
        OriginalLootFlow flow(borrow, {}, {}, random);
        GenerationReceipt generated;
        check(flow.generate({}, {}, generated, error) && generated.source_no_table &&
              generated.items_after == 0 && generated.random_calls_before == generated.random_calls_after,
              "Absent source loot invented items or RNG calls");
        LootRequest required;
        required.table = 0;
        check(!flow.generate(required, {}, generated, error) && !error.empty(),
              "Missing original loot providers accepted");
        const auto actual_id = borrow.item_lists().front().front().item;
        LootTemporaryInventoryV8 source(borrow);
        store_actual(source, actual_id, error);
        auto* identity = source.peek();
        Destination destination;
        PickupReceipt receipt;
        destination.full = true;
        check(transfer_drop(source, destination.services(), receipt, error), error);
        check(receipt.outcome == PickupOutcome::retained && source.peek() == identity && destination.adds == 0,
              "Inventory-full gate lost world item");
        destination.full = false;
        check(transfer_drop(source, destination.services(), receipt, error), error);
        check(receipt.outcome == PickupOutcome::transferred && source.items().empty() &&
              destination.items.front().get() == identity && destination.quests == 1 && destination.gold_calls == 1,
              "Pickup did not retain SAME item or original tails");
        check(transfer_drop(source, destination.services(), receipt, error), error);
        check(receipt.outcome == PickupOutcome::already_empty && destination.adds == 1 && destination.quests == 1,
              "Duplicate pickup delivered duplicate inventory/quest effects");
        LootTemporaryInventoryV8 failed(borrow);
        store_actual(failed, actual_id, error);
        Destination failure;
        failure.fail_after_add = true;
        check(!transfer_drop(failed, failure.services(), receipt, error), "Reached quest failure ignored");
        check(failure.adds == 1 && failure.items.size() == 1 && receipt.outcome == PickupOutcome::failed,
              "Reached failure lost actual transferred item");
        check(!transfer_drop(failed, failure.services(), receipt, error) && failure.adds == 1,
              "Failed source prefix allowed duplicate retry");
        LootTemporaryInventoryV8 missing(borrow);
        store_actual(missing, actual_id, error);
        check(!transfer_drop(missing, {}, receipt, error) && missing.peek(), "Missing provider silently accepted");
        std::cout << "{\"validation\":\"PASS\",\"original_tables\":" << borrow.loots().size()
                  << ",\"authored_fixed_entries\":" << authored_entries
                  << ",\"fullness_retention\":true,\"duplicate_pickup\":true,\"failure_prefix\":true}\n";
    } catch (const std::exception& exception) { std::cerr << exception.what() << '\n'; return 1; }
}
