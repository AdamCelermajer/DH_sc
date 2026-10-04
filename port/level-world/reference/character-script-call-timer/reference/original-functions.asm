
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

# _ZN3sfc6script3lua5Value9setNumberEf
0031b5e8: mov      r3, #3
0031b5ec: str      r1, [r0, #8]
0031b5f0: str      r3, [r0, #4]
0031b5f4: bx       lr

# _ZN3sfc6script3lua5ValueC1Ei
0037ca9c: ldr      r2, [pc, #0x78]
0037caa0: ldr      ip, [pc, #0x78]
0037caa4: mov      r3, r0
0037caa8: add      r2, pc, r2
0037caac: ldr      ip, [r2, ip]
0037cab0: push     {r4, r5, r6, lr}
0037cab4: add      ip, ip, #8
0037cab8: mov      r4, r0
0037cabc: str      ip, [r3], #0xc
0037cac0: mov      r5, r1
0037cac4: mov      r0, r3
0037cac8: mov      r1, #0x10
0037cacc: str      r3, [r4, #0x1c]
0037cad0: str      r3, [r4, #0x20]
0037cad4: bl       #0x31167c
0037cad8: ldr      r2, [r4, #0x1c]
0037cadc: add      r3, r4, #0x24
0037cae0: mov      r6, #0
0037cae4: strb     r6, [r2]
0037cae8: mov      r0, r3
0037caec: str      r3, [r4, #0x64]
0037caf0: str      r3, [r4, #0x68]
0037caf4: bl       #0x37be44
0037caf8: ldr      r3, [r4, #0x64]
0037cafc: mov      r0, r5
0037cb00: str      r6, [r3]
0037cb04: bl       #0x30e964
0037cb08: mov      r1, r0
0037cb0c: mov      r0, r4
0037cb10: bl       #0x31b5e8
0037cb14: mov      r0, r4
0037cb18: pop      {r4, r5, r6, pc}
0037cb1c: rsbeq    r7, r1, r8, ror #31
0037cb20: muleq    r0, r8, r7

# _ZNK9LuaScript12_GetFuncNameEPKc
0037c314: push     {r4, r5, r6, lr}
0037c318: mov      r5, r0
0037c31c: mov      r0, r1
0037c320: mov      r4, r1
0037c324: bl       #0x37c164
0037c328: ldr      r3, [r5, #0x38]
0037c32c: add      r5, r5, #0x34
0037c330: cmp      r3, #0
0037c334: beq      #0x37c388
0037c338: mov      r1, r5
0037c33c: b        #0x37c344
0037c340: mov      r3, r2
0037c344: ldr      r2, [r3, #0x10]
0037c348: cmp      r0, r2
0037c34c: ldrhi    r2, [r3, #0xc]
0037c350: ldrls    r2, [r3, #8]
0037c354: movhi    r3, r1
0037c358: mov      r1, r3
0037c35c: cmp      r2, #0
0037c360: bne      #0x37c340
0037c364: cmp      r5, r3
0037c368: beq      #0x37c380
0037c36c: ldr      r2, [r3, #0x10]
0037c370: cmp      r0, r2
0037c374: blo      #0x37c388
0037c378: cmp      r5, r3
0037c37c: ldrne    r4, [r3, #0x28]
0037c380: mov      r0, r4
0037c384: pop      {r4, r5, r6, pc}
0037c388: mov      r3, r5
0037c38c: b        #0x37c378

# _ZN6CharAI13OnScriptTimerEj
003d0ca0: push     {r4, lr}
003d0ca4: ldr      r3, [r0, #0x1c]
003d0ca8: cmp      r3, #0
003d0cac: beq      #0x3d0cc0
003d0cb0: mov      r0, r3
003d0cb4: ldr      r3, [r3]
003d0cb8: mov      lr, pc
003d0cbc: ldr      pc, [r3, #0x90]
003d0cc0: pop      {r4, pc}

# _ZN10AISDefault13OnScriptTimerEj
003dcc80: push     {r4, r5, r6, lr}
003dcc84: sub      sp, sp, #8
003dcc88: mov      r5, r0
003dcc8c: mov      r6, r1
003dcc90: mov      r0, sp
003dcc94: bl       #0x3192b4
003dcc98: mov      r0, sp
003dcc9c: mov      r1, r6
003dcca0: bl       #0x3cdd78
003dcca4: ldr      r1, [pc, #0x20]
003dcca8: mov      r0, r5
003dccac: mov      r2, sp
003dccb0: add      r1, pc, r1
003dccb4: bl       #0x37c41c
003dccb8: mov      r0, sp
003dccbc: mov      r4, sp
003dccc0: bl       #0x319228
003dccc4: add      sp, sp, #8
003dccc8: pop      {r4, r5, r6, pc}
003dcccc: strdeq   r8, sb, [lr], #-0xc0

# _ZN3sfc6script3lua9Arguments11pushIntegerEi
003cdd78: push     {r4, r5, r6, r7, lr}
003cdd7c: ldr      r4, [pc, #0x9c]
003cdd80: ldr      r6, [pc, #0x9c]
003cdd84: sub      sp, sp, #0x7c
003cdd88: add      r4, pc, r4
003cdd8c: ldr      r3, [r4, r6]
003cdd90: add      r5, sp, #4
003cdd94: ldr      r3, [r3]
003cdd98: str      r3, [sp, #0x74]
003cdd9c: ldr      r7, [r0, #4]
003cdda0: mov      r0, r5
003cdda4: bl       #0x37ca9c
003cdda8: mov      r1, r5
003cddac: mov      r0, r7
003cddb0: bl       #0x3195c0
003cddb4: ldr      r3, [pc, #0x6c]
003cddb8: add      r0, r5, #0x24
003cddbc: add      r5, r5, #0xc
003cddc0: ldr      r3, [r4, r3]
003cddc4: add      r3, r3, #8
003cddc8: str      r3, [sp, #4]
003cddcc: bl       #0x3193b0
003cddd0: ldr      r0, [sp, #0x24]
003cddd4: cmp      r0, r5
003cddd8: beq      #0x3cddf8
003cdddc: cmp      r0, #0
003cdde0: beq      #0x3cddf8
003cdde4: ldr      r1, [sp, #0x10]
003cdde8: rsb      r1, r0, r1
003cddec: cmp      r1, #0x80
003cddf0: bhi      #0x3cde14
003cddf4: bl       #0x708f00
003cddf8: ldr      r3, [r4, r6]
003cddfc: ldr      r2, [sp, #0x74]
003cde00: ldr      r3, [r3]
003cde04: cmp      r2, r3
003cde08: bne      #0x3cde1c
003cde0c: add      sp, sp, #0x7c
003cde10: pop      {r4, r5, r6, r7, pc}
003cde14: bl       #0x310440
003cde18: b        #0x3cddf8
003cde1c: bl       #0x30e310
003cde20: subseq   r6, ip, r8, lsl #26
003cde24: andeq    r4, r0, ip, lsr #1
003cde28: muleq    r0, r8, r7
