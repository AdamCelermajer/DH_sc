#include "asset_catalog.hpp"
#include "actor_profiles.hpp"
#include "actor_population.hpp"
#include "features/levels/level_class_registry.hpp" // P16 LEVELS: one-time unsupported class log
#include "actor_movement.hpp"
#include "collision_scene.hpp"
#include "controller_policy.hpp"
#include "equipment_visual.hpp"
#include "hud_geometry.hpp"
#include "original_actor_properties.hpp"
#include "player_profile_properties.hpp"
#include "original_actor_labels.hpp"
#include "original_camera_config.hpp"
#include "original_actor_camera_anchor.hpp"
#include "original_actor_lifecycle.hpp"
#include "original_campaign_world_adapter.hpp"
#include "source_world_objects.hpp"
#include "source_root_scopes.hpp"
#include "source_module_floors.hpp"
#include "original_actor_bounds.hpp"
#include "original_actor_body_plan.hpp"
#include "playable_actor_bodies.hpp"
#include "original_actor_collision_filter.hpp"
#include "original_actor_motion_flags.hpp"
#include "original_actor_target_position.hpp"
#include "features/character_menu/character_menu.hpp"
#include "features/character_menu/menu_text.hpp"
#include "features/character_menu/menu_text_layout_v1.hpp"
#include "features/character_menu/stat_training_v1.hpp"
// P16 MAP: RoomZone visits and the character-menu Map page model (features/map_visit).
#include "features/map_visit/room_zone_visit_v1.hpp"
#include "features/map_visit/map_page_v1.hpp"
#include "features/pause_ui/source_pause_ui_render_v1.hpp"
#include "features/frontend/rich_text.hpp"
#include "features/combat/object_of_interest_world_v1.hpp" // B004/B029: OOI owner + rendered target marker
#include "features/combat/context_button_v1.hpp" // P16 CONTEXT: Space context button + action icon
#include "features/interactions/interactable_registry_v1.hpp" // P16 CONTEXT: interaction-type providers (chests/NPCs register here)
#include "features/loot/world_item_contact_v1.hpp" // P16 CONTEXT: walk-over pickup contact rule
#include "features/generic_skills/runtime_skills_menu_v1.hpp"
#include "features/generic_skills/runtime_skill_progression_v1.hpp"
#include "features/generic_skills/runtime_skill_session_training_v1.hpp"
#include "features/generic_skills/runtime_skills_text_v1.hpp"
#include "features/generic_skills/runtime_skill_animation_bank_v1.hpp"
#include "features/generic_skills/runtime_skill_cast_coordinator_v1.hpp"
#include "features/generic_skills/runtime_skill_cast_prepare_v1.hpp"
#include "features/generic_skills/pc_gameplay_hud_v1.hpp"
#include "features/campaign_host/campaign_host.hpp" // P16 HOST
#include "features/skill_ui/skill_ui.hpp"
#include "features/equipment/runtime_equipment_text_v1.hpp"
#include "features/faery_menu/character_state_faery_v1.hpp" // P14 FAERY: CharacterState Faery page host + script effects
#include "features/equipment/equipment_inventory_actions_v1.hpp"
#include "features/equipment/runtime_player_locomotion_library_v1.hpp"
#include "features/combat/runtime_player_profile_attack_bank_v1.hpp"
#include "../script-runtime/script_constants.hpp"
#include "features/loot/runtime_session_death_rewards_v1.hpp"
// P16 QUESTS: thin quest runtime and the generic quest event bus (raise_quest_event).
#include "features/quest_runtime/quest_runtime_v1.hpp"
#include "features/quest_runtime/quest_events_v1.hpp"
// P14 DROPS: world item presentation, pickup rules and item name text
#include "features/interactions/world_drop_runtime_v1.hpp"
#include "features/containers/container_declarations_v1.hpp" // P16 containers
#include "features/containers/container_runtime_v1.hpp" // P16 containers (T2/T3)
#include "features/containers/container_loot_v1.hpp" // P16 CONTAINERS2 (T4 DoOpen loot)
#include "features/containers/container_open_script_v1.hpp" // P16 CONTAINERS2 (T6 OnOpen contract)
#include "features/containers/container_world_v1.hpp" // P16 CONTAINERS2 (T5 OBJS persistence)
#include "features/despawn/despawn_after_death_v1.hpp" // P16 DESPAWN (automatic despawn after death)
#include "features/quest_runtime/quest_zones_v1.hpp" // P16 QUESTUI: quest trigger zones
#include "features/quests/quest_banner_presenter_v1.hpp" // P16 QUESTUI: quest banners
#include "features/quests/runtime_quest_menu_v1.hpp" // P16 QUESTUI: Quest Log tab page
#include "features/quests/source_quest_menu_page_provider_v1.hpp" // P16 QUESTUI
#include "features/quests/quest_text_resolver_v1.hpp" // P16 QUESTUI
#include "features/inventory/source_item_descriptors.hpp"
#include "../engine-ui/item_text_owner_v5.hpp"
#include "features/inventory/runtime_session_potion_use_v1.hpp"
#include "features/frontend/menu_return/menu_return_v1.hpp"
#include "features/menu_metadata/menu_metadata_v1.hpp" // P14 schema: per-slot menu metadata stamping/projection
#if defined(_WIN32)
#include "features/audio/frontend_menu_audio_v1.hpp"
#endif
#include "features/effects/runtime_effects_renderer_v1.hpp"
#include "features/effects/runtime_swing_fx_observer_v1.hpp"
#include "features/effects/runtime_level_up_presentation_v1.hpp" // P15 LEVELUP (I026)
#include "features/effects/celest_target_fx_dispatch_v1.hpp"
#include "features/platform_input/semantic_input.hpp"
#include "features/combat_text/combat_text.hpp"
#include "features/combat_text/combat_text_live.hpp"
#include "features/enemy_ai/runtime_enemy_navigation_v1.hpp"
#include "features/enemy_ai/runtime_enemy_contact_binding_v1.hpp"
#include "features/physics/runtime_session_contact_v1.hpp"
#include "features/physics/session_actor_transition_v1.hpp"
#include "features/audio/runtime_audio_host_v1.hpp"
#include "features/audio/level_music_v1.hpp"
#include "features/audio/runtime_session_audio_v1.hpp"
#include "features/loot/world_item_sound_v1.hpp"
#include "features/frontend/creation/generic_creation_host_v1.hpp"
#include "features/frontend/creation/dynamic_text_bindings.hpp"
#include "features/frontend/creation/runtime_creation_source_loader_v1.hpp"
#include "features/frontend/creation/runtime_creation_persistence_v1.hpp"
#include "../engine-audio/audio_listener_rows_v38.hpp"
#include "../level-world/character_body_config.hpp"
#include "../level-world/character_animation_ai.hpp"
#include "modular_defaults.hpp"
#include "retained_animation_owner.hpp"
#include "overlay_renderer.hpp"
#include "content_paths.hpp"
#include "assembled_level.hpp"
#include "camera.hpp"
#include "combat_session.hpp"
#include "source_navigation_world_storage.hpp"
#include "game_save.hpp"
#include "hud_glyphs.hpp"
#include "character_state.hpp"
#include "original_character.hpp"
#include "original_scene.hpp"
#include "platform_win32.hpp"
#include "renderer.hpp"
#include "features/equipment/source_equipment_material_binding.hpp"
#include "render_queue.hpp"
#include "save_store.hpp"
#include "texture_loader.hpp"
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include "platform_key_codes.hpp"
#endif
#include "platform_sleep.hpp"
#include "features/startup/boot_runner_v1.hpp"  // Preview 15 startup boot
#include "features/startup/loading_screen_v1.hpp"  // Preview 15 campaign loading screen
#include "features/audio/winmm_output.hpp"         // Preview 15 boot soundtrack output (platform pump only)
#include "../engine-audio/audio_mixer_v34.hpp"
#include <GL/gl.h>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <limits>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include "features/spawn/spawn_character_v1.hpp" // P16 SPAWN: --spawn-test owner (default off)
#include "features/spawn/actor_profile_derivation_v1.hpp" // P16 PROFILES: profile/melee/policy derivation from pydata (default path for unauthored rows)

namespace f = dh::foundation;
namespace fs = std::filesystem;
struct GameplayRewardContext {
    std::function<bool(const char*,bool&,std::string&)> debug;
    std::function<bool(f::ActorId,f::loot::RuntimeDeathActorV1&,std::string&)> character;
    // Filled from the shared profile (CharacterState current/unlocked difficulty) at each reward binding.
    std::int32_t currentDifficulty{},unlockedDifficulty{};
    bool rewardsSuppressed{};
    static bool admission(void* raw,f::loot::RuntimeDeathRewardAdmissionV1& out,std::string& e) {
        out.rewards_suppressed=static_cast<GameplayRewardContext*>(raw)->rewardsSuppressed;e.clear();return true;
    }
    static bool difficulty(void* raw,std::int32_t& current,std::int32_t& unlocked,std::string& e) {
        const auto& self=*static_cast<GameplayRewardContext*>(raw);
        current=self.currentDifficulty;unlocked=self.unlockedDifficulty;e.clear();return true;
    }
    static bool debugQuery(void* raw,const char* key,bool& value,std::string& e) {
        return static_cast<GameplayRewardContext*>(raw)->debug(key,value,e);
    }
    static bool resolve(void* raw,f::ActorId id,f::loot::RuntimeDeathActorV1& out,std::string& e) {
        return static_cast<GameplayRewardContext*>(raw)->character(id,out,e);
    }
    static bool lootEntry(void* raw,const dh2::data::LootEntryRequestV8& request,std::int32_t& value,std::string& e) {
        value=0;
        if(request.operation==dh2::data::LootEntryOperationV8::debug_query) {
            bool enabled{};if(!debugQuery(raw,request.key,enabled,e))return false;value=enabled?1:0;
        } else if(request.operation==dh2::data::LootEntryOperationV8::assertion) {
            e="Original loot weighted selection exhausted";return false;
        } else if(request.operation!=dh2::data::LootEntryOperationV8::debug_load) {
            e="Unexpected source loot entry operation";return false;
        }
        e.clear();return true;
    }
};
f::Mat4 multiplyRenderMatrices(const f::Mat4& left,const f::Mat4& right) {
    f::Mat4 result{};for(unsigned c=0;c<4;++c)for(unsigned r=0;r<4;++r)for(unsigned k=0;k<4;++k)result[c*4+r]+=left[k*4+r]*right[c*4+k];return result;
}
std::string glyphTextureKey(const f::HudGlyphQuad& glyph,const char* prefix) {
    std::uint64_t hash=14695981039346656037ull;
    for(const auto byte:glyph.image.rgba){hash^=byte;hash*=1099511628211ull;}
    return std::string(prefix)+"/"+std::to_string(glyph.codepoint)+"/"+
        std::to_string(glyph.image.width)+"/"+std::to_string(glyph.image.height)+"/"+std::to_string(hash);
}
// P14 DROPS: centred world/status label from the original Fontin glyph owner.
// rgb is 0xRRGGBB; (centreX, baselineY) are window pixels; scale = window px per source px.
bool drawScreenLabel(f::HudGlyphFont& font,const std::string& text,std::uint32_t rgb,int sourceHeight,
                     float centreX,float baselineY,float scale,f::Renderer& renderer,f::OverlayRenderer& overlay,
                     std::map<std::string,std::uint32_t>& textures,std::string& error) {
    f::HudGlyphRun run;
    if(!font.raster(text,sourceHeight,scale,run,error))return false;
    const std::array<float,4> color{float((rgb>>16)&255)/255.f,float((rgb>>8)&255)/255.f,float(rgb&255)/255.f,1.f};
    for(const auto& glyph:run.glyphs) {
        if(glyph.width<=0||glyph.height<=0)continue;
        const auto key=glyphTextureKey(glyph,"drop-glyph");
        auto handle=textures.find(key);
        if(handle==textures.end()) {
            const auto texture=renderer.createTexture(glyph.image.width,glyph.image.height,glyph.image.rgba.data());
            if(!texture){error="Original item-label glyph upload failed";return false;}
            handle=textures.emplace(key,texture).first;
        }
        f::OverlaySprite sprite;
        sprite.x=centreX+(glyph.x-run.advance*.5f)*scale;sprite.y=baselineY+glyph.y*scale;
        sprite.width=glyph.width*scale;sprite.height=glyph.height*scale;sprite.u1=glyph.u1;sprite.v1=glyph.v1;
        sprite.texture=handle->second;sprite.color=color;overlay.drawSprite(sprite);
    }
    error.clear();return true;
}
bool drawCombatGlyphs(const std::vector<f::CombatTextGlyph>& glyphs,f::Renderer& renderer,
                     f::OverlayRenderer& overlay,std::map<std::string,std::uint32_t>& textures,
                     std::string& error) {
    for(const auto& quad:glyphs) {
        const auto& glyph=quad.glyph;
        const auto key=glyphTextureKey(glyph,"combat-glyph");
        auto handle=textures.find(key);
        if(handle==textures.end()) {
            const auto texture=renderer.createTexture(glyph.image.width,glyph.image.height,glyph.image.rgba.data());
            if(!texture){error="Original combat glyph upload failed";return false;}
            handle=textures.emplace(key,texture).first;
        }
        const f::OverlayTriangleVertex corners[4]{{quad.xy[0],quad.xy[1],0,0},
            {quad.xy[2],quad.xy[3],glyph.u1,0},{quad.xy[4],quad.xy[5],glyph.u1,glyph.v1},
            {quad.xy[6],quad.xy[7],0,glyph.v1}};
        const f::OverlayTriangleVertex triangles[6]{corners[0],corners[1],corners[2],corners[0],corners[2],corners[3]};
        if(!overlay.drawTriangles(triangles,6,handle->second,quad.rgba)) {
            error="Original combat glyph geometry rejected";return false;
        }
    }
    error.clear();return true;
}
// P16 QUESTUI: console name of a runtime banner kind (NEW QUEST / QUEST UPDATED / QUEST COMPLETED).
const char* questBannerKindName(f::quest_runtime::QuestBannerV1::Kind kind) {
    return kind==f::quest_runtime::QuestBannerV1::Kind::new_quest?"NEW QUEST":
           kind==f::quest_runtime::QuestBannerV1::Kind::updated?"QUEST UPDATED":"QUEST COMPLETED";
}
// P16 QUESTUI: quest banner placeholder panel + lines (original Fontin glyphs via drawScreenLabel).
// The panel is a PLACEHOLDER: the original dialog frame art is not exported in this build.
bool drawQuestBanner(const f::QuestBannerDisplayV1& display,f::HudGlyphFont& font,f::Renderer& renderer,
                     f::OverlayRenderer& overlay,std::map<std::string,std::uint32_t>& textures,
                     int windowWidth,int windowHeight,float scale,std::string& error) {
    if(display.lines.empty()||display.alpha<=0.f)return true;
    const float gap=8.f*scale,padding=12.f*scale,panelWidth=300.f*scale;
    float contentHeight=0.f;
    for(const auto& line:display.lines)contentHeight+=float(line.source_height)*1.25f*scale+gap;
    const float panelHeight=contentHeight+2.f*padding;
    const float left=(float(windowWidth)-panelWidth)*.5f,top=float(windowHeight)*.12f;
    const auto fill=[&](float x,float y,float w,float h,std::array<float,4> color) {
        const f::OverlayTriangleVertex q[6]{{x,y,0,0},{x+w,y,0,0},{x+w,y+h,0,0},{x,y,0,0},{x+w,y+h,0,0},{x,y+h,0,0}};
        return overlay.drawTriangles(q,6,0,color);
    };
    if(!fill(left-2.f*scale,top-2.f*scale,panelWidth+4.f*scale,panelHeight+4.f*scale,{0.55f,0.42f,0.20f,0.9f*display.alpha})) {
        error="Quest banner border geometry rejected";return false;
    }
    if(!fill(left,top,panelWidth,panelHeight,{0.06f,0.05f,0.04f,0.85f*display.alpha})) {
        error="Quest banner panel geometry rejected";return false;
    }
    float y=top+padding;
    for(const auto& line:display.lines) {
        const float lineHeight=float(line.source_height)*1.25f*scale;
        const auto fade=[&](std::uint32_t channel) {return std::uint32_t(float(channel)*display.alpha);};
        const std::uint32_t rgb=(fade((line.rgb>>16)&255)<<16)|(fade((line.rgb>>8)&255)<<8)|fade(line.rgb&255);
        if(!drawScreenLabel(font,line.text,rgb,line.source_height,float(windowWidth)*.5f,y+float(line.source_height)*scale,scale,renderer,overlay,textures,error))return false;
        y+=lineHeight+gap;
    }
    error.clear();return true;
}

struct Options {
    fs::path assets, scene, level, profiles, save = "character.save", liveSave="gameplay.save", capture;
    std::set<std::string> activeConditions;
    std::set<std::string> inactiveConditions;
    std::string module, characterName, playClip;
    std::string actorRow, controllerPolicy, motionNode, cameraRoot;
    std::vector<f::EquipmentVisualDefinition> equipment;
    f::CharacterVisualConfig character;
    std::map<std::string,double> clipRates;
    f::CombatSessionConfig combat;
    std::map<std::string,f::CombatSessionChoice> locomotionChoices;
    std::string meleeBindings;
    bool diagnosticAI=false;
    int attackFrames=0, attackStartFrame=0, targetFrame=-1, saveFrame=-1, loadFrame=-1;
    f::Vec3 actorPosition{};
    std::optional<f::CameraVec3> focus;
    float distance = 0;
    int frames = 0, reloadFrame = 0;
    std::vector<std::pair<std::string,int>> interactRequests; // P16 containers: --interact-at DECLARATION@FRAME (repeatable; default off)
    double fixedStep = 0;
    bool probe = false, timeline = false, saveNow = false;
    bool movable=false, sourceCamera=false, hud=false, freshPlayer=false;
    bool combatText=false;
    std::optional<bool> staticActorAnchor;
    bool retainHiddenActors=false;
    bool runtimeEnemyAI=false;
    bool populationTemplates=false;
    bool runtimeAudio=false;
    fs::path audioAssets,audioTable;
    int audioListener=-1;
    std::string startMode="menu",menuActions,menuReturnActions;
    bool skipBoot=false;std::string introMovie,loadingCapture; // Preview 15 startup boot (--skip-boot, --intro-movie, --loading-capture)
    std::vector<double> bootPresses;std::vector<std::pair<double,fs::path>> bootCaptures;double bootMaxSeconds=0; // boot verification hooks
    fs::path menuAssets,menuUiAssets,menuCaptureDirectory;
    int menuCaptureEvery=6;
    int menuFrames=0,selectedSaveSlot=0;
    unsigned menuClassIndex=0;
    bool verifyFrontendCreation=false,verifyFrontendSlots=false;
    std::string campaignCommands;
    bool campaignTriggers=false; // P16 HOST: --campaign-triggers (off until verified)
    int campaignSkipFrame=-1; std::string campaignStart; // P16 CINE: scripted SKIP press frame; harness start by authored script name
    struct ScheduledSourceCommand {std::string script;std::size_t index=0;int frame=0;};
    std::vector<ScheduledSourceCommand> sourceCommands;
    // P16 SPAWN: --spawn-test TEMPLATE@X,Y,Z@FRAME (debug; empty by default).
    std::vector<f::spawn::SpawnTestRequestV1> spawnTests;
    std::vector<std::pair<std::string,std::string>> containerScriptOverrides; // P16 CONTAINERS2 debug: --container-script DECL=SCRIPT (default none)
    std::vector<f::spawn::SpawnNamedRequestV1> spawnDeclared; // P16 SPAWN: --spawn-declared NAME@FRAME (authored Limbus/PreSpawn declaration; implies --retain-hidden-actors)
    std::vector<f::spawn::SpawnNamedRequestV1> despawnTests;  // P16 SPAWN: --despawn-test NAME@FRAME (live pool slot by population name)
    std::map<std::string,f::OriginalAttackSelection> lifecycleSpawns;
    std::map<std::string,f::CombatSessionChoice> lifecyclePreSpawns;
    f::InputMove2D scriptedMove{};
    int moveFrames=0;
    int moveFromFrame=0; // P16 CONTEXT: scripted move starts at this frame (quiet walk-over checks)
    struct MoveSegmentV1{int start=0;int end=0;f::InputMove2D axis{};};
    std::vector<MoveSegmentV1> moveSegments; // P16 CONTEXT: --move-segment start:end:x,y (quiet walk-over checks)
    bool scriptedRun=true;
    bool sourceBodyBounds=false;
    bool sourceNativeBodies=false;
    std::string sourceRootScopes;
    std::string campaignContextObject;
    bool sourceFloorProbe=false;
    bool sourceFloorMotion=false;
    bool sourceHeadingRotation=false;
    bool sourceTargetPosition=false;
    unsigned hudPortrait=0;
    int profileClickFrame=-1;
    int skillsPageFrame=-1;
    int equipmentPageFrame=-1;
    int faeryPageFrame=-1; // P14 FAERY
    int mapPageFrame=-1; // P16 MAP: --map-page-frame=N opens the character menu on the Map tab
    bool mapLegend=false; float mapZoom=1; // P16 MAP diagnostics: --map-legend shows the legend, --map-zoom=Z sets the zoom at selection
    int questPageFrame=-1; // P16 QUESTUI: test aid, opens the menu on the Quest Log tab at this frame
    std::vector<std::string> bagItemIds; // P14 EQUIP: --bag-item diagnostic rows
    struct MenuRelease {int frame;float x,y;};
    std::vector<MenuRelease> menuReleases;
    std::vector<std::pair<int,int>> skillKeyFrames;
    std::vector<int> pickupFrames; // P14 DROPS: scripted world-item pickup key presses (same action as E)
    std::vector<int> questTalkFrames; // P16 QUESTUI: test aid, scripted interact presses for NPC talk (not gameplay)
    std::vector<std::pair<int,int>> spaceKeyIntervals;
    // P16 QUESTS test aid (not gameplay): frame-scheduled quest bus events, so the real EXE path
    // (bus -> runtime -> CQPG save -> rewards -> banner) can be exercised without scripted combat.
    // --quest-debug-kill FRAME:TEMPLATE:COUNT raises COUNT kill events; --quest-debug-accept FRAME:ROW accepts a row.
    struct QuestDebugEventOption {int frame=-1;bool accept=false;int id=-1;int count=1;};
    std::vector<QuestDebugEventOption> questDebugEvents;
    int menuCloseFrame=-1;
    int pausePageFrame=-1,pauseCloseFrame=-1;
    int windowWidth=0,windowHeight=720,resizeFrame=-1,resizeWidth=0,resizeHeight=0;
};
Options parse(int argc, char** argv) {
    fs::path startup;
    if(argc==1) {auto candidate=fs::absolute(argv[0]).parent_path()/"startup.args";if(fs::exists(candidate))startup=candidate;}
    else if(argc==3&&std::string(argv[1])=="--startup-config")startup=fs::absolute(argv[2]);
    if(!startup.empty()) {
        std::ifstream source(startup);if(!source)throw std::runtime_error("Cannot read startup configuration: "+startup.string());
        std::vector<std::string> arguments{argv[0]};std::string line;
        while(std::getline(source,line)){if(!line.empty()&&line.back()=='\r')line.pop_back();if(line.empty()||line[0]=='#')continue;if(line=="--startup-config"||line.size()>4096||arguments.size()>512)throw std::runtime_error("Invalid startup configuration argument");arguments.push_back(line);}
        if(arguments.size()==1)throw std::runtime_error("Startup configuration is empty");
        fs::current_path(startup.parent_path());std::vector<char*> pointers;for(auto& argument:arguments)pointers.push_back(argument.data());return parse(int(pointers.size()),pointers.data());
    }
    Options o;
    auto choice=[](const std::string& text,bool rootAllowed=false) {
        std::istringstream stream(text);std::string profile,state,variant,path,extra;
        if(!std::getline(stream,profile,':')||!std::getline(stream,state,':')||!std::getline(stream,variant,':')||!std::getline(stream,path,':')||std::getline(stream,extra,':')||profile.empty()||state.empty())throw std::runtime_error("Combat choice must be PROFILE:STATE:VARIANT:LEAF_PATH");
        auto ordinal=[](const std::string& value){if(value.empty()||value.find_first_not_of("0123456789")!=std::string::npos)throw std::runtime_error("Combat hierarchy ordinals must be unsigned integers");return std::size_t(std::stoull(value));};
        f::CombatSessionChoice selected;selected.state=state;selected.variant=ordinal(variant);
        if(!(rootAllowed&&path=="*")){std::istringstream parts(path);while(std::getline(parts,extra,','))selected.leafPath.push_back(ordinal(extra));if(selected.leafPath.empty())throw std::runtime_error("Combat path must be explicit");}
        return std::make_pair(profile,selected);
    };
    auto pair=[](const std::string& text){auto split=text.find('=');if(split==std::string::npos||split==0||split+1==text.size())throw std::runtime_error("Expected PROFILE=VALUE");return std::make_pair(text.substr(0,split),text.substr(split+1));};
    auto vector=[](std::string text) {std::replace(text.begin(),text.end(),',',' ');std::istringstream s(text);f::Vec3 v;std::string extra;if(!(s>>v.x>>v.y>>v.z)||s>>extra||!std::isfinite(v.x)||!std::isfinite(v.y)||!std::isfinite(v.z))throw std::runtime_error("Expected finite X,Y,Z");return v;};
    for(int i=1;i<argc;++i) {
        const std::string arg=argv[i];
        auto value=[&]() -> std::string { if(++i>=argc) throw std::runtime_error("Missing value for "+arg); return argv[i]; };
        if(arg=="--assets") o.assets=value();
        else if(arg=="--scene") o.scene=value();
        else if(arg=="--level") o.level=value();
        else if(arg=="--profiles") o.profiles=value();
        else if(arg=="--actor-row") o.actorRow=value();
        else if(arg=="--combat-bindings") o.meleeBindings=value();
        else if(arg=="--combat-player") {o.combat.playerProfileId=value();o.combat.playerId=std::numeric_limits<f::ActorId>::max();}
        else if(arg=="--combat-seed") {auto text=value();std::size_t used=0;auto seed=std::stoull(text,&used);if(used!=text.size()||seed>UINT32_MAX||text.empty()||text[0]=='-')throw std::runtime_error("Combat seed must be uint32");o.combat.diagnosticRngSeed=std::uint32_t(seed);}
        else if(arg=="--combat-action") {auto c=choice(value());o.combat.profiles[c.first].action=c.second;}
        else if(arg=="--combat-sequence") {auto c=choice(value(),true);auto& selected=o.combat.profiles[c.first].sequenceAction;if(!selected)selected=f::OriginalAttackSelection{};selected->state=c.second.state;selected->variant=c.second.variant;selected->group_path=c.second.leafPath;}
        else if(arg=="--combat-idle") {auto c=choice(value());o.combat.profiles[c.first].initialIdle=c.second;}
        else if(arg=="--animation-only") {auto id=value();if(id.empty())throw std::runtime_error("Animation-only profile must be nonempty");auto& profile=o.combat.profiles[id];profile.animationOnly=true;profile.retainedPhaseClock=true;profile.propertyOptions.refill_vitals=false;}
        else if(arg=="--combat-locomotion") {auto c=choice(value());o.locomotionChoices[c.first]=c.second;}
        else if(arg=="--retain-hidden-actors") o.retainHiddenActors=true;
        else if(arg=="--spawn-test") {f::spawn::SpawnTestRequestV1 test;std::string parseError;if(!f::spawn::parse_spawn_test_v1(value(),test,parseError))throw std::runtime_error(parseError);o.spawnTests.push_back(test);} // P16 SPAWN
        else if(arg=="--spawn-declared") {f::spawn::SpawnNamedRequestV1 request;std::string parseError;if(!f::spawn::parse_spawn_named_v1(value(),request,parseError))throw std::runtime_error("--spawn-declared: "+parseError);o.spawnDeclared.push_back(request);o.retainHiddenActors=true;} // P16 SPAWN
        else if(arg=="--container-script") {const auto text=value();const auto eq=text.find('=');if(eq==std::string::npos||eq==0||eq+1>=text.size())throw std::runtime_error("--container-script expects DECLARATION=SCRIPT");o.containerScriptOverrides.emplace_back(text.substr(0,eq),text.substr(eq+1));} // P16 CONTAINERS2 debug (plant/zombie OnOpen variants on Swamp)
        else if(arg=="--despawn-test") {f::spawn::SpawnNamedRequestV1 request;std::string parseError;if(!f::spawn::parse_spawn_named_v1(value(),request,parseError))throw std::runtime_error("--despawn-test: "+parseError);o.despawnTests.push_back(request);} // P16 SPAWN
        else if(arg=="--enemy-ai") o.runtimeEnemyAI=true;
        else if(arg=="--population-templates") o.populationTemplates=true;
        else if(arg=="--audio") o.runtimeAudio=true;
        else if(arg=="--audio-assets") o.audioAssets=value();
        else if(arg=="--audio-table") o.audioTable=value();
        else if(arg=="--audio-listener") o.audioListener=std::stoi(value());
        else if(arg=="--start-mode") {o.startMode=value();if(o.startMode!="menu"&&o.startMode!="swamp")throw std::runtime_error("Start mode must be menu or swamp");}
        else if(arg=="--menu-assets") o.menuAssets=value();
        else if(arg=="--menu-ui-assets") o.menuUiAssets=value();
        else if(arg=="--menu-capture-directory") o.menuCaptureDirectory=value();
        else if(arg=="--menu-capture-every") {o.menuCaptureEvery=std::stoi(value());if(o.menuCaptureEvery<1)throw std::runtime_error("Menu capture interval must be positive");}
        else if(arg=="--menu-actions") o.menuActions=value();
        else if(arg=="--menu-return-actions") o.menuReturnActions=value();
        else if(arg=="--menu-class-index") {const auto index=std::stoi(value());if(index<0||index>2)throw std::runtime_error("Menu class index must be 0, 1 or 2");o.menuClassIndex=unsigned(index);}
        else if(arg=="--pause-page-frame") o.pausePageFrame=std::stoi(value());
        else if(arg=="--pause-close-frame") o.pauseCloseFrame=std::stoi(value());
        else if(arg=="--menu-frames") {o.menuFrames=std::stoi(value());if(o.menuFrames<0)throw std::runtime_error("Menu frame limit must be nonnegative");}
        else if(arg=="--verify-frontend-create") o.verifyFrontendCreation=true;
        else if(arg=="--verify-frontend-slots") o.verifyFrontendSlots=true;
        else if(arg=="--save-slot") {o.selectedSaveSlot=std::stoi(value());if(o.selectedSaveSlot<0)throw std::runtime_error("Save slot must be nonnegative");}
        else if(arg=="--lifecycle-spawn") {auto c=choice(value(),true);f::OriginalAttackSelection selected;selected.state=c.second.state;selected.variant=c.second.variant;selected.group_path=c.second.leafPath;o.lifecycleSpawns[c.first]=std::move(selected);}
        else if(arg=="--lifecycle-prespawn") {auto c=choice(value());o.lifecyclePreSpawns[c.first]=c.second;}
        else if(arg=="--campaign-commands") o.campaignCommands=value();
        else if(arg=="--campaign-triggers") o.campaignTriggers=true; // P16 HOST
        else if(arg=="--campaign-skip-frame") o.campaignSkipFrame=std::stoi(value()); // P16 CINE
        else if(arg=="--campaign-start") o.campaignStart=value(); // P16 CINE
        else if(arg=="--campaign-command") {auto text=value();std::istringstream parts(text);Options::ScheduledSourceCommand c;std::string index,frame,extra;if(!std::getline(parts,c.script,':')||!std::getline(parts,index,':')||!std::getline(parts,frame,':')||std::getline(parts,extra,':')||c.script.empty()||index.empty()||frame.empty()||index.find_first_not_of("0123456789")!=std::string::npos||frame.find_first_not_of("0123456789")!=std::string::npos)throw std::runtime_error("Campaign command must be SCRIPT:INDEX:FRAME");c.index=std::stoull(index);c.frame=std::stoi(frame);o.sourceCommands.push_back(std::move(c));}
        else if(arg=="--combat-react") {auto c=choice(value());o.combat.profiles[c.first].reaction=c.second;}
        else if(arg=="--combat-death") {auto c=choice(value());o.combat.profiles[c.first].death=c.second;}
        else if(arg=="--combat-marker") {auto c=pair(value());o.combat.profiles[c.first].damageMarkerNames.push_back(c.second);}
        else if(arg=="--combat-ai-gate") {auto c=pair(value());o.combat.profiles[c.first].requiredAIState=c.second;}
        else if(arg=="--combat-state") {auto c=pair(value());o.combat.profiles[c.first].originalCombatState=std::stoi(c.second);}
        else if(arg=="--combat-auto") o.diagnosticAI=true;
        else if(arg=="--equipped-item") o.combat.equippedItemIds.push_back(value());
        else if(arg=="--bag-item") o.bagItemIds.push_back(value()); // P14 EQUIP diagnostic: unequipped bag row (see direct bootstrap)
        else if(arg=="--combat-main-item") o.combat.mainItemId=value();
        else if(arg=="--attack-frames") {o.attackFrames=std::stoi(value());if(o.attackFrames<1)throw std::runtime_error("Attack frames must be positive");}
        else if(arg=="--attack-start-frame") {o.attackStartFrame=std::stoi(value());if(o.attackStartFrame<0)throw std::runtime_error("Attack start frame must be nonnegative");}
        else if(arg=="--target-frame") {o.targetFrame=std::stoi(value());if(o.targetFrame<0)throw std::runtime_error("Target frame must be nonnegative");}
        else if(arg=="--move") o.movable=true;
        else if(arg=="--fresh-player") o.freshPlayer=true;
        else if(arg=="--motion-node") o.motionNode=value();
        else if(arg=="--controller-policy") o.controllerPolicy=value();
        else if(arg=="--original-camera") o.sourceCamera=true;
        else if(arg=="--original-native-bodies") o.sourceNativeBodies=true;
        else if(arg=="--source-root-scopes") o.sourceRootScopes=value();
        else if(arg=="--campaign-context-object") o.campaignContextObject=value();
        else if(arg=="--source-floor-probe") o.sourceFloorProbe=true;
        else if(arg=="--original-floor-motion") o.sourceFloorMotion=true;
        else if(arg=="--original-heading-rotation") o.sourceHeadingRotation=true;
        else if(arg=="--original-target-position") o.sourceTargetPosition=true;
        else if(arg=="--profile-click-frame") o.profileClickFrame=std::stoi(value());
        else if(arg=="--skills-page-frame") o.skillsPageFrame=std::stoi(value());
        else if(arg=="--equipment-page-frame") o.equipmentPageFrame=std::stoi(value());
        else if(arg=="--faery-page-frame") o.faeryPageFrame=std::stoi(value()); // P14 FAERY
        else if(arg=="--map-page-frame") o.mapPageFrame=std::stoi(value()); // P16 MAP
        else if(arg=="--map-legend") o.mapLegend=true; // P16 MAP diagnostic
        else if(arg=="--map-zoom") o.mapZoom=std::stof(value()); // P16 MAP diagnostic
        else if(arg=="--quest-page-frame") o.questPageFrame=std::stoi(value()); // P16 QUESTUI
        else if(arg=="--menu-release") {
            std::istringstream input(value());Options::MenuRelease release{};char first=0,second=0;
            if(!(input>>release.frame>>first>>release.x>>second>>release.y)||first!=':'||second!=':'||release.frame<0||!std::isfinite(release.x)||!std::isfinite(release.y))throw std::runtime_error("Menu release requires FRAME:AUTHORED_X:AUTHORED_Y");
            o.menuReleases.push_back(release);
        }
        else if(arg=="--menu-close-frame") o.menuCloseFrame=std::stoi(value());
        else if(arg=="--quest-talk-frame") {const int frame=std::stoi(value());if(frame<0)throw std::runtime_error("Quest talk frame must be nonnegative");o.questTalkFrames.push_back(frame);}
        else if(arg=="--pickup-frame") {const int frame=std::stoi(value());if(frame<0)throw std::runtime_error("Pickup frame must be nonnegative");o.pickupFrames.push_back(frame);}
        else if(arg=="--skill-key-frame") {
            const auto text=value();const auto split=text.find(':');
            if(split==std::string::npos)throw std::runtime_error("HUD key frame requires FRAME:KEY(1..5)");
            const int frame=std::stoi(text.substr(0,split)),key=std::stoi(text.substr(split+1));
            if(frame<0||key<1||key>5)throw std::runtime_error("HUD key frame requires nonnegative frame and key1..5");
            o.skillKeyFrames.emplace_back(frame,key);
        }
        else if(arg=="--quest-debug-kill"||arg=="--quest-debug-accept") {
            const auto text=value();const auto first=text.find(':');
            const auto second=first==std::string::npos?std::string::npos:text.find(':',first+1);
            Options::QuestDebugEventOption item;item.accept=arg=="--quest-debug-accept";
            if(first==std::string::npos||(item.accept?second!=std::string::npos:second==std::string::npos))
                throw std::runtime_error("Quest debug event requires FRAME:ID or FRAME:TEMPLATE:COUNT");
            item.frame=std::stoi(text.substr(0,first));
            item.id=std::stoi(text.substr(first+1,item.accept?std::string::npos:second-first-1));
            if(!item.accept)item.count=std::stoi(text.substr(second+1));
            if(item.frame<0||item.count<1||item.count>64)throw std::runtime_error("Quest debug event frame/count out of range");
            o.questDebugEvents.push_back(item);
        }
        else if(arg=="--space-key-interval") {
            const auto text=value();const auto split=text.find(':');
            if(split==std::string::npos)throw std::runtime_error("Space key interval requires START:FRAME_COUNT");
            const int start=std::stoi(text.substr(0,split)),count=std::stoi(text.substr(split+1));
            if(start<0||count<1||std::int64_t(start)+count>std::numeric_limits<int>::max())throw std::runtime_error("Space key interval requires nonnegative start and positive bounded count");
            o.spaceKeyIntervals.emplace_back(start,count);
        }
        else if(arg=="--window-size"||arg=="--resize-at") {
            auto dimensions=value();
            if(arg=="--resize-at") {const auto split=dimensions.find(':');if(split==std::string::npos)throw std::runtime_error("Resize requires FRAME:WIDTH,HEIGHT");o.resizeFrame=std::stoi(dimensions.substr(0,split));dimensions.erase(0,split+1);}
            const auto comma=dimensions.find(',');if(comma==std::string::npos)throw std::runtime_error("Window size requires WIDTH,HEIGHT");
            const auto width=std::stoi(dimensions.substr(0,comma)),height=std::stoi(dimensions.substr(comma+1));
            if(width<=0||height<=0)throw std::runtime_error("Window dimensions must be positive");
            if(arg=="--window-size"){o.windowWidth=width;o.windowHeight=height;}else{o.resizeWidth=width;o.resizeHeight=height;}
        }
        else if(arg=="--original-actor-anchor") {auto mode=value();if(mode!="forward"&&mode!="static")throw std::runtime_error("Actor camera anchor must be forward or static");o.staticActorAnchor=mode=="static";}
        else if(arg=="--camera-root") o.cameraRoot=value();
        else if(arg=="--hud") o.hud=true;
        else if(arg=="--combat-text") o.combatText=true;
        else if(arg=="--hud-portrait") {auto n=std::stoi(value());if(n<0||n>2)throw std::runtime_error("HUD portrait source frame must be 0..2");o.hudPortrait=unsigned(n);}
        else if(arg=="--move-axis") {auto v=vector(value());o.scriptedMove={v.x,v.y};}
        else if(arg=="--move-frames") {o.moveFrames=std::stoi(value());if(o.moveFrames<1)throw std::runtime_error("Move frames must be positive");}
        else if(arg=="--move-segment") {const auto text=value();const auto c1=text.find(':'),c2=text.find(':',c1==std::string::npos?0:c1+1),c3=text.find(',',c2==std::string::npos?0:c2+1);if(c1==std::string::npos||c2==std::string::npos||c3==std::string::npos)throw std::runtime_error("Move segment must be start:end:x,y");Options::MoveSegmentV1 seg;seg.start=std::stoi(text.substr(0,c1));seg.end=std::stoi(text.substr(c1+1,c2-c1-1));seg.axis={std::stof(text.substr(c2+1,c3-c2-1)),std::stof(text.substr(c3+1))};o.moveSegments.push_back(seg);}
        else if(arg=="--move-from-frame") {o.moveFromFrame=std::stoi(value());if(o.moveFromFrame<0)throw std::runtime_error("Move start frame must be nonnegative");}
        else if(arg=="--move-run") o.scriptedRun=true;
        else if(arg=="--move-walk") o.scriptedRun=false;
        else if(arg=="--original-body-bounds") o.sourceBodyBounds=true;
        else if(arg=="--attach") {auto v=value();auto split=v.find('@');if(split==std::string::npos)throw std::runtime_error("Attachment must be MODEL@AUTHORED_ANCHOR");f::EquipmentVisualDefinition d;d.id=v;d.model_uri=v.substr(0,split);d.anchor_name=v.substr(split+1);o.equipment.push_back(std::move(d));}
        else if(arg=="--condition-active") o.activeConditions.insert(value());
        else if(arg=="--condition-inactive") o.inactiveConditions.insert(value());
        else if(arg=="--module") o.module=value();
        else if(arg=="--save") o.save=value();
        else if(arg=="--game-save") o.liveSave=value();
        else if(arg=="--combat-motion-root") {auto v=value();auto split=v.find('=');if(split==std::string::npos||split==0||split+1==v.size())throw std::runtime_error("Combat motion root must be PROFILE=ROOT");o.combat.profiles[v.substr(0,split)].motionRoot=v.substr(split+1);}
        else if(arg=="--combat-retained-phase") o.combat.profiles[value()].retainedPhaseClock=true;
        else if(arg=="--combat-source-combo") o.combat.profiles[value()].sourceCombo=true;
        else if(arg=="--resume-game") o.loadFrame=0;
        else if(arg=="--save-frame") {o.saveFrame=std::stoi(value());if(o.saveFrame<0)throw std::runtime_error("Save frame must be nonnegative");}
        else if(arg=="--load-frame") {o.loadFrame=std::stoi(value());if(o.loadFrame<0)throw std::runtime_error("Load frame must be nonnegative");}
        else if(arg=="--character-name") o.characterName=value();
        else if(arg=="--save-now") o.saveNow=true;
        else if(arg=="--play") o.playClip=value();
        else if(arg=="--position") o.actorPosition=vector(value());
        else if(arg=="--focus") {auto v=vector(value());o.focus=f::CameraVec3{v.x,v.y,v.z};}
        else if(arg=="--distance") {o.distance=std::stof(value());if(!std::isfinite(o.distance)||o.distance<=0)throw std::runtime_error("Distance must be positive");}
        else if(arg=="--reload-frame") {o.reloadFrame=std::stoi(value());if(o.reloadFrame<1)throw std::runtime_error("Reload frame must be positive");}
        else if(arg=="--interact-at") {const auto at=value();const auto sep=at.rfind('@');if(sep==std::string::npos||sep==0||sep+1>=at.size())throw std::runtime_error("--interact-at expects DECLARATION@FRAME");const int frame=std::stoi(at.substr(sep+1));if(frame<0)throw std::runtime_error("Interact frame must be nonnegative");o.interactRequests.emplace_back(at.substr(0,sep),frame);}
        else if(arg=="--model") o.character.model_path=value();
        else if(arg=="--template") o.character.template_clip_path=value();
        else if(arg=="--idle") o.character.animation_paths[0]=value();
        else if(arg=="--walk") o.character.animation_paths[1]=value();
        else if(arg=="--attack") o.character.animation_paths[2]=value();
        else if(arg=="--skin") o.character.skin_id_contains=value();
        else if(arg=="--clip") {auto v=value();auto split=v.find('=');if(split==std::string::npos)throw std::runtime_error("Clip must be NAME=PATH");o.character.clips.emplace_back(v.substr(0,split),v.substr(split+1));}
        else if(arg=="--clip-rate") {auto v=value();auto split=v.find('=');if(split==std::string::npos||split==0)throw std::runtime_error("Clip rate must be NAME=POSITIVE_RATE");auto rate=std::stod(v.substr(split+1));if(!std::isfinite(rate)||rate<=0)throw std::runtime_error("Clip rate must be finite and positive");o.clipRates[v.substr(0,split)]=rate;}
        else if(arg=="--frames") {o.frames=std::stoi(value());if(o.frames<1) throw std::runtime_error("Frames must be positive");}
        else if(arg=="--capture") o.capture=value();
        else if(arg=="--fixed-step") {o.fixedStep=std::stod(value());if(!std::isfinite(o.fixedStep)||o.fixedStep<=0||o.fixedStep>1)throw std::runtime_error("Fixed step must be in (0,1]");}
        else if(arg=="--probe") o.probe=true;
        else if(arg=="--skip-boot") o.skipBoot=true;  // Preview 15: tests run without logo/movie/title
        else if(arg=="--intro-movie") o.introMovie=value();
        else if(arg=="--loading-capture") o.loadingCapture=value();  // <prefix>: writes <prefix>-NNN.ppm per loading stage
        else if(arg=="--boot-press") o.bootPresses.push_back(std::stod(value()));  // scripted press/tap (verification)
        else if(arg=="--boot-max-seconds") o.bootMaxSeconds=std::stod(value());
        else if(arg=="--boot-capture") {  // <seconds>=<file.ppm>, written by the boot runner
            const auto spec=value();const auto eq=spec.find('=');
            if(eq==std::string::npos)throw std::runtime_error("--boot-capture expects <seconds>=<file>");
            o.bootCaptures.push_back({std::stod(spec.substr(0,eq)),fs::path(spec.substr(eq+1))});
        }
        else if(arg=="--timeline") o.timeline=true;
        else if(arg=="--help") {
            std::cout<<"dh-foundation [--assets DIR] [--scene RELATIVE_BDAE | --level RELATIVE_MLX] [--module NODE]\n"
              " [--model MODEL --template CLIP --idle CLIP --walk CLIP --attack CLIP --skin FILTER]\n"
              " [--clip NAME=PATH ... --clip-rate NAME=RATE --play NAME] [--save FILE --save-now --character-name NAME]\n"
              " [--position X,Y,Z --focus X,Y,Z --distance UNITS] [--reload-frame N]\n"
              " [--profiles PROFILE_XML --condition-active EXPLICIT_CONDITION ...]\n"
              " [--actor-row ROW --fresh-player --move --motion-node NODE --controller-policy XML]\n"
              " [--original-body-bounds] (source joint/rest AABB provider, distinct from movement solver)\n"
              " [--original-camera --camera-root DIR --original-actor-anchor forward|static] [--attach MODEL@ANCHOR] [--hud --hud-portrait N]\n"
              " [--move-axis X,Y,0 --move-frames N --move-run] (diagnostic input through the same controller)\n"
              " [--combat-bindings XML --combat-player PROFILE --combat-seed UINT32]\n"
              " [--combat-action/idle/react/death PROFILE:STATE:VARIANT:LEAF_PATH]\n"
              " [--combat-locomotion ALIAS:STATE:VARIANT:LEAF_PATH] (explicit source leaf)\n"
              " [--retain-hidden-actors --lifecycle-spawn PROFILE:STATE:VARIANT:GROUP_PATH]\n"
              " [--lifecycle-prespawn PROFILE:STATE:VARIANT:LEAF_PATH]\n"
              " [--campaign-commands XML --campaign-command SCRIPT:INDEX:FRAME] (serialized command diagnostic, global context)\n"
              " [--combat-sequence PROFILE:STATE:VARIANT:GROUP_PATH] ('*' executes the root)\n"
              " [--combat-marker PROFILE=MARKER --equipped-item ITEM_NAME --combat-main-item ITEM_NAME]\n"
              " [--combat-ai-gate PROFILE=STATE --combat-state PROFILE=INTEGER --combat-auto]\n"
              " [--attack-frames N --attack-start-frame N --target-frame N] (diagnostic combat requests)\n"
              " [--probe] [--frames N --capture IMAGE.ppm --fixed-step SECONDS] [--timeline]\n"
              "WASD/QE camera, arrows rotate, 1/2/3 animations, T timeline, F5 save, F9 load, Esc exit.\n";
            std::exit(0);
        } else throw std::runtime_error("Unknown argument: "+arg);
    }
    if(o.movable&&(o.actorRow.empty()||o.motionNode.empty()||o.controllerPolicy.empty()||o.level.empty()))throw std::runtime_error("Movement requires an original actor row, motion node, controller policy and level");
    if(o.hud&&o.actorRow.empty())throw std::runtime_error("HUD requires actual bound original actor properties");
    if(o.sourceBodyBounds&&o.actorRow.empty())throw std::runtime_error("Source body bounds require actual original actor properties");
    if(!o.meleeBindings.empty()) {
        if(o.actorRow.empty()||o.combat.playerProfileId.empty()||!o.combat.diagnosticRngSeed)throw std::runtime_error("Combat requires original actor row, explicit player profile and diagnostic RNG seed");
        o.combat.tableRoot="original-cache/data/pydata";
        for(auto& entry:o.combat.profiles) {entry.second.propertyOptions.refill_vitals=!entry.second.animationOnly;entry.second.diagnosticAIEnabled=!entry.second.animationOnly&&o.diagnosticAI&&entry.first!=o.combat.playerProfileId;entry.second.customization.allow_missing_animation_targets=true;}
    }
    return o;
}
void capture(const fs::path& path,int w,int h) {
    std::vector<unsigned char> pixels(std::size_t(w)*h*3);
    glPixelStorei(GL_PACK_ALIGNMENT,1);glReadBuffer(GL_BACK);
    glReadPixels(0,0,w,h,GL_RGB,GL_UNSIGNED_BYTE,pixels.data());
    std::ofstream out(path,std::ios::binary);if(!out) throw std::runtime_error("Cannot write capture");
    out<<"P6\n"<<w<<' '<<h<<"\n255\n";
    for(int y=h-1;y>=0;--y) out.write(reinterpret_cast<char*>(pixels.data()+std::size_t(y)*w*3),w*3);
    if(!out) throw std::runtime_error("Capture write failed");
}
f::Camera camera(const f::CameraPose& p) {
    f::Camera c;c.eye={p.position.x,p.position.y,p.position.z};c.target={p.target.x,p.target.y,p.target.z};
    c.up={p.up.x,p.up.y,p.up.z};c.verticalFovDegrees=p.verticalFovDegrees;c.nearPlane=.5f;c.farPlane=100000.f;return c;
}
bool project(const f::Camera& camera,f::Vec3 point,int width,int height,std::array<float,2>& screen) {
    f::CameraPose pose;pose.position={camera.eye.x,camera.eye.y,camera.eye.z};pose.target={camera.target.x,camera.target.y,camera.target.z};pose.up={camera.up.x,camera.up.y,camera.up.z};
    const auto view=f::cameraViewMatrix(pose),projection=f::cameraProjectionMatrix(camera.verticalFovDegrees,camera.aspectRatio>0?camera.aspectRatio:float(width)/height,camera.nearPlane,camera.farPlane);
    float world[]{point.x,point.y,point.z,1},eye[4]{},clip[4]{};
    for(unsigned row=0;row<4;++row)for(unsigned col=0;col<4;++col)eye[row]+=view[col*4+row]*world[col];
    for(unsigned row=0;row<4;++row)for(unsigned col=0;col<4;++col)clip[row]+=projection[col*4+row]*eye[col];
    if(!std::isfinite(clip[3])||clip[3]<=0)return false;
    const float x=clip[0]/clip[3],y=clip[1]/clip[3],z=clip[2]/clip[3];
    if(x< -1||x>1||y< -1||y>1||z< -1||z>1)return false;
    screen={(x+1)*width*.5f,(1-y)*height*.5f};return true;
}
int main(int argc,char** argv) {
    try {
        const auto executablePath=fs::absolute(argv[0]);
        auto options=parse(argc,argv);
        if(options.assets.empty()) options.assets=f::AssetCatalog::discover_root(executablePath);
        f::AssetCatalog assets(options.assets);
        const auto launchOptions=options;
        // P14 schema: LevelList (data/levels_pyarray.bin) for slot metadata; loaded once, on first use.
        dh2::data::LevelTables metadataLevels;bool metadataLevelsTried=false,metadataLevelsLoaded=false;
        const auto loadMetadataLevels=[&](const f::AssetCatalog& catalog)->const dh2::data::LevelTables* {
            if(!metadataLevelsTried) {
                metadataLevelsTried=true;std::string levelsError;
                metadataLevelsLoaded=f::menu_metadata::load_level_tables(catalog,metadataLevels,levelsError);
                if(!metadataLevelsLoaded)std::cerr<<"Menu metadata LevelList diagnostic: "<<levelsError<<'\n';
            }
            return metadataLevelsLoaded?&metadataLevels:nullptr;
        };
        // Source SG_SetSaveDate + SG_SetLevelId at a profile save point (Level::SG_SavePlayer, F5/checkpoint, menu return).
        const auto stampSaveMetadata=[&](f::CharacterState& profile,const std::string& levelUri) {
            f::menu_metadata::stamp_menu_metadata_for_level(profile,std::uint32_t(std::time(nullptr)),loadMetadataLevels(assets),levelUri);
        };
        bool returnMenuScriptConsumed=false;
        bool bootShown=false;  // Preview 15: the boot runs once per process, not on return-to-menu
        f::Window window;f::Renderer renderer;bool windowOpened=false;
        for(;;) {
        auto sharedCharacter=std::make_shared<f::CharacterState>(f::make_default_character());
        auto& state=*sharedCharacter;
        f::frontend::creation::RuntimeCreationSourceOwnerV1 menuSourceOwner;
        dh2::data::LootRandom8V2 creationRandom{};
        bool frontendStarted=false;
        std::optional<dh2::data::CombatRandom> frontendRandomState;
        if(options.startMode=="menu"&&!options.probe) {
            namespace creation=f::frontend::creation;
            const int height=options.windowHeight,width=options.windowWidth?options.windowWidth:1280;
            if(!windowOpened) {
                if(!window.open("Dungeon Hunter II",width,height)||!renderer.initialize(window.width(),window.height()))
                    throw std::runtime_error("Frontend window/renderer: "+window.error());
                windowOpened=true;
            } else if((window.width()!=width||window.height()!=height)&&!window.resize(width,height))
                throw std::runtime_error("Frontend retained window resize: "+window.error());
            // Preview 15 startup boot (original order): intro movie (contains the Gameloft logo, SKIP) -> touch to
            // continue -> main menu, first menu entry only. --skip-boot bypasses it for tests; boot asset failures
            // are logged and the menu still runs.
            if(!options.skipBoot&&!bootShown) {
                bootShown=true;
                f::startup::BootRunConfig bootConfig;bootConfig.assets=&assets;
                bootConfig.intro_movie=options.introMovie.empty()?assets.root()/"converted-media"/"intro_v1.mpg":fs::path(options.introMovie);
                bootConfig.window_width=width;
                bootConfig.scripted_presses=options.bootPresses;bootConfig.captures=options.bootCaptures;
                bootConfig.max_seconds=options.bootMaxSeconds;
                bootConfig.capture=[](const fs::path& p,int w,int h){capture(p,w,h);};
                // The movie soundtrack runs on its own mixer; the boot owns the platform output while it runs.
                auto bootMixer=std::make_unique<dh2::audio::AudioMixerV34>();
                f::audio::WinmmAudioOutput bootOutput(*bootMixer);
                std::string bootAudioError;
                if(bootOutput.open(bootAudioError)) {
                    bootConfig.audio_mixer=bootMixer.get();
                    bootConfig.audio_latency_frames=std::uint64_t(f::audio::kWinmmBufferCount)*f::audio::kWinmmFramesPerBuffer;
                    bootConfig.audio_pump=[&bootOutput](){std::string e;if(!bootOutput.update(e)){static bool reported=false;if(!reported){reported=true;std::cerr<<"Boot audio pump: "<<e<<std::endl;}}};
                } else std::cerr<<"Boot audio unavailable ("<<bootAudioError<<"); the movie runs on the wall clock"<<std::endl;
                const auto boot=f::startup::run_boot_v1(window,renderer,bootConfig);
                bootOutput.close();
                // std::endl flushes: verification jobs may be killed after the boot ends.
                std::cout<<"Boot outcome="<<int(boot.outcome)<<" movie=\""<<boot.movie_status<<"\" movie_frames="<<boot.movie_frames_shown
                         <<" movie_clock=\""<<boot.movie_clock<<"\" soundtrack_seconds="<<boot.soundtrack_seconds
                         <<" soundtrack_duration="<<boot.soundtrack_duration<<" soundtrack_released="<<int(boot.soundtrack_released)
                         <<" seconds="<<boot.seconds<<std::endl;
                if(!boot.error.empty())std::cerr<<"Boot: "<<boot.error<<std::endl;
                if(boot.outcome==f::startup::BootRunOutcome::quit)return 0;
            }
            // The caller owns this startup stream. Named fields are transferred
            // after creation; population and combat continue its call count.
            creationRandom={options.combat.diagnosticRngSeed.value_or(0),0};
            if(!options.combat.diagnosticRngSeed)creationRandom.seed=GetTickCount();
            const f::AssetCatalog menuSource(options.menuAssets.empty()?assets.root():options.menuAssets);
            auto& sourceOwner=menuSourceOwner;std::string menuError;
            if(!creation::load_runtime_creation_source_v1(menuSource,creationRandom,sourceOwner,menuError))
                throw std::runtime_error("Frontend source tables: "+menuError);
            const auto profileBasePath=fs::absolute(options.save);
            const auto slotPath=[&](int slot) {
                if(slot<0||slot>3)throw std::runtime_error("Profile slot is outside the original menu range");
                if(slot==0)return profileBasePath;
                return profileBasePath.parent_path()/(profileBasePath.stem().string()+"-slot-"+std::to_string(slot)+profileBasePath.extension().string());
            };
            int selectedMenuSlot=options.selectedSaveSlot;
            int assignedSlot=-1;creation::RuntimeCreationFlowServicesV1 flowServices;
            flowServices.shared_state=sharedCharacter;flowServices.source=sourceOwner.source();
            flowServices.select_new_profile=[&](const std::string& name,const std::string& cls,int& slot,
                creation::RuntimeCreationRequestV1& request,std::string& e) {
                slot=selectedMenuSlot;const auto path=slotPath(slot);
                if(fs::exists(path)){e="Selected profile file already exists; select/load it explicitly";return false;}
                request.shared_state=sharedCharacter;request.character_id="profile-slot-"+std::to_string(slot);
                request.player_name=name;request.class_token=cls;request.save_path=path;
                request.source_timer=GetTickCount();request.saved_date=std::uint32_t(std::time(nullptr));
                e.clear();return true;
            };
            flowServices.selected_save_path=[&](int slot,fs::path& path,std::string& e) {
                if(slot!=selectedMenuSlot){e="Profile selection differs from the actual selected slot";return false;}
                path=slotPath(slot);if(!fs::is_regular_file(path)){e="Selected profile file is unavailable";return false;}e.clear();return true;
            };
            flowServices.assign_selected_slot=[&](int slot,int player,std::string& e) {
                if(player!=0||slot!=selectedMenuSlot||!fs::is_regular_file(slotPath(slot))){e="Selected local profile assignment is invalid";return false;}
                assignedSlot=slot;options.selectedSaveSlot=slot;e.clear();return true;
            };
            flowServices.start_same_state=[&](const std::shared_ptr<f::CharacterState>& selected,int difficulty,std::string& e) {
                if(selected!=sharedCharacter||assignedSlot!=options.selectedSaveSlot){e="Selected profile/start owner is unavailable";return false;}
                // P14 schema: NativeStartGame sets CurrentDifficulty; the menu may choose 0..UnlockedDiff (profile field, no longer fixed to Normal).
                if(difficulty<0||difficulty>f::character_unlocked_difficulty(*selected)){e="Selected difficulty is not unlocked for this profile";return false;}
                selected->current_difficulty=difficulty;
                if(selected->stats.level>std::uint32_t(INT32_MAX/256)){e="Saved level exceeds the source property scale";return false;}
                const auto* selectedClass=creation::find_class(selected->class_id);
                if(!selectedClass){e="Saved class is not an authored playable base profile";return false;}
                options.hudPortrait=selectedClass->menu_index;
                f::ActorProfileLibrary selectedProfiles;
                if(options.profiles.empty())options.profiles="actor-profiles-v2.xml";
                if(!selectedProfiles.load(assets,options.profiles.generic_string(),e))return false;
                const auto* profile=selectedProfiles.find(selected->class_id);
                if(!profile){e="Selected class source visual profile is absent";return false;}
                f::ActorCustomization customization;customization.use_authored_modular_defaults=true;
                customization.allow_missing_animation_targets=true;
                f::CharacterVisualConfig sourceVisual;
                if(!f::make_visual_config(assets,*profile,customization,sourceVisual,e))return false;
                sourceVisual.controller_ids={"MC_Head__naked-mesh-skin"};
                std::vector<f::EquipmentVisualDefinition> sourceWeapons;
                const auto& items=sourceOwner.source().loot.items();
                for(const auto& equipped:selected->equipment) {
                    if(equipped.equipment_set>0)continue;
                    const auto instance=std::find_if(selected->inventory.begin(),selected->inventory.end(),[&](const auto& value){return value.instance_id==equipped.item_instance_id;});
                    if(instance==selected->inventory.end()){e="Selected equipment instance is absent";return false;}
                    const auto* item=dh2::data::item(items,dh2::data::item_id(items,instance->definition_id));
                    if(!item){e="Selected equipment definition is absent in source ItemTable";return false;}
                    if(item->name.rfind("MC_Torso_",0)==0||item->name.rfind("MC_Feet_",0)==0||item->name.rfind("MC_Hands_",0)==0)
                        sourceVisual.controller_ids.push_back(item->name+"-mesh-skin");
                    if(item->name.rfind("MC_RWeapon_",0)==0) {
                        if(equipped.source_slot!=1&&equipped.source_slot!=2){e="Source weapon uses an unsupported attachment slot";return false;}
                        sourceWeapons.push_back({instance->instance_id,item->name+".bdae",
                            equipped.source_slot==1?"anchor_weapon_right_offset":"anchor_weapon_left_offset"});
                    }
                }
                sourceVisual.expected_controller_count=unsigned(sourceVisual.controller_ids.size());
                sourceVisual.skin_id_contains.clear();
                // Generic movement still uses these public aliases; each names
                // an actual clip from this selected class's own source bank.
                for(const auto& binding:std::vector<std::pair<std::string,std::string>>{{"idle","Idle"},{"walk","Walk"}}) {
                    const auto clip=std::find_if(sourceVisual.clips.begin(),sourceVisual.clips.end(),[&](const auto& value){return value.first==binding.second;});
                    if(clip==sourceVisual.clips.end()){e="Selected class source locomotion clip is absent";return false;}
                    const auto sourcePath=clip->second;sourceVisual.clips.emplace_back(binding.first,sourcePath);
                }
                f::CharacterVisual proof;f::EquipmentAttachmentSet weaponProof;
                if(!proof.load(assets,sourceVisual,e)||!proof.select("idle",true,e)||
                   !weaponProof.load(assets,sourceWeapons,e)||!weaponProof.update(proof,e))return false;
                options.character=std::move(sourceVisual);options.actorRow=selected->class_id;
                options.characterName=selected->name;options.freshPlayer=true;options.playClip="idle";
                options.clipRates.clear();options.save=slotPath(assignedSlot);
                options.equipment=std::move(sourceWeapons);options.combat.equippedItemIds.clear();options.combat.mainItemId.clear();options.combat.offItemId.clear();
                for(const auto& equipped:selected->equipment) {
                    const auto item=std::find_if(selected->inventory.begin(),selected->inventory.end(),[&](const auto& value){return value.instance_id==equipped.item_instance_id;});
                    if(item==selected->inventory.end()){e="Selected equipment does not resolve in the same inventory";return false;}
                    if(equipped.equipment_set>0)continue;
                    options.combat.equippedItemIds.push_back(item->definition_id);
                    if(equipped.source_slot==1)options.combat.mainItemId=item->definition_id;
                    if(equipped.source_slot==2)options.combat.offItemId=item->definition_id;
                }
                if(selected->class_id!="KnightPlayerBase") {
                    // Presentation and movement use the selected class's own
                    // source bank. Combat capability remains separate.
                    f::CombatSessionProfile animationProfile;
                    animationProfile.animationOnly=true;animationProfile.receiveDamage=true;animationProfile.retainedPhaseClock=true;
                    animationProfile.initialIdle={"Idle",0,{0}};
                    animationProfile.reaction=f::CombatSessionChoice{"Injured",0,{0}};
                    animationProfile.death=f::CombatSessionChoice{"Died",0,{0}};
                    animationProfile.reactionMinimalRandoms=false;
                    animationProfile.propertyOptions.refill_vitals=false;
                    options.combat.profiles[selected->class_id]=std::move(animationProfile);
                    std::cout<<"Selected class "<<selected->class_id<<": source movement/HUD/menu and incoming damage; outgoing attacks await their own source pipeline\n";
                }
                options.combat.playerProfileId=selected->class_id;
                options.combat.playerId=std::numeric_limits<f::ActorId>::max();
                if(options.level.empty())options.level="data/scene/001_swamp.mlx";
                options.movable=true;frontendStarted=true;e.clear();return true;
            };
            f::frontend::FrontendRuntimeServicesV1 frontendServices;
#if defined(_WIN32)
            std::unique_ptr<f::audio::FrontendMenuAudioSessionV1> menuAudio;
            const auto pumpMenuAudio=[&]() {
                if(!menuAudio)return;
                menuAudio->pump_receipts();dh2::audio::AudioReceiptV34 receipt;
                while(menuAudio->take_receipt(receipt))std::cout<<"Frontend audio device token="<<receipt.token<<" kind="<<int(receipt.kind)<<" frame="<<receipt.frame<<'\n';
            };
            const auto menuAudioActivity=[&](bool focused,bool minimized) {
                if(!options.runtimeAudio)return;
                std::string audioError;
                if(!menuAudio&&focused&&!minimized) {
                    auto audio=std::make_unique<f::audio::FrontendMenuAudioSessionV1>();
                    const auto audioRoot=options.audioAssets.empty()?assets.root():options.audioAssets;
                    if(audio->start(fs::absolute(audioRoot).generic_string(),focused,minimized,audioError))menuAudio=std::move(audio);
                    else std::cerr<<"Frontend audio initialization diagnostic: "<<audioError<<'\n';
                }
                if(menuAudio&&!menuAudio->window_activity(focused,minimized,audioError))std::cerr<<"Frontend audio activity diagnostic: "<<audioError<<'\n';
                pumpMenuAudio();
            };
            frontendServices.window_activity=menuAudioActivity;
            frontendServices.navigation.authored_menu_sound=[&](const char* menu,const char* button,const char* action) {
                if(!options.runtimeAudio)return;
                menuAudioActivity(window.focused(),window.minimized());
                if(!menuAudio){std::cout<<"Frontend audio skipped without actual window focus menu="<<menu<<" button="<<button<<'\n';return;}
                std::int64_t eventNs{};std::string audioError;f::audio::FrontendMenuAudioReceiptV1 receipt;
                if(!f::audio::frontend_menu_event_qpc_ns_v1(eventNs,audioError))std::cerr<<"Frontend audio QPC diagnostic: "<<audioError<<'\n';
                else {
                    menuAudio->play_authored_menu_action(menu,button,action,eventNs,receipt,audioError);
                    std::cout<<"Frontend audio authored menu="<<menu<<" button="<<button<<" action="<<action<<" ordinal="<<receipt.source_ordinal<<" uid="<<receipt.xml_sound_uid<<" token="<<receipt.token<<" status="<<int(receipt.status)<<" qpcNs="<<eventNs<<" diagnostic="<<audioError<<'\n';
                }
                pumpMenuAudio();
            };
#endif
            f::character_menu::MenuLocalization profileLocalization;
            if(!profileLocalization.load(menuSource,"original-cache/data",0,menuError))
                throw std::runtime_error("Frontend profile localization: "+menuError);
            const auto inspectProfileSlot=[&](int slot,f::frontend::flow::SlotFact& fact,std::string& e) {
                if(slot<0||slot>3){e="Profile slot is outside the original four-slot menu";return false;}
                std::error_code fileError;const auto path=slotPath(slot);
                const bool occupied=fs::exists(path,fileError);
                if(fileError){e="Profile slot inspection failed: "+fileError.message();return false;}
                if(occupied&&!fs::is_regular_file(path,fileError)){e="Profile slot path is not a regular save file";return false;}
                fact={slot,occupied,path};e.clear();return true;
            };
            frontendServices.navigation.inspect_slot=inspectProfileSlot;
            frontendServices.borrow_selected_profile_snapshot=[&](const f::frontend::flow::SlotFact& expected,std::string& e)->std::optional<f::frontend::FrontendSelectedProfileSnapshotV1> {
                f::frontend::flow::SlotFact actual;
                if(expected.id!=selectedMenuSlot||!inspectProfileSlot(expected.id,actual,e)) {
                    if(e.empty())e="Saved model presentation refers to a stale selected slot";
                    return std::nullopt;
                }
                if(!actual.in_use||!expected.in_use||actual.save_path!=expected.save_path) {
                    e="Saved model presentation path/occupancy differs from current slot";return std::nullopt;
                }
                auto saved=std::make_shared<f::CharacterState>();
                if(!f::load_character(actual.save_path,*saved,e))return std::nullopt;
                f::frontend::FrontendSelectedProfileSnapshotV1 snapshot{actual,std::move(saved)};
                if(!snapshot.valid_for(expected)){e="Saved model presentation has no matching immutable profile identity";return std::nullopt;}
                e.clear();return snapshot;
            };
            frontendServices.navigation.project_selected_profile=[&](const f::frontend::flow::SlotFact& fact,std::vector<f::frontend::flow::SourceText>& fields,std::string& e) {
                fields.clear();
                if(fact.id!=selectedMenuSlot||fact.save_path!=slotPath(fact.id)||!fact.in_use){e="Profile presentation refers to a stale slot/path";return false;}
                f::CharacterState saved;if(!f::load_character(fact.save_path,saved,e))return false;
                const auto& characters=sourceOwner.source().properties->characters;
                const auto row=std::find(characters.names.begin(),characters.names.end(),saved.class_id);
                if(row==characters.names.end()){e="Saved profile class is absent from source CharacterTable";return false;}
                std::string classLabel;
                if(!profileLocalization.string_id(characters.rows[std::size_t(row-characters.names.begin())][5],classLabel,e))return false;
                const auto projected=creation::saved_profile_text_bindings(saved,[&](const f::CharacterState& same,std::string& slotError){
                    creation::SavedProfilePresentation value{};
                    value.character_id=same.id;value.class_token=same.class_id;value.class_label=classLabel;
                    // P14 schema: act, location, difficulty and last-save date come from the slot's schema-v4
                    // metadata through engine-ui menu_save_slot_projection_v1 (NativeGetSaveSlotDetails 0x44aa28).
                    // Legacy v1-v3 slots (menu_metadata.known=false) keep blank rows until their next save point.
                    const auto* levelTables=loadMetadataLevels(menuSource);
                    if(!levelTables){slotError="Menu LevelList assets are unavailable";return std::optional<creation::SavedProfilePresentation>();}
                    if(!f::menu_metadata::project_slot_presentation(same,fact.id,std::int32_t(row-characters.names.begin()),characters,*levelTables,profileLocalization,0,value,slotError))
                        return std::optional<creation::SavedProfilePresentation>();
                    return std::optional<creation::SavedProfilePresentation>(std::move(value));
                });
                if(!projected.ok()){e=projected.error;return false;}
                for(const auto& field:projected.fields)fields.push_back({field.field_path,field.html_text,false});
                e.clear();return true;
            };
            frontendServices.navigation.select_slot=[&](int current,int direction,f::frontend::flow::SlotFact& fact,std::string& e) {
                if(current!=selectedMenuSlot||(direction!=1&&direction!=-1)){e="Profile arrow refers to a stale slot";return false;}
                if(!inspectProfileSlot(current+direction,fact,e))return false;
                selectedMenuSlot=fact.id;assignedSlot=-1;e.clear();return true;
            };
            frontendServices.navigation.remove_selected_slot=[&](const f::frontend::flow::SlotFact& selected,std::string& e) {
                const int slot=selected.id;
                if(slot!=selectedMenuSlot){e="Removal refers to a stale profile slot";return false;}
                f::frontend::flow::SlotFact fact;if(!inspectProfileSlot(slot,fact,e))return false;
                if(selected.save_path!=fact.save_path||!selected.in_use){e="Removal path no longer matches the confirmed profile";return false;}
                if(!fact.in_use){e="The selected profile slot is already empty";return false;}
                // A confirmed Remove clears the selected slot while retaining
                // the original bytes in a uniquely named sibling for recovery.
                const auto stem=fact.save_path.string()+".removed-"+std::to_string(std::time(nullptr));
                fs::path archived=stem;unsigned suffix=0;
                while(fs::exists(archived))archived=stem+"-"+std::to_string(++suffix);
                std::error_code fileError;fs::rename(fact.save_path,archived,fileError);
                if(fileError){e="Profile removal failed: "+fileError.message();return false;}
                assignedSlot=-1;std::cout<<"Removed selected profile slot="<<slot<<"; recoverable file="<<archived.string()<<'\n';e.clear();return true;
            };
            creation::GenericCreationFrontendHostV1 frontend(std::move(frontendServices),std::move(flowServices));
            f::frontend::FrontendRunConfigV1 menuConfig;menuConfig.asset_root=menuSource.root();
            menuConfig.ui_assets=options.menuUiAssets;menuConfig.frames=options.menuFrames;
            menuConfig.capture_directory=options.menuCaptureDirectory;menuConfig.capture_every=options.menuCaptureEvery;
            menuConfig.width=window.width();menuConfig.height=window.height();menuConfig.action_script=options.menuActions;
            menuConfig.selected_class=options.menuClassIndex;
            menuConfig.verify_generic_creation=options.verifyFrontendCreation;
            if(options.verifyFrontendSlots) {
                if(!options.save.is_absolute()||options.verifyFrontendCreation)throw std::runtime_error("Profile slot verification requires an explicit absolute sandbox save path and separate verification mode");
                const auto diagnosticRoot=fs::absolute("../../.local-inputs/v19-frontend-hotfix").lexically_normal();
                const auto candidate=fs::absolute(options.save).lexically_normal();
                const auto relative=candidate.lexically_relative(diagnosticRoot);
                if(relative.empty()||*relative.begin()=="..")throw std::runtime_error("Profile slot verification is confined to the frontend hotfix sandbox");
            }
            menuConfig.verify_profile_slots=options.verifyFrontendSlots;
            if(options.menuFrames>0)menuConfig.capture_path=options.capture;
            menuConfig.selected_slot={options.selectedSaveSlot,fs::is_regular_file(slotPath(options.selectedSaveSlot)),slotPath(options.selectedSaveSlot)};
            const auto result=frontend.complete(f::frontend::run_frontend_v1(window,renderer,menuConfig,frontend.runtime_services()));
#if defined(_WIN32)
            if(menuAudio){pumpMenuAudio();std::string audioError;if(!menuAudio->shutdown(audioError))throw std::runtime_error("Frontend audio shutdown: "+audioError);menuAudio.reset();}
#endif
            if(!result.gameplay_started_for(sharedCharacter,options.selectedSaveSlot)) {
                if(result.frontend.outcome==f::frontend::FrontendRuntimeOutcomeV1::host_failed||result.frontend.outcome==f::frontend::FrontendRuntimeOutcomeV1::source_operation_failed)
                    throw std::runtime_error("Frontend: "+result.frontend.error);
                return 0;
            }
            frontendRandomState=dh2::data::CombatRandom{creationRandom.seed,creationRandom.calls};
            options.combat.initialRandomState=frontendRandomState;options.combat.diagnosticRngSeed.reset();
            std::cout<<"Frontend launched same CharacterState slot="<<options.selectedSaveSlot<<" class="<<state.class_id<<" sourceRNG="<<creationRandom.seed<<'/'<<creationRandom.calls<<'\n';
        }
        // Preview 15 campaign loading screen (menu route only; tests skip it). Stages below report real progress.
        f::startup::LoadingScreenV1 loadingScreen(window,renderer,options.startMode=="menu"&&!options.skipBoot,1.5,&assets);
        if(loadingScreen.enabled()) {
            // Original NativeGetLoadingTipStrID: LCG over the help-page table; the seed is the game Random seed.
            std::uint32_t loadingTipSeed=std::uint32_t(GetTickCount())|1u;
            f::startup::LoadingTip tip;std::string tipError;
            if(f::startup::choose_loading_tip_v1(assets,loadingTipSeed,tip,tipError)) {
                std::cout<<"Loading tip string_id="<<tip.string_id<<" text=\""<<tip.text<<'"'<<std::endl;
                loadingScreen.set_tip(tip);
            } else std::cerr<<"Loading tip unavailable: "<<tipError<<std::endl;
        }
        if(!options.loadingCapture.empty())loadingScreen.set_capture(options.loadingCapture,[](const fs::path& p,int w,int h){capture(p,w,h);});
        loadingScreen.progress(0.0);
        // P16 MAP: module RoomZone sources of the loaded level (empty until load_level_with_module_zones succeeds).
        std::vector<f::LevelModuleZone> levelModuleZones;
        f::OriginalScene scene;f::CharacterVisual visual;f::ActorProfileLibrary profiles;f::ActorPopulation population;f::EquipmentAttachmentSet equipment;std::string error;
        f::containers::ContainerTablesV1 containerTables;f::containers::ContainerClassRegistryV1 containerRegistry;std::vector<f::containers::ContainerInstanceV1> containerInstances;f::containers::ContainerRuntimeV1 containerRuntime; // P16 containers (T2/T3)
        f::containers::ContainerLootV1 containerLoot;std::set<std::string> containerScriptsNoticed; // P16 CONTAINERS2 (T4 loot, T6 OnOpen contracts)
        f::OriginalPropertyDatabase properties;f::OriginalActorProperties actorProperties;f::Vec3 actorScale{1,1,1};
        dh2::data::PropertyRules menuSkillPropertyRules;
        bool directFirstSkillGrantPending=false;
        if(!options.actorRow.empty()) {
            if(!f::load_original_property_tables(assets,"original-cache/data/pydata",properties,error))throw std::runtime_error("Property tables: "+error);
            bool ok=options.freshPlayer?f::resolve_original_fresh_player(properties,options.actorRow,actorProperties,error):f::resolve_original_actor_properties(properties.characters,properties.classes,options.actorRow,{std::nullopt,true},actorProperties,error);
            if(!ok)throw std::runtime_error("Actor properties: "+error);
            actorScale={actorProperties.sheets.base[12]*.009f,actorProperties.sheets.base[13]*.009f,actorProperties.sheets.base[14]*.01f};
            std::cout<<"Original actor="<<options.actorRow<<" HP="<<actorProperties.health<<"/"<<actorProperties.max_health<<" MP="<<actorProperties.resource<<"/"<<actorProperties.max_resource<<" (before equipment contributors)\n";
        }
        // Preview 14: a fresh direct run with an existing --save profile loads it (test profiles with points).
        // B052: the load must precede the direct skill bank preload below; the bank binds the CharacterState
        // id/class/assignments at preload time, so a later load made every saved-profile cast fail.
        if(!frontendStarted&&options.freshPlayer&&fs::exists(options.save)) {
            if(!f::load_character(options.save,state,error)) throw std::runtime_error("Save: "+error);
            // The direct bootstrap below regenerates starter gear from the live actor; drop the file copy so slots are not duplicated.
            state.equipment.clear();state.inventory.clear();
        }
        if(!frontendStarted&&options.freshPlayer&&options.hud&&!options.meleeBindings.empty()) {
            if(options.actorRow!=options.combat.playerProfileId)throw std::runtime_error("Direct skill bank requires the same source player class");
            if(!menuSourceOwner.valid()&&!f::frontend::creation::load_runtime_creation_source_v1(assets,creationRandom,menuSourceOwner,error))
                throw std::runtime_error("Direct skill bank source: "+error);
        }
        if(!frontendStarted&&options.freshPlayer&&options.hud&&!options.meleeBindings.empty()&&!state.source_skill_slots_known) {
            state.class_id=options.actorRow;
            state.stats.level=unsigned(std::max(1,actorProperties.level_raw/256));
            if(!f::frontend::creation::initialize_source_skill_rows_v1(menuSourceOwner.skill_owner->borrow(),actorProperties.sheets.resolved[28],state,error))
                throw std::runtime_error("Direct skill bank rows: "+error);
            // Preview 14: the starter row-0 grant belongs to a fresh character only.
            // An existing profile (loaded above) keeps its points.
            directFirstSkillGrantPending=!fs::exists(options.save);
        }
        options.character.motion_node_id=options.motionNode;options.character.consume_root_motion=options.movable;
        if(!options.profiles.empty()&&!profiles.load(assets,options.profiles.generic_string(),error))throw std::runtime_error("Profiles: "+error);
        dh2::data::CharacterTemplateTableV78 populationTemplateTable;
        std::optional<dh2::data::CombatRandom> populationStartupRandom;
        const auto populationStartupSeed=frontendRandomState?std::optional<std::uint32_t>(frontendRandomState->seed):options.combat.diagnosticRngSeed;
        f::PopulationTemplateSelectionV1 templateSelection;
        if(options.populationTemplates) {
            if(!populationStartupSeed)throw std::runtime_error("Population template selection requires an explicit shared startup RNG seed");
            if(properties.characters.names.empty()&&!f::load_original_property_tables(assets,"original-cache/data/pydata",properties,error))throw std::runtime_error("Population property tables: "+error);
            const auto records=assets.read("original-cache/data/pydata/character_templates_pyarray.bin");
            const auto names=assets.read("original-cache/data/pydata/character_templates_pyarraynames.bin");
            if(!populationTemplateTable.load({records.data(),records.size()},{names.data(),names.size()},error))throw std::runtime_error("Population template table: "+error);
            templateSelection.characters=&properties.characters;templateSelection.templates=&populationTemplateTable;
            templateSelection.random_index=[&](std::int32_t bound,std::int32_t& index,std::string& e){
                if(!populationStartupRandom||bound<0){e="Population shared startup stream/range unavailable";return false;}
                index=dh2_combat_random(&*populationStartupRandom,bound);e.clear();return true;
            };
        }
        bool equipmentStateInitialized=frontendStarted;
        dh2::data::Dictionary skillAnimationDictionary;
        dh2::data::AnimationTables skillAnimationTables;
        std::optional<f::generic_skills::RuntimeSkillAnimationBankV1> skillAnimationBank;
        f::OriginalCombatVisualPlan skillVisualPlan;
        f::OriginalSequencePolicies skillSequencePolicies;
        dh2::data::FaeryTables sourceFaeryOwner;
        dh2::data::FaeryTables::Borrow sourceFaeryTables;
        f::faery_menu::HottyCooldownClockV1 faeryCooldownClock;
        std::unique_ptr<f::generic_skills::RuntimeSkillCastCoordinatorV1> skillCastCoordinator;
        std::function<bool(f::CombatSession&,double,std::string&)> sourcePhysicalFrameBegin;
        // Original GameOption GOD_MANA defaults off; Character0x14F0 is
        // constructor-zero and only an explicit debug-console toggle changes it.
        struct OrdinarySkillManaPolicy {bool savedGodMana=false,actorDebugBypass=false;} skillManaPolicy;
        int lastSkillPhase=-1;std::uint64_t lastSkillGeneration=0;
        const auto playerPolicy=options.combat.profiles.find(options.combat.playerProfileId);
        const bool sourcePlayerStanceEnabled=!options.meleeBindings.empty()&&playerPolicy!=options.combat.profiles.end()&&playerPolicy->second.retainedPhaseClock;
        dh2::data::ItemTable locomotionItems;
        std::optional<f::equipment_menu::RuntimePlayerLocomotionLibraryV1> locomotionLibrary;
        if(sourcePlayerStanceEnabled) {
            const auto read=[&](const char* file){return f::read_content(assets,(std::filesystem::path(options.combat.tableRoot)/file).generic_string());};
            const auto records=read("loot_table_pyarray.bin"),names=read("loot_table_pyarraynames.bin"),fields=read("loot_table_pystructnames.bin");
            if(!dh2::data::load_items({records.data(),records.size()},{names.data(),names.size()},{fields.data(),fields.size()},locomotionItems,error))throw std::runtime_error("Locomotion source items: "+error);
        }
        const auto currentLocomotionItems=[&]() {
            std::pair<int,int> hands{-1,-1};
            const auto id=[&](const std::string& definition){const int value=dh2::data::item_id(locomotionItems,definition);if(value<0)throw std::runtime_error("Locomotion equipped item is absent from source table: "+definition);return value;};
            if(!equipmentStateInitialized) {
                if(!options.combat.mainItemId.empty())hands.first=id(options.combat.mainItemId);
                if(!options.combat.offItemId.empty())hands.second=id(options.combat.offItemId);
            } else for(const auto& equipped:state.equipment) {
                if(equipped.equipment_set>0)continue;
                int slot=equipped.source_slot;
                if(slot<0&&equipped.slot=="slot-1")slot=1;
                if(slot<0&&equipped.slot=="slot-2")slot=2;
                if(slot!=1&&slot!=2)continue;
                const auto item=std::find_if(state.inventory.begin(),state.inventory.end(),[&](const auto& candidate){return candidate.instance_id==equipped.item_instance_id;});
                if(item==state.inventory.end())throw std::runtime_error("Locomotion equipment instance is no longer owned");
                (slot==1?hands.first:hands.second)=id(item->definition_id);
            }
            return hands;
        };
        // P16 SPAWN: pool of admitted slots and the CharacterTemplate table (both used only with --spawn-test).
        f::spawn::SpawnPoolV1 spawnPool;
        dh2::data::CharacterTemplateTableV78 spawnTemplateTable;
        // P16 PROFILES: tables for profiles derived from CharacterTable/AnimTable rows, and the derived IDs to publish
        // into the melee bindings once they load.
        f::spawn::ProfileDerivationTablesV1 derivedTables;bool derivedTablesLoaded=false;std::vector<std::string> derivedProfileIds;
        auto loadContent=[&](f::OriginalScene& scene,f::CharacterVisual& visual) {
        if(sourcePlayerStanceEnabled) {
            const auto* profile=profiles.find(options.combat.playerProfileId);
            if(!profile)throw std::runtime_error("Source stance profile is absent");
            if(options.character.model_path!=profile->model_uri) {
                const auto configured=f::read_content(assets,options.character.model_path);
                const auto authored=f::read_content(assets,profile->model_uri);
                if(configured!=authored)throw std::runtime_error("Source stance model differs from the original selected profile");
                options.character.model_path=fs::relative(f::resolve_content_path(assets,profile->model_uri),assets.root()).generic_string();
            }
            const auto hands=currentLocomotionItems();
            f::equipment_menu::RuntimePlayerLocomotionLibraryRequestV1 request;
            request.profile_id=options.combat.playerProfileId;request.profile_library_uri=options.profiles.generic_string();request.role="player-source-locomotion";
            request.main_hand_item_id=hands.first;request.off_hand_item_id=hands.second;
            const auto prefix=request.role+"/";
            options.character.clips.erase(std::remove_if(options.character.clips.begin(),options.character.clips.end(),[&](const auto& clip){return clip.first.compare(0,prefix.size(),prefix)==0;}),options.character.clips.end());
            f::equipment_menu::RuntimePlayerLocomotionLibraryV1 next;
            if(!f::equipment_menu::RuntimePlayerLocomotionLibraryV1::build(assets,options.character,request,next,error)||!next.merge_named_clips(options.character,error))throw std::runtime_error("Source stance preload: "+error);
            locomotionLibrary=std::move(next);
        }
        if(options.populationTemplates)populationStartupRandom=frontendRandomState.value_or(dh2::data::CombatRandom{*populationStartupSeed,0});
        if(!options.level.empty()) {
            if(!f::load_level_with_module_zones(assets,options.level,scene,levelModuleZones,error)) throw std::runtime_error("Level: "+error);
            loadingScreen.progress(0.4);  // level stage complete
            for(const auto& notice:scene.notices)std::cerr<<"Level notice: "<<notice<<'\n';
            std::cout<<"Level triangles="<<scene.triangleCount<<" instances="<<scene.instanceCount<<" ranges="<<scene.mesh.ranges.size()<<'\n';
        }
        if(!options.scene.empty()) {
            bool ok;
            if(options.module.empty()) ok=f::load_original_scene(assets.resolve(options.scene),scene,error);
            else ok=f::decode_original_scene_module(assets.read(options.scene),options.module,f::identity(),scene,error);
            if(!ok) throw std::runtime_error("Scene: "+error);
            std::cout<<"Scene triangles="<<scene.triangleCount<<" instances="<<scene.instanceCount<<" ranges="<<scene.mesh.ranges.size()<<" bounds="
                <<scene.minimum.x<<','<<scene.minimum.y<<','<<scene.minimum.z<<" -> "<<scene.maximum.x<<','<<scene.maximum.y<<','<<scene.maximum.z<<'\n';
        }
        if(state.source_skill_slots_known&&menuSourceOwner.valid()) {
            const std::string skillClipPrefix="player-source-skills/";
            options.character.clips.erase(std::remove_if(options.character.clips.begin(),options.character.clips.end(),[&](const auto& clip){return clip.first.compare(0,skillClipPrefix.size(),skillClipPrefix)==0;}),options.character.clips.end());
            const auto read=[&](const char* file){return f::read_content(assets,std::string("data/pydata/")+file);};
            if(state.source_faery_state_known&&!sourceFaeryTables) {
                const auto records=read("faeries_pyarray.bin"),names=read("faeries_pyarraynames.bin"),fields=read("faeries_pystructnames.bin");
                if(!sourceFaeryOwner.load({records.data(),records.size()},{names.data(),names.size()},{fields.data(),fields.size()},error))throw std::runtime_error("Source Faery tables: "+error);
                sourceFaeryTables=sourceFaeryOwner.borrow();
            }
            const auto names=read("animations_dictionary_pyarraynames.bin"),values=read("animations_dictionary_pyarray.bin");
            const auto records=read("animations_pyarray.bin"),animationNames=read("animations_pyarraynames.bin"),fields=read("animations_pystructnames.bin");
            if(!dh2::data::load_dictionary({names.data(),names.size()},{values.data(),values.size()},skillAnimationDictionary,error)||
               !dh2::data::load_animation_tables({records.data(),records.size()},{animationNames.data(),animationNames.size()},{fields.data(),fields.size()},skillAnimationDictionary,skillAnimationTables,error))throw std::runtime_error("Source skill animation tables: "+error);
            skillVisualPlan={};skillVisualPlan.profileId=options.combat.playerProfileId;skillVisualPlan.roleId="player-source-skills";skillVisualPlan.config=options.character;
            skillSequencePolicies={};
            f::generic_skills::RuntimeSkillAnimationBankRequestV1 request;
            request.character=&state;request.characters=&properties.characters;request.skills=menuSourceOwner.skill_owner->borrow();request.assets=&assets;
            request.animations=&skillAnimationTables;request.animation_dictionary=&skillAnimationDictionary;request.same_actor_visual=&options.character;
            request.actor_role=skillVisualPlan.roleId;request.preload_current_class_roots=true;
            request.source_animation_table_id=dh2_character_animation_table_id(actorProperties.sheets.resolved[2],std::int32_t(skillAnimationTables.characters.size()));
            f::generic_skills::RuntimeSkillAnimationBankV1 bank;
            if(!f::generic_skills::build_runtime_skill_animation_bank_v1(request,skillVisualPlan,bank,error)||
               !f::generic_skills::merge_runtime_skill_animation_bank_v1(bank,skillVisualPlan,skillSequencePolicies,error))throw std::runtime_error("Source skill animation preload: "+error);
            options.character=skillVisualPlan.config;skillAnimationBank=std::move(bank);
            for(const auto& root:skillAnimationBank->class_skill_roots)if(!root.loaded)std::cerr<<"Source skill optional root unavailable "<<root.source_skill_name<<": "<<root.diagnostic<<'\n';
            std::cout<<"Source skill bank preloaded class="<<state.class_id<<" roots="<<skillAnimationBank->class_skill_roots.size()<<" faeryCast="<<skillAnimationBank->active_faery_cast_sequence.value_or(-1)<<'\n';
            if(state.class_id=="RoguePlayerBase"&&locomotionLibrary) {
                const std::string attackClipPrefix="player-source-attack/";
                options.character.clips.erase(std::remove_if(options.character.clips.begin(),options.character.clips.end(),[&](const auto& clip){return clip.first.compare(0,attackClipPrefix.size(),attackClipPrefix)==0;}),options.character.clips.end());
                const auto* sourceProfile=profiles.find(options.combat.playerProfileId);
                if(!sourceProfile)throw std::runtime_error("Source melee player profile is absent");
                const auto hands=currentLocomotionItems();
                f::equipment_menu::RuntimePlayerLocomotionSelectionV1 selected;
                if(!locomotionLibrary->select_exact(hands.first,hands.second,selected,error))throw std::runtime_error("Source melee stance: "+error);
                const auto constantsBytes=f::read_content(assets,"data/animations_pycst.bin");
                const auto destroyConstants=[](dh2_script_constants* value){if(value)dh2_script_constants_destroy(value);};
                std::unique_ptr<dh2_script_constants,decltype(destroyConstants)> constants(dh2_script_constants_create(),destroyConstants);
                dh2_script_constants_reload reload{};
                if(!constants||dh2_script_constants_load(constants.get(),constantsBytes.data(),std::uint32_t(constantsBytes.size()),&reload)!=0||reload.consumed!=constantsBytes.size())throw std::runtime_error("Source melee stance constants failed");
                f::equipment_menu::RuntimePlayerLocomotionConstantsV1 stance;
                if(!f::equipment_menu::load_runtime_player_locomotion_constants_v1([&](const char* group,const char* key,std::int32_t& value,std::string& e){if(dh2_script_constants_get(constants.get(),group,key,&value)){e="Source melee constant is absent";return false;}return true;},stance,error))throw std::runtime_error(error);
                f::equipment_menu::RuntimePlayerLocomotionV1 equipped;
                if(!request.source_animation_table_id)throw std::runtime_error("Source melee animation table is absent");
                equipped.animation_table=*request.source_animation_table_id;equipped.stance=selected.stance;
                f::OriginalCombatVisualPlan base;base.profileId=options.combat.playerProfileId;base.roleId="player-source-attack";base.config=options.character;
                f::combat::RuntimePlayerProfileAttackBankPlanV1 attack;
                if(!f::combat::plan_runtime_player_profile_attack_bank_v1(assets,*sourceProfile,equipped,stance.stanced_list_mask,skillAnimationTables,skillAnimationDictionary,base,base.roleId,attack,error))throw std::runtime_error("Source player attack preload: "+error);
                options.character=attack.bank.config;
                auto& policy=options.combat.profiles.at(options.combat.playerProfileId);
                policy.animationOnly=false;policy.receiveDamage=false;
                policy.sourceAttackBank=attack.bank;policy.sourceAttackPolicies=attack.sequence_policies;
                policy.sequenceAction=attack.static_selection;policy.sourceAttackStateSelection=true;
                policy.sourceMeleeHandMarkers=true;policy.sourceCombo=true;
                // B044: the offhand damage marker exists only for a selected dual phase (off slot filled).
                policy.damageMarkerNames={"attack_mainhand"};if(hands.second>=0)policy.damageMarkerNames.push_back("attack_offhand");
                std::cout<<"Source player melee bank class="<<state.class_id<<" stance="<<selected.stance<<" moving="<<attack.moving_sequence_id<<" static="<<attack.static_sequence_id<<" main="<<hands.first<<" off="<<hands.second<<"; current initial gear, later bank replacement remains explicit\n";
            }
        }
        if(!options.character.model_path.empty() && !visual.load(assets,options.character,error)) throw std::runtime_error("Character: "+error);
        if(!equipment.load(assets,options.equipment,error))throw std::runtime_error("Equipment: "+error);
        if(!options.playClip.empty() && !visual.select(options.playClip,true,error)) throw std::runtime_error("Clip: "+error);
        if(!options.profiles.empty()) {
            auto policy=[&](const f::ActorDefinition& a) {
                auto it=a.properties.find("activate_cond");
                if(it!=a.properties.end()&&!it->second.empty()&&!options.activeConditions.count(it->second))return f::PopulationDecision::unknown;
                it=a.properties.find("deactivate_cond");
                if(it!=a.properties.end()&&!it->second.empty()) {
                    if(options.activeConditions.count(it->second))return f::PopulationDecision::exclude;
                    if(!options.inactiveConditions.count(it->second))return f::PopulationDecision::unknown;
                }
                it=a.properties.find("auto_spawn");if(it!=a.properties.end()&&it->second=="0")return options.retainHiddenActors?f::PopulationDecision::deferred:f::PopulationDecision::unknown;
                it=a.properties.find("spawn_prob");if(it!=a.properties.end()&&it->second!="100"&&it->second!="100.0")return f::PopulationDecision::unknown;
                return f::PopulationDecision::include;
            };
            auto customization=[&](const f::ActorDefinition&,const f::ActorProfile&) {
                f::ActorCustomization c;c.allow_missing_animation_targets=true;
                c.use_authored_modular_defaults=true;
                return c;
            };
            if(!population.load(assets,options.level.generic_string(),profiles,policy,customization,error,options.populationTemplates?&templateSelection:nullptr))throw std::runtime_error("Population: "+error);
            f::levels::log_unsupported_classes_once(population.definitions()); // P16 LEVELS: unknown classes logged once per process
            if(populationStartupRandom)std::cout<<"Population source template RNG seed="<<populationStartupRandom->seed<<" calls="<<populationStartupRandom->calls<<'\n';
            for(auto& actor:population.actors())if(!actor.visual.select("Idle",true,error))std::cerr<<"Population initial pose: "<<actor.definition.name<<": "<<error<<'\n';
            std::cout<<"Population visuals="<<population.actors().size()<<" declarations="<<population.authored_count()<<" skipped="<<population.skipped_count()<<'\n';
            for(const auto& notice:population.notices())std::cerr<<"Population notice: "<<notice.sourceId<<": "<<notice.reason<<'\n';
            // P16 containers: general loader over the authored declarations of the loaded level (class registry; no level data here).
            {std::string containerError;f::containers::ContainerLoadReportV1 containerReport;std::vector<std::string> containerNotices;std::vector<f::containers::ContainerInstanceV1> containerInstances;
             if(!containerTables.ready()&&!containerTables.load(assets,containerError))std::cout<<"Containers unavailable: "<<containerError<<'\n';
             else if(!f::containers::load_container_instances_v1(containerTables,containerRegistry,population.definitions(),containerInstances,containerReport,containerError))std::cout<<"Containers unavailable: "<<containerError<<'\n';
             else if(![&]{ // P16 CONTAINERS2 debug: --container-script overrides the authored OnOpen script of named declarations (logged)
                 for(const auto& [declName,script]:options.containerScriptOverrides){
                     bool found=false;for(auto& instance:containerInstances)if(instance.name==declName){instance.script=script;found=true;}
                     std::cout<<"Container script override declaration="<<declName<<" script="<<script<<(found?" status=applied":" status=unknown_declaration")<<'\n';
                 }
                 return containerRuntime.adopt(assets,std::move(containerInstances),containerNotices,containerError);}())std::cout<<"Containers unavailable: "<<containerError<<'\n';
             else {
                 std::size_t visible=0;for(const auto& view:containerRuntime.views())if(view.visual)++visible;
                 std::string unsupported;for(const auto& u:containerReport.unsupported){if(!unsupported.empty())unsupported+=",";unsupported+=u.first+":"+std::to_string(u.second);}
                 std::cout<<"Containers instantiated="<<containerReport.instantiated<<" declarations="<<containerReport.declarations<<" visuals="<<visible<<" unsupported="<<(unsupported.empty()?std::string("none"):unsupported)<<'\n';
                 for(const auto& view:containerRuntime.views())if(view.instance)std::cout<<"Container declaration="<<view.instance->name<<" interaction="<<view.instance->interaction_type<<" at="<<view.instance->transform[12]<<','<<view.instance->transform[13]<<','<<view.instance->transform[14]<<" script="<<(view.instance->script.empty()?std::string("(none)"):view.instance->script)<<'\n';
                 for(const auto& n:containerReport.notices)std::cerr<<"Container notice: "<<n<<'\n';
                 for(const auto& n:containerNotices)std::cerr<<"Container notice: "<<n<<'\n';
             }}

            // P16 SPAWN (--spawn-test only): slots for every candidate profile that has an actor profile AND an
            // explicit combat policy are admitted into THIS population before the CombatSession is built. Other
            // candidates are not admitted; the spawn owner then rejects them with an explicit reason.
            // P16 CONTAINERS2 (T6): Summon targets of this level's OnOpen contracts reserve pool slots like --spawn-test names.
            std::vector<std::string> spawnNames;
            for(const auto& test:options.spawnTests)spawnNames.push_back(test.name);
            for(const auto& view:containerRuntime.views())if(view.instance)if(const auto* contract=f::containers::find_open_script_contract_v1(view.instance->script))for(const auto& character:f::containers::open_contract_summon_characters_v1(*contract))if(std::find(spawnNames.begin(),spawnNames.end(),character)==spawnNames.end())spawnNames.push_back(character);
            if(!spawnNames.empty()) {
                if(properties.characters.names.empty()&&!f::load_original_property_tables(assets,"original-cache/data/pydata",properties,error))throw std::runtime_error("Spawn property tables: "+error);
                const auto templateRecords=assets.read("original-cache/data/pydata/character_templates_pyarray.bin");
                const auto templateNames=assets.read("original-cache/data/pydata/character_templates_pyarraynames.bin");
                if(!spawnTemplateTable.load({templateRecords.data(),templateRecords.size()},{templateNames.data(),templateNames.size()},error))throw std::runtime_error("Spawn template table: "+error);
                f::ActorCustomization spawnCustomization;spawnCustomization.allow_missing_animation_targets=true;spawnCustomization.use_authored_modular_defaults=true;
                const auto hostLevelRaw=frontendStarted?std::int32_t(state.stats.level*256):actorProperties.level_raw;
                const std::uint32_t spawnSlotsPerProfile=2;
                std::set<std::string> reservedProfiles;
                for(const auto& testName:spawnNames) {
                    std::vector<std::string> candidates;
                    if(!f::spawn::spawn_candidate_profiles_v1(testName,properties.characters,spawnTemplateTable,candidates,error))throw std::runtime_error("Spawn test: "+error);
                    for(const auto& profileId:candidates) {
                        // P16 PROFILES: a CharacterTable row with no authored profile/policy is derived from the original
                        // tables (features/spawn/actor_profile_derivation_v1). Authored entries always win; derived melee
                        // bindings are published after the melee XML loads (initializeCombat).
                        if(!profiles.find(profileId)||options.combat.profiles.find(profileId)==options.combat.profiles.end()) {
                            if(!derivedTablesLoaded) {
                                if(!f::spawn::load_profile_derivation_tables_v1(assets,"original-cache/data/pydata",derivedTables,error))throw std::runtime_error("Profile derivation tables: "+error);
                                derivedTablesLoaded=true;
                            }
                            if(!profiles.find(profileId)) {
                                f::ActorProfile derived;
                                if(f::spawn::derive_actor_profile_v1(derivedTables,profileId,derived,error)) {
                                    if(!profiles.add_derived(derived,error))throw std::runtime_error("Derived profile: "+error);
                                    derivedProfileIds.push_back(profileId);
                                    std::cout<<"SPAWN profile derived from tables: "<<profileId<<" animationTable="<<derived.animation_table<<" states="<<derived.states.size()<<'\n';
                                } else std::cout<<"SPAWN profile not derivable: "<<profileId<<" ("<<error<<")\n";
                            }
                            if(profiles.find(profileId)&&options.combat.profiles.find(profileId)==options.combat.profiles.end()) {
                                f::CombatSessionProfile derivedPolicy;
                                if(!f::spawn::derive_enemy_combat_policy_v1(derivedTables,profileId,derivedPolicy,error))throw std::runtime_error("Derived combat policy: "+error);
                                derivedPolicy.diagnosticAIEnabled=options.diagnosticAI&&profileId!=options.combat.playerProfileId;
                                options.combat.profiles.emplace(profileId,std::move(derivedPolicy));
                            }
                        }
                        const auto* profile=profiles.find(profileId);
                        const auto policy=options.combat.profiles.find(profileId);
                        if(!profile||policy==options.combat.profiles.end()||!reservedProfiles.insert(profileId).second) {
                            if(!profile||policy==options.combat.profiles.end())std::cout<<"SPAWN profile not admitted (no actor profile or combat policy): "<<profileId<<'\n';
                            continue;
                        }
                        std::int32_t level=-1;
                        if(!f::spawn::spawn_level_raw_v1(properties.characters,profileId,hostLevelRaw,level,error))throw std::runtime_error("Spawn level: "+error);
                        if(level>=0)policy->second.propertyOptions.level_raw=level;
                        if(!spawnPool.reserve(profileId,spawnSlotsPerProfile,level,error))throw std::runtime_error("Spawn pool: "+error);
                        const auto declared=spawnPool.declarations(options.level.generic_string());
                        for(std::size_t i=declared.size()-spawnSlotsPerProfile;i<declared.size();++i)
                            if(!population.admit_declared(assets,declared[i],*profile,f::PopulationDecision::deferred,spawnCustomization,error))throw std::runtime_error("Spawn admission: "+error);
                        std::cout<<"SPAWN pool profile="<<profileId<<" slots="<<spawnSlotsPerProfile<<" level_raw="<<level<<'\n';
                    }
                }
            }
        }
        };
        loadContent(scene,visual);
        f::OriginalMeleeBindings meleeBindings;std::shared_ptr<f::CombatSession> combatSession;f::ObjectOfInterestOwnerV1 objectOfInterest; // B004/B029
        f::InteractableRegistryV1 interactables; // P16 CONTEXT: non-actor interaction-type providers (empty until containers/NPC register)
        f::loot::WorldItemContactTrackerV1 worldItemContacts; // P16 CONTEXT: walk-over pickup state
        int lastActionIcon=-2; // P16 CONTEXT: last published HUD action-button frame (log on change)
        const auto bindSourcePlayerLocomotion=[&](f::CombatSession& session) {
            if(!locomotionLibrary)return;
            const auto hands=currentLocomotionItems();
            f::equipment_menu::RuntimePlayerLocomotionSelectionV1 selected;
            if(!locomotionLibrary->select_exact(hands.first,hands.second,selected,error))throw std::runtime_error("Source stance selection: "+error);
            const auto* props=session.world()->combat_properties(session.player_id());
            const double rate=f::original_speed_modifier(props->sheets.resolved[46]);
            for(const auto& named:std::vector<std::pair<std::string,std::string>>{{"idle","Idle"},{"walk","Walk"},{"run","Run"}}) {
                const auto* sequence=selected.program->plan.sequence(named.second,0);
                if(!sequence||sequence->phases.empty())throw std::runtime_error("Source locomotion state has no reachable phase");
                if(!session.bind_actor_locomotion_from_bank(session.player_id(),named.first,selected.program->plan,{named.second,0,sequence->phases.front().sourcePath},named.first=="idle"?1.0:rate,sequence->loop!=0,error))throw std::runtime_error("Source locomotion bank binding: "+error);
            }
            std::cout<<"Source player stance="<<selected.stance<<" main="<<hands.first<<" off="<<hands.second<<" preloadedPrograms="<<locomotionLibrary->program_count()<<'\n';
        };
        auto initializeCombat=[&]() {
            if(options.meleeBindings.empty())return;
            skillCastCoordinator=std::make_unique<f::generic_skills::RuntimeSkillCastCoordinatorV1>();
            lastSkillPhase=-1;lastSkillGeneration=0;
            if(equipmentStateInitialized) {
                options.combat.equippedItemIds.clear();options.combat.mainItemId.clear();options.combat.offItemId.clear();
                for(const auto& equipped:state.equipment) {
                    if(equipped.equipment_set>0)continue;
                    const auto item=std::find_if(state.inventory.begin(),state.inventory.end(),[&](const auto& candidate){return candidate.instance_id==equipped.item_instance_id;});
                    if(item==state.inventory.end())throw std::runtime_error("Selected equipment instance is not owned");
                    options.combat.equippedItemIds.push_back(item->definition_id);
                    if(equipped.source_slot==1||equipped.slot=="slot-1")options.combat.mainItemId=item->definition_id;
                    if(equipped.source_slot==2||equipped.slot=="slot-2")options.combat.offItemId=item->definition_id;
                }
            }
            if(!meleeBindings.load(assets,options.meleeBindings,error))throw std::runtime_error("Melee bindings: "+error);
            // P16 PROFILES: melee bindings for profiles derived from CharacterTable/AnimTable (no authored melee entry).
            for(const auto& derivedId:derivedProfileIds) {
                if(meleeBindings.find_actor(derivedId))continue;
                f::OriginalMeleeActor derivedActor;
                if(!f::spawn::derive_melee_actor_v1(derivedTables,derivedId,derivedActor,error))throw std::runtime_error("Derived melee: "+error);
                if(!meleeBindings.add_derived_actor(std::move(derivedActor),error))throw std::runtime_error("Derived melee: "+error);
                std::cout<<"SPAWN melee bindings derived from tables: "<<derivedId<<'\n';
            }
            options.combat.playerVisualConfig=options.character;
            options.combat.selectedPlayerProfile=frontendStarted?&state:nullptr;
            if(populationStartupRandom){options.combat.initialRandomState=*populationStartupRandom;options.combat.diagnosticRngSeed.reset();}
            for(auto& profile:options.combat.profiles)if(profile.first!=options.combat.playerProfileId)profile.second.customization.use_authored_modular_defaults=true;
            options.combat.profiles[options.combat.playerProfileId].propertyOptions.level_raw=frontendStarted?std::int32_t(state.stats.level*256):actorProperties.level_raw;
            f::ActorCustomization customization;customization.skin_id_contains=options.character.skin_id_contains;customization.allow_missing_animation_targets=options.character.allow_missing_animation_targets;
            customization.controller_ids=options.character.controller_ids;
            customization.expected_controller_count=options.character.expected_controller_count;
            customization.use_authored_modular_defaults=options.character.use_authored_modular_defaults;
            customization.include_static_instances=options.character.include_static_instances;
            auto next=std::make_shared<f::CombatSession>();
            if(!next->initialize(assets,properties,meleeBindings,options.combat,visual,population,options.actorPosition,customization,error))throw std::runtime_error("Combat initialization: "+error);
            if(frontendStarted) {
                auto* world=next->world();const auto id=next->player_id();
                const auto* projected=world->combat_properties(id);
                if(!projected)throw std::runtime_error("Selected profile source properties are unavailable");
                auto* controlled=next->actor(id);
                state.stats.health=controlled->health;state.stats.max_health=controlled->max_health;
                state.stats.resource=controlled->resource;state.stats.max_resource=controlled->max_resource;
                std::cout<<"Selected profile source projection HP="<<controlled->health<<'/'<<controlled->max_health<<" MP="<<controlled->resource<<'/'<<controlled->max_resource<<" XPraw="<<projected->sheets.resolved[33]<<" skillPointsRaw="<<projected->sheets.resolved[157]<<" prebind=1\n";
            }
            if(equipmentStateInitialized) {
                auto* controlled=next->actor(next->player_id());controlled->equipment.clear();
                std::cout<<"Selected equipment initialization main="<<options.combat.mainItemId<<" count="<<state.equipment.size()<<'\n';
                for(const auto& equipped:state.equipment) {
                    if(equipped.equipment_set>0)continue;
                    const auto item=std::find_if(state.inventory.begin(),state.inventory.end(),[&](const auto& candidate){return candidate.instance_id==equipped.item_instance_id;});
                    controlled->equipment.push_back({equipped.slot,item->definition_id,item->instance_id});
                }
            }
            if(locomotionLibrary)bindSourcePlayerLocomotion(*next);
            else for(const auto& binding:options.locomotionChoices) {
                const double rate=binding.first=="idle"?1.0:actorProperties.walk_multiplier;
                if(!next->bind_player_locomotion(binding.first,binding.second,rate,true,error))throw std::runtime_error("Source locomotion binding: "+error);
            }
            combatSession=std::move(next);
            // P16 CONTAINERS2 (T5): authored containers become neutral objects of this session, so GameSave persists their OBJS state.
            {std::vector<std::string> containerObjectNotices;std::string containerObjectError;
             if(!f::containers::bind_container_world_objects_v1(*combatSession->world(),containerRuntime,containerObjectNotices,containerObjectError))throw std::runtime_error("Container objects: "+containerObjectError);
             for(const auto& n:containerObjectNotices)std::cerr<<"Container notice: "<<n<<'\n';
             std::cout<<"Containers world objects declarations="<<containerRuntime.size()<<" notices="<<containerObjectNotices.size()<<'\n';}
            faeryCooldownClock={};faeryCooldownClock.binding_lease=combatSession->actor_binding_lease();faeryCooldownClock.has_binding_lease=true;
            combatSession->set_frame_begin_provider([&](auto& session,double delta,std::string& e) {
                if(sourceFaeryTables&&!f::faery_menu::advance_hotty_cooldown_clock_v1(session,delta,faeryCooldownClock,e))return false;
                if(skillCastCoordinator&&!skillCastCoordinator->advance_after_session_update(session,delta,e))return false;
                return !sourcePhysicalFrameBegin||sourcePhysicalFrameBegin(session,delta,e);
            });
            for(const auto& line:combatSession->logs())std::cout<<"Combat binding: "<<line<<'\n';
        };
        initializeCombat();
        f::OriginalScene targetMarker;f::OriginalActorLabels actorLabels;f::HudGlyphFont targetFont;
        if(combatSession&&options.hud) {
            const auto& markers=f::original_target_marker_art();const auto marker=std::find_if(markers.begin(),markers.end(),[](const auto& art){return art.interaction_type==8;});
            if(marker==markers.end()||!f::decode_original_scene(f::read_content(assets,marker->model_uri),targetMarker,error))throw std::runtime_error("Original attack marker: "+error);
            for(auto& range:targetMarker.mesh.ranges)range.material.lightingEnabled=!marker->self_illum;
            if(!targetFont.load(assets.resolve("data/Fontin SmallCaps.ttf"),error)||!actorLabels.load(assets,"original-cache/data",0,error))throw std::runtime_error("Original target text: "+error);
        }
        auto visualStep=[&](double seconds) {
            const auto active=std::find_if(options.character.clips.begin(),options.character.clips.end(),[&](const auto& clip){return clip.second==visual.animation_name();});
            auto rate=options.clipRates.find(active==options.character.clips.end()?"":active->first);
            const bool locomotion=active!=options.character.clips.end()&&(active->first=="walk"||active->first=="run");
            return visual.update(seconds*(rate==options.clipRates.end()?1.0:rate->second)*(locomotion?actorProperties.walk_multiplier:1.0),error);
        };
        const bool runBound=combatSession&&combatSession->uses_retained_player_locomotion()?combatSession->has_player_locomotion("run"):std::any_of(options.character.clips.begin(),options.character.clips.end(),[](const auto& clip){return clip.first=="run";});
        if(options.movable&&!runBound)std::cout<<"Run input disabled: no authored run clip bound\n";
        loadingScreen.progress(0.8);  // actor population stage complete; collision next
        f::CollisionScene collision;std::unique_ptr<f::ActorMovement> motor;
        std::map<f::ActorId,std::unique_ptr<f::ActorMovement>> populationMotors;
        struct MotionReceipt {unsigned enabled=0,disabled=0;double sourceXY=0,worldXY=0;};
        std::map<f::ActorId,MotionReceipt> motionReceipts;
        if(options.movable) {
            if(!f::load_collision_level(assets,options.level,collision,error))throw std::runtime_error("Collision: "+error);
            f::ActorMovementConfig config;config.turnSpeedRadians=actorProperties.turn_radians_per_second;config.forwardAxis={0,-1,0};
            if(!visual.update(0,error))throw std::runtime_error("Initial movement pose: "+error);
            f::Vec3 lo{},hi{};
            if(!visual.indexed_bounds(lo,hi,error))throw std::runtime_error("Movement bounds: "+error);
            // Preview body approximation; original physical owners use skin joint bounds.
            config.bodyRadius=.5f*std::max((hi.x-lo.x)*actorScale.x,(hi.y-lo.y)*actorScale.y)*actorProperties.collision_scale;
            config.bodyHeight=(hi.z-lo.z)*actorScale.z;
            std::string provenance;if(!f::load_controller_policy(assets,options.controllerPolicy,config,provenance,error))throw std::runtime_error("Controller policy: "+error);
            std::cout<<"Ground controller policy="<<provenance<<" radius="<<config.bodyRadius<<" height="<<config.bodyHeight<<"; speed from authored root motion\n";
            motor=std::make_unique<f::ActorMovement>(config);motor->setPosition(options.actorPosition);
        }
        std::optional<f::OriginalActorBoundsResult> playerSourceBox;
        std::map<f::ActorId,f::OriginalActorBodyPlan> sourceBodyPlans;
        std::map<f::ActorId,std::uint8_t> sourceBodyStatic;
        std::map<f::ActorId,std::string> sourceBodyNames;
        auto prepareBodyPlans=[&]() {
            if(!options.sourceBodyBounds)return;
            if(!combatSession)throw std::runtime_error("Source body plans require the shared actor registry");
            sourceBodyPlans.clear();sourceBodyStatic.clear();sourceBodyNames.clear();
            for(const auto& entry:combatSession->world()->actors()) {
                auto* actor=combatSession->actor(entry.first);
                const auto* properties=combatSession->world()->combat_properties(entry.first);
                f::OriginalActorBodyPlanInput input;input.owner_identity=actor->id;
                input.properties=&properties->sheets;input.ai=combatSession->original_ai_tables();
                input.position={actor->transform.position[0],actor->transform.position[1],actor->transform.position[2]};
                input.rotation_degrees={actor->transform.rotation[0]*180.f/3.14159265358979323846f,actor->transform.rotation[1]*180.f/3.14159265358979323846f,actor->transform.rotation[2]*180.f/3.14159265358979323846f};
                const f::CharacterVisualConfig* configured=nullptr;
                if(actor->id==combatSession->player_id())configured=visual.configuration();
                else {
                    const auto placed=std::find_if(population.actors().begin(),population.actors().end(),[&](const auto& a){return a.definition.stableId==actor->id;});
                    if(placed==population.actors().end())throw std::runtime_error("Body plan lacks its actual loaded visual");
                    configured=placed->visual.configuration();input.source_name=placed->definition.name;
                    const auto stat=placed->definition.properties.find("static");
                    if(stat!=placed->definition.properties.end()){if(stat->second!="0"&&stat->second!="1")throw std::runtime_error("Invalid original actor static byte");input.static84=stat->second=="1";}
                }
                if(!configured)throw std::runtime_error("Body plan needs actual loaded component selection");input.visual=*configured;
                f::OriginalActorBodyPlan plan;
                if(!f::make_original_actor_body_plan(assets,input,plan,error))throw std::runtime_error("Source body plan: "+error);
                if(actor->id==combatSession->player_id()&&plan.character_type==0)throw std::runtime_error("Player type-zero body requires its actual source name provider");
                sourceBodyStatic[actor->id]=input.static84;sourceBodyNames[actor->id]=input.source_name;
                sourceBodyPlans.emplace(actor->id,std::move(plan));
            }
            const auto& player=sourceBodyPlans.at(combatSession->player_id());playerSourceBox=player.bounds;
            if(motor){if(!player.circular||!motor->set_body_radius(player.radius_game_units,error))throw std::runtime_error("Source player movement radius: "+error);}
            std::cout<<"Source movement body radius="<<player.radius_game_units<<" from original joint bounds/body definition; "<<(options.sourceNativeBodies?"native binding follows, floor solver remains pending":"native body publication and floor solver remain pending")<<'\n';
            const auto& box=player.bounds;
            std::cout<<"Source body rest components="<<player.selected_controller_count<<" relative="<<box.relative_box[0]<<','<<box.relative_box[1]<<','<<box.relative_box[2]<<','<<box.relative_box[3]<<','<<box.relative_box[4]<<','<<box.relative_box[5]<<"; actual source AABB provider, movement collision solver remains approximate\n";
            for(const auto& plan:sourceBodyPlans)if(plan.first!=combatSession->player_id())std::cout<<"Source actor body plan actor="<<plan.first<<" radius="<<plan.second.radius_game_units<<" circular="<<plan.second.circular<<"\n";
        };
        prepareBodyPlans();
        auto nativeWorld=std::make_shared<dh2::physical::NativeWorld>();
        f::OriginalActorCollisionFilter nativeCollisionFilter;
        struct NativeBodyContext {
            std::array<float,3> destination{};std::uintptr_t attached=0,visual=0;
            bool idleSuppressed=false;
            std::uint32_t gate528=0;
            std::optional<std::uint32_t> attackTimerMs,skillPinTimerMs;
        };
        std::map<f::ActorId,std::shared_ptr<NativeBodyContext>> nativeBodyContexts;
        struct SourceContactOwners {
            std::shared_ptr<f::enemy_ai::RuntimeEnemyControllerV1> controller;
            std::shared_ptr<f::enemy_ai::RuntimeEnemyContactBindingV1> binding;
            std::shared_ptr<f::physics::RuntimeSessionContactOwnerV1> owner;
            std::shared_ptr<f::physics::SessionActorTransitionConsumerV1> transitions;
        };
        auto sourceContacts=std::make_shared<SourceContactOwners>();
        bool sourcePhysicalFrameEnabled=false;
        double sourcePhysicalFractionMs=0;
        f::PlayableActorBodies nativeBodies;
        const std::map<std::string,bool> physicsDebug{{"MP_NoCollisions",false},{"MP_NoPhysics",false}};
        auto clearNativeBodies=[&]() {if(!nativeBodies.clear(error))throw std::runtime_error(error);nativeBodyContexts.clear();sourceContacts->owner.reset();sourceContacts->binding.reset();sourceContacts->controller.reset();sourceContacts->transitions.reset();sourcePhysicalFrameEnabled=false;sourcePhysicalFractionMs=0;};
        auto bindNativeActor=[&](f::ActorId id,std::string& e) {
            if(!options.sourceNativeBodies)return true;
            auto* actor=combatSession->actor(id);const auto* props=combatSession->world()->combat_properties(id);
            if(!actor||!props){e="Native body requires its current shared actor/properties";return false;}
            std::cout<<"Motion provenance stage=pre-body actor="<<id<<" position="<<actor->transform.position[0]<<','<<actor->transform.position[1]<<','<<actor->transform.position[2]<<std::endl;
            const auto savedPhysicalPresence=actor->source_physical_present;
            auto plan=sourceBodyPlans.at(id);
            if(!f::PlayableActorBodies::rebase_plan(plan,id,actor->transform.position,e))return false;
            sourceBodyPlans[id]=plan;
            auto context=std::make_shared<NativeBodyContext>();nativeBodyContexts[id]=context;
            f::OriginalActorPhysicalBindings bindings;bindings.actor_lease=combatSession;bindings.data_lease=combatSession;bindings.world_lease=nativeWorld;
            bindings.world=nativeWorld.get();bindings.ai=combatSession->original_ai_tables();
            bindings.destination1a8=context->destination.data();bindings.attached2e0=&context->attached;bindings.visual2d8=&context->visual;
            bindings.static84=[&,id,context](std::uint8_t& v,std::string&){v=sourceBodyStatic.at(id);return true;};
            bindings.is_player=[&,id](std::int32_t type,bool& v,std::string&){v=f::original_actor_source_is_player(type,sourceBodyNames.at(id));return true;};
            bindings.debug_switch=[&](const char* key,bool& v,std::string& problem){auto found=physicsDebug.find(key);if(found==physicsDebug.end()){problem="Unknown runtime physics debug key";return false;}v=found->second;return true;};
            // This NativeWorld contains only these typed character body receivers.
            bindings.filter=[&,id](void* peer,const auto& own,const auto& other,bool& allowed,std::string& problem){
                f::ActorId peerId=f::invalid_actor_id;
                if(!nativeBodies.resolve_physical_actor(peer,peerId,problem))return false;
                return nativeCollisionFilter.test_one(id,peerId,own,other,allowed,problem);
            };
            bindings.contact=[&,id,contacts=sourceContacts](auto event,void* peer,unsigned side,std::string& problem){
                if(!contacts->owner){problem="Native contact continuation requires its current same-Session contact owner";return false;}
                f::ActorId peerId=f::invalid_actor_id;
                if(!nativeBodies.resolve_physical_actor(peer,peerId,problem))return false;
                return contacts->owner->physical_event(id,static_cast<f::physics::RuntimeSessionContactEventV1>(event),peerId,side,problem);
            };
            if(!nativeBodies.bind(*actor,*props,plan,std::move(bindings),e))return false;
            if(savedPhysicalPresence&&!*savedPhysicalPresence&&nativeBodies.physical(id)->native().body)
                if(!nativeBodies.remove_physical(id,e))return false;
            const bool actualPhysicalPresence=nativeBodies.physical(id)->native().body!=nullptr;
            if(savedPhysicalPresence&&*savedPhysicalPresence&&!actualPhysicalPresence) {e="Saved physical receiver cannot be assigned from the current source body plan";return false;}
            actor->source_physical_present=actualPhysicalPresence;
            std::cout<<"Source physical presence actor="<<id<<" known=1 present="<<actualPhysicalPresence<<" restored="<<savedPhysicalPresence.has_value()<<std::endl;
            std::cout<<"Motion provenance stage=post-body actor="<<id<<" position="<<actor->transform.position[0]<<','<<actor->transform.position[1]<<','<<actor->transform.position[2]<<" pf="<<nativeBodies.navigation(id)->motion.position[0]<<','<<nativeBodies.navigation(id)->motion.position[1]<<','<<nativeBodies.navigation(id)->motion.position[2]<<std::endl;
            std::cout<<"Native body bound actor="<<id<<" radius="<<nativeBodies.physical(id)->native().radius*100.f<<" pfUser="<<nativeBodies.navigation(id)->user<<"; real body/position backing before InitFinal, world Step remains unbound\n";
            return true;
        };
        auto rebuildNativeBodies=[&]() {
            if(!options.sourceNativeBodies)return;
            clearNativeBodies();
            if(!options.sourceBodyBounds||!combatSession)throw std::runtime_error("Native bodies require shared source body plans");
            float domain[4]{scene.minimum.x,scene.minimum.y,scene.maximum.x,scene.maximum.y};
            for(const auto& entry:sourceBodyPlans){const auto* a=combatSession->actor(entry.first);const auto& b=entry.second.bounds.relative_box;const float r=std::max(b[3]-b[0],b[4]-b[1])*.5f;domain[0]=std::min(domain[0],a->transform.position[0]-r);domain[1]=std::min(domain[1],a->transform.position[1]-r);domain[2]=std::max(domain[2],a->transform.position[0]+r);domain[3]=std::max(domain[3],a->transform.position[1]+r);}
            // In-house broadphase envelope from loaded content, not a source PF solver.
            domain[0]=(domain[0]-1000)*.01f;domain[1]=(domain[1]-1000)*.01f;domain[2]=(domain[2]+1000)*.01f;domain[3]=(domain[3]+1000)*.01f;nativeWorld->load(domain);
            nativeCollisionFilter=f::OriginalActorCollisionFilter{};
            f::OriginalCollisionOwnerServices owners;
            owners.character_handle=[&](f::ActorId id,bool& found,std::string&){found=combatSession->actor(id)!=nullptr;return true;};
            owners.original_state=[&](f::ActorId id,std::int32_t& state,std::string& problem){state=combatSession->original_actor_state(id);if(state<0){problem="Original collision state unavailable";return false;}return true;};
            owners.enabled80=[&](f::ActorId id,std::uint8_t& value,std::string& problem){if(id==combatSession->player_id()){value=1;return true;}for(const auto& a:population.actors())if(a.definition.stableId==id){value=a.enabled;return true;}problem="Collision enabled owner unavailable";return false;};
            for(const auto& entry:sourceBodyPlans)if(!nativeCollisionFilter.bind(entry.first,entry.second.circular?f::OriginalCollisionReceiverKind::po_character:f::OriginalCollisionReceiverKind::physical_object,owners,error))throw std::runtime_error(error);
            for(const auto& entry:sourceBodyPlans)if(!bindNativeActor(entry.first,error))throw std::runtime_error("Native body initialization: "+error);
            f::CombatSessionAnimationNotificationServices notifications;
            const auto binding=combatSession->actor_binding_lease();
            const auto lifetime=combatSession->lifetime_lease();
            notifications.state_event=[&,binding,lifetime](f::ActorId id,std::uint32_t event,const f::RetainedAnimationEvent*,std::string& e) {
                if(event!=0x22)return true;
                const auto live=lifetime.lock();
                if(!live||!live->alive()||!combatSession||binding.expired()) {e="Death End34 belongs to an expired Session";return false;}
                const auto current=combatSession->actor_binding_lease();
                if(current.expired()||binding.owner_before(current)||current.owner_before(binding)) {e="Death End34 belongs to a stale actor binding";return false;}
                if(combatSession->original_actor_state(id)!=12)return true;
                const auto* physical=nativeBodies.physical(id);
                if(!physical)return true;
                const bool before=physical->native().body!=nullptr;
                if(before&&!nativeBodies.remove_physical(id,e))return false;
                combatSession->actor(id)->source_physical_present=physical->native().body!=nullptr;
                std::cout<<"Source death End34 actor="<<id<<" serial="<<combatSession->update_serial()<<" physicalBefore="<<before<<" physicalAfter="<<(physical->native().body!=nullptr)<<" retained="<<(combatSession->retained_actor_pose(id)!=nullptr)<<std::endl;
                e.clear();return true;
            };
            const auto checkpoint=[&,binding,lifetime](std::string& e) {
                const auto live=lifetime.lock();
                if(!live||!live->alive()||!combatSession||binding.expired()) {e="Body checkpoint belongs to an expired Session";return false;}
                const auto current=combatSession->actor_binding_lease();
                if(current.expired()||binding.owner_before(current)||current.owner_before(binding)) {e="Body checkpoint belongs to a stale actor binding";return false;}
                for(const auto& entry:sourceBodyPlans) {
                    const auto* actor=combatSession->actor(entry.first);
                    const auto* physical=nativeBodies.physical(entry.first);
                    if(!actor||!physical||!actor->source_physical_present||
                       *actor->source_physical_present!=(physical->native().body!=nullptr)) {
                        e="Body checkpoint lacks matching current source physical-presence facts";return false;
                    }
                    if(sourcePhysicalFrameEnabled) {
                        const auto context=nativeBodyContexts.find(entry.first);
                        if(context==nativeBodyContexts.end()||context->second->attackTimerMs||context->second->skillPinTimerMs) {
                            e="Body checkpoint requires quiescent source physical timers actor="+std::to_string(entry.first)+
                              " attackRemainingMs="+std::to_string(context==nativeBodyContexts.end()?0:context->second->attackTimerMs.value_or(0))+
                              " skillPinRemainingMs="+std::to_string(context==nativeBodyContexts.end()?0:context->second->skillPinTimerMs.value_or(0));return false;
                        }
                        const auto* props=combatSession->world()->combat_properties(entry.first);
                        const auto* traits=combatSession->world()->traits(entry.first);
                        std::optional<std::uint32_t> expected;
                        if(props&&traits)switch(props->facts.original_state) {
                        case 3:expected=0x2380u;break;
                        case 4:expected=0x23c1u;break;
                        case 5:expected=0x2341u;break;
                        case 6:expected=0x6341u;break;
                        case 7:expected=0x6301u;break;
                        case 11:expected=0x2b41u;break;
                        case 12:expected=0x241u|(traits->is_player?0x2000u:0u);break;
                        }
                        if(!expected||!actor->source_flags520||*actor->source_flags520!=*expected) {
                            e="Body checkpoint source flags mismatch actor="+std::to_string(entry.first)+
                              " state="+std::to_string(props?props->facts.original_state:-1)+
                              " flags="+std::to_string(actor->source_flags520.value_or(0))+
                              " expected="+std::to_string(expected.value_or(0));return false;
                        }
                    }
                }
                e.clear();return true;
            };
            if(!combatSession->bind_reconstructible_animation_notifications(std::move(notifications),checkpoint,error))
                throw std::runtime_error("Body completion checkpoint binding: "+error);
        };
        rebuildNativeBodies();
        f::OriginalGameplayCamera originalCamera;
        f::OriginalActorCameraAnchor actorCameraAnchor;
        double cameraFractionMs=0;
        auto actorCameraFrame=[&]() {
            f::OriginalActorCameraAnchorFrame frame;
            frame.position={options.actorPosition.x,options.actorPosition.y,options.actorPosition.z};
            const auto* player=combatSession?combatSession->actor(combatSession->player_id()):nullptr;
            const float yaw=player?player->transform.rotation[2]:(motor?motor->state().facingRadians:0);
            frame.lookAt=f::original_actor_camera_look_at(yaw);
            if(motor) {
                const auto& heading=motor->state().desiredHeading;
                frame.headingActive=heading.active;frame.heading={heading.direction.x,heading.direction.y,heading.direction.z};
                frame.moving=motor->state().action==f::ActorAction::Walk||motor->state().action==f::ActorAction::Run;
                frame.attacking=motor->state().action==f::ActorAction::Attack;
            }
            if(player) {frame.moving=player->action==f::CharacterAction::moving;frame.attacking=player->action==f::CharacterAction::attacking;}
            return frame;
        };
        if(options.staticActorAnchor&&!options.sourceCamera)throw std::runtime_error("Actor camera anchor requires --original-camera");
        if(options.sourceCamera) {
            f::CameraVec3 anchor{options.actorPosition.x,options.actorPosition.y,options.actorPosition.z};
            if(options.staticActorAnchor) {
                f::OriginalActorCameraAnchorConfig config;
                if(!f::load_original_actor_camera_anchor_config(assets,*options.staticActorAnchor,config,error)||!actorCameraAnchor.initialize(config,actorCameraFrame(),error))throw std::runtime_error("Actor camera anchor: "+error);
                anchor=actorCameraAnchor.position();
                std::cout<<"Original actor anchor="<<(*options.staticActorAnchor?"static":"forward")<<" maximum="<<config.maximumDistance<<" incrementPerUpdate="<<config.distancePerUpdate<<"; explicit debug-switch result, combat heading command gates remain incomplete\n";
            }
            if(!originalCamera.load(assets,options.level.generic_string(),options.cameraRoot,error)||!originalCamera.reset(anchor,error))throw std::runtime_error("Original camera: "+error);
            std::cout<<"Original camera distance="<<originalCamera.authoredDistance()<<" FOV="<<originalCamera.pose().verticalFovDegrees<<" aspect="<<originalCamera.sourceAspect()<<'\n';
        }
        // Preview 13 behaviour kept: without a combat session the save is loaded here (fresh runs loaded it above).
        if(!frontendStarted&&!combatSession&&!options.freshPlayer&&fs::exists(options.save)) {if(!f::load_character(options.save,state,error)) throw std::runtime_error("Save: "+error);}
        // P14 FAERY: legacy slots without Faery rows get creation-equivalent zero rows at first use, so Swamp_Intro can unlock Celest.
        if(f::faery_menu::ensure_source_faery_rows_v1(state)) std::cout<<"Legacy save Faery rows initialized to creation zeros\n";
        if(!options.characterName.empty())state.name=options.characterName;
        if(combatSession) {
            auto* player=combatSession->actor(combatSession->player_id());player->persistent_character_id=state.id;
            if(frontendStarted)for(auto& equipped:player->equipment) {
                const auto binding=std::find_if(state.equipment.begin(),state.equipment.end(),[&](const auto& value) {
                    if(value.equipment_set>0)return false;
                    if(equipped.item_instance_id)return value.item_instance_id==*equipped.item_instance_id;
                    const auto item=std::find_if(state.inventory.begin(),state.inventory.end(),[&](const auto& instance){return instance.instance_id==value.item_instance_id;});
                    return item!=state.inventory.end()&&item->definition_id==equipped.definition_id;
                });
                if(binding==state.equipment.end())throw std::runtime_error("Gameplay equipment lost selected profile inventory binding");
                equipped.item_instance_id=binding->item_instance_id;
            }
            if(!frontendStarted)state.class_id=player->definition_id;
            if(!frontendStarted) {
                state.stats.level=unsigned(std::max(1,actorProperties.level_raw/256));
                state.stats.health=player->health;state.stats.max_health=player->max_health;state.stats.resource=player->resource;state.stats.max_resource=player->max_resource;
                // Preview 14: a loaded direct profile owns its points and attributes.
                // Project it onto the live sheet (as the frontend start does) before the
                // bootstrap below reads points back; otherwise the fresh sheet overwrites them.
                if(state.source_points_known) {
                    const auto* loadedSheet=combatSession->world()->combat_properties(player->id);
                    const auto* loadedTraits=combatSession->world()->traits(player->id);
                    f::OriginalCombatProperties projectedSheet;
                    if(!loadedSheet||!loadedTraits||!f::project_player_profile_properties(properties,state,*loadedSheet,projectedSheet,error))
                        throw std::runtime_error("Direct profile projection: "+error);
                    if(!combatSession->world()->update_combat_properties(player->id,std::move(projectedSheet),*loadedTraits,error))
                        throw std::runtime_error("Direct profile publish: "+error);
                }
                const auto* source=combatSession->world()->combat_properties(player->id);
                if(!source||source->sheets.resolved[148]<0||source->sheets.resolved[157]<0)
                    throw std::runtime_error("Direct player bootstrap requires current source stat/skill points");
                state.source_stat_points=std::uint32_t(source->sheets.resolved[148]>>8);
                state.source_skill_points=std::uint32_t(source->sheets.resolved[157]>>8);
                state.source_points_known=true;
                std::cout<<"Direct player source points actor="<<player->id<<" stat="<<state.source_stat_points<<" skill="<<state.source_skill_points<<" known=1\n";
            }
            if(!frontendStarted) {
                // Diagnostic startup items are still exact original definitions.
                // Publish their source slot metadata into the same owned state.
                const auto readItemTable=[&](const char* file){return f::read_content(assets,(std::filesystem::path(options.combat.tableRoot)/file).generic_string());};
                const auto itemRecords=readItemTable("loot_table_pyarray.bin"),itemNames=readItemTable("loot_table_pyarraynames.bin"),itemFields=readItemTable("loot_table_pystructnames.bin");
                dh2::data::ItemTable items;
                if(!dh2::data::load_items({itemRecords.data(),itemRecords.size()},{itemNames.data(),itemNames.size()},{itemFields.data(),itemFields.size()},items,error))throw std::runtime_error("Diagnostic equipment source slots: "+error);
                for(auto& entry:player->equipment)if(entry.item_instance_id) {
                    const auto* item=dh2::data::item(items,dh2::data::item_id(items,entry.definition_id));
                    if(!item)throw std::runtime_error("Diagnostic equipped definition is absent from original ItemTable");
                    const int sourceSlot=entry.slot=="main_hand"?1:entry.slot=="off_hand"?2:item->record.words[26];
                    const bool slotKnown=sourceSlot>=0&&sourceSlot<9;
                    if(slotKnown)entry.slot="slot-"+std::to_string(sourceSlot);
                    state.inventory.push_back({*entry.item_instance_id,entry.definition_id,1});
                    state.equipment.push_back({entry.slot,*entry.item_instance_id,0,slotKnown?sourceSlot:-1});
                }
                // P14 EQUIP diagnostic (--bag-item DEF): unequipped rows for the direct bootstrap, which otherwise owns only equipped items.
                for(std::size_t bag=0;bag<options.bagItemIds.size();++bag)
                    state.inventory.push_back({"bag-"+std::to_string(bag)+"-"+options.bagItemIds[bag],options.bagItemIds[bag],1});
            }
            equipmentStateInitialized=true;
        }
        if(options.saveNow&&!f::save_character(options.save,state,error))throw std::runtime_error("Save: "+error);
        f::OriginalActorLifecycle actorLifecycle;
        f::OriginalCampaignRuntime sourceCampaign;
        f::SourceWorldObjects sourceObjects;
        f::CampaignCameraAdapter sourceCameraTargets;
        f::OriginalCampaignWorldAdapter campaignWorld(actorLifecycle,&sourceCameraTargets);
        // P16 HOST: script-host providers and generic trigger zones (bound only with --campaign-triggers).
        f::campaign_host::CampaignHostServices hostServices;
        f::CameraClipLibrary cameraClipLibrary; // P16 CINE2: PlayCamera dictionary -> cs_* clip bytes
        cameraClipLibrary.set_scene_file(originalCamera.config().file); // the level camera the clips drive
        hostServices.read_camera_clip=[&](std::int32_t id,std::vector<std::uint8_t>& clip,std::vector<std::uint8_t>& scene,std::string& path,std::string& e){return cameraClipLibrary.read(assets,id,clip,scene,path,e);};
        hostServices.all_actors=[&](std::vector<f::ActorId>& out,std::string& e){if(!combatSession){e="Campaign host needs the live combat session";return false;}out.clear();for(const auto& entry:combatSession->world()->actors())out.push_back(entry.first);return true;};
        hostServices.actor_state=[&](f::ActorId id,bool& alive,std::int32_t& state,std::string& e){const auto* actor=combatSession?combatSession->actor(id):nullptr;if(!actor){e="Campaign host actor unavailable";return false;}alive=actor->alive();state=combatSession->original_actor_state(id);return true;};
        hostServices.set_actor_state=[&](f::ActorId id,std::int32_t state,std::string& e){return combatSession&&combatSession->set_actor_original_state(id,state,e);};
        f::campaign_host::CampaignHost campaignHost(hostServices);
        bool globalControllerBlocked=false;
        std::map<f::ActorId,bool> characterControllerBlocked;
        f::CampaignCameraFrame lastSourceCameraFrame;
        std::map<f::ActorId,bool> lifecyclePhysical,lifecycleCollisions,lifecycleIdleSuppressed;
        std::map<f::ActorId,std::uint32_t> lifecycleFlags;
        // P16 SPAWN: explicit --lifecycle-spawn choice when given (intro path); otherwise the source Spawn state's
        // first leaf, the same clip Summon(spawn=true) plays through SM_SetSpawnState. Function scope on purpose:
        // the lifecycle services stored by bind() outlive the enclosing lifecycle block.
        const auto lifecycleSpawnChoice=[&options](const std::string& profileId)->f::OriginalAttackSelection {
            const auto explicitChoice=options.lifecycleSpawns.find(profileId);
            if(explicitChoice!=options.lifecycleSpawns.end())return explicitChoice->second;
            f::OriginalAttackSelection generic;generic.state="Spawn";generic.variant=0;
            return generic;
        };
        const bool lifecycleEnabled=!options.lifecycleSpawns.empty()||!spawnPool.empty()||!options.spawnDeclared.empty(); // P16 SPAWN pool slots and declared spawns need the lifecycle
        if((options.retainHiddenActors||lifecycleEnabled)&&!combatSession)throw std::runtime_error("Deferred live actors require the shared combat registry");
        if(!options.sourceCommands.empty()&&options.campaignCommands.empty())throw std::runtime_error("Source command replay requires original campaign XML");
        std::shared_ptr<f::SourceRootScopes> sourceScopes;
        std::unique_ptr<f::SourceModuleFloors> sourceFloors;
        if((options.sourceFloorProbe||options.sourceFloorMotion)&&options.sourceRootScopes.empty())throw std::runtime_error("Source floor world requires --source-root-scopes URI");
        if(options.sourceFloorMotion&&(!options.sourceNativeBodies||!options.sourceBodyBounds||!motor||!combatSession))throw std::runtime_error("Source floor motion requires shared movement, body plans and native actor ownership");
        if(options.sourceHeadingRotation&&(!options.sourceFloorMotion||!combatSession->uses_retained_player_locomotion()))throw std::runtime_error("Source heading/rotation requires shared original floor movement and retained locomotion");
        bool sourceMoveFocusActive=false;
        float sourceVisualYaw=motor?motor->state().facingRadians:0;
        double sourceRotationFractionMs=0;
        std::uint64_t sourceHeadingChecks=0,sourceHeadingSlides=0,sourceLateRotations=0;
        std::uint64_t sourceTargetNodeQueries=0,sourceTargetCacheWrites=0;
        auto actorVisual=[&](f::ActorId id)->f::CharacterVisual* {
            if(id==combatSession->player_id())return &visual;
            for(auto& placed:population.actors())if(placed.definition.stableId==id)return &placed.visual;
            return nullptr;
        };
        auto initializeSourceTargetNodes=[&]() {
            if(!options.sourceTargetPosition)return;
            if(!combatSession)throw std::runtime_error("Source target nodes require the shared actor world");
            unsigned nodes=0,absent=0;
            for(const auto& entry:combatSession->world()->actors()) {
                auto* actor=combatSession->actor(entry.first);auto* graphics=actorVisual(entry.first);
                if(!graphics||!graphics->loaded())throw std::runtime_error("Source target lookup requires its actual loaded visual hierarchy");
                f::original_target_position_ctor_prefix(actor->source_target_node180,actor->source_target_position184);
                std::uintptr_t node=0;if(!graphics->source_target_node(node,error))throw std::runtime_error(error);
                actor->source_target_node180=node;if(node)++nodes;else ++absent;
            }
            std::cout<<"Source own target nodes bound="<<nodes<<" absent="<<absent<<"; actual authored NAME lookup, full InitFinal remains incomplete\n";
        };
        std::shared_ptr<f::SourceNavigationWorldStorage> sourceObstacles;
        auto initializeSourceNavigation=[&]() {
            if(!options.sourceFloorMotion)return;
            if(!sourceFloors)throw std::runtime_error("Source navigation requires completed actual floor world");
            sourceObstacles=std::make_shared<f::SourceNavigationWorldStorage>(sourceFloors->world(),sourceBodyPlans.size());
            f::OriginalActorNavigationWorldBindings world;
            world.floor_lease=sourceFloors->world();world.obstacle_lease=sourceObstacles;
            world.floors=sourceFloors->world().get();world.obstacles=&sourceObstacles->registry;
            for(const auto& entry:sourceBodyPlans) {
                if(!nativeBodies.initialize_navigation(entry.first,world,error))throw std::runtime_error("Source navigation initialization: "+error);
                const auto* actor=combatSession->actor(entry.first);const auto* nav=nativeBodies.navigation(entry.first);
                std::cout<<"Motion provenance stage=post-navigation actor="<<entry.first<<" position="<<actor->transform.position[0]<<','<<actor->transform.position[1]<<','<<actor->transform.position[2]<<" pf="<<nav->motion.position[0]<<','<<nav->motion.position[1]<<','<<nav->motion.position[2]<<" floor="<<nav->motion.floor<<" room="<<nav->motion.room<<std::endl;
                if(options.sourceHeadingRotation&&!f::original_character_motion_ctor_prefix(combatSession->actor(entry.first)->source_validate_boundary452,error))throw std::runtime_error(error);
            }
            sourceMoveFocusActive=false;sourceVisualYaw=combatSession->actor(combatSession->player_id())->transform.rotation[2];
            std::cout<<"Original floor motion bound actors="<<sourceBodyPlans.size()<<" obstacles="<<sourceObstacles->registry.count<<"; shared PF position/direction policy, native Step remains unbound\n";
        };
        int sourceCommandContext=-1;
        auto rebuildSourceScopes=[&]() {
            if(options.sourceCommands.empty()&&!options.retainHiddenActors&&!lifecycleEnabled&&options.sourceRootScopes.empty()&&!options.campaignTriggers)return; // P16 HOST
            if(!sourceObjects.load(population.definitions(),error))throw std::runtime_error(error);
            if(!options.sourceRootScopes.empty()) {
                sourceScopes=std::make_shared<f::SourceRootScopes>();
                if(!sourceScopes->build(assets,options.sourceRootScopes,population.definitions(),sourceObjects,error))throw std::runtime_error("Source constructor scopes: "+error);
                std::cout<<"Source root constructor scopes modules="<<sourceScopes->count()<<"; full Level initialization remains incomplete\n";
                for(const auto& module:sourceScopes->modules().entries())std::cout<<"Source module scope id="<<module.runtimeId<<" occurrence="<<module.occurrence.hostOccurrence<<'\n';
            }
            if(options.sourceFloorProbe||options.sourceFloorMotion) {
                if(!sourceScopes)throw std::runtime_error("Source floor probe requires actual constructor scopes");
                sourceFloors=std::make_unique<f::SourceModuleFloors>(std::make_shared<dh2::floors::World>());
                for(const auto& module:sourceScopes->modules().entries()) {
                    f::SourceModuleFloorRequest request;request.trace=module;
                    request.pose_phase=f::SourceModulePosePhase::constructor_degrees;
                    // Explicit geometry diagnostic admission, matching the
                    // loaded validation scene. Campaign gates are not inferred.
                    request.admitted=true;
                    for(const auto& record:sourceScopes->module_records())if(record->receiver->base().identity()==module.receiver.identity)request.record=record;
                    std::shared_ptr<f::SourceModuleFloorRoom> room;
                    if(!sourceFloors->load(assets,request,room,error))throw std::runtime_error("Source floor load: "+error);
                }
                if(!sourceFloors->post_load(error))throw std::runtime_error("Source floor postload: "+error);
                std::cout<<"Source floor diagnostic rooms="<<sourceFloors->rooms().size()<<" floors="<<sourceFloors->world()->records.size()<<"; explicit geometry admission, campaign gates remain separate\n";
                initializeSourceNavigation();
            }
            if(!options.campaignContextObject.empty()) {
                f::ActorId contextObject=0;bool found=false;
                if(!sourceObjects.named_object(options.campaignContextObject,-1,contextObject,found,error)||!found||!sourceObjects.module_context(contextObject,sourceCommandContext,error))throw std::runtime_error("Source command context: "+error);
            }
        };
        rebuildSourceScopes();
        initializeSourceTargetNodes();
        f::effects::RuntimeEffectsRendererV1 sourceEffectsRenderer(renderer);
        dh2::data::EffectsTables sourceEffectsTables;
        std::unique_ptr<f::effects::RuntimeEffectsFactoryV1> sourceEffectsFactory;
        std::shared_ptr<f::effects::RuntimeSwingFxObserverV1> sourceSwingFx;
        std::unique_ptr<f::effects::CelestTargetFxDispatchV1> celestEffects;
        f::Camera sourceEffectsCamera;
        struct SourceEffectsCameraContext {
            std::function<bool(const dh2::scene::Scene&,float*,float*,std::string&)> sample;
        } sourceEffectsCameraContext;
        sourceEffectsCameraContext.sample=[&](const dh2::scene::Scene& borrowed,float* view,float* position,std::string& e) {
            const auto* current=combatSession?combatSession->retained_actor_visual_borrow(combatSession->player_id()):nullptr;
            if(!current||current->retained_scene_borrow()!=&borrowed){e="FX camera lost the exact current retained actor Scene";return false;}
            f::CameraPose pose{{sourceEffectsCamera.eye.x,sourceEffectsCamera.eye.y,sourceEffectsCamera.eye.z},
                {sourceEffectsCamera.target.x,sourceEffectsCamera.target.y,sourceEffectsCamera.target.z},
                {sourceEffectsCamera.up.x,sourceEffectsCamera.up.y,sourceEffectsCamera.up.z},sourceEffectsCamera.verticalFovDegrees};
            const auto matrix=f::cameraViewMatrix(pose);std::copy(matrix.begin(),matrix.end(),view);
            position[0]=sourceEffectsCamera.eye.x;position[1]=sourceEffectsCamera.eye.y;position[2]=sourceEffectsCamera.eye.z;e.clear();return true;
        };
        std::int32_t sourceEffectsMs=0;double sourceEffectsFractionMs=0;
        bool sourceEffectsPresentationFailed=false;
        std::uint64_t sourceEffectsPacketFrames=0,sourceEffectsPackets=0;
        const auto retireSourceEffects=[&]() {
            sourceSwingFx.reset();
            std::string e;
            if(!sourceEffectsRenderer.finish_and_drain(e))throw std::runtime_error("FX queue drain: "+e);
            celestEffects.reset();
            if(sourceEffectsFactory)sourceEffectsFactory->clear_original_textures();
            sourceEffectsFactory.reset();
        };
        const auto bindSourceEffects=[&]() {
            retireSourceEffects();sourceEffectsMs=0;sourceEffectsFractionMs=0;sourceEffectsPresentationFailed=false;
            if(!combatSession||!sourceFloors)return;
            std::string e;
            try {
                const auto records=f::read_content(assets,"data/pydata/effects_pyarray.bin");
                const auto names=f::read_content(assets,"data/pydata/effects_pyarraynames.bin");
                const auto schema=f::read_content(assets,"data/pydata/effects_pystructnames.bin");
                const auto dictionaryNames=f::read_content(assets,"data/effects_dictionary_pyarraynames.bin");
                const auto dictionaryPaths=f::read_content(assets,"data/effects_dictionary_pyarray.bin");
                const auto bytes=[](const auto& data){return dh2::data::Bytes{data.data(),data.size()};};
                if(!sourceEffectsTables.load(bytes(records),bytes(names),bytes(schema),bytes(dictionaryNames),bytes(dictionaryPaths),e))throw std::runtime_error(e);
                f::effects::CelestTargetFxIdsV1 ids;
                if(!f::effects::resolve_celest_target_fx_ids_v1(sourceEffectsTables.borrow(),ids,e))throw std::runtime_error(e);
                f::effects::RuntimeEffectsFactoryBindingsV1 bindings;
                bindings.assets=&assets;bindings.tables=sourceEffectsTables.borrow();bindings.same_pf_world=&sourceFloors->world()->collision_world;
                bindings.scene_view.context=&sourceEffectsCameraContext;
                bindings.scene_view.camera=[](void* raw,const auto& scene,float* view,float* position,std::string& problem){return static_cast<SourceEffectsCameraContext*>(raw)->sample(scene,view,position,problem);};
                bindings.scene_view.particle_color_policy=f::effects::RuntimeEffectsParticleColorPolicyV1::source_white;
                sourceEffectsRenderer.bind_factory_services(bindings);
                sourceEffectsFactory=f::effects::RuntimeEffectsFactoryV1::create(*combatSession,combatSession->player_id(),std::move(bindings),e);
                if(!sourceEffectsFactory)throw std::runtime_error(e);
                celestEffects=std::make_unique<f::effects::CelestTargetFxDispatchV1>(*sourceEffectsFactory,sourceEffectsTables.borrow(),ids);
                std::cout<<"Source Celest FX bound player="<<combatSession->player_id()<<" PlayerSet="<<ids.player_pre<<" MainSet="<<ids.target_main<<" particleInput=source-white\n";
            } catch(const std::exception& problem) {std::cerr<<"Source Celest FX bind diagnostic: "<<problem.what()<<'\n';}
        };
        struct SourceMotionReceipt {std::uint64_t solves=0,accepted=0,clamped=0;};
        std::map<f::ActorId,SourceMotionReceipt> sourceMotionReceipts;
        std::set<f::ActorId> firstSourceMotionTraced;
        auto applyActorRootMotion=[&](f::ActorId id,f::ActorMovement& movement,f::Vec3 delta,std::string& e,bool authored=false) {
            if(!options.sourceFloorMotion){if(authored)movement.apply_authored_root_motion(delta,collision);else movement.apply_root_motion(delta,collision);return true;}
            f::ActorMovement::SourceMotionAdmission admission=[&](f::Vec3 current,f::Vec3 worldDelta,f::ActorMovement::SourceMotionAdmissionResult& out,std::string& problem) {
                const auto previousFloor=nativeBodies.navigation(id)->motion.floor;
                const auto previousRoom=nativeBodies.navigation(id)->motion.room;
                const bool traceMotion=(worldDelta.x!=0||worldDelta.y!=0||worldDelta.z!=0)&&firstSourceMotionTraced.insert(id).second;
                if(traceMotion){const auto* nav=nativeBodies.navigation(id);std::cout<<"Motion provenance stage=first-admission actor="<<id<<" current="<<current.x<<','<<current.y<<','<<current.z<<" delta="<<worldDelta.x<<','<<worldDelta.y<<','<<worldDelta.z<<" pf="<<nav->motion.position[0]<<','<<nav->motion.position[1]<<','<<nav->motion.position[2]<<" floor="<<previousFloor<<" room="<<previousRoom<<std::endl;}
                f::OriginalActorNavigationMoveResult result;
                if(!nativeBodies.move_grounded(id,current,worldDelta,result,problem))return false;
                if(traceMotion)std::cout<<"Motion provenance stage=first-admission-result actor="<<id<<" kind="<<result.kind<<" valid="<<result.position_valid<<" position="<<result.position.x<<','<<result.position.y<<','<<result.position.z<<std::endl;
                auto& receipt=sourceMotionReceipts[id];++receipt.solves;
                if(result.kind==2)++receipt.accepted;
                if(result.kind==3) {
                    if(!receipt.clamped) {
                        const float requested[]{current.x+worldDelta.x,current.y+worldDelta.y,current.z+worldDelta.z};
                        auto& world=sourceFloors->world()->collision_world;dh2::navigation::HeightHit cached{},global{};
                        int cachedHit=0;if(previousFloor<world.floor_count)cachedHit=dh2_nav_floor_height(&cached,world.floors[previousFloor].selector,requested);
                        const auto globalHit=dh2_nav_world_height(&global,&world,requested,0);
                        std::cout<<std::setprecision(9)<<"Source PF first clamp actor="<<id<<" current="<<current.x<<','<<current.y<<','<<current.z<<" requested="<<requested[0]<<','<<requested[1]<<','<<requested[2]<<" previousRoom="<<previousRoom<<" previousFloor="<<previousFloor<<" valid="<<result.position_valid<<" cachedHit="<<cachedHit<<" cachedHeight="<<cached.height<<" worldHit="<<globalHit<<" worldHeight="<<global.height<<" result="<<result.position.x<<','<<result.position.y<<','<<result.position.z<<std::setprecision(6)<<'\n';
                    }
                    ++receipt.clamped;
                }
                out.position=result.position;out.grounded=result.grounded;return true;
            };
            bool moved=false;return authored?movement.apply_authored_root_motion(delta,admission,moved,e):movement.apply_root_motion(delta,admission,moved,e);
        };
        // Controller commands belong to the shared actor world, independently
        // of whether this checkpoint binds any Spawn animation lifecycle.
        if(combatSession) {
            f::OriginalCampaignWorldProviders providers;
            providers.named_character=[&](const std::string& name,int module,f::ActorId& id,bool& found,std::string& e){return sourceObjects.named_character(name,module,id,found,e);};
            providers.global_controller_blocked=[&](bool blocked,std::string&){globalControllerBlocked=blocked;return true;};
            providers.character_controller_blocked=[&](f::ActorId id,bool blocked,std::string& e){if(!combatSession->actor(id)){e="Source character controller unavailable";return false;}characterControllerBlocked[id]=blocked;return true;};
            // P14 FAERY (T3): Script_SetFaeryState / Script_IncFaeryLevel write the live CharacterState and persist it.
            // Legacy saves without source Faery rows (known=false) are a logged limitation: the script continues, nothing is invented.
            providers.set_faery_state=[&](std::uint32_t slot,std::uint32_t value,std::string& e){
                if(!state.source_faery_state_known){std::cout<<"Source SetFaeryState slot="<<slot<<" skipped: legacy save has no source Faery rows (limitation)\n";return true;}
                if(!f::faery_menu::apply_source_set_faery_state_v1(state,f::faery_menu::active_faery_difficulty_v1(),slot,value,e))return false;
                std::cout<<"Source SetFaeryState slot="<<slot<<" state="<<value<<" committed to CharacterState\n";
                return f::save_character(options.save,state,e);};
            providers.inc_faery_level=[&](std::uint32_t slot,std::string& e){
                if(!state.source_faery_state_known){std::cout<<"Source IncFaeryLevel slot="<<slot<<" skipped: legacy save has no source Faery rows (limitation)\n";return true;}
                if(!f::faery_menu::apply_source_inc_faery_level_v1(state,f::faery_menu::active_faery_difficulty_v1(),slot,e))return false;
                std::cout<<"Source IncFaeryLevel slot="<<slot<<" level="<<state.faery_by_difficulty[std::size_t(f::faery_menu::active_faery_difficulty_v1())].faeries[slot].level<<" committed to CharacterState\n";
                return f::save_character(options.save,state,e);};
            if(options.campaignTriggers)campaignHost.bind_world_providers(providers); // P16 HOST
            campaignWorld.bind(std::move(providers));
            if(!options.sourceCommands.empty()) {
                combatSession->set_diagnostic_controller_admission_provider(
                    [&](f::ActorId id,f::DiagnosticControllerAdmissionFacts& facts,std::string& e){
                        if(!combatSession->actor(id)){e="Controller command actor unavailable";return false;}
                        facts.owner_id=id;facts.controllable=id;
                        facts.global_blocked=globalControllerBlocked;
                        facts.local_locked=characterControllerBlocked[id];
                        // Current in-house controllers issue ordinary commands;
                        // forced and network controllers have no live backend yet.
                        facts.forced=0;facts.network_enabled=0;return true;
                    },
                    [](f::ActorId,bool& online,std::string&){online=false;return true;});
            }
        }
        if((options.sourceCamera&&!options.sourceCommands.empty())||options.campaignTriggers) { // P16 HOST: camera admission also serves trigger scripts
            if(!combatSession)throw std::runtime_error("Source camera targets require the bound local player");
            f::CampaignCameraProviders providers;
            providers.local_player=[&](std::uint64_t& id,std::string&){id=combatSession->player_id();return true;};
            providers.named_target=[&](const std::string& name,std::uint64_t& id,bool& found,std::string& e){
                if(name=="Player"||name=="LocalPlayer"){id=combatSession->player_id();found=true;return true;}
                return sourceObjects.named_object(name,-1,id,found,e);
            };
            providers.anchor=[&](std::uint64_t id,f::CameraVec3& anchor,std::string& e){
                if(id==combatSession->player_id()){anchor=actorCameraAnchor.initialized()?actorCameraAnchor.position():f::CameraVec3{options.actorPosition.x,options.actorPosition.y,options.actorPosition.z};return true;}
                return sourceObjects.anchor(id,anchor,e);
            };
            sourceCameraTargets.bind(std::move(providers));
            if(!sourceCameraTargets.seed_target(combatSession->player_id(),error))throw std::runtime_error(error);
        }
        if(options.retainHiddenActors||lifecycleEnabled) {
            combatSession->set_actor_combat_permission_provider([&](f::ActorId id){
                if(id==combatSession->player_id())return true;
                if(actorLifecycle.status(id))return actorLifecycle.combat_enabled(id)&&lifecyclePhysical[id]&&lifecycleCollisions[id];
                const auto placed=std::find_if(population.actors().begin(),population.actors().end(),[&](const auto& p){return p.definition.stableId==id;});
                return placed!=population.actors().end()&&placed->enabled;
            });
        }
        if(lifecycleEnabled) {
            f::CombatSessionStateAnimationServices animationServices;
            animationServices.event=[&](f::ActorId id,const f::RetainedAnimationEvent& event,std::string& e){return actorLifecycle.animation_event(id,event.name,e);};
            animationServices.finished=[&](f::ActorId id,std::string& e){
                if(!actorLifecycle.animation_finished(id,e))return false;
                std::cout<<"Lifecycle whole sequence finished actor="<<id<<" state="<<actorLifecycle.status(id)->state<<'\n';return true;
            };
            f::OriginalLifecycleServices services;
            services.floor_height=[&](std::array<float,3> p,bool& found,float& height,std::string& e){
                if(options.sourceFloorMotion) {
                    dh2::navigation::HeightHit hit{};hit.height=p[2];
                    const auto status=dh2_nav_world_height(&hit,&sourceFloors->world()->collision_world,p.data(),0);
                    if(status<0){e="Original lifecycle floor query rejected source world";return false;}
                    found=status==1;if(found)height=hit.height;return true;
                }
                f::FloorHit hit;found=collision.floor(p[0],p[1],p[2],hit);if(found)height=hit.point.z;return true;
            };
            // Physical presence/collision flags below govern this host's existing
            // render-bound movement approximation; source physical joint bodies,
            // navigation and actor/actor collisions remain separate gaps.
            services.invoke=[&,animationServices](const f::OriginalLifecycleRequest& request,std::string& e){
                auto& actor=*request.actor;
                const auto placed=std::find_if(population.actors().begin(),population.actors().end(),[&](const auto& p){return p.definition.stableId==actor.id;});
                if(placed==population.actors().end()){e="Lifecycle population visual unavailable";return false;}
                switch(request.operation) {
                case f::OriginalLifecycleOperation::set_flags:lifecycleFlags[actor.id]=request.flags;return f::original_motion_source_set_flags(actor.source_flags520,request.flags,e);
                case f::OriginalLifecycleOperation::set_enabled:return population.set_enabled(actor.id,request.enabled,e);
                case f::OriginalLifecycleOperation::restore_initial_position:
                    if(options.sourceNativeBodies)return nativeBodies.set_position(actor.id,request.initial_transform.position,true,e);
                    actor.transform.position=request.initial_transform.position;return true;
                case f::OriginalLifecycleOperation::restore_initial_rotation:actor.transform.rotation=request.initial_transform.rotation;return true;
                case f::OriginalLifecycleOperation::revive:actor.health=actor.max_health;actor.resource=actor.max_resource;f::reset_actor_action(actor,f::CharacterAction::idle);return true;
                case f::OriginalLifecycleOperation::clear_aggro:
                case f::OriginalLifecycleOperation::clear_and_sync_target:
                    return combatSession->set_source_target(actor.id,f::invalid_actor_id,false,e)&&combatSession->sync_source_last_target(actor.id,e);
                case f::OriginalLifecycleOperation::cancel_sneaking:return true; // No sneak state is instantiated by this diagnostic host.
                case f::OriginalLifecycleOperation::clear_idle_suppressed:lifecycleIdleSuppressed[actor.id]=false;return true;
                case f::OriginalLifecycleOperation::init_physical:
                    if(options.sourceNativeBodies){const auto* body=nativeBodies.physical(actor.id);if(!body){e="Lifecycle native actor unavailable";return false;}if(!body->native().body&&!nativeBodies.initialize_physical(actor.id,e))return false;}
                    lifecyclePhysical[actor.id]=true;return true;
                case f::OriginalLifecycleOperation::remove_physical:
                    if(options.sourceNativeBodies&&!nativeBodies.remove_physical(actor.id,e))return false;
                    lifecyclePhysical[actor.id]=false;return true;
                case f::OriginalLifecycleOperation::set_collisions_enabled:
                    if(options.sourceNativeBodies&&nativeBodies.physical(actor.id)->native().body&&!nativeBodies.set_physical_filter_enabled(actor.id,request.collisions_enabled,e))return false;
                    lifecycleCollisions[actor.id]=request.collisions_enabled;return true;
                case f::OriginalLifecycleOperation::freeze_animation_speed:return combatSession->freeze_actor_state_animation(actor.id,e);
                case f::OriginalLifecycleOperation::notify_state_changed:
                    if(!combatSession->set_actor_original_state(actor.id,request.state,e))return false;
                    std::cout<<"Lifecycle state actor="<<actor.id<<" previous="<<request.previous_state<<" state="<<request.state<<" enabled="<<placed->enabled<<'\n';return true;
                case f::OriginalLifecycleOperation::select_state_animation: {
                    const auto policy=options.combat.profiles.find(placed->profileId);
                    if(policy==options.combat.profiles.end()){e="Source lifecycle profile unavailable";return false;}
                    // P16 LIFECYCLE: request.state is the admitted transition target (Blur/Focus recipes in the consumer).
                    if(request.state==1)return combatSession->play_actor_state_sequence(actor.id,lifecycleSpawnChoice(placed->profileId),animationServices,e,request.state);
                    if(request.state==3)return combatSession->select_actor_state_leaf(actor.id,policy->second.initialIdle,1,false,animationServices,e,request.state);
                    // P16 DESPAWN: lifecycle state 2 plays the actor's Despawn clip (CSDespawn::OnFocus). Its end is the Limbus transition.
                    if(request.state==2)return combatSession->select_actor_state_leaf(actor.id,f::CombatSessionChoice{"Despawn",0,{0}},1,false,animationServices,e,request.state);
                    const auto* source=meleeBindings.find_actor(placed->profileId);
                    const auto pre=source->states.find("PreSpawn");
                    if(pre==source->states.end()){e="PreSpawn source availability metadata absent";return false;}
                    if(!pre->second.empty()) {
                        const auto chosen=options.lifecyclePreSpawns.find(placed->profileId);
                        if(chosen==options.lifecyclePreSpawns.end()){e="Authored PreSpawn needs explicit leaf selection";return false;}
                        return combatSession->select_actor_state_leaf(actor.id,chosen->second,1,false,animationServices,e,request.state);
                    }
                    const auto spawn=lifecycleSpawnChoice(placed->profileId);
                    auto path=spawn.group_path;path.push_back(0);
                    return combatSession->select_actor_state_leaf(actor.id,{spawn.state,spawn.variant,path},1,true,animationServices,e,request.state);
                }}
                e="Unimplemented lifecycle operation";return false;
            };
            actorLifecycle.bind(std::move(services));
            // P16 SPAWN: an authored declaration joins the lifecycle only when a --spawn-declared trigger names it (its
            // Limbus/PreSpawn preset then starts at PreSpawn17, as in the source). Every other declaration is unchanged.
            const auto declaredNamed=[&](const std::string& name){return std::any_of(options.spawnDeclared.begin(),options.spawnDeclared.end(),[&](const auto& request){return request.name==name;});};
            for(const auto& placed:population.actors())if(options.lifecycleSpawns.count(placed.profileId)||spawnPool.owns(placed.definition.stableId)||declaredNamed(placed.definition.name)) { // P16 SPAWN pool slots
                auto* actor=combatSession->actor(placed.definition.stableId);
                const auto* source=meleeBindings.find_actor(placed.profileId);
                if(!actor||!source)throw std::runtime_error("Lifecycle actor needs a retained shared profile");
                f::OriginalLifecycleFacts facts;facts.initial_transform=actor->transform;facts.initially_enabled=placed.enabled;
                const auto preset=placed.definition.properties.find("ai_state");
                // Original Character declaration13cc defaults to the empty name.
                facts.preset_ai_state=preset==placed.definition.properties.end()?"":preset->second;
                if(facts.preset_ai_state=="Limbus"||facts.preset_ai_state=="PreSpawn") {
                    const auto pre=source->states.find("PreSpawn");if(pre==source->states.end())throw std::runtime_error("Source PreSpawn availability missing");
                    facts.pre_spawn_has_animation=!pre->second.empty();
                    const auto visible=placed.definition.properties.find("ai_state_visible");
                    // Original Character property declaration13e4 defaults to1.
                    // Absence differs from the intro actors' authored zero.
                    if(visible!=placed.definition.properties.end()&&visible->second!="0"&&visible->second!="1")throw std::runtime_error("Invalid source PreSpawn visibility fact");
                    facts.pre_spawn_stay_enabled=visible==placed.definition.properties.end()||visible->second=="1";
                }
                lifecyclePhysical[actor->id]=placed.enabled;lifecycleCollisions[actor->id]=placed.enabled;
                if(!actorLifecycle.add(*actor,facts,error))throw std::runtime_error("Lifecycle initialization: "+error);
            }
            std::cout<<"Lifecycle diagnostic uses source animations and enable/state gates; full source physical/aggro/sneak owners remain incomplete\n";
        }
        if(options.retainHiddenActors||lifecycleEnabled)if(!combatSession->refresh_actor_combat_permissions(error))throw std::runtime_error(error);
        if(!options.campaignCommands.empty()) {
            if(!sourceCampaign.load(assets,options.campaignCommands,error))throw std::runtime_error(error);
            for(const auto& scheduled:options.sourceCommands) {
                const auto id=sourceCampaign.script_id(scheduled.script);
                if(id<0||scheduled.index>=sourceCampaign.scripts().at(id).commands.size())throw std::runtime_error("Unknown serialized source command");
            }
            std::cout<<"Serialized command replay context="<<sourceCommandContext<<"; original trigger admission and complete cutscene/UI providers remain incomplete\n";
        }
        std::cout<<"Character name="<<state.name<<" class="<<state.class_id<<" xp="<<state.experience<<'\n';
        // P16 HOST: live executor binding and trigger zones from the loaded level declarations (off by default).
        if(options.campaignTriggers) {
            if(options.campaignCommands.empty()||!combatSession)throw std::runtime_error("--campaign-triggers requires --campaign-commands and the live combat session");
            if(!campaignHost.bind_executor(sourceCampaign,campaignWorld,error))throw std::runtime_error("Campaign host: "+error);
            if(!campaignHost.build_zones(population.definitions(),error))throw std::runtime_error("Campaign trigger zones: "+error);
            // P16 CINE: harness start of an authored script by name (same runtime start as DoTutorial; not a production starter).
            if(!options.campaignStart.empty()) {
                const int script=sourceCampaign.script_id(options.campaignStart,true);
                if(script<0)throw std::runtime_error("--campaign-start: no authored script named "+options.campaignStart);
                if(!sourceCampaign.start(script,-1,true,error))throw std::runtime_error("--campaign-start "+options.campaignStart+": "+error);
                std::cout<<"[campaign] harness start script="<<options.campaignStart<<" id="<<script<<'\n';
            }
        }
        if(options.probe) {
            if(visual.loaded()) for(auto pose:{f::CharacterPose::idle,f::CharacterPose::walk,f::CharacterPose::attack}) {
                visual.select(pose);if(!visual.update(.25,error)) throw std::runtime_error(error);
                std::size_t count=0;for(const auto& mesh:visual.meshes()) count+=mesh.vertices.size();
                std::cout<<"Animation "<<visual.animation_name()<<" vertices="<<count<<'\n';
            }
            std::cout<<"Content probe completed; rendering not exercised.\n";return 0;
        }
        const int initialHeight=options.windowHeight,initialWidth=options.windowWidth?options.windowWidth:(options.sourceCamera?int(std::lround(initialHeight*originalCamera.sourceAspect())):1280);
        if(!windowOpened&&!window.open(combatSession?"Dungeon Hunter reconstruction | WASD move | Space attack | Tab target | F5/F9 save":"Game foundation | WASD move/camera | F5/F9 save",initialWidth,initialHeight)) throw std::runtime_error(window.error());
        if(windowOpened&&window.width()!=initialWidth&&!window.resize(initialWidth,initialHeight))throw std::runtime_error(window.error());
        float sourceProjectionAspect=options.sourceCamera?originalCamera.sourceAspect():0;int previousWidth=window.width(),previousHeight=window.height();
        if(!windowOpened&&!renderer.initialize(window.width(),window.height())) throw std::runtime_error("Renderer initialization failed");
        std::cout<<"OpenGL "<<glGetString(GL_VERSION)<<" / "<<glGetString(GL_RENDERER)<<'\n';
        std::map<std::string,std::uint32_t> textures;
        auto loadImage=[&](const std::string& uri,dh::foundation::TextureImage& image)->bool {
            try {if(f::load_texture(f::resolve_content_path(assets,uri),image,error))return true;}catch(const std::exception&){}
            auto name=fs::path(uri).filename();std::vector<fs::path> candidates={fs::path(uri),fs::path("data/3d/textures")/name,fs::path("original-cache/data/3d/textures")/name,fs::path("textures")/name};
            for(const auto& candidate:candidates) try {if(dh::foundation::load_texture(assets.resolve(candidate),image,error))return true;} catch(const std::exception&){}
            std::cerr<<"Missing/unsupported texture: "<<uri<<'\n';return false;
        };
        auto loadTexture=[&](const std::string& uri,const std::string& mask=std::string(),bool blue=false)->std::uint32_t {
            if(uri.empty()&&mask.empty())return 0;
            auto key=uri+"|"+mask+(blue?"|blue":"|red");
            if(auto it=textures.find(key);it!=textures.end()) return it->second;
            dh::foundation::TextureImage image;
            if(uri.empty()){image.width=image.height=1;image.rgba={255,255,255,255};}
            else if(!loadImage(uri,image)){textures[key]=0;return 0;}
            if(!mask.empty()) {
                dh::foundation::TextureImage alpha;
                if(!loadImage(mask,alpha))throw std::runtime_error("Required alpha map missing: "+mask);
                for(std::uint32_t y=0;y<image.height;++y)for(std::uint32_t x=0;x<image.width;++x){
                    auto dst=(std::size_t(y)*image.width+x)*4+3;
                    auto mx=std::uint32_t(std::uint64_t(x)*alpha.width/image.width),my=std::uint32_t(std::uint64_t(y)*alpha.height/image.height);
                    auto value=alpha.rgba[(std::size_t(my)*alpha.width+mx)*4+(blue?2:0)];
                    image.rgba[dst]=blue?value:std::uint8_t((unsigned(image.rgba[dst])*value+127)/255);
                }
            }
            auto id=renderer.createTexture(image.width,image.height,image.rgba.data());if(!id)throw std::runtime_error("GPU texture upload failed");textures[key]=id;return id;
        };
        auto bindMaterials=[&]() {
        for(std::size_t i=0;i<scene.materials.size();++i) {
            const auto& m=scene.materials[i];
            const bool blue=m.effectFile=="GL_Diffuse_L1_VC_iPhone.bdae"&&m.technique=="L1_Vc_Al_----_----_----_----";
            scene.mesh.ranges[i].material.texture=loadTexture(m.diffuse,m.alphaMap,blue);
        }
        if(visual.loaded()) for(std::size_t i=0;i<visual.mutable_meshes().size();++i)
            for(auto& r:visual.mutable_meshes()[i].ranges) r.material.texture=loadTexture(visual.texture_uris()[i]);
        for(auto& actor:population.actors()) for(std::size_t i=0;i<actor.visual.mutable_meshes().size();++i) {
            const auto& m=actor.visual.original_materials()[i];
            bool blue=m.effectFile=="GL_Diffuse_L1_VC_iPhone.bdae"&&m.technique=="L1_Vc_Al_----_----_----_----";
            for(auto& range:actor.visual.mutable_meshes()[i].ranges)range.material.texture=loadTexture(m.diffuse,m.alphaMap,blue);
        }
        for(auto& attached:equipment.mutable_attachments())for(std::size_t i=0;i<attached.visual.mutable_meshes().size();++i) {
            const auto& m=attached.visual.original_materials()[i];bool blue=m.effectFile=="GL_Diffuse_L1_VC_iPhone.bdae"&&m.technique=="L1_Vc_Al_----_----_----_----";
            for(auto& range:attached.visual.mutable_meshes()[i].ranges)range.material.texture=loadTexture(m.diffuse,m.alphaMap,blue);
        }
        for(std::size_t i=0;i<targetMarker.materials.size();++i)targetMarker.mesh.ranges[i].material.texture=loadTexture(targetMarker.materials[i].diffuse,targetMarker.materials[i].alphaMap);
        };
        // P16 containers: bind the original textures of each container visual (same rule as actors).
        for(const auto& view:containerRuntime.views())if(view.visual)for(std::size_t i=0;i<view.visual->mutable_meshes().size();++i){
            const auto& m=view.visual->original_materials()[i];const bool blue=m.effectFile=="GL_Diffuse_L1_VC_iPhone.bdae"&&m.technique=="L1_Vc_Al_----_----_----_----";
            for(auto& range:view.visual->mutable_meshes()[i].ranges)range.material.texture=loadTexture(m.diffuse,m.alphaMap,blue);}
        bindMaterials();
        std::uint32_t hudTexture=options.hud?loadTexture("MenusGraphics_droid.tga"):0;f::OverlayRenderer overlay;
        f::CombatTextLiveAdapter combatText;
        std::optional<f::Camera> combatTextCamera;
        std::uint64_t combatTextResults=0,combatTextDrawnFrames=0;
        std::uint64_t combatTextLabels=0;int combatTextFrame=-1;
        double combatTextFractionMs=0;
        auto bindCombatText=[&]() {
            if(!options.combatText)return;
            if(!combatSession||!options.hud||!options.sourceBodyBounds||!options.sourceTargetPosition||!sourceScopes)
                throw std::runtime_error("Combat text requires shared combat/HUD and source body, target and debug providers");
            f::CombatTextLiveServices services;
            services.delivery=f::CombatTextDeliveryMode::synchronous_snapshot;
            services.design.debug=[&](const char* key,std::string& e){bool value=false;return sourceScopes->debug_switch(key,value,e);};
            services.design.player_character=[&](std::uintptr_t& id,std::string&){id=combatSession->player_id();return true;};
            services.design.player_name=[&](std::uintptr_t id,std::string& name,std::string& e){
                if(id!=combatSession->player_id()){e="Combat text localization queried another player";return false;}
                name=state.name;return true;
            };
            services.target_position=[&](f::ActorId id,std::array<float,3>& out,std::string& e){
                const auto* actor=combatSession->actor(id);if(!actor){e="Combat text actor position owner unavailable";return false;}
                auto enabled=[&](std::uint8_t& value,std::string& problem){
                    if(id==combatSession->player_id()){value=1;return true;}
                    for(const auto& placed:population.actors())if(placed.definition.stableId==id){value=placed.enabled;return true;}
                    problem="Combat text enabled owner unavailable";return false;
                };
                const f::OriginalTargetPosition* selected=nullptr;
                if(!f::original_get_target_position(actor->source_target_node180,actor->source_target_position184,
                    actor->transform.position,enabled,selected,e))return false;
                out=*selected;return true;
            };
            services.bounds_height=[&](f::ActorId id,float& value,std::string& e){
                const auto found=sourceBodyPlans.find(id);if(found==sourceBodyPlans.end()){e="Combat text source relative bounds unavailable";return false;}
                const auto& box=found->second.bounds.relative_box;value=box[5]-box[2];return true;
            };
            services.project=[&](f::Vec3 point,float& x,float& y,std::string& e){
                std::array<float,2> screen;
                if(!combatTextCamera||!project(*combatTextCamera,point,window.width(),window.height(),screen)){e="Combat text point cannot be projected by current camera";return false;}
                x=screen[0];y=screen[1];return true;
            };
            services.glyph_draw=[&](const auto& glyphs,std::string& e){return drawCombatGlyphs(glyphs,renderer,overlay,textures,e);};
            services.display_event=[&](const f::CombatTextDisplayEvent& event,std::string&){
                ++combatTextLabels;
                std::cout<<"Combat text frame="<<combatTextFrame<<" style="<<event.style<<" text="<<event.text<<" color="<<event.argb<<" position="<<event.position.x<<','<<event.position.y<<','<<event.position.z<<'\n';return true;
            };
            if(!combatText.load(assets,*combatSession,std::move(services),error))throw std::runtime_error("Original combat text: "+error);
            combatSession->world()->set_resolution_observer([&](const auto& resolution,std::string& e){
                if(!combatText.capture_synchronous_result(resolution,e))return false;
                ++combatTextResults;return true;
            });
            combatTextFractionMs=0;
        };
        bindCombatText();
        f::character_menu::MenuLocalization menuLocalization;
        if(options.hud&&combatSession&&!menuLocalization.load(assets,"original-cache/data",0,error))throw std::runtime_error("Character menu labels: "+error);
        if(options.hud&&combatSession&&!menuLocalization.bind_profile(&state,error))throw std::runtime_error("Character menu profile: "+error);
        // P16 CINE: caption lines resolve their authored StrID through the same original localization owner.
        if(options.campaignTriggers) campaignHost.set_caption_text([&menuLocalization](std::int32_t id,std::string& text,std::string& e){return menuLocalization.string_id(id,text,e);});
        f::CameraPose start;
        float extent=200;
        if(!scene.mesh.vertices.empty()) {
            start.target={(scene.minimum.x+scene.maximum.x)/2,(scene.minimum.y+scene.maximum.y)/2,(scene.minimum.z+scene.maximum.z)/2};
            extent=std::max({scene.maximum.x-scene.minimum.x,scene.maximum.y-scene.minimum.y,scene.maximum.z-scene.minimum.z,10.f});
        } else if(visual.loaded()) {
            f::Vec3 lo{1e30f,1e30f,1e30f},hi{-1e30f,-1e30f,-1e30f};
            for(const auto& m:visual.meshes()) for(const auto& v:m.vertices) {lo.x=std::min(lo.x,v.position.x);lo.y=std::min(lo.y,v.position.y);lo.z=std::min(lo.z,v.position.z);hi.x=std::max(hi.x,v.position.x);hi.y=std::max(hi.y,v.position.y);hi.z=std::max(hi.z,v.position.z);}
            start.target={(lo.x+hi.x)/2,(lo.y+hi.y)/2,(lo.z+hi.z)/2};extent=std::max({hi.x-lo.x,hi.y-lo.y,hi.z-lo.z,10.f});
        }
        if(options.focus)start.target=*options.focus;
        if(options.distance>0)extent=options.distance;
        // Original content coordinates are Z-up. This preview choice is independent of map identity.
        start.up={0,0,1};start.position={start.target.x+extent*.65f,start.target.y-extent*.85f,start.target.z+extent*.8f};
        f::FreeCamera freeCamera(start);auto end=start;end.position.x-=extent*.9f;
        auto cut=end;cut.position.y+=extent*.8f;
        f::CameraTimeline timeline({{0,start,false},{3,end,false},{5,cut,true},{8,start,false}});
        bool useTimeline=options.timeline;
        if(useTimeline) timeline.play();
        std::unique_ptr<f::audio::RuntimeSessionAudioV1> runtimeAudio;
        if(options.runtimeAudio) {
            try {
                std::vector<std::uint8_t> soundData;
                if(options.audioTable.empty())soundData=f::read_content(assets,"data/pydata/sounds_pyarray.bin");
                else {
                    std::ifstream input(options.audioTable,std::ios::binary);
                    if(!input)throw std::runtime_error("Cannot read original audio table: "+options.audioTable.string());
                    soundData={std::istreambuf_iterator<char>(input),std::istreambuf_iterator<char>()};
                }
                auto next=std::make_unique<f::audio::RuntimeSessionAudioV1>(std::cout);
                const auto audioRoot=options.audioAssets.empty()?assets.root():options.audioAssets;
                // The original Level constructor's normal listener is row1;
                // an explicit index remains a diagnostic source-data override.
                if(!next->start(audioRoot.string(),soundData,options.audioListener<0?1:options.audioListener,
                                combatSession,window.focused(),window.minimized(),error,assets.root().string()))
                    throw std::runtime_error(error);
                runtimeAudio=std::move(next);
                // Original LevelConfig music (Level::Update -> PlayMusic); the scene's own name.
                f::audio::LevelMusicNamesV1 levelMusic;
                if(!f::audio::read_level_music_names_v1(assets,options.level.generic_string(),levelMusic,error)||
                   !runtimeAudio->set_level_music(levelMusic.music,error))
                    std::cerr<<"Level music diagnostic: "<<error<<'\n';
            } catch(const std::exception& failure) {
                std::cerr<<"Audio initialization diagnostic: "<<failure.what()<<"; gameplay continues\n";
            }
        }
        std::set<int> held;int drawn=0;double previous=window.seconds(),dt=0;
        const auto bindSourcePresentations=[&]() {
            if(runtimeAudio)runtimeAudio->clear_step_entry_presentation_observers();
            bindSourceEffects();
            if(sourceEffectsFactory&&!skillAnimationTables.sequences.empty()) {
                sourceSwingFx=std::make_shared<f::effects::RuntimeSwingFxObserverV1>(*combatSession,skillAnimationTables,locomotionItems,sourceEffectsTables.borrow(),sourceEffectsFactory->manager(),
                    [&](const auto& diagnostic) {
                        if(diagnostic.source_sets.empty()&&diagnostic.detail.empty())return;
                        std::cout<<"Source player step FX frame="<<drawn<<" actor="<<diagnostic.actor<<" sequence="<<diagnostic.sequence_id<<" step="<<diagnostic.step<<" occurrence="<<diagnostic.occurrence<<" dispatched="<<diagnostic.dispatched<<" sets=";
                        for(const auto id:diagnostic.source_sets)std::cout<<id<<',';
                        std::cout<<" diagnostic="<<diagnostic.detail<<'\n';
                    },[&](f::ActorId id,std::uint8_t& enabled,std::string& e) {
                        if(!combatSession||!combatSession->actor(id)){e="Step FX current actor is unavailable";return false;}
                        if(id==combatSession->player_id()){enabled=1;e.clear();return true;}
                        const auto placed=std::find_if(population.actors().begin(),population.actors().end(),[&](const auto& value){return value.definition.stableId==id;});
                        if(placed==population.actors().end()){e="Step FX enabled source actor is unavailable";return false;}
                        enabled=std::uint8_t(placed->enabled);e.clear();return true;
                    });
                std::weak_ptr<f::effects::RuntimeSwingFxObserverV1> weak=sourceSwingFx;
                f::CombatSessionStepObserver observer=[weak,&combatSession](const auto& entry) {
                    if(!combatSession||entry.actor!=combatSession->player_id())return;
                    if(const auto owner=weak.lock())owner->step_entry_observer()(entry);
                };
                if(runtimeAudio) {
                    if(!runtimeAudio->add_step_entry_presentation_observer(std::move(observer),error)||!runtimeAudio->bind(combatSession,error))throw std::runtime_error("Source step presentation: "+error);
                } else combatSession->set_step_entry_observer(std::move(observer));
                std::cout<<"Source player step FX observer bound audioFirst="<<bool(runtimeAudio)<<'\n';
            }
        };
        // Motion services can run during selection before the next update.
        // Keep their input and dt providers alive across frame boundaries.
        f::InputActions gameplayInput;
        std::function<void()> sourcePhysicalPlayerControls;
        f::character_menu::Presenter characterMenu;
        // P16 MAP: visited-room tracker for the current level, and the Map page zoom state (reset = full level).
        f::map_visit::RoomZoneVisitTrackerV1 mapVisits;bool mapVisitsReady=false;f::map_visit::MapViewV1 mapView;
        // P16 MAP parchment texture (sheet fill, menus/map_bottom.tga), uploaded on the first Map frame.
        std::uint32_t mapParchmentTexture=0;float mapParchmentTexelsW=1,mapParchmentTexelsH=1;
        f::character_menu::Bindings characterMenuBindings;
        characterMenuBindings.character=&state;
        auto characterMenuComposition=std::make_unique<f::character_menu::SourceCompositionV1>(sharedCharacter);
        std::shared_ptr<f::generic_skills::RuntimeSkillsMenuV1> runtimeSkillsMenu;
        f::generic_skills::RuntimeSkillsTextProviderV1 runtimeSkillsText;
        std::unique_ptr<f::equipment_menu::RuntimeEquipmentBindingV1> runtimeEquipment;
        std::shared_ptr<f::equipment_menu::RuntimeEquipmentPageV1> runtimeEquipmentPage;
        f::equipment_menu::RuntimeEquipmentTextProviderV1 runtimeEquipmentText;
        // P14 EQUIP: Details Transmute amount (source ValueBox value). Drop publishes through equipment_menu::drop_item_to_world,
        // which reports "no world store bound" until the DROPS stream's world-item store is merged.
        std::function<bool(const f::InventoryItem&,std::int32_t&,std::string&)> equipmentTransmuteAmount;
        f::effects::EffectTextureServices equipmentTextureServices;
        equipmentTextureServices.upload=[&](const f::TextureImage& image,std::uint32_t& id,std::string& e){id=renderer.createTexture(image.width,image.height,image.rgba.data());if(!id){e="Equipment original texture upload failed";return false;}e.clear();return true;};
        equipmentTextureServices.release=[&](std::uint32_t id){renderer.destroyTexture(id);};
        f::SourceEquipmentOriginalBindingsV1 equipmentRenderBindings(assets,std::move(equipmentTextureServices),{});
        const f::EquipmentAttachmentSet* runtimeEquipmentAttachments=nullptr;
        std::weak_ptr<const void> equipmentAttemptLease;
        bool menuUsedSkillPoint=false;
        // Preview 14/15: Stats +/- staged spends of one menu visit (features/character_menu/stat_training_v1.hpp); cleared at each open.
        auto statTrainingVisit=std::make_shared<f::character_menu::StatTrainingVisitV1>();
        bool equipmentRebindRequested=false; // P14 DROPS: a pickup added a definition the equipment text policy must list
        const auto bindEquipmentPage=[&]() {
            if(!combatSession||!menuSourceOwner.valid())return;
            std::string equipmentError;
            if(equipmentRebindRequested){equipmentRebindRequested=false;equipmentAttemptLease={};}
            else if(runtimeEquipment&&runtimeEquipment->ready(equipmentError))return;
            const auto currentLease=combatSession->actor_binding_lease();
            if(!currentLease.expired()&&!equipmentAttemptLease.expired()&&
               !currentLease.owner_before(equipmentAttemptLease)&&!equipmentAttemptLease.owner_before(currentLease))return;
            equipmentAttemptLease=currentLease;
            if(characterMenu.tab()==f::character_menu::Tab::equipment)
                characterMenuComposition->select(characterMenu,f::character_menu::Tab::stats,equipmentError);
            f::character_menu::SourcePageProviderV1 unavailable;
            unavailable.owner=sharedCharacter;
            unavailable.ready=[](std::string& e){e="Equipment provider is awaiting the current Session binding";return false;};
            unavailable.append=[](f::character_menu::Frame&,std::string& e){e="Equipment provider is unavailable";return false;};
            unavailable.release=[](float,float,std::string& e){e="Equipment provider is unavailable";return false;};
            if(!characterMenuComposition->register_page(f::character_menu::Tab::equipment,std::move(unavailable),equipmentError))throw std::runtime_error(equipmentError);
            if(runtimeEquipmentPage)runtimeEquipmentPage->leave_page();
            runtimeEquipmentAttachments=nullptr;runtimeEquipmentPage.reset();runtimeEquipment.reset();
            runtimeEquipmentText=f::equipment_menu::RuntimeEquipmentTextProviderV1{};
            f::equipment_menu::RuntimeEquipmentOptionsV1 config;
            config.source_appearance_debug.load=[&](std::string& e){if(!sourceScopes){e="Equipment appearance requires the actual source Debug owner";return false;}return sourceScopes->debug_load(e);};
            config.source_appearance_debug.query=[&](const char* key,std::string& e){bool value=false;if(!sourceScopes){e="Equipment appearance requires the actual source Debug owner";return false;}return sourceScopes->debug_switch(key,value,e);};
            for(unsigned slot=0;slot<config.slots.size();++slot)config.slots[slot]="slot-"+std::to_string(slot);
            for(const auto& equipped:state.equipment)if(equipped.equipment_set<=0) {
                if(equipped.source_slot>=0&&equipped.source_slot<9)config.slots[std::size_t(equipped.source_slot)]=equipped.slot;
                else if(equipped.slot=="main")config.slots[1]=equipped.slot;
                else if(equipped.slot=="off")config.slots[2]=equipped.slot;
            }
            config.table_root=fs::relative(f::resolve_content_path(assets,"data/pydata/loot_table_pyarray.bin").parent_path(),assets.root()).generic_string();
            f::equipment_menu::RuntimeEquipmentBareDefinitionPolicyV1 policy;
            std::set<std::string> heldDefinitions;
            for(const auto& item:state.inventory)heldDefinitions.insert(item.definition_id);
            policy.supported_unpowered_definition_ids.assign(heldDefinitions.begin(),heldDefinitions.end());
            f::equipment_menu::RuntimeEquipmentPageBindingsV1 pageBindings;
            pageBindings.inventory_text.equipment_slots=config.slots;
            if(!runtimeEquipmentText.bind(menuSourceOwner.source().loot.items(),properties.characters,menuLocalization,state,std::move(policy),config,pageBindings,equipmentError)) {
                std::cerr<<"Equipment text diagnostic: "<<equipmentError<<'\n';return;
            }
            const auto designBytes=assets.read("original-cache/data/pydata/design_pycst.bin");
            const auto destroyDesign=[](dh2_script_constants* value){if(value)dh2_script_constants_destroy(value);};
            std::unique_ptr<dh2_script_constants,decltype(destroyDesign)> design(dh2_script_constants_create(),destroyDesign);
            dh2_script_constants_reload designReload{};std::int32_t transmuteMultiplier{};
            if(!design||dh2_script_constants_load(design.get(),designBytes.data(),std::uint32_t(designBytes.size()),&designReload)!=0||designReload.consumed!=designBytes.size()||
               dh2_script_constants_get(design.get(),"CharacterDesign","TransmuteMultiplier",&transmuteMultiplier)!=0)
                throw std::runtime_error("Equipment source TransmuteMultiplier is unavailable");
            // One source packet (ItemInstance value x property 197 x TransmuteMultiplier) feeds both the Details ValueBox text and the
            // Transmute action (Character::INV_TransmuteItem), so the displayed amount is exactly what is paid.
            const auto transmutePacket=[&,transmuteMultiplier](const f::InventoryItem& item,f::equipment_menu::RuntimeEquipmentTransmuteValuePacketV1& packet,std::string& e) {
                if(!combatSession||!combatSession->world()){e="Equipment ValueBox requires current Session";return false;}
                const auto* player=combatSession->actor(combatSession->player_id());
                const auto* source=combatSession->world()->combat_properties(combatSession->player_id());
                if(!player||!source||!player->persistent_character_id||*player->persistent_character_id!=state.id){e="Equipment ValueBox player/profile identity differs";return false;}
                return runtimeEquipmentText.transmute_value(item,source->sheets.resolved[197],transmuteMultiplier,packet,e);
            };
            pageBindings.details_text.transmute_value=[transmutePacket](const f::InventoryItem& item,std::string& value,std::string& e) {
                f::equipment_menu::RuntimeEquipmentTransmuteValuePacketV1 packet;
                if(!transmutePacket(item,packet,e))return false;
                value=packet.formatted_value;e.clear();return true;
            };
            equipmentTransmuteAmount=[transmutePacket](const f::InventoryItem& item,std::int32_t& amount,std::string& e) {
                f::equipment_menu::RuntimeEquipmentTransmuteValuePacketV1 packet;
                if(!transmutePacket(item,packet,e))return false;
                amount=packet.transmute_value;e.clear();return true;
            };
            auto binding=std::make_unique<f::equipment_menu::RuntimeEquipmentBindingV1>();
            if(!binding->bind(*combatSession,state,assets,assets,assets,properties,std::move(config),equipmentError)) {
                std::cerr<<"Equipment binding diagnostic: "<<equipmentError<<'\n';return;
            }
            auto page=std::make_shared<f::equipment_menu::RuntimeEquipmentPageV1>();
            if(!page->bind(*binding,state,std::move(pageBindings),equipmentError))throw std::runtime_error(equipmentError);
            f::character_menu::SourcePageProviderV1 provider;
            if(!page->source_page_provider(sharedCharacter,provider,equipmentError)||
               !characterMenuComposition->register_page(f::character_menu::Tab::equipment,std::move(provider),equipmentError))throw std::runtime_error(equipmentError);
            f::equipment_menu::RuntimeEquipmentRenderChangeV1 initialRender;
            if(binding->take_render_change(initialRender,equipmentError)) {
                if(initialRender.actor_id!=combatSession->player_id()||initialRender.same_session_visual!=combatSession->retained_actor_visual_borrow(initialRender.actor_id)||!initialRender.attachments)
                    throw std::runtime_error("Initial equipment render receipt lost the current Session visual");
                runtimeEquipmentAttachments=initialRender.attachments;
            } else if(!equipmentError.empty())throw std::runtime_error(equipmentError);
            runtimeEquipment=std::move(binding);runtimeEquipmentPage=std::move(page);
        };
        const auto retireEquipmentPage=[&]() {
            // The provider borrows the actor/Scene through this binding. Retire
            // it while the old Session is alive, before any actor replacement.
            if(runtimeEquipmentPage)runtimeEquipmentPage->leave_page();
            f::character_menu::SourcePageProviderV1 unavailable;
            unavailable.owner=sharedCharacter;
            unavailable.ready=[](std::string& e){e="Equipment provider awaits the replacement Session";return false;};
            unavailable.append=[](f::character_menu::Frame&,std::string& e){e="Equipment provider awaits the replacement Session";return false;};
            unavailable.release=[](float,float,std::string& e){e="Equipment provider awaits the replacement Session";return false;};
            if(!characterMenuComposition->register_page(f::character_menu::Tab::equipment,std::move(unavailable),error))throw std::runtime_error(error);
            runtimeEquipmentAttachments=nullptr;runtimeEquipmentPage.reset();runtimeEquipment.reset();equipmentAttemptLease.reset();
            characterMenuBindings.actor=nullptr;characterMenuBindings.properties=nullptr;
        };
        // P14 FAERY: set below, after updatePcHud is defined; the Faery page calls it after a selection change.
        std::function<void()> refreshPcHudForFaery;
        if(options.hud&&combatSession) {
            if(!menuSourceOwner.valid()) {
                std::string sourceError;
                if(!f::frontend::creation::load_runtime_creation_source_v1(assets,creationRandom,menuSourceOwner,sourceError))
                    std::cerr<<"Skills source diagnostic: "<<sourceError<<'\n';
            }
            if(menuSourceOwner.valid()) {
                const auto skillTables=menuSourceOwner.skill_owner->borrow();
                f::generic_skills::CharacterDesignSkillCapsV1 skillCaps;
                skillCaps.known=menuSourceOwner.character_design_caps_known;
                skillCaps.max_skill_level=menuSourceOwner.character_design_skill_caps;
                skillCaps.unlocked_difficulty=std::size_t(f::character_unlocked_difficulty(state)); // P14 schema: profile field (Normal for legacy saves).
                if(!frontendStarted&&(!state.source_skill_slots_known||directFirstSkillGrantPending)) {
                    const auto* current=combatSession->world()->combat_properties(combatSession->player_id());
                    if(!current||!skillCaps.known)throw std::runtime_error("Direct Skills bootstrap requires current source SkillTree and CharacterDesign caps");
                    if(!state.source_skill_slots_known&&!f::frontend::creation::initialize_source_skill_rows_v1(skillTables,current->sheets.resolved[28],state,error))
                        throw std::runtime_error("Direct Skills source rows: "+error);
                    bool grant=false;
                    // Preview 14: only a fresh direct character receives the starter row-0 grant.
                    if(directFirstSkillGrantPending&&!f::generic_skills::probe_skill_training_in_session_v1(state,*combatSession,properties,skillTables,skillCaps,0,grant,error))
                        throw std::runtime_error("Direct Skills source first grant: "+error);
                    if(grant) {
                        f::generic_skills::SessionSkillTrainingCommitV1 commit;
                        if(!f::generic_skills::train_skill_in_session_v1(state,*combatSession,properties,skillTables,skillCaps,0,commit,error))
                            throw std::runtime_error("Direct Skills source first grant: "+error);
                    }
                    directFirstSkillGrantPending=false;
                    std::cout<<"Direct source Skills rows="<<state.skills.size()<<" firstRank="<<state.skills.front().rank<<" points="<<state.source_skill_points<<" sameSession=1\n";
                }
                f::generic_skills::ServicesV1 services;
                services.can_assign=[&,skillTables,skillCaps](const f::CharacterState& character,int id,int position,bool& available,std::string& e) {
                    f::generic_skills::SkillProgressionV1 progress;
                    if(!f::generic_skills::evaluate_skill_progression_v1(character,properties.characters,skillTables,skillCaps,position,progress,e))return false;
                    if(progress.skill_table_id!=id){e="Skills assignment source row changed";return false;}
                    if(!progress.equippable_known){e="Skill assignment requires known saved rank";return false;}
                    available=progress.equippable;e.clear();return true;
                };
                services.train=[&,skillTables,skillCaps](f::CharacterState& character,int list,int position,int id,std::string& e) {
                    f::generic_skills::SkillProgressionV1 progress;
                    if(!f::generic_skills::evaluate_skill_progression_v1(character,properties.characters,skillTables,skillCaps,position,progress,e))return false;
                    if(progress.skill_list_id!=list||progress.skill_table_id!=id){e="Skill training source list/row changed";return false;}
                    if(&character!=sharedCharacter.get()||!combatSession){e="Skill training requires the current same CharacterState/Session";return false;}
                    bool available=false;
                    if(!f::generic_skills::probe_skill_training_in_session_v1(character,*combatSession,properties,skillTables,skillCaps,position,available,e))return false;
                    if(!available){e="Source same-Session skill training probe rejected the current row";return false;}
                    // The authored first skill spend saves before mutation;
                    // subsequent spends in this menu visit skip that prefix.
                    if(!menuUsedSkillPoint&&!f::save_character(options.save,character,e))return false;
                    f::generic_skills::SessionSkillTrainingCommitV1 commit;
                    if(!f::generic_skills::train_skill_in_session_v1(character,*combatSession,properties,skillTables,skillCaps,position,commit,e))return false;
                    // Preview 14: the authored prefix above saves only the pre-spend state, so persist the committed spend too (every spend, not only the first in a visit).
                    if(!f::save_character(options.save,character,e))return false;
                    menuUsedSkillPoint=true;
                    std::cout<<"Source skill training row="<<commit.saved_skill_row<<" rank="<<commit.previous_rank<<"->"<<commit.current_rank<<" points="<<commit.remaining_points<<" potionCapacity="<<unsigned(commit.potion_capacity)<<'\n';
                    return true;
                };
                if(!runtimeSkillsText.bind(properties.characters,skillTables,menuLocalization,state,services,error))throw std::runtime_error("Skills text: "+error);
                if(!dh2::data::load_property_rules(properties.characters,menuSkillPropertyRules,error))throw std::runtime_error("Skills source property rules: "+error);
                f::generic_skills::RuntimeSkillsDetailsBindingsV1 detailsBindings;
                detailsBindings.classes=&properties.classes;detailsBindings.property_rules=&menuSkillPropertyRules;
                detailsBindings.current_resolved=[&](const f::CharacterState& character,dh2::data::PropertySheet& out,std::string& e) {
                    if(&character!=sharedCharacter.get()||!combatSession||!combatSession->world()) {e="Skill details require the current same CharacterState/Session";return false;}
                    const auto* current=combatSession->world()->combat_properties(combatSession->player_id());
                    if(!current){e="Skill details source property sheet is unavailable";return false;}
                    out=current->sheets.resolved;e.clear();return true;
                };
                detailsBindings.max_skill_level=[&,skillCaps](const f::CharacterState& character,std::int32_t& out,std::string& e) {
                    if(&character!=sharedCharacter.get()||!skillCaps.known||skillCaps.unlocked_difficulty>=skillCaps.max_skill_level.size()) {e="Skill details source difficulty cap is unknown";return false;}
                    out=static_cast<std::int32_t>(skillCaps.max_skill_level[skillCaps.unlocked_difficulty]);e.clear();return true;
                };
                detailsBindings.can_increment=[&,skillTables,skillCaps](const f::CharacterState& character,int position,bool& out,std::string& e) {
                    f::generic_skills::SkillProgressionV1 progression;
                    if(&character!=sharedCharacter.get()||!f::generic_skills::evaluate_skill_progression_v1(character,properties.characters,skillTables,skillCaps,position,progression,e))return false;
                    if(!progression.can_increment_known){e="Skill details source rank/progression is unknown";return false;}
                    out=progression.can_increment;e.clear();return true;
                };
                if(!runtimeSkillsText.bind_details(detailsBindings,services,error))throw std::runtime_error("Skills source details: "+error);
                runtimeSkillsMenu=std::make_shared<f::generic_skills::RuntimeSkillsMenuV1>(
                    sharedCharacter,sharedCharacter,properties.characters,skillTables,std::move(services),
                    f::generic_skills::RuntimeSkillsRendererV1{},
                    [](const f::CharacterState& character,std::optional<unsigned>& active,std::string& e) {
                        // This runtime currently loads equipment set0. Legacy
                        // saves with unknown source slot maps stay unknown.
                        if(character.source_skill_slots_known)active=0;else active.reset();
                        e.clear();return true;
                    },f::generic_skills::RuntimeSkillsReleaseV1{},
                    [&menuLocalization,&state](std::string_view symbol,std::string& value,std::string& e) {
                        return menuLocalization.symbol(std::string(symbol),&state,value,e);
                    },[&,skillTables,skillCaps](const f::CharacterState& character,int list,int position,int id,bool& available,std::string& e) {
                        f::generic_skills::SkillProgressionV1 progression;
                        if(&character!=sharedCharacter.get()){e="Skill training probe reached a different CharacterState owner";return false;}
                        if(!f::generic_skills::evaluate_skill_progression_v1(character,properties.characters,skillTables,skillCaps,position,progression,e))return false;
                        if(progression.skill_list_id!=list||progression.skill_table_id!=id){e="Skill training probe source list/row changed";return false;}
                        if(!combatSession){e="Skill training probe requires the current Session";return false;}
                        return f::generic_skills::probe_skill_training_in_session_v1(character,*combatSession,properties,skillTables,skillCaps,position,available,e);
                    });
                if(!characterMenuComposition->register_page(f::character_menu::Tab::skills,runtimeSkillsMenu->source_page_provider(),error))
                    throw std::runtime_error("Skills composition: "+error);
                // P16 MAP: the Map page is selectable on every level. Its content (visited geometry, player marker)
                // is drawn by the host (drawMapPage); this provider only binds selection to the same character owner.
                {
                    f::character_menu::SourcePageProviderV1 mapProvider;
                    mapProvider.owner=sharedCharacter;
                    mapProvider.ready=[](std::string&){return true;};
                    mapProvider.append=[](f::character_menu::Frame&,std::string&){return true;};
                    mapProvider.release=[](float,float,std::string&){return true;};
                    if(!characterMenuComposition->register_page(f::character_menu::Tab::map,std::move(mapProvider),error))
                        throw std::runtime_error("Map composition: "+error);
                }
                // Preview 14: live Stats-tab +/- route (was unregistered: "no original source hit resolver").
            if(!f::character_menu::register_stat_training_v1(*characterMenuComposition,sharedCharacter,state,
                   [&]()->f::CombatSession*{return combatSession.get();},properties,
                   [&](const f::CharacterState& c,std::string& e){return f::save_character(options.save,c,e);},
                   statTrainingVisit,error))throw std::runtime_error("Stats training: "+error);
                // P14 FAERY: Faery tab over the live CharacterState (features/faery_menu/character_state_faery_v1.*).
                // An unbindable page (e.g. legacy state without source Faery rows) stays unregistered and is diagnosed.
                {
                    // A loaded save reaches here without the creation-source owner, so the Faery tables are loaded here too.
                    // Tables are immutable content, loaded even for legacy saves whose Faery rows are unknown (page shows them as unknown).
                    std::string faeryError;
                    if(!sourceFaeryTables) {
                        const auto records=f::read_content(assets,"data/pydata/faeries_pyarray.bin"),names=f::read_content(assets,"data/pydata/faeries_pyarraynames.bin"),fields=f::read_content(assets,"data/pydata/faeries_pystructnames.bin");
                        if(!sourceFaeryOwner.load({records.data(),records.size()},{names.data(),names.size()},{fields.data(),fields.size()},faeryError))std::cerr<<"Faery tables diagnostic: "<<faeryError<<'\n';
                        else sourceFaeryTables=sourceFaeryOwner.borrow();
                    }
                    f::faery_menu::CharacterStateFaeryPageHostV1 faeryHost;
                    faeryHost.owner=sharedCharacter;faeryHost.tables=sourceFaeryTables;
                    faeryHost.localize=[&](const std::string& symbol,std::string& value,std::string& e){return menuLocalization.symbol(symbol,&state,value,e);};
                    faeryHost.persist=[&](std::string& e){return f::save_character(options.save,state,e);};
                    faeryHost.refresh_hud=[&](){
                        std::cout<<"Faery selection committed current="<<state.faery_by_difficulty[std::size_t(f::faery_menu::active_faery_difficulty_v1())].current_faery<<" HUD key-4 refresh\n";
                        if(refreshPcHudForFaery)refreshPcHudForFaery();};
                    if(!f::faery_menu::register_character_state_faery_page_v1(*characterMenuComposition,std::move(faeryHost),faeryError))
                        std::cerr<<"Faery page diagnostic: "<<faeryError<<'\n';
                }
                if(!characterMenuComposition->install_content(characterMenuBindings,error))throw std::runtime_error(error);
            }
        }
        f::loot::RuntimeSessionDeathRewardsV1 deathRewards;
        std::shared_ptr<f::loot::RuntimeWorldItemAdapterV1> worldItems;
        // P14 DROPS: presentation + pickup state for items lying in the world.
        std::unique_ptr<f::interactions::WorldDropRuntimeV1> worldDrops;
        f::loot::RuntimeWorldItemIdV1 worldItemTarget=f::loot::invalid_runtime_world_item_v1;
        double worldItemFractionMs=0;
        std::string worldItemStatus;std::uint32_t worldItemStatusRgb=0xFFFFFF;int worldItemStatusFrames=0;
        dh2::ui::HudTextV1* dropHudText=nullptr;dh2::ui::HudTextEnvironmentV1 dropTextEnvironment;
        std::unique_ptr<dh2::ui::ItemTextOwnerV5> dropTextOwner;
        std::map<std::string,std::string> dropNameCache;
        auto rewardContext=std::make_shared<GameplayRewardContext>();
        std::uint64_t rewardBindingGeneration{};
        rewardContext->debug=[&](const char* key,bool& value,std::string& e) {
            if(!sourceScopes){e="Source gameplay Debug settings are unavailable";return false;}
            return sourceScopes->debug_switch(key,value,e);
        };
        rewardContext->character=[&](f::ActorId id,f::loot::RuntimeDeathActorV1& out,std::string& e) {
            if(!combatSession||!combatSession->actor(id)){e="Reward actor is absent from the current Session";return false;}
            out={};out.binding_lifecycle=rewardBindingGeneration;
            if(id==combatSession->player_id())out.character=sharedCharacter;
            e.clear();return true;
        };
        // P16 CONTAINERS2 (T4): DoOpen loot through the same DROPS store and session loot RNG the death rewards use.
        const auto bindContainerLoot=[&]() {
            containerLoot=f::containers::ContainerLootV1();
            if(!combatSession||!deathRewards.bound()||!worldItems)return;
            f::containers::ContainerLootInputsV1 lootInputs;
            lootInputs.tables=deathRewards.loot_source().loot;
            lootInputs.powers=deathRewards.loot_source().power_resources;
            lootInputs.entry=deathRewards.loot_entry_services();
            lootInputs.store=worldItems.get();
            lootInputs.rng_context=combatSession->world();
            lootInputs.with_rng=[](void* raw,const f::interactions::SourceContainerLootRngOperationV1& operation,std::string& e){return static_cast<f::PlayableActorWorld*>(raw)->with_loot_random(operation,e);};
            lootInputs.bonus_context=combatSession->world();
            lootInputs.opener_bonus256=[](void* raw,f::ActorId id,std::int32_t& bonus,std::string& e){
                const auto* properties=static_cast<f::PlayableActorWorld*>(raw)->combat_properties(id);
                if(!properties){e="Opener has no source properties";return false;}
                bonus=properties->sheets.resolved[195];return true;};
            std::string lootError;
            if(!containerLoot.bind(lootInputs,lootError))throw std::runtime_error("Container loot: "+lootError);
            std::cout<<"Container loot bound\n";
        };
        const auto bindDeathRewards=[&]() {
            deathRewards.reset();
            if(!combatSession||!menuSourceOwner.valid())return;
            // P14 schema: CurrentDifficulty/UnlockedDiff come from the shared profile (single accessor), not a fixed Normal.
            rewardContext->currentDifficulty=f::character_current_difficulty(state);rewardContext->unlockedDifficulty=f::character_unlocked_difficulty(state);
            if(!worldItems)worldItems=std::make_shared<f::loot::RuntimeWorldItemAdapterV1>(menuSourceOwner.loot_owner->borrow());
            // P14 DROPS: ground items belong to the session being bound. A reload
            // (R/F5-F9) or new session clears them; nothing is granted or duplicated.
            if(worldItems){worldItems->clear();worldItemTarget=f::loot::invalid_runtime_world_item_v1;}
            ++rewardBindingGeneration;
            f::loot::RuntimeSessionDeathRewardBindingsV1 rewardBindings;
            rewardBindings.gameplay_context_lease=rewardContext;rewardBindings.context=rewardContext.get();
            rewardBindings.read_reward_admission=&GameplayRewardContext::admission;
            rewardBindings.read_difficulty=&GameplayRewardContext::difficulty;
            rewardBindings.source_loot_entry=&GameplayRewardContext::lootEntry;
            rewardBindings.query_debug_switch=&GameplayRewardContext::debugQuery;
            rewardBindings.resolve_character=&GameplayRewardContext::resolve;
            // P15 LEVELUP (I026): Character::LevelUp presentation (FX set 135 on the
            // hero; MENU_LEVEL_UP text logged only). An FX failure does not abort the award.
            rewardBindings.level_up_presentation=[&](f::ActorId id,std::int32_t level,std::string& e) {
                f::effects::RuntimeLevelUpPresentationResultV1 result;
                const bool ok=f::effects::play_level_up_presentation_v1(sourceEffectsTables.borrow(),
                    [&](std::int32_t set,const float* position,const float* rotation,std::uintptr_t anchor,std::uintptr_t* created,std::string& fxError) {
                        if(!sourceEffectsFactory){fxError="no effects factory";return false;}
                        return sourceEffectsFactory->manager().play_set(set,position,rotation,anchor,created,fxError);
                    },id,result,e);
                std::cout<<"Level up presentation frame="<<drawn<<" actor="<<id<<" level="<<level<<" "<<result.fx<<" "<<result.text<<'\n';
                return ok;
            };
            if(!deathRewards.bind(*combatSession,menuSourceOwner,assets,worldItems,std::move(rewardBindings),error))
                throw std::runtime_error("Source death rewards: "+error);
            // P15 B048: item drop/pickup cues. The ordinal comes from the item's own ItemAudioVisualTable row
            // (drop row+4, pickup row+8). A selected WAV that is absent stays silent and is logged, never substituted.
            worldItems->set_sound_observer([&runtimeAudio,audiovisual=deathRewards.loot_source().audiovisual](
                    f::loot::WorldItemSoundEventV1 event,const f::loot::RuntimeWorldItemEntryV1& entry) {
                const char* eventName=event==f::loot::WorldItemSoundEventV1::drop?"drop":"pickup";
                std::int32_t ordinal=-1;std::string soundError;
                if(!f::loot::world_item_sound_ordinal_v1(audiovisual,entry.visual_row,event,ordinal,soundError)) {
                    std::cout<<"World item sound item="<<entry.identity<<" event="<<eventName<<" status=silent detail="<<soundError<<'\n';
                    return;
                }
                if(!runtimeAudio) {
                    std::cout<<"World item sound item="<<entry.identity<<" event="<<eventName<<" source="<<ordinal<<" status=no_audio_runtime\n";
                    return;
                }
                f::audio::WorldItemSoundResultV1 result;
                if(!runtimeAudio->submit_world_item_sound(ordinal,entry.source_position,result,soundError)) {
                    std::cout<<"World item sound item="<<entry.identity<<" event="<<eventName<<" source="<<ordinal<<" status=failed detail="<<soundError<<'\n';
                    return;
                }
                if(result.status==f::audio::WorldItemSoundStatusV1::asset_missing) {
                    std::cout<<"World item sound uid="<<result.uid<<" event="<<eventName<<" source="<<ordinal<<" item="<<entry.identity<<" status=asset_missing\n";
                    std::cout<<"cue uid="<<result.uid<<" asset missing: silent ("<<result.uri<<")\n";
                    return;
                }
                std::cout<<"World item sound uid="<<result.uid<<" event="<<eventName<<" source="<<ordinal<<" item="<<entry.identity<<" status=submitted\n";
            });
        };
        // P16 QUESTS: the authored v2Quest table is decoded once; the runtime is bound per live session.
        // Quest state lives in CharacterState::source_quest_progress_cqpg (saved with the character).
        std::shared_ptr<const f::quest_runtime::QuestTableV1> questTable;
        std::map<f::ActorId,std::pair<std::int32_t,std::int32_t>> questActorIdentity; // (CharacterTable row, Charater_Templates row)
        std::unique_ptr<f::quest_runtime::QuestRuntimeV1> questRuntime;
        f::quest_runtime::QuestZoneSetV1 questZones; // P16 QUESTUI: MoveInZone boxes of this level
        f::QuestBannerPresenterV1 questBanners;      // P16 QUESTUI: NEW QUEST / updates / QUEST COMPLETED
        std::set<std::int32_t> questTalkOids;        // P16 QUESTUI: TalkToNPC oid1 values (CharacterTable rows)
        bool questTalkHeld=false;                    // P16 QUESTUI: interact press edge for NPC talk
        std::shared_ptr<f::CharacterQuestProgressV1> questMenuProgress; // P16 QUESTUI: Quest Log page progress (CQPG view)
        std::shared_ptr<f::RuntimeQuestMenuV1> questMenu;
        std::shared_ptr<f::RuntimeQuestCharacterMenuBindingV1> questMenuBinding;
        std::shared_ptr<dh2::ui::HudTextEnvironmentV1> questTextEnvironment; // P16 QUESTUI: outlives the Quest Log text resolver
        // P16 QUESTUI: every runtime banner is queued for the presenter and returned for the console line.
        const auto takeQuestBanners=[&]() {
            auto banners=questRuntime?questRuntime->take_banners():std::vector<f::quest_runtime::QuestBannerV1>{};
            for(const auto& banner:banners)questBanners.push(banner);
            return banners;
        };
        std::int32_t questLevelRow=-1;
        const auto bindQuestRuntime=[&]() {
            f::quest_runtime::bind_quest_event_sink({});
            questRuntime.reset();questActorIdentity.clear();questZones=f::quest_runtime::QuestZoneSetV1();
            if(!combatSession||!menuSourceOwner.valid())return;
            if(!questTable) {
                std::string questError;
                const auto questArray=assets.read("original-cache/data/pydata/v2quests_pyarray.bin");
                const auto questNames=assets.read("original-cache/data/pydata/v2quests_pyarraynames.bin");
                if(!f::quest_runtime::decode_quest_table_v1(questArray,questNames,questTable,questError)) {
                    std::cerr<<"Quest table diagnostic: "<<questError<<'\n';return;
                }
            }
            f::quest_runtime::QuestRuntimeServicesV1 questServices;
            questServices.give_experience=[&](std::int32_t amount,std::string& e) {
                if(!deathRewards.bound()||!combatSession){e="Quest XP owner is not bound";return false;}
                return deathRewards.award_experience(combatSession->player_id(),amount,e);
            };
            questServices.current_level_row=[&]()->std::int32_t {
                const auto* levels=loadMetadataLevels(assets);
                return levels?f::menu_metadata::find_level_row(*levels,options.level.generic_string()):-1;
            };
            // P16 QUESTUI: authored StringIDs (objective text) through the shared StringManager owner (drop-name owner).
            questServices.text=[&](std::int32_t id,std::string& text) {
                std::string textError;bool isNull=false;
                if(!menuLocalization.bind_profile(&state,textError)||!menuLocalization.borrow_text(dropHudText,dropTextEnvironment,textError)||!dropHudText)return false;
                return dropHudText->integer_string(id,dropTextEnvironment.localization,text,isNull,textError)&&!isNull;
            };
            questRuntime=std::make_unique<f::quest_runtime::QuestRuntimeV1>(state,questTable,std::move(questServices));
            std::string questError;
            if(!questRuntime->load(questError))std::cerr<<"Quest runtime load diagnostic: "<<questError<<'\n';
            // P16 QUESTUI: explicit charpropsname bindings (NPCs) have no population row; resolve it by name
            // through the CharacterTable names (the quest talk oids are these rows).
            std::map<std::string,std::int32_t> characterRows;
            {
                std::string rowError;
                if(!f::quest_runtime::decode_character_row_names_v1(assets.read("original-cache/data/pydata/character_properties_pyarraynames.bin"),characterRows,rowError))
                    std::cerr<<"Quest character rows diagnostic: "<<rowError<<'\n';
            }
            for(const auto& placed:population.actors()) {
                std::int32_t row=placed.source_character_cache;
                const auto charprops=placed.definition.properties.find("charpropsname");
                if(row<0&&charprops!=placed.definition.properties.end()) {
                    const auto found=characterRows.find(charprops->second);
                    if(found!=characterRows.end())row=found->second;
                }
                questActorIdentity[placed.definition.stableId]={row,placed.source_template_cache};
            }
            // P16 QUESTUI: TalkToNPC oids of the table (CharacterTable rows), for NPC talk.
            questTalkOids.clear();
            for(const auto& row:questTable->rows()) {
                if(row.accept.type==5)questTalkOids.insert(row.accept.oid1);
                for(const auto& objective:row.objectives)if(objective.type==5)questTalkOids.insert(objective.oid1);
            }
            // P16 QUESTUI: quest trigger zones from this level's declarations (quest-named zones only; any level).
            {
                std::vector<f::quest_runtime::QuestZoneDeclarationV1> zoneDeclarations;
                for(const auto& declaration:population.definitions()) {
                    if(declaration.name.empty())continue;
                    zoneDeclarations.push_back(f::quest_runtime::make_quest_zone_declaration_v1(declaration.name,declaration.properties,
                        {declaration.placement[12],declaration.placement[13],declaration.placement[14]}));
                }
                questZones.build(*questTable,zoneDeclarations);
                {const auto* levels=loadMetadataLevels(assets);questLevelRow=levels?f::menu_metadata::find_level_row(*levels,options.level.generic_string()):-1;}
                for(const auto& note:questZones.notes())std::cout<<"Quest zone note: "<<note<<'\n';
                std::cout<<"Quest zones built="<<questZones.zones().size();
                for(const auto& zone:questZones.zones())std::cout<<' '<<zone.name<<'['<<zone.min[0]<<','<<zone.min[1]<<','<<zone.min[2]<<'|'<<zone.max[0]<<','<<zone.max[1]<<','<<zone.max[2]<<']';
                std::cout<<" level="<<questLevelRow<<'\n';
            }
            // P16 QUESTUI: Quest Log tab (btnQuestLogTab). The page reads the same CQPG the runtime writes; the
            // source art (menu_QuestLogSheetNEW) and hit routes come from the existing provider.
            if(characterMenuComposition) {
                std::string menuError;
                questMenuProgress=std::make_shared<f::CharacterQuestProgressV1>();
                f::CharacterQuestLogPolicyV1 policy;policy.debug_priority=f::quest_runtime::kQuestPriorityDebugV1;
                f::CharacterQuestTextV1 menuText;
                dh2::ui::HudTextV1* menuHud=nullptr;questTextEnvironment=std::make_shared<dh2::ui::HudTextEnvironmentV1>();
                if(!menuLocalization.bind_profile(&state,menuError)||!menuLocalization.borrow_text(menuHud,*questTextEnvironment,menuError)||!menuHud)
                    std::cerr<<"Quest Log text diagnostic: "<<menuError<<'\n';
                else if(!f::bind_source_quest_text_resolver_v1(*menuHud,*questTextEnvironment,menuText,menuError))
                    std::cerr<<"Quest Log text diagnostic: "<<menuError<<'\n';
                questMenu=std::make_shared<f::RuntimeQuestMenuV1>(state,*questMenuProgress,questTable,policy,menuText);
                questMenuBinding=std::make_shared<f::RuntimeQuestCharacterMenuBindingV1>(questMenu,sharedCharacter,sharedCharacter,
                    []{return true;},
                    [&](const std::string& symbol,std::string& value,std::string& symbolError){return menuLocalization.symbol(symbol,&state,value,symbolError);});
                f::character_menu::SourcePageProviderV1 questProvider;
                if(!questMenuBinding->load_progress_from_character(menuError)||!questMenuBinding->show(0,0,f::CharacterQuestCategoryV1::assigned,menuError)||
                   !f::bind_source_quest_menu_page_provider_v1(questMenuBinding,sharedCharacter,sharedCharacter,questProvider,menuError)||
                   !characterMenuComposition->register_page(f::character_menu::Tab::quest,std::move(questProvider),menuError))
                    std::cerr<<"Quest Log page diagnostic: "<<menuError<<'\n';
            }
            std::cout<<"Quest runtime bound rows="<<questTable->rows().size()<<" actors="<<questActorIdentity.size()
                     <<" current="<<questRuntime->current_quest()<<" cqpg="<<state.source_quest_progress_cqpg.size()<<'\n';
            for(const auto& banner:takeQuestBanners())
                std::cout<<"Quest banner kind="<<questBannerKindName(banner.kind)
                         <<" row="<<banner.row<<" xp="<<banner.reward_xp<<" gold="<<banner.reward_gold<<" (bind)\n";
            f::quest_runtime::bind_quest_event_sink([&](const f::quest_runtime::QuestEvent& event) {
                std::string e;
                const auto applied=questRuntime->handle(event,e);
                if(!e.empty())std::cerr<<"Quest event diagnostic: "<<e<<'\n';
                std::string saveError;
                if(!questRuntime->save(saveError))std::cerr<<"Quest save diagnostic: "<<saveError<<'\n';
                std::cout<<"Quest event kind="<<int(event.kind)<<" property="<<event.property_id<<" template="<<event.template_id
                         <<" applied="<<applied<<'\n';
                for(const auto& banner:takeQuestBanners())
                    std::cout<<"Quest banner kind="<<questBannerKindName(banner.kind)
                             <<" row="<<banner.row<<" objective="<<banner.objective_text_id<<" xp="<<banner.reward_xp
                             <<" gold="<<banner.reward_gold<<" text='"<<banner.text<<"'\n";
            });
        };
        // P16 QUESTS: kills of this update (death events whose attacker is the player) become quest kill events
        // carrying the victim's CharacterTable row (property) and Charater_Templates row (template).
        const auto raiseQuestKills=[&]() {
            if(!questRuntime||!combatSession)return;
            for(const auto& event:combatSession->events()) {
                if(!event.applied||!event.target_died||event.attacker!=combatSession->player_id())continue;
                const auto identity=questActorIdentity.find(event.target);
                if(identity==questActorIdentity.end())continue;
                f::quest_runtime::QuestEvent kill;
                kill.kind=f::quest_runtime::QuestEvent::Kind::kill;
                kill.property_id=identity->second.first;kill.template_id=identity->second.second;
                f::quest_runtime::raise_quest_event(kill);
            }
        };
        // P16 QUESTUI: NPC talk on the interact press edge: the nearest live actor whose CharacterTable row is a
        // TalkToNPC oid, within 200 units (CharacterDesign.OOI_Distance). Approximation of Character::Interact.
        const auto talkNearestNpc=[&]() {
            if(!questRuntime||!combatSession||questTalkOids.empty())return;
            const auto* player=combatSession->actor(combatSession->player_id());
            if(!player)return;
            bool found=false;float bestDistance=200.f;std::int32_t bestRow=-1;float nearestAny=-1.f;std::int32_t nearestRow=-1;std::array<float,3> nearestAt{};
            // Authored NPCs are placed population actors (static world placement), not combat-session actors.
            for(const auto& placed:population.actors()) {
                const auto identity=questActorIdentity.find(placed.definition.stableId);
                if(identity==questActorIdentity.end()||!questTalkOids.count(identity->second.first))continue;
                const float dx=placed.definition.placement[12]-player->transform.position[0];
                const float dy=placed.definition.placement[13]-player->transform.position[1];
                const float dz=placed.definition.placement[14]-player->transform.position[2];
                const float distance=std::sqrt(dx*dx+dy*dy+dz*dz);
                if(nearestAny<0.f||distance<nearestAny){nearestAny=distance;nearestRow=identity->second.first;nearestAt={placed.definition.placement[12],placed.definition.placement[13],placed.definition.placement[14]};}
                if(distance<=bestDistance){found=true;bestDistance=distance;bestRow=identity->second.first;}
            }
            if(!found) {
                std::cout<<"Quest talk none within 200 nearest_row="<<nearestRow<<" distance="<<nearestAny<<" at="<<nearestAt[0]<<','<<nearestAt[1]<<','<<nearestAt[2]<<" player="<<player->transform.position[0]<<','<<player->transform.position[1]<<','<<player->transform.position[2]<<'\n';
                return;
            }
            f::quest_runtime::QuestEvent talk;
            talk.kind=f::quest_runtime::QuestEvent::Kind::talk_to_npc;
            talk.object_id=bestRow;talk.secondary_id=questLevelRow;
            std::cout<<"Quest talk npc row="<<bestRow<<" distance="<<bestDistance<<" level="<<questLevelRow<<'\n';
            f::quest_runtime::raise_quest_event(talk);
        };
        // P16 QUESTUI: quest zone entries of this frame (player position against the level's quest zones).
        const auto raiseQuestZones=[&]() {
            if(!questRuntime||!combatSession||questZones.zones().empty())return;
            const auto* player=combatSession->actor(combatSession->player_id());
            if(!player)return;
            for(const auto& name:questZones.update({player->transform.position[0],player->transform.position[1],player->transform.position[2]})) {
                f::quest_runtime::QuestEvent entry;
                entry.kind=f::quest_runtime::QuestEvent::Kind::zone_enter;
                entry.zone=name;entry.level_row=questLevelRow;
                std::cout<<"Quest zone entered zone="<<name<<" level="<<questLevelRow<<'\n';
                f::quest_runtime::raise_quest_event(entry);
            }
        };
        bindDeathRewards();
        bindContainerLoot();
        bindQuestRuntime();
        // P14 DROPS: source itemdrops.bdae presentation over the same world-item store.
        const auto bindWorldDrops=[&]() {
            if(worldDrops||!combatSession||!worldItems||!menuSourceOwner.valid()||!deathRewards.bound())return;
            auto drops=std::make_unique<f::interactions::WorldDropRuntimeV1>();
            std::string dropError;
            if(!drops->load(assets,worldItems,deathRewards.loot_source().audiovisual,renderer,dropError)) {
                std::cerr<<"World item presentation diagnostic: "<<dropError<<'\n';return;
            }
            std::cout<<"World item presentation resolved visuals:";
            for(const auto& name:drops->resolved_visuals())std::cout<<' '<<name;
            std::cout<<" unresolved:";
            for(const auto& pair:drops->unresolved_visuals())std::cout<<' '<<pair.first<<'('<<pair.second<<')';
            std::cout<<'\n';
            worldDrops=std::move(drops);
        };
        bindWorldDrops();
        // ItemInstance name text through the same source ItemText owner the inventory uses.
        const auto itemDisplayName=[&](const f::InventoryItem& item,std::string& out,std::string& e)->bool {
            const auto cached=dropNameCache.find(item.definition_id);
            if(cached!=dropNameCache.end()){out=cached->second;e.clear();return true;}
            if(!menuSourceOwner.valid()){e="Item name requires the source loot tables";return false;}
            const auto& itemTable=menuSourceOwner.source().loot.items();
            if(!dropTextOwner) {
                if(!menuLocalization.bind_profile(&state,e)||!menuLocalization.borrow_text(dropHudText,dropTextEnvironment,e)||!dropHudText){if(e.empty())e="Item name text owner unavailable";return false;}
                dropTextOwner=std::make_unique<dh2::ui::ItemTextOwnerV5>(itemTable,properties.characters,*dropHudText,dropTextEnvironment);
            }
            f::inventory::SourceDescriptors descriptors;
            if(!f::inventory::source_bare_item_descriptors(item,itemTable,dropTextOwner->services(),descriptors,e))return false;
            dropNameCache[item.definition_id]=descriptors.name;out=descriptors.name;e.clear();return true;
        };
        sourceEffectsCamera=camera(options.sourceCamera?campaignHost.source_camera_pose(originalCamera.pose()):(useTimeline?timeline.sample():freeCamera.pose()));
        bindSourcePresentations();
        f::platform_input::SemanticInput semanticInput;
        std::uint64_t menuOpened=0,menuDrawn=0;bool mouseHeld=false,escapeClosedMenu=false;
        bool pauseMenuOpen=false,pauseConfirmation=false;
        // Preview 15 B049: the Stats confirmation box (see statConfirmYes/No below). Every menu close path
        // (Back, Escape, profile key, release actions) goes through the guard while points are staged.
        bool statConfirmOpen=false;
        characterMenu.close_guard=[&]() {
            if(!statTrainingVisit->has_staged())return true;
            if(!statConfirmOpen)std::cout<<"Stats confirmation opened staged="<<statTrainingVisit->staged_total()<<'\n';
            statConfirmOpen=true;
            return false;
        };
        const auto pauseHudArt=f::pause_ui::source_pause_ui_frame_v1(f::pause_ui::SourcePauseSurfaceV1::hud_pause_button);
        const auto pausePageArt=f::pause_ui::source_pause_ui_frame_v1(f::pause_ui::SourcePauseSurfaceV1::pause_page);
        const auto pauseConfirmArt=f::pause_ui::source_pause_ui_frame_v1(f::pause_ui::SourcePauseSurfaceV1::confirmation);
        const auto currentPauseArt=[&]() -> const f::pause_ui::SourcePauseUiFrameV1& {return pauseConfirmation?pauseConfirmArt:pausePageArt;};
        f::frontend::FrontendText pauseText;
        f::frontend::FrontendText pcHudText;
        f::generic_skills::PcGameplayHudPresentationV1 pcHudPresentation;
        bool pcHudReady=false;
        std::string pcHudTextSignature;
        int pauseTextSurface=-1,pauseTextWidth=0,pauseTextHeight=0;
        if(options.hud&&combatSession&&!pauseText.load(assets.root()/"original-cache",error))throw std::runtime_error("Pause source font: "+error);
        if(options.hud&&combatSession&&!pcHudText.load(assets.root()/"original-cache",error))throw std::runtime_error("PC HUD font: "+error);
        f::frontend::FrontendText cinematicText; // P16 CINE: caption and SKIP text (same frontend text owner as the PC HUD)
        std::string cinematicTextSignature;
        if(options.campaignTriggers&&combatSession&&!cinematicText.load(assets.root()/"original-cache",error))throw std::runtime_error("Cinematic font: "+error);
        const auto updatePcHud=[&]() {
            pcHudReady=false;
            if(!options.hud||!combatSession||!menuSourceOwner.valid()||!state.source_skill_slots_known)return;
            const auto row=std::find(properties.characters.names.begin(),properties.characters.names.end(),state.class_id);
            unsigned classFrame=0;
            if(row==properties.characters.names.end()||!f::skill_ui::original_class_frame_for_row(properties.characters,int(row-properties.characters.names.begin()),classFrame,error))throw std::runtime_error("PC HUD class: "+error);
            std::array<f::generic_skills::PcSkillHudSourceStatusV1,3> status{};
            for(unsigned i=0;i<status.size();++i)status[i].source_slot=i;
            f::generic_skills::PcSkillHudFrameV1 frame;
            if(!f::generic_skills::project_pc_skill_hud_v1(state,combatSession->player_id(),properties.characters,menuSourceOwner.skill_owner->borrow(),0,status,skillCastCoordinator?skillCastCoordinator->receipt(combatSession->player_id()):nullptr,frame,error))throw std::runtime_error("PC HUD source: "+error);
            // HUDBTN: original bottom action row (Android reference): five rings centred on the 480x320 stage,
            // pitch 56, radius 24, centre y 270. The 1-5 key legends are the PC adaptation, placed under each ring.
            f::generic_skills::PcGameplayHudLayoutV1 layout;
            const auto circle=[](float x,float labelLeft,float labelRight) {return f::generic_skills::PcGameplayHudCirclePlacementV1{x,270,24,{labelLeft,labelRight,298,312}};};
            layout.skills={circle(128,116,140),circle(184,172,196),circle(240,228,252)};layout.faery=circle(296,276,316);layout.potion=circle(352,321,383);
            // HUDBTN: real CoolDown per physical cell. Each cell's skill timer (SetSkillCooldown, per actor/skill row) gives
            // remaining = 1 - elapsed/total; FastUpdate frame = clamp((int)(remaining*100)-1, 0, 99). Faery uses its 5000 ms spell clock.
            if(skillCastCoordinator) for(auto& cell:frame.left_middle_right) if(cell.skill_table_id) {
                const double remaining=skillCastCoordinator->skill_cooldown_remaining_fraction_v1(combatSession->player_id(),*cell.skill_table_id);
                cell.source_cooldown_frame=f::generic_skills::pc_cooldown_frame_from_remaining_v1(remaining);
            }
            {
                const auto spell=faeryCooldownClock.spell_ready_at_ms.find(combatSession->player_id());
                const double remaining=spell==faeryCooldownClock.spell_ready_at_ms.end()?0.0:f::generic_skills::pc_cooldown_remaining_fraction_v1(double(spell->second),double(faeryCooldownClock.elapsed_ms),5000.0);
                layout.faery_cooldown_frame=f::generic_skills::pc_cooldown_frame_from_remaining_v1(remaining);
            }
            // B002/B024: exact NativeHUDGetActiveFaery result = Character::SG_GetCurrentFaerieId(-1), the saved current_faery of difficulty 0 (difficulty used by this build's Faery cast arm).
            if(state.source_faery_state_known)layout.active_faery_id=state.faery_by_difficulty[std::size_t(f::faery_menu::active_faery_difficulty_v1())].current_faery;
            if(!f::generic_skills::compose_pc_gameplay_hud_v1(frame,classFrame,layout,pcHudPresentation,error))throw std::runtime_error("PC HUD geometry: "+error);
            std::uint64_t potionCount=0;
            for(const auto& item:state.inventory)if(item.definition_id=="Potion0")potionCount+=item.quantity;
            for(auto& field:pcHudPresentation.art.text_fields) {
                if(field.path=="pc_hud/key_4"){field.initial_text="4 Faery";field.source_height=10;}
                if(field.path=="pc_hud/key_5"){field.initial_text="5 Potion: "+std::to_string(potionCount);field.source_height=10;}
            }
            pcHudReady=true;
        };
        updatePcHud();
        refreshPcHudForFaery=[&](){updatePcHud();}; // P14 FAERY: key-4 circle re-composed after a page selection
        f::inventory::RuntimeSessionPotionUseV1 potionUse;
        struct PotionDebugContext {
            std::function<bool(std::string&)> load;
            std::function<bool(const char*,bool&,std::string&)> get;
            std::map<std::uintptr_t,std::string> strings;
            std::uintptr_t serial=1;
            std::string error;
        } potionDebug;
        potionDebug.load=[&](std::string& e){if(!sourceScopes){e="Potion source Debug owner is unavailable";return false;}return sourceScopes->debug_load(e);};
        potionDebug.get=[&](const char* key,bool& value,std::string& e){if(!sourceScopes){e="Potion source Debug owner is unavailable";return false;}return sourceScopes->debug_switch(key,value,e);};
        dh2::character::skills::SkillAttackNativeServicesV6 potionDebugServices{&potionDebug,[](void* raw,const auto* request,std::uintptr_t* output)->int {
            if(!raw||!request||!output)return -1;
            auto& context=*static_cast<PotionDebugContext*>(raw);*output=0;
            using namespace dh2::character::skills;
            switch(request->service) {
            case skill_attack_debug_load_v6:return context.load(context.error)?0:-1;
            case skill_attack_string_construct_v6:
                if(!request->name)return -1;
                *output=context.serial++;context.strings.emplace(*output,request->name);return 0;
            case skill_attack_debug_get_v6: {
                const auto found=context.strings.find(request->subject);bool value=false;
                if(found==context.strings.end()||!context.get(found->second.c_str(),value,context.error))return -1;
                *output=std::uintptr_t(value);return 0;
            }
            case skill_attack_string_destroy_v6:return context.strings.erase(request->subject)==1?0:-1;
            default:return -1;
            }
        }};
        // messageSymbol (Preview 15 B049): replaces the WarningBox/confirm_msg text of the confirmation frame.
        const auto drawPauseArt=[&](const f::pause_ui::SourcePauseUiFrameV1& frame,bool labels,const char* messageSymbol) {
            auto art=frame.art;
            const std::string messageSuffix="WarningBox/confirm_msg/text";
            if(labels)for(auto& field:art.text_fields) {
                field.initial_text.clear();
                if(messageSymbol&&field.path.size()>=messageSuffix.size()&&
                   field.path.compare(field.path.size()-messageSuffix.size(),messageSuffix.size(),messageSuffix)==0) {
                    if(!menuLocalization.symbol(std::string(messageSymbol),&state,field.initial_text,error))throw std::runtime_error("Stats confirmation text: "+error);
                    continue;
                }
                for(const auto& binding:f::pause_ui::source_pause_ui_text_bindings_v1()) {
                    const std::string suffix(binding.path_suffix);
                    if(field.path.size()<suffix.size()||field.path.compare(field.path.size()-suffix.size(),suffix.size(),suffix)!=0)continue;
                    if(!menuLocalization.symbol(std::string(binding.localization_symbol),&state,field.initial_text,error))throw std::runtime_error("Pause source label: "+error);
                    break;
                }
            }
            f::HudGeometry geometry;
            if(!f::frontend::art::project(art,window.width(),window.height(),geometry,error))throw std::runtime_error("Pause source geometry: "+error);
            for(std::size_t i=0;i<geometry.batches.size();++i) {
                std::vector<f::OverlayTriangleVertex> vertices;
                for(const auto& v:geometry.batches[i].triangles)vertices.push_back({v.x,v.y,v.u,v.v});
                const auto bitmap=art.bitmap_ids.at(i);
                if(bitmap!=0&&bitmap!=1)throw std::runtime_error("Pause source frame uses an unavailable atlas");
                if(!overlay.drawTriangles(vertices,bitmap?hudTexture:0,art.batch_colors.at(i)))throw std::runtime_error("Pause source art draw rejected");
            }
            if(labels) {
                // A replaced message is a different text key, so the same confirmation surface rebuilds its text.
                const int textKey=int(frame.surface)+(messageSymbol?1000:0);
                if(pauseTextSurface!=textKey||pauseTextWidth!=window.width()||pauseTextHeight!=window.height()) {
                    if(!pauseText.rebuild(art,window.width(),window.height(),renderer,error))throw std::runtime_error("Pause source text: "+error);
                    pauseTextSurface=textKey;pauseTextWidth=window.width();pauseTextHeight=window.height();
                }
                pauseText.draw(overlay);
            }
        };
        f::platform_input::Surface uiSurface;
        uiSurface.hit=[&](auto point){
            using namespace f::platform_input;
            if(pauseMenuOpen) {
                const auto hit=f::pause_ui::source_pause_ui_hit_test_v1(currentPauseArt(),point.x*480.f/window.width(),point.y*320.f/window.height());
                return hit.hit?Hit{Control::menu_item,std::uint64_t(hit.action)}:Hit{};
            }
            if(characterMenu.is_open()) {
                if(point.x<0||point.y<0||point.x>=window.width()||point.y>=window.height())return Hit{};
                const auto action=characterMenu.hit_test(point.x,point.y,window.width(),window.height());
                // Page body contours are dispatched by their retained provider;
                // modal input must reach that route as well as the top tabs.
                return Hit{Control::menu_item,std::uint64_t(action)};
            }
            if(!options.hud||!combatSession)return Hit{};
            if(f::pause_ui::source_pause_ui_hit_test_v1(pauseHudArt,point.x*480.f/window.width(),point.y*320.f/window.height()).hit)return Hit{Control::pause,0};
            if(pcHudReady) {
                const float scale=window.height()/320.f;
                const float offset=(window.width()/scale-480.f)*.5f;
                Hit hit;
                if(!f::generic_skills::pc_gameplay_hud_hit_test_v1(pcHudPresentation,point.x/scale-offset,point.y/scale,hit,error))throw std::runtime_error("PC HUD hit: "+error);
                if(hit.control!=Control::none)return hit;
            }
            std::array<float,4> bounds;if(!f::original_hud_portrait_bounds(0,bounds,error))throw std::runtime_error(error);
            const float scale=window.height()/320.f;
            if(point.x>=bounds[0]*scale&&point.x<=bounds[1]*scale&&point.y>=bounds[2]*scale&&point.y<=bounds[3]*scale)return Hit{Control::profile,0};
            return Hit{};
        };
        semanticInput.set_surface(std::move(uiSurface));
        std::shared_ptr<f::enemy_ai::RuntimeEnemyNavigationV1> enemyNavigation;
        std::shared_ptr<f::enemy_ai::RuntimeEnemyControllerV1> enemyController;
        std::shared_ptr<f::SourceNavigationWorldStorage> enemyNavigationWorld;
        auto bindEnemyAI=[&](){
            if(!options.runtimeEnemyAI)return;
            if(!combatSession||!sourceObstacles||!motor)
                throw std::runtime_error("Enemy AI requires the shared combat, movement and navigation world");
            if(enemyNavigation&&enemyNavigationWorld==sourceObstacles)return;
            f::enemy_ai::RuntimeEnemyNavigationConfigV1 config;
            config.navigation_world=sourceObstacles;config.bodies=&nativeBodies;config.scene_collision=&collision;
            config.walk_alias="ai-walk";config.walk_choice={"Walk",0,{0}};
            config.idle_alias="ai-idle";config.idle_choice={"Idle",0,{0}};
            enemyNavigation=std::make_shared<f::enemy_ai::RuntimeEnemyNavigationV1>(std::move(config));
            enemyController=std::make_shared<f::enemy_ai::RuntimeEnemyControllerV1>(
                f::enemy_ai::make_runtime_enemy_navigation_services_v1(enemyNavigation));
            combatSession->set_actor_decision_provider([owner=enemyNavigation,controller=enemyController](auto& session,double dt,std::string& error){
                return controller->update(session,dt,error);
            });
            enemyNavigationWorld=sourceObstacles;
        };
        auto bindSourceContacts=[&](f::CombatSession& session,std::uint32_t dtMs,std::string& e) {
            if(!enemyController||!sourceScopes){e="Physical contact needs the installed controller and source Debug owner";return false;}
            if(!enemyController->begin_contact_frame(session,dtMs,e))return false;
            if(sourceContacts->owner&&sourceContacts->owner->owns_binding()&&sourceContacts->controller==enemyController)return true;
            using Policy=f::enemy_ai::RuntimeEnemyContactAISPolicyV1;
            auto policies=std::make_shared<std::map<f::ActorId,Policy>>();
            for(const auto& body:sourceBodyPlans) {
                Policy policy=Policy::inherited_default;
                if(body.first!=session.player_id()) {
                    const auto placed=std::find_if(population.actors().begin(),population.actors().end(),[&](const auto& p){return p.definition.stableId==body.first;});
                    if(placed==population.actors().end()){e="Selected contact policy lacks its configured actor occurrence";return false;}
                    const auto profile=options.combat.profiles.find(placed->profileId);
                    if(profile==options.combat.profiles.end()||(profile->second.animationOnly&&!profile->second.receiveDamage))policy=Policy::absent;
                }
                policies->emplace(body.first,policy);
            }
            f::enemy_ai::RuntimeEnemyContactProjectionServicesV1 services;
            services.selected_ais_policy=[policies](f::ActorId id,Policy& out,std::string& e){const auto found=policies->find(id);if(found==policies->end()){e="Contact actor has no installed behavior policy";return false;}out=found->second;e.clear();return true;};
            // Installed player/enemy behaviors use the source-proven inherited
            // default collision methods. Presentation-only roles have no active
            // modern AIS; this does not claim their original script InitPost.
            services.debug_load=[scope=sourceScopes](std::string& e){return scope->debug_load(e);};
            services.debug_switch=[scope=sourceScopes](const char* key,bool& value,std::string& e){return scope->debug_switch(key,value,e);};
            services.sneaking.skill_tables=menuSourceOwner.source().skills;
            auto binding=std::make_shared<f::enemy_ai::RuntimeEnemyContactBindingV1>(session,*enemyController,std::move(services));
            auto owner=std::make_shared<f::physics::RuntimeSessionContactOwnerV1>(session,binding->services());
            for(const auto& body:sourceBodyPlans)if(!owner->bind_body(body.first,e))return false;
            sourceContacts->controller=enemyController;sourceContacts->binding=std::move(binding);sourceContacts->owner=std::move(owner);
            e.clear();return true;
        };
        std::uint64_t sourcePhysicalSteps=0,sourcePhysicsImports=0,sourcePhysicsSuppressedRoots=0;
        auto bindSourcePhysicalTransitions=[&]() {
            if(sourcePhysicalFrameEnabled||!options.sourceNativeBodies||!options.sourceFloorMotion||!options.runtimeEnemyAI)return;
            if(!combatSession||!enemyNavigation)throw std::runtime_error("Physical transitions require current Session/navigation");
            auto contextFor=[&](f::ActorId id)->NativeBodyContext& {
                const auto found=nativeBodyContexts.find(id);
                if(found==nativeBodyContexts.end())throw std::runtime_error("Physical transition lacks its current movement context");
                return *found->second;
            };
            // Normalize data from the canonical saved state, without replaying
            // Focus/Blur or mutating the saved body-presence decision.
            for(const auto& body:sourceBodyPlans) {
                auto* actor=combatSession->actor(body.first);
                const auto* props=combatSession->world()->combat_properties(body.first);
                const auto* traits=combatSession->world()->traits(body.first);
                if(!actor||!props||!traits)throw std::runtime_error("Physical reconstruction requires current source facts");
                if(props->facts.original_state==3)actor->source_flags520=0x2380u;
                else if(props->facts.original_state==12)actor->source_flags520=0x241u|(traits->is_player?0x2000u:0u);
                // P16 SPAWN: PreSpawn17 / Spawn1 bodies are owned by the lifecycle, whose source flags for these
                // states are the ones OriginalActorLifecycle::change publishes (0x1300 / 0x241). Pool and intro
                // actors reach this point while hidden or spawning.
                else if(const auto* lifecycleStatus=actorLifecycle.status(body.first);lifecycleStatus&&(lifecycleStatus->state==17||lifecycleStatus->state==1))actor->source_flags520=lifecycleStatus->flags;
                else throw std::runtime_error("Physical reconstruction supports normalized Idle/Dead only: actor="+std::to_string(body.first)+" worldState="+std::to_string(props->facts.original_state)+" sessionState="+std::to_string(combatSession->original_actor_state(body.first)));
                auto& context=contextFor(body.first);context.idleSuppressed=false;context.gate528=0;
                context.destination=actor->transform.position;
            }
            f::physics::SessionActorTransitionConsumerConfigV1 config;
            config.bodies=&nativeBodies;config.navigation=enemyNavigation.get();
            config.source.read_idle_suppressed538=[contextFor](f::ActorId id,bool& value,std::string& e){value=contextFor(id).idleSuppressed;e.clear();return true;};
            config.source.clear_idle_suppressed538=[contextFor](f::ActorId id,std::string& e){contextFor(id).idleSuppressed=false;e.clear();return true;};
            config.source.stop_after_route_drop=[&,contextFor](f::ActorState& actor,auto&,bool& fromPhysics,std::string& e) {
                if(!actor.source_flags520){e="Stop requires current source flags";return false;}
                contextFor(actor.id).destination=actor.transform.position;
                if(actor.id==combatSession->player_id()) {if(motor)motor->stopDesiredHeading();}
                else {const auto movement=populationMotors.find(actor.id);if(movement!=populationMotors.end())movement->second->stopDesiredHeading();}
                fromPhysics=(*actor.source_flags520&2u)!=0;e.clear();return true;
            };
            const auto attackDelay=[&](f::ActorId id,std::uint32_t& delay,std::string& e) {
                const auto* props=combatSession->world()->combat_properties(id);
                const auto* ai=props?dh2::data::ai_props(combatSession->world()->factions(),props->sheets.resolved[1]):nullptr;
                if(!ai||ai->attack_delay<0){e="Physical attack timer requires source AI AttackDelay";return false;}
                delay=std::uint32_t(ai->attack_delay);e.clear();return true;
            };
            config.source.attack_focus_delay_gate=[contextFor,attackDelay](const auto& event,std::string& e) {
                std::uint32_t delay=0;if(!attackDelay(event.actor,delay,e))return false;
                if(delay)contextFor(event.actor).gate528|=1u;return true;
            };
            config.source.attack_blur_delay_timer=[contextFor,attackDelay](const auto& event,std::string& e) {
                std::uint32_t delay=0;if(!attackDelay(event.actor,delay,e))return false;
                if(delay)contextFor(event.actor).attackTimerMs=delay;return true;
            };
            config.source.skill_focus_moving_byte=[&](const auto& event,bool& moving,std::string& e) {
                const auto* receipt=skillCastCoordinator?skillCastCoordinator->receipt(event.actor):nullptr;
                const auto rows=menuSourceOwner.skill_owner->borrow();
                if(!receipt||receipt->generation!=event.generation||receipt->skill_table_id<0||std::size_t(receipt->skill_table_id)>=rows.skills().size()) {
                    e="Physical Skill Focus requires current cast generation/source row";return false;
                }
                moving=(rows.skills()[std::size_t(receipt->skill_table_id)].scalar.words[2]&0xff)!=0;e.clear();return true;
            };
            config.source.skill_focus_gate528=[contextFor](const auto& event,bool moving,std::string& e) {
                auto& gate=contextFor(event.actor).gate528;gate=(gate&~0x140u)|(moving?0x100u:0u);e.clear();return true;
            };
            config.source.skill_blur_timer10_event48=[contextFor](const auto& event,bool& started,std::string& e) {
                auto& context=contextFor(event.actor);started=(context.gate528&0x100u)!=0;
                if(started) {
                    context.skillPinTimerMs=10u;
                    std::cout<<"Source physical timer stored actor="<<event.actor<<" event=48 delayMs=10 serial="<<event.update_serial<<" occurrence="<<event.occurrence<<std::endl;
                }
                e.clear();return true;
            };
            config.source.source_is_player=[&](f::ActorId id,bool& value,std::string& e) {
                const auto* traits=combatSession->world()->traits(id);if(!traits){e="Dead Focus requires current IsPlayer";return false;}
                value=traits->is_player;e.clear();return true;
            };
            config.source.dead_focus_physical_filter=[&](f::ActorId id,std::string& e){return nativeBodies.set_source_physical_filter(id,[&](f::ActorId current){return combatSession->actor(current);},0,0x51c,3,false,e);};
            config.source.dead_blur_reset_filter=[&](f::ActorId id,std::string& e){return nativeBodies.reset_source_physical_filter(id,[&](f::ActorId current){return combatSession->actor(current);},e);};
            auto consumer=f::physics::make_session_actor_transition_consumer_v1(std::move(config));
            if(!consumer->bind(*combatSession,error))throw std::runtime_error("Source physical transition binding: "+error);
            sourceContacts->transitions=std::move(consumer);sourcePhysicalFrameEnabled=true;
            std::cout<<"Source physical frame bound actors="<<sourceBodyPlans.size()<<" stateReconstruction=Idle/Dead clock=actual-session"<<std::endl;
        };
        sourcePhysicalFrameBegin=[&](f::CombatSession& session,double delta,std::string& e) {
            if(!sourcePhysicalFrameEnabled||delta==0)return true;
            const double milliseconds=delta*1000+sourcePhysicalFractionMs;
            if(!std::isfinite(milliseconds)||milliseconds<0||milliseconds>std::numeric_limits<std::uint32_t>::max()){e="Physical frame millisecond clock is invalid";return false;}
            const auto dtMs=std::uint32_t(milliseconds);sourcePhysicalFractionMs=milliseconds-dtMs;
            if(!bindSourceContacts(session,dtMs,e))return false;
            bool stepped=false;
            if(!nativeBodies.step_world(*nativeWorld,session.update_serial(),dtMs,[&session](f::ActorId id){return session.actor(id);},stepped,e))return false;
            if(stepped)++sourcePhysicalSteps;
            for(auto& entry:nativeBodyContexts) {
                auto& context=*entry.second;
                if(context.attackTimerMs) {
                    if(dtMs>=*context.attackTimerMs){context.attackTimerMs.reset();context.gate528&=~1u;}
                    else *context.attackTimerMs-=dtMs;
                }
                if(context.skillPinTimerMs) {
                    if(dtMs>=*context.skillPinTimerMs) {
                        context.skillPinTimerMs.reset(); // source one-shot retires before delivery
                        const auto* props=session.world()->combat_properties(entry.first);
                        const auto* physical=nativeBodies.physical(entry.first);
                        const bool shouldPin=props&&props->facts.original_state==3&&physical&&physical->native().body;
                        if(shouldPin)
                            if(!nativeBodies.set_pinned(entry.first,true,e))return false;
                        std::cout<<"Source physical timer expired actor="<<entry.first<<" event=48 serial="<<session.update_serial()<<" state="<<(props?props->facts.original_state:-1)<<" pinned="<<shouldPin<<std::endl;
                    } else *context.skillPinTimerMs-=dtMs;
                }
            }
            if(!enemyController->process_contact_pause(session,*sourceContacts->owner,e))return false;
            if(sourcePhysicalPlayerControls)sourcePhysicalPlayerControls();
            e.clear();return true;
        };
        bool gameplayWasPaused=false;std::uint64_t gameplayPausedFrames=0;
        bool returnToFrontend=false;
        // Preview 15 B049: Stats staged spends. Closing the Character menu with staged points opens the original
        // "Confirm character point allocation?" box instead: Yes saves and closes; No refunds every staged spend
        // on the live Session, saves the reverted state and closes (features/character_menu/stat_training_v1).
        const auto persistStatState=[&](const f::CharacterState& c,std::string& e){return f::save_character(options.save,c,e);};
        const auto statConfirmYes=[&]() {
            std::string e;
            if(!f::character_menu::commit_stat_visit_v1(state,*statTrainingVisit,persistStatState,e)) {std::cerr<<"Stats confirmation diagnostic: "<<e<<'\n';return;}
            statConfirmOpen=false;characterMenu.close();std::cout<<"Stats confirmed frame="<<drawn<<" points="<<state.source_stat_points<<'\n';
        };
        const auto statConfirmNo=[&]() {
            std::string e;
            if(!combatSession||!f::character_menu::cancel_stat_visit_v1(state,*combatSession,properties,*statTrainingVisit,persistStatState,e)) {std::cerr<<"Stats cancel diagnostic: "<<(combatSession?e:std::string("no live Session"))<<'\n';return;}
            statConfirmOpen=false;characterMenu.close();std::cout<<"Stats cancelled frame="<<drawn<<" points="<<state.source_stat_points<<'\n';
        };
        const auto routeStatConfirmClick=[&](const f::platform_input::Point& point) {
            const auto hit=f::pause_ui::source_pause_ui_hit_test_v1(pauseConfirmArt,point.x*480.f/window.width(),point.y*320.f/window.height());
            const auto route=f::pause_ui::source_pause_ui_route_v1(pauseConfirmArt,hit);
            if(route.kind==f::pause_ui::SourcePauseRouteKindV1::return_to_main_menu)statConfirmYes();
            else if(route.kind==f::pause_ui::SourcePauseRouteKindV1::cancel_confirmation)statConfirmNo();
        };
        const auto routePauseClick=[&](const f::platform_input::Point& point) {
            const auto& frame=currentPauseArt();
            const auto hit=f::pause_ui::source_pause_ui_hit_test_v1(frame,point.x*480.f/window.width(),point.y*320.f/window.height());
            const auto route=f::pause_ui::source_pause_ui_route_v1(frame,hit);
            switch(route.kind) {
            case f::pause_ui::SourcePauseRouteKindV1::resume_game:pauseMenuOpen=false;pauseConfirmation=false;std::cout<<"Pause menu resumed frame="<<drawn<<'\n';break;
            case f::pause_ui::SourcePauseRouteKindV1::open_main_menu_confirmation:pauseConfirmation=true;break;
            case f::pause_ui::SourcePauseRouteKindV1::cancel_confirmation:pauseConfirmation=false;break;
            case f::pause_ui::SourcePauseRouteKindV1::return_to_main_menu:returnToFrontend=true;std::cout<<"Pause menu confirmed main menu frame="<<drawn<<'\n';break;
            case f::pause_ui::SourcePauseRouteKindV1::unsupported_action:std::cerr<<"Pause menu diagnostic: "<<route.diagnostic<<'\n';break;
            default:break;
            }
        };
        const auto reportGameplayPause=[&](const char* transition) {
            if(!combatSession)return;
            std::cout<<"Character menu gameplay "<<transition<<" frame="<<drawn<<" updateSerial="<<combatSession->update_serial()<<'\n';
            for(const auto& entry:combatSession->world()->actors()) {
                const auto& actor=entry.second;
                std::cout<<"Character menu gameplay snapshot actor="<<actor.id<<" HP="<<actor.health<<" position="<<actor.transform.position[0]<<','<<actor.transform.position[1]<<','<<actor.transform.position[2];
                if(const auto* pose=combatSession->retained_actor_pose(actor.id))std::cout<<" poseMs="<<pose->slots().at(pose->current_slot()).timeline.current_ms;
                std::cout<<'\n';
            }
        };
        loadingScreen.finish();  // holds 100% for the minimum display time, then gameplay
        // P16 DESPAWN: automatic despawn after death through the lifecycle (features/despawn/despawn_after_death_v1). Tracks
        // dead lifecycle actors (state Idle), releases the body at the death-animation end (source CSDead event 34), runs
        // CharacterDesign.Despawn_Delay (source event 46), plays the Despawn clip (lifecycle state 2) and completes into Limbus.
        // Summoned actors release their spawn-pool slot on completion. Owner state is transient (dropped with the world).
        f::despawn::DespawnAfterDeathV1 despawnOwner;
        std::uint32_t despawnDelayMs=0;
        const auto despawnServices=[&]() {
            f::despawn::Services s;
            s.log=[](const std::string& line){std::cout<<line<<'\n';};
            s.release_body=[&](std::uint64_t id,std::string& e){return actorLifecycle.release_body(id,e);};
            s.play_clip=[&](std::uint64_t id,std::string& e){return actorLifecycle.despawn(id,e);};
            s.hide=[&](std::uint64_t id,std::string& e){return actorLifecycle.put_limbus(id,e);};
            s.finished=[&](std::uint64_t id,bool& done,std::string& e){
                const auto* status=actorLifecycle.status(id);
                if(!status){e="Despawn actor has no lifecycle record";return false;}
                done=status->state==0;return true;};
            s.release_slot=[&](std::uint64_t id,std::string& e){
                if(!spawnPool.owns(id)){e="Despawn actor is not a spawn-pool slot";return false;}
                return spawnPool.release(id,e);};
            return s;
        };
        // One frame of the despawn owner. Returns false with the reason when an owner refuses (the caller reports it).
        const auto despawnTick=[&](double seconds,std::uint64_t frame,std::string& e)->bool {
            if(!combatSession||!lifecycleEnabled)return true;
            if(!despawnDelayMs) { // CharacterDesign.Despawn_Delay from design_pycst (2000 in the source data)
                const auto designBytes=assets.read("original-cache/data/pydata/design_pycst.bin");
                const auto destroyDesign=[](dh2_script_constants* value){if(value)dh2_script_constants_destroy(value);};
                std::unique_ptr<dh2_script_constants,decltype(destroyDesign)> design(dh2_script_constants_create(),destroyDesign);
                dh2_script_constants_reload designReload{};std::int32_t delay{};
                if(!design||dh2_script_constants_load(design.get(),designBytes.data(),std::uint32_t(designBytes.size()),&designReload)!=0||designReload.consumed!=designBytes.size()||
                   dh2_script_constants_get(design.get(),"CharacterDesign","Despawn_Delay",&delay)!=0||delay<0) {e="CharacterDesign.Despawn_Delay is unavailable";return false;}
                despawnDelayMs=std::uint32_t(delay);
            }
            auto services=despawnServices();
            for(const auto& placed:population.actors()) {
                const auto id=placed.definition.stableId;
                const auto* status=actorLifecycle.status(id);
                if(!status||status->failed)continue;
                const auto* actor=combatSession->actor(id);
                if(!despawnOwner.tracked(id)) {
                    if(status->state!=3||!actor||actor->alive())continue;
                    const auto melee=meleeBindings.find_actor(placed.profileId);
                    bool hasClip=false;
                    if(melee){const auto clip=melee->states.find("Despawn");hasClip=clip!=melee->states.end()&&!clip->second.empty();}
                    if(!despawnOwner.track(id,spawnPool.owns(id),hasClip,despawnDelayMs,e))return false;
                    std::cout<<"DESPAWN tracked actor="<<id<<" name="<<placed.definition.name<<" summoned="<<spawnPool.owns(id)<<" clip="<<(hasClip?"Despawn":"none")<<" frame="<<frame<<'\n';
                    continue;
                }
                const auto* record=despawnOwner.record(id);
                if(record->phase!=f::despawn::Phase::dying)continue;
                // Source CSDead event 34: the death clip has ended (whole sequence complete).
                const auto* pose=combatSession->retained_actor_pose(id);
                if(pose&&pose->current_ended()&&!despawnOwner.death_ended(id,services,e)) {
                    if(e.empty())e="Despawn death end refused";return false;}
            }
            if(!despawnOwner.advance(static_cast<std::uint32_t>(std::lround(seconds*1000.0)),services,e))return false;
            return despawnOwner.poll(services,e);
        };
        while(!window.should_close()) {
            if(options.hud&&combatSession)bindEquipmentPage();
            combatTextFrame=drawn;
            if(drawn==options.resizeFrame) {
                if(!window.resize(options.resizeWidth,options.resizeHeight))throw std::runtime_error("Window resize: "+window.error());
                std::cout<<"Window resized frame="<<drawn<<" width="<<options.resizeWidth<<" height="<<options.resizeHeight<<'\n';
            }
            window.poll();
            if(drawn==options.pausePageFrame){pauseMenuOpen=true;pauseConfirmation=false;std::cout<<"Pause menu opened frame="<<drawn<<" via source-page diagnostic\n";}
            if(!window.key_down(VK_ESCAPE))escapeClosedMenu=false;
            semanticInput.set_menu_open(characterMenu.is_open()||pauseMenuOpen);
            if(!window.focused()){semanticInput.lose_focus();mouseHeld=false;}
            const std::array<int,15> uiKeys{'A','D','W','S',VK_SHIFT,VK_SPACE,VK_TAB,'E','C',VK_ESCAPE,'1','2','3','4','5'};
            const bool scheduledSpaceHeld=std::any_of(options.spaceKeyIntervals.begin(),options.spaceKeyIntervals.end(),[&](const auto& interval){return drawn>=interval.first&&drawn<interval.first+interval.second;});
            for(int key:uiKeys)semanticInput.key(key,window.key_down(key)||(key==VK_SPACE&&scheduledSpaceHeld));
            float pointerX=0,pointerY=0;
            if(window.cursor_position(pointerX,pointerY)) {
                const bool down=window.key_down(VK_LBUTTON);
                if(down&&!mouseHeld)semanticInput.pointer(0,f::platform_input::PointerPhase::down,{pointerX,pointerY});
                else if(!down&&mouseHeld) {
                    // P16 CINE: a release on the placeholder SKIP control of a running cutscene presses SKIP (ignored when hidden).
                    if(campaignHost.enabled()&&campaignHost.cinematic_skip_hit(pointerX,pointerY,float(window.width()),float(window.height())))campaignHost.press_skip();
                    semanticInput.pointer(0,f::platform_input::PointerPhase::up,{pointerX,pointerY});
                }
                else if(down)semanticInput.pointer(0,f::platform_input::PointerPhase::move,{pointerX,pointerY});
                mouseHeld=down;
            }
            if(drawn==options.profileClickFrame) {
                std::array<float,4> bounds;if(!f::original_hud_portrait_bounds(0,bounds,error))throw std::runtime_error(error);
                const float scale=window.height()/320.f;const f::platform_input::Point point{(bounds[0]+bounds[1])*.5f*scale,(bounds[2]+bounds[3])*.5f*scale};
                semanticInput.pointer(-1,f::platform_input::PointerPhase::down,point);semanticInput.pointer(-1,f::platform_input::PointerPhase::up,point);
            }
            if(drawn==options.menuCloseFrame)semanticInput.key(VK_ESCAPE,true);
            if(drawn==options.pauseCloseFrame)semanticInput.key(VK_ESCAPE,true);
            for(const auto& release:options.menuReleases)if(release.frame==drawn) {
                const f::platform_input::Point point{release.x*window.width()/480.f,release.y*window.height()/320.f};
                semanticInput.pointer(-2,f::platform_input::PointerPhase::down,point);
                semanticInput.pointer(-2,f::platform_input::PointerPhase::up,point);
            }
            for(const auto& scheduled:options.skillKeyFrames)if(scheduled.first==drawn)semanticInput.key('0'+scheduled.second,true);
            auto uiInput=semanticInput.take_frame();
            if(!options.spaceKeyIntervals.empty())std::cout<<"Semantic Space frame="<<drawn<<" scheduled="<<scheduledSpaceHeld<<" pressed="<<uiInput.attack.pressed<<" held="<<uiInput.attack.held<<" released="<<uiInput.attack.released<<" action="<<uiInput.actions.attack<<" focused="<<window.focused()<<'\n';
            for(const auto& scheduled:options.skillKeyFrames)if(scheduled.first==drawn)semanticInput.key('0'+scheduled.second,false);
            if(drawn==options.menuCloseFrame)semanticInput.key(VK_ESCAPE,false);
            if(drawn==options.pauseCloseFrame)semanticInput.key(VK_ESCAPE,false);
            if(drawn==options.campaignSkipFrame) { campaignHost.press_skip(); std::cout<<"Scripted SKIP press frame="<<drawn<<'\n'; } // P16 CINE (verification input)
            if(uiInput.menu_back){
                if(statConfirmOpen){statConfirmOpen=false;std::cout<<"Stats confirmation dismissed frame="<<drawn<<" via Escape\n";}
                else if(characterMenu.is_open()){characterMenu.close();escapeClosedMenu=true;}
                else if(pauseConfirmation)pauseConfirmation=false;
                else if(pauseMenuOpen){pauseMenuOpen=false;std::cout<<"Pause menu resumed frame="<<drawn<<" via Escape\n";}
            }
            if(uiInput.pause_pressed&&options.hud&&combatSession){pauseMenuOpen=true;pauseConfirmation=false;std::cout<<"Pause menu opened frame="<<drawn<<" via source HUD/Escape\n";}
            if(uiInput.profile_pressed&&options.hud&&combatSession){pauseMenuOpen=false;pauseConfirmation=false;if(characterMenu.is_open())characterMenu.close();else{characterMenu.open();menuUsedSkillPoint=false;statTrainingVisit->open_visit();++menuOpened;std::cout<<"Character menu opened frame="<<drawn<<" via profile input\n";}}
            if(drawn==options.skillsPageFrame) {
                if(!characterMenu.is_open()){characterMenu.open();menuUsedSkillPoint=false;statTrainingVisit->open_visit();++menuOpened;}
                if(!characterMenuComposition->select(characterMenu,f::character_menu::Tab::skills,error))throw std::runtime_error("Skills page diagnostic selection: "+error);
                std::cout<<"Character menu Skills selected frame="<<drawn<<" via same-state source provider\n";
            }
            if(drawn==options.equipmentPageFrame) {
                if(!characterMenu.is_open()){characterMenu.open();menuUsedSkillPoint=false;statTrainingVisit->open_visit();++menuOpened;}
                if(!characterMenuComposition->select(characterMenu,f::character_menu::Tab::equipment,error))throw std::runtime_error("Equipment page diagnostic selection: "+error);
                std::cout<<"Character menu Equipment selected frame="<<drawn<<" via same-state source provider\n";
            }
            // P16 MAP: --map-page-frame=N opens the character menu on the Map tab (composition provider).
            if(drawn==options.mapPageFrame) {
                if(!characterMenu.is_open()){characterMenu.open();menuUsedSkillPoint=false;++menuOpened;}
                if(!characterMenuComposition->select(characterMenu,f::character_menu::Tab::map,error))throw std::runtime_error("Map page diagnostic selection: "+error);
                std::cout<<"Character menu Map selected frame="<<drawn<<" level="<<options.level.generic_string()<<" zones="<<levelModuleZones.size()<<'\n';
                if(options.mapLegend&&characterMenu.map_legend_shown()==false)characterMenu.map_control(f::character_menu::Action::map_legend);
                if(options.mapZoom>1)mapView.zoom=std::clamp(options.mapZoom,f::map_visit::map_zoom_min,f::map_visit::map_zoom_max);
            }
            // P14 FAERY: --faery-page-frame=N opens the menu on the Faery tab (CharacterState provider).
            if(drawn==options.faeryPageFrame) {
                if(!characterMenu.is_open()){characterMenu.open();menuUsedSkillPoint=false;++menuOpened;}
                if(!characterMenuComposition->select(characterMenu,f::character_menu::Tab::faery,error))throw std::runtime_error("Faery page diagnostic selection: "+error);
                std::cout<<"Character menu Faery selected frame="<<drawn<<" via CharacterState provider\n";
            }
            // P16 QUESTUI: --quest-page-frame=N opens the menu on the Quest Log tab (test aid).
            if(drawn==options.questPageFrame) {
                if(!characterMenu.is_open()){characterMenu.open();menuUsedSkillPoint=false;++menuOpened;}
                if(!characterMenuComposition->select(characterMenu,f::character_menu::Tab::quest,error))throw std::runtime_error("Quest Log page diagnostic selection: "+error);
                std::cout<<"Character menu Quest Log selected frame="<<drawn<<'\n';
            }
            for(const auto& click:uiInput.clicks) {
                if(statConfirmOpen){routeStatConfirmClick(click.position);continue;}
                if(pauseMenuOpen){routePauseClick(click.position);continue;}
                const auto action=characterMenu.hit_test(click.position.x,click.position.y,window.width(),window.height());
                {
                    // P14 FAERY: Faery goes through the composition like Equipment/Skills (provider registered above).
                    characterMenuComposition->release(characterMenu,click.position.x,click.position.y,window.width(),window.height(),error);
                    // P16 QUESTUI: a MAKE ACTIVE / row release changes the CQPG; the runtime reloads it so the next save keeps it.
                    if(questRuntime) { std::string reloadError;if(!questRuntime->load(reloadError))std::cerr<<"Quest reload diagnostic: "<<reloadError<<'\n'; }
                    if(!error.empty())std::cerr<<"Character menu action diagnostic: "<<error<<'\n';
                    if(runtimeEquipmentPage) {
                        f::equipment_menu::RuntimeEquipmentPageReleaseV1::PendingCommand pending;
                        if(runtimeEquipmentPage->take_source_pending_command(pending,error)) {
                            // P14 EQUIP: original NativeInvAutoEquipSlot(slot) for the Details button and NativeInvAutoEquipSlot(-1) for the ALL banner.
                            if(pending.command==f::equipment_menu::MainPageCommand::request_auto_equip) {
                                if(!runtimeEquipment->auto_equip_slot(pending.source_slot,error))
                                    std::cerr<<"Equipment AutoEquip diagnostic: "<<error<<'\n';
                                else std::cout<<"Equipment AutoEquip slot="<<pending.source_slot<<" -> "<<f::equipment_menu::describe_equipment(*sharedCharacter)<<'\n';
                            } else if(pending.command==f::equipment_menu::MainPageCommand::request_auto_equip_all) {
                                if(!runtimeEquipment->auto_equip_all(error))
                                    std::cerr<<"Equipment AutoEquip diagnostic: "<<error<<'\n';
                                else std::cout<<"Equipment AutoEquip ALL -> "<<f::equipment_menu::describe_equipment(*sharedCharacter)<<'\n';
                            } else if(pending.command==f::equipment_menu::MainPageCommand::request_transmute||pending.command==f::equipment_menu::MainPageCommand::request_drop) {
                                // P14 EQUIP: NativeInvTransmuteItem / NativeInvDropItem on the selected Details row. The original asks menu_confirm2
                                // first (GAMEPLAYMENUS_TRANSMUTE_QUESTION / _DROP_QUESTION); that confirmation popup is not ported yet (EQUIP report).
                                const bool transmute=pending.command==f::equipment_menu::MainPageCommand::request_transmute;
                                const auto listIndex=runtimeEquipmentPage->details_selected_index();
                                const auto goldBefore=sharedCharacter->gold;
                                const auto ownedAt=std::find_if(sharedCharacter->inventory.begin(),sharedCharacter->inventory.end(),[&](const f::InventoryItem& item){return item.instance_id==pending.selected_instance_id;});
                                const std::string definition=ownedAt==sharedCharacter->inventory.end()?std::string("?"):ownedAt->definition_id;
                                bool done=false;std::int32_t amount=0;
                                if(ownedAt==sharedCharacter->inventory.end())error="Selected item is not owned";
                                else if(transmute) {
                                    done=equipmentTransmuteAmount&&equipmentTransmuteAmount(*ownedAt,amount,error)&&
                                        f::equipment_menu::transmute_inventory_item(*sharedCharacter,pending.selected_instance_id,amount,error);
                                    if(!equipmentTransmuteAmount&&error.empty())error="Transmute value owner is unavailable";
                                } else if(worldItems&&combatSession&&combatSession->actor(combatSession->player_id())) {
                                    // Integration (P14 EQUIP + DROPS): the real world-item store publishes the dropped unit at the player.
                                    const auto* dropper=combatSession->actor(combatSession->player_id());
                                    f::loot::RuntimeWorldItemIdV1 publishedDrop=f::loot::invalid_runtime_world_item_v1;
                                    done=f::loot::drop_item_to_world(*worldItems,*sharedCharacter,pending.selected_instance_id,1,
                                        {dropper->transform.position[0],dropper->transform.position[1],dropper->transform.position[2]},combatSession->player_id(),publishedDrop,error);
                                } else done=f::equipment_menu::drop_inventory_item(*sharedCharacter,pending.selected_instance_id,error);
                                if(!done)std::cerr<<(transmute?"Equipment Transmute diagnostic: ":"Equipment Drop diagnostic: ")<<error<<" instance="<<pending.selected_instance_id<<'\n';
                                else {
                                    if(transmute)std::cout<<"Equipment Transmute instance="<<pending.selected_instance_id<<" item="<<definition<<" gold "<<goldBefore<<" -> "<<sharedCharacter->gold<<" amount="<<amount<<'\n';
                                    else std::cout<<"Equipment Drop instance="<<pending.selected_instance_id<<" item="<<definition<<" -> world item"<<'\n';
                                    if(!runtimeEquipmentPage->reselect_details_near(listIndex,error))throw std::runtime_error("Equipment Details reselection: "+error);
                                }
                            } else std::cerr<<"Equipment action requires a gameplay owner: command="<<static_cast<int>(pending.command)<<" instance="<<pending.selected_instance_id<<'\n';
                        } else if(!error.empty())throw std::runtime_error(error);
                        f::equipment_menu::RuntimeEquipmentRenderChangeV1 changed;
                        bool equipmentChanged=runtimeEquipmentPage->take_source_render_change(changed,error);
                        if(!equipmentChanged&&error.empty())equipmentChanged=runtimeEquipment->take_render_change(changed,error);
                        if(equipmentChanged) {
                            if(changed.actor_id!=combatSession->player_id()||changed.same_session_visual!=combatSession->retained_actor_visual_borrow(changed.actor_id)||!changed.attachments)
                                throw std::runtime_error("Equipment render receipt lost the same Session visual");
                            runtimeEquipmentAttachments=changed.attachments;
                            if(locomotionLibrary)bindSourcePlayerLocomotion(*combatSession);
                            std::cout<<"Equipment render revision="<<changed.revision<<" actor="<<changed.actor_id<<'\n';
                            std::cout<<"Equipment state count="<<sharedCharacter->equipment.size()<<" attachments="<<changed.attachments->attachments().size();
                            for(const auto& equipped:sharedCharacter->equipment)std::cout<<" slot="<<equipped.source_slot<<":"<<equipped.item_instance_id;
                            std::cout<<'\n';
                        } else if(!error.empty())throw std::runtime_error(error);
                    }
                }
            }
            if(returnToFrontend) {
                if(worldItems)worldItems->clear(); // P14 DROPS: ground items never survive a return to the main menu
                // B040: original MenuMainMenu::Hide StopMusic(1000); must run while the gameplay audio host is still alive.
                if(runtimeAudio) {std::string audioError;if(!runtimeAudio->on_return_to_menu(audioError))std::cerr<<"Audio return-to-menu diagnostic: "<<audioError<<'\n';}
                std::string saveError;
                f::frontend::menu_return::TicketV1 ticket;
                const bool admitted=combatSession&&skillCastCoordinator&&
                    f::frontend::menu_return::request_v1({combatSession.get(),skillCastCoordinator.get(),true},ticket,saveError);
                if(admitted) {
                    stampSaveMetadata(state,options.level.generic_string()); // P14 schema: SG_SavePlayer date + LevelList row before the slot write
                    f::frontend::menu_return::CommitV1 receipt;
                    if(!f::frontend::menu_return::commit_v1(ticket,*skillCastCoordinator,options.level.generic_string(),options.liveSave,options.save,state,receipt,saveError)||!receipt.may_return_to_frontend)
                        throw std::runtime_error("Pause main-menu save: "+saveError);
                    std::cout<<"Pause main-menu save prefix frame="<<drawn<<" HP="<<state.stats.health<<" MP="<<state.stats.resource<<" profile="<<options.save.generic_string()<<" levelSaved="<<receipt.level_saved<<'\n';
                } else {
                    // A transient world program is not serializable here. Its
                    // stable profile remains independently persistable.
                    auto profile=state;
                    if(combatSession) {
                        const auto* player=combatSession->actor(combatSession->player_id());
                        if(!player||!player->persistent_character_id||*player->persistent_character_id!=profile.id)
                            throw std::runtime_error("Pause profile-only save identity differs from current player");
                        profile.stats.health=player->health;profile.stats.max_health=player->max_health;
                        profile.stats.resource=player->resource;profile.stats.max_resource=player->max_resource;
                    }
                    const auto checkpointDiagnostic=saveError;
                    stampSaveMetadata(profile,options.level.generic_string()); // P14 schema
                    if(!f::save_character(options.save,profile,saveError))throw std::runtime_error("Pause profile-only save: "+saveError);
                    state=std::move(profile);
                    std::cout<<"Pause main-menu profile-only save frame="<<drawn<<" HP="<<state.stats.health<<" MP="<<state.stats.resource<<" profile="<<options.save.generic_string()<<" worldCheckpointOmitted="<<checkpointDiagnostic<<'\n';
                }
                break;
            }
            semanticInput.set_menu_open(characterMenu.is_open()||pauseMenuOpen);
            if(characterMenu.is_open()||pauseMenuOpen)uiInput.actions={};
            double now=window.seconds();dt=options.fixedStep>0?options.fixedStep:std::clamp(now-previous,0.,.1);previous=now;
            // B039 measurement: per-second frame count and worst clamped frame time, printed only with --frames.
            if(options.frames>0) {static double secondStart=now,worstDt=0;static int secondFrames=0,secondIndex=0;++secondFrames;worstDt=std::max(worstDt,dt);if(now-secondStart>=1.0){++secondIndex;std::cout<<"Frame rate second="<<secondIndex<<" frames="<<secondFrames<<" worstFrameMs="<<worstDt*1000.0<<'\n';secondStart=now;secondFrames=0;worstDt=0;}}
            if(runtimeAudio) {std::string audioError;if(!runtimeAudio->window_activity(window.focused(),window.minimized(),audioError))std::cerr<<"Audio activity diagnostic: "<<audioError<<'\n';}
            if(window.minimized()) {dh::foundation::platform_sleep_milliseconds(10);continue;}
            // P16 SPAWN: --spawn-test TEMPLATE@X,Y,Z@FRAME. Owners: the pool's admitted actors, this session's
            // actor transforms/native bodies, the shared combat RNG and the actor lifecycle (same as authored population).
            // P16 SPAWN/CONTAINERS2: one service set for every caller of the spawn owner (--spawn-test and container Summon).
            const auto makeSpawnServices=[&]() -> f::spawn::SpawnServicesV1 {
                f::spawn::SpawnServicesV1 spawnServices;
                spawnServices.characters=&properties.characters;spawnServices.templates=&spawnTemplateTable;
                spawnServices.random_index=[&](std::int32_t bound,std::int32_t& index,std::string& e){
                    std::uint32_t value=0;
                    if(bound<=0||!combatSession->world()->random_uniform(std::uint32_t(bound),value,e))return false;
                    index=std::int32_t(value);return true;
                };
                spawnServices.profile_available=[&](const std::string& profileId,std::string& e){
                    if(!profiles.find(profileId)||!options.combat.profiles.count(profileId)){e="no actor profile or combat policy";return false;}
                    return true;
                };
                spawnServices.place=[&](std::uint64_t actorId,const std::array<float,3>& position,float heading,std::string& e){
                    auto* spawned=combatSession->actor(actorId);
                    if(!spawned){e="spawn actor is not in the session";return false;}
                    if(options.sourceNativeBodies&&!nativeBodies.set_position(actorId,position,true,e))return false;
                    spawned->transform.position=position;spawned->transform.rotation[2]=heading;return true;
                };
                spawnServices.begin=[&](std::uint64_t actorId,f::spawn::SpawnClipPolicy clip,std::string& e){
                    return clip==f::spawn::SpawnClipPolicy::source_spawn_state?actorLifecycle.spawn(actorId,e):actorLifecycle.put_idle(actorId,e);
                };
                spawnServices.hide=[&](std::uint64_t actorId,std::string& e){return actorLifecycle.put_limbus(actorId,e);};
                spawnServices.log=[&](const std::string& line){std::cout<<line<<'\n';};
                return spawnServices;
            };
            // P16 CONTAINERS2 (T4 DoOpen loot, T6 OnOpen contract): one opened declaration, once per open event.
            const auto runContainerOpen=[&](std::size_t index,std::uint64_t frame) {
                if(!combatSession||index>=containerRuntime.size())return;
                const auto instance=containerRuntime.instance(index);
                const f::ActorDefinition* definition=nullptr;
                for(const auto& candidate:population.definitions())if(candidate.stableId==instance.stableId){definition=&candidate;break;}
                const auto* object=combatSession->world()->find_object(instance.stableId);
                if(!definition||!object)std::cout<<"Container loot declaration="<<instance.name<<" status=unbound frame="<<frame<<'\n';
                else if(!containerLoot.bound())std::cout<<"Container loot declaration="<<instance.name<<" status=no_loot_owner frame="<<frame<<'\n';
                else {
                    f::containers::ContainerLootOutcomeV1 outcome;std::string lootError;
                    if(!containerLoot.open(*definition,*object,combatSession->player_id(),rewardBindingGeneration,instance.loot_id,outcome,lootError))
                        std::cout<<"Container loot declaration="<<instance.name<<" table="<<instance.loot_id<<" status=refused detail="<<lootError<<" frame="<<frame<<'\n';
                    else std::cout<<"Container loot declaration="<<instance.name<<" table="<<instance.loot_id<<" selected="<<outcome.receipt.selected_items<<" delivered="<<outcome.receipt.delivered_items<<" gold_unpriced="<<outcome.gold_unpriced<<" status=ok frame="<<frame<<'\n';
                }
                if(instance.script.empty()){std::cout<<"Container OnOpen script=(none) declaration="<<instance.name<<" frame="<<frame<<'\n';return;}
                const auto* contract=f::containers::find_open_script_contract_v1(instance.script);
                if(!contract){if(containerScriptsNoticed.insert(instance.script).second)std::cout<<"Container OnOpen script="<<instance.script<<" status=no_contract (logged once)\n";return;}
                f::containers::OpenScriptRunReportV1 report;std::string scriptError;
                const auto random=[&](std::int32_t lo,std::int32_t hi,std::int32_t& value,std::string& e){
                    std::uint32_t drawnValue=0;
                    if(lo!=0||hi!=100){e="GetRand bounds outside the original 0..100 contract";return false;}
                    if(!combatSession->world()->random_uniform(101,drawnValue,e))return false;
                    value=std::int32_t(drawnValue);std::cout<<"Container OnOpen GetRand(0,100)="<<value<<" declaration="<<instance.name<<" frame="<<frame<<'\n';return true;};
                const auto summon=[&](const f::containers::OpenScriptSummonRequestV1& request,std::string& reason){
                    f::spawn::SpawnRequestV1 spawnRequest;spawnRequest.name=request.character;
                    spawnRequest.position={instance.transform[12],instance.transform[13],instance.transform[14]};
                    spawnRequest.heading_radians=0;
                    spawnRequest.host_level_raw=frontendStarted?std::int32_t(state.stats.level*256):actorProperties.level_raw;
                    f::spawn::SpawnResultV1 spawnResult;std::string spawnError;
                    if(f::spawn::spawn_character_v1(spawnPool,spawnRequest,makeSpawnServices(),spawnResult,spawnError))return true;
                    reason=spawnError;return false;};
                if(!f::containers::run_open_script_v1(*contract,random,summon,report,scriptError))
                    std::cout<<"Container OnOpen script="<<instance.script<<" status=failed detail="<<scriptError<<" frame="<<frame<<'\n';
                std::cout<<"Container OnOpen script="<<instance.script<<" declaration="<<instance.name<<" draws="<<report.draws<<" summon_calls="<<report.summon_calls<<" spawned="<<report.summons_spawned<<" frame="<<frame<<'\n';
                for(const auto& line:report.summon_lines)std::cout<<"Container OnOpen script="<<instance.script<<" declaration="<<instance.name<<" "<<line<<" frame="<<frame<<'\n';
            };
            for(const auto& test:options.spawnTests)if(test.frame==drawn&&combatSession) {
                auto spawnServices=makeSpawnServices();
                f::spawn::SpawnRequestV1 spawnRequest;
                spawnRequest.name=test.name;spawnRequest.position=test.position;spawnRequest.heading_radians=0;
                spawnRequest.host_level_raw=frontendStarted?std::int32_t(state.stats.level*256):actorProperties.level_raw;
                f::spawn::SpawnResultV1 spawnResult;std::string spawnError;
                if(!f::spawn::spawn_character_v1(spawnPool,spawnRequest,spawnServices,spawnResult,spawnError))std::cout<<"SPAWN test frame="<<drawn<<" not spawned: "<<spawnError<<'\n';
            }
            // P16 SPAWN: --spawn-declared NAME@FRAME (stub for Script_SpawnCharacter of an authored declaration) and
            // --despawn-test NAME@FRAME (a live pool slot). Both use the lifecycle owner the pool uses.
            const auto placedNamed=[&](const std::string& name){return std::find_if(population.actors().begin(),population.actors().end(),[&](const auto& p){return p.definition.name==name;});};
            for(const auto& request:options.spawnDeclared)if(request.frame==drawn&&combatSession) {
                const auto placed=placedNamed(request.name);
                if(placed==population.actors().end()){std::cout<<"SPAWN declared rejected name="<<request.name<<" reason=no authored declaration in this level\n";continue;}
                const auto* status=actorLifecycle.status(placed->definition.stableId);
                if(!status){std::cout<<"SPAWN declared rejected name="<<request.name<<" reason=declaration is not admitted to the lifecycle\n";continue;}
                f::spawn::SpawnServicesV1 declaredServices;
                declaredServices.begin=[&](std::uint64_t actorId,f::spawn::SpawnClipPolicy,std::string& e){return actorLifecycle.spawn(actorId,e);};
                declaredServices.log=[&](const std::string& line){std::cout<<line<<'\n';};
                std::string declaredLine,declaredError;
                f::spawn::spawn_declared_v1(request.name,placed->definition.stableId,status->state,declaredServices,declaredLine,declaredError);
            }
            for(const auto& request:options.despawnTests)if(request.frame==drawn&&combatSession) {
                const auto placed=placedNamed(request.name);
                if(placed==population.actors().end()){std::cout<<"SPAWN despawn rejected name="<<request.name<<" reason=no population actor with this name\n";continue;}
                f::spawn::SpawnServicesV1 despawnServices;
                despawnServices.hide=[&](std::uint64_t actorId,std::string& e){return actorLifecycle.put_limbus(actorId,e);};
                despawnServices.log=[&](const std::string& line){std::cout<<line<<'\n';};
                std::string despawnError;
                if(!f::spawn::despawn_character_v1(spawnPool,placed->definition.stableId,despawnServices,despawnError))std::cout<<"SPAWN despawn rejected name="<<request.name<<" reason="<<despawnError<<'\n';
            }
            for(const auto& scheduled:options.sourceCommands)if(scheduled.frame==drawn) {
                const auto id=sourceCampaign.script_id(scheduled.script);
                const auto& command=sourceCampaign.scripts().at(id).commands.at(scheduled.index);
                bool blocking=false;
                if(!campaignWorld.command(f::CampaignCommandPhase::execute,command,sourceCommandContext,false,blocking,error)||blocking)throw std::runtime_error("Serialized command replay: "+error);
                std::cout<<"Source command frame="<<drawn<<" script="<<scheduled.script<<" index="<<scheduled.index<<" kind="<<command.kind<<'\n';
                if(command.kind==8)std::cout<<"Source camera command frame="<<drawn<<" target="<<sourceCameraTargets.target()<<" remaining="<<sourceCameraTargets.transition_remaining()<<'\n';
            }
            // P16 HOST: executor tick on the frame clock, then trigger contacts (only with --campaign-triggers).
            if(campaignHost.enabled()&&combatSession) {
                if(const auto* player=combatSession->actor(combatSession->player_id()))campaignHost.frame(std::int32_t(dt*1000.0),{player->transform.position[0],player->transform.position[1],player->transform.position[2]},player->alive());
            }
            auto pressed=[&](int key){bool down=window.key_down(key);bool first=down&&!held.count(key);if(down)held.insert(key);else held.erase(key);return first;};
            if(pressed('T')) {useTimeline=!useTimeline;if(useTimeline){timeline.reset();timeline.play();}}
            if(!combatSession||!combatSession->uses_retained_player_locomotion()) {
                if(pressed('1')) visual.select(f::CharacterPose::idle);
                if(pressed('2')) visual.select(f::CharacterPose::walk);
                if(pressed('3')) visual.select(f::CharacterPose::attack);
            }
            const auto checkpointAllowed=[&](const char* operation) {
                if(!combatSession)return true;
                if((skillCastCoordinator&&!skillCastCoordinator->checkpoint_v1(*combatSession,error))||
                   !combatSession->validate_lifecycle_checkpoint(error)) {
                    std::cerr<<operation<<" checkpoint rejected frame="<<drawn<<" serial="<<combatSession->update_serial()<<": "<<error<<'\n';
                    return false;
                }
                return true;
            };
            // P16 containers: debug interaction request (--interact-at); the context button will call the same API later.
            for(const auto& request:options.interactRequests)if(request.second==drawn){std::size_t index=0;
                if(!containerRuntime.find_by_name(request.first,index))std::cout<<"Container interact declaration="<<request.first<<" status=unknown_declaration frame="<<drawn<<'\n';
                else{float p[3]={0,0,0};if(combatSession)if(const auto* live=combatSession->actor(combatSession->player_id())){p[0]=live->transform.position[0];p[1]=live->transform.position[1];p[2]=live->transform.position[2];}
                    const auto r=containerRuntime.interact(index,p);
                    std::cout<<"Container interact declaration="<<request.first<<" status="<<f::containers::container_status_name(r.status)<<" state="<<int(r.state_before)<<"->"<<int(r.state_after)<<" distance="<<r.distance<<" frame="<<drawn<<'\n';}}

            if((pressed('R')||(options.reloadFrame&&drawn==options.reloadFrame))&&checkpointAllowed("Reload")) {
                std::optional<f::GameSave> liveSnapshot;
                if(combatSession){f::GameSave snapshot;if(!f::capture_game_save(options.level.generic_string(),combatSession->player_id(),state,*combatSession->world(),snapshot,error))throw std::runtime_error("Reload snapshot: "+error);liveSnapshot=std::move(snapshot);}
                if(options.combatText)combatText.clear_for_reload();
                retireSourceEffects();
                if(runtimeAudio)runtimeAudio->unbind();
                deathRewards.reset();
                retireEquipmentPage();
                clearNativeBodies();combatSession.reset();
                populationMotors.clear();
                f::OriginalScene nextScene;f::CharacterVisual nextVisual;
                loadContent(nextScene,nextVisual);scene=std::move(nextScene);visual=std::move(nextVisual);initializeCombat();bindMaterials();
                prepareBodyPlans();
                if(liveSnapshot){combatSession->actor(combatSession->player_id())->persistent_character_id=state.id;combatSession->detach_for_restore();if(!f::restore_game_save(*liveSnapshot,options.level.generic_string(),*combatSession->world(),state,error)||!combatSession->rebind_after_restore(error))throw std::runtime_error("Reload live actors: "+error);}
                if(combatSession){faeryCooldownClock={};faeryCooldownClock.binding_lease=combatSession->actor_binding_lease();faeryCooldownClock.has_binding_lease=true;faeryCooldownClock.session_update_serial=combatSession->update_serial();}
                if(liveSnapshot&&combatSession){std::string containerRestoreError;if(!f::containers::restore_container_world_state_v1(*combatSession->world(),containerRuntime,containerRestoreError))throw std::runtime_error("Container restore: "+containerRestoreError);} // P16 CONTAINERS2 (T5)
                rebuildNativeBodies();
                rebuildSourceScopes();
                initializeSourceTargetNodes();
                bindCombatText();
                if(runtimeAudio&&!runtimeAudio->bind(combatSession,error))
                    std::cerr<<"Audio reload diagnostic: "<<error<<'\n';
                bindEquipmentPage();
                bindDeathRewards();
                bindContainerLoot();
                bindQuestRuntime(); // P16 QUESTS
                bindSourcePresentations();
                std::cout<<"Content unloaded and reloaded at frame="<<drawn<<'\n';
            }
            if((pressed(VK_F5)||drawn==options.saveFrame)&&checkpointAllowed("Save")) {
                if(combatSession) {
                    f::GameSave snapshot;
                    stampSaveMetadata(state,options.level.generic_string()); // P14 schema: checkpoint save = SG_SavePlayer (date + LevelList row)
                    if(!f::capture_game_save(options.level.generic_string(),combatSession->player_id(),state,*combatSession->world(),snapshot,error)||!f::save_game(options.liveSave,snapshot,error))throw std::runtime_error("Live save: "+error);
                    // P14 schema (approved decision 3): F5 in combat also rewrites the slot profile so the menu panel is current.
                    if(!options.save.empty()&&!f::save_character(options.save,snapshot.character,error))throw std::runtime_error("Live save slot profile: "+error);
                    std::cout<<"Saved live checkpoint frame="<<drawn<<" HP="<<snapshot.character.stats.health<<" RNG="<<snapshot.random.seed<<'/'<<snapshot.random.calls<<'\n';
                } else {
                    stampSaveMetadata(state,options.level.generic_string()); // P14 schema: non-combat F5 is a profile save point
                    if(!f::save_character(options.save,state,error))std::cerr<<error<<'\n';else std::cout<<"Saved character\n";
                }
            }
            if((pressed(VK_F9)||drawn==options.loadFrame)&&checkpointAllowed("Restore")) {
                if(options.combatText)combatText.clear_for_reload();
                if(combatSession) {
                    f::GameSave snapshot;if(!f::load_game(options.liveSave,snapshot,error))throw std::runtime_error("Read live save: "+error);
                    if(!combatSession->detach_for_restore([&](std::string& e) {
                        retireSourceEffects();
                        clearNativeBodies();
                        if(runtimeAudio)runtimeAudio->unbind();
                        retireEquipmentPage();
                        deathRewards.reset();
                        e.clear();return true;
                    },error))throw std::runtime_error("Prepare live restore: "+error);
                    const bool restored=f::restore_game_save(snapshot,options.level.generic_string(),*combatSession->world(),state,error);const auto restoreError=error;
                    if(!combatSession->rebind_after_restore(error))throw std::runtime_error("Rebind live save: "+error);
                    if(!restored)throw std::runtime_error("Restore live save: "+restoreError);
                    {std::string containerRestoreError;if(!f::containers::restore_container_world_state_v1(*combatSession->world(),containerRuntime,containerRestoreError))throw std::runtime_error("Container restore: "+containerRestoreError);}
                    rebuildNativeBodies();
                    initializeSourceNavigation();
                    initializeSourceTargetNodes();
                    bindCombatText();
                    const auto* player=combatSession->actor(combatSession->player_id());options.actorPosition={player->transform.position[0],player->transform.position[1],player->transform.position[2]};
                    if(motor)motor->setPosition(options.actorPosition,player->transform.rotation[2]);
                    if(actorCameraAnchor.initialized()&&!actorCameraAnchor.reset(actorCameraFrame(),error))throw std::runtime_error(error);
                    cameraFractionMs=0;
                    const auto restoredAnchor=actorCameraAnchor.initialized()?actorCameraAnchor.position():f::CameraVec3{options.actorPosition.x,options.actorPosition.y,options.actorPosition.z};
                    if(options.sourceCamera&&!originalCamera.reset(restoredAnchor,error))throw std::runtime_error(error);
                    if(runtimeAudio&&!runtimeAudio->bind(combatSession,error))
                        std::cerr<<"Audio restore diagnostic: "<<error<<'\n';
                    bindEquipmentPage();
                    bindDeathRewards();
                    bindContainerLoot();
                    bindQuestRuntime(); // P16 QUESTS
                    bindSourcePresentations();
                    skillCastCoordinator=std::make_unique<f::generic_skills::RuntimeSkillCastCoordinatorV1>();lastSkillPhase=-1;lastSkillGeneration=0;
                    faeryCooldownClock={};faeryCooldownClock.binding_lease=combatSession->actor_binding_lease();faeryCooldownClock.has_binding_lease=true;faeryCooldownClock.session_update_serial=combatSession->update_serial();
                    std::cout<<"Restored live checkpoint frame="<<drawn<<" HP="<<player->health<<" RNG="<<snapshot.random.seed<<'/'<<snapshot.random.calls<<'\n';
                } else if(!f::load_character(options.save,state,error))std::cerr<<error<<'\n';else std::cout<<"Restored character\n";
            }
            // Original offline GSFlashMenu updates its UI without recursing
            // into the underlying gameplay state. Keep device/UI clocks live.
            const bool gameplayPaused=characterMenu.is_open()||pauseMenuOpen;
            const double gameplayDt=gameplayPaused?0.0:dt;
            if(gameplayPaused!=gameplayWasPaused)reportGameplayPause(gameplayPaused?"paused":"resumed");
            // B051: the Equipment avatar has its own idle clock. Opening a paused page restarts it
            // (original Show/CreateAvatarCamera -> idle); every frame advances it by real dt (original RenderCharacterPane).
            if(gameplayPaused&&!gameplayWasPaused&&runtimeEquipment)runtimeEquipment->restart_preview_clock();
            if(runtimeEquipment)runtimeEquipment->advance_preview_clock(dt);
            gameplayWasPaused=gameplayPaused;if(gameplayPaused)++gameplayPausedFrames;
            const float speed=float(gameplayDt)*extent*.4f;
            if(!motor)freeCamera.move((window.key_down('D')-window.key_down('A'))*speed,(window.key_down('E')-window.key_down('Q'))*speed,(window.key_down('W')-window.key_down('S'))*speed);
            freeCamera.rotate(float(gameplayDt)*60*(window.key_down(VK_RIGHT)-window.key_down(VK_LEFT)),float(gameplayDt)*60*(window.key_down(VK_UP)-window.key_down(VK_DOWN)));
            if(!gameplayPaused&&timeline.playing()) timeline.update(gameplayDt);
            gameplayInput=uiInput.actions;
            // P16 CONTEXT: Space is one context button (decide_context_button_v1). A held press over a non-combat OOI
            // (chest, NPC) suppresses the attack (source Cmd_UseOOI replaces Cmd_Attack); the press edge starts the use.
            if(combatSession&&!gameplayPaused&&combatSession->actor(combatSession->player_id())) {
                const auto* ctxOwner=combatSession->actor(combatSession->player_id());
                f::ContextButtonInputV1 ctxInput;
                ctxInput.pressed_edge=uiInput.attack.pressed;ctxInput.held=uiInput.attack.held;
                ctxInput.object_present=objectOfInterest.object()!=f::invalid_actor_id;ctxInput.object_type=objectOfInterest.interaction_type();
                ctxInput.owner_has_attack_target=ctxOwner->target_id!=f::invalid_actor_id;
                ctxInput.owner_idle_or_moving=ctxOwner->action==f::CharacterAction::idle||ctxOwner->action==f::CharacterAction::moving;
                const auto ctxDecision=f::decide_context_button_v1(ctxInput);
                if(!ctxDecision.attack_held)gameplayInput.attack=false;
                if(ctxInput.pressed_edge&&ctxDecision.use_object_of_interest) {
                    const auto ooi=objectOfInterest.object();
                    if(combatSession->actor(ooi)) { // actor OOI: source AI_SetTarget(OOI, 0)
                        std::string ctxError;
                        if(!combatSession->set_source_target(combatSession->player_id(),ooi,false,ctxError))throw std::runtime_error("Context button target: "+ctxError);
                    }
                    std::cout<<"Context button frame="<<drawn<<" ooi="<<ooi<<" type="<<objectOfInterest.interaction_type()<<" use="<<ctxDecision.use_object_of_interest<<" actor="<<(combatSession->actor(ooi)!=nullptr)<<'\n';
                }
            }
            gameplayInput.attack=gameplayInput.attack||(drawn>=options.attackStartFrame&&std::int64_t(drawn)<std::int64_t(options.attackStartFrame)+options.attackFrames);gameplayInput.targetSelect=gameplayInput.targetSelect||drawn==options.targetFrame;
            const auto playerCapabilities=options.combat.profiles.find(options.combat.playerProfileId);
            if(playerCapabilities!=options.combat.profiles.end()&&playerCapabilities->second.animationOnly){gameplayInput.attack=false;gameplayInput.targetSelect=false;}
            if(frontendStarted&&!combatSession){gameplayInput.attack=false;gameplayInput.targetSelect=false;}
            gameplayInput.run=runBound&&gameplayInput.run;
            if(drawn>=options.moveFromFrame&&drawn<options.moveFrames){gameplayInput.move2D=options.scriptedMove;gameplayInput.run=options.scriptedRun&&runBound;}
            for(const auto& seg:options.moveSegments)if(int(drawn)>=seg.start&&int(drawn)<seg.end){gameplayInput.move2D=seg.axis;gameplayInput.run=options.scriptedRun&&runBound;}
            if(gameplayPaused)gameplayInput={};
            const bool playerControllerBlocked=combatSession&&(!combatSession->actor(combatSession->player_id())->alive()||globalControllerBlocked||characterControllerBlocked[combatSession->player_id()]);
            bindEnemyAI();
            bindSourcePhysicalTransitions();
            auto applyPlayerFrameControls=[&]() {
            if(sourcePhysicalFrameEnabled&&!gameplayPaused&&!playerControllerBlocked&&combatSession&&gameplayInput.targetSelect) {
                f::ActorId selected=f::invalid_actor_id;
                if(!combatSession->select_next_player_target(selected,error))throw std::runtime_error("Source target command: "+error);
                // Commit one accepted PC selection to the real controller's
                // preferred-target owner; Session update must not cycle twice.
                gameplayInput.targetSelect=false;
                if(!enemyController||!enemyController->set_contact_target(*combatSession,combatSession->player_id(),selected,0,error))
                    throw std::runtime_error("Source preferred target: "+error);
                std::cout<<"Source target command frame="<<drawn<<" actor="<<combatSession->player_id()<<" selected="<<selected<<" serial="<<combatSession->update_serial()<<std::endl;
            }
            if(!gameplayPaused&&!playerControllerBlocked&&combatSession&&skillCastCoordinator)for(unsigned pcKeyIndex=0;pcKeyIndex<3;++pcKeyIndex)if(uiInput.skills[pcKeyIndex].pressed) {
                std::uint32_t slot=0;
                if(!f::generic_skills::pc_skill_number_to_source_slot_v1(pcKeyIndex+1,slot,error))throw std::runtime_error("PC skill mapping: "+error);
                const auto* livePlayer=combatSession->actor(combatSession->player_id());
                state.stats.health=livePlayer->health;state.stats.max_health=livePlayer->max_health;
                state.stats.resource=livePlayer->resource;state.stats.max_resource=livePlayer->max_resource;
                f::generic_skills::RuntimeSkillCastReceiptV1 receipt;std::string castError;
                if(!skillAnimationBank||!menuSourceOwner.valid())castError="Selected profile has no initialized source skill bank";
                else {
                    f::generic_skills::RuntimeSkillAnimationSlotV1 selected;
                    if(f::generic_skills::resolve_runtime_skill_animation_slot_v1(state,properties.characters,menuSourceOwner.skill_owner->borrow(),0,slot,*skillAnimationBank,selected,castError)) {
                        const auto* sequence=skillVisualPlan.sequence(selected.selection_state,0);
                        if(!sequence||sequence->phases.empty())castError="Assigned source skill has no reachable animation phase";
                        else {
                            f::generic_skills::RuntimeSkillCastRequestV1 request;
                            request.actor=combatSession->player_id();request.character=&state;request.source_slot=slot;
                            request.characters=&properties.characters;request.skills=menuSourceOwner.skill_owner->borrow();request.classes=&properties.classes;request.property_rules=&menuSkillPropertyRules;
                            request.population=&population;request.animation_bank=&*skillAnimationBank;request.visual_plan=&skillVisualPlan;request.sequence_policies=&skillSequencePolicies;
                            request.selection={selected.selection_state,0,sequence->phases.front().sourcePath};
                            if(sourceScopes)request.mana_policy.application_byte5=sourceScopes->context().onlineByte5!=0;
                            const auto* traits=combatSession->world()->traits(request.actor);if(traits)request.mana_policy.is_player=traits->is_player;
                            request.mana_policy.god_mana_registered=skillManaPolicy.savedGodMana;request.mana_policy.character_byte14f0=skillManaPolicy.actorDebugBypass;
                            bool value=false;std::string policyError;
                            if(sourceScopes&&sourceScopes->debug_switch("GOD_MANA",value,policyError))request.mana_policy.god_mana_enabled=value;
                            if(sourceScopes&&sourceScopes->debug_switch("isTracingChar_Stats",value,policyError))request.mana_policy.tracing_character_stats=value;
                            for(const auto& placed:population.actors())if(combatSession->actor(placed.definition.stableId)) {
                                const auto* targetTraits=combatSession->world()->traits(placed.definition.stableId);
                                // Current generic query scope is the active loaded population;
                                // no separately activated zone owner exists in this scope.
                                request.target_facts.push_back({placed.definition.stableId,placed.enabled,false,false,true,targetTraits&&targetTraits->targetable});
                            }
                            request.non_character_attackable_objects_absent=combatSession->world()->objects().empty();
                            request.downstream_animation_services.event=[&](f::ActorId id,const f::RetainedAnimationEvent& event,std::string&) {
                                if(event.name!="do_skill")return true;
                                const auto* caster=combatSession->actor(id);
                                std::cout<<"Source skill Use geometry actor="<<id<<" frame="<<drawn<<" sourceEvent="<<event.name<<" generation="<<event.generation<<" position="<<caster->transform.position[0]<<','<<caster->transform.position[1]<<','<<caster->transform.position[2]<<" heading="<<caster->transform.rotation[2]<<std::endl;
                                for(const auto& placed:population.actors())if(const auto* candidate=combatSession->actor(placed.definition.stableId)) {
                                    const auto* traits=combatSession->world()->traits(candidate->id);
                                    std::cout<<"Source skill Use candidate actor="<<candidate->id<<" enabled="<<placed.enabled<<" alive="<<candidate->alive()<<" targetable="<<bool(traits&&traits->targetable)<<" position="<<candidate->transform.position[0]<<','<<candidate->transform.position[1]<<','<<candidate->transform.position[2]<<" ownNode="<<candidate->source_target_node180.value_or(0);
                                    if(candidate->source_target_position184){const auto& cache=*candidate->source_target_position184;std::cout<<" cache="<<cache[0]<<','<<cache[1]<<','<<cache[2];}
                                    std::cout<<std::endl;
                                }
                                return true;
                            };
                            skillCastCoordinator->begin_skill_cast_v1(request,*combatSession,receipt,castError);
                            if(motor)motor->setFacingRadians(combatSession->actor(request.actor)->transform.rotation[2]);
                        }
                    }
                }
                std::cout<<"Source skill key="<<pcKeyIndex+1<<" slot="<<slot<<" frame="<<drawn<<" generation="<<receipt.generation<<" phase="<<int(receipt.phase)<<" skill="<<receipt.skill_name<<" MP="<<state.stats.resource<<" diagnostic="<<castError<<'\n';
            }
            if(!gameplayPaused&&!playerControllerBlocked&&uiInput.spell.pressed&&combatSession&&skillCastCoordinator) {
                std::string castError;f::generic_skills::RuntimeSkillCastReceiptV1 receipt;
                f::generic_skills::RuntimeSkillFaeryAnimationSlotV1 selected;
                // P14 FAERY: Rocky/Wetty/Windy are selectable but have no spell; key 4 logs that instead of failing silently.
                const auto currentFaery=state.faery_by_difficulty[std::size_t(f::faery_menu::active_faery_difficulty_v1())].current_faery;
                if(state.source_faery_state_known&&!f::faery_menu::faery_slot_has_spell_v1(currentFaery)){selected.faery_slot=currentFaery;castError=f::faery_menu::faery_no_spell_message_v1(currentFaery);}
                else if(!sourceFaeryTables||!skillAnimationBank)castError="Current saved Faery has no initialized source tables/animation bank";
                else if(f::generic_skills::resolve_runtime_faery_animation_slot_v1(state,f::faery_menu::active_faery_difficulty_v1(),*skillAnimationBank,selected,castError)) {
                    const auto* sequence=skillVisualPlan.sequence(selected.selection_state,0);
                    if(!sequence||sequence->phases.empty())castError="Current Faery has no reachable source Cast phase";
                    else {
                        const auto* livePlayer=combatSession->actor(combatSession->player_id());
                        state.stats.health=livePlayer->health;state.stats.max_health=livePlayer->max_health;state.stats.resource=livePlayer->resource;state.stats.max_resource=livePlayer->max_resource;
                        f::faery_menu::HottySourcePolicyV1 policy;policy.has_controller=true;
                        policy.source_online=sourceScopes&&sourceScopes->context().onlineByte5!=0;
                        policy.saved_god_mana=skillManaPolicy.savedGodMana;policy.character_god_mana=skillManaPolicy.actorDebugBypass;
                        policy.complete=sourceScopes&&sourceScopes->debug_switch("GOD_MANA",policy.debug_god_mana,castError);
                        f::faery_menu::HottyAuthoredActorOrderV1 order;order.complete=true;
                        std::vector<f::faery_menu::HottySourceActorFactsV1> facts;
                        const auto playerId=combatSession->player_id();
                        const auto* playerTraits=combatSession->world()->traits(playerId);
                        order.actor_ids.push_back(playerId);
                        facts.push_back({playerId,true,false,false,true,playerTraits&&playerTraits->targetable});
                        for(const auto& placed:population.actors())if(combatSession->world()->find_actor(placed.definition.stableId)) {
                            if(placed.definition.stableId==playerId)continue;
                            order.actor_ids.push_back(placed.definition.stableId);
                            const auto* traits=combatSession->world()->traits(placed.definition.stableId);
                            facts.push_back({placed.definition.stableId,placed.enabled,false,false,true,traits&&traits->targetable});
                        }
                        f::faery_menu::HottySourceTargetListV1 targets;
                        const auto rank=state.faery_by_difficulty[std::size_t(f::faery_menu::active_faery_difficulty_v1())].faeries[std::size_t(selected.faery_slot)].level;
                        if(policy.complete&&f::faery_menu::query_hotty_character_targets_v1(*combatSession->world(),combatSession->player_id(),order,facts,rank==0?600:800,targets,castError)) {
                            f::generic_skills::RuntimeSkillFaerySpellArmV1 arm;
                            arm.tables=&sourceFaeryTables;arm.difficulty=f::faery_menu::active_faery_difficulty_v1();arm.policy=&policy;arm.actor_order=&order;arm.source_facts=&facts;arm.targets=&targets;arm.cooldown_clock=&faeryCooldownClock;
                            arm.celest_effect_dispatch=celestEffects.get();
                            f::generic_skills::RuntimeSkillCastRequestV1 request;
                            request.dispatch=f::generic_skills::RuntimeSkillCastDispatchV1::native_hud_spell;request.actor=combatSession->player_id();request.character=&state;
                            request.classes=&properties.classes;request.property_rules=&menuSkillPropertyRules;request.characters=&properties.characters;request.skills=menuSourceOwner.skill_owner->borrow();
                            request.visual_plan=&skillVisualPlan;request.sequence_policies=&skillSequencePolicies;request.selection={selected.selection_state,0,{}};request.active_faery_spell=&arm; /* P15 HOTTY: empty group = whole state-7 root (pre clip, then do_spell clip); a leaf scope ended Hotty after its pre phase */
                            // P15 FAERYSOUND (B050): original OnPreSkill_ sounds are queued on the audio session
                            // (every cast, empty target list included) and submitted with this frame's device clock.
                            skillCastCoordinator->set_faery_pre_sound_sink([&](const f::generic_skills::RuntimeSkillFaeryPreSoundV1& pre) {
                                if(!runtimeAudio){std::cout<<"Faery cast sound frame="<<drawn<<" targets="<<pre.target_count<<" status=dropped detail=no audio session"<<std::endl;return;}
                                runtimeAudio->queue_faery_pre_sounds(pre.caster,pre.position,pre.target_count,pre.labels);
                            });
                            skillCastCoordinator->begin_skill_cast_v1(request,*combatSession,receipt,castError);
                        }
                    }
                }
                std::cout<<"Source Faery key=4 frame="<<drawn<<" slot="<<selected.faery_slot<<" sequence="<<selected.animation_sequence_id<<" generation="<<receipt.generation<<" phase="<<int(receipt.phase)<<" MP="<<state.stats.resource<<" diagnostic="<<castError<<'\n';
            }
            if(!gameplayPaused&&!playerControllerBlocked&&combatSession&&uiInput.potion.pressed) {
                f::inventory::RuntimePotionUseReceiptV1 receipt;std::string potionError;
                potionDebug.error.clear();potionDebug.strings.clear();
                const f::inventory::RuntimePotionUseServicesV1 services{&locomotionItems,&menuSkillPropertyRules,&potionDebugServices};
                const bool used=potionUse.dispatch(*combatSession,combatSession->player_id(),state,uiInput.potion,services,receipt,potionError);
                std::cout<<"Source potion key=5 frame="<<drawn<<" result="<<int(receipt.result)<<" consumed="<<receipt.consumed<<" quantity="<<receipt.quantity_before<<"->"<<receipt.quantity_after<<" HPraw="<<receipt.hp_before<<"->"<<receipt.hp_after<<" MPraw="<<receipt.mp_before<<"->"<<receipt.mp_after<<" diagnostic="<<potionError;
                if(!used&&!potionDebug.error.empty())std::cout<<" sourceDebug="<<potionDebug.error;
                std::cout<<'\n';
            }
            if(playerControllerBlocked) {
                // Attack requests reach the shared source admission wrapper.
                // Direction/target input still uses the preview admission policy.
                gameplayInput.move2D={};gameplayInput.run=false;gameplayInput.targetSelect=false;
            }
            if(!gameplayPaused&&motor&&(!combatSession||(combatSession->actor(combatSession->player_id())->alive()&&!combatSession->owns_pose(combatSession->player_id())))) {
                f::InputActions input;input.move2D=gameplayInput.move2D;input.run=gameplayInput.run;
                if(playerControllerBlocked)input={};
                auto basis=options.sourceCamera?originalCamera.movementBasis():f::cameraMovementBasis(freeCamera.pose());
                if(options.sourceHeadingRotation){if(!motor->steer_source_intent(input,basis,gameplayDt))throw std::runtime_error("Source movement intent rejected");}
                else motor->steer(input,basis,gameplayDt);
                if(combatSession&&combatSession->uses_retained_player_locomotion()) {
                    if(!combatSession->select_player_locomotion(motor->state().animationName,error))throw std::runtime_error("Source movement clip: "+error);
                } else {
                    if(!visual.select(motor->state().animationName,true,error))throw std::runtime_error("Movement clip: "+error);
                    if(!visualStep(gameplayDt))throw std::runtime_error(error);
                    auto delta=visual.take_root_motion();delta.x*=actorScale.x;delta.y*=actorScale.y;
                    if(!applyActorRootMotion(combatSession?combatSession->player_id():f::invalid_actor_id,*motor,delta,error))throw std::runtime_error("Source player root motion: "+error);options.actorPosition=motor->state().position;
                }
            } else if(!gameplayPaused&&!motor&&visual.loaded()&&(!combatSession||!combatSession->owns_pose(combatSession->player_id()))&&!visualStep(gameplayDt)) throw std::runtime_error(error);
            };
            if(sourcePhysicalFrameEnabled)sourcePhysicalPlayerControls=applyPlayerFrameControls;
            else {sourcePhysicalPlayerControls={};applyPlayerFrameControls();}
            if(combatSession) {
                auto applyRetainedMotion=[&](f::ActorState& actor,f::Vec3 delta,bool enabled,std::string& e){
                    if(!motor)return true;
                    auto* movement=motor.get();auto scale=actorScale;
                    if(actor.id!=combatSession->player_id()) {
                        const auto placed=std::find_if(population.actors().begin(),population.actors().end(),[&](const auto& p){return p.definition.stableId==actor.id;});
                        if(placed==population.actors().end()){e="Attack movement visual unavailable";return false;}
                        const auto policy=options.combat.profiles.find(placed->profileId);
                        if(policy==options.combat.profiles.end()||policy->second.motionRoot.empty())return true;
                        scale={std::hypot(placed->transform[0],placed->transform[1]),std::hypot(placed->transform[4],placed->transform[5]),std::abs(placed->transform[10])};
                        auto& owner=populationMotors[actor.id];
                        if(!owner) {
                            auto config=motor->config();f::Vec3 lo{},hi{};
                            if(!placed->visual.indexed_bounds(lo,hi,e))return false;
                            const auto* properties=combatSession->world()->combat_properties(actor.id);
                            // Compatibility radius for unbound source plans;
                            // source plans supply player and NPC radii alike.
                            config.bodyRadius=.5f*std::max((hi.x-lo.x)*scale.x,(hi.y-lo.y)*scale.y)*(properties->sheets.resolved[16]*.01f);
                            if(options.sourceBodyBounds){const auto& plan=sourceBodyPlans.at(actor.id);if(!plan.circular){e="Moving source actor needs supported circular body plan";return false;}config.bodyRadius=plan.radius_game_units;}
                            config.bodyHeight=(hi.z-lo.z)*scale.z;
                            owner=std::make_unique<f::ActorMovement>(config);
                        }
                        movement=owner.get();movement->setPosition({actor.transform.position[0],actor.transform.position[1],actor.transform.position[2]},actor.transform.rotation[2]);
                    }
                    movement->setFacingRadians(actor.transform.rotation[2]);
                    const auto before=movement->state().position;
                    auto& receipt=motionReceipts[actor.id];
                    if(receipt.enabled+receipt.disabled==0) {
                        std::cout<<"Motion provenance stage=first-root actor="<<actor.id<<" frame="<<drawn<<" enabled="<<enabled<<" position="<<actor.transform.position[0]<<','<<actor.transform.position[1]<<','<<actor.transform.position[2]<<" heading="<<actor.transform.rotation[2]<<" delta="<<delta.x<<','<<delta.y<<','<<delta.z;
                        if(options.sourceFloorMotion){const auto* nav=nativeBodies.navigation(actor.id);std::cout<<" pf="<<nav->motion.position[0]<<','<<nav->motion.position[1]<<','<<nav->motion.position[2]<<" floor="<<nav->motion.floor<<" room="<<nav->motion.room;}
                        std::cout<<std::endl;
                    }
                    if(enabled)++receipt.enabled;else ++receipt.disabled;
                    receipt.sourceXY+=std::hypot(delta.x,delta.y);
                    f::InputActions motionIntent;motionIntent.attack=actor.action==f::CharacterAction::attacking||actor.action==f::CharacterAction::hurt||actor.action==f::CharacterAction::dead;
                    if(actor.id==combatSession->player_id()&&actor.action==f::CharacterAction::moving) {motionIntent.move2D=gameplayInput.move2D;motionIntent.run=gameplayInput.run;}
                    const auto basis=options.sourceCamera?originalCamera.movementBasis():f::cameraMovementBasis(freeCamera.pose());
                    const bool locomotionAlreadySteered=actor.id==combatSession->player_id()&&combatSession->uses_retained_player_locomotion()&&(actor.action==f::CharacterAction::idle||actor.action==f::CharacterAction::moving);
                    if(!locomotionAlreadySteered&&!(options.sourceHeadingRotation&&actor.id==combatSession->player_id()?movement->steer_source_intent(motionIntent,basis,gameplayDt):movement->steer(motionIntent,basis,gameplayDt))){e="Actor movement steering rejected";return false;}
                    if(!enabled)delta={};
                    delta.x*=scale.x;delta.y*=scale.y;
                    if(!applyActorRootMotion(actor.id,*movement,delta,e,actor.id!=combatSession->player_id()||combatSession->owns_pose(actor.id)))return false;
                    const auto position=movement->state().position;
                    receipt.worldXY+=std::hypot(position.x-before.x,position.y-before.y);
                    if(options.sourceNativeBodies){if(!nativeBodies.set_position(actor.id,{position.x,position.y,position.z},false,e))return false;}
                    else actor.transform.position={position.x,position.y,position.z};
                    if(actor.id==combatSession->player_id())options.actorPosition=position;
                    return true;
                };
                if(sourcePhysicalFrameEnabled) {
                    if(!combatSession->set_motion_phase_handler([&](f::ActorState& actor,std::uint64_t frame,double,
                            const std::vector<f::CombatSessionMotionSample>& samples,std::string& e) {
                        const auto* traits=combatSession->world()->traits(actor.id);
                        if(!traits||!actor.source_flags520){e="Physical actor phase lacks actual source role/flags";return false;}
                        f::PlayableActorBodies::PhysicsPositionResult imported;
                        if(!nativeBodies.reconcile_physics_position(actor.id,[&](f::ActorId id){return combatSession->actor(id);},traits->is_player,imported,e))return false;
                        const bool physicsAccepted=imported.xy_changed&&(!imported.floor_checked||imported.floor_valid);
                        if(physicsAccepted)++sourcePhysicsImports;
                        if(actor.id==combatSession->player_id()&&motor)motor->sync_source_position({actor.transform.position[0],actor.transform.position[1],actor.transform.position[2]},actor.transform.rotation[2]);
                        for(const auto& sample:samples) {
                            if(sample.move_go&&!physicsAccepted&&(*actor.source_flags520&1u)) {
                                if(!applyRetainedMotion(actor,sample.authored_motion,true,e))return false;
                            } else {
                                auto& receipt=motionReceipts[actor.id];
                                if(sample.move_go)++receipt.enabled;else ++receipt.disabled;
                                receipt.sourceXY+=std::hypot(sample.authored_motion.x,sample.authored_motion.y);
                                if(physicsAccepted&&sample.move_go)++sourcePhysicsSuppressedRoots;
                            }
                        }
                        if(!nativeBodies.set_position(actor.id,actor.transform.position,false,e))return false;
                        if(actor.id==combatSession->player_id()) {
                            options.actorPosition={actor.transform.position[0],actor.transform.position[1],actor.transform.position[2]};
                            if(motor)motor->sync_source_position(options.actorPosition,actor.transform.rotation[2]);
                        }
                        if(physicsAccepted)std::cout<<"Source physics import actor="<<actor.id<<" serial="<<frame<<" floorChecked="<<imported.floor_checked<<" accepted="<<physicsAccepted<<" rootSamples="<<samples.size()<<" position="<<actor.transform.position[0]<<','<<actor.transform.position[1]<<','<<actor.transform.position[2]<<'\n';
                        return true;
                    },error))throw std::runtime_error("Source physical actor phase: "+error);
                } else combatSession->set_motion_handler(applyRetainedMotion);
                bindEnemyAI();
                const f::RetainedFrameAudioClock* audioClock=nullptr;
                if(runtimeAudio) {
                    bool minimalRandoms=false;std::string audioError;
                    const bool settingsKnown=!sourceScopes||sourceScopes->debug_switch("MP_MinimalRandoms",minimalRandoms,audioError);
                    auto listenerCamera=camera(options.sourceCamera?campaignHost.source_camera_pose(originalCamera.pose()):(useTimeline?timeline.sample():freeCamera.pose()));
                    if(settingsKnown)audioClock=runtimeAudio->before_update(listenerCamera,window.focused(),window.minimized(),minimalRandoms,std::uint64_t(drawn),audioError);
                    // P15 FAERYSOUND (B050): submit this frame's queued Faery cast sounds on the same device clock (nullptr drops them, logged).
                    if(runtimeAudio){std::string faeryAudioError;if(!runtimeAudio->flush_faery_pre_sounds(audioClock,faeryAudioError)&&!faeryAudioError.empty())std::cerr<<"Faery cast sound diagnostic: "<<faeryAudioError<<'\n';}
                    if(!audioError.empty()&&drawn==0)std::cerr<<"Audio frame diagnostic: "<<audioError<<'\n';
                    if(gameplayPaused&&audioClock&&(gameplayPausedFrames==1||gameplayPausedFrames%60==0))
                        std::cout<<"Character menu audio clock frame="<<drawn<<" generation="<<audioClock->output_generation<<" deviceSamples="<<audioClock->device_samples<<" qpcNs="<<audioClock->qpc_monotonic_ns<<'\n';
                }
                if(!gameplayPaused&&!combatSession->update(gameplayDt,gameplayInput,options.actorPosition,motor?motor->state().facingRadians:0,error,audioClock))throw std::runtime_error("Live combat: "+error);
                if(!gameplayPaused)f::update_object_of_interest_v1(*combatSession,combatSession->player_id(),gameplayDt,objectOfInterest,&interactables); // B004/B029 (+P16 registry)
                if(!gameplayPaused) { // P16 DESPAWN: per-frame despawn owner (after the combat update that produced the death)
                    std::string despawnError;
                    if(!despawnTick(gameplayDt,drawn,despawnError))throw std::runtime_error("Despawn: "+despawnError);
                }
                if(!gameplayPaused&&combatSession) { // P16 CONTEXT: HUD action-button frame from the cached OOI type (MenuManager 0x42eab4)
                    const int icon=f::action_button_icon_v1(objectOfInterest.interaction_type());
                    if(icon!=lastActionIcon) { lastActionIcon=icon; std::cout<<"Action icon frame="<<drawn<<" icon="<<icon<<" type="<<objectOfInterest.interaction_type()<<'\n'; }
                }
                sourcePhysicalPlayerControls={};
                if(!gameplayPaused) {
                    const auto* livePlayer=combatSession->actor(combatSession->player_id());
                    state.stats.health=livePlayer->health;state.stats.max_health=livePlayer->max_health;
                    state.stats.resource=livePlayer->resource;state.stats.max_resource=livePlayer->max_resource;
                }
                if(!gameplayPaused&&skillCastCoordinator) {
                    if(const auto* receipt=skillCastCoordinator->receipt(combatSession->player_id()))if(lastSkillPhase!=int(receipt->phase)||lastSkillGeneration!=receipt->generation) {
                        lastSkillPhase=int(receipt->phase);lastSkillGeneration=receipt->generation;
                        const auto rng=combatSession->world()->random_state();
                        std::cout<<"Source skill lifecycle frame="<<drawn<<" generation="<<receipt->generation<<" phase="<<int(receipt->phase)<<" skill="<<receipt->skill_name<<" hits="<<receipt->applied_results.size()<<" MP="<<state.stats.resource<<" RNG="<<rng.seed<<'/'<<rng.calls<<" diagnostic="<<receipt->detail<<'\n';
                    }
                }
                if(!gameplayPaused) raiseQuestKills(); // P16 QUESTS: kill events of this update
                if(!gameplayPaused) raiseQuestZones(); // P16 QUESTUI: zone entries of this update
                if(!gameplayPaused) questBanners.tick(float(dt)); // P16 QUESTUI: banner timing
                // P16 QUESTUI: NPC talk on the interact press edge (--quest-talk-frame is a scripted press for tests).
                const bool questTalkPressed=uiInput.actions.interact||std::find(options.questTalkFrames.begin(),options.questTalkFrames.end(),int(drawn))!=options.questTalkFrames.end();
                if(!gameplayPaused&&questTalkPressed&&!questTalkHeld) talkNearestNpc();
                questTalkHeld=questTalkPressed;
                // P16 QUESTS test aid: frame-scheduled bus events (--quest-debug-kill / --quest-debug-accept).
                if(!gameplayPaused&&questRuntime) for(const auto& debug:options.questDebugEvents) if(debug.frame==drawn) {
                    if(!debug.accept) {
                        for(int i=0;i<debug.count;++i) {
                            f::quest_runtime::QuestEvent kill;kill.kind=f::quest_runtime::QuestEvent::Kind::kill;kill.template_id=debug.id;
                            f::quest_runtime::raise_quest_event(kill);
                        }
                        std::cout<<"Quest debug kill frame="<<drawn<<" template="<<debug.id<<" count="<<debug.count<<" current="<<questRuntime->current_quest()<<'\n';
                    } else {
                        std::string e;
                        if(!questRuntime->accept_quest(debug.id,e)) std::cout<<"Quest debug accept frame="<<drawn<<" row="<<debug.id<<" refused: "<<e<<'\n';
                        else {
                            std::string saveError;
                            if(!questRuntime->save(saveError))std::cerr<<"Quest save diagnostic: "<<saveError<<'\n';
                            std::cout<<"Quest debug accept frame="<<drawn<<" row="<<debug.id<<" accepted\n";
                            for(const auto& banner:takeQuestBanners())
                                std::cout<<"Quest banner kind="<<questBannerKindName(banner.kind)
                                         <<" row="<<banner.row<<" xp="<<banner.reward_xp<<" gold="<<banner.reward_gold<<'\n';
                        }
                    }
                    std::cout<<"Quest state frame="<<drawn<<" gold="<<state.gold<<" xp="<<state.experience<<" cqpg="<<state.source_quest_progress_cqpg.size()<<'\n';
                }
                if(!gameplayPaused&&deathRewards.bound()) {
                    std::vector<f::loot::RuntimeDeathRewardOutcomeV1> rewards;
                    if(!deathRewards.after_update(*combatSession,rewards,error))
                        std::cerr<<"Source death reward diagnostic: "<<error<<'\n';
                    for(const auto& reward:rewards)
                        std::cout<<"Source death reward frame="<<drawn<<" victim="<<reward.victim
                                 <<" state="<<static_cast<int>(reward.state)<<" xpRecipients="<<reward.xp_recipients
                                 <<" xp="<<state.experience<<" level="<<state.stats.level
                                 <<" spawned="<<reward.spawned_items<<" store="<<worldItems->size()
                                 <<" suppressed="<<reward.rewards_suppressed<<'\n';
                }
                // P14 DROPS: ground items travel to their landing point. ItemObject::_DoAutoPickupHack: an item whose
                // PickUpType is Automatic is collected at once by the killer (here: the local player). The nearest item
                // whose sensor box contains the player becomes the target (ItemObject::OnCollisionBegins -> tooltip), and
                // PC adaptation: the interact key (E) or --pickup-frame while targeted runs ItemObject::Interact.
                if(!gameplayPaused&&worldItems&&worldDrops) {
                    const double worldItemMs=gameplayDt*1000+worldItemFractionMs;const auto worldItemWhole=std::uint32_t(worldItemMs);worldItemFractionMs=worldItemMs-worldItemWhole;
                    worldItems->advance(worldItemWhole);
                    const auto* itemPlayer=combatSession->actor(combatSession->player_id());
                    const auto runWorldItemPickup=[&](f::loot::RuntimeWorldItemIdV1 id,const char* reason) {
                        f::loot::RuntimeWorldItemEntryV1 targetEntry;std::string targetError;
                        f::loot::RuntimeWorldItemInteractionServicesV1 pickupServices;
                        pickupServices.context=&sharedCharacter;
                        pickupServices.resolve_character_state=[](void* raw,f::ActorId,std::shared_ptr<f::CharacterState>& out,std::string& e){out=*static_cast<std::shared_ptr<f::CharacterState>*>(raw);e.clear();return bool(out);};
                        f::loot::WorldItemPickupRulesV1 pickupRules;
                        if(const auto* sheet=combatSession->world()->combat_properties(combatSession->player_id()))pickupRules.potion_capacity=f::loot::potion_capacity_from_property_v1(sheet->sheets.resolved[194]);
                        if(sourceScopes){bool infinite=false;std::string debugError;if(sourceScopes->debug_switch("InfiniteInventory",infinite,debugError))pickupRules.infinite_inventory=infinite;}
                        std::string pickedId;if(worldItems->inspect(id,targetEntry,targetError)&&targetEntry.authored_item)pickedId=worldItems->tables().items().identifiers.at(std::size_t(targetEntry.source_outcome.item_id));
                        const auto goldBefore=state.gold;const auto stacksBefore=state.inventory.size();
                        f::loot::WorldItemPickupReportV1 pickup;
                        const bool picked=f::loot::interact_world_item_v1(*worldItems,id,combatSession->player_id(),true,itemPlayer,pickupServices,pickupRules,pickup);
                        std::cout<<"World item pickup frame="<<drawn<<" item="<<id<<" id="<<pickedId<<" reason="<<reason<<" outcome="<<int(pickup.outcome)<<" picked="<<picked<<" gold="<<goldBefore<<"->"<<state.gold<<" stacks="<<stacksBefore<<"->"<<state.inventory.size()<<" store="<<worldItems->size()<<'\n';
                        std::string textError;
                        if(picked) {
                            if(worldItemTarget==id)worldItemTarget=f::loot::invalid_runtime_world_item_v1;
                            // P16 QUESTUI: item pickup event for quest objectives that count pickups (none in Act 1 rows yet).
                            { f::quest_runtime::QuestEvent pickupEvent;pickupEvent.kind=f::quest_runtime::QuestEvent::Kind::item_pickup;pickupEvent.object_id=std::int32_t(targetEntry.source_outcome.item_id);f::quest_runtime::raise_quest_event(pickupEvent); }
                            equipmentRebindRequested=true; // the equipment page's bare-definition policy lists held items
                            std::uint32_t rgb=0xFFFFFF;worldDrops->item_color(targetEntry,rgb,textError);
                            f::InventoryItem shown;shown.definition_id=pickedId;shown.quantity=targetEntry.quantity;
                            std::string shownName;
                            if(pickup.item_type==f::loot::item_type_gold_v1)shownName=std::to_string(targetEntry.source_outcome.resolved_gold_value.value_or(0))+" gold";
                            else if(!itemDisplayName(shown,shownName,textError))shownName=pickedId;
                            worldItemStatus=shownName;worldItemStatusRgb=rgb;worldItemStatusFrames=90;
                        } else if(pickup.outcome==f::loot::WorldItemPickupOutcomeV1::inventory_full) {
                            std::string text;if(menuLocalization.symbol("GAMEPLAYMENUS_INVENTORY_FULL",&state,text,textError))worldItemStatus=text;else worldItemStatus="GAMEPLAYMENUS_INVENTORY_FULL";
                            worldItemStatusRgb=0xFFFFFF;worldItemStatusFrames=90;
                        }
                    };
                    if(itemPlayer&&itemPlayer->alive()) {
                        std::vector<f::loot::RuntimeWorldItemIdV1> automatic;
                        for(const auto& pair:worldItems->entries())
                            if(f::loot::world_item_is_automatic_pickup_v1(pair.second))automatic.push_back(pair.first);
                        for(const auto id:automatic)runWorldItemPickup(id,"automatic");
                    }
                    const auto previousTarget=worldItemTarget;
                    worldItemTarget=itemPlayer&&itemPlayer->alive()?f::loot::select_world_item_target_v1(*worldItems,itemPlayer->transform.position):f::loot::invalid_runtime_world_item_v1;
                    f::loot::RuntimeWorldItemEntryV1 targetEntry;std::string targetError;
                    if(worldItemTarget!=f::loot::invalid_runtime_world_item_v1&&worldItemTarget!=previousTarget&&worldItems->inspect(worldItemTarget,targetEntry,targetError))
                        std::cout<<"World item target frame="<<drawn<<" item="<<worldItemTarget<<" id="<<(targetEntry.authored_item?worldItems->tables().items().identifiers.at(std::size_t(targetEntry.source_outcome.item_id)):std::string("?"))<<" qty="<<targetEntry.quantity<<" position="<<targetEntry.source_position[0]<<","<<targetEntry.source_position[1]<<","<<targetEntry.source_position[2]<<'\n';
                    // P16 CONTEXT: walk-over pickup (no E key). Contact = the item's sensor box holds the player; a contact that
                    // begins while the character is moving is consumed on the next update (ItemObject::OnCollisionBegins +
                    // GameObject::Update -> ItemObject::Interact). Standing still on an item picks nothing up.
                    if(itemPlayer&&itemPlayer->alive()) {
                        std::vector<f::loot::RuntimeWorldItemIdV1> contacts;
                        for(const auto& pair:worldItems->entries()) {
                            const auto& p=pair.second.source_position;
                            if(std::fabs(p[0]-itemPlayer->transform.position[0])<=f::loot::world_item_sensor_half_extent_v1&&std::fabs(p[1]-itemPlayer->transform.position[1])<=f::loot::world_item_sensor_half_extent_v1)
                                contacts.push_back(pair.first);
                        }
                        static std::size_t lastContactCount=~std::size_t(0);
                        if(contacts.size()!=lastContactCount) { // diagnostic: contact set changes (walk-over evidence)
                            lastContactCount=contacts.size();
                            std::cout<<"World item contacts frame="<<drawn<<" count="<<contacts.size()<<" moving="<<(itemPlayer->action==f::CharacterAction::moving)<<" player="<<itemPlayer->transform.position[0]<<","<<itemPlayer->transform.position[1]<<'\n';
                        }
                        const auto due=worldItemContacts.advance(std::uint64_t(combatSession->player_id()),itemPlayer->action==f::CharacterAction::moving,contacts,
                            [&](f::loot::RuntimeWorldItemIdV1 id){f::loot::RuntimeWorldItemEntryV1 e;std::string err;return worldItems->inspect(id,e,err);});
                        for(const auto id:due)runWorldItemPickup(id,"walk-over");
                    }
                    // --pickup-frame is a scripted test hook (quiet batches), not a player input; it picks the current target.
                    const bool scheduledPickup=std::find(options.pickupFrames.begin(),options.pickupFrames.end(),int(drawn))!=options.pickupFrames.end();
                    if(scheduledPickup&&itemPlayer&&worldItemTarget!=f::loot::invalid_runtime_world_item_v1)
                        runWorldItemPickup(worldItemTarget,"scripted");
                    if(worldItemStatusFrames>0)--worldItemStatusFrames;
                }
                if(runtimeAudio) {std::string audioError;if(!runtimeAudio->after_update(audioError))std::cerr<<"Audio output diagnostic: "<<audioError<<'\n';}
                if(!gameplayPaused) {
                for(const auto& event:combatSession->original_animation_dispatches())
                    std::cout<<"Source animation frame="<<drawn<<" actor="<<event.actor<<" event="<<event.event
                             <<" service="<<event.service<<" name="<<event.authored_name<<" lag="<<event.lag_ms
                             <<" controllerBlocked="<<(globalControllerBlocked||characterControllerBlocked[event.actor])
                             <<" synchronousHit="<<event.synchronous_source_hit<<'\n';
                // Reaction/death and diagnostic leaf playback have no authored
                // movement service yet. Never leak their delta into locomotion.
                visual.take_root_motion();
                if(motor)motor->setFacingRadians(combatSession->actor(combatSession->player_id())->transform.rotation[2]);
                if(options.sourceHeadingRotation) {
                    auto* actor=combatSession->actor(combatSession->player_id());
                    const double milliseconds=gameplayDt*1000+sourceRotationFractionMs;const auto unsignedMs=std::uint32_t(milliseconds);sourceRotationFractionMs=milliseconds-unsignedMs;
                    if(actor->action==f::CharacterAction::moving) {
                        if(!sourcePhysicalFrameEnabled&&(!sourceMoveFocusActive||!actor->source_flags520)) {
                            f::OriginalMotionPrefixResult prefix;
                            if(!f::original_motion_focus_prefix(f::OriginalMotionFocusPrefix::move,actor->source_flags520,actor->source_movement_type,0,prefix,error))throw std::runtime_error(error);
                            sourceMoveFocusActive=true;
                        }
                        f::OriginalManualHeadingBindings heading;heading.controller_lease=combatSession;
                        heading.read_heading=[&](auto& out,std::string&){const auto& h=motor->state().desiredHeading;out={{h.direction.x,h.direction.y,h.direction.z},h.sourceAngleRadians,unsigned(h.active),0};return true;};
                        heading.publish_heading=[&](const auto& value,std::string& e){if(!motor->setDesiredHeading({value.direction[0],value.direction[1],value.direction[2]})){e="Source heading publication rejected";return false;}return true;};
                        heading.source_policy=[&](auto& flags,auto& validate,std::string& e){if(!actor->source_flags520||!actor->source_validate_boundary452){e="Actual source motion cells unavailable";return false;}flags=*actor->source_flags520;validate=*actor->source_validate_boundary452;return true;};
                        heading.skip_boundary=[&](auto& skip,std::string& e){bool value;if(!sourceScopes->debug_switch("DisableWallAvoidance",value,e))return false;skip=unsigned(value);return true;};
                        const auto before=motor->state().desiredHeading.direction;f::OriginalManualHeadingResult result;
                        if(!nativeBodies.update_manual_heading(actor->id,heading,result,error))throw std::runtime_error("Source manual heading: "+error);
                        if(result.boundary_checked)++sourceHeadingChecks;
                        const auto after=motor->state().desiredHeading.direction;if(before.x!=after.x||before.y!=after.y)++sourceHeadingSlides;
                        std::uint32_t* turn=nullptr;if(!combatSession->borrow_rotation_turn(actor->id,turn,error))throw std::runtime_error(error);
                        const auto* properties=combatSession->world()->combat_properties(actor->id);
                        f::ActorMovement::SourceRotationBorrow borrow{actor->transform.rotation.data(),&motor->state().desiredHeading.sourceAngleRadians,turn};
                        auto sync=[&](const float* angles,std::string& e){if(angles!=actor->transform.rotation.data()){e="Visual rotation publication used another actor";return false;}sourceVisualYaw=angles[2];return true;};
                        if(!motor->apply_source_character_rotation_late(borrow,&*actor->source_flags520,properties->sheets.resolved.data(),unsignedMs,visual.loaded(),sync,error))throw std::runtime_error("Source late rotation: "+error);
                        ++sourceLateRotations;
                    } else {
                        // Unsupported focus tails remain unknown. Target turning
                        // keeps its existing scoped source kernel path.
                        sourceMoveFocusActive=false;
                        if(!sourcePhysicalFrameEnabled){actor->source_flags520.reset();actor->source_movement_type.reset();}
                        sourceVisualYaw=actor->transform.rotation[2];
                    }
                }
                for(const auto& boundary:combatSession->combo_boundaries())
                    std::cout<<"Source combo boundary frame="<<drawn<<" actor="<<boundary.actor<<" generation="<<boundary.generation<<" begin="<<boundary.beginning<<" depth="<<boundary.depth<<" step="<<boundary.step<<" count="<<boundary.count<<" clip="<<boundary.source_clip<<'\n';
                for(const auto& event:combatSession->events()) {
                    std::cout<<"Damage frame="<<drawn<<" attacker="<<event.attacker<<" target="<<event.target<<" marker="<<event.marker_name<<" removed="<<event.health_removed<<" dead="<<event.target_died<<'\n';
                    if(const auto* sourceAttack=combatSession->source_attack_state(event.attacker))
                        std::cout<<"Source combo hit frame="<<drawn<<" actor="<<event.attacker<<" index="<<sourceAttack->index<<" last="<<sourceAttack->last<<" continued="<<sourceAttack->continued<<'\n';
                }
                for(const auto& diagnostic:combatSession->world()->take_resolution_observer_errors())
                    std::cerr<<"Combat presentation diagnostic: "<<diagnostic<<'\n';
                }
            }
            if(!equipment.attachments().empty()&&!equipment.update(visual,error))throw std::runtime_error("Equipment pose: "+error);
            if(runtimeEquipmentAttachments&&(!runtimeEquipment||!runtimeEquipment->sample_render_pose(error)))throw std::runtime_error("Runtime equipment pose: "+error);
            for(auto& actor:population.actors())if(!gameplayPaused&&actor.enabled&&(!combatSession||!combatSession->owns_population_pose(actor.definition.stableId))&&!actor.visual.update(gameplayDt,error))throw std::runtime_error("Actor pose: "+error);
            // P16 containers: advance activating clips; the authored 'opened' marker is DoOpen (loot via T4, Lua OnOpen later).
            if(!gameplayPaused){std::vector<f::containers::ContainerEventV1> containerEvents;std::string containerError;if(!containerRuntime.update(gameplayDt,containerEvents,containerError))throw std::runtime_error("Container update: "+containerError);for(const auto& event:containerEvents){std::cout<<"Container opened declaration="<<event.name<<" loot="<<event.loot_id<<" clipMs="<<event.elapsed_ms<<" frame="<<drawn<<'\n';runContainerOpen(event.index,drawn);}
                if(combatSession){std::string persistError;if(!f::containers::persist_container_world_state_v1(*combatSession->world(),containerRuntime,persistError))throw std::runtime_error("Container persist: "+persistError);}}
            if(options.sourceTargetPosition) {
                for(const auto& entry:combatSession->world()->actors()) {
                    auto* actor=combatSession->actor(entry.first);auto* graphics=actorVisual(entry.first);
                    f::OriginalTargetPositionUpdateResult result;
                    auto absolute=[&](std::uintptr_t node,f::OriginalTargetPosition& out,std::string& e){
                        f::Mat4 local;if(!graphics->source_target_node_world(node,local,e))return false;
                        f::Vec3 scale=actorScale;float yaw=options.sourceHeadingRotation?sourceVisualYaw:(motor?motor->state().facingRadians:0);
                        if(actor->id!=combatSession->player_id()) {
                            const auto placed=std::find_if(population.actors().begin(),population.actors().end(),[&](const auto& value){return value.definition.stableId==actor->id;});
                            if(placed==population.actors().end()){e="Target node lost its actual visual placement";return false;}
                            scale={std::hypot(placed->transform[0],placed->transform[1]),std::hypot(placed->transform[4],placed->transform[5]),placed->transform[10]};yaw=actor->transform.rotation[2];
                        }
                        // Same compensated model pose and actor placement used
                        // by the renderer; no combat-target position is borrowed.
                        const float c=std::cos(yaw),s=std::sin(yaw);
                        out={actor->transform.position[0]+c*scale.x*local[12]-s*scale.y*local[13],
                             actor->transform.position[1]+s*scale.x*local[12]+c*scale.y*local[13],
                             actor->transform.position[2]+scale.z*local[14]};return true;
                    };
                    if(!f::original_update_target_position(actor->source_target_node180,actor->source_target_position184,absolute,result,error))throw std::runtime_error("Source target cache: "+error);
                    sourceTargetNodeQueries+=result.node_queried;sourceTargetCacheWrites+=result.cache_written;
                }
            }
            renderer.resize(window.width(),window.height());
            if(options.sourceCamera) {
                const double elapsedMs=gameplayDt*1000+cameraFractionMs;
                const auto wholeMs=std::uint32_t(elapsedMs);cameraFractionMs=elapsedMs-wholeMs;
                auto anchor=f::CameraVec3{options.actorPosition.x,options.actorPosition.y,options.actorPosition.z};
                if(actorCameraAnchor.initialized()) {
                    if(!actorCameraAnchor.update(actorCameraFrame(),wholeMs,error))throw std::runtime_error("Actor camera anchor update: "+error);
                    anchor=actorCameraAnchor.position();
                }
                bool damping=true;
                if(sourceCameraTargets.target()!=0){f::CampaignCameraFrame frame;if(!sourceCameraTargets.tick(wholeMs,frame,error))throw std::runtime_error("Source camera target: "+error);lastSourceCameraFrame=frame;if(frame.hasTarget){anchor=frame.anchor;damping=frame.applyDamping;}}
                if(!originalCamera.update_anchor(anchor,wholeMs,damping,error))throw std::runtime_error("Camera update: "+error);
            }
            if(playerSourceBox&&!f::OriginalActorBounds::update_absolute(*playerSourceBox,options.actorPosition,error))throw std::runtime_error("Source body absolute bounds: "+error);
            if(options.sourceNativeBodies&&!sourcePhysicalFrameEnabled)for(const auto& entry:sourceBodyPlans){auto* actor=combatSession->actor(entry.first);if(!nativeBodies.set_position(entry.first,actor->transform.position,false,error))throw std::runtime_error("Native body position sync: "+error);}
            auto activeCamera=camera(options.sourceCamera?campaignHost.source_camera_pose(originalCamera.pose()):(useTimeline?timeline.sample():freeCamera.pose()));
            if(options.sourceCamera){if(window.width()!=previousWidth||window.height()!=previousHeight){sourceProjectionAspect=float(window.width())/window.height();previousWidth=window.width();previousHeight=window.height();}activeCamera.nearPlane=originalCamera.config().nearPlane;activeCamera.farPlane=originalCamera.config().farPlane;activeCamera.aspectRatio=sourceProjectionAspect;}
            sourceEffectsCamera=activeCamera;
            if(sourceEffectsFactory&&!sourceEffectsPresentationFailed) {
                std::string effectError;bool prepared=true;
                if(!gameplayPaused) {
                    const double elapsed=gameplayDt*1000+sourceEffectsFractionMs;
                    const auto integerMs=std::int32_t(elapsed);sourceEffectsFractionMs=elapsed-integerMs;
                    if(sourceEffectsMs>std::numeric_limits<std::int32_t>::max()-integerMs){effectError="Source FX gameplay clock exceeds supported range";prepared=false;}
                    else {sourceEffectsMs+=integerMs;prepared=sourceEffectsFactory->runtime().update(std::uint64_t(drawn)+1,sourceEffectsMs,integerMs,effectError);}
                }
                std::shared_ptr<const f::effects::EffectRenderFrame> frame;
                if(prepared)prepared=sourceEffectsFactory->runtime().prepare_render_frame(frame,effectError);
                if(prepared&&frame&&!frame->packets.empty()) {
                    ++sourceEffectsPacketFrames;sourceEffectsPackets+=frame->packets.size();
                    prepared=sourceEffectsRenderer.enqueue(frame,effectError);
                    if(sourceEffectsPacketFrames<=3)std::cout<<"Source FX frame="<<drawn<<" packets="<<frame->packets.size()<<" gameplayMs="<<sourceEffectsMs<<" textures="<<sourceEffectsRenderer.texture_uploads()<<'\n';
                }
                if(!prepared){sourceEffectsPresentationFailed=true;std::cerr<<"Source FX presentation diagnostic frame="<<drawn<<": "<<effectError<<'\n';}
            }
            if(options.combatText) {
                combatTextCamera=activeCamera;
                const double elapsed=gameplayDt*1000+combatTextFractionMs;const auto integerMs=std::uint32_t(elapsed);combatTextFractionMs=elapsed-integerMs;
                if(!combatText.after_host_update(std::uint64_t(drawn),integerMs,window.width()/480.f,window.height()/320.f,error))throw std::runtime_error("Combat text update: "+error);
            }
            // P16 MAP: RoomZone visits for the CURRENT level (RoomZone::Update rule, features/map_visit): a zone in the
            // gameplay camera frustum that contains the local player (inclusive XY) becomes visited, once. Saved via schema v4.
            if(!levelModuleZones.empty()&&combatSession) {
                if(!mapVisitsReady) {
                    std::string mapError;
                    if(!mapVisits.configure(levelModuleZones,f::map_visit::visited_module_ids(state,options.level.generic_string()),mapError))throw std::runtime_error("Map room zones: "+mapError);
                    mapVisitsReady=true;
                    std::cout<<"Map room zones level="<<options.level.generic_string()<<" modules="<<levelModuleZones.size()<<" visited="<<mapVisits.visited_count()<<'\n';
                    for(const auto& zone:levelModuleZones)
                        std::cout<<"Map room zone id="<<zone.id<<" name="<<zone.name<<" bounds="<<zone.bounds[0]<<','<<zone.bounds[1]<<','<<zone.bounds[2]<<" -> "<<zone.bounds[3]<<','<<zone.bounds[4]<<','<<zone.bounds[5]<<" ranges="<<zone.firstRange<<'+'<<zone.rangeCount<<'\n';
                }
                f::map_visit::CameraBasisV1 mapBasis;std::string mapError;
                if(!f::map_visit::camera_basis_v1(activeCamera,float(window.width())/float(window.height()),mapBasis,mapError))throw std::runtime_error("Map camera basis: "+mapError);
                std::optional<std::array<float,3>> mapPlayer;
                if(const auto* playerActor=combatSession->actor(combatSession->player_id()))
                    mapPlayer=std::array<float,3>{playerActor->transform.position[0],playerActor->transform.position[1],playerActor->transform.position[2]};
                const auto newlyVisited=mapVisits.update(f::map_visit::frustum_planes_v1(mapBasis),mapPlayer);
                if(!newlyVisited.empty()) {
                    f::map_visit::record_visited_modules(state,options.level.generic_string(),newlyVisited);
                    std::cout<<"Map room visited frame="<<drawn<<" level="<<options.level.generic_string()<<" modules=";
                    for(const auto id:newlyVisited)std::cout<<id<<' ';
                    std::cout<<"total="<<mapVisits.visited_count()<<'\n';
                }
            }
            renderer.beginFrame(activeCamera);
            auto actorWorld=f::identity();float angle=options.sourceHeadingRotation?sourceVisualYaw:(motor?motor->state().facingRadians:0),c=std::cos(angle),s=std::sin(angle);
            actorWorld[0]=c*actorScale.x;actorWorld[1]=s*actorScale.x;actorWorld[4]=-s*actorScale.y;actorWorld[5]=c*actorScale.y;actorWorld[10]=actorScale.z;
            actorWorld[12]=options.actorPosition.x;actorWorld[13]=options.actorPosition.y;actorWorld[14]=options.actorPosition.z;
            f::RenderQueue queue;
            if(!scene.mesh.vertices.empty())queue.submit(scene.mesh);
            for(const auto& mesh:visual.meshes())queue.submit(mesh,actorWorld);
            const auto& displayedEquipment=runtimeEquipmentAttachments?runtimeEquipmentAttachments->attachments():equipment.attachments();
            for(const auto& attached:displayedEquipment) {
                f::Mat4 world{};for(unsigned col=0;col<4;++col)for(unsigned row=0;row<4;++row)for(unsigned k=0;k<4;++k)world[col*4+row]+=actorWorld[k*4+row]*attached.socket_world[col*4+k];
                for(std::size_t meshIndex=0;meshIndex<attached.visual.meshes().size();++meshIndex) {
                    const auto& mesh=attached.visual.meshes()[meshIndex];
                    if(runtimeEquipmentAttachments) {
                        const auto& material=attached.visual.original_materials()[meshIndex];
                        const bool blue=material.effectFile=="GL_Diffuse_L1_VC_iPhone.bdae"&&material.technique=="L1_Vc_Al_----_----_----_----";
                        const auto texture=loadTexture(material.diffuse,material.alphaMap,blue);
                        if(!queue.submit(mesh,world,std::vector<std::uint32_t>(mesh.ranges.empty()?1:mesh.ranges.size(),texture),error))throw std::runtime_error(error);
                    } else queue.submit(mesh,world);
                }
            }
            for(const auto& actor:population.actors()) {
                if(!actor.enabled)continue;
                auto placement=actor.transform;
                if(const auto* live=combatSession?combatSession->actor(actor.definition.stableId):nullptr) {
                    const float sx=std::hypot(placement[0],placement[1]),sy=std::hypot(placement[4],placement[5]);
                    const float yaw=live->transform.rotation[2];placement[0]=std::cos(yaw)*sx;placement[1]=std::sin(yaw)*sx;placement[4]=-std::sin(yaw)*sy;placement[5]=std::cos(yaw)*sy;
                    placement[12]=live->transform.position[0];placement[13]=live->transform.position[1];placement[14]=live->transform.position[2];
                }
                for(const auto& mesh:actor.visual.meshes())queue.submit(mesh,placement);
            }
            // P16 containers: visuals at the authored transform (closed or animated pose from ContainerRuntimeV1).
            for(const auto& view:containerRuntime.views())if(view.visual&&view.instance)for(const auto& mesh:view.visual->meshes())queue.submit(mesh,view.instance->transform);
            // B004/B029: rendered marker = last target, else OOI, gated by eligibility (combat target is not used).
            const auto* markerTarget=combatSession?combatSession->actor(f::rendered_target_marker_actor_v1(*combatSession,combatSession->player_id(),objectOfInterest)):nullptr;
            if(const auto* target=markerTarget;target&&target->alive()&&!targetMarker.mesh.vertices.empty()) {
                auto placement=f::identity();placement[12]=target->transform.position[0];placement[13]=target->transform.position[1];placement[14]=target->transform.position[2];
                const auto original=std::find_if(population.actors().begin(),population.actors().end(),[&](const auto& actor){return actor.definition.stableId==target->id;});
                if(original!=population.actors().end()){placement[0]=std::hypot(original->transform[0],original->transform[1]);placement[5]=std::hypot(original->transform[4],original->transform[5]);placement[10]=original->transform[10];}
                queue.submit(targetMarker.mesh,placement);
            }
            // P14 DROPS: world items drawn from the original itemdrops.bdae packets in the normal queue.
            if(worldDrops&&worldItems&&worldItems->size()) {
                std::string dropError;
                if(worldDrops->prepare(dropError)) {
                    if(const auto* dropFrame=worldDrops->frame()) {
                        for(const auto& draw:dropFrame->draws)if(draw.mesh&&draw.source_pass_ready)queue.submit(*draw.mesh,draw.world);
                        static std::set<std::string> reportedDropIssues;
                        for(const auto& skipped:dropFrame->unresolved)
                            if(reportedDropIssues.insert(skipped.visual_uri+"|"+skipped.reason).second)
                                std::cerr<<"World item not drawn visual="<<skipped.visual_uri<<" item="<<skipped.item_id<<": "<<skipped.reason<<'\n';
                        if(!dropFrame->renderer_ready&&reportedDropIssues.insert("pass|"+std::to_string(dropFrame->draws.size())).second)
                            std::cerr<<"World item source pass unresolved for "<<dropFrame->draws.size()<<" draw(s); those meshes are not drawn\n";
                        static std::size_t lastDrawCount=~std::size_t(0);
                        if(lastDrawCount!=dropFrame->draws.size()){lastDrawCount=dropFrame->draws.size();std::cout<<"World item draws frame="<<drawn<<" count="<<lastDrawCount<<" store="<<worldItems->size()<<'\n';}
                    }
                } else {static std::string lastDropError;if(lastDropError!=dropError){lastDropError=dropError;std::cerr<<"World item presentation diagnostic frame="<<drawn<<": "<<dropError<<'\n';}}
            }
            queue.flush(renderer,activeCamera);
            if(!sourceEffectsRenderer.draw_queued(error))throw std::runtime_error("Source FX draw: "+error);
            if(options.hud) {
                auto frame=[&](unsigned current,unsigned maximum){auto n=std::int32_t(std::uint32_t(actorProperties.sheets.resolved[current])*100u);auto d=actorProperties.sheets.resolved[maximum];if(!d)throw std::runtime_error("Unbound HUD maximum");return unsigned(std::clamp(int(std::int64_t(n)/d)-1,0,99));};
                auto liveFrame=[](float current,float maximum){if(!std::isfinite(current)||!std::isfinite(maximum)||maximum<=0)throw std::runtime_error("Unbound live HUD maximum");return unsigned(std::clamp(int(current*100/maximum)-1,0,99));};
                const auto* player=combatSession?combatSession->actor(combatSession->player_id()):nullptr;
                // B038: XP bar from the live player sheet (resolved 33 = XP, 34 = XP for level); empty without a player.
                unsigned xpFrame=0;
                if(player){const auto* xpSheet=combatSession->world()->combat_properties(combatSession->player_id());if(!xpSheet||!f::original_hud_xp_frame(xpSheet->sheets.resolved[33],xpSheet->sheets.resolved[34],xpFrame))throw std::runtime_error("Unbound live HUD XP sheet");}
                f::HudGeometry hud;if(!f::compose_original_hud(0,player?liveFrame(player->health,player->max_health):frame(36,38),player?liveFrame(player->resource,player->max_resource):frame(41,43),xpFrame,options.hudPortrait,hud,error))throw std::runtime_error(error);
                overlay.begin(window.width(),window.height());
                const float scale=float(window.height())/hud.height;
                if(campaignHost.hud_visible()) for(const auto& batch:hud.batches){std::vector<f::OverlayTriangleVertex> vertices; // P16 HOST: HideFlash HUD also hides the original HUD batches
for(const auto& v:batch.triangles)vertices.push_back({v.x*scale,v.y*scale,v.u,v.v});if(!overlay.drawTriangles(vertices,hudTexture))throw std::runtime_error("HUD triangle draw rejected");}
                updatePcHud();
                if(pcHudReady&&!characterMenu.is_open()&&!pauseMenuOpen&&campaignHost.hud_visible()) { // P16 HOST: HideFlash HUD
                    const float offset=(window.width()/scale-480.f)*.5f;
                    for(std::size_t i=0;i<pcHudPresentation.art.batches.size();++i) {
                        std::vector<f::OverlayTriangleVertex> vertices;
                        for(const auto& v:pcHudPresentation.art.batches[i].triangles)vertices.push_back({(v.x+offset)*scale,v.y*scale,v.u,v.v});
                        if(!overlay.drawTriangles(vertices,pcHudPresentation.art.bitmap_ids[i]?hudTexture:0,pcHudPresentation.art.batch_colors[i]))throw std::runtime_error("PC HUD draw rejected");
                    }
                    auto signature=std::to_string(window.width())+":"+std::to_string(window.height());
                    for(const auto& field:pcHudPresentation.art.text_fields)signature+=':'+field.initial_text;
                    if(pcHudTextSignature!=signature) {
                        auto art=pcHudPresentation.art;
                        const float xScale=480.f/(window.width()/scale);
                        for(auto& field:art.text_fields){field.matrix[0]*=xScale;field.matrix[2]*=xScale;field.matrix[4]=(field.matrix[4]+offset)*xScale;}
                        if(!pcHudText.rebuild(art,window.width(),window.height(),renderer,error))throw std::runtime_error("PC HUD text: "+error);
                        pcHudTextSignature=signature;
                    }
                    pcHudText.draw(overlay);
                }
                // P16 CINE: cinematic presentation. Placeholder box/SKIP quads in the PC HUD letterbox mapping;
                // caption and SKIP text through the frontend text owner (authored x pre-scaled like the PC HUD fields).
                if(campaignHost.enabled()) {
                    const auto cinFrame=campaignHost.cinematic().build_frame();
                    const auto view=f::cinematic_runner::viewport_for(float(window.width()),float(window.height()));
                    for(const auto& rect:cinFrame.rects) {
                        const float x0=(rect.x+view.offset)*view.scale,y0=rect.y*view.scale,x1=(rect.x+rect.w+view.offset)*view.scale,y1=(rect.y+rect.h)*view.scale;
                        const std::vector<f::OverlayTriangleVertex> quad{{x0,y0,0,0},{x1,y0,0,0},{x1,y1,0,0},{x0,y0,0,0},{x1,y1,0,0},{x0,y1,0,0}};
                        if(!overlay.drawTriangles(quad,0,rect.rgba))throw std::runtime_error("Cinematic box draw rejected");
                    }
                    const float xScale=480.f*view.scale/float(window.width());
                    f::frontend::art::ScreenArt cinArt;
                    auto cinSignature=std::to_string(window.width())+":"+std::to_string(window.height());
                    for(const auto& item:cinFrame.texts) {
                        f::frontend::art::TextField field;
                        field.font_id=5; // only mapped source font (Fontin SmallCaps); placeholder face, see report
                        field.source_height=14; field.rgba=item.rgba; field.align=0; field.margins={2,2,0}; field.leading=0;
                        field.matrix={xScale,0,0,1,0,0};
                        const float left=(item.x+view.offset)*xScale; // FrontendText maps left*width/480 back to window pixels
                        field.local_bounds={left,left+item.w*xScale,item.y,item.y+item.h};
                        field.bounds=field.local_bounds;
                        field.initial_text=item.text;
                        cinArt.text_fields.push_back(field);
                        cinSignature+=':'+item.text;
                    }
                    if(cinematicTextSignature!=cinSignature) {
                        if(!cinematicText.rebuild(cinArt,window.width(),window.height(),renderer,error))throw std::runtime_error("Cinematic text: "+error);
                        cinematicTextSignature=cinSignature;
                    }
                    cinematicText.draw(overlay);
                }
                if(const auto* target=markerTarget;target&&target->alive()) {
                    auto head=f::Vec3{target->transform.position[0],target->transform.position[1],target->transform.position[2]};
                    const auto original=std::find_if(population.actors().begin(),population.actors().end(),[&](const auto& actor){return actor.definition.stableId==target->id;});
                    if(original!=population.actors().end()){f::Vec3 lo{},hi{};if(!original->visual.indexed_bounds(lo,hi,error))throw std::runtime_error(error);head.z+=hi.z*original->transform[10];}
                    std::array<float,2> screen{};
                    if(project(activeCamera,head,window.width(),window.height(),screen)) {
                        f::HudTargetGeometry art;if(!f::compose_original_target_hud(liveFrame(target->health,target->max_health),art,error))throw std::runtime_error(error);
                        auto bounded=[](float value,float low,float high){return low>high?low:std::clamp(value,low,high);};
                        const float left=bounded(screen[0]-(art.bounds[0]+art.bounds[1])*.5f*scale,-art.bounds[0]*scale,window.width()-art.bounds[1]*scale);
                        const float top=bounded(screen[1]-art.bounds[3]*scale,-art.bounds[2]*scale,window.height()-art.bounds[3]*scale);
                        for(const auto& batch:art.art.batches){std::vector<f::OverlayTriangleVertex> vertices;for(const auto& v:batch.triangles)vertices.push_back({left+v.x*scale,top+v.y*scale,v.u,v.v});if(!overlay.drawTriangles(vertices,hudTexture))throw std::runtime_error("Target HUD triangle draw rejected");}
                        const auto* props=combatSession->world()->combat_properties(target->id);f::OriginalActorLabel label;
                        if(!props||!actorLabels.label({props->sheets.resolved[18],props->sheets.resolved[19],false,false,{}},label,error))throw std::runtime_error("Original target label: "+error);
                        for(const auto& field:art.text_fields) {
                            f::HudGlyphRun run;if(!targetFont.raster(field.role=="enemy_name"?label.name:label.level,int(field.source_height),scale,run,error)||!f::apply_original_target_font_layout(field.source_font,field.source_height,run,error))throw std::runtime_error(error);
                            std::array<float,2> baseline{};if(!f::layout_original_target_text(field,run.advance,baseline,error))throw std::runtime_error(error);
                            for(const auto& glyph:run.glyphs)if(glyph.width>0&&glyph.height>0) {
                                const auto key=glyphTextureKey(glyph,"glyph");
                                auto handle=textures.find(key);if(handle==textures.end()){auto texture=renderer.createTexture(glyph.image.width,glyph.image.height,glyph.image.rgba.data());if(!texture)throw std::runtime_error("Original glyph upload failed");handle=textures.emplace(key,texture).first;}
                                f::OverlaySprite sprite;sprite.x=left+(baseline[0]+field.matrix[0]*glyph.x+field.matrix[2]*glyph.y)*scale;sprite.y=top+(baseline[1]+field.matrix[1]*glyph.x+field.matrix[3]*glyph.y)*scale;
                                sprite.width=glyph.width*scale;sprite.height=glyph.height*scale;sprite.u1=glyph.u1;sprite.v1=glyph.v1;sprite.texture=handle->second;overlay.drawSprite(sprite);
                            }
                        }
                    }
                }
                // P14 DROPS: ItemObject::ShowTooltip name text (item rarity colour) over the targeted
                // ground item, plus the transient pickup / inventory-full status line.
                if(worldItems&&worldDrops&&!characterMenu.is_open()&&!pauseMenuOpen) {
                    f::loot::RuntimeWorldItemEntryV1 labelEntry;std::string labelError;
                    if(worldItemTarget!=f::loot::invalid_runtime_world_item_v1&&worldItems->inspect(worldItemTarget,labelEntry,labelError)&&labelEntry.authored_item) {
                        std::array<float,2> itemScreen{};
                        if(project(activeCamera,f::Vec3{labelEntry.source_position[0],labelEntry.source_position[1],labelEntry.source_position[2]},window.width(),window.height(),itemScreen)) {
                            f::InventoryItem shown;shown.definition_id=worldItems->tables().items().identifiers.at(std::size_t(labelEntry.source_outcome.item_id));shown.quantity=labelEntry.quantity;
                            std::string labelText;std::uint32_t labelRgb=0xFFFFFF;
                            if(labelEntry.authored_item->record.words[f::loot::item_word_type_v1]==f::loot::item_type_gold_v1)labelText=std::to_string(labelEntry.source_outcome.resolved_gold_value.value_or(0))+" gold";
                            else if(!itemDisplayName(shown,labelText,labelError))labelText=shown.definition_id;
                            worldDrops->item_color(labelEntry,labelRgb,labelError);
                            if(!drawScreenLabel(targetFont,labelText,labelRgb,12,itemScreen[0],itemScreen[1]-18.f*scale,scale,renderer,overlay,textures,labelError))throw std::runtime_error("World item label: "+labelError);
                        }
                    }
                    if(worldItemStatusFrames>0&&!worldItemStatus.empty()) {
                        std::string statusError;
                        if(!drawScreenLabel(targetFont,worldItemStatus,worldItemStatusRgb,14,window.width()*.5f,window.height()*.25f,scale,renderer,overlay,textures,statusError))throw std::runtime_error("World item status: "+statusError);
                    }
                }
                // P16 QUESTUI: quest banner over the HUD (placeholder panel; see report).
                if(questBanners.visible()&&!characterMenu.is_open()&&!pauseMenuOpen) {
                    std::string bannerError;
                    if(!drawQuestBanner(questBanners.current(),targetFont,renderer,overlay,textures,window.width(),window.height(),scale,bannerError))throw std::runtime_error("Quest banner: "+bannerError);
                }
                if(options.combatText&&!characterMenu.is_open()) {
                    if(!combatText.draw(window.width()/480.f,window.height()/320.f,error))throw std::runtime_error("Combat text draw: "+error);
                    if(combatText.active_count())++combatTextDrawnFrames;
                }
                if(!characterMenu.is_open())drawPauseArt(pauseHudArt,false,nullptr);
                if(pauseMenuOpen)drawPauseArt(currentPauseArt(),true,nullptr);
                if(characterMenu.is_open()) {
                    // Character pages occupy the full viewport. Original SWF
                    // bitmap transparency must not expose gameplay behind them.
                    const float menuW=float(window.width()),menuH=float(window.height());
                    const std::vector<f::OverlayTriangleVertex> backdrop{
                        {0,0,0,0},{menuW,0,0,0},{menuW,menuH,0,0},
                        {0,0,0,0},{menuW,menuH,0,0},{0,menuH,0,0}};
                    if(!overlay.drawTriangles(backdrop,0,{0,0,0,1}))throw std::runtime_error("Fullscreen character menu backdrop draw rejected");
                    auto& bindings=characterMenuBindings;bindings.actor=player;bindings.properties=combatSession->world()->combat_properties(player->id);bindings.character=&state;
                    const auto classRow=std::find(properties.characters.names.begin(),properties.characters.names.end(),player->definition_id);
                    if(classRow==properties.characters.names.end())throw std::runtime_error("Original class header has no same player source row");
                    if(!menuLocalization.character_class_level(properties.characters,static_cast<std::int32_t>(classRow-properties.characters.names.begin()),&state,bindings.class_label,error))throw std::runtime_error("Original class header: "+error);
                    bindings.text=[&](const std::string& path,std::string& value,std::string& e){return menuLocalization.label(path,&state,value,e);};
                    // P16 QUESTUI: the Quest Log page is refreshed from the CQPG while its tab is open.
                    if(questMenuBinding&&characterMenu.is_open()&&characterMenu.tab()==f::character_menu::Tab::quest) {
                        std::string questMenuError;
                        if(!questMenuBinding->load_progress_from_character(questMenuError)||!questMenuBinding->show(0,0,f::CharacterQuestCategoryV1::assigned,questMenuError))
                            std::cerr<<"Quest Log refresh diagnostic: "<<questMenuError<<'\n';
                    }
                    f::character_menu::Frame menu;if(!characterMenu.frame(bindings,window.width(),window.height(),menu,error))throw std::runtime_error("Character menu: "+error);
                    const auto& transform=menu.transform;
                    auto drawMenuSolid=[&](const f::character_menu::MenuSolidBatch& solid) {
                        std::vector<f::OverlayTriangleVertex> vertices;
                        for(const auto& v:solid.geometry.triangles)vertices.push_back({transform.x+v.x*transform.scale_x,transform.y+v.y*transform.scale_y,v.u,v.v});
                        if(!overlay.drawTriangles(vertices,0,solid.rgba))throw std::runtime_error("Character menu original solid draw rejected");
                    };
                    // P16 MAP: Map page. The visited level is drawn top-down through the map camera inside the authored
                    // RenderMap rectangle (after that contour, see the batch loop), then the player marker (family 3).
                    if(characterMenu.take_map_reset_zoom())mapView=f::map_visit::map_reset_zoom_v1(mapView);
                    // P16 MAP icon: the authored icon art of an icon type (MapIconsDynamic frame), origin at (x,y) window px.
                    constexpr float mapIconScale=1.55f;// authored MapIconsDummy/legend icon scale (1.55)
                    const auto drawMapIcon=[&](unsigned type,float x,float y) {
                        std::vector<f::OverlayTriangleVertex> vertices;
                        for(const auto& batch:f::character_menu::original_map_icon_art(type).batches)
                            for(const auto& v:batch.triangles)vertices.push_back({x+v.x*mapIconScale*transform.scale_x,y+v.y*mapIconScale*transform.scale_y,v.u,v.v});
                        if(!vertices.empty()&&!overlay.drawTriangles(vertices,hudTexture))throw std::runtime_error("Map icon draw rejected");
                    };
                    const auto drawMapPage=[&]() {
                        if(!mapVisitsReady)return;
                        float minX=std::numeric_limits<float>::max(),minY=minX,maxX=-minX,maxY=-minX;bool found=false;
                        for(const auto& solid:menu.solids)if(solid.geometry.role=="menu_MapSheet/RenderMap/1")
                            for(const auto& v:solid.geometry.triangles){found=true;minX=std::min(minX,v.x);minY=std::min(minY,v.y);maxX=std::max(maxX,v.x);maxY=std::max(maxY,v.y);}
                        if(!found)throw std::runtime_error("Map page RenderMap rectangle is absent from the authored menu art");
                        const float rectX=transform.x+minX*transform.scale_x,rectY=transform.y+minY*transform.scale_y;
                        const float rectW=(maxX-minX)*transform.scale_x,rectH=(maxY-minY)*transform.scale_y;
                        const f::map_visit::MapRectV1 mapRect{rectX,rectY,rectW,rectH};
                        // Sheet parchment (SWF shape 600 bitmap fill = menus/map_bottom.tga) over the RenderMap rectangle.
                        {
                            const auto& source=f::character_menu::original_map_parchment();
                            if(!mapParchmentTexture) {
                                f::TextureImage image;std::string textureError;
                                if(!f::load_texture(assets.resolve(source.texture),image,textureError))throw std::runtime_error("Map parchment texture: "+textureError);
                                mapParchmentTexture=renderer.createTexture(int(image.width),int(image.height),image.rgba.data());
                                if(!mapParchmentTexture)throw std::runtime_error("Map parchment texture upload rejected");
                                mapParchmentTexelsW=float(image.width);mapParchmentTexelsH=float(image.height);
                            }
                            // Bitmap texel = (twips - offset) / twips-per-texel; the texture is 1024 px BTEX (width/height from the file).
                            const auto texU=[&](float ax){return ((ax*20.0f-source.offset_x_twips)/source.twips_per_texel)/mapParchmentTexelsW;};
                            const auto texV=[&](float ay){return ((ay*20.0f-source.offset_y_twips)/source.twips_per_texel)/mapParchmentTexelsH;};
                            const float px0=transform.x+source.x0*transform.scale_x,px1=transform.x+source.x1*transform.scale_x;
                            const float py0=transform.y+source.y0*transform.scale_y,py1=transform.y+source.y1*transform.scale_y;
                            const std::vector<f::OverlayTriangleVertex> parchment{
                                {px0,py0,texU(source.x0),texV(source.y0)},{px1,py0,texU(source.x1),texV(source.y0)},{px1,py1,texU(source.x1),texV(source.y1)},
                                {px0,py0,texU(source.x0),texV(source.y0)},{px1,py1,texU(source.x1),texV(source.y1)},{px0,py1,texU(source.x0),texV(source.y1)}};
                            if(!overlay.drawTriangles(parchment,mapParchmentTexture))throw std::runtime_error("Map parchment draw rejected");
                        }
                        std::optional<std::array<float,3>> mapPlayer;
                        if(const auto* playerActor=combatSession?combatSession->actor(combatSession->player_id()):nullptr)
                            mapPlayer=std::array<float,3>{playerActor->transform.position[0],playerActor->transform.position[1],playerActor->transform.position[2]};
                        f::Camera mapCamera;std::string mapError;
                        if(!f::map_visit::map_camera_v1(f::map_visit::map_extent_v1(mapVisits.zones()),mapPlayer,mapView,mapCamera,mapError))throw std::runtime_error("Map camera: "+mapError);
                        const int vx=int(std::floor(rectX)),vyTop=int(std::floor(rectY)),vr=int(std::ceil(rectX+rectW)),vb=int(std::ceil(rectY+rectH));
                        if(!renderer.withViewport(vx,window.height()-vb,vr-vx,vb-vyTop,mapCamera,[&] {
                            // Visited modules only (Module::visited3fc). Original map shows visited room geometry as dark slate.
                            for(const auto& zone:mapVisits.zones()) {
                                if(!mapVisits.visited(zone.id))continue;
                                for(std::size_t r=zone.firstRange;r<zone.firstRange+zone.rangeCount&&r<scene.mesh.ranges.size();++r) {
                                    f::DrawRange range=scene.mesh.ranges[r];
                                    range.material.texture=0;range.material.transparent=false;range.material.alphaReference=0;
                                    range.material.additive=false;range.material.lightingEnabled=false;range.material.sourcePass.reset();
                                    range.material.color={0.23f,0.25f,0.27f,1.0f};
                                    renderer.drawRange(scene.mesh,range);
                                }
                            }
                        })) throw std::runtime_error("Map page viewport rejected");
                        if(mapPlayer) {
                            float px=0,py=0;
                            // Family 3 (local player) = the authored Character icon (frame 3 of MapIconsDynamic).
                            if(f::map_visit::map_project_v1(mapCamera,mapRect,*mapPlayer,px,py))drawMapIcon(3,px,py);
                        }
                    };
                    for(const auto& solid:menu.solids)if(solid.after_bitmap_role.empty())drawMenuSolid(solid);
                    const auto& sourcePanes=f::inventory::original_inventory_character_panes_v1();
                    std::vector<bool> drawnPanes(sourcePanes.size(),false);
                    const auto drawEquipmentPane=[&](const f::inventory::SourceCharacterPaneV1& pane) {
                        if(!runtimeEquipment->with_preview_packets(equipmentRenderBindings.callbacks(),[&](const auto& frame,std::string& e) {
                            const int x=int(std::floor(transform.x+pane.bounds[0]*transform.scale_x));
                            const int yTop=int(std::floor(transform.y+pane.bounds[1]*transform.scale_y));
                            const int right=int(std::ceil(transform.x+pane.bounds[2]*transform.scale_x));
                            const int bottom=int(std::ceil(transform.y+pane.bounds[3]*transform.scale_y));
                            const int width=right-x,height=bottom-yTop;
                            auto camera=frame.presentation.camera;
                            camera.aspectRatio=float(width)/float(height);
                            if(!renderer.withViewport(x,window.height()-bottom,width,height,camera,[&] {
                                for(const auto& packet:frame.packets->packets)
                                    renderer.draw(packet.mesh,multiplyRenderMatrices(frame.presentation.source_inventory_rebase,packet.world));
                            })) {e="Original Equipment avatar viewport rejected";return false;}
                            e.clear();return true;
                        },error))throw std::runtime_error("Original Equipment avatar: "+error);
                    };
                    // P16 MAP: the level is drawn after the sheet backdrop batch and before the MapSheet art (icons, legend, buttons).
                    bool mapDrawn=false;
                    for(const auto& batch:menu.art.batches) {
                        if(!mapDrawn&&characterMenu.tab()==f::character_menu::Tab::map&&batch.role.compare(0,14,"menu_MapSheet/")==0){drawMapPage();mapDrawn=true;}
                        if(characterMenu.tab()==f::character_menu::Tab::equipment&&runtimeEquipment)
                            for(std::size_t i=0;i<sourcePanes.size();++i)
                                if(!drawnPanes[i]&&!sourcePanes[i].before_role.empty()&&
                                   batch.role.compare(0,sourcePanes[i].before_role.size(),sourcePanes[i].before_role)==0) {
                                    drawEquipmentPane(sourcePanes[i]);drawnPanes[i]=true;
                                }
                        std::vector<f::OverlayTriangleVertex> vertices;for(const auto& v:batch.triangles)vertices.push_back({transform.x+v.x*transform.scale_x,transform.y+v.y*transform.scale_y,v.u,v.v});
                        if(!overlay.drawTriangles(vertices,hudTexture))throw std::runtime_error("Character menu original-art draw rejected");
                        for(const auto& solid:menu.solids)if(solid.after_bitmap_role==batch.role)drawMenuSolid(solid);
                    }
                    if(characterMenu.tab()==f::character_menu::Tab::map&&!mapDrawn)drawMapPage();
                    // P16 MAP legend: each legend caption's icon type at its authored position (legend popup frame).
                    if(characterMenu.tab()==f::character_menu::Tab::map&&characterMenu.map_legend_shown())
                        for(const auto& icon:f::character_menu::original_map_legend_icons())
                            drawMapIcon(icon.type,transform.x+icon.x*transform.scale_x,transform.y+icon.y*transform.scale_y);
                    if(characterMenu.tab()==f::character_menu::Tab::equipment&&runtimeEquipment&&
                       std::none_of(drawnPanes.begin(),drawnPanes.end(),[](bool drawn){return drawn;}))
                        throw std::runtime_error("Original Equipment active avatar display-list anchor is unavailable");
                    const auto drawMenuGlyph=[&](const f::character_menu::MenuTextField& field,
                                                 const std::array<float,2>& baseline,
                                                 const f::HudGlyphQuad& glyph,float cursor,
                                                 std::uint32_t texture,const std::array<float,4>& color) {
                        const auto& m=field.matrix;
                        const auto corner=[&](float x,float y,float u,float v) {
                            return f::OverlayTriangleVertex{
                                transform.x+(baseline[0]+m[0]*x+m[2]*y)*transform.scale_x,
                                transform.y+(baseline[1]+m[1]*x+m[3]*y)*transform.scale_y,u,v};
                        };
                        const float left=cursor+glyph.x,right=left+glyph.width;
                        const float top=glyph.y,bottom=top+glyph.height;
                        const auto a=corner(left,top,0,0);
                        const auto b=corner(right,top,glyph.u1,0);
                        const auto c=corner(right,bottom,glyph.u1,glyph.v1);
                        const auto d=corner(left,bottom,0,glyph.v1);
                        if(!overlay.drawTriangles({a,b,c,a,c,d},texture,color))
                            throw std::runtime_error("Original menu glyph affine draw rejected");
                    };
                    for(const auto& text:menu.text) {
                        const auto sourceFlags=std::find_if(std::begin(f::character_menu::source_text_flag_rows_v1),std::end(f::character_menu::source_text_flag_rows_v1),[&](const auto& row) {
                            return row.movie==f::character_menu::SourceMenuMovieV1::character_menu&&row.field_character==text.field.character_id;
                        });
                        if(sourceFlags!=std::end(f::character_menu::source_text_flag_rows_v1)&&(sourceFlags->raw_flags&0x6000)) {
                            f::character_menu::MenuTextLayoutFieldV1 field;
                            field.page_character=sourceFlags->page_character;field.field_character=sourceFlags->field_character;
                            field.local_rect=text.field.local_bounds;field.matrix=text.field.matrix;field.margins=text.field.margins;
                            field.alignment=text.field.align;field.source_height=text.field.source_height;field.paragraph_leading=text.field.leading;
                            field.source_font_character=static_cast<std::uint16_t>(text.field.font_id);
                            for(unsigned channel=0;channel<4;++channel)field.source_rgba[channel]=text.field.rgba[channel]/255.f;
                            const auto* font=f::character_menu::original_menu_font(text.field.font_id);
                            if(!font)throw std::runtime_error("Multiline menu font is unavailable");
                            field.font_descent=font->descent;field.font_leading=font->leading;field.define_font3=font->define_font3;
                            f::character_menu::MenuTextLayoutV1 layout;
                            if(!f::character_menu::menu_text_layout_v1(field,text.value,[&](std::string_view value,float& advance,std::string& e) {
                                f::HudGlyphRun measured;if(!targetFont.raster(std::string(value),int(field.source_height),transform.scale,measured,e))return false;
                                advance=measured.advance;return true;
                            },layout,error))throw std::runtime_error("Multiline menu text: "+error);
                            GLint oldClip[4];glGetIntegerv(GL_SCISSOR_BOX,oldClip);const bool wasClipped=glIsEnabled(GL_SCISSOR_TEST)==GL_TRUE;
                            const auto& m=field.matrix;
                            float left=std::numeric_limits<float>::max(),right=-left,top=left,bottom=-left;
                            for(float x:{field.local_rect[0],field.local_rect[1]})for(float y:{field.local_rect[2],field.local_rect[3]}) {
                                const float px=transform.x+(m[0]*x+m[2]*y+m[4])*transform.scale_x;
                                const float py=transform.y+(m[1]*x+m[3]*y+m[5])*transform.scale_y;
                                left=std::min(left,px);right=std::max(right,px);top=std::min(top,py);bottom=std::max(bottom,py);
                            }
                            const int x0=std::clamp(int(std::floor(left)),0,window.width()),x1=std::clamp(int(std::ceil(right)),0,window.width());
                            const int y0=std::clamp(int(std::floor(top)),0,window.height()),y1=std::clamp(int(std::ceil(bottom)),0,window.height());
                            glEnable(GL_SCISSOR_TEST);glScissor(x0,window.height()-y1,x1-x0,y1-y0);
                            for(const auto& line:layout.lines) {
                                float cursor=0;
                                for(const auto& styled:line.runs) {
                                    f::HudGlyphRun run;if(!targetFont.raster(styled.text,int(field.source_height),transform.scale,run,error))throw std::runtime_error(error);
                                    for(const auto& glyph:run.glyphs)if(glyph.width>0&&glyph.height>0) {
                                        const auto key=glyphTextureKey(glyph,"menu-glyph");
                                        auto handle=textures.find(key);if(handle==textures.end()){auto texture=renderer.createTexture(glyph.image.width,glyph.image.height,glyph.image.rgba.data());if(!texture)throw std::runtime_error("Menu multiline glyph upload failed");handle=textures.emplace(key,texture).first;}
                                        drawMenuGlyph(text.field,line.baseline,glyph,cursor,handle->second,styled.rgba);
                                    }
                                    cursor+=styled.advance;
                                }
                            }
                            glScissor(oldClip[0],oldClip[1],oldClip[2],oldClip[3]);if(!wasClipped)glDisable(GL_SCISSOR_TEST);
                            continue;
                        }
                        f::HudGlyphRun run;if(!targetFont.raster(text.value,int(text.field.source_height),transform.scale,run,error))throw std::runtime_error("Character menu glyphs: "+error);
                        std::array<float,2> baseline;if(!f::character_menu::layout_menu_text(text.field,run.advance,baseline,error))throw std::runtime_error(error);
                        for(const auto& glyph:run.glyphs)if(glyph.width>0&&glyph.height>0) {
                            const auto key=glyphTextureKey(glyph,"menu-glyph");
                            auto handle=textures.find(key);if(handle==textures.end()){auto texture=renderer.createTexture(glyph.image.width,glyph.image.height,glyph.image.rgba.data());if(!texture)throw std::runtime_error("Character menu glyph upload failed");handle=textures.emplace(key,texture).first;}
                            std::array<float,4> color{};for(unsigned channel=0;channel<4;++channel)color[channel]=text.field.rgba[channel]/255.f;
                            drawMenuGlyph(text.field,baseline,glyph,0,handle->second,color);
                        }
                    }
                    ++menuDrawn;
                }
                // Preview 15 B049: "Confirm character point allocation?" over the Character menu (original WarningBox frame).
                if(characterMenu.is_open()&&statConfirmOpen)drawPauseArt(pauseConfirmArt,true,"GAMEPLAYMENUS_POINTS_CONFIRM");
                overlay.end();
            }
            renderer.endFrame();
            if(!sourceEffectsRenderer.finish_and_drain(error))throw std::runtime_error("Source FX drain: "+error);
            ++drawn;
            if(options.frames&&drawn>=options.frames&&!options.capture.empty()) capture(options.capture,window.width(),window.height());
            window.swap();
            if(options.frames&&drawn>=options.frames) break;
            dh::foundation::platform_sleep_milliseconds(1);
        }
        std::cout<<"Source FX final packetFrames="<<sourceEffectsPacketFrames<<" packets="<<sourceEffectsPackets<<" textureUploads="<<sourceEffectsRenderer.texture_uploads()<<" presentationFailed="<<sourceEffectsPresentationFailed<<'\n';
        retireSourceEffects();
        if(options.combatText)std::cout<<"Combat text final results="<<combatTextResults<<" labels="<<combatTextLabels<<" drawnFrames="<<combatTextDrawnFrames<<" active="<<combatText.active_count()<<"; original source snapshots and styles, full CUI call ordering remains incomplete\n";
        if(runtimeAudio) {
            runtimeAudio->summary();std::string audioError;
            if(!runtimeAudio->shutdown(audioError))std::cerr<<"Audio shutdown diagnostic: "<<audioError<<'\n';
        }
        equipmentRenderBindings.clear_textures();
        for(const auto& item:textures) if(item.second) renderer.destroyTexture(item.second);
        std::cout<<"Rendered frames="<<drawn<<"; clean shutdown\n";
        std::cout<<"Character menu final opened="<<menuOpened<<" drawn="<<menuDrawn<<" open="<<characterMenu.is_open()<<"; original-art development panel, full menu mutations/AS flow remain incomplete\n";
        std::cout<<"Character menu gameplay pausedFrames="<<gameplayPausedFrames<<" updateSerial="<<(combatSession?combatSession->update_serial():0)<<'\n';
        if(locomotionLibrary&&combatSession)if(const auto* pose=combatSession->retained_player_pose())
            std::cout<<"Source player locomotion final actor="<<combatSession->player_id()<<" clip="<<pose->slots().at(pose->current_slot()).clip_id<<" timelineMs="<<pose->slots().at(pose->current_slot()).timeline.current_ms<<'\n';
        if(motor)std::cout<<"Actor final position="<<options.actorPosition.x<<','<<options.actorPosition.y<<','<<options.actorPosition.z<<" grounded="<<motor->state().grounded<<'\n';
        std::cout<<"Population final enabled="<<population.enabled_count()<<" deferredInitially="<<population.initial_deferred_count()<<" loaded="<<population.actors().size()<<'\n';
        if(lifecycleEnabled)for(const auto& placed:population.actors())if(const auto* lifecycle=actorLifecycle.status(placed.definition.stableId))std::cout<<"Lifecycle final actor="<<placed.definition.stableId<<" name="<<placed.definition.name<<" state="<<lifecycle->state<<" enabled="<<placed.enabled<<" physical="<<lifecyclePhysical[placed.definition.stableId]<<" collisions="<<lifecycleCollisions[placed.definition.stableId]<<'\n';
        if(actorCameraAnchor.initialized()){const auto p=actorCameraAnchor.position();std::cout<<"Actor camera anchor final="<<p.x<<','<<p.y<<','<<p.z<<'\n';}
        if(options.campaignTriggers)campaignHost.print_summary(std::cout); // P16 HOST
    if(sourceCameraTargets.target()!=0)std::cout<<"Source camera final target="<<sourceCameraTargets.target()<<" remaining="<<sourceCameraTargets.transition_remaining()<<" globalControllerBlocked="<<globalControllerBlocked<<" anchor="<<lastSourceCameraFrame.anchor.x<<','<<lastSourceCameraFrame.anchor.y<<','<<lastSourceCameraFrame.anchor.z<<" damping="<<lastSourceCameraFrame.applyDamping<<'\n';
        if(playerSourceBox){const auto& b=playerSourceBox->absolute_box;std::cout<<"Source body final absolute="<<b[0]<<','<<b[1]<<','<<b[2]<<','<<b[3]<<','<<b[4]<<','<<b[5]<<'\n';}
        if(sourceFloors&&combatSession)for(const auto& entry:sourceBodyPlans) {
            const auto* actor=combatSession->actor(entry.first);dh2::navigation::HeightHit hit{};hit.height=actor->transform.position[2];
            const auto status=dh2_nav_world_height(&hit,&sourceFloors->world()->collision_world,actor->transform.position.data(),0);
            if(status<0)throw std::runtime_error("Final original floor query rejected source world");
            std::cout<<"Source floor final actor="<<entry.first<<" hit="<<(status==1)<<" height="<<hit.height<<" room="<<hit.room<<" floor="<<hit.floor<<'\n';
        }
        if(options.sourceNativeBodies)for(const auto& entry:sourceBodyPlans) {
            const auto* actor=combatSession->actor(entry.first);const auto* physical=nativeBodies.physical(entry.first);
            f::OriginalTriggerActorBorrow source;if(!nativeBodies.actor_borrow(entry.first,source,error))throw std::runtime_error(error);
            std::cout<<"Native body final actor="<<entry.first<<" physical="<<bool(*source.physical2dc)<<" radius="<<physical->native().radius*100.f<<" pfUser="<<nativeBodies.navigation(entry.first)->user;
            if(physical->native().body){dh2::physical::NativeBodyObservation observed{};if(dh2_native_body_observe(&observed,&physical->native()))throw std::runtime_error("Native body observation failed");
                if(observed.position[0]!=actor->transform.position[0]*.01f||observed.position[1]!=actor->transform.position[1]*.01f)throw std::runtime_error("Native body differs from shared actor position");
                std::cout<<" physicsXY="<<observed.position[0]<<','<<observed.position[1]<<" sourcePositionMatch=1";}
            std::cout<<'\n';
            if(options.sourceFloorMotion) {
                const auto* nav=nativeBodies.navigation(entry.first);
                if(nav->user!=entry.first)throw std::runtime_error("Source navigation identity differs from shared actor");
                std::cout<<"Navigation final actor="<<entry.first<<" pfUser="<<nav->user<<" radius="<<nav->radius<<" room="<<nav->motion.room<<" floor="<<nav->motion.floor<<" position="<<nav->motion.position[0]<<','<<nav->motion.position[1]<<','<<nav->motion.position[2]<<'\n';
            }
        }
        for(const auto& entry:motionReceipts)std::cout<<"Motion final actor="<<entry.first<<" enabledSegments="<<entry.second.enabled<<" disabledSegments="<<entry.second.disabled<<" sourceXY="<<entry.second.sourceXY<<" worldXY="<<entry.second.worldXY<<'\n';
        for(const auto& entry:sourceMotionReceipts)std::cout<<"Source PF motion final actor="<<entry.first<<" solves="<<entry.second.solves<<" accepted="<<entry.second.accepted<<" clamped="<<entry.second.clamped<<'\n';
        if(options.sourceHeadingRotation)std::cout<<"Source manual heading final checks="<<sourceHeadingChecks<<" slides="<<sourceHeadingSlides<<" lateRotations="<<sourceLateRotations<<"; move focus prefix, full FSM/path/physics frame remains incomplete\n";
        if(sourcePhysicalFrameEnabled)std::cout<<"Source physical frame final steps="<<sourcePhysicalSteps<<" imports="<<sourcePhysicsImports<<" suppressedRoots="<<sourcePhysicsSuppressedRoots<<"; actual Session/NativeWorld physical projection, global original traversal fidelity unverified\n";
        if(options.sourceTargetPosition) {
            std::cout<<"Source own target cache final nodeQueries="<<sourceTargetNodeQueries<<" writes="<<sourceTargetCacheWrites<<"; same rendered hierarchy, full source frame remains incomplete\n";
            for(const auto& entry:combatSession->world()->actors()) {
                const auto& actor=entry.second;
                if(!actor.source_target_node180||!actor.source_target_position184)throw std::runtime_error("Source own target node/cache producer is missing");
                const auto& p=*actor.source_target_position184;
                std::cout<<"Source own target final actor="<<actor.id<<" nodePresent="<<bool(*actor.source_target_node180)<<" cache="<<p[0]<<','<<p[1]<<','<<p[2]<<'\n';
            }
        }
        if(combatSession)for(const auto& entry:combatSession->world()->actors())std::cout<<"Actor final id="<<entry.first<<" definition="<<entry.second.definition_id<<" HP="<<entry.second.health<<'/'<<entry.second.max_health<<" target="<<entry.second.target_id<<" action="<<int(entry.second.action)<<" position="<<entry.second.transform.position[0]<<','<<entry.second.transform.position[1]<<','<<entry.second.transform.position[2]<<'\n';
        if(combatSession)for(const auto& placed:population.actors()) {
            const auto policy=options.combat.profiles.find(placed.profileId);
            if(policy==options.combat.profiles.end()||!policy->second.animationOnly)continue;
            const auto* pose=combatSession->retained_actor_pose(placed.definition.stableId);
            std::cout<<"Animation-only final actor="<<placed.definition.stableId<<" profile="<<placed.profileId<<" enabled="<<placed.enabled<<" retained="<<bool(pose);
            if(pose){const auto slot=pose->current_slot();std::cout<<" slot="<<slot<<" clip="<<pose->slots().at(slot).clip_id<<" timelineMs="<<pose->slots().at(slot).timeline.current_ms;if(!pose->samples().empty())std::cout<<" sampleMs="<<pose->samples().back().current_ms;}
            std::cout<<'\n';
        }
        if(returnToFrontend) {
            pcHudText.clear(renderer);pauseText.clear(renderer);cinematicText.clear(renderer); // P16 CINE
            const int selectedSlot=options.selectedSaveSlot;
            options=launchOptions;options.startMode="menu";options.selectedSaveSlot=selectedSlot;
            options.menuActions.clear();
            if(!returnMenuScriptConsumed&&!launchOptions.menuReturnActions.empty()) {
                options.menuActions=launchOptions.menuReturnActions;
                options.menuReleases.clear(); // one diagnostic return, no repeated gameplay clicks
                returnMenuScriptConsumed=true;
            }
            options.verifyFrontendCreation=false;options.verifyFrontendSlots=false;
            continue;
        }
        pauseText.clear(renderer);
        pcHudText.clear(renderer);
        return 0;
        }
    } catch(const std::exception& e) {std::cerr<<"Foundation error: "<<e.what()<<'\n';return 1;}
}
