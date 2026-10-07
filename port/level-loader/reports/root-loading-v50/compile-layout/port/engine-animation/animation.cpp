#include "animation.hpp"
#include "angle_interpreter.hpp"
#include "../engine-math/math.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>

namespace {
float add(float a,float b){volatile float out=a+b;return out;}
float multiply(float a,float b){volatile float out=a*b;return out;}
unsigned word(const std::uint8_t* p){return p[0]|(unsigned(p[1])<<8)|(unsigned(p[2])<<16)|(unsigned(p[3])<<24);}
}
extern "C" void dh2_animation_lerp3(float* out,const float* a,const float* b,float fraction){
    volatile float inverse=1.0f-fraction;
    for(unsigned i=0;i<3;++i)out[i]=add(add(multiply(inverse,a[i]),0.f),multiply(fraction,b[i]));
}
extern "C" void dh2_animation_quaternion(float* out,const float* a,const float* b,float fraction){
    // CBlender skips zero weights, starts with the first nonzero quaternion,
    // then slerps with weight/sum. Preserve the float-rounded sum operation.
    volatile float inverse=1.0f-fraction;
    if(inverse==0.f){std::copy(b,b+4,out);return;}
    if(inverse==1.f||fraction==0.f){std::copy(a,a+4,out);return;}
    volatile float sum=inverse+fraction;
    volatile float t=fraction/sum;
    dh2::math::Quaternion left{a[0],a[1],a[2],a[3]},right{b[0],b[1],b[2],b[3]},value;
    dh2_quat_slerp(&value,&left,&right,t);std::memcpy(out,&value,16);
}
namespace dh2::animation {
namespace {
struct DefaultReader {
    const resources::BresView& image;
    const std::uint8_t* at(std::uint64_t offset,std::uint64_t size)const{
        if(offset>image.size||size>image.size-offset)throw std::runtime_error("Compiled default outside BRES");
        return image.bytes+offset;
    }
    std::uint32_t w(std::uint64_t offset)const{return word(at(offset,4));}
    std::string name(std::uint32_t offset)const{
        if(!offset)throw std::runtime_error("Missing compiled default node ID");
        const auto* first=at(offset,1);
        const auto* end=static_cast<const std::uint8_t*>(std::memchr(first,0,std::min<std::size_t>(image.size-offset,4096)));
        if(!end)throw std::runtime_error("Invalid compiled default node ID");
        return std::string(reinterpret_cast<const char*>(first),end-first);
    }
    std::uint32_t find_node(std::uint32_t offset,const std::string& uri,unsigned depth,unsigned& visited)const{
        if(depth>64||++visited>10000)throw std::runtime_error("Excessive compiled default graph");
        at(offset,80);
        if(name(w(offset))==uri)return offset;
        const auto count=w(offset+56),children=w(offset+60);
        if(count>10000||(!children&&count))throw std::runtime_error("Invalid compiled default children");
        at(children,std::uint64_t(count)*80);
        for(unsigned i=0;i<count;++i){const auto found=find_node(children+i*80,uri,depth+1,visited);if(found)return found;}
        return 0;
    }
    bool get(const TransformTarget& target,float* out)const{
        // CColladaDatabase::getNode calls getVisualScene(0) at 61c29c.
        const auto root=image.root_offset,count=w(root+152),scenes=w(root+156);
        if(!count)return false;
        if(count>10000||!scenes)throw std::runtime_error("Invalid compiled default scene library");
        at(scenes,std::uint64_t(count)*16);
        const auto n=w(scenes+8),nodes=w(scenes+12);
        if(n>10000||(!nodes&&n))throw std::runtime_error("Invalid compiled default roots");
        at(nodes,std::uint64_t(n)*80);unsigned visited=0;
        for(unsigned i=0;i<n;++i){
            const auto node=find_node(nodes+i*80,target.uri,0,visited);if(!node)continue;
            const auto property=target.type>=1&&target.type<=4?12:target.type>=5&&target.type<=9?24:40;
            std::memcpy(out,at(node+property,target.components*4),target.components*4);return true;
        }
        return false;
    }
};
bool overlap(std::uintptr_t a,std::size_t an,std::uintptr_t b,std::size_t bn){
    return a<=b?b-a<an:a-b<bn;
}
bool find_cursor_key(const assets::Animation& accessor,std::int32_t ms,std::int32_t prior,
                     std::int32_t& key,float& fraction,bool& between){
    assets::Vector times{};if(!dh2_animation_vector(&accessor,0,false,&times)||!times.count)return false;
    const auto type=dh2_animation_time_type(&accessor,0);
    const float factor=type==4?1.0f:0x1.0aaaaap+5f;
    const float query=static_cast<float>(ms);volatile float frame=query/factor;
    auto time=[&](std::int32_t index){float value=0;dh2_vector_read(&times,index,&value);return value;};
    const auto last=static_cast<std::int32_t>(times.count)-1;
    key=std::max(0,std::min(last,prior));
    // Original66a368/66a894/66b... probes at most one backward/two forward
    // neighbors, preserving an inclusive upper bound before binary fallback.
    if(time(key)>frame){if(key>0)--key;}
    else if(key<last&&time(key+1)<frame){++key;if(key<last&&time(key+1)<frame)++key;}
    if(time(key)>frame||(key<last&&time(key+1)<frame)){
        // Queries before the first key retain key0, as does the source fallback.
        dh2_animation_find_index(&accessor,0,ms,&key);
    }
    const float start_float=multiply(time(key),factor);
    between=dh2_animation_interpolation(&accessor,0)!=0&&query!=start_float&&key!=last;
    if(!between)return true;
    const float end_float=multiply(time(key+1),factor);
    if(!(start_float>=-2147483648.0f&&start_float<2147483648.0f&&end_float>=-2147483648.0f&&end_float<2147483648.0f))return false;
    const auto start=static_cast<std::int32_t>(start_float),end=static_cast<std::int32_t>(end_float);
    const auto wrapped=[](std::uint32_t word){std::int32_t result;std::memcpy(&result,&word,4);return result;};
    volatile float ratio=static_cast<float>(wrapped(std::uint32_t(ms)-std::uint32_t(start)))/static_cast<float>(wrapped(std::uint32_t(end)-std::uint32_t(start)));
    fraction=ratio<0.f?0.f:ratio<1.f?ratio:1.f;return true;
}
}
std::int32_t TransformSet::find_clip(std::int32_t id)const{
    for(std::size_t i=0;i<clips_.size();++i)if(clips_[i].id==id)return static_cast<std::int32_t>(i);
    return -1;
}
const TransformBinding* TransformSet::clip_target(std::size_t ci,std::size_t ti)const{
    return ci<clips_.size()&&ti<targets_.size()?&bindings_[ci*targets_.size()+ti]:nullptr;
}
bool TransformSet::compile(const std::vector<TransformClipInput>& inputs,const scene::Scene& authored,std::string& error,TransformTemplatePolicy policy){
    return compile_internal(inputs,authored,error,policy,false);
}
bool TransformSet::compile_internal(const std::vector<TransformClipInput>& inputs,const scene::Scene& authored,std::string& error,TransformTemplatePolicy policy,bool dynamic_channels){
    error.clear();try{
        if(inputs.empty()||inputs.size()>1024||authored.graph.size()>10000||(policy!=TransformTemplatePolicy::authored&&policy!=TransformTemplatePolicy::none))throw std::runtime_error("Compiled transform set exceeds limits");
        for(std::size_t i=0;i<authored.graph.size();++i){
            if(authored.graph[i].id.empty())throw std::runtime_error("Missing authored target ID");
            for(std::size_t j=0;j<i;++j)if(authored.graph[j].id==authored.graph[i].id)throw std::runtime_error("Ambiguous authored target ID");
        }
        TransformSet candidate;candidate.storage_.reserve(inputs.size());candidate.clips_.reserve(inputs.size());
        std::vector<std::vector<std::uint32_t>> unions;
        for(const auto& input:inputs){
            if(!input.player||input.player->bytes.empty()||candidate.find_clip(input.id)>=0)throw std::runtime_error("Invalid compiled clip input");
            Player validated;
            if(!validated.load(input.player->bytes.data(),input.player->bytes.size(),authored,error,MissingTargets::ignore))throw std::runtime_error(error);
            if(validated.skipped&&!dynamic_channels)throw std::runtime_error("Compiled transform set requires full position/quaternion/scale channels");
            Storage storage;storage.bytes=std::move(validated.bytes);storage.ranges=std::move(validated.ranges);
            resources::BresView image{};
            if(dh2_bres_open(&image,storage.bytes.data(),storage.bytes.size())!=resources::BresError::ok)throw std::runtime_error("Compiled animation BRES rejected");
            const auto count=dh2_bres_library_count(&image,resources::Library::animation);
            const auto segments=storage.ranges.size();
            TransformClip clip;clip.id=input.id;clip.events=std::move(validated.events);
            if(!storage.ranges.empty()){clip.start=storage.ranges.front()[0];clip.end=storage.ranges.back()[1];}
            std::vector<std::uint32_t> mapping;
            for(unsigned segment=0;segment<segments;++segment)for(unsigned i=0;i<count;++i){
                assets::Animation accessor{};
                if(dh2_animation_open(&accessor,&image,i,segment)!=assets::Error::ok)throw std::runtime_error("Compiled animation accessor rejected");
                const auto type=dh2_animation_type(&accessor,0);const auto* uri=dh2_animation_target(&accessor);
                const bool position_axis=type>=2&&type<=4;
                const bool angle=type==9;
                if(!uri||(type!=1&&type!=5&&type!=10&&!(dynamic_channels&&(position_axis||angle))))throw std::runtime_error("Unsupported compiled animation channel");
                assets::Vector values{};if(!dh2_animation_vector(&accessor,0,true,&values))throw std::runtime_error("Compiled key vector rejected");
                if(dynamic_channels){
                    assets::Vector times{};
                    const unsigned key_components=type==5?4:position_axis||angle?1:3;
                    if(dh2_animation_channels(&accessor)!=1||dh2_animation_samplers(&accessor)!=1||dh2_animation_scales(&accessor)||dh2_animation_offsets(&accessor)
                       ||values.type!=6||values.components!=key_components||!values.count||values.count>100000
                       ||!dh2_animation_vector(&accessor,0,false,&times)||times.count!=values.count||times.components!=1
                       ||(times.type!=1&&times.type!=3&&times.type!=4)||dh2_animation_interpolation(&accessor,0)>1
                       ||word(image.bytes+accessor.record+20))throw std::runtime_error("Unsupported dynamic key layout");
                    std::int32_t prior=INT32_MIN;
                    for(unsigned k=0;k<values.count;++k){
                        float key[4]{};if(!dh2_vector_read(&values,k,key))throw std::runtime_error("Dynamic key rejected");
                        for(unsigned c=0;c<key_components;++c)if(!std::isfinite(key[c]))throw std::runtime_error("Nonfinite dynamic key");
                        const auto time=dh2_animation_key_time(&accessor,0,k);
                        if(time<prior)throw std::runtime_error("Dynamic times out of order");
                        prior=time;
                    }
                    if(position_axis||angle){
                        const auto* defaults=dh2_animation_default(&accessor);
                        const auto bytes=angle?16U:12U;
                        if(angle&&!defaults)throw std::runtime_error("Angle track requires authored axis default");
                        if(defaults&&(defaults<image.bytes||std::size_t(defaults-image.bytes)>image.size||bytes>image.size-std::size_t(defaults-image.bytes)))throw std::runtime_error("Dynamic interpreter default outside BRES");
                    }
                }
                storage.tracks.push_back({i,type,segment,accessor,values});
                if(segment)continue;
                const auto node=std::find_if(authored.graph.begin(),authored.graph.end(),[&](const scene::Node& n){return n.id==uri;});
                // TransformationTemplate::hasTarget(6e22cc), called at6607ac,
                // filters unregistered URIs before the first-seen union pass.
                if(policy==TransformTemplatePolicy::authored&&node==authored.graph.end()){mapping.push_back(UINT32_MAX);continue;}
                auto found=std::find_if(candidate.targets_.begin(),candidate.targets_.end(),[&](const TransformTarget& t){
                    const bool compatible=t.type==type||(dynamic_channels&&((t.type>=1&&t.type<=4&&type>=1&&type<=4)||((t.type==5||t.type==9)&&(type==5||type==9))));
                    return compatible&&t.uri==uri;
                });
                std::uint32_t target;
                if(found==candidate.targets_.end()){
                    target=static_cast<std::uint32_t>(candidate.targets_.size());
                    if(target>=10000)throw std::runtime_error("Compiled target union exceeds limit");
                    candidate.targets_.push_back({uri,type,node==authored.graph.end()?UINT32_MAX:static_cast<std::uint32_t>(node-authored.graph.begin()),type==5||angle?4U:3U});
                }else target=static_cast<std::uint32_t>(found-candidate.targets_.begin());
                if(!dynamic_channels&&std::find(mapping.begin(),mapping.end(),target)!=mapping.end())throw std::runtime_error("Duplicate compiled clip channel");
                mapping.push_back(target);
            }
            unions.push_back(std::move(mapping));candidate.storage_.push_back(std::move(storage));candidate.clips_.push_back(std::move(clip));
        }
        if(std::uint64_t(inputs.size())*candidate.targets_.size()>1000000)throw std::runtime_error("Compiled binding table exceeds limit");
        for(std::size_t ci=0;ci<inputs.size();++ci){
            resources::BresView image{};const auto& bytes=candidate.storage_[ci].bytes;
            dh2_bres_open(&image,bytes.data(),bytes.size());DefaultReader defaults{image};
            for(std::size_t ti=0;ti<candidate.targets_.size();++ti){
                TransformBinding binding;const auto& target=candidate.targets_[ti];
                const auto track=std::find(unions[ci].begin(),unions[ci].end(),ti);
                const auto animation=track==unions[ci].end()?UINT32_MAX:static_cast<std::uint32_t>(track-unions[ci].begin());
                binding.mode=animation==UINT32_MAX?1U:2U;binding.has_default=defaults.get(target,binding.default_value);
                // At 660980 a present animation branches directly to mode2;
                // only absent mode1 with no clip DB default reaches 6609cc.
                if(policy==TransformTemplatePolicy::authored&&binding.mode==1&&!binding.has_default&&target.node!=UINT32_MAX){
                    const auto& node=authored.graph[target.node];const float* value=target.type==1?node.translation:target.type==5?node.quaternion:node.scale;
                    std::memcpy(binding.default_value,value,target.components*4);binding.has_default=true;
                }
                candidate.bindings_.push_back(binding);candidate.animations_.push_back(animation);
            }
        }
        *this=std::move(candidate);return true;
    }catch(const std::exception& exception){error=exception.what();return false;}
}
bool TransformSet::compile_dynamic(const std::vector<TransformClipInput>& inputs,const scene::Scene& scene_bindings,std::string& error,const Player* default_library,TransformMismatchBehavior mismatch){
    error.clear();try{
        if(mismatch!=TransformMismatchBehavior::prune&&mismatch!=TransformMismatchBehavior::retain)throw std::runtime_error("Invalid dynamic mismatch policy");
        TransformSet candidate;
        // Source compatibility groups position1..4 and quaternion5/angle9.
        // The first registered handler controls union width/application, while
        // every raw accessor retains its own interpreter for key production.
        if(!candidate.compile_internal(inputs,scene_bindings,error,TransformTemplatePolicy::none,true))return false;
        resources::BresView defaults{};
        if(default_library&&(default_library->bytes.empty()||dh2_bres_open(&defaults,default_library->bytes.data(),default_library->bytes.size())!=resources::BresError::ok))throw std::runtime_error("Invalid dynamic default library");
        const auto old_count=candidate.targets_.size();
        std::vector<std::uint32_t> retained;
        for(std::uint32_t ti=0;ti<old_count;++ti){
            bool keep=true;
            if(mismatch==TransformMismatchBehavior::prune)for(std::size_t ci=0;ci<inputs.size();++ci){
                const auto& binding=candidate.bindings_[ci*old_count+ti];
                if(binding.mode==1&&!binding.has_default){keep=false;break;}
            }
            if(keep)retained.push_back(ti);
        }
        std::vector<TransformTarget> targets;std::vector<TransformBinding> bindings;std::vector<std::uint32_t> animations;
        for(const auto ti:retained)targets.push_back(candidate.targets_[ti]);
        for(std::size_t ci=0;ci<inputs.size();++ci){
            resources::BresView image{};const auto& bytes=candidate.storage_[ci].bytes;dh2_bres_open(&image,bytes.data(),bytes.size());
            const auto* root=image.bytes+image.root_offset;
            candidate.clips_[ci].start=static_cast<std::int32_t>(word(root+28));candidate.clips_[ci].end=static_cast<std::int32_t>(word(root+32));
            for(const auto ti:retained){
                auto binding=candidate.bindings_[ci*old_count+ti];
                if(default_library&&!binding.has_default)binding.has_default=DefaultReader{defaults}.get(candidate.targets_[ti],binding.default_value);
                bindings.push_back(binding);animations.push_back(candidate.animations_[ci*old_count+ti]);
            }
        }
        candidate.targets_=std::move(targets);candidate.bindings_=std::move(bindings);candidate.animations_=std::move(animations);
        *this=std::move(candidate);return true;
    }catch(const std::exception& exception){error=exception.what();return false;}
}
bool TransformSet::sample(std::size_t ci,std::size_t ti,std::int32_t ms,float* out,std::size_t capacity,std::int32_t* cursor,std::string& error,bool interpolate)const{
    error.clear();const auto* binding=clip_target(ci,ti);
    if(!binding||!out||capacity<targets_[ti].components||reinterpret_cast<std::uintptr_t>(out)%alignof(float)
       ||(cursor&&(reinterpret_cast<std::uintptr_t>(cursor)%alignof(std::int32_t)||overlap(reinterpret_cast<std::uintptr_t>(out),targets_[ti].components*4,reinterpret_cast<std::uintptr_t>(cursor),4)))){
        error="Invalid compiled target sampling storage";return false;
    }
    const auto components=targets_[ti].components;float result[4];std::memcpy(result,out,components*4);
    if(binding->has_default)std::memcpy(result,binding->default_value,components*4);
    std::int32_t selected=0;bool evaluated=false;
    if(binding->mode==2){
        const auto& storage=storage_[ci];unsigned segment=0;
        while(segment+1<storage.ranges.size()&&ms>=storage.ranges[segment][1])++segment;
        const auto count=storage.ranges.empty()?0:storage.tracks.size()/storage.ranges.size();
        const auto animation=animations_[ci*targets_.size()+ti];
        if(!count||animation>=count){error="Invalid compiled accessor binding";return false;}
        const auto& track=storage.tracks[segment*count+animation];float fraction=0;bool between=false;
        if(!find_cursor_key(track.accessor,ms,cursor?*cursor:0,selected,fraction,between)){error="Compiled animation cursor search rejected";return false;}
        if(selected<0||unsigned(selected)>=track.values.count){error="Compiled animation key out of range";return false;}
        float first[4]{},second[4]{};
        if(!dh2_vector_read(&track.values,selected,first)){error="Compiled animation key rejected";return false;}
        if(between&&interpolate){
            if(unsigned(selected)+1>=track.values.count||!dh2_vector_read(&track.values,selected+1,second)){error="Compiled animation successor rejected";return false;}
            if(track.type==5)dh2_animation_quaternion(result,first,second,fraction);
            else if(track.type==1||track.type==10)dh2_animation_lerp3(result,first,second,fraction);
        }
        const bool position_axis=track.type>=2&&track.type<=4;
        if(position_axis){
            const auto* defaults=dh2_animation_default(&track.accessor);
            if(defaults)std::memcpy(result,defaults,12);
            float value=first[0];
            if(between&&interpolate){volatile float delta=second[0]-first[0];value=add(first[0],multiply(fraction,delta));}
            result[defaults?track.type-2:0]=value;
        }else if(track.type==9){
            const auto* raw_default=dh2_animation_default(&track.accessor);
            float authored_default[4];std::memcpy(authored_default,raw_default,16);
            const float values[2]{first[0],second[0]};
            const AngleAccessor24 accessor{values,authored_default,2,0};
            math::Quaternion quaternion;
            const auto status=between&&interpolate?dh2_animation_angle_between(&quaternion,&accessor,0,1,fraction):dh2_animation_angle_key(&quaternion,&accessor,0);
            if(status){error="Angle interpreter rejected";return false;}
            std::memcpy(result,&quaternion,16);
        }else if(!(between&&interpolate))std::memcpy(result,first,components*4);
        evaluated=true;
    }
    std::memcpy(out,result,components*4);if(cursor&&evaluated)*cursor=selected;return true;
}
bool Player::load(const std::uint8_t* input,std::size_t size,const scene::Scene& scene,std::string& error,MissingTargets policy){
    tracks.clear();ranges.clear();rest.clear();bytes.clear();events=EventTrack{};start=end=0;skipped=unbound=0;error.clear();
    try{
        if(!input||!size||size>32*1024*1024)throw std::runtime_error("Animation image exceeds limit");
        bytes.assign(input,input+size);resources::BresView view{};
        if(dh2_bres_open(&view,bytes.data(),bytes.size())!=resources::BresError::ok)throw std::runtime_error("Animation BRES rejected");
        if(!events.load(view,error))throw std::runtime_error(error);
        rest=scene.graph;
        const auto count=dh2_bres_library_count(&view,resources::Library::animation);
        if(count>10000)throw std::runtime_error("Animation track count exceeds limit");
        const auto segments=dh2_animation_segments(&view);
        if(!count)return true;
        if(!segments||segments>256||std::uint64_t(segments)*count>100000)throw std::runtime_error("Animation segment count exceeds limit");
        for(unsigned segment=0;segment<segments;++segment){
        for(unsigned i=0;i<count;++i){
            assets::Animation a{};
            if(dh2_animation_open(&a,&view,i,segment)!=assets::Error::ok)throw std::runtime_error("Animation accessor rejected");
            if(i==0){
                if(a.segment_end<=a.segment_start||(segment&&a.segment_start!=ranges.back()[1]))throw std::runtime_error("Animation segments are not contiguous");
                ranges.push_back({a.segment_start,a.segment_end});
            }else if(a.segment_start!=ranges.back()[0]||a.segment_end!=ranges.back()[1])throw std::runtime_error("Track segment ranges differ");
            if(dh2_animation_channels(&a)!=1||dh2_animation_samplers(&a)!=1){++skipped;continue;}
            const auto type=dh2_animation_type(&a,0);
            // Factory branch 10 chooses CSceneNodeScaleMixin<float> when
            // offsets/scales are absent. Other properties are not guessed.
            if(type!=1&&type!=5&&type!=10){++skipped;continue;}
            // Absolute key playback does not read the optional rest/default
            // record. Offset/scale interpreters select different factories.
            if(dh2_animation_scales(&a)||dh2_animation_offsets(&a))
                throw std::runtime_error("Track compression scales are not implemented");
            auto* target=dh2_animation_target(&a);if(!target)throw std::runtime_error("Missing animation target");
            auto node=std::find_if(rest.begin(),rest.end(),[&](const scene::Node& n){return n.id==target;});
            if(node==rest.end()&&policy==MissingTargets::reject)throw std::runtime_error(std::string("Unresolved animation target: ")+target);
            assets::Vector values{},times{};
            const unsigned components=type==5?4:3;
            if(!dh2_animation_vector(&a,0,true,&values)||!dh2_animation_vector(&a,0,false,&times)||values.type!=6||values.components!=components||!values.count||values.count!=times.count||values.count>100000)
                throw std::runtime_error("Unsupported transform-key layout");
            if(times.components!=1||(times.type!=1&&times.type!=3&&times.type!=4)||dh2_animation_interpolation(&a,0)>1)
                throw std::runtime_error("Unsupported time or interpolation layout");
            std::int32_t prior=INT32_MIN;
            for(unsigned k=0;k<values.count;++k){
                float key[4]{};if(!dh2_vector_read(&values,k,key))throw std::runtime_error("Animation value rejected");
                for(unsigned c=0;c<components;++c)if(!std::isfinite(key[c]))throw std::runtime_error("Nonfinite animation key");
                if(type==5){float norm=0;for(float x:key)norm+=x*x;if(!std::isfinite(norm)||norm<.5f||norm>1.5f)throw std::runtime_error("Invalid quaternion key norm");}
                const auto time=dh2_animation_key_time(&a,0,k);
                if(time<prior)throw std::runtime_error("Animation times out of order");
                prior=time;
            }
            // The validated reader has already checked the inner key buffers.
            // Reject nonzero runtime animator state, rather than borrowing it.
            if(word(view.bytes+a.record+20))throw std::runtime_error("Runtime-mutated animation image");
            // Optimized models omit unused bones present in shared clips.
            // The original applyAnimationValues skips null target bindings
            // at 0x65d9c4..0x65da08. Still validate an unbound track's keys.
            if(node==rest.end()){++unbound;continue;}
            const unsigned index=node-rest.begin();
            if(std::any_of(tracks.begin(),tracks.end(),[&](const Track& t){return t.node==index&&t.type==type&&t.segment==segment;}))
                throw std::runtime_error("Multiple tracks target the same property");
            tracks.push_back({index,type,segment,a,values});
            if(tracks.size()==1){start=a.segment_start;end=a.segment_end;}
            else{start=std::min(start,a.segment_start);end=std::max(end,a.segment_end);}
        }
        }
        skipped/=segments;
        unbound/=segments;
        if(!tracks.empty()&&(std::int64_t(end)-start<=0||std::int64_t(end)-start>3600000))throw std::runtime_error("Animation playback range exceeds limit");
        return true;
    }catch(const std::exception& e){tracks.clear();ranges.clear();rest.clear();bytes.clear();events=EventTrack{};start=end=0;skipped=unbound=0;error=e.what();return false;}
}
bool Player::sample(scene::Scene& scene,std::int32_t ms,std::string& error)const{
    error.clear();if(scene.graph.size()!=rest.size()){error="Animation graph differs";return false;}
    auto candidate=scene;
    for(unsigned i=0;i<rest.size();++i){
        if(scene.graph[i].id!=rest[i].id){error="Animation target order differs";return false;}
        std::copy(rest[i].scale,rest[i].scale+3,candidate.graph[i].scale);
        std::copy(rest[i].translation,rest[i].translation+3,candidate.graph[i].translation);
        std::copy(rest[i].quaternion,rest[i].quaternion+4,candidate.graph[i].quaternion);
    }
    unsigned segment=0;while(segment+1<ranges.size()&&ms>=ranges[segment][1])++segment;
    for(const auto& t:tracks){
        if(t.segment!=segment)continue;
        std::int32_t key=0;float fraction=0;const bool interpolate=dh2_animation_find(&t.accessor,0,ms,&key,&fraction);
        if(key<0||unsigned(key)>=t.values.count){error="Selected animation key out of range";return false;}
        float a[4]{},b[4]{};dh2_vector_read(&t.values,key,a);
        float* destination=t.type==1?candidate.graph[t.node].translation:t.type==5?candidate.graph[t.node].quaternion:candidate.graph[t.node].scale;
        if(interpolate){
            if(unsigned(key)+1>=t.values.count){error="Animation successor out of range";return false;}
            dh2_vector_read(&t.values,key+1,b);
            if(t.type==5)dh2_animation_quaternion(destination,a,b,fraction);else dh2_animation_lerp3(destination,a,b,fraction);
        }else std::copy(a,a+(t.type==5?4:3),destination);
    }
    if(!scene::update_world(candidate,error))return false;
    scene=std::move(candidate);return true;
}
bool Player::sample_reuse(scene::Scene& scene,std::int32_t ms,PoseSampleWorkspaceV32& workspace,std::string& error)const{
    error.clear();++workspace.calls;
    if(scene.graph.size()!=rest.size()){error="Animation graph differs";return false;}
    for(unsigned i=0;i<rest.size();++i)if(scene.graph[i].id!=rest[i].id){error="Animation target order differs";return false;}
    const auto capacity=workspace.nodes.capacity();workspace.nodes.resize(rest.size());
    if(workspace.nodes.capacity()!=capacity)++workspace.storage_growths;
    for(unsigned i=0;i<rest.size();++i){auto& n=workspace.nodes[i];
        std::copy(rest[i].scale,rest[i].scale+3,n.scale);
        std::copy(rest[i].translation,rest[i].translation+3,n.translation);
        std::copy(rest[i].quaternion,rest[i].quaternion+4,n.quaternion);
    }
    unsigned segment=0;while(segment+1<ranges.size()&&ms>=ranges[segment][1])++segment;
    for(const auto& t:tracks){
        if(t.segment!=segment)continue;
        std::int32_t key=0;float fraction=0;const bool interpolate=dh2_animation_find(&t.accessor,0,ms,&key,&fraction);
        if(key<0||unsigned(key)>=t.values.count){error="Selected animation key out of range";return false;}
        float a[4]{},b[4]{};dh2_vector_read(&t.values,key,a);
        float* destination=t.type==1?workspace.nodes[t.node].translation:t.type==5?workspace.nodes[t.node].quaternion:workspace.nodes[t.node].scale;
        if(interpolate){
            if(unsigned(key)+1>=t.values.count){error="Animation successor out of range";return false;}
            dh2_vector_read(&t.values,key+1,b);
            if(t.type==5)dh2_animation_quaternion(destination,a,b,fraction);else dh2_animation_lerp3(destination,a,b,fraction);
        }else std::copy(a,a+(t.type==5?4:3),destination);
    }
    // Same transactional update_world arithmetic/order, in numeric scratch.
    // Metadata remains in the live graph and is validated freshly each call.
    for(unsigned i=0;i<scene.graph.size();++i){const auto parent=scene.graph[i].parent;auto& n=workspace.nodes[i];
        if(parent< -1||parent>=std::int32_t(i)){error="Invalid graph parent order";return false;}
        for(float x:n.translation)if(!std::isfinite(x)){error="Invalid node translation";return false;}
        for(float x:n.quaternion)if(!std::isfinite(x)){error="Invalid node rotation";return false;}
        for(float x:n.scale)if(!std::isfinite(x)){error="Invalid node scale";return false;}
        std::array<float,16> local;dh2_node_matrix(local.data(),n.translation,n.quaternion,n.scale);
        n.world=parent<0?local:scene::multiply(workspace.nodes[parent].world,local);
        for(float x:n.world)if(!std::isfinite(x)){error="World transform overflow";return false;}
    }
    for(const auto& instance:scene.instances)if(instance.node_index>=workspace.nodes.size()){
        error="Invalid instance node index";return false;
    }
    for(unsigned i=0;i<scene.graph.size();++i){auto& n=scene.graph[i];const auto& sampled=workspace.nodes[i];
        std::copy(sampled.translation,sampled.translation+3,n.translation);
        std::copy(sampled.quaternion,sampled.quaternion+4,n.quaternion);
        std::copy(sampled.scale,sampled.scale+3,n.scale);n.world=sampled.world;
    }
    for(auto& instance:scene.instances)instance.world=workspace.nodes[instance.node_index].world;
    return true;
}
}
