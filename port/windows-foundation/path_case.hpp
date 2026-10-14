#pragma once

#include <filesystem>

namespace dh::foundation {

// The authored content (level XML, scripts, startup args, original data tables) names files the way the Windows build
// did, where "data/3D/skybox/Skybox_Swamp.bdae" and "data/3d/skybox/skybox_swamp.bdae" are the same file. On a
// case-sensitive file system (Linux ext4/btrfs/xfs) the shipped spelling is the only one that opens. This is the ONE
// place that reconciles the two: every open of GAME DATA goes through it. Files the game itself creates (saves,
// captures, logs) keep their exact names and must not use it.
//
// Rules, identical for every caller:
//   - the exact spelling wins; case folding is only tried for a component that does not exist as spelled;
//   - each path component is matched against its directory listing, ignoring ASCII case;
//   - when two entries differ only by case and none equals the request, the lexicographically first is used
//     (deterministic);
//   - directory listings are cached per directory (re-read when the directory's modification time changes), guarded
//     by a mutex;
//   - on Windows both functions are exact pass-throughs (no behaviour change).

// root/relative, with relative's components matched ignoring case below root. Returns the matching path, or an empty
// path when no entry matches. `relative` must be relative; ".." is not interpreted here (callers reject it).
std::filesystem::path find_path_ignoring_case(const std::filesystem::path& root,
                                              const std::filesystem::path& relative);

// `path` itself when it exists; otherwise the existing path that matches it ignoring case (the longest existing
// ancestor is kept exactly, the remaining components are matched); otherwise `path` unchanged, so the caller's own
// "cannot open" handling still reports the authored name.
std::filesystem::path existing_path_ignoring_case(const std::filesystem::path& path);

} // namespace dh::foundation
