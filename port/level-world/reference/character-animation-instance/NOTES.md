# Retained per-character native animation ownership

`character_animation_instance.hpp/.cpp` provides port ownership around the
recovered PAB1 decoder, resource `Player`, original occurrence registration,
dynamic compiler and two-slot `BlendedPlayback`. It does not substitute a new
AI, FSM, physical body or Character factory.

`CharacterAnimationResources` owns the immutable factory scene, full bank
metadata and every unique Player, including their byte buffers. Its reader
receives each complete resource descriptor; the module checks byte counts and
independently computes SHA256 of every resource using the native asset utility.
Failed loads, including full-size corrupted resources, preserve the previous
shared owner. This digest utility is a port implementation, not an original
engine counterpart.

Each immovable `CharacterAnimationInstance` pins that immutable owner and owns
its own Scene, SceneBinding, scheduler, compiled occurrence map, pose slots and
root/event histories. Registration preserves every original repeated occurrence
and maps a game dictionary ID to the first resource-identity occurrence. The
source default library is retained independently. Copying a model's mutable
scene per character avoids pose/clock aliasing between actors and permits CPU
backing to remain alive across GL resource recreation.

Selection calls the genuine native `BlendedPlayback::start` with caller-owned
AnimationRandom. Scene and Animator phases remain separate so the renderer can
perform every character's Scene work before one shared physics Step, then the
original Character phases. No private physics Step or second FSM elapsed update
is introduced. Genuine AI/FSM event observers and actual body/runtime producers
remain caller responsibilities; this ownership class does not accept their
absence as a completed full frame.

## Actual four-kind evidence

The new test executes all staged original-cache monster registrations and the
genuine owned StateOwner Idle Focus, using actual CharAnim Idle sequence values
and native playback selection. Its Character event1d observer is explicitly a
test fixture: full CharAI delivery is not proved by this composition.

| Character configuration | Resources | Ordered occurrences | Checks | Native frames |
|---|---:|---:|---:|---:|
| Crypt_Skeleton | 23 | 44 | 81,048 | 180 |
| CryptSlime | 23 | 37 | 26,328 | 180 |
| CryptSlime_RE | 23 | 37 | 26,328 | 180 |
| Crypt_Ghost | 18 | 28 | 130,003 | 180 |

Tests check first occurrence indices, full compiled occurrence counts, independent
pose/clock storage, finite transforms, failed-load ownership preservation and
continued playback after caller resource/factory owners are released. Ghost's
template start remains the signed value -133ms in both Player and compiled
library0. Source Idle sets flags0x2380 and runs native ANIM_Set. The wrapper is
port architecture over proved kernels, not an original ARM32 ownership-layout
counterpart or whole-frame instruction equivalence claim.

The central report
`../../reports/character-owned-monster-animation-initialization-main-linked-host-audit.json`
records 53 passing actual CMake suites with no sanitizer findings, including the
four above, retained full skill-to-character views and the original-authoring
CAI1 sidecar loader. It binds current source/input/compiler records and DSO
hashes before and after execution. The four staged bank assets remain outside
the Android asset directory at this milestone.

Both repository and Android Studio builds passed for ARM64/x86_64. The source
capture `.local-inputs/native-owned-monster-animation-initialization-build-capture.zip`
has 437 compiler-recorded source/build inputs and SHA256
`321bf331f3a7f7a57c331856e60da489ab6a687531e5dfab5a56993499969dac`.
`../../../android-native/reports/native-owned-monster-animation-initialization-source-build-inspection.json`
verifies these actual inputs, required exports, 266 existing prototype assets,
and sixteen ELF64 libraries with 16KiB alignment. Its live validation is NOT_RUN.

## Next live connection

Retain this instance and a genuine CharacterStateOwner on each existing
MonsterScriptHandle before its CharacterScriptSession member. Pass that same
native FSM to the session; publish original Init first, then initialize from
the actual CAI1 resolved preset through the recovered LevelLoadCharStates command.
Supply actual CharacterRaiseEvent1d→CharAI getter→OnStateChanged and animation
event/end helpers, rather than suppressing their synchronous callbacks. AISExternal
OnEndOfAnim is nonempty even though its current Lua callback is empty.

Use an independently computed digest of actual DACT bytes when loading CAI1.
Bind actual per-character positions/scales, source equipment/stance/property
facts, gameplay backing and RNG. Once a live actor uses this FSM/playback,
disable its parallel prototype controller/scheduler; rendering must use the
retained actor's own pose. Full physical/frame/timer/navigation/combat/AI services
are still required beyond the bounded startup milestone.
