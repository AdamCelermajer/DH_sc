#include "source_campaign_light_environment_v113.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_conditions_v70.hpp"
#include "source_campaign_class_release_v106.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "source_campaign_object_update_actor_v104.hpp"
#include "source_campaign_noncharacter_owners_v105.hpp"
#include "source_campaign_fx_v77.hpp"
#include "native_resource_budget_v38.hpp"
#include <application_services_owner_v5.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <canonical_character_candidate_v60.hpp>
#include <player_light_tweaker_owner_v90.hpp>
#include <gameobject_scene_root_registry_v1.hpp>
#include <admitted_cpu_bytes_v40.hpp>
#include <source_assertion_process_v76.hpp>
#include <character_design_services.hpp>
#include <scene.hpp>
#include <thread>
#include <map>
namespace model_renderer {namespace {
using namespace dh2;
template<class A,class B>bool same(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);}
struct LightBarrierV113 final:world::LightQuiescenceLeaseV67 {
 std::thread::id thread;std::function<bool()> valid;
 bool validate_current_owning_thread()const noexcept override{try{return thread==std::this_thread::get_id()&&valid&&valid();}catch(...){return false;}}
};
struct LightActorReferenceV113 final:world::LightActorNodeReferenceV67 {
 std::shared_ptr<world::NativeSceneLightNodeV113> node;bool consumed{};
 bool validate_exact_actor_reference(const world::LightQuiescenceLeaseV67& q)const noexcept override{
  return !consumed&&node&&node->live()&&node_identity==node->identity()&&node->light()&&
   light_identity==reinterpret_cast<std::uintptr_t>(node->light().get())&&stamp==q.stamp&&same(runtime_owner,q.runtime_owner)&&q.validate_current_owning_thread();}
 void drop_actual_actor_node_reference()noexcept override{if(consumed||!node)std::terminate();consumed=true;node->drop();}
};
struct LightParameterLeaseV113 {
 std::shared_ptr<world::NativeSceneLightNodeV113> node;
 std::shared_ptr<world::NativeLightV113> light;bool grabbed{};
 ~LightParameterLeaseV113(){if(grabbed)light->drop();}
};
struct LightBaseD1V113 final:world::LightObjectBaseContinuationV67 {
 std::weak_ptr<void> actor;std::function<bool()> current;
 std::shared_ptr<world::NativeConditionRuntimeV69> conditions;
 std::shared_ptr<SourceConditionDependenciesV70> diagnostics;
 unsigned cursor{};
 bool validate_exact_actor(const world::LightQuiescenceLeaseV67& q)const noexcept override{try{return !actor.expired()&&stamp==q.stamp&&same(runtime_owner,q.runtime_owner)&&q.validate_current_owning_thread()&&current&&current();}catch(...){return false;}}
 void destroy_actual_objectbase_fields(world::LightObjectBaseFieldsV67&)noexcept override{std::terminate();} //production uses checked source transport below
 bool destroy_actual_objectbase_fields_checked_v113(world::LightObjectBaseFieldsV67& f,std::string& e)override{
  if(f.actor_identity!=actor_identity||!current||!current()||!f.publication29||!f.owned2c||!f.condition_a8||!f.condition_cc||!f.tested_ac||!f.tested_d0){e="Required SAME LightPoint ObjectBase D1 fields";return false;}
  if(cursor==0){if(*f.publication29){
   auto assertion=world::SourceAssertionProcessV76::borrow();if(!assertion||!assertion->source_level()){e="Actual process ObjectBase29 assertion mode absent";return false;}
   const auto mode=*assertion->source_level();if(mode==2){e="Original ObjectBaseD2 registered29 fatal assertion";return false;}
   if(mode==1&&!assertion->report("..\\..\\project_vs2005\\Game/..\\..\\sources\\Core\\ObjectManager\\ObjectBase.cpp",113,"!IsLocked()",e))return false;
  }++cursor;}
  if(cursor==1){if(*f.owned2c){e="Actual positive LightPoint ObjectBase2c allocation free producer required";return false;}++cursor;}
  const auto clear=[](std::string* s,std::string& e){if(!s){e="Missing SAME constructed ObjectBase CString";return false;}std::string{}.swap(*s);return true;};
  if(cursor==2){if(!clear(f.difficultyd4,e))return false;++cursor;}
  if(cursor==3){if(*f.condition_cc&&(!conditions||!conditions->destroy(*f.condition_cc,e)))return false;*f.condition_cc=0;if(!clear(f.deactivateb4,e))return false;++cursor;}
  if(cursor==4){if(*f.condition_a8&&(!conditions||!conditions->destroy(*f.condition_a8,e)))return false;*f.condition_a8=0;if(!clear(f.activate90,e))return false;++cursor;}
  for(auto field:{f.room68,f.archetype48,f.name30,f.template8}){const unsigned step=field==f.room68?5:field==f.archetype48?6:field==f.name30?7:8;if(cursor==step){if(!clear(field,e))return false;++cursor;}}
  e.clear();return true;
 }
};
}
class SourceCampaignLightEnvironmentV113 final:public std::enable_shared_from_this<SourceCampaignLightEnvironmentV113> {
 struct Node {std::shared_ptr<world::NativeSceneLightNodeV113> value;bool creator_reference{true};};
 struct Imported {std::shared_ptr<resources::AdmittedVectorV40> bytes;resources::BresView image{};scene::Scene graph;scene::AuthoredVisibilityV76 visibility;};
 std::weak_ptr<SourceWorldBorrowV61> world_;std::weak_ptr<void> runtime_owner_;
 std::map<std::uintptr_t,Node> nodes_;std::map<std::uintptr_t,std::weak_ptr<world::CanonicalLightPointV53>> actors_;
 std::vector<std::weak_ptr<loader::CanonicalLightPointRecordV53>> records_;
 std::vector<std::shared_ptr<Imported>> imported_;std::vector<std::pair<std::uintptr_t,std::uintptr_t>> automatic424_;
 std::thread::id thread_{std::this_thread::get_id()};std::uint64_t generation_{reinterpret_cast<std::uintptr_t>(this)};
public:
 explicit SourceCampaignLightEnvironmentV113(std::weak_ptr<SourceWorldBorrowV61> world):world_(std::move(world)){}
 bool scope(SourceCampaignCandidateBorrowV55& c,std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e)const{
  w=world_.lock();if(std::this_thread::get_id()!=thread_||!w||!w->canonical_world||!borrow_source_campaign_candidate_runtime_v61(c,e)||!same(c.actual_world,w->owner)||!same(c.application,w->application)||!c.roots||c.roots!=w->canonical_world->scene_roots_v20){if(e.empty())e="Required SAME light campaign Scene and owning thread";return false;}return true;
 }
 world::LightRuntimeStampV67 stamp(const SourceCampaignCandidateBorrowV55& c)const{
  return {reinterpret_cast<std::uintptr_t>(c.application.get()),reinterpret_cast<std::uintptr_t>(c.level.get()),reinterpret_cast<std::uintptr_t>(c.objects.get()),c.roots->scene_manager_identity_v16(),reinterpret_cast<std::uintptr_t>(c.roots->source_light_runtime_v113().get()),generation_};
 }
 bool delivery(const loader::LightDeliveryV67& b,SourceCampaignCandidateBorrowV55& c,std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e){
  if(!scope(c,w,e)||!same(c.actual_world,b.actual_world)||!same(c.level,b.level)||!same(c.objects,b.objects)||b.properties!=c.properties||!b.runtime_owner||!same(b.runtime_owner,runtime_owner_.lock())){if(e.empty())e="Foreign actual LightPoint native delivery";return false;}return true;
 }
 bool register_owner(const std::shared_ptr<void>& owner,std::string& e){auto previous=runtime_owner_.lock();if(!owner||(previous&&!same(previous,owner))){e="Actual Light environment runtime owner replaced";return false;}runtime_owner_=owner;e.clear();return true;}
 bool native_node(const loader::LightDeliveryV67& b,const world::LightNodeBorrowV53& borrow,std::shared_ptr<world::NativeSceneLightNodeV113>& out,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!delivery(b,c,w,e))return false;
  auto i=nodes_.find(borrow.identity);if(i==nodes_.end()||!i->second.value||!i->second.value->live()||!same(borrow.owner,i->second.value)||!i->second.value->light()||borrow.actual_light_identity!=reinterpret_cast<std::uintptr_t>(i->second.value->light().get())){e="Foreign retained LightPoint node120";return false;}out=i->second.value;return true;
 }
 bool construct(const loader::LightDeliveryV67& b,bool allocate,world::LightNodeBorrowV53& out,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!delivery(b,c,w,e)||!b.receiver||!allocate)return false;
  std::shared_ptr<world::NativeSceneLightNodeV113> node;if(!world::NativeSceneLightNodeV113::construct(node,e))return false;
  nodes_.emplace(node->identity(),Node{node});return lend(b,node,out,e);
 }
 bool lend(const loader::LightDeliveryV67& b,const std::shared_ptr<world::NativeSceneLightNodeV113>& node,world::LightNodeBorrowV53& out,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!delivery(b,c,w,e)||!b.receiver||!node||!node->grab(e))return false;
  auto reference=std::make_unique<LightActorReferenceV113>();reference->runtime_owner=b.runtime_owner;reference->stamp=stamp(c);reference->actor_identity=b.receiver->identity();reference->node_identity=node->identity();reference->light_identity=reinterpret_cast<std::uintptr_t>(node->light().get());reference->node=node;
  out.owner=node;out.identity=node->identity();out.actual_light_identity=reference->light_identity;out.actor_reference=std::move(reference);
  //SAME canonical receiver weak reference, independent of its containing World.
  e.clear();return true;
 }
 bool first(const loader::LightDeliveryV67& b,const std::string& file,const char* xref,bool a,bool z,world::LightNodeBorrowV53& out,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!delivery(b,c,w,e)||!xref||*xref||a||z||!w->read_admitted_v81){if(e.empty())e="Actual LightBase.LoadScene parameters/resource reader required";return false;}
  auto resource=std::make_shared<Imported>();resource->bytes=std::make_shared<resources::AdmittedVectorV40>();imported_.push_back(resource);bool found{};
  if(!w->read_admitted_v81(file,found,resource->bytes->bytes,[resource](std::uint32_t bytes,std::string& e){return resource->bytes->admission.reserve(android_resources::budget_lease_v39(),resources::ResourceScopeV37::actor,bytes,e);},e))return false;
  if(!found){imported_.pop_back();out={};e.clear();return true;}if(!resource->bytes->admission.commit(e))return false;
  if(dh2_bres_open(&resource->image,resource->bytes->bytes.data(),resource->bytes->bytes.size())!=resources::BresError::ok||!scene::load_authored_v76(resource->image,resource->graph,resource->visibility,e)){if(e.empty())e="Actual LightBase scene BRES invalid";return false;}
  if(resource->graph.lights_v113.empty()){out={};e.clear();return true;}
  //The actual RootSceneNode.light188 list is populated in authored traversal
  //order. Its first node is selected; the source discards the temporary root.
  const auto first=resource->graph.lights_v113.front();std::shared_ptr<world::NativeSceneLightNodeV113> node;
  if(first.node_index>=resource->graph.graph.size()||!world::NativeSceneLightNodeV113::construct(node,e))return false;
  nodes_.emplace(node->identity(),Node{node});if(!node->load_light(resource->image,first.light,e)||!node->set_authored_matrix_v113(resource->graph.graph[first.node_index].world,e))return false;
  return lend(b,node,out,e);
 }
 bool attach(const loader::LightDeliveryV67& b,const world::LightNodeBorrowV53& n,std::string& e){
  std::shared_ptr<world::NativeSceneLightNodeV113> node;if(!native_node(b,n,node,e))return false;SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!delivery(b,c,w,e))return false;
  const std::weak_ptr<world::NativeSceneLightNodeV113> weak=node;world::GameObjectSceneRootBorrowV1 root;root.owner=node;root.identity=node->identity();root.flags11c=&node->flags();root.parentec=&node->parent();
  root.notify_visibility=[weak](bool parent,std::string& e){auto n=weak.lock();return n&&n->notify_parent_visibility_v113(parent,e);};
  root.remove_animators=[weak](std::string& e){auto n=weak.lock();if(!n||!n->live()){e="Retired actual static light animator C1";return false;}e.clear();return true;}; //actual constructor-empty animator list
  root.scene_phase_v69=[weak](std::uint32_t,std::string& e){auto n=weak.lock();return n&&n->scene_phase(e);};
  root.scene_manager_changed=[weak](std::uintptr_t value,std::string& e){auto n=weak.lock();return n&&n->set_scene_manager_v113(value,e);};
  root.light_by_name_v113=[weak](const std::string& name,std::shared_ptr<world::NativeLightV113>& out,std::string& e){auto n=weak.lock();if(!n||!n->live()){e="Retired actual Scene light NAME receiver";return false;}out=n->source_name_v113()==name?n->light():nullptr;e.clear();return true;};
  root.modular_receivers_v114=[weak](auto& out,std::string& e){auto n=weak.lock();if(!n||!n->live())return false;out.clear();e.clear();return true;}; //CLight node has no modular descendants
  root.acquire_parent_reference_v110=[weak](std::string& e){auto n=weak.lock();return n&&n->grab(e);};root.release_parent_reference_v110=[weak](std::string& e){auto n=weak.lock();if(!n||!n->live())return false;n->drop();e.clear();return true;};
  if(!c.roots->add_child(std::move(root),e))return false;auto& stored=nodes_.at(n.identity);if(stored.creator_reference){node->drop();stored.creator_reference=false;}e.clear();return true;
 }
 bool parameters(const loader::LightDeliveryV67& b,const world::LightNodeBorrowV53& n,world::LightParameterFieldsV53& out,std::string& e){
  std::shared_ptr<world::NativeSceneLightNodeV113> node;if(!native_node(b,n,node,e))return false;auto lease=std::make_shared<LightParameterLeaseV113>();lease->node=node;lease->light=node->light();auto light=lease->light;if(!light->grab(e))return false;lease->grabbed=true;const std::weak_ptr<LightParameterLeaseV113> weak=lease;
  out={lease,reinterpret_cast<std::uintptr_t>(light.get()),&light->ambient4,&light->diffuse14,&light->specular24,&light->attenuation34,&light->radius40,[weak](auto& e){auto p=weak.lock();if(!p||!p->grabbed||!p->node->live()||p->node->light()!=p->light||p->light->native_destroyed_v113){e="Actual CLight local intrusive reference expired";return false;}e.clear();return true;}};e.clear();return true;
 }
 bool debug(const char* name,bool& value,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!scope(c,w,e)||!w->debug||!w->debug_files||!name)return false;std::uint32_t out{};if(dh2_character_debug_get(&out,w->debug.get(),name,w->debug_files)!=1){e="Actual light Debug.GetSwitch failed";return false;}value=out!=0;e.clear();return true;}
 bool position(target_providers::Handle16& h,bool& found,std::array<float,3>& out,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!scope(c,w,e))return false;const world::CanonicalObjectBorrowV1* object{};if(!c.objects->resolve_handle_v4(h,false,object,{},e))return false;found=object!=nullptr;if(!found){e.clear();return true;}
  std::uintptr_t character{};if(!object->as_character||!object->as_character(object->context,character,e))return false;
  if(character){SourceCampaignCameraActorBorrowV67 actor;if(!borrow_source_campaign_camera_actor_v67(w->owner,object->identity,actor,e)||!actor.position160)return false;out={actor.position160[0],actor.position160[1],actor.position160[2]};}
  else{std::shared_ptr<void> pin;world::CanonicalGameObjectBaseOwnerV1* base{};if(!borrow_source_campaign_object_base_v77(c,object->identity,pin,base,e)||!base)return false;auto actual=base->vector3(0x160);if(!actual){e="Actual attached GameObject160 position absent";return false;}out={actual[0],actual[1],actual[2]};}
  e.clear();return true;
 }
 bool attachment(const loader::LightDeliveryV67& b,world::LightPointAttachmentServicesV113& out,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!delivery(b,c,w,e)||!b.receiver)return false;
  std::shared_ptr<world::NativeSceneLightNodeV113> node;if(!native_node(b,b.receiver->actual_node(),node,e))return false;
  const auto id=b.receiver->identity();const std::weak_ptr<SourceCampaignLightEnvironmentV113> weak=shared_from_this();const std::weak_ptr<world::NativeSceneLightNodeV113> weak_node=node;
  world::LightPointAttachmentServicesV113 s;s.owner=shared_from_this();
  s.local_player_name=[weak](bool& positive,std::string& name,std::string& e){auto t=weak.lock();SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!t||!t->scope(c,w,e)||!w->player_manager)return false;player::PlayerInfoFieldsV1* record{};if(!w->player_manager->get_local_player(0,true,record,e)||!record)return false;positive=record->character660!=0;if(!positive){e.clear();return true;}SourceCampaignCharacterBorrowV62 actor;if(!borrow_source_campaign_character_v62(w->owner,record->character660,actor,e)||!actor.character||!actor.character->actor)return false;name=actor.character->actor->source_name();e.clear();return true;};
  s.assign_tweaker=[weak,id,weak_node](std::int32_t set,std::int32_t index,std::string& e){auto t=weak.lock();auto n=weak_node.lock();SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!t||!n||!t->scope(c,w,e))return false;if(static_cast<std::uint32_t>(index)>4){e.clear();return true;}auto lights=c.roots->source_light_runtime_v113();if(!lights||!lights->set_light(set,index,n->light(),e))return false;auto tweaker=w->application->source_tweaker58_v90(set);auto a=t->actors_[id].lock();if(!tweaker||!a){e="Actual App light tweaker or SAME LightPoint record absent";return false;}const auto attenuation=a->vector(0x134),ambient=a->vector(0x140),diffuse=a->vector(0x14c),specular=a->vector(0x158);if(!attenuation||!ambient||!diffuse||!specular)return false;return tweaker->source_assign_light_v113(index,id,*attenuation,*ambient,*diffuse,*specular,e);};
  s.find_handle=[weak](const std::string& name,std::int32_t room,target_providers::Handle16& h,bool& valid,std::string& e){auto t=weak.lock();SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!t||!t->scope(c,w,e)||!c.objects->by_name(name.c_str(),room,false,nullptr,h,e))return false;const world::CanonicalObjectBorrowV1* object{};if(!c.objects->resolve_handle_v4(h,false,object,{},e))return false;valid=object!=nullptr;return true;};
  s.resolve_position=[weak](auto& h,auto& found,auto& p,auto& e){auto t=weak.lock();return t&&t->position(h,found,p,e);};
  s.add_active=[weak,weak_node](auto h,std::string& e){auto t=weak.lock();auto n=weak_node.lock();SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;return t&&n&&t->scope(c,w,e)&&c.roots->source_light_runtime_v113()->add_active(h,n->borrow(),e);};
  s.debug_player_headlight=[weak](bool& value,std::string& e){auto t=weak.lock();return t&&t->debug("EnablePlayerHeadLight",value,e);};
  s.node_position=[weak_node](const auto& p,std::string& e){auto n=weak_node.lock();return n&&n->set_position(p,e);};out=std::move(s);e.clear();return true;
 }
 bool quiescence(std::shared_ptr<world::LightQuiescenceLeaseV67>& out,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;auto runtime=runtime_owner_.lock();if(!scope(c,w,e)||!runtime||!require_source_campaign_class_delivery_v106(w->owner,e))return false;
  auto q=std::make_shared<LightBarrierV113>();q->runtime_owner=runtime;q->owning_thread_barrier=shared_from_this();q->stamp=stamp(c);q->thread=thread_;const std::weak_ptr<SourceCampaignLightEnvironmentV113> weak=shared_from_this();const auto expected=q->stamp;
  q->valid=[weak,expected](){auto t=weak.lock();SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;std::string e;return t&&t->scope(c,w,e)&&t->stamp(c)==expected&&require_source_campaign_class_delivery_v106(w->owner,e);};out=std::move(q);e.clear();return true;
 }
 bool dtor(const loader::LightDeliveryV67& b,std::uintptr_t id,std::unique_ptr<world::LightObjectBaseContinuationV67>& out,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!delivery(b,c,w,e)||!b.receiver||b.receiver->identity()!=id)return false;auto actual=actors_[id].lock();if(!actual){e="Actual retained LightPoint class receiver absent";return false;}
  auto tail=std::make_unique<LightBaseD1V113>();tail->runtime_owner=b.runtime_owner;tail->stamp=stamp(c);tail->actor_identity=id;tail->actor=actual;tail->conditions=w->conditions_v70;tail->diagnostics=w->condition_dependencies_v70;
  const std::weak_ptr<SourceCampaignLightEnvironmentV113> weak=shared_from_this();tail->current=[weak,id](){auto t=weak.lock();SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;std::string e;return t&&t->scope(c,w,e)&&!t->actors_[id].expired()&&require_source_campaign_class_delivery_v106(w->owner,e);};out=std::move(tail);e.clear();return true;
 }
 bool retain(const loader::LightDeliveryV67&,const std::shared_ptr<loader::CanonicalLightPointRecordV53>& record,std::string& e){if(!record){e="Actual retained light source record required";return false;} //owner is assigned AFTER this genuine before-C1 journal enrollment
  records_.push_back(record);e.clear();return true;
 }
 bool bind_actor(const loader::LightDeliveryV67& b,std::string& e){
  if(!b.receiver){e="Actual LightPoint receiver absent";return false;}
  //The canonical factory's shared receiver is the same object loan published
  //by the actual source manager/journal. Borrow its existing lease, never own
  //a new actor or manufacture a second control block.
  for(const auto& weak:records_)if(auto record=weak.lock())if(record->owner.get()==b.receiver){actors_[b.receiver->identity()]=std::shared_ptr<world::CanonicalLightPointV53>(record,b.receiver);e.clear();return true;}
  e="Actual LightPoint class journal source receiver lease absent";return false;
 }
 bool update(std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!scope(c,w,e))return false;auto lights=c.roots->source_light_runtime_v113();world::NativeLightUpdateServicesV113 s;s.owner=shared_from_this();auto self=shared_from_this();s.debug=[self](auto n,auto& v,auto& e){return self->debug(n,v,e);};s.resolve_position=[self](auto& h,auto& found,auto& p,auto& e){return self->position(h,found,p,e);};return lights&&lights->update(s,e);}
 bool clear(std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!scope(c,w,e)||!require_source_campaign_class_delivery_v106(w->owner,e))return false;
  for(const auto& pair:nodes_)if(pair.second.value&&pair.second.value->live()&&pair.second.value->parent()){e="Actual Scene.clear parent removal must precede LightSet assignment";return false;}
  //A failed constructor's source-local reference is independent of actor120;
  //it can retire only after its class journal actually completed D0.
  for(const auto& weak:records_)if(auto r=weak.lock())if(r->owner&&r->owner->source_teardown_state()!=world::LightTeardownStateV67::ObjectBaseCompleted){e="Actual LightPoint D0 must finish before source SceneManager.clear";return false;}
  auto lights=c.roots->source_light_runtime_v113();if(!lights){e="Actual source LightSet assignment receiver absent";return false;}lights->source_assign_default_v113();
  for(auto& pair:nodes_)if(pair.second.creator_reference&&pair.second.value&&pair.second.value->live()){pair.second.value->drop();pair.second.creator_reference=false;}
  nodes_.clear();actors_.clear();records_.clear();imported_.clear();
  //Original354468 does not clear SceneManager.automatic424: preserve that
  //source domain rather than inventing an automatic-list lifecycle store.
  e.clear();return true;
 }
 bool tweak(std::uintptr_t id,application::TweakLightV90& out,std::string& e){auto actor=actors_[id].lock();if(!actor){e="Actual LightPoint App128 receiver retired";return false;}
  const std::weak_ptr<world::CanonicalLightPointV53> weak=actor;application::TweakLightV90 s;s.owner=actor;s.identity=id;
  const auto setter=[weak](std::uint32_t offset,const std::array<float,3>& value,std::string& e){auto a=weak.lock();if(!a){e="Actual LightBase setter receiver expired";return false;}return a->source_parameter_setter_v113(offset,value,e);};
  s.attenuation=[setter](auto v,auto& e){return setter(0x134,v,e);};s.ambient=[setter](auto v,auto& e){return setter(0x140,v,e);};s.diffuse=[setter](auto v,auto& e){return setter(0x14c,v,e);};s.specular=[setter](auto v,auto& e){return setter(0x158,v,e);};out=std::move(s);e.clear();return true;
 }
 std::shared_ptr<const loader::LightEnvironmentLeavesV67> leaves(const std::shared_ptr<void>& owner,const std::shared_ptr<world::LightSetNameOwnerV3>& names){
  auto self=shared_from_this();auto p=std::make_shared<loader::LightEnvironmentLeavesV67>();p->owner=owner;p->names=names;
  p->register_delivery_owner_v113=[self](auto& owner,auto& e){return self->register_owner(owner,e);};
  p->validate_current=[self](const auto& b,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;return self->delivery(b,c,w,e);};
  p->bind_objectbase_dtor=[self](const auto& b,auto id,auto& out,auto& e){return self->bind_actor(b,e)&&self->dtor(b,id,out,e);};
  p->observe_record_v113=[self](const auto& b,const auto& record,auto& e){return self->retain(b,record,e);};
  p->first_scene_light=[self](const auto& b,const auto& file,auto x,auto a,auto z,auto& out,auto& e){return self->bind_actor(b,e)&&self->first(b,file,x,a,z,out,e);};
  p->construct_light_node=[self](const auto& b,auto a,auto& out,auto& e){return self->bind_actor(b,e)&&self->construct(b,a,out,e);};
  p->attach_root=[self](const auto& b,const auto& n,auto& e){return self->attach(b,n,e);};
  p->add_automatic=[self](const auto& b,auto id,const auto& n,auto& e){std::shared_ptr<world::NativeSceneLightNodeV113> actual;if(!self->native_node(b,n,actual,e)||!b.receiver||b.receiver->identity()!=id)return false;self->automatic424_.emplace_back(id,n.identity);e.clear();return true;};
  p->borrow_light_parameters=[self](const auto& b,const auto& n,auto& out,auto& e){return self->parameters(b,n,out,e);};
  p->debug_switch=[self](const auto&,auto name,auto& value,auto& e){return self->debug(name,value,e);};
  p->set_type=[self](const auto& b,const auto& n,auto type,auto& e){std::shared_ptr<world::NativeSceneLightNodeV113> actual;if(!self->native_node(b,n,actual,e))return false;actual->light()->type58=type;e.clear();return true;}; //source direct field store, not invented recalc
  p->bind_attachment_v113=[self](const auto& b,auto& out,auto& e){return self->attachment(b,out,e);};
  p->refresh_attachment=[self](const auto& b,auto id,const auto&,auto&,auto& e){if(!b.receiver||b.receiver->identity()!=id)return false;world::LightPointAttachmentServicesV113 s;return self->attachment(b,s,e)&&b.receiver->source_refresh_attachment_v113(s,e);};
  //V69 appends the actual class journal registration/retirement leaves before
  //CatalogEnvironment admission. No placeholder successful release callbacks.
  return p;
 }
};
bool prepare_source_campaign_light_environment_v113(const SourceCampaignCandidateBorrowV55& c,std::shared_ptr<const dh2::loader::LightEnvironmentLeavesV67>& out,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61>w;std::shared_ptr<dh2::world::NativeLightSetV113> lights;
 if(!borrow_source_campaign_condition_world_v70(c,w,e)||!prepare_source_campaign_light_set_v113(c,lights,e)||!w->files_owner)return false;
 if(out){e="Actual native LightPoint provider already supplied";return false;}
 if(!w->light_environment_v113)w->light_environment_v113=std::make_shared<SourceCampaignLightEnvironmentV113>(w);
 out=w->light_environment_v113->leaves(w->files_owner,lights->names());e.clear();return true;
}
bool borrow_source_campaign_light_quiescence_v113(const std::shared_ptr<void>& world,std::shared_ptr<dh2::world::LightQuiescenceLeaseV67>& out,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!borrow_source_campaign_candidate_runtime_v61(c,e)||!same(c.actual_world,world)||!borrow_source_campaign_condition_world_v70(c,w,e)||!w->light_environment_v113){if(e.empty())e="Actual native light environment absent";return false;}return w->light_environment_v113->quiescence(out,e);}
bool update_source_campaign_light_set_v113(const std::shared_ptr<void>& world,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!borrow_source_campaign_candidate_runtime_v61(c,e)||!same(c.actual_world,world)||!borrow_source_campaign_condition_world_v70(c,w,e)||!w->light_environment_v113)return false;return w->light_environment_v113->update(e);}
bool clear_source_campaign_light_set_v113(const std::shared_ptr<void>& world,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61>w;if(!borrow_source_campaign_candidate_runtime_v61(c,e)||!same(c.actual_world,world)||!borrow_source_campaign_condition_world_v70(c,w,e)||!w->light_environment_v113)return false;return w->light_environment_v113->clear(e);}
bool borrow_source_campaign_tweak_light_v113(const std::shared_ptr<SourceWorldBorrowV61>& w,std::uintptr_t id,dh2::application::TweakLightV90& out,std::string& e){if(!w||!w->light_environment_v113){e="Actual campaign LightBase tweak provider absent";return false;}return w->light_environment_v113->tweak(id,out,e);}
}
