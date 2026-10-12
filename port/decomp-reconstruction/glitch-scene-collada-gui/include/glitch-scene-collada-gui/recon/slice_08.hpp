// Slice 08 reconstruction (wave 8, worker 08).
//
// Contents:
//   - getInstance of the CVirtualEx<CApplyValueEx<...>> instantiations: a
//     function-local static instance, returned by address. One template covers
//     all 39 instantiations; the applicator tag of each is not mapped yet.
//   - Key-based getKeyBasedValue / getKeyBasedValueEx / applyKeyBasedValue
//     paths for float[4], vector3d<float> (position and scale) and float[3]
//     applicators, plus the CLightColor (unsigned char[3]) key path.
//   - CColladaDatabase construct helpers (skin, controller, image).
//
// The 38 D0 deleting destructors of this slice (12-byte bodies, operator delete
// through LC_API_FREE_0) are compiler-generated thunks and are not rebuilt.
//
// Names follow the original address: getKeyBasedValue_612128 is the body at
// 0x612128. Parameters follow the Hex-Rays pseudocode; the `this` pointer that
// the decompiler leaves unused is dropped.
#pragma once

#include <cstddef>
#include <cstdint>

#include "glitch-scene-collada-gui/animation_track_virtual_ex.hpp"
#include "glitch-scene-collada-gui/collada_database.hpp"
#include "glitch-scene-collada-gui/deps/video_stand_ins.hpp"

namespace glitch {
class IReferenceCounted;
}

namespace glitch::collada {
class CRootSceneNode;
struct SAnimationAccessor;  // declared by the animation accessor module
namespace animation_track {
class CApplicatorInfo;
}  // namespace animation_track
}  // namespace glitch::collada

namespace glitch::collada::recon_08 {

// ---------------------------------------------------------------------------
// Accessor output. In the original, getOutput(accessor, 0) returns an object
// whose word at +0x04 is the key buffer. Only that word is mapped here.
// The accessor module defines getOutput; nothing in the tree declares it yet.
// ---------------------------------------------------------------------------
struct SAnimationOutput {
    std::uint32_t reserved00;  // +0x00
    std::uint8_t* keys;        // +0x04, key buffer
};

// unattached: needs the SAnimationAccessor::getOutput declaration in the accessor header.
SAnimationOutput* getOutput(const SAnimationAccessor& accessor, int index);

// Key element types. Original strides: 0x10 (four floats), 0x0c (three floats),
// 0x03 (three bytes for CLightColorMixin<unsigned char>).
struct Key4f {
    float v[4];
};
struct Key3f {
    float v[3];
};
struct KeyRgb {
    std::uint8_t v[3];
};

// The original returns the output object as an int from the copy-style paths.
inline int outputHandle(const SAnimationOutput* output) {
    return static_cast<int>(reinterpret_cast<std::intptr_t>(output));
}

// ---------------------------------------------------------------------------
// getInstance: the function-local static of each CVirtualEx instantiation. The
// original guards the static with __cxa_guard and registers the destructor with
// atexit; a function-local static gives the same single instance.
// Addresses: 0x60ffb4 .. 0x611a4c, 39 instantiations.
// unattached: each instantiation needs its member declaration in
// animation_track_virtual_ex.hpp; the applicator tag is not mapped.
// ---------------------------------------------------------------------------
template <typename Applicator>
animation_track::CVirtualEx<Applicator>* getInstance_CVirtualEx() {
    static animation_track::CVirtualEx<Applicator> instance;
    return &instance;
}

// ---------------------------------------------------------------------------
// Key-based paths, float[4] mixin (CMixin<float,4,...,-1,float>).
// ---------------------------------------------------------------------------
// 0x612128 getKeyBasedValue(SAnimationAccessor const&, int, void*) const
int getKeyBasedValue_612128(const SAnimationAccessor& accessor, int index, void* out);
// 0x612174 getKeyBasedValue(SAnimationAccessor const&, int, int, void*) const
float getKeyBasedValue_612174(const SAnimationAccessor& accessor, int from, int to, void* out);
// 0x6121c0 CInterpreter<...float[4]...>::getKeyBasedValueEx(accessor, int, int, int, float, void*)
float getKeyBasedValueEx_6121c0(const SAnimationAccessor& accessor, int sub, int from, int to,
                                float t, void* out);
// 0x612238 getKeyBasedValue(SAnimationAccessor const&, int, int, int, float, void*) const
float getKeyBasedValue_612238(const SAnimationAccessor& accessor, int sub, int from, int to,
                              float t, void* out);

// ---------------------------------------------------------------------------
// Key-based paths, vector3d<float> with CSceneNodePositionMixin<float>.
// ---------------------------------------------------------------------------
// 0x61225c getKeyBasedValue(SAnimationAccessor const&, int, void*) const
int getKeyBasedValue_61225c(const SAnimationAccessor& accessor, int index, void* out);
// 0x6122a4 getKeyBasedValue(SAnimationAccessor const&, int, int, void*) const
float getKeyBasedValue_6122a4(const SAnimationAccessor& accessor, int from, int to, void* out);
// 0x6122f4 CInterpreter<CMixin<float,3,SSceneNodePosition,...>>::getKeyBasedValueEx
float getKeyBasedValueEx_6122f4(const SAnimationAccessor& accessor, int sub, int from, int to,
                                float t, void* out);
// 0x612370 getKeyBasedValue(SAnimationAccessor const&, int, int, int, float, void*) const
float getKeyBasedValue_612370(const SAnimationAccessor& accessor, int sub, int from, int to,
                              float t, void* out);

// ---------------------------------------------------------------------------
// Key-based paths, vector3d<float> with CSceneNodeScaleMixin<float>.
// ---------------------------------------------------------------------------
// 0x612394 getKeyBasedValue(SAnimationAccessor const&, int, void*) const
int getKeyBasedValue_612394(const SAnimationAccessor& accessor, int index, void* out);
// 0x6123dc getKeyBasedValue(SAnimationAccessor const&, int, int, void*) const
float getKeyBasedValue_6123dc(const SAnimationAccessor& accessor, int from, int to, void* out);
// 0x61242c CInterpreter<CSceneNodeScaleMixin<float>,float,3,...>::getKeyBasedValueEx
float getKeyBasedValueEx_61242c(const SAnimationAccessor& accessor, int sub, int from, int to,
                                float t, void* out);
// 0x6124a8 getKeyBasedValue(SAnimationAccessor const&, int, int, int, float, void*) const
float getKeyBasedValue_6124a8(const SAnimationAccessor& accessor, int sub, int from, int to,
                              float t, void* out);

// ---------------------------------------------------------------------------
// Key-based paths, float[3] with CMixin<float,3,SMaterialSetParam<...>,-1,float>.
// ---------------------------------------------------------------------------
// 0x6124cc getKeyBasedValue(SAnimationAccessor const&, int, void*) const
int getKeyBasedValue_6124cc(const SAnimationAccessor& accessor, int index, void* out);
// 0x612514 getKeyBasedValue(SAnimationAccessor const&, int, int, void*) const
float getKeyBasedValue_612514(const SAnimationAccessor& accessor, int from, int to, void* out);
// 0x612564 CInterpreter<CMixin<float,3,...,-1,float>,float,3,...>::getKeyBasedValueEx
float getKeyBasedValueEx_612564(const SAnimationAccessor& accessor, int sub, int from, int to,
                                float t, void* out);
// 0x6125e0 getKeyBasedValue(SAnimationAccessor const&, int, int, int, float, void*) const
float getKeyBasedValue_6125e0(const SAnimationAccessor& accessor, int sub, int from, int to,
                              float t, void* out);

// ---------------------------------------------------------------------------
// CLightColorMixin<unsigned char> (vector3d<float> applicator, 3-byte keys).
// ---------------------------------------------------------------------------
// unattached: applyValueEx of CApplicator<SLightColor> is not declared anywhere.
float applyValueEx_SLightColor(void* applicator, const std::uint8_t* value);
// 0x612604 CApplyValueEx<vector3d<float>,CLightColorMixin<unsigned char>>::applyKeyBasedValueEx
float applyKeyBasedValueEx_612604(const SAnimationAccessor& accessor, int index, void* applicator);
// 0x612670 applyKeyBasedValue(SAnimationAccessor const&, int, void*, CApplicatorInfo*) const
float applyKeyBasedValue_612670(const SAnimationAccessor& accessor, int index, void* applicator,
                                animation_track::CApplicatorInfo* info);
// 0x612684 getKeyBasedValue(SAnimationAccessor const&, int, void*) const
int getKeyBasedValue_612684(const SAnimationAccessor& accessor, int index, void* out);

// ---------------------------------------------------------------------------
// glitch::IReferenceCounted and CColladaDatabase helpers. The count is the word
// at +0x04. The collaborators below are not declared in any header yet.
// ---------------------------------------------------------------------------
// unattached: IReferenceCounted::grab (++count at +0x04) is not declared.
void grabReference(glitch::IReferenceCounted* object);
// unattached: IReferenceCounted::drop is not declared.
void dropReference(glitch::IReferenceCounted* object);
// unattached: CRootSceneNode::attachSkin is not declared.
void attachSkin(CRootSceneNode* root, glitch::IReferenceCounted* skin);
// unattached: the skin factory is the object at database +0x04; its create slot is +0x58.
glitch::IReferenceCounted* createSkinFromFactory(const void* database, glitch::video::IVideoDriver* driver,
                                                 SController* controller, CRootSceneNode* root);
// unattached: the controller lookup used by constructController is not declared; the
// pseudocode passes the driver and root argument pair to getController.
SController* lookupControllerByOwner(const glitch::video::IVideoDriver* driver, const CRootSceneNode* root);
// unattached: the (driver, int, controller, root) overload of constructController is
// not declared in any header; it is the next function in the chain.
glitch::IReferenceCounted** constructControllerFromLookup(glitch::IReferenceCounted** out,
                                                          glitch::video::IVideoDriver* driver, int index,
                                                          SController* controller,
                                                          CRootSceneNode* owner);
// unattached: CColladaDatabase::getImage(int) overload is not declared.
SImage* lookupImage(const void* database, int index);
// unattached: the CImage constructor and operator new are not declared.
glitch::IReferenceCounted* newImage(const void* root, const SImage* image);

// 0x60f924 CColladaDatabase::constructSkin(glitch::video::IVideoDriver*, SController*, CRootSceneNode*) const
// The second argument is the database; the receiver of the factory call is its +0x04 member.
glitch::IReferenceCounted** constructSkin_60f924(glitch::IReferenceCounted** out,
                                                 const void* database, glitch::video::IVideoDriver* driver,
                                                 SController* controller, CRootSceneNode* root);
// 0x60fa70 CColladaDatabase::constructController(glitch::video::IVideoDriver*, int, CRootSceneNode*) const
// Pseudocode arguments in order: driver, index, root, owner. The decompiler types the
// second and fourth arguments as drivers and roots; the demangled name lists three.
glitch::IReferenceCounted** constructController_60fa70(glitch::IReferenceCounted** out,
                                                       glitch::video::IVideoDriver* driver, int index,
                                                       CRootSceneNode* root,
                                                       CRootSceneNode* owner);
// 0x60fc70 CColladaDatabase::constructImage(SImage*, CRootSceneNode*) const
// Pseudocode argument order: root, then image (the image is tested for null first).
glitch::IReferenceCounted** constructImage_60fc70(glitch::IReferenceCounted** out,
                                                  const void* root, const SImage* image);
// 0x60fcc0 CColladaDatabase::constructImage(int, CRootSceneNode*) const
glitch::IReferenceCounted** constructImage_60fcc0(glitch::IReferenceCounted** out,
                                                  const void* database, int index);

}  // namespace glitch::collada::recon_08
