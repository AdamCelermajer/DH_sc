
# _ZN15VisualFXManager16_BuildAnimFXSetsEv
00496954: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00496958: ldr      r1, [pc, #0x238]
0049695c: ldr      r2, [r0, #0x1c]
00496960: ldr      r3, [r0, #0x20]
00496964: sub      sp, sp, #0x4c
00496968: add      r1, pc, r1
0049696c: cmp      r2, r3
00496970: str      r1, [sp, #4]
00496974: mov      sl, r0
00496978: beq      #0x496984
0049697c: add      sp, sp, #0x4c
00496980: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00496984: ldr      r2, [pc, #0x210]
00496988: ldr      r3, [r1, r2]
0049698c: str      r2, [sp, #0x14]
00496990: ldr      r3, [r3]
00496994: cmp      r3, #0
00496998: beq      #0x49697c
0049699c: add      r3, sp, #0x2c
004969a0: str      r3, [sp, #8]
004969a4: ldr      r3, [pc, #0x1f4]
004969a8: ldr      r2, [sp, #8]
004969ac: mov      sb, #0
004969b0: ldr      r3, [r1, r3]
004969b4: add      r1, r0, #0x1c
004969b8: str      r1, [sp, #0x18]
004969bc: ldr      r1, [sp, #8]
004969c0: add      r2, r2, #0x10
004969c4: str      r2, [sp, #0x10]
004969c8: add      r1, r1, #4
004969cc: add      r2, sp, #0x44
004969d0: str      r3, [sp, #0x1c]
004969d4: mov      r7, sb
004969d8: mov      r8, #1
004969dc: str      r1, [sp, #0x20]
004969e0: str      r2, [sp, #0x24]
004969e4: str      r0, [sp, #0xc]
004969e8: mov      sl, sb
004969ec: ldr      r3, [sp, #0x1c]
004969f0: ldr      r1, [sp, #0x10]
004969f4: str      r7, [sp, #0x30]
004969f8: ldr      r5, [r3]
004969fc: str      r7, [sp, #0x34]
00496a00: str      r7, [sp, #0x38]
00496a04: add      r5, r5, sb
00496a08: str      r1, [sp, #0x3c]
00496a0c: str      r1, [sp, #0x40]
00496a10: str      r5, [sp, #0x2c]
00496a14: ldr      r1, [r5, #0xc]
00496a18: cmp      r1, #0
00496a1c: beq      #0x496ab4
00496a20: mov      r4, r7
00496a24: mov      r6, r7
00496a28: ldr      r3, [r5, #0x10]
00496a2c: add      r3, r3, r4
00496a30: ldr      r2, [r3, #4]
00496a34: cmn      r2, #1
00496a38: beq      #0x496aa4
00496a3c: ldr      fp, [r3, #0x1c]
00496a40: cmp      fp, #0
00496a44: bne      #0x496b08
00496a48: mov      r1, fp
00496a4c: mov      r0, #8
00496a50: bl       #0x310570
00496a54: str      r0, [sp, #0x44]
00496a58: strb     fp, [r0]
00496a5c: ldr      r3, [r5, #0x10]
00496a60: add      r3, r3, r4
00496a64: ldr      r2, [r3, #4]
00496a68: ldr      r3, [sp, #0x44]
00496a6c: str      r2, [r3, #4]
00496a70: ldr      r1, [sp, #0x34]
00496a74: ldr      r3, [sp, #0x38]
00496a78: cmp      r1, r3
00496a7c: beq      #0x496b78
00496a80: ldr      r3, [sp, #0x44]
00496a84: str      r3, [r1]
00496a88: ldr      r3, [sp, #0x34]
00496a8c: add      r3, r3, #4
00496a90: str      r3, [sp, #0x34]
00496a94: ldrb     r3, [r5, #4]
00496a98: cmp      r3, #0
00496a9c: bne      #0x496aec
00496aa0: ldr      r1, [r5, #0xc]
00496aa4: add      r6, r6, #1
00496aa8: cmp      r1, r6
00496aac: add      r4, r4, #0x30
00496ab0: bhi      #0x496a28
00496ab4: ldr      r1, [sp, #8]
00496ab8: ldr      r0, [sp, #0x18]
00496abc: bl       #0x4941d8
00496ac0: ldr      r0, [sp, #8]
00496ac4: bl       #0x4940d8
00496ac8: ldr      r1, [sp, #4]
00496acc: ldr      r2, [sp, #0x14]
00496ad0: add      sl, sl, #1
00496ad4: add      sb, sb, #0x18
00496ad8: ldr      r3, [r1, r2]
00496adc: ldr      r3, [r3]
00496ae0: cmp      r3, sl
00496ae4: bhi      #0x4969ec
00496ae8: b        #0x49697c
00496aec: ldr      r3, [r5, #0x10]
00496af0: ldr      r0, [sp, #0xc]
00496af4: add      r3, r3, r4
00496af8: ldr      r1, [r3, #4]
00496afc: bl       #0x496434
00496b00: ldr      r1, [r5, #0xc]
00496b04: b        #0x496aa4
00496b08: mov      r1, #0
00496b0c: mov      r0, #8
00496b10: bl       #0x310570
00496b14: str      r0, [sp, #0x44]
00496b18: strb     r8, [r0]
00496b1c: ldr      r3, [r5, #0x10]
00496b20: add      r3, r3, r4
00496b24: ldr      r2, [r3, #4]
00496b28: ldr      r3, [sp, #0x44]
00496b2c: str      r2, [r3, #4]
00496b30: ldr      r1, [sp, #0x34]
00496b34: ldr      r3, [sp, #0x38]
00496b38: cmp      r1, r3
00496b3c: beq      #0x496b88
00496b40: ldr      r3, [sp, #0x44]
00496b44: str      r3, [r1]
00496b48: ldr      r3, [sp, #0x34]
00496b4c: add      r3, r3, #4
00496b50: str      r3, [sp, #0x34]
00496b54: ldrb     r3, [r5, #4]
00496b58: cmp      r3, #0
00496b5c: beq      #0x496aa0
00496b60: ldr      r3, [r5, #0x10]
00496b64: ldr      r0, [sp, #0xc]
00496b68: add      r3, r3, r4
00496b6c: ldr      r1, [r3, #4]
00496b70: bl       #0x4967e8
00496b74: b        #0x496aa0
00496b78: ldr      r0, [sp, #0x20]
00496b7c: ldr      r2, [sp, #0x24]
00496b80: bl       #0x49448c
00496b84: b        #0x496a94
00496b88: ldr      r0, [sp, #0x20]
00496b8c: ldr      r2, [sp, #0x24]
00496b90: bl       #0x49448c
00496b94: b        #0x496b54
00496b98: subeq    lr, pc, r8, lsr #2
00496b9c: andeq    r0, r0, r4, asr #13
00496ba0: andeq    r3, r0, r0, ror sb

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

# _ZN6Arrays10EffectDict4readEP11IStreamBase
004b8458: push     {r4, r5, r6, r7, r8, sl, lr}
004b845c: sub      sp, sp, #0xc
004b8460: mov      sl, r0
004b8464: bl       #0x313a90
004b8468: ldr      r6, [pc, #0x124]
004b846c: mov      r3, #1
004b8470: cmp      r3, #0
004b8474: str      r0, [sp, #4]
004b8478: str      r3, [sp]
004b847c: add      r6, pc, r6
004b8480: bne      #0x4b84c8
004b8484: add      r3, sp, #4
004b8488: add      r2, r3, #2
004b848c: add      r3, r3, #1
004b8490: ldrb     r0, [r2, #1]
004b8494: ldrb     r1, [r3, #-1]
004b8498: cmp      r2, r3
004b849c: eor      r1, r0, r1
004b84a0: strb     r1, [r3, #-1]
004b84a4: ldrb     r0, [r2, #1]
004b84a8: eor      r1, r1, r0
004b84ac: strb     r1, [r2, #1]
004b84b0: ldrb     r0, [r3, #-1]
004b84b4: sub      r2, r2, #1
004b84b8: eor      r1, r1, r0
004b84bc: strb     r1, [r3, #-1]
004b84c0: add      r3, r3, #1
004b84c4: bhi      #0x4b8490
004b84c8: bl       #0x4a3ea4
004b84cc: ldr      r7, [pc, #0xc4]
004b84d0: ldr      r4, [sp, #4]
004b84d4: mov      r5, #0xc
004b84d8: ldr      r3, [r6, r7]
004b84dc: mul      r0, r5, r4
004b84e0: str      r4, [r3]
004b84e4: add      r0, r0, #8
004b84e8: mov      r1, #1
004b84ec: bl       #0x31056c
004b84f0: cmp      r4, #0
004b84f4: str      r5, [r0]
004b84f8: str      r4, [r0, #4]
004b84fc: add      r3, r0, #8
004b8500: beq      #0x4b8530
004b8504: ldr      r1, [pc, #0x90]
004b8508: mov      r2, #0
004b850c: mov      ip, r2
004b8510: ldr      r1, [r6, r1]
004b8514: add      r1, r1, #8
004b8518: add      r2, r2, #1
004b851c: cmp      r2, r4
004b8520: str      r1, [r0, #8]
004b8524: str      ip, [r0, #0x10]
004b8528: add      r0, r0, #0xc
004b852c: bne      #0x4b8518
004b8530: ldr      r2, [r6, r7]
004b8534: ldr      r8, [pc, #0x64]
004b8538: ldr      r1, [r2]
004b853c: ldr      r2, [r6, r8]
004b8540: cmp      r1, #0
004b8544: str      r3, [r2]
004b8548: beq      #0x4b858c
004b854c: mov      r4, #0
004b8550: mov      r5, r4
004b8554: b        #0x4b8560
004b8558: ldr      r3, [r6, r8]
004b855c: ldr      r3, [r3]
004b8560: add      r0, r3, r4
004b8564: mov      r1, sl
004b8568: ldr      r3, [r3, r4]
004b856c: mov      lr, pc
004b8570: ldr      pc, [r3, #0xc]
004b8574: ldr      r3, [r6, r7]
004b8578: add      r5, r5, #1
004b857c: add      r4, r4, #0xc
004b8580: ldr      r3, [r3]
004b8584: cmp      r3, r5
004b8588: bhi      #0x4b8558
004b858c: add      sp, sp, #0xc
004b8590: pop      {r4, r5, r6, r7, r8, sl, pc}
004b8594: subeq    ip, sp, r4, lsl r6
004b8598: andeq    r0, r0, r8, lsl #23
004b859c: strheq   r0, [r0], -ip
004b85a0: strheq   r1, [r0], -r8

# _ZN9VectorSetIiE16push_back_uniqueERKi
0049437c: push     {r4, r5, r6, r7, lr}
00494380: mov      r4, r0
00494384: sub      sp, sp, #0xc
00494388: mov      r5, r1
0049438c: add      r3, sp, #4
00494390: ldr      r0, [r0]
00494394: ldr      r1, [r4, #4]
00494398: mov      r2, r5
0049439c: bl       #0x369350
004943a0: ldr      r3, [r4, #4]
004943a4: mov      r6, r0
004943a8: cmp      r0, r3
004943ac: beq      #0x4943b8
004943b0: add      sp, sp, #0xc
004943b4: pop      {r4, r5, r6, r7, pc}
004943b8: ldr      r3, [r4, #8]
004943bc: cmp      r0, r3
004943c0: beq      #0x4943dc
004943c4: ldr      r3, [r5]
004943c8: str      r3, [r0]
004943cc: ldr      r3, [r4, #4]
004943d0: add      r3, r3, #4
004943d4: str      r3, [r4, #4]
004943d8: b        #0x4943b0
004943dc: ldr      r3, [r4]
004943e0: rsb      r3, r3, r0
004943e4: asr      r3, r3, #2
004943e8: cmp      r3, #1
004943ec: addhs    r1, r3, r3
004943f0: addlo    r1, r3, #1
004943f4: cmn      r1, #0xc0000001
004943f8: bhi      #0x49447c
004943fc: cmp      r3, r1
00494400: bhi      #0x49447c
00494404: add      r2, sp, #8
00494408: str      r1, [r2, #-8]!
0049440c: add      r0, r4, #8
00494410: mov      r2, sp
00494414: bl       #0x35fd5c
00494418: ldr      r1, [r4]
0049441c: mov      r7, r0
00494420: subs     r6, r6, r1
00494424: moveq    r6, r0
00494428: beq      #0x494438
0049442c: mov      r2, r6
00494430: bl       #0x30df38
00494434: add      r6, r0, r6
00494438: ldr      r3, [r5]
0049443c: str      r3, [r6], #4
00494440: ldr      r0, [r4]
00494444: ldr      r1, [r4, #8]
00494448: cmp      r0, #0
0049444c: beq      #0x494464
00494450: rsb      r1, r0, r1
00494454: bic      r1, r1, #3
00494458: cmp      r1, #0x80
0049445c: bhi      #0x494484
00494460: bl       #0x708f00
00494464: ldr      r3, [sp]
00494468: str      r7, [r4]
0049446c: str      r6, [r4, #4]
00494470: add      r7, r7, r3, lsl #2
00494474: str      r7, [r4, #8]
00494478: b        #0x4943b0
0049447c: mvn      r1, #0xc0000000
00494480: b        #0x494404
00494484: bl       #0x310440
00494488: b        #0x494464

# _ZN15VisualFXManagerC1Ev
004932c4: ldr      r1, [pc, #0x54]
004932c8: ldr      ip, [pc, #0x54]
004932cc: mov      r2, #0
004932d0: add      r1, pc, r1
004932d4: ldr      ip, [r1, ip]
004932d8: str      r4, [sp, #-4]!
004932dc: add      r4, r0, #8
004932e0: add      ip, ip, #8
004932e4: str      r2, [r0, #0x30]
004932e8: str      ip, [r0]
004932ec: str      r4, [r0, #0xc]
004932f0: strb     r2, [r0, #4]
004932f4: str      r4, [r0, #8]
004932f8: str      r2, [r0, #0x10]
004932fc: str      r2, [r0, #0x14]
00493300: str      r2, [r0, #0x18]
00493304: str      r2, [r0, #0x1c]
00493308: str      r2, [r0, #0x20]
0049330c: str      r2, [r0, #0x24]
00493310: str      r2, [r0, #0x28]
00493314: str      r2, [r0, #0x2c]
00493318: ldm      sp!, {r4}
0049331c: bx       lr
00493320: subseq   r1, r0, r0, asr #15
00493324: andeq    r3, r0, r0, lsr r4

# _ZN15VisualFXManager10_GetAnimFXEi
00494ad4: push     {r4, r5, r6, r7, r8, sl, lr}
00494ad8: ldr      sl, [pc, #0x118]
00494adc: subs     r7, r1, #0
00494ae0: sub      sp, sp, #0xc
00494ae4: add      sl, pc, sl
00494ae8: bge      #0x494afc
00494aec: mov      r5, #0
00494af0: mov      r0, r5
00494af4: add      sp, sp, #0xc
00494af8: pop      {r4, r5, r6, r7, r8, sl, pc}
00494afc: ldr      r3, [pc, #0xf8]
00494b00: ldr      r3, [sl, r3]
00494b04: ldr      r3, [r3]
00494b08: cmp      r7, r3
00494b0c: bge      #0x494aec
00494b10: ldr      r3, [r0, #0x28]
00494b14: mov      r8, #0x18
00494b18: mla      r8, r8, r7, r3
00494b1c: ldr      r2, [r8, #8]
00494b20: ldr      r3, [r8, #4]
00494b24: rsb      r3, r3, r2
00494b28: asrs     r3, r3, #2
00494b2c: bne      #0x494bd0
00494b30: mov      r6, r8
00494b34: ldr      r4, [r6, #0x10]!
00494b38: cmp      r4, r6
00494b3c: beq      #0x494b58
00494b40: ldr      r4, [r4]
00494b44: add      r3, r3, #1
00494b48: cmp      r6, r4
00494b4c: bne      #0x494b40
00494b50: cmp      r3, #5
00494b54: bhi      #0x494aec
00494b58: mov      r1, #0
00494b5c: mov      r0, #0x54
00494b60: bl       #0x310570
00494b64: mov      r1, r7
00494b68: mov      r5, r0
00494b6c: bl       #0x492374
00494b70: ldr      r3, [pc, #0x88]
00494b74: mov      r0, #0xc
00494b78: ldr      r2, [pc, #0x84]
00494b7c: ldr      r1, [sl, r3]
00494b80: mvn      ip, #0
00494b84: mov      r3, #0x3f800000
00494b88: ldr      r1, [r1]
00494b8c: add      r2, pc, r2
00494b90: mla      r7, r0, r7, r1
00494b94: mov      r0, r5
00494b98: ldr      r1, [r7, #8]
00494b9c: str      ip, [sp]
00494ba0: mov      ip, #1
00494ba4: str      ip, [sp, #4]
00494ba8: bl       #0x49296c
00494bac: mov      r0, r6
00494bb0: bl       #0x494ab4
00494bb4: str      r5, [r0, #8]
00494bb8: ldr      r3, [r8, #0x14]
00494bbc: str      r4, [r0]
00494bc0: str      r3, [r0, #4]
00494bc4: str      r0, [r3]
00494bc8: str      r0, [r8, #0x14]
00494bcc: b        #0x494af0
00494bd0: ldr      r5, [r2, #-4]
00494bd4: mov      r1, #0x3f800000
00494bd8: add      r4, r8, #0x10
00494bdc: ldr      r0, [r5, #0x2c]
00494be0: bl       #0x472708
00494be4: ldr      r3, [r8, #8]
00494be8: mov      r0, r4
00494bec: sub      r3, r3, #4
00494bf0: str      r3, [r8, #8]
00494bf4: b        #0x494bb0
00494bf8: subeq    pc, pc, ip, lsr #31
00494bfc: andeq    r0, r0, r8, lsl #23
00494c00: strheq   r1, [r0], -r8
00494c04: subeq    r6, r3, ip, ror ip

# _ZN15VisualFXManager17_PreCacheAnimDictEv
004933d8: mov      r3, #1
004933dc: strb     r3, [r0, #4]
004933e0: bx       lr

# _ZN15VisualFXManager14DropAnimatedFXERP10AnimatedFX
00494978: push     {r4, r5, r6, r7, r8, lr}
0049497c: ldr      r3, [r1]
00494980: ldr      r6, [pc, #0x124]
00494984: mov      r5, r1
00494988: cmp      r3, #0
0049498c: add      r6, pc, r6
00494990: beq      #0x4949e4
00494994: ldrb     r2, [r0, #4]
00494998: cmp      r2, #0
0049499c: beq      #0x4949e8
004949a0: ldr      r3, [r3, #8]
004949a4: cmp      r3, #0
004949a8: blt      #0x4949d8
004949ac: ldr      r2, [r0, #0x2c]
004949b0: ldr      r0, [r0, #0x28]
004949b4: rsb      r2, r0, r2
004949b8: asr      r2, r2, #3
004949bc: add      r1, r2, r2, lsl #2
004949c0: add      r1, r1, r1, lsl #4
004949c4: add      r1, r1, r1, lsl #8
004949c8: add      r1, r1, r1, lsl #16
004949cc: add      r2, r2, r1, lsl #1
004949d0: cmp      r3, r2
004949d4: blo      #0x4949f0
004949d8: mov      r3, #0
004949dc: str      r3, [r5]
004949e0: pop      {r4, r5, r6, r7, r8, pc}
004949e4: pop      {r4, r5, r6, r7, r8, pc}
004949e8: str      r2, [r1]
004949ec: pop      {r4, r5, r6, r7, r8, pc}
004949f0: mov      r8, #0x18
004949f4: mla      r8, r8, r3, r0
004949f8: mov      r7, r8
004949fc: ldr      r0, [r7, #0x10]!
00494a00: cmp      r7, r0
00494a04: beq      #0x494a28
00494a08: ldr      r3, [r0, #8]
00494a0c: ldr      r2, [r5]
00494a10: ldr      r4, [r0]
00494a14: cmp      r2, r3
00494a18: beq      #0x494a90
00494a1c: mov      r0, r4
00494a20: cmp      r7, r0
00494a24: bne      #0x494a08
00494a28: add      r0, r8, #4
00494a2c: mov      r1, r5
00494a30: bl       #0x494550
00494a34: ldr      r3, [r5]
00494a38: mov      r4, #0
00494a3c: mov      r1, #1
00494a40: mov      r0, r3
00494a44: str      r4, [r3, #0x28]
00494a48: bl       #0x492aa0
00494a4c: ldr      r2, [pc, #0x5c]
00494a50: ldr      r3, [r5]
00494a54: mov      r1, r4
00494a58: ldr      r2, [r6, r2]
00494a5c: mov      r0, r3
00494a60: ldr      lr, [r2]
00494a64: ldr      ip, [r2, #4]
00494a68: ldr      r2, [r2, #8]
00494a6c: str      lr, [r3, #0x34]
00494a70: str      ip, [r3, #0x38]
00494a74: str      r2, [r3, #0x3c]
00494a78: bl       #0x492aa0
00494a7c: ldr      r0, [r5]
00494a80: mov      r1, r4
00494a84: bl       #0x492ef0
00494a88: str      r4, [r5]
00494a8c: pop      {r4, r5, r6, r7, r8, pc}
00494a90: ldr      r3, [r0, #4]
00494a94: mov      r1, #0xc
00494a98: str      r4, [r3]
00494a9c: str      r3, [r4, #4]
00494aa0: bl       #0x708f00
00494aa4: mov      r0, r4
00494aa8: b        #0x494a20
00494aac: subseq   r0, r0, r4, lsl #2
00494ab0: andeq    r3, r0, ip, lsr #30

# _ZN15VisualFXManager14_FlushAnimDictEv
00494e3c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00494e40: mov      r5, r0
00494e44: sub      sp, sp, #0xc
00494e48: mov      r4, r0
00494e4c: ldr      r6, [r5, #8]!
00494e50: b        #0x494e64
00494e54: add      r1, r6, #8
00494e58: mov      r0, r4
00494e5c: bl       #0x494978
00494e60: ldr      r6, [r6]
00494e64: cmp      r6, r5
00494e68: bne      #0x494e54
00494e6c: ldr      r8, [r4, #0x28]
00494e70: ldr      r3, [r4, #0x2c]
00494e74: rsb      r2, r8, r3
00494e78: asr      r2, r2, #3
00494e7c: add      r1, r2, r2, lsl #2
00494e80: add      r1, r1, r1, lsl #4
00494e84: add      r1, r1, r1, lsl #8
00494e88: add      r1, r1, r1, lsl #16
00494e8c: add      r2, r2, r1, lsl #1
00494e90: cmp      r2, #0
00494e94: beq      #0x494f74
00494e98: mov      sb, #0
00494e9c: mov      fp, sb
00494ea0: mov      sl, sb
00494ea4: add      r8, r8, sb
00494ea8: ldr      r2, [r8, #8]
00494eac: ldr      r7, [r8, #4]
00494eb0: rsb      r3, r7, r2
00494eb4: lsrs     r3, r3, #2
00494eb8: beq      #0x494ef8
00494ebc: mov      r6, #0
00494ec0: ldr      r3, [r7, r6, lsl #2]
00494ec4: cmp      r3, #0
00494ec8: beq      #0x494ee8
00494ecc: mov      r0, r3
00494ed0: ldr      r3, [r3]
00494ed4: mov      lr, pc
00494ed8: ldr      pc, [r3, #4]
00494edc: str      sl, [r7, r6, lsl #2]
00494ee0: ldr      r2, [r8, #8]
00494ee4: ldr      r7, [r8, #4]
00494ee8: add      r6, r6, #1
00494eec: rsb      r3, r7, r2
00494ef0: cmp      r6, r3, asr #2
00494ef4: blo      #0x494ec0
00494ef8: cmp      r2, r7
00494efc: strne    r7, [r8, #8]
00494f00: ldr      r6, [r8, #0x10]!
00494f04: cmp      r6, r8
00494f08: beq      #0x494f38
00494f0c: ldr      r3, [r6, #8]
00494f10: cmp      r3, #0
00494f14: beq      #0x494f2c
00494f18: mov      r0, r3
00494f1c: ldr      r3, [r3]
00494f20: mov      lr, pc
00494f24: ldr      pc, [r3, #4]
00494f28: str      sl, [r6, #8]
00494f2c: ldr      r6, [r6]
00494f30: cmp      r6, r8
00494f34: bne      #0x494f0c
00494f38: mov      r0, r6
00494f3c: bl       #0x493c04
00494f40: ldr      r8, [r4, #0x28]
00494f44: ldr      r3, [r4, #0x2c]
00494f48: add      fp, fp, #1
00494f4c: add      sb, sb, #0x18
00494f50: rsb      r2, r8, r3
00494f54: asr      r2, r2, #3
00494f58: add      r1, r2, r2, lsl #2
00494f5c: add      r1, r1, r1, lsl #4
00494f60: add      r1, r1, r1, lsl #8
00494f64: add      r1, r1, r1, lsl #16
00494f68: add      r2, r2, r1, lsl #1
00494f6c: cmp      fp, r2
00494f70: blo      #0x494ea4
00494f74: ldr      r1, [r4, #0x1c]
00494f78: ldr      r2, [r4, #0x20]
00494f7c: rsb      r0, r1, r2
00494f80: asr      r0, r0, #3
00494f84: add      ip, r0, r0, lsl #2
00494f88: add      ip, ip, ip, lsl #4
00494f8c: add      ip, ip, ip, lsl #8
00494f90: add      ip, ip, ip, lsl #16
00494f94: add      r0, r0, ip, lsl #1
00494f98: cmp      r0, #0
00494f9c: beq      #0x495050
00494fa0: mov      sb, #0
00494fa4: mov      fp, sb
00494fa8: mov      r7, sb
00494fac: add      sl, r1, sb
00494fb0: mov      r8, sl
00494fb4: ldr      r6, [r8, #0x10]!
00494fb8: cmp      r6, r8
00494fbc: beq      #0x494fe0
00494fc0: ldr      r0, [r6, #8]
00494fc4: cmp      r0, #0
00494fc8: beq      #0x494fd4
00494fcc: bl       #0x310440
00494fd0: str      r7, [r6, #8]
00494fd4: ldr      r6, [r6]
00494fd8: cmp      r6, r8
00494fdc: bne      #0x494fc0
00494fe0: ldr      r6, [sl, #4]
00494fe4: ldr      r3, [sl, #8]
00494fe8: cmp      r3, r6
00494fec: beq      #0x495014
00494ff0: ldr      r0, [r6]
00494ff4: cmp      r0, #0
00494ff8: beq      #0x495008
00494ffc: bl       #0x310440
00495000: str      r7, [r6]
00495004: ldr      r3, [sl, #8]
00495008: add      r6, r6, #4
0049500c: cmp      r6, r3
00495010: bne      #0x494ff0
00495014: ldr      r1, [r4, #0x1c]
00495018: ldr      r2, [r4, #0x20]
0049501c: add      fp, fp, #1
00495020: add      sb, sb, #0x18
00495024: rsb      r3, r1, r2
00495028: asr      r3, r3, #3
0049502c: add      r0, r3, r3, lsl #2
00495030: add      r0, r0, r0, lsl #4
00495034: add      r0, r0, r0, lsl #8
00495038: add      r0, r0, r0, lsl #16
0049503c: add      r3, r3, r0, lsl #1
00495040: cmp      fp, r3
00495044: blo      #0x494fac
00495048: ldr      r8, [r4, #0x28]
0049504c: ldr      r3, [r4, #0x2c]
00495050: cmp      r8, r3
00495054: beq      #0x495074
00495058: mov      r2, r3
0049505c: mov      r1, r8
00495060: add      r0, r4, #0x28
00495064: add      r3, sp, #4
00495068: bl       #0x494dd4
0049506c: ldr      r1, [r4, #0x1c]
00495070: ldr      r2, [r4, #0x20]
00495074: cmp      r1, r2
00495078: beq      #0x495088
0049507c: add      r0, r4, #0x1c
00495080: mov      r3, sp
00495084: bl       #0x494910
00495088: mov      r0, r5
0049508c: bl       #0x493c04
00495090: mov      r3, #0
00495094: strb     r3, [r4, #4]
00495098: add      sp, sp, #0xc
0049509c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

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

# _ZN15VisualFXManager17PreCacheLibrariesEv
00495a88: push     {r4, r5, r6, r7, r8, lr}
00495a8c: ldr      r4, [pc, #0xb0]
00495a90: ldr      r6, [pc, #0xb0]
00495a94: ldr      r2, [pc, #0xb0]
00495a98: add      r4, pc, r4
00495a9c: ldr      r3, [r4, r6]
00495aa0: ldr      r7, [r4, r2]
00495aa4: sub      sp, sp, #0x20
00495aa8: ldr      r3, [r3]
00495aac: mov      r8, r0
00495ab0: mov      r0, r7
00495ab4: str      r3, [sp, #0x1c]
00495ab8: bl       #0x337888
00495abc: ldr      r1, [pc, #0x8c]
00495ac0: add      r5, sp, #4
00495ac4: mov      r2, sp
00495ac8: add      r1, pc, r1
00495acc: mov      r0, r5
00495ad0: bl       #0x3140ec
00495ad4: mov      r0, r7
00495ad8: mov      r1, r5
00495adc: bl       #0x337ec8
00495ae0: mov      r7, r0
00495ae4: ldr      r0, [sp, #0x18]
00495ae8: cmp      r0, r5
00495aec: beq      #0x495b0c
00495af0: cmp      r0, #0
00495af4: beq      #0x495b0c
00495af8: ldr      r1, [sp, #4]
00495afc: rsb      r1, r0, r1
00495b00: cmp      r1, #0x80
00495b04: bhi      #0x495b38
00495b08: bl       #0x708f00
00495b0c: cmp      r7, #0
00495b10: beq      #0x495b1c
00495b14: mov      r0, r8
00495b18: bl       #0x4933d8
00495b1c: ldr      r3, [r4, r6]
00495b20: ldr      r2, [sp, #0x1c]
00495b24: ldr      r3, [r3]
00495b28: cmp      r2, r3
00495b2c: bne      #0x495b40
00495b30: add      sp, sp, #0x20
00495b34: pop      {r4, r5, r6, r7, r8, pc}
00495b38: bl       #0x310440
00495b3c: b        #0x495b0c
00495b40: bl       #0x30e310
00495b44: strdeq   lr, pc, [pc], #-0xf8
00495b48: andeq    r4, r0, ip, lsr #1
00495b4c: andeq    r0, r0, r4, lsl #17
00495b50: subeq    pc, r3, r0, lsl #11

# _ZN7Structs6AnimFX4readEP11IStreamBase
00506990: push     {r4, r5, r6, lr}
00506994: mov      r4, r0
00506998: sub      sp, sp, #8
0050699c: mov      r0, r1
005069a0: mov      r6, r1
005069a4: add      r1, r4, #4
005069a8: bl       #0x459090
005069ac: mov      r3, #1
005069b0: cmp      r3, #0
005069b4: str      r3, [sp, #4]
005069b8: bne      #0x5069fc
005069bc: add      r3, r4, #5
005069c0: add      r2, r4, #6
005069c4: ldrb     r0, [r2, #1]
005069c8: ldrb     r1, [r3, #-1]
005069cc: cmp      r3, r2
005069d0: eor      r1, r0, r1
005069d4: strb     r1, [r3, #-1]
005069d8: ldrb     r0, [r2, #1]
005069dc: eor      r1, r1, r0
005069e0: strb     r1, [r2, #1]
005069e4: ldrb     r0, [r3, #-1]
005069e8: sub      r2, r2, #1
005069ec: eor      r1, r1, r0
005069f0: strb     r1, [r3, #-1]
005069f4: add      r3, r3, #1
005069f8: blo      #0x5069c4
005069fc: add      r1, r4, #8
00506a00: mov      r0, r6
00506a04: bl       #0x4db89c
00506a08: mov      r0, r6
00506a0c: add      r1, r4, #0xc
00506a10: bl       #0x459090
00506a14: mov      r3, #1
00506a18: cmp      r3, #0
00506a1c: str      r3, [sp, #4]
00506a20: bne      #0x506a64
00506a24: add      r3, r4, #0xd
00506a28: add      r2, r4, #0xe
00506a2c: ldrb     r0, [r2, #1]
00506a30: ldrb     r1, [r3, #-1]
00506a34: cmp      r3, r2
00506a38: eor      r1, r0, r1
00506a3c: strb     r1, [r3, #-1]
00506a40: ldrb     r0, [r2, #1]
00506a44: eor      r1, r1, r0
00506a48: strb     r1, [r2, #1]
00506a4c: ldrb     r0, [r3, #-1]
00506a50: sub      r2, r2, #1
00506a54: eor      r1, r1, r0
00506a58: strb     r1, [r3, #-1]
00506a5c: add      r3, r3, #1
00506a60: blo      #0x506a2c
00506a64: add      r1, r4, #0x10
00506a68: mov      r0, r6
00506a6c: bl       #0x4db89c
00506a70: mov      r0, r6
00506a74: add      r1, r4, #0x11
00506a78: bl       #0x4db89c
00506a7c: mov      r0, r6
00506a80: add      r1, r4, #0x14
00506a84: bl       #0x459090
00506a88: mov      r3, #1
00506a8c: cmp      r3, #0
00506a90: str      r3, [sp, #4]
00506a94: bne      #0x506ad8
00506a98: add      r3, r4, #0x15
00506a9c: add      r2, r4, #0x16
00506aa0: ldrb     r0, [r2, #1]
00506aa4: ldrb     r1, [r3, #-1]
00506aa8: cmp      r3, r2
00506aac: eor      r1, r0, r1
00506ab0: strb     r1, [r3, #-1]
00506ab4: ldrb     r0, [r2, #1]
00506ab8: eor      r1, r1, r0
00506abc: strb     r1, [r2, #1]
00506ac0: ldrb     r0, [r3, #-1]
00506ac4: sub      r2, r2, #1
00506ac8: eor      r1, r1, r0
00506acc: strb     r1, [r3, #-1]
00506ad0: add      r3, r3, #1
00506ad4: blo      #0x506aa0
00506ad8: mov      r0, r6
00506adc: add      r1, r4, #0x18
00506ae0: bl       #0x459090
00506ae4: mov      r3, #1
00506ae8: cmp      r3, #0
00506aec: str      r3, [sp, #4]
00506af0: bne      #0x506b34
00506af4: add      r3, r4, #0x19
00506af8: add      r2, r4, #0x1a
00506afc: ldrb     r0, [r2, #1]
00506b00: ldrb     r1, [r3, #-1]
00506b04: cmp      r2, r3
00506b08: eor      r1, r0, r1
00506b0c: strb     r1, [r3, #-1]
00506b10: ldrb     r0, [r2, #1]
00506b14: eor      r1, r1, r0
00506b18: strb     r1, [r2, #1]
00506b1c: ldrb     r0, [r3, #-1]
00506b20: sub      r2, r2, #1
00506b24: eor      r1, r1, r0
00506b28: strb     r1, [r3, #-1]
00506b2c: add      r3, r3, #1
00506b30: bhi      #0x506afc
00506b34: mov      r0, r6
00506b38: add      r1, r4, #0x1c
00506b3c: bl       #0x459090
00506b40: mov      r3, #1
00506b44: cmp      r3, #0
00506b48: str      r3, [sp, #4]
00506b4c: bne      #0x506b90
00506b50: add      r3, r4, #0x1d
00506b54: add      r2, r4, #0x1e
00506b58: ldrb     r0, [r2, #1]
00506b5c: ldrb     r1, [r3, #-1]
00506b60: cmp      r2, r3
00506b64: eor      r1, r0, r1
00506b68: strb     r1, [r3, #-1]
00506b6c: ldrb     r0, [r2, #1]
00506b70: eor      r1, r1, r0
00506b74: strb     r1, [r2, #1]
00506b78: ldrb     r0, [r3, #-1]
00506b7c: sub      r2, r2, #1
00506b80: eor      r1, r1, r0
00506b84: strb     r1, [r3, #-1]
00506b88: add      r3, r3, #1
00506b8c: bhi      #0x506b58
00506b90: add      r1, r4, #0x20
00506b94: mov      r0, r6
00506b98: bl       #0x4db89c
00506b9c: mov      r0, r6
00506ba0: add      r1, r4, #0x21
00506ba4: bl       #0x4db89c
00506ba8: mov      r0, r6
00506bac: add      r1, r4, #0x24
00506bb0: bl       #0x4db94c
00506bb4: mov      r3, #1
00506bb8: cmp      r3, #0
00506bbc: str      r3, [sp, #4]
00506bc0: bne      #0x506c04
00506bc4: add      r3, r4, #0x25
00506bc8: add      r2, r4, #0x26
00506bcc: ldrb     r0, [r2, #1]
00506bd0: ldrb     r1, [r3, #-1]
00506bd4: cmp      r2, r3
00506bd8: eor      r1, r0, r1
00506bdc: strb     r1, [r3, #-1]
00506be0: ldrb     r0, [r2, #1]
00506be4: eor      r1, r1, r0
00506be8: strb     r1, [r2, #1]
00506bec: ldrb     r0, [r3, #-1]
00506bf0: sub      r2, r2, #1
00506bf4: eor      r1, r1, r0
00506bf8: strb     r1, [r3, #-1]
00506bfc: add      r3, r3, #1
00506c00: bhi      #0x506bcc
00506c04: mov      r0, r6
00506c08: add      r1, r4, #0x28
00506c0c: bl       #0x3df1a0
00506c10: mov      r3, #1
00506c14: cmp      r3, #0
00506c18: str      r3, [sp, #4]
00506c1c: bne      #0x506c60
00506c20: add      r3, r4, #0x29
00506c24: add      r2, r4, #0x2a
00506c28: ldrb     r0, [r2, #1]
00506c2c: ldrb     r1, [r3, #-1]
00506c30: cmp      r2, r3
00506c34: eor      r1, r0, r1
00506c38: strb     r1, [r3, #-1]
00506c3c: ldrb     r0, [r2, #1]
00506c40: eor      r1, r1, r0
00506c44: strb     r1, [r2, #1]
00506c48: ldrb     r0, [r3, #-1]
00506c4c: sub      r2, r2, #1
00506c50: eor      r1, r1, r0
00506c54: strb     r1, [r3, #-1]
00506c58: add      r3, r3, #1
00506c5c: bhi      #0x506c28
00506c60: ldr      r0, [r4, #0x2c]
00506c64: cmp      r0, #0
00506c68: beq      #0x506c70
00506c6c: bl       #0x310440
00506c70: ldr      r0, [r4, #0x28]
00506c74: mov      r1, #1
00506c78: mov      r5, #0
00506c7c: add      r0, r0, r1
00506c80: bl       #0x31056c
00506c84: ldr      r2, [r4, #0x28]
00506c88: mov      r1, r0
00506c8c: str      r0, [r4, #0x2c]
00506c90: mov      r3, r5
00506c94: mov      r0, r6
00506c98: bl       #0x317454
00506c9c: ldr      r3, [r4, #0x28]
00506ca0: ldr      r2, [r4, #0x2c]
00506ca4: strb     r5, [r2, r3]
00506ca8: add      sp, sp, #8
00506cac: pop      {r4, r5, r6, pc}

# _ZN15VisualFXManager17_BuildAnimLibraryEv
00496ba4: push     {r4, lr}
00496ba8: ldr      r2, [r0, #0x28]
00496bac: ldr      r3, [r0, #0x2c]
00496bb0: mov      r4, r0
00496bb4: cmp      r2, r3
00496bb8: beq      #0x496bc0
00496bbc: pop      {r4, pc}
00496bc0: mov      r3, #1
00496bc4: strb     r3, [r0, #4]
00496bc8: bl       #0x495364
00496bcc: mov      r0, r4
00496bd0: pop      {r4, lr}
00496bd4: b        #0x496954

# _ZN12VisualObjectC1EP10GameObjectRKSsS3_
00472a0c: push     {r4, r5, r6, r7, r8, sl, lr}
00472a10: ldr      r6, [pc, #0x234]
00472a14: ldr      r5, [pc, #0x234]
00472a18: mov      ip, #0xbf000000
00472a1c: add      r6, pc, r6
00472a20: ldr      r5, [r6, r5]
00472a24: mov      lr, #0
00472a28: add      ip, ip, #0x800000
00472a2c: mov      r7, r1
00472a30: add      r1, r5, #8
00472a34: mov      r5, #0
00472a38: mov      r8, r3
00472a3c: sub      sp, sp, #0xc
00472a40: str      r1, [r0]
00472a44: str      lr, [r0, #0x24]
00472a48: str      ip, [r0, #0x74]
00472a4c: str      lr, [r0, #0x10]
00472a50: str      lr, [r0, #0x14]
00472a54: str      lr, [r0, #0x18]
00472a58: str      lr, [r0, #0x1c]
00472a5c: str      lr, [r0, #0x20]
00472a60: str      ip, [r0, #0x58]
00472a64: str      ip, [r0, #0x5c]
00472a68: str      ip, [r0, #0x60]
00472a6c: str      ip, [r0, #0x64]
00472a70: str      ip, [r0, #0x68]
00472a74: str      r7, [r0, #4]
00472a78: str      r5, [r0, #8]
00472a7c: str      r5, [r0, #0xc]
00472a80: strb     r5, [r0, #0x28]
00472a84: str      r5, [r0, #0x2c]
00472a88: str      r5, [r0, #0x30]
00472a8c: str      r5, [r0, #0x34]
00472a90: str      r5, [r0, #0x38]
00472a94: strb     r5, [r0, #0x3c]
00472a98: str      r5, [r0, #0x40]
00472a9c: str      r5, [r0, #0x44]
00472aa0: str      r5, [r0, #0x48]
00472aa4: str      r5, [r0, #0x4c]
00472aa8: str      r5, [r0, #0x50]
00472aac: str      r5, [r0, #0x54]
00472ab0: strb     r5, [r0, #0x6c]
00472ab4: strb     r5, [r0, #0x7c]
00472ab8: strb     r5, [r0, #0x7d]
00472abc: strb     r5, [r0, #0x7e]
00472ac0: strb     r5, [r0, #0x7f]
00472ac4: str      r5, [r0, #0x80]
00472ac8: str      r5, [r0, #0x84]
00472acc: str      r5, [r0, #0x88]
00472ad0: str      r5, [r0, #0x8c]
00472ad4: str      r5, [r0, #0x90]
00472ad8: str      r5, [r0, #0x94]
00472adc: str      r5, [r0, #0x9c]
00472ae0: str      r5, [r0, #0xa0]
00472ae4: str      r5, [r0, #0xa4]
00472ae8: strb     r5, [r0, #0xa9]
00472aec: mov      sl, r2
00472af0: mov      r4, r0
00472af4: bl       #0x50a564
00472af8: ldr      ip, [r8, #0x10]
00472afc: ldr      r2, [r8, #0x14]
00472b00: ldr      r1, [sl, #0x14]
00472b04: mov      r3, r5
00472b08: cmp      ip, r2
00472b0c: moveq    r2, r5
00472b10: mvn      ip, #0x80000000
00472b14: str      ip, [sp]
00472b18: bl       #0x50a504
00472b1c: cmp      r0, r5
00472b20: str      r0, [r4, #8]
00472b24: beq      #0x472c40
00472b28: mov      r1, r7
00472b2c: mov      r0, r4
00472b30: bl       #0x47295c
00472b34: ldr      r0, [r4, #8]
00472b38: bl       #0x35c854
00472b3c: mov      r0, r4
00472b40: bl       #0x4718f0
00472b44: ldr      r3, [pc, #0x108]
00472b48: ldr      r1, [r4, #8]
00472b4c: ldr      r5, [r6, r3]
00472b50: ldr      r3, [r5, #0x10]
00472b54: ldr      r3, [r3, #0x1c]
00472b58: ldr      r3, [r3, #4]
00472b5c: mov      r0, r3
00472b60: ldr      r3, [r3]
00472b64: mov      lr, pc
00472b68: ldr      pc, [r3, #0x5c]
00472b6c: ldr      r3, [r5, #0x10]
00472b70: ldr      r0, [r3, #0x1c]
00472b74: bl       #0x350ee0
00472b78: ldr      r3, [r5, #0x10]
00472b7c: ldr      r2, [pc, #0xd4]
00472b80: ldr      r1, [r4, #8]
00472b84: ldr      r0, [r3, #0x1c]
00472b88: add      r2, pc, r2
00472b8c: mov      r3, #1
00472b90: bl       #0x35a0e4
00472b94: subs     r2, r0, #0
00472b98: beq      #0x472c08
00472b9c: mov      r3, #1
00472ba0: strb     r3, [r4, #0x28]
00472ba4: ldr      r3, [r5, #0x10]
00472ba8: movw     r1, #0x6164
00472bac: movt     r1, #0x6d65
00472bb0: ldr      r3, [r3, #0x1c]
00472bb4: mov      r0, r3
00472bb8: ldr      r3, [r3]
00472bbc: mov      lr, pc
00472bc0: ldr      pc, [r3, #0x1c]
00472bc4: cmp      r0, #0
00472bc8: str      r0, [r4, #0xc]
00472bcc: beq      #0x472bec
00472bd0: ldr      r3, [r0]
00472bd4: ldr      r3, [r3, #-0xc]
00472bd8: add      r0, r0, r3
00472bdc: ldr      r3, [r0, #4]
00472be0: add      r3, r3, #1
00472be4: str      r3, [r0, #4]
00472be8: ldr      r0, [r4, #0xc]
00472bec: mov      r1, #0
00472bf0: strb     r1, [r0, #0x138]
00472bf4: ldr      r3, [r4, #0xc]
00472bf8: mov      r0, r3
00472bfc: ldr      r3, [r3]
00472c00: mov      lr, pc
00472c04: ldr      pc, [r3, #0x48]
00472c08: mov      r0, r4
00472c0c: bl       #0x47211c
00472c10: mov      r0, r4
00472c14: bl       #0x470a54
00472c18: mov      r1, #0
00472c1c: mov      r0, #8
00472c20: bl       #0x310570
00472c24: ldr      r1, [r4, #8]
00472c28: mov      r5, r0
00472c2c: mov      r2, #0
00472c30: bl       #0x474d30
00472c34: mov      r0, r4
00472c38: mov      r1, r5
00472c3c: bl       #0x470a84
00472c40: mov      r0, r4
00472c44: add      sp, sp, #0xc
00472c48: pop      {r4, r5, r6, r7, r8, sl, pc}
00472c4c: subseq   r2, r2, r4, ror r0
00472c50: muleq    r0, r4, lr
00472c54: strdeq   r3, r4, [r0], -r4
00472c58: subeq    sl, r5, r0, lsr #22

# _Z24SetNodeToSelfIlluminatedPN6glitch5scene10ISceneNodeE
0050e398: push     {r4, r5, r6, lr}
0050e39c: mov      r5, r0
0050e3a0: ldr      r3, [r0]
0050e3a4: mov      lr, pc
0050e3a8: ldr      pc, [r3, #0x88]
0050e3ac: ldr      r4, [r5, #0xf4]!
0050e3b0: cmp      r4, r5
0050e3b4: beq      #0x50e3d4
0050e3b8: cmp      r4, #0
0050e3bc: moveq    r0, r4
0050e3c0: subne    r0, r4, #4
0050e3c4: bl       #0x50e398
0050e3c8: ldr      r4, [r4]
0050e3cc: cmp      r5, r4
0050e3d0: bne      #0x50e3b8
0050e3d4: pop      {r4, r5, r6, pc}

# _ZN14AnimController8SetScaleEfj
00474920: ldr      r3, [pc, #0x58]
00474924: push     {r4, lr}
00474928: mov      r4, r1
0047492c: ldr      r1, [pc, #0x50]
00474930: add      r3, pc, r3
00474934: ldr      ip, [r3, r1]
00474938: ldrb     r3, [ip]
0047493c: cmp      r3, #0
00474940: bne      #0x474948
00474944: pop      {r4, pc}
00474948: mov      r1, r2
0047494c: bl       #0x4748b8
00474950: subs     r3, r0, #0
00474954: beq      #0x474944
00474958: ldr      r3, [r3]
0047495c: mov      lr, pc
00474960: ldr      pc, [r3, #0x44]
00474964: subs     r3, r0, #0
00474968: beq      #0x474944
0047496c: ldr      r3, [r3]
00474970: mov      r1, r4
00474974: mov      lr, pc
00474978: ldr      pc, [r3, #0x48]
0047497c: pop      {r4, pc}
00474980: subseq   r0, r2, r0, ror #2
00474984: andeq    r3, r0, ip, ror #27

# _ZN12AssetManager13loadSceneNodeEPKcS1_bi
0050a504: ldr      r0, [pc, #0x2c]
0050a508: ldr      r3, [pc, #0x2c]
0050a50c: str      r4, [sp, #-4]!
0050a510: add      r0, pc, r0
0050a514: ldr      r4, [r0, r3]
0050a518: subs     ip, r2, #0
0050a51c: movne    ip, #1
0050a520: mov      r3, #0
0050a524: ldr      r0, [r4, #0x10]
0050a528: ldr      r0, [r0, #0x1c]
0050a52c: str      ip, [sp, #4]
0050a530: ldm      sp!, {r4}
0050a534: b        #0x3596f8
0050a538: subeq    sl, r8, r0, lsl #11
0050a53c: strdeq   r3, r4, [r0], -r4

# _ZN14AnimController12SetCallbacksEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
00474d10: b        #0x474cac

# _ZN6Arrays19AnimatedEffectTable4readEP11IStreamBase
004bc504: push     {r4, r5, r6, r7, r8, sl, lr}
004bc508: sub      sp, sp, #0xc
004bc50c: mov      sl, r0
004bc510: bl       #0x313a90
004bc514: ldr      r6, [pc, #0x120]
004bc518: mov      r3, #1
004bc51c: cmp      r3, #0
004bc520: str      r0, [sp, #4]
004bc524: str      r3, [sp]
004bc528: add      r6, pc, r6
004bc52c: bne      #0x4bc574
004bc530: add      r3, sp, #4
004bc534: add      r2, r3, #2
004bc538: add      r3, r3, #1
004bc53c: ldrb     r0, [r2, #1]
004bc540: ldrb     r1, [r3, #-1]
004bc544: cmp      r2, r3
004bc548: eor      r1, r0, r1
004bc54c: strb     r1, [r3, #-1]
004bc550: ldrb     r0, [r2, #1]
004bc554: eor      r1, r1, r0
004bc558: strb     r1, [r2, #1]
004bc55c: ldrb     r0, [r3, #-1]
004bc560: sub      r2, r2, #1
004bc564: eor      r1, r1, r0
004bc568: strb     r1, [r3, #-1]
004bc56c: add      r3, r3, #1
004bc570: bhi      #0x4bc53c
004bc574: ldr      r7, [pc, #0xc4]
004bc578: bl       #0x4a8944
004bc57c: ldr      r4, [sp, #4]
004bc580: ldr      r3, [r6, r7]
004bc584: mov      r1, #1
004bc588: add      r0, r4, r4, lsl #1
004bc58c: add      r0, r0, r1
004bc590: str      r4, [r3]
004bc594: lsl      r0, r0, #3
004bc598: bl       #0x31056c
004bc59c: mov      r3, #0x18
004bc5a0: cmp      r4, #0
004bc5a4: stm      r0, {r3, r4}
004bc5a8: add      r3, r0, #8
004bc5ac: beq      #0x4bc5d8
004bc5b0: ldr      r1, [pc, #0x8c]
004bc5b4: mov      r2, #0
004bc5b8: mov      ip, r2
004bc5bc: ldr      r1, [r6, r1]
004bc5c0: add      r1, r1, #8
004bc5c4: add      r2, r2, #1
004bc5c8: cmp      r2, r4
004bc5cc: str      r1, [r0, #8]
004bc5d0: str      ip, [r0, #0x18]!
004bc5d4: bne      #0x4bc5c4
004bc5d8: ldr      r2, [r6, r7]
004bc5dc: ldr      r8, [pc, #0x64]
004bc5e0: ldr      r1, [r2]
004bc5e4: ldr      r2, [r6, r8]
004bc5e8: cmp      r1, #0
004bc5ec: str      r3, [r2]
004bc5f0: beq      #0x4bc634
004bc5f4: mov      r4, #0
004bc5f8: mov      r5, r4
004bc5fc: b        #0x4bc608
004bc600: ldr      r3, [r6, r8]
004bc604: ldr      r3, [r3]
004bc608: add      r0, r3, r4
004bc60c: mov      r1, sl
004bc610: ldr      r3, [r3, r4]
004bc614: mov      lr, pc
004bc618: ldr      pc, [r3, #0xc]
004bc61c: ldr      r3, [r6, r7]
004bc620: add      r5, r5, #1
004bc624: add      r4, r4, #0x18
004bc628: ldr      r3, [r3]
004bc62c: cmp      r3, r5
004bc630: bhi      #0x4bc600
004bc634: add      sp, sp, #0xc
004bc638: pop      {r4, r5, r6, r7, r8, sl, pc}
004bc63c: subeq    r8, sp, r8, ror #10
004bc640: andeq    r0, r0, r4, asr #13
004bc644: strheq   r1, [r0], -ip
004bc648: andeq    r3, r0, r0, ror sb

# _ZN15VisualFXManager18_HandleEndOfLoopCBEP10AnimatedFXPNS_13AnimFXSetDataE
00496364: push     {r4, r5, r6}
00496368: ldr      r5, [r2, #8]
0049636c: mov      r4, #0x18
00496370: ldr      ip, [r0, #0x1c]
00496374: mul      r5, r4, r5
00496378: ldr      r5, [ip, r5]
0049637c: ldr      r5, [r5, #0x14]
00496380: cmp      r5, #1
00496384: beq      #0x4963c4
00496388: ldr      r3, [r2, #0x2c]
0049638c: cmp      r3, #0
00496390: beq      #0x4963b4
00496394: ldr      r5, [r3, #8]
00496398: mov      r6, #1
0049639c: strb     r6, [r3]
004963a0: mul      r4, r4, r5
004963a4: ldr      r3, [ip, r4]
004963a8: ldr      r3, [r3, #0x14]
004963ac: cmp      r3, r6
004963b0: beq      #0x4963bc
004963b4: pop      {r4, r5, r6}
004963b8: bx       lr
004963bc: pop      {r4, r5, r6}
004963c0: b        #0x496144
004963c4: mov      r3, #0
004963c8: pop      {r4, r5, r6}
004963cc: b        #0x496144

# _ZN13DebugSwitchesC1Ev
00335fcc: mov      r2, #0
00335fd0: mov      r1, r0
00335fd4: mov      r3, r0
00335fd8: str      r2, [r0, #4]
00335fdc: strb     r2, [r0]
00335fe0: str      r0, [r3, #8]
00335fe4: str      r0, [r3, #0xc]
00335fe8: str      r2, [r0, #0x10]
00335fec: str      r2, [r0, #0x1c]
00335ff0: strb     r2, [r1, #0x18]!
00335ff4: str      r1, [r0, #0x24]
00335ff8: str      r2, [r0, #0x28]
00335ffc: str      r1, [r0, #0x20]
00336000: bx       lr

# _ZN10AnimatedFX7_CBLoopEPN6glitch5scene19ITimelineControllerEPv
004928a4: mov      r0, r1
004928a8: b        #0x4927a4

# _ZN10AnimatedFX8SetSpeedEf
004924b0: push     {r4, lr}
004924b4: ldr      r3, [r0, #0x2c]
004924b8: str      r1, [r0, #0x20]
004924bc: cmp      r3, #0
004924c0: beq      #0x4924dc
004924c4: ldr      r3, [r3, #0x38]
004924c8: mov      r2, #0
004924cc: mov      r0, r3
004924d0: ldr      r3, [r3]
004924d4: mov      lr, pc
004924d8: ldr      pc, [r3, #0x28]
004924dc: pop      {r4, pc}

# _ZN10AnimatedFX6UpdateEv
00492f68: push     {r4, r5, r6, r7, r8, sl, lr}
00492f6c: ldr      r5, [pc, #0x1a4]
00492f70: ldr      r6, [pc, #0x1a4]
00492f74: ldr      r3, [r0, #0x28]
00492f78: add      r5, pc, r5
00492f7c: ldr      r2, [r5, r6]
00492f80: sub      sp, sp, #0x44
00492f84: cmp      r3, #0
00492f88: ldr      r2, [r2]
00492f8c: mov      r4, r0
00492f90: str      r2, [sp, #0x3c]
00492f94: beq      #0x492fb8
00492f98: mov      r0, r3
00492f9c: ldr      r3, [r3]
00492fa0: mov      lr, pc
00492fa4: ldr      pc, [r3, #0x34]
00492fa8: cmp      r0, #0
00492fac: beq      #0x4930d4
00492fb0: mov      r3, #0
00492fb4: str      r3, [r4, #0x28]
00492fb8: ldr      r7, [r4, #0x1c]
00492fbc: cmp      r7, #0
00492fc0: blt      #0x492fe4
00492fc4: ldr      r3, [pc, #0x154]
00492fc8: ldr      r0, [r5, r3]
00492fcc: bl       #0x31f66c
00492fd0: rsb      r0, r0, r7
00492fd4: cmp      r0, #0
00492fd8: movle    r3, #0
00492fdc: str      r0, [r4, #0x1c]
00492fe0: strle    r3, [r4, #0x14]
00492fe4: ldr      r3, [r4, #0x28]
00492fe8: cmp      r3, #0
00492fec: beq      #0x492ffc
00492ff0: ldrb     r1, [r3, #0x84]
00492ff4: cmp      r1, #0
00492ff8: beq      #0x4930c8
00492ffc: ldrb     r3, [r4, #0x24]
00493000: cmp      r3, #0
00493004: beq      #0x493088
00493008: ldr      r3, [r4, #0x50]
0049300c: cmp      r3, #0
00493010: beq      #0x4930ec
00493014: ldr      r8, [pc, #0x108]
00493018: add      r7, sp, #0x24
0049301c: ldr      sl, [r5, r8]
00493020: mov      r0, sl
00493024: bl       #0x337888
00493028: ldr      r1, [pc, #0xf8]
0049302c: add      r2, sp, #8
00493030: mov      r0, r7
00493034: add      r1, pc, r1
00493038: bl       #0x3140ec
0049303c: mov      r0, sl
00493040: mov      r1, r7
00493044: bl       #0x337a88
00493048: mov      r0, r7
0049304c: bl       #0x3139ac
00493050: ldr      r8, [r5, r8]
00493054: add      r7, sp, #0xc
00493058: mov      r0, r8
0049305c: bl       #0x337888
00493060: ldr      r1, [pc, #0xc4]
00493064: add      r2, sp, #4
00493068: mov      r0, r7
0049306c: add      r1, pc, r1
00493070: bl       #0x3140ec
00493074: mov      r0, r8
00493078: mov      r1, r7
0049307c: bl       #0x337a88
00493080: mov      r0, r7
00493084: bl       #0x3139ac
00493088: mov      r0, r4
0049308c: ldr      r1, [r4, #0x20]
00493090: bl       #0x4924b0
00493094: ldrb     r7, [r4, #0x18]
00493098: cmp      r7, #0
0049309c: bne      #0x4930ac
004930a0: ldrb     r3, [r4, #0x24]
004930a4: cmp      r3, #0
004930a8: bne      #0x4930f4
004930ac: ldr      r3, [r5, r6]
004930b0: ldr      r2, [sp, #0x3c]
004930b4: ldr      r3, [r3]
004930b8: cmp      r2, r3
004930bc: bne      #0x493114
004930c0: add      sp, sp, #0x44
004930c4: pop      {r4, r5, r6, r7, r8, sl, pc}
004930c8: mov      r0, r4
004930cc: bl       #0x492aa0
004930d0: b        #0x492ffc
004930d4: ldr      r3, [r4, #0x28]
004930d8: ldrb     r3, [r3, #0x81]
004930dc: cmp      r3, #0
004930e0: movne    r3, #0
004930e4: strne    r3, [r4, #0x28]
004930e8: b        #0x492fb8
004930ec: ldr      r8, [pc, #0x30]
004930f0: b        #0x493050
004930f4: mov      r0, r4
004930f8: bl       #0x4924e0
004930fc: cmp      r0, #0
00493100: beq      #0x4930ac
00493104: mov      r0, r4
00493108: mov      r1, r7
0049310c: bl       #0x492ef0
00493110: b        #0x4930ac
00493114: bl       #0x30e310
00493118: subseq   r1, r0, r8, lsl fp
0049311c: andeq    r4, r0, ip, lsr #1
00493120: strdeq   r3, r4, [r0], -r4
00493124: andeq    r0, r0, r4, lsl #17
00493128: strdeq   r1, r2, [r4], #-0xfc
0049312c: subeq    r1, r4, r4, asr #31

# _ZN7Structs9AnimFXTpl4readEP11IStreamBase
004ed73c: push     {r4, r5, r6, r7, lr}
004ed740: mov      r5, r0
004ed744: sub      sp, sp, #0xc
004ed748: mov      r0, r1
004ed74c: mov      r7, r1
004ed750: add      r1, r5, #4
004ed754: bl       #0x4db89c
004ed758: ldr      r6, [pc, #0x210]
004ed75c: mov      r0, r7
004ed760: add      r1, r5, #8
004ed764: bl       #0x459090
004ed768: mov      r3, #1
004ed76c: cmp      r3, #0
004ed770: str      r3, [sp, #4]
004ed774: add      r6, pc, r6
004ed778: bne      #0x4ed7bc
004ed77c: add      r3, r5, #9
004ed780: add      r2, r5, #0xa
004ed784: ldrb     r0, [r2, #1]
004ed788: ldrb     r1, [r3, #-1]
004ed78c: cmp      r2, r3
004ed790: eor      r1, r0, r1
004ed794: strb     r1, [r3, #-1]
004ed798: ldrb     r0, [r2, #1]
004ed79c: eor      r1, r1, r0
004ed7a0: strb     r1, [r2, #1]
004ed7a4: ldrb     r0, [r3, #-1]
004ed7a8: sub      r2, r2, #1
004ed7ac: eor      r1, r1, r0
004ed7b0: strb     r1, [r3, #-1]
004ed7b4: add      r3, r3, #1
004ed7b8: bhi      #0x4ed784
004ed7bc: mov      r0, r7
004ed7c0: add      r1, r5, #0xc
004ed7c4: bl       #0x3df1a0
004ed7c8: mov      r3, #1
004ed7cc: cmp      r3, #0
004ed7d0: str      r3, [sp, #4]
004ed7d4: bne      #0x4ed818
004ed7d8: add      r3, r5, #0xd
004ed7dc: add      r2, r5, #0xe
004ed7e0: ldrb     r0, [r2, #1]
004ed7e4: ldrb     r1, [r3, #-1]
004ed7e8: cmp      r3, r2
004ed7ec: eor      r1, r0, r1
004ed7f0: strb     r1, [r3, #-1]
004ed7f4: ldrb     r0, [r2, #1]
004ed7f8: eor      r1, r1, r0
004ed7fc: strb     r1, [r2, #1]
004ed800: ldrb     r0, [r3, #-1]
004ed804: sub      r2, r2, #1
004ed808: eor      r1, r1, r0
004ed80c: strb     r1, [r3, #-1]
004ed810: add      r3, r3, #1
004ed814: blo      #0x4ed7e0
004ed818: ldr      r3, [r5, #0x10]
004ed81c: cmp      r3, #0
004ed820: beq      #0x4ed868
004ed824: ldr      r2, [r3, #-4]
004ed828: mov      r0, #0x30
004ed82c: mla      r0, r0, r2, r3
004ed830: cmp      r3, r0
004ed834: bne      #0x4ed840
004ed838: b        #0x4ed860
004ed83c: mov      r0, r4
004ed840: sub      r4, r0, #0x30
004ed844: ldr      r3, [r0, #-0x30]
004ed848: mov      r0, r4
004ed84c: mov      lr, pc
004ed850: ldr      pc, [r3]
004ed854: ldr      r0, [r5, #0x10]
004ed858: cmp      r0, r4
004ed85c: bne      #0x4ed83c
004ed860: sub      r0, r0, #8
004ed864: bl       #0x310440
004ed868: ldr      r4, [r5, #0xc]
004ed86c: mov      r0, #6
004ed870: mov      r1, #1
004ed874: mul      r0, r0, r4
004ed878: add      r0, r0, r1
004ed87c: lsl      r0, r0, #3
004ed880: bl       #0x31056c
004ed884: mov      r3, #0x30
004ed888: cmp      r4, #0
004ed88c: stm      r0, {r3, r4}
004ed890: add      r3, r0, #8
004ed894: beq      #0x4ed8c4
004ed898: ldr      r1, [pc, #0xd4]
004ed89c: mov      r2, #0
004ed8a0: mov      ip, r2
004ed8a4: ldr      r1, [r6, r1]
004ed8a8: add      r1, r1, #8
004ed8ac: add      r2, r2, #1
004ed8b0: cmp      r2, r4
004ed8b4: str      r1, [r0, #8]
004ed8b8: str      ip, [r0, #0x34]
004ed8bc: add      r0, r0, #0x30
004ed8c0: bne      #0x4ed8ac
004ed8c4: ldr      r2, [r5, #0xc]
004ed8c8: str      r3, [r5, #0x10]
004ed8cc: cmp      r2, #0
004ed8d0: beq      #0x4ed90c
004ed8d4: mov      r4, #0
004ed8d8: mov      r6, r4
004ed8dc: b        #0x4ed8e4
004ed8e0: ldr      r3, [r5, #0x10]
004ed8e4: add      r0, r3, r4
004ed8e8: mov      r1, r7
004ed8ec: ldr      r3, [r3, r4]
004ed8f0: mov      lr, pc
004ed8f4: ldr      pc, [r3, #0xc]
004ed8f8: ldr      r3, [r5, #0xc]
004ed8fc: add      r6, r6, #1
004ed900: add      r4, r4, #0x30
004ed904: cmp      r3, r6
004ed908: bhi      #0x4ed8e0
004ed90c: mov      r0, r7
004ed910: add      r1, r5, #0x14
004ed914: bl       #0x459090
004ed918: mov      r3, #1
004ed91c: cmp      r3, #0
004ed920: str      r3, [sp, #4]
004ed924: bne      #0x4ed968
004ed928: add      r3, r5, #0x16
004ed92c: add      r5, r5, #0x15
004ed930: ldrb     r1, [r3, #1]
004ed934: ldrb     r2, [r5, #-1]
004ed938: cmp      r3, r5
004ed93c: eor      r2, r1, r2
004ed940: strb     r2, [r5, #-1]
004ed944: ldrb     r1, [r3, #1]
004ed948: eor      r2, r2, r1
004ed94c: strb     r2, [r3, #1]
004ed950: ldrb     r1, [r5, #-1]
004ed954: sub      r3, r3, #1
004ed958: eor      r2, r2, r1
004ed95c: strb     r2, [r5, #-1]
004ed960: add      r5, r5, #1
004ed964: bhi      #0x4ed930
004ed968: add      sp, sp, #0xc
004ed96c: pop      {r4, r5, r6, r7, pc}
004ed970: subeq    r7, sl, ip, lsl r3
004ed974: andeq    r0, r0, ip, lsl r7

# _ZN10AnimatedFX14_HandleLoopEndEv
004927a4: push     {r4, r5, r6, lr}
004927a8: mov      r1, #0
004927ac: mov      r4, r0
004927b0: ldr      r0, [r0, #0x20]
004927b4: bl       #0x30e9ac
004927b8: cmp      r0, #0
004927bc: beq      #0x4927cc
004927c0: ldr      r3, [r4, #4]
004927c4: cmp      r3, #0
004927c8: beq      #0x49288c
004927cc: ldr      r1, [r4, #0x14]
004927d0: cmp      r1, #0
004927d4: blt      #0x49288c
004927d8: bne      #0x492890
004927dc: mov      r0, r4
004927e0: bl       #0x492694
004927e4: ldr      r3, [r4, #0x50]
004927e8: cmp      r3, #0
004927ec: movne    r2, #1
004927f0: strbne   r2, [r3]
004927f4: ldr      r3, [r4, #4]
004927f8: cmp      r3, #0
004927fc: beq      #0x49288c
00492800: ldr      r1, [r4, #0x50]
00492804: mov      r0, r4
00492808: blx      r3
0049280c: mov      r3, #0
00492810: str      r3, [r4, #4]
00492814: mov      r0, r4
00492818: bl       #0x49267c
0049281c: ldr      r3, [r0]
00492820: mov      lr, pc
00492824: ldr      pc, [r3, #0x44]
00492828: ldr      r3, [r4, #0x2c]
0049282c: mov      r0, r4
00492830: ldr      r6, [r3, #8]
00492834: bl       #0x49267c
00492838: ldr      r3, [r0]
0049283c: mov      lr, pc
00492840: ldr      pc, [r3, #0x44]
00492844: ldr      r5, [r0, #0x14]
00492848: mov      r0, r4
0049284c: bl       #0x49267c
00492850: ldr      r3, [r0]
00492854: mov      lr, pc
00492858: ldr      pc, [r3, #0x44]
0049285c: ldr      r3, [r0, #4]
00492860: cmp      r5, r3
00492864: beq      #0x492870
00492868: mov      r0, r4
0049286c: bl       #0x4926e4
00492870: mov      r0, r4
00492874: bl       #0x49267c
00492878: mov      r1, r6
0049287c: mov      r2, r5
00492880: ldr      r3, [r0]
00492884: mov      lr, pc
00492888: ldr      pc, [r3, #0x10]
0049288c: pop      {r4, r5, r6, pc}
00492890: sub      r1, r1, #1
00492894: mov      r0, r4
00492898: str      r1, [r4, #0x14]
0049289c: pop      {r4, r5, r6, lr}
004928a0: b        #0x492744

# _ZN10AnimatedFXC1Ei
00492374: push     {r4, r5}
00492378: ldr      r4, [pc, #0x80]
0049237c: ldr      r2, [pc, #0x80]
00492380: str      r1, [r0, #8]
00492384: add      r4, pc, r4
00492388: ldr      r2, [r4, r2]
0049238c: mvn      r1, #0
00492390: str      r1, [r0, #0x1c]
00492394: mov      r1, #0x3f800000
00492398: mov      ip, #0
0049239c: add      r5, r2, #8
004923a0: str      r1, [r0, #0x20]
004923a4: mov      r2, #0
004923a8: mov      r1, #1
004923ac: str      r2, [r0, #0x50]
004923b0: str      r5, [r0]
004923b4: strb     r1, [r0, #0x24]
004923b8: str      ip, [r0, #0x48]
004923bc: str      r2, [r0, #0xc]
004923c0: str      r2, [r0, #0x10]
004923c4: str      r2, [r0, #0x14]
004923c8: strb     r2, [r0, #0x18]
004923cc: str      r2, [r0, #0x28]
004923d0: str      r2, [r0, #0x2c]
004923d4: strb     r2, [r0, #0x30]
004923d8: strb     r2, [r0, #0x31]
004923dc: strb     r2, [r0, #0x32]
004923e0: str      ip, [r0, #0x34]
004923e4: str      ip, [r0, #0x38]
004923e8: str      ip, [r0, #0x3c]
004923ec: str      ip, [r0, #0x40]
004923f0: str      ip, [r0, #0x44]
004923f4: strb     r2, [r0, #0x4c]
004923f8: pop      {r4, r5}
004923fc: bx       lr
00492400: subseq   r2, r0, ip, lsl #14
00492404: andeq    r3, r0, ip, lsl r1

# _ZN10AnimatedFX11GetAnimatorEv
0049267c: ldr      r0, [r0, #0x2c]
00492680: cmp      r0, #0
00492684: bxeq     lr
00492688: ldr      r0, [r0, #0x38]
0049268c: mov      r1, #0
00492690: b        #0x4748b8

# _ZN10AnimatedFX6SetEndEv
004926e4: push     {r4, r5, r6, lr}
004926e8: ldr      r3, [r0, #0x2c]
004926ec: mov      r4, r0
004926f0: cmp      r3, #0
004926f4: beq      #0x492740
004926f8: bl       #0x49267c
004926fc: cmp      r0, #0
00492700: beq      #0x492740
00492704: mov      r0, r4
00492708: bl       #0x49267c
0049270c: ldr      r3, [r0]
00492710: mov      lr, pc
00492714: ldr      pc, [r3, #0x44]
00492718: ldr      r5, [r0, #0x14]
0049271c: mov      r0, r4
00492720: bl       #0x49267c
00492724: ldr      r3, [r0]
00492728: mov      lr, pc
0049272c: ldr      pc, [r3, #0x44]
00492730: mov      r1, r5
00492734: ldr      r3, [r0]
00492738: mov      lr, pc
0049273c: ldr      pc, [r3, #0xc]
00492740: pop      {r4, r5, r6, pc}

# _ZN15VisualFXManager6UpdateEv
00496594: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00496598: ldr      r6, [pc, #0x230]
0049659c: ldr      r8, [pc, #0x230]
004965a0: ldr      r2, [pc, #0x230]
004965a4: add      r6, pc, r6
004965a8: ldr      r3, [r6, r8]
004965ac: ldr      r7, [r6, r2]
004965b0: sub      sp, sp, #0x2c
004965b4: ldr      r3, [r3]
004965b8: mov      r5, r0
004965bc: mov      r0, r7
004965c0: str      r3, [sp, #0x24]
004965c4: bl       #0x337888
004965c8: ldr      r1, [pc, #0x20c]
004965cc: add      r4, sp, #0xc
004965d0: add      r2, sp, #8
004965d4: add      r1, pc, r1
004965d8: mov      r0, r4
004965dc: bl       #0x3140ec
004965e0: mov      r0, r7
004965e4: mov      r1, r4
004965e8: bl       #0x337ec8
004965ec: mov      r7, r0
004965f0: ldr      r0, [sp, #0x20]
004965f4: cmp      r0, r4
004965f8: beq      #0x496618
004965fc: cmp      r0, #0
00496600: beq      #0x496618
00496604: ldr      r1, [sp, #0xc]
00496608: rsb      r1, r0, r1
0049660c: cmp      r1, #0x80
00496610: bhi      #0x4967c4
00496614: bl       #0x708f00
00496618: cmp      r7, #0
0049661c: bne      #0x49663c
00496620: ldr      r3, [r6, r8]
00496624: ldr      r2, [sp, #0x24]
00496628: ldr      r3, [r3]
0049662c: cmp      r2, r3
00496630: bne      #0x4967cc
00496634: add      sp, sp, #0x2c
00496638: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0049663c: ldr      r0, [pc, #0x19c]
00496640: add      r0, pc, r0
00496644: bl       #0x3136b4
00496648: ldr      r7, [r5, #0x28]
0049664c: ldr      r3, [r5, #0x2c]
00496650: rsb      r3, r7, r3
00496654: asr      r3, r3, #3
00496658: add      r2, r3, r3, lsl #2
0049665c: add      r2, r2, r2, lsl #4
00496660: add      r2, r2, r2, lsl #8
00496664: add      r2, r2, r2, lsl #16
00496668: add      r3, r3, r2, lsl #1
0049666c: cmp      r3, #0
00496670: beq      #0x4966d0
00496674: mov      sl, #0
00496678: mov      sb, sl
0049667c: add      r7, r7, sl
00496680: ldr      r4, [r7, #0x10]!
00496684: b        #0x496694
00496688: ldr      r0, [r4, #8]
0049668c: bl       #0x492f68
00496690: ldr      r4, [r4]
00496694: cmp      r7, r4
00496698: bne      #0x496688
0049669c: ldr      r7, [r5, #0x28]
004966a0: ldr      r3, [r5, #0x2c]
004966a4: add      sb, sb, #1
004966a8: add      sl, sl, #0x18
004966ac: rsb      r3, r7, r3
004966b0: asr      r3, r3, #3
004966b4: add      r2, r3, r3, lsl #2
004966b8: add      r2, r2, r2, lsl #4
004966bc: add      r2, r2, r2, lsl #8
004966c0: add      r2, r2, r2, lsl #16
004966c4: add      r3, r3, r2, lsl #1
004966c8: cmp      sb, r3
004966cc: blo      #0x49667c
004966d0: mov      r7, r5
004966d4: ldr      r4, [r7, #8]!
004966d8: cmp      r4, r7
004966dc: addne    sl, sp, #4
004966e0: beq      #0x4967b4
004966e4: cmp      r7, r4
004966e8: beq      #0x4967b4
004966ec: ldr      r3, [r4, #8]
004966f0: mov      r0, r3
004966f4: str      r3, [sp, #4]
004966f8: bl       #0x49267c
004966fc: ldr      r3, [r0]
00496700: mov      lr, pc
00496704: ldr      pc, [r3, #0x44]
00496708: ldr      r0, [sp, #4]
0049670c: ldr      r3, [r0, #0x2c]
00496710: ldr      fp, [r3, #8]
00496714: bl       #0x49267c
00496718: ldr      r3, [r0]
0049671c: mov      lr, pc
00496720: ldr      pc, [r3, #0x44]
00496724: ldr      sb, [r0, #0x14]
00496728: ldr      r0, [sp, #4]
0049672c: bl       #0x49267c
00496730: ldr      r3, [r0]
00496734: mov      lr, pc
00496738: ldr      pc, [r3, #0x44]
0049673c: ldr      r3, [r0, #4]
00496740: cmp      sb, r3
00496744: beq      #0x496750
00496748: ldr      r0, [sp, #4]
0049674c: bl       #0x4926e4
00496750: ldr      r0, [sp, #4]
00496754: bl       #0x49267c
00496758: mov      r2, sb
0049675c: ldr      r3, [r0]
00496760: mov      r1, fp
00496764: mov      lr, pc
00496768: ldr      pc, [r3, #0x10]
0049676c: ldr      r3, [sp, #4]
00496770: ldrb     r3, [r3, #0x24]
00496774: cmp      r3, #0
00496778: ldrne    sb, [r4]
0049677c: bne      #0x4967a8
00496780: ldr      sb, [r4]
00496784: ldr      r3, [r4, #4]
00496788: mov      r0, r4
0049678c: mov      r1, #0xc
00496790: str      sb, [r3]
00496794: str      r3, [sb, #4]
00496798: bl       #0x708f00
0049679c: mov      r0, r5
004967a0: mov      r1, sl
004967a4: bl       #0x494978
004967a8: mov      r4, sb
004967ac: cmp      r7, r4
004967b0: bne      #0x4966ec
004967b4: ldr      r0, [pc, #0x28]
004967b8: add      r0, pc, r0
004967bc: bl       #0x3136b8
004967c0: b        #0x496620
004967c4: bl       #0x310440
004967c8: b        #0x496618
004967cc: bl       #0x30e310
004967d0: subeq    lr, pc, ip, ror #9
004967d4: andeq    r4, r0, ip, lsr #1
004967d8: andeq    r0, r0, r4, lsl #17
004967dc: subeq    lr, r3, r4, ror sl
004967e0: subeq    lr, r3, r0, lsr sl
004967e4: strheq   lr, [r3], #-0x88

# _ZN15VisualFXManager16_BuildAnimFXDictEv
00495364: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00495368: ldr      sl, [pc, #0xb4]
0049536c: ldr      sb, [pc, #0xb4]
00495370: sub      sp, sp, #0x24
00495374: add      sl, pc, sl
00495378: ldr      r3, [sl, sb]
0049537c: ldr      r3, [r3]
00495380: cmp      r3, #0
00495384: beq      #0x49541c
00495388: ldr      r2, [pc, #0x9c]
0049538c: mov      r6, #0
00495390: add      r7, sp, #8
00495394: str      r2, [sp, #4]
00495398: add      fp, r0, #0x28
0049539c: mov      r4, r6
004953a0: mov      r5, r6
004953a4: add      r8, r7, #0x10
004953a8: b        #0x4953f0
004953ac: ldr      r2, [sp, #4]
004953b0: ldr      r3, [sl, r2]
004953b4: ldr      r3, [r3]
004953b8: add      r3, r3, r6
004953bc: ldr      r3, [r3, #8]
004953c0: str      r3, [sp, #8]
004953c4: mov      r1, r7
004953c8: mov      r0, fp
004953cc: bl       #0x495240
004953d0: mov      r0, r7
004953d4: bl       #0x493f88
004953d8: ldr      r3, [sl, sb]
004953dc: add      r4, r4, #1
004953e0: add      r6, r6, #0xc
004953e4: ldr      r3, [r3]
004953e8: cmp      r3, r4
004953ec: bls      #0x49541c
004953f0: cmp      r4, #0
004953f4: str      r5, [sp, #0xc]
004953f8: str      r5, [sp, #0x10]
004953fc: str      r5, [sp, #0x14]
00495400: str      r8, [sp, #0x18]
00495404: str      r8, [sp, #0x1c]
00495408: blt      #0x495414
0049540c: cmp      r3, r4
00495410: bgt      #0x4953ac
00495414: str      r5, [sp, #8]
00495418: b        #0x4953c4
0049541c: add      sp, sp, #0x24
00495420: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00495424: subeq    pc, pc, ip, lsl r7
00495428: andeq    r0, r0, r8, lsl #23
0049542c: strheq   r1, [r0], -r8

# _ZN15VisualFXManagerD1Ev
004950a4: ldr      r3, [pc, #0x7c]
004950a8: ldr      r2, [pc, #0x7c]
004950ac: push     {r4, r5, r6, lr}
004950b0: add      r3, pc, r3
004950b4: ldr      r2, [r3, r2]
004950b8: mov      r5, r0
004950bc: mov      r4, r0
004950c0: add      r2, r2, #8
004950c4: str      r2, [r5], #0x28
004950c8: bl       #0x4950a0
004950cc: mov      r0, r5
004950d0: bl       #0x494058
004950d4: add      r0, r4, #0x1c
004950d8: bl       #0x4942fc
004950dc: ldr      r0, [r4, #0x10]
004950e0: add      r3, r4, #0x10
004950e4: cmp      r0, #0
004950e8: beq      #0x495104
004950ec: ldr      r1, [r3, #8]
004950f0: rsb      r1, r0, r1
004950f4: bic      r1, r1, #3
004950f8: cmp      r1, #0x80
004950fc: bhi      #0x495114
00495100: bl       #0x708f00
00495104: add      r0, r4, #8
00495108: bl       #0x493c04
0049510c: mov      r0, r4
00495110: pop      {r4, r5, r6, pc}
00495114: bl       #0x310440
00495118: add      r0, r4, #8
0049511c: bl       #0x493c04
00495120: mov      r0, r4
00495124: pop      {r4, r5, r6, pc}
00495128: subeq    pc, pc, r0, ror #19
0049512c: andeq    r3, r0, r0, lsr r4

# _ZN13DebugSwitches9SetModuleERKSsb
00337404: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00337408: ldr      r4, [pc, #0xc4]
0033740c: ldr      r6, [pc, #0xc4]
00337410: add      r7, r0, #0x18
00337414: add      r4, pc, r4
00337418: ldr      r3, [r4, r6]
0033741c: sub      sp, sp, #0x20
00337420: mov      r8, r0
00337424: ldr      r3, [r3]
00337428: mov      r0, r7
0033742c: mov      sb, r2
00337430: mov      sl, r1
00337434: str      r3, [sp, #0x1c]
00337438: bl       #0x3369a8
0033743c: cmp      r0, r7
00337440: mov      r5, r0
00337444: beq      #0x33747c
00337448: ldrb     r3, [r0, #0x28]
0033744c: cmp      r3, sb
00337450: beq      #0x337460
00337454: strb     sb, [r0, #0x28]
00337458: mov      r0, r8
0033745c: bl       #0x337d54
00337460: ldr      r3, [r4, r6]
00337464: ldr      r2, [sp, #0x1c]
00337468: ldr      r3, [r3]
0033746c: cmp      r2, r3
00337470: bne      #0x3374d0
00337474: add      sp, sp, #0x20
00337478: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0033747c: ldr      r3, [pc, #0x58]
00337480: add      r7, sp, #4
00337484: ldr      r8, [r4, r3]
00337488: mov      r0, r8
0033748c: bl       #0x337888
00337490: ldr      r1, [pc, #0x48]
00337494: mov      r2, sp
00337498: mov      r0, r7
0033749c: add      r1, pc, r1
003374a0: bl       #0x3140ec
003374a4: mov      r1, r7
003374a8: mov      r0, r8
003374ac: bl       #0x337a88
003374b0: mov      r0, r7
003374b4: bl       #0x318254
003374b8: mov      r0, r5
003374bc: mov      r1, sl
003374c0: bl       #0x337288
003374c4: mov      r3, #1
003374c8: strb     r3, [r0]
003374cc: b        #0x337460
003374d0: bl       #0x30e310
003374d4: rsbeq    sp, r5, ip, ror r6
003374d8: andeq    r4, r0, ip, lsr #1
003374dc: andeq    r0, r0, r4, lsl #17
003374e0: subseq   r8, r8, r4, ror #17

# _ZN10AnimatedFX4LoadEPKcS1_fib
0049296c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00492970: ldr      r5, [pc, #0x11c]
00492974: ldr      sl, [pc, #0x11c]
00492978: sub      sp, sp, #0x48
0049297c: add      r5, pc, r5
00492980: ldr      ip, [r5, sl]
00492984: mov      r4, r0
00492988: add      r6, sp, #0x2c
0049298c: ldr      ip, [ip]
00492990: str      r3, [r0, #0x20]
00492994: ldr      r3, [sp, #0x68]
00492998: mov      r8, r2
0049299c: str      r1, [r0, #0xc]
004929a0: str      r3, [r0, #0x14]
004929a4: add      r7, sp, #0x14
004929a8: str      r2, [r4, #0x10]
004929ac: mov      r0, r6
004929b0: add      r2, sp, #0x10
004929b4: str      ip, [sp, #0x44]
004929b8: ldrb     sb, [sp, #0x6c]
004929bc: bl       #0x3140ec
004929c0: mov      r1, r8
004929c4: add      r2, sp, #0xc
004929c8: mov      r0, r7
004929cc: bl       #0x3140ec
004929d0: mov      r1, #0
004929d4: mov      r0, #0xac
004929d8: bl       #0x310570
004929dc: mov      r3, r7
004929e0: mov      r1, #0
004929e4: mov      r2, r6
004929e8: mov      r8, r0
004929ec: bl       #0x472a0c
004929f0: mov      r0, r7
004929f4: str      r8, [r4, #0x2c]
004929f8: bl       #0x3139ac
004929fc: mov      r0, r6
00492a00: bl       #0x3139ac
00492a04: ldr      r3, [r4, #0x2c]
00492a08: cmp      r3, #0
00492a0c: beq      #0x492a64
00492a10: ldr      r3, [r3, #0x38]
00492a14: ldr      r2, [pc, #0x80]
00492a18: mov      r6, #0
00492a1c: ldr      ip, [r3]
00492a20: ldr      r1, [r5, r2]
00492a24: mov      r0, r3
00492a28: mov      r2, r4
00492a2c: mov      r3, r6
00492a30: str      r6, [sp]
00492a34: mov      lr, pc
00492a38: ldr      pc, [ip, #0x2c]
00492a3c: ldr      r3, [r4, #0x2c]
00492a40: mov      r2, r6
00492a44: ldr      r1, [r4, #0x20]
00492a48: ldr      r3, [r3, #0x38]
00492a4c: mov      r0, r3
00492a50: ldr      r3, [r3]
00492a54: mov      lr, pc
00492a58: ldr      pc, [r3, #0x28]
00492a5c: cmp      sb, r6
00492a60: bne      #0x492a80
00492a64: ldr      r3, [r5, sl]
00492a68: ldr      r2, [sp, #0x44]
00492a6c: ldr      r3, [r3]
00492a70: cmp      r2, r3
00492a74: bne      #0x492a90
00492a78: add      sp, sp, #0x48
00492a7c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00492a80: ldr      r3, [r4, #0x2c]
00492a84: ldr      r0, [r3, #8]
00492a88: bl       #0x50e398
00492a8c: b        #0x492a64
00492a90: bl       #0x30e310
00492a94: subseq   r2, r0, r4, lsl r1
00492a98: andeq    r4, r0, ip, lsr #1
00492a9c: andeq    r2, r0, ip, lsr sp

# _ZN10AnimatedFX11SyncIrrDataEb
00492aa0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00492aa4: ldr      r3, [r0, #0x2c]
00492aa8: ldr      r8, [pc, #0x3d0]
00492aac: sub      sp, sp, #0x74
00492ab0: cmp      r3, #0
00492ab4: mov      r4, r0
00492ab8: add      r8, pc, r8
00492abc: beq      #0x492c00
00492ac0: ldrb     r3, [r0, #0x30]
00492ac4: ldr      r7, [r0, #0x34]
00492ac8: ldr      r6, [r0, #0x38]
00492acc: cmp      r3, #0
00492ad0: ldr      r5, [r0, #0x3c]
00492ad4: movne    sb, #1
00492ad8: beq      #0x492c08
00492adc: ldr      r2, [r4, #0x50]
00492ae0: cmp      r2, #0
00492ae4: beq      #0x492af0
00492ae8: cmp      r1, #0
00492aec: bne      #0x492c18
00492af0: ldr      r0, [r4, #0x28]
00492af4: cmp      r0, #0
00492af8: beq      #0x492bc8
00492afc: bl       #0x3935dc
00492b00: mov      sl, r0
00492b04: ldr      r1, [sl]
00492b08: mov      r0, r7
00492b0c: bl       #0x30eba4
00492b10: ldr      r1, [sl, #4]
00492b14: mov      r7, r0
00492b18: mov      r0, r6
00492b1c: bl       #0x30eba4
00492b20: ldr      r1, [sl, #8]
00492b24: mov      r6, r0
00492b28: mov      r0, r5
00492b2c: bl       #0x30eba4
00492b30: cmp      sb, #0
00492b34: mov      r5, r0
00492b38: beq      #0x492bbc
00492b3c: ldr      r3, [r4, #0x28]
00492b40: ldr      r2, [r3, #0x2d8]
00492b44: cmp      r2, #0
00492b48: beq      #0x492e30
00492b4c: ldr      r3, [r2, #8]
00492b50: mov      r0, r3
00492b54: ldr      r3, [r3]
00492b58: mov      lr, pc
00492b5c: ldr      pc, [r3, #0x38]
00492b60: mov      r1, r0
00492b64: add      r0, sp, #0x64
00492b68: bl       #0x432bbc
00492b6c: movw     r1, #0xfa35
00492b70: ldr      r0, [sp, #0x68]
00492b74: movt     r1, #0x3c8e
00492b78: bl       #0x30ed6c
00492b7c: movw     r1, #0xfa35
00492b80: mov      fp, r0
00492b84: movt     r1, #0x3c8e
00492b88: ldr      r0, [sp, #0x6c]
00492b8c: bl       #0x30ed6c
00492b90: movw     r1, #0xfa35
00492b94: mov      sl, r0
00492b98: movt     r1, #0x3c8e
00492b9c: ldr      r0, [sp, #0x64]
00492ba0: bl       #0x30ed6c
00492ba4: str      fp, [r4, #0x44]
00492ba8: str      r0, [r4, #0x40]
00492bac: str      sl, [r4, #0x48]
00492bb0: ldr      r0, [r4, #0x2c]
00492bb4: add      r1, r4, #0x40
00492bb8: bl       #0x472874
00492bbc: ldrb     r3, [r4, #0x32]
00492bc0: cmp      r3, #0
00492bc4: bne      #0x492c50
00492bc8: ldrb     r3, [r4, #0x4c]
00492bcc: cmp      r3, #0
00492bd0: bne      #0x492cd4
00492bd4: ldr      ip, [r4, #0x28]
00492bd8: cmp      ip, #0
00492bdc: beq      #0x492cf0
00492be0: cmp      sb, #0
00492be4: beq      #0x492ce4
00492be8: ldr      r0, [r4, #0x2c]
00492bec: add      r1, sp, #0x1c
00492bf0: str      r7, [sp, #0x1c]
00492bf4: str      r6, [sp, #0x20]
00492bf8: str      r5, [sp, #0x24]
00492bfc: bl       #0x470c24
00492c00: add      sp, sp, #0x74
00492c04: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00492c08: cmp      r1, #0
00492c0c: moveq    sb, r1
00492c10: ldrbne   sb, [r0, #0x31]
00492c14: b        #0x492adc
00492c18: ldrb     r3, [r4, #0x31]
00492c1c: cmp      r3, #0
00492c20: beq      #0x492af0
00492c24: ldr      r3, [r2, #0x2c]
00492c28: cmp      r3, #0
00492c2c: beq      #0x492c40
00492c30: mov      r2, r3
00492c34: ldr      r3, [r2, #0x2c]
00492c38: cmp      r3, #0
00492c3c: bne      #0x492c30
00492c40: ldr      r3, [r2, #4]
00492c44: cmp      r3, #0
00492c48: movgt    sb, #0
00492c4c: b        #0x492af0
00492c50: ldr      r3, [pc, #0x22c]
00492c54: ldr      r2, [r4, #0x28]
00492c58: ldr      r3, [r8, r3]
00492c5c: mov      r0, r2
00492c60: ldr      ip, [r3, #8]
00492c64: ldr      r1, [r3]
00492c68: ldr      r3, [r3, #4]
00492c6c: str      ip, [sp, #0x60]
00492c70: str      r1, [sp, #0x58]
00492c74: str      r3, [sp, #0x5c]
00492c78: ldr      r3, [r2]
00492c7c: mov      lr, pc
00492c80: ldr      pc, [r3, #0xa4]
00492c84: cmn      r0, #1
00492c88: beq      #0x492e10
00492c8c: ldr      r3, [r4, #0x28]
00492c90: add      r0, sp, #0x4c
00492c94: mov      r1, r3
00492c98: ldr      r3, [r3]
00492c9c: mov      lr, pc
00492ca0: ldr      pc, [r3, #0xa0]
00492ca4: ldr      r3, [sp, #0x4c]
00492ca8: str      r3, [sp, #0x58]
00492cac: ldr      r3, [sp, #0x50]
00492cb0: str      r3, [sp, #0x5c]
00492cb4: ldr      r3, [sp, #0x54]
00492cb8: str      r3, [sp, #0x60]
00492cbc: ldr      r0, [r4, #0x2c]
00492cc0: add      r1, sp, #0x58
00492cc4: bl       #0x4727ac
00492cc8: ldrb     r3, [r4, #0x4c]
00492ccc: cmp      r3, #0
00492cd0: beq      #0x492bd4
00492cd4: ldr      r0, [r4, #0x2c]
00492cd8: add      r1, r4, #0x40
00492cdc: bl       #0x472874
00492ce0: b        #0x492be8
00492ce4: ldrb     r3, [r4, #0x31]
00492ce8: cmp      r3, #0
00492cec: bne      #0x492be8
00492cf0: mov      sl, #0
00492cf4: cmp      ip, #0
00492cf8: str      sl, [sp, #0x40]
00492cfc: str      sl, [sp, #0x44]
00492d00: str      sl, [sp, #0x48]
00492d04: beq      #0x492e4c
00492d08: ldr      r0, [ip, #0x1ec]
00492d0c: str      r0, [sp, #0x40]
00492d10: ldr      sb, [ip, #0x1f0]
00492d14: mov      r1, r0
00492d18: str      sb, [sp, #0x44]
00492d1c: ldr      fp, [ip, #0x1f4]
00492d20: str      fp, [sp, #0x48]
00492d24: bl       #0x30ed6c
00492d28: mov      r1, sb
00492d2c: mov      r3, r0
00492d30: mov      r0, sb
00492d34: str      r3, [sp, #0x14]
00492d38: bl       #0x30ed6c
00492d3c: ldr      r3, [sp, #0x14]
00492d40: mov      r1, r0
00492d44: mov      r0, r3
00492d48: bl       #0x30eba4
00492d4c: mov      r1, fp
00492d50: mov      sb, r0
00492d54: mov      r0, fp
00492d58: bl       #0x30ed6c
00492d5c: mov      r1, r0
00492d60: mov      r0, sb
00492d64: bl       #0x30eba4
00492d68: mov      r1, sl
00492d6c: bl       #0x30df8c
00492d70: cmp      r0, #0
00492d74: beq      #0x492be8
00492d78: ldr      r3, [pc, #0x108]
00492d7c: mov      ip, #0
00492d80: mov      r2, ip
00492d84: ldr      r0, [r8, r3]
00492d88: add      r1, sp, #0x28
00492d8c: add      r3, sp, #0x40
00492d90: str      ip, [sp]
00492d94: str      ip, [sp, #4]
00492d98: str      ip, [sp, #8]
00492d9c: str      r7, [sp, #0x28]
00492da0: str      r6, [sp, #0x2c]
00492da4: str      r5, [sp, #0x30]
00492da8: bl       #0x525508
00492dac: ldr      r0, [sp, #0x40]
00492db0: mov      r1, r0
00492db4: bl       #0x30ed6c
00492db8: mov      r8, r0
00492dbc: ldr      r0, [sp, #0x44]
00492dc0: mov      r1, r0
00492dc4: bl       #0x30ed6c
00492dc8: mov      r1, r0
00492dcc: mov      r0, r8
00492dd0: bl       #0x30eba4
00492dd4: mov      r8, r0
00492dd8: ldr      r0, [sp, #0x48]
00492ddc: mov      r1, r0
00492de0: bl       #0x30ed6c
00492de4: mov      r1, r0
00492de8: mov      r0, r8
00492dec: bl       #0x30eba4
00492df0: mov      r1, sl
00492df4: bl       #0x30df8c
00492df8: cmp      r0, #0
00492dfc: movne    r3, #0x3f800000
00492e00: strne    sl, [sp, #0x44]
00492e04: strne    sl, [sp, #0x40]
00492e08: strne    r3, [sp, #0x48]
00492e0c: b        #0x492be8
00492e10: ldr      r3, [r4, #0x28]
00492e14: ldr      r2, [r3, #0x120]
00492e18: str      r2, [sp, #0x58]
00492e1c: ldr      r2, [r3, #0x124]
00492e20: str      r2, [sp, #0x5c]
00492e24: ldr      r3, [r3, #0x128]
00492e28: str      r3, [sp, #0x60]
00492e2c: b        #0x492cbc
00492e30: ldr      r2, [r3, #0x16c]
00492e34: str      r2, [r4, #0x40]
00492e38: ldr      r2, [r3, #0x170]
00492e3c: str      r2, [r4, #0x44]
00492e40: ldr      r3, [r3, #0x174]
00492e44: str      r3, [r4, #0x48]
00492e48: b        #0x492bb0
00492e4c: ldr      r3, [pc, #0x34]
00492e50: mov      r2, ip
00492e54: add      r1, sp, #0x34
00492e58: ldr      r0, [r8, r3]
00492e5c: add      r3, sp, #0x40
00492e60: str      r7, [sp, #0x34]
00492e64: str      r6, [sp, #0x38]
00492e68: str      r5, [sp, #0x3c]
00492e6c: str      ip, [sp]
00492e70: str      ip, [sp, #4]
00492e74: str      ip, [sp, #8]
00492e78: bl       #0x525508
00492e7c: b        #0x492be8
00492e80: ldrsbeq  r1, [r0], #-0xf8
00492e84: andeq    r3, r0, ip, lsr #30
00492e88: andeq    r1, r0, r4, lsl #4

# _ZN13DebugSwitches9GetModuleERKSs
00337ec8: push     {r4, r5, r6, r7, r8, sl, lr}
00337ecc: ldr      r4, [pc, #0xac]
00337ed0: ldr      r5, [pc, #0xac]
00337ed4: add      r8, r0, #0x18
00337ed8: add      r4, pc, r4
00337edc: ldr      r3, [r4, r5]
00337ee0: sub      sp, sp, #0x24
00337ee4: mov      r0, r8
00337ee8: ldr      r3, [r3]
00337eec: mov      r7, r1
00337ef0: str      r3, [sp, #0x1c]
00337ef4: bl       #0x3369a8
00337ef8: cmp      r0, r8
00337efc: mov      r6, r0
00337f00: ldrbne   r0, [r0, #0x28]
00337f04: beq      #0x337f24
00337f08: ldr      r3, [r4, r5]
00337f0c: ldr      r2, [sp, #0x1c]
00337f10: ldr      r3, [r3]
00337f14: cmp      r2, r3
00337f18: bne      #0x337f7c
00337f1c: add      sp, sp, #0x24
00337f20: pop      {r4, r5, r6, r7, r8, sl, pc}
00337f24: ldr      r3, [pc, #0x5c]
00337f28: add      r8, sp, #4
00337f2c: ldr      sl, [r4, r3]
00337f30: mov      r0, sl
00337f34: bl       #0x337888
00337f38: ldr      r1, [pc, #0x4c]
00337f3c: mov      r2, sp
00337f40: mov      r0, r8
00337f44: add      r1, pc, r1
00337f48: bl       #0x3140ec
00337f4c: mov      r1, r8
00337f50: mov      r0, sl
00337f54: bl       #0x337a88
00337f58: mov      r0, r8
00337f5c: bl       #0x318254
00337f60: mov      r0, r6
00337f64: mov      r1, r7
00337f68: bl       #0x337288
00337f6c: mov      r3, #1
00337f70: strb     r3, [r0]
00337f74: mov      r0, r3
00337f78: b        #0x337f08
00337f7c: bl       #0x30e310
00337f80: strhteq  ip, [r5], #-0xb8
00337f84: andeq    r4, r0, ip, lsr #1
00337f88: andeq    r0, r0, r4, lsl #17
00337f8c: subseq   r7, r8, ip, lsr lr

# _ZN10AnimatedFX9SetAnimFXEbbbfiPN15VisualFXManager13AnimFXSetDataEPFvPS_S2_E
00492e8c: push     {r4, r5, r6, lr}
00492e90: mov      r4, r0
00492e94: mov      r5, r2
00492e98: strb     r1, [r0, #0x30]
00492e9c: mov      r1, #0
00492ea0: mov      r6, r3
00492ea4: bl       #0x492aa0
00492ea8: mov      r0, r4
00492eac: mov      r1, #1
00492eb0: strb     r5, [r4, #0x31]
00492eb4: bl       #0x492aa0
00492eb8: mov      r0, r4
00492ebc: mov      r1, #0
00492ec0: strb     r6, [r4, #0x32]
00492ec4: bl       #0x492aa0
00492ec8: mov      r0, r4
00492ecc: ldr      r1, [sp, #0x10]
00492ed0: bl       #0x4924b0
00492ed4: ldr      r3, [sp, #0x18]
00492ed8: str      r3, [r4, #0x50]
00492edc: ldr      r3, [sp, #0x14]
00492ee0: str      r3, [r4, #0x14]
00492ee4: ldr      r3, [sp, #0x1c]
00492ee8: str      r3, [r4, #4]
00492eec: pop      {r4, r5, r6, pc}

# _ZN12SceneManager9LoadSceneEPKcS1_bb
003596f8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003596fc: ldr      r4, [pc, #0x308]
00359700: ldr      r7, [pc, #0x308]
00359704: subs     r5, r2, #0
00359708: add      r4, pc, r4
0035970c: ldr      r2, [r4, r7]
00359710: sub      sp, sp, #0x3c
00359714: str      r0, [sp, #8]
00359718: ldr      r2, [r2]
0035971c: mov      r6, r1
00359720: mov      fp, r3
00359724: ldrb     sb, [sp, #0x60]
00359728: str      r2, [sp, #0x34]
0035972c: beq      #0x35973c
00359730: ldrsb    r3, [r5]
00359734: cmp      r3, #0
00359738: bne      #0x359990
0035973c: ldr      r3, [pc, #0x2d0]
00359740: ldr      r8, [pc, #0x2d0]
00359744: mov      r1, r6
00359748: ldr      r0, [r4, r3]
0035974c: mov      r2, #1
00359750: ldr      r3, [r4, r8]
00359754: ldr      r0, [r0, #0x10]
00359758: ldr      r0, [r0, #0x10]
0035975c: bl       #0x61bbd4
00359760: mov      sl, r0
00359764: cmp      sl, #0
00359768: beq      #0x3597b0
0035976c: mov      r0, r6
00359770: bl       #0x30de54
00359774: mov      r1, r6
00359778: add      r2, r6, r0
0035977c: add      r0, sl, #0x1bc
00359780: bl       #0x3109e0
00359784: cmp      r5, #0
00359788: add      r3, sl, #0x1d4
0035978c: beq      #0x3599f8
00359790: mov      r0, r5
00359794: str      r3, [sp, #4]
00359798: bl       #0x30de54
0035979c: ldr      r3, [sp, #4]
003597a0: add      r2, r5, r0
003597a4: mov      r1, r5
003597a8: mov      r0, r3
003597ac: bl       #0x3109e0
003597b0: cmp      sb, #0
003597b4: beq      #0x3597c8
003597b8: cmp      sl, #0
003597bc: beq      #0x3597c8
003597c0: mov      r0, sl
003597c4: bl       #0x35cc0c
003597c8: subs     sb, r5, #0
003597cc: movne    sb, #1
003597d0: cmp      sl, #0
003597d4: cmpne    r5, #0
003597d8: bne      #0x359954
003597dc: cmp      sl, #0
003597e0: beq      #0x3598a0
003597e4: mov      r0, sl
003597e8: mov      r1, #2
003597ec: bl       #0x59719c
003597f0: cmp      sb, #0
003597f4: beq      #0x359898
003597f8: ldr      r8, [sl, #0xf4]
003597fc: cmp      r8, #0
00359800: subne    r8, r8, #4
00359804: ldr      r3, [r8]
00359808: mov      r0, r8
0035980c: mov      lr, pc
00359810: ldr      pc, [r3, #0x24]
00359814: ldr      r1, [pc, #0x200]
00359818: add      r1, pc, r1
0035981c: bl       #0x30ebd4
00359820: cmp      r0, #0
00359824: beq      #0x359898
00359828: ldr      r5, [r8, #0xf4]!
0035982c: cmp      r5, r8
00359830: beq      #0x359898
00359834: ldr      r3, [pc, #0x1e4]
00359838: ldr      sb, [pc, #0x1e4]
0035983c: add      r3, pc, r3
00359840: str      r3, [sp, #0xc]
00359844: ldr      r3, [pc, #0x1dc]
00359848: add      sb, pc, sb
0035984c: add      r3, pc, r3
00359850: str      r3, [sp, #0x10]
00359854: ldr      r3, [pc, #0x1d0]
00359858: add      r3, pc, r3
0035985c: str      r3, [sp, #0x14]
00359860: cmp      r5, #0
00359864: moveq    r6, r5
00359868: subne    r6, r5, #4
0035986c: ldr      r3, [r6]
00359870: mov      r0, r6
00359874: ldr      r5, [r5]
00359878: mov      lr, pc
0035987c: ldr      pc, [r3, #0x24]
00359880: mov      r1, sb
00359884: bl       #0x30ebd4
00359888: cmp      r0, #0
0035988c: beq      #0x3598e0
00359890: cmp      r8, r5
00359894: bne      #0x359860
00359898: cmp      fp, #0
0035989c: bne      #0x3598c0
003598a0: ldr      r3, [r4, r7]
003598a4: ldr      r2, [sp, #0x34]
003598a8: mov      r0, sl
003598ac: ldr      r3, [r3]
003598b0: cmp      r2, r3
003598b4: bne      #0x359a08
003598b8: add      sp, sp, #0x3c
003598bc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003598c0: ldr      r2, [sp, #8]
003598c4: mov      r1, sl
003598c8: ldr      r3, [r2, #4]
003598cc: mov      r0, r3
003598d0: ldr      r3, [r3]
003598d4: mov      lr, pc
003598d8: ldr      pc, [r3, #0x5c]
003598dc: b        #0x3598a0
003598e0: ldr      r3, [r6]
003598e4: mov      r0, r6
003598e8: mov      lr, pc
003598ec: ldr      pc, [r3, #0x24]
003598f0: ldr      r1, [sp, #0xc]
003598f4: bl       #0x30ebd4
003598f8: cmp      r0, #0
003598fc: bne      #0x359890
00359900: ldr      r3, [r6]
00359904: mov      r0, r6
00359908: mov      lr, pc
0035990c: ldr      pc, [r3, #0x24]
00359910: ldr      r1, [sp, #0x10]
00359914: bl       #0x30ebd4
00359918: cmp      r0, #0
0035991c: bne      #0x359890
00359920: ldr      r3, [r6]
00359924: mov      r0, r6
00359928: mov      lr, pc
0035992c: ldr      pc, [r3, #0x24]
00359930: ldr      r1, [sp, #0x14]
00359934: bl       #0x30ebd4
00359938: cmp      r0, #0
0035993c: bne      #0x359890
00359940: mov      r0, r6
00359944: ldr      r3, [r6]
00359948: mov      lr, pc
0035994c: ldr      pc, [r3, #0x68]
00359950: b        #0x359890
00359954: mov      r0, r6
00359958: ldr      r1, [r4, r8]
0035995c: bl       #0x61967c
00359960: subs     r5, r0, #0
00359964: beq      #0x3597dc
00359968: mov      r0, sl
0035996c: ldr      r3, [sl]
00359970: mov      r1, r5
00359974: mov      lr, pc
00359978: ldr      pc, [r3, #0x6c]
0035997c: ldr      r3, [r5]
00359980: ldr      r0, [r3, #-0xc]
00359984: add      r0, r5, r0
00359988: bl       #0x31d584
0035998c: b        #0x3597dc
00359990: add      ip, sp, #0x1c
00359994: mov      r1, r5
00359998: add      r2, sp, #0x18
0035999c: mov      r0, ip
003599a0: str      ip, [sp, #4]
003599a4: bl       #0x3140ec
003599a8: ldr      r1, [pc, #0x80]
003599ac: ldr      ip, [sp, #4]
003599b0: ldr      r8, [pc, #0x60]
003599b4: add      r1, pc, r1
003599b8: mov      r0, ip
003599bc: add      r2, r1, #5
003599c0: bl       #0x310804
003599c4: ldr      r3, [pc, #0x48]
003599c8: mov      r1, r6
003599cc: ldr      r2, [sp, #0x30]
003599d0: ldr      r0, [r4, r3]
003599d4: ldr      r3, [r4, r8]
003599d8: ldr      r0, [r0, #0x10]
003599dc: ldr      r0, [r0, #0x10]
003599e0: bl       #0x61c878
003599e4: ldr      ip, [sp, #4]
003599e8: mov      sl, r0
003599ec: mov      r0, ip
003599f0: bl       #0x318254
003599f4: b        #0x359764
003599f8: ldr      r2, [pc, #0x34]
003599fc: add      r2, pc, r2
00359a00: mov      r1, r2
00359a04: b        #0x3597a8
00359a08: bl       #0x30e310
00359a0c: rsbeq    fp, r3, r8, lsl #7
00359a10: andeq    r4, r0, ip, lsr #1
00359a14: strdeq   r3, r4, [r0], -r4
00359a18: andeq    r0, r0, ip, lsr #26
00359a1c: subseq   r7, r6, r0, ror #7
00359a20: ldrsbeq  r7, [r6], #-0x34
00359a24: ldrheq   r7, [r6], #-0x38
00359a28: subseq   r7, r6, ip, asr #7
00359a2c: subseq   r7, r6, r8, asr #7
00359a30: subseq   r7, r6, ip, lsr r2
00359a34: subseq   r1, r7, ip, lsl #28

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

# _ZN10AnimatedFXD1Ev
0049245c: push     {r4, lr}
00492460: ldr      r3, [pc, #0x40]
00492464: ldr      r2, [pc, #0x40]
00492468: ldr      r1, [r0, #0x2c]
0049246c: add      r3, pc, r3
00492470: ldr      r2, [r3, r2]
00492474: cmp      r1, #0
00492478: mov      r4, r0
0049247c: add      r2, r2, #8
00492480: str      r2, [r0]
00492484: beq      #0x4924a0
00492488: ldr      r3, [r1]
0049248c: mov      r0, r1
00492490: mov      lr, pc
00492494: ldr      pc, [r3, #4]
00492498: mov      r3, #0
0049249c: str      r3, [r4, #0x2c]
004924a0: mov      r0, r4
004924a4: pop      {r4, pc}
004924a8: subseq   r2, r0, r4, lsr #12
004924ac: andeq    r3, r0, ip, lsl r1

# _ZN12AssetManager15GetAssetManagerEv
0050a564: push     {r4, r5, r6, r7, r8, lr}
0050a568: ldr      r5, [pc, #0xac]
0050a56c: ldr      r4, [pc, #0xac]
0050a570: add      r5, pc, r5
0050a574: ldr      r6, [r5, #4]
0050a578: add      r4, pc, r4
0050a57c: ands     r6, r6, #1
0050a580: beq      #0x50a594
0050a584: ldr      r0, [pc, #0x98]
0050a588: add      r0, pc, r0
0050a58c: add      r0, r0, #8
0050a590: pop      {r4, r5, r6, r7, r8, pc}
0050a594: add      r8, r5, #4
0050a598: mov      r0, r8
0050a59c: bl       #0x30e76c
0050a5a0: cmp      r0, #0
0050a5a4: beq      #0x50a584
0050a5a8: mov      r7, r5
0050a5ac: strb     r6, [r7, #8]!
0050a5b0: add      r2, r7, #0x18
0050a5b4: add      r3, r7, #0x38
0050a5b8: str      r2, [r5, #0x2c]
0050a5bc: str      r3, [r5, #0x4c]
0050a5c0: mov      r0, r8
0050a5c4: str      r2, [r5, #0x28]
0050a5c8: str      r3, [r5, #0x48]
0050a5cc: str      r6, [r5, #0x50]
0050a5d0: str      r6, [r5, #0xc]
0050a5d4: str      r7, [r5, #0x10]
0050a5d8: str      r7, [r5, #0x14]
0050a5dc: str      r6, [r5, #0x18]
0050a5e0: str      r6, [r5, #0x24]
0050a5e4: strb     r6, [r5, #0x20]
0050a5e8: str      r6, [r5, #0x30]
0050a5ec: str      r6, [r5, #0x38]
0050a5f0: str      r6, [r5, #0x3c]
0050a5f4: str      r6, [r5, #0x44]
0050a5f8: strb     r6, [r5, #0x40]
0050a5fc: bl       #0x30ea3c
0050a600: ldr      r3, [pc, #0x20]
0050a604: mov      r0, r7
0050a608: ldr      r1, [r4, r3]
0050a60c: ldr      r3, [pc, #0x18]
0050a610: ldr      r2, [r4, r3]
0050a614: bl       #0x30e304
0050a618: b        #0x50a584
0050a61c: subeq    fp, lr, r8, asr #24
0050a620: subeq    sl, r8, r8, lsl r5
0050a624: subeq    fp, lr, r0, lsr ip
0050a628: strdeq   r0, r1, [r0], -r0
0050a62c: muleq    r0, r0, r8

# _ZN15VisualFXManager14BuildLibrariesEv
00496bd8: push     {r4, r5, r6, r7, r8, lr}
00496bdc: ldr      r4, [pc, #0xb0]
00496be0: ldr      r6, [pc, #0xb0]
00496be4: ldr      r2, [pc, #0xb0]
00496be8: add      r4, pc, r4
00496bec: ldr      r3, [r4, r6]
00496bf0: ldr      r7, [r4, r2]
00496bf4: sub      sp, sp, #0x20
00496bf8: ldr      r3, [r3]
00496bfc: mov      r8, r0
00496c00: mov      r0, r7
00496c04: str      r3, [sp, #0x1c]
00496c08: bl       #0x337888
00496c0c: ldr      r1, [pc, #0x8c]
00496c10: add      r5, sp, #4
00496c14: mov      r2, sp
00496c18: add      r1, pc, r1
00496c1c: mov      r0, r5
00496c20: bl       #0x3140ec
00496c24: mov      r0, r7
00496c28: mov      r1, r5
00496c2c: bl       #0x337ec8
00496c30: mov      r7, r0
00496c34: ldr      r0, [sp, #0x18]
00496c38: cmp      r0, r5
00496c3c: beq      #0x496c5c
00496c40: cmp      r0, #0
00496c44: beq      #0x496c5c
00496c48: ldr      r1, [sp, #4]
00496c4c: rsb      r1, r0, r1
00496c50: cmp      r1, #0x80
00496c54: bhi      #0x496c88
00496c58: bl       #0x708f00
00496c5c: cmp      r7, #0
00496c60: beq      #0x496c6c
00496c64: mov      r0, r8
00496c68: bl       #0x496ba4
00496c6c: ldr      r3, [r4, r6]
00496c70: ldr      r2, [sp, #0x1c]
00496c74: ldr      r3, [r3]
00496c78: cmp      r2, r3
00496c7c: bne      #0x496c90
00496c80: add      sp, sp, #0x20
00496c84: pop      {r4, r5, r6, r7, r8, pc}
00496c88: bl       #0x310440
00496c8c: b        #0x496c5c
00496c90: bl       #0x30e310
00496c94: subeq    sp, pc, r8, lsr #29
00496c98: andeq    r4, r0, ip, lsr #1
00496c9c: andeq    r0, r0, r4, lsl #17
00496ca0: subeq    lr, r3, r0, lsr r4

# _ZN15VisualFXManager16__Anim_EndOfLoopEP10AnimatedFXPNS_13AnimFXSetDataE
004963d0: push     {r4, r5, r6, lr}
004963d4: ldr      r4, [pc, #0x50]
004963d8: subs     r2, r1, #0
004963dc: mov      r5, r0
004963e0: add      r4, pc, r4
004963e4: beq      #0x496424
004963e8: ldr      r6, [pc, #0x40]
004963ec: mov      r1, r0
004963f0: ldr      r0, [r4, r6]
004963f4: bl       #0x496364
004963f8: ldr      r4, [r4, r6]
004963fc: add      r6, r4, #8
00496400: mov      r0, r6
00496404: bl       #0x494ab4
00496408: str      r5, [r0, #8]
0049640c: ldr      r3, [r4, #0xc]
00496410: str      r6, [r0]
00496414: str      r3, [r0, #4]
00496418: str      r0, [r3]
0049641c: str      r0, [r4, #0xc]
00496420: pop      {r4, r5, r6, pc}
00496424: ldr      r6, [pc, #4]
00496428: b        #0x4963f8
0049642c: strheq   lr, [pc], #-0x60
00496430: andeq    r1, r0, r8, lsl #22
