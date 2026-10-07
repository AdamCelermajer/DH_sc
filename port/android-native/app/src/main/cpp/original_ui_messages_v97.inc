//Members of the ONE retained OriginalUiSession::Impl; queues survive HUD
//movie/camera generations, while message cached characters remain weak.
std::shared_ptr<SourceScriptUiWorldV97> script_ui_world_v97;
ui::DialogActorsV97 dialog_actors_v97;
std::shared_ptr<ui::MenuDialogMessagesV97> dialogs_v97;
ui::MenuAuxiliaryMessagesV97 auxiliary_messages_v97;
bool dialog_data_attempted_v97{};
std::string dialog_data_failure_v97;
bool ensure_dialog_data_v97(std::string& e){
 if(dialog_actors_v97.initialized())return true;
 if(dialog_data_attempted_v97){e=dialog_data_failure_v97;return false;}
 dialog_data_attempted_v97=true;
 std::vector<std::uint8_t> records,names,schema;
 if(!assets.read("data/pydata/dialogs_pyarray.bin",records,e)||
    !assets.read("data/pydata/dialogs_pyarraynames.bin",names,e)||
    !assets.read("data/pydata/dialogs_pystructnames.bin",schema,e)||
    !dialog_actors_v97.decode(records,names,schema,e)){
  dialog_data_failure_v97=e.empty()?"Actual DialogActors table initialization failed":e;return false;
 }return true;
}
bool invoke_message_v97(ui::MessageFamilyV66 family,const char* method,std::int32_t context,std::string& e){
 if(!loaded||!movie){e.clear();return true;} //actual GetHUDRoot NULL branch
 std::uintptr_t root{};
 if(!movie->menu_action_script(&root,[](void* raw,ui::SwfAsGraph& graph,std::string& error){
  ui::SwfAsValue value;if(!graph.root_value(value,error))return false;*static_cast<std::uintptr_t*>(raw)=value.identity();return true;
 },e))return false;
 if(!root){e.clear();return true;}
 return message_caches_v66.invoke(*movie,family,root,method,context,e);
}
std::shared_ptr<ui::MenuDialogMessagesV97> dialog_owner_v97(){
 if(dialogs_v97)return dialogs_v97;
 struct Provider {std::weak_ptr<Impl> ui;};auto owner=std::make_shared<Provider>();owner->ui=shared_from_this();
 ui::MenuDialogServicesV97 services;services.owner=owner;
 services.localized=[owner](auto id,auto& text,auto& e){
  auto ui=owner->ui.lock();if(!ui){e="Actual dialogue StringManager expired";return false;}
  bool null{};if(!ui->localization.integer_string(id,ui->text_services(),text,null,e))return false;
  //CString's NULL input becomes its constructor-empty native text.
  if(null)text.clear();return true;
 };
 services.actor_text_id=[owner](auto actor,auto& id,auto& e){
  auto ui=owner->ui.lock();if(!ui||!ui->ensure_dialog_data_v97(e))return false;
  const auto* row=ui->dialog_actors_v97.row(actor);
  if(!row){e="Original DialogMsg.SetActorName indexed outside actual Actor rows";return false;}
  id=row->name10;e.clear();return true;
 };
 services.style_name=[owner](auto style,auto& text,auto& e){
  auto ui=owner->ui.lock();if(!ui){e="Actual dialogue constant owner expired";return false;}
  if(ui->script_ui_world_v97)return ui->script_ui_world_v97->style_name(style,text,e);
  //A standalone, genuinely empty queue can still be inspected; use the same
  //existing UI constant owner, whose original missing-group result is empty.
  const auto* name=dh2_script_constants_name(ui->constants.get(),"DialogStyles",style);
  if(!name){e="Actual UI constant reverse lookup failed";return false;}text=name;e.clear();return true;
 };
 services.invoke=[owner](auto method,auto context,auto& e){
  auto ui=owner->ui.lock();if(!ui){e="Actual dialogue HUD provider expired";return false;}
  return ui->invoke_message_v97(dh2::ui::MessageFamilyV66::dialog,method,context,e);
 };
 dialogs_v97=std::make_shared<ui::MenuDialogMessagesV97>(std::move(services));return dialogs_v97;
}
static bool source_player_character_v97(void* context,std::uintptr_t& value,std::string& e){
 auto& ui=*static_cast<Impl*>(context);
 if(ui.script_ui_world_v97)return ui.script_ui_world_v97->player_character(value,e);
 return no_player(context,value,e); //existing genuine standalone NULL branch
}
static bool source_player_name_v97(void* context,std::uintptr_t value,std::string& text,std::string& e){
 auto& ui=*static_cast<Impl*>(context);
 if(ui.script_ui_world_v97)return ui.script_ui_world_v97->player_name(value,text,e);
 return unavailable_player_name(context,value,text,e);
}
bool achievement_native_v119(const char* name,const gameswf::fn_call& call,bool& handled,std::string& e){
 handled=false;if(!name)return true;
 if(!std::strcmp(name,"NativeGetNextAchievementMessage")){
  handled=true;
  //440278: invalid arity/type leaves the ActionScript result untouched.
  if(call.nargs!=1||!call.arg(0).is_object()){e.clear();return true;}
  auto* object=call.arg(0).to_object();
  if(!object){e="Original achievement reader dereferences NULL argument object";return false;}
  const auto* first=auxiliary_messages_v97.next_achievement();
  if(!first){if(call.result)call.result->set_bool(false);e.clear();return true;}
  //The original copies the front value before invoking five SetMember calls.
  //Keep that copy across synchronous AS reentry without pinning deque storage.
  const auto message=*first;
  object->set_member("Name",gameswf::as_value(message.title0.c_str()));
  object->set_member("Desc",gameswf::as_value(message.message18.c_str()));
  object->set_member("TrophyType",gameswf::as_value(message.field30));
  object->set_member("Grade",gameswf::as_value(message.field34));
  object->set_member("Label",gameswf::as_value(message.field38));
  if(call.result)call.result->set_bool(true);e.clear();return true;
 }
 if(!std::strcmp(name,"NativeSkipAchievementMessage")){
  handled=true;
  if(!auxiliary_messages_v97.next_achievement()){e.clear();return true;}
  auxiliary_messages_v97.skip_achievement();
  //442994: s_SkipFuncName9a54e8 is genuine BSSNULL. No guessed animation
  //callback. Only a remaining message invokes the original start method.
  if(auxiliary_messages_v97.next_achievement())
   return invoke_message_v97(ui::MessageFamilyV66::achievement,"onAchievementMessage",0,e);
  e.clear();return true;
 }
 return true;
}
bool message_native_v97(const char* name,const gameswf::fn_call& call,bool& handled,std::string& e){
 handled=false;if(!name)return true;
 if(!achievement_native_v119(name,call,handled,e))return false;
 if(handled)return true;
 if(!std::strcmp(name,"NativeGetNextTutorialMessage")){
  handled=true;if(call.nargs!=1||!call.arg(0).is_object())return true;
  auto* object=call.arg(0).to_object();if(!object){e="Original tutorial message dereferences NULL argument object";return false;}
  const auto* first=auxiliary_messages_v97.next_tutorial();
  //The shipping empty reader leaves its local scalars undefined. Native UI
  //returns the actual false queue result without exposing undefined data.
  if(!first){if(call.result)call.result->set_bool(false);e.clear();return true;}
  const auto message=*first;std::string text;bool is_null{};
  auto services=text_services();
  if(!localization.integer_string(message.word0,services,text,is_null,e))return false;
  if(is_null)text.clear();std::uintptr_t player{};
  if(!services.player_character||!services.player_name||!services.player_character(services.context,player,e))return false;
  if(player){
   if(!services.player_character(services.context,player,e)||!player){if(e.empty())e="Tutorial second local Character lookup became NULL";return false;}
   std::string name,parsed;if(!services.player_name(services.context,player,name,e)||
      !ui::localization_player(text,name,localization.pack()>=4&&localization.pack()<=6,parsed,e))return false;
   text=std::move(parsed);
  }
  object->set_member("TutorialMessage",gameswf::as_value(text.c_str()));
  object->set_member("TutorialDuration",gameswf::as_value(double(message.word4)));
  if(call.result)call.result->set_bool(true);e.clear();return true;
 }
 if(!std::strcmp(name,"NativeGetNextDialogMessage"))return ui::menu_dialog_native_v97(*dialog_owner_v97(),name,call,handled,e);
 if(!std::strcmp(name,"NativeStopMessage")){
  handled=true;
  if(call.nargs<1||!call.arg(0).is_string())return true;
  if(!script_ui_world_v97){
   //The existing standalone/legacy status owner remains its own authority.
   //Source-campaign dialogue enrollment below always installs the real PM
   //prelude; do not turn an absent dialogue transport into success.
   if(!std::strcmp(call.arg(0).to_string(),"status"))return message_owner()->stop_status(e);
   e="NativeStopMessage requires actual Application/PM prelude";return false;
  }
  if(!script_ui_world_v97->before_stop_message(e))return false;
  const auto* family=call.arg(0).to_string();
  if(!std::strcmp(family,"dialog")){
   if(!script_ui_world_v97->before_stop_dialog(e))return false;
   return dialog_owner_v97()->skip(e);
  }
  if(!std::strcmp(family,"status"))return message_owner()->stop_status(e);
  if(!std::strcmp(family,"online")){
   const bool had=!auxiliary_messages_v97.online_empty();auxiliary_messages_v97.skip_online();
   if(had&&!auxiliary_messages_v97.online_empty())return invoke_message_v97(ui::MessageFamilyV66::online,"onOnlineStatusMessage",0,e);
   e.clear();return true;
  }
  if(!std::strcmp(family,"tutorial"))return skip_tutorial_message_v118(e);
  if(!std::strcmp(family,"quest")){e="Required original quest message owner";return false;}
  e.clear();return true; //original unknown-category branch
 }
 return true;
}
bool skip_tutorial_message_v118(std::string& e){
 if(!auxiliary_messages_v97.next_tutorial()){e.clear();return true;}
 auxiliary_messages_v97.skip_tutorial();
 if(!invoke_message_v97(ui::MessageFamilyV66::tutorial,"skipTutorialMessage",0,e))return false;
 if(auxiliary_messages_v97.next_tutorial())return invoke_message_v97(ui::MessageFamilyV66::tutorial,"onTutorialMessage",0,e);
 e.clear();return true;
}
