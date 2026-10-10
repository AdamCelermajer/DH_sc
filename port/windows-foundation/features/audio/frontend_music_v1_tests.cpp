// B064: frontend title/main-menu music rules, request director and Navigator hook.
#include "frontend_music_v1.hpp"
#include "level_music_v1.hpp"
#include "../frontend/flow/menu_flow.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace dh::foundation::audio;
namespace flow = dh::foundation::frontend::flow;
namespace {
int checks = 0;
void require(bool value, const char* why) { ++checks; if(!value) throw std::runtime_error(why); }
}

int main() try {
    // Rule table: the screens whose authored AS calls NativePlayMusic("TitleMusic").
    for(const char* screen : {"title_splash", "menu_MainMenu"}) {
        const auto* rule = frontend_music_rule_v1(screen);
        require(rule && std::string(rule->track) == "TitleMusic" && rule->fade_ms == 2000, "screen has the title track with the PlayMusic fade 2000");
    }
    // No authored music call: the current track keeps playing (original behaviour), so no rule.
    for(const char* screen : {"menu_EnterName", "menu_SelectClass", "menu_StartGame", "menu_Options", "loading", ""})
        require(!frontend_music_rule_v1(screen), "screen without an authored PlayMusic has no rule");

    // Director: remembers until applied; a screen without a rule never clears or replaces the request.
    FrontendMusicDirectorV1 director;
    require(!director.has_pending(), "starts idle");
    director.on_screen("menu_EnterName");
    require(!director.has_pending(), "unruled screen requests nothing");
    director.on_screen("title_splash");
    require(director.has_pending() && director.pending().track == "TitleMusic" && director.pending().screen == "title_splash", "title request pending");
    director.on_screen("menu_SelectClass");
    require(director.has_pending() && director.pending().screen == "title_splash", "unruled screen keeps the pending request (output not ready yet)");
    director.on_screen("menu_MainMenu");
    require(director.pending().screen == "menu_MainMenu", "later request supersedes");
    director.applied();
    require(!director.has_pending(), "applied request is consumed (no restart on later frames)");

    // Navigator hook: initial main menu, push, back, duplicate top.
    std::vector<std::string> entered;
    flow::Services services;
    services.menu_entered = [&](const char* menu) { entered.push_back(menu); };
    std::string error;
    flow::Navigator navigator(services);
    require(entered.size() == 1 && entered[0] == "menu_MainMenu", "initial main menu announced by the constructor");
    require(navigator.single_player({0, false, "slot-0.sav"}, error) && entered.size() == 2 && entered[1] == "menu_EnterName", "push announces the new top");
    require(navigator.back(error) && entered.size() == 3 && entered[2] == "menu_MainMenu", "back announces the revealed main menu");
    require(navigator.go_to_main_menu(0, error) && entered.size() == 3, "going to the main menu while it is on top is not a new entry");
    // Wiring the hook through the director: only main menu starts music.
    FrontendMusicDirectorV1 wired;
    for(const auto& menu : entered) wired.on_screen(menu);
    require(wired.has_pending() && wired.pending().screen == "menu_MainMenu", "main menu is the last ruled entry");

    // One transition line format for every owner.
    require(music_transition_line_v1("Frontend", "start", "TitleMusic", 2000, "screen=title_splash") ==
            "Frontend music transition: kind=start track=TitleMusic fadeMs=2000 screen=title_splash", "frontend transition line");
    require(level_music_transition_line_v1("start", "SwampHubAmbientMusic", 2000, "") ==
            "Level music transition: kind=start track=SwampHubAmbientMusic fadeMs=2000", "level line unchanged");
    require(kFrontendMusicHandoffFadeMs > 0 && kFrontendMusicHandoffFadeMs <= 2000, "handoff fade is bounded");

    std::cout << "PASS frontend_music_v1: " << checks << " assertions\n";
    return 0;
} catch(const std::exception& e) {
    std::cerr << "FAIL frontend_music_v1: " << e.what() << '\n';
    return 1;
}
