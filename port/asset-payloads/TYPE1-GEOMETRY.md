# Bounded type-1 geometry support

## Implemented contract

The cache contains nine geometry type-1 records. This component supports them
as a narrow immutable inspection view, using only properties shared by all nine
owner-supplied inputs:

- the type-1 record is geometry index zero;
- its ID is `Circle01-spline` or `Line01-spline` and its name is empty;
- its five uninterpreted prefix words are exactly `[0, 15, 3, 0, 0]`;
- the adjacent geometry index one is type 0; and
- the adjacent row's payload offset is exactly the type-1 payload offset plus
  20 bytes.

`dh2_type1_geometry_open` validates this relationship, opens the shared suffix
with the normal checked mesh reader, and returns geometry one through
`source_mesh_geometry`. It rejects altered prefixes, identifiers, alias types,
alias offsets and malformed nested data. The view borrows the immutable BRES
image and does not allocate, relocate or mutate asset bytes.

The targeted test pins all nine file hashes, record/payload offsets, IDs,
adjacent mesh IDs/names, materials, vertex/index payload hashes and decoded
counts. It also confirms that both rows borrow identical vertex/index bytes and
that the input remains unchanged. No game BRES bytes are stored in this
repository.

## Original-code boundary

The APK evidence does not contain a runtime type-1 parser. In the checked
`libDungeonHunter2.so` (SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`),
`CColladaDatabase::constructGeometry(IVideoDriver*, SGeometry*)` at
`0x0060e634` is 132 bytes with SHA-256
`d7c22aecae934a74d1038d0c772e343ec9ce9e628d8534e4cd136978f70292c2`.
It calls the geometry factory only when `SGeometry+8` is zero and returns null
for nonzero types. The known index, name, scene-instance and morph routes all
return to that gate.

Accordingly, `dh2_mesh_open` preserves the original rejection of the type-1
row. The supported source mesh is the adjacent type-0 row identified by the
cache alias relationship. This implementation does not route type 1 into the
normal renderer or claim original spline behavior.

The APK/call-route hashes and the cache alias audit are pinned in
[`reports/branch-audit-2026-10-05/engine-branch-evidence.json`](../../reports/branch-audit-2026-10-05/engine-branch-evidence.json).
The exact per-input expectations live in
[`tests/type1_geometry.py`](tests/type1_geometry.py).

## Remaining limit

The meanings of the five prefix words, the logical outer-payload extent and any
authoring-side or unlabelled spline consumer remain unknown. Inputs that merely
resemble these records are rejected instead of being treated as a generalized
type-1 format. The generic follow-spline animator found in the APK accepts an
owned vector of points; no source-backed path connects it to these BRES rows.

Run the bounded gate with owner-supplied cache files kept outside the checkout:

```sh
python build.py
python tests/type1_geometry.py \
  --cache /path/to/cache/files \
  --library build/libdh2_asset_payloads_host.so \
  --report /tmp/type1-validation.json
```
