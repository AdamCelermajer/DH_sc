# Infected Village Ambush host slice

This host-only slice connects an explicitly caller-supplied Character AABB to
the checked `port/script-runtime` scheduler, then projects Ambush requests
through the existing actor registry. It does not change Android code or
emulate the game's physics. Trigger contact and activation are approximations:
the recovered inclusive AABB comparison is known, but the complete native
`Zone::IsInside` path and final world-bound update are not proven.

## Source-backed scope

`005_infectedvillage.mlx` places `_module_infectedvillage_01_001` at
`(-3448.5, 3000, 0)`. Its `infected01.mgp` record `_prim_TriggerZone_ambush`
has local position `(5993.8, -1889.17, 1313.39)`, scale
`(1.92682, 1.92682, 1)`, zero rotation, `script="Ambush"`,
`triggercount="1"`, and `triggerdelay="0"`. The translated trigger center is
`(2545.3, 1110.83, 1313.39)`.

The trigger leaves all player/move-out script and effect fields empty. The
recovered TriggerZone path selects the ordinary `script` branch and requires a
player Character contact. `GameObject::IsTouching(GameObject const*)` compares
closed X/Y/Z AABBs with inclusive endpoints, and
`Trigger::GetNumPlayerActivating` uses that comparison. However, the final
`Zone::IsInside` implementation and trigger bounds update remain unresolved.
This adapter's AABB proxy/contact rule is therefore a labeled approximation;
it receives the Character bounds from its caller and never invents them.
This slice covers the module 01 trigger record; the level also has a second
same-named Ambush trigger in module 02. A shared manager with independent
activation state for both trigger instances is outside this adapter.

Implemented pre-contact gates are the ones visible in the recovered path:

- The local Character's scripted flag blocks the update.
- A disabled GameObject blocks activation.
- A positive activation-count limit blocks after it is consumed.
- A delay timer value of 1 ms or more blocks activation; the source trigger's
  configured delay is zero.
- An associated door in native state 1 or 3 blocks activation. This record has
  no door binding, so normal fixtures have no associated door.
- The native base `GameObject::MeetCondition()` body returns true. No extra
  condition evaluator is added.
- The adapter returns `unsupported: online path`; the online-only virtual gate
  is unresolved.

Blocked gates return before contact sampling, preserving the current edge
latch. On a rising approximate contact edge, `script-runtime::enter_trigger()`
starts Ambush or records that the same script is already running. Either path
consumes the trigger activation count, matching `Trigger::Activate()` after
`SafeStartScriptOnlyOnce()`.

The authored count is one, so later frames are rejected before contact
sampling after the first activation. That one-shot limit is source-backed.
Re-entry behavior after an exit is not claimed as native behavior because the
count gate prevents the contact latch from observing an exit.

## Approximate trigger bounds

The MGP record has no `dimensions` override. `Zone::DeclareProperties`
supplies inherited dimensions `(200, 200, 200)`. The adapter now calls the
shared `port/zone-contact-runtime/zone_geometry.cpp` implementation, which
reproduces the recovered `Zone::InitPost` local AABB calculation by multiplying
dimensions by scale and storing their positive and negative halves. It then
adds that local box to the recovered center using translation only. With the
module's unit scale, zero rotation, and the trigger's zero rotation, the
derived proxy bounds are X `[2352.618,2737.982]`, Y
`[918.148,1303.512]`, Z `[1213.39,1413.39]`.
`make_approximate_trigger_bounds()` builds this proxy for tests. Only the
local-box arithmetic is source-backed; the virtual final bounds update and
`Zone::IsInside` semantics remain unresolved. The translated coordinates and
overlap activations therefore remain explicit approximations. This integration
does not claim exact world transforms, actor contact bounds, or native trigger
contact behavior.

## Ambush-to-registry projection

`ambush_vertical_slice.cpp` seeds the checked scheduler from imported
Character records, advances the original decoded Ambush script, and maps its
five ordered SpawnCharacter events into `actor-runtime::request_spawn()`. It
preserves `_prim_tmp_infected17` as a lookup miss because the imported level
contains no Character with that name. The existing registry changes only its
own `spawn_requested` projection for the four present records. This slice
does not allocate native actors, run their AI, or create render objects.

## Host verification

From the repository root, run:

```powershell
python port/trigger-contact/tests/run_host.py
```

The tests validate source MGP/MLX values, compile and link the shared recovered
Zone local-box implementation into the trigger adapter, decode the original
common and Infected Village script tables, resolve Ambush, and check the
adapter's synthetic contact/gate cases. A second cache-backed host test imports
the Infected Village world records into the actor registry, checks the
translated approximate proxy bounds, supplies explicit outside and inside player AABBs, runs the
checked Ambush scheduler, and asserts the exact five projected request names
and results, including the unresolved `_prim_tmp_infected17` miss. It also
checks that projection does not change source actor counts or allocate render
objects.

## Native references

- `recovered/native/decompiled/libDungeonHunter2.so/functions-002.pseudo.c`:
  TriggerZone update at `0x003AB458`; trigger gate/count at `0x003A87AC` and
  `0x003A87F8`; script-once path at `0x003AB2A0`; trigger property setup at
  `0x003A8A88`; AABB overlap at `0x0039B518`; Zone setup/defaults at
  `0x003A7DF8` and `0x003A771C`.
- Matching recovered ARM assembly is in `TriggerZone-86f63f4f8b1e-001.asm`,
  `Trigger-8b9c643731c9-001.asm`, `GameObject-f8242da72c27-001.asm`, and
  `Zone-a8a06e4a5695-001.asm`. Assembly offsets are generally `0x1000` lower
  than the pseudo-C annotations.
- `recovered/native/decompiled/libDungeonHunter2.so/functions-006.pseudo.c`
  shows `Script_MarkCharacterAsScripted::Set/Unset` setting and clearing the
  Character gate used by TriggerZone.
