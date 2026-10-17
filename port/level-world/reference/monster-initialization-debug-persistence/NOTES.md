# Positive monster initialization composition

This dedicated host test includes the unchanged root-owned
tests/monster_initialization_session.cpp driver with its main renamed. It reuses
that driver's real Lua VM, original scripts, actual Character/Class/Level/Design
tables, class recalculation and property effects. Only the previously unresolved
debug side of SetLevel regeneration is replaced by a concrete adapter.

The adapter calls the maintained DebugSwitches Runtime and live-map save writer.
It opens real temporary files, writes each source primitive synchronously, and
closes the source stream. It retains independent read backing while nested saves
replace the configuration file. Native std::string objects are allocated,
retained by identity, queried and destroyed in the source HP/MP ordering.
Original cache evidence is read and remains unchanged.

The 18 positive cases cover both Crypt Ghost records and actual Crypt, Swamp and
Infected Village catalogue ranges for three difficulties. They execute unchanged
monster OnInit, reach positive HP and MP branches, perform the actual five saves
from loading the 23-entry cache configuration, insert the missing Stats switch
without saving, and refill the live properties through source add operations.
The same VM subsequently dispatches the original enemy-spotted callback.

The other six cases verify the discarded true tracing value still permits actual
regeneration for both Ghost records, zero-delta initialization skips debug, a real
file-write failure retains source partial effects without closing active resources,
and HP/MP query failures retain their native string backing and preceding effects.
The completed HP add is retained when a subsequent MP query fails.

Application/PlayerInfo/current-Level selection and raw host facts remain explicitly
host supplied. Selected Level catalogue ranges are actual cache data. Real file
backend calls are maintained adapters, not newly reconstructed original OS stream
bodies. This adds no original complete-caller body count and claims no Android,
active-AIS lifecycle, candidate-room acquisition or pursuit behavior.

```powershell
python port/level-world/tests/run_monster_initialization_debug_persistence_host.py --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe --cache C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/cache/files
```
