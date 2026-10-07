# Native player save-loading coordinator

The native source now implements the complete `SG_Load` (0x465430), `_Load`
(0x464f4c), and `_LoadVolatileQuestsLog` (0x468574) orchestration. It retains the
same `PlayerSavegameV1` used by the player. Its owned profile field is the single
profile authority for this owner; copying a Save or deriving profile presence
from the slot is not permitted.

The implementation preserves every mask branch and ordering: named scalar
sections, levels/skills/faeries/quest initialization, full inventory/properties
sections, isolated skills/properties/quests reload, then the conditional online
volatile quest tail. CFEE's writer remains present even when its reader is null;
its profile is captured before the online callback, while later section calls
read the current profile again. Every reached dependency is required and a
rejected dependency preserves the already reached prefix.

A genuinely fresh Save with slot -1 and the constructor's null profile completes
masks 8 and 32 without any service. That is the original guarded path used for
fresh-player skill/property reload. An existing profile, even with slot -1,
requires its actual named-section reader.

Verification: 1,780 executions of all three original ARM functions produced
23,017 ordered boundaries. Native sanitized and optimized host runs match all
cases and exercise every one of those required failure prefixes. ASan, UBSan
and leak checking pass. The production translation unit also compiles for
Android ARM64 with NDK29. The hash-bound report is
`port/game-data/reports/player-save-load-owner-v1-host-audit.json`.

This proves orchestration, mask selection and owned profile lifetime. Original
file/profile operations, named-section readers, initializers, global game and
online/quest producers are explicit test fixtures. Their complete native
implementations and real save-file round trips remain separate work. Callback
profile rebinding is supported by the API but does not yet have original/native
mutation differential coverage. No Android build, APK or emulator was changed
by this proof.

Integration: retain `PlayerSaveLoadOwnerV1` beside the same Save/player graph.
The V6 skill reload's Load callback must validate Save pointer identity and call
this owner's `load(mask, error)`. Required file services must be supplied when
the real owned profile or reached mask calls for them; do not supply successful
empty callbacks.
