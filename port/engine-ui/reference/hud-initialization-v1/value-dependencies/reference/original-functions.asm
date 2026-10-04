
# _ZNK15SavegameManager12getOptionMaxEPKc
0046d330: push     {r4, lr}
0046d334: sub      sp, sp, #8
0046d338: add      r3, sp, #8
0046d33c: str      r1, [r3, #-4]!
0046d340: add      r4, r0, #0x10
0046d344: mov      r1, r3
0046d348: mov      r0, r4
0046d34c: bl       #0x46ce64
0046d350: cmp      r0, r4
0046d354: mvneq    r0, #0
0046d358: beq      #0x46d370
0046d35c: ldr      r3, [r0, #0x28]
0046d360: ldr      r2, [r3, #0x18]
0046d364: ldr      r0, [r3, #0xc]
0046d368: cmp      r2, #2
0046d36c: subeq    r0, r0, #1
0046d370: add      sp, sp, #8
0046d374: pop      {r4, pc}

# _ZNK7gameswf8as_value9is_numberEv
00439d8c: str      lr, [sp, #-4]!
00439d90: ldrsb    r3, [r0, #1]
00439d94: sub      sp, sp, #0x14
00439d98: cmp      r3, #2
00439d9c: movne    r0, #0
00439da0: beq      #0x439dac
00439da4: add      sp, sp, #0x14
00439da8: ldm      sp!, {pc}
00439dac: ldr      r2, [r0, #8]
00439db0: ldr      r3, [r0, #4]
00439db4: str      r2, [sp, #0xc]
00439db8: str      r3, [sp, #8]
00439dbc: ldrd     r0, r1, [sp, #8]
00439dc0: mov      r2, r0
00439dc4: mov      r3, r1
00439dc8: bl       #0x30e2bc
00439dcc: rsbs     r0, r0, #1
00439dd0: movlo    r0, #0
00439dd4: b        #0x439da4

# _ZNK15SavegameManager9getOptionEPKc
0046d474: push     {r4, lr}
0046d478: sub      sp, sp, #8
0046d47c: add      r3, sp, #8
0046d480: str      r1, [r3, #-4]!
0046d484: add      r4, r0, #0x10
0046d488: mov      r1, r3
0046d48c: mov      r0, r4
0046d490: bl       #0x46ce64
0046d494: cmp      r0, r4
0046d498: mvneq    r0, #0
0046d49c: ldrne    r0, [r0, #0x2c]
0046d4a0: add      sp, sp, #8
0046d4a4: pop      {r4, pc}

# _ZNK15SavegameManager15getOptionStringEPKc
0046d2b8: push     {r4, lr}
0046d2bc: sub      sp, sp, #8
0046d2c0: add      r3, sp, #8
0046d2c4: str      r1, [r3, #-4]!
0046d2c8: add      r4, r0, #0x10
0046d2cc: mov      r1, r3
0046d2d0: mov      r0, r4
0046d2d4: bl       #0x46ce64
0046d2d8: cmp      r0, r4
0046d2dc: ldrne    r3, [r0, #0x28]
0046d2e0: ldrne    r0, [r0, #0x2c]
0046d2e4: mvneq    r0, #0
0046d2e8: ldrne    r3, [r3, #0x1c]
0046d2ec: addne    r0, r0, r3
0046d2f0: add      sp, sp, #8
0046d2f4: pop      {r4, pc}

# nativeIsSupportMM
00533570: ldr      r0, [pc, #0x48]
00533574: ldr      r1, [pc, #0x48]
00533578: push     {r4, lr}
0053357c: add      r0, pc, r0
00533580: add      r1, pc, r1
00533584: ldr      r4, [pc, #0x3c]
00533588: bl       #0x533560
0053358c: ldr      r3, [pc, #0x38]
00533590: add      r4, pc, r4
00533594: ldr      r1, [pc, #0x34]
00533598: ldr      r3, [r4, r3]
0053359c: add      r1, pc, r1
005335a0: ldr      r3, [r3]
005335a4: ldr      r2, [r1, #0x28]
005335a8: ldr      r1, [r1]
005335ac: mov      r0, r3
005335b0: ldr      r3, [r3]
005335b4: mov      lr, pc
005335b8: ldr      pc, [r3, #0x204]
005335bc: pop      {r4, pc}
005335c0: eorseq   sl, sl, ip, lsr r5
005335c4: eorseq   sl, sl, r8, ror #10
005335c8: subeq    r1, r6, r0, lsl #10
005335cc: muleq    r0, r4, sl
005335d0: subeq    r2, ip, r4, lsl #30
