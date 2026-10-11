#include "ambient_decor_v1.hpp"

#include "../../content_paths.hpp"
#include "../../../asset-payloads/payloads.hpp"
#include "../../../engine-resources/resources.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <map>
#include <set>

namespace dh::foundation::scene_decor {
namespace {
constexpr std::uint32_t channel_color = 86, channel_offset_v = 87, channel_offset_u = 88;

std::string lower(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return char(std::tolower(c)); });
    return text;
}
std::string base_name(const std::string& path) {
    const auto slash = path.find_last_of("/\\");
    return slash == std::string::npos ? path : path.substr(slash + 1);
}
} // namespace

std::array<float, 4> sample_ambient_track(const AmbientTrack& track, double ms) {
    if (track.values.empty() || track.times.size() != track.values.size()) return {};
    if (ms <= track.times.front()) return track.values.front();
    if (ms >= track.times.back()) return track.values.back();
    // Last key whose time is <= ms.
    const auto upper = std::upper_bound(track.times.begin(), track.times.end(), ms,
                                        [](double value, std::int32_t time) { return value < time; });
    const std::size_t next = std::size_t(upper - track.times.begin()), key = next - 1;
    if (!track.linear) return track.values[key];
    const double span = double(track.times[next]) - double(track.times[key]);
    const float f = span > 0 ? float((ms - track.times[key]) / span) : 0.0f;
    std::array<float, 4> out{};
    for (int c = 0; c < 4; ++c) out[c] = track.values[key][c] + (track.values[next][c] - track.values[key][c]) * f;
    return out;
}

bool parse_ambient_model(const std::vector<std::uint8_t>& bytes, AmbientDecorModel& out, std::string& error) {
    out = {};
    dh2::resources::BresView view{};
    if (dh2_bres_open(&view, bytes.data(), bytes.size()) != dh2::resources::BresError::ok) {
        error = "BRES rejected";
        return false;
    }
    const auto count = dh2_bres_library_count(&view, dh2::resources::Library::animation);
    bool haveRange = false;
    for (std::uint32_t i = 0; i < count; ++i) {
        dh2::assets::Animation animation{};
        if (dh2_animation_open(&animation, &view, std::int32_t(i), 0) != dh2::assets::Error::ok) continue;
        const char* target = dh2_animation_target(&animation);
        if (!target) continue;
        AmbientMaterialClip clip;
        clip.materialId = target;
        const auto channels = dh2_animation_channels(&animation);
        for (std::uint32_t c = 0; c < channels; ++c) {
            const auto type = dh2_animation_type(&animation, std::int32_t(c));
            if (type != channel_color && type != channel_offset_u && type != channel_offset_v) continue;
            int& slot = type == channel_color ? clip.color : type == channel_offset_u ? clip.offsetU : clip.offsetV;
            if (slot >= 0) continue; // first channel of each kind (the second sampler layer is not representable)
            dh2::assets::Vector values{};
            if (!dh2_animation_vector(&animation, std::int32_t(c), true, &values) || !values.count || values.components > 4) continue;
            AmbientTrack track;
            track.linear = dh2_animation_interpolation(&animation, std::int32_t(c)) != 0;
            const float scale = values.type == 1 ? 1.0f / 255.0f : 1.0f; // type 1 = unsigned bytes (colour keys)
            bool ok = true;
            for (std::uint32_t k = 0; k < values.count && ok; ++k) {
                float raw[4]{};
                if (!dh2_vector_read(&values, k, raw)) { ok = false; break; }
                std::array<float, 4> value{};
                for (unsigned q = 0; q < values.components; ++q) value[q] = raw[q] * scale;
                track.values.push_back(value);
                track.times.push_back(dh2_animation_key_time(&animation, std::int32_t(c), std::int32_t(k)));
            }
            if (!ok || !std::is_sorted(track.times.begin(), track.times.end())) continue;
            slot = int(out.tracks.size());
            out.tracks.push_back(std::move(track));
        }
        if (clip.offsetU < 0 && clip.offsetV < 0 && clip.color < 0) continue;
        if (!haveRange) { out.startMs = animation.segment_start; out.endMs = animation.segment_end; haveRange = true; }
        else { out.startMs = std::min(out.startMs, animation.segment_start); out.endMs = std::max(out.endMs, animation.segment_end); }
        out.clips.push_back(std::move(clip));
    }
    error.clear();
    return true;
}

bool AmbientDecorRuntimeV1::load(const AssetCatalog& assets, const std::vector<ActorDefinition>& definitions,
                                 const std::vector<std::string>& skipNames, std::vector<std::string>& notices,
                                 std::string& error) {
    error.clear();
    instances_.clear();
    const std::set<std::string> skip(skipNames.begin(), skipNames.end());
    std::map<std::string, std::vector<std::uint8_t>> bytesByModel;
    std::map<std::string, std::shared_ptr<AmbientDecorModel>> clipsByModel;
    std::set<std::string> reported;
    for (const auto& definition : definitions) {
        // AnimatedDecor (material-animated effects) and Decor (static props: corpses, piles, bloodstain decals) share the
        // declaration form dae + xrefobject; Decor simply has no clips.
        if ((definition.gametype != "AnimatedDecor" && definition.gametype != "Decor") || definition.modelPath.empty()) continue;
        if (skip.count(definition.name)) continue;
        if (lower(base_name(definition.modelPath)).rfind("cs_", 0) == 0) continue; // cutscene clip libraries
        const auto xref = definition.properties.find("xrefobject");
        const auto report = [&](const std::string& text) {
            if (reported.insert(text).second) notices.push_back(text);
        };
        if (xref == definition.properties.end() || xref->second.empty()) {
            report("ambient decor " + definition.name + " has no xrefobject");
            continue;
        }
        try {
            auto bytes = bytesByModel.find(definition.modelPath);
            if (bytes == bytesByModel.end()) bytes = bytesByModel.emplace(definition.modelPath, read_content(assets, definition.modelPath)).first;
            auto clips = clipsByModel.find(definition.modelPath);
            if (clips == clipsByModel.end()) {
                auto model = std::make_shared<AmbientDecorModel>();
                std::string parseError;
                if (!parse_ambient_model(bytes->second, *model, parseError)) { report("ambient decor model " + definition.modelPath + ": " + parseError); continue; }
                model->path = definition.modelPath;
                clips = clipsByModel.emplace(definition.modelPath, std::move(model)).first;
            }
            Instance instance;
            instance.name = definition.name;
            instance.model = definition.modelPath;
            instance.clip = clips->second;
            instance.scene = std::make_unique<OriginalScene>();
            std::string decodeError;
            if (!decode_original_scene_module(bytes->second, xref->second + "-node", definition.placement, *instance.scene, decodeError)) {
                // Particle-only roots (GL_PCloud dust motes) carry no triangle geometry: expected, reported once per root.
                report("ambient decor " + xref->second + " in " + definition.modelPath + " has no static geometry: " + decodeError);
                continue;
            }
            for (auto& range : instance.scene->mesh.ranges) {
                if (range.material.additive) range.material.lightingEnabled = false; // additive effect quads are unlit
            }
            instance.scene->mesh.staticGeometry = true;
            instance.rangeClip.assign(instance.scene->mesh.ranges.size(), -1);
            for (std::size_t k = 0; k < instance.rangeClip.size() && k < instance.scene->materials.size(); ++k) {
                for (std::size_t c = 0; c < instance.clip->clips.size(); ++c)
                    if (instance.clip->clips[c].materialId == instance.scene->materials[k].id) { instance.rangeClip[k] = int(c); break; }
            }
            instances_.push_back(std::move(instance));
        } catch (const std::exception& failure) {
            report("ambient decor " + definition.name + " unavailable: " + failure.what());
        }
    }
    return true;
}

void AmbientDecorRuntimeV1::bind_textures(const std::function<std::uint32_t(const OriginalMaterial&)>& fn) {
    for (auto& instance : instances_)
        for (std::size_t k = 0; k < instance.scene->mesh.ranges.size() && k < instance.scene->materials.size(); ++k)
            instance.scene->mesh.ranges[k].material.texture = fn(instance.scene->materials[k]);
}

void AmbientDecorRuntimeV1::update(double seconds) {
    for (auto& instance : instances_) {
        instance.clockMs += seconds * 1000.0;
        const auto& model = *instance.clip;
        const double length = double(model.endMs) - double(model.startMs);
        const double ms = length > 0 ? model.startMs + std::fmod(instance.clockMs, length) : model.startMs;
        auto& ranges = instance.scene->mesh.ranges;
        for (std::size_t k = 0; k < ranges.size() && k < instance.rangeClip.size(); ++k) {
            if (instance.rangeClip[k] < 0) continue;
            const auto& clip = model.clips[std::size_t(instance.rangeClip[k])];
            auto& material = ranges[k].material;
            if (clip.offsetU >= 0 || clip.offsetV >= 0) {
                // The animated offset replaces the material's authored texture-matrix translation (t[8], t[9]) that the
                // decoder already folded into the vertex UVs.
                const auto& tm = instance.scene->materials[k].textureMatrix;
                material.uvOffset[0] = clip.offsetU >= 0 ? sample_ambient_track(model.tracks[std::size_t(clip.offsetU)], ms)[0] - tm[8] : 0.0f;
                material.uvOffset[1] = clip.offsetV >= 0 ? sample_ambient_track(model.tracks[std::size_t(clip.offsetV)], ms)[0] - tm[9] : 0.0f;
            }
            if (clip.color >= 0) {
                const auto value = sample_ambient_track(model.tracks[std::size_t(clip.color)], ms);
                for (int c = 0; c < 4; ++c) material.color[std::size_t(c)] = std::clamp(value[std::size_t(c)], 0.0f, 1.0f);
            }
        }
    }
}

} // namespace dh::foundation::scene_decor
