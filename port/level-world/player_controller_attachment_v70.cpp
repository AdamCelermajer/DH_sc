#include "player_controller_attachment_v70.hpp"
namespace dh2::player {
bool PlayerControllerAttachmentV70::event(void* raw,const events::EventBorrowV12& event,events::EventManagerOwnerV12&,std::int32_t& result,std::string& error){
 auto* owner=static_cast<CallbackContext*>(raw)->owner;if(!owner){error="Released source Emu controller receiver";return false;}auto& self=*owner;std::int32_t type{};
 if(!event.get_type||!event.get_type(event.context,type,error))return false;
 if(type!=0&&type!=2){result=0;return true;} // actual406494/98 return0.
 if(!self.services_.emulation_event){error=type==0?"Required v2EmuController keyboard4060c0":"Required v2EmuController mouse405f48";return false;}
 return self.services_.emulation_event(event,type,result,error);
}
bool PlayerControllerAttachmentV70::construct(std::uintptr_t actor,std::uintptr_t controllable,std::int32_t gamepad,
 PlayerControllerAttachmentServicesV70 services,const std::function<bool(character::ControllerCommandState32&,std::uintptr_t,bool,std::string&)>& publish,std::string& e){
 if(attempted_||!actor||!controllable||!services.events||!services.online||!publish){e="Required fresh SAME local Character/controller/AppEvents/online attachment";return false;}attempted_=true;services_=std::move(services);
 // Mixed C1 base408dfc..18 then embedded Controllable C2404db8 and empty
 // children vector18/1c/20. Actual Character pointerC is assigned AFTER
 // source SetController publishes it, exactly36f130/134.
 controllable4_=controllable;mixed_={identity(),0,0,0,0,0};proxy_.controller4=0;
 if(!publish(mixed_,controllable4_,false,e))return false;published_=true;mixed_.owner=actor;
 // 36f13c..154: GetOnline occurs after Mixed.C assignment and before any
 // child allocation; a failed query retains precisely that publication.
 bool online{};if(!services_.online(online,e))return false;if(online)network_a_=1;
 auto add=[&](PlayerControllerChildV70::Kind kind){constructing_=std::make_unique<PlayerControllerChildV70>();auto& child=*constructing_;
  child.kind=kind;child.controllable4=reinterpret_cast<std::uintptr_t>(&proxy_);
  child.command={reinterpret_cast<std::uintptr_t>(&child.command),0,0,0,0,0};child.network_a=0;};
 add(PlayerControllerChildV70::Kind::gamepad);constructing_->gamepad_index10=gamepad;
 // AddController408fdc/e0 copies Mixed.C into each child before append.
 constructing_->command.owner=mixed_.owner;children_.push_back(std::move(constructing_));
 add(PlayerControllerChildV70::Kind::emulation);
 events::EventReceiverV12 receiver;receiver.identity=reinterpret_cast<std::uintptr_t>(constructing_.get());callback_token_->owner=this;receiver.context=callback_token_.get();receiver.on_event=event;receiver.lifetime=callback_token_;
 bool inserted{};if(!services_.events->attach(0,receiver,0,inserted,e))return false;attached0_=inserted;
 if(!services_.events->attach(2,receiver,0,inserted,e))return false;attached2_=inserted;
 constructing_->command.owner=mixed_.owner;children_.push_back(std::move(constructing_));
 add(PlayerControllerChildV70::Kind::hud);constructing_->command.owner=mixed_.owner;children_.push_back(std::move(constructing_));
 complete_=true;return true;
}
bool PlayerControllerAttachmentV70::source_end_loading_unblock_v97(std::uintptr_t actual,std::string& e){
 if(!published_||!complete_||!actual||actual!=identity()||mixed_.controller!=actual){e="Required SAME live published MixedController loading field8";return false;}
 mixed_.global_blocked=0;e.clear();return true;
}
bool PlayerControllerAttachmentV70::close(std::string& e){
 PlayerControllerChildV70* emulation=constructing_&&constructing_->kind==PlayerControllerChildV70::Kind::emulation?constructing_.get():nullptr;
 for(auto& child:children_)if(child->kind==PlayerControllerChildV70::Kind::emulation)emulation=child.get();
 if((attached0_||attached2_)&&(!services_.events||!emulation)){e="Required source Emu event receiver before controller destruction";return false;}
 const auto id=reinterpret_cast<std::uintptr_t>(emulation);bool removed{};
 if(attached0_){if(!services_.events->detach(0,id,removed,e))return false;attached0_=false;}
 if(attached2_){if(!services_.events->detach(2,id,removed,e))return false;attached2_=false;}
 if(callback_token_)callback_token_->owner=nullptr;callback_token_.reset();children_.clear();constructing_.reset();complete_=false;return true;
}
bool PlayerControllerAttachmentV70::source_update_v107(input::SourceInputManagerV60& manager,
 const std::function<bool(PlayerControllerChildV70&,const std::shared_ptr<const input::GamepadC1V60>&,std::string&)>& positive,
 std::string& e){
 if(!published_){e="Required actual MixedController source publication";return false;}
 const auto count=children_.size(); //408ae0 captured end-minus-begin count.
 for(std::size_t i=0;i<count;++i){
  if(i>=children_.size()||!children_[i]){e="Source MixedController child removed during Update";return false;}
  auto& child=*children_[i];
  //Captured actual Emu967828/Hud967888 virtual8=base3a2f44 BXLR.
  if(child.kind!=PlayerControllerChildV70::Kind::gamepad)continue;
  const auto index=child.gamepad_index10;
  if(index>=manager.gamepad_count())continue; //406c60 signed branch.
  std::shared_ptr<const input::GamepadC1V60> gamepad;
  if(index<0){if(!manager.get_first_connected_gamepad_v107(gamepad,e))return false;}
  else if(!manager.get_gamepad(index,gamepad,e))return false;
  if(!gamepad)continue; //4080f8 genuine NULL epilogue.
  if(!positive){e="Required positive v2GamepadController406c90 button/axis mappings";return false;}
  if(!positive(child,gamepad,e))return false;
 }
 e.clear();return true;
}
PlayerControllerAttachmentV70::~PlayerControllerAttachmentV70(){std::string ignored;if(!close(ignored)&&callback_token_)callback_token_->owner=nullptr;}
}

