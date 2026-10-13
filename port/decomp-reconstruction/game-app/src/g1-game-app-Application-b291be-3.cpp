// Batch g1-game-app-Application-b291be-3 (Application, part 4 of 4).
// Rebuilt from the Hex-Rays pseudocode and checked against the assembly (tools/decomp_asm.py).
// Member offsets are the original 32-bit ones, recorded in dh2/Application.hpp.
//
// PostInit and InitWin32 are PARTIAL: each note names the part of the original that is not written and why.
#include "dh2/Application.hpp"

#include "dh2/EventManager.hpp"
#include "dh2/GSInit.hpp"
#include "dh2/LuaManager.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/PhysicalWorld.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/StateMachine.hpp"
#include "dh2/StringManager.hpp"
#include "dh2/global/net_singletons.hpp"
#include "net-session-online/c_online_impl.hpp"

// 0x32f7e8 Application::PostInit()
// PARTIAL: the managers that have a declared class are created in the original order, each as operator new(size) then
// its constructor, and stored in its member. Not written:
//   - PyDataConstants (+0x2c, operator new(0x24)) and PyDataArrays (+0x30, operator new(0x3c)), both constructed from
//     m_dataReloaderManager (+0x28). Their header pydata-accessors/w7_pydata12.hpp defines a stand-in IStreamBase that
//     conflicts with dh2/PlayerManager.hpp in one translation unit, and this file needs PlayerManager.
//   - the four PlayerLightTweaker objects (+0x58..+0x64, operator new(0x144)) and the name each gets at its +0xf8
//     string (the name table PlayerLightTweaker::TWEAKER_NAMES is not declared, and the header reads +0xf8 as an int
//     named m_lightType). dh2/PlayerLightTweaker.hpp also includes glitch-debugger/CTweakable.hpp, which is not in the
//     tree, so the header cannot be included yet.
//   - the GamepadTweaker (+0x68, operator new(0xe8)): dh2/GamepadTweaker.hpp includes the same missing CTweakable header.
//   - the store of 1 to IDevice +0x1c -> +0x250 (asm: m_device, then the object at +0x1c, byte +0x250). IDevice +0x1c is
//     not declared (layout request).
//   - the ZoomHandler (+0x50, operator new(0x44)): dh2/ZoomHandler.hpp includes the scene-collada-gui event stand-in,
//     which redefines glitch::IEventReceiver and glitch::SEvent. It conflicts with the glitch-core-io glitch_core that
//     dh2/GSInit.hpp brings in, and this file needs GSInit for the state switch below.
//   - the Console (+0x48, operator new(0xa8)) and Console::Console: waits on dh2/Console.hpp (not in the tree yet).
void Application::PostInit() {
    m_eventManager = new EventManager();               // +0x14, operator new(0x30)
    m_stateMachine = new StateMachine();               // +0x18, operator new(0x20)
    m_stringManager = new StringManager();             // +0x34, operator new(0x7d8)
    m_objectManager = new ObjectManager();             // +0x38, operator new(0x1b0)
    m_luaManager = new LuaManager();                   // +0x3c, operator new(0x1c)
    m_playerManager = new PlayerManager();             // +0x40, operator new(0x720)
    m_physicalWorld = new PhysicalWorld();             // +0x44, operator new(0x14)
    m_underlayNode = nullptr;                          // +0xb8 (the original stores 0 here)
    m_stateMachine->SwitchState(&GSInit::s_inst, false);  // StateMachine::SwitchState(GSInit, owned = false)
    // The original calls COnlineImpl::Initialize directly on the online object (GetOnline returns COnline).
    static_cast<COnlineImpl*>(GetOnline())->Initialize();
}

// 0x32fa7c Application::RegisterForIrrlichtEvents(glitch::IEventReceiver *)
// The original links a new node in front of the list sentinel (+0x08): that is push_back. A null receiver is ignored.
void Application::RegisterForIrrlichtEvents(glitch::IEventReceiver* receiver) {
    if (receiver != nullptr) {
        m_eventReceivers.push_back(receiver);
    }
}

// 0x32fab4 Application::InitWin32(glitch::IDevice *)
// PARTIAL: the savegame manager, Init, PostInit and the menu manager are written in the original order. Not written:
//   - the touch screen (+0x20, operator new(0x1b8), TouchScreenWin32(width, height)), which is the first step of the
//     original. width and height are read from IDevice +0x10 -> the object at +0xcc -> the pointer stored 4 bytes
//     before it -> +0x0c and +0x10. IDevice +0x10 and its object are not declared (layout request), so the values
//     cannot be passed. m_touchScreen therefore stays null here.
//   - the registration of the touch screen as an event receiver (RegisterForIrrlichtEvents on its IEventReceiver
//     sub-object, +0x1b4 in the original). Because m_touchScreen is null, the original would register nothing. The call
//     is not written: core-platform/touch_screen_win32.hpp brings a second glitch_core that conflicts with dh2/GSInit.hpp
//     in this translation unit. The registration belongs with the touch screen creation.
//   - the device call through IDevice +0x34: vtable +0x18 with ("shaders.pak", 1, 0). The class at +0x34 is not declared.
//   - the font block: get_fontfile("_sans", ...) fills a 256-byte name buffer; the object at IDevice +0x18 is then
//     asked (vtable +0x50) to load the name with length 14, and if that returns 0 to load the same buffer again from
//     its second byte. A non-zero result goes to vtable +0x38 on the same object, and that object's vtable +0x2c
//     receives (result, 0). The class at +0x18 is not declared (layout request), and get_fontfile is not declared.
void Application::InitWin32(glitch::IDevice* device) {
    m_savegameManager = new SavegameManager();         // +0x4c, operator new(0x58)

    Init(device);
    PostInit();

    m_menuManager = new MenuManager();                 // +0x54, operator new(0x114)
}

// 0x32fca0 Application::~Application() (D1, complete object)
// 0x32fd10 Application::~Application() (D2, base object)
// Both variants have the same 805-byte body, so this one definition serves both. The vtable pointer store at the start
// of the original is written by the compiler. The original does the following, in this order:
//   1. clears the receiver list (+0x08)
//   2. resizes the list to zero (list::resize(0, nullptr), a no-op on the empty list)
//   3. frees the buffers of the std::string at +0xd4 (unk_d4) and +0xbc (m_uncompiledData)
//   4. clears the receiver list again (a no-op)
// Steps 3 and 4 are the member destructors of the C++ class, which run after this body in reverse declaration order:
// unk_d4, then m_uncompiledData, then the list. That is the same order of frees as the original.
Application::~Application() {
    m_eventReceivers.clear();
    m_eventReceivers.resize(0, nullptr);
}

// 0x32fd80 Application::~Application() (D0, deleting destructor)
// SKIPPED: compiler-generated. The original calls D1 (0x32fca0) and then CustomFree(this). The C++ delete expression
// emits the same shape, so no body is written here.
