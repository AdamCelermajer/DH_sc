# Adam verified-v69 authored frontend import

Pinned source: `791e961b12233100b303038c961666834f4beb9d`, with the
`session-contributions/menu-launch/verified-v69` overlay taking precedence.
`import_reference.py` reads blobs without changing Adam's pinned checkout.
`import-manifest.json` records exact upstream paths, blob IDs and byte hashes.
Existing differing game-data owners were retained rather than overwritten.
Original cache, SWF, media, fonts, save files and APK bytes are private build
inputs. The asset manifest publishes only paths, lengths and hashes.

## Selected ownership

`menu_frontend_v69.cmake` selects the imported GameSWF r1714 core, its reviewed
font/input/frame/text overlays, FreeType 2.3.7 and frontend helpers. It links
the existing game-data DSO and sole five-TU inventory-text DSO. Localization,
HudText and inventory text sources are not compiled into the menu DSO again.
The native frontend borrows canonical DebugSwitches load/query callbacks.
Common-text and font constants use the existing immutable PyCST decoder over
retained bytes. There is no additional Lua VM, gameplay inventory, Player,
property owner, RNG, mutable debug map or gameplay save owner.

The separate main/shared SWF players are the authored per-renderer owners
already present in v69. `menu_StartGame` is the real authored sprite 471.
The occupied main button publishes root SlotID and pushes that clip. Its
single-player button calls NativeAssignSaveSlotToPlayer(root.SlotID,0), then
NativeStartGame(root.CurrentDiff). Navigation preserves this authored screen;
it does not assign a player or queue gameplay merely by pushing the clip.
The registered NativeStartGame gateway classifies its actual AS argument and
calls an explicitly declared development Crypt continuation over the canonical
assigned PlayerInfo slot. The full original 568-byte Level/temporary-Save/
SG_Save/LoadLevel caller remains open; this gateway is not its parity receipt.
The GL owner drains the copied launch event after the ActionScript scope
unwinds. A development return after a consumed gameplay handoff retires the
old movie stack before loading one fresh main menu. An open Start clip retains
its timeline through GL recreation. Failed services retain their reached
effects; no fabricated result is published.

## Scoped adaptations

- Localization adds optional Application title/version services and raw symbol
  lookup over its same cache. Existing no-Application behavior is preserved.
- Native Create converts name then class and publishes only the genuine
  creation callback's reached result. Assign preserves first/second numeric
  conversion and negative gates; local ordinal is distinct from internal ID.
- NativeSetSaveSlotIDToMainMenu converts its first argument before optional
  force-bool conversion and borrows canonical preview dispatch. It never
  assigns PlayerInfo664. NativeStartGame's development gateway requires finite
  numeric requests and a mandatory continuation; unsupported difficulty policy
  belongs to that explicit development boundary. Nonfinite rejection is a
  native guard, not an original NativeStartGame branch.
- NativeGetParsedString consumes actual GameSWF array values and uses existing
  parseEx. NaN produces the source zero Variant without EABI conversion.
  Array growth, non-array objects and malformed native calls fail explicitly
  where unsafe original pointer/allocation behavior is outside this port.
  Unknown symbols pass a null string to parseEx, matching original
  getStringFromSymbol's null return. Function pins are static source evidence;
  these wrappers' host gate is not ARM replay.
- GameOption difficulty count comes from the validated second Design group in
  the same immutable Snapshot; the names count must match serialized rows.
- Menu metadata uses existing seven named Save readers and a temporary display
  receipt. Present QEST requires the canonical quest provider and never silently
  substitutes LNAM. Full campaign quest presentation remains unconnected.
- Menu location caption reads the retained named `LevelDeclaration.level_name_id`,
  corresponding to original runtime offset +0x24. The existing level model is
  preserved. Class preview reads immutable tables; it does not grant items.
- Scene Material retains authored effect/technique strings. Modular resource
  loading adds exact selected-node handling while preserving existing DACT
  fields and spawning behavior.
- OriginalUiAssets requires its exact URI catalog; there is no second full-cache
  fallback. Native include paths use selected target include directories.
- MinGW's standard snprintf/isnan spelling is preserved with a Windows-only
  guard in imported `vendor/gameswf1714/base/utility.h`. Android preprocessing
  is unchanged. Host tests borrow repository zlib when no system development
  library exists; Android uses platform zlib.
- MinGW menu linking explicitly exports the selected DSO facade alongside
  GameSWF's own Windows exports. Android linking remains unchanged.
- Imported GameSWF last-player retirement now nulls deleted builtin map
  pointers. Standard tag-loader readiness follows the actual shared table's
  end-tag entry, so the next renderer registers loaders after genuine teardown.
  This fixes the menu-to-HUD boundary without retaining a sentinel player,
  skipping cleanup or creating another registration table.
- The native Android diagnostic callback logs the six repeated authored
  hard-coded-label traces once per movie lifetime (five distinct strings).
  Exact labels may end with one LF or CRLF. Their AS calls still execute;
  errors and all other diagnostics are unchanged.
- FrontAudio.java is the exact v69 file, attributed in the import manifest;
  parent integration owns its Java/JNI lifetime and audio delivery.

## Verification boundaries

The existing selected text gate was rerun after the Localization delta:
1,000 original weapon cases, 1,322 actual item constructors, 936 power text
cases, 1,087 parseEx cases and 355 VarArgs cases, with zero mismatches and
unchanged guarded source inputs. Its report remains
`reports/branch-audit-2026-10-05/inventory-binding-v1-host.json`.
The native syntax report executes both ABIs' actual selected NDK command
lines with their includes and warning gates. It proves syntax, not linking,
APK packaging or live menu behavior. Parent integration owns those gates.
The scoped frontend host test executes the selected menu/text/data libraries,
actual GameSWF objects, caller conversion/result failure prefixes, immutable
difficulty count and named location projection. Its service fixtures are
explicit host boundaries, not a complete Player/campaign reconstruction.
The scoped fixture has 68 checks, including four preview cases, eleven
development Start checks, sixteen exact diagnostic-boundary checks, and twelve
actual player-lifetime checks across repeated last-owner retirement/recreation.
`main-menu-save-slot-original-audit.json` pins the private authored SWF body
offsets/hashes and decoded Assign-before-Start order; no movie bytes are copied
into the host fixture or this public evidence.
