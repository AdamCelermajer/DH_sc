# Native FX alignment and enemy target-ring checkpoint

Verified APK: `port/android-native/build/checkpoints/dh2-native-fx-target-v28-2334e8a8.apk`.
SHA256: `2334e8a84f1d5bd1cc0c9ea108175533711502b5575b468b59537697989a6b46`.
621,444,317 bytes; ARM64 and x86_64. Original cache is embedded inside this one APK.
The exact artifact receipt is `port/android-native/reports/fx-target-v28/checkpoint.json`.

## Result

The original animated red target-circle artwork now follows the selected enemy
through the existing 3D effects manager, source material/shader/texture owners,
and transparent draw queue. The nine-slot source Character indicator receiver
retains four real declared resources and genuine NULL slots. Changing targets
within one interaction family does not restart its authored animation. Clearing
the actual target hides the indicator. Item/chest virtual dispatch is a later
connection; this checkpoint verifies enemy targeting.

General effect placement now follows the original VisualObject Euler mapping
`(Y,-X,-Z)`, the actual character visual-root orientation when present, and the
source scale reset when reusing pooled effects. This applies to the shared FX
path rather than one chosen skill. Authored node quaternion tracks are unchanged.

Live testing found two missing connections after CPU tests: the anchored
PFObject normal path and the original target material's `ALPHABLEND` define.
Both were connected before acceptance. Earlier failing candidates and recordings
are retained under the report's `failed-candidate-*` directories.

## Verification

- Full APK builds for ARM64 and x86_64 passed.
- Original arithmetic comparisons: 192 outer matrices and 192 anchored rotations.
- Actual-asset native target-marker test: 1,271 checks including retargeting,
  motion applied once, authored geometry, hiding, pool reuse and destruction.
- Anchored floor branch: eight sanitizer checks with declared query fixtures;
  the live renderer uses its actual PFWorld selector and actor PF normal.
- Normal main-menu -> character-selection -> Crypt flow passed on visible5554.
- Normal movement and attack displayed the original circle below the enemy and
  the sword trail. A targeted skill was accepted with one real target and
  delivered enemy HP `11266 -> 5834`; no native frame error was recorded.
- Original frames from the local recording show the skill effect at the combat
  actors. Screenshots and recording are in `reports/fx-target-v28`.

This is one live Crypt scenario plus source arithmetic coverage. It is not an
all-skill, all-camera or physical-device acceptance claim. Whole death/reward/XP,
player initialization, campaign saves and the full Swamp chapter remain unfinished.

## Parallel work continuing

Three subagents own player initialization/XP, kill/drop/pickup, and original
character/skills/items menu connection. The loader chat continues Swamp constructor
and full Level lifecycle work. A separate project chat,
`01a11070-399a-7e62-b099-8e6df81dfc45`, owns remaining authored FX resource families
in new versioned files. Root owns integration, builds and visible emulator tests.

The shared workspace continues changing after this exact APK was frozen. Immutable
component packets and their receipts distinguish later code from included code.
