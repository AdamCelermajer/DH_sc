#include "flash_anim_manager_v92.hpp"
#include <cmath>
#include <utility>
namespace dh2::ui {
namespace {
template<class F,class... A>bool invoke(const F& f,const char* name,std::string& e,A&&... a){if(!f){e=std::string("Required actual FlashAnim producer: ")+name;return false;}return f(std::forward<A>(a)...,e);}
bool same(const std::shared_ptr<void>& a,const std::shared_ptr<void>& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);}
}
FlashAnimManagerV92::FlashAnimManagerV92():backend_(std::make_shared<CombatFlashSwfV1>()){}
bool FlashAnimManagerV92::bind_hud(const std::shared_ptr<SwfMovie>& movie,std::weak_ptr<void> owner,CombatFlashProjectionV1 projection,std::string& e){
 if(updating_||scanned_fx_){e="Flash HUD rebind requires completed identity-matched reset";return false;}
 return backend_->bind_movie_v92(movie,std::move(owner),projection,e);
}
bool FlashAnimManagerV92::scan_hud(std::string& e){
 const auto fx=backend_->bound_movie_v92();if(!fx){e="Required actual bound HUD before source scanner";return false;}
 if(updating_||backend_->busy_v92()){e="Flash scan attempted during source update/dispatch";return false;}
 if(scanned_fx_){std::int32_t mode{};if(!application_.actual_application||!invoke(application_.assertion_mode,"assertion mode",e,mode))return false;
  if(mode==1){e.clear();return true;}if(mode==2){e="Source ScanForAnims assertion: existing scanned MenuFX";return false;}}
 scanned_fx_=fx; // Original publishes before FindCharacters; failure keeps prefix.
 CombatFlashScanReceiptV92 receipt;if(!backend_->scan_receipt_v92(receipt,e))return false;
 return adopt_successful_scan(receipt,e);
}
bool FlashAnimManagerV92::adopt_successful_scan(const CombatFlashScanReceiptV92& r,std::string& e){
 if(!backend_->validate_receipt_v92(r)||(scanned_fx_&&scanned_fx_!=r.movie)){e="Required actual successful scan receipt from SAME backend/movie generation";return false;}
 scanned_fx_=r.movie;e.clear();return true;
}
bool FlashAnimManagerV92::reset_scan_for_anims(std::uintptr_t fx,std::string& e){
 if(fx!=scanned_fx_){e.clear();return true;} // Source unmatched reset is a no-op.
 if(updating_||backend_->busy_v92()){e="Flash reset requires completed source update/graph dispatch";return false;}
 if(scanned_fx_){
  if(backend_->bound_movie_v92()!=fx){e="Source Flash owner differs from same backend HUD binding";return false;}
  scanned_fx_=0; // Original owner clear precedes vector/flag/pin release.
  return backend_->reset_movie_v92(fx,e);
 }
 // Matching null reset still clears the genuine source vector/flags. It
 // does not erase an adapter's staged movie binding before actual Scan.
 return backend_->reset_scan_fields_v92(e);
}
bool FlashAnimManagerV92::discard_unscanned_hud(std::uintptr_t fx,std::string& e){
 if(scanned_fx_||updating_){e="Unscanned HUD discard cannot erase source scanner prefix";return false;}
 return backend_->discard_unscanned_binding_v92(fx,e);
}
bool FlashAnimManagerV92::claim_menu_update(std::shared_ptr<void> manager,FlashAnimApplicationServicesV92 app,std::string& e){
 if(!manager||!app.actual_application||!app.current_level||!app.level_phase||!app.application_dt){e="Required SAME MenuManager and actual Application/Level/dt producers";return false;}
 if(updating_||(menu_update_owner_&&!same(menu_update_owner_,manager))){e="Flash source update still belongs to another live MenuManager";return false;}
 menu_update_owner_=std::move(manager);application_=std::move(app);e.clear();return true;
}
bool FlashAnimManagerV92::release_menu_update(const std::shared_ptr<void>& manager,std::string& e){
 if(updating_||!same(menu_update_owner_,manager)){e="Flash update-owner release requires same quiescent MenuManager";return false;}
 menu_update_owner_.reset();application_={};e.clear();return true;
}
bool FlashAnimManagerV92::update(std::string& e){
 if(!menu_update_owner_||!application_.actual_application||updating_){e="Required sole live MenuManager Flash update owner";return false;}
 updating_=true;struct Finish{bool& v;~Finish(){v=false;}} finish{updating_};
 std::uintptr_t level{};bool loading=false;if(!invoke(application_.current_level,"Application.GetCurrentLevel",e,level))return false;
 if(level){std::int32_t phase{};if(!invoke(application_.level_phase,"Level+130",e,level,phase))return false;
  if(phase>1){if(!invoke(application_.level_phase,"Level+130 repeated",e,level,phase))return false;loading=phase<=26;}}
 std::int32_t interval=33;
 if(scanned_fx_&&!loading){float fps{};if(!backend_->source_frame_rate_v92(fps,e))return false;float step=1000.f/fps;
  if(!std::isfinite(step)||step<0||step>=2147483648.f){e="Invalid actual source Flash frame interval";return false;}interval=std::int32_t(step);}
 std::uint32_t dt{};if(!invoke(application_.application_dt,"Application.GetDt",e,dt))return false;
 return backend_->update_interval_v92(dt,interval,e);
}
}
