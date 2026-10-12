// glitch::collada::animation_track::CVirtualEx<Applicator>: the wrapper the value
// tracks instantiate once per applicator. In the original, the D1 destructor of every
// instantiation below is a bare return, and its getIdentityValue writes the identity
// value word by word. The applicator layouts are not mapped, so these bodies are
// reconstructed as shared templates keyed by the word count and unit-w flag they write.
#pragma once

#include <cstddef>
#include <cstdint>

namespace glitch::collada::animation_track {

// Empty D1 destructors of CVirtualEx<CApplyValueEx<...>> (48 instantiations). The
// address and the mangled name are followed by the instantiated applicator.
//   0x60ed10 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEED1Ev
//       CApplyValueEx<quaternion, CSceneNodeQuaternionMixin<float> > 
//   0x60ed14 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEEEED1Ev
//       CApplyValueEx<quaternion, CSceneNodeQuaternionMixin<short> > 
//   0x60ed18 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEEEED1Ev
//       CApplyValueEx<quaternion, CSceneNodeQuaternionMixin<char> > 
//   0x60ed1c _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIfEEEEED1Ev
//       CApplyValueEx<quaternion, CSceneNodeQuaternionAngleMixin<float> > 
//   0x60ed20 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEEEED1Ev
//       CApplyValueEx<quaternion, CSceneNodeQuaternionAngleMixin<short> > 
//   0x60ed24 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEEEED1Ev
//       CApplyValueEx<quaternion, CSceneNodeQuaternionAngleMixin<char> > 
//   0x60ed28 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodePositionMixin<float> > 
//   0x60ed2c _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodePositionMixin<short> > 
//   0x60ed30 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodePositionMixin<char> > 
//   0x60ed34 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float> > 
//   0x60ed38 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionXEx<short>, 0, short> > 
//   0x60ed3c _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionXEx<char>, 0, char> > 
//   0x60ed40 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float> > 
//   0x60ed44 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionYEx<short>, 1, short> > 
//   0x60ed48 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionYEx<char>, 1, char> > 
//   0x60ed4c _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionZEx<float>, 2, float> > 
//   0x60ed50 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionZEx<short>, 2, short> > 
//   0x60ed54 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionZEx<char>, 2, char> > 
//   0x60ed58 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodeScaleMixin<float> > 
//   0x60ed5c _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodeScaleMixin<short> > 
//   0x60ed60 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodeScaleMixin<char> > 
//   0x60ed64 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleXEx<float>, 0, float> > 
//   0x60ed68 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleXEx<short>, 0, short> > 
//   0x60ed6c _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleXEx<char>, 0, char> > 
//   0x60ed70 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleYEx<float>, 1, float> > 
//   0x60ed74 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleYEx<short>, 1, short> > 
//   0x60ed78 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleYEx<char>, 1, char> > 
//   0x60ed7c _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleZEx<float>, 2, float> > 
//   0x60ed80 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleZEx<short>, 2, short> > 
//   0x60ed84 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleZEx<char>, 2, char> > 
//   0x60ed88 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_16CLightColorMixinIhEEEEED1Ev
//       CApplyValueEx<vector3d<float>, CLightColorMixin<unsigned char> > 
//   0x60ed8c _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEED1Ev
//       CApplyValueEx<float, CMixin<float, 1, SMaterialSetParam<SAnimationTypes<float, float> >, -1, float> > 
//   0x60ed90 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEED1Ev
//       CApplyValueEx<float [2], CMixin<float, 2, SMaterialSetParam<SAnimationTypes<float [2], float [2]> >, -1, float> > 
//   0x60ed94 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEED1Ev
//       CApplyValueEx<float [2], CMixin<float, 2, SMaterialSetParam<SAnimationTypes<float [2], float [2]> >, 0, float> > 
//   0x60ed98 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEED1Ev
//       CApplyValueEx<float [2], CMixin<float, 2, SMaterialSetParam<SAnimationTypes<float [2], float [2]> >, 1, float> > 
//   0x60ed9c _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEED1Ev
//       CApplyValueEx<float [3], CMixin<float, 3, SMaterialSetParam<SAnimationTypes<float [3], float [3]> >, -1, float> > 
//   0x60eda0 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEED1Ev
//       CApplyValueEx<float [3], CMixin<float, 3, SMaterialSetParam<SAnimationTypes<float [3], float [3]> >, 0, float> > 
//   0x60eda4 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEED1Ev
//       CApplyValueEx<float [4], CMixin<float, 4, SMaterialSetParam<SAnimationTypes<float [4], float [4]> >, -1, float> > 
//   0x60eda8 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEED1Ev
//       CApplyValueEx<float [4], CMixin<float, 4, SMaterialSetParam<SAnimationTypes<float [4], float [4]> >, 0, float> > 
//   0x60edac _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEED1Ev
//       CApplyValueEx<float [4], CMixin<float, 4, SMaterialSetParam<SAnimationTypes<float [4], float [4]> >, 1, float> > 
//   0x60edb0 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEED1Ev
//       CApplyValueEx<float [4], CMixin<float, 4, SMaterialSetParam<SAnimationTypes<float [4], float [4]> >, 2, float> > 
//   0x60edb4 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEED1Ev
//       CApplyValueEx<float [4], CMixin<float, 4, SMaterialSetParam<SAnimationTypes<float [4], float [4]> >, 3, float> > 
//   0x60edb8 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEED1Ev
//       CApplyValueEx<unsigned char [4], CMixin<unsigned char, 4, SMaterialSetParam<SAnimationTypes<unsigned char [4], SColor> >, 0, unsigned char> > 
//   0x60edbc _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEED1Ev
//       CApplyValueEx<unsigned char [4], CMixin<unsigned char, 4, SMaterialSetParam<SAnimationTypes<unsigned char [4], SColor> >, 1, unsigned char> > 
//   0x60edc0 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEED1Ev
//       CApplyValueEx<unsigned char [4], CMixin<unsigned char, 4, SMaterialSetParam<SAnimationTypes<unsigned char [4], SColor> >, 2, unsigned char> > 
//   0x60edc4 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEED1Ev
//       CApplyValueEx<unsigned char [4], CMixin<unsigned char, 4, SMaterialSetParam<SAnimationTypes<unsigned char [4], SColor> >, 3, unsigned char> > 
//   0x60edc8 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEED1Ev
//       CApplyValueEx<unsigned char [4], CMixin<unsigned char, 4, SMaterialSetParam<SAnimationTypes<unsigned char [4], SColor> >, -1, unsigned char> > 
//   0x60edcc _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEED1Ev
//       CApplyValueEx<unsigned char [3], CMixin<unsigned char, 3, SMaterialSetParam<SAnimationTypes<unsigned char [3], SColor> >, -1, unsigned char> > 
template <typename Applicator>
class CVirtualEx {
public:
    // Every D1 address listed above has this empty body.
    ~CVirtualEx() = default;
};

// getIdentityValue bodies of the instantiations listed below. The body zeroes Words
// 32-bit words of the value; UnitW writes 1.0f (0x3f800000) to the fourth word, which
// is the w component of the quaternion instantiations.
template <std::size_t Words, bool UnitW>
struct CIdentityValueWriter {
    static_assert(!UnitW || Words >= 4, "the unit w word is the fourth word");
    static void getIdentityValue(void* value) noexcept {
        auto* words = static_cast<std::uint32_t*>(value);
        for (std::size_t i = 0; i < Words; ++i) {
            words[i] = 0;
        }
        if constexpr (UnitW) {
            words[3] = 0x3f800000u;  // 1.0f
        }
    }
};

// getIdentityValue addresses of the instantiations, with the writer each one uses:
// the word count and unit-w flag decoded from the pseudocode stores.
//   0x60f03c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEEEE16getIdentityValueEPv
//       4 words, unit w yes (size 28)
//   0x60f060 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEEEE16getIdentityValueEPv
//       4 words, unit w yes (size 28)
//   0x60f084 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIfEEEEE16getIdentityValueEPv
//       4 words, unit w yes (size 28)
//   0x60f0a8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEEEE16getIdentityValueEPv
//       4 words, unit w yes (size 28)
//   0x60f0cc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEEEE16getIdentityValueEPv
//       4 words, unit w yes (size 28)
//   0x60f0f0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE16getIdentityValueEPv
//       4 words, unit w yes (size 28)
//   0x61878c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x6187cc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x61880c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x61884c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x61888c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x6188cc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x61890c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x61894c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x61898c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x6189cc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618a0c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618a4c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618a8c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618acc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618b0c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618b4c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618b8c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618bcc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618c0c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618c4c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618c8c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618ccc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618d0c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618d4c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618d8c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_16CLightColorMixinIhEEEEE16getIdentityValueEPv
//       3 words, unit w no (size 28)
//   0x618da8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE16getIdentityValueEPv
//       4 words, unit w no (size 32)
//   0x618dc8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getIdentityValueEPv
//       8 words, unit w no (size 48)
//   0x618df8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE16getIdentityValueEPv
//       8 words, unit w no (size 48)
//   0x618e28 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618ea8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE16getIdentityValueEPv
//       8 words, unit w no (size 48)
//   0x618ed8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE16getIdentityValueEPv
//       12 words, unit w no (size 64)
//   0x618f28 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEE16getIdentityValueEPv
//       4 words, unit w no (size 32)
//   0x618f48 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEE16getIdentityValueEPv
//       4 words, unit w no (size 32)
//   0x618f68 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEE16getIdentityValueEPv
//       4 words, unit w no (size 32)
//   0x618f88 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE16getIdentityValueEPv
//       4 words, unit w no (size 32)
//   0x618fa8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE16getIdentityValueEPv
//       3 words, unit w no (size 28)
//   0x618fc4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE16getIdentityValueEPv
//       4 words, unit w no (size 32)

}  // namespace glitch::collada::animation_track
