# Residue owner rules (2026-10-15)

- Read once: port/decomp-reconstruction/TRACK-B.md, TRACK-B-HANDOVER.md (section 3 corrections must not regress), tools/trackb/WORKER.md.
- No git. No subagents. Never write outside the worktree.
- Use ONLY your own build dir `build/own-<name>`; never run ninja in build/tb-int.
- Layout decisions: you own them for your categories. The binary wins (decision 9: members public;
  decision 10: original mangled symbols). Recover with `py -3 tools/trackb/field_sweep.py <Class>`,
  `py -3 tools/decomp_vtable.py <Class>`, constructors, IDA ida-pro-mcp (read-only).
- Transcribe large bodies fully. Keep a row partial only for a truly unknown table/behaviour, with the note saying exactly what.
- Before reporting: `bash tools/trackb/check_tu.sh <file>` on EVERY file you touched must print OK.
- Ledger parts: `<category>/ledger.d/<tag>-<Class>.csv`, written with Python csv.writer
  (elf_address,symbol,status,source_file,notes). Never edit ledger.csv.
- One line per class in reports/decomp-reconstruction/trackb-progress/owners-2026-10-15.md (append).
- Final report: at most 8 lines (rows done per category, rows still partial with main reasons, files changed outside your categories).
