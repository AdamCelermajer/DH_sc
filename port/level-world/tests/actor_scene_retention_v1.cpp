#include "../actor_scene_retention_v1.hpp"
#include "../../game-data/animation_bank.hpp"
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
#include <tuple>
using namespace dh2;
namespace retention=actor_scene_retention_v1;
void ck(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
template<class T>bool same(const T& a,const T& b){return !std::memcmp(&a,&b,sizeof(T));}
struct Catalogue {
    std::filesystem::path assets;std::set<std::string> inputs;scene::Scene rest;
    data::Dictionary dictionary;data::AnimationTables tables;data::AnimationBank metadata;actor::ClipBank bank;
    std::vector<std::uint8_t> read(const std::string& name){inputs.insert(name);std::ifstream f(assets/name,std::ios::binary);ck(bool(f),"asset absent: "+name);return {std::istreambuf_iterator<char>(f),{}};}
    static data::Bytes bytes(const std::vector<std::uint8_t>& v){return {v.data(),v.size()};}
    explicit Catalogue(std::filesystem::path root):assets(std::move(root)){
        std::string error;auto a=read("data/animations_dictionary_pyarraynames.bin"),b=read("data/animations_dictionary_pyarray.bin");ck(data::load_dictionary(bytes(a),bytes(b),dictionary,error),error);
        a=read("data/animations_pyarray.bin");b=read("data/animations_pyarraynames.bin");auto c=read("data/animations_pystructnames.bin");ck(data::load_animation_tables(bytes(a),bytes(b),bytes(c),dictionary,tables,error),error);
        a=read("models/prince_modular.bdae");resources::BresView image{};ck(dh2_bres_open(&image,a.data(),a.size())==resources::BresError::ok&&scene::load(image,rest,error),error);
        a=read("data/prince-animation-bank.bin");ck(data::load_animation_bank(bytes(a),metadata,error),error);
        ck(metadata.template_clip_id==1111&&metadata.resources.size()==116&&metadata.registration_requests.size()==158,"actual Prince bank source cardinalities");
        for(const auto& resource:metadata.resources){ck(dictionary.values.at(resource.clip_id)==resource.authored_path,"actual dictionary differs");a=read(resource.asset);ck(a.size()==resource.bytes,"actual bank bytes differ");ck(bank[resource.clip_id].load(a.data(),a.size(),rest,error,animation::MissingTargets::ignore),error);}
    }
};
using Receipt=std::tuple<std::uint32_t,std::uint32_t,std::int32_t,std::int32_t,std::uint32_t,std::uint32_t,std::string>;
struct Rig {
    Catalogue& cat;actor::BlendedPlayback playback;visual::SceneBinding visual;scene::Scene scene;
    data::AnimationRandom random{0xD22026u,0};std::vector<Receipt> events;std::string error;
    explicit Rig(Catalogue& data):cat(data),scene(data.rest){
        ck(visual.bind(scene,error),error);animation::RegistrationSet registration;
        for(auto id:cat.metadata.registration_requests)ck(registration.append(id,data::animation_resource_identity(cat.metadata,id),&cat.bank.at(id),error),error);
        ck(registration.set_default(data::animation_resource_identity(cat.metadata,cat.metadata.template_clip_id),&cat.bank.at(cat.metadata.template_clip_id),error),error);registration.refresh_indices();
        ck(playback.compile_dynamic(cat.bank,registration,cat.rest,visual,error),error);
        playback.observer={this,event};visual.root.position[0]=17;visual.root.position[1]=-9;visual.root.scale[0]=visual.root.scale[1]=visual.root.scale[2]=.01f;
        const float euler[3]{0,0,.31f};ck(visual.set_rotation(euler),"source visual rotation");
    }
    static void event(void* raw,actor::BlendedPlayback&,const actor::BlendedPlaybackEvent& event){auto& r=*static_cast<Rig*>(raw);const auto& e=event.event;
        r.events.emplace_back(e.handoff.event_id,event.slot,e.clip,e.handoff.lag_ms,e.timestamp,e.phase,e.handoff.payload?e.handoff.payload:"");}
    void start(int sequence,float speed=1){ck(playback.start(cat.tables,sequence,random,cat.bank,visual,scene,speed,error),error);}
    void scene_phase(unsigned now,bool time=false){ck(time?playback.time_phase(now,cat.bank,visual,scene,error):playback.scene_phase(now,cat.bank,visual,scene,error),error);}
    void animator(float speed=1){ck(playback.animator_phase(cat.tables,random,cat.bank,visual,scene,speed,playback.completion.extra_ms,error),error);}
    void tick(unsigned now){scene_phase(now);animator();}
    unsigned count(unsigned id)const{return unsigned(std::count_if(events.begin(),events.end(),[&](const auto& e){return std::get<0>(e)==id;}));}
};
void pose_equal(const scene::Scene& a,const scene::Scene& b){
    ck(a.graph.size()==b.graph.size()&&a.instances.size()==b.instances.size(),"pose graph size");
    for(std::size_t i=0;i<a.graph.size();++i){const auto& x=a.graph[i];const auto& y=b.graph[i];
        ck(x.id==y.id&&x.parent==y.parent&&!std::memcmp(x.translation,y.translation,12)&&!std::memcmp(x.quaternion,y.quaternion,16)&&!std::memcmp(x.scale,y.scale,12)&&same(x.world,y.world),"exact retained local/world pose differs");}
    for(std::size_t i=0;i<a.instances.size();++i)ck(same(a.instances[i].world,b.instances[i].world),"instance world pose differs");
}
void equal(const Rig& a,const Rig& b){
    const auto& x=a.playback;const auto& y=b.playback;
    ck(a.events==b.events&&same(a.random,b.random)&&same(a.visual.root,b.visual.root)&&same(x.blend,y.blend)&&same(x.completion,y.completion)&&same(x.applicator_completion,y.applicator_completion)&&same(x.aggregate,y.aggregate),"continuation events/RNG/fade/root/completion differs");
    ck(x.root_timestamp==y.root_timestamp&&x.completions==y.completions&&x.restarts==y.restarts&&x.sequence_closed==y.sequence_closed&&x.displacement==y.displacement&&x.stop_requested==y.stop_requested&&x.last_event_lag==y.last_event_lag&&x.target_enabled==y.target_enabled&&x.scheduler.active()==y.scheduler.active(),"continuation playback scalar differs");
    const auto& f=x.scheduler.frames();const auto& g=y.scheduler.frames();ck(f.size()==g.size(),"scheduler depth differs");for(std::size_t i=0;i<f.size();++i)ck(same(f[i],g[i]),"source scheduler frame differs");
    ck(x.scheduler.clip().anim==y.scheduler.clip().anim&&x.scheduler.clip().blend_out==y.scheduler.clip().blend_out&&x.scheduler.clip().speed==y.scheduler.clip().speed,"source scheduler step differs");
    for(unsigned i=0;i<2;++i){const auto& p=x.slots[i];const auto& q=y.slots[i];ck(same(p.timeline,q.timeline)&&same(p.event_cursor,q.event_cursor)&&same(p.root_history,q.root_history)&&p.clip_id==q.clip_id&&p.compiled_clip==q.compiled_clip&&p.generation==q.generation&&p.key_cursors==q.key_cursors,"source slot clock/event/key/root history differs");}
    for(std::size_t i=0;i<x.transform_set().targets().size();++i)
        ck(x.values(i)==y.values(i),"cached source contribution differs");
    pose_equal(a.scene,b.scene);
}
unsigned restorations=0,continuations=0,guards=0;
void recreate(Rig& r){
    const auto* compiled=r.playback.transform_set().clip(0);const auto events=r.events;const auto random=r.random;
    // Opaque before/after bytes additionally protect private queued selection,
    // global speed, copied resource owners and contribution vector identities.
    const auto* bytes=reinterpret_cast<const unsigned char*>(&r.playback);std::vector<unsigned char> before(bytes,bytes+sizeof(r.playback));
    const auto pose=r.scene;const auto root=r.visual.root;retention::Snapshot snapshot;
    ck(snapshot.capture(r.playback,r.cat.bank,r.visual,r.scene,r.error)&&!snapshot.empty(),r.error);
    r.scene={};r.visual={}; // Destroy the prior graph/binding before rebuilding.
    r.scene=r.cat.rest;ck(snapshot.restore(r.playback,r.cat.bank,r.visual,r.scene,r.error),r.error);
    ck(!std::memcmp(before.data(),&r.playback,before.size())&&r.events==events&&same(r.random,random)&&same(r.visual.root,root)&&r.playback.transform_set().clip(0)==compiled,"restore rewrote playback/clock/RNG/events/resource identity");
    pose_equal(pose,r.scene);snapshot.clear();ck(snapshot.empty(),"snapshot clear");++restorations;
}
int direct(Catalogue& cat,int clip,int fade){data::AnimationSequence seq;seq.loop=0;seq.steps.resize(1);seq.steps[0].anim=clip;seq.steps[0].blend_out=fade;seq.steps[0].move_go=true;cat.tables.sequences.push_back(seq);return int(cat.tables.sequences.size()-1);}
int main(int argc,char** argv){try{
    ck(argc==2,"actual packaged assets required");Catalogue cat(argv[1]);const int attack=direct(cat,955,300),dead=direct(cat,1023,0);unsigned authored=0,closures=0;
    {Rig a(cat),b(cat);a.start(262);b.start(262);a.tick(1000);b.tick(1000);a.start(280);b.start(280);a.tick(1017);b.tick(1017);
        ck(a.playback.blend.weights[0]>0&&a.playback.blend.weights[1]>0,"real mid-fade boundary missing");recreate(b);equal(a,b);
        for(unsigned now=1034;now<1500;now+=17){a.tick(now);b.tick(now);equal(a,b);++continuations;}}
    {Rig a(cat),b(cat);a.start(280);b.start(280);a.tick(1000);b.tick(1000);a.tick(1037);b.tick(1037);
        // An explicit disabled local channel remains part of the real graph;
        // neither sampling nor cached fade reblending may replace these bytes.
        a.playback.target_enabled[0]=b.playback.target_enabled[0]=0;a.scene.graph[0].scale[1]=b.scene.graph[0].scale[1]=1.125f;
        ck(a.visual.update_world(a.scene,a.error)&&b.visual.update_world(b.scene,b.error),"disabled-channel world rebuild");const auto pose=a.scene;
        a.scene_phase(1074,true);b.scene_phase(1074,true);a.scene_phase(1111,true);b.scene_phase(1111,true);pose_equal(pose,a.scene);recreate(b);equal(a,b);
        for(unsigned now=1148;now<1600;now+=37){a.tick(now);b.tick(now);equal(a,b);++continuations;}}
    {Rig a(cat),b(cat);a.start(attack);b.start(attack);unsigned now=1000;
        for(;now<6000&&!a.count(0x28);now+=37){a.tick(now);b.tick(now);}ck(a.count(0x28)>0,"real attack event producer absent");recreate(b);recreate(b);equal(a,b);
        for(;now<7000;now+=37){a.tick(now);b.tick(now);equal(a,b);++continuations;}ck(a.count(0x22)==1,"real attack closure count");authored+=a.count(0x28);closures+=a.count(0x22);}
    {Rig a(cat),b(cat);a.start(dead);b.start(dead);unsigned now=1000;
        for(;now<12000&&!a.playback.sequence_closed;now+=37){a.tick(now);b.tick(now);}ck(a.playback.sequence_closed&&a.count(0x22)==1&&a.playback.current_timeline().ended,"real Died completion absent");recreate(b);recreate(b);equal(a,b);
        for(unsigned i=0;i<20;++i,now+=37){a.tick(now);b.tick(now);equal(a,b);++continuations;}ck(a.count(0x22)==1,"closed Died event replayed");closures+=a.count(0x22);}
    {Rig a(cat),b(cat);a.start(attack);b.start(attack);unsigned now=1000;
        for(;now<7000;now+=37){a.scene_phase(now);b.scene_phase(now);if(a.playback.completion.pending)break;a.animator();b.animator();}
        ck(a.playback.completion.pending,"actual pending completion source boundary absent");a.start(262,1.3f);b.start(262,1.3f);recreate(b);equal(a,b);
        a.animator(1.3f);b.animator(1.3f);equal(a,b);++continuations;
        for(now+=37;now<8000;now+=37){a.tick(now);b.tick(now);equal(a,b);++continuations;}closures+=a.count(0x22);}
    {Rig r(cat),other(cat);r.start(280);r.tick(1000);retention::Snapshot snapshot;ck(snapshot.capture(r.playback,cat.bank,r.visual,r.scene,r.error),r.error);
        auto fail=[&](scene::Scene invalid,actor::BlendedPlayback& playback,const actor::ClipBank& bank){visual::SceneBinding binding;const auto before=invalid;const auto root=binding.root;
            ck(!snapshot.restore(playback,bank,binding,invalid,r.error)&&same(binding.root,root)&&binding.animated_node()==-1,"invalid restore mutated binding");pose_equal(before,invalid);++guards;};
        auto invalid=cat.rest;invalid.graph[0].id+="changed";fail(invalid,r.playback,cat.bank);
        invalid=cat.rest;invalid.graph[1].parent=-1;fail(invalid,r.playback,cat.bank);
        invalid=cat.rest;invalid.graph.pop_back();fail(invalid,r.playback,cat.bank);
        invalid=cat.rest;invalid.graph.at(r.visual.animated_node()).name="removed_root";fail(invalid,r.playback,cat.bank);
        invalid=cat.rest;invalid.instances[0].node_index=UINT32_MAX;fail(invalid,r.playback,cat.bank);
        fail(cat.rest,other.playback,cat.bank);actor::ClipBank empty;fail(cat.rest,r.playback,empty);
        invalid=r.scene;invalid.graph[1].parent=1;ck(!snapshot.capture(r.playback,cat.bank,r.visual,invalid,r.error),"bad capture accepted");++guards;
        auto fresh=cat.rest;visual::SceneBinding fresh_visual;ck(snapshot.restore(r.playback,cat.bank,fresh_visual,fresh,r.error),r.error);pose_equal(r.scene,fresh);
        snapshot.clear();fail(cat.rest,r.playback,cat.bank);
    }
    std::cout<<"{\"validation\":\"PASS\",\"actual_bank_resources\":"<<cat.bank.size()<<",\"actual_registration_occurrences\":"<<cat.metadata.registration_requests.size()<<",\"restoration_boundaries\":"<<restorations<<",\"exact_continuation_frames\":"<<continuations<<",\"graph_owner_failure_guards\":"<<guards<<",\"real_authored_attack_events\":"<<authored<<",\"real_finite_closures\":"<<closures<<",\"native_recreation\":false,\"assets\":[";
    bool first=true;for(const auto& path:cat.inputs){if(!first)std::cout<<',';first=false;std::cout<<std::quoted(path);}std::cout<<"]}\n";return 0;
}catch(const std::exception& e){std::cerr<<"scene retention: "<<e.what()<<'\n';return 1;}}
