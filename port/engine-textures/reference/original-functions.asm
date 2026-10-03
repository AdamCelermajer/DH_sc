
# _ZN6glitch5video12_GLOBAL__N_113readPVRHeaderEPNS_2io9IReadFileERNS1_10SPVRHeaderERb, ELF VA 0x6058a4, 620 bytes
006058a4: f0452de9 push     {r4, r5, r6, r7, r8, sl, lr}
006058a8: 4c529fe5 ldr      r5, [pc, #0x24c]
006058ac: 4c829fe5 ldr      r8, [pc, #0x24c]
006058b0: 1cd04de2 sub      sp, sp, #0x1c
006058b4: 05508fe0 add      r5, pc, r5
006058b8: 083095e7 ldr      r3, [r5, r8]
006058bc: 0170a0e1 mov      r7, r1
006058c0: 0010a0e3 mov      r1, #0
006058c4: 00c093e5 ldr      ip, [r3]
006058c8: 02a0a0e1 mov      sl, r2
006058cc: 003090e5 ldr      r3, [r0]
006058d0: 0120a0e1 mov      r2, r1
006058d4: 14c08de5 str      ip, [sp, #0x14]
006058d8: 0040a0e1 mov      r4, r0
006058dc: 0fe0a0e1 mov      lr, pc
006058e0: 18f093e5 ldr      pc, [r3, #0x18]
006058e4: 0030a0e3 mov      r3, #0
006058e8: 0030cae5 strb     r3, [sl]
006058ec: 0c608de2 add      r6, sp, #0xc
006058f0: 1330cde5 strb     r3, [sp, #0x13]
006058f4: 0c30cde5 strb     r3, [sp, #0xc]
006058f8: 0d30cde5 strb     r3, [sp, #0xd]
006058fc: 0e30cde5 strb     r3, [sp, #0xe]
00605900: 0f30cde5 strb     r3, [sp, #0xf]
00605904: 1030cde5 strb     r3, [sp, #0x10]
00605908: 1130cde5 strb     r3, [sp, #0x11]
0060590c: 1230cde5 strb     r3, [sp, #0x12]
00605910: 0610a0e1 mov      r1, r6
00605914: 0820a0e3 mov      r2, #8
00605918: 003094e5 ldr      r3, [r4]
0060591c: 0400a0e1 mov      r0, r4
00605920: 0fe0a0e1 mov      lr, pc
00605924: 0cf093e5 ldr      pc, [r3, #0xc]
00605928: d4119fe5 ldr      r1, [pc, #0x1d4]
0060592c: 0600a0e1 mov      r0, r6
00605930: 0820a0e3 mov      r2, #8
00605934: 01108fe0 add      r1, pc, r1
00605938: cf24f4eb bl       #0x30ec7c
0060593c: 000050e3 cmp      r0, #0
00605940: 1100001a bne      #0x60598c
00605944: 003094e5 ldr      r3, [r4]
00605948: 0400a0e1 mov      r0, r4
0060594c: 0710a0e1 mov      r1, r7
00605950: 3420a0e3 mov      r2, #0x34
00605954: 0fe0a0e1 mov      lr, pc
00605958: 0cf093e5 ldr      pc, [r3, #0xc]
0060595c: 0130a0e3 mov      r3, #1
00605960: 340050e3 cmp      r0, #0x34
00605964: 0030cae5 strb     r3, [sl]
00605968: 1400000a beq      #0x6059c0
0060596c: 0000a0e3 mov      r0, #0
00605970: 083095e7 ldr      r3, [r5, r8]
00605974: 14209de5 ldr      r2, [sp, #0x14]
00605978: 003093e5 ldr      r3, [r3]
0060597c: 030052e1 cmp      r2, r3
00605980: 5c00001a bne      #0x605af8
00605984: 1cd08de2 add      sp, sp, #0x1c
00605988: f085bde8 pop      {r4, r5, r6, r7, r8, sl, pc}
0060598c: 0610a0e1 mov      r1, r6
00605990: 0820a0e3 mov      r2, #8
00605994: 0700a0e1 mov      r0, r7
00605998: b223f4eb bl       #0x30e868
0060599c: 003094e5 ldr      r3, [r4]
006059a0: 0400a0e1 mov      r0, r4
006059a4: 081087e2 add      r1, r7, #8
006059a8: 2c20a0e3 mov      r2, #0x2c
006059ac: 0fe0a0e1 mov      lr, pc
006059b0: 0cf093e5 ldr      pc, [r3, #0xc]
006059b4: 080080e2 add      r0, r0, #8
006059b8: 340050e3 cmp      r0, #0x34
006059bc: eaffff1a bne      #0x60596c
006059c0: 40119fe5 ldr      r1, [pc, #0x140]
006059c4: 2c0087e2 add      r0, r7, #0x2c
006059c8: 0420a0e3 mov      r2, #4
006059cc: 01108fe0 add      r1, pc, r1
006059d0: a924f4eb bl       #0x30ec7c
006059d4: 000050e3 cmp      r0, #0
006059d8: e3ffff1a bne      #0x60596c
006059dc: 003097e5 ldr      r3, [r7]
006059e0: 340053e3 cmp      r3, #0x34
006059e4: e0ffff1a bne      #0x60596c
006059e8: 100097e5 ldr      r0, [r7, #0x10]
006059ec: 013c10e2 ands     r3, r0, #0x100
006059f0: 0200000a beq      #0x605a00
006059f4: 0c2097e5 ldr      r2, [r7, #0xc]
006059f8: 000052e3 cmp      r2, #0
006059fc: daffff0a beq      #0x60596c
00605a00: 010a10e3 tst      r0, #0x1000
00605a04: 3000001a bne      #0x605acc
00605a08: 000053e3 cmp      r3, #0
00605a0c: 3200000a beq      #0x605adc
00605a10: 083097e5 ldr      r3, [r7, #8]
00605a14: 000053e3 cmp      r3, #0
00605a18: 0020e003 mvneq    r2, #0
00605a1c: 0300000a beq      #0x605a30
00605a20: 0020e0e3 mvn      r2, #0
00605a24: a330b0e1 lsrs     r3, r3, #1
00605a28: 012082e2 add      r2, r2, #1
00605a2c: fcffff1a bne      #0x605a24
00605a30: 043097e5 ldr      r3, [r7, #4]
00605a34: 08208de5 str      r2, [sp, #8]
00605a38: 000053e3 cmp      r3, #0
00605a3c: 0010e003 mvneq    r1, #0
00605a40: 0300000a beq      #0x605a54
00605a44: 0010e0e3 mvn      r1, #0
00605a48: a330b0e1 lsrs     r3, r3, #1
00605a4c: 011081e2 add      r1, r1, #1
00605a50: fcffff1a bne      #0x605a48
00605a54: 010910e3 tst      r0, #0x4000
00605a58: 04108de5 str      r1, [sp, #4]
00605a5c: 2000001a bne      #0x605ae4
00605a60: 0130a0e3 mov      r3, #1
00605a64: 0000e0e3 mvn      r0, #0
00605a68: a330b0e1 lsrs     r3, r3, #1
00605a6c: 010080e2 add      r0, r0, #1
00605a70: fcffff1a bne      #0x605a68
00605a74: 020051e1 cmp      r1, r2
00605a78: 0120a081 movhi    r2, r1
00605a7c: 04308d82 addhi    r3, sp, #4
00605a80: 08308d92 addls    r3, sp, #8
00605a84: 000052e1 cmp      r2, r0
00605a88: 0d30a031 movlo    r3, sp
00605a8c: 00008de5 str      r0, [sp]
00605a90: 002093e5 ldr      r2, [r3]
00605a94: 0c3097e5 ldr      r3, [r7, #0xc]
00605a98: 030052e1 cmp      r2, r3
00605a9c: 0e00000a beq      #0x605adc
00605aa0: 003094e5 ldr      r3, [r4]
00605aa4: 0400a0e1 mov      r0, r4
00605aa8: 0fe0a0e1 mov      lr, pc
00605aac: 28f093e5 ldr      pc, [r3, #0x28]
00605ab0: 54109fe5 ldr      r1, [pc, #0x54]
00605ab4: 0020a0e1 mov      r2, r0
00605ab8: 0300a0e3 mov      r0, #3
00605abc: 01108fe0 add      r1, pc, r1
00605ac0: 5b1500eb bl       #0x60b034
00605ac4: 0000a0e3 mov      r0, #0
00605ac8: a8ffffea b        #0x605970
00605acc: 302097e5 ldr      r2, [r7, #0x30]
00605ad0: 060052e3 cmp      r2, #6
00605ad4: a4ffff1a bne      #0x60596c
00605ad8: caffffea b        #0x605a08
00605adc: 0100a0e3 mov      r0, #1
00605ae0: a2ffffea b        #0x605970
00605ae4: 303097e5 ldr      r3, [r7, #0x30]
00605ae8: 000053e3 cmp      r3, #0
00605aec: 0000e003 mvneq    r0, #0
00605af0: dbffff1a bne      #0x605a64
00605af4: deffffea b        #0x605a74
00605af8: 0422f4eb bl       #0x30e310
00605afc: dcf13800 ldrsbteq pc, [r8], -ip
00605b00: ac400000 andeq    r4, r0, ip, lsr #1

# _ZL18InterpolateColoursPKiS0_S0_S0_iiiPi, ELF VA 0x69f570, 388 bytes
0069f570: f00f2de9 push     {r4, r5, r6, r7, r8, sb, sl, fp}
0069f574: 48d04de2 sub      sp, sp, #0x48
0069f578: 74c09de5 ldr      ip, [sp, #0x74]
0069f57c: 01b0a0e1 mov      fp, r1
0069f580: 38708de2 add      r7, sp, #0x38
0069f584: 0010a0e3 mov      r1, #0
0069f588: 28508de2 add      r5, sp, #0x28
0069f58c: 18608de2 add      r6, sp, #0x18
0069f590: 08408de2 add      r4, sp, #8
0069f594: 04c08de5 str      ip, [sp, #4]
0069f598: 019090e7 ldr      sb, [r0, r1]
0069f59c: 01a09be7 ldr      sl, [fp, r1]
0069f5a0: 018092e7 ldr      r8, [r2, r1]
0069f5a4: 01c093e7 ldr      ip, [r3, r1]
0069f5a8: 019087e7 str      sb, [r7, r1]
0069f5ac: 01a085e7 str      sl, [r5, r1]
0069f5b0: 018086e7 str      r8, [r6, r1]
0069f5b4: 01c084e7 str      ip, [r4, r1]
0069f5b8: 041081e2 add      r1, r1, #4
0069f5bc: 100051e3 cmp      r1, #0x10
0069f5c0: f4ffff1a bne      #0x69f598
0069f5c4: 70209de5 ldr      r2, [sp, #0x70]
0069f5c8: 68a09de5 ldr      sl, [sp, #0x68]
0069f5cc: 04c09de5 ldr      ip, [sp, #4]
0069f5d0: 02b0e0e1 mvn      fp, r2
0069f5d4: 02b00be2 and      fp, fp, #2
0069f5d8: 033002e2 and      r3, r2, #3
0069f5dc: 00005ae3 cmp      sl, #0
0069f5e0: 8bb083e1 orr      fp, r3, fp, lsl #1
0069f5e4: 3900000a beq      #0x69f6d0
0069f5e8: 6c209de5 ldr      r2, [sp, #0x6c]
0069f5ec: 02b04be2 sub      fp, fp, #2
0069f5f0: 0800a0e3 mov      r0, #8
0069f5f4: 0280e0e1 mvn      r8, r2
0069f5f8: 048008e2 and      r8, r8, #4
0069f5fc: 073002e2 and      r3, r2, #7
0069f600: 888083e1 orr      r8, r3, r8, lsl #1
0069f604: 048048e2 sub      r8, r8, #4
0069f608: 0030a0e3 mov      r3, #0
0069f60c: 04b08de5 str      fp, [sp, #4]
0069f610: 031097e7 ldr      r1, [r7, r3]
0069f614: 03b095e7 ldr      fp, [r5, r3]
0069f618: 032096e7 ldr      r2, [r6, r3]
0069f61c: 910009e0 mul      sb, r1, r0
0069f620: 0b1061e0 rsb      r1, r1, fp
0069f624: 03b094e7 ldr      fp, [r4, r3]
0069f628: 92000ae0 mul      sl, r2, r0
0069f62c: 0b2062e0 rsb      r2, r2, fp
0069f630: 919821e0 mla      r1, r1, r8, sb
0069f634: 92a822e0 mla      r2, r2, r8, sl
0069f638: 04a09de5 ldr      sl, [sp, #4]
0069f63c: 022061e0 rsb      r2, r1, r2
0069f640: 9a0202e0 mul      r2, sl, r2
0069f644: 011182e0 add      r1, r2, r1, lsl #2
0069f648: 03108ce7 str      r1, [ip, r3]
0069f64c: 043083e2 add      r3, r3, #4
0069f650: 100053e3 cmp      r3, #0x10
0069f654: edffff1a bne      #0x69f610
0069f658: 68b09de5 ldr      fp, [sp, #0x68]
0069f65c: 00005be3 cmp      fp, #0
0069f660: 1200001a bne      #0x69f6b0
0069f664: 0e009ce8 ldm      ip, {r1, r2, r3}
0069f668: c110a0e1 asr      r1, r1, #1
0069f66c: c220a0e1 asr      r2, r2, #1
0069f670: c330a0e1 asr      r3, r3, #1
0069f674: 0e008ce8 stm      ip, {r1, r2, r3}
0069f678: 0c108ce2 add      r1, ip, #0xc
0069f67c: 0030a0e3 mov      r3, #0
0069f680: 03209ce7 ldr      r2, [ip, r3]
0069f684: c22282e0 add      r2, r2, r2, asr #5
0069f688: 03208ce7 str      r2, [ip, r3]
0069f68c: 043083e2 add      r3, r3, #4
0069f690: 0c0053e3 cmp      r3, #0xc
0069f694: f9ffff1a bne      #0x69f680
0069f698: 003091e5 ldr      r3, [r1]
0069f69c: 433283e0 add      r3, r3, r3, asr #4
0069f6a0: 003081e5 str      r3, [r1]
0069f6a4: 48d08de2 add      sp, sp, #0x48
0069f6a8: f00fbde8 pop      {r4, r5, r6, r7, r8, sb, sl, fp}
0069f6ac: 1eff2fe1 bx       lr
0069f6b0: 0f009ce8 ldm      ip, {r0, r1, r2, r3}
0069f6b4: 4111a0e1 asr      r1, r1, #2
0069f6b8: 4001a0e1 asr      r0, r0, #2
0069f6bc: 4221a0e1 asr      r2, r2, #2
0069f6c0: c330a0e1 asr      r3, r3, #1
0069f6c4: 0f008ce8 stm      ip, {r0, r1, r2, r3}
0069f6c8: 0c108ce2 add      r1, ip, #0xc
0069f6cc: eaffffea b        #0x69f67c
0069f6d0: 6c309de5 ldr      r3, [sp, #0x6c]
0069f6d4: 02b04be2 sub      fp, fp, #2
0069f6d8: 0400a0e3 mov      r0, #4
0069f6dc: 0380e0e1 mvn      r8, r3
0069f6e0: 028008e2 and      r8, r8, #2
0069f6e4: 033003e2 and      r3, r3, #3
0069f6e8: 888083e1 orr      r8, r3, r8, lsl #1
0069f6ec: 028048e2 sub      r8, r8, #2
0069f6f0: c4ffffea b        #0x69f608

# _ZL9TwiddleUVmmmm, ELF VA 0x69f6f4, 116 bytes
0069f6f4: 010050e1 cmp      r0, r1
0069f6f8: 0100a021 movhs    r0, r1
0069f6fc: 0310a031 movlo    r1, r3
0069f700: 0210a021 movhs    r1, r2
0069f704: 010050e3 cmp      r0, #1
0069f708: 0030a093 movls    r3, #0
0069f70c: 70002de9 push     {r4, r5, r6}
0069f710: 0350a091 movls    r5, r3
0069f714: 0360a091 movls    r6, r3
0069f718: 0e00009a bls      #0x69f758
0069f71c: 0050a0e3 mov      r5, #0
0069f720: 0140a0e3 mov      r4, #1
0069f724: 0560a0e1 mov      r6, r5
0069f728: 04c0a0e1 mov      ip, r4
0069f72c: 000000ea b        #0x69f734
0069f730: 0441a0e1 lsl      r4, r4, #2
0069f734: 02001ce1 tst      ip, r2
0069f738: 04508511 orrne    r5, r5, r4
0069f73c: 03001ce1 tst      ip, r3
0069f740: 8cc0a0e1 lsl      ip, ip, #1
0069f744: 84508511 orrne    r5, r5, r4, lsl #1
0069f748: 0c0050e1 cmp      r0, ip
0069f74c: 016086e2 add      r6, r6, #1
0069f750: f6ffff8a bhi      #0x69f730
0069f754: 8630a0e1 lsl      r3, r6, #1
0069f758: 3166a0e1 lsr      r6, r1, r6
0069f75c: 160385e1 orr      r0, r5, r6, lsl r3
0069f760: 7000bde8 pop      {r4, r5, r6}
0069f764: 1eff2fe1 bx       lr

# _Z15PVRTCDecompressPKviiiPh, ELF VA 0x69f768, 2172 bytes
0069f768: f04f2de9 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0069f76c: 5ade4de2 sub      sp, sp, #0x5a0
0069f770: 04d04de2 sub      sp, sp, #4
0069f774: 000051e3 cmp      r1, #0
0069f778: 28108de5 str      r1, [sp, #0x28]
0069f77c: 30008de5 str      r0, [sp, #0x30]
0069f780: 0810a013 movne    r1, #8
0069f784: 0410a003 moveq    r1, #4
0069f788: 0200a0e1 mov      r0, r2
0069f78c: 6c208de5 str      r2, [sp, #0x6c]
0069f790: b0308de5 str      r3, [sp, #0xb0]
0069f794: 68108de5 str      r1, [sp, #0x68]
0069f798: c1baf1eb bl       #0x30e2a4
0069f79c: b0209de5 ldr      r2, [sp, #0xb0]
0069f7a0: 020050e3 cmp      r0, #2
0069f7a4: 0200a0b3 movlt    r0, #2
0069f7a8: 2c008de5 str      r0, [sp, #0x2c]
0069f7ac: 070052e3 cmp      r2, #7
0069f7b0: 4231a0c1 asrgt    r3, r2, #2
0069f7b4: 24308dc5 strgt    r3, [sp, #0x24]
0069f7b8: 040000ca bgt      #0x69f7d0
0069f7bc: b0409de5 ldr      r4, [sp, #0xb0]
0069f7c0: 000054e3 cmp      r4, #0
0069f7c4: 8b0100da ble      #0x69fdf8
0069f7c8: 0250a0e3 mov      r5, #2
0069f7cc: 24508de5 str      r5, [sp, #0x24]
0069f7d0: 0030a0e3 mov      r3, #0
0069f7d4: 70308de5 str      r3, [sp, #0x70]
0069f7d8: 78358de5 str      r3, [sp, #0x578]
0069f7dc: 7c358de5 str      r3, [sp, #0x57c]
0069f7e0: 80358de5 str      r3, [sp, #0x580]
0069f7e4: 84358de5 str      r3, [sp, #0x584]
0069f7e8: dc379fe5 ldr      r3, [pc, #0x7dc]
0069f7ec: 68709de5 ldr      r7, [sp, #0x68]
0069f7f0: d8079fe5 ldr      r0, [pc, #0x7d8]
0069f7f4: 03308fe0 add      r3, pc, r3
0069f7f8: ac308de5 str      r3, [sp, #0xac]
0069f7fc: d0379fe5 ldr      r3, [pc, #0x7d0]
0069f800: a720a0e1 lsr      r2, r7, #1
0069f804: 002062e2 rsb      r2, r2, #0
0069f808: 03308fe0 add      r3, pc, r3
0069f80c: b4308de5 str      r3, [sp, #0xb4]
0069f810: c0379fe5 ldr      r3, [pc, #0x7c0]
0069f814: 00c0a0e3 mov      ip, #0
0069f818: a0208de5 str      r2, [sp, #0xa0]
0069f81c: 03308fe0 add      r3, pc, r3
0069f820: c0308de5 str      r3, [sp, #0xc0]
0069f824: 50c08de5 str      ip, [sp, #0x50]
0069f828: c4008de5 str      r0, [sp, #0xc4]
0069f82c: 6c309de5 ldr      r3, [sp, #0x6c]
0069f830: 000053e3 cmp      r3, #0
0069f834: 650100da ble      #0x69fdd0
0069f838: 50409de5 ldr      r4, [sp, #0x50]
0069f83c: b0509de5 ldr      r5, [sp, #0xb0]
0069f840: 137d8de2 add      r7, sp, #0x4c0
0069f844: 022044e2 sub      r2, r4, #2
0069f848: 013045e2 sub      r3, r5, #1
0069f84c: 033002e0 and      r3, r2, r3
0069f850: 030083e2 add      r0, r3, #3
0069f854: 0410e0e1 mvn      r1, r4
0069f858: 000053e3 cmp      r3, #0
0069f85c: 032004e2 and      r2, r4, #3
0069f860: 0030a0b1 movlt    r3, r0
0069f864: 021001e2 and      r1, r1, #2
0069f868: 811082e1 orr      r1, r2, r1, lsl #1
0069f86c: 4331a0e1 asr      r3, r3, #2
0069f870: a8108de5 str      r1, [sp, #0xa8]
0069f874: 54308de5 str      r3, [sp, #0x54]
0069f878: d0308de2 add      r3, sp, #0xd0
0069f87c: a8509de5 ldr      r5, [sp, #0xa8]
0069f880: 54009de5 ldr      r0, [sp, #0x54]
0069f884: 083043e2 sub      r3, r3, #8
0069f888: 1c308de5 str      r3, [sp, #0x1c]
0069f88c: 24309de5 ldr      r3, [sp, #0x24]
0069f890: 087087e2 add      r7, r7, #8
0069f894: 3c708de5 str      r7, [sp, #0x3c]
0069f898: b2cf8de2 add      ip, sp, #0x2c8
0069f89c: 011080e2 add      r1, r0, #1
0069f8a0: a8409de5 ldr      r4, [sp, #0xa8]
0069f8a4: 010085e2 add      r0, r5, #1
0069f8a8: 6c709de5 ldr      r7, [sp, #0x6c]
0069f8ac: 00038ce0 add      r0, ip, r0, lsl #6
0069f8b0: 012043e2 sub      r2, r3, #1
0069f8b4: 013045e2 sub      r3, r5, #1
0069f8b8: 03338ce0 add      r3, ip, r3, lsl #6
0069f8bc: 64c08de5 str      ip, [sp, #0x64]
0069f8c0: bc008de5 str      r0, [sp, #0xbc]
0069f8c4: 2cc09de5 ldr      ip, [sp, #0x2c]
0069f8c8: 3c009de5 ldr      r0, [sp, #0x3c]
0069f8cc: 022001e0 and      r2, r1, r2
0069f8d0: 0443a0e1 lsl      r4, r4, #6
0069f8d4: 017047e2 sub      r7, r7, #1
0069f8d8: a4408de5 str      r4, [sp, #0xa4]
0069f8dc: 58208de5 str      r2, [sp, #0x58]
0069f8e0: b8308de5 str      r3, [sp, #0xb8]
0069f8e4: 8c708de5 str      r7, [sp, #0x8c]
0069f8e8: 3c209de5 ldr      r2, [sp, #0x3c]
0069f8ec: 3c309de5 ldr      r3, [sp, #0x3c]
0069f8f0: 3c409de5 ldr      r4, [sp, #0x3c]
0069f8f4: 3c509de5 ldr      r5, [sp, #0x3c]
0069f8f8: 3c709de5 ldr      r7, [sp, #0x3c]
0069f8fc: 01c04ce2 sub      ip, ip, #1
0069f900: 200080e2 add      r0, r0, #0x20
0069f904: 3c109de5 ldr      r1, [sp, #0x3c]
0069f908: 90c08de5 str      ip, [sp, #0x90]
0069f90c: 98008de5 str      r0, [sp, #0x98]
0069f910: 1cc09de5 ldr      ip, [sp, #0x1c]
0069f914: a4009de5 ldr      r0, [sp, #0xa4]
0069f918: 602082e2 add      r2, r2, #0x60
0069f91c: 103083e2 add      r3, r3, #0x10
0069f920: 304084e2 add      r4, r4, #0x30
0069f924: 505085e2 add      r5, r5, #0x50
0069f928: 707087e2 add      r7, r7, #0x70
0069f92c: 401081e2 add      r1, r1, #0x40
0069f930: 78208de5 str      r2, [sp, #0x78]
0069f934: 7c308de5 str      r3, [sp, #0x7c]
0069f938: 80408de5 str      r4, [sp, #0x80]
0069f93c: 84508de5 str      r5, [sp, #0x84]
0069f940: 88708de5 str      r7, [sp, #0x88]
0069f944: 572e8de2 add      r2, sp, #0x570
0069f948: 163d8de2 add      r3, sp, #0x580
0069f94c: 564e8de2 add      r4, sp, #0x560
0069f950: 555e8de2 add      r5, sp, #0x550
0069f954: 157d8de2 add      r7, sp, #0x540
0069f958: 74108de5 str      r1, [sp, #0x74]
0069f95c: 00c08ce0 add      ip, ip, r0
0069f960: 0010a0e3 mov      r1, #0
0069f964: 082082e2 add      r2, r2, #8
0069f968: 083083e2 add      r3, r3, #8
0069f96c: 084084e2 add      r4, r4, #8
0069f970: 085085e2 add      r5, r5, #8
0069f974: 087087e2 add      r7, r7, #8
0069f978: 94c08de5 str      ip, [sp, #0x94]
0069f97c: 10108de5 str      r1, [sp, #0x10]
0069f980: 44208de5 str      r2, [sp, #0x44]
0069f984: 34308de5 str      r3, [sp, #0x34]
0069f988: 60408de5 str      r4, [sp, #0x60]
0069f98c: 5c508de5 str      r5, [sp, #0x5c]
0069f990: 9c708de5 str      r7, [sp, #0x9c]
0069f994: a0209de5 ldr      r2, [sp, #0xa0]
0069f998: 10109de5 ldr      r1, [sp, #0x10]
0069f99c: 8c309de5 ldr      r3, [sp, #0x8c]
0069f9a0: 020081e0 add      r0, r1, r2
0069f9a4: 000003e0 and      r0, r3, r0
0069f9a8: 68109de5 ldr      r1, [sp, #0x68]
0069f9ac: 3cbaf1eb bl       #0x30e2a4
0069f9b0: 0040a0e1 mov      r4, r0
0069f9b4: 0430a0e1 mov      r3, r4
0069f9b8: 2c109de5 ldr      r1, [sp, #0x2c]
0069f9bc: 54209de5 ldr      r2, [sp, #0x54]
0069f9c0: 24009de5 ldr      r0, [sp, #0x24]
0069f9c4: 4affffeb bl       #0x69f6f4
0069f9c8: 90709de5 ldr      r7, [sp, #0x90]
0069f9cc: 30109de5 ldr      r1, [sp, #0x30]
0069f9d0: 015084e2 add      r5, r4, #1
0069f9d4: 075005e0 and      r5, r5, r7
0069f9d8: 80c181e0 add      ip, r1, r0, lsl #3
0069f9dc: 54209de5 ldr      r2, [sp, #0x54]
0069f9e0: 2c109de5 ldr      r1, [sp, #0x2c]
0069f9e4: 0530a0e1 mov      r3, r5
0069f9e8: 24009de5 ldr      r0, [sp, #0x24]
0069f9ec: 88c58de5 str      ip, [sp, #0x588]
0069f9f0: 3fffffeb bl       #0x69f6f4
0069f9f4: 30209de5 ldr      r2, [sp, #0x30]
0069f9f8: 0430a0e1 mov      r3, r4
0069f9fc: 2c109de5 ldr      r1, [sp, #0x2c]
0069fa00: 80c182e0 add      ip, r2, r0, lsl #3
0069fa04: 58209de5 ldr      r2, [sp, #0x58]
0069fa08: 24009de5 ldr      r0, [sp, #0x24]
0069fa0c: 8cc58de5 str      ip, [sp, #0x58c]
0069fa10: 37ffffeb bl       #0x69f6f4
0069fa14: 30409de5 ldr      r4, [sp, #0x30]
0069fa18: 0530a0e1 mov      r3, r5
0069fa1c: 2c109de5 ldr      r1, [sp, #0x2c]
0069fa20: 80c184e0 add      ip, r4, r0, lsl #3
0069fa24: 58209de5 ldr      r2, [sp, #0x58]
0069fa28: 24009de5 ldr      r0, [sp, #0x24]
0069fa2c: 90c58de5 str      ip, [sp, #0x590]
0069fa30: 2fffffeb bl       #0x69f6f4
0069fa34: 34109de5 ldr      r1, [sp, #0x34]
0069fa38: 803184e0 add      r3, r4, r0, lsl #3
0069fa3c: 1020a0e3 mov      r2, #0x10
0069fa40: 44009de5 ldr      r0, [sp, #0x44]
0069fa44: 94358de5 str      r3, [sp, #0x594]
0069fa48: e4baf1eb bl       #0x30e5e0
0069fa4c: 000050e3 cmp      r0, #0
0069fa50: 8100000a beq      #0x69fc5c
0069fa54: 28509de5 ldr      r5, [sp, #0x28]
0069fa58: 68c09de5 ldr      ip, [sp, #0x68]
0069fa5c: 64709de5 ldr      r7, [sp, #0x64]
0069fa60: 00e0a0e3 mov      lr, #0
0069fa64: 590e8de2 add      r0, sp, #0x590
0069fa68: 0e5055e0 subs     r5, r5, lr
0069fa6c: 0150a013 movne    r5, #1
0069fa70: 0cc1a0e1 lsl      ip, ip, #2
0069fa74: 080080e2 add      r0, r0, #8
0069fa78: 18508de5 str      r5, [sp, #0x18]
0069fa7c: 38708de5 str      r7, [sp, #0x38]
0069fa80: 20e08de5 str      lr, [sp, #0x20]
0069fa84: 48c08de5 str      ip, [sp, #0x48]
0069fa88: 4c008de5 str      r0, [sp, #0x4c]
0069fa8c: 0120a0e3 mov      r2, #1
0069fa90: 20309de5 ldr      r3, [sp, #0x20]
0069fa94: 34109de5 ldr      r1, [sp, #0x34]
0069fa98: 0000a0e3 mov      r0, #0
0069fa9c: 8340a0e1 lsl      r4, r3, #1
0069faa0: 836181e0 add      r6, r1, r3, lsl #3
0069faa4: 0050a0e1 mov      r5, r0
0069faa8: 40408de5 str      r4, [sp, #0x40]
0069faac: 40109de5 ldr      r1, [sp, #0x40]
0069fab0: 003096e5 ldr      r3, [r6]
0069fab4: 3c709de5 ldr      r7, [sp, #0x3c]
0069fab8: 054081e0 add      r4, r1, r5
0069fabc: 00c0a0e3 mov      ip, #0
0069fac0: 844287e0 add      r4, r7, r4, lsl #5
0069fac4: 047093e5 ldr      r7, [r3, #4]
0069fac8: 0410a0e1 mov      r1, r4
0069facc: 0130c7e3 bic      r3, r7, #1
0069fad0: 0338a0e1 lsl      r3, r3, #0x10
0069fad4: 2778a0e1 lsr      r7, r7, #0x10
0069fad8: 2338a0e1 lsr      r3, r3, #0x10
0069fadc: 9c758de5 str      r7, [sp, #0x59c]
0069fae0: 98358de5 str      r3, [sp, #0x598]
0069fae4: a383a0e1 lsr      r8, r3, #7
0069fae8: a371a0e1 lsr      r7, r3, #3
0069faec: 0fa003e2 and      sl, r3, #0xf
0069faf0: 8aa0a0e1 lsl      sl, sl, #1
0069faf4: 1e8008e2 and      r8, r8, #0x1e
0069faf8: 1e7007e2 and      r7, r7, #0x1e
0069fafc: 020913e3 tst      r3, #0x8000
0069fb00: 14a08de5 str      sl, [sp, #0x14]
0069fb04: 288288e1 orr      r8, r8, r8, lsr #4
0069fb08: 277287e1 orr      r7, r7, r7, lsr #4
0069fb0c: 1fa003e2 and      sl, r3, #0x1f
0069fb10: 53b5e4e7 ubfx     fp, r3, #0xa, #5
0069fb14: d392e4e7 ubfx     sb, r3, #5, #5
0069fb18: 0e00000a beq      #0x69fb58
0069fb1c: 00005ce3 cmp      ip, #0
0069fb20: 00b081e5 str      fp, [r1]
0069fb24: 000681e9 stmib    r1, {sb, sl}
0069fb28: 08309405 ldreq    r3, [r4, #8]
0069fb2c: 43328301 orreq    r3, r3, r3, asr #4
0069fb30: 08308405 streq    r3, [r4, #8]
0069fb34: 0f30a0e3 mov      r3, #0xf
0069fb38: 01005ce3 cmp      ip, #1
0069fb3c: 0c3081e5 str      r3, [r1, #0xc]
0069fb40: 101081e2 add      r1, r1, #0x10
0069fb44: 1200000a beq      #0x69fb94
0069fb48: 4cc09de5 ldr      ip, [sp, #0x4c]
0069fb4c: 04309ce5 ldr      r3, [ip, #4]
0069fb50: 01c0a0e3 mov      ip, #1
0069fb54: e2ffffea b        #0x69fae4
0069fb58: 008081e5 str      r8, [r1]
0069fb5c: 047081e5 str      r7, [r1, #4]
0069fb60: 14709de5 ldr      r7, [sp, #0x14]
0069fb64: 00005ce3 cmp      ip, #0
0069fb68: a335a0e1 lsr      r3, r3, #0xb
0069fb6c: 087081e5 str      r7, [r1, #8]
0069fb70: 087094e5 ldr      r7, [r4, #8]
0069fb74: 0e3003e2 and      r3, r3, #0xe
0069fb78: c7718701 orreq    r7, r7, r7, asr #3
0069fb7c: 47728711 orrne    r7, r7, r7, asr #4
0069fb80: 01005ce3 cmp      ip, #1
0069fb84: 087084e5 str      r7, [r4, #8]
0069fb88: 0c3081e5 str      r3, [r1, #0xc]
0069fb8c: 101081e2 add      r1, r1, #0x10
0069fb90: ecffff1a bne      #0x69fb48
0069fb94: 003096e5 ldr      r3, [r6]
0069fb98: 021093e8 ldm      r3, {r1, ip}
0069fb9c: 18309de5 ldr      r3, [sp, #0x18]
0069fba0: 01c00ce2 and      ip, ip, #1
0069fba4: 0ca013e0 ands     sl, r3, ip
0069fba8: 9500000a beq      #0x69fe04
0069fbac: 38409de5 ldr      r4, [sp, #0x38]
0069fbb0: 0090a0e3 mov      sb, #0
0069fbb4: 0970a0e1 mov      r7, sb
0069fbb8: 04b080e0 add      fp, r0, r4
0069fbbc: 1cc09de5 ldr      ip, [sp, #0x1c]
0069fbc0: 0e8089e0 add      r8, sb, lr
0069fbc4: 0030a0e3 mov      r3, #0
0069fbc8: 08808ce0 add      r8, ip, r8
0069fbcc: 008088e0 add      r8, r8, r0
0069fbd0: 03c0a0e1 mov      ip, r3
0069fbd4: 0ba089e0 add      sl, sb, fp
0069fbd8: 07402ce0 eor      r4, ip, r7
0069fbdc: 010014e3 tst      r4, #1
0069fbe0: 03400102 andeq    r4, r1, #3
0069fbe4: 032088e7 str      r2, [r8, r3]
0069fbe8: 03408a07 streq    r4, [sl, r3]
0069fbec: 043083e2 add      r3, r3, #4
0069fbf0: 2111a001 lsreq    r1, r1, #2
0069fbf4: 200053e3 cmp      r3, #0x20
0069fbf8: 01c08ce2 add      ip, ip, #1
0069fbfc: f5ffff1a bne      #0x69fbd8
0069fc00: 017087e2 add      r7, r7, #1
0069fc04: 040057e3 cmp      r7, #4
0069fc08: 409089e2 add      sb, sb, #0x40
0069fc0c: eaffff1a bne      #0x69fbbc
0069fc10: 48709de5 ldr      r7, [sp, #0x48]
0069fc14: 015085e2 add      r5, r5, #1
0069fc18: 020055e3 cmp      r5, #2
0069fc1c: 046086e2 add      r6, r6, #4
0069fc20: 070080e0 add      r0, r0, r7
0069fc24: a0ffff1a bne      #0x69faac
0069fc28: 20c09de5 ldr      ip, [sp, #0x20]
0069fc2c: 38009de5 ldr      r0, [sp, #0x38]
0069fc30: 01ec8ee2 add      lr, lr, #0x100
0069fc34: 01c08ce2 add      ip, ip, #1
0069fc38: 010c80e2 add      r0, r0, #0x100
0069fc3c: 020c5ee3 cmp      lr, #0x200
0069fc40: 20c08de5 str      ip, [sp, #0x20]
0069fc44: 38008de5 str      r0, [sp, #0x38]
0069fc48: 90ffff1a bne      #0x69fa90
0069fc4c: 34509de5 ldr      r5, [sp, #0x34]
0069fc50: 44709de5 ldr      r7, [sp, #0x44]
0069fc54: 0f0095e8 ldm      r5, {r0, r1, r2, r3}
0069fc58: 0f0087e8 stm      r7, {r0, r1, r2, r3}
0069fc5c: 60709de5 ldr      r7, [sp, #0x60]
0069fc60: 10409de5 ldr      r4, [sp, #0x10]
0069fc64: 50509de5 ldr      r5, [sp, #0x50]
0069fc68: 28c09de5 ldr      ip, [sp, #0x28]
0069fc6c: 3c009de5 ldr      r0, [sp, #0x3c]
0069fc70: 98109de5 ldr      r1, [sp, #0x98]
0069fc74: 74209de5 ldr      r2, [sp, #0x74]
0069fc78: 78309de5 ldr      r3, [sp, #0x78]
0069fc7c: 00c08de5 str      ip, [sp]
0069fc80: b0008de9 stmib    sp, {r4, r5, r7}
0069fc84: 39feffeb bl       #0x69f570
0069fc88: 28c09de5 ldr      ip, [sp, #0x28]
0069fc8c: 04408de5 str      r4, [sp, #4]
0069fc90: 5c409de5 ldr      r4, [sp, #0x5c]
0069fc94: 7c008de2 add      r0, sp, #0x7c
0069fc98: 0f0090e8 ldm      r0, {r0, r1, r2, r3}
0069fc9c: 00c08de5 str      ip, [sp]
0069fca0: 08508de5 str      r5, [sp, #8]
0069fca4: 0c408de5 str      r4, [sp, #0xc]
0069fca8: 30feffeb bl       #0x69f570
0069fcac: 28509de5 ldr      r5, [sp, #0x28]
0069fcb0: 94009de5 ldr      r0, [sp, #0x94]
0069fcb4: 000055e3 cmp      r5, #0
0069fcb8: 10709d15 ldrne    r7, [sp, #0x10]
0069fcbc: 10c09d05 ldreq    ip, [sp, #0x10]
0069fcc0: 0720e011 mvnne    r2, r7
0069fcc4: 0c20e001 mvneq    r2, ip
0069fcc8: 04200212 andne    r2, r2, #4
0069fccc: 07300712 andne    r3, r7, #7
0069fcd0: 02200202 andeq    r2, r2, #2
0069fcd4: 03300c02 andeq    r3, ip, #3
0069fcd8: 823083e1 orr      r3, r3, r2, lsl #1
0069fcdc: 034190e7 ldr      r4, [r0, r3, lsl #2]
0069fce0: 000054e3 cmp      r4, #0
0069fce4: 7100000a beq      #0x69feb0
0069fce8: 28709de5 ldr      r7, [sp, #0x28]
0069fcec: 000057e3 cmp      r7, #0
0069fcf0: 7600000a beq      #0x69fed0
0069fcf4: a8c09de5 ldr      ip, [sp, #0xa8]
0069fcf8: 0c2023e0 eor      r2, r3, ip
0069fcfc: 012012e2 ands     r2, r2, #1
0069fd00: 7d00000a beq      #0x69fefc
0069fd04: 010054e3 cmp      r4, #1
0069fd08: 9400000a beq      #0x69ff60
0069fd0c: 020054e3 cmp      r4, #2
0069fd10: 8200000a beq      #0x69ff20
0069fd14: b8409de5 ldr      r4, [sp, #0xb8]
0069fd18: bc109de5 ldr      r1, [sp, #0xbc]
0069fd1c: b4509de5 ldr      r5, [sp, #0xb4]
0069fd20: 032191e7 ldr      r2, [r1, r3, lsl #2]
0069fd24: 033194e7 ldr      r3, [r4, r3, lsl #2]
0069fd28: 0040a0e3 mov      r4, #0
0069fd2c: 022195e7 ldr      r2, [r5, r2, lsl #2]
0069fd30: 03c195e7 ldr      ip, [r5, r3, lsl #2]
0069fd34: 02c08ce0 add      ip, ip, r2
0069fd38: 01c08ce2 add      ip, ip, #1
0069fd3c: accf8ce0 add      ip, ip, ip, lsr #31
0069fd40: ccc0a0e1 asr      ip, ip, #1
0069fd44: 9c509de5 ldr      r5, [sp, #0x9c]
0069fd48: 5c609de5 ldr      r6, [sp, #0x5c]
0069fd4c: 60709de5 ldr      r7, [sp, #0x60]
0069fd50: 0030a0e3 mov      r3, #0
0069fd54: 032097e7 ldr      r2, [r7, r3]
0069fd58: 030096e7 ldr      r0, [r6, r3]
0069fd5c: 8211a0e1 lsl      r1, r2, #3
0069fd60: 002062e0 rsb      r2, r2, r0
0069fd64: 921c22e0 mla      r2, r2, ip, r1
0069fd68: c221a0e1 asr      r2, r2, #3
0069fd6c: 032085e7 str      r2, [r5, r3]
0069fd70: 043083e2 add      r3, r3, #4
0069fd74: 100053e3 cmp      r3, #0x10
0069fd78: f5ffff1a bne      #0x69fd54
0069fd7c: 70309de5 ldr      r3, [sp, #0x70]
0069fd80: 000054e3 cmp      r4, #0
0069fd84: 10409de5 ldr      r4, [sp, #0x10]
0069fd88: c8559de5 ldr      r5, [sp, #0x5c8]
0069fd8c: 4c159de5 ldr      r1, [sp, #0x54c]
0069fd90: 48059de5 ldr      r0, [sp, #0x548]
0069fd94: 042083e0 add      r2, r3, r4
0069fd98: 023185e0 add      r3, r5, r2, lsl #2
0069fd9c: 00c0a013 movne    ip, #0
0069fda0: 54c5dd05 ldrbeq   ip, [sp, #0x554]
0069fda4: 54c58d15 strne    ip, [sp, #0x554]
0069fda8: 0201c5e7 strb     r0, [r5, r2, lsl #2]
0069fdac: 0110c3e5 strb     r1, [r3, #1]
0069fdb0: 6c709de5 ldr      r7, [sp, #0x6c]
0069fdb4: 50259de5 ldr      r2, [sp, #0x550]
0069fdb8: 014084e2 add      r4, r4, #1
0069fdbc: 070054e1 cmp      r4, r7
0069fdc0: 10408de5 str      r4, [sp, #0x10]
0069fdc4: 03c0c3e5 strb     ip, [r3, #3]
0069fdc8: 0220c3e5 strb     r2, [r3, #2]
0069fdcc: f0feff1a bne      #0x69f994
0069fdd0: 50c09de5 ldr      ip, [sp, #0x50]
0069fdd4: 70109de5 ldr      r1, [sp, #0x70]
0069fdd8: b0009de5 ldr      r0, [sp, #0xb0]
0069fddc: 6c209de5 ldr      r2, [sp, #0x6c]
0069fde0: 01c08ce2 add      ip, ip, #1
0069fde4: 00005ce1 cmp      ip, r0
0069fde8: 021081e0 add      r1, r1, r2
0069fdec: 50c08de5 str      ip, [sp, #0x50]
0069fdf0: 70108de5 str      r1, [sp, #0x70]
0069fdf4: 8cfeff1a bne      #0x69f82c
0069fdf8: 69df8de2 add      sp, sp, #0x1a4
0069fdfc: 01db8de2 add      sp, sp, #0x400
0069fe00: f08fbde8 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0069fe04: 18309de5 ldr      r3, [sp, #0x18]
0069fe08: 000053e3 cmp      r3, #0
0069fe0c: 1300001a bne      #0x69fe60
0069fe10: 1c909de5 ldr      sb, [sp, #0x1c]
0069fe14: 64b09de5 ldr      fp, [sp, #0x64]
0069fe18: 03a0a0e1 mov      sl, r3
0069fe1c: 0a808ee0 add      r8, lr, sl
0069fe20: 08708be0 add      r7, fp, r8
0069fe24: 088089e0 add      r8, sb, r8
0069fe28: 008088e0 add      r8, r8, r0
0069fe2c: 007087e0 add      r7, r7, r0
0069fe30: 0030a0e3 mov      r3, #0
0069fe34: 034001e2 and      r4, r1, #3
0069fe38: 03c088e7 str      ip, [r8, r3]
0069fe3c: 034087e7 str      r4, [r7, r3]
0069fe40: 043083e2 add      r3, r3, #4
0069fe44: 100053e3 cmp      r3, #0x10
0069fe48: 2111a0e1 lsr      r1, r1, #2
0069fe4c: f8ffff1a bne      #0x69fe34
0069fe50: 40a08ae2 add      sl, sl, #0x40
0069fe54: 010c5ae3 cmp      sl, #0x100
0069fe58: efffff1a bne      #0x69fe1c
0069fe5c: 6bffffea b        #0x69fc10
0069fe60: 38409de5 ldr      r4, [sp, #0x38]
0069fe64: 1c909de5 ldr      sb, [sp, #0x1c]
0069fe68: 047080e0 add      r7, r0, r4
0069fe6c: 0a808ee0 add      r8, lr, sl
0069fe70: 088089e0 add      r8, sb, r8
0069fe74: 008088e0 add      r8, r8, r0
0069fe78: 0030a0e3 mov      r3, #0
0069fe7c: 014011e2 ands     r4, r1, #1
0069fe80: 0340a013 movne    r4, #3
0069fe84: 03c088e7 str      ip, [r8, r3]
0069fe88: 034087e7 str      r4, [r7, r3]
0069fe8c: 043083e2 add      r3, r3, #4
0069fe90: 200053e3 cmp      r3, #0x20
0069fe94: a110a0e1 lsr      r1, r1, #1
0069fe98: f7ffff1a bne      #0x69fe7c
0069fe9c: 40a08ae2 add      sl, sl, #0x40
0069fea0: 010c5ae3 cmp      sl, #0x100
0069fea4: 407087e2 add      r7, r7, #0x40
0069fea8: efffff1a bne      #0x69fe6c
0069feac: 57ffffea b        #0x69fc10
0069feb0: a4109de5 ldr      r1, [sp, #0xa4]
0069feb4: 64509de5 ldr      r5, [sp, #0x64]
0069feb8: 032181e0 add      r2, r1, r3, lsl #2
0069febc: 18319fe5 ldr      r3, [pc, #0x118]
0069fec0: 022095e7 ldr      r2, [r5, r2]
0069fec4: 03308fe0 add      r3, pc, r3
0069fec8: 02c193e7 ldr      ip, [r3, r2, lsl #2]
0069fecc: 9cffffea b        #0x69fd44
0069fed0: a4709de5 ldr      r7, [sp, #0xa4]
0069fed4: 64c09de5 ldr      ip, [sp, #0x64]
0069fed8: ac009de5 ldr      r0, [sp, #0xac]
0069fedc: 033187e0 add      r3, r7, r3, lsl #2
0069fee0: 03409ce7 ldr      r4, [ip, r3]
0069fee4: 043180e0 add      r3, r0, r4, lsl #2
0069fee8: 10c093e5 ldr      ip, [r3, #0x10]
0069feec: 020054e3 cmp      r4, #2
0069fef0: 0040a013 movne    r4, #0
0069fef4: 0140a003 moveq    r4, #1
0069fef8: 91ffffea b        #0x69fd44
0069fefc: a4009de5 ldr      r0, [sp, #0xa4]
0069ff00: 64409de5 ldr      r4, [sp, #0x64]
0069ff04: 031180e0 add      r1, r0, r3, lsl #2
0069ff08: d0309fe5 ldr      r3, [pc, #0xd0]
0069ff0c: 011094e7 ldr      r1, [r4, r1]
0069ff10: 0240a0e1 mov      r4, r2
0069ff14: 03308fe0 add      r3, pc, r3
0069ff18: 01c193e7 ldr      ip, [r3, r1, lsl #2]
0069ff1c: 88ffffea b        #0x69fd44
0069ff20: a4c09de5 ldr      ip, [sp, #0xa4]
0069ff24: 64709de5 ldr      r7, [sp, #0x64]
0069ff28: 011083e2 add      r1, r3, #1
0069ff2c: 013043e2 sub      r3, r3, #1
0069ff30: 0c2087e0 add      r2, r7, ip
0069ff34: 033192e7 ldr      r3, [r2, r3, lsl #2]
0069ff38: c0009de5 ldr      r0, [sp, #0xc0]
0069ff3c: 011192e7 ldr      r1, [r2, r1, lsl #2]
0069ff40: 0040a0e3 mov      r4, #0
0069ff44: 03c190e7 ldr      ip, [r0, r3, lsl #2]
0069ff48: 012190e7 ldr      r2, [r0, r1, lsl #2]
0069ff4c: 02c08ce0 add      ip, ip, r2
0069ff50: 01c08ce2 add      ip, ip, #1
0069ff54: accf8ce0 add      ip, ip, ip, lsr #31
0069ff58: ccc0a0e1 asr      ip, ip, #1
0069ff5c: 78ffffea b        #0x69fd44
0069ff60: b8509de5 ldr      r5, [sp, #0xb8]
0069ff64: bc709de5 ldr      r7, [sp, #0xbc]
0069ff68: 64209de5 ldr      r2, [sp, #0x64]
0069ff6c: a4409de5 ldr      r4, [sp, #0xa4]
0069ff70: 030195e7 ldr      r0, [r5, r3, lsl #2]
0069ff74: c4509de5 ldr      r5, [sp, #0xc4]
0069ff78: 03c197e7 ldr      ip, [r7, r3, lsl #2]
0069ff7c: 041082e0 add      r1, r2, r4
0069ff80: 012043e2 sub      r2, r3, #1
0069ff84: 024191e7 ldr      r4, [r1, r2, lsl #2]
0069ff88: 012083e2 add      r2, r3, #1
0069ff8c: 05308fe0 add      r3, pc, r5
0069ff90: 022191e7 ldr      r2, [r1, r2, lsl #2]
0069ff94: 001193e7 ldr      r1, [r3, r0, lsl #2]
0069ff98: 0c0193e7 ldr      r0, [r3, ip, lsl #2]
0069ff9c: 04c193e7 ldr      ip, [r3, r4, lsl #2]
0069ffa0: 022193e7 ldr      r2, [r3, r2, lsl #2]
0069ffa4: 003081e0 add      r3, r1, r0
0069ffa8: 023083e2 add      r3, r3, #2
0069ffac: 0c3083e0 add      r3, r3, ip
0069ffb0: 023083e0 add      r3, r3, r2
0069ffb4: 000053e3 cmp      r3, #0
0069ffb8: 03c083e2 add      ip, r3, #3
0069ffbc: 03c0a0a1 movge    ip, r3
0069ffc0: 4cc1a0e1 asr      ip, ip, #2
0069ffc4: 0040a0e3 mov      r4, #0
0069ffc8: 5dffffea b        #0x69fd44
0069ffcc: 04b72400 eoreq    fp, r4, r4, lsl #14
0069ffd0: 6caf2400 eoreq    sl, r4, ip, ror #30

# _ZNK6glitch5video15CImageLoaderPVR17loadTextureHeaderEPNS_2io9IReadFileERNS0_12STextureDescE, ELF VA 0x605b10, 1056 bytes
00605b10: f0402de9 push     {r4, r5, r6, r7, lr}
00605b14: 3cd04de2 sub      sp, sp, #0x3c
00605b18: 0100a0e1 mov      r0, r1
00605b1c: 0150a0e1 mov      r5, r1
00605b20: 0240a0e1 mov      r4, r2
00605b24: 0d10a0e1 mov      r1, sp
00605b28: 37208de2 add      r2, sp, #0x37
00605b2c: 5cffffeb bl       #0x6058a4
00605b30: e4639fe5 ldr      r6, [pc, #0x3e4]
00605b34: 000050e3 cmp      r0, #0
00605b38: 06608fe0 add      r6, pc, r6
00605b3c: 0100001a bne      #0x605b48
00605b40: 3cd08de2 add      sp, sp, #0x3c
00605b44: f080bde8 pop      {r4, r5, r6, r7, pc}
00605b48: 10309de5 ldr      r3, [sp, #0x10]
00605b4c: 3770dde5 ldrb     r7, [sp, #0x37]
00605b50: 010a13e3 tst      r3, #0x1000
00605b54: 0220a013 movne    r2, #2
00605b58: 00208415 strne    r2, [r4]
00605b5c: 7800000a beq      #0x605d44
00605b60: 000094e5 ldr      r0, [r4]
00605b64: 04209de5 ldr      r2, [sp, #4]
00605b68: 08109de5 ldr      r1, [sp, #8]
00605b6c: 010050e3 cmp      r0, #1
00605b70: 0000a0e3 mov      r0, #0
00605b74: 142084e5 str      r2, [r4, #0x14]
00605b78: 080084e5 str      r0, [r4, #8]
00605b7c: 101084e5 str      r1, [r4, #0x10]
00605b80: 30209d05 ldreq    r2, [sp, #0x30]
00605b84: 0120a013 movne    r2, #1
00605b88: 5334e0e7 ubfx     r3, r3, #8, #1
00605b8c: 182084e5 str      r2, [r4, #0x18]
00605b90: 1c30c4e5 strb     r3, [r4, #0x1c]
00605b94: 003095e5 ldr      r3, [r5]
00605b98: 0500a0e1 mov      r0, r5
00605b9c: 0fe0a0e1 mov      lr, pc
00605ba0: 20f093e5 ldr      pc, [r3, #0x20]
00605ba4: 003094e5 ldr      r3, [r4]
00605ba8: 000057e3 cmp      r7, #0
00605bac: 14209de5 ldr      r2, [sp, #0x14]
00605bb0: 0870a013 movne    r7, #8
00605bb4: 020053e3 cmp      r3, #2
00605bb8: 0630a003 moveq    r3, #6
00605bbc: 0130a013 movne    r3, #1
00605bc0: 920303e0 mul      r3, r2, r3
00605bc4: 340040e2 sub      r0, r0, #0x34
00605bc8: 007067e0 rsb      r7, r7, r0
00605bcc: 030057e1 cmp      r7, r3
00605bd0: 5f00001a bne      #0x605d54
00605bd4: 10309de5 ldr      r3, [sp, #0x10]
00605bd8: ff2003e2 and      r2, r3, #0xff
00605bdc: 560052e3 cmp      r2, #0x56
00605be0: 02f18f90 addls    pc, pc, r2, lsl #2
00605be4: 7c0000ea b        #0x605ddc
00605be8: 870000ea b        #0x605e0c
00605bec: 890000ea b        #0x605e18
00605bf0: 8b0000ea b        #0x605e24
00605bf4: 780000ea b        #0x605ddc
00605bf8: 8c0000ea b        #0x605e30
00605bfc: 8e0000ea b        #0x605e3c
00605c00: 750000ea b        #0x605ddc
00605c04: 8f0000ea b        #0x605e48
00605c08: 910000ea b        #0x605e54
00605c0c: 720000ea b        #0x605ddc
00605c10: 710000ea b        #0x605ddc
00605c14: 700000ea b        #0x605ddc
00605c18: 900000ea b        #0x605e60
00605c1c: 940000ea b        #0x605e74
00605c20: 6d0000ea b        #0x605ddc
00605c24: 6c0000ea b        #0x605ddc
00605c28: 960000ea b        #0x605e88
00605c2c: 980000ea b        #0x605e94
00605c30: 9a0000ea b        #0x605ea0
00605c34: 7a0000ea b        #0x605e24
00605c38: 670000ea b        #0x605ddc
00605c3c: 7b0000ea b        #0x605e30
00605c40: 800000ea b        #0x605e48
00605c44: 820000ea b        #0x605e54
00605c48: 840000ea b        #0x605e60
00605c4c: 880000ea b        #0x605e74
00605c50: 790000ea b        #0x605e3c
00605c54: 600000ea b        #0x605ddc
00605c58: 5f0000ea b        #0x605ddc
00605c5c: 5e0000ea b        #0x605ddc
00605c60: 5d0000ea b        #0x605ddc
00605c64: 5c0000ea b        #0x605ddc
00605c68: 8f0000ea b        #0x605eac
00605c6c: 930000ea b        #0x605ec0
00605c70: 920000ea b        #0x605ec0
00605c74: 940000ea b        #0x605ecc
00605c78: 930000ea b        #0x605ecc
00605c7c: 560000ea b        #0x605ddc
00605c80: 550000ea b        #0x605ddc
00605c84: 540000ea b        #0x605ddc
00605c88: 530000ea b        #0x605ddc
00605c8c: 520000ea b        #0x605ddc
00605c90: 900000ea b        #0x605ed8
00605c94: 500000ea b        #0x605ddc
00605c98: 4f0000ea b        #0x605ddc
00605c9c: 4e0000ea b        #0x605ddc
00605ca0: 4d0000ea b        #0x605ddc
00605ca4: 4c0000ea b        #0x605ddc
00605ca8: 4b0000ea b        #0x605ddc
00605cac: 4a0000ea b        #0x605ddc
00605cb0: 490000ea b        #0x605ddc
00605cb4: 480000ea b        #0x605ddc
00605cb8: 470000ea b        #0x605ddc
00605cbc: 460000ea b        #0x605ddc
00605cc0: 450000ea b        #0x605ddc
00605cc4: 440000ea b        #0x605ddc
00605cc8: 430000ea b        #0x605ddc
00605ccc: 840000ea b        #0x605ee4
00605cd0: 410000ea b        #0x605ddc
00605cd4: 850000ea b        #0x605ef0
00605cd8: 3f0000ea b        #0x605ddc
00605cdc: 3e0000ea b        #0x605ddc
00605ce0: 3d0000ea b        #0x605ddc
00605ce4: 3c0000ea b        #0x605ddc
00605ce8: 3b0000ea b        #0x605ddc
00605cec: 3a0000ea b        #0x605ddc
00605cf0: 390000ea b        #0x605ddc
00605cf4: 380000ea b        #0x605ddc
00605cf8: 370000ea b        #0x605ddc
00605cfc: 360000ea b        #0x605ddc
00605d00: 350000ea b        #0x605ddc
00605d04: 340000ea b        #0x605ddc
00605d08: 330000ea b        #0x605ddc
00605d0c: 320000ea b        #0x605ddc
00605d10: 310000ea b        #0x605ddc
00605d14: 300000ea b        #0x605ddc
00605d18: 2f0000ea b        #0x605ddc
00605d1c: 2e0000ea b        #0x605ddc
00605d20: 2d0000ea b        #0x605ddc
00605d24: 2c0000ea b        #0x605ddc
00605d28: 730000ea b        #0x605efc
00605d2c: 2a0000ea b        #0x605ddc
00605d30: 290000ea b        #0x605ddc
00605d34: 730000ea b        #0x605f08
00605d38: 270000ea b        #0x605ddc
00605d3c: 260000ea b        #0x605ddc
00605d40: 0e0000ea b        #0x605d80
00605d44: 012913e2 ands     r2, r3, #0x4000
00605d48: 0120a013 movne    r2, #1
00605d4c: 002084e5 str      r2, [r4]
00605d50: 82ffffea b        #0x605b60
00605d54: 003095e5 ldr      r3, [r5]
00605d58: 0500a0e1 mov      r0, r5
00605d5c: 0fe0a0e1 mov      lr, pc
00605d60: 28f093e5 ldr      pc, [r3, #0x28]
00605d64: b4119fe5 ldr      r1, [pc, #0x1b4]
00605d68: 0020a0e1 mov      r2, r0
00605d6c: 0300a0e3 mov      r0, #3
00605d70: 01108fe0 add      r1, pc, r1
00605d74: ae1400eb bl       #0x60b034
00605d78: 0000a0e3 mov      r0, #0
00605d7c: 6fffffea b        #0x605b40
00605d80: 1d20a0e3 mov      r2, #0x1d
00605d84: 042084e5 str      r2, [r4, #4]
00605d88: 020c13e3 tst      r3, #0x200
00605d8c: 6000000a beq      #0x605f14
00605d90: 043094e5 ldr      r3, [r4, #4]
00605d94: 2820a0e3 mov      r2, #0x28
00605d98: 920303e0 mul      r3, r2, r3
00605d9c: 80219fe5 ldr      r2, [pc, #0x180]
00605da0: 022096e7 ldr      r2, [r6, r2]
00605da4: 034092e7 ldr      r4, [r2, r3]
00605da8: 084014e2 ands     r4, r4, #8
00605dac: 5800001a bne      #0x605f14
00605db0: 003095e5 ldr      r3, [r5]
00605db4: 0500a0e1 mov      r0, r5
00605db8: 0fe0a0e1 mov      lr, pc
00605dbc: 28f093e5 ldr      pc, [r3, #0x28]
00605dc0: 60119fe5 ldr      r1, [pc, #0x160]
00605dc4: 0020a0e1 mov      r2, r0
00605dc8: 0300a0e3 mov      r0, #3
00605dcc: 01108fe0 add      r1, pc, r1
00605dd0: 971400eb bl       #0x60b034
00605dd4: 0400a0e1 mov      r0, r4
00605dd8: 58ffffea b        #0x605b40
00605ddc: 003095e5 ldr      r3, [r5]
00605de0: 0500a0e1 mov      r0, r5
00605de4: 0fe0a0e1 mov      lr, pc
00605de8: 28f093e5 ldr      pc, [r3, #0x28]
00605dec: 38119fe5 ldr      r1, [pc, #0x138]
00605df0: 0020a0e1 mov      r2, r0
00605df4: 1030dde5 ldrb     r3, [sp, #0x10]
00605df8: 0300a0e3 mov      r0, #3
00605dfc: 01108fe0 add      r1, pc, r1
00605e00: 8b1400eb bl       #0x60b034
00605e04: 0000a0e3 mov      r0, #0
00605e08: 4cffffea b        #0x605b40
00605e0c: 0620a0e3 mov      r2, #6
00605e10: 042084e5 str      r2, [r4, #4]
00605e14: dbffffea b        #0x605d88
00605e18: 0820a0e3 mov      r2, #8
00605e1c: 042084e5 str      r2, [r4, #4]
00605e20: d8ffffea b        #0x605d88
00605e24: 0520a0e3 mov      r2, #5
00605e28: 042084e5 str      r2, [r4, #4]
00605e2c: d5ffffea b        #0x605d88
00605e30: 0a20a0e3 mov      r2, #0xa
00605e34: 042084e5 str      r2, [r4, #4]
00605e38: d2ffffea b        #0x605d88
00605e3c: 0d20a0e3 mov      r2, #0xd
00605e40: 042084e5 str      r2, [r4, #4]
00605e44: cfffffea b        #0x605d88
00605e48: 0020a0e3 mov      r2, #0
00605e4c: 042084e5 str      r2, [r4, #4]
00605e50: ccffffea b        #0x605d88
00605e54: 0420a0e3 mov      r2, #4
00605e58: 042084e5 str      r2, [r4, #4]
00605e5c: c9ffffea b        #0x605d88
00605e60: 020913e3 tst      r3, #0x8000
00605e64: 1920a013 movne    r2, #0x19
00605e68: 1820a003 moveq    r2, #0x18
00605e6c: 042084e5 str      r2, [r4, #4]
00605e70: c4ffffea b        #0x605d88
00605e74: 020913e3 tst      r3, #0x8000
00605e78: 1b20a013 movne    r2, #0x1b
00605e7c: 1a20a003 moveq    r2, #0x1a
00605e80: 042084e5 str      r2, [r4, #4]
00605e84: bfffffea b        #0x605d88
00605e88: 0720a0e3 mov      r2, #7
00605e8c: 042084e5 str      r2, [r4, #4]
00605e90: bcffffea b        #0x605d88
00605e94: 0920a0e3 mov      r2, #9
00605e98: 042084e5 str      r2, [r4, #4]
00605e9c: b9ffffea b        #0x605d88
00605ea0: 0e20a0e3 mov      r2, #0xe
00605ea4: 042084e5 str      r2, [r4, #4]
00605ea8: b6ffffea b        #0x605d88
00605eac: 020913e3 tst      r3, #0x8000
00605eb0: 1220a013 movne    r2, #0x12
00605eb4: 1120a003 moveq    r2, #0x11
00605eb8: 042084e5 str      r2, [r4, #4]
00605ebc: b1ffffea b        #0x605d88
00605ec0: 1320a0e3 mov      r2, #0x13
00605ec4: 042084e5 str      r2, [r4, #4]
00605ec8: aeffffea b        #0x605d88
00605ecc: 1420a0e3 mov      r2, #0x14
00605ed0: 042084e5 str      r2, [r4, #4]
00605ed4: abffffea b        #0x605d88
00605ed8: 1020a0e3 mov      r2, #0x10
00605edc: 042084e5 str      r2, [r4, #4]
00605ee0: a8ffffea b        #0x605d88
00605ee4: 0220a0e3 mov      r2, #2
00605ee8: 042084e5 str      r2, [r4, #4]
00605eec: a5ffffea b        #0x605d88
00605ef0: 0120a0e3 mov      r2, #1
00605ef4: 042084e5 str      r2, [r4, #4]
00605ef8: a2ffffea b        #0x605d88
00605efc: 1f20a0e3 mov      r2, #0x1f
00605f00: 042084e5 str      r2, [r4, #4]
00605f04: 9fffffea b        #0x605d88
00605f08: 1e20a0e3 mov      r2, #0x1e
00605f0c: 042084e5 str      r2, [r4, #4]
00605f10: 9cffffea b        #0x605d88
00605f14: 0100a0e3 mov      r0, #1
00605f18: 08ffffea b        #0x605b40
00605f1c: 58ef3800 eorseq   lr, r8, r8, asr pc
00605f20: 08ec2d00 eoreq    lr, sp, r8, lsl #24
00605f24: 341f0000 andeq    r1, r0, r4, lsr pc
00605f28: 0cec2d00 eoreq    lr, sp, ip, lsl #24
00605f2c: aceb2d00 eoreq    lr, sp, ip, lsr #23
