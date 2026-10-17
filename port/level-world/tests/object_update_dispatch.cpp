#include "../object_update_dispatch.hpp"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::object_update_dispatch;

namespace {
struct Call {
    std::uint32_t operation;
    std::uintptr_t subject, arg1, arg2, arg3;
};
struct Fixture {
    std::uint32_t first_character, second_character, online, remote;
    std::uint32_t mutate_gate, fail_at;
    std::uint32_t character_calls = 0, callback_calls = 0;
    std::uint32_t throw_at = 0;
    bool mutate_before_error=false;
    Object* expected_object=nullptr;
    std::uintptr_t expected_identity=0;
    ManagerContext* manager=nullptr;
    Services* mutable_services=nullptr;
    std::vector<Call> calls;
};

std::int32_t invoke_fixture(void* context, Object* object,
                            const Request* request, Response* response) {
    auto& fixture = *static_cast<Fixture*>(context);
    if (fixture.expected_object) assert(object==fixture.expected_object);
    if (fixture.expected_identity) assert(request->object==fixture.expected_identity);
    ++fixture.callback_calls;
    fixture.calls.push_back({static_cast<std::uint32_t>(request->operation),
                              request->subject, request->argument1,
                              request->argument2, request->argument3});
    if (fixture.mutate_before_error &&
        (fixture.callback_calls==fixture.throw_at || fixture.callback_calls==fixture.fail_at)) {
        object->culling_phase_86=123;object->update_marker_88=124;
    }
    if (fixture.throw_at && fixture.callback_calls==fixture.throw_at)
        throw std::runtime_error("provider failure");
    if (fixture.fail_at && fixture.callback_calls == fixture.fail_at) return 1;
    switch (request->operation) {
    case Operation::is_character:
        if (!fixture.character_calls && (fixture.mutate_gate&4)) {
            object->update_gate_85=1;object->update_gate_8a=1;
        }
        if (fixture.character_calls && (fixture.mutate_gate&64)) object->culling_phase_86=111;
        if (fixture.character_calls && (fixture.mutate_gate&256) && fixture.manager) {
            fixture.manager->unload_arg1=0x123;fixture.manager->unload_arg3=0x456;
        }
        if (fixture.mutable_services) {
            fixture.mutable_services->invoke=nullptr;fixture.mutable_services->context=nullptr;
        }
        response->raw = fixture.character_calls++ == 0 ?
            fixture.first_character : fixture.second_character;
        break;
    case Operation::update_ai_pointers:
        if (fixture.mutate_gate&1) object->update_gate_85 = 0;
        if (fixture.mutate_gate&2) object->update_gate_8a = 0;
        response->raw = 0xfeed;
        break;
    case Operation::get_online_byte:
        if (fixture.mutate_gate&8) {object->update_gate_85=1;object->update_gate_8a=1;}
        if (fixture.mutate_gate&16) object->deletion_81=7;
        response->raw = fixture.online;break;
    case Operation::is_remotely_updated:
        if (fixture.mutate_gate&32) object->deletion_81=7;
        response->raw = fixture.remote;break;
    case Operation::unload_script_process: response->raw = 0xbad; break;
    case Operation::dispatch_virtual_update:
        if (fixture.mutate_gate&128) object->update_marker_88=9;
        response->raw = 0xa5;break;
    }
    return 0;
}

unsigned number(const char* value) {
    return static_cast<unsigned>(std::strtoul(value, nullptr, 0));
}

int cli(int argc, char** argv) {
    // first-char second-char gate85 gate8a online remote deletion phase marker
    // mutate-gate fail-call has-manager unload-arg1 unload-arg3
    assert(argc == 15);
    Object object{0x1122334455667788ULL, 0xffffffffu, 0,
                  static_cast<std::uint8_t>(number(argv[7])),
                  static_cast<std::uint8_t>(number(argv[3])),
                  static_cast<std::uint8_t>(number(argv[8])),
                  static_cast<std::uint8_t>(number(argv[9])),
                  static_cast<std::uint8_t>(number(argv[4])), {0, 0}};
    Fixture fixture{};
    fixture.first_character = number(argv[1]);
    fixture.second_character = number(argv[2]);
    fixture.online = number(argv[5]);
    fixture.remote = number(argv[6]);
    fixture.mutate_gate = number(argv[10]);
    fixture.fail_at = number(argv[11]);
    ManagerContext manager{number(argv[13]), number(argv[14])};
    fixture.manager=&manager;fixture.expected_object=&object;fixture.expected_identity=object.identity;
    const Services services{&fixture, invoke_fixture};
    Result result{};
    const auto status = dispatch(&object, number(argv[12]) ? &manager : nullptr,
                                 &services, &result);
    std::cout << "{\"status\":" << static_cast<int>(status)
              << ",\"route\":" << static_cast<unsigned>(result.route)
              << ",\"calls\":" << result.service_calls
              << ",\"phase_writes\":" << result.culling_phase_writes
              << ",\"marker_writes\":" << result.update_marker_writes
              << ",\"phase\":" << static_cast<unsigned>(object.culling_phase_86)
              << ",\"marker\":" << static_cast<unsigned>(object.update_marker_88)
              << ",\"trace\":[";
    for (std::size_t i = 0; i < fixture.calls.size(); ++i) {
        if (i) std::cout << ',';
        const auto& call = fixture.calls[i];
        std::cout << '[' << call.operation << ',' << call.subject << ','
                  << call.arg1 << ',' << call.arg2 << ',' << call.arg3 << ']';
    }
    std::cout << "]}\n";
    return 0;
}

struct ReentrantFixture { Object* object; Result nested_result{}; Status nested{}; };
std::int32_t reenter(void* context, Object*, const Request*, Response* response) {
    auto& fixture = *static_cast<ReentrantFixture*>(context);
    Services nested_services{context, [](void* inner, Object*, const Request*, Response* out) {
        static_cast<ReentrantFixture*>(inner)->nested_result.route = Route::none;
        out->raw = 0;
        return 0;
    }};
    fixture.nested = dispatch(fixture.object, nullptr, &nested_services,
                              &fixture.nested_result);
    response->raw = 0;
    return 0;
}

struct NestedFixture {
    Object* object;
    const Services* services;
    const ManagerContext* manager;
    Result* result;
    Status status=Status::complete;
    unsigned calls=0;
    std::uintptr_t captured_identity;
    bool change_identity=false;
};
std::int32_t invoke_nested(void* context,Object* object,const Request* request,Response* response) {
    auto& f=*static_cast<NestedFixture*>(context);
    assert(request->object==f.captured_identity);
    if (!f.calls++) {
        if(f.change_identity) ++object->identity;
        f.status=dispatch(f.object,f.manager,f.services,f.result);
    }
    response->raw=0;
    return 0;
}

void guards() {
    unsigned checks=0;
    auto check=[&](bool value) { if(!value) throw std::runtime_error("protocol guard failed");++checks; };
    Object bad{};
    const Services empty{nullptr, nullptr};
    Result result{};
    check(dispatch(&bad, nullptr, &empty, &result) == Status::invalid_argument);

    Object object{1, 0xffffffffu, 0, 1, 1, 7, 9, 1, {0, 0}};
    Fixture deleting{};
    const Services deleting_services{&deleting, invoke_fixture};
    check(dispatch(&object, nullptr, &deleting_services, &result) ==
           Status::unsupported_deletion_branch);
    check(object.update_marker_88 == 9 && result.route == Route::none);

    object = {2, 0xffffffffu, 0, 0, 0, 8, 9, 1, {0, 0}};
    Fixture missing_manager{};
    missing_manager.first_character = 1;
    missing_manager.second_character = 1;
    const Services missing_services{&missing_manager, invoke_fixture};
    check(dispatch(&object, nullptr, &missing_services, &result) ==
           Status::invalid_source_fact);
    check(object.culling_phase_86 == 0 && result.culling_phase_writes == 1);

    object = {3, 0xffffffffu, 0, 0, 0, 8, 9, 1, {0, 0}};
    ReentrantFixture nested{&object};
    const Services nested_services{&nested, reenter};
    check(dispatch(&object, nullptr, &nested_services, &result) == Status::complete);
    check(nested.nested == Status::reentrant_call);

    object = {4, 0xffffffffu, 0, 0, 0, 8, 9, 1, {0, 0}};
    Fixture fail_after_reset{};
    fail_after_reset.first_character = 1;
    fail_after_reset.second_character = 1;
    fail_after_reset.fail_at = 4; // fail on second IsCharacter
    const Services failed_services{&fail_after_reset, invoke_fixture};
    const ManagerContext manager{0xabc, 0xdef};
    check(dispatch(&object, &manager, &failed_services, &result) == Status::service_failed);
    check(object.culling_phase_86 == 0 && result.culling_phase_writes == 1 &&
           result.route == Route::none);

    object = {5, 0xffffffffu, 0, 0, 1, 8, 9, 1, {0, 0}};
    Fixture fail_update{};
    fail_update.fail_at = 2; // marker store precedes the slot +0x2c provider
    const Services update_failure{&fail_update, invoke_fixture};
    check(dispatch(&object, nullptr, &update_failure, &result) == Status::service_failed);
    check(object.update_marker_88 == 0 && result.update_marker_writes == 1 &&
           result.route == Route::none);

    object = {6, 0xffffffffu, 0, 0, 0, 7, 9, 1, {0, 0}};
    Fixture oversized_online{};
    oversized_online.online = 300;
    const Services oversized_online_services{&oversized_online, invoke_fixture};
    check(dispatch(&object, nullptr, &oversized_online_services, &result) ==
           Status::invalid_source_fact);
    check(object.culling_phase_86 == 7 && result.culling_phase_writes == 0);

    object = {7, 0xffffffffu, 0, 0, 1, 7, 9, 1, {0, 0}};
    check(dispatch(&object, nullptr, &empty, &result) == Status::service_unavailable);
    check(object.update_marker_88 == 9 && result.route == Route::none);

    for (bool throwing:{false,true}) {
        for(unsigned failure=1;failure<=5;++failure) {
            Object current{0x100000001ull,0xffffffffu,0,0,0,7,9,1,{}};
            Fixture f{};f.first_character=f.second_character=1;
            if(throwing) f.throw_at=failure;else f.fail_at=failure;
            const Services services{&f,invoke_fixture};Result out{};
            check(dispatch(&current,&manager,&services,&out)==Status::service_failed);
            check(out.service_calls==failure && out.culling_phase_writes==(failure>=4?1u:0u) &&
                  current.culling_phase_86==(failure>=4?0:7) && current.update_marker_88==9);
        }
        for(unsigned failure=1;failure<=3;++failure) {
            Object current{0x100000001ull,0xffffffffu,0,0,1,7,9,1,{}};
            Fixture f{};f.first_character=1;
            if(throwing) f.throw_at=failure;else f.fail_at=failure;
            const Services services{&f,invoke_fixture};Result out{};
            check(dispatch(&current,&manager,&services,&out)==Status::service_failed);
            check(out.service_calls==failure && out.update_marker_writes==(failure==3?1u:0u) &&
                  current.update_marker_88==(failure==3?0:9) && current.culling_phase_86==7);
        }
        for(bool updating:{false,true}) {
            Object current{0x100000001ull,0xffffffffu,0,0,
                static_cast<std::uint8_t>(updating),7,9,1,{}};
            Fixture f{};f.first_character=f.second_character=1;f.mutate_before_error=true;
            if(throwing) f.throw_at=updating?3:5;else f.fail_at=updating?3:5;
            const Services services{&f,invoke_fixture};Result out{};
            check(dispatch(&current,&manager,&services,&out)==Status::service_failed);
            check(current.culling_phase_86==123 && current.update_marker_88==124 &&
                  out.route==Route::none &&
                  (updating?out.update_marker_writes:out.culling_phase_writes)==1);
        }
    }
    {
        Object current{0x100000001ull,0xffffffffu,0,0,1,7,9,1,{}};
        Fixture f{};Services services{&f,invoke_fixture};f.mutable_services=&services;
        Result out{};
        check(dispatch(&current,nullptr,&services,&out)==Status::complete && out.service_calls==2);
        check(!services.invoke && !services.context && current.update_marker_88==0);
    }
    {
        Object current{0x100000001ull,0xffffffffu,0,0,0,7,9,1,{}};
        Fixture f{};f.first_character=f.second_character=1;
        const ManagerContext wide{0x200000002ull,0x300000003ull};
        const Services services{&f,invoke_fixture};Result out{};
        check(dispatch(&current,&wide,&services,&out)==Status::complete);
        check(f.calls.back().arg1==wide.unload_arg1 && f.calls.back().arg2==0 &&
              f.calls.back().arg3==wide.unload_arg3);
    }
    for(unsigned kind=0;kind<7;++kind) {
        Object current{0x100000001ull,0xffffffffu,0,0,1,7,9,1,{}};
        Object other{0x200000002ull,0xffffffffu,0,0,1,7,9,1,{}};
        Fixture child{};Services child_services{&child,invoke_fixture};Result out{},nested_out{};
        nested_out.service_calls=42;
        NestedFixture f{kind==0?&current:&other,&child_services,nullptr,
            kind==1?&out:&nested_out,Status::complete,0,current.identity,kind==0};
        if(kind==3) f.object=reinterpret_cast<Object*>(&out);
        if(kind==4) f.services=reinterpret_cast<const Services*>(&out);
        if(kind==5) f.manager=reinterpret_cast<const ManagerContext*>(&out);
        if(kind==6) {other.identity=current.identity;}
        Services services{&f,invoke_nested};
        check(dispatch(&current,nullptr,&services,&out)==Status::complete);
        check(f.status==(kind==0||kind==6?Status::reentrant_call:
            kind==2?Status::complete:Status::invalid_argument));
        check(out.service_calls==2 && out.update_marker_writes==1 && current.update_marker_88==0);
        check(kind==2?nested_out.service_calls==2:nested_out.service_calls==42);
    }
    {
        Object current{0x100000001ull,0xffffffffu,0,0,1,7,9,1,{}};
        Fixture f{};Services services{&f,invoke_fixture};Result out{};out.service_calls=42;
        check(dispatch(nullptr,nullptr,&services,&out)==Status::invalid_argument);
        check(dispatch(&current,nullptr,nullptr,&out)==Status::invalid_argument);
        check(dispatch(&current,nullptr,&services,nullptr)==Status::invalid_argument);
        check(dispatch(&current,reinterpret_cast<const ManagerContext*>(&current),&services,&out)==Status::invalid_argument);
        check(dispatch(&current,&manager,reinterpret_cast<const Services*>(&out),&out)==Status::invalid_argument);
        check(dispatch(&current,&manager,&services,reinterpret_cast<Result*>(&current))==Status::invalid_argument);
        check(dispatch(&current,&manager,reinterpret_cast<const Services*>(&manager),&out)==Status::invalid_argument);
        check(dispatch(&current,reinterpret_cast<const ManagerContext*>(&services),&services,&out)==Status::invalid_argument);
        check(dispatch(&current,reinterpret_cast<const ManagerContext*>(&out),&services,&out)==Status::invalid_argument);
        check(out.service_calls==42 && current.culling_phase_86==7 && current.update_marker_88==9);
        alignas(Object) unsigned char object_bytes[sizeof(Object)+alignof(Object)]{};
        alignas(Services) unsigned char service_bytes[sizeof(Services)+alignof(Services)]{};
        alignas(ManagerContext) unsigned char manager_bytes[sizeof(ManagerContext)+alignof(ManagerContext)]{};
        alignas(Result) unsigned char result_bytes[sizeof(Result)+alignof(Result)]{};
        check(dispatch(reinterpret_cast<Object*>(object_bytes+1),nullptr,&services,&out)==Status::invalid_argument);
        check(dispatch(&current,nullptr,reinterpret_cast<const Services*>(service_bytes+1),&out)==Status::invalid_argument);
        check(dispatch(&current,reinterpret_cast<const ManagerContext*>(manager_bytes+1),&services,&out)==Status::invalid_argument);
        check(dispatch(&current,nullptr,&services,reinterpret_cast<Result*>(result_bytes+1))==Status::invalid_argument);
    }
    std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<"}\n";
}
}

int main(int argc, char** argv) {
    if (argc == 2 && std::string(argv[1]) == "--guards") { guards(); return 0; }
    return cli(argc, argv);
}
