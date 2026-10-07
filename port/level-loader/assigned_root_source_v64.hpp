#pragma once
#include <canonical_level_context_v1.hpp>
#include "canonical_cached_file_v1.hpp"
#include "lifecycle_v36.hpp"
#include <optional>
#include <exception>
namespace dh2::loader {
// Main lends ACTUAL assigned receiver cells, not the temporary generator.
// owning_slot belongs to Level; its owner must not alias/retain that Level.
enum class RootSourceModeV64 {none,filename,assigned};
inline bool select_root_source_mode_v64(std::uintptr_t actual140,RootSourceModeV64& mode,std::string& e){
 if(mode==RootSourceModeV64::none)mode=actual140?RootSourceModeV64::assigned:RootSourceModeV64::filename;
 if(mode==RootSourceModeV64::filename&&actual140){e="Pending filename source cannot switch to newly assigned Level140";return false;}
 return true; // assigned remains selected after clear; its route validates lifetime
}
struct AssignedRootBorrowV64 {
 std::shared_ptr<const void> level_alias;
 std::uintptr_t level_identity{};
 std::uintptr_t* slot140{};std::shared_ptr<void>* owning_slot{};
 std::uintptr_t assigned_identity{},stream_identity{};
 std::shared_ptr<void> assigned_alias;
 std::shared_ptr<const void> embedded_stream_alias;
 const std::uint8_t* buffer_flag34{};
 std::uintptr_t* document38{};std::uintptr_t* root3c{};std::uintptr_t* node40{};std::uintptr_t* child44{};
 std::uint8_t* flag48{};
};
// Release lends live Level/ownership metadata only. A failed D0 prefix may
// already clear140; retry must not dereference retired data/parser cells.
struct AssignedRootReleaseV64 {
 std::shared_ptr<const void> level_alias;
 std::uintptr_t level_identity{},assigned_identity{};
 std::uintptr_t* slot140{};std::shared_ptr<void>* owning_slot{};
 std::shared_ptr<void> assigned_alias;
};
inline bool assigned_root_live_v64(const AssignedRootBorrowV64& b,std::string& e){
 if(!b.slot140||!b.owning_slot||*b.slot140!=b.assigned_identity||!*b.owning_slot||b.owning_slot->get()!=b.assigned_alias.get()||b.owning_slot->owner_before(b.assigned_alias)||b.assigned_alias.owner_before(*b.owning_slot)){e="Assigned native receiver cleared/replaced during pending delivery";return false;}return true;
}
struct AssignedRootServicesV64 {
 std::shared_ptr<void> owner;
 std::function<bool(CanonicalLevelContextV1&,AssignedRootBorrowV64&,std::string&)> borrow;
 // Whole Read/TiXML C1/LoadFromBuffer/debug/init-root prefix over SAME owned
 // assigned copy. pending may not replay C1 or fabricate parsed readiness.
 std::function<LifecycleStepV36(const AssignedRootBorrowV64&,const std::string&,
  XmlDocumentV1::Borrow&,std::shared_ptr<const void>& document_alias,std::string&)> read_document;
 // Bind the existing canonical source parse_result semantics, not a new parser.
 std::function<bool(const AssignedRootBorrowV64&,bool,std::string&)> parse_result;
 std::function<bool(const AssignedRootBorrowV64&,const LevelFileWalkV1&,std::string&)> observe_walk;
 std::function<bool(const AssignedRootBorrowV64&,const XmlDocumentV1::Borrow&,std::uint32_t,std::string&)> before_element;
 // Whole actual D0(s), then SAME140 clear and Level-owned lease reset. On retry
 // with140==0 only finish live ownership cleanup; never access retired cells.
 std::function<bool(const AssignedRootReleaseV64&,std::string&)> release;
};
inline bool release_assigned_root_metadata_v64(const AssignedRootBorrowV64& b,const AssignedRootServicesV64& services,std::string& error){
 try{
  if(!*b.slot140&&!*b.owning_slot)return true; // authentic abort already released owner
  if((*b.slot140&&*b.slot140!=b.assigned_identity)||(*b.owning_slot&&(b.owning_slot->get()!=b.assigned_alias.get()||b.owning_slot->owner_before(b.assigned_alias)||b.assigned_alias.owner_before(*b.owning_slot)))){error="Release cannot retire replacement assigned receiver";return false;}
  if(!services.release){error="Required assigned native release provider";return false;}
  AssignedRootReleaseV64 release{b.level_alias,b.level_identity,b.assigned_identity,b.slot140,b.owning_slot,b.assigned_alias};
  if(!services.release(release,error))return false;
  if(*b.slot140||*b.owning_slot){error="Actual assigned release must clear SAME140 and Level-owned lease";return false;}return true;
 }catch(const std::exception& ex){if(error.empty())error=ex.what();return false;}
}
class AssignedRootRouteV64 {
 std::optional<AssignedRootBorrowV64> actual_;
 std::weak_ptr<CanonicalLevelContextV1> level_seen_;
 XmlDocumentV1::Borrow document_;std::shared_ptr<const void> document_alias_;
 std::string source_name_;bool started_{},completed_{},failed_{},busy_{};
 std::shared_ptr<bool> stopped_=std::make_shared<bool>(false);
 static bool checked(CanonicalLevelContextV1& level,const AssignedRootBorrowV64& b,std::string& e){
  auto native=level.constructor_borrow_v3();
  if(!native.owner||!native.fields||b.level_alias.get()!=&level||b.level_alias.owner_before(native.owner)||native.owner.owner_before(b.level_alias)||b.level_identity!=level.identity()||b.slot140!=&native.fields->field140||!b.owning_slot||!*b.owning_slot||!b.assigned_identity||*b.slot140!=b.assigned_identity||b.assigned_alias.get()!=reinterpret_cast<void*>(b.assigned_identity)||b.owning_slot->get()!=b.assigned_alias.get()||b.owning_slot->owner_before(b.assigned_alias)||b.assigned_alias.owner_before(*b.owning_slot)||(!b.assigned_alias.owner_before(native.owner)&&!native.owner.owner_before(b.assigned_alias))||!b.stream_identity||b.embedded_stream_alias.get()!=reinterpret_cast<const void*>(b.stream_identity)||!b.buffer_flag34||!b.document38||!b.root3c||!b.node40||!b.child44||!b.flag48){e="Required SAME Level140/Level-owned assigned copy/cells with no Level ownership cycle";return false;}return true;
 }
public:
 bool started()const noexcept{return started_;}
 // Stage7 may yield after Assign and before first root Read. Capture only the
 // actual owning metadata before external teardown can retire that receiver.
 bool capture_release_prefix_before_unload(const std::shared_ptr<CanonicalLevelContextV1>& level,const AssignedRootServicesV64& services,std::string& e){
  if(busy_){e="Cannot capture assigned cleanup prefix during delivery";return false;}
  if(actual_||!level||!level->constructor_fields_v3().field140)return true;
  if(!services.owner||!services.borrow||!services.release){e="Required assigned cleanup borrow/release providers";return false;}
  busy_=true;struct Busy{bool& value;~Busy(){value=false;}} guard{busy_};
  try{AssignedRootBorrowV64 b;
   if(!services.borrow(*level,b,e)||!checked(*level,b,e))return false;
   actual_=std::move(b);level_seen_=level;failed_=true;*stopped_=true;return true;
  }catch(const std::exception& ex){if(e.empty())e=ex.what();return false;}
 }
 // Called only AFTER authentic candidate/Level unload succeeds. It also covers
 // Read-pending prefixes whose canonical source callbacks were never installed.
 // Failed release retains all prefix aliases for metadata-only explicit retry.
 bool release_after_unload(const std::shared_ptr<CanonicalLevelContextV1>& level,const AssignedRootServicesV64& services,std::string& e){
  if(busy_){e="Cannot release assigned prefix during delivery";return false;}
  failed_=true;*stopped_=true;
  if(!actual_){if(level&&level->constructor_fields_v3().field140){e="Unload left assigned140 without a captured release prefix";return false;}return true;}
  busy_=true;struct Busy{bool& value;~Busy(){value=false;}} guard{busy_};
  if(!release_assigned_root_metadata_v64(*actual_,services,e))return false;
  document_={};document_alias_.reset();actual_.reset();return true;
 }
 template<class Consume>
 LifecycleStepV36 step(const std::shared_ptr<CanonicalLevelContextV1>& level,
  const std::string& name,const AssignedRootServicesV64& services,Consume consume,std::string& e){
  if(busy_){failed_=true;*stopped_=true;e="Assigned root source reentered";return LifecycleStepV36::failed;}
  if(completed_){if(name!=source_name_||level_seen_.lock()!=level){e="Completed assigned occurrence changed Level/raw source identity";return LifecycleStepV36::failed;}return LifecycleStepV36::complete;}
  if(failed_)return LifecycleStepV36::failed;
  busy_=true;struct Busy{bool& value;~Busy(){value=false;}} busy{busy_};
  started_=true;
  auto fail=[&](const char* why,LifecycleStepV36 result=LifecycleStepV36::failed){failed_=true;*stopped_=true;if(e.empty())e=why;return result;};
  try{
   if(!level||!services.owner||!services.borrow||!services.read_document||!services.parse_result||!services.observe_walk||!services.before_element||!services.release)return fail("Required actual assigned Read/parser/cursor/release providers",LifecycleStepV36::dependency_missing);
   if(!actual_){AssignedRootBorrowV64 b;if(!services.borrow(*level,b,e)||*stopped_||!checked(*level,b,e))return fail("Required actual assigned140 borrow");actual_=std::move(b);level_seen_=level;source_name_=name;}
   if(name!=source_name_||!checked(*level,*actual_,e))return fail("Assigned source/receiver changed after occurrence began");
   if(!document_){XmlDocumentV1::Borrow next;std::shared_ptr<const void> next_alias;const auto result=services.read_document(*actual_,name,next,next_alias,e);
    if(*stopped_)return fail("Assigned source reentered during Read prefix");
    if(result==LifecycleStepV36::pending)return result;
    if(result!=LifecycleStepV36::complete)return fail("Required whole assigned document prefix",result);
    if(!checked(*level,*actual_,e))return fail("Assigned receiver changed during Read/parser prefix");
    if(!next||!next.used_level_buffer_route()||!next_alias||!*actual_->document38||next_alias.get()!=reinterpret_cast<const void*>(*actual_->document38))return fail("Required same assigned XML38/level-buffer document owner");
    document_=std::move(next);document_alias_=std::move(next_alias);
   }
   const auto b=*actual_;const auto stopped=stopped_;CanonicalFileSourceServicesV1 source;
   source.parse_result=[b,services,stopped](bool ok,auto& error){if(*stopped)return false;const bool result=assigned_root_live_v64(b,error)&&services.parse_result(b,ok,error);return !*stopped&&result&&assigned_root_live_v64(b,error);};
   source.before_element=[b,services,stopped](const auto& doc,auto element,auto& error){if(*stopped)return false;const bool result=assigned_root_live_v64(b,error)&&services.before_element(b,doc,element,error);return !*stopped&&result&&assigned_root_live_v64(b,error);};
   source.observe_walk=[b,services,stopped](const auto& walk,auto& error){if(*stopped)return false;const bool result=assigned_root_live_v64(b,error)&&services.observe_walk(b,walk,error);return !*stopped&&result&&assigned_root_live_v64(b,error);};
   source.release_load_state=[b,services](auto& error){return release_assigned_root_metadata_v64(b,services,error);};
   const auto result=consume(document_,actual_->assigned_alias,std::move(source),e);
   if(*stopped_)return fail("Assigned source delivery reentered");
   if(result==LevelFileWalkStepV1::failed)return fail("Actual assigned canonical XML/factory transport failed");
   if(result==LevelFileWalkStepV1::pending)return LifecycleStepV36::pending;
   // Only live Level/ownership cells are read after the release callback.
   if(*actual_->slot140||*actual_->owning_slot)return fail("Assigned completion lacks original release prefix");
   completed_=true;document_={};document_alias_.reset();actual_.reset();return LifecycleStepV36::complete;
  }catch(const std::exception& ex){if(e.empty())e=ex.what();return fail("Assigned source provider exception");}
 }
};
// References belong to SourceLoading's stable Impl; callbacks are dropped before
// Impl teardown. Existing external unload is mandatory and runs before release.
template<class LevelGetter>
std::function<LifecycleStepV36(std::string&)> compose_assigned_cancel_v64(AssignedRootRouteV64& route,bool& unload_completed,LevelGetter level,AssignedRootServicesV64 services,std::function<LifecycleStepV36(std::string&)> original){
 if(!original)return {};
 return [&route,&unload_completed,level=std::move(level),services=std::move(services),original=std::move(original)](std::string& e){
  if(!unload_completed){
   if(!route.capture_release_prefix_before_unload(level(),services,e))return LifecycleStepV36::failed;
   const auto result=original(e);if(result!=LifecycleStepV36::complete)return result;unload_completed=true;
  }
  if(!route.release_after_unload(level(),services,e))return LifecycleStepV36::failed;
  return LifecycleStepV36::complete;
 };
}
}