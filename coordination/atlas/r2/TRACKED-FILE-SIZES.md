# Generated-file tracking recommendation

Preserve the existing round-1 files for this round. A later repository cleanup should stop tracking the regenerable files below, keep source generators and compact checks/manifests, and use ignored runtime output or release artifacts for full snapshots.

| Existing generated file | Bytes | Action |
| --- | --- | --- |
| coordination/atlas/data/graph.json | 466723460 | Untrack in a separate reviewed cleanup; regenerate locally |
| coordination/atlas/data/references.json | 256903099 | Untrack in a separate reviewed cleanup; regenerate locally |
| coordination/atlas/data/mob-profiles.json | 128754365 | Untrack in a separate reviewed cleanup; regenerate locally |
| coordination/atlas/data/packed-sites.json | 92960087 | Untrack in a separate reviewed cleanup; regenerate locally |
| coordination/atlas/index.html | 72199016 | Untrack in a separate reviewed cleanup; regenerate locally |
| coordination/atlas/data/references-extra.json | 68719694 | Untrack in a separate reviewed cleanup; regenerate locally |
| coordination/atlas/data/ui-text-index.json | 63887735 | Untrack in a separate reviewed cleanup; regenerate locally |

Shard JSON by source category/act and stable IDs, or compress each section, with a manifest listing hash, row count and schema. Keep every tracked generated file below 40,000,000 bytes. Avoid embedding the complete graph into tracked HTML; the local viewer already queries a persistent ignored SQLite cache.

Removing files from the current tree does not remove oversized blobs from Git history. Before a later GitHub push, the repository owner must choose a separate history-cleanup or artifact-storage migration and preserve a recoverable local backup. This round does not rewrite history or push.
