#include "death_flow_v1.hpp"
#include "death_screen_v1.hpp"
#include "death_screen_art_geometry_v1.hpp"

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
};

bool record(Trace& trace, const char* name, std::string& error) {
    trace.calls.emplace_back(name);
    if (trace.fail_at == name) {
        error = std::string("provider failed at ") + name;
        return false;
    }
    return true;
}
#define CALLBACK(name) \
    bool name(void* context, std::string& error) { \
        return record(*static_cast<Trace*>(context), #name, error); \
    }
CALLBACK(pop_fade_to_black)
CALLBACK(push_fade_from_black)
CALLBACK(load_level_checkpoint)
CALLBACK(clear_character_buffs)
CALLBACK(restore_revive_position)
CALLBACK(revive_to_source_vitals)
CALLBACK(set_character_idle)
CALLBACK(ensure_minimum_potions)
CALLBACK(restart_level_music)
CALLBACK(restart_previous_music)
CALLBACK(notify_online_handoff)
#undef CALLBACK

bool goto_parent_label(void* context, const char* label, std::string& error) {
    auto& trace = *static_cast<Trace*>(context);
    trace.calls.emplace_back(std::string("goto:") + (label ? label : "<null>"));
    if (trace.fail_at == "goto") {
        error = "provider failed at goto";
        return false;
    }
    return true;
}

void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}

DeathAdmissionV1 offline_admission() {
    return {true, true, true, true, false};
}

ContinueServicesV1 continue_services(Trace& t) {
    return {goto_parent_label, &t};
}

ReviveServicesV1 revive_services(Trace& t) {
    return {pop_fade_to_black, push_fade_from_black, load_level_checkpoint,
            clear_character_buffs, restore_revive_position,
            revive_to_source_vitals, set_character_idle, ensure_minimum_potions,
            restart_level_music, notify_online_handoff, &t};
}

void test_admission_gates() {
    const DeathAdmissionV1 base = offline_admission();
    for (int gate = 0; gate < 4; ++gate) {
        DeathAdmissionV1 in = base;
        if (gate == 0) in.gameplay_level_type_38 = false;
        if (gate == 1) in.local_character_assigned = false;
        if (gate == 2) in.character_dead = false;
        if (gate == 3) in.player_manager_phase_active = false;
        DeathFlowTicketV1 ticket;
        std::string error;
        require(!begin_death_flow_v1(in, ticket, error) && !ticket.valid,
                "inactive source admission gate rejects death flow");
    }
}

void test_source_death_screen_manifest() {
    const auto& timeline = original_death_screen_timeline_v1();
    require(timeline.swf_sprite == 742 && timeline.frame_rate == 30 &&
            timeline.stage_width_twips == 9600 && timeline.stage_height_twips == 6400,
            "death screen manifest retains source sprite/stage timing");
    require(std::string(timeline.continue_label) == "Hide" &&
            timeline.continue_goto_frame == 76 && timeline.revive_callback_frame == 88,
            "Continue is the exact Hide label before the authored revive frame");
    const auto& art = original_death_screen_art_v1();
    require(art.size() == 5 && art[0].swf_character == 33 &&
            art[0].local_bounds_twips[1] == 9887 && art[3].swf_character == 739 &&
            art[4].swf_character == 741,
            "manifest maps source hitbox, panel art, and quote objects");
    require(art_data::kPanelFill1.size() == 7668 &&
            art_data::kPanelFill2.size() == 12 && art_data::kFade.size() == 6,
            "render data contains source shape739 black/red fills and shape496 fade triangles");

    DeathScreenFrameV1 frame;
    std::string error;
    require(project_original_death_screen_frame_v1(64, false, 3, frame, error) &&
            frame.screen_black_callback && frame.black_alpha == 255 &&
            frame.continue_visible && frame.quote_number == 3 && frame.draw_count == 3 &&
            frame.draw_items[0].role == DeathScreenArtRoleV1::black_fade_overlay &&
            frame.draw_items[1].alpha == 255 && frame.draw_items[2].alpha == 0,
            "frame64 projects depth-ordered fade/panel/hidden quote and black callback");
    require(project_original_death_screen_frame_v1(1, false, 3, frame, error) &&
            frame.draw_items[1].alpha == 8 && frame.black_alpha == 0,
            "frame1 begins source panel fade-in while black transition stays transparent");
    require(project_original_death_screen_frame_v1(65, false, 3, frame, error) &&
            frame.draw_items[2].alpha == 26,
            "frame65 starts localized quote fade-in over the black background");
    require(project_original_death_screen_frame_v1(74, false, 3, frame, error) &&
            frame.draw_items[2].alpha == 255,
            "frame74 completes localized quote fade-in");
    require(project_original_death_screen_frame_v1(46, false, 3, frame, error) &&
            frame.black_alpha == 13 && frame.draw_items[1].alpha == 255 &&
            project_original_death_screen_frame_v1(75, false, 3, frame, error) &&
            frame.stopped && frame.black_alpha == 255,
            "black fade starts at frame46 and the settled screen stops at frame75");
    require(original_death_continue_hit_test_v1(240.f, 160.f, 64) &&
            !original_death_continue_hit_test_v1(240.f, 160.f, 0) &&
            !original_death_continue_hit_test_v1(240.f, 160.f, 76),
            "large source Continue hit target is active only in its authored display frames");
    constexpr std::array<std::uint8_t, 7> hide_quote_alpha{{219, 183, 146, 110, 73, 37, 0}};
    for (std::uint16_t i = 0; i < hide_quote_alpha.size(); ++i) {
        require(project_original_death_screen_frame_v1(
                    static_cast<std::uint16_t>(82 + i), true, 3, frame, error) &&
                frame.black_alpha == 255 && frame.draw_items[2].alpha == hide_quote_alpha[i],
                "Hide frames82-88 fade the quote while the black overlay remains opaque");
        if (i < 6) require(!frame.revive_callback,
                           "Hide frames82-87 have not invoked revive yet");
    }
    require(project_original_death_screen_frame_v1(88, true, 3, frame, error) &&
            frame.revive_callback && frame.previous_music_callback && frame.black_alpha == 255 &&
            frame.draw_items[2].alpha == 0,
            "Hide frame88 fades the quote to zero while black cover remains until native pop");
    require(!project_original_death_screen_frame_v1(88, true, 6, frame, error),
            "invalid localized quote selector is rejected");
}

void test_continue_then_authored_revive() {
    DeathFlowTicketV1 flow;
    std::string error;
    require(begin_death_flow_v1(offline_admission(), flow, error),
            "offline death passes source gates");
    Trace trace;
    auto ui = continue_services(trace);
    require(!run_authored_revive_v1(flow, true, revive_services(trace),
                                    restart_previous_music, &trace, error),
            "frame-88 revive cannot run before Continue");
    require(trace.calls.empty(), "pre-Continue callback has no side effects");
    require(activate_continue_v1(flow, ui, error), "Continue action succeeds");
    const std::vector<std::string> prefix{"goto:Hide"};
    require(trace.calls == prefix, "Continue performs one parent.gotoAndPlay(\"Hide\") action");
    require(!activate_continue_v1(flow, ui, error), "duplicate Continue is rejected");
    require(trace.calls == prefix, "duplicate Continue has no repeated effects");
    require(run_authored_revive_v1(flow, true, revive_services(trace),
                                   restart_previous_music, &trace, error),
            "authored frame-88 callback succeeds after Continue");
    const std::vector<std::string> expected{
        "goto:Hide", "pop_fade_to_black",
        "push_fade_from_black", "load_level_checkpoint", "clear_character_buffs",
        "restore_revive_position", "revive_to_source_vitals", "set_character_idle",
        "ensure_minimum_potions", "restart_level_music", "restart_previous_music"};
    require(trace.calls == expected,
            "frame-88 callback preserves checkpoint/Character/music effect order");
    require(!run_authored_revive_v1(flow, true, revive_services(trace),
                                    restart_previous_music, &trace, error),
            "duplicate frame-88 callback is rejected");
    require(trace.calls == expected, "duplicate frame-88 has no repeated effects");
}

void test_continue_failure_is_not_replayed() {
    DeathFlowTicketV1 flow;
    std::string error;
    require(begin_death_flow_v1(offline_admission(), flow, error),
            "failure test admission succeeds");
    Trace trace;
    trace.fail_at = "goto";
    auto ui = continue_services(trace);
    require(!activate_continue_v1(flow, ui, error), "failed parent label call stops flow");
    require(trace.calls == std::vector<std::string>{"goto:Hide"},
            "failed timeline call is a single source action");
    trace.fail_at.clear();
    require(!activate_continue_v1(flow, ui, error),
            "failed Continue action cannot be replayed");
    require(trace.calls == std::vector<std::string>{"goto:Hide"},
            "failed Continue remains duplicate-safe");
}

void test_revive_failure_and_invalid_level() {
    DeathFlowTicketV1 flow;
    std::string error;
    require(begin_death_flow_v1(offline_admission(), flow, error),
            "revive failure test admission succeeds");
    Trace trace;
    auto ui = continue_services(trace);
    require(activate_continue_v1(flow, ui, error), "Continue succeeds before revive failure");
    trace.calls.clear();
    require(!run_authored_revive_v1(flow, false, revive_services(trace),
                                    restart_previous_music, &trace, error),
            "callback rejects a non-gameplay Level");
    require(trace.calls.empty(), "invalid Level has no source effects");

    DeathFlowTicketV1 failure_flow;
    require(begin_death_flow_v1(offline_admission(), failure_flow, error),
            "provider failure admission succeeds");
    require(activate_continue_v1(failure_flow, ui, error), "Continue enables callback");
    trace.calls.clear();
    trace.fail_at = "load_level_checkpoint";
    require(!run_authored_revive_v1(failure_flow, true, revive_services(trace),
                                    restart_previous_music, &trace, error),
            "checkpoint provider failure stops revive prefix");
    require(trace.calls == std::vector<std::string>{"pop_fade_to_black",
            "push_fade_from_black", "load_level_checkpoint"},
            "failed checkpoint cannot reach Character or music providers");
    trace.fail_at.clear();
    require(!run_authored_revive_v1(failure_flow, true, revive_services(trace),
                                    restart_previous_music, &trace, error),
            "failed frame-88 prefix cannot replay");
}

void test_online_route_excludes_offline_continue() {
    DeathAdmissionV1 in{true, true, true, true, true};
    DeathFlowTicketV1 flow;
    std::string error;
    require(begin_death_flow_v1(in, flow, error) &&
            flow.route == LocalDeathRouteV1::start_online_death_timer,
            "online local death selects its timer route");
    Trace trace;
    auto ui = continue_services(trace);
    require(!activate_continue_v1(flow, ui, error),
            "online local timer does not enter offline Continue UI");
    require(trace.calls.empty(), "online route does not call offline UI providers");
}
}

int main() {
    test_source_death_screen_manifest();
    test_admission_gates();
    test_continue_then_authored_revive();
    test_continue_failure_is_not_replayed();
    test_revive_failure_and_invalid_level();
    test_online_route_excludes_offline_continue();
    std::cout << "PASS: exact parent.gotoAndPlay(\"Hide\")→frame-88 revive order, source UI mapping, admission, online exclusion, failures, and duplicate suppression\n";
    return 0;
}
