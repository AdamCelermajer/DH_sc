# progress — slice `char-ai` (category game-character)

Files written (nothing outside `_parts/` was touched; no git, no build):

- `.../deepseek-audit/_parts/findings-char-ai.csv`
- `.../deepseek-audit/_parts/progress-char-ai.md`

## Coverage

Worklist `queue_char-ai.tsv` = 420 rows. The brief's coverage target (all prio 0–2, plus as many
prio-1 `const:`/`float:` rows as can be done rigorously) is the 88-row subset below.

| prio | in slice | audited (both sides read and compared) | not reached |
|---|---|---|---|
| 0 (ARM differ) | 1 | 1 | 0 |
| 1 with `const:`/`float:` lead | 85 | 77 | 8 |
| 2 (>=120 orig lines, no lead) | 2 | 2 | 0 |
| **total target** | **88** | **80** | **8** |

Prio 3 (32 rows) and prio 4 (230 rows) were not started: the 88-row target already exceeded the
time available (two of its rows are 450–892 lines of original each).

Method per row: read the original span in `recovered/native/hexrays-all/<chunk>`, read the
reconstruction span, and resolve every `const:`/`float:` lead against the member offsets documented
in `port/decomp-reconstruction/game/include/dh2/*.hpp` (and, where the name mattered, against the
host record headers in `pydata-accessors`). Ledger `notes` were consulted for every row that had one.

## Findings

By severity: **high 1**, medium 0, low 0. One root cause, one row.

| address | function | severity | kind |
|---|---|---|---|
| 0x370020 | `PlayerManager::_PackInventory(PlayerInfo &)` | high | compare |

`chunk_005.c:6958` compares the equipment slot with an **unsigned** `v8 - 1 > 1`, so slot 0 takes
the per-item fold; the reconstruction declares `int slot`, where `slot - 1 > 1` is false at slot 0,
so slot 0 is folded through the slots-1+2 sum branch. The sum is then XORed twice and cancels and
slot 0's item id is never folded, so the packed-inventory hash changes under different conditions
than the original and the replicated counter/array (`unk_4a0`, `unk_3b0`) go stale when only the
hand slots change. `g1-game-character-PlayerManager-025dba-1.cpp:453`.

## Addresses not reached (8) and why

Not read to the end on both sides — listed rather than guessed at:

- `0x37280c PlayerManager::_ManageCharacters()` (892 orig lines) — the largest row in the slice;
  starting it without finishing it would have produced an unverifiable "clean".
- `0x376380 PlayerManager::OnHostChangedNotification(int)` (210), `0x3d4434 CharAI::_OnAnimEvent`
  (190), `0x3d37d0 CharAI::AddToGroup(Character*)` (151), `0x36ed0c _UpdatePlayerNumbers()` (131),
  `0x3d3af4 _ParseObjectAnimEvent` (107) — the reconstruction side of some of these was read while
  chasing neighbouring rows, never the original span.
- `0x36f0dc _AttachControllerToPlayerCharacter(int)` (79), `0x36f2bc PostInitCharacters()` (68) —
  not started.

Late additions after the first pass (all clean): `0x3d22e4 OnTargetInSight`, `0x3d14b4
OnEnemySpotted`, `0x3cc280 _HandleProjectile`, `0x3d2cc0 GroupInfo::~GroupInfo`,
`0x3d4120 _OnAnimStepBegin_Move` (squared distance from `m_destination`, GameObject +0x1a8),
`0x3d1050 CharAI::OnUpdate`, `0x3714ac`/`0x37159c ~PlayerManager` (the empty body is correct: the
header declares `m_players` +0x690, `m_playerIdList` +0x6a8, `m_localPlayerIdList` +0x6b4,
`m_questSyncBuffer` +0x6e0 and the null record at +0x8 in that order, so reverse declaration order
reproduces the original's release sequence).

No row was dropped because its address could not be located: every address in the 88-row target
resolved to an original span and to a reconstruction file/span.

## Deliberately not reported (documented or non-observable)

- **Documented deviations**, each with a ledger row and/or a source comment stating the reason:
  `_PackInventory` neighbours aside — `0x3cd384 CharAI::ClearAllAggro` (the port skips a null
  `Character*` the original dereferences), `0x3cd964 CharAI::~CharAI` (member destructors instead of
  the manual release order), `0x3ce9b8 CharAI::IncUpdateQueue` (void return; the original's `int*`
  result is unused), `0x3ce044 SetSkillsAndSpells` and the `PlayerInfo` setters/`Reset`/constructor
  (the stale stack temporary that decides `NetStructMember::SetChanged`, i.e. only
  `NetStruct::s_changeCounter` can differ), `0x3777d0 PlayerInfo` copy constructor (byte arrays deep
  copied instead of sharing the source buffer), `0x3dd588 AISMonster::OnUpdate`/`0x3d01ac
  AI_DoMeleeAttack` (`SetSortType` calls reach the `.clone.1` at 0x3d015c, which ignores its argument
  and installs `_sortFrontal`), `0x3de318`/`0x3dd99c`/`0x3dca50` (register/`sprintf`-vs-`snprintf`
  level details). The 0x3da144–0x3db16c script bindings and 0x3dccf4…0x3dcd48 are plain Lua
  forwarders and matched exactly.
- **`AI_ClearAggro` (0x3d6d68)**: the pseudocode's `OnDeAggro` call shows only the callee in r0 and
  drops the argument; `AI_ClearAllAggro` (same file, same slot 15) passes the de-aggroing character
  explicitly, so the port's one-argument call is the correct reading, not a missing argument.
- **Extra `IsDead()` virtual calls in `CharAI::GroupInfo::OnDied` (0x3d2628)** case 1: the original
  keeps walking the leader list after it has already latched "not all dead"; the port stops at the
  first live leader. The extra calls are on a pure predicate, so nothing observable differs.
- **`++Random::s_debugRandomCounters[0]`** in `CharAI::_UpdateAggro` (0x3cf3f0): the original symbol
  is a scalar; the header's `[2]` array with index 0 is the reconstruction's documented mapping.
- **`0x3acd58 Character::CreatePlayer` / `0x372220 PlayerManager::_AddCharacter`** return values: the
  original returns a leftover pointer/register that no caller reads; the ports return 0/nullptr with
  a comment saying so (`kind` `other`, not a behavioural difference a caller can see).
- All `const:`/`float:` leads that were only struct byte-offsets, magic-number divisions
  (`-1227133513` = the /3 or /7 magic), float bit patterns (`1065353216` = 1.0f, `0x80000000` sign
  flips), `0x80` allocator thresholds and template sizes (`NetStructString<32u>`,
  `NetStructByteArray<36u>/<3u>/<30u>`) were resolved and are **not** findings.
- `PlayerManager::_GetInternalIDByFriendlyID`, `GetNumPlayerCharacterRogue/Mage` (325/290),
  `PlayerManager::_UpdateJoiningController` (button order 37,38,39,40,10,11,12,13,0,1; threshold
  `(max+min+1.0)*0.5`; `SetState(0)` only after the last channel), `_HandleGlobalDeaths`,
  `_HandleLocalDeaths`, `_CheckLocalDeaths`, `_CheckLocalControllers`, `ReviveLocalPlayers`,
  `RemoveCharacter`, `RemovePlayer` (`!NetGateOpen()` reproduces the four-term inline gate) and the
  whole `AIS*` hook family were compared term by term and match.
