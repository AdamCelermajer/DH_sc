#include "core-platform/w7_console_00.hpp"

// 0x3301c4 _ZN7Console17_setMenuLevelListEv
// Partial: the body matches the listing. _createList is not defined in this slice.
void Console::_setMenuLevelList() {
    _createList(ConsoleVectorAt(this, 0x10), const_cast<char*>("LEVELS"));
}

// 0x3301d8 _ZN7Console15_setMenuCamerasEv
// Body is a bare return in the listing.
void Console::_setMenuCameras() {}

// 0x3301dc _ZN7Console25_setMenuLoadQuestProgressEv
// Partial: the body matches the listing. _createList is not defined in this slice.
void Console::_setMenuLoadQuestProgress() {
    _createList(ConsoleVectorAt(this, 0x34), const_cast<char*>("Load Quest Progress"));
}

// 0x33026c _ZN7Console24SetShowRoomDisplayedAnimEi
// Body is a bare return in the listing; the argument is unused.
void Console::SetShowRoomDisplayedAnim(int) {}
