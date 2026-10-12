# Persistent native character design snapshot

`character_game_design.hpp/.cpp` adds a native ownership adapter around the
previously proved decoders, property rules, class formulas, design registry,
constant loader and Lua callbacks. It does not replace the original Application
or implement a complete DataReloader manager.

`GameDesignInputs256` accepts fifteen caller-provided streams: records/names/schema
for characters, classes, AI, AI factions and levels, followed by an explicit
ordered list of constant streams. Initialization copies every stream into a new
heap snapshot before decoding. All decoded strings, rows, class-row projections,
name pointer arrays, registration descriptors, constants and the
`dh2_script_design_bindings` context belong to that snapshot. Inputs may be
overwritten or destroyed after successful initialization. Bounded preflight runs
before copying: each stream is at most 8 MiB, total input is at most 64 MiB, and
the constant list is at most 4,096 entries. These are native malformed-input
boundaries, not claimed original limits.

`CharacterGameDesign` cannot be copied or moved. Its move-only `Borrow` retains
the pinned snapshot. Retain a Borrow for the entire lifetime of every VM, callback,
property/class consumer and finalizer that uses its views. Moving the Borrow does
not move the snapshot or its bindings. `initialize` rejects while a Borrow is
alive; a failed initialization leaves the previous snapshot untouched. A retained
Borrow also safely keeps the snapshot alive after owner destruction. These APIs
are synchronous and do not establish a concurrent live reload protocol.

The six supported registrations retain their relative order in the original
PyDataArrays constructor. Each descriptor includes its source call and getter
identity as provenance; these ARM32 addresses are not executed by the native VM.

| Namespace | Cache-backed names | Source registration call | Source getter |
| --- | --- | --- | --- |
| AIProps | AI schema section 0, 15 fields | `0x4be600` | `0x4af590` |
| AITable | 76 AI row names | `0x4be618` | `0x4af51c` |
| ClassFuncList | Class schema section 1, `list_entries` | `0x4be834` | `0x4af1e0` |
| ClassTable | 260 class row names | `0x4be84c` | `0x4af16c` |
| CharacterProperties | 224 character field names | `0x4be89c` | `0x4af110` |
| CharacterTable | 448 character row names | `0x4be8b4` | `0x3fa188` |

The native `GameDesignTables` backend performs the proved source first-match
strcmp lookup and preserves the last-registration-wins manager semantics. No
additional namespaces are registered. `registered_names.inc` copies the complete
142-delivery/138-effective-name original catalog from
`port/script-runtime/reference/design-bindings/registry-probe.json`. A query for
one of the original registered namespaces outside the six returns required
delivery failure, with output unchanged. This explicitly identifies a missing
backend instead of treating it as an authored member miss. A truly unregistered
namespace, case mismatch or absent member in a supported namespace returns source
`-1`. GetPyStruct and GetPyOID share the same source namespace dispatch.

Kind 0 dispatches the genuine `dh2_script_constants_get` against the snapshot's
private constant map. Constant streams are replayed exactly in the supplied
sequence with `dh2_script_constants_load`; later duplicates overwrite earlier
entries and positive source name-stop status 1 retains its assigned prefix.
`constant_loads()` exposes each source status/projection. A negative bounded
delivery failure rejects the new snapshot atomically. An explicitly empty
constant sequence leaves the genuine new map empty, whose misses return zero.
The test sequence follows the bundled manifest list solely as a caller fixture;
original Application startup file order is unproved. Snapshot replacement is
explicit native reinitialization, not a claim that original reloadData clears
old entries or that live Application reload can run while consumers borrow data.

Borrow exposes CharacterTable, ClassTables, AiTables, LevelTables, PropertyRules,
class-row views, registration descriptors and persistent script bindings. Level
data contains both 33 fast-travel and 51 level rows from the actual cache. Its
namespaces remain unsupported because only the six listed getter backends are
in scope. `level_model(out, property_view, error)` composes stable class/design
references with a caller-owned genuine mutable PropertyView. The actor still
owns base, saved, gear, resolved and buff state, as well as the temporary property
sheet and DebugSwitch service. The helper does not invent those producers.

`Borrow::bind(vm)` is a convenience installer for GetPyCst/GetPyStruct/GetPyOID.
A genuine ScriptOwner provider may instead install the same stable bindings at
their original delivered registration slots. This adapter does not change the
owner's constructor, publication, registration order or namespace override rules.

`reports/character-game-design-host-audit.json` records an isolated sanitized
DSO/test linked to the actual stable central Lua, game-data and level-world DSOs.
Before/after source and dependency hashes and dladdr/ldd identities are bound.
It passes 1,024 authored names from the existing original six-getter corpus,
26 actual constant streams with 5,608 assignments, 1,059 native lookup checks,
2,061 genuine VM checks, 16 malformed/atomic/lifetime guards and one real Lua
`newproxy` finalizer after owner destruction. The finalizer accesses both a
character row OID and a constant while Borrow keeps the snapshot alive.
ASan/UBSan/leak detection reports zero findings.

The audit also verifies caller stream destruction, reload rejection while
borrowed, failed replacement preserving old data, a positive source name-stop,
duplicate constant order, empty constant input, first-NUL/case behavior,
required errors for known unsupported namespaces, actual GetProp/SetLevel/fixed
math composition using Knight row 263, and exact resulting property sheets.
It retains the existing runtime's guarded VM contract; no general VM reentry is
introduced.

Reproduce the isolated proof with:

```text
python port/level-world/tests/character_game_design_host.py
```

For a later main-linked audit, `tests/character_game_design.cpp` takes:

```text
port/level-world/reference/character-game-design/real-cache-inputs.bin
port/game-data/reference/game-design-tables/table-fixtures.bin
```

The fixture contains exact caller bytes and the original catalog. The report
binds each original asset, underlying original proof, fixture, new source and
actual linked binary. The new ownership adapter is a host composition proof,
not a whole-owner original-instruction differential. Complete Application
ownership/load order, full design manager, live NPC integration, APK instruction
parity and physical ARM64 testing remain separate work.
