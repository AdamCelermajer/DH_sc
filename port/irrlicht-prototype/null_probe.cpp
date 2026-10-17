// Isolated Android ABI smoke entry point for the official Irrlicht 1.8.5 Null driver.
#include "CNullDriver.h"
#include "irrlicht.h"

extern "C" __attribute__((visibility("default"))) int irrlicht_null_probe() {
    irr::video::CNullDriver driver(nullptr, irr::core::dimension2d<irr::u32>(16, 16));
    if (!driver.beginScene(false, false)) return 0;
    irr::scene::SMeshBuffer mesh;
    mesh.Vertices.push_back(irr::video::S3DVertex(
        0.f, 0.f, 0.f, 0.f, 0.f, 1.f, irr::video::SColor(255, 255, 255, 255), 0.f, 0.f));
    mesh.Vertices.push_back(irr::video::S3DVertex(
        1.f, 0.f, 0.f, 0.f, 0.f, 1.f, irr::video::SColor(255, 255, 255, 255), 1.f, 0.f));
    mesh.Vertices.push_back(irr::video::S3DVertex(
        0.f, 1.f, 0.f, 0.f, 0.f, 1.f, irr::video::SColor(255, 255, 255, 255), 0.f, 1.f));
    mesh.Indices.push_back(0);
    mesh.Indices.push_back(1);
    mesh.Indices.push_back(2);
    mesh.recalculateBoundingBox();
    mesh.Material.Lighting = false;
    driver.setMaterial(mesh.Material);
    driver.drawMeshBuffer(&mesh);
    if (!driver.endScene()) return 0;
    return driver.getPrimitiveCountDrawn() == 1 ? 1 : 0;
}
