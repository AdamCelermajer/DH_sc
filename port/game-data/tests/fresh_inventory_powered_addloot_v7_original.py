"""Capture one real AddLoot powered fixed-entry result from the original ARM ELF.

The selected LootTable row is an explicit host seam fixture, not a claimed
Crypt drop. Text, debug and storage imports retain the original bounded source
harness providers; this does not claim complete AddLoot gameplay parity.
"""
import argparse
import hashlib
import json
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "port/game-data/tests"))
from fresh_inventory_v2_original import Fresh, W
from unicorn import UC_HOOK_CODE


class ControlledAddPowerFresh(Fresh):
    """Run the native loot loop with a bounded 32-byte AddPower provider.

    ItemInstance::AddPower has table, property, allocation and StringManager
    dependencies this harness does not own. This provider only supplies the
    native vector stride and ID field; the remaining record bytes are fixtures.
    """
    def __init__(self):
        super().__init__()
        # Fresh's legacy harness stopped after LootTable. Random Loot rows also
        # require the intervening MerchantTable and original NumProbArray data.
        self.invoke(0x4b9d40, [self.stream], budget=10_000_000)
        self.invoke(0x4b9bf4, [self.stream], budget=10_000_000)
        if self.cursor != 284104:
            raise RuntimeError(f"original Merchant/NumProb readers ended at {self.cursor}")

    def inventory_service(self, uc, address, size, unused):
        if self.capture and address == 0x3fbc60:
            item, power = self.reg(0), self.reg(1)
            begin, end = self.word(item + 0x5c), self.word(item + 0x60)
            if not begin:
                begin = self.allocate(256)
                self.pointer(item + 0x5c, begin)
                end = begin
            if (end - begin) % 32:
                raise RuntimeError("source AddPower vector is not PowerInfo-aligned")
            record = W(power) + bytes(28)
            uc.mem_write(end, record)
            self.pointer(item + 0x60, end + len(record))
            self.emit(address, self.uc.reg_read(self.lr) - 4, item, power,
                      self.reg(2))
            self.returned()
            return
        super().inventory_service(uc, address, size, unused)

    def external(self, uc, address, size, unused):
        name = self.imports.get(address)
        if name and name.startswith("pthread_mutex"):
            # The emulated cache readers run single-threaded; the lock object
            # is a controlled synchronization boundary in this fixture.
            self.put(0, 0)
            uc.reg_write(self.pc, uc.reg_read(self.lr))
            return
        super().external(uc, address, size, unused)


def snapshot_by_get_num_powers(source):
    """Normalize raw 4-byte serialization to native 32-byte power semantics.

    The shared harness snapshot exposes the backing bytes as words. This
    fixture compares native GetNumPowers/GetPowerId only and omits the
    controlled remainder of each 32-byte PowerInfo record.
    """
    raw = source.snapshot_inventory()
    count = struct.unpack_from("<I", raw, 8)[0]
    out = bytearray(raw[:12])
    at = 12
    slots = source.word(source.inv + 8)
    for index in range(count):
        slot = source.word(slots + 4 * index)
        item = source.word(slot)
        out.extend(raw[at:at + 24])  # six inventory/item fields
        at += 24
        raw_word_count = struct.unpack_from("<I", raw, at)[0]
        at += 4
        begin, end = source.word(item + 0x5c), source.word(item + 0x60)
        if end < begin or (end - begin) % 32 or raw_word_count != (end - begin) // 4:
            raise RuntimeError("source PowerInfo vector and raw snapshot differ")
        power_count = source.invoke(0x3f9e80, [item])
        out.extend(W(power_count))
        ids = [source.invoke(0x3fa038, [item, power_index])
               for power_index in range(power_count)]
        if ids:
            out.extend(W(*ids))
        at += raw_word_count * 4
    out.extend(raw[at:])  # potion and both equipment sets
    return bytes(out)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, required=True,
                        help="canonical cache data/pydata directory")
    parser.add_argument("--loot-id", type=int, default=5,
                        help="explicit LootTable row used by the source fixture")
    parser.add_argument("--seed", type=int, default=1,
                        help="initial source RNG seed used by the fixture")
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()

    # Row 5 is the fixed-entry baseline; row 6 is a separate bounded random
    # entry case. Both IDs are explicit cache-backed source fixtures.
    loot_id, seed, rng_calls, capacity = args.loot_id, args.seed, 0, 12
    value_bonus, power_bonus, requested, difficulty = 0, 0, 1, 0
    source = ControlledAddPowerFresh()
    powers = args.cache / "item_powers_pyarray.bin"
    source.blob = powers.read_bytes()
    source.cursor = 0
    for address in (0x4bacc8, 0x4bab7c):
        source.invoke(address, [source.stream], budget=30_000_000)
    if source.cursor != len(source.blob):
        raise RuntimeError("original ItemPower readers did not consume cache")

    source.fresh_inventory(capacity)
    source.pointer(source.inv + 4, 0)
    source.loading = True
    source.capture = True
    source.requests = []
    source.mutation = 0
    source.minimal = False
    source.uc.mem_write(source.seed, W(seed))
    source.uc.mem_write(source.rngcalls, W(rng_calls))
    # AddLoot's fifth and sixth parameters are stack arguments; the harness
    # exposes them through invoke(stack=...), after preparing its call frame.
    order, selection_trace = [], []
    ordered_calls = {0x403310, 0x4020b4, 0x3ff5d4}
    selection_calls = {0x401b90, 0x402a60, 0x402bdc, 0x402d70, 0x4039a0}
    def observe_source(uc, address, size, user):
        if address in ordered_calls:
            order.append(address)
        elif address in selection_calls:
            row = {"address": hex(address)}
            if address == 0x402bdc:
                pointer = source.reg(0)
                row["entry_words"] = list(struct.unpack(
                    "<9I", bytes(uc.mem_read(pointer, 36))))
            selection_trace.append(row)
    source.uc.hook_add(UC_HOOK_CODE, observe_source)
    source.invoke(0x40407c,
                  [source.inv, loot_id, value_bonus, power_bonus],
                  stack=(requested, 0),
                  budget=50_000_000)
    source.capture = False
    snapshot = snapshot_by_get_num_powers(source)
    add_item_prefix = [0x403310, 0x4020b4, 0x3ff5d4]
    if (not order or len(order) % len(add_item_prefix) or
            any(order[at:at + len(add_item_prefix)] != add_item_prefix
                for at in range(0, len(order), len(add_item_prefix)))):
        raise RuntimeError(f"original AddLoot item-prefix call order changed: {order!r}")
    source_inventory_items = struct.unpack_from("<I", snapshot, 8)[0]

    payload = (b"ALV7" + W(loot_id, seed, rng_calls, capacity, value_bonus,
                             power_bonus, requested, difficulty,
                             source.word(source.seed), source.word(source.rngcalls),
                             len(snapshot)) + snapshot)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(payload)
    print(json.dumps({
        "validation": "PASS",
        "original_library_sha256": hashlib.sha256(
            (ROOT / ".local-inputs/libDungeonHunter2.so").read_bytes()).hexdigest(),
        "item_power_cache_sha256": hashlib.sha256(powers.read_bytes()).hexdigest(),
        "source_calls": [hex(x) for x in order],
        "selection_calls": selection_trace,
        "source_inventory_items": source_inventory_items,
        "source_power_count": struct.unpack_from("<I", snapshot, 36)[0] if source_inventory_items else 0,
        "source_rng_seed_after": source.word(source.seed),
        "source_rng_calls_after": source.word(source.rngcalls),
        "fixture_sha256": hashlib.sha256(payload).hexdigest(),
        "fixture": str(args.output),
        "AddPower_body_executed": False,
        "AddPower_payload": "controlled 32-byte record; source ID at offset zero",
        "rng_scope": "native AddLoot loop with controlled AddPower provider; no full AddPower parity claim",
        "scope": "original powered fixed-entry AddLoot output; GetNumPowers-normalized snapshot",
    }))


if __name__ == "__main__":
    main()
