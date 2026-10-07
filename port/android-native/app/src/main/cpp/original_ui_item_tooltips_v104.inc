//Inside OriginalUiSession::Impl. ONE missing process SWFAnimManager bank,
//distinct from FlashAnimManager's temporary scrolling-text contexts.
struct ItemAnimV104 {ui::DebugCachedCharacterV66 cached;std::uintptr_t movie{},generation{};bool free4{true};};
std::map<std::string,std::vector<std::unique_ptr<ItemAnimV104>>> item_anim_banks_v104;
struct ItemTooltipV104 {std::unique_ptr<ui::SwfAnimTooltipV104> source;};
std::map<std::uintptr_t,std::unique_ptr<ItemTooltipV104>> item_tooltips_v104;
bool item_anim_scope_v104(const std::function<bool(ui::SwfAsGraph&,std::string&)>& operation,std::string& e){
 if(!loaded||!movie){e="Required actual HUD3/MenuFX for source SWFAnimManager";return false;}
 auto call=operation;return movie->menu_action_script(&call,[](void* p,ui::SwfAsGraph& graph,std::string& e){return (*static_cast<std::function<bool(ui::SwfAsGraph&,std::string&)>*>(p))(graph,e);},e);
}
bool item_anim_character_v104(ui::SwfAsGraph& graph,std::uintptr_t id,ItemAnimV104*& anim,gameswf::character*& clip,std::string& e){
 anim=nullptr;clip=nullptr;for(auto& bank:item_anim_banks_v104)for(auto& candidate:bank.second)if(reinterpret_cast<std::uintptr_t>(candidate.get())==id)anim=candidate.get();
 if(!anim||!movie||anim->movie!=reinterpret_cast<std::uintptr_t>(movie.get())||anim->generation!=movie->player_identity()){e="Released/foreign SAME SWFAnim cached HUD generation";return false;}
 ui::SwfAsValue value;if(!anim->cached.get_character(graph,value,e))return false;
 gameswf::as_object* object{};if(!graph.borrow_object(value,object,e)||!object||!object->is(gameswf::character::m_class_id)){if(e.empty())e="Required actual SWFAnim CachedChar character";return false;}
 clip=static_cast<gameswf::character*>(object);return true;
}
bool grab_item_anim_v104(const char* name,std::uintptr_t& out,std::string& e){
 out=0;if(!name){e="Required original SWFAnim bank CString";return false;}
 return item_anim_scope_v104([&](auto& graph,auto& e){auto& bank=item_anim_banks_v104[name];
  for(auto& anim:bank)if(anim->free4){ItemAnimV104* actual{};gameswf::character* clip{};if(!item_anim_character_v104(graph,reinterpret_cast<std::uintptr_t>(anim.get()),actual,clip,e))return false;
   actual->free4=false;out=reinterpret_cast<std::uintptr_t>(actual);return true;}
  if(bank.size()>=4096){e="Native SWFAnim bank resource admission exhausted";return false;}
  ui::SwfAsValue value;gameswf::as_object* object{};
  if(bank.empty()){ui::SwfAsValue root;if(!graph.root_value(root,e)||!graph.find_target(root,name,value,e))return false;
   if(!value.identity())return true; //Original missing source character reaches caller assertion.
  }else{
   ItemAnimV104* last{};gameswf::character* clip{};if(!item_anim_character_v104(graph,reinterpret_cast<std::uintptr_t>(bank.back().get()),last,clip,e))return false;
   auto* parent=clip->get_parent();if(!parent||!parent->is(gameswf::sprite_instance::m_class_id)){e="Required actual source SWFAnim clone parent";return false;}
   char suffix[12]{};std::snprintf(suffix,sizeof suffix,"_%02u",unsigned(bank.size()));
   auto* clone=clip->clone_display_object(tu_string((std::string(name)+suffix).c_str()),static_cast<gameswf::sprite_instance*>(parent)->get_highest_depth());
   if(!clone||!graph.retain_object(clone,value,e)){if(e.empty())e="Actual SWFAnim ExtendBank clone failed";return false;}
   clone->set_visible(false); //4987f4 hides only newly cloned bank entries.
  }
  if(!graph.borrow_object(value,object,e)||!object||!object->is(gameswf::character::m_class_id)){if(e.empty())e="Required original SWFAnim source character";return false;}
  auto anim=std::make_unique<ItemAnimV104>();anim->movie=reinterpret_cast<std::uintptr_t>(movie.get());anim->generation=movie->player_identity();
  ui::SwfAsValue parent;if(!graph.root_value(parent,e)||!anim->cached.refresh_character(graph,value,anim->movie,&parent,e))return false;
  anim->free4=false;out=reinterpret_cast<std::uintptr_t>(anim.get());bank.push_back(std::move(anim));return true;
 },e);
}
bool drop_item_anim_v104(std::uintptr_t id,std::string& e){if(!id){e.clear();return true;}
 return item_anim_scope_v104([&](auto& graph,auto& e){ItemAnimV104* anim{};gameswf::character* clip{};if(!item_anim_character_v104(graph,id,anim,clip,e))return false;clip->set_visible(false);anim->free4=true;return true;},e);
}
ui::SwfAnimTooltipServicesV104 item_tooltip_services_v104(std::shared_ptr<void> actual_files){
 ui::SwfAnimTooltipServicesV104 s;s.owner=std::move(actual_files);
 const std::weak_ptr<Impl> weak=shared_from_this();
 s.grab=[weak](const char* name,auto& out,auto& e){auto ui=weak.lock();return ui&&ui->grab_item_anim_v104(name,out,e);};
 s.drop=[weak](auto id,auto& e){auto ui=weak.lock();return ui&&ui->drop_item_anim_v104(id,e);};
 s.play=[weak](auto id,const char* label,auto& e){auto ui=weak.lock();return ui&&ui->item_anim_scope_v104([&](auto& graph,auto& e){ItemAnimV104* anim{};gameswf::character* clip{};if(!ui->item_anim_character_v104(graph,id,anim,clip,e))return false;
   if(clip->goto_labeled_frame(label))clip->set_play_state(gameswf::character::PLAY);return true;},e);}; //source ignores missing-label bool.
 s.visibility=[weak](auto id,bool value,auto& e){auto ui=weak.lock();return ui&&ui->item_anim_scope_v104([&](auto& graph,auto& e){ItemAnimV104* anim{};gameswf::character* clip{};if(!ui->item_anim_character_v104(graph,id,anim,clip,e))return false;clip->set_visible(value);return true;},e);};
 s.visible=[weak](auto id,bool& out,auto& e){auto ui=weak.lock();return ui&&ui->item_anim_scope_v104([&](auto& graph,auto& e){ItemAnimV104* anim{};gameswf::character* clip{};if(!ui->item_anim_character_v104(graph,id,anim,clip,e))return false;out=true;return true;},e);}; //496cdc GetChar then literal1.
 s.playing=[weak](auto id,bool& out,auto& e){auto ui=weak.lock();return ui&&ui->item_anim_scope_v104([&](auto& graph,auto& e){ItemAnimV104* anim{};gameswf::character* clip{};if(!ui->item_anim_character_v104(graph,id,anim,clip,e))return false;out=clip->get_play_state()!=gameswf::character::STOP;return true;},e);};
 s.dt=[](auto& out,auto& e){return model_renderer::borrow_application_dt_v93(out,e);};
 s.missing_anim_assertion=[](auto& e){auto process=world::SourceAssertionProcessV76::borrow();if(!process){e="Required actual tooltip assertion mode";return false;}
  const auto mode=*process->source_level();if(mode==2){e="Original SWFAnimToolTip C1 deliberate NULL-store assertion";return false;}
  if(mode==1)return process->report("..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\SWFAnim\\SWFAnimTooltip.cpp",13,"m_swfAnim != 0",e);return true;};
 return s;
}
bool item_tooltip_position_v104(std::uintptr_t anim,const float* point,std::string& e){
 std::int32_t px{},py{};if(!point||!combat_project(this,point,&px,&py,e))return false;
 return item_anim_scope_v104([&](auto& graph,auto& e){ItemAnimV104* actual{};gameswf::character* clip{};if(!item_anim_character_v104(graph,anim,actual,clip,e))return false;
  if(!clip->is(gameswf::sprite_instance::m_class_id)){e="Required actual SWFAnim root-definition receiver";return false;}
  auto* root=static_cast<gameswf::sprite_instance*>(clip)->get_root();if(!root||!root->m_def||!root->m_viewport_width||!root->m_viewport_height){e="Required actual SWFAnim source rectangle/viewport";return false;}
  const auto& rectangle=root->m_def->m_frame_size;const float sx=((rectangle.m_x_max-rectangle.m_x_min)/20.f)/float(root->m_viewport_width);
  const float sy=((rectangle.m_y_max-rectangle.m_y_min)/20.f)/float(root->m_viewport_height);
  const auto old=clip->get_matrix();auto next=old;next.m_[0][2]=float(int(float(px)*sx))*20.f;next.m_[1][2]=float(int(float(py)*sy))*20.f;clip->set_matrix(next);return true;
 },e);
}
