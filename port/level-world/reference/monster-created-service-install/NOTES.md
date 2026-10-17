# Created-VM callback ownership bridge

This is a native port ownership adapter, with **zero new original function-body
credit**. It connects the existing source-built Lua Session to the prepared
Ghost controller callbacks without changing source construction/publication
order. Android autonomous Ghost AI is still pending.

## Original order and retained owners

In the pinned original ELF (`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`),
`CharAI::SetScript<AISExternal>` at `0x3ccaf4/240` calls the constructor at
`0x3ccb68` before storing the returned pending pointer at `0x3ccb6c`.
The existing [constructor evidence](../ais-external-initialization/original-functions.json)
and [lifecycle mapping](../character-script-lifecycle/NOTES.md) cover those
original functions; this bridge does not claim their implementation anew.

The constructor first creates a deferred Session VM. After genuine pending
publication, `Owner::prepare_pending` supplies same-owner services and a retained
callback context. `Session::install_created_services` accepts these only while
that VM is created and unbound. AIS/Character binding and common/external loads
then advance the same VM. Active publication precedes `Owner::bind_staged`.

The installer copies the table, commits the new table and lifetime, and releases
the old lifetime while busy. It does not replace the VM, alias map, cache,
stage or counters. All later stages, a different/zero owner, missing lifetime
and synchronous reentry are rejected. A missing used service still produces a
real Lua error after earlier effects.

`vm_identity()` exposes the actual owned VM address as an opaque full-width
identifier. It is never an object access API. Explicit reset and destruction
clear the owned pointer and release callbacks while busy, preventing a lifetime
deleter from resurrecting or closing the Session recursively.

The retained callback context does not own borrowed world/Character/property/
controller/path/lifecycle projections. Retire frame/timer calls, detach the
frame owner, close the VM while those native projections are alive, then release
the projections. Output storage must survive the synchronous call and all its
callbacks; a retiring context cannot be its sole owner.

## Verification

The dedicated runner executes the unchanged original `_commons.luac` and
`monster.luac` using Adam's source-built float32 Lua:

- 7 bridge cases: same actual VM, new callback context, full-width enemy identity,
  real EnemySpotted effects, retained context and explicit missing-service error.
- 11 guard cases: empty/bound/loaded/faulted stages, owner/lifetime validation,
  preserved statistics/aliases and a real resolved-path cache sentinel.
- 3 retirement cases: services stored inside retiring table backing, reset and
  destruction, with nested reset/create/install/bind/dispatch rejected as busy.
- AddressSanitizer, UndefinedBehaviorSanitizer and LeakSanitizer pass the same
  executable tests, including input-table retirement and destruction.
- Three Ghost Owner host cases now create the actual VM while pending is zero,
  publish pending, install callbacks, bind/load/publish/adopt, and acquire a
  target/path using the same VM. Borrowed projections outlive explicit VM close.
- Independent review checks the focused and Owner reports and source hashes.
- The six existing Session-dependent regression gates pass after the change.

Run on a 64-bit host with matching C/C++ compilers:

```text
python port/level-world/tests/run_monster_created_service_install_host.py --compiler <g++>
python port/android-native/tests/run_ghost_ai_owner_host.py --compiler <g++>
```

These fixtures establish the ownership adapter and source/provider composition.
They do not establish complete original construction/InitScriptProcess parity,
native autonomous pursuit, Android movement or a finished game.
