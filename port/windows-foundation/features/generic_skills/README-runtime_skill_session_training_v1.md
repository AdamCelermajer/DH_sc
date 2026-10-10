# Same-Session skill training commit

## Source evidence and behavior

This helper adapts the recovered original `IncSkill` operation to the current
host's canonical owners. The source Skills menu calls `NativeSkillsTrainSkill`
with the selected class-list position. `dh2_player_increment_skill_v2` performs
the source admission sequence: save/rows present, resolved `Skill_Points`
property 157, `SkillTable.RequiredLevel`, the unlocked CharacterDesign cap,
saved uint16 rank, and `Character::CanIncrementSkill`; on acceptance it subtracts
one point, increments the saved row, updates skill-derived state, recalculates
the source class sheet, reads property 194, then stores its byte capacity.
The native owner implementation uses the same effects in
`port/engine-ui/character_menu_actions_owner_v1.cpp` and the operation order is
explicit in `port/level-world/player_initial_grants_v2.cpp`.

There is no direct visual counterpart to this atomicity helper. Its visible
consumer is the Skills point count (`NativeSkillsGetSkillPointsLeft`, backed by
resolved property 157) and the saved rank shown by the Skills page. Fresh
creation resolves property 157 into the source profile and subtracts its
initial free-skill grant in `features/frontend/creation/runtime_creation_persistence_v1.cpp`;
loaded profiles publish their saved point total through
`player_profile_properties.cpp`. Both paths therefore provide the same initial
point/rank pair this helper validates.

The commit stages both `CharacterState` and the live controlled actor's
`OriginalCombatProperties`, runs the original increment kernel against those
staged owners, then publishes the staged World properties and performs a
no-throw swap into the same `CharacterState`. Rejection, owner mismatch, or
provider failure leaves those published owners and the Session RNG unchanged.
The typed result includes the exact source property-194-derived potion-capacity
byte; the present portable inventory has no separate mutable capacity cache,
so property 194 remains authoritative and the byte is diagnostic output only.

The source `UpdateAllSkills` call refreshes the original script-owned
`PlayerSkills` cache. The modern `CombatSession` has no such cache: skill menus
and casts resolve ranks from the retained `CharacterState` on each query, and
class animation roots are preloaded. Thus the provider documents this as an
explicit modern-host cache invariant and does not invent a callback or second
rank cache.

`CharacterTable.SkillTree`/property 28 is an unscaled source list index, while
level and point properties 19 and 157 are Q8 values. The helper verifies the
exact class list and saved-row ordering before using the selected list position.

## Verification status

The dedicated Knight Session test is
`runtime_skill_session_training_v1_tests.cpp`. It loads the actual Knight
SkillList and `design_pycst.bin` CharacterDesign cap constants, initializes a
same-class CombatSession, loads a source-known profile through SaveStore, and
checks a nonmutating source probe, out-of-list and zero-point rejection, one
accepted training commit, class/gear property recalculation, a post-training
SaveStore roundtrip, and GameSave restore. The private copied-archive C++17
runner passed; it does not compile Session sources or change shared build
outputs. With the optional output path and source-level argument, the test
emits isolated level-3 or level-6 Knight diagnostic profiles for menu
acceptance. In each fixture the actual initialized Session property157 is read
before creating the state; the native row-0 starter grant is reflected by a
source `dh2_property_add(-256)` debit. Its rank/point totals are test
diagnostics, not gameplay-earned progress.

For that optional file only, HP/MP fields are now populated from
`resolve_original_actor_properties` using the same class and exact Session
level raw value with `refill_vitals=true`. That function recalculates the
original CharacterTable/ClassTables formula sheet and applies the tested
source SetLevel refill of current HP/MP to maxima. The test checks the saved
profile roundtrip and the current≤maximum constraints. It does not modify the
training Session's HP/MP or the ordinary training test state. The fresh
level-3 output `diagnostic-knight-level3-refilled-final.dhsave` records HP
173.293/173.293, MP 29.75/29.75, 2 skill points, BashDown rank 1, and
GroundSlam rank 0. The fresh level-6 output
`diagnostic-knight-level6-refilled.dhsave` records HP 185.586/185.586, MP
33.5/33.5, 5 source points after the same row-0 starter grant, BashDown rank
1, GroundSlam rank 0, Charge rank 0, and only the source slot0→saved-row0
binding. The level override is explicit diagnostic metadata; this profile
does not claim its level or points were earned through gameplay.

The separate `runtime_skill_session_training_classes_v1_tests.cpp` extends
the same transaction check across Knight, Rogue, and Mage base-class source
rows. It resolves each CharacterTable.SkillTree and source `SkillList` from
the loaded corpus, derives level and property157 from that class's initialized
same-owner Session, applies the native row-0 starter rank/slot grant and exact
property debit, then trains the first actual list position admitted by the
source-required-level/rank/point/cap evaluation. It verifies that the saved
row, CharacterState point count, resolved World property157, SaveStore
roundtrip, and GameSave restore remain consistent for each class. Its strict
C++17 private copied-archive runner passed all three classes without a
class-specific helper branch. At level 3 each initialized class Session
reported property157=3 points; the row-0 starter grant left 2, and training
the level-admitted position 1 left 1. Those rows resolve from the actual source
SkillLists as Knight `GroundSlam`, Rogue `ViciousStrike`, and Mage
`StoneEscape`. This is a focused host transaction test, not a GUI or
integrated-runtime acceptance claim.

The source caller/admission path is evidenced by the original Skills page
`onUp` handler recorded in `README-runtime_skills_text_v1.md`: it selects the
class-list position, refreshes the description, calls
`NativeSkillsTrainSkill(true, position, player 0)` for the selected Add Skill
visibility, and uses the localized `GAMEPLAYMENUS_ADD_SKILL` label. The native
initial-slot grant in `port/level-world/player_initial_grants_v2.cpp`
establishes hotbar set0/slot0 and calls the source increment for saved row0
only when its level is zero. The recovered increment kernel then gates on
save/rows, resolved property157, SkillTable.RequiredLevel, difficulty cap,
saved rank and CanIncrementSkill; on commit it debits property157, increments
that saved row, updates skill state, recalculates properties, and stores the
property194-derived capacity. The helper tests exercise those shared source
semantics against actual class tables and current host owners.

With an optional second argument, the test writes an additional level-3 Knight
diagnostic profile only when the destination does not already exist. It uses
the source level-3 Skill_Points total less the initial source row-0 grant and
retains the original Knight SkillList/rank assignments. This fixture is for
menu-button wiring checks; it does not assert that the level or points were
earned through gameplay.

Remaining boundary: the returned property-194 capacity byte must be consumed by
any future inventory owner that models an independent capacity cache. No such
mutable cache exists in the current portable inventory path.
