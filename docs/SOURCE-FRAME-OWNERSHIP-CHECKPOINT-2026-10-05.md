# Source frame ownership checkpoint — 2026-10-05

Branch: `reconstruction/android17-irrlicht-rebuild-2026-10-03`.

This milestone adds source prerequisites and fixes retained Lua ownership.
**The full game and autonomous native enemy AI remain unfinished.** The
[project checklist](PROJECT-CHECKLIST.md) tracks verified work and all remaining
completion requirements. Adam's contributions and measured reconstruction reach
are in the [combined status](COMBINED-RECONSTRUCTION-STATUS.md).

## What changed

| Component | Verified work | Remaining integration |
|---|---|---|
| Created Session callback bridge | Install prepared, retained callbacks only on the existing unbound VM; preserve construction/publication order and actual VM identity. Independent review caught and fixed destructor reentry; reset, install and destruction retain busy guards. | Adopt it during native pending/active controller publication without replaying initialization or replacing the VM. |
| `GameObject::Stop` | Reconstructed the bounded caller and virtual physics-policy selection. 246 original ARM cases pass; ordered path/body effects are retained on later failures. | Bind real native path ownership and separate physical velocity, transform and sleep operations. |
| Character physics-position policy | Reuse the source flag decoder, read live flags and retain full-width identity. 67,594 Character and 1,024 base-object comparisons pass. | Invoke the policy through real native Stop ownership. Ordinary Idle/Move flags do not force physical reset. |
| `Character::Update` scheduler slice | Reconstructed lazy loading, signed timestamp ordering, eviction caps and replacement semantics. 18 ARM cases cover all 127 reached instructions; 15 guards and three partial-effect cases pass. | Complete surrounding gates and provide the shared native map, stable actors and real unload/controller providers. This is not the whole Update body. |

The bridge is a native port adapter with **zero new original function-body
credit**. Original provider bodies and ownership boundaries are explicitly
recorded in each component's notes. The scheduler map projection is a bounded
port-owned sorted array, not the original red-black tree or allocator layout.

Source references:

- [Created-VM ownership notes](../port/level-world/reference/monster-created-service-install/NOTES.md).
- [Stop evidence](../port/level-world/reference/game-object-stop/NOTES.md).
- [Physics-position evidence](../port/level-world/reference/character-physics-position/NOTES.md).
- [Scheduler evidence](../port/level-world/reference/character-update-script-scheduler/NOTES.md).

## Build and runtime verification

Both ARM64 and x86_64 Android builds pass. The current local APK is frozen
against **466 actual repository compiler/build inputs**, 255 assets and 16 ELF64
libraries. The source export gate verifies 60 bounded units and 96 required
export groups for each ABI. ELF/ZIP 16 KiB alignment and signing pass.

Artifact identity:

- Local APK: `port/android-native/app/build/source-frame-ownership-final-2026-10-05/crypt-source-frame-ownership.apk`.
  27,652,316 bytes; SHA-256 `99dc04b89980fa426ed39d5af981be477fea29c01e1ab3f647e40fd47c544a2a`.
- Exact raw compiler-input ZIP in the same local directory:
  `crypt-source-frame-ownership-compiled-source.zip`.
  900,854 bytes; SHA-256 `aec0bd30cc0e76cbbddde2b1b1ac813fdddae2b6828afcd13ab06ba3cc4add34`.
  External tools and the documented asset/dependency setup remain necessary.

The Android 17/API 37 emulator reports **16,384-byte pages**. The actual new APK
passes original Crypt touch contact, Wait/Spawn, ordered unchanged Ghost
OnInit/HP/MP/skills/Post/Final calls, damage retention, all 14 native Character
nodes, reload and rotation. VM, health, vectors, catalogue backing and timers
remain owned; initialization does not replay. Original overrides are restored
after testing. Root inspected the textured Prince/Ghost screenshot.

Focused bridge and scheduler tests pass ASan/UBSan/LSan. Independent bridge and
Ghost Owner review passes; six existing Session-dependent regression gates
pass. The [frozen evidence directory](../reports/reconstruction-2026-10-05/source-frame-ownership/)
records 1,314 component/compiler source-hash checks, build identities, component
reports, screenshots and runtime logs.

## Published download and limits

The [published native frame foundations release](https://github.com/Noamcelermajer/DH_sc/releases/tag/native-frame-foundations-2026-10-04)
retains its previous APK and exact source identity. The new local APK above is
a later development build; these identities must not be interchanged.

New Stop/policy/scheduler code is compiled, but not yet invoked by an autonomous
native Character frame. Scene/culling, room enrollment/InZone, state ownership,
the shared update queue, target/path/body providers and AI/DoT timer expiries
remain integration dependencies. Nonempty skills, full combat, campaign,
process-death saves and physical ARM64 gameplay remain open. The two Ghosts'
authored initialization still has zero ordinary skills and five null-script
faeries. No complete-level or full-game claim follows from this regression.

## Next work order

1. Complete and review culling, room/zoning/visibility and native Stop providers.
2. Adopt the prepared controller on the retained native VM, then wire source
   eligibility, state callbacks, acquisition and pursuit in original frame order.
3. Bind the shared scheduling and timer owners; preserve reload/teardown lifetimes.
4. Complete nonempty skills and integrated combat, then finish one original level.
5. Follow the remaining campaign, modding and release gates in the checklist.
