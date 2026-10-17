#include "../pydata-scripts/native/pydata_scripts.h"
#include "../script-runtime/script_runtime.hpp"
#include "../world-data/world.hpp"

#include <jni.h>
#include <cerrno>
#include <climits>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {
using dh2_script_runtime::Runtime;
using dh2_script_runtime::Event;
using dh2_script_runtime::ObjectSeed;

constexpr std::size_t kMaxMgpBytes = 8u * 1024u * 1024u;
constexpr std::uint32_t kSimulationStepMs = 10;

struct Bytes {
    std::uint8_t* data;
    std::uint32_t size;
};

struct TraceSession {
    dh2_script_table common;
    dh2_script_table swamp;
    Runtime runtime;
    bool started;
};

TraceSession* g_session = nullptr;

bool copy_java(JNIEnv* env, jbyteArray array, std::uint32_t limit, Bytes* output) {
    if (!env || !array || !output) return false;
    const jsize length = env->GetArrayLength(array);
    if (length <= 0 || static_cast<std::uint32_t>(length) > limit) return false;
    output->data = static_cast<std::uint8_t*>(std::malloc(static_cast<std::size_t>(length)));
    if (!output->data) return false;
    output->size = static_cast<std::uint32_t>(length);
    env->GetByteArrayRegion(array, 0, length, reinterpret_cast<jbyte*>(output->data));
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        std::free(output->data);
        *output = {};
        return false;
    }
    return true;
}

jstring string(JNIEnv* env, const char* value) {
    return env->NewStringUTF(value ? value : "SWAMP trace: unknown error");
}

void destroy_session(TraceSession* session) {
    if (!session) return;
    dh2_script_table_destroy(&session->common);
    dh2_script_table_destroy(&session->swamp);
    std::free(session);
}

bool parse_i32(const char* source, std::int32_t* value) {
    if (!source || !*source || !value) return false;
    errno = 0;
    char* end = nullptr;
    const long parsed = std::strtol(source, &end, 10);
    if (errno || end == source || *end || parsed < INT32_MIN || parsed > INT32_MAX)
        return false;
    *value = static_cast<std::int32_t>(parsed);
    return true;
}

bool copy_field(char* output, std::size_t capacity, const char* value) {
    if (!output || !capacity || !value) return false;
    const std::size_t size = std::strlen(value);
    if (size >= capacity) return false;
    std::memcpy(output, value, size + 1);
    return true;
}

const char* event_name(dh2_script_runtime::EventType type) {
    using namespace dh2_script_runtime;
    switch (type) {
        case EVENT_TRIGGER_STARTED: return "Trigger manually activated";
        case EVENT_TRIGGER_REJECTED: return "Trigger rejected";
        case EVENT_TRIGGER_SCRIPT_ALREADY_RUNNING: return "Duplicate trigger suppressed";
        case EVENT_SCRIPT_STARTED: return "Script started";
        case EVENT_SCRIPT_COMPLETED: return "Script completed";
        case EVENT_WAIT_STARTED: return "Wait";
        case EVENT_PLAYER_LOCK_CHANGED: return "Player lock";
        case EVENT_CAMERA_TARGET_CHANGED: return "Camera target request (visual no-op)";
        case EVENT_CUTSCENE_MODE_CHANGED: return "Cutscene flag (visual no-op)";
        case EVENT_OBJECT_LOOKUP_MISS: return "Object lookup miss";
        case EVENT_CHARACTER_SPAWN_STATE_REQUESTED: return "Spawn state 1 requested (no actor allocation/render)";
        case EVENT_TUTORIAL_GATED: return "Tutorial gate closed";
        case EVENT_UNRESOLVED_SCRIPT: return "Unresolved script";
        case EVENT_UNSUPPORTED_COMMAND: return "Unsupported command (explicit no-op)";
        case EVENT_EXEC_DUPLICATE_SUPPRESSED: return "Duplicate script suppressed";
        default: return "Runtime event";
    }
}

bool all_tasks_complete(const Runtime& runtime) {
    for (std::uint32_t i = 0; i < runtime.task_slots_used; ++i)
        if (runtime.tasks[i].active) return false;
    return true;
}

const dh2_script_runtime::Task* lizard_task(const Runtime& runtime) {
    for (std::uint32_t i = 0; i < runtime.task_slots_used; ++i) {
        const auto& task = runtime.tasks[i];
        if (task.active && task.script_id == runtime.trigger_script_id) return &task;
    }
    return nullptr;
}

const char* actor_state(const dh2_script_runtime::ObjectRecord* object) {
    if (!object) return "missing source record";
    switch (object->state) {
        case dh2_script_runtime::CHARACTER_STATE_LIMBUS: return "Limbus (preloaded)";
        case dh2_script_runtime::CHARACTER_STATE_NATIVE_1_REQUESTED:
            return "native state 1 requested; no actor allocation/render";
        default: return "unknown";
    }
}

void append(char* output, std::size_t capacity, std::size_t* used,
            const char* format, ...) {
    if (!output || !used || *used >= capacity) return;
    va_list args;
    va_start(args, format);
    const int written = std::vsnprintf(output + *used, capacity - *used, format, args);
    va_end(args);
    if (written < 0) return;
    const std::size_t size = static_cast<std::size_t>(written);
    *used += size < capacity - *used ? size : capacity - *used - 1;
}

jstring snapshot(JNIEnv* env, const TraceSession* session) {
    if (!session) return string(env, "LizardMan_Intro trace is not active.");
    const Runtime& runtime = session->runtime;
    char output[8192]{};
    std::size_t used = 0;
    const auto* task = lizard_task(runtime);
    const auto* actor1 = dh2_script_runtime::find_object(
        &runtime, "_prim_Monster_LizManIntro1");
    const auto* actor2 = dh2_script_runtime::find_object(
        &runtime, "_prim_Monster_LizManIntro2");
    const auto* local_player_lookup = dh2_script_runtime::find_event(
        &runtime, dh2_script_runtime::EVENT_OBJECT_LOOKUP_MISS,
        runtime.trigger_script_id, 8, "LocalPlayer", 0);
    append(output, sizeof(output), &used,
        "LizardMan_Intro · explicit developer trace\n"
        "Manual activation only; no collision trigger\n"
        "Time %llu ms · trigger %u/1 %s · %u module-one records\n",
        static_cast<unsigned long long>(runtime.time_ms),
        runtime.trigger_activations,
        runtime.trigger_fired ? "fired" : "not fired", runtime.object_count);
    if (task) {
        append(output, sizeof(output), &used,
            "Script ID %d · PC %u · %s",
            task->script_id, task->pc,
            task->waiting ? "waiting" : "runnable");
        if (task->waiting) {
            append(output, sizeof(output), &used, " %u/%u ms",
                task->wait_elapsed_ms, task->wait_duration_ms);
        }
        append(output, sizeof(output), &used, "\n");
    } else {
        append(output, sizeof(output), &used, "LizardMan_Intro task complete\n");
    }
    append(output, sizeof(output), &used,
        "Player lock: %s (logical) · cutscene: %s (logical)\n",
        runtime.player_locked ? "ON" : "OFF",
        runtime.cutscene_mode ? "ON" : "OFF");
    append(output, sizeof(output), &used,
        "Camera target: %s (camera movement not implemented)\n",
        runtime.camera_target[0] ? runtime.camera_target : "none");
    if (local_player_lookup) {
        append(output, sizeof(output), &used,
            "Return camera request: LocalPlayer unresolved; this preview has no native player Character\n");
    }
    append(output, sizeof(output), &used,
        "LizManIntro1: %s\nLizManIntro2: %s\n",
        actor_state(actor1), actor_state(actor2));
    append(output, sizeof(output), &used,
        "End: %s · Return to encounter preserves its authored state\n",
        all_tasks_complete(runtime) ? "complete" : "running");
    append(output, sizeof(output), &used,
        "Events (latest first; visuals/unsupported effects are marked no-op):\n");
    const std::uint32_t count = runtime.event_count;
    const std::uint32_t first = count > 24 ? count - 24 : 0;
    for (std::uint32_t i = count; i > first; --i) {
        const Event& event = runtime.events[i - 1];
        append(output, sizeof(output), &used, "[%04llu] %s",
            static_cast<unsigned long long>(event.time_ms), event_name(event.type));
        if (event.detail[0]) append(output, sizeof(output), &used, ": %s", event.detail);
        if (event.type == dh2_script_runtime::EVENT_WAIT_STARTED)
            append(output, sizeof(output), &used, " (%d ms)", event.value);
        if (event.type == dh2_script_runtime::EVENT_PLAYER_LOCK_CHANGED)
            append(output, sizeof(output), &used, " (%s)", event.value ? "ON" : "OFF");
        if (event.type == dh2_script_runtime::EVENT_CUTSCENE_MODE_CHANGED)
            append(output, sizeof(output), &used, " (%s)", event.value ? "ON" : "OFF");
        append(output, sizeof(output), &used, "\n");
    }
    if (runtime.error != dh2_script_runtime::ERROR_OK) {
        append(output, sizeof(output), &used, "ERROR: %s\n",
            dh2_script_runtime::error_name(runtime.error));
    }
    return string(env, output);
}

bool read_trigger_and_seeds(dh2::world::SourceLevel* level,
                            ObjectSeed* seeds, std::uint32_t* seed_count,
                            char* trigger_name, char* trigger_script,
                            std::int32_t* trigger_count,
                            std::int32_t* trigger_delay,
                            char* error, std::size_t error_capacity) {
    const dh2::world::Object* trigger = nullptr;
    *seed_count = 0;
    for (std::uint32_t i = 0; i < level->entity_count; ++i) {
        const auto& object = level->entities[i];
        if (object.module_index != 1 || object.kind != dh2::world::RecordKind::mgp)
            continue;
        if (*seed_count >= dh2_script_runtime::MAX_OBJECTS) {
            copy_field(error, error_capacity, "Module-one MGP exceeds the scheduler object bound.");
            return false;
        }
        ObjectSeed& seed = seeds[*seed_count];
        std::memset(&seed, 0, sizeof(seed));
        if (!copy_field(seed.name, sizeof(seed.name), object.name) ||
            !copy_field(seed.gametype, sizeof(seed.gametype), object.gametype)) {
            copy_field(error, error_capacity, "Module-one object name or type exceeds scheduler bounds.");
            return false;
        }
        const char* ai_state = dh2_world_field(&object, "ai_state");
        const char* auto_spawn = dh2_world_field(&object, "auto_spawn");
        if (ai_state && !copy_field(seed.ai_state, sizeof(seed.ai_state), ai_state)) {
            copy_field(error, error_capacity, "Module-one AI state exceeds scheduler bounds.");
            return false;
        }
        seed.auto_spawn = auto_spawn && std::strcmp(auto_spawn, "1") == 0;
        ++*seed_count;
        if (std::strcmp(object.name, "_prim_TriggerZone_LizManIntro") == 0) {
            if (trigger) {
                copy_field(error, error_capacity, "Module one contains duplicate Lizard intro triggers.");
                return false;
            }
            trigger = &object;
        }
    }
    if (!trigger || std::strcmp(trigger->gametype, "TriggerZone") != 0 ||
        !copy_field(trigger_name, dh2_script_runtime::MAX_NAME_BYTES, trigger->name)) {
        copy_field(error, error_capacity, "The source Lizard intro TriggerZone record is missing or invalid.");
        return false;
    }
    const char* script = dh2_world_field(trigger, "script");
    const char* count = dh2_world_field(trigger, "triggercount");
    const char* delay = dh2_world_field(trigger, "triggerdelay");
    if (!script || std::strcmp(script, "LizardMan_Intro") != 0 ||
        !parse_i32(count, trigger_count) || !parse_i32(delay, trigger_delay) ||
        *trigger_count != 1 || *trigger_delay != 0 ||
        !copy_field(trigger_script, dh2_script_runtime::MAX_NAME_BYTES, script)) {
        copy_field(error, error_capacity, "Source trigger properties do not match one-shot LizardMan_Intro (count 1, delay 0).");
        return false;
    }
    return *seed_count > 0;
}

}  // namespace

extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_SwampPreviewActivity_startIntroTrace(
        JNIEnv* env, jclass, jbyteArray common_names_array,
        jbyteArray common_programs_array, jbyteArray swamp_names_array,
        jbyteArray swamp_programs_array, jbyteArray mlx_array,
        jbyteArray module_mgp_array) {
    if (g_session) return string(env, "LizardMan_Intro trace is already active.");
    Bytes common_names{}, common_programs{}, swamp_names{}, swamp_programs{}, mlx{}, mgp{};
    bool copied = copy_java(env, common_names_array, DH2_SCRIPT_MAX_TABLE_BYTES, &common_names) &&
        copy_java(env, common_programs_array, DH2_SCRIPT_MAX_TABLE_BYTES, &common_programs) &&
        copy_java(env, swamp_names_array, DH2_SCRIPT_MAX_TABLE_BYTES, &swamp_names) &&
        copy_java(env, swamp_programs_array, DH2_SCRIPT_MAX_TABLE_BYTES, &swamp_programs) &&
        copy_java(env, mlx_array, 32u * 1024u * 1024u, &mlx) &&
        copy_java(env, module_mgp_array, static_cast<std::uint32_t>(kMaxMgpBytes), &mgp);
    Bytes* all_input_bytes[] = {&common_names, &common_programs, &swamp_names,
        &swamp_programs, &mlx, &mgp};
    if (!copied) {
        for (std::size_t i = 0; i < sizeof(all_input_bytes) / sizeof(all_input_bytes[0]); ++i)
            std::free(all_input_bytes[i]->data);
        return string(env, "Source script tables or module-one MGP could not be read within bounds.");
    }
    auto* session = static_cast<TraceSession*>(std::calloc(1, sizeof(TraceSession)));
    if (!session) {
        for (std::size_t i = 0; i < sizeof(all_input_bytes) / sizeof(all_input_bytes[0]); ++i)
            std::free(all_input_bytes[i]->data);
        return string(env, "Not enough memory for bounded LizardMan_Intro trace state.");
    }
    dh2_script_error script_error{};
    const auto common_result = dh2_script_table_decode(
        common_names.data, common_names.size, common_programs.data,
        common_programs.size, &session->common, &script_error);
    const auto common_error = script_error;
    auto swamp_result = DH2_SCRIPT_OK;
    if (common_result == DH2_SCRIPT_OK) {
        swamp_result = dh2_script_table_decode(
            swamp_names.data, swamp_names.size, swamp_programs.data,
            swamp_programs.size, &session->swamp, &script_error);
    }
    for (std::size_t i = 0; i < 4; ++i) std::free(all_input_bytes[i]->data);
    if (common_result != DH2_SCRIPT_OK || swamp_result != DH2_SCRIPT_OK) {
        const bool failed_common = common_result != DH2_SCRIPT_OK;
        const dh2_script_error& issue = failed_common ? common_error : script_error;
        char message[192]{};
        std::snprintf(message, sizeof(message), "Source script parse failed: %s at byte %u.",
            dh2_script_error_name(failed_common ? common_result : swamp_result), issue.offset);
        std::free(mlx.data); std::free(mgp.data); destroy_session(session);
        return string(env, message);
    }

    dh2::world::SourceLevel level{};
    dh2::world::Diagnostic world_error{};
    bool world_ok = dh2_world_import_level(&level, "SWAMP", "data/scene/001_swamp.mlx",
        mlx.data, mlx.size, &world_error) == dh2::world::Error::ok;
    if (world_ok && (level.module_count < 2 ||
        std::strcmp(level.modules[1].cache_mgp,
            "data/3d/modules/swamp/mgp/obj_3of4_brdwalk_sw_00.mgp") != 0)) {
        world_ok = false;
        std::snprintf(world_error.message, sizeof(world_error.message),
            "MLX module one does not resolve to the supplied Lizard intro MGP.");
    }
    if (world_ok) {
        world_ok = dh2_world_import_module_objects(&level, 1,
            dh2::world::RecordKind::mgp,
            "data/3d/modules/swamp/mgp/obj_3of4_brdwalk_sw_00.mgp",
            mgp.data, mgp.size, &world_error) == dh2::world::Error::ok;
    }
    std::free(mlx.data);
    std::free(mgp.data);
    if (!world_ok) {
        char message[256]{};
        std::snprintf(message, sizeof(message), "Module-one MGP source parse failed: %s.",
            world_error.message[0] ? world_error.message : "unknown source error");
        dh2_world_free(&level); destroy_session(session);
        return string(env, message);
    }

    ObjectSeed seeds[dh2_script_runtime::MAX_OBJECTS]{};
    std::uint32_t seed_count = 0;
    char trigger_name[dh2_script_runtime::MAX_NAME_BYTES]{};
    char trigger_script[dh2_script_runtime::MAX_NAME_BYTES]{};
    std::int32_t trigger_count = 0, trigger_delay = 0;
    char seed_error[192]{};
    const bool seeds_ok = read_trigger_and_seeds(&level, seeds, &seed_count,
        trigger_name, trigger_script, &trigger_count, &trigger_delay,
        seed_error, sizeof(seed_error));
    dh2_world_free(&level);
    if (!seeds_ok) {
        destroy_session(session);
        return string(env, seed_error);
    }
    dh2_script_runtime::Options options{};
    // The rendered entrypoint warrior is only a preview pose, not a native
    // player Character. Keep native player/tutorial gates closed.
    const auto initialized = dh2_script_runtime::init(&session->runtime,
        &session->common, &session->swamp, seeds, seed_count,
        trigger_name, trigger_script, trigger_count, trigger_delay, &options);
    if (initialized != dh2_script_runtime::ERROR_OK) {
        char message[192]{};
        std::snprintf(message, sizeof(message), "Scheduler initialization failed: %s.",
            dh2_script_runtime::error_name(initialized));
        destroy_session(session);
        return string(env, message);
    }
    if (!dh2_script_runtime::enter_trigger(&session->runtime) ||
        dh2_script_runtime::advance(&session->runtime, 0) !=
            dh2_script_runtime::ERROR_OK) {
        char message[192]{};
        std::snprintf(message, sizeof(message), "Trigger start failed: %s.",
            dh2_script_runtime::error_name(session->runtime.error));
        destroy_session(session);
        return string(env, message);
    }
    session->started = true;
    g_session = session;
    return snapshot(env, g_session);
}

extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_SwampPreviewActivity_advanceIntroTrace(
        JNIEnv* env, jclass, jint delta_ms) {
    if (!g_session) return string(env, "LizardMan_Intro trace is not active.");
    if (delta_ms < 0 || delta_ms > 1000 || delta_ms % kSimulationStepMs != 0)
        return string(env, "Trace update is outside the bounded 10 ms simulation step.");
    for (jint elapsed = 0; elapsed < delta_ms;
         elapsed += static_cast<jint>(kSimulationStepMs)) {
        const auto result = dh2_script_runtime::advance(
            &g_session->runtime, kSimulationStepMs);
        if (result != dh2_script_runtime::ERROR_OK) {
            dh2_script_runtime::Runtime& runtime = g_session->runtime;
            if (runtime.error == dh2_script_runtime::ERROR_OK) runtime.error = result;
            break;
        }
    }
    return snapshot(env, g_session);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_SwampPreviewActivity_destroyIntroTrace(JNIEnv*, jclass) {
    destroy_session(g_session);
    g_session = nullptr;
}
