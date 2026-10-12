# Source host-player and current-level globals

The original engine SHA256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. The native module reconstructs the three Lua wrapper bodies, with manager/Application/session ownership supplied explicitly. It does not choose a host player, publish an active Level, or execute the rest of monster `Init`.

## GetHostPlayerLevel

`LuaScript::_GetHostPlayerLevel` `0x37cc00` ignores all arguments. It reads the player-manager pointer from the global application owner at `+0x40`, calls `PlayerManager::GetHostingPlayer` `0x36e09c`, reads the returned `PlayerInfo+0x330` signed word, then invokes `ReturnValues::pushInteger` `0x37cb24`. The wrapper does **not** call `Character::GetLevel` and does not recalculate a property.

`GetHostingPlayer` checks the debug-client byte `+5` from `0x7fd794`. When false it calls `GetPlayerByInternalID(0,false)` `0x36dfb0`. When true, it checks Application network-enabled `+0x24` from `0x320e98`, the network object's virtual `+0x64`, and network-manager `IsInitialized` `0x8100e0`. Only all true gates use the manager's hosting internal ID `+0x170`. The manager is obtained again after the initialization callback, so the second instance supplies that ID. `hosting_probe.py` executes these original gates for 128 cases, including replacement between the two manager calls; 424 ordered service calls match. Network/debug/manager allocation and final player lookup remain explicit services.

`GetPlayerByInternalID` `0x36dfb0` returns the manager's embedded fallback `manager+8` for sentinel ID `-1` and absent local entries. Other lookup/network branches are captured in `player/reference/original-functions.asm`. A null host player is not an original fallback; the native boundary rejects a missing receiver instead of inventing a cache value.

`PlayerInfo::SetCharacterLevel` `0x370e48` publishes through the netInteger object at `+0x310`; its stored word is `+0x330`. The cache has more than one actual producer:

- `_ManageCharacters` `0x372cf8–0x372d2c` compares the cache to live `PROPS_GetInt(19,false)`, rereads the property when different, then calls `SetCharacterLevel`.
- `0x373040–0x373054` instead compares/writes `PlayerSavegame+0x30` when the selected Character is absent.
- `0x3733d4–0x373434` can publish the cache into savegame and adjust live property19 to agree with the cached value. Therefore a cache is not universally replaceable by a fresh property read.

`Character::GetLevel` `0x3bd120` is exactly `PROPS_GetInt(19,false)` on Character properties `+0x560`. `PROPS_GetInt` `0x3df6e0` selects the cached resolved sheet and arithmetic-shifts the raw word by eight. The new `sync_level` helper calls the already reconstructed `dh2_character_get_level` through the genuine world DSO, then writes the borrowed cache projection. It is a separately selected write projection: the manager's comparison, second read and netInteger publication are not implemented by that helper. The global always reads its supplied cache, including a deliberately stale one.

## GetHostPlayerDifficulty

`0x37cb8c` ignores arguments, calls `Application::GetCurrentLevel` `0x31f594`, and pushes `Level+0x118` when present. A null current Level pushes zero, an actual source branch. `GetCurrentLevel` itself reads the current-Level global pointer; it does not inspect the passed Application receiver.

`Level` C2 `0x3f34c0` writes its final signed integer constructor argument to `+0x118` at `0x3f35d8` (entry stack `+0x14`, after prologue current stack `+0x464`). This is a caller/session producer, not a proof that every live difficulty is zero. `NativeSetCurrentDifficulty` `0x43cd68` writes another application subowner's `+0x0c`; this capture does not reconstruct the whole transition from menu/savegame/network difficulty into Level construction.

## GetCurrentLevelRange

`0x37f1f0` calls `GetCurrentLevel`, then dereferences `Level+0x3c` **without** a null gate. Only actual row index `-1` pushes `(-1,-1)`, before consulting any arguments. Missing Level is a native provider/data failure; it never returns that sentinel pair.

For a real row, no arguments or a first value whose source type is not numeric3 selects tier0. First numeric3 calls the actual `Value::getNumber` `0x31bbf0`, then the imported `__aeabi_f2iz` `0x30e4cc`. Signed truncated tiers0/1/2 select their respective range. All other converted integers return zero values. Additional arguments are ignored. NaN conversion-to-zero and signed saturation for nonfinite/overflow values are the explicit ARM soft-float import contract used in the oracle; the import is not falsely claimed as recovered library code.

The source Level table stride is72. Tier0 reads minimum `+0x3c` then maximum `+0x30`; tier1 `+0x40` then `+0x34`; tier2 `+0x44` then `+0x38`. It pushes the first signed value, then reloads the table data pointer from the retained global slot before reading/pushing the second. The captured row index and tier remain unchanged, even if a push replaces the current Level or row table. The native core issues two range-array services around the first push, preserving this ordering. Its bounded caller contract rejects other negative or out-of-range row indices instead of reproducing original unsafe reads.

The actual decoded rows come from the hash-bound original `FastTravelList`/`LevelList` loaders and record reader evidence in `port/game-data/reference/level-tables`. That separate source capture executes33 travel records,51 level records and128 synthetic records. The native `LevelProjection72.words[12..17]` are raw signed-word bits corresponding to maximum[3] followed by minimum[3]; `memcpy` preserves them into `LevelRangeRow24`.

Actual row `GOTHICUS_CRYPT_01` has ranges8–10,45–47,74–76. `GOTHICUS_CRYPT_02` has9–12,46–49,75–78. These are supplied cache rows, not fixtures substituted as global fallbacks.

The Level constructor initially writes row index `-1` at `0x3f3538`. At `0x3f36bc–0x3f3704` it scans the source table in ascending order, constructs/lowercases the row string at `+0x20` (`ToLowerCase` `0x34e414`), compares to its owned supplied Level string `+0x10c`, and records the first matching row index, plus source flag fields. This identifies a real index producer, but the native host module does not implement its string ownership or establish the active application's supplied level name. Names/schema/cache ownership is handled separately by the Level decoder; an index must be selected by a genuine session producer before live integration.

## Native API and proof boundary

`character_host_context.hpp/.cpp` expose:

- Borrowed `HostPlayer8{cached_level,reserved}`, `HostLevel8{row_index,difficulty}`, `LevelRangeRow24{maximum[3],minimum[3]}`.
- `dh2_character_host_context_query(operation,args,count,services)` with ordered manager/current-Level/range-array/integer-push services. Completion1 includes zero pushes; malformed-1 has no effects; provider/data-2 preserves delivered prefixes.
- `dh2_character_host_context_sync_level(player,PropertyView)` for the explicit live cache-write projection. It preserves the cache on failure and rejects output aliasing the property view/sheets.
- `dh2_character_host_context_bind(vm,bindings)` registers all three exact globals through genuine source-value projection. The binding/provider and borrowed owners outlive the VM callbacks. It owns no manager, application, player, level, row table or Character. Providers do not throw, destroy the VM, or use generic same-VM operations while busy.

The VM bridge captures integer pushes in caller result storage and converts signed integers to float32 before Lua returns. It does not claim original STL ReturnValues allocation, allocator/GC callback parity, or a complete source Application. Its outer Lua protected failure remains explicit when a genuine provider is missing.

The differential executes all original wrapper instructions and original numeric `Value::getNumber`, plus actual current-Level pointer access. Manager lookup and STL integer pushes are bounded services.1,233 original/O2 comparisons include153 actual decoded row/tier combinations, cache and difficulty signed extrema, nonnumber gates, ignored extra arguments, zero/sentinel/invalid tiers, IEEE conversion cases, and reentrant table replacement. All4,041 ordered services match; five native atomic guards pass.

The sanitizer host replays that gold, then executes the real world `GetLevel`, game-data Level decoder, and private float32 Lua DSOs.31,004 checks include179 real-VM cases, all153 actual row/tier combinations, nine genuine cache-sync cases, five failure prefixes and11 guards; ASan/UBSan findings0. All binary/source/cache hashes are bound in the dedicated reports. No APK, complete monster Init, world/session ownership, or full original frame parity is claimed.

Reproduce from repository root with direct Python:

1. `port/level-world/tools/build_character_host_context_oracle.ps1`
2. `port/level-world/tests/character_host_context_differential.py --library .local-inputs/character-host-context/oracle.so`
3. `port/level-world/reference/character-host-context/hosting_probe.py`
4. `port/level-world/tests/character_host_context_host.py`

Parent integration adds the source to the world library and can use host target `character_host_context_audit`, source `tests/character_host_context.cpp`, links world/runtime/game-data/dl, with arguments `reference/character-host-context/host-context-fixtures.bin` and `port/game-data/reference/level-tables`. The runner's `--main-linked` mode binds the actual central world export instead of the isolated wrapper DSO.
