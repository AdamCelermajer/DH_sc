# Source LightPoint and class-menu light route

IDA source is under `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so`.

| Function | Address | Recovered fact used by the preview owner |
|---|---:|---|
| `GetNewInstance<LightPoint>()` | `0x34115c` | Allocates `0x1b4` bytes and calls `LightPoint::LightPoint`. |
| `LightPoint::LightPoint()` | `0x40bdd8` | Calls `LightBase::LightBase(this, 19)`, initializes `attachedTo` to empty, constructs the attached-object handle, and zeroes the three attached-offset floats. |
| `LightBase::LightBase()` | `0x40aba4` | Initializes its node pointer to null and constructs its source string. Fields not written by C1 stay unproduced until the source initializer reaches them. |
| `LightBase::InitPost()` | `0x40b0c0` | Converts authored color/attenuation fields, loads or creates the source CLightSceneNode, registers it with the driver, conditionally appends the LightBase, then dispatches the derived refresh. |
| `LightPoint::InitPost()` | `0x40bcdc` | Clears the two reached state bytes, calls LightBase InitPost, sets the CLight type to point, then dispatches `RefreshAttachment`. |
| `LightBase::setLightNode()` | `0x40b498` | Retains the passed scene node and copies its CLight radius, attenuation, ambient, diffuse, and specular fields into the same LightBase. |
| `LightPoint::AssignTweaker()` | `0x40b828` | For an in-range slot, writes the same LightBase into `LightSetManager::SetLight`, then stores pointer/attenuation and byte colors in the selected Application tweaker cells. |
| `LightBase::SetAttenuation()` | `0x40b3c8` | Stores constant unchanged, divides linear by 1000 and quadratic by 1,000,000 in both LightBase and the borrowed CLight. |
| `LightBase::SetAmbientColor()` | `0x40b720` | Writes the supplied RGB floats directly and alpha 1 to LightBase and the same CLight. |
| `LightBase::SetDiffuseColor()` | `0x40b674` | Same direct RGB/alpha behavior for diffuse. |
| `LightBase::SetSpecularColor()` | `0x40b5c8` | Same direct RGB/alpha behavior for specular. |
| `LightPoint::Update()` / `RefreshAttachment()` | `0x40ba0c` / `0x40bac0` | Updates the same light node and handles the authored attachment/tweaker branch. |
| `LightPoint::~LightPoint()` | `0x40bd1c` | Releases the LightPoint string, then the LightBase subobject. |
| `MenuCharacterSelect::Show()` | `0x428f38` | Gets the first `'lght'` node; if present, spawns `LightPoint` named `charselectLight` with `(true,true)`; checks source type 19; binds that same node; calls `AssignTweaker(0,0)`; appends that same point to SceneManager's automatic-light vector; then applies attenuation `(0.375,0.51172,1.0469)` and ambient/diffuse/specular `(1,1,1)`. |

`SourceLightPointNativeV1` is the next owned prefix. Its type-19 factory adopts
the same already-published `CanonicalObjectBorrowV1` and Application owner; it
does not allocate or fabricate ObjectBase. It owns the LightBase/LightPoint C1
fields recovered at `0x40aba4`/`0x40bdd8`, including the empty strings, default
attached ObjectHandle, zero offsets, null node and explicit “not produced”
state for fields C1 does not write. `set_light_node` validates a real
`NativeSceneLightNodeV113` borrow, retains that node through its actual native
`grab/drop` methods, and copies radius, attenuation and colors from that same
node's `NativeLightV113`.

`InitPost`, `Update`, `RefreshAttachment`, and the ObjectBase destructor tail
remain required services from the live Application/ObjectManager graph. The
owner rejects a missing provider when reached. In particular, this preview
does not supply LightBase's driver/SceneManager construction, actor-handle
resolution, PlayerLight tweaker publication, automatic-light append, or source
ObjectManager publication/deletion. `SourceLightPointOwnerV1` remains the
Show-order coordinator, but a complete root publication must bind it to this
same type-19 receiver and the actual source services; its older recording tests
are not integration acceptance.

The native contract test directly compiles `native_scene_lights_v113.cpp` and
checks C1 field state, type-19 identity, and `setLightNode`/InitPost/destructor
ordering against the actual `NativeSceneLightNodeV113` and `NativeLightV113`
owners. Its Application lifecycle services are test receivers only. It does
not claim a live SceneManager, ObjectManager, LightPoint factory allocation,
tweaker, automatic-light list, or renderer material uniform provider. The
existing preview still exposes decoded `Omni01` input metadata until the
application publishes that complete same-owner graph.

Verification 2026-10-09: feature CMake target `source_light_point_v1_tests`
built with LLVM MinGW and passed 1/1 CTest test. The standalone native contract
script `tests/run_source_light_point_native_v1_contract.ps1` passed against
the actual native light owner source.
