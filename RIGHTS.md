# Provenance and rights

This is an independent reconstruction of Dungeon Hunter 2 from owner-supplied APK/cache material. It is not a recovered Gameloft development repository or an official release. The game itself has not been relicensed as open source.

Recovered Java, smali, pseudocode, assembly, debugging records, XML configurations, shaders, and binary-derived constants retain the provenance and rights of the supplied materials. No repository-wide open-source license is asserted for these recovered materials. Individual third-party notices, if present in an input, remain relevant. Assets excluded from this repository can be regenerated from the owner's inputs with the supplied tooling.

The Python and Java recovery scripts were authored for this project. Their purpose is to export evidence and make recovery reproducible. No studio source-code access was used. The reconstructed JNI component contains independently written implementation code and binary-derived tables; it is marked separately from exact recovered bytes.

## Third-party reconstruction contributions

Selected native reconstruction research and source contributed in the public `AdamCelermajer/DH_sc` repository is included with its origin and pinned revision recorded in `port/ADAM-CORE-SOURCE-IMPORT.md`. Its source is not represented as official studio code. Attribution to Adam is retained there. Third-party notices for vendored dependencies remain with their modules.

The exact game Lua scripts under `recovered/scripts/original/` retain their
cache provenance and original rights status. The separately downloaded Lua
5.1.4 interpreter under `port/lua-runtime/vendor/` is upstream third-party
source distributed under its retained MIT license in `lua-5.1.4/COPYRIGHT`.
Its archive and per-file identities are recorded in that module's manifest.
That interpreter license does not apply to the recovered game scripts/assets.
