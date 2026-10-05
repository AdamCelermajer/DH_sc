
# _Z33NativeChangeRolloverInputBehaviorRKN7gameswf7fn_callE
0043cdac: push     {r4, r5, r6, lr}
0043cdb0: ldr      r3, [r0, #0xc]
0043cdb4: mov      r4, r0
0043cdb8: ldr      r0, [r0, #0x14]
0043cdbc: ldr      r3, [r3]
0043cdc0: mov      r5, #0xc
0043cdc4: mla      r0, r5, r0, r3
0043cdc8: bl       #0x797a54
0043cdcc: bl       #0x30ea24
0043cdd0: ldr      r3, [r4, #0xc]
0043cdd4: mov      r6, r0
0043cdd8: ldr      r0, [r4, #0x14]
0043cddc: ldr      r3, [r3]
0043cde0: sub      r0, r0, #1
0043cde4: mla      r0, r5, r0, r3
0043cde8: bl       #0x797960
0043cdec: subs     r4, r0, #0
0043cdf0: beq      #0x43ce18
0043cdf4: bl       #0x42ca8c
0043cdf8: ldr      r3, [r0, #0xf4]
0043cdfc: cmp      r6, #3
0043ce00: movhi    r0, #0
0043ce04: addls    r6, r3, r6, lsl #2
0043ce08: ldrls    r0, [r6, #0x134]
0043ce0c: mov      r1, #0x84
0043ce10: pop      {r4, r5, r6, lr}
0043ce14: b        #0x7a7c98
0043ce18: bl       #0x42ca8c
0043ce1c: ldr      r3, [r0, #0xf4]
0043ce20: cmp      r6, #3
0043ce24: movhi    r0, r4
0043ce28: addls    r6, r3, r6, lsl #2
0043ce2c: ldrls    r0, [r6, #0x134]
0043ce30: mov      r1, #4
0043ce34: pop      {r4, r5, r6, lr}
0043ce38: b        #0x7a7c98

# _ZN12GameSWFUtils23GetAbsoluteBoundingRectEPN7gameswf9characterE
00416a7c: push     {r4, r5, r6, lr}
00416a80: subs     r5, r1, #0
00416a84: sub      sp, sp, #8
00416a88: mov      r4, r0
00416a8c: beq      #0x416b30
00416a90: add      r0, r5, #0x3c
00416a94: bl       #0x386144
00416a98: ldr      r1, [r5, #0x40]
00416a9c: mov      r0, sp
00416aa0: bl       #0x4169e8
00416aa4: ldr      r6, [sp]
00416aa8: ldr      r3, [r5]
00416aac: mov      r0, r5
00416ab0: mov      r1, r4
00416ab4: ldr      r5, [sp, #4]
00416ab8: mov      lr, pc
00416abc: ldr      pc, [r3, #0x12c]
00416ac0: ldr      r1, [r4]
00416ac4: mov      r0, r6
00416ac8: bl       #0x30eba4
00416acc: mov      r1, #0x41000000
00416ad0: add      r1, r1, #0xa00000
00416ad4: bl       #0x30ec94
00416ad8: ldr      r1, [r4, #4]
00416adc: str      r0, [r4]
00416ae0: mov      r0, r6
00416ae4: bl       #0x30eba4
00416ae8: mov      r1, #0x41000000
00416aec: add      r1, r1, #0xa00000
00416af0: bl       #0x30ec94
00416af4: ldr      r1, [r4, #8]
00416af8: str      r0, [r4, #4]
00416afc: mov      r0, r5
00416b00: bl       #0x30eba4
00416b04: mov      r1, #0x41000000
00416b08: add      r1, r1, #0xa00000
00416b0c: bl       #0x30ec94
00416b10: ldr      r1, [r4, #0xc]
00416b14: str      r0, [r4, #8]
00416b18: mov      r0, r5
00416b1c: bl       #0x30eba4
00416b20: mov      r1, #0x41000000
00416b24: add      r1, r1, #0xa00000
00416b28: bl       #0x30ec94
00416b2c: str      r0, [r4, #0xc]
00416b30: mov      r0, r4
00416b34: add      sp, sp, #8
00416b38: pop      {r4, r5, r6, pc}

# _ZN12GameSWFUtils19GetAbsolutePositionEPN7gameswf9characterE
004169e8: push     {r4, r5, r6, r7, r8, lr}
004169ec: mov      r6, #0
004169f0: subs     r7, r1, #0
004169f4: mov      r5, r0
004169f8: str      r6, [r0]
004169fc: str      r6, [r0, #4]
00416a00: moveq    r4, r7
00416a04: beq      #0x416a60
00416a08: ldr      r3, [r7, #0x4c]
00416a0c: mov      r1, r6
00416a10: mov      r4, r7
00416a14: ldr      r0, [r3, #8]
00416a18: bl       #0x30eba4
00416a1c: str      r0, [r5]
00416a20: ldr      r3, [r7, #0x4c]
00416a24: mov      r1, r6
00416a28: ldr      r0, [r3, #0x14]
00416a2c: bl       #0x30eba4
00416a30: str      r0, [r5, #4]
00416a34: b        #0x416a60
00416a38: ldr      r3, [r4, #0x4c]
00416a3c: ldr      r0, [r5]
00416a40: ldr      r1, [r3, #8]
00416a44: bl       #0x30eba4
00416a48: str      r0, [r5]
00416a4c: ldr      r3, [r4, #0x4c]
00416a50: ldr      r0, [r5, #4]
00416a54: ldr      r1, [r3, #0x14]
00416a58: bl       #0x30eba4
00416a5c: str      r0, [r5, #4]
00416a60: add      r0, r4, #0x3c
00416a64: bl       #0x386144
00416a68: ldr      r4, [r4, #0x40]
00416a6c: cmp      r4, #0
00416a70: bne      #0x416a38
00416a74: mov      r0, r5
00416a78: pop      {r4, r5, r6, r7, r8, pc}
