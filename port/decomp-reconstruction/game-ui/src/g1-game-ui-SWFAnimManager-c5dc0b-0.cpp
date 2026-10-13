// SWFAnimManager bodies, batch g1-game-ui-SWFAnimManager-c5dc0b-0 (the whole class).
// The bank of pooled SWFAnim objects, keyed by name. Rebuilt from the Hex-Rays pseudocode of libDungeonHunter2.so.
// Offsets in comments are the original 32-bit ARM offsets; the code never uses them for addressing.
//
// PARTIAL, one reason: ExtendBank clears the flag byte +0x9b of each clone it creates (`*((_BYTE *)v22 + 155) = 0`).
// That byte is not declared in the vendored gameswf::character (the same field FlashAnimManager is partial on).
// Layout request: "gameswf::character +0x9b uint8". The store is left out and ExtendBank is PARTIAL.
//
// The map walk in FlushAll is the in-order traversal of the original tree, which std::map iteration gives directly.
// The VectorSet storage is not released when a bank is cleared, as in the original (VectorSet has no destructor).

#include <cstdio>
#include <cstdlib>
#include <string>

#include "dh2/DebugCachedCharacter.hpp"
#include "dh2/MenuFX.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/RenderFX.hpp"
#include "dh2/SWFAnim.hpp"
#include "dh2/SWFAnimManager.hpp"
#include "dh2/VectorSet.hpp"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_sprite.h"

// gAssertLevel is declared in core-platform/memory.hpp, which is not included here (the same choice as FlashAnimManager).
extern int gAssertLevel;

// 0x4971e0 _ZN14SWFAnimManagerC2Ev SWFAnimManager::SWFAnimManager()  (C2)
// 0x497228 _ZN14SWFAnimManagerC1Ev SWFAnimManager::SWFAnimManager()  (C1, the same body: one C++ constructor serves both forms)
SWFAnimManager::SWFAnimManager() {
    // +0x00 vptr (set by the compiler). +0x04 .. +0x17: the empty map. The original writes an empty tree header by hand:
    // color 0, root null (+0x08 = 0), leftmost and rightmost the header itself (+0x0c, +0x10), node count 0 (+0x14).
    // std::map's default constructor builds the same state.
}

// 0x497270 _ZN14SWFAnimManager9CloneAnimERK7SWFAnim SWFAnimManager::CloneAnim(SWFAnim const&)
// The original is a stub that returns null (MOV R0, #0; BX LR). No caller reads it.
SWFAnim* SWFAnimManager::CloneAnim(const SWFAnim& anim) {
    (void)anim;
    return nullptr;
}

// 0x4973c8 _ZN14SWFAnimManager9FlushBankER9VectorSetIP7SWFAnimE SWFAnimManager::FlushBank(VectorSet<SWFAnim *> &)
void SWFAnimManager::FlushBank(VectorSet<SWFAnim*>& bank) {
    SWFAnim** first = bank.m_start;   // +0x00
    SWFAnim** last = bank.m_finish;   // +0x04
    if (first != last) {
        do {
            SWFAnim* anim = *first++;
            if (anim) {
                // The original calls vtable slot +0x04 of the anim: its deleting destructor (D0, CustomFree).
                delete anim;
            }
        } while (first != last);
        // The bank keeps its storage and is emptied: the end pointer moves back to the start.
        if (bank.m_start != bank.m_finish) {
            bank.m_finish = bank.m_start;
        }
    }
}

// 0x497418 _ZN14SWFAnimManager8DropAnimEP7SWFAnim SWFAnimManager::DropAnim(SWFAnim *)
// The Hex-Rays return value is the r0 that SWFAnim::SetVisible leaves behind; SetVisible returns void, so this is void.
void SWFAnimManager::DropAnim(SWFAnim* anim) {
    if (anim) {
        anim->SetVisible(false);
        anim->m_free = true;  // +0x04 byte set to 1: the anim is free in its bank again
    }
}

// 0x4974f4 _ZN14SWFAnimManager8FlushAllEv SWFAnimManager::FlushAll()
void SWFAnimManager::FlushAll() {
    // Visit the banks in key order (the original walks the tree in order) and empty each one.
    for (auto& entry : m_banks) {
        FlushBank(entry.second);
    }
    // The original erases the nodes when the count (+0x14) is not zero and resets the header to empty. clear() does the same.
    m_banks.clear();
}

// 0x4975a8 _ZN14SWFAnimManagerD1Ev SWFAnimManager::~SWFAnimManager()  (D1)
// 0x497628 _ZN14SWFAnimManagerD2Ev SWFAnimManager::~SWFAnimManager()  (D2, the same body: one C++ destructor serves both forms)
SWFAnimManager::~SWFAnimManager() {
    // The vptr store at the start of the original is the compiler's. FlushAll empties the map, so the original's
    // trailing erase (taken only when the count is non-zero) is never reached here; the map member's destructor
    // then finds it empty.
    FlushAll();
}

// 0x49760c _ZN14SWFAnimManagerD0Ev: scalar deleting destructor (D0). Compiler-generated for delete: it runs ~SWFAnimManager
// (D1 above) and then CustomFree(this). No body is written.

// 0x498620 _ZN14SWFAnimManager10ExtendBankEPKcP6MenuFXi SWFAnimManager::ExtendBank(char const*, MenuFX*, int)
// PARTIAL: the original clears the flag byte +0x9b of each new clone (layout request "gameswf::character +0x9b uint8").
// Left out. Also PARTIAL: an empty bank (size 0, reachable only through GrabAnim with a count of 0) makes the original
// read the word before the storage (undefined). The rebuild prints the assert and returns the result so far.
// Otherwise rebuilt: the bank is created from the movie clip named by `name` when it is missing, the first anim of the
// bank is the new one, and the bank is extended to `count` anims by cloning the last anim's character into the parent
// sprite, each clone named <name>_<two-digit index>.
SWFAnim* SWFAnimManager::ExtendBank(const char* name, MenuFX* menu, int count) {
    if (count <= 0) {
        return nullptr;
    }

    const std::string key(name);
    SWFAnim* result = nullptr;  // v43: the anim this call returns (the first one it creates)

    if (m_banks.find(key) == m_banks.end()) {
        // The bank is missing: build its first anim from the movie clip named `name`, found under "_root".
        // The DebugCachedCharacter lives only for this block, as in the original (its destructor runs before the extension).
        {
            DebugCachedCharacter tmp;
            tmp.RefreshCache(name, menu, static_cast<gameswf::character*>(menu->Find("_root")));
            bool created = false;
            if (tmp.m_character != nullptr) {
                // The proxy's +0x04 byte is the weak_proxy alive flag.
                if (tmp.m_characterProxy.get_proxy()->is_alive()) {
                    gameswf::character* clip = tmp.GetChar();
                    result = new SWFAnim(menu, clip);
                    m_banks[key].push_back_unique(result);
                    created = true;
                } else {
                    // The clip died: release the proxy and forget the cached pointer.
                    tmp.m_characterProxy = nullptr;
                    tmp.m_character = nullptr;
                }
            }
            if (!created) {
                // ASSERT(false) FAILED at SWFAnimManager.cpp:122. Level 2 stops the program (the original stores to address 0);
                // level 1 prints the message.
                if (gAssertLevel == 2) {
                    std::abort();
                } else if (gAssertLevel == 1) {
                    std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "false",
                                 "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\SWFAnim\\SWFAnimManager.cpp", 122);
                }
                return nullptr;
            }
        }
        // The insert cannot fail, so the bank is always found from here on: the original's second lookup finds it and
        // falls through to the extension.
    }

    VectorSet<SWFAnim*>& bank = m_banks.find(key)->second;  // v37
    const int size = static_cast<int>(bank.m_finish - bank.m_start);  // v17 before the extension

    if (count > size) {
        if (size == 0) {
            // ASSERT(bank.size() >= 1) FAILED at SWFAnimManager.cpp:134.
            if (gAssertLevel == 2) {
                std::abort();
            } else if (gAssertLevel == 1) {
                std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "bank.size() >= 1",
                             "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\SWFAnim\\SWFAnimManager.cpp", 134);
            }
            // PARTIAL: the original reads the element in front of the storage here (undefined). Not reproduced.
            return result;
        }

        // The template is the last anim of the bank: its character (the anim DebugCachedCharacter at +0x0c).
        SWFAnim* last = bank.m_finish[-1];
        gameswf::character* templ = last->m_anim.GetChar();  // v11 / v12
        // The parent sprite of the template (v38). The original reads the weak m_parent inline and clears it when the
        // parent died. That is what character::get_parent() does through weak_ptr::get_ptr().
        gameswf::sprite_instance* parent = static_cast<gameswf::sprite_instance*>(templ->get_parent());

        // The clone names are the bank name with "_00" appended; the two digits are rewritten for each clone.
        std::string animName = key + "_00";

        int index = size;  // v17: the index of the next clone
        do {
            animName[animName.size() - 2] = static_cast<char>(index / 10 + '0');
            animName[animName.size() - 1] = static_cast<char>(index % 10 + '0');
            const int depth = parent->get_highest_depth();  // highest depth of the parent sprite
            // Virtual slot +0xd4 of the template: character::clone_display_object (sprite_instance overrides it).
            gameswf::character* clone = templ->clone_display_object(tu_string(animName.c_str()), depth);
            ++index;
            if (!clone) {
                // ASSERT(ch != 0) FAILED at SWFAnimManager.cpp:156.
                if (gAssertLevel == 2) {
                    std::abort();
                } else if (gAssertLevel == 1) {
                    std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "ch != 0",
                                 "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\SWFAnim\\SWFAnimManager.cpp", 156);
                }
            }
            // PARTIAL: the original clears the flag byte +0x9b of the clone here (layout request, see the header note).
            SWFAnim* anim = new SWFAnim(menu, clone);
            bank.push_back_unique(anim);
            if (!result) {
                result = anim;
            }
        } while (index != count);
    }
    return result;
}

// 0x498bcc _ZN14SWFAnimManager8GrabAnimEPKcP6MenuFXi SWFAnimManager::GrabAnim(char const*, MenuFX*, int)
SWFAnim* SWFAnimManager::GrabAnim(const char* name, MenuFX* menu, int count) {
    const std::string key(name);
    // A missing bank is built first when a count is asked for.
    if (m_banks.find(key) == m_banks.end() && count > 0) {
        ExtendBank(name, menu, count);
    }

    // operator[] creates an empty bank when the name is still absent (for a count of zero), as the original does.
    VectorSet<SWFAnim*>& bank = m_banks[key];
    const int size = static_cast<int>(bank.m_finish - bank.m_start);

    // The first free anim of the bank is handed out.
    for (SWFAnim** p = bank.m_start; p != bank.m_finish; ++p) {
        SWFAnim* anim = *p;
        if (anim->m_free) {
            anim->m_free = false;  // +0x04 byte cleared: the anim is in use
            return anim;
        }
    }

    if (count <= 0) {
        return nullptr;
    }
    // Every anim is in use: extend the bank by count more (the size of the bank is added to the request).
    SWFAnim* result = ExtendBank(name, menu, count + size);
    if (result) {
        result->m_free = false;
    }
    return result;
}

// 0x498cb0 _ZN14SWFAnimManager8GrabAnimEPKc SWFAnimManager::GrabAnim(char const*)
SWFAnim* SWFAnimManager::GrabAnim(const char* name) {
    MenuManager* manager = MenuManager::GetInstance();
    MenuFX* hudRoot = manager->GetHUDRoot();
    return GrabAnim(name, hudRoot, 1);
}
