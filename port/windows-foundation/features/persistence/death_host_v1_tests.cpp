#include "death_host_v1.hpp"

#include <iostream>
#include <string>
#include <vector>

using namespace dh::foundation::persistence::death_host;

namespace {
struct Trace {
    std::vector<std::string> calls;
    std::string fail_at;
};

bool note(void* raw, const char* name, std::string& error) {
    auto& trace = *static_cast<Trace*>(raw);
    trace.calls.emplace_back(name);
    if (trace.fail_at == name) {
        error = std::string("failed at ") + name;
        return false;
    }
    error.clear();
    return true;
}

bool fx_muted(void* raw, bool muted, std::string& error) {
    return note(raw, muted ? "fx_mute" : "fx_restore", error);
}
bool black(void* raw, std::string& error) { return note(raw, "black", error); }
bool load(void* raw, std::string& error) { return note(raw, "load", error); }
bool buffs(void* raw, std::string& error) { return note(raw, "buffs", error); }
bool position(void* raw, std::string& error) { return note(raw, "position", error); }
bool vitals(void* raw, std::string& error) { return note(raw, "vitals", error); }
bool idle(void* raw, std::string& error) { return note(raw, "idle", error); }
bool potions(void* raw, std::string& error) { return note(raw, "potions", error); }
bool level_music(void* raw, std::string& error) { return note(raw, "level_music", error); }
bool previous_music(void* raw, std::string& error) { return note(raw, "previous_music", error); }
bool pop_fade(void* raw, std::string& error) { return note(raw, "pop_fade", error); }

ServicesV1 make_services(Trace& trace) {
    ServicesV1 s;
    s.context = &trace;
    s.set_fx_muted = &fx_muted;
    s.native_screen_is_black = &black;
    s.load_level_checkpoint = &load;
    s.clear_character_buffs = &buffs;
    s.restore_revive_position = &position;
    s.revive_to_source_vitals = &vitals;
    s.set_character_idle = &idle;
    s.ensure_minimum_potions = &potions;
    s.restart_level_music = &level_music;
    s.native_play_previous_music = &previous_music;
    s.pop_fade_from_black = &pop_fade;
    return s;
}

AdmissionV1 dead_ready() {
    AdmissionV1 a;
    a.level_ready = true;
    a.player_manager_phase_active = true;
    a.local_character_assigned = true;
    a.local_character_dead = true;
    return a;
}

int failures = 0;
void check(bool ok, const std::string& what) {
    if (!ok) {
        ++failures;
        std::cout << "FAIL: " << what << "\n";
    }
}
int count(const Trace& t, const char* name) {
    int n = 0;
    for (const auto& c : t.calls) n += c == name;
    return n;
}
// One authored tick is 1/30 s; the epsilon absorbs floating-point accumulation.
void ticks(DeathHostV1& host, int n, bool ready, const ServicesV1& s, std::string& e) {
    for (int i = 0; i < n; ++i) host.advance(1.0 / 30.0 + 1e-7, ready, s, e);
}

void test_gate_and_single_admission() {
    Trace t;
    auto s = make_services(t);
    DeathHostV1 host;
    std::string e;
    AdmissionV1 alive = dead_ready();
    alive.local_character_dead = false;
    check(!host.observe(alive, 1, s, e) && e.empty() && !host.modal(), "alive player is not a death");
    AdmissionV1 notReady = dead_ready();
    notReady.level_ready = false;
    check(!host.observe(notReady, 1, s, e) && !e.empty() && !host.modal(), "dead player outside READY Level is rejected");
    check(count(t, "fx_mute") == 0, "rejected gate mutes nothing");
    AdmissionV1 online = dead_ready();
    online.online = true;
    check(!host.observe(online, 1, s, e) && !e.empty(), "online route is not admitted by the offline host");
    check(host.observe(dead_ready(), 3, s, e) && e.empty(), "dead READY player starts the death flow");
    check(host.death_screen_visible() && host.modal() && host.fx_muted(), "death screen is modal and FX muted");
    check(count(t, "fx_mute") == 1, "FX muted once");
    check(!host.observe(dead_ready(), 3, s, e) && count(t, "fx_mute") == 1, "duplicate observation does not replay the start");
}

void test_timeline_frame64_and_continue_revive_order() {
    Trace t;
    auto s = make_services(t);
    DeathHostV1 host;
    std::string e;
    host.observe(dead_ready(), 2, s, e);
    ticks(host, 63, true, s, e);
    check(count(t, "black") == 0, "no NativeScreenIsBlack before authored frame64");
    ticks(host, 1, true, s, e);
    check(count(t, "black") == 1 && host.screen_black_seen(), "frame64 NativeScreenIsBlack fires once");

    // A release in the letterbox bars of a 1000x320 window is outside the stage and absorbed.
    check(!host.pointer_release(5.f, 5.f, 1000, 320, s, e) && host.death_screen_visible(), "letterbox release is absorbed");
    check(host.pointer_release(240.f, 160.f, 480, 320, s, e), "stage release triggers Continue");
    check(!host.pointer_release(240.f, 160.f, 480, 320, s, e), "duplicate Continue does not replay");
    check(!host.continue_key(s, e), "duplicate Space Continue does not replay");
    ticks(host, 11, true, s, e); // Hide timeline frames 77..87
    check(count(t, "load") == 0, "revive waits for authored frame88");
    ticks(host, 1, true, s, e);  // frame 88 runs the authored revive
    const std::vector<std::string> expected{"fx_mute", "black", "load", "fx_restore", "buffs", "position",
                                            "vitals", "idle", "potions", "level_music", "previous_music"};
    check(t.calls == expected, "revive order: LoadCheckpoint, FX restore, buffs, position, vitals, idle, potions, level music, previous music");
    check(host.fade_visible() && host.phase() == PhaseV1::fade_from_black, "frame88 starts the sprite743 fade");
    check(!host.death_screen_visible(), "the death screen is popped by the revive");
    ticks(host, 5, true, s, e);
    check(count(t, "load") == 1 && count(t, "previous_music") == 1, "revive callbacks never replay");
}

void test_early_continue_skips_frame64() {
    Trace t;
    auto s = make_services(t);
    DeathHostV1 host;
    std::string e;
    host.observe(dead_ready(), 4, s, e);
    ticks(host, 3, true, s, e);
    check(host.pointer_release(240.f, 160.f, 480, 320, s, e), "early Continue at frame3 is accepted");
    ticks(host, 12, true, s, e); // 76..88
    check(count(t, "black") == 0, "early Continue skips the unvisited frame64 callback");
    check(count(t, "load") == 1 && count(t, "previous_music") == 1, "early Continue still revives once");
}

void test_space_is_continue() {
    Trace t;
    auto s = make_services(t);
    DeathHostV1 host;
    std::string e;
    host.observe(dead_ready(), 5, s, e);
    ticks(host, 10, true, s, e);
    check(host.continue_key(s, e), "Space (PC adaptation) fires Continue");
    check(!host.continue_key(s, e), "Space repeated after Continue is absorbed");
}

void test_revive_failure_fails_closed() {
    Trace t;
    t.fail_at = "vitals";
    auto s = make_services(t);
    DeathHostV1 host;
    std::string e;
    host.observe(dead_ready(), 1, s, e);
    ticks(host, 64, true, s, e);
    host.continue_key(s, e);
    ticks(host, 12, true, s, e);
    check(host.failed() && !host.modal(), "a failed revive latches and releases the modal");
    check(count(t, "idle") == 0 && count(t, "potions") == 0 && count(t, "previous_music") == 0,
          "failure stops later revive callbacks");
    check(!host.observe(dead_ready(), 1, s, e) && !e.empty(), "a failed host refuses a new flow");
}

void test_fade_pops_once_at_frame32_and_second_death() {
    Trace t;
    auto s = make_services(t);
    DeathHostV1 host;
    std::string e;
    host.observe(dead_ready(), 1, s, e);
    ticks(host, 64, true, s, e);
    host.continue_key(s, e);
    ticks(host, 12, true, s, e);
    check(host.fade_visible(), "fade visible after revive");
    ticks(host, 31, true, s, e);
    check(count(t, "pop_fade") == 0 && host.fade_visible(), "fade has not popped before frame32");
    ticks(host, 1, true, s, e);
    check(count(t, "pop_fade") == 1 && !host.modal() && host.phase() == PhaseV1::idle, "frame32 pops the fade once");
    ticks(host, 40, true, s, e);
    check(count(t, "pop_fade") == 1, "no pop replays after the fade ends");
    check(host.observe(dead_ready(), 2, s, e) && count(t, "fx_mute") == 2, "a second death opens a fresh flow with FX muted again");
    check(host.death_screen_visible() && count(t, "black") == 1, "the second flow starts from a fresh timeline");
}

void test_wall_clock_is_thirty_hz() {
    Trace t;
    auto s = make_services(t);
    DeathHostV1 host;
    std::string e;
    host.observe(dead_ready(), 1, s, e);
    for (int i = 0; i < 190; ++i) host.advance(0.01, true, s, e); // 1.9 s -> 57 ticks
    check(count(t, "black") == 0, "57 authored ticks have not reached frame64");
    for (int i = 0; i < 100; ++i) host.advance(0.01, true, s, e); // +1.0 s -> 87 ticks
    check(count(t, "black") == 1 && host.death_screen_visible(), "wall-clock ticks reach frame64 once and stop at frame75 without Continue");
    check(count(t, "load") == 0, "no revive without Continue");
}

void test_non_ready_level_at_frame88_fails_closed() {
    Trace t;
    auto s = make_services(t);
    DeathHostV1 host;
    std::string e;
    host.observe(dead_ready(), 1, s, e);
    ticks(host, 64, true, s, e);
    host.continue_key(s, e);
    ticks(host, 11, false, s, e);
    check(!host.failed() && count(t, "load") == 0, "no revive before frame88 even without READY");
    ticks(host, 1, false, s, e);
    check(host.failed() && count(t, "load") == 0 && !host.modal(), "non-READY Level at frame88 fails closed before LoadCheckpoint");
}
} // namespace

int main() {
    test_gate_and_single_admission();
    test_timeline_frame64_and_continue_revive_order();
    test_early_continue_skips_frame64();
    test_space_is_continue();
    test_revive_failure_fails_closed();
    test_fade_pops_once_at_frame32_and_second_death();
    test_wall_clock_is_thirty_hz();
    test_non_ready_level_at_frame88_fails_closed();
    if (failures) {
        std::cout << failures << " death host check(s) failed\n";
        return 1;
    }
    std::cout << "PASS death_host_v1\n";
    return 0;
}
