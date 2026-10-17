#include "../game/scene_mesh_adapter.hpp"
#include "../../texture-assets/texture.hpp"

#include <irrlicht.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include <cstdint>
#include <cstring>
#include <vector>

namespace {
constexpr const char* kTag = "DH2IrrlichtCacheScene";
constexpr const char* kBresAsset = "dh2/void_maze.bdae";
constexpr const char* kTextureAsset = "dh2/env_voidmaze.tga";
irr::video::ITexture* debug_texture = nullptr;

void log_message(int priority, const char* message) {
    __android_log_print(priority, kTag, "%s", message);
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

struct TextureContext {
    irr::video::ITexture* texture;
    std::uint32_t assigned_count = 0;
};

irr::video::ITexture* resolve_texture(
    const dh2::viewer::SceneDrawDescriptor&,
    const dh2::viewer::SceneTextureReference* references,
    std::uint32_t count, void* user_data) {
    auto* context = static_cast<TextureContext*>(user_data);
    if (!context || !context->texture || !references) return nullptr;
    for (std::uint32_t i = 0; i < count; ++i) {
        const char* source = references[i].source_path;
        if (!source[0]) continue;
        const char* base = source;
        for (const char* at = source; *at; ++at)
            if (*at == '/' || *at == '\\') base = at + 1;
        if (std::strcmp(base, "env_voidmaze.tga") != 0) continue;
        ++context->assigned_count;
        return context->texture;
    }
    return nullptr;
}

irr::video::ITexture* decode_diffuse(irr::IrrlichtDevice* device,
                                     const std::vector<std::uint8_t>& bytes,
                                     std::uint32_t* width_out,
                                     std::uint32_t* height_out) {
    if (!device || bytes.empty()) return nullptr;
    dh2::textures::TextureView view{};
    const auto opened = dh2_texture_open(&view, bytes.data(), bytes.size());
    if (opened != dh2::textures::Error::ok || !view.width || !view.height ||
        view.width > 4096 || view.height > 4096 ||
        std::uint64_t(view.width) * view.height > 16U * 1024U * 1024U ||
        (view.format != dh2::textures::Format::pvrtc_2bpp &&
         view.format != dh2::textures::Format::pvrtc_4bpp)) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
                            "BTEX texture open rejected source status=%u format=%u size=%ux%u",
                            static_cast<unsigned>(opened), static_cast<unsigned>(view.format),
                            view.width, view.height);
        return nullptr;
    }

    const std::size_t image_bytes = std::size_t(view.width) * view.height * 4U;
    std::vector<std::uint8_t> rgba(image_bytes), bgra(image_bytes);
    const auto decoded = dh2_texture_decode_rgba8(
        rgba.data(), rgba.size(), std::size_t(view.width) * 4U,
        bytes.data(), bytes.size());
    if (decoded != dh2::textures::Error::ok) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
                            "BTEX PVRTC decode failed status=%u",
                            static_cast<unsigned>(decoded));
        return nullptr;
    }
    // Irrlicht 1.9 ECF_A8R8G8B8 uses BGRA byte order on Android's little-endian
    // ABIs. The checked source decoder emits RGBA bytes.
    for (std::size_t i = 0; i < image_bytes; i += 4) {
        bgra[i] = rgba[i + 2];
        bgra[i + 1] = rgba[i + 1];
        bgra[i + 2] = rgba[i];
        bgra[i + 3] = rgba[i + 3];
    }
    auto* image = device->getVideoDriver()->createImageFromData(
        irr::video::ECF_A8R8G8B8,
        irr::core::dimension2d<irr::u32>(view.width, view.height),
        bgra.data(), false);
    if (!image) {
        log_message(ANDROID_LOG_ERROR, "Irrlicht could not create decoded BTEX image");
        return nullptr;
    }
    auto* texture = device->getVideoDriver()->addTexture("dh2-env-voidmaze", image);
    image->drop();
    if (texture) {
        if (width_out) *width_out = view.width;
        if (height_out) *height_out = view.height;
    }
    return texture;
}

bool install_scene(irr::IrrlichtDevice* device,
                   const std::vector<std::uint8_t>& bres_bytes,
                   const std::vector<std::uint8_t>& texture_bytes) {
    dh2::resources::BresView bres{};
    const auto bres_error = dh2_bres_open(&bres, bres_bytes.data(), bres_bytes.size());
    if (bres_error != dh2::resources::BresError::ok) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
                            "dh2_bres_open rejected sample status=%u",
                            static_cast<unsigned>(bres_error));
        return false;
    }

    dh2::viewer::SceneMesh scene_mesh{};
    const auto scene_error = dh2_viewer_scene_mesh(&scene_mesh, &bres);
    if (scene_error != dh2::viewer::SceneMeshError::ok) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
                            "SceneMesh assembly failed status=%u",
                            static_cast<unsigned>(scene_error));
        return false;
    }

    std::uint32_t texture_width = 0, texture_height = 0;
    auto* diffuse_texture = decode_diffuse(device, texture_bytes,
                                           &texture_width, &texture_height);
    debug_texture = diffuse_texture;
    TextureContext texture_context{diffuse_texture};
    dh2::irrlicht_adapter::MeshBuildStatus adapter_status{};
    std::uint32_t buffer_count = 0;
    auto* mesh = dh2::irrlicht_adapter::build_mesh(
        scene_mesh, &adapter_status, resolve_texture, &texture_context, &buffer_count);
    const auto draws = scene_mesh.draw_commands;
    const auto vertices = scene_mesh.vertex_count;
    const auto indices = scene_mesh.index_count;
    char diffuse[sizeof(scene_mesh.first_diffuse_texture)]{};
    std::memcpy(diffuse, scene_mesh.first_diffuse_texture, sizeof(diffuse));
    float uv_min[2] = {1.0e30f, 1.0e30f}, uv_max[2] = {-1.0e30f, -1.0e30f};
    for (std::uint32_t i = 0; i < scene_mesh.vertex_count; ++i) {
        for (std::uint32_t axis = 0; axis < 2; ++axis) {
            const float uv = scene_mesh.vertices[std::size_t(i) * 5 + 3 + axis];
            if (uv < uv_min[axis]) uv_min[axis] = uv;
            if (uv > uv_max[axis]) uv_max[axis] = uv;
        }
    }
    std::uint32_t textured_draws = 0, textured_vertices = 0;
    float textured_min[3] = {1.0e30f, 1.0e30f, 1.0e30f};
    float textured_max[3] = {-1.0e30f, -1.0e30f, -1.0e30f};
    double textured_sum[3] = {0.0, 0.0, 0.0};
    for (std::uint32_t draw_index = 0; draw_index < scene_mesh.draw_commands; ++draw_index) {
        const auto& draw = scene_mesh.draws[draw_index];
        bool matches_texture = false;
        for (std::uint32_t ref_index = 0; ref_index < draw.texture_count; ++ref_index) {
            const char* source = scene_mesh.texture_references[draw.first_texture + ref_index].source_path;
            const char* base = source;
            for (const char* at = source; *at; ++at)
                if (*at == '/' || *at == '\\') base = at + 1;
            matches_texture |= std::strcmp(base, "env_voidmaze.tga") == 0;
        }
        if (!draw.visible || !matches_texture) continue;
        ++textured_draws;
        for (std::uint32_t vertex_index = 0; vertex_index < draw.vertex_count; ++vertex_index) {
            const float* vertex = scene_mesh.vertices +
                static_cast<std::size_t>(draw.first_vertex + vertex_index) * 5;
            ++textured_vertices;
            for (std::uint32_t axis = 0; axis < 3; ++axis) {
                if (vertex[axis] < textured_min[axis]) textured_min[axis] = vertex[axis];
                if (vertex[axis] > textured_max[axis]) textured_max[axis] = vertex[axis];
                textured_sum[axis] += vertex[axis];
            }
        }
    }
    std::uint32_t postbuild_textured_buffers = 0;
    if (mesh) {
        for (irr::u32 i = 0; i < mesh->getMeshBufferCount(); ++i) {
            const auto* buffer = mesh->getMeshBuffer(i);
            if (buffer && buffer->getMaterial().getTexture(0)) ++postbuild_textured_buffers;
        }
    }
    std::vector<dh2::viewer::SceneDrawDescriptor> textured_draw_descriptors(
        scene_mesh.draws, scene_mesh.draws + scene_mesh.draw_commands);
    std::uint32_t texture_only_draw_count = 0;
    for (auto& draw : textured_draw_descriptors) {
        bool matches_texture = false;
        for (std::uint32_t ref_index = 0; ref_index < draw.texture_count; ++ref_index) {
            const char* source = scene_mesh.texture_references[draw.first_texture + ref_index].source_path;
            const char* base = source;
            for (const char* at = source; *at; ++at)
                if (*at == '/' || *at == '\\') base = at + 1;
            matches_texture |= std::strcmp(base, "env_voidmaze.tga") == 0;
        }
        if (matches_texture && draw.visible) ++texture_only_draw_count;
        else draw.visible = 0;
    }
    auto texture_only_scene = scene_mesh;
    texture_only_scene.draws = textured_draw_descriptors.data();
    dh2::irrlicht_adapter::MeshBuildStatus texture_only_status{};
    std::uint32_t texture_only_buffer_count = 0;
    TextureContext texture_only_context{diffuse_texture};
    auto* texture_only_mesh = dh2::irrlicht_adapter::build_mesh(
        texture_only_scene, &texture_only_status, resolve_texture, &texture_only_context,
        &texture_only_buffer_count);
    const bool adapter_ok = mesh &&
        adapter_status == dh2::irrlicht_adapter::MeshBuildStatus::ok &&
        buffer_count != 0;
    dh2_viewer_scene_mesh_free(&scene_mesh);
    if (!adapter_ok) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
                            "Irrlicht adapter failed status=%u buffers=%u",
                            static_cast<unsigned>(adapter_status), buffer_count);
        if (mesh) mesh->drop();
        return false;
    }

    auto* manager = device->getSceneManager();
    auto* node = manager->addMeshSceneNode(mesh);
    mesh->drop();
    if (!node) {
        log_message(ANDROID_LOG_ERROR, "Irrlicht could not create scene node");
        return false;
    }
    node->setAutomaticCulling(irr::scene::EAC_OFF);
    const auto bounds = node->getBoundingBox();
    const irr::core::vector3df center(
        (bounds.MinEdge.X + bounds.MaxEdge.X) * 0.5f,
        (bounds.MinEdge.Y + bounds.MaxEdge.Y) * 0.5f,
        (bounds.MinEdge.Z + bounds.MaxEdge.Z) * 0.5f);
    node->setPosition(-center);
    for (irr::u32 i = 0; i < node->getMaterialCount(); ++i) {
        auto& material = node->getMaterial(i);
        material.setFlag(irr::video::EMF_LIGHTING, false);
        material.setFlag(irr::video::EMF_BACK_FACE_CULLING, false);
        material.DiffuseColor = irr::video::SColor(255, 255, 255, 255);
        material.AmbientColor = irr::video::SColor(255, 255, 255, 255);
    }
    irr::core::vector3df camera_target(0.0f, 0.0f, 0.0f);
    if (textured_vertices) {
        camera_target.set(
            static_cast<irr::f32>(textured_sum[0] / textured_vertices) - center.X,
            static_cast<irr::f32>(textured_sum[1] / textured_vertices) - center.Y,
            static_cast<irr::f32>(textured_sum[2] / textured_vertices) - center.Z);
    }
    node->setVisible(false);
    if (texture_only_mesh &&
        texture_only_status == dh2::irrlicht_adapter::MeshBuildStatus::ok &&
        texture_only_buffer_count == texture_only_draw_count) {
        auto* texture_node = manager->addMeshSceneNode(texture_only_mesh);
        texture_only_mesh->drop();
        if (texture_node) {
            texture_node->setAutomaticCulling(irr::scene::EAC_OFF);
            texture_node->setPosition(-center);
            for (irr::u32 i = 0; i < texture_node->getMaterialCount(); ++i) {
                auto& material = texture_node->getMaterial(i);
                material.setFlag(irr::video::EMF_LIGHTING, false);
                material.setFlag(irr::video::EMF_BACK_FACE_CULLING, false);
                material.DiffuseColor = irr::video::SColor(255, 255, 255, 255);
                material.AmbientColor = irr::video::SColor(255, 255, 255, 255);
            }
        }
    } else if (texture_only_mesh) {
        texture_only_mesh->drop();
    }

    auto* camera = manager->addCameraSceneNode(
        nullptr, camera_target + irr::core::vector3df(0.15f, -0.2f, 0.8f),
        camera_target);
    if (camera) {
        camera->setUpVector(irr::core::vector3df(0.0f, 1.0f, 0.0f));
        camera->setNearValue(0.05f);
        camera->setFarValue(100.0f);
        manager->setActiveCamera(camera);
    }
    device->getGUIEnvironment()->addStaticText(
        L"void_maze BRES • Irrlicht r6038",
        irr::core::rect<irr::s32>(18, 14, 760, 58), false, false);

    __android_log_print(ANDROID_LOG_INFO, kTag,
        "PASS: actual cache BRES assembled into Irrlicht: draws=%u vertices=%u indices=%u buffers=%u first_diffuse=%s texture_size=%ux%u texture_assigned_buffers=%u postbuild_textured_buffers=%u uv=[%.3f %.3f]-[%.3f %.3f] textured_draws=%u textured_vertices=%u textured_bounds=[%.3f %.3f %.3f]-[%.3f %.3f %.3f] textured_centroid=[%.3f %.3f %.3f] visible_texture_only=%u texture_only_assignments=%u camera_target=[%.3f %.3f %.3f] camera=%d bounds=[%.3f %.3f %.3f]-[%.3f %.3f %.3f]",
        draws, vertices, indices, buffer_count, diffuse, texture_width, texture_height,
        texture_context.assigned_count, postbuild_textured_buffers,
        uv_min[0], uv_min[1], uv_max[0], uv_max[1], textured_draws, textured_vertices,
        textured_min[0], textured_min[1], textured_min[2],
        textured_max[0], textured_max[1], textured_max[2],
        textured_vertices ? static_cast<float>(textured_sum[0] / textured_vertices) : 0.0f,
        textured_vertices ? static_cast<float>(textured_sum[1] / textured_vertices) : 0.0f,
        textured_vertices ? static_cast<float>(textured_sum[2] / textured_vertices) : 0.0f,
        texture_only_buffer_count,
        texture_only_context.assigned_count,
        camera_target.X, camera_target.Y, camera_target.Z,
        camera != nullptr, bounds.MinEdge.X, bounds.MinEdge.Y, bounds.MinEdge.Z,
        bounds.MaxEdge.X, bounds.MaxEdge.Y, bounds.MaxEdge.Z);
    return true;
}
}

void android_main(android_app* app) {
    if (!app || !app->activity) return;

    std::vector<std::uint8_t> bres_bytes;
    std::vector<std::uint8_t> texture_bytes;
    if (!read_asset(app->activity->assetManager, kBresAsset, &bres_bytes) ||
        !read_asset(app->activity->assetManager, kTextureAsset, &texture_bytes)) {
        log_message(ANDROID_LOG_ERROR, "Could not read packaged cache sample assets");
        return;
    }

    irr::SIrrlichtCreationParameters params;
    params.DriverType = irr::video::EDT_OGLES2;
    params.WindowSize = irr::core::dimension2d<irr::u32>(0, 0);
    params.PrivateData = app;
    params.Bits = 24;
    params.ZBufferBits = 16;
    params.AntiAlias = 0;
    params.LoggingLevel = irr::ELL_INFORMATION;
    auto* device = irr::createDeviceEx(params);
    if (!device) {
        log_message(ANDROID_LOG_ERROR, "createDeviceEx failed");
        return;
    }

    const bool loaded = install_scene(device, bres_bytes, texture_bytes);
    if (!loaded) {
        device->drop();
        return;
    }

    auto* driver = device->getVideoDriver();
    while (device->run()) {
        if (device->isWindowActive()) {
            driver->beginScene(true, true, irr::video::SColor(255, 17, 26, 40));
            device->getSceneManager()->drawAll();
            device->getGUIEnvironment()->drawAll();
            if (debug_texture) {
                driver->draw2DImage(debug_texture,
                    irr::core::rect<irr::s32>(20, 80, 276, 336),
                    irr::core::rect<irr::s32>(0, 0, 256, 256), nullptr, nullptr, false);
            }
            driver->endScene();
        }
        device->yield();
    }
    device->drop();
}
