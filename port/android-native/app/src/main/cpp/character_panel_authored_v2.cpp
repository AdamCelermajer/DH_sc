#include "character_panel_session_v1.hpp"
namespace dh2::android_ui {
namespace {struct Borrow {const model_renderer::PlayerGameplayBinding*& slot;const model_renderer::PlayerGameplayBinding* prior;
 Borrow(const model_renderer::PlayerGameplayBinding*& target,const model_renderer::PlayerGameplayBinding& p):slot(target),prior(target){slot=&p;}
 ~Borrow(){slot=prior;}};}
bool CharacterPanelSessionV1::initialize_authored(const model_renderer::PlayerGameplayBinding& p,const ui::AuthoredCharacterPanelServicesV2& services,std::string& error){
 if(!p.active||!p.world_owner||!prepare(error)){if(error.empty())error="Authored panel requires current live profile";return false;}
 Borrow borrow(authored_player_,p);auto candidate=std::make_unique<ui::AuthoredCharacterPanelV2>();auto source=services;
 source.queries=[this](const char* name,auto& call,auto& e){if(!authored_player_){e="Authored character callback outside a current profile borrow";return false;}return dispatch(*authored_player_,name,call,e);};
 if(!source.localization.text)source.localization.text=&text_;
 struct Initializing {ui::AuthoredCharacterPanelV2*& slot;~Initializing(){slot=nullptr;}} initializing{initializing_authored_};
 initializing_authored_=candidate.get();
 if(!candidate->initialize(source,error))return false;
 authored_=std::move(candidate);return true;
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
