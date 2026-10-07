
# _ZN6CharAI21_ParseObjectAnimEventEPKc
003d3af4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003d3af8: ldr      r4, [pc, #0x210]
003d3afc: ldr      r6, [pc, #0x210]
003d3b00: mov      sb, r1
003d3b04: add      r4, pc, r4
003d3b08: ldr      r3, [r4, r6]
003d3b0c: sub      sp, sp, #0x58
003d3b10: mov      r8, r0
003d3b14: ldr      r3, [r3]
003d3b18: mov      r1, #0x2f
003d3b1c: mov      r0, sb
003d3b20: str      r3, [sp, #0x54]
003d3b24: bl       #0x30ec28
003d3b28: subs     r5, r0, #0
003d3b2c: beq      #0x3d3c00
003d3b30: rsb      r7, sb, r5
003d3b34: add      sl, sp, #0x14
003d3b38: mov      r1, sb
003d3b3c: mov      r2, r7
003d3b40: mov      r0, sl
003d3b44: bl       #0x30de24
003d3b48: ldr      r1, [pc, #0x1c8]
003d3b4c: add      r2, sp, #0x58
003d3b50: add      r3, r2, r7
003d3b54: mov      r7, #0
003d3b58: add      r1, pc, r1
003d3b5c: mov      r0, sl
003d3b60: strb     r7, [r3, #-0x44]
003d3b64: bl       #0x30e31c
003d3b68: subs     sb, r0, #0
003d3b6c: add      r5, r5, #1
003d3b70: beq      #0x3d3c1c
003d3b74: ldr      r3, [pc, #0x1a0]
003d3b78: add      r8, sp, #8
003d3b7c: mov      r2, sl
003d3b80: ldr      r1, [r4, r3]
003d3b84: mov      r0, r8
003d3b88: mvn      r3, #0
003d3b8c: ldr      r1, [r1, #0x38]
003d3b90: str      r7, [sp]
003d3b94: str      r7, [sp, #4]
003d3b98: bl       #0x34aca0
003d3b9c: mov      r0, r8
003d3ba0: bl       #0x33ff54
003d3ba4: subs     sl, r0, #0
003d3ba8: beq      #0x3d3cc0
003d3bac: ldr      r3, [pc, #0x16c]
003d3bb0: ldr      r3, [r4, r3]
003d3bb4: ldr      r8, [r3]
003d3bb8: cmp      r8, #0
003d3bbc: beq      #0x3d3cb0
003d3bc0: ldr      r3, [pc, #0x15c]
003d3bc4: ldr      r3, [r4, r3]
003d3bc8: ldr      sb, [r3]
003d3bcc: b        #0x3d3bdc
003d3bd0: add      r7, r7, #1
003d3bd4: cmp      r7, r8
003d3bd8: beq      #0x3d3cb0
003d3bdc: mov      r0, r5
003d3be0: ldr      r1, [sb, r7, lsl #2]
003d3be4: bl       #0x30e31c
003d3be8: cmp      r0, #0
003d3bec: bne      #0x3d3bd0
003d3bf0: mov      r1, r7
003d3bf4: add      r0, sl, #0x490
003d3bf8: add      r0, r0, #0xc
003d3bfc: bl       #0x3cacb0
003d3c00: ldr      r3, [r4, r6]
003d3c04: ldr      r2, [sp, #0x54]
003d3c08: ldr      r3, [r3]
003d3c0c: cmp      r2, r3
003d3c10: bne      #0x3d3d0c
003d3c14: add      sp, sp, #0x58
003d3c18: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003d3c1c: ldr      r3, [pc, #0xf8]
003d3c20: ldr      r0, [r4, r3]
003d3c24: bl       #0x31f594
003d3c28: cmp      r0, #0
003d3c2c: beq      #0x3d3c00
003d3c30: ldr      sl, [r0, #0x128]
003d3c34: cmp      sl, r7
003d3c38: beq      #0x3d3c00
003d3c3c: ldr      r1, [r8, #4]
003d3c40: mov      r0, sl
003d3c44: bl       #0x40f980
003d3c48: cmp      r0, r7
003d3c4c: beq      #0x3d3c00
003d3c50: ldr      r3, [pc, #0xd0]
003d3c54: ldr      r3, [r4, r3]
003d3c58: ldr      r8, [r3]
003d3c5c: cmp      r8, r7
003d3c60: beq      #0x3d3cb8
003d3c64: ldr      r3, [pc, #0xc0]
003d3c68: mov      r7, sb
003d3c6c: ldr      r3, [r4, r3]
003d3c70: ldr      sb, [r3]
003d3c74: b        #0x3d3c84
003d3c78: add      r7, r7, #1
003d3c7c: cmp      r7, r8
003d3c80: beq      #0x3d3cb8
003d3c84: mov      r0, r5
003d3c88: ldr      r1, [sb, r7, lsl #2]
003d3c8c: bl       #0x30e31c
003d3c90: cmp      r0, #0
003d3c94: bne      #0x3d3c78
003d3c98: mov      r1, r7
003d3c9c: mov      r0, sl
003d3ca0: mov      r2, #0
003d3ca4: mov      r3, #1
003d3ca8: bl       #0x40f904
003d3cac: b        #0x3d3c00
003d3cb0: mvn      r1, #0
003d3cb4: b        #0x3d3bf4
003d3cb8: mvn      r1, #0
003d3cbc: b        #0x3d3c9c
003d3cc0: mov      r0, r8
003d3cc4: bl       #0x33fee4
003d3cc8: cmp      r0, #0
003d3ccc: beq      #0x3d3c00
003d3cd0: ldr      r3, [r0, #0x2d8]
003d3cd4: cmp      r3, #0
003d3cd8: beq      #0x3d3c00
003d3cdc: ldr      r3, [r3, #0x38]
003d3ce0: cmp      r3, #0
003d3ce4: beq      #0x3d3c00
003d3ce8: ldr      ip, [r3]
003d3cec: mov      r2, sl
003d3cf0: mov      r0, r3
003d3cf4: mov      r1, r5
003d3cf8: str      sl, [sp]
003d3cfc: mov      r3, sl
003d3d00: mov      lr, pc
003d3d04: ldr      pc, [ip, #0x20]
003d3d08: b        #0x3d3c00
003d3d0c: bl       #0x30e310
003d3d10: subseq   r0, ip, ip, lsl #31
003d3d14: andeq    r4, r0, ip, lsr #1
003d3d18: subeq    r1, pc, r0, asr #19
003d3d1c: strdeq   r3, r4, [r0], -r4
003d3d20: andeq    r2, r0, r8, asr #20
003d3d24: andeq    r2, r0, r4, asr #32
003d3d28: andeq    r2, r0, r8, lsr r2
003d3d2c: muleq    r0, r8, lr

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

# _ZN6CharAI12_OnAnimEventEPKc
003d4434: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003d4438: ldr      r4, [pc, #0x528]
003d443c: ldr      r7, [pc, #0x528]
003d4440: mov      r6, r0
003d4444: add      r4, pc, r4
003d4448: ldr      r3, [r4, r7]
003d444c: ldr      r0, [r0, #4]
003d4450: sub      sp, sp, #0xd4
003d4454: ldr      r3, [r3]
003d4458: add      r0, r0, #0x490
003d445c: add      r0, r0, #0xc
003d4460: mov      r5, r1
003d4464: str      r3, [sp, #0xcc]
003d4468: bl       #0x3c932c
003d446c: mov      r8, r0
003d4470: ldr      r0, [r6, #4]
003d4474: add      r0, r0, #0x490
003d4478: add      r0, r0, #0xc
003d447c: bl       #0x3c934c
003d4480: ldr      r1, [pc, #0x4e8]
003d4484: mov      r0, r5
003d4488: mov      r2, #3
003d448c: add      r1, pc, r1
003d4490: bl       #0x30ec7c
003d4494: cmp      r0, #0
003d4498: beq      #0x3d4634
003d449c: ldr      r1, [pc, #0x4d0]
003d44a0: mov      r0, r5
003d44a4: mov      r2, #3
003d44a8: add      r1, pc, r1
003d44ac: bl       #0x30ec7c
003d44b0: cmp      r0, #0
003d44b4: beq      #0x3d464c
003d44b8: ldr      r1, [pc, #0x4b8]
003d44bc: mov      r0, r5
003d44c0: mov      r2, #3
003d44c4: add      r1, pc, r1
003d44c8: bl       #0x30ec7c
003d44cc: subs     sl, r0, #0
003d44d0: beq      #0x3d45bc
003d44d4: ldr      r1, [pc, #0x4a0]
003d44d8: mov      r0, r5
003d44dc: mov      r2, #4
003d44e0: add      r1, pc, r1
003d44e4: bl       #0x30ec7c
003d44e8: cmp      r0, #0
003d44ec: bne      #0x3d465c
003d44f0: ldr      r3, [pc, #0x488]
003d44f4: add      r5, r5, #4
003d44f8: ldr      r3, [r4, r3]
003d44fc: ldr      sb, [r3]
003d4500: cmp      sb, #0
003d4504: beq      #0x3d459c
003d4508: ldr      r3, [pc, #0x474]
003d450c: mov      r8, r0
003d4510: ldr      r3, [r4, r3]
003d4514: ldr      fp, [r3]
003d4518: b        #0x3d4528
003d451c: add      r8, r8, #1
003d4520: cmp      r8, sb
003d4524: beq      #0x3d459c
003d4528: mov      r0, r5
003d452c: ldr      r1, [fp, r8, lsl #2]
003d4530: bl       #0x30e31c
003d4534: subs     sl, r0, #0
003d4538: bne      #0x3d451c
003d453c: cmn      r8, #1
003d4540: beq      #0x3d459c
003d4544: ldr      r3, [pc, #0x43c]
003d4548: ldr      r0, [r6, #4]
003d454c: ldr      r3, [r4, r3]
003d4550: ldr      sb, [r3]
003d4554: bl       #0x3935dc
003d4558: ldr      lr, [r0, #4]
003d455c: ldr      r5, [r0]
003d4560: ldr      r6, [r0, #8]
003d4564: mov      ip, #0xbf000000
003d4568: add      ip, ip, #0x800000
003d456c: str      lr, [sp, #0x14]
003d4570: mov      r0, sb
003d4574: mov      lr, #1
003d4578: mov      r1, r8
003d457c: mov      r3, sl
003d4580: add      r2, sp, #0x10
003d4584: str      r5, [sp, #0x10]
003d4588: str      r6, [sp, #0x18]
003d458c: str      lr, [sp]
003d4590: str      ip, [sp, #8]
003d4594: str      ip, [sp, #4]
003d4598: bl       #0x36b5d8
003d459c: ldr      r3, [r4, r7]
003d45a0: ldr      r2, [sp, #0xcc]
003d45a4: mov      r0, #1
003d45a8: ldr      r3, [r3]
003d45ac: cmp      r2, r3
003d45b0: bne      #0x3d4964
003d45b4: add      sp, sp, #0xd4
003d45b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003d45bc: ldr      r3, [pc, #0x3c8]
003d45c0: add      r5, r5, #3
003d45c4: ldr      r3, [r4, r3]
003d45c8: ldr      sb, [r3]
003d45cc: cmp      sb, #0
003d45d0: beq      #0x3d459c
003d45d4: ldr      r3, [pc, #0x3b4]
003d45d8: ldr      r3, [r4, r3]
003d45dc: ldr      fp, [r3]
003d45e0: b        #0x3d45f0
003d45e4: add      sl, sl, #1
003d45e8: cmp      sl, sb
003d45ec: beq      #0x3d459c
003d45f0: mov      r0, r5
003d45f4: ldr      r1, [fp, sl, lsl #2]
003d45f8: bl       #0x30e31c
003d45fc: subs     r8, r0, #0
003d4600: bne      #0x3d45e4
003d4604: cmn      sl, #1
003d4608: beq      #0x3d459c
003d460c: ldr      r0, [r6, #4]
003d4610: bl       #0x3935dc
003d4614: ldr      r3, [pc, #0x378]
003d4618: mov      r2, r0
003d461c: mov      r1, sl
003d4620: ldr      r0, [r4, r3]
003d4624: mov      r3, r8
003d4628: str      r8, [sp]
003d462c: bl       #0x495d14
003d4630: b        #0x3d459c
003d4634: mov      r0, r6
003d4638: add      r1, r5, #3
003d463c: ldr      r3, [r6]
003d4640: mov      lr, pc
003d4644: ldr      pc, [r3, #0x94]
003d4648: b        #0x3d459c
003d464c: mov      r0, r6
003d4650: add      r1, r5, #3
003d4654: bl       #0x3d3af4
003d4658: b        #0x3d459c
003d465c: ldr      r0, [r6, #4]
003d4660: add      r0, r0, #0x4f0
003d4664: add      r0, r0, #0xc
003d4668: bl       #0x3c01ac
003d466c: sub      r0, r0, #5
003d4670: cmp      r0, #8
003d4674: addls    pc, pc, r0, lsl #2
003d4678: b        #0x3d459c
003d467c: b        #0x3d4750
003d4680: b        #0x3d46f8
003d4684: b        #0x3d46a0
003d4688: b        #0x3d459c
003d468c: b        #0x3d459c
003d4690: b        #0x3d459c
003d4694: b        #0x3d459c
003d4698: b        #0x3d459c
003d469c: b        #0x3d482c
003d46a0: ldr      r1, [pc, #0x2f0]
003d46a4: mov      r0, r5
003d46a8: add      r1, pc, r1
003d46ac: bl       #0x30e31c
003d46b0: cmp      r0, #0
003d46b4: bne      #0x3d459c
003d46b8: ldr      r3, [pc, #0x2dc]
003d46bc: add      r5, sp, #0x3c
003d46c0: ldr      r8, [r4, r3]
003d46c4: mov      r0, r8
003d46c8: bl       #0x337888
003d46cc: add      r1, sp, #0x24
003d46d0: mov      r0, r5
003d46d4: bl       #0x3d43ec
003d46d8: mov      r1, r5
003d46dc: mov      r0, r8
003d46e0: bl       #0x337a88
003d46e4: mov      r0, r5
003d46e8: bl       #0x3139ac
003d46ec: mov      r0, r6
003d46f0: bl       #0x3d8ba4
003d46f4: b        #0x3d459c
003d46f8: ldr      r1, [pc, #0x2a0]
003d46fc: mov      r0, r5
003d4700: add      r1, pc, r1
003d4704: bl       #0x30e31c
003d4708: cmp      r0, #0
003d470c: bne      #0x3d459c
003d4710: ldr      r3, [pc, #0x284]
003d4714: add      r5, sp, #0x54
003d4718: ldr      r8, [r4, r3]
003d471c: mov      r0, r8
003d4720: bl       #0x337888
003d4724: add      r1, sp, #0x28
003d4728: mov      r0, r5
003d472c: bl       #0x3d43ec
003d4730: mov      r1, r5
003d4734: mov      r0, r8
003d4738: bl       #0x337a88
003d473c: mov      r0, r5
003d4740: bl       #0x3139ac
003d4744: mov      r0, r6
003d4748: bl       #0x3d8bf8
003d474c: b        #0x3d459c
003d4750: ldr      r3, [r6, #4]
003d4754: add      r1, sp, #0x20
003d4758: mov      r2, r1
003d475c: mov      r0, r3
003d4760: ldr      ip, [r3]
003d4764: add      r3, sp, #0x1c
003d4768: mov      lr, pc
003d476c: ldr      pc, [ip, #0x128]
003d4770: cmp      r0, #0
003d4774: beq      #0x3d488c
003d4778: ldr      r1, [pc, #0x224]
003d477c: mov      r0, r5
003d4780: add      r1, pc, r1
003d4784: bl       #0x30e31c
003d4788: cmp      r0, #0
003d478c: beq      #0x3d47c0
003d4790: ldr      r1, [pc, #0x210]
003d4794: mov      r0, r5
003d4798: add      r1, pc, r1
003d479c: bl       #0x30e31c
003d47a0: cmp      r0, #0
003d47a4: beq      #0x3d47c0
003d47a8: ldr      r1, [pc, #0x1fc]
003d47ac: mov      r0, r5
003d47b0: add      r1, pc, r1
003d47b4: bl       #0x30e31c
003d47b8: cmp      r0, #0
003d47bc: bne      #0x3d459c
003d47c0: ldr      r3, [pc, #0x1d4]
003d47c4: add      r5, sp, #0xb4
003d47c8: ldr      r8, [r4, r3]
003d47cc: mov      r0, r8
003d47d0: bl       #0x337888
003d47d4: add      r1, sp, #0x38
003d47d8: mov      r0, r5
003d47dc: bl       #0x3d43ec
003d47e0: mov      r1, r5
003d47e4: mov      r0, r8
003d47e8: bl       #0x337a88
003d47ec: mov      r0, r5
003d47f0: bl       #0x3139ac
003d47f4: ldr      r3, [pc, #0x1b4]
003d47f8: mov      ip, #0
003d47fc: ldr      r2, [r6, #4]
003d4800: ldr      r0, [r4, r3]
003d4804: ldr      r3, [pc, #0x1a8]
003d4808: ldr      r1, [sp, #0x1c]
003d480c: str      ip, [sp]
003d4810: ldr      lr, [r4, r3]
003d4814: mov      r3, ip
003d4818: str      ip, [sp, #8]
003d481c: str      lr, [sp, #4]
003d4820: str      ip, [sp, #0xc]
003d4824: bl       #0x3e701c
003d4828: b        #0x3d459c
003d482c: ldr      r1, [pc, #0x184]
003d4830: mov      r0, r5
003d4834: add      r1, pc, r1
003d4838: bl       #0x30e31c
003d483c: cmp      r0, #0
003d4840: bne      #0x3d459c
003d4844: ldr      r3, [pc, #0x150]
003d4848: add      r5, sp, #0x6c
003d484c: ldr      r8, [r4, r3]
003d4850: mov      r0, r8
003d4854: bl       #0x337888
003d4858: add      r1, sp, #0x2c
003d485c: mov      r0, r5
003d4860: bl       #0x3d43ec
003d4864: mov      r1, r5
003d4868: mov      r0, r8
003d486c: bl       #0x337a88
003d4870: mov      r0, r5
003d4874: bl       #0x3139ac
003d4878: mov      r0, r6
003d487c: ldr      r3, [r6]
003d4880: mov      lr, pc
003d4884: ldr      pc, [r3, #0xa0]
003d4888: b        #0x3d459c
003d488c: ldr      r1, [pc, #0x128]
003d4890: mov      r0, r5
003d4894: add      r1, pc, r1
003d4898: bl       #0x30e31c
003d489c: subs     sl, r0, #0
003d48a0: beq      #0x3d4910
003d48a4: ldr      r1, [pc, #0x114]
003d48a8: mov      r0, r5
003d48ac: add      r1, pc, r1
003d48b0: bl       #0x30e31c
003d48b4: cmp      r0, #0
003d48b8: bne      #0x3d459c
003d48bc: ldr      r3, [pc, #0xd8]
003d48c0: add      r5, sp, #0x84
003d48c4: ldr      sl, [r4, r3]
003d48c8: mov      r0, sl
003d48cc: bl       #0x337888
003d48d0: add      r1, sp, #0x30
003d48d4: mov      r0, r5
003d48d8: bl       #0x3d43ec
003d48dc: mov      r1, r5
003d48e0: mov      r0, sl
003d48e4: bl       #0x337a88
003d48e8: mov      r0, r5
003d48ec: bl       #0x3139ac
003d48f0: mov      r0, r6
003d48f4: sub      r2, r8, #1
003d48f8: ldr      ip, [r6]
003d48fc: ldr      r1, [r6, #0x74]
003d4900: mov      r3, #1
003d4904: mov      lr, pc
003d4908: ldr      pc, [ip, #0xa8]
003d490c: b        #0x3d459c
003d4910: ldr      r3, [pc, #0x84]
003d4914: add      r5, sp, #0x9c
003d4918: ldr      sb, [r4, r3]
003d491c: mov      r0, sb
003d4920: bl       #0x337888
003d4924: add      r1, sp, #0x34
003d4928: mov      r0, r5
003d492c: bl       #0x3d43ec
003d4930: mov      r1, r5
003d4934: mov      r0, sb
003d4938: bl       #0x337a88
003d493c: mov      r0, r5
003d4940: bl       #0x3139ac
003d4944: mov      r0, r6
003d4948: sub      r2, r8, #1
003d494c: mov      r3, sl
003d4950: ldr      ip, [r6]
003d4954: ldr      r1, [r6, #0x74]
003d4958: mov      lr, pc
003d495c: ldr      pc, [ip, #0xa8]
003d4960: b        #0x3d459c
003d4964: bl       #0x30e310
003d4968: subseq   r0, ip, ip, asr #12
003d496c: andeq    r4, r0, ip, lsr #1
003d4970: strheq   r1, [pc], #-4
003d4974: subeq    r1, pc, r0, lsr #1
003d4978: subeq    r1, pc, ip, lsl #1
003d497c: subeq    r1, pc, r8, ror r0
003d4980: andeq    r3, r0, r8, lsr sp
003d4984: andeq    r3, r0, r8, lsr #19
003d4988: andeq    r0, r0, r4, lsr #27
003d498c: andeq    r0, r0, r4, asr #13
003d4990: muleq    r0, r4, r2
003d4994: andeq    r1, r0, r8, lsl #22
003d4998: subeq    r0, pc, r8, lsl #30
003d499c: andeq    r0, r0, r4, lsl #17
003d49a0: subeq    r0, pc, r0, lsl #29
003d49a4: subeq    r0, pc, r0, ror #27
003d49a8: ldrdeq   r0, r1, [pc], #-0xd8
003d49ac: ldrdeq   r0, r1, [pc], #-0xd0
003d49b0: andeq    r0, r0, ip, asr #16
003d49b4: andeq    r1, r0, r4, lsr r8
003d49b8: subeq    r0, pc, ip, ror #26
003d49bc: ldrdeq   r0, r1, [pc], #-0xcc
003d49c0: subeq    r0, pc, r4, ror #25

# _ZNK10AnimatedFX12HasCompletedEv
004924e0: push     {r4, lr}
004924e4: ldr      r3, [pc, #0x5c]
004924e8: ldr      r1, [pc, #0x5c]
004924ec: ldr      r2, [r0, #0x2c]
004924f0: add      r3, pc, r3
004924f4: ldr      r0, [r3, r1]
004924f8: movw     r1, #0x6164
004924fc: movt     r1, #0x7065
00492500: ldr      r0, [r0, #0x10]
00492504: ldr      r2, [r2, #8]
00492508: ldr      r3, [r0, #0x1c]
0049250c: mov      r0, r3
00492510: ldr      r3, [r3]
00492514: mov      lr, pc
00492518: ldr      pc, [r3, #0x1c]
0049251c: subs     r3, r0, #0
00492520: beq      #0x492540
00492524: ldr      r3, [r3]
00492528: mov      lr, pc
0049252c: ldr      pc, [r3, #0xf8]
00492530: cmp      r0, #0
00492534: movgt    r0, #0
00492538: movle    r0, #1
0049253c: pop      {r4, pc}
00492540: mov      r0, #1
00492544: pop      {r4, pc}
00492548: subseq   r2, r0, r0, lsr #11
0049254c: strdeq   r3, r4, [r0], -r4

# _ZNK7Point3DIfEeqERKS0_
00312b6c: push     {r4, r5, r6, lr}
00312b70: mov      r4, r0
00312b74: mov      r5, r1
00312b78: ldr      r0, [r0]
00312b7c: ldr      r1, [r1]
00312b80: bl       #0x30e3ac
00312b84: movw     r1, #0xb717
00312b88: bic      r0, r0, #0x80000000
00312b8c: movt     r1, #0x38d1
00312b90: bl       #0x30e70c
00312b94: cmp      r0, #0
00312b98: beq      #0x312bf0
00312b9c: ldr      r1, [r5, #4]
00312ba0: ldr      r0, [r4, #4]
00312ba4: bl       #0x30e3ac
00312ba8: movw     r1, #0xb717
00312bac: bic      r0, r0, #0x80000000
00312bb0: movt     r1, #0x38d1
00312bb4: bl       #0x30e70c
00312bb8: cmp      r0, #0
00312bbc: beq      #0x312bf0
00312bc0: ldr      r1, [r5, #8]
00312bc4: ldr      r0, [r4, #8]
00312bc8: bl       #0x30e3ac
00312bcc: movw     r1, #0xb717
00312bd0: bic      r0, r0, #0x80000000
00312bd4: movt     r1, #0x38d1
00312bd8: bl       #0x30e70c
00312bdc: cmp      r0, #0
00312be0: mov      r0, #0
00312be4: movne    r0, #1
00312be8: uxtb     r0, r0
00312bec: pop      {r4, r5, r6, pc}
00312bf0: mov      r0, #0
00312bf4: pop      {r4, r5, r6, pc}
