#include "character_panel_session_v1.hpp"
#include "renderer_character_campaign_v62.hpp"
namespace dh2::android_ui {
namespace {struct Borrow {const model_renderer::PlayerGameplayBinding*& slot;const model_renderer::PlayerGameplayBinding* prior;
 Borrow(const model_renderer::PlayerGameplayBinding*& target,const model_renderer::PlayerGameplayBinding& p):slot(target),prior(target){slot=&p;}
 ~Borrow(){slot=prior;}};}
bool CharacterPanelSessionV1::can_retire_campaign_v104(const std::shared_ptr<void>& world,std::string& error)const{
 if(!world||authored_player_||initializing_authored_||failed_authored_prefix_v98_){error="Character campaign detach requires actual world and quiescent native callbacks";return false;}
 const auto bound=gameplay_world_v104_.lock();
 if(bound&&(bound.get()!=world.get()||bound.owner_before(world)||world.owner_before(bound))){error="Character callbacks belong to another World";return false;}
 if(gameplay_services_.owner&&!bound){error="Character callback owner lacks its actual campaign scope";return false;}
 error.clear();return true;
}
bool CharacterPanelSessionV1::retire_campaign_v104(const std::shared_ptr<void>& world,std::string& error){
 if(!can_retire_campaign_v104(world,error))return false;
 //Transport aliases only. No MenuFX D0, resource clear, pointer release or
 //widget reset is synthesized by this final post-destruction detach.
 gameplay_services_={};gameplay_world_v104_.reset();campaign_detached_v104_=true;error.clear();return true;
}
bool CharacterPanelSessionV1::adopt_source_gameplay_v109(const model_renderer::PlayerGameplayBinding& p,const ui::AuthoredCharacterPanelServicesV2& services,std::string& e){
 if(!p.active||!p.world_owner||authored_player_||initializing_authored_||failed_authored_prefix_v98_||!authored_||!authored_->bound()||
    !gameplay_services_.owner||gameplay_world_v104_.lock()!=p.world_owner){e="Required admitted SAME campaign and retained primary1 for gameplay adoption";return false;}
 auto source=services;const std::weak_ptr<void> campaign=p.world_owner;const auto character=p.character;
 source.queries=[this,campaign,character](const char* name,auto& call,auto& e){auto world=campaign.lock();model_renderer::PlayerGameplayBinding live;
  if(!world||!model_renderer::borrow_source_campaign_player_gameplay_v67(world,live,e)||!live.active||live.character!=character){if(e.empty())e="Retired/replaced selected character menu";return false;}
  Borrow borrow(authored_player_,live);return dispatch(live,name,call,e);
 };
 if(!source.localization.text)source.localization.text=&text_;
 if(!authored_->rebind_process_services_v104(source,e))return false;
 campaign_detached_v104_=false;return true;
}
bool CharacterPanelSessionV1::retire_failed_authored_prefix_v98(std::string& error){
 if(authored_player_||initializing_authored_){error="Character prefix retirement requires quiescent native dispatch";return false;}
 if(!failed_authored_prefix_v98_){error.clear();return true;}
 auto& p=*failed_authored_prefix_v98_;auto* stack=p.source_stack_v4();
 if(p.source_render_identity_v91()||p.source_movie_v91()||(failed_authored_identity_v98_&&(!stack||stack->render(failed_authored_identity_v98_)))){
  error="Required actual failed primary1 Multi unload/D0/field and render retirement";return false;
 }
 failed_authored_prefix_v98_.reset();failed_authored_identity_v98_=0;failed_authored_error_v98_.clear();error.clear();return true;
}
bool CharacterPanelSessionV1::initialize_authored(const model_renderer::PlayerGameplayBinding& p,const ui::AuthoredCharacterPanelServicesV2& services,std::string& error){return initialize_authored_impl_v98(p,services,nullptr,{},error);}
bool CharacterPanelSessionV1::load_source_primary1_v98(const model_renderer::PlayerGameplayBinding& p,const ui::AuthoredCharacterPanelServicesV2& services,const char* uri,
 std::function<bool(model_renderer::PlayerGameplayBinding&,std::string&)> live_binding,std::string& error){
 if(!uri||!*uri||!live_binding){error="Required original primary1 URI and genuine current selected Character borrower";return false;}
 return initialize_authored_impl_v98(p,services,uri,std::move(live_binding),error);
}
bool CharacterPanelSessionV1::initialize_authored_impl_v98(const model_renderer::PlayerGameplayBinding& p,const ui::AuthoredCharacterPanelServicesV2& services,const char* source_uri,
 std::function<bool(model_renderer::PlayerGameplayBinding&,std::string&)> live_binding,std::string& error){
 if(initializing_authored_||failed_authored_prefix_v98_){if(failed_authored_error_v98_.empty())failed_authored_error_v98_="Character constructor/Load prefix requires actual teardown before replay";error=failed_authored_error_v98_;return false;}
 if(source_uri){model_renderer::PlayerGameplayBinding actual;
  if(!live_binding||!live_binding(actual,error)||actual.active||!actual.world_owner||
     actual.world_owner.get()!=p.world_owner.get()||actual.world_owner.owner_before(p.world_owner)||p.world_owner.owner_before(actual.world_owner)||
     actual.character!=p.character||actual.save!=p.save||actual.skills!=p.skills||actual.gear!=p.gear){
   if(error.empty())error="Required genuine SAME source-loading selected profile";return false;
  }
 }
 if((!source_uri&&!p.active)||!p.world_owner||!prepare(error)){if(error.empty())error="Authored panel requires current live profile";return false;}
 Borrow borrow(authored_player_,p);auto source=services;
 source.queries=[this](const char* name,auto& call,auto& e){if(!authored_player_){e="Authored character callback outside a current profile borrow";return false;}return dispatch(*authored_player_,name,call,e);};
 if(source_uri){
  if(authored_&&authored_->source_render_identity_v91()){error="Existing primary1 awaits genuine Stage2 unload before source reconstruction";return false;}
  const auto campaign=std::weak_ptr<void>(p.world_owner);
  const auto incoming_continue=source.source_continue_v98;
  const auto incoming_movie_continue=source.movie.source_continue_v98;
  source.source_continue_v98=[this,campaign,incoming_continue,incoming_movie_continue](auto& e){
   if(!failed_authored_error_v98_.empty()){e=failed_authored_error_v98_;return false;}
   if(campaign.expired()){e="Primary1 source campaign expired during native resource delivery";return false;}
   if(incoming_continue&&!incoming_continue(e))return false;
   if(incoming_movie_continue&&!incoming_movie_continue(e))return false;
   if(!failed_authored_error_v98_.empty()){e=failed_authored_error_v98_;return false;}
   return true;
  };
  source.movie.source_continue_v98=source.source_continue_v98;
  source.queries=[this,campaign,live_binding=std::move(live_binding)](const char* name,auto& call,auto& e){
   if(!failed_authored_error_v98_.empty()){e=failed_authored_error_v98_;return false;}model_renderer::PlayerGameplayBinding live;
   const bool borrowed=live_binding(live,e);
   if(!failed_authored_error_v98_.empty()){e=failed_authored_error_v98_;return false;}
   if(!borrowed||!live.world_owner){if(e.empty())e="Required genuine current selected Character for primary1 native query";return false;}
   const auto world=campaign.lock();if(!world||world.get()!=live.world_owner.get()||world.owner_before(live.world_owner)||live.world_owner.owner_before(world)){
    e="Primary1 native query belongs to replaced campaign";return false;
   }
   Borrow borrow(authored_player_,live);
   const bool delivered=live.active?dispatch(live,name,call,e):dispatch_source_loading_v98(live,name,call,e);
   if(!failed_authored_error_v98_.empty()){e=failed_authored_error_v98_;return false;}return delivered;
  };
 }
 if(!source.localization.text)source.localization.text=&text_;
 if(!source_uri&&campaign_detached_v104_&&authored_&&authored_->bound()){
  if(!authored_->rebind_process_services_v104(source,error))return false;
  campaign_detached_v104_=false;return true;
 }
 struct Initializing {ui::AuthoredCharacterPanelV2*& slot;~Initializing(){slot=nullptr;}} initializing{initializing_authored_};
 //Session owns the actual candidate before any publication/native startup.
 //An exception or failed callback keeps its resource available to real unload.
 failed_authored_prefix_v98_=std::make_unique<ui::AuthoredCharacterPanelV2>();
 initializing_authored_=failed_authored_prefix_v98_.get();
 struct PrefixReceipt {CharacterPanelSessionV1& self;~PrefixReceipt(){if(self.failed_authored_prefix_v98_){
  self.failed_authored_identity_v98_=self.failed_authored_prefix_v98_->source_render_identity_v91();
 }}} receipt{*this};
 const bool initialized=source_uri?failed_authored_prefix_v98_->load_source_primary1_v98(source,source_uri,error):failed_authored_prefix_v98_->initialize(source,error);
 if(!initialized||!failed_authored_error_v98_.empty()){
  if(failed_authored_error_v98_.empty())failed_authored_error_v98_=error.empty()?"Actual Character primary1 initialization failed":error;error=failed_authored_error_v98_;return false;
 }
 authored_=std::move(failed_authored_prefix_v98_);failed_authored_identity_v98_=0;failed_authored_error_v98_.clear();campaign_detached_v104_=false;return true;
}
bool CharacterPanelSessionV1::authored_graph_scope_v4(void* context,bool(*callback)(void*,ui::SwfAsGraph&,std::string&),std::string& error){
 auto* panel=authored_native_v4();if(!panel){error="Required retained authored character movie Scope";return false;}
 return panel->scoped_graph(context,callback,error);
}
bool CharacterPanelSessionV1::authored_open(const model_renderer::PlayerGameplayBinding& p,std::string& e){if(!authored_bound()){e="Authored panel platform not initialized";return false;}Borrow borrow(authored_player_,p);return authored_->open(e);}
bool CharacterPanelSessionV1::authored_tab(const model_renderer::PlayerGameplayBinding& p,unsigned tab,std::string& e){if(!authored_bound()){e="Authored panel platform not initialized";return false;}Borrow borrow(authored_player_,p);return authored_->tab(tab,e);}
bool CharacterPanelSessionV1::authored_release(const model_renderer::PlayerGameplayBinding& p,const char* path,std::string& e){if(!authored_bound()){e="Authored panel platform not initialized";return false;}Borrow borrow(authored_player_,p);return authored_->release(path,e);}
bool CharacterPanelSessionV1::authored_back(const model_renderer::PlayerGameplayBinding& p,std::string& e){if(!authored_bound()){e="Authored panel platform not initialized";return false;}Borrow borrow(authored_player_,p);return authored_->back(e);}
bool CharacterPanelSessionV1::authored_pointer(const model_renderer::PlayerGameplayBinding& p,int action,int id,float x,float y,std::string& e){if(!authored_bound()){e="Authored panel platform not initialized";return false;}Borrow borrow(authored_player_,p);return authored_->pointer(action,id,x,y,e);}
bool CharacterPanelSessionV1::authored_geometry(const model_renderer::PlayerGameplayBinding& p,const char* path,float x,float y,ui::AuthoredHudGeometryV1& out,std::string& e){if(!authored_bound()){e="Authored panel platform not initialized";return false;}Borrow borrow(authored_player_,p);return authored_->geometry(path,x,y,out,e);}
bool CharacterPanelSessionV1::authored_frame(const model_renderer::PlayerGameplayBinding& p,float dt,std::string& e){if(!authored_bound()){e="Authored panel platform not initialized";return false;}Borrow borrow(authored_player_,p);return authored_->advance(dt,e);}
bool CharacterPanelSessionV1::authored_display(const model_renderer::PlayerGameplayBinding& p,int x,int y,int w,int h,std::string& e){if(!authored_bound()){e="Authored panel platform not initialized";return false;}Borrow borrow(authored_player_,p);return authored_->display(x,y,w,h,e);}
bool CharacterPanelSessionV1::authored_source_display_v4(const model_renderer::PlayerGameplayBinding& p,std::string& e){if(!authored_bound()){e="Authored panel platform not initialized";return false;}Borrow borrow(authored_player_,p);return authored_->movie()->movie()->display_source_stage_clip_v5("_root",e);}
bool CharacterPanelSessionV1::authored_viewport(const model_renderer::PlayerGameplayBinding& p,const ui::ViewportState64& seed,const ui::SwfViewportDriver& driver,std::string& e){if(!authored_bound()){e="Authored panel platform not initialized";return false;}Borrow borrow(authored_player_,p);return authored_->connect_viewport(seed,driver,e);}
}
