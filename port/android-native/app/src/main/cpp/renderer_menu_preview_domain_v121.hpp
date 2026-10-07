#pragma once
#include "native_menu_preview_v121.hpp"
#include "character_game_design.hpp"
#include "physical_world.hpp"
#include "hud_text_v1.hpp"
#include <android/asset_manager.h>
namespace dh2::data {class DesignSettingsOwner;class SkillTables;class EffectsTables;struct Dictionary;}
namespace dh2::character {struct DebugSwitches;struct DebugFileServices24;class CharacterCandidateCacheV62;}
namespace dh2::character {class CharacterAnimationSetCacheV6;}
namespace dh2::world {class CanonicalPropertyMapV1;}
namespace dh2::fx {class VisualFxManagerLibrariesV63;}
namespace dh2::world {struct CanonicalCharacterCandidateServicesV60;}
namespace model_renderer {
// Process resources for menu Characters. This is an independent native cache
// and service lifetime, never a fabricated Level or gameplay World.
struct MenuPreviewProcessDomainV121 {
 std::shared_ptr<void> owner;
 std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1> scene;
 std::shared_ptr<dh2::world::CanonicalObjectManagerV1> objects;
 std::shared_ptr<dh2::world::CanonicalPropertyMapV1> properties;
 std::shared_ptr<dh2::physical::NativeWorld> physical;
 std::shared_ptr<dh2::character::CharacterGameDesign> design;
 std::shared_ptr<dh2::data::DesignSettingsOwner> settings;
 std::shared_ptr<dh2::data::SkillTables> skills;
 std::shared_ptr<dh2::data::EffectsTables> effects;
 std::shared_ptr<dh2::data::Dictionary> animations;
 std::shared_ptr<dh2::character::CharacterCandidateCacheV62> character_cache;
 std::shared_ptr<dh2::character::CharacterAnimationSetCacheV6> animation_manager;
 std::shared_ptr<dh2::fx::VisualFxManagerLibrariesV63> fx_libraries;
 const std::uint32_t* controller_blocked{};
 dh2::ui::HudTextV1* localization{};
 dh2::ui::HudTextEnvironmentV1 text_environment;
 std::shared_ptr<void> text_owner;
 std::shared_ptr<dh2::character::DebugSwitches> debug;
 const dh2::character::DebugFileServices24* debug_files{};
 std::function<bool(const std::string&,bool&,std::vector<std::uint8_t>&,std::string&)> read,read_save;
 std::function<bool(std::vector<std::string>&,std::string&)> directory;
 // The same process resource transport supplies the ordinary canonical
 // constructor/InitPost providers. No gameplay World admission is implied.
 std::function<bool(std::int32_t,bool,dh2::world::CanonicalCharacterCandidateServicesV60&,std::string&)> character_services;
};
bool borrow_native_menu_preview_domain_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 AAssetManager*,const std::string&,MenuPreviewProcessDomainV121&,std::string&);
bool build_native_menu_preview_renderer_services_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 AAssetManager*,const std::string&,NativeMenuPreviewServicesV121&,std::string&);
bool create_native_menu_preview_player_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 AAssetManager*,const std::string&,std::int32_t,bool,MenuPreviewCharacterV121&,std::string&);
bool build_native_menu_preview_character_services_v122(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 const MenuPreviewProcessDomainV121&,std::int32_t,bool,dh2::world::CanonicalCharacterCandidateServicesV60&,std::string&);
bool borrow_native_menu_preview_text_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 dh2::ui::HudTextV1*&,dh2::ui::HudTextEnvironmentV1&,std::shared_ptr<void>&,std::string&);
}
