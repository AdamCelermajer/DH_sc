# Native actor and Item checkpoint

Accepted APK: `port/android-native/build/checkpoints/dh2-native-actor-item-foundation-11c8219e.apk`.
SHA256: `11c8219e13e55e24dc3668c45821fb29b03627d6e5810b8a10e340bd86764908`.
Size: 619,217,869 bytes. Original assets remain bundled inside this one APK.
Both ARM64 and x86_64 builds passed. Root emulator5554 is left in Crypt gameplay
with the player and edge-aligned original HUD visible.

## Included source systems

The generic NPC visual/animation capsule loads each family's original model
and animation dictionary onto its same attached Scene and SceneBinding. It
does not copy a second scene or substitute the Crypt animation bank. Same-scene
native fixtures cover the actual Priest, Skeleton, Slime and Ghost, including
the Priest's type2 physical body. Marker-aware mesh bounds now preserve the
original already-scaled marker branch. All affected visual consumers were
rebuilt together after the shared layout changes.

The native world Item lifecycle, movement/frame ordering, pickup transfer,
loot creation, death-loot transport, gathering-quest tail and online-update
owners now link in the production libraries with their existing dependencies.
They share the existing manager, inventory, scene roots and physical world.
Their actual app services are required when reached; live Drop, pickup and
complete lethal Kill are still not bound. The isolated145-object cache/scene/
physics/PF graph and real-inventory transfer tests provide narrower proof.

The source PropertyMap now handles shipped `_templateName` attributes through
the existing template descriptor clone, preserving authored chest data and
positions. This is an explicit modern native correction. The loader's actual
assertion-policy and full initialization remain separate requirements.

The complete outer GSLevel constructor/destructor now has a typed owner over
the same Level/current-global slots. It preserves loading-menu, publication,
callback reread and cleanup ordering. It passed192 original ARM/native trace
comparisons and22 native failure/reentry checks. Full underlying Level
construction and unloading are still required, so this source system is not
yet an active gameplay Level in the app.

## Live regression evidence

The accepted APK passes the original main menu → Single Player → Crypt flow,
edge portrait → original character menu, normal melee and particle/mesh effects.
The actual Bash/Headsplitter input selected one enemy and executed clip1234's
do_skill event, reducing `_prim_tmp_cultist07` resolved HP from23723 to19117.
Its sequence closed back to idle; no native error or black screen occurred.
The actual potion input restored health and mana. Source property values, HP,
RNG and actor positions were not edited to produce these outcomes.

Receipt: `port/android-native/reports/native-family-integration-v6/checkpoint.json`.
Detailed live evidence: `targeted-skill.log`, `targeted-skill-world.png` and
`targeted-skill-paused.png` in that same directory. Final parked-world evidence:
`port/android-native/reports/front-game-flow-v1/180-native-systems-visible-world.*`.

The frozen68-entry changed-system source/evidence patch is
`port/android-native/reports/native-family-integration-v6/changed-engine-source-11c8219e.zip`,
SHA256 `7a51c2c065240e3590a7e0a00140d74721457b6f1aafb52a61fccce13cac9302`.
It preserves this checkpoint's changed source before the next shared-header
work. It is not a standalone complete game source archive.

## Next integration work

Root owns full Level construction, current-level lifetime and renderer wiring.
The loader has a retained9-module/386-submission Swamp preparation API and has
reached the real priest's construction/properties without filtering actors.
Its next required position service is being connected by the NPC agent; the
Item agent is finishing the original chest dictionary/row callbacks and real
gathering registrations plus bounded nested pickup delivery.

The camera agent has recovered authored scene selection, follow/transition/
zoom bodies, real camera/design tables and source matrix math. Camera AnimSet/
controller ownership and live renderer binding remain. This APK still uses
the development camera; it does not claim the video's exact player framing.

First Swamp chapter gameplay, campaign scripts/quests, complete loot/XP/death,
save restoration, remaining skills/faery/audio and physical-device testing
are still unfinished.
