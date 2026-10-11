#include "level_light_v1.hpp"
#include "../../actor_lighting.hpp"
#include "../../content_paths.hpp"
#include "../../../level-loader/xml_document_v1.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <exception>

namespace dh::foundation::level_lighting {

bool read_level_player_light_v1(const AssetCatalog& assets, const std::string& levelUri, bool fixedFunctionSet,
                                LevelPlayerLightV1& out, std::string& error) {
    out = {};
    try {
        dh2::loader::XmlDocumentV1 document;
        if (!document.capture_level_buffer(levelUri, read_content(assets, levelUri), error)) return false;
        const auto source = document.borrow();
        if (!source.parsed()) { error = source.diagnostic().message; return false; }
        std::string setUri;
        for (const auto& element : source.elements()) {
            const auto* type = element.attribute("gametype");
            if (!type || *type != "LevelConfig") continue;
            if (const auto* name = element.attribute(fixedFunctionSet ? "fixed_light_set" : "light_set")) setUri = *name;
            if (const auto* clear = element.attribute("clear_color")) {
                std::array<float, 3> color{};
                if (std::sscanf(clear->c_str(), "%f , %f , %f", &color[0], &color[1], &color[2]) == 3 &&
                    std::all_of(color.begin(), color.end(), [](float v) { return std::isfinite(v) && v >= 0.0f && v <= 1.0f; })) {
                    out.hasClearColor = true;
                    out.clearColor = color;
                }
            }
            break;
        }
        if (setUri.empty()) { error.clear(); return true; }
        const auto bytes = read_content(assets, setUri);
        std::vector<AuthoredPointLight> lights;
        if (!parseAuthoredPointLights(std::string_view(reinterpret_cast<const char*>(bytes.data()), bytes.size()), lights, error))
            return false;
        for (const auto& authored : lights) {
            if (authored.attachedTo != "Player") continue;
            const auto light = normalizeSourceLight(authored);
            out.present = true;
            out.attachedOffset = authored.attachedOffset;
            out.attenuation = light.attenuation;
            for (int i = 0; i < 3; ++i) { out.ambient[i] = light.ambient[i]; out.diffuse[i] = light.diffuse[i]; out.specular[i] = light.specular[i]; }
            out.lightSetUri = setUri;
            break;
        }
        error.clear();
        return true;
    } catch (const std::exception& failure) {
        error = std::string("Level light read failed: ") + failure.what();
        return false;
    }
}

} // namespace dh::foundation::level_lighting
