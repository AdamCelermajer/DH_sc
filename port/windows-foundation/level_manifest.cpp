#include "level_manifest.hpp"
#include "../level-loader/vendor/tinyxml/tinyxml.h"
#include "../engine-math/math.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace dh::foundation {
namespace {
constexpr std::size_t max_bytes = 8 * 1024 * 1024;
constexpr std::size_t max_elements = 100000;
constexpr std::size_t max_depth = 128;
constexpr std::size_t max_modules = 4096;

std::string attribute(const TiXmlElement& element, const char* key) {
    const char* value = element.Attribute(key);
    return value ? value : "";
}

// Bound recursion before invoking TinyXML's recursive parser. TinyXML performs
// the full syntax/name matching check after this lexical resource-limit pass.
void preflight(std::string_view xml) {
    std::size_t depth = 0, count = 0;
    for (std::size_t at = 0; at < xml.size();) {
        const auto start = xml.find('<', at);
        if (start == std::string_view::npos) break;
        if (xml.substr(start, 4) == "<!--") {
            const auto end = xml.find("-->", start + 4);
            if (end == std::string_view::npos) throw std::runtime_error("Unterminated XML comment");
            at = end + 3; continue;
        }
        if (xml.substr(start, 9) == "<![CDATA[") {
            const auto end = xml.find("]]>", start + 9);
            if (end == std::string_view::npos) throw std::runtime_error("Unterminated CDATA");
            at = end + 3; continue;
        }
        if (xml.substr(start, 2) == "<?") {
            const auto end = xml.find("?>", start + 2);
            if (end == std::string_view::npos) throw std::runtime_error("Unterminated XML declaration");
            at = end + 2; continue;
        }
        if (xml.substr(start, 2) == "<!") throw std::runtime_error("DTD declarations are unsupported");
        char quote = 0;
        std::size_t end = start + 1;
        for (; end < xml.size(); ++end) {
            const char c = xml[end];
            if (quote) { if (c == quote) quote = 0; }
            else if (c == '\'' || c == '"') quote = c;
            else if (c == '>') break;
        }
        if (end == xml.size()) throw std::runtime_error("Unterminated XML element");
        if (xml[start + 1] == '/') {
            if (depth == 0) throw std::runtime_error("Unexpected XML closing element");
            --depth;
        } else {
            if (++count > max_elements) throw std::runtime_error("XML element limit exceeded");
            if (++depth > max_depth) throw std::runtime_error("XML nesting limit exceeded");
            std::size_t previous = end;
            while (previous > start && (xml[previous - 1] == ' ' || xml[previous - 1] == '\t' ||
                   xml[previous - 1] == '\r' || xml[previous - 1] == '\n')) --previous;
            if (previous > start && xml[previous - 1] == '/') --depth;
        }
        at = end + 1;
    }
}

template<std::size_t N>
std::array<float, N> tuple(std::string value, std::array<float, N> fallback) {
    if (value.empty()) return fallback;
    std::replace(value.begin(), value.end(), ',', ' ');
    std::istringstream stream(value);
    stream.imbue(std::locale::classic());
    std::array<float, N> values{};
    for (float& component : values) {
        if (!(stream >> component) || !std::isfinite(component))
            throw std::runtime_error("Invalid authored transform tuple");
    }
    stream >> std::ws;
    if (!stream.eof()) throw std::runtime_error("Trailing authored transform tuple data");
    return values;
}

Mat4 placement(const TiXmlElement& element) {
    const auto matrix = attribute(element, "matrix");
    if (!matrix.empty()) return tuple<16>(matrix, {});
    const auto position = tuple<3>(attribute(element, "position"), {});
    const auto rotation = tuple<3>(attribute(element, "rotation"), {});
    auto scale = tuple<3>(attribute(element, "scale"), {1, 1, 1});
    for (float& component : scale) if (std::abs(component) < 0.0001f) component = 1;
    constexpr float radians = 0.01745329251994329577f;
    dh2::math::Quaternion q{};
    // This export's Y,-X,-Z conversion is recovered in visual_transform_v1.
    dh2_quat_from_euler(&q, rotation[1] * radians, -rotation[0] * radians, -rotation[2] * radians);
    const float x=q.x, y=q.y, z=q.z, w=q.w;
    Mat4 result{
        (1-2*y*y-2*z*z)*scale[0], (2*x*y+2*z*w)*scale[0], (2*x*z-2*y*w)*scale[0], 0,
        (2*x*y-2*z*w)*scale[1], (1-2*x*x-2*z*z)*scale[1], (2*y*z+2*x*w)*scale[1], 0,
        (2*x*z+2*y*w)*scale[2], (2*y*z-2*x*w)*scale[2], (1-2*x*x-2*y*y)*scale[2], 0,
        position[0], position[1], position[2], 1};
    for (float component : result) if (!std::isfinite(component))
        throw std::runtime_error("Authored transform overflow");
    return result;
}
}

bool decode_level_manifest(const std::vector<std::uint8_t>& bytes,
                           std::vector<ModulePlacement>& output, std::string& error) {
    try {
        if (bytes.empty() || bytes.size() > max_bytes) throw std::runtime_error("Level XML size outside limits");
        if (std::find(bytes.begin(), bytes.end(), 0) != bytes.end()) throw std::runtime_error("Level XML contains a NUL byte");
        const std::string xml(bytes.begin(), bytes.end());
        preflight(xml);
        TiXmlDocument document;
        document.Parse(xml.c_str());
        if (document.Error()) throw std::runtime_error(std::string("Invalid level XML: ") + document.ErrorDesc());
        const TiXmlElement* root = document.RootElement();
        if (!root || root->NextSiblingElement()) throw std::runtime_error("Level XML must have one root element");
        std::vector<ModulePlacement> staged;
        std::function<void(const TiXmlElement&)> visit = [&](const TiXmlElement& element) {
            if (attribute(element, "gametype") == "Module" || std::string(element.Value()) == "Module") {
                if (staged.size() == max_modules) throw std::runtime_error("Level module limit exceeded");
                ModulePlacement module;
                module.name = attribute(element, "name");
                module.assetPath = attribute(element, "dae");
                module.authoredNode = attribute(element, "xrefobject");
                if (module.assetPath.empty() || module.authoredNode.empty())
                    throw std::runtime_error("Module requires dae and xrefobject: " + module.name);
                module.authoredNode += "-node";
                module.placement = placement(element);
                const auto visible = attribute(element, "visible");
                if (!visible.empty() && visible != "0" && visible != "1")
                    throw std::runtime_error("Module visibility must be 0 or 1: " + module.name);
                module.visible = visible != "0";
                module.activateCondition = attribute(element, "activate_cond");
                module.conditionDescription = attribute(element, "condition_desc");
                module.templatePath = attribute(element, "template");
                if (module.templatePath.empty()) module.templatePath = attribute(element, "templateName");
                staged.push_back(std::move(module));
            }
            for (const TiXmlElement* child = element.FirstChildElement(); child; child = child->NextSiblingElement()) visit(*child);
        };
        visit(*root);
        if (staged.empty()) throw std::runtime_error("Level XML contains no authored modules");
        output.swap(staged);
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

bool load_level_manifest(const std::filesystem::path& path,
                         std::vector<ModulePlacement>& output, std::string& error) {
    try {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file) throw std::runtime_error("Cannot open level XML");
        const auto length = file.tellg();
        if (length <= 0 || length > static_cast<std::streamoff>(max_bytes)) throw std::runtime_error("Level XML size outside limits");
        std::vector<std::uint8_t> bytes(static_cast<std::size_t>(length));
        file.seekg(0);
        if (!file.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(bytes.size())))
            throw std::runtime_error("Cannot read level XML");
        return decode_level_manifest(bytes, output, error);
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

} // namespace dh::foundation
