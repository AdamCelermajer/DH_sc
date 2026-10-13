#include "../game_object_trap_tables_v1.hpp"
#include "../timer_trap_external_script_v1.hpp"
#include "../timer_trap_external_script_v1.hpp"
#include "../timer_trap_v1.hpp"

#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {
using namespace dh2::timer_trap_v1;

std::vector<std::uint8_t> read(const std::string& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) throw std::runtime_error("cannot open " + path);
    return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}
void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct Fixture {
    std::int32_t dt{16};
    bool script_allows{true};
    std::vector<Identity> characters{0x10, 0x20};
    std::vector<Identity> rolled, attacked, triggered;
    std::vector<std::string> clips, events;
    int deleted{};
    bool clip_exists_remove{true};
};
Fixture* fx(void* raw) { return static_cast<Fixture*>(raw); }

Services make(Fixture& f) {
    Services s;
    s.context = &f;
    s.dt_ms = [](void* c) { return fx(c)->dt; };
    s.is_character = [](void* c, Identity o) {
        for (auto id : fx(c)->characters) if (id == o) return true;
        return false;
    };
    s.can_trigger = [](void* c, Identity o, bool& allowed) {
        fx(c)->triggered.push_back(o);
        allowed = fx(c)->script_allows;
        return true;
    };
    s.script_damage_roll = [](void* c, Identity o) {
        fx(c)->rolled.push_back(o);
        return true;
    };
    s.go_attack = [](void* c, Identity o, std::int32_t, std::int32_t) {
        fx(c)->attacked.push_back(o);
        return true;
    };
    s.script_anim_event = [](void* c, const char* n) {
        fx(c)->events.emplace_back(n);
        return true;
    };
    s.start_clip = [](void* c, const char* n, bool) {
        auto* f = fx(c);
        f->clips.emplace_back(n);
        if (!std::strcmp(n, "spawn")) return false;
        if (!std::strcmp(n, "remove")) return f->clip_exists_remove;
        return true;
    };
    s.play_sound = [](void*, std::int32_t) { return true; };
    s.delete_self = [](void* c) { ++fx(c)->deleted; return true; };
    return s;
}

void cycle() {
    Fixture f;
    const auto services = make(f);
    Config config;
    config.damager_id = 0;
    config.delay_ms = 1000;
    config.row_hurt_chance = 0;
    config.has_script = true;
    config.has_visual = true;
    Trap trap(config, 1, 0x99, 7);
    require(trap.hurt_chance() == 7, "Create's hurt override is returned");
    require(trap.init_post(services), "init_post");
    require(f.clips.size() == 2 && f.clips[0] == "spawn" && f.clips[1] == "idle" &&
            trap.state().idle_started && trap.state().timer_ms == 1000,
            "a BDAE without a spawn clip goes straight to idle");
    trap.observe(0x10, true);
    trap.observe(0x30, true);
    // 61 frames of 16 ms: the timer reaches 0 on the 63rd, so nothing yet.
    for (int i = 0; i < 62; ++i) require(trap.update(services), "update");
    require(!trap.state().active && f.rolled.empty(), "no activation before the delay");
    require(trap.update(services), "update");
    require(trap.state().active && f.clips.back() == "activate",
            "the elapsed timer activates the trap");
    // The same frame transferred and rolled the contact victims once.
    require(f.rolled.size() == 1 && f.rolled[0] == 0x10,
            "only Characters in the zone take an OnDamageRoll");
    require(trap.state().hit.count(0x10) && trap.state().hit.count(0x30) &&
            trap.state().victims.empty(), "victims move to the hit set");
    for (int i = 0; i < 10; ++i) require(trap.update(services), "update");
    require(f.rolled.size() == 1, "an object is struck once per activation");
    require(trap.clip_ended(services) && !trap.state().active && trap.state().hit.empty() &&
            f.clips.back() == "idle", "the animation end re-arms the trap");
    // A contact that left the zone is pruned.
    trap.observe(0x10, false);
    require(trap.state().contacts.count(0x10) == 0, "observe erases a rejected object");
    // Remove with a remove clip deletes at the clip end.
    require(trap.remove(services) && f.deleted == 0 && f.clips.back() == "remove",
            "_Remove plays the remove clip");
    require(trap.clip_ended(services) && f.deleted == 1, "the clip end deletes the trap");
    require(trap.clip_event(services, "inccounter") && f.events.size() == 1 &&
            f.events[0] == "inccounter", "events reach OnAnimEvent");
}

void no_visual_and_denied() {
    Fixture f;
    f.script_allows = false;
    const auto services = make(f);
    Config config;
    config.delay_ms = 32;
    config.has_script = false;
    config.has_visual = false;
    Trap trap(config, 0, 0x99, -1);
    require(trap.hurt_chance() == 0, "row hurt chance without an override");
    require(trap.init_post(services) && f.clips.empty(), "no visual starts no clip");
    trap.observe(0x10, true);
    for (int i = 0; i < 8; ++i) require(trap.update(services), "update");
    // Without a visual the activation never sets the active byte, so nothing
    // is ever transferred (TriggerTrap::Activate 0x39e198).
    require(!trap.state().active && f.attacked.empty() && f.triggered.empty(),
            "a visual-less trap never strikes");
    require(trap.remove(services) && f.deleted == 1, "_Remove without a visual deletes");

    Fixture g;
    g.script_allows = false;
    const auto gs = make(g);
    config.has_visual = true;
    Trap denied(config, 0, 0x99, -1);
    require(denied.init_post(gs), "init");
    denied.observe(0x10, true);
    for (int i = 0; i < 8; ++i) require(denied.update(gs), "update");
    require(denied.state().active && !g.triggered.empty() && g.rolled.empty() &&
            g.attacked.empty(), "OnTrigger false keeps the object out of the victims");
}

void tables(const std::string& root) {
    const auto data = read(root + "/game_objects_pyarray.bin");
    const auto names = read(root + "/game_objects_pyarraynames.bin");
    const auto fields = read(root + "/game_objects_pystructnames.bin");
    const auto dictionary = read(root + "/game_objects_dictionary_pyarray.bin");
    const auto dictionary_names = read(root + "/game_objects_dictionary_pyarraynames.bin");
    dh2::game_object_trap_tables_v1::Tables tables;
    std::string error;
    require(tables.load({data.data(), data.size()}, {names.data(), names.size()},
                        {fields.data(), fields.size()},
                        {dictionary.data(), dictionary.size()},
                        {dictionary_names.data(), dictionary_names.size()}, error),
            error.c_str());
    require(tables.timer_trap_count() == 2 && tables.damager_count() == 14,
            "TimerTraps / GameObjectDamager row counts differ");
    std::int32_t id = -1;
    const auto* trap = tables.find_timer_trap("SwampkingSteamCloud", &id);
    require(trap && id == 1 && trap->delay == 1000 && trap->hurt_chance == 0 &&
            trap->damage == 0 && trap->script == "swampking_trap" && trap->sound == -1,
            "SwampkingSteamCloud row differs");
    const auto* path = tables.visual_path(trap->visual);
    require(path && *path == "data/3D/GameObjects/go_trap_swampking_steam_cloud.bdae",
            "SwampkingSteamCloud visual path differs");
    const auto* damager = tables.find_damager("Level01Damage", &id);
    require(damager && id == 0 && damager->min == 5 && damager->max == 10 &&
            damager->damage == -1 && damager->immunity_delay == 0,
            "Level01Damage row differs");
    require(tables.find_timer_trap("__missing__") == nullptr, "unknown trap resolved");
}

struct ScriptFixture {
    std::vector<std::pair<std::string, std::string>> oids;
    std::vector<std::pair<std::int32_t, std::uintptr_t>> damages;
    int removed{};
};
void script(const std::string& path) {
    namespace ts = dh2::timer_trap_external_script_v1;
    const auto bytes = read(path);
    ScriptFixture f;
    ts::Services services;
    services.context = &f;
    services.get_py_oid = [](void* c, const char* cat, const char* member,
                             std::int32_t& value, std::string&) {
        static_cast<ScriptFixture*>(c)->oids.emplace_back(cat, member);
        value = 0;
        return true;
    };
    services.get_py_constant = [](void*, const char*, const char*, std::int32_t& value,
                                  std::string&) { value = 3; return true; };
    services.deal_damages = [](void* c, std::uintptr_t victim, std::int32_t damager,
                               bool& character, std::int32_t& amount, std::string&) {
        static_cast<ScriptFixture*>(c)->damages.emplace_back(damager, victim);
        character = true;
        amount = 7 << 8;
        return true;
    };
    services.remove = [](void* c, std::string&) {
        ++static_cast<ScriptFixture*>(c)->removed;
        return true;
    };
    ts::Owner owner;
    std::string error;
    require(owner.initialize({bytes.data(), bytes.size(), "data/scripts/objects/swampking_trap.luac"},
                             services, error) == ts::Status::complete, error.c_str());
    bool allowed = false;
    require(owner.on_trigger(0x10, allowed, error) == ts::Status::complete && allowed,
            "OnTrigger returns true");
    require(owner.on_damage_roll(0x10, error) == ts::Status::complete, error.c_str());
    require(f.damages.size() == 1 && f.damages[0].first == 0 && f.damages[0].second == 0x10,
            "OnDamageRoll calls DealDamages(victim, Level01Damage)");
    // OnAnimEvent counts "inccounter" and removes the trap on the sixth call.
    for (int i = 0; i < 5; ++i)
        require(owner.on_anim_event("inccounter", error) == ts::Status::complete && !f.removed,
                "the counter has not elapsed");
    require(owner.on_anim_event("other", error) == ts::Status::complete && !f.removed,
            "other events do not count");
    require(owner.on_anim_event("inccounter", error) == ts::Status::complete && f.removed == 1,
            "the sixth inccounter removes the trap");
}
} // namespace

int main(int argc, char** argv) {
    try {
        cycle();
        no_visual_and_denied();
        if (argc >= 2) tables(argv[1]);
        if (argc >= 3) script(argv[2]);
        std::cout << "timer trap source cycle ok tables=" << (argc >= 2 ? 1 : 0)
                  << " script=" << (argc >= 3 ? 1 : 0) << '\n';
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
