# Raw angle track, factory, binding and interpreter proof

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The manifest captures 16 actual source functions, including resource post-load,
typed factory/singleton, raw key wrappers, angle-axis conversion, accessor
dispatch/default helpers and scene quaternion retrieval. This module adds only
`angle_interpreter.hpp/.cpp` and dedicated tests; existing animation code is
owned and integrated separately by the parent.

## Runtime handler is independent of the first union handler

`CResFileManager::postLoadProcess` (`0x658c90`) traverses every 32-byte
SAnimation record at `0x658d40`, calls the actual `getAnimationTrackEx`
(`0x611ae0`) at `0x658d4c`, then stores its returned handler into **that raw
animation's +0x14** at `0x658d58`. Dynamic union `addAnimation` (`0x62f354`)
separately calls the same factory when first registering a compatible target,
retains the first channel record and stores that union handler in set +0x18.
Later compatible registrations do not replace the first union record/handler.

`getAnimationValue` (`0x65f7b4`) uses the union handler at `0x65f818..0x65f840`
only to obtain its value size and copy the selected binding's DB default into
output. Sampling at `0x65f8a0` calls actual SAnimationAccessor::getValue
(`0x66a1a8`), whose getAnimator (`0x66a004`) reads raw SAnimation +0x14, then
invokes that handler's virtual +0x68. Assigning the union handler to raw +0x14
before sampling changes the original behavior and invalidates cross-type proof.
The earlier manually mapped component captures are provisional and are not
reference inputs to this module. The parent's actual-raw-factories corpus
executes the real factory for both identities and mirrors the documented
post-load store as an explicit initialization fixture.

There is a deliberate compatibility asymmetry. Union registration reads the
compatibility mask for the existing first union type. The initialized masks
group 1..4 and 5..9. DB lookup (`0x61c0c8`), however, accepts raw 1..4 for requests
1..4; a request5 or9 accepts only raw5 or9 (`0x61c1b8`); request6/7/8 reaches
exact type/hash-byte comparison (`0x61c114/0x61c1d0`). Do not infer a completely
symmetric DB lookup from the broader union mask.

## Type9 is a scalar angle with authored axis, not scale Z

Factory `0x611ae0` dispatches types6..9 to `0x611d78`. With offset/scale variant
`SAnimation+0x1c == null`, `0x611f44` tails `0x6100dc`, whose exact symbol is
`CVirtualEx<CApplyValueEx<quaternion,CSceneNodeQuaternionAngleMixin<float>>>
::getInstance`. That singleton's value size is 16 bytes. No source branch
specializes type9 as an XYZ scale component. Full scale is type10; scale X/Y/Z
are11/12/13. Factory selection with compressed offset/scale variants is captured
but not substituted by this float/no-offset module.

The actual seven type9 records in supplied Prince ranged clips1077/1078/1079
have output scalar type6 (float), output component count1, no offset/scale
variant, and a present default variant with metadata `[8,4,value_pointer]`.
Clips1077/1078 each contain left calf, left forearm and right calf; clip1079
contains left forearm. The value keys are scalar radians; the raw default's
first three floats provide the axis. The fourth default word is replaced by
the sampled angle. Neither four-float key vectors nor scale-Z interpretations
describe these authored records.

## Two different default pointer paths

DB getDefaultValue (`0x61c6bc` -> `0x61c2f8`) queries **the union channel**.
For types1..4 its pointer is node+0x0c (full position); for types5..9 it is
node+0x18 (full quaternion); type10 is node+0x28 (scale). This optional binding
default is copied using union value size before sampling.

Raw accessor hasDefaultValue (`0x669e54`) separately tests raw SAnimation
+0x18 descriptor pointer. Raw getDefaultValue (`0x669e68`) dereferences that
descriptor's +8 value pointer. These are distinct from the selected clip DB or
dynamic default library. A DB default copy must not fabricate the raw variant.
Dynamic binding compile `0x62f830` queries clip DB first; `0x62f860..0x62f880`
then falls back to the explicit default DB when configured, even for a present
raw animation. Static template fallback is distinct: it applies to absent
animations only (`0x660980..0x6609cc`).

For position-component raw interpreters, a present raw variant fills all XYZ
and replaces the designated component. With no raw variant the scalar helper
writes output word0, including Y/Z handlers, leaving remaining caller words
intact. The outer DB copy and raw variant therefore must remain independently
represented. Full position/scale raw handlers read full vectors according to
their raw types; a scalar raw handler does not become a full-vector reader
because its first union type was full position.

## Recovered float angle kernel

Single-key scalar helper (`0x61f51c`) copies raw default XYZ then puts the key
at scratch+12. Linear helper (`0x61f898`) copies the same XYZ and evaluates:

`angle = a + fraction * (b - a)`

Each subtraction, multiplication and addition is rounded to float32 in source
order. There is no fraction clamp in this helper. The single-key conversion
wrapper (`0x61f594`) and pair wrapper (`0x61f94c`) invoke the actual quaternion
fromAngleAxis (`0x60cdbc`): half=angle*0.5, sine=sinf(half), W=cosf(half),
XYZ=axisXYZ*sine. The axis is not normalized; angles are not converted from
degrees. The native helper reuses the independently reconstructed existing
`dh2_quat_from_angle_axis` rather than copying or modeling its arithmetic.

`AngleAccessor24` borrows float values and a four-float raw default plus count
and zero reserved field. `dh2_animation_angle_key` and
`dh2_animation_angle_between` write a complete Quaternion and return0 on
success, -1 for malformed native inputs. All output/input overlap is rejected
atomically; no original aliased-input claim is made.

Missing raw default returns native -2. This is an explicit unsupported branch:
the original wrappers initialize only scratch XYZ to zero; their scalar helper
without default writes scratch word0 and leaves scratch angle word3 unset.
The pair helper also dereferences a present descriptor's null value without a
guard. Assigning zero/identity/DB default to that angle would invent behavior.
The authored seven records have valid defaults and exercise the defined path.
Delta wrappers `0x61f5e4/0x61f6f4` remain captured, separate source boundaries;
this module implements normal key and linear pair sampling only.

## Verification

Actual original wrappers, accessor output/default instructions and original
angle-axis math execute against optimized NDK29 ARM64. **1,491 cases pass**,
including **147 samples from seven genuine authored tracks**, 2,982 ordered
libm calls, and15 atomic native guards. Host ASan/UBSan replays the same gold
with no findings. Synthetic cases cover unnormalized/zero axes, signed zeros,
negative/outside-unit fractions, float overflow, infinities and NaNs.

Gold `angle-fixtures.bin` SHA256:
`4760cda2097adf8a0c6024c51cfea9cb679acf56c66f357890f683fe26ddcd00`.
Reports bind final source/test, original ELF, actual built binary and authored
asset hashes. All finite and signed-zero output words are exact. Arithmetic
NaNs compare classification; 270 words differ in NaN payload under the two
instruction architectures. This is not a copy-payload tolerance.

Imported sinf/cosf use the same Windows UCRT contract on both instruction CPUs.
Compiler-generated sincosf uses the same two modeled imports. Native/source
conversion arithmetic still executes; no math algorithm is replaced. Host
replay binds exact original libm call inputs/results, including the combined
call fixture where applicable. Historical Android Bionic libm equivalence,
search/timeline, generic compressed tracks, complete resource loading and APK
or renderer parity are not claimed.
