# Native targeting and attack animation milestone

Checkpoint: `port/android-native/build/checkpoints/dh2-native-targeted-combat-d33c029f.apk`

SHA256: `d33c029f7bceee424b306c4b5be5bcca48821f9fafcc16fcdffa438520fc5ac0`

The visible root emulator5554 runs this same APK. The checkpoint includes 18 ELF64 libraries across ARM64/x86_64, 771 assets and the canonical bundled cache containing 6,833 files. Its compiler-input snapshot contains 1,317 source inputs. No external cache installation or ARM32 compatibility runner is required.

## Delivered source integration

Attack input now executes the reconstructed original player attack owner over the registered World, retained target handle, same Gear, Character state machine and real actor properties. Null input searches enemies through the source acquisition kernel; melee reach uses original AI radius and the selected weapon parameter. The renderer no longer uses its former nearest-index attack shortcut.

The original CharAI attack-step begin/end methods run from the actual blended animator. Their callbacks borrow the current animator phase and source target, deliver original AI/FSM events, and preserve combo continuation, source pre-attack checks and step mutations. A wrong offset mapping was fixed: owner+528 is the attack gate, while owner+520 holds Character flags. Confusing these had blocked combo continuation.

Actual Character registration now writes the targetability constructor byte from nested CharAI+4d at Character+415. Player sneaking and world targeting borrow that same byte. This restores a missing source producer rather than changing NPC flags to manufacture targets.

The original enemy HUD now has a live positive target: Skull Slave, level8, and a health bar derived from that monster's actual HP. The bottom HUD shows Headsplitter level1, two empty skill slots, the locked faery and the actual potion stock. Character opens Stats, Equipment, Skills and Faeries. These panels are functional native views; the complete original animated SWF navigation remains unfinished.

## Verification

- Both Android ABIs and Java built successfully. The installed APK hash matches the checkpoint.
- Original ARM attack-step differential: 1,376 cases, 3,507 ordered callbacks, three guard/failure-prefix checks; current main host library linked under ASAN/UBSAN passed. Dependency callbacks are explicit fixtures, so this is not proof of the complete hit pipeline.
- Real Attack button, no actor-position or target injection: source acquisition selected the actual registered skeleton and played its attack step cycle. Existing native melee application reduced HP25600→21247.
- In the same live session, Equipment unequip/re-equip changed armor4→2→4 and rebuilt the world. Another Attack touch acquired the same monster and reduced HP21247→16725. A later visible-player check reduced HP16725→12759.
- The retained-session panel check passed skill-slot roundtrip, disabled zero-point training, original locked-faery names, refusal to waste a full-health potion and real healing after the existing enemy attack development command. Potion stock changed5→4.
- The development enemy damage command focuses its attacker; the final visible check restored the follow-player camera without moving actors. The player, skill/faery HUD and enemy HP bar are visible together.

Receipts: `port/android-native/reports/native-targeted-combat-d33c029f-checkpoint-validation.json`, `source-targeted-combat-panel-v1/`, `source-attack-after-equipment-v1/`, and `source-targeted-combat-visible-v1/`. The immutable checkpoint source ZIP includes the panel/attack receipts and original host evidence.

## Incomplete and failed checks

The precision waypoint travel harness did not pass: small joystick corrections oscillated and introduced orthogonal travel near the target. Its current-APK FAIL receipt is preserved in the checkpoint. Successful combat at the touch-reached position does not establish reliable precision movement or full pursuit/pathing.

Melee HP application still uses the existing native combat adapter; the complete original authored OnAttack/Hit/presentation tail is the next replacement. Positive targeted Headsplitter damage and visible injury have not been verified. Enemy AI/physics, skill FX/audio, unlocked faery casting, complete item menus, campaign/save progression and physical ARM64 devices remain incomplete. The top authored potion display still shows its static99 value; the bottom live control has the actual stock.

Three parallel tasks continue: original authored melee hit events and application; same-world NPC skill injury reactions; and full NPC initialization with visual/physical/PF services. Their unintegrated modules are not part of this APK's verified scope.
