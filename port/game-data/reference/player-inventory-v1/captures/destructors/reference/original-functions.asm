
# _ZN12ItemInstanceD0Ev
003faab0: push     {r4, lr}
003faab4: mov      r4, r0
003faab8: bl       #0x3faa64
003faabc: mov      r0, r4
003faac0: bl       #0x310440
003faac4: mov      r0, r4
003faac8: pop      {r4, pc}

# _ZN12ItemInstanceD2Ev
003faacc: ldr      r3, [pc, #0x3c]
003faad0: ldr      r2, [pc, #0x3c]
003faad4: push     {r4, lr}
003faad8: add      r3, pc, r3
003faadc: ldr      r2, [r3, r2]
003faae0: mov      r4, r0
003faae4: add      r2, r2, #8
003faae8: str      r2, [r0], #0x5c
003faaec: bl       #0x3faa00
003faaf0: add      r0, r4, #0x38
003faaf4: bl       #0x3139ac
003faaf8: add      r0, r4, #0x20
003faafc: bl       #0x3139ac
003fab00: add      r0, r4, #8
003fab04: bl       #0x3139ac
003fab08: mov      r0, r4
003fab0c: pop      {r4, pc}
003fab10: ldrheq   sb, [sb], #-0xf8
003fab14: andeq    r4, r0, r0, lsl #16

# _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003139ac: ldr      r3, [r0, #0x14]
003139b0: cmp      r3, r0
003139b4: bxeq     lr
003139b8: cmp      r3, #0
003139bc: bxeq     lr
003139c0: ldr      r1, [r0]
003139c4: rsb      r1, r3, r1
003139c8: cmp      r1, #0x80
003139cc: bhi      #0x3139d8
003139d0: mov      r0, r3
003139d4: b        #0x708f00
003139d8: mov      r0, r3
003139dc: b        #0x310440

# _ZN12ItemInstanceD1Ev
003faa64: ldr      r3, [pc, #0x3c]
003faa68: ldr      r2, [pc, #0x3c]
003faa6c: push     {r4, lr}
003faa70: add      r3, pc, r3
003faa74: ldr      r2, [r3, r2]
003faa78: mov      r4, r0
003faa7c: add      r2, r2, #8
003faa80: str      r2, [r0], #0x5c
003faa84: bl       #0x3faa00
003faa88: add      r0, r4, #0x38
003faa8c: bl       #0x3139ac
003faa90: add      r0, r4, #0x20
003faa94: bl       #0x3139ac
003faa98: add      r0, r4, #8
003faa9c: bl       #0x3139ac
003faaa0: mov      r0, r4
003faaa4: pop      {r4, pc}
003faaa8: subseq   sl, sb, r0, lsr #32
003faaac: andeq    r4, r0, r0, lsl #16
