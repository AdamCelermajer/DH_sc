// B064: frontend title/main-menu music rules, request director and Navigator hook.
#include "frontend_music_v1.hpp"
#include "level_music_v1.hpp"
#include "../frontend/flow/menu_flow.hpp"
#include "../../../engine-audio/audio_sample_v34.hpp"
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace dh::foundation::audio;
namespace flow = dh::foundation::frontend::flow;
namespace {
int checks = 0;
void require(bool value, const char* why) { ++checks; if(!value) throw std::runtime_error(why); }
}

// Minimal VoxN with one IMA block (stereo, 32 kHz, block 1024); only the Afmt bits field varies.
std::vector<std::uint8_t> make_voxn(std::uint16_t bits, std::uint16_t format = 17) {
    std::vector<std::uint8_t> out;
    auto u32 = [&](std::uint32_t v) { for(int i = 0; i < 4; ++i) out.push_back(std::uint8_t(v >> (8 * i))); };
    auto u16 = [&](std::uint16_t v) { out.push_back(std::uint8_t(v)); out.push_back(std::uint8_t(v >> 8)); };
    auto tag = [&](const char* t, std::uint32_t n) { out.insert(out.end(), t, t + 4); u32(n); };
    tag("VoxN", 16); out.insert(out.end(), {'0', '.', '0', '.', '1', 0, 0, 0}); u32(0); u32(0);
    tag("Afmt", 12); u16(format); u16(2); u32(32000); u16(1024); u16(bits);
    tag("Segm", 28); u32(1); u32(0); u32(1024); u32(1017); u32(0); u32(0); u32(0);
    tag("Rule", 4); u32(0);
    tag("Plst", 12); u32(1); u32(0); u32(0);
    tag("Stat", 36); u32(1); u32(0); { char name[28] = "s"; out.insert(out.end(), name, name + 28); }
    tag("Trsn", 4); u32(0);
    tag("Grps", 4); u32(0);
    tag("Grpe", 4); u32(0);
    tag("Data", 1024); out.insert(out.end(), 1024, 0);
    const auto size = std::uint32_t(out.size());
    std::memcpy(out.data() + 16, &size, 4);
    return out;
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

    // m_title.vxn stores Afmt bits=16, the level banks 4; the original reader overwrites that field with 16.
    for(const auto bits : {std::uint16_t(4), std::uint16_t(16)}) {
        dh2::audio::AudioSampleV34 sample;
        auto bytes = std::make_shared<const std::vector<std::uint8_t>>(make_voxn(bits));
        require(dh2::audio::audio_sample_open_v34(bytes, sample, error) && sample.native && sample.format == 17 && sample.segments.size() == 1,
                "VoxN IMA with Afmt bits 4 or 16 opens");
    }
    {
        dh2::audio::AudioSampleV34 sample;
        auto bad_bits = std::make_shared<const std::vector<std::uint8_t>>(make_voxn(8));
        require(!dh2::audio::audio_sample_open_v34(bad_bits, sample, error), "other Afmt bits stay rejected");
        auto bad_codec = std::make_shared<const std::vector<std::uint8_t>>(make_voxn(16, 2));
        require(!dh2::audio::audio_sample_open_v34(bad_codec, sample, error), "unknown codec stays rejected");
    }

    std::cout << "PASS frontend_music_v1: " << checks << " assertions\n";
    return 0;
} catch(const std::exception& e) {
    std::cerr << "FAIL frontend_music_v1: " << e.what() << '\n';
    return 1;
}
