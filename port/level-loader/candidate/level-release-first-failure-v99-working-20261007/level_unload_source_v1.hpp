#pragma once
#include "canonical_level_context_v1.hpp"
#include "gslevel_lifecycle_v2.hpp"
#include "../level-world/player_level_quicksave_v29.hpp"
#include "../level-world/canonical_object_manager_v1.hpp"
#include <exception>
namespace dh2::loader {
// Actual native receiver/containing lease. This is not an emulated ARM pointer,
// a replacement singleton or a new engine owner. Borrowers capture App weakly.
struct LevelReleaseReceiverV1 {std::shared_ptr<void> owner;std::uintptr_t identity{};explicit operator bool()const noexcept{return owner&&identity;}};
struct LevelReleaseRandomV1 {std::shared_ptr<void> owner;std::uint32_t *seed{},*synced_seed{};};
struct LevelUnloadServicesV1 {
 std::shared_ptr<void> owner; // independent services, never enclosing App/World/Level
 // Native safety admission before destructive source delivery: stop the SAME
 // frame/draw/event/object transports, retaining their actual release journals.
 // No resource/class storage is dropped by this quiescence leaf.
 std::function<bool(const std::shared_ptr<CanonicalLevelContextV1>&,std::string&)> quiesce_delivery;
 std::function<bool(std::shared_ptr<player::PlayerLevelQuickSaveV29>&,std::string&)> quicksave;
 std::function<bool(LevelReleaseReceiverV1&,std::string&)> menu_manager,sound_manager,application;
 std::function<bool(const LevelReleaseReceiverV1&,const char*,LevelReleaseReceiverV1&,std::string&)> menu_by_name;
 std::function<bool(const LevelReleaseReceiverV1&,const LevelReleaseReceiverV1&,std::string&)> push_menu,pop_menu;
 std::function<bool(const LevelReleaseReceiverV1&,bool&,std::string&)> menu_visible;
 std::function<bool(std::uintptr_t,LevelReleaseReceiverV1&,std::string&)> camera;
 // Genuine CameraLevel/CameraOverview D0 over this reached SAME receiver. It
 // must retire native membership/controller/active references before returning.
 // Loader, not this leaf, performs the subsequent Level128/12c zero stores.
 std::function<bool(const LevelReleaseReceiverV1&,std::string&)> camera_d0;
 std::function<bool(const std::shared_ptr<CanonicalLevelContextV1>&,bool,std::string&)> save_all_players;
 std::function<bool(const LevelReleaseReceiverV1&,std::int32_t,std::string&)> stop_all_sounds;
 std::function<bool(const LevelReleaseReceiverV1&,std::shared_ptr<world::CanonicalObjectManagerV1>&,std::string&)> objects;
 std::function<bool(world::CanonicalObjectManagerV1&,std::string&)> network_uninit_level;
 std::function<bool(const LevelReleaseReceiverV1&,std::shared_ptr<player::PlayerManagerOwnerV1>&,std::string&)> players;
 std::function<bool(player::PlayerManagerOwnerV1&,std::string&)> player_update;
 std::function<bool(std::uint32_t&,std::string&)> real_time;
 std::function<bool(LevelReleaseRandomV1&,std::string&)> random_globals;
};
enum class LevelUnloadPhaseV1 {idle,quiesce,quicksave,menu_manager,menu_lookup,menu_push,camera128,camera12c,save_players,sound,menu_visible,menu_pop,phase_zero,application,objects,network_uninit,players,pm_byte_zero,pm_update,real_time,random_stores,complete,failed};
// Whole Level::Unload3f0690. D1 destruction is DISTINCT and occurs later in
// existing GSLevelLifecycleV2. The journal stores no live engine/Level borrow.
class LevelUnloadSourceV1 final {
 std::weak_ptr<CanonicalLevelContextV1> level_;LevelUnloadServicesV1 services_;
 LevelUnloadPhaseV1 phase_{LevelUnloadPhaseV1::idle},failed_at_{LevelUnloadPhaseV1::idle};
 bool busy_{},attempted_{},complete_{};std::string failure_;
 bool fail(std::string& e){if(failure_.empty())failure_=e.empty()?"Required genuine Level.Unload leaf at phase "+std::to_string(unsigned(phase_)):e;if(phase_!=LevelUnloadPhaseV1::failed){failed_at_=phase_;phase_=LevelUnloadPhaseV1::failed;}e=failure_;return false;}
 //The first failure is authoritative even when a provider clears its mutable error.
 bool continuing(std::string& e)const{if(!failure_.empty()){e=failure_;return false;}return true;}
 template<class F,class...A> bool call(LevelUnloadPhaseV1 p,const F& f,std::string& e,A&&...a){
  if(!continuing(e))return false;phase_=p;if(!f)return fail(e);
  const bool delivered=f(std::forward<A>(a)...,e);
  if(!continuing(e))return false;if(!delivered)return fail(e);return true;
 }
public:
 LevelUnloadSourceV1(std::weak_ptr<CanonicalLevelContextV1> level,LevelUnloadServicesV1 s):level_(std::move(level)),services_(std::move(s)){}
 bool execute(const std::shared_ptr<CanonicalLevelContextV1>& level,std::string& e){
  //Latch recursion at the active source phase, including a foreign nested receiver.
  if(busy_){e="Level.Unload cannot reenter a reached cleanup prefix";return fail(e);}
  auto expected=level_.lock();if(!expected||!level||expected.get()!=level.get()||expected.owner_before(level)||level.owner_before(expected)||!services_.owner){e="Required SAME actual Level/independent unload services";return false;}
  if(complete_){e.clear();return true;}if(attempted_){e=failure_.empty()?"Level.Unload cannot replay/reenter a reached cleanup prefix":failure_;return false;}
  attempted_=true;busy_=true;struct Guard{bool& b;~Guard(){b=false;}} guard{busy_};e.clear();
  try{
   auto borrow=level->constructor_borrow_v3();if(!borrow.owner||borrow.identity!=level->identity()||!borrow.fields){e="Required SAME original Level C1 fields";return fail(e);}auto& fields=*borrow.fields;
   if(!call(LevelUnloadPhaseV1::quiesce,services_.quiesce_delivery,e,level))return false;
   const bool disable_save=fields.byte_f0!=0; // authentic read BEFORE QuickSave
   std::shared_ptr<player::PlayerLevelQuickSaveV29> quick;
   if(!call(LevelUnloadPhaseV1::quicksave,services_.quicksave,e,quick)||!quick){if(e.empty())e="Required existing real PlayerLevelQuickSaveV29";return fail(e);}
   player::PlayerQuickSaveLevelV29 quick_level{borrow.owner,borrow.identity,&fields.save_ec,&fields.field130};
   if(!quick->execute(quick_level,disable_save,e)||!continuing(e))return fail(e);
   LevelReleaseReceiverV1 menus,menu;
   if(!call(LevelUnloadPhaseV1::menu_manager,services_.menu_manager,e,menus)||!menus){if(e.empty())e="Required real MenuManager";return fail(e);}
   if(!call(LevelUnloadPhaseV1::menu_lookup,services_.menu_by_name,e,menus,"menu_Loading",menu))return false;
   // Original PushMenu is unconditional; do not turn a missing leaf/menu into
   // semantic absence. IsVisible later dereferences this SAME cached menu.
   if(!call(LevelUnloadPhaseV1::menu_push,services_.push_menu,e,menus,menu))return false;
   auto camera_d0=[&](std::uintptr_t& slot,LevelUnloadPhaseV1 phase){if(!continuing(e))return false;phase_=phase;const auto identity=slot;if(!identity)return true;LevelReleaseReceiverV1 camera;
    if(!services_.camera||!services_.camera(identity,camera,e)||!continuing(e)||!camera||camera.identity!=identity){if(e.empty())e="Required SAME reached Camera deleting receiver";return fail(e);}
    if(!services_.camera_d0||!services_.camera_d0(camera,e)||!continuing(e))return fail(e);slot=0;return true;
   };
   if(!camera_d0(fields.field128,LevelUnloadPhaseV1::camera128)||!camera_d0(fields.field12c,LevelUnloadPhaseV1::camera12c))return false;
   // Source reloads byteF0 AFTER both D0 calls, never reuses QuickSave snapshot.
   if(!call(LevelUnloadPhaseV1::save_players,services_.save_all_players,e,level,fields.byte_f0!=0))return false;
   LevelReleaseReceiverV1 sound;if(!call(LevelUnloadPhaseV1::sound,services_.sound_manager,e,sound)||!sound){if(e.empty())e="Required real VoxSoundManager";return fail(e);}
   if(!call(LevelUnloadPhaseV1::sound,services_.stop_all_sounds,e,sound,500))return false;
   bool visible=false;if(!menu){e="Original Unload requires nonNULL cached menu for IsVisible";return fail(e);}
   if(!call(LevelUnloadPhaseV1::menu_visible,services_.menu_visible,e,menu,visible))return false;
   if(visible&&!call(LevelUnloadPhaseV1::menu_pop,services_.pop_menu,e,menus,menu))return false;
   if(!continuing(e))return false;phase_=LevelUnloadPhaseV1::phase_zero;fields.field130=0;
   LevelReleaseReceiverV1 app;if(!call(LevelUnloadPhaseV1::application,services_.application,e,app)||!app){if(e.empty())e="Required actual Application singleton";return fail(e);}
   std::shared_ptr<world::CanonicalObjectManagerV1> objects;
   if(!call(LevelUnloadPhaseV1::objects,services_.objects,e,app,objects)||!objects){if(e.empty())e="Required SAME App38 ObjectManager";return fail(e);}
   if(!call(LevelUnloadPhaseV1::network_uninit,services_.network_uninit_level,e,*objects))return false;
   std::shared_ptr<player::PlayerManagerOwnerV1> players;
   if(!call(LevelUnloadPhaseV1::players,services_.players,e,app,players)||!players){if(e.empty())e="Required SAME App40 PlayerManager";return fail(e);}
   auto* pm=players->source_frame_fields_v68();if(!continuing(e))return false;phase_=LevelUnloadPhaseV1::pm_byte_zero;if(!pm){e="Required authentic PM C1 byte6c9";return fail(e);}pm->byte6c9=0;
   if(!call(LevelUnloadPhaseV1::pm_update,services_.player_update,e,*players))return false;
   std::uint32_t now{};if(!call(LevelUnloadPhaseV1::real_time,services_.real_time,e,now))return false;
   LevelReleaseRandomV1 random;if(!call(LevelUnloadPhaseV1::random_stores,services_.random_globals,e,random)||!random.owner||!random.seed||!random.synced_seed){if(e.empty())e="Required SAME Random::s_seed/s_syncedSeed cells";return fail(e);}
   *random.seed=now;*random.synced_seed=0;if(!continuing(e))return false;phase_=LevelUnloadPhaseV1::complete;complete_=true;e.clear();return true;
  }catch(const std::exception& ex){e=ex.what();return fail(e);}catch(...){e="Original Level.Unload provider threw; retained source prefix";return fail(e);}
 }
 auto phase()const noexcept{return phase_;}auto failed_at()const noexcept{return failed_at_;}
 bool complete()const noexcept{return complete_;}
};
}
