# Dungeon Hunter 2 reconstruction

## Investigate before implementing

This rule applies to every feature session, including root, the integration lead,
the dispatcher, ordinary workers, reviewers, and resumed sessions. It was explicitly
requested by the user on 2026-10-09.

Before implementing or changing game behavior, investigate the corresponding
original behavior and record a concise evidence note in the existing feature
report or handoff. A completed helper or source audit is not a completed feature.

The note must include:

1. **Visual evidence:** inspect the relevant original gameplay sequence or supplied
   screenshot. Record the reference, timestamps/frames, and what was actually
   observed. Use a sequence of frames when timing, animation continuity, hit
   reactions, targeting, or transitions matter. A still frame cannot prove a
   continuous behavior. Distinguish inference from direct observation.
2. **Logic evidence:** investigate IDA decompilation/disassembly or an existing
   recovered-source handoff for the same behavior. Identify functions/addresses
   or exact source locations, conditions, state, timing, and side effects. Examine
   the callers and admission gates as well as the function that performs the
   action. Do not infer an outcome from HP loss or a single flag when a source
   gate, cooldown, target state, or class/weapon rule also controls it.
3. **Expected behavior:** describe the visible result and gameplay rules to
   preserve, then the minimal reusable implementation that produces them.
   Fidelity concerns content, art, gameplay and timing; it does not require
   reproducing the original engine's internal object graph or callback structure.
4. **Uncertainties:** state missing evidence and possible version differences.
   Do not turn an unknown into a guessed rule or call it original behavior.
   Continue independent work while acquiring the missing evidence. A deliberate
   user-requested PC input adaptation should be identified as that adaptation.
5. **Verification:** define a focused, observable test before coding, then run it
   as soon as the feature is testable. Verify UI visually and verify behavior in
   the real shared session where applicable. Do not wait for packaging to discover
   whether a feature works. Test outcome branches and failure/duplicate cases,
   rather than only the easiest successful case.

Reuse valid existing evidence instead of repeating investigations. Recheck it
when the behavior, asset/version, or assumption changes. For internal infrastructure
with no direct original visual counterpart, investigate its original consumer and
the visible/gameplay invariant it supports; do not invent a footage requirement
for an invisible implementation detail.

Existing sessions must audit their current work against this rule before calling
it complete. Reviewers verify the evidence covers the actual implementation.
Reports must distinguish investigation, implementation, isolated tests, integrated
runtime/visual verification, and remaining gaps. Audit-only and helper-only
sessions require follow-up ownership until the requested behavior works.

## Coordination and releases

Root owns the goal and accepted releases. The integration lead owns main/CMake
integration; the workforce dispatcher maintains useful assignments and reopens
unfinished feature work. Every reported bug needs a named implementation owner
and reviewer. Split bounded disjoint work among agents when useful; avoid two
writers editing the same files concurrently. Ordinary workers use GPT-6 Luna
HIGH per the user's current cost preference, with GPT-6.1 Sol MEDIUM escalation
when needed. Root and the integration lead retain their current models.

## Bug priority and immediate preview flow

The user clarified this workflow on 2026-10-10. Bugs take priority over feature
work. `docs/BUGS-AND-IMPLEMENTATION.md` is the shared editable task authority for
root and the workforce dispatcher. Preserve the user's additions and stable IDs.
Read its user inbox before assignments and after work/test/integration updates.

When a new bug is added, immediately assign a **fresh subagent**, rather than
reusing a completed conversation. Give it a bounded implementation scope and a
reviewer. If the available agent slots are full, record the queued bug honestly
and free a slot from lower-priority feature work; never claim an agent was opened
when it was not. Existing workers may finish their current source handoff, but
newly reported bugs get fresh implementation sessions.

The worker investigates original visual sequences and IDA/recovered logic,
reproduces the actual failure, creates the smallest reusable patch, checks its
logic and runs a focused test. It then sends the concrete patch/API and evidence
to the integration lead. Exclusive file ownership still applies: workers propose
changes to shared main/core/build files to their current owner instead of racing
that owner. Private tests may compile source-terminal code; obsolete source holds
must not be carried into new assignments.

The integration lead applies/wires the fix and verifies its real production path
with the relevant logic and visual tests, using isolated saves and test windows.
Identify the tested frozen executable and preserve source, test and runtime
evidence. A helper pass, non-reproduction, or compile alone does not resolve a
reported bug. Do not hide an unknown by inventing source behavior.

After a fix is implemented and its integrated checks pass, remove its entry from
the active bug table and append the stable ID, fix, evidence and build to
`docs/RESOLVED-BUGS.md`. Keep that history instead of deleting the evidence. Root
accepts/packages a preview as soon as the actively assigned fixes for that preview
are verified; wait for remaining active bug fixes, without holding them behind
unrelated future feature work. New feature gaps remain in the Act 1 TODO section.

Preserve accepted packages and user saves. Tests use fresh isolated saves and
their own windows; do not control the user's live game window. Confirmed reported
bugs remain gates for the next preview, even when a helper's tests pass. Do not
claim a feature is playable until its normal production path is integrated and
verified. Keep all game systems reusable across maps, classes, weapons and actors.
