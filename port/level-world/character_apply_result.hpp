#pragma once
#include "../game-data/combat_result.hpp"
#include <cstdint>

namespace dh2::character_apply_result {
// Live scalar projections, not original ARM object overlays. The identities
// are stable through the call; service callbacks may change the other fields.
struct Actor {
    std::uintptr_t identity;
    std::uint16_t combo_14d0;
    std::uint8_t invulnerable_14f0, push_death_53b;
    // ObjectBase::IsRemotelyUpdated consumes this word; it is not a PyData OID.
    std::int32_t remote_update_word_110;
};
struct Arguments { Actor* attacker; Actor* defender; std::uint32_t mode; };
struct Globals { std::uintptr_t debug_switches,application,visual_fx; };
enum class Operation : std::uint32_t {
    online_mode,is_player,debug_load,string_construct,debug_query,string_destroy,
    saved_option,coop_player_count,effective_threat,add_aggro,hit_for,
    is_dead,blood_death_fx,blood_fx,target_position,play_anim_fx_set,
    regen_hp,regen_mp,apply_dot,dodge,block,hurt,push,read_property,stun,fear,
    slow,cancel_sneaking,combat_text,combat_sound,ai_combat_result,
    critical_camera,player_by_character,player_stat_increment,player_stat_increase,
    state_machine_is_idle,player_property_add_int,player_property_get_int,
    player_is_local,trophy_lookup,trophy_unlock,inventory_add_gold,
    is_monster
};
struct Request {
    Operation operation;
    std::uintptr_t subject,peer,identity;
    const char* text;
    data::CombatResult* result;
    std::uintptr_t attacker,defender;
    std::uint32_t word0,word1,word2,word3;
};
struct Reply { std::uintptr_t identity; std::uint32_t word; };
struct Services {
    void* context;
    std::int32_t (*invoke)(void*,const Request*,Reply*);
    // Source attacker ItemInventory::gold is a direct field read, not a callee. Resolve
    // it from that same Character's retained canonical inventory owner only
    // when the gold-damage mask is reached.
    std::int32_t (*read_inventory_gold)(void*,std::uintptr_t attacker_character,
                                       std::int32_t* gold){};
    // DesignSettingsTable::members is a retained global table. Read only source
    // offsets +52/+56 after the corresponding actor classification branch.
    std::int32_t (*read_coop_factor)(void*,std::uint32_t member_offset,
                                     std::uint32_t* binary32){};
};
struct Result {
    std::uint32_t calls,last_operation,debug_phases,hits,regenerations,status_calls,
                  notifications,ai_callbacks,unsupported_pc,captured_hit_amount,threat;
};
enum class Status : std::int32_t {
    complete,invalid_argument,service_unavailable,service_failed,
    unsupported_network,unsupported_player,unsupported_gold,
    unsupported_coop_scaling,unsupported_threat_value,invalid_provider_result
};

// Original F_ApplyResult offline continuation for nonplayer and Player
// defenders, plus the source-ordered Player attacker tail. This is a bounded
// caller, not a new whole-body reconstruction. Network effects remain outside
// this offline traversal; multiplayer damage scaling is supported.
// All other reached services are mandatory, including Debug/string operations,
// HitFor, status, visual FX, cancel-sneak, scrolling text and combat audio. A
// successful no-op effect provider violates this dependency contract. This
// caller does not itself claim those callees or a complete health/death backend.
// effective_threat returns the source getter's binary32 word (finite int32/256);
// add_aggro returns its RAW binary32 word, including NaN and signed zero.
Status execute(const Arguments*,const Globals*,const Services*,data::CombatResult*,Result*);

// Request subject/peer are Character identities for actor operations. Native
// adapters resolve the original embedded properties/CharAI/state-machine, not
// a replacement actor. For FX, subject=live VFX manager, peer=defender whose
// live orientation+16c is used, identity=borrowed target-position pointer,
// word0=FX key, word1/2=the two null stack arguments. read_property word0 is
// the source field ID; stun/fear words are duration/true/final bool; push words
// are push-bit/special; apply_dot words are duration/amount/element. All requests
// retain the mutable result and original attacker/defender identities. Virtual
// AI result providers resolve current CharAI and active AIS at their call phase.
// coop_player_count is the current Application+40 PlayerManager+6c4 field read,
// not a synthesized actor count or a query of a Game object. Its adapter
// must not introduce unrelated side effects. No owner yields a provider error.
//
// One owning thread retains all borrowed controls, actors, result and selected
// service backing through return. Aliases are rejected except attacker==defender;
// one Character identity must use one shared Actor projection, not two copies.
// Service bindings and argument identities are captured once. Actor scalars,
// mutable result and selected global singleton identities remain live; retired
// owners/returned positions/strings must survive every reached callback. Same
// actors/result/control reentry or destruction is forbidden; independent owners
// may nest. A service error/exception stops without rollback, invented cleanup,
// or later effect calls. Missing providers fail only when reached. In particular
// string destruction is not added during error unwinding. Player operations
// use Application as subject for PlayerManager lookup/stat dispatch. The
// lookup request has peer=Character identity and word0=0 (the source's false
// lookup flag); its reply.word is PlayerInfo+0x1648. Stat increment uses
// word0=stat enum and word1=internal player ID. Stat increase uses word0=stat,
// word1=source signed amount>>8 bit pattern, word2=internal player ID.
// critical_camera receives the live result and attacker identity; its adapter
// follows the optional source camera-level/rig path and succeeds when no level
// or camera is present, as the original code does. Player-defender status
// requests use state_machine_is_idle with word0=the source false flag;
// player_property_add_int uses word0=property id, word1=integer delta 1;
// player_property_get_int uses word0=property id, word1=source false flag and
// returns the source whole integer in Reply.word. player_is_local has subject
// Application and peer=Character. trophy_lookup receives its exact source name
// in text and returns its table id; trophy_unlock has subject Application and
// word0=that id. The three counter pairs are properties 215/214/222, thresholds
// >=500/>=500/>49, and trophies epic_invisible/epic_invincible/
// knocked_down_50_times. AddInt uses source integer delta 1; false locality
// skips the trophy lookup/unlock. mode is the source
// bool word and is not consulted in the supported offline branch.
// For mask 0x400000, read_inventory_gold resolves the same attacker's retained
// ItemInventory and returns its signed gold field without a synthetic service
// call. The kernel performs the source signed min(gold<<8, amount), updates
// AttackResult.amount, then inventory_add_gold(subject=attacker, word0=the
// signed AddGold delta) dispatches ItemInventory::AddGold through that same
// canonical V4 owner before threat calculation.
// For a positive amount and PlayerManager count > 1, the source calls
// is_monster(attacker). A monster reads DesignSettings members+52, multiplies
// the local HitFor amount, and writes it back to AttackResult. Otherwise it
// calls is_player(attacker); a player reads members+56 and divides only the
// local HitFor amount. read_coop_factor receives the exact source member
// offset and returns its raw binary32 word without a synthetic call event. The
// player branch leaves AttackResult.amount unchanged, so its later threat
// calculation uses the unscaled amount. The unused factor is not read.
} // namespace dh2::character_apply_result
