# Original character screens: connected milestone

The gameplay portrait now opens the original character movie, with its original
Stats, Skills and Inventory tabs and item-list artwork. Native menu callbacks
borrow the same live Crypt player, equipment, inventory, skill state and Save.
There is no separate menu profile. Inventory renders that player's equipment
through the original avatar camera. The world pauses while the panel is open;
the original Back action resumes that same world.

## Current live acceptance

On emulator5554, PID9645, the combined build passed the following flow:
main menu -> Start Game -> Single Player -> Crypt -> gameplay portrait ->
Stats -> Skills -> Inventory -> right-hand weapon list -> Unequip -> Equip ->
Back -> same gameplay world. Screenshots and PID-filtered logs are in
`port/android-native/reports/front-game-flow-v1/86-*` through `95-*`.
The weapon pane clears and repopulates after the actual equipment mutations.
The item-list gold label now displays0 instead of `undefined`.

The gold repair implements the original callback's string and object forms,
including writes to the actual returned object's Gold and GoldString members.
It uses the same inventory and original formatter. Missing original pickup
WAV files follow the original nonfatal not-ready path; available menu effects
use the actual bundled files. No replacement pickup sound was invented.

The final combined build, PID11213, additionally passes portrait alignment and
stage clipping. Screenshot109 and its log show the portrait paint centre moving
from692.261/699.882 to759.348/678.999twips, exactly matching the ornate aperture.
Screenshot110 shows the Stats artwork within the fitted stage; screenshot114
shows the clipped Inventory and real player preview. The same projection still
handles portrait and tab clicks; invisible overscan cannot receive a new press.

Original skill drag-and-drop changes the actual saved mapping, survives screen
reopening, and can be restored through the same UI. Log112 records saved rows
0/-1/-1 changing to-1/-1/0 and IDs7/-1/-1 changing to-1/-1/7. Log116 records
restoration to the initial mapping. A separate stale HUD icon bug was exposed:
the original query appends to its array, so refresh must run the original
onPush lifecycle that creates a fresh array. That repair now passes live:
screenshots128 and135 show the correct moved/restored gameplay button, matching
the actual saved row/ID logs. The cached-SWF sanitizer regression also passes.

This milestone does not claim positive stat allocation or skill training acceptance:
the demo has zero spendable points. The original guards and menu data render,
but a genuine earned-point progression test is still required.

## Source ownership

- Android GL/menu transport: native_character_menu_v4.inc, NativeApp and
  CharacterPanelSessionV1; same world lease and source menu stack.
- Authored movie/platform: engine-ui AuthoredCharacterPanelV2 and the retained
  MenuManager/RenderFX records; source projections also drive hit testing.
- Preview and equipment: renderer_character_inventory_preview_v4.inc and
  the live player's existing Gear draw parts.
- Gold/local-player identity: character_menu_gold_v4 and
  player_network_local_owner_v4, with original-wrapper differential tests.

## Remaining development boundaries

| Area | Current boundary |
|---|---|
| Gameplay portrait | Clicking and art-derived alignment are live-tested. |
| Skills | Original screen/descriptions/learned skill/slots render; saved assignment/reopen/restore and visible HUD refresh pass. Targeted Bash now delivers its authored hit, draws all five FX materials and changes actual enemy HP without black-screen failure. Positive training with earned points and other live skills remain required. |
| Inventory | Basic unpowered equip/unequip and preview are live. Drop needs the canonical live item pool, visual/physics/audio and pickup composition. |
| Persistence | Demo Save has a genuine null profile and takes the original no-write branch. Main-menu metadata is not campaign restoration. |
| Loot/XP | Isolated owners exist; complete death -> world loot -> pickup -> XP/level progression remains unaccepted. |
| First Swamp chapter | Loader adopted the verified LevelConfig and RoomZone safety repairs. Two containers register; real template data/positions and the next PriestGood constructor remain blockers. Full chapter gameplay is incomplete. |
| Effects | All24 inventoried player FX resources pass CPU loading; this is not equivalent to live GPU acceptance of every skill. |

The loader receives immutable, manifest-verified owner handoffs. Modern fixes
for original undefined initialization are labeled as port safety repairs,
not claimed as recovered original behavior. This checkpoint is a substantial
character-menu integration, not a finished game or completed Swamp chapter.

## Saved checkpoint

`port/android-native/build/checkpoints/dh2-native-original-character-menus-55abdfdd.apk`
has SHA25655abdfddedaf1bea9314af85ba1654e8602f7a6fcc47e662dc699f3716287b4d,
607,909,981 bytes. The single APK contains the bundled game data. Final
emulator5554 PID14432 is left on original Inventory, screenshot146, with an
alive equipped player. No native errors are present in the saved flow.

The final build adds only read-only event-local HP logging to the preceding
fully tested menu build. The exact final APK's skill receipt is
`port/android-native/reports/generic-skill-live-v9/live-receipt.json`:
actual Bash do_skill on clip1234 changes the same NPC's fixed-point HP from
25,600 to22,875, submits five authored GPU materials, and returns normally.
No HP/RNG values were forced. Source texture loading now uses a general
modern decoded-RGBA backend with the actual original sampler policy; all41
image names in38 supported FX resources passed the upload census. This is
not runtime acceptance of every skill or exact reference-video frame timing.

The checkpoint receipt distinguishes current-APK observations from the
preceding identical-menu build's equip/unequip/mapping mutation evidence.
Physical-device tests, actual campaign saves, complete loot/XP and the
first Swamp chapter remain required work.
