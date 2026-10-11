#include "death_checkpoint_load_v1.hpp"
#include "death_screen_runtime_v1.hpp"
#include "death_fade_from_black_v1.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace dh::foundation::persistence::death_flow;
using namespace dh::foundation::persistence::death_restart;

namespace {
struct Trace {
    std::vector<std::string> calls;
    std::string fail_at;
    bool character_present = true;
};

bool note(Trace& trace, const char* name, std::string& error) {
    trace.calls.emplace_back(name);
    if (trace.fail_at == name) {
        error = std::string("failed at ") + name;
        return false;
    }
    return true;
}
#define SIMPLE_CALLBACK(name) \
    bool name(void* p, std::string& error) { return note(*static_cast<Trace*>(p), #name, error); }
SIMPLE_CALLBACK(screen_black)
SIMPLE_CALLBACK(pop_fade)
SIMPLE_CALLBACK(push_fade)
SIMPLE_CALLBACK(load_world)
SIMPLE_CALLBACK(clear_buffs)
SIMPLE_CALLBACK(restore_position)
SIMPLE_CALLBACK(revive_character)
SIMPLE_CALLBACK(set_idle)
SIMPLE_CALLBACK(min_potions)
SIMPLE_CALLBACK(level_music)
SIMPLE_CALLBACK(previous_music)
#undef SIMPLE_CALLBACK

bool continue_hide(void* p, const char* label, std::string& error) {
    auto& trace = *static_cast<Trace*>(p);
    trace.calls.emplace_back(std::string("goto:") + (label ? label : "<null>"));
    if (trace.fail_at == "goto") { error = "failed at goto"; return false; }
    return true;
}

bool set_loading(void* p, bool value, std::string& error) {
    auto& trace = *static_cast<Trace*>(p);
    return note(trace, value ? "loading:true" : "loading:false", error);
}

bool resolve_character(void* p, bool& present, std::string& error) {
    auto& trace = *static_cast<Trace*>(p);
    if (!note(trace, "resolve_character", error)) return false;
    present = trace.character_present;
    return true;
}

void check(bool condition, const char* message) {
    if (!condition) { std::cerr << "FAIL: " << message << '\n'; std::exit(1); }
}

DeathAdmissionV1 admission(bool online = false) { return {true, true, true, true, online}; }

ReviveServicesV1 revive_services(Trace& t) {
    return {pop_fade, push_fade, load_world, clear_buffs, restore_position,
            revive_character, set_idle, min_potions, level_music, nullptr, &t};
}

DeathScreenRuntimeServicesV1 screen_services(Trace& t) {
    return {screen_black, &t, revive_services(t), previous_music, &t};
}

void test_visible_timeline_release_and_revive() {
    DeathFlowTicketV1 flow;
    std::string error;
    check(begin_death_flow_v1(admission(), flow, error), "offline death admission");
    DeathScreenRuntimeV1 ui;
    check(ui.start(flow, 2, error), "source screen starts with selected quote");
    Trace trace;
    const auto services = screen_services(trace);
    DeathScreenFrameV1 frame;
    check(ui.frame(frame, error) && frame.frame == 0 && !frame.continue_visible,
          "sprite742 begins at source frame0 with button off stage");
    for (unsigned i = 0; i < 64; ++i) check(ui.advance(true, services, error), "opening frame advance");
    check(ui.current_frame() == 64 && trace.calls == std::vector<std::string>{"screen_black"},
          "frame64 dispatches NativeScreenIsBlack once");
    for (unsigned i = 64; i < 75; ++i) check(ui.advance(true, services, error), "opening tail advance");
    check(ui.current_frame() == 75, "opening timeline reaches its authored stop");
    check(ui.advance(true, services, error) && ui.current_frame() == 75,
          "timeline remains stopped at frame75 until Continue");
    check(!ui.on_release(-20.f, -20.f, {continue_hide, &trace}, error),
          "outside pointer release does not activate Continue");
    check(ui.on_release(240.f, 160.f, {continue_hide, &trace}, error) &&
          ui.current_frame() == 76 && ui.hide_timeline(),
          "valid release jumps to Hide exactly once");
    check(!ui.on_release(240.f, 160.f, {continue_hide, &trace}, error),
          "duplicate Continue release is rejected");
    for (unsigned i = 76; i < 88; ++i) check(ui.advance(true, services, error), "Hide frame advance");
    check(ui.complete() && ui.current_frame() == 88 &&
          trace.calls == std::vector<std::string>{
              "screen_black", "goto:Hide", "pop_fade", "push_fade", "load_world",
              "clear_buffs", "restore_position", "revive_character", "set_idle",
              "min_potions", "level_music", "previous_music"},
          "frame88 executes native revive then separate previous-music callback in order");
    check(!ui.advance(true, services, error), "completed authored callback cannot replay");
}

void test_early_continue_skips_unreached_frame64() {
    DeathFlowTicketV1 flow;
    std::string error;
    check(begin_death_flow_v1(admission(), flow, error), "early-click admission");
    DeathScreenRuntimeV1 ui;
    check(ui.start(flow, 1, error), "early-click screen starts");
    Trace trace;
    check(ui.advance(true, screen_services(trace), error), "advance to visible Continue frame");
    check(ui.on_release(240.f, 160.f, {continue_hide, &trace}, error),
          "Continue is active before frame64");
    for (unsigned i = 76; i < 88; ++i) check(ui.advance(true, screen_services(trace), error), "early Hide advance");
    check(ui.complete() && trace.calls.size() == 11 && trace.calls[0] == "goto:Hide" &&
          trace.calls.back() == "previous_music",
          "jumping to Hide skips opening-only frame64 NativeScreenIsBlack action");
}

void test_online_and_provider_failure() {
    DeathFlowTicketV1 online;
    std::string error;
    check(begin_death_flow_v1(admission(true), online, error), "online timer admission");
    DeathScreenRuntimeV1 rejected;
    check(!rejected.start(online, 1, error), "online timer route cannot show offline screen");

    DeathFlowTicketV1 flow;
    check(begin_death_flow_v1(admission(), flow, error), "provider-failure admission");
    DeathScreenRuntimeV1 ui;
    check(ui.start(flow, 5, error), "provider-failure screen starts");
    Trace trace;
    trace.fail_at = "screen_black";
    const auto services = screen_services(trace);
    for (unsigned i = 0; i < 63; ++i) check(ui.advance(true, services, error), "advance before failed callback");
    check(!ui.advance(true, services, error) && ui.failed() && ui.current_frame() == 64,
          "failed source frame64 callback fail-stops without replay");
    check(!ui.advance(true, services, error) && trace.calls.size() == 1,
          "failed screen callback cannot be invoked twice");
}

void test_pointer_letterbox_projection() {
    float x = -1.f, y = -1.f;
    check(original_death_screen_to_stage_v1(500.f, 500.f, 1000, 1000, x, y) &&
          x == 240.f && y == 160.f,
          "window pointer is inverse-projected through exact 480x320 letterbox transform");
    check(!original_death_screen_to_stage_v1(500.f, 0.f, 1000, 1000, x, y),
          "letterbox bar is outside the source SWF stage");
    check(!original_death_screen_to_stage_v1(0.f, 0.f, 0, 320, x, y),
          "zero-sized viewport is rejected");
}

struct CheckpointServices {
    Trace* trace;
};
bool cp_set(void* p, bool value, std::string& e) { return set_loading(p, value, e); }
bool cp_world(void* p, std::string& e) { return note(*static_cast<Trace*>(p), "load_world", e); }
bool cp_events(void* p, std::string& e) { return note(*static_cast<Trace*>(p), "event_reinit", e); }
bool cp_character(void* p, bool& present, std::string& e) { return resolve_character(p, present, e); }
bool cp_qest_checkpoint(void* p, std::string& e) { return note(*static_cast<Trace*>(p), "load_checkpoint_qest16", e); }
bool cp_qest_fallback(void* p, std::string& e) { return note(*static_cast<Trace*>(p), "load_fallback_qest16", e); }
bool cp_update(void* p, std::string& e) { return note(*static_cast<Trace*>(p), "update1", e); }
DeathCheckpointLoadServicesV1 checkpoint_services(Trace& t, bool checkpoint_receiver = false) {
    DeathCheckpointLoadServicesV1 s;
    s.context = &t;
    s.player_checkpoint_savegame_receiver_present = checkpoint_receiver;
    s.set_level_checkpoint_loading = cp_set;
    s.load_level_checkpoint = cp_world;
    s.reinitialize_game_events = cp_events;
    s.resolve_local_character = cp_character;
    s.load_checkpoint_qest = cp_qest_checkpoint;
    s.load_qest_fallback = cp_qest_fallback;
    s.update_character_quests = cp_update;
    return s;
}

DeathCheckpointLoadTicketV1 checkpoint_ticket(bool level_savegame, std::string& error) {
    DeathCheckpointLoadTicketV1 ticket;
    check(begin_death_checkpoint_load_v1(true, level_savegame, ticket, error),
          "source gameplay checkpoint admission");
    return ticket;
}

void test_checkpoint_order_and_optional_source_owners() {
    std::string error;
    auto ticket = checkpoint_ticket(true, error);
    Trace trace;
    DeathCheckpointLoadReceiptV1 receipt;
    check(execute_death_checkpoint_load_v1(ticket, checkpoint_services(trace, true), receipt, error), error.c_str());
    check(receipt.complete && receipt.loading_flag_cleared && receipt.local_character_present &&
          trace.calls == std::vector<std::string>{
              "loading:true", "load_world", "event_reinit", "resolve_character",
              "load_checkpoint_qest16", "update1", "loading:false"},
          "checkpoint world/event/player-QEST/SG_Update order matches Level::LoadCheckpoint");
    check(!execute_death_checkpoint_load_v1(ticket, checkpoint_services(trace), receipt, error),
          "checkpoint load ticket cannot be replayed");

    auto no_world_ticket = checkpoint_ticket(false, error);
    Trace no_world;
    check(execute_death_checkpoint_load_v1(no_world_ticket, checkpoint_services(no_world), receipt, error), error.c_str());
    check(no_world.calls == std::vector<std::string>{
              "loading:true", "resolve_character", "load_fallback_qest16", "update1", "loading:false"},
          "missing LevelSavegame skips only source world-load and event-reinit branch");

    auto fallback_ticket = checkpoint_ticket(true, error);
    Trace fallback;
    check(execute_death_checkpoint_load_v1(fallback_ticket, checkpoint_services(fallback, false), receipt, error), error.c_str());
    check(std::find(fallback.calls.begin(), fallback.calls.end(), "load_fallback_qest16") != fallback.calls.end() &&
          std::find(fallback.calls.begin(), fallback.calls.end(), "load_checkpoint_qest16") == fallback.calls.end(),
          "missing PlayerSavegame checkpoint receiver takes source SG_Load(16) fallback");

    auto no_character_ticket = checkpoint_ticket(true, error);
    Trace no_character;
    no_character.character_present = false;
    check(execute_death_checkpoint_load_v1(no_character_ticket, checkpoint_services(no_character), receipt, error), error.c_str());
    check(no_character.calls == std::vector<std::string>{
              "loading:true", "load_world", "event_reinit", "resolve_character", "loading:false"},
          "missing local Character skips source checkpoint profile load and SG_Update");
}

void test_checkpoint_failure_prefix_and_cleanup() {
    for (const auto& failure : std::vector<std::string>{
             "loading:true", "load_world", "event_reinit", "resolve_character",
             "load_checkpoint_qest16", "load_fallback_qest16", "update1", "loading:false"}) {
        std::string error;
        auto ticket = checkpoint_ticket(true, error);
        Trace trace;
        trace.fail_at = failure;
        DeathCheckpointLoadReceiptV1 receipt;
        const bool ok = execute_death_checkpoint_load_v1(
            ticket, checkpoint_services(trace, failure != "load_fallback_qest16"), receipt, error);
        check(!ok && !receipt.complete, "checkpoint service failure is reported");
        if (failure == "loading:true") {
            check(trace.calls == std::vector<std::string>{"loading:true"} &&
                  !receipt.loading_flag_cleared,
                  "failure entering source loading flag performs no later effects");
        } else {
            check(!trace.calls.empty() && trace.calls.back() == "loading:false",
                  "failure after entering source load always attempts guard cleanup");
            check(receipt.loading_flag_cleared == (failure != "loading:false"),
                  "cleanup receipt reflects provider result");
        }
        check(!execute_death_checkpoint_load_v1(ticket, checkpoint_services(trace), receipt, error),
              "failed checkpoint prefix cannot be replayed");
    }
}

void test_invalid_checkpoint_admission() {
    DeathCheckpointLoadTicketV1 ticket;
    std::string error;
    check(!begin_death_checkpoint_load_v1(false, true, ticket, error) && !ticket.valid,
          "invalid current level rejects checkpoint load");
}

void test_source_fade_from_black_timeline() {
    constexpr std::array<std::uint8_t, 33> alpha{{
        255,248,240,232,224,216,208,200,192,184,176,
        168,160,152,144,136,128,120,112,104,96,88,
        80,72,64,56,48,40,32,24,16,8,0
    }};
    OriginalFadeFromBlackRuntimeV1 runtime;
    std::string error;
    check(runtime.start(error), "source sprite743 fade starts once after native push");
    FadeFromBlackFrameV1 frame;
    check(runtime.frame(frame, error) && frame.frame == 0 && frame.black_alpha == 255 &&
          !frame.pop_menu_callback,
          "sprite743 begins with the black cover fully opaque");
    for (std::uint8_t i = 1; i <= 32; ++i) {
        check(runtime.advance(frame, error), "sprite743 advances one authored frame");
        check(frame.frame == i && frame.black_alpha == alpha[i] &&
              frame.pop_menu_callback == (i == 32),
              "sprite743 frame alpha and frame32 native pop match the source timeline");
        if (i == 32) {
            check(!runtime.advance(frame, error),
                  "frame32 cannot advance before the native menu pop is serviced");
            check(runtime.confirm_native_pop(true, error) && runtime.popped(),
                  "frame32 performs one native menu pop");
        }
    }
    check(!runtime.advance(frame, error) && !runtime.confirm_native_pop(true, error),
          "a completed fade cannot be advanced or popped twice");

    OriginalFadeFromBlackRuntimeV1 failed_pop;
    check(failed_pop.start(error), "independent source fade instance starts");
    for (int i = 0; i < 32; ++i) check(failed_pop.advance(frame, error), "failed-pop fixture advances");
    check(!failed_pop.confirm_native_pop(false, error) && !failed_pop.advance(frame, error),
          "failed native pop is terminal and cannot silently remove the fade");
}
}

int main() {
    test_visible_timeline_release_and_revive();
    test_early_continue_skips_unreached_frame64();
    test_online_and_provider_failure();
    test_pointer_letterbox_projection();
    test_checkpoint_order_and_optional_source_owners();
    test_checkpoint_failure_prefix_and_cleanup();
    test_invalid_checkpoint_admission();
    test_source_fade_from_black_timeline();
    std::cout << "PASS: rendered-source death timeline, Continue/Hide/revive, and source-ordered checkpoint QEST load failure prefixes\n";
}
