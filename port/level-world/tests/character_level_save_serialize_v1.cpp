#include "../character_level_save_serialize_v1.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace save = dh2::character_level_save_serialize_v1;
namespace {
void check(bool value, const char* message) {
    if (!value) { std::fprintf(stderr, "FAIL: %s\n", message); std::exit(1); }
}
struct Fixture {
    save::View view{};
    std::vector<std::uint8_t> primary, secondary, p, target, v1306, v1309;
    std::uintptr_t object{};
    std::uint8_t game_object_owner{}, properties_owner{}, state_owner{}, ai_owner{};
    static bool read(void* raw, std::uintptr_t identity, save::View* out,
                     std::string&) {
        auto& self = *static_cast<Fixture*>(raw);
        if (identity != 0x1234) return true;
        *out = self.view;
        return false;
    }
    static bool read_game_object(void* raw, std::uintptr_t identity,
                                 dh2::level_savegame_owner_v1::GameObjectFields* out,
                                 std::string&) {
        auto& self = *static_cast<Fixture*>(raw);
        self.object = identity;
        *out = {1, 0, 0x12345678};
        return false;
    }
    static bool read_record(void* raw,
        const dh2::character_runtime_factory_v1::Record& record,
        save::View* out, std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        const auto owner = [&record](dh2::character_constructor_owner_v1::Component c) {
            const auto& slot = record.components.slots[static_cast<std::size_t>(c)];
            return slot.constructed ? slot.canonical_owner : nullptr;
        };
        if (record.character.identity != 0x1234 ||
            owner(dh2::character_constructor_owner_v1::Component::properties) != &self.properties_owner ||
            owner(dh2::character_constructor_owner_v1::Component::state_machine) != &self.state_owner ||
            owner(dh2::character_constructor_owner_v1::Component::ai) != &self.ai_owner) {
            error = "factory component owner identity mismatch";
            return true;
        }
        *out = self.view;
        return false;
    }
    static bool read_record_object(void* raw,
        const dh2::character_runtime_factory_v1::Record& record,
        dh2::level_savegame_owner_v1::GameObjectFields* out,
        std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        const auto& slot = record.components.slots[static_cast<std::size_t>(
            dh2::character_constructor_owner_v1::Component::game_object)];
        if (record.character.identity != 0x1234 ||
            !slot.constructed || slot.canonical_owner != &self.game_object_owner) {
            error = "factory GameObject owner identity mismatch";
            return true;
        }
        *out = {1, 0, 0x12345678};
        return false;
    }
    void initialize() {
        primary.assign(900, 0x11); secondary.assign(900, 0x22);
        p.assign(12, 0x33); target.assign(12, 0x44);
        v1306.assign(12, 0x55); v1309.assign(12, 0x66);
        view = {7, 0, {primary.data(),primary.size()},
            {secondary.data(),secondary.size()}, {p.data(),p.size()},
            {target.data(),target.size()}, 1, {v1306.data(),v1306.size()},
            {v1309.data(),v1309.size()}, 1, -2, 0, 1};
    }
};
std::uint32_t word(const std::vector<std::uint8_t>& bytes, std::size_t at) {
    return std::uint32_t(bytes.at(at)) | std::uint32_t(bytes.at(at+1)) << 8 |
           std::uint32_t(bytes.at(at+2)) << 16 | std::uint32_t(bytes.at(at+3)) << 24;
}
}

int main() {
    Fixture f; f.initialize();
    const save::Services services{&f, Fixture::read, Fixture::read_game_object};
    std::vector<std::uint8_t> bytes{0xee};
    save::Result result{};
    std::string error;
    check(save::serialize(0x1234, &services, bytes, &result, error) ==
              save::Status::complete,
          "Character::Serialize provider rejected exact source view");
    check(f.object == 0x1234 && bytes.size() == 1865 &&
          result.game_object_bytes == 6 && result.property_blocks == 2 &&
          result.ai_tail_bytes == 6 && result.character_bytes == bytes.size(),
          "Character::Serialize length or provider identity differs");
    check(bytes[0] == 1 && bytes[1] == 0 && word(bytes, 6) == 7 &&
          bytes[10] == 0x11 && bytes[910] == 0x22 &&
          bytes[1810] == 0x33 && bytes[1822] == 0x44 &&
          bytes[1834] == 1 && bytes[1835] == 0x55 &&
          bytes[1847] == 0x66 && word(bytes, 1859) == 0xfffffffeu &&
          bytes[1863] == 0 && bytes[1864] == 1,
          "Character::Serialize write order or exact source spans differ");

    dh2::character_runtime_factory_v1::Record record;
    record.character.identity = record.game_object.identity =
        record.aggro_object.identity = 0x1234;
    record.character.object = &record.aggro_object;
    auto bind_component = [&](dh2::character_constructor_owner_v1::Component c,
                              void* owner) {
        auto& slot = record.components.slots[static_cast<std::size_t>(c)];
        slot.component = c;
        slot.canonical_owner = owner;
        slot.constructed = true;
    };
    bind_component(dh2::character_constructor_owner_v1::Component::game_object, &f.game_object_owner);
    bind_component(dh2::character_constructor_owner_v1::Component::properties, &f.properties_owner);
    bind_component(dh2::character_constructor_owner_v1::Component::state_machine, &f.state_owner);
    bind_component(dh2::character_constructor_owner_v1::Component::ai, &f.ai_owner);
    const save::CanonicalServices canonical{&f, Fixture::read_record,
                                             Fixture::read_record_object};
    f.initialize();
    check(save::serialize_record(record, &canonical, bytes, &result, error) ==
              save::Status::complete && bytes.size() == 1865,
          "canonical Character factory record did not serialize through existing component owners");
    f.view.is_player = 1;
    f.view.properties_primary = {};
    f.view.properties_secondary = {};
    check(save::serialize_record(record, &canonical, bytes, &result, error) ==
              save::Status::complete && bytes.size() == 65 &&
              result.property_blocks == 0,
          "canonical player save should not require the raw NPC property images");
    f.view.is_player = 0;
    f.initialize();
    f.primary.resize(896);
    f.secondary.resize(896);
    f.view.properties_primary = {f.primary.data(), f.primary.size()};
    f.view.properties_secondary = {f.secondary.data(), f.secondary.size()};
    bytes = {0xaa};
    check(save::serialize_record(record, &canonical, bytes, &result, error) ==
              save::Status::invalid_view && bytes == std::vector<std::uint8_t>{0xaa},
          "896-byte property sheets must not stand in for the source 900-byte images");
    record.components.slots[static_cast<std::size_t>(
        dh2::character_constructor_owner_v1::Component::properties)].constructed = false;
    bytes = {0xaa};
    check(save::serialize_record(record, &canonical, bytes, &result, error) ==
              save::Status::provider_unavailable &&
          bytes == std::vector<std::uint8_t>{0xaa},
          "factory save accepted missing canonical properties owner");
    record.components.slots[static_cast<std::size_t>(
        dh2::character_constructor_owner_v1::Component::properties)].constructed = true;

    f.view.is_player = 1;
    f.view.properties_primary = {};
    f.view.properties_secondary = {};
    check(save::serialize(0x1234, &services, bytes, &result, error) ==
              save::Status::complete && bytes.size() == 65 &&
          result.property_blocks == 0,
          "player Character unexpectedly wrote NPC property blobs");

    const save::Services missing{};
    bytes = {0xaa};
    check(save::serialize(0x1234, &missing, bytes, &result, error) ==
              save::Status::provider_unavailable && bytes == std::vector<std::uint8_t>{0xaa},
          "missing canonical Character providers did not fail closed");
    std::puts("PASS character_level_save_serialize_v1 checks=8");
}
