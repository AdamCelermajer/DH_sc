# Authored declaration preparation milestone

This is an internal source preparation stage. It does not establish real object
creation, activation, rendering, gameplay, saves or an agreed shared ABI.

`FixedDeclarationsV1` owns a retained map/source borrow and a source-occurrence
index. Every level root child and each selected module gameplay/visual root
child remains present, including types/names missing from the source. Repeated
module references get distinct occurrences while keeping authored names. Raw
XML and complete element/attribute trees remain retained. No template defaults,
condition decisions, campaign IDs or factory handles are invented.

Independent original XML comparisons pass for 2,331 occurrences across all 16
assembled fixed definitions, both optimized and ASan/UBSan builds with leak
detection. The 35 original source blockers remain explicit. The probe checks
failed publication, separate visit identity, retained owners after parser/map/
ZIP-owner teardown, and declarations after another preparation. Standalone
ARM64 and x86_64 builds pass; declaration execution on Android is not verified.

`reports/swamp-fixed-declarations.json` contains the complete retained Swamp
index (50 Character declarations and five OpenableContainer declarations).
Those include conditional/script actors and are not an active population.
`reports/fixed-declarations-host.json` and
`reports/fixed-declarations-sanitizers.json` bind native probes, original XML,
cache and source hashes. `reports/declarations-checkpoint.json` captures the
new milestone. Older map/preview receipts describe their recorded source and
APK; they must not be represented as current declaration binary checks.

Original `ObjectManager::LoadFromXML` at `0x34b868` reads gametype/name, sets
the separate XML `template` before default properties, applies registered XML
overrides, calls `InitPost` immediately only for `LevelConfig`, then checks
`IsGameObject()` before adding the current module translation to game objects.
`_templateName` is a registered property; it is retained separately from
`template`. Captures are in `reference/property-functions` and
`reference/loader-functions`. The checked authored position plus offset is a
projection, not the final position after complete initialization. The earlier
`InitPre` label was incorrect; later evidence in
`reports/original-loader-vtables.json` resolves the slots directly. Class defaults,
table-backed resources and runtime execution still belong to main services.

Preparation groups records by source/module occurrence for inspection. It does
not claim original initialization/publication order. Missing transform fields
remain absent; malformed/nonfinite tuples fail explicitly in this checked
projection domain. Multiple matching roots and unresolved conditional/alternate
module selection remain unsupported. Text nodes are retained through raw XML
but are not covered by the element/attribute comparison.

Reproduce from this isolated checkout in WSL:

```
cmake --build ../build/host-xml -j 2
python3 port/level-loader/tests/fixed_declarations_host.py --cache <canonical-cache.zip> --probe ../build/host-xml/dh2_loader_fixed_declarations_probe --map-coverage port/level-loader/reports/fixed-map-level-coverage.json --out port/level-loader/reports/fixed-declarations-host.json --swamp-out port/level-loader/reports/swamp-fixed-declarations.json
cmake --build ../build/host-sanitizers -j 2
env ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 python3 port/level-loader/tests/fixed_declarations_host.py --cache <canonical-cache.zip> --probe ../build/host-sanitizers/dh2_loader_fixed_declarations_probe --map-coverage port/level-loader/reports/fixed-map-level-coverage.json --out port/level-loader/reports/fixed-declarations-sanitizers.json
python3 port/level-loader/tools/capture_declarations_checkpoint.py
```

Review the two production declaration files and their CMake addition against
the isolation baseline. Do not stage inherited changes or private preview app
settings. The shared factory/event/condition/restore contract is still proposed
in `INTERFACE-PROPOSAL.md`; no shared integration or merge is authorized here.
