#include "core-platform/w7_console_00.hpp"

// 0x3301c4 _ZN7Console17_setMenuLevelListEv
// Partial: the body matches the listing (lists_[0] is the vector at ARM offset +0x10).
// Console::_createList is declared in w7_console_01.hpp but its definition is not in
// this slice, so the call does not link yet.
void Console::_setMenuLevelList() {
    _createList(lists_[0], const_cast<char*>("LEVELS"));
}

// 0x3301d8 _ZN7Console15_setMenuCamerasEv
// Body is a bare return in the listing.
void Console::_setMenuCameras() {}

// 0x3301dc _ZN7Console25_setMenuLoadQuestProgressEv
// Partial: the body matches the listing (lists_[3] is the vector at ARM offset +0x34).
// Console::_createList is declared but not defined in this slice.
void Console::_setMenuLoadQuestProgress() {
    _createList(lists_[3], const_cast<char*>("Load Quest Progress"));
}

// 0x33026c _ZN7Console24SetShowRoomDisplayedAnimEi
// Body is a bare return in the listing; the argument is unused.
void Console::SetShowRoomDisplayedAnim(int) {}
