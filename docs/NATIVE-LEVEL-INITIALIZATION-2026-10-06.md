# Native level initialization checkpoint, 2026-10-06

Accepted and installed on visible emulator5554:
`port/android-native/build/checkpoints/dh2-native-level-initialization-b3708a7e.apk`.
SHA256 `b3708a7ee51486db36d687aed577729f8ff10f2e1051ee00d05c5560e31df6fa`.
ARM64 and x86_64, one APK including the original cache. No ARM32 runtime library.

The checkpoint compiles the complete recovered Level C1, generic private Lua
script/shared file-cache composition, staged native Save owner, source Event
manager, same actor position hooks, Item gathering and real container data
providers coherently. It retains a native Application service identity with the
distinct heap EventManager14 published by the source PostInit prefix and one
process CameraBase active owner across GL surface recreation. The actual
character settings wrapper now borrows this same Application identity.

Level C1 remains a native kernel ready for the loader's actual service binding;
the live Crypt development launch still does not execute the whole canonical
Level Init or publish a completed Swamp map. Application PostInit/Shutdown as a
whole remain unimplemented. Compiled native source is not full runtime readiness.

Source verification: whole Level C1 3,180 original cases/zero mismatches, 48
native failure guards, 53 same-Level/current-GSLevel/lifetime checks, 459 checks
using the actual 51-row design and original Swamp combat/death script contents
with private states/shared cache and native save construction. Constructor
handoff30 entries was sent to the loader. Character SetPosition21-entry handoff
was accepted there, allowing its original Priest constructor to finish.

New visible correction: inventory avatar initially disappeared when portrait,
Skills and Inventory opened before the first gameplay scene update. Gear draw
placements lacked a root transform that the preview's inverse already removed.
The pane now publishes the same scene's current matrices before extracting
Gear draw parts, without advancing AI, gameplay or animation time. It renders
immediately and after genuine Unequip/Equip. Its initial pose is the current rest
pose until gameplay sampling; dedicated menu-idle timing/fidelity remains open.

Live normal-input regression on this exact APK: main menu -> SinglePlayer ->
Crypt -> portrait -> Stats/Skills/Inventory, immediate visible avatar, weapon
removal/restoration, real attack and Bash on NPC100000006, HP23723->19117,
skill state6 closure, particles/mesh render and potion stock5->4. No E/DH2Native
errors in the captured PID25265 sequence. Input did not edit HP, RNG or position.

The gameplay camera is still the development camera. Original camera target,
resource/timeline, active-scene membership, matrix/projection, Zoom and teardown
source work is progressing separately; its composed native acceptance is not a
live view/input binding. Do not call this checkpoint a player-framing fix.
V19 camera handoff187 source entries is now independently archive/workspace
verified, SHA256 `671ded51e696a0ec7dc6893218b4e907d0f9331f860daf2e6e8df3982e29e544`.
Its same-cache lifecycle/failure cleanup passed; the next root feature is the
atomic live camera/view/input connection. Required positive skybox/offset/ray
branches remain explicit in its integration contract.

Loader received genuine Dummy41-declaration/616-check and complementary
Decor/SpawnPoint21-declaration/208-check packages. Its unfiltered Swamp graph has
advanced through real chests/Priest/Dummy and is currently reaching the actual
DestructibleContainer constructor. Approved later PropertyMap successors are
separate from this immutable checkpoint.

Parallel lanes at this checkpoint: original camera lifecycle and live view/input
connection; whole actor OBJS Save/Load serialization; remaining genuine Swamp
class-family construction, including DestructibleContainer. The complete source
Save framing/backup/job/OBJS orchestration package was independently verified,
but real Android storage, canonical manager dispatch and actor virtual bodies
must be connected before claiming game persistence.

Receipt: `port/android-native/reports/native-level-initialization-v5/checkpoint.json`.
Current compiled-source plus header/include/CMake workspace snapshot1662 entries:
`native-source-snapshot-b3708a7e.zip`, SHA256
`800063eba22cb7c0cfeb8dd0c68c7119e0c01ad2e50639c405594949a939e77a`.
Its scope is explicit and includes supplementary uncompiled headers; it is not
a standalone finished game source archive. Older APK/source archives remain.

Remaining milestone: complete and connect the first Swamp chapter, source
camera/input, full actor/quest/loot/XP/save/audio behavior and actual-device tests.
