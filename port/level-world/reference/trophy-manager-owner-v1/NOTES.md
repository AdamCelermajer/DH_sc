# Original trophy owner

The source is `.local-inputs/libDungeonHunter2.so`, SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.asm/json` records the original manager constructor,
initialization, query, unlock callback, four-word bitmap load/save and cache
readers. The 69 authored rows come from the original cache entries
`pydata/trophies_pyarray.bin`, `trophies_pyarraynames.bin` and
`trophies_pystructnames.bin`; private extraction receipts retain source paths
and hashes. No achievement rows or names are generated.

The original-instruction constructor probe executes InitTrophies and the real
vector machinery. Allocation, debug observers and temporary CString helpers
are explicit probe fixtures. Its 621 data fields form the binary source oracle;
host checks compare all fields plus query and prefix behavior against this
oracle. These fixtures do not establish real application, text, UI or storage
provider availability.

`TrophyManagerOwnerV1` owns actual catalog-derived data and pending IDs. A single
`TrophyNativeBindingsV1` borrows that manager for SkillAI operation8 capture,
operation9 catalog, operation10 unlock, and HitFor's trophy services. Catalog
names are retained by the owner; no substitute manager identity is returned.

Unlock writes the original unlocked byte before resolving text, querying the
application, delivering the complete achievement message continuation and
saving the original 16-byte bitmap. Missing required providers fail after that
real prefix. Message delivery must include the source first-message behavior;
merely recording a message is not a complete provider. The source callback
erases the pending ID from a copied vector; the owned original vector remains.

Invalid positive indices and catalogs above the original 128-bit save domain
are rejected explicitly. Full original assertion/UI delivery outside the valid
index domain is not claimed. Trophy storage, TextManager, application state and
achievement UI remain borrowed integration services.
