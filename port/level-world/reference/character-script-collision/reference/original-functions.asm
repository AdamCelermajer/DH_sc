
# _ZN11Application5GetDtEv
0031f66c: ldr      r0, [r0, #0x8c]
0031f670: bx       lr

# _ZNK16CharStateMachine11SM_IsMovingEb
003c029c: push     {r4, lr}
003c02a0: mov      r4, r1
003c02a4: bl       #0x3c01ac
003c02a8: cmp      r0, #4
003c02ac: beq      #0x3c02c0
003c02b0: cmp      r0, #0x13
003c02b4: beq      #0x3c02c8
003c02b8: mov      r0, #0
003c02bc: pop      {r4, pc}
003c02c0: mov      r0, #1
003c02c4: pop      {r4, pc}
003c02c8: eor      r0, r4, #1
003c02cc: pop      {r4, pc}

# _ZNK16CharStateMachine11SM_GetStateEv
003c01ac: ldr      r3, [r0, #0x20]
003c01b0: cmp      r3, #0
003c01b4: mvneq    r0, #0
003c01b8: ldrne    r0, [r3]
003c01bc: bx       lr

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
