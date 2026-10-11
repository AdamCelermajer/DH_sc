#include "../monster_external_script_session.hpp"
#include "../ais_external_init_callbacks.hpp"
#include "../../android-native/app/src/main/cpp/player_gameplay_audio.hpp"

#include <cstdio>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::monster_external_script;

namespace {
constexpr std::uintptr_t owner = 0x1234567800000011ull;
constexpr std::uintptr_t enemy = 0x2345678900000023ull;
constexpr std::uintptr_t other = 0x3456789a00000035ull;

void require(bool value, const char* reason) {
    if (!value) throw std::runtime_error(reason);
}
std::string read(const char* path) {
    std::ifstream stream(path, std::ios::binary);
    require(bool(stream), "original script fixture missing");
    return {std::istreambuf_iterator<char>(stream), {}};
}
Source source(const std::string& bytes) { return {bytes.data(), bytes.size()}; }

struct Fixture {
    Session* session = nullptr;
    Source commons{}, monster{};
    std::vector<std::string> trace;
    std::uintptr_t target = 0;
    std::uintptr_t master = 0, last_master = 0, last_warp_master = 0;
    std::int32_t state = 3;
    std::uint32_t path = 0, has_target_override = 0;
    bool override_has_target = false;
    bool property_fails = false;
    bool set_fails_after_mutation = false;
    bool head_throws = false;
    bool reenter = false;
    bool change_target_during_path = false;
    bool store_other_target = false;
    Status reenter_dispatch{}, reenter_reset{}, reenter_initialize{};
    std::uintptr_t last_face = 0, last_move = 0;
    std::uintptr_t last_attack = 0;
    unsigned property_reads = 0, faces = 0, moves = 0, stops = 0, attacks = 0;
    unsigned master_sets = 0, target_clears = 0, warps = 0;
    unsigned play_sound_calls = 0, stop_sound_calls = 0;
    std::string sound_label, stopped_sound_label;
    std::size_t sound_label_bytes = 0, stopped_sound_label_bytes = 0;
    bool sound_looping = false, sound_stop_music = false;
    float sound_fade_ms = 0, stopped_sound_fade_ms = 0;
};

Fixture& actor(void* raw, std::uintptr_t identity) {
    require(identity == owner, "borrowed owner identity changed");
    return *static_cast<Fixture*>(raw);
}
std::int32_t structure(void* raw, const char* category, const char* member,
                        std::int32_t* value) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.trace.emplace_back("Struct");
    require(std::string(category) == "CharacterProperties" && std::string(member) == "SkillTree",
            "original load-time field query changed");
    *value = 28;  // Exact original static field table; provider remains borrowed.
    return 0;
}
std::int32_t property(void* raw, std::uintptr_t identity, std::int32_t id, float* value) {
    auto& fixture = actor(raw, identity);
    fixture.trace.emplace_back("Prop");
    require(id == 28, "SkillTree property key changed");
    ++fixture.property_reads;
    *value = -256.0f;
    return fixture.property_fails ? 1 : 0;
}
std::int32_t constant(void* raw, const char* category, const char* member,
                       std::int32_t* value) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.trace.emplace_back("Idle");
    require(std::string(category) == "AIStates" &&
                (std::string(member) == "Idle" || std::string(member) == "Move"),
            "original AI state query changed");
    *value = std::string(member) == "Idle" ? 3 : 4;
    return 0;
}
Services bind(Fixture&);
std::int32_t has_target(void* raw, std::uintptr_t identity, std::uint32_t* value) {
    auto& fixture = actor(raw, identity);
    fixture.trace.emplace_back("HasTarget");
    if (fixture.reenter) {
        std::string error;
        fixture.reenter_dispatch = fixture.session->dispatch(Event::enemy_spotted, enemy, error);
        fixture.reenter_reset = fixture.session->reset(error);
        fixture.reenter_initialize = fixture.session->initialize(fixture.commons, fixture.monster, bind(fixture), error);
    }
    *value = fixture.override_has_target ? fixture.has_target_override : fixture.target != 0;
    return 0;
}
std::int32_t get_target(void* raw, std::uintptr_t identity, std::uintptr_t* value) {
    auto& fixture = actor(raw, identity);
    fixture.trace.emplace_back("GetTarget");
    *value = fixture.target;
    return 0;
}
std::int32_t get_state(void* raw, std::uintptr_t identity, std::int32_t* value) {
    auto& fixture = actor(raw, identity);
    fixture.trace.emplace_back("State");
    *value = fixture.state;
    return 0;
}
std::int32_t has_path(void* raw, std::uintptr_t identity, std::uint32_t* value) {
    auto& fixture = actor(raw, identity);
    fixture.trace.emplace_back("Path");
    if (fixture.change_target_during_path) fixture.target = other;
    *value = fixture.path;
    return 0;
}
std::int32_t set_target(void* raw, std::uintptr_t identity, std::uintptr_t target) {
    auto& fixture = actor(raw, identity);
    fixture.trace.emplace_back("SetTarget");
    require(target == enemy, "enemy opaque identity lost through Lua table");
    fixture.target = fixture.store_other_target ? other : target;
    return fixture.set_fails_after_mutation ? 1 : 0;
}
std::int32_t head_to(void* raw, std::uintptr_t identity, std::uintptr_t target) {
    auto& fixture = actor(raw, identity);
    fixture.trace.emplace_back("HeadTo");
    if (fixture.head_throws) throw std::runtime_error("native HeadTo exception");
    fixture.last_face = target;
    ++fixture.faces;
    return 0;
}
std::int32_t move_to(void* raw, std::uintptr_t identity, std::uintptr_t target) {
    auto& fixture = actor(raw, identity);
    fixture.trace.emplace_back("MoveTo");
    fixture.last_move = target;
    ++fixture.moves;
    return 0;
}
std::int32_t actor_stop(void* raw, std::uintptr_t identity) {
    auto& fixture = actor(raw, identity);
    fixture.trace.emplace_back("Stop");
    ++fixture.stops;
    return 0;
}
std::int32_t has_master(void* raw,std::uintptr_t identity,std::uint32_t* value) {
    auto& fixture=actor(raw,identity);fixture.trace.emplace_back("HasMaster");
    *value=fixture.master!=0;return 0;
}
std::int32_t set_master(void* raw,std::uintptr_t identity,std::uintptr_t master) {
    auto& fixture=actor(raw,identity);fixture.trace.emplace_back("SetMaster");
    fixture.master=fixture.last_master=master;++fixture.master_sets;return 0;
}
std::int32_t clear_target(void* raw,std::uintptr_t identity) {
    auto& fixture=actor(raw,identity);fixture.trace.emplace_back("ClearTarget");
    fixture.target=0;++fixture.target_clears;return 0;
}
std::int32_t warp_behind(void* raw,std::uintptr_t identity,std::uintptr_t master) {
    auto& fixture=actor(raw,identity);fixture.trace.emplace_back("WarpBehind");
    fixture.last_warp_master=master;++fixture.warps;return 0;
}
std::int32_t actor_attack(void* raw, std::uintptr_t identity, std::uintptr_t target) {
    auto& fixture = actor(raw, identity);
    fixture.trace.emplace_back("Attack");
    fixture.last_attack = target;
    ++fixture.attacks;
    return 0;
}
std::int32_t play_sound(void* raw, const char* label, std::size_t label_bytes,
                        bool looping, float fade_ms, bool stop_music) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.trace.emplace_back("PlaySound");
    require(label && label_bytes == std::string("sfx_skill_mage_thunder_braid_add_2").size() &&
                std::string(label, label_bytes) == "sfx_skill_mage_thunder_braid_add_2",
            "source PlaySound label or byte length changed");
    fixture.sound_label.assign(label, label_bytes);
    fixture.sound_label_bytes = label_bytes;
    fixture.sound_looping = looping;
    fixture.sound_fade_ms = fade_ms;
    fixture.sound_stop_music = stop_music;
    ++fixture.play_sound_calls;
    return dh2::player_gameplay_audio::play_source_sound(raw,label,label_bytes,looping,fade_ms,stop_music);
}
std::int32_t stop_sound(void* raw, const char* label, std::size_t label_bytes, float fade_ms) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.trace.emplace_back("StopSound");
    require(label && std::string(label, label_bytes) == "sfx_skill_mage_thunder_braid_add_2",
            "source StopSound label changed");
    fixture.stopped_sound_label.assign(label, label_bytes);
    fixture.stopped_sound_label_bytes = label_bytes;
    fixture.stopped_sound_fade_ms = fade_ms;
    ++fixture.stop_sound_calls;
    return dh2::player_gameplay_audio::stop_source_sound(raw,label,label_bytes,fade_ms);
}
Services bind(Fixture& fixture) {
    Services services{&fixture, owner, structure, property, constant, has_target, get_target,
                      get_state, has_path, set_target, head_to, move_to};
    services.stop = actor_stop;
    services.attack = actor_attack;
    services.play_sound = play_sound;
    services.stop_sound = stop_sound;
    services.has_master = has_master;
    services.set_master = set_master;
    services.clear_target = clear_target;
    services.warp_behind = warp_behind;
    return services;
}
void initialized(Session& session, Fixture& fixture, Source commons, Source monster) {
    fixture.session = &session;
    fixture.commons = commons;
    fixture.monster = monster;
    std::string error;
    require(session.initialize(commons, monster, bind(fixture), error) == Status::complete && error.empty(),
            "original monster session initialization failed");
    require(session.statistics().source_libraries_opened == 4 &&
                session.statistics().source_functions_bound == 35,
            "AIS BindFunction must open the four source libraries and register 35 ordered globals");
    require(fixture.trace == std::vector<std::string>{"Struct", "Prop"} && fixture.property_reads == 1,
            "original top-level query order or unintended OnInit changed");
    fixture.trace.clear();
}
void trace(const Fixture& fixture, std::initializer_list<const char*> expected) {
    std::vector<std::string> values(expected.begin(), expected.end());
    require(fixture.trace == values, "original Lua callback service order changed");
}
void audio_bridge_case(Source commons, Source monster) {
    const std::string extension=std::string(static_cast<const char*>(monster.bytes),monster.size)+R"lua(
function NativeAudioBridge()
    PlaySound('sfx_skill_mage_thunder_braid_add_2', false, 0, false)
    StopSound('sfx_skill_mage_thunder_braid_add_2', 0)
    StopSound('sfx_skill_mage_thunder_braid_add_2', 125.9)
end
AddToVFTable('OnTargetInMeleeRange', 'NativeAudioBridge')
)lua";
    Session audio_session;Fixture audio_fixture;
    std::string error;
    initialized(audio_session,audio_fixture,commons,source(extension));
    require(audio_session.dispatch(Event::target_in_melee_range,owner,error)==Status::complete &&
            audio_fixture.play_sound_calls==1 && audio_fixture.stop_sound_calls==2 &&
            audio_fixture.sound_label=="sfx_skill_mage_thunder_braid_add_2" &&
            audio_fixture.sound_label_bytes==audio_fixture.sound_label.size() &&
            !audio_fixture.sound_looping && audio_fixture.sound_fade_ms==0.0f &&
            !audio_fixture.sound_stop_music &&
            audio_fixture.stopped_sound_label==audio_fixture.sound_label &&
            audio_fixture.stopped_sound_label_bytes==audio_fixture.sound_label_bytes &&
            audio_fixture.stopped_sound_fade_ms==125.9f,
            "AIS PlaySound/StopSound source argument order or values changed");
    std::string queued_audio;
    require(dh2::player_gameplay_audio::consume(queued_audio) &&
            queued_audio=="dh2fx,play,"+std::to_string(reinterpret_cast<std::uintptr_t>(&audio_fixture))+
                ",118,0,0,sfx_skill_mage_thunder_braid_add_2.wav" &&
            dh2::player_gameplay_audio::consume(queued_audio) &&
            queued_audio=="dh2fx,stop,"+std::to_string(reinterpret_cast<std::uintptr_t>(&audio_fixture))+",118,0" &&
            dh2::player_gameplay_audio::consume(queued_audio) &&
            queued_audio=="dh2fx,stop,"+std::to_string(reinterpret_cast<std::uintptr_t>(&audio_fixture))+",118,125" &&
            !dh2::player_gameplay_audio::consume(queued_audio),
            "monster PlaySound/StopSound must enqueue UID-scoped commands to the shared audio queue");
    trace(audio_fixture,{"PlaySound","StopSound","StopSound"});

    Session unsupported_stop;Fixture unsupported_fixture;
    auto unsupported_services=bind(unsupported_fixture);
    unsupported_services.stop_sound=nullptr;
    unsupported_fixture.session=&unsupported_stop;
    unsupported_fixture.commons=commons;
    const auto unsupported_extension=std::string(static_cast<const char*>(monster.bytes),monster.size)+R"lua(
function NativeUnsupportedStop()
    PlaySound('sfx_skill_mage_thunder_braid_add_2', false, 0, false)
    StopSound('sfx_skill_mage_thunder_braid_add_2', 0)
    StopSound('sfx_skill_mage_thunder_braid_add_2')
end
AddToVFTable('OnTargetInMeleeRange', 'NativeUnsupportedStop')
)lua";
    require(unsupported_stop.initialize(commons,source(unsupported_extension),unsupported_services,error)==Status::complete,
            "unsupported StopSound fixture must initialize before callback");
    const auto unsupported_status=unsupported_stop.dispatch(Event::target_in_melee_range,owner,error);
    require(unsupported_status==Status::script_error && unsupported_fixture.play_sound_calls==1 &&
            unsupported_fixture.stop_sound_calls==0 && error.find("monster StopSound failed or unsupported")!=std::string::npos,
            "missing StopSound provider must fail after the source PlaySound side effect");

    Session missing_fade;Fixture missing_fade_fixture;
    const auto missing_fade_extension=std::string(static_cast<const char*>(monster.bytes),monster.size)+R"lua(
function NativeMissingStopFade()
    StopSound('sfx_skill_mage_thunder_braid_add_2')
end
AddToVFTable('OnTargetInMeleeRange', 'NativeMissingStopFade')
)lua";
    initialized(missing_fade,missing_fade_fixture,commons,source(missing_fade_extension));
    const auto missing_fade_status=missing_fade.dispatch(Event::target_in_melee_range,owner,error);
    require(missing_fade_status==Status::script_error && missing_fade_fixture.stop_sound_calls==0 &&
            error.find("unsupported monster StopSound arguments")!=std::string::npos,
            "source StopSound requires its numeric fade argument");
}
}  // namespace

int main(int argc, char** argv) {
    try {
        if (argc == 4 && std::string(argv[1]) == "--follower-only") {
            const auto commons_bytes=read(argv[2]),follower_bytes=read(argv[3]);
            const auto commons=source(commons_bytes),follower=source(follower_bytes);
            Session follower_session;Fixture follower_fixture;
            follower_fixture.session=&follower_session;
            auto services=bind(follower_fixture);
            std::string follower_error;
            require(follower_session.create(services,follower_error)==Status::complete &&
                    follower_session.bind_functions(follower_error)==Status::complete &&
                    follower_session.load_common(commons,follower_error)==Status::complete &&
                    follower_session.load_external_named(follower,
                        "data/scripts/ai/follower.luac",follower_error)==Status::complete &&
                    follower_session.ready(),
                    "source follower must load on the same external AIS VM");
            bool present=false;
            for (const auto event : {Event::friend_spotted,Event::master_out_of_sight,
                    Event::master_out_of_range,Event::master_in_ranged_range,
                    Event::master_in_close_range,Event::master_in_melee_range}) {
                const char* alias=follower_session.source_alias(event);
                require(alias&&follower_session.contains_source_alias(
                    event==Event::friend_spotted?"OnFriendSpotted":
                    event==Event::master_out_of_sight?"OnMasterOutOfSight":
                    event==Event::master_out_of_range?"OnMasterOutOfRange":
                    event==Event::master_in_ranged_range?"OnMasterInRangedRange":
                    event==Event::master_in_close_range?"OnMasterInCloseRange":
                    "OnMasterInMeleeRange",present)&&present,
                    "source follower callback alias missing from its AIS VF table");
            }
            const auto master_identity=0x2345678900000023ull;
            require(follower_session.dispatch(Event::friend_spotted,master_identity,
                        follower_error)==Status::complete&&
                    follower_fixture.master==master_identity&&
                    follower_fixture.master_sets==1,
                    "follower friend callback did not set its first live master");
            require(follower_session.dispatch(Event::friend_spotted,
                        0x3456789a00000035ull,follower_error)==Status::complete&&
                    follower_fixture.master==master_identity&&
                    follower_fixture.master_sets==1,
                    "follower replaced an existing master");
            require(follower_session.dispatch(Event::master_out_of_sight,0,
                        follower_error)==Status::complete&&follower_fixture.warps==1&&
                    follower_fixture.last_warp_master==master_identity,
                    "follower out-of-sight callback lost cached master identity");
            follower_fixture.state=3;
            require(follower_session.dispatch(Event::master_out_of_range,0,
                        follower_error)==Status::complete&&follower_fixture.moves==1&&
                    follower_fixture.last_move==master_identity&&
                    follower_fixture.target_clears==1,
                    "follower out-of-range MoveTo/ClearTarget order changed");
            follower_fixture.state=4;
            require(follower_session.dispatch(Event::master_out_of_range,0,
                        follower_error)==Status::complete&&follower_fixture.moves==1&&
                    follower_fixture.target_clears==2,
                    "follower Move state did not suppress only MoveTo");
            require(follower_session.dispatch(Event::master_in_ranged_range,0,
                        follower_error)==Status::complete&&
                    follower_session.dispatch(Event::master_in_close_range,0,
                        follower_error)==Status::complete&&
                    follower_session.dispatch(Event::master_in_melee_range,0,
                        follower_error)==Status::complete&&follower_fixture.stops==3,
                    "follower range callbacks did not route Stop through the same VM");
            std::puts("PASS: recovered follower AISExternal callbacks on one VM");
            return 0;
        }
        if (argc == 4 && std::string(argv[1]) == "--audio-only") {
            const auto commons_bytes=read(argv[2]),monster_bytes=read(argv[3]);
            audio_bridge_case(source(commons_bytes),source(monster_bytes));
            std::puts("PASS: monster AIS PlaySound/StopSound source forwarding");
            return 0;
        }
        require(argc == 3, "pass unchanged original commons and monster paths");
        const auto commons_bytes = read(argv[1]), monster_bytes = read(argv[2]);
        const auto commons = source(commons_bytes), monster = source(monster_bytes);
        unsigned cases = 0;
        std::string error;
        Session session;
        Fixture fixture;
        initialized(session, fixture, commons, monster);
        require(session.ready() && session.statistics().lua_memory_used > 0, "owned Lua state absent");
        require(std::string(session.source_alias(Event::enemy_spotted)) == "monster_OnEnemySpotted" &&
                    std::string(session.source_alias(Event::target_out_of_range)) == "monster_OnTargetOutOfRange" &&
                    std::string(session.source_alias(Event::died)) == "monster_OnDied" &&
                    session.source_alias(static_cast<Event>(999)) == nullptr,
                "original VFTable aliases must remain owned and visible");
        ++cases;

        const auto callbacks_before_death = session.statistics().completed_callbacks;
        require(session.dispatch(Event::died, enemy, error) == Status::complete &&
                    session.ready() &&
                    session.statistics().completed_callbacks == callbacks_before_death + 1 &&
                    session.statistics().failed_callbacks == 0,
                "source OnDied must execute on the existing VM");
        fixture.trace.clear();
        const std::string death_arity_script = monster_bytes + R"lua(
function NativeOnDiedArity(...)
    assert(select('#', ...) == 1, 'OnDied killer argument missing or duplicated')
    local killer = (...)
    assert(type(killer) == 'table' and type(killer._this) == 'userdata',
           'OnDied killer is not a GameObject userdata table')
    assert(killer._this == GetTarget()._this, 'OnDied killer identity changed')
end
AddToVFTable('OnDied', 'NativeOnDiedArity')
)lua";
        Session death_arity; Fixture death_arity_fixture;
        death_arity_fixture.target = enemy;
        initialized(death_arity, death_arity_fixture, commons, source(death_arity_script));
        require(death_arity.dispatch(Event::died, enemy, error) == Status::complete &&
                    death_arity.ready() &&
                    std::string(death_arity.source_alias(Event::died)) == "NativeOnDiedArity" &&
                    death_arity.statistics().completed_callbacks == 1,
                "event2 must pass its exact killer as one source userdata");
        ++cases;

        const std::string null_death_script = monster_bytes + R"lua(
function NativeNullOnDied(...)
    assert(select('#', ...) == 1, 'null OnDied killer argument missing')
    assert((...) == nil, 'null source killer must become Lua nil')
end
AddToVFTable('OnDied', 'NativeNullOnDied')
)lua";
        Session null_death; Fixture null_death_fixture;
        initialized(null_death, null_death_fixture, commons, source(null_death_script));
        require(null_death.dispatch(Event::died, 0, error) == Status::complete &&
                    null_death.ready() && null_death.statistics().completed_callbacks == 1,
                "null source killer must be one Lua nil argument");
        ++cases;

        const std::string combat_callback_script = monster_bytes + R"lua(
function NativeCombatResult(...)
    assert(select('#', ...) == 2, 'combat callback must receive attacker and defender')
    local attacker, defender = ...
    assert(type(attacker) == 'table' and type(attacker._this) == 'userdata',
           'attacker must be original Character userdata')
    assert(type(defender) == 'table' and type(defender._this) == 'userdata',
           'defender must be original Character userdata')
    assert(attacker._this ~= defender._this, 'combat identities were aliased')
    SetTarget(attacker)
    HeadTo(defender)
end
AddToVFTable('OnTargetHit', 'NativeCombatResult')
AddToVFTable('OnTargetMissed', 'NativeCombatResult')
)lua";
        Session combat_callback; Fixture combat_fixture;
        combat_fixture.session = &combat_callback;
        combat_fixture.commons = commons;
        combat_fixture.monster = source(combat_callback_script);
        require(combat_callback.initialize(commons, source(combat_callback_script),
                    bind(combat_fixture), error) == Status::complete,
                "source combat callback fixture failed to initialize");
        // Initialization itself performs the source Struct/Prop queries.
        // Clear those before asserting the runtime combat callback order.
        combat_fixture.trace.clear();
        const auto combat_vm = combat_callback.vm_identity();
        require(combat_callback.source_alias(Event::target_hit) ==
                    std::string("NativeCombatResult") &&
                combat_callback.source_alias(Event::target_missed) ==
                    std::string("NativeCombatResult"),
                "combat callbacks must resolve through the retained AIS VFTable");
        require(combat_callback.dispatch_combat_result(Event::target_hit, enemy, other, error) ==
                    Status::complete && combat_callback.vm_identity() == combat_vm &&
                combat_fixture.target == enemy && combat_fixture.last_face == other &&
                combat_fixture.faces == 1 && combat_callback.statistics().completed_callbacks == 1 &&
                combat_callback.statistics().projected_object_table_arguments == 4,
                "OnTargetHit must call the existing VM with original attacker/defender order");
        trace(combat_fixture, {"SetTarget", "HeadTo"});
        combat_fixture.trace.clear();
        require(combat_callback.dispatch_combat_result(Event::target_missed, enemy, other, error) ==
                    Status::complete && combat_callback.vm_identity() == combat_vm &&
                combat_fixture.last_face == other && combat_fixture.faces == 2 &&
                combat_callback.statistics().completed_callbacks == 2 &&
                combat_callback.statistics().projected_object_table_arguments == 8,
                "OnTargetMissed must dispatch through the same retained VM");
        trace(combat_fixture, {"SetTarget", "HeadTo"});
        require(combat_callback.dispatch(Event::target_hit, enemy, error) == Status::invalid_argument &&
                combat_callback.dispatch_combat_result(Event::target_in_melee_range, enemy, other, error) ==
                    Status::unsupported_callback,
                "one-argument or non-combat callback routes must fail closed");
        ++cases;

        Session melee_range; Fixture melee_range_fixture;
        melee_range_fixture.target = enemy;
        initialized(melee_range, melee_range_fixture, commons, monster);
        require(std::string(melee_range.source_alias(Event::target_in_melee_range)) ==
                    "monster_OnTargetInMeleeRange" &&
                    melee_range.dispatch(Event::target_in_melee_range, 0, error) == Status::complete &&
                    melee_range.ready() && melee_range_fixture.stops == 1 &&
                    melee_range_fixture.attacks == 1 && melee_range_fixture.last_attack == enemy,
                "source event 17 must execute OnTargetInMeleeRange on the same VM");
        trace(melee_range_fixture, {"Stop", "GetTarget", "Attack"});
        ++cases;

        const std::string attack_script = monster_bytes + R"lua(
function NativeAttackContract(...)
    assert(select('#', ...) == 1, 'OnDied killer argument missing or duplicated')
    local killer = (...)
    assert(type(killer) == 'table' and type(killer._this) == 'userdata',
           'OnDied killer is not a GameObject userdata table')
    Stop()
    local function attack_with_one_target(...)
        assert(select('#', ...) == 1, 'Attack target arity changed')
        Attack(...)
    end
    attack_with_one_target(GetTarget())
    Attack(false)
end
AddToVFTable('OnDied', 'NativeAttackContract')
)lua";
        Session attack_session; Fixture attack_fixture;
        attack_fixture.target = enemy;
        initialized(attack_session, attack_fixture, commons, source(attack_script));
        auto different_attack_services = bind(attack_fixture);
        different_attack_services.attack = nullptr;
        require(attack_session.dispatch(Event::died, enemy, error) == Status::complete &&
                    attack_session.ready() && attack_fixture.stops == 1 &&
                    attack_fixture.attacks == 1 && attack_fixture.last_attack == enemy &&
                    attack_session.statistics().projected_object_table_arguments == 1 &&
                    !attack_session.uses_services(different_attack_services),
                "OnDied killer and Attack target must preserve source arity and identity");
        trace(attack_fixture, {"Stop", "GetTarget", "Attack"});
        ++cases;

        Session missing_attack; Fixture missing_attack_fixture;
        missing_attack_fixture.target = enemy;
        missing_attack_fixture.session = &missing_attack;
        missing_attack_fixture.commons = commons;
        missing_attack_fixture.monster = source(attack_script);
        auto missing_attack_services = bind(missing_attack_fixture);
        missing_attack_services.attack = nullptr;
        require(missing_attack.initialize(commons, source(attack_script), missing_attack_services, error) ==
                    Status::complete,
                "missing-attack regression session failed to initialize");
        missing_attack_fixture.trace.clear();
        require(missing_attack.dispatch(Event::died, enemy, error) == Status::script_error &&
                    missing_attack_fixture.stops == 1 && missing_attack_fixture.attacks == 0 &&
                    !missing_attack.ready(),
                "missing valid-target Attack provider must fail closed after Stop");
        trace(missing_attack_fixture, {"Stop", "GetTarget"});
        ++cases;

        const std::string implicit_attack_script = monster_bytes + R"lua(
function NativeImplicitAttack(...)
    assert(select('#', ...) == 1, 'OnDied killer argument missing or duplicated')
    Stop()
    Attack()
end
AddToVFTable('OnDied', 'NativeImplicitAttack')
)lua";
        Session implicit_attack; Fixture implicit_attack_fixture;
        initialized(implicit_attack, implicit_attack_fixture, commons, source(implicit_attack_script));
        require(implicit_attack.dispatch(Event::died, enemy, error) == Status::script_error &&
                    implicit_attack_fixture.stops == 1 && implicit_attack_fixture.attacks == 0 &&
                    !implicit_attack.ready(),
                "unmodeled ReturnValues target for no-argument Attack must fail closed");
        trace(implicit_attack_fixture, {"Stop"});
        ++cases;

        const std::string null_identity_attack_script = monster_bytes + R"lua(
function NativeNullIdentityAttack(...)
    assert(select('#', ...) == 1, 'OnDied killer argument missing or duplicated')
    Stop()
    Attack({})
end
AddToVFTable('OnDied', 'NativeNullIdentityAttack')
)lua";
        Session null_identity_attack; Fixture null_identity_fixture;
        initialized(null_identity_attack, null_identity_fixture, commons, source(null_identity_attack_script));
        require(null_identity_attack.dispatch(Event::died, enemy, error) == Status::script_error &&
                    null_identity_fixture.stops == 1 && null_identity_fixture.attacks == 0 &&
                    !null_identity_attack.ready(),
                "type-7 userdata with null identity must not report a successful Attack");
        trace(null_identity_fixture, {"Stop"});
        ++cases;

        fixture.store_other_target = true;
        require(session.dispatch(Event::enemy_spotted, enemy, error) == Status::complete &&
                    fixture.target == other && fixture.last_face == enemy && fixture.faces == 1,
                "spotted callback must face its original parameter after live target mutation");
        trace(fixture, {"HasTarget", "SetTarget", "HeadTo"});
        require(session.statistics().projected_object_table_arguments == 2,
                "enemy must reach both native object actions as original table type7");
        ++cases;
        fixture.trace.clear();
        require(session.dispatch(Event::enemy_spotted, enemy, error) == Status::complete && fixture.faces == 1,
                "existing target prevents replacement or facing");
        trace(fixture, {"HasTarget"});
        ++cases;

        fixture.trace.clear(); fixture.state = 4;
        require(session.dispatch(Event::target_out_of_range, 0, error) == Status::complete,
                "non-Idle callback failed");
        trace(fixture, {"State", "Idle"});
        ++cases;
        fixture.trace.clear(); fixture.state = 3; fixture.path = 1;
        require(session.dispatch(Event::target_out_of_range, 0, error) == Status::complete && fixture.moves == 0,
                "existing path should gate movement");
        trace(fixture, {"State", "Idle", "Path"});
        ++cases;
        fixture.trace.clear(); fixture.path = 0; fixture.target = enemy;
        fixture.change_target_during_path = true;
        require(session.dispatch(Event::target_out_of_range, 0, error) == Status::complete &&
                    fixture.last_move == other && fixture.moves == 1,
                "GetTarget must be read after HasPath and retain its full opaque identity");
        trace(fixture, {"State", "Idle", "Path", "GetTarget", "MoveTo"});
        require(session.statistics().projected_object_table_arguments == 3,
                "GetTarget wrapper must reach MoveTo as original table type7");
        ++cases;
        fixture.trace.clear(); fixture.change_target_during_path = false; fixture.target = 0;
        require(session.dispatch(Event::target_out_of_range, 0, error) == Status::complete && fixture.moves == 1,
                "MoveTo(nil) should take source non-object no-op overload");
        trace(fixture, {"State", "Idle", "Path", "GetTarget"});
        ++cases;

        fixture.trace.clear();
        require(session.dispatch(static_cast<Event>(999), enemy, error) == Status::unsupported_callback &&
                    fixture.trace.empty() && session.ready(), "unknown callback must fail before native services");
        ++cases;
        require(session.dispatch(Event::enemy_spotted, 0, error) == Status::invalid_argument &&
                    fixture.trace.empty() && session.ready(), "missing enemy must fail before script dispatch");
        ++cases;

        fixture.reenter = true; fixture.store_other_target = false;
        require(session.dispatch(Event::enemy_spotted, enemy, error) == Status::complete &&
                    fixture.reenter_dispatch == Status::busy && fixture.reenter_reset == Status::busy &&
                    fixture.reenter_initialize == Status::busy && session.ready(),
                "all session mutation reentry must be rejected without destroying active VM");
        trace(fixture, {"HasTarget", "SetTarget", "HeadTo"});
        ++cases;
        fixture.reenter = false; fixture.trace.clear();

        const std::string malformed = "function broken(";
        const auto before = session.statistics();
        const char* retained_alias = session.source_alias(Event::enemy_spotted);
        require(session.initialize(commons, source(malformed), bind(fixture), error) == Status::script_error &&
                    session.ready() && session.statistics().completed_callbacks == before.completed_callbacks &&
                    session.source_alias(Event::enemy_spotted) == retained_alias,
                "malformed candidate must preserve earlier VM and alias map");
        ++cases;
        fixture.property_fails = true;
        require(session.initialize(commons, monster, bind(fixture), error) == Status::script_error && session.ready(),
                "load-time provider failure must preserve earlier VM");
        ++cases;
        fixture.property_fails = false;
        require(session.initialize(commons, monster, bind(fixture), error, 1) == Status::allocation_failed && session.ready(),
                "candidate allocation rejection must preserve earlier VM");
        ++cases;
        const std::string memory_stress = "local unused='" + std::string(100000, 'x') + "';";
        require(session.initialize(commons, source(memory_stress), bind(fixture), error, 65536) == Status::script_error &&
                    error.find("not enough memory") != std::string::npos && session.ready(),
                "actual Lua allocation failure must preserve the prior VM");
        ++cases;
        const unsigned char bytecode[]{0x1b, 'L', 'u', 'a'};
        require(session.initialize(commons, {bytecode, sizeof(bytecode)}, bind(fixture), error) == Status::invalid_argument &&
                    session.ready(), "session is source-only");
        ++cases;

        Session copied_session; Fixture copied_fixture;
        auto copied_services = bind(copied_fixture);
        require(copied_session.initialize(commons, monster, copied_services, error) == Status::complete,
                "copied service initialization");
        copied_services.has_target = nullptr;
        copied_fixture.trace.clear();
        require(copied_session.dispatch(Event::enemy_spotted, enemy, error) == Status::complete &&
                    copied_fixture.target == enemy, "service table must be copied rather than borrowed stack storage");
        ++cases;
        require(session.reset(error) == Status::complete && !session.ready() && copied_session.ready() &&
                    copied_session.dispatch(Event::target_out_of_range, 0, error) == Status::complete,
                "one session teardown must not invalidate another VM/map/context");
        ++cases;
        require(session.dispatch(Event::enemy_spotted, enemy, error) == Status::not_ready,
                "closed session rejects dispatch");
        ++cases;

        Session missing; Fixture missing_fixture;
        auto missing_services = bind(missing_fixture);
        missing_services.has_target = nullptr;
        require(missing.initialize(commons, monster, missing_services, error) == Status::complete &&
                    missing.dispatch(Event::enemy_spotted, enemy, error) == Status::script_error && !missing.ready() &&
                    missing.statistics().failed_callbacks == 1 &&
                    missing.dispatch(Event::enemy_spotted, enemy, error) == Status::not_ready,
                "missing used service faults session and prevents accidental replay");
        ++cases;

        Session unsupported_global; Fixture unsupported_fixture;
        const auto unsupported_services=bind(unsupported_fixture);
        const std::string unsupported_common_text="Trace('no source Trace provider')";
        const auto unsupported_common=source(unsupported_common_text);
        const auto unsupported_create=unsupported_global.create(unsupported_services,error);
        const auto unsupported_bind=unsupported_global.bind_functions(error);
        const auto unsupported_count=unsupported_global.statistics().source_functions_bound;
        const auto unsupported_load=unsupported_global.load_common(unsupported_common,error);
        require(unsupported_create==Status::complete&&unsupported_bind==Status::complete&&
                    unsupported_count==35&&unsupported_load==Status::script_error&&
                    unsupported_global.stage()==Stage::faulted&&!unsupported_global.ready()&&
                    error.find("unsupported source script function: Trace")!=std::string::npos,
                "registered but unavailable Trace must stop the pending source VM with an explicit error");
        ++cases;

        Session numeric_arity; Fixture numeric_fixture;
        const std::string numeric_arity_script =
            "AddToVFTable('OnEnemySpotted','monster_OnEnemySpotted')\n"
            "AddToVFTable('OnTargetOutOfRange','monster_OnTargetOutOfRange')\n"
            "function monster_OnEnemySpotted(enemy)\n"
            " if BitAnd()==nil and BitAnd(1)==nil and BitAnd(7,3)==3 and BitOr(4,1)==5 then SetTarget(enemy) end\n"
            "end\n"
            "function monster_OnTargetOutOfRange() end\n";
        require(numeric_arity.initialize(commons, source(numeric_arity_script), bind(numeric_fixture), error) ==
                    Status::complete &&
                    numeric_arity.dispatch(Event::enemy_spotted, enemy, error) == Status::complete &&
                    numeric_fixture.target == enemy,
                "registered numeric helpers must preserve zero-result invalid arity and valid results");
        ++cases;

        Session short_circuit; Fixture short_fixture; short_fixture.state = 4;
        auto short_services = bind(short_fixture); short_services.has_path = nullptr;
        require(short_circuit.initialize(commons, monster, short_services, error) == Status::complete,
                "unused service should not be a load requirement");
        short_fixture.trace.clear();
        require(short_circuit.dispatch(Event::target_out_of_range, 0, error) == Status::complete,
                "unused HasPath must not be called for non-Idle state");
        trace(short_fixture, {"State", "Idle"});
        ++cases;

        Session failing; Fixture failing_fixture; failing_fixture.set_fails_after_mutation = true;
        initialized(failing, failing_fixture, commons, monster);
        require(failing.dispatch(Event::enemy_spotted, enemy, error) == Status::script_error &&
                    failing_fixture.target == enemy && failing_fixture.faces == 0 && !failing.ready(),
                "action failure must retain prior native effect and stop later action");
        trace(failing_fixture, {"HasTarget", "SetTarget"});
        ++cases;
        Session throwing; Fixture throwing_fixture; throwing_fixture.head_throws = true;
        initialized(throwing, throwing_fixture, commons, monster);
        require(throwing.dispatch(Event::enemy_spotted, enemy, error) == Status::script_error &&
                    throwing_fixture.target == enemy && error.find("native HeadTo exception") != std::string::npos,
                "C++ callback exception must become protected Lua error without crossing C trampoline");
        trace(throwing_fixture, {"HasTarget", "SetTarget", "HeadTo"});
        ++cases;
        Session malformed_bool; Fixture bool_fixture;
        bool_fixture.override_has_target = true; bool_fixture.has_target_override = 2;
        initialized(malformed_bool, bool_fixture, commons, monster);
        require(malformed_bool.dispatch(Event::enemy_spotted, enemy, error) == Status::script_error && bool_fixture.faces == 0,
                "invalid adapter bool must not silently choose a gameplay branch");
        ++cases;
        require(throwing.initialize(commons, monster, bind(copied_fixture), error) == Status::complete && throwing.ready() &&
                    throwing.statistics().failed_callbacks == 0,
                "explicit source reinitialization recovers faulted session");
        ++cases;

        Session staged; Fixture staged_fixture; staged_fixture.session = &staged;
        staged_fixture.commons = commons; staged_fixture.monster = monster;
        const auto staged_services = bind(staged_fixture);
        require(staged.create(staged_services, error) == Status::complete &&
                    staged.stage() == Stage::created && !staged.ready() &&
                    staged.uses_services(staged_services),
                "staged AIS must create one owned VM with its stable actor services");
        ++cases;
        require(staged.load_common(commons, error) == Status::not_ready && staged_fixture.trace.empty(),
                "common load before BindFunction must have no effects");
        require(staged.bind_functions(error) == Status::complete &&
                    staged.stage() == Stage::functions_bound && !staged.ready(),
                "staged BindFunction transition failed");
        const auto staged_common_status = staged.load_common(commons, error);
        require(staged_common_status == Status::complete &&
                    staged.stage() == Stage::common_loaded && !staged.ready() &&
                    staged.source_alias(Event::enemy_spotted) == nullptr &&
                    staged_fixture.trace.empty(),
                "pending VM must preserve commons order and remain unpromoted");
        require(staged.load_external(monster, error) == Status::complete && staged.ready() &&
                    staged.stage() == Stage::external_loaded &&
                    std::string(staged.source_alias(Event::enemy_spotted)) == "monster_OnEnemySpotted" &&
                    staged_fixture.trace == std::vector<std::string>{"Struct", "Prop"},
                "source monster load must finish pending AIS aliases");
        require(staged.dispatch(Event::enemy_spotted, enemy, error) == Status::complete &&
                    staged_fixture.target == enemy && staged_fixture.last_face == enemy,
                "same staged pending VM must become the dispatched actor VM");
        trace(staged_fixture, {"Struct", "Prop", "HasTarget", "SetTarget", "HeadTo"});
        ++cases;

        Session staged_failure; Fixture staged_failure_fixture;
        const auto staged_failure_services = bind(staged_failure_fixture);
        const auto malformed_external = source(malformed);
        require(staged_failure.create(staged_failure_services, error) == Status::complete &&
                    staged_failure.bind_functions(error) == Status::complete &&
                    staged_failure.load_common(commons, error) == Status::complete &&
                    staged_failure.load_external(malformed_external, error) == Status::script_error &&
                    staged_failure.stage() == Stage::faulted && !staged_failure.ready() &&
                    staged_failure.load_external(monster, error) == Status::not_ready &&
                    staged_failure.dispatch(Event::enemy_spotted, enemy, error) == Status::not_ready,
                "a failed staged source load must stop without replay/fallback");
        ++cases;

        Session split; Fixture split_fixture;
        const auto split_services = bind(split_fixture);
        require(split.create(split_services, error) == Status::complete &&
                    split.bind_character_functions(error) == Status::not_ready &&
                    split.bind_ais_functions(error) == Status::complete &&
                    split.stage() == Stage::ais_functions_bound &&
                    split.load_common(commons, error) == Status::not_ready &&
                    split.bind_ais_functions(error) == Status::not_ready &&
                    split.bind_character_functions(error) == Status::complete &&
                    split.bind_character_functions(error) == Status::not_ready &&
                    split.load_common(commons, error) == Status::complete &&
                    split.load_external(monster, error) == Status::complete,
                "native AIS/Character registration split violated stage order");
        bool present = false;
        require(split.contains_source_alias("OnInit", present) && present &&
                    split.contains_source_alias("OnUpdate", present) && !present &&
                    split.contains_source_alias("OnEnemySpotted", present) && present &&
                    split.dispatch(Event::enemy_spotted, enemy, error) == Status::complete &&
                    split_fixture.target == enemy,
                "native membership must read actual VFTable on the same staged VM");
        ++cases;

        {
            // Original commons post/final bodies are empty. They must execute
            // on this exact retained VM without invoking OnInit or its stats.
            Session retained; Fixture live;
            initialized(retained, live, commons, monster);
            const auto bound=bind(live);
            namespace init=dh2::ais_external_init_callbacks;
            init::State ais{0x456789ab00000047ull};init::Result result{};
            const init::Services caller{&retained,[](void* raw,init::State*,const init::Request* request)->std::int32_t {
                std::string failure;
                const auto event=request->callback==init::Callback::post?Event::init_post:Event::init_final;
                return static_cast<Session*>(raw)->dispatch(event,0,failure)==Status::complete?0:1;
            }};
            require(init::invoke(&ais,init::Callback::post,&caller,&result)==init::Status::complete && result.calls==1 && !result.default_init_completed &&
                    init::invoke(&ais,init::Callback::final,&caller,&result)==init::Status::complete && result.calls==1 && !result.default_init_completed &&
                    retained.ready() && retained.uses_services(bound) && live.trace.empty() && live.property_reads==1 &&
                    retained.statistics().completed_callbacks==2 && retained.statistics().failed_callbacks==0,
                    "post/final callers must retain the VM and avoid replaying OnInit");
            require(std::string(retained.source_alias(Event::init_post))=="OnInitPost" &&
                    std::string(retained.source_alias(Event::init_final))=="OnInitFinal",
                    "common post/final use current source alias fallback");
            ++cases;
        }
        {
            // A fan callback returns a table whose ordinary _this lookup adds
            // a Final alias. The next call must observe that discarded return
            // effect through the same source alias map and VM.
            const std::string extension=std::string(monster_bytes.begin(),monster_bytes.end())+R"lua(
function NativePostProof()
    HasTarget()
    return setmetatable({}, {__index=function(_, key)
        assert(key=='_this')
        AddToVFTable('OnInitFinal', 'NativeFinalProof')
        return nil
    end})
end
function NativeFinalProof() HasTarget() end
AddToVFTable('OnInitPost', 'NativePostProof')
)lua";
            Session retained;Fixture live;
            initialized(retained,live,commons,source(extension));
            require(retained.dispatch(Event::init_post,0,error)==Status::complete &&
                    std::string(retained.source_alias(Event::init_final))=="NativeFinalProof" &&
                    retained.dispatch(Event::init_final,0,error)==Status::complete &&
                    retained.statistics().completed_callbacks==2 && retained.ready(),
                    "post discarded returns and fresh final alias resolution lost");
            trace(live,{"HasTarget","HasTarget"});++cases;
        }
        {audio_bridge_case(commons,monster);++cases;}

        std::printf("{\"monster_external_session_cases\":%u,\"unchanged_original_scripts_executed\":true,"
                    "\"spotted_callback_order\":true,\"idle_path_short_circuit\":true,"
                    "\"fresh_target_after_path_query\":true,\"opaque_64bit_identity_tables\":true,"
                    "\"service_lifetime_and_reentry\":true,\"failure_preserves_prior_effects\":true,"
                    "\"staged_same_vm_lifecycle\":true,\"staged_errors_stop_without_fallback\":true,"
                    "\"source_libraries_and_35_bindings\":true,\"unsupported_globals_fail_closed\":true,"
                    "\"numeric_result_arity\":true,\"unknown_callbacks_rejected\":true,"
                    "\"same_vm_post_final_callbacks\":true,\"post_discarded_return_updates_final_alias\":true,"
                    "\"death_callback_same_vm\":true,\"death_killer_identity_and_nil\":true,"
                    "\"combat_callback_two_character_abi\":true,"
                    "\"stop_attack_callbacks\":true,"
                    "\"native_wired\":false,\"mismatches\":0}\n", cases);
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "monster external session: %s\n", error.what());
        return 1;
    }
}
