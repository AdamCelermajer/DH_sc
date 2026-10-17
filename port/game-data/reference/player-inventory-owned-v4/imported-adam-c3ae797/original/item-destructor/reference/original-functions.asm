
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
