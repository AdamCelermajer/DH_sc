//Actual SetData allocation, PlayAnimFXStep and callback ancestry. This is a
//part of the one existing V4 manager and borrows its source library pools.
std::shared_ptr<CharacterMeshFxOwnerV4::Impl::Set> CharacterMeshFxOwnerV4::Impl::set_data_v118(std::uintptr_t id){
 for(const auto& data:sets)if(reinterpret_cast<std::uintptr_t>(data.get())==id)return data;return {};
}
bool CharacterMeshFxOwnerV4::Impl::play_set_v118(std::int32_t id,const float* p,const float* rotation,std::uintptr_t anchor,const std::shared_ptr<Set>& parent,std::uintptr_t* out){
 if(id<0||std::size_t(id)>=tables.sets().size())return true;
 if(!set_dispatch_depth_v118)set_dispatch_count_v118=0;
 if(++set_dispatch_count_v118>65536||set_dispatch_depth_v118>=4096){failure="Authored FX redirect/sequence dispatch exceeds native lifetime bound";return false;}
 ++set_dispatch_depth_v118;struct Depth{std::size_t& n;~Depth(){--n;}}depth{set_dispatch_depth_v118};
 const auto& row=tables.sets()[id];if(row.steps.empty()){failure="Original FX set has no selected step";return false;}
 std::uint32_t index=0;
 if(row.type==2){if(!set_random_v118){failure="Required SAME source Random for authored FX set";return false;}
  if(!set_random_v118(static_cast<std::uint32_t>(row.steps.size()),index,failure))return false;
  if(index>=row.steps.size()){failure="Source FX Random returned outside authored step array";return false;}}
 auto data=std::make_shared<Set>();data->id=id;data->step=index;data->loops=row.loop;data->anchor=anchor;data->parent=parent;
 std::copy_n(p,3,data->position);if(rotation){std::copy_n(rotation,3,data->rotation);data->explicit_rotation=true;}
 //GetAnimFXData precedes list insertion in shipping Play; validate the
 //selected scalar domain before publishing, without changing any pool.
 const auto& step=row.steps[index];FxStep24V1 native{step.orient_once,step.orient_with_anchor,step.scale_with_anchor,0,step.loop,step.play_time};
 std::memcpy(&native.speed,&step.speed_bits,4);FxData32V1 projected{};
 if(dh2_fx_data_v1(&projected,&native,row.type,data->loops,reinterpret_cast<std::uintptr_t>(data.get()))){failure="Invalid source authored FX step data";return false;}
 sets.push_back(data);if(!publish_set_v88(data))return false;
 return play_step_v118(data,out,true);
}
bool CharacterMeshFxOwnerV4::Impl::play_step_v118(const std::shared_ptr<Set>& data,std::uintptr_t* out,bool initial){
 if(!data||data->id<0||std::size_t(data->id)>=tables.sets().size()){failure="Required actual live FX SetData";return false;}
 const auto& row=tables.sets()[data->id];if(data->step>=row.steps.size()){failure="Source FX sequence step outside authored array";return false;}
 const auto& step=row.steps[data->step];
 if(initial&&step.file==-1)return play_file_v118(-1,data,out,initial); //original initial direct-file branch
 bool redirect=step.redir!=0;std::int32_t destination=step.file;
 if(source_libraries){auto* library=source_libraries->set_info_v88(data->id);
  if(!library){failure="Required SAME authored FX SetInfo for selected step";return false;}
  //BuildLibraries skips source File==-1 entries; selected runtime index is
  //the actual packed steps4 index, not an invented raw-row replacement.
  if(data->step>=library->steps4.size()||!library->steps4[data->step]){failure="Selected source FX step missing from actual steps4";return false;}
  redirect=library->steps4[data->step]->redirect0!=0;destination=library->steps4[data->step]->file_or_set4;
 }
 const bool rotated=initial?data->explicit_rotation:(data->rotation[0]!=0.f||data->rotation[1]!=0.f||data->rotation[2]!=0.f);
 if(redirect){const float* rotation=rotated?data->rotation:nullptr;
  return play_set_v118(destination,data->position,rotation,data->anchor,data,out);}
 return play_file_v118(destination,data,out,initial);
}
bool CharacterMeshFxOwnerV4::Impl::play_file_v118(std::int32_t file,const std::shared_ptr<Set>& data,std::uintptr_t* out,bool initial){
 if(!module())return failure.empty(); //original Debug/Module BEFORE _GetAnimFX
 if(file<0)return true; //real _GetAnimFX negative-ID NULL branch
 if(std::size_t(file)>=tables.dictionary().values.size()){failure="FX source file outside actual dictionary";return false;}
 std::shared_ptr<Runtime> r;auto& pool=free[file];auto& current=active[file];
 if(!pool.empty()){auto identity=pool.back();auto found=records.find(identity);if(found==records.end()){failure="Lost SAME free FX instance";return false;}
  r=found->second;std::fill_n(r->visual_scale,3,1.f);pool.pop_back();r->pooled=false;++warm;
 }else{if(current.size()>5)return true;
  r=std::make_shared<Runtime>();r->state.file=file;r->uri=tables.dictionary().values[file];r->resource=std::make_shared<Resource>();
  if(!r->resource->load(assets,r->uri,factory,*live,failure))return false;
  r->state.visual=reinterpret_cast<std::uintptr_t>(r->resource.get());r->state.loop=-1;r->timeline.scale=1;r->timeline.library_present=1;
  if(dh2_timeline_clip(&r->timeline,0,r->resource->start_ms(),r->resource->end_ms())){failure="FX source timeline construction";return false;}
  records.emplace(reinterpret_cast<std::uintptr_t>(r.get()),r);++cold;
 }
 const auto identity=reinterpret_cast<std::uintptr_t>(r.get());current.push_back(identity);if(!publish_active_v88(r))return false;
 const auto& row=tables.sets()[data->id];const auto& step=row.steps[data->step];
 FxStep24V1 native{step.orient_once,step.orient_with_anchor,step.scale_with_anchor,0,step.loop,step.play_time};std::memcpy(&native.speed,&step.speed_bits,4);FxData32V1 projected{};
 if(dh2_fx_data_v1(&projected,&native,row.type,data->loops,reinterpret_cast<std::uintptr_t>(data.get()))){failure="FX step source projection";return false;}
 auto callbacks=state_services();const bool rotated=initial?data->explicit_rotation:(data->rotation[0]!=0.f||data->rotation[1]!=0.f||data->rotation[2]!=0.f);
 const float* rotation=rotated?data->rotation:nullptr;
 if(dh2_fx_play_v1(&r->state,data->position,rotation,data->anchor,&projected,1,&callbacks)){if(failure.empty())failure="FX step play source delivery";return false;}
 if(out)*out=identity;return true;
}
bool CharacterMeshFxOwnerV4::Impl::sequence_v118(const std::shared_ptr<Set>& input,bool parent){
 auto selected=parent?input->parent.lock():input;if(!selected){failure="Expired source FX sequence parent";return false;}
 if(++set_dispatch_count_v118>65536||set_dispatch_depth_v118>=4096){failure="Authored FX callback ancestry exceeds native lifetime bound";return false;}
 ++set_dispatch_depth_v118;struct Depth{std::size_t& n;~Depth(){--n;}}depth{set_dispatch_depth_v118};
 const auto& row=tables.sets()[selected->id];
 if(row.type!=1){auto up=selected->parent.lock();if(!up)return true;up->finished=true;return sequence_v118(selected,true);}
 //Source walks the actual set's active10 list, including other invocations
 //whose finished byte was written before this synchronous callback.
 for(std::size_t i=0;i<sets.size();++i){auto data=sets[i];if(data->id!=selected->id||!data->finished)continue;data->finished=false;
  if(data->step+1<row.steps.size()){++data->step;if(!play_step_v118(data,nullptr))return false;continue;}
  if(data->loops>0||data->loops==-1){if(data->loops>0)--data->loops;data->step=0;if(!play_step_v118(data,nullptr))return false;continue;}
  if(!data->parent.expired()&&!sequence_v118(input,true))return false;
 }
 return true;
}
bool CharacterMeshFxOwnerV4::Impl::loop_callback_v118(const std::shared_ptr<Set>& data){
 if(!set_dispatch_depth_v118)set_dispatch_count_v118=0;
 if(tables.sets()[data->id].type==1)return sequence_v118(data,false);
 if(auto parent=data->parent.lock()){parent->finished=true;if(tables.sets()[parent->id].type==1)return sequence_v118(data,true);}
 return true;
}
void CharacterMeshFxOwnerV4::bind_set_random_v118(std::function<bool(std::uint32_t,std::uint32_t&,std::string&)> random){impl_->set_random_v118=std::move(random);}
