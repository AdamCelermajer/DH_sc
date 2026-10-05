# Original loading order and named-node transform evidence

This supplements the procedural-module milestone. The visible Android APK and
renderer were not changed in this follow-up. The loader remains incomplete:
the gameplay factory contract, objects, full scenery, transitions and saved
state restoration still require integration.

## Corrected initialization boundary

The earlier declaration/interface notes incorrectly called ObjectManager's
virtual slot `+0x20` `InitPre`. The original vtables resolve it to
`IsGameObject()`. Slot `+0x1c` is `InitPost`.

`ObjectManager::LoadFromXML` applies the separate `template` field, loads
defaults, and applies registered XML overrides. Its comparison at `0x34ba4c`
uses the literal `LevelConfig`. Only that type receives immediate `InitPost`
at `0x34bb98`. Loading then checks `IsGameObject()` and adds the module
translation to game objects at `0x34ba8c..0x34bad8`. A false type check skips
that position update; it is not a failed pre-initialization result. The ordinary
object initialization phase remains separate and requires further lifecycle
recovery. The raw declaration index is still an authored projection.

The current proposal, README, declaration comments and handoff have been
corrected. Historical receipts retain their original bytes and old scope
wording; their hashes do not attest to these later comment/documentation edits.
No gameplay behavior or factory ABI was added by this correction.

## Named-node transform sequence

The original VisualObject constructor passes a null selector when its xref
string is empty. AssetManager forwards a non-null selector as the final
`reset_from_file` argument to SceneManager. The original SceneManager appends
`-node` to a nonempty selector and invokes Collada `constructNode`.

The captured Collada constructor obtains a wrapper through the factory,
constructs the selected node beneath it and attaches that node. SceneManager
then calls `RootSceneNode::ResetPositionFromFile` when the reset flag is true.
That helper clears the first child's position and quaternion, sets its scale
to one, clears wrapper position and updates the first child. It does not reset
sibling or descendant transforms. VisualObject synchronization applies the
game object's position, quaternion and scale to the wrapper.

RootSceneNode has a virtual base: its constructor installs the address point
at vtable symbol `+28`, rather than the ordinary `+8`. At that address point,
the transform slots resolve to the inherited ISceneNode getters/setters.

For this checked sequence, replacing the selected source root with the
projected module transform in `fixed_map_v1` is equivalent to the wrapper plus
identity child. Multiplying the selected root's old file scale back into the
placement would contradict the observed reset. This is evidence for the
collapsed transform sequence, not proof of full Collada construction, static
optimization, material/lighting parity, animation or complete scene rendering.

## Verification and reproduction

`reports/original-loader-vtables.json` resolves five relevant vtables and the
LevelConfig literal against the original ELF. `reports/visual-root-policy-host.json`
and `reports/visual-root-policy-sanitizers.json` each compare 135 placements,
three constructor-prefix selector cases and two reset guard cases. Native
position/scale bytes match exactly and quaternion maximum error is zero.
Randomized source-root transforms are reset; sibling/descendant records stay
unchanged. Both actual ISceneNode setters and the original ARM normalization,
quaternion and synchronization code execute. Absolute-position updates and
mesh-box work are explicit service boundaries; imported arithmetic/libm use
host arithmetic/Python libm. Complete initialization is not executed.

The root-policy test uses the existing built transform probe. From PowerShell:

```powershell
$loaderRoot = 'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc'
$loaderPython = 'C:\Users\adamc\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe'
$loaderDeps = 'C:\Users\adamc\.codex\worktrees\generic-level-loader\dependencies'
$loaderEngine = 'C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so'
Set-Location $loaderRoot
& $loaderPython port/level-loader/tools/inspect_original_loader_vtables.py --engine $loaderEngine --dependency-root $loaderDeps --out port/level-loader/reports/original-loader-vtables.json
& $loaderPython port/level-loader/tests/visual_root_policy_original.py --engine $loaderEngine --dependency-root $loaderDeps --probe ../build/host-xml/dh2_loader_visual_transform_probe --out port/level-loader/reports/visual-root-policy-host.json
& $loaderPython port/level-loader/tests/visual_root_policy_original.py --engine $loaderEngine --dependency-root $loaderDeps --probe ../build/host-sanitizers/dh2_loader_visual_transform_probe --out port/level-loader/reports/visual-root-policy-sanitizers.json
```

Original symbol-bounded captures are retained under `reference/visual-root-policy`,
`reference/visual-root-construction`, `reference/visual-node-construction` and
`reference/visual-node-setters`. Test receipts bind their source/capture hashes,
the original ELF, Cpu helper and native probe. The prior module/Android
checkpoint remains historical; this follow-up does not rerun its 82 frames or
claim that every renderer detail is faithful.
