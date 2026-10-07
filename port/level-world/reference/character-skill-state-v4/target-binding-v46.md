# Same-owner cast completion target binding V46

Confirmed integration defect: `model_renderer.cpp` copies CharAI current target
into `SkillStateV4.target` at selection. The original V4 body then writes its
projected `last_target` at blur, but the renderer never publishes that store to
the actual `ScriptCharacterObject::target.last_target`. The authored marker
reads the actual last target, so this projection cannot fulfill original blur.

Original `CSSkill::OnBlur` at `3c434c` executes Debug/CString calls, then
`3c43ac..3c43b0` calls `CharAI::SyncLastTarget` (`3d49c4`) on Character+3c8,
then `GameObject::Stop` (`3938f8`), then RaiseEvent31/Post. The existing source
capture is `original/reference/original-functions.asm` in this directory.

`character_skill_state_target_bound_v46` preserves the recovered V4 body.
Its synchronous bridge performs actual `dh2_character_ai_sync_last_target`
at the original boundary immediately before Stop. It refreshes from the sole
TargetState at that point, including debug callback retargeting; it does not
write back after reentrant Stop/Post. Null/cleared targets stay null. No target
selection, forced retention, dead-target revival, camera mutation or heading
policy is added.

Integration: include `character_skill_target_binding_v46.hpp`, link its CPP in
level-world. In `prince_owner_service`'s state6 body branch, replace the call to
`dh2_character_skill_state_v4(&t.skill_state,operation,event,0,payload,0,&services)`
with `character_skill_state_target_bound_v46(t.skill_state,
t.world->player_object->target,operation,event,0,payload,0,services)`.
Keep the original operation expression and event/payload unchanged.

Validation: strict both-ABI compilation of production helper and test; O1/O2
ASan/UBSan seven contrasts each: surviving current target, retarget during
debug, retarget during Stop, null/dead-cleared target, debug failure before
sync, Stop failure after sync, distinct CharAI and Character identities.
This fixes a source store omission; live surviving-target lock and FX alignment
remain unverified. Capture current408/last40c/OOI14a4 and actual selected actor
life/property36 at cast begin, Use and blur before attributing any remaining
lock loss to source target death, range/sight cleanup or Lua-only PreSearch.
