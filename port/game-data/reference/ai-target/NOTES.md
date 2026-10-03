# Character AI range, relationships and target events

The captured original is SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The manifest and assembly retain 13 original routines, including SetTarget and
monster callbacks for reference; the full SetTarget service is not implemented.

`AI_IsInMeleeRange`'s character branch computes three single-precision
subtractions, three squares, `(x*x+y*y)+z*z`, and compares the distance strictly
against the square of both characters' summed melee radii. Inventory contributes
a whole-number reach to each character's configured radius. Current unarmed
actors supply zero inventory contribution. No independent vertical cutoff exists.
`AI_IsInSight` compares against the configured ViewRadius squared, also strictly.
The differential executes original arithmetic and compares both booleans and
distance bits. RTTI, world-position pointers and individual reaches are fixtures;
the discarded melee debug query is explicitly skipped. Arithmetic NaN payloads
are compared by unordered class, with no sign/payload portability claim.

The AI data reader follows the recovered AIProps serialization: 33-byte prefix
(including one-byte DelayedLoad), length-prefixed Script, and 20-byte suffix.
The native in-memory original layout is 68 bytes, not the packed cache record.
The cache contains 76 configurations and 16 variable-length faction lists.
All rows are byte-roundtripped in the sanitizer audit. Invalid AI indices use
Basic (8); invalid faction indices use Neutrals (10), as the captured getters do.
The character enemy query traverses the owner's faction list; the first matching
entry's signed value is negative for an enemy. Two players are never enemies.
The original faction traversal executes for every pair and player classification.

`_UpdateTarget` skips state 0 (limbus), state 17 (awaiting spawn), and no target.
An untargetable object clears current/last targets and emits event 12 with null
argument. Otherwise, death/revival edges emit 10/11, sight edges emit 12/13,
followed by the appropriate range event each update: 14 out, 15 ranged,
16 close, or 17 melee. The native request supplies virtual query snapshots and
whether the supplied callback clears a target on death/out-of-sight. The monster
script does both; a death clear suppresses later sight/range queries. All 20,480
request/previous-state combinations execute the original UpdateTarget routine,
with original virtual/FSM queries and event services supplied as fixtures.
Other callbacks that replace a target or change targetability mid-update remain
outside this snapshot contract. Current and last target presence must agree on
entry; independently retained last targets are not represented by this request.
The live monster adapter satisfies that restriction after SetTarget/SyncLastTarget.

OnEnemySpotted assigns the supplied character only if there is no current target,
then enables target seeking. SetTarget(false) initializes alive/sight snapshots.
OnTargetDied requests controller Stop, clears the target and synchronizes the
last target. OutOfSight additionally requests MoveTo(last position), which still
needs the native pursuit backend. OnTargetInMeleeRange checks enemy relation,
then requests Stop and Attack. The renderer maps these requests to its existing
native animation scheduler. It retains target snapshots across GL recreation.

The current Crypt adapter supplies one hostile candidate (the Prince), using
the configured ViewRadiusNoAggro/ ViewRadius distance. It does not recover the
original spatial-query index/filter/order, query timer, complete UpdateAggro,
leash/obstacle pursuit, controller seeking, AttackDelay/full FSM, target switching,
or the original global producer/RNG interleaving. It may acquire across obstacles;
those filters and pathfinding are pending. Attack animation requests resume after
finite authored sequences; the range guard prevents hits while the Prince is
outside melee reach. This is an executable native reconstruction checkpoint,
not a claim that full original enemy AI is finished.

Live combat now maintains outgoing/reciprocal incoming aggression relations
using the verified module. Defender/player/dead facts are sampled before damage,
so a killing blow does not incorrectly fail the original pre-damage Set guard.
The current adapter updates tables after the application API using its recovered
threat output; callbacks are logged pending services. The original invokes the
aggression service before HitFor. Restoring callbacks that mutate gameplay must
move this service to its original position inside the application call sequence.
Identities are stable 64-bit scene indices, not recovered original allocation
ordering. Multiple-positive-threat ties and death relation cleanup still need
the complete native character manager/lifecycle.
