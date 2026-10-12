// glitch::core::detail::SIDedCollection for glitch-video. The layout is shared with the other
// categories and lives in port/decomp-reconstruction/shared/include/glitch-shared/sid_collection.hpp
// (copied from glitch-core-io core/sid_collection.hpp). This header only exposes it under the
// original names:
//   glitch::core::detail::SIDedCollection<T, IdType, Flag, Props, Traits>
//   glitch::core::detail::sidedcollection::SValueTraits
// The include is relative so it resolves without any extra -I flag.
#pragma once

#include "../../../../shared/include/glitch-shared/sid_collection.hpp"

namespace glitch::core::detail {

namespace sidedcollection {
// Intrusive reference counting (referenceCounter at +0x04 and drop()), as used by boost::intrusive_ptr.
using SValueTraits = sid_layout::IntrusiveTraits;
}  // namespace sidedcollection

// Original instances (binary symbols, ARM, uint16 ids, the bool is false in every instance):
//   SIDedCollection<intrusive_ptr<ITexture>, ushort, false, texturemanager::STextureProperties, SValueTraits>
//   SIDedCollection<intrusive_ptr<IShader>, ushort, false, shadermanager::SShaderProperties, SValueTraits>
//   SIDedCollection<intrusive_ptr<IShaderCode>, ushort, false, SEmptyProperties, SValueTraits>
//   SIDedCollection<intrusive_ptr<CMaterialRenderer>, ushort, false, materialrenderermanager::SProperties, SValueTraits>
// The global material parameter manager instance (SIDedCollection<SShaderParameterDef, ushort, false,
// globalmaterialparametermanager::SPropeties, SValueTraits>) stores values, not pointers, with 20-byte
// entries. The pointer-value template does not model it (see the shared header's open questions).
//
// IdType is not a parameter of the shared template: the binary uses unsigned short everywhere, and the
// shared template fixes the id at uint16_t. Flag is not modelled (its meaning is not recovered).
template <class T, class IdType, bool Flag, class Props,
          class Traits = sidedcollection::SValueTraits>
using SIDedCollection = sid_layout::SIDedCollection<T, Props, Traits>;

}  // namespace glitch::core::detail
