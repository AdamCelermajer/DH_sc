// AchievementMsg: one achievement popup, queued through MenuMessageManager<AchievementMsg, 1>.
// NativeGetNextAchievementMessage copies the front entry to the script as the properties "Name", "Desc",
// "TrophyType", "Grade" and "Label". The first string is the base (class-facts lists std::string as its base),
// so "Name" is the base's content.
// Original 32-bit size 0x3c (60): two 0x18-byte strings and three ints. The size comes from the 60-byte
// stack copy in NativeGetNextAchievementMessage (0x440278).
// Reconstructed from libDungeonHunter2.so pseudocode (copy ctor 0x380554, ctor 0x433acc / 0x433b08).
#pragma once

#include <string>

class AchievementMsg : public std::string {
public:
    AchievementMsg(const AchievementMsg& other);                        // 0x380554

    // Ctor in original order. C1 at 0x433acc, C2 at 0x433b08.
    AchievementMsg(const std::string& name, const std::string& desc, int trophyType, int grade, int label);  // 0x433acc / 0x433b08

    // std::string base at +0x00 .. +0x17: "Name".
    std::string m_desc;                                                 // +0x18 "Desc"
    int m_trophyType;                                                   // +0x30 "TrophyType"
    int m_grade;                                                        // +0x34 "Grade"
    int m_label;                                                        // +0x38 "Label"
};
