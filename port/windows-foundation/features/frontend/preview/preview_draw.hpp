#pragma once
#include "class_preview_scene.hpp"
namespace dh::foundation::frontend {
// Call after the existing host material binder assigns textures. Renderer
// beginFrame/endFrame and overlay submission belong to the native frame owner.
inline void draw_creation_preview(Renderer& renderer,const ClassPreviewScene& scene,
    const CreationPreview& preview,RenderPass pass=RenderPass::All) {
    if(!scene.loaded()||!preview.loaded())return;
    renderer.draw(scene.backdrop().mesh,identity(),pass);
    for(unsigned i=0;i<3;++i) {
        const auto& actor=preview.actors()[i];
        // Show copies dummy position, rotation AND scale after body load,
        // overriding Character's property-derived visual scale.
        const auto& placement=scene.anchors()[i];
        for(const auto& mesh:actor.body.meshes())renderer.draw(mesh,placement,pass);
        for(const auto& attachment:actor.equipment.attachments()) {
            const auto world=dh2::scene::multiply(placement,attachment.socket_world);
            for(const auto& mesh:attachment.visual.meshes())renderer.draw(mesh,world,pass);
        }
    }
}
}
