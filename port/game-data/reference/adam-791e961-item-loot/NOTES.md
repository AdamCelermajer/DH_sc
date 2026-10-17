# Item presentation and powered-loot reuse

Imported from Adam Celermajer's `DH_sc` commit
`791e961b12233100b303038c961666834f4beb9d`. `import-manifest.json` records exact
upstream paths, bytes and hashes. The source modules are his reconstruction;
the compatibility changes and selected-library wrapper are continuation work.
The Adam audit checkout remains pinned at c3; files came from pinned Git blobs.

`ItemPresentationOwnerV5` borrows the existing immutable power authority and
keys presentation records to actual V4 item identities. Its header uses our
existing `ItemInstanceV1` rather than importing another inventory owner.
Call `forget` before item destruction. Required text failures preserve source
clear/append/value prefixes; sorting preserves positional SortingOrder words.

`LootPowerCreationV7` now borrows `InventoryRandomServiceV4`, the same stream0
descriptor accepted by our V4 inventory. Selection/quantity/value arithmetic is
shared with the retained C fixtures; there is no second RNG state or formula.
The compatibility constructor borrows a supplied fixture RNG without copying
its seed/counter. Required RNG failures retain already-performed draws.

The central game-data CMake now selects both files. The scoped wrapper detects
that selection and adds neither twice. The selected-library host gate was rerun
after selection; both Android ABIs compile them. Native loot binding remains open.
The test replays original-derived presentation, power-instance and loot gold,
then creates/powers/values/stores actual items in V4 using its caller-owned live
PropertyState and RNG. Text/debug remain explicit fixtures. Actual localization,
native gear binding, drops/pickups/subloot/merchant callers and gameplay are open.

The copied binary fixtures contain controlled comparative inputs/results. No
original cache/APK/library payload is copied. The canonical cache is a runtime
test input supplied separately. Original-code address/hash evidence for the
loot functions is retained in `loot-power-creation-v7/original-functions.json`;
the presentation evidence already retained under player-inventory-owned-v4 is
the same source caller range used by this import.
