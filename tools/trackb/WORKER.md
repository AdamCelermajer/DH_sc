# Track B long-running worker protocol

A worker is one subagent that takes a long list of batches (often 20 to 40) and works through all of
them, one after another, in a single session. Read this file, `port/decomp-reconstruction/TRACK-B.md`
(rules and decisions) and `port/decomp-reconstruction/README.md` ("Rules for reconstructed code") once at
the start. Do not run git: the integrator commits.

## Inputs

- A wave name `<wave>`. Batches: `reports/decomp-reconstruction/trackb-batches/<wave>.json`.
- Your batch ids, a list from that file. Show one with `py -3 tools/trackb/show_batch.py <wave> <id>`
  (the function list, then the Hex-Rays pseudocode).
- A mode: `new` (no body yet), `redo` (the functions are `partial`; complete them in place), or `platform`
  (skipped Android/JNI glue now in scope).

## Progress log (resumable)

Keep `reports/decomp-reconstruction/trackb-progress/<wave>-<worker>.md`. Append one line per finished
batch: `<id> | reconstructed N | partial N | skipped N | upstream N | compiles yes/no | note`. At the start,
read it if it exists and skip the batches already listed. A later worker can then pick up where you stopped.

## Per batch

1. Read every function in the batch. Find where its class is declared: game classes in
   `game/include/dh2/<Class>.hpp`; engine classes in their category's headers (grep). In `redo` mode, grep
   the category's `src/` for each 0x address to find the existing body, and the category's `ledger.csv` for
   the note that says why it is partial.
2. Write or complete the bodies. In `new` mode, write to the new file `<category>/src/<batch-id>.cpp`. In
   `redo` mode, complete the body in place, and never define a function twice. Each function starts with
   `// 0x<addr> <demangled symbol>`. Follow the Hex-Rays control flow and side effects exactly. Keep the 32-bit
   offsets as `// +0xNN` comments, use real types and names, and forward thunks to their targets.
3. Symbols: the function must reproduce the original mangled symbol: namespaces, name, parameter types and
   const-ness (TRACK-B decision 10).
4. Headers: add missing declarations or members with small Edit insertions only. Never rewrite, reorder or
   rename members of a header (other workers use it at the same time). Do not make local stand-in structs for
   other classes. If a member you need is missing from another class's header and the binary proves it, add
   it there with a small insertion and a `// +0xNN` comment and evidence. If the evidence is unclear, keep the
   function partial and say what is missing.
5. No guessing: anything unknown keeps the function `partial`, and the note names what is missing.
6. Compile: `bash tools/trackb/check_tu.sh <your .cpp> <each header you changed>`. Iterate until OK.
7. Self-check: for the two largest functions you marked reconstructed, compare your body with the assembly
   (`py -3 tools/decomp_asm.py --address 0x...`): conditions, constants, offsets, callees, argument order and
   return value. Fix any difference. IDA (ida-pro-mcp tools, loaded with ToolSearch) is read-only for you.
8. Test where the logic can run on its own: `<category>/tests/<batch-id>_test.cpp` returns non-zero on
   failure. Run it with `bash tools/trackb/check_tu.sh --test <test> <sources>`.
9. Ledger: `<category>/ledger.d/<batch-id>.csv` with the header `elf_address,symbol,status,source_file,notes`
   and one row per batch function. Quote any field that contains a comma. source_file is relative to
   `port/decomp-reconstruction`.
10. Append the line to your progress log.

## When you finish (or have to stop)

Report: batches done, counts per status, batches that do not compile (and why), and the layout requests
you could not apply (class, offset, type, evidence). Keep the report short. The details are in the ledger
parts and the progress log.
