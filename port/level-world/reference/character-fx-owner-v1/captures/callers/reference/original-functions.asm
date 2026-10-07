
# _ZN6Random9GetRandomEib.clone.1
00493780: push     {r4, lr}
00493784: ldr      r4, [pc, #0x7c]
00493788: cmp      r0, #0
0049378c: add      r4, pc, r4
00493790: beq      #0x4937f0
00493794: ldr      r2, [pc, #0x70]
00493798: mov      r1, r0
0049379c: movw     r0, #0xe6ab
004937a0: ldr      r2, [r4, r2]
004937a4: movw     r3, #0xdb17
004937a8: movt     r3, #0x2b52
004937ac: ldr      lr, [r2]
004937b0: movw     ip, #0xf26b
004937b4: movt     ip, #0xda
004937b8: mul      r0, r0, lr
004937bc: add      r0, r0, #0x2b000
004937c0: add      r0, r0, #0x3fc
004937c4: add      r0, r0, #1
004937c8: umull    lr, r3, r3, r0
004937cc: rsb      lr, r3, r0
004937d0: add      r3, r3, lr, lsr #1
004937d4: lsr      r3, r3, #0x17
004937d8: mls      r3, ip, r3, r0
004937dc: mov      r0, r3
004937e0: str      r3, [r2]
004937e4: bl       #0x30eb2c
004937e8: eor      r0, r1, r1, asr #31
004937ec: sub      r0, r0, r1, asr #31
004937f0: ldr      r3, [pc, #0x18]
004937f4: ldr      r3, [r4, r3]
004937f8: ldr      r2, [r3]
004937fc: add      r2, r2, #1
00493800: str      r2, [r3]
00493804: pop      {r4, pc}
00493808: subseq   r1, r0, r4, lsl #6
0049380c: muleq    r0, r4, ip
00493810: andeq    r1, r0, r8, lsl #1

# _ZN9Character14DisableStateFXEv
003a40e4: movw     r3, #0x148c
003a40e8: ldr      r2, [r0, r3]
003a40ec: ldr      r3, [pc, #0x1c]
003a40f0: cmp      r2, #0
003a40f4: add      r3, pc, r3
003a40f8: bxeq     lr
003a40fc: ldr      r2, [pc, #0x10]
003a4100: add      r1, r0, #0x1480
003a4104: add      r1, r1, #0xc
003a4108: ldr      r0, [r3, r2]
003a410c: b        #0x494978

# _ZN10GameObject7_DropFXERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0038fc74: push     {r4, lr}
0038fc78: ldr      r2, [r0, #4]
0038fc7c: ldr      r4, [pc, #0x64]
0038fc80: sub      sp, sp, #8
0038fc84: ldm      r2, {r1, r3}
0038fc88: add      r4, pc, r4
0038fc8c: rsb      r3, r1, r3
0038fc90: asr      r3, r3, #4
0038fc94: add      r2, r3, r3, lsl #3
0038fc98: add      r2, r2, r2, lsl #6
0038fc9c: add      r2, r3, r2, lsl #3
0038fca0: add      r2, r2, r2, lsl #15
0038fca4: add      r3, r3, r2, lsl #3
0038fca8: cmp      r3, #0
0038fcac: bne      #0x38fcb8
0038fcb0: add      sp, sp, #8
0038fcb4: pop      {r4, pc}
0038fcb8: ldr      r3, [r1, #4]
0038fcbc: cmp      r3, #2
0038fcc0: bne      #0x38fcb0
0038fcc4: mov      r1, #0
0038fcc8: bl       #0x37baf8
0038fccc: bl       #0x31b580
0038fcd0: ldr      r3, [pc, #0x14]
0038fcd4: add      r1, sp, #8
0038fcd8: str      r0, [r1, #-4]!
0038fcdc: ldr      r0, [r4, r3]
0038fce0: bl       #0x494978
0038fce4: b        #0x38fcb0
0038fce8: rsbeq    r4, r0, r8, lsl #28
0038fcec: andeq    r1, r0, r8, lsl #22

# _ZN10GameObject7_GrabFXERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
00392620: push     {r4, r5, r6, r7, r8, lr}
00392624: ldr      r3, [r0, #4]
00392628: mov      r6, r1
0039262c: mov      r7, r2
00392630: ldr      r1, [r3, #4]
00392634: ldr      r3, [r3]
00392638: ldr      r4, [pc, #0xc4]
0039263c: mov      r5, r0
00392640: rsb      r1, r3, r1
00392644: asr      r1, r1, #4
00392648: add      r4, pc, r4
0039264c: add      r2, r1, r1, lsl #3
00392650: add      r2, r2, r2, lsl #6
00392654: add      r2, r1, r2, lsl #3
00392658: add      r2, r2, r2, lsl #15
0039265c: add      r1, r1, r2, lsl #3
00392660: cmp      r1, #0
00392664: bne      #0x39266c
00392668: pop      {r4, r5, r6, r7, r8, pc}
0039266c: ldr      r3, [r3, #4]
00392670: cmp      r3, #3
00392674: bne      #0x392668
00392678: mov      r1, #0
0039267c: bl       #0x37baf8
00392680: bl       #0x38d798
00392684: ldr      r3, [pc, #0x7c]
00392688: ldr      r3, [r4, r3]
0039268c: ldr      r3, [r3]
00392690: cmp      r0, r3
00392694: bhs      #0x392668
00392698: ldr      r5, [r5, #4]
0039269c: ldm      r5, {r0, r3}
003926a0: rsb      r3, r0, r3
003926a4: asr      r3, r3, #4
003926a8: add      r2, r3, r3, lsl #3
003926ac: add      r2, r2, r2, lsl #6
003926b0: add      r2, r3, r2, lsl #3
003926b4: add      r2, r2, r2, lsl #15
003926b8: add      r3, r3, r2, lsl #3
003926bc: cmp      r3, #0
003926c0: bne      #0x3926d4
003926c4: ldr      r0, [pc, #0x40]
003926c8: add      r0, pc, r0
003926cc: bl       #0x708eb0
003926d0: ldr      r0, [r5]
003926d4: bl       #0x31bbf0
003926d8: ldr      r3, [pc, #0x30]
003926dc: ldr      r4, [r4, r3]
003926e0: bl       #0x8be2a0
003926e4: mov      r2, r7
003926e8: mov      r1, r0
003926ec: mov      r0, r4
003926f0: bl       #0x495430
003926f4: mov      r1, r0
003926f8: mov      r0, r6
003926fc: pop      {r4, r5, r6, r7, r8, lr}
00392700: b        #0x38eb00
00392704: rsbeq    r2, r0, r8, asr #8
00392708: andeq    r0, r0, r4, asr #13
0039270c: subseq   fp, r2, r0, lsr #27
00392710: andeq    r1, r0, r8, lsl #22

# _ZN9Character13UpdateStateFXEv
003a4470: push     {r4, r5, r6, r7, r8, lr}
003a4474: movw     r2, #0x1490
003a4478: ldr      r3, [r0]
003a447c: mov      r4, r0
003a4480: ldr      r6, [r0, r2]
003a4484: mov      lr, pc
003a4488: ldr      pc, [r3, #0x34]
003a448c: ldr      r5, [pc, #0x1f8]
003a4490: cmp      r0, #0
003a4494: add      r5, pc, r5
003a4498: bne      #0x3a45a8
003a449c: cmp      r6, #4
003a44a0: ble      #0x3a45ac
003a44a4: mvn      r7, #0
003a44a8: mov      r6, #0
003a44ac: movw     r3, #0x1490
003a44b0: ldr      r2, [r4, r3]
003a44b4: cmp      r2, r6
003a44b8: beq      #0x3a45a8
003a44bc: cmn      r7, #1
003a44c0: str      r6, [r4, r3]
003a44c4: beq      #0x3a4674
003a44c8: movw     r3, #0x148c
003a44cc: ldr      r3, [r4, r3]
003a44d0: cmp      r3, #0
003a44d4: beq      #0x3a4654
003a44d8: ldr      r6, [pc, #0x1b0]
003a44dc: add      r1, r4, #0x1480
003a44e0: add      r1, r1, #0xc
003a44e4: ldr      r0, [r5, r6]
003a44e8: bl       #0x494978
003a44ec: ldr      r0, [r5, r6]
003a44f0: mov      r1, r7
003a44f4: mov      r2, #0
003a44f8: bl       #0x495430
003a44fc: movw     r6, #0x148c
003a4500: cmp      r0, #0
003a4504: str      r0, [r4, r6]
003a4508: beq      #0x3a45a8
003a450c: str      r4, [r0, #0x28]
003a4510: mov      r1, #1
003a4514: bl       #0x492aa0
003a4518: ldr      r2, [pc, #0x174]
003a451c: ldr      r3, [r4, r6]
003a4520: mov      r1, #0
003a4524: ldr      r2, [r5, r2]
003a4528: mov      r0, r3
003a452c: ldr      lr, [r2]
003a4530: ldr      ip, [r2, #4]
003a4534: ldr      r2, [r2, #8]
003a4538: str      lr, [r3, #0x34]
003a453c: str      ip, [r3, #0x38]
003a4540: str      r2, [r3, #0x3c]
003a4544: bl       #0x492aa0
003a4548: ldr      r0, [r4, r6]
003a454c: mov      r1, #1
003a4550: bl       #0x492ef0
003a4554: ldr      r0, [r4, r6]
003a4558: bl       #0x49267c
003a455c: cmp      r0, #0
003a4560: beq      #0x3a45a8
003a4564: ldr      r0, [r4, r6]
003a4568: bl       #0x49267c
003a456c: ldr      r3, [r0]
003a4570: mov      lr, pc
003a4574: ldr      pc, [r3, #0x44]
003a4578: cmp      r0, #0
003a457c: beq      #0x3a45a8
003a4580: ldr      r0, [r4, r6]
003a4584: bl       #0x49267c
003a4588: ldr      r3, [r0]
003a458c: mov      lr, pc
003a4590: ldr      pc, [r3, #0x44]
003a4594: mov      r1, #1
003a4598: ldr      r3, [r0]
003a459c: mov      lr, pc
003a45a0: ldr      pc, [r3, #0x40]
003a45a4: pop      {r4, r5, r6, r7, r8, pc}
003a45a8: pop      {r4, r5, r6, r7, r8, pc}
003a45ac: add      r7, r4, #0x4f0
003a45b0: add      r7, r7, #0xc
003a45b4: mov      r0, r7
003a45b8: mov      r1, #1
003a45bc: bl       #0x3c0378
003a45c0: cmp      r0, #0
003a45c4: beq      #0x3a45d4
003a45c8: ldr      r3, [r4, #0x520]
003a45cc: tst      r3, #0x800
003a45d0: bne      #0x3a465c
003a45d4: cmp      r6, #3
003a45d8: bgt      #0x3a44a4
003a45dc: mov      r0, r7
003a45e0: mov      r1, #1
003a45e4: bl       #0x3c034c
003a45e8: cmp      r0, #0
003a45ec: beq      #0x3a461c
003a45f0: ldr      r3, [r4, #0x520]
003a45f4: tst      r3, #0x400
003a45f8: beq      #0x3a461c
003a45fc: ldr      r3, [pc, #0x94]
003a4600: mov      r6, #3
003a4604: ldr      r3, [r5, r3]
003a4608: ldr      r3, [r3]
003a460c: ldr      r7, [r3, #0x74]
003a4610: cmn      r7, #1
003a4614: bne      #0x3a44ac
003a4618: b        #0x3a44a8
003a461c: cmp      r6, #1
003a4620: bgt      #0x3a44a4
003a4624: add      r0, r4, #0x560
003a4628: bl       #0x3de6c4
003a462c: mov      r1, #0x3f800000
003a4630: bl       #0x30e70c
003a4634: cmp      r0, #0
003a4638: beq      #0x3a44a4
003a463c: ldr      r3, [pc, #0x54]
003a4640: mov      r6, #1
003a4644: ldr      r3, [r5, r3]
003a4648: ldr      r3, [r3]
003a464c: ldr      r7, [r3, #0x7c]
003a4650: b        #0x3a4610
003a4654: ldr      r6, [pc, #0x34]
003a4658: b        #0x3a44ec
003a465c: ldr      r3, [pc, #0x34]
003a4660: mov      r6, #4
003a4664: ldr      r3, [r5, r3]
003a4668: ldr      r3, [r3]
003a466c: ldr      r7, [r3, #0x80]
003a4670: b        #0x3a4610
003a4674: ldr      r3, [pc, #0x14]
003a4678: add      r1, r4, #0x1480
003a467c: add      r1, r1, #0xc
003a4680: ldr      r0, [r5, r3]
003a4684: pop      {r4, r5, r6, r7, r8, lr}
003a4688: b        #0x494978
003a468c: ldrsheq  r0, [pc], #-0x5c
003a4690: andeq    r1, r0, r8, lsl #22
003a4694: andeq    r3, r0, ip, lsr #30
003a4698: andeq    r3, r0, r8, asr #5

# _ZN12CharAnimator17_PlayItemSwooshFXEP12ItemInstanceb
003c955c: push     {r4, r5, r6, r7, lr}
003c9560: ldr      r5, [pc, #0x90]
003c9564: cmp      r1, #0
003c9568: sub      sp, sp, #0xc
003c956c: mov      r6, r0
003c9570: add      r5, pc, r5
003c9574: mov      r4, r2
003c9578: beq      #0x3c95f0
003c957c: mov      r0, r1
003c9580: bl       #0x3f9e08
003c9584: ldr      r7, [r0, #0x18]
003c9588: cmn      r7, #1
003c958c: beq      #0x3c95f0
003c9590: cmp      r4, #0
003c9594: bne      #0x3c95d0
003c9598: ldr      r0, [r6, #4]
003c959c: bl       #0x3935dc
003c95a0: ldr      r3, [r6, #4]
003c95a4: mov      r2, r0
003c95a8: ldr      r0, [pc, #0x4c]
003c95ac: mov      r1, r7
003c95b0: add      r3, r3, #0x16c
003c95b4: ldr      r0, [r5, r0]
003c95b8: str      r4, [sp, #4]
003c95bc: str      r4, [sp]
003c95c0: bl       #0x495888
003c95c4: mov      r0, #1
003c95c8: add      sp, sp, #0xc
003c95cc: pop      {r4, r5, r6, r7, pc}
003c95d0: ldr      r3, [pc, #0x24]
003c95d4: mov      r1, r7
003c95d8: ldr      r2, [r6, #4]
003c95dc: ldr      r0, [r5, r3]
003c95e0: mov      r3, #0
003c95e4: bl       #0x495f04
003c95e8: mov      r0, #1
003c95ec: b        #0x3c95c8
003c95f0: mov      r0, #0
003c95f4: b        #0x3c95c8
003c95f8: subseq   fp, ip, r0, lsr #10
003c95fc: andeq    r1, r0, r8, lsl #22

# _ZN10GameObject14_SetFXEndPointERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
00390974: push     {r4, r5, lr}
00390978: ldr      r2, [r0, #4]
0039097c: sub      sp, sp, #0x14
00390980: mov      r4, r0
00390984: ldm      r2, {r1, r3}
00390988: rsb      r3, r1, r3
0039098c: asr      r3, r3, #4
00390990: add      r2, r3, r3, lsl #3
00390994: add      r2, r2, r2, lsl #6
00390998: add      r2, r3, r2, lsl #3
0039099c: add      r2, r2, r2, lsl #15
003909a0: add      r3, r3, r2, lsl #3
003909a4: rsb      r3, r3, #0
003909a8: cmp      r3, #2
003909ac: beq      #0x3909c0
003909b0: cmp      r3, #4
003909b4: beq      #0x3909c0
003909b8: add      sp, sp, #0x14
003909bc: pop      {r4, r5, pc}
003909c0: ldr      r2, [r1, #4]
003909c4: cmp      r2, #2
003909c8: bne      #0x3909b8
003909cc: cmp      r3, #2
003909d0: beq      #0x390a7c
003909d4: cmp      r3, #4
003909d8: beq      #0x390ac4
003909dc: mov      r1, #0
003909e0: mov      r0, r4
003909e4: bl       #0x37baf8
003909e8: bl       #0x31b580
003909ec: ldr      r2, [r4, #4]
003909f0: mov      r3, #0
003909f4: str      r3, [sp, #0xc]
003909f8: str      r3, [sp, #4]
003909fc: str      r3, [sp, #8]
00390a00: ldr      r3, [r2]
00390a04: ldr      r2, [r2, #4]
00390a08: mov      r5, r0
00390a0c: rsb      r3, r3, r2
00390a10: asr      r3, r3, #4
00390a14: add      r2, r3, r3, lsl #3
00390a18: add      r2, r2, r2, lsl #6
00390a1c: add      r2, r3, r2, lsl #3
00390a20: add      r2, r2, r2, lsl #15
00390a24: add      r3, r3, r2, lsl #3
00390a28: cmn      r3, #2
00390a2c: beq      #0x390b0c
00390a30: mov      r1, #1
00390a34: mov      r0, r4
00390a38: bl       #0x37baf8
00390a3c: bl       #0x31bbf0
00390a40: mov      r1, #2
00390a44: str      r0, [sp, #4]
00390a48: mov      r0, r4
00390a4c: bl       #0x37baf8
00390a50: bl       #0x31bbf0
00390a54: mov      r1, #3
00390a58: str      r0, [sp, #8]
00390a5c: mov      r0, r4
00390a60: bl       #0x37baf8
00390a64: bl       #0x31bbf0
00390a68: str      r0, [sp, #0xc]
00390a6c: mov      r0, r5
00390a70: add      r1, sp, #4
00390a74: bl       #0x492560
00390a78: b        #0x3909b8
00390a7c: mov      r0, r4
00390a80: mov      r1, #1
00390a84: bl       #0x37baf8
00390a88: ldr      r3, [r0, #4]
00390a8c: cmp      r3, #7
00390a90: bne      #0x3909b8
00390a94: ldr      r3, [r4, #4]
00390a98: ldr      r2, [r3, #4]
00390a9c: ldr      r3, [r3]
00390aa0: rsb      r3, r3, r2
00390aa4: asr      r3, r3, #4
00390aa8: add      r2, r3, r3, lsl #3
00390aac: add      r2, r2, r2, lsl #6
00390ab0: add      r2, r3, r2, lsl #3
00390ab4: add      r2, r2, r2, lsl #15
00390ab8: add      r3, r3, r2, lsl #3
00390abc: rsb      r3, r3, #0
00390ac0: b        #0x3909d4
00390ac4: mov      r0, r4
00390ac8: mov      r1, #1
00390acc: bl       #0x37baf8
00390ad0: ldr      r3, [r0, #4]
00390ad4: cmp      r3, #3
00390ad8: bne      #0x3909b8
00390adc: mov      r1, #2
00390ae0: mov      r0, r4
00390ae4: bl       #0x37baf8
00390ae8: ldr      r1, [r0, #4]
00390aec: cmp      r1, #3
00390af0: bne      #0x3909b8
00390af4: mov      r0, r4
00390af8: bl       #0x37baf8
00390afc: ldr      r3, [r0, #4]
00390b00: cmp      r3, #3
00390b04: bne      #0x3909b8
00390b08: b        #0x3909dc
00390b0c: mov      r1, #1
00390b10: mov      r0, r4
00390b14: bl       #0x37baf8
00390b18: bl       #0x31b5a0
00390b1c: bl       #0x3935dc
00390b20: ldr      r3, [r0]
00390b24: str      r3, [sp, #4]
00390b28: ldr      r3, [r0, #4]
00390b2c: str      r3, [sp, #8]
00390b30: ldr      r3, [r0, #8]
00390b34: str      r3, [sp, #0xc]
00390b38: b        #0x390a6c

# _ZN9Character13DisableSelfFXEv
003a40b0: movw     r3, #0x1484
003a40b4: ldr      r2, [r0, r3]
003a40b8: ldr      r3, [pc, #0x1c]
003a40bc: cmp      r2, #0
003a40c0: add      r3, pc, r3
003a40c4: bxeq     lr
003a40c8: ldr      r2, [pc, #0x10]
003a40cc: add      r1, r0, #0x1480
003a40d0: add      r1, r1, #4
003a40d4: ldr      r0, [r3, r2]
003a40d8: b        #0x494978
003a40dc: ldrsbeq  r0, [pc], #-0x90
003a40e0: andeq    r1, r0, r8, lsl #22
