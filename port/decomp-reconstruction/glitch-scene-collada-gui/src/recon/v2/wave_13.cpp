// Wave 13 (partial-row rerun). The two pseudocode bodies below are empty
// (`;`) in the original, so there is nothing to reproduce beyond the signature.
#include "glitch-scene-collada-gui/recon/v2/wave_13.hpp"

namespace glitch::collada::recon_v2_13 {

// 0x5482a0 _ZNK6glitch3gui15CGUIModalScreen19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUIModalScreen_serializeAttributes(const glitch::gui::CGUIModalScreen* /*self*/,
                                         glitch::io::IAttributes* /*a1*/,
                                         glitch::io::SAttributeReadWriteOptions* /*a2*/) {}

// 0x5482a4 _ZN6glitch3gui15CGUIModalScreen21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUIModalScreen_deserializeAttributes(glitch::gui::CGUIModalScreen* /*self*/,
                                           glitch::io::IAttributes* /*a1*/,
                                           glitch::io::SAttributeReadWriteOptions* /*a2*/) {}

// CBatchMesh::loadSegmentExtraData (0x578920): canonical empty member in src/scene/CBatchMesh.cpp.

}  // namespace glitch::collada::recon_v2_13
