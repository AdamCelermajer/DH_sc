# FX77 material color capability

Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The adjacent manifest binds35 complete function byte ranges. The source probe
and original/optimized ARM64 differential are reproducible without an Android
build, emulator or shared library rebuild.

## Actual resource and typed factory

`zombie_spawn_fx.bdae` is17288 bytes, SHA256
`b54e3b05ba684eca654c8a58b8ebd5e84f7cad25929e0b5a60405f66dfdc772e`.
FX77's nonparticle channel is URI `Standard_13`, type86, parameter
`diffuse-color`. Its sampler produces one unsigned byte per key; its default
metadata is type8/count4 with four bytes `ff ff ff 00`. There are12 alpha keys:
`00 00 80 ff ff ea bf 95 6a 40 15 00`. The clip range is0..1833ms.
The two other channels, `gl_pcloud_debris-emitter` and
`gl_pcloud_dust-emitter`, are type28 and remain unsupported.

The probe executes actual factory `CColladaDatabase::getAnimationTrackEx`
`0x611ae0`: type86 dispatches through`0x611c88`; unsigned-byte scalar sampler
through`0x611e70`; default type8 through`0x611ecc -> 0x61200c -> 0x611924`.
Result singleton`0x9f6ef8`, vtable`0x97a618` plus8, is the **uchar4 material
component3** handler. It is not a generic scalar material track or a full-color
track sampling four values per key. Only initialized C++ static singleton
storage is supplied as a factory fixture; the selection instructions execute.
getValueSize`0x60edec` returns4.

Four-byte colors remain in source storage order. Component3 is alpha. No
independent native red/blue channel reinterpretation is introduced here.

## Exact indexed sampling and contribution

The new `material_color.hpp/.cpp` expose native `ColorAccessor24` and
`dh2_material_alpha_key`, `between`, `delta`. They reconstruct the original
component3 interpreters`0x61a3fc`, `0x61a484`, `0x619978`. The original source
accessor getter bodies`0x669e24/0x669e54/0x669e68` execute in the differential.
No replacement key-selection/timeline/cursor producer is claimed.

With default metadata present, key sampling copies three default bytes and
writes the selected byte to component3. Metadata absent writes component0
instead, retaining the remaining caller bytes. A key call with metadata but
null value pointer also follows that component0 branch. Interpolated and delta
calls dereference the default pointer when metadata exists; the native caller
contract rejects that unsafe null case atomically with`-2`.

Between keys, the source performs separate f32 operations:
`float(first) + fraction * float(next-first)`. Delta first converts each signed
byte difference from the reference to an unsigned byte, then interpolates those
wrapped values. This differs from interpolating a signed difference directly.
No positive-factor check or clamp to0..255 is invented.

`dh2_material_color_blend` reconstructs actual`0x626124`: count0 produces four
zero bytes; count1 copies its four input bytes and ignores weight; other counts
multiply and add each byte in slot order from+0 using separately rounded f32
operations. Every component is converted through original`__aeabi_f2uiz`
`0x8be2a0`, then its low byte is stored. Negative/subunit values and NaNs
convert to0; positive overflow/Inf saturate toUINT32_MAX and store255.
Values such as256 can wrap to byte0. This is not a saturating uchar lerp.

Caller output/input overlaps, negative contribution counts, invalid key indices
and malformed native spans are rejected atomically. These are bounded native
caller contracts, not original ARM checks. Nonfinite fractions/weights remain
valid arithmetic inputs; the final converted byte comparison is exact.

## Actual material application and owned binding metadata

Original key application`0x622a18` and interpolation application`0x622a74`
evaluate the component handler, obtain unsigned16-bit parameter index from
applicator-info+8, and call real `CMaterial::setParameterCvt<SColor>`
`0x5cad38` with element0. Source blended application`0x625a28` does the same
after uchar4 contribution. Both application routes execute in the differential.

`ColorParameter32` is one **already resolved** runtime parameter projection;
it is not an ARM32 CMaterial object or a shader/material factory. Native
`dh2_material_color_set` reconstructs the proved runtime storage types8 and16:

* Type8 writes four source-order float words. Each byte is converted to float
  and multiplied by exact factor bits`0x3b808081`. The source compares the prior
  components numerically; a difference sets CMaterial cache stamps+0xc/+0x10
  to`-1`. Every converted component is stored even when numerically equal.
* Type16 copies the four packed source bytes; a packed-word difference sets the
  same cache stamps. The remaining projection words are retained.
* Element index outside the source declaration count returns false without a
  write. For valid elements, the source body uses the declaration's stored
  byte offset directly; it does not add an element-index stride in these paths.

Other runtime types are explicitly unsupported`-2`; no original type17
conversion/helper acceptance is substituted. Declaration index/name lookup,
allowed-type masks, runtime byte offsets and target ownership must be supplied
by a genuine material backend before this projection is integrated.

The source probe executes original `forceBind(0x667f18)` and
`getParameterIndex(0x5d308c)` against an explicit named runtime-directory
fixture. Material search`0x65ca30`, interned string identity`0x6a5074` and the
setTarget receiver are explicit services. Source cases confirm:

* Found material/parameter: binds the material and index0.
* Found material/missing parameter: still binds it with index65535.
* Missing material: binds null with null applicator info.

Original `CMaterialApplicatorInfo::clone(0x667ed0)` allocates12 bytes and copies
the name identity and parameter index into owned metadata in both found-material
cases. Allocation is the explicit fixture; clone instructions execute.

Do not use the direct typed `applyValue(0x6229d0)` as an invented fixed path:
this shipping wrapper sets its info register to0 and dereferences+8. The complete
generic single-slot default-application producer has not been reconstructed.
The verified key/interpolation and blended application routes have proper info.

## SelfIllum does not write a lighting flag in this shipping helper

`AnimatedFX::Load` calls `SetNodeToSelfIlluminated(0x50e398)` when the metadata
SelfIllum byte is true. The name alone is misleading. Actual `ISceneNode` C1
`0x599268` produces primary vptr`0x976b24` = vtable+0x1c (virtual-base prefix).
Virtual+0x88 therefore resolves **getMaterialCount**, not a lighting setter.
Base getter`0x5970b0` returns0; mesh override`0x6461cc` queries its mesh's
virtual+0x10 count. The helper calls that virtual once, discards the count, then
recursively visits intrusive children at+0xf4, reading next after recursion.

The original probe executes the constructor, getter and helper over a three-node
base tree. All node bytes remain unchanged, and getter visitation is parent then
children in list order. It does not infer effects of arbitrary unrecovered
virtual overrides, fabricate a mesh count provider, or set native shader flags.
The literal helper body contains no per-material loop or flag assignment.

## Remaining genuine engine capability

Raw Standard_13 material points to
`#ProfileCOMMON_Standard_13-fx1302961790_zombie_spawn_fx` and contains
`__irrlicht_Diffuse_color` (raw type7), while its animated bind name is
`diffuse-color`. Native Scene's reduced material projection does not retain a
runtime parameter directory, owned CMaterial/applicator info or source shader
conversion. Aliasing those names by hand or writing Scene.color is not proved.

Source factory paths captured for next recovery are game
`ColladaFactory::createMaterial(0x3506e0)` -> engine`0x6323d0` ->
factory material-renderer virtual+0x1c -> material instance construction
`0x631ce8`. Renderer-manager `createMaterialInstance(0x5d9af0)` remains an
explicit deeper capability. Root material lookup`0x65b538/0x65ca30` retains
material instances by name. These factory bodies are captured, not implemented
or executed as successful native services by this stage.

Minimum next integration is source-backed runtime material definition/instance
ownership and name binding, then source accessor/time/clone attachment to that
target. Particle28 scene construction, emitter simulation, factory resource
ownership, AnimatedFX playback/event lifecycle and GPU execution remain open.
The current module is reusable typed engine math/application logic, not FX77
startup/render parity.

## Proof and isolated integration

`reports/material-color-arm64-differential.json`: original versus NDK29 O2
ARM64,7,899 comparisons,1,740 actual FX77 key/interpolation-to-material writes,
1,200 actual original blended-to-material writes,13 native atomic guards,0
mismatches. Includes missing-default/source component0 branches, wrapped deltas,
zero/single/multiple slots, negative/overflow/nonfinite arithmetic, source
parameter-range rejection and cache-stamp behavior.

`reports/material-color-host-audit.json`: the same original-derived binary gold
through a genuine isolated `libdh2_material_color_audit.so`,7,899 cases and13
atomic guards, AddressSanitizer/UndefinedBehaviorSanitizer findings0. No central
DSO was rebuilt or borrowed; no historical Bionic libm service is needed by
these kernels. Library/executable/source/gold hashes bind the report.

Reproduce from repo root with the direct configured Python and dependency path:

```powershell
powershell -NoProfile -File port/engine-animation/tools/build_material_color_oracle.ps1
python port/engine-animation/reference/fx-material-animation/probe.py
python port/engine-animation/tests/material_color_differential.py
python port/engine-animation/tests/material_color_host.py
```

Parent integration: add only `material_color.cpp` to engine-animation. A host
audit target can link `tests/material_color.cpp` against that actual animation
DSO and take one argument, `reference/fx-material-animation/material-color-fixtures.bin`.
No libm wrappers or scene/runtime dependencies are required for this test.
