
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

# _ZN6CharAI13_SkillCleanUpEv
003d8ae0: push     {r4, r5, r6, lr}
003d8ae4: ldr      r3, [r0, #0xb4]
003d8ae8: ldr      r6, [r0, #0xb8]
003d8aec: mov      r5, r0
003d8af0: rsb      r6, r3, r6
003d8af4: asrs     r6, r6, #2
003d8af8: beq      #0x3d8b24
003d8afc: mov      r4, #0
003d8b00: b        #0x3d8b08
003d8b04: ldr      r3, [r5, #0xb4]
003d8b08: ldr      r0, [r3, r4, lsl #2]
003d8b0c: add      r4, r4, #1
003d8b10: cmp      r0, #0
003d8b14: beq      #0x3d8b1c
003d8b18: bl       #0x3daafc
003d8b1c: cmp      r4, r6
003d8b20: bne      #0x3d8b04
003d8b24: pop      {r4, r5, r6, pc}

# _ZNSs19_M_range_initializeEPKcS0_
003116e8: push     {r4, r5, r6, r7, r8, lr}
003116ec: rsb      r5, r1, r2
003116f0: mov      r4, r1
003116f4: mov      r7, r2
003116f8: add      r1, r5, #1
003116fc: mov      r6, r0
00311700: bl       #0x31167c
00311704: cmp      r7, r4
00311708: ldr      r0, [r6, #0x14]
0031170c: beq      #0x311720
00311710: mov      r1, r4
00311714: mov      r2, r5
00311718: bl       #0x30e868
0031171c: add      r0, r0, r5
00311720: mov      r3, #0
00311724: str      r0, [r6, #0x10]
00311728: strb     r3, [r0]
0031172c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN3sfc6script3lua5Value9setNumberEf
0031b5e8: mov      r3, #3
0031b5ec: str      r1, [r0, #8]
0031b5f0: str      r3, [r0, #4]
0031b5f4: bx       lr

# _ZN6CharAI13_SpellCleanUpEv
003d8a98: push     {r4, r5, r6, lr}
003d8a9c: ldr      r3, [r0, #0xc0]
003d8aa0: ldr      r6, [r0, #0xc4]
003d8aa4: mov      r5, r0
003d8aa8: rsb      r6, r3, r6
003d8aac: asrs     r6, r6, #2
003d8ab0: beq      #0x3d8adc
003d8ab4: mov      r4, #0
003d8ab8: b        #0x3d8ac0
003d8abc: ldr      r3, [r5, #0xc0]
003d8ac0: ldr      r0, [r3, r4, lsl #2]
003d8ac4: add      r4, r4, #1
003d8ac8: cmp      r0, #0
003d8acc: beq      #0x3d8ad4
003d8ad0: bl       #0x3daafc
003d8ad4: cmp      r4, r6
003d8ad8: bne      #0x3d8abc
003d8adc: pop      {r4, r5, r6, pc}

# _ZNSt6vectorIP17CharAISkillScriptSaIS1_EE7reserveEj
003cd584: push     {r4, r5, r6, lr}
003cd588: mov      r4, r0
003cd58c: ldr      r2, [r0]
003cd590: ldr      r0, [r0, #8]
003cd594: sub      sp, sp, #8
003cd598: str      r1, [sp, #4]
003cd59c: rsb      r0, r2, r0
003cd5a0: cmp      r1, r0, asr #2
003cd5a4: bls      #0x3cd610
003cd5a8: cmn      r1, #0xc0000001
003cd5ac: bhi      #0x3cd618
003cd5b0: ldr      r3, [r4, #4]
003cd5b4: cmp      r2, #0
003cd5b8: rsb      r5, r2, r3
003cd5bc: asr      r5, r5, #2
003cd5c0: beq      #0x3cd634
003cd5c4: add      r1, sp, #4
003cd5c8: mov      r0, r4
003cd5cc: bl       #0x3cd310
003cd5d0: mov      r6, r0
003cd5d4: ldr      r0, [r4]
003cd5d8: ldr      r1, [r4, #8]
003cd5dc: cmp      r0, #0
003cd5e0: beq      #0x3cd5f8
003cd5e4: rsb      r1, r0, r1
003cd5e8: bic      r1, r1, #3
003cd5ec: cmp      r1, #0x80
003cd5f0: bhi      #0x3cd62c
003cd5f4: bl       #0x708f00
003cd5f8: ldr      r3, [sp, #4]
003cd5fc: add      r5, r6, r5, lsl #2
003cd600: str      r5, [r4, #4]
003cd604: add      r3, r6, r3, lsl #2
003cd608: str      r3, [r4, #8]
003cd60c: str      r6, [r4]
003cd610: add      sp, sp, #8
003cd614: pop      {r4, r5, r6, pc}
003cd618: ldr      r0, [pc, #0x2c]
003cd61c: add      r0, pc, r0
003cd620: bl       #0x708e40
003cd624: ldr      r2, [r4]
003cd628: b        #0x3cd5b0
003cd62c: bl       #0x310440
003cd630: b        #0x3cd5f8
003cd634: add      r2, sp, #8
003cd638: ldr      r1, [r2, #-4]!
003cd63c: add      r0, r4, #8
003cd640: bl       #0x3cd2a0
003cd644: mov      r6, r0
003cd648: b        #0x3cd5f8
003cd64c: subeq    r0, pc, ip, asr #28

# _ZN9LuaScript4CallEPKc
0037c514: ldr      r3, [pc, #0x60]
0037c518: ldr      r2, [pc, #0x60]
0037c51c: push     {r4, r5, r6, r7, lr}
0037c520: add      r3, pc, r3
0037c524: ldr      r5, [r3, r2]
0037c528: sub      sp, sp, #0x34
0037c52c: add      r4, sp, #4
0037c530: ldr      r2, [r5]
0037c534: mov      r6, r0
0037c538: mov      r7, r1
0037c53c: mov      r0, r4
0037c540: str      r2, [sp, #0x2c]
0037c544: bl       #0x31b434
0037c548: mov      r2, r4
0037c54c: mov      r0, r6
0037c550: mov      r1, r7
0037c554: bl       #0x37c494
0037c558: mov      r0, r4
0037c55c: bl       #0x31b398
0037c560: ldr      r2, [sp, #0x2c]
0037c564: ldr      r3, [r5]
0037c568: cmp      r2, r3
0037c56c: bne      #0x37c578
0037c570: add      sp, sp, #0x34
0037c574: pop      {r4, r5, r6, r7, pc}
0037c578: bl       #0x30e310
0037c57c: rsbeq    r8, r1, r0, ror r5
0037c580: andeq    r4, r0, ip, lsr #1

# _ZN3sfc6script3lua9ArgumentsD1Ev
00319228: ldr      r3, [pc, #0x28]
0031922c: ldr      r2, [pc, #0x28]
00319230: push     {r4, lr}
00319234: add      r3, pc, r3
00319238: ldr      r2, [r3, r2]
0031923c: mov      r4, r0
00319240: ldr      r0, [r0, #4]
00319244: add      r2, r2, #8
00319248: str      r2, [r4]
0031924c: bl       #0x31d194
00319250: mov      r0, r4
00319254: pop      {r4, pc}
00319258: rsbeq    fp, r7, ip, asr r8
0031925c: andeq    r2, r0, r0, ror #6

# _ZN3sfc6script3lua9Arguments10pushStringEPKc
0039ec10: ldr      r3, [pc, #0x58]
0039ec14: ldr      r2, [pc, #0x58]
0039ec18: push     {r4, r5, r6, lr}
0039ec1c: add      r3, pc, r3
0039ec20: ldr      r5, [r3, r2]
0039ec24: sub      sp, sp, #0x78
0039ec28: add      r4, sp, #4
0039ec2c: ldr      r3, [r5]
0039ec30: str      r3, [sp, #0x74]
0039ec34: ldr      r6, [r0, #4]
0039ec38: mov      r0, r4
0039ec3c: bl       #0x37c84c
0039ec40: mov      r0, r6
0039ec44: mov      r1, r4
0039ec48: bl       #0x3195c0
0039ec4c: mov      r0, r4
0039ec50: bl       #0x3193e8
0039ec54: ldr      r2, [sp, #0x74]
0039ec58: ldr      r3, [r5]
0039ec5c: cmp      r2, r3
0039ec60: bne      #0x39ec6c
0039ec64: add      sp, sp, #0x78
0039ec68: pop      {r4, r5, r6, pc}
0039ec6c: bl       #0x30e310
0039ec70: subseq   r5, pc, r4, ror lr
0039ec74: andeq    r4, r0, ip, lsr #1

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

# _ZNSs9_M_assignEPKcS0_
003109e0: push     {r4, r5, r6, r7, r8, lr}
003109e4: mov      r4, r0
003109e8: ldr      r3, [r0, #0x10]
003109ec: ldr      r0, [r0, #0x14]
003109f0: rsb      r5, r1, r2
003109f4: mov      r6, r2
003109f8: rsb      r2, r0, r3
003109fc: cmp      r5, r2
00310a00: mov      r7, r1
00310a04: bhi      #0x310a3c
00310a08: cmp      r5, #0
00310a0c: bne      #0x310a5c
00310a10: add      r2, r0, r5
00310a14: cmp      r2, r3
00310a18: beq      #0x310a34
00310a1c: ldrb     r1, [r3]
00310a20: rsb      r3, r3, r2
00310a24: strb     r1, [r0, r5]
00310a28: ldr      r2, [r4, #0x10]
00310a2c: add      r3, r2, r3
00310a30: str      r3, [r4, #0x10]
00310a34: mov      r0, r4
00310a38: pop      {r4, r5, r6, r7, r8, pc}
00310a3c: cmp      r2, #0
00310a40: bne      #0x310a7c
00310a44: add      r1, r7, r2
00310a48: mov      r0, r4
00310a4c: mov      r2, r6
00310a50: bl       #0x310804
00310a54: mov      r0, r4
00310a58: pop      {r4, r5, r6, r7, r8, pc}
00310a5c: mov      r2, r5
00310a60: bl       #0x30e868
00310a64: ldr      r0, [r4, #0x14]
00310a68: ldr      r3, [r4, #0x10]
00310a6c: add      r2, r0, r5
00310a70: cmp      r2, r3
00310a74: bne      #0x310a1c
00310a78: b        #0x310a34
00310a7c: bl       #0x30e868
00310a80: ldr      r3, [r4, #0x14]
00310a84: ldr      r2, [r4, #0x10]
00310a88: mov      r0, r4
00310a8c: rsb      r2, r3, r2
00310a90: add      r1, r7, r2
00310a94: mov      r2, r6
00310a98: bl       #0x310804
00310a9c: b        #0x310a54

# _ZN3sfc6script3lua5Value9setStringEPKc
0031c46c: mov      r3, #4
0031c470: push     {r4, r5, r6, lr}
0031c474: mov      r4, r0
0031c478: str      r3, [r0, #4]
0031c47c: mov      r0, r1
0031c480: mov      r5, r1
0031c484: bl       #0x30de54
0031c488: mov      r1, r5
0031c48c: add      r2, r5, r0
0031c490: add      r0, r4, #0xc
0031c494: pop      {r4, r5, r6, lr}
0031c498: b        #0x3109e0

# _ZN3sfc6script3lua9ArgumentsC1Ev
003192b4: ldr      r3, [pc, #0x28]
003192b8: ldr      r2, [pc, #0x28]
003192bc: push     {r4, lr}
003192c0: add      r3, pc, r3
003192c4: ldr      r2, [r3, r2]
003192c8: mov      r4, r0
003192cc: add      r2, r2, #8
003192d0: str      r2, [r0]
003192d4: bl       #0x31ce84
003192d8: str      r0, [r4, #4]
003192dc: mov      r0, r4
003192e0: pop      {r4, pc}
