#include "../character_faery_selection.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace k = dh2::character_faery_selection;

struct Fixture {
    std::array<std::int32_t, 5> member_ids{{2, 4, 5, 6, 3}};
    std::array<k::FaeryListRow, 2> lists{};
    std::array<k::FaeryRow, 7> faeries{};
    k::Tables tables{};
    k::Character character{0x1000, -1};
    k::Globals globals{};
    std::vector<std::int32_t> counts{7, 7};
    std::vector<k::Assertion> assertions;
    std::uint32_t query_index = 0;
    bool mutate_size_on_first_count = false;
    bool mutate_member_after_type_assertion = false;
    std::int32_t error_query = -1;
    k::Services services{};
    k::Result result{};

    Fixture() {
        lists[0] = {0, 5, member_ids.data()};
        lists[1] = {1, 5, member_ids.data()};
        for (std::uint32_t i = 0; i < member_ids.size(); ++i)
            faeries[static_cast<std::uint32_t>(member_ids[i])].words[8] = i;
        tables = {lists.data(), static_cast<std::uint32_t>(lists.size()),
                  faeries.data(), static_cast<std::uint32_t>(faeries.size())};
        globals = {&tables, 0};
        services = {this, &Fixture::get_count, &Fixture::report_assertion};
    }
    static std::int32_t get_count(void* context, k::Character*, const k::Request* request,
                                  k::Response* response) {
        auto& f = *static_cast<Fixture*>(context);
        assert(request && request->operation == k::Operation::faery_types_count);
        assert(request->category && std::string(request->category) == "FaeryTypes");
        assert(request->key && std::string(request->key) == "COUNT");
        const auto i = f.query_index++;
        if (static_cast<std::int32_t>(i) == f.error_query) return 1;
        response->word = i < f.counts.size() ? f.counts[i] : f.counts.back();
        if (f.mutate_size_on_first_count && i == 0) f.lists[0].list_size = 7;
        return 0;
    }
    static std::int32_t report_assertion(void* context, k::Character*, const k::Request* request) {
        auto& f = *static_cast<Fixture*>(context);
        assert(request && request->operation == k::Operation::assertion);
        f.assertions.push_back(request->assertion);
        if (f.mutate_member_after_type_assertion &&
            request->assertion == k::Assertion::table_type_matches_index)
            f.member_ids[2] = 4;
        return 0;
    }
    k::Status run(std::int32_t index) {
        return k::select(&character, index, &globals, &services, &result);
    }
};

int main(int argc, char** argv) {
    if (argc == 2) {
        Fixture f;
        std::int32_t index = 2;
        if (argv[1] == std::string("valid_explicit")) {
            f.character.faery_list_id_106c = 1;
        } else if (argv[1] == std::string("invalid_list_fallback")) {
            f.character.faery_list_id_106c = -1;
            index = 4;
        } else if (argv[1] == std::string("first_count_small")) {
            f.counts = {2, 7};
            index = 4;
        } else if (argv[1] == std::string("type_mismatch")) {
            f.faeries[5].words[8] = 6;
        } else if (argv[1] == std::string("list_size_mismatch")) {
            f.counts = {7, 6};
        } else {
            return 2;
        }
        const auto status = f.run(index);
        const auto row_index = f.result.row ?
            static_cast<std::int32_t>(f.result.row - f.faeries.data()) : -1;
        std::cout << "{\"status\":" << static_cast<std::int32_t>(status)
                  << ",\"authored_list_id\":" << f.result.authored_list_id
                  << ",\"list_id\":" << f.result.selected_list_id
                  << ",\"row_index\":" << row_index
                  << ",\"queries\":" << f.result.constant_queries
                  << ",\"type_matches\":" << f.result.type_matches << "}\n";
        return status == k::Status::complete ? 0 : 1;
    }
    std::uint32_t cases = 0;
    {
        Fixture f;
        f.character.faery_list_id_106c = 1;
        assert(f.run(2) == k::Status::complete);
        assert(f.result.authored_list_id == 1 && f.result.selected_list_id == 1 &&
               f.result.row == &f.faeries[5]);
        assert(f.result.constant_queries == 2 && f.result.type_matches == 1);
        ++cases;
    }
    {
        Fixture f;
        f.character.faery_list_id_106c = -1;
        assert(f.run(4) == k::Status::complete);
        assert(f.result.authored_list_id == -1 && f.result.selected_list_id == 0 &&
               f.result.row == &f.faeries[3]);
        ++cases;
    }
    {
        Fixture f;
        f.character.faery_list_id_106c = 99;
        assert(f.run(0) == k::Status::complete);
        assert(f.result.authored_list_id == 99 && f.result.selected_list_id == 0 &&
               f.result.row == &f.faeries[2]);
        ++cases;
    }
    {
        Fixture f;
        f.mutate_size_on_first_count = true;
        f.counts = {5, 7};
        assert(f.run(2) == k::Status::complete);
        assert(f.result.row == &f.faeries[5] && f.result.constant_queries == 2);
        ++cases;
    }
    {
        Fixture f;
        f.counts = {2, 7};
        assert(f.run(4) == k::Status::complete);
        assert(f.result.row == &f.faeries[3] && f.result.assertions == 0);
        ++cases;
    }
    {
        Fixture f;
        f.faeries[5].words[8] = 6;
        assert(f.run(2) == k::Status::complete);
        assert(f.result.row == &f.faeries[5] && f.result.type_matches == 0);
        ++cases;
    }
    {
        Fixture f;
        f.counts = {7, 6};
        assert(f.run(2) == k::Status::complete);
        assert(f.result.row == &f.faeries[5]);
        ++cases;
    }
    {
        Fixture f;
        assert(f.run(-1) == k::Status::invalid_source_fact);
        assert(f.result.constant_queries == 1);
        ++cases;
    }
    {
        Fixture f;
        f.lists[0].list_size = 1;
        assert(f.run(2) == k::Status::invalid_source_fact);
        ++cases;
    }
    {
        Fixture f;
        f.error_query = 0;
        assert(f.run(0) == k::Status::service_failed);
        assert(f.result.constant_queries == 1);
        ++cases;
    }
    {
        Fixture f;
        f.globals.assert_level = 1;
        f.counts = {2, 7};
        f.faeries[5].words[8] = 6;
        f.mutate_member_after_type_assertion = true;
        assert(f.run(2) == k::Status::complete);
        assert(f.assertions.size() == 3);
        assert(f.assertions[0] == k::Assertion::faery_id_in_range);
        assert(f.assertions[1] == k::Assertion::list_size_matches_types);
        assert(f.assertions[2] == k::Assertion::table_type_matches_index);
        assert(f.result.row == &f.faeries[4]);
        ++cases;
    }
    {
        Fixture f;
        f.globals.assert_level = 2;
        f.counts = {2, 7};
        assert(f.run(2) == k::Status::fatal_source_assertion);
        assert(f.result.constant_queries == 1 && f.result.assertions == 0);
        ++cases;
    }
    std::cout << "{\"validation\":\"PASS\",\"host_cases\":" << cases
              << ",\"source\":\"Character::GetCharFaery\"}\n";
}
