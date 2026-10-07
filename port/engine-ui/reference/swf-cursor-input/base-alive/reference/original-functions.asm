
# _ZN7gameswf9as_object10this_aliveEv
0076c518: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0076c51c: ldr      r3, [r0, #0x30]
0076c520: mov      r5, r0
0076c524: cmp      r3, #0
0076c528: beq      #0x76c59c
0076c52c: ldr      r0, [r0, #0x2c]
0076c530: ldrb     r2, [r0, #4]
0076c534: cmp      r2, #0
0076c538: beq      #0x76c5d8
0076c53c: ldr      r3, [r3, #0x30]
0076c540: ldr      r2, [r5, #0x34]
0076c544: cmp      r2, r3
0076c548: beq      #0x76c59c
0076c54c: ldr      r2, [r5, #0xc]
0076c550: str      r3, [r5, #0x34]
0076c554: cmp      r2, #0
0076c558: beq      #0x76c574
0076c55c: ldr      r1, [r2, #4]
0076c560: cmp      r1, #0
0076c564: movlt    r4, #0
0076c568: bge      #0x76c5a0
0076c56c: adds     sl, r5, #0xc
0076c570: bne      #0x76c620
0076c574: ldr      r3, [r5, #0x28]
0076c578: cmp      r3, #0
0076c57c: beq      #0x76c590
0076c580: mov      r0, r3
0076c584: ldr      r3, [r3]
0076c588: mov      lr, pc
0076c58c: ldr      pc, [r3, #0x44]
0076c590: ldrsb    r3, [r5, #0x11]
0076c594: cmp      r3, #5
0076c598: beq      #0x76c600
0076c59c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0076c5a0: mov      r3, #8
0076c5a4: mov      r4, #0
0076c5a8: ldr      r0, [r2, r3]
0076c5ac: add      ip, r2, r3
0076c5b0: add      r3, r3, #0x28
0076c5b4: cmn      r0, #2
0076c5b8: beq      #0x76c5c8
0076c5bc: ldr      r0, [ip, #4]
0076c5c0: cmn      r0, #1
0076c5c4: bne      #0x76c56c
0076c5c8: add      r4, r4, #1
0076c5cc: cmp      r4, r1
0076c5d0: ble      #0x76c5a8
0076c5d4: b        #0x76c56c
0076c5d8: ldr      r1, [r0]
0076c5dc: sub      r1, r1, #1
0076c5e0: cmp      r1, #0
0076c5e4: str      r1, [r0]
0076c5e8: bne      #0x76c5f0
0076c5ec: bl       #0x752b38
0076c5f0: mov      r3, #0
0076c5f4: str      r3, [r5, #0x30]
0076c5f8: str      r3, [r5, #0x2c]
0076c5fc: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0076c600: ldr      r3, [r5, #0x14]
0076c604: cmp      r3, #0
0076c608: beq      #0x76c59c
0076c60c: mov      r0, r3
0076c610: ldr      r3, [r3]
0076c614: mov      lr, pc
0076c618: ldr      pc, [r3, #0x44]
0076c61c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0076c620: ldr      r2, [sl]
0076c624: mov      r8, #0
0076c628: b        #0x76c638
0076c62c: ldr      r1, [r0, #4]
0076c630: cmn      r1, #1
0076c634: beq      #0x76c694
0076c638: add      r7, r4, r4, lsl #2
0076c63c: add      r7, r7, #1
0076c640: lsl      r7, r7, #3
0076c644: cmp      r2, #0
0076c648: beq      #0x76c574
0076c64c: ldr      ip, [r2, #4]
0076c650: cmp      r4, ip
0076c654: bgt      #0x76c574
0076c658: add      r3, r2, r7
0076c65c: ldrsb    r1, [r3, #0x1d]
0076c660: cmp      r1, #5
0076c664: beq      #0x76c6a4
0076c668: add      r4, r4, #1
0076c66c: cmp      r4, ip
0076c670: bgt      #0x76c638
0076c674: add      r3, r4, r4, lsl #2
0076c678: add      r3, r3, #1
0076c67c: lsl      r3, r3, #3
0076c680: ldr      r1, [r2, r3]
0076c684: add      r0, r2, r3
0076c688: add      r3, r3, #0x28
0076c68c: cmn      r1, #2
0076c690: bne      #0x76c62c
0076c694: add      r4, r4, #1
0076c698: cmp      ip, r4
0076c69c: blt      #0x76c638
0076c6a0: b        #0x76c680
0076c6a4: ldr      r6, [r3, #0x20]
0076c6a8: cmp      r6, #0
0076c6ac: beq      #0x76c668
0076c6b0: ldr      r3, [r5, #0x30]
0076c6b4: cmp      r3, #0
0076c6b8: beq      #0x76c6cc
0076c6bc: ldr      r0, [r5, #0x2c]
0076c6c0: ldrb     r2, [r0, #4]
0076c6c4: cmp      r2, #0
0076c6c8: beq      #0x76c700
0076c6cc: ldr      r3, [r3, #0x30]
0076c6d0: ldr      r2, [r6, #0x34]
0076c6d4: cmp      r2, r3
0076c6d8: beq      #0x76c6ec
0076c6dc: mov      r0, r6
0076c6e0: ldr      r3, [r6]
0076c6e4: mov      lr, pc
0076c6e8: ldr      pc, [r3, #0x44]
0076c6ec: ldr      r2, [sl]
0076c6f0: ldr      ip, [r2, #4]
0076c6f4: cmp      r4, ip
0076c6f8: bgt      #0x76c644
0076c6fc: b        #0x76c668
0076c700: ldr      r1, [r0]
0076c704: sub      r1, r1, #1
0076c708: cmp      r1, #0
0076c70c: str      r1, [r0]
0076c710: bne      #0x76c718
0076c714: bl       #0x752b38
0076c718: str      r8, [r5, #0x2c]
0076c71c: str      r8, [r5, #0x30]
0076c720: mov      r3, r8
0076c724: b        #0x76c6cc
