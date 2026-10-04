
# _ZN9LuaScript19_GetHostPlayerLevelERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037cc00: ldr      r3, [pc, #0x2c]
0037cc04: ldr      r2, [pc, #0x2c]
0037cc08: push     {r4, lr}
0037cc0c: add      r3, pc, r3
0037cc10: ldr      r2, [r3, r2]
0037cc14: mov      r4, r1
0037cc18: ldr      r0, [r2, #0x40]
0037cc1c: bl       #0x36e09c
0037cc20: ldr      r3, [r0, #0x330]
0037cc24: mov      r0, r4
0037cc28: mov      r1, r3
0037cc2c: pop      {r4, lr}
0037cc30: b        #0x37cb24
0037cc34: rsbeq    r7, r1, r4, lsl #29
0037cc38: strdeq   r3, r4, [r0], -r4

# _ZN9LuaScript24_GetHostPlayerDifficultyERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037cb8c: ldr      r3, [pc, #0x3c]
0037cb90: ldr      r2, [pc, #0x3c]
0037cb94: push     {r4, lr}
0037cb98: add      r3, pc, r3
0037cb9c: ldr      r0, [r3, r2]
0037cba0: mov      r4, r1
0037cba4: bl       #0x31f594
0037cba8: subs     r3, r0, #0
0037cbac: beq      #0x37cbc0
0037cbb0: ldr      r1, [r3, #0x118]
0037cbb4: mov      r0, r4
0037cbb8: pop      {r4, lr}
0037cbbc: b        #0x37cb24
0037cbc0: mov      r0, r4
0037cbc4: mov      r1, r3
0037cbc8: pop      {r4, lr}
0037cbcc: b        #0x37cb24

# _ZN9LuaScript21_GetCurrentLevelRangeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037f1f0: push     {r4, r5, r6, r7, r8, lr}
0037f1f4: ldr      r4, [pc, #0x14c]
0037f1f8: ldr      r3, [pc, #0x14c]
0037f1fc: mov      r7, r0
0037f200: add      r4, pc, r4
0037f204: ldr      r0, [r4, r3]
0037f208: mov      r6, r1
0037f20c: bl       #0x31f594
0037f210: ldr      r5, [r0, #0x3c]
0037f214: cmn      r5, #1
0037f218: beq      #0x37f32c
0037f21c: ldr      r2, [r7, #4]
0037f220: ldm      r2, {r0, r3}
0037f224: rsb      r3, r0, r3
0037f228: asr      r3, r3, #4
0037f22c: add      r2, r3, r3, lsl #3
0037f230: add      r2, r2, r2, lsl #6
0037f234: add      r2, r3, r2, lsl #3
0037f238: add      r2, r2, r2, lsl #15
0037f23c: add      r3, r3, r2, lsl #3
0037f240: cmp      r3, #0
0037f244: bne      #0x37f284
0037f248: mov      r3, #0x48
0037f24c: mul      r5, r3, r5
0037f250: ldr      r3, [pc, #0xf8]
0037f254: mov      r0, r6
0037f258: ldr      r4, [r4, r3]
0037f25c: ldr      r3, [r4]
0037f260: add      r3, r3, r5
0037f264: ldr      r1, [r3, #0x3c]
0037f268: bl       #0x37cb24
0037f26c: ldr      r3, [r4]
0037f270: mov      r0, r6
0037f274: add      r5, r3, r5
0037f278: ldr      r1, [r5, #0x30]
0037f27c: pop      {r4, r5, r6, r7, r8, lr}
0037f280: b        #0x37cb24
0037f284: ldr      r3, [r0, #4]
0037f288: cmp      r3, #3
0037f28c: bne      #0x37f248
0037f290: bl       #0x31bbf0
0037f294: bl       #0x30e4cc
0037f298: cmp      r0, #1
0037f29c: beq      #0x37f2b4
0037f2a0: cmp      r0, #2
0037f2a4: beq      #0x37f2f0
0037f2a8: cmp      r0, #0
0037f2ac: beq      #0x37f248
0037f2b0: pop      {r4, r5, r6, r7, r8, pc}
0037f2b4: mov      r3, #0x48
0037f2b8: mul      r5, r3, r5
0037f2bc: ldr      r3, [pc, #0x8c]
0037f2c0: mov      r0, r6
0037f2c4: ldr      r4, [r4, r3]
0037f2c8: ldr      r3, [r4]
0037f2cc: add      r3, r3, r5
0037f2d0: ldr      r1, [r3, #0x40]
0037f2d4: bl       #0x37cb24
0037f2d8: ldr      r3, [r4]
0037f2dc: mov      r0, r6
0037f2e0: add      r5, r3, r5
0037f2e4: ldr      r1, [r5, #0x34]
0037f2e8: pop      {r4, r5, r6, r7, r8, lr}
0037f2ec: b        #0x37cb24
0037f2f0: mov      r3, #0x48
0037f2f4: mul      r5, r3, r5
0037f2f8: ldr      r3, [pc, #0x50]
0037f2fc: mov      r0, r6
0037f300: ldr      r4, [r4, r3]
0037f304: ldr      r3, [r4]
0037f308: add      r3, r3, r5
0037f30c: ldr      r1, [r3, #0x44]
0037f310: bl       #0x37cb24
0037f314: ldr      r3, [r4]
0037f318: mov      r0, r6
0037f31c: add      r5, r3, r5
0037f320: ldr      r1, [r5, #0x38]
0037f324: pop      {r4, r5, r6, r7, r8, lr}
0037f328: b        #0x37cb24
0037f32c: mov      r0, r6
0037f330: mov      r1, r5
0037f334: bl       #0x37cb24
0037f338: mov      r0, r6
0037f33c: mov      r1, r5
0037f340: pop      {r4, r5, r6, r7, r8, lr}
0037f344: b        #0x37cb24
0037f348: mlseq    r1, r0, r8, r5
0037f34c: strdeq   r3, r4, [r0], -r4
0037f350: andeq    r0, r0, r4, ror r8
