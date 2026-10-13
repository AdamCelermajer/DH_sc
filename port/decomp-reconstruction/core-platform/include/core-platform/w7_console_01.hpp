// Console declarations used by w7_console_01.cpp.
// Offsets in comments are the ELF pseudocode offsets (ARM 32-bit layout). Field
// names describe how the rebuilt code uses each field; the class is not ABI-checked.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "dh2/EventManager.hpp"    // EventManager (game header, the one definition)
#include "dh2/IEventReceiver.hpp"  // ::IEventReceiver (game header, the one definition)

namespace glitch {
namespace scene {
class ISceneNode;
}
}

// EventManager instance; ELF global dword_99F740. Defined by the owning category.
extern EventManager* g_eventManager;

// File lister used by the module and scene lists. The object is the file system at
// dword_99F73C+0x34 (IDevice +0x34). Its ELF vtable slot +0x7c is
// FileSystemWin32::getFiles(char const*, vector<string>&), the one call used here.
class ConsoleFileLister {
public:
    virtual ~ConsoleFileLister() = default;
    // ELF vtable slot +0x7c. Appends the names matching `dir` (a path with a wildcard
    // such as "data/scene/*.xml") to `out`.
    virtual void getFiles(const char* dir, std::vector<std::string>& out) = 0;
};
extern ConsoleFileLister* g_consoleFileLister;

// Abstract for now: IEventReceiver::onEvent is pure in the canonical dh2/IEventReceiver.hpp, and
// Console::onEvent (0x335b14) and onEventNotOpened (0x33271c) are not reconstructed yet (skipped rows).
// Nothing constructs a Console in this tree (Application holds a Console* only).
class Console : public IEventReceiver {
public:
    Console();                  // 0x333a4c C1 and 0x33401c C2 (identical bodies)
    ~Console() override;        // 0x331a2c D2 and 0x331944 D1 (identical bodies)

    // 0x332488 static table of 15 command labels. Contents are in the data category.
    static const char* const COMMAND_LABELS[15];

    // Menu page builders and helpers. Definitions are in other console slices.
    void _createList(std::vector<std::string>& items, char* title);
    void _destroyList();
    void _setMenu();
    void _setMenuLevelList();
    void _setMenuCommands();
    void _setMenuLevelListPyData();
    void _setMenuWorldMap();
    void _setMenuStats();
    void _setMenuDebugModules();
    void _setMenuDebugSwitches();
    void _setMenuQuestList();
    void _setMenuLoadModuleFolderList();
    void _setMenuLoadQuestProgress();
    void _setMenuListeners();
    void _setMenuCameras();
    void SetShowRoomDisplayedAnim(int);  // 0x33026c (from w7_console_00)
    void _setMenuMenus();
    void _setMenuScripts();
    void _setMenuCharModules();
    void _setLootTableList();
    void _setMenuAnimList();
    void _setMenuVFXList();
    void _setMenuVFXSetList();

    void Toggle();              // 0x334bd8

private:
    bool open_ = false;                     // +0x04
    int menu_page_ = 0;                     // +0x08, index used by _setMenu()
    int selected_ = 0;                      // +0x0C
    std::vector<std::string> lists_[4];     // +0x10, +0x1C, +0x28, +0x34
    void* menu_ = nullptr;                  // +0x40, menu view object (Clear/Add/Set slots)
    std::vector<glitch::scene::ISceneNode*> toggled_nodes_;  // +0x48
    void* toggle_display_ = nullptr;        // +0x54
    std::string buffer_58_;                 // +0x58
    void* show_room_object_ = nullptr;      // +0x74
    int show_room_index_ = -1;              // +0x78
    void* show_room_camera_ = nullptr;      // +0x7C
    std::uint8_t touch_flags_ = 0;          // +0x80
    std::int16_t touch_x_ = 0;              // +0x82
    std::int16_t touch_y_ = 0;              // +0x84
    std::uint8_t touch_pending_ = 0;        // +0x86
    int touch_aux_ = 0;                     // +0x88
    int touch_id_ = -1;                     // +0x8C
    std::string module_pattern_;            // +0x90
};

// 0x331a10 D0 (deleting destructor): runs ~Console() then frees the object.
void ConsoleDeletingDestructor(Console* self);
