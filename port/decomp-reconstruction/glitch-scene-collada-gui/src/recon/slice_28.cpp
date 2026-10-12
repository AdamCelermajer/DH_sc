// Slice 28 reconstruction bodies. See the header for the unattached rationale.
#include "glitch-scene-collada-gui/recon/slice_28.hpp"

#include <cstdint>

namespace glitch {
namespace scene {
namespace recon_28 {
// 0x6d72d0 _ZN6glitch5scene15createPlaneMeshEjPNS_5video12IVideoDriverEfRKNS1_6SColorE
glitch::IReferenceCounted* createPlaneMesh(unsigned int, glitch::video::IVideoDriver*, float, const glitch::video::SColor&) { return nullptr; }

// 0x6d7808 _ZN6glitch5scene16createSphereMeshEjPNS_5video12IVideoDriverEfjj
glitch::IReferenceCounted* createSphereMesh(unsigned int, glitch::video::IVideoDriver*, float, unsigned int, unsigned int) { return nullptr; }

// 0x6d841c _ZN6glitch5scene19createHillPlaneMeshEjPNS_5video12IVideoDriverERKNS_4core11dimension2dIfEERKNS5_IjEERKN5boost13intrusive_ptrINS1_9CMaterialEEERKNSD_INS1_27CMaterialVertexAttributeMapEEEfS8_S8_
glitch::IReferenceCounted* createHillPlaneMesh(unsigned int, glitch::video::IVideoDriver*, const glitch::core::dimension2d<float>&, const glitch::core::dimension2d<unsigned int>&, const boost::intrusive_ptr<glitch::video::CMaterial>&, const boost::intrusive_ptr<glitch::video::CMaterialVertexAttributeMap>&, float, const glitch::core::dimension2d<float>&, const glitch::core::dimension2d<float>&) { return nullptr; }

// 0x6d8cb4 _ZN6glitch5scene14createCubeMeshEjPNS_5video12IVideoDriverEf
glitch::IReferenceCounted* createCubeMesh(unsigned int, glitch::video::IVideoDriver*, float) { return nullptr; }

// 0x6d945c _ZN6glitch5scene18createCylinderMeshEjPNS_5video12IVideoDriverEffjRKNS1_6SColorEbf
glitch::IReferenceCounted* createCylinderMesh(unsigned int, glitch::video::IVideoDriver*, float, float, unsigned int, const glitch::video::SColor&, bool, float) { return nullptr; }

// 0x6d9fc4 _ZN6glitch5scene14createConeMeshEjPNS_5video12IVideoDriverEffjRKNS1_6SColorES6_f
glitch::IReferenceCounted* createConeMesh(unsigned int, glitch::video::IVideoDriver*, float, float, unsigned int, const glitch::video::SColor&, const glitch::video::SColor&, float) { return nullptr; }

// 0x6da6d8 _ZN6glitch5scene15createArrowMeshEjPNS_5video12IVideoDriverEjjffffNS1_6SColorES4_
glitch::IReferenceCounted* createArrowMesh(unsigned int, glitch::video::IVideoDriver*, unsigned int, unsigned int, float, float, float, float, glitch::video::SColor, glitch::video::SColor) { return nullptr; }

}  // namespace recon_28
}  // namespace scene
}  // namespace glitch

namespace glitch {
namespace collada {
namespace recon_28 {
namespace CAnimationSetTransformationTemplate {
// 0x6e264c _ZN6glitch7collada35CAnimationSetTransformationTemplateC1EPNS0_10CSceneNodeE
// 0x6e2690 _ZN6glitch7collada35CAnimationSetTransformationTemplateC1EPNS0_10CSceneNodeE
void construct(void* self, glitch::collada::CSceneNode*) {}

// 0x6e2830 _ZN6glitch7collada35CAnimationSetTransformationTemplateC1EPNS0_16CColladaDatabaseE
// 0x6e28f0 _ZN6glitch7collada35CAnimationSetTransformationTemplateC1EPNS0_16CColladaDatabaseE
void construct(void* self, glitch::collada::CColladaDatabase*) {}

}  // namespace CAnimationSetTransformationTemplate
}  // namespace recon_28
}  // namespace collada
}  // namespace glitch

namespace glitch {
namespace collada {
namespace recon_28 {
namespace CAnimationTrackEx {
// 0x6e29d4 _ZNK6glitch7collada17CAnimationTrackEx13retrieveValueEPvS2_
void* retrieveValue(const void* self, void*, void*) { return nullptr; }

// 0x6e2a18 _ZNK6glitch7collada17CAnimationTrackEx10applyValueERKNS0_18SAnimationAccessorEiPvPNS0_15animation_track15CApplicatorInfoERifb
int applyValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, glitch::collada::animation_track::CApplicatorInfo*, int&, float, bool) { return {}; }

// 0x6e2ad8 _ZNK6glitch7collada17CAnimationTrackEx10applyValueERKNS0_18SAnimationAccessorEiPvPNS0_15animation_track15CApplicatorInfoERib
int applyValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, glitch::collada::animation_track::CApplicatorInfo*, int&, bool) { return {}; }

// 0x6e2b8c _ZNK6glitch7collada17CAnimationTrackEx8getValueERKNS0_18SAnimationAccessorEiPvRifb
int getValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, int&, float, bool) { return {}; }

// 0x6e2ce8 _ZNK6glitch7collada17CAnimationTrackEx10applyValueERKNS0_18SAnimationAccessorEiiPvPNS0_15animation_track15CApplicatorInfoERib
int applyValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, void*, glitch::collada::animation_track::CApplicatorInfo*, int&, bool) { return {}; }

// 0x6e2dbc _ZNK6glitch7collada17CAnimationTrackEx8getValueERKNS0_18SAnimationAccessorEiiPvRib
int getValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, void*, int&, bool) { return {}; }

// 0x6e2e90 _ZNK6glitch7collada17CAnimationTrackEx10applyValueERKNS0_18SAnimationAccessorEiPvPNS0_15animation_track15CApplicatorInfoEfb
int applyValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, glitch::collada::animation_track::CApplicatorInfo*, float, bool) { return {}; }

// 0x6e2f3c _ZNK6glitch7collada17CAnimationTrackEx10applyValueERKNS0_18SAnimationAccessorEiPvPNS0_15animation_track15CApplicatorInfoEb
int applyValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, glitch::collada::animation_track::CApplicatorInfo*, bool) { return {}; }

// 0x6e2fdc _ZNK6glitch7collada17CAnimationTrackEx8getValueERKNS0_18SAnimationAccessorEiPvfb
int getValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, float, bool) { return {}; }

// 0x6e307c _ZNK6glitch7collada17CAnimationTrackEx8getValueERKNS0_18SAnimationAccessorEiPvb
int getValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, bool) { return {}; }

}  // namespace CAnimationTrackEx
}  // namespace recon_28
}  // namespace collada
}  // namespace glitch

namespace glitch {
namespace collada {
namespace recon_28 {
namespace animation_track {
namespace CFloatEx {
// 0x6e3124 _ZNK6glitch7collada15animation_track8CFloatEx15getBlendedValueEPvPfiS3_
float getBlendedValue(const void* self, void* values, float* weights, int count, void* out)
{
    const float* v = static_cast<const float*>(values);
    float* o = static_cast<float*>(out);
    float total = 0.0f;
    for (int i = 0; i < count; ++i) {
        total = total + v[i] * weights[i];
    }
    *o = total;
    return count > 0 ? total : 0.0f;
}

// 0x6e3184 _ZNK6glitch7collada15animation_track8CFloatEx13getAddedValueEPvPfiS3_
float getAddedValue(const void* self, void* values, float* weights, int count, void* out)
{
    const float* v = static_cast<const float*>(values);
    float* o = static_cast<float*>(out);
    float total = 0.0f;
    for (int i = 0; i < count; ++i) {
        total = total + v[i] * weights[i];
    }
    *o = total;
    return count > 0 ? total : 0.0f;
}

// 0x6e3258 _ZNK6glitch7collada15animation_track8CFloatEx10applyValueEPvS3_PNS1_15CApplicatorInfoE
void* applyValue(const void* self, void*, void*, glitch::collada::animation_track::CApplicatorInfo*) { return nullptr; }

// 0x6e3288 _ZN6glitch7collada15animation_track8CFloatEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, void*) { return {}; }

// 0x6e32b8 _ZNK6glitch7collada15animation_track8CFloatEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, void*) { return {}; }

// 0x6e32cc _ZNK6glitch7collada15animation_track8CFloatEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
int getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*) { return {}; }

// 0x6e32f4 _ZN6glitch7collada15animation_track8CFloatEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, int, float, void*) { return {}; }

// 0x6e3350 _ZNK6glitch7collada15animation_track8CFloatEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, int, float, void*) { return {}; }

// 0x6e3374 _ZN6glitch7collada15animation_track8CFloatEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*) { return {}; }

// 0x6e33c4 _ZNK6glitch7collada15animation_track8CFloatEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*) { return {}; }

}  // namespace CFloatEx
}  // namespace animation_track
}  // namespace recon_28
}  // namespace collada
}  // namespace glitch

namespace glitch {
namespace collada {
namespace recon_28 {
namespace animation_track {
namespace CTextureTransformEx {
// 0x6e3420 _ZNK6glitch7collada15animation_track19CTextureTransformEx15getBlendedValueEPvPfiS3_
float getBlendedValue(const void* self, void*, float*, int, void*) { return {}; }

// 0x6e358c _ZNK6glitch7collada15animation_track19CTextureTransformEx13getAddedValueEPvPfiS3_
float getAddedValue(const void* self, void*, float*, int, void*) { return {}; }

// 0x6e38c4 _ZN6glitch7collada15animation_track19CTextureTransformEx12applyValueExEPvRNS2_5SDataEPNS1_15CApplicatorInfoE
int applyValueEx(void* self, void*, void*, glitch::collada::animation_track::CApplicatorInfo*) { return {}; }

// 0x6e395c _ZN6glitch7collada15animation_track19CTextureTransformEx19applyBlendedValueExEPvPfiS3_
int applyBlendedValueEx(void* self, void*, float*, int, void*) { return {}; }

// 0x6e3a90 _ZNK6glitch7collada15animation_track19CTextureTransformEx15applyAddedValueEPvPfiS3_
int applyAddedValue(const void* self, void*, float*, int, void*) { return {}; }

// 0x6e3aa4 _ZNK6glitch7collada15animation_track19CTextureTransformEx17applyBlendedValueEPvPfiS3_
int applyBlendedValue(const void* self, void*, float*, int, void*) { return {}; }

// 0x6e3ab8 _ZNK6glitch7collada15animation_track19CTextureTransformEx10applyValueEPvS3_PNS1_15CApplicatorInfoE
int applyValue(const void* self, void*, void*, glitch::collada::animation_track::CApplicatorInfo*) { return {}; }

// 0x6e3ac4 _ZN6glitch7collada15animation_track19CTextureTransformEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiiifPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, int, int, float, void*) { return {}; }

// 0x6e3b1c _ZN6glitch7collada15animation_track19CTextureTransformEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiiPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, int, void*) { return {}; }

// 0x6e3b48 _ZN6glitch7collada15animation_track19CTextureTransformEx10getValueExERKNS0_18SAnimationAccessorEiiPvRib
int getValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, void*, int&, bool) { return {}; }

// 0x6e3d08 _ZNK6glitch7collada15animation_track19CTextureTransformEx8getValueERKNS0_18SAnimationAccessorEiiPvRib
int getValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, void*, int&, bool) { return {}; }

// 0x6e3d30 _ZN6glitch7collada15animation_track19CTextureTransformEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, int, float, void*) { return {}; }

// 0x6e3d78 _ZN6glitch7collada15animation_track19CTextureTransformEx10getValueExERKNS0_18SAnimationAccessorEiPvRib
int getValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, void*, int&, bool) { return {}; }

// 0x6e3efc _ZN6glitch7collada15animation_track19CTextureTransformEx12applyValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoERib
int applyValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, void*, glitch::collada::animation_track::CApplicatorInfo*, int&, bool) { return {}; }

// 0x6e3f58 _ZNK6glitch7collada15animation_track19CTextureTransformEx10applyValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoERib
int applyValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, glitch::collada::animation_track::CApplicatorInfo*, int&, bool) { return {}; }

// 0x6e3f80 _ZNK6glitch7collada15animation_track19CTextureTransformEx8getValueERKNS0_18SAnimationAccessorEiPvRib
int getValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, int&, bool) { return {}; }

}  // namespace CTextureTransformEx
}  // namespace animation_track
}  // namespace recon_28
}  // namespace collada
}  // namespace glitch

namespace glitch {
namespace collada {
namespace recon_28 {
namespace animation_track {
namespace CVector3dEx {
// 0x6e42d0 _ZNK6glitch7collada15animation_track11CVector3dEx15getBlendedValueEPvPfiS3_
float getBlendedValue(const void* self, void*, float*, int, void*) { return {}; }

// 0x6e42e4 _ZNK6glitch7collada15animation_track11CVector3dEx13getAddedValueEPvPfiS3_
void getAddedValue(const void* self, void* vectors, float* weights, int count, void* out)
{
    const float* v = static_cast<const float*>(vectors);
    float* o = static_cast<float*>(out);
    float x = 0.0f, y = 0.0f, z = 0.0f;
    for (int i = 0; i < count; ++i) {
        const float w = weights[i];
        x = x + w * v[3 * i];
        y = y + w * v[3 * i + 1];
        z = z + w * v[3 * i + 2];
    }
    o[0] = x;
    o[1] = y;
    o[2] = z;
}

// 0x6e4390 _ZNK6glitch7collada15animation_track11CVector3dEx15getBlendedValueEPvPfiS3_f
float getBlendedValue(const void* self, void*, float*, int, void*, float) { return {}; }

// 0x6e4450 _ZN6glitch7collada15animation_track11CVector3dEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
int getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, void*) { return {}; }

// 0x6e4498 _ZNK6glitch7collada15animation_track11CVector3dEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiPvf
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, float) { return {}; }

// 0x6e4568 _ZNK6glitch7collada15animation_track11CVector3dEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
int getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*) { return {}; }

// 0x6e4578 _ZN6glitch7collada15animation_track11CVector3dEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*) { return {}; }

// 0x6e464c _ZN6glitch7collada15animation_track11CVector3dEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvf
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*, float) { return {}; }

// 0x6e4718 _ZNK6glitch7collada15animation_track11CVector3dEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPvf
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*, float) { return {}; }

// 0x6e473c _ZNK6glitch7collada15animation_track11CVector3dEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*) { return {}; }

// 0x6e4758 _ZN6glitch7collada15animation_track11CVector3dEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, void*) { return {}; }

// 0x6e47d4 _ZNK6glitch7collada15animation_track11CVector3dEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, void*) { return {}; }

// 0x6e47e8 _ZN6glitch7collada15animation_track11CVector3dEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, int, float, void*) { return {}; }

// 0x6e4904 _ZNK6glitch7collada15animation_track11CVector3dEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, int, float, void*) { return {}; }

}  // namespace CVector3dEx
}  // namespace animation_track
}  // namespace recon_28
}  // namespace collada
}  // namespace glitch

namespace glitch {
namespace collada {
namespace recon_28 {
namespace animation_track {
namespace CVisibilityEx {
// 0x6e4960 _ZNK6glitch7collada15animation_track13CVisibilityEx15getBlendedValueEPvPfiS3_
void getBlendedValue(const void* self, void* flags, float* weights, int count, void* out)
{
    const std::uint32_t* f = static_cast<const std::uint32_t*>(flags);
    std::uint32_t* o = static_cast<std::uint32_t*>(out);
    *o = 1;
    for (int i = 0; i < count; ++i) {
        if (weights[i] != 0.0f && f[i] == 0) {
            *o = 0;
            break;
        }
    }
}

// 0x6e49c4 _ZNK6glitch7collada15animation_track13CVisibilityEx13getAddedValueEPvPfiS3_
void getAddedValue(const void* self, void* flags, float* weights, int count, void* out)
{
    const std::uint32_t* f = static_cast<const std::uint32_t*>(flags);
    std::uint32_t* o = static_cast<std::uint32_t*>(out);
    *o = 0;
    for (int i = 0; i < count; ++i) {
        if (weights[i] != 0.0f && f[i] == 1) {
            *o = 1;
            break;
        }
    }
}

// 0x6e4a98 _ZN6glitch7collada15animation_track13CVisibilityEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
bool getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, void*) { return {}; }

// 0x6e4acc _ZNK6glitch7collada15animation_track13CVisibilityEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
bool getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, void*) { return {}; }

// 0x6e4adc _ZNK6glitch7collada15animation_track13CVisibilityEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
bool getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*) { return {}; }

// 0x6e4aec _ZN6glitch7collada15animation_track13CVisibilityEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
bool getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*) { return {}; }

// 0x6e4b20 _ZNK6glitch7collada15animation_track13CVisibilityEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
bool getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, int, float, void*) { return {}; }

// 0x6e4b3c _ZNK6glitch7collada15animation_track13CVisibilityEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
bool getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*) { return {}; }

// 0x6e4b6c _ZN6glitch7collada15animation_track13CVisibilityEx20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*, glitch::collada::animation_track::CApplicatorInfo*) { return {}; }

// 0x6e4bb4 _ZNK6glitch7collada15animation_track13CVisibilityEx18applyKeyBasedValueERKNS0_18SAnimationAccessorEiiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, int, float, void*, glitch::collada::animation_track::CApplicatorInfo*) { return {}; }

// 0x6e4bd8 _ZNK6glitch7collada15animation_track13CVisibilityEx18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*, glitch::collada::animation_track::CApplicatorInfo*) { return {}; }

// 0x6e4bfc _ZN6glitch7collada15animation_track13CVisibilityEx19applyBlendedValueExEPvPfiS3_
int applyBlendedValueEx(void* self, void*, float*, int, void*) { return {}; }

// 0x6e4c7c _ZNK6glitch7collada15animation_track13CVisibilityEx17applyBlendedValueEPvPfiS3_PNS1_15CApplicatorInfoE
int applyBlendedValue(const void* self, void*, float*, int, void*, glitch::collada::animation_track::CApplicatorInfo*) { return {}; }

// 0x6e4c90 _ZNK6glitch7collada15animation_track13CVisibilityEx10applyValueEPvS3_PNS1_15CApplicatorInfoE
int applyValue(const void* self, void*, void*, glitch::collada::animation_track::CApplicatorInfo*) { return {}; }

// 0x6e4cac _ZN6glitch7collada15animation_track13CVisibilityEx20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, void*, glitch::collada::animation_track::CApplicatorInfo*) { return {}; }

// 0x6e4cf4 _ZNK6glitch7collada15animation_track13CVisibilityEx18applyKeyBasedValueERKNS0_18SAnimationAccessorEiiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, void*, glitch::collada::animation_track::CApplicatorInfo*) { return {}; }

// 0x6e4d08 _ZNK6glitch7collada15animation_track13CVisibilityEx18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, glitch::collada::animation_track::CApplicatorInfo*) { return {}; }

// 0x6e4d1c _ZN6glitch7collada15animation_track13CVisibilityEx17applyAddedValueExEPvPfiS3_
int applyAddedValueEx(void* self, void*, float*, int, void*) { return {}; }

// 0x6e4da0 _ZNK6glitch7collada15animation_track13CVisibilityEx15applyAddedValueEPvPfiS3_PNS1_15CApplicatorInfoE
int applyAddedValue(const void* self, void*, float*, int, void*, glitch::collada::animation_track::CApplicatorInfo*) { return {}; }

}  // namespace CVisibilityEx
}  // namespace animation_track
}  // namespace recon_28
}  // namespace collada
}  // namespace glitch

namespace glitch {
namespace collada {
namespace recon_28 {
namespace animation_track {
namespace CWeightEx {
// 0x6e4dc8 _ZNK6glitch7collada15animation_track9CWeightEx15getBlendedValueEPvPfiS3_
float getBlendedValue(const void* self, void* values, float* weights, int count, void* out)
{
    const float* v = static_cast<const float*>(values);
    float* o = static_cast<float*>(out);
    float total = 0.0f;
    for (int i = 0; i < count; ++i) {
        total = total + v[i] * weights[i];
    }
    *o = total;
    return count > 0 ? total : 0.0f;
}

// 0x6e4e28 _ZNK6glitch7collada15animation_track9CWeightEx13getAddedValueEPvPfiS3_
float getAddedValue(const void* self, void* values, float* weights, int count, void* out)
{
    const float* v = static_cast<const float*>(values);
    float* o = static_cast<float*>(out);
    float total = 0.0f;
    for (int i = 0; i < count; ++i) {
        total = total + v[i] * weights[i];
    }
    *o = total;
    return count > 0 ? total : 0.0f;
}

// 0x6e4e88 _ZNK6glitch7collada15animation_track9CWeightEx17applyBlendedValueEPvPfiS3_PNS1_15CApplicatorInfoE
float applyBlendedValue(const void* self, void* values, float* weights, int count, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator)
{
    const float* v = static_cast<const float*>(values);
    float* o = static_cast<float*>(out);
    float total = 0.0f;
    for (int i = 0; i < count; ++i) {
        total = total + v[i] * weights[i];
    }
    *o = total;
    return count > 0 ? total : 0.0f;
}

// 0x6e4ee8 _ZNK6glitch7collada15animation_track9CWeightEx15applyAddedValueEPvPfiS3_
float applyAddedValue(const void* self, void* values, float* weights, int count, void* out)
{
    const float* v = static_cast<const float*>(values);
    float* o = static_cast<float*>(out);
    float total = 0.0f;
    for (int i = 0; i < count; ++i) {
        total = total + v[i] * weights[i];
    }
    *o = total;
    return count > 0 ? total : 0.0f;
}

// 0x6e4fbc _ZN6glitch7collada15animation_track9CWeightEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, void*) { return {}; }

// 0x6e4fec _ZNK6glitch7collada15animation_track9CWeightEx18applyKeyBasedValueERKNS0_18SAnimationAccessorEiiPvPNS1_15CApplicatorInfoE
float applyKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, void*, glitch::collada::animation_track::CApplicatorInfo*) { return {}; }

// 0x6e5000 _ZNK6glitch7collada15animation_track9CWeightEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
int getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, void*) { return {}; }

// 0x6e5028 _ZN6glitch7collada15animation_track9CWeightEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*) { return {}; }

// 0x6e5078 _ZNK6glitch7collada15animation_track9CWeightEx18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
float applyKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*, glitch::collada::animation_track::CApplicatorInfo*) { return {}; }

// 0x6e5094 _ZNK6glitch7collada15animation_track9CWeightEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, int, float, void*) { return {}; }

// 0x6e50b0 _ZNK6glitch7collada15animation_track9CWeightEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*) { return {}; }

// 0x6e50cc _ZNK6glitch7collada15animation_track9CWeightEx10applyValueEPvS3_PNS1_15CApplicatorInfoE
void* applyValue(const void* self, void*, void*, glitch::collada::animation_track::CApplicatorInfo*) { return nullptr; }

// 0x6e5110 _ZNK6glitch7collada15animation_track9CWeightEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
int getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*) { return {}; }

// 0x6e5138 _ZNK6glitch7collada15animation_track9CWeightEx18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, glitch::collada::animation_track::CApplicatorInfo*) { return {}; }

}  // namespace CWeightEx
}  // namespace animation_track
}  // namespace recon_28
}  // namespace collada
}  // namespace glitch

