#include "character_culling_runtime.hpp"

#include <limits>

namespace dh2::character_culling_runtime {
namespace {
namespace e = character_update_eligibility;
namespace c = object_update_culling;
struct Range { std::uintptr_t begin, end; };
template<class T> bool range(const T* p, Range& r) {
    const auto begin = reinterpret_cast<std::uintptr_t>(p);
    if (!p || begin % alignof(T) ||
        begin > std::numeric_limits<std::uintptr_t>::max() - sizeof(T))
        return false;
    r = {begin, begin + sizeof(T)};
    return true;
}
bool overlap(Range a, Range b) {
    return a.begin < b.end && b.begin < a.end;
}
struct ActiveOutput { Range output; ActiveOutput* previous; };
thread_local ActiveOutput* active_output = nullptr;
bool outside_active_outputs(Range candidate) {
    for (auto* frame=active_output;frame;frame=frame->previous)
        if (overlap(candidate,frame->output)) return false;
    return true;
}
struct OutputGuard {
    ActiveOutput frame;
    explicit OutputGuard(Range output) : frame{output,active_output} { active_output=&frame; }
    ~OutputGuard() { active_output=frame.previous; }
};
template<class T> bool excludes(const T* p, Range control, Range out) {
    if (!p) return true;
    const auto begin = reinterpret_cast<std::uintptr_t>(p);
    if (begin > std::numeric_limits<std::uintptr_t>::max() - sizeof(T)) return false;
    const Range r{begin, begin + sizeof(T)};
    return !overlap(r, control) && !overlap(r, out);
}
struct Context {
    Bindings bindings;
    e::Services character_services;
    c::Services camera_services;
    Result* result;
    std::uintptr_t character_identity;
    Range control;
    Range output;
    e::Visual* captured_visual;
};

// Nested kernels know their own output only. Keep all reached projections
// outside the complete composition output/control and the other caller's state.
template<class T> bool clean_projection(const Context& ctx, const T* p) {
    if (!p) return true;
    Range candidate{}, character{}, table{}, camera{};
    if (!range(p,candidate) || !outside_active_outputs(candidate) ||
        overlap(candidate,ctx.control) || overlap(candidate,ctx.output) ||
        !range(ctx.bindings.character,character) || overlap(candidate,character) ||
        !range(ctx.bindings.character_services,table) || overlap(candidate,table)) return false;
    if (ctx.bindings.camera_services &&
        (!range(ctx.bindings.camera_services,camera) || overlap(candidate,camera))) return false;
    const auto outside_input=[&](const auto* input) {
        Range other{};
        return !input || (range(input,other) && !overlap(candidate,other));
    };
    return outside_input(ctx.bindings.object) && outside_input(ctx.bindings.aabb) &&
           outside_input(ctx.bindings.globals);
}
bool clean_visual_graph_at(const Context& ctx,const e::Visual* visual) {
    if (!clean_projection(ctx,visual)) return false;
    if (!visual) return true; // The source kernel diagnoses reached null facts.
    const auto* root=visual->root;
    if (!clean_projection(ctx,root)) return false;
    Range vr{}, rr{};
    return !root || (range(visual,vr) && range(root,rr) && !overlap(vr,rr));
}
bool clean_visual_graph(const Context& ctx) {
    return clean_visual_graph_at(ctx,ctx.captured_visual) &&
           clean_visual_graph_at(ctx,ctx.bindings.character->visual_2d8);
}
template<class T> bool clean_foreign_projection(const Context& ctx,const T* p) {
    if (!p) return true;
    if (!clean_projection(ctx,p)) return false;
    Range candidate{}, visual_range{}, root_range{};
    if (!range(p,candidate)) return false;
    const auto outside_graph=[&](const e::Visual* visual) {
        if (!visual) return true;
        if (range(visual,visual_range) && overlap(candidate,visual_range)) return false;
        // Do not diagnose unrelated late Character facts before culling completes.
        // Read a Visual pointer field only after its own projection is safe.
        if (!clean_projection(ctx,visual)) return true;
        return !visual->root || !range(visual->root,root_range) || !overlap(candidate,root_range);
    };
    return outside_graph(ctx.captured_visual) &&
           outside_graph(ctx.bindings.character->visual_2d8);
}

int query_culling(void* raw, c::Object* object, const c::Request* request,
                  c::Response* response) {
    auto& ctx = *static_cast<Context*>(raw);
    if (object != ctx.bindings.object ||
        request->object != ctx.character_identity) return 1;
    if (request->operation == c::Operation::get_online_byte ||
        request->operation == c::Operation::is_remotely_updated) {
        if (!ctx.character_services.invoke) return 1;
        const e::Request translated{
            request->operation == c::Operation::get_online_byte
                ? e::Operation::get_online_byte : e::Operation::is_remotely_updated,
            ctx.character_identity, request->subject, 0, 0, 0};
        e::Response value{};
        const int status = ctx.character_services.invoke(
            ctx.character_services.context, ctx.bindings.character,
            &translated, &value);
        response->raw = value.raw;
        return status;
    }
    if (!ctx.camera_services.invoke) return 1;
    const int status=ctx.camera_services.invoke(ctx.camera_services.context,object,request,response);
    if (status) return status;
    if (request->operation==c::Operation::get_current_level && response->level) {
        const auto* level=response->level;
        if (!clean_foreign_projection(ctx,level)) return 1;
        const auto* camera=level->camera_128;
        if (!clean_foreign_projection(ctx,camera)) return 1;
        if (camera && !clean_foreign_projection(ctx,camera->root_8)) return 1;
    }
    if (request->operation==c::Operation::get_view_frustum &&
        !clean_foreign_projection(ctx,response->frustum)) return 1;
    return 0;
}

int query_character(void* raw, const e::Character* character,
                    const e::Request* request, e::Response* response) {
    auto& ctx = *static_cast<Context*>(raw);
    if (character != ctx.bindings.character ||
        request->character != ctx.character_identity) return 1;
    if (request->operation != e::Operation::test_culling_before_update) {
        if (!ctx.character_services.invoke) return 1;
        const int status=ctx.character_services.invoke(ctx.character_services.context,character,request,response);
        return status ? status : clean_visual_graph(ctx) ? 0 : 1;
    }
    ++ctx.result->culling_calls;
    Range object_range{};
    if (request->subject != ctx.character_identity || request->argument != 0x12c ||
        !range(ctx.bindings.object, object_range) ||
        ctx.bindings.object->identity != ctx.character_identity) {
        ctx.result->culling_status = c::Status::invalid_source_fact;
        return 1;
    }
    const c::Services services{&ctx, query_culling};
    ctx.result->culling_status = c::evaluate(
        ctx.bindings.object, ctx.bindings.aabb, ctx.bindings.globals,
        &services, &ctx.result->culling);
    if (ctx.result->culling_status != c::Status::complete) return 1;
    if (!clean_visual_graph(ctx)) return 1;
    response->raw = ctx.result->culling.can_update;
    return 0;
}
}

Status evaluate(const Bindings* bindings, Result* result) {
    Range control{}, out{}, character{}, services{};
    if (!range(bindings, control) || !range(result, out) ||
        !outside_active_outputs(control) || !outside_active_outputs(out) ||
        overlap(control, out) || !range(bindings->character, character) ||
        !range(bindings->character_services, services) ||
        !outside_active_outputs(character) || !outside_active_outputs(services) ||
        overlap(control, character) || overlap(out, character) ||
        overlap(control, services) || overlap(out, services) ||
        overlap(character, services)) return Status::invalid_argument;
    Range camera_services{};
    if (bindings->camera_services &&
        (!range(bindings->camera_services, camera_services) ||
         !outside_active_outputs(camera_services) ||
         overlap(control, camera_services) || overlap(out, camera_services) ||
         overlap(character, camera_services) || overlap(services, camera_services)))
        return Status::invalid_argument;
    if (!excludes(bindings->object, control, out) ||
        !excludes(bindings->aabb, control, out) ||
        !excludes(bindings->globals, control, out) ||
        !excludes(bindings->character->visual_2d8, control, out))
        return Status::invalid_argument;
    if (!excludes(bindings->object,character,services) ||
        !excludes(bindings->aabb,character,services) ||
        !excludes(bindings->globals,character,services) ||
        !excludes(bindings->object,camera_services,camera_services) ||
        !excludes(bindings->aabb,camera_services,camera_services) ||
        !excludes(bindings->globals,camera_services,camera_services)) return Status::invalid_argument;
    Range lazy{};
    if ((bindings->object && (!range(bindings->object,lazy) || !outside_active_outputs(lazy))) ||
        (bindings->aabb && (!range(bindings->aabb,lazy) || !outside_active_outputs(lazy))) ||
        (bindings->globals && (!range(bindings->globals,lazy) || !outside_active_outputs(lazy))))
        return Status::invalid_argument;
    const Context captured{*bindings, *bindings->character_services,
        bindings->camera_services ? *bindings->camera_services : c::Services{},
        result, bindings->character->identity,control,out,bindings->character->visual_2d8};
    Context ctx = captured;
    if (!clean_visual_graph(ctx)) return Status::invalid_argument;
    OutputGuard output_guard(out);
    *result = {};
    const e::Services bound{&ctx, query_character};
    result->character_status = e::evaluate(bindings->character, &bound,
                                           &result->character);
    if (result->character_status == e::Status::complete) return Status::complete;
    return result->culling_calls && result->culling_status != c::Status::complete
        ? Status::culling_failed : Status::character_failed;
}
} // namespace dh2::character_culling_runtime
