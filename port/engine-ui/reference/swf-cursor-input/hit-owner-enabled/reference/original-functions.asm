
# _ZNK7gameswf9character10is_enabledEv
00752dd4: mov      r0, #1
00752dd8: bx       lr

# _ZNK7gameswf15sprite_instance10is_enabledEv
00782de4: push     {r4, lr}
00782de8: mov      r4, r0
00782dec: ldrb     r0, [r0, #0xea]
00782df0: cmp      r0, #0
00782df4: bne      #0x782dfc
00782df8: pop      {r4, pc}
00782dfc: ldr      r3, [r4, #0x40]
00782e00: cmp      r3, #0
00782e04: beq      #0x782e58
00782e08: ldr      r0, [r4, #0x3c]
00782e0c: ldrb     r2, [r0, #4]
00782e10: cmp      r2, #0
00782e14: beq      #0x782e2c
00782e18: mov      r0, r3
00782e1c: ldr      r3, [r3]
00782e20: mov      lr, pc
00782e24: ldr      pc, [r3, #0x170]
00782e28: pop      {r4, pc}
00782e2c: ldr      r1, [r0]
00782e30: sub      r1, r1, #1
00782e34: cmp      r1, #0
00782e38: str      r1, [r0]
00782e3c: bne      #0x782e44
00782e40: bl       #0x752b38
00782e44: mov      r3, #0
00782e48: str      r3, [r4, #0x40]
00782e4c: str      r3, [r4, #0x3c]
00782e50: mov      r0, #1
00782e54: pop      {r4, pc}
00782e58: mov      r0, #1
00782e5c: pop      {r4, pc}
