> Imported static research from engine branch `e6da25b`. Current implementation and native wiring are tracked in the [branch audit](../../docs/BRANCH-AUDIT-2026-10-05.md).

# Camera and light runtime evidence

This package documents the engine-side camera and dynamic-light paths recovered from the supplied APK's ARMv7 library. It contains exact recovered ARM disassembly and a machine-readable provenance manifest. The ARM listing is evidence, not assembler-ready source.

## Files

- `ANALYSIS.md` summarizes the camera matrix/projection path, frustum updates, and light-node handoff.
- `original-functions.json` indexes 36 physical function ranges and two supporting camera vtable words. It records each address, size, SHA-256, source listing, and APK verification result.
- `reference/original-functions.asm` contains the selected ARM disassembly excerpts copied from the repository's recovery workspace.

The package reconstructs no game-specific camera controller or level light policy. It adds no C++ helper: the recovered operations depend on the engine's matrix layout, object offsets, virtual interface, and renderer state.
