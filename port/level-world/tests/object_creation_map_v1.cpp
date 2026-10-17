#include "../object_creation_map_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <string_view>

using namespace dh2::object_creation_map_v1;

namespace {
void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}
}

int main() {
    require(entries.size() == 33, "source ObjectManager factory count changed");
    for (std::size_t i = 0; i < entries.size(); ++i) {
        require(!entries[i].source_type.empty() && entries[i].source_thunk != 0,
                "factory entry is incomplete");
        for (std::size_t j = i + 1; j < entries.size(); ++j)
            require(entries[i].source_type != entries[j].source_type,
                    "source factory type name is duplicated");
        const auto found = resolve(entries[i].source_type.data());
        require(found.status == Status::resolved && found.entry == &entries[i] &&
                found.source_index == i, "source-order exact lookup changed");
    }

    const auto faery = resolve("Character");
    const auto player = resolve("Player");
    require(faery.status == Status::resolved &&
            faery.entry->constructor == Constructor::character &&
            faery.entry->source_thunk == 0x340800 && faery.source_index == 9,
            "Faery MGP Character type no longer selects Character factory");
    require(player.status == Status::resolved &&
            player.entry->constructor == faery.entry->constructor &&
            player.entry->source_thunk == faery.entry->source_thunk,
            "Player and Character source aliases diverged");
    require(resolve("Block").entry->constructor == Constructor::module &&
            resolve("Block").entry->source_thunk == resolve("Module").entry->source_thunk,
            "Block and Module source aliases diverged");
    require(resolve("Faery").status == Status::not_found,
            "Faery template must not be misused as an ObjectManager type");
    require(resolve("character").status == Status::not_found,
            "source type lookup must remain case-sensitive");
    require(resolve(nullptr).status == Status::invalid_name &&
            resolve("").status == Status::invalid_name,
            "invalid source names must fail closed");

    std::cout << "PASS objectCreationMap=33 characterFactory=0x340800 "
                 "FaeryUsesCharacterType=true\n";
}
