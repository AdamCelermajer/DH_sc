#ifndef DH2_SCRIPT_RUNTIME_HPP
#define DH2_SCRIPT_RUNTIME_HPP

#include "../pydata-scripts/native/pydata_scripts.h"

#include <stdint.h>

namespace dh2_script_runtime {

enum {
    MAX_OBJECTS = 256,
    MAX_TASKS = 16,
    MAX_DEPTH = 8,
    MAX_EVENTS = 512,
    MAX_STEPS_PER_ADVANCE = 2048,
    MAX_NAME_BYTES = 96,
    MAX_TUTORIAL_FLAGS = 16
};

enum Error {
    ERROR_OK = 0,
    ERROR_INVALID_ARGUMENT,
    ERROR_CAPACITY,
    ERROR_BAD_SCRIPT,
    ERROR_DEPTH_LIMIT,
    ERROR_STEP_LIMIT,
    ERROR_TIME_OVERFLOW,
    ERROR_TASK_TICKET_OVERFLOW,
    ERROR_EVENT_CAPACITY,
    ERROR_CHARACTER_SERVICE,
    ERROR_CAMERA_SERVICE,
    ERROR_ACTOR_SERVICE,
    ERROR_DIALOG_SERVICE,
    ERROR_ACTOR_ANIMATION_SERVICE,
    ERROR_UNSUPPORTED_ONLINE,
    ERROR_CHAR_MENU_TUTORIAL_SERVICE,
    ERROR_DOOR_SERVICE,
    ERROR_HOST_COMMAND_SERVICE
};

enum EventType {
    EVENT_TRIGGER_STARTED = 1,
    EVENT_TRIGGER_REJECTED,
    EVENT_TRIGGER_SCRIPT_ALREADY_RUNNING,
    EVENT_SCRIPT_STARTED,
    EVENT_SCRIPT_COMPLETED,
    EVENT_WAIT_STARTED,
    EVENT_PLAYER_LOCK_CHANGED,
    EVENT_CAMERA_TARGET_CHANGED,
    EVENT_CUTSCENE_MODE_CHANGED,
    EVENT_OBJECT_LOOKUP_MISS,
    EVENT_CHARACTER_SPAWN_STATE_REQUESTED,
    EVENT_TUTORIAL_GATED,
    EVENT_UNRESOLVED_SCRIPT,
    EVENT_UNSUPPORTED_COMMAND,
    EVENT_EXEC_DUPLICATE_SUPPRESSED
};

enum CharacterState {
    CHARACTER_STATE_UNKNOWN = 0,
    CHARACTER_STATE_LIMBUS = 1,
    CHARACTER_STATE_NATIVE_1_REQUESTED = 2
};

struct ObjectSeed {
    char name[MAX_NAME_BYTES];
    char gametype[32];
    char ai_state[32];
    uint8_t auto_spawn;
};

struct ObjectRecord {
    char name[MAX_NAME_BYTES];
    char gametype[32];
    char initial_ai_state[32];
    uint8_t is_character;
    uint8_t auto_spawn;
    uint8_t spawn_state_requested;
    uint32_t transition_count;
    CharacterState state;
    // Optional canonical ObjectManager identity resolved on demand by the
    // live Level provider. These are provenance fields, not another registry.
    uintptr_t identity;
    int32_t source_handle;
    int32_t module_id;
    uint8_t global_object;
};

struct Options {
    uint8_t local_player_has_character;
    uint8_t online;
    int32_t game_difficulty;
    uint8_t tutorial_pending[MAX_TUTORIAL_FLAGS];
};

struct Event {
    EventType type;
    uint64_t time_ms;
    int32_t script_id;
    uint32_t program_counter;
    uint32_t command_id;
    int32_t value;
    char detail[MAX_NAME_BYTES];
};

struct SpawnServices {
    void* context;
    // Runs synchronously at the original SpawnCharacter command boundary.
    // >0 accepted, 0 source lookup miss, <0 bound Character service failure.
    int (*request)(void*, const Event&);
    // Executes the source PlayCamera command at its scheduler boundary.
    // use_crit selects the selected CamAnimSet Crit; otherwise clip_id is
    // resolved as an AnimDict ID in that same selected set.
    int (*play_camera)(void*, uint8_t use_crit, int32_t clip_id);
    // Optional live ObjectManager lookup. Returns 1 for an exact canonical
    // row, 0 for source lookup miss, and -1 when identity validation fails.
    int (*lookup_object)(void*, const char*, ObjectRecord*);
    // Source actor commands are dispatched synchronously against the active
    // ObjectManager/Character owners. Names preserve command payload order
    // after decoding: actor_name is the acted-on object, target_name the
    // position/look target.
    struct ActorCommandRequest {
        uint32_t command_id;
        char actor_name[MAX_NAME_BYTES];
        char target_name[MAX_NAME_BYTES];
    };
    // >0 accepted, 0 canonical ObjectManager name miss, <0 provider failure.
    int (*actor_command)(void*, const ActorCommandRequest*);
    struct DialogRequest {
        int32_t actor_id;
        int32_t style_id;
        int32_t text_string_id;
    };
    struct DialogIdRequest {
        int32_t dialog_id;
    };
    // StartDialog executes at its source scheduler boundary. A zero result is
    // a valid source flush/no-enqueue branch; negative is a provider failure.
    int (*start_dialog)(void*, const DialogRequest*);
    // StartDialogID resolves every row from the source Dialogs table, then
    // appends those rows to the same retained DialogMsg queue.
    int (*start_dialog_id)(void*, const DialogIdRequest*);
    // WaitDialog::IsBlocking reads the one active Level's DialogMsg queue.
    // Returns 1 while nonempty, 0 when empty, and -1 when stale/unavailable.
    int (*dialog_active)(void*);
    struct ActorAnimationRequest {
        int32_t anim_dictionary_a;
        int32_t anim_dictionary_b;
        int32_t animation_type;
        char actor_name[MAX_NAME_BYTES];
    };
    // Executes PlayActorAnim against one canonical Character and returns its
    // retained identity for the subsequent source IsBlocking polls.
    int (*play_actor_animation)(void*, const ActorAnimationRequest*, uintptr_t*);
    // Source IsBlocking checks the retained ObjectHandle and current animator
    // state. Returns 1 while the requested state remains active, 0 otherwise.
    int (*actor_animation_active)(void*, uintptr_t, int32_t);
    struct CharMenuTutorialRequest {
        char frame[MAX_NAME_BYTES];
        char menu[MAX_NAME_BYTES];
        int32_t string_id;
    };
    int (*enqueue_char_menu_tutorial)(void*, const CharMenuTutorialRequest*);
    int (*skip_char_menu_tutorial)(void*, uint8_t skip_all);
    struct DoorCommandRequest {
        uint32_t command_id;
        char door_name[MAX_NAME_BYTES];
        uint8_t force;
    };
    // Door commands target the canonical ObjectManager row. The returned
    // identity is retained on the script task for the source IsBlocking poll.
    int (*door_command)(void*, const DoorCommandRequest*, uintptr_t*);
    int (*door_is_blocking)(void*, uintptr_t, uint32_t command_id);
    // Generic host boundary for decoded commands without a typed service above
    // (actor visibility, FX, sound, Flash, Faery, level flow, ...). The host
    // implements each against its canonical owners. Returns 1 executed,
    // 2 executed and blocking (handle polled by host_command_blocking),
    // 0 source lookup miss, -1 provider failure, -2 not implemented.
    int (*host_command)(void*, const dh2_script_command*, uintptr_t* handle);
    // 1 while the command still blocks the task, 0 when done, <0 failure.
    int (*host_command_blocking)(void*, uintptr_t handle, uint32_t command_id);
};

struct Task {
    const dh2_script *script;
    uint64_t ticket;
    int32_t script_id;
    uint32_t pc;
    uint32_t wait_duration_ms;
    uint32_t wait_elapsed_ms;
    int32_t parent_script_id;
    // ScriptManager::StartScript(id, arg, check) arguments are retained on
    // the same task record. The selected interpreter subset may reject the
    // script before activation when its commands lack providers.
    int32_t start_argument;
    uint8_t depth;
    uint8_t active;
    uint8_t waiting;
    uint8_t waiting_for_dialog;
    uint8_t start_check_running;
    uint8_t waiting_for_actor_animation;
    uintptr_t actor_animation_identity;
    int32_t actor_animation_state;
    uint8_t waiting_for_door;
    uintptr_t door_identity;
    uint32_t door_command_id;
    uint8_t waiting_for_host;
    uintptr_t host_handle;
    uint32_t host_command_id;
};

struct Runtime {
    const dh2_script_table *common_table;
    const dh2_script_table *level_table;
    ObjectRecord objects[MAX_OBJECTS];
    uint32_t object_count;
    Task tasks[MAX_TASKS];
    uint32_t task_slots_used;
    uint64_t next_task_ticket;
    Event events[MAX_EVENTS];
    uint32_t event_count;
    Options options;
    uint64_t time_ms;
    int32_t trigger_count;
    uint32_t trigger_activations;
    int32_t trigger_script_id;
    char trigger_name[MAX_NAME_BYTES];
    uint8_t trigger_fired;
    uint8_t player_locked;
    uint8_t cutscene_mode;
    uint8_t camera_target_is_player;
    char camera_target[MAX_NAME_BYTES];
    SpawnServices spawn_services;
    ObjectRecord lookup_scratch;
    Error error;
};

void init_options(Options *options);

Error init(Runtime *runtime,
           const dh2_script_table *common_table,
           const dh2_script_table *level_table,
           const ObjectSeed *object_seeds,
           uint32_t object_count,
           const char *trigger_name,
           const char *trigger_script_name,
           int32_t trigger_count,
           int32_t trigger_delay_ms,
           const Options *options);

/* A successful return means this trigger activation was consumed. The native
 * TriggerZone increments its activation count even when SafeStart finds the
 * script already running; the configured count still gates later activations. */
bool enter_trigger(Runtime *runtime);
/* Starts a source-named TriggerZone script in the same level ScriptManager.
 * Per-trigger activation counters live with the TriggerZone GameObject while
 * tasks, tables, and time remain shared by the single Runtime owner. */
bool enter_trigger_for(Runtime *runtime, int32_t script_id,
                       const char *trigger_name, int32_t trigger_count,
                       uint32_t *trigger_activations,
                       uint8_t *trigger_fired);
/* ScriptManager adapters over this Runtime's single task table. Negative IDs
 * represent source GetIDFromName misses: running=false and start is a
 * successful no-op. StartScript's check-running guard is online-only; this
 * offline runtime rejects online dispatch because network routing is absent. */
bool is_script_running(const Runtime *runtime, int32_t script_id);
Error start_script(Runtime *runtime, int32_t script_id, int32_t argument,
                   uint8_t check_running, uint8_t online);

/* Advances game time once and executes only tasks present at update start.
 * A blocking Wait checks prior elapsed, receives this delta, and returns
 * without rechecking. Newly executed positive Waits also receive this delta.
 * Child tasks wait for the next manager update. */
Error advance(Runtime *runtime, uint32_t delta_ms);

const Event *find_event(const Runtime *runtime,
                        EventType type,
                        int32_t script_id,
                        uint32_t command_id,
                        const char *detail,
                        uint32_t ordinal);

const ObjectRecord *find_object(const Runtime *runtime, const char *name);
const char *error_name(Error error);

}  // namespace dh2_script_runtime

#endif
