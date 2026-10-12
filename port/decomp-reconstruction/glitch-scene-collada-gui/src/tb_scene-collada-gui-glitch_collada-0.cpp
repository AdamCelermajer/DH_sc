// Track B batch e1-glitch-scene-collada-gui-glitch_collada-0: glitch::collada free functions of the
// material creation path. Declarations are in glitch-scene-collada-gui/tb/glitch_collada_material_creation.hpp.

#include "glitch-scene-collada-gui/tb/glitch_collada_material_creation.hpp"

namespace glitch::collada {

// 0x667c38 _ZN6glitch7collada18getStringsInternalEPNS0_18ISceneNodeAnimator20E_INTERPOLATION_MODEE
// Partial. The original is `return &strings;`, the address of a function-local static table. The
// element count and the string contents are not mapped: the pseudocode gives only the address, and
// the table is not in the decompiled data. The hook stays null, the same way IGUIElement::getTypeName
// is handled in NOTES.md. Layout request: map the `strings` table (element count and contents) from
// the data at the table address. The E_INTERPOLATION_MODE* argument is never read by the original.
char** getStringsInternal() {
    return nullptr;
}

// 0x631ce8 _ZN6glitch7collada14createMaterialERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS4_17CMaterialRendererEEERNS0_9SMaterialEPNS0_14CRootSceneNodeE
// Partial, no body. The pseudocode is a 1,500-line ARM register-level flow. It depends on types and
// tables that are not mapped in this category:
//   - glitch::video::CMaterial::allocate and the shared-string heap entry (SSharedStringHeapEntry::SData)
//     that the material name is read from (the first word of the SMaterial argument);
//   - CMaterialRenderer::getParameterID and getTechniqueID, whose not-found result is a sentinel
//     address (elf_hash_bucket) compared by pointer, not a value;
//   - the SMaterial parameter list. The pseudocode reads a count at SMaterial +0x10 and 24-byte entries
//     from the array at +0x14, with a type word at entry +0x08, a value pointer at entry +0x10 and a
//     nested list pointer at entry +0x14. The entry layout is not mapped in collada_database.hpp;
//   - the SShaderParameterTypeInspection tables (ValueTypeArraySize, ValueTypeSize, ValueTypeBaseType),
//     the anonymous-namespace cvt table and ShaderParameterValueTypeMap;
//   - the IMaterialParameters setParameter, setParameterCvt and texture-typed setParameter templates;
//   - CRootSceneNode::addURLToResolve, the CMatrix4 identity check, and the os::Printer::logf
//     diagnostics. The getStringsInternal() table is also used here for parameter names.
// Lacking those pieces, no body is written and the row stays partial.
//
// 0x6357a0 _ZN6glitch7collada32createMaterialRendererForProfileINS0_18SProfileNullTraitsEEEN5boost13intrusive_ptrINS_5video17CMaterialRendererEEERKNS0_16CColladaDatabaseEPNS5_12IVideoDriverEPKcRKNS0_11SEffectListEPNS0_14CRootSceneNodeE
// Partial, no body. The SProfileNullTraits instantiation of the profile template. Only the signature,
// the local declarations and the first part of the flow were read in this batch. The rest is not
// reconstructed: the profile-traits layout (SProfileNullTraits) is not mapped, and so are the
// CMaterialRenderer construction path, the SEffectList walk, the parameter-name and value-type
// lookups, and the diagnostics.
//
// 0x6361e8 _ZN6glitch7collada32createMaterialRendererForProfileINS0_19SProfileGLES2TraitsEEEN5boost13intrusive_ptrINS_5video17CMaterialRendererEEERKNS0_16CColladaDatabaseEPNS5_12IVideoDriverEPKcRKNS0_11SEffectListEPNS0_14CRootSceneNodeE
// Partial, no body. The SProfileGLES2Traits instantiation. The same unmapped pieces as the NullTraits
// instantiation apply, plus the GLES2 traits layout, which is not mapped.

}  // namespace glitch::collada
