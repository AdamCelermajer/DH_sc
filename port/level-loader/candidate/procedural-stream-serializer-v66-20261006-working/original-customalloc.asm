# _Z11CustomAllocj 0x310454 size=164
00310454: push {r4, r5, lr}
00310458: sub sp, sp, #0xc
0031045c: bl #0x30e6f4
00310460: ldr r5, [pc, #0x74]
00310464: subs r4, r0, #0
00310468: add r5, pc, r5
0031046c: beq #0x31047c
00310470: mov r0, r4
00310474: add sp, sp, #0xc
00310478: pop {r4, r5, pc}
0031047c: ldr r0, [pc, #0x5c]
00310480: add r0, pc, r0
00310484: bl #0x31041c
00310488: ldr r3, [pc, #0x54]
0031048c: ldr r3, [r5, r3]
00310490: ldr r3, [r3]
00310494: cmp r3, #2
00310498: streq r4, [r4]
0031049c: beq #0x310470
003104a0: cmp r3, #1
003104a4: bne #0x310470
003104a8: ldr r0, [pc, #0x38]
003104ac: ldr r1, [pc, #0x38]
003104b0: ldr r2, [pc, #0x38]
003104b4: ldr r0, [r5, r0]
003104b8: ldr r3, [pc, #0x34]
003104bc: mov ip, #0x3a
003104c0: add r1, pc, r1
003104c4: add r2, pc, r2
003104c8: add r3, pc, r3
003104cc: add r0, r0, #0xa8
003104d0: str ip, [sp]
003104d4: bl #0x30e004
003104d8: b #0x310470
003104dc: rsbeq r4, r8, r8, lsr #12
003104e0: subseq sp, sl, r8, asr #30
003104e4: andeq r3, r0, r0, asr #19
003104e8: andeq r1, r0, r0, asr #19
003104ec: subseq sp, sl, r8, lsl pc
003104f0: subseq sp, sl, r4, lsr pc
003104f4: subseq sp, sl, r8, lsr pc
