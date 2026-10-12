// Default (empty-bodied) virtual implementations in the glitch::collada
// animation-track interfaces, and a few constant-returning defaults.
// Each is a function template so one definition serves every overload
// signature; the address and symbol in the comment identify the original.
#pragma once

#include <cstddef>

namespace glitch::collada::track_defaults {

// IAnimationTrackEx: every virtual is empty in the original.
// 0x60def8 IAnimationTrackEx::applyBlendedValue(void*, float*, int, void*, CApplicatorInfo*) const
template <class... A> void IAnimationTrackEx_applyBlendedValue_60def8(A&&...) {}
// 0x60defc IAnimationTrackEx::applyAddedValue(void*, float*, int, void*, CApplicatorInfo*) const
template <class... A> void IAnimationTrackEx_applyAddedValue_60defc(A&&...) {}
// 0x60df00 IAnimationTrackEx::getBlendedValue(void*, float*, int, void*, float) const
template <class... A> void IAnimationTrackEx_getBlendedValue_60df00(A&&...) {}
// 0x60df04 IAnimationTrackEx::getAddedValue(void*, float*, int, void*, float) const
template <class... A> void IAnimationTrackEx_getAddedValue_60df04(A&&...) {}
// 0x60df08 IAnimationTrackEx::getKeyBasedValue(SAnimationAccessor const&, int, int, float, void*, float) const
template <class... A> void IAnimationTrackEx_getKeyBasedValue_60df08(A&&...) {}
// 0x60df0c IAnimationTrackEx::getKeyBasedValue(SAnimationAccessor const&, int, void*, float) const
template <class... A> void IAnimationTrackEx_getKeyBasedValue_60df0c(A&&...) {}
// 0x60df70 IAnimationTrackEx::applyKeyBasedValue(SAnimationAccessor const&, int, void*, CApplicatorInfo*) const
template <class... A> void IAnimationTrackEx_applyKeyBasedValue_60df70(A&&...) {}
// 0x60dfa8 IAnimationTrackEx::applyBlendedValue(void*, float*, int, void*, CApplicatorInfo*, float) const
template <class... A> void IAnimationTrackEx_applyBlendedValue_60dfa8(A&&...) {}
// 0x60dfac IAnimationTrackEx::applyAddedValue(void*, float*, int, void*, CApplicatorInfo*, float) const
template <class... A> void IAnimationTrackEx_applyAddedValue_60dfac(A&&...) {}
// 0x60dfb0 IAnimationTrackEx::applyKeyBasedValue(SAnimationAccessor const&, int, int, float, void*, CApplicatorInfo*, float) const
template <class... A> void IAnimationTrackEx_applyKeyBasedValue_60dfb0(A&&...) {}
// 0x60dfb4 IAnimationTrackEx::applyKeyBasedValue(SAnimationAccessor const&, int, void*, CApplicatorInfo*, float) const
template <class... A> void IAnimationTrackEx_applyKeyBasedValue_60dfb4(A&&...) {}
// 0x60dfb8 IAnimationTrackEx::applyValue(SAnimationAccessor const&, int, void*, CApplicatorInfo*, bool) const
template <class... A> void IAnimationTrackEx_applyValue_60dfb8(A&&...) {}
// 0x60dfbc IAnimationTrackEx::applyValue(SAnimationAccessor const&, int, void*, CApplicatorInfo*, bool&) const
template <class... A> void IAnimationTrackEx_applyValue_60dfbc(A&&...) {}

// ISceneNodeAnimator::isTrackEnabled(int): returns true in the original.
// 0x366214 _ZN6glitch7collada18ISceneNodeAnimator14isTrackEnabledEi
inline bool ISceneNodeAnimator_isTrackEnabled(int) { return true; }

// CColladaFactory::createMaterialRenderer(...): returns 0 in the original.
// 0x3506a4 _ZN6glitch7collada15CColladaFactory22createMaterialRendererERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverEPNS0_7SEffectEPNS0_14CRootSceneNodeE
template <class... A> void* CColladaFactory_createMaterialRenderer_3506a4(A&&...) { return nullptr; }

// CSceneNodeAnimatorSynchronizedSet empty-bodied overloads.
// 0x368ddc getSynchronizationLength(int, unsigned int) const
template <class... A> void SynchronizedSet_getSynchronizationLength_368ddc(A&&...) {}
// 0x368e10 getSynchronizationTimestamp(int, unsigned int) const
template <class... A> void SynchronizedSet_getSynchronizationTimestamp_368e10(A&&...) {}
// 0x368e8c getSynchronizationSegment(int, int) const
template <class... A> void SynchronizedSet_getSynchronizationSegment_368e8c(A&&...) {}

// CVirtualEx<CApplyValueEx<...>> retrieveValue / getValueSize (per instantiation).
// 0x60eddc, 0x60ede8, 0x60edf4, 0x60ee00, 0x60ee0c, 0x60ee18: retrieveValue, empty.
template <class... A> void VirtualEx_retrieveValue(A&&...) {}
// getValueSize per instantiation (size in bytes).
// 0x60edd4: 3; 0x60ede0, 0x60edec, 0x60edf8, 0x60ee04, 0x60ee10: 4.
template <std::size_t N> std::size_t VirtualEx_getValueSize() { return N; }

}  // namespace glitch::collada::track_defaults
