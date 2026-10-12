#pragma once
#include <android/asset_manager.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include "data.hpp"
#include "item_presentation_v5.hpp"
#include "loot_entry_selection_v1.hpp"
namespace dh2::character_runtime_factory_v1 { class Record; }
namespace dh2::native::quests { struct QuestTextServicesV1; }
namespace model_renderer {
using DisplayFastTravelServiceV1 = bool (*)(void*,bool,const std::string&,
    const std::string&,std::int32_t,std::string&);
using ItemTextServicesFactoryV5 = dh2::data::ItemTextServicesV5 (*)(
 void*,const dh2::data::ItemTable&,const dh2::data::CharacterTable&);
void bind_item_text_services(void*,ItemTextServicesFactoryV5);
struct SourceDialogServicesV1 {
 void* context{};
 // Returns 1 after source enqueue/flush dispatch, -1 on unavailable owner or
 // rejected metadata. Dialog source IDs are resolved by OriginalUiSession.
 int (*enqueue)(void*,std::int32_t actor_id,std::int32_t style_id,
                std::int32_t text_string_id){};
 // Returns 1 while the source DialogMsg queue is nonempty, 0 when empty,
 // and -1 when the retained UI session cannot answer.
 int (*active)(void*){};
 // Resolves an original Dialogs pyarray row and enqueues all DialogSteps on
 // the same retained SWF-backed DialogMsg queue.
 int (*enqueue_id)(void*,std::int32_t dialog_id){};
};
void bind_source_dialog_services(const SourceDialogServicesV1&);
struct SourceCharMenuTutorialServicesV1 {
 void* context{};
 int (*enqueue)(void*,std::int32_t string_id,const char* frame,const char* menu){};
 int (*skip)(void*,bool skip_all){};
};
void bind_source_char_menu_tutorial_services(const SourceCharMenuTutorialServicesV1&);
// Borrows the retained OriginalUiSession DesignSettingsTable bytes for
// source DistributeXP scaling; caller keeps that table owner alive.
void bind_xp_design_settings(const std::uint8_t*,std::size_t);
// Source DesignSettings::CombatMusicTrigger (+0x14), borrowed from the same
// retained OriginalUiSession table owner.
bool ui_combat_music_trigger(std::int32_t&,std::string&);
void mod_directory(std::string);
void runtime_directory(std::string);
std::string profile_slot(int);
bool menu_debug_load(AAssetManager*,std::string&);
bool menu_debug_query(AAssetManager*,const char*,std::string&);
bool menu_debug_switch_value(AAssetManager*,const char*,bool&,std::string&);
bool create_menu_save_slot(AAssetManager*,const std::string&,const std::string&,std::int32_t&,bool&,std::string&,void*,bool (*)(void*,std::uint32_t*,std::string&));
bool assign_menu_save_slot(std::int32_t slot,std::int32_t ordinal,std::string&);
bool selected_menu_save_slot(std::int32_t&,std::string&);
bool select_menu_preview_slot(std::int32_t,bool,std::string&);
// Rebuilds the current main-menu preview after its source scene is reloaded.
// An unset selected slot remains an empty preview.
bool restore_menu_preview(std::string&);
bool request_menu_start(bool,std::int32_t,std::int32_t&,std::string&);
bool request_menu_continue(std::int32_t&,std::string&);
// Authored static MLX LevelList rows (debug launch overrides) and the
// startup-report check shared by the Android and desktop hosts.
// Head point of the displayed enemy target projected for the HUD enemy bar
// (user-requested follow; the original keeps HealthBars.enemy fixed).
bool ui_enemy_target_head_projection(std::uintptr_t target,float head_world[3],
    float view_projection[16],int& width,int& height,bool& valid,std::string& error);
const char* static_level_name_for_row(std::int32_t row);
bool gameplay_startup_loaded(const std::string& startup);
std::string start_menu_game(std::int32_t,AAssetManager*,std::int32_t debug_level_row=-1,
                            bool has_numeric_difficulty=false,std::int32_t requested_difficulty=0);
// Retain the application's existing UI session for source Level callbacks
// that use the active HUDRoot. The renderer borrows this process-lifetime owner.
void bind_gameplay_ui_session(void*) noexcept;
// Dispatches one typed NativeGoToZone request after the GameSWF callback has
// unwound, on the same GL thread that owns the renderer and live Level.
std::string transition_level(const std::string& destination_name,
                            double raw_entry_point,AAssetManager*,void*,
                            DisplayFastTravelServiceV1);
// Source MenuCharMenu_Map::ShowLevelName reads the active LevelList row's
// localized name ID. Returns -1 when no current world/row is attached.
std::int32_t ui_current_level_name_id() noexcept;
std::vector<std::uint8_t> read_asset(AAssetManager*,const std::string&);
void reset_context();
void deactivate(bool defer_source_level_finish=false);
// Returns false while retaining the gameplay owner if terminal retirement is
// blocked by an active source dispatch or borrowed timer/state storage.
bool unload_game_to_menu(std::string& error);
bool active();
// Source NativeStartGame returns before inspecting its arguments while the
// retained gameplay Level is active (source state 38).
bool source_start_game_blocked_by_current_world() noexcept;
std::string load(const std::uint8_t*,std::size_t,AAssetManager*);
std::string load_menu_background(AAssetManager*);
void draw_menu_background(int width,int height);
bool class_scene_active();
bool class_scene_input_enabled();
std::string load_class_scene(AAssetManager*);
bool select_class_scene(int index,int dt_ms,std::string&);
void draw_class_scene(int width,int height);
std::string load_world(const std::uint8_t*,std::size_t,AAssetManager*,
                       const std::uint8_t* generated_spawnpoints=nullptr,
                       std::size_t generated_spawnpoints_size=0);
void move_axis(float x,float y);
// Direct HUD pointer-up route: Stop the GameObject, then raise event 63.
void stop_movement();
// Owning GL thread only. Applies the original ZoomHandler pinch delta to the
// active CameraLevel owner; returns false when that source camera is inactive.
bool camera_pinch_zoom(float previous_distance,float current_distance);
bool camera_touch_pan(std::int32_t delta_x,std::int32_t delta_y);
// Authored NativeLockZoom/NativeUnlockZoom mutate +134 on the currently bound
// source CameraLevel, retaining the canonical player/map camera ownership.
void camera_zoom_lock(bool locked) noexcept;
// MenuCharMenu_Map owns a distinct camera rig and CameraLevel. Open/close are
// called on the GL/UI owner thread when that authored menu enters/leaves.
bool map_camera_open(std::string& error);
void map_camera_close() noexcept;
// Authored NativeResetMapZoom resets the active MenuCharMenu_Map CameraLevel.
bool map_camera_reset_zoom(std::string& error);
// Submit the already-updated world into MenuCharMenu_Map::RenderMap's clipped
// viewport. This render-only pass never advances Level or actor simulation.
bool draw_map_scene(int width,int height,std::string& error);
// Non-owning render registration for the canonical Character factory Record.
// The factory remains the sole Record owner and must unregister before retire.
bool register_faery_visual_record(dh2::character_runtime_factory_v1::Record& player,
                                  dh2::character_runtime_factory_v1::Record& faery,
                                  std::string& error);
void unregister_faery_visual_record(
    dh2::character_runtime_factory_v1::Record& player,
    dh2::character_runtime_factory_v1::Record& faery) noexcept;
// Authored HUD joystick already produces the source-rotated direction and
// magnitude. It enters the existing Player controller directly, bypassing the
// Android touch/gamepad deadzone and camera remapping. stop=true schedules the
// source Cmd_Stop path; a zero non-stop vector remains HeadTowards(0).
bool authored_hud_command(const float* direction,bool stop,std::string& error);
bool ui_player_controller_allowed(std::uintptr_t character,bool& allowed,std::string& error);
void focus_object(int index);
std::string set_object_state(int index,const std::string& state);
std::string spawn_character(const std::string& exact_name);
std::string debug_character_hit(const std::string& exact_name,std::uint32_t raw_damage);
std::string debug_player_skill_cooldown(std::uint32_t delay_ms);
std::string debug_player_skill_check(std::uint32_t slot);
std::string debug_player_mana(std::uint32_t amount);
std::string debug_player_scalar(std::int32_t value,bool write);
std::string debug_player_death();
std::string set_combat_target(int index,int target);
std::string player_attack(int target=-1);
std::array<int,7> player_vitals();
// Read-only live Player and nearest source enemy snapshot for bounded desktop
// input checks. The enemy index is diagnostic only; attack selection remains
// owned by player_attack() with its default target.
struct GameplayPoseSnapshotV1 {
 std::array<float,3> player_position{};
 std::int32_t enemy_index=-1;
 std::array<float,3> enemy_position{};
 std::int32_t enemy_hp=0,enemy_max_hp=0;
 // Read through the same live reach provider used by player_attack().
 bool enemy_in_melee_range=false;
 // Player current/max HP property values (24.8 fixed point).
 std::int32_t player_hp=0,player_max_hp=0;
};
bool gameplay_pose_snapshot(GameplayPoseSnapshotV1&,std::string&);
// Development check: dropped world items (ItemManager projections) and the
// one nearest the Player.
struct GameplayWorldItemSnapshotV1 {
 std::int32_t count=0;
 std::array<float,3> nearest{};
 float distance=0.f;
};
bool gameplay_world_item_snapshot(GameplayWorldItemSnapshotV1&,std::string&);
// Computes a temporary selected-floor route for the desktop input check. It
// does not install a Character path or issue Cmd_MoveTo; callers still steer
// through move_axis and the live collision/controller owners.
struct GameplayRouteSnapshotV1 {
 bool graph_sewn{};
 bool path_found{};
 std::uint32_t graph_nodes{},graph_edges{};
 std::uint32_t source_node{},target_node{},query_status{};
 std::vector<std::array<float,3>> waypoints;
};
bool gameplay_route_snapshot(const std::array<float,3>&,
                             GameplayRouteSnapshotV1&,std::string&);
struct GameplayEnemyStateSnapshotV1 {
 std::string name;
 std::int32_t hp{},max_hp{};
 bool dead{};
};
bool gameplay_enemy_state_snapshot(std::int32_t,
                                   GameplayEnemyStateSnapshotV1&,std::string&);
// Read-only localization projection of the one live Player Character and its
// associated Save name. Requires the attached world; creates no profile owner.
bool ui_player_identity(std::uintptr_t&,std::string&);
bool ui_player_name(std::uintptr_t,std::string&,std::string&);
struct UiFastTravelReadV1 {
 std::string level_name;std::int32_t description_id{},importance{},entry_point_id{},string_id{};bool visible{};
};
// Read-only projection of the selected difficulty's canonical Save unlock
// bits and the source FastTravelList. No parallel travel or unlock owner.
bool ui_player_fast_travel_rows(std::uintptr_t,std::vector<UiFastTravelReadV1>&,std::string&);
bool ui_player_class_specialization_text_ids(std::uintptr_t,
 std::array<std::int32_t,4>&,std::string&);
// Apply the authored NativeSelectClassSpec selector through the active
// Character properties, Save, Player skill runtime and profile transport.
bool ui_player_select_class_specialization(std::uintptr_t,std::int32_t,
                                           std::string&);
struct UiPlayerReloadResultV1 {std::uint32_t phase{},calls{},specialization{};};
// The current retained character-menu movie supplies the two final AS menu
// operations; all earlier phases run through the same live Player owners.
using UiPlayerReloadMenuServiceV1 = int (*)(void*,std::uint32_t service,
    std::uint32_t argument,std::uintptr_t subject,const char* path,
    const char* callback,std::uintptr_t& identity,std::int32_t& value,
    std::string& error);
bool ui_player_reload_skills(std::uintptr_t,void*,UiPlayerReloadMenuServiceV1,
                             UiPlayerReloadResultV1&,std::string&);
struct UiPlayerStatsReadV1 {
 std::string name,class_name;std::int32_t icon{},level{},hp{},hp_bonus{},max_hp{};
 std::int32_t mp{},mp_bonus{},max_mp{},xp{},max_xp{},strength{},dexterity{};
 std::int32_t endurance{},energy{},points{};
 // Remaining NativeGetPlayerStats fields read by the authored character
 // sheet. Values are projected from the same resolved Character properties
 // and live inventory/equipment owners as combat; no second stats store.
 std::int32_t rating_attack{},rating_critical{},rating_defense{},rating_dodge{},rating_block{};
 std::int32_t resistance_fire{},resistance_earth{},resistance_water{},resistance_air{},resistance_lightning{};
 std::int32_t damage_min_main_hand{},damage_max_main_hand{};
 std::int32_t damage_elemental_min_main_hand{},damage_elemental_max_main_hand{},damage_elemental_type_main_hand{};
 std::int32_t damage_min_off_hand{},damage_max_off_hand{};
 std::int32_t damage_elemental_min_off_hand{},damage_elemental_max_off_hand{},damage_elemental_type_off_hand{};
 std::int32_t damage_fire_min_main_hand{},damage_fire_max_main_hand{};
 std::int32_t damage_water_min_main_hand{},damage_water_max_main_hand{};
 std::int32_t damage_lightning_min_main_hand{},damage_lightning_max_main_hand{};
 std::int32_t damage_air_min_main_hand{},damage_air_max_main_hand{};
 std::int32_t damage_earth_min_main_hand{},damage_earth_max_main_hand{};
 bool is_weapon_two_handed{},has_off_hand_weapon{},has_staff{},has_bow{};
 std::int32_t menu_average_melee_to_hit{},physical_armor{},spell_rating_dodge{};
 std::int32_t menu_melee_damage_reduction{},spell_rating_critical{},menu_average_spell_to_hit{};
 std::int32_t spell_damage_bonus_fire{},spell_damage_bonus_earth{},spell_damage_bonus_water{};
 std::int32_t spell_damage_bonus_air{},spell_damage_bonus_lightning{};
 std::int32_t regen_hp{},regen_mp{},leech_hp{},leech_mp{};
 std::int32_t special_loot_gold_multiplier{},special_loot_magical_chance{},stun_resist_chance{};
};
// Read-only values consumed by the authored gameplay HUD. The prior value is
// passed back in so source usability checks can retain their 500 ms cadence;
// timers, potion count, points and the one selected class/faery stay live.
struct UiPlayerHudProjectionV1 {
 std::uintptr_t character{};
 std::int8_t object_of_interest_type{-1};
 std::int32_t class_id{-1},faery_id{-1};
 std::array<std::int32_t,3> skill_slots{-1,-1,-1};
 std::array<std::int32_t,17> infos{};
};
bool ui_player_hud_projection(std::uintptr_t,bool refresh_usable,
                              UiPlayerHudProjectionV1&,std::string&);
// Synchronous snapshots of the existing player CharAI target and selected
// combat target for the authored enemy bar. Names borrow the actor's retained
// Character data; no target, HP, or lifecycle state is owned by this view.
struct UiEnemyHudActorV1 {
 std::uintptr_t identity{};
 const char* debug_name{};
 std::int32_t name_symbol{-1},network_id{-1},level{-1};
 float hp_fraction{};
 bool present{},is_character{},is_monster{},is_boss{};
};
struct UiEnemyHudProjectionV1 {
 std::uintptr_t player_identity{};
 bool player_dead{};
 UiEnemyHudActorV1 ai_target{},character_target{};
};
bool ui_player_enemy_hud_projection(std::uintptr_t,UiEnemyHudProjectionV1&,
                                    std::string&);
bool ui_player_stats(std::uintptr_t,UiPlayerStatsReadV1&,std::string&);
bool ui_player_assign_stat(std::uintptr_t,std::uint32_t,std::string&);
bool ui_player_set_current_quest(std::uintptr_t,std::int32_t,std::string&);
struct UiQuestReadV1 {
 std::int32_t id{-1},state{-1},priority{-1};
 std::array<std::int32_t,4> text_ids{{-1,-1,-1,-1}};
 bool current{};
 std::string objective_description;
};
// Rows remain in SG_GetQuestByID order. The authored SWF provider applies
// predicate filtering and localized title sorting before writing its array.
// Optional text services borrow the active SWF localization owner when the
// dynamic objective description field is requested.
bool ui_player_quest_rows(std::uintptr_t,std::vector<UiQuestReadV1>&,std::string&,
                          const dh2::native::quests::QuestTextServicesV1* =nullptr);
bool ui_player_resolve_current_quest(std::uintptr_t,
    const std::vector<UiQuestReadV1>& source_order,
    std::vector<UiQuestReadV1>& displayed_rows,std::string&);
bool ui_player_save_game(std::uintptr_t,std::string&);
// Authored NativeReviveAllPlayers action. The implementation snapshots the
// active source Level generation before calling the canonical PlayerManager.
bool ui_revive_local_players(std::string&);
// Authored NativeSaveGame trophy tail, called only after its canonical
// Player profile save has completed.
bool ui_player_save_trophy_side_effects(std::uintptr_t,std::string&);
// Reached only after the original _GiveXP max-level gate. Advances the same
// Character/Save through LevelUp property reset, base-class recalc, RegenHP,
// RegenMP, SG_SetPlayerLevel, then the canonical SG_Save transport.
bool ui_player_apply_level_up_state(std::uintptr_t,std::string&);
struct UiSkillReadV1 {
 std::int32_t id{-1},level{-1},slot{-1},required_level{-1};
 std::int32_t character_level{},difficulty{},faerie_text_offset{};
 std::int32_t name_text{-1},description_text{-1},current_text{-1},next_text{-1};
 bool assignable{},faerie_dependent_text{},can_increment{};std::string icon;
};
struct UiInventoryItemReadV1 {
 std::int32_t id{-1},index{-1},quantity{},slot{-1},power_count{};std::string name;
 bool equippable{},equipped{},equipped_other_hand{};
};
struct UiEquippedItemReadV1 {
 std::int32_t id{-1},index{-1},power_count{};std::string name;
};
struct UiItemDetailsReadV1 {
 std::int32_t id{-1},index{-1},value{},buy_value{},sell_value{},transmute_property_raw{},transmute_value{1};
 std::string name,stats,requirements,icon;
 bool stackable{},equippable{},equipped{},equipped_other_hand{};std::vector<std::string> power_descriptions;
};
struct UiMerchantItemReadV1 {
 std::int32_t id{-1},index{-1},quantity{},power_count{};
 std::int32_t value{},buy_value{},sell_value{};
 std::string name;
 bool equippable{},equipped{},equipped_other_hand{},buyback{};
};
bool ui_player_skill_slots(std::uintptr_t,std::array<std::int32_t,3>&,std::string&);
bool ui_player_use_skill_slot(std::uintptr_t identity,std::int32_t hud_slot,std::string& error);
bool ui_player_use_spell(std::uintptr_t identity,std::string& error);
bool ui_player_skill_points(std::uintptr_t,std::int32_t&,std::string&);
bool ui_player_skill(std::uintptr_t,std::uint32_t,UiSkillReadV1&,std::string&);
bool ui_player_skill_display_properties(std::uintptr_t,std::uint32_t,std::int32_t,
                                       std::vector<std::int32_t>&,std::string&);
bool ui_player_train_skill(std::uintptr_t,std::uint32_t,bool,std::uint32_t&,std::int32_t&,std::string&);
bool ui_player_equip_skill(std::uintptr_t,std::int32_t slot,std::int32_t skill_index,std::string&);
bool ui_player_active_faery(std::uintptr_t,std::int32_t&,std::int32_t&,std::string&);
bool ui_player_set_active_faery(std::uintptr_t,std::uint32_t,std::string&);
bool ui_player_faery_unlocked(std::uintptr_t,std::uint32_t,bool&,std::string&);
bool ui_player_inventory_gold(std::uintptr_t,std::int32_t&,std::string&);
bool ui_player_drop_inventory_item(std::uintptr_t,std::int32_t,std::string&);
bool ui_player_inventory_slot(std::uintptr_t,std::int32_t,std::vector<UiInventoryItemReadV1>&,std::string&);
bool ui_player_inventory_item_details(std::uintptr_t,std::int32_t,std::uint32_t,
                                      UiItemDetailsReadV1&,std::string&);
// Merchant callbacks pass ObjectManager::GetObjectByName's source module filter
// (or -1 wildcard), not the returned ObjectHandle ledger key. Resolve that
// filter plus the exact source name to a retained CharType 7 ObjectActor/V4.
bool ui_source_merchant_identity(std::int32_t,const std::string&,std::uintptr_t&,std::string&);
// Resolves the current canonical Player OOI to a merchant only when the
// Character::GetInteractionSpot/AI range kernel accepts the live interaction.
bool ui_player_merchant_interaction_target(std::string&,std::int32_t&,
                                           std::uintptr_t&,std::string&);
// Executes Character::Interact against the Player's current source OOI when
// it is a live in-range Character target. `identity` is zero when no NPC
// action applies; a nonempty error means the selected source path failed.
bool ui_player_character_interact_target(std::uintptr_t& identity,
                                         std::string& error);
bool ui_source_merchant_inventory_slot(std::int32_t,const std::string&,std::int32_t,
                                       std::vector<UiMerchantItemReadV1>&,std::string&);
bool ui_source_merchant_item_details(std::uintptr_t,std::int32_t,const std::string&,
                                     std::int32_t,std::uint32_t,
                                     UiItemDetailsReadV1&,std::string&);
bool ui_source_merchant_buy_quote(std::int32_t,const std::string&,std::int32_t,
                                  std::int32_t&,bool&,std::string&);
// Source NativeMerchantBuyItem status domain: 0 success/no-op, 1 insufficient
// gold, 2 full inventory, 3 potion capacity reached. A false return means the
// live owner rejected a reached source step; its already-applied prefix stays.
bool ui_source_merchant_buy_transaction(std::int32_t,const std::string&,
 std::int32_t,std::uintptr_t,std::int32_t&,std::string&);
// NativeMerchantSellItem mutates once per visible menu_Merchant lifetime and
// returns false even after completing its source transaction.
bool ui_source_merchant_sell_transaction(std::int32_t,const std::string&,
 std::int32_t,std::uintptr_t,std::int32_t&,std::string&);
bool ui_player_transmute_item(std::uintptr_t,std::int32_t,std::uint32_t,std::string&);
bool ui_player_equipped_item(std::uintptr_t,std::int32_t,UiEquippedItemReadV1&,bool&,std::string&);
bool ui_player_equip_item(std::uintptr_t,std::int32_t item_index,std::int32_t equipment_slot,std::string&);
bool ui_player_unequip_item(std::uintptr_t,std::int32_t equipment_slot,std::string&);
bool ui_player_auto_equip(std::uintptr_t,std::int32_t item_index,std::int32_t& result,std::string&);
bool ui_player_auto_equip_slot(std::uintptr_t,std::int32_t equipment_slot,std::string&);
bool ui_player_swap_equipment(std::uintptr_t,std::string&);
bool ui_player_weapon_flags(std::uintptr_t,bool&,bool&,std::string&);
bool ui_player_potions(std::uintptr_t,std::int32_t&,std::int32_t&,std::string&);
// Source NativeUsePotion -> v2Controller::Cmd_UsePotion -> Character::Ctrl_UsePotion.
// Uses the active controller flags, one V4 inventory, and the live PropertyState.
bool ui_player_use_potion(std::uintptr_t,std::string&);
struct LootStagingResultV1 {std::size_t first_world_item{},item_count{};};
// Owning GL thread only. Stages powered Loot through the existing Character
// V4 inventory/RNG and its retained Item presentation owner. This does not
// attach a world transform, render a pooled Item, or invoke death handling.
bool stage_world_loot_table(std::int32_t,const dh2::data::LootEntrySelectionContextV1&,
 std::int32_t value_bonus256,std::int32_t power_bonus256,
 std::int32_t requested_power_count,std::int32_t difficulty,
 LootStagingResultV1&,std::string& error);
bool retire_staged_world_loot_item(std::size_t,std::string& error);
void orbit(float dx,float dy,float zoom);
// Owning GL thread only. Refreshes the active ZoomHandler equivalent from
// the current drawable target dimensions; logical SWF stage stays separate.
void drawable_target_changed(int width,int height);
void set_time(int milliseconds);
void set_enemy_ai(bool);
void draw(int width,int height);
// Owning GL thread only. Synchronously borrows the sole published player's
// cached resolved sheet. The consumer must not retire/replace the world or
// retain the pointer; no property resolution or gameplay update occurs here.
bool with_player_status_sheet(void* context,
 bool (*consume)(void*,const std::int32_t*,std::size_t,std::uintptr_t,std::string&),
 std::string& error);
}
