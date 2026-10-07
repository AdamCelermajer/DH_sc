# Character combat results and real F_Attack callback scope

Source capture: `reference/character-combat-results-ai-v1/original-source.asm`, original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

`CharAI::OnCombatResults(Character*,Character*,void*)` 3d0da4 reads the actual selected AIS+1c and dispatches its virtual+b4; null selected AIS completes. Default Character receiver3dc9a8 constructs two source object arguments, reads result byte+18, selects OnTargetMissed8c5978 when outcomes&3 and live callback flags&1000, otherwise OnTargetHit8c5968 when flags&800. The GameObject default overload3dbf04 is a different literal no-op and is never substituted. The caller must supply actual retained virtual+b4; this helper currently implements only proven3dc9a8, leaving another reached endpoint explicit.

`CombatResultsAiBorrowV1` selects exactly one existing ScriptOwner or ScriptOwnerV2; fresh active view, flags, aliases, and Lua global are read for every receiver. Source objects are pushed through that VM's retained real object service. `character_combat_results_pair_v1` calls attacker then target. Ordinary positive Lua diagnostics retain source continuation, negative native failure stops. First ordinary diagnostic survives successful target delivery. No actor, AIS, VM, property, Save, or World is allocated here.

The existing zero-return scoped binder cannot bind F_Attack: actual native3b9fbc returns damage numbers/boolean. The additive `dh2_script_vm_bind_source_scoped_values` preserves the same primitive result domain and required-failure epoch while granting a real callback capability. Existing APIs retain their behavior. CharacterScriptSessionV3 wraps only exact3b9fbc, forwards the same registered native function/context, retains a per-private-session wrapper, and uses RAII to restore the previous scope. `current_skill_callback_scope()` returns a freshly validated borrow only during this reached native call. A nested callback shadows the outer capability; validation prevents misuse.

Renderer Apply19 binding:

```cpp
CombatResultsAiBorrowV1 ai;
ai.owner_v2 = &actual_actor_session.owner();
ai.scope = actual_actor_session.current_skill_callback_scope();
ai.source_character_callback = actual_retained_ais_virtuals[0xb4 / 4];
```

For the target use its own actual selected session/AIS, and pass the real current player scope only where that same VM is actually executing. The helper uses scoped object calls only when VM identities match. Another genuine idle target VM uses the normal source status/object API. An already-busy VM without its real capability remains a failure. Source required failures caught by Lua are detected through the same VM epoch even on scoped calls.

The ELF virtual-slot audit `actual-virtual-slots.json` additionally proves all six actual AIS primary tables use3dc9a8 in their Character virtual+b4, including AISExternal (table966a48, primaryvptr966a50). ScriptOwnerV2 now retains the actual constructor-selected source primaryvptr/endpoint in its private Session and exposes `source_combat_results_callback(out)` for the currently selected owner. A missing active Session returns false. This getter removes any need for renderer NPC actor-type inference or a literal callback fixture.

Changed existing files: script-runtime/script_runtime.h and script_runtime.c; level-world/character_script_session_v3.hpp and .cpp; level-world/character_script_owner_v2.hpp and .cpp. New helper .hpp/.cpp requires level-world CMake inclusion. No renderer, packaging, or device mutations were performed.

Validation: strict ARM64 C++17 syntax PASS helper/session and both new tests; strict ARM64 C99 syntax PASS actual compiled script_runtime_return_v3.c with source lua headers. Standalone tests are script-runtime/tests/script_scoped_values_v1.cpp and level-world/tests/character_combat_results_ai_v1.cpp (latter takes real13535-byte AI commons). Runtime execution was attempted through WSL and denied Wsl/E_ACCESSDENIED; no runtime or live-gameplay success is claimed yet.
