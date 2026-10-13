// Host test for native_player_skill_host_v1: argument checks, defaults and
// return arity of the GameObject/Character natives the Player skill VM serves
// (PlayFX 0x3923cc, PlaySound3D 0x39212c, GetRand 0x393310, GetPropHP 0x3b6cd8,
// HasBow 0x3b6f84, CanAttackInMelee 0x3b6f64, RegenHP 0x3b77ec,
// SetIsTargetable 0x3b7a64, SetTarget 0x3b8f38, Kill 0x3b7e40,
// SetStunState 0x3b9580, SetScareState 0x3b96b0, SetKnockBackState 0x3b7680,
// GetState, Is*, GetPosition, GetName, LOCK/UNLOCK) and the movement/summon
// group (HasPath 0x38e98c, MoveTo 0x3bada8, HeadTo 0x3ba71c, WarpTo 0x3bb268,
// Stop 0x3b56b4, SetMaster 0x3b9084, SetLevel 0x3b73a4, Summon 0x39193c,
// RegisterAnim 0x3b70d8, PlayAnim 0x3ba388, SetProp 0x3b7eac and
// ApplyPropClass 0x3babdc on a non-Player receiver).
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
dh2_script_value nilv() {
    return dh2_script_value{};
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
    struct Control { host::ControlCommand command; std::uintptr_t receiver, object; float point[3]; };
    std::vector<Control> controls;
    struct PropSet { std::uintptr_t receiver; std::uint32_t id; std::int32_t value; };
    std::vector<PropSet> prop_sets;
    struct ClassApply { std::uintptr_t receiver; std::int32_t class_id; bool to_temp; };
    std::vector<ClassApply> class_applies;
    std::vector<std::pair<std::uintptr_t, std::uintptr_t>> masters;
    std::vector<std::pair<std::uintptr_t, float>> levels;
    struct Summoned { std::uintptr_t summoner; host::SummonRequest request; };
    std::vector<Summoned> summons;
    std::uintptr_t summon_result = 0x100000200ull;
    std::vector<std::pair<std::uintptr_t, std::int32_t>> registered_anims;
    struct Play { std::uintptr_t receiver; host::PlayAnimRequest request; };
    std::vector<Play> plays;
    bool path = true;
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
            out->rotation[0] = 0.f; out->rotation[1] = 0.f; out->rotation[2] = 0.5f;
            std::strcpy(out->name, "PlayerCharacter_0");
            return 0;
        }
        if (id == ENEMY) {
            out->is_character = true;
            out->is_dead = true;
            out->state = 12;
            out->position[0] = 1.f; out->position[1] = 2.f; out->position[2] = 3.f;
            out->rotation[0] = 0.1f; out->rotation[1] = 0.2f; out->rotation[2] = 1.25f;
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
    static int set_property(void* p, std::uintptr_t receiver, std::uint32_t id, std::int32_t value, std::string&) {
        self(p).prop_sets.push_back({receiver, id, value});
        return 0;
    }
    static int apply_class(void* p, std::uintptr_t receiver, std::int32_t class_id, bool to_temp, std::string&) {
        self(p).class_applies.push_back({receiver, class_id, to_temp});
        return 0;
    }
    static std::uint32_t class_table_size(void*) { return 50; }
    static std::uint32_t character_table_size(void*) { return 20; }
    static std::uint32_t anim_dict_size(void*) { return 100; }
    static int control(void* p, host::ControlCommand command, std::uintptr_t receiver, std::uintptr_t object,
                       const float point[3], std::string&) {
        Control row{command, receiver, object, {0, 0, 0}};
        if (point) std::memcpy(row.point, point, sizeof(row.point));
        self(p).controls.push_back(row);
        return 0;
    }
    static int has_path(void* p, std::uintptr_t, bool* value, std::string&) {
        *value = self(p).path;
        return 0;
    }
    static int set_master(void* p, std::uintptr_t receiver, std::uintptr_t master, std::string&) {
        self(p).masters.emplace_back(receiver, master);
        return 0;
    }
    static int set_level(void* p, std::uintptr_t receiver, float raw, std::string&) {
        self(p).levels.emplace_back(receiver, raw);
        return 0;
    }
    static int summon(void* p, std::uintptr_t summoner, const host::SummonRequest& request, std::uintptr_t* created,
                      std::string&) {
        auto& f = self(p);
        f.summons.push_back({summoner, request});
        *created = f.summon_result;
        return 0;
    }
    static int register_anim(void* p, std::uintptr_t receiver, std::int32_t dict, std::string&) {
        self(p).registered_anims.emplace_back(receiver, dict);
        return 0;
    }
    static int play_anim(void* p, std::uintptr_t receiver, const host::PlayAnimRequest& request, std::string&) {
        self(p).plays.push_back({receiver, request});
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
        s.set_property = set_property;
        s.apply_property_class = apply_class;
        s.class_table_size = class_table_size;
        s.character_table_size = character_table_size;
        s.anim_dict_size = anim_dict_size;
        s.control = control;
        s.has_path = has_path;
        s.set_master = set_master;
        s.set_level = set_level;
        s.summon = summon;
        s.register_anim = register_anim;
        s.play_anim = play_anim;
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
                   Fn::character_get_prop, Fn::character_set_prop, Fn::game_object_has_path,
                   Fn::game_object_summon, Fn::character_move_to, Fn::character_head_to,
                   Fn::character_warp_to, Fn::character_stop, Fn::character_set_master,
                   Fn::character_set_level, Fn::character_register_anim, Fn::character_play_anim})
        check(host::handles(f), "host must own this native");
    for (auto f : {Fn::game_object_get_target_list_top, Fn::character_skill_combat_roll,
                   Fn::character_use_mana, Fn::character_look_at, Fn::character_clear_target,
                   Fn::character_start_timer})
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
        fake.class_applies.clear();
        check(run(s, Fn::character_apply_prop_class, ENEMY, {num(9)}).status == 0 &&
              fake.class_applies.size() == 1 && fake.class_applies[0].receiver == ENEMY &&
              fake.class_applies[0].class_id == 9 && !fake.class_applies[0].to_temp,
              "target:ApplyPropClass(class) applies to the receiver's resolved sheet");
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

    // --- SetProp / ApplyPropClass on a non-Player receiver (PROPS_Set / PROPS_ApplyClass).
    {
        fake.prop_sets.clear();
        check(run(s, Fn::character_set_prop, ENEMY, {num(19), num(3.9f * 256.f)}).status == 0 &&
              fake.prop_sets.size() == 1 && fake.prop_sets[0].receiver == ENEMY && fake.prop_sets[0].id == 19 &&
              fake.prop_sets[0].value == static_cast<std::int32_t>(3.9f * 256.f),
              "target:SetProp(id, value) truncates the value like __aeabi_f2iz");
        check(run(s, Fn::character_set_prop, ENEMY, {num(19), num(1), boolean(true)}).status == 0 &&
              fake.prop_sets.size() == 2, "a boolean third argument still takes PROPS_Set");
        run(s, Fn::character_set_prop, ENEMY, {num(224), num(1)});
        run(s, Fn::character_set_prop, ENEMY, {str("19"), num(1)});
        run(s, Fn::character_set_prop, ENEMY, {num(19), str("1")});
        run(s, Fn::character_set_prop, ENEMY, {num(19)});
        check(fake.prop_sets.size() == 2, "SetProp ignores an out-of-range id or non-number arguments");
        check(run(s, Fn::character_set_prop, ENEMY, {num(19), num(1), identity(0, DH2_SCRIPT_IDENTITY)}).status == 0 &&
              fake.prop_sets.size() == 2, "a null sheet Pointer does nothing");
        check(run(s, Fn::character_set_prop, ENEMY, {num(19), num(1), identity(0xb0ffull, DH2_SCRIPT_IDENTITY)}).status ==
              DH2_SCRIPT_REQUIRED_SERVICE_FAILURE, "PROPS_SetToSheet on another Character is not attached");
        check(run(s, Fn::character_set_prop, SELF, {num(19), num(1)}).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE,
              "the Player's properties belong to its property owner");

        fake.class_applies.clear();
        run(s, Fn::character_apply_prop_class, ENEMY, {num(9), boolean(true)});
        run(s, Fn::character_apply_prop_class, ENEMY, {num(9), boolean(false)});
        run(s, Fn::character_apply_prop_class, ENEMY, {num(9), num(1)});
        run(s, Fn::character_apply_prop_class, ENEMY, {num(50)});
        run(s, Fn::character_apply_prop_class, ENEMY, {num(51)});
        run(s, Fn::character_apply_prop_class, ENEMY, {str("9")});
        run(s, Fn::character_apply_prop_class, ENEMY);
        check(fake.class_applies.size() == 4 && fake.class_applies[0].to_temp && !fake.class_applies[1].to_temp &&
              !fake.class_applies[2].to_temp && fake.class_applies[3].class_id == 50,
              "ApplyPropClass: boolean selects s_temp, class id <= ClassTable::size, non-number ignored");
    }

    // --- HasPath / Stop.
    {
        fake.path = true;
        auto has = run(s, Fn::game_object_has_path, SELF);
        check(has.returned == 1 && has.out[0].type == DH2_SCRIPT_BOOLEAN && has.out[0].boolean == 1, "HasPath pushes true");
        fake.path = false;
        check(run(s, Fn::game_object_has_path, ENEMY).out[0].boolean == 0, "HasPath pushes false");
        fake.controls.clear();
        check(run(s, Fn::character_stop, SELF).status == 0 && fake.controls.size() == 1 &&
              fake.controls[0].command == host::ControlCommand::stop && fake.controls[0].receiver == SELF,
              "Stop is Cmd_Stop on the receiver");
        run(s, Fn::character_stop, ENEMY, {num(1)});
        check(fake.controls.size() == 2 && fake.controls[1].receiver == ENEMY, "target:Stop ignores arguments");
    }

    // --- MoveTo / HeadTo / WarpTo argument shapes.
    {
        fake.controls.clear();
        run(s, Fn::character_move_to, SELF);
        run(s, Fn::character_move_to, SELF, {num(1), num(2)});
        run(s, Fn::character_move_to, SELF, {num(1)});
        run(s, Fn::character_move_to, SELF, {str("x"), str("y"), str("z")});
        run(s, Fn::character_move_to, SELF, {boolean(true), nilv(), identity(ENEMY, 7)});
        check(fake.controls.empty(), "MoveTo needs one object or three values with a number among the first three");

        run(s, Fn::character_move_to, SELF, {identity(ENEMY, 7)});
        run(s, Fn::character_move_to, SELF, {identity(ENEMY, DH2_SCRIPT_IDENTITY)});
        run(s, Fn::character_head_to, SELF, {identity(ENEMY, 7)});
        check(fake.controls.size() == 3 && fake.controls[0].command == host::ControlCommand::move_to_object &&
              fake.controls[0].object == ENEMY && fake.controls[1].command == host::ControlCommand::move_to_object &&
              fake.controls[2].command == host::ControlCommand::move_to_object && fake.controls[2].object == ENEMY,
              "MoveTo(obj) and HeadTo(obj) both issue Cmd_MoveTo(GameObject*)");
        fake.controls.clear();
        run(s, Fn::character_warp_to, SELF, {identity(ENEMY, 7)});
        check(fake.controls.size() == 1 && fake.controls[0].command == host::ControlCommand::warp_to_point &&
              fake.controls[0].point[0] == 1.f && fake.controls[0].point[1] == 2.f && fake.controls[0].point[2] == 3.f,
              "WarpTo(obj) passes the object's +0x160 point");
        run(s, Fn::character_warp_to, SELF, {num(5), num(6), num(7), boolean(false)});
        check(fake.controls.back().command == host::ControlCommand::warp_to_point &&
              fake.controls.back().point[0] == 5.f && fake.controls.back().point[1] == 6.f &&
              fake.controls.back().point[2] == 7.f, "WarpTo(x,y,z,false) is absolute");
        run(s, Fn::character_move_to, SELF, {num(5), num(6), num(7)});
        run(s, Fn::character_head_to, ENEMY, {num(5), num(6), num(7), num(1)});
        check(fake.controls[fake.controls.size() - 2].command == host::ControlCommand::move_to_point &&
              fake.controls.back().command == host::ControlCommand::head_to_point &&
              fake.controls.back().receiver == ENEMY && fake.controls.back().point[2] == 7.f,
              "MoveTo/HeadTo coordinate overloads; a non-boolean fourth argument stays absolute");
        // First argument not a number, but the second is: the coordinates read the
        // non-number as getNumber would (a string converts through lua_tonumber).
        run(s, Fn::character_move_to, SELF, {str("4"), num(6), nilv()});
        check(fake.controls.back().point[0] == 4.f && fake.controls.back().point[1] == 6.f &&
              fake.controls.back().point[2] == 0.f, "Value::getNumber of string and nil");

        // Relative form: position + a0 * (K x L) + a1 * L + a2 * K, L = (sin h, -cos h, 0).
        run(s, Fn::character_move_to, SELF, {num(0), num(650), num(0), boolean(true)});
        const float h = 0.5f;
        check(std::fabs(fake.controls.back().point[0] - (10.f + 650.f * std::sin(h))) < 0.01f &&
              std::fabs(fake.controls.back().point[1] - (20.f - 650.f * std::cos(h))) < 0.01f &&
              fake.controls.back().point[2] == 30.f, "MoveTo(0, 650, 0, true) walks 650 forward");
        run(s, Fn::character_warp_to, SELF, {num(100), num(0), num(50), boolean(true)});
        check(std::fabs(fake.controls.back().point[0] - (10.f + 100.f * std::cos(h))) < 0.01f &&
              std::fabs(fake.controls.back().point[1] - (20.f + 100.f * std::sin(h))) < 0.01f &&
              fake.controls.back().point[2] == 80.f, "relative a0 is the lateral axis (K x L) and a2 is up");
        check(run(s, Fn::character_move_to, SELF, {num(1), num(2), num(3), boolean(true)}).status == 0, "relative form succeeds");
        host::Services no_facts = fake.services();
        no_facts.object_facts = nullptr;
        check(run(no_facts, Fn::character_move_to, SELF, {num(1), num(2), num(3), boolean(true)}).status ==
              DH2_SCRIPT_REQUIRED_SERVICE_FAILURE, "the relative form needs the receiver's facts");
    }

    // --- SetMaster / SetLevel.
    {
        run(s, Fn::character_set_master, ENEMY, {identity(SELF, 7)});
        run(s, Fn::character_set_master, ENEMY, {identity(SELF, DH2_SCRIPT_IDENTITY)});
        run(s, Fn::character_set_master, ENEMY, {num(3)});
        run(s, Fn::character_set_master, ENEMY);
        check(fake.masters.size() == 2 && fake.masters[0] == std::make_pair(ENEMY, SELF) &&
              fake.masters[1] == std::make_pair(ENEMY, SELF), "SetMaster takes a Pointer or UserData");
        run(s, Fn::character_set_level, ENEMY, {num(5.f * 256.f)});
        run(s, Fn::character_set_level, ENEMY, {str("5")});
        run(s, Fn::character_set_level, ENEMY);
        check(fake.levels.size() == 1 && fake.levels[0].first == ENEMY && fake.levels[0].second == 1280.f,
              "SetLevel needs a numeric first argument and forwards the raw word");
    }

    // --- Summon (GameObject::_Summon argument decoding).
    {
        fake.summons.clear();
        const auto oid = num(7);
        check(run(s, Fn::game_object_summon, SELF).returned == 0 && run(s, Fn::game_object_summon, SELF, {oid}).returned == 0,
              "Summon needs two arguments");
        run(s, Fn::game_object_summon, SELF, {oid, num(1)});
        run(s, Fn::game_object_summon, SELF, {str("7"), boolean(false)});
        run(s, Fn::game_object_summon, SELF, {num(20), boolean(false), num(1), num(2), num(3)});
        check(fake.summons.empty(), "Summon ignores a non-boolean spawn flag, a non-number id and an id >= CharacterTable::size");

        auto absolute = run(s, Fn::game_object_summon, SELF, {oid, boolean(false), num(1), num(2), num(3)});
        check(absolute.status == 0 && absolute.returned == 1 && absolute.out[0].type == DH2_SCRIPT_IDENTITY &&
              absolute.out[0].identity == 0x100000200ull, "Summon pushes the new Character");
        const auto& a = fake.summons.back();
        check(a.summoner == SELF && a.request.character_table_oid == 7 && !a.request.request_spawn_state &&
              a.request.position[0] == 1.f && a.request.position[1] == 2.f && a.request.position[2] == 3.f &&
              a.request.rotation[2] == 0.5f && a.request.forward_anim_ms == 0, "Summon(oid, false, x, y, z) is absolute at the summoner's rotation");
        run(s, Fn::game_object_summon, SELF, {oid, boolean(true), num(1), num(2), num(3), boolean(false)});
        check(fake.summons.back().request.request_spawn_state && fake.summons.back().request.position[0] == 1.f,
              "a false sixth argument stays absolute");
        run(s, Fn::game_object_summon, SELF, {oid, boolean(false), num(0), num(650), num(0), boolean(true)});
        check(std::fabs(fake.summons.back().request.position[0] - (10.f + 650.f * std::sin(0.5f))) < 0.01f &&
              std::fabs(fake.summons.back().request.position[1] - (20.f - 650.f * std::cos(0.5f))) < 0.01f,
              "Summon(oid, b, 0, 650, 0, true) is 650 in front of the summoner");
        run(s, Fn::game_object_summon, SELF, {oid, boolean(false), identity(ENEMY, 7)});
        check(fake.summons.back().request.position[0] == 1.f && fake.summons.back().request.position[2] == 3.f &&
              fake.summons.back().request.rotation[2] == 1.25f, "Summon(oid, b, UserData) takes the object's position and rotation");
        run(s, Fn::game_object_summon, SELF, {oid, boolean(false)});
        check(fake.summons.back().request.position[0] == 10.f && fake.summons.back().request.position[2] == 30.f &&
              fake.summons.back().request.forward_anim_ms == 0, "Summon(oid, b) uses the summoner position");
        run(s, Fn::game_object_summon, SELF, {oid, boolean(false), num(300)});
        check(fake.summons.back().request.position[1] == 20.f && fake.summons.back().request.forward_anim_ms == 300,
              "Summon(oid, b, n) forwards the spawn animation n ms");
        run(s, Fn::game_object_summon, SELF, {oid, boolean(false), num(1), num(2)});
        run(s, Fn::game_object_summon, SELF, {oid, boolean(false), num(1), num(2), str("z")});
        check(fake.summons.size() == 8 && fake.summons[6].request.position[0] == 10.f &&
              fake.summons[7].request.position[0] == 10.f && fake.summons[7].request.forward_anim_ms == 0,
              "any other shape falls back to the summoner position");
        // getUInteger of a negative number is 0 (__aeabi_f2uiz): it names row 0.
        run(s, Fn::game_object_summon, SELF, {num(-1), boolean(false), num(1), num(2), num(3)});
        check(fake.summons.size() == 9 && fake.summons[8].request.character_table_oid == 0,
              "a negative CharacterTable id converts to row 0");
        fake.summon_result = 0;
        check(run(s, Fn::game_object_summon, SELF, {oid, boolean(false)}).returned == 0, "a rejected request pushes nothing");
        fake.summon_result = 0x100000200ull;
    }

    // --- RegisterAnim / PlayAnim.
    {
        run(s, Fn::character_register_anim, SELF, {num(12.7f)});
        run(s, Fn::character_register_anim, SELF, {num(-1)});
        run(s, Fn::character_register_anim, SELF, {str("12")});
        run(s, Fn::character_register_anim, SELF);
        check(fake.registered_anims.size() == 2 && fake.registered_anims[0].second == 12 && fake.registered_anims[1].second == -1,
              "RegisterAnim(number) truncates; non-number ignored");
        run(s, Fn::character_play_anim, SELF, {num(12)});
        run(s, Fn::character_play_anim, SELF, {num(100)});
        check(fake.plays.size() == 1 && fake.plays[0].request.anim_dict == 12 && !fake.plays[0].request.has_second &&
              !fake.plays[0].request.flag, "PlayAnim(dict) below AnimDict::size");
        run(s, Fn::character_play_anim, SELF, {num(-3)});
        check(fake.plays.size() == 2 && fake.plays[1].request.anim_dict == 0, "a negative AnimDict id converts to 0");
        fake.plays.pop_back();
        run(s, Fn::character_play_anim, SELF, {num(12), num(14), boolean(true)});
        check(fake.plays.back().request.has_second && fake.plays.back().request.second == 14 && fake.plays.back().request.flag,
              "PlayAnim(dict, second, true)");
        run(s, Fn::character_play_anim, SELF, {num(12), boolean(true), boolean(true)});
        check(!fake.plays.back().request.has_second && fake.plays.back().request.flag,
              "a non-number second argument is ignored and the flag is read at index 2");
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
        check(run(bare, Fn::game_object_has_path, SELF).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE, "no path provider");
        check(run(bare, Fn::character_stop, SELF).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE, "no controller provider");
        check(run(bare, Fn::character_move_to, SELF, {num(1), num(2), num(3)}).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE,
              "no controller provider for MoveTo");
        check(run(bare, Fn::character_set_master, ENEMY, {identity(SELF, 7)}).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE,
              "no AI_SetMaster provider");
        check(run(bare, Fn::character_set_level, ENEMY, {num(1)}).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE, "no SetLevel provider");
        check(run(bare, Fn::character_register_anim, SELF, {num(1)}).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE,
              "no animator provider");
        check(run(bare, Fn::game_object_summon, SELF, {num(1), boolean(false)}).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE,
              "no CharacterTable provider");
        check(run(s, Fn::character_look_at, SELF).status == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE, "an unowned native is refused");
    }
    std::puts("{\"validation\":\"PASS\",\"groups\":18}");
    return 0;
}
