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

} // namespace dh::foundation
