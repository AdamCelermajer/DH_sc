# Native skill initialization and same-owner contract

This feature composes existing source owners. It does not construct or initialize
an alternative Character, state machine, skill vector or VM.

For a canonical NPC, retain the existing `RetainedCharacterActorV1`, its
`CharacterScriptSession`, its actual native machine and
`CanonicalNpcSkillsV84`. Borrow `CanonicalNpcSkillsV84::owner()` and
`source_ai_fields_v115()`. The latter returns null until the actual CharAI
constructor fields were produced; null is unavailable, not an instruction to
initialize replacement fields. `NativeSkillLuaServicesV1` supports exactly this
Session/CharacterSkillOwner graph, including the V1 uncached property28 lookup
and source fallback list3. It pins the same original SkillTables data.

The original actor's source construction, world/property publication and
selected AI script loading must precede source InitSkills/configuration. Use
the existing canonical initialization scheduler and its existing configure
service; the binder does not call configure on first skill use. Retain nullable
slots and class list positions. DoSkill(0) addresses position0 of this actor's
actual instance vector, not global SkillTable row0. The SkillAIContext descriptor
must point at `&owner().state()` and the constructor-produced fields. It borrows
the real SkillAIOwner character/flags projection and native SkillState/FSM state.
The descriptor owns no lifecycle fields.

For a player already retained by `CharacterPlayerSkillsV6`, retain that owner and
its same `CharacterScriptSessionV3` and existing `CharacterSkillOwnerV6` graph.
Do not construct another V6 owner to expose it. The source player facade requires
the authored AI script `__player__`; it is not a generic NPC owner. It already
owns VM, buffs, timers, SkillAI current/continued/last and saved skill backing.
Use its `native_skill_animation_event()` or source AI Event operation for
`do_skill`; older `CharacterSkillContextV4` incorrectly uses AI_UseSkill(0).
Any new narrow loan of its private instance/context must expose that existing
graph. `NativeSkillLuaServices` supports a borrowed V3/V6 graph if those exact
references are already available.

The original Session input supports a shared `PropertyState`, `CombatActorState`
and temporary property sheet. These must be the actor's live source authority,
not copies of CombatSession presentation stats. The same PropertyView must be
used by mana, inventory, buffs, source Lua and damage producers. Current Windows
CombatSession has no public canonical SkillAI/SkillState/mutable property loan;
root must add a genuine registered-owner borrow or fail unavailable.

The binder supplies actual Check/Pre/Use/Post through
`skill_callback_session_v3`: SetSkill, selected indexed return and source result
cleanup use the existing selected VM and original callback choreography.
Inside a Lua DoSkill native, this unscoped transport intentionally rejects a busy
VM. Inject the enemy integration lane's original scoped indexed-return helper
through `set_callback_transport`. The actual callback capability is borrowed
only within that native call; do not retain it, synthesize it or retry unscoped
when scoped delivery fails.

`NativeSkillLuaServicesV1` supplies native FSM UsingSkill/CastingSpell/current
queries, actual class-row lookup, original properties, source load step and fresh
source flags, TimerStore/native growth provider and marker Event→Use. V1 source
IsPlayer and GameDesign constants remain required host services. The V3 helper
can obtain IsPlayer/constants from its actual Session.

Required host services remain:

- Native SetSkillState50005/state6 transition admission, source old blur/new
  focus and notification ordering on the same machine; no `current=6` shortcut.
- Source raises30/31 delivering Skill Focus/Blur into this same AI/VM, so actual
  Pre consumes mana/targets and Post clears targets at source time.
- Actual debug ownership; Stop, heading, cancel-sneaking, physical pin/unpin,
  monster/miniboss/boss predicates and stance from real equipment. Use existing
  `character_skill_runtime_bound_v48` with borrowed actual physical2dc and
  TargetState48 to preserve late source reloads.
- Original source animation ANIM_Set/SetSpeed/StopLoop and whole completion.
  `SessionSkillAnimation` redirects ANIM_Set to a complete direct AnimTable plan
  through the same Session retained animation owner. Root has added
  `CombatSession::play_actor_source_sequence` and per-profile
  `sourceAnimationClips`. Acquire `actor_binding_lease()` with each bound
  native adapter; detach/replacement expires it and restore rebind creates a
  new lease. Reconstruct adapters from fresh native loans after that boundary.
- Mana native HasMana3b78ec/UseMana3b7864: actual same PropertyView and original
  application/player/debug state. `CharacterSkillNativeBindingsV5` requires its
  Session properties to match the same FreshInventoryOwnedV4 shared sheets.
- Actual SkillInfo/saved level/CalcManaCost, target room registry and source
  filters/range/angle/order, controller LookAt, combat RNG/outcomes, property
  application, buffs, FX, sounds and object capabilities. The V5/V6 bindings
  already have recovered leaves; register them against the same selected VM.
- Player trophy/network/difficulty services when reached. NPC IsPlayer false is
  genuine source classification, not a skipped player path based on faction.

Input routes differ. Character::_DoSkill3b8bd8 invokes AI_UseSkill3d8868 (Begin
then End). Controller press Ctrl_BeginSkill3ad86c invokes Begin, release
Ctrl_EndSkill3ad85c invokes End. Authored do_skill in native Skill state6 invokes
_SkillEvent3d8bf8, which calls Use on the currently selected instance. There is
no basic-attack fallback. Hotbar saved position→class-list position→actual native
instance index must come from the same saved/native owner.

The feature-owned command factory preserves numeric argument gate, original
Value.getUnsigned, source Character SkillList count, then Value.getNumber plus
signed ARM __fixsfsi and raw unsigned command word. These conversions are
required callbacks. The enemy lane has the source assembly proof and scoped
DoSkill registration; use that implementation for active monster callbacks.

Verified here: native source admission/selection dispatch tests and real original
three-clip retained body playback. Native Lua binders compile, but no full live
Lua/World/FSM gameplay execution is claimed by these tests. Root integration and
same-owner provider publication are still required.
