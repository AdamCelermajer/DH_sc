
# _ZN15VisualFXManager17_PreCacheAnimDictEv
004933d8: mov      r3, #1
004933dc: strb     r3, [r0, #4]
004933e0: bx       lr

# _ZN15VisualFXManager17PreCacheLibrariesEv
00495a88: push     {r4, r5, r6, r7, r8, lr}
00495a8c: ldr      r4, [pc, #0xb0]
00495a90: ldr      r6, [pc, #0xb0]
00495a94: ldr      r2, [pc, #0xb0]
00495a98: add      r4, pc, r4
00495a9c: ldr      r3, [r4, r6]
00495aa0: ldr      r7, [r4, r2]
00495aa4: sub      sp, sp, #0x20
00495aa8: ldr      r3, [r3]
00495aac: mov      r8, r0
00495ab0: mov      r0, r7
00495ab4: str      r3, [sp, #0x1c]
00495ab8: bl       #0x337888
00495abc: ldr      r1, [pc, #0x8c]
00495ac0: add      r5, sp, #4
00495ac4: mov      r2, sp
00495ac8: add      r1, pc, r1
00495acc: mov      r0, r5
00495ad0: bl       #0x3140ec
00495ad4: mov      r0, r7
00495ad8: mov      r1, r5
00495adc: bl       #0x337ec8
00495ae0: mov      r7, r0
00495ae4: ldr      r0, [sp, #0x18]
00495ae8: cmp      r0, r5
00495aec: beq      #0x495b0c
00495af0: cmp      r0, #0
00495af4: beq      #0x495b0c
00495af8: ldr      r1, [sp, #4]
00495afc: rsb      r1, r0, r1
00495b00: cmp      r1, #0x80
00495b04: bhi      #0x495b38
00495b08: bl       #0x708f00
00495b0c: cmp      r7, #0
00495b10: beq      #0x495b1c
00495b14: mov      r0, r8
00495b18: bl       #0x4933d8
00495b1c: ldr      r3, [r4, r6]
00495b20: ldr      r2, [sp, #0x1c]
00495b24: ldr      r3, [r3]
00495b28: cmp      r2, r3
00495b2c: bne      #0x495b40
00495b30: add      sp, sp, #0x20
00495b34: pop      {r4, r5, r6, r7, r8, pc}
00495b38: bl       #0x310440
00495b3c: b        #0x495b0c
00495b40: bl       #0x30e310
00495b44: strdeq   lr, pc, [pc], #-0xf8
00495b48: andeq    r4, r0, ip, lsr #1
00495b4c: andeq    r0, r0, r4, lsl #17
00495b50: subeq    pc, r3, r0, lsl #11

# _ZN15VisualFXManager14BuildLibrariesEv
00496bd8: push     {r4, r5, r6, r7, r8, lr}
00496bdc: ldr      r4, [pc, #0xb0]
00496be0: ldr      r6, [pc, #0xb0]
00496be4: ldr      r2, [pc, #0xb0]
00496be8: add      r4, pc, r4
00496bec: ldr      r3, [r4, r6]
00496bf0: ldr      r7, [r4, r2]
00496bf4: sub      sp, sp, #0x20
00496bf8: ldr      r3, [r3]
00496bfc: mov      r8, r0
00496c00: mov      r0, r7
00496c04: str      r3, [sp, #0x1c]
00496c08: bl       #0x337888
00496c0c: ldr      r1, [pc, #0x8c]
00496c10: add      r5, sp, #4
00496c14: mov      r2, sp
00496c18: add      r1, pc, r1
00496c1c: mov      r0, r5
00496c20: bl       #0x3140ec
00496c24: mov      r0, r7
00496c28: mov      r1, r5
00496c2c: bl       #0x337ec8
00496c30: mov      r7, r0
00496c34: ldr      r0, [sp, #0x18]
00496c38: cmp      r0, r5
00496c3c: beq      #0x496c5c
00496c40: cmp      r0, #0
00496c44: beq      #0x496c5c
00496c48: ldr      r1, [sp, #4]
00496c4c: rsb      r1, r0, r1
00496c50: cmp      r1, #0x80
00496c54: bhi      #0x496c88
00496c58: bl       #0x708f00
00496c5c: cmp      r7, #0
00496c60: beq      #0x496c6c
00496c64: mov      r0, r8
00496c68: bl       #0x496ba4
00496c6c: ldr      r3, [r4, r6]
00496c70: ldr      r2, [sp, #0x1c]
00496c74: ldr      r3, [r3]
00496c78: cmp      r2, r3
00496c7c: bne      #0x496c90
00496c80: add      sp, sp, #0x20
00496c84: pop      {r4, r5, r6, r7, r8, pc}
00496c88: bl       #0x310440
00496c8c: b        #0x496c5c
00496c90: bl       #0x30e310
00496c94: subeq    sp, pc, r8, lsr #29
00496c98: andeq    r4, r0, ip, lsr #1
00496c9c: andeq    r0, r0, r4, lsl #17
00496ca0: subeq    lr, r3, r0, lsr r4
