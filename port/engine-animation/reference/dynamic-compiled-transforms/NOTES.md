# Source dynamic compiled transform domain

`TransformSet::compile_dynamic` reconstructs the original
`CDynamicAnimationSet` compiler for full node types 1/5/10. It is separate from
the unchanged static `compile` entry point and leaves legacy Player behavior
unchanged. Original ELF SHA256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The 67-function manifest and disassembly include the constructor, actual
dynamic compile/addAnimation/pruning/default paths, common CompileInternal,
getAnimationValue and actual typed accessor/cached-key interpreter bodies.

The game `AnimationSet::CreateAnimSet` at `0x364ca0` constructs dynamic
`0x3648c4` and sets mismatch behavior1. `GetAnimator` at `0x4762c4` executes
dynamic compile through virtual+0x38 before constructing each independent
AnimatorSet. Interpolation constructor/init/caller evidence is recorded in
`../compiled-transforms/interpolation-policy/NOTES.md`: field0 enables
interpolation; only field1 disables it. Both newly constructed slots select
registered library0, cursors are zero, timeline loop defaults to1, and this
initialization does not advance a clock.

## Domain and policy

Library order is supplied explicitly by the caller. Original database vector
append at `0x6601d4`/`0x62ecac` preserves that order, including repeated IDs at
the raw library level. Full compile `0x62f61c` walks this vector in order and
each serialized animation library in increasing record index. The exact
URI/type first-seen union matches original `addAnimation` at `0x62f354` for
the 1/5/10 identity compatibility submatrix. It does not use the static
transformation-template URI filter. Unbound URIs have node `UINT32_MAX` in the
native output; sampling remains possible and scene application is a caller
policy. No component/material/compressed channel compatibility is inferred.

Mismatch prune0 removes each target for which ANY registered library lacks
both a matching animation track and a clip-database default. This phase
`0x62f708..0x62f978` precedes fallback and does not consult the designated
default resource. Retain1 preserves these targets and is the actual game
CreateAnimSet producer's value. Retained ordering is stable after pruning.

Each binding first queries the clip database's first visual scene through
actual `getDefaultValue` (`0x61c6bc`) and `getBlendableAnimation`
(`0x61c1e0`). Mode2 has a track; mode1 has none. A null clip default then falls
back to the explicit designated default database for BOTH modes via
`0x62f850..0x62f86c` to `0x62f7cc`. A missing fallback retains caller bytes in
mode1. The default resource is borrowed only during compile; copied defaults
remain valid after all input Players die. Native Scene is used only for URI
binding and validation, never as an invented dynamic default database.

Original `AddTemplateAnim` at `0x476398` loads/registers its clip before
selecting its resource as the default database. `setDefaultAnimationLibrary`
at `0x62fc90` selects a CC database directly; the index overload is `0x62fcf8`.
Original common CompileInternal `0x6605ac` reads each registered BRES root
signed start/end at +0x1c/+0x20, independently of accessor key ranges. Dynamic
native clip metadata reproduces those exact words. The game ID-map traversal
in `_UpdateAnimationIndices` `0x364af4` maps IDs to existing vector indices;
it does not sort the resource vector. The complete real Prince caller schedule
and designated template identity remain separate integration producers.

The native API retains bounded caller contracts: 1..1024 unique clip IDs,
supported full node channels with no duplicate per-clip URI/type, validated
resource/key format, valid enum policy and sampling storage. Rejections are
atomic. Original source is not claimed to perform these bounds checks.

## Original, optimized ARM64 and sanitizer evidence

The new seven-case corpus reuses immutable original-cache resource bytes and
the independently authored original-format accessor fixtures from the static
input corpus. That static gold remains unchanged at SHA256
`d5999b9cd6f9bec206d13dfd17b4e24101623f9a0126c0fb8831d6e15ca2ca31`.
Five genuine clips are IdleShield, Walk1Hand, stationary combo01, moving
combo01 and dying01, plus exact Prince_modular model. Extra fixtures cover an
unregistered URI, empty animation library, a two-segment scale track with IEEE
clip defaults, and ushort time keys. Registration order is deliberately
reordered in independent cases.

Actual full original dynamic compiler instructions execute. Its vectors have
supplied capacities, while append/clear/resize/dedup/prune/bind/bounds code
still executes. The extended track factory allocation/type identity is an
explicit fixture returning actual original typed vtables. Selected immutable
segment identity is a fixture for sampling; actual typed accessors and prior
cursor neighbor/binary searches execute. Mode/default/bounds/order comparisons
come from the full original compiler, not a reimplementation used as oracle.

All 117,030 original-versus-O2-ARM64 samples match bit-for-bit, including raw
IEEE default/retained bytes, signed zero, exact keys with multiple prior
cursors, both interpolation policies and segment boundaries. 147 ordered
targets and 967 mode/default bindings match. 29,380 samples retain caller
bytes, and 15,070 ordered original libm calls are audited. Retain-nine banks
have33 targets; strict with the same empty/missing bank has0; strict five real
clips have23; other reordered/default cases have1/28/29.

ASan/UBSan host replay passes the same 117,030 samples, 27 atomic rejection
checks and 37 retained event names with zero findings. Events and resources
survive destruction of every input Player. Rejected compilation preserves
previous output/cursor bytes. Direct-source host linkage wraps sinf/acosf/sqrtf
only in the test: wrappers verify source call kind and input bits, then supply
the audited UCRT result. Production math remains unchanged; no arbitrary
body/pose tolerance or historical Bionic libm parity is claimed.

A newly compiled static host regression replays the unchanged prior gold:
11,656 samples, 62 ordered targets, 558 bindings, 8 atomic rejection checks and
9 event names, zero sanitizer findings. Historical static reports are preserved
as historical source bindings; the new dynamic host report records this fresh
static regression against the current additive source.

This is the full supported dynamic compiler and raw sampler domain, not a
whole generic resource cache, animation-set mutation, blended scene/FSM,
GPU pose or whole-game parity claim. No Android build or emulator operation
was performed for this work.
