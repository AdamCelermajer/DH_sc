
# _ZNK3sfc6script3lua5Value9getStringEv
0031c49c: push     {r4, r5, r6, r7, r8, lr}
0031c4a0: ldr      r4, [pc, #0x16c]
0031c4a4: ldr      r6, [pc, #0x16c]
0031c4a8: ldr      r3, [r0, #4]
0031c4ac: add      r4, pc, r4
0031c4b0: ldr      r2, [r4, r6]
0031c4b4: sub      sp, sp, #0x28
0031c4b8: cmp      r3, #0
0031c4bc: ldr      r2, [r2]
0031c4c0: mov      r5, r0
0031c4c4: str      r2, [sp, #0x24]
0031c4c8: beq      #0x31c54c
0031c4cc: cmp      r3, #1
0031c4d0: beq      #0x31c5ec
0031c4d4: cmp      r3, #4
0031c4d8: beq      #0x31c5e4
0031c4dc: cmp      r3, #3
0031c4e0: bne      #0x31c59c
0031c4e4: ldr      r7, [r0, #8]
0031c4e8: mov      r0, r7
0031c4ec: bl       #0x30ecb8
0031c4f0: mov      r1, r0
0031c4f4: mov      r0, r7
0031c4f8: bl       #0x30df8c
0031c4fc: cmp      r0, #0
0031c500: bne      #0x31c570
0031c504: mov      r0, r7
0031c508: bl       #0x30e8a4
0031c50c: ldr      r8, [pc, #0x108]
0031c510: add      r7, sp, #4
0031c514: mov      r2, r0
0031c518: add      r8, pc, r8
0031c51c: mov      r3, r1
0031c520: mov      r0, r7
0031c524: mov      r1, r8
0031c528: bl       #0x30eae4
0031c52c: mov      r0, r7
0031c530: bl       #0x30de54
0031c534: mov      r1, r7
0031c538: add      r2, r7, r0
0031c53c: add      r0, r5, #0xc
0031c540: bl       #0x3109e0
0031c544: ldr      r0, [r5, #0x20]
0031c548: b        #0x31c554
0031c54c: ldr      r0, [pc, #0xcc]
0031c550: add      r0, pc, r0
0031c554: ldr      r3, [r4, r6]
0031c558: ldr      r2, [sp, #0x24]
0031c55c: ldr      r3, [r3]
0031c560: cmp      r2, r3
0031c564: bne      #0x31c610
0031c568: add      sp, sp, #0x28
0031c56c: pop      {r4, r5, r6, r7, r8, pc}
0031c570: mov      r0, r5
0031c574: bl       #0x31bbf0
0031c578: bl       #0x30e4cc
0031c57c: ldr      r8, [pc, #0xa0]
0031c580: add      r7, sp, #4
0031c584: mov      r2, r0
0031c588: add      r8, pc, r8
0031c58c: mov      r0, r7
0031c590: mov      r1, r8
0031c594: bl       #0x30eae4
0031c598: b        #0x31c52c
0031c59c: cmp      r3, #2
0031c5a0: beq      #0x31c5b0
0031c5a4: cmp      r3, #7
0031c5a8: movne    r0, #0
0031c5ac: bne      #0x31c554
0031c5b0: ldr      r1, [pc, #0x70]
0031c5b4: add      r7, sp, #4
0031c5b8: mov      r2, #8
0031c5bc: add      r1, pc, r1
0031c5c0: ldr      r3, [r5, #0x6c]
0031c5c4: mov      r0, r7
0031c5c8: bl       #0x30eae4
0031c5cc: mov      r0, r7
0031c5d0: bl       #0x30de54
0031c5d4: mov      r1, r7
0031c5d8: add      r2, r7, r0
0031c5dc: add      r0, r5, #0xc
0031c5e0: bl       #0x3109e0
0031c5e4: ldr      r0, [r5, #0x20]
0031c5e8: b        #0x31c554
0031c5ec: bl       #0x31bc80
0031c5f0: cmp      r0, #0
0031c5f4: bne      #0x31c604
0031c5f8: ldr      r0, [pc, #0x2c]
0031c5fc: add      r0, pc, r0
0031c600: b        #0x31c554
0031c604: ldr      r0, [pc, #0x24]
0031c608: add      r0, pc, r0
0031c60c: b        #0x31c554
0031c610: bl       #0x30e310
0031c614: rsbeq    r8, r7, r4, ror #11
0031c618: andeq    r4, r0, ip, lsr #1
0031c61c: subseq   r1, sl, r8, asr #28

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

# _ZN10AISDefault8OnUpdateEv
003dc798: push     {r4, lr}
003dc79c: ldr      r3, [r0, #0xbc]
003dc7a0: mov      r4, r0
003dc7a4: cmp      r3, #0xc7
003dc7a8: bhi      #0x3dc7b0
003dc7ac: pop      {r4, pc}
003dc7b0: ldr      r0, [r0, #0x98]
003dc7b4: mov      r3, #0
003dc7b8: str      r3, [r4, #0xbc]
003dc7bc: add      r0, r0, #0x3c8
003dc7c0: mov      r1, #0x3e8
003dc7c4: bl       #0x3cb748
003dc7c8: ldr      r3, [r4, #0x98]
003dc7cc: ldr      r0, [r3, #0x378]
003dc7d0: pop      {r4, lr}
003dc7d4: b        #0x40559c
