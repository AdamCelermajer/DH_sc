"""Apply evidence-backed pointer types to the original ARM32 IDB.

Run in IDAPython. This changes analysis types only, never ELF bytes.
The prefix describes only the proven PlayerInfo Character field, not its size.
"""
import hashlib
import json

import ida_hexrays
import ida_ida
import ida_name
import ida_nalt
import ida_typeinf


ELF_SHA256 = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
PREFIX = "DH2_PlayerInfoCharacterPrefix"
FUNCTIONS = (
    ("_ZN13PlayerManager14GetLocalPlayerEib", 0x36E478,
     PREFIX + " *__fastcall recovered_local_player(PlayerManager *, int, bool);"),
    ("_ZN13PlayerManager23GetLocalPlayerCharacterEv", 0x330794,
     "Character *__fastcall recovered_local_character(PlayerManager *);"),
)


def apply_types():
    with open(ida_nalt.get_input_file_path(), "rb") as stream:
        digest = hashlib.file_digest(stream, "sha256").hexdigest()
    if digest != ELF_SHA256 or ida_ida.inf_is_64bit():
        raise RuntimeError("Expected the reviewed original ARM32 ELF")

    previous = []
    for symbol, expected, declaration in FUNCTIONS:
        address = ida_name.get_name_ea(0, symbol)
        if address != expected:
            raise RuntimeError("Source symbol/address mismatch: " + symbol)
        original = ida_typeinf.tinfo_t()
        if not ida_nalt.get_tinfo(original, address):
            raise RuntimeError("Cannot preserve the previous function type")
        previous.append((address, original, declaration))

    # GetLocalPlayerCharacter (0x330794) returns exactly
    # *(GetLocalPlayer(this, 0, true) + 0x660). HitFor (0x3a8f30-54)
    # null-checks this Character and invokes virtual +0x34, IsDead.
    # A misleading inferred VoxSoundManager type falsely makes damage appear
    # dependent on audio readiness. Keep this prefix distinct from full ABI types.
    prefix = ida_typeinf.tinfo_t()
    if not prefix.get_named_type(None, PREFIX):
        declaration = ("struct " + PREFIX + " { unsigned char unknown_000[0x660]; "
                       "Character *character_660; };")
        if ida_typeinf.parse_decls(None, declaration, None, 0) != 0:
            raise RuntimeError("Cannot define the proven PlayerInfo prefix")

    changed = []
    local_backup = None
    try:
        for address, original, declaration in previous:
            recovered = ida_typeinf.tinfo_t()
            if not ida_typeinf.parse_decl(recovered, None, declaration, 0):
                raise RuntimeError("Cannot parse recovered function type")
            if not ida_typeinf.apply_tinfo(address, recovered,
                                          ida_typeinf.TINFO_DEFINITE):
                raise RuntimeError("Cannot apply recovered function type")
            changed.append((address, original))
        # The optimizer reuses this stack slot later for the real Vox manager.
        # Pin only the earlier Character lifetime, not the later sound pointer.
        ida_hexrays.mark_cfunc_dirty(0x3A8BC4)
        function = ida_hexrays.decompile(0x3A8BC4)
        for variable in function.lvars:
            if variable.defea != 0x3A8C69:
                continue
            local = ida_hexrays.lvar_saved_info_t()
            local.ll = ida_hexrays.lvar_locator_t(variable.location, variable.defea)
            local.type = variable.type()
            local_backup = local
            character = ida_typeinf.tinfo_t()
            pointer = ida_typeinf.tinfo_t()
            if not character.get_named_type(None, "Character") or not pointer.create_ptr(character):
                raise RuntimeError("Cannot resolve the existing Character pointer type")
            corrected = ida_hexrays.lvar_saved_info_t()
            corrected.ll = local.ll
            corrected.type = pointer
            if not ida_hexrays.modify_user_lvar_info(0x3A8BC4, ida_hexrays.MLI_TYPE, corrected):
                raise RuntimeError("Cannot correct the HitFor Character local")
            break
        else:
            raise RuntimeError("The reviewed HitFor local lifetime was not found")
        for address in (0x36E478, 0x330794, 0x3A8BC4):
            ida_hexrays.mark_cfunc_dirty(address)
    except Exception:
        if local_backup is not None:
            ida_hexrays.modify_user_lvar_info(0x3A8BC4, ida_hexrays.MLI_TYPE, local_backup)
        for address, original in reversed(changed):
            ida_typeinf.apply_tinfo(address, original, ida_typeinf.TINFO_DEFINITE)
        raise
    return {"sha256": digest, "previous_types": [
        {"address": hex(address), "type": str(original)}
        for address, original, _ in previous], "applied": len(changed)}


if __name__ == "__main__":
    print(json.dumps(apply_types(), sort_keys=True))
