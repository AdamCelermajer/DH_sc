//Methods of the retained process UI facade. GSInit11 remains the sole
//TrophyManager producer; these services own no Trophy/World/second UI queue.
bool OriginalUiSession::process_trophy_services_v119(
 const std::shared_ptr<application::ApplicationServicesOwnerV5>& app,
 ProcessTrophyServicesV100& out,std::string& e){
 if(!app||!impl_->manager||!impl_->debug||!impl_->process_arrays_v101){
  e="Required actual Application/process UI resource owners for GSInit";return false;
 }
 //The already-constructed UI Debug becomes the process singleton before any
 //World. A previously published process Debug is authoritative on adoption.
 if(app->source_debug_services_v55())impl_->debug=app->source_debug_services_v55();
 else if(!app->publish_source_debug_services_v55(impl_->debug,e))return false;
 const auto actual_files=app->source_save_files_v61();
 if(actual_files){
  if(!actual_files->belongs_to_application(app)||!actual_files->files()){
   e="Required SAME process FileManager for UI Debug directory";return false;
  }
  impl_->directory=actual_files->files()->directory_v59();
 }
 struct Provider {
  std::weak_ptr<application::ApplicationServicesOwnerV5> weak_app;
  std::shared_ptr<Impl> ui;
  character::DebugFileServices24 debug_files{this,open,close};
  static int open(void* raw,const char* name,std::uintptr_t* out){
   auto& self=*static_cast<Provider*>(raw);auto app=self.weak_app.lock();
   if(!app||!name||!out||std::strcmp(name,"DebugSwitches.savegame"))return 1;
   const auto files=app->source_save_files_v61();
   if(!files||!files->belongs_to_application(app)||!files->files())return 1;
   *out=0;errno=0;
   //Same FileManager private-directory authority; Debug's existing native
   //stdio transport keeps its original open/close-only load behavior.
   const auto path=files->files()->directory_v59()+"/"+name;
   auto* file=std::fopen(path.c_str(),"rb");
   if(!file)return errno==ENOENT?0:1;
   *out=reinterpret_cast<std::uintptr_t>(file);return 0;
  }
  static int close(void*,std::uintptr_t handle){
   return !handle||std::fclose(reinterpret_cast<std::FILE*>(handle))?1:0;
  }
  bool current(std::shared_ptr<application::ApplicationServicesOwnerV5>& app,std::string& e)const{
   app=weak_app.lock();
   if(!app||!ui||app->source_debug_services_v55()!=ui->debug){
    e="Actual process trophy provider Application/Debug expired or changed";return false;
   }
   e.clear();return true;
  }
 };
 auto provider=std::make_shared<Provider>();provider->weak_app=app;provider->ui=impl_;
 ProcessTrophyServicesV100 services;services.owner=provider;
 services.debug=impl_->debug;services.debug_files=&provider->debug_files;services.debug_file_owner=provider;
 services.cache=[provider](const char* uri,bool& found,std::vector<std::uint8_t>& bytes,std::string& e){
  std::shared_ptr<application::ApplicationServicesOwnerV5> app;
  if(!provider->current(app,e)||!uri)return false;
  const auto* group=provider->ui->process_arrays_v101->group("TrophyTable");
  if(!provider->ui->process_arrays_v101->ready()||!group||!group->records_loaded||
     !group->names_loaded||!group->members_loaded){
   e="Required actual GSInit3-loaded process TrophyTable before GSInit11";return false;
  }
  //Schemas are native retained compiler metadata, as in the SAME process
  //Arrays loader. Original record/name files use the shared APK cache reader.
  if(!std::strcmp(uri,"data/pydata/trophies_pystructnames.bin")){
   if(!provider->ui->raw_asset("data/trophies_pystructnames.bin",bytes,e))return false;
   found=true;return true;
  }
  return provider->ui->original(uri,found,bytes,e);
 };
 services.files=[provider](std::shared_ptr<application::ApplicationSaveFilesOwnerV61>& files,std::string& e){
  std::shared_ptr<application::ApplicationServicesOwnerV5> app;
  if(!provider->current(app,e))return false;
  files=app->source_save_files_v61(); //Observed NULL remains genuine NULL.
  if(files&&!files->belongs_to_application(app)){e="Different Application FileManager in trophy getter";return false;}
  e.clear();return true;
 };
 services.text=[provider](std::int32_t id,std::string& text,std::string& e){
  std::shared_ptr<application::ApplicationServicesOwnerV5> app;
  if(!provider->current(app,e))return false;
  if(!provider->ui->process_text_ready_v101){e="Required actual GSInit StringManager before Trophy text lookup";return false;}
  bool null{};
  if(!provider->ui->localization.integer_string(id,provider->ui->text_services(),text,null,e))return false;
  if(null){e="Original Trophy CString requires nonNULL localized text";return false;}
  return true;
 };
 services.in_game=[provider](bool& value,std::string& e){
  std::shared_ptr<application::ApplicationServicesOwnerV5> app;
  if(!provider->current(app,e))return false;
  loader::CanonicalCurrentLevelBorrowV1 current;
  if(!model_renderer::borrow_current_native_level_v27(current,e))return false;
  //380e48 uses GetCurrentLevel itself, not IsCurrentlyInGameView/byte198.
  value=bool(current);e.clear();return true;
 };
 services.achievement=[provider](const trophies::AchievementMessageV1& message,std::string& e){
  std::shared_ptr<application::ApplicationServicesOwnerV5> app;
  if(!provider->current(app,e))return false;
  auto& ui=*provider->ui;
  ui.auxiliary_messages_v97.enqueue_achievement({message.name,message.desc,message.type,message.grade,message.label});
  //Preserve the accepted queue prefix if the positive authored callback fails.
  if(ui.auxiliary_messages_v97.achievement_count()==1)
   return ui.invoke_message_v97(dh2::ui::MessageFamilyV66::achievement,"onAchievementMessage",0,e);
  e.clear();return true;
 };
 out=std::move(services);e.clear();return true;
}
