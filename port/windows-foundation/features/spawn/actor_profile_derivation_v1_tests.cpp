// P16 PROFILES validation: regenerate every authored actor profile and melee binding
// from the original pydata tables and diff them field by field against the authored
// XML. Every difference is printed as DIFF; the test fails on any difference.
// Usage: actor_profile_derivation_v1_tests <repo-root> [assets-root]
#include "actor_profile_derivation_v1.hpp"

#include "../../asset_catalog.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <set>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::spawn;

namespace {

void require(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

int differences = 0;
std::size_t compared = 0;

void diff(const std::string& actor, const std::string& field, const std::string& authored, const std::string& derived) {
    ++compared;
    if (authored == derived) return;
    ++differences;
    std::cout << "DIFF " << actor << " " << field << ": authored=[" << authored << "] derived=[" << derived << "]\n";
}

std::string lower(std::string value) {
    for (auto& c : value) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return value;
}

void compare_steps(const std::string& actor, const std::string& where, const std::vector<OriginalMeleeStep>& authored,
                   const std::vector<OriginalMeleeStep>& derived) {
    diff(actor, where + ".stepCount", std::to_string(authored.size()), std::to_string(derived.size()));
    for (std::size_t i = 0; i < std::min(authored.size(), derived.size()); ++i) {
        const auto& a = authored[i];
        const auto& d = derived[i];
        const auto at = where + ".step" + std::to_string(i);
        diff(actor, at + ".index", std::to_string(a.index), std::to_string(d.index));
        diff(actor, at + ".animationId", std::to_string(a.animationId), std::to_string(d.animationId));
        diff(actor, at + ".redirect", std::to_string(a.redirect), std::to_string(d.redirect));
        diff(actor, at + ".blendOut", std::to_string(a.blendOut), std::to_string(d.blendOut));
        diff(actor, at + ".moveGO", std::to_string(a.moveGO), std::to_string(d.moveGO));
        // Speed is a 32-bit float in the original table; compare at float precision.
        diff(actor, at + ".speed", std::to_string(static_cast<float>(a.speed)), std::to_string(static_cast<float>(d.speed)));
        diff(actor, at + ".uri", a.uri, d.uri);
        compare_steps(actor, at, a.children, d.children);
    }
}

void compare_melee(const OriginalMeleeActor& a, const OriginalMeleeActor& d) {
    const auto& actor = a.id;
    diff(actor, "propertyRow", std::to_string(a.propertyRow), std::to_string(d.propertyRow));
    diff(actor, "factionId", std::to_string(a.factionId), std::to_string(d.factionId));
    diff(actor, "aiId", std::to_string(a.aiId), std::to_string(d.aiId));
    diff(actor, "aiName", a.aiName, d.aiName);
    diff(actor, "model", a.model, d.model);
    diff(actor, "template", a.templateClip, d.templateClip);
    for (const auto& [key, value] : a.aiProperties) {
        const auto found = d.aiProperties.find(key);
        diff(actor, "ai." + key, value, found == d.aiProperties.end() ? "<absent>" : found->second);
    }
    std::set<std::string> states;
    for (const auto& s : a.states) states.insert(s.first);
    for (const auto& s : d.states) states.insert(s.first);
    for (const auto& name : states) {
        const auto ai = a.states.find(name);
        const auto di = d.states.find(name);
        if (ai == a.states.end() || di == d.states.end()) {
            diff(actor, "state." + name, ai == a.states.end() ? "<absent>" : "present",
                 di == d.states.end() ? "<absent>" : "present");
            continue;
        }
        diff(actor, "state." + name + ".sequenceCount", std::to_string(ai->second.size()), std::to_string(di->second.size()));
        for (std::size_t i = 0; i < std::min(ai->second.size(), di->second.size()); ++i) {
            const auto& sa = ai->second[i];
            const auto& sd = di->second[i];
            const auto where = "state." + name + ".seq" + std::to_string(i);
            diff(actor, where + ".id", std::to_string(sa.id), std::to_string(sd.id));
            diff(actor, where + ".name", sa.name, sd.name);
            diff(actor, where + ".loop", std::to_string(sa.loop), std::to_string(sd.loop));
            diff(actor, where + ".type", std::to_string(sa.type), std::to_string(sd.type));
            compare_steps(actor, where, sa.steps, sd.steps);
        }
    }
}

int run(const std::filesystem::path& root, const std::filesystem::path& assets_root) {
    AssetCatalog assets(assets_root);
    ProfileDerivationTablesV1 tables;
    std::string error;
    require(load_profile_derivation_tables_v1(assets, "original-cache/data/pydata", tables, error), error);
    ActorProfileLibrary authored_profiles;
    require(authored_profiles.load(assets, "actor-profiles-v2.xml", error), error);
    OriginalMeleeBindings authored_melee;
    require(authored_melee.load(assets, "original-melee-bindings.xml", error), error);

    std::size_t profiles_checked = 0, melee_checked = 0;
    for (const auto& [id, a] : authored_profiles.profiles()) {
        ActorProfile d;
        if (!derive_actor_profile_v1(tables, id, d, error)) {
            diff(id, "derive", "ok", error);
            continue;
        }
        ++profiles_checked;
        diff(id, "character", a.character_uri, d.character_uri);
        diff(id, "model", a.model_uri, d.model_uri);
        diff(id, "template", a.template_clip_uri, d.template_clip_uri);
        diff(id, "propertyRow", a.property_row, d.property_row);
        diff(id, "animationTable", a.animation_table, d.animation_table);
        diff(id, "animationTableName", a.animation_table_name, d.animation_table_name);
        std::set<std::string> names;
        for (const auto& s : a.states) names.insert(s.first);
        for (const auto& s : d.states) names.insert(s.first);
        for (const auto& name : names) {
            const auto ai = a.states.find(name);
        const auto di = d.states.find(name);
            if (ai == a.states.end() || di == d.states.end()) {
                diff(id, "state." + name, ai == a.states.end() ? "<absent>" : "present", di == d.states.end() ? "<absent>" : "present");
                continue;
            }
            diff(id, "state." + name + ".clipCount", std::to_string(ai->second.size()), std::to_string(di->second.size()));
            for (std::size_t i = 0; i < std::min(ai->second.size(), di->second.size()); ++i) {
                // Case-only differences are reported, not hidden: the authored XML uses data/3D/, the table data/3d/ or as stored.
                diff(id, "state." + name + ".clip" + std::to_string(i), ai->second[i].uri, di->second[i].uri);
            }
        }
        diff(id, "rawPropertyCount", std::to_string(a.raw_properties.size()), std::to_string(d.raw_properties.size()));
        for (const auto& [key, value] : a.raw_properties) {
            const auto found = d.raw_properties.find(key);
            if (found == d.raw_properties.end()) {
                diff(id, "raw." + key, std::to_string(value.raw), "<absent>");
                continue;
            }
            diff(id, "raw." + key, std::to_string(value.raw), std::to_string(found->second.raw));
            diff(id, "raw." + key + ".encoding", value.encoding, found->second.encoding);
        }
    }

    for (const auto& [id, a] : authored_melee.actors()) {
        OriginalMeleeActor d;
        if (!derive_melee_actor_v1(tables, id, d, error)) {
            diff(id, "deriveMelee", "ok", error);
            continue;
        }
        ++melee_checked;
        compare_melee(a, d);
    }

    // Minions has a CharacterTable row and no authored profile: derive it and report what it resolves to.
    const std::string minions = "Swamp_Moth_Minions";
    ActorProfile minion_profile;
    OriginalMeleeActor minion_melee;
    CombatSessionProfile minion_policy;
    require(derive_actor_profile_v1(tables, minions, minion_profile, error), error);
    require(derive_melee_actor_v1(tables, minions, minion_melee, error), error);
    require(derive_enemy_combat_policy_v1(tables, minions, minion_policy, error), error);
    std::cout << "MINIONS model=" << minion_profile.model_uri << " animationTable=" << minion_profile.animation_table
              << " (" << minion_profile.animation_table_name << ") template=" << minion_profile.template_clip_uri << "\n";
    for (const auto& [state, clips] : minion_profile.states)
        std::cout << "MINIONS profile state " << state << " clips=" << clips.size() << "\n";
    for (const auto& [state, sequences] : minion_melee.states) {
        std::cout << "MINIONS melee state " << state << " sequences=" << sequences.size();
        for (const auto& sequence : sequences) std::cout << " [" << sequence.name << " id=" << sequence.id << "]";
        std::cout << "\n";
    }

    // Derived-only check: Minions differs from the authored Moth only where the original tables differ.
    const auto& moth_melee = *authored_melee.find_actor("Swamp_Moth_Type1");
    ActorProfile moth_derived;
    OriginalMeleeActor moth_melee_derived;
    require(derive_melee_actor_v1(tables, "Swamp_Moth_Type1", moth_melee_derived, error), error);
    (void)moth_melee;
    require(derive_actor_profile_v1(tables, "Swamp_Moth_Type1", moth_derived, error), error);
    std::set<std::string> minion_only;
    for (const auto& [state, sequences] : minion_melee.states) {
        const auto& moth_sequences = moth_melee_derived.states.at(state);
        if (sequences.size() != moth_sequences.size() || (!sequences.empty() && sequences.front().id != moth_sequences.front().id))
            minion_only.insert(state);
    }
    std::cout << "MINIONS melee states differing from Swamp_Moth_Type1:";
    for (const auto& state : minion_only) std::cout << " " << state;
    std::cout << "\n";

    std::cout << "VALIDATION profiles=" << profiles_checked << " melee=" << melee_checked << " compared=" << compared
              << " differences=" << differences << "\n";
    require(profiles_checked == authored_profiles.profiles().size(), "Not every authored profile was derived");
    require(melee_checked == authored_melee.actors().size(), "Not every authored melee actor was derived");
    require(differences == 0, "Derived profiles/bindings differ from authored XML (see DIFF lines)");
    return 0;
}

} // namespace

int main(int argc, char** argv) {
    try {
        require(argc >= 2 && argc <= 3, "Supply repository root and optional assets root");
        const std::filesystem::path root(argv[1]);
        const auto assets = argc == 3 ? std::filesystem::path(argv[2]) : root / ".local-inputs/windows-shared-assets";
        const int status = run(root, assets);
        std::cout << "actor_profile_derivation_v1 tests passed\n";
        return status;
    } catch (const std::exception& exception) {
        std::cerr << "actor_profile_derivation_v1 tests failed: " << exception.what() << "\n";
        return 1;
    }
}
