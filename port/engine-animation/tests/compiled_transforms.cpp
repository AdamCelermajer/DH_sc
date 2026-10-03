#include "../animation.hpp"
#include <array>
#include <cstring>
#include <iostream>
#include <fstream>
#include <stdexcept>

namespace {
using namespace dh2::animation;
[[maybe_unused]] void check(bool value,const char* reason){if(!value)throw std::runtime_error(reason);}
struct Fixture {dh2::scene::Scene authored;TransformSet sets[2];};
Fixture* create(const std::uint8_t* model,std::uint32_t size,unsigned count,const std::uint8_t* const* bytes,const std::uint32_t* sizes,const std::int32_t* ids){
    auto fixture=new Fixture;dh2::resources::BresView view{};std::string error;
    if(dh2_bres_open(&view,model,size)!=dh2::resources::BresError::ok||!dh2::scene::load(view,fixture->authored,error)){delete fixture;return nullptr;}
    std::vector<Player> players(count);std::vector<TransformClipInput> inputs;
    for(unsigned i=0;i<count;++i){
        if(!players[i].load(bytes[i],sizes[i],fixture->authored,error,MissingTargets::ignore)){delete fixture;return nullptr;}
        inputs.push_back({ids[i],&players[i]});
    }
    for(unsigned policy=0;policy<2;++policy)if(!fixture->sets[policy].compile(inputs,fixture->authored,error,policy?TransformTemplatePolicy::none:TransformTemplatePolicy::authored)){delete fixture;return nullptr;}
    // Input Players (including their events and key storage) die here. Every
    // later observation therefore checks compiled resource independence.
    return fixture;
}
}
extern "C" {
void* dh2_transform_test_create(const std::uint8_t* model,std::uint32_t size,unsigned count,const std::uint8_t*const* bytes,const std::uint32_t* sizes,const std::int32_t* ids){return create(model,size,count,bytes,sizes,ids);}
void dh2_transform_test_destroy(void* f){delete static_cast<Fixture*>(f);}
unsigned dh2_transform_test_count(void* f,unsigned policy){return static_cast<Fixture*>(f)->sets[policy].targets().size();}
unsigned dh2_transform_test_target(void* f,unsigned policy,unsigned target,unsigned* info,char* uri,unsigned capacity){
    const auto& targets=static_cast<Fixture*>(f)->sets[policy].targets();if(target>=targets.size())return 0;const auto& t=targets[target];
    if(t.uri.size()+1>capacity)return 0;
    info[0]=t.type;info[1]=t.node;info[2]=t.components;std::memcpy(uri,t.uri.c_str(),t.uri.size()+1);return 1;
}
unsigned dh2_transform_test_binding(void* f,unsigned policy,unsigned clip,unsigned target,unsigned* output){
    const auto* b=static_cast<Fixture*>(f)->sets[policy].clip_target(clip,target);if(!b)return 0;
    output[0]=b->mode;output[1]=b->has_default;std::memcpy(output+2,b->default_value,16);return 1;
}
unsigned dh2_transform_test_sample(void* f,unsigned policy,unsigned clip,unsigned target,std::int32_t ms,float* output,unsigned capacity,std::int32_t* cursor,unsigned interpolate){
    std::string error;return static_cast<Fixture*>(f)->sets[policy].sample(clip,target,ms,output,capacity,cursor,error,interpolate!=0);
}
}

#ifndef DH2_TRANSFORM_ORACLE
namespace {
struct Reader {
    std::vector<char> bytes;std::size_t offset=0;
    explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f),"Missing original corpus");bytes.assign(std::istreambuf_iterator<char>(f),{});}
    unsigned word(){check(offset+4<=bytes.size(),"Truncated corpus");unsigned w;std::memcpy(&w,bytes.data()+offset,4);offset+=4;return w;}
    std::vector<std::uint8_t> block(){const auto n=word();check(n<=bytes.size()-offset,"Truncated corpus block");std::vector<std::uint8_t> out(bytes.begin()+offset,bytes.begin()+offset+n);offset+=n;return out;}
};
std::vector<std::array<unsigned,3>> trig;unsigned trig_next=0,trig_calls=0;
float dependency(unsigned kind,float input){unsigned bits;std::memcpy(&bits,&input,4);check(trig_next<trig.size(),"Unexpected host libm call");const auto& call=trig[trig_next++];check(call[0]==kind&&call[1]==bits,"Original libm order/input mismatch");float out;std::memcpy(&out,&call[2],4);++trig_calls;return out;}
}
extern "C" float __wrap_sinf(float input){return dependency(0,input);}
extern "C" float __wrap_acosf(float input){return dependency(1,input);}
extern "C" float __wrap_sqrtf(float input){return dependency(2,input);}
int main(int argc,char** argv){try{
    check(argc==2,"Usage: compiled-transforms-audit original-corpus.bin");Reader reader(argv[1]);check(reader.word()==0x31535443,"Wrong CTS1 magic");
    const auto model=reader.block();const auto count=reader.word();std::vector<std::vector<std::uint8_t>> clips;std::vector<const std::uint8_t*> pointers;std::vector<unsigned> sizes;std::vector<std::int32_t> ids;
    for(unsigned i=0;i<count;++i){ids.push_back(static_cast<std::int32_t>(reader.word()));clips.push_back(reader.block());}
    for(const auto& clip:clips){pointers.push_back(clip.data());sizes.push_back(clip.size());}
    // Scene construction's authored identity quaternions do not call libm.
    auto* fixture=create(model.data(),model.size(),count,pointers.data(),sizes.data(),ids.data());check(fixture,"Compiled set creation rejected");
    unsigned target_count=0,binding_count=0;
    for(unsigned policy=0;policy<2;++policy){
        auto& set=fixture->sets[policy];const auto n=reader.word();check(set.targets().size()==n,"Original ordered target count differs");target_count+=n;
        for(unsigned i=0;i<n;++i){const auto uri=reader.block();const auto type=reader.word(),node=reader.word(),components=reader.word();const auto& t=set.targets()[i];
            check(t.uri==std::string(uri.begin(),uri.end())&&t.type==type&&t.node==node&&t.components==components,"Original ordered target identity differs");
        }
        for(unsigned ci=0;ci<count;++ci){check(set.find_clip(ids[ci])==int(ci)&&set.clip(ci)->id==ids[ci],"Clip ID mapping differs");
            for(unsigned ti=0;ti<n;++ti){const auto* b=set.clip_target(ci,ti);check(b&&b->mode==reader.word()&&b->has_default==bool(reader.word()),"Original mode/default binding differs");
                unsigned expected[4];for(auto& w:expected)w=reader.word();check(!std::memcmp(expected,b->default_value,16),"Original default bytes differ");++binding_count;
            }
        }
    }
    const auto records=reader.word();unsigned retained=0;
    for(unsigned record=0;record<records;++record){
        const auto policy=reader.word(),ci=reader.word(),ti=reader.word();const auto ms=static_cast<std::int32_t>(reader.word());const auto interpolate=reader.word();std::int32_t cursor=static_cast<std::int32_t>(reader.word());
        unsigned initial[4],expected[4];for(auto& w:initial)w=reader.word();const auto expected_cursor=static_cast<std::int32_t>(reader.word());for(auto& w:expected)w=reader.word();
        trig.clear();trig_next=0;const auto calls=reader.word();for(unsigned i=0;i<calls;++i)trig.push_back({reader.word(),reader.word(),reader.word()});
        unsigned output[6];output[0]=output[5]=0xaabbccdd;std::memcpy(output+1,initial,16);
        check(dh2_transform_test_sample(fixture,policy,ci,ti,ms,reinterpret_cast<float*>(output+1),4,&cursor,interpolate),"Native raw sample rejected");
        check(cursor==expected_cursor&&!std::memcmp(expected,output+1,16),"Original raw sample differs");check(output[0]==0xaabbccdd&&output[5]==0xaabbccdd,"Raw sample guard changed");
        check(trig_next==trig.size(),"Original libm calls not consumed");retained+=!std::memcmp(initial,expected,16);
    }
    check(reader.offset==reader.bytes.size(),"Unexpected corpus tail");
    auto& set=fixture->sets[0];std::string error;float out[4]{1,2,3,4};std::int32_t cursor=17;const float before[4]{1,2,3,4};unsigned rejected=0;
    float compile_before[4]{1,2,3,4};check(set.sample(6,0,0,compile_before,4,nullptr,error),"Atomic compile baseline rejected");
    for(unsigned i=0;i<6;++i){
        bool accepted=i==0?set.sample(count,0,10,out,4,&cursor,error):i==1?set.sample(0,set.targets().size(),10,out,4,&cursor,error):i==2?set.sample(0,0,10,nullptr,4,&cursor,error):i==3?set.sample(0,0,10,out,0,&cursor,error):i==4?set.sample(0,0,10,out,4,reinterpret_cast<std::int32_t*>(out),error):set.sample(0,0,10,reinterpret_cast<float*>(reinterpret_cast<char*>(out)+1),4,&cursor,error);
        check(!accepted&&!error.empty()&&!std::memcmp(before,out,16)&&cursor==17,"Malformed sample was not atomic");++rejected;
    }
    auto malformed=clips[0];auto word=[&](unsigned offset){unsigned value;std::memcpy(&value,malformed.data()+offset,4);return value;};const auto root=word(32),record=word(root+40),channel=word(record+16);const unsigned component=2;std::memcpy(malformed.data()+channel+8,&component,4);
    Player unsupported;check(unsupported.load(malformed.data(),malformed.size(),fixture->authored,error,MissingTargets::ignore)&&unsupported.skipped,"Unsupported channel fixture invalid");const auto old_count=set.targets().size();
    check(!set.compile({{999,&unsupported}},fixture->authored,error)&&set.targets().size()==old_count,"Unsupported compile was not atomic");++rejected;
    check(!set.compile({{1,nullptr}},fixture->authored,error)&&set.targets().size()==old_count,"Null compile was not atomic");++rejected;
    float compile_after[4]{1,2,3,4};check(set.sample(6,0,0,compile_after,4,nullptr,error)&&!std::memcmp(compile_before,compile_after,16),"Failed compile changed prior sample bytes");
    // Set-owned events survive destruction of every input Player; old batches
    // retain their names while another independent set is moved.
    unsigned event_names=0;
    auto inspect_events=[&](const dh2::animation::EventView& view){check(dh2_events_validate(&view),"Independent compiled events rejected");for(unsigned i=0;i<view.count;++i)for(unsigned j=0;j<view.groups[i].count;++j){check(std::strlen(view.groups[i].names[j])>0,"Compiled event name lease expired");++event_names;}};
    for(unsigned ci=0;ci<count;++ci)inspect_events(set.clip(ci)->events.view());
    const auto event_view=fixture->sets[1].clip(2)->events.view();auto moved=std::move(fixture->sets[1]);check(moved.targets().size()&&event_view.count==moved.clip(2)->events.view().count,"Resource lease changed after move");inspect_events(event_view);check(event_names>0,"No authored event names audited");
    delete fixture;
    std::cout<<"{\"validation\":\"PASS\",\"original_raw_samples\":"<<records<<",\"ordered_targets\":"<<target_count<<",\"mode_default_bindings\":"<<binding_count<<",\"retained_samples\":"<<retained<<",\"libm_fixture_calls\":"<<trig_calls<<",\"atomic_rejections\":"<<rejected<<",\"retained_event_names\":"<<event_names<<",\"input_player_independence\":true,\"sanitizer_findings\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
#endif
