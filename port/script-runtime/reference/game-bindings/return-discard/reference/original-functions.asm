
# _ZN9LuaScript4CallEPKcRKN3sfc6script3lua9ArgumentsERNS4_12ReturnValuesE
0037c390: push     {r4, r5, r6, r7, r8, lr}
0037c394: mov      r5, r3
0037c398: ldr      r3, [r3, #0x24]
0037c39c: ldr      r4, [pc, #0x70]
0037c3a0: sub      sp, sp, #8
0037c3a4: ldr      lr, [r3]
0037c3a8: ldr      ip, [r3, #4]
0037c3ac: add      r4, pc, r4
0037c3b0: mov      r6, r0
0037c3b4: cmp      lr, ip
0037c3b8: mov      r7, r1
0037c3bc: mov      r8, r2
0037c3c0: beq      #0x37c3d8
0037c3c4: mov      r0, r3
0037c3c8: mov      r1, lr
0037c3cc: mov      r2, ip
0037c3d0: add      r3, sp, #4
0037c3d4: bl       #0x31c3cc
0037c3d8: mov      r1, r7
0037c3dc: mov      r0, r6
0037c3e0: bl       #0x37c314
0037c3e4: mov      r2, r8
0037c3e8: mov      r1, r0
0037c3ec: mov      r3, r5
0037c3f0: add      r0, r6, #4
0037c3f4: bl       #0x31abe8
0037c3f8: ldr      r3, [pc, #0x18]
0037c3fc: ldr      r3, [r4, r3]
0037c400: ldr      r2, [r3]
0037c404: add      r2, r2, #1
0037c408: str      r2, [r3]
0037c40c: add      sp, sp, #8
0037c410: pop      {r4, r5, r6, r7, r8, pc}
0037c414: rsbeq    r8, r1, r4, ror #13
0037c418: strdeq   r2, r3, [r0], -ip

# _ZN9LuaScript4CallEPKcRKN3sfc6script3lua9ArgumentsE
0037c41c: ldr      r3, [pc, #0x68]
0037c420: ldr      ip, [pc, #0x68]
0037c424: push     {r4, r5, r6, r7, r8, lr}
0037c428: add      r3, pc, r3
0037c42c: ldr      r5, [r3, ip]
0037c430: sub      sp, sp, #0x30
0037c434: mov      r8, r1
0037c438: ldr      r1, [r5]
0037c43c: add      r4, sp, #4
0037c440: mov      r6, r0
0037c444: mov      r7, r2
0037c448: mov      r0, r4
0037c44c: str      r1, [sp, #0x2c]
0037c450: bl       #0x31b434
0037c454: mov      r2, r7
0037c458: mov      r3, r4
0037c45c: mov      r0, r6
0037c460: mov      r1, r8
0037c464: bl       #0x37c390
0037c468: mov      r0, r4
0037c46c: bl       #0x31b398
0037c470: ldr      r2, [sp, #0x2c]
0037c474: ldr      r3, [r5]
0037c478: cmp      r2, r3
0037c47c: bne      #0x37c488
0037c480: add      sp, sp, #0x30
0037c484: pop      {r4, r5, r6, r7, r8, pc}
0037c488: bl       #0x30e310
0037c48c: rsbeq    r8, r1, r8, ror #12
0037c490: andeq    r4, r0, ip, lsr #1

# _ZN3sfc6script3lua12ReturnValues13_addFromStackEP9lua_Statei
0031b4ac: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0031b4b0: ldr      r4, [pc, #0xb8]
0031b4b4: ldr      r6, [pc, #0xb8]
0031b4b8: sub      sp, sp, #0x78
0031b4bc: add      r4, pc, r4
0031b4c0: ldr      r3, [r4, r6]
0031b4c4: ldr      r8, [r0, #0x24]
0031b4c8: add      r5, sp, #4
0031b4cc: ldr      r3, [r3]
0031b4d0: mov      r7, r0
0031b4d4: mov      r0, r5
0031b4d8: mov      sl, r2
0031b4dc: str      r3, [sp, #0x74]
0031b4e0: mov      sb, r1
0031b4e4: bl       #0x3194e0
0031b4e8: mov      r1, r5
0031b4ec: mov      r0, r8
0031b4f0: bl       #0x3195c0
0031b4f4: mov      r0, r5
0031b4f8: bl       #0x3193e8
0031b4fc: ldr      r5, [r7, #0x24]
0031b500: ldm      r5, {r2, r3}
0031b504: rsb      r3, r2, r3
0031b508: asr      r3, r3, #4
0031b50c: add      r7, r3, r3, lsl #3
0031b510: add      r7, r7, r7, lsl #6
0031b514: add      r7, r3, r7, lsl #3
0031b518: add      r7, r7, r7, lsl #15
0031b51c: add      r7, r3, r7, lsl #3
0031b520: rsb      r7, r7, #0
0031b524: subs     r7, r7, #1
0031b528: bhs      #0x31b53c
0031b52c: ldr      r0, [pc, #0x44]
0031b530: add      r0, pc, r0
0031b534: bl       #0x708eb0
0031b538: ldr      r2, [r5]
0031b53c: mov      r0, #0x70
0031b540: mla      r0, r0, r7, r2
0031b544: mov      r1, sb
0031b548: mov      r2, sl
0031b54c: bl       #0x31c9c8
0031b550: ldr      r3, [r4, r6]
0031b554: ldr      r2, [sp, #0x74]
0031b558: ldr      r3, [r3]
0031b55c: cmp      r2, r3
0031b560: bne      #0x31b56c
0031b564: add      sp, sp, #0x78
0031b568: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0031b56c: bl       #0x30e310

# _ZN3sfc6script3lua8Instance5pCallERKNS1_9ArgumentsERNS1_12ReturnValuesE
0031ab4c: push     {r4, r5, r6, r7, r8, lr}
0031ab50: ldr      r3, [r1, #4]
0031ab54: mov      r4, r0
0031ab58: mov      r8, r2
0031ab5c: ldm      r3, {r0, r2}
0031ab60: mov      r5, r1
0031ab64: rsb      r3, r0, r2
0031ab68: asr      r3, r3, #4
0031ab6c: add      r2, r3, r3, lsl #3
0031ab70: add      r2, r2, r2, lsl #6
0031ab74: add      r2, r3, r2, lsl #3
0031ab78: add      r2, r2, r2, lsl #15
0031ab7c: add      r1, r3, r2, lsl #3
0031ab80: rsb      r1, r1, #0
0031ab84: cmp      r1, #0
0031ab88: beq      #0x31abd8
0031ab8c: mov      r6, #0
0031ab90: mov      r7, r6
0031ab94: add      r0, r0, r6
0031ab98: ldr      r1, [r4, #4]
0031ab9c: bl       #0x31cac4
0031aba0: ldr      r2, [r5, #4]
0031aba4: add      r7, r7, #1
0031aba8: add      r6, r6, #0x70
0031abac: ldm      r2, {r0, r3}
0031abb0: rsb      r3, r0, r3
0031abb4: asr      r3, r3, #4
0031abb8: add      r1, r3, r3, lsl #3
0031abbc: add      r1, r1, r1, lsl #6
0031abc0: add      r1, r3, r1, lsl #3
0031abc4: add      r1, r1, r1, lsl #15
0031abc8: add      r1, r3, r1, lsl #3
0031abcc: rsb      r1, r1, #0
0031abd0: cmp      r7, r1
0031abd4: blo      #0x31ab94
0031abd8: mov      r0, r4
0031abdc: mov      r2, r8
0031abe0: pop      {r4, r5, r6, r7, r8, lr}
0031abe4: b        #0x31aa78

# _ZN3sfc6script3lua8Instance6pCall_EjRNS1_12ReturnValuesE
0031aa78: push     {r4, r5, r6, r7, r8, lr}
0031aa7c: mov      r4, r0
0031aa80: ldr      r0, [r0, #4]
0031aa84: mov      r5, r2
0031aa88: mov      r7, r1
0031aa8c: bl       #0x84b12c
0031aa90: ldr      r6, [r4, #4]
0031aa94: mov      r3, #0
0031aa98: mov      r1, r7
0031aa9c: mvn      r2, #0
0031aaa0: mov      r8, r0
0031aaa4: mov      r0, r6
0031aaa8: bl       #0x84bc50
0031aaac: mov      r1, r6
0031aab0: mov      r2, r0
0031aab4: add      r0, r5, #4
0031aab8: bl       #0x31a8ac
0031aabc: ldr      r3, [r5, #8]
0031aac0: cmp      r3, #0
0031aac4: beq      #0x31aacc
0031aac8: pop      {r4, r5, r6, r7, r8, pc}
0031aacc: ldr      r0, [r4, #4]
0031aad0: bl       #0x84b12c
0031aad4: rsb      r7, r7, r8
0031aad8: rsb      r7, r7, #1
0031aadc: add      r7, r7, r0
0031aae0: cmp      r7, #0
0031aae4: ble      #0x31ab04
0031aae8: rsb      r6, r7, #0
0031aaec: mov      r2, r6
0031aaf0: mov      r0, r5
0031aaf4: ldr      r1, [r4, #4]
0031aaf8: bl       #0x31b4ac
0031aafc: adds     r6, r6, #1
0031ab00: bne      #0x31aaec
0031ab04: ldr      r0, [r4, #4]
0031ab08: mvn      r1, r7
0031ab0c: pop      {r4, r5, r6, r7, r8, lr}
0031ab10: b        #0x84b140
