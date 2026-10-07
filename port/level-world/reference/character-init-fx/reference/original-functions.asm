
# _ZN15VisualFXManager16RegisterFXToLoadEi
00496434: push     {r4, r5, r6, r7, r8, sl, lr}
00496438: ldr      r4, [pc, #0x13c]
0049643c: ldr      r6, [pc, #0x13c]
00496440: ldr      r8, [pc, #0x13c]
00496444: add      r4, pc, r4
00496448: ldr      r3, [r4, r6]
0049644c: ldr      r7, [r4, r8]
00496450: sub      sp, sp, #0x4c
00496454: ldr      r3, [r3]
00496458: mov      sl, r0
0049645c: mov      r0, r7
00496460: str      r3, [sp, #0x44]
00496464: str      r1, [sp, #4]
00496468: bl       #0x337888
0049646c: ldr      r1, [pc, #0x114]
00496470: add      r5, sp, #0x2c
00496474: add      r2, sp, #0x10
00496478: add      r1, pc, r1
0049647c: mov      r0, r5
00496480: bl       #0x3140ec
00496484: mov      r0, r7
00496488: mov      r1, r5
0049648c: bl       #0x337ec8
00496490: mov      r7, r0
00496494: ldr      r0, [sp, #0x40]
00496498: cmp      r0, r5
0049649c: beq      #0x4964bc
004964a0: cmp      r0, #0
004964a4: beq      #0x4964bc
004964a8: ldr      r1, [sp, #0x2c]
004964ac: rsb      r1, r0, r1
004964b0: cmp      r1, #0x80
004964b4: bhi      #0x496500
004964b8: bl       #0x708f00
004964bc: cmp      r7, #0
004964c0: beq      #0x4964e4
004964c4: ldr      r3, [sp, #4]
004964c8: cmp      r3, #0
004964cc: blt      #0x4964e4
004964d0: ldr      r2, [pc, #0xb4]
004964d4: ldr      r2, [r4, r2]
004964d8: ldr      r2, [r2]
004964dc: cmp      r3, r2
004964e0: blt      #0x496508
004964e4: ldr      r3, [r4, r6]
004964e8: ldr      r2, [sp, #0x44]
004964ec: ldr      r3, [r3]
004964f0: cmp      r2, r3
004964f4: bne      #0x496578
004964f8: add      sp, sp, #0x4c
004964fc: pop      {r4, r5, r6, r7, r8, sl, pc}
00496500: bl       #0x310440
00496504: b        #0x4964bc
00496508: ldr      r7, [r4, r8]
0049650c: add      r5, sp, #0x14
00496510: mov      r0, r7
00496514: bl       #0x337888
00496518: ldr      r1, [pc, #0x70]
0049651c: add      r2, sp, #0xc
00496520: mov      r0, r5
00496524: add      r1, pc, r1
00496528: bl       #0x3140ec
0049652c: mov      r0, r7
00496530: mov      r1, r5
00496534: bl       #0x337a88
00496538: ldr      r0, [sp, #0x28]
0049653c: cmp      r0, r5
00496540: beq      #0x496560
00496544: cmp      r0, #0
00496548: beq      #0x496560
0049654c: ldr      r1, [sp, #0x14]
00496550: rsb      r1, r0, r1
00496554: cmp      r1, #0x80
00496558: bhi      #0x496570
0049655c: bl       #0x708f00
00496560: add      r0, sl, #0x10
00496564: add      r1, sp, #4
00496568: bl       #0x49437c
0049656c: b        #0x4964e4
00496570: bl       #0x310440
00496574: b        #0x496560
00496578: bl       #0x30e310
0049657c: subeq    lr, pc, ip, asr #12
00496580: andeq    r4, r0, ip, lsr #1
00496584: andeq    r0, r0, r4, lsl #17
00496588: ldrdeq   lr, pc, [r3], #-0xb0
0049658c: andeq    r0, r0, r8, lsl #23
00496590: subeq    lr, r3, r4, lsr fp

# _ZN15VisualFXManager19RegisterFXSetToLoadEi
004967e8: push     {r4, r5, r6, r7, r8, sl, lr}
004967ec: ldr      r4, [pc, #0x148]
004967f0: ldr      r7, [pc, #0x148]
004967f4: ldr      r2, [pc, #0x148]
004967f8: add      r4, pc, r4
004967fc: ldr      r3, [r4, r7]
00496800: ldr      r8, [r4, r2]
00496804: sub      sp, sp, #0x24
00496808: ldr      r3, [r3]
0049680c: mov      r5, r0
00496810: mov      r0, r8
00496814: str      r3, [sp, #0x1c]
00496818: mov      sl, r1
0049681c: bl       #0x337888
00496820: ldr      r1, [pc, #0x120]
00496824: add      r6, sp, #4
00496828: mov      r2, sp
0049682c: add      r1, pc, r1
00496830: mov      r0, r6
00496834: bl       #0x3140ec
00496838: mov      r0, r8
0049683c: mov      r1, r6
00496840: bl       #0x337ec8
00496844: mov      r8, r0
00496848: ldr      r0, [sp, #0x18]
0049684c: cmp      r0, r6
00496850: beq      #0x496870
00496854: cmp      r0, #0
00496858: beq      #0x496870
0049685c: ldr      r1, [sp, #4]
00496860: rsb      r1, r0, r1
00496864: cmp      r1, #0x80
00496868: bhi      #0x496930
0049686c: bl       #0x708f00
00496870: cmp      r8, #0
00496874: beq      #0x496914
00496878: cmp      sl, #0
0049687c: blt      #0x496914
00496880: ldr      r3, [pc, #0xc4]
00496884: ldr      r3, [r4, r3]
00496888: ldr      r3, [r3]
0049688c: cmp      sl, r3
00496890: bge      #0x496914
00496894: ldr      r3, [pc, #0xb4]
00496898: mov      r2, #0x18
0049689c: ldr      r3, [r4, r3]
004968a0: ldr      r3, [r3]
004968a4: mla      sl, r2, sl, r3
004968a8: ldr      r3, [sl, #0xc]
004968ac: cmp      r3, #0
004968b0: ble      #0x496914
004968b4: mov      r6, #0
004968b8: mov      r8, r6
004968bc: b        #0x4968e0
004968c0: ldr      r1, [r3, #4]
004968c4: mov      r0, r5
004968c8: bl       #0x496434
004968cc: ldr      r3, [sl, #0xc]
004968d0: add      r8, r8, #1
004968d4: add      r6, r6, #0x30
004968d8: cmp      r3, r8
004968dc: ble      #0x496914
004968e0: ldr      r3, [sl, #0x10]
004968e4: add      r3, r3, r6
004968e8: ldr      r2, [r3, #0x1c]
004968ec: cmp      r2, #1
004968f0: bne      #0x4968c0
004968f4: ldr      r1, [r3, #4]
004968f8: mov      r0, r5
004968fc: bl       #0x4967e8
00496900: ldr      r3, [sl, #0xc]
00496904: add      r8, r8, #1
00496908: add      r6, r6, #0x30
0049690c: cmp      r3, r8
00496910: bgt      #0x4968e0
00496914: ldr      r3, [r4, r7]
00496918: ldr      r2, [sp, #0x1c]
0049691c: ldr      r3, [r3]
00496920: cmp      r2, r3
00496924: bne      #0x496938
00496928: add      sp, sp, #0x24
0049692c: pop      {r4, r5, r6, r7, r8, sl, pc}
00496930: bl       #0x310440
00496934: b        #0x496870
00496938: bl       #0x30e310
0049693c: umaaleq  lr, pc, r8, r2
00496940: andeq    r4, r0, ip, lsr #1
00496944: andeq    r0, r0, r4, lsl #17
00496948: subeq    lr, r3, ip, lsl r8
0049694c: andeq    r0, r0, r4, asr #13
00496950: andeq    r3, r0, r0, ror sb

# _ZN15VisualFXManager10GrabAnimFXEiP10GameObject
00495430: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00495434: ldr      r4, [pc, #0x260]
00495438: ldr      r5, [pc, #0x260]
0049543c: ldr      ip, [pc, #0x260]
00495440: add      r4, pc, r4
00495444: ldr      r3, [r4, r5]
00495448: ldr      sl, [r4, ip]
0049544c: sub      sp, sp, #0x84
00495450: ldr      r3, [r3]
00495454: mov      r8, r0
00495458: mov      r0, sl
0049545c: str      r3, [sp, #0x7c]
00495460: mov      r7, r1
00495464: mov      sb, r2
00495468: bl       #0x337888
0049546c: ldr      r1, [pc, #0x234]
00495470: add      r6, sp, #0x64
00495474: add      r2, sp, #0x60
00495478: add      r1, pc, r1
0049547c: mov      r0, r6
00495480: bl       #0x3140ec
00495484: mov      r0, sl
00495488: mov      r1, r6
0049548c: bl       #0x337ec8
00495490: mov      sl, r0
00495494: ldr      r0, [sp, #0x78]
00495498: cmp      r0, r6
0049549c: beq      #0x4954bc
004954a0: cmp      r0, #0
004954a4: beq      #0x4954bc
004954a8: ldr      r1, [sp, #0x64]
004954ac: rsb      r1, r0, r1
004954b0: cmp      r1, #0x80
004954b4: bhi      #0x4954e8
004954b8: bl       #0x708f00
004954bc: cmp      sl, #0
004954c0: bne      #0x4954f4
004954c4: mov      r6, #0
004954c8: ldr      r3, [r4, r5]
004954cc: ldr      r2, [sp, #0x7c]
004954d0: mov      r0, r6
004954d4: ldr      r3, [r3]
004954d8: cmp      r2, r3
004954dc: bne      #0x495698
004954e0: add      sp, sp, #0x84
004954e4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004954e8: bl       #0x310440
004954ec: cmp      sl, #0
004954f0: beq      #0x4954c4
004954f4: cmp      r7, #0
004954f8: blt      #0x4954c4
004954fc: ldr      r3, [pc, #0x1a8]
00495500: ldr      r3, [r4, r3]
00495504: ldr      r3, [r3]
00495508: cmp      r7, r3
0049550c: bge      #0x4954c4
00495510: mov      sl, #0x18
00495514: ldr      fp, [r8, #0x1c]
00495518: mul      sl, sl, r7
0049551c: mov      r0, r8
00495520: ldr      r3, [fp, sl]
00495524: add      r1, fp, sl
00495528: str      r1, [sp, #0x14]
0049552c: ldr      r3, [r3, #0x10]
00495530: ldr      r1, [r3, #4]
00495534: bl       #0x494ad4
00495538: subs     r6, r0, #0
0049553c: beq      #0x4954c8
00495540: ldr      r2, [fp, sl]
00495544: ldr      r3, [r2, #0x14]
00495548: cmp      r3, #2
0049554c: movne    r3, #0
00495550: beq      #0x495684
00495554: ldr      r1, [pc, #0x154]
00495558: mov      ip, #0
0049555c: ldr      r2, [r2, #8]
00495560: ldr      r0, [r4, r1]
00495564: mov      r1, r7
00495568: add      r7, sp, #0x1c
0049556c: ldr      lr, [r0, #4]
00495570: ldr      fp, [r0, #8]
00495574: ldr      sl, [r0]
00495578: str      ip, [sp, #4]
0049557c: add      ip, sp, #0x54
00495580: str      ip, [sp, #8]
00495584: mov      r0, r8
00495588: add      ip, sp, #0x48
0049558c: str      lr, [sp, #0x4c]
00495590: str      ip, [sp, #0xc]
00495594: str      lr, [sp, #0x58]
00495598: str      sl, [sp, #0x48]
0049559c: str      sl, [sp, #0x54]
004955a0: str      fp, [sp, #0x50]
004955a4: str      fp, [sp, #0x5c]
004955a8: str      sb, [sp]
004955ac: bl       #0x4935b8
004955b0: ldr      r1, [sp, #0x14]
004955b4: mov      sl, r0
004955b8: mov      r0, r7
004955bc: bl       #0x493924
004955c0: mov      r1, r8
004955c4: mov      r3, sl
004955c8: mov      r2, r7
004955cc: add      r0, sp, #0x34
004955d0: bl       #0x4933e4
004955d4: ldr      r2, [sp, #0x14]
004955d8: mov      r0, r7
004955dc: add      r7, r2, #0x10
004955e0: bl       #0x4940d8
004955e4: mov      r0, r7
004955e8: bl       #0x493814
004955ec: str      sl, [r0, #8]
004955f0: ldr      r1, [sp, #0x14]
004955f4: mov      r3, r0
004955f8: ldr      r2, [r1, #0x14]
004955fc: str      r7, [r0]
00495600: mov      r0, r6
00495604: str      r2, [r3, #4]
00495608: str      r3, [r2]
0049560c: str      r3, [r1, #0x14]
00495610: str      sb, [r6, #0x28]
00495614: mov      r1, #1
00495618: bl       #0x492aa0
0049561c: mov      r0, r6
00495620: mov      r1, #1
00495624: bl       #0x492aa0
00495628: mov      r0, r6
0049562c: mov      r1, #1
00495630: bl       #0x492694
00495634: mov      r0, r6
00495638: bl       #0x492744
0049563c: ldr      lr, [sp, #0x38]
00495640: ldr      r0, [pc, #0x6c]
00495644: ldrb     r1, [sp, #0x34]
00495648: str      lr, [sp]
0049564c: mvn      lr, #0
00495650: ldr      ip, [r4, r0]
00495654: str      lr, [sp, #4]
00495658: ldr      lr, [sp, #0x44]
0049565c: mov      r0, r6
00495660: ldrb     r2, [sp, #0x35]
00495664: ldrb     r3, [sp, #0x36]
00495668: str      lr, [sp, #8]
0049566c: str      ip, [sp, #0xc]
00495670: bl       #0x492e8c
00495674: mov      r0, r6
00495678: mov      r1, #1
0049567c: bl       #0x492ef0
00495680: b        #0x4954c8
00495684: ldr      r0, [r2, #0xc]
00495688: bl       #0x493780
0049568c: ldr      r2, [fp, sl]
00495690: mov      r3, r0
00495694: b        #0x495554
00495698: bl       #0x30e310
0049569c: subeq    pc, pc, r0, asr r6
004956a0: andeq    r4, r0, ip, lsr #1
004956a4: andeq    r0, r0, r4, lsl #17

# _ZNK9Character10GetFXBloodEv
003a33d0: movw     r3, #0x1014
003a33d4: ldr      r2, [r0, r3]
003a33d8: ldr      r3, [pc, #0x4c]
003a33dc: cmp      r2, #0
003a33e0: add      r3, pc, r3
003a33e4: blt      #0x3a3418
003a33e8: ldr      r1, [pc, #0x40]
003a33ec: ldr      r1, [r3, r1]
003a33f0: ldr      r1, [r1]
003a33f4: cmp      r2, r1
003a33f8: bge      #0x3a3418
003a33fc: ldr      r1, [pc, #0x30]
003a3400: ldr      r3, [r3, r1]
003a3404: mov      r1, #0x18
003a3408: ldr      r3, [r3]
003a340c: mla      r2, r1, r2, r3
003a3410: ldr      r0, [r2, #8]
003a3414: bx       lr
003a3418: ldr      r2, [pc, #0x14]
003a341c: ldr      r3, [r3, r2]
003a3420: ldr      r3, [r3]
003a3424: ldr      r0, [r3, #8]
003a3428: bx       lr
003a342c: ldrheq   r1, [pc], #-0x60
003a3430: andeq    r1, r0, ip, ror #3
003a3434: andeq    r0, r0, r4, asr #30

# _ZNK9Character15GetFXBloodDeathEv
003a3368: movw     r3, #0x1014
003a336c: ldr      r2, [r0, r3]
003a3370: ldr      r3, [pc, #0x4c]
003a3374: cmp      r2, #0
003a3378: add      r3, pc, r3
003a337c: blt      #0x3a33b0
003a3380: ldr      r1, [pc, #0x40]
003a3384: ldr      r1, [r3, r1]
003a3388: ldr      r1, [r1]
003a338c: cmp      r2, r1
003a3390: bge      #0x3a33b0
003a3394: ldr      r1, [pc, #0x30]
003a3398: ldr      r3, [r3, r1]
003a339c: mov      r1, #0x18
003a33a0: ldr      r3, [r3]
003a33a4: mla      r2, r1, r2, r3
003a33a8: ldr      r0, [r2, #4]
003a33ac: bx       lr
003a33b0: ldr      r2, [pc, #0x14]
003a33b4: ldr      r3, [r3, r2]
003a33b8: ldr      r3, [r3]
003a33bc: ldr      r0, [r3, #4]
003a33c0: bx       lr
003a33c4: subseq   r1, pc, r8, lsl r7
003a33c8: andeq    r1, r0, ip, ror #3
003a33cc: andeq    r0, r0, r4, asr #30

# _ZN9Character24RegisterCharacterFXTableEv
003b4738: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b473c: ldr      r4, [pc, #0xdc]
003b4740: ldr      r6, [pc, #0xdc]
003b4744: sub      sp, sp, #0x20
003b4748: add      r4, pc, r4
003b474c: ldr      r3, [r4, r6]
003b4750: mov      r7, r0
003b4754: add      r5, sp, #4
003b4758: ldr      r3, [r3]
003b475c: str      r3, [sp, #0x1c]
003b4760: bl       #0x3a3300
003b4764: mov      sl, r0
003b4768: mov      r0, r7
003b476c: bl       #0x3a33d0
003b4770: mov      sb, r0
003b4774: mov      r0, r7
003b4778: bl       #0x3a3368
003b477c: ldr      r3, [pc, #0xa4]
003b4780: mov      r8, r0
003b4784: ldr      r7, [r4, r3]
003b4788: mov      r0, r7
003b478c: bl       #0x337888
003b4790: ldr      r1, [pc, #0x94]
003b4794: mov      r2, sp
003b4798: mov      r0, r5
003b479c: add      r1, pc, r1
003b47a0: bl       #0x3140ec
003b47a4: mov      r1, r5
003b47a8: mov      r0, r7
003b47ac: bl       #0x337a88
003b47b0: mov      r0, r5
003b47b4: bl       #0x318254
003b47b8: cmp      sl, #0
003b47bc: blt      #0x3b47d0
003b47c0: ldr      r3, [pc, #0x68]
003b47c4: mov      r1, sl
003b47c8: ldr      r0, [r4, r3]
003b47cc: bl       #0x4967e8
003b47d0: cmp      sb, #0
003b47d4: blt      #0x3b47e8
003b47d8: ldr      r3, [pc, #0x50]
003b47dc: mov      r1, sb
003b47e0: ldr      r0, [r4, r3]
003b47e4: bl       #0x4967e8
003b47e8: cmp      r8, #0
003b47ec: blt      #0x3b4800
003b47f0: ldr      r3, [pc, #0x38]
003b47f4: mov      r1, r8
003b47f8: ldr      r0, [r4, r3]
003b47fc: bl       #0x4967e8
003b4800: ldr      r3, [r4, r6]
003b4804: ldr      r2, [sp, #0x1c]
003b4808: ldr      r3, [r3]
003b480c: cmp      r2, r3
003b4810: bne      #0x3b481c
003b4814: add      sp, sp, #0x20
003b4818: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b481c: bl       #0x30e310
003b4820: subseq   r0, lr, r8, asr #6
003b4824: andeq    r4, r0, ip, lsr #1
003b4828: andeq    r0, r0, r4, lsl #17
003b482c: subseq   pc, r0, r4, ror r6
003b4830: andeq    r1, r0, r8, lsl #22

# _ZNK9Character14GetFXFootprintEv
003a3300: movw     r3, #0x1014
003a3304: ldr      r2, [r0, r3]
003a3308: ldr      r3, [pc, #0x4c]
003a330c: cmp      r2, #0
003a3310: add      r3, pc, r3
003a3314: blt      #0x3a3348
003a3318: ldr      r1, [pc, #0x40]
003a331c: ldr      r1, [r3, r1]
003a3320: ldr      r1, [r1]
003a3324: cmp      r2, r1
003a3328: bge      #0x3a3348
003a332c: ldr      r1, [pc, #0x30]
003a3330: ldr      r3, [r3, r1]
003a3334: mov      r1, #0x18
003a3338: ldr      r3, [r3]
003a333c: mla      r2, r1, r2, r3
003a3340: ldr      r0, [r2, #0xc]
003a3344: bx       lr
003a3348: ldr      r2, [pc, #0x14]
003a334c: ldr      r3, [r3, r2]
003a3350: ldr      r3, [r3]
003a3354: ldr      r0, [r3, #0xc]
003a3358: bx       lr
003a335c: subseq   r1, pc, r0, lsl #15
003a3360: andeq    r1, r0, ip, ror #3
003a3364: andeq    r0, r0, r4, asr #30
