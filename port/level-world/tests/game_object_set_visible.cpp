#include "../game_object_set_visible.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace s=dh2::game_object_set_visible;
constexpr s::Address OBJ=0x02110000, VA=0x02220000, VB=0x02230000;
constexpr s::Address NA=0x02310000, NB=0x02320000, MA=0x02410000, MB=0x02420000;
constexpr s::Address SA=0x02510000, SB=0x02510010;
struct Fixture;
struct Context { Fixture* fixture; };
struct Fixture {
    s::SceneNode a{NA,SA,0}, b{NB,SB,0x12340000};
    s::VisualObject va{VA,&a},vb{VB,&b};
    s::GameObject game{OBJ,&va,77,1};
    s::Device device{MA},other_device{MB};
    s::Application application{&device},other_application{&other_device};
    s::Globals globals{&application};
    Context context{this};
    s::Services services{&context,sizeof(context),dispatch};
    std::vector<s::Request> trace;
    std::uint32_t mutation=0,owner_word=0;
    int fail_op=-1;
    bool throw_error=false,null_reload=false,invalid_reload=false,copy_table=false;
    int nesting=0;
    bool in_nested=false;
    s::Result* outer_result=nullptr;
    s::Status nested_status=s::Status::complete;
    s::Result nested_result{};
    static std::int32_t dispatch(void* p,const s::Request* request) {
        auto& f=*static_cast<Context*>(p)->fixture;
        const auto r=*request; f.trace.push_back(r);
        if (r.operation==s::Operation::sync_visibility) {
            if (f.mutation&1) f.game.visual_2d8=&f.vb;
            if (f.mutation&2) f.game.enabled_8a=97;
            if (f.mutation&4) f.game.visibility_80=201;
            if (f.nesting && !f.in_nested) {
                f.in_nested=true;
                auto* output=f.nesting==2?f.outer_result:&f.nested_result;
                f.nested_status=s::set_game_object(&f.game,0,&f.services,output);
                f.in_nested=false;
            }
        } else if (r.operation==s::Operation::force_register) {
            if (f.mutation&1) f.va.root_8=&f.b;
            if (f.mutation&2) f.a.set_visible_target_48=SB;
            if (f.mutation&4) f.a.flags_11c^=1;
            if (f.mutation&8) f.globals.application=&f.other_application;
            if (f.mutation&256) f.owner_word=1;
            if (f.null_reload) f.va.root_8=nullptr;
            if (f.invalid_reload) f.va.root_8=reinterpret_cast<s::SceneNode*>(f.outer_result);
            if (f.copy_table) f.services.invoke=nullptr;
        } else {
            auto* node=r.object==NA?&f.a:r.object==NB?&f.b:nullptr;
            if (!node || (r.target!=SA && r.target!=SB)) throw std::runtime_error("bad node dispatch");
            // Named fixture method effect; the original virtual body is external.
            node->flags_11c=(node->flags_11c&~1u)|(r.value?1u:0u);
            if (f.mutation&16) f.va.root_8=&f.b;
            if (f.mutation&32) { f.game.visibility_80=211; f.game.enabled_8a=71; }
        }
        if (static_cast<int>(r.operation)==f.fail_op) {
            if (f.throw_error) throw std::runtime_error("provider error");
            return -17;
        }
        return 0;
    }
};
std::size_t checks=0;
void check(bool value,const char* what) { ++checks; if (!value) throw std::runtime_error(what); }
bool zero_trace(const Fixture& f) {return f.trace.empty();}
void guards() {
    {
        Fixture f; s::Result out{}; out.service_calls=99;
        check(s::set_game_object(nullptr,1,&f.services,&out)==s::Status::invalid_argument &&
              out.service_calls==99 && zero_trace(f),"null primary unchanged");
        check(s::set_game_object(&f.game,1,&f.services,nullptr)==s::Status::invalid_argument,"null output");
        alignas(std::max_align_t) std::array<std::uint8_t,256> raw{};
        check(s::set_game_object(reinterpret_cast<s::GameObject*>(raw.data()+1),1,&f.services,&out)==
              s::Status::invalid_argument,"primary alignment");
        check(s::set_game_object(&f.game,1,&f.services,reinterpret_cast<s::Result*>(raw.data()+1))==
              s::Status::invalid_argument,"output alignment");
        check(s::set_game_object(&f.game,1,reinterpret_cast<s::Services*>(raw.data()+1),&out)==
              s::Status::invalid_argument,"services alignment");
        check(s::set_game_object(&f.game,1,&f.services,reinterpret_cast<s::Result*>(&f.game))==
              s::Status::invalid_argument,"output/primary alias");
        check(s::set_game_object(&f.game,1,reinterpret_cast<s::Services*>(&out),&out)==
              s::Status::invalid_argument,"output/services alias before read");
        const auto* near=reinterpret_cast<s::Services*>(std::numeric_limits<s::Address>::max()-7);
        check(s::set_game_object(&f.game,1,near,&out)==s::Status::invalid_argument,"table address wrap");
        auto services=f.services;services.context=&out;services.context_extent=sizeof(out);
        check(s::set_game_object(&f.game,1,&services,&out)==s::Status::invalid_argument,"output/context alias");
        services=f.services;services.context=nullptr;services.context_extent=8;
        check(s::set_game_object(&f.game,1,&services,&out)==s::Status::invalid_argument,"invalid described context");
    }
    {
        Fixture f;s::Result out{};
        f.game.visual_2d8=nullptr;f.game.enabled_8a=255;
        check(s::set_game_object(&f.game,1,nullptr,&out)==s::Status::complete &&
              f.game.visibility_80==255 && out.written_visibility==255 && out.source_writes==1,"no visual needs no service");
        f.game.visual_2d8=&f.va;
        check(s::set_game_object(&f.game,0,nullptr,&out)==s::Status::service_unavailable &&
              f.game.visibility_80==0 && out.source_writes==1,"missing sync preserves store");
        f.game.visual_2d8=reinterpret_cast<s::VisualObject*>(reinterpret_cast<std::uint8_t*>(&f.va)+1);
        check(s::set_game_object(&f.game,1,&f.services,&out)==s::Status::invalid_source_fact &&
              f.game.visibility_80==255 && out.source_writes==1,"late visual alignment preserves store");
        f.game.visual_2d8=reinterpret_cast<s::VisualObject*>(&out);
        check(s::set_game_object(&f.game,0,&f.services,&out)==s::Status::invalid_source_fact &&
              f.game.visibility_80==0,"late visual/output alias");
        f.game.visual_2d8=&f.va;f.va.identity=0;
        check(s::set_game_object(&f.game,1,&f.services,&out)==s::Status::invalid_source_fact &&
              f.game.visibility_80==255,"late visual identity");
    }
    {
        Fixture f;s::Result out{};
        f.va.root_8=nullptr;
        check(s::set_visual_object(&f.va,0xffffffffu,nullptr,nullptr,&out)==s::Status::complete && zero_trace(f),"null root skips dependencies");
        f.va.root_8=&f.a;
        check(s::set_visual_object(&f.va,0,nullptr,&f.services,&out)==s::Status::complete &&
              f.trace.size()==1 && f.trace[0].operation==s::Operation::node_set_visible,"equal still calls setter without globals");
        f.trace.clear();
        check(s::set_visual_object(&f.va,1,nullptr,&f.services,&out)==s::Status::invalid_source_fact && zero_trace(f),"reached missing globals");
        auto globals=f.globals; globals.application=nullptr;
        check(s::set_visual_object(&f.va,1,&globals,&f.services,&out)==s::Status::invalid_source_fact,"reached null app");
        auto application=f.application;application.device_10=nullptr;globals.application=&application;
        check(s::set_visual_object(&f.va,1,&globals,&f.services,&out)==s::Status::invalid_source_fact,"reached null device");
        globals.application=reinterpret_cast<s::Application*>(&globals);
        check(s::set_visual_object(&f.va,1,&globals,&f.services,&out)==s::Status::invalid_source_fact,"same-size cross-type alias");
        check(s::set_visual_object(&f.va,1,reinterpret_cast<s::Globals*>(&out),&f.services,&out)==
              s::Status::invalid_source_fact,"globals/output alias before read");
        f.a.set_visible_target_48=0;
        check(s::set_visual_object(&f.va,0,nullptr,&f.services,&out)==s::Status::invalid_source_fact,"missing virtual target");
        f.a.set_visible_target_48=SA;
        check(s::set_visual_object(&f.va,0,nullptr,nullptr,&out)==s::Status::service_unavailable,"mandatory virtual service");
    }
    for (bool throwing:{false,true}) for (int op:{0,1,2}) {
        Fixture f;s::Result out{};f.fail_op=op;f.throw_error=throwing;f.mutation=op==0?4:op==1?1:0;
        const auto status=op==0?s::set_game_object(&f.game,1,&f.services,&out):
            s::set_visual_object(&f.va,1,&f.globals,&f.services,&out);
        check(status==s::Status::service_failed,"provider failure caught");
        check(out.service_calls==static_cast<unsigned>(op==2?2:1),"provider failure order");
        check(op==0?f.game.visibility_80==201:op==1?f.va.root_8==&f.b:f.a.flags_11c==1,"provider partial effects retained");
    }
    {
        Fixture f;s::Result out{};f.outer_result=&out;f.null_reload=true;
        check(s::set_visual_object(&f.va,1,&f.globals,&f.services,&out)==s::Status::invalid_source_fact &&
              f.va.root_8==nullptr && out.service_calls==1,"fresh null root after force retains effect");
        f.va.root_8=&f.a;f.null_reload=false;f.invalid_reload=true;
        check(s::set_visual_object(&f.va,1,&f.globals,&f.services,&out)==s::Status::invalid_source_fact &&
              out.service_calls==1,"fresh root/result alias");
    }
    {
        Fixture f;s::Result out{};f.copy_table=true;f.mutation=1|2;
        check(s::set_visual_object(&f.va,255,&f.globals,&f.services,&out)==s::Status::complete &&
              f.trace.size()==2 && f.trace[1].object==NB && f.trace[1].target==SB && f.trace[1].value==255 &&
              f.services.invoke==nullptr,"fresh root and captured services");
    }
    for (int nesting:{1,2}) {
        Fixture f;s::Result out{};f.outer_result=&out;f.nesting=nesting;
        check(s::set_game_object(&f.game,1,&f.services,&out)==s::Status::complete,"outer nested call completes");
        check(f.nested_status==(nesting==1?s::Status::complete:s::Status::invalid_argument),"nested output contract");
        check(f.game.visibility_80==static_cast<unsigned>(nesting==1?0:1) &&
              out.service_calls==1 && out.source_writes==1 && f.trace.size()==static_cast<unsigned>(nesting==1?2:1),
              "same owner nested writes preserve source effects");
    }
    if (sizeof(s::Address)>4) {
        Fixture f;s::Result out{};f.va.identity=0x100000000ull+VA;f.game.visual_2d8=&f.va;
        check(s::set_game_object(&f.game,1,&f.services,&out)==s::Status::complete &&
              f.trace[0].object==0x100000000ull+VA,"full-width identity");
    }
    std::cout<<"{\"status\":\"PASS\",\"guard_checks\":"<<checks<<",\"mismatches\":0}\n";
}
void cases(const char* path) {
    std::ifstream in(path);if(!in)throw std::runtime_error("fixture open");
    std::uint32_t kind,arg,enabled,visible,visual,root,flags,mutation,owner;
    while(in>>kind>>arg>>enabled>>visible>>visual>>root>>flags>>mutation>>owner) {
        Fixture f;f.game.enabled_8a=static_cast<std::uint8_t>(enabled);f.game.visibility_80=static_cast<std::uint8_t>(visible);
        f.game.visual_2d8=visual?&f.va:nullptr;f.va.root_8=root?&f.a:nullptr;
        f.a.flags_11c=flags;f.mutation=mutation;f.owner_word=owner;s::Result out{};
        auto status=kind==0?s::set_game_object(&f.game,arg,&f.services,&out):s::set_visual_object(&f.va,arg,&f.globals,&f.services,&out);
        std::cout<<"{\"status\":"<<static_cast<int>(status)<<",\"writes\":"<<out.source_writes<<",\"calls\":"<<out.service_calls
            <<",\"state\":["<<static_cast<unsigned>(f.game.visibility_80)<<','<<static_cast<unsigned>(f.game.enabled_8a)<<','
            <<(f.game.visual_2d8?f.game.visual_2d8->identity:0)<<','<<(f.va.root_8?f.va.root_8->identity:0)<<','
            <<f.a.flags_11c<<','<<f.b.flags_11c<<','<<f.globals.application->device_10->scene_manager_1c<<','<<f.owner_word<<"],\"trace\":[";
        for(std::size_t i=0;i<f.trace.size();++i) {
            if(i)std::cout<<',';
            const auto& r=f.trace[i];
            std::cout<<'['<<static_cast<unsigned>(r.operation)<<','<<r.object<<','<<r.target<<','<<r.value<<']';
        }
        std::cout<<"]}\n";
    }
    if(!in.eof())throw std::runtime_error("fixture parse");
}
int main(int argc,char** argv) {
    try {
        if(argc==2 && std::string(argv[1])=="--guards")guards();
        else if(argc==3 && std::string(argv[1])=="--cases")cases(argv[2]);
        else throw std::runtime_error("usage --guards | --cases fixture");
    } catch(const std::exception& e) {std::cerr<<e.what()<<'\n';return 1;}
}
