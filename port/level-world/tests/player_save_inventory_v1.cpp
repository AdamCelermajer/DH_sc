#ifdef NDEBUG
#undef NDEBUG
#endif

#include "../player_save_inventory_v1.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace saved = dh2::player_save_inventory_v1;
using Raw = std::vector<std::uint8_t>;

namespace {
unsigned checks = 0;

void require(bool condition, const char* message) {
    ++checks;
    if (!condition) throw std::runtime_error(message);
}

void word(Raw& bytes, std::uint32_t value) {
    bytes.push_back(std::uint8_t(value));
    bytes.push_back(std::uint8_t(value >> 8));
    bytes.push_back(std::uint8_t(value >> 16));
    bytes.push_back(std::uint8_t(value >> 24));
}

void text(Raw& bytes, const std::string& value) {
    word(bytes, static_cast<std::uint32_t>(value.size() + 1));
    bytes.insert(bytes.end(), value.begin(), value.end());
    bytes.push_back(0);
}

Raw expected_payload() {
    Raw bytes;
    word(bytes, 0x10203040);
    word(bytes, 1);
    word(bytes, 2);

    text(bytes, "ring");
    word(bytes, static_cast<std::uint32_t>(-4));
    word(bytes, 5);
    word(bytes, static_cast<std::uint32_t>(-1));
    word(bytes, static_cast<std::uint32_t>(-777));
    bytes.push_back(0xa5);
    word(bytes, 2);
    text(bytes, "freeze");
    text(bytes, "poison");

    text(bytes, "sword");
    word(bytes, 0xffffffff);
    word(bytes, 0xffffffff);
    word(bytes, 500);
    word(bytes, 0x12345678);
    bytes.push_back(0);
    word(bytes, 0);
    return bytes;
}

}  // namespace

int main() {
    try {
        const std::vector<std::string> item_names{"sword", "ring"};
        const std::vector<std::string> power_names{"poison", "freeze"};
        dh2::data::ItemInstanceV1 first;
        first.id = 1;
        first.quantity = 0xffff;
        first.value = -777;
        first.identified = 0xa5;
        first.powers = {1, 0};
        dh2::data::ItemInstanceV1 second;
        second.id = 0;
        second.quantity = 500;
        second.value = 0x12345678;
        second.identified = 0;
        dh2::data::OwnedItemSlotV4 first_slot;
        first_slot.slots = {-4, 5};
        dh2::data::OwnedItemSlotV4 second_slot;
        const saved::ItemRef items[]{{&first, &first_slot}, {&second, &second_slot}};
        const saved::GearView view{0x10203040, 1, items, 2, &item_names,
                                   &power_names};
        const auto expected = expected_payload();

        Raw actual(expected.size(), 0xee);
        saved::Result result{};
        std::string error = "sentinel";
        const auto status = saved::save(view, {actual.data(), actual.size()},
                                        &result, error);
        require(status == saved::Status::complete, "complete GEAR write failed");
        require(error.empty(), "successful GEAR write retained an error");
        require(result.stage == saved::Stage::complete &&
                    result.source_caller == 0x46a314,
                "complete GEAR return boundary changed");
        require(result.written == expected.size() &&
                    std::equal(expected.begin(), expected.end(), actual.begin()),
                "GEAR byte order or field encoding differs from source");
        require(result.stream_writes == 23 && result.declared_items == 2 &&
                    result.completed_items == 2 && result.declared_powers == 0 &&
                    result.completed_powers == 0,
                "GEAR source write accounting differs");

        unsigned prefixes = 0;
        for (std::size_t capacity = 0; capacity < expected.size(); ++capacity) {
            Raw bounded(expected.size(), 0xee);
            saved::Result prefix{};
            std::string failure;
            const auto prefix_status = saved::save(
                view, {bounded.data(), capacity}, &prefix, failure);
            require(prefix_status == saved::Status::failed && !failure.empty(),
                    "truncated GEAR output claimed success");
            require(prefix.written <= capacity &&
                        std::equal(expected.begin(), expected.begin() + prefix.written,
                                   bounded.begin()),
                    "truncated GEAR output lost its exact completed-write prefix");
            require(std::all_of(bounded.begin() + capacity, bounded.end(),
                                [](std::uint8_t byte) { return byte == 0xee; }),
                    "GEAR writer crossed the supplied output bound");
            ++prefixes;
        }

        auto invalid_item = items[0];
        dh2::data::ItemInstanceV1 missing_identifier;
        missing_identifier.id = 8;
        invalid_item.item = &missing_identifier;
        const saved::ItemRef invalid_items[]{invalid_item};
        const saved::GearView invalid_view{7, 0, invalid_items, 1, &item_names,
                                           &power_names};
        Raw invalid_output(64, 0xcc);
        saved::Result invalid_result{};
        error.clear();
        require(saved::save(invalid_view,
                            {invalid_output.data(), invalid_output.size()},
                            &invalid_result, error) == saved::Status::failed &&
                    invalid_result.stage == saved::Stage::item_name &&
                    invalid_result.written == 12 && !error.empty(),
                "unsupported item ID did not stop after the source header");

        dh2::data::ItemInstanceV1 missing_power;
        missing_power.id = 0;
        missing_power.powers = {9};
        const saved::ItemRef invalid_power_items[]{{&missing_power, &second_slot}};
        const saved::GearView invalid_power_view{7, 0, invalid_power_items, 1,
                                                 &item_names, &power_names};
        saved::Result power_result{};
        error.clear();
        require(saved::save(invalid_power_view,
                            {invalid_output.data(), invalid_output.size()},
                            &power_result, error) == saved::Status::failed &&
                    power_result.stage == saved::Stage::power_name &&
                    power_result.declared_powers == 1 &&
                    power_result.completed_powers == 0 && !error.empty(),
                "unsupported power ID did not retain the source prefix");

        const saved::GearView empty_view{0, 0, nullptr, 0, &item_names,
                                         &power_names};
        std::array<std::uint8_t, 12> empty_bytes{};
        saved::Result empty_result{};
        error.clear();
        require(saved::save(empty_view, {empty_bytes.data(), empty_bytes.size()},
                            &empty_result, error) == saved::Status::complete &&
                    empty_result.written == 12 && empty_result.completed_items == 0,
                "empty GEAR inventory did not write its three header words");

        std::cout << "{\"validation\":\"PASS\",\"checks\":" << checks
                  << ",\"truncation_prefixes\":" << prefixes
                  << ",\"payload_bytes\":" << expected.size() << "}\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
