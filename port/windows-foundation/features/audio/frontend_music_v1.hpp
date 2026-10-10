#pragma once

#include <string>
#include <string_view>

// B064: frontend (title / main menu) music. Pure data and decision logic, no platform calls.
//
// Original evidence (dqmenus_droid.swf constant pools + ActionScript, IDA libDungeonHunter2.so):
//  - menu_splash ("touch the screen to continue", loading text MENU_LOADING_TITLE) requests
//    NativePlayMusic("TitleMusic") in its show block.
//  - menu_MainMenu.onPush (two builds of the movie, SWF offsets 272854 and 291257) calls
//    NativePlayMusic("TitleMusic") ("IN PUSH MM"); the credits menus do too.
//  - NativePlayMusic 0x43ad84 -> Arrays::GetMemberIDByString<Sounds> -> VoxSoundManager::PlayMusic
//    (0x36bd78) with loop=1, force=0, fade 2000 ms. Same id still playing => Resume(0.05 s), so a
//    repeat request never restarts the track. TitleMusic = sounds.xml uid 464, m_title.vxn, loop=yes.
//  - No other frontend screen (enter name, class select, options, slot menu, loading) has its own
//    music call: the title track simply keeps playing until the level's own PlayMusic (Level::Update,
//    fade 2000 ms) replaces it.
namespace dh::foundation::audio {

struct FrontendMusicRuleV1 {
    const char* screen;   // frontend screen id
    const char* track;    // sounds.xml label (Arrays::Sounds member name)
    int fade_ms;          // PlayMusic fade
};

inline constexpr FrontendMusicRuleV1 kFrontendMusicRulesV1[] = {
    {"title_splash", "TitleMusic", 2000},
    {"menu_MainMenu", "TitleMusic", 2000},
};

inline const FrontendMusicRuleV1* frontend_music_rule_v1(std::string_view screen) noexcept {
    for(const auto& rule : kFrontendMusicRulesV1)
        if(screen == rule.screen)return &rule;
    return nullptr;
}

// Frontend -> gameplay handoff. The original crossfades into the level music at the first Level::Update
// (PlayMusic fade 2000). The desktop gameplay audio owns a different output session, so the frontend
// track fades out over this time before that owner starts (documented deviation).
inline constexpr int kFrontendMusicHandoffFadeMs = 600;

// Remembers the wanted track until the output session can honour it (focused window, session started).
class FrontendMusicDirectorV1 {
public:
    struct Request {
        std::string screen;
        std::string track;
        int fade_ms{};
    };
    // Screens without a rule leave the current track untouched (original: only some screens call PlayMusic).
    void on_screen(std::string_view screen) {
        if(const auto* rule = frontend_music_rule_v1(screen)) {
            pending_ = Request{std::string(screen), rule->track, rule->fade_ms};
            has_pending_ = true;
        }
    }
    bool has_pending() const noexcept { return has_pending_; }
    const Request& pending() const noexcept { return pending_; }
    void applied() noexcept { has_pending_ = false; }
private:
    Request pending_;
    bool has_pending_{};
};

} // namespace dh::foundation::audio
