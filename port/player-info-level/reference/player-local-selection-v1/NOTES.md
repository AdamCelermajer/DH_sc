# Local-player ordinal selection and slot assignment

This module borrows the existing `PlayerRegistry` and `PlayerInfoProjection`
identities. It adds no registry, player, inventory, properties, VM or profile.
Local-controller byte +66c, Character +660, actual internal word +670, local
network vector +6b4/+6b8, and selected save-slot +664 are mandatory borrowed
fields/services. The internal word is distinct from the registry map key.

`_GetInternalIDByLocalID` performs fresh online/GameState/Matching IsInRoom/
NetManager eligibility queries. Offline it first unsigned-bounds-checks the
ordinal against tree size, then traverses canonical ascending map order,
counting only nonzero +66c entries. Its true flag additionally requires a
nonnull Character +660. It returns the actual selected +670 word. A miss is
-1. The network route uses the local-ID vector, rather than locality's all-ID
vector; the true flag performs actual internal-ID lookups and Character reads,
reloads span bounds on each continuation, and reloads the vector start at the
captured index on a hit. A span descriptor includes bounds for explicit port
validation; the original final hit reloads start only. Source-invalid empty or
truncated spans fail explicitly rather than accessing beyond the borrowed span.

`GetLocalPlayer` then calls the existing selected `GetPlayerByInternalID` body
with lookup flag zero. Its independent fresh queries are retained; -1 and
missing keys return the same manager+8 fallback. Local ordinal is never
substituted as internal ID. The narrow new public wrapper in the existing
host-level files publishes that canonical pointer without reading Level;
existing `GetHostingPlayer` still forwards flag zero and retains its behavior.
Thrown lookup providers now return the same explicit service-failed status as
provider error returns, preserving their completed effects and stopping delivery.

`AssignSaveSlotToPlayer` captures Application once. It loads actual
SavegameManager from captured Application+4c and stores last-slot +8 before
loading that same captured Application's PlayerManager+40. It selects
`GetLocalPlayer(ordinal,false)` and stores actual selected PlayerInfo+664. The
plain signed arguments are preserved; the SWF wrapper's nonnegative check is a
separate caller. Any reached provider failure stops immediately and preserves
the first slot store. Slot assignment to fallback does not create a registered
Character association, load a profile or initialize a Character.

The gate executes four complete original ARM bodies and calls the actual
selected shared host-level pointer selector. Network/Matching query callees are
declared fixtures, not claimed reconstructed constructors. Service-return and
exception failure prefixes, absent mandatory fields, output aliases, span
bounds, captured Application changes, local ordinals versus map keys, separate
actual internal words, fresh online changes and fresh vector changes are tested.
Only actual Ninja-selected dependencies, CMake inputs and scoped evidence are
hashed around a clean selected rebuild. Original ELF bytes stay private.

## Next registration cut (static audit only)

`_CheckLocalControllers` scans at least one input and forces input zero present.
An absent offline map ID calls `_AddPlayer(index,-1,index,true)`. `_AddPlayer`
requires genuine PlayerInfo construction/copy/assignment/destruction, map
insertion into the same manager registry, then `_UpdatePlayerNumbers` updates
actual +678/+67c ordinals. These callees remain mandatory and are not delivered
by this module.

`_ManageCharacters` separately requires PlayerInfo.IsActive virtual+5c, local +66c,
slot +664, and metadata Save+680 (constructed as PlayerSavegame(slot,1,false)
when absent). It synchronizes class/Level and requires actual current Level and
class before offline `_AddCharacter`. That producer spawns `Character` named
`PlayerCharacter_%d`, stores Character+660 at 0x3722ec, then initializes its
gameplay Save, queries virtual locality and runs slot/InitAll continuations.
Failures after publication must retain the reached association. Controller,
light, camera, room and PostInit obligations remain open. None is synthesized
to make slot assignment appear to register a development Prince.

PlayerInfo.IsActive at 0x36d48c performs a fresh Online read and returns true
immediately offline. Online it requires nonnegative +178/+1a0/+1c8 and +1f0==3.
The inherited CNetPlayerInfo.IsActive has that scalar predicate without the
offline override. State +240, changed by CNetPlayerInfo.SetState, is a distinct
member from the active-network scalar +1f0. These static dependencies are pinned
but are not executed by this gate or supplied as fabricated initialization.
