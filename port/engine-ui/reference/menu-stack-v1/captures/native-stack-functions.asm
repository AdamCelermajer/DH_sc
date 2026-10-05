
# _Z14NativePushMenuRKN7gameswf7fn_callE 0x43b1b4
0043b1b4: push     {r4, lr}
0043b1b8: ldr      r3, [r0, #0xc]
0043b1bc: ldr      r2, [r0, #0x14]
0043b1c0: mov      r0, #0xc
0043b1c4: ldr      r3, [r3]
0043b1c8: mla      r0, r0, r2, r3
0043b1cc: bl       #0x796f5c
0043b1d0: mov      r4, r0
0043b1d4: bl       #0x42ca8c
0043b1d8: mov      r1, r4
0043b1dc: pop      {r4, lr}
0043b1e0: b        #0x431924

# _Z13NativePopMenuRKN7gameswf7fn_callE 0x43b158
0043b158: push     {r4, r5, r6, lr}
0043b15c: mov      r4, r0
0043b160: bl       #0x42ca8c
0043b164: ldr      r1, [r4, #0x10]
0043b168: mov      r5, r0
0043b16c: cmp      r1, #0
0043b170: beq      #0x43b19c
0043b174: ldr      r3, [r4, #0xc]
0043b178: ldr      r2, [r4, #0x14]
0043b17c: mov      r0, #0xc
0043b180: ldr      r3, [r3]
0043b184: mla      r0, r0, r2, r3
0043b188: bl       #0x796f5c
0043b18c: mov      r1, r0
0043b190: mov      r0, r5
0043b194: pop      {r4, r5, r6, lr}
0043b198: b        #0x42e2b0
0043b19c: ldr      r3, [r0, #0xf4]
0043b1a0: mov      r0, r3
0043b1a4: ldr      r3, [r3]
0043b1a8: mov      lr, pc
0043b1ac: ldr      pc, [r3, #0x38]
0043b1b0: pop      {r4, r5, r6, pc}

# _Z17NativePopAllAboveRKN7gameswf7fn_callE 0x43ac28
0043ac28: push     {r4, r5, r6, lr}
0043ac2c: ldr      r4, [r0, #0x10]
0043ac30: cmp      r4, #1
0043ac34: beq      #0x43ac3c
0043ac38: pop      {r4, r5, r6, pc}
0043ac3c: ldr      r3, [r0, #0xc]
0043ac40: ldr      r2, [r0, #0x14]
0043ac44: mov      r0, #0xc
0043ac48: ldr      r3, [r3]
0043ac4c: mla      r0, r0, r2, r3
0043ac50: ldrb     r3, [r0, #1]
0043ac54: sub      r3, r3, #3
0043ac58: uxtb     r3, r3
0043ac5c: cmp      r3, #1
0043ac60: bhi      #0x43ac38
0043ac64: bl       #0x796f5c
0043ac68: mov      r5, r0
0043ac6c: bl       #0x42ca8c
0043ac70: ldr      r3, [r0, #0xf4]
0043ac74: mov      r1, r5
0043ac78: mov      r2, r4
0043ac7c: mov      r0, r3
0043ac80: ldr      r3, [r3]
0043ac84: mov      lr, pc
0043ac88: ldr      pc, [r3, #0x3c]
0043ac8c: b        #0x43ac38

# _Z17NativePopAllMenusRKN7gameswf7fn_callE 0x439dd8
00439dd8: push     {r4, lr}
00439ddc: bl       #0x42ca8c
00439de0: ldr      r3, [r0, #0xf4]
00439de4: mov      r1, #1
00439de8: mov      r0, r3
00439dec: ldr      r3, [r3]
00439df0: mov      lr, pc
00439df4: ldr      pc, [r3, #0x38]
00439df8: pop      {r4, pc}

# _Z15NativeBackToHudRKN7gameswf7fn_callE 0x444990
00444990: push     {r4, r5, r6, r7, r8, sl, lr}
00444994: ldr      r4, [pc, #0x130]
00444998: ldr      r5, [pc, #0x130]
0044499c: sub      sp, sp, #0x24
004449a0: add      r4, pc, r4
004449a4: ldr      r8, [r4, r5]
004449a8: mov      r0, r8
004449ac: bl       #0x31f594
004449b0: cmp      r0, #0
004449b4: beq      #0x444a2c
004449b8: ldr      r3, [pc, #0x114]
004449bc: mov      r7, #1
004449c0: strb     r7, [r0, #0x198]
004449c4: ldr      r3, [r4, r3]
004449c8: ldrb     r6, [r3, #0x30]
004449cc: cmp      r6, #0
004449d0: beq      #0x444a34
004449d4: ldr      r4, [r4, r5]
004449d8: ldr      r0, [r4, #0x50]
004449dc: bl       #0x3830bc
004449e0: ldr      r3, [r4, #0x54]
004449e4: ldr      r0, [r3, #0xf4]
004449e8: bl       #0x43799c
004449ec: bl       #0x41b11c
004449f0: bl       #0x418bec
004449f4: ldr      r0, [r4, #0x40]
004449f8: mov      r1, #0
004449fc: mov      r2, #1
00444a00: bl       #0x36e478
00444a04: ldr      r3, [r0, #0x660]
00444a08: cmp      r3, #0
00444a0c: beq      #0x444a2c
00444a10: ldr      r0, [r4, #0x40]
00444a14: mov      r1, #0
00444a18: mov      r2, #1
00444a1c: bl       #0x36e478
00444a20: ldr      r0, [r0, #0x660]
00444a24: add      r0, r0, #0x37c
00444a28: bl       #0x3fcff8
00444a2c: add      sp, sp, #0x24
00444a30: pop      {r4, r5, r6, r7, r8, sl, pc}
00444a34: ldr      r0, [r8, #0x54]
00444a38: bl       #0x42cb8c
00444a3c: mov      r8, r0
00444a40: bl       #0x7a7cac
00444a44: bl       #0x774154
00444a48: ldr      r2, [pc, #0x88]
00444a4c: mov      r1, r0
00444a50: mov      r3, r6
00444a54: add      r2, pc, r2
00444a58: mov      r0, r8
00444a5c: str      r6, [sp]
00444a60: bl       #0x7abe0c
00444a64: mov      r0, r8
00444a68: bl       #0x7a7cac
00444a6c: bl       #0x774154
00444a70: mov      sl, r0
00444a74: bl       #0x42ca8c
00444a78: mov      r3, #2
00444a7c: ldr      r0, [r0, #0x108]
00444a80: strb     r6, [sp, #0xc]
00444a84: strb     r3, [sp, #0xd]
00444a88: bl       #0x30ed30
00444a8c: strd     r0, r1, [sp, #0x18]
00444a90: ldr      ip, [sp, #0x18]
00444a94: ldr      r2, [pc, #0x40]
00444a98: add      r6, sp, #0xc
00444a9c: str      ip, [sp, #0x10]
00444aa0: ldr      ip, [sp, #0x1c]
00444aa4: mov      r0, r8
00444aa8: mov      r1, sl
00444aac: str      ip, [r6, #8]
00444ab0: add      r2, pc, r2
00444ab4: mov      r3, r6
00444ab8: str      r7, [sp]
00444abc: bl       #0x7abe0c
00444ac0: mov      r0, r6
00444ac4: bl       #0x797124
00444ac8: b        #0x4449d4
00444acc: ldrsheq  r0, [r5], #-0
00444ad0: strdeq   r3, r4, [r0], -r4
00444ad4: andeq    r1, r0, r0, lsr #20
00444ad8: ldrdeq   r7, r8, [r8], #-0x3c
00444adc: subeq    r5, r8, r8, asr #13

# _ZN11MenuManager8PushMenuEP8MenuBase 0x4317e8
004317e8: push     {r4, r5, r6, r7, r8, sl, lr}
004317ec: ldr      r4, [pc, #0x11c]
004317f0: ldr      r5, [pc, #0x11c]
004317f4: sub      sp, sp, #0x24
004317f8: add      r4, pc, r4
004317fc: ldr      r3, [r4, r5]
00431800: subs     r6, r1, #0
00431804: mov      r7, r0
00431808: ldr      r3, [r3]
0043180c: str      r3, [sp, #0x1c]
00431810: beq      #0x431840
00431814: ldr      r3, [r6]
00431818: mov      r0, r6
0043181c: mov      lr, pc
00431820: ldr      pc, [r3, #0x3c]
00431824: cmp      r0, #0
00431828: bne      #0x43185c
0043182c: mov      r0, r7
00431830: ldr      r8, [r6, #4]
00431834: bl       #0x42cb8c
00431838: cmp      r8, r0
0043183c: beq      #0x4318fc
00431840: ldr      r3, [r4, r5]
00431844: ldr      r2, [sp, #0x1c]
00431848: ldr      r3, [r3]
0043184c: cmp      r2, r3
00431850: bne      #0x43190c
00431854: add      sp, sp, #0x24
00431858: pop      {r4, r5, r6, r7, r8, sl, pc}
0043185c: ldr      r3, [r7, #0xf4]
00431860: mov      r1, r6
00431864: mov      r0, r3
00431868: ldr      r3, [r3]
0043186c: mov      lr, pc
00431870: ldr      pc, [r3, #0x40]
00431874: cmp      r0, #0
00431878: bne      #0x43182c
0043187c: ldr      r3, [pc, #0x94]
00431880: add      r8, sp, #4
00431884: ldr      sl, [r4, r3]
00431888: mov      r0, sl
0043188c: bl       #0x328f40
00431890: ldr      r0, [sl, #0x20]
00431894: bl       #0x33c568
00431898: ldr      r3, [r7, #0xf4]
0043189c: mov      r1, r6
004318a0: mov      r0, r3
004318a4: ldr      r3, [r3]
004318a8: mov      lr, pc
004318ac: ldr      pc, [r3, #0x34]
004318b0: ldr      r3, [pc, #0x64]
004318b4: ldr      sl, [r4, r3]
004318b8: mov      r0, sl
004318bc: bl       #0x337888
004318c0: ldr      r1, [pc, #0x58]
004318c4: mov      r2, sp
004318c8: mov      r0, r8
004318cc: add      r1, pc, r1
004318d0: bl       #0x3140ec
004318d4: mov      r1, r8
004318d8: mov      r0, sl
004318dc: bl       #0x337a88
004318e0: mov      r0, r8
004318e4: bl       #0x318254
004318e8: mov      r0, r7
004318ec: ldr      r8, [r6, #4]
004318f0: bl       #0x42cb8c
004318f4: cmp      r8, r0
004318f8: bne      #0x431840
004318fc: mov      r0, r7
00431900: mov      r1, r6
00431904: bl       #0x431750
00431908: b        #0x431840
0043190c: bl       #0x30e310

# _ZN11MenuManager7PopMenuEP8MenuBase 0x42e208
0042e208: push     {r4, r5, r6, lr}
0042e20c: ldr      r4, [pc, #0x94]
0042e210: subs     r6, r1, #0
0042e214: mov      r5, r0
0042e218: add      r4, pc, r4
0042e21c: beq      #0x42e24c
0042e220: ldr      r3, [r6, #4]
0042e224: cmp      r3, #0
0042e228: moveq    r4, r3
0042e22c: beq      #0x42e250
0042e230: ldr      r3, [r0, #0xf4]
0042e234: mov      r0, r3
0042e238: ldr      r3, [r3]
0042e23c: mov      lr, pc
0042e240: ldr      pc, [r3, #0x40]
0042e244: cmp      r0, #0
0042e248: bne      #0x42e264
0042e24c: ldr      r4, [r6, #4]
0042e250: mov      r0, r5
0042e254: bl       #0x42cb8c
0042e258: cmp      r4, r0
0042e25c: beq      #0x42e298
0042e260: pop      {r4, r5, r6, pc}
0042e264: ldr      r3, [pc, #0x40]
0042e268: ldr      r4, [r4, r3]
0042e26c: mov      r0, r4
0042e270: bl       #0x328f40
0042e274: ldr      r0, [r4, #0x20]
0042e278: bl       #0x33c568
0042e27c: ldr      r3, [r5, #0xf4]
0042e280: mov      r1, #0
0042e284: mov      r0, r3
0042e288: ldr      r3, [r3]
0042e28c: mov      lr, pc
0042e290: ldr      pc, [r3, #0x38]
0042e294: b        #0x42e24c
0042e298: mov      r0, r5
0042e29c: mov      r1, r6
0042e2a0: pop      {r4, r5, r6, lr}
0042e2a4: b        #0x42e110
0042e2a8: subseq   r6, r6, r8, ror r8
0042e2ac: strdeq   r3, r4, [r0], -r4

# _ZN11MenuManager8PushMenuEPKc 0x431924
00431924: push     {r4, lr}
00431928: mov      r4, r0
0043192c: bl       #0x42d1f0
00431930: subs     r1, r0, #0
00431934: beq      #0x431944
00431938: mov      r0, r4
0043193c: pop      {r4, lr}
00431940: b        #0x4317e8
00431944: pop      {r4, pc}

# _ZN11MenuManager7PopMenuEPKc 0x42e2b0
0042e2b0: push     {r4, lr}
0042e2b4: mov      r4, r0
0042e2b8: bl       #0x42d1f0
0042e2bc: subs     r1, r0, #0
0042e2c0: beq      #0x42e2d0
0042e2c4: mov      r0, r4
0042e2c8: pop      {r4, lr}
0042e2cc: b        #0x42e208
0042e2d0: pop      {r4, pc}

# _ZN11MenuManager7HideAllEv 0x42d234
0042d234: push     {r4, r5, r6, lr}
0042d238: mov      r5, r0
0042d23c: bl       #0x42cb38
0042d240: subs     r6, r0, #0
0042d244: ble      #0x42d28c
0042d248: mov      r4, #0
0042d24c: b        #0x42d25c
0042d250: add      r4, r4, #1
0042d254: cmp      r6, r4
0042d258: beq      #0x42d28c
0042d25c: ldr      r3, [r5, #0x64]
0042d260: ldr      r0, [r3, r4, lsl #2]
0042d264: bl       #0x41f3f4
0042d268: cmp      r0, #0
0042d26c: beq      #0x42d250
0042d270: ldr      r3, [r5, #0x64]
0042d274: mov      r1, #0
0042d278: ldr      r0, [r3, r4, lsl #2]
0042d27c: add      r4, r4, #1
0042d280: bl       #0x4223bc
0042d284: cmp      r6, r4
0042d288: bne      #0x42d25c
0042d28c: pop      {r4, r5, r6, pc}

# _ZN11MenuManager13GetMenuByNameEPKc 0x42d1f0
0042d1f0: push     {r4, r5, r6, r7, r8, lr}
0042d1f4: mov      r7, r1
0042d1f8: ldr      r6, [r0, #0x68]
0042d1fc: ldr      r4, [r0, #0x64]
0042d200: b        #0x42d220
0042d204: ldr      r5, [r4]
0042d208: mov      r0, r7
0042d20c: add      r4, r4, #4
0042d210: add      r1, r5, #8
0042d214: bl       #0x30e31c
0042d218: cmp      r0, #0
0042d21c: beq      #0x42d22c
0042d220: cmp      r4, r6
0042d224: bne      #0x42d204
0042d228: mov      r5, #0
0042d22c: mov      r0, r5
0042d230: pop      {r4, r5, r6, r7, r8, pc}

# _ZN16MultiMenuManager8PushMenuEP8MenuBase 0x438278
00438278: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0043827c: mov      r8, r1
00438280: sub      sp, sp, #0xc
00438284: mov      r6, r0
00438288: add      r1, r1, #8
0043828c: ldr      r0, [r8, #4]
00438290: bl       #0x7a82c4
00438294: ldr      r7, [pc, #0x8b4]
00438298: subs     r4, r0, #0
0043829c: add      r7, pc, r7
004382a0: beq      #0x4384c8
004382a4: ldr      r0, [pc, #0x8a8]
004382a8: add      r5, r4, #8
004382ac: mov      r1, r5
004382b0: add      r0, pc, r0
004382b4: bl       #0x324114
004382b8: ldr      r1, [pc, #0x898]
004382bc: mov      r0, r5
004382c0: add      r1, pc, r1
004382c4: bl       #0x30e31c
004382c8: cmp      r0, #0
004382cc: beq      #0x438514
004382d0: ldr      r1, [pc, #0x884]
004382d4: mov      r0, r5
004382d8: add      r1, pc, r1
004382dc: bl       #0x30e31c
004382e0: cmp      r0, #0
004382e4: beq      #0x4385b0
004382e8: ldr      r1, [pc, #0x870]
004382ec: mov      r0, r5
004382f0: add      r1, pc, r1
004382f4: bl       #0x30e31c
004382f8: subs     sl, r0, #0
004382fc: beq      #0x438584
00438300: ldr      r1, [pc, #0x85c]
00438304: mov      r0, r5
00438308: add      r1, pc, r1
0043830c: bl       #0x30e31c
00438310: cmp      r0, #0
00438314: bne      #0x4384d0
00438318: ldr      r3, [pc, #0x848]
0043831c: mov      r2, #1
00438320: ldr      r3, [r7, r3]
00438324: strb     r2, [r3]
00438328: ldr      r3, [pc, #0x83c]
0043832c: mov      sl, #0
00438330: ldr      r3, [r7, r3]
00438334: strb     sl, [r3]
00438338: ldr      sb, [r6, #0x128]
0043833c: cmp      sb, sl
00438340: ble      #0x438554
00438344: ldr      r3, [r6, #0x124]
00438348: sub      r2, sb, #1
0043834c: ldr      r7, [r3, r2, lsl #2]
00438350: ldr      r3, [r7, #0x118]
00438354: cmp      r3, #0
00438358: ble      #0x438554
0043835c: mov      r0, r7
00438360: bl       #0x7a7f30
00438364: mov      fp, r0
00438368: ldr      r3, [fp], #8
0043836c: mov      sb, r0
00438370: mov      lr, pc
00438374: ldr      pc, [r3, #0x18]
00438378: ldr      r2, [pc, #0x7f0]
0043837c: mov      r1, fp
00438380: mov      r3, sl
00438384: add      r2, pc, r2
00438388: mov      r0, r7
0043838c: str      sl, [sp]
00438390: bl       #0x7ad7e8
00438394: ldr      fp, [r7, #0xf8]
00438398: ands     fp, fp, #0x40
0043839c: beq      #0x438750
004383a0: mov      r1, #0
004383a4: mov      r0, r7
004383a8: bl       #0x7a7c74
004383ac: ldr      r1, [r0, #0x10]
004383b0: add      r0, sb, #0x50
004383b4: bl       #0x427ba8
004383b8: ldr      r3, [r7, #0xf8]
004383bc: tst      r3, #8
004383c0: addeq    r7, r4, #0x48
004383c4: bne      #0x4386d4
004383c8: ldr      sb, [r6, #0x128]
004383cc: ldr      r3, [r6, #0x12c]
004383d0: ldr      r8, [r8, #4]
004383d4: add      sl, sb, #1
004383d8: cmp      sl, r3
004383dc: movle    r2, sb
004383e0: bgt      #0x438570
004383e4: ldr      r3, [r6, #0x124]
004383e8: str      r8, [r3, r2, lsl #2]
004383ec: ldr      r3, [r6, #0x124]
004383f0: str      sl, [r6, #0x128]
004383f4: ldr      r6, [r3, sb, lsl #2]
004383f8: ldr      r8, [r6, #0x118]
004383fc: adds     sl, r8, #1
00438400: beq      #0x438410
00438404: ldr      r3, [r6, #0x11c]
00438408: cmp      sl, r3
0043840c: bgt      #0x438714
00438410: ldr      r3, [r6, #0x114]
00438414: mov      r2, #0
00438418: str      r2, [r3, r8, lsl #2]
0043841c: ldr      r3, [r6, #0x114]
00438420: str      sl, [r6, #0x118]
00438424: str      r4, [r3, r8, lsl #2]
00438428: ldr      r3, [r4, #0x4c]
0043842c: cmp      r3, r2
00438430: beq      #0x438444
00438434: ldr      r0, [r4, #0x48]
00438438: ldrb     r2, [r0, #4]
0043843c: cmp      r2, #0
00438440: beq      #0x438664
00438444: mov      r2, #1
00438448: strb     r2, [r3, #0x9b]
0043844c: ldr      r3, [r6, #0xf8]
00438450: tst      r3, #8
00438454: bne      #0x438618
00438458: mov      r0, r7
0043845c: bl       #0x438224
00438460: mov      r1, r0
00438464: mov      r0, r6
00438468: bl       #0x7a7ee8
0043846c: ldr      r2, [pc, #0x700]
00438470: mov      ip, #0
00438474: mov      r1, r5
00438478: mov      r3, ip
0043847c: add      r2, pc, r2
00438480: mov      r0, r6
00438484: str      ip, [sp]
00438488: bl       #0x7ad7e8
0043848c: ldr      r3, [r6, #0xf8]
00438490: ands     r5, r3, #0x40
00438494: beq      #0x4385f0
00438498: tst      r3, #1
0043849c: bne      #0x4385dc
004384a0: mov      r0, r4
004384a4: ldr      r3, [r4]
004384a8: mov      lr, pc
004384ac: ldr      pc, [r3, #0xc]
004384b0: ldr      r3, [r4]
004384b4: mov      r0, r4
004384b8: mov      lr, pc
004384bc: ldr      pc, [r3, #0x14]
004384c0: mov      r3, #1
004384c4: str      r3, [r4, #0x58]
004384c8: add      sp, sp, #0xc
004384cc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004384d0: ldr      r1, [pc, #0x6a0]
004384d4: mov      r0, r5
004384d8: add      r1, pc, r1
004384dc: bl       #0x30e31c
004384e0: cmp      r0, #0
004384e4: beq      #0x43868c
004384e8: ldr      r1, [pc, #0x68c]
004384ec: mov      r0, r5
004384f0: add      r1, pc, r1
004384f4: bl       #0x30e31c
004384f8: cmp      r0, #0
004384fc: bne      #0x438724
00438500: ldr      r3, [pc, #0x678]
00438504: mov      r2, #2
00438508: ldr      r3, [r7, r3]
0043850c: str      r2, [r3]
00438510: b        #0x438328
00438514: ldr      r3, [pc, #0x668]
00438518: ldr      r3, [r7, r3]
0043851c: ldrb     r3, [r3]
00438520: cmp      r3, #0
00438524: bne      #0x43870c
00438528: ldr      r0, [pc, #0x658]
0043852c: add      r0, pc, r0
00438530: bl       #0x324114
00438534: ldr      r2, [pc, #0x650]
00438538: mov      r3, #1
0043853c: ldr      r1, [r7, r2]
00438540: ldr      r2, [pc, #0x648]
00438544: strb     r3, [r1]
00438548: ldr      r2, [r7, r2]
0043854c: strb     r3, [r2]
00438550: b        #0x4382d0
00438554: ldr      r3, [r6, #0x12c]
00438558: add      sl, sb, #1
0043855c: add      r7, r4, #0x48
00438560: cmp      sl, r3
00438564: ldr      r8, [r8, #4]
00438568: movle    r2, sb
0043856c: ble      #0x4383e4
00438570: add      r0, r6, #0x124
00438574: add      r1, sl, sl, asr #1
00438578: bl       #0x437a6c
0043857c: ldr      r2, [r6, #0x128]
00438580: b        #0x4383e4
00438584: ldr      r0, [pc, #0x608]
00438588: add      r0, pc, r0
0043858c: bl       #0x324114
00438590: ldr      r3, [pc, #0x5f4]
00438594: mov      r1, #1
00438598: ldr      r2, [r7, r3]
0043859c: ldr      r3, [pc, #0x5ec]
004385a0: strb     r1, [r2]
004385a4: ldr      r3, [r7, r3]
004385a8: strb     sl, [r3]
004385ac: b        #0x438300
004385b0: ldr      r0, [pc, #0x5e0]
004385b4: add      r0, pc, r0
004385b8: bl       #0x324114
004385bc: ldr      r2, [pc, #0x5c8]
004385c0: mov      r3, #1
004385c4: ldr      r1, [r7, r2]
004385c8: ldr      r2, [pc, #0x5c0]
004385cc: strb     r3, [r1]
004385d0: ldr      r2, [r7, r2]
004385d4: strb     r3, [r2]
004385d8: b        #0x4382e8
004385dc: mov      r0, r6
004385e0: ldr      r3, [r6]
004385e4: mov      lr, pc
004385e8: ldr      pc, [r3, #0x24]
004385ec: b        #0x4384a0
004385f0: mov      r0, r7
004385f4: bl       #0x438224
004385f8: ldr      r2, [pc, #0x59c]
004385fc: mov      r1, r0
00438600: mov      r3, r5
00438604: add      r2, pc, r2
00438608: mov      r0, r6
0043860c: bl       #0x7aba04
00438610: ldr      r3, [r6, #0xf8]
00438614: b        #0x438498
00438618: ldr      r3, [r4, #0x4c]
0043861c: cmp      r3, #0
00438620: beq      #0x438634
00438624: ldr      r0, [r4, #0x48]
00438628: ldrb     r2, [r0, #4]
0043862c: cmp      r2, #0
00438630: beq      #0x4386ac
00438634: mov      r0, r3
00438638: mov      r1, #2
0043863c: ldr      r3, [r3]
00438640: mov      lr, pc
00438644: ldr      pc, [r3, #8]
00438648: cmp      r0, #0
0043864c: beq      #0x438458
00438650: mov      r0, r7
00438654: bl       #0x438224
00438658: mov      r3, #1
0043865c: strb     r3, [r0, #0xea]
00438660: b        #0x438458
00438664: ldr      r1, [r0]
00438668: sub      r1, r1, #1
0043866c: cmp      r1, #0
00438670: str      r1, [r0]
00438674: bne      #0x43867c
00438678: bl       #0x752b38
0043867c: mov      r3, #0
00438680: str      r3, [r4, #0x48]
00438684: str      r3, [r4, #0x4c]
00438688: b        #0x438444
0043868c: ldr      r3, [pc, #0x50c]
00438690: ldr      r2, [r7, r3]
00438694: ldr      r3, [pc, #0x4e4]
00438698: strb     r0, [r2]
0043869c: ldr      r3, [r7, r3]
004386a0: mov      r2, #1
004386a4: str      r2, [r3]
004386a8: b        #0x438328
004386ac: ldr      r1, [r0]
004386b0: sub      r1, r1, #1
004386b4: cmp      r1, #0
004386b8: str      r1, [r0]
004386bc: bne      #0x4386c4
004386c0: bl       #0x752b38
004386c4: mov      r3, #0
004386c8: str      r3, [r4, #0x48]
004386cc: str      r3, [r4, #0x4c]
004386d0: b        #0x438634
004386d4: add      r7, r4, #0x48
004386d8: mov      r0, r7
004386dc: bl       #0x4381d0
004386e0: mov      r1, #2
004386e4: ldr      r3, [r0]
004386e8: mov      lr, pc
004386ec: ldr      pc, [r3, #8]
004386f0: cmp      r0, #0
004386f4: beq      #0x4383c8
004386f8: add      r0, sb, #0x48
004386fc: bl       #0x438224
00438700: mov      r3, #0
00438704: strb     r3, [r0, #0xea]
00438708: b        #0x4383c8
0043870c: bl       #0x89becc
00438710: b        #0x438528
00438714: add      r0, r6, #0x114
00438718: add      r1, sl, sl, asr #1
0043871c: bl       #0x437c1c
00438720: b        #0x438410
00438724: ldr      r1, [pc, #0x478]
00438728: mov      r0, r5
0043872c: add      r1, pc, r1
00438730: bl       #0x30e31c
00438734: cmp      r0, #0
00438738: bne      #0x4387b4
0043873c: ldr      r3, [pc, #0x43c]
00438740: mov      r2, #3
00438744: ldr      r3, [r7, r3]
00438748: str      r2, [r3]
0043874c: b        #0x438328
00438750: add      sl, sb, #0x48
00438754: mov      r0, sl
00438758: bl       #0x438224
0043875c: ldr      r2, [pc, #0x444]
00438760: mov      r3, fp
00438764: mov      r1, r0
00438768: add      r2, pc, r2
0043876c: mov      r0, r7
00438770: bl       #0x7aba04
00438774: subs     fp, r0, #0
00438778: movne    r3, #4
0043877c: strne    r3, [sb, #0x58]
00438780: bne      #0x4383a0
00438784: mov      r0, sl
00438788: bl       #0x438224
0043878c: ldr      r2, [pc, #0x418]
00438790: mov      r3, fp
00438794: mov      r1, r0
00438798: add      r2, pc, r2
0043879c: mov      r0, r7
004387a0: bl       #0x7aba04
004387a4: cmp      r0, #0
004387a8: movne    r3, #2
004387ac: strne    r3, [sb, #0x58]
004387b0: b        #0x4383a0
004387b4: ldr      r1, [pc, #0x3f4]
004387b8: mov      r0, r5
004387bc: add      r1, pc, r1
004387c0: bl       #0x30e31c
004387c4: cmp      r0, #0
004387c8: bne      #0x4387e0
004387cc: ldr      r3, [pc, #0x3ac]
004387d0: mov      r2, #4
004387d4: ldr      r3, [r7, r3]
004387d8: str      r2, [r3]
004387dc: b        #0x438328
004387e0: ldr      r1, [pc, #0x3cc]
004387e4: mov      r0, r5
004387e8: add      r1, pc, r1
004387ec: bl       #0x30e31c
004387f0: cmp      r0, #0
004387f4: bne      #0x438820
004387f8: ldr      r3, [pc, #0x3a0]
004387fc: ldr      r3, [r7, r3]
00438800: ldrb     r3, [r3]
00438804: cmp      r3, #0
00438808: ldr      r3, [pc, #0x370]
0043880c: movne    r2, #5
00438810: moveq    r2, #6
00438814: ldr      r3, [r7, r3]
00438818: str      r2, [r3]
0043881c: b        #0x438328
00438820: ldr      r1, [pc, #0x390]
00438824: mov      r0, r5
00438828: add      r1, pc, r1
0043882c: bl       #0x30e31c
00438830: cmp      r0, #0
00438834: bne      #0x43884c
00438838: ldr      r3, [pc, #0x340]
0043883c: mov      r2, #7
00438840: ldr      r3, [r7, r3]
00438844: str      r2, [r3]
00438848: b        #0x438328
0043884c: ldr      r1, [pc, #0x368]
00438850: mov      r0, r5
00438854: add      r1, pc, r1
00438858: bl       #0x30e31c
0043885c: cmp      r0, #0
00438860: bne      #0x438878
00438864: ldr      r3, [pc, #0x314]
00438868: mov      r2, #8
0043886c: ldr      r3, [r7, r3]
00438870: str      r2, [r3]
00438874: b        #0x438328
00438878: ldr      r1, [pc, #0x340]
0043887c: mov      r0, r5
00438880: add      r1, pc, r1
00438884: bl       #0x30e31c
00438888: cmp      r0, #0
0043888c: bne      #0x4388a4
00438890: ldr      r3, [pc, #0x2e8]
00438894: mov      r2, #9
00438898: ldr      r3, [r7, r3]
0043889c: str      r2, [r3]
004388a0: b        #0x438328
004388a4: ldr      r1, [pc, #0x318]
004388a8: mov      r0, r5
004388ac: add      r1, pc, r1
004388b0: bl       #0x30e31c
004388b4: cmp      r0, #0
004388b8: bne      #0x4388e0
004388bc: ldr      r3, [pc, #0x2dc]
004388c0: mov      r1, #1
004388c4: ldr      r2, [r7, r3]
004388c8: ldr      r3, [pc, #0x2b0]
004388cc: strb     r1, [r2]
004388d0: ldr      r3, [r7, r3]
004388d4: mov      r2, #0xa
004388d8: str      r2, [r3]
004388dc: b        #0x438328
004388e0: ldr      r1, [pc, #0x2e0]
004388e4: mov      r0, r5
004388e8: add      r1, pc, r1
004388ec: bl       #0x30e31c
004388f0: cmp      r0, #0
004388f4: bne      #0x43890c
004388f8: ldr      r3, [pc, #0x280]
004388fc: mov      r2, #0xb
00438900: ldr      r3, [r7, r3]
00438904: str      r2, [r3]
00438908: b        #0x438328
0043890c: ldr      r1, [pc, #0x2b8]
00438910: mov      r0, r5
00438914: add      r1, pc, r1
00438918: bl       #0x30e31c
0043891c: cmp      r0, #0
00438920: beq      #0x438980
00438924: ldr      r1, [pc, #0x2a4]
00438928: mov      r0, r5
0043892c: add      r1, pc, r1
00438930: bl       #0x30e31c
00438934: cmp      r0, #0
00438938: beq      #0x4389f0
0043893c: ldr      r1, [pc, #0x290]
00438940: mov      r0, r5
00438944: add      r1, pc, r1
00438948: bl       #0x30e31c
0043894c: cmp      r0, #0
00438950: beq      #0x4389cc
00438954: ldr      r1, [pc, #0x27c]
00438958: mov      r0, r5
0043895c: add      r1, pc, r1
00438960: bl       #0x30e31c
00438964: cmp      r0, #0
00438968: bne      #0x4389a0
0043896c: ldr      r3, [pc, #0x20c]
00438970: mov      r2, #0x11
00438974: ldr      r3, [r7, r3]
00438978: str      r2, [r3]
0043897c: b        #0x438328
00438980: ldr      r3, [pc, #0x1f8]
00438984: ldr      r0, [pc, #0x250]
00438988: mov      r2, #0x13
0043898c: ldr      r3, [r7, r3]
00438990: add      r0, pc, r0
00438994: str      r2, [r3]
00438998: bl       #0x324114
0043899c: b        #0x438328
004389a0: ldr      r1, [pc, #0x238]
004389a4: mov      r0, r5
004389a8: add      r1, pc, r1
004389ac: bl       #0x30e31c
004389b0: cmp      r0, #0
004389b4: bne      #0x438a10
004389b8: ldr      r3, [pc, #0x1c0]
004389bc: mov      r2, #0x10
004389c0: ldr      r3, [r7, r3]
004389c4: str      r2, [r3]
004389c8: b        #0x438328
004389cc: ldr      r2, [pc, #0x1ac]
004389d0: ldr      r0, [pc, #0x20c]
004389d4: mov      r3, #0x12
004389d8: ldr      r2, [r7, r2]
004389dc: add      r0, pc, r0
004389e0: mov      r1, r3
004389e4: str      r3, [r2]
004389e8: bl       #0x324114
004389ec: b        #0x438328
004389f0: ldr      r3, [pc, #0x188]
004389f4: ldr      r0, [pc, #0x1ec]
004389f8: mov      r2, #0x13
004389fc: ldr      r3, [r7, r3]
00438a00: add      r0, pc, r0
00438a04: str      r2, [r3]
00438a08: bl       #0x324114
00438a0c: b        #0x438328
00438a10: ldr      r1, [pc, #0x1d4]
00438a14: mov      r0, r5
00438a18: add      r1, pc, r1
00438a1c: bl       #0x30e31c
00438a20: cmp      r0, #0
00438a24: bne      #0x438a3c
00438a28: ldr      r3, [pc, #0x150]
00438a2c: mov      r2, #0xf
00438a30: ldr      r3, [r7, r3]
00438a34: str      r2, [r3]
00438a38: b        #0x438328
00438a3c: ldr      r1, [pc, #0x1ac]
00438a40: mov      r0, r5
00438a44: add      r1, pc, r1
00438a48: bl       #0x30e31c
00438a4c: cmp      r0, #0
00438a50: bne      #0x438a68
00438a54: ldr      r3, [pc, #0x124]
00438a58: mov      r2, #0xe
00438a5c: ldr      r3, [r7, r3]
00438a60: str      r2, [r3]
00438a64: b        #0x438328
00438a68: ldr      r1, [pc, #0x184]
00438a6c: mov      r0, r5
00438a70: add      r1, pc, r1
00438a74: bl       #0x30e31c
00438a78: cmp      r0, #0
00438a7c: beq      #0x438b28
00438a80: ldr      r1, [pc, #0x170]
00438a84: mov      r0, r5
00438a88: add      r1, pc, r1
00438a8c: bl       #0x30e31c
00438a90: cmp      r0, #0
00438a94: beq      #0x438b28
00438a98: ldr      r1, [pc, #0x15c]
00438a9c: mov      r0, r5
00438aa0: add      r1, pc, r1
00438aa4: bl       #0x30e31c
00438aa8: cmp      r0, #0
00438aac: beq      #0x438b28
00438ab0: ldr      r1, [pc, #0x148]
00438ab4: mov      r0, r5
00438ab8: add      r1, pc, r1
00438abc: bl       #0x30e31c
00438ac0: cmp      r0, #0
00438ac4: beq      #0x438b28
00438ac8: ldr      r1, [pc, #0x134]
00438acc: mov      r0, r5
00438ad0: add      r1, pc, r1
00438ad4: bl       #0x30e31c
00438ad8: cmp      r0, #0
00438adc: beq      #0x438b28
00438ae0: ldr      r1, [pc, #0x120]
00438ae4: mov      r0, r5
00438ae8: add      r1, pc, r1
00438aec: bl       #0x30e31c
00438af0: cmp      r0, #0
00438af4: beq      #0x438b28
00438af8: ldr      r1, [pc, #0x10c]
00438afc: mov      r0, r5
00438b00: add      r1, pc, r1
00438b04: bl       #0x30e31c
00438b08: cmp      r0, #0
00438b0c: beq      #0x438b28
00438b10: ldr      r1, [pc, #0xf8]
00438b14: mov      r0, r5
00438b18: add      r1, pc, r1
00438b1c: bl       #0x30e31c
00438b20: cmp      r0, #0
00438b24: bne      #0x438b3c
00438b28: ldr      r3, [pc, #0x50]
00438b2c: mov      r2, #0xc
00438b30: ldr      r3, [r7, r3]
00438b34: str      r2, [r3]
00438b38: b        #0x438328
00438b3c: ldr      r3, [pc, #0x3c]
00438b40: mov      r2, #0xd
00438b44: ldr      r3, [r7, r3]
00438b48: str      r2, [r3]
00438b4c: b        #0x438328
00438b50: ldrsheq  ip, [r5], #-0x74
00438b54: strdeq   r3, r4, [sb], #-0x40
00438b58: subeq    r3, sb, r0, lsl #10
00438b5c: subeq    r3, sb, r8, lsr r5
00438b60: subeq    r6, r8, r0, ror #27
00438b64: subeq    r0, sb, r0, ror #29
00438b68: andeq    r1, r0, r4, lsr #29
00438b6c: andeq    r1, r0, ip, lsr #28
00438b70: subeq    r3, sb, r4, lsl r7
00438b74: subeq    r3, sb, ip, lsr r6
00438b78: strdeq   r6, r7, [r8], #-0xb8
00438b7c: subeq    r3, sb, r0, asr #7
00438b80: andeq    r3, r0, r0, asr r8
00438b84: andeq    r2, r0, ip, lsl #2
00438b88: subeq    r3, sb, r4, lsr #5
00438b8c: ldrdeq   r2, r3, [r0], -r4
00438b90: ldrdeq   r2, r3, [r0], -ip
00438b94: strdeq   r3, r4, [sb], #-0x20
00438b98: subeq    r3, sb, ip, ror r2
00438b9c: strheq   r3, [sb], #-0x4c
00438ba0: andeq    r4, r0, r0, asr r5
00438ba4: subeq    r0, sb, r4, lsl #22
00438ba8: subeq    r3, sb, r8, lsr r3
00438bac: subeq    r3, sb, r8, lsl r3
00438bb0: subeq    r3, sb, r4, lsl #2
00438bb4: subeq    r3, sb, r8, ror #1
00438bb8: subeq    r1, sb, r0, lsr #6
00438bbc: subeq    sb, r8, ip, ror r7
00438bc0: subeq    r6, r8, r0, asr #9
00438bc4: subeq    r6, r8, ip, ror #8
00438bc8: subeq    sb, r8, r8, asr #12
00438bcc: subeq    r1, sb, ip, asr #4
00438bd0: subeq    r3, sb, r4, lsl r0
00438bd4: umaaleq  r0, sb, r4, r8
00438bd8: subeq    r0, sb, r4, ror #31
00438bdc: subeq    r2, sb, r8, asr pc
00438be0: subeq    r1, sb, r8, asr #3
00438be4: subeq    r2, sb, ip, asr #31
00438be8: subeq    r2, sb, r0, asr pc
00438bec: umaaleq  sb, r8, r8, r3
00438bf0: strheq   r2, [sb], #-0xf4
00438bf4: umaaleq  r6, r8, r0, r2
00438bf8: subeq    r2, sb, r0, lsl #31
00438bfc: subeq    r2, sb, r0, lsl #31
00438c00: subeq    r2, sb, r8, lsl #31
00438c04: subeq    r2, sb, r8, lsl #31
00438c08: subeq    r2, sb, r8, lsl #31
00438c0c: subeq    r2, sb, r0, lsl #31
00438c10: subeq    r2, sb, r0, ror #23

# _ZN16MultiMenuManager7PopMenuEb 0x438c14
00438c14: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00438c18: ldr      r4, [pc, #0x624]
00438c1c: ldr      r8, [pc, #0x624]
00438c20: sub      sp, sp, #0x14
00438c24: add      r4, pc, r4
00438c28: ldr      r3, [r4, r8]
00438c2c: mov      r5, r0
00438c30: mov      r7, r1
00438c34: ldrb     r3, [r3]
00438c38: cmp      r3, #0
00438c3c: beq      #0x438c74
00438c40: ldr      r3, [pc, #0x604]
00438c44: ldr      r3, [r4, r3]
00438c48: ldr      r1, [r3]
00438c4c: cmp      r1, #7
00438c50: moveq    r1, #0xa
00438c54: streq    r1, [r3]
00438c58: beq      #0x438c68
00438c5c: cmp      r1, #0xa
00438c60: cmpne    r1, #5
00438c64: bne      #0x439170
00438c68: ldr      r0, [pc, #0x5e0]
00438c6c: add      r0, pc, r0
00438c70: bl       #0x324114
00438c74: ldr      r6, [pc, #0x5d8]
00438c78: ldr      r3, [r4, r6]
00438c7c: ldrb     r3, [r3]
00438c80: cmp      r3, #0
00438c84: bne      #0x438ce8
00438c88: ldr      r3, [pc, #0x5bc]
00438c8c: ldr      r3, [r4, r3]
00438c90: ldr      r1, [r3]
00438c94: cmp      r1, #4
00438c98: beq      #0x439198
00438c9c: cmp      r1, #6
00438ca0: beq      #0x439220
00438ca4: cmp      r1, #0xe
00438ca8: beq      #0x4391dc
00438cac: cmp      r1, #3
00438cb0: beq      #0x439180
00438cb4: cmp      r1, #5
00438cb8: addeq    r1, r1, #5
00438cbc: streq    r1, [r3]
00438cc0: beq      #0x438cdc
00438cc4: cmp      r1, #0xc
00438cc8: moveq    r1, #9
00438ccc: streq    r1, [r3]
00438cd0: beq      #0x438cdc
00438cd4: cmp      r1, #0xf
00438cd8: beq      #0x439238
00438cdc: ldr      r0, [pc, #0x574]
00438ce0: add      r0, pc, r0
00438ce4: bl       #0x324114
00438ce8: ldr      r3, [r4, r6]
00438cec: mov      r2, #0
00438cf0: ldr      r6, [pc, #0x564]
00438cf4: strb     r2, [r3]
00438cf8: ldr      r2, [pc, #0x560]
00438cfc: ldr      r3, [r5, #0x128]
00438d00: ldr      sl, [pc, #0x55c]
00438d04: str      r2, [sp, #0xc]
00438d08: ldr      r2, [pc, #0x558]
00438d0c: ldr      fp, [pc, #0x558]
00438d10: add      r6, pc, r6
00438d14: add      r2, pc, r2
00438d18: add      sl, pc, sl
00438d1c: add      fp, pc, fp
00438d20: str      r2, [sp, #8]
00438d24: add      sb, r5, #0x124
00438d28: cmp      r3, #0
00438d2c: beq      #0x438f0c
00438d30: ldr      r2, [r5, #0x124]
00438d34: sub      r3, r3, #1
00438d38: ldr      r4, [r2, r3, lsl #2]
00438d3c: ldr      r3, [r4, #0x118]
00438d40: cmp      r3, #0
00438d44: ble      #0x438f0c
00438d48: ldr      r2, [r4, #0x114]
00438d4c: sub      r3, r3, #1
00438d50: ldr      r3, [r2, r3, lsl #2]
00438d54: mov      r0, r3
00438d58: ldr      r3, [r3]
00438d5c: mov      lr, pc
00438d60: ldr      pc, [r3, #0x18]
00438d64: ldr      r2, [r4, #0x118]
00438d68: ldr      r3, [r4, #0x114]
00438d6c: sub      r2, r2, #1
00438d70: ldr      r3, [r3, r2, lsl #2]
00438d74: mov      r0, r3
00438d78: ldr      r3, [r3]
00438d7c: mov      lr, pc
00438d80: ldr      pc, [r3, #0x10]
00438d84: ldr      r2, [r4, #0x118]
00438d88: ldr      r3, [r4, #0x114]
00438d8c: mov      ip, #0
00438d90: sub      r2, r2, #1
00438d94: ldr      r1, [r3, r2, lsl #2]
00438d98: mov      r0, r4
00438d9c: mov      r3, ip
00438da0: add      r1, r1, #8
00438da4: mov      r2, r6
00438da8: str      ip, [sp]
00438dac: bl       #0x7ad7e8
00438db0: ldr      r8, [r4, #0xf8]
00438db4: ands     r8, r8, #0x40
00438db8: beq      #0x438fa8
00438dbc: ldr      r2, [r4, #0x118]
00438dc0: ldr      r3, [r4, #0x114]
00438dc4: sub      r2, r2, #1
00438dc8: ldr      r3, [r3, r2, lsl #2]
00438dcc: mov      r2, #2
00438dd0: str      r2, [r3, #0x58]
00438dd4: ldr      r3, [r4, #0xf8]
00438dd8: tst      r3, #8
00438ddc: bne      #0x438f3c
00438de0: mov      r0, r4
00438de4: bl       #0x7a7cac
00438de8: ldr      r1, [r0, #0x10]
00438dec: mov      r0, r4
00438df0: bl       #0x7a7ee8
00438df4: ldr      r8, [r4, #0x118]
00438df8: subs     r8, r8, #1
00438dfc: beq      #0x438e0c
00438e00: ldr      r3, [r4, #0x11c]
00438e04: cmp      r8, r3
00438e08: bgt      #0x438fe8
00438e0c: str      r8, [r4, #0x118]
00438e10: ldr      r4, [r5, #0x128]
00438e14: subs     r4, r4, #1
00438e18: streq    r4, [r5, #0x128]
00438e1c: beq      #0x438f04
00438e20: ldr      r3, [r5, #0x12c]
00438e24: cmp      r4, r3
00438e28: bgt      #0x438fd8
00438e2c: cmp      r4, #0
00438e30: str      r4, [r5, #0x128]
00438e34: ble      #0x438f04
00438e38: ldr      r3, [r5, #0x124]
00438e3c: sub      r4, r4, #1
00438e40: ldr      r4, [r3, r4, lsl #2]
00438e44: cmp      r4, #0
00438e48: beq      #0x438f04
00438e4c: ldr      r3, [r4, #0x118]
00438e50: cmp      r3, #0
00438e54: ble      #0x438f04
00438e58: ldr      r2, [r4, #0x114]
00438e5c: sub      r3, r3, #1
00438e60: ldr      r8, [r2, r3, lsl #2]
00438e64: ldr      r3, [r8, #0x4c]
00438e68: cmp      r3, #0
00438e6c: beq      #0x438e80
00438e70: ldr      r0, [r8, #0x48]
00438e74: ldrb     r2, [r0, #4]
00438e78: cmp      r2, #0
00438e7c: beq      #0x439148
00438e80: mov      r2, #1
00438e84: strb     r2, [r3, #0x9b]
00438e88: ldr      r3, [r4, #0xf8]
00438e8c: tst      r3, #8
00438e90: bne      #0x4390b4
00438e94: ldr      r2, [r4, #0x118]
00438e98: ldr      r3, [r4, #0x114]
00438e9c: sub      r2, r2, #1
00438ea0: ldr      r0, [r3, r2, lsl #2]
00438ea4: add      r0, r0, #0x48
00438ea8: bl       #0x438224
00438eac: mov      r1, r0
00438eb0: mov      r0, r4
00438eb4: bl       #0x7a7ee8
00438eb8: ldr      r3, [r4, #0xf8]
00438ebc: ands     r8, r3, #0x40
00438ec0: beq      #0x439050
00438ec4: tst      r3, #1
00438ec8: bne      #0x438ff8
00438ecc: ldr      r2, [r4, #0x118]
00438ed0: ldr      r3, [r4, #0x114]
00438ed4: sub      r2, r2, #1
00438ed8: ldr      r3, [r3, r2, lsl #2]
00438edc: mov      r0, r3
00438ee0: ldr      r3, [r3]
00438ee4: mov      lr, pc
00438ee8: ldr      pc, [r3, #0x14]
00438eec: ldr      r2, [r4, #0x118]
00438ef0: ldr      r3, [r4, #0x114]
00438ef4: sub      r2, r2, #1
00438ef8: ldr      r3, [r3, r2, lsl #2]
00438efc: mov      r2, #3
00438f00: str      r2, [r3, #0x58]
00438f04: cmp      r7, #0
00438f08: bne      #0x438f14
00438f0c: add      sp, sp, #0x14
00438f10: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00438f14: ldr      r3, [r5, #0x128]
00438f18: cmp      r3, #0
00438f1c: ble      #0x438f0c
00438f20: ldr      r2, [r5, #0x124]
00438f24: sub      r1, r3, #1
00438f28: ldr      r2, [r2, r1, lsl #2]
00438f2c: ldr      r2, [r2, #0x118]
00438f30: cmp      r2, #0
00438f34: bgt      #0x438d28
00438f38: b        #0x438f0c
00438f3c: ldr      r2, [r4, #0x118]
00438f40: ldr      r3, [r4, #0x114]
00438f44: sub      r2, r2, #1
00438f48: ldr      r8, [r3, r2, lsl #2]
00438f4c: ldr      r3, [r8, #0x4c]
00438f50: cmp      r3, #0
00438f54: beq      #0x438f68
00438f58: ldr      r0, [r8, #0x48]
00438f5c: ldrb     r2, [r0, #4]
00438f60: cmp      r2, #0
00438f64: beq      #0x439120
00438f68: mov      r0, r3
00438f6c: mov      r1, #2
00438f70: ldr      r3, [r3]
00438f74: mov      lr, pc
00438f78: ldr      pc, [r3, #8]
00438f7c: cmp      r0, #0
00438f80: beq      #0x438de0
00438f84: ldr      r2, [r4, #0x118]
00438f88: ldr      r3, [r4, #0x114]
00438f8c: sub      r2, r2, #1
00438f90: ldr      r0, [r3, r2, lsl #2]
00438f94: add      r0, r0, #0x48
00438f98: bl       #0x438224
00438f9c: mov      r3, #0
00438fa0: strb     r3, [r0, #0xea]
00438fa4: b        #0x438de0
00438fa8: ldr      r2, [r4, #0x118]
00438fac: ldr      r3, [r4, #0x114]
00438fb0: sub      r2, r2, #1
00438fb4: ldr      r0, [r3, r2, lsl #2]
00438fb8: add      r0, r0, #0x48
00438fbc: bl       #0x438224
00438fc0: mov      r2, sl
00438fc4: mov      r1, r0
00438fc8: mov      r3, r8
00438fcc: mov      r0, r4
00438fd0: bl       #0x7aba04
00438fd4: b        #0x438dbc
00438fd8: mov      r0, sb
00438fdc: add      r1, r4, r4, asr #1
00438fe0: bl       #0x437a6c
00438fe4: b        #0x438e2c
00438fe8: add      r0, r4, #0x114
00438fec: add      r1, r8, r8, asr #1
00438ff0: bl       #0x437c1c
00438ff4: b        #0x438e0c
00438ff8: ldr      r2, [r4, #0x118]
00438ffc: ldr      r3, [r4, #0x114]
00439000: sub      r2, r2, #1
00439004: ldr      r0, [r3, r2, lsl #2]
00439008: add      r0, r0, #0x50
0043900c: bl       #0x438224
00439010: cmp      r0, #0
00439014: beq      #0x438ecc
00439018: mov      r0, r4
0043901c: mov      r1, #0
00439020: bl       #0x7ac410
00439024: ldr      r2, [r4, #0x118]
00439028: ldr      r3, [r4, #0x114]
0043902c: sub      r2, r2, #1
00439030: ldr      r0, [r3, r2, lsl #2]
00439034: add      r0, r0, #0x50
00439038: bl       #0x438224
0043903c: mov      r2, #0
00439040: mov      r1, r0
00439044: mov      r0, r4
00439048: bl       #0x7ac228
0043904c: b        #0x438ecc
00439050: ldr      r1, [r4, #0x118]
00439054: ldr      r3, [r4, #0x114]
00439058: mov      r2, fp
0043905c: sub      r1, r1, #1
00439060: ldr      r1, [r3, r1, lsl #2]
00439064: mov      r0, r4
00439068: mov      r3, r8
0043906c: add      r1, r1, #8
00439070: str      r8, [sp]
00439074: bl       #0x7ad7e8
00439078: ldr      r2, [r4, #0x118]
0043907c: ldr      r3, [r4, #0x114]
00439080: sub      r2, r2, #1
00439084: ldr      r0, [r3, r2, lsl #2]
00439088: add      r0, r0, #0x48
0043908c: bl       #0x438224
00439090: mov      r3, r8
00439094: mov      r1, r0
00439098: ldr      r2, [sp, #8]
0043909c: mov      r0, r4
004390a0: bl       #0x7aba04
004390a4: subs     r8, r0, #0
004390a8: beq      #0x4391a4
004390ac: ldr      r3, [r4, #0xf8]
004390b0: b        #0x438ec4
004390b4: ldr      r2, [r5, #0x118]
004390b8: ldr      r3, [r5, #0x114]
004390bc: sub      r2, r2, #1
004390c0: ldr      r8, [r3, r2, lsl #2]
004390c4: ldr      r3, [r8, #0x4c]
004390c8: cmp      r3, #0
004390cc: beq      #0x4390e0
004390d0: ldr      r0, [r8, #0x48]
004390d4: ldrb     r2, [r0, #4]
004390d8: cmp      r2, #0
004390dc: beq      #0x4391f8
004390e0: mov      r0, r3
004390e4: mov      r1, #2
004390e8: ldr      r3, [r3]
004390ec: mov      lr, pc
004390f0: ldr      pc, [r3, #8]
004390f4: cmp      r0, #0
004390f8: beq      #0x438e94
004390fc: ldr      r2, [r4, #0x118]
00439100: ldr      r3, [r4, #0x114]
00439104: sub      r2, r2, #1
00439108: ldr      r0, [r3, r2, lsl #2]
0043910c: add      r0, r0, #0x48
00439110: bl       #0x438224
00439114: mov      r3, #1
00439118: strb     r3, [r0, #0xea]
0043911c: b        #0x438e94
00439120: ldr      r1, [r0]
00439124: sub      r1, r1, #1
00439128: cmp      r1, #0
0043912c: str      r1, [r0]
00439130: bne      #0x439138
00439134: bl       #0x752b38
00439138: mov      r3, #0
0043913c: str      r3, [r8, #0x4c]
00439140: str      r3, [r8, #0x48]
00439144: b        #0x438f68
00439148: ldr      r1, [r0]
0043914c: sub      r1, r1, #1
00439150: cmp      r1, #0
00439154: str      r1, [r0]
00439158: bne      #0x439160
0043915c: bl       #0x752b38
00439160: mov      r3, #0
00439164: str      r3, [r8, #0x4c]
00439168: str      r3, [r8, #0x48]
0043916c: b        #0x438e80
00439170: cmp      r1, #0xe
00439174: movne    r1, #9
00439178: strne    r1, [r3]
0043917c: b        #0x438c68
00439180: ldr      r2, [r4, r8]
00439184: ldrb     r2, [r2]
00439188: cmp      r2, #0
0043918c: addne    r1, r1, #7
00439190: strne    r1, [r3]
00439194: bne      #0x438cdc
00439198: mov      r1, #1
0043919c: str      r1, [r3]
004391a0: b        #0x438cdc
004391a4: ldr      r2, [r4, #0x118]
004391a8: ldr      r3, [r4, #0x114]
004391ac: sub      r2, r2, #1
004391b0: ldr      r0, [r3, r2, lsl #2]
004391b4: add      r0, r0, #0x48
004391b8: bl       #0x438224
004391bc: ldr      ip, [sp, #0xc]
004391c0: mov      r1, r0
004391c4: mov      r3, r8
004391c8: mov      r0, r4
004391cc: add      r2, pc, ip
004391d0: bl       #0x7aba04
004391d4: ldr      r3, [r4, #0xf8]
004391d8: b        #0x438ec4
004391dc: ldr      r2, [r4, r8]
004391e0: ldrb     r2, [r2]
004391e4: cmp      r2, #0
004391e8: movne    r1, #5
004391ec: moveq    r1, #6
004391f0: str      r1, [r3]
004391f4: b        #0x438cdc
004391f8: ldr      r1, [r0]
004391fc: sub      r1, r1, #1
00439200: cmp      r1, #0
00439204: str      r1, [r0]
00439208: bne      #0x439210
0043920c: bl       #0x752b38
00439210: mov      r3, #0
00439214: str      r3, [r8, #0x4c]
00439218: str      r3, [r8, #0x48]
0043921c: b        #0x4390e0
00439220: ldr      r2, [r4, r8]
00439224: ldrb     r2, [r2]
00439228: cmp      r2, #0
0043922c: addne    r1, r1, #4
00439230: strne    r1, [r3]
00439234: bne      #0x438cdc
00439238: mov      r1, #4
0043923c: str      r1, [r3]
00439240: b        #0x438cdc
00439244: subseq   fp, r5, ip, ror #28
00439248: andeq    r4, r0, r0, asr r5
0043924c: andeq    r3, r0, r0, asr r8
00439250: subeq    r2, sb, ip, asr lr
00439254: andeq    r1, r0, ip, lsr #28
00439258: subeq    r2, sb, r0, lsl lr
0043925c: subeq    r2, sb, r8, lsl #27
00439260: strdeq   r2, r3, [sb], #-0x84
00439264: umaaleq  r2, sb, r8, sp
00439268: subeq    r2, sb, r4, lsl #28
0043926c: umaaleq  r2, sb, ip, sp

# _ZN16MultiMenuManager7PopMenuEPKcb 0x439270
00439270: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00439274: cmp      r2, #0
00439278: sub      sp, sp, #0x2c
0043927c: mov      r4, r0
00439280: mov      r6, r1
00439284: beq      #0x439724
00439288: ldr      r3, [pc, #0x97c]
0043928c: ldr      r7, [pc, #0x97c]
00439290: ldr      r8, [pc, #0x97c]
00439294: add      r3, pc, r3
00439298: ldr      fp, [pc, #0x978]
0043929c: str      r3, [sp, #0x10]
004392a0: ldr      r3, [pc, #0x974]
004392a4: add      r7, pc, r7
004392a8: add      r8, pc, r8
004392ac: add      fp, pc, fp
004392b0: add      sb, r0, #0x124
004392b4: str      r3, [sp, #0x14]
004392b8: ldr      r3, [r4]
004392bc: ldr      r5, [r3, #0x40]
004392c0: bl       #0x42ca8c
004392c4: mov      r1, r6
004392c8: bl       #0x42d1f0
004392cc: mov      r1, r0
004392d0: mov      r0, r4
004392d4: blx      r5
004392d8: cmp      r0, #0
004392dc: bne      #0x4392e8
004392e0: add      sp, sp, #0x2c
004392e4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004392e8: ldr      r3, [r4, #0x128]
004392ec: cmp      r3, #0
004392f0: ble      #0x4392e0
004392f4: ldr      r2, [r4, #0x124]
004392f8: sub      r3, r3, #1
004392fc: ldr      r5, [r2, r3, lsl #2]
00439300: ldr      r3, [r5, #0x118]
00439304: cmp      r3, #0
00439308: ble      #0x4392e0
0043930c: mov      r0, r5
00439310: bl       #0x7a7f30
00439314: mov      sl, r0
00439318: bl       #0x42ca8c
0043931c: mov      r1, r6
00439320: bl       #0x42d1f0
00439324: cmp      sl, r0
00439328: beq      #0x4392e0
0043932c: ldr      r2, [r5, #0x118]
00439330: ldr      r3, [r5, #0x114]
00439334: sub      r2, r2, #1
00439338: ldr      r3, [r3, r2, lsl #2]
0043933c: mov      r0, r3
00439340: ldr      r3, [r3]
00439344: mov      lr, pc
00439348: ldr      pc, [r3, #0x18]
0043934c: ldr      r2, [r5, #0x118]
00439350: ldr      r3, [r5, #0x114]
00439354: sub      r2, r2, #1
00439358: ldr      r3, [r3, r2, lsl #2]
0043935c: mov      r0, r3
00439360: ldr      r3, [r3]
00439364: mov      lr, pc
00439368: ldr      pc, [r3, #0x10]
0043936c: ldr      r2, [r5, #0x118]
00439370: ldr      r3, [r5, #0x114]
00439374: mov      ip, #0
00439378: sub      r2, r2, #1
0043937c: ldr      r1, [r3, r2, lsl #2]
00439380: mov      r0, r5
00439384: mov      r3, ip
00439388: add      r1, r1, #8
0043938c: mov      r2, r7
00439390: str      ip, [sp]
00439394: bl       #0x7ad7e8
00439398: ldr      sl, [r5, #0xf8]
0043939c: ands     sl, sl, #0x40
004393a0: beq      #0x43955c
004393a4: ldr      r2, [r5, #0x118]
004393a8: ldr      r3, [r5, #0x114]
004393ac: sub      r2, r2, #1
004393b0: ldr      r3, [r3, r2, lsl #2]
004393b4: mov      r2, #2
004393b8: str      r2, [r3, #0x58]
004393bc: ldr      r3, [r5, #0xf8]
004393c0: tst      r3, #8
004393c4: bne      #0x4394f0
004393c8: mov      r0, r5
004393cc: bl       #0x7a7cac
004393d0: ldr      r1, [r0, #0x10]
004393d4: mov      r0, r5
004393d8: bl       #0x7a7ee8
004393dc: ldr      sl, [r5, #0x118]
004393e0: subs     sl, sl, #1
004393e4: beq      #0x4393f4
004393e8: ldr      r3, [r5, #0x11c]
004393ec: cmp      sl, r3
004393f0: bgt      #0x43958c
004393f4: str      sl, [r5, #0x118]
004393f8: ldr      r5, [r4, #0x128]
004393fc: subs     r5, r5, #1
00439400: streq    r5, [r4, #0x128]
00439404: beq      #0x4392b8
00439408: ldr      r3, [r4, #0x12c]
0043940c: cmp      r5, r3
00439410: bgt      #0x43959c
00439414: cmp      r5, #0
00439418: str      r5, [r4, #0x128]
0043941c: ble      #0x4392b8
00439420: ldr      r3, [r4, #0x124]
00439424: sub      r5, r5, #1
00439428: ldr      r5, [r3, r5, lsl #2]
0043942c: cmp      r5, #0
00439430: beq      #0x4392b8
00439434: ldr      r3, [r5, #0x118]
00439438: cmp      r3, #0
0043943c: ble      #0x4392b8
00439440: ldr      r2, [r5, #0x114]
00439444: sub      r3, r3, #1
00439448: ldr      sl, [r2, r3, lsl #2]
0043944c: ldr      r3, [sl, #0x4c]
00439450: cmp      r3, #0
00439454: beq      #0x439468
00439458: ldr      r0, [sl, #0x48]
0043945c: ldrb     r2, [r0, #4]
00439460: cmp      r2, #0
00439464: beq      #0x4396fc
00439468: mov      r2, #1
0043946c: strb     r2, [r3, #0x9b]
00439470: ldr      r3, [r5, #0xf8]
00439474: tst      r3, #8
00439478: bne      #0x439690
0043947c: ldr      r2, [r5, #0x118]
00439480: ldr      r3, [r5, #0x114]
00439484: sub      r2, r2, #1
00439488: ldr      r0, [r3, r2, lsl #2]
0043948c: add      r0, r0, #0x48
00439490: bl       #0x438224
00439494: mov      r1, r0
00439498: mov      r0, r5
0043949c: bl       #0x7a7ee8
004394a0: ldr      r3, [r5, #0xf8]
004394a4: ands     sl, r3, #0x40
004394a8: beq      #0x43962c
004394ac: tst      r3, #1
004394b0: bne      #0x4395d4
004394b4: ldr      r2, [r5, #0x118]
004394b8: ldr      r3, [r5, #0x114]
004394bc: sub      r2, r2, #1
004394c0: ldr      r3, [r3, r2, lsl #2]
004394c4: mov      r0, r3
004394c8: ldr      r3, [r3]
004394cc: mov      lr, pc
004394d0: ldr      pc, [r3, #0x14]
004394d4: ldr      r2, [r5, #0x118]
004394d8: ldr      r3, [r5, #0x114]
004394dc: sub      r2, r2, #1
004394e0: ldr      r3, [r3, r2, lsl #2]
004394e4: mov      r2, #3
004394e8: str      r2, [r3, #0x58]
004394ec: b        #0x4392b8
004394f0: ldr      r2, [r5, #0x118]
004394f4: ldr      r3, [r5, #0x114]
004394f8: sub      r2, r2, #1
004394fc: ldr      sl, [r3, r2, lsl #2]
00439500: ldr      r3, [sl, #0x4c]
00439504: cmp      r3, #0
00439508: beq      #0x43951c
0043950c: ldr      r0, [sl, #0x48]
00439510: ldrb     r2, [r0, #4]
00439514: cmp      r2, #0
00439518: beq      #0x4395ac
0043951c: mov      r0, r3
00439520: mov      r1, #2
00439524: ldr      r3, [r3]
00439528: mov      lr, pc
0043952c: ldr      pc, [r3, #8]
00439530: cmp      r0, #0
00439534: beq      #0x4393c8
00439538: ldr      r2, [r5, #0x118]
0043953c: ldr      r3, [r5, #0x114]
00439540: sub      r2, r2, #1
00439544: ldr      r0, [r3, r2, lsl #2]
00439548: add      r0, r0, #0x48
0043954c: bl       #0x438224
00439550: mov      r3, #0
00439554: strb     r3, [r0, #0xea]
00439558: b        #0x4393c8
0043955c: ldr      r2, [r5, #0x118]
00439560: ldr      r3, [r5, #0x114]
00439564: sub      r2, r2, #1
00439568: ldr      r0, [r3, r2, lsl #2]
0043956c: add      r0, r0, #0x48
00439570: bl       #0x438224
00439574: mov      r2, r8
00439578: mov      r1, r0
0043957c: mov      r3, sl
00439580: mov      r0, r5
00439584: bl       #0x7aba04
00439588: b        #0x4393a4
0043958c: add      r0, r5, #0x114
00439590: add      r1, sl, sl, asr #1
00439594: bl       #0x437c1c
00439598: b        #0x4393f4
0043959c: mov      r0, sb
004395a0: add      r1, r5, r5, asr #1
004395a4: bl       #0x437a6c
004395a8: b        #0x439414
004395ac: ldr      r1, [r0]
004395b0: sub      r1, r1, #1
004395b4: cmp      r1, #0
004395b8: str      r1, [r0]
004395bc: bne      #0x4395c4
004395c0: bl       #0x752b38
004395c4: mov      r3, #0
004395c8: str      r3, [sl, #0x4c]
004395cc: str      r3, [sl, #0x48]
004395d0: b        #0x43951c
004395d4: ldr      r2, [r5, #0x118]
004395d8: ldr      r3, [r5, #0x114]
004395dc: sub      r2, r2, #1
004395e0: ldr      r0, [r3, r2, lsl #2]
004395e4: add      r0, r0, #0x50
004395e8: bl       #0x438224
004395ec: cmp      r0, #0
004395f0: beq      #0x4394b4
004395f4: mov      r0, r5
004395f8: mov      r1, #0
004395fc: bl       #0x7ac410
00439600: ldr      r2, [r5, #0x118]
00439604: ldr      r3, [r5, #0x114]
00439608: sub      r2, r2, #1
0043960c: ldr      r0, [r3, r2, lsl #2]
00439610: add      r0, r0, #0x50
00439614: bl       #0x438224
00439618: mov      r2, #0
0043961c: mov      r1, r0
00439620: mov      r0, r5
00439624: bl       #0x7ac228
00439628: b        #0x4394b4
0043962c: ldr      r1, [r5, #0x118]
00439630: ldr      r3, [r5, #0x114]
00439634: mov      r2, fp
00439638: sub      r1, r1, #1
0043963c: ldr      r1, [r3, r1, lsl #2]
00439640: mov      r0, r5
00439644: mov      r3, sl
00439648: add      r1, r1, #8
0043964c: str      sl, [sp]
00439650: bl       #0x7ad7e8
00439654: ldr      r2, [r5, #0x118]
00439658: ldr      r3, [r5, #0x114]
0043965c: sub      r2, r2, #1
00439660: ldr      r0, [r3, r2, lsl #2]
00439664: add      r0, r0, #0x48
00439668: bl       #0x438224
0043966c: mov      r3, sl
00439670: mov      r1, r0
00439674: ldr      r2, [sp, #0x10]
00439678: mov      r0, r5
0043967c: bl       #0x7aba04
00439680: subs     sl, r0, #0
00439684: beq      #0x439af4
00439688: ldr      r3, [r5, #0xf8]
0043968c: b        #0x4394ac
00439690: ldr      r2, [r4, #0x118]
00439694: ldr      r3, [r4, #0x114]
00439698: sub      r2, r2, #1
0043969c: ldr      sl, [r3, r2, lsl #2]
004396a0: ldr      r3, [sl, #0x4c]
004396a4: cmp      r3, #0
004396a8: beq      #0x4396bc
004396ac: ldr      r0, [sl, #0x48]
004396b0: ldrb     r2, [r0, #4]
004396b4: cmp      r2, #0
004396b8: beq      #0x439be4
004396bc: mov      r0, r3
004396c0: mov      r1, #2
004396c4: ldr      r3, [r3]
004396c8: mov      lr, pc
004396cc: ldr      pc, [r3, #8]
004396d0: cmp      r0, #0
004396d4: beq      #0x43947c
004396d8: ldr      r2, [r5, #0x118]
004396dc: ldr      r3, [r5, #0x114]
004396e0: sub      r2, r2, #1
004396e4: ldr      r0, [r3, r2, lsl #2]
004396e8: add      r0, r0, #0x48
004396ec: bl       #0x438224
004396f0: mov      r3, #1
004396f4: strb     r3, [r0, #0xea]
004396f8: b        #0x43947c
004396fc: ldr      r1, [r0]
00439700: sub      r1, r1, #1
00439704: cmp      r1, #0
00439708: str      r1, [r0]
0043970c: bne      #0x439714
00439710: bl       #0x752b38
00439714: mov      r3, #0
00439718: str      r3, [sl, #0x4c]
0043971c: str      r3, [sl, #0x48]
00439720: b        #0x439468
00439724: bl       #0x42ca8c
00439728: mov      r1, r6
0043972c: bl       #0x42d1f0
00439730: ldr      fp, [r4, #0x128]
00439734: mov      sb, r0
00439738: subs     fp, fp, #1
0043973c: bmi      #0x4392e0
00439740: ldr      r3, [pc, #0x4d8]
00439744: lsl      sl, fp, #2
00439748: add      r3, pc, r3
0043974c: str      r3, [sp, #0x18]
00439750: ldr      r3, [pc, #0x4cc]
00439754: add      r3, pc, r3
00439758: str      r3, [sp, #0x20]
0043975c: ldr      r3, [pc, #0x4c4]
00439760: add      r3, pc, r3
00439764: str      r3, [sp, #0x24]
00439768: ldr      r3, [pc, #0x4bc]
0043976c: add      r3, pc, r3
00439770: str      r3, [sp, #0x1c]
00439774: add      r3, r4, #0x124
00439778: str      r3, [sp, #0x14]
0043977c: ldr      r3, [r4, #0x124]
00439780: ldr      r7, [r3, sl]
00439784: ldr      r8, [r7, #0x118]
00439788: subs     r6, r8, #1
0043978c: bmi      #0x4397d4
00439790: ldr      ip, [pc, #0x498]
00439794: sub      r8, r8, #2
00439798: lsl      r8, r8, #2
0043979c: str      ip, [sp, #0x10]
004397a0: lsl      r5, r6, #2
004397a4: b        #0x4397b0
004397a8: ldr      r3, [r4, #0x124]
004397ac: ldr      r7, [r3, sl]
004397b0: ldr      r3, [r7, #0x114]
004397b4: ldr      r3, [r3, r5]
004397b8: cmp      sb, r3
004397bc: beq      #0x4397e8
004397c0: sub      r6, r6, #1
004397c4: cmn      r6, #1
004397c8: sub      r5, r5, #4
004397cc: sub      r8, r8, #4
004397d0: bne      #0x4397a8
004397d4: sub      fp, fp, #1
004397d8: cmn      fp, #1
004397dc: sub      sl, sl, #4
004397e0: bne      #0x43977c
004397e4: b        #0x4392e0
004397e8: mov      r0, sb
004397ec: ldr      r3, [sb]
004397f0: mov      lr, pc
004397f4: ldr      pc, [r3, #0x18]
004397f8: ldr      r3, [r7, #0x114]
004397fc: ldr      r3, [r3, r5]
00439800: mov      r0, r3
00439804: ldr      r3, [r3]
00439808: mov      lr, pc
0043980c: ldr      pc, [r3, #0x10]
00439810: ldr      r2, [r7, #0x114]
00439814: mov      ip, #0
00439818: mov      r3, ip
0043981c: ldr      r1, [r2, r5]
00439820: mov      r0, r7
00439824: ldr      r2, [sp, #0x18]
00439828: add      r1, r1, #8
0043982c: str      ip, [sp]
00439830: bl       #0x7ad7e8
00439834: ldr      r3, [r7, #0xf8]
00439838: ands     r3, r3, #0x40
0043983c: beq      #0x4399c8
00439840: ldr      r3, [r7, #0x114]
00439844: mov      r2, #2
00439848: ldr      r3, [r3, r5]
0043984c: str      r2, [r3, #0x58]
00439850: ldr      r3, [r7, #0xf8]
00439854: tst      r3, #8
00439858: bne      #0x43996c
0043985c: mov      r0, r7
00439860: bl       #0x7a7cac
00439864: ldr      r1, [r0, #0x10]
00439868: mov      r0, r7
0043986c: bl       #0x7a7ee8
00439870: add      r0, r7, #0x114
00439874: mov      r1, r6
00439878: bl       #0x437c98
0043987c: ldr      r0, [sp, #0x14]
00439880: mov      r1, fp
00439884: bl       #0x437ae8
00439888: ldr      r3, [r4, #0x128]
0043988c: cmp      r3, #0
00439890: ble      #0x4397c0
00439894: ldr      r2, [r4, #0x124]
00439898: sub      r3, r3, #1
0043989c: ldr      r7, [r2, r3, lsl #2]
004398a0: cmp      r7, #0
004398a4: beq      #0x4397c0
004398a8: ldr      r3, [r7, #0x118]
004398ac: cmp      r3, #0
004398b0: ble      #0x4397c0
004398b4: ldr      r2, [r7, #0x114]
004398b8: sub      r3, r3, #1
004398bc: ldr      r3, [r2, r3, lsl #2]
004398c0: ldr      r1, [r3, #0x4c]
004398c4: cmp      r1, #0
004398c8: beq      #0x4398dc
004398cc: ldr      r0, [r3, #0x48]
004398d0: ldrb     r2, [r0, #4]
004398d4: cmp      r2, #0
004398d8: beq      #0x439b84
004398dc: mov      r2, #1
004398e0: strb     r2, [r1, #0x9b]
004398e4: ldr      r3, [r7, #0xf8]
004398e8: tst      r3, #8
004398ec: bne      #0x439b2c
004398f0: ldr      r2, [r7, #0x118]
004398f4: ldr      r3, [r7, #0x114]
004398f8: sub      r2, r2, #1
004398fc: ldr      r0, [r3, r2, lsl #2]
00439900: add      r0, r0, #0x48
00439904: bl       #0x438224
00439908: mov      r1, r0
0043990c: mov      r0, r7
00439910: bl       #0x7a7ee8
00439914: ldr      r3, [r7, #0xf8]
00439918: ands     ip, r3, #0x40
0043991c: bne      #0x439a90
00439920: ldr      r2, [r7, #0x118]
00439924: cmp      r6, r2
00439928: beq      #0x4399f4
0043992c: tst      r3, #1
00439930: bne      #0x439a9c
00439934: ldr      r3, [r7, #0x114]
00439938: sub      r2, r2, #1
0043993c: ldr      r3, [r3, r2, lsl #2]
00439940: mov      r0, r3
00439944: ldr      r3, [r3]
00439948: mov      lr, pc
0043994c: ldr      pc, [r3, #0x14]
00439950: ldr      r3, [r7, #0x118]
00439954: ldr      r2, [r7, #0x114]
00439958: sub      r3, r3, #1
0043995c: ldr      r3, [r2, r3, lsl #2]
00439960: mov      r2, #3
00439964: str      r2, [r3, #0x58]
00439968: b        #0x4397c0
0043996c: ldr      r3, [r7, #0x114]
00439970: ldr      r3, [r3, r5]
00439974: ldr      r2, [r3, #0x4c]
00439978: cmp      r2, #0
0043997c: beq      #0x439990
00439980: ldr      r0, [r3, #0x48]
00439984: ldrb     r1, [r0, #4]
00439988: cmp      r1, #0
0043998c: beq      #0x439bb4
00439990: mov      r0, r2
00439994: ldr      r3, [r2]
00439998: mov      r1, #2
0043999c: mov      lr, pc
004399a0: ldr      pc, [r3, #8]
004399a4: cmp      r0, #0
004399a8: beq      #0x43985c
004399ac: ldr      r3, [r7, #0x114]
004399b0: ldr      r0, [r3, r5]
004399b4: add      r0, r0, #0x48
004399b8: bl       #0x438224
004399bc: mov      r3, #0
004399c0: strb     r3, [r0, #0xea]
004399c4: b        #0x43985c
004399c8: ldr      r2, [r7, #0x114]
004399cc: ldr      r0, [r2, r5]
004399d0: str      r3, [sp, #0xc]
004399d4: add      r0, r0, #0x48
004399d8: bl       #0x438224
004399dc: ldr      r2, [sp, #0x1c]
004399e0: mov      r1, r0
004399e4: ldr      r3, [sp, #0xc]
004399e8: mov      r0, r7
004399ec: bl       #0x7aba04
004399f0: b        #0x439840
004399f4: ldr      r1, [r7, #0x114]
004399f8: mov      r3, ip
004399fc: ldr      r2, [sp, #0x20]
00439a00: ldr      r1, [r1, r8]
00439a04: mov      r0, r7
00439a08: str      ip, [sp]
00439a0c: add      r1, r1, #8
00439a10: str      ip, [sp, #0xc]
00439a14: bl       #0x7ad7e8
00439a18: ldr      r2, [r7, #0x118]
00439a1c: ldr      r3, [r7, #0x114]
00439a20: sub      r2, r2, #1
00439a24: ldr      r0, [r3, r2, lsl #2]
00439a28: add      r0, r0, #0x48
00439a2c: bl       #0x438224
00439a30: ldr      ip, [sp, #0xc]
00439a34: mov      r1, r0
00439a38: ldr      r2, [sp, #0x24]
00439a3c: mov      r3, ip
00439a40: mov      r0, r7
00439a44: bl       #0x7aba04
00439a48: subs     r3, r0, #0
00439a4c: ldrne    r3, [r7, #0xf8]
00439a50: ldrne    r2, [r7, #0x118]
00439a54: bne      #0x43992c
00439a58: ldr      r1, [r7, #0x118]
00439a5c: ldr      r2, [r7, #0x114]
00439a60: sub      r1, r1, #1
00439a64: ldr      r0, [r2, r1, lsl #2]
00439a68: str      r3, [sp, #0xc]
00439a6c: add      r0, r0, #0x48
00439a70: bl       #0x438224
00439a74: ldr      ip, [sp, #0x10]
00439a78: mov      r1, r0
00439a7c: ldr      r3, [sp, #0xc]
00439a80: mov      r0, r7
00439a84: add      r2, pc, ip
00439a88: bl       #0x7aba04
00439a8c: ldr      r3, [r7, #0xf8]
00439a90: tst      r3, #1
00439a94: ldr      r2, [r7, #0x118]
00439a98: beq      #0x439934
00439a9c: ldr      r3, [r7, #0x114]
00439aa0: sub      r2, r2, #1
00439aa4: ldr      r0, [r3, r2, lsl #2]
00439aa8: add      r0, r0, #0x50
00439aac: bl       #0x438224
00439ab0: cmp      r0, #0
00439ab4: beq      #0x439aec
00439ab8: mov      r1, #0
00439abc: mov      r0, r7
00439ac0: bl       #0x7ac410
00439ac4: ldr      r2, [r7, #0x118]
00439ac8: ldr      r3, [r7, #0x114]
00439acc: sub      r2, r2, #1
00439ad0: ldr      r0, [r3, r2, lsl #2]
00439ad4: add      r0, r0, #0x50
00439ad8: bl       #0x438224
00439adc: mov      r2, #0
00439ae0: mov      r1, r0
00439ae4: mov      r0, r7
00439ae8: bl       #0x7ac228
00439aec: ldr      r2, [r7, #0x118]
00439af0: b        #0x439934
00439af4: ldr      r2, [r5, #0x118]
00439af8: ldr      r3, [r5, #0x114]
00439afc: sub      r2, r2, #1
00439b00: ldr      r0, [r3, r2, lsl #2]
00439b04: add      r0, r0, #0x48
00439b08: bl       #0x438224
00439b0c: ldr      ip, [sp, #0x14]
00439b10: mov      r1, r0
00439b14: mov      r3, sl
00439b18: mov      r0, r5
00439b1c: add      r2, pc, ip
00439b20: bl       #0x7aba04
00439b24: ldr      r3, [r5, #0xf8]
00439b28: b        #0x4394ac
00439b2c: ldr      r1, [r4, #0x118]
00439b30: ldr      r3, [r4, #0x114]
00439b34: sub      r1, r1, #1
00439b38: ldr      r0, [r3, r1, lsl #2]
00439b3c: str      r2, [sp, #0xc]
00439b40: add      r0, r0, #0x48
00439b44: bl       #0x4381d0
00439b48: mov      r1, #2
00439b4c: ldr      r3, [r0]
00439b50: mov      lr, pc
00439b54: ldr      pc, [r3, #8]
00439b58: cmp      r0, #0
00439b5c: beq      #0x4398f0
00439b60: ldr      r1, [r7, #0x118]
00439b64: ldr      r3, [r7, #0x114]
00439b68: sub      r1, r1, #1
00439b6c: ldr      r0, [r3, r1, lsl #2]
00439b70: add      r0, r0, #0x48
00439b74: bl       #0x438224
00439b78: ldr      r2, [sp, #0xc]
00439b7c: strb     r2, [r0, #0xea]
00439b80: b        #0x4398f0
00439b84: ldr      r1, [r0]
00439b88: sub      r1, r1, #1
00439b8c: cmp      r1, #0
00439b90: str      r1, [r0]
00439b94: bne      #0x439ba4
00439b98: str      r3, [sp, #0xc]
00439b9c: bl       #0x752b38
00439ba0: ldr      r3, [sp, #0xc]
00439ba4: mov      r1, #0
00439ba8: str      r1, [r3, #0x4c]
00439bac: str      r1, [r3, #0x48]
00439bb0: b        #0x4398dc
00439bb4: ldr      r1, [r0]
00439bb8: sub      r1, r1, #1
00439bbc: cmp      r1, #0
00439bc0: str      r1, [r0]
00439bc4: bne      #0x439bd4
00439bc8: str      r3, [sp, #0xc]
00439bcc: bl       #0x752b38
00439bd0: ldr      r3, [sp, #0xc]
00439bd4: mov      r2, #0
00439bd8: str      r2, [r3, #0x4c]
00439bdc: str      r2, [r3, #0x48]
00439be0: b        #0x439990
00439be4: ldr      r1, [r0]
00439be8: sub      r1, r1, #1
00439bec: cmp      r1, #0
00439bf0: str      r1, [r0]
00439bf4: bne      #0x439bfc
00439bf8: bl       #0x752b38
00439bfc: mov      r3, #0
00439c00: str      r3, [sl, #0x4c]
00439c04: str      r3, [sl, #0x48]
00439c08: b        #0x4396bc
00439c0c: subeq    r2, sb, r4, lsl #17
00439c10: strdeq   r2, r3, [sb], #-0x74
00439c14: subeq    r2, sb, r8, lsl #16
00439c18: subeq    r2, sb, ip, lsl #16
00439c1c: subeq    r1, sb, r4, lsr #31
00439c20: subeq    r2, sb, r0, asr r3
00439c24: subeq    r2, sb, r4, ror #6
00439c28: strheq   r2, [sb], #-0x38
00439c2c: subeq    r2, sb, r4, asr #6
00439c30: subeq    r2, sb, ip, lsr r0

# _ZN20MenuCharMenu_InvMain4ShowEv 0x452b1c
00452b1c: push     {r4, lr}
00452b20: ldr      ip, [pc, #0x30]
00452b24: ldr      r3, [pc, #0x30]
00452b28: ldr      r1, [pc, #0x30]
00452b2c: add      ip, pc, ip
00452b30: ldr      r2, [ip, r3]
00452b34: mov      r4, r0
00452b38: mov      r3, r0
00452b3c: add      r1, pc, r1
00452b40: ldr      r0, [r0, #4]
00452b44: bl       #0x7a91d8
00452b48: bl       #0x4528d0
00452b4c: mov      r0, r4
00452b50: pop      {r4, lr}
00452b54: b        #0x425450
00452b58: subseq   r1, r4, r4, ror #30
00452b5c: andeq    r4, r0, r8, ror #19
00452b60: strdeq   r8, sb, [r7], #-0x9c

# _ZN20MenuCharMenu_InvMain4HideEv 0x4528a0
004528a0: push     {r4, lr}
004528a4: mov      r4, r0
004528a8: bl       #0x452810
004528ac: mov      r0, r4
004528b0: pop      {r4, lr}
004528b4: b        #0x424af4

# _ZN23MenuCharMenu_InvDetails4ShowEv 0x453014
00453014: push     {r4, r5, r6, r7, r8, lr}
00453018: ldr      r4, [pc, #0xd4]
0045301c: ldr      r6, [pc, #0xd4]
00453020: sub      sp, sp, #0x20
00453024: add      r4, pc, r4
00453028: ldr      r2, [r4, r6]
0045302c: ldr      r3, [r0]
00453030: mov      r5, r0
00453034: ldr      r2, [r2]
00453038: str      r2, [sp, #0x1c]
0045303c: mov      lr, pc
00453040: ldr      pc, [r3, #0x3c]
00453044: cmp      r0, #0
00453048: beq      #0x4530d4
0045304c: ldr      r3, [pc, #0xa8]
00453050: add      r7, sp, #4
00453054: ldr      r8, [r4, r3]
00453058: mov      r0, r8
0045305c: bl       #0x337888
00453060: mov      r0, r7
00453064: mov      r1, #0x21
00453068: str      r7, [sp, #0x14]
0045306c: str      r7, [sp, #0x18]
00453070: bl       #0x31167c
00453074: ldr      r1, [pc, #0x84]
00453078: mov      r2, #0x20
0045307c: ldr      r0, [sp, #0x18]
00453080: add      r1, pc, r1
00453084: bl       #0x30e868
00453088: add      r3, r0, #0x20
0045308c: str      r3, [sp, #0x14]
00453090: mov      r3, #0
00453094: strb     r3, [r0, #0x20]
00453098: mov      r1, r7
0045309c: mov      r0, r8
004530a0: bl       #0x337a88
004530a4: mov      r0, r7
004530a8: bl       #0x3139ac
004530ac: bl       #0x4528d0
004530b0: ldr      r3, [pc, #0x4c]
004530b4: ldr      r1, [pc, #0x4c]
004530b8: ldr      r0, [r5, #4]
004530bc: ldr      r2, [r4, r3]
004530c0: add      r1, pc, r1
004530c4: mov      r3, r5
004530c8: bl       #0x7a91d8
004530cc: mov      r0, r5
004530d0: bl       #0x425450
004530d4: ldr      r3, [r4, r6]
004530d8: ldr      r2, [sp, #0x1c]
004530dc: ldr      r3, [r3]
004530e0: cmp      r2, r3
004530e4: bne      #0x4530f0
004530e8: add      sp, sp, #0x20
004530ec: pop      {r4, r5, r6, r7, r8, pc}
004530f0: bl       #0x30e310
004530f4: subseq   r1, r4, ip, ror #20
004530f8: andeq    r4, r0, ip, lsr #1
004530fc: andeq    r0, r0, r4, lsl #17
00453100: umaaleq  sb, r7, r0, ip
00453104: andeq    r4, r0, r8, ror #19
00453108: subeq    r8, r7, r8, ror r4

# _ZN23MenuCharMenu_InvDetails4HideEv 0x4528b8
004528b8: push     {r4, lr}
004528bc: mov      r4, r0
004528c0: bl       #0x452810
004528c4: mov      r0, r4
004528c8: pop      {r4, lr}
004528cc: b        #0x424af4

# _ZN20MenuCharMenu_InvMain18CreateAvatarCameraEv 0x4528d0
004528d0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
004528d4: ldr      r4, [pc, #0x234]
004528d8: ldr      r5, [pc, #0x234]
004528dc: sub      sp, sp, #0x30
004528e0: add      r4, pc, r4
004528e4: ldr      r3, [r4, r5]
004528e8: ldr      r3, [r3]
004528ec: cmp      r3, #0
004528f0: beq      #0x4528fc
004528f4: add      sp, sp, #0x30
004528f8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
004528fc: bl       #0x42ca8c
00452900: ldr      r3, [r0, #0x60]
00452904: cmp      r3, #0
00452908: beq      #0x452af0
0045290c: ldr      r6, [pc, #0x204]
00452910: mov      r2, #0x43000000
00452914: mov      ip, #0xc4000000
00452918: mov      r3, #0
0045291c: add      ip, ip, #0x480000
00452920: add      r2, r2, #0x480000
00452924: mov      r1, #0
00452928: mov      r0, #0x38c
0045292c: mov      r8, r1
00452930: str      ip, [sp, #0x28]
00452934: str      r3, [sp, #0x1c]
00452938: str      r2, [sp, #0x20]
0045293c: str      r3, [sp, #0x24]
00452940: str      r2, [sp, #0x2c]
00452944: str      r3, [sp, #0x18]
00452948: bl       #0x5341ac
0045294c: add      r2, sp, #0x24
00452950: add      r3, sp, #0x18
00452954: mvn      r1, #0
00452958: mov      r7, r0
0045295c: str      r8, [sp]
00452960: bl       #0x583734
00452964: ldr      sb, [r4, r6]
00452968: ldr      sl, [r4, r5]
0045296c: mov      r1, r7
00452970: ldr      r3, [sb, #0x10]
00452974: str      r7, [sl]
00452978: ldr      r3, [r3, #0x1c]
0045297c: ldr      r3, [r3, #4]
00452980: mov      r0, r3
00452984: ldr      r3, [r3]
00452988: mov      lr, pc
0045298c: ldr      pc, [r3, #0x5c]
00452990: ldr      r0, [sb, #0x40]
00452994: mov      r1, r8
00452998: mov      r2, #1
0045299c: bl       #0x36e478
004529a0: ldr      r3, [r0, #0x660]
004529a4: cmp      r3, r8
004529a8: beq      #0x4529d0
004529ac: ldr      r3, [r3, #0x2d8]
004529b0: cmp      r3, r8
004529b4: beq      #0x4529d0
004529b8: ldr      r3, [r3, #8]
004529bc: ldr      r1, [sl]
004529c0: mov      r0, r3
004529c4: ldr      r3, [r3]
004529c8: mov      lr, pc
004529cc: ldr      pc, [r3, #0x5c]
004529d0: ldr      r5, [r4, r5]
004529d4: ldr      r3, [r5]
004529d8: ldr      r2, [r3]
004529dc: ldr      r0, [r2, #-0xc]
004529e0: add      r0, r3, r0
004529e4: bl       #0x31d584
004529e8: ldr      r4, [r4, r6]
004529ec: ldr      r1, [r5]
004529f0: ldr      r3, [r4, #0x10]
004529f4: ldr      r0, [r3, #0x1c]
004529f8: bl       #0x5890c0
004529fc: ldr      r0, [r5]
00452a00: mov      ip, #0x3f800000
00452a04: mov      r2, #0
00452a08: ldr      r3, [r0]
00452a0c: add      r1, sp, #0xc
00452a10: ldr      r3, [r3, #0x114]
00452a14: str      ip, [sp, #0x14]
00452a18: str      r2, [sp, #0x10]
00452a1c: str      r2, [sp, #0xc]
00452a20: blx      r3
00452a24: ldr      r3, [r5]
00452a28: movw     r1, #0x78e9
00452a2c: movt     r1, #0x3fd5
00452a30: mov      r0, r3
00452a34: ldr      r3, [r3]
00452a38: mov      lr, pc
00452a3c: ldr      pc, [r3, #0x138]
00452a40: ldr      r3, [r5]
00452a44: movw     r1, #0xfb1a
00452a48: movt     r1, #0x3f0e
00452a4c: mov      r0, r3
00452a50: ldr      r3, [r3]
00452a54: mov      lr, pc
00452a58: ldr      pc, [r3, #0x13c]
00452a5c: ldr      r3, [r5]
00452a60: mov      r1, #0x41000000
00452a64: add      r1, r1, #0x200000
00452a68: ldr      r2, [r3]
00452a6c: ldr      r2, [r2, #-0xc]
00452a70: add      r3, r3, r2
00452a74: ldr      r2, [r3, #4]
00452a78: add      r2, r2, #1
00452a7c: str      r2, [r3, #4]
00452a80: ldr      r3, [r5]
00452a84: mov      r0, r3
00452a88: ldr      r3, [r3]
00452a8c: mov      lr, pc
00452a90: ldr      pc, [r3, #0x130]
00452a94: ldr      r3, [r5]
00452a98: mov      r1, #0x44000000
00452a9c: add      r1, r1, #0x7a0000
00452aa0: mov      r0, r3
00452aa4: ldr      r3, [r3]
00452aa8: mov      lr, pc
00452aac: ldr      pc, [r3, #0x134]
00452ab0: ldr      r0, [r4, #0x40]
00452ab4: mov      r1, #0
00452ab8: mov      r2, #1
00452abc: bl       #0x36e478
00452ac0: ldr      r4, [r0, #0x660]
00452ac4: cmp      r4, #0
00452ac8: beq      #0x4528f4
00452acc: add      r4, r4, #0x4f0
00452ad0: add      r4, r4, #0xc
00452ad4: mov      r0, r4
00452ad8: bl       #0x3c03f0
00452adc: subs     r1, r0, #0
00452ae0: bne      #0x4528f4
00452ae4: mov      r0, r4
00452ae8: bl       #0x3c1a00
00452aec: b        #0x4528f4
00452af0: bl       #0x42ca8c
00452af4: ldr      r6, [pc, #0x1c]
00452af8: ldr      r3, [r4, r6]
00452afc: ldr      r3, [r3, #0x10]
00452b00: ldr      r3, [r3, #0x1c]
00452b04: ldr      r3, [r3, #0xe4]
00452b08: str      r3, [r0, #0x60]
00452b0c: b        #0x452910
00452b10: ldrheq   r2, [r4], #-0x10
00452b14: andeq    r1, r0, ip, lsl #15
00452b18: strdeq   r3, r4, [r0], -r4

# _ZN20MenuCharMenu_InvMain19DestroyAvatarCameraEv 0x452810
00452810: push     {r4, r5, r6, lr}
00452814: ldr      r4, [pc, #0x78]
00452818: ldr      r3, [pc, #0x78]
0045281c: add      r4, pc, r4
00452820: ldr      r5, [r4, r3]
00452824: ldr      r3, [r5]
00452828: cmp      r3, #0
0045282c: beq      #0x452890
00452830: mov      r0, r3
00452834: ldr      r3, [r3]
00452838: mov      lr, pc
0045283c: ldr      pc, [r3, #0x68]
00452840: ldr      r3, [r5]
00452844: ldr      r2, [r3]
00452848: ldr      r0, [r2, #-0xc]
0045284c: add      r0, r3, r0
00452850: bl       #0x31d584
00452854: mov      r3, #0
00452858: str      r3, [r5]
0045285c: bl       #0x42ca8c
00452860: ldr      r3, [r0, #0x60]
00452864: cmp      r3, #0
00452868: beq      #0x452890
0045286c: ldr      r3, [pc, #0x28]
00452870: ldr      r3, [r4, r3]
00452874: ldr      r3, [r3, #0x10]
00452878: ldr      r4, [r3, #0x1c]
0045287c: bl       #0x42ca8c
00452880: ldr      r1, [r0, #0x60]
00452884: mov      r0, r4
00452888: pop      {r4, r5, r6, lr}
0045288c: b        #0x5890c0
00452890: pop      {r4, r5, r6, pc}
00452894: subseq   r2, r4, r4, ror r2
00452898: andeq    r1, r0, ip, lsl #15
0045289c: strdeq   r3, r4, [r0], -r4

# _ZN20MenuCharMenu_InvMain19RenderCharacterPaneERN7gameswf12render_stateEPv 0x452468
00452468: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0045246c: mov      r0, r1
00452470: sub      sp, sp, #0x5c
00452474: mov      r4, r1
00452478: ldr      r5, [r1, #4]
0045247c: bl       #0x42204c
00452480: ldr      r1, [pc, #0x378]
00452484: mov      r2, r0
00452488: mov      r0, r5
0045248c: add      r1, pc, r1
00452490: bl       #0x7a8a84
00452494: ldr      r5, [pc, #0x368]
00452498: mov      r1, r0
0045249c: add      r0, sp, #0x30
004524a0: bl       #0x416a7c
004524a4: ldr      r3, [pc, #0x35c]
004524a8: add      r5, pc, r5
004524ac: ldr      r0, [r4, #4]
004524b0: ldr      r6, [r5, r3]
004524b4: ldr      r3, [r6, #0x10]
004524b8: ldr      r8, [r3, #0x10]
004524bc: ldr      r3, [r8, #0xcc]
004524c0: ldr      r3, [r3, #-4]
004524c4: ldr      r2, [r3, #0x14]
004524c8: str      r2, [sp, #0x20]
004524cc: ldr      r2, [r3, #0x18]
004524d0: str      r2, [sp, #0x24]
004524d4: ldr      r2, [r3, #0x1c]
004524d8: str      r2, [sp, #0x28]
004524dc: ldr      r3, [r3, #0x20]
004524e0: str      r3, [sp, #0x2c]
004524e4: bl       #0x7a7cac
004524e8: bl       #0x416538
004524ec: mov      r7, r0
004524f0: ldr      r0, [r4, #4]
004524f4: bl       #0x7a7cac
004524f8: bl       #0x416578
004524fc: mov      r1, r7
00452500: mov      r4, r0
00452504: ldr      r0, [sp, #0x30]
00452508: bl       #0x30ec94
0045250c: bl       #0x30e4cc
00452510: mov      r1, r7
00452514: mov      sb, r0
00452518: ldr      r0, [sp, #0x34]
0045251c: bl       #0x30ec94
00452520: bl       #0x30e4cc
00452524: mov      r1, r4
00452528: mov      r7, r0
0045252c: ldr      r0, [sp, #0x38]
00452530: bl       #0x30ec94
00452534: bl       #0x30e4cc
00452538: mov      r1, r4
0045253c: mov      sl, r0
00452540: ldr      r0, [sp, #0x3c]
00452544: bl       #0x30ec94
00452548: bl       #0x30e4cc
0045254c: str      r7, [sp, #0x18]
00452550: str      r0, [sp, #0x1c]
00452554: str      sb, [sp, #0x10]
00452558: str      sl, [sp, #0x14]
0045255c: ldr      r3, [r8, #0xcc]
00452560: add      r1, sp, #0x10
00452564: ldr      r3, [r3, #-4]
00452568: mov      r0, r3
0045256c: ldr      r3, [r3]
00452570: mov      lr, pc
00452574: ldr      pc, [r3, #0xc]
00452578: ldr      r3, [sp, #0x10]
0045257c: ldr      r0, [sp, #0x18]
00452580: rsb      r0, r3, r0
00452584: ldr      r3, [pc, #0x280]
00452588: ldr      r4, [r5, r3]
0045258c: bl       #0x30e964
00452590: ldr      r3, [sp, #0x14]
00452594: mov      r7, r0
00452598: ldr      r0, [sp, #0x1c]
0045259c: ldr      r5, [r4]
004525a0: rsb      r0, r3, r0
004525a4: bl       #0x30e964
004525a8: mov      r1, r0
004525ac: mov      r0, r7
004525b0: bl       #0x30ec94
004525b4: ldr      r3, [r5]
004525b8: mov      r1, r0
004525bc: mov      r0, r5
004525c0: mov      lr, pc
004525c4: ldr      pc, [r3, #0x138]
004525c8: ldr      r3, [r6, #0x10]
004525cc: ldr      r1, [r4]
004525d0: ldr      r0, [r3, #0x1c]
004525d4: bl       #0x5890c0
004525d8: mov      r0, r6
004525dc: bl       #0x31f594
004525e0: mov      r1, #0
004525e4: mov      fp, r0
004525e8: mov      r2, #1
004525ec: ldr      r0, [r6, #0x40]
004525f0: bl       #0x36e478
004525f4: ldr      r5, [r0, #0x660]
004525f8: cmp      r5, #0
004525fc: beq      #0x4527f8
00452600: add      r0, r5, #0x490
00452604: add      r0, r0, #0xc
00452608: bl       #0x3caf3c
0045260c: ldr      r3, [r6, #0x10]
00452610: mov      r0, r6
00452614: mov      r7, #0
00452618: ldr      r4, [r3, #0x1c]
0045261c: mov      sl, sp
00452620: ldr      r3, [r4]
00452624: ldr      sb, [r3, #0x60]
00452628: bl       #0x31f66c
0045262c: bl       #0x30e2e0
00452630: mov      r2, #0
00452634: mov      r1, r0
00452638: mov      r0, r4
0045263c: blx      sb
00452640: ldr      r3, [r5, #0x2d8]
00452644: ldr      r0, [r4, #0x254]
00452648: ldr      r4, [r3, #8]
0045264c: bl       #0x30e4cc
00452650: mov      r1, r0
00452654: mov      r0, r4
00452658: bl       #0x35c268
0045265c: ldr      r3, [r4]
00452660: mov      r0, r4
00452664: mov      lr, pc
00452668: ldr      pc, [r3, #0xa0]
0045266c: ldr      r3, [r4]
00452670: add      r1, sp, #0x4c
00452674: mov      r0, r4
00452678: ldr      r3, [r3, #0xa4]
0045267c: str      r7, [sp, #0x4c]
00452680: str      r7, [sp, #0x50]
00452684: str      r7, [sp, #0x54]
00452688: blx      r3
0045268c: ldr      r3, [r4]
00452690: mov      r0, r4
00452694: mov      lr, pc
00452698: ldr      pc, [r3, #0x98]
0045269c: str      r7, [sp, #0x48]
004526a0: str      r7, [sp, #0x40]
004526a4: str      r7, [sp, #0x44]
004526a8: ldr      r3, [r4]
004526ac: mov      r0, r4
004526b0: mov      lr, pc
004526b4: ldr      pc, [r3, #0x98]
004526b8: add      r1, sp, #0x40
004526bc: bl       #0x432e58
004526c0: movw     r1, #0xfa35
004526c4: ldr      r0, [sp, #0x40]
004526c8: movt     r1, #0x3c8e
004526cc: bl       #0x30ed6c
004526d0: movw     r1, #0xfa35
004526d4: mov      r7, r0
004526d8: movt     r1, #0x3c8e
004526dc: ldr      r0, [sp, #0x44]
004526e0: str      r7, [sp, #0x40]
004526e4: bl       #0x30ed6c
004526e8: movw     r1, #0xfa35
004526ec: mov      sb, r0
004526f0: movt     r1, #0x3c8e
004526f4: ldr      r0, [sp, #0x48]
004526f8: str      sb, [sp, #0x44]
004526fc: bl       #0x30ed6c
00452700: str      r0, [sp, #0x48]
00452704: ldr      ip, [r4]
00452708: mov      r2, sb
0045270c: mov      r1, r7
00452710: mov      r3, #0xbf000000
00452714: mov      r0, sp
00452718: ldr      r7, [ip, #0x9c]
0045271c: bl       #0x35c9d8
00452720: mov      r0, r4
00452724: mov      r1, sp
00452728: blx      r7
0045272c: ldr      r3, [r4]
00452730: mov      r0, r4
00452734: mov      r1, #1
00452738: mov      lr, pc
0045273c: ldr      pc, [r3, #0xb8]
00452740: ldr      r3, [r6, #0x10]
00452744: ldr      r3, [r3, #0x1c]
00452748: ldr      r2, [r3, #0xe4]
0045274c: cmp      r2, #0
00452750: beq      #0x452768
00452754: mov      r0, r3
00452758: mov      r1, r4
0045275c: ldr      r3, [r3]
00452760: mov      lr, pc
00452764: ldr      pc, [r3, #0x3c]
00452768: ldr      r3, [r8, #0xcc]
0045276c: add      r1, sp, #0x20
00452770: ldr      r3, [r3, #-4]
00452774: mov      r0, r3
00452778: ldr      r3, [r3]
0045277c: mov      lr, pc
00452780: ldr      pc, [r3, #0xc]
00452784: mov      r0, r5
00452788: add      r1, r5, #0x160
0045278c: mov      r2, #1
00452790: bl       #0x393db4
00452794: ldr      r3, [r5, #0x2e0]
00452798: cmp      r3, #0
0045279c: beq      #0x4527c4
004527a0: mov      r0, r3
004527a4: ldr      r3, [r3]
004527a8: mov      lr, pc
004527ac: ldr      pc, [r3, #8]
004527b0: ldr      r3, [r5, #0x2e0]
004527b4: mov      r0, r3
004527b8: ldr      r3, [r3]
004527bc: mov      lr, pc
004527c0: ldr      pc, [r3, #0xc]
004527c4: ldr      r4, [fp, #0x128]
004527c8: mov      r1, r5
004527cc: mov      r2, #0
004527d0: mov      r0, r4
004527d4: bl       #0x4119c4
004527d8: mov      r0, r4
004527dc: bl       #0x40f45c
004527e0: mov      r0, r4
004527e4: ldr      r3, [r4]
004527e8: mov      lr, pc
004527ec: ldr      pc, [r3, #0x10]
004527f0: ldr      r0, [r5, #0x2d8]
004527f4: bl       #0x472948
004527f8: add      sp, sp, #0x5c
004527fc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00452800: subeq    sb, r7, ip, lsr #1
00452804: subseq   r2, r4, r8, ror #11
00452808: strdeq   r3, r4, [r0], -r4
0045280c: andeq    r1, r0, ip, lsl #15

# _ZN9Character12ReloadSkillsEv 0x3a9db4
003a9db4: push     {r4, r5, r6, lr}
003a9db8: add      r5, r0, #0x560
003a9dbc: sub      sp, sp, #0x18
003a9dc0: mov      r4, r0
003a9dc4: mov      r0, r5
003a9dc8: bl       #0x3e0af8
003a9dcc: add      r6, r4, #0x3c8
003a9dd0: mov      r1, #0x20
003a9dd4: mov      r0, r4
003a9dd8: bl       #0x3bc4d0
003a9ddc: mov      r0, r6
003a9de0: bl       #0x3d8cfc
003a9de4: mov      r0, r6
003a9de8: bl       #0x3d8894
003a9dec: mov      r1, #1
003a9df0: mov      r0, r5
003a9df4: bl       #0x3e0810
003a9df8: mov      r0, r4
003a9dfc: bl       #0x3a9d10
003a9e00: mov      r0, r4
003a9e04: bl       #0x3bb828
003a9e08: cmp      r0, #0xb
003a9e0c: bgt      #0x3a9e64
003a9e10: mov      r5, #0
003a9e14: bl       #0x42ca8c
003a9e18: ldr      r1, [pc, #0x88]
003a9e1c: ldr      r3, [r0, #0xf4]
003a9e20: ldr      r2, [pc, #0x84]
003a9e24: add      r4, sp, #0xc
003a9e28: ldr      r0, [r3, #0x138]
003a9e2c: mov      ip, #1
003a9e30: mov      lr, #0
003a9e34: add      r1, pc, r1
003a9e38: add      r2, pc, r2
003a9e3c: mov      r3, r4
003a9e40: strb     lr, [sp, #0xc]
003a9e44: strb     r5, [sp, #0x10]
003a9e48: str      ip, [sp]
003a9e4c: strb     ip, [sp, #0xd]
003a9e50: bl       #0x7ad7e8
003a9e54: mov      r0, r4
003a9e58: bl       #0x797124
003a9e5c: add      sp, sp, #0x18
003a9e60: pop      {r4, r5, r6, pc}
003a9e64: mov      r0, r4
003a9e68: bl       #0x3bb7fc
003a9e6c: movw     r3, #0x107
003a9e70: cmp      r0, r3
003a9e74: beq      #0x3a9ea0
003a9e78: mov      r0, r4
003a9e7c: bl       #0x3bb7fc
003a9e80: movw     r3, #0x145
003a9e84: cmp      r0, r3
003a9e88: beq      #0x3a9ea0
003a9e8c: mov      r0, r4
003a9e90: bl       #0x3bb7fc
003a9e94: movw     r3, #0x122
003a9e98: cmp      r0, r3
003a9e9c: bne      #0x3a9e10
003a9ea0: mov      r5, #1
003a9ea4: b        #0x3a9e14
003a9ea8: subseq   sb, r1, r4, lsr r7
003a9eac: subseq   sb, r1, r0, asr r7

# _ZN6CharAI15AI_ReloadSkillsEv 0x3d8cfc
003d8cfc: push     {r4, r5, r6, r7, r8, lr}
003d8d00: ldr      r4, [r0, #0xb4]
003d8d04: ldr      r5, [r0, #0xb8]
003d8d08: mov      r6, r0
003d8d0c: cmp      r4, r5
003d8d10: beq      #0x3d8d54
003d8d14: mov      r7, #0
003d8d18: ldr      r3, [r4]
003d8d1c: cmp      r3, #0
003d8d20: beq      #0x3d8d38
003d8d24: mov      r0, r3
003d8d28: ldr      r3, [r3]
003d8d2c: mov      lr, pc
003d8d30: ldr      pc, [r3, #4]
003d8d34: str      r7, [r4]
003d8d38: add      r4, r4, #4
003d8d3c: cmp      r5, r4
003d8d40: bne      #0x3d8d18
003d8d44: ldr      r3, [r6, #0xb4]
003d8d48: ldr      r2, [r6, #0xb8]
003d8d4c: cmp      r3, r2
003d8d50: strne    r3, [r6, #0xb8]
003d8d54: ldr      r0, [r6, #4]
003d8d58: bl       #0x3bbe2c
003d8d5c: mov      r0, r6
003d8d60: bl       #0x3ce044
003d8d64: mov      r0, r6
003d8d68: pop      {r4, r5, r6, r7, r8, lr}
003d8d6c: b        #0x3d8894

# _ZN9Character7SG_LoadEi 0x3bc4d0
003bc4d0: movw     r3, #0x14e8
003bc4d4: ldr      r0, [r0, r3]
003bc4d8: cmp      r0, #0
003bc4dc: bxeq     lr
003bc4e0: b        #0x465430

# _ZN14CharProperties20PROPS_RemoveAllBuffsEv 0x3e0af8
003e0af8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e0afc: ldr      r2, [pc, #0x160]
003e0b00: sub      sp, sp, #0x34
003e0b04: add      r3, r0, #0xe10
003e0b08: add      r2, pc, r2
003e0b0c: str      r2, [sp, #8]
003e0b10: ldr      r2, [pc, #0x150]
003e0b14: add      r3, r3, #8
003e0b18: str      r3, [sp, #4]
003e0b1c: ldr      sl, [r0, #0xe20]
003e0b20: mov      r7, r0
003e0b24: add      sb, sp, #0x20
003e0b28: add      r4, sp, #0x10
003e0b2c: str      r2, [sp, #0xc]
003e0b30: ldr      r3, [sp, #4]
003e0b34: cmp      r3, sl
003e0b38: beq      #0x3e0bec
003e0b3c: add      r5, sl, #0x34
003e0b40: ldm      r5, {r0, r1, r2, r3}
003e0b44: stm      sb, {r0, r1, r2, r3}
003e0b48: add      r0, sl, #0x44
003e0b4c: mov      r1, sb
003e0b50: bl       #0x3de870
003e0b54: subs     r8, r0, #0
003e0b58: beq      #0x3e0ba8
003e0b5c: mov      r6, #0
003e0b60: ldm      r5, {r0, r1, r2, r3}
003e0b64: stm      r4, {r0, r1, r2, r3}
003e0b68: mov      r1, r6
003e0b6c: mov      r0, r4
003e0b70: bl       #0x3de8b4
003e0b74: ldr      r3, [sp, #0x10]
003e0b78: ldr      r0, [r7, #4]
003e0b7c: add      r6, r6, #1
003e0b80: ldr      fp, [r3]
003e0b84: add      r0, r0, #0x3b4
003e0b88: ldr      r1, [fp, #0x388]
003e0b8c: bl       #0x3db2d8
003e0b90: mov      r0, fp
003e0b94: bl       #0x4c5740
003e0b98: mov      r0, fp
003e0b9c: bl       #0x310440
003e0ba0: cmp      r6, r8
003e0ba4: bne      #0x3e0b60
003e0ba8: ldr      r2, [sp, #8]
003e0bac: ldr      r3, [sp, #0xc]
003e0bb0: add      r1, sl, #0x18
003e0bb4: ldr      r0, [r2, r3]
003e0bb8: bl       #0x494978
003e0bbc: ldr      r2, [sl, #0xc]
003e0bc0: cmp      r2, #0
003e0bc4: bne      #0x3e0bd0
003e0bc8: b        #0x3e0c30
003e0bcc: mov      r2, r3
003e0bd0: ldr      r3, [r2, #8]
003e0bd4: cmp      r3, #0
003e0bd8: bne      #0x3e0bcc
003e0bdc: ldr      r3, [sp, #4]
003e0be0: mov      sl, r2
003e0be4: cmp      r3, sl
003e0be8: bne      #0x3e0b3c
003e0bec: ldr      r3, [r7, #0xe28]
003e0bf0: cmp      r3, #0
003e0bf4: beq      #0x3e0c1c
003e0bf8: ldr      r0, [sp, #4]
003e0bfc: ldr      r1, [r7, #0xe1c]
003e0c00: bl       #0x3e0ab8
003e0c04: ldr      r2, [sp, #4]
003e0c08: mov      r3, #0
003e0c0c: str      r3, [r7, #0xe28]
003e0c10: str      r2, [r7, #0xe24]
003e0c14: str      r2, [r7, #0xe20]
003e0c18: str      r3, [r7, #0xe1c]
003e0c1c: mov      r0, r7
003e0c20: mov      r1, #1
003e0c24: bl       #0x3e0810
003e0c28: add      sp, sp, #0x34
003e0c2c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e0c30: ldr      r3, [sl, #4]
003e0c34: ldr      r1, [r3, #0xc]
003e0c38: cmp      r1, sl
003e0c3c: bne      #0x3e0c58
003e0c40: mov      sl, r3
003e0c44: ldr      r3, [r3, #4]
003e0c48: ldr      r2, [r3, #0xc]
003e0c4c: cmp      r2, sl
003e0c50: beq      #0x3e0c40
003e0c54: ldr      r2, [sl, #0xc]
003e0c58: cmp      r3, r2
003e0c5c: movne    sl, r3
003e0c60: b        #0x3e0b30
003e0c64: subseq   r3, fp, r8, lsl #31
003e0c68: andeq    r1, r0, r8, lsl #22

# _ZN9Character26INV_CheckItemsRequirementsEv 0x3a9d10
003a9d10: push     {r4, r5, r6, r7, r8, lr}
003a9d14: add      r5, r0, #0x37c
003a9d18: mov      r6, r0
003a9d1c: mov      r0, r5
003a9d20: bl       #0x3ffd20
003a9d24: subs     r7, r0, #0
003a9d28: beq      #0x3a9d8c
003a9d2c: mov      r4, #0
003a9d30: mov      r8, r4
003a9d34: b        #0x3a9d44
003a9d38: add      r4, r4, #1
003a9d3c: cmp      r4, r7
003a9d40: beq      #0x3a9d84
003a9d44: mov      r1, r4
003a9d48: mov      r0, r5
003a9d4c: bl       #0x3ffe3c
003a9d50: mov      r1, r0
003a9d54: mov      r0, r6
003a9d58: bl       #0x3a4930
003a9d5c: cmp      r0, #0
003a9d60: bne      #0x3a9d38
003a9d64: mov      r1, r4
003a9d68: mov      r0, r5
003a9d6c: mvn      r2, #0
003a9d70: add      r4, r4, #1
003a9d74: bl       #0x40050c
003a9d78: cmp      r4, r7
003a9d7c: mov      r8, #1
003a9d80: bne      #0x3a9d44
003a9d84: cmp      r8, #0
003a9d88: bne      #0x3a9d90
003a9d8c: pop      {r4, r5, r6, r7, r8, pc}
003a9d90: add      r0, r6, #0x560
003a9d94: bl       #0x3e08a8
003a9d98: mov      r0, r6
003a9d9c: bl       #0x3a9d10
003a9da0: mov      r0, r6
003a9da4: bl       #0x3a999c
003a9da8: mov      r0, r6
003a9dac: pop      {r4, r5, r6, r7, r8, lr}
003a9db0: b        #0x3bd140

# _ZN17CharAISkillScriptD1Ev 0x3cc238
003cc238: ldr      r3, [pc, #0x24]
003cc23c: ldr      r2, [pc, #0x24]
003cc240: push     {r4, lr}
003cc244: add      r3, pc, r3
003cc248: ldr      r2, [r3, r2]
003cc24c: mov      r4, r0
003cc250: add      r2, r2, #8
003cc254: str      r2, [r0], #0xc
003cc258: bl       #0x319228
003cc25c: mov      r0, r4
003cc260: pop      {r4, pc}
003cc264: subseq   r8, ip, ip, asr #16
003cc268: andeq    r0, r0, ip, lsr #23

# _ZN17CharAISkillScriptD0Ev 0x3cc448
003cc448: ldr      r3, [pc, #0x2c]
003cc44c: ldr      r2, [pc, #0x2c]
003cc450: push     {r4, lr}
003cc454: add      r3, pc, r3
003cc458: ldr      r2, [r3, r2]
003cc45c: mov      r4, r0
003cc460: add      r2, r2, #8
003cc464: str      r2, [r0], #0xc
003cc468: bl       #0x319228
003cc46c: mov      r0, r4
003cc470: bl       #0x310440
003cc474: mov      r0, r4
003cc478: pop      {r4, pc}
003cc47c: subseq   r8, ip, ip, lsr r6
003cc480: andeq    r0, r0, ip, lsr #23

# _ZN9Character15SG_ReloadSkillsEv 0x3bbe2c
003bbe2c: movw     r3, #0x14e8
003bbe30: ldr      r0, [r0, r3]
003bbe34: cmp      r0, #0
003bbe38: bxeq     lr
003bbe3c: b        #0x467324

# _ZN14PlayerSavegame15SG_ReloadSkillsEv 0x467324
00467324: push     {r4, lr}
00467328: mov      r4, r0
0046732c: ldr      r0, [r0, #0x80]
00467330: cmp      r0, #0
00467334: beq      #0x467344
00467338: bl       #0x310440
0046733c: mov      r3, #0
00467340: str      r3, [r4, #0x80]
00467344: mov      r0, r4
00467348: bl       #0x469764
0046734c: mov      r0, r4
00467350: mov      r1, #8
00467354: pop      {r4, lr}
00467358: b        #0x465430

# _ZN14PlayerSavegame7SG_LoadEi 0x465430
00465430: push     {r4, r5, r6, lr}
00465434: mov      r5, r0
00465438: mov      r4, r1
0046543c: bl       #0x464f4c
00465440: mov      r0, r5
00465444: mov      r1, r4
00465448: pop      {r4, r5, r6, lr}
0046544c: b        #0x468574

# _ZN14PlayerSavegame5_LoadEi 0x464f4c
00464f4c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00464f50: ldr      r5, [pc, #0x40c]
00464f54: ldr      r7, [pc, #0x40c]
00464f58: ldr      r8, [r0, #8]
00464f5c: add      r5, pc, r5
00464f60: ldr      r3, [r5, r7]
00464f64: sub      sp, sp, #0x2c
00464f68: cmp      r8, #0
00464f6c: ldr      r3, [r3]
00464f70: mov      r4, r0
00464f74: mov      r6, r1
00464f78: str      r3, [sp, #0x24]
00464f7c: beq      #0x4652f0
00464f80: tst      r6, #1
00464f84: beq      #0x46508c
00464f88: ldr      r0, [r4, #8]
00464f8c: cmp      r0, #0
00464f90: beq      #0x46508c
00464f94: ldr      r3, [pc, #0x3d0]
00464f98: ldr      r1, [pc, #0x3d0]
00464f9c: str      r4, [sp]
00464fa0: ldr      r2, [r5, r3]
00464fa4: ldr      r3, [pc, #0x3c8]
00464fa8: add      r1, pc, r1
00464fac: ldr      r3, [r5, r3]
00464fb0: bl       #0x315848
00464fb4: ldr      r3, [pc, #0x3bc]
00464fb8: ldr      r1, [pc, #0x3bc]
00464fbc: ldr      r0, [r4, #8]
00464fc0: ldr      r2, [r5, r3]
00464fc4: ldr      r3, [pc, #0x3b4]
00464fc8: add      r1, pc, r1
00464fcc: str      r4, [sp]
00464fd0: ldr      r3, [r5, r3]
00464fd4: bl       #0x315848
00464fd8: ldr      r3, [pc, #0x3a4]
00464fdc: ldr      r1, [pc, #0x3a4]
00464fe0: ldr      r0, [r4, #8]
00464fe4: ldr      r2, [r5, r3]
00464fe8: ldr      r3, [pc, #0x39c]
00464fec: add      r1, pc, r1
00464ff0: str      r4, [sp]
00464ff4: ldr      r3, [r5, r3]
00464ff8: bl       #0x315848
00464ffc: ldr      r3, [pc, #0x38c]
00465000: ldr      r1, [pc, #0x38c]
00465004: ldr      r0, [r4, #8]
00465008: ldr      r2, [r5, r3]
0046500c: ldr      r3, [pc, #0x384]
00465010: add      r1, pc, r1
00465014: str      r4, [sp]
00465018: ldr      r3, [r5, r3]
0046501c: bl       #0x315848
00465020: ldr      r3, [pc, #0x374]
00465024: ldr      r1, [pc, #0x374]
00465028: ldr      r0, [r4, #8]
0046502c: ldr      r2, [r5, r3]
00465030: ldr      r3, [pc, #0x36c]
00465034: add      r1, pc, r1
00465038: str      r4, [sp]
0046503c: ldr      r3, [r5, r3]
00465040: bl       #0x315848
00465044: ldr      r3, [pc, #0x35c]
00465048: ldr      r1, [pc, #0x35c]
0046504c: ldr      r0, [r4, #8]
00465050: ldr      r2, [r5, r3]
00465054: ldr      r3, [pc, #0x354]
00465058: add      r1, pc, r1
0046505c: str      r4, [sp]
00465060: ldr      r3, [r5, r3]
00465064: bl       #0x315848
00465068: ldr      r3, [pc, #0x344]
0046506c: ldr      r1, [pc, #0x344]
00465070: ldr      r0, [r4, #8]
00465074: ldr      r2, [r5, r3]
00465078: ldr      r3, [pc, #0x33c]
0046507c: add      r1, pc, r1
00465080: str      r4, [sp]
00465084: ldr      r3, [r5, r3]
00465088: bl       #0x315848
0046508c: tst      r6, #2
00465090: bne      #0x4652c4
00465094: tst      r6, #4
00465098: beq      #0x4651f8
0046509c: ldr      r0, [r4, #8]
004650a0: cmp      r0, #0
004650a4: beq      #0x4651f8
004650a8: ldr      r3, [pc, #0x310]
004650ac: ldr      r1, [pc, #0x310]
004650b0: str      r4, [sp]
004650b4: ldr      r2, [r5, r3]
004650b8: ldr      r3, [pc, #0x308]
004650bc: add      r1, pc, r1
004650c0: ldr      r3, [r5, r3]
004650c4: bl       #0x315848
004650c8: ldr      r3, [pc, #0x2fc]
004650cc: ldr      r1, [pc, #0x2fc]
004650d0: ldr      r0, [r4, #8]
004650d4: ldr      r2, [r5, r3]
004650d8: ldr      r3, [pc, #0x2f4]
004650dc: add      r1, pc, r1
004650e0: str      r4, [sp]
004650e4: ldr      r3, [r5, r3]
004650e8: bl       #0x315848
004650ec: ldr      r3, [pc, #0x2e4]
004650f0: ldr      r1, [pc, #0x2e4]
004650f4: ldr      r0, [r4, #8]
004650f8: ldr      r2, [r5, r3]
004650fc: ldr      r3, [pc, #0x2dc]
00465100: add      r1, pc, r1
00465104: str      r4, [sp]
00465108: ldr      r3, [r5, r3]
0046510c: bl       #0x315848
00465110: ldr      r8, [r4, #8]
00465114: bl       #0x7fd794
00465118: ldrb     r3, [r0, #5]
0046511c: cmp      r3, #0
00465120: beq      #0x465148
00465124: ldr      r3, [pc, #0x2b8]
00465128: ldr      r3, [r5, r3]
0046512c: ldr      r3, [r3, #0x40]
00465130: ldrb     r3, [r3, #0x71b]
00465134: cmp      r3, #0
00465138: bne      #0x465148
0046513c: ldr      r3, [pc, #0x2a4]
00465140: ldr      r2, [r5, r3]
00465144: b        #0x46514c
00465148: mov      r2, #0
0046514c: ldr      r3, [pc, #0x298]
00465150: ldr      r1, [pc, #0x298]
00465154: mov      r0, r8
00465158: ldr      r3, [r5, r3]
0046515c: add      r1, pc, r1
00465160: str      r4, [sp]
00465164: bl       #0x315848
00465168: ldr      r3, [pc, #0x284]
0046516c: ldr      r1, [pc, #0x284]
00465170: ldr      r0, [r4, #8]
00465174: ldr      r2, [r5, r3]
00465178: ldr      r3, [pc, #0x27c]
0046517c: add      r1, pc, r1
00465180: str      r4, [sp]
00465184: ldr      r3, [r5, r3]
00465188: bl       #0x315848
0046518c: ldr      r3, [pc, #0x26c]
00465190: ldr      r1, [pc, #0x26c]
00465194: ldr      r0, [r4, #8]
00465198: ldr      r2, [r5, r3]
0046519c: ldr      r3, [pc, #0x264]
004651a0: add      r1, pc, r1
004651a4: str      r4, [sp]
004651a8: ldr      r3, [r5, r3]
004651ac: bl       #0x315848
004651b0: ldr      r3, [pc, #0x254]
004651b4: ldr      r1, [pc, #0x254]
004651b8: ldr      r0, [r4, #8]
004651bc: ldr      r2, [r5, r3]
004651c0: ldr      r3, [pc, #0x24c]
004651c4: add      r1, pc, r1
004651c8: str      r4, [sp]
004651cc: ldr      r3, [r5, r3]
004651d0: bl       #0x315848
004651d4: ldr      r3, [pc, #0x23c]
004651d8: ldr      r1, [pc, #0x23c]
004651dc: ldr      r0, [r4, #8]
004651e0: ldr      r2, [r5, r3]
004651e4: ldr      r3, [pc, #0x234]
004651e8: add      r1, pc, r1
004651ec: str      r4, [sp]
004651f0: ldr      r3, [r5, r3]
004651f4: bl       #0x315848
004651f8: tst      r6, #8
004651fc: beq      #0x46522c
00465200: ldr      r0, [r4, #8]
00465204: cmp      r0, #0
00465208: beq      #0x46522c
0046520c: ldr      r3, [pc, #0x1b8]
00465210: ldr      r1, [pc, #0x20c]
00465214: str      r4, [sp]
00465218: ldr      r2, [r5, r3]
0046521c: ldr      r3, [pc, #0x1b0]
00465220: add      r1, pc, r1
00465224: ldr      r3, [r5, r3]
00465228: bl       #0x315848
0046522c: tst      r6, #0x20
00465230: beq      #0x465260
00465234: ldr      r0, [r4, #8]
00465238: cmp      r0, #0
0046523c: beq      #0x465260
00465240: ldr      r3, [pc, #0x1b8]
00465244: ldr      r1, [pc, #0x1dc]
00465248: str      r4, [sp]
0046524c: ldr      r2, [r5, r3]
00465250: ldr      r3, [pc, #0x1b0]
00465254: add      r1, pc, r1
00465258: ldr      r3, [r5, r3]
0046525c: bl       #0x315848
00465260: tst      r6, #0x10
00465264: beq      #0x4652a8
00465268: ldr      r3, [r4, #8]
0046526c: cmp      r3, #0
00465270: beq      #0x4652a8
00465274: add      r0, r4, #0xb8
00465278: bl       #0x46c1a8
0046527c: add      r0, r4, #0x118
00465280: bl       #0x46c1a8
00465284: ldr      r3, [pc, #0x168]
00465288: ldr      r1, [pc, #0x19c]
0046528c: ldr      r0, [r4, #8]
00465290: ldr      r2, [r5, r3]
00465294: ldr      r3, [pc, #0x160]
00465298: add      r1, pc, r1
0046529c: str      r4, [sp]
004652a0: ldr      r3, [r5, r3]
004652a4: bl       #0x315848
004652a8: ldr      r3, [r5, r7]
004652ac: ldr      r2, [sp, #0x24]
004652b0: ldr      r3, [r3]
004652b4: cmp      r2, r3
004652b8: bne      #0x465360
004652bc: add      sp, sp, #0x2c
004652c0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004652c4: mov      r0, r4
004652c8: bl       #0x46954c
004652cc: mov      r0, r4
004652d0: bl       #0x469764
004652d4: mov      r0, r4
004652d8: bl       #0x4694c8
004652dc: add      r0, r4, #0xb8
004652e0: bl       #0x46c1a8
004652e4: add      r0, r4, #0x118
004652e8: bl       #0x46c1a8
004652ec: b        #0x465094
004652f0: ldr      r3, [r0, #4]
004652f4: cmn      r3, #1
004652f8: beq      #0x464f80
004652fc: add      sl, sp, #0xc
00465300: mov      r0, sl
00465304: mov      r1, #0x10
00465308: str      sl, [sp, #0x1c]
0046530c: str      sl, [sp, #0x20]
00465310: bl       #0x31167c
00465314: ldr      r2, [sp, #0x1c]
00465318: mov      r3, r8
0046531c: mov      r1, sl
00465320: strb     r8, [r2]
00465324: ldr      r0, [r4, #4]
00465328: mov      r2, r8
0046532c: bl       #0x463c84
00465330: mov      r1, r8
00465334: mov      r0, #0x3c
00465338: ldr      fp, [sp, #0x20]
0046533c: bl       #0x310570
00465340: mov      r1, fp
00465344: mov      sb, r0
00465348: mov      r2, r8
0046534c: bl       #0x315ed8
00465350: str      sb, [r4, #8]
00465354: mov      r0, sl
00465358: bl       #0x3139ac
0046535c: b        #0x464f80
00465360: bl       #0x30e310
00465364: subseq   pc, r2, r4, lsr fp
00465368: andeq    r4, r0, ip, lsr #1
0046536c: andeq    r1, r0, r8, lsl #10
00465370: subeq    r8, r6, r0, asr #4
00465374: strdeq   r1, r2, [r0], -r4
00465378: andeq    r1, r0, r8, ror #30
0046537c: subeq    r8, r6, r8, lsr #4
00465380: strdeq   r1, r2, [r0], -r0
00465384: strheq   r2, [r0], -ip
00465388: subeq    r8, r6, ip, lsl #4
0046538c: andeq    r2, r0, r8, asr #29
00465390: andeq    r0, r0, r8, lsr #26
00465394: strdeq   r8, sb, [r6], #-0x10
00465398: strheq   r1, [r0], -r8
0046539c: andeq    r3, r0, r4, lsr #12
004653a0: ldrdeq   r8, sb, [r6], #-0x14
004653a4: andeq    r3, r0, r8, ror r7
004653a8: andeq    r3, r0, ip, asr #26
004653ac: strheq   r8, [r6], #-0x18
004653b0: strheq   r2, [r0], -r0
004653b4: andeq    r4, r0, r0, lsr r0
004653b8: umaaleq  r8, r6, ip, r1
004653bc: andeq    r2, r0, ip, ror #12
004653c0: andeq    r3, r0, r0, lsl #30
004653c4: subeq    r8, r6, r4, ror #2
004653c8: andeq    r4, r0, ip, ror r6
004653cc: muleq    r0, r4, r8
004653d0: subeq    r8, r6, ip, asr #2
004653d4: andeq    r1, r0, r4, lsr fp
004653d8: andeq    r1, r0, ip, lsr r3
004653dc: subeq    r8, r6, r0, lsr r1
004653e0: andeq    r0, r0, r4, lsr #14
004653e4: strdeq   r3, r4, [r0], -r4
004653e8: andeq    r2, r0, ip, asr #21
004653ec: strdeq   r2, r3, [r0], -r4
004653f0: ldrdeq   r8, sb, [r6], #-0xc
004653f4: andeq    r2, r0, r8, asr #12
004653f8: subeq    r8, r6, r4, asr #1
004653fc: andeq    r3, r0, r8, lsl r4
00465400: andeq    r0, r0, r4, asr #14
00465404: subeq    r8, r6, r8, lsr #1
00465408: andeq    r4, r0, ip, ror #23
0046540c: andeq    r2, r0, ip, lsr r4
00465410: subeq    r8, r6, ip, lsl #1
00465414: andeq    r4, r0, r8, lsl #5
00465418: andeq    r0, r0, ip, lsl #29
0046541c: subeq    r8, r6, r0, ror r0
00465420: andeq    r1, r0, r0, ror #24
00465424: subeq    r8, r6, r8
00465428: strdeq   r7, r8, [r6], #-0xf4
0046542c: subeq    r7, r6, r8, lsr #31

# _ZN14PlayerSavegame22_LoadVolatileQuestsLogEi 0x468574
00468574: push     {r4, r5, r6, r7, r8, lr}
00468578: ldr      r4, [pc, #0xa4]
0046857c: tst      r1, #0x14
00468580: mov      r5, r0
00468584: add      r4, pc, r4
00468588: bne      #0x468590
0046858c: pop      {r4, r5, r6, r7, r8, pc}
00468590: bl       #0x7fd794
00468594: ldrb     r3, [r0, #5]
00468598: cmp      r3, #0
0046859c: beq      #0x46858c
004685a0: ldr      r3, [pc, #0x80]
004685a4: ldr      r6, [r4, r3]
004685a8: ldr      r0, [r6, #0x40]
004685ac: bl       #0x36f074
004685b0: cmp      r0, #0
004685b4: ldreq    r7, [r6, #0x40]
004685b8: bne      #0x468610
004685bc: add      r6, r7, #0x6e0
004685c0: ldr      r3, [r7, #0x6e0]
004685c4: mov      r0, r6
004685c8: mov      lr, pc
004685cc: ldr      pc, [r3, #8]
004685d0: orrs     r1, r0, r1
004685d4: beq      #0x46858c
004685d8: ldr      r1, [r7, #0x6e0]
004685dc: mov      r0, r6
004685e0: mov      r2, #0
004685e4: mov      r3, #0
004685e8: mov      lr, pc
004685ec: ldr      pc, [r1, #0x20]
004685f0: ldr      r3, [pc, #0x34]
004685f4: add      r0, r5, #0x118
004685f8: mov      r2, r6
004685fc: ldr      r1, [r4, r3]
00468600: mov      r3, #0
00468604: ldr      r1, [r1]
00468608: pop      {r4, r5, r6, r7, r8, lr}
0046860c: b        #0x46c48c
00468610: ldr      r7, [r6, #0x40]
00468614: ldrb     r3, [r7, #0x719]
00468618: cmp      r3, #0
0046861c: beq      #0x46858c
00468620: b        #0x4685bc
00468624: subseq   ip, r2, ip, lsl #10
00468628: strdeq   r3, r4, [r0], -r4
0046862c: muleq    r0, ip, sl

# _Z18NativeReloadSkillsRKN7gameswf7fn_callE 0x43dbb8
0043dbb8: push     {r4, lr}
0043dbbc: ldr      r3, [r0, #0x10]
0043dbc0: cmp      r3, #1
0043dbc4: movne    r0, #0
0043dbc8: beq      #0x43dbe8
0043dbcc: mov      r1, #0
0043dbd0: bl       #0x43c388
0043dbd4: cmp      r0, #0
0043dbd8: beq      #0x43dbe4
0043dbdc: pop      {r4, lr}
0043dbe0: b        #0x3a9db4
0043dbe4: pop      {r4, pc}
0043dbe8: ldr      r3, [r0, #0xc]
0043dbec: ldr      r2, [r0, #0x14]
0043dbf0: mov      r0, #0xc
0043dbf4: ldr      r3, [r3]
0043dbf8: mla      r0, r0, r2, r3
0043dbfc: bl       #0x797a54
0043dc00: bl       #0x30ea24
0043dc04: b        #0x43dbcc
