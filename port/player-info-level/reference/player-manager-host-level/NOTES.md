# Player manager host selection and level synchronization

This unit preserves two narrow source paths:

1. `PlayerManager::GetHostingPlayer` selects an internal player ID and calls
   `GetPlayerByInternalID`.
2. The `PlayerManager::_ManageCharacters` level leaf compares the selected
   `PlayerInfo+0x330` word to `CharProperties::PROPS_GetInt(19, false)`, repeats
   that property read after a mismatch, and passes the second result to
   `PlayerInfo::SetCharacterLevel`.

It does not construct a `PlayerInfo`, reproduce manager iteration, or provide
network singleton implementations.

## Host player selection

`GetHostingPlayer` begins with ID 0. It changes that ID only if the fresh
`GetOnline()+5` byte is nonzero, the fresh online-game-state `+0x24` byte is
nonzero, `CMatching::Get().IsHost()` is nonzero, and a `GetNetPlayerMgr()`
instance reports initialized. After that check, it calls `GetNetPlayerMgr()` a
second time and reads that fresh manager's `+0x170` ID. It then calls
`GetPlayerByInternalID(manager, id, false)`.

The nested lookup has its own fresh online/game-state/matching/network-manager
checks. For ID `-1`, it immediately returns `PlayerManager+8`. For another ID,
an initialized online manager uses `GetNetPlayerInfo(manager, id, false)`;
otherwise it searches the local ID tree. A tree miss also returns
`PlayerManager+8`. The kernel models that fallback as an explicit projection
and never replaces a missing network response with a different local player.

The owned host projection keeps stable player identity, internal ID, and an
actual projected `PlayerInfo+0x330` integer member. Its entries are sorted by
unique internal ID, matching ordered-tree lookup. Online/network state remains
fresh typed services, so state changes between the outer and nested helper are
observable. The exact query order, manager reacquisition, ID `-1` bypass, and
lookup argument `false` have focused host tests.

The offline fallback is also a real source object: `PlayerManager::C1` at
`0x374b28` calls `PlayerInfo::C1` at `this+8`, then initializes its empty local
ID-tree header/root. `GetPlayerByInternalID` returns that embedded `this+8`
object for ID `-1` and for a local tree miss. `PlayerInfo::C1` initializes the
integer member at `+0x310`; after its conditional member-initializer branch,
`+0x330` is zero. The branch writes zero and calls `NetStructMember::SetChanged`
only when the pre-existing word was nonzero. In the normal pre-zeroed embedded
manager object, the member takes the no-dirty branch. This pins the valid
offline fallback's level member without claiming that the native viewport
constructs the complete original manager or `PlayerInfo`.

## Level synchronization

The source caller invokes `PROPS_GetInt(19, false)` and compares the returned
signed integer directly with `PlayerInfo+0x330`. If equal, there is no second
read and no setter. If different, the source calls `PROPS_GetInt(19, false)` a
second time and passes that fresh value directly to `SetCharacterLevel`; it
does not compare the second value again. Thus the second read can equal the
old member and still produce a setter call. The property getter itself
converts its resolved fixed-point property using arithmetic shift right by 8;
this caller does not shift its result again.

The reconciliation kernel accepts an already-proven PlayerInfo, Character,
and CharProperties association. It preserves the repeated read and reports
provider errors as port boundaries. If a setter provider mutates the member
before reporting an error, the kernel retains and reports that completed
mutation rather than attempting rollback. No whole `_ManageCharacters`
behavior is claimed.

## Original instruction checks

Run:

```powershell
python port/player-info-level/tests/run_player_manager_host_level.py
```

The runner validates the pinned original ELF and exact symbol ranges, executes
the original offline `GetHostingPlayer` / `GetPlayerByInternalID` ARM paths,
and executes `_ManageCharacters` instructions `0x372cf8..0x372d30` for the
compare/re-read/setter branch. It also executes the `PlayerInfo::C1`
member-initializer slice `0x3742d8..0x37434c` with the actual +0x330 word set
to zero and nonzero values; one PC-relative global-table literal is retargeted
to harness-owned storage, and `SetChanged` is hooked to record its call.
At named boundaries, `GetOnline`, `PROPS_GetInt`, and `SetCharacterLevel` are
harness hooks. The property hook returns explicit signed values, while the
caller instructions prove when reads repeat and which value reaches the
setter. The setter's member and dirty effects are validated separately by
`reference/character-level-member`.

The complete original functions and supporting ranges are pinned in
`original-functions.json`. The runner does not emulate multiplayer services,
the whole manager loop, a PlayerInfo constructor, or a native registration
path. Native wiring remains false until the app has a genuine stable
PlayerInfo/manager projection; a Prince character property alone is not a
substitute for `PlayerInfo+0x330`.
