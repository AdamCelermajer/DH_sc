# Bounded loading diagnostics and private-save transport V39

Root applied the surgical transfer to the shared renderer and save provider.
This agent owns the new include/helper/tests/tools; it did not edit the shared
model renderer, constructor include or CMake. Root reports the complete default
diagnostics-OFF APK build passed for both ABI. No emulator, graphics context,
device or ADB operation occurred for this system.

## Loading feature

The exact original five all-floor-pairs and two per-floor development proof
sweeps were removed from the normal world-load path and retained in
`renderer_load_diagnostics_v39.inc`. Shipping CMake defines
`DH2_ENABLE_LOAD_DIAGNOSTICS=0` by default. QA compilation alone is insufficient:
the separate explicit `DH2_LOAD_DIAGNOSTICS_QA_REQUEST_V39` must be true. Parent
CMake adds an OFF request option and rejects requesting diagnostics when the
diagnostic feature is disabled. Actual floor/graph construction and gameplay
navigation/controller services stay in their existing source path.

Requested QA work validates64floors/4096nodes/65536edges before allocating proof
vectors, checks a maximum25,000 proof units, and checks elapsed time before
each unit. There are five F² sweeps and two F sweeps; at64floors the original
loop body has20,608 units plus bounded graph initialization. Original10,000
search expansion inputs, calls, digest bytes, counters and diagnostic log text
are preserved. Real diagnostic failures rethrow to the prior load error path.

**The1,000ms limit is a cooperative checkpoint bound, not hard preemption.** A
native search/navigation call already running can finish after the deadline.
Its work remains constrained by graph/node/edge bounds and the original search
limit, but no strict real-time duration is claimed. A budget stop logs an
incomplete proof with its named reason, clears the diagnostic route cache and
allows genuine world loading to continue. Non-sewn/absent graphs are explicitly
skipped without a proof-complete claim. Other exceptions clear the cache and
remain actual failures. No phase38, source count, Level-ready flag or controller
state is forced.

## Private save authority and successor

The recovered complete stream/profile index has a **UINT32_MAX byte domain**;
section lengths are uint32, INFO is an int32 and OBJS contains a count, two
signed-positive32 CString lengths, room32, payload64 and variable virtual object
Save bodies. A small original engine maximum is not proved. The proposed and
root-applied **16MiB byte limit is explicit application transport policy**, not
a source-format maximum or proof that every future chapter/save virtual fits.
`saved-authority.json` pins the source files and this distinction. No actual
saved chapter samples are available to establish whole-game adequacy.

`renderer_private_save_read_v39.hpp` keeps the same private directory, name and
actual retained Save owner. Missing ENOENT remains found=false; other failures
remain required errors. On POSIX a nonblocking open permits fstat to reject
FIFO/nonregular inputs without waiting. The actual descriptor size must fit
the supplied positive policy and native uint32 domain before allocation. The
reader allocates exactly that accepted size, reads in at-most8192-byte requests,
checks one extra byte and real I/O/close errors, then publishes the complete
unchanged bytes. Growth/shrink is a failure rather than successful truncation.
Failed output stays empty and descriptors close on every path.

The original cache decides invalid-primary/backup loading after a successful
transport read. A byte-budget failure does not turn into a fake miss or cause a
silent successful backup. File bytes remain unchanged; this does not certify
Level.Init, OBJS virtual implementations or saved-campaign restoration. Input
size also does not bound all duplicated profile/object allocations: aggregate
parser/object-stage accounting remains separate work.

## Verification

| Check | Result | Evidence |
| --- | --- | --- |
| Exact extracted original body after removing only7step+1graph guards | PASS5 structural checks | `extraction-test.json` |
| Actual integrated renderer, ARM64 and x86_64, diagnosticsON/request0 andrequest1 | PASS4 compiles with production include/define flags and `-Werror` | `abi-compile.json` |
| Owned reader/helper and extracted guard/test | Strict `-Wall -Wextra -Werror` syntax PASS without warning suppression | owned commands in `host-receipt.json` |
| Bounded private-save files and actual source cache/index | PASS132 UBSan checks | `host-receipt.json` |
| Actual extracted QA guard | PASS10 UBSan checks | `host-receipt.json` |
| Full default APK bothABI build | PASS reported by parent after integration | parent build receipt |
| Actual gameplay/startup duration/QA sweeps | PENDING safe runtime environment | no runtime claim |

Host tests are small source/filesystem fixtures with512MiB compiler and128MiB
runtime address-space limits, CPU/time/file bounds, and no GPU. The private-save
fixture uses a unique Linux `/tmp` directory because WSL's Windows mount does
not support POSIX FIFOs. It covers true missing/exact-size/oversized/empty files,
invalid filenames/limits, actual nonregular/FIFO refusal, explicit test-link
injection of file growth/shrink/close failure, real INFO decode, actual source
invalid-primary backup handling and cap-required failure. Injection wrappers
are fixture-only; production has no test hooks. Repeated failed reads remain
required errors. The source cache/profile dependency retains its pre-existing
one-line-style warning suppression only in the composed host link; own files
also pass independently without it.

The budget test compiles the actual class definition extracted from the include,
checks exact graph caps, permits25,000units, rejects the next before accounting,
and validates named time/iteration failures. These tests do not execute all
original navigation sweeps on a real loaded game; exact extraction plus actual
bothABI compilation is the current evidence for that retained QA body.

## Transfer and rollback

`.local-inputs/transfer_renderer_load_v39.py` defaults to dry-run, verifies the
exact frozen old proof/saved() hashes and writes reviewable patches. Root used
`--apply-load-diags --apply-save-read`. It preserves adjacent changes, is
idempotent once the corresponding marker is present, and stops if either old
block changed. CMake stays root-owned. The old exact blocks are retained here
for focused rollback/review; they must not be pasted over unrelated renderer
changes. Actual GL comparison from V38 remains refused while host RAM is below
its separate7GiB admission threshold.
