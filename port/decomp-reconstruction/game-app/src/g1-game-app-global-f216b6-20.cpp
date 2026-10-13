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

#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"

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
// PARTIAL: stub returns 0 and writes nothing. The original copies the world point into a three-word array, calls the
// camera's virtual at vtable +0x18 as (out[2] words, camera, point array, 0), stores the two words of out as the
// screen position, and returns the call's result. The camera is the scene manager's word at +0x2c, and the scene
// manager is the word at +0x1c of the device (dword_99F73C, Singleton<Application>::s_inst.m_device). Blocked: glitch::IDevice
// (glitch-core-io/os/device.hpp) declares neither the +0x1c scene manager nor the +0x10 driver. The +0x2c camera is not the
// engine CSceneManager's activeCamera (that field is at +0xe4), and CameraBase declares no virtual at +0x18 (layout requests:
// IDevice +0x1c, the scene manager's +0x2c class and its slot +0x18).
int GetScreenPosFromWorldPos(const glitch::core::vector3d<float>& worldPos,
                             glitch::core::position2d<int>& screenPos) {
    (void)worldPos;
    (void)screenPos;
    return 0;
}

// 0x50ed8c GetWorldPosFromScreenPos(Point2D<float> const&, Point3D<float>&, float)
// PARTIAL: stub returns 0 and writes nothing. The original converts the screen position to int (truncating), asks
// the camera for the near and far points of the ray through it (its virtual at vtable +0x14, with the same camera
// as GetScreenPosFromWorldPos), normalises dir = far - near, and intersects the ray with the z = 0 plane:
// t = near.z / dir.z, world = near - t * dir, and returns world.z. The third float argument is never read.
// Blocked on the same device and camera layout as GetScreenPosFromWorldPos.
float GetWorldPosFromScreenPos(const Point2D<float>& screenPos, Point3D<float>& worldPos, float unusedArg) {
    (void)screenPos;
    (void)worldPos;
    (void)unusedArg;
    return 0.0f;
}

// 0x50f1d4 IsVisibleAndNotCulled(glitch::scene::ISceneNode*)
// PARTIAL: the visible branch is a placeholder. The original returns !CSceneManager::isCulled(scene, node), where
// scene is the device's +0x1c scene manager (layout request). IsNodeVisible is always 1, so the culling test is the
// whole result. Until the scene manager is declared, the placeholder reports the node as culled (returns 0).
int IsVisibleAndNotCulled(glitch::scene::ISceneNode* node) {
    int result = IsNodeVisible(node);
    if (result) {
        return 0;
    }
    return result;
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
// PARTIAL: stub returns nullptr. The original takes the node's mesh through the node's virtual at vtable +0xf8,
// copies it with glitch::scene::createMeshCopy(mesh, device's +0x10 driver, -1, 0) (five arguments; the tree declares
// only a three-argument overload), reads three node properties through the virtuals at vtable +0x90, +0x98 and
// +0xa0, allocates a 320-byte CMeshSceneNode and constructs it from (copied mesh, -1, the three properties). It then
// calls the new node's virtual at vtable +0x28 with the result of the parent's virtual at +0x24 (the parent comes
// from getParent), and returns the new node. The mesh, property and CMeshSceneNode declarations now exist in the engine
// (scene/CMeshSceneNode.hpp, ISceneNode slots 0x90 to 0xa0). Still blocked: glitch::IDevice declares no +0x10 driver
// (layout request), and the engine createMeshCopy overload takes (mesh, driver, flags) while the original passes one
// more argument (0) after the flags.
void* CopyMeshSceneNode(glitch::scene::IMeshSceneNode* node) {
    (void)node;
    return nullptr;
}

// 0x530a6c SetFinalOrientation(int)
// PARTIAL: stub returns 0. The original reads the auto-reorientation option from the savegame manager
// (Application +0x4c, SavegameManager::getAutoReorientation, not declared) and returns it. When that value is 1 and
// lastOpenMenuID is not 9, it acts on the orientation argument: for 1 it writes 0 to the touch screen's +0x1a8 word
// (Application +0x20 m_touchScreen), calls IVideoDriver::setOrientation(0) on the device's driver (+0x10) and sets
// isScreenOriented to 1; for -1 it writes 2, calls setOrientation(2) and sets isScreenOriented to 0. The returned
// value is then the setOrientation result. Blocked on SavegameManager::getAutoReorientation, TouchScreenBase +0x1a8,
// IVideoDriver::setOrientation and the globals lastOpenMenuID (ELF 0x9f63fc) and isScreenOriented (ELF 0x99b100).
extern "C" int SetFinalOrientation(int orientation) {
    (void)orientation;
    return 0;
}

// 0x530b5c SetDisplayString(char*)
// PARTIAL: no call is made. The original calls VoxSoundManager::ShowPlayingMusicTitle(VoxSoundManager::s_instance,
// title). ShowPlayingMusicTitle is not declared anywhere in the tree.
extern "C" void SetDisplayString(char* title) {
    (void)title;
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
