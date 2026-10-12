# Same-level publication and cleanup

`gslevel_lifecycle_v2.hpp` reconstructs the complete outer GSLevel Ctor386190
and Dtor3860bc call/store order, with required deeper engine services. It uses
the existing retained Level type and directly borrows the existing global
`s_level` storage. Production should instantiate it with
`CanonicalLevelContextV1`; it creates no Application singleton or second level
registry. The GSLevel instance retains field34 while the global retains that
same receiver. Loader preparation by itself never publishes a level.

Ctor flushes animation sets, captures name18 before allocation, reads the other
arguments after allocation, invokes the required Level constructor on that same
allocation, and writes loading38, field34, s_level and active3c in source order.
It then resolves **menu_Loading**, reads the actual online byte5/state34, applies
the original mode3 overlay gate and, if necessary, pushes the loading menu and
invokes **onProgress** with zero arguments. RenderFX is captured before the menu
weak-proxy check; the character is read after it. All these services must use
their actual owners. This is not a new gameplay HUD onPush path.

Dtor calls the actual Level::Unload, resolves **menu_HUD_0**, invokes its
virtual10 method if present, rereads field34 after callbacks, invokes actual
virtual destruction on that receiver, clears field34 and finally clears the
same s_level global. There is deliberately no added identity guard: the source
clears s_level even if callbacks replaced it or field34 was initially null.
Retained native leases keep callback receivers safe without caching those live
field reads. Loading/active flags remain unchanged during Dtor, as in source.

Missing providers fail at their actual source stage and preserve already
completed effects. Constructor/destructor attempts are not replayable;
reentrant lifecycle calls are rejected. No destructor implicitly pretends an
unload succeeded. Allocated storage and the returned Level must be identical.

`reports/gslevel-lifecycle-v2/receipt.json` compares the real original ARM Ctor
and Dtor instructions against the x86_64 Android native implementation:
128 Ctor cases, 64 Dtor cases, zero trace/field mismatches, plus 22 native
failure/reentry checks. This includes null menus, online mode/overlay branches,
allocation-time argument changes, same-pointer/different-control-block rejection,
weak-proxy mutations, and field34/global
changes during Unload and menu close. ARM64 compiles with strict warnings.
The deeper allocation/Level/MenuManager/online/RenderFX/destructor providers in
those tests are explicit fixtures; **whole Level construction, live app
integration and gameplay readiness are not proven by this receipt**.

The root helper `.local-inputs/root_verify_gslevel_v2.py` builds both ABIs and
runs the isolated executable on root emulator5554 without touching the game.
Loader must retarget its own test runner to loader5590. Original engine hash:
36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80.
