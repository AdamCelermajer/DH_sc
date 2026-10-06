#include "retained_level_module_graph_v1.hpp"
#include <exception>
namespace dh2::loader {
namespace { struct BusyGuard {bool& value;explicit BusyGuard(bool& v):value(v){value=true;}~BusyGuard(){value=false;}}; }
RetainedLevelModuleGraphV1::RetainedLevelModuleGraphV1(RetainedLevelModuleGraphInputsV1 in):input_(std::move(in)){
    root_=std::make_unique<CanonicalCachedFileV1>(input_.archive,*input_.manager,
        input_.classes,input_.source,CanonicalModuleContextV1{input_.candidate_owner},ObjectEntryRouteV1::level);
}
bool RetainedLevelModuleGraphV1::create(RetainedLevelModuleGraphInputsV1 in,
    std::shared_ptr<RetainedLevelModuleGraphV1>& out,std::string& e){
    if(!in.level||!in.candidate_owner||!in.manager||!in.classes.context||!in.classes.construct||
       !in.graph||!in.floors||!in.rooms||in.rooms->world()!=in.floors){
        e="Required same retained Level/manager/class dispatch/module graph/floor world";return false;
    }
    auto retained=std::shared_ptr<RetainedLevelModuleGraphV1>(new RetainedLevelModuleGraphV1(std::move(in)));
    if(!CanonicalModuleFilesV1::create(retained->input_.archive,*retained->input_.manager,
       retained->input_.classes,retained->input_.source,retained->input_.level,
       retained->input_.candidate_owner,retained->files_,e))return false;
    out=std::move(retained);return true;
}
bool RetainedLevelModuleGraphV1::fail(RetainedModulePreparationStageV1 at,const std::string& reason,std::string& e){
    if(status_.stage!=RetainedModulePreparationStageV1::failed){status_.failed_at=at;status_.error=reason;status_.stage=RetainedModulePreparationStageV1::failed;}
    e=status_.error;return false;
}
bool RetainedLevelModuleGraphV1::enter(RetainedModulePreparationStageV1 at,std::string& e){
    if(!active_){e="Retained preparation journals have been released";return false;}
    if(busy_)return fail(at,"Retained preparation reentered",e);
    if(status_.stage==RetainedModulePreparationStageV1::failed){e=status_.error;return false;}
    status_.stage=at;return true;
}
bool RetainedLevelModuleGraphV1::collect_modules(std::string& e){
    // Collect only actual completed Module/Block factory results. This is a
    // projection of the unfiltered walk, not a filter for source construction.
    for(std::size_t i=status_.declarations;i<root_->attempts().size();++i){
        const auto& a=*root_->attempts()[i];
        auto* type=a.source().entry().source().attribute("gametype");
        auto* attempt=a.factory_attempt();
        if(type&&(*type=="Module"||*type=="Block")&&attempt&&attempt->stage()==world::CanonicalFactoryStageV1::complete){
            auto* object=input_.manager->object(attempt->handle().key);
            if(!object||!object->type_f4||*object->type_f4!=5||!object->lease){e="Required actual registered Module receiver";return false;}
            auto record=std::static_pointer_cast<world::CanonicalModuleRecordV2>(object->lease);
            if(!record->receiver||record->receiver->base().identity()!=object->identity){e="Module graph receiver identity differs";return false;}
            modules_.push_back(std::move(record));init_attempted_.push_back(false);final_attempted_.push_back(false);source_load_attempted_.push_back(false);source_load_complete_.push_back(false);
        }
        status_.declarations=i+1;
    }
    status_.modules=modules_.size();return true;
}
LevelFileWalkStepV1 RetainedLevelModuleGraphV1::load_root_step(){
    return load_root_step_v38(input_.level->source_request().definition);
}
LevelFileWalkStepV1 RetainedLevelModuleGraphV1::load_root_step_v38(const std::string& resolved_actual_filename){
    std::string e;if(!enter(RetainedModulePreparationStageV1::root_file,e))return LevelFileWalkStepV1::failed;
    if(status_.root_complete){
        if(root_->step(resolved_actual_filename,"Level")!=LevelFileWalkStepV1::complete){fail(RetainedModulePreparationStageV1::root_file,root_->error(),e);return LevelFileWalkStepV1::failed;}
        status_.stage=RetainedModulePreparationStageV1::root_complete;return LevelFileWalkStepV1::complete;
    }
    BusyGuard guard(busy_);
    const auto result=root_->step(resolved_actual_filename,"Level");
    if(!collect_modules(e)){fail(RetainedModulePreparationStageV1::root_file,e,e);return LevelFileWalkStepV1::failed;}
    if(result==LevelFileWalkStepV1::failed){fail(RetainedModulePreparationStageV1::root_file,root_->error(),e);return result;}
    if(status_.stage==RetainedModulePreparationStageV1::failed)return LevelFileWalkStepV1::failed;
    if(result==LevelFileWalkStepV1::complete){status_.root_complete=true;status_.stage=RetainedModulePreparationStageV1::root_complete;}
    return result;
}
bool RetainedLevelModuleGraphV1::initialize_next_module(std::string& e){
    const auto at=RetainedModulePreparationStageV1::module_init_post;if(!enter(at,e))return false;
    if(!status_.root_complete)return fail(at,"Required completed original Level source walk",e);
    if(status_.initialized==modules_.size()){status_.stage=RetainedModulePreparationStageV1::modules_initialized;return true;}
    BusyGuard guard(busy_);const auto i=status_.initialized;
    if(init_attempted_[i])return fail(at,"SAME Module InitPost was already attempted",e);
    init_attempted_[i]=true;
    auto* object=input_.manager->object(modules_[i]->receiver->base().shared_handle().key);
    if(!object||!input_.classes.init_post||!input_.classes.init_post(input_.classes.context,*object,e))return fail(at,e.empty()?"Required SAME Module InitPost":e,e);
    if(status_.stage==RetainedModulePreparationStageV1::failed){e=status_.error;return false;}
    ++status_.initialized;if(status_.initialized==modules_.size())status_.stage=RetainedModulePreparationStageV1::modules_initialized;return true;
}
bool RetainedLevelModuleGraphV1::prepare_floors(std::string& e){
    const auto at=RetainedModulePreparationStageV1::floor_post_load;if(!enter(at,e))return false;
    if(!status_.root_complete||status_.initialized!=modules_.size())return fail(at,"Required actual Module InitPost before floor post_load",e);
    if(status_.floors_prepared){status_.stage=RetainedModulePreparationStageV1::floors_prepared;return true;}
    BusyGuard guard(busy_);if(floor_attempted_)return fail(at,"SAME floor post_load was already attempted",e);floor_attempted_=true;
    if(!floors::post_load(*input_.floors,e)||!input_.rooms->publish_collision_bounds(e))return fail(at,e,e);
    if(status_.stage==RetainedModulePreparationStageV1::failed){e=status_.error;return false;}
    status_.floors_prepared=true;status_.stage=RetainedModulePreparationStageV1::floors_prepared;return true;
}
bool RetainedLevelModuleGraphV1::finalize_next_module(std::string& e){
    const auto at=RetainedModulePreparationStageV1::module_init_final;if(!enter(at,e))return false;
    if(!status_.floors_prepared)return fail(at,"Required SAME floor post_load before Module InitFinal",e);
    if(status_.finalized==modules_.size()){status_.geometry_prepared=true;status_.stage=RetainedModulePreparationStageV1::geometry_prepared;return true;}
    BusyGuard guard(busy_);const auto i=status_.finalized;
    if(final_attempted_[i])return fail(at,"SAME Module InitFinal was already attempted",e);
    final_attempted_[i]=true;bool eligible{};
    if(!modules_[i]->receiver->init_final(eligible,e))return fail(at,e,e);
    if(status_.stage==RetainedModulePreparationStageV1::failed){e=status_.error;return false;}
    // eligible is the genuine source outcome, not a fabricated readiness gate.
    ++status_.finalized;if(status_.finalized==modules_.size()){status_.geometry_prepared=true;status_.stage=RetainedModulePreparationStageV1::geometry_prepared;}return true;
}
bool RetainedLevelModuleGraphV1::load_next_module_sources(const world::ModuleXmlServicesV1& random,std::string& e){
    const auto at=RetainedModulePreparationStageV1::module_files;if(!enter(at,e))return false;
    if(!status_.geometry_prepared)return fail(at,"Required retained Module geometry preparation",e);
    if(status_.loaded==modules_.size()){status_.object_sources_complete=true;status_.stage=RetainedModulePreparationStageV1::object_sources_complete;return true;}
    return load_module_source_index_v38(status_.loaded,random,e);
}

bool RetainedLevelModuleGraphV1::load_module_source_index_v38(std::size_t i,const world::ModuleXmlServicesV1& xml,std::string& e){
    const auto at=RetainedModulePreparationStageV1::module_files;
    if(i>=modules_.size())return fail(at,"Source Module identity/index outside actual retained records",e);
    if(source_load_complete_[i]){e.clear();return true;}
    if(source_load_attempted_[i])return fail(at,"Source Module file prefix already attempted",e);
    BusyGuard guard(busy_);source_load_attempted_[i]=true;
    if(!files_->begin_module(static_cast<std::uint32_t>(i),e)||!modules_[i]->receiver->load(xml,files_->load_borrow(),e))return fail(at,e,e);
    if(status_.stage==RetainedModulePreparationStageV1::failed){e=status_.error;return false;}
    source_load_complete_[i]=true;++status_.loaded;
    if(status_.loaded==modules_.size()){status_.object_sources_complete=true;status_.stage=RetainedModulePreparationStageV1::object_sources_complete;}
    e.clear();return true;
}
bool RetainedLevelModuleGraphV1::load_source_module_v38(std::uintptr_t identity,const world::ModuleXmlServicesV1& xml,std::string& e){
    const auto at=RetainedModulePreparationStageV1::module_files;if(!enter(at,e))return false;
    CanonicalLevelContextV1::LoadingFieldsV26 fields;
    if(!status_.root_complete||!input_.level->loading_fields_v26(fields,e)||!fields.state130||*fields.state130!=10||input_.manager->source_init_phase7c_v38()!=1)
        return fail(at,e.empty()?"Require actual completed root, Level state10 and SAME manager phase1":e,e);
    for(std::size_t i=0;i<modules_.size();++i)if(modules_[i]&&modules_[i]->receiver&&modules_[i]->receiver->base().identity()==identity)
        return load_module_source_index_v38(i,xml,e);
    return fail(at,"Actual manager Module has no retained source record; dynamic record producer required",e);
}
bool RetainedLevelModuleGraphV1::source_object_init_post_v38(const world::CanonicalObjectBorrowV1& object,std::string& e){
    const auto at=RetainedModulePreparationStageV1::module_init_post;if(!enter(at,e))return false;
    CanonicalLevelContextV1::LoadingFieldsV26 fields;
    const auto* actual=object.shared_handle?input_.manager->object(object.shared_handle->key):nullptr;
    if(!input_.level->loading_fields_v26(fields,e)||!fields.state130||*fields.state130!=10||input_.manager->source_init_phase7c_v38()!=3||
       !actual||actual->identity!=object.identity||actual->lease!=object.lease||!input_.classes.init_post)
        return fail(at,e.empty()?"Required actual state10/phase3 class receiver and InitPost producer":e,e);
    std::size_t tracked=modules_.size();
    for(std::size_t i=0;i<modules_.size();++i)if(modules_[i]->receiver->base().identity()==object.identity){
        if(init_attempted_[i])return fail(at,"Actual Module InitPost replayed",e);
        tracked=i;init_attempted_[i]=true;break;
    }
    BusyGuard guard(busy_);
    if(!input_.classes.init_post(input_.classes.context,*actual,e))return fail(at,e,e);
    if(tracked<modules_.size())++status_.initialized;
    e.clear();return true;
}

bool RetainedLevelModuleGraphV1::capture_draw_frames(std::vector<ModuleDrawFrameV1>& out,std::string& e)const{
    if(!active_||busy_){e="Retained graph is released or mutating";return false;}
    std::vector<ModuleDrawFrameV1> frames;frames.reserve(modules_.size());
    for(const auto& module:modules_){
        auto visual=input_.graph->visual(*module->receiver->base().pointer(0x2d8));ModuleDrawFrameV1 frame;
        if(!capture_module_draw_frame_v1(visual,module->receiver->module_id(),frame,e))return false;
        frames.push_back(std::move(frame));
    }
    out=std::move(frames);return true;
}
bool RetainedLevelModuleGraphV1::discard_after_owner_release(const std::function<bool(std::string&)>& release,std::string& e){
    if(busy_){e="Retained preparation cleanup reentered";return false;}
    BusyGuard guard(busy_);
    if(active_){if(!release){e="Required actual Level/manager/visual release";return false;}if(!release(e))return false;active_=false;}
    if(!root_->discard_after_owner_release([](std::string&){return true;},e))return false;
    return files_->discard_after_owner_release([](std::string&){return true;},e);
}
}
