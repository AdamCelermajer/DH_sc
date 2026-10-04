
# _Z19NativeGetNumPotionsRKN7gameswf7fn_callE
0044996c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00449970: ldr      r4, [pc, #0x184]
00449974: ldr      r8, [pc, #0x184]
00449978: ldr      r2, [r0, #0xc]
0044997c: add      r4, pc, r4
00449980: ldr      r1, [r4, r8]
00449984: sub      sp, sp, #0x50
00449988: ldr      r3, [r0, #0x14]
0044998c: ldr      r1, [r1]
00449990: str      r1, [sp, #0x4c]
00449994: ldr      r2, [r2]
00449998: mov      r1, #0xc
0044999c: mla      r3, r1, r3, r2
004499a0: mov      r1, #0
004499a4: ldrsb    r2, [r3, #1]
004499a8: cmp      r2, #5
004499ac: ldreq    r5, [r3, #4]
004499b0: ldr      r3, [pc, #0x14c]
004499b4: mov      r2, #1
004499b8: movne    r5, #0
004499bc: ldr      r3, [r4, r3]
004499c0: ldr      r0, [r3, #0x40]
004499c4: bl       #0x36e478
004499c8: ldr      r6, [r0, #0x660]
004499cc: cmp      r6, #0
004499d0: beq      #0x449abc
004499d4: ldr      r1, [pc, #0x12c]
004499d8: ldr      r3, [r5]
004499dc: add      sb, sp, #0x38
004499e0: add      r1, pc, r1
004499e4: mov      r0, sb
004499e8: ldr      sl, [r3, #0x1c]
004499ec: bl       #0x413a7c
004499f0: add      r0, r6, #0x37c
004499f4: bl       #0x3fc690
004499f8: mov      r3, #0
004499fc: strb     r3, [sp, #0xc]
00449a00: mov      r3, #2
00449a04: strb     r3, [sp, #0xd]
00449a08: bl       #0x30ed30
00449a0c: strd     r0, r1, [sp, #0x18]
00449a10: ldr      r3, [sp, #0x18]
00449a14: add      r7, sp, #0xc
00449a18: mov      r1, sb
00449a1c: str      r3, [sp, #0x10]
00449a20: ldr      r3, [sp, #0x1c]
00449a24: mov      r2, r7
00449a28: mov      r0, r5
00449a2c: str      r3, [r7, #8]
00449a30: blx      sl
00449a34: mov      r0, r7
00449a38: bl       #0x797124
00449a3c: ldrsb    r3, [sp, #0x38]
00449a40: cmn      r3, #1
00449a44: beq      #0x449ad8
00449a48: ldr      r1, [pc, #0xbc]
00449a4c: ldr      r3, [r5]
00449a50: add      sl, sp, #0x24
00449a54: add      r1, pc, r1
00449a58: mov      r0, sl
00449a5c: ldr      r7, [r3, #0x1c]
00449a60: bl       #0x413a7c
00449a64: ldrb     r0, [r6, #0x3a8]
00449a68: mov      r3, #0
00449a6c: strb     r3, [sp]
00449a70: sxtb     r0, r0
00449a74: mov      r3, #2
00449a78: strb     r3, [sp, #1]
00449a7c: bl       #0x30ed30
00449a80: strd     r0, r1, [sp, #0x18]
00449a84: ldr      r3, [sp, #0x18]
00449a88: mov      r1, sl
00449a8c: mov      r2, sp
00449a90: str      r3, [sp, #4]
00449a94: ldr      r3, [sp, #0x1c]
00449a98: mov      r0, r5
00449a9c: mov      r6, sp
00449aa0: str      r3, [sp, #8]
00449aa4: blx      r7
00449aa8: mov      r0, sp
00449aac: bl       #0x797124
00449ab0: ldrsb    r3, [sp, #0x24]
00449ab4: cmn      r3, #1
00449ab8: beq      #0x449ae8
00449abc: ldr      r3, [r4, r8]
00449ac0: ldr      r2, [sp, #0x4c]
00449ac4: ldr      r3, [r3]
00449ac8: cmp      r2, r3
00449acc: bne      #0x449af8
00449ad0: add      sp, sp, #0x50
00449ad4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00449ad8: ldr      r0, [sp, #0x44]
00449adc: ldr      r1, [sp, #0x40]
00449ae0: bl       #0x752b38
00449ae4: b        #0x449a48
00449ae8: ldr      r0, [sp, #0x30]
00449aec: ldr      r1, [sp, #0x2c]
00449af0: bl       #0x752b38
00449af4: b        #0x449abc
00449af8: bl       #0x30e310
00449afc: subseq   fp, r4, r4, lsl r1
00449b00: andeq    r4, r0, ip, lsr #1
00449b04: strdeq   r3, r4, [r0], -r4
00449b08: subeq    r2, r8, r0, asr #26
00449b0c: ldrdeq   r2, r3, [r8], #-0xcc

# _ZN6Device22IsProgrammablePipelineEv
003816a8: ldr      r3, [pc, #0x78]
003816ac: ldr      r2, [pc, #0x78]
003816b0: push     {r4, lr}
003816b4: add      r3, pc, r3
003816b8: ldr      r2, [r3, r2]
003816bc: ldrb     r2, [r2]
003816c0: cmp      r2, #0
003816c4: bne      #0x3816f0
003816c8: ldr      r2, [pc, #0x60]
003816cc: ldr      r2, [r3, r2]
003816d0: ldrb     r2, [r2]
003816d4: cmp      r2, #0
003816d8: bne      #0x3816f0
003816dc: ldr      r2, [pc, #0x50]
003816e0: ldr      r2, [r3, r2]
003816e4: ldrb     r2, [r2]
003816e8: cmp      r2, #0
003816ec: beq      #0x3816f8
003816f0: mov      r0, #0
003816f4: pop      {r4, pc}
003816f8: ldr      r2, [pc, #0x38]
003816fc: ldr      r3, [r3, r2]
00381700: ldr      r3, [r3, #0x10]
00381704: ldr      r3, [r3, #0x10]
00381708: mov      r0, r3
0038170c: ldr      r3, [r3]
00381710: mov      lr, pc
00381714: ldr      pc, [r3, #0x5c]
00381718: tst      r0, #0x78
0038171c: moveq    r0, #0
00381720: movne    r0, #1
00381724: pop      {r4, pc}

# _Z25NativeGetStringNumPotionsRKN7gameswf7fn_callE
00446978: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0044697c: ldr      r4, [pc, #0x160]
00446980: ldr      r8, [pc, #0x160]
00446984: ldr      r3, [r0, #0xc]
00446988: add      r4, pc, r4
0044698c: ldr      r2, [r4, r8]
00446990: sub      sp, sp, #0x44
00446994: ldr      r0, [r0, #0x14]
00446998: ldr      r2, [r2]
0044699c: str      r2, [sp, #0x3c]
004469a0: ldr      r3, [r3]
004469a4: mov      r2, #0xc
004469a8: mla      r2, r2, r0, r3
004469ac: sub      r0, r0, #1
004469b0: ldrsb    r1, [r2, #1]
004469b4: cmp      r1, #5
004469b8: ldreq    r7, [r2, #4]
004469bc: mov      r2, #0xc
004469c0: mla      r0, r2, r0, r3
004469c4: movne    r7, #0
004469c8: bl       #0x797a54
004469cc: bl       #0x30ea24
004469d0: mov      r1, #0
004469d4: bl       #0x43c388
004469d8: subs     sb, r0, #0
004469dc: beq      #0x446ab4
004469e0: add      r5, sp, #0x10
004469e4: mov      r0, r5
004469e8: mov      r1, #0x10
004469ec: str      r5, [sp, #0x20]
004469f0: str      r5, [sp, #0x24]
004469f4: bl       #0x31167c
004469f8: ldr      r2, [sp, #0x20]
004469fc: ldr      r3, [pc, #0xe8]
00446a00: mov      r6, #0
00446a04: strb     r6, [r2]
00446a08: ldr      r3, [r4, r3]
00446a0c: ldr      r2, [pc, #0xdc]
00446a10: ldr      r1, [pc, #0xdc]
00446a14: ldr      r0, [r3, #0x2c]
00446a18: add      r2, pc, r2
00446a1c: add      r1, pc, r1
00446a20: ldr      sl, [r3, #0x34]
00446a24: bl       #0x4c4bdc
00446a28: mov      r1, r0
00446a2c: mov      r0, sl
00446a30: bl       #0x508edc
00446a34: mov      fp, r0
00446a38: add      r0, sb, #0x37c
00446a3c: bl       #0x3fc690
00446a40: mov      r2, fp
00446a44: mov      r3, r0
00446a48: mov      r1, r5
00446a4c: mov      r0, sl
00446a50: bl       #0x508ef4
00446a54: ldr      r1, [pc, #0x9c]
00446a58: ldr      r3, [r7]
00446a5c: add      fp, sp, #0x28
00446a60: add      sl, sp, #4
00446a64: add      r1, pc, r1
00446a68: mov      r0, fp
00446a6c: ldr      sb, [r3, #0x1c]
00446a70: bl       #0x413a7c
00446a74: mov      r0, sl
00446a78: ldr      r1, [sp, #0x24]
00446a7c: strb     r6, [sp, #5]
00446a80: strb     r6, [sp, #4]
00446a84: bl       #0x797350
00446a88: mov      r1, fp
00446a8c: mov      r2, sl
00446a90: mov      r0, r7
00446a94: blx      sb
00446a98: mov      r0, sl
00446a9c: bl       #0x797124
00446aa0: ldrsb    r3, [sp, #0x28]
00446aa4: cmn      r3, #1
00446aa8: beq      #0x446ad0
00446aac: mov      r0, r5
00446ab0: bl       #0x3139ac
00446ab4: ldr      r3, [r4, r8]
00446ab8: ldr      r2, [sp, #0x3c]
00446abc: ldr      r3, [r3]
00446ac0: cmp      r2, r3
00446ac4: bne      #0x446ae0
00446ac8: add      sp, sp, #0x44
00446acc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00446ad0: ldr      r0, [sp, #0x34]
00446ad4: ldr      r1, [sp, #0x30]
00446ad8: bl       #0x752b38
00446adc: b        #0x446aac
00446ae0: bl       #0x30e310
00446ae4: subseq   lr, r4, r8, lsl #2
00446ae8: andeq    r4, r0, ip, lsr #1
00446aec: strdeq   r3, r4, [r0], -r4
00446af0: subeq    r5, r8, r8, lsl r7
00446af4: subeq    r8, r7, ip, lsl #4
00446af8: subeq    r5, r8, r4, ror #13
