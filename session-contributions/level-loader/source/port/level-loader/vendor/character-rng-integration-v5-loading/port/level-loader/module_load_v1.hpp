#pragma once
#include "object_entry_v1.hpp"
namespace dh2::loader {
enum class ModuleFileStepV1 { pending, complete, failed };
enum class ModuleLoadStepV1 { pending, complete, failed };
enum class ModuleLoadPhaseV1 { set_id, set_offset, choose, gameplay, visual, clear_offset, clear_id, complete };
struct ModuleLoadV1 {
    ObjectEntryV1 entry;
    std::int32_t runtime_module_id{}; // Class-owned Module+0x40c, not an authored/save/index ID.
    std::array<float,3> class_position{}; // After class property/default/override services.
    std::string gameplay,visual,error;
    ModuleLoadPhaseV1 phase{ModuleLoadPhaseV1::set_id};
    bool context_attempted{},file_open{},offset_cleared{},id_cleared{},failed{},completed{},discarded{};
};
class ModuleLoadServicesV1 {
public:
    virtual ~ModuleLoadServicesV1()=default;
    // Required bound level context; callbacks returning false report execution
    // failure, never conditions. Internal candidate, not an agreed shared ABI.
    virtual bool set_module_id(std::int32_t,std::string& error)=0;
    virtual bool set_module_offset(const std::array<float,3>&,std::string& error)=0;
    // Real owner chooses from post-property module fields. This kernel does not
    // substitute raw XML attributes for template/default/alternative selection.
    virtual bool choose_xmls(const ObjectEntryV1&,std::string& gameplay,std::string& visual,std::string& error)=0;
    // Same retained file service is polled until completion. It must report
    // parser/service failure separately, not map original terminal true to ready.
    virtual ModuleFileStepV1 load_file(const std::string& uri,const char* root,std::string& error)=0;
    virtual bool discard_file(std::string& error)=0;
};
bool prepare_module_load_v1(ObjectEntryV1,std::int32_t runtime_module_id,
    const std::array<float,3>& class_position,ModuleLoadV1& out,std::string& error);
// Source loads both files synchronously; yielding on a pending file is an
// explicit native scheduling policy. A bound level context cannot be shared
// between interleaved module candidates. Providers enforce that ownership.
ModuleLoadStepV1 step_module_load_v1(ModuleLoadV1&,ModuleLoadServicesV1&);
// Explicit cancellation/failure cleanup: retained pending file first, then
// zero offset and ID -1. Failed cleanup retains source for an explicit retry.
bool discard_module_load_v1(ModuleLoadV1&,ModuleLoadServicesV1&,std::string& error);
}
