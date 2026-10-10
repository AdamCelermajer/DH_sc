#pragma once

// P16 OPENING4: scripted scene objects. An AnimatedDecor declaration (for example the Swamp prison cage
// `_anim_cage_001`, data/3D/AnimatedDecors/cs_swamp_intro_cage.bdae) is drawn at its authored transform and plays the
// clip that a script names with Script_PlayAnimByName (kind 19: object @24, clip @12; IDA GameObject visual animator).
//
// General: only declarations that a loaded script names are instantiated (the rule is data-driven: names come from the
// scripts and the declarations, nothing is keyed by level). Their clip library is the BDAE clip list, the same loader the
// containers use. The start clip is the declaration's authored `startanim` (default idle).
#include "../../actor_definitions.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_character.hpp"
#include "../../renderer.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::scene_decor {

struct SceneDecorView {
    std::string name;
    const CharacterVisual* visual = nullptr;
    Mat4 transform{};
};

class SceneDecorRuntimeV1 {
public:
    // Instantiates the AnimatedDecor declarations named in `wanted` that have a model. Missing models are noticed and the
    // object stays unplayable (a script PlayAnimByName then reports it as not found).
    bool adopt(const AssetCatalog& assets, const std::vector<ActorDefinition>& definitions,
               const std::vector<std::string>& wanted, std::vector<std::string>& notices, std::string& error);

    // Plays `clip` on the named object. found=false when the object is not instantiated (the source does nothing).
    // Clips named idle* loop; the others play once and then hold their last frame.
    bool play(const std::string& name, const std::string& clip, bool& found, std::string& error);

    bool update(double seconds, std::string& error);
    std::vector<SceneDecorView> views() const;
    std::size_t size() const noexcept { return slots_.size(); }

private:
    struct Slot {
        std::string name, model_path;
        Mat4 transform{};
        std::unique_ptr<CharacterVisual> visual;
    };
    std::vector<Slot> slots_;
};

} // namespace dh::foundation::scene_decor
