// Own header for recon slice 28: glitch::scene mesh factories and the glitch::collada
// animation track applicator and accessor members. None of these members is declared in
// the layout headers, so each body is a free function here (unattached). Rows whose body
// needs data the layouts do not declare are stubs (see ledger_parts/slice_28.csv).
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/deps/video_stand_ins.hpp"
#include "glitch-scene-collada-gui/layouts/animation_track.hpp"
#include "glitch-scene-collada-gui/layouts/collada_db.hpp"

namespace glitch {
class IReferenceCounted;
}  // namespace glitch

namespace glitch::collada {
class CSceneNode;
}  // namespace glitch::collada

namespace glitch {
namespace scene {
namespace recon_28 {
// 0x6d72d0 _ZN6glitch5scene15createPlaneMeshEjPNS_5video12IVideoDriverEfRKNS1_6SColorE
glitch::IReferenceCounted* createPlaneMesh(unsigned int, glitch::video::IVideoDriver*, float, const glitch::video::SColor&);

// 0x6d7808 _ZN6glitch5scene16createSphereMeshEjPNS_5video12IVideoDriverEfjj
glitch::IReferenceCounted* createSphereMesh(unsigned int, glitch::video::IVideoDriver*, float, unsigned int, unsigned int);

// 0x6d841c _ZN6glitch5scene19createHillPlaneMeshEjPNS_5video12IVideoDriverERKNS_4core11dimension2dIfEERKNS5_IjEERKN5boost13intrusive_ptrINS1_9CMaterialEEERKNSD_INS1_27CMaterialVertexAttributeMapEEEfS8_S8_
glitch::IReferenceCounted* createHillPlaneMesh(unsigned int, glitch::video::IVideoDriver*, const glitch::core::dimension2d<float>&, const glitch::core::dimension2d<unsigned int>&, const boost::intrusive_ptr<glitch::video::CMaterial>&, const boost::intrusive_ptr<glitch::video::CMaterialVertexAttributeMap>&, float, const glitch::core::dimension2d<float>&, const glitch::core::dimension2d<float>&);

// 0x6d8cb4 _ZN6glitch5scene14createCubeMeshEjPNS_5video12IVideoDriverEf
glitch::IReferenceCounted* createCubeMesh(unsigned int, glitch::video::IVideoDriver*, float);

// 0x6d945c _ZN6glitch5scene18createCylinderMeshEjPNS_5video12IVideoDriverEffjRKNS1_6SColorEbf
glitch::IReferenceCounted* createCylinderMesh(unsigned int, glitch::video::IVideoDriver*, float, float, unsigned int, const glitch::video::SColor&, bool, float);

// 0x6d9fc4 _ZN6glitch5scene14createConeMeshEjPNS_5video12IVideoDriverEffjRKNS1_6SColorES6_f
glitch::IReferenceCounted* createConeMesh(unsigned int, glitch::video::IVideoDriver*, float, float, unsigned int, const glitch::video::SColor&, const glitch::video::SColor&, float);

// 0x6da6d8 _ZN6glitch5scene15createArrowMeshEjPNS_5video12IVideoDriverEjjffffNS1_6SColorES4_
glitch::IReferenceCounted* createArrowMesh(unsigned int, glitch::video::IVideoDriver*, unsigned int, unsigned int, float, float, float, float, glitch::video::SColor, glitch::video::SColor);

}  // namespace recon_28
}  // namespace scene
}  // namespace glitch

namespace glitch {
namespace collada {
namespace recon_28 {
namespace animation_track {
namespace CFloatEx {
// 0x6e3124 _ZNK6glitch7collada15animation_track8CFloatEx15getBlendedValueEPvPfiS3_
float getBlendedValue(const void* self, void* values, float* weights, int count, void* out);

// 0x6e3184 _ZNK6glitch7collada15animation_track8CFloatEx13getAddedValueEPvPfiS3_
float getAddedValue(const void* self, void* values, float* weights, int count, void* out);

// 0x6e3258 _ZNK6glitch7collada15animation_track8CFloatEx10applyValueEPvS3_PNS1_15CApplicatorInfoE
void* applyValue(const void* self, void*, void*, glitch::collada::animation_track::CApplicatorInfo*);

// 0x6e3288 _ZN6glitch7collada15animation_track8CFloatEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, void*);

// 0x6e32b8 _ZNK6glitch7collada15animation_track8CFloatEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, void*);

// 0x6e32cc _ZNK6glitch7collada15animation_track8CFloatEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
int getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*);

// 0x6e32f4 _ZN6glitch7collada15animation_track8CFloatEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, int, float, void*);

// 0x6e3350 _ZNK6glitch7collada15animation_track8CFloatEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, int, float, void*);

// 0x6e3374 _ZN6glitch7collada15animation_track8CFloatEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*);

// 0x6e33c4 _ZNK6glitch7collada15animation_track8CFloatEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*);

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
float getBlendedValue(const void* self, void*, float*, int, void*);

// 0x6e358c _ZNK6glitch7collada15animation_track19CTextureTransformEx13getAddedValueEPvPfiS3_
float getAddedValue(const void* self, void*, float*, int, void*);

// 0x6e38c4 _ZN6glitch7collada15animation_track19CTextureTransformEx12applyValueExEPvRNS2_5SDataEPNS1_15CApplicatorInfoE
int applyValueEx(void* self, void*, void*, glitch::collada::animation_track::CApplicatorInfo*);

// 0x6e395c _ZN6glitch7collada15animation_track19CTextureTransformEx19applyBlendedValueExEPvPfiS3_
int applyBlendedValueEx(void* self, void*, float*, int, void*);

// 0x6e3a90 _ZNK6glitch7collada15animation_track19CTextureTransformEx15applyAddedValueEPvPfiS3_
int applyAddedValue(const void* self, void*, float*, int, void*);

// 0x6e3aa4 _ZNK6glitch7collada15animation_track19CTextureTransformEx17applyBlendedValueEPvPfiS3_
int applyBlendedValue(const void* self, void*, float*, int, void*);

// 0x6e3ab8 _ZNK6glitch7collada15animation_track19CTextureTransformEx10applyValueEPvS3_PNS1_15CApplicatorInfoE
int applyValue(const void* self, void*, void*, glitch::collada::animation_track::CApplicatorInfo*);

// 0x6e3ac4 _ZN6glitch7collada15animation_track19CTextureTransformEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiiifPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, int, int, float, void*);

// 0x6e3b1c _ZN6glitch7collada15animation_track19CTextureTransformEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiiPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, int, void*);

// 0x6e3b48 _ZN6glitch7collada15animation_track19CTextureTransformEx10getValueExERKNS0_18SAnimationAccessorEiiPvRib
int getValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, void*, int&, bool);

// 0x6e3d08 _ZNK6glitch7collada15animation_track19CTextureTransformEx8getValueERKNS0_18SAnimationAccessorEiiPvRib
int getValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, void*, int&, bool);

// 0x6e3d30 _ZN6glitch7collada15animation_track19CTextureTransformEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, int, float, void*);

// 0x6e3d78 _ZN6glitch7collada15animation_track19CTextureTransformEx10getValueExERKNS0_18SAnimationAccessorEiPvRib
int getValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, void*, int&, bool);

// 0x6e3efc _ZN6glitch7collada15animation_track19CTextureTransformEx12applyValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoERib
int applyValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, void*, glitch::collada::animation_track::CApplicatorInfo*, int&, bool);

// 0x6e3f58 _ZNK6glitch7collada15animation_track19CTextureTransformEx10applyValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoERib
int applyValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, glitch::collada::animation_track::CApplicatorInfo*, int&, bool);

// 0x6e3f80 _ZNK6glitch7collada15animation_track19CTextureTransformEx8getValueERKNS0_18SAnimationAccessorEiPvRib
int getValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, int&, bool);

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
float getBlendedValue(const void* self, void*, float*, int, void*);

// 0x6e42e4 _ZNK6glitch7collada15animation_track11CVector3dEx13getAddedValueEPvPfiS3_
void getAddedValue(const void* self, void* vectors, float* weights, int count, void* out);

// 0x6e4390 _ZNK6glitch7collada15animation_track11CVector3dEx15getBlendedValueEPvPfiS3_f
float getBlendedValue(const void* self, void*, float*, int, void*, float);

// 0x6e4450 _ZN6glitch7collada15animation_track11CVector3dEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
int getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, void*);

// 0x6e4498 _ZNK6glitch7collada15animation_track11CVector3dEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiPvf
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, float);

// 0x6e4568 _ZNK6glitch7collada15animation_track11CVector3dEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
int getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*);

// 0x6e4578 _ZN6glitch7collada15animation_track11CVector3dEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*);

// 0x6e464c _ZN6glitch7collada15animation_track11CVector3dEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvf
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*, float);

// 0x6e4718 _ZNK6glitch7collada15animation_track11CVector3dEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPvf
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*, float);

// 0x6e473c _ZNK6glitch7collada15animation_track11CVector3dEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*);

// 0x6e4758 _ZN6glitch7collada15animation_track11CVector3dEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, void*);

// 0x6e47d4 _ZNK6glitch7collada15animation_track11CVector3dEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, void*);

// 0x6e47e8 _ZN6glitch7collada15animation_track11CVector3dEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, int, float, void*);

// 0x6e4904 _ZNK6glitch7collada15animation_track11CVector3dEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, int, float, void*);

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
void getBlendedValue(const void* self, void* flags, float* weights, int count, void* out);

// 0x6e49c4 _ZNK6glitch7collada15animation_track13CVisibilityEx13getAddedValueEPvPfiS3_
void getAddedValue(const void* self, void* flags, float* weights, int count, void* out);

// 0x6e4a98 _ZN6glitch7collada15animation_track13CVisibilityEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
bool getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, void*);

// 0x6e4acc _ZNK6glitch7collada15animation_track13CVisibilityEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
bool getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, void*);

// 0x6e4adc _ZNK6glitch7collada15animation_track13CVisibilityEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
bool getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*);

// 0x6e4aec _ZN6glitch7collada15animation_track13CVisibilityEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
bool getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*);

// 0x6e4b20 _ZNK6glitch7collada15animation_track13CVisibilityEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
bool getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, int, float, void*);

// 0x6e4b3c _ZNK6glitch7collada15animation_track13CVisibilityEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
bool getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*);

// 0x6e4b6c _ZN6glitch7collada15animation_track13CVisibilityEx20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*, glitch::collada::animation_track::CApplicatorInfo*);

// 0x6e4bb4 _ZNK6glitch7collada15animation_track13CVisibilityEx18applyKeyBasedValueERKNS0_18SAnimationAccessorEiiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, int, float, void*, glitch::collada::animation_track::CApplicatorInfo*);

// 0x6e4bd8 _ZNK6glitch7collada15animation_track13CVisibilityEx18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*, glitch::collada::animation_track::CApplicatorInfo*);

// 0x6e4bfc _ZN6glitch7collada15animation_track13CVisibilityEx19applyBlendedValueExEPvPfiS3_
int applyBlendedValueEx(void* self, void*, float*, int, void*);

// 0x6e4c7c _ZNK6glitch7collada15animation_track13CVisibilityEx17applyBlendedValueEPvPfiS3_PNS1_15CApplicatorInfoE
int applyBlendedValue(const void* self, void*, float*, int, void*, glitch::collada::animation_track::CApplicatorInfo*);

// 0x6e4c90 _ZNK6glitch7collada15animation_track13CVisibilityEx10applyValueEPvS3_PNS1_15CApplicatorInfoE
int applyValue(const void* self, void*, void*, glitch::collada::animation_track::CApplicatorInfo*);

// 0x6e4cac _ZN6glitch7collada15animation_track13CVisibilityEx20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, void*, glitch::collada::animation_track::CApplicatorInfo*);

// 0x6e4cf4 _ZNK6glitch7collada15animation_track13CVisibilityEx18applyKeyBasedValueERKNS0_18SAnimationAccessorEiiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, void*, glitch::collada::animation_track::CApplicatorInfo*);

// 0x6e4d08 _ZNK6glitch7collada15animation_track13CVisibilityEx18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, glitch::collada::animation_track::CApplicatorInfo*);

// 0x6e4d1c _ZN6glitch7collada15animation_track13CVisibilityEx17applyAddedValueExEPvPfiS3_
int applyAddedValueEx(void* self, void*, float*, int, void*);

// 0x6e4da0 _ZNK6glitch7collada15animation_track13CVisibilityEx15applyAddedValueEPvPfiS3_PNS1_15CApplicatorInfoE
int applyAddedValue(const void* self, void*, float*, int, void*, glitch::collada::animation_track::CApplicatorInfo*);

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
float getBlendedValue(const void* self, void* values, float* weights, int count, void* out);

// 0x6e4e28 _ZNK6glitch7collada15animation_track9CWeightEx13getAddedValueEPvPfiS3_
float getAddedValue(const void* self, void* values, float* weights, int count, void* out);

// 0x6e4e88 _ZNK6glitch7collada15animation_track9CWeightEx17applyBlendedValueEPvPfiS3_PNS1_15CApplicatorInfoE
float applyBlendedValue(const void* self, void* values, float* weights, int count, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator);

// 0x6e4ee8 _ZNK6glitch7collada15animation_track9CWeightEx15applyAddedValueEPvPfiS3_
float applyAddedValue(const void* self, void* values, float* weights, int count, void* out);

// 0x6e4fbc _ZN6glitch7collada15animation_track9CWeightEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, void*);

// 0x6e4fec _ZNK6glitch7collada15animation_track9CWeightEx18applyKeyBasedValueERKNS0_18SAnimationAccessorEiiPvPNS1_15CApplicatorInfoE
float applyKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, void*, glitch::collada::animation_track::CApplicatorInfo*);

// 0x6e5000 _ZNK6glitch7collada15animation_track9CWeightEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
int getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, void*);

// 0x6e5028 _ZN6glitch7collada15animation_track9CWeightEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float getKeyBasedValueEx(void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*);

// 0x6e5078 _ZNK6glitch7collada15animation_track9CWeightEx18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
float applyKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*, glitch::collada::animation_track::CApplicatorInfo*);

// 0x6e5094 _ZNK6glitch7collada15animation_track9CWeightEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, int, float, void*);

// 0x6e50b0 _ZNK6glitch7collada15animation_track9CWeightEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, int, float, void*);

// 0x6e50cc _ZNK6glitch7collada15animation_track9CWeightEx10applyValueEPvS3_PNS1_15CApplicatorInfoE
void* applyValue(const void* self, void*, void*, glitch::collada::animation_track::CApplicatorInfo*);

// 0x6e5110 _ZNK6glitch7collada15animation_track9CWeightEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
int getKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*);

// 0x6e5138 _ZNK6glitch7collada15animation_track9CWeightEx18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue(const void* self, const glitch::collada::SAnimationAccessor&, int, void*, glitch::collada::animation_track::CApplicatorInfo*);

}  // namespace CWeightEx
}  // namespace animation_track
}  // namespace recon_28
}  // namespace collada
}  // namespace glitch

