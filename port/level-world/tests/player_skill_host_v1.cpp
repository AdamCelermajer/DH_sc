// Host test for native_player_skill_host_v1: argument checks, defaults and
// return arity of the GameObject/Character natives the Player skill VM serves
// (PlayFX 0x3923cc, PlaySound3D 0x39212c, GetRand 0x393310, GetPropHP 0x3b6cd8,
// HasBow 0x3b6f84, CanAttackInMelee 0x3b6f64, RegenHP 0x3b77ec,
// SetIsTargetable 0x3b7a64, SetTarget 0x3b8f38, Kill 0x3b7e40,
// SetStunState 0x3b9580, SetScareState 0x3b96b0, SetKnockBackState 0x3b7680,
// GetState, Is*, GetPosition, GetName, LOCK/UNLOCK).
#include "../../android-native/app/src/main/cpp/native_player_skill_host_v1.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace host = dh2::native::player_skill_host_v1;
using Fn = dh2::character_native_bindings::Function;

namespace {
constexpr std::uintptr_t SELF = 0x100000001ull, ENEMY = 0x100000042ull;

void check(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "FAIL: %s\n", message);
        std::exit(1);
    }
}

dh2_script_value num(float n) {
    dh2_script_value v{};
    v.type = DH2_SCRIPT_NUMBER;
    v.number = n;
    return v;
}
dh2_script_value boolean(bool b) {
    dh2_script_value v{};
    v.type = DH2_SCRIPT_BOOLEAN;
    v.boolean = b ? 1u : 0u;
    return v;
}
dh2_script_value identity(std::uintptr_t id, std::uint32_t tag) {
    dh2_script_value v{};
    v.type = tag;
    v.identity = id;
    return v;
}
dh2_script_value str(const char* text) {
    dh2_script_value v{};
    v.type = DH2_SCRIPT_STRING;
    v.text = text;
    v.text_bytes = std::strlen(text);
    return v;
}

struct Fake {
    struct Fx { std::int32_t effect; std::uintptr_t attach; bool has_position; float position[3]; };
    struct Sound { std::string name; float position[3]; };
    struct Command { host::ObjectCommand command; std::uintptr_t receiver, other; std::uint32_t duration; bool flag; };
    std::vector<Fx> fx;
    std::vector<Sound> sounds;
    std::vector<Command> commands;
    std::vector<std::int32_t> regen;
    std::vector<std::pair<std::uintptr_t, bool>> targetable;
    std::vector<std::pair<std::uintptr_t, std::uintptr_t>> targets;
    std::uint32_t weapon = 0;
    bool in_melee = true;
    struct Buff { host::BuffCommand command; std::uintptr_t receiver; std::uint32_t count; };
    std::vector<Buff> buffs;
    std::vector<std::uintptr_t> melee_queries;
    bool fail_next = false;
    dh2_random_state random{{1234, 99}, {0, 0}};
    std::uint8_t online = 0;
    std::uint32_t seed = 0;

    static Fake& self(void* p) { return *static_cast<Fake*>(p); }
    static int facts(void* p, std::uintptr_t id, host::ObjectFacts* out) {
        (void)p;
        *out = {};
        if (id == SELF) {
            out->is_character = out->is_player = true;
            out->state = 3;
            out->hp_raw = 100 * 256;
            out->max_hp_raw = 200 * 256;
            out->position[0] = 10.f; out->position[1] = 20.f; out->position[2] = 30.f;
            std::strcpy(out->name, "PlayerCharacter_0");
            return 0;
        }
        if (id == ENEMY) {
            out->is_character = true;
            out->is_dead = true;
            out->state = 12;
            out->position[0] = 1.f; out->position[1] = 2.f; out->position[2] = 3.f;
            out->ai_flags = 0x44; out->has_ai_flags = true;
            std::strcpy(out->name, "Skeleton_01");
            return 0;
        }
        if (id == 0x100000077ull) { // a door
            out->is_door = true;
            std::strcpy(out->name, "Door_a");
            return 0;
        }
        return 1;
    }
    static int play_fx(void* p, std::int32_t effect, std::uintptr_t attach, const float* position, std::string& error) {
        auto& f = self(p);
        if (f.fail_next) { error = "boom"; return 1; }
        Fx row{effect, attach, position != nullptr, {0, 0, 0}};
        if (position) std::memcpy(row.position, position, sizeof(row.position));
        f.fx.push_back(row);
        return 0;
    }
    static int play_sound(void* p, const char* name, std::size_t bytes, const float position[3], std::string&) {
        auto& f = self(p);
        Sound row{std::string(name, bytes), {position[0], position[1], position[2]}};
        f.sounds.push_back(row);
        return 0;
    }
    static int weapon_flags(void* p, std::uint32_t* flags, std::string&) {
        *flags = self(p).weapon;
        return 0;
    }
    static int regen_hp(void* p, std::int32_t amount, std::string&) {
        self(p).regen.push_back(amount);
        return 0;
    }
    static int set_targetable(void* p, std::uintptr_t receiver, bool value, std::string&) {
        self(p).targetable.emplace_back(receiver, value);
        return 0;
    }
    static int set_target(void* p, std::uintptr_t receiver, std::uintptr_t target, std::string&) {
        self(p).targets.emplace_back(receiver, target);
        return 0;
    }
    static int command(void* p, host::ObjectCommand c, std::uintptr_t receiver, std::uintptr_t other,
                       std::uint32_t duration, bool flag, std::string&) {
        self(p).commands.push_back({c, receiver, other, duration, flag});
        return 0;
    }

    static int buff(void* p, host::BuffCommand command, std::uintptr_t receiver, const dh2_script_value*,
                    std::uint32_t count, dh2_script_value* out, std::uint32_t, std::uint32_t* returned, char*, std::size_t) {
        self(p).buffs.push_back({command, receiver, count});
        if (command == host::BuffCommand::create) {
            out[0] = identity(0xb0ffull, DH2_SCRIPT_IDENTITY);
            *returned = 1;
        }
        return 0;
    }
    static int get_property(void*, std::uintptr_t receiver, std::uint32_t id, std::int32_t* value, std::string&) {
        if (receiver != ENEMY) return 1;
        *value = static_cast<std::int32_t>(id) * 256;
        return 0;
    }
    static int melee(void* p, std::uintptr_t receiver, bool* value, std::string&) {
        auto& f = self(p);
        f.melee_queries.push_back(receiver);
        *value = f.in_melee;
        return 0;
    }

    host::Services services() {
        host::Services s{};
        s.context = this;
        s.self = SELF;
        s.random = &random;
        s.online = &online;
        s.object_seed = &seed;
        s.object_facts = facts;
        s.play_fx = play_fx;
        s.play_sound_3d = play_sound;
        s.weapon_flags = weapon_flags;
        s.regen_hp = regen_hp;
        s.set_targetable = set_targetable;
        s.set_target = set_target;
        s.object_command = command;
        s.target_in_melee_range = melee;
        s.buff_command = buff;
        s.get_property = get_property;
        return s;
    }
};

struct Result {
    int status = 0;
    std::uint32_t returned = 99;
    std::array<dh2_script_value, 16> out{};
    char text[256]{};
};

Result run(const host::Services& s, Fn function, std::uintptr_t receiver,
           std::vector<dh2_script_value> args = {}) {
    Result r;
    r.status = host::invoke(s, function, "test", receiver, args.data(),
                            static_cast<std::uint32_t>(args.size()), r.out.data(),
                            static_cast<std::uint32_t>(r.out.size()), &r.returned, r.text, sizeof(r.text));
    return r;
}
} // namespace

int main() {
    // Ownership predicate: the natives this module serves, and none of the
    // ones with existing owners.
    for (auto f : {Fn::game_object_play_fx, Fn::game_object_play_sound3_d, Fn::game_object_rand,
                   Fn::game_object_get_self, Fn::game_object_get_id, Fn::game_object_get_position,
                   Fn::character_get_prop_hp, Fn::character_has_bow, Fn::character_can_attack_in_melee,
                   Fn::character_regen_hp, Fn::character_set_is_targetable, Fn::character_set_target,
                   Fn::character_kill, Fn::character_set_stun_state, Fn::character_set_knock_back_state,
                   Fn::character_get_char_ai_flags, Fn::character_target_in_melee_range,
                   Fn::character_create_buff, Fn::character_remove_buff, Fn::character_apply_prop_class,
                   Fn::character_get_prop})
        check(host::handles(f), "host must own this native");
    for (auto f : {Fn::game_object_get_target_list_top, Fn::character_skill_combat_roll,
                   Fn::character_use_mana, Fn::character_look_at, Fn::character_clear_target,
                   Fn::character_set_prop, Fn::character_start_timer})
        check(!host::handles(f), "an existing owner must not be taken over");

    Fake fake;
    const auto s = fake.services();

    // --- PlayFX (GameObject::_PlayFX): <=3 args attach, >=4 positional.
    {
        auto r = run(s, Fn::game_object_play_fx, SELF, {num(7)});
        check(r.status == 0 && r.returned == 0, "PlayFX(id) succeeds without returns");
        check(fake.fx.size() == 1 && fake.fx[0].effect == 7 && fake.fx[0].attach == SELF && !fake.fx[0].has_position,
              "PlayFX(id) attaches to the receiver");
        run(s, Fn::game_object_play_fx, ENEMY, {num(9)});
        check(fake.fx.back().attach == ENEMY && fake.fx.back().effect == 9, "target:PlayFX attaches to the target");
        run(s, Fn::game_object_play_fx, SELF, {num(5), num(1.f)});
        check(fake.fx.back().attach == SELF && !fake.fx.back().has_position, "two arguments still attach");
        run(s, Fn::game_object_play_fx, SELF, {num(5), num(1.f), num(2.f)});
        check(fake.fx.back().attach == SELF && !fake.fx.back().has_position, "three arguments still attach");
        run(s, Fn::game_object_play_fx, SELF, {num(6), num(11.f), num(12.f), num(13.f)});
        check(fake.fx.back().attach == 0 && fake.fx.back().has_position && fake.fx.back().position[0] == 11.f &&
              fake.fx.back().position[1] == 12.f && fake.fx.back().position[2] == 13.f,
              "PlayFX(id,x,y,z) plays at the position");
        run(s, Fn::game_object_play_fx, SELF, {num(6), str("x"), num(12.f), num(13.f)});
        check(fake.fx.back().attach == 0 && fake.fx.back().has_position && fake.fx.back().position[0] == 10.f &&
              fake.fx.back().position[2] == 30.f, "non-numeric coordinates fall back to the receiver position");
        const auto count = fake.fx.size();
        check(run(s, Fn::game_object_play_fx, SELF).status == 0 && fake.fx.size() == count, "PlayFX() is a no-op");
        check(run(s, Fn::game_object_play_fx, SELF, {str("7")}).status == 0 && fake.fx.size() == count,
              "non-number id is a no-op");
        run(s, Fn::game_object_play_fx, SELF, {num(-4)});
        check(fake.fx.back().effect == 0, "a negative id converts like __aeabi_f2uiz (zero)");
        run(s, Fn::game_object_play_fx, SELF, {num(5e9f)});
        check(fake.fx.back().effect == -1, "an unsigned id beyond the row index range names no row");
        fake.fail_next = true;
        auto bad = run(s, Fn::game_object_play_fx, SELF, {num(7)});
        check(bad.status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE && std::strstr(bad.text, "boom"),
              "provider failure is a required-service failure carrying the reason");
        fake.fail_next = false;
    }

    // --- PlaySound3D: name, position defaults.
    {
        auto r = run(s, Fn::game_object_play_sound3_d, SELF, {str("sfx_spell_lightning_small")});
        check(r.status == 0 && r.returned == 0 && fake.sounds.size() == 1 &&
              fake.sounds[0].name == "sfx_spell_lightning_small" && fake.sounds[0].position[0] == 10.f &&
              fake.sounds[0].position[2] == 30.f, "PlaySound3D(name) plays at the receiver position");
        run(s, Fn::game_object_play_sound3_d, SELF, {str("a"), num(1), num(2), num(3)});
        check(fake.sounds.back().position[1] == 2.f, "PlaySound3D(name,x,y,z) plays at the position");
        run(s, Fn::game_object_play_sound3_d, SELF, {str("b"), num(1), num(2)});
        check(fake.sounds.back().position[0] == 10.f, "PlaySound3D with fewer than three coordinates uses the receiver");
        check(run(s, Fn::game_object_play_sound3_d, SELF).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE,
              "PlaySound3D() has no name");
    }

    // --- GetRand: [0,n), [a,b), default 100, wrong tags push nothing.
    {
        dh2_random_state mirror = fake.random;
        auto one = run(s, Fn::game_object_rand, SELF, {num(10)});
        check(one.status == 0 && one.returned == 1 && one.out[0].type == DH2_SCRIPT_NUMBER &&
              one.out[0].number == float(dh2_random_next(&mirror, 10, 0)), "GetRand(n) draws from the process stream");
        auto two = run(s, Fn::game_object_rand, SELF, {num(5), num(9)});
        check(two.returned == 1 && two.out[0].number == float(dh2_random_next(&mirror, 4, 0) + 5), "GetRand(a,b)");
        auto none = run(s, Fn::game_object_rand, SELF);
        check(none.returned == 1 && none.out[0].number == float(dh2_random_next(&mirror, 100, 0)), "GetRand() defaults to 100");
        auto three = run(s, Fn::game_object_rand, SELF, {num(1), num(2), num(3)});
        check(three.returned == 1 && three.out[0].number == float(dh2_random_next(&mirror, 100, 0)), "three arguments default to 100");
        auto wrong = run(s, Fn::game_object_rand, SELF, {str("x")});
        check(wrong.status == 0 && wrong.returned == 0, "a non-number bound pushes nothing");
        auto wrong2 = run(s, Fn::game_object_rand, SELF, {num(1), boolean(true)});
        check(wrong2.status == 0 && wrong2.returned == 0, "a non-number upper bound pushes nothing");
    }

    // --- GetSelf/GetID, GetPosition, GetName, queries, GetState.
    {
        auto self = run(s, Fn::game_object_get_self, SELF);
        check(self.returned == 1 && self.out[0].type == DH2_SCRIPT_IDENTITY && self.out[0].identity == SELF, "GetSelf");
        auto id = run(s, Fn::game_object_get_id, SELF);
        check(id.returned == 1 && id.out[0].identity == SELF, "GetID");
        auto pos = run(s, Fn::game_object_get_position, ENEMY);
        check(pos.returned == 3 && pos.out[0].number == 1.f && pos.out[1].number == 2.f && pos.out[2].number == 3.f, "GetPosition");
        auto name = run(s, Fn::game_object_get_name, ENEMY);
        check(name.returned == 1 && name.out[0].type == DH2_SCRIPT_STRING &&
              std::string(name.out[0].text, name.out[0].text_bytes) == "Skeleton_01", "GetName");
        check(run(s, Fn::game_object_is_dead, ENEMY).out[0].boolean == 1 && run(s, Fn::game_object_is_dead, SELF).out[0].boolean == 0, "IsDead");
        check(run(s, Fn::game_object_is_player, SELF).out[0].boolean == 1 && run(s, Fn::game_object_is_player, ENEMY).out[0].boolean == 0, "IsPlayer");
        check(run(s, Fn::game_object_is_character, ENEMY).out[0].boolean == 1 && run(s, Fn::game_object_is_character, 0x100000077ull).out[0].boolean == 0, "IsCharacter");
        check(run(s, Fn::game_object_is_door, 0x100000077ull).out[0].boolean == 1 && run(s, Fn::game_object_is_decor, 0x100000077ull).out[0].boolean == 0, "IsDoor/IsDecor");
        check(run(s, Fn::character_get_state, ENEMY).out[0].number == 12.f, "GetState");
        check(run(s, Fn::character_get_state, 0x100000077ull).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE, "GetState of a non-Character fails");
        check(run(s, Fn::game_object_get_position, 0x100000999ull).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE, "unknown receiver fails explicitly");
    }

    // --- GetCharAIFlags / TargetInMeleeRange.
    {
        auto flags = run(s, Fn::character_get_char_ai_flags, ENEMY);
        check(flags.returned == 1 && flags.out[0].type == DH2_SCRIPT_NUMBER && flags.out[0].number == float(0x44), "GetCharAIFlags");
        check(run(s, Fn::character_get_char_ai_flags, SELF).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE,
              "a receiver without an AITable row fails explicitly");
        auto range = run(s, Fn::character_target_in_melee_range, ENEMY);
        check(range.returned == 1 && range.out[0].type == DH2_SCRIPT_BOOLEAN && range.out[0].boolean == 1 &&
              fake.melee_queries.size() == 1 && fake.melee_queries[0] == ENEMY, "TargetInMeleeRange asks the receiver's CharAI");
        fake.in_melee = false;
        check(run(s, Fn::character_target_in_melee_range, SELF).out[0].boolean == 0 && fake.melee_queries.back() == SELF, "TargetInMeleeRange false");
    }

    // --- Buffs and GetProp on a non-Player receiver go to that receiver's own owners.
    {
        auto created = run(s, Fn::character_create_buff, ENEMY, {num(9), num(3000), boolean(false), num(1)});
        check(created.status == 0 && created.returned == 1 && created.out[0].identity == 0xb0ffull &&
              fake.buffs.back().command == host::BuffCommand::create && fake.buffs.back().receiver == ENEMY &&
              fake.buffs.back().count == 4, "target:CreateBuff reaches the receiver BuffOwner");
        check(run(s, Fn::character_apply_prop_class, ENEMY, {num(9), identity(0xb0ffull, DH2_SCRIPT_IDENTITY)}).status == 0 &&
              fake.buffs.back().command == host::BuffCommand::apply, "target:ApplyBuff (class + buff Pointer)");
        check(run(s, Fn::character_apply_prop_class, ENEMY, {num(9)}).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE,
              "ApplyPropClass without a buff Pointer is not attached for another Character");
        check(run(s, Fn::character_remove_buff, ENEMY, {num(9), identity(0xb0ffull, DH2_SCRIPT_IDENTITY)}).status == 0 &&
              fake.buffs.back().command == host::BuffCommand::remove, "target:RemoveBuff");
        check(run(s, Fn::character_create_buff, SELF, {num(9)}).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE,
              "the Player's own buffs are never served by this module");
        auto prop = run(s, Fn::character_get_prop, ENEMY, {num(19)});
        check(prop.status == 0 && prop.returned == 1 && prop.out[0].number == 19.f * 256.f, "target:GetProp(id)");
        check(run(s, Fn::character_get_prop, ENEMY, {num(19), boolean(true)}).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE,
              "the shared temporary sheet belongs to the Player");
        check(run(s, Fn::character_get_prop, ENEMY, {num(500)}).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE, "GetProp id range");
    }

    // --- GetPropHP/GetHP: current, max, percent as integers.
    {
        auto hp = run(s, Fn::character_get_prop_hp, SELF);
        check(hp.returned == 3 && hp.out[0].number == 100.f && hp.out[1].number == 200.f && hp.out[2].number == 50.f, "GetPropHP 100/200/50");
        auto dead = run(s, Fn::character_get_prop_hp, ENEMY);
        check(dead.returned == 3 && dead.out[0].number == 0.f && dead.out[1].number == 0.f && dead.out[2].number == 0.f,
              "an unset Max_HP pushes zeros");
    }

    // --- HasBow / CanAttackInMelee from the equipment flags (bow=2, ranged=128).
    {
        fake.weapon = 0;
        check(run(s, Fn::character_has_bow, SELF).out[0].boolean == 0, "no bow");
        check(run(s, Fn::character_can_attack_in_melee, SELF).out[0].boolean == 1, "melee allowed without a ranged weapon");
        fake.weapon = 2 | 128;
        check(run(s, Fn::character_has_bow, SELF).out[0].boolean == 1, "bow");
        check(run(s, Fn::character_can_attack_in_melee, SELF).out[0].boolean == 0, "no melee with a ranged weapon");
        fake.weapon = 4 | 128;
        check(run(s, Fn::character_has_bow, SELF).out[0].boolean == 0 && run(s, Fn::character_can_attack_in_melee, SELF).out[0].boolean == 0,
              "a staff is ranged but not a bow");
        check(run(s, Fn::character_has_bow, ENEMY).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE,
              "only the Player's inventory is attached");
    }

    // --- RegenHP: truncating integer conversion, non-number ignored.
    {
        run(s, Fn::character_regen_hp, SELF, {num(5.9f)});
        run(s, Fn::character_regen_hp, SELF, {num(-1.f)});
        check(fake.regen.size() == 2 && fake.regen[0] == 5 && fake.regen[1] == -1, "RegenHP converts with __aeabi_f2iz");
        run(s, Fn::character_regen_hp, SELF, {str("9")});
        run(s, Fn::character_regen_hp, SELF);
        check(fake.regen.size() == 2, "RegenHP ignores a missing or non-number amount");
    }

    // --- SetIsTargetable / SetTarget argument tags.
    {
        run(s, Fn::character_set_is_targetable, SELF, {boolean(false)});
        run(s, Fn::character_set_is_targetable, ENEMY, {boolean(true)});
        run(s, Fn::character_set_is_targetable, SELF, {num(1)});
        check(fake.targetable.size() == 2 && fake.targetable[0] == std::make_pair(SELF, false) &&
              fake.targetable[1] == std::make_pair(ENEMY, true), "SetIsTargetable only honours a boolean");
        run(s, Fn::character_set_target, SELF, {identity(ENEMY, 7)});
        run(s, Fn::character_set_target, SELF, {identity(ENEMY, DH2_SCRIPT_IDENTITY)});
        run(s, Fn::character_set_target, SELF, {identity(0, DH2_SCRIPT_IDENTITY)});
        run(s, Fn::character_set_target, SELF, {num(3)});
        run(s, Fn::character_set_target, SELF);
        check(fake.targets.size() == 3 && fake.targets[0].second == ENEMY && fake.targets[1].second == ENEMY &&
              fake.targets[2].second == 0, "SetTarget accepts Pointer and UserData (tags 2 and 7)");
    }

    // --- Object commands.
    {
        run(s, Fn::game_object_lock, ENEMY);
        run(s, Fn::game_object_unlock, ENEMY);
        check(fake.commands.size() == 2 && fake.commands[0].command == host::ObjectCommand::lock &&
              fake.commands[1].command == host::ObjectCommand::unlock && fake.commands[0].receiver == ENEMY, "LOCK/UNLOCK");
        fake.commands.clear();
        run(s, Fn::character_kill, ENEMY, {identity(SELF, 7)});
        run(s, Fn::character_kill, ENEMY, {identity(SELF, DH2_SCRIPT_IDENTITY)});
        run(s, Fn::character_kill, ENEMY);
        check(fake.commands.size() == 3 && fake.commands[0].other == SELF && fake.commands[1].other == 0 && fake.commands[2].other == 0,
              "Kill forwards the killer only for a UserData (tag 7) argument");
        fake.commands.clear();
        run(s, Fn::character_set_stun_state, ENEMY, {num(3000)});
        run(s, Fn::character_set_stun_state, ENEMY, {num(1500), boolean(false)});
        run(s, Fn::character_set_stun_state, ENEMY, {num(10), num(0)});
        run(s, Fn::character_set_stun_state, ENEMY);
        run(s, Fn::character_set_stun_state, ENEMY, {boolean(true)});
        check(fake.commands.size() == 3, "SetStunState needs a numeric duration");
        check(fake.commands[0].duration == 3000 && fake.commands[0].flag, "SetStunState flag defaults to true");
        check(fake.commands[1].duration == 1500 && !fake.commands[1].flag, "SetStunState honours a boolean flag");
        check(fake.commands[2].flag, "a non-boolean second argument keeps the default");
        run(s, Fn::character_set_scare_state, ENEMY, {num(2000), boolean(true)});
        check(fake.commands.back().command == host::ObjectCommand::scare && fake.commands.back().duration == 2000, "SetScareState");
        fake.commands.clear();
        run(s, Fn::character_set_knock_back_state, ENEMY);
        run(s, Fn::character_set_knock_back_state, ENEMY, {boolean(false)});
        run(s, Fn::character_set_knock_back_state, ENEMY, {boolean(true)});
        run(s, Fn::character_set_knock_back_state, ENEMY, {num(0)});
        check(fake.commands.size() == 4 && fake.commands[0].flag && !fake.commands[1].flag && fake.commands[2].flag && fake.commands[3].flag,
              "SetKnockBackState is great unless the first argument is the boolean false");
    }

    // --- Missing providers are explicit failures, never silent no-ops.
    {
        host::Services bare{};
        bare.self = SELF;
        check(run(bare, Fn::game_object_play_fx, SELF, {num(1)}).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE, "no FX provider");
        check(run(bare, Fn::game_object_get_position, SELF).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE, "no facts provider");
        check(run(bare, Fn::game_object_rand, SELF).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE, "no RNG");
        check(run(bare, Fn::character_set_knock_back_state, SELF).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE, "no command provider");
        check(run(bare, Fn::game_object_get_self, SELF).status == 0, "GetSelf needs no provider");
        check(run(s, Fn::character_look_at, SELF).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE, "an unowned native is refused");
    }
    std::puts("{\"validation\":\"PASS\",\"groups\":12}");
    return 0;
}
