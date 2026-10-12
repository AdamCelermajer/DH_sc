#include "procedural_generator_stream_v66.hpp"
#include <exception>
namespace dh2::loader {
namespace {
struct GeneratedStreamV66 {
 std::unique_ptr<level::SavegameStreamV2> stream=std::make_unique<level::SavegameStreamV2>();
 bool retired{},serialized{},generated_outcome{};std::uint32_t completed_seed{};
};
enum class PhaseV66{sources,blocks,connections,lists,rules,layout,modules,serialize,release,complete,failed};
struct RandomGeneratorV66 {
 std::unique_ptr<world::CanonicalLevelConfigV1> config;
 ProceduralSourcesV1 sources;ProceduralBlocksV1 blocks;ProceduralConnectionsV1 connections;
 ProceduralListsV1 lists;ProceduralRulesV1 rules;ProceduralLayoutResultV1 layout;
 ProceduralModulePlanV1 modules;
 std::uint32_t seed{},random_state{};std::string identity,filename,error;
 PhaseV66 phase{PhaseV66::sources};bool cancelled{},retired{};
 void retire(){
  // Retire real config/reader/layout resources before14c pins can drop.
  // Envelope metadata may survive an alias; it owns no live generator body.
  config.reset();modules={};layout={};rules={};lists={};connections={};blocks={};sources={};retired=true;
 }
};
template<class A,class B>bool owner_equal(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){return !a.owner_before(b)&&!b.owner_before(a);}
}
struct ProceduralGeneratorBindingV66::Impl {
 std::weak_ptr<CanonicalLevelContextV1> level;ProceduralGenerationServicesV66 services;
 std::shared_ptr<GeneratedStreamV66> temporary;
 std::shared_ptr<RandomGeneratorV66> current;
 bool busy{},poisoned{},generation_cancelled{};std::string error;
 bool fail(const char* why,std::string& e){if(error.empty())error=e.empty()?why:e;e=error;poisoned=true;if(current){current->error=error;current->phase=PhaseV66::failed;}return false;}
 bool same_temporary(const GeneratedSourceStreamV38& source,std::string& e)const{
  if(!temporary||temporary->retired||!temporary->stream||source.identity!=reinterpret_cast<std::uintptr_t>(temporary->stream.get())||source.actual_owner.get()!=temporary->stream.get()||!owner_equal(source.actual_owner,temporary)){e="Required SAME live generated StreamBuffer owner/cursor";return false;}return true;
 }
 bool same_current(CanonicalLevelContextV1& actual,std::string& e)const{
  const auto native=actual.constructor_fields_v3().field14c;const auto& owning=actual.procedural_generator_owner_slot_v66();
  if(!current||native!=reinterpret_cast<std::uintptr_t>(current.get())||owning.get()!=current.get()||!owner_equal(owning,current)){e="Actual generator14c cleared/replaced during pending source operation";return false;}return true;
 }
 bool release_generator(CanonicalLevelContextV1& actual,std::string& e){
  auto native=actual.constructor_borrow_v3();auto& owning=actual.procedural_generator_owner_slot_v66();
  if(!native.fields->field14c&&!owning){current.reset();return true;}
  if(!owning||owning.get()!=reinterpret_cast<void*>(native.fields->field14c)){e="Required actual generator14c owned receiver for D0";return false;}
  if(current&&(!same_current(actual,e)))return false;
  // Original RandomGenerator D1 owns config/rules/tiles; CustomFree follows.
  // Do not hold a strong Level in that owning slot. Local aliases retire here.
  auto receiver=std::static_pointer_cast<RandomGeneratorV66>(owning);
  if(!receiver->retired)receiver->retire();
  current.reset();owning.reset();native.fields->field14c=0;return true;
 }
 bool construct(GeneratedSourceStreamV38& out,std::string& e){
  if(busy)return fail("Generated temporary constructor reentered",e);
  if(temporary){e="Existing generated temporary must complete D1 first";return false;}
  auto next=std::make_shared<GeneratedStreamV66>();temporary=next;generation_cancelled=false;poisoned=false;error.clear();
  out={std::shared_ptr<void>(next,next->stream.get()),reinterpret_cast<std::uintptr_t>(next->stream.get())};return true;
 }
 LifecycleStepV36 generate(const GeneratedSourceStreamV38& source,std::uint32_t seed,bool& generated,std::string& e){
  if(busy){fail("Original procedural generator reentered",e);return LifecycleStepV36::failed;}
  if(poisoned||generation_cancelled){e=error.empty()?"Cancelled generator cannot resume":error;return LifecycleStepV36::failed;}
  if(!same_temporary(source,e))return LifecycleStepV36::failed;
  auto actual=level.lock();if(!actual){e="Actual generator Level lifetime ended";return LifecycleStepV36::failed;}
  if(temporary->serialized){if(current||seed!=temporary->completed_seed){e="Completed generated occurrence changed seed/retirement";return LifecycleStepV36::failed;}generated=temporary->generated_outcome;return LifecycleStepV36::complete;}
  busy=true;struct Guard{bool& flag;~Guard(){flag=false;}} guard{busy};
  try{
   if(!current){
    // Original GenerateRandomLevel D0s preexisting14c before400/C1/config.
    if(!release_generator(*actual,e))return LifecycleStepV36::failed;
    auto next=std::make_shared<RandomGeneratorV66>();next->seed=seed;next->identity=actual->source_request().identity;next->filename=actual->constructor_fields_v3().name_f8;
    if(next->filename.empty()||next->filename.find('\0')!=std::string::npos){e="Required actual Level.name_f8 rule CString";return LifecycleStepV36::failed;}
    next->config=std::make_unique<world::CanonicalLevelConfigV1>(services.xml.actual_runtime_owner,world::LevelConfigServicesV1{});
    auto config=next->config->canonical(services.xml.actual_runtime_owner);if(!config.class_name20){e="Required SAME temporary LevelConfig class-name field";return LifecycleStepV36::failed;}
    *config.class_name20="LevelConfig";auto properties=next->config->properties();
    if(!services.xml.properties->init_properties(properties,e)||!services.xml.properties->load_defaults(properties,e))return LifecycleStepV36::failed;
    current=std::move(next);actual->procedural_generator_owner_slot_v66()=current;
    actual->constructor_borrow_v3().fields->field14c=reinterpret_cast<std::uintptr_t>(current.get());
   }
   if(!same_current(*actual,e)||current->seed!=seed||current->filename!=actual->constructor_fields_v3().name_f8||current->identity!=actual->source_request().identity){fail("Pending generator changed Level/source/seed identity",e);return LifecycleStepV36::failed;}
   bool ok=true;
   switch(current->phase){
    case PhaseV66::sources:ok=current->sources.prepare_rule_root_v66(services.actual_resources,current->identity,current->filename,
     [&](const auto& document,auto root,auto& error){return load_original_rule_config_v66(document,root,*current->config,services.xml,error);},e);
     if(ok)current->phase=PhaseV66::blocks;break;
    case PhaseV66::blocks:ok=current->blocks.prepare(current->sources.borrow(),e);if(ok)current->phase=PhaseV66::connections;break;
    case PhaseV66::connections:ok=current->connections.prepare(current->blocks.borrow(),e);if(ok)current->phase=PhaseV66::lists;break;
    case PhaseV66::lists:ok=current->lists.prepare(current->connections.borrow(),e);if(ok)current->phase=PhaseV66::rules;break;
    case PhaseV66::rules:ok=current->rules.prepare_for_generation_v68(current->lists.borrow(),services.pool_runtime,e);if(ok)current->phase=PhaseV66::layout;break;
    case PhaseV66::layout:ok=generate_procedural_layout_v68(current->rules.borrow(),seed,current->random_state,current->layout,e);if(ok)current->phase=PhaseV66::modules;break;
    case PhaseV66::modules:
     if(current->layout.generated)ok=prepare_procedural_modules_v66(services.actual_resources,current->layout,current->modules,e);
     else current->modules.layout=current->layout; // Save STILL runs when Generate returned false.
     if(ok)current->phase=PhaseV66::serialize;break;
    case PhaseV66::serialize:{std::vector<std::uint8_t> bytes;
     ok=save_modular_level_to_bytes_v66(current->modules,*current->config,current->filename,services.xml,bytes,e);
     if(poisoned)return LifecycleStepV36::failed;
     if(ok){
      // Source expand resets read/write to0, then virtual Write writes printer
      // Size bytes (no terminal NUL), leaving read0/writeSize for subsequent COPY.
      if(bytes.size()>UINT32_MAX){e="Original printer-size32 domain exceeded";ok=false;}
      else{temporary->stream->source_expand_v66(static_cast<std::uint32_t>(bytes.size()));
       ok=temporary->stream->write({bytes.data(),bytes.size()},e);if(ok)current->phase=PhaseV66::release;}
     }break;
    }
    case PhaseV66::release:{const bool outcome=current->layout.generated;
     if(!release_generator(*actual,e))return LifecycleStepV36::failed;
     temporary->serialized=true;temporary->generated_outcome=outcome;temporary->completed_seed=seed;generated=outcome;return LifecycleStepV36::complete;
    }
    case PhaseV66::complete:return LifecycleStepV36::complete;
    case PhaseV66::failed:e=current->error;return LifecycleStepV36::failed;
   }
   if(poisoned)return LifecycleStepV36::failed;
   if(!ok){fail("Required original procedural generation/emission source body",e);return LifecycleStepV36::failed;}
   if(!same_current(*actual,e)){fail("Generator receiver changed after reached source stage",e);return LifecycleStepV36::failed;}
   return LifecycleStepV36::pending;
  }catch(const std::exception& ex){if(e.empty())e=ex.what();fail("Original generation source operation threw",e);return LifecycleStepV36::failed;}
 }
 bool destroy(GeneratedSourceStreamV38& source,std::string& e){
  if(busy)return fail("Generated temporary D1 during active generator",e);
  if(!temporary){if(!source.actual_owner&&!source.identity)return true;e="Foreign generated temporary D1";return false;}
  if(!same_temporary(source,e))return false;
  if(current){e="Actual generator must quiesce/D0 before temporary D1";return false;}
  temporary->stream.reset();temporary->retired=true;source={};temporary.reset();return true;
 }
 LifecycleStepV36 cancel(const GeneratedSourceStreamV38& source,std::string& e){
  if(busy){fail("Generator cancellation reentered active source stage",e);return LifecycleStepV36::failed;}
  if(!same_temporary(source,e))return LifecycleStepV36::failed;
  generation_cancelled=true;auto actual=level.lock();
  if(!actual){if(current){e="Missing actual Level for retained generator14c teardown";return LifecycleStepV36::failed;}return LifecycleStepV36::complete;}
  if(!release_generator(*actual,e))return LifecycleStepV36::failed;return LifecycleStepV36::complete;
 }
};
ProceduralGeneratorBindingV66::ProceduralGeneratorBindingV66(std::unique_ptr<Impl> p):impl_(std::move(p)){}
ProceduralGeneratorBindingV66::~ProceduralGeneratorBindingV66()=default;
bool ProceduralGeneratorBindingV66::create(std::weak_ptr<CanonicalLevelContextV1> weak,ProceduralGenerationServicesV66 services,std::shared_ptr<ProceduralGeneratorBindingV66>& out,std::string& e){
 auto actual=weak.lock();
 if(!actual||!services.owner||!services.xml.actual_runtime_owner||!services.xml.properties||!services.xml.module_globals){e="Required SAME actual Level/runtime/property-map/module-global generation binding";return false;}
 const auto native=actual->constructor_borrow_v3();if(owner_equal(services.owner,native.owner)||owner_equal(services.xml.actual_runtime_owner,native.owner)||(services.pool_runtime.owner&&owner_equal(services.pool_runtime.owner,native.owner))){e="Level-owned generator must not retain Level-owning engine aliases";return false;}
 auto impl=std::make_unique<Impl>();impl->level=std::move(weak);impl->services=std::move(services);
 out=std::shared_ptr<ProceduralGeneratorBindingV66>(new ProceduralGeneratorBindingV66(std::move(impl)));return true;
}
AssignedCopyLeavesV65 ProceduralGeneratorBindingV66::generated_copy_leaves(){
 auto self=shared_from_this();AssignedCopyLeavesV65 leaves;leaves.owner=self;
 leaves.size=[self](const SourceIStreamBorrowV65& source,std::uint64_t& size,std::string& e){
  if(!self->impl_->same_temporary({source.actual_owner,source.identity},e)||!self->impl_->temporary->serialized){e="Required complete original generated stream before Size";return false;}size=self->impl_->temporary->stream->size();return true;
 };
 leaves.read=[self](const SourceIStreamBorrowV65& source,void* out,std::uint32_t bytes,std::string& e){
  if(!self->impl_->same_temporary({source.actual_owner,source.identity},e)||!self->impl_->temporary->serialized){e="Required complete original generated stream before Read";return false;}
  return self->impl_->temporary->stream->read(out,bytes,e);
 };return leaves;
}
Stage7ServicesV38 ProceduralGeneratorBindingV66::services(){
 auto self=shared_from_this();Stage7ServicesV38 result;
 result.construct_stream=[self](auto& source,auto& e){return self->impl_->construct(source,e);};
 result.generate=[self](const auto& source,auto seed,auto& generated,auto& e){return self->impl_->generate(source,seed,generated,e);};
 result.destroy_stream=[self](auto& source,auto& e){return self->impl_->destroy(source,e);};
 result.cancel_generation=[self](const auto& source,auto& e){return self->impl_->cancel(source,e);};
 result.assign_stream=assigned_stream_stage7_binding_v65(impl_->level,generated_copy_leaves());return result;
}
}
