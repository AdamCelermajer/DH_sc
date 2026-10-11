#pragma once

#include "container_declarations_v1.hpp"
#include "../../original_character.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::containers {

// Source container states (Container::SetState values read by Container::Interact
// at 0x3a0b38: states 3 and 4 reject the interaction; 2 is the idle closed state).
inline constexpr std::uint8_t kContainerStateIdle = 2;
inline constexpr std::uint8_t kContainerStateActivating = 3;
inline constexpr std::uint8_t kContainerStateOpened = 4;

// Range of the object-of-interest query that feeds the context button
// (CharacterDesign.OOI_Distance = 200, horizontal distance). Container::Interact
// itself has no range check; the caller's query supplies it.
inline constexpr float kContainerInteractRange = 200.0f;

enum class ContainerInteractStatusV1 : std::uint8_t {
    accepted,            // openable: activate started; destructible: broken, activate started
    hit,                 // destructible: one stage hit, stage clip played, not broken yet
    unknown_declaration,
    no_visual,
    out_of_range,
    rejected_state,
};

struct ContainerInteractResultV1 {
    ContainerInteractStatusV1 status = ContainerInteractStatusV1::unknown_declaration;
    std::uint8_t state_before = 0, state_after = 0;
    std::size_t index = 0;
    float distance = 0.0f;
    std::uint32_t hits_remaining = 0;   // destructible only, after this interaction
    std::int32_t sound_id = -1;         // destructible: row sound (played by the caller's sound owner)
};

const char* container_status_name(ContainerInteractStatusV1 status);

enum class ContainerEventKindV1 : std::uint8_t { opened };

struct ContainerEventV1 {
    ContainerEventKindV1 kind = ContainerEventKindV1::opened;
    std::size_t index = 0;
    std::int32_t loot_id = -1;
    std::string script;  // authored OnOpen script name (empty when none)
    double elapsed_ms = 0.0;  // activate clip time at the authored marker
    std::string name;
};

// Owns the loaded container declarations of the current level: one visual and one
// state machine per declaration, so each object animates and persists separately.
class ContainerRuntimeV1 {
public:
    struct View {
        const ContainerInstanceV1* instance = nullptr;
        CharacterVisual* visual = nullptr;
        std::uint8_t state = 0;
    };

    // Binds each instance's embedded-scene visual. Missing visuals keep the
    // declaration instantiated but hidden (notices explain why).
    bool adopt(const AssetCatalog& assets, std::vector<ContainerInstanceV1> instances,
               std::vector<std::string>& notices, std::string& error);

    std::size_t size() const noexcept { return slots_.size(); }
    std::vector<View> views() const;
    bool find_by_name(const std::string& name, std::size_t& index) const;
    const ContainerInstanceV1& instance(std::size_t index) const { return slots_.at(index).instance; }
    std::uint8_t state(std::size_t index) const { return slots_.at(index).state; }
    std::uint32_t hits_remaining(std::size_t index) const { return slots_.at(index).remaining; }
    // Interaction type exposed to the context-button system (0 chest, 8 destructible),
    // or -1 when the declaration is unknown.
    std::int32_t interaction_type(std::size_t index) const;
    // Container::Interact gate order: unknown -> visual -> range -> state 3/4 -> accept.
    // Destructibles: each accepted interaction is one hit (stage clip) until the
    // stage count is used up, then the break starts the activate clip.
    ContainerInteractResultV1 interact(std::size_t index, const float player_position[3]);
    // Advances activating clips; emits 'opened' (DoOpen) once per break/open.
    bool update(double seconds, std::vector<ContainerEventV1>& events, std::string& error);

    // Declarations whose persisted state (state or remaining hits) changed since the last call.
    std::vector<std::size_t> take_changed();
    // Restores a persisted state without rewards or events (GameSave load).
    // state 2 = closed (idle), 4 = opened (idleactive); 3 completes without a reward.
    bool restore(std::size_t index, std::uint8_t state, std::uint32_t hits_remaining,
                 std::string& error);

private:
    struct Slot {
        ContainerInstanceV1 instance;
        std::unique_ptr<CharacterVisual> visual;
        std::uint8_t state = kContainerStateIdle;
        double clip_end_ms = -1.0;     // authored end of the activate clip
        double opened_marker_ms = -1.0;  // authored 'opened' marker of the activate clip
        bool opened_fired = false;
        // Destructible stages: BDAE clip library order (embedded scene), stages = clips - 3.
        std::vector<std::string> clip_names;
        std::uint32_t stages = 0;
        std::uint32_t remaining = 0;
        bool dirty = false;
    };
    std::vector<Slot> slots_;
    std::vector<ContainerEventV1> pending_;  // events raised by interact (break without an activate clip)
};

} // namespace dh::foundation::containers
