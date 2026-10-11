#pragma once

// B076: ambient AnimatedDecor of a level (Swamp: tiki torches with fire/glow quads, water caustics, cave-entrance glow).
//
// Source facts (original data, not guesses):
//  * Every Module's .mvp declares AnimatedDecor objects: `dae` (data/3D/AnimatedDecors/*.bdae) + `xrefobject` (the root
//    node of that library scene, id "<xrefobject>-node"). The declaration's position/rotation REPLACES the root TRS and
//    the child locals are kept, exactly like a Module declaration (decode_original_scene_module).
//  * Their clips animate MATERIALS, not nodes: channel type87 = texture-matrix offsetU,88 = offsetV (flip-book of the
//    fire/caustic atlas, targets "torch_fire"/"watercaustic" = material ids), 86 = diffuse colour RGBA 0..255 (glow
//    pulse "wiimaterial_shader*"). The clip loops from segment start to segment end.
//  * `Decor` declarations (Props/Swamp/prop_swamp_corpses.bdae: corpses, corpse piles, bloodstain decals) use the same
//    form and are static.
//  * Declarations with `startanim="idle"`; cutscene-only models (file name `cs_`) stay script-driven (SceneDecorRuntimeV1).
//
// Not covered (documented gaps): torch bone scale breathing (node scale 1.00..1.03), GL_PCloud particle nodes (dust motes).
#include "../../actor_definitions.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_scene.hpp"
#include "../../original_material_texture_binding.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::scene_decor {

// One animated channel set of one material id.
struct AmbientMaterialClip {
    std::string materialId;
    // Indices into AmbientDecorModel::tracks, -1 when the clip has no such channel.
    int offsetU = -1, offsetV = -1, color = -1;
};

struct AmbientTrack {
    bool linear = false;
    std::vector<std::int32_t> times;           // milliseconds, ascending
    std::vector<std::array<float, 4>> values;  // 1 or 4 components used
};

// Per library file: parsed material clips (shared by every instance of that file).
struct AmbientDecorModel {
    std::string path;
    std::int32_t startMs = 0, endMs = 0;
    std::vector<AmbientTrack> tracks;
    std::vector<AmbientMaterialClip> clips;
};

// Pure sampler (testable): value of `track` at `ms` (hold before first / after last key; linear when track.linear).
std::array<float, 4> sample_ambient_track(const AmbientTrack& track, double ms);

// Parses the animation library of a BDAE into material clips (types 86/87/88 only). Other channel types are ignored.
bool parse_ambient_model(const std::vector<std::uint8_t>& bytes, AmbientDecorModel& out, std::string& error);

class AmbientDecorRuntimeV1 {
public:
    struct Instance {
        std::string name, model;
        std::shared_ptr<AmbientDecorModel> clip;
        std::unique_ptr<OriginalScene> scene;
        std::vector<int> rangeClip; // per draw range: index into clip->clips or -1
        double clockMs = 0;
    };
    // Loads every AnimatedDecor declaration (except cutscene `cs_` models and names in `skipNames`) whose library scene
    // decodes. Unavailable ones are reported in `notices` and skipped.
    bool load(const AssetCatalog& assets, const std::vector<ActorDefinition>& definitions,
              const std::vector<std::string>& skipNames, std::vector<std::string>& notices, std::string& error);
    // Binds diffuse/alpha and authored specular textures through the same
    // transport as baked level geometry. Lookup owns resource failure policy.
    void bind_textures(const OriginalTextureLookup& lookup);
    void update(double seconds);
    std::vector<Instance>& instances() noexcept { return instances_; }
    const std::vector<Instance>& instances() const noexcept { return instances_; }

private:
    std::vector<Instance> instances_;
};

} // namespace dh::foundation::scene_decor
