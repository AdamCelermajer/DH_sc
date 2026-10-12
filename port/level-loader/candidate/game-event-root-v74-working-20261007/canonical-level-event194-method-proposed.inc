// NEW method only. Adopt next to existing loading_fields_v26; original C1
// must already be complete. This creates no global, stage or current-Level.
bool CanonicalLevelContextV1::game_event_fields_v50(GameEventLevelFieldsV50& out,std::string& error){
 LoadingFieldsV26 loading;
 if(!loading_fields_v26(loading,error))return false;
 if(loading.level_owner.get()!=this||loading.identity!=identity()||loading.state130!=&constructor_fields_v3_.field130){error="GameEvent194 requires SAME completed source C1";return false;}
 out={std::move(loading.level_owner),identity(),&constructor_fields_v3_.field130,&constructor_fields_v3_.field194,&game_events194_};
 error.clear();return true;
}
// Full GameEventManager::Unload479968 includes objective Unregister and source
// effect teardown. Do not reset game_events194_ as an abort/Unload substitute;
// retain it through actual teardown and clear raw194 + lease only afterwards.
