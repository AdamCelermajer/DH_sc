#include "../game/scene_mesh_adapter.hpp"
#include "../game/prince_character_runtime.hpp"
#include "../game/swamp_actor_floor_bridge.hpp"
#include "../game/swamp_actor_session.hpp"
#include "alpha_map_policy.hpp"
#include "control_policy.hpp"
#include "../../android-app/scene_buffers.hpp"
#include "../../android-app/swamp_render_policy.hpp"
#include "../../world-data/world.hpp"
#include "../../world-data/world_scene.hpp"
#include "../../navigation/navigation.hpp"
#include "../../level-world/character_scene.hpp"
#include "../../level-world/decor_body_config.hpp"
#include "../../swamp-movement/movement.hpp"
#include "../../engine-resources/resources.hpp"
#include "../../scene-payloads/scene.hpp"
#include "../../texture-assets/texture.hpp"

#include <irrlicht.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <android/native_window.h>
#include <android_native_app_glue.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <string>
#include <vector>

namespace {
constexpr char kTag[] = "DH2IrrlichtSwamp";
constexpr char kBresAsset[] = "dh2/swamp.bdae";
constexpr char kMlxAsset[] = "dh2/swamp.mlx";
constexpr char kSpawnAsset[] = "dh2/swamp-entry-mgp.mgp";
constexpr char kDiffuseAsset[] = "dh2/swamp-diffuse.tga";
constexpr char kAlphaMapAsset[] = "dh2/swamp-alpha.tga";
constexpr char kPrinceModelAsset[] = "dh2/prince-modular.bdae";
constexpr char kPrinceAtlasAsset[] = "dh2/prince-atlas.tga";
constexpr std::uint32_t kPlayerPathMask =
    dh2::irrlicht_game::swamp_module_zero_player_path_mask;

void log(int priority, const char* message) {
    __android_log_print(priority, kTag, "%s", message ? message : "");
}

bool read_asset(AAssetManager* manager, const char* path,
                std::vector<std::uint8_t>* output) {
    if (!manager || !path || !output) return false;
    AAsset* asset = AAssetManager_open(manager, path, AASSET_MODE_BUFFER);
    if (!asset) return false;
    const auto length = AAsset_getLength64(asset);
    if (length <= 0 || length > 64 * 1024 * 1024) {
        AAsset_close(asset);
        return false;
    }
    output->resize(static_cast<std::size_t>(length));
    std::size_t offset = 0;
    while (offset < output->size()) {
        const auto got = AAsset_read(asset, output->data() + offset,
                                     output->size() - offset);
        if (got <= 0) {
            AAsset_close(asset);
            output->clear();
            return false;
        }
        offset += static_cast<std::size_t>(got);
    }
    AAsset_close(asset);
    return true;
}

bool read_prince_asset(void* context, const char* path,
                       std::vector<std::uint8_t>* output, std::string* error) {
    auto* manager = static_cast<AAssetManager*>(context);
    if (read_asset(manager, path, output)) return true;
    if (error) *error = std::string("Android APK Prince asset is missing: ") +
                         (path ? path : "(null)");
    return false;
}

bool build_actor_floor_bridge(const dh2::navigation::Navigation& navigation,
                              dh2::irrlicht_game::SwampActorFloorBridge& bridge,
                              std::string& error) {
    std::vector<dh2::irrlicht_game::SwampSourceSurfaceView> surfaces(
        navigation.surface_count);
    std::vector<dh2::irrlicht_game::SwampSourceTriangleView> triangles(
        navigation.triangle_count);
    for (std::uint32_t i = 0; i < navigation.surface_count; ++i) {
        dh2::navigation::Surface surface{};
        if (dh2_nav_surface(&navigation, i, &surface) !=
            dh2::navigation::Error::ok) {
            error = "Source SWAMP Navigation surface copy failed";
            return false;
        }
        auto& output = surfaces[i];
        output.source_surface_index = i;
        output.module_index = surface.module_index;
        output.module_source_record = surface.module_source_record;
        output.node_record = surface.node_record;
        output.geometry_index = surface.geometry_index;
        output.visible = surface.visible;
        output.first_triangle = surface.first_triangle;
        output.triangle_count = surface.triangle_count;
        output.vertex_count = surface.vertex_count;
        output.primitive_count = surface.primitive_count;
        output.floor_type_flags = surface.floor_type_flags;
        output.floor_type_flags_known = surface.floor_type_flags_known ? 1U : 0U;
        output.floor_type_tag_present = surface.floor_type_tag_present ? 1U : 0U;
        std::memcpy(output.module_name, surface.module_name, sizeof(output.module_name));
        std::memcpy(output.source_node_id, surface.source_node_id,
                    sizeof(output.source_node_id));
        std::memcpy(output.source_node_name, surface.source_node_name,
                    sizeof(output.source_node_name));
        std::memcpy(output.source_geometry_id, surface.source_geometry_id,
                    sizeof(output.source_geometry_id));
        std::memcpy(output.source_geometry_name, surface.source_geometry_name,
                    sizeof(output.source_geometry_name));
        std::memcpy(output.floor_type_tag, surface.floor_type_tag,
                    sizeof(output.floor_type_tag));
    }
    for (std::uint32_t i = 0; i < navigation.triangle_count; ++i) {
        dh2::navigation::SurfaceTriangle source{};
        if (dh2_nav_get_triangle(&navigation, i, &source) !=
            dh2::navigation::Error::ok) {
            error = "Source SWAMP Navigation triangle copy failed";
            return false;
        }
        auto& output = triangles[i];
        std::memcpy(output.a, source.a, sizeof(output.a));
        std::memcpy(output.b, source.b, sizeof(output.b));
        std::memcpy(output.c, source.c, sizeof(output.c));
        output.surface_index = source.surface_index;
        output.primitive_index = source.primitive_index;
        output.source_triangle_index = source.source_triangle_index;
    }
    const dh2::irrlicht_game::SwampSourceNavigationView view{
        surfaces.data(), static_cast<std::uint32_t>(surfaces.size()),
        static_cast<std::uint32_t>(surfaces.size()), triangles.data(),
        static_cast<std::uint32_t>(triangles.size()),
        static_cast<std::uint32_t>(triangles.size())};
    return bridge.build_module_zero(view, kPlayerPathMask, error);
}

bool make_source_actor_session_config(
    dh2::irrlicht_game::PrinceActor& actor,
    dh2::irrlicht_game::PrinceCharacterRuntime& character,
    const irr::core::vector3df& spawn,
    dh2::irrlicht_game::SwampActorSessionConfig& config,
    std::string& error) {
    std::array<std::int32_t, 224> resolved{};
    if (!character.copy_resolved_properties(resolved)) {
        error = "Source Prince resolved property sheet is unavailable";
        return false;
    }
    const float origin[3]{0.0f, 0.0f, 0.0f};
    if (!character.update_pose(origin, error)) return false;
    std::array<float, 6> mesh_box{
        std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity()};
    for (const auto& part : actor.parts()) {
        for (const auto& vertex : part.vertices) {
            for (unsigned axis = 0; axis < 3; ++axis) {
                const float value = vertex.position[axis];
                if (!std::isfinite(value)) {
                    error = "Source Prince Idle owner bounds contain a nonfinite vertex";
                    return false;
                }
                mesh_box[axis] = std::min(mesh_box[axis], value);
                mesh_box[axis + 3] = std::max(mesh_box[axis + 3], value);
            }
        }
    }
    for (const float value : mesh_box) {
        if (!std::isfinite(value)) {
            error = "Source Prince Idle owner bounds are empty";
            return false;
        }
    }

    dh2::physical::CharacterOwnerBoundsInput owner_input{};
    std::copy(mesh_box.begin(), mesh_box.end(), owner_input.mesh_box);
    owner_input.position[0] = spawn.X;
    owner_input.position[1] = spawn.Y;
    owner_input.position[2] = spawn.Z;
    owner_input.collision_scale = resolved[16];
    owner_input.already_scaled = 0; // Freshly created source actor owner.
    owner_input.previous_flat = 0;
    dh2::physical::CharacterOwnerBounds owner_bounds{};
    if (dh2_character_owner_bounds(&owner_bounds, &owner_input) != 0) {
        error = "Recovered source Character owner-bounds producer rejected Prince";
        return false;
    }

    float world_bounds[4]{};
    if (dh2_decor_level_world_bounds(world_bounds) != 0) {
        error = "Recovered source Level physical-world bounds producer failed";
        return false;
    }
    std::copy(world_bounds, world_bounds + 4,
              config.physics_world_bounds.begin());
    config.initial_position = {spawn.X, spawn.Y, spawn.Z};
    std::copy(owner_bounds.relative_box, owner_bounds.relative_box + 6,
              config.local_bounds.begin());
    std::copy(owner_bounds.absolute_box, owner_bounds.absolute_box + 6,
              config.absolute_bounds.begin());
    config.resolved_character_properties = resolved;
    return true;
}

struct TextureContext {
    irr::video::ITexture* diffuse = nullptr;
    irr::video::ITexture* alpha_map_diffuse = nullptr;
    const std::vector<dh2::irrlicht_swamp::AlphaMapMode>* alpha_modes = nullptr;
    std::uint32_t assigned_draws = 0;
    std::uint32_t alpha_map_al_draws = 0;
    std::uint32_t alpha_map_at_draws = 0;
    std::uint32_t alpha_map_selector_unresolved_draws = 0;
};

bool load_source_alpha_modes(
    const dh2::resources::BresView& bres,
    std::vector<dh2::irrlicht_swamp::AlphaMapMode>& modes,
    std::vector<std::string>& selector_summaries,
    std::string& error) {
    const auto material_count = dh2_bres_library_count(
        &bres, dh2::resources::Library::material);
    if (!material_count || material_count > 4096) {
        error = "SWAMP BRES material count is outside the selector reader limit";
        return false;
    }
    modes.assign(material_count, dh2::irrlicht_swamp::AlphaMapMode::unsupported);
    selector_summaries.assign(material_count, "(no checked selector)");
    for (std::uint32_t i = 0; i < material_count; ++i) {
        dh2::materials::Material material{};
        if (dh2_material_record(&material, &bres, static_cast<std::int32_t>(i)) !=
                dh2::materials::Error::ok) {
            error = "SWAMP material inventory failed checked BRES decoding";
            return false;
        }
        std::vector<dh2::scene_materials::CurrentTechnique> selectors;
        std::string selector_error;
        if (dh2::scene_materials::material_current_techniques(
                material, selectors, selector_error) !=
                dh2::scene_materials::TechniqueSelectorError::ok) {
            selector_summaries[i] = "(selector decode failed: " + selector_error + ")";
            continue;
        }
        const auto selection = dh2::irrlicht_swamp::resolve_alpha_map_selection(selectors);
        modes[i] = selection.mode;
        if (selectors.empty()) continue;
        selector_summaries[i].clear();
        for (const auto& selector : selectors) {
            if (!selector_summaries[i].empty()) selector_summaries[i] += ";";
            selector_summaries[i] += selector.profile + ":" + selector.name;
        }
    }
    return true;
}

irr::video::ITexture* resolve_texture(
    const dh2::viewer::SceneDrawDescriptor& draw,
    const dh2::viewer::SceneTextureReference* references,
    std::uint32_t count, void* opaque) {
    auto* context = static_cast<TextureContext*>(opaque);
    if (!context || !references) return nullptr;
    const auto mode = context->alpha_modes
        ? dh2::irrlicht_swamp::alpha_map_mode_for_material(
              draw.material_index, *context->alpha_modes)
        : dh2::irrlicht_swamp::AlphaMapMode::unsupported;
    if (dh2::irrlicht_swamp::swamp_draw_uses_alpha_map_mode(
            draw, references, count, mode)) {
        if (!context->alpha_map_diffuse) return nullptr;
        ++context->assigned_draws;
        if (mode == dh2::irrlicht_swamp::AlphaMapMode::source_al_fractional)
            ++context->alpha_map_al_draws;
        else
            ++context->alpha_map_at_draws;
        return context->alpha_map_diffuse;
    }
    if (dh2::irrlicht_swamp::swamp_draw_has_alpha_map_pair(
            draw, references, count))
        ++context->alpha_map_selector_unresolved_draws;
    for (std::uint32_t i = 0; i < count; ++i) {
        const auto& reference = references[i];
        if (dh2::irrlicht_swamp::is_swamp_diffuse_reference(reference) &&
            context->diffuse) {
            ++context->assigned_draws;
            return context->diffuse;
        }
    }
    return nullptr;
}

struct PrinceTextureContext {
    irr::video::ITexture* atlas = nullptr;
};

irr::video::ITexture* resolve_prince_texture(
    const dh2::irrlicht_game::PrinceMeshPart& part, void* opaque) {
    auto* context = static_cast<PrinceTextureContext*>(opaque);
    if (!context || !context->atlas || part.diffuse_texture.empty()) return nullptr;
    const auto* basename = std::strrchr(part.diffuse_texture.c_str(), '/');
    basename = basename ? basename + 1 : part.diffuse_texture.c_str();
    return std::strcmp(basename, "atlas_modular_warrior.tga") == 0
        ? context->atlas : nullptr;
}

bool decode_pvrtc_rgba8(
    const std::vector<std::uint8_t>& bytes, const char* label,
    std::vector<std::uint8_t>* rgba, irr::u32* width_out,
    irr::u32* height_out, dh2::textures::Format* format_out) {
    if (bytes.empty() || !rgba || !width_out || !height_out) return false;
    dh2::textures::TextureView view{};
    const auto opened = dh2_texture_open(&view, bytes.data(), bytes.size());
    if (opened != dh2::textures::Error::ok || !view.width || !view.height ||
        view.width > 4096 || view.height > 4096 ||
        std::uint64_t(view.width) * view.height > 16U * 1024U * 1024U ||
        (view.format != dh2::textures::Format::pvrtc_2bpp &&
         view.format != dh2::textures::Format::pvrtc_4bpp)) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
            "%s texture rejected: status=%u format=%u size=%ux%u", label,
            static_cast<unsigned>(opened), static_cast<unsigned>(view.format),
            view.width, view.height);
        return false;
    }

    const std::size_t size = std::size_t(view.width) * view.height * 4U;
    rgba->resize(size);
    const auto decoded = dh2_texture_decode_rgba8(
        rgba->data(), rgba->size(), std::size_t(view.width) * 4U,
        bytes.data(), bytes.size());
    if (decoded != dh2::textures::Error::ok) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
                            "%s PVRTC decode failed: status=%u", label,
                            static_cast<unsigned>(decoded));
        rgba->clear();
        return false;
    }
    *width_out = view.width;
    *height_out = view.height;
    if (format_out) *format_out = view.format;
    return true;
}

irr::video::ITexture* upload_rgba8_texture(
    irr::IrrlichtDevice* device, const char* name, irr::u32 width,
    irr::u32 height, const std::vector<std::uint8_t>& rgba) {
    if (!device || !width || !height ||
        rgba.size() != std::size_t(width) * height * 4U) return nullptr;
    std::vector<std::uint8_t> bgra(rgba.size());
    for (std::size_t i = 0; i < rgba.size(); i += 4) {
        bgra[i] = rgba[i + 2];
        bgra[i + 1] = rgba[i + 1];
        bgra[i + 2] = rgba[i];
        bgra[i + 3] = rgba[i + 3];
    }
    auto* image = device->getVideoDriver()->createImageFromData(
        irr::video::ECF_A8R8G8B8,
        irr::core::dimension2d<irr::u32>(width, height),
        bgra.data(), false);
    if (!image) return nullptr;
    auto* texture = device->getVideoDriver()->addTexture(name, image);
    image->drop();
    return texture;
}

bool decode_swamp_textures(
    irr::IrrlichtDevice* device,
    const std::vector<std::uint8_t>& diffuse_bytes,
    const std::vector<std::uint8_t>& alpha_map_bytes,
    irr::video::ITexture** diffuse_out,
    irr::video::ITexture** alpha_map_out) {
    if (diffuse_out) *diffuse_out = nullptr;
    if (alpha_map_out) *alpha_map_out = nullptr;
    if (!device || !diffuse_out || !alpha_map_out) return false;

    std::vector<std::uint8_t> diffuse_rgba, alpha_map_rgba;
    irr::u32 diffuse_width = 0, diffuse_height = 0;
    irr::u32 alpha_width = 0, alpha_height = 0;
    dh2::textures::Format alpha_format = dh2::textures::Format::unknown;
    if (!decode_pvrtc_rgba8(diffuse_bytes, "env_swamp",
                            &diffuse_rgba, &diffuse_width, &diffuse_height,
                            nullptr) ||
        !decode_pvrtc_rgba8(alpha_map_bytes, "pvr2_env_swamp_alpha",
                            &alpha_map_rgba, &alpha_width, &alpha_height,
                            &alpha_format))
        return false;
    if (alpha_format != dh2::textures::Format::pvrtc_2bpp) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
            "SWAMP AlphaMap is not the expected PVRTC2 format: format=%u",
            static_cast<unsigned>(alpha_format));
        return false;
    }
    if (diffuse_width != alpha_width || diffuse_height != alpha_height) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
            "SWAMP diffuse/AlphaMap dimensions differ: diffuse=%ux%u AlphaMap=%ux%u",
            diffuse_width, diffuse_height, alpha_width, alpha_height);
        return false;
    }

    *diffuse_out = upload_rgba8_texture(
        device, "dh2-env-swamp", diffuse_width, diffuse_height, diffuse_rgba);
    if (!*diffuse_out) return false;

    if (!dh2::irrlicht_swamp::apply_swamp_alpha_map(
            diffuse_rgba.data(), alpha_map_rgba.data(),
            std::size_t(diffuse_width) * diffuse_height)) {
        log(ANDROID_LOG_ERROR, "SWAMP AlphaMap composition failed");
        return false;
    }
    *alpha_map_out = upload_rgba8_texture(
        device, "dh2-env-swamp-alpha-map-blue-to-alpha", diffuse_width, diffuse_height,
        diffuse_rgba);
    if (!*alpha_map_out) {
        log(ANDROID_LOG_ERROR, "SWAMP AlphaMap blue-to-diffuse-alpha texture upload failed");
        return false;
    }
    return true;
}

class MovementInput final : public irr::IEventReceiver {
public:
    void set_screen(irr::u32 width, irr::u32 height) {
        width_ = std::max<irr::u32>(1, width);
        height_ = std::max<irr::u32>(1, height);
    }

    void reset() {
        active_id_ = kNoPointer;
        stick_x_ = stick_y_ = 0.0f;
    }

    bool active() const { return active_id_ != kNoPointer; }
    irr::s32 center_x() const { return static_cast<irr::s32>(width_ * 0.18f); }
    irr::s32 center_y() const { return static_cast<irr::s32>(height_ * 0.77f); }
    irr::s32 pad_radius() const {
        return static_cast<irr::s32>(std::max(1.0f,
            std::min(width_ * 0.18f, height_ * 0.24f)));
    }

    bool OnEvent(const irr::SEvent& event) override {
        // Irrlicht forwards Android lifecycle commands to the event receiver
        // before updating its own Focused/Paused flags. Clear held movement
        // here on the exact loss/pause event, then return false so Irrlicht's
        // native handler still updates those flags.
        if (event.EventType == irr::EET_SYSTEM_EVENT &&
            event.SystemEvent.EventType == irr::ESET_ANDROID_CMD) {
            const auto command = event.SystemEvent.AndroidCmd.Cmd;
            if (command == APP_CMD_LOST_FOCUS || command == APP_CMD_PAUSE ||
                command == APP_CMD_TERM_WINDOW) {
                reset();
            }
            return false;
        }
        if (event.EventType != irr::EET_TOUCH_INPUT_EVENT) return false;
        const auto& touch = event.TouchInput;
        if (touch.Event == irr::ETIE_PRESSED_DOWN) {
            // Use a visible fixed lower-left joystick. Touches elsewhere in
            // the module remain available for ordinary device interaction.
            const float dx = touch.X - center_x();
            const float dy = touch.Y - center_y();
            if (active_id_ != kNoPointer ||
                dx * dx + dy * dy > float(pad_radius()) * pad_radius()) return false;
            active_id_ = touch.ID;
            origin_x_ = center_x();
            origin_y_ = center_y();
            update(touch.X, touch.Y);
            return true;
        }
        if (touch.ID != active_id_) return false;
        if (touch.Event == irr::ETIE_LEFT_UP) {
            active_id_ = kNoPointer;
            stick_x_ = stick_y_ = 0.0f;
            return true;
        }
        if (touch.Event == irr::ETIE_MOVED) {
            update(touch.X, touch.Y);
            return true;
        }
        return false;
    }

    float x() const { return stick_x_; }
    float y() const { return stick_y_; }

private:
    static constexpr std::size_t kNoPointer = static_cast<std::size_t>(-1);
    void update(irr::s32 x, irr::s32 y) {
        const auto stick = dh2::irrlicht_swamp::source_stick_from_screen_delta(
            static_cast<float>(x - origin_x_),
            static_cast<float>(y - origin_y_),
            static_cast<float>(pad_radius()));
        stick_x_ = stick.x;
        stick_y_ = stick.y;
    }

    irr::u32 width_ = 1, height_ = 1;
    std::size_t active_id_ = kNoPointer;
    irr::s32 origin_x_ = 0, origin_y_ = 0;
    float stick_x_ = 0.0f, stick_y_ = 0.0f;
};

bool assemble_source_module_zero(
    const std::vector<std::uint8_t>& bres_bytes,
    const std::vector<std::uint8_t>& mlx_bytes,
    const std::vector<std::uint8_t>& spawn_bytes,
    dh2::world::SourceLevel* level,
    dh2::resources::BresView* bres,
    dh2::scene_payload::Scene* source_scene,
    dh2::navigation::Navigation* navigation,
    dh2::viewer::SceneMesh* output_mesh,
    irr::IrrlichtDevice* device,
    const std::vector<std::uint8_t>& diffuse_bytes,
    const std::vector<std::uint8_t>& alpha_map_bytes,
    irr::core::vector3df* spawn_out,
    std::uint32_t* source_draws_out,
    std::uint32_t* visible_draws_out,
    dh2::irrlicht_adapter::SourceRoomRenderCounts* room_render_counts_out,
    std::uint32_t* additive_draws_out,
    std::uint32_t* additive_one_one_mapped_out,
    std::uint32_t* diffuse_refs_out,
    std::uint32_t* alpha_refs_out,
    std::uint32_t* unresolved_alpha_refs_out,
    std::uint32_t* lightmap_refs_out,
    std::uint32_t* specular_refs_out,
    std::uint32_t* other_refs_out,
    std::uint32_t* no_texture_out,
    std::uint32_t* visible_no_texture_out,
    std::uint32_t* assigned_draws_out,
    std::uint32_t* alpha_map_al_draws_out,
    std::uint32_t* alpha_map_at_draws_out,
    std::uint32_t* alpha_map_selector_unresolved_draws_out) {
    dh2::world::Diagnostic world_diag{};
    if (dh2_world_import_level(level, "SWAMP", "data/scene/001_swamp.mlx",
            mlx_bytes.data(), mlx_bytes.size(), &world_diag) != dh2::world::Error::ok) {
        __android_log_print(ANDROID_LOG_ERROR, kTag, "MLX import: %s", world_diag.message);
        return false;
    }
    if (!level->module_count || std::strcmp(level->modules[0].cache_dae,
            "data/3d/modules/swamp/swamp.bdae") != 0 ||
        dh2_world_import_module_objects(level, 0, dh2::world::RecordKind::mgp,
            level->modules[0].cache_mgp, spawn_bytes.data(), spawn_bytes.size(),
            &world_diag) != dh2::world::Error::ok) {
        __android_log_print(ANDROID_LOG_ERROR, kTag, "module-zero MGP import: %s",
            world_diag.message[0] ? world_diag.message : "unexpected SWAMP catalogue path");
        return false;
    }

    const dh2::world::Object* entry = nullptr;
    for (std::uint32_t i = 0; i < level->entity_count; ++i) {
        const auto& object = level->entities[i];
        const char* id = dh2_world_field(&object, "entrypointID");
        if (object.module_index == 0 && object.kind == dh2::world::RecordKind::mgp &&
            object.gametype && std::strcmp(object.gametype, "SpawnPoint") == 0 &&
            id && std::strcmp(id, "0") == 0) {
            if (entry) {
                log(ANDROID_LOG_ERROR, "Multiple module-zero entrypointID 0 SpawnPoints");
                return false;
            }
            entry = &object;
        }
    }
    if (!entry) {
        log(ANDROID_LOG_ERROR, "Module-zero entrypointID 0 SpawnPoint is missing");
        return false;
    }

    if (dh2_bres_open(bres, bres_bytes.data(), bres_bytes.size()) !=
            dh2::resources::BresError::ok ||
        dh2_scene_open(source_scene, bres) != dh2::scene_payload::Error::ok) {
        log(ANDROID_LOG_ERROR, "SWAMP BRES/scene import failed");
        return false;
    }
    dh2::navigation::Diagnostic nav_diag{};
    if (dh2_nav_build_swamp(navigation, level, source_scene, &nav_diag) !=
            dh2::navigation::Error::ok) {
        __android_log_print(ANDROID_LOG_ERROR, kTag, "SWAMP navigation: %s", nav_diag.message);
        return false;
    }

    dh2::world::ModuleBinding binding{};
    if (dh2_world_bind_module(&binding, &level->modules[0], source_scene,
                              &world_diag) != dh2::world::Error::ok) {
        __android_log_print(ANDROID_LOG_ERROR, kTag, "module-zero root binding: %s",
                            world_diag.message);
        return false;
    }
    std::vector<std::uint32_t> records(65536);
    std::uint32_t record_count = 0;
    if (dh2_world_module_records(records.data(), records.size(), &record_count,
            &binding, source_scene, &world_diag) != dh2::world::Error::ok) {
        __android_log_print(ANDROID_LOG_ERROR, kTag, "module-zero subtree: %s",
                            world_diag.message);
        return false;
    }
    dh2::math::Matrix4f correction{};
    if (dh2_world_placement_matrix(&correction, &binding, &world_diag) !=
            dh2::world::Error::ok ||
        dh2_world_scene_mesh_nodes(output_mesh, bres, records.data(), record_count,
                                   &correction) != dh2::viewer::SceneMeshError::ok) {
        __android_log_print(ANDROID_LOG_ERROR, kTag, "module-zero SceneMesh: %s",
                            world_diag.message);
        return false;
    }

    std::vector<dh2::irrlicht_swamp::AlphaMapMode> material_alpha_modes;
    std::vector<std::string> material_selector_summaries;
    std::string selector_error;
    if (!load_source_alpha_modes(*bres, material_alpha_modes,
                                 material_selector_summaries, selector_error)) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
                            "SWAMP CurrentTechnique inventory: %s",
                            selector_error.c_str());
        return false;
    }

    irr::video::ITexture* diffuse = nullptr;
    irr::video::ITexture* alpha_map_diffuse = nullptr;
    if (!decode_swamp_textures(device, diffuse_bytes, alpha_map_bytes,
                               &diffuse, &alpha_map_diffuse)) {
        log(ANDROID_LOG_ERROR, "Could not decode/compose SWAMP diffuse and AlphaMap textures");
        return false;
    }
    TextureContext texture_context{diffuse, alpha_map_diffuse,
                                   &material_alpha_modes};
    std::vector<dh2::viewer::SceneDrawDescriptor> renderer_draws(
        output_mesh->draws, output_mesh->draws + output_mesh->draw_commands);
    std::uint32_t visible = 0, additive_draws = 0;
    dh2::irrlicht_adapter::SourceRoomRenderCounts expected_room_filter{};
    std::uint32_t additive_one_one_mapped = 0;
    std::uint32_t diffuse_refs = 0, alpha_refs = 0, unresolved_alpha_refs = 0;
    std::uint32_t lightmap_refs = 0;
    std::uint32_t specular_refs = 0, other_refs = 0;
    std::uint32_t no_texture = 0, visible_no_texture = 0;
    std::uint32_t alpha_selector_al_draws = 0, alpha_selector_at_draws = 0;
    std::uint32_t alpha_selector_unresolved_draws = 0;
    std::vector<bool> selector_logged(material_alpha_modes.size(), false);
    for (auto& draw : renderer_draws) {
        for (std::uint32_t i = 0; i < draw.texture_count; ++i) {
            const auto& ref = output_mesh->texture_references[draw.first_texture + i];
            if (dh2::irrlicht_swamp::is_swamp_diffuse_reference(ref)) ++diffuse_refs;
            else if (std::strcmp(ref.parameter_id, "AlphaMap") == 0) {
                if (dh2::irrlicht_swamp::is_swamp_alpha_map_reference(ref)) ++alpha_refs;
                else ++unresolved_alpha_refs;
            }
            else if (std::strcmp(ref.parameter_id, "LightMap") == 0) ++lightmap_refs;
            else if (std::strcmp(ref.parameter_id, "Specular") == 0) ++specular_refs;
            else ++other_refs;
        }
        if (draw.texture_count && dh2::irrlicht_swamp::swamp_draw_has_alpha_map_pair(
                draw, output_mesh->texture_references + draw.first_texture,
                draw.texture_count)) {
            const auto mode = dh2::irrlicht_swamp::alpha_map_mode_for_material(
                draw.material_index, material_alpha_modes);
            if (mode == dh2::irrlicht_swamp::AlphaMapMode::source_al_fractional)
                ++alpha_selector_al_draws;
            else if (mode == dh2::irrlicht_swamp::AlphaMapMode::source_at_cutout)
                ++alpha_selector_at_draws;
            else
                ++alpha_selector_unresolved_draws;
            if (draw.material_index >= 0 &&
                static_cast<std::size_t>(draw.material_index) < selector_logged.size() &&
                !selector_logged[static_cast<std::size_t>(draw.material_index)]) {
                selector_logged[static_cast<std::size_t>(draw.material_index)] = true;
                const auto index = static_cast<std::size_t>(draw.material_index);
                const bool known_alpha_selector_pair =
                    std::strcmp(draw.material_id, "Material__11611") == 0 &&
                    material_selector_summaries[index] ==
                        "GLES:L1_Vc_Al_----_----_----_----;GLES2:L1_Vc_Al_Sp_----_----_----";
                __android_log_print(ANDROID_LOG_INFO, kTag,
                    "SWAMP CurrentTechnique: source_material_index=%d material=%s profile_selectors={%s} derived_mode=%s named_table_ordinals=%s chosen_effect_group=unresolved active_profile=unresolved compiled_ordinal=unresolved",
                    draw.material_index, draw.material_id,
                    material_selector_summaries[index].c_str(),
                    dh2::irrlicht_swamp::alpha_map_mode_name(mode),
                    known_alpha_selector_pair ? "GLES:2,GLES2:4" : "unresolved");
            }
        }
        no_texture += draw.texture_count == 0 ? 1U : 0U;
        additive_draws += dh2::viewer::swamp_material_uses_additive_one_one(
            draw.material_id) ? 1U : 0U;
        const auto room_role = dh2::irrlicht_adapter::classify_source_room_draw(
            draw, binding.node_record);
        if (draw.visible) {
            switch (room_role) {
            case dh2::irrlicht_adapter::SourceRoomDrawRole::room_root_bounds:
                ++expected_room_filter.room_root_bounds;
                break;
            case dh2::irrlicht_adapter::SourceRoomDrawRole::navigation_floor:
                ++expected_room_filter.navigation_floors;
                break;
            case dh2::irrlicht_adapter::SourceRoomDrawRole::exit_marker:
                ++expected_room_filter.exit_markers;
                break;
            case dh2::irrlicht_adapter::SourceRoomDrawRole::scenery:
                ++visible;
                visible_no_texture += draw.texture_count == 0 ? 1U : 0U;
                break;
            }
        }
    }
    auto renderer_mesh = *output_mesh;
    renderer_mesh.draws = renderer_draws.data();
    dh2::irrlicht_adapter::MeshBuildStatus adapter_status{};
    std::uint32_t buffer_count = 0;
    dh2::irrlicht_adapter::SourceRoomRenderCounts room_filter_counts{};
    auto* mesh = dh2::irrlicht_adapter::build_source_room_scenery_mesh(
        renderer_mesh, binding.node_record, &adapter_status, resolve_texture,
        &texture_context, &buffer_count, &room_filter_counts);
    if (!mesh || adapter_status != dh2::irrlicht_adapter::MeshBuildStatus::ok ||
        buffer_count != visible || !visible ||
        room_filter_counts.room_root_bounds != expected_room_filter.room_root_bounds ||
        room_filter_counts.navigation_floors != expected_room_filter.navigation_floors ||
        room_filter_counts.exit_markers != expected_room_filter.exit_markers) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
            "Irrlicht source-room scenery adapter mismatch status=%u buffers=%u visible=%u expected_root=%u actual_root=%u expected_floor=%u actual_floor=%u expected_exit=%u actual_exit=%u",
            static_cast<unsigned>(adapter_status), buffer_count, visible,
            expected_room_filter.room_root_bounds, room_filter_counts.room_root_bounds,
            expected_room_filter.navigation_floors, room_filter_counts.navigation_floors,
            expected_room_filter.exit_markers, room_filter_counts.exit_markers);
        if (mesh) mesh->drop();
        if (diffuse) diffuse->drop();
        return false;
    }

    for (irr::u32 i = 0; i < mesh->getMeshBufferCount(); ++i) {
        auto* buffer = mesh->getMeshBuffer(i);
        if (!buffer) {
            mesh->drop();
            if (diffuse) diffuse->drop();
            log(ANDROID_LOG_ERROR, "Irrlicht adapter returned a null SWAMP mesh buffer");
            return false;
        }
        auto& material = buffer->getMaterial();
        material.setFlag(irr::video::EMF_LIGHTING, false);
        material.setFlag(irr::video::EMF_BACK_FACE_CULLING, false);
        material.DiffuseColor = irr::video::SColor(255, 255, 255, 255);
        material.AmbientColor = irr::video::SColor(255, 255, 255, 255);
    }
    // Source Material__11598 uses GL_ONE/GL_ONE with GL_FUNC_ADD, depth test
    // LEQUAL, and depth writes disabled. Irrlicht's GLES2 EMT_TRANSPARENT_ADD_COLOR
    // uses ONE/ONE_MINUS_SRC_COLOR, so use ONETEXTURE_BLEND with explicit
    // separate factors and the source ADD equation for these two buffers only.
    irr::u32 material_index = 0;
    for (const auto& draw : renderer_draws) {
        if (!draw.visible ||
            dh2::irrlicht_adapter::classify_source_room_draw(
                draw, binding.node_record) !=
                dh2::irrlicht_adapter::SourceRoomDrawRole::scenery) continue;
        auto* buffer = mesh->getMeshBuffer(material_index++);
        if (!buffer) continue;
        const auto alpha_map_mode = dh2::irrlicht_swamp::alpha_map_mode_for_material(
            draw.material_index, material_alpha_modes);
        const bool alpha_map_source_selected = draw.texture_count &&
            dh2::irrlicht_swamp::swamp_draw_uses_alpha_map_mode(
                draw, output_mesh->texture_references + draw.first_texture,
                draw.texture_count, alpha_map_mode);
        auto& material = buffer->getMaterial();
        if (dh2::viewer::swamp_material_uses_additive_one_one(draw.material_id)) {
            material.MaterialType = irr::video::EMT_ONETEXTURE_BLEND;
            material.MaterialTypeParam = irr::video::pack_textureBlendFuncSeparate(
                irr::video::EBF_ONE, irr::video::EBF_ONE,
                irr::video::EBF_ONE, irr::video::EBF_ONE,
                irr::video::EMFN_MODULATE_1X, irr::video::EAS_NONE);
            material.BlendOperation = irr::video::EBO_ADD;
            material.ZBuffer = irr::video::ECFN_LESSEQUAL;
            material.ZWriteEnable = irr::video::EZW_OFF;
            ++additive_one_one_mapped;
        } else {
            if (alpha_map_source_selected && alpha_map_mode ==
                    dh2::irrlicht_swamp::AlphaMapMode::source_al_fractional) {
                material.MaterialType =
                    irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
                // The AL selector decodes to fractional alpha. This Irrlicht
                // mapping uses its standard SRC_ALPHA blending; original DH2
                // blend/depth state is still unresolved.
                material.ZWriteEnable = irr::video::EZW_AUTO;
            } else if (alpha_map_source_selected && alpha_map_mode ==
                           dh2::irrlicht_swamp::AlphaMapMode::source_at_cutout) {
                material.MaterialType =
                    irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
                material.MaterialTypeParam = 0.8f;
                // Upstream GLES2 implements Color.a < AlphaRef discard. AT
                // therefore uses its recovered 0.8 cutout threshold here.
                // Original DH2 GL blend/depth state remains unresolved.
                material.ZWriteEnable = irr::video::EZW_AUTO;
            } else {
                material.setFlag(irr::video::EMF_ZWRITE_ENABLE,
                    dh2::viewer::swamp_draw_writes_depth(draw.material_id, false));
            }
        }
    }
    if (material_index != mesh->getMeshBufferCount() || additive_draws != 2 ||
        additive_one_one_mapped != additive_draws ||
        texture_context.alpha_map_al_draws + texture_context.alpha_map_at_draws +
            texture_context.alpha_map_selector_unresolved_draws != alpha_refs ||
        texture_context.alpha_map_al_draws != alpha_selector_al_draws ||
        texture_context.alpha_map_at_draws != alpha_selector_at_draws ||
        texture_context.alpha_map_selector_unresolved_draws !=
            alpha_selector_unresolved_draws) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
            "SWAMP material mapping mismatch: additive_source=%u additive_mapped=%u buffers=%u configured=%u AlphaMap_refs=%u AL_draws=%u AT_draws=%u selector_unresolved=%u",
            additive_draws, additive_one_one_mapped, mesh->getMeshBufferCount(),
            material_index, alpha_refs, texture_context.alpha_map_al_draws,
            texture_context.alpha_map_at_draws,
            texture_context.alpha_map_selector_unresolved_draws);
        mesh->drop();
        if (diffuse) diffuse->drop();
        return false;
    }

    auto* node = device->getSceneManager()->addMeshSceneNode(mesh);
    mesh->drop();
    if (!node) {
        if (diffuse) diffuse->drop();
        log(ANDROID_LOG_ERROR, "Irrlicht could not add the assembled SWAMP module");
        return false;
    }
    node->setAutomaticCulling(irr::scene::EAC_OFF);
    // Keep the texture registered with IVideoDriver for the entire lifetime
    // of the scene materials. SMaterial stores a non-owning pointer; dropping
    // the addTexture result here unregisters it and Irrlicht reports
    // "Tried to set a texture not owned by this driver" during every draw.

    bool floor_found = false;
    dh2::navigation::FloorHit floor{};
    const auto floor_error = dh2_nav_query_actor_floor(navigation,
        entry->world_position[0], entry->world_position[1], entry->world_position[2],
        dh2::movement::native_floor_vertical_tolerance,
        dh2::movement::edge_tolerance, kPlayerPathMask, &floor, &floor_found);
    if (floor_error != dh2::navigation::Error::ok || !floor_found ||
        floor.vertical_distance >= dh2::movement::native_floor_vertical_tolerance) {
        log(ANDROID_LOG_ERROR, "Entry point is not on a path-eligible module-zero floor");
        return false;
    }
    dh2::navigation::Surface surface{};
    if (dh2_nav_surface(navigation, floor.surface_index, &surface) !=
            dh2::navigation::Error::ok || surface.module_index != 0) {
        log(ANDROID_LOG_ERROR, "Entry point floor did not resolve to SWAMP module zero");
        return false;
    }
    spawn_out->set(entry->world_position[0], entry->world_position[1], floor.height);

    *source_draws_out = output_mesh->draw_commands;
    *visible_draws_out = visible;
    *room_render_counts_out = room_filter_counts;
    *additive_draws_out = additive_draws;
    *additive_one_one_mapped_out = additive_one_one_mapped;
    *diffuse_refs_out = diffuse_refs;
    *alpha_refs_out = alpha_refs;
    *unresolved_alpha_refs_out = unresolved_alpha_refs;
    *lightmap_refs_out = lightmap_refs;
    *specular_refs_out = specular_refs;
    *other_refs_out = other_refs;
    *no_texture_out = no_texture;
    *visible_no_texture_out = visible_no_texture;
    *assigned_draws_out = texture_context.assigned_draws;
    *alpha_map_al_draws_out = texture_context.alpha_map_al_draws;
    *alpha_map_at_draws_out = texture_context.alpha_map_at_draws;
    *alpha_map_selector_unresolved_draws_out =
        texture_context.alpha_map_selector_unresolved_draws;
    __android_log_print(ANDROID_LOG_INFO, kTag,
        "SWAMP module 0 assembled in Irrlicht r6038: subtree_records=%u source_draws=%u visible_scenery_draws=%u room_root_bounds_filtered=%u navigation_floor_draws_filtered=%u exit_marker_draws_filtered=%u additive_Material__11598=%u additive_one_one_mapped=%u vertices=%u indices=%u path_mask=0x%X start=(%.3f,%.3f,%.3f) diffuse_refs=%u diffuse_draws=%u AlphaMap_refs=%u AlphaMap_unresolved_refs=%u AlphaMap_AL_draws=%u AlphaMap_AT_draws=%u AlphaMap_selector_unresolved_draws=%u alpha_map_policy=source-selector-derived alpha_profile_selection=unresolved effect_named_table_ordinals=GLES:2,GLES2:4 chosen_effect_group=unresolved alpha_compiled_ordinal=unresolved source_blend_depth_state=unresolved AT_threshold=0.8 LightMap_refs=%u Specular_refs=%u other_refs=%u no_texture_source_draws=%u visible_no_texture_draws=%u",
        record_count, *source_draws_out, *visible_draws_out,
        room_filter_counts.room_root_bounds, room_filter_counts.navigation_floors,
        room_filter_counts.exit_markers,
        *additive_draws_out, *additive_one_one_mapped_out,
        output_mesh->vertex_count, output_mesh->index_count,
        kPlayerPathMask, spawn_out->X, spawn_out->Y, spawn_out->Z,
        *diffuse_refs_out, *assigned_draws_out, *alpha_refs_out,
        *unresolved_alpha_refs_out, *alpha_map_al_draws_out,
        *alpha_map_at_draws_out, *alpha_map_selector_unresolved_draws_out,
        *lightmap_refs_out, *specular_refs_out, *other_refs_out,
        *no_texture_out, *visible_no_texture_out);
    return true;
}

irr::core::stringw utf8_wide(const char* text) {
    irr::core::stringw result;
    for (const auto codepoint : dh2::irrlicht_swamp::utf8_codepoints(text))
        result += static_cast<wchar_t>(codepoint);
    return result;
}

void set_follow_camera(irr::scene::ICameraSceneNode* camera,
                       const irr::core::vector3df& player) {
    if (!camera) return;
    // Frame the measured source Prince bounds (Idle height 342.65, Walk
    // height 328.17) around their first-Idle placement center, rather than the
    // earlier 20-unit marker. This is camera framing only; no mesh rescale.
    camera->setTarget(player + irr::core::vector3df(0.0f, 0.0f, 170.0f));
    camera->setPosition(player + irr::core::vector3df(-480.0f, -640.0f, 410.0f));
    camera->setUpVector(irr::core::vector3df(0.0f, 0.0f, 1.0f));
}

void draw_touch_pad(irr::video::IVideoDriver* driver,
                    const MovementInput& input) {
    if (!driver) return;
    const irr::s32 cx = input.center_x(), cy = input.center_y();
    const irr::s32 radius = input.pad_radius();
    const irr::video::SColor outline(210, 193, 214, 228);
    const irr::video::SColor crosshair(150, 151, 178, 195);
    const irr::video::SColor knob(235, 88, 218, 174);
    constexpr irr::s32 segments = 40;
    for (irr::s32 i = 0; i < segments; ++i) {
        const float a = 6.28318530718f * i / segments;
        const float b = 6.28318530718f * (i + 1) / segments;
        driver->draw2DLine(
            irr::core::position2d<irr::s32>(cx + static_cast<irr::s32>(std::cos(a) * radius),
                                            cy + static_cast<irr::s32>(std::sin(a) * radius)),
            irr::core::position2d<irr::s32>(cx + static_cast<irr::s32>(std::cos(b) * radius),
                                            cy + static_cast<irr::s32>(std::sin(b) * radius)),
            outline);
    }
    driver->draw2DLine({cx - radius / 2, cy}, {cx + radius / 2, cy}, crosshair);
    driver->draw2DLine({cx, cy - radius / 2}, {cx, cy + radius / 2}, crosshair);
    const irr::s32 knob_x = cx + static_cast<irr::s32>(input.x() * radius);
    const irr::s32 knob_y = cy - static_cast<irr::s32>(input.y() * radius);
    const irr::s32 knob_radius = std::max<irr::s32>(8, radius / 4);
    for (irr::s32 i = 0; i < segments; ++i) {
        const float a = 6.28318530718f * i / segments;
        const float b = 6.28318530718f * (i + 1) / segments;
        driver->draw2DLine(
            irr::core::position2d<irr::s32>(knob_x + static_cast<irr::s32>(std::cos(a) * knob_radius),
                                            knob_y + static_cast<irr::s32>(std::sin(a) * knob_radius)),
            irr::core::position2d<irr::s32>(knob_x + static_cast<irr::s32>(std::cos(b) * knob_radius),
                                            knob_y + static_cast<irr::s32>(std::sin(b) * knob_radius)),
            knob);
    }
}
}

void android_main(android_app* app) {
    if (!app || !app->activity) return;
    MovementInput input;
    irr::SIrrlichtCreationParameters params;
    params.DriverType = irr::video::EDT_OGLES2;
    params.WindowSize = irr::core::dimension2d<irr::u32>(0, 0);
    params.PrivateData = app;
    params.EventReceiver = &input;
    params.Bits = 24;
    params.ZBufferBits = 16;
    params.AntiAlias = 0;
    params.LoggingLevel = irr::ELL_WARNING;
    auto* device = irr::createDeviceEx(params);
    if (!device) {
        log(ANDROID_LOG_ERROR, "createDeviceEx failed; no Irrlicht render device");
        return;
    }

    auto screen = device->getVideoDriver()->getScreenSize();
    input.set_screen(screen.Width, screen.Height);
    std::vector<std::uint8_t> bres_bytes, mlx_bytes, spawn_bytes, diffuse_bytes,
        alpha_map_bytes, prince_model_bytes, prince_atlas_bytes;
    if (!read_asset(app->activity->assetManager, kBresAsset, &bres_bytes) ||
        !read_asset(app->activity->assetManager, kMlxAsset, &mlx_bytes) ||
        !read_asset(app->activity->assetManager, kSpawnAsset, &spawn_bytes) ||
        !read_asset(app->activity->assetManager, kDiffuseAsset, &diffuse_bytes) ||
        !read_asset(app->activity->assetManager, kAlphaMapAsset, &alpha_map_bytes) ||
        !read_asset(app->activity->assetManager, kPrinceModelAsset, &prince_model_bytes) ||
        !read_asset(app->activity->assetManager, kPrinceAtlasAsset, &prince_atlas_bytes)) {
        log(ANDROID_LOG_ERROR, "Could not read the local SWAMP/Prince cache assets packaged in this APK");
        device->drop();
        return;
    }

    dh2::irrlicht_game::PrinceActor prince;
    std::string prince_error;
    if (!prince.load(prince_model_bytes.data(), prince_model_bytes.size(), prince_error)) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
            "Source Prince rig import failed: %s", prince_error.c_str());
        device->drop();
        return;
    }
    std::vector<std::uint8_t> prince_atlas_rgba;
    irr::u32 prince_atlas_width = 0, prince_atlas_height = 0;
    if (!decode_pvrtc_rgba8(prince_atlas_bytes, "atlas_modular_warrior",
                            &prince_atlas_rgba, &prince_atlas_width,
                            &prince_atlas_height, nullptr)) {
        log(ANDROID_LOG_ERROR, "Source Prince diffuse atlas is not a supported PVRTC asset");
        device->drop();
        return;
    }
    auto* prince_atlas = upload_rgba8_texture(
        device, "dh2-prince-warrior-atlas", prince_atlas_width,
        prince_atlas_height, prince_atlas_rgba);
    if (!prince_atlas) {
        log(ANDROID_LOG_ERROR, "Source Prince diffuse atlas upload failed");
        device->drop();
        return;
    }

    dh2::world::SourceLevel level{};
    dh2::resources::BresView bres{};
    dh2::scene_payload::Scene source_scene{};
    dh2::navigation::Navigation navigation{};
    dh2::viewer::SceneMesh source_mesh{};
    irr::core::vector3df player_position{};
    std::uint32_t source_draws = 0, visible_draws = 0;
    dh2::irrlicht_adapter::SourceRoomRenderCounts room_render_counts{};
    std::uint32_t additive_draws = 0, diffuse_refs = 0, alpha_refs = 0;
    std::uint32_t additive_one_one_mapped = 0;
    std::uint32_t unresolved_alpha_refs = 0;
    std::uint32_t lightmap_refs = 0, specular_refs = 0, other_refs = 0;
    std::uint32_t no_texture_draws = 0, visible_no_texture_draws = 0;
    std::uint32_t textured_draws = 0, alpha_map_al_draws = 0;
    std::uint32_t alpha_map_at_draws = 0, alpha_map_selector_unresolved_draws = 0;
    const bool assembled = assemble_source_module_zero(
        bres_bytes, mlx_bytes, spawn_bytes, &level, &bres, &source_scene,
        &navigation, &source_mesh, device, diffuse_bytes, alpha_map_bytes,
        &player_position,
        &source_draws, &visible_draws, &room_render_counts, &additive_draws,
        &additive_one_one_mapped,
        &diffuse_refs, &alpha_refs, &unresolved_alpha_refs,
        &lightmap_refs, &specular_refs, &other_refs,
        &no_texture_draws, &visible_no_texture_draws, &textured_draws,
        &alpha_map_al_draws, &alpha_map_at_draws,
        &alpha_map_selector_unresolved_draws);
    if (!assembled) {
        dh2_viewer_scene_mesh_free(&source_mesh);
        dh2_nav_free(&navigation);
        dh2_world_free(&level);
        device->drop();
        return;
    }

    dh2::irrlicht_game::PrinceCharacterRuntime prince_character;
    if (!prince_character.load(prince, read_prince_asset,
                               app->activity->assetManager, prince_error)) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
            "Source Prince Character/bank initialization failed: %s",
            prince_error.c_str());
        dh2_viewer_scene_mesh_free(&source_mesh);
        dh2_nav_free(&navigation);
        dh2_world_free(&level);
        device->drop();
        return;
    }

    dh2::irrlicht_game::SwampActorFloorBridge actor_floor;
    if (!build_actor_floor_bridge(navigation, actor_floor, prince_error)) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
            "Source SWAMP actor-floor bridge failed: %s", prince_error.c_str());
        dh2_viewer_scene_mesh_free(&source_mesh);
        dh2_nav_free(&navigation);
        dh2_world_free(&level);
        device->drop();
        return;
    }
    dh2::irrlicht_game::SwampActorSessionConfig actor_config{};
    if (!make_source_actor_session_config(prince, prince_character,
            player_position, actor_config, prince_error)) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
            "Source Prince owner/world config failed: %s", prince_error.c_str());
        dh2_viewer_scene_mesh_free(&source_mesh);
        dh2_nav_free(&navigation);
        dh2_world_free(&level);
        device->drop();
        return;
    }
    // Keep these locals ordered as Prince -> Character -> floor -> session.
    // The session borrows the first three and its destructor must detach its
    // Character body callbacks before their objects or NativeWorld are freed.
    dh2::irrlicht_game::SwampActorSession actor_session;
    if (!actor_session.initialize(prince, prince_character, actor_floor,
                                   actor_config, prince_error)) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
            "Source SWAMP Character/actor session failed: %s", prince_error.c_str());
        dh2_viewer_scene_mesh_free(&source_mesh);
        dh2_nav_free(&navigation);
        dh2_world_free(&level);
        device->drop();
        return;
    }
    auto actor_frame = actor_session.last_frame();
    if (!actor_session.ready() || actor_frame.source_state != 3 ||
        actor_frame.state_flags != prince_character.state_flags() ||
        actor_frame.body_pinned != 1 || actor_frame.body_present != 1 ||
        !prince_character.body_present() ||
        !prince_character.body_services_bound() ||
        actor_frame.source_floor == 0xffffffffu) {
        log(ANDROID_LOG_ERROR, "SWAMP source Actor session did not start in its pinned Idle state");
        actor_session.shutdown();
        dh2_viewer_scene_mesh_free(&source_mesh);
        dh2_nav_free(&navigation);
        dh2_world_free(&level);
        device->drop();
        return;
    }
    player_position.set(actor_frame.position[0], actor_frame.position[1],
                        actor_frame.position[2]);
    __android_log_print(ANDROID_LOG_INFO, kTag,
        "SOURCE_ACTOR_SESSION_READY: source_state=%d state_flags=0x%X sequence=%d clip=%d actor_phase=%u body_present=%u body_pinned=%u body_service_count=%u actor_steps=%u world_steps=%u source_path_segments=%u source_path_requested=%u floor=%u source_xyz=(%.3f,%.3f,%.3f) physics_xy=(%.4f,%.4f) path_mask=0x%X; Character Move/Stop body services bound to NativeWorld after body creation; movement owner is SceneBinding source root",
        actor_frame.source_state, actor_frame.state_flags,
        actor_frame.sequence, actor_frame.clip, actor_frame.actor_phase,
        actor_frame.body_present, actor_frame.body_pinned,
        actor_frame.body_service_count, actor_frame.frame, actor_frame.world_steps,
        actor_frame.source_path_segments, actor_frame.source_path_requested,
        actor_frame.source_floor,
        actor_frame.position[0], actor_frame.position[1], actor_frame.position[2],
        actor_frame.physics_position[0], actor_frame.physics_position[1],
        kPlayerPathMask);

    auto* scene_manager = device->getSceneManager();
    PrinceTextureContext prince_texture_context{prince_atlas};
    dh2::irrlicht_adapter::MutablePrinceMesh prince_mesh;
    if (!dh2::irrlicht_adapter::build_mutable_prince_mesh(
            prince, &prince_mesh, prince_error, resolve_prince_texture,
            &prince_texture_context)) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
            "Source Prince Irrlicht mesh build failed: %s", prince_error.c_str());
        dh2_viewer_scene_mesh_free(&source_mesh);
        dh2_nav_free(&navigation);
        dh2_world_free(&level);
        device->drop();
        return;
    }
    auto* player = scene_manager->addMeshSceneNode(prince_mesh.mesh);
    if (!player) {
        log(ANDROID_LOG_ERROR, "Irrlicht could not create the source Prince scene node");
        dh2_viewer_scene_mesh_free(&source_mesh);
        dh2_nav_free(&navigation);
        dh2_world_free(&level);
        device->drop();
        return;
    }
    // Source SceneBinding produces owner * helper * authored graph world
    // transforms in the CPU-skinned vertices. Keep Irrlicht at identity so the
    // spawn owner is not applied a second time.
    player->setPosition(irr::core::vector3df(0, 0, 0));
    player->setRotation(irr::core::vector3df(0, 0, 0));
    player->setScale(irr::core::vector3df(1, 1, 1));
    player->setAutomaticCulling(irr::scene::EAC_OFF);
    __android_log_print(ANDROID_LOG_INFO, kTag,
        "PRINCE: controllers=%u joints=%u vertices=%u triangles=%u parts=%u atlas mapped=%u unmapped diffuse=%u ignored AlphaMaps=%u; source_visual_binding=owner_helper_graph; Character=shared_source_coordinator; authored_bank_resources=%u registration_occurrences=%u state=%d sequence=%d clip=%d; SWAMP owner position development producer; Irrlicht node identity",
        prince.controller_count(), prince.joint_count(), prince.vertex_count(),
        prince.triangle_count(), static_cast<unsigned>(prince.parts().size()),
        prince_mesh.mapped_textures, prince_mesh.unmapped_diffuse_textures,
        prince_mesh.alpha_map_materials,
        prince_character.registered_resource_count(),
        prince_character.registration_occurrence_count(),
        prince_character.state_id(), prince_character.sequence_id(),
        prince_character.clip_id());
    auto* camera = scene_manager->addCameraSceneNode();
    if (camera) {
        camera->setNearValue(1.0f);
        camera->setFarValue(12000.0f);
        set_follow_camera(camera, player_position);
        scene_manager->setActiveCamera(camera);
    }

    auto* gui = device->getGUIEnvironment();
    gui->addStaticText(L"SWAMP module zero · source movement · Irrlicht OGL-ES r6038",
        irr::core::rect<irr::s32>(14, 10, 900, 46), false, false);
    auto* diagnostics = gui->addStaticText(L"Loading source diagnostics…",
        irr::core::rect<irr::s32>(14, 50, 1150, 170), false, false);
    if (diagnostics) diagnostics->setOverrideColor(irr::video::SColor(255, 233, 240, 246));
    auto* hint = gui->addStaticText(
        L"Touch and drag the lower-left pad to move on source X/Y; release to stop.",
        irr::core::rect<irr::s32>(14, static_cast<irr::s32>(screen.Height) - 54,
                                  1000, static_cast<irr::s32>(screen.Height) - 18),
        false, false);
    if (hint) hint->setOverrideColor(irr::video::SColor(255, 230, 236, 241));

    char initial_status[1536]{};
    std::snprintf(initial_status, sizeof(initial_status),
        "SOURCE XYZ  X %.2f  Y %.2f  Z %.2f  | player path mask 0x%X\n"
        "Shown scenery draws %u/%u; filtered source room-root=%u navigation-floors=%u exit-markers=%u; Material__11598 additive passes %u; exact GL_ONE/GL_ONE + GL_FUNC_ADD mapped %u.\n"
        "Samplers: Diffuse refs %u (%u draws mapped); AlphaMap %u refs (source selector AL %u, AT %u, selector unresolved %u), %u unresolved texture refs. Profile choice, active effect group/compiled ordinal, and original blend/depth remain unresolved. LightMap %u, Specular %u, other %u ignored. No-texture draws %u (%u visible default-material fallbacks).\n"
        "Prince: %u warrior skins, %u joints, %u vertices; authored bank %u resources/%u registrations; state %d flags 0x%X, sequence %d, clip %d.\n"
        "Actor step %u / NativeWorld steps %u; body pinned %u; floor %u. Limits: AI/scripts, other modules, environment contacts, walls and swept collision.",
        player_position.X, player_position.Y, player_position.Z, kPlayerPathMask,
        visible_draws, source_draws,
        room_render_counts.room_root_bounds, room_render_counts.navigation_floors,
        room_render_counts.exit_markers, additive_draws,
        additive_one_one_mapped,
        diffuse_refs, textured_draws, alpha_refs, alpha_map_al_draws,
        alpha_map_at_draws, alpha_map_selector_unresolved_draws,
        unresolved_alpha_refs,
        lightmap_refs, specular_refs, other_refs, no_texture_draws,
        visible_no_texture_draws, prince.controller_count(), prince.joint_count(),
        prince.vertex_count(), prince_character.registered_resource_count(),
        prince_character.registration_occurrence_count(),
        prince_character.state_id(), prince_character.state_flags(),
        prince_character.sequence_id(), prince_character.clip_id(),
        actor_frame.frame, actor_frame.world_steps, actor_frame.body_pinned,
        actor_frame.source_floor);
    if (diagnostics) diagnostics->setText(utf8_wide(initial_status).c_str());
    __android_log_print(ANDROID_LOG_INFO, kTag,
        "ENGINE: Irrlicht is confirmed as the engine family; this diagnostic embeds official upstream OGL-ES r6038. The exact customized DH2 Irrlicht fork/revision and game-specific layer remain under investigation. SWAMP Prince input runs through the shared source Character coordinator, actor_runtime and NativeWorld on developer-selected 20 ms logical ticks; owner movement follows source SceneBinding root motion. Script manager, multi-module level, environment physics, AI and combat remain unsupported.");
    __android_log_print(ANDROID_LOG_INFO, kTag,
        "RENDER-LIMITS: source room role filter omitted root-bounds=%u navigation-floors=%u exit-markers=%u; these floor meshes remain in navigation/physics inputs; Material__11598 additive passes=%u exact GL_ONE/GL_ONE + GL_FUNC_ADD mapped=%u with source LEQUAL/depth-write-off; SWAMP diffuse draws=%u; AlphaMap refs=%u source-selector-derived AL draws=%u AT draws=%u selector-unresolved=%u and %u unresolved texture refs; source mask uses decoded blue channel; Irrlicht AL blend is an approximation and AT is a 0.8 cutout mapping. Profile selection, chosen effect group/compiled ordinal, and original GL blend/depth state remain unresolved. Ignored LightMap=%u Specular=%u other=%u; visible no-texture defaults=%u. Prince: four source warrior skins, atlas mapped parts=%u, unmapped diffuse=%u, ignored source AlphaMap parts=%u; exact remaining source shader/effects not reconstructed.",
        room_render_counts.room_root_bounds,
        room_render_counts.navigation_floors,
        room_render_counts.exit_markers, additive_draws, additive_one_one_mapped,
        textured_draws, alpha_refs, alpha_map_al_draws, alpha_map_at_draws,
        alpha_map_selector_unresolved_draws,
        unresolved_alpha_refs,
        lightmap_refs, specular_refs, other_refs, visible_no_texture_draws,
        prince_mesh.mapped_textures, prince_mesh.unmapped_diffuse_textures,
        prince_mesh.alpha_map_materials);

    irr::video::IVideoDriver* driver = device->getVideoDriver();
    using Clock = std::chrono::steady_clock;
    auto previous = Clock::now();
    bool character_failure_logged = false;
    bool pose_changed = false;
    bool character_runtime_ok = true;
    bool first_source_character_frame_reported = false;
    bool movement_blocked = false;
    dh2::irrlicht_swamp::FixedStepAccumulator fixed_steps;
    auto last_report = previous;
    auto last_movement_log = previous - std::chrono::seconds(1);
    const char* last_logged_mode = "";
    while (device->run()) {
        if (!app->window || !device->isWindowActive()) {
            // Defensive fallback for lifecycle paths that do not deliver a
            // command event before pause/TERM_WINDOW. Irrlicht 6038 can still
            // report active while Android has detached its EGL surface.
            input.reset();
            previous = Clock::now();
            fixed_steps.reset();
            device->yield();
            continue;
        }
        const irr::core::dimension2d<irr::u32> window_size(
            static_cast<irr::u32>(std::max(0, ANativeWindow_getWidth(app->window))),
            static_cast<irr::u32>(std::max(0, ANativeWindow_getHeight(app->window))));
        if (window_size.Width && window_size.Height &&
            (window_size.Width != screen.Width || window_size.Height != screen.Height)) {
            device->getVideoDriver()->OnResize(window_size);
            screen = device->getVideoDriver()->getScreenSize();
            input.reset();
            input.set_screen(screen.Width, screen.Height);
            if (camera) camera->setAspectRatio(
                static_cast<float>(screen.Width) / screen.Height);
            if (hint) hint->setRelativePosition(irr::core::rect<irr::s32>(
                14, static_cast<irr::s32>(screen.Height) - 54,
                std::min<irr::s32>(1000, static_cast<irr::s32>(screen.Width) - 14),
                static_cast<irr::s32>(screen.Height) - 18));
            previous = Clock::now();
            fixed_steps.reset();
            __android_log_print(ANDROID_LOG_INFO, kTag,
                "SURFACE_RESIZED: window=%ux%u driver=%ux%u; touch input reset, source actor owners retained",
                window_size.Width, window_size.Height, screen.Width, screen.Height);
        }
        const auto now = Clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - previous);
        previous = now;
        const auto steps = fixed_steps.advance(static_cast<std::uint64_t>(elapsed.count()));
        movement_blocked = false;
        pose_changed = false;
        for (std::uint32_t step = 0; step < steps; ++step) {
            if (!character_runtime_ok) {
                break;
            }
            const bool accepted = std::fabs(input.x()) > 0.01f ||
                                  std::fabs(input.y()) > 0.01f;
            const dh2::irrlicht_game::SwampActorFrameInput frame_input{
                20, input.x(), input.y(), accepted};
            if (!actor_session.frame(frame_input, actor_frame, prince_error)) {
                character_runtime_ok = false;
                if (!character_failure_logged) {
                    __android_log_print(ANDROID_LOG_ERROR, kTag,
                        "SWAMP source Actor session stopped at actor_step=%u world_steps=%u: %s",
                        actor_frame.frame, actor_frame.world_steps,
                        prince_error.c_str());
                    character_failure_logged = true;
                }
                break;
            }
            player_position.set(actor_frame.position[0], actor_frame.position[1],
                                actor_frame.position[2]);
            movement_blocked = accepted &&
                (!actor_frame.path_boundary_checked ||
                 !actor_frame.path_direction_valid);
            pose_changed = true;
        }
        if (pose_changed &&
            !dh2::irrlicht_adapter::update_mutable_prince_mesh(
                prince, prince_mesh, prince_error)) {
            character_runtime_ok = false;
            if (!character_failure_logged) {
                __android_log_print(ANDROID_LOG_ERROR, kTag,
                    "Source Character Irrlicht vertex update stopped: %s",
                    prince_error.c_str());
                character_failure_logged = true;
            }
        }
        set_follow_camera(camera, player_position);

        if (diagnostics && now - last_report >= std::chrono::milliseconds(100)) {
            char status[1280]{};
            const char* mode = "IDLE";
            if (!character_runtime_ok)
                mode = "SOURCE TICKS STOPPED · last completed diagnostics retained";
            else if (movement_blocked)
                mode = "BLOCKED · source path/boundary check · position held";
            else if (std::fabs(input.x()) > 0.01f || std::fabs(input.y()) > 0.01f)
                mode = "MOVE";
            std::snprintf(status, sizeof(status),
                "%s  |  SOURCE XYZ  X %.2f  Y %.2f  Z %.2f  | player mask 0x%X floor %u\n"
                "Prince source state %d flags 0x%X; sequence %d (%s), clip %d; four warrior skins (%u joints, %u vertices); atlas parts %u; ignored Prince AlphaMaps %u.\n"
                "Source actor step %u / NativeWorld steps %u; phase %u; body present %u pinned %u; source body service calls %u; path boundary %u direction %u path_segments %u requested %u; physics XY %.4f, %.4f.\n"
                "SWAMP draws %u/%u; source-room-filter root=%u navigation-floor=%u exit=%u; additive %u GL_ONE/GL_ONE + ADD mapped %u; diffuse %u/%u; AlphaMap %u selectors AL=%u AT=%u unresolved=%u (unresolved texture refs %u); profile/effect-group/compiled ordinal and source blend/depth unresolved; ignored LightMap %u, Specular %u, other %u; no-texture %u (%u visible).",
                mode, player_position.X, player_position.Y, player_position.Z,
                kPlayerPathMask, actor_frame.source_floor,
                prince_character.state_id(), prince_character.state_flags(),
                prince_character.sequence_id(),
                prince_character.sequence_id() == prince_character.walk_sequence_id()
                    ? "WALK" : prince_character.sequence_id() == prince_character.idle_sequence_id()
                        ? "IDLE" : "OTHER",
                prince_character.clip_id(),
                prince.joint_count(), prince.vertex_count(), prince_mesh.mapped_textures,
                prince_mesh.alpha_map_materials, actor_frame.frame,
                actor_frame.world_steps, actor_frame.actor_phase,
                actor_frame.body_present, actor_frame.body_pinned,
                actor_frame.body_service_count,
                actor_frame.path_boundary_checked, actor_frame.path_direction_valid,
                actor_frame.source_path_segments, actor_frame.source_path_requested,
                actor_frame.physics_position[0], actor_frame.physics_position[1],
                visible_draws, source_draws,
                room_render_counts.room_root_bounds,
                room_render_counts.navigation_floors,
                room_render_counts.exit_markers, additive_draws,
                additive_one_one_mapped,
                diffuse_refs, textured_draws, alpha_refs, alpha_map_al_draws,
                alpha_map_at_draws, alpha_map_selector_unresolved_draws,
                unresolved_alpha_refs, lightmap_refs, specular_refs, other_refs, no_texture_draws,
                visible_no_texture_draws);
            diagnostics->setText(utf8_wide(status).c_str());
            last_report = now;
        }

        // Keep source-coordinate motion verifiable without OCR: emit a
        // throttled state/position line on transitions and while held input
        // changes. This is a diagnostic channel, not gameplay telemetry.
        const char* movement_mode = "IDLE";
        if (!character_runtime_ok)
            movement_mode = "ACTOR_STOPPED";
        else if (movement_blocked)
            movement_mode = "BLOCKED";
        else if (std::fabs(input.x()) > 0.01f || std::fabs(input.y()) > 0.01f)
            movement_mode = "MOVE";
        const auto log_interval = std::strcmp(movement_mode, "IDLE") == 0
            ? std::chrono::milliseconds(1000)
            : std::chrono::milliseconds(200);
        if (std::strcmp(movement_mode, last_logged_mode) != 0 ||
            now - last_movement_log >= log_interval) {
            __android_log_print(ANDROID_LOG_INFO, kTag,
                "MOVE state=%s animation=%s source_state=%d sequence=%d clip=%d x=%.3f y=%.3f z=%.3f stick_x=%.3f stick_y=%.3f path_mask=0x%X source_state_flags=0x%X actor_step=%u world_steps=%u actor_phase=%u body_present=%u body_pinned=%u body_service_count=%u floor=%u path_boundary_checked=%u path_direction_valid=%u source_path_segments=%u source_path_requested=%u physics_x=%.4f physics_y=%.4f desired_x=%.3f desired_y=%.3f validated_x=%.3f validated_y=%.3f owner_delta_x=%.3f owner_delta_y=%.3f animation_root_delta_x=%.3f animation_root_delta_y=%.3f desired_rotation=%.3f current_rotation=%.3f body_radius=%.3f body_radius_physics=%.4f",
                movement_mode,
                prince_character.sequence_id() == prince_character.walk_sequence_id()
                    ? "WALK" : prince_character.sequence_id() == prince_character.idle_sequence_id()
                        ? "IDLE" : "OTHER",
                prince_character.state_id(), prince_character.sequence_id(),
                prince_character.clip_id(),
                player_position.X, player_position.Y, player_position.Z,
                input.x(), input.y(), kPlayerPathMask,
                prince_character.state_flags(), actor_frame.frame,
                actor_frame.world_steps, actor_frame.actor_phase,
                actor_frame.body_present, actor_frame.body_pinned,
                actor_frame.body_service_count, actor_frame.source_floor,
                actor_frame.path_boundary_checked,
                actor_frame.path_direction_valid,
                actor_frame.source_path_segments,
                actor_frame.source_path_requested,
                actor_frame.physics_position[0], actor_frame.physics_position[1],
                actor_frame.desired_heading[0], actor_frame.desired_heading[1],
                actor_frame.validated_heading[0], actor_frame.validated_heading[1],
                actor_frame.source_owner_delta[0], actor_frame.source_owner_delta[1],
                actor_frame.animation_root_delta[0], actor_frame.animation_root_delta[1],
                actor_frame.desired_rotation, actor_frame.current_rotation,
                actor_frame.body_radius_source, actor_frame.body_radius_physics);
            last_logged_mode = movement_mode;
            last_movement_log = now;
        }

        driver->beginScene(true, true, irr::video::SColor(255, 18, 27, 37));
        scene_manager->drawAll();
        draw_touch_pad(driver, input);
        gui->drawAll();
        const bool frame_presented = driver->endScene();
        if (frame_presented && !first_source_character_frame_reported) {
            __android_log_print(ANDROID_LOG_INFO, kTag,
                "FIRST_SOURCE_CHARACTER_FRAME: source_state=%d state_flags=0x%X sequence=%d clip=%d authored_bank_resources=%u registration_occurrences=%u actor_step=%u world_steps=%u actor_phase=%u body_pinned=%u floor=%u source_path_segments=%u source_path_requested=%u source_xyz=(%.3f,%.3f,%.3f); renderer swap succeeded",
                prince_character.state_id(), prince_character.state_flags(),
                prince_character.sequence_id(),
                prince_character.clip_id(),
                prince_character.registered_resource_count(),
                prince_character.registration_occurrence_count(),
                actor_frame.frame, actor_frame.world_steps,
                actor_frame.actor_phase, actor_frame.body_pinned,
                actor_frame.source_floor, actor_frame.source_path_segments,
                actor_frame.source_path_requested, actor_frame.position[0],
                actor_frame.position[1], actor_frame.position[2]);
            first_source_character_frame_reported = true;
        }
        device->yield();
    }

    actor_session.shutdown();
    dh2_viewer_scene_mesh_free(&source_mesh);
    dh2_nav_free(&navigation);
    dh2_world_free(&level);
    device->drop();
}
