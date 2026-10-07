#pragma once
#include "character_menu_movie_v1.hpp"
#include "authored_character_menu_session_v1.hpp"
#include "menu_stack_actions_v1.hpp"
#include "character_menu_as_bridge_v1.hpp"
#include "authored_gameplay_hud_v1.hpp"
#include <array>
namespace dh2::ui {
struct CharacterPanelRenderBorrowV2 {
 std::uintptr_t identity{};std::uint32_t flags{};
 MenuStackCharacterV1* root{};MenuStackCharacterV1* focus{};
};
struct AuthoredCharacterPanelServicesV2 {
 SwfServices movie;
 CharacterMenuAsBridgeV1::Dispatch queries;
 std::uint32_t maximum_occurrences{};
 // Actual Application MenuManager bootstrap owns this same directory and
 // stack before the player-dependent character movie is loaded.
 std::shared_ptr<MenuStackOwnerV1> shared_stack_v27;
 // Actual application's existing HUD/base RenderFX, not surrogate panel IDs.
  CharacterPanelRenderBorrowV2 hud,base;
  std::vector<CharacterPanelRenderBorrowV2> additional_renders;
 MenuStackGlobalsV1* globals{};
 AuthoredMenuLocalizationServicesV1 localization;
 // Optional concrete override; source PostLoad creates the discovered screens
 // as MenuBase whose Create/GotFocus/LostFocus are literal empty methods.
 // Nonmovie lifecycle/application endpoints remain mandatory when reached.
 std::function<bool(AuthoredMenuFieldsV1&,std::string&)> create;
 AuthoredMenuLifecycleServicesV1 lifecycle;
 MenuStackServicesV1 remaining_stack;
 // Exact source native-character projection (including focus flag), evaluated
 // in the owning Scope. No guessed focus/enable constructor fields.
 std::function<bool(gameswf::as_object*,MenuStackCharacterV1&,std::string&)> character;
 std::function<bool(std::uint32_t&,std::string&)> panel_render_flags;
 // Multi.LoadSWF437dc8 publishes the retained MenuFX before virtual Load.
 // Allows genuine current-manager lookup during startup NativeReloadSkills.
 std::function<bool(std::uintptr_t,std::string&)> publish_render;
};
// Sole authored character movie and MenuManager stack for the existing Android
// CharacterPanelSession. Native profile authority remains wholly borrowed.
// Uses original dqshared+dqcharmenu bytes; tabs/confirmation/training/equipment
// handlers execute their actual authored ActionScript, never replacement UI.
class AuthoredCharacterPanelV2 {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 AuthoredCharacterPanelV2();~AuthoredCharacterPanelV2();
 AuthoredCharacterPanelV2(const AuthoredCharacterPanelV2&)=delete;
 AuthoredCharacterPanelV2& operator=(const AuthoredCharacterPanelV2&)=delete;
 bool initialize(const AuthoredCharacterPanelServicesV2&,std::string&);
 bool open(std::string&); // original manager PushMenu(menu_CharacterMenu)
 bool advance(float seconds,std::string&);
 bool display(int x,int y,int width,int height,std::string&);
 bool connect_viewport(const ViewportState64&,const SwfViewportDriver&,std::string&);
 // Named access for Android/accessibility/tests invokes source handlers. Real
 // touch transport supplies exact scoped shape-selected path, not a grid ID.
 bool release(const char* actual_button_path,std::string&);
 bool geometry(const char* actual_character_path,float screen_x,float screen_y,
               AuthoredHudGeometryV1&,std::string&);
 // Android transport feeds the retained GameSWF root's native mouse state.
 // Its source event generator performs dynamic shape selection, rollover,
 // button timelines, startDrag/stopDrag and scoped ActionScript handlers.
 // action0 down,1 up,2 move,3 platform cancellation. Single primary pointer.
 bool pointer(int action,int pointer_id,float screen_x,float screen_y,std::string&);
 bool tab(unsigned index,std::string&); //0stats,1inventory,2skills,3faery
 bool back(std::string&);
 bool bound()const noexcept;
  MenuStackOwnerV1* stack()noexcept;
  MenuStackOwnerV1* source_stack_v4()noexcept;
 CharacterMenuMovieV1* movie()noexcept;
 // Reuses the already active callback Scope during initialization/native AS.
 bool scoped_graph(void*,bool(*)(void*,SwfAsGraph&,std::string&),std::string&);
};
}
