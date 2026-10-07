//Logical SAME33 DebugCachedCharacter C1 owners used by HUDControls419b4c.
//Offset suffixes are source provenance, never casts into ARM memory.
struct SourceControlCacheV63 {
 struct Cache {weak_ptr<gameswf::character> character;std::string path8;std::uintptr_t render20{},root24{};};
 std::array<Cache,33> cache;
 std::array<std::array<float,2>,11> initial_style0_674{},initial_style1_6cc{};
 std::array<std::array<float,2>,11> saved_style0_724{},saved_style1_77c{};
 std::uint8_t customize66c{},captured66d{},captured66e{},saved66f{},saved670{};
 static unsigned slot(std::uint32_t offset){if(offset==0x1c)return 0;if(offset==0x4c)return 1;return 2+(offset-0x88)/0x30;}
 gameswf::character* get(std::uint32_t offset){return cache.at(slot(offset)).character.get_ptr();}
 bool initialize(ui::SwfAsGraph& graph,std::uintptr_t fx,std::int32_t style,ui::AuthoredJoystickStateV1& joystick,std::uint8_t& initialized,std::string& e){
  ui::SwfAsValue root;if(!graph.root_value(root,e))return false;
  auto find=[&](const ui::SwfAsValue& base,const char* path,ui::SwfAsValue& out){ui::AuthoredMenuSearchIndexV1 index;bool found{};
   if(!index.initialize(graph,base,e)||!index.find(path,out,found,e))return false;if(!found)out={};return true;};
  auto object=[&](const ui::SwfAsValue& value,gameswf::character*& out){out=nullptr;if(!value.identity())return true;gameswf::as_object* raw{};
   if(!graph.borrow_object(value,raw,e))return false;if(!raw||!raw->is(gameswf::character::m_class_id)){e="HUDControls cache Find returned non-character";return false;}out=static_cast<gameswf::character*>(raw);return true;};
  auto assign=[&](std::uint32_t offset,const ui::SwfAsValue& value,const char* path,const ui::SwfAsValue& base){gameswf::character* raw{};if(!object(value,raw))return false;
   auto& c=cache.at(slot(offset));c.character=raw;if(path)c.path8=path;c.render20=fx;c.root24=base.identity();return true;};
  ui::SwfAsValue hud;if(!find(root,("_root.menu_HUD_"+std::to_string(style)).c_str(),hud)||!assign(0x388,hud,nullptr,{}))return false;
  auto refresh=[&](std::uint32_t offset,const char* path,const ui::SwfAsValue& base){ui::SwfAsValue value;
   return find(base.identity()?base:root,path,value)&&assign(offset,value,path,base);};
  if(!refresh(0x1c,"HUDelements.controls.controls.Joystick.stick",hud)||
     !refresh(0x88,"HUDelements.controls.controls.btn_interact",hud)||
     !refresh(0x4c,"HUDelements.controls.controls.Joystick",hud)||!refresh(0xb8,"pinch",{})||
     !refresh(0xe8,"HUDelements.HealthBars.btn_potion",hud)||
     !refresh(0x3b8,"HUDelements.controls.controls.btn_spell",hud)||
     !refresh(0x3e8,"HUDelements.controls.controls.btn_skill1",hud)||
     !refresh(0x418,"HUDelements.controls.controls.btn_skill2",hud)||
     !refresh(0x448,"HUDelements.controls.controls.btn_skill3",hud)||
     !refresh(0x478,"HUDelements.btn_charactermenu",hud)||
     !refresh(0x4a8,"HUDelements.btn_mainmenu",hud)||
     !refresh(0x4d8,"HUDelements.HealthBars.player",hud)||
     !refresh(0x508,"HUDelements.HealthBars.btn_potion",hud)||
     !refresh(0x538,"HUDelements.itunes_controls.btn_itunes",hud)||
     !refresh(0x568,"HUDelements.itunes_controls",hud))return false;
  constexpr std::uint32_t skill[]{0x118,0x148,0x178};
  constexpr const char* skill_paths[]{"HUDelements.controls.controls.btn_skill1","HUDelements.controls.controls.btn_skill2","HUDelements.controls.controls.btn_skill3"};
  for(unsigned i=0;i<3;++i)if(!refresh(skill[i],style>1?"HUDelements.controls.controls.list.dragHandler":skill_paths[i],hud))return false;
  constexpr std::uint32_t common[]{0x1a8,0x1d8,0x208,0x238,0x268,0x298,0x2c8,0x2f8,0x328};
  constexpr const char* common_paths[]{"HUDelements.controls.controls.btn_spell","HUDelements.btn_charactermenu","HUDelements.btn_mainmenu","HUDelements.itunes_controls.btn_itunes","HUDelements.itunes_controls.itunes_bar.btn_play","HUDelements.itunes_controls.itunes_bar.btn_pause","HUDelements.itunes_controls.itunes_bar.btn_next","HUDelements.itunes_controls.itunes_bar.btn_previous","HUDelements.itunes_controls.itunes_bar.btn_playlist"};
  for(unsigned i=0;i<9;++i)if(!refresh(common[i],common_paths[i],hud))return false;
  if(!refresh(0x358,"LocationsMenu.btn_FastTravel",{}))return false;
  auto* stick_base=get(0x4c);ui::SwfAsValue base,bg;
  if(!stick_base||!graph.retain_object(stick_base,base,e)||!find(base,"bg",bg))return false;
  gameswf::character* background{};if(!object(bg,background)||!background){if(e.empty())e="HUDControls Joystick.bg source width receiver unavailable";return false;}
  if(!ui::authored_joystick_initialize_v1(joystick,background->get_width(),e))return false;
  auto* stick=get(0x1c);if(!stick){e="HUDControls SetPosition NULL stick";return false;}
  auto stick_matrix=stick->get_matrix();stick_matrix.m_[0][2]=0.f;stick_matrix.m_[1][2]=0.f;
  stick->set_matrix(stick_matrix); //source SetPosition(...,0,0)
  ui::SwfAsValue custom;if(!find(root,"_root.menu_custom",custom)||!assign(0x388,custom,nullptr,{})||
     !refresh(0x598,"btn_MENU_CUSTOM_RESET",custom)||!refresh(0x5c8,"btn_MENU_CUSTOM_SAVE",custom))return false;
  auto visible=[&](std::uint32_t offset,bool value){auto* c=get(offset);if(!c){e="HUDControls unchecked visibility cache missing at "+std::to_string(offset);return false;}c->set_visible(value);return true;};
  if(!visible(0x388,true))return false;
  ui::SwfAsValue locations;if(!find(root,"_root.LocationsMenu",locations)||!assign(0x388,locations,nullptr,{})||
     !refresh(0x5f8,"btn_FastTravel",locations)||!refresh(0x628,"btn_LocTravel",locations))return false;
  if(!visible(0x598,customize66c!=0)||!visible(0x5c8,customize66c!=0)||
     !visible(0x4d8,customize66c==0)||!visible(0x568,customize66c==0))return false;
  if(customize66c&&(!visible(0x5f8,false)||!visible(0x388,false)))return false;
  constexpr std::uint32_t capture[]{0x4c,0x88,0x3b8,0x3e8,0x418,0x448,0x478,0x4a8,0x4d8,0x508,0x568};
  auto capture_layout=[&](auto& out){for(unsigned i=0;i<11;++i){auto* c=get(capture[i]);if(!c){e="HUDControls source layout capture lacks actual matrix receiver";return false;}
    out[i][0]=c->get_matrix().m_[0][2];out[i][1]=c->get_matrix().m_[1][2];}return true;};
  if(style==0){if(!captured66d){if(!capture_layout(initial_style0_674))return false;captured66d=1;}
   if(saved66f){e="Required original SetHUDPos4187a0 saved layout transform producer";return false;}}
  else if(style==1){if(!captured66e){if(!capture_layout(initial_style1_6cc))return false;captured66e=1;}
   if(saved670){e="Required original SetHUDPos4187a0 saved layout transform producer";return false;}}
  initialized=1;e.clear();return true;
 }
};
SourceControlCacheV63 source_controls_cache_v63;
bool source_controls_initialize_v63(std::string& e){
 if(!movie||reinterpret_cast<std::uintptr_t>(movie.get())!=source_controls_root658_v62){e="HUDControls init requires same LoadMenu3 generation";return false;}
 ui::HudManagerRequest request{};request.operation=ui::HudManagerOperation::saved_option;request.text="HUDStyle";ui::HudManagerResponse option{};
 if(!source_hud_world_v62||source_hud_world_v62(request,option,e)!=1)return false;
 struct Init{Impl& self;std::int32_t style;static bool run(void* raw,ui::SwfAsGraph& graph,std::string& e){auto& q=*static_cast<Init*>(raw);
  return q.self.source_controls_cache_v63.initialize(graph,q.self.source_controls_root658_v62,q.style,q.self.joystick,q.self.source_controls_cache8_v62,e);}} init{*this,option.value};
 return movie->menu_action_script(&init,Init::run,e);
}
