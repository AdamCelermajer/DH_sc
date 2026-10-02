# Archived native assembly and symbol evidence

The [`assembly/`](assembly/) and [`symbols/`](symbols/) trees are exact text
exports from the historical recovery bundles, copied as ordinary Git files.
They can be read alongside the [Ghidra pseudocode export](decompiled/README.md)
and the independently reconstructed modules under [`port/`](../../port/).

| Bundle | SHA-256 | Imported files | Uncompressed bytes |
| --- | --- | ---: | ---: |
| `assembly.tar.gz` | `e7bdc73d7db5c0b8320815f92c7d27c9c86c17710649645c027a4df4081e5601` | 3,625 `.asm` | 134,762,768 |
| `symbols.tar.gz` | `f2dd9d64a9f19a66cfd05e194d30c7482c399ef07d7b488d737ac929652481ca` | 71 JSON/CSV/Markdown | 166,746,022 |

The [per-file manifest](evidence-manifest.json) records byte counts and SHA-256
values for all 3,696 files. The archived [`symbols/README.md`](symbols/README.md)
is itself an exact bundle member; this page is the authored guide. Some exports
exceed 20 MB and may need a local clone or raw-file view rather than a web code
preview.

These are generated, binary-derived records: grouped ARM/Thumb disassembly,
symbol tables, function/address indexes, relocations, strings, vtable and unwind
metadata. They are **not original studio assembly or C/C++ source**, a
compilable game project, or proof that inferred names and types are correct.
The original ELF binaries, APK, cache, asset XML/shaders and compressed bundle
archives are not included in this import. See [`RIGHTS.md`](../../RIGHTS.md):
these records retain the supplied materials' provenance and rights, and the
repository does not assert a game-wide open-source license.

Verify the checked-out files against their manifest:

```sh
python tools/verify_native_evidence_import.py
```

With the separately downloaded `assembly.tar.gz` and `symbols.tar.gz`, also
verify the pinned bundle hashes and each exact archive member:

```sh
python tools/verify_native_evidence_import.py --bundle-dir /path/to/bundles
```
