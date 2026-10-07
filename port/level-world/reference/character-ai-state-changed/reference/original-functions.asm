
# _ZN9LuaScript4CallEPKcRN3sfc6script3lua12ReturnValuesE
0037c494: push     {r4, r5, r6, r7, lr}
0037c498: ldr      r3, [r2, #0x24]
0037c49c: mov      r5, r2
0037c4a0: ldr      r4, [pc, #0x64]
0037c4a4: ldr      ip, [r3]
0037c4a8: ldr      r2, [r3, #4]
0037c4ac: sub      sp, sp, #0xc
0037c4b0: mov      r6, r0
0037c4b4: cmp      ip, r2
0037c4b8: add      r4, pc, r4
0037c4bc: mov      r7, r1
0037c4c0: beq      #0x37c4d4
0037c4c4: mov      r0, r3
0037c4c8: mov      r1, ip
0037c4cc: add      r3, sp, #4
0037c4d0: bl       #0x31c3cc
0037c4d4: mov      r1, r7
0037c4d8: mov      r0, r6
0037c4dc: bl       #0x37c314
0037c4e0: mov      r2, r5
0037c4e4: mov      r1, r0
0037c4e8: add      r0, r6, #4
0037c4ec: bl       #0x31ab14
0037c4f0: ldr      r3, [pc, #0x18]
0037c4f4: ldr      r3, [r4, r3]
0037c4f8: ldr      r2, [r3]
0037c4fc: add      r2, r2, #1
0037c500: str      r2, [r3]
0037c504: add      sp, sp, #0xc
0037c508: pop      {r4, r5, r6, r7, pc}

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

# _ZN10AISDefault11OnEndOfAnimEv
003dbeec: bx       lr

# _ZN6CharAI14OnStateChangedEii
003d0bec: push     {r4, lr}
003d0bf0: ldr      r3, [r0, #0x1c]
003d0bf4: cmp      r3, #0
003d0bf8: beq      #0x3d0c0c
003d0bfc: mov      r0, r3
003d0c00: ldr      r3, [r3]
003d0c04: mov      lr, pc
003d0c08: ldr      pc, [r3, #0x20]
003d0c0c: pop      {r4, pc}

# _ZN10AISDefault14OnStateChangedEii
003dbe8c: bx       lr

# _ZN11AISExternal11OnEndOfAnimEv
003dccd0: push     {r4, lr}
003dccd4: mov      r4, r0
003dccd8: bl       #0x3dbeec
003dccdc: ldr      r1, [pc, #0xc]
003dcce0: mov      r0, r4
003dcce4: add      r1, pc, r1
003dcce8: pop      {r4, lr}
003dccec: b        #0x37c514
003dccf0: subeq    r8, lr, r4, asr #25

# _ZN6CharAI11OnEndOfAnimEv
003d0ce8: push     {r4, lr}
003d0cec: ldr      r3, [r0, #0x1c]
003d0cf0: cmp      r3, #0
003d0cf4: beq      #0x3d0d08
003d0cf8: mov      r0, r3
003d0cfc: ldr      r3, [r3]
003d0d00: mov      lr, pc
003d0d04: ldr      pc, [r3, #0x98]
003d0d08: pop      {r4, pc}
