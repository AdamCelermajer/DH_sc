"""Original ARM32 loot-entry helpers versus selected ARM64 game-data code.

The original helper bodies and Random clone execute. Debug-switch values and
PlayerManager class counts are controlled caller facts; only their service
boundaries are hooked. No full AddLoot loop or gameplay claim is made.
"""
import argparse
import hashlib
import json
import random
import struct
import sys
from pathlib import Path

from unicorn import UC_HOOK_CODE

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "port/game-data/tests"))
from combat_differential import Cpu as OriginalCpu, i32
from aggro_differential import Cpu as Arm64Cpu

W = lambda *items: struct.pack("<" + "I" * len(items), *(item & 0xffffffff for item in items))
I = lambda *items: struct.pack("<" + "i" * len(items), *items)
word = lambda cpu, address: struct.unpack("<I", cpu.uc.mem_read(address, 4))[0]
HELPERS = {
    "_ZN13ItemInventory20_IsLootEntryUsingPctERKN7Structs9LootEntryE": (0x4027a4, 176),
    "_ZN13ItemInventory17_GetEffectiveProbERKN7Structs9LootEntryE": (0x402854, 124),
    "_ZN13ItemInventory19_GetRandomLootEntryERKSt6vectorIPKN7Structs9LootEntryESaIS4_EE": (0x4028d0, 400),
    "_ZN13ItemInventory19_GetRandomLootEntryERKN7Structs4LootE": (0x402a60, 380),
    "_ZN13ItemInventory10_DoPctRollERKN7Structs9LootEntryE": (0x402bdc, 264),
    "_ZN6Random9GetRandomEib.clone.1": (0x401afc, 148),
}


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


class Original(OriginalCpu):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.heap = self.data + 0x1000000
        self.infinite_drops = False
        self.class_counts = (0, 0, 0)
        self.debug_queries = 0

    def external(self, uc, address, size, unused):
        name = self.imports.get(address)
        if name == "strlen":
            pointer = self.reg(0)
            length = 0
            while uc.mem_read(pointer + length, 1)[0]:
                length += 1
            self.put(0, length)
        elif name in ("pthread_mutex_lock", "pthread_mutex_unlock"):
            # DebugSwitches' lock is a controlled service boundary in this
            # single-threaded oracle run.
            self.put(0, 0)
        elif name in ("_Znwj", "_Znaj", "malloc"):
            size = self.reg(0)
            pointer = self.heap
            self.heap += (max(size, 1) + 7) & ~7
            uc.mem_write(pointer, bytes(size))
            self.put(0, pointer)
        elif name in ("_ZdlPv", "_ZdaPv", "free"):
            # Temporary std::string storage is bounded and released at the
            # end of each helper; retaining these tiny test allocations keeps
            # the emulated allocator isolated from engine state.
            self.put(0, 0)
        else:
            return super().external(uc, address, size, unused)
        uc.reg_write(self.pc, uc.reg_read(self.lr))

    def word(self, address):
        return struct.unpack("<I", self.uc.mem_read(address, 4))[0]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--library", type=Path, required=True,
                        help="selected Android ARM64 libdh2_game_data.so")
    parser.add_argument("--original-elf", type=Path,
                        default=ROOT / ".local-inputs/libDungeonHunter2.so")
    parser.add_argument("--report", type=Path, required=True)
    args = parser.parse_args()

    manifest_path = ROOT / "port/game-data/reference/player-creation-v2/original/loot/original-functions.json"
    manifest = json.loads(manifest_path.read_text())
    if sha(args.original_elf) != manifest["original_sha256"]:
        raise AssertionError("original ARM32 ELF hash differs from the pinned loot source")
    manifest_functions = {row["original_symbol"]: row for row in manifest["functions"]}
    for symbol, (address, size) in HELPERS.items():
        row = manifest_functions.get(symbol)
        if not row or int(row["elf_address"], 16) != address or row["size"] != size:
            raise AssertionError("original helper range is absent or differs: " + symbol)

    old = Original(args.original_elf, False, manifest)
    new = Arm64Cpu(args.library, True, {"functions": []})
    base = 0x401b10 + old.word(0x401b84)
    seed = old.word(base + old.word(0x401b88))
    calls = old.word(base + old.word(0x401b8c))
    if not seed or not calls:
        raise AssertionError("original Random clone state addresses are unresolved")

    old_entries = old.data + 0x1000
    old_ptrs = old.data + 0x5000
    old_vector = old.data + 0x6000
    old_loot = old.data + 0x6100
    new_entries = new.data + 0x1000
    new_counts = new.data + 0x5000
    new_rng = new.data + 0x5100
    new_out = new.data + 0x5200
    new_pct = new.data + 0x5300
    totals = {"percentage": 0, "effective_probability": 0, "percent_roll": 0,
              "vector_choice": 0, "loot_choice": 0}
    selected_raw_comparisons = 0
    rng_draws = 0

    class_getters = {0x36eac8: 0, 0x36eac0: 1, 0x36eab8: 2}

    def source_services(uc, address, size, unused):
        if address == 0x337888:
            old.put(0, 0)  # DebugSwitches::load: settings are injected below.
            uc.reg_write(old.pc, uc.reg_read(old.lr))
        elif address == 0x337a88:
            old.debug_queries += 1
            old.put(0, int(old.infinite_drops))
            uc.reg_write(old.pc, uc.reg_read(old.lr))
        elif address in class_getters:
            old.put(0, old.class_counts[class_getters[address]])
            uc.reg_write(old.pc, uc.reg_read(old.lr))

    old.uc.hook_add(UC_HOOK_CODE, source_services)

    def reset_old_random(initial_seed, initial_calls):
        old.uc.mem_write(seed, W(initial_seed))
        old.uc.mem_write(calls, W(initial_calls))

    def old_rng_state():
        return (old.word(seed), old.word(calls))

    def run_candidate(function, payload, initial_seed, initial_calls, counts=(0, 0, 0), infinite=False):
        new.uc.mem_write(new_entries, payload)
        new.uc.mem_write(new_counts, I(*counts))
        new.uc.mem_write(new_rng, W(initial_seed, initial_calls))
        new.uc.mem_write(new_out, W(0xA5A5A5A5))
        new.uc.mem_write(new_pct, W(0x5A5A5A5A))
        if function == "percent":
            status = new.invoke("dh2_loot_entry_is_percent_v1",
                                [new_pct, new_entries, int(infinite)])
            if status: raise AssertionError((function, status))
            result = word(new, new_pct)
        elif function == "effective":
            status = new.invoke("dh2_loot_entry_effective_probability_v1",
                                [new_out, new_entries, new_counts, int(infinite)])
            if status: raise AssertionError((function, status))
            result = struct.unpack("<i", new.uc.mem_read(new_out, 4))[0]
        elif function == "roll":
            status = new.invoke("dh2_loot_entry_do_percent_roll_v1",
                                [new_pct, new_entries, int(infinite), new_rng])
            if status: raise AssertionError((function, status))
            result = word(new, new_pct)
        elif function == "weighted":
            count = len(payload) // 32
            status = new.invoke("dh2_loot_entries_choose_weighted_v1",
                                [new_out, new_entries, count, new_counts,
                                 int(infinite), new_rng])
            if status: raise AssertionError((function, status))
            result = word(new, new_out)
        else:
            raise AssertionError(function)
        state = struct.unpack("<II", new.uc.mem_read(new_rng, 8))
        return result, state

    def native_rows(rows):
        return b"".join(I(0, *row) for row in rows)

    def payload_rows(rows):
        return b"".join(I(*row) for row in rows)

    # The source payload names/order are pinned by loot_table_pystructnames.
    # Pct is words[4], Prob words[5], class modifiers words[2,6,7].
    pct_values = (-2147483648, -1, 0, 1, 50, 99, 100, 101, 2147483647)
    rng = random.Random(0x4027A4)
    for index, pct in enumerate(pct_values):
        row = [index + 10, 1, 2, 3, pct, 17, 4, 5]
        payload = payload_rows([row])
        source_native = old_entries
        old.uc.mem_write(source_native, native_rows([row]))
        for infinite in (False, True):
            old.infinite_drops = infinite
            expected = old.invoke(0x4027a4, [source_native])
            actual, _ = run_candidate("percent", payload, 1, 0, infinite=infinite)
            if actual != expected:
                raise AssertionError(("IsLootEntryUsingPct", pct, infinite, expected, actual))
            totals["percentage"] += 1

            old.class_counts = (2, 3, 4)
            expected = i32(old.invoke(0x402854, [source_native]))
            actual, _ = run_candidate("effective", payload, 1, 0,
                                      counts=old.class_counts, infinite=infinite)
            if actual != expected:
                raise AssertionError(("GetEffectiveProb", pct, infinite, expected, actual))
            totals["effective_probability"] += 1

    edge = (-2147483648, -1, 0, 1, 2, 65535, 2147483647)
    for index in range(64):
        row = [index, 1, rng.choice(edge), 0, 101, rng.choice(edge),
               rng.choice(edge), rng.choice(edge)]
        counts = tuple(rng.choice(edge) for _ in range(3))
        payload = payload_rows([row])
        old.uc.mem_write(old_entries, native_rows([row]))
        old.class_counts = counts
        old.infinite_drops = False
        expected = i32(old.invoke(0x402854, [old_entries]))
        actual, _ = run_candidate("effective", payload, 1, 0, counts=counts)
        if actual != expected:
            raise AssertionError(("GetEffectiveProb wrap", index, row, counts,
                                  expected, actual))
        totals["effective_probability"] += 1

    for index, pct in enumerate((-2147483648, -1, 0, 1, 50, 99, 100, 101)):
        row = [index, 1, 0, 0, pct, 20, 0, 0]
        payload = payload_rows([row])
        old.uc.mem_write(old_entries, native_rows([row]))
        for infinite in (False, True):
            initial_seed = rng.getrandbits(32)
            initial_calls = rng.getrandbits(32)
            old.infinite_drops = infinite
            reset_old_random(initial_seed, initial_calls)
            expected = old.invoke(0x402bdc, [old_entries])
            expected_state = old_rng_state()
            actual, actual_state = run_candidate("roll", payload, initial_seed,
                                                  initial_calls, infinite=infinite)
            if (actual, actual_state) != (expected, expected_state):
                raise AssertionError(("DoPctRoll", pct, infinite, expected,
                                      expected_state, actual, actual_state))
            totals["percent_roll"] += 1
            rng_draws += (expected_state[1] - initial_calls) & 0xffffffff

    weighted_sets = [
        [],
        [[1, 0, 0, 0, 101, 1, 0, 0]],
        [[1, 0, 0, 0, 101, 10, 0, 0], [2, 0, 0, 0, 100, 1000, 0, 0],
         [3, 0, 0, 0, 101, 20, 0, 0]],
    ]
    for case in range(48):
        rows = []
        for index in range(1 + case % 8):
            # Keep cumulative weights positive so the original diagnostic
            # fallback branch remains outside this helper pilot.
            rows.append([case * 16 + index, rng.randrange(0, 12),
                         rng.randrange(0, 5), rng.randrange(0, 5),
                         rng.choice((99, 100, 101, 120)), rng.randrange(1, 80),
                         rng.randrange(0, 5), rng.randrange(0, 5)])
        weighted_sets.append(rows)

    for case, rows in enumerate(weighted_sets):
        payload = payload_rows(rows)
        old.uc.mem_write(old_entries, native_rows(rows) if rows else bytes(36))
        pointer_bytes = b"".join(W(old_entries + 36 * index) for index in range(len(rows)))
        old.uc.mem_write(old_ptrs, pointer_bytes if pointer_bytes else bytes(4))
        old.uc.mem_write(old_vector, W(old_ptrs, old_ptrs + len(rows) * 4,
                                       old_ptrs + len(rows) * 4))
        old.uc.mem_write(old_loot, bytes(12) + W(len(rows), old_entries))
        counts = (case % 4, (case // 2) % 4, (case // 3) % 4)
        for infinite in (False, True):
            initial_seed = rng.getrandbits(32)
            initial_calls = rng.getrandbits(32)
            old.class_counts = counts
            old.infinite_drops = infinite
            for kind, address, argument, key in (
                    ("vector", 0x4028d0, old_vector, "vector_choice"),
                    ("loot", 0x402a60, old_loot, "loot_choice")):
                reset_old_random(initial_seed, initial_calls)
                expected_index = old.invoke(address, [argument])
                expected_state = old_rng_state()
                actual_index, actual_state = run_candidate(
                    "weighted", payload, initial_seed, initial_calls,
                    counts=counts, infinite=infinite)
                if (actual_index, actual_state) != (expected_index, expected_state):
                    raise AssertionError(("GetRandomLootEntry", kind, case, infinite,
                                          expected_index, expected_state,
                                          actual_index, actual_state, rows, counts))
                source_raw = (bytes(old.uc.mem_read(old_entries + expected_index * 36 + 4, 32))
                              if rows else bytes(32))
                candidate_raw = (payload[actual_index * 32:(actual_index + 1) * 32]
                                 if rows else bytes(32))
                if source_raw != candidate_raw:
                    raise AssertionError(("selected raw entry", kind, case,
                                          source_raw.hex(), candidate_raw.hex()))
                totals[key] += 1
                selected_raw_comparisons += 1
                rng_draws += (expected_state[1] - initial_calls) & 0xffffffff

    executed_symbols = {old.address_owner[address] for address in old.seen
                        if address in old.address_owner}
    if not set(HELPERS).issubset(executed_symbols):
        raise AssertionError(("source helper body was not executed", sorted(set(HELPERS) - executed_symbols)))
    report = {
        "validation": "PASS",
        "original_elf_sha256": sha(args.original_elf),
        "pinned_source_manifest_sha256": sha(manifest_path),
        "selected_arm64_library_sha256": sha(args.library),
        "comparisons": totals,
        "total_comparisons": sum(totals.values()),
        "selected_raw_entry_comparisons": selected_raw_comparisons,
        "original_rng_draws_observed": rng_draws,
        "all_selected_rng_states_match": True,
        "original_helpers_executed": sorted(HELPERS),
        "original_helper_range_hashes": {
            symbol: manifest_functions[symbol]["sha256"] for symbol in sorted(HELPERS)},
        "controlled_original_services": {
            "DebugSwitches::load/GetSwitch": "load returns; GetSwitch returns supplied InfiniteLootDrops state",
            "PlayerManager class counts": {"Mage": "0x36eac8", "Rogue": "0x36eac0",
                                            "Warrior": "0x36eab8"},
            "temporary std::string allocation": "bounded emulated allocator; no-op release",
            "mutex": "single-threaded no-op lock/unlock",
        },
        "scope": "Original helper bodies and Random clone execute under controlled debug and class-count facts. This validates loot-entry classification, effective weights, percentage rolls and both weighted-selection overloads; no recursive AddLootItems or gameplay claim.",
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report))


if __name__ == "__main__":
    main()
