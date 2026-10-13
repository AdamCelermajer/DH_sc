// Global variables of libDungeonHunter2.so that game code reads or writes directly (<global>). Each has its own ELF
// object, so each is declared here with its address (ELF symbol value); the type is the one the accesses show.
// Definitions: game-app/src/Globals.cpp.
//
// Words inside a singleton object are not globals and are not declared here. The IDA names dword_99F7xx below are
// members of Singleton<Application>::s_inst (0x99f72c, dh2/Application.hpp) and must be used as members:
//   dword_99F73C = s_inst +0x10 m_device           dword_99F740 = s_inst +0x14 m_eventManager
//   dword_99F74C = s_inst +0x20 m_touchScreen      dword_99F764 = s_inst +0x38 m_objectManager
//   dword_99F768 = s_inst +0x3c m_luaManager       dword_99F76C = s_inst +0x40 m_playerManager
//   dword_99F778 = s_inst +0x4c m_savegameManager  dword_99F77C = s_inst +0x50 m_zoomHandler
//   dword_99F7A0 = s_inst +0x74 m_frameCount (the UpdateStateFX frame counter read by Character::Update)
// Other words in singleton objects: byte_99FC31 is Singleton<CXPlayerManager>::s_inst +0x05, and the ELF byte at
// 0x9a24d4 is GSKeyboard::s_inst +0x04 (m_querying).
#pragma once

#include <cstdint>

class updateJob_thread;

// .data, 1 byte, initial value 1. Character::Update tests it (LDRB) together with LG_DEVICES to pick the faerie
// position copy.
extern bool P1000TF;                                   // 0x99975c

// .bss, 16 bytes: the faerie position copy in Character::Update. Words 0..2 are the x, y, z words of
// GameObject::m_position (Point3D<float> at Character +0x160, +0x164, +0x168) copied bit for bit; word 3 is stored
// as zero. The IDA names dword_9F69B8, dword_9F69BC and dword_9F69C0 are words 1, 2 and 3 of this object.
extern float m_fairyPos_new[4];                        // 0x9f69b4

// .bss, 1 byte: leaderboard flag, cleared by MenuLeaderboard::Hide.
extern bool leader;                                    // 0x9a5c10

// .bss, 4 bytes: written by NativeShowLeaderboard together with leader; its meaning is not recovered.
extern int leaderboardcount;                           // 0x9a5c14

// .bss, 1 byte: set to 1 by the "quit" command and to 0 by "no" in NativeHUDInteract.
extern bool InExitConfirmation;                        // 0x9a5c18

// .bss, 4 bytes: id of the last opened menu. Set to 9 by the menu event code when btn_MENU_CONTINUE is pressed, and
// compared with 17 by Script_LockTutorial (script_command_globals.hpp declares the same global).
extern int lastOpenMenuID;                             // 0x9f63fc

// .bss, 1 byte: cheat switch. The cheat block of SavegameManager::toggleOption (LevelUp, GiveGold) runs only while it is set.
// Definition belongs to the globals batch (Globals.cpp); none in the tree yet.
extern bool handle_cheats_inGame;                      // 0x9f640a

// .data, 1 byte, initial value 1: orientation flag, set by the orientation code in the globals batch.
extern bool isScreenOriented;                          // 0x99b100

// .bss, 4 bytes each: accelerometer words written by the JNI nativeAccelerometer and copied out by
// GetAccelerometerParams. The accesses are 32-bit copies only (the JNI arguments are jfloat, so float is likely,
// but no float arithmetic is evidenced), so the type stays uint32_t until a consumer shows it.
extern std::uint32_t mAccelerometerX;                  // 0xa354f4
extern std::uint32_t mAccelerometerY;                  // 0xa354f8
extern std::uint32_t mAccelerometerZ;                  // 0xa354f0

// .bss, 4 bytes: operator package id, compared with 4 in Application::GetVersionString.
extern int OPERATOR_PACKAGE_INGAME;                    // 0x9f6410

// .bss, 1 byte: first-run flag. Application::Application stores it with a byte store (STRB).
extern bool FirstTime;                                 // 0x99fa5d

// .bss, 16 bytes: the skip label of the subtitles. LoadSubtitles writes it as text ("Passer", "Salta", ...) and as
// words (+0, +4, +8) and a byte (+0xc). GetSkipText returns its address.
extern char g_skip[16];                                // 0x9a1da8

// .bss: the Korean-build flag (1 byte, 0 at start) and the two language words (lang_kor, .bss, 0 at start;
// lang_curr, .data, initial value -1).
extern bool isKOREAN_BUILD;                            // 0x9f640b
extern int lang_kor;                                   // 0x9a5c0c
extern int lang_curr;                                  // 0x99983c, initial value -1

// .bss: job threads 1..4 (the Application constructor stores them). Job thread 0 is Application::m_updateJobThread0,
// the member at +0x04.
extern updateJob_thread* m_updateJobThread1;           // 0x99fa2c
extern updateJob_thread* m_updateJobThread2;           // 0x99fa30
extern updateJob_thread* m_updateJobThread3;           // 0x99fa34
extern updateJob_thread* m_updateJobThread4;           // 0x99fa38
