// Slice 31 reconstruction (see include/glitch-scene-collada-gui/recon/slice_31.hpp).
// Each function keeps the original address and mangled symbol in its comment.
// Attribute calls go through the IAttributes stand-in; the slot offsets are
// listed in the header.

#include "glitch-scene-collada-gui/recon/slice_31.hpp"

#include "glitch-video/material.hpp"
#include "glitch-video/texture.hpp"

namespace glitch::collada::recon_31 {

// 0x35a9d8 _ZNK6glitch7collada21CSkinnedMeshSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Thunk: forwards to the ISceneNode body.
void CSkinnedMeshSceneNode_serializeAttributes(const CSkinnedMeshSceneNode& self, IAttributes* attributes,
                                               SAttributeReadWriteOptions* options) {
    ISceneNode_serializeAttributes(&self, attributes, options);
}

// 0x35a9ec _ZN6glitch7collada21CSkinnedMeshSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Thunk: forwards to the ISceneNode body.
void CSkinnedMeshSceneNode_deserializeAttributes(CSkinnedMeshSceneNode& self, IAttributes* attributes,
                                                 SAttributeReadWriteOptions* options) {
    ISceneNode_deserializeAttributes(&self, attributes, options);
}

// ---------------------------------------------------------------------------
// glitch::scene::CBillboardSceneNode
// ---------------------------------------------------------------------------

// 0x580bec _ZNK6glitch5scene19CBillboardSceneNode8getColorERNS_5video6SColorES4_
void CBillboardSceneNode_getColor(const CBillboardSceneNode& self, glitch::video::SColor& top,
                                  glitch::video::SColor& bottom) {
    bottom = self.colorBottom;
    top = self.colorTop;
}

// 0x580c48 _ZNK6glitch5scene19CBillboardSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CBillboardSceneNode_serializeAttributes(const CBillboardSceneNode& self, IAttributes* attributes,
                                             SAttributeReadWriteOptions* options) {
    ISceneNode_serializeAttributes(&self, attributes, options);
    attributes->addFloat("Width", self.width, false);
    attributes->addFloat("Height", self.height, false);
    attributes->addColor("Shade_Top", self.colorTop, false);
    attributes->addColor("Shade_Down", self.colorBottom, false);
}

// 0x580cf8 _ZN6glitch5scene19CBillboardSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CBillboardSceneNode_deserializeAttributes(CBillboardSceneNode& self, IAttributes* attributes,
                                               SAttributeReadWriteOptions* options) {
    ISceneNode_deserializeAttributes(&self, attributes, options);
    self.width = attributes->getFloat("Width");
    self.height = attributes->getFloat("Height");
    self.colorTop = attributes->getColor("Shade_Top");
    self.colorBottom = attributes->getColor("Shade_Down");
    self.setSize(glitch::core::dimension2d<float>{self.width, self.height});
}

// ---------------------------------------------------------------------------
// glitch::scene::CLightSceneNode
// ---------------------------------------------------------------------------

// 0x583d88 _ZN6glitch5scene15CLightSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CLightSceneNode_deserializeAttributes(CLightSceneNode& self, IAttributes* attributes,
                                           SAttributeReadWriteOptions* options) {
    ISceneNode_deserializeAttributes(&self, attributes, options);
    attributes->push("Light");
    CLight_deserializeAttributes(self.light, attributes, options);
    attributes->pop();
}

}  // namespace glitch::collada::recon_31
