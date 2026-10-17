# CPU pose retention at development world/GL recreation

This is a port lifecycle API, with zero newly reconstructed original bodies.
The caller retains the existing BlendedPlayback, its immutable ClipBank and
scene clock. The compiled TransformSet already owns copied resource bytes,
tracks, default values and event names; its compiled_bank check still requires
the canonical bank object at the same address. RegistrationSet may be temporary.

Snapshot capture copies only the actual Scene node IDs/parents/local TRS and
complete visual Root. It records the original animated node and the two owner
addresses. It does not own another playback, bank, timeline, cursor, completion,
frame schedule or GPU handle. Capture occurs while the existing owner is paused
at a completed source phase/frame boundary; the retained owner/bank remain alive
and unchanged through restore. The pending-completion test explicitly pauses
between completed scene and animator phases to verify queued selection retention.

Restore first validates node IDs/parent topology and same playback/bank identity.
It stages a fresh SceneBinding for the replacement graph, copies the retained
actual TRS/Root and invokes existing SceneBinding::update_world. Only a successful
staged result replaces the caller's graph and binding. Fresh materials and
instances come from the new graph; instance matrices use their current links.
Failures preserve graph/binding and a previous successful snapshot. The caller
clears the snapshot after successful restoration or terminal teardown.

SceneBinding::bind resets its private single-clip delta history. This API is
specifically for BlendedPlayback, whose authoritative root histories live in its
two retained slots; it does not restore single-clip Playback/SceneBinding sampling.
The complete Root includes owner/helper compensation and flags, so restore does
not repeat displacement or recompute a rotation from Euler facts.

Neither replay_phase nor scene_phase is a restoration operation: they update
timelines/fades, dispatch authored events, consume root deltas and set completion.
Reblending cached contributions with current weights is also incorrect after
time_phase, which intentionally advances timelines/fades/events while preserving
the last actual scene pose and root history. Snapshot restores the actual scene
bytes, including untouched or disabled channels, without any such work.

The selected-library test uses the packaged Prince bank's116 unique resources
and158 registration occurrences. It compares interrupted/recreated playback with
an uninterrupted retained owner at identical source timestamps. Cases cover a
real Idle→Walk fade, time-only pose, real attack events, closed Died playback and
real pending completion with a queued source Idle selection. Exact comparisons
include local/world/instance pose, full Root, slot timelines/event/key/root
cursors, weights, completion, aggregate delta, scheduler state, RNG and callback
receipts. Repeated restoration leaves the playback object and compiled resource
identities unchanged and produces no callbacks. Direct Attack/Died sequences are
declared test producers over real resources; no native game input or GPU parity
is claimed. Graph/root/owner/link failures are tested before committing output.

Native integration must capture before clearing the old scene; retain the bank,
playback and clock during ordinary world/GL recreation; restore after loading
the matching graph; and clear all retained owners on terminal world discard.
Source Idle/Move cancellation/refocus belongs after successful restore and can
then change animation through its existing source service. Nonidle preservation
must not compile/start/replay a replacement animation or synthesize a final pose.
