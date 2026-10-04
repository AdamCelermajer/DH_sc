
# _ZN6CharAI9GroupInfo8CanSpawnEP9Character
003d24fc: mov      r0, #0
003d2500: bx       lr

# _ZNK9Character10CanRespawnEv
003a5248: push     {r4, lr}
003a524c: movw     r3, #0x1481
003a5250: ldrb     r3, [r0, r3]
003a5254: mov      r4, r0
003a5258: cmp      r3, #0
003a525c: beq      #0x3a5268
003a5260: mov      r0, #0
003a5264: pop      {r4, pc}
003a5268: add      r1, r0, #0xff0
003a526c: add      r1, r1, #4
003a5270: add      r0, r0, #0x560
003a5274: mov      r2, #0xb
003a5278: bl       #0x3dedb4
003a527c: cmp      r0, #0
003a5280: ble      #0x3a5260
003a5284: ldr      r0, [r4, #0x3fc]
003a5288: cmp      r0, #0
003a528c: beq      #0x3a529c
003a5290: ldr      r1, [r4, #0x3cc]
003a5294: pop      {r4, lr}
003a5298: b        #0x3d2a34
003a529c: mov      r0, #1
003a52a0: pop      {r4, pc}

# _ZN6CharAI9GroupInfo10CanRespawnEP9Character
003d2a34: push     {r4, r5, r6, r7, r8, lr}
003d2a38: ldr      r3, [r1, #0x400]
003d2a3c: mov      r5, r0
003d2a40: cmp      r3, #0
003d2a44: beq      #0x3d2a74
003d2a48: cmp      r3, #3
003d2a4c: beq      #0x3d2a58
003d2a50: mov      r0, #0
003d2a54: pop      {r4, r5, r6, r7, r8, pc}
003d2a58: ldr      r6, [r0, #0x24]
003d2a5c: cmp      r6, #1
003d2a60: beq      #0x3d2a80
003d2a64: cmp      r6, #2
003d2a68: movne    r0, #0
003d2a6c: moveq    r0, #1
003d2a70: pop      {r4, r5, r6, r7, r8, pc}
003d2a74: ldrb     r0, [r0, #0x28]
003d2a78: eor      r0, r0, #1
003d2a7c: pop      {r4, r5, r6, r7, r8, pc}
003d2a80: ldr      r3, [r0, #0x18]
003d2a84: ldr      r7, [r0, #0x1c]
003d2a88: rsb      r7, r3, r7
003d2a8c: asrs     r7, r7, #2
003d2a90: beq      #0x3d2adc
003d2a94: mov      r4, #0
003d2a98: b        #0x3d2aa0
003d2a9c: ldr      r3, [r5, #0x18]
003d2aa0: ldr      r0, [r3, r4, lsl #2]
003d2aa4: add      r4, r4, #1
003d2aa8: add      r0, r0, #0x4f0
003d2aac: add      r0, r0, #0xc
003d2ab0: bl       #0x3c01c0
003d2ab4: cmp      r0, #0
003d2ab8: moveq    r6, #0
003d2abc: cmp      r4, r7
003d2ac0: bne      #0x3d2a9c
003d2ac4: cmp      r6, #0
003d2ac8: moveq    r0, r6
003d2acc: moveq    r3, #1
003d2ad0: bne      #0x3d2adc
003d2ad4: str      r3, [r5, #0x24]
003d2ad8: pop      {r4, r5, r6, r7, r8, pc}
003d2adc: mov      r0, #1
003d2ae0: mov      r3, #2
003d2ae4: b        #0x3d2ad4
