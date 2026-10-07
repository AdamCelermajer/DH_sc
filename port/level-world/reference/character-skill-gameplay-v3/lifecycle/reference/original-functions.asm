
# _ZNK6CharAI16AI_IsSkillUsableEj
003d8358: push     {r4, r5, r6, r7, lr}
003d835c: mov      r4, r0
003d8360: ldr      r0, [r0, #4]
003d8364: sub      sp, sp, #0xc
003d8368: mov      r5, r1
003d836c: add      r0, r0, #0x4f0
003d8370: add      r0, r0, #0xc
003d8374: bl       #0x3c02e8
003d8378: ldr      r6, [pc, #0xdc]
003d837c: cmp      r0, #0
003d8380: ldreq    r0, [r4, #4]
003d8384: add      r6, pc, r6
003d8388: beq      #0x3d83a8
003d838c: ldr      r0, [r4, #4]
003d8390: ldr      r3, [r0, #0x520]
003d8394: tst      r3, #0x8000
003d8398: bne      #0x3d83a8
003d839c: mov      r0, #0
003d83a0: add      sp, sp, #0xc
003d83a4: pop      {r4, r5, r6, r7, pc}
003d83a8: add      r0, r0, #0x4f0
003d83ac: add      r0, r0, #0xc
003d83b0: bl       #0x3c0334
003d83b4: subs     r7, r0, #0
003d83b8: bne      #0x3d839c
003d83bc: mov      r0, r4
003d83c0: bl       #0x3cb458
003d83c4: cmp      r0, #0
003d83c8: beq      #0x3d839c
003d83cc: ldr      r3, [r4, #0xb4]
003d83d0: ldr      r2, [r4, #0xb8]
003d83d4: rsb      r2, r3, r2
003d83d8: cmp      r5, r2, asr #2
003d83dc: blo      #0x3d8444
003d83e0: ldr      r3, [pc, #0x78]
003d83e4: ldr      r3, [r6, r3]
003d83e8: ldr      r3, [r3]
003d83ec: cmp      r3, #2
003d83f0: streq    r7, [r7]
003d83f4: beq      #0x3d839c
003d83f8: cmp      r3, #1
003d83fc: bne      #0x3d839c
003d8400: ldr      r0, [pc, #0x5c]
003d8404: ldr      r1, [pc, #0x5c]
003d8408: ldr      r2, [pc, #0x5c]
003d840c: ldr      r0, [r6, r0]
003d8410: ldr      r3, [pc, #0x58]
003d8414: add      r2, pc, r2
003d8418: mov      ip, #0xb5
003d841c: add      r3, pc, r3
003d8420: add      r1, pc, r1
003d8424: add      r0, r0, #0xa8
003d8428: str      ip, [sp]
003d842c: bl       #0x30e004
003d8430: ldr      r2, [r4, #0xb8]
003d8434: ldr      r3, [r4, #0xb4]
003d8438: rsb      r2, r3, r2
003d843c: cmp      r5, r2, asr #2
003d8440: bhs      #0x3d839c
003d8444: ldr      r0, [r3, r5, lsl #2]
003d8448: cmp      r0, #0
003d844c: beq      #0x3d839c
003d8450: add      sp, sp, #0xc
003d8454: pop      {r4, r5, r6, r7, lr}
003d8458: b        #0x3da9dc
003d845c: subseq   ip, fp, ip, lsl #14
003d8460: andeq    r3, r0, r0, asr #19
003d8464: andeq    r1, r0, r0, asr #19
003d8468: strheq   r5, [lr], #-0xf8
003d846c: subeq    sp, lr, ip, ror #6
003d8470: subeq    sp, lr, ip, lsl #6

# _ZN6CharAI13AI_BeginSkillEj
003d86bc: push     {r4, r5, r6, r7, r8, sl, lr}
003d86c0: mov      r4, r0
003d86c4: sub      sp, sp, #0xc
003d86c8: ldr      r0, [r0, #4]
003d86cc: mov      r7, r1
003d86d0: bl       #0x3bc784
003d86d4: ldr      r6, [r0, #0x48]
003d86d8: ldr      r5, [pc, #0x170]
003d86dc: mov      r8, r0
003d86e0: cmp      r6, #1
003d86e4: add      r5, pc, r5
003d86e8: beq      #0x3d876c
003d86ec: mov      r0, r4
003d86f0: mov      r1, r7
003d86f4: bl       #0x3d8358
003d86f8: cmp      r0, #0
003d86fc: bne      #0x3d8708
003d8700: add      sp, sp, #0xc
003d8704: pop      {r4, r5, r6, r7, r8, sl, pc}
003d8708: ldr      r0, [r4, #4]
003d870c: mov      r6, #0
003d8710: str      r7, [r4, #0xcc]
003d8714: strb     r6, [r4, #0xd0]
003d8718: strb     r6, [r4, #0xd1]
003d871c: add      r0, r0, #0x4f0
003d8720: ldrb     r2, [r8, #8]
003d8724: mov      r1, r7
003d8728: add      r0, r0, #0xc
003d872c: mov      r3, r6
003d8730: str      r6, [sp]
003d8734: bl       #0x3c6670
003d8738: ldr      r3, [r4, #4]
003d873c: mov      r0, r3
003d8740: ldr      r3, [r3]
003d8744: mov      lr, pc
003d8748: ldr      pc, [r3, #0x28]
003d874c: cmp      r0, r6
003d8750: bne      #0x3d8794
003d8754: ldr      r0, [r4, #4]
003d8758: add      r0, r0, #0x4f0
003d875c: add      r0, r0, #0xc
003d8760: add      sp, sp, #0xc
003d8764: pop      {r4, r5, r6, r7, r8, sl, lr}
003d8768: b        #0x3c02e8
003d876c: mov      r0, r4
003d8770: mov      r1, r7
003d8774: bl       #0x3d85d4
003d8778: cmp      r0, #0
003d877c: beq      #0x3d86ec
003d8780: ldr      r3, [r4, #0xb4]
003d8784: ldr      r0, [r3, r7, lsl #2]
003d8788: bl       #0x3da8b8
003d878c: mov      r0, r6
003d8790: b        #0x3d8700
003d8794: ldr      r0, [r4, #4]
003d8798: mov      r1, #0xd8
003d879c: mov      r2, #1
003d87a0: add      r0, r0, #0x560
003d87a4: bl       #0x3e0798
003d87a8: ldr      r3, [pc, #0xa4]
003d87ac: ldr      r0, [r4, #4]
003d87b0: mov      r1, #0xd8
003d87b4: ldr      r3, [r5, r3]
003d87b8: add      r0, r0, #0x560
003d87bc: mov      r2, r6
003d87c0: ldr      r7, [r3]
003d87c4: bl       #0x3df6e0
003d87c8: cmp      r0, #0xc7
003d87cc: ble      #0x3d8754
003d87d0: ldr      r3, [pc, #0x80]
003d87d4: ldr      r1, [r4, #4]
003d87d8: ldr      r3, [r5, r3]
003d87dc: ldr      r0, [r3, #0x40]
003d87e0: bl       #0x36effc
003d87e4: cmp      r0, r6
003d87e8: beq      #0x3d8754
003d87ec: ldr      r3, [pc, #0x68]
003d87f0: ldr      r3, [r5, r3]
003d87f4: ldr      sl, [r3]
003d87f8: cmp      sl, r6
003d87fc: beq      #0x3d8848
003d8800: ldr      r3, [pc, #0x58]
003d8804: ldr      r8, [pc, #0x58]
003d8808: ldr      r3, [r5, r3]
003d880c: add      r8, pc, r8
003d8810: ldr      r5, [r3]
003d8814: b        #0x3d8824
003d8818: add      r6, r6, #1
003d881c: cmp      r6, sl
003d8820: beq      #0x3d8848
003d8824: ldr      r1, [r5, r6, lsl #2]
003d8828: mov      r0, r8
003d882c: bl       #0x30e31c
003d8830: cmp      r0, #0
003d8834: bne      #0x3d8818
003d8838: mov      r1, r6
003d883c: mov      r0, r7
003d8840: bl       #0x3813b8
003d8844: b        #0x3d8754
003d8848: mvn      r1, #0
003d884c: b        #0x3d883c
003d8850: subseq   ip, fp, ip, lsr #7
003d8854: andeq    r1, r0, r0, ror sp
003d8858: strdeq   r3, r4, [r0], -r4
003d885c: strdeq   r0, r1, [r0], -ip
003d8860: andeq    r1, r0, ip, lsr #32
003d8864: strheq   ip, [lr], #-0xfc

# _ZN17CharAISkillScript10OnPreSkillEv
003da8b8: push     {r4, r5, r6, r7, lr}
003da8bc: ldr      r4, [pc, #0x108]
003da8c0: ldr      r7, [pc, #0x108]
003da8c4: sub      sp, sp, #0x34
003da8c8: add      r4, pc, r4
003da8cc: ldr      r3, [r4, r7]
003da8d0: add      r5, sp, #4
003da8d4: mov      r6, r0
003da8d8: ldr      r3, [r3]
003da8dc: mov      r0, r5
003da8e0: str      r3, [sp, #0x2c]
003da8e4: bl       #0x31b434
003da8e8: ldr      r3, [r6, #4]
003da8ec: ldr      r0, [r3, #0x3e4]
003da8f0: cmp      r0, #0
003da8f4: beq      #0x3da918
003da8f8: ldr      r1, [pc, #0xd4]
003da8fc: mov      r3, r5
003da900: add      r2, r6, #0xc
003da904: add      r1, pc, r1
003da908: bl       #0x37c390
003da90c: ldr      r3, [sp, #0xc]
003da910: cmp      r3, #0
003da914: beq      #0x3da944
003da918: mov      r6, #0
003da91c: mov      r0, r5
003da920: bl       #0x31b398
003da924: ldr      r3, [r4, r7]
003da928: ldr      r2, [sp, #0x2c]
003da92c: mov      r0, r6
003da930: ldr      r3, [r3]
003da934: cmp      r2, r3
003da938: bne      #0x3da9c8
003da93c: add      sp, sp, #0x34
003da940: pop      {r4, r5, r6, r7, pc}
003da944: ldr      r0, [sp, #0x28]
003da948: ldm      r0, {r1, r2}
003da94c: cmp      r1, r2
003da950: beq      #0x3da95c
003da954: mov      r3, sp
003da958: bl       #0x31c3cc
003da95c: ldr      r3, [r6, #4]
003da960: ldr      r1, [pc, #0x70]
003da964: mov      r2, r5
003da968: ldr      r0, [r3, #0x3e4]
003da96c: add      r1, pc, r1
003da970: bl       #0x37c494
003da974: ldr      r1, [sp, #0xc]
003da978: cmp      r1, #0
003da97c: bne      #0x3da918
003da980: ldr      r2, [sp, #0x28]
003da984: ldr      r3, [r2]
003da988: ldr      r2, [r2, #4]
003da98c: rsb      r3, r3, r2
003da990: asr      r3, r3, #4
003da994: add      r2, r3, r3, lsl #3
003da998: add      r2, r2, r2, lsl #6
003da99c: add      r2, r3, r2, lsl #3
003da9a0: add      r2, r2, r2, lsl #15
003da9a4: add      r3, r3, r2, lsl #3
003da9a8: cmp      r3, #0
003da9ac: moveq    r6, #1
003da9b0: beq      #0x3da91c
003da9b4: mov      r0, r5
003da9b8: bl       #0x3da43c
003da9bc: bl       #0x31bc80
003da9c0: mov      r6, r0
003da9c4: b        #0x3da91c
003da9c8: bl       #0x30e310
003da9cc: subseq   sl, fp, r8, asr #3
003da9d0: andeq    r4, r0, ip, lsr #1
003da9d4: subeq    sl, lr, ip, asr #30
003da9d8: subeq    sl, lr, ip, lsl #30

# _ZN9Character11_RemoveBuffERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b842c: push     {r4, r5, lr}
003b8430: ldr      r3, [r0, #4]
003b8434: ldr      r4, [pc, #0x128]
003b8438: sub      sp, sp, #0xc
003b843c: ldr      r1, [r3, #4]
003b8440: ldr      ip, [r3]
003b8444: add      r4, pc, r4
003b8448: mov      r5, r0
003b844c: rsb      r3, ip, r1
003b8450: asr      r3, r3, #4
003b8454: add      r1, r3, r3, lsl #3
003b8458: add      r1, r1, r1, lsl #6
003b845c: add      r1, r3, r1, lsl #3
003b8460: add      r1, r1, r1, lsl #15
003b8464: add      r3, r3, r1, lsl #3
003b8468: cmp      r3, #0
003b846c: bne      #0x3b8478
003b8470: add      sp, sp, #0xc
003b8474: pop      {r4, r5, pc}
003b8478: ldr      r3, [ip, #4]
003b847c: cmp      r3, #3
003b8480: bne      #0x3b8470
003b8484: mov      r1, #0
003b8488: str      r2, [sp, #4]
003b848c: bl       #0x37baf8
003b8490: bl       #0x38d798
003b8494: ldr      r3, [pc, #0xcc]
003b8498: ldr      r2, [sp, #4]
003b849c: ldr      r3, [r4, r3]
003b84a0: ldr      r3, [r3]
003b84a4: cmp      r0, r3
003b84a8: bhs      #0x3b8470
003b84ac: ldr      r3, [r5, #4]
003b84b0: ldr      r1, [r3, #4]
003b84b4: ldr      r3, [r3]
003b84b8: rsb      r1, r3, r1
003b84bc: asr      r1, r1, #4
003b84c0: add      r0, r1, r1, lsl #3
003b84c4: add      r0, r0, r0, lsl #6
003b84c8: add      r0, r1, r0, lsl #3
003b84cc: add      r0, r0, r0, lsl #15
003b84d0: add      r1, r1, r0, lsl #3
003b84d4: rsb      r1, r1, #0
003b84d8: cmp      r1, #1
003b84dc: bls      #0x3b8534
003b84e0: ldr      r3, [r3, #0x74]
003b84e4: cmp      r3, #2
003b84e8: bne      #0x3b8470
003b84ec: mov      r1, #0
003b84f0: mov      r0, r5
003b84f4: str      r2, [sp, #4]
003b84f8: bl       #0x37baf8
003b84fc: bl       #0x38d798
003b8500: mov      r1, #1
003b8504: mov      r4, r0
003b8508: mov      r0, r5
003b850c: bl       #0x37baf8
003b8510: bl       #0x31b580
003b8514: ldr      r2, [sp, #4]
003b8518: mov      r3, r0
003b851c: mov      r1, r4
003b8520: add      r0, r2, #0x560
003b8524: mov      r2, r3
003b8528: add      sp, sp, #0xc
003b852c: pop      {r4, r5, lr}
003b8530: b        #0x3e101c
003b8534: mov      r1, #0
003b8538: mov      r0, r5
003b853c: str      r2, [sp, #4]
003b8540: bl       #0x37baf8
003b8544: bl       #0x38d798
003b8548: ldr      r2, [sp, #4]
003b854c: mov      r1, r0
003b8550: add      r0, r2, #0x560
003b8554: mov      r2, #0
003b8558: add      sp, sp, #0xc
003b855c: pop      {r4, r5, lr}
003b8560: b        #0x3e101c
003b8564: subseq   ip, sp, ip, asr #12
003b8568: andeq    r3, r0, r8, ror #10

# _ZN17CharAISkillScript19OnSkillCheck_ActiveEv
003db16c: push     {r4, r5, r6, r7, lr}
003db170: ldr      r4, [pc, #0x100]
003db174: ldr      r7, [pc, #0x100]
003db178: sub      sp, sp, #0x34
003db17c: add      r4, pc, r4
003db180: ldr      r3, [r4, r7]
003db184: add      r5, sp, #4
003db188: mov      r6, r0
003db18c: ldr      r3, [r3]
003db190: mov      r0, r5
003db194: str      r3, [sp, #0x2c]
003db198: bl       #0x31b434
003db19c: ldr      r3, [r6, #4]
003db1a0: ldr      r0, [r3, #0x3e4]
003db1a4: cmp      r0, #0
003db1a8: beq      #0x3db1cc
003db1ac: ldr      r1, [pc, #0xcc]
003db1b0: mov      r3, r5
003db1b4: add      r2, r6, #0xc
003db1b8: add      r1, pc, r1
003db1bc: bl       #0x37c390
003db1c0: ldr      r3, [sp, #0xc]
003db1c4: cmp      r3, #0
003db1c8: beq      #0x3db1f8
003db1cc: mov      r6, #0
003db1d0: mov      r0, r5
003db1d4: bl       #0x31b398
003db1d8: ldr      r3, [r4, r7]
003db1dc: ldr      r2, [sp, #0x2c]
003db1e0: mov      r0, r6
003db1e4: ldr      r3, [r3]
003db1e8: cmp      r2, r3
003db1ec: bne      #0x3db274
003db1f0: add      sp, sp, #0x34
003db1f4: pop      {r4, r5, r6, r7, pc}
003db1f8: ldr      r0, [sp, #0x28]
003db1fc: ldm      r0, {r1, r2}
003db200: cmp      r1, r2
003db204: beq      #0x3db210
003db208: mov      r3, sp
003db20c: bl       #0x31c3cc
003db210: ldr      r3, [r6, #4]
003db214: ldr      r1, [pc, #0x68]
003db218: mov      r2, r5
003db21c: ldr      r0, [r3, #0x3e4]
003db220: add      r1, pc, r1
003db224: bl       #0x37c494
003db228: ldr      r3, [sp, #0xc]
003db22c: cmp      r3, #0
003db230: bne      #0x3db1cc
003db234: ldr      r2, [sp, #0x28]
003db238: ldm      r2, {r0, r3}
003db23c: rsb      r3, r0, r3
003db240: asr      r3, r3, #4
003db244: add      r2, r3, r3, lsl #3
003db248: add      r2, r2, r2, lsl #6
003db24c: add      r2, r3, r2, lsl #3
003db250: add      r2, r2, r2, lsl #15
003db254: add      r3, r3, r2, lsl #3
003db258: rsb      r3, r3, #0
003db25c: cmp      r3, #1
003db260: bls      #0x3db1cc
003db264: add      r0, r0, #0x70
003db268: bl       #0x31bc80
003db26c: mov      r6, r0
003db270: b        #0x3db1d0
003db274: bl       #0x30e310
003db278: subseq   sb, fp, r4, lsl sb
003db27c: andeq    r4, r0, ip, lsr #1
003db280: umaaleq  sl, lr, r8, r6
003db284: subeq    sl, lr, r8, ror #12

# _ZN6CharAI11AI_UseSkillEj
003d8868: push     {r4, r5, r6, lr}
003d886c: mov      r5, r0
003d8870: mov      r4, r1
003d8874: bl       #0x3d86bc
003d8878: cmp      r0, #0
003d887c: beq      #0x3d8890
003d8880: mov      r0, r5
003d8884: mov      r1, r4
003d8888: bl       #0x3d8474
003d888c: mov      r0, #1
003d8890: pop      {r4, r5, r6, pc}

# _ZN17CharAISkillScript19OnSkillCheck_UsableEv
003da9dc: push     {r4, r5, r6, r7, lr}
003da9e0: ldr      r4, [pc, #0x104]
003da9e4: ldr      r7, [pc, #0x104]
003da9e8: sub      sp, sp, #0x34
003da9ec: add      r4, pc, r4
003da9f0: ldr      r3, [r4, r7]
003da9f4: add      r5, sp, #4
003da9f8: mov      r6, r0
003da9fc: ldr      r3, [r3]
003daa00: mov      r0, r5
003daa04: str      r3, [sp, #0x2c]
003daa08: bl       #0x31b434
003daa0c: ldr      r3, [r6, #4]
003daa10: ldr      r0, [r3, #0x3e4]
003daa14: cmp      r0, #0
003daa18: beq      #0x3daa3c
003daa1c: ldr      r1, [pc, #0xd0]
003daa20: mov      r3, r5
003daa24: add      r2, r6, #0xc
003daa28: add      r1, pc, r1
003daa2c: bl       #0x37c390
003daa30: ldr      r3, [sp, #0xc]
003daa34: cmp      r3, #0
003daa38: beq      #0x3daa68
003daa3c: mov      r6, #0
003daa40: mov      r0, r5
003daa44: bl       #0x31b398
003daa48: ldr      r3, [r4, r7]
003daa4c: ldr      r2, [sp, #0x2c]
003daa50: mov      r0, r6
003daa54: ldr      r3, [r3]
003daa58: cmp      r2, r3
003daa5c: bne      #0x3daae8
003daa60: add      sp, sp, #0x34
003daa64: pop      {r4, r5, r6, r7, pc}
003daa68: ldr      r0, [sp, #0x28]
003daa6c: ldm      r0, {r1, r2}
003daa70: cmp      r1, r2
003daa74: beq      #0x3daa80
003daa78: mov      r3, sp
003daa7c: bl       #0x31c3cc
003daa80: ldr      r3, [r6, #4]
003daa84: ldr      r1, [pc, #0x6c]
003daa88: mov      r2, r5
003daa8c: ldr      r0, [r3, #0x3e4]
003daa90: add      r1, pc, r1
003daa94: bl       #0x37c494
003daa98: ldr      r1, [sp, #0xc]
003daa9c: cmp      r1, #0
003daaa0: bne      #0x3daa3c
003daaa4: ldr      r2, [sp, #0x28]
003daaa8: ldr      r3, [r2]
003daaac: ldr      r2, [r2, #4]
003daab0: rsb      r3, r3, r2
003daab4: asr      r3, r3, #4
003daab8: add      r2, r3, r3, lsl #3
003daabc: add      r2, r2, r2, lsl #6
003daac0: add      r2, r3, r2, lsl #3
003daac4: add      r2, r2, r2, lsl #15
003daac8: add      r3, r3, r2, lsl #3
003daacc: cmp      r3, #0
003daad0: beq      #0x3daa3c
003daad4: mov      r0, r5
003daad8: bl       #0x3da43c
003daadc: bl       #0x31bc80
003daae0: mov      r6, r0
003daae4: b        #0x3daa40
003daae8: bl       #0x30e310
003daaec: subseq   sl, fp, r4, lsr #1
003daaf0: andeq    r4, r0, ip, lsr #1
003daaf4: subeq    sl, lr, r8, lsr #28
003daaf8: strdeq   sl, fp, [lr], #-0xd8

# _ZN17CharAISkillScript11OnPostSkillEv
003da6c0: push     {r4, r5, r6, r7, lr}
003da6c4: ldr      r4, [pc, #0xb8]
003da6c8: ldr      r7, [pc, #0xb8]
003da6cc: sub      sp, sp, #0x34
003da6d0: add      r4, pc, r4
003da6d4: ldr      r3, [r4, r7]
003da6d8: add      r5, sp, #4
003da6dc: mov      r6, r0
003da6e0: ldr      r3, [r3]
003da6e4: mov      r0, r5
003da6e8: str      r3, [sp, #0x2c]
003da6ec: bl       #0x31b434
003da6f0: ldr      r3, [r6, #4]
003da6f4: ldr      r0, [r3, #0x3e4]
003da6f8: cmp      r0, #0
003da6fc: beq      #0x3da720
003da700: ldr      r1, [pc, #0x84]
003da704: mov      r3, r5
003da708: add      r2, r6, #0xc
003da70c: add      r1, pc, r1
003da710: bl       #0x37c390
003da714: ldr      r3, [sp, #0xc]
003da718: cmp      r3, #0
003da71c: beq      #0x3da744
003da720: mov      r0, r5
003da724: bl       #0x31b398
003da728: ldr      r3, [r4, r7]
003da72c: ldr      r2, [sp, #0x2c]
003da730: ldr      r3, [r3]
003da734: cmp      r2, r3
003da738: bne      #0x3da780
003da73c: add      sp, sp, #0x34
003da740: pop      {r4, r5, r6, r7, pc}
003da744: ldr      r0, [sp, #0x28]
003da748: ldm      r0, {r1, r2}
003da74c: cmp      r1, r2
003da750: beq      #0x3da75c
003da754: mov      r3, sp
003da758: bl       #0x31c3cc
003da75c: ldr      r3, [r6, #4]
003da760: ldr      r1, [pc, #0x28]
003da764: mov      r2, r5
003da768: ldr      r0, [r3, #0x3e4]
003da76c: add      r1, pc, r1
003da770: bl       #0x37c494
003da774: mov      r0, r5
003da778: bl       #0x31b398
003da77c: b        #0x3da728
003da780: bl       #0x30e310
003da784: subseq   sl, fp, r0, asr #7
003da788: andeq    r4, r0, ip, lsr #1
003da78c: subeq    fp, lr, r4, asr #2
003da790: strdeq   fp, ip, [lr], #-4

# _ZN6CharAI11AI_EndSkillEj
003d8474: push     {r4, r5, r6, lr}
003d8478: mov      r4, r0
003d847c: ldr      r0, [r0, #4]
003d8480: mov      r5, r1
003d8484: add      r0, r0, #0x4f0
003d8488: add      r0, r0, #0xc
003d848c: bl       #0x3c02e8
003d8490: cmp      r0, #0
003d8494: bne      #0x3d849c
003d8498: pop      {r4, r5, r6, pc}
003d849c: mov      r1, r5
003d84a0: ldr      r0, [r4, #4]
003d84a4: bl       #0x3bc784
003d84a8: ldr      r3, [r0, #0x48]
003d84ac: cmp      r3, #2
003d84b0: bne      #0x3d8498
003d84b4: ldrb     r3, [r4, #0xd0]
003d84b8: cmp      r3, #0
003d84bc: moveq    r3, #1
003d84c0: strbeq   r3, [r4, #0xd1]
003d84c4: beq      #0x3d8498
003d84c8: ldr      r0, [r4, #4]
003d84cc: mov      r1, #1
003d84d0: add      r0, r0, #0x490
003d84d4: add      r0, r0, #0xc
003d84d8: pop      {r4, r5, r6, lr}
003d84dc: b        #0x3c948c

# _ZNK6CharAI16AI_IsSkillActiveEj
003d85d4: push     {r4, r5, lr}
003d85d8: mov      r4, r0
003d85dc: ldr      r2, [r4, #0xb4]
003d85e0: ldr      r0, [r0, #0xb8]
003d85e4: ldr      r3, [pc, #0xb8]
003d85e8: sub      sp, sp, #0xc
003d85ec: rsb      r2, r2, r0
003d85f0: cmp      r1, r2, asr #2
003d85f4: mov      r5, r1
003d85f8: add      r3, pc, r3
003d85fc: blo      #0x3d8624
003d8600: ldr      r2, [pc, #0xa0]
003d8604: ldr      r2, [r3, r2]
003d8608: ldr      r2, [r2]
003d860c: cmp      r2, #2
003d8610: moveq    r3, #0
003d8614: streq    r3, [r3]
003d8618: beq      #0x3d8624
003d861c: cmp      r2, #1
003d8620: beq      #0x3d8670
003d8624: ldr      r0, [r4, #4]
003d8628: add      r0, r0, #0x4f0
003d862c: add      r0, r0, #0xc
003d8630: bl       #0x3c02e8
003d8634: cmp      r0, #0
003d8638: beq      #0x3d864c
003d863c: ldr      r3, [r4, #0xcc]
003d8640: cmp      r5, r3
003d8644: moveq    r0, #1
003d8648: beq      #0x3d8668
003d864c: ldr      r3, [r4, #0xb4]
003d8650: ldr      r0, [r3, r5, lsl #2]
003d8654: cmp      r0, #0
003d8658: beq      #0x3d8668
003d865c: add      sp, sp, #0xc
003d8660: pop      {r4, r5, lr}
003d8664: b        #0x3db16c
003d8668: add      sp, sp, #0xc
003d866c: pop      {r4, r5, pc}
003d8670: ldr      r0, [pc, #0x34]
003d8674: ldr      r1, [pc, #0x34]
003d8678: ldr      r2, [pc, #0x34]
003d867c: ldr      r0, [r3, r0]
003d8680: ldr      r3, [pc, #0x30]
003d8684: mov      ip, #0xc7
003d8688: add      r1, pc, r1
003d868c: add      r2, pc, r2
003d8690: add      r3, pc, r3
003d8694: add      r0, r0, #0xa8
003d8698: str      ip, [sp]
003d869c: bl       #0x30e004
003d86a0: b        #0x3d8624

# _ZN9Character8_UseManaERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b7864: push     {r4, lr}
003b7868: ldr      r3, [r0, #4]
003b786c: mov      r4, r1
003b7870: sub      sp, sp, #8
003b7874: ldr      r1, [r3, #4]
003b7878: ldr      ip, [r3]
003b787c: rsb      r3, ip, r1
003b7880: asr      r3, r3, #4
003b7884: add      r1, r3, r3, lsl #3
003b7888: add      r1, r1, r1, lsl #6
003b788c: add      r1, r3, r1, lsl #3
003b7890: add      r1, r1, r1, lsl #15
003b7894: add      r3, r3, r1, lsl #3
003b7898: cmp      r3, #0
003b789c: bne      #0x3b78a8
003b78a0: add      sp, sp, #8
003b78a4: pop      {r4, pc}
003b78a8: ldr      r3, [ip, #4]
003b78ac: cmp      r3, #3
003b78b0: bne      #0x3b78a0
003b78b4: mov      r1, #0
003b78b8: str      r2, [sp, #4]
003b78bc: bl       #0x37baf8
003b78c0: bl       #0x31bbf0
003b78c4: bl       #0x30e4cc
003b78c8: ldr      r2, [sp, #4]
003b78cc: mov      r1, r0
003b78d0: mov      r0, r2
003b78d4: bl       #0x3bdef4
003b78d8: mov      r1, r0
003b78dc: mov      r0, r4
003b78e0: add      sp, sp, #8
003b78e4: pop      {r4, lr}
003b78e8: b        #0x37c7e4

# _ZN9Character11_CreateBuffERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b86a8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b86ac: ldr      r3, [r0, #4]
003b86b0: mov      r4, r1
003b86b4: mov      r8, r2
003b86b8: ldr      r1, [r3, #4]
003b86bc: ldr      r3, [r3]
003b86c0: ldr      r5, [pc, #0x4f8]
003b86c4: sub      sp, sp, #0x14
003b86c8: rsb      r1, r3, r1
003b86cc: asr      r1, r1, #4
003b86d0: add      r5, pc, r5
003b86d4: add      r2, r1, r1, lsl #3
003b86d8: mov      r6, r0
003b86dc: add      r2, r2, r2, lsl #6
003b86e0: add      r2, r1, r2, lsl #3
003b86e4: add      r2, r2, r2, lsl #15
003b86e8: add      r1, r1, r2, lsl #3
003b86ec: cmp      r1, #0
003b86f0: bne      #0x3b86fc
003b86f4: add      sp, sp, #0x14
003b86f8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b86fc: ldr      r3, [r3, #4]
003b8700: cmp      r3, #3
003b8704: bne      #0x3b86f4
003b8708: mov      r1, #0
003b870c: bl       #0x37baf8
003b8710: bl       #0x38d798
003b8714: ldr      r3, [pc, #0x4a8]
003b8718: ldr      r3, [r5, r3]
003b871c: ldr      r3, [r3]
003b8720: cmp      r0, r3
003b8724: bhs      #0x3b86f4
003b8728: ldr      r7, [r6, #4]
003b872c: ldm      r7, {r0, r3}
003b8730: rsb      r3, r0, r3
003b8734: asr      r3, r3, #4
003b8738: add      r2, r3, r3, lsl #3
003b873c: add      r2, r2, r2, lsl #6
003b8740: add      r2, r3, r2, lsl #3
003b8744: add      r2, r2, r2, lsl #15
003b8748: add      r3, r3, r2, lsl #3
003b874c: cmp      r3, #0
003b8750: bne      #0x3b8764
003b8754: ldr      r0, [pc, #0x46c]
003b8758: add      r0, pc, r0
003b875c: bl       #0x708eb0
003b8760: ldr      r0, [r7]
003b8764: bl       #0x31bbf0
003b8768: bl       #0x30e4cc
003b876c: ldr      r3, [r6, #4]
003b8770: mov      sb, r0
003b8774: ldm      r3, {r2, r3}
003b8778: rsb      r2, r2, r3
003b877c: asr      r2, r2, #4
003b8780: add      r3, r2, r2, lsl #3
003b8784: add      r3, r3, r3, lsl #6
003b8788: add      r3, r2, r3, lsl #3
003b878c: add      r3, r3, r3, lsl #15
003b8790: add      r3, r2, r3, lsl #3
003b8794: rsb      r3, r3, #0
003b8798: cmp      r3, #1
003b879c: bls      #0x3b87e4
003b87a0: mov      r0, r6
003b87a4: mov      r1, #1
003b87a8: bl       #0x37baf8
003b87ac: ldr      r3, [r0, #4]
003b87b0: cmp      r3, #0
003b87b4: bne      #0x3b8964
003b87b8: ldr      r3, [r6, #4]
003b87bc: ldr      r2, [r3, #4]
003b87c0: ldr      r3, [r3]
003b87c4: rsb      r2, r3, r2
003b87c8: asr      r2, r2, #4
003b87cc: add      r3, r2, r2, lsl #3
003b87d0: add      r3, r3, r3, lsl #6
003b87d4: add      r3, r2, r3, lsl #3
003b87d8: add      r3, r3, r3, lsl #15
003b87dc: add      r3, r2, r3, lsl #3
003b87e0: rsb      r3, r3, #0
003b87e4: mov      sl, #0
003b87e8: cmp      r3, #2
003b87ec: bhi      #0x3b8898
003b87f0: mov      r7, #1
003b87f4: cmp      r3, #3
003b87f8: bhi      #0x3b88e0
003b87fc: mov      fp, #0
003b8800: cmp      r3, #4
003b8804: bhi      #0x3b8850
003b8808: mvn      r5, #0
003b880c: cmp      r3, #5
003b8810: bhi      #0x3b8928
003b8814: ldr      ip, [pc, #0x3b0]
003b8818: add      ip, pc, ip
003b881c: mov      r1, sb
003b8820: add      r0, r8, #0x560
003b8824: mov      r2, sl
003b8828: mov      r3, r7
003b882c: str      fp, [sp]
003b8830: stmib    sp, {r5, ip}
003b8834: bl       #0x3e232c
003b8838: subs     r1, r0, #0
003b883c: beq      #0x3b86f4
003b8840: mov      r0, r4
003b8844: add      sp, sp, #0x14
003b8848: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b884c: b        #0x38eb00
003b8850: mov      r0, r6
003b8854: mov      r1, #4
003b8858: bl       #0x37baf8
003b885c: ldr      r3, [r0, #4]
003b8860: cmp      r3, #0
003b8864: bne      #0x3b8a10
003b8868: ldr      r2, [r6, #4]
003b886c: ldr      r1, [r2, #4]
003b8870: ldr      r3, [r2]
003b8874: rsb      r3, r3, r1
003b8878: asr      r3, r3, #4
003b887c: add      r2, r3, r3, lsl #3
003b8880: add      r2, r2, r2, lsl #6
003b8884: add      r2, r3, r2, lsl #3
003b8888: add      r2, r2, r2, lsl #15
003b888c: add      r3, r3, r2, lsl #3
003b8890: rsb      r3, r3, #0
003b8894: b        #0x3b8808
003b8898: mov      r0, r6
003b889c: mov      r1, #2
003b88a0: bl       #0x37baf8
003b88a4: ldr      r3, [r0, #4]
003b88a8: cmp      r3, #0
003b88ac: bne      #0x3b89b0
003b88b0: ldr      r2, [r6, #4]
003b88b4: ldr      r1, [r2, #4]
003b88b8: ldr      r3, [r2]
003b88bc: rsb      r3, r3, r1
003b88c0: asr      r3, r3, #4
003b88c4: add      r2, r3, r3, lsl #3
003b88c8: add      r2, r2, r2, lsl #6
003b88cc: add      r2, r3, r2, lsl #3
003b88d0: add      r2, r2, r2, lsl #15
003b88d4: add      r3, r3, r2, lsl #3
003b88d8: rsb      r3, r3, #0
003b88dc: b        #0x3b87f0
003b88e0: mov      r0, r6
003b88e4: mov      r1, #3
003b88e8: bl       #0x37baf8
003b88ec: ldr      r3, [r0, #4]
003b88f0: cmp      r3, #0
003b88f4: bne      #0x3b8ac8
003b88f8: ldr      r2, [r6, #4]
003b88fc: ldr      r1, [r2, #4]
003b8900: ldr      r3, [r2]
003b8904: rsb      r3, r3, r1
003b8908: asr      r3, r3, #4
003b890c: add      r2, r3, r3, lsl #3
003b8910: add      r2, r2, r2, lsl #6
003b8914: add      r2, r3, r2, lsl #3
003b8918: add      r2, r2, r2, lsl #15
003b891c: add      r3, r3, r2, lsl #3
003b8920: rsb      r3, r3, #0
003b8924: b        #0x3b87fc
003b8928: mov      r0, r6
003b892c: mov      r1, #5
003b8930: bl       #0x37baf8
003b8934: ldr      r3, [r0, #4]
003b8938: cmp      r3, #0
003b893c: beq      #0x3b8814
003b8940: mov      r0, r6
003b8944: mov      r1, #5
003b8948: bl       #0x37baf8
003b894c: ldr      r3, [r0, #4]
003b8950: cmp      r3, #4
003b8954: beq      #0x3b8b84
003b8958: ldr      ip, [pc, #0x270]
003b895c: add      ip, pc, ip
003b8960: b        #0x3b881c
003b8964: mov      r0, r6
003b8968: mov      r1, #1
003b896c: bl       #0x37baf8
003b8970: ldr      r3, [r0, #4]
003b8974: cmp      r3, #3
003b8978: beq      #0x3b8b40
003b897c: ldr      r3, [r6, #4]
003b8980: mov      sl, #0
003b8984: ldr      r2, [r3, #4]
003b8988: ldr      r3, [r3]
003b898c: rsb      r2, r3, r2
003b8990: asr      r2, r2, #4
003b8994: add      r3, r2, r2, lsl #3
003b8998: add      r3, r3, r3, lsl #6
003b899c: add      r3, r2, r3, lsl #3
003b89a0: add      r3, r3, r3, lsl #15
003b89a4: add      r3, r2, r3, lsl #3
003b89a8: rsb      r3, r3, #0
003b89ac: b        #0x3b87e8
003b89b0: mov      r0, r6
003b89b4: mov      r1, #2
003b89b8: bl       #0x37baf8
003b89bc: ldr      r7, [r0, #4]
003b89c0: cmp      r7, #1
003b89c4: beq      #0x3b8b14
003b89c8: mov      r1, #2
003b89cc: mov      r0, r6
003b89d0: bl       #0x37baf8
003b89d4: bl       #0x31bbf0
003b89d8: bl       #0x30e4cc
003b89dc: ldr      r2, [r6, #4]
003b89e0: mov      r7, r0
003b89e4: ldr      r1, [r2, #4]
003b89e8: ldr      r3, [r2]
003b89ec: rsb      r3, r3, r1
003b89f0: asr      r3, r3, #4
003b89f4: add      r2, r3, r3, lsl #3
003b89f8: add      r2, r2, r2, lsl #6
003b89fc: add      r2, r3, r2, lsl #3
003b8a00: add      r2, r2, r2, lsl #15
003b8a04: add      r3, r3, r2, lsl #3
003b8a08: rsb      r3, r3, #0
003b8a0c: b        #0x3b87f4
003b8a10: mov      r0, r6
003b8a14: mov      r1, #4
003b8a18: bl       #0x37baf8
003b8a1c: ldr      r3, [r0, #4]
003b8a20: cmp      r3, #3
003b8a24: beq      #0x3b8a80
003b8a28: mov      r1, #4
003b8a2c: mov      r0, r6
003b8a30: bl       #0x37baf8
003b8a34: bl       #0x38d798
003b8a38: ldr      r3, [pc, #0x194]
003b8a3c: ldr      r3, [r5, r3]
003b8a40: ldr      r3, [r3]
003b8a44: cmp      r0, r3
003b8a48: blo      #0x3b8a80
003b8a4c: ldr      r3, [r6, #4]
003b8a50: mvn      r5, #0
003b8a54: ldr      r2, [r3, #4]
003b8a58: ldr      r3, [r3]
003b8a5c: rsb      r3, r3, r2
003b8a60: asr      r3, r3, #4
003b8a64: add      r2, r3, r3, lsl #3
003b8a68: add      r2, r2, r2, lsl #6
003b8a6c: add      r2, r3, r2, lsl #3
003b8a70: add      r2, r2, r2, lsl #15
003b8a74: add      r3, r3, r2, lsl #3
003b8a78: rsb      r3, r3, #0
003b8a7c: b        #0x3b880c
003b8a80: mov      r1, #4
003b8a84: mov      r0, r6
003b8a88: bl       #0x37baf8
003b8a8c: bl       #0x31bbf0
003b8a90: bl       #0x30e4cc
003b8a94: ldr      r3, [r6, #4]
003b8a98: mov      r5, r0
003b8a9c: ldr      r2, [r3, #4]
003b8aa0: ldr      r3, [r3]
003b8aa4: rsb      r2, r3, r2
003b8aa8: asr      r2, r2, #4
003b8aac: add      r3, r2, r2, lsl #3
003b8ab0: add      r3, r3, r3, lsl #6
003b8ab4: add      r3, r2, r3, lsl #3
003b8ab8: add      r3, r3, r3, lsl #15
003b8abc: add      r3, r2, r3, lsl #3
003b8ac0: rsb      r3, r3, #0
003b8ac4: b        #0x3b880c
003b8ac8: mov      r1, #3
003b8acc: mov      r0, r6
003b8ad0: bl       #0x37baf8
003b8ad4: ldr      r1, [r0, #4]
003b8ad8: cmp      r1, #3
003b8adc: beq      #0x3b8b9c
003b8ae0: ldr      r2, [r6, #4]
003b8ae4: mov      fp, #0
003b8ae8: ldr      r1, [r2, #4]
003b8aec: ldr      r3, [r2]
003b8af0: rsb      r3, r3, r1
003b8af4: asr      r3, r3, #4
003b8af8: add      r2, r3, r3, lsl #3
003b8afc: add      r2, r2, r2, lsl #6
003b8b00: add      r2, r3, r2, lsl #3
003b8b04: add      r2, r2, r2, lsl #15
003b8b08: add      r3, r3, r2, lsl #3
003b8b0c: rsb      r3, r3, #0
003b8b10: b        #0x3b8800
003b8b14: mov      r1, #2
003b8b18: mov      r0, r6
003b8b1c: bl       #0x37baf8
003b8b20: bl       #0x31bc80
003b8b24: ldr      r3, [r6, #4]
003b8b28: cmp      r0, #0
003b8b2c: movne    r7, #0
003b8b30: ldr      r2, [r3, #4]
003b8b34: ldr      r3, [r3]
003b8b38: rsb      r3, r3, r2
003b8b3c: b        #0x3b89f0
003b8b40: mov      r1, #1
003b8b44: mov      r0, r6
003b8b48: bl       #0x37baf8
003b8b4c: bl       #0x38d798
003b8b50: ldr      r3, [r6, #4]
003b8b54: mov      sl, r0
003b8b58: ldr      r2, [r3, #4]
003b8b5c: ldr      r3, [r3]
003b8b60: rsb      r3, r3, r2
003b8b64: asr      r3, r3, #4
003b8b68: add      r2, r3, r3, lsl #3
003b8b6c: add      r2, r2, r2, lsl #6
003b8b70: add      r2, r3, r2, lsl #3
003b8b74: add      r2, r2, r2, lsl #15
003b8b78: add      r3, r3, r2, lsl #3
003b8b7c: rsb      r3, r3, #0
003b8b80: b        #0x3b87e8
003b8b84: mov      r1, #5
003b8b88: mov      r0, r6
003b8b8c: bl       #0x37baf8
003b8b90: bl       #0x31c49c
003b8b94: mov      ip, r0
003b8b98: b        #0x3b881c
003b8b9c: mov      r0, r6
003b8ba0: bl       #0x37baf8
003b8ba4: bl       #0x38d798
003b8ba8: ldr      r3, [r6, #4]
003b8bac: mov      fp, r0
003b8bb0: ldr      r2, [r3, #4]
003b8bb4: ldr      r3, [r3]
003b8bb8: rsb      r3, r3, r2
003b8bbc: b        #0x3b8af4
003b8bc0: subseq   ip, sp, r0, asr #7
003b8bc4: andeq    r3, r0, r8, ror #10
003b8bc8: subseq   r5, r0, r0, lsl sp
003b8bcc: ldrsheq  r2, [r1], #-0xf0
003b8bd0: subseq   r2, r1, ip, lsr #29
003b8bd4: andeq    r0, r0, r4, asr #13

# _ZN9Character8_HasManaERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b78ec: push     {r4, lr}
003b78f0: ldr      r3, [r0, #4]
003b78f4: mov      r4, r1
003b78f8: sub      sp, sp, #8
003b78fc: ldr      r1, [r3, #4]
003b7900: ldr      ip, [r3]
003b7904: rsb      r3, ip, r1
003b7908: asr      r3, r3, #4
003b790c: add      r1, r3, r3, lsl #3
003b7910: add      r1, r1, r1, lsl #6
003b7914: add      r1, r3, r1, lsl #3
003b7918: add      r1, r1, r1, lsl #15
003b791c: add      r3, r3, r1, lsl #3
003b7920: cmp      r3, #0
003b7924: bne      #0x3b7930
003b7928: add      sp, sp, #8
003b792c: pop      {r4, pc}
003b7930: ldr      r3, [ip, #4]
003b7934: cmp      r3, #3
003b7938: bne      #0x3b7928
003b793c: mov      r1, #0
003b7940: str      r2, [sp, #4]
003b7944: bl       #0x37baf8
003b7948: bl       #0x31bbf0
003b794c: bl       #0x30e4cc
003b7950: ldr      r2, [sp, #4]
003b7954: mov      r1, r0
003b7958: mov      r0, r2
003b795c: bl       #0x3bd40c
003b7960: mov      r1, r0
003b7964: mov      r0, r4
003b7968: add      sp, sp, #8
003b796c: pop      {r4, lr}
003b7970: b        #0x37c7e4
