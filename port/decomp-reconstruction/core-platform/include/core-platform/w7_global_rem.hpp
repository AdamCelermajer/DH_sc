// Declarations for the global_rem slice (w7_global_rem.cpp): the remaining
// <global> rows that are not in the console, global_00..04 or touch slices.
//
// Stand-ins: the listings reference types and globals owned by the game, video,
// audio and gameswf categories, none of which are in this tree. They are declared
// here so this slice compiles. The owning category defines them at integration.
#pragma once

#include <cstdint>
#include <iostream>
#include <string>

#include "glitch-shared/glitch_core.hpp"

namespace gameswf {
struct fn_call;
}  // namespace gameswf

// ---- ObjectHandle (stand-in; three words, filled by ObjectManager) ----------
struct ObjectManager;
struct ObjectHandle {
    std::uint32_t words[3];
};
// ObjectHandle::GetObject(bool assertFound), as declared in game/include/dh2/ObjectHandle.hpp
// (0x33fdc0). The listing passes 0 (false) for the presence check and 1 (true) for the
// object itself. Returns the object, or null.
void* ObjectHandleGetObject(const ObjectHandle* handle, bool assertFound);
// Name pointer of the object (word 17, byte +0x44 of ObjectBase in the listing).
const char* ObjectBaseName(void* object);
// ObjectManager::GetObjectByName(name, flags, active, type), declared in
// game/include/dh2/ObjectManager.hpp (0x34aca0). The listing returns the handle through
// a hidden pointer, so it is returned by value here. Called on dword_99F764 with
// flags -1, active = true (the listing's `&dword_0 + 1` is the integer 1) and type null.
ObjectHandle ObjectManagerGetObjectByName(ObjectManager* mgr, const char* name, int flags, bool active,
                                          const char* type);
extern ObjectManager* g_ObjectManagerInstance;  // dword_99F764

// ---- Debug drawing (stand-in driver call) -----------------------------------
// dword_99F73C is the debug-draw owner; the driver pointer is at its +0x10. The
// driver's vtable slot at byte offset +0x28 is glitch::video::IVideoDriver::draw3DBox
// (aabbox3d<float> const&, SColor) in the IVideoDriver vtable listing; the owner's
// type is not confirmed as IVideoDriver, so the call stays a stand-in.
void* DbgDrawOwnerDriver();  // *(dword_99F73C + 0x10)
int DbgDrawBoxViaDriverSlot28(void* driver, const float rect[6], std::uint32_t packedColor);

// ---- Player and character stand-ins -----------------------------------------
// The local player's Character class id (short at byte offset 5064 of the
// Character, the listing's `__int16*` index 2532). Returns false when the local
// player has no character (character_660 is null).
bool LocalCharacterClassId(int* outClassId);
// Value at +1632 of the local player (a Character*), or null.
void* LocalPlayerCharacterSlot();
// Character::SG_GetPlayerName.
const char* CharacterSgPlayerName(void* character);
// PyDataConstants::getConstant(table, key) for the StrID table.
int PyDataConstantStrId(const char* table, const char* key);
// StringManager::parse(this, out, pattern, text, key, value). The meaning of the
// pattern argument ("^$") is not recovered.
void StringManagerParse(std::string& out, const char* pattern, const char* text, const char* key, const char* value);

// ---- Application and sound stand-ins ----------------------------------------
// Application::GetSavedOption and Application::ResetOrientation on
// Singleton<Application>::s_inst.
int ApplicationGetSavedOption(const char* name);
void ApplicationResetOrientation(int value);
// VoxSoundManager::s_instance and its ResumeAllSounds (return value treated as int).
struct VoxSoundManager;
extern VoxSoundManager* g_VoxSoundManagerInstance;
int VoxSoundManagerResumeAllSounds(VoxSoundManager* mgr);

// Owned by the level-loading category.
extern int s_dbgLevelLoadCount;

// ---- Declarations ------------------------------------------------------------

// 0x340154 _Z21DBG_DisplayPointAsBoxRKN6glitch4core8vector3dIfEERKNS_5video6SColorEi
int DBG_DisplayPointAsBox(const glitch::core::vector3d<float>& point, const glitch::video::SColor& color, int size);

// 0x342478 _ZlsRSoR12ObjectHandle
std::ostream& operator<<(std::ostream& os, ObjectHandle& handle);

// 0x34b170 _ZrsRSiR12ObjectHandle
std::istream& operator>>(std::istream& is, ObjectHandle& handle);

// 0x3ef9fc _Z25DBG_DisplayLoadLevelStatsPKc
int DBG_DisplayLoadLevelStats(const char* nextLevelName);

// 0x41f834 _Z18playerClassAsStrIDi
int playerClassAsStrID(int classId);

// 0x4218e4 _Z16playerClassAsStrv
const char* playerClassAsStr();

// 0x429c10 _Z24DisplayBuildInfoOnSplashv
void DisplayBuildInfoOnSplash();

// 0x429c14 _Z21HideBuildInfoOnSplashv
void HideBuildInfoOnSplash();

// 0x433d00 _Z15ParsePlayerNameRKSs
std::string ParsePlayerName(const std::string& name);

// 0x43a8f0 _Z17UpdateSavedValuesv
void UpdateSavedValues();

// 0x43acb4 _Z21NatvieResumeAllSoundsRKN7gameswf7fn_callE
int NatvieResumeAllSounds(const gameswf::fn_call& call);
