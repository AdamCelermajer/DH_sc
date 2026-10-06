#include "canonical_level_context_v1.hpp"
namespace dh2::loader {
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
bool CanonicalLevelContextV1::retain_prepared_source(LevelPreparationV1::Borrow source,
    std::string& error){
    error.clear();if(!source){error="required completed Level source borrow";return false;}
    const auto& actual=source.request();
    if(actual.identity!=request_.identity||actual.definition!=request_.definition||actual.kind!=request_.kind||
       actual.seed!=request_.seed||actual.repair_known_references!=request_.repair_known_references||
       actual.allow_original_backup!=request_.allow_original_backup){
        error="prepared source belongs to a different canonical Level request";return false;
    }
    prepared_=std::move(source);return true;
}
CanonicalLevelContextV1::ConfigFields CanonicalLevelContextV1::config_fields(){
    return {shared_from_this(),&config38_,&music11c_,&safezone120_,&ambient124_};
}
CanonicalLevelContextV1::ModuleLoadFields CanonicalLevelContextV1::module_load_fields(){
    return {shared_from_this(),&object_module_id18c_,module_offset160_.data()};
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
