#include "scene_mesh_adapter.hpp"

#include <irrlicht.h>
#include <android_native_app_glue.h>
#include <android/log.h>

#include <cstdint>
#include <cstring>

namespace {
constexpr const char* kTag = "DH2IrrlichtAdapterSmoke";

struct SyntheticScene {
    // Four independent triangular faces form a small, deterministic pyramid.
    // Every triangle owns three vertices, matching the adapter's per-draw
    // vertex/index span contract.
    float vertices[12 * 5]{};
    std::uint16_t indices[12]{};
    dh2::viewer::SceneDrawDescriptor draws[4]{};
    dh2::viewer::SceneMesh mesh{};

    SyntheticScene() {
        constexpr float points[4][3][3] = {
            {{-1.0f, -0.8f, -0.8f}, {1.0f, -0.8f, -0.8f}, {0.0f, 1.2f, 0.0f}},
            {{1.0f, -0.8f, -0.8f}, {1.0f, -0.8f, 0.8f}, {0.0f, 1.2f, 0.0f}},
            {{1.0f, -0.8f, 0.8f}, {-1.0f, -0.8f, 0.8f}, {0.0f, 1.2f, 0.0f}},
            {{-1.0f, -0.8f, 0.8f}, {-1.0f, -0.8f, -0.8f}, {0.0f, 1.2f, 0.0f}},
        };
        for (std::uint32_t face = 0; face < 4; ++face) {
            auto& draw = draws[face];
            draw.first_vertex = face * 3;
            draw.vertex_count = 3;
            draw.first_index = face * 3;
            draw.index_count = 3;
            draw.visible = 1;
            draw.geometry_index = static_cast<std::int32_t>(face);
            draw.material_index = static_cast<std::int32_t>(face);
            for (std::uint32_t vertex = 0; vertex < 3; ++vertex) {
                const std::uint32_t at = face * 3 + vertex;
                vertices[at * 5 + 0] = points[face][vertex][0];
                vertices[at * 5 + 1] = points[face][vertex][1];
                vertices[at * 5 + 2] = points[face][vertex][2];
                vertices[at * 5 + 3] = vertex == 1 ? 1.0f : 0.0f;
                vertices[at * 5 + 4] = vertex == 2 ? 1.0f : 0.0f;
                indices[at] = static_cast<std::uint16_t>(at);
            }
        }
        mesh.vertices = vertices;
        mesh.indices = indices;
        mesh.vertex_count = mesh.vertex_capacity = 12;
        mesh.index_count = mesh.index_capacity = 12;
        mesh.draws = draws;
        mesh.draw_commands = mesh.draw_capacity = 4;
    }
};

void log_result(const char* message) {
    __android_log_print(ANDROID_LOG_INFO, kTag, "%s", message);
}
}

void android_main(android_app* app) {
    if (!app) return;

    irr::SIrrlichtCreationParameters params;
    params.DriverType = irr::video::EDT_OGLES2;
    params.WindowSize = irr::core::dimension2d<irr::u32>(0, 0);
    params.PrivateData = app;
    params.Bits = 24;
    params.ZBufferBits = 16;
    params.AntiAlias = 0;
    params.LoggingLevel = irr::ELL_INFORMATION;
    irr::IrrlichtDevice* device = irr::createDeviceEx(params);
    if (!device) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
                            "createDeviceEx failed; no Irrlicht render device");
        return;
    }

    SyntheticScene synthetic;
    dh2::irrlicht_adapter::MeshBuildStatus status{};
    std::uint32_t buffer_count = 0;
    irr::scene::IMesh* mesh = dh2::irrlicht_adapter::build_mesh(
        synthetic.mesh, &status, nullptr, nullptr, &buffer_count);
    if (!mesh || status != dh2::irrlicht_adapter::MeshBuildStatus::ok ||
        buffer_count != 4) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
                            "SceneMesh adapter failed status=%u buffers=%u",
                            static_cast<unsigned>(status), buffer_count);
        if (mesh) mesh->drop();
        device->drop();
        return;
    }

    auto* scene = device->getSceneManager();
    auto* node = scene->addMeshSceneNode(mesh);
    mesh->drop();
    if (!node) {
        __android_log_print(ANDROID_LOG_ERROR, kTag,
                            "Irrlicht could not create a mesh scene node");
        device->drop();
        return;
    }
    const irr::video::SColor colors[4] = {
        irr::video::SColor(255, 235, 78, 74),
        irr::video::SColor(255, 72, 191, 132),
        irr::video::SColor(255, 70, 135, 232),
        irr::video::SColor(255, 235, 190, 72),
    };
    for (irr::u32 i = 0; i < node->getMaterialCount(); ++i) {
        node->getMaterial(i).DiffuseColor = colors[i % 4];
        node->getMaterial(i).AmbientColor = colors[i % 4];
        node->getMaterial(i).EmissiveColor = colors[i % 4];
        node->getMaterial(i).setFlag(irr::video::EMF_LIGHTING, false);
        node->getMaterial(i).setFlag(irr::video::EMF_BACK_FACE_CULLING, false);
    }
    node->setAutomaticCulling(irr::scene::EAC_OFF);
    node->setPosition(irr::core::vector3df(1.8f, 0.0f, 0.0f));

    // Control object: if this built-in scene mesh appears while
    // the adapter pyramid does not, the failure is isolated to adapter mesh
    // content/buffers instead of camera or scene-manager rendering.
    auto* control_cube = scene->addCubeSceneNode(
        1.5f, nullptr, -1, irr::core::vector3df(-2.0f, 0.0f, 0.0f));
    if (control_cube) {
        control_cube->setAutomaticCulling(irr::scene::EAC_OFF);
        for (irr::u32 i = 0; i < control_cube->getMaterialCount(); ++i) {
            control_cube->getMaterial(i).DiffuseColor = irr::video::SColor(255, 220, 40, 220);
            control_cube->getMaterial(i).EmissiveColor = irr::video::SColor(255, 220, 40, 220);
            control_cube->getMaterial(i).setFlag(irr::video::EMF_LIGHTING, false);
        }
    }
    auto* camera = scene->addCameraSceneNode(nullptr,
        irr::core::vector3df(0.0f, 0.0f, -8.0f),
        irr::core::vector3df(0.0f, 0.0f, 0.0f));
    if (camera) {
        camera->setNearValue(0.1f);
        camera->setFarValue(100.0f);
        scene->setActiveCamera(camera);
    }
    device->getGUIEnvironment()->addStaticText(
        L"SceneMesh adapter • synthetic Irrlicht mesh",
        irr::core::rect<irr::s32>(20, 16, 740, 58), false, false);
    const auto& bounds = node->getBoundingBox();
    __android_log_print(ANDROID_LOG_INFO, kTag,
        "ADAPTER scene buffers=%u bounds=[%.2f %.2f %.2f]-[%.2f %.2f %.2f] visible=%d; control_cube=%d camera=%d",
        buffer_count, bounds.MinEdge.X, bounds.MinEdge.Y, bounds.MinEdge.Z,
        bounds.MaxEdge.X, bounds.MaxEdge.Y, bounds.MaxEdge.Z,
        node->isVisible(), control_cube != nullptr, camera != nullptr);
    log_result("PASS: SceneMesh adapter produced four buffers; synthetic pyramid added to the Irrlicht scene");

    irr::video::IVideoDriver* driver = device->getVideoDriver();
    while (device->run()) {
        if (device->isWindowActive()) {
            driver->beginScene(true, true, irr::video::SColor(255, 17, 26, 40));
            scene->drawAll();
            device->getGUIEnvironment()->drawAll();
            driver->endScene();
        }
        device->yield();
    }
    device->drop();
}
