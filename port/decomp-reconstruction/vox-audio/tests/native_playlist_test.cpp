// Native playlist containers: element reset, playlist accessors and the manager's
// Init, count and flag. The manager calls that need unported NativePlaylist methods
// are not linked here.
#include <cstdio>
#include <cstdlib>

#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/native_playlist.hpp"

namespace vox {
struct PlaylistElementTestAccess {
    static std::int32_t Field(const PlaylistElement& e, int index) {
        switch (index) {
            case 0: return e.field_00_;
            case 1: return e.field_04_;
            case 2: return e.field_08_;
            case 3: return e.field_0c_;
            default: return e.field_10_;
        }
    }
};
struct NativePlaylistTestAccess {
    static void Set(NativePlaylist& p, bool valid, int current, std::int32_t* elements) {
        p.valid_ = valid;
        p.current_ = current;
        p.elements_ = elements;
        p.elements_end_ = elements + 3;
    }
    static void SetTable(NativePlaylist& p, std::int32_t* begin, std::int32_t* end) {
        p.elements_ = begin;
        p.elements_end_ = end;
    }
    static void SetFlag(NativePlaylist& p, int flag) { p.field_14_ = flag; }
};
struct NativePlaylistsManagerTestAccess {
    static NativePlaylist** Playlists(NativePlaylistsManager& m) { return m.playlists_; }
    static void SetCount(NativePlaylistsManager& m, int count) { m.count_ = count; }
};
}  // namespace vox

namespace {
int g_failures = 0;
#define CHECK(cond)                                                       \
    do {                                                                  \
        if (!(cond)) {                                                    \
            std::fprintf(stderr, "%s:%d: CHECK failed: %s\n", __FILE__,   \
                         __LINE__, #cond);                                \
            ++g_failures;                                                 \
        }                                                                 \
    } while (0)
}  // namespace

int main() {
    // PlaylistElement: the constructor and Reset set the same five words.
    vox::PlaylistElement element;
    CHECK(vox::PlaylistElementTestAccess::Field(element, 0) == -1);
    CHECK(vox::PlaylistElementTestAccess::Field(element, 4) == 1);
    element.Reset();
    CHECK(vox::PlaylistElementTestAccess::Field(element, 0) == -1);
    CHECK(vox::PlaylistElementTestAccess::Field(element, 1) == 0);

    // NativePlaylist accessors.
    std::int32_t ids[3] = {7, 8, 9};
    vox::NativePlaylist playlist;
    vox::NativePlaylistTestAccess::Set(playlist, true, 2, ids);
    CHECK(playlist.GetCurrentElement() == 9);
    CHECK(playlist.GetCurrentPosition() == 2);
    CHECK(playlist.IsValid());
    vox::NativePlaylistTestAccess::Set(playlist, false, 0, ids);
    CHECK(!playlist.IsValid());
    CHECK(playlist.GetCurrentElement() == 7);

    // GetPlaylistElement needs the int flag at +0x14 and an index inside [0, count).
    vox::NativePlaylistTestAccess::SetTable(playlist, ids, ids + 3);
    vox::NativePlaylistTestAccess::SetFlag(playlist, 0);
    CHECK(playlist.GetPlaylistElement(1) == 0);
    vox::NativePlaylistTestAccess::SetFlag(playlist, 1);
    CHECK(playlist.GetPlaylistElement(1) == 8);
    CHECK(playlist.GetPlaylistElement(3) == 0);
    CHECK(playlist.GetPlaylistElement(-1) == 0);

    // Manager: starts valid with no playlists; Init allocates the pointer array and
    // leaves the count alone.
    vox::NativePlaylistsManager manager;
    CHECK(manager.IsValid());
    CHECK(manager.GetNbPlaylists() == 0);
    manager.Init(4);
    CHECK(manager.IsValid());
    CHECK(vox::NativePlaylistsManagerTestAccess::Playlists(manager) != nullptr);
    CHECK(manager.GetNbPlaylists() == 0);
    vox::NativePlaylistsManagerTestAccess::SetCount(manager, 4);
    CHECK(manager.GetNbPlaylists() == 4);
    // The manager destructor frees the pointer array; count 0 keeps it from touching the
    // uninitialised entries.
    vox::NativePlaylistsManagerTestAccess::SetCount(manager, 0);

    std::printf("native_playlist_test: %s\n", g_failures == 0 ? "ok" : "FAILED");
    return g_failures == 0 ? 0 : 1;
}
