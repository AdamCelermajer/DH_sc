# Original targeting and skill reference

Reference supplied by the user:
https://www.youtube.com/watch?v=z_Zky7qQdYs&t=282s
Title: Dungeon Hunter 2 (v1.0.3) Part 1 [720p], CrisR82.
Our original cache is v1.0.2. The footage supplies visual behavior evidence;
actual skill IDs, authored events and source execution must be checked against
our own tables and scripts rather than inferred from a similar icon.

Inspected the original browser video paused at 720p60, using native player frame
stepping. Verified media timestamps and waited for the corresponding decoded
image before interpreting it. No replacement artwork or video reconstruction.

| Video time | Visible behavior |
|---|---|
| 4:41.995 | Warrior winding up toward a nearby Bogwomp; enemy name/level/HP are visible. |
| 4:42.495 | Selected nearby Bogwomp has a red ring under its feet; sword points toward it. |
| 4:43.278 | Short yellow/green flash on the selected enemy; a second enemy approaches. |
| 4:43.995 | Selected enemy HP has fallen to roughly half; ring remains on that enemy. |
| 4:45.495 | First enemy lies dead to the left; selected ring and live target display correspond to the next living Bogwomp to the north. Small yellow flashes also strike another enemy near the fairy. |
| 4:46.328 | Large orange/white fiery streak runs from the player's weapon through the selected enemy and extends beyond it. This is distinct from the smaller fairy flashes. Skill/fairy buttons are greyed during the action. |
| 4:46.995 | Large streak has ended; white damage21 appears over the selected enemy, whose HP is nearly depleted. Selection ring remains; mana is lower. |

The exact frame in a gameplay recording does not prove its underlying native
callback, skill ID, hit timing or input. These observations must be matched to
the original authored animation events and scripts.

Additional decoded opening-chapter samples:8:30.541 shows the original Torso
inventory (Auto-equip/Unequip/Equip, real character preview and item list);
12:00.198 shows a loading transition;13:01.015 shows the Warrior's level3 Stats
sheet;15:01.048 shows outdoor shallow-water travel with a companion following.
The requested4:42–4:47 combat sample is indoors, so it must not be mislabeled as
proof of Swamp module geometry. These are sampled states, not a complete quest
or route reconstruction from the entire22-minute recording.

## Assigned implementation scope

Combat-animation agent owns the shared acquisition, actual facing, skill target
retention, per-frame target validation/death/out-of-sight, effect aim and authored
animation event comparison. Death/loot agent retains sole CtrlKill reward and
OnDied order. Module agent continues level loading.

Concrete source defects already found:

- Renderer Search objects never received actual EulerZ+174, leaving facing at
  constructor zero. Both normal attack and skill acquisition consume it.
- Renderer Search objects omitted signed cached Character+1310/+1314, actually
  the same resolved Special_Sneak[198]/Sneak_Detection[199] fields.
- Renderer did not call the recovered whole CharAI::_UpdateTarget3cb908 frame
  path. It is the genuine TargetDied/OutOfSight producer; adding an independent
  notification in Kill would change source ordering.

First two field projections have source-backed code corrections and native
normal/skill Search regression coverage (29 checks), with both-ABI syntax checks.
Whole target frame/event composition and normal melee pursuit are connected.
Live visual acceptance remains pending. No new APK checkpoint is accepted yet.

Bashdown/Headsplitter Pre uses FrontalFirst/Enemy/AttackableOnly RANGE160 and
caches its selected Lua target before LookAt/mana. Charge uses RANGE300, then
ANGLE120/NoSort on Use from the actual facing. Bashdown's apparent scripted
PlayFX call is commented out; its visible effect must be matched through actual
animation metadata. A FIRE-labeled hit effect alone is not that proof.

## Shared authored-effect omission

The actual Knight BashDown row7 uses animation347/clip1234 with AnchorFX1 and
effect set164. The playback adapter previously ignored the authored FX and
AnchorFX fields, so the damage presentation could not reproduce this effect.
The cached resource contains meshes and two particle emitters. The debris
emitter binds Deflector then Gravity; the dust emitter binds no forces.

The all-skill table audit found86 rows with animations,41 with authored step
effects,45 unique referenced effect files and28 files combining mesh and
particles. Every referenced file was found in the original cache, and bounded
geometry inspection completed without parsing errors. This is asset evidence,
not runtime support for all skills or script-triggered effects. See
`port/level-world/reference/shared-target-facing-v1/all-skill-authored-fx-inventory-v4.json`.

## Live failure reproduction and fixes

The development c438 build reached source target OutRange event14 and failed
Cmd_MoveTo(Point). Its FindPath caller supplied a zero-capacity output buffer.
The completed binding now retains graph-sized output storage. Native execution
on the actual Crypt335-node graph found the real route from the captured
player/enemy coordinates. Evidence is in
`port/level-world/reports/android-native-owner-tests/player-target-route-storage-v2/receipt.json`.

The subsequent c828066a build successfully published the same12 actors into
one canonical world manager and launched Crypt through the original main menu.
A nearby skeleton attack exposed a second black-frame prerequisite: scrolling
combat text still expected the old NPC inventory projection although NPCs now
own their genuine inventory. The consumer now queries the same selected
main/off-hand item records and cached properties through the original
equipment-query kernel. Its actual1322-row cache/native regression passes;
the packaged023f238e correction also survives actual NPC attacks and eight
normal player hits in the visible emulator. Enemy HP falls25600 to0/dead1 and
the selected target/health display clears. This normal attack path still uses
the legacy direct damage/death presentation; source CtrlKill rewards are not
accepted by this observation.

The same live run then executes BashDown with nativeTargets1 against a living
enemy: animation347/clip1234 enters state6 and returns state3, including source
root motion toward that enemy, without an E/DH2Native failure. This proves
acquisition and animation recovery only. The activation counter does not prove
the authored hit callback, damage, effect geometry or exact hit timing. The
player subsequently dies from that enemy's normal attacks, with the genuine
dead state12 rejecting another skill request. Evidence is retained in
`port/android-native/reports/front-game-flow-v1/49-combat-text-fixed-main.json`
through `55-live-enemy-close-skill.json` with one APK/PID.

These separate failures were encountered before the authored skill mesh and
particle renderer is bound. Passing isolated tests does not establish
skill-on-enemy visual parity, a complete lethal pipeline, or Swamp readiness.

## Acceptance

Verify acquisition and facing from different starting angles, normal attack and
at least two real authored skills, targeted/non-targeted casts, visible authored
effect origin and direction, exact hit/event timing, animation recovery, and
clear/retarget when the current enemy dies or leaves sight. Full lethal gameplay
acceptance also requires the death/loot lane. Retain a coherent menu-to-game
flow and visible emulator; checkpoint only after the connected system passes.
