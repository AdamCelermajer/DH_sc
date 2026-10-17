#include "../character_culling_runtime.hpp"

#include <array>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace r = dh2::character_culling_runtime;
namespace e = dh2::character_update_eligibility;
namespace c = dh2::object_update_culling;
namespace {
void require(bool test, const char* reason) {
    if (!test) throw std::runtime_error(reason);
}
struct Fixture {
    static constexpr std::uintptr_t actor = 0x100000001ull;
    e::SceneNode node{0x200000002ull, 1, 9, {}}, alternate{0x300000003ull, 1, 9, {}};
    e::Visual visual{0x400000004ull, &node};
    e::Visual alternate_visual{0xa0000000aull,&alternate};
    e::Character character{actor, &visual, 0x7001, 1, 0, 0, 0};
    c::Object object{actor, 0xffffffffu, 1, 0, {}};
    c::Aabb box{{0xbf800000,0xbf800000,0xbf800000},{0x3f800000,0x3f800000,0x3f800000}};
    c::Globals globals{0x500000005ull};
    c::CameraRoot root{0x600000006ull};
    c::CameraVisual camera{0x700000007ull, &root};
    c::Level level{0x800000008ull, &camera};
    c::Frustum frustum{0x900000009ull, {}};
    e::Services character_services{this, query_character};
    c::Services camera_services{this, query_camera};
    r::Bindings bindings{&character,&object,&box,&globals,&character_services,&camera_services};
    unsigned online=0, local=0x7002, dead=0, respawn=0, level_present=1, mutations=0;
    unsigned online_calls=0;
    int failing=-1;
    bool throwing=false, recurse=false;
    r::Result* recursive_output=nullptr;
    const r::Bindings* recursive_bindings=nullptr;
    r::Result* alias_output=nullptr;
    unsigned alias_kind=0;
    r::Status nested=r::Status::complete;
    std::vector<unsigned> calls;
    static int query_character(void* raw, const e::Character* character,
                               const e::Request* request, e::Response* response) {
        auto& f=*static_cast<Fixture*>(raw);
        require(character==&f.character && request->character==f.character.identity,"wrong Character identity");
        const auto op=static_cast<unsigned>(request->operation);
        require(op!=3,"culling escaped source composition");
        f.calls.push_back(op);
        if (op==0) {
            ++f.online_calls;
            if (f.mutations&1) f.object.culling_phase_86=1;
            if (f.mutations&16) {f.character_services.invoke=nullptr;f.camera_services.invoke=nullptr;}
            if (f.recurse) {
                f.recurse=false;r::Result nested{};
                f.nested=r::evaluate(f.recursive_bindings?f.recursive_bindings:&f.bindings,
                    f.recursive_output?f.recursive_output:&nested);
            }
            response->raw=f.online;
            if(f.alias_output && f.alias_kind==6)
                f.character.visual_2d8=reinterpret_cast<e::Visual*>(&f.alias_output->culling);
        } else if (op==1) {
            c::RemoteResult remote{};
            require(c::is_remotely_updated(&f.object,&remote)==c::Status::complete,"source remote leaf");
            response->raw=remote.raw;
        } else if (op==2) response->identity=f.local;
        else if (op==4) response->raw=f.dead;
        else if (op==5) response->raw=f.respawn;
        if (int(op)==f.failing) {
            if(f.throwing) throw std::runtime_error("provider failed after effects");
            return 1;
        }
        return 0;
    }
    static int query_camera(void* raw, c::Object* object, const c::Request* request,
                            c::Response* response) {
        auto& f=*static_cast<Fixture*>(raw);
        require(object==&f.object && request->object==f.object.identity,"wrong Object identity");
        const unsigned op=static_cast<unsigned>(request->operation)+4;
        require(op==6 || op==7,"online/remote camera provider was used");
        f.calls.push_back(op);
        if(op==6) {
            require(request->subject==f.globals.application,"wrong Application identity");
            if(f.mutations&2) f.box.minimum[0]=0x42c80000;
            if(f.mutations&4) f.object.culling_phase_86=77;
            response->level=f.level_present?&f.level:nullptr;
            if(f.alias_output && f.alias_kind==2)
                response->level=reinterpret_cast<c::Level*>(&f.alias_output->character);
            if(f.alias_output && f.alias_kind==3)
                f.level.camera_128=reinterpret_cast<c::CameraVisual*>(&f.alias_output->character);
            if(f.alias_output && f.alias_kind==4)
                f.camera.root_8=reinterpret_cast<c::CameraRoot*>(&f.alias_output->character);
        } else {
            require(request->subject==f.root.identity,"wrong camera root identity");
            if(f.mutations&8) f.visual.root=&f.alternate;
            response->frustum=&f.frustum;
            if(f.alias_output && f.alias_kind==1)
                f.visual.root=reinterpret_cast<e::SceneNode*>(&f.alias_output->culling);
            if(f.alias_output && f.alias_kind==5)
                response->frustum=reinterpret_cast<const c::Frustum*>(&f.alias_output->character);
            if(f.alias_output && f.alias_kind==7) {
                f.character.visual_2d8=&f.alternate_visual;
                f.visual.root=reinterpret_cast<e::SceneNode*>(&f.alias_output->culling);
            }
            if(f.alias_kind==8) f.character.visual_2d8=&f.alternate_visual;
        }
        if(int(op)==f.failing) {
            if(f.throwing) throw std::runtime_error("camera failed after effects");
            return 1;
        }
        return 0;
    }
};
void guards() {
    unsigned count=0;
    auto check=[&](bool condition,const char* reason){require(condition,reason);++count;};
    {
        Fixture f;r::Result result{};
        f.bindings.object=nullptr;
        check(r::evaluate(&f.bindings,&result)==r::Status::culling_failed,"missing reached Object");
        check(f.node.update_flag_200==0 && result.culling_calls==1,"failure prefix was lost");
        f.local=0x7001;
        check(r::evaluate(&f.bindings,&result)==r::Status::complete && !result.culling_calls,"unused Object was required");
    }
    {
        Fixture f;r::Result result{};f.object.identity+=1;
        check(r::evaluate(&f.bindings,&result)==r::Status::culling_failed,"foreign Object accepted");
        check(f.object.culling_phase_86==1,"foreign Object changed");
    }
    for(int failing:{0,2,4,6,7}) for(bool throwing:{false,true}) {
        Fixture f;r::Result result{};f.failing=failing;f.throwing=throwing;
        f.mutations=4|8;
        const auto status=r::evaluate(&f.bindings,&result);
        check(status==(failing>=6?r::Status::culling_failed:r::Status::character_failed),"provider failure was hidden");
        check(f.node.update_flag_200==0,"scene prefix failure was rolled back");
        if(failing==6) check(f.object.culling_phase_86==77,"phase callback failure was rolled back");
        if(failing==7) check(f.visual.root==&f.alternate,"root callback failure was rolled back");
    }
    {
        Fixture f;r::Result result{};f.mutations=16;
        check(r::evaluate(&f.bindings,&result)==r::Status::complete,"captured service tables were replaced");
    }
    {
        Fixture f;r::Result result{};f.recurse=true;
        check(r::evaluate(&f.bindings,&result)==r::Status::complete && f.nested==r::Status::character_failed,"same Character recursive call accepted");
    }
    for(bool independent_actor:{false,true}) {
        Fixture f,other;r::Result result{};f.recurse=true;f.recursive_output=&result;
        other.character.identity+=1;other.object.identity=other.character.identity;
        if(independent_actor) f.recursive_bindings=&other.bindings;
        check(r::evaluate(&f.bindings,&result)==r::Status::complete &&
              f.nested==r::Status::invalid_argument,"active output reused by nested call");
        check(result.character.service_calls==4 && result.culling_calls==1 &&
              result.culling.planes_tested==6 && f.node.update_flag_200==1 &&
              other.node.update_flag_200==9,"active output rejection lost source prefix");
    }
    {
        Fixture f,other;r::Result result{},nested{};f.recurse=true;
        other.character.identity+=1;other.object.identity=other.character.identity;
        f.recursive_bindings=&other.bindings;f.recursive_output=&nested;
        check(r::evaluate(&f.bindings,&result)==r::Status::complete &&
              f.nested==r::Status::complete,"independent nested output rejected");
        check(result.culling.planes_tested==6 && nested.culling.planes_tested==6 &&
              f.node.update_flag_200==1 && other.node.update_flag_200==1,
              "independent nested source effects lost");
    }
    {
        Fixture f;r::Result result{};f.bindings.camera_services=nullptr;f.object.culling_phase_86=0;
        check(r::evaluate(&f.bindings,&result)==r::Status::complete,"unused camera table required");
        f.object.culling_phase_86=1;
        check(r::evaluate(&f.bindings,&result)==r::Status::culling_failed,"missing camera silently replaced");
    }
    {
        Fixture f;r::Result result{};f.alias_output=&result;f.alias_kind=1;
        check(r::evaluate(&f.bindings,&result)==r::Status::character_failed,"fresh root aliases sibling output");
        check(result.culling.planes_tested==6 && f.node.update_flag_200==0,"alias guard must preserve completed culling effects");
    }
    {
        Fixture f;r::Result result{};f.alias_output=&result;f.alias_kind=7;
        check(r::evaluate(&f.bindings,&result)==r::Status::character_failed,
              "replaced Visual hid captured root/output alias");
        check(result.culling.planes_tested==6 && f.object.culling_phase_86==2 &&
              f.node.update_flag_200==0 && f.alternate.update_flag_200==9,
              "captured graph guard changed completed culling effects");
    }
    {
        Fixture f;r::Result result{};f.alias_kind=8;
        check(r::evaluate(&f.bindings,&result)==r::Status::complete,
              "valid current Visual replacement rejected");
        check(f.node.update_flag_200==1 && f.alternate.update_flag_200==9 &&
              result.character.captured_visual==f.visual.identity,
              "final source mark must use captured Visual");
    }
    for(unsigned kind:{2,3,4,5,6}) {
        Fixture f;r::Result result{};f.alias_output=&result;f.alias_kind=kind;
        check(r::evaluate(&f.bindings,&result)==(kind==6?r::Status::character_failed:r::Status::culling_failed),
              "fresh projection aliases whole output");
        check(f.node.update_flag_200==0 && f.object.culling_phase_86==1,"late alias failure retains preceding effects");
    }
    {
        Fixture f;r::Result result{};result.culling.planes_tested=33;
        f.visual.root=reinterpret_cast<e::SceneNode*>(&result.culling);
        check(r::evaluate(&f.bindings,&result)==r::Status::invalid_argument &&
              result.culling.planes_tested==33,"initial root/output alias before clear");
        f.visual.root=&f.node;f.bindings.object=reinterpret_cast<c::Object*>(&f.character);
        check(r::evaluate(&f.bindings,&result)==r::Status::invalid_argument &&
              f.node.update_flag_200==9,"Object/Character alias before prefix");
        f.bindings.object=reinterpret_cast<c::Object*>(&f.character_services);
        check(r::evaluate(&f.bindings,&result)==r::Status::invalid_argument,"Object/character-table alias");
        f.bindings.object=reinterpret_cast<c::Object*>(&f.camera_services);
        check(r::evaluate(&f.bindings,&result)==r::Status::invalid_argument,"Object/camera-table alias");
        f.bindings.object=&f.object;f.character.visual_2d8=reinterpret_cast<e::Visual*>(&f.object);
        check(r::evaluate(&f.bindings,&result)==r::Status::invalid_argument &&
              f.node.update_flag_200==9,"Visual/Object alias before source prefix");
        f.character.visual_2d8=&f.visual;f.visual.root=reinterpret_cast<e::SceneNode*>(&f.box);
        check(r::evaluate(&f.bindings,&result)==r::Status::invalid_argument &&
              f.box.minimum[0]==0xbf800000,"SceneNode/AABB alias before source prefix");
    }
    {
        Fixture f;r::Result result{};f.bindings.object=reinterpret_cast<c::Object*>(&result);
        check(r::evaluate(&f.bindings,&result)==r::Status::invalid_argument,"result/Object alias accepted");
        f.bindings.object=&f.object;
        check(r::evaluate(&f.bindings,reinterpret_cast<r::Result*>(&f.bindings))==r::Status::invalid_argument,"result/bindings alias accepted");
    }
    std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<count<<"}\n";
}
}
int main(int argc,char** argv) {
    try {
        if(argc==2 && std::string(argv[1])=="--guards") {guards();return 0;}
        unsigned phase,online,remote_word,remote_byte,local,dead,respawn,visible,node_cull,gate,level,mutation;
        while(std::cin>>phase>>online>>remote_word>>remote_byte>>local>>dead>>respawn>>visible>>node_cull>>gate>>level>>mutation) {
            Fixture f;f.object.culling_phase_86=static_cast<std::uint8_t>(phase);
            f.online=online;f.object.remote_word_110=remote_word;f.object.remote_byte_118=static_cast<std::uint8_t>(remote_byte);
            f.local=local;f.dead=dead;f.respawn=respawn;f.character.current_visibility_80=static_cast<std::uint8_t>(visible);
            f.node.culling_word_118=node_cull;f.character.culling_gate_1480=static_cast<std::uint8_t>(gate);
            f.level_present=level;f.mutations=mutation;
            for(auto& value:f.box.minimum) std::cin>>value;
            for(auto& value:f.box.maximum) std::cin>>value;
            for(auto& plane:f.frustum.planes) {for(auto& value:plane.normal) std::cin>>value;std::cin>>plane.distance;}
            r::Result result{};
            require(r::evaluate(&f.bindings,&result)==r::Status::complete,"composition failed");
            std::cout<<"{\"can_update\":"<<result.character.can_update<<",\"phase\":"<<unsigned(f.object.culling_phase_86)
                <<",\"node_flag\":"<<unsigned(f.node.update_flag_200)<<",\"alternate_flag\":"<<unsigned(f.alternate.update_flag_200)<<",\"calls\":[";
            for(unsigned i=0;i<f.calls.size();++i) std::cout<<(i?",":"")<<f.calls[i];
            std::cout<<"]}\n";
        }
        return 0;
    } catch(const std::exception& error) {std::cerr<<error.what()<<'\n';return 1;}
}
