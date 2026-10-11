#pragma once

// Preview 17 DOORS: gated world objects that are not doors.
//
// TriggerObject (lever): source TriggerObject::TestInteractiveCond evaluates unlock_cond and plays "unlock";
// TriggerObject::Interact (only when unlocked, CanActivate) plays the row sound (Play3D row+16), plays
// "activate", runs the authored script (ScriptManager::StartScript) and raises the TriggerOn quest event.
// The quest event is not raised yet (its object id mapping is not decoded; gap). GetInteractionType is the
// row's type field (row+4) and is what the context-button system receives.
//
// AnimatedDecor: source AnimatedDecor::InitPost. MeetCondition false removes the decor; true plays startanim
// (default "idle") looping. A flip therefore starts (creates + plays) or stops (removes) the decor.
// Only decor with an authored activate_cond/deactivate_cond are instantiated here; ungated decor is not drawn
// by this owner (unchanged from before this stream).

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

struct LeverInstanceV1 {
    std::uint64_t stableId = 0;
    std::string name, data, script, unlock_cond;
    Mat4 transform{};
    std::int32_t row = -1, interaction_type = -1, sound = -1, visual_id = -1;
    std::string visual_file;
};

struct LeverUseResultV1 {
    enum class Status : std::uint8_t { accepted, locked, unknown_declaration, no_visual };
    Status status = Status::unknown_declaration;
    std::string script;                 // authored script to start (accepted only)
    std::int32_t sound_ordinal = -1;    // Play3D request for the caller's audio owner
    std::array<float, 3> position{};
};

class LeverRuntimeV1 {
public:
    bool load(const containers::ContainerTablesV1& tables, const std::vector<ActorDefinition>& definitions,
              std::vector<std::string>& notices, std::string& error);
    bool adopt(const AssetCatalog& assets, std::vector<std::string>& notices, std::string& error);
    std::size_t size() const noexcept { return slots_.size(); }
    const LeverInstanceV1& instance(std::size_t i) const { return slots_.at(i).instance; }
    bool find_by_name(const std::string& name, std::size_t& index) const;
    bool find_by_stable_id(std::uint64_t id, std::size_t& index) const;
    bool unlocked(std::size_t i) const { return slots_.at(i).unlocked; }
    void set_unlocked(std::size_t i, bool value);
    // unlock_cond of every lever (source TestInteractiveCond). Unknown condition = locked and noticed.
    bool refresh_conditions(const std::function<bool(const std::string& condition, bool& value)>& condition,
                            std::vector<std::string>& notices, std::string& error);
    LeverUseResultV1 use(std::size_t i);
    bool update(double seconds, std::string& error);
    struct View { const LeverInstanceV1* instance; CharacterVisual* visual; };
    std::vector<View> views() const;

private:
    struct Slot {
        LeverInstanceV1 instance;
        std::unique_ptr<CharacterVisual> visual;
        bool unlocked = false;
    };
    std::vector<Slot> slots_;
};

struct DecorInstanceV1 {
    std::uint64_t stableId = 0;
    std::string name, dae, startanim, activate_cond, deactivate_cond;
    Mat4 transform{};
};

class DecorRuntimeV1 {
public:
    // Gated AnimatedDecor declarations only (activate_cond/deactivate_cond). Presence is applied by
    // refresh_conditions; load() only records the declarations.
    bool load(const std::vector<ActorDefinition>& definitions, std::vector<std::string>& notices, std::string& error);
    bool refresh_conditions(const std::function<bool(const std::string& condition, bool& value)>& condition,
                            std::vector<std::string>& notices, std::string& error);
    // Keeps the asset catalog for later creation (presence flips after load) and creates the visuals of the
    // decor present at load. Missing assets are noticed; the decor stays present without a visual.
    bool adopt(const AssetCatalog& assets, std::vector<std::string>& notices, std::string& error);
    std::size_t size() const noexcept { return slots_.size(); }
    const DecorInstanceV1& instance(std::size_t i) const { return slots_.at(i).instance; }
    bool find_by_stable_id(std::uint64_t id, std::size_t& index) const;
    bool present(std::size_t i) const { return slots_.at(i).present; }
    // Presence flip: true creates the visual and plays startanim (looping); false removes it.
    bool set_present(std::size_t i, bool value, std::string& error);
    bool update(double seconds, std::string& error);
    struct View { const DecorInstanceV1* instance; CharacterVisual* visual; };
    std::vector<View> views() const;

private:
    struct Slot {
        DecorInstanceV1 instance;
        std::unique_ptr<CharacterVisual> visual;
        bool present = false;
    };
    bool create(Slot& slot, std::string& error);
    const AssetCatalog* assets_ = nullptr;
    std::vector<Slot> slots_;
};

} // namespace dh::foundation::doors
