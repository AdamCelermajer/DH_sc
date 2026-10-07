
# _ZNSsC1EPKcRKSaIcE.clone.1
003d43ec: push     {r4, lr}
003d43f0: mov      r4, r0
003d43f4: str      r0, [r4, #0x10]
003d43f8: str      r0, [r4, #0x14]
003d43fc: mov      r1, #0x1a
003d4400: bl       #0x31167c
003d4404: ldr      r1, [pc, #0x24]
003d4408: ldr      r0, [r4, #0x14]
003d440c: mov      r2, #0x19
003d4410: add      r1, pc, r1
003d4414: bl       #0x30e868
003d4418: add      r3, r0, #0x19
003d441c: str      r3, [r4, #0x10]
003d4420: mov      r3, #0
003d4424: strb     r3, [r0, #0x19]
003d4428: mov      r0, r4
003d442c: pop      {r4, pc}
003d4430: subeq    r1, pc, r0, lsl r1
