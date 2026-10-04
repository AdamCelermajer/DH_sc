
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

# _ZNK3sfc6script3lua9ArgumentsixEj
0037baf8: push     {r4, r5, r6, lr}
0037bafc: ldr      r4, [r0, #4]
0037bb00: mov      r5, r1
0037bb04: ldm      r4, {r2, r3}
0037bb08: rsb      r3, r2, r3
0037bb0c: asr      r3, r3, #4
0037bb10: add      r1, r3, r3, lsl #3
0037bb14: add      r1, r1, r1, lsl #6
0037bb18: add      r1, r3, r1, lsl #3
0037bb1c: add      r1, r1, r1, lsl #15
0037bb20: add      r3, r3, r1, lsl #3
0037bb24: rsb      r3, r3, #0
0037bb28: cmp      r5, r3
0037bb2c: blo      #0x37bb40
0037bb30: ldr      r0, [pc, #0x14]
0037bb34: add      r0, pc, r0
0037bb38: bl       #0x708eb0
0037bb3c: ldr      r2, [r4]
0037bb40: mov      r0, #0x70
0037bb44: mla      r0, r0, r5, r2
0037bb48: pop      {r4, r5, r6, pc}
0037bb4c: subseq   r2, r4, r4, lsr sb
