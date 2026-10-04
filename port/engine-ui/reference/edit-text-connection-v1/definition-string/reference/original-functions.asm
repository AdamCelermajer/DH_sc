
# _ZN7gameswf10removeHTMLERNS_9tu_stringE
0078b4d8: push     {r4, r5, r6, r7, r8, sl, lr}
0078b4dc: ldr      r4, [pc, #0x118]
0078b4e0: ldr      r6, [pc, #0x118]
0078b4e4: ldrsb    sl, [r0]
0078b4e8: add      r4, pc, r4
0078b4ec: ldr      r3, [r4, r6]
0078b4f0: sub      sp, sp, #0x20c
0078b4f4: cmn      sl, #1
0078b4f8: ldr      r3, [r3]
0078b4fc: mov      r5, r0
0078b500: str      r3, [sp, #0x204]
0078b504: beq      #0x78b5c4
0078b508: ldr      r1, [pc, #0xf4]
0078b50c: add      r8, r0, #1
0078b510: mov      r0, r8
0078b514: add      r1, pc, r1
0078b518: bl       #0x30ebd4
0078b51c: sub      sl, sl, #1
0078b520: cmp      sl, #0
0078b524: blt      #0x78b568
0078b528: sub      r8, r8, #1
0078b52c: cmp      r0, r8
0078b530: bls      #0x78b568
0078b534: ldrsb    r3, [r0]
0078b538: cmp      r3, #0x3e
0078b53c: moveq    r1, r0
0078b540: beq      #0x78b584
0078b544: mov      r1, r0
0078b548: b        #0x78b558
0078b54c: ldrsb    r3, [r1]
0078b550: cmp      r3, #0x3e
0078b554: beq      #0x78b584
0078b558: sub      r1, r1, #1
0078b55c: sub      r8, r8, #1
0078b560: cmp      r8, r1
0078b564: blo      #0x78b54c
0078b568: ldr      r3, [r4, r6]
0078b56c: ldr      r2, [sp, #0x204]
0078b570: ldr      r3, [r3]
0078b574: cmp      r2, r3
0078b578: bne      #0x78b5f8
0078b57c: add      sp, sp, #0x20c
0078b580: pop      {r4, r5, r6, r7, r8, sl, pc}
0078b584: add      r1, r1, #1
0078b588: rsb      r7, r1, r0
0078b58c: cmp      r7, #0
0078b590: ble      #0x78b5e4
0078b594: add      r8, sp, #4
0078b598: mov      r2, r7
0078b59c: mov      r0, r8
0078b5a0: bl       #0x30e868
0078b5a4: add      r3, sp, #0x208
0078b5a8: add      r7, r3, r7
0078b5ac: mov      r3, #0
0078b5b0: mov      r0, r5
0078b5b4: mov      r1, r8
0078b5b8: strb     r3, [r7, #-0x204]
0078b5bc: bl       #0x76c818
0078b5c0: b        #0x78b568
0078b5c4: ldr      r8, [r0, #0xc]
0078b5c8: ldr      r1, [pc, #0x38]
0078b5cc: mov      r0, r8
0078b5d0: add      r1, pc, r1
0078b5d4: bl       #0x30ebd4
0078b5d8: ldr      sl, [r5, #4]
0078b5dc: sub      sl, sl, #1
0078b5e0: b        #0x78b520
0078b5e4: ldr      r1, [pc, #0x20]
0078b5e8: mov      r0, r5
0078b5ec: add      r1, pc, r1
0078b5f0: bl       #0x76c818
0078b5f4: b        #0x78b568
0078b5f8: bl       #0x30e310
0078b5fc: eoreq    sb, r0, r8, lsr #11
0078b600: andeq    r4, r0, ip, lsr #1
0078b604: andseq   r0, r5, r4, lsl ip
0078b608: andseq   r0, r5, r8, asr fp
0078b60c: andseq   r0, r4, ip, lsl r2
