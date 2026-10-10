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
    accepted,
    unknown_declaration,
    no_visual,
    out_of_range,
    rejected_state,
    unsupported_family,  // destructible hit/destroy path not yet bound (see report)
};

struct ContainerInteractResultV1 {
    ContainerInteractStatusV1 status = ContainerInteractStatusV1::unknown_declaration;
    std::uint8_t state_before = 0, state_after = 0;
    std::size_t index = 0;
    float distance = 0.0f;
};

const char* container_status_name(ContainerInteractStatusV1 status);

enum class ContainerEventKindV1 : std::uint8_t { opened };

struct ContainerEventV1 {
    ContainerEventKindV1 kind = ContainerEventKindV1::opened;
    std::size_t index = 0;
    std::int32_t loot_id = -1;
    std::string script;  // Lua OnOpen script name (not run yet: spawn/script provider unbound)
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
    // Interaction type exposed to the context-button system (0 chest, 8 destructible),
    // or -1 when the declaration is unknown.
    std::int32_t interaction_type(std::size_t index) const;
    // Container::Interact gate order: unknown -> visual -> range -> state 3/4 -> accept.
    ContainerInteractResultV1 interact(std::size_t index, const float player_position[3]);
    // Advances activating clips; emits 'opened' when the authored marker is reached.
    bool update(double seconds, std::vector<ContainerEventV1>& events, std::string& error);

private:
    struct Slot {
        ContainerInstanceV1 instance;
        std::unique_ptr<CharacterVisual> visual;
        std::uint8_t state = kContainerStateIdle;
        double clip_end_ms = -1.0;     // authored end of the activate clip
        double opened_marker_ms = -1.0;  // authored 'opened' marker of the activate clip
        bool opened_fired = false;
    };
    std::vector<Slot> slots_;
};

} // namespace dh::foundation::containers
