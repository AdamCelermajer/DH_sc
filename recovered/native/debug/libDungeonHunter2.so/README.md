# Native DWARF recovery evidence

This is a reproducible export of debug metadata from the supplied native library. It is not original source code. No function bodies are fabricated or claimed recovered.

`dies.jsonl.gz` preserves every DIE (including null terminators), parent, DWARF form, attribute offset, decoded value, raw value and reference. Byte strings retain exact hex plus a convenience UTF-8 text view. For a byte-for-byte section copy, run the recovery tool with `--raw-sections`.

`line-program.jsonl.gz` preserves line-program commands and states; `compilation-units.json` maps their file indices. `types.jsonl.gz` and `functions.jsonl.gz` are derived indexes. `locations.jsonl.gz` and `ranges.jsonl.gz` decode referenced lists while retaining their raw operands. `function-address-index.json` is a compact map for disassembler address matching. All addresses are original ELF virtual addresses.

`declarations/*.cpp.txt` are readable C++ declaration sketches for review, not compilation-ready code. Their limitations are noted at the top of each file. For exact semantics, consult the associated DIE.

The original debug units in this APK identify license/online glue, STLport and libgcc. A large debug section therefore does not mean gameplay implementation was recovered. See `summary.json` for counts and `parse-errors.json` for any parse limitations.
