# Same-world player attack coordinator

The successor composes existing original-source `dh2_character_cmd_attack`,
`dh2_character_ai_melee_attack`, target search/heap, SetTarget/BackupTarget and
`world_ai_can_attack_v1`. It borrows the registered player's SAME State and
TargetState48; target/last-target, flags and heading are refreshed projections.
AttackState64.owner_flags528 borrows State.attack_gate (Character+528), never
State.flags (Character+520). Attack focus flags2341 have bit1 independently;
only the source cooldown/timer gate prevents a repeated command.
It owns only temporary target-list storage and failure diagnostics.

Unheaded source attack resets sort to zero, preserving source registry/heap
ordering. Heading-active attack keeps closest sort and derives its cone from
the real CharacterDesign/Attack_FrontalAngle constant. No nearest-actor shortcut
is used. CanAttack uses complete registered-world enemy classification and
mandatory equipment/geometry query providers.

Character constructor source 0x3a9594 stores signed OOI type byte +0x14a8=-1
(r6=-1). Source 0x3a967c stores OOI pointer +0x14a4=0 (r8=0). CharAI constructor
0x3cedc0 stores seeking byte +0x4a=1. The supplemental attack index +0x74,
continued +0x78, last +0x79 and finisher +0x7a are not initialized in the captured
CharAI constructor; their actual initialization/state producers remain required.
The helper does not invent zero initializers for them. Constructor evidence is
already retained in character-idle-startup/reference/original-functions.asm.

Reached debug/frontal constant, ranged attack, network send/mode and attack-FSM
bodies remain explicit providers. FSM backend must execute the original
SetAttack/event 0xc354 on the same machine, never directly assign an animation
state. Unknown providers latch an error through the old void callback ABI;
subsequent mutations are suppressed and completed source prefixes remain.
There is no exception crossing the source C boundary.

Host ASAN/UBSAN checks cover full world search, different headed/unheaded order,
same SetTarget/BackupTarget, continued attacks, controller lock, and a reached
range-provider failure after SetTarget with no FSM transition. Test geometry,
debug and FSM callbacks are explicitly fixtures, not production providers.
Strict NDK syntax checks pass both arm64-v8a and x86_64. Production renderer
integration and whole native attack execution are not claimed by these tests.

Existing trophy/execution checkpoint 4156b785 sources were not changed.
