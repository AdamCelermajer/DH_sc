// Include in source_campaign_runtime_v61.cpp, inside namespace model_renderer,
// after source_campaign_v55 and borrow_source_campaign_candidate_runtime_v61.
bool bind_source_campaign_module_conditions_runtime_v75(const SourceCampaignCandidateBorrowV55& expected,
 const dh2::world::ConditionDataInitServicesV3& conditions,std::string& e){
 SourceCampaignCandidateBorrowV55 actual;
 if(!borrow_source_campaign_candidate_runtime_v61(actual,e))return false;
 const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 if(!same(actual.actual_world,expected.actual_world)||!same(actual.application,expected.application)||
    !same(actual.level,expected.level)||!same(actual.objects,expected.objects)||actual.properties!=expected.properties||
    !source_campaign_v55||source_campaign_v55->failed||!source_campaign_v55->candidate||!source_campaign_v55->candidate->graph||
    actual.level->constructor_fields_v3().field130>=7){
  e="Condition services must bind SAME current Module graph before stage7/XML constructors";return false;
 }
 return source_campaign_v55->candidate->graph->bind_condition_services_v75(conditions,e);
}
