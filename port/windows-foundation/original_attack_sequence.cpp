#include "original_attack_sequence.hpp"
#include <algorithm>
#include <charconv>
#include <cmath>
#include <cstring>
#include <limits>
#include <set>
#include <stdexcept>

namespace dh::foundation {
namespace {
void require(bool v,const std::string& e){if(!v)throw std::runtime_error(e);}
std::int64_t number(const OriginalBindingProperties& p,const char* name){const auto found=p.find(name);require(found!=p.end(),std::string("Missing source sequence policy ")+name);std::int64_t value{};const auto parsed=std::from_chars(found->second.data(),found->second.data()+found->second.size(),value);require(parsed.ec==std::errc{}&&parsed.ptr==found->second.data()+found->second.size(),"Invalid source sequence policy number");return value;}
bool prefix(const std::vector<std::size_t>& a,const std::vector<std::size_t>& b){return a.size()<=b.size()&&std::equal(a.begin(),a.end(),b.begin());}
std::int32_t wall_ms(double seconds){const double value=std::ceil(seconds*1000-1e-9);require(std::isfinite(value)&&value>=0&&value<=INT32_MAX,"Virtual attack timeline exceeds limit");return static_cast<std::int32_t>(value);}
}
bool original_sequence_policies(const OriginalMeleeBindings& bindings,OriginalSequencePolicies& output,std::string& error){try {
    OriginalSequencePolicies next;bool found=false;
    for(const auto& node:bindings.extra_nodes())if(node.tag=="sequencePolicies") {
        require(!found,"Duplicate source sequence-policy directory");found=true;
        for(const auto& row:node.children)if(row.tag=="sequence") {
            OriginalSequencePolicy policy;policy.id=number(row.properties,"id");policy.type=number(row.properties,"type");policy.loop=number(row.properties,"loop");
            const auto name=row.properties.find("name");if(name!=row.properties.end())policy.name=name->second;
            require(policy.id>=0,"Invalid source sequence policy ID");require(next.emplace(policy.id,policy).second,"Duplicate source sequence policy ID");
        }
    }
    require(found&&!next.empty(),"Original redirected sequence type/loop metadata is required");output=std::move(next);error.clear();return true;
}catch(const std::exception& e){error=e.what();return false;}}
bool OriginalAttackSequence::prepare(const OriginalCombatVisualPlan& plan,const OriginalSequencePolicies& policies,
    const OriginalAttackSelection& selection,OriginalAttackSequenceServices services,const std::string& alias,std::string& error){try {
    require(!alias.empty(),"Virtual attack action name is required");require(services.restart_clip&&services.visual.select&&services.visual.range&&services.visual.markers&&services.visual.update,"Complete attack visual/restart services are required");
    require(bool(services.take_motion)==bool(services.apply_motion),"Attack motion extraction and application services must be paired");
    require(std::isfinite(selection.actor_rate)&&selection.actor_rate>0&&selection.actor_rate<=std::numeric_limits<float>::max(),"Invalid attack actor rate");
    const auto* sequence=plan.sequence(selection.state,selection.variant);require(sequence,"Explicit original state/variant does not exist");require(!sequence->phases.empty(),"Original attack state has no phases");
    std::set<std::vector<std::size_t>> sourcePaths,usedChoices;
    for(const auto& phase:sequence->phases)require(!phase.sourcePath.empty()&&sourcePaths.insert(phase.sourcePath).second,"Duplicate or empty original phase path");
    std::vector<const OriginalCombatPhase*> selected;
    std::function<void(std::vector<std::size_t>)> visit=[&](std::vector<std::size_t> path) {
        const OriginalCombatPhase* first=nullptr;std::set<std::size_t> children;
        for(const auto& phase:sequence->phases)if(prefix(path,phase.sourcePath)) {
            if(!first)first=&phase;
            if(phase.sourcePath.size()==path.size()) {require(phase.redirect==0&&phase.has_visual(),"Nonvisual or symbolic original attack step is unsupported");selected.push_back(&phase);return;}
            children.insert(phase.sourcePath[path.size()]);
        }
        require(first&&!children.empty(),"Original phase hierarchy is incomplete");require(path.size()<3,"Original redirect depth exceeds supported source stack");
        std::size_t expectedChild=0;for(const auto child:children)require(child==expectedChild++,"Original sequence child hierarchy has missing steps");
        OriginalSequencePolicy policy{sequence->id,sequence->type,sequence->loop,sequence->name};
        if(!path.empty()) {
            require(first->ancestors.size()>=path.size(),"Missing redirected original sequence ancestry");const auto& redirect=first->ancestors[path.size()-1];require(redirect.redirect==1,"Unsupported original redirect mode");
            const auto found=policies.find(redirect.animationId);require(found!=policies.end(),"Missing redirected sequence Type/Loop for "+std::to_string(redirect.animationId));policy=found->second;
        }
        require(policy.loop==0,"Nonzero original sequence loops require additional replay/selection policy");require(policy.type>=0&&policy.type<=2,"Unsupported original sequence type");
        std::vector<std::size_t> indexes;
        if(policy.type==1)indexes.assign(children.begin(),children.end());
        else if(policy.type==2) {const auto choice=selection.choices.find(path);require(choice!=selection.choices.end(),"Explicit choice required for original type2 group");require(children.count(choice->second)!=0,"Original type2 group choice outside source steps");usedChoices.insert(path);indexes.push_back(choice->second);}
        else {require(children.count(0)!=0,"Original type0 first step is absent");indexes.push_back(0);}
        for(auto index:indexes){auto child=path;child.push_back(index);visit(std::move(child));}
    };
    visit(selection.group_path);require(!selected.empty()&&selected.size()<=4096,"Selected attack phase count is invalid");
    require(usedChoices.size()==selection.choices.size(),"Explicit type2 choice does not correspond to selected source hierarchy");
    std::vector<OriginalAttackPhase> phases;std::vector<OriginalAttackScheduledMarker> schedule;double total=0;
    for(const auto* source:selected) {
        require(source->clipName!=alias,"Virtual attack alias conflicts with source clip name");require(std::isfinite(source->speed)&&source->speed>0&&source->speed<=std::numeric_limits<float>::max(),"Original phase rate is invalid");
        volatile float actualRate=static_cast<float>(selection.actor_rate)*static_cast<float>(source->speed);require(std::isfinite(actualRate)&&actualRate>0,"Original effective phase rate is invalid");
        OriginalAttackPhase phase;phase.source=*source;phase.rate=actualRate;require(services.visual.range(source->clipName,phase.start_ms,phase.end_ms,error),error);require(phase.end_ms>phase.start_ms,"Original phase clip range is invalid");
        const auto* markers=services.visual.markers(source->clipName,error);require(markers,error);require(markers->start_ms()==phase.start_ms&&markers->end_ms()==phase.end_ms,"Source phase marker/visual ranges differ");
        phase.wall_start_seconds=total;phase.wall_duration_seconds=(std::int64_t(phase.end_ms)-phase.start_ms)/1000.0/phase.rate;
        for(const auto& marker:markers->markers())if(marker.time_ms>=phase.start_ms&&marker.time_ms<=phase.end_ms) {
            require(schedule.size()<1000000,"Original attack marker schedule exceeds limit");
            const auto time=total+(std::int64_t(marker.time_ms)-phase.start_ms)/1000.0/phase.rate;
            schedule.push_back({marker,phases.size(),time,wall_ms(time)});
        }
        total+=phase.wall_duration_seconds;require(std::isfinite(total),"Original attack duration overflow");phases.push_back(std::move(phase));
    }
    std::stable_sort(schedule.begin(),schedule.end(),[](const auto& a,const auto& b){if(a.virtual_ms!=b.virtual_ms)return a.virtual_ms<b.virtual_ms;if(a.phase!=b.phase)return a.phase<b.phase;return a.source.index<b.source.index;});
    const auto end=wall_ms(total);require(end>0,"Virtual attack timeline is empty");
    std::vector<std::uint8_t> times(schedule.size()*4);std::vector<dh2::animation::EventGroup> groups(schedule.size());std::vector<const char*> names(schedule.size());
    for(std::size_t i=0;i<schedule.size();++i){const auto value=static_cast<std::uint32_t>(schedule[i].virtual_ms);for(unsigned byte=0;byte<4;++byte)times[i*4+byte]=std::uint8_t(value>>(byte*8));names[i]=schedule[i].source.name.c_str();groups[i]={1,&names[i]};}
    AnimationMarkers aggregate;const dh2::animation::EventView view{4,static_cast<std::uint32_t>(schedule.size()),times.data(),groups.data()};require(aggregate.load(view,0,end,error),error);
    alias_=alias;services_=std::move(services);phases_=std::move(phases);schedule_=std::move(schedule);virtual_markers_=std::move(aggregate);virtual_end_ms_=end;phase_=0;elapsed_=phase_elapsed_=0;active_=false;error.clear();return true;
}catch(const std::exception& e){error=e.what();return false;}}
bool OriginalAttackSequence::enter(std::size_t phase,std::string& error){if(!services_.restart_clip(phases_[phase].source.clipName,error)||!services_.visual.update(0,error)){active_=false;return false;}return true;}
bool OriginalAttackSequence::begin(std::string& error){if(phases_.empty()){error="Original attack sequence is not prepared";return false;}if(!enter(0,error))return false;phase_=0;elapsed_=phase_elapsed_=0;active_=true;error.clear();return true;}
bool OriginalAttackSequence::advance(double seconds,std::string& error){
    if(!std::isfinite(seconds)||seconds<0){error="Invalid original attack wall interval";return false;}
    if(!active_){error="Original attack sequence is not active";return false;}
    while(phase_<phases_.size()) {
        const auto& current=phases_[phase_];const double remaining=std::max(0.0,current.wall_duration_seconds-phase_elapsed_);const double step=std::min(seconds,remaining);
        if(step>0&&!services_.visual.update(step*current.rate,error)){active_=false;return false;}
        if(step>0&&services_.take_motion) {
            const auto motion=services_.take_motion();
            if(!services_.apply_motion(motion,current.source.moveGO!=0,error)){active_=false;return false;}
        }
        phase_elapsed_+=step;elapsed_+=step;seconds-=step;
        if(phase_elapsed_+1e-12<current.wall_duration_seconds)break;
        phase_elapsed_=0;++phase_;
        if(phase_>=phases_.size())break;
        if(!enter(phase_,error))return false;
        if(seconds<=0)break;
    }
    error.clear();return true;
}
CombatVisualBinding OriginalAttackSequence::binding(){return {
    [this](const std::string& name,bool loop,std::string& error){if(phases_.empty()){error="Original attack sequence is not prepared";return false;}if(name!=alias_){active_=false;return services_.visual.select(name,loop,error);}if(loop){error="Original finite attack does not support virtual looping";return false;}return begin(error);},
    [this](const std::string& name,std::int32_t& start,std::int32_t& end,std::string& error){if(phases_.empty()){error="Original attack sequence is not prepared";return false;}if(name!=alias_)return services_.visual.range(name,start,end,error);start=0;end=virtual_end_ms_;error.clear();return true;},
    [this](const std::string& name,std::string& error)->const AnimationMarkers*{if(phases_.empty()){error="Original attack sequence is not prepared";return nullptr;}if(name!=alias_)return services_.visual.markers(name,error);error.clear();return &virtual_markers_;},
    [this](double seconds,std::string& error){if(phases_.empty()){error="Original attack sequence is not prepared";return false;}return active_?advance(seconds,error):services_.visual.update(seconds,error);}
};}
}
