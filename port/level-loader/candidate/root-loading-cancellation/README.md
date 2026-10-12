# Integrated loader cancellation

The active root loading connection previously exposed only normal GS updates.
This change forwards explicit request_cancel/drain_cancel through its existing
three owners to the ONE actual SourceLoading driver. Normal frames stop while
cancellation is requested, and draining performs loader cleanup without the
per-frame loading prefix, menu update or gameplay continuation.

Main keeps the existing sibling connection alive, calls request_cancel, and
drains until cancelled. On failed cleanup, keep all owners, surface the error,
call request_cancel again and retry. The lower lifecycle already requires this
explicit retry after failure. Release the connection only after cancelled; then
perform the existing GS/publication teardown. A changed publication can still
be cleaned using the pinned source owner, but must never resume normal updates.

These are adapter operations, not invented original Level.Update behavior.
Stage7/native-file cleanup implementations remain in their existing workstreams.
No new current-Level slot, manager, save owner or readiness state is introduced.
Source implementation only: validation is reserved for the integrated milestone.
Shared source files are unchanged. Apply the surgical diff to current Main code.
