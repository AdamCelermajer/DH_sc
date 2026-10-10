#pragma once

#include "renderer.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace dh::foundation {

struct ModulePlacement {
    std::string name;
    std::string assetPath;
    std::string authoredNode;
    Mat4 placement{};
    bool visible = true;
    // Retained for campaign evaluation; a renderer must not assume these pass.
    std::string activateCondition;
    std::string conditionDescription;
    std::string templatePath;
    // P17 PROCGEN2: the module's visual document (`mvp`), which declares its Decor and AnimatedDecor children.
    std::string visualPath;
    // Decor entries returned by decode_module_decor(): the declaration's gametype and its world transform.
    std::string gametype;
};

// Extracts authored Module declarations from original XML. Asset paths remain
// authored paths for the asset boundary. Does not expand template/module files
// or evaluate gameplay conditions. Output is unchanged on failure.
// TRS rotation uses the original export's degrees and axis conversion; optional
// matrix attributes contain 16 column-major floats and take precedence over TRS.
bool decode_level_manifest(const std::vector<std::uint8_t>& bytes,
                           std::vector<ModulePlacement>& output, std::string& error);
bool load_level_manifest(const std::filesystem::path& path,
                         std::vector<ModulePlacement>& output, std::string& error);

// P17 PROCGEN2: the `skybox` attribute of the level's LevelConfig declaration (empty when there is none or the XML
// does not parse). Fixed maps and generated documents both carry it as a LevelConfig attribute.
std::string decode_level_skybox(const std::vector<std::uint8_t>& bytes);

// P17 PROCGEN2: the Decor and AnimatedDecor declarations of one Module visual document (`mvp`, e.g. the cave water
// `_anim_water_*` of the swamp witch cave). Each entry's `placement` is the module placement times the declaration's
// own TRS (module-local, as exported), and `authoredNode` is its xrefobject. Declarations with no dae or xrefobject,
// or with visible="0", are skipped. Other gametypes are not returned. Entries are appended on success; output is
// unchanged on failure.
bool decode_module_decor(const std::vector<std::uint8_t>& mvpBytes, const Mat4& modulePlacement,
                         std::vector<ModulePlacement>& output, std::string& error);

} // namespace dh::foundation
