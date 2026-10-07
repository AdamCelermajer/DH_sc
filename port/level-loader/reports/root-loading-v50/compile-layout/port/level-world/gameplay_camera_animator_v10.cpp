#include "gameplay_camera_animator_v10.hpp"
#include <cstring>
#include <algorithm>
namespace dh2::camera {
namespace {
std::uint32_t word(const std::uint8_t* p){return p[0]|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;}
bool ranges(CameraAnimationResourceV10& r,std::string& e){const auto* library=dh2_bres_root_part(&r.bres,resources::RootPart::animation_clip_library);if(!library){e="Required original camera clip library";return false;}const auto count=word(library),offset=word(library+4);if(count>1024||offset>r.bytes.size()||std::uint64_t(count)*12>r.bytes.size()-offset){e="Invalid original camera clip range table";return false;}for(unsigned i=0;i<count;++i){const auto* p=r.bytes.data()+offset+i*12;std::int32_t start,end;auto a=word(p+4),b=word(p+8);std::memcpy(&start,&a,4);std::memcpy(&end,&b,4);r.ranges.push_back({start,end});}return true;}
}
std::shared_ptr<CameraAnimationSetV10> GameplayCameraAnimationManagerV10::find(std::int32_t id)const{auto i=sets_.find(id);return i==sets_.end()?nullptr:i->second;}
bool GameplayCameraAnimationManagerV10::create(std::int32_t& id,std::string& e){
 auto raw=static_cast<std::uint32_t>(current1c_)-1;std::memcpy(&current1c_,&raw,4);const auto candidate=current1c_;
 if(find(candidate)){e="Required original AnimSetManager duplicate Create assertion/reset";return false;}
 auto set=std::make_shared<CameraAnimationSetV10>();sets_.emplace(candidate,set);
 if(!services_.application_lease||!services_.actual_lg_devices){e="Required same Application LG_DEVICES source producer";set->failed=true;set->failure=e;return false;}std::uint8_t lg;if(!services_.actual_lg_devices(lg,e)){set->failed=true;set->failure=e;return false;}set->skip_load3c=lg!=0;id=candidate;return true;
}
bool GameplayCameraAnimationManagerV10::load(CameraAnimationSetV10& set,std::int32_t id,const scene::Scene& scene,std::shared_ptr<CameraAnimationResourceV10>& result,std::string& e){
 if(id<0){result.reset();return true;}if(!services_.dictionary||std::size_t(id)>=services_.dictionary->values.size()){e="Required source camera AnimDict ID";return false;}auto existing=set.by_id.find(id);if(existing!=set.by_id.end()){result=existing->second;return true;}
 const auto& path=services_.dictionary->values[id];auto cached=resources_.find(path);
 if(cached!=resources_.end())result=cached->second;
 else{auto resource=std::make_shared<CameraAnimationResourceV10>();if(!services_.read||!services_.read(path,resource->bytes,e))return false;if(dh2_bres_open(&resource->bres,resource->bytes.data(),resource->bytes.size())!=resources::BresError::ok){e="Invalid actual camera animation BRES";return false;}if(!ranges(*resource,e)||!resource->player.load(resource->bytes.data(),resource->bytes.size(),scene,e,animation::MissingTargets::ignore))return false;resources_.emplace(path,resource);result=std::move(resource);}
 set.by_id.emplace(id,result);return true;
}
bool GameplayCameraAnimationManagerV10::register_camera(std::int32_t id,const data::CameraAnimationSet& row,const scene::Scene& graph,std::string& e){
 auto set=find(id);if(!set||set->failed||set->ready){e="Required fresh source camera AnimationSet prefix";return false;}
 auto fail=[&](){set->failed=true;set->failure=e;return false;};
 auto append=[&](std::int32_t resource,bool is_template){std::shared_ptr<CameraAnimationResourceV10> data;if(!load(*set,resource,graph,data,e))return false;if(data){auto identity=reinterpret_cast<std::uintptr_t>(data.get());if(!set->registration.append(resource,identity,&data->player,e))return false;if(is_template&&!set->registration.set_default(identity,&data->player,e))return false;}return true;};
 if(!append(row.template_id,true))return fail();
 auto animation=[&](std::int32_t resource){if(set->skip_load3c)return true;if(!append(resource,false))return false;if(!services_.debug_after_add){e="Required original AddAnim Debug callback";return false;}return services_.debug_after_add(e);};
 for(auto resource:{row.idle,row.shake,row.crit})if(!animation(resource))return fail();for(auto resource:row.cam_anims)if(!animation(resource))return fail();
 set->registration.refresh_indices();set->ready=true;return true;
}
GameplayCameraAnimatorV10::GameplayCameraAnimatorV10(std::shared_ptr<GameplayCameraSceneV3> scene,std::shared_ptr<CameraAnimationSetV10> set):scene_(std::move(scene)),set_(std::move(set)){timeline.scale=1;timeline.loop=1;timeline.library_present=1;timeline.clip_index=-1;}
bool GameplayCameraAnimatorV10::initialize(std::string& e){
 if(ready_||!scene_||!set_||!set_->ready||set_->failed||!set_->registration.default_player()){e="Required retained complete camera AnimationSet/default library";return false;}
 if(!compiled_.compile_dynamic(set_->registration.compiled_inputs(),scene_->graph(),e,set_->registration.default_player()))return false;
 if(!compiled_.clip_count()){e="Source camera AnimatorSet has no registered library";return false;}values_.resize(compiled_.targets().size());initialized_.resize(values_.size());cursors_.resize(values_.size());
 const auto* clip=compiled_.clip(0);if(dh2_timeline_clip(&timeline,0,clip->start,clip->end)){e="Source initial camera timeline rejected";return false;}ready_=true;return true;
}
bool GameplayCameraAnimatorV10::pose(std::string& e){
 for(unsigned i=0;i<compiled_.targets().size();++i){const auto& t=compiled_.targets()[i];if(t.node==UINT32_MAX)continue;if(t.node>=scene_->graph().graph.size()||(t.type!=1&&t.type!=5&&t.type!=10)){e="Required supported actual camera node applicator";return false;}const auto* binding=compiled_.clip_target(engine_index_,i);if(!initialized_[i]&&(!binding||(binding->mode==1&&!binding->has_default))){e="Required source initial camera applicator value";return false;}if(!compiled_.sample(engine_index_,i,timeline.current_ms,values_[i].data(),t.components,&cursors_[i],e))return false;initialized_[i]=1;auto& node=scene_->graph().graph[t.node];auto* out=t.type==1?node.translation:t.type==5?node.quaternion:node.scale;std::copy_n(values_[i].data(),t.components,out);}
 float root[3];return scene_->root_position(root,e)&&scene_->set_root_position(root,e);
}
bool GameplayCameraAnimatorV10::play(std::int32_t resource,bool loop,bool& played,std::string& e){
 played=false;if(!ready_){e="Required initialized same-scene camera AnimatorSet";return false;}if(resource==-1)return true;const auto mapped=set_->registration.lookup(resource);if(mapped<0)return true;
 const auto previous=engine_index_;engine_index_=mapped;const auto* clip=compiled_.clip(mapped);if(!clip){e="Required source mapped camera animation library";return false;}
 const auto& occurrence=set_->registration.occurrences().at(mapped);const CameraAnimationResourceV10* raw=nullptr;for(auto& entry:set_->by_id)if(&entry.second->player==occurrence.player){raw=entry.second.get();break;}
 if(!raw||clip_indexc<0||std::size_t(clip_indexc)>=raw->ranges.size()){e="Required original camera animation clip indexc";return false;}const auto range=raw->ranges[clip_indexc];if(dh2_timeline_clip(&timeline,clip_indexc,range[0],range[1])){e="Source camera clip selection rejected";return false;}
 if(previous==mapped&&!timeline.loop){const auto sum=std::uint32_t(timeline.start_ms)+std::uint32_t(completion_.extra_ms);std::int32_t ms;std::memcpy(&ms,&sum,4);if(dh2_timeline_jump(&timeline,ms)){e="Source camera replay jump rejected";return false;}}
 if(dh2_timeline_loop(&timeline,loop)||dh2_timeline_scale(&timeline,1)){e="Source camera loop/scale rejected";return false;}
 // Root::NewAnim(false) stores displacement byte1ec=0 then returns.
 // It does not sample pose or deliver callbacks until the real scene phase.
 if(byte10){e="Required original displacement-enabled camera NewAnim";return false;}
 played=true;return true;
}
bool GameplayCameraAnimatorV10::scene_phase(std::uint32_t stamp,std::string& e){
 if(!ready_){e="Required active camera root animator";return false;}root_timestamp_=stamp;std::int32_t time;std::memcpy(&time,&stamp,4);timeline::Services callbacks{&completion_,[](void* p,timeline::State* t){dh2_timeline_notify(static_cast<timeline::Completion*>(p),t);}};
 if(dh2_timeline_update(&timeline,time,&callbacks)){e="Source camera timeline update rejected";return false;}if(!pose(e))return false;
 // CameraLevel Load sets a finite callback and NULL authored trigger callback.
 // CheckCallback delivers only an actual timeline ending, after pose.
 if(completion_.pending&&completion_callback_){completion_callback_();completion_.pending=0;}return true;
}
}
