// Reconstruction of the global_rem slice (17 rows of the <global> owner).
// Each function is listed with its ELF address and mangled symbol. The bodies
// follow the Hex-Rays export and the ARM listing. link-1: they call the game classes
// directly (the stand-in seams this file used are gone, see core-platform/w7_global_rem.hpp).

#include "core-platform/w7_global_rem.hpp"

#include <cstdio>

#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/Level.hpp"
#include "dh2/ObjectBase.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/StringManager.hpp"
#include "pydata-accessors/w7_pydata12.hpp"
#include "vox-audio/vox/sound_manager.hpp"

// 0x340154 _Z21DBG_DisplayPointAsBoxRKN6glitch4core8vector3dIfEERKNS_5video6SColorEi
// Partial: the box extents and the packed colour follow the listing (rect order
// x-r, y-r, z-r, x+r, y+r, z+r; colour bytes packed R | G<<8 | B<<16 | A<<24).
// The driver call at vtable +0x28 is a stand-in (DbgDrawBoxViaDriverSlot28). That
// slot is IVideoDriver::draw3DBox(aabbox3d<float> const&, SColor) in the IVideoDriver
// vtable, and the argument shape matches, but glitch::IDevice does not model the driver at +0x10.
// The stand-in's result is returned, as the listing does.
int DBG_DisplayPointAsBox(const glitch::core::vector3d<float>& point, const glitch::video::SColor& color, int size) {
    const float half = static_cast<float>(size / 2);
    const float rect[6] = {
        point.X - half, point.Y - half, point.Z - half,
        point.X + half, point.Y + half, point.Z + half,
    };
    const std::uint32_t packed = static_cast<std::uint32_t>(color.component(0)) |
                                 (static_cast<std::uint32_t>(color.component(1)) << 8) |
                                 (static_cast<std::uint32_t>(color.component(2)) << 16) |
                                 (static_cast<std::uint32_t>(color.component(3)) << 24);
    void* driver = DbgDrawOwnerDriver();
    return DbgDrawBoxViaDriverSlot28(driver, rect, packed);
}

// 0x342478 _ZlsRSoR12ObjectHandle
// ObjectHandle::GetObject(bool) is called with false as the presence check and true for the object.
// The name is the string at +0x30 of the object (its data pointer is the word at +0x44 in the listing). The
// original writes it with _M_put_nowiden, which pads to the stream width as operator<< does, so
// operator<< matches it here.
std::ostream& operator<<(std::ostream& os, ObjectHandle& handle) {
    if (handle.GetObject(false)) {
        ObjectBase* object = handle.GetObject(true);
        os << object->m_name.c_str();
    }
    return os;
}

// 0x34b170 _ZrsRSiR12ObjectHandle
// Reads one whitespace-delimited name, then calls GetObjectByName(name, -1, true, nullptr) on the
// ObjectManager (Application +0x38, the listing's dword_99F764) and copies the handle back. The listing's
// third argument `&dword_0 + 1` is the integer 1, which is the bool `active`, and the fourth is 0 (null type).
std::istream& operator>>(std::istream& is, ObjectHandle& handle) {
    std::string name;
    is >> name;
    ObjectHandle found = ApplicationGetInstance().m_objectManager->GetObjectByName(name.c_str(), -1, true, nullptr);
    handle = found;
    return is;
}

// 0x3ef9fc _Z25DBG_DisplayLoadLevelStatsPKc
// Level::s_dbgLevelLoadCount is defined in game-app/src/statics_Level.cpp.
int DBG_DisplayLoadLevelStats(const char* nextLevelName) {
    std::puts("*******************************************************************");
    std::printf("Loaded a level for the %d time\n", Level::s_dbgLevelLoadCount);
    std::printf("Next level to be loaded is : %s \n", nextLevelName);
    return std::puts("*******************************************************************");
}

// 0x41f834 _Z18playerClassAsStrIDi
// The class id is 263 (warrior, base), 325 (+1) or 290 (+2) from the StrID table entry GAMEPLAYMENUS_CLASS_00 of the
// PyDataConstants (Application +0x2c, the listing's dword_99F758). With -1 the id is the class of the local player's
// character (PlayerManager::GetLocalPlayer(0, true), PlayerInfo +0x660, Character +0x13c8 as a 16-bit value);
// other ids return -1.
int playerClassAsStrID(int classId) {
    int v1 = classId;
    if (classId == -1) {
        Character* character = ApplicationGetInstance().m_playerManager->GetLocalPlayer(0, true)->unk_660;
        if (!character) {
            return -1;
        }
        v1 = character->m_charPropsId;
    }
    int result = ApplicationGetInstance().m_pyDataConstants->getConstant("StrID", "GAMEPLAYMENUS_CLASS_00");
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
// The class names are "mage" (290), "rogue" (325) and "warrior" (263), read from the local player's character
// (PlayerManager::GetLocalPlayer(0, true), PlayerInfo +0x660, Character +0x13c8 as a 16-bit value).
const char* playerClassAsStr() {
    Character* character = ApplicationGetInstance().m_playerManager->GetLocalPlayer(0, true)->unk_660;
    if (!character) {
        return nullptr;
    }
    const int classId = character->m_charPropsId;
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
// When the local player has a character (PlayerInfo +0x660), the name is the "$player" substitution of the input through
// StringManager::parse (Application +0x34, the listing's dword_99F760), with the pattern "^$" and the character's
// SG_GetPlayerName; otherwise the input is copied. The meaning of the "^$" argument is not recovered.
std::string ParsePlayerName(const std::string& name) {
    std::string out;
    PlayerManager* players = ApplicationGetInstance().m_playerManager;
    if (players->GetLocalPlayer(0, true)->unk_660 != nullptr) {
        Character* character = players->GetLocalPlayer(0, true)->unk_660;
        ApplicationGetInstance().m_stringManager->parse(out, "^$", name.c_str(), "$player", character->SG_GetPlayerName());
    } else {
        out = name;
    }
    return out;
}

// 0x43a8f0 _Z17UpdateSavedValuesv
// Reads the "AutoOrientation" option and applies it to the orientation, both on Singleton<Application>::s_inst
// (Application::GetSavedOption 0x320e44, Application::ResetOrientation 0x31f748, which is empty).
void UpdateSavedValues() {
    int savedOption = ApplicationGetInstance().GetSavedOption("AutoOrientation");
    ApplicationGetInstance().ResetOrientation(savedOption);
}

// 0x43acb4 _Z21NatvieResumeAllSoundsRKN7gameswf7fn_callE
// Resumes all sounds when VoxSoundManager::s_instance (0x9a238c) exists. The listing returns the instance
// pointer's register when there is no instance and ResumeAllSounds' leftover r0 otherwise; nothing reads the result.
int NatvieResumeAllSounds(const gameswf::fn_call& call) {
    (void)call;
    VoxSoundManager* instance = VoxSoundManager::s_instance;
    if (instance != nullptr) {
        instance->ResumeAllSounds();
    }
    return 0;
}
