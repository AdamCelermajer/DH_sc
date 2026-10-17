#include "../character_update_eligibility.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>
#include <vector>

namespace eu = dh2::character_update_eligibility;

struct Input {
    std::uint32_t online=0, remote=0, local=0, cull=1, dead=0, respawn=0;
    std::uint32_t visual=1, node_cull=1, visibility_80=0, field_2fc=0,
                  gate_1480=0, node_flag=0, mutations=0;
};
struct Context {
    eu::Character* character;
    eu::Visual* primary_visual;
    eu::Visual* alternate_visual;
    Input input;
    std::vector<eu::Request> calls;
    const eu::Services* services=nullptr;
    eu::Result nested_result{};
    eu::Status nested_status=eu::Status::complete;
    int fail_operation=-1;
    bool throw_failure=false;
};

static std::int32_t invoke(void* opaque, const eu::Character*,
                           const eu::Request* request, eu::Response* response) {
    auto& c=*static_cast<Context*>(opaque);
    c.calls.push_back(*request);
    auto& x=c.input;
    if(c.fail_operation==static_cast<int>(request->operation)) {
        c.character->field_418=0x7005;
        if(c.throw_failure)throw std::runtime_error("fixture service failure");
        return 1;
    }
    switch(request->operation) {
    case eu::Operation::get_online_byte:
        response->raw=x.online;
        if(x.mutations&1u)c.character->field_418=0x7002;
        if(x.mutations&1024u)c.character->identity=0x2001;
        if(x.mutations&32u)
            c.nested_status=eu::evaluate(c.character,c.services,&c.nested_result);
        if(x.mutations&64u)
            c.primary_visual->root=reinterpret_cast<eu::SceneNode*>(c.primary_visual);
        return 0;
    case eu::Operation::is_remotely_updated:
        response->raw=x.remote;
        if(x.mutations&2u)c.character->visual_2d8=c.alternate_visual;
        return 0;
    case eu::Operation::local_player_character:
        response->identity=x.local;
        if(x.mutations&4u)c.character->field_418=0x7003;
        if(x.mutations&512u) {
            c.character->visual_2d8=c.alternate_visual;
            c.alternate_visual->root=reinterpret_cast<eu::SceneNode*>(c.alternate_visual);
        }
        return 0;
    case eu::Operation::test_culling_before_update:
        response->raw=x.cull;
        if(x.mutations&8u)c.character->culling_gate_1480=1;
        return 0;
    case eu::Operation::is_dead:
        response->raw=x.dead;
        if(x.mutations&16u)c.character->current_visibility_80=1;
        if(x.mutations&128u)c.primary_visual->root=c.alternate_visual->root;
        return 0;
    case eu::Operation::can_respawn:
        response->raw=x.respawn;
        if(x.mutations&256u) {
            c.character->current_visibility_80=1;
            c.primary_visual->root=c.alternate_visual->root;
        }
        return 0;
    }
    return 1;
}

static Input parse(int argc,char** argv) {
    assert(argc==14);
    Input x{};
    auto u=[&](int i){return static_cast<std::uint32_t>(std::stoull(argv[i]));};
    x.online=u(1);x.remote=u(2);x.local=u(3);x.cull=u(4);x.dead=u(5);x.respawn=u(6);
    x.visual=u(7);x.node_cull=u(8);x.visibility_80=u(9);x.field_2fc=u(10);
    x.gate_1480=u(11);x.node_flag=u(12);x.mutations=u(13);return x;
}

int main(int argc,char** argv) {
    if(argc==2&&std::string(argv[1])=="--guards") {
        Input input{};
        eu::SceneNode node{0x3001,1,9,{}};
        eu::SceneNode alt_node{0x3002,1,9,{}};
        eu::Visual visual{0x2001,&node},alt_visual{0x2002,&alt_node};
        eu::Character character{0x1001,&visual,0x7001,0,0,0,0};
        Context context{&character,&visual,&alt_visual,input,{}};
        const eu::Services services{&context,invoke};context.services=&services;
        eu::Result output{0xabcdu,eu::Outcome::allowed,0x1234u,99u,88u};
        alignas(eu::Visual) unsigned char misaligned_storage[sizeof(eu::Visual)+1]{};
        character.visual_2d8=reinterpret_cast<eu::Visual*>(misaligned_storage+1);
        auto status=eu::evaluate(&character,&services,&output);
        assert(status==eu::Status::invalid_source_fact);
        assert(output.can_update==0xabcdu&&output.service_calls==99u&&context.calls.empty());

        character.visual_2d8=&visual;
        visual.root=reinterpret_cast<eu::SceneNode*>(&visual);
        status=eu::evaluate(&character,&services,&output);
        assert(status==eu::Status::invalid_source_fact);
        assert(output.can_update==0xabcdu&&context.calls.empty());

        visual.root=&node;
        status=eu::evaluate(&character,&services,reinterpret_cast<eu::Result*>(&visual));
        assert(status!=eu::Status::complete&&context.calls.empty());

        visual.root=&node;
        character.current_visibility_80=1;
        context.input.online=1;context.input.remote=1;context.input.mutations=64;
        output={0xabcdu,eu::Outcome::allowed,0x1234u,99u,88u};
        status=eu::evaluate(&character,&services,&output);
        assert(status==eu::Status::invalid_source_fact);
        assert(context.calls.size()==2&&node.update_flag_200==0);

        visual.root=&node;character.current_visibility_80=0;
        context.input.online=0;context.input.remote=0;context.input.local=0x7002;
        context.input.mutations=512;context.calls.clear();output={};
        status=eu::evaluate(&character,&services,&output);
        assert(status==eu::Status::invalid_source_fact&&context.calls.size()==2);
        assert(output.scene_flag_writes==1&&node.update_flag_200==0);

        // The previous callback left Character pointing at the invalid
        // alternate Visual. Restore the entire object graph before reentry.
        character.visual_2d8=&visual;
        visual.root=&node;
        alt_visual.root=&alt_node;
        character.current_visibility_80=0;
        context.input.mutations=32;context.input.local=0x7001;
        context.calls.clear();
        output={};
        context.nested_result={0xabcdu,eu::Outcome::allowed,0x1234u,99u,88u};
        status=eu::evaluate(&character,&services,&output);
        assert(status==eu::Status::complete&&output.can_update==1);
        assert(context.nested_status==eu::Status::reentrant_call);
        assert(context.nested_result.can_update==0xabcdu&&
               context.nested_result.service_calls==99u&&context.calls.size()==3);
        context.calls.clear();context.input.mutations=0x100u;context.input.online=0x100u;
        context.input.dead=0;visual.root=&node;node.update_flag_200=9;
        output={};status=eu::evaluate(&character,&services,&output);
        assert(status==eu::Status::invalid_source_fact&&context.calls.size()==1&&
               output.scene_flag_writes==1&&node.update_flag_200==0);
        std::cout<<"{\"validation\":\"PASS\",\"misaligned_visual\":1,\"initial_graph_alias\":1,\"result_visual_alias\":1,\"late_captured_root_alias\":1,\"fresh_visual_root_alias\":1,\"same_character_reentry\":1,\"online_byte_domain\":1}\n";
        return 0;
    }
    if(argc==2&&std::string(argv[1])=="--failures") {
        for(int op=0;op<=5;++op)for(int throwing=0;throwing<=1;++throwing) {
            Input input{};input.local=0x7001;input.node_cull=1;
            if(op==1)input.online=1;
            if(op==3)input.local=0x7002;
            if(op==5)input.dead=1;
            if(op==0)input.mutations=1;
            eu::SceneNode node{0x3001,1,9,{}};eu::Visual visual{0x2001,&node};
            eu::Character character{0x1001,&visual,0x7001,0,0,0,0};
            Context context{&character,&visual,&visual,input,{}};
            context.fail_operation=op;context.throw_failure=throwing!=0;
            const eu::Services services{&context,invoke};context.services=&services;
            eu::Result result{};const auto status=eu::evaluate(&character,&services,&result);
            assert(status==eu::Status::service_failed&&result.service_calls==context.calls.size());
            assert(result.service_calls>0&&result.scene_flag_writes==1&&node.update_flag_200==0);
            assert(character.field_418==0x7005);
        }
        Input input{};eu::SceneNode node{0x3001,1,9,{}};eu::Visual visual{0x2001,&node};
        eu::Character character{0x1001,&visual,0x7001,0,0,0,0};
        Context context{&character,&visual,&visual,input,{}};eu::Result result{};
        const eu::Services missing{&context,nullptr};
        const auto status=eu::evaluate(&character,&missing,&result);
        assert(status==eu::Status::service_unavailable&&result.service_calls==0);
        assert(result.scene_flag_writes==1&&node.update_flag_200==0);
        std::cout<<"{\"validation\":\"PASS\",\"operation_failures\":12,\"missing_provider\":1}\n";
        return 0;
    }
    Input input=parse(argc,argv);
    eu::SceneNode node{0x3001,input.node_cull,
                       static_cast<std::uint8_t>(input.node_flag),{}};
    eu::SceneNode alt_node{0x3002,input.node_cull+1,
                           static_cast<std::uint8_t>(input.node_flag),{}};
    eu::Visual visual{0x2001,&node},alt_visual{0x2002,&alt_node};
    eu::Character character{0x1001,input.visual?&visual:nullptr,
                            0x7001,static_cast<std::uint8_t>(input.visibility_80),
                            static_cast<std::uint8_t>(input.field_2fc),
                            static_cast<std::uint8_t>(input.gate_1480),0};
    Context context{&character,&visual,&alt_visual,input,{}};
    const eu::Services services{&context,invoke};context.services=&services;eu::Result result{};
    const auto status=eu::evaluate(&character,&services,&result);
    std::cout << "{\"status\":" << static_cast<int>(status)
              << ",\"value\":" << result.can_update
              << ",\"outcome\":" << static_cast<unsigned>(result.outcome)
              << ",\"calls\":" << result.service_calls
              << ",\"writes\":" << result.scene_flag_writes
              << ",\"node_flag\":" << unsigned(node.update_flag_200)
              << ",\"alt_flag\":" << unsigned(alt_node.update_flag_200)
              << ",\"nested_status\":" << static_cast<int>(context.nested_status)
              << ",\"nested_can_update\":" << context.nested_result.can_update
              << ",\"nested_calls\":" << context.nested_result.service_calls
              << ",\"trace\":[";
    for(std::size_t i=0;i<context.calls.size();++i){
        if(i)std::cout<<',';
        const auto& q=context.calls[i];
        std::cout<<'['<<static_cast<unsigned>(q.operation)<<','<<q.character<<','
                 <<q.subject<<','<<q.argument<<','<<q.first<<','<<q.second<<']';
    }
    std::cout<<"]}\n";
}
