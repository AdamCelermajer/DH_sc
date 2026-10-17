Original `AI_BeginSkill` 0x3d86bc, `AI_EndSkill` 0x3d8474, `AI_UseSkill` 0x3d8868, `AI_IsSkillActive` 0x3d85d4 and nested `SM_SetSkillState` 0x3c6670 are pinned in `original-functions.json`.
The new kernel preserves retained Begin row vs setter reread, source field/event order and partial effects; script, property, trophy, FSM and stance providers remain borrowed services.
Native skill activation and full Player AI lifecycle are not implemented here.

At 0x3d87ac the original captures AI+4, forms its properties receiver at 0x3d87b8, captures the trophy manager at 0x3d87c0, then calls GetInt(216,false) at 0x3d87c4. The kernel retains that receiver across the manager provider. An ARM/native regression changes AI+4 at the manager load and proves GetInt uses the old owner while the final FSM query uses the new owner.
EndSkill comparisons seed both executions' d1 with 0xa5 and compare the actual original byte. Only using/type-2/continued-zero writes 1; the other branches preserve 0xa5.
`cache-inputs.json` pins the 33 private inputs required by the Knight passive VM fixture, including its 15 loaded scripts. The runner verifies those bytes and the actual loaded script paths without reading an ignored previous validation report.
The kernel is selected once in `dh2_level_world`; the host runner imports that DLL and the existing shared Lua DLL. This selection provides a compilation and regression boundary, with native gameplay activation still pending.
