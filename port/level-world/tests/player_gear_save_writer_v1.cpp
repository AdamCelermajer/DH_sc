#ifdef NDEBUG
#undef NDEBUG
#endif

#include "../player_gear_save_writer_v1.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace gear = dh2::player_gear_save_writer_v1;
using Bytes = std::vector<std::uint8_t>;

namespace {
unsigned checks = 0;
void require(bool okay, const char* message) {
    ++checks;
    if (!okay) throw std::runtime_error(message);
}
Bytes file(const std::filesystem::path& path) {
    std::ifstream stream(path, std::ios::binary | std::ios::ate);
    require(bool(stream), "cache file missing");
    const auto size = stream.tellg();
    require(size >= 0, "cache file size invalid");
    Bytes bytes(static_cast<std::size_t>(size));
    stream.seekg(0);
    require(bool(stream.read(reinterpret_cast<char*>(bytes.data()),
                             std::streamsize(bytes.size()))),
            "cache file truncated");
    return bytes;
}
bool never_random(void*, std::int32_t, std::uint32_t, std::int32_t&,
                  std::string& error) {
    error = "empty GEAR test unexpectedly requested RNG";
    return false;
}
bool inventory_effects(void*, dh2::data::FreshInventoryOwnedV4&,
                       const dh2::data::OwnedInventoryRequestV4& request,
                       dh2::data::OwnedInventoryResponseV4& response,
                       std::string& error) {
    using Op = dh2::data::OwnedInventoryOperationV4;
    switch (request.operation) {
        case Op::update_name:
        case Op::update_stats:
        case Op::update_requirements:
        case Op::full_notifications:
        case Op::gold_notifications:
        case Op::debug_load:
            error.clear();
            return true;
        case Op::debug_query:
            response.value = 0;
            error.clear();
            return true;
        case Op::inventory_full:
            response.value = 0;
            error.clear();
            return true;
        default:
            error = "unexpected operation in focused writer test: " +
                    std::to_string(static_cast<std::uint32_t>(request.operation));
            return false;
    }
}
void observe_storage(void*, dh2::data::FreshInventoryOwnedV4&,
                     const dh2::data::OwnedInventoryRequestV4&) {}
template <std::size_t N>
std::uint32_t word(const std::array<std::uint8_t, N>& bytes,
                   std::size_t offset) {
    require(offset + 4 <= N, "truncated word in output");
    return std::uint32_t(bytes[offset]) |
           std::uint32_t(bytes[offset + 1]) << 8 |
           std::uint32_t(bytes[offset + 2]) << 16 |
           std::uint32_t(bytes[offset + 3]) << 24;
}
}

int main(int argc, char** argv) {
    try {
        require(argc == 2, "cache directory argument missing");
        const std::filesystem::path cache(argv[1]);
        std::string error;
        const auto load_table = [&](const char* stem, auto& table) {
            const auto records = file(cache / (std::string(stem) + "_pyarray.bin"));
            const auto names = file(cache / (std::string(stem) + "_pyarraynames.bin"));
            const auto schema = file(cache / (std::string(stem) + "_pystructnames.bin"));
            return table.load({records.data(), records.size()},
                              {names.data(), names.size()},
                              {schema.data(), schema.size()}, error);
        };
        dh2::data::LootTablesV2 loot;
        dh2::data::ItemPowerTablesV5 powers;
        require(load_table("loot_table", loot), "LootTable snapshot failed");
        require(load_table("item_powers", powers), "ItemPower snapshot failed");

        constexpr std::uintptr_t character = UINT64_C(0x100000001);
        dh2::data::PropertyState properties{};
        dh2::data::FreshInventoryOwnedV4 inventory(
            character, loot.borrow(), {nullptr, never_random}, 12, properties);
        const auto& item_rows = inventory.table().rows;
        std::int32_t item_id = -1;
        for (std::size_t i = 0; i < item_rows.size(); ++i) {
            const auto type = dh2::data::item_type(item_rows[i]);
            if (type != 13 && type != 14) {
                item_id = static_cast<std::int32_t>(i);
                break;
            }
        }
        require(item_id >= 0, "actual cache has no ordinary Item row");
        dh2::data::OwnedInventoryServicesV4 services{
            nullptr, inventory_effects, observe_storage, true};
        std::unique_ptr<dh2::data::ItemInstanceV1> incoming;
        require(inventory.create_item(item_id, 0xffff,
                                      {&incoming}, services, error),
                "canonical V4 Item construction failed");
        incoming->value = -777;
        incoming->identified = 0xa5;
        std::int32_t item_index = -1;
        if (!inventory.add_item(incoming, true, false, item_index,
                                services, error) || incoming || item_index != 0)
            throw std::runtime_error("canonical V4 Item transfer failed: " + error);
        inventory.items()[0]->slots = {-2, 7};
        require(inventory.set_gold(0x12345678, services, error),
                "canonical V4 gold setup failed");
        inventory.project_current_equipment(1);
        dh2::data::PlayerSavegameV1 save;
        save.set_character(character);

        std::array<std::uint8_t, 4096> payload{};
        gear::Result result{};
        error = "sentinel";
        require(gear::save({&save, &inventory, powers.borrow()},
                           {payload.data(), payload.size()}, &result, error) ==
                    gear::Status::complete,
                "canonical empty GEAR write failed");
        require(error.empty() && result.completed_items == 1,
                "GEAR writer did not serialize the canonical V4 Item");
        require(word(payload, 0) == 0x12345678 && word(payload, 4) == 1 &&
                    word(payload, 8) == 1,
                "writer changed source gold/set/count header order or width");
        std::size_t at = 12;
        const auto read_word = [&]() {
            const auto value = word(payload, at);
            at += 4;
            return value;
        };
        const auto read_text = [&]() {
            const auto length = read_word();
            require(length && at + length <= result.written &&
                        payload[at + length - 1] == 0,
                    "source GEAR string encoding invalid");
            const std::string value(
                reinterpret_cast<const char*>(payload.data() + at), length - 1);
            at += length;
            return value;
        };
        require(read_text() == inventory.table().identifiers.at(
                                   static_cast<std::size_t>(item_id)),
                "V4 item order or source table identifier changed");
        require(read_word() == static_cast<std::uint32_t>(-2) &&
                    read_word() == 7 && read_word() == UINT32_MAX &&
                    read_word() == static_cast<std::uint32_t>(-777) &&
                    at < result.written && payload[at++] == 0xa5 &&
                    read_word() == 0 && at == result.written,
                "source slot/quantity/value/identified widths or order changed");

        std::array<std::uint8_t, 11> prefix{};
        gear::Result partial{};
        error.clear();
        require(gear::save({&save, &inventory, powers.borrow()},
                           {prefix.data(), prefix.size()}, &partial, error) ==
                    gear::Status::failed && partial.written == 8 &&
                    partial.stage ==
                        dh2::player_save_inventory_v1::Stage::item_count &&
                    !error.empty(),
                "bounded write failed to preserve the source header prefix");

        dh2::data::PlayerSavegameV1 foreign_save;
        foreign_save.set_character(character + 1);
        gear::Result mismatch{};
        error.clear();
        require(gear::save({&foreign_save, &inventory, powers.borrow()},
                           {payload.data(), payload.size()}, &mismatch, error) ==
                    gear::Status::failed && mismatch.written == 0 &&
                    !error.empty(),
                "writer accepted a Save from another Character");

        std::cout << "{\"validation\":\"PASS\",\"checks\":" << checks
                  << ",\"actual_V4_items\":1,\"payload_bytes\":"
                  << result.written << "}\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
