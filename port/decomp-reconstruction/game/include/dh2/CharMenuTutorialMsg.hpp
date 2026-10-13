// CharMenuTutorialMsg: one queued tutorial message for the character menu. It holds a string id and
// two strings naming the menu frame and menu. Queued through MenuMessageManager<CharMenuTutorialMsg, 1>.
// Original 32-bit size 0x34 (52): int at +0x00, std::string at +0x04 and +0x1c (24 bytes each).
// Reconstructed from libDungeonHunter2.so pseudocode (ctors 0x433b44 / 0x433b70, copy ctor 0x45a780).
// No vtable of its own. The vtable belongs to MenuMessageManager<CharMenuTutorialMsg, 1>.
#pragma once

#include <cstdint>
#include <string>

class CharMenuTutorialMsg {
public:
    CharMenuTutorialMsg(int stringId, const std::string& frame, const std::string& menu); // 0x433b44 (C1), 0x433b70 (C2)
    CharMenuTutorialMsg(const CharMenuTutorialMsg& other);              // 0x45a780

    int m_stringId;                                                     // +0x00 string table id of the tutorial text
    std::string m_frame;                                                // +0x04 menu frame name (24 bytes at 32-bit)
    std::string m_menu;                                                 // +0x1c menu name (24 bytes at 32-bit)
};
