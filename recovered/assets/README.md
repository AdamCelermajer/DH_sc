# Recovered shader and configuration evidence

`source-data/` contains exact historical recovery ZIP members: **2,164 original
text resources**, totaling **4,397,508 bytes**. Of these, 34 are marked shader
source and 2,130 are other text resources in the archived provenance. Their
serialized filenames are retained, including `.glsl`, configuration, particle
and scene resource extensions. They are evidence from the supplied cache,
without a game-wide open-source license grant; see [rights](../../RIGHTS.md).

The unchanged `source-data/provenance.json` records the earlier, incomplete
cache archive (`f01c1657…`), file hashes, CRCs and nested shader archive members.
`cache-manifest.json` is that historical extraction's accounting, including
its truncation. These reports must not be described as the later complete
cache audit. [The complete cache](../../docs/COMPLETE-CACHE.md) is a different
owner-supplied archive and remains outside Git.

The [import ledger](../../reports/remaining-recovery-evidence-import.json)
pins all of these bytes against the verified recovery ZIP, together with
48 native debug export files. [The verifier](../../tools/verify_remaining_recovery_import.py)
checks the checkout and optionally all corresponding ZIP members:

```sh
python tools/verify_remaining_recovery_import.py
python tools/verify_remaining_recovery_import.py \
  --archive /private/Dungeon-Hunter-2-Source-Recovery.zip
```

This import supplies shader/configuration evidence for renderer and resource
reconstruction. It does not integrate their consumers or compile a complete game.
