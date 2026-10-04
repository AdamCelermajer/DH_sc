
# _ZN9AISPlayer7InitVCBEv
003dd884: push     {r4, r5, r6, lr}
003dd888: mov      r4, r0
003dd88c: bl       #0x3dc7d8
003dd890: ldr      r1, [pc, #0x24]
003dd894: mov      r0, r4
003dd898: ldr      r5, [r4, #0xb8]
003dd89c: add      r1, pc, r1
003dd8a0: bl       #0x37c2a0
003dd8a4: cmp      r0, #0
003dd8a8: movne    r0, #0x400
003dd8ac: moveq    r0, #0
003dd8b0: orr      r5, r0, r5
003dd8b4: str      r5, [r4, #0xb8]
003dd8b8: pop      {r4, r5, r6, pc}
003dd8bc: strdeq   r8, sb, [lr], #-0x24
