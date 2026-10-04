
# _ZN7Structs18ItemPowerEntryList4readEP11IStreamBase
004dc8fc: push     {r4, r5, r6, r7, r8, lr}
004dc900: mov      r5, r0
004dc904: sub      sp, sp, #8
004dc908: mov      r0, r1
004dc90c: mov      r7, r1
004dc910: ldr      r6, [pc, #0x148]
004dc914: add      r1, r5, #4
004dc918: bl       #0x3df1a0
004dc91c: mov      r3, #1
004dc920: cmp      r3, #0
004dc924: str      r3, [sp, #4]
004dc928: add      r6, pc, r6
004dc92c: bne      #0x4dc970
004dc930: add      r3, r5, #5
004dc934: add      r2, r5, #6
004dc938: ldrb     r0, [r2, #1]
004dc93c: ldrb     r1, [r3, #-1]
004dc940: cmp      r3, r2
004dc944: eor      r1, r0, r1
004dc948: strb     r1, [r3, #-1]
004dc94c: ldrb     r0, [r2, #1]
004dc950: eor      r1, r1, r0
004dc954: strb     r1, [r2, #1]
004dc958: ldrb     r0, [r3, #-1]
004dc95c: sub      r2, r2, #1
004dc960: eor      r1, r1, r0
004dc964: strb     r1, [r3, #-1]
004dc968: add      r3, r3, #1
004dc96c: blo      #0x4dc938
004dc970: ldr      r3, [r5, #8]
004dc974: cmp      r3, #0
004dc978: beq      #0x4dc9c0
004dc97c: ldr      r2, [r3, #-4]
004dc980: mov      r0, #0xc
004dc984: mla      r0, r0, r2, r3
004dc988: cmp      r3, r0
004dc98c: bne      #0x4dc998
004dc990: b        #0x4dc9b8
004dc994: mov      r0, r4
004dc998: sub      r4, r0, #0xc
004dc99c: ldr      r3, [r0, #-0xc]
004dc9a0: mov      r0, r4
004dc9a4: mov      lr, pc
004dc9a8: ldr      pc, [r3]
004dc9ac: ldr      r0, [r5, #8]
004dc9b0: cmp      r0, r4
004dc9b4: bne      #0x4dc994
004dc9b8: sub      r0, r0, #8
004dc9bc: bl       #0x310440
004dc9c0: ldr      r4, [r5, #4]
004dc9c4: mov      r8, #0xc
004dc9c8: mov      r1, #1
004dc9cc: mul      r0, r8, r4
004dc9d0: add      r0, r0, #8
004dc9d4: bl       #0x31056c
004dc9d8: cmp      r4, #0
004dc9dc: str      r8, [r0]
004dc9e0: str      r4, [r0, #4]
004dc9e4: add      r3, r0, #8
004dc9e8: beq      #0x4dca10
004dc9ec: ldr      r1, [pc, #0x70]
004dc9f0: mov      r2, #0
004dc9f4: ldr      r1, [r6, r1]
004dc9f8: add      r1, r1, #8
004dc9fc: add      r2, r2, #1
004dca00: cmp      r2, r4
004dca04: str      r1, [r0, #8]
004dca08: add      r0, r0, #0xc
004dca0c: bne      #0x4dc9fc
004dca10: ldr      r2, [r5, #4]
004dca14: str      r3, [r5, #8]
004dca18: cmp      r2, #0
004dca1c: beq      #0x4dca58
004dca20: mov      r4, #0
004dca24: mov      r6, r4
004dca28: b        #0x4dca30
004dca2c: ldr      r3, [r5, #8]
004dca30: add      r0, r3, r4
004dca34: mov      r1, r7
004dca38: ldr      r3, [r3, r4]
004dca3c: mov      lr, pc
004dca40: ldr      pc, [r3, #0xc]
004dca44: ldr      r3, [r5, #4]
004dca48: add      r6, r6, #1
004dca4c: add      r4, r4, #0xc
004dca50: cmp      r3, r6
004dca54: bhi      #0x4dca2c
004dca58: add      sp, sp, #8
004dca5c: pop      {r4, r5, r6, r7, r8, pc}
004dca60: subeq    r8, fp, r8, ror #2
004dca64: andeq    r0, r0, ip, lsl #14

# _ZN6Arrays13ItemPowerList4readEP11IStreamBase
004bacc8: push     {r4, r5, r6, r7, r8, sl, lr}
004baccc: sub      sp, sp, #0xc
004bacd0: mov      sl, r0
004bacd4: bl       #0x313a90
004bacd8: ldr      r6, [pc, #0x124]
004bacdc: mov      r3, #1
004bace0: cmp      r3, #0
004bace4: str      r0, [sp, #4]
004bace8: str      r3, [sp]
004bacec: add      r6, pc, r6
004bacf0: bne      #0x4bad38
004bacf4: add      r3, sp, #4
004bacf8: add      r2, r3, #2
004bacfc: add      r3, r3, #1
004bad00: ldrb     r0, [r2, #1]
004bad04: ldrb     r1, [r3, #-1]
004bad08: cmp      r2, r3
004bad0c: eor      r1, r0, r1
004bad10: strb     r1, [r3, #-1]
004bad14: ldrb     r0, [r2, #1]
004bad18: eor      r1, r1, r0
004bad1c: strb     r1, [r2, #1]
004bad20: ldrb     r0, [r3, #-1]
004bad24: sub      r2, r2, #1
004bad28: eor      r1, r1, r0
004bad2c: strb     r1, [r3, #-1]
004bad30: add      r3, r3, #1
004bad34: bhi      #0x4bad00
004bad38: bl       #0x4a6cf4
004bad3c: ldr      r7, [pc, #0xc4]
004bad40: ldr      r4, [sp, #4]
004bad44: mov      r5, #0xc
004bad48: ldr      r3, [r6, r7]
004bad4c: mul      r0, r5, r4
004bad50: str      r4, [r3]
004bad54: add      r0, r0, #8
004bad58: mov      r1, #1
004bad5c: bl       #0x31056c
004bad60: cmp      r4, #0
004bad64: str      r5, [r0]
004bad68: str      r4, [r0, #4]
004bad6c: add      r3, r0, #8
004bad70: beq      #0x4bada0
004bad74: ldr      r1, [pc, #0x90]
004bad78: mov      r2, #0
004bad7c: mov      ip, r2
004bad80: ldr      r1, [r6, r1]
004bad84: add      r1, r1, #8
004bad88: add      r2, r2, #1
004bad8c: cmp      r2, r4
004bad90: str      r1, [r0, #8]
004bad94: str      ip, [r0, #0x10]
004bad98: add      r0, r0, #0xc
004bad9c: bne      #0x4bad88
004bada0: ldr      r2, [r6, r7]
004bada4: ldr      r8, [pc, #0x64]
004bada8: ldr      r1, [r2]
004badac: ldr      r2, [r6, r8]
004badb0: cmp      r1, #0
004badb4: str      r3, [r2]
004badb8: beq      #0x4badfc
004badbc: mov      r4, #0
004badc0: mov      r5, r4
004badc4: b        #0x4badd0
004badc8: ldr      r3, [r6, r8]
004badcc: ldr      r3, [r3]
004badd0: add      r0, r3, r4
004badd4: mov      r1, sl
004badd8: ldr      r3, [r3, r4]
004baddc: mov      lr, pc
004bade0: ldr      pc, [r3, #0xc]
004bade4: ldr      r3, [r6, r7]
004bade8: add      r5, r5, #1
004badec: add      r4, r4, #0xc
004badf0: ldr      r3, [r3]
004badf4: cmp      r3, r5
004badf8: bhi      #0x4badc8
004badfc: add      sp, sp, #0xc
004bae00: pop      {r4, r5, r6, r7, r8, sl, pc}
004bae04: subeq    sb, sp, r4, lsr #27
004bae08: andeq    r0, r0, r8, asr #22
004bae0c: andeq    r3, r0, r8, lsl #1
004bae10: andeq    r0, r0, r8, lsr #15

# _ZN7Structs14ItemPowerEntry4readEP11IStreamBase
004ed200: push     {r4, r5, lr}
004ed204: mov      r4, r0
004ed208: sub      sp, sp, #0xc
004ed20c: mov      r0, r1
004ed210: mov      r5, r1
004ed214: add      r1, r4, #4
004ed218: bl       #0x459090
004ed21c: mov      r3, #1
004ed220: cmp      r3, #0
004ed224: str      r3, [sp, #4]
004ed228: bne      #0x4ed26c
004ed22c: add      r3, r4, #5
004ed230: add      r2, r4, #6
004ed234: ldrb     r0, [r2, #1]
004ed238: ldrb     r1, [r3, #-1]
004ed23c: cmp      r3, r2
004ed240: eor      r1, r0, r1
004ed244: strb     r1, [r3, #-1]
004ed248: ldrb     r0, [r2, #1]
004ed24c: eor      r1, r1, r0
004ed250: strb     r1, [r2, #1]
004ed254: ldrb     r0, [r3, #-1]
004ed258: sub      r2, r2, #1
004ed25c: eor      r1, r1, r0
004ed260: strb     r1, [r3, #-1]
004ed264: add      r3, r3, #1
004ed268: blo      #0x4ed234
004ed26c: mov      r0, r5
004ed270: add      r1, r4, #8
004ed274: bl       #0x4db9fc
004ed278: add      sp, sp, #0xc
004ed27c: pop      {r4, r5, pc}
