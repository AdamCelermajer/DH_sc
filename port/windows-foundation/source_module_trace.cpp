#include "source_module_trace.hpp"
#include "source_world_objects.hpp"
#include "../level-world/canonical_level_config_module_v1.hpp"
#include "../level-world/canonical_level_module_bindings_v2.hpp"
#include <cmath>
#include <utility>

namespace dh::foundation {
namespace {
bool sameSource(const SourceModuleOccurrence& a,const SourceModuleOccurrence& b){
    return a.sourceUri==b.sourceUri&&a.element==b.element&&a.fileOccurrence==b.fileOccurrence;
}
bool read(const SourceModuleReceiverBorrow& receiver,std::int32_t& id,
          std::array<float,3>& position,std::string& error){
    if(!receiver.receiver||!receiver.identity||!receiver.read){error="Actual constructed Module receiver/provider is required";return false;}
    if(!receiver.read(id,position,error))return false;
    if(id<0){error="Source module ID outside host's supported nonnegative runtime range";return false;}
    for(float value:position)if(!std::isfinite(value)){error="Actual Module position is nonfinite";return false;}
    return true;
}
}
SourceModuleReceiverBorrow borrow_source_module_receiver(const std::shared_ptr<dh2::world::CanonicalModuleV1>& module){
    if(!module)return {};
    SourceModuleReceiverBorrow borrow;borrow.receiver=module;borrow.identity=module->base().identity();
    borrow.read=[module](std::int32_t& id,std::array<float,3>& position,std::string& error){
        id=module->module_id();
        const auto* actual=module->base().runtime().subobjects.position;
        for(unsigned axis=0;axis<3;++axis)position[axis]=actual[axis];
        error.clear();return true;
    };
    return borrow;
}
SourceModuleReceiverBorrow borrow_source_module_record(const std::shared_ptr<dh2::world::CanonicalModuleRecordV2>& record){
    if(!record||!record->receiver)return {};
    return borrow_source_module_receiver(std::shared_ptr<dh2::world::CanonicalModuleV1>(record,record->receiver.get()));
}
bool SourceModuleConstructionTrace::observe_record(SourceModuleOccurrence occurrence,
    const std::shared_ptr<dh2::world::CanonicalModuleRecordV2>& record,
    const std::shared_ptr<const void>& declaration,std::string& error){
    if(!record||!record->receiver||!record->declaration||!declaration||
       record->declaration.get()!=declaration.get()||record->declaration.owner_before(declaration)||
       declaration.owner_before(record->declaration)){
        error="Module record must retain the SAME canonical source declaration lease";return false;
    }
    return observe(std::move(occurrence),borrow_source_module_record(record),error);
}
bool SourceModuleConstructionTrace::observe(SourceModuleOccurrence occurrence,
    const std::shared_ptr<dh2::world::CanonicalModuleV1>& receiver,std::string& error){
    return observe(std::move(occurrence),borrow_source_module_receiver(receiver),error);
}
bool SourceModuleConstructionTrace::observe(SourceModuleOccurrence occurrence,SourceModuleReceiverBorrow receiver,std::string& error){
    if(occurrence.hostOccurrence.empty()||occurrence.sourceUri.empty()){
        error="Source module trace needs explicit host occurrence and retained source URI";return false;
    }
    std::int32_t id=-1;std::array<float,3> position;
    if(!read(receiver,id,position,error))return false;
    for(auto& entry:entries_){
        const bool sameReceiver=entry.receiver.identity==receiver.identity;
        if(sameReceiver||entry.occurrence.hostOccurrence==occurrence.hostOccurrence||
           sameSource(entry.occurrence,occurrence)||entry.runtimeId==id){
            if(sameReceiver&&entry.runtimeId==id&&entry.occurrence.hostOccurrence==occurrence.hostOccurrence&&
               sameSource(entry.occurrence,occurrence)&&
               !entry.receiver.receiver.owner_before(receiver.receiver)&&!receiver.receiver.owner_before(entry.receiver.receiver)){
                entry.position=position;error.clear();return true;
            }
            error="Conflicting source Module receiver, runtime ID, or occurrence mapping";return false;
        }
    }
    entries_.push_back({std::move(occurrence),std::move(receiver),id,position});error.clear();return true;
}
bool SourceModuleConstructionTrace::refresh(std::string& error){
    std::vector<std::array<float,3>> positions;positions.reserve(entries_.size());
    for(const auto& entry:entries_){
        std::int32_t id=-1;std::array<float,3> position;
        if(!read(entry.receiver,id,position,error))return false;
        if(id!=entry.runtimeId){error="Actual constructed Module runtime ID changed";return false;}
        positions.push_back(position);
    }
    for(std::size_t i=0;i<entries_.size();++i)entries_[i].position=positions[i];error.clear();return true;
}
bool SourceModuleConstructionTrace::bind(SourceWorldObjects& objects,std::string& error) const{
    auto staged=objects;
    for(const auto& entry:entries_){
        std::int32_t id=-1;std::array<float,3> position;
        if(!read(entry.receiver,id,position,error))return false;
        if(id!=entry.runtimeId){error="Actual constructed Module runtime ID changed before host binding";return false;}
        if(!staged.bind_module(entry.occurrence.hostOccurrence,id,error))return false;
    }
    // Serialized loader commit: the validated operations only mutate the room
    // binding map. Keep existing ActorDefinition storage/references intact.
    for(const auto& entry:entries_)
        if(!objects.bind_module(entry.occurrence.hostOccurrence,entry.runtimeId,error))return false;
    error.clear();return true;
}
} // namespace dh::foundation
