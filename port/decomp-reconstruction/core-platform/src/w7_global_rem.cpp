// Reconstruction of the global_rem slice (17 rows of the <global> owner).
// Each function is listed with its ELF address and mangled symbol. The bodies
// follow the Hex-Rays export and the ARM listing; stand-ins are declared in
// core-platform/w7_global_rem.hpp.

#include "core-platform/w7_global_rem.hpp"

#include <cstdio>

// 0x340154 _Z21DBG_DisplayPointAsBoxRKN6glitch4core8vector3dIfEERKNS_5video6SColorEi
// Partial: the box extents and the packed colour follow the listing (rect order
// x-r, y-r, z-r, x+r, y+r, z+r; colour bytes packed R | G<<8 | B<<16 | A<<24).
// The driver call at vtable +0x28 is a stand-in (DbgDrawBoxViaDriverSlot28). That
// slot is IVideoDriver::draw3DBox(aabbox3d<float> const&, SColor) in the IVideoDriver
// vtable, and the argument shape matches, but the owner's type at dword_99F73C+0x10
// is not confirmed. The stand-in's result is returned, as the listing does.
int DBG_DisplayPointAsBox(const glitch::core::vector3d<float>& point, const glitch::video::SColor& color, int size) {
    const float half = static_cast<float>(size / 2);
    const float rect[6] = {
        point.X - half, point.Y - half, point.Z - half,
        point.X + half, point.Y + half, point.Z + half,
    };
    const auto* bytes = reinterpret_cast<const unsigned char*>(&color);
    const std::uint32_t packed = static_cast<std::uint32_t>(bytes[0]) |
                                 (static_cast<std::uint32_t>(bytes[1]) << 8) |
                                 (static_cast<std::uint32_t>(bytes[2]) << 16) |
                                 (static_cast<std::uint32_t>(bytes[3]) << 24);
    void* driver = DbgDrawOwnerDriver();
    return DbgDrawBoxViaDriverSlot28(driver, rect, packed);
}

// 0x342478 _ZlsRSoR12ObjectHandle
// Partial: ObjectHandle is a stand-in (three words). ObjectHandle::GetObject(bool)
// is called with false as the presence check and true for the object, as the owner
// declaration in game/include/dh2/ObjectHandle.hpp gives. The name is the pointer
// at +0x44 of the object. The original writes it with _M_put_nowiden, which pads
// to the stream width as operator<< does, so operator<< matches it here.
std::ostream& operator<<(std::ostream& os, ObjectHandle& handle) {
    if (ObjectHandleGetObject(&handle, false)) {
        void* object = ObjectHandleGetObject(&handle, true);
        os << ObjectBaseName(object);
    }
    return os;
}

// 0x34b170 _ZrsRSiR12ObjectHandle
// Partial: reads one whitespace-delimited name, then calls
// GetObjectByName(name, -1, true, nullptr) on the ObjectManager and copies the
// three-word handle back. The listing's third argument `&dword_0 + 1` is the integer
// 1, which is the bool `active`, and the fourth is 0 (null type). The
// ObjectManager stand-in is not in this tree, so the handle lookup stays partial.
std::istream& operator>>(std::istream& is, ObjectHandle& handle) {
    std::string name;
    is >> name;
    ObjectHandle found = ObjectManagerGetObjectByName(g_ObjectManagerInstance, name.c_str(), -1, true, nullptr);
    handle = found;
    return is;
}

// 0x3ef9fc _Z25DBG_DisplayLoadLevelStatsPKc
// s_dbgLevelLoadCount is declared in w7_global_rem.hpp and defined by the owner.
int DBG_DisplayLoadLevelStats(const char* nextLevelName) {
    std::puts("*******************************************************************");
    std::printf("Loaded a level for the %d time\n", s_dbgLevelLoadCount);
    std::printf("Next level to be loaded is : %s \n", nextLevelName);
    return std::puts("*******************************************************************");
}

// 0x41f834 _Z18playerClassAsStrIDi
// Partial: the class id is 263 (warrior, base), 325 (+1) or 290 (+2) from the
// StrID table entry GAMEPLAYMENUS_CLASS_00. The local-character read is a stand-in
// (LocalCharacterClassId); other ids return -1.
int playerClassAsStrID(int classId) {
    int v1 = classId;
    if (classId == -1) {
        int localClass = 0;
        if (!LocalCharacterClassId(&localClass)) {
            return -1;
        }
        v1 = localClass;
    }
    int result = PyDataConstantStrId("StrID", "GAMEPLAYMENUS_CLASS_00");
    if (v1 != 263) {
        if (v1 == 325) {
            ++result;
        } else if (v1 == 290) {
            result += 2;
        } else {
            return -1;
        }
    }
    return result;
}

// 0x4218e4 _Z16playerClassAsStrv
// Partial: the class names are "mage" (290), "rogue" (325) and "warrior" (263).
// The local-character read is a stand-in (LocalCharacterClassId).
const char* playerClassAsStr() {
    int classId = 0;
    if (!LocalCharacterClassId(&classId)) {
        return nullptr;
    }
    if (classId == 290) {
        return "mage";
    }
    if (classId != 325) {
        if (classId == 263) {
            return "warrior";
        }
        return nullptr;
    }
    return "rogue";
}

// 0x429c10 _Z24DisplayBuildInfoOnSplashv
// Body is a bare return (BX LR in the listing).
void DisplayBuildInfoOnSplash() {}

// 0x429c14 _Z21HideBuildInfoOnSplashv
// Body is a bare return (BX LR in the listing).
void HideBuildInfoOnSplash() {}

// 0x433d00 _Z15ParsePlayerNameRKSs
// Partial: when the local player has a character, the name is the "$player"
// substitution of the input through StringManager::parse; otherwise the input is
// copied. The stand-in parse signature takes the pattern argument as "^$"; its
// meaning is not recovered.
std::string ParsePlayerName(const std::string& name) {
    std::string out;
    void* character = LocalPlayerCharacterSlot();
    if (character != nullptr) {
        const char* playerName = CharacterSgPlayerName(character);
        StringManagerParse(out, "^$", name.c_str(), "$player", playerName);
    } else {
        out = name;
    }
    return out;
}

// 0x43a8f0 _Z17UpdateSavedValuesv
// Partial: reads the "AutoOrientation" option and applies it to the orientation.
// Both calls are stand-ins for Application members. Their signatures match
// game/include/dh2/Application.hpp (GetSavedOption(const char*) at 0x320e44,
// ResetOrientation(int) at 0x31f748, which the owner header marks empty), but the
// Application instance (Singleton<Application>::s_inst) is not reachable from this category.
void UpdateSavedValues() {
    int savedOption = ApplicationGetSavedOption("AutoOrientation");
    ApplicationResetOrientation(savedOption);
}

// 0x43acb4 _Z21NatvieResumeAllSoundsRKN7gameswf7fn_callE
// Partial: resumes all sounds when the VoxSoundManager instance exists. The return
// value is the stand-in ResumeAllSounds result (declared int); the original's
// return type is not confirmed.
int NatvieResumeAllSounds(const gameswf::fn_call& call) {
    (void)call;
    VoxSoundManager* instance = g_VoxSoundManagerInstance;
    if (instance != nullptr) {
        return VoxSoundManagerResumeAllSounds(instance);
    }
    return 0;
}
