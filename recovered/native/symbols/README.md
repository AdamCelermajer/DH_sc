# Native binary evidence

These inventories are recovered from the supplied ARM32 ELF libraries. They retain original addresses, symbols and byte encodings. They do not claim to be original C++ source, an ARM64 port or a validated running game.

Each library folder contains:

| File | Evidence |
| --- | --- |
| `symbols.csv`, `symbols-NNN.json` | Every ELF symbol-table row, including `.dynsym` / `.symtab` duplicates, mangled and demangled names, bindings, sections, sizes and ARM/Thumb mode bits. |
| `function-index.csv`, `function-index-NNN.json` | One assembly body per distinct function range, with every named alias and table reference. |
| `mapping-symbols.json` | Original ARM, Thumb and literal-pool transitions where preserved. |
| `imports.json`, `dynamic.json` | Undefined symbols and ELF dynamic-linking metadata. |
| `relocations.csv`, `relocations-NNN.json` | Every relocation, symbol reference and raw implicit/explicit addend. |
| `vtables-NNN.json`, `typeinfo.json` | C++ vtable words, symbols, typeinfo and relocations. Raw words may require runtime relocation. |
| `strings.csv`, `strings-NNN.json` | Printable ASCII runs of at least four bytes and ASCII UTF-16LE runs of at least four characters, scanned across all file-backed sections. This covers the stated formats, not every possible encoding. |
| `build-source-filenames.json` | Original ELF `STT_FILE` filenames. No original file contents are implied. |
| `arm-unwind-index.json` | Original `.ARM.exidx` function-address and unwind metadata, including unnamed addresses. |
| `class-groups.json`, `assembly-groups.json` | Demangled C++ class/scope group names and safe, hashed assembly filenames. |
| `gameplay-symbol-evidence.json` | Selected gameplay-related symbol names, also present in the complete symbol inventory. |
| `coverage.json`, `unresolved-ranges-NNN.json` | Exact byte accounting: decoded instructions, mapping-declared data, decoder failures and bytes outside named functions. |
| `summary.json` | Per-library counts, hashes, coverage and chunk indexes. |

Assembly is in `../assembly/<library>/`. Function files contain addresses and hex bytes alongside ARM/Thumb decoding. Original mapping symbols distinguish literal pools where available. All remaining executable-section bytes are preserved in `unattributed-executable-bytes.asm` without assigning an unproven instruction mode or function name. These are analysis listings and are not directly assembler-ready.

The game engine preserves a full `.symtab`, so its recovery is substantially better than the stripped support libraries. Capstone accepting an instruction encoding does not prove that bytes are executable instructions; support-library literal pools may decode as instructions when ARM mapping symbols are absent. Zero-size symbols use an explicitly marked inferred boundary.

The reproducible exporter is `../../../tools/recover_native.py` and the aggregate evidence is `../../../reports/native-inventory.json`. The exporter clears and replaces only its generated per-library `symbols` and `assembly` directories on rerun.
