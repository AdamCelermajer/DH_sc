// Storage of the static data members of AchievementMsg that its header declares and no batch defined (link-1). One translation unit per
// class, so a program links the constructors these members need only when it uses the class.
#include "dh2/AchievementMsg.hpp"

const char* AchievementMsg::s_StartFuncName = "onAchievementMessage";  // 0x999838, .data: points at "onAchievementMessage" (0x8cb6a0)
const char* AchievementMsg::s_SkipFuncName = nullptr;                   // 0x9a54e8, .bss
