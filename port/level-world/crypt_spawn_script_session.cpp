#include <map>
#include "crypt_spawn_script_session.hpp"
#include <cstring>
#include <set>

namespace dh2::character::crypt_scripts {
namespace {
bool decode(const std::vector<std::uint8_t>& names,
            const std::vector<std::uint8_t>& programs,
            dh2_script_table& table, std::string& error) {
    if (names.size() > DH2_SCRIPT_MAX_TABLE_BYTES ||
        programs.size() > DH2_SCRIPT_MAX_TABLE_BYTES) {
        error = "Crypt script table exceeds decoded input limit";
        return false;
    }
    dh2_script_error detail{};
    const auto code = dh2_script_table_decode(names.data(), std::uint32_t(names.size()),
        programs.data(), std::uint32_t(programs.size()), &table, &detail);
    if (code != DH2_SCRIPT_OK) {
        error = std::string("Crypt script decode: ") + dh2_script_error_name(code) +
                " at byte " + std::to_string(detail.offset);
        return false;
    }
    return true;
}
bool bounded(const char* value, std::size_t size) {
    return std::memchr(value, 0, size) != nullptr;
}
bool spawn_program(const dh2_script& script,const SpawnDispatch& dispatch) {
    for (std::uint32_t i = 0; i < script.command_count; ++i) {
        const auto& command = script.commands[i];
        if (command.command_id == 0) {
            if (command.field_count != 4 ||
                command.fields[0].kind != DH2_SCRIPT_VALUE_BOOL ||
                command.fields[1].kind != DH2_SCRIPT_VALUE_I32 ||
                command.fields[2].kind != DH2_SCRIPT_VALUE_I32_ARRAY ||
                command.fields[2].value.i32_array.count != 0 ||
                command.fields[3].kind != DH2_SCRIPT_VALUE_BOOL) return false;
            continue;
        }
        if(command.command_id==5){
            if(!dispatch.play_camera||command.field_count!=2||
               command.fields[0].kind!=DH2_SCRIPT_VALUE_I32||
               command.fields[1].kind!=DH2_SCRIPT_VALUE_BOOL)return false;
            continue;
        }
        if(command.command_id==41||command.command_id==46){
            if(!dispatch.actor_command||command.field_count!=2||
               command.fields[0].kind!=DH2_SCRIPT_VALUE_STRING||
               command.fields[1].kind!=DH2_SCRIPT_VALUE_STRING||
               command.fields[0].value.string.size>=
                   dh2_script_runtime::MAX_NAME_BYTES||
               command.fields[1].value.string.size>=
                   dh2_script_runtime::MAX_NAME_BYTES)return false;
            continue;
        }
        if(command.command_id==45){
            if(command.field_count!=5||
               command.fields[0].kind!=DH2_SCRIPT_VALUE_I32||
               command.fields[1].kind!=DH2_SCRIPT_VALUE_I32||
               command.fields[2].kind!=DH2_SCRIPT_VALUE_I32||
               command.fields[3].kind!=DH2_SCRIPT_VALUE_STRING||
               command.fields[4].kind!=DH2_SCRIPT_VALUE_BOOL||
               !dispatch.play_actor_animation||
               (command.fields[4].value.boolean&&!dispatch.actor_animation_active)||
               command.fields[3].value.string.size>=
                   dh2_script_runtime::MAX_NAME_BYTES)return false;
            continue;
        }
        if(command.command_id==10){
            if(!dispatch.start_dialog||command.field_count!=3||
               command.fields[0].kind!=DH2_SCRIPT_VALUE_I32||
               command.fields[1].kind!=DH2_SCRIPT_VALUE_I32||
               command.fields[2].kind!=DH2_SCRIPT_VALUE_I32)return false;
            continue;
        }
        if(command.command_id==11){
            if(!dispatch.start_dialog_id||command.field_count!=1||
               command.fields[0].kind!=DH2_SCRIPT_VALUE_I32)return false;
            continue;
        }
        if(command.command_id==12){
            if(!dispatch.dialog_active||command.field_count!=0)return false;
            continue;
        }
        if(command.command_id==74){
            if(!dispatch.enqueue_char_menu_tutorial||command.field_count!=3||
               command.fields[0].kind!=DH2_SCRIPT_VALUE_STRING||
               command.fields[1].kind!=DH2_SCRIPT_VALUE_STRING||
               command.fields[2].kind!=DH2_SCRIPT_VALUE_I32||
               command.fields[0].value.string.size>=dh2_script_runtime::MAX_NAME_BYTES||
               command.fields[1].value.string.size>=dh2_script_runtime::MAX_NAME_BYTES)
                return false;
            continue;
        }
        if(command.command_id==75||command.command_id==76){
            if(!dispatch.skip_char_menu_tutorial||command.field_count!=0)return false;
            continue;
        }
        if(command.command_id==54||command.command_id==55){
            if(!dispatch.door_command||!dispatch.door_is_blocking||
               command.field_count!=2||
               command.fields[0].kind!=DH2_SCRIPT_VALUE_STRING||
               command.fields[1].kind!=DH2_SCRIPT_VALUE_BOOL||
               command.fields[0].value.string.size>=
                   dh2_script_runtime::MAX_NAME_BYTES)return false;
            continue;
        }
        if(dispatch.host_supports&&dispatch.host_command&&
           dispatch.host_supports(dispatch.context,&command))continue;
        if ((command.command_id != 26 && command.command_id != 30) ||
            command.field_count != 1 ||
            command.fields[0].kind != (command.command_id == 26 ?
                DH2_SCRIPT_VALUE_I32 : DH2_SCRIPT_VALUE_STRING) ||
            (command.command_id == 30 &&
             command.fields[0].value.string.size >=
                 dh2_script_runtime::MAX_NAME_BYTES)) {
            return false;
        }
    }
    return true;
}
}

struct SpawnSession::Impl {
    dh2_script_table common{}, level{};
    dh2_script_runtime::Runtime scheduler{};
    SpawnDispatch dispatch{};
    std::uint32_t consumed_events = 0, dispatched = 0;
    bool updating = false, failed = false;
    ~Impl() {
        // The task records are discarded before their owned table storage.
        scheduler = {};
        dh2_script_table_destroy(&level);
        dh2_script_table_destroy(&common);
    }
};

SpawnSession::SpawnSession() = default;
SpawnSession::~SpawnSession() = default;

bool SpawnSession::load(const std::vector<std::uint8_t>& common_names,
                       const std::vector<std::uint8_t>& common_programs,
                       const std::vector<std::uint8_t>& level_names,
                       const std::vector<std::uint8_t>& level_programs,
                       const dh2_script_runtime::ObjectSeed* objects, std::uint32_t count,
                       const char* trigger_name, const char* script_name,
                       std::int32_t trigger_count, const SpawnDispatch& dispatch,
                       std::string& error) {
    using namespace dh2_script_runtime;
    if ((impl_ && impl_->updating) || !dispatch.request ||
        ((trigger_name == nullptr) != (script_name == nullptr)) ||
        count > MAX_OBJECTS || (count && !objects)) {
        error = "Crypt script session arguments are invalid";
        return false;
    }
    std::set<std::string> names;
    for (std::uint32_t i = 0; i < count; ++i) {
        const auto& object = objects[i];
        if (!bounded(object.name, sizeof(object.name)) ||
            !bounded(object.gametype, sizeof(object.gametype)) ||
            !bounded(object.ai_state, sizeof(object.ai_state)) ||
            !object.name[0] || !names.insert(object.name).second) {
            error = "Crypt script object names/fields are invalid or ambiguous";
            return false;
        }
    }
    auto candidate = std::make_unique<Impl>();
    if (!decode(common_names, common_programs, candidate->common, error) ||
        !decode(level_names, level_programs, candidate->level, error)) return false;
    if (trigger_name != nullptr) {
        const auto id = dh2_script_resolve_id(&candidate->common, &candidate->level,
            reinterpret_cast<const std::uint8_t*>(script_name),
            std::uint32_t(std::strlen(script_name)), 0);
        if (id < std::int32_t(candidate->common.script_count)) {
            error = "Crypt level script is absent";
            return false;
        }
        const auto& script = candidate->level.scripts[id - candidate->common.script_count];
        // The selected Crypt trigger runs Wait/SpawnCharacter/PlayCamera. Other
        // programs must fail before activation instead of taking diagnostic
        // scheduler no-ops.
        if (!spawn_program(script,dispatch)) {
            error = "Crypt script requires an unbound command service";
            return false;
        }
    }
    const auto result = init(&candidate->scheduler, &candidate->common, &candidate->level,
        objects, count, trigger_name, script_name, trigger_count, 0, nullptr);
    if (result != ERROR_OK) {
        error = std::string("Crypt scheduler initialization: ") + error_name(result);
        return false;
    }
    candidate->dispatch = dispatch;
    candidate->scheduler.spawn_services = {candidate.get(), [](void* raw, const Event& event) {
        auto& owner = *static_cast<Impl*>(raw);
        int accepted;
        try { accepted = owner.dispatch.request(owner.dispatch.context, event); }
        catch (...) { owner.failed = true; throw; }
        if (accepted < 0) owner.failed = true;
        if (accepted > 0) ++owner.dispatched;
        return accepted;
    }, [](void* raw,std::uint8_t use_crit,std::int32_t clip_id) {
        auto& owner=*static_cast<Impl*>(raw);
        if(!owner.dispatch.play_camera)return -1;
        int accepted;
        try{accepted=owner.dispatch.play_camera(owner.dispatch.context,use_crit,clip_id);}
        catch(...){owner.failed=true;throw;}
        if(accepted<0)owner.failed=true;
        return accepted;
    },nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
      nullptr,nullptr,nullptr};
    if(dispatch.lookup_object) {
        candidate->scheduler.spawn_services.lookup_object =
            [](void* raw,const char* name,ObjectRecord* output) {
                auto& owner=*static_cast<Impl*>(raw);
                int result;
                try {
                    result=owner.dispatch.lookup_object(
                        owner.dispatch.context,name,output);
                } catch(...) {
                    owner.failed=true;
                    throw;
                }
                if(result<0)owner.failed=true;
                return result;
            };
    }
    if(dispatch.actor_command) {
        candidate->scheduler.spawn_services.actor_command =
            [](void* raw,const SpawnServices::ActorCommandRequest* request) {
                auto& owner=*static_cast<Impl*>(raw);
                int result;
                try {
                    result=owner.dispatch.actor_command(
                        owner.dispatch.context,request);
                } catch(...) {
                    owner.failed=true;
                    throw;
                }
                if(result<0)owner.failed=true;
                return result;
            };
    }
    if(dispatch.play_actor_animation) {
        candidate->scheduler.spawn_services.play_actor_animation =
            [](void* raw,const SpawnServices::ActorAnimationRequest* request,
               std::uintptr_t* identity) {
                auto& owner=*static_cast<Impl*>(raw);
                if(!request||!identity||!owner.dispatch.play_actor_animation)
                    return -1;
                int result;
                try {
                    result=owner.dispatch.play_actor_animation(
                        owner.dispatch.context,request,identity);
                } catch(...) {
                    owner.failed=true;
                    throw;
                }
                if(result<0)owner.failed=true;
                return result;
            };
    }
    if(dispatch.actor_animation_active) {
        candidate->scheduler.spawn_services.actor_animation_active =
            [](void* raw,std::uintptr_t identity,std::int32_t expected_state) {
                auto& owner=*static_cast<Impl*>(raw);
                if(!owner.dispatch.actor_animation_active)return -1;
                int result;
                try {
                    result=owner.dispatch.actor_animation_active(
                        owner.dispatch.context,identity,expected_state);
                } catch(...) {
                    owner.failed=true;
                    throw;
                }
                if(result<0)owner.failed=true;
                return result;
            };
    }
    if(dispatch.start_dialog) {
        candidate->scheduler.spawn_services.start_dialog =
            [](void* raw,const SpawnServices::DialogRequest* request) {
                auto& owner=*static_cast<Impl*>(raw);
                if(!request||!owner.dispatch.start_dialog)return -1;
                int result;
                try {
                    result=owner.dispatch.start_dialog(
                        owner.dispatch.dialog_context,request);
                } catch(...) {
                    owner.failed=true;
                    throw;
                }
                if(result<0)owner.failed=true;
                return result;
            };
    }
    if(dispatch.start_dialog_id) {
        candidate->scheduler.spawn_services.start_dialog_id =
            [](void* raw,const SpawnServices::DialogIdRequest* request) {
                auto& owner=*static_cast<Impl*>(raw);
                if(!request||!owner.dispatch.start_dialog_id)return -1;
                int result;
                try {
                    result=owner.dispatch.start_dialog_id(
                        owner.dispatch.dialog_context,request);
                } catch(...) {
                    owner.failed=true;
                    throw;
                }
                if(result<0)owner.failed=true;
                return result;
            };
    }
    if(dispatch.enqueue_char_menu_tutorial) {
        candidate->scheduler.spawn_services.enqueue_char_menu_tutorial =
            [](void* raw,const SpawnServices::CharMenuTutorialRequest* request) {
                auto& owner=*static_cast<Impl*>(raw);
                if(!request||!owner.dispatch.enqueue_char_menu_tutorial)return -1;
                int result;
                try { result=owner.dispatch.enqueue_char_menu_tutorial(
                    owner.dispatch.char_menu_tutorial_context,request); }
                catch(...) { owner.failed=true; throw; }
                if(result<0)owner.failed=true;
                return result;
            };
    }
    if(dispatch.skip_char_menu_tutorial) {
        candidate->scheduler.spawn_services.skip_char_menu_tutorial =
            [](void* raw,uint8_t skip_all) {
                auto& owner=*static_cast<Impl*>(raw);
                if(!owner.dispatch.skip_char_menu_tutorial)return -1;
                int result;
                try { result=owner.dispatch.skip_char_menu_tutorial(
                    owner.dispatch.char_menu_tutorial_context,skip_all!=0); }
                catch(...) { owner.failed=true; throw; }
                if(result<0)owner.failed=true;
                return result;
            };
    }
    if(dispatch.door_command) {
        candidate->scheduler.spawn_services.door_command =
            [](void* raw,const SpawnServices::DoorCommandRequest* request,
               uintptr_t* identity) {
                auto& owner=*static_cast<Impl*>(raw);
                if(!request||!identity||!owner.dispatch.door_command)return -1;
                int result;
                try { result=owner.dispatch.door_command(
                    owner.dispatch.context,request,identity); }
                catch(...) { owner.failed=true; throw; }
                if(result<0)owner.failed=true;
                return result;
            };
    }
    if(dispatch.host_command) {
        candidate->scheduler.spawn_services.host_command =
            [](void* raw,const dh2_script_command* command,uintptr_t* handle) {
                auto& owner=*static_cast<Impl*>(raw);
                if(!command||!handle||!owner.dispatch.host_command)return -1;
                int result;
                try { result=owner.dispatch.host_command(
                    owner.dispatch.context,command,handle); }
                catch(...) { owner.failed=true; throw; }
                if(result==-1)owner.failed=true;
                return result;
            };
    }
    if(dispatch.host_command_blocking) {
        candidate->scheduler.spawn_services.host_command_blocking =
            [](void* raw,uintptr_t handle,uint32_t command_id) {
                auto& owner=*static_cast<Impl*>(raw);
                if(!owner.dispatch.host_command_blocking)return -1;
                int result;
                try { result=owner.dispatch.host_command_blocking(
                    owner.dispatch.context,handle,command_id); }
                catch(...) { owner.failed=true; throw; }
                if(result<0)owner.failed=true;
                return result;
            };
    }
    if(dispatch.door_is_blocking) {
        candidate->scheduler.spawn_services.door_is_blocking =
            [](void* raw,uintptr_t identity,uint32_t command_id) {
                auto& owner=*static_cast<Impl*>(raw);
                if(!owner.dispatch.door_is_blocking)return -1;
                int result;
                try { result=owner.dispatch.door_is_blocking(
                    owner.dispatch.context,identity,command_id); }
                catch(...) { owner.failed=true; throw; }
                if(result<0)owner.failed=true;
                return result;
            };
    }
    if(dispatch.dialog_active) {
        candidate->scheduler.spawn_services.dialog_active =
            [](void* raw) {
                auto& owner=*static_cast<Impl*>(raw);
                if(!owner.dispatch.dialog_active)return -1;
                int result;
                try {
                    result=owner.dispatch.dialog_active(
                        owner.dispatch.dialog_context);
                } catch(...) {
                    owner.failed=true;
                    throw;
                }
                if(result<0)owner.failed=true;
                return result;
            };
    }
    impl_ = std::move(candidate);
    error.clear();
    return true;
}


bool SpawnSession::load_level(
        const std::vector<std::uint8_t>& common_names,
        const std::vector<std::uint8_t>& common_programs,
        const std::vector<std::uint8_t>& level_names,
        const std::vector<std::uint8_t>& level_programs,
        const dh2_script_runtime::ObjectSeed* objects, std::uint32_t count,
        const SpawnDispatch& dispatch, std::string& error) {
    return load(common_names, common_programs, level_names, level_programs,
                objects, count, nullptr, nullptr, -1, dispatch, error);
}

bool SpawnSession::activate() {
    return ready() && !impl_->updating && dh2_script_runtime::enter_trigger(&impl_->scheduler);
}
dh2_crypt_spawn_trigger::Status SpawnSession::contact(
        dh2_crypt_spawn_trigger::State& state, const dh2_crypt_spawn_trigger::Frame& frame) {
    if (!ready() || impl_->updating) return dh2_trigger_contact::STATUS_RUNTIME_ERROR;
    const auto result = dh2_crypt_spawn_trigger::update(&impl_->scheduler, &state, &frame);
    if (impl_->scheduler.error != dh2_script_runtime::ERROR_OK) impl_->failed = true;
    return result;
}
dh2_crypt_spawn_trigger::Status SpawnSession::contact(
        dh2_crypt_spawn_trigger::State& state,
        const dh2_crypt_spawn_trigger::SourceFacts& source,
        const dh2_crypt_spawn_trigger::Frame& frame) {
    if (!ready() || impl_->updating) return dh2_trigger_contact::STATUS_RUNTIME_ERROR;
    const auto result = dh2_crypt_spawn_trigger::update(
        &impl_->scheduler, &state, &source, &frame);
    if (impl_->scheduler.error != dh2_script_runtime::ERROR_OK)
        impl_->failed = true;
    return result;
}
bool SpawnSession::supports_script(const char* script_name,
                                   std::int32_t* script_id,
                                   std::string& error) const {
    if (!ready() || !script_name || !*script_name || !script_id) {
        error = "Crypt script support query arguments are invalid";
        return false;
    }
    const auto id = dh2_script_resolve_id(
        impl_->scheduler.common_table, impl_->scheduler.level_table,
        reinterpret_cast<const std::uint8_t*>(script_name),
        static_cast<std::uint32_t>(std::strlen(script_name)), 0);
    if (id < 0) {
        error = "Crypt level script is absent";
        return false;
    }
    const auto common_count = static_cast<std::int32_t>(
        impl_->scheduler.common_table->script_count);
    if (id < common_count) {
        error = "Crypt trigger script must resolve in its level table";
        return false;
    }
    const auto& script = impl_->scheduler.level_table->scripts[id - common_count];
    if (!spawn_program(script,impl_->dispatch)) {
        error = "Crypt script requires an unbound command service";
        return false;
    }
    *script_id = id;
    error.clear();
    return true;
}
TriggerScriptKind SpawnSession::classify_trigger_script(
        const char* script_name, std::int32_t* script_id,
        std::string& error) const {
    if (!ready() || !script_name || !script_id) {
        error = "Crypt script support query arguments are invalid";
        return TriggerScriptKind::unsupported;
    }
    if (!*script_name) {
        *script_id = -1;
        error.clear();
        return TriggerScriptKind::no_script;
    }
    if (!supports_script(script_name, script_id, error))
        return TriggerScriptKind::unsupported;
    return TriggerScriptKind::supported;
}
bool SpawnSession::script_is_running(std::int32_t script_id, bool& running,
                                    std::string& error) const {
    if (!ready()) {
        error = "Crypt ScriptManager session is unavailable";
        return false;
    }
    if (script_id < 0) {
        running = false;
        error.clear();
        return true;
    }
    const auto& runtime = impl_->scheduler;
    const auto common_count = static_cast<std::int32_t>(
        runtime.common_table->script_count);
    const auto total_count = common_count + static_cast<std::int32_t>(
        runtime.level_table->script_count);
    if (script_id >= total_count) {
        error = "Crypt ScriptManager query ID is outside loaded tables";
        return false;
    }
    running = dh2_script_runtime::is_script_running(&runtime, script_id);
    error.clear();
    return true;
}

bool SpawnSession::start_script(std::int32_t script_id, std::int32_t argument,
                                bool check_running, bool online,
                                std::string& error) {
    if (!ready()) {
        error = "Crypt ScriptManager session is unavailable";
        return false;
    }
    if (online) {
        error = "Online Crypt ScriptManager routing is unavailable";
        return false;
    }
    if (script_id < 0) {
        error.clear();
        return true;
    }
    auto& runtime = impl_->scheduler;
    const auto common_count = static_cast<std::int32_t>(
        runtime.common_table->script_count);
    const auto total_count = common_count + static_cast<std::int32_t>(
        runtime.level_table->script_count);
    if (script_id >= total_count) {
        error = "Crypt ScriptManager start ID is outside loaded tables";
        return false;
    }
    const dh2_script& script = script_id < common_count
        ? runtime.common_table->scripts[script_id]
        : runtime.level_table->scripts[script_id - common_count];
    if (!spawn_program(script, impl_->dispatch)) {
        error = "Crypt script requires an unbound command service";
        return false;
    }
    const auto result = dh2_script_runtime::start_script(
        &runtime, script_id, argument, check_running ? 1 : 0, online ? 1 : 0);
    if (result != dh2_script_runtime::ERROR_OK) {
        error = std::string("Crypt ScriptManager start: ") +
            dh2_script_runtime::error_name(result);
        impl_->failed = true;
        return false;
    }
    error.clear();
    return true;
}
bool SpawnSession::script_is_running(const std::string& script_name,
                                    bool& running, std::string& error) const {
    if (!ready() || script_name.size() >= dh2_script_runtime::MAX_NAME_BYTES) {
        error = "Crypt ScriptManager name query is unavailable or out of bounds";
        return false;
    }
    const auto id = dh2_script_resolve_id(
        impl_->scheduler.common_table, impl_->scheduler.level_table,
        reinterpret_cast<const std::uint8_t*>(script_name.data()),
        static_cast<std::uint32_t>(script_name.size()), 1);
    return script_is_running(id, running, error);
}

bool SpawnSession::start_script(const std::string& script_name,
                                std::int32_t argument, bool check_running,
                                bool online,
                                std::string& error) {
    if (!ready() || script_name.size() >= dh2_script_runtime::MAX_NAME_BYTES) {
        error = "Crypt ScriptManager name start is unavailable or out of bounds";
        return false;
    }
    const auto id = dh2_script_resolve_id(
        impl_->scheduler.common_table, impl_->scheduler.level_table,
        reinterpret_cast<const std::uint8_t*>(script_name.data()),
        static_cast<std::uint32_t>(script_name.size()), 1);
    return start_script(id, argument, check_running, online, error);
}
bool SpawnSession::advance(std::uint32_t milliseconds, std::string& error) {
    using namespace dh2_script_runtime;
    if (!ready() || impl_->updating) {
        error = "Crypt script session is unavailable or reentered";
        return false;
    }
    struct Borrow {
        bool& value;
        explicit Borrow(bool& flag) : value(flag) { value = true; }
        ~Borrow() { value = false; }
    } borrow(impl_->updating);
    const auto code = dh2_script_runtime::advance(&impl_->scheduler, milliseconds);
    if (code != ERROR_OK) {
        impl_->failed = true;
        error = std::string("Crypt script advance: ") + error_name(code);
        return false;
    }
    while (impl_->consumed_events < impl_->scheduler.event_count) {
        const auto event = impl_->scheduler.events[impl_->consumed_events++];
        if (event.type == EVENT_UNSUPPORTED_COMMAND) {
            impl_->failed = true;
            error = "Crypt script emitted an unsupported command";
            return false;
        }
    }
    error.clear();
    return true;
}
bool SpawnSession::ready() const { return impl_ && !impl_->failed; }
bool SpawnSession::running() const {
    if (!ready()) return false;
    for (std::uint32_t i = 0; i < impl_->scheduler.task_slots_used; ++i)
        if (impl_->scheduler.tasks[i].active) return true;
    return false;
}
const dh2_script_runtime::Runtime* SpawnSession::runtime() const {
    return impl_ ? &impl_->scheduler : nullptr;
}
void SpawnSession::supported_level_scripts(std::uint32_t& supported, std::uint32_t& total,
                                           std::string* blockers) const {
    supported = total = 0;
    if (!impl_ || !impl_->scheduler.level_table) return;
    const auto& table = *impl_->scheduler.level_table;
    std::map<std::uint32_t, std::uint32_t> first_blocker;
    for (std::uint32_t i = 0; i < table.script_count; ++i) {
        ++total;
        const auto& script = table.scripts[i];
        if (spawn_program(script, impl_->dispatch)) { ++supported; continue; }
        // Find the first single command that fails the provider check.
        for (std::uint32_t c = 0; c < script.command_count; ++c) {
            dh2_script one = script;
            one.commands = &script.commands[c];
            one.command_count = 1;
            if (!spawn_program(one, impl_->dispatch)) {
                ++first_blocker[script.commands[c].command_id];
                break;
            }
        }
    }
    if (blockers) {
        blockers->clear();
        for (const auto& [id, count] : first_blocker)
            *blockers += std::to_string(id) + "x" + std::to_string(count) + " ";
    }
}
std::uint32_t SpawnSession::dispatch_count() const { return impl_ ? impl_->dispatched : 0; }
void SpawnSession::clear() {
    if (impl_ && impl_->updating) return; // Retained borrowed table lifetime.
    impl_.reset();
}
} // namespace dh2::character::crypt_scripts
