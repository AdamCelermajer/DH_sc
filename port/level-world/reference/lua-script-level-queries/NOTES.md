# LuaScript level-query callbacks

This source slice reconstructs three callbacks used by the original monster
`OnInit` script: host player level, current level difficulty, and the level
range for the current difficulty. It does not implement the Lua VM or claim
full monster initialization.

## Recovered behavior

- `_GetHostPlayerLevel` reads the application singleton's `PlayerManager` at
  `+0x40`, calls `PlayerManager::GetHostingPlayer`, reads the returned
  `PlayerInfo` word at `+0x330`, then pushes that signed integer. The source
  callback has no null check for the returned player. The port adapter reports
  a service failure instead of dereferencing a missing player.
- `_GetHostPlayerDifficulty` calls `Application::GetCurrentLevel`. A null
  current level pushes `0`; otherwise it pushes the signed word at `Level+0x118`.
- `_GetCurrentLevelRange` calls `GetCurrentLevel` first and captures the signed
  level-list row id at `Level+0x3c` before reading arguments. Row id `-1` pushes
  `[-1,-1]` and returns without reading arguments or the level table. Otherwise
  only the first argument is considered. A type-3 value is read through the
  original `Value::getNumber` path and converted by `__aeabi_f2iz`; empty or
  nonnumeric input selects difficulty zero. Difficulty 0, 1, and 2 select the
  normal, hard, and very-hard columns. Other numeric results return no values.
- Row addressing uses the ARM32 wrapped byte offset `uint32(row_id * 72)`. The
  LevelTable object is captured once after difficulty conversion. The minimum
  word is read from that object's current backing rows and pushed; the maximum
  word is then read from the same object after that push, so a changed backing
  row pointer is observed. A changed global LevelTable object is not recaptured.

The host kernel uses typed borrowed services for Application, PlayerManager,
PlayerInfo, current Level, arguments, Value conversion, LevelTable storage, and
return pushes. Missing objects fail closed in the port adapter, even where the
original callback assumes its caller supplied a valid object.

## Evidence and validation

The runner executes the pinned original ARM32 caller bodies and compares their
outputs to the host kernel. It also executes the original `Value::getNumber`
body for a numeric argument and models the finite `__aeabi_f2iz` import. The
test includes row id 7, sentinel row id -1, table backing-store replacement
between pushes, invalid difficulty, host-level values, and null difficulty.

Run from the repository root:

```powershell
python port/level-world/tests/run_lua_script_level_queries_host.py
```

Latest report: `port/level-world/build/lua-script-level-queries/validation.json`.
It records 17 host cases and 15 original-vs-host comparisons with zero
mismatches. The original ELF SHA-256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

## Live-producer boundary

The Level constructor’s `+0x3c` row is selected from the incoming level file
path, not from the LevelList name string. The source chain is:

1. `NativeGoToZone` (`0x4422b8`) resolves the selected LevelName through
   `Arrays::LevelList::m_memberNames`, then passes the selected record's
   `+0x20` `LevelFile` to `Application::LoadLevel` (`0x32bdc8`).
2. `Application::LoadLevel` passes that filename to the static
   `GSLevel::LoadLevel` (`0x386818`). The latter stores it in its owned string;
   `GSLevel::Ctor` (`0x386190`) passes that string's character pointer to
   `Level::Level` (`0x3f3128`).
3. `Level::Level` copies the argument into its string at `+0xf8`. It scans the
   0x48-byte LevelList records in order, lowercases each record's `+0x20`
   LevelFile, and searches for that string in the constructor filename using
   `strstr`. On the first match it writes the record ordinal to `Level+0x3c`.

The original cache catalogue has one `007_crypt_01.rule.xml` LevelFile row:
ordinal 23, LevelName `GOTHICUS_CRYPT_01`. Thus a Level constructed from that
filename selects `+0x3c = 23` for the current catalogue. The source algorithm
is a first-match substring search, so duplicate LevelFiles would select the
earliest row; the matching rule is not a LevelName lookup.

`Level+0x118` is also source-traced. `NativeGoToZone` supplies the current
character's `SG_GetGameDifficulty()` as `Application::LoadLevel` parameter 7.
That reaches the final integer parameter of `GSLevel::LoadLevel`, is stored
at `GSLevel+0x40`, forwarded by `GSLevel::Ctor`, and stored by `Level::Level`
at `+0x118`. Other callers may provide another value; the Level constructor
stores its supplied argument.

The PlayerInfo word has a separate producer route. `PlayerManager::_ManageCharacters`
compares its live PlayerInfo `+0x330` with
`CharProperties::PROPS_GetInt(19,false)` and calls
`PlayerInfo::SetCharacterLevel` on a mismatch. The getter arithmetic-shifts
the fixed property word right by 8 and returns an ordinary signed integer.
`SetCharacterLevel` dispatches through PlayerInfo `+0x310`; the integer
NetStructMember vtable path writes its `+0x20`, which is PlayerInfo `+0x330`,
and updates the network dirty/serial fields. `PlayerInfo::Reset` also calls
the setter with `-1`.

These traces recover the source producers but do not provide a live original
`GSLevel`, LevelList owner, hosting PlayerInfo, or PlayerManager lifecycle in
the native reconstruction. The native app's normalized LevelTables parser
validates 84 records and 153 difficulty-range queries, but does not itself
create the current Level or host player. Do not substitute Prince's authored
Character level for PlayerInfo `+0x330`, or infer current-level identity from
DACT room/character metadata. The outer LuaScript/OnInit binding remains a
separate integration step.
