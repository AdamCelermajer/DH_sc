#pragma once

// Preview 17 DOORS: authored Door declarations as live world objects.
//
// Source (IDA libDungeonHunter2.so, Door::Open/Close/Opened/Closed/__Callback/__EventCallback):
//   state 0 = closed (IdleClosed loop), 1 = opened (IdleOpened loop), 2 = closing (clip "Close"),
//   3 = opening (clip "Open"). Open needs state 0 and no transition in progress; Close needs state 1.
//   Skip (script skip) or a missing visual applies the end state at once. A transition ends on the
//   clip end (Door::__Callback), or earlier on the authored 'opened'/'closed' marker (__EventCallback,
//   which applies the end state without replaying the idle clip). The sound is Play3D of the Arrays::Doors
//   row (sound10 on open, sound_c on close) at the door position. Serialize writes the 4-byte state;
//   Deserialize maps 1/3 to opened and anything else to closed.
//
// Visual: the game-object row (Arrays::Doors, data name -> row) gives visual14 -> GameObjectDict file.
// Collision: InitPost builds the PhysicalObject only when door_collision is true, from the same visual's
// ApplyMeshBox AABB. Opened/Closed update its filter at source markers or clip end. Physical geometry is
// supplied by the canonical visual/body owner; indexed render bounds are not a substitute.

#include "../../actor_definitions.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_character.hpp"
#include "../containers/container_declarations_v1.hpp"

#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::doors {

inline constexpr const char* door_state_component_v1 = "dh2.world-gate.door.v1";  // 1 byte: DoorStateV1 (P17 DOORS2 name; was dh2.door.state.v1)

enum class DoorStateV1 : std::uint8_t { closed = 0, opened = 1, closing = 2, opening = 3 };

// One Play3D request for the caller's audio owner (ordinal = authored Arrays::Doors sound id).
struct DoorSoundRequestV1 {
    std::int32_t ordinal = -1;
    std::array<float, 3> position{};
};

struct DoorInstanceV1 {
    std::uint64_t stableId = 0;
    std::string name, data;
    Mat4 transform{};
    std::int32_t row = -1;
    std::int32_t visual_id = -1, sound_close = -1, sound_open = -1;
    std::string visual_file;   // GameObjectDict file, package-relative
    std::string activate_cond; // authored gate (empty when none)
    std::string deactivate_cond;
    bool opened_authored = false;
    bool door_collision = true; // source property default at Door construction
};

struct DoorViewV1 {
    const DoorInstanceV1* instance = nullptr;
    CharacterVisual* visual = nullptr;  // null when absent or without a visual
    DoorStateV1 state = DoorStateV1::closed;
    bool present = true;
};

// Value snapshot consumed by the shared physical owner. A body is required only when both
// collision_enabled and present are true. filter_enabled is latched at the source Opened/Closed
// callback, so it retains the prior value during animated state 2/3 transitions. Geometry remains
// owned by the same DoorViewV1 visual and authored transform; use the recovered ApplyMeshBox path.
struct DoorPhysicalBindingV1 {
    std::uint64_t stable_id = 0;
    bool collision_enabled = true;
    bool present = false;
    bool filter_enabled = true;
};

class DoorRuntimeV1 {
public:
    // Builds one slot per authored Door declaration. Rows are read from the SAME game-object borrow the
    // containers use. Declarations whose data name is not in Arrays::Doors are noticed and skipped.
    bool load(const containers::ContainerTablesV1& tables, const std::vector<ActorDefinition>& definitions,
              std::vector<std::string>& notices, std::string& error);

    // Loads the embedded-scene visuals (missing assets keep the door instantiated without a visual).
    bool adopt(const AssetCatalog& assets, std::vector<std::string>& notices, std::string& error);

    // Presence from the authored gates: activate_cond true = present, deactivate_cond true = absent.
    // Source Door::InitPost MeetCondition: a door whose gate is not met is deleted. Unknown condition = absent
    // and noticed. Called at load and after every restore of the session (never replays a state change).
    bool refresh_conditions(const std::function<bool(const std::string& condition, bool& value)>& condition,
                            std::vector<std::string>& notices, std::string& error);
    bool find_by_stable_id(std::uint64_t id, std::size_t& index) const;

    std::size_t size() const noexcept { return slots_.size(); }
    const DoorInstanceV1& instance(std::size_t index) const { return slots_.at(index).instance; }
    bool find_by_name(const std::string& name, std::size_t& index) const;
    DoorStateV1 state(std::size_t index) const { return slots_.at(index).state; }
    bool busy(std::size_t index) const { return slots_.at(index).busy; }
    bool present(std::size_t index) const { return slots_.at(index).present; }
    // Current source filter state (true = collision filter enabled). It stays latched through clips.
    bool collision_closed(std::size_t index) const { return slots_.at(index).filter_enabled; }
    std::vector<DoorViewV1> views();
    // Stable-ID snapshots for idempotent native body/filter reconciliation. Values only; pair them
    // with the same stable ID's DoorViewV1 visual and DoorInstanceV1 transform for source geometry.
    std::vector<DoorPhysicalBindingV1> physical_bindings() const;

    // Script_OpenDoor / Script_CloseDoor (kind 54 / 55). skip = the script skip flag.
    bool open(std::size_t index, bool skip, std::string& error);
    bool close(std::size_t index, bool skip, std::string& error);
    // Script IsBlocking: true while a transition started with wait=1 is still playing.
    bool blocking(std::size_t index) const;

    // Advances the clips of every visual; applies transitions whose clip ended.
    bool update(double seconds, std::string& error);

    // Gate owner: activate_cond/deactivate_cond. present=false removes the door from the world (no draw, no
    // commands); present=true recreates it in its persisted state.
    bool set_present(std::size_t index, bool present, std::string& error);

    // Restores a persisted state with no sound and no replay (GameSave load).
    bool restore(std::size_t index, DoorStateV1 state, std::string& error);

    std::vector<DoorSoundRequestV1> take_sound_requests();
    std::vector<std::size_t> take_changed();

private:
    struct Slot {
        DoorInstanceV1 instance;
        std::unique_ptr<CharacterVisual> visual;
        DoorStateV1 state = DoorStateV1::closed;
        bool collision_enabled = true; // authored door_collision property, source ctor default true
        bool filter_enabled = true;    // source PhysicalObject filter state, changed only at Opened/Closed
        bool busy = false;
        bool present = true;
        bool dirty = false;
        // Per direction (0 = closing 'close' clip, 1 = opening 'open' clip): authored clip end and
        // the 'opened'/'closed' marker in milliseconds, -1 when absent.
        double clip_end_ms[2] = {-1.0, -1.0};
        double marker_ms[2] = {-1.0, -1.0};
        bool marker_fired = false;
    };
    bool transition(std::size_t index, bool opening, bool skip, std::string& error);
    bool show_idle(Slot& slot, std::string& error);
    void request_sound(const Slot& slot, bool opening);
    std::vector<Slot> slots_;
    std::vector<DoorSoundRequestV1> sounds_;
};

} // namespace dh::foundation::doors
