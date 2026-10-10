#include "source_level_construction.hpp"
#include "../level-world/canonical_level_module_bindings_v2.hpp"
#include <utility>
#include <exception>

namespace dh::foundation {
struct SourceLevelConstruction::Impl final : dh2::loader::LevelFileWalkServicesV1 {
    SourceLevelConstructionProviders providers;
    dh2::loader::XmlDocumentV1 document;
    dh2::loader::LevelFileWalkV1 walk;
    std::vector<std::unique_ptr<dh2::loader::CanonicalBoundSourceAttemptV1>> attempts;
    SourceModuleConstructionTrace trace;
    SourceLevelConstructionStatus status;
    std::uint64_t fileOccurrence=0;
    bool busy=false;
    bool fail(const std::string& error){status.step=SourceLevelConstructionStep::failed;status.error=error;return false;}
    bool parse_result(bool success,std::string& error) override {
        if(!providers.file.parse_result){error="Required actual source XML parser-result continuation";return false;}
        return providers.file.parse_result(success,error);
    }
    bool release_load_state(std::string& error) override {
        if(!providers.file.release_load_state){error="Required actual source load-state release continuation";return false;}
        return providers.file.release_load_state(error);
    }
    bool load_element(const dh2::loader::XmlDocumentV1::Borrow& doc,std::uint32_t element,std::string& error) override {
        status.element=element;status.gametype.clear();status.factoryPrefix=dh2::world::CanonicalFactoryStageV1::empty;
        if(const auto* type=doc.elements().at(element).attribute("gametype"))status.gametype=*type;
        if(providers.file.before_element&&!providers.file.before_element(doc,element,error))return false;
        if(status.step==SourceLevelConstructionStep::failed){error=status.error;return false;}
        // Root Level source has no type filter and uses the SAME reset context.
        if(*providers.levelModuleId18c!=-1||providers.levelModuleOffset160[0]!=0||
           providers.levelModuleOffset160[1]!=0||providers.levelModuleOffset160[2]!=0){
            error="Actual root Level module context changed before source construction";return false;
        }
        dh2::loader::CanonicalSourceBindingV1 binding;
        dh2::loader::CanonicalModuleContextV1 context{providers.candidateOwner};
        if(!dh2::loader::prepare_canonical_source_binding_v1(doc,element,
            dh2::loader::ObjectEntryRouteV1::level,{},context,binding,error))return false;
        const auto before=providers.moduleGlobals->next_module_id;
        attempts.push_back(std::make_unique<dh2::loader::CanonicalBoundSourceAttemptV1>(std::move(binding)));
        status.attempts=attempts.size();auto& attempt=*attempts.back();
        const bool executed=attempt.execute(*providers.manager,providers.classes,error);
        const auto* factory=attempt.factory_attempt();
        if(factory)status.factoryPrefix=factory->prefix();
        if(!executed)return false; // Preserve actual failed prefix and counter mutations.
        if(!factory||factory->stage()!=dh2::world::CanonicalFactoryStageV1::complete||
           (status.gametype!="Module"&&status.gametype!="Block"))return true;
        if(providers.moduleGlobals->next_module_id!=before+1u){
            error="Module constructor did not advance SAME actual source counter exactly once";return false;
        }
        if(!providers.moduleRecord){error="Completed source Module requires SAME retained constructor record provider";return false;}
        std::shared_ptr<dh2::world::CanonicalModuleRecordV2> record;
        const auto* object=providers.manager->object(factory->handle().key);
        if(!object){error="Completed source Module absent from SAME actual ObjectManager";return false;}
        if(!providers.moduleRecord(object->identity,record,error))return false;
        if(!record||!record->receiver||record->receiver->base().identity()!=object->identity||
           std::uint32_t(record->receiver->module_id())!=before){
            error="Module record differs from actual completed source factory receiver/counter";return false;
        }
        if(!providers.hostOccurrence){error="Actual Module declaration requires explicit host occurrence association";return false;}
        std::string host;if(!providers.hostOccurrence(doc,element,record,host,error))return false;
        const auto request=attempt.source().request();
        if(!trace.observe_record({host,doc.uri(),element,fileOccurrence},record,request.source_lease,error))return false;
        status.modules=trace.entries().size();return true;
    }
};
SourceLevelConstruction::SourceLevelConstruction():impl_(std::make_unique<Impl>()){}
SourceLevelConstruction::~SourceLevelConstruction()=default;
SourceLevelConstruction::SourceLevelConstruction(SourceLevelConstruction&&) noexcept=default;
SourceLevelConstruction& SourceLevelConstruction::operator=(SourceLevelConstruction&&) noexcept=default;
bool SourceLevelConstruction::begin(SourceLevelConstructionProviders providers,std::string uri,
    std::vector<std::uint8_t> xml,std::uint64_t occurrence,std::string& error){
    if(impl_->status.step!=SourceLevelConstructionStep::idle){error="Source construction owner cannot replay begin";return false;}
    if(!providers.levelOwner||!providers.candidateOwner||!providers.globalsOwner||!providers.classesOwner||
       !providers.levelModuleId18c||!providers.levelModuleOffset160||!providers.moduleGlobals||!providers.manager){
        error="Required actual lease-backed Level/module-global/manager/class construction providers";return false;
    }
    if(*providers.levelModuleId18c!=-1||providers.levelModuleOffset160[0]!=0||providers.levelModuleOffset160[1]!=0||providers.levelModuleOffset160[2]!=0){
        error="Root source construction requires actual reset Level18c/160";return false;
    }
    auto next=std::make_unique<Impl>();next->providers=std::move(providers);next->fileOccurrence=occurrence;
    if(!next->document.capture_level_buffer(uri,std::move(xml),error)||
       !dh2::loader::prepare_level_file_walk_v1(next->document.borrow(),"Level",next->walk,error))return false;
    next->status.uri=std::move(uri);next->status.step=SourceLevelConstructionStep::pending;
    impl_=std::move(next);error.clear();return true;
}
SourceLevelConstructionStep SourceLevelConstruction::tick(){
    if(impl_->status.step!=SourceLevelConstructionStep::pending)return impl_->status.step;
    if(impl_->busy){impl_->fail("Source Level construction reentered");return impl_->status.step;}
    impl_->busy=true;
    dh2::loader::LevelFileWalkStepV1 result;
    try {
        result=dh2::loader::step_level_file_walk_v1(impl_->walk,*impl_);
        if(impl_->providers.file.observe_walk&&!impl_->providers.file.observe_walk(impl_->walk,impl_->status.error))
            impl_->fail(impl_->status.error.empty()?"Actual source walk observer failed":impl_->status.error);
    } catch(const std::exception& ex){impl_->fail(ex.what());result=dh2::loader::LevelFileWalkStepV1::failed;}
    catch(...){impl_->fail("Actual source construction provider threw");result=dh2::loader::LevelFileWalkStepV1::failed;}
    impl_->busy=false;
    if(impl_->status.step==SourceLevelConstructionStep::failed)return impl_->status.step;
    if(result==dh2::loader::LevelFileWalkStepV1::failed)impl_->fail(impl_->walk.error);
    else if(result==dh2::loader::LevelFileWalkStepV1::complete)impl_->status.step=SourceLevelConstructionStep::complete;
    return impl_->status.step;
}
const SourceLevelConstructionStatus& SourceLevelConstruction::status() const noexcept{return impl_->status;}
const SourceModuleConstructionTrace& SourceLevelConstruction::modules() const noexcept{return impl_->trace;}
bool SourceLevelConstruction::bind(SourceWorldObjects& objects,std::string& error) const{
    if(impl_->status.step!=SourceLevelConstructionStep::complete){error="Source root construction traversal must complete before scope binding";return false;}
    return impl_->trace.bind(objects,error);
}
} // namespace dh::foundation
