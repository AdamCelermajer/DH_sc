# Method + exact output format (batch subagents, slice anim-ui)

You are auditing a C++ reconstruction of Dungeon Hunter 2 (Android, 2012) against the original
Hex-Rays ARM pseudocode. Read-only bug hunt. NEVER edit anything outside the two files you are
told to write (your part CSV and your note); NEVER run git; NEVER build.

## Paths

- REPO: `C:\Users\nacworkstation\Documents\Projects\Dungeon Hunter 2 Recomp\DH_sc\.worktrees\track-b`
  (`D:\...` may be mapped elsewhere; always use this absolute path)
- ORIGINAL: `<REPO>\recovered\native\hexrays-all\chunk_NNN.c` (the row says which chunk + line span)
- RECONSTRUCTION: `<REPO>\port\decomp-reconstruction\<recon_file>` (row says file + line span)
- LEDGER (check the `notes` column, grep the address): `<REPO>\port\decomp-reconstruction\<category>\ledger.csv`
- Python: `C:\Users\nacworkstation\.dsh\dsh-runtimes\dsh-primary-runtime\dependencies\python\python.exe`

Read both sides with the `read` tool (offset/limit around the given spans). Spans in the worklist
were captured when the queue was generated; if the reconstruction line numbers have drifted,
re-locate the `// 0x<addr>` comment above the function and cite the CURRENT line number.

## What is NOT a bug (never report these)

- raw offsets like `*((_DWORD *)this + 45)` in the original vs a named member in ours (headers
  document members with `// +0xNN` comments; +45 dwords = +0xB4; `a2[88]` = +0x160);
- `std::` containers / smart pointers instead of raw arrays and manual refcounts;
- ASSERT blocks (`gAssertLevel`, `fprintf("ASSERT(...)")`) omitted or simplified;
- different variable names, reordered independent statements;
- compiler-inlined helpers expanded in the original (temporaries, `sub_XXXX` inline bodies,
  Hex-Rays' synthetic NetStruct / `off_XXXX` setups, vtable-slot calls like
  `(*(void (**)(int))(*(_DWORD *)x + 68))(x)` that our named getter covers);
- register residue returned where the caller discards the result;
- Hex-Rays printing a float constant as its integer bit pattern, or printing `x >= a`
  where the listing has `!(x < a)`;
- a documented deviation in the ledger `notes` for that exact address.

## What to report (real behavioural differences)

- a branch, `case`, early return or loop missing or added;
- an inverted/changed comparison (`<` vs `<=`, `!=` vs `==`, signed vs unsigned), swapped if/else;
- a different constant, multiplier, divisor, default value, array index or bit mask;
- a call to a different function or virtual slot, wrong argument order, missing argument;
- a field read/written that is clearly a different member than the original's offset;
- a missing or extra side effect (store, counter update, notify/sound/flag);
- integer vs float math differences (truncation, rounding, double vs float literals);
- reference-counting differences (missing or doubled add_ref/release).

One finding per root cause per function. NO speculation. Honesty beats volume: reporting 5
verified findings and 200 clean functions is far better than 50 guesses. If a row is clean, do not
write a row for it. If you cannot decide, either skip it or report it as `low` and say why in the
explanation.

## Output: your part CSV

Header exactly:

```
address,function,source_file,line,severity,kind,original_snippet,reconstruction_snippet,explanation
```

- Build it with Python's `csv` module (`csv.writer`, `open(path, "w", newline="", encoding="utf-8")`).
- `address`: original hex address, lowercase, e.g. `0x3b10b4`.
- `function`: the original name from the hexrays `// ===== <addr> <name> =====` header line.
- `source_file`: repo-relative path of the RECONSTRUCTION file, forward slashes, e.g.
  `port/decomp-reconstruction/game-ui/src/g1-game-ui-....cpp`.
- `line`: the INTEGER reconstruction line number of the difference.
- `original_snippet` / `reconstruction_snippet`: up to 3 short lines each, quoted EXACTLY as in the
  file, multiple lines joined with `\n` inside the cell.
- `explanation`: one or two sentences. It MUST contain the original citation
  `recovered/native/hexrays-all/chunk_XXX.c:LINE` and say what differs and the gameplay effect.
- Write/re-rewrite the whole CSV (all rows so far) after every ~10 functions so partial work
  survives. Zero findings is a valid result: write just the header.

## Your note file

Markdown with: rows audited (list the addresses, or a count + the range), findings by severity,
rows you could not match/read (address + reason), and anything you deliberately did not report
because the ledger documents it as a deviation.

## Final message back to the parent

`rows audited: N`, `findings: N (high/medium/low)`, part CSV absolute path, note absolute path,
addresses not matched, and one line on overall reconstruction quality in your batch.
