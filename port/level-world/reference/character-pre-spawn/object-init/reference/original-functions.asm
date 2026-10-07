
# _ZN13ObjectManager8InitPostEv
0034552c: push     {r4, r5, r6, r7, lr}
00345530: ldr      r5, [pc, #0x374]
00345534: sub      sp, sp, #0x24
00345538: mov      r4, r0
0034553c: add      r5, pc, r5
00345540: ldr      r6, [r5, #0x1c]
00345544: ands     r6, r6, #1
00345548: beq      #0x345620
0034554c: ldr      r5, [pc, #0x35c]
00345550: add      r5, pc, r5
00345554: ldr      r6, [r5, #0x24]
00345558: ands     r6, r6, #1
0034555c: beq      #0x345644
00345560: add      r6, sp, #0x10
00345564: mov      r0, r6
00345568: bl       #0x33f50c
0034556c: ldr      r2, [r4, #0x7c]
00345570: cmp      r2, #0
00345574: bne      #0x34559c
00345578: ldr      r3, [pc, #0x334]
0034557c: ldr      r2, [r4, #0x68]
00345580: add      r3, pc, r3
00345584: str      r2, [r3, #0x28]
00345588: ldr      r2, [r4, #0x14]
0034558c: str      r2, [r3, #0x20]
00345590: ldr      r2, [r4, #0x7c]
00345594: add      r2, r2, #1
00345598: str      r2, [r4, #0x7c]
0034559c: ldr      r5, [pc, #0x314]
003455a0: add      r1, r4, #0x68
003455a4: add      r5, pc, r5
003455a8: ldr      r3, [r5, #0x28]
003455ac: cmp      r1, r3
003455b0: beq      #0x345744
003455b4: cmp      r2, #1
003455b8: beq      #0x345880
003455bc: ldr      r5, [pc, #0x2f8]
003455c0: add      r1, r4, #0xc
003455c4: add      r5, pc, r5
003455c8: ldr      r3, [r5, #0x20]
003455cc: cmp      r1, r3
003455d0: beq      #0x345708
003455d4: cmp      r2, #3
003455d8: beq      #0x3457a0
003455dc: cmp      r2, #4
003455e0: beq      #0x345668
003455e4: ldr      r2, [r3, #0xc]
003455e8: cmp      r2, #0
003455ec: bne      #0x3455f8
003455f0: b        #0x34576c
003455f4: mov      r2, r3
003455f8: ldr      r3, [r2, #8]
003455fc: cmp      r3, #0
00345600: bne      #0x3455f4
00345604: mov      r3, r2
00345608: ldr      r2, [pc, #0x2b0]
0034560c: mov      r0, #0
00345610: add      r2, pc, r2
00345614: str      r3, [r2, #0x20]
00345618: add      sp, sp, #0x24
0034561c: pop      {r4, r5, r6, r7, pc}
00345620: add      r7, r5, #0x1c
00345624: mov      r0, r7
00345628: bl       #0x30e76c
0034562c: cmp      r0, #0
00345630: beq      #0x34554c
00345634: str      r6, [r5, #0x20]
00345638: mov      r0, r7
0034563c: bl       #0x30ea3c
00345640: b        #0x34554c
00345644: add      r7, r5, #0x24
00345648: mov      r0, r7
0034564c: bl       #0x30e76c
00345650: cmp      r0, #0
00345654: beq      #0x345560
00345658: str      r6, [r5, #0x28]
0034565c: mov      r0, r7
00345660: bl       #0x30ea3c
00345664: b        #0x345560
00345668: ldr      r5, [r3, #0x2c]
0034566c: cmp      r5, #0
00345670: beq      #0x3455e4
00345674: ldr      r0, [pc, #0x248]
00345678: ldr      r1, [r5, #0x5c]
0034567c: add      r0, pc, r0
00345680: bl       #0x30e6e8
00345684: cmp      r0, #0
00345688: bne      #0x345840
0034568c: mov      r3, #0xc
00345690: add      r0, sp, #0x20
00345694: str      r3, [r0, #-4]!
00345698: bl       #0x708ec0
0034569c: str      r5, [r0, #8]
003456a0: ldr      r3, [r4, #0x28]
003456a4: add      r2, r4, #0x24
003456a8: stm      r0, {r2, r3}
003456ac: str      r0, [r3]
003456b0: str      r0, [r4, #0x28]
003456b4: mov      r0, r5
003456b8: bl       #0x396c44
003456bc: ldrb     r3, [r5, #0xac]
003456c0: cmp      r3, #0
003456c4: bne      #0x345818
003456c8: ldr      r3, [r5, #0xa8]
003456cc: cmp      r3, #0
003456d0: beq      #0x345818
003456d4: add      r6, r4, #0x44
003456d8: mov      r0, r6
003456dc: bl       #0x343168
003456e0: str      r5, [r0, #8]
003456e4: ldr      r2, [r4, #0x48]
003456e8: ldr      r3, [pc, #0x1d8]
003456ec: str      r6, [r0]
003456f0: str      r2, [r0, #4]
003456f4: add      r3, pc, r3
003456f8: str      r0, [r2]
003456fc: str      r0, [r4, #0x48]
00345700: ldr      r3, [r3, #0x20]
00345704: b        #0x3455e4
00345708: add      r2, r2, #1
0034570c: cmp      r2, #4
00345710: str      r2, [r4, #0x7c]
00345714: movne    r0, #1
00345718: bne      #0x345618
0034571c: ldr      r3, [r4, #0x14]
00345720: add      r0, r4, #0x2c
00345724: str      r3, [r5, #0x20]
00345728: bl       #0x34526c
0034572c: add      r0, r4, #0x44
00345730: bl       #0x34526c
00345734: add      r0, r4, #0x34
00345738: bl       #0x34526c
0034573c: mov      r0, #0
00345740: b        #0x345618
00345744: cmp      r2, #1
00345748: bne      #0x3455bc
0034574c: ldr      r3, [r4, #0x14]
00345750: mov      r2, #2
00345754: str      r2, [r4, #0x7c]
00345758: str      r3, [r5, #0x20]
0034575c: ldr      r2, [r4, #0x7c]
00345760: add      r2, r2, #1
00345764: str      r2, [r4, #0x7c]
00345768: b        #0x3455bc
0034576c: ldr      r1, [r3, #4]
00345770: ldr      r0, [r1, #0xc]
00345774: cmp      r0, r3
00345778: bne      #0x345794
0034577c: mov      r3, r1
00345780: ldr      r1, [r1, #4]
00345784: ldr      r2, [r1, #0xc]
00345788: cmp      r3, r2
0034578c: beq      #0x34577c
00345790: ldr      r2, [r3, #0xc]
00345794: cmp      r1, r2
00345798: movne    r3, r1
0034579c: b        #0x345608
003457a0: add      r4, sp, #4
003457a4: ldr      r1, [r3, #0x2c]
003457a8: mov      r0, r4
003457ac: bl       #0x33f524
003457b0: ldr      r2, [sp, #8]
003457b4: add      r6, r6, #4
003457b8: ldr      r3, [sp, #4]
003457bc: str      r2, [r6], #4
003457c0: ldr      r2, [r4, #8]
003457c4: add      r4, sp, #0x10
003457c8: mov      r0, r4
003457cc: mov      r1, #0
003457d0: str      r2, [r6]
003457d4: str      r3, [sp, #0x10]
003457d8: bl       #0x33fdc0
003457dc: cmp      r0, #0
003457e0: beq      #0x345810
003457e4: mov      r1, #1
003457e8: mov      r0, r4
003457ec: bl       #0x33fdc0
003457f0: ldr      r3, [r0]
003457f4: mov      lr, pc
003457f8: ldr      pc, [r3, #0x1c]
003457fc: mov      r1, #1
00345800: mov      r0, r4
00345804: bl       #0x33fdc0
00345808: mov      r1, #0
0034580c: bl       #0x33e6d4
00345810: ldr      r3, [r5, #0x20]
00345814: b        #0x3455e4
00345818: ldrb     r3, [r5, #0xd0]
0034581c: cmp      r3, #0
00345820: bne      #0x34589c
00345824: ldr      r3, [r5, #0xcc]
00345828: cmp      r3, #0
0034582c: bne      #0x3456d4
00345830: ldr      r3, [pc, #0x94]
00345834: add      r3, pc, r3
00345838: ldr      r3, [r3, #0x20]
0034583c: b        #0x3455e4
00345840: ldr      r3, [r5]
00345844: mov      r0, r5
00345848: mov      lr, pc
0034584c: ldr      pc, [r3, #0x38]
00345850: cmp      r0, #0
00345854: beq      #0x3456bc
00345858: add      r6, r4, #0x2c
0034585c: mov      r0, r6
00345860: bl       #0x343168
00345864: str      r5, [r0, #8]
00345868: ldr      r3, [r4, #0x30]
0034586c: str      r6, [r0]
00345870: str      r3, [r0, #4]
00345874: str      r0, [r3]
00345878: str      r0, [r4, #0x30]
0034587c: b        #0x3456bc
00345880: ldr      r0, [r3, #8]
00345884: bl       #0x38a88c
00345888: ldr      r3, [r5, #0x28]
0034588c: ldr      r3, [r3]
00345890: str      r3, [r5, #0x28]
00345894: ldr      r2, [r4, #0x7c]
00345898: b        #0x3455bc
0034589c: ldr      r3, [pc, #0x2c]
003458a0: add      r3, pc, r3
003458a4: ldr      r3, [r3, #0x20]
003458a8: b        #0x3455e4
003458ac: rsbeq    ip, r5, r0, lsl #18
003458b0: rsbeq    ip, r5, ip, ror #17
003458b4: strhteq  ip, [r5], #-0x8c
003458b8: mlseq    r5, r8, r8, ip
003458bc: rsbeq    ip, r5, r8, ror r8
003458c0: rsbeq    ip, r5, ip, lsr #16
003458c4: ldrsheq  sl, [r7], #-0xc4
003458c8: rsbeq    ip, r5, r8, asr #14
003458cc: rsbeq    ip, r5, r8, lsl #12
003458d0: mlseq    r5, ip, r5, ip
