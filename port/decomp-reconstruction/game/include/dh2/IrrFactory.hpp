// IrrFactory: the game's factory for the glitch engine objects. It derives from the engine singleton
// glitch::CIrrFactory and overrides the file-system and scene-manager creators. It declares no data of its own.
// The engine header in the tree declares only the constructor, the virtual destructor and getInstance, so the
// two creators below are plain virtuals here, not overrides. Add override once the engine header lists them.
// Reconstructed from libDungeonHunter2.so pseudocode (dtors 0x350a4c, 0x350c40; createFileSystem 0x350aec;
// createSceneManager 0x350a80). The IrrFactory constructor is not in the queue, so it is not declared.
#pragma once

#include "glitch-core-io/boost/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/irr_factory.hpp"

class SceneManager;

namespace glitch {
namespace video {
class IVideoDriver;
}  // namespace video
namespace io {
class IFileSystem;
}  // namespace io
namespace gui {
class ICursorControl;
class IGUIEnvironment;
}  // namespace gui
}  // namespace glitch

class IrrFactory : public glitch::CIrrFactory {
public:
    ~IrrFactory() override;                                         // slot +0x00 0x350a4c (D1), slot +0x04 0x350c40 (D0): reset the vptr, then the base dtor

    // IrrFactory's own vtable slots, in vtable order. Slot +0x10 is glitch::CIrrFactory::createGUIEnvironment,
    // inherited and not redeclared.
    virtual boost::intrusive_ptr<glitch::io::IFileSystem> createFileSystem();                   // slot +0x08 0x350aec: new FileSystemWin32, returned counted
    virtual SceneManager* createSceneManager(glitch::video::IVideoDriver* driver,               // slot +0x0c 0x350a80: new SceneManager, 0x494 bytes
                                             const boost::intrusive_ptr<glitch::io::IFileSystem>& fileSystem,
                                             glitch::gui::ICursorControl* cursor,
                                             glitch::gui::IGUIEnvironment* gui);
};
