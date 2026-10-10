#include "retained_sequence_playback.hpp"
#include <cmath>
#include <limits>
#include <set>
#include <algorithm>
namespace dh::foundation {
bool RetainedSequencePlayback::prepare(const AssetCatalog& assets,const OriginalCombatVisualPlan& plan,
    const OriginalSequencePolicies& policies,const OriginalAttackSelection& selection,
    RetainedSequenceServices services,const std::string& action,std::string& error){
    return prepare_internal(false,assets,plan,policies,selection,std::move(services),action,error);
}
bool RetainedSequencePlayback::prepare_preserving(const AssetCatalog& assets,const OriginalCombatVisualPlan& plan,
    const OriginalSequencePolicies& policies,const OriginalAttackSelection& selection,
    RetainedSequenceServices services,const std::string& action,std::string& error){
    return prepare_internal(true,assets,plan,policies,selection,std::move(services),action,error);
}
bool RetainedSequencePlayback::prepare_seeded(const AssetCatalog& assets,const OriginalCombatVisualPlan& plan,
    const OriginalCombatPhase& phase,RetainedSequenceServices services,std::string& error){
    error.clear();
    if(active_||seeded_){error="Cancel retained playback before preparing a seeded source state";return false;}
    if(!root_||!services.frame){error="Explicit root and source-frame policies are required";return false;}
    if(phase.clipName.empty()||phase.resolvedPath.empty()){
        error="Seeded source phase requires its exact visual and event asset paths";return false;
    }
    const auto known=std::find_if(plan.sequences.begin(),plan.sequences.end(),[&](const auto& sequence){
        return std::any_of(sequence.phases.begin(),sequence.phases.end(),[&](const auto& source){
            return source.clipName==phase.clipName&&source.resolvedPath==phase.resolvedPath&&
                   source.animationId==phase.animationId;
        });
    });
    if(known==plan.sequences.end()){
        error="Seeded phase is not owned by the supplied source visual plan";return false;
    }
    auto next=std::make_unique<RetainedAnimationOwner>(visual_,root_);
    try{
        const auto bytes=assets.read(phase.resolvedPath);
        if(!next->bind_events(phase.clipName,bytes.data(),bytes.size(),error))return false;
    }catch(const std::exception& failure){error=failure.what();return false;}
    animation_=std::move(next);event_paths_={{phase.clipName,phase.resolvedPath}};
    services_=std::move(services);action_.clear();validation_.reset();phases_.clear();containers_.clear();scope_path_.clear();
    phase_=0;completed_=0;finished_=false;active_=false;seeded_=false;
    error.clear();return true;
}
bool RetainedSequencePlayback::prepare_internal(bool preserve,const AssetCatalog& assets,const OriginalCombatVisualPlan& plan,
    const OriginalSequencePolicies& policies,const OriginalAttackSelection& selection,
    RetainedSequenceServices services,const std::string& action,std::string& error){
    if(active_&&!preserve){error="Cancel active retained sequence before preparing another";return false;}
    if(preserve&&generation_==UINT64_MAX){error="Retained sequence generation exhausted";return false;}
    if(!root_||!services.frame||!services.closed){error="Explicit root, frame and next-state policies are required";return false;}
    auto compiler=std::make_unique<OriginalAttackSequence>();
    OriginalAttackSequenceServices validation;
    validation.visual=combat_visual_binding(visual_);
    validation.restart_clip=[](const std::string&,std::string&){return true;};
    if(!compiler->prepare(plan,policies,selection,validation,action,error))return false;
    std::map<std::vector<std::size_t>,Container> containers;
    const auto* sourceSequence=plan.sequence(selection.state,selection.variant);
    std::map<std::vector<std::size_t>,std::set<std::size_t>> children;
    for(const auto& phase:sourceSequence->phases){
        for(std::size_t depth=0;depth<phase.sourcePath.size();++depth){
            const std::vector<std::size_t> path(phase.sourcePath.begin(),phase.sourcePath.begin()+depth);
            Container source{sourceSequence->id,sourceSequence->type,sourceSequence->loop,0};
            if(depth){
                if(phase.ancestors.size()<depth){error="Source hierarchy ancestor metadata missing";return false;}
                const auto found=policies.find(phase.ancestors[depth-1].animationId);
                if(found==policies.end()){error="Source hierarchy redirected policy missing";return false;}
                source={found->second.id,found->second.type,found->second.loop,0};
            }
            const auto existing=containers.find(path);
            if(existing!=containers.end()&&(existing->second.id!=source.id||existing->second.type!=source.type||existing->second.loops!=source.loops)){
                error="Source hierarchy path has conflicting container metadata";return false;
            }
            containers[path]=source;children[path].insert(phase.sourcePath[depth]);
        }
    }
    for(auto& pair:containers){
        const auto& indexes=children[pair.first];std::size_t expected=0;
        for(const auto index:indexes)if(index!=expected++){error="Source hierarchy contains missing child indices";return false;}
        pair.second.count=static_cast<std::uint32_t>(indexes.size());
    }
    auto next=preserve&&animation_?nullptr:std::make_unique<RetainedAnimationOwner>(visual_,root_);
    auto* receiver=next?next.get():animation_.get();
    auto bound=preserve?event_paths_:std::map<std::string,std::string>{};
    try{
        for(const auto& phase:compiler->phases()){
            if(phase.source.blendOut<INT32_MIN||phase.source.blendOut>INT32_MAX){error="Source BlendOut exceeds native width";return false;}
            if(phase.source.resolvedPath.empty()){error="Explicit source event asset path is required";return false;}
            const auto prior=bound.find(phase.source.clipName);
            if(prior!=bound.end()&&prior->second!=phase.source.resolvedPath){error="Clip alias has inconsistent source event paths";return false;}
            if(bound.emplace(phase.source.clipName,phase.source.resolvedPath).second){const auto bytes=assets.read(phase.source.resolvedPath);if(!receiver->bind_events(phase.source.clipName,bytes.data(),bytes.size(),error))return false;if(preserve&&!next)event_paths_[phase.source.clipName]=phase.source.resolvedPath;}
        }
    }catch(const std::exception& failure){error=failure.what();return false;}
    if(next)animation_=std::move(next);phases_=compiler->phases();validation_=std::move(compiler);event_paths_=std::move(bound);action_=action;services_=std::move(services);
    containers_=std::move(containers);scope_path_=selection.group_path;
    if(preserve)++generation_;phase_=0;completed_=0;finished_=false;active_=false;if(!preserve)seeded_=false;error.clear();return true;
}
bool RetainedSequencePlayback::seed(const AssetCatalog& assets,const std::string& clip,const std::string& path,
    bool loop,float rate,std::int32_t blend,bool move,RetainedAnimationFrame& output,std::string& error){
    if(!animation_){error="Seed requires a prepared retained sequence";return false;}
    if(generation_==UINT64_MAX){error="Retained sequence generation exhausted";return false;}
    const auto existing=event_paths_.find(clip);
    if(existing!=event_paths_.end()&&existing->second!=path){error="Seed source path conflicts with bound clip";return false;}
    if(existing==event_paths_.end()){
        try{const auto bytes=assets.read(path);if(!animation_->bind_events(clip,bytes.data(),bytes.size(),error))return false;event_paths_.emplace(clip,path);}
        catch(const std::exception& failure){error=failure.what();return false;}
    }
    animation_->take_completion();
    if(!animation_->select(clip,loop,rate,blend,move,output,error))return false;
    ++generation_;if(active_)finished_=false;active_=false;seeded_=true;seed_repeats_=0;
    seed_sequence_=seed_completion_delivered_=false;
    seed_clip_=clip;seed_rate_=rate;seed_blend_=blend;seed_move_=move;error.clear();return true;
}
bool RetainedSequencePlayback::seed_sequence(const AssetCatalog& assets,const std::string& clip,const std::string& path,
    float rate,std::int32_t blend,bool move,std::int32_t repeats,RetainedAnimationFrame& output,std::string& error){
    if(repeats< -1){error="Source seed repeat policy must be -1 or nonnegative";return false;}
    if(!seed(assets,clip,path,false,rate,blend,move,output,error))return false;
    seed_repeats_=repeats;seed_sequence_=true;return true;
}
bool RetainedSequencePlayback::advance_seeded(double seconds,std::string& error){
    if(!animation_||active_||!seeded_){error="Inactive explicit seeded playback is required";return false;}
    const auto generation=generation_;RetainedAnimationFrame frame;
    if(!animation_->advance(seconds,frame,error))return false;
    if(!deliver(SIZE_MAX,frame,error))return false;
    if(generation_!=generation||active_){error.clear();return true;}
    const auto completion=animation_->take_completion();
    if(completion.pending&&seed_repeats_!=0){
        if(seed_repeats_>0)--seed_repeats_;
        RetainedAnimationFrame selection;
        if(!animation_->select(seed_clip_,false,seed_rate_,seed_blend_,seed_move_,selection,error,completion.extra_ms))return false;
        if(!deliver(SIZE_MAX,selection,error))return false;
    }else if(completion.pending&&seed_sequence_&&!seed_completion_delivered_){
        // Close only the explicit source sequence, after its last genuine
        // completion. Retire before invoking borrowed consumers: they may
        // fail, reenter or select a new pose, but cannot replay this old end.
        seed_completion_delivered_=true;
        const auto callback=services_.seeded_closed;
        if(callback)return callback(completion,error);
    }
    error.clear();return true;
}
CombatVisualBinding RetainedSequencePlayback::binding(){return {
    [this](const std::string& name,bool loop,std::string& error){if(name!=action_||loop){error="Live retained sequence requires its explicit finite action alias";return false;}return begin(error);},
    [this](const std::string& name,std::int32_t& start,std::int32_t& end,std::string& error){if(!validation_){error="Retained sequence validation is not prepared";return false;}return validation_->binding().range(name,start,end,error);},
    [this](const std::string& name,std::string& error)->const AnimationMarkers*{if(!validation_){error="Retained sequence validation is not prepared";return nullptr;}return validation_->binding().markers(name,error);},
    [this](double seconds,std::string& error){return advance(seconds,error);}
};}
bool RetainedSequencePlayback::deliver(std::size_t phase,const RetainedAnimationFrame& frame,std::string& error){
    const auto callback=services_.frame;return callback(phase,frame,error);
}
bool RetainedSequencePlayback::begin_hierarchy(std::size_t phase,std::size_t firstDepth,std::string& error){
    if(!services_.boundary)return true;
    const auto generation=generation_;const auto leafPath=phases_.at(phase).source.sourcePath;
    for(std::size_t depth=firstDepth;depth<leafPath.size();++depth){
        const std::vector<std::size_t> containerPath(leafPath.begin(),leafPath.begin()+depth);
        const auto source=containers_.at(containerPath);
        RetainedSequenceBoundary boundary{true,phase,static_cast<std::uint32_t>(depth),static_cast<std::uint32_t>(leafPath[depth]),
            source.count,source.id,source.type,source.loops,containerPath,leafPath};
        RetainedSequenceCursorDecision decision;const auto callback=services_.boundary;
        if(!callback(boundary,decision,error))return false;
        if(generation_!=generation||!active_)return true;
        if(decision.setStep||decision.skipNext||decision.stop){error="Source cursor redirects must occur at End or via explicit owner reentry";return false;}
    }
    return true;
}
bool RetainedSequencePlayback::complete_hierarchy(std::optional<std::size_t>& next,std::size_t& beginDepth,std::string& error){
    next.reset();beginDepth=0;
    const auto generation=generation_;const auto leafPath=phases_.at(phase_).source.sourcePath;
    if(leafPath.empty()){error="Source completed leaf has no hierarchy path";return false;}
    for(std::size_t depth=leafPath.size();depth-->0;){
        const std::vector<std::size_t> containerPath(leafPath.begin(),leafPath.begin()+depth);
        const auto source=containers_.at(containerPath);
        RetainedSequenceBoundary boundary{false,phase_,static_cast<std::uint32_t>(depth),static_cast<std::uint32_t>(leafPath[depth]),
            source.count,source.id,source.type,source.loops,containerPath,leafPath};
        RetainedSequenceCursorDecision decision;const auto callback=services_.boundary;
        if(!callback(boundary,decision,error))return false;
        if(generation_!=generation||!active_)return true;
        if(decision.stop)return true;
        // Explicit scoped leaf/group playback never escapes its selected source
        // boundary even if source callbacks express a broader root continuation.
        if(scope_path_.size()==leafPath.size())return true;
        std::uint32_t cursor=decision.setStep?*decision.setStep:boundary.step;
        if(decision.skipNext)++cursor;
        if(source.type==1)++cursor;
        if(source.type==1&&cursor<source.count){
            auto prefix=containerPath;prefix.push_back(cursor);
            for(std::size_t candidate=0;candidate<phases_.size();++candidate){
                const auto& path=phases_[candidate].source.sourcePath;
                if(prefix.size()<=path.size()&&std::equal(prefix.begin(),prefix.end(),path.begin())){
                    next=candidate;beginDepth=depth;return true;
                }
            }
            error="Source cursor redirected to a leaf outside the prepared selection";return false;
        }
        if(source.loops!=0){error="Source repeated hierarchy needs an explicit replay/selection owner";return false;}
        if(!scope_path_.empty()&&containerPath==scope_path_)return true;
    }
    return true;
}
bool RetainedSequencePlayback::enter(std::size_t phase,std::int32_t extra,std::string& error){
    const auto& selected=phases_[phase];RetainedAnimationFrame frame;
    if(!animation_->select(selected.source.clipName,false,static_cast<float>(selected.rate),
       static_cast<std::int32_t>(selected.source.blendOut),selected.source.moveGO!=0,frame,error,extra))return false;
    return deliver(phase,frame,error);
}
bool RetainedSequencePlayback::begin(std::string& error){
    if(!animation_||phases_.empty()){error="Retained sequence is not prepared";return false;}
    if(generation_==UINT64_MAX){error="Retained sequence generation exhausted";return false;}
    ++generation_;phase_=0;completed_=0;active_=true;finished_=false;seeded_=false;
    animation_->take_completion(); // old action's latch cannot close a new action
    const auto generation=generation_;
    if(!begin_hierarchy(0,0,error)){active_=false;return false;}
    if(generation_!=generation||!active_){error.clear();return true;}
    // Preserve retained slot identities/history across separate source actions.
    if(!enter(0,0,error)){active_=false;return false;}error.clear();return true;
}
bool RetainedSequencePlayback::advance(double seconds,std::string& error){
    if(!active_){error="Retained sequence is not active";return false;}
    if(!std::isfinite(seconds)||seconds<0){error="Invalid retained sequence wall interval";return false;}
    const auto generation=generation_;const auto phase=phase_;RetainedAnimationFrame frame;
    if(!animation_->advance(seconds,frame,error)){active_=false;return false;}
    // Source scene phase events/root precede animator Step completion selection.
    if(!deliver(phase,frame,error)){if(generation_==generation)active_=false;return false;}
    if(generation_!=generation||!active_){error.clear();return true;}
    const auto completion=animation_->take_completion();
    if(!completion.pending){error.clear();return true;}
    ++completed_;
    if(services_.boundary){
        std::optional<std::size_t> nextPhase;std::size_t beginDepth=0;
        if(!complete_hierarchy(nextPhase,beginDepth,error)){if(generation_==generation)active_=false;return false;}
        if(generation_!=generation||!active_){error.clear();return true;}
        if(!nextPhase){active_=false;finished_=true;const auto callback=services_.closed;return callback(completion,error);}
        phase_=*nextPhase;
        if(!begin_hierarchy(phase_,beginDepth,error)){if(generation_==generation)active_=false;return false;}
        if(generation_!=generation||!active_){error.clear();return true;}
        if(!enter(phase_,completion.extra_ms,error)){if(generation_==generation)active_=false;return false;}
        error.clear();return true;
    }
    if(phase_+1==phases_.size()){
        active_=false;finished_=true;
        const auto callback=services_.closed;return callback(completion,error);
    }
    ++phase_;
    // No wall subframe remainder is advanced. NewAnim selects at the sampled
    // timestamp; ONLY the selected physical slot's same-ID replay can use extra.
    if(!enter(phase_,completion.extra_ms,error)){if(generation_==generation)active_=false;return false;}
    error.clear();return true;
}
void RetainedSequencePlayback::cancel() noexcept{
    active_=false;finished_=false;seeded_=false;if(generation_!=UINT64_MAX)++generation_;
    if(animation_)animation_->clear();
}
}
