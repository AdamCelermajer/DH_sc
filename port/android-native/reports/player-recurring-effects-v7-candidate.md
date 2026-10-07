Player recurring-effects V7 candidate
====================================

The successor calls CharacterPlayerSkillsV6::session().update_timers(dt, blocked)
on the retained player's existing session. It owns no timers, VM, Save, properties
or callback service. The frozen CharacterPlayerSkillsV6::update_timers guard is
unchanged. Owner readiness, errors before/after traversal and session error are
returned separately from the native timer traversal status. No diagnostic is
cleared. A successful traversal must not be reported as successful callbacks.

Validation: NDK 29 clang++ aarch64-linux-android24, C++17, Wall/Wextra/Werror
syntax check PASS for player_recurring_effects_v7.cpp. This is compilation
evidence, not a composed timer33/35/36 runtime regression.

FaeryPressLifecycleV1: JBR javac + java standalone ownership test PASS. Two
presses produce one BeginCast1; release produces one EndCast3; repeated release
or cancel without a press produces no command; a subsequent press works.
GameplayHud integration and native source gates are the renderer's responsibility.

DoT player reaction prefix: original F_ApplyResult 3b17ec..3b1830 asks the actual
defender SM_IsIdle(false), then if idle and outcome bits0x16 are absent adds
injure0x10 for positive damage or dodge2 otherwise. New isolated V7 prefix
requires that genuine FSM query. WSL g++ C++17 Wall/Wextra/Werror with ASan+UBSan
PASS: 3072 input combinations and failed-query prefix preservation. ARM64 NDK
strict syntax PASS. These tests are source-derived host checks, not original
instruction differential evidence or a complete player DoT application.

Full positive player DoT still requires original player HitFor low-health,
tutorial/achievement and actual Apply FSM reaction services. The existing V6
nonplayer kernels retain their explicit unsupported boundaries. No health-only
subtraction, synthetic successful HitFor, fake status or world identity is added.
