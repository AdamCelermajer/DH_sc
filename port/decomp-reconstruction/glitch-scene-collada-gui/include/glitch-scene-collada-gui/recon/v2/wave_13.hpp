// Wave 13 (partial-row rerun): reconstructed bodies whose pseudocode is empty.
// Signatures follow the Hex-Rays category; the owning classes are only named
// here (no layout member is touched by these bodies).
#ifndef GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_13_HPP
#define GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_13_HPP

namespace glitch {
namespace io {
class IAttributes;
class IReadFile;
class SAttributeReadWriteOptions;
}  // namespace io
namespace gui {
class CGUIModalScreen;
}  // namespace gui
}  // namespace glitch

namespace glitch::collada::recon_v2_13 {

// 0x5482a0 _ZNK6glitch3gui15CGUIModalScreen19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUIModalScreen_serializeAttributes(const glitch::gui::CGUIModalScreen* self,
                                         glitch::io::IAttributes* a1,
                                         glitch::io::SAttributeReadWriteOptions* a2);

// 0x5482a4 _ZN6glitch3gui15CGUIModalScreen21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUIModalScreen_deserializeAttributes(glitch::gui::CGUIModalScreen* self,
                                           glitch::io::IAttributes* a1,
                                           glitch::io::SAttributeReadWriteOptions* a2);

// CBatchMesh::loadSegmentExtraData (0x578920) is a canonical member of scene/CBatchMesh.hpp.

}  // namespace glitch::collada::recon_v2_13

#endif  // GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_13_HPP
