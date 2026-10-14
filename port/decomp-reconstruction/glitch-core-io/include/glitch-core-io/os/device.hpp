// glitch::IDevice: the device interface. Only the base members the queued functions use are
// declared. Layout from the original offsets (kept as comments):
//   +0x08 resource file manager, +0x0c animation streaming manager (both created by the constructor),
//   +0x10 video driver, +0x1c scene manager, +0x20 timer (created by the constructor),
//   +0x28 event receiver, +0x2c logger (pointer, non-null when a logger is attached),
//   +0x34 file system (created by the constructor through the CIrrFactory),
//   +0x3c video mode list, +0x5c creation parameters (0x58 bytes, copied by the constructor).
// The creation parameters' fullscreen flag is byte 0x0e of that block (original +0x6a).
// The window-offset fields read by postMouseEventFromUser are not modelled.
#pragma once

#include <cstdint>

#include "glitch-core-io/deps/glitch_core.hpp"

class FileSystemWin32;  // core-platform/filesystem_win32.hpp: the file system the game reaches through the device (+0x34)

namespace glitch {

namespace video { class IVideoDriver; }
namespace scene { class ISceneManager; }
namespace os { class Timer; }
namespace gui { class IGUIEnvironment; }
namespace collada { class CResFileManager; class CAnimationStreamingManager; }

// Creation parameters, kept as the 0x58-byte block the constructor copies.
struct SCreationParameters {
    unsigned char bytes[0x58];
};

class IDevice : public IReferenceCounted {
public:
    // 0x672040 (C1), 0x67228c (C2): copies the parameters and clears the members.
    explicit IDevice(const SCreationParameters& parameters);

    // 0x67142c: the base device never requests exit.
    virtual bool isExitRequested();
    // 0x671438: no-op in the base device.
    virtual void setScreenCapture(bool enable);
    // 0x67143c: the video mode list (original +0x3c).
    virtual video::CVideoModeList* getVideoModeList();
    // 0x671494: the fullscreen flag from the creation parameters (original +0x6a).
    virtual bool isFullscreen() const;
    // 0x67149c: the colour format the base device reports (constant 5).
    virtual int getColorFormat() const;
    // 0x6714c8: stores the receiver (+0x28), passes it to the logger when one is attached and
    // to the object at +0x18 (vtable +0x34, not reconstructed).
    // PARTIAL: the +0x18 object is not modelled; see src/device.cpp.
    virtual void setEventReceiver(IEventReceiver* receiver);
    // vtable +0x30 (CAndroidOSDevice::closeDevice 0x6a02a4 clears the running flag at +0x109). Application::Quit calls it.
    virtual void closeDevice() {}
    // 0x671e84: true when the application's compiled version string equals the library version
    // (0.1.0.2). Otherwise it logs a warning at ELL_WARNING and returns false.
    bool checkVersion(const char* compiledVersion);

    // +0x08: CResFileManager (0x2c bytes), created by the constructor with the device
    collada::CResFileManager* resFileManager = nullptr;
    // +0x0c: CAnimationStreamingManager (0x20 bytes), created by the constructor
    collada::CAnimationStreamingManager* animationStreamingManager = nullptr;
    // +0x10: the video driver. The constructor clears it, the derived device sets it; Application::CleanGlitch calls the
    // virtual at its vtable +0xac, and the game's debug draw reads it.
    video::IVideoDriver* videoDriver = nullptr;
    // +0x14: the 2D driver, created by createGUIAndScene from the video driver (C2DDriver, 0x20 bytes). Its vtable
    // slots +0x0c/+0x10/+0x14 are null in the library, so it is not the +0x18 receiver.
    void* unk_14 = nullptr;
    // +0x18: IDENTIFIED as the GUI environment. createGUIAndScene (0x671554) stores the CIrrFactory instance's
    // vtable +0x10 there (asm 0x6715a8), and CIrrFactory+0x10 is createGUIEnvironment (0x533f28), which allocates a
    // glitch::gui::CGUIEnvironment. The same function stores the factory's +0x0c (createSceneManager) at device
    // +0x1c, which is the already-established scene manager, so the producer side pins both words.
    //   * Application::_Draw (0x32afdc) and GSConsole::Draw2D (0x3832c4) both call vtable slot +0x0c, and
    //     _ZTVN6glitch3gui15CGUIEnvironmentE + 0x0c is drawAll -- so those calls are IGUIEnvironment::drawAll().
    //   * IDevice::setEventReceiver (0x6714c8) forwards the receiver to slot +0x34, which for CGUIEnvironment is
    //     setUserEventReceiver.
    //   * Application::InitWin32 (0x32fbf0) calls slot +0x50, which is getTTFont.
    // The header comment in setEventReceiver below used to say the +0x18 object was "not modelled"; it is this one.
    gui::IGUIEnvironment* guiEnvironment = nullptr;
    // +0x1c: the scene manager (the game's GSViewer reads it from Application::m_device)
    scene::ISceneManager* sceneManager = nullptr;
    // +0x20: the timer the constructor creates (0x8 bytes, vptr and a flag)
    os::Timer* timer = nullptr;
    // +0x24: cleared by the constructor, role not identified
    void* unk_24 = nullptr;
    // +0x28
    IEventReceiver* eventReceiver = nullptr;
    // +0x2c: logger, null when none is attached (CLogger is not reconstructed)
    void* logger = nullptr;
    // +0x30: cleared by the constructor, role not identified
    void* unk_30 = nullptr;
    // +0x34: the file system. The constructor takes it from the CIrrFactory (vtable +0x08 of the factory instance). The
    // game calls its savefile slots through Application::m_device: getFiles (+0x7c), getFilesMatching (+0x80), openSavefile
    // (+0x94), deleteSavefile (+0x9c) and doesSavefileExists (+0xb0). The dynamic type is FileSystemWin32.
    FileSystemWin32* fileSystem = nullptr;
    // +0x38: cleared by the constructor, role not identified
    void* unk_38 = nullptr;
    // +0x3c
    video::CVideoModeList videoModeList;
    // +0x5c
    SCreationParameters creationParameters;
};

}  // namespace glitch
