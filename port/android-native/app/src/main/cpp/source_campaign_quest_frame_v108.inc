//Included inside model_renderer anonymous namespace by V76; one weak campaign
//transport over SAME Save/Quest/script/item owners. No frame or state copy.
struct QuestFrameTransportV108:std::enable_shared_from_this<QuestFrameTransportV108> {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::weak_ptr<dh2::character::CharacterMenuQuestsV51> quests;
 bool scope(SourceCampaignCandidateBorrowV55& c,std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e)const{
  w=world.lock();return w&&borrow_source_campaign_candidate_runtime_v61(c,e)&&c.actual_world==w->owner&&c.application==w->application?true:required(e,"live SAME Quest frame campaign");
 }
 bool actor(std::uintptr_t id,SourceCampaignCharacterBorrowV62& out,std::string& e)const{
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  return scope(c,w,e)&&borrow_source_campaign_character_v62(w->owner,id,out,e)&&out.character&&out.character->actor;
 }
 bool online(bool& value,std::string& e)const{
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!scope(c,w,e))return false;
  auto online=w->application->get_online_loading_v55();if(!online)return required(e,"actual COnline byte5");value=online->byte5()!=0;e.clear();return true;
 }
 bool local(SourceCampaignCharacterBorrowV62& out,std::string& e)const{
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!scope(c,w,e)||!w->player_manager)return required(e,"actual PM local0");
  dh2::player::PlayerInfoFieldsV1* info;if(!w->player_manager->get_local_player(0,true,info,e)||!info)return required(e,"source PlayerInfo dummy/local receiver");
  out={};if(info->character660)return borrow_source_campaign_character_v62(w->owner,info->character660,out,e);e.clear();return true;
 }
 bool constant(const char* group,const char* key,std::int32_t& value,std::string& e)const{
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!scope(c,w,e)||!w->design)return false;
  auto table=w->design->borrow();const auto* d=table.design();if(!d||!d->lookup||d->lookup(d->context,0,group,key,&value))return required(e,"actual Quest PyDataConstants");e.clear();return true;
 }
 bool collection(dh2::data::QuestSavegameV1*& out,std::shared_ptr<dh2::character::CharacterMenuQuestsV51>& hold,std::string& e)const{
  hold=quests.lock();if(!hold||!hold->save())return required(e,"actual retained QuestSavegame");
  bool net;if(!online(net,e))return false;out=net?&hold->save()->volatile_quests_v45():&hold->save()->regular_quests_v45();return true;
 }
 bool difficulty(std::uintptr_t id,std::int32_t& out,std::string& e)const{
  SourceCampaignCharacterBorrowV62 b;if(!actor(id,b,e)||!b.character->save_fields)return false;
  return dh2::player::character_game_difficulty_v29({b.character,id,b.character->save_fields->save_slot14e8(),b.character->services.difficulty_global},out,e);
 }
 bool progress(std::uint32_t field,std::int32_t value,std::int32_t requested,std::string& e)const{
  SourceCampaignCharacterBorrowV62 b;if(!local(b,e))return false;if(!b.character){e.clear();return true;} //Character SG wrappers' real NULL branch
  const auto& r=b.character;if(!r->save_fields||!r->save_fields->save_slot14e8())return required(e,"actual SG progress Save14e8");
  if(!*r->save_fields->save_slot14e8()){e.clear();return true;}
  auto selected=requested;if(selected==-1&&!difficulty(r->actor->shared_handle().cached,selected,e))return false;
  bool net;if(!online(net,e))return false;auto& q=net?r->save->volatile_quests_v45():r->save->regular_quests_v45();
  return q.source_store_progress_v108(field,value,selected,e);
 }
 bool transition_save(std::string& e)const{
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!scope(c,w,e))return false;
  dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,e))return false;
  if(current){if(current.level()!=c.level)return required(e,"captured current Level for Quest.HandleSave");
   const auto& f=c.level->constructor_fields_v3();
   if(f.save_ec){if(!w->native_level_c1_v25||!*w->native_level_c1_v25)return required(e,"actual Level.SG_BlockLevel receiver");
    auto bindings=(*w->native_level_c1_v25)->bindings();auto save=bindings?bindings->save():nullptr;
    if(!save||save.get()!=f.save_ec.get())return required(e,"SAME Level Save_ec");save->owner().fields().inhibit_save39=0;}
   std::shared_ptr<dh2::player::PlayerLevelQuickSaveV29> quick;if(!borrow_source_campaign_quicksave_v88(c,quick,e)||!quick->execute({c.level,current.identity(),&f.save_ec,&f.field130},false,e))return false;
  }
  //Original repeats local0 after Level callbacks; never retain earlier660.
  SourceCampaignCharacterBorrowV62 b;if(!local(b,e))return false;if(!b.character){e.clear();return true;}
  auto& r=*b.character;if(!r.save_fields||!r.save_fields->save_slot14e8())return required(e,"Character.SG_Block source field");
  if(!*r.save_fields->save_slot14e8()){e.clear();return true;}
  if(!r.load||!r.profile_bootstrap||!r.profile_bootstrap->campaign_writer())return required(e,"whole Character.SG_Save writer");
  r.load->source_block_store_v88(false);dh2::data::PlayerSaveWriteOwnerV1 writer(r.load,r.profile_bootstrap->campaign_writer()->write_services());return writer.save(e);
 }
 bool reward(std::uintptr_t id,const dh2::data::QuestRewardDefinitionV51& q,bool& given,std::string& e)const{
  given=false;SourceCampaignCharacterBorrowV62 b;if(!actor(id,b,e))return false;auto& r=*b.character;auto* gear=r.equipment?r.equipment.get():r.prepared_equipment_v60;
  auto cell=[&](std::uint32_t offset,std::int32_t value){dh2::world::GameObjectInitializationFieldsV62 f;if(!r.actor->inherited_initialization_fields_v62(b.character,f,e))return false;auto* p=f.integer?f.integer(offset):nullptr;if(!p)return required(e,"actual reward Character display cell");*p=value;return true;};
  std::int32_t raw;auto bits=std::uint32_t(q.parameter1)<<8;std::memcpy(&raw,&bits,4);
  switch(q.type){
   case 0:if(!gear||gear->inventory()!=r.inventory37c||!gear->loot_add_gold_v8(q.parameter1,e)||!cell(0x1500,q.parameter1))return required(e,"SAME Reward_Gold inventory37c");given=true;return true;
   case 1:{bool accepted;SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!scope(c,w,e)||!source_campaign_give_xp_v108(w->owner,id,raw,true,accepted,e))return false;if(accepted&&!cell(0x1504,raw))return false;given=true;e.clear();return true;}
   case 2:{bool player;if(!r.is_player(player,e))return false;if(!player){given=true;e.clear();return true;}
    if(raw<0){auto assertion=dh2::world::SourceAssertionProcessV76::borrow();auto mode=*assertion->source_level();if(mode==2)return required(e,"ModifyCharProps negative source null-store assertion");if(mode==1&&!assertion->report("..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\Character_Stats.cpp",0x18e,"amount >= 0",e))return false;}
    auto* view=r.player_script_owner_v62?&r.player_script_owner_v62->session().property_view():&r.view;
    if(dh2_property_add(view,q.parameter2,raw))return required(e,"actual PROPS_Add reward property");
    dh2::character::NpcInitPostRequestV1 request;request.subject=id;dh2::character::NpcInitPostResponseV1 out;
    request.source_entry=0x3defbc;if(!dh2::world::CanonicalCharacterCandidateRecordV60::init_service(&r,request,out,e))return false;
    if(!r.init_fields.properties_id13c8&&!r.actor->init_post_fields(r.init_fields,e))return false;
    request.source_entry=0x3df2a4;request.argument0=static_cast<std::uint32_t>(*r.init_fields.properties_id13c8);if(!dh2::world::CanonicalCharacterCandidateRecordV60::init_service(&r,request,out,e))return false;
    request.source_entry=0x3e0810;if(!dh2::world::CanonicalCharacterCandidateRecordV60::init_service(&r,request,out,e))return false;
    std::uint32_t tracing;if(!r.services.debug||!r.services.debug_files||dh2_character_debug_load(r.services.debug,r.services.debug_files)!=1||dh2_character_debug_get(&tracing,r.services.debug,"isTracingChar_Stats",r.services.debug_files)!=1)return required(e,"actual ModifyCharProps Debug");given=true;return true;
   }
   case 3:{SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!scope(c,w,e)||!source_campaign_reward_drop_v108(w->owner,id,q.parameter2,e))return false;given=true;return true;}
   case 4:{bool consumed;if(!gear||gear->inventory()!=r.inventory37c||!gear->try_consuming_source_v108(q.parameter2,q.parameter1,consumed,e))return required(e,"SAME Reward_ConsumeLoot inventory37c");
    //Source performs one ignored FindItem only when TryConsuming returns0.
    if(!consumed){const auto* inventory=gear->inventory();const auto* found=inventory->potion();if(!found||found->id!=q.parameter2){found=nullptr;for(const auto& slot:inventory->items())if(slot&&slot->item&&slot->item->id==q.parameter2){found=slot->item.get();break;}}(void)found;}
    e.clear();return true; //Original ConsumeLoot.Give returns0 in BOTH branches.
   }
   default:return required(e,"source Reward table type0..4");
  }
 }
 dh2::world::NativeQuestFrameServicesV108 services(){
  auto self=shared_from_this();dh2::world::NativeQuestFrameServicesV108 s;s.provider=self;
  s.constant=[self](auto group,auto key,auto& value,auto& e){return self->constant(group,key,value,e);};
  s.application_time70=[](auto& time,auto& e){return borrow_application_time_v68(time,e);};
  s.online=[self](auto& value,auto& e){return self->online(value,e);};
  s.frame_gate=[self](bool& permitted,std::string& e){permitted=true;SourceCampaignCharacterBorrowV62 b;if(!self->local(b,e))return false;if(!b.character){e.clear();return true;}
   auto& r=*b.character;if(!r.save_fields||!r.save_fields->save_slot14e8())return required(e,"Quest.Update actual Save14e8");
   if(!*r.save_fields->save_slot14e8()){permitted=false;e.clear();return true;}
   if(!r.save||*r.save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(r.save.get()))return required(e,"Quest.Update SAME Save association");
   if(!r.save->source_quest_sync_ready14_v3()){permitted=false;e.clear();return true;}
   bool net;if(!self->online(net,e))return false;if(!net){e.clear();return true;}
   bool hosting;if(!r.services.quest_sync.local_hosting||!r.services.quest_sync.local_hosting(hosting,e))return required(e,"actual PM.IsLocalHosting");
   if(hosting){e.clear();return true;}return required(e,"actual ScriptManager160 online quest gate");
  };
  s.script_id=[self](const char* value,std::int32_t& id,std::string& e){id=-1;if(!value){e.clear();return true;}auto* dot=std::strchr(value,'.');if(!dot){e.clear();return true;}
   SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!self->scope(c,w,e))return false;auto manager=w->application->source_script_manager_v52();if(!manager)return required(e,"actual Quest script manager");id=manager->id_from_name(dot+1,false);e.clear();return true;};
  s.script_running=[self](auto id,auto& running,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!self->scope(c,w,e))return false;auto m=w->application->source_script_manager_v52();return m?m->is_script_running_v96(id,running,e):required(e,"actual script wait");};
  s.start_script=[self](auto id,auto type,auto execute,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!self->scope(c,w,e))return false;auto m=w->application->source_script_manager_v52();return m?m->start_script_v96(id,type,execute,e):required(e,"actual Quest script start");};
  s.transition_save=[self](auto& e){return self->transition_save(e);};
  s.current_quest=[self](auto value,auto diff,auto& e){return self->progress(0x2c,value,diff,e);};
  s.current_primary=[self](auto value,auto diff,auto& e){return self->progress(0x38,value,diff,e);};
  s.current_act=[self](auto value,auto diff,auto& e){return self->progress(0x44,value,diff,e);};
  s.new_dialog=[self](const auto& q,auto& e){std::int32_t title,style;if(!q.definition||!self->constant("StrID","GLOBAL_QUEST_NEW",title,e)||!self->constant("DialogStyles","QuestMsgDialog",style,e))return false;
   SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!self->scope(c,w,e))return false;dh2::android_ui::SourceScriptUiLeavesV96 ui;if(!build_native_source_script_ui_leaves_v99(c,ui,e)||!ui.enqueue_dialog)return false;return ui.enqueue_dialog(title,q.definition->text_fields[1],style,-1,0,true,e);};
  s.completed_dialog=[self](const auto& q,const auto& rewards,auto& e){std::int32_t style;if(!q.definition||!self->constant("DialogStyles","QuestCompletedMsgDialog",style,e))return false;SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;return self->scope(c,w,e)&&source_native_quest_completed_dialog_v108(w->owner,q.definition->text_fields[1],style,rewards,e);};
  s.give_reward=[self](auto owner,const auto& row,auto& given,auto& e){return self->reward(owner,row,given,e);};
  s.online_activation=[](auto& e){return required(e,"original online Quest activation message");};
  s.online_act=[](auto& e){return required(e,"original online Quest current-act message");};
  s.is_local=[self](auto id,auto& local,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;return self->scope(c,w,e)&&w->player_manager&&w->player_manager->source_is_local_player_v61(id,local,e);};
  s.all_quests_trophy=[self](auto id,auto& e){SourceCampaignCharacterBorrowV62 b;if(!self->actor(id,b,e))return false;auto q=b.character->profile_bootstrap?b.character->profile_bootstrap->quest_owner_v70():nullptr;if(!q)return required(e,"actual all-quests collection");
   std::int32_t diff;if(!self->difficulty(id,diff,e)||diff<0||diff>=3)return required(e,"actual all-quests difficulty");bool net;if(!self->online(net,e))return false;auto& collection=net?b.character->save->volatile_quests_v45():b.character->save->regular_quests_v45();
   const auto count=collection.source_quests_v45()[std::size_t(diff)].size();
   for(std::size_t index=0;index<count;++index){
    //Source repeats SG_GetQuestByID(index,-1), including CompileQuests(false)
    //and fresh global difficulty/online selectors on EACH iteration.
    if(!self->difficulty(id,diff,e)||diff<0||diff>=3||!self->online(net,e))return required(e,"actual live all-quests selector");
    auto& current=net?b.character->save->volatile_quests_v45():b.character->save->regular_quests_v45();
    dh2::world::QuestConditionCompileBorrowV70 fields{std::shared_ptr<void>(b.character->save,&current),&current,current.source_character5c_v70(),current.source_compiled28_v70()};
    dh2::world::QuestConditionCompileServicesV70 compile;compile.transport=self;
    compile.character_difficulty3bb8e4=[self](auto owner,auto& value,auto& e){return self->difficulty(owner,value,e);};
    compile.quest_compile480178=[q](auto quest,auto& e){return q->compile_quest_v76(quest,e);};
    if(!dh2::world::quest_condition_compile_v70(fields,false,compile,e))return false;
    const auto& list=current.source_quests_v45()[std::size_t(diff)];if(index>=list.size())return required(e,"actual all-quests live slot");
    auto* quest=q->resolve_v70(list[index]);if(!quest)return required(e,"actual all-quests receiver");if(quest->state<=12){e.clear();return true;}
   }
   SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!self->scope(c,w,e))return false;dh2::android_ui::ProcessTrophyBorrowV100 trophy;if(!dh2::android_ui::borrow_source_process_trophies_v100(w->application,trophy,e)||!trophy.manager)return required(e,"actual process TrophyManager");
   if(trophy.manager->unlock(trophy.manager->catalog().index("quest_allcomplete"))!=0){e=trophy.manager->error();return false;}e.clear();return true;};
  return s;
 }
};
