# GameSWF r1714 (upstream source, subset used by the DH2 build)

Third-party upstream code, not Track A reconstruction.

## Provenance and limits

- Copied as-is (bytes, CRLF kept) from `port/engine-ui/vendor/gameswf1714` (Track A tree). No manifest exists for that tree.
- Track A's pin (`port/engine-ui/gameswf_sources.cmake`) says "official GameSWF r1714, explicit portability/fork patches in reference/gameswf-core". That fork-patch folder is not in the Track A worktree, so this copy cannot be checked against r1714. It may contain unmarked portability edits.
- Binary evidence: the DH2 build does not match this copy in every detail. Its `gameswf::get_gameswf_version` string is `"LINUX"`, where this copy builds `"WIN " __DATE__ " " __TIME__`. 34 base-library functions sit in namespace `gameswf::` in the binary, but are global or in `utf8::` and `image::` here. None of those are changed here.
- Track A overlays (`port/engine-ui/overlays/`) are not included. They are DH2 integration code.

## Selection

- Of the 95 units listed in `gameswf_sources.cmake`, 68 have a compile unit in the DH2 build (`recovered/native/symbols/libDungeonHunter2.so/build-source-filenames.json`). Those 68 are copied, plus the transitive `#include "..."` closure: 170 files, which is the 68 units and 102 headers.
- Not copied (27 units with no compile unit in the DH2 build): `as_class.cpp`, `as_loadvars.cpp`, `as_mouse.cpp`, `as_sharedobject.cpp`, `as_xml.cpp`, `as_xmlsocket.cpp`, `ear_clip_triangulate_sint16.cpp`, `file_util.cpp`, `gameswf_tools.cpp`, `gameswf_video_impl.cpp`, `http_client.cpp`, `http_file_handler.cpp`, `http_helper.cpp`, `http_server.cpp`, `image_filters.cpp`, `logger.cpp`, `net_interface_tcp.cpp`, `png_helper.cpp`, `test_client.cpp`, `triangulate_float.cpp`, `triangulate_sint32.cpp`, `tu_gc_singlethreaded_marksweep.cpp`, `tu_net_file.cpp`, `tu_types.cpp`, `utility.cpp`, `webtweaker.cpp`, `webwidgets.cpp`.
- Gameloft units absent from this tree (not available here): `GameSWFUtils.cpp`, `gameswf_allocator.cpp`, `gameswf_bitmapfont.cpp`, `gameswf_render_handler_glitch.cpp`, `gameswf_scene_node.cpp`, `gameswf_texture_cache.cpp`.

## DH2 changes

None applied. This is upstream as found, with no `// DH2:` markers.

## Unresolved includes (conditional, expected)

- `config.h` (under `HAVE_CONFIG_H`), `gameswf_jit.h` (x86 JIT platform header), `Winsock.h` (Windows).

## Build

Track A builds the subset with: C++17; include directory = this folder (system); definitions `TU_CONFIG_LINK_TO_JPEGLIB=0 TU_CONFIG_LINK_TO_LIBPNG=0 TU_CONFIG_LINK_TO_FREETYPE=0 TU_CONFIG_LINK_TO_THREAD=0`; flags `-w -fno-fast-math -ffp-contract=off` (GCC also `-fpermissive`); zlib (`zlib_adapter.cpp`). The render handler, sound handler and file hooks that the game supplies are not in this folder. Linking the subset needs the DH2 side implementations (for example the glitch render handler, which is not in the tree).

## Host-port changes (Track B, MinGW host build; marked `HOST-PORT:`)

- `base/utility.h`: the `_WIN32` branch's `__PRETTY_FUNCTION__` → `__FUNCDNAME__` alias and the `__asm { int 3 }` assert apply to MSVC only (`_MSC_VER`). MinGW keeps GCC's `__PRETTY_FUNCTION__` and the standard `assert`. No effect on the ARM build the binary came from.
