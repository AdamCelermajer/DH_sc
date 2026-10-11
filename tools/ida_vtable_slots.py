"""Read ARM32 vtable slots in IDA; offsets are bytes from the address point.

Call vtable_slots(0x965f38, [0x28, 0x34, 0x54, 0xdc, 0xf4]) via IDAPython.
The address point excludes the Itanium offset-to-top and RTTI entries.
"""
import ida_bytes
import ida_ida
import ida_name


def vtable_slots(address_point, byte_offsets):
    if ida_ida.inf_is_64bit() or ida_ida.inf_get_procname().lower() != "arm":
        raise ValueError("This lookup is for the original ARM32 game")
    rows = []
    for offset in byte_offsets:
        if not isinstance(offset, int) or offset < 0 or offset % 4:
            raise ValueError("Use nonnegative, four-byte-aligned byte offsets")
        slot = address_point + offset
        if not all(ida_bytes.is_loaded(slot + i) for i in range(4)):
            raise ValueError(f"Unreadable vtable slot {slot:#x}")
        pointer = ida_bytes.get_dword(slot)
        target = pointer & ~1  # ARM interworking bit is not part of the address.
        rows.append({
            "byte_offset_hex": hex(offset), "byte_offset_decimal": offset,
            "slot": hex(slot), "pointer": hex(pointer),
            "target": hex(target), "symbol": ida_name.get_name(target),
        })
    return rows
