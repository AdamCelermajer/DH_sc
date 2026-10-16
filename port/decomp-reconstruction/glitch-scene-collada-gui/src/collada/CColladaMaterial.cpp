// glitch::collada free functions of the material creation path (canonical header collada/CColladaMaterial.hpp), reconstructed by residue
// pass ob. This file replaces the partial row of src/tb_scene-collada-gui-glitch_collada-0.cpp for createMaterial 0x631ce8.
//
// A comment `// 0xADDR symbol` precedes each function.
#include "glitch-scene-collada-gui/collada/CColladaMaterial.hpp"

#include <cstdio>
#include <cstring>

#include "glitch-core-io/os/logging.hpp"
#include "glitch-scene-collada-gui/collada/CColladaDatabase.hpp"
#include "glitch-scene-collada-gui/collada/CRootSceneNode.hpp"
#include "glitch-scene-collada-gui/collada/ResString.hpp"
#include "glitch-shared/shared_string_heap.hpp"
#include "glitch-video/material.hpp"
#include "glitch-video/material_renderer.hpp"
#include "glitch-video/video_driver.hpp"
#include "glitch-video/texture.hpp"

namespace glitch::collada {

namespace {

// glitch::video::detail::SShaderParameterTypeInspection tables (ELF 0x8e2c44 / 0x8e2ca4 / 0x8e2bf8), read from the binary. They are
// indexed by the renderer-side value type code (0 .. 18); the listing indexes them with the SParam type plus one.
constexpr std::uint8_t kValueTypeArraySize[19] = {1, 1, 2, 3, 4, 1, 2, 3, 4, 4, 9, 16, 1, 1, 1, 1, 4, 4, 1};
constexpr std::uint8_t kValueTypeSize[19] = {1, 4, 8, 12, 16, 4, 8, 12, 16, 16, 36, 4, 4, 4, 4, 4, 4, 16, 4};
constexpr std::uint32_t kValueTypeBaseType[19] = {0, 1, 1, 1, 1, 5, 5, 5, 5, 5, 5, 5, 12, 13, 14, 15, 0, 5, 18};

// isParameterTypeValidSourceFor(SParam::E_TYPE, E_SHADER_PARAMETER_VALUE_TYPE)::cvt (ELF 0x8e4e54): for each renderer value type, the bit set
// of the SParam types that may fill it.
constexpr std::uint32_t kCvt[19] = {0x0,   0x1f,   0xe,    0xc,    0x8,    0xf1,   0xe0,    0xc0,    0x18080, 0x100,
                                    0x200, 0x400,  0x800,  0x1000, 0x2000, 0x4000, 0x180cc, 0x180cc, 0xa0000};

// ShaderParameterValueTypeMap (ELF 0x8e4ea0): the renderer value type an SParam type is stored as (255: none). The entry for int4 is 3,
// as in the binary.
constexpr std::int32_t kValueTypeMap[21] = {1, 2, 3, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 255, 255, 255};

// getParameterString(int)::C (ELF 0x957570): the names of the SParam types.
const char* const kParameterStrings[22] = {"int",      "int2",       "int3",        "int4",   "float",     "float2",  "float3",
                                           "float4",   "float2x2",   "float3x3",    "float4x4", "sampler2D", "sampler3D", "samplerCube",
                                           "samplerRect", "color",   "colorf",      "light",  "surface",   "url",     "technique",
                                           nullptr};

// video::getStringsInternal(E_SHADER_PARAMETER_VALUE_TYPE*) (ELF 0x99b738): the names of the renderer value types.
const char* const kValueTypeStrings[19] = {"ubyte",  "int",    "int2",      "int3",        "int4",   "float",      "float2",
                                           "float3", "float4", "float2x2",  "float3x3",    "float4x4", "sampler2D", "sampler3D",
                                           "samplerCUBE", "samplerRECT", "color", "colorf", "light"};

// The bytes one element of an SParam type takes in the file: ValueTypeArraySize[t + 1] * ValueTypeSize[ValueTypeBaseType[t + 1]].
std::uint32_t elementBytes(std::int32_t paramType) {
    const std::int32_t index = paramType + 1;
    if (index < 0 || index >= 19) {
        return 0;
    }
    return static_cast<std::uint32_t>(kValueTypeArraySize[index]) * kValueTypeSize[kValueTypeBaseType[index]];
}

const char* orNull(const char* text) { return text != nullptr ? text : "(null)"; }

// The shared body of createMaterial's parameter switch (0x631f54) and of setMaterialParameter<intrusive_ptr<CMaterialRenderer>, SNewParam>
// (0x6324c4), which differ only in where the parameter record keeps its type, the owner they write into and the name they log. `values` is the
// parameter's value array and `elementCount` the number of elements it holds. Returns false after logging a mismatch.
template <typename Owner>
bool setParameterValues(Owner& owner, const boost::intrusive_ptr<Owner>& handle, std::uint16_t id, std::int32_t paramType,
                        const std::uint32_t* elementCount, const void* values, const char* ownerName, CRootSceneNode* root) {
    const video::detail::SParameterEntry* def = owner.parameters.getParameterDef(id);
    if (def == nullptr) {
        // The original reads through a null entry here. The ids come from the renderer's own lookup, so this cannot happen for a valid pair.
        return false;
    }
    const std::uint32_t rendererCount = def->count;
    const std::uint32_t rendererType = def->type;
    const char* parameterName = core::detail::sharedStringText(static_cast<const core::detail::SSharedStringData*>(def->name));
    if (rendererCount > *elementCount) {
        os::Printer::logf(ELL_ERROR, "Not enough elements in parameter array \"%s/%s\"", orNull(ownerName), orNull(parameterName));
        return false;
    }
    // A renderer type past the table reads past it in the original; the port treats it as accepting nothing.
    const std::uint32_t allowed = rendererType < 19 ? kCvt[rendererType] : 0;
    if ((allowed & (1u << paramType)) == 0) {
        const char* want = rendererType < 19 ? kValueTypeStrings[rendererType] : "unknown";
        os::Printer::logf(ELL_ERROR, "parameter type mismatch when setting \"%s/%s\": want %s, got %s", orNull(ownerName),
                          orNull(parameterName), want, orNull(paramType >= 0 && paramType < 22 ? kParameterStrings[paramType] : nullptr));
        return false;
    }
    switch (rendererType) {
        case 9:
        case 10:
            return true;
        case 11: {
            // One matrix per element; an identity matrix is not written (the slot already holds the identity).
            const std::uint32_t stride = elementBytes(paramType);
            const std::uint8_t* source = static_cast<const std::uint8_t*>(values);
            core::CMatrix4<float> matrix(core::CMatrix4<float>::EM4CONST_IDENTITY);
            for (std::uint32_t index = 0; index != rendererCount; ++index) {
                std::memcpy(matrix.M, source + static_cast<std::size_t>(index) * stride, sizeof(matrix.M));
                matrix.flag = false;
                if (!matrix.isIdentity()) {
                    owner.parameters.setParameter(id, index, matrix);
                }
            }
            return true;
        }
        case 12:
        case 13:
        case 14:
        case 15: {
            // One texture per element; an element without an image record keeps the parameter's own texture.
            const SParamSampler* const* samplers = static_cast<const SParamSampler* const*>(values);
            for (std::uint32_t index = 0; index != rendererCount; ++index) {
                const SImage* image = samplers[index]->image;
                if (image != nullptr) {
                    boost::intrusive_ptr<video::ITexture> texture(static_cast<video::ITexture*>(image->imageData));
                    owner.parameters.template setParameter<boost::intrusive_ptr<video::ITexture>>(id, index, texture);
                }
            }
            return true;
        }
        case 18: {
            // A light is named by a URL, resolved after the whole file is built; the walk stops at an empty text or at "#" alone.
            if (rendererCount == 0) {
                return true;
            }
            const char* const* urls = static_cast<const char* const*>(values);
            for (std::uint32_t index = 0; index != rendererCount; ++index) {
                const char* text = urls[index];
                // The original reads the length word before the text; the port tests the text itself for empty (a null text counts as empty).
                if (text == nullptr || text[0] == '\0') {
                    break;
                }
                if (text[0] == '#' && text[1] == '\0') {
                    break;
                }
                if (root != nullptr) {
                    const res::String url(text);
                    root->addURLToResolve(handle, id, index, url);
                }
            }
            return true;
        }
        default:
            owner.parameters.setParameterCvt(id, kValueTypeMap[paramType], values, static_cast<int>(elementBytes(paramType)));
            return true;
    }
}

// 0x6324c4 _ZN6glitch7collada12_GLOBAL__N_120setMaterialParameterIN5boost13intrusive_ptrINS_5video17CMaterialRendererEEENS0_9SNewParamEEEbRKT_tRT0_PNS0_14CRootSceneNodeE
// The value of an effect's new-param written into the renderer's default parameter `id` (the checks, conversions and diagnostics are the ones
// of createMaterial's parameter switch, with the renderer's name in the messages). Returns false after a logged mismatch.
[[maybe_unused]] bool setMaterialParameter(const boost::intrusive_ptr<video::CMaterialRenderer>& renderer, std::uint16_t id,
                                           const SNewParam& parameter, CRootSceneNode* root) {
    return setParameterValues<video::CMaterialRenderer>(*renderer, renderer, id, parameter.type, parameter.elementCount, parameter.values,
                                                         renderer->m_name, root);
}

}  // namespace

// 0x631ce8 _ZN6glitch7collada14createMaterialERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS4_17CMaterialRendererEEERNS0_9SMaterialEPNS0_14CRootSceneNodeE
// The material is allocated over the renderer (technique 0, the record's name) and every parameter of the record is applied: a parameter
// the renderer does not know is skipped, except a technique parameter, which selects the material's technique when the renderer has one
// of that name; for a known one the value is checked and converted as setParameterValues does. A mismatch is logged and the parameter
// skipped (the next one is still applied).
boost::intrusive_ptr<video::CMaterial> createMaterial(const CColladaDatabase&, video::IVideoDriver*,
                                                      const boost::intrusive_ptr<video::CMaterialRenderer>& renderer, SMaterial& record,
                                                      CRootSceneNode* root) {
    boost::intrusive_ptr<video::CMaterial> result;
    if (!renderer) {
        return result;
    }
    result = video::CMaterial::allocate(renderer, record.name, 0);
    for (std::int32_t i = 0; i < record.paramCount; ++i) {
        const SParam& param = record.params[i];
        const std::uint16_t id = renderer->getParameterID(param.name, 0);
        if (id == 0xFFFF) {
            if (param.type == SParam::ET_TECHNIQUE) {
                const std::uint8_t technique = renderer->getTechniqueID(static_cast<const SParamTechnique*>(param.values)->name);
                if (technique != 0xFF) {
                    result->m_technique = technique;
                }
            }
            continue;
        }
        const char* materialName = result->m_hasName ? result->m_nameText.c_str() : nullptr;
        setParameterValues<video::CMaterial>(*result, result, id, param.type, param.elementCount, param.values, materialName, root);
    }
    return result;
}

// 0x636b6c _ZN6glitch7collada22createMaterialRendererERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverEPKcRKNS0_11SEffectListEPNS0_14CRootSceneNodeE
// The driver (vtable +0x5c) tells its family: no GLES (bits 7) and some GLES2 bits (0x18) give the GLES2 profile; a driver of type 0
// (no bit of 7, 0x18 or 0x360) gives the null profile; every other family leaves the result null.
boost::intrusive_ptr<video::CMaterialRenderer> createMaterialRenderer(const CColladaDatabase& db, video::IVideoDriver* driver,
                                                                      const char* name, const SEffectList& effects, CRootSceneNode* root) {
    boost::intrusive_ptr<video::CMaterialRenderer> result;
    const std::uint32_t family = static_cast<std::uint32_t>(driver->getDriverType());
    if ((family & 7u) == 0) {
        if ((family & 0x18u) != 0) {
            result = createMaterialRendererForProfile<SProfileGLES2Traits>(db, driver, name, effects, root);
        } else if ((family & 0x360u) == 0 && family == 0) {
            result = createMaterialRendererForProfile<SProfileNullTraits>(db, driver, name, effects, root);
        }
    }
    return result;
}

}  // namespace glitch::collada
