// One immutable table snapshot from the original grouped CFS streams.
bool borrow_source_campaign_game_object_arrays_v81(const SourceCampaignCandidateBorrowV55& expected,
 dh2::loader::GameObjectArraysOwnerV81::Borrow& out,std::string& e){
 SourceCampaignCandidateBorrowV55 actual;
 if(!borrow_source_campaign_candidate_v55(actual,e))return false;
 const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 if(!same(actual.actual_world,expected.actual_world)||!same(actual.application,expected.application)||
    !same(actual.level,expected.level)||!same(actual.objects,expected.objects)||
    actual.properties!=expected.properties||!renderer_immutable_tables_v55){
  e="Required SAME current campaign/process Arrays owner";return false;
 }
 auto& tables=*renderer_immutable_tables_v55;
 if(!tables.game_object_arrays_v81||!tables.game_object_arrays_v81->ready()){
  if(tables.game_object_arrays_attempted_v81){
   e=tables.game_object_arrays_failure_v81.empty()?"Incomplete actual game_objects process load":tables.game_object_arrays_failure_v81;return false;
  }
  tables.game_object_arrays_attempted_v81=true;
  tables.game_object_arrays_v81=std::make_shared<dh2::loader::GameObjectArraysOwnerV81>();
  std::shared_ptr<SourceWorldBorrowV61> world;
  if(!borrow_source_campaign_condition_world_v70(actual,world,e)||!world||!world->read){
   if(e.empty())e="Required SAME actual CFS table read authority";
   tables.game_object_arrays_failure_v81=e;return false;
  }
  std::vector<std::uint8_t> values,names,dictionary,dictionary_names;
  const auto read_table=[&](const char* name,auto& bytes){
   bool found{};
   if(!world->read(std::string("data/pydata/")+name,found,bytes,e))return false;
   if(!found){e=std::string("Missing original grouped object table resource: ")+name;return false;}
   return true;
  };
  if(!read_table("game_objects_pyarray.bin",values)||!read_table("game_objects_pyarraynames.bin",names)||
     !read_table("game_objects_dictionary_pyarray.bin",dictionary)||
     !read_table("game_objects_dictionary_pyarraynames.bin",dictionary_names)||
     !tables.game_object_arrays_v81->initialize(values.data(),values.size(),names.data(),names.size(),
       dictionary.data(),dictionary.size(),dictionary_names.data(),dictionary_names.size(),e)){
   tables.game_object_arrays_failure_v81=e;return false;
  }
 }
 return tables.game_object_arrays_v81->borrow(out,e);
}
