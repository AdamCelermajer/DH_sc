# progress — slice `world-misc` (category game-world minus the object classes)

Tooling: read-only. `showfunc`-style dumps were produced from the worklist TSV plus the
reconstruction located by its `// 0x<addr>` comment (see "queue traps" below). No file outside
`...\deepseek-audit\_parts\` was written; no git, no build.

## Worklist and coverage

`index/queue_world-misc.tsv` = 580 rows: prio0 7, prio1 264 (**180 `const:` + 24 `float:` leads = 204**),
prio2 2, prio3 33, prio4 274.

| prio | rows in scope | audited | how |
|---|---|---|---|
| 0 | 7 | 7 | both sides read in full |
| 1 (`const:`/`float:`) | 204 | 204 triaged, ~35 read line-by-line | automated semantic delta + manual reads |
| 2 | 2 | 2 | read (one with the inlined STL/refcount lines filtered) |
| 3 | 33 | 0 | **not reached** |
| 4 | 274 | 0 | **not reached** |

The 204 prio-1 rows were first passed through a normaliser that (a) masks Hex-Rays member accesses
(`*((_DWORD *)this + 45)`, `*(float *)(a1 + 308)`, `a1[7]`, `(char *)this + 1074`) so that raw offsets do
not count as constants, (b) normalises `0x..`/decimal/`float f`-suffixed literals to one form, (c) treats an
integer as a float bit pattern when it decodes to one, (d) extracts literals used in a *value* context
(comparison, assignment, argument, `*`/`/` scale, `&`/`|` mask) and drops offset/index uses, and (e) compares
those sets plus the comparison-operator and call-name sets against the reconstruction body. ~55 rows came out
with **no delta at all**; the other ~150 produced only the deltas listed below, each of which was judged by
hand. Rows read line-by-line on both sides (the strongest evidence):

prio0: 0x479fc8, 0x312848, 0x4933e4, 0x524230, 0x5241c0, 0x5241d4, 0x350e80.
prio1/2: 0x40f4dc, 0x40f714, 0x40f2f0, 0x40f394, 0x41165c, 0x4117bc, 0x40a8f8, 0x40b3c8, 0x40b5c8,
0x40a784, 0x40b828, 0x495b54, 0x4956b8, 0x410930, 0x410ea4, 0x41106c, 0x35f348, 0x35f780, 0x352c3c,
0x520588, 0x51fa64, 0x51badc, 0x40d368, 0x40cbc0, 0x40cff0, 0x355704, 0x418c48, 0x312b6c, 0x34bd08,
0x362810, 0x36299c, 0x40b674, 0x40b720, 0x35d168, 0x35d4cc, 0x363958, 0x361298, 0x411234, 0x410aec.

## Findings

2 findings, both in `CameraOverview::CameraOverview()` (0x410ea4 = C2, 0x41106c = C1; one C++ definition
covers both, so both omissions live in the same body and are not double reported):

1. **medium / side-effect** — the original links the freshly created camera node into the scene graph
   (`addChild`, vtable +0x5c of the scene manager's ISceneNode subobject at `*(device+0x1c)+4`,
   chunk_004.c:5940-5943). The reconstruction passes a null parent to the factory (which only adds a node
   when the parent is non-null) and never calls `addChild`, so the overview camera node stays outside the
   graph.
2. **low / call** — the original calls `setUpVector(0, 1, 0)` (vtable +0x114) on the camera node before
   `setTarget` (chunk_012.c:5946-5950); the reconstruction performs only the `setTarget(0,0,0)` call.

Severity counts: high 0, medium 1, low 1.

## Deliberately not reported (documented deviations, verified)

* `Vector3DFList::addClosest` — the listing leaves a float/bool in r0 (the caller's `result` is even returned
  by the GetPositions template), the header and the ledger (game-world/ledger.csv, 0x479fc8) declare void and
  say so. Not a bug.
* `SceneManager::ChangeCustomXrayTechnique(..., bool c)` — the third flag is never read
  (chunk_004.c:5305-5353 reads only a3/a4); the reconstruction passes three `true`s and documents `(void)c`.
* `BufferedRenderer::reset` returns void in the port; the ledger records that the single caller ignores the
  returned technique.
* `SceneManager::DisableFog` — the recon writes `m_lightSetManager.m_fogEnabled`; the header split places that
  byte at +0x432, equal to the listing's `+1074` (chunk_004.c:735). The prio-0 ARMDIFF "word 0x41000000 vs
  0x41200000" is an unrelated state difference in the harness.
* `PFFloor::_CreateNodes` returns early for `count == 0` as well as for the void-floor flag; the listing's
  loop would not run anyway (ledger documents it).
* `CameraBase::GetScreenCoord`, `GetWorldCoord` — the extra null guards are marked `host-necessitated guard` in
  the source.

## False-alarm classes seen in the prio-1 leads (all checked, none reported)

* float bit patterns: 1065353216 = 1.0f, 1120403456 / -1027080192 = +/-100.0f, 1128792064 = 200.0f,
  0x80000000 = -0.0f, 953267991 = 0.0001f, 981668463 = 0.001f, 1016003125 = 0.017453292f;
* truncated decimal renderings that are the same float: `0.017453` vs `0.017453292519943295f`,
  `0.0001` (0x38d1b717), `1000.0`/`1000.0f`, `1e3`/`1e6` (SetAttenuation);
* STL growth/deallocation constants: 0x80, 0xFFFFFFFC/F8, 0x3FFFFFFF, 0x1FFFFFFF, 0x15555555,
  -858993459, 1431655766, 357913941, 715827883, 678152731, 1356305462, 56512727;
* printable ASCII literals (68 'D', 86 'V', 83 'S', 78 'N', 85 'U', 65 'A', 88 'X', 87 'W', 81 'Q');
* `operator new(size)` sizes (124, 360, 84, 44, 40, 380, 416, 328, 336, 352, 12, 24, 48) and row strides
  (48 = sizeof(AnimFX), 24 = sizeof(AnimFXSetInfo), 12 = vector3d<float>, 20 = a light-set row);
* macros/functions of the glitch engine that are pure inline expansions of the C++ source
  (`LODWORD`, `COERCE_FLOAT/INT`, `LOBYTE`, `CustomFree`, `GlitchFree`, `_M_deallocate`, `intrusive_ptr` ctor/dtor);
* `|`/`&` bit masks expressed as named flags in the port (e.g. `flags & 0x400` vs
  `ESNF_ANIMATE_ONLY_VISIBLE` in 0x35d168 — the enum values in `ISceneNode.hpp` were checked and match
  0x001/0x020/0x200/0x400);
* vector<bool>/vector internals rewritten as `resize`+loop (0x40cff0 InitLightFilter: shrink/grow to 5 then
  set entries 0..4 either way) and vector `clear()` (all `D1`/`D2` destructors, 0x51cca4, 0x51ce3c, 0x521c54,
  0x521db0, 0x524750, 0x524f50, 0x356bec, 0x4950a4, 0x49514c, ...);
* `setUpVector`/`setTarget` argument registers and the `(0, 1, 0)` axis of `quaternion::fromAngleAxis` in the
  CameraOverview constructor (only the missing *call* is reported, not the axis).

## Coverage gaps / things I could not do

* prio 3 (33 rows) and prio 4 (274 rows) were **not audited**.
* ~20 prio-1 rows whose only deltas were STL/refcount inlining and where the reconstruction body is 5-10x
  shorter than the Hex-Rays body (0x3587a0 `_renderLists` 673 lines, 0x358008 `renderCustomLists` 402,
  0x51fd78 `PFFloor::_Link` 337, 0x3e693c `ProjectileManager::_Create` 326, 0x352fa8 `AutoLoadNormalMap` 271,
  0x50dc84 `BatchNodeCompiler::Compile` 245, 0x363138 `Xray::render` 216, 0x7d7f28, 0x5219cc, 0x394xxx ...)
  were screened by the automated delta only, not read line by line; their remaining deltas are the vector
  growth constants listed above, but a re-read is advisable if the parent wants prio-1 exhaustiveness.
* Sub-agent delegation was attempted but is rejected at this depth (`subagent depth 2 exceeds maxDepth 1`),
  so all rows were processed in one session; that is the main reason prio 3/4 are untouched.
* Six destructor rows have no anchor in the reconstruction and were skipped by the locator:
  0x362e1c, 0x360aa8, 0x360c7c, 0x35b910, 0x3e715c, 0x35babc (their `// 0x...` comments live in index blocks
  or in another definition's comment, not above a definition).

## Queue traps found while working

* Rows whose demangled symbol contains a tab (0x520588, 0x51fa64, 0x51badc, 0x51b96c, 0x51b874) break naive
  `csv.DictReader` parsing of the TSV (an unbalanced `"` in the symbol swallows following rows: 247 instead of
  580 rows). Parse with `csv.reader(..., quoting=csv.QUOTE_NONE)` or a manual `split("\t")`.
* `recon_span` is stale/collapsed for a number of rows (e.g. 0x3587a0 `8-8`, 0x359b68 `14-14`, 0x410ea4
  `176-176`, 0x40d5a0 `282-311` vs the real 276-304). Locate the function by its `// 0x<addr>` comment and
  require that the next `// 0x` hit is far away, otherwise the file's leading address index is matched.
