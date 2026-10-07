#pragma once
#include "application_services_owner_v5.hpp"
#include "gameobject_scene_root_registry_v1.hpp"
#include "gameplay_camera_factory_v16.hpp"
#include "menu_avatar_preview_v1.hpp"
#include <array>
#include <functional>
#include <memory>
#include <string>
namespace dh2::world {class CanonicalObjectManagerV1;}
namespace model_renderer {
//Actual retained resource/receiver loans. Identities address native source
//owners, never raw ARM32 addresses, copied actor poses, or mock GameObjects.
struct MenuPreviewSceneV121 {
 std::shared_ptr<void> owner;
 std::uintptr_t identity{};
 dh2::world::GameObjectSceneRootBorrowV1 root;
 std::function<bool(std::string&)> drop_constructor_reference;
};
struct MenuPreviewCharacterV121 {
 std::shared_ptr<void> owner;
 std::uintptr_t identity{},visual_root{};
 std::function<bool(std::uint32_t,std::string&)> sg_load;
 std::function<bool(std::string&)> attach_visual;
 std::function<bool(const std::array<float,3>&,std::string&)> visual_position;
 std::function<bool(const std::array<float,4>&,std::string&)> visual_rotation;
 std::function<bool(bool,std::string&)> visual_visible;
};
struct NativeMenuPreviewServicesV121 {
 std::shared_ptr<void> owner; //Independent actual cache/renderer domain; App weak.
 std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1> scene;
 std::shared_ptr<dh2::world::CanonicalObjectManagerV1> objects;
 dh2::camera::CameraFactoryBackendV16 camera_backend;
 std::function<bool(std::string&)> current,remove_all_scene_nodes,flush_objects,flush_animation_sets;
 std::function<bool(std::string&)> free_textures,clean_glitch;
 std::function<bool(float,float,float,float,std::string&)> load_physics;
 std::function<bool(const char*,MenuPreviewSceneV121&,std::string&)> construct_scene;
 //Actual native selection-scene owner; NULL is observed from that resource
 //domain, never guessed from an unwritten source CharacterSelect+c4 word.
 std::function<bool(std::uintptr_t&,std::string&)> selected_scene_root;
 std::function<bool(std::uintptr_t,std::string&)> remove_selected_root;
 std::function<bool(std::int32_t,bool&,std::string&)> save_exists;
 //Original CreatePlayer(slot,0,0,"PlayerCharacter_0",newProfile,0,...).
 //This must construct the canonical source Character/Save/visual receiver;
 //a base-model or starting-kit preview is insufficient for occupied slots.
 std::function<bool(std::int32_t,bool new_profile,MenuPreviewCharacterV121&,std::string&)> create_player;
 //SAME FrontUiSession MenuAvatarPreviewState slot, also used by NativeSetSlot.
 dh2::ui::MenuAvatarPreviewStateV1* avatar_state{};
 std::shared_ptr<void> avatar_state_owner;
 std::function<bool(std::int32_t,std::string&)> stop_music;
};
//Root renderer supplies its genuine Scene/ObjectManager/cache/render owners.
//The preview lifecycle implementation never creates a replacement World/Level.
bool build_native_menu_preview_services_v121(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 NativeMenuPreviewServicesV121&,std::string&);
class NativeMenuPreviewV121;
bool borrow_native_menu_preview_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 std::shared_ptr<NativeMenuPreviewV121>&,std::string&);
bool native_menu_preview_main_hide_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,std::string&);
bool native_menu_preview_select_hide_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 std::int32_t selected_slot,std::uintptr_t actual_select_root,std::string&);
//The source Front NativeSetSaveSlot callback can borrow these SAME lifecycle
//methods without a second preview owner or a no-op camera provider.
bool native_menu_avatar_services_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 dh2::ui::MenuAvatarPreviewServicesV1&,std::shared_ptr<void>&,std::string&);
bool native_menu_preview_prepare_main_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,std::string&);
bool native_menu_preview_destroy_scene_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,std::string&);
bool native_menu_preview_destroy_avatar_camera_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,std::string&);
bool native_menu_preview_camera_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 std::shared_ptr<dh2::camera::CameraProceduralNodeV16>&,std::string&);
bool native_menu_preview_camera_view_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 dh2::camera::CameraViewV11&,bool& present,std::string&);
}
