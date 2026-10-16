# Progress — slice `net-core` (categories: net-session-online, core-platform)

Auditor: delegated subagent (deepseek-harness). Read-only audit of the track-b worktree;
only `reports/decomp-reconstruction/deepseek-audit/_parts/` was written.

Worklist: `dh2-deepseek-audit/index/queue_net-core.tsv` — 1640 rows
(prio0 42, prio1 509 of which 335 carry a `const:`/`float:` lead, prio2 16, prio3 81, prio4 992).

## Method

1. `_scratch_netcore/mkbatch.py` renders side-by-side batches (original body + reconstruction body,
   both with real file line numbers, cap 700 lines/function, ~110 KB per batch) from the queue.
2. One auditor subagent per batch compares every row, checks the ledger notes for the address, and
   returns structured findings with exact quoted snippets plus the `chunk_XXX.c:LINE` citation
   (a `workflow` fan-out, 20 batches).
3. `_scratch_netcore/verify.py` re-checks every returned finding mechanically against the files on
   disk (reconstruction line exists and matches the quoted snippet; cited original line exists and
   matches the quoted original snippet; address/source_file agree with the worklist). Findings that
   fail a check are dropped or corrected by hand, never emitted blindly.
4. The 3 findings of the pilot batches (prio0/prio2) were additionally re-read by hand.

## Coverage

| prio | rows | planned | audited |
|------|------|---------|---------|
| 0    | 42   | all     | 42 (pilot, incl. hand re-verification of the cluster around 0x82b9fc / 0x42b150 / 0x7fdf24) |
| 1 (const:/float: lead) | 335 | all | 335 in 17 batches (net-session-online 236 first, then core-platform 99) |
| 2    | 16   | all     | 16 |
| 3    | 81   | as far as time allows | see below |
| 4    | 992  | not reached | 0 |

## Findings

Emitted so far (see `findings-net-core.csv`): 2 high/medium rows —
`0x42b150 fs_read_func` (missing cursor advance + return 0) and
`0x7fdf24 CEvent::CopyData` (always returns -1 instead of the copied size).

## Deliberately not reported (checked and dismissed)

- `0x82b9fc`…`0x82bc48` GLXPlayerUserFriend getters: the `const:31` leads are the `+0x3c` count
  offset; index bounds (`>count` inclusive for all but `GetName`, which is `< count`) and the
  per-getter defaults (0x0/0/-1/-1.0) match chunk_055.c:3416-3582 one by one. The extra
  `index >= field.size()` guard only replaces the original's out-of-range read, and the
  raw-`char*`-array → `std::vector<std::string>` change is an allowed representation change
  (documented in `glx_player_user_friend.hpp`); `GetAvatarKey`'s span also covers the two
  `*ForTest` seams, which are additions by design.
- Harness-class ARMDIFF leads (`A-bool`, `A-sret-abi`, `A-fault-order`, `A-garbage`, `A-oob-index`,
  `B-representation`, `B-benign-init`, `C-stl`) are emulation/layout artefacts, not logic
  differences, unless a body-level difference backs them up.

## Addresses not matched / unreadable

None so far: every row of prio 0-2 had both spans present in the files (rows whose `recon_span`
was `None` in the queue were re-located by the `// 0x<addr>` marker in the reconstruction file).
