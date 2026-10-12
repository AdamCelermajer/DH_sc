// Track B batch e1-glitch-scene-collada-gui-glitch_collada_detail_CColladaSoftwareSkinTechni-0:
// glitch::collada::detail::CColladaSoftwareSkinTechnique (init, preparePtrCache, computeBoundingBox,
// prepareCache, skin). Declarations are in glitch-scene-collada-gui/include/glitch-scene-collada-gui/collada_misc.hpp.
//
// Every body below is PARTIAL: the bodies are not reconstructed because the layouts they read are not
// mapped in this category. Each function keeps its signature and returns a neutral value. The
// per-function notes name what is missing. No raw offset arithmetic is done on purpose.

#include "glitch-scene-collada-gui/collada_misc.hpp"

namespace glitch::collada::detail {

// 0x66f8ec _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique4initERNS0_11SSkinBufferEPNS_5scene11CMeshBufferEPNS_5video12IVideoDriverEb
// Partial. Not reconstructed:
//   1. IColladaSkinTechnique::initProxyBuffer (the base init that returns the proxy buffer object).
//   2. The proxy vertex-stream setup: CVertexStreams::setStream calls for the three streams (stream
//      attribute ids 0 and 6 with stride 3, and the optional stream when bit 0x20000 of the stream flags
//      is set; plus the optional stream when the byte at +0x18 is set). The stream record layout (16
//      bytes per entry, the reference counted buffer slot) is not mapped.
//   3. The material/technique lookup used for the process-buffer flags: CMaterial::getTechnique and
//      the nested technique tables (IVideoDriver process-buffer request, flags 0x60001 or 0x20001).
//      The driver request itself is IVideoDriver::getProcessBuffer, whose layout is not mapped.
// Neutral return: 0 (the original returns 0 when skipProcessBuffer is set).
int CColladaSoftwareSkinTechnique::init(SSkinBuffer& /*skinBuffer*/, glitch::scene::CMeshBuffer* /*meshBuffer*/,
                                        glitch::video::IVideoDriver* /*driver*/, bool /*skipProcessBuffer*/) {
    return 0;
}

// 0x66fab4 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique15preparePtrCacheEv
// Partial. Reconstructable intent: when bit 0x10000 of the skin-cache flags word is set, resize the
// cache's scene-node pointer vector to the skin's joint count (skin +0x74), fill each entry from the
// joint name array (skin +0x78) through ISceneNode::getSceneNodeFromScopeID on the attached node
// (+0x14), store the virtual result (vtable slot +0x38, name not mapped) or null, then clear bit
// 0x10000. Not reconstructed: the SSkin and SSkinCache layouts (joint count, name array, cache vector),
// the scene-node virtual slot name, and the ISceneNode scope lookup signature in this category.
void CColladaSoftwareSkinTechnique::preparePtrCache() {
}

// 0x66fb84 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique17computeBoundingBoxEv
// Partial. The decompiled function takes an extra argument (r1) that is read as the skin technique
// (skin at +0x0c, skin cache at +0x10, the cache flags cleared with bit 0x08 at the end), and writes a
// six-float box (min x/y/z, max x/y/z) into the receiver's first 24 bytes before returning the
// receiver. The demangled symbol has no argument, so the receiver and argument roles do not agree.
// Not reconstructed until that is settled, and the SSkinBuffer vertex/bone block (+0x8c, +0x90) and
// core::CMatrix4::transformBox usage are not mapped.
void CColladaSoftwareSkinTechnique::computeBoundingBox() {
}

// 0x66fe34 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique12prepareCacheEv
// Partial. Reconstructable intent: when bit 0x01 of the skin-cache flags word is set, call
// preparePtrCache, resize the cache's matrix vector to the skin's joint count, and for each joint
// store the product (joint matrix from the skin) * (skin block at +0x10) as a 0x41-byte record.
// Then clear bit 0x01. Not reconstructed: the SMatrix record layout (68 bytes, 16 floats plus a flag
// byte), the skin's per-joint matrix table (skin +0x04 with 64-byte stride), and the matrix product
// operator on that layout.
void CColladaSoftwareSkinTechnique::prepareCache() {
}

// 0x66ff48 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique4skinERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
// Partial. The body is a large vertex-skinning routine (11112 bytes of pseudocode). It maps the
// source and destination vertex buffers (IBuffer::map), walks the per-vertex joint indices and weights
// (up to four weights per vertex through the skin matrix table), writes position, normal and tangent
// outputs, and unmaps every buffer it mapped (dropping the reference when the buffer's count reaches
// zero). Not reconstructed: the SSkinBuffer and CMeshBuffer layouts (stream fields at +0x14..+0x2c,
// vertex stride and offset words), the CVertexStreams::getStream result layout, and the vertex
// index/weight stream formats. Neutral return: 0.
int CColladaSoftwareSkinTechnique::skin(SSkinBuffer& /*skinBuffer*/, glitch::scene::CMeshBuffer* /*meshBuffer*/) {
    return 0;
}

}  // namespace glitch::collada::detail
