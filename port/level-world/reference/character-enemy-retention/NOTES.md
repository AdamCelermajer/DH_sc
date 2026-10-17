# Existing-enemy retention search

This is one bounded `_UpdateAggro` continuation and one specialized source
TargetList domain. It is not a complete CharAI body or a live actor integration.
The original ELF and exact function ranges are pinned in
`original-functions.json`. The runner executes original caller/filter instructions;
callee providers in that comparison are explicit boundaries.

## Correct identities and data

`PlayerManager::GetLocalPlayer(0,true)` at `0x36e478` returns PlayerInfo.
`PlayerInfo+0x660` is **Character identity**, established independently by the named
`GetLocalPlayerCharacter()` at `0x330794`: it makes that call and loads exactly
`+0x660`. It is not a property set, registry ID, index, modulo-derived ID, or an
arbitrary nearest player. The owner tree rooted at Character+`0x448`, with its
sentinel at `+0x444`, is CharAI's outgoing map (embedded AI starts at `+0x3c8`,
tree sentinel AI+`0x7c`). Its Character keys use unsigned pointer ordering.
`data::AggroTable` supplies that existing sorted native storage projection.
Membership is read without insertion; the result is captured before queue pops.

## Search flags1 / object filter2 / closest sort1

Constructor `0x4a2730`, SetRefObject `0x4a191c`, SearchEff `0x4a3428`, common Search
`0x4a2f34`, character predicate `0x4a1ab8` and object predicate `0x4a1950` supply
the source gates. Existing melee(flags1/filter0) and aggro(maxflags/filter2)
modules remain unchanged. Only their neutral types and existing heap-pop API
are reused; accepting a noncharacter through the melee type8 path is excluded.

SetRefObject reloads its reference object after IsCharacter. SearchEff obtains
look direction by value before obtaining a borrowed owner target-position point.
Common Search runs its debug switch query (ignored normal return), clears the
old queue, checks radius>=0, and obtains reference AI melee radius once.
The source assertion/log/crash branches for negative or NaN radius are excluded;
the port reports an invalid source fact after the preceding source effects.
Cone is fixed to raw `0x40c90fdb` (2pi). GetChar still runs for null/self/disabled
objects. Candidates are tested for self, stored enabled/visibility byte `+0x8a`,
IsZonable and bytes `+0x2ee/+0x2f0`, then IsInteractive(reference object).
Noncharacters fail filter2 without an interaction-type virtual query.

With a reference Character, flags1 checks signed owner+`0x1314` >= candidate+
`0x1310`, IsDead(candidate), **fresh** reference AI_IsEnemy(candidate),
IsPlayer(candidate), and, if candidate is a player, **fresh** reference
IsPlayer(owner). Alive enemy players are accepted only for nonplayer owners.
Without a reference Character, the predicate accepts immediately. There is no
dead/friend/neutral merchant/invisibility alternative in this exact flags domain.

InteractionRadius precedes candidate GetTargetPosition. Both borrowed point
backings are read after that candidate callback; replacing the owner's selected
point later does not replace the captured pointer. Binary32 distance uses x/y/z
subtractions, xx, yy, xx+yy, zz, sum+zz, sqrt, minus target radius, minus reference
radius. Only strict distance>radius rejects. The Point3D angle provider is still
called at full cone, preserving side effects; its sign bit is cleared.
Closest heap ordering uses strict distance greater and the existing source heap
pop. No identity tie breaker is introduced. Intrusive Next reads the current
entry's next link after callbacks; there is no first-enemy early break.

## Caller continuation `0x3cf9a8..0x3cfbac`

1. Fresh owner constructs flags1/filter2/sort1 list.
2. Capture Application, Level+`0x38` and Level room-list sentinel `+0x70`.
   Capture owner **before** capturing global AIProps base, then GetCharAIId on
   that owner. Read captured base's selected row+`0x40` after the getter.
   The existing GetCharAIId provider owns its original fallback8 semantics.
3. Search radius ViewRadiusNoAggro with 2pi. The room-list Reset rereads the
   captured sentinel's next link. No substitution of Level+`0x60` is made.
4. Empty search: original AI ClearAggro(resolved current), SetTarget(null,0),
   SyncLastTarget, return. It does not query a player manager.
5. Nonempty: fresh PlayerManager field+`0x40` from captured Application,
   GetLocalPlayer(0,true), fresh owner, PlayerInfo Character+`0x660`, then
   unsigned outgoing-map membership. Pop only entries before the first exact
   Character identity match. A match remains unpopped until local destruction.
6. Known map key returns regardless of whether the queue contains the player.
   Unknown and no match returns. Unknown and matching player makes a fresh-owner
   AI_IsEnemy query. False returns. True captures the next fresh owner **before**
   global Design+`0x30`, then calls that owner's AI_AddAggro(player, raw amount).
   Zero, negative and NaN amount words are passed unchanged to its source body.

## Ownership, tests and remaining work

Application, PlayerManager, PlayerInfo, AI table rows, owner/map, source objects,
positions and intrusive topology are borrowed. Replaced objects/backings must
stay alive until synchronous completion. Providers may change live fields and
owner bindings; one thread owns a queue, scratch and output. Reentry into the
same queue/scratch/output is forbidden; independent sessions may nest.
Alignment/overlap/capacity/order guards are port bounds, not invented source
assertions. Exceptions and provider errors preserve completed side effects;
there is no rollback or extra game callback on unwinding. Local native storage
needs no source deque allocation/destructor emulation.

Files: `character_enemy_retention.{hpp,cpp}`,
`tests/character_enemy_retention.cpp`, `tests/run_character_enemy_retention_host.py`.
The host gate tests actual specialized enumeration, fresh character gates,
borrowed point/table mutation, source identities, strict distance, heap scanning,
empty cleanup, raw Design floats, errors, aliases and capacities. ARM comparison
executes the original flags1 character predicate/filter2 object predicate and
caller continuation including tree lower_bound/unsigned comparisons. Constructor,
SearchEff, PlayerManager, queue pop/destructor and action callees are modeled
providers in the **caller oracle**, not full source-body proof. The original
common Search numerical body/library and heap internals are not replayed there.
Angle implementation/PlayerManager internals/AddAggro effects and live ownership
bindings remain external; the module is not wired into Android/native/CMake.
