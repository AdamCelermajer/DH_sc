// MenuDebugHUD bodies, batch g1-game-ui-MenuDebugHUD-c7396f-0 (the 15 ELF addresses of the class).
// Rebuilt from the Hex-Rays pseudocode of libDungeonHunter2.so. Offsets in comments are the original 32-bit ARM
// offsets. Partial functions say in their comment which part is missing.
//
// Two gaps recur in this class, and the bodies that hit them are PARTIAL:
//   - The debug-stats file is reached through the file system at dword_99F73C +0x34 (Application::m_device, then
//     glitch::IDevice +0x34). That member and the file-system vtable slots (open +0x94, close +0x78) are not declared in
//     this tree, the same gap as DebugSwitches::load and save. So LoadFromFile and SaveToFile never open the file.
//     Layout request: "glitch::IDevice +0x34 file-system pointer, used with the open and close slots of the same object".
//   - The visible flag at +0x9b of gameswf::character is set and cleared by these functions. The field is not declared
//     (FlashAnimManager and the MenuBase batches leave the same flag partial). Those stores are not made.
//     Layout request: "gameswf::character +0x9b uint8, visible flag set by MenuDebugHUD::SetText and the huge-number and seal functions".

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "dh2/DebugCachedCharacter.hpp"
#include "dh2/IStreamBase.hpp"
#include "dh2/MenuDebugHUD.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/RenderFX.hpp"
#include "dh2/StreamReader.hpp"
#include "vendor-gameswf/smart_ptr_proxy.hpp"  // gameswf::smart_ptr_proxy and its weak_proxy (get_proxy, is_alive)

// Global assert level. core-platform/memory.hpp declares it too; it is not included here, as in the other game sources.
extern int gAssertLevel;

namespace {

// The debug-stats file holds the int 22 and then the 22 flags of MenuDebugHUD::m_debugStatFlags.
constexpr int kDebugStatCount = 22;
// The count is written as a raw 4-byte write (the original passes 4 as the length, R2:R3 = 4:0).
constexpr std::uint64_t kCountSize = 4;

// The ASSERT of SaveToFile when the count write is short (StreamReader.h line 116, "bytesWritten == sizeof(T)").
// gAssertLevel 2 stops the program (the original stores to address 0 to crash). gAssertLevel 1 prints to stderr.
void ReportCountWriteFailure() {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "bytesWritten == sizeof(T)",
                     "..\\..\\project_vs2005\\Game/..\\..\\sources/Utils/StreamReader.h", 116);
    }
}

// The read LoadFromFile makes on the open stream: the int count (it must be 22), then the 22 flags. When the count
// differs, no flag is read.
void ReadDebugStatFlags(IStreamBase* stream, bool (&flags)[kDebugStatCount]) {
    if (StreamReader::readAs<int>(stream) == kDebugStatCount) {
        for (int i = 0; i != kDebugStatCount; ++i) {
            flags[i] = StreamReader::readAs<bool>(stream);
        }
    }
}

// The write SaveToFile makes on the open stream: the count 22 as a raw 4-byte write, then the 22 flags.
void WriteDebugStatFlags(IStreamBase* stream, const bool (&flags)[kDebugStatCount]) {
    const int count = kDebugStatCount;
    if (stream->write(&count, kCountSize) != kCountSize) {
        ReportCountWriteFailure();
    }
    for (int i = 0; i != kDebugStatCount; ++i) {
        StreamReader::writeAs<bool>(stream, flags[i]);
    }
}

// The weak proxy of a cached character is still alive (the original tests the proxy's alive byte at +0x04).
bool CacheIsAlive(const DebugCachedCharacter& cache) {
    return cache.m_characterProxy.get_proxy()->is_alive();
}

// The else branch of the original: release one reference on the proxy (the smart pointer's assignment frees it when
// the count reaches zero) and clear the cached character. The original does this by hand, so the assignment of nullptr
// replaces its decrement and free and the two stores.
void ReleaseCachedCharacter(DebugCachedCharacter& cache) {
    cache.m_characterProxy = nullptr;
    cache.m_character = nullptr;
}

}  // namespace

// 0x429c18 _ZN12MenuDebugHUD6UpdateEv
// The original is an empty function (a bare return, 4 bytes).
void MenuDebugHUD::Update() {}

// 0x429cd4 _ZN12MenuDebugHUD12LoadFromFileEv
// PARTIAL: the original opens DEBUGSTATS_SAVEFILENAME[0] for reading through the file system at dword_99F73C +0x34,
// reads the flags with ReadDebugStatFlags, then closes the stream through the file system's vtable slot +0x78. The
// file-system member is not declared (see the note at the top), so no stream is opened here and the flags keep their
// current values. The original also returns without reading when the count is not 22; ReadDebugStatFlags does that too.
void MenuDebugHUD::LoadFromFile() {
    IStreamBase* stream = nullptr;  // the stream the original opens here; it cannot be obtained yet
    if (stream == nullptr) {
        return;  // the original returns without reading when the stream does not open
    }
    ReadDebugStatFlags(stream, m_debugStatFlags);
    // The original then closes the stream through vtable slot +0x78 (not reproduced).
}

// 0x429e40 _ZN12MenuDebugHUD7SetTextEPKc
// PARTIAL: when the menu (State +0x04, m_menu) and the cached character of m_text are both set, the original calls
// RenderFX::SetText on the menu with the character, the text and the bool 0, then sets the visible flag (+0x9b) of
// the character to 1 for a text, or 0 for a null text. The flag store is not made (see the note at the top). A null
// text therefore changes nothing here.
void MenuDebugHUD::SetText(const char* text) {
    if (m_menu == nullptr) {
        return;
    }
    if (m_text.GetChar() == nullptr) {
        return;
    }
    if (text != nullptr) {
        m_menu->RenderFX::SetText(m_text.GetChar(), text, false);
        // PARTIAL: the store of 1 at +0x9b of the character of m_text is not made.
    }
    // PARTIAL: for a null text the original stores 0 at +0x9b of the character of m_text. Not made.
}

// 0x429f4c _ZN12MenuDebugHUD4InitEv
// Registers the menu with MenuManager (RegisterMenu, partial in this tree). When the menu has a MenuFX, the original
// caches the five debug characters by path ("text", "times_loaded", "times_loaded.text", "seal_of_freshness",
// "seal_of_freshness_fail") and then loads the debug-stats file (LoadFromFile, partial). The body itself is exact.
void MenuDebugHUD::Init() {
    MenuManager::GetInstance()->RegisterMenu(this);
    if (m_menu == nullptr) {
        return;
    }
    m_text.RefreshCache("text", m_menu, GetCurrentMenuContext());
    m_timesLoaded.RefreshCache("times_loaded", m_menu, GetCurrentMenuContext());
    m_timesLoadedText.RefreshCache("times_loaded.text", m_menu, GetCurrentMenuContext());
    m_sealOfFreshness.RefreshCache("seal_of_freshness", m_menu, GetCurrentMenuContext());
    m_sealOfFreshnessFail.RefreshCache("seal_of_freshness_fail", m_menu, GetCurrentMenuContext());
    LoadFromFile();
}

// 0x42a1f8 _ZN12MenuDebugHUD10SaveToFileEv
// PARTIAL: the original opens DEBUGSTATS_SAVEFILENAME[0] for writing through the file system at dword_99F73C +0x34,
// writes the count 22 and the 22 flags (WriteDebugStatFlags, including its ASSERT), then closes the stream through
// vtable slot +0x78. The file-system member is not declared (see the note at the top), so nothing is written.
void MenuDebugHUD::SaveToFile() {
    IStreamBase* stream = nullptr;  // the stream the original opens here; it cannot be obtained yet
    if (stream == nullptr) {
        return;  // the original returns without writing when the stream does not open
    }
    WriteDebugStatFlags(stream, m_debugStatFlags);
    // The original then closes the stream through vtable slot +0x78 (not reproduced).
}

// 0x42a33c _ZN12MenuDebugHUDD1Ev (complete-object destructor)
// 0x42a3e4 _ZN12MenuDebugHUDD2Ev (base-object destructor, the same bytes)
// The original releases the five DebugCachedCharacters (+0x1b8, +0x188, +0x158, +0x128, +0xf8), then the std::map at
// +0xc8 (erase the tree, reset the header), then calls MenuBase::~MenuBase. The compiler emits the same steps after an
// empty body, in the same order (members in reverse declaration order, then the base). Chain note: the MenuBase
// destructor is partial in this tree (its weak-proxy releases are missing), see the MenuBase rows.
MenuDebugHUD::~MenuDebugHUD() {}

// 0x42a3c8 _ZN12MenuDebugHUDD0Ev is the compiler-generated deleting destructor (~MenuDebugHUD, then CustomFree). It
// is not written here; see its ledger row.

// 0x42a470 _ZN12MenuDebugHUDC1Ev (complete-object constructor)
// 0x42a584 _ZN12MenuDebugHUDC2Ev (base-object constructor, the same bytes)
// MenuBase("menu_DebugHUD"), then the empty map and the five DebugCachedCharacters (default constructed by the compiler
// in declaration order), then Init(). The constructor does not write the debug-stat flags.
// Chain note: Init is exact here, but its callees RegisterMenu and LoadFromFile are partial in this tree.
MenuDebugHUD::MenuDebugHUD() : MenuBase("menu_DebugHUD") {
    Init();
}

// 0x42a4fc _ZN12MenuDebugHUD11GetInstanceEv
// Function-local static, constructed on first use. The guard, the constructor call and the atexit registration of the
// original are what the compiler emits for a static local with a non-trivial constructor and destructor.
MenuDebugHUD* MenuDebugHUD::GetInstance() {
    static MenuDebugHUD instance;
    return &instance;
}

// 0x42a738 _ZN12MenuDebugHUD14HideHugeNumberEv
// PARTIAL: when the menu and the cached character of m_timesLoadedText are set and the proxy is alive, the original
// clears the visible flag (+0x9b) of the characters of m_timesLoadedText and m_timesLoaded. Those stores are not made
// (see the note at the top). When the proxy has died, the original releases it and clears the cached character.
void MenuDebugHUD::HideHugeNumber() {
    if (m_menu == nullptr || m_timesLoadedText.m_character == nullptr) {
        return;
    }
    if (!CacheIsAlive(m_timesLoadedText)) {
        ReleaseCachedCharacter(m_timesLoadedText);
        return;
    }
    // PARTIAL: the stores of 0 at +0x9b of the characters of m_timesLoadedText and m_timesLoaded are not made.
}

// 0x42a7b0 _ZN12MenuDebugHUD17DisplayHugeNumberEi
// PARTIAL: when the menu and the cached character of m_timesLoadedText are set and the proxy is alive, the original
// formats the number as "%d" into m_timesLoadedText with RenderFX::FormatText on the menu, then sets the visible flag
// (+0x9b) of m_timesLoadedText and m_timesLoaded to 1. Those two stores are not made (see the note at the top).
// When the proxy has died, the original releases it and clears the cached character.
void MenuDebugHUD::DisplayHugeNumber(int value) {
    if (m_menu == nullptr || m_timesLoadedText.m_character == nullptr) {
        return;
    }
    if (!CacheIsAlive(m_timesLoadedText)) {
        ReleaseCachedCharacter(m_timesLoadedText);
        return;
    }
    m_menu->RenderFX::FormatText(m_timesLoadedText.GetChar(), "%d", value);
    // PARTIAL: the stores of 1 at +0x9b of the characters of m_timesLoadedText and m_timesLoaded are not made.
}

// 0x42a854 _ZN12MenuDebugHUD29SetDisplaySealOfFreshnessFAILEb
// PARTIAL: when the menu and the cached character of m_sealOfFreshnessFail are set and the proxy is alive, the original
// stores the bool argument into the visible flag (+0x9b) of that character. The store is not made (see the note at the
// top). When the proxy has died, the original releases it and clears the cached character.
void MenuDebugHUD::SetDisplaySealOfFreshnessFAIL(bool shown) {
    if (m_menu == nullptr || m_sealOfFreshnessFail.m_character == nullptr) {
        return;
    }
    if (!CacheIsAlive(m_sealOfFreshnessFail)) {
        ReleaseCachedCharacter(m_sealOfFreshnessFail);
        return;
    }
    static_cast<void>(shown);  // PARTIAL: the store of this value at +0x9b of the character is not made.
}

// 0x42a8c0 _ZN12MenuDebugHUD25SetDisplaySealOfFreshnessEb
// PARTIAL: the same as SetDisplaySealOfFreshnessFAIL, for the cached character of m_sealOfFreshness.
void MenuDebugHUD::SetDisplaySealOfFreshness(bool shown) {
    if (m_menu == nullptr || m_sealOfFreshness.m_character == nullptr) {
        return;
    }
    if (!CacheIsAlive(m_sealOfFreshness)) {
        ReleaseCachedCharacter(m_sealOfFreshness);
        return;
    }
    static_cast<void>(shown);  // PARTIAL: the store of this value at +0x9b of the character is not made.
}
