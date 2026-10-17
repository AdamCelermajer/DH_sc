#pragma once

#include <memory>
#include <string>
#include <string_view>

struct AAssetManager;

namespace dh2::desktop_native {

// Desktop output adapter for the application's existing gameplay audio
// command queue. It owns only the SDL device and active output voices; source
// cue selection and ordering remain in player_gameplay_audio.
class GameplayAudioSdl {
public:
    GameplayAudioSdl();
    ~GameplayAudioSdl();
    GameplayAudioSdl(const GameplayAudioSdl&) = delete;
    GameplayAudioSdl& operator=(const GameplayAudioSdl&) = delete;

    bool open(std::string& error);
    void pump(AAssetManager* assets);
    bool play_menu_sound(AAssetManager* assets, std::string_view filename,
                         std::string& error);
    bool apply_menu_audio_command(std::string_view command, std::string& error);
    void close() noexcept;
    bool available() const noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace dh2::desktop_native
