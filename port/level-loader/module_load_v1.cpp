#include "module_load_v1.hpp"
namespace dh2::loader {
bool prepare_module_load_v1(ObjectEntryV1 entry,std::int32_t id,const std::array<float,3>& position,
    ModuleLoadV1& out,std::string& error) {
    error.clear();
    if(!entry.document||entry.element>=entry.document.elements().size()||
       entry.disposition!=ObjectEntryDispositionV1::registered_requires_services) {
        error="Module load requires a retained registered entry";return false;
    }
    const auto* type=entry.source().attribute("gametype");
    if(!type||(*type!="Module"&&*type!="Block")) {
        error="Module load requires the original Module/Block class route";return false;
    }
    ModuleLoadV1 candidate;candidate.entry=std::move(entry);candidate.runtime_module_id=id;candidate.class_position=position;
    out=std::move(candidate);return true;
}
ModuleLoadStepV1 step_module_load_v1(ModuleLoadV1& state,ModuleLoadServicesV1& services) {
    const auto fail=[&](const char* operation) {
        state.failed=true;state.error=std::string(operation)+(state.error.empty()?": failed":": "+state.error);
        return ModuleLoadStepV1::failed;
    };
    if(state.failed||state.discarded)return ModuleLoadStepV1::failed;
    if(state.completed)return ModuleLoadStepV1::complete;
    if(!state.entry.document)return fail("Module source unavailable");
    for(;;) {
        switch(state.phase) {
        case ModuleLoadPhaseV1::set_id:
            state.context_attempted=true;
            if(!services.set_module_id(state.runtime_module_id,state.error))return fail("SetObjectModuleId");
            state.phase=ModuleLoadPhaseV1::set_offset;break;
        case ModuleLoadPhaseV1::set_offset:
            if(!services.set_module_offset(state.class_position,state.error))return fail("Module translation");
            state.phase=ModuleLoadPhaseV1::choose;break;
        case ModuleLoadPhaseV1::choose:
            if(!services.choose_xmls(state.entry,state.gameplay,state.visual,state.error))return fail("ChooseXmls service");
            if(state.gameplay.find('\0')!=std::string::npos||state.visual.find('\0')!=std::string::npos)
                return fail("Selected XML contains NUL");
            state.phase=ModuleLoadPhaseV1::gameplay;break;
        case ModuleLoadPhaseV1::gameplay:
        case ModuleLoadPhaseV1::visual: {
            const bool gameplay=state.phase==ModuleLoadPhaseV1::gameplay;
            const auto& uri=gameplay?state.gameplay:state.visual;
            if(!uri.empty()) {
                state.file_open=true;
                const auto result=services.load_file(uri,"Module",state.error);
                if(result==ModuleFileStepV1::pending)return ModuleLoadStepV1::pending;
                if(result==ModuleFileStepV1::failed)return fail("Module file service");
                state.file_open=false;
            }
            state.phase=gameplay?ModuleLoadPhaseV1::visual:ModuleLoadPhaseV1::clear_offset;break;
        }
        case ModuleLoadPhaseV1::clear_offset:
            if(!services.set_module_offset({0,0,0},state.error))return fail("Clear module translation");
            state.offset_cleared=true;state.phase=ModuleLoadPhaseV1::clear_id;break;
        case ModuleLoadPhaseV1::clear_id:
            if(!services.set_module_id(-1,state.error))return fail("Clear module ID");
            state.id_cleared=true;state.phase=ModuleLoadPhaseV1::complete;break;
        case ModuleLoadPhaseV1::complete:
            state.completed=true;return ModuleLoadStepV1::complete;
        }
    }
}
bool discard_module_load_v1(ModuleLoadV1& state,ModuleLoadServicesV1& services,std::string& error) {
    error.clear();
    if(state.discarded)return true;
    if(state.file_open) {
        if(!services.discard_file(error))return false;
        state.file_open=false;
    }
    if(state.context_attempted) {
        if(!state.offset_cleared) {
            if(!services.set_module_offset({0,0,0},error))return false;
            state.offset_cleared=true;
        }
        if(!state.id_cleared) {
            if(!services.set_module_id(-1,error))return false;
            state.id_cleared=true;
        }
    }
    state.entry={};state.discarded=true;return true;
}
}
