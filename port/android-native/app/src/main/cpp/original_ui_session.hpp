#pragma once
#include <android/asset_manager.h>
#include <memory>
#include <string>
#include <cstdint>
#include "item_presentation_v5.hpp"
#include "../../../../../engine-ui/game_option_table_v1.hpp"

namespace dh2::android_ui {
// Source NativeStartGame arguments captured after the authored Assign call.
// This is only an asynchronous delivery record; PlayerInfo/save/profile remain
// owned by the existing native runtime.
struct NativeStartGameIntentV1 {
    std::int32_t slot{-1};
    bool has_numeric_difficulty{};
    std::int32_t requested_difficulty{};
};
// Synchronous borrowed source services. Creation publishes a result only after
// the original caller reaches persistence; invalid class is success/no-result.
// The real authored Start button calls Assign before NativeStartGame. The
// latter invokes an explicit development Crypt continuation over the actual
// assigned owner; the full original NativeStartGame Save/Level body is open.
// World loading occurs only after the GL owner's menu delivery unwinds.
struct FrontRuntimeServices {
    void* context{};
    bool (*create_save_slot)(void*,const std::string& name,const std::string& character,
                            std::int32_t& slot,bool& publish_result,std::string&){};
    bool (*assign_save_slot)(void*,std::int32_t slot,std::int32_t ordinal,std::string&){};
    bool (*eabi_integer)(void*,double,std::int32_t&,std::string&){};
    bool (*request_start_game)(void*,bool has_numeric_difficulty,std::int32_t requested_difficulty,
                               std::int32_t& selected_slot,std::string&){};
    // Source FS_ContinueGame reads SavegameManager+8 and queues the same
    // deferred saved-Level transition as NativeStartGame's port continuation.
    bool (*request_continue_game)(void*,std::int32_t& last_slot,std::string&){};
    // Borrow the sole native DebugSwitches owner, including its fresh source
    // load/query effects. The frontend never constructs another debug map.
    bool (*debug_load)(void*,std::string&){};
    bool (*debug_query)(void*,const char* key,std::string&){};
    bool (*change_preview_slot)(void*,std::int32_t slot,bool force,std::string&){};
    // Source NativeSaveGame resolves this local Character and calls its same
    // PlayerSavegame::SG_Save owner synchronously on the GL/UI thread.
    bool (*save_game)(void*,std::uintptr_t character,std::string&){};
    // NativeLockZoom/NativeUnlockZoom mutate +134 on the currently bound
    // CameraLevel; CameraLevel itself remains owned by the world renderer.
    void (*camera_zoom_lock)(void*,bool locked){};
};
// Retained authored HUD owner. Movie/font/texture CPU state survives GL
// recreation; the connected status view borrows actual world properties.
class OriginalUiSession {
public:
    OriginalUiSession();
    ~OriginalUiSession();
    OriginalUiSession(const OriginalUiSession&)=delete;
    OriginalUiSession& operator=(const OriginalUiSession&)=delete;
    bool initialize(AAssetManager*,std::string&);
    void bind_front_runtime(const FrontRuntimeServices&);
    bool consume_launch_request(NativeStartGameIntentV1& request);
    // Consume the authored gameplay HUD's deferred main-menu request after
    // the current SWF input dispatch has unwound.
    bool consume_gameplay_main_menu_request() noexcept;
    bool game_difficulty_count(std::uint32_t& count,std::string&);
    // Borrow row zero from the one retained Application DesignSettings owner.
    // Valid while the main/gameplay session keeps its GameOption snapshot.
    dh2::ui::GameOptionBytesV1 design_settings_table() const noexcept;
    // Borrow the same persisted source Language value used by GSInit when it
    // selects the opening movie and MyVideoView subtitle table.
    std::int32_t language() const noexcept;
    bool load_front_screen(const std::string& private_directory,const std::string& screen,std::string&);
    bool load_health_panel(const std::string& private_directory,std::string&);
    bool attach_player(const std::string& private_directory,std::string&);
    bool render_player(int width,int height,const std::int32_t*,std::size_t,
                       std::uintptr_t character,std::string&);
    // Source FlashAnimManager text is rendered by the retained dqhud movie;
    // callers supply its already projected authored-stage position and color.
    bool play_authored_animation_text(const char* style,std::uint32_t slot,
        float x_twips,float y_twips,const char* text,std::uint8_t r,std::uint8_t g,
        std::uint8_t b,std::uint8_t a,std::string&);
    bool play_authored_animation_screen(const char* style,std::uint32_t slot,
        float screen_x_px,float screen_y_px,const char* text,std::uint8_t r,
        std::uint8_t g,std::uint8_t b,std::uint8_t a,std::string&);
    bool authored_animation_style_id(const char* style,std::int32_t& id,std::string&);
    bool source_combat_text_constant(const char* group,const char* name,
                                     std::int32_t& value,std::string&);
    bool source_combat_text_string(std::int32_t string_id,std::string&,
                                   std::string&);
    bool source_combat_text_format_int(std::int32_t string_id,std::int32_t argument,
                                       std::string&,std::string&);
    bool render(int width,int height,std::string&);
    std::string consume_menu_sound(); // GL owner thread; drains one source request
    std::string consume_menu_audio(); // GL owner -> Android audio control delivery
    bool debug_menu_sound(const std::string& probe,std::string&);
    // Returns a descriptor which borrows this session's live StringManager
    // cache and the supplied immutable Item/Character tables. The returned
    // context remains valid until this session is destroyed or rebound.
    data::ItemTextServicesV5 item_text_services(const data::ItemTable&,
                                               const data::CharacterTable&) noexcept;
    bool touch(float x,float y,int action,std::string&);
    bool touch(float x,float y,int action,std::uint32_t cursor_index,std::string&);
    // Android keyboard events are accepted only while the authored name
    // entry menu owns the front stack; printable text still uses GameSWF's
    // focused editable-text path.
    bool menu_text_input_active() const noexcept;
    bool menu_key_event(std::uint32_t key_code,bool down,std::string&);
    bool camera_pan_allowed() const noexcept;
    // Android system Back uses the same live gameplay menu owner as the
    // authored NativeBackToHud callback; it never reloads the selected level.
    bool back_to_hud(std::string&);
    bool active() const;
    bool overlays_player() const;
    void deactivate();
    // Re-enable the retained authored front renderer after a failed deferred
    // NativeStartGame continuation without resetting its current menu stack.
    bool resume_front_after_start_failure(std::string&);
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
}
