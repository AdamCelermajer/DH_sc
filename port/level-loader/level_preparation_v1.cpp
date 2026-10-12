#include "level_preparation_v1.hpp"
#include "source_reference_repairs_v1.hpp"
#include <stdexcept>
namespace dh2::loader {
struct LevelPreparationV1::Snapshot {
    LevelSourceRequestV1 request;
    LevelSourceResolutionV1 resolution;
    FixedMapV1::Borrow map;
    FixedDeclarationsV1::Borrow declarations;
    std::shared_ptr<const ProceduralModulePlanV1> procedural_modules;
    ProceduralLayoutResultV1 attempted_layout;
};
struct LevelPreparationV1::Candidate {
    LevelSourceRequestV1 request;
    LevelSourceResolutionV1 resolution;
    ProceduralSourcesV1 procedural_sources;
    ProceduralBlocksV1 blocks;
    ProceduralConnectionsV1 connections;
    ProceduralListsV1 lists;
    ProceduralRulesV1 rules;
    ProceduralLayoutResultV1 layout;
    ProceduralModulePlanV1 modules;
    ProceduralMapSourcesV1 derived;
    FixedSourcesV1 fixed_sources;
    FixedSourcesV1::Borrow sources;
    FixedMapV1 map;
    FixedDeclarationsV1 declarations;
};
const LevelSourceRequestV1& LevelPreparationV1::Borrow::request()const {
    if(!snapshot_)throw std::logic_error("Prepared level source unavailable");
    return snapshot_->request;
}
const LevelSourceResolutionV1& LevelPreparationV1::Borrow::resolution()const {
    if(!snapshot_)throw std::logic_error("Prepared level source unavailable");
    return snapshot_->resolution;
}
const ProceduralLayoutResultV1& LevelPreparationV1::Borrow::attempted_layout()const {
    if(!snapshot_)throw std::logic_error("Prepared level source unavailable");
    return snapshot_->attempted_layout;
}
const FixedMapV1::Borrow& LevelPreparationV1::Borrow::map()const {
    if(!snapshot_)throw std::logic_error("Prepared level source unavailable");
    return snapshot_->map;
}
const FixedDeclarationsV1::Borrow& LevelPreparationV1::Borrow::declarations()const {
    if(!snapshot_)throw std::logic_error("Prepared level source unavailable");
    return snapshot_->declarations;
}
const std::shared_ptr<const ProceduralModulePlanV1>& LevelPreparationV1::Borrow::procedural_modules()const {
    if(!snapshot_)throw std::logic_error("Prepared level source unavailable");
    return snapshot_->procedural_modules;
}
LevelPreparationV1::LevelPreparationV1(assets::ZipAssetPackV1 archive):archive_(std::move(archive)){}
LevelPreparationV1::~LevelPreparationV1()=default;
bool LevelPreparationV1::begin(LevelSourceRequestV1 request,std::string& error) {
    error.clear();
    if(candidate_) {error="Discard the pending or failed source candidate before replacing it";return false;}
    if(request.identity.empty()||request.definition.empty()||
       request.identity.find('\0')!=std::string::npos||request.definition.find('\0')!=std::string::npos||
       (request.kind!=LevelSourceKindV1::fixed&&request.kind!=LevelSourceKindV1::procedural)) {
        error="Invalid level source request";return false;
    }
    auto next=std::make_unique<Candidate>();next->request=std::move(request);
    next->resolution.definition=next->request.definition;
    const auto first=next->request.kind==LevelSourceKindV1::procedural?
        LevelPreparationStageV1::procedural_sources:LevelPreparationStageV1::sources;
    candidate_=std::move(next);stage_=first;failure_=LevelPreparationFailureV1::none;
    error_.clear();completed_stages_=0;return true;
}
LevelPreparationStepV1 LevelPreparationV1::fail(LevelPreparationFailureV1 kind,const std::string& error) {
    error_=std::string(level_preparation_stage_v1(stage_))+": "+error;
    failure_=kind;stage_=LevelPreparationStageV1::failed;return LevelPreparationStepV1::failed;
}
LevelPreparationStepV1 LevelPreparationV1::step() {
    if(stage_==LevelPreparationStageV1::source_ready)return LevelPreparationStepV1::source_ready;
    if(stage_==LevelPreparationStageV1::failed)return LevelPreparationStepV1::failed;
    if(stage_==LevelPreparationStageV1::cancelled)return LevelPreparationStepV1::cancelled;
    if(!candidate_)return fail(LevelPreparationFailureV1::preparation,"No source candidate was begun");
    auto& c=*candidate_;std::string error;bool ok=false;
    auto next=stage_;
    try {
        switch(stage_) {
        case LevelPreparationStageV1::procedural_sources:
            ok=c.procedural_sources.prepare(archive_,c.request.identity,c.request.definition,error);
            next=LevelPreparationStageV1::blocks;break;
        case LevelPreparationStageV1::blocks:
            ok=c.blocks.prepare(c.procedural_sources.borrow(),error);next=LevelPreparationStageV1::connections;break;
        case LevelPreparationStageV1::connections:
            ok=c.connections.prepare(c.blocks.borrow(),error);next=LevelPreparationStageV1::lists;break;
        case LevelPreparationStageV1::lists:
            ok=c.lists.prepare(c.connections.borrow(),error);next=LevelPreparationStageV1::rules;break;
        case LevelPreparationStageV1::rules:
            ok=c.rules.prepare(c.lists.borrow(),error);next=LevelPreparationStageV1::layout;break;
        case LevelPreparationStageV1::layout:
            ok=generate_procedural_layout_v1(c.rules.borrow(),c.request.seed,c.layout,error);
            if(ok&&!c.layout.generated) {
                c.resolution.original_no_layout=true;
                if(!c.request.allow_original_backup)return fail(LevelPreparationFailureV1::original_no_layout,
                    "Original generator produced no layout for "+c.request.identity+" seed "+std::to_string(c.request.seed));
                c.resolution.definition=original_backup_definition_v1(c.request.definition);
                c.resolution.backup_used=true;next=LevelPreparationStageV1::backup_sources;
            }else next=LevelPreparationStageV1::modules;
            break;
        case LevelPreparationStageV1::modules:
            ok=prepare_procedural_modules_v1(archive_,c.layout,c.modules,error);
            if(ok&&c.request.repair_known_references)ok=repair_procedural_references_v1(archive_,c.modules,error);
            next=LevelPreparationStageV1::sources;break;
        case LevelPreparationStageV1::backup_sources:
            ok=c.fixed_sources.prepare(archive_,c.request.identity,c.resolution.definition,error);
            if(!ok)return fail(LevelPreparationFailureV1::original_backup_unavailable,
                "Original generation produced no layout; backup "+c.resolution.definition+" unavailable: "+error);
            c.sources=c.fixed_sources.borrow();next=LevelPreparationStageV1::map;break;
        case LevelPreparationStageV1::sources:
            if(c.request.kind==LevelSourceKindV1::procedural) {
                ok=prepare_procedural_map_sources_v1(archive_,c.modules,c.derived,error);
                if(ok)c.sources=c.derived.sources;
            }else {
                ok=c.fixed_sources.prepare(archive_,c.request.identity,c.request.definition,error);
                if(ok)c.sources=c.fixed_sources.borrow();
            }
            next=LevelPreparationStageV1::map;break;
        case LevelPreparationStageV1::map:
            ok=c.map.prepare(archive_,c.sources,error);next=LevelPreparationStageV1::declarations;break;
        case LevelPreparationStageV1::declarations:
            ok=c.declarations.prepare(c.map.borrow(),error);next=LevelPreparationStageV1::publish_source;break;
        case LevelPreparationStageV1::publish_source: {
            auto prepared=std::make_shared<Snapshot>();prepared->request=c.request;
            prepared->resolution=c.resolution;prepared->attempted_layout=c.layout;
            prepared->map=c.map.borrow();prepared->declarations=c.declarations.borrow();
            prepared->procedural_modules=c.derived.modules;
            latest_=std::move(prepared);candidate_.reset();++completed_stages_;
            stage_=LevelPreparationStageV1::source_ready;return LevelPreparationStepV1::source_ready;
        }
        default:return fail(LevelPreparationFailureV1::preparation,"Unexpected preparation stage");
        }
        if(!ok)return fail(LevelPreparationFailureV1::preparation,error.empty()?"Preparation returned no diagnostic":error);
        ++completed_stages_;stage_=next;return LevelPreparationStepV1::pending;
    }catch(const std::exception& caught) {return fail(LevelPreparationFailureV1::preparation,caught.what());}
}
void LevelPreparationV1::discard()noexcept {
    candidate_.reset();stage_=LevelPreparationStageV1::cancelled;
}
const char* level_preparation_stage_v1(LevelPreparationStageV1 value) {
    switch(value) {
    case LevelPreparationStageV1::idle:return "idle";
    case LevelPreparationStageV1::procedural_sources:return "procedural_sources";
    case LevelPreparationStageV1::blocks:return "blocks";
    case LevelPreparationStageV1::connections:return "connections";
    case LevelPreparationStageV1::lists:return "lists";
    case LevelPreparationStageV1::rules:return "rules";
    case LevelPreparationStageV1::layout:return "layout";
    case LevelPreparationStageV1::modules:return "modules";
    case LevelPreparationStageV1::backup_sources:return "backup_sources";
    case LevelPreparationStageV1::sources:return "sources";
    case LevelPreparationStageV1::map:return "map";
    case LevelPreparationStageV1::declarations:return "declarations";
    case LevelPreparationStageV1::publish_source:return "publish_source";
    case LevelPreparationStageV1::source_ready:return "source_ready";
    case LevelPreparationStageV1::failed:return "failed";
    case LevelPreparationStageV1::cancelled:return "cancelled";
    }return "invalid";
}
}
