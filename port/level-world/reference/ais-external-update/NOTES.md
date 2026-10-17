# AIS external update

This bounded source unit reconstructs `AISExternal::OnUpdate()` and its
`AISDefault::OnUpdate()` prefix. It is not a complete external AI or enemy
behavior rebuild.

## Recovered caller order

- `AISExternal::OnUpdate()` calls `AISDefault::OnUpdate()` first.
- The default method reads unsigned `AIS+0xbc`. Values through `199` return
  without pausing. Values `200` and above reset the counter to zero, call
  `CharAI::AI_PauseUpdate(1000)`, then call `v2Controller::Cmd_Stop()`.
- The controller lookup is reloaded from `AIS+0x98` after the pause call. A
  synchronous pause callback that changes the owner therefore changes the
  subsequent controller used by `Cmd_Stop`.
- After the default method returns, `AISExternal::OnUpdate()` rereads
  `AIS+0xb8` and calls `LuaScript::Call("OnUpdate")` only when bit zero is
  set. The bundled plain `monster.luac` does not add an `OnUpdate` VTable
  override; this is a source fact, not a replacement for the default path.
- It then calls `CharAIScript::CallStateUpdate()` followed by
  `CharAIScript::CallStateConditions()` on every normal path. Each helper
  independently reads the live `AIS+0xb4` state-table pointer. Null skips that
  helper's Lua call; nonnull reads the callback name at `+0x14` or `+0x2c`
  and dispatches it through the same `LuaScript::Call` dependency.

`ais_external_update.cpp` owns the sequencing and the default counter write.
Its callbacks are explicit source dependencies. A native caller must bind the
pause callback to the actual `CharAI::AI_PauseUpdate`, the controller callback
to the actual character controller, and the three script boundaries to the
live AIS VM/state table. The `call_state_update` and `call_state_conditions`
providers must perform their separate live `+0xb4` checks and callback-name
reads; the core does not assume a state-table registration.

The source dependencies `CharAI::AI_PauseUpdate`, `v2Controller::Cmd_Stop`,
`CharTimers::TMR_Start`, and `LuaScript::Call` are byte-pinned but are observed
test boundaries here. The runner executes the two update callers and the two
20-byte `CallState*` wrappers from the original ARM ELF; it does not run the
original VM callback bodies or the original Android game.

## Host test

From the checkout root, run:

```powershell
python port/level-world/tests/run_ais_external_update_host.py `
  --original-elf ../test_strategy/libDungeonHunter2.so `
  --compiler C:\Users\noamc\.local\mingw\mingw64\bin\g++.exe
```

The runner verifies symbol addresses, declared sizes, the ELF hash, and all
manifest range hashes before comparing compiled orchestration against the
original ARM caller instructions. Fixtures exercise counter 199/200 and
unsigned maximum, raw override flags, owner replacement during pause,
override flag changes during the default call, state-table replacement during
Lua calls, and the source callback order. Separate host cases verify port
failure/alias rejection behavior. They do not claim live Android wiring.
