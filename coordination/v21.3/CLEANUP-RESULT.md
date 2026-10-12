# Completed v21.3 cleanup — 2026-10-12

v21.3 is accepted; all177 tests pass and final independent domain/package reviews are READY. Source and reports were committed/pushed to the AdamCelermajer `publish` fork. No push was made to Noam's `origin`.

All67 extra Git worktrees were removed:65 through exact preserved-ref/clean-status checks and2 through Codex archive_worktree. Only the primary DH_sc checkout remains. This includes the final temporary C:\DHqa observer checkout. Each exact worktree HEAD is retained locally and on the user's fork under `codex/archive-wt-20261012-*`; mapping and original branches are in WORKTREE-PRESERVATION.json. The dirty generic-loader/native source was committed at8b322ec5 (13,677 files) and pushed on its separate archive branch before removal.

Shared `.local-inputs` junctions were unlinked nonrecursively; their primary target remains intact. Owned ignored input trees and the native local.properties file were preserved and hash-checked under `.local-inputs/cleanup-preserved-20261012`. Reproducible cache removal totaled2,345,464,142 bytes. Sixty-one obsolete build directories, including C:\DHqbuild, were removed after preserving arbitrary source/config/save/capture/log/executable evidence by content hash. Compiler/debug products were discarded. Hardlinks preserve unchanged files without duplicating their disk contents; originals are no longer in the removed build directories.

Actual free-space measurements and removal counts are in CLEANUP-RESULT.json. Raw action/preservation receipts remain in `.local-inputs/v21.3/root`: ignored-cleanup-receipt.jsonl, worktree-cleanup-executed.jsonl, obsolete-build-cleanup.json. The first build-copy attempt was stopped only in its own process to switch to verified hardlink preservation; its completed records and saved evidence were retained. No user's game process/window was controlled.

After worktree removal, root verified all4,420 accepted package manifest entries and all7 copied progress hashes. Previous accepted packages and original progress remain untouched. Unregistered scratch directories remain preserved; they are not extra Git worktrees.
