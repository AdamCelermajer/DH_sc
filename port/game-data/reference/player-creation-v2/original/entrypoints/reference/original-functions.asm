
# _ZN9Character11ResetSkillsEv
003bc860: push     {r4, r5, r6, r7, r8, lr}
003bc864: movw     r3, #0x14e8
003bc868: ldr      r3, [r0, r3]
003bc86c: ldr      r5, [pc, #0x15c]
003bc870: sub      sp, sp, #0x10
003bc874: cmp      r3, #0
003bc878: mov      r4, r0
003bc87c: add      r5, pc, r5
003bc880: beq      #0x3bc970
003bc884: ldr      r2, [r3, #0x80]
003bc888: cmp      r2, #0
003bc88c: beq      #0x3bc970
003bc890: ldr      r8, [r3, #0x84]
003bc894: cmp      r8, #0
003bc898: moveq    r7, r8
003bc89c: beq      #0x3bc91c
003bc8a0: mov      r6, #0
003bc8a4: mov      r7, r6
003bc8a8: b        #0x3bc8c4
003bc8ac: mov      r2, #1
003bc8b0: sub      r7, r7, #1
003bc8b4: bl       #0x3bbebc
003bc8b8: add      r6, r6, #1
003bc8bc: cmp      r6, r8
003bc8c0: beq      #0x3bc91c
003bc8c4: mov      r1, r6
003bc8c8: mov      r0, r4
003bc8cc: bl       #0x3bbed0
003bc8d0: mov      r1, r6
003bc8d4: add      r7, r7, r0
003bc8d8: mov      r2, #0
003bc8dc: mov      r0, r4
003bc8e0: bl       #0x3bbebc
003bc8e4: cmp      r6, #0
003bc8e8: mov      r1, r6
003bc8ec: mov      r0, r4
003bc8f0: beq      #0x3bc8ac
003bc8f4: bl       #0x3bbe84
003bc8f8: cmn      r0, #1
003bc8fc: mov      r1, r0
003bc900: mvn      r2, #0
003bc904: mov      r0, r4
003bc908: beq      #0x3bc8b8
003bc90c: add      r6, r6, #1
003bc910: bl       #0x3bbe54
003bc914: cmp      r6, r8
003bc918: bne      #0x3bc8c4
003bc91c: mov      r2, r7
003bc920: add      r0, r4, #0x560
003bc924: mov      r1, #0x9d
003bc928: bl       #0x3e0798
003bc92c: ldr      r3, [pc, #0xa0]
003bc930: add      r1, sp, #0x10
003bc934: mov      r2, #1
003bc938: ldr      r3, [r5, r3]
003bc93c: mov      r0, r4
003bc940: add      r5, r4, #0x3c8
003bc944: str      r3, [r1, #-4]!
003bc948: bl       #0x3a7b24
003bc94c: mov      r0, r5
003bc950: bl       #0x3cf1f0
003bc954: mov      r0, r5
003bc958: mov      r1, #1
003bc95c: bl       #0x3ce7c0
003bc960: mov      r0, r4
003bc964: bl       #0x3bc4a8
003bc968: add      sp, sp, #0x10
003bc96c: pop      {r4, r5, r6, r7, r8, pc}
003bc970: ldr      r2, [pc, #0x60]
003bc974: ldr      r2, [r5, r2]
003bc978: ldr      r2, [r2]
003bc97c: cmp      r2, #2
003bc980: moveq    r2, #0
003bc984: streq    r2, [r2]
003bc988: beq      #0x3bc890
003bc98c: cmp      r2, #1
003bc990: bne      #0x3bc890
003bc994: ldr      r0, [pc, #0x40]
003bc998: ldr      r1, [pc, #0x40]
003bc99c: ldr      r2, [pc, #0x40]
003bc9a0: ldr      r0, [r5, r0]
003bc9a4: ldr      r3, [pc, #0x3c]
003bc9a8: mov      ip, #0xc3
003bc9ac: add      r1, pc, r1
003bc9b0: add      r3, pc, r3
003bc9b4: add      r0, r0, #0xa8
003bc9b8: add      r2, pc, r2
003bc9bc: str      ip, [sp]
003bc9c0: bl       #0x30e004
003bc9c4: movw     r3, #0x14e8
003bc9c8: ldr      r3, [r4, r3]
003bc9cc: b        #0x3bc890
003bc9d0: subseq   r8, sp, r4, lsl r2
003bc9d4: andeq    r1, r0, r4, lsr r1
003bc9d8: andeq    r3, r0, r0, asr #19
003bc9dc: andeq    r1, r0, r0, asr #19
003bc9e0: subseq   r1, r0, ip, lsr #20
003bc9e4: subseq   r7, r0, r0, lsr #29
003bc9e8: subseq   r7, r0, r0, asr lr

# _ZN8MenuBase17FS_SetPlayerClassEPKcS1_Pv
0041f640: ldr      r3, [pc, #0x70]
0041f644: push     {r4, r5, r6, r7, r8, lr}
0041f648: mov      r4, r1
0041f64c: ldr      r1, [pc, #0x68]
0041f650: add      r3, pc, r3
0041f654: mov      r8, r2
0041f658: ldr      r1, [r3, r1]
0041f65c: ldr      r6, [r1]
0041f660: cmp      r6, #0
0041f664: beq      #0x41f6a8
0041f668: ldr      r2, [pc, #0x50]
0041f66c: mov      r5, #0
0041f670: ldr      r3, [r3, r2]
0041f674: ldr      r7, [r3]
0041f678: b        #0x41f688
0041f67c: add      r5, r5, #1
0041f680: cmp      r5, r6
0041f684: beq      #0x41f6a8
0041f688: ldr      r1, [r7, r5, lsl #2]
0041f68c: mov      r0, r4
0041f690: bl       #0x30e31c
0041f694: cmp      r0, #0
0041f698: bne      #0x41f67c
0041f69c: str      r5, [r8, #0x98]
0041f6a0: mov      r0, #1
0041f6a4: pop      {r4, r5, r6, r7, r8, pc}
0041f6a8: mvn      r5, #0
0041f6ac: str      r5, [r8, #0x98]
0041f6b0: mov      r0, #1
0041f6b4: pop      {r4, r5, r6, r7, r8, pc}
0041f6b8: subseq   r5, r7, r0, asr #8
0041f6bc: andeq    r4, r0, r4, lsl #4
0041f6c0: andeq    r3, r0, r8, lsl #24

# _ZN9Character17SG_SetPlayerLevelEi
003bb840: movw     r3, #0x14e8
003bb844: ldr      r3, [r0, r3]
003bb848: cmp      r3, #0
003bb84c: strne    r1, [r3, #0x30]
003bb850: bx       lr

# _ZN9Character16SG_SetPlayerNameEPKc
003bbf08: push     {r4, r5, r6, lr}
003bbf0c: movw     r3, #0x14e8
003bbf10: ldr      r4, [r0, r3]
003bbf14: mov      r5, r1
003bbf18: cmp      r4, #0
003bbf1c: beq      #0x3bbf3c
003bbf20: mov      r0, r1
003bbf24: bl       #0x30de54
003bbf28: mov      r1, r5
003bbf2c: add      r2, r5, r0
003bbf30: add      r0, r4, #0x18
003bbf34: pop      {r4, r5, r6, lr}
003bbf38: b        #0x3109e0
003bbf3c: pop      {r4, r5, r6, pc}

# _ZN8MenuBase12FS_StartGameEPKcS1_Pv
004220a0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
004220a4: ldr      r4, [pc, #0x1b8]
004220a8: ldr      r7, [pc, #0x1b8]
004220ac: sub      sp, sp, #0x1b8
004220b0: add      r4, pc, r4
004220b4: ldr      r3, [r4, r7]
004220b8: subs     r6, r1, #0
004220bc: mov      sl, r2
004220c0: ldr      r3, [r3]
004220c4: str      r3, [sp, #0x1b4]
004220c8: beq      #0x422248
004220cc: ldr      r1, [pc, #0x198]
004220d0: mov      r0, r6
004220d4: add      r1, pc, r1
004220d8: bl       #0x30e31c
004220dc: cmp      r0, #0
004220e0: bne      #0x422248
004220e4: ldr      r8, [pc, #0x184]
004220e8: ldr      r3, [r4, r8]
004220ec: ldr      r3, [r3, #0x4c]
004220f0: ldr      sb, [r3, #8]
004220f4: cmn      sb, #1
004220f8: moveq    sb, #0
004220fc: streq    sb, [r3, #8]
00422100: mov      r0, sb
00422104: bl       #0x464d5c
00422108: cmp      r0, #0
0042210c: bne      #0x422254
00422110: add      r5, sp, #0x1c
00422114: mov      r1, sb
00422118: mov      r2, #1
0042211c: mov      r3, #0
00422120: mov      r0, r5
00422124: bl       #0x4655ac
00422128: bl       #0x429b10
0042212c: ldr      sb, [sl, #0x94]
00422130: ldr      r3, [r0, #0x164]
00422134: ldr      sl, [pc, #0x138]
00422138: mov      r0, sb
0042213c: str      r3, [sp, #0x50]
00422140: bl       #0x30de54
00422144: mov      r1, sb
00422148: add      r2, sb, r0
0042214c: add      r0, r5, #0x18
00422150: bl       #0x3109e0
00422154: mov      r3, #1
00422158: mov      r0, r5
0042215c: str      r3, [sp, #0x4c]
00422160: bl       #0x467744
00422164: ldr      r2, [r4, sl]
00422168: add      ip, sp, #0x1b8
0042216c: mov      r3, #0x99000000
00422170: ldr      r2, [r2]
00422174: mov      r0, r5
00422178: add      r1, r2, #0x10
0042217c: add      r2, r2, #0x14
00422180: add      r2, ip, r2, lsl #2
00422184: add      r1, ip, r1, lsl #2
00422188: mov      ip, #0x29
0042218c: str      ip, [r2, r3, asr #22]
00422190: mov      r2, #0
00422194: str      r2, [r1, r3, asr #22]
00422198: bl       #0x464b2c
0042219c: cmp      r6, #0
004221a0: beq      #0x4221bc
004221a4: ldr      r1, [pc, #0xcc]
004221a8: mov      r0, r6
004221ac: add      r1, pc, r1
004221b0: bl       #0x30e31c
004221b4: cmp      r0, #0
004221b8: beq      #0x422220
004221bc: ldr      r3, [r4, sl]
004221c0: ldr      r2, [pc, #0xb4]
004221c4: add      ip, sp, #0x1b8
004221c8: ldr      r3, [r3]
004221cc: ldr      r2, [r4, r2]
004221d0: mov      lr, #1
004221d4: add      r1, ip, r3, lsl #2
004221d8: ldr      ip, [r1, #-0x14c]
004221dc: ldr      r1, [r2]
004221e0: add      r2, sp, #0x1b8
004221e4: add      r3, r2, r3, lsl #2
004221e8: ldr      r2, [r3, #-0x15c]
004221ec: mov      r3, #0x48
004221f0: mla      r3, r3, ip, r1
004221f4: ldr      r0, [r4, r8]
004221f8: ldr      r1, [r3, #0x20]
004221fc: mov      ip, #0
00422200: ldr      r3, [sp, #0x20]
00422204: str      lr, [sp, #4]
00422208: str      ip, [sp, #0x14]
0042220c: str      ip, [sp]
00422210: str      ip, [sp, #8]
00422214: str      ip, [sp, #0xc]
00422218: str      ip, [sp, #0x10]
0042221c: bl       #0x32bdc8
00422220: mov      r0, r5
00422224: bl       #0x46378c
00422228: ldr      r3, [r4, r7]
0042222c: ldr      r2, [sp, #0x1b4]
00422230: mov      r0, #1
00422234: ldr      r3, [r3]
00422238: cmp      r2, r3
0042223c: bne      #0x422260
00422240: add      sp, sp, #0x1b8
00422244: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00422248: ldr      r0, [sl, #4]
0042224c: bl       #0x7abb20
00422250: b        #0x4220e4
00422254: mov      r0, sb
00422258: bl       #0x464ca0
0042225c: b        #0x422110
00422260: bl       #0x30e310
00422264: subseq   r2, r7, r0, ror #19
00422268: andeq    r4, r0, ip, lsr #1
0042226c: umaaleq  ip, sb, r4, r4
00422270: strdeq   r3, r4, [r0], -r4
00422274: muleq    r0, ip, sl
00422278: strheq   ip, [sb], #-0x3c
0042227c: andeq    r0, r0, r4, ror r8

# _ZN9Character7InitAllEv
003b35f0: push     {r4, lr}
003b35f4: mov      r4, r0
003b35f8: ldr      r3, [r0]
003b35fc: mov      lr, pc
003b3600: ldr      pc, [r3, #0x1c]
003b3604: mov      r0, r4
003b3608: ldr      r3, [r4]
003b360c: mov      lr, pc
003b3610: ldr      pc, [r3, #0x58]
003b3614: pop      {r4, pc}

# _ZN9Character16_InitSkillsSlotsEv
003b3a90: push     {r4, r5, r6, lr}
003b3a94: mov      r5, r0
003b3a98: bl       #0x3bbea0
003b3a9c: subs     r4, r0, #0
003b3aa0: beq      #0x3b3aa8
003b3aa4: pop      {r4, r5, r6, pc}
003b3aa8: mov      r1, r4
003b3aac: mov      r2, r4
003b3ab0: mov      r0, r5
003b3ab4: add      r6, r5, #0x37c
003b3ab8: bl       #0x3bbe54
003b3abc: mov      r0, r6
003b3ac0: bl       #0x3fc6c8
003b3ac4: mov      r1, r4
003b3ac8: mov      r2, r4
003b3acc: mov      r0, r5
003b3ad0: bl       #0x3bbe54
003b3ad4: mov      r0, r6
003b3ad8: bl       #0x3fc6c8
003b3adc: mov      r1, r4
003b3ae0: mov      r0, r5
003b3ae4: bl       #0x3bbed0
003b3ae8: subs     r1, r0, #0
003b3aec: bne      #0x3b3aa4
003b3af0: mov      r0, r5
003b3af4: mov      r2, r1
003b3af8: pop      {r4, r5, r6, lr}
003b3afc: b        #0x3bcc58

# _ZN9Character24InitializePlayerSavegameEv
003b36b0: push     {r4, r5, r6, lr}
003b36b4: mov      r1, #0
003b36b8: mov      r4, r0
003b36bc: mov      r0, #0x198
003b36c0: bl       #0x310570
003b36c4: mov      r5, r0
003b36c8: bl       #0x465ae0
003b36cc: movw     r3, #0x14e8
003b36d0: mov      r0, r4
003b36d4: mov      r1, r4
003b36d8: str      r5, [r4, r3]
003b36dc: pop      {r4, r5, r6, lr}
003b36e0: b        #0x3bb754

# _Z15NativeStartGameRKN7gameswf7fn_callE
0043e0d0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0043e0d4: ldr      r4, [pc, #0x214]
0043e0d8: ldr      r5, [pc, #0x214]
0043e0dc: ldr      r6, [pc, #0x214]
0043e0e0: add      r4, pc, r4
0043e0e4: ldr      r3, [r4, r5]
0043e0e8: sub      sp, sp, #0x1c4
0043e0ec: mov      r8, r0
0043e0f0: ldr      r3, [r3]
0043e0f4: ldr      r0, [r4, r6]
0043e0f8: str      r3, [sp, #0x1bc]
0043e0fc: bl       #0x31f594
0043e100: cmp      r0, #0
0043e104: beq      #0x43e114
0043e108: ldr      r3, [r0, #0x130]
0043e10c: cmp      r3, #0x26
0043e110: beq      #0x43e220
0043e114: ldr      r3, [r4, r6]
0043e118: mov      r1, #0
0043e11c: mov      r2, r1
0043e120: ldr      r0, [r3, #0x40]
0043e124: bl       #0x36e478
0043e128: ldr      r7, [r0, #0x664]
0043e12c: cmp      r7, #0
0043e130: blt      #0x43e138
0043e134: bl       #0x4660c8
0043e138: add      sl, sp, #0x24
0043e13c: mov      r3, #0
0043e140: mov      r0, sl
0043e144: mov      r1, r7
0043e148: mov      r2, #1
0043e14c: bl       #0x4655ac
0043e150: ldr      r3, [r8, #0x10]
0043e154: cmp      r3, #1
0043e158: beq      #0x43e290
0043e15c: ldr      r8, [pc, #0x198]
0043e160: mov      fp, #0
0043e164: ldr      r8, [r4, r8]
0043e168: add      r0, sp, #0x1c0
0043e16c: ldr      r3, [r8]
0043e170: add      r3, r0, r3, lsl #2
0043e174: ldr      sb, [r3, #-0x14c]
0043e178: bl       #0x7fd794
0043e17c: ldrb     r3, [r0, #5]
0043e180: cmp      r3, #0
0043e184: bne      #0x43e23c
0043e188: ldr      r1, [r8]
0043e18c: add      r0, sp, #0x1c0
0043e190: cmn      sb, #1
0043e194: add      r3, r0, r1
0043e198: add      r1, r0, r1, lsl #2
0043e19c: ldr      r2, [r1, #-0x15c]
0043e1a0: ldrb     r1, [r3, #-0x150]
0043e1a4: str      r1, [sp, #0x1c]
0043e1a8: beq      #0x43e27c
0043e1ac: mov      r8, #0
0043e1b0: sub      r3, r3, #0x154
0043e1b4: strb     r8, [r3, #4]
0043e1b8: mov      r0, sl
0043e1bc: str      r2, [sp, #0x18]
0043e1c0: bl       #0x464b2c
0043e1c4: ldr      r3, [pc, #0x134]
0043e1c8: mov      ip, #0x48
0043e1cc: ldr      r0, [r4, r6]
0043e1d0: ldr      r3, [r4, r3]
0043e1d4: ldr      r2, [sp, #0x18]
0043e1d8: ldr      r1, [r3]
0043e1dc: mov      r3, r7
0043e1e0: mla      sb, ip, sb, r1
0043e1e4: mov      ip, #1
0043e1e8: ldr      r1, [sb, #0x20]
0043e1ec: str      ip, [sp]
0043e1f0: ldr      ip, [sp, #0x1c]
0043e1f4: str      fp, [sp, #8]
0043e1f8: str      r8, [sp, #0xc]
0043e1fc: str      ip, [sp, #4]
0043e200: str      r8, [sp, #0x10]
0043e204: str      r8, [sp, #0x14]
0043e208: bl       #0x32bdc8
0043e20c: ldrb     r3, [sp, #0x1b8]
0043e210: mov      r0, sl
0043e214: cmp      r3, r8
0043e218: strbne   r8, [sp, #0x1b8]
0043e21c: bl       #0x46378c
0043e220: ldr      r3, [r4, r5]
0043e224: ldr      r2, [sp, #0x1bc]
0043e228: ldr      r3, [r3]
0043e22c: cmp      r2, r3
0043e230: bne      #0x43e2ec
0043e234: add      sp, sp, #0x1c4
0043e238: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0043e23c: ldr      r3, [r4, r6]
0043e240: ldr      r0, [r3, #0x40]
0043e244: bl       #0x36f074
0043e248: ldr      r1, [r8]
0043e24c: cmp      r0, #0
0043e250: addne    r2, sp, #0x1c0
0043e254: addne    r3, r2, r1, lsl #2
0043e258: movne    r0, #0
0043e25c: ldrne    r2, [r3, #-0x15c]
0043e260: streq    r0, [sp, #0x1c]
0043e264: moveq    r2, #1
0043e268: strne    r0, [sp, #0x1c]
0043e26c: add      ip, sp, #0x1c0
0043e270: cmn      sb, #1
0043e274: add      r3, ip, r1
0043e278: bne      #0x43e1ac
0043e27c: ldr      r1, [pc, #0x80]
0043e280: ldr      r1, [r4, r1]
0043e284: ldr      r1, [r1]
0043e288: ldr      sb, [r1, #0x24]
0043e28c: b        #0x43e1ac
0043e290: ldr      r3, [r8, #0xc]
0043e294: ldr      r0, [r8, #0x14]
0043e298: mov      sb, #0xc
0043e29c: ldr      r3, [r3]
0043e2a0: mla      r0, sb, r0, r3
0043e2a4: bl       #0x439d8c
0043e2a8: cmp      r0, #0
0043e2ac: beq      #0x43e15c
0043e2b0: ldr      r3, [r8, #0xc]
0043e2b4: ldr      r0, [r8, #0x14]
0043e2b8: ldr      r8, [pc, #0x3c]
0043e2bc: ldr      r3, [r3]
0043e2c0: mla      r0, sb, r0, r3
0043e2c4: bl       #0x797a54
0043e2c8: bl       #0x30ea24
0043e2cc: ldr      r3, [sp, #0x60]
0043e2d0: mov      fp, r0
0043e2d4: cmp      r3, r0
0043e2d8: ldrge    r3, [r4, r8]
0043e2dc: strge    r0, [r3]
0043e2e0: mov      r0, sl
0043e2e4: bl       #0x464b2c
0043e2e8: b        #0x43e164
0043e2ec: bl       #0x30e310
0043e2f0: ldrheq   r6, [r5], #-0x90
0043e2f4: andeq    r4, r0, ip, lsr #1
0043e2f8: strdeq   r3, r4, [r0], -r4
0043e2fc: muleq    r0, ip, sl
0043e300: andeq    r0, r0, r4, ror r8
0043e304: andeq    r3, r0, r8, asr #5
