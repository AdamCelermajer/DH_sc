// Included inside namespace dh2::fx after CharacterMeshFxOwnerV4::Impl.
bool CharacterMeshFxOwnerV4::grab_marker_v28(std::int32_t set,std::uintptr_t anchor,std::uintptr_t& out,std::string& error){
 error.clear();out=0;auto& s=*impl_;s.failure.clear();
 // The shipping module check precedes the signed set bounds.
 if(!s.module()){error=s.failure;return error.empty();}
 if(!s.tables){error="Required actual marker Effects tables";return false;}
 if(set<0||std::size_t(set)>=s.tables.sets().size())return true;
 const auto& row=s.tables.sets()[set];
 if(row.steps.empty()){error="Original GrabAnimFX set has no first authored step";return false;}
 //Grab selects its resource from authored step0 BEFORE the random data row.
 const auto file=row.steps[0].file;
 if(file<0||std::size_t(file)>=s.tables.dictionary().values.size())return true;
 std::shared_ptr<Impl::Runtime> r;auto& pool=s.free[file];auto& active=s.active[file];
 if(!pool.empty()){
  const auto identity=pool.back();r=s.records.at(identity);
  // _GetAnimFX494bdc calls VisualObject.SetScaling472708(1.0).
  std::fill_n(r->visual_scale,3,1.f);
  pool.pop_back();r->pooled=false;++s.warm;
 }else{
  if(active.size()>5)return true;
  r=std::make_shared<Impl::Runtime>();r->state.file=file;
  r->uri=s.tables.dictionary().values[file];r->resource=std::make_shared<Resource>();
  if(!r->resource->load(s.assets,r->uri,s.factory,*s.live,error))return false;
  r->state.visual=reinterpret_cast<std::uintptr_t>(r->resource.get());r->state.loop=-1;
  r->timeline.scale=1;r->timeline.library_present=1;
  if(dh2_timeline_clip(&r->timeline,0,r->resource->start_ms(),r->resource->end_ms())){error="Marker timeline construction";return false;}
  s.records.emplace(reinterpret_cast<std::uintptr_t>(r.get()),r);++s.cold;
 }
 const auto identity=reinterpret_cast<std::uintptr_t>(r.get());active.push_back(identity);if(!s.publish_active_v88(r)){error=s.failure;return false;}
 auto data=std::make_shared<Impl::Set>();data->id=set;
 data->loops=row.loop;data->anchor=anchor;
 if(row.type==2){if(!s.set_random_v118||!s.set_random_v118(static_cast<std::uint32_t>(row.steps.size()),data->step,error)){if(error.empty())error="Required source Random for GrabAnimFX";return false;}
  if(data->step>=row.steps.size()){error="GrabAnimFX Random outside actual step array";return false;}}
 const auto& step=row.steps[data->step];
 const auto data_identity=reinterpret_cast<std::uintptr_t>(data.get());s.sets.push_back(data);if(!s.publish_set_v88(data)){error=s.failure;return false;}
 auto services=s.state_services();r->state.anchor=anchor;
 // Exact Grab suffix: SetAnchor/Sync, explicit Sync(true), looping, Start,
 // SetAnimData with the source override -1, then SetVisible(true).
 if(!s.sync(*r,true)||!s.sync(*r,true)||dh2_timeline_loop(&r->timeline,1)||dh2_timeline_jump(&r->timeline,r->timeline.start_ms)){
  error=s.failure.empty()?"Marker grab source prefix":s.failure;return false;
 }
 r->state.looping=1;
 FxStep24V1 native{step.orient_once,step.orient_with_anchor,step.scale_with_anchor,0,step.loop,step.play_time};
 std::memcpy(&native.speed,&step.speed_bits,4);FxData32V1 projected{};
 if(dh2_fx_data_v1(&projected,&native,row.type,row.loop,data_identity)){error="Marker source SetAnimData projection";return false;}
 projected.loop=-1;
 if(dh2_fx_set_anim_v1(&r->state,&projected,1,&services)){
  error=s.failure.empty()?"Marker source SetAnimData receiver":s.failure;return false;
 }
 out=identity;return marker_visible_v28(identity,true,error);
}
bool CharacterMeshFxOwnerV4::marker_anchor_v28(std::uintptr_t id,std::uintptr_t anchor,bool reset,std::string& error){
 error.clear();auto& s=*impl_;s.failure.clear();auto at=s.records.find(id);
 if(at==s.records.end()||at->second->pooled){error="Required same active marker identity";return false;}
 at->second->state.anchor=anchor;
 if(!s.sync(*at->second,reset)){error=s.failure;return false;}return true;
}
bool CharacterMeshFxOwnerV4::marker_visible_v28(std::uintptr_t id,bool visible,std::string& error){
 error.clear();auto& s=*impl_;s.failure.clear();auto at=s.records.find(id);
 if(at==s.records.end()||at->second->pooled){error="Required same active marker identity";return false;}
 auto& r=*at->second;
 if(r.state.visible==std::uint32_t(visible))return true;
 r.state.visible=visible; // Source stores byte24 before virtual visual visibility.
 if(!s.sync(r,false)){error=s.failure;return false;}return true;
}
