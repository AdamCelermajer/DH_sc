
# _ZN11AISExternal6OnInitEv
003dcea4: push     {r4, lr}
003dcea8: mov      r4, r0
003dceac: bl       #0x3dbe78
003dceb0: ldr      r1, [pc, #0xc]
003dceb4: mov      r0, r4
003dceb8: add      r1, pc, r1
003dcebc: pop      {r4, lr}
003dcec0: b        #0x37c514
003dcec4: subeq    r8, lr, r0, ror #24

# _ZN11AISExternalC1Eb
003dd0e4: push     {r4, r5, r6, lr}
003dd0e8: ldr      r5, [pc, #0x30]
003dd0ec: mov      r4, r0
003dd0f0: bl       #0x3d8fb0
003dd0f4: ldr      r2, [pc, #0x28]
003dd0f8: add      r5, pc, r5
003dd0fc: mov      r3, #0
003dd100: ldr      r2, [r5, r2]
003dd104: str      r3, [r4, #0xc0]
003dd108: str      r3, [r4, #0xb8]
003dd10c: add      r2, r2, #8
003dd110: str      r2, [r4]
003dd114: str      r3, [r4, #0xbc]
003dd118: mov      r0, r4
003dd11c: pop      {r4, r5, r6, pc}

# _ZN11AISExternal8OnUpdateEv
003dce64: push     {r4, lr}
003dce68: mov      r4, r0
003dce6c: bl       #0x3dc798
003dce70: ldr      r3, [r4, #0xb8]
003dce74: tst      r3, #1
003dce78: beq      #0x3dce8c
003dce7c: ldr      r1, [pc, #0x1c]
003dce80: mov      r0, r4
003dce84: add      r1, pc, r1
003dce88: bl       #0x37c514
003dce8c: mov      r0, r4
003dce90: bl       #0x3d8eb4
003dce94: mov      r0, r4
003dce98: pop      {r4, lr}
003dce9c: b        #0x3d8ea0
003dcea0: subeq    r8, lr, r4, lsl #25
