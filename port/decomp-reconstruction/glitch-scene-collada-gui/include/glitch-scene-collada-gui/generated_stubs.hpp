// Stubs for trivial glitch::collada bodies: each is either an empty body or a
// single constant return in the Hex-Rays pseudocode (re-checked in the Hex-Rays
// re-run; see NOTES.md). The first argument is the `this` pointer where the
// original is a member. The bodies are complete, so their ledger rows are
// reconstructed; they stay free function templates named by their original
// address until each is attached to its owning class. The entries of the classes that became canonical in the unification wave
// (IMesh, CMesh, CMorphingMesh, ISceneNodeAnimator, CSceneNodeAnimatorSet, CSceneNodeAnimatorSynchronizedSet, CColladaDatabase,
// CResFileManager) moved to their members.
#pragma once

#include <cstdint>

namespace glitch::collada::stubs {

// 0x60ee1c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [4], glitch::collada::animation_track::CMixin<float, 4, glitch::collada::animation_track::SMaterialSe
template <class... A> std::uint32_t stub_60ee1c(A&&...) { return 0x10u; }

// 0x60ee24 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE13retrieveValueEPvSD_
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [4], glitch::collada::animation_track::CMixin<float, 4, glitch::collada::animation_track::SMaterialSe
template <class... A> void stub_60ee24(A&&...) {}

// 0x60ee28 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [4], glitch::collada::animation_track::CMixin<float, 4, glitch::collada::animation_track::SMaterialSe
template <class... A> std::uint32_t stub_60ee28(A&&...) { return 0x10u; }

// 0x60ee30 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE13retrieveValueEPvSD_
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [4], glitch::collada::animation_track::CMixin<float, 4, glitch::collada::animation_track::SMaterialSe
template <class... A> void stub_60ee30(A&&...) {}

// 0x60ee34 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [4], glitch::collada::animation_track::CMixin<float, 4, glitch::collada::animation_track::SMaterialSe
template <class... A> std::uint32_t stub_60ee34(A&&...) { return 0x10u; }

// 0x60ee3c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE13retrieveValueEPvSD_
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [4], glitch::collada::animation_track::CMixin<float, 4, glitch::collada::animation_track::SMaterialSe
template <class... A> void stub_60ee3c(A&&...) {}

// 0x60ee40 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [4], glitch::collada::animation_track::CMixin<float, 4, glitch::collada::animation_track::SMaterialSe
template <class... A> std::uint32_t stub_60ee40(A&&...) { return 0x10u; }

// 0x60ee48 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE13retrieveValueEPvSD_
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [4], glitch::collada::animation_track::CMixin<float, 4, glitch::collada::animation_track::SMaterialSe
template <class... A> void stub_60ee48(A&&...) {}

// 0x60ee4c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [4], glitch::collada::animation_track::CMixin<float, 4, glitch::collada::animation_track::SMaterialSe
template <class... A> std::uint32_t stub_60ee4c(A&&...) { return 0x10u; }

// 0x60ee54 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE13retrieveValueEPvSD_
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [4], glitch::collada::animation_track::CMixin<float, 4, glitch::collada::animation_track::SMaterialSe
template <class... A> void stub_60ee54(A&&...) {}

// 0x60ee58 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [3], glitch::collada::animation_track::CMixin<float, 3, glitch::collada::animation_track::SMaterialSe
template <class... A> std::uint32_t stub_60ee58(A&&...) { return 0xcu; }

// 0x60ee60 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE13retrieveValueEPvSD_
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [3], glitch::collada::animation_track::CMixin<float, 3, glitch::collada::animation_track::SMaterialSe
template <class... A> void stub_60ee60(A&&...) {}

// 0x60ee64 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [3], glitch::collada::animation_track::CMixin<float, 3, glitch::collada::animation_track::SMaterialSe
template <class... A> std::uint32_t stub_60ee64(A&&...) { return 0xcu; }

// 0x60ee6c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE13retrieveValueEPvSD_
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [3], glitch::collada::animation_track::CMixin<float, 3, glitch::collada::animation_track::SMaterialSe
template <class... A> void stub_60ee6c(A&&...) {}

// 0x60ee70 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [2], glitch::collada::animation_track::CMixin<float, 2, glitch::collada::animation_track::SMaterialSe
template <class... A> std::uint32_t stub_60ee70(A&&...) { return 8u; }

// 0x60ee78 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE13retrieveValueEPvSD_
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [2], glitch::collada::animation_track::CMixin<float, 2, glitch::collada::animation_track::SMaterialSe
template <class... A> void stub_60ee78(A&&...) {}

// 0x60ee7c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [2], glitch::collada::animation_track::CMixin<float, 2, glitch::collada::animation_track::SMaterialSe
template <class... A> std::uint32_t stub_60ee7c(A&&...) { return 8u; }

// 0x60ee84 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE13retrieveValueEPvSD_
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [2], glitch::collada::animation_track::CMixin<float, 2, glitch::collada::animation_track::SMaterialSe
template <class... A> void stub_60ee84(A&&...) {}

// 0x60ee88 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [2], glitch::collada::animation_track::CMixin<float, 2, glitch::collada::animation_track::SMaterialSe
template <class... A> std::uint32_t stub_60ee88(A&&...) { return 8u; }

// 0x60ee90 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE13retrieveValueEPvSD_
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float [2], glitch::collada::animation_track::CMixin<float, 2, glitch::collada::animation_track::SMaterialSe
template <class... A> void stub_60ee90(A&&...) {}

// 0x60ee9c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE13retrieveValueEPvSC_
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float, glitch::collada::animation_track::CMixin<float, 1, glitch::collada::animation_track::SMaterialSetPar
template <class... A> void stub_60ee9c(A&&...) {}

// 0x60eea0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_16CLightColorMixinIhEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CLightColorMixin<unsigned char> > >::getVa
template <class... A> std::uint32_t stub_60eea0(A&&...) { return 3u; }

// 0x60ef0c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CSceneNodeScaleComponentMixin<glitch::coll
template <class... A> std::uint32_t stub_60ef0c(A&&...) { return 0xcu; }

// 0x60ef14 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CSceneNodeScaleComponentMixin<glitch::coll
template <class... A> std::uint32_t stub_60ef14(A&&...) { return 0xcu; }

// 0x60ef24 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CSceneNodeScaleComponentMixin<glitch::coll
template <class... A> std::uint32_t stub_60ef24(A&&...) { return 0xcu; }

// 0x60ef2c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CSceneNodeScaleComponentMixin<glitch::coll
template <class... A> std::uint32_t stub_60ef2c(A&&...) { return 0xcu; }

// 0x60ef3c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CSceneNodeScaleComponentMixin<glitch::coll
template <class... A> std::uint32_t stub_60ef3c(A&&...) { return 0xcu; }

// 0x60ef44 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CSceneNodeScaleComponentMixin<glitch::coll
template <class... A> std::uint32_t stub_60ef44(A&&...) { return 0xcu; }

// 0x60ef54 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CSceneNodeScaleMixin<char> > >::getValueSi
template <class... A> std::uint32_t stub_60ef54(A&&...) { return 0xcu; }

// 0x60ef5c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CSceneNodeScaleMixin<short> > >::getValueS
template <class... A> std::uint32_t stub_60ef5c(A&&...) { return 0xcu; }

// 0x60efa0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CSceneNodePositionComponentMixin<glitch::c
template <class... A> std::uint32_t stub_60efa0(A&&...) { return 0xcu; }

// 0x60efa8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CSceneNodePositionComponentMixin<glitch::c
template <class... A> std::uint32_t stub_60efa8(A&&...) { return 0xcu; }

// 0x60efb8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CSceneNodePositionComponentMixin<glitch::c
template <class... A> std::uint32_t stub_60efb8(A&&...) { return 0xcu; }

// 0x60efc0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CSceneNodePositionComponentMixin<glitch::c
template <class... A> std::uint32_t stub_60efc0(A&&...) { return 0xcu; }

// 0x60efd0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CSceneNodePositionComponentMixin<glitch::c
template <class... A> std::uint32_t stub_60efd0(A&&...) { return 0xcu; }

// 0x60efd8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CSceneNodePositionComponentMixin<glitch::c
template <class... A> std::uint32_t stub_60efd8(A&&...) { return 0xcu; }

// 0x60efe8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CSceneNodePositionMixin<char> > >::getValu
template <class... A> std::uint32_t stub_60efe8(A&&...) { return 0xcu; }

// 0x60eff0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, glitch::collada::animation_track::CSceneNodePositionMixin<short> > >::getVal
template <class... A> std::uint32_t stub_60eff0(A&&...) { return 0xcu; }

// 0x60f034 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::quaternion, glitch::collada::animation_track::CSceneNodeQuaternionAngleMixin<char> > >::getVa
template <class... A> std::uint32_t stub_60f034(A&&...) { return 0x10u; }

// 0x60f058 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::quaternion, glitch::collada::animation_track::CSceneNodeQuaternionAngleMixin<short> > >::getV
template <class... A> std::uint32_t stub_60f058(A&&...) { return 0x10u; }

// 0x60f07c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIfEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::quaternion, glitch::collada::animation_track::CSceneNodeQuaternionAngleMixin<float> > >::getV
template <class... A> std::uint32_t stub_60f07c(A&&...) { return 0x10u; }

// 0x60f0a0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::quaternion, glitch::collada::animation_track::CSceneNodeQuaternionMixin<char> > >::getValueSi
template <class... A> std::uint32_t stub_60f0a0(A&&...) { return 0x10u; }

// 0x60f0c4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEEEE12getValueSizeEv
// glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<glitch::core::quaternion, glitch::collada::animation_track::CSceneNodeQuaternionMixin<short> > >::getValueS
template <class... A> std::uint32_t stub_60f0c4(A&&...) { return 0x10u; }

// 0x62fee4 _ZN6glitch7collada15particle_system15CForceSceneNode4bindEPNS0_24CParticleSystemSceneNodeE
// glitch::collada::particle_system::CForceSceneNode::bind(glitch::collada::CParticleSystemSceneNode*)
template <class... A> void stub_62fee4(A&&...) {}

// 0x62fee8 _ZN6glitch7collada15particle_system15CForceSceneNode4bindEPNS0_33CGlitchNewParticleSystemSceneNodeE
// glitch::collada::particle_system::CForceSceneNode::bind(glitch::collada::CGlitchNewParticleSystemSceneNode*)
template <class... A> void stub_62fee8(A&&...) {}

// 0x62feec _ZNK6glitch7collada15particle_system15CForceSceneNode7getTypeEv
// glitch::collada::particle_system::CForceSceneNode::getType() const
template <class... A> std::uint32_t stub_62feec(A&&...) { return 0x66656164u; }

// 0x62fef8 _ZN6glitch7collada15particle_system15CForceSceneNode6renderEPv
// glitch::collada::particle_system::CForceSceneNode::render(void*)
template <class... A> void stub_62fef8(A&&...) {}

// 0x62ff04 _ZN6glitch7collada15CColladaFactory20getAdditionalEffectsERKNS0_16CColladaDatabaseEPNS0_7SEffectERNS0_11SEffectListE
// glitch::collada::CColladaFactory::getAdditionalEffects(glitch::collada::CColladaDatabase const&, glitch::collada::SEffect*, glitch::collada::SEffectList&)
template <class... A> void stub_62ff04(A&&...) {}

// 0x62ff38 _ZN6glitch7collada15CColladaFactory20isSharingMeshBuffersERKNS0_16CColladaDatabaseEPNS0_9SGeometryE
// glitch::collada::CColladaFactory::isSharingMeshBuffers(glitch::collada::CColladaDatabase const&, glitch::collada::SGeometry*)
template <class... A> std::uint32_t stub_62ff38(A&&...) { return 1u; }

// 0x637b78 _ZNK6glitch7collada33CGlitchNewParticleSystemSceneNode7getTypeEv
// glitch::collada::CGlitchNewParticleSystemSceneNode::getType() const
template <class... A> std::uint32_t stub_637b78(A&&...) { return 0x67656164u; }

// 0x646160 _ZNK6glitch7collada14CMeshSceneNode7getTypeEv
// glitch::collada::CMeshSceneNode::getType() const
template <class... A> std::uint32_t stub_646160(A&&...) { return 0x6d656164u; }

// 0x646188 _ZN6glitch7collada14CMeshSceneNode7setMeshERKN5boost13intrusive_ptrINS_5scene5IMeshEEE
// glitch::collada::CMeshSceneNode::setMesh(boost::intrusive_ptr<glitch::scene::IMesh> const&)
template <class... A> void stub_646188(A&&...) {}

// 0x6493f8 _ZNK6glitch7collada28CModularSkinnedMeshSceneNode7getTypeEv
// glitch::collada::CModularSkinnedMeshSceneNode::getType() const
template <class... A> std::uint32_t stub_6493f8(A&&...) { return 0x4d656164u; }

// 0x64bb84 _ZNK6glitch7collada24CParticleSystemSceneNode7getTypeEv
// glitch::collada::CParticleSystemSceneNode::getType() const
template <class... A> std::uint32_t stub_64bb84(A&&...) { return 0x70656164u; }

// 0x65cd1c _ZNK6glitch7collada10CSceneNode7getTypeEv
// glitch::collada::CSceneNode::getType() const
template <class... A> std::uint32_t stub_65cd1c(A&&...) { return 0x6e656164u; }

// 0x6669f8 _ZNK6glitch7collada21CSkinnedMeshSceneNode7getTypeEv
// glitch::collada::CSkinnedMeshSceneNode::getType() const
template <class... A> std::uint32_t stub_6669f8(A&&...) { return 0x73656164u; }

// 0x667774 _ZN6glitch7collada24IParticleSystemSceneNode5cloneEv
// glitch::collada::IParticleSystemSceneNode::clone()
template <class... A> std::uint32_t stub_667774(A&&...) { return 0u; }

// 0x669b6c _ZN6glitch7collada12ISkinnedMesh14setBoundingBoxERKNS_4core8aabbox3dIfEE
// glitch::collada::ISkinnedMesh::setBoundingBox(glitch::core::aabbox3d<float> const&)
template <class... A> void stub_669b6c(A&&...) {}

// 0x66b938 _ZNK6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique16needOutputBufferEv
// glitch::collada::detail::CColladaHardwareMatrixSkinTechnique::needOutputBuffer() const
template <class... A> std::uint32_t stub_66b938(A&&...) { return 0u; }

// 0x66d078 _ZNK6glitch7collada6detail33CColladaHardwareQuatSkinTechnique16needOutputBufferEv
// glitch::collada::detail::CColladaHardwareQuatSkinTechnique::needOutputBuffer() const
template <class... A> std::uint32_t stub_66d078(A&&...) { return 0u; }

// 0x66e770 _ZNK6glitch7collada6detail36CColladaHardwareTextureSkinTechnique16needOutputBufferEv
// glitch::collada::detail::CColladaHardwareTextureSkinTechnique::needOutputBuffer() const
template <class... A> std::uint32_t stub_66e770(A&&...) { return 0u; }

// 0x66f6b8 _ZNK6glitch7collada6detail29CColladaSoftwareSkinTechnique16needOutputBufferEv
// glitch::collada::detail::CColladaSoftwareSkinTechnique::needOutputBuffer() const
template <class... A> std::uint32_t stub_66f6b8(A&&...) { return 1u; }

// 0x66f6c0 _ZNK6glitch7collada6detail29CColladaSoftwareSkinTechnique17checkAvailabilityERKNS_5video10STechniqueE
// glitch::collada::detail::CColladaSoftwareSkinTechnique::checkAvailability(glitch::video::STechnique const&) const
template <class... A> std::uint32_t stub_66f6c0(A&&...) { return 1u; }

// 0x6e3110 _ZNK6glitch7collada15animation_track8CFloatEx12getValueSizeEv
// glitch::collada::animation_track::CFloatEx::getValueSize() const
template <class... A> std::uint32_t stub_6e3110(A&&...) { return 4u; }

// 0x6e4db4 _ZNK6glitch7collada15animation_track9CWeightEx12getValueSizeEv
// glitch::collada::animation_track::CWeightEx::getValueSize() const
template <class... A> std::uint32_t stub_6e4db4(A&&...) { return 4u; }

// 0x6e5168 _ZNK6glitch7collada16CCameraSceneNode7getTypeEv
// glitch::collada::CCameraSceneNode::getType() const
template <class... A> std::uint32_t stub_6e5168(A&&...) { return 0x63656164u; }

// 0x6e5a2c _ZNK6glitch7collada17CCoronasSceneNode7getTypeEv
// glitch::collada::CCoronasSceneNode::getType() const
template <class... A> std::uint32_t stub_6e5a2c(A&&...) { return 0x736e7263u; }

}  // namespace glitch::collada::stubs
