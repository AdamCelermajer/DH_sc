#include "canonical_level_context_v1.hpp"
#include "game_event_manager_v50.hpp"
namespace dh2::loader {
bool CanonicalLevelContextV1::game_event_fields_v50(GameEventLevelFieldsV50& out,std::string& error){
 LoadingFieldsV26 loading;if(!loading_fields_v26(loading,error))return false;
 if(loading.level_owner.get()!=this||loading.identity!=identity()||loading.state130!=&constructor_fields_v3_.field130){
  error="GameEvent194 requires same completed source C1";return false;
 }
 out={std::move(loading.level_owner),identity(),&constructor_fields_v3_.field130,&constructor_fields_v3_.field194,&game_events194_};
 error.clear();return true;
}
CanonicalLevelContextV1::CanonicalLevelContextV1(LevelSourceRequestV1 request,
    std::shared_ptr<void> services):request_(std::move(request)),services_(std::move(services)){
    kill_.identity=reinterpret_cast<std::uintptr_t>(this);
    kill_.loot_gate150=0; // Exact LevelC1 store at3f3270, not a loot policy.
    kill_.reserved=0; // Required native KillLevel16 padding, not source storage.
}
bool CanonicalLevelContextV1::create(LevelSourceRequestV1 request,
    std::shared_ptr<void> services,std::shared_ptr<CanonicalLevelContextV1>& out,
    std::string& error){
    error.clear();
    if(!services){error="required canonical Level service lease";return false;}
    if(request.identity.empty()||request.definition.empty()||
       request.identity.find('\0')!=std::string::npos||request.definition.find('\0')!=std::string::npos){
        error="required retained selected Level identity and definition";return false;
    }
    if(request.kind!=LevelSourceKindV1::fixed&&request.kind!=LevelSourceKindV1::procedural){
        error="unsupported selected Level source kind";return false;
    }
    out=std::shared_ptr<CanonicalLevelContextV1>(new CanonicalLevelContextV1(std::move(request),std::move(services)));
    return true;
}
CanonicalLevelContextV1::ConfigFields CanonicalLevelContextV1::config_fields(){
    return {shared_from_this(),&config38_,&music11c_,&safezone120_,&ambient124_};
}
CanonicalLevelContextV1::ModuleLoadFields CanonicalLevelContextV1::module_load_fields(){
    return {shared_from_this(),&object_module_id18c_,module_offset160_.data()};
}
LevelConstructorBorrowV3 CanonicalLevelContextV1::constructor_borrow_v3(){
    // External constructor borrows pin the SAME Level through callbacks.
    std::shared_ptr<void> borrowed(shared_from_this(),this);
    return {std::move(borrowed),identity(),&constructor_fields_v3_,&config38_,
        &music11c_,&safezone120_,&ambient124_,&kill_.loot_gate150,
        module_offset160_.data(),&object_module_id18c_};
}
bool CanonicalLevelContextV1::construct_source_v3(LevelConstructorArgumentsV3 arguments,
    LevelConstructorServicesV3 services,std::string& error){
    if(constructor_v3_){error="Same Level constructor continuation already attempted";return false;}
    auto borrow=constructor_borrow_v3();
    // The once-only owner is embedded in this Level. Retaining its Level lease
    // inside it would cycle; synchronous caller/shared_from_this pins the call.
    auto pin=borrow.owner;borrow.owner=services_;
    constructor_v3_=std::make_unique<LevelConstructorV3>(std::move(borrow),std::move(services));
    const bool result=constructor_v3_->construct(arguments);error=constructor_v3_->error();return result;
}
bool CanonicalLevelContextV1::loading_fields_v26(LoadingFieldsV26& out,std::string& error){
    if(!constructor_v3_||constructor_v3_->phase()!=LevelConstructorPhaseV3::complete){
        error="Required completed SAME Level C1 before loading field borrow";return false;
    }
    out={shared_from_this(),identity(),&constructor_fields_v3_.phase30,&constructor_fields_v3_.field130};error.clear();return true;
}
bool borrow_current_canonical_level_v1(const CanonicalGSLevelGlobalSlotV1& slot,
    CanonicalCurrentLevelBorrowV1& out,std::string& error){
    error.clear();if(!slot.globals_owner||!slot.s_level){
        error="required actual GSLevel::s_level global slot and lease";return false;
    }
    CanonicalCurrentLevelBorrowV1 next;next.globals_=slot.globals_owner;
    next.level_=*slot.s_level;out=std::move(next);return true;
}
}
