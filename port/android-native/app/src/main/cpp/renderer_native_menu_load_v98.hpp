#pragma once
#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <cstdint>
namespace dh2::application {class ApplicationServicesOwnerV5;}
namespace dh2::ui {struct AuthoredCharacterPanelServicesV2;}
namespace model_renderer {
using NativePrimary1ContinueV98=std::function<bool(std::string&)>;
struct NativePrimaryMenuFactsV98 {
 std::shared_ptr<void> receiver;
 const std::int32_t* width_screen{}; //actual Width_Screen99b114, not view rectangle
 const std::int32_t* height_screen{}; //actual Height_Screen, MenuFlash2DCamera C1 half dimensions
 const std::uint8_t* htc_devices{}; //9f6403, reached width800 first
 const std::uint8_t* no_igp{}; //9f640d, width800/HTCfalse only
};
struct NativePrimary1LoadServicesV98 {
 std::shared_ptr<void> owner; //existing process menu/session/resource authority
 std::function<bool(NativePrimaryMenuFactsV98&,std::string&)> facts;
 //Existing Multi primary1/CharacterPanelSession, URI exactly as selected.
 //Publish SAME retained RenderFX BEFORE virtual Load/native startup callbacks.
 //Allow genuine HUD3NULL; defer common manager PostLoad/Hide to caller below.
 //Publish RenderFX only: caller owns paired camera C1 AFTER native Load.
 //Retain a failed constructor/Load prefix in that SAME owner for real teardown.
 //Legacy signature remains for existing transport consumers. Reached source12
 //requires scoped delivery so native prefix effects see the actual loading gate.
 std::function<bool(const char* actual_uri,std::uint32_t slot,std::string&)> load_primary;
 std::function<bool(const char* actual_uri,std::uint32_t slot,const NativePrimary1ContinueV98&,std::string&)> load_primary_scoped;
};
bool bind_native_primary1_loader_v98(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 NativePrimary1LoadServicesV98,std::string&);
bool compose_native_primary1_scope_v98(dh2::ui::AuthoredCharacterPanelServicesV2&,NativePrimary1ContinueV98,std::string&);
bool source_native_loadmenu1_v98(std::shared_ptr<void> actual_world,std::string&);
}
