// DebugCachedCharacter: a (path, gameswf::character weak pointer) pair that MenuManager keeps embedded at
// +0x20 (original size 0x30) so a debug UI element can be re-found by name once the weak pointer dies.
// Offsets are the original 32-bit ones. Constructor 0x41aeec (default), 0x431428 (clone, takes a character);
// destructor 0x41a9fc releases the weak_ptr proxy refcount and frees the string.
#pragma once

#include <cstdint>
#include <string>

class MenuFX;
namespace gameswf { class character; }

class DebugCachedCharacter {
public:
    DebugCachedCharacter();                                             // 0x41aeec
    explicit DebugCachedCharacter(gameswf::character* character);       // 0x431428
    ~DebugCachedCharacter();                                            // 0x41a9fc

    void RefreshCache(gameswf::character* character, MenuFX* menu, gameswf::character* root);   // 0x427c44
    void RefreshCache(const char* path, MenuFX* menu, gameswf::character* root);                // 0x427ca0
    gameswf::character* GetChar();                                      // 0x427d50 asserts m_character.get_ptr() != 0

    uint8_t m_flag;                                                     // +0x00 0 (the ctor writes it as a byte)
    uint8_t pad_01[3];
    uint32_t unk_04;                                                    // +0x04 0
    std::string m_path;                                                 // +0x08 STLport string, 0x18 bytes (+0x08 .. +0x1f)
    uint32_t unk_20;                                                    // +0x20 0
    uint32_t unk_24;                                                    // +0x24 0
    void* m_characterProxy;                                             // +0x28 gameswf::weak_ptr<character>: proxy (refcount at *proxy)
    gameswf::character* m_character;                                    // +0x2c ... cached pointer
};
