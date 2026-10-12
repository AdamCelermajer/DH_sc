#include <assigned_root_source_v64.hpp>
#include <cached_level_file_v1.hpp>
#include <canonical_cached_file_v1.hpp>
#include <stdexcept>
#include <iostream>
using namespace dh2::loader;
static void check(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
// Controlled native-seam fixture ONLY: bytes are a literal test document,
// not claimed original generated output. Actual copied-state/slot lifetimes
// and existing XmlDocument/LevelFileWalk implementation are exercised.
struct State {
 std::vector<std::uint8_t> copied;std::shared_ptr<XmlDocumentV1> xml;
 std::uint8_t flag34{1},flag48{};std::uintptr_t document38{},root3c{},node40{},child44{};
 bool retired{};
 explicit State(const std::vector<std::uint8_t>& bytes):copied(bytes){}
};
struct Facet {
 std::shared_ptr<CanonicalLevelContextV1> level;
 std::shared_ptr<void> owning_slot;std::weak_ptr<State> observed_state;
 unsigned reads{},parses{},observes{},elements{},d0{},releases{},release_failures{};
 bool pending_once{true};
};
static std::shared_ptr<Facet> fixture(bool malformed=false){
 auto f=std::make_shared<Facet>();std::string e;LevelSourceRequestV1 q{"SWAMP_02","data/scene/022_swamp2.rule.xml",LevelSourceKindV1::procedural,0};
 check(CanonicalLevelContextV1::create(q,std::make_shared<unsigned>(0),f->level,e),e);
 std::string literal=malformed?"<Level><Dummy></Level>":"<Level><Dummy name='copied_only'/></Level>";
 auto temporary=std::make_shared<std::vector<std::uint8_t>>(literal.begin(),literal.end());std::weak_ptr<std::vector<std::uint8_t>> weak_temporary=temporary;
 auto assigned=std::make_shared<State>(*temporary);f->owning_slot=assigned;f->observed_state=assigned;
 f->level->constructor_borrow_v3().fields->field140=reinterpret_cast<std::uintptr_t>(assigned.get());temporary.reset();
 check(weak_temporary.expired()&&assigned->copied.size()==literal.size(),"assigned state borrowed temporary generator storage");return f;
}
static AssignedRootServicesV64 services(const std::shared_ptr<Facet>& f){
 AssignedRootServicesV64 s;s.owner=f;
 s.borrow=[f](auto& level,auto& out,auto& e){auto state=std::static_pointer_cast<State>(f->owning_slot);if(!state){e="fixture owner missing";return false;}auto fields=level.constructor_borrow_v3();out={f->level,level.identity(),&fields.fields->field140,&f->owning_slot,reinterpret_cast<std::uintptr_t>(state.get()),reinterpret_cast<std::uintptr_t>(&state->copied),state,std::shared_ptr<const void>(state,&state->copied),&state->flag34,&state->document38,&state->root3c,&state->node40,&state->child44,&state->flag48};return true;};
 s.read_document=[f](const auto& b,const auto& name,auto& doc,auto& alias,auto& e){auto state=std::static_pointer_cast<State>(b.assigned_alias);check(!state->retired,"post-D0 Read/native cell access");++f->reads;if(!state->xml){state->xml=std::make_shared<XmlDocumentV1>();state->document38=reinterpret_cast<std::uintptr_t>(state->xml.get());if(!state->xml->capture_level_buffer(name,state->copied,e))return LifecycleStepV36::failed;state->root3c=reinterpret_cast<std::uintptr_t>(state->xml.get());}doc=state->xml->borrow();alias=state->xml;if(f->pending_once){f->pending_once=false;return LifecycleStepV36::pending;}return LifecycleStepV36::complete;};
 s.parse_result=[f](const auto& b,bool,auto&){check(!std::static_pointer_cast<State>(b.assigned_alias)->retired,"post-D0 parse callback");++f->parses;return true;};
 s.observe_walk=[f](const auto& b,const auto& walk,auto&){auto state=std::static_pointer_cast<State>(b.assigned_alias);check(!state->retired,"post-D0 cursor loan");++f->observes;state->child44=0;if(walk.document&&walk.root_cursor<walk.document.top_level_nodes().size()){const auto& root=walk.document.top_level_nodes()[walk.root_cursor];state->node40=walk.root_cursor+1;if(root.kind==1&&root.value=="Level"&&!walk.advance_root){const auto& children=walk.document.elements()[root.element].children;if(walk.child_cursor<children.size())state->child44=children[walk.child_cursor]+1;}}return true;};
 s.before_element=[f](const auto& b,const auto&,auto element,auto&){check(!std::static_pointer_cast<State>(b.assigned_alias)->retired&&*b.child44==element+1,"wrong native child cursor/post-D0 loan");++f->elements;return true;};
 s.release=[f](const AssignedRootReleaseV64& b,auto& e){++f->releases;if(*b.slot140){auto state=std::static_pointer_cast<State>(b.assigned_alias);check(!state->retired,"D0 replay");state->retired=true;++f->d0;*b.slot140=0;}
  // When140 is already0 this branch touches ONLY live slot/lease metadata.
  if(f->release_failures){--f->release_failures;e="fixture partial release failure";return false;}b.owning_slot->reset();return true;};return s;
}
struct WalkServices final:LevelFileWalkServicesV1 {
 CanonicalFileSourceServicesV1 source;std::vector<std::uint32_t> journal;
 bool parse_result(bool ok,std::string& e)override{return source.parse_result(ok,e);}
 bool load_element(const XmlDocumentV1::Borrow& doc,std::uint32_t element,std::string& e)override{if(!source.before_element(doc,element,e))return false;journal.push_back(element);return true;}
 bool release_load_state(std::string& e)override{return source.release_load_state(e);}
};
struct Harness {
 AssignedRootRouteV64 route;CachedLevelFileV1 file{dh2::assets::ZipAssetPackV1{}};std::unique_ptr<WalkServices> walk;unsigned consumes{};
 LifecycleStepV36 poll(const std::shared_ptr<Facet>& f,const AssignedRootServicesV64& s,std::string& e){return route.step(f->level,"022_swamp2.rule.xml",s,[this](auto doc,auto owner,auto source,auto& error){++consumes;if(!walk){walk=std::make_unique<WalkServices>();walk->source=std::move(source);}const auto result=file.step_document(std::move(doc),std::move(owner),"Level",*walk);if(result==LevelFileWalkStepV1::pending&&!walk->source.observe_walk(file.walk(),error))return LevelFileWalkStepV1::failed;if(result==LevelFileWalkStepV1::failed)error=file.error();return result;},e);}
};
// Focused canonical boundary fixtures: all failures occur before preparing a
// binding/journal attempt or invoking the real class constructor callback.
static void reject_callback_before_factory(unsigned trigger){
 auto f=fixture();auto s=services(f);AssignedRootRouteV64 route;std::string e;
 dh2::world::CanonicalObjectManagerV1 manager({});unsigned factories=0;
 dh2::world::CanonicalClassServicesV1 classes;classes.context=&factories;
 classes.construct=[](void* context,const dh2::world::CanonicalFactoryEntryV1&,
  const dh2::world::CanonicalSourceObjectRequestV1&,dh2::world::CanonicalObjectBorrowV1&,std::string& error){
  ++*static_cast<unsigned*>(context);error="unexpected factory after callback failure";return false;
 };
 auto pin=std::make_shared<unsigned>(0);
 CanonicalCachedFileV1 canonical(dh2::assets::ZipAssetPackV1{},manager,classes,{},CanonicalModuleContextV1{pin},ObjectEntryRouteV1::level);
 auto recurse=[&](const AssignedRootBorrowV64& b){
  const auto state=std::static_pointer_cast<State>(b.assigned_alias);
  canonical.step_document(state->xml->borrow(),b.assigned_alias,"Level",{});
 };
 if(trigger==0)s.before_element=[&](const auto& b,const auto&,auto,auto&){recurse(b);return true;};
 if(trigger==1)s.parse_result=[&](const auto& b,bool,auto&){recurse(b);return true;};
 if(trigger==2)s.before_element=[](const auto& b,const auto&,auto,auto&){*b.slot140=0;return true;};
 if(trigger==3)s.before_element=[f](const auto& b,const auto&,auto,auto&){
  auto replacement=std::make_shared<State>(std::static_pointer_cast<State>(b.assigned_alias)->copied);
  *b.slot140=reinterpret_cast<std::uintptr_t>(replacement.get());*b.owning_slot=replacement;return true;
 };
 if(trigger==4)s.parse_result=[](const auto& b,bool,auto&){*b.slot140=0;return true;};
 if(trigger==5)s.observe_walk=[](const auto& b,const auto&,auto&){*b.slot140=0;return true;};
 if(trigger==6){auto original=s.borrow;s.borrow=[original](auto& level,auto& out,auto& error){
  if(!original(level,out,error))return false;
  out.level_alias=std::shared_ptr<const void>(&level,[](const void*){});return true;
 };}
 auto result=LifecycleStepV36::pending;
 for(unsigned poll=0;poll<20&&result==LifecycleStepV36::pending;++poll){
  result=route.step(f->level,"022_swamp2.rule.xml",s,[&](auto doc,auto owner,auto source,auto& error){
   const auto delivered=canonical.step_document(std::move(doc),std::move(owner),"Level",std::move(source));
   if(delivered==LevelFileWalkStepV1::failed)error=canonical.error();return delivered;
  },e);
 }
 check(result==LifecycleStepV36::failed&&factories==0&&canonical.attempts().empty(),"callback failure/mutation prepared or executed a factory");
 if(trigger<2)check(canonical.error().find("reenter")!=std::string::npos,"first recursive diagnostic overwritten");
 if(trigger==6)check(f->reads==0,"forged Level alias reached Read prefix");
}
int main(){
 std::string e;RootSourceModeV64 mode=RootSourceModeV64::none;check(select_root_source_mode_v64(0,mode,e)&&mode==RootSourceModeV64::filename,"initial null branch");check(!select_root_source_mode_v64(1,mode,e)&&mode==RootSourceModeV64::filename,"fixed pending switched before assigned provider rejection");mode=RootSourceModeV64::none;check(select_root_source_mode_v64(1,mode,e)&&select_root_source_mode_v64(0,mode,e)&&mode==RootSourceModeV64::assigned,"assigned mode not sticky after clear");
 auto f=fixture();auto s=services(f);Harness h;check(h.poll(f,s,e)==LifecycleStepV36::pending&&f->reads==1&&h.consumes==0,"pending Read published document/transport prematurely");LifecycleStepV36 result=LifecycleStepV36::pending;for(unsigned i=0;i<20&&result==LifecycleStepV36::pending;++i)result=h.poll(f,s,e);check(result==LifecycleStepV36::complete&&f->reads==2&&f->parses==1&&f->elements==1&&f->d0==1&&!f->owning_slot&&f->level->constructor_fields_v3().field140==0&&h.walk->journal.size()==1,"same common XML/read/release sequence failed");
 auto replacement=fixture();auto rs=services(replacement);Harness rh;check(rh.poll(replacement,rs,e)==LifecycleStepV36::pending,"pending setup");auto old=replacement->observed_state.lock();auto newer=std::make_shared<State>(old->copied);replacement->owning_slot=newer;replacement->level->constructor_borrow_v3().fields->field140=reinterpret_cast<std::uintptr_t>(newer.get());auto reads=replacement->reads;check(rh.poll(replacement,rs,e)==LifecycleStepV36::failed&&replacement->reads==reads&&rh.consumes==0&&!old->retired,"pending replacement reached new Read/provider or dropped old prefix");
 auto cleared=fixture();auto cs=services(cleared);Harness ch;check(ch.poll(cleared,cs,e)==LifecycleStepV36::pending,"clear setup");cleared->level->constructor_borrow_v3().fields->field140=0;check(ch.poll(cleared,cs,e)==LifecycleStepV36::failed&&ch.consumes==0,"premature clear fell back to filename source");
 auto malformed=fixture(true);auto ms=services(malformed);Harness mh;result=LifecycleStepV36::pending;for(unsigned i=0;i<8&&result==LifecycleStepV36::pending;++i)result=mh.poll(malformed,ms,e);check(result==LifecycleStepV36::failed&&malformed->elements==0&&malformed->owning_slot&&mh.file.source(),"malformed generated document instantiated/dropped prefix");check(mh.file.discard(*mh.walk,e)&&malformed->d0==1&&!malformed->owning_slot,"malformed discard did not call original release seam");
 auto cancel=fixture();auto ks=services(cancel);Harness kh;check(kh.poll(cancel,ks,e)==LifecycleStepV36::pending&&kh.poll(cancel,ks,e)==LifecycleStepV36::pending,"cancel setup");cancel->release_failures=2;auto retained=cancel->observed_state;check(!kh.file.discard(*kh.walk,e)&&cancel->d0==1&&cancel->owning_slot&&kh.file.source()&&!retained.expired(),"failed cancel discarded owner/document/source callbacks");check(!kh.file.discard(*kh.walk,e)&&cancel->d0==1&&cancel->owning_slot,"release retry dereferenced native retired cells/replayed D0");check(kh.file.discard(*kh.walk,e)&&cancel->d0==1&&!cancel->owning_slot,"metadata-only release retry did not finish");kh.route=AssignedRootRouteV64{}; // only AFTER authentic release/discard succeeds
 auto detached=fixture();auto ds=services(detached);{Harness transient;check(transient.poll(detached,ds,e)==LifecycleStepV36::pending,"detached setup");}check(detached->owning_slot&&detached->level->constructor_fields_v3().field140&&!detached->observed_state.expired(),"wrapper destruction left raw140 without independent owner");
 auto cycle=fixture();auto state=cycle->observed_state.lock();cycle->owning_slot=std::shared_ptr<void>(cycle->level,state.get());auto ys=services(cycle);Harness yh;check(yh.poll(cycle,ys,e)==LifecycleStepV36::failed&&cycle->reads==0,"Level-owning alias stored back in owning_slot accepted");
 // Canonical recursive cursor callback must keep first failure and stop delivery.
 dh2::world::CanonicalObjectManagerV1 manager({});auto pin=std::make_shared<unsigned>(0);XmlDocumentV1 empty;check(empty.capture_level_buffer("fixture-empty",{'<','L','e','v','e','l','/','>'},e),e);auto document=empty.borrow();CanonicalFileSourceServicesV1 source;source.parse_result=[](bool,auto&){return true;};source.release_load_state=[](auto&){return true;};source.before_element=[](const auto&,auto,auto&){return true;};CanonicalCachedFileV1 canonical(dh2::assets::ZipAssetPackV1{},manager,{}, {},CanonicalModuleContextV1{pin},ObjectEntryRouteV1::level);unsigned recursive=0;source.observe_walk=[&](const auto&,auto&){++recursive;canonical.step_document(document,pin,"Level",source);return true;};check(canonical.step_document(document,pin,"Level",source)==LevelFileWalkStepV1::failed&&recursive==1&&canonical.error().find("reenter")!=std::string::npos,"recursive assigned callback returned pending/overwrote first error");
 for(unsigned trigger=0;trigger<7;++trigger)reject_callback_before_factory(trigger);
 std::cout<<"PASS controlled_assigned_route common_XML_walk=1 copied_temp_lifetime=1 pending_outputs_staged=1 sticky_modes=1 replacement_clear_rejected=1 malformed_no_factory=1 partial_release_retry_metadata_only=1 independent_Level_owner=1 Level_alias_cycle_rejected=1 canonical_recursive_failure=1 original_generator_Read_D0_proof=0\n";
}