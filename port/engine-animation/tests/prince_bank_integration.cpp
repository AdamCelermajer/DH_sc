#include "../animation_registration.hpp"
#include "../../engine-resources/resources.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

namespace {
void require(bool v,const std::string& s){if(!v)throw std::runtime_error(s);}
std::vector<std::uint8_t> file(const std::string& path){
    std::ifstream f(path,std::ios::binary);require(bool(f),"open: "+path);
    return {std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>()};
}
struct Reader {
    std::vector<std::uint8_t> bytes;std::size_t at=0;
    std::uint32_t word(){require(at+4<=bytes.size(),"short fixture");std::uint32_t v;std::memcpy(&v,bytes.data()+at,4);at+=4;return v;}
    std::string str(){auto n=word();require(at+n<=bytes.size(),"short path");std::string s(reinterpret_cast<const char*>(bytes.data()+at),n);at+=n;return s;}
};
struct Resource{std::int32_t id,start,end;std::string path;};
struct Canary {std::uint32_t before=0x12345678;float value[4];std::uint32_t after=0xabcdef09;};
}
int main(int argc,char**argv){try{
    require(argc==3,"usage: prince_bank_integration fixture assets");
    Reader r{file(argv[1])};require(r.word()==0x314b4250,"PBK1 magic");
    auto nr=r.word(),nq=r.word(),np=r.word(),default_id=r.word();
    require(nr==116&&nq==158&&np==17&&default_id==1111,"authored bank counts");
    auto model=r.str();std::vector<Resource> resources;
    for(unsigned i=0;i<nr;++i){Resource v;v.id=static_cast<std::int32_t>(r.word());v.start=static_cast<std::int32_t>(r.word());v.end=static_cast<std::int32_t>(r.word());v.path=r.str();resources.push_back(v);}
    std::vector<std::int32_t> requests;for(unsigned i=0;i<nq;++i)requests.push_back(static_cast<std::int32_t>(r.word()));
    std::vector<std::pair<std::int32_t,std::int32_t>> projections;
    for(unsigned i=0;i<np;++i){auto id=r.word();auto index=r.word();projections.emplace_back(id,index);}
    require(r.at==r.bytes.size(),"trailing fixture");
    const std::string root=argv[2];auto model_bytes=file(root+"/"+model);
    dh2::resources::BresView view{};require(dh2_bres_open(&view,model_bytes.data(),model_bytes.size())==dh2::resources::BresError::ok,"model BRES");
    dh2::scene::Scene scene;std::string error;require(dh2::scene::load(view,scene,error),error);require(scene.graph.size()==35,"Prince nodes");
    dh2::animation::TransformSet set;std::map<std::int32_t,std::int32_t> first;
    std::uint64_t legacy_skipped=0,legacy_unbound=0,identity_checks=0;
    {
        std::map<std::int32_t,std::unique_ptr<dh2::animation::Player>> players;
        for(const auto&res:resources){auto bytes=file(root+"/"+res.path);auto player=std::make_unique<dh2::animation::Player>();
            require(player->load(bytes.data(),bytes.size(),scene,error,dh2::animation::MissingTargets::ignore),"load "+std::to_string(res.id)+": "+error);
            legacy_skipped+=player->skipped;legacy_unbound+=player->unbound;
            require(players.emplace(res.id,std::move(player)).second,"duplicate staged resource");
        }
        dh2::animation::RegistrationSet registration;
        for(unsigned i=0;i<nq;++i){auto id=requests[i];const auto*player=players.at(id).get();
            require(registration.append(id,static_cast<std::uint64_t>(id),player,error),error);first.emplace(id,i);
            const auto&occ=registration.occurrences().back();require(occ.engine_index==static_cast<std::int32_t>(i)&&occ.dictionary_id==id&&occ.resource_identity==static_cast<std::uint64_t>(id)&&occ.player==player,"occurrence identity");++identity_checks;
        }
        require(registration.set_default(default_id,players.at(default_id).get(),error),error);
        require(registration.occurrences().size()==nq&&registration.entries().size()==nr,"default separate from registration");
        registration.refresh_indices();
        for(const auto&entry:first)require(registration.lookup(entry.first)==entry.second,"first occurrence map");
        for(const auto&entry:projections)require(registration.lookup(entry.first)==entry.second,"original current17 projection");
        require(registration.lookup(-1)==-1&&registration.lookup(0x7fffffff)==-1,"missing game IDs");
        const auto inputs=registration.compiled_inputs();require(inputs.size()==nq,"compiled occurrence count");
        for(unsigned i=0;i<nq;++i)require(inputs[i].id==static_cast<std::int32_t>(i)&&inputs[i].player==players.at(requests[i]).get(),"synthetic engine ID");
        require(set.compile_dynamic(inputs,scene,error,registration.default_player(),dh2::animation::TransformMismatchBehavior::retain),"dynamic full bank: "+error);
    } // All borrowed Players/registration storage die before every sample.
    require(set.clip_count()==nq&&!set.targets().empty(),"compiled bank retained");
    std::map<std::int32_t,std::pair<std::int32_t,std::int32_t>> bounds;
    for(const auto&res:resources)bounds.emplace(res.id,std::make_pair(res.start,res.end));
    for(unsigned c=0;c<nq;++c){const auto expected=bounds.at(requests[c]);require(set.clip(c)->start==expected.first&&set.clip(c)->end==expected.second,"original compiled clip bounds "+std::to_string(requests[c]));}
    for(const auto&entry:projections)require(set.find_clip(entry.second)==entry.second&&set.clip(entry.second)->id==entry.second,"game to engine bridge");
    std::map<unsigned,unsigned> target_types;std::uint64_t samples=0,retained=0,defaults=0,accessors=0,unbound=0,finite=0,nonfinite=0;
    for(const auto&target:set.targets()){++target_types[target.type];require(target.components==3||target.components==4,"target width");if(target.node==UINT32_MAX)++unbound;else require(target.node<scene.graph.size(),"target binding");}
    for(unsigned c=0;c<nq;++c){const auto*clip=set.clip(c);require(clip&&clip->id==static_cast<std::int32_t>(c),"ordered clip IDs");
        for(std::size_t t=0;t<set.targets().size();++t){const auto*binding=set.clip_target(c,t);require(binding&&(binding->mode==1||binding->mode==2),"binding mode");
            const auto width=set.targets()[t].components;
            for(auto ms:{clip->start,clip->end}){Canary out;const std::uint32_t initial[4]={0x3f800000,0xc0000000,0x40400000,0x40a00000};std::memcpy(out.value,initial,16);std::int32_t cursor=0;
                require(set.sample(c,t,ms,out.value,width,&cursor,error),"sample "+std::to_string(c)+"/"+std::to_string(t)+": "+error);
                require(out.before==0x12345678&&out.after==0xabcdef09,"sample storage canaries");
                std::uint32_t words[4];std::memcpy(words,out.value,16);if(width==3)require(words[3]==initial[3],"float3 fourth-word canary");
                if(binding->mode==1&&!binding->has_default){require(std::memcmp(words,initial,16)==0&&cursor==0,"null binding retention");++retained;}
                else if(binding->mode==1)++defaults;else ++accessors;
                for(unsigned lane=0;lane<width;++lane){if((words[lane]&0x7f800000)==0x7f800000)++nonfinite;else ++finite;}
                ++samples;
            }
        }
    }
    require(samples==nq*set.targets().size()*2&&accessors>0,"all bounds samples");
    std::cout<<"{\"validation\":\"PASS\",\"resources\":"<<nr<<",\"registration_requests\":"<<nq<<",\"unique_game_ids\":"<<first.size()<<",\"original_current17_mappings\":"<<np<<",\"scene_nodes\":35,\"compiled_clips\":"<<set.clip_count()<<",\"ordered_targets\":"<<set.targets().size()<<",\"target_types\":{";
    bool comma=false;for(const auto&entry:target_types){if(comma)std::cout<<',';comma=true;std::cout<<'"'<<entry.first<<"\":"<<entry.second;}
    std::cout<<"},\"samples\":"<<samples<<",\"accessor_samples\":"<<accessors<<",\"default_samples\":"<<defaults<<",\"retained_samples\":"<<retained<<",\"unbound_targets\":"<<unbound<<",\"finite_output_words\":"<<finite<<",\"nonfinite_output_words\":"<<nonfinite<<",\"borrowed_identity_checks\":"<<identity_checks<<",\"legacy_player_skipped\":"<<legacy_skipped<<",\"legacy_player_unbound\":"<<legacy_unbound<<",\"default_designation_does_not_append\":true,\"sample_after_player_destruction\":true,\"sanitizer_findings\":0}\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
