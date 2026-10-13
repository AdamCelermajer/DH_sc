// Default (empty-bodied) virtual implementations in the glitch::collada
// animation-track interfaces, and a few constant-returning defaults.
// Each is a function template so one definition serves every overload
// signature; the address and symbol in the comment identify the original.
#pragma once

#include <cstddef>

namespace glitch::collada::track_defaults {

// The IAnimationTrackEx defaults and ISceneNodeAnimator::isTrackEnabled are members of the canonical classes
// (collada/IAnimationTrackEx.hpp, collada/ISceneNodeAnimator.hpp).

// CColladaFactory::createMaterialRenderer(...): returns 0 in the original.
// 0x3506a4 _ZN6glitch7collada15CColladaFactory22createMaterialRendererERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverEPNS0_7SEffectEPNS0_14CRootSceneNodeE
template <class... A> void* CColladaFactory_createMaterialRenderer_3506a4(A&&...) { return nullptr; }

// The CSceneNodeAnimatorSynchronizedSet overloads are members of the canonical class (collada/CSceneNodeAnimatorSynchronizedSet.hpp).

// CVirtualEx<CApplyValueEx<...>> retrieveValue / getValueSize (per instantiation).
// 0x60eddc, 0x60ede8, 0x60edf4, 0x60ee00, 0x60ee0c, 0x60ee18: retrieveValue, empty.
template <class... A> void VirtualEx_retrieveValue(A&&...) {}
// getValueSize per instantiation (size in bytes).
// 0x60edd4: 3; 0x60ede0, 0x60edec, 0x60edf8, 0x60ee04, 0x60ee10: 4.
template <std::size_t N> std::size_t VirtualEx_getValueSize() { return N; }

}  // namespace glitch::collada::track_defaults
