#include "../character_runtime_factory_v1.hpp"
#include "../character_init_post_nonplayer_v1.hpp"
#include "../character_gameplay_save_v1.hpp"
#include "../character_kill_death_tail_v1.hpp"
#include "../object_update_culling.hpp"
#include "../../game-data/player_save_load_owner_v1.hpp"
#include "../../android-native/app/src/main/cpp/native_character_list.hpp"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>

namespace factory = dh2::character_runtime_factory_v1;
namespace npc_post = dh2::character_init_post_nonplayer_v1;
namespace ctor = dh2::character_constructor_owner_v1;
namespace manager = dh2::object_manager_runtime_owner_v1;
namespace aggro = dh2::character::aggro_search;
namespace data = dh2::data;
namespace faery_session = dh2::character_faery_script_session_v1;
namespace kill_field = dh2::character_kill_field_14e4_v1;

struct FaerySessionFixture {
    faery_session::Identity ais{0x710000};
    faery_session::Identity lua_state{0x720000};
};

static int construct_faery_session(void* raw, faery_session::Identity,
        faery_session::Identity ai, faery_session::Session& session,
        std::string&) {
    const auto& fixture = *static_cast<FaerySessionFixture*>(raw);
    session.kind = faery_session::ScriptKind::faery;
    session.char_ai_owner = ai;
    session.ais = session.char_ai_script = fixture.ais;
    session.lua_instance = fixture.ais + 4;
    session.lua_state = fixture.lua_state;
    session.constructed = session.close_owned = true;
    return 0;
}

static int close_faery_session(void*, const faery_session::Request& request,
        faery_session::Session& session, std::string&) {
    if (request.operation != faery_session::Operation::close) return 1;
    session = {};
    return 0;
}

struct PlayerSaveGraph {
    data::PlayerSavegameV1 save;
    data::PlayerSaveProfileV1 profile;
    std::vector<std::pair<unsigned, unsigned>> load_calls;
    data::PlayerSaveLoadOwnerV1 loader;
    factory::gameplay_save::SaveRef ref;

    explicit PlayerSaveGraph(std::uintptr_t character)
        : loader(save, profile, {std::make_shared<int>(1),
              [this](const data::PlayerSaveLoadRequestV1& request,
                     data::PlayerSaveLoadResponseV1&,
                     std::string&) {
                  assert(request.save == &save);
                  load_calls.emplace_back(static_cast<unsigned>(request.operation),
                                           request.argument);
                  return true;
              }}),
          ref(factory::gameplay_save::borrow_save(
              reinterpret_cast<std::uintptr_t>(&save), save, &loader)) {
        save.set_character(character);
    }
};

struct Fixture {
    std::vector<int> calls;
    std::vector<int> lifecycle_calls;
    int fail_state = -1;
    int fail_init_post_count = 0;
    int fail_init_final_count = 0;
    bool fail_roster = false;
    bool inventory_retired_before_controllable_rollback = false;
    std::vector<aggro::Character*> characters;
    std::array<std::uint64_t, factory::component_count> owner_tokens{};
    std::vector<int> nonplayer_init_post_calls;
    factory::Record* nonplayer_record{};
    factory::Owner* factory_owner{};
};

struct AiBranchFixture {
    factory::Record* record{};
    std::int32_t ai_id{1};
    std::uint32_t id_queries{};
    std::vector<std::uint32_t> script_services;
};

struct EffectChainFixture {
    factory::Record* record{};
    std::int32_t fx_base{0x7fffffff};
    std::uint32_t fx_bits{};
    std::uintptr_t fx_handle{0xfeed1234u};
    std::vector<npc_post::EffectOperation> calls;
};

struct KillScriptFlagFixture {
    factory::Owner* owner{};
    factory::Record* record{};
    dh2::object_update_culling::Object object{};
    unsigned objective_calls{};
};

static bool kill_flag_preflight(void* raw,
        const dh2::character_kill_death_tail_v1::Request& request,
        std::string& error) {
    auto& f=*static_cast<KillScriptFlagFixture*>(raw);
    if (!f.owner || !f.record || request.character!=f.record->character.identity) {
        error="Kill flag test Character identity mismatch";return false;
    }
    return true;
}
static bool kill_flag_effect(void*,std::uintptr_t,std::uintptr_t,std::string&) { return true; }
static bool kill_flag_convert(void*,std::uintptr_t,std::uintptr_t,
        std::uintptr_t& killer_character,bool& xp_credit,std::string&) {
    killer_character=0;xp_credit=false;return true;
}
static bool kill_flag_virtual54(void* raw,std::uintptr_t character,
        std::int32_t& result,std::string& error) {
    auto& f=*static_cast<KillScriptFlagFixture*>(raw);
    if (character!=f.object.identity) {error="Kill flag ObjectBase identity mismatch";return false;}
    dh2::object_update_culling::RemoteResult remote{};
    if (dh2::object_update_culling::is_remotely_updated(&f.object,&remote)!=
        dh2::object_update_culling::Status::complete) {
        error="source IsRemotelyUpdated failed";return false;
    }
    result=static_cast<std::int32_t>(remote.raw);return true;
}
static bool kill_flag_read(void* raw,std::uintptr_t character,std::uint8_t& value,
        std::string& error) {
    auto& f=*static_cast<KillScriptFlagFixture*>(raw);
    if (!f.record || character!=f.record->character.identity) {
        error="Kill flag factory Record identity mismatch";return false;
    }
    value=f.record->source_script_created_539;return true;
}
static bool kill_flag_objective(void* raw,std::uintptr_t character,std::uintptr_t,
        std::int32_t virtual_result,std::uint8_t script_flag,std::string& error) {
    auto& f=*static_cast<KillScriptFlagFixture*>(raw);
    if (!f.record || character!=f.record->character.identity || virtual_result!=0 || script_flag!=0) {
        error="Kill objective tail was not correctly gated";return false;
    }
    ++f.objective_calls;return true;
}

static int effect_read_fx_base(void* raw, factory::Record& record,
                               std::int32_t* base) {
    auto& f = *static_cast<EffectChainFixture*>(raw);
    assert(&record == f.record && base);
    *base = f.fx_base;
    return 0;
}
static int effect_grab_anim_fx(void* raw, std::uint32_t fx_bits,
                               std::uintptr_t character,
                               std::uintptr_t* handle) {
    auto& f = *static_cast<EffectChainFixture*>(raw);
    assert(f.record && character == f.record->character.identity && handle);
    f.calls.push_back(npc_post::EffectOperation::grab_anim_fx);
    f.fx_bits = fx_bits;
    *handle = f.fx_handle;
    return 0;
}
static int effect_register(void* raw, factory::Record& record) {
    auto& f = *static_cast<EffectChainFixture*>(raw);
    assert(&record == f.record && record.source_self_anim_fx_1484 == f.fx_handle);
    f.calls.push_back(npc_post::EffectOperation::register_character_fx_table);
    return 0;
}
static int effect_set_animation(void* raw, void* animator) {
    auto& f = *static_cast<EffectChainFixture*>(raw);
    assert(f.record && animator == f.record->components.find(ctor::Component::animator));
    f.calls.push_back(npc_post::EffectOperation::set_animation_set);
    return 0;
}
static int effect_init_sounds(void* raw, factory::Record& record) {
    auto& f = *static_cast<EffectChainFixture*>(raw);
    assert(&record == f.record);
    f.calls.push_back(npc_post::EffectOperation::init_sounds);
    return 0;
}

static int ai_branch_get_id(void* raw, factory::Record& record,
                            std::int32_t* id) {
    auto& f = *static_cast<AiBranchFixture*>(raw);
    assert(f.record == &record && id);
    ++f.id_queries;
    *id = f.ai_id;
    return 0;
}

static void ai_branch_script_service(void* raw,
        dh2::character::ScriptLifecycleState64* state,
        const dh2::character::ScriptLifecycleRequest32* request,
        dh2::character::ScriptLifecycleResponse16* response) {
    auto& f = *static_cast<AiBranchFixture*>(raw);
    assert(f.record && state && request && response &&
           state->owner == f.record->character.identity);
    f.script_services.push_back(request->service);
    using namespace dh2::character;
    switch (request->service) {
    case script_create_step:
        assert(state->load_step == 0 && !state->pending);
        state->pending = 0x700000001ull;
        state->scripted = 1;
        response->identity = state->pending;
        break;
    case script_bind_functions:
        assert(state->load_step == 1 && request->subject == state->pending);
        break;
    case script_set_character:
        assert(state->load_step == 2 && request->subject == state->pending &&
               request->payload == state->owner);
        break;
    case script_load_common:
        assert(state->load_step == 3 && state->scripted);
        break;
    case script_ai_init:
        assert(state->load_step == 5 && request->subject == 0);
        break;
    default:
        assert(false && "unexpected service in source LoadScriptProcess");
    }
}

static int component(void* context, ctor::Component component_id,
                     ctor::Identity identity,
                     factory::ComponentStorage::Slot* slot,
                     std::string&) {
    auto& fixture = *static_cast<Fixture*>(context);
    fixture.calls.push_back(100 + static_cast<int>(component_id));
    assert(slot->character_record &&
           slot->character_record->character.identity == identity);
    slot->canonical_owner = &fixture.owner_tokens[static_cast<std::size_t>(component_id)];
    assert(identity != 0);
    return 0;
}

static int associate(void* context, ctor::Association association,
                     ctor::Identity identity,
                     const factory::ComponentStorage& components,
                     std::string&) {
    auto& fixture = *static_cast<Fixture*>(context);
    fixture.calls.push_back(200 + static_cast<int>(association));
    assert(identity != 0);
    const std::size_t available = association == ctor::Association::target_list
        ? factory::component_count - 1 : factory::component_count;
    for (std::size_t i = 0; i < available; ++i) {
        if (!components.find(static_cast<ctor::Component>(i))) {
            std::fprintf(stderr, "missing component slot %zu during association %u\n",
                         i, static_cast<unsigned>(association));
            std::abort();
        }
    }
    return 0;
}

static int register_state(void* context, ctor::Identity identity,
                          std::uint32_t state, std::string&) {
    auto& fixture = *static_cast<Fixture*>(context);
    fixture.calls.push_back(300 + static_cast<int>(state));
    assert(identity != 0);
    return fixture.fail_state == static_cast<int>(state) ? 1 : 0;
}

static void rollback(void* context, ctor::Action action, std::uint32_t value,
                     ctor::Identity identity,
                     factory::ComponentStorage& components) noexcept {
    auto& fixture = *static_cast<Fixture*>(context);
    fixture.calls.push_back(400 + static_cast<int>(action) * 100 +
                            static_cast<int>(value));
    assert(identity != 0);
    if (action == ctor::Action::component) {
        assert(components.slots[value].canonical_owner != nullptr);
        if (value == std::uint32_t(ctor::Component::controllable)) {
            auto* record = components.slots[value].character_record;
            assert(record);
            fixture.inventory_retired_before_controllable_rollback =
                !record->source_inventory.constructed &&
                components.find(ctor::Component::inventory) == nullptr;
        }
    }
}

struct FaeryPointCommandFixture {
    std::uint32_t calls = 0;
    dh2::character::ControllerPointCommandRequest32 last{};
};

static int invoke_faery_point_command(
        void* raw,
        const dh2::character::ControllerPointCommandRequest32* request) {
    auto* fixture = static_cast<FaeryPointCommandFixture*>(raw);
    if (!fixture || !request) return -1;
    ++fixture->calls;
    fixture->last = *request;
    return 1;
}

static int enroll(void* context, aggro::Character* character, bool duplicate,
                  bool* appended) {
    auto& fixture = *static_cast<Fixture*>(context);
    assert(!duplicate && character && appended);
    if (fixture.fail_roster) {
        fixture.characters.push_back(character); // exercise rollback after partial publication
        *appended = true;
        return 1;
    }
    fixture.characters.push_back(character);
    *appended = true;
    return 0;
}

static int remove_character(void* context, aggro::Character* character,
                            std::size_t* removed) {
    auto& fixture = *static_cast<Fixture*>(context);
    *removed = 0;
    for (auto it = fixture.characters.begin(); it != fixture.characters.end();) {
        if (*it == character) {
            it = fixture.characters.erase(it);
            ++*removed;
        } else ++it;
    }
    return 0;
}

static int init_post(void* context, factory::Record& record,
                     std::string& error) {
    auto& fixture = *static_cast<Fixture*>(context);
    assert(record.source_properties_ready());
    assert(record.character.identity == record.game_object.identity);
    fixture.lifecycle_calls.push_back(1);
    if (fixture.fail_init_post_count > 0) {
        --fixture.fail_init_post_count;
        error = "injected InitPost failure";
        return 1;
    }
    return 0;
}

static int init_final(void* context, factory::Record& record,
                      std::string& error) {
    auto& fixture = *static_cast<Fixture*>(context);
    assert(record.source_properties_ready() && record.init_post_complete());
    fixture.lifecycle_calls.push_back(2);
    if (fixture.fail_init_final_count > 0) {
        --fixture.fail_init_final_count;
        error = "injected InitFinal failure";
        return 1;
    }
    return 0;
}

static factory::Services services(Fixture& f) {
    return {&f, &component, &associate, &register_state, &rollback};
}
static factory::RosterServices roster(Fixture& f) {
    return {&f, &enroll, &remove_character};
}
static factory::LifecycleServices lifecycle(Fixture& f) {
    return {&f, &init_post, &init_final};
}

static int npc_is_player(void* raw, const factory::Record& record, bool* value) {
    auto& f = *static_cast<Fixture*>(raw);
    assert(f.nonplayer_record == &record && value);
    f.nonplayer_init_post_calls.push_back(0);
    *value = false;
    return 0;
}
static int npc_spawn_gate(void* raw, factory::Record& record, bool* accepted) {
    auto& f = *static_cast<Fixture*>(raw);
    assert(f.nonplayer_record == &record && accepted);
    f.nonplayer_init_post_calls.push_back(1);
    *accepted = true;
    return 0;
}
static int npc_resolve_properties(void* raw, factory::Record& record,
                                 std::int16_t* id) {
    auto& f = *static_cast<Fixture*>(raw);
    assert(f.nonplayer_record == &record && id);
    f.nonplayer_init_post_calls.push_back(2);
    *id = 263;
    return 0;
}
static int npc_load_base(void* raw, void* properties, std::int16_t id) {
    auto& f = *static_cast<Fixture*>(raw);
    assert(f.nonplayer_record && properties ==
        f.nonplayer_record->components.find(ctor::Component::properties) && id == 263);
    f.nonplayer_init_post_calls.push_back(3);
    return 0;
}
static int npc_recalc(void* raw, void* properties, bool force) {
    auto& f = *static_cast<Fixture*>(raw);
    assert(f.nonplayer_record && properties ==
        f.nonplayer_record->components.find(ctor::Component::properties) && force);
    f.nonplayer_init_post_calls.push_back(4);
    return 0;
}
static int npc_model_name(void* raw, factory::Record& record, std::string* name) {
    auto& f = *static_cast<Fixture*>(raw);
    assert(f.nonplayer_record == &record && name);
    f.nonplayer_init_post_calls.push_back(5);
    *name = "crypt-npc";
    return 0;
}
static int npc_read_scale(void* raw, void* properties, std::int32_t xyz[3]) {
    auto& f = *static_cast<Fixture*>(raw);
    assert(f.nonplayer_record && properties ==
        f.nonplayer_record->components.find(ctor::Component::properties) && xyz);
    f.nonplayer_init_post_calls.push_back(6);
    xyz[0] = 100; xyz[1] = 200; xyz[2] = 300;
    return 0;
}
static int npc_apply_scale(void* raw, void* game_object, const float xyz[3]) {
    auto& f = *static_cast<Fixture*>(raw);
    assert(f.nonplayer_record && game_object ==
        f.nonplayer_record->components.find(ctor::Component::game_object) && xyz);
    assert(xyz[0] > 0.89f && xyz[0] < 0.91f && xyz[1] > 1.79f &&
           xyz[1] < 1.81f && xyz[2] > 2.99f && xyz[2] < 3.01f);
    f.nonplayer_init_post_calls.push_back(7);
    return 0;
}
static int npc_game_object_init_post(void* raw, void* game_object) {
    auto& f = *static_cast<Fixture*>(raw);
    assert(f.nonplayer_record && game_object ==
        f.nonplayer_record->components.find(ctor::Component::game_object));
    f.nonplayer_init_post_calls.push_back(8);
    return 0;
}
static int npc_meet_condition(void* raw, void* game_object, bool* matched) {
    auto& f = *static_cast<Fixture*>(raw);
    assert(f.nonplayer_record && game_object ==
        f.nonplayer_record->components.find(ctor::Component::game_object) && matched);
    f.nonplayer_init_post_calls.push_back(9);
    *matched = true;
    return 0;
}
static int npc_load_save_mask2(void* raw, factory::Record& record,
                               factory::gameplay_save::Result* result,
                               std::string& error) {
    auto& f = *static_cast<Fixture*>(raw);
    assert(f.factory_owner && f.nonplayer_record == &record && result &&
           record.source_save_slot_kind == factory::SourceSaveSlotKind::nonplayer_null &&
           !record.source_save_14e8);
    f.nonplayer_init_post_calls.push_back(10);
    return f.factory_owner->load_source_save_mask2(record.source_handle, result,
        error) == factory::Status::complete ? 0 : 1;
}
static npc_post::Services nonplayer_post_services(Fixture& f) {
    return {&f, &npc_is_player, &npc_spawn_gate, &npc_resolve_properties,
        &npc_load_base, &npc_recalc, &npc_model_name, &npc_read_scale,
        &npc_apply_scale, &npc_game_object_init_post, &npc_meet_condition,
        &npc_load_save_mask2};
}

static manager::GameObject game_object(float x) {
    manager::GameObject object{};
    object.world_x = x;
    object.world_y = x + 1.0f;
    return object;
}
static aggro::GameObject aggro_object(float x) {
    aggro::GameObject object{};
    object.position[0] = x;
    object.position[1] = x + 1.0f;
    object.visible = 1;
    return object;
}

int main() {
    manager::Owner objects;
    Fixture fixture;
    factory::Owner owner(objects, roster(fixture));
    fixture.factory_owner = &owner;
    factory::Record* first = nullptr;
    factory::Result result{};
    std::string error;

    assert(owner.create(41, game_object(3), aggro_object(3), services(fixture),
                        &first, &result, error) == factory::Status::complete);
    assert(first && result.constructor.registered_state_count == 20);
    assert(result.constructor.registered_state_mask == ctor::all_registered_states);
    assert(result.object_registered && result.character_listed);
    assert(first->character.object == &first->aggro_object);
    assert(first->character.identity == first->game_object.identity);
    assert(first->aggro_object.identity == first->game_object.identity);
    std::uint8_t kill_field_value = 0xff;
    assert(first->kill_field_14e4.character == first->character.identity &&
           first->kill_field_14e4.constructor_write_seen == 1 &&
           first->kill_field_14e4.network_spawn_write_seen == 0 &&
           first->kill_field_14e4.value == 0 &&
           kill_field::read(&first->kill_field_14e4, first->character.identity,
                            &kill_field_value) == kill_field::Status::complete &&
           kill_field_value == 0);
    assert(first->source_inventory.constructed &&
           first->source_inventory.character == first->character.identity &&
           first->source_inventory.selected_set == 0 &&
           first->components.find(ctor::Component::inventory) ==
               &first->source_inventory);
    for (const auto& equipment_set : first->source_inventory.equipment)
        for (const auto* item_slot : equipment_set) assert(item_slot == nullptr);
    assert(first->coordinator && first->coordinator->owner() == first->character.identity &&
           first->coordinator->timers().capacity == 20 &&
           first->components.find(ctor::Component::timers) == first->coordinator.get() &&
           first->components.find(ctor::Component::state_machine) == first->coordinator.get());
    assert(first->net_state.constructed &&
           first->net_state.character_identity == first->character.identity &&
           first->net_state.primary.component_offset == 0x1508 &&
           first->net_state.secondary.component_offset == 0x1a48 &&
           first->net_state.primary.character_identity == first->character.identity &&
           first->net_state.secondary.character_identity == first->character.identity &&
           first->components.find(ctor::Component::net_state_primary) ==
               &first->net_state.primary &&
           first->components.find(ctor::Component::net_state_secondary) ==
               &first->net_state.secondary &&
           first->net_state.primary.change_counter == &first->net_state_change_counter &&
           first->net_state.secondary.change_counter == &first->net_state_change_counter);
    static_assert(ctor::source_steps[8].value ==
                  std::uint32_t(ctor::Component::net_state_primary));
    static_assert(ctor::source_steps[9].value ==
                  std::uint32_t(ctor::Component::net_state_secondary));
    assert(!first->source_properties_ready() && !first->init_post_complete() &&
           !first->init_final_complete() && fixture.lifecycle_calls.empty());
    assert(objects.object_count() == 1 && fixture.characters.size() == 1);
    const auto* stable_character = &first->character;

    // Lifecycle cannot run during construction: the caller must explicitly
    // confirm canonical source properties/XML overrides are loaded first.
    assert(owner.run_init_post(41, lifecycle(fixture), &result, error) ==
           factory::Status::properties_not_ready);
    assert(owner.run_init_final(41, lifecycle(fixture), &result, error) ==
           factory::Status::init_post_not_complete);
    assert(fixture.lifecycle_calls.empty());
    assert(owner.mark_source_properties_ready(41, &result, error) ==
           factory::Status::complete);
    assert(first->source_properties_ready() && result.source_properties_ready);
    assert(owner.mark_source_properties_ready(41, &result, error) ==
           factory::Status::complete); // readiness marker is idempotent

    fixture.fail_init_post_count = 1;
    assert(owner.run_init_post(41, lifecycle(fixture), &result, error) ==
           factory::Status::init_post_failed);
    assert(!first->init_post_complete() && !result.init_post_complete &&
           error == "injected InitPost failure");
    assert(owner.run_init_final(41, lifecycle(fixture), &result, error) ==
           factory::Status::init_post_not_complete);
    assert(owner.run_init_post(41, lifecycle(fixture), &result, error) ==
           factory::Status::complete);
    assert(first->init_post_complete() && result.init_post_complete);
    const auto post_calls = fixture.lifecycle_calls.size();
    assert(owner.run_init_post(41, lifecycle(fixture), &result, error) ==
           factory::Status::complete);
    assert(fixture.lifecycle_calls.size() == post_calls); // successful stage one-shot

    fixture.fail_init_final_count = 1;
    assert(owner.run_init_final(41, lifecycle(fixture), &result, error) ==
           factory::Status::init_final_failed);
    assert(!first->init_final_complete() && !result.init_final_complete &&
           error == "injected InitFinal failure");
    assert(owner.run_init_final(41, lifecycle(fixture), &result, error) ==
           factory::Status::complete);
    assert(first->init_final_complete() && result.init_final_complete);
    const auto final_calls = fixture.lifecycle_calls.size();
    assert(owner.run_init_final(41, lifecycle(fixture), &result, error) ==
           factory::Status::complete);
    assert(fixture.lifecycle_calls.size() == final_calls); // successful stage one-shot
    assert((fixture.lifecycle_calls == std::vector<int>{1, 1, 2, 2}));

    factory::Record* second = nullptr;
    assert(owner.create(42, game_object(8), aggro_object(8), services(fixture),
                        &second, &result, error) == factory::Status::complete);
    assert(second);
    assert(first->source_script_created_539 == 0 &&
           second->source_script_created_539 == 0);
    assert(owner.mark_script_created(42, error) == factory::Status::complete &&
           second->source_script_created_539 == 1 &&
           first->source_script_created_539 == 0);
    assert(owner.mark_script_created(42, error) == factory::Status::complete &&
           second->source_script_created_539 == 1);
    assert(owner.mark_script_created(0xdeadbeef, error) ==
           factory::Status::not_found);
    // Join the real CreateNPC-owned Character+0x14e4 projection to the shared
    // Character::Kill tail: authored NPCs reach the objective tail, script
    // CreateNPC actors skip it after the source +0x54 predicate returns zero.
    {
        using namespace dh2::character_kill_death_tail_v1;
        const auto run_kill_flag=[&](factory::Record* record) {
            KillScriptFlagFixture kill_fixture{};
            kill_fixture.owner=&owner;
            kill_fixture.record=record;
            kill_fixture.object={record->character.identity,UINT32_MAX,0,0,{0,0}};
            Services kill_services{&kill_fixture,kill_flag_preflight,
                kill_flag_effect,kill_flag_effect,kill_flag_convert,
                kill_flag_effect,kill_flag_virtual54,kill_flag_read,
                kill_flag_objective};
            Runtime runtime({record->character.identity,0x9911u,0},kill_services);
            Result kill_result{};
            assert(runtime.run(&kill_result,error)==Status::complete);
            return std::pair<Result,unsigned>{kill_result,kill_fixture.objective_calls};
        };
        const auto authored_kill=run_kill_flag(first);
        const auto scripted_kill=run_kill_flag(second);
        assert(authored_kill.first.character_14e4==0 &&
               authored_kill.first.objective_tail_completed && authored_kill.second==1);
        assert(scripted_kill.first.character_14e4==1 &&
               scripted_kill.first.objective_tail_skipped &&
               !scripted_kill.first.objective_tail_attempted && scripted_kill.second==0);
    }
    assert(second && owner.find(41) == first && &owner.find(41)->character == stable_character);
    assert(second->net_state.constructed &&
           second->net_state.character_identity == second->character.identity &&
           second->net_state.primary.identity != first->net_state.primary.identity &&
           second->net_state.secondary.identity != first->net_state.secondary.identity);
    assert(objects.object_count() == 2 && fixture.characters.size() == 2);

    factory::Record* npc_record = nullptr;
    assert(owner.create(45, game_object(11), aggro_object(11), services(fixture),
                        &npc_record, &result, error) == factory::Status::complete);
    assert(owner.find_character(npc_record->character.identity) == npc_record &&
           owner.find_character(0) == nullptr &&
           owner.find_character(npc_record->character.identity + 1) == nullptr);
    assert(owner.mark_source_properties_ready(45, &result, error) ==
           factory::Status::complete);
    fixture.nonplayer_record = npc_record;
    const auto npc_services = nonplayer_post_services(fixture);
    npc_post::Result npc_result{};
    factory::gameplay_save::Result npc_save_result{};
    assert(npc_post::run_prefix(*npc_record, npc_services, &npc_result, error) ==
           npc_post::Status::awaiting_scene_runtime);
    assert(npc_result.prefix_complete && !npc_result.full_init_post_complete &&
           npc_result.condition_met && npc_result.spawn_accepted &&
           npc_result.save_mask2_loaded &&
           npc_record->property_id_13c8 == 263 &&
           npc_record->source_save_slot_kind == factory::SourceSaveSlotKind::nonplayer_null &&
           npc_record->source_model_name == "crypt-npc" &&
           npc_record->source_owner_scale[0] > 0.89f &&
           npc_record->source_owner_scale[2] > 2.99f &&
           npc_result.provider_calls == 11 &&
           (fixture.nonplayer_init_post_calls ==
             std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
    const auto completed_prefix_calls = fixture.nonplayer_init_post_calls.size();
    assert(npc_post::run_prefix(*npc_record, npc_services, &npc_result, error) ==
           npc_post::Status::awaiting_scene_runtime &&
           fixture.nonplayer_init_post_calls.size() == completed_prefix_calls);
    assert(npc_record->source_save_mask2_attempted &&
           npc_record->source_save_mask2_complete);
    assert(owner.load_source_save_mask2(45, &npc_save_result, error) ==
           factory::Status::init_post_failed &&
           fixture.nonplayer_init_post_calls.size() == completed_prefix_calls);
    assert(owner.run_init_final(45, lifecycle(fixture), &result, error) ==
           factory::Status::init_post_not_complete);
    assert(!npc_record->init_post_complete() && !npc_record->init_final_complete());

    // Character::InitPost's next source branch reads the selected AITable row.
    // A non-delayed NPC runs the existing LoadScriptProcess kernel against the
    // same canonical CharAI component and persists its resulting fields there.
    dh2::character_ai_initialization::State npc_ai{};
    npc_ai.identity = reinterpret_cast<std::uintptr_t>(&npc_ai);
    npc_ai.owner_04 = npc_record->character.identity;
    npc_ai.pointer_28 = 0;
    npc_ai.word_10 = npc_ai.word_14 = UINT32_MAX;
    npc_record->components.slots[static_cast<std::size_t>(ctor::Component::ai)]
        .canonical_owner = &npc_ai;
    data::AiTables ai_tables;
    ai_tables.rows.resize(9);
    ai_tables.rows[1].delayed_load = 0;
    ai_tables.rows[1].script = "__monster__";
    AiBranchFixture ai_branch_fixture{npc_record, 1, 0, {}};
    npc_post::AiBranchServices no_script_service{
        &ai_branch_fixture, &ai_branch_get_id, &ai_tables, {}};
    npc_post::AiBranchResult ai_branch_result{};
    assert(npc_post::run_ai_branch(*npc_record, npc_ai, no_script_service,
                                   &ai_branch_result, error) ==
           npc_post::AiBranchStatus::service_unavailable);
    assert(!npc_record->nonplayer_ai_postload_complete &&
           !npc_ai.active_ais_1c && !npc_ai.alternate_ais_20 &&
           npc_ai.pointer_28 == 0 && ai_branch_fixture.id_queries == 1 &&
           ai_branch_fixture.script_services.empty());
    npc_post::AiBranchServices ai_branch_services{
        &ai_branch_fixture, &ai_branch_get_id, &ai_tables,
        {&ai_branch_fixture, &ai_branch_script_service}};
    assert(npc_post::run_ai_branch(*npc_record, npc_ai, ai_branch_services,
                                   &ai_branch_result, error) ==
           npc_post::AiBranchStatus::complete);
    assert(ai_branch_result.completed && ai_branch_result.script_load_called &&
           !ai_branch_result.delayed_init_requested &&
           ai_branch_result.ai_id == 1 && ai_branch_result.delayed_load == 0 &&
           ai_branch_result.lifecycle_calls == 5 && ai_branch_fixture.id_queries == 2 &&
           npc_ai.active_ais_1c == 0x700000001ull &&
           npc_ai.alternate_ais_20 == npc_ai.active_ais_1c &&
           npc_ai.pointer_28 == 7 && npc_ai.byte_2c == 1 &&
           (ai_branch_fixture.script_services == std::vector<std::uint32_t>{
               dh2::character::script_create_step,
               dh2::character::script_bind_functions,
               dh2::character::script_set_character,
               dh2::character::script_load_common,
               dh2::character::script_ai_init}));
    const auto ai_calls_after_load = ai_branch_fixture.script_services.size();
    assert(npc_post::run_ai_branch(*npc_record, npc_ai, ai_branch_services,
                                   &ai_branch_result, error) ==
           npc_post::AiBranchStatus::already_complete &&
           ai_branch_fixture.script_services.size() == ai_calls_after_load &&
           ai_branch_fixture.id_queries == 2);

    // This boundary models the exact source order only. Fixture callbacks
    // validate identity and ordering; they do not claim FX/audio parity.
    ai_tables.rows[1].self_fx = 2;
    EffectChainFixture effect_fixture{npc_record, 0x7fffffff, 0,
                                      0xfeed1234u, {}};
    npc_post::EffectChainServices missing_effect_service{
        &effect_fixture, &effect_read_fx_base, &effect_grab_anim_fx,
        &effect_register, &effect_set_animation, nullptr};
    npc_post::EffectChainResult effect_result{};
    assert(npc_post::run_effect_chain(*npc_record, ai_tables,
        missing_effect_service, &effect_result, error) ==
        npc_post::EffectChainStatus::service_unavailable);
    assert(!npc_record->source_anim_fx_base_ready &&
           npc_record->nonplayer_init_post_effects_stage == 0 &&
           npc_record->nonplayer_init_post_effects_attempted == 0xff &&
           effect_fixture.calls.empty());
    const npc_post::EffectChainServices effect_services{
        &effect_fixture, &effect_read_fx_base, &effect_grab_anim_fx,
        &effect_register, &effect_set_animation, &effect_init_sounds};
    assert(npc_post::run_effect_chain(*npc_record, ai_tables,
        effect_services, &effect_result, error) ==
        npc_post::EffectChainStatus::complete);
    assert(effect_result.complete && effect_result.completed_operations == 4 &&
           effect_result.provider_calls == 5 && effect_result.ai_id == 1 &&
           effect_result.character_identity == npc_record->character.identity &&
           effect_result.animator_identity == reinterpret_cast<std::uintptr_t>(
               npc_record->components.find(ctor::Component::animator)) &&
           effect_fixture.fx_bits == 0x80000001u &&
           npc_record->source_self_anim_fx_id == -2147483647 &&
           npc_record->source_self_anim_fx_1484 == effect_fixture.fx_handle &&
           (effect_fixture.calls == std::vector<npc_post::EffectOperation>{
               npc_post::EffectOperation::grab_anim_fx,
               npc_post::EffectOperation::register_character_fx_table,
               npc_post::EffectOperation::set_animation_set,
               npc_post::EffectOperation::init_sounds}));
    const auto effect_call_count = effect_fixture.calls.size();
    assert(npc_post::run_effect_chain(*npc_record, ai_tables,
        effect_services, &effect_result, error) ==
        npc_post::EffectChainStatus::already_complete &&
        effect_fixture.calls.size() == effect_call_count);

    const auto count_before_failure = fixture.calls.size();
    fixture.fail_state = 7;
    factory::Record* rejected = nullptr;
    assert(owner.create(43, game_object(13), aggro_object(13), services(fixture),
                        &rejected, &result, error) == factory::Status::constructor_failed);
    assert(!rejected && owner.find(43) == nullptr);
    assert(objects.object_count() == 3 && fixture.characters.size() == 3);
    assert(result.constructor.failed_callsite == 0x3a9820);
    assert(result.constructor.rolled_back_steps == 18 + 7);
    assert(fixture.inventory_retired_before_controllable_rollback);
    assert(fixture.calls.size() > count_before_failure);
    fixture.fail_state = -1;

    fixture.fail_roster = true;
    assert(owner.create(44, game_object(20), aggro_object(20), services(fixture),
                        &rejected, &result, error) == factory::Status::roster_failed);
    assert(!rejected && owner.find(44) == nullptr);
    assert(objects.object_count() == 3 && fixture.characters.size() == 3);
    fixture.fail_roster = false;

    FaerySessionFixture faery_fixture;
    const faery_session::Services faery_services{
        &faery_fixture, &construct_faery_session, &close_faery_session};
    assert(first->faery_script.construct(first->character.identity,
        reinterpret_cast<faery_session::Identity>(
            first->components.find(ctor::Component::ai)),
        faery_services, error) == faery_session::Status::complete);
    assert(owner.retire(41, &result, error) == factory::Status::cleanup_incomplete);
    assert(owner.find(41) == first && first->object_registered &&
           first->character_listed);
    assert(first->faery_script.close(error) == faery_session::Status::complete);
    assert(owner.retire(41, &result, error) == factory::Status::complete);
    assert(result.source_properties_ready && result.init_post_complete &&
           result.init_final_complete);
    assert(owner.find(41) == nullptr && objects.object_count() == 2);
    assert(owner.run_init_post(41, lifecycle(fixture), &result, error) ==
           factory::Status::not_found);
    assert(fixture.characters.size() == 2 && fixture.characters[0] == &second->character);
    assert(owner.retire(42, &result, error) == factory::Status::complete);
    assert(owner.retire(45, &result, error) == factory::Status::complete);
    assert(owner.size() == 0 && objects.object_count() == 0 && fixture.characters.empty());

    // Constructor leaves source Character+0x14e8 null. The non-player prefix
    // classifies that as the correct no-Save slot; the wrapper must not invent
    // a Save or call a loader. A player may bind its actual Save+LoadOwner and
    // the same wrapper must forward precisely mask 2 through that owner.
    factory::Record* save_player = nullptr;
    assert(owner.create(46, game_object(24), aggro_object(24), services(fixture),
                        &save_player, &result, error) == factory::Status::complete);
    assert(save_player && !save_player->source_save_14e8 &&
           save_player->source_save_slot_kind == factory::SourceSaveSlotKind::constructor_null);
    auto player_save = std::make_shared<PlayerSaveGraph>(save_player->character.identity);
    assert(owner.bind_player_save_slot(46, &player_save->ref, player_save, error) ==
           factory::Status::complete);
    assert(save_player->source_save_slot_kind == factory::SourceSaveSlotKind::player_save &&
           save_player->source_save_14e8 == &player_save->ref &&
           save_player->source_save_lifetime == player_save);
    factory::gameplay_save::Result save_result{};
    assert(owner.load_source_save_mask2(46, &save_result, error) ==
           factory::Status::complete);
    const std::vector<std::pair<unsigned, unsigned>> expected_mask2_calls{
        {static_cast<unsigned>(data::PlayerSaveLoadOpV1::init_levels), 0},
        {static_cast<unsigned>(data::PlayerSaveLoadOpV1::init_skills), 0},
        {static_cast<unsigned>(data::PlayerSaveLoadOpV1::init_faeries), 0},
        {static_cast<unsigned>(data::PlayerSaveLoadOpV1::init_quests), 0},
        {static_cast<unsigned>(data::PlayerSaveLoadOpV1::init_quests), 1}};
    assert(save_result.captured_character == save_player->character.identity &&
           save_result.captured_save == player_save->ref.identity &&
           save_result.mask == 2 && save_result.load_calls == 1 &&
           save_player->source_save_mask2_attempted &&
           save_player->source_save_mask2_complete &&
           player_save->load_calls == expected_mask2_calls);
    factory::gameplay_save::Result repeated_save_result{};
    assert(owner.load_source_save_mask2(46, &repeated_save_result, error) ==
           factory::Status::init_post_failed && player_save->load_calls.size() == 5);
    assert(owner.retire(46, &result, error) == factory::Status::complete &&
           player_save->load_calls.size() == 5);

    // The delayed-load path writes only Character+0x3ec's semantic byte and
    // does not touch CharAI ScriptLifecycle or require an initialized VM.
    factory::Record* delayed_npc = nullptr;
    assert(owner.create(47, game_object(27), aggro_object(27), services(fixture),
                        &delayed_npc, &result, error) == factory::Status::complete);
    assert(owner.mark_source_properties_ready(47, &result, error) ==
           factory::Status::complete);
    fixture.nonplayer_record = delayed_npc;
    const auto delayed_services = nonplayer_post_services(fixture);
    npc_post::Result delayed_prefix{};
    assert(npc_post::run_prefix(*delayed_npc, delayed_services, &delayed_prefix,
                                error) == npc_post::Status::awaiting_scene_runtime);
    dh2::character_ai_initialization::State delayed_ai{};
    delayed_ai.identity = reinterpret_cast<std::uintptr_t>(&delayed_ai);
    delayed_ai.owner_04 = delayed_npc->character.identity;
    delayed_ai.byte_24 = 1;
    delayed_npc->components.slots[static_cast<std::size_t>(ctor::Component::ai)]
        .canonical_owner = &delayed_ai;
    data::AiTables delayed_tables;
    delayed_tables.rows.resize(9);
    delayed_tables.rows[1].delayed_load = 1;
    AiBranchFixture delayed_fixture{delayed_npc, 1, 0, {}};
    npc_post::AiBranchServices delayed_branch_services{
        &delayed_fixture, &ai_branch_get_id, &delayed_tables, {}};
    npc_post::AiBranchResult delayed_result{};
    assert(npc_post::run_ai_branch(*delayed_npc, delayed_ai,
             delayed_branch_services, &delayed_result, error) ==
           npc_post::AiBranchStatus::complete);
    assert(delayed_result.completed && delayed_result.delayed_init_requested &&
           !delayed_result.script_load_called && !delayed_result.lifecycle_calls &&
           delayed_npc->source_delay_init_3ec == 1 &&
           delayed_ai.pointer_28 == 0 && !delayed_ai.active_ais_1c);
    assert(owner.retire(47, &result, error) == factory::Status::complete);

    // Also run the selected app roster owner through its production adapter.
    manager::Owner selected_objects;
    dh2::native::character_list::Owner selected_roster;
    factory::Owner selected_factory(selected_objects,
        dh2::native::character_list::factory_services(selected_roster));
    Fixture selected_fixture;
    factory::Record* selected = nullptr;
    assert(selected_factory.create(71, game_object(2), aggro_object(2),
        services(selected_fixture), &selected, &result, error) == factory::Status::complete);
    assert(selected && selected_roster.owned_nodes() == 1);
    const auto controller = reinterpret_cast<std::uintptr_t>(
        selected->components.find(ctor::Component::controller));
    const auto controllable = reinterpret_cast<std::uintptr_t>(
        selected->components.find(ctor::Component::controllable));
    assert(dh2::character_faery_controller_v1::bind(
        *selected, controller, controllable, error) ==
        dh2::character_faery_controller_v1::Status::complete);
    FaeryPointCommandFixture point_fixture{};
    const dh2::character::ControllerPointCommandServices16 point_services{
        &point_fixture, invoke_faery_point_command};
    const float point[3]{3.f, 4.f, 5.f};
    dh2::character_faery_controller_v1::Result point_result{};
    assert(dh2::character_faery_controller_v1::point_command(*selected,
        dh2::character::controller_head_to_point, point, 1, 0, 0,
        point_services, &point_result, error) ==
        dh2::character_faery_controller_v1::Status::complete);
    assert(!point_result.dispatched && point_fixture.calls == 0);
    assert(dh2::character_faery_controller_v1::point_command(*selected,
        dh2::character::controller_head_to_point, point, 0, 0, 0,
        point_services, &point_result, error) ==
        dh2::character_faery_controller_v1::Status::complete);
    assert(point_result.dispatched && point_fixture.calls == 1 &&
        point_fixture.last.owner == selected->character.identity &&
        point_fixture.last.command == dh2::character::controller_head_to_point &&
        point_fixture.last.point[0] == point[0] &&
        point_fixture.last.point[1] == point[1] &&
        point_fixture.last.point[2] == point[2]);
    std::size_t occurrences = 0;
    assert(selected_roster.contains_identity(selected->character.identity, &occurrences));
    assert(occurrences == 1);
    assert(selected_factory.retire(71, &result, error) == factory::Status::complete);
    assert(selected_roster.owned_nodes() == 0 && selected_objects.object_count() == 0);

    std::puts("{\"stable_owner\":true,\"constructor_order\":true,\"kill_field_14e4_constructor_projection\":true,\"source_item_inventory_constructor\":true,\"net_state_pair\":true,\"canonical_timer_fsm_owner\":true,\"nonplayer_init_post_prefix\":true,\"nonplayer_ai_postload_branch\":true,\"nonplayer_init_post_fx_animation_sound_chain\":true,\"script_created_character_flag\":true,\"script_created_kill_objective_gate\":true,\"init_final_gated\":true,\"source_save_slot_null_and_player_mask2\":true,\"registered_states\":20,\"manager_roster_publication\":true,\"constructor_and_roster_rollback\":true,\"staged_lifecycle_order\":true,\"premature_stage_rejection\":true,\"lifecycle_failure_retry\":true,\"lifecycle_idempotence\":true,\"faery_session_retirement_gate\":true,\"retirement\":true,\"native_roster_adapter\":true}");
    return 0;
}
