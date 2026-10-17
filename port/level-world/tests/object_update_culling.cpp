#include "../object_update_culling.hpp"

#include <array>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace c = dh2::object_update_culling;
namespace {
void require(bool value, const char* reason) { if (!value) throw std::runtime_error(reason); }
struct Fixture {
    c::Object object{0x100000001ull, 0xffffffffu, 1, 0, {0, 0}};
    c::Aabb aabb{{0xbf800000u,0xbf800000u,0xbf800000u},{0x3f800000u,0x3f800000u,0x3f800000u}};
    c::Globals globals{0x200000002ull};
    c::CameraRoot root{0x300000003ull}, alternate_root{0x400000004ull};
    c::CameraVisual camera{0x500000005ull,&root};
    c::Level level{0x600000006ull,&camera};
    c::Frustum frustum{0x700000007ull,{}};
    c::Services services{this,invoke};
    std::uint32_t online=0, remote=0, level_present=1, mutation=0;
    int fail=-1, throws=-1;
    c::Level* level_override=nullptr;
    const c::Frustum* frustum_override=nullptr;
    bool null_frustum=false;
    bool nested_same=false,nested_independent=false;
    c::Status nested_status=c::Status::complete;
    std::vector<std::array<std::uint32_t,2>> calls;
    static std::int32_t invoke(void* raw,c::Object* object,const c::Request* request,c::Response* response) {
        auto& f=*static_cast<Fixture*>(raw);require(object==&f.object,"wrong source Object");
        require(request->object==f.object.identity,"truncated Object identity");
        const unsigned op=static_cast<unsigned>(request->operation);
        const unsigned subject=request->subject==0?0:request->subject==f.object.identity?1:
            request->subject==0x200000002ull?2:request->subject==f.root.identity?3:
            request->subject==f.alternate_root.identity?4:99;
        f.calls.push_back({op,subject});
        if(op==0) {
            response->raw=f.online;
            if(f.mutation&1) f.object.culling_phase_86=1;
            if(f.mutation&2) f.aabb.minimum[0]=0xc1100000u;
            if(f.mutation&256) f.services.invoke=nullptr;
            if(f.nested_same) {c::Result result{};f.nested_status=c::evaluate(&f.object,&f.aabb,&f.globals,&f.services,&result);}
            if(f.nested_independent) {Fixture nested;nested.object.identity+=8;nested.object.culling_phase_86=0;c::Result result{};
                f.nested_status=c::evaluate(&nested.object,nullptr,nullptr,&nested.services,&result);}
        } else if(op==1) {
            response->raw=f.remote;
            if(f.mutation&4) f.object.culling_phase_86=0;
            if(f.mutation&8) f.object.culling_phase_86=1;
        } else if(op==2) {
            response->level=f.level_present?(f.level_override?f.level_override:&f.level):nullptr;
            if(f.mutation&16) f.aabb.minimum[0]=0x42c80000u;
            if(f.mutation&32) f.object.culling_phase_86=77;
            if(f.mutation&64) f.camera.root_8=&f.alternate_root;
            if(f.mutation&512) f.globals.application=0x800000008ull;
        } else if(op==3) {
            response->frustum=f.null_frustum?nullptr:f.frustum_override?f.frustum_override:&f.frustum;
            if(f.mutation&128) f.camera.root_8=&f.root;
            if(f.mutation&1024) f.frustum.planes[2].distance=0x3f800000u;
        }
        if(int(op)==f.throws) throw std::runtime_error("provider error");
        return int(op)==f.fail ? 1 : 0;
    }
};
void print(const Fixture& f,c::Status status,const c::Result& out) {
    std::printf("{\"status\":%u,\"raw\":%u,\"phase\":%u,\"phase_writes\":%u,\"planes\":%u,\"last\":%u,\"aabb\":[",
        unsigned(status),out.can_update,f.object.culling_phase_86,out.phase_writes,out.planes_tested,out.last_distance_bits);
    for(unsigned i=0;i<6;++i)std::printf("%s%u",i?",":"",out.captured_aabb[i]);
    std::printf("],\"calls\":[");
    for(unsigned i=0;i<f.calls.size();++i)std::printf("%s[%u,%u]",i?",":"",f.calls[i][0],f.calls[i][1]);
    std::printf("]}\n");
}
unsigned guards() {
    unsigned cases=0;
    Fixture f;c::Result result{};
    std::memset(&result,0x5a,sizeof result);const auto saved=result;
    auto unchanged=[&](c::Status status){require(status==c::Status::invalid_argument &&
        std::memcmp(&result,&saved,sizeof result)==0 && f.calls.empty(),"preflight changed output");++cases;};
    unchanged(c::evaluate(nullptr,&f.aabb,&f.globals,&f.services,&result));
    unchanged(c::evaluate(&f.object,&f.aabb,&f.globals,nullptr,&result));
    require(c::evaluate(&f.object,&f.aabb,&f.globals,&f.services,nullptr)==c::Status::invalid_argument,"null output");++cases;
    alignas(c::Object) std::array<unsigned char,sizeof(c::Object)+1> object_bytes{};
    unchanged(c::evaluate(reinterpret_cast<c::Object*>(object_bytes.data()+1),&f.aabb,&f.globals,&f.services,&result));
    alignas(c::Services) std::array<unsigned char,sizeof(c::Services)+1> service_bytes{};
    unchanged(c::evaluate(&f.object,&f.aabb,&f.globals,reinterpret_cast<c::Services*>(service_bytes.data()+1),&result));
    alignas(c::Result) std::array<unsigned char,sizeof(c::Result)+1> out_bytes{};
    require(c::evaluate(&f.object,&f.aabb,&f.globals,&f.services,reinterpret_cast<c::Result*>(out_bytes.data()+1))==
        c::Status::invalid_argument,"misaligned output");++cases;
    unchanged(c::evaluate(&f.object,reinterpret_cast<const c::Aabb*>(&result),&f.globals,&f.services,&result));
    unchanged(c::evaluate(&f.object,&f.aabb,reinterpret_cast<const c::Globals*>(&result),&f.services,&result));
    require(c::evaluate(&f.object,&f.aabb,&f.globals,&f.services,reinterpret_cast<c::Result*>(&f.object))==
        c::Status::invalid_argument,"object/output alias");++cases;
    require(c::evaluate(&f.object,&f.aabb,&f.globals,&f.services,reinterpret_cast<c::Result*>(&f.services))==
        c::Status::invalid_argument,"service/output alias");++cases;
    {Fixture x;x.object.identity=0;unchanged(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result));}
    unchanged(c::evaluate(&f.object,reinterpret_cast<const c::Aabb*>(reinterpret_cast<std::uintptr_t>(&f.aabb)+1),
        &f.globals,&f.services,&result));
    unchanged(c::evaluate(&f.object,&f.aabb,reinterpret_cast<const c::Globals*>(reinterpret_cast<std::uintptr_t>(&f.globals)+1),
        &f.services,&result));
    {Fixture x;x.object.culling_phase_86=0;require(c::evaluate(&x.object,nullptr,nullptr,&x.services,&result)==
        c::Status::complete && result.can_update && x.object.culling_phase_86==1,"untaken null lazy fields");++cases;}
    {Fixture x;require(c::evaluate(&x.object,nullptr,&x.globals,&x.services,&result)==c::Status::invalid_source_fact &&
        x.calls.size()==1 && x.object.culling_phase_86==1,"reached null AABB");++cases;}
    {Fixture x;require(c::evaluate(&x.object,&x.aabb,nullptr,&x.services,&result)==c::Status::invalid_source_fact,"reached null globals");++cases;}
    {Fixture x;x.online=256;require(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result)==
        c::Status::invalid_source_fact && x.calls.size()==1,"wider-than-byte online value");++cases;}
    {Fixture x;x.level.camera_128=nullptr;require(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result)==
        c::Status::invalid_source_fact && x.calls.size()==2,"null camera");++cases;}
    {Fixture x;x.level_override=reinterpret_cast<c::Level*>(reinterpret_cast<std::uintptr_t>(&x.level)+1);
        require(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result)==c::Status::invalid_source_fact,"misaligned Level");++cases;}
    {Fixture x;x.level_override=reinterpret_cast<c::Level*>(&result);
        require(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result)==c::Status::invalid_source_fact,"late Level/output alias");++cases;}
    {Fixture x;x.level.camera_128=reinterpret_cast<c::CameraVisual*>(reinterpret_cast<std::uintptr_t>(&x.camera)+1);
        require(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result)==c::Status::invalid_source_fact,"misaligned camera");++cases;}
    {Fixture x;x.camera.root_8=nullptr;require(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result)==
        c::Status::invalid_source_fact,"null camera root");++cases;}
    {Fixture x;x.camera.root_8=reinterpret_cast<c::CameraRoot*>(reinterpret_cast<std::uintptr_t>(&x.root)+1);
        require(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result)==c::Status::invalid_source_fact,"misaligned camera root");++cases;}
    {Fixture x;x.camera.root_8=reinterpret_cast<c::CameraRoot*>(&result);
        require(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result)==c::Status::invalid_source_fact,"late root/output alias");++cases;}
    {Fixture x;x.frustum_override=reinterpret_cast<c::Frustum*>(reinterpret_cast<std::uintptr_t>(&x.frustum)+1);
        require(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result)==c::Status::invalid_source_fact,"misaligned frustum");++cases;}
    {Fixture x;x.frustum_override=reinterpret_cast<c::Frustum*>(&result);
        require(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result)==c::Status::invalid_source_fact,"late frustum/output alias");++cases;}
    {Fixture x;x.null_frustum=true;require(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result)==
        c::Status::invalid_source_fact,"null reached frustum");++cases;}
    {Fixture x;x.nested_same=true;require(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result)==c::Status::complete &&
        x.nested_status==c::Status::reentrant_call,"same Object reentry");++cases;}
    {Fixture x;x.nested_independent=true;require(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result)==c::Status::complete &&
        x.nested_status==c::Status::complete,"independent Object reentry");++cases;}
    {Fixture x;x.mutation=256;require(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result)==c::Status::complete &&
        result.service_calls==3,"service-table snapshot not retained");++cases;}
    {Fixture x;x.services.invoke=nullptr;require(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result)==
        c::Status::service_unavailable && !result.service_calls,"missing reached service");++cases;}
    for(int op=0;op<4;++op)for(bool throwing:{false,true}) {
        Fixture x;x.online=1;x.mutation=op>=2?32:8;if(throwing)x.throws=op;else x.fail=op;
        require(c::evaluate(&x.object,&x.aabb,&x.globals,&x.services,&result)==c::Status::service_failed &&
            result.service_calls==unsigned(op+1) && !result.phase_writes &&
            x.object.culling_phase_86==(op>=2?77:1),"provider failure lost earlier source effect");++cases;
    }
    c::RemoteResult remote{99};
    require(c::is_remotely_updated(nullptr,&remote)==c::Status::invalid_argument && remote.raw==99,"remote null");++cases;
    require(c::is_remotely_updated(&f.object,reinterpret_cast<c::RemoteResult*>(&f.object))==c::Status::invalid_argument,"remote alias");++cases;
    for(unsigned byte=0;byte<256;++byte) {
        f.object.remote_word_110=0xffffffffu;f.object.remote_byte_118=static_cast<std::uint8_t>(byte);
        require(c::is_remotely_updated(&f.object,&remote)==c::Status::complete && remote.raw==byte,"remote byte normalized");
    }
    ++cases;
    return cases;
}
}

int main(int argc,char** argv) {
    try {
        if(argc==2 && std::string(argv[1])=="--guards") {
            const auto count=guards();std::printf("{\"guard_and_failure_cases\":%u,\"mismatches\":0}\n",count);return 0;
        }
        if(argc==2 && std::string(argv[1])=="--remote") {
            std::uint32_t word,byte;while(std::cin>>word>>byte) {
                require(byte<256,"invalid remote input byte");c::Object object{0x100000001ull,word,0,static_cast<std::uint8_t>(byte),{0,0}};
                c::RemoteResult result{};require(c::is_remotely_updated(&object,&result)==c::Status::complete,"remote case failed");
                std::printf("%u\n",result.raw);
            }return 0;
        }
        require(argc==1,"unknown host mode");std::uint32_t phase;
        while(std::cin>>phase) {
            require(phase<256,"invalid input phase");Fixture f;f.object.culling_phase_86=static_cast<std::uint8_t>(phase);
            require(bool(std::cin>>f.online>>f.remote>>f.level_present>>f.mutation),"short case header");
            for(auto& v:f.aabb.minimum)require(bool(std::cin>>v),"short AABB");
            for(auto& v:f.aabb.maximum)require(bool(std::cin>>v),"short AABB");
            for(auto& plane:f.frustum.planes) {for(auto& v:plane.normal)require(bool(std::cin>>v),"short plane");
                require(bool(std::cin>>plane.distance),"short plane D");}
            c::Result out{};const auto status=c::evaluate(&f.object,&f.aabb,&f.globals,&f.services,&out);print(f,status,out);
        }
        return 0;
    } catch(const std::exception& error){std::fprintf(stderr,"%s\n",error.what());return 1;}
}
