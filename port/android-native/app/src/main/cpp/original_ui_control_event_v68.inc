//Normal HUDControls.OnEvent418d28: cache identity routing and source fields,
//before MenuManager forwards the SAME event to the actual primary3 state.
bool source_control_native_event_v68(ui::SwfEvent48& event,std::string& e){
 if(!movie||reinterpret_cast<std::uintptr_t>(movie.get())!=source_controls_root658_v62||!source_controls_constructed_v62){e="HUD native event requires SAME constructed controls/movie generation";return false;}
 auto& prefix=source_controls_v62.prefix;ui::HudAttackActorBorrowV46 player;
 if(!prefix.local_player||!prefix.local_player(prefix.context,0,false,player,e)){if(e.empty())e="HUD native event requires actual PM local0,false";return false;}
 bool map_visible{};if(!source_controls_v62.world_map_visible||!source_controls_v62.world_map_visible(map_visible,e)){if(e.empty())e="Required original WorldMap.GetInstance/IsVisible";return false;}
 if(source_controls_cache_v63.customize66c){e="Required HUDControls customization OnEvent418f40..419b08";return false;}
 float x{};std::memcpy(&x,&event.value0,4);const float y=event.x;
 const auto matches=[&](std::uint32_t offset){return event.character==reinterpret_cast<std::uintptr_t>(source_controls_cache_v63.get(offset));};
 if(matches(0x1c)){
  struct Joystick {Impl& self;ui::AuthoredJoystickServicesV1 actual;bool player_present;};
  Joystick j{*this,source_controls_v62.joystick,player.character!=0};auto services=j.actual;services.context=&j;
  services.position_stick=[](void* raw,std::int32_t x,std::int32_t y,std::string& e){auto& self=static_cast<Joystick*>(raw)->self;auto* stick=self.source_controls_cache_v63.get(0x1c);
   if(!stick){e="Required actual cached joystick stick";return false;}auto m=stick->get_matrix();m.m_[0][2]=static_cast<float>(x)*20.f;m.m_[1][2]=static_cast<float>(y)*20.f;stick->set_matrix(m);return true;};
  services.controller_allowed=[](void* raw,bool& out,std::string& e){auto& q=*static_cast<Joystick*>(raw);if(!q.player_present){out=false;return true;}return q.actual.controller_allowed&&q.actual.controller_allowed(q.actual.context,out,e);};
  services.rotate_direction=[](void* raw,const float base[3],float angle,float out[3],std::string& e){auto& q=*static_cast<Joystick*>(raw);return q.actual.rotate_direction&&q.actual.rotate_direction(q.actual.context,base,angle,out,e);};
  services.head_towards=[](void* raw,const float direction[3],std::string& e){auto& q=*static_cast<Joystick*>(raw);return q.actual.head_towards&&q.actual.head_towards(q.actual.context,direction,e);};
  services.stop=[](void* raw,std::string& e){auto& q=*static_cast<Joystick*>(raw);return q.actual.stop&&q.actual.stop(q.actual.context,e);};
  if(event.kind==4){if(!ui::authored_joystick_press_v1(joystick,x,y,e))return false;}
  else if(event.kind==5){auto* receiver=source_controls_cache_v63.get(0x1c);if(!receiver){e="Required actual joystick event receiver matrix";return false;}
   const auto& m=receiver->get_matrix();if(!ui::authored_joystick_drag_v1(joystick,x,y,m.m_[0][2],m.m_[1][2],services,e))return false;}
  else if(event.kind==6||event.kind==7){if(!ui::authored_joystick_release_v1(joystick,player.character!=0,services,e))return false;}
  event.consumed=1;return true;
 }
 if(matches(0x88)){
  bool consumed{};if(!ui::hud_attack_event_v46(attack_controls_v46,event.kind,consumed,e))return false;
  //418f10 marks event24 on every interact receiver event, even non4/6/7.
  event.consumed=1;return true;
 }
 for(const auto offset:{0xe8u,0x118u,0x148u,0x178u,0x1a8u,0x1d8u,0x208u,0x238u,0x268u,0x298u,0x2c8u,0x2f8u,0x328u,0x358u}){
  if(!matches(offset))continue;attack_controls_v46.pending_x7c=-1;attack_controls_v46.pending_y80=-1;attack_controls_v46.consumed84=1;return true;
 }
 if(map_visible){attack_controls_v46.pending_x7c=-1;attack_controls_v46.pending_y80=-1;attack_controls_v46.consumed84=1;}
 return true;
}
