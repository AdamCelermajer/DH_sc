# Initialization alias audit: no owner correction required

The previously reported distinction between machine+0x3c and Character+0x538
was an arithmetic error in the investigation. The machine is embedded at
Character+0x4fc, and **0x4fc+0x3c=0x538**. They are one original byte. The frozen
owner's `initialize_level` clearing `State.idle_suppressed` is source-correct.
Do not add another policy byte or clear a different Character field to fix this.
The NEW frame notes were corrected; frozen owner/source/reports remain intact.

The original ELF SHA256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The focused manifest/assembly binds the actual routines and exact bytes.

## Original caller and command composition

`Level::_LoadCharStates` 0x3eff98 obtains each nonnull Character from the actual
ObjectManager Character-list iterator and queries GetPreSetAIState. At
0x3effd4/0x3effdc it forms Character+0x4f0+0xc = Character+0x4fc. Preset0 or17
branches to 0x3efffc/0x3f0000 and forms the same embedded receiver for
`SM_SetPreSpawnState` 0x3c1a64. Other presets call `SM_SetIdleState(false)`
0x3c1a00 with that embedded receiver.

Idle's exact first instruction is `strb r1,[r0,#0x3c]` at0x3c1a00. Level supplies
zero, so it clears Character+0x538 **before** outgoing Blur. It then calls
`_SetState(3,-1,NULL)` unconditionally. PreSpawn calls `_SetState(17,-1,NULL)`
and does not touch that byte in its command prefix. The actual behavior methods
may subsequently change it; those effects belong to the invoked behavior, not
an invented initialization reset. `CSIdle::OnFocus` explicitly reads Character
argument+0x538 at0x3c3080, the same backing byte for the embedded owner.

`_SetState` captures previous/requested IDs, invokes previous Blur before lookup,
stores selected nullable StateInfo at machine+0x20, resets machine+0x60 only for
an existing different ID, invokes incoming Focus, and raises Character event0x1d
with captured previous ID. Same-state transitions preserve elapsed, while still
blur/focus/notifying. A missing target keeps elapsed and clears current. No
additional initialization write to an independent idle field is present.

## Other logical aliases and remaining fields

These fields use a single logical backing in the frozen owner, consistent with
the source embedded layout:

|Logical field|Machine offset|Character offset|
|---|---:|---:|
|flags|0x24|0x520|
|animation_override|0x28|0x524|
|attack_gate|0x2c|0x528|
|cached_speed|0x30|0x52c|
|idle_suppressed|0x3c|0x538|
|dead_alternate byte|0x3e|0x53a|
|move_type|0x40|0x53c|
|elapsed_ms|0x60|0x55c|

Current is a nullable machine+0x20 StateInfo pointer, represented by independent
presence/current-index metadata and signed logical ID; it is not a Character
integer field. `current_animation` at Character+0x4e8, heading-active at+0x1b5,
stop-attack byte+0x442, physical+0x2dc, and controller lock belong to other source
locations/services. They are not machine-offset aliases.

The actual machine constructor0x3c1ac4 writes animation_override+0x28=-1,
current+0x20=NULL, and zeroes words+0x24,+0x2c,+0x30,+0x34,+0x38,+0x3c,+0x40..+0x60.
In particular its word store at+0x3c clears bytes+0x3c..+0x3f. The frozen bounded
projection does not claim to implement every other machine field: secondary
death selection+0x38 and the extra byte+0x3f are distinct fields used by the
substantive death-command producer, and remain an explicit future boundary.
They are not silently merged into idle_suppressed. Constructor zeroing by itself
does not reconstruct those full producers. No further collapsed-field defect
was found in the captured Level prefix or generic transition stores.

Machine+0x4 stores a Character pointer separately. Changing it to a different
Character without moving the embedded receiver does not move the receiver's
bytes. Such an artificial fixture requires separate source projections; the
native production owner must stay bound to its actual containing Character.
The existing transition tests permit identity reentry to verify reloads, which
does not establish whole replacement-Character storage ownership.

## Focused original/O2 proof

`tests/character_state_owner_initialization_alias.py` reuses the historical
source tree-walk/dispatch fixture, without editing it. It executes actual
ARM32 Level, Idle/PreSpawn commands and `_SetState`, then the already frozen
optimized ARM64 owner initializer. The frozen library hash and every source
hash are checked against `character-state-owner-arm64-differential.json` before
execution; no old binary is rebound to new source.

The focused corpus crosses all256 initial byte values, six presets
(-1,0,3,4,17,19), and five previous states (-1,0,3,5,17): **7680 cases**,21504
ordered callbacks,zero mismatches. An actual Unicorn memory-write hook observes
all5120 Idle-prefix writes at PC0x3c1a00 to the exact address obtained by BOTH
receiver expressions. Both PreSpawn preset branches preserve every initial byte
at the prefix. Behavior bodies remain explicit services, so their deeper effects
are not claimed by this field-alias proof.

`state-owner-gold.json` retains both-address write traces and all ten input
words per case. `state-owner-fixtures.bin` uses distinct SAI1 magic and never
overwrites historical SBO1 fixtures. `differential.json` binds gold, optimized
ELF, source, original ELF and script hashes. The109 original registration records
are also unchanged. This is instruction proof of initialization aliasing and
dispatch order, not a full AI/gameplay/backend or APK claim.

Concrete patch proposal: **none to production**. Retain the current owner's
initializer. Correct only the erroneous NEW frame note/diagnosis, as done here.
