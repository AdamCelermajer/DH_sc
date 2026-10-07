// Include after renderer_player_death_v56.inc. Concrete missing publishers
// for existing V43 Kill/V44 progression providers. No current-Level, ready145
// pool, profile writer or LevelUp presentation is invented by this adapter.
[[maybe_unused]] static bool renderer_player_kill_borrow_v56(PlayerSkillsRuntime& t,
 dh2::character::CharacterKillLiveBorrowV21& out,std::string& e){
 if(!t.world||!t.world->player_canonical||!t.world->player_object||!t.player||!t.player->ready()){
  e="Required retained SAME player before Kill borrow";return false;
 }
 auto& meta=*t.world->player_canonical;auto& object=*t.world->player_object;auto& view=t.player->session().property_view();
 const auto found=t.world_actors.find(object.identity);
 if(found==t.world_actors.end()||found->second.object.get()!=&object||
    found->second.shared_handle!=&meta.handle||meta.handle.cached!=object.identity||
    t.fsm.character!=object.identity||!meta.kill_fields.produced||!object.properties||!object.life||object.life->dead>1||
    view.resolved!=object.properties->resolved.data()||view.base!=object.properties->base.data()||
    view.saved!=object.properties->saved.data()||view.gear!=object.properties->gear.data()){
  e="Required identical player Kill sheets/life/FSM/C1/Handle authorities";return false;
 }
 dh2::character::CharacterKillLiveBorrowV21 b;
 b.identity=object.identity;b.controller=reinterpret_cast<std::uintptr_t>(&prince_runtime.controller);
 b.controllable_character=object.identity;b.receiver=t.world->player_object;
 b.properties=&view;b.life=object.life.get();b.fields=&meta.kill_fields;
 b.oid64=&meta.room64;b.property13c8=&meta.properties13c8;b.tracked14a4=&meta.ooi14a4;
 b.outgoing=&prince_combat.aggro.outgoing_table;b.shared_handle=&meta.handle;
 out=std::move(b);return true;
}
template<class SameKillProviders> static bool populate_kill_player_contracts_v56(PlayerSkillsRuntime& t,SameKillProviders& out,std::string& e){
 dh2::character::CharacterKillLiveBorrowV21 actual;
 if(!renderer_player_kill_borrow_v56(t,actual,e))return false;
 out.scope=[&t](){return t.player->session().current_skill_callback_scope();};
 out.player=[&t](auto& b,auto& error){return renderer_player_kill_borrow_v56(t,b,error);};
 out.player_died=[&t](auto attacker,const auto* scope,auto& error){return renderer_player_raise_death_v56(t,attacker,scope,error);};
 return true;
}
template<class SameProgressionProviders> static bool populate_progression_save14e8_v56(PlayerSkillsRuntime& t,SameProgressionProviders& out,std::string& e){
 if(!t.world||!t.world->player_canonical||!t.world->player_canonical->loot_fields_v49){e="Required actual produced Character.Save14e8 association";return false;}
 out.save_slot14e8=[&t](std::uintptr_t character,dh2::data::PlayerSavegameV1*& actual,std::string& error){
  actual=nullptr;
  if(!t.world||!t.world->player_object||!t.world->player_canonical||character!=t.world->player_object->identity||
     !t.world->player_canonical->loot_fields_v49){error="Required SAME source player Save14e8 receiver";return false;}
  dh2::character::LootCharacterSaveBorrowV44 borrow;
  if(!t.world->player_canonical->loot_fields_v49->borrow_save(t.world->player_object,borrow,error))return false;
  if(borrow.save){
   if(!t.player||borrow.save!=t.save.get()||borrow.save!=t.player->native_savegame()){error="Character.Save14e8 differs from retained native Save owner";return false;}
   actual=t.save.get(); // Proven same mutable retained owner; no const cast.
  }
  return true; // Genuine source NULL remains NULL, never auto-create a Save.
 };
 return true;
}
