#pragma once
#include "lifecycle_v36_counter_borrow.hpp"
#include <exception>
namespace dh2::loader {
struct Stage7FieldsV38 {
 LifecycleBorrowV36 level;
 std::uint32_t* party_dc{};std::uint32_t* word_e0{};
 std::uint8_t* procedural_e8{};std::string* name_f8{};
};
template<class Context> bool borrow_stage7_fields_v38(const std::shared_ptr<Context>& level,Stage7FieldsV38& out,std::string& error){
 Stage7FieldsV38 next;if(!borrow_lifecycle_fields_v36(level,next.level,error))return false;
 auto source=level->constructor_borrow_v3();if(!source.fields){error="Required actual Level C1 source fields";return false;}
 next.party_dc=&source.fields->party_dc;next.word_e0=&source.fields->word_e0;
 next.procedural_e8=&source.fields->byte_e8;next.name_f8=&source.fields->name_f8;out=std::move(next);return true;
}
struct Stage7GlobalsV38 {
 std::shared_ptr<void> actual_owner;
 std::uint32_t* source_dc_global{};std::uint32_t* source_e0_global{};
};
struct GeneratedSourceStreamV38 {std::shared_ptr<void> actual_owner;std::uintptr_t identity{};};
struct Stage7ServicesV38 {
 std::function<bool(std::uint8_t&,std::string&)> online_byte5;
 std::function<bool(GeneratedSourceStreamV38&,std::string&)> construct_stream;
 // Actual GenerateRandomLevel output, including faithful serializer/ownership.
 // pending is a native responsiveness adaptation over one retained generator
 // cursor; it must not reroll/restart the original generator on each tick.
 std::function<LifecycleStepV36(const GeneratedSourceStreamV38&,std::uint32_t,bool&,std::string&)> generate;
 std::function<bool(const LifecycleBorrowV36&,const GeneratedSourceStreamV38&,std::string&)> assign_stream;
 std::function<bool(GeneratedSourceStreamV38&,std::string&)> destroy_stream;
 // Exact Level.LoadFile occurrence: named root Level and original root child
 // factory order/Player guard. CanonicalCachedFileV1 supplies fixed files.
 std::function<LifecycleStepV36(const std::string&,const char*,std::string&)> root_file_step;
 std::function<bool(std::string&)> increment_actual_file13c;
 // Native cancellation adaptation for a started generator that has not completed.
 // Stop its retained cursor/work before temporary D1 or world teardown; failure
 // preserves the cursor/owner, and an explicit retry finishes that same prefix.
 std::function<LifecycleStepV36(const GeneratedSourceStreamV38&,std::string&)> cancel_generation;
};
enum class Stage7PhaseV38 {publish_globals,online,stream,generate,assign,backup,destroy,load_counter,root_file,file_index,complete,failed};
class Stage7BodyV38 {
 Stage7FieldsV38 fields_;Stage7GlobalsV38 globals_;Stage7ServicesV38 services_;
 GeneratedSourceStreamV38 stream_;std::uint32_t seed_{};bool generated_{},busy_{};
 // These are local cleanup cursors, not Level/RNG/readiness or save fields.
 bool cancel_requested_{},generation_entered_{},generation_finished_{},generator_quiesced_{};
 bool temporary_destroy_complete_{},temporary_drop_complete_{},cleanup_reentered_{};
 std::shared_ptr<void> temporary_cleanup_pin_;std::uintptr_t temporary_identity_{};
 Stage7PhaseV38 phase_{Stage7PhaseV38::publish_globals};std::string error_;
 LifecycleStepV36 fail(const char* operation){phase_=Stage7PhaseV38::failed;if(error_.empty())error_=std::string("Required actual stage7 provider: ")+operation;return LifecycleStepV36::failed;}
 template<class F,class... A> bool call(const F& fn,A&&... args){return fn&&fn(std::forward<A>(args)...,error_)&&phase_!=Stage7PhaseV38::failed;}
 bool same_temporary_owner()const noexcept{
  return !stream_.actual_owner||(stream_.actual_owner.get()==temporary_cleanup_pin_.get()&&
   !stream_.actual_owner.owner_before(temporary_cleanup_pin_)&&!temporary_cleanup_pin_.owner_before(stream_.actual_owner));
 }
 bool destroy_temporary(std::string& error){
  if(temporary_drop_complete_)return true;
  if(!temporary_cleanup_pin_){
   if(!stream_.actual_owner&&!stream_.identity){temporary_destroy_complete_=true;temporary_drop_complete_=true;return true;}
   if(!stream_.actual_owner){error="Required actual temporary StreamBuffer owner before D1";return false;}
   temporary_cleanup_pin_=stream_.actual_owner;temporary_identity_=stream_.identity;
  }
  if(!same_temporary_owner()||(temporary_identity_&&stream_.identity&&stream_.identity!=temporary_identity_)){
   error="Temporary StreamBuffer receiver/owner replaced during destruction";return false;
  }
  // Retain genuine ownership metadata across a partial provider failure; a
  // cleared native identity stays cleared, so retry must not repeat completed D1.
  if(!stream_.actual_owner)stream_.actual_owner=temporary_cleanup_pin_;
  if(!temporary_destroy_complete_){
   if(!services_.destroy_stream){error="Required actual temporary StreamBuffer D1 provider";return false;}
   const bool complete=services_.destroy_stream(stream_,error);
   if(complete)temporary_destroy_complete_=true;
   if(cleanup_reentered_){
    if(!stream_.actual_owner)stream_.actual_owner=temporary_cleanup_pin_;
    error=error_;return false;
   }
   if(!same_temporary_owner()||(temporary_identity_&&stream_.identity&&stream_.identity!=temporary_identity_)){
    if(error.empty())error="Temporary StreamBuffer receiver/owner changed during D1";return false;
   }
   if(!complete){
    if(!stream_.actual_owner)stream_.actual_owner=temporary_cleanup_pin_;
    return false;
   }
  }
  stream_={};temporary_cleanup_pin_.reset();temporary_identity_=0;temporary_drop_complete_=true;return true;
 }
public:
 Stage7BodyV38(Stage7FieldsV38 fields,Stage7GlobalsV38 globals,Stage7ServicesV38 services):fields_(std::move(fields)),globals_(std::move(globals)),services_(std::move(services)){}
 Stage7BodyV38(const Stage7BodyV38&)=delete;
 Stage7PhaseV38 phase()const noexcept{return phase_;}
 const std::string& error()const noexcept{return error_;}
 // Source has no cancellation branch. This safety continuation quiesces a
 // retained generator, then calls the mandatory real temporary D1 before
 // actual world teardown. It never advances source130/file13c or reruns Generate.
 // Partial failure retains source/services/leases for an explicit cancel retry.
 LifecycleStepV36 prepare_cancel(std::string& error){
  cancel_requested_=true;
  if(busy_){cleanup_reentered_=true;fail("cleanup reentered during delivery");error=error_;return LifecycleStepV36::failed;}
  cleanup_reentered_=false;busy_=true;struct Busy{bool& value;~Busy(){value=false;}} guard{busy_};
  try{
   if(!generator_quiesced_){
    if(generation_entered_&&!generation_finished_){
     if(!services_.cancel_generation){error="Required pending-generator quiescence before temporary D1/unload";return LifecycleStepV36::dependency_missing;}
     const auto result=services_.cancel_generation(stream_,error);
     if(result==LifecycleStepV36::complete)generator_quiesced_=true;
     if(cleanup_reentered_){error=error_;return LifecycleStepV36::failed;}
     if(result!=LifecycleStepV36::complete)return result;
    }
    generator_quiesced_=true;
   }
   // The retained generator is now authentically idle; its callback captures
   // can be dropped before D1, while all remaining providers stay on failure.
   services_.generate={};services_.cancel_generation={};
   if(!destroy_temporary(error))return LifecycleStepV36::failed;
   error.clear();return LifecycleStepV36::complete;
  }catch(const std::exception& ex){if(error.empty())error=ex.what();return LifecycleStepV36::failed;}
  catch(...){if(error.empty())error="Stage7 cleanup provider threw";return LifecycleStepV36::failed;}
 }
 // Only the successful genuine unload composer may dispose remaining provider
 // and global pins. Keeping this body for diagnostics after failure stays safe.
 void finish_cancel()noexcept{
  if(cancel_requested_&&generator_quiesced_&&temporary_drop_complete_){services_={};globals_={};}
 }
 // Original3f72b4..3f72c8: dispatcher writes130=8 BEFORE file13c++.
 // Bind this as LifecycleServices.after_source_increment[7].
 bool after_source_increment(std::string& error){
  if(cancel_requested_){error="Stage7 source increment suppressed after cancellation";return false;}
  if(busy_){fail("post-increment reentered");error=error_;return false;}
  struct Guard{bool& b;Guard(bool& v):b(v){b=true;}~Guard(){b=false;}} guard(busy_);
  if(phase_==Stage7PhaseV38::complete){error.clear();return true;}
  if(phase_!=Stage7PhaseV38::file_index||!fields_.level.fields.state130||*fields_.level.fields.state130!=8){fail("source130=8 before file13c increment");error=error_;return false;}
  if(!call(services_.increment_actual_file13c)){fail("actual source file13c increment");error=error_;return false;}
  phase_=Stage7PhaseV38::complete;error.clear();return true;
 }
 LifecycleStepV36 step(){
  if(cancel_requested_)return fail("source delivery suppressed after cancellation");
  if(phase_==Stage7PhaseV38::failed)return LifecycleStepV36::failed;if(phase_==Stage7PhaseV38::complete||phase_==Stage7PhaseV38::file_index)return LifecycleStepV36::complete;
  if(busy_)return fail("runtime-thread nonreentrancy");struct Guard{bool& b;Guard(bool& v):b(v){b=true;}~Guard(){b=false;}} guard(busy_);
  if(!fields_.level.actual_level_owner||!fields_.level.fields.state130||*fields_.level.fields.state130!=7||!fields_.level.fields.current138||!fields_.party_dc||!fields_.word_e0||!fields_.procedural_e8||!fields_.name_f8)return fail("same actual Level stage7 fields");
  switch(phase_){
  case Stage7PhaseV38::publish_globals:
   if(!globals_.actual_owner||!globals_.source_dc_global||!globals_.source_e0_global)return fail("actual source globals");
   *globals_.source_dc_global=*fields_.party_dc;*globals_.source_e0_global=*fields_.word_e0;
   phase_=*fields_.procedural_e8?Stage7PhaseV38::online:Stage7PhaseV38::load_counter;break;
  case Stage7PhaseV38::online:{
   std::uint8_t online{};if(!call(services_.online_byte5,online))return fail("GetOnline byte5");
   // Source reads selected GLOBAL after online callback, before Stream C1.
   seed_=online?*globals_.source_e0_global:*globals_.source_dc_global;phase_=Stage7PhaseV38::stream;break;}
  case Stage7PhaseV38::stream:
   if(!call(services_.construct_stream,stream_)||!stream_.actual_owner||!stream_.identity)return fail("actual StreamBuffer C1");phase_=Stage7PhaseV38::generate;break;
  case Stage7PhaseV38::generate:{
   if(!services_.generate)return fail("GenerateRandomLevel/faithful output serializer");generation_entered_=true;const auto result=services_.generate(stream_,seed_,generated_,error_);if(result==LifecycleStepV36::complete)generation_finished_=true;
   if(phase_==Stage7PhaseV38::failed)return LifecycleStepV36::failed;if(result==LifecycleStepV36::pending)return result;if(result!=LifecycleStepV36::complete)return fail("GenerateRandomLevel");phase_=generated_?Stage7PhaseV38::assign:Stage7PhaseV38::backup;break;}
  case Stage7PhaseV38::assign:
   if(!call(services_.assign_stream,fields_.level,stream_))return fail("AssignSteamToLoadDataFile actual field140");phase_=Stage7PhaseV38::destroy;break;
  case Stage7PhaseV38::backup:{
   auto& name=*fields_.name_f8;if(name.empty()||name.find('\0')!=std::string::npos)return fail("nonempty source CString backup domain");
   name[0]='x';const auto dot=name.find('.');if(dot!=std::string::npos){name.resize(dot);name+="_BACKUP.mlx";}
   phase_=Stage7PhaseV38::destroy;break;}
  case Stage7PhaseV38::destroy:
   if(!destroy_temporary(error_))return fail("actual temporary StreamBuffer destruction");if(phase_==Stage7PhaseV38::failed)return LifecycleStepV36::failed;phase_=Stage7PhaseV38::load_counter;break;
  case Stage7PhaseV38::load_counter:*fields_.level.fields.current138=500;phase_=Stage7PhaseV38::root_file;break;
  case Stage7PhaseV38::root_file:{
   if(!services_.root_file_step)return fail("Level.LoadFile/root factory transport");const auto result=services_.root_file_step(*fields_.name_f8,"Level",error_);
   if(phase_==Stage7PhaseV38::failed)return LifecycleStepV36::failed;if(result==LifecycleStepV36::pending)return result;if(result!=LifecycleStepV36::complete)return fail("Level.LoadFile");phase_=Stage7PhaseV38::file_index;return LifecycleStepV36::complete;}
  default:return fail("valid original stage7 phase");
  }
  if(phase_==Stage7PhaseV38::failed)return LifecycleStepV36::failed;return LifecycleStepV36::pending;
 }
};
}
