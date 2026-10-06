XP integration V1
=================

Add player_progression_v1.cpp and player_xp_text_v1.cpp to level-world; include
player_progression_v1.hpp, player_xp_text_v1.hpp and renderer_player_progression_v1.inc.
The include only produces a synchronous same-Gear/property/save borrow. Root
owns all main renderer/CMake/UI edits.

Actual settings are PlayerSkillsRuntime.world->settings: select its authored
row_index("Default"), then pass that retained rows()[index]. Source offsets
8c/90/94/98/9c/a0/a4 remain exact bits. Never supply XP or distance defaults.
Borrow NPC resolved property35 and actual position160/164 through the existing
world actor registry. NPC does not need Save/classes for distribution. Enumerate
the actual retained PlayerManager-equivalent selected local actors; count is
not the number of NPC registrations. Single-player count1 needs its real
selected player producer, not a hardcoded accepted fake party.

Call progression_distribute_xp_v1(killer, victim, player_array, count,
settingsRow, sourceServices, results, error). The caller owns/pins all borrows
through the synchronous call. Below-threshold XP updates SAME Saved/Resolved
33. Source adds floor(scaledXP+1)<<8 then applies property201 ModifiedXP. Even
an out-of-range zero share can receive +1 if some other player qualifies; this
surprising behavior is the original second loop, not a filtering bug.

Required services
-----------------

constant: actual CharacterDesign GetPyCst MaxLevelBNormal, then CHard/DVeryHard
for saved unlocked difficulty1/2. debug: actual Debug LoadScript/GetSwitch with
the supplied exact names (OneKillLevelUp, isTracingChar_Stats,
isTracingXPDistributionCst/isTracingXPDistribution). Statistics callback must
resolve actual PlayerManager GetPlayerByCharacter before original IncreaseStat
3790e0 literal empty body. regen_full: exact existing source RegenHP/MP(-1)
against SAME properties and debug owner. save: whole actual SG_Save backend,
not an in-memory receipt; Save.set_player_level implements only source+30 store.

LevelUp increments19 by256, resets33, resets BASE to defaults/actual actorrow,
applies source class formulas/resolves224, refillsvitals, stores SAME Save level,
then calls original Save, player presentation and carry. Actual cache host
proves the class formulas produce attribute and skill points. Do not add points
manually or recursively loop LevelUp: original one award grants at most one
level; excessive integer carry becomes new threshold integer minus1. Cap leaves
XP0. Saved spent-point deltas remain present throughout recalculation.

level_presentation must implement complete localized MENU_LEVEL_UP console /
optional dialog / PlayerLevelUp menu dispatch, same VisualFX event87, level2
application/tutorial flags branches, level12 _root.menu_CharacterMenu.IsSpecTime,
and local Trophy epic_lvl10..100. They are explicit absent providers today;
do not substitute returntrue. A failure retains source mutations and reports
incomplete; carry is not reached until these providers complete.

xp_text: Distribution captures actual HudManager, computes ModifiedXP, resolves
display anim_sct_xp via414678 and ScrollingCombatText/XPColor. Then invoke
player_xp_text_v1(victim,modified>>8,color,services) borrowing same StringManager
and queue. Whole3af0c8 gets victimposition, raises z by source bounds158-14c,
localizes StrID/GAMEPLAYMENUS_REWARD_XP, calls StringManager.format508ef4 with
integer, then HudManager413dc4 TEXT enqueue. It is NOT numeric413fa0. format
callback is required; enqueue must copy the transient formatted string.

Kill ordering and current limitation
-----------------------------------

Original Character::Kill3a5b18 early IsDead return prevents duplicates, then
stores dead1449=1/HP36=0, queries CurrentLevel and calls DropLoot EARLY when
level.loot_gate150==0, BEFORE the contributor loop and XP. It resolves actual
killer/master/contributors and only when credited calls DistributeXP at3a5e18,
before quest async/death event tails. The existing character_kill.hpp/cpp whole
dh2_character_kill/ctrl_kill already preserves these gates and provider slots;
prefer composing that owner rather than manually imposing XP-before-loot.
Current renderer's
health update/pending_death transition is a development bridge, not whole
original Character::Kill. Root should label it accordingly. Attach ONE
ProgressionDeathReceiptV1 to same NPC lifetime and call its dispatch ONLY once
at the credited source XP call within genuine alive->dead delivery. It marks
attempted BEFORE mutation, even when required save/UI fails. Root retains the
source early DropLoot and contributor/credit order;
never attach two independent render/deathclip hooks or retry failed XP.
Only actual source Revive/new lifetime may reset the receipt.

Proof
-----

tests/player_progression_v1_original.py executes original ModifiedXP3de7ec and
GetLevelScaledXP3bd918 for3200 cases, with real integer/branch bodies and imported
IEEE oracle. Host replay O1/O2 ASAN+UBSAN passes22443 checks each, actual authored
KnightPlayerClass cache source increments/maxHP/MP/points/carry and same Save
store, mandatory provider prefix failures and duplicate receipt guard. Full
GiveXP/LevelUp external callbacks are observers in host, not reconstructed
production save/presentation. Original full distribution/level methods are
saved in reference/player-progression-v1/original-functions.asm; whole-method
ARM differential remains outside the current math proof. Both ABI strict NDK
syntax compile passed. No Gradle/APK/live integration performed by this agent.
