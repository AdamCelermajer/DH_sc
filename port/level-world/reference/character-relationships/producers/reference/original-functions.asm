
# _ZN7Structs10AIFactions4readEP11IStreamBase
004dd158: push     {r4, r5, r6, r7, r8, lr}
004dd15c: mov      r5, r0
004dd160: sub      sp, sp, #8
004dd164: mov      r0, r1
004dd168: mov      r7, r1
004dd16c: ldr      r6, [pc, #0x148]
004dd170: add      r1, r5, #4
004dd174: bl       #0x3df1a0
004dd178: mov      r3, #1
004dd17c: cmp      r3, #0
004dd180: str      r3, [sp, #4]
004dd184: add      r6, pc, r6
004dd188: bne      #0x4dd1cc
004dd18c: add      r3, r5, #5
004dd190: add      r2, r5, #6
004dd194: ldrb     r0, [r2, #1]
004dd198: ldrb     r1, [r3, #-1]
004dd19c: cmp      r3, r2
004dd1a0: eor      r1, r0, r1
004dd1a4: strb     r1, [r3, #-1]
004dd1a8: ldrb     r0, [r2, #1]
004dd1ac: eor      r1, r1, r0
004dd1b0: strb     r1, [r2, #1]
004dd1b4: ldrb     r0, [r3, #-1]
004dd1b8: sub      r2, r2, #1
004dd1bc: eor      r1, r1, r0
004dd1c0: strb     r1, [r3, #-1]
004dd1c4: add      r3, r3, #1
004dd1c8: blo      #0x4dd194
004dd1cc: ldr      r3, [r5, #8]
004dd1d0: cmp      r3, #0
004dd1d4: beq      #0x4dd21c
004dd1d8: ldr      r2, [r3, #-4]
004dd1dc: mov      r0, #0xc
004dd1e0: mla      r0, r0, r2, r3
004dd1e4: cmp      r3, r0
004dd1e8: bne      #0x4dd1f4
004dd1ec: b        #0x4dd214
004dd1f0: mov      r0, r4
004dd1f4: sub      r4, r0, #0xc
004dd1f8: ldr      r3, [r0, #-0xc]
004dd1fc: mov      r0, r4
004dd200: mov      lr, pc
004dd204: ldr      pc, [r3]
004dd208: ldr      r0, [r5, #8]
004dd20c: cmp      r0, r4
004dd210: bne      #0x4dd1f0
004dd214: sub      r0, r0, #8
004dd218: bl       #0x310440
004dd21c: ldr      r4, [r5, #4]
004dd220: mov      r8, #0xc
004dd224: mov      r1, #1
004dd228: mul      r0, r8, r4
004dd22c: add      r0, r0, #8
004dd230: bl       #0x31056c
004dd234: cmp      r4, #0
004dd238: str      r8, [r0]
004dd23c: str      r4, [r0, #4]
004dd240: add      r3, r0, #8
004dd244: beq      #0x4dd26c
004dd248: ldr      r1, [pc, #0x70]
004dd24c: mov      r2, #0
004dd250: ldr      r1, [r6, r1]
004dd254: add      r1, r1, #8
004dd258: add      r2, r2, #1
004dd25c: cmp      r2, r4
004dd260: str      r1, [r0, #8]
004dd264: add      r0, r0, #0xc
004dd268: bne      #0x4dd258
004dd26c: ldr      r2, [r5, #4]
004dd270: str      r3, [r5, #8]
004dd274: cmp      r2, #0
004dd278: beq      #0x4dd2b4
004dd27c: mov      r4, #0
004dd280: mov      r6, r4
004dd284: b        #0x4dd28c
004dd288: ldr      r3, [r5, #8]
004dd28c: add      r0, r3, r4
004dd290: mov      r1, r7
004dd294: ldr      r3, [r3, r4]
004dd298: mov      lr, pc
004dd29c: ldr      pc, [r3, #0xc]
004dd2a0: ldr      r3, [r5, #4]
004dd2a4: add      r6, r6, #1
004dd2a8: add      r4, r4, #0xc
004dd2ac: cmp      r3, r6
004dd2b0: bhi      #0x4dd288
004dd2b4: add      sp, sp, #8
004dd2b8: pop      {r4, r5, r6, r7, r8, pc}
004dd2bc: subeq    r7, fp, ip, lsl #18
004dd2c0: ldrdeq   r1, r2, [r0], -r0

# _ZN6Arrays14AIFactionTable4readEP11IStreamBase
004b3a38: push     {r4, r5, r6, r7, r8, sl, lr}
004b3a3c: sub      sp, sp, #0xc
004b3a40: mov      sl, r0
004b3a44: bl       #0x313a90
004b3a48: ldr      r6, [pc, #0x124]
004b3a4c: mov      r3, #1
004b3a50: cmp      r3, #0
004b3a54: str      r0, [sp, #4]
004b3a58: str      r3, [sp]
004b3a5c: add      r6, pc, r6
004b3a60: bne      #0x4b3aa8
004b3a64: add      r3, sp, #4
004b3a68: add      r2, r3, #2
004b3a6c: add      r3, r3, #1
004b3a70: ldrb     r0, [r2, #1]
004b3a74: ldrb     r1, [r3, #-1]
004b3a78: cmp      r2, r3
004b3a7c: eor      r1, r0, r1
004b3a80: strb     r1, [r3, #-1]
004b3a84: ldrb     r0, [r2, #1]
004b3a88: eor      r1, r1, r0
004b3a8c: strb     r1, [r2, #1]
004b3a90: ldrb     r0, [r3, #-1]
004b3a94: sub      r2, r2, #1
004b3a98: eor      r1, r1, r0
004b3a9c: strb     r1, [r3, #-1]
004b3aa0: add      r3, r3, #1
004b3aa4: bhi      #0x4b3a70
004b3aa8: bl       #0x4a9fb8
004b3aac: ldr      r7, [pc, #0xc4]
004b3ab0: ldr      r4, [sp, #4]
004b3ab4: mov      r5, #0xc
004b3ab8: ldr      r3, [r6, r7]
004b3abc: mul      r0, r5, r4
004b3ac0: str      r4, [r3]
004b3ac4: add      r0, r0, #8
004b3ac8: mov      r1, #1
004b3acc: bl       #0x31056c
004b3ad0: cmp      r4, #0
004b3ad4: str      r5, [r0]
004b3ad8: str      r4, [r0, #4]
004b3adc: add      r3, r0, #8
004b3ae0: beq      #0x4b3b10
004b3ae4: ldr      r1, [pc, #0x90]
004b3ae8: mov      r2, #0
004b3aec: mov      ip, r2
004b3af0: ldr      r1, [r6, r1]
004b3af4: add      r1, r1, #8
004b3af8: add      r2, r2, #1
004b3afc: cmp      r2, r4
004b3b00: str      r1, [r0, #8]
004b3b04: str      ip, [r0, #0x10]
004b3b08: add      r0, r0, #0xc
004b3b0c: bne      #0x4b3af8
004b3b10: ldr      r2, [r6, r7]
004b3b14: ldr      r8, [pc, #0x64]
004b3b18: ldr      r1, [r2]
004b3b1c: ldr      r2, [r6, r8]
004b3b20: cmp      r1, #0
004b3b24: str      r3, [r2]
004b3b28: beq      #0x4b3b6c
004b3b2c: mov      r4, #0
004b3b30: mov      r5, r4
004b3b34: b        #0x4b3b40
004b3b38: ldr      r3, [r6, r8]
004b3b3c: ldr      r3, [r3]
004b3b40: add      r0, r3, r4
004b3b44: mov      r1, sl
004b3b48: ldr      r3, [r3, r4]
004b3b4c: mov      lr, pc
004b3b50: ldr      pc, [r3, #0xc]
004b3b54: ldr      r3, [r6, r7]
004b3b58: add      r5, r5, #1
004b3b5c: add      r4, r4, #0xc
004b3b60: ldr      r3, [r3]
004b3b64: cmp      r3, r5
004b3b68: bhi      #0x4b3b38
004b3b6c: add      sp, sp, #0xc
004b3b70: pop      {r4, r5, r6, r7, r8, sl, pc}
004b3b74: subeq    r1, lr, r4, lsr r0
004b3b78: andeq    r2, r0, r4, asr #4
004b3b7c: andeq    r3, r0, r8, lsr ip
004b3b80: andeq    r4, r0, ip, lsr #12

# _ZN7Structs9AIFaction4readEP11IStreamBase
004eb408: push     {r4, r5, lr}
004eb40c: mov      r4, r0
004eb410: sub      sp, sp, #0xc
004eb414: mov      r0, r1
004eb418: mov      r5, r1
004eb41c: add      r1, r4, #4
004eb420: bl       #0x459090
004eb424: mov      r3, #1
004eb428: cmp      r3, #0
004eb42c: str      r3, [sp, #4]
004eb430: bne      #0x4eb474
004eb434: add      r3, r4, #5
004eb438: add      r2, r4, #6
004eb43c: ldrb     r0, [r2, #1]
004eb440: ldrb     r1, [r3, #-1]
004eb444: cmp      r3, r2
004eb448: eor      r1, r0, r1
004eb44c: strb     r1, [r3, #-1]
004eb450: ldrb     r0, [r2, #1]
004eb454: eor      r1, r1, r0
004eb458: strb     r1, [r2, #1]
004eb45c: ldrb     r0, [r3, #-1]
004eb460: sub      r2, r2, #1
004eb464: eor      r1, r1, r0
004eb468: strb     r1, [r3, #-1]
004eb46c: add      r3, r3, #1
004eb470: blo      #0x4eb43c
004eb474: mov      r0, r5
004eb478: add      r1, r4, #8
004eb47c: bl       #0x459090
004eb480: mov      r3, #1
004eb484: cmp      r3, #0
004eb488: str      r3, [sp, #4]
004eb48c: bne      #0x4eb4d0
004eb490: add      r3, r4, #0xa
004eb494: add      r4, r4, #9
004eb498: ldrb     r1, [r3, #1]
004eb49c: ldrb     r2, [r4, #-1]
004eb4a0: cmp      r3, r4
004eb4a4: eor      r2, r1, r2
004eb4a8: strb     r2, [r4, #-1]
004eb4ac: ldrb     r1, [r3, #1]
004eb4b0: eor      r2, r2, r1
004eb4b4: strb     r2, [r3, #1]
004eb4b8: ldrb     r1, [r4, #-1]
004eb4bc: sub      r3, r3, #1
004eb4c0: eor      r2, r2, r1
004eb4c4: strb     r2, [r4, #-1]
004eb4c8: add      r4, r4, #1
004eb4cc: bhi      #0x4eb498
004eb4d0: add      sp, sp, #0xc
004eb4d4: pop      {r4, r5, pc}

# _ZNK9Character18GetCharAIFactionIdEv
003a3180: ldr      r0, [r0, #0xff8]
003a3184: ldr      r3, [pc, #0x24]
003a3188: cmp      r0, #0
003a318c: add      r3, pc, r3
003a3190: blt      #0x3a31a8
003a3194: ldr      r2, [pc, #0x18]
003a3198: ldr      r3, [r3, r2]
003a319c: ldr      r3, [r3]
003a31a0: cmp      r0, r3
003a31a4: bxlt     lr
003a31a8: mov      r0, #0xa
003a31ac: bx       lr
003a31b0: subseq   r1, pc, r4, lsl #18
003a31b4: andeq    r2, r0, r4, asr #4

# _ZNK9Character16GetCharAIFactionEv
003a31b8: ldr      r3, [pc, #0x20]
003a31bc: ldr      r2, [pc, #0x20]
003a31c0: push     {r4, lr}
003a31c4: add      r3, pc, r3
003a31c8: ldr      r2, [r3, r2]
003a31cc: ldr      r4, [r2]
003a31d0: bl       #0x3a3180
003a31d4: mov      r3, #0xc
003a31d8: mla      r0, r3, r0, r4
003a31dc: pop      {r4, pc}
003a31e0: subseq   r1, pc, ip, asr #17
003a31e4: andeq    r4, r0, ip, lsr #12
