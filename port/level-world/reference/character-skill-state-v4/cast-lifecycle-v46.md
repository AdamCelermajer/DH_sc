# Authored cast lifecycle and alignment audit V46

Bundled skill scripts are plaintext despite their `.luac` suffix. The extracted
original cache files and SHA256 are recorded in `skill-lua-v46.json`; no
decompilation or inferred behavior is needed for these three callbacks.

| Family | Source aim timing | Source impact selection | Source Post target rule |
| --- | --- | --- | --- |
| BashDown | Pre: Enemy/AttackableOnly, FrontalFirst, range160, local target then LookAt | Use attacks the captured Lua local target once | Unconditional ClearTarget |
| Charge | Pre: Enemy/AttackableOnly, FrontalFirst, range300 then LookAt first | Use searches current direction, NoSort, range300/angle120, rolls all candidates | ClearTarget only when not TargetInMeleeRange |
| GroundSlam | Pre consumes mana/cooldown without LookAt | Use: ClosestFirst, authored range, LookAt first candidate once, rolls all | Unconditional ClearTarget |

All three PreSearch lists are distinct from the actual CharAI target fields:
none of these scripts calls SetTarget. A cast can therefore successfully hit
a Lua-selected target while current408 is null. `ClearTarget` is the genuine
scoped receiver in `character_target_bindings.cpp`: SetTarget(NULL,false)
followed by SyncLastTarget on the SAME owner, clearing current408 and last40c.
Preserving a living Bash/GroundSlam target by skipping Post would change the
authored script. Investigate source control/attack reacquisition separately.

CSSkill Focus original order: debug; flags/gates; Character RaiseEvent30
(`3c4500`, SkillPre); SM_SetAnim(-1) (`3c4510`); speed1; byte412 disable;
CancelSneak; moving gate; unpin; monster flags. Native V4 follows that order.
`BlendedPlayback::apply_selection` calls the authored step FX leaf before
beginning blend/new clip; this matches the recovered `_SetAnimStep` branch.
Anchored step passes source ZERO and owner; unanchored step passes actual
GetTargetPosition and raw rotation16c. The effect's orient flags decide whether
Sync uses visual-root rotation, fixed raw rotation or floor continuation.

Current composed LookAt updates desired heading and controller heading;
anchored orient-once Sync reads the actual visual-root quaternion. Their values
can differ because source Actor rotation interpolation and visual root sync
occur in the actor frame. A stale-root defect is presently a hypothesis, not
permission to snap the root or replace it with desired heading. GroundSlam's
Use-time LookAt especially prevents a universal Pre-time facing rule.

`renderer_cast_target_trace_v46.inc` records event-local current/last/OOI,
registered life and HP36 (with explicit known flags), projected SkillState
targets, raw rotation, desired heading and actual root quaternion. Its
`source_cast_fx_play_trace_v46` records exact step set, anchor, supplied position
and optional rotation. Capture these at Focus, real step play, Use before/after
and Blur before/after. Distinguish legitimate Post clears, death/range cleanup,
callback reentry and different Lua impact targets before changing selection or
animation timing. This is not live visual acceptance.
