#include "../loot_entry_selection_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <vector>

using namespace dh2::data;

namespace {
void ck(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct Draws {
    std::vector<std::int32_t> values;
    std::vector<std::int32_t> bounds;
    std::vector<std::uint32_t> streams;
    std::size_t next{};
    bool fail{};

    static bool draw(void* context, std::int32_t bound, std::uint32_t stream,
                     std::int32_t& value, std::string& error) {
        auto& self = *static_cast<Draws*>(context);
        self.bounds.push_back(bound);
        self.streams.push_back(stream);
        if (self.fail) {
            error = "expected fixture RNG failure";
            return false;
        }
        ck(self.next < self.values.size(), "unexpected source RNG draw");
        value = self.values[self.next++];
        return true;
    }

    InventoryRandomServiceV4 service() { return {this, draw}; }
};

LootEntry32V2 row(std::int32_t pct, std::int32_t base,
                  std::int32_t mage = 0, std::int32_t rogue = 0,
                  std::int32_t warrior = 0) {
    LootEntry32V2 value{};
    value.words[2] = mage;
    value.words[4] = pct;
    value.words[5] = base;
    value.words[6] = rogue;
    value.words[7] = warrior;
    return value;
}
}

int main() {
    try {
        ck(!loot_entry_uses_percent_v1(row(-1, 20), false), "negative Pct is unsigned weighted");
        ck(loot_entry_uses_percent_v1(row(0, 20), false), "zero Pct is percentage");
        ck(loot_entry_uses_percent_v1(row(100, 20), false), "Pct 100 is percentage");
        ck(!loot_entry_uses_percent_v1(row(101, 20), false), "Pct 101 is weighted");
        ck(loot_entry_uses_percent_v1(row(101, 20), true), "infinite drops forces percentage");

        const LootPlayerClassCountsV1 counts{2, 3, 4};
        auto weighted = row(101, 10, 2, 3, 4);
        ck(loot_entry_effective_probability_v1(weighted, counts, false) == 39,
           "base plus Mage/Rogue/Warrior terms");
        ck(loot_entry_effective_probability_v1(row(100, 99), counts, false) == 0,
           "percentage entries contribute no weight");
        auto wrap = row(101, std::numeric_limits<std::int32_t>::max(), 1);
        ck(loot_entry_effective_probability_v1(wrap, counts, false) == -2147483647,
           "source 32-bit effective probability wraps");

        std::string error;
        Draws pct_zero{{0}}, pct_edge{{99}}, pct_fail{{99}};
        bool accepted = false;
        ck(loot_entry_do_percent_roll_v1(row(0, 0), false, pct_zero.service(),
                                          accepted, error) && accepted,
           "source percentage comparison is inclusive at zero");
        ck(pct_zero.bounds == std::vector<std::int32_t>{100} &&
           pct_zero.streams == std::vector<std::uint32_t>{0}, "percentage RNG bound/stream");
        accepted = false;
        ck(loot_entry_do_percent_roll_v1(row(99, 0), false, pct_edge.service(),
                                          accepted, error) && accepted,
           "Pct 99 accepts draw 99");
        accepted = true;
        ck(loot_entry_do_percent_roll_v1(row(98, 0), false, pct_fail.service(),
                                          accepted, error) && !accepted,
           "Pct 98 rejects draw 99");
        Draws none;
        accepted = true;
        ck(loot_entry_do_percent_roll_v1(row(101, 0), false, none.service(),
                                          accepted, error) && !accepted && none.bounds.empty(),
           "weighted entry has no percent roll");
        accepted = false;
        ck(loot_entry_do_percent_roll_v1(row(101, 0), true, none.service(),
                                          accepted, error) && accepted && none.bounds.empty(),
           "infinite loot accepts without RNG");
        accepted = true;
        pct_fail.fail = true;
        ck(!loot_entry_do_percent_roll_v1(row(40, 0), false, pct_fail.service(),
                                           accepted, error) && accepted &&
           error == "expected fixture RNG failure", "failed roll preserves output");

        const LootEntry32V2 rows[] = {
            row(101, 10), row(100, 9000), row(101, 20), row(101, 0)
        };
        Draws boundary{{10}};
        std::uint32_t selected = 99;
        ck(loot_entries_choose_weighted_v1(rows, 4, {}, false, boundary.service(),
                                            selected, error) && selected == 2,
           "weighted boundary selects the next positive entry");
        ck(boundary.bounds == std::vector<std::int32_t>{30} &&
           boundary.streams == std::vector<std::uint32_t>{0}, "weighted draw bound/stream");
        Draws before{{9}};
        ck(loot_entries_choose_weighted_v1(rows, 4, {}, false, before.service(),
                                            selected, error) && selected == 0,
           "weighted draw selects first entry below boundary");
        Draws no_weight;
        const LootEntry32V2 pct_only[] = {row(100, 99), row(0, 50)};
        selected = 99;
        ck(loot_entries_choose_weighted_v1(pct_only, 2, {}, false, no_weight.service(),
                                            selected, error) && selected == 0 &&
           no_weight.bounds.empty(), "all percentage entries return zero without RNG");
        Draws infinite;
        ck(loot_entries_choose_weighted_v1(rows, 4, counts, true, infinite.service(),
                                            selected, error) && selected == 0 &&
           infinite.bounds.empty(), "infinite-drop weighted choice returns zero without RNG");
        Draws failed{{1}};
        failed.fail = true;
        selected = 77;
        ck(!loot_entries_choose_weighted_v1(rows, 4, {}, false, failed.service(),
                                             selected, error) && selected == 77 &&
           error == "expected fixture RNG failure", "failed weighted draw preserves output");
        selected = 77;
        ck(loot_entries_choose_weighted_v1(nullptr, 0, {}, false, no_weight.service(),
                                            selected, error) && selected == 0,
           "empty source vector returns zero");

        LootRandom8V2 borrowed{1, 0};
        selected = 99;
        ck(dh2_loot_entries_choose_weighted_v1(&selected, rows, 4, &counts, 0,
                                                &borrowed) == 0 &&
           selected < 4 && borrowed.calls == 1,
           "fixture adapter borrows the established Random state once");

        std::cout << "{\"validation\":\"PASS\",\"percentage_boundaries\":5,"
                     "\"effective_probability_cases\":3,\"roll_cases\":6,"
                     "\"weighted_cases\":7,\"duplicate_rng_owner\":false}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "loot_entry_selection_v1_host: " << error.what() << '\n';
        return 1;
    }
}
