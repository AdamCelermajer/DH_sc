# Native scene-node animation playback

Current source work adds a separate dynamic compiled set and registration adapter.
Its first-seen union, clip/default binding and raw interpreter rules now support
full position/quaternion/scale plus position-axis2..4 and authored scalar-angle9.
[Source evidence](reference/component-transforms/NOTES.md) distinguishes raw track
factories from union contribution handlers; type9 produces a quaternion from an
angle and authored axis, while scale-axis types are11..13. The extended sampler
passes9,376 original/optimized-ARM64 checks and sanitized host replay; static
11,656 and earlier dynamic117,030 sample regressions still pass. The standalone
[component applicator](reference/component-applicator/NOTES.md) also verifies
full-vector position/scale setter effects and their dirty flags. Static Player
and static compiled sets retain their earlier restricted behavior below.

These source libraries build for ARM64 and x86_64. Full-bank live rendering,
source script/equipment connections and physical-device validation remain
separate milestones; the preserved timing APK predates these changes.

## Authored animation events

`events.cpp` reconstructs the event manager's searches, named-time lookup,
callback lag/order, interval updates and one-wrap updates. The [captured original
routines](reference/events/original-functions.json) and [binding evidence](reference/events/binding.asm)
locate `SEventsTrack` at BRES root offset `0x2c`. Its six 32-bit words are
time type, component count, key count, key pointer, group count, group pointer.
Each eight-byte group contains a count and a pointer to an array of event-name
pointers. Type 1 uses byte frames, type 3 uses unsigned-short frames (30 fps),
and type 4 uses signed millisecond keys. The reader requires one component,
matching key/group counts, ordered times, bounded names and immutable offsets.

Frame arithmetic preserves the original float constant `0x42055555`, including
its separate rounded operations. A triggered event contains lag in milliseconds
and its name. Lookup retains the **last** matching name. The four-time update
includes the previous millisecond and suppresses a repeated boundary event using
the saved last entry; the two-time interval update excludes the previous time
and keeps no cursor. Equal timestamps and disabled callbacks leave the four-time
cursor unchanged. Unknown formats are rejected by the port's validation.

[30,775 original ARM32 versus compiled ARM64 comparisons](reports/events-arm64-differential.json)
verify searches, named lookup, 11,000 interval updates and 11,000 wrap-capable
updates, comparing 109,462 callback observations and saved cursor values.
They cover the five authored monster event tracks plus 105 synthetic tracks,
duplicates, empty groups, integer boundaries, null callbacks and repeated updates.
Imported libc/soft-float and the caller observation callback are modeled; engine
dispatch instructions execute unchanged. Original object ownership, callback
mutation/reentrancy and complete animator timelines are outside this comparison.

`EventTrack` owns a copy of the image and is move-only. `Player` loads its event
track alongside transform tracks. The sanitizer audit checks all 38 bundled actor
images, five event tracks, move ownership and 5,000 malformed images. The runtime
dispatches events independently for each monster and resets its event cursor
when its scheduler enters a clip. This clock adapter still uses the port's frame
policy; damage, event-to-state-machine mapping, blending, FX and audio remain
pending. Frozen pose inspection does not execute events.

This module connects the already reconstructed animation accessor/search code to the checked scene graph. It owns a copy of the BRES key image, retains safe borrowed views into that image, samples keys and updates node/world transforms. Ownership is move-only to prevent shallow copies of key pointers. Failed loads clear all tracks; failed sampling leaves the scene unchanged.

The first supported property is float-3 scale (channel type 10). Original `CColladaDatabase::getAnimationTrackEx` selects `CSceneNodeScaleMixin<float>` for this uncompressed variant; its applicator calls the node's scale setter at vtable slot `0x94`. Captured routines and hashes are in [original-functions.json](original-functions.json) and [assembly evidence](reference/original-functions.asm).

`dh2_animation_lerp3` preserves the original scale interpreter's arithmetic at `0x628850`: `(1-fraction)*left + 0`, then `fraction*right` added to that result, with float rounding at each operation. It calls the existing original-behavior key search; exact keys, duplicate times, step interpolation and before/after-range selection retain that module's semantics. Absolute key playback ignores the optional default/rest record, as the original interpreter does. Compressed offset/scale variants are rejected.

Supported properties now include float-3 position (type 1), float-4 quaternion (type 5), and float-3 scale (type 10). Position uses the original interpreter at `0x6286cc`. Quaternion playback reproduces the two-key blender at `0x613294/0x6130d4`, including zero-weight endpoints, float-rounded weight sums, and the already reconstructed quaternion slerp. Sampling restores the complete rest TRS before applying keys.

Legacy Player limits: up to 256 contiguous segments, one channel and one sampler per supported track; finite output keys; recognized scalar time formats; interpolation enums 0 or 1. Compressed offset/scale tracks remain rejected. Other properties are counted as skipped. Its scene-sampling API does not apply component tracks or blend slots; the separate compiled sampler and two-slot coordinator handle their verified domains. The inspector's clock loops over the clip range; this is a preview policy, not reconstructed game scheduling.

## Verified milestone

- The original candle has two scale tracks, targeting `_bone_-node` and `_bones_flame_common-node`, with a segment from -166 to 1,866 ms.
- The host audit samples every millisecond of that range (2,033 samples), checks changed and finite world transforms, an exact scale key at 100 ms, move ownership and invalid-image rejection. It additionally probes 5,000 mutated images under ASan/UBSan.
- [284 ARM64 interpolation cases / 852 components](reports/lerp-differential.json) match original ARM32 instructions byte for byte. The original getter executes against a minimal relocated caller fixture; only imported memcpy and soft-float helpers are modeled. No getter or interpolation algorithm is mocked.
- Android emulator screenshots show live movement, different fixed poses at 100 and 500 ms, and a stable repeated frozen pose. Reports live in [the Android project](../android-native/reports/animation-smoke.json).

```sh
cmake -S port/engine-animation -B port/engine-animation/build/host -G Ninja
cmake --build port/engine-animation/build/host
port/engine-animation/build/host/animation_audit path/to/candle_flame.bdae
```

The original engine binary and Unicorn are validation tools only; neither is packaged in the APK. This module now feeds [native character skinning](../engine-skinning/README.md), but the project is still a rendering prototype rather than a playable game.

## Next character integration

[The locally verified character inventory](reports/character-input-audit.json) identifies 173 mesh geometries and 173 skin controllers in `prince_modular.bdae`. The separate `prince_menu_idle_knight.bdae` clip now plays all 29 tracks: 25 quaternion, three position and one scale, across two segments. [Original-instruction comparisons](../engine-skinning/reports/arm64-differential.json) verify position and quaternion arithmetic. `goblin_idle_01.bdae` has 23 tracks but is not yet integrated. These original files stay ignored outside Git.

Actor integration adds an explicit `MissingTargets::ignore` policy. Strict rejection remains the default. The optimized skeleton binds 23 transform tracks and counts three absent targets separately from unsupported track types. Unbound keys still pass the same layout, time, quaternion and runtime-state validation before being ignored. [Original animator evidence](reference/unbound-targets/original-functions.json) and [assembly](reference/unbound-targets/original-functions.asm) show `applyAnimationValues` skipping null bindings at `0x65d9c4`. [Object tests](../level-world/reports/objects-host-audit.json) verify strict rejection, valid playback and rejection of a malformed unbound key.
