
# _ZN10GameObject16_GetDistanceFromERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
00393444: push     {r4, r5, r6, r7, r8, lr}
00393448: ldr      ip, [r0, #4]
0039344c: mov      r5, r1
00393450: mov      r4, r2
00393454: ldm      ip, {r1, r6}
00393458: ldr      r3, [pc, #0x170]
0039345c: sub      sp, sp, #0x18
00393460: rsb      r6, r1, r6
00393464: asr      r6, r6, #4
00393468: add      r3, pc, r3
0039346c: add      r2, r6, r6, lsl #3
00393470: add      r2, r2, r2, lsl #6
00393474: add      r2, r6, r2, lsl #3
00393478: add      r2, r2, r2, lsl #15
0039347c: add      r6, r6, r2, lsl #3
00393480: cmp      r6, #0
00393484: bne      #0x393490
00393488: add      sp, sp, #0x18
0039348c: pop      {r4, r5, r6, r7, r8, pc}
00393490: ldr      r2, [r1, #4]
00393494: cmp      r2, #4
00393498: beq      #0x39357c
0039349c: cmp      r2, #7
003934a0: bne      #0x393488
003934a4: ldr      r6, [r0, #4]
003934a8: ldm      r6, {r0, r3}
003934ac: rsb      r3, r0, r3
003934b0: asr      r3, r3, #4
003934b4: add      r2, r3, r3, lsl #3
003934b8: add      r2, r2, r2, lsl #6
003934bc: add      r2, r3, r2, lsl #3
003934c0: add      r2, r2, r2, lsl #15
003934c4: add      r3, r3, r2, lsl #3
003934c8: cmp      r3, #0
003934cc: bne      #0x3934e0
003934d0: ldr      r0, [pc, #0xfc]
003934d4: add      r0, pc, r0
003934d8: bl       #0x708eb0
003934dc: ldr      r0, [r6]
003934e0: bl       #0x31b5a0
003934e4: mov      r6, r0
003934e8: cmp      r6, #0
003934ec: moveq    r1, #0xbf000000
003934f0: addeq    r1, r1, #0x800000
003934f4: beq      #0x393570
003934f8: ldr      r1, [r4, #0x160]
003934fc: ldr      r0, [r6, #0x160]
00393500: bl       #0x30e3ac
00393504: ldr      r1, [r4, #0x164]
00393508: mov      r8, r0
0039350c: ldr      r0, [r6, #0x164]
00393510: bl       #0x30e3ac
00393514: ldr      r1, [r4, #0x168]
00393518: mov      r7, r0
0039351c: ldr      r0, [r6, #0x168]
00393520: bl       #0x30e3ac
00393524: mov      r1, r8
00393528: mov      r6, r0
0039352c: mov      r0, r8
00393530: bl       #0x30ed6c
00393534: mov      r1, r7
00393538: mov      r4, r0
0039353c: mov      r0, r7
00393540: bl       #0x30ed6c
00393544: mov      r1, r0
00393548: mov      r0, r4
0039354c: bl       #0x30eba4
00393550: mov      r1, r6
00393554: mov      r4, r0
00393558: mov      r0, r6
0039355c: bl       #0x30ed6c
00393560: mov      r1, r0
00393564: mov      r0, r4
00393568: bl       #0x30eba4
0039356c: mov      r1, r0
00393570: mov      r0, r5
00393574: bl       #0x37ccbc
00393578: b        #0x393488
0039357c: cmp      r2, #4
00393580: bne      #0x3934a4
00393584: ldr      r2, [pc, #0x4c]
00393588: mov      r1, #0
0039358c: add      r6, sp, #0xc
00393590: ldr      r3, [r3, r2]
00393594: ldr      r7, [r3, #0x38]
00393598: bl       #0x37baf8
0039359c: bl       #0x31c49c
003935a0: mov      ip, #0
003935a4: mov      r2, r0
003935a8: mov      r1, r7
003935ac: mov      r0, r6
003935b0: mvn      r3, #0
003935b4: str      ip, [sp, #4]
003935b8: str      ip, [sp]
003935bc: bl       #0x34aca0
003935c0: mov      r0, r6
003935c4: bl       #0x33fee4
003935c8: mov      r6, r0
003935cc: b        #0x3934e8
003935d0: rsbeq    r1, r0, r8, lsr #12

# _ZN10GameObject19_GetDistanceBetweenERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
00391438: push     {r4, r5, r6, r7, r8, sl, lr}
0039143c: ldr      r7, [r0, #4]
00391440: mov      r6, r1
00391444: ldr      r4, [pc, #0x210]
00391448: ldm      r7, {r1, r3}
0039144c: add      r4, pc, r4
00391450: sub      sp, sp, #0x24
00391454: rsb      r3, r1, r3
00391458: asr      r3, r3, #4
0039145c: mov      r5, r0
00391460: add      r2, r3, r3, lsl #3
00391464: add      r2, r2, r2, lsl #6
00391468: add      r2, r3, r2, lsl #3
0039146c: add      r2, r2, r2, lsl #15
00391470: add      r3, r3, r2, lsl #3
00391474: rsb      r3, r3, #0
00391478: cmp      r3, #1
0039147c: bls      #0x391494
00391480: cmp      r3, #0
00391484: beq      #0x39149c
00391488: ldr      r3, [r1, #4]
0039148c: cmp      r3, #4
00391490: beq      #0x3914b0
00391494: add      sp, sp, #0x24
00391498: pop      {r4, r5, r6, r7, r8, sl, pc}
0039149c: ldr      r0, [pc, #0x1bc]
003914a0: add      r0, pc, r0
003914a4: bl       #0x708eb0
003914a8: ldr      r1, [r7]
003914ac: b        #0x391488
003914b0: mov      r0, r5
003914b4: mov      r1, #1
003914b8: bl       #0x37baf8
003914bc: ldr      r3, [r0, #4]
003914c0: cmp      r3, #4
003914c4: bne      #0x391494
003914c8: ldr      r7, [r5, #4]
003914cc: ldr      r8, [pc, #0x190]
003914d0: ldm      r7, {r0, r3}
003914d4: ldr      r2, [r4, r8]
003914d8: rsb      r3, r0, r3
003914dc: asr      r3, r3, #4
003914e0: ldr      sl, [r2, #0x38]
003914e4: add      r2, r3, r3, lsl #3
003914e8: add      r2, r2, r2, lsl #6
003914ec: add      r2, r3, r2, lsl #3
003914f0: add      r2, r2, r2, lsl #15
003914f4: add      r3, r3, r2, lsl #3
003914f8: cmp      r3, #0
003914fc: bne      #0x391510
00391500: ldr      r0, [pc, #0x160]
00391504: add      r0, pc, r0
00391508: bl       #0x708eb0
0039150c: ldr      r0, [r7]
00391510: bl       #0x31c49c
00391514: add      r7, sp, #0x14
00391518: mov      r2, r0
0039151c: mov      ip, #0
00391520: mvn      r3, #0
00391524: mov      r0, r7
00391528: mov      r1, sl
0039152c: str      ip, [sp, #4]
00391530: str      ip, [sp]
00391534: bl       #0x34aca0
00391538: mov      r0, r7
0039153c: bl       #0x33fee4
00391540: ldr      r5, [r5, #4]
00391544: mov      r7, r0
00391548: ldr      r2, [r4, r8]
0039154c: ldm      r5, {r0, r3}
00391550: ldr      r8, [r2, #0x38]
00391554: rsb      r3, r0, r3
00391558: asr      r3, r3, #4
0039155c: add      r2, r3, r3, lsl #3
00391560: add      r2, r2, r2, lsl #6
00391564: add      r2, r3, r2, lsl #3
00391568: add      r2, r2, r2, lsl #15
0039156c: add      r3, r3, r2, lsl #3
00391570: rsb      r3, r3, #0
00391574: cmp      r3, #1
00391578: bhi      #0x39158c
0039157c: ldr      r0, [pc, #0xe8]
00391580: add      r0, pc, r0
00391584: bl       #0x708eb0
00391588: ldr      r0, [r5]
0039158c: add      r0, r0, #0x70
00391590: bl       #0x31c49c
00391594: add      r4, sp, #8
00391598: mov      r1, r8
0039159c: mov      ip, #0
003915a0: mov      r2, r0
003915a4: mvn      r3, #0
003915a8: mov      r0, r4
003915ac: str      ip, [sp, #4]
003915b0: str      ip, [sp]
003915b4: bl       #0x34aca0
003915b8: mov      r0, r4
003915bc: bl       #0x33fee4
003915c0: cmp      r0, #0
003915c4: cmpne    r7, #0
003915c8: moveq    r1, #0xbf000000
003915cc: mov      r4, r0
003915d0: addeq    r1, r1, #0x800000
003915d4: beq      #0x391650
003915d8: ldr      r1, [r0, #0x160]
003915dc: ldr      r0, [r7, #0x160]
003915e0: bl       #0x30e3ac
003915e4: ldr      r1, [r4, #0x164]
003915e8: mov      sl, r0
003915ec: ldr      r0, [r7, #0x164]
003915f0: bl       #0x30e3ac
003915f4: ldr      r1, [r4, #0x168]
003915f8: mov      r8, r0
003915fc: ldr      r0, [r7, #0x168]
00391600: bl       #0x30e3ac
00391604: mov      r1, sl
00391608: mov      r5, r0
0039160c: mov      r0, sl
00391610: bl       #0x30ed6c
00391614: mov      r1, r8
00391618: mov      r4, r0
0039161c: mov      r0, r8
00391620: bl       #0x30ed6c
00391624: mov      r1, r0
00391628: mov      r0, r4
0039162c: bl       #0x30eba4
00391630: mov      r1, r5
00391634: mov      r4, r0
00391638: mov      r0, r5
0039163c: bl       #0x30ed6c
00391640: mov      r1, r0
00391644: mov      r0, r4
00391648: bl       #0x30eba4
0039164c: mov      r1, r0
00391650: mov      r0, r6
00391654: bl       #0x37ccbc
00391658: b        #0x391494
0039165c: rsbeq    r3, r0, r4, asr #12
00391660: subseq   ip, r2, r8, asr #31
00391664: strdeq   r3, r4, [r0], -r4
00391668: subseq   ip, r2, r4, ror #30
0039166c: subseq   ip, r2, r8, ror #29

# _ZNK3sfc6script3lua5Value11getUserDataEv
0031b5a0: ldr      r3, [r0, #4]
0031b5a4: cmp      r3, #2
0031b5a8: beq      #0x31b5b8
0031b5ac: cmp      r3, #7
0031b5b0: movne    r0, #0
0031b5b4: bxne     lr
0031b5b8: ldr      r0, [r0, #0x6c]
0031b5bc: bx       lr

# _ZN10GameObject12_GetPositionERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0038e700: push     {r4, r5, r6, lr}
0038e704: mov      r0, r1
0038e708: mov      r4, r2
0038e70c: mov      r5, r1
0038e710: ldr      r1, [r2, #0x160]
0038e714: bl       #0x37ccbc
0038e718: mov      r0, r5
0038e71c: ldr      r1, [r4, #0x164]
0038e720: bl       #0x37ccbc
0038e724: ldr      r1, [r4, #0x168]
0038e728: mov      r0, r5
0038e72c: pop      {r4, r5, r6, lr}
0038e730: b        #0x37ccbc
