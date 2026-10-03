# Authored Android player locomotion selection

Read-only source trace against original ELF SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80` and
supplied complete cache SHA256
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.
Scratch captures, literal resolutions, input hashes, table rows and a 336-case
original UpdateType execution probe are under
`.local-inputs/live-animation-selection-discovery`. No scheduler/renderer source
was changed by this trace.

## Movement thresholds and platform binding

`CSMove::UpdateType` 0x3c0f18 asks virtual IsPlayer at +0x28. For players it
computes single-precision squared XYZ length of Character heading at +0x1b8.
Its actual GOT resolves DesignSettingsTable::members to 0x9a6498. The first
runtime row +0x5c/+0x60 contains PlayerRunToWalkPercent/PlayerWalkToRunPercent.
The corresponding original serialization/schema supplies float 0.45
(0x3ee66666) and float 0.85 (0x3f59999a). Original DesignSettings::read
0x4ee0d0 copies those fields; the probe executes all 43 field writes with a
cache-backed stream-read fixture.

Type 2 (Run) remains Run until squared heading is strictly below squared 0.45.
Other types enter Run when squared heading is strictly above squared 0.85.
Initial type 0 otherwise selects Walk type 1. Unchanged Walk returns without
replaying. Nonplayers select Walk only from initial type 0. Threshold equality
retains the current type, except initial type 0 becomes Walk. Z contributes to
the comparison. Multiplication and accumulation retain original float rounding.

The supplied Android ELF itself selects the platform constants. Its literal
query operands passed to PyDataConstants::getConstant 0x4c4bdc resolve to:

| Consumer | Group | Key | Original string addresses |
| --- | --- | --- | --- |
| Move UpdateType Walk/Run | AnimStancedAnim | SL__LIST_IPHONE | 0x8c4bc0 / 0x8c4bd0 |
| Character GetAnimStance | AnimStances | COUNT_IPHONE | 0x8c3298 / 0x8c32a8 |

This binding is not inferred from a platform-like filename. PyDataConstants::Load
0x4c3708 has the actual `pydata/animations_pycst.bin` literal at 0x4c3be4 and
the registration name `animations_pycst.bin` at 0x4c3c08. That original cache
file supplies mask 210 (0xd2) and stance count 5. Walk bit 0x10 and Idle bit
0x2 are enabled; Run bit 0x20 is disabled. Missing getConstant entries return
zero in the original lookup. The scratch probe observes these actual literal
query operands and supplies parsed cache values as an explicit constants-service
fixture; full PyDataConstants map loading is not claimed as emulated.

The current live joystick destination adapter is a development input producer.
Original UpdatePath normalizes sufficiently distant destination heading to
length 1, which selects Run. That does not reproduce the original touch input's
analog heading magnitude; its touch-to-heading producer remains a separate
source-reconstruction boundary.

## Stance and authored rows

GetAnimStance 0x3a53e0 uses this priority for players: HasStaff -> 3, HasBow -> 4,
offhand weapon/IsDualWielding -> 2, HasTwoHander -> 1, no mainhand weapon -> 5,
otherwise -> 0. It returns the candidate only when candidate < COUNT_IPHONE;
bare-handed candidate 5 therefore becomes default 0. Nonplayers return 0.
HasShield does not affect this priority; a shield remains default stance 0.
These equipment predicates refer to the original current equip set.

Character GetCharAnimTableId 0x3a3228 reads resolved Character +0x1000, validates
the original table count, and falls back to table 17 on an invalid ID.
KnightPlayerBase character row 263 maps to animation row 48 PlayerKnight.
Rows 49 PlayerMage and 50 PlayerRogue share these base locomotion IDs.
Character animation rows have runtime stride 0xa0; Idle/Walk/Run are at
+0x28/+0x94/+0x70. Under the Android mask the reachable IDs are:

| State | Sequences | Direct dictionaries | Files | Step Speed | MoveGO |
| --- | --- | --- | --- | --- | --- |
| Idle default/staff | 262 / 265 | 1040,1041 | prince_idle_shield.bdae, prince_idle_shield_02.bdae | 1.0 | 1 |
| Idle twohand | 263 | 1035,1036 | prince_idle_2hand.bdae, prince_idle_2hand_02.bdae | 1.0 | 1 |
| Idle dual | 264 | 1037,1038 | prince_idle_dual.bdae, prince_idle_dual_02.bdae | 1.0 | 1 |
| Idle bow | 266 | 1039 | prince_idle_ranged.bdae | 1.0 | 1 |
| Walk, all five stances | 280..284 | 1126 | prince_walk_slow.bdae | float 1.3 | 1 |
| Run, unstanced on Android | 271 | 1114 | prince_walk_1hand.bdae | float 1.3 | 1 |

Every listed sequence has Loop=-1. Walk/Run have Type=0 and one direct step.
Idle has Type=2 (random): default, twohand, dual and staff each repeat their
first dictionary in five of six step slots, with the alternate in the sixth.
Bow has two identical direct step slots. Idle BlendOut is 100; Walk/Run is 0.
The existing scheduler preserves those weighted choices and nested redirections.
Run variants 272..275 exist, but are not reached by the original Android mask.
The low-heading Walk clip is the slow clip, not prince_walk_1hand.

ANIM_Set 0x3cacb0 normally resets CharAnimator +0x40 to float 1 before selecting.
If +0x49 is pending, it stores the requested sequence in +0x50 instead.
Move UpdateType calls ANIM_Set, caches PROPS_GetWalkSpeed at Character +0x52c,
then calls ANIM_SetSpeed. Its source walk property is index 46 of the resolved
224-word sheet and gives max(fixed256 * 0.01 + 1,0). Both initial step setup and
ANIM_SetSpeed set controller scale to property multiplier * authored Step.Speed.

## Entering and leaving Move/Idle

Move OnFocus 0x3c3bf8 writes literal Character flags 0x23c1, clears type +0x53c,
calls UpdateType, then calls physical Unpin 0x46eae0 when a body exists.
Move OnBlur 0x3c3aa4 calls GameObject::Stop 0x3938f8, then physical Pin
0x46eb20 when a body exists. Thus the actual stop transition pins on Move Blur;
it is not an invented operation in Idle Focus.

Idle OnFocus 0x3c3020 first checks Character byte +0x538; nonzero returns without
changing flags or animation. Otherwise it writes literal flags 0x2380 and calls
ANIM_Set(Idle base + stance). It neither pins nor calls ANIM_SetSpeed. Idle's
normal multiplier therefore resets to 1 and authored step speed is 1.0. Idle
OnBlur 0x3c2d3c clears +0x538. Idle flags disable position/rotation-from-visual
and physics while retaining source path/update policy bits. Use the existing
verified move-policy getters to decode these flags rather than zeroing all
subobject policy.

## Completion, overshoot and animation identity

Timeline completion captures overshoot before final current_ms is recomputed.
For AnimatorSet, _HandleAnimEnding 0x3672c8 stores extra at animator +0x68 and
pending at +0x88. AnimatorBlender _HandleAnimEnding 0x366628 verifies the active
subanimator timeline, stores extra at blender +0x98 and pending at +0xb8.
Those fields are applicator +0x10/+0x30. Applicator CheckCallback 0x36440c later
calls its installed callback and clears its pending byte. Character's actual
__Callback 0x3c90ec only sets CharAnimator +0x49; authored advancement/replay
is handled during the later actor CharAnimator::Update phase.

_SetAnimStep 0x3ca79c does NOT recompute extra. At 0x3ca990 it reads
CharAnimator +0x44 into PlayClip's r3 argument, passes loop=false, and only
after PlayClip applies the authored/property speed product. The blended
PlayClip 0x47680c does not consume that r3 argument. Its same-local-clip branch
instead reads applicator +0x10, adding that captured extra to clip start.
These are distinct stores. Do not call CalculateExtraTime on the finalized
timeline to replace applicator extra: that creates a different replay stride.
Whole ELF ARM branch/pointer/relocation scans found no callers/references to
CharAnimator::CalculateExtraTime 0x3c90f8 beyond its symbol-table definition;
the native helper is verified callable behavior, not an assumed live call.

Global dictionary ID (e.g. 1114) and a controller's local clip index are distinct.
AnimatorSet::SetCurrentAnimation 0x3674ac obtains AnimationSet::GetAnimation's
entry for the dictionary ID and returns its +0x20 local index. ReplayFacts
mapped_clip/previous_clip must refer to that binding identity. The present
single-clip scene adapter can compare dictionary identity consistently, but
must not claim that dictionary number is an original controller-local index.
Full dynamic library binding/refcounts and blend setup remain external.

## Evidence and asset preparation

The original-only probe runs 336 combinations of player classification,
equipment predicate facts, prior type and float neighbors around both
thresholds; it records exact selected sequence IDs and literal constant queries
with zero mismatches against the recovered branch policy. Actual UpdateType,
GetAnimStance priority/clamp, GetCharAnimTableId, property getter and
ANIM_SetSpeed execute. Inventory predicates, constant values, dispatch and
runtime animation rows are explicit authored/caller fixtures. This is source
control-flow evidence, not an independent native differential module.

`port/android-native/tools/bundle_locomotion.py` derives all reachable Knight
Idle/Walk/Run roots from original tables and the actual constants, follows
redirections within the original three-layer limit, rejects duplicate archive
entries and filename collisions, verifies the complete cache hash, and writes
nine original clip assets plus player-locomotion-provenance.json. Repo and
Studio asset copies are byte-identical; provenance SHA256 is
`d70c80bed9f4a49c40d4b5dedfef4c4b46d7ff186ac9fc1dc5cb3956bce292d0`.
Invoke with --cache and one or more --project paths. Extraction alone does not
confirm live scheduler/timeline/physics/render integration; the parent owns
that integration and its builds.

The existing ASan/UBSan scene/root asset audit passes the expanded bank:
46 clips, 4,446 samples, 1,340 graph nodes, zero errors. All 4,446 sampled root
positions also pass the original ARM32 versus standalone ARM64 delta and root
displacement kernels; the stable base 1,796-record corpus is unchanged. Results
are preserved in scratch root-samples.json/bin and root-differential.json.
The original root is root_camera at graph index 33 for these Prince clips.
Walk slow spans 0..1066 ms with accumulated XYZ (0,-250.667,0); Run onehand
spans 0..800 ms with (-0.000037998,-603.274,0). Both shield Idle clips have
zero accumulated XYZ (durations 1066/2333 ms). These are authored clip samples
before timeline speed scaling, not invented game-units-per-second constants.
