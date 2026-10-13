#include "../script_runtime.hpp"
#include "../../engine-ui/source_char_menu_tutorial_queue_v1.hpp"

#include <cstdint>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace dh2_script_runtime;

static int failures = 0;

struct CameraCall { uint32_t calls; uint8_t use_crit; int32_t clip_id; int result; };
static int camera_dispatch(void *context,uint8_t use_crit,int32_t clip_id) {
    CameraCall *call=(CameraCall *)context;
    if(call==NULL)return -1;
    ++call->calls;call->use_crit=use_crit;call->clip_id=clip_id;
    return call->result;
}

static void check(bool condition, const char *label) {
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", label);
        ++failures;
    }
}

static void test_play_camera_command() {
    const uint8_t script_name_bytes[]={'C','a','m'};
    dh2_script_command command={};
    command.command_id=5;command.field_count=2;
    command.fields[0].kind=DH2_SCRIPT_VALUE_I32;
    command.fields[0].value.i32=44;
    command.fields[1].kind=DH2_SCRIPT_VALUE_BOOL;
    command.fields[1].value.boolean=1;
    dh2_script script={};
    script.command_count=1;script.name={script_name_bytes,3};script.commands=&command;
    dh2_script_table common={};common.script_count=1;common.scripts=&script;
    dh2_script_table level={};
    CameraCall call={0,0,0,0};
    Runtime runtime={};runtime.common_table=&common;runtime.level_table=&level;
    runtime.spawn_services.context=&call;
    runtime.spawn_services.play_camera=camera_dispatch;
    runtime.tasks[0].script=&script;runtime.tasks[0].ticket=1;
    runtime.tasks[0].script_id=0;runtime.tasks[0].active=1;
    runtime.task_slots_used=1;runtime.next_task_ticket=1;
    check(advance(&runtime,16)==ERROR_OK,"PlayCamera callback completes in the shared scheduler");
    check(call.calls==1&&call.use_crit==0&&call.clip_id==44,
          "PlayCamera passes the not-skipping bool and its external AnimDict ID");

    CameraCall rejected={0,0,0,-1};
    Runtime failed={};failed.common_table=&common;failed.level_table=&level;
    failed.spawn_services.context=&rejected;
    failed.spawn_services.play_camera=camera_dispatch;
    failed.tasks[0].script=&script;failed.tasks[0].ticket=1;
    failed.tasks[0].script_id=0;failed.tasks[0].active=1;
    failed.task_slots_used=1;failed.next_task_ticket=1;
    check(advance(&failed,16)==ERROR_CAMERA_SERVICE&&rejected.calls==1,
          "PlayCamera provider failure stops the same scheduler at its command boundary");
}

struct ActorCommandCall {
    uint32_t calls;
    uint32_t command_id;
    char actor_name[MAX_NAME_BYTES];
    char target_name[MAX_NAME_BYTES];
    int result;
};
static int actor_command_dispatch(void *context,
        const SpawnServices::ActorCommandRequest *request) {
    ActorCommandCall *call=(ActorCommandCall *)context;
    if(call==NULL||request==NULL)return -1;
    ++call->calls;
    call->command_id=request->command_id;
    memcpy(call->actor_name,request->actor_name,sizeof(call->actor_name));
    memcpy(call->target_name,request->target_name,sizeof(call->target_name));
    return call->result;
}

static void test_actor_command(uint32_t command_id) {
    const uint8_t script_name_bytes[]={'A','c','t','o','r'};
    const uint8_t target_bytes[]={'W','a','y','p','o','i','n','t'};
    const uint8_t actor_bytes[]={'L','o','c','a','l','P','l','a','y','e','r'};
    dh2_script_command command={};
    command.command_id=command_id;command.field_count=2;
    command.fields[0].kind=DH2_SCRIPT_VALUE_STRING;
    command.fields[0].value.string={target_bytes,sizeof(target_bytes)};
    command.fields[1].kind=DH2_SCRIPT_VALUE_STRING;
    command.fields[1].value.string={actor_bytes,sizeof(actor_bytes)};
    dh2_script script={};
    script.command_count=1;script.name={script_name_bytes,sizeof(script_name_bytes)};
    script.commands=&command;
    dh2_script_table common={};common.script_count=1;common.scripts=&script;
    dh2_script_table level={};
    ActorCommandCall call={};call.result=1;
    Runtime runtime={};runtime.common_table=&common;runtime.level_table=&level;
    runtime.spawn_services.context=&call;
    runtime.spawn_services.actor_command=actor_command_dispatch;
    runtime.tasks[0].script=&script;runtime.tasks[0].ticket=1;
    runtime.tasks[0].script_id=0;runtime.tasks[0].active=1;
    runtime.task_slots_used=1;runtime.next_task_ticket=1;
    check(advance(&runtime,16)==ERROR_OK,
          "synchronous actor command completes through shared scheduler");
    check(call.calls==1&&call.command_id==command_id&&
          strcmp(call.actor_name,"LocalPlayer")==0&&
          strcmp(call.target_name,"Waypoint")==0,
          "actor command preserves source target-first payload order");

    ActorCommandCall rejected={};rejected.result=-1;
    Runtime failed={};failed.common_table=&common;failed.level_table=&level;
    failed.spawn_services.context=&rejected;
    failed.spawn_services.actor_command=actor_command_dispatch;
    failed.tasks[0].script=&script;failed.tasks[0].ticket=1;
    failed.tasks[0].script_id=0;failed.tasks[0].active=1;
    failed.task_slots_used=1;failed.next_task_ticket=1;
    check(advance(&failed,16)==ERROR_ACTOR_SERVICE&&rejected.calls==1,
          "actor command provider failure stops the same scheduler at its command boundary");
}

struct ActorAnimationCall {
    std::uint32_t starts=0,polls=0;
    std::int32_t dict_a=-1,dict_b=-1,anim_type=-1;
    char actor_name[MAX_NAME_BYTES]{};
    std::uintptr_t identity=0x1234;
    bool active=true;
};
static int actor_animation_start(void* raw,
        const SpawnServices::ActorAnimationRequest* request,
        std::uintptr_t* identity) {
    auto* call=static_cast<ActorAnimationCall*>(raw);
    if(!call||!request||!identity)return -1;
    ++call->starts;call->dict_a=request->anim_dictionary_a;
    call->dict_b=request->anim_dictionary_b;
    call->anim_type=request->animation_type;
    std::memcpy(call->actor_name,request->actor_name,sizeof(call->actor_name));
    *identity=call->identity;return 1;
}
static int actor_animation_active(void* raw,std::uintptr_t identity,
                                  std::int32_t expected_state) {
    auto* call=static_cast<ActorAnimationCall*>(raw);
    if(!call||identity!=call->identity||expected_state!=call->anim_type+2)return -1;
    ++call->polls;return call->active?1:0;
}
static void test_play_actor_animation_wait() {
    const uint8_t script_name[]={'A','n','i','m'};
    const uint8_t actor_name[]={'N','P','C'};
    dh2_script_command command={};command.command_id=45;command.field_count=5;
    for(unsigned i=0;i<3;++i) {
        command.fields[i].kind=DH2_SCRIPT_VALUE_I32;
        command.fields[i].value.i32=static_cast<int32_t>(31+i);
    }
    command.fields[2].value.i32=6;
    command.fields[3].kind=DH2_SCRIPT_VALUE_STRING;
    command.fields[3].value.string={actor_name,sizeof(actor_name)};
    command.fields[4].kind=DH2_SCRIPT_VALUE_BOOL;
    command.fields[4].value.boolean=1;
    dh2_script script={};script.command_count=1;
    script.name={script_name,sizeof(script_name)};script.commands=&command;
    dh2_script_table common={};common.script_count=1;common.scripts=&script;
    dh2_script_table level={};ActorAnimationCall call{};
    Runtime runtime={};runtime.common_table=&common;runtime.level_table=&level;
    runtime.spawn_services.context=&call;
    runtime.spawn_services.play_actor_animation=actor_animation_start;
    runtime.spawn_services.actor_animation_active=actor_animation_active;
    runtime.tasks[0].script=&script;runtime.tasks[0].ticket=1;
    runtime.tasks[0].script_id=0;runtime.tasks[0].active=1;
    runtime.task_slots_used=1;runtime.next_task_ticket=1;
    check(advance(&runtime,16)==ERROR_OK&&call.starts==1&&call.polls==1&&
          runtime.tasks[0].waiting_for_actor_animation&&runtime.tasks[0].pc==0,
          "blocking PlayActorAnim starts once and polls source state immediately");
    call.active=false;
    check(advance(&runtime,16)==ERROR_OK&&call.starts==1&&call.polls==2&&
          !runtime.tasks[0].active&&runtime.tasks[0].pc==1,
          "PlayActorAnim resumes at next command when source IsBlocking clears");

    command.fields[4].value.boolean=0;ActorAnimationCall nonblocking{};
    Runtime immediate={};immediate.common_table=&common;immediate.level_table=&level;
    immediate.spawn_services.context=&nonblocking;
    immediate.spawn_services.play_actor_animation=actor_animation_start;
    immediate.spawn_services.actor_animation_active=actor_animation_active;
    immediate.tasks[0].script=&script;immediate.tasks[0].ticket=1;
    immediate.tasks[0].script_id=0;immediate.tasks[0].active=1;
    immediate.task_slots_used=1;immediate.next_task_ticket=1;
    check(advance(&immediate,16)==ERROR_OK&&nonblocking.starts==1&&
          nonblocking.polls==0&&!immediate.tasks[0].active,
          "nonblocking PlayActorAnim does not poll or yield");
}

struct DialogCall {
    uint32_t starts;
    uint32_t id_starts;
    uint32_t active_queries;
    int32_t actor_id;
    int32_t style_id;
    int32_t text_string_id;
    int32_t dialog_id;
    int queue_active;
};
static int start_dialog_dispatch(void *context,
        const SpawnServices::DialogRequest *request) {
    DialogCall *call=(DialogCall *)context;
    if(call==NULL||request==NULL)return -1;
    ++call->starts;
    call->actor_id=request->actor_id;
    call->style_id=request->style_id;
    call->text_string_id=request->text_string_id;
    call->queue_active=1;
    return 1;
}
static int start_dialog_id_dispatch(void *context,
        const SpawnServices::DialogIdRequest *request) {
    DialogCall *call=(DialogCall *)context;
    if(call==NULL||request==NULL)return -1;
    ++call->id_starts;call->dialog_id=request->dialog_id;
    call->queue_active=1;return 1;
}
static int dialog_active_dispatch(void *context) {
    DialogCall *call=(DialogCall *)context;
    if(call==NULL)return -1;
    ++call->active_queries;
    return call->queue_active;
}
static void test_source_dialog_wait() {
    const uint8_t script_name_bytes[]={'D','i','a','l','o','g'};
    dh2_script_command commands[3]={};
    commands[0].command_id=10;commands[0].field_count=3;
    for(uint32_t i=0;i<3;++i)commands[0].fields[i].kind=DH2_SCRIPT_VALUE_I32;
    commands[0].fields[0].value.i32=7;
    commands[0].fields[1].value.i32=2;
    commands[0].fields[2].value.i32=341;
    commands[1].command_id=11;commands[1].field_count=1;
    commands[1].fields[0].kind=DH2_SCRIPT_VALUE_I32;
    commands[1].fields[0].value.i32=0;
    commands[2].command_id=12;commands[2].field_count=0;
    dh2_script script={};script.command_count=3;
    script.name={script_name_bytes,sizeof(script_name_bytes)};
    script.commands=commands;
    dh2_script_table common={};common.script_count=1;common.scripts=&script;
    dh2_script_table level={};
    DialogCall call={};
    Runtime runtime={};runtime.common_table=&common;runtime.level_table=&level;
    runtime.spawn_services.context=&call;
    runtime.spawn_services.start_dialog=start_dialog_dispatch;
    runtime.spawn_services.start_dialog_id=start_dialog_id_dispatch;
    runtime.spawn_services.dialog_active=dialog_active_dispatch;
    runtime.tasks[0].script=&script;runtime.tasks[0].ticket=1;
    runtime.tasks[0].script_id=0;runtime.tasks[0].active=1;
    runtime.task_slots_used=1;runtime.next_task_ticket=1;

    check(advance(&runtime,16)==ERROR_OK,
          "StartDialog enqueues through the shared source scheduler");
    check(call.starts==1&&call.actor_id==7&&call.style_id==2&&
          call.text_string_id==341,
          "StartDialog preserves actor/style/text IDs for UI resolution");
    check(call.id_starts==1&&call.dialog_id==0,
          "StartDialogID resolves through the source dialog-ID service");
    check(runtime.tasks[0].active&&runtime.tasks[0].waiting_for_dialog&&
          runtime.tasks[0].pc==2,
          "WaitDialog blocks at its command while source queue is active");
    check(advance(&runtime,16)==ERROR_OK&&runtime.tasks[0].active&&
          runtime.tasks[0].waiting_for_dialog&&runtime.tasks[0].pc==2,
          "active DialogMsg queue keeps the same task and PC suspended");
    call.queue_active=0;
    check(advance(&runtime,16)==ERROR_OK&&!runtime.tasks[0].active&&
          runtime.tasks[0].pc==3,
          "WaitDialog resumes after the source queue becomes empty");
    check(call.active_queries==3,
          "WaitDialog queries the source queue on start and each resumed update");
}

struct TutorialQueueCall {
    uint32_t enqueues{}, skips{}, skip_all_calls{};
    int32_t string_id{-1};
    bool skip_all{};
    char frame[MAX_NAME_BYTES]{},menu[MAX_NAME_BYTES]{};
    uint32_t queue_size{};
};
static int enqueue_char_menu_tutorial_dispatch(void* context,
        const SpawnServices::CharMenuTutorialRequest* request) {
    auto* call=static_cast<TutorialQueueCall*>(context);
    if(!call||!request)return -1;
    ++call->enqueues;call->string_id=request->string_id;
    std::memcpy(call->frame,request->frame,sizeof(call->frame));
    std::memcpy(call->menu,request->menu,sizeof(call->menu));
    ++call->queue_size;return 1;
}
static int skip_char_menu_tutorial_dispatch(void* context,uint8_t skip_all) {
    auto* call=static_cast<TutorialQueueCall*>(context);
    if(!call)return -1;
    ++call->skips;call->skip_all=skip_all!=0;
    if(skip_all){++call->skip_all_calls;call->queue_size=0;}
    else if(call->queue_size)--call->queue_size;
    return 1;
}
static void test_source_char_menu_tutorial_queue_commands() {
    const uint8_t name_bytes[]={'T'};
    const uint8_t frame_bytes[]={'S','k','i','l','l','s'};
    const uint8_t menu_bytes[]={'m','e','n','u','_','C','h','a','r','a','c','t','e','r'};
    dh2_script_command commands[4]={};
    commands[0].command_id=74;commands[0].field_count=3;
    commands[0].fields[0].kind=DH2_SCRIPT_VALUE_STRING;
    commands[0].fields[0].value.string={frame_bytes,sizeof(frame_bytes)};
    commands[0].fields[1].kind=DH2_SCRIPT_VALUE_STRING;
    commands[0].fields[1].value.string={menu_bytes,sizeof(menu_bytes)};
    commands[0].fields[2].kind=DH2_SCRIPT_VALUE_I32;
    commands[0].fields[2].value.i32=341;
    commands[1].command_id=75;
    commands[2]=commands[0];commands[2].fields[2].value.i32=342;
    commands[3].command_id=76;
    dh2_script script={};script.command_count=4;
    script.name={name_bytes,sizeof(name_bytes)};script.commands=commands;
    dh2_script_table common={};common.script_count=1;common.scripts=&script;
    dh2_script_table level={};TutorialQueueCall call{};
    Runtime runtime{};runtime.common_table=&common;runtime.level_table=&level;
    runtime.spawn_services.context=&call;
    runtime.spawn_services.enqueue_char_menu_tutorial=
        enqueue_char_menu_tutorial_dispatch;
    runtime.spawn_services.skip_char_menu_tutorial=
        skip_char_menu_tutorial_dispatch;
    runtime.tasks[0].script=&script;runtime.tasks[0].ticket=1;
    runtime.tasks[0].script_id=0;runtime.tasks[0].active=1;
    runtime.task_slots_used=1;runtime.next_task_ticket=1;
    check(advance(&runtime,16)==ERROR_OK&&!runtime.tasks[0].active,
          "source tutorial producer and skip commands complete in scheduler order");
    check(call.enqueues==2&&call.string_id==342&&
          std::strcmp(call.frame,"Skills")==0&&
          std::strcmp(call.menu,"menu_Character")==0,
          "command 74 preserves source frame/menu/string-ID payload order");
    check(call.skips==2&&call.skip_all&&call.skip_all_calls==1&&call.queue_size==0,
          "command 75 skips one record and command 76 drains the remaining queue");
}
static void test_source_char_menu_tutorial_queue_owner() {
    dh2::ui::SourceCharMenuTutorialQueueV1 queue;
    dh2::ui::SourceCharMenuTutorialQueueV1::Message message;
    bool found=true;
    check(queue.front(message,found)&&!found&&message.string_id==-1&&
          message.frame.empty()&&message.menu.empty(),
          "empty CharacterMenu tutorial queue preserves source empty sentinel");
    queue.enqueue(341,"Skills","menu_Character");
    queue.enqueue(342,"Faery","menu_Character");
    check(queue.front(message,found)&&found&&message.string_id==341&&
          message.frame=="Skills"&&message.menu=="menu_Character",
          "NativeGetCharMenuTutorialMessage peeks the first source record");
    check(queue.skip_one()&&queue.size()==1&&queue.front(message,found)&&
          found&&message.string_id==342,
          "NativeSkipCharMenuTutorialMessage removes one front record");
    queue.skip_all();
    check(!queue.active()&&!queue.skip_one(),
          "Script_SkipAllCharMenuTutorialMessages drains the canonical queue");
}

static uint8_t *read_file(const char *path, uint32_t *out_size) {
    FILE *file = fopen(path, "rb");
    long length;
    uint8_t *data;
    if (file == NULL || fseek(file, 0, SEEK_END) != 0 ||
        (length = ftell(file)) < 0 || (unsigned long)length > 0xfffffffful ||
        fseek(file, 0, SEEK_SET) != 0) {
        if (file != NULL) fclose(file);
        fprintf(stderr, "cannot open/size %s\n", path);
        return NULL;
    }
    data = (uint8_t *)malloc(length == 0 ? 1u : (size_t)length);
    if (data == NULL || fread(data, 1, (size_t)length, file) != (size_t)length) {
        free(data);
        fclose(file);
        fprintf(stderr, "cannot read %s\n", path);
        return NULL;
    }
    fclose(file);
    *out_size = (uint32_t)length;
    return data;
}

static bool copy_token(char *out, size_t capacity, const char *input) {
    if (input == NULL) return false;
    const size_t length = strlen(input);
    if (length >= capacity) return false;
    memcpy(out, input, length + 1);
    return true;
}

static bool load_seed_file(const char *path,
                           char *trigger_name,
                           char *trigger_script,
                           int32_t *trigger_count,
                           int32_t *trigger_delay,
                           ObjectSeed *objects,
                           uint32_t *object_count) {
    FILE *file = fopen(path, "rb");
    char line[512];
    bool have_trigger = false;
    *object_count = 0;
    if (file == NULL) return false;
    while (fgets(line, sizeof(line), file) != NULL) {
        char *tokens[5] = {NULL, NULL, NULL, NULL, NULL};
        uint32_t count = 0;
        char *cursor = line;
        while (count < 5 && cursor != NULL) {
            tokens[count++] = cursor;
            char *separator = strpbrk(cursor, "\t\r\n");
            if (separator == NULL) break;
            if (*separator == '\t') {
                *separator = '\0';
                cursor = separator + 1;
            } else {
                *separator = '\0';
                cursor = NULL;
            }
        }
        if (count == 0) continue;
        if (strcmp(tokens[0], "TRIGGER") == 0) {
            if (count != 5 || have_trigger ||
                !copy_token(trigger_name, MAX_NAME_BYTES, tokens[1]) ||
                !copy_token(trigger_script, MAX_NAME_BYTES, tokens[2])) {
                fclose(file);
                return false;
            }
            *trigger_count = (int32_t)strtol(tokens[3], NULL, 10);
            *trigger_delay = (int32_t)strtol(tokens[4], NULL, 10);
            have_trigger = true;
        } else if (strcmp(tokens[0], "OBJECT") == 0) {
            if (count != 5 || *object_count >= MAX_OBJECTS) {
                fclose(file);
                return false;
            }
            ObjectSeed *seed = &objects[(*object_count)++];
            memset(seed, 0, sizeof(*seed));
            if (!copy_token(seed->name, sizeof(seed->name), tokens[1]) ||
                !copy_token(seed->gametype, sizeof(seed->gametype), tokens[2]) ||
                !copy_token(seed->ai_state, sizeof(seed->ai_state), tokens[3])) {
                fclose(file);
                return false;
            }
            seed->auto_spawn = (uint8_t)(strtol(tokens[4], NULL, 10) != 0);
        } else {
            fclose(file);
            return false;
        }
    }
    fclose(file);
    return have_trigger && *object_count != 0;
}

static const Task *find_task(const Runtime &runtime, int32_t script_id) {
    for (uint32_t i = 0; i < runtime.task_slots_used; ++i) {
        const Task *task = &runtime.tasks[i];
        if (task->active && task->script_id == script_id) return task;
    }
    return NULL;
}

static bool load_table(const char *names_path, const char *programs_path,
                       dh2_script_table *table, const char *label) {
    uint32_t names_size = 0, programs_size = 0;
    uint8_t *names = read_file(names_path, &names_size);
    uint8_t *programs = read_file(programs_path, &programs_size);
    dh2_script_error error = {};
    if (names == NULL || programs == NULL) {
        free(names);
        free(programs);
        return false;
    }
    const dh2_script_error_code result = dh2_script_table_decode(
        names, names_size, programs, programs_size, table, &error);
    free(names);
    free(programs);
    if (result != DH2_SCRIPT_OK) {
        fprintf(stderr, "%s decode failed: %s input=%u offset=%u\n", label,
                dh2_script_error_name(result), (unsigned)error.input_kind,
                (unsigned)error.offset);
        return false;
    }
    return true;
}

static void run_flow(const dh2_script_table &common,
                     const dh2_script_table &swamp,
                     const char *seed_path) {
    ObjectSeed seeds[MAX_OBJECTS] = {};
    uint32_t seed_count = 0;
    char trigger_name[MAX_NAME_BYTES] = {};
    char trigger_script[MAX_NAME_BYTES] = {};
    int32_t trigger_count = 0;
    int32_t trigger_delay = 0;
    Options options;
    Runtime runtime = {};
    Runtime disabled_trigger_runtime = {};
    Runtime already_running_trigger_runtime = {};

    check(load_seed_file(seed_path, trigger_name, trigger_script,
                         &trigger_count, &trigger_delay, seeds, &seed_count),
          "read source-backed trigger and MGP object records");
    if (seed_count == 0) return;
    check(seed_count == 148, "all 148 original SWAMP MGP object records seeded");
    check(trigger_count == 1 && trigger_delay == 0,
          "original Lizard intro trigger is one-shot with zero delay");
    check(strcmp(trigger_name, "_prim_TriggerZone_LizManIntro") == 0 &&
          strcmp(trigger_script, "LizardMan_Intro") == 0,
          "original trigger record selects LizardMan_Intro");

    const ObjectRecord *lizard1_before = NULL;
    const ObjectRecord *lizard2_before = NULL;
    for (uint32_t i = 0; i < seed_count; ++i) {
        if (strcmp(seeds[i].name, "_prim_Monster_LizManIntro1") == 0) {
            check(strcmp(seeds[i].gametype, "Character") == 0 &&
                  strcmp(seeds[i].ai_state, "Limbus") == 0 &&
                  seeds[i].auto_spawn == 0,
                  "first lizard is a preloaded, non-auto-spawn Limbus Character");
        }
        if (strcmp(seeds[i].name, "_prim_Monster_LizManIntro2") == 0) {
            check(strcmp(seeds[i].gametype, "Character") == 0 &&
                  strcmp(seeds[i].ai_state, "Limbus") == 0 &&
                  seeds[i].auto_spawn == 0,
                  "second lizard is a preloaded, non-auto-spawn Limbus Character");
        }
        if (strcmp(seeds[i].name, "_prim_Waypoint_NewCamSpot") == 0) {
            check(strcmp(seeds[i].gametype, "Dummy") == 0,
                  "camera waypoint is present as a preloaded named object");
        }
    }

    init_options(&options);
    options.local_player_has_character = 1;
    options.online = 0;
    options.game_difficulty = 0;
    options.tutorial_pending[7] = 1;
    Error result = init(&runtime, &common, &swamp, seeds, seed_count,
                        trigger_name, trigger_script, trigger_count,
                        trigger_delay, &options);
    check(result == ERROR_OK, "initialize bounded script scheduler");
    if (result != ERROR_OK) {
        fprintf(stderr, "runtime init: %s\n", error_name(result));
        return;
    }
    check(runtime.trigger_script_id == 32, "trigger resolves local table ID to 32");
    check(runtime.object_count == seed_count, "runtime retains source object records");
    check(init(&disabled_trigger_runtime, &common, &swamp, seeds, seed_count,
               trigger_name, trigger_script, 0, trigger_delay, &options) == ERROR_OK,
          "zero-count trigger configuration initializes");
    check(!enter_trigger(&disabled_trigger_runtime) &&
          disabled_trigger_runtime.trigger_activations == 0,
          "zero-count trigger cannot activate");
    check(init(&already_running_trigger_runtime, &common, &swamp, seeds,
               seed_count, trigger_name, trigger_script, 2, trigger_delay,
               &options) == ERROR_OK,
          "multiple-count trigger configuration initializes");
    check(enter_trigger(&already_running_trigger_runtime),
          "first multi-count trigger activation starts its script");
    check(enter_trigger(&already_running_trigger_runtime) &&
          already_running_trigger_runtime.trigger_activations == 2,
          "running script suppresses duplicate task but native trigger count is consumed");
    check(find_event(&already_running_trigger_runtime,
                     EVENT_TRIGGER_SCRIPT_ALREADY_RUNNING, -1, 0,
                     "script is already running", 0) != NULL,
          "duplicate trigger activation is recorded separately from count rejection");
    check(find_task(already_running_trigger_runtime, 32) != NULL,
          "duplicate suppression retains only the already-running script context");
    check(!enter_trigger(&already_running_trigger_runtime),
          "multi-count trigger stops after its configured count is consumed");

    lizard1_before = find_object(&runtime, "_prim_Monster_LizManIntro1");
    lizard2_before = find_object(&runtime, "_prim_Monster_LizManIntro2");
    check(lizard1_before != NULL && lizard1_before->is_character &&
          lizard1_before->state == CHARACTER_STATE_LIMBUS &&
          !lizard1_before->auto_spawn,
          "runtime actor lookup starts from cache Limbus state");
    check(lizard2_before != NULL && lizard2_before->is_character &&
          lizard2_before->state == CHARACTER_STATE_LIMBUS &&
          !lizard2_before->auto_spawn,
          "runtime has second preloaded actor before command execution");
    check(find_object(&runtime, "_prim_Waypoint_NewCamSpot") != NULL,
          "camera command target resolves from preloaded object records");

    check(!runtime.player_locked && !runtime.cutscene_mode,
          "initial player state is unlocked outside cutscene mode");
    check(enter_trigger(&runtime), "first trigger activation starts its script");
    check(runtime.trigger_activations == 1 && runtime.trigger_fired,
          "one-shot trigger activation state is retained");
    check(!enter_trigger(&runtime), "trigger count rejects a second activation");
    check(runtime.trigger_activations == 1,
          "rejected one-shot activation does not consume another count");

    result = advance(&runtime, 0);
    check(result == ERROR_OK, "drain Lizard intro through its first wait");
    const Task *lizard_task = find_task(runtime, 32);
    check(lizard_task != NULL && lizard_task->waiting && lizard_task->pc == 3 &&
          lizard_task->wait_duration_ms == 500 && lizard_task->wait_elapsed_ms == 0,
          "PC and initial 500 ms wait persist after immediate commands");
    const Task *begin_cutscene = find_task(runtime, 1);
    check(begin_cutscene != NULL && begin_cutscene->pc == 0 &&
          !runtime.cutscene_mode && runtime.player_locked,
          "ExecScript child is queued; Lizard lock runs before next manager update");
    check(runtime.camera_target_is_player == 0 &&
          strcmp(runtime.camera_target, "_prim_Waypoint_NewCamSpot") == 0,
          "first camera target resolves to the cache waypoint");

    result = advance(&runtime, 0);
    check(result == ERROR_OK, "run queued BeginScriptedCutScene on the next update");
    check(find_task(runtime, 1) == NULL && runtime.cutscene_mode &&
          runtime.player_locked,
          "deferred common child enters cutscene mode on the next update");

    result = advance(&runtime, 499);
    check(result == ERROR_OK, "advance to one millisecond before first wait ends");
    lizard_task = find_task(runtime, 32);
    check(lizard_task != NULL && lizard_task->pc == 3 && lizard_task->waiting &&
          lizard_task->wait_elapsed_ms == 499,
          "500 ms wait preserves PC and elapsed timer at 499 ms");
    check(find_event(&runtime, EVENT_CHARACTER_SPAWN_STATE_REQUESTED, 32,
                     30, "_prim_Monster_LizManIntro1", 0) == NULL,
          "first lizard is not transitioned before 500 ms");

    result = advance(&runtime, 1);
    check(result == ERROR_OK, "update first wait elapsed to exactly 500 ms");
    check(find_event(&runtime, EVENT_CHARACTER_SPAWN_STATE_REQUESTED, 32,
                     30, "_prim_Monster_LizManIntro1", 0) == NULL &&
          find_task(runtime, 32) != NULL && find_task(runtime, 32)->waiting &&
          find_task(runtime, 32)->wait_elapsed_ms == 500,
          "Update reaching target does not recheck IsBlocking in the same pass");
    result = advance(&runtime, 0);
    check(result == ERROR_OK, "next manager pass observes the completed first wait");
    const ObjectRecord *lizard1_after = find_object(&runtime,
                                                    "_prim_Monster_LizManIntro1");
    check(lizard1_after != NULL && lizard1_after->spawn_state_requested &&
          lizard1_after->state == CHARACTER_STATE_NATIVE_1_REQUESTED &&
          lizard1_after->transition_count == 1,
          "first SpawnCharacter requests native state 1 on its preloaded record");
    const Event *spawn1 = find_event(&runtime,
        EVENT_CHARACTER_SPAWN_STATE_REQUESTED, 32, 30,
        "_prim_Monster_LizManIntro1", 0);
    check(spawn1 != NULL && spawn1->time_ms == 500 && spawn1->value == 1,
          "first spawn state transition occurs at 500 ms");
    lizard_task = find_task(runtime, 32);
    check(lizard_task != NULL && lizard_task->waiting && lizard_task->pc == 5 &&
          lizard_task->wait_duration_ms == 1500 && lizard_task->wait_elapsed_ms == 0,
          "1500 ms wait starts after first spawn and keeps the next PC");

    result = advance(&runtime, 1500);
    check(result == ERROR_OK, "update second wait elapsed to exactly 1500 ms");
    check(find_event(&runtime, EVENT_CHARACTER_SPAWN_STATE_REQUESTED, 32,
                     30, "_prim_Monster_LizManIntro2", 0) == NULL,
          "second wait blocks its target-reaching update");
    result = advance(&runtime, 0);
    check(result == ERROR_OK, "next manager pass observes the completed second wait");
    const ObjectRecord *lizard2_after = find_object(&runtime,
                                                    "_prim_Monster_LizManIntro2");
    check(lizard2_after != NULL && lizard2_after->spawn_state_requested &&
          lizard2_after->state == CHARACTER_STATE_NATIVE_1_REQUESTED &&
          lizard2_after->transition_count == 1,
          "second SpawnCharacter transitions its preloaded record");
    const Event *spawn2 = find_event(&runtime,
        EVENT_CHARACTER_SPAWN_STATE_REQUESTED, 32, 30,
        "_prim_Monster_LizManIntro2", 0);
    check(spawn2 != NULL && spawn2->time_ms == 2000 && spawn2->value == 1,
          "second spawn state transition occurs at 2000 ms");
    lizard_task = find_task(runtime, 32);
    check(lizard_task != NULL && lizard_task->waiting && lizard_task->pc == 7 &&
          lizard_task->wait_duration_ms == 2000 && lizard_task->wait_elapsed_ms == 0,
          "2000 ms wait starts after second spawn");

    result = advance(&runtime, 2000);
    check(result == ERROR_OK, "update final wait elapsed to exactly 2000 ms");
    check(runtime.player_locked &&
          find_event(&runtime, EVENT_PLAYER_LOCK_CHANGED, 32, 25, "All", 0) == NULL,
          "final wait blocks its target-reaching update");
    result = advance(&runtime, 0);
    check(result == ERROR_OK, "next manager pass observes the completed final wait");
    check(runtime.time_ms == 4000, "total scripted wait timeline is 500+1500+2000 ms");
    check(lizard1_after->transition_count == 1 && lizard2_after->transition_count == 1,
          "one-shot flow transitions each preloaded lizard once");
    const Event *unlock = find_event(&runtime, EVENT_PLAYER_LOCK_CHANGED,
                                     32, 25, "All", 0);
    check(unlock != NULL && unlock->time_ms == 4000 && unlock->value == 0,
          "Lizard flow unlocks the player after the 2000 ms final wait");
    const Event *camera_back = find_event(&runtime, EVENT_CAMERA_TARGET_CHANGED,
                                          32, 8, "LocalPlayer", 0);
    check(camera_back != NULL && camera_back->time_ms == 4000,
          "camera returns to LocalPlayer at 4000 ms");
    check(runtime.camera_target_is_player &&
          strcmp(runtime.camera_target, "LocalPlayer") == 0,
          "camera state is LocalPlayer after the intro");
    const Event *tutorial = find_event(&runtime, EVENT_SCRIPT_STARTED,
                                      16, 78, "CombatTuto", 0);
    check(tutorial != NULL && tutorial->time_ms == 4000,
          "eligible DoTutorial asynchronously starts level script CombatTuto");
    check(runtime.options.tutorial_pending[7] == 0,
          "DoTutorial consumes the source setting flag after the gate passes");
    check(!runtime.player_locked && runtime.cutscene_mode,
          "Lizard unlocks at 4000 ms while its EndScriptedCutScene child is queued");
    check(find_task(runtime, 32) == NULL && find_task(runtime, 3) != NULL &&
          find_task(runtime, 16) != NULL && find_task(runtime, 16)->pc == 0,
          "EndScriptedCutScene and CombatTuto wait for the next manager update");
    check(find_event(&runtime, EVENT_UNSUPPORTED_COMMAND, 16, 10, NULL, 0) == NULL,
          "queued CombatTuto has not run StartDialog in its start update");

    result = advance(&runtime, 0);
    check(result == ERROR_OK, "run queued EndScriptedCutScene and CombatTuto");
    check(!runtime.player_locked && !runtime.cutscene_mode,
          "supported end-script commands restore unlocked non-cutscene state");
    check(find_task(runtime, 32) == NULL && find_task(runtime, 16) == NULL,
          "Lizard intro and no-op-safe tutorial contexts complete on their update");
    check(find_task(runtime, 1) != NULL && find_task(runtime, 1)->pc == 0 &&
          find_task(runtime, 3) != NULL && find_task(runtime, 3)->pc == 0,
          "Begin/End children spawned by CombatTuto remain queued for another update");
    check(find_event(&runtime, EVENT_UNSUPPORTED_COMMAND, 16, 10, NULL, 0) != NULL,
          "tutorial dialogs are reported as unsupported no-op events");
    check(find_event(&runtime, EVENT_UNSUPPORTED_COMMAND, 16, 12, NULL, 0) != NULL,
          "unsupported WaitDialog is explicit, not silently simulated");
    check(find_event(&runtime, EVENT_CUTSCENE_MODE_CHANGED, 1, 1,
                     "enter", 1) == NULL,
          "CombatTuto's newly started cutscene child did not run in the same update");

    result = advance(&runtime, 0);
    check(result == ERROR_OK, "run the second-generation common script children");
    check(!runtime.player_locked && !runtime.cutscene_mode &&
          find_task(runtime, 1) == NULL && find_task(runtime, 3) == NULL,
          "queued common cutscene contexts complete on the following update");
    check(find_event(&runtime, EVENT_CUTSCENE_MODE_CHANGED, 1, 1,
                     "enter", 1) != NULL,
          "deferred CombatTuto BeginScriptedCutScene runs on its scheduled update");
    check(runtime.error == ERROR_OK && runtime.event_count < MAX_EVENTS,
          "runtime remains within fixed event/task/step bounds");
    check(!enter_trigger(&runtime), "one-shot trigger remains spent after script completion");

    for (uint32_t i = 0; i < runtime.event_count; ++i) {
        if (runtime.events[i].type == EVENT_SCRIPT_STARTED) {
            check(runtime.events[i].value <= MAX_DEPTH,
                  "script activation depth is within the configured bound");
        }
    }

    Runtime fixed_ticks = {};
    check(init(&fixed_ticks, &common, &swamp, seeds, seed_count,
               trigger_name, trigger_script, trigger_count, trigger_delay,
               &options) == ERROR_OK && enter_trigger(&fixed_ticks),
          "initialize source frame-order fixture with ordinary 25 ms ticks");
    check(advance(&fixed_ticks, 0) == ERROR_OK,
          "begin fixed-tick flow with the first Wait at zero elapsed");
    for (uint32_t i = 0; i < 20; ++i)
        check(advance(&fixed_ticks, 25) == ERROR_OK, "advance first fixed-tick wait");
    check(find_event(&fixed_ticks, EVENT_CHARACTER_SPAWN_STATE_REQUESTED,
                     32, 30, "_prim_Monster_LizManIntro1", 0) == NULL,
          "500 ms target-reaching frame remains blocked");
    check(advance(&fixed_ticks, 25) == ERROR_OK,
          "dispatch first spawn on the next fixed-tick pass");
    const Task *fixed_task = find_task(fixed_ticks, 32);
    const Event *fixed_first = find_event(&fixed_ticks,
        EVENT_CHARACTER_SPAWN_STATE_REQUESTED, 32, 30,
        "_prim_Monster_LizManIntro1", 0);
    check(fixed_first != NULL && fixed_first->time_ms == 525 &&
          fixed_task != NULL && fixed_task->wait_duration_ms == 1500 &&
          fixed_task->wait_elapsed_ms == 25,
          "new positive Wait receives this spawn frame's dt immediately");
    for (uint32_t i = 0; i < 59; ++i)
        check(advance(&fixed_ticks, 25) == ERROR_OK, "advance second fixed-tick wait");
    check(find_event(&fixed_ticks, EVENT_CHARACTER_SPAWN_STATE_REQUESTED,
                     32, 30, "_prim_Monster_LizManIntro2", 0) == NULL,
          "1500 ms target-reaching frame remains blocked");
    check(advance(&fixed_ticks, 25) == ERROR_OK,
          "dispatch second spawn on the next fixed-tick pass");
    const Event *fixed_second = find_event(&fixed_ticks,
        EVENT_CHARACTER_SPAWN_STATE_REQUESTED, 32, 30,
        "_prim_Monster_LizManIntro2", 0);
    check(fixed_second != NULL && fixed_second->time_ms == 2025,
          "fixed-tick second spawn records actual manager-frame time");
    for (uint32_t i = 0; i < 79; ++i)
        check(advance(&fixed_ticks, 25) == ERROR_OK, "advance final fixed-tick wait");
    check(fixed_ticks.player_locked && advance(&fixed_ticks, 25) == ERROR_OK,
          "dispatch unlock only on the next manager pass after final target");
    const Event *fixed_unlock = find_event(&fixed_ticks,
        EVENT_PLAYER_LOCK_CHANGED, 32, 25, "All", 0);
    check(fixed_unlock != NULL && fixed_unlock->time_ms == 4025,
          "fixed-tick unlock records source blocking/update frame order");

    Runtime large_ticks = {};
    check(init(&large_ticks, &common, &swamp, seeds, seed_count,
               trigger_name, trigger_script, trigger_count, trigger_delay,
               &options) == ERROR_OK && enter_trigger(&large_ticks) &&
          advance(&large_ticks, 600) == ERROR_OK,
          "execute a new Wait and update it once with a target-exceeding dt");
    const Task *large_task = find_task(large_ticks, 32);
    check(large_task != NULL && large_task->pc == 3 && large_task->waiting &&
          large_task->wait_elapsed_ms == 600 &&
          find_event(&large_ticks, EVENT_CHARACTER_SPAWN_STATE_REQUESTED,
                     32, 30, "_prim_Monster_LizManIntro1", 0) == NULL,
          "new Wait keeps overshoot and still blocks its execution pass");
    check(advance(&large_ticks, 100) == ERROR_OK,
          "next pass observes prior wait overshoot then executes a new wait");
    large_task = find_task(large_ticks, 32);
    check(large_task != NULL && large_task->pc == 5 && large_task->waiting &&
          large_task->wait_elapsed_ms == 100 && large_ticks.time_ms == 700,
          "next Wait resets elapsed and uses only current dt; no overshoot carry");
}

int main(int argc, char **argv) {
    if(argc==2&&strcmp(argv[1],"--play-camera-only")==0){
        test_play_camera_command();
        if(failures!=0)return 1;
        printf("typed PlayCamera scheduler checks passed\n");
        return 0;
    }
    if(argc==2&&strcmp(argv[1],"--actor-command-only")==0){
        test_actor_command(41);
        test_actor_command(46);
        if(failures!=0)return 1;
        printf("typed actor command scheduler checks passed\n");
        return 0;
    }
    if(argc==2&&strcmp(argv[1],"--play-actor-animation-only")==0){
        test_play_actor_animation_wait();
        if(failures!=0)return 1;
        printf("PlayActorAnim source wait checks passed\n");
        return 0;
    }
    if(argc==2&&strcmp(argv[1],"--dialog-only")==0){
        test_source_dialog_wait();
        test_source_char_menu_tutorial_queue_owner();
        test_source_char_menu_tutorial_queue_commands();
        if(failures!=0)return 1;
        printf("source dialog and CharacterMenu tutorial queue checks passed\n");
        return 0;
    }
    if (argc != 6) {
        fprintf(stderr, "usage: %s COMMON_NAMES COMMON_PROGRAMS SWAMP_NAMES "
                "SWAMP_PROGRAMS SOURCE_SEED_TSV\n", argv[0]);
        return 2;
    }
    dh2_script_table common = {};
    dh2_script_table swamp = {};
    const bool common_ok = load_table(argv[1], argv[2], &common, "common");
    const bool swamp_ok = load_table(argv[3], argv[4], &swamp, "SWAMP");
    check(common_ok && swamp_ok, "decode original common and SWAMP tables");
    test_play_camera_command();
    test_play_actor_animation_wait();
    if (common_ok && swamp_ok) {
        check(common.name_count == 15 && swamp.name_count == 55,
              "native tables expose all common and SWAMP names");
        check(dh2_script_resolve_id(&common, &swamp,
                                    (const uint8_t *)"LizardMan_Intro", 15, 0) == 32,
              "script ID resolves with level name base");
        check(dh2_script_resolve_id(&common, &swamp,
                                    (const uint8_t *)"CombatTuto", 10, 1) == 16,
              "tutorial script resolves as global ID 16");
        run_flow(common, swamp, argv[5]);
    }
    dh2_script_table_destroy(&common);
    dh2_script_table_destroy(&swamp);
    if (failures != 0) return 1;
    printf("bounded SWAMP script runtime checks passed\n");
    return 0;
}
