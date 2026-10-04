# Retained native scene Character objects

`CharacterScriptObjects` owns stable native Character records for the renderer's
actual actors. Each record shares their PropertyState and CombatActorState,
stores their raw GameObject position, and owns TargetOwner/TargetState/binding
storage. These allocations survive vector moves and graphics recreation. The
world context and records remain alive through all session VM finalizers.

This is native lifetime architecture, not a replacement for original
SceneManager GetHandle, discovery, deletion, cached-node position producers or
Character frame scheduling. A supplied identity must be one of these actual
retained scene Character objects. Source raw GetID and GetTarget preserve the
supplied identity; unresolved identities and unreconstructed methods fail.
All original Character method registrations remain available in their original
order, with only the recovered GetID/GetTarget object methods delivered here.

SetTarget/ClearTarget use the existing original-derived setter kernel. Their
debug map runs the real missing-file DebugSwitches path, GetCharAIId reads the
shared resolved properties, IsDead uses the genuine source Character query
against the current life byte and decoded AI type rows, and sight uses the
decoded AI ViewRadius with the retained raw points. These records currently
have no cached target-position node. Other backends must supply such positions
before introducing those source producers.

The live Crypt renderer supplies these optional target/object providers to
eleven original monster sessions. GetID/HasTarget/GetTarget/SetTarget/ClearTarget
raise supported globals from27 to32 per monster. Per-load native API checks
confirm GetID and HasTarget against actual scene backings. The existing prototype
enemy controller still uses its development targeting path; it has not been
replaced by full original enemy AI in this stage.

The actual main CMake `character_script_objects_audit` runs the original monster
Init and real Lua object tables, then checks shared dead/position rereads,
raw dead255 XOR1 -> alive254, retained table identity, unsupported method errors,
self-targeting and target clearing from an actual VM finalizer. Its448 checks
and three guards pass under ASan/UBSan. Host range/player selection is a declared
Crypt fixture; it does not prove source Application/session creation.

The finalizer test revealed that lua_close was running while vm.busy remained0,
invalidating scoped source callbacks. The runtime now retains busy1 through
close, as it does during protected Lua operations. Generic VM reentry remains
rejected; dedicated callback scopes are valid. The existing target-session
audit now executes two formerly swallowed object-method finalizer checks,
increasing its total from544 to546. No prior reports were overwritten.

The new checkpoint's original monster/rotation smoke additionally verifies
eleven stable CharAI record identities, unchanged VM identities, unchanged
properties and initial null targets in portrait and landscape. Full timer/AI
frames, source FSM startup, physical ARM64 testing and complete game assets
remain separate required work.
