# Shared melee frontal acquisition correction

Production CharacterWorldPlayerAttackOwnerV1 now calls target_frontal_sort_v41 in attack_list_reset_sort. Source clone3d015c selects _sortFrontal38d5b4 through GOT996a6c; native sort2 implements that comparator. Old sort0 accepted registry/insertion order. The helper drains the actual existing heap through source Pop before selecting frontal, preserving count/capacity guards and required failure diagnostics.

Root link requirement: add **target_frontal_sort_v41.cpp** to level-world. No global renderer/CMake changes were made by this task. The old V41 ZIP remains immutable and predates this production fix.

Strict compilation passed ARM64 and x86_64 for both changed production units. O1/O2 ASAN+UBSAN passed eleven target identity contrasts each using actual native search, comparator and heap: front versus closer side/back; equal angular ties; exact maximum range and exclusion outside range; hidden/dead/friendly exclusion; narrow cone; malformed count, capacity and sort guards. Eligibility callbacks are explicit fixtures. This does not prove live acquired targets, terrain line of sight, a projectile path or all ranged skills.

Additional urgent read-only lethal audit found no hit_controller_kill branch in current player_manager_hit_v2: service8 falls through to required original Hit source service8. The production CtrlKill helper exists separately but is not called by that actual backend. This is an identified source-provider seam, not a pointer-leak diagnosis or permission to bypass source DropLoot/XP/quest.
