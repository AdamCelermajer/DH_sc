// Included at native root after the actual current-Level/profile borrowers.
// Only this synchronous original Load callback scope admits inactive fields.
bool dh2::android_ui::CharacterPanelSessionV1::dispatch_source_loading_v98(
 const model_renderer::PlayerGameplayBinding& expected,const char* name,
 dh2::ui::CharacterMenuCallV1& call,std::string& e){
 dh2::loader::CanonicalCurrentLevelBorrowV1 current;model_renderer::PlayerGameplayBinding actual;
 if(!expected.world_owner||!name||!model_renderer::borrow_current_native_level_v27(current,e)||!current||
    current.level()->constructor_fields_v3().field130<12||current.level()->constructor_fields_v3().field130>=38||
    !model_renderer::borrow_source_campaign_player_pre_gameplay_v68(expected.world_owner,actual,e)||
    actual.active||actual.world_owner.get()!=expected.world_owner.get()||actual.world_owner.owner_before(expected.world_owner)||
    expected.world_owner.owner_before(actual.world_owner)||actual.character!=expected.character||
    actual.save!=expected.save||actual.skills!=expected.skills||actual.gear!=expected.gear){
  if(e.empty())e="Required SAME genuine source-loading selected profile at Level12";return false;
 }
 const std::string function(name);
 const bool readable=function=="NativeReloadSkills"||function.rfind("NativeGet",0)==0||
  function.rfind("NativeInvGet",0)==0||function.rfind("NativeHUDGet",0)==0||
  function.rfind("NativeSkillGet",0)==0||function.rfind("NativeSkillsGet",0)==0;
 if(!readable){e="Character action unavailable before actual gameplay admission: "+function;return false;}
 if(function=="NativeReloadSkills"&&current.level()->constructor_fields_v3().field130!=12){e="Startup ReloadSkills requires original primary1 Load at Level12";return false;}
 struct Scope {bool& field;bool prior;Scope(bool& f):field(f),prior(f){field=true;}~Scope(){field=prior;}} scope(source_loading_profile_scope_v98_);
 return dispatch(actual,name,call,e);
}
