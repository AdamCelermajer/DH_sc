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
    enum class Kind : std::int32_t { start_game = 0, level_transition = 1 };
    std::int32_t slot{-1};
    bool has_numeric_difficulty{};
    std::int32_t requested_difficulty{};
    Kind kind{Kind::start_game};
    std::string destination_name;
    double raw_entry_point{};
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
    // NativeReviveAllPlayers is an authored zero-argument HUD action. The
    // retained renderer provides the current source-level generation check.
    bool (*revive_local_players)(void*,std::string&){};
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
    // Source MenuManager::DisplayFastTravel callback invoked by
    // NativeGoToZone after its save/slot/difficulty prefix has completed.
    bool display_fast_travel(bool source_flag,const std::string& level_name,
        const std::string& repeated_level_name,std::int32_t entry_point,
        std::string& error);
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
    // Source ItemObject::Interact StatusMsg producers and the authored HUD
    // queue share this retained session and canonical StringManager.
    bool enqueue_source_status_item_pickup(const std::string& item_name,
        std::int32_t power_count,std::string& error);
    bool enqueue_source_status_auto_transmute(const std::string& item_name,
        std::int32_t power_count,std::int32_t payout,std::string& error);
    bool enqueue_source_status_constant(const char* group,const char* key,
        std::string& error);
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
    // True when the camera may consume finger `slot` (0..3): no menu owns the
    // press (the map menu excepted) and the HUD gameplay layer is active.
    bool camera_touch_allowed(std::uint32_t slot) const noexcept;
    // One finger event of a multi-touch gesture. `slot` is the source finger
    // index (0..3). The HUD/menus see it first through touch(); fingers they
    // leave alone then drive the camera pan/pinch, as ZoomHandler does.
    bool pointer(float x,float y,int action,std::uint32_t slot,std::string&);
    // Development probe: camera gestures pointer() has handed to the CameraLevel
    // (pans delivered / applied, pinch steps) since the session started.
    void camera_gesture_counts(unsigned& pans_delivered,unsigned& pans_applied,unsigned& pinches) const noexcept;
    // Development probe for desktop scenarios: centre (drawable pixels) of the
    // area that accepts a press for authored HUD control `control`
    // (ui::AuthoredHudControlV1 ordinal; 7 attack, 8 joystick), found with the
    // same hit test touch() uses.
    bool hud_control_screen_point(unsigned control,int width,int height,float& x,float& y,std::string&);
    // The live gameplay movie that owns StringManager text and AS callbacks.
    bool gameplay_text_ready() const noexcept;
    // Android system Back uses the same live gameplay menu owner as the
    // authored NativeBackToHud callback; it never reloads the selected level.
    bool back_to_hud(std::string&);
    // Retained source MultiMenuManager overlay used while a gameplay Level is
    // leaving/loading. These calls target the existing shared menu symbol in
    // dqcharmenu's imported dqshared movie and preserve the live HUD/session.
    bool push_source_loading_menu(std::string&);
    // PlayerManager::_HandleLocalDeaths (offline): PushMenu(menu_FadeToBlackScreen)
    // only while that menu is not already visible.
    bool push_source_death_fade_menu(bool& pushed,std::string&);
    bool pop_source_loading_menu(std::string&);
    bool source_loading_menu_visible(bool&,std::string&);
    // Retained source MenuMessageManager<DialogMsg, 1>. Text, actor names,
    // and style labels resolve through this session's existing StringManager
    // and PyDataConstants views; no second queue or VM is created.
    bool enqueue_source_dialog(std::int32_t actor_id,std::int32_t style_id,
        std::int32_t text_string_id,std::string& error);
    // StartDialogID expands one source Dialogs row into the existing
    // MenuMessageManager<DialogMsg, 1> queue and starts it only if idle.
    bool enqueue_source_dialog_id(std::int32_t dialog_id,std::string& error);
    // Quest::SetState(Active): the "new quest" DialogMsg (QuestMsgDialog style).
    bool enqueue_source_new_quest_message(std::int32_t quest_name_string_id,
                                          std::string& error);
    // Quest::SetState(Closed): DialogMsg(quest name, RewardList text,
    // QuestCompletedMsgDialog, actor 1) on the same queue.
    bool enqueue_source_quest_completed_message(std::int32_t quest_name_string_id,
                                                const std::string& reward_text,
                                                std::string& error);
    // RewardList::CreateRewardString line text: kind 0 gold, 1 XP, 2 none.
    bool source_quest_reward_text(std::int32_t kind,std::int32_t amount,
                                  std::string& line,std::string& error);
    bool source_dialog_active(bool& active,std::string& error) const;
    // Script_FlushMessages 0x460a58: flushes the DialogMsg and StatusMsg
    // MenuMessageManagers (Achievement/OnlineStatus are not reconstructed).
    bool flush_source_messages(std::string& error);
    // Script_ChangeLevel (0x45cba0) enters the same deferred LoadLevel
    // transaction as NativeGoToZone through the retained launch queue.
    void queue_level_transition(const std::string& destination,double raw_entry_point);
    // Script_LockTutorial 0x45925c: clear the tutorial byte, then saveSettings.
    bool lock_source_tutorial(std::int32_t id,std::string& error);
    // Script_ShowFlash 0x45f8c8 / Script_HideFlash 0x45f78c for menu_* names:
    // MenuBase::Show/Hide (0x425450/0x424af4) on the named clip of the retained
    // HUD movie. No MenuManager stack push. HUD-named flashes use set_source_hud_shown.
    bool show_script_flash(const std::string& name,std::string& error);
    bool hide_script_flash(const std::string& name,std::string& error);
    // The same two commands for a "HUD"-named flash (strstr branch): Show runs the
    // HUD root's DisplayRightHud, FillActionIcon and DisplayOfCurrentHud(true);
    // Hide runs DisplayOfCurrentHud(false). Used by BeginScriptedCutScene and
    // EndScriptedCutScene. Without a loaded gameplay HUD movie there is nothing to drive.
    bool set_source_hud_shown(bool show,std::string& error);
    // Character::SetFaeryState 0x3ae7d4 reads settings tutorial byte id (settings+41+id)
    // before its offline Normal cinematic. Clearing reuses lock_source_tutorial.
    bool source_tutorial_active(std::int32_t id,bool& active,std::string& error) const;
    // ScriptManager commands 74/75/76 and the authored Native callbacks share
    // the retained CharacterMenu tutorial queue owned by this UI session.
    bool enqueue_source_char_menu_tutorial(std::int32_t string_id,
        const std::string& frame,const std::string& menu,std::string& error);
    bool skip_source_char_menu_tutorial(bool skip_all,std::string& error);
    // GSLevel::Dtor hides menu_HUD_0 on the retained movie before deleting
    // the old Level. This does not destroy the UI session or pop other menus.
    bool close_source_hud(std::string&);
    // PlayerManager::RemoveCharacter's RenderFX::InvokeASCallback tail on the
    // retained authored gameplay movie. Succeeds as a no-op when HUDRoot is absent.
    bool complete_source_hud_refresh(std::string&);
    // Script_EnterCutSceneMode 0x45a384 / Script_ExitCutSceneMode 0x4599ac:
    // StartCutscene / StopCutscene on the retained HUD root movie, toggling the
    // source cutscene byte. Neither command blocks (IsBlocking returns 0).
    bool enter_source_cutscene(std::string& error);
    bool exit_source_cutscene(std::string& error);
    bool source_cutscene_active() const noexcept;
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
