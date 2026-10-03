
# luaU_undump
0085c320: push     {r4, r5, r6, r7, r8, sl, lr}
0085c324: ldr      r4, [pc, #0xe4]
0085c328: ldr      r5, [pc, #0xe4]
0085c32c: ldrsb    ip, [r3]
0085c330: add      r4, pc, r4
0085c334: ldr      lr, [r4, r5]
0085c338: mov      r7, r0
0085c33c: cmp      ip, #0x40
0085c340: cmpne    ip, #0x3d
0085c344: ldr      r0, [lr]
0085c348: sub      sp, sp, #0x34
0085c34c: addeq    r3, r3, #1
0085c350: str      r0, [sp, #0x2c]
0085c354: streq    r3, [sp, #0x10]
0085c358: beq      #0x85c368
0085c35c: cmp      ip, #0x1b
0085c360: strne    r3, [sp, #0x10]
0085c364: beq      #0x85c3fc
0085c368: add      sl, sp, #0x20
0085c36c: mov      r0, sl
0085c370: add      r8, sp, #0x14
0085c374: add      r6, sp, #4
0085c378: str      r1, [sp, #8]
0085c37c: str      r2, [sp, #0xc]
0085c380: str      r7, [sp, #4]
0085c384: bl       #0x85bc64
0085c388: mov      r2, #0xc
0085c38c: mov      r0, r6
0085c390: mov      r1, r8
0085c394: bl       #0x85bcfc
0085c398: mov      r0, sl
0085c39c: mov      r1, r8
0085c3a0: mov      r2, #0xc
0085c3a4: bl       #0x30e5e0
0085c3a8: cmp      r0, #0
0085c3ac: beq      #0x85c3c0
0085c3b0: ldr      r1, [pc, #0x60]
0085c3b4: mov      r0, r6
0085c3b8: add      r1, pc, r1
0085c3bc: bl       #0x85bcc8
0085c3c0: ldr      r1, [pc, #0x54]
0085c3c4: mov      r2, #2
0085c3c8: mov      r0, r7
0085c3cc: add      r1, pc, r1
0085c3d0: bl       #0x857f08
0085c3d4: mov      r1, r0
0085c3d8: mov      r0, r6
0085c3dc: bl       #0x85bddc
0085c3e0: ldr      r3, [r4, r5]
0085c3e4: ldr      r2, [sp, #0x2c]
0085c3e8: ldr      r3, [r3]
0085c3ec: cmp      r2, r3
0085c3f0: bne      #0x85c40c
0085c3f4: add      sp, sp, #0x34
0085c3f8: pop      {r4, r5, r6, r7, r8, sl, pc}
0085c3fc: ldr      r3, [pc, #0x1c]
0085c400: add      r3, pc, r3
0085c404: str      r3, [sp, #0x10]
0085c408: b        #0x85c368
0085c40c: bl       #0x30e310
0085c410: andseq   r8, r3, r0, ror #14
0085c414: andeq    r4, r0, ip, lsr #1
0085c418: strdeq   r4, r5, [fp], -r8
0085c41c: strdeq   r4, r5, [fp], -r4
0085c420: andeq    r4, fp, r0, lsr #15

# luaU_header
0085bc64: push     {r4, r5, lr}
0085bc68: ldr      r1, [pc, #0x54]
0085bc6c: mov      r5, #4
0085bc70: sub      sp, sp, #0xc
0085bc74: mov      r3, #1
0085bc78: add      r1, pc, r1
0085bc7c: mov      r2, r5
0085bc80: mov      r4, r0
0085bc84: str      r3, [sp, #4]
0085bc88: bl       #0x30e868
0085bc8c: ldrb     r1, [sp, #4]
0085bc90: mov      r3, #0
0085bc94: add      r2, r4, #0xa
0085bc98: mov      r0, #0x51
0085bc9c: strb     r5, [r4, #0xa]
0085bca0: strb     r0, [r4, #4]
0085bca4: strb     r1, [r4, #6]
0085bca8: strb     r3, [r4, #5]
0085bcac: strb     r5, [r4, #7]
0085bcb0: strb     r5, [r4, #8]
0085bcb4: strb     r5, [r4, #9]
0085bcb8: strb     r3, [r2, #1]
0085bcbc: add      sp, sp, #0xc
0085bcc0: pop      {r4, r5, pc}
0085bcc4: strheq   r4, [fp], -r0

# _ZN6CharAI14AI_PauseUpdateEj
003cb748: str      lr, [sp, #-4]!
003cb74c: ldr      r3, [r0, #4]
003cb750: mov      ip, #0
003cb754: mov      r2, #1
003cb758: strb     r2, [r0, #0x18]
003cb75c: sub      sp, sp, #0xc
003cb760: add      r0, r3, #0x3b4
003cb764: mov      r2, ip
003cb768: mov      r3, #0x31
003cb76c: str      ip, [sp]
003cb770: bl       #0x3dbe24
003cb774: add      sp, sp, #0xc
003cb778: ldm      sp!, {pc}

# _ZN12CharAIScript19CallStateConditionsEv
003d8ea0: ldr      r3, [r0, #0xb4]
003d8ea4: cmp      r3, #0
003d8ea8: bxeq     lr
003d8eac: ldr      r1, [r3, #0x2c]
003d8eb0: b        #0x37c514

# _ZN12CharAIScript15CallStateUpdateEv
003d8eb4: ldr      r3, [r0, #0xb4]
003d8eb8: cmp      r3, #0
003d8ebc: bxeq     lr
003d8ec0: ldr      r1, [r3, #0x14]
003d8ec4: b        #0x37c514

# _ZN10AISDefault18OnCollisionPersistEP10GameObjectb
003dbfa0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003dbfa4: mov      r4, r0
003dbfa8: ldr      r0, [r0, #0x98]
003dbfac: mov      r5, r1
003dbfb0: mov      r1, #0
003dbfb4: add      r0, r0, #0x4f0
003dbfb8: add      r0, r0, #0xc
003dbfbc: mov      r7, r2
003dbfc0: bl       #0x3c029c
003dbfc4: ldr      r6, [pc, #0x158]
003dbfc8: cmp      r0, #0
003dbfcc: add      r6, pc, r6
003dbfd0: beq      #0x3dc004
003dbfd4: ldr      r3, [r4, #0x98]
003dbfd8: ldr      sl, [r3, #0x408]
003dbfdc: cmp      r5, sl
003dbfe0: beq      #0x3dc004
003dbfe4: ldr      r3, [r5]
003dbfe8: mov      r0, r5
003dbfec: mov      lr, pc
003dbff0: ldr      pc, [r3, #0x24]
003dbff4: cmp      r0, #0
003dbff8: bne      #0x3dc060
003dbffc: cmp      r7, #0
003dc000: bne      #0x3dc008
003dc004: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003dc008: ldr      r3, [r5]
003dc00c: mov      r0, r5
003dc010: mov      lr, pc
003dc014: ldr      pc, [r3, #0x24]
003dc018: cmp      r0, #0
003dc01c: beq      #0x3dc0dc
003dc020: ldr      r2, [pc, #0x100]
003dc024: ldr      r3, [r4, #0xc0]
003dc028: ldr      r0, [r6, r2]
003dc02c: ldr      r2, [r0, #0x74]
003dc030: cmp      r3, r2
003dc034: beq      #0x3dc004
003dc038: ldr      r3, [r4, #0x98]
003dc03c: ldrb     r3, [r3, #0x3e0]
003dc040: cmp      r3, #0
003dc044: bne      #0x3dc004
003dc048: str      r2, [r4, #0xc0]
003dc04c: ldr      r5, [r4, #0xbc]
003dc050: bl       #0x31f66c
003dc054: add      r0, r0, r5
003dc058: str      r0, [r4, #0xbc]
003dc05c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003dc060: ldr      r2, [r4, #0x98]
003dc064: mov      r0, r2
003dc068: ldr      r3, [r2]
003dc06c: ldr      sb, [r2, #0x418]
003dc070: mov      lr, pc
003dc074: ldr      pc, [r3, #0x28]
003dc078: subs     r8, r0, #0
003dc07c: bne      #0x3dc09c
003dc080: rsbs     r3, sb, #1
003dc084: movlo    r3, #0
003dc088: cmp      sb, sl
003dc08c: moveq    sl, r3
003dc090: orrne    sl, r3, #1
003dc094: cmp      sl, #0
003dc098: bne      #0x3dc0f4
003dc09c: ldr      r3, [r4, #0x98]
003dc0a0: mov      r0, r3
003dc0a4: ldr      r3, [r3]
003dc0a8: mov      lr, pc
003dc0ac: ldr      pc, [r3, #0x28]
003dc0b0: cmp      r0, #0
003dc0b4: beq      #0x3dbffc
003dc0b8: ldr      r0, [r4, #0x98]
003dc0bc: mov      r1, r5
003dc0c0: add      r0, r0, #0x3c8
003dc0c4: bl       #0x3d574c
003dc0c8: cmp      r0, #0
003dc0cc: beq      #0x3dbffc
003dc0d0: ldr      r0, [r4, #0x98]
003dc0d4: bl       #0x3bc6b8
003dc0d8: b        #0x3dbffc
003dc0dc: ldr      r3, [r5, #0xf4]
003dc0e0: cmp      r3, #0x15
003dc0e4: beq      #0x3dc020
003dc0e8: cmp      r3, #2
003dc0ec: bne      #0x3dc004
003dc0f0: b        #0x3dc020
003dc0f4: ldr      r0, [r4, #0x98]
003dc0f8: mov      r1, r5
003dc0fc: add      r0, r0, #0x3c8
003dc100: bl       #0x3d574c
003dc104: cmp      r0, #0
003dc108: beq      #0x3dc09c
003dc10c: ldr      r0, [r4, #0x98]
003dc110: mov      r1, r5
003dc114: mov      r2, r8
003dc118: add      r0, r0, #0x3c8
003dc11c: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003dc120: b        #0x3d6890
003dc124: subseq   r8, fp, r4, asr #21
003dc128: strdeq   r3, r4, [r0], -r4

# _ZN12v2Controller8Cmd_StopEv
0040559c: push     {r4, lr}
004055a0: ldrb     r2, [r0, #9]
004055a4: ldr      r3, [pc, #0x44]
004055a8: cmp      r2, #0
004055ac: add      r3, pc, r3
004055b0: bne      #0x4055d8
004055b4: ldr      r2, [pc, #0x38]
004055b8: ldr      r3, [r3, r2]
004055bc: ldrb     r3, [r3]
004055c0: cmp      r3, #0
004055c4: beq      #0x4055cc
004055c8: pop      {r4, pc}
004055cc: ldrb     r3, [r0, #8]
004055d0: cmp      r3, #0
004055d4: bne      #0x4055c8
004055d8: ldr      r3, [r0, #4]
004055dc: mov      r0, r3
004055e0: ldr      r3, [r3]
004055e4: mov      lr, pc
004055e8: ldr      pc, [r3, #0x34]
004055ec: pop      {r4, pc}
004055f0: subseq   pc, r8, r4, ror #9
004055f4: andeq    r3, r0, r0, asr r6

# _ZN9LuaScript4LoadEPKc
0037b574: ldr      r3, [pc, #0x1c]
0037b578: ldr      r2, [pc, #0x1c]
0037b57c: mov      ip, r0
0037b580: add      r3, pc, r3
0037b584: ldr      r0, [r3, r2]
0037b588: mov      r2, r1
0037b58c: mov      r1, ip
0037b590: ldr      r0, [r0, #0x3c]
0037b594: b        #0x37b23c
0037b598: rsbeq    sb, r1, r0, lsl r5
0037b59c: strdeq   r3, r4, [r0], -r4

# _ZN9LuaScriptC2Eb
0037c674: push     {r4, r5, r6, r7, r8, lr}
0037c678: ldr      r6, [pc, #0xd8]
0037c67c: ldr      r3, [pc, #0xd8]
0037c680: mov      r7, r0
0037c684: add      r6, pc, r6
0037c688: ldr      r3, [r6, r3]
0037c68c: mov      r4, r0
0037c690: mov      r8, r1
0037c694: add      r3, r3, #8
0037c698: str      r3, [r7], #4
0037c69c: mov      r0, r7
0037c6a0: bl       #0x31b268
0037c6a4: ldr      r2, [pc, #0xb4]
0037c6a8: mov      r5, #0
0037c6ac: mov      r3, r4
0037c6b0: ldr      r2, [r6, r2]
0037c6b4: str      r7, [r4, #0x14]
0037c6b8: str      r5, [r4, #0x18]
0037c6bc: add      r2, r2, #8
0037c6c0: str      r2, [r4, #0x10]
0037c6c4: str      r5, [r4, #0x20]
0037c6c8: mov      r2, r4
0037c6cc: strb     r5, [r3, #0x1c]!
0037c6d0: str      r3, [r4, #0x28]
0037c6d4: str      r3, [r4, #0x24]
0037c6d8: str      r5, [r4, #0x2c]
0037c6dc: mov      r3, r4
0037c6e0: str      r5, [r4, #0x38]
0037c6e4: strb     r5, [r2, #0x34]!
0037c6e8: str      r2, [r4, #0x40]
0037c6ec: str      r2, [r4, #0x3c]
0037c6f0: add      r0, r4, #0x68
0037c6f4: str      r5, [r4, #0x44]
0037c6f8: str      r5, [r4, #0x50]
0037c6fc: strb     r5, [r3, #0x4c]!
0037c700: str      r3, [r4, #0x58]
0037c704: str      r3, [r4, #0x54]
0037c708: str      r5, [r4, #0x5c]
0037c70c: strb     r5, [r4, #0x64]
0037c710: str      r0, [r4, #0x78]
0037c714: str      r0, [r4, #0x7c]
0037c718: mov      r1, #0x10
0037c71c: bl       #0x31167c
0037c720: ldr      r2, [r4, #0x78]
0037c724: mov      r3, r4
0037c728: cmp      r8, r5
0037c72c: strb     r5, [r2]
0037c730: str      r5, [r4, #0x84]
0037c734: strb     r5, [r3, #0x80]!
0037c738: str      r3, [r4, #0x8c]
0037c73c: str      r5, [r4, #0x90]
0037c740: str      r3, [r4, #0x88]
0037c744: bne      #0x37c750
0037c748: mov      r0, r4
0037c74c: bl       #0x37b5a0
0037c750: mov      r0, r4
0037c754: pop      {r4, r5, r6, r7, r8, pc}
0037c758: rsbeq    r8, r1, ip, lsl #8
0037c75c: andeq    r1, r0, r4, ror r6
0037c760: andeq    r3, r0, r8, asr r6
