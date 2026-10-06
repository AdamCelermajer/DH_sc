#pragma once
#include "hud_manager.hpp"
#include "hud_manager_core.hpp"
#include "hud_advance_owner.hpp"
#include "swf_movie.hpp"
#include "enemy_hud_presentation_v2.hpp"
#include <memory>
namespace dh2::ui {
// Synchronous borrow of the genuine player/world. The actor projections must
// remain stable until this world is released, including across source reloads.
struct EnemyHudWorldBorrowV1 {
 HudManagerActor* player{};
 HudManagerServices services{};
 std::uintptr_t world{};
 EnemyHudPresentationServicesV2 presentation{};
};
struct EnemyHudTextServicesV1 {
 void* context{};
 bool(*integer_string)(void*,std::int32_t,std::string&,bool&,std::string&){};
};
// Separate entry into the already reconstructed FastUpdate enemy block. It
// calls the identical body; it does not replace full manager FastUpdate.
extern "C" int dh2_ui_hud_enemy_v1(HudManagerState*,HudManagerActor*,const HudManagerServices*) noexcept;
class EnemyStatusHudV1 {
public:
 explicit EnemyStatusHudV1(SwfMovie&,EnemyHudTextServicesV1);
 ~EnemyStatusHudV1();
  bool update(const EnemyHudWorldBorrowV1&,std::string&,std::int32_t hud_style=0);
 std::uintptr_t target()const{return target_;}
 int hp_frame()const{return hp_frame_;}
 bool visible()const{return visible_;}
 const std::string& name()const{return name_;}
 const std::string& level()const{return level_;}
private:
 SwfMovie& movie_;
 EnemyHudTextServicesV1 text_;
 HudManagerCore core_;
 HudAdvanceOwner advance_;
 HudManagerState state_{};
 const EnemyHudWorldBorrowV1* borrow_{};
 SwfAsGraph* graph_{};
 std::string* error_{};
 std::string localized_,name_,level_;
  bool bound_{},visible_{};
  std::int32_t hud_style_{};
 std::uintptr_t world_{},target_{};
 int hp_frame_{};
 EnemyHudPresentationV2 presentation_{};
 float root_anchor_[2]{};
 float viewport_pixels_[4]{},visible_root_[4]{};
 bool presentation_hidden_{};
 static bool apply(void*,SwfAsGraph&,std::string&);
 static bool present(void*,SwfAsGraph&,std::string&);
 static int service(void*,HudManagerState*,const HudManagerRequest*,HudManagerResponse*);
 static int text_operation(void*,const HudManagerRequest&,HudManagerResponse&,std::string&);
 static bool notify(void*,gameswf::sprite_instance*,std::string&);
 static bool sound(void*,std::uintptr_t&,std::string&);
 static bool pause(void*,std::uintptr_t,std::int32_t,bool,std::string&);
};
}
