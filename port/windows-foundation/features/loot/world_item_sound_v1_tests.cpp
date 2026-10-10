// B048 focused test: world item drop/pickup sound ordinals from the actual
// ItemAudioVisualTable. argv[1] = a package assets root containing
// data/loot_audiovisual_{pyarray,pyarraynames,pystructnames}.bin.
#include "world_item_sound_v1.hpp"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

using dh::foundation::loot::WorldItemSoundEventV1;
using dh::foundation::loot::world_item_sound_ordinal_v1;

void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

std::vector<std::uint8_t> read_file(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("Cannot read " + path.string());
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

void put_word(std::vector<std::uint8_t>& out, std::uint32_t value) {
    for (int shift = 0; shift < 32; shift += 8) out.push_back(std::uint8_t(value >> shift));
}

void put_text(std::vector<std::uint8_t>& out, const std::string& text) {
    put_word(out, std::uint32_t(text.size()));
    out.insert(out.end(), text.begin(), text.end());
}

dh2::data::Bytes bytes_of(const std::vector<std::uint8_t>& value) {
    return {value.data(), value.size()};
}

} // namespace

int main(int argc, char** argv) try {
    check(argc == 2, "usage: world_item_sound_v1_tests <package-assets-root>");
    const std::filesystem::path root(argv[1]);
    const auto records = read_file(root / "data/loot_audiovisual_pyarray.bin");
    const auto names = read_file(root / "data/loot_audiovisual_pyarraynames.bin");
    const auto schema = read_file(root / "data/loot_audiovisual_pystructnames.bin");

    // Real table: row order equals ItemTable AudioVisualID (names index order).
    dh2::data::LootAudioVisualV8 table;
    std::string error;
    check(table.load(bytes_of(records), bytes_of(names), bytes_of(schema), error),
          "actual loot audiovisual table did not load: " + error);
    const auto borrow = table.borrow();
    check(borrow && borrow.rows().size() == 29 && borrow.names().size() == 29,
          "unexpected ItemAudioVisualTable row count");

    struct Expect { int row; const char* name; int drop; int pickup; };
    // Gold stacks use DropGold/PickupGold; Potion uses DropPotion/PickupPotion;
    // Knife is a weapon (DropSword/PickupWeapon); Belt is armor (DropArmor/PickupArmor).
    const Expect expected[] = {
        {13, "GoldStack1", 62, 155},
        {14, "GoldStack2", 62, 155},
        {15, "GoldStack3", 62, 155},
        {20, "Potion", 63, 156},
        {17, "Knife", 64, 157},
        {2, "Belt", 61, 154},
    };
    for (const auto& item : expected) {
        check(borrow.names()[std::size_t(item.row)] == item.name,
              std::string("row name mismatch for ") + item.name);
        std::int32_t ordinal = -2;
        check(world_item_sound_ordinal_v1(borrow, item.row, WorldItemSoundEventV1::drop, ordinal, error) &&
              ordinal == item.drop,
              std::string("drop ordinal mismatch for ") + item.name + ": " + error);
        check(world_item_sound_ordinal_v1(borrow, item.row, WorldItemSoundEventV1::pickup, ordinal, error) &&
              ordinal == item.pickup,
              std::string("pickup ordinal mismatch for ") + item.name + ": " + error);
    }

    // Outcome branches: outside the table and no loaded table fail silently.
    std::int32_t ordinal = 99;
    check(!world_item_sound_ordinal_v1(borrow, -1, WorldItemSoundEventV1::drop, ordinal, error) && ordinal == -1,
          "negative AudioVisualID must not select a sound");
    check(!world_item_sound_ordinal_v1(borrow, 29, WorldItemSoundEventV1::pickup, ordinal, error) && ordinal == -1,
          "AudioVisualID past the table must not select a sound");
    check(!world_item_sound_ordinal_v1(dh2::data::LootAudioVisualV8::Borrow(), 13,
          WorldItemSoundEventV1::drop, ordinal, error),
          "unbound table must fail");

    // Synthetic table: a row whose sound id is -1 is silent by source (Play3D early return).
    std::vector<std::uint8_t> synth_records, synth_names, synth_schema;
    put_word(synth_records, 1);
    put_word(synth_records, 0xFFFFFFFFu);
    put_word(synth_records, 5);
    put_text(synth_records, "dummy_itemdrop_Silent");
    put_word(synth_names, 1);
    put_text(synth_names, "Silent");
    put_word(synth_schema, 3);
    put_text(synth_schema, "AudioDrop");
    put_text(synth_schema, "AudioPickup");
    put_text(synth_schema, "Visual");
    dh2::data::LootAudioVisualV8 synth;
    check(synth.load(bytes_of(synth_records), bytes_of(synth_names), bytes_of(synth_schema), error),
          "synthetic table did not load: " + error);
    check(!world_item_sound_ordinal_v1(synth.borrow(), 0, WorldItemSoundEventV1::drop, ordinal, error) &&
          ordinal == -1 && !error.empty(), "drop sound id -1 must stay silent");
    check(world_item_sound_ordinal_v1(synth.borrow(), 0, WorldItemSoundEventV1::pickup, ordinal, error) &&
          ordinal == 5, "pickup sound id 5 must map to ordinal 5");

    std::cout << "PASS world item sound ordinals: drop/pickup rows GoldStack 62/155, Potion 63/156, "
                 "Knife 64/157, Belt 61/154; out-of-range, unbound and -1 branches silent\n";
    return 0;
} catch (const std::exception& error) {
    std::cerr << "FAIL " << error.what() << '\n';
    return 1;
}
