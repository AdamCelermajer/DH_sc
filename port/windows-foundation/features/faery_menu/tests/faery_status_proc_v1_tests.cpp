// v21.8 faery2: the status procs of the faery spells (Wetty slow, Windy push) come from the authored ClassTable rows
// Spell_<Element>_1/_2 that faerie_<name>.luac applies with ApplyPropClass(CLASS_ID + skill_level) before each
// SpellCombatRoll. The original rolls them in Character::F_CalcResult (CF__CalcSlow / CF__CalcPush, spell mode 2):
// chance = SnS_*_Target_Chance + attackerLevel/8 - defender resist - defenderLevel/8, success when chance > roll(0..100)<<8,
// and a non-positive SnS chance never procs. This test prints/asserts which authored rows carry the proc chances.
#include "../../../asset_catalog.hpp"
#include "../../../original_actor_properties.hpp"
#include "../../../actor_profiles.hpp"
#include "../../../content_paths.hpp"
#include "../../../../game-data/animation_tables.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
// Property ids of the original CharacterProperties schema (character_properties_pystructnames.bin order + 4 header slots).
constexpr unsigned kHurtChance = 182, kPushChance = 183, kStunChance = 184, kStunDur = 185, kFearChance = 186, kFearDur = 187,
                   kSlowChance = 188, kSlowDur = 189, kSnsLevel = 172;
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply repository root");
        const std::filesystem::path repo(argv[1]);
        AssetCatalog assets(repo / ".local-inputs/windows-source-clock-v19-preview-9/assets");
        std::string error;
        OriginalPropertyDatabase properties;
        check(load_original_property_tables(assets, "original-cache/data/pydata", properties, error), error);
        dh2::data::PropertyRules rules;
        check(dh2::data::load_property_rules(properties.characters, rules, error), error);
        for (const char* base : {"Spell_Fire_1", "Spell_Water_1", "Spell_Air_1", "Spell_Earth_1", "Spell_Lightning_1"}) {
            const auto found = std::find(properties.classes.names.begin(), properties.classes.names.end(), base);
            if (found == properties.classes.names.end()) { std::cout << "class absent: " << base << '\n'; continue; }
            const auto id = std::int32_t(found - properties.classes.names.begin());
            for (int rank = 0; rank <= 1; ++rank) {
                dh2::data::PropertySheet sheet = rules.defaults, owner = rules.defaults;
                sheet[kSnsLevel] = owner[kSnsLevel] = std::int32_t(rank << 8);
                check(dh2::data::apply_class(properties.classes, id + rank, sheet, error, &owner), error);
                std::cout << properties.classes.names[std::size_t(id + rank)] << ": hurt=" << sheet[kHurtChance] / 256.0 << " push=" << sheet[kPushChance] / 256.0
                          << " stun=" << sheet[kStunChance] / 256.0 << "/" << sheet[kStunDur] / 256.0 << " fear=" << sheet[kFearChance] / 256.0 << "/" << sheet[kFearDur] / 256.0
                          << " slow=" << sheet[kSlowChance] / 256.0 << "/" << sheet[kSlowDur] / 256.0 << '\n';
                const std::string name = properties.classes.names[std::size_t(id + rank)];
                const bool water = name.rfind("Spell_Water", 0) == 0, air = name.rfind("Spell_Air", 0) == 0;
                // Only the Water spell rows carry a slow chance (15 / 25 %) with a 5000 ms duration; only the Air rows a push chance (80 / 105).
                check((sheet[kSlowChance] > 0) == water && (sheet[kSlowDur] > 0) == water, name + ": slow proc row differs from the authored Water-only rule");
                check((sheet[kPushChance] > 0) == air, name + ": push proc row differs from the authored Air-only rule");
                if (water) check(sheet[kSlowChance] == (rank == 0 ? 15 : 25) * 256 && sheet[kSlowDur] == 5000 * 256, name + ": slow chance/duration changed");
                if (air) check(sheet[kPushChance] == (rank == 0 ? 80 : 105) * 256, name + ": push chance changed");
                check(sheet[kHurtChance] == 0 && sheet[kStunChance] == 0 && sheet[kFearChance] == 0, name + ": unexpected hurt/stun/fear chance");
            }
        }
        {
            // Monster side of SM_SetKnockBackState: the roots it reads are CharAnimTable +72 (normal) and +36 (great) of the defender's row.
            const auto bytesOf = [](const std::vector<std::uint8_t>& v) { return dh2::data::Bytes{v.data(), v.size()}; };
            const auto readPy = [&](const char* name) { return read_content(assets, std::string("original-cache/data/pydata/") + name); };
            auto clipNames = read_content(assets, "data/animations_dictionary_pyarraynames.bin");
            auto clipValues = readPy("animations_dictionary_pyarray.bin");
            dh2::data::Dictionary clips;
            check(dh2::data::load_dictionary(bytesOf(clipNames), bytesOf(clipValues), clips, error), error);
            auto records = readPy("animations_pyarray.bin"), names = readPy("animations_pyarraynames.bin"), fields = readPy("animations_pystructnames.bin");
            dh2::data::AnimationTables animations;
            check(dh2::data::load_animation_tables(bytesOf(records), bytesOf(names), bytesOf(fields), clips, animations, error), error);
            ActorProfileLibrary profiles;
            check(profiles.load(assets, "actor-profiles-v2.xml", error), error);
            for (const char* id : {"Swamp_LizadMan_Type1", "KnightPlayerBase", "Swamp_Moth_Minions"}) {
                const auto* profile = profiles.find(id);
                if (!profile) { std::cout << id << ": no profile\n"; continue; }
                const auto row = std::stoi(profile->animation_table);
                const auto& f = animations.characters.at(std::size_t(row)).fields;
                std::cout << id << ": CharAnimTable " << row << " normalKnockedBackRoot(field16)=" << f[16][0] << " greatKnockedBackRoot(field8)=" << f[8][0] << '\n';
                // The Windy push needs authored roots on the defender's row: the lizard (the test dummy) and the player both have them.
                check(f[16][0] >= 0 && f[8][0] >= 0, std::string(id) + ": CharAnimTable lacks a KnockedBack root");
            }
        }
        std::cout << "faery_status_proc_v1 PASS\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "faery_status_proc_v1 FAIL: " << e.what() << '\n';
        return 1;
    }
}
