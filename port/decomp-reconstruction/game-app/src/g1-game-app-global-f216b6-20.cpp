// Batch g1-game-app-global-f216b6-20 (<global>, part 21 of 22): the scene-node queries and projections
// (0x50e568..0x50f89c), the orientation, display, music, native-resource and accelerometer glue
// (0x530a6c..0x532f14) and the PVRTC colour and twiddle helpers (0x69f570..0x69f768).
// Declarations: dh2/global/scene_nodes.hpp, dh2/global/platform_app.hpp and dh2/global/pvrtc_decompress.hpp.

#include "dh2/global/pvrtc_decompress.hpp"
#include "dh2/global/platform_app.hpp"
#include "dh2/global/scene_nodes.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "dh2/Application.hpp"
#include "dh2/global/Globals.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "glitch-core-io/os/device.hpp"
#include "vox-audio/vox/sound_manager.hpp"

#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"
#include "glitch-scene-collada-gui/scene/IMesh.hpp"
#include "glitch-scene-collada-gui/scene/CMeshSceneNode.hpp"
#include "glitch-scene-collada-gui/scene/mesh_functions.hpp"
#include "glitch-scene-collada-gui/scene/CSceneManager.hpp"
#include "glitch-scene-collada-gui/scene/CSceneCollisionManager.hpp"

namespace {

// ARM register-specified shifts: an amount of 32 or more gives zero. The original shifts by registers.
std::uint32_t ArmLogicalShiftRight(std::uint32_t value, int amount) {
    return amount >= 32 ? 0u : value >> amount;
}

std::uint32_t ArmLogicalShiftLeft(std::uint32_t value, int amount) {
    return amount >= 32 ? 0u : value << amount;
}

}  // namespace

// 0x50e568 GetNodePolyCount(glitch::scene::ISceneNode*, bool)
// PARTIAL: stub returns 0. The original adds, for a mesh node, the triangle count of its mesh: the node's virtual
// at vtable +0xf8 gives the IMesh (an IReferenceCounted), the mesh's vtable +0x10 gives the buffer count, its
// vtable +0x14 gives buffer i, and the buffer's word at +0x20 divided by 3 is the triangle count (each buffer is
// dropped after use). With the flag set it then adds GetNodePolyCount(child, true) for every child on the node's
// children list (an intrusive list whose sentinel is at node +0xf4; the element is the link address minus 4).
// Blocked: ISceneNode has no vtable declared, the children list layout disagrees with the engine header (which maps
// a std::vector at +0xf0), and IMesh / IReferenceCounted buffer accessors are not declared (layout requests).
int GetNodePolyCount(glitch::scene::ISceneNode* node, bool includeChildren) {
    (void)node;
    (void)includeChildren;
    return 0;
}

// 0x50e674 IsNodeVisible(glitch::scene::ISceneNode*)
// The original body is a single "return 1" (asm: MOV R0,#1; BX LR).
int IsNodeVisible(glitch::scene::ISceneNode* node) {
    (void)node;
    return 1;
}

// 0x50e67c IsVisibleMeshNode(glitch::scene::ISceneNode*)
int IsVisibleMeshNode(glitch::scene::ISceneNode* node) {
    int result = IsNodeVisible(node);
    if (result) {
        return IsNodeMesh(node);
    }
    return result;
}

// 0x50e830 GetScreenPosFromWorldPos(glitch::core::vector3d<float> const&, glitch::core::position2d<int>&)
// Reconstructed from the listing. The scene manager is the device's +0x1c member
// (Singleton<Application>::s_inst.m_device, declared in glitch-core-io/os/device.hpp); the world point goes to its
// collision manager (+0x2c, CSceneManager::collisionManager), whose virtual slot +0x18 is
// ISceneCollisionManager::getScreenCoordinatesFrom3DPosition(position, camera), called with a null camera (the
// active one). The original's position2d result is written through a hidden pointer and then copied to the out
// parameter, and r0 on return is that hidden pointer (the address of the callee's temporary); every caller in the
// tree drops it, so the port returns the first word it wrote.
int GetScreenPosFromWorldPos(const glitch::core::vector3d<float>& worldPos,
                             glitch::core::position2d<int>& screenPos) {
    glitch::scene::CSceneManager* scene =
        reinterpret_cast<glitch::scene::CSceneManager*>(ApplicationGetInstance().m_device->sceneManager);
    const glitch::core::position2d<std::int32_t> projected =
        scene->collisionManager->getScreenCoordinatesFrom3DPosition(worldPos, nullptr);
    screenPos = projected;
    return projected.X;
}

// 0x50ed8c GetWorldPosFromScreenPos(Point2D<float> const&, Point3D<float>&, float)
// Reconstructed from the listing. The screen point is truncated to ints and the collision manager of the scene
// manager (device +0x1c, scene +0x2c) gives the world ray through it: virtual slot +0x14,
// ISceneCollisionManager::getRayFromScreenCoordinates(position, camera) with a null camera. dir = end - start is
// normalised in place, t = start.Z / dir.Z, world = start - t * dir, and the result is world.Z. The third argument
// is never read by the original.
float GetWorldPosFromScreenPos(const Point2D<float>& screenPos, Point3D<float>& worldPos, float unusedArg) {
    (void)unusedArg;
    glitch::scene::CSceneManager* scene =
        reinterpret_cast<glitch::scene::CSceneManager*>(ApplicationGetInstance().m_device->sceneManager);
    const glitch::core::position2d<std::int32_t> position{static_cast<std::int32_t>(screenPos.X),
                                                          static_cast<std::int32_t>(screenPos.Y)};
    const glitch::core::line3d<float> ray = scene->collisionManager->getRayFromScreenCoordinates(position, nullptr);
    glitch::core::vector3d<float> dir{ray.end.X - ray.start.X, ray.end.Y - ray.start.Y, ray.end.Z - ray.start.Z};
    dir.normalize();
    const float t = ray.start.Z / dir.Z;
    worldPos.X = ray.start.X - t * dir.X;
    worldPos.Y = ray.start.Y - t * dir.Y;
    worldPos.Z = ray.start.Z - t * dir.Z;
    return worldPos.Z;
}

// 0x50f1d4 IsVisibleAndNotCulled(glitch::scene::ISceneNode*)
// Reconstructed from the listing: IsNodeVisible (always 1) first, then the inverted CSceneManager::isCulled of the
// device's +0x1c scene manager (asm: BL isCulled, EOR R0,R0,#1).
int IsVisibleAndNotCulled(glitch::scene::ISceneNode* node) {
    const int visible = IsNodeVisible(node);
    if (visible) {
        glitch::scene::CSceneManager* scene =
            reinterpret_cast<glitch::scene::CSceneManager*>(ApplicationGetInstance().m_device->sceneManager);
        return static_cast<int>(scene->isCulled(node)) ^ 1;
    }
    return visible;
}

// 0x50f780 findSceneNodeRef(glitch::scene::ISceneNode*, char const*)
// The original takes the node's name through its virtual at vtable +0x24 (ISceneNode::getName, slot 0x24 in the engine
// header). When strcmp(name, the argument) is zero the node itself is returned. Otherwise it walks the child list (the
// sentinel at node +0xf4, which is ISceneNode::children; a child is its sibling link minus 4) and returns the first
// non-null recursive result, or null when no child matches. The children are read through the member, not through
// getChildren(), so this translation unit needs no out-of-line engine symbol.
glitch::scene::ISceneNode* findSceneNodeRef(glitch::scene::ISceneNode* root, const char* name) {
    if (std::strcmp(root->getName(), name) == 0) {
        return root;
    }
    for (auto it = root->children.begin(); it != root->children.end(); ++it) {
        glitch::scene::ISceneNode* found = findSceneNodeRef(*it, name);
        if (found != nullptr) {
            return found;
        }
    }
    return nullptr;
}

// 0x50f89c CopyMeshSceneNode(glitch::scene::IMeshSceneNode*)
// Reconstructed from the listing. The node's mesh (IMeshSceneNode vtable +0xf8, getMesh) is copied with
// glitch::scene::createMeshCopy(mesh, driver, 0xffffffff, 0), where the driver is the device's +0x10 videoDriver
// (the earlier note's "three-argument overload" and "IDevice declares no +0x10" are both stale:
// scene/mesh_functions.hpp declares createMeshCopy(mesh, driver, mask, extra) and glitch-core-io/os/device.hpp
// declares videoDriver). The three node properties are read through their virtuals (getPosition +0xa0,
// getRotation +0x98, getScale +0x90), a 320-byte CMeshSceneNode is constructed from (copy, -1, position, rotation,
// scale), and its setName (+0x28) receives the parent's getName (+0x24, getParent is non-virtual). The original's
// explicit intrusive_ptr grabs and drops around the constructor are reproduced by the port's intrusive_ptr locals,
// which hold the same references; the new node is returned (the caller casts it to CMeshSceneNode*).
void* CopyMeshSceneNode(glitch::scene::IMeshSceneNode* node) {
    const boost::intrusive_ptr<glitch::scene::IMesh> mesh = node->getMesh();  // vtable +0xf8
    const boost::intrusive_ptr<glitch::scene::IMesh> copy =
        glitch::scene::createMeshCopy(mesh, ApplicationGetInstance().m_device->videoDriver, 0xffffffffu, 0);
    glitch::scene::CMeshSceneNode* newNode = new glitch::scene::CMeshSceneNode(
        copy, -1, node->getPosition(), node->getRotation(), node->getScale());
    newNode->setName(node->getParent()->getName());  // +0x28 with the parent's +0x24
    return newNode;
}

// 0x530a6c SetFinalOrientation(int)
// PARTIAL: stub returns 0. Body verified against asm 0x530a6c, and the two remaining items are both re-confirmed:
//  (1) the two stores go into TouchScreenBase::orientation_ (+0x1a8), which core-platform/touch_screen_base.hpp:83
//      declares in the protected section (line 74) with no public setter, so this free function cannot write it; a
//      core-platform header edit, outside this batch.
//  (2) IVideoDriver::setOrientation (glitch-video/video_driver.hpp, +0x13c) returns void. The original's result is
//      NOT void: IVideoDriver::setOrientation (0x5a9fd4) returns the virtual call at the object +0xc8's vtable +0x0c
//      when the orientation changed and returns `this` when it did not, and SetFinalOrientation returns that r0.
// The rest of the old list is stale and available: SavegameManager::getAutoReorientation (SavegameManager.hpp,
// reached as *(app)+0x4c = Application::m_savegameManager), the touch screen at Application +0x20, the device's
// driver at IDevice +0x10, lastOpenMenuID (0x9f63fc) and isScreenOriented (0x99b100) in dh2/global/Globals.hpp.
extern "C" int SetFinalOrientation(int orientation) {
    (void)orientation;
    return 0;
}

// 0x530b5c SetDisplayString(char*)
// VoxSoundManager::ShowPlayingMusicTitle(VoxSoundManager::s_instance, title).
extern "C" void SetDisplayString(char* title) {
    VoxSoundManager::s_instance->ShowPlayingMusicTitle(title);
}

// 0x530b80 GetGameMusicVolume()
// Asm: loads the address of Singleton<Application>::s_inst and the string "VolumeMusic", then calls
// Application::GetSavedOption on it. ApplicationGetInstance() is that address (dh2/global/ctrl_camera.hpp; its
// definition belongs to the integrator). GetSavedOption is declared in dh2/Application.hpp.
extern "C" int GetGameMusicVolume() {
    return ApplicationGetInstance().GetSavedOption("VolumeMusic");
}

// 0x531228 GetNativeSoundRaw()
// The original is a bare return (ELF size 4, BX LR).
extern "C" void GetNativeSoundRaw() {}

// 0x53122c GetNativeResourceSoundRawLength()
// The original is a bare return (ELF size 4, BX LR).
extern "C" void GetNativeResourceSoundRawLength() {}

// 0x53226c GetNativeResourceFull(int, int)
// PARTIAL: stub, nothing is copied. The original is Android JNI glue on the GLResLoader class (mClassGLResLoader),
// with the JNIEnv in mEnv. Its calls, by function-table byte offset (standard JNINativeInterface index; no jni.h is in
// the tree to check them): NewStringUTF(name) (+0x29c), CallStaticObjectMethod(loader, getResFullID, jstring)
// (+0x1c8), GetArrayLength(array) (+0x2ac), GetByteArrayRegion(array, 0, length, out) (+0x320), then two
// DeleteLocalRef (+0x5c). The return is the leftover of the last DeleteLocalRef, so the function is void.
// Blocked: mEnv, mClassGLResLoader and getResFullID are not declared, and no JNI header is in the tree.
extern "C" void GetNativeResourceFull(const char* name, void* out) {
    (void)name;
    (void)out;
}

// 0x532350 GetNativeResourceLength(int)
// PARTIAL: stub returns 0. The original calls NewStringUTF(name) (+0x29c), then
// CallStaticIntMethod(loader, getResLengthID, jstring) (+0x204), then DeleteLocalRef (+0x5c), and returns the int.
// Blocked on the same JNI state as GetNativeResourceFull.
extern "C" int GetNativeResourceLength(const char* name) {
    (void)name;
    return 0;
}

// 0x5323d8 GetNativeResourceBytes(int, int, size_t)
// PARTIAL: stub returns nullptr. The original calls NewStringUTF(name), then CallStaticObjectMethod(loader,
// getResBytesID, jstring, argument, length) (+0x1c8) for a byte array, allocates malloc(length), fills it with
// GetByteArrayRegion(array, 0, length, buffer) (+0x320), releases the array and the string with DeleteLocalRef
// (+0x5c) and returns the buffer. Blocked on the same JNI state as GetNativeResourceFull. The meaning of the second
// Java argument is not recovered.
extern "C" void* GetNativeResourceBytes(const char* name, int argument, std::size_t length) {
    (void)name;
    (void)argument;
    (void)length;
    return nullptr;
}

// 0x532f14 GetAccelerometerParams(int*, int*, int*)
// The original copies mAccelerometerX (ELF 0xa354f4) into the first destination, mAccelerometerY (0xa354f8) into the
// second and mAccelerometerZ (0xa354f0) into the third, then returns mAccelerometerY (Hex-Rays: result = mAccelerometerY,
// returned after the copies). The globals are 32-bit unsigned words (Globals.hpp, from the accesses); the destinations
// are the signed words this header declares, and the conversions keep the bits.
extern "C" int GetAccelerometerParams(std::int32_t* x, std::int32_t* y, std::int32_t* z) {
    *x = static_cast<std::int32_t>(mAccelerometerX);
    const std::int32_t yWord = static_cast<std::int32_t>(mAccelerometerY);
    *y = yWord;
    *z = static_cast<std::int32_t>(mAccelerometerZ);
    return yWord;
}

// 0x69f570 InterpolateColours(int const*, int const*, int const*, int const*, int, int, int, int*)
// Internal linkage in the original. The four colour words of each input (a1 to a4) are copied into a 16-word
// scratch array: corner[k] = a4[k], corner[4+k] = a3[k], corner[8+k] = a2[k], corner[12+k] = a1[k]. The horizontal
// weight v14 and the result scale come from a5 (8 steps when set, 4 otherwise) and a6; the vertical weight s7 comes
// from a7. Each a8[k] is the blend of its corners. Then a8[0..3] are shifted down (by 2, 2, 2, 1 for 8 steps, and by
// 1, 1, 1 for 4 steps, leaving a8[3] alone), and each a8[k] gets its own value shifted by 5 (by 4 for a8[3]) added
// back. The return is the new a8[0] for 8 steps and 4 for 4 steps.
int InterpolateColours(const int* a1, const int* a2, const int* a3, const int* a4, int a5, int a6, int a7,
                       int* a8) {
    int corner[16];
    for (int i = 0; i != 4; ++i) {
        corner[i] = a4[i];
        corner[4 + i] = a3[i];
        corner[8 + i] = a2[i];
        corner[12 + i] = a1[i];
    }

    int result = 0;
    int v14 = 0;
    if (a5) {
        result = 8;
        v14 = ((a6 & 7) | (2 * (~a6 & 4))) - 4;
    } else {
        result = 4;
        v14 = ((a6 & 3) | (2 * (~a6 & 2))) - 2;
    }
    const int s7 = ((a7 & 3) | (2 * (~a7 & 2))) - 2;

    for (int j = 0; j != 4; ++j) {
        const int v16 = corner[12 + j] * result + (corner[8 + j] - corner[12 + j]) * v14;
        a8[j] = s7 * (corner[4 + j] * result + (corner[j] - corner[4 + j]) * v14 - v16) + 4 * v16;
    }

    if (a5) {
        const int v20 = a8[1] >> 2;
        result = a8[0] >> 2;
        const int v21 = a8[2] >> 2;
        const int v22 = a8[3] >> 1;
        a8[0] = result;
        a8[1] = v20;
        a8[2] = v21;
        a8[3] = v22;
    } else {
        const int v17 = a8[1] >> 1;
        const int v18 = a8[2] >> 1;
        a8[0] >>= 1;
        a8[1] = v17;
        a8[2] = v18;
    }

    for (int k = 0; k != 3; ++k) {
        a8[k] += a8[k] >> 5;
    }
    a8[3] += a8[3] >> 4;
    return result;
}

// 0x69f6f4 TwiddleUV(unsigned long, unsigned long, unsigned long, unsigned long)
// Internal linkage in the original. If a1 < a2 the second value becomes a4 (a1 keeps its value); otherwise the first
// value takes the second. When the original a1 >= a2 held, the second value then becomes a3. With a1 > 1 the bits of
// a3 (x) and a4 (y) are interleaved into a Morton index, two bits per step, and the step count shifts the second value
// ((a2 >> steps) << 2 * steps) into the index. For a1 <= 1 the index is 0 and the result is the second value.
std::uint32_t TwiddleUV(std::uint32_t a1, std::uint32_t a2, std::uint32_t a3, std::uint32_t a4) {
    const bool noBorrow = a1 >= a2;
    if (a1 < a2) {
        a2 = a4;
    } else {
        a1 = a2;
    }
    if (noBorrow) {
        a2 = a3;
    }

    std::uint32_t bits = 0;
    int steps = 0;
    int spread = 0;
    if (a1 > 1) {
        std::uint32_t weight = 1;
        std::uint32_t mask = 1;
        for (;;) {
            if ((mask & a3) != 0) {
                bits |= weight;
            }
            const bool yClear = (mask & a4) == 0;
            mask *= 2;
            if (!yClear) {
                bits |= 2 * weight;
            }
            ++steps;
            if (a1 <= mask) {
                break;
            }
            weight *= 4;
        }
        spread = 2 * steps;
    } else {
        spread = 0;
        bits = 0;
        steps = 0;
    }
    return bits | ArmLogicalShiftLeft(ArmLogicalShiftRight(a2, steps), spread);
}

// 0x69f768 PVRTCDecompress(void const*, int, int, int, unsigned char*)
// PARTIAL: the pixel decode is not rebuilt, so nothing is written to result. What is reconstructed: the block size
// (8 bytes when do2bitMode is set, else 4), the start value xDim / block size (at least 2), and the return value.
// The original returns yDim after its row loop when yDim > 0, and the start value otherwise. The loop itself
// addresses blocks with TwiddleUV, blends with InterpolateColours, and reads modulation values from the static tables
// RepVals0 and RepVals1 of GetModulationValue (ELF 0x8eaf00 and 0x8eaf10, four words each). GetModulationValue is not
// in this batch or the tree, and the scratch buffers alias through byte offsets, so the decode is not written.
int PVRTCDecompress(const void* compressed, int do2bitMode, int xDim, int yDim, std::uint8_t* result) {
    (void)compressed;
    (void)result;
    const int blockBytes = do2bitMode ? 8 : 4;
    int startValue = xDim / blockBytes;
    if (startValue < 2) {
        startValue = 2;
    }
    if (yDim > 0) {
        return yDim;
    }
    return startValue;
}
