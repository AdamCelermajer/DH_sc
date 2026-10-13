#pragma once
// B155: TimerTrap game objects created by a skill script's SummonTimerTrap (the Mud Lord's spit leaves the
// SwampkingSteamCloud trap, the "mist" of the reference video).
//
// Evidence (IDA libDungeonHunter2.so; data tables): SummonTimerTrap 0x390c50 -> TimerTrap::Create 0x39d724 spawns
// "TimerTrap_%04u" at the given point. TimerTraps rows come from game_objects_pyarray.bin (PyDataArrays group 7, shape
// Damage, Delay, HurtChance, Script, Sound, Visual); the model is GameObjectDict[Visual] with the clips idle / activate /
// remove. TimerTrap::SpecificUpdate 0x39d688 calls Activate every Delay ms (the "activate" clip); its authored events reach
// the object script's OnAnimEvent; swampking_trap.luac counts "inccounter" and calls Remove() (the "remove" clip, then the
// object is deleted) once the counter reached COUNTER_DELAY. While active, TriggerTrap::Update 0x39eedc damages the
// occupants of the trap zone through the script's OnDamageRoll -> DealDamages(victim, GameObjectDamager, Elemental).
// The zone: Zone::InitPost 0x39771c looks for the model node "_colzone" (here _colzone_GL_pcloud) and, when SearchByType finds
// exactly one mesh node under it, builds a CTriangleSelector from its mesh; Zone::IsInside 0x3970c8 then tests the vertical
// +-100 segment through the victim against that selector. B155 decode (IDA + the BRES): the node is a glitch particle-cloud
// emitter (SEmitter: EmitterType 0 = box 5 x 350 x 350, particle type 3 = billboards, 30/s) and createParticleSystem
// 0x634c64 only attaches a particle mesh for types 1/2/7 and geometry references, so (inference, not run in the original) this emitter has no mesh: the selector is
// empty and IsInside never reports a victim. Video 19:14-19:28 (56 frames at 4 fps, red HP bar pixel extent) shows the hero's
// HP bar unchanged while he stands in the mist for all 6 activations, which a 5..10 per activation Level01Damage would have
// visibly reduced. This owner therefore keeps the mist visual-only on purpose; the damage rule above is recorded for traps whose
// _colzone does have a mesh (not decoded, so not modelled here).
#include "../../asset_catalog.hpp"
#include "../../../game-data/data.hpp"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace dh::foundation::bosses {

struct TimerTrapRowV1 { std::string name, script; std::int32_t damage = -1, delay_ms = 0, hurt_chance = 0, sound = -1, visual = -1; };
struct GameObjectDamagerRowV1 { std::string name; std::int32_t damage = -1, immunity_delay = 0, max = 0, min = 0, type = -1; };
// Decodes the TimerTraps and GameObjectDamager groups of the grouped game_objects stream (all 11 groups are walked in the
// PyDataArrays order with the shapes of game_object_arrays_owner_v81).
bool load_timer_trap_tables_v1(dh2::data::Bytes records, dh2::data::Bytes names, std::vector<TimerTrapRowV1>& traps,
                               std::vector<GameObjectDamagerRowV1>& damagers, std::string& error);

// Facts of the trap object script (data/scripts/objects/<Script>.luac).
struct TimerTrapScriptV1 {
    std::string count_event;  // OnAnimEvent: if (event == "<name>")
    std::int32_t counter_limit = 0; // COUNTER_DELAY: Remove() once g_counter >= limit, else g_counter + 1
    std::string damage_damager, damage_element; // OnDamageRoll: DealDamages(victim, GameObjectDamager <name>, Elemental <name>)
};
bool parse_timer_trap_script_v1(const std::string& text, TimerTrapScriptV1& output, std::string& error);

struct TimerTrapModelV1 {
    std::string uri;
    std::int32_t idle_start = 0, idle_end = 0, activate_start = 0, activate_end = 0, remove_start = 0, remove_end = 0;
    std::vector<std::pair<std::string, std::int32_t>> events; // authored marker name, authored ms (BRES timeline)
};
bool load_timer_trap_model_v1(const AssetCatalog&, const std::string& uri, TimerTrapModelV1& output, std::string& error);

struct TimerTrapDefinitionV1 { TimerTrapRowV1 row; TimerTrapScriptV1 script; TimerTrapModelV1 model; };
// B155 review: the counter only advances on the count event inside the activate span (RuntimeTimerTrapsV1::update). A definition
// whose model does not author that event would never reach COUNTER_DELAY and never be removed, so it is refused before spawn.
bool validate_timer_trap_definition_v1(const TimerTrapDefinitionV1& def, std::string& error);

class RuntimeTimerTrapsV1 {
public:
    struct View { std::uint64_t id = 0; std::string model_uri; std::int32_t source_ms = 0, age_ms = 0; std::array<float, 3> position{}; };
    std::uint64_t spawn(const TimerTrapDefinitionV1&, const std::array<float, 3>& at);
    // Advances every trap: idle loop, Activate every Delay, authored events (the count event), Remove and deletion.
    void update(std::int32_t dt_ms);
    std::vector<View> views() const;
    std::size_t size() const noexcept { return traps_.size(); }
    void clear() { traps_.clear(); }
    std::vector<std::string> take_log();
private:
    enum class Phase : std::uint8_t { idle, activate, remove };
    struct Trap {
        std::uint64_t id = 0; TimerTrapDefinitionV1 def; std::array<float, 3> at{};
        Phase phase = Phase::idle; std::int32_t clip_ms = 0, since_activate_ms = 0, age_ms = 0, counter = 0, activations = 0;
        bool removing = false;
    };
    std::vector<Trap> traps_;
    std::uint64_t next_ = 1;
    std::vector<std::string> log_;
};

} // namespace dh::foundation::bosses
