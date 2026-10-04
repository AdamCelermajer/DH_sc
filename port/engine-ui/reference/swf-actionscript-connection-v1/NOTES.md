# Retained native ActionScript connection v1

The facade installs typed native functions in the actual GameSWF global
namespace before original shared/HUD initialization. A graph-start callback
can install exact-player construction/setter observers before any movie is
loaded. Native providers own their callback state independently of the graph.

Opaque values retain their exact graph and preserve native AS tags, object
identity, property getter timing, watcher/setter execution, and argument order.
Operations require the existing facade Scope. Foreign values, released graphs,
missing provider ownership, callback failures, and nested facade entry reject
delivery. Failed replacement preserves the previous movie. Quiescent teardown
retains property targets/functions without executing their getters.

The connected host batch loads the real shared/HUD movies and calls the real
authored potion button handler. Settings/capability/potion callback bodies and
GPU uploads are explicitly host fixtures. Their registration is connection
proof, not a working game backend or complete ActionScript fork equivalence.

The plain-text helper implements source type gates, maximum-length behavior,
and untruncated bound-variable writes. Glyph formatting uses the current
native upstream plain formatter. Full original fork text layout and HTML
reading are not implemented; required HTML delivery rejects before mutation.

The receipt binds the actual sanitized UI DSO, native core/font archives,
executable, compiler records, inputs and source files. Shared CMake changes
are recorded as a build snapshot rather than frozen production source, so
additional completed systems can be integrated without rewriting this proof.

No APK is promoted by this source batch. The visible emulator remains on the
previous validated player/status-HUD milestone until a complete live system
has genuine player/inventory/input producers.
