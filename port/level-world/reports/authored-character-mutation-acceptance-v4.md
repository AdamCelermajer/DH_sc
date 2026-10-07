# Authored character screen acceptance (current source audit)

Read-only audit, 2026-10-06. This lists tests to run; it is not a live PASS
receipt. Original artwork/navigation and source query rendering have separate
root receipts. Use the same live player, Gear inventory, Save, SkillV6 and
PropertyView throughout. Capture before/after native values plus the UI result.

| Action | Bound source path | Acceptance check | Remaining production boundary |
|---|---|---|---|
| Equip/unequip | Queries -> Actions -> same Gear equip/unequip -> properties, requirements, Skin, HP/MP validation | Select an actual item; equip to a source-compatible slot, verify actual equipment cell/selected set and derived stats; unequip it; preview and resumed world must show the same result. Inventory count/item identity must remain correct. | No known missing provider in current basic unpowered-item path. Powered items and material/asset misses must still report their reached source errors. |
| Swap set | same Gear swap/refresh -> actual HUD DisplayRightHud then FillActionIcon | Swap twice; verify selected set0/1, weapons, preview, stats and actual HUD icons after Back. Source saved skill-slot map remains the build's map0; do not infer separate skill assignments from gear selection. | HUD hooks are now bound in NativeApp. Validate actual invocation/input, not just native set-byte changes. |
| Transmute | same inventory quantity/remove -> gold -> property213 -> actual local-player/trophy checks -> source-only Skin | Use one actual low-value item below trophy thresholds; confirm once. Quantity/count decreases once, gold increases by actual preview amount, saved property213 increases256, slots/preview update. Reopen the screen and compare native results. | First ordinary local-predicate producer is now bound through source Matching/CNet identity. Gold >=10000/100000/1000000 and transmute count >=300 reach actual trophy localization/queue/bitmap-save services, which remain required. No live transmute acceptance is established by isolated tests. |
| Drop | actual online query -> NULL-character temporary -> source quantity transfer -> DropInventory | Accept only after real world endpoint is bound. Verify same item leaves Gear once, appears as a canonical type3 world object with real visual/body, source lock5000/friendly index, then can be picked up through actual interaction. | Current renderer does NOT set drop_world/drop_packet or retain the complete145 Item runtime. V9 typed transport is proved, but canonical Item factory, world visual/audio/PF/interaction services are not live-complete. Source DropInventory does not auto-pick up. Do not label a required-error/removal prefix a successful drop. |
| Train skill | actual selected SkillList/Save -> original IncSkill -> points157, saved row level, whole V6 update, recalc, actual capacity store | With genuine available points, train a permitted row once. Verify points delta, saved level and displayed current/next description; points/level-unavailable cases must remain source no-ops. Reopen, assign the learned row, return to HUD and verify its actual ID/icon and working activation. | Lua skill initialization/update can reach additional gameplay providers. Preserve the actual first failure and saved/points mutation prefix; a UI level change alone does not prove skill runtime completion. Current class/actor domain remains the actual development player. |
| Assign skill | availability/assignability/saved-level guards -> same Save.set_skill_in_slot -> whole V6 update | Drag one genuinely learned assignable skill to each of three slots. Verify same Save IDs, old assignment clearing, real HUD icons and source activation after Back. Test locked/unlearned rows without injecting levels. | Same possible V6 callback providers as above. No fabricated unlocked rows or fake activation success. |
| Allocate stat | source points148/attribute149..152 -> saved delta -> original base/class/buff-aware resolve -> real Debug | With genuine spendable points, add once to an actual attribute. Verify points-256 and attribute+256 in canonical saved state plus recomputed UI/HP/MP/ratings. At zero points, confirm the original no-op. Reopen and inspect the same owner. | Current binding is the actual KnightPlayerBase development actor. Other frontend class selections are not automatically proof of their live actor construction. Debug/table/class providers are bound; no additional basic allocation gap found. |

## Save, cancellation and preview limits

- NativeSaveGame runs actual SG_Save and later achievement checks. With the
  retained NULL profile it takes the genuine no-write branch: changes can
  remain in memory, but no disk persistence/relaunch restoration is proved.
  A real profile requires actual campaign named-section/file/job services;
  current renderer deliberately reports those required writer operations.
- Run the authored confirmation/Back flow. Do not claim disk-backed rollback
  from NativeReloadSkills for the development NULL-profile graph. Check its
  actual branch/results; real-profile save/reload must be a separate test.
- Preview borrows actual Gear draw parts and the same player pose, uses source
  avatar camera/rotation, and restores GL state. Test after equip, unequip and
  swap, plus resize and Back. It is not a separately simulated avatar/AI.
  World rendering also rereads Gear parts each frame, so neither view should
  keep stale equipment. Original1322 item rows have empty per-item ItemIcon;
  authored category art must not be represented as unique per-item artwork.

## Evidence required per live test

Record APK hash/current PID, exact authored action and confirmation, canonical
before/after quantities/slots/points/gold/skill IDs, screenshot of resulting
screen and resumed HUD/world, and absence of new required-provider errors.
At a required failure, record the first source boundary and its real mutation
prefix; keep that action unaccepted. Zero available points/locked skills test
guards only, not a positive allocation/training operation.

Isolated receipts (not live-menu acceptance): mutations91, source Drop176,
network-local163; original ARM local predicate72 cases with zero mismatches.

No wholly isolated new file can close the current live-drop gap: it requires
production canonical manager/pool/scene/physics/audio/interaction composition.
The remaining trophy tail is a separable owner/provider task, but it must borrow
actual localization, notification queue and persisted bitmap services; it
cannot be completed by installing a successful empty callback.
