// Base-class defaults of glitch::collada::CColladaFactory. The factory is an
// interface here (the concrete builders live with the scene code), so only the
// bodies the original defines on the base class are kept.
#include "glitch-scene-collada-gui/collada_factory.hpp"

namespace glitch::collada {

// 0x630008 CColladaFactory::getExternalLightSceneNode(CColladaDatabase const&, intrusive_ptr<CMaterialRenderer const> const&, unsigned short, unsigned int, char const*)
// The original tail-calls a shared error helper (0x62ff80: sets r1 to 0x23 and
// calls 0x30ec28) that is not reconstructed. Here the default does nothing.
// partial: the helper's behaviour is missing.
void CColladaFactory::getExternalLightSceneNode(const CColladaDatabase&,
                                                const boost::intrusive_ptr<video::CMaterialRenderer const>&,
                                                std::uint16_t, std::uint32_t, const char*) {}

// 0x630010 CColladaFactory::getExternalLightSceneNode(CColladaDatabase const&, intrusive_ptr<CMaterial const> const&, unsigned short, unsigned int, char const*)
// Same shared error helper as 0x630008. partial: the helper's behaviour is missing.
void CColladaFactory::getExternalLightSceneNode(const CColladaDatabase&,
                                                const boost::intrusive_ptr<video::CMaterial const>&,
                                                std::uint16_t, std::uint32_t, const char*) {}

}  // namespace glitch::collada
