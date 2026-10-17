#include "game_object_set_visible.hpp"

#include <limits>

namespace dh2::game_object_set_visible { namespace {
struct Range { Address address; std::size_t size; };
bool range(const void* p, std::size_t n, std::size_t alignment = 1) {
    const auto a = reinterpret_cast<Address>(p);
    return p && a % alignment == 0 && n <= std::numeric_limits<Address>::max() - a;
}
bool overlaps(Range a, Range b) {
    return a.size && b.size && a.address < b.address + b.size && b.address < a.address + a.size;
}
template<class T> Range span(const T* p) { return {reinterpret_cast<Address>(p), sizeof(T)}; }
struct Active {
    Range output;
    Active* previous;
};
thread_local Active* active = nullptr;
struct Scope {
    Active entry;
    explicit Scope(Result* p) : entry{span(p),active} { active=&entry; }
    ~Scope() { active=entry.previous; }
};

struct Check {
    Range result;
    Range table;
    Range context;
    struct Source { Range range; const void* type; };
    Source sources[6]{};
    std::size_t count=0;
    bool start(const Services* s, Result* r) {
        if (!range(r,sizeof(*r),alignof(Result))) return false;
        result=span(r);
        for (auto* p=active;p;p=p->previous) if (overlaps(result,p->output)) return false;
        if (s) {
            if (!range(s,sizeof(*s),alignof(Services))) return false;
            table=span(s);
            if (overlaps(table,result)) return false;
            for (auto* p=active;p;p=p->previous) if (overlaps(table,p->output)) return false;
            if (s->context_extent) {
                if (!range(s->context,s->context_extent)) return false;
                context={reinterpret_cast<Address>(s->context),s->context_extent};
                if (overlaps(context,result)) return false;
            }
        }
        return true;
    }
    template<class T> bool source(const T* p) {
        static const char type=0;
        if (!range(p,sizeof(*p),alignof(T))) return false;
        const Range s=span(p);
        if (overlaps(s,result) || overlaps(s,table)) return false;
        for (auto* frame=active;frame;frame=frame->previous)
            if (overlaps(s,frame->output)) return false;
        for (std::size_t i=0;i<count;++i) {
            if (sources[i].range.address==s.address && sources[i].range.size==s.size && sources[i].type==&type)
                return true;
            if (overlaps(s,sources[i].range)) return false;
        }
        if (count==6) return false;
        sources[count++]={s,&type};
        return true;
    }
};
Status invoke(const Services& s, Result& r, Operation op, Address object,
              Address target=0, std::uint32_t value=0) {
    if (!s.invoke) return Status::service_unavailable;
    const Request request{op,object,target,value};
    ++r.service_calls; r.last_operation=static_cast<std::uint32_t>(op); r.last_argument=value;
    try { if (s.invoke(s.context,&request)!=0) return Status::service_failed; }
    catch (...) { return Status::service_failed; }
    return Status::complete;
}
Result initial() { return Result{0,0,0xffffffffu,0,0,0,0}; }
}

Status set_game_object(GameObject* object, std::uint32_t raw,
                       const Services* services, Result* out) {
    Check check{};
    if (!check.start(services,out) || !check.source(object) || !object->identity)
        return Status::invalid_argument;
    const Services captured=services?*services:Services{};
    Scope scope(out); *out=initial();
    // 38b0f4 captures visual before the byte read/store; 38b0f8 only reads +8a
    // for nonzero R1. Tail target 4713d0 is SyncVisibility(), not SetVisible().
    VisualObject* const visual=object->visual_2d8;
    const std::uint8_t value=raw?object->enabled_8a:0;
    object->visibility_80=value;
    ++out->source_writes; out->written_visibility=value;
    if (!visual) return Status::complete;
    if (!check.source(visual) || !visual->identity) return Status::invalid_source_fact;
    out->captured_visual=visual->identity;
    return invoke(captured,*out,Operation::sync_visibility,visual->identity);
}

Status set_visual_object(VisualObject* visual, std::uint32_t raw, const Globals* globals,
                         const Services* services, Result* out) {
    Check check{};
    if (!check.start(services,out) || !check.source(visual) || !visual->identity)
        return Status::invalid_argument;
    const Services captured=services?*services:Services{};
    Scope scope(out); *out=initial();
    SceneNode* node=visual->root_8;
    if (!node) return Status::complete;
    if (!check.source(node) || !node->identity) return Status::invalid_source_fact;
    if (raw!=(node->flags_11c & 1u)) {
        // Source singleton -> Application+10 -> Device+1c are only reached here.
        if (!check.source(globals)) return Status::invalid_source_fact;
        Application* const app=globals->application;
        if (!check.source(app)) return Status::invalid_source_fact;
        Device* const device=app->device_10;
        if (!check.source(device) || !device->scene_manager_1c) return Status::invalid_source_fact;
        const auto status=invoke(captured,*out,Operation::force_register,device->scene_manager_1c);
        if (status!=Status::complete) return status;
        node=visual->root_8; // 4713ac reload, after ForceRegister side effects.
        if (!check.source(node) || !node->identity) return Status::invalid_source_fact;
    }
    const Address target=node->set_visible_target_48; // Fresh vptr/slot after reload.
    if (!target) return Status::invalid_source_fact;
    out->dispatched_root=node->identity;
    return invoke(captured,*out,Operation::node_set_visible,node->identity,target,raw);
}
}
