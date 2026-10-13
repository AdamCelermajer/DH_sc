// glitch::core::detail::SIDedCollection, sidedcollection::SValueTraits and SEmptyProperties have one definition:
// glitch-core-io/core/sided_collection.hpp (over glitch-shared/sid_collection.hpp). This header only forwards so that the
// includes of the earlier waves keep working.
//
// Original instances (binary symbols, ARM, uint16 ids, the bool is false in every instance):
//   SIDedCollection<intrusive_ptr<ITexture>, ushort, false, texturemanager::STextureProperties, SValueTraits>
//   SIDedCollection<intrusive_ptr<IShader>, ushort, false, shadermanager::SShaderProperties, SValueTraits>
//   SIDedCollection<intrusive_ptr<IShaderCode>, ushort, false, SEmptyProperties, SValueTraits>
//   SIDedCollection<intrusive_ptr<CMaterialRenderer>, ushort, false, materialrenderermanager::SProperties, SValueTraits>
//   SIDedCollection<SShaderParameterDef, ushort, false, globalmaterialparametermanager::SPropeties, SValueTraits>
#pragma once

#include "glitch-core-io/core/sided_collection.hpp"
