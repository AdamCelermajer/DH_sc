
# _ZN10AISDefault6OnInitEv
003dbe78: bx       lr

# _ZN6CharAI9SetScriptI15AISPlayerIPhoneEEvv
003ccfe4: push     {r4, r5, r6, r7, lr}
003ccfe8: ldr      r3, [r0, #4]
003ccfec: ldr      r5, [pc, #0x108]
003ccff0: sub      sp, sp, #0xc
003ccff4: cmp      r3, #0
003ccff8: mov      r4, r0
003ccffc: add      r5, pc, r5
003cd000: beq      #0x3cd0a8
003cd004: ldr      r3, [r4, #0x20]
003cd008: cmp      r3, #0
003cd00c: beq      #0x3cd044
003cd010: ldr      r3, [r4]
003cd014: mov      r0, r4
003cd018: mov      lr, pc
003cd01c: ldr      pc, [r3, #0x14]
003cd020: ldr      r3, [r4, #0x20]
003cd024: cmp      r3, #0
003cd028: beq      #0x3cd044
003cd02c: mov      r0, r3
003cd030: ldr      r3, [r3]
003cd034: mov      lr, pc
003cd038: ldr      pc, [r3, #4]
003cd03c: mov      r3, #0
003cd040: str      r3, [r4, #0x20]
003cd044: mov      r1, #0
003cd048: mov      r0, #0xd8
003cd04c: bl       #0x310570
003cd050: mov      r1, #1
003cd054: mov      r6, r0
003cd058: bl       #0x3d8fb0
003cd05c: ldr      r3, [pc, #0x9c]
003cd060: mov      r7, #0
003cd064: mov      r0, r6
003cd068: ldr      r3, [r5, r3]
003cd06c: str      r7, [r6, #0xb8]
003cd070: str      r7, [r6, #0xbc]
003cd074: add      r3, r3, #8
003cd078: str      r7, [r6, #0xc0]
003cd07c: str      r3, [r0], #0xc4
003cd080: bl       #0x3ccf9c
003cd084: ldr      r3, [pc, #0x78]
003cd088: str      r7, [r6, #0xd4]
003cd08c: str      r7, [r6, #0xd0]
003cd090: ldr      r3, [r5, r3]
003cd094: add      r3, r3, #8
003cd098: str      r3, [r6]
003cd09c: str      r6, [r4, #0x20]
003cd0a0: add      sp, sp, #0xc
003cd0a4: pop      {r4, r5, r6, r7, pc}
003cd0a8: ldr      r2, [pc, #0x58]
003cd0ac: ldr      r2, [r5, r2]
003cd0b0: ldr      r2, [r2]
003cd0b4: cmp      r2, #2
003cd0b8: streq    r3, [r3]
003cd0bc: beq      #0x3cd004
003cd0c0: cmp      r2, #1
003cd0c4: bne      #0x3cd004
003cd0c8: ldr      r0, [pc, #0x3c]
003cd0cc: ldr      r1, [pc, #0x3c]
003cd0d0: ldr      r2, [pc, #0x3c]
003cd0d4: ldr      r0, [r5, r0]
003cd0d8: ldr      r3, [pc, #0x38]
003cd0dc: movw     ip, #0x2a1
003cd0e0: add      r1, pc, r1
003cd0e4: add      r2, pc, r2
003cd0e8: add      r3, pc, r3
003cd0ec: add      r0, r0, #0xa8
003cd0f0: str      ip, [sp]
003cd0f4: bl       #0x30e004
003cd0f8: b        #0x3cd004

# _ZN6CharAI6OnInitEv
003d12b0: push     {r4, r5, r6, r7, lr}
003d12b4: ldr      r3, [r0, #4]
003d12b8: sub      sp, sp, #0xc
003d12bc: mov      r4, r0
003d12c0: mov      r0, r3
003d12c4: ldr      r3, [r3]
003d12c8: mov      lr, pc
003d12cc: ldr      pc, [r3, #0x34]
003d12d0: ldr      r5, [pc, #0xe8]
003d12d4: cmp      r0, #0
003d12d8: add      r5, pc, r5
003d12dc: bne      #0x3d139c
003d12e0: ldr      r1, [r4, #0x10]
003d12e4: cmn      r1, #1
003d12e8: beq      #0x3d12f8
003d12ec: ldr      r0, [r4, #4]
003d12f0: add      r0, r0, #0x3b4
003d12f4: bl       #0x3db2d8
003d12f8: ldr      r6, [pc, #0xc4]
003d12fc: ldr      r1, [pc, #0xc4]
003d1300: ldr      r2, [pc, #0xc4]
003d1304: ldr      r3, [r5, r6]
003d1308: add      r1, pc, r1
003d130c: add      r2, pc, r2
003d1310: ldr      r0, [r3, #0x2c]
003d1314: ldr      r7, [r4, #4]
003d1318: bl       #0x4c4bdc
003d131c: add      r7, r7, #0x3b4
003d1320: mov      r1, r0
003d1324: mov      ip, #0
003d1328: mov      r0, r7
003d132c: mvn      r2, #0
003d1330: mov      r3, #0x33
003d1334: str      ip, [sp]
003d1338: bl       #0x3dbe24
003d133c: ldr      r1, [r4, #0x14]
003d1340: str      r0, [r4, #0x10]
003d1344: cmn      r1, #1
003d1348: beq      #0x3d1358
003d134c: ldr      r0, [r4, #4]
003d1350: add      r0, r0, #0x3b4
003d1354: bl       #0x3db2d8
003d1358: ldr      r3, [r5, r6]
003d135c: ldr      r1, [pc, #0x6c]
003d1360: ldr      r2, [pc, #0x6c]
003d1364: ldr      r0, [r3, #0x2c]
003d1368: add      r1, pc, r1
003d136c: add      r2, pc, r2
003d1370: ldr      r5, [r4, #4]
003d1374: bl       #0x4c4bdc
003d1378: add      r5, r5, #0x3b4
003d137c: mov      r1, r0
003d1380: mov      ip, #0
003d1384: mov      r0, r5
003d1388: mvn      r2, #0
003d138c: mov      r3, #0x34
003d1390: str      ip, [sp]
003d1394: bl       #0x3dbe24
003d1398: str      r0, [r4, #0x14]
003d139c: ldr      r3, [r4, #0x20]
003d13a0: cmp      r3, #0
003d13a4: beq      #0x3d13b8
003d13a8: mov      r0, r3
003d13ac: ldr      r3, [r3]
003d13b0: mov      lr, pc
003d13b4: ldr      pc, [r3, #8]
003d13b8: add      sp, sp, #0xc
003d13bc: pop      {r4, r5, r6, r7, pc}
003d13c0: ldrheq   r3, [ip], #-0x78
003d13c4: strdeq   r3, r4, [r0], -r4
003d13c8: subeq    r0, pc, r8, asr #8
003d13cc: subeq    r4, pc, ip, ror #2
003d13d0: subeq    r0, pc, r8, ror #7
003d13d4: subeq    r4, pc, r4, lsl r1

# _ZN6CharAIC1Ev
003ced50: ldr      r3, [pc, #0x14c]
003ced54: ldr      r2, [pc, #0x14c]
003ced58: push     {r4, r5, lr}
003ced5c: add      r3, pc, r3
003ced60: ldr      r2, [r3, r2]
003ced64: mov      r4, r0
003ced68: mov      r1, #0
003ced6c: add      r2, r2, #8
003ced70: str      r2, [r4]
003ced74: ldr      r2, [pc, #0x130]
003ced78: mov      r0, #1
003ced7c: mvn      ip, #0
003ced80: mov      r5, r4
003ced84: strb     r0, [r4, #0x55]
003ced88: str      r1, [r4, #8]
003ced8c: str      r1, [r4, #0xc]
003ced90: strb     r1, [r4, #0x18]
003ced94: str      r1, [r4, #0x1c]
003ced98: str      r1, [r4, #0x20]
003ced9c: strb     r1, [r4, #0x24]
003ceda0: str      r1, [r4, #0x28]
003ceda4: strb     r1, [r4, #0x2c]
003ceda8: str      r1, [r4, #0x30]
003cedac: str      r1, [r4, #0x34]
003cedb0: str      r1, [r4, #0x3c]
003cedb4: str      r1, [r4, #0x40]
003cedb8: str      r1, [r4, #0x44]
003cedbc: strb     r1, [r4, #0x49]
003cedc0: strb     r0, [r4, #0x4a]
003cedc4: strb     r0, [r4, #0x4b]
003cedc8: strb     r1, [r4, #0x4c]
003cedcc: strb     r0, [r4, #0x4d]
003cedd0: str      r1, [r4, #0x50]
003cedd4: strb     r0, [r4, #0x54]
003cedd8: str      r1, [r4, #0x58]
003ceddc: mov      r0, r4
003cede0: str      r1, [r4, #0x60]
003cede4: str      ip, [r4, #0x10]
003cede8: str      ip, [r4, #0x14]
003cedec: str      ip, [r4, #0x38]
003cedf0: strb     r1, [r5, #0x5c]!
003cedf4: str      r5, [r4, #0x68]
003cedf8: str      r5, [r4, #0x64]
003cedfc: str      r1, [r4, #0x6c]
003cee00: str      r1, [r4, #0x80]
003cee04: strb     r1, [r0, #0x7c]!
003cee08: ldr      r5, [r3, r2]
003cee0c: mov      r2, r4
003cee10: str      r0, [r4, #0x88]
003cee14: str      r0, [r4, #0x84]
003cee18: str      r1, [r4, #0x8c]
003cee1c: str      r1, [r4, #0x98]
003cee20: add      r0, r4, #0xac
003cee24: strb     r1, [r2, #0x94]!
003cee28: str      r2, [r4, #0xa0]
003cee2c: str      r0, [r4, #0xb0]
003cee30: str      ip, [r4, #0xcc]
003cee34: strb     r1, [r4, #0xd1]
003cee38: str      r2, [r4, #0x9c]
003cee3c: str      r1, [r4, #0xa4]
003cee40: str      r0, [r4, #0xac]
003cee44: str      r1, [r4, #0xb4]
003cee48: str      r1, [r4, #0xb8]
003cee4c: str      r1, [r4, #0xbc]
003cee50: str      r1, [r4, #0xc0]
003cee54: str      r1, [r4, #0xc4]
003cee58: str      r1, [r4, #0xc8]
003cee5c: strb     r1, [r4, #0xd0]
003cee60: ldr      r1, [r5, #0x18]
003cee64: ldr      r2, [r5, #0x10]
003cee68: sub      sp, sp, #0xc
003cee6c: sub      r3, r1, #4
003cee70: cmp      r2, r3
003cee74: str      r4, [sp, #4]
003cee78: beq      #0x3cee98
003cee7c: str      r4, [r2]
003cee80: ldr      r3, [r5, #0x10]
003cee84: add      r3, r3, #4
003cee88: str      r3, [r5, #0x10]
003cee8c: mov      r0, r4
003cee90: add      sp, sp, #0xc
003cee94: pop      {r4, r5, pc}
003cee98: add      r0, sp, #4
003cee9c: bl       #0x3ce810
003ceea0: b        #0x3cee8c
003ceea4: subseq   r5, ip, r4, lsr sp
003ceea8: andeq    r4, r0, ip, asr #12
003ceeac: andeq    r4, r0, ip, lsr #19

# _ZN6CharAI11OnInitFinalEv
003d0ba4: push     {r4, lr}
003d0ba8: ldr      r3, [r0, #0x1c]
003d0bac: cmp      r3, #0
003d0bb0: beq      #0x3d0bc4
003d0bb4: mov      r0, r3
003d0bb8: ldr      r3, [r3]
003d0bbc: mov      lr, pc
003d0bc0: ldr      pc, [r3, #0x10]
003d0bc4: pop      {r4, pc}

# _ZN6CharAI6UpdateEv
003cfbf4: push     {r4, r5, r6, lr}
003cfbf8: mov      r5, r0
003cfbfc: ldr      r0, [pc, #0x140]
003cfc00: ldr      r4, [pc, #0x140]
003cfc04: add      r0, pc, r0
003cfc08: bl       #0x3136b4
003cfc0c: ldrb     r3, [r5, #0x18]
003cfc10: add      r4, pc, r4
003cfc14: cmp      r3, #0
003cfc18: bne      #0x3cfc44
003cfc1c: ldr      r6, [r5, #4]
003cfc20: ldr      r3, [r6, #0x378]
003cfc24: ldrb     r2, [r3, #9]
003cfc28: cmp      r2, #0
003cfc2c: bne      #0x3cfc60
003cfc30: ldr      r2, [pc, #0x114]
003cfc34: ldr      r2, [r4, r2]
003cfc38: ldrb     r2, [r2]
003cfc3c: cmp      r2, #0
003cfc40: beq      #0x3cfc54
003cfc44: ldr      r0, [pc, #0x104]
003cfc48: add      r0, pc, r0
003cfc4c: pop      {r4, r5, r6, lr}
003cfc50: b        #0x3136b8
003cfc54: ldrb     r3, [r3, #8]
003cfc58: cmp      r3, #0
003cfc5c: bne      #0x3cfc44
003cfc60: ldr      r3, [r6, #0x520]
003cfc64: tst      r3, #0x100
003cfc68: beq      #0x3cfc44
003cfc6c: ldr      r3, [r6]
003cfc70: mov      r0, r6
003cfc74: mov      lr, pc
003cfc78: ldr      pc, [r3, #0xc4]
003cfc7c: cmp      r0, #0
003cfc80: beq      #0x3cfc90
003cfc84: ldrb     r3, [r6, #0x2ee]
003cfc88: cmp      r3, #0
003cfc8c: bne      #0x3cfd34
003cfc90: ldr      r6, [pc, #0xbc]
003cfc94: ldr      r3, [r5, #4]
003cfc98: mov      r2, #1
003cfc9c: add      r6, pc, r6
003cfca0: ldr      r4, [pc, #0xb0]
003cfca4: strb     r2, [r3, #0x88]
003cfca8: mov      r0, r6
003cfcac: bl       #0x3136b4
003cfcb0: mov      r0, r5
003cfcb4: bl       #0x3cb908
003cfcb8: add      r4, pc, r4
003cfcbc: mov      r0, r6
003cfcc0: ldr      r6, [pc, #0x94]
003cfcc4: bl       #0x3136b8
003cfcc8: mov      r0, r4
003cfccc: bl       #0x3136b4
003cfcd0: mov      r0, r5
003cfcd4: bl       #0x3cc5a4
003cfcd8: add      r6, pc, r6
003cfcdc: mov      r0, r4
003cfce0: ldr      r4, [pc, #0x78]
003cfce4: bl       #0x3136b8
003cfce8: mov      r0, r6
003cfcec: bl       #0x3136b4
003cfcf0: mov      r0, r5
003cfcf4: bl       #0x3cf3f0
003cfcf8: add      r4, pc, r4
003cfcfc: mov      r0, r6
003cfd00: bl       #0x3136b8
003cfd04: mov      r0, r4
003cfd08: bl       #0x3136b4
003cfd0c: mov      r0, r5
003cfd10: ldr      r3, [r5]
003cfd14: mov      lr, pc
003cfd18: ldr      pc, [r3, #0x18]
003cfd1c: mov      r0, r4
003cfd20: bl       #0x3136b8
003cfd24: ldr      r0, [pc, #0x38]
003cfd28: add      r0, pc, r0
003cfd2c: pop      {r4, r5, r6, lr}
003cfd30: b        #0x3136b8
003cfd34: ldrb     r3, [r6, #0x2f0]
003cfd38: cmp      r3, #0
003cfd3c: beq      #0x3cfc44
003cfd40: b        #0x3cfc90
003cfd44: strdeq   r5, r6, [pc], #-0x7c
003cfd48: subseq   r4, ip, r0, lsl #29
003cfd4c: andeq    r3, r0, r0, asr r6
003cfd50: strheq   r5, [pc], #-0x78
003cfd54: subeq    r5, pc, ip, ror r7
003cfd58: subeq    r5, pc, r8, ror r7
003cfd5c: subeq    r5, pc, r0, ror r7
003cfd60: subeq    r5, pc, r8, ror #14
003cfd64: ldrdeq   r5, r6, [pc], #-0x68

# _ZN6CharAI10OnInitPostEv
003d0b80: push     {r4, lr}
003d0b84: ldr      r3, [r0, #0x1c]
003d0b88: cmp      r3, #0
003d0b8c: beq      #0x3d0ba0
003d0b90: mov      r0, r3
003d0b94: ldr      r3, [r3]
003d0b98: mov      lr, pc
003d0b9c: ldr      pc, [r3, #0xc]
003d0ba0: pop      {r4, pc}

# _ZN10AISDefault11OnInitFinalEv
003dbe80: bx       lr

# _ZN6CharAI14StepLoadScriptEv
003cdf7c: push     {r4, r5, r6, r7, lr}
003cdf80: ldr      r4, [pc, #0xac]
003cdf84: ldr      r6, [pc, #0xac]
003cdf88: ldr      r1, [r0, #0x30]
003cdf8c: add      r4, pc, r4
003cdf90: ldr      r3, [r4, r6]
003cdf94: sub      sp, sp, #0x24
003cdf98: cmp      r1, #0
003cdf9c: ldr      r3, [r3]
003cdfa0: str      r3, [sp, #0x1c]
003cdfa4: beq      #0x3ce00c
003cdfa8: ldr      r0, [r0, #0x20]
003cdfac: bl       #0x37b574
003cdfb0: ldr      r3, [pc, #0x84]
003cdfb4: add      r5, sp, #4
003cdfb8: ldr      r7, [r4, r3]
003cdfbc: mov      r0, r7
003cdfc0: bl       #0x337888
003cdfc4: ldr      r1, [pc, #0x74]
003cdfc8: mov      r2, sp
003cdfcc: mov      r0, r5
003cdfd0: add      r1, pc, r1
003cdfd4: bl       #0x3140ec
003cdfd8: mov      r0, r7
003cdfdc: mov      r1, r5
003cdfe0: bl       #0x337a88
003cdfe4: ldr      r0, [sp, #0x18]
003cdfe8: cmp      r0, r5
003cdfec: beq      #0x3ce00c
003cdff0: cmp      r0, #0
003cdff4: beq      #0x3ce00c
003cdff8: ldr      r1, [sp, #4]
003cdffc: rsb      r1, r0, r1
003ce000: cmp      r1, #0x80
003ce004: bhi      #0x3ce028
003ce008: bl       #0x708f00
003ce00c: ldr      r3, [r4, r6]
003ce010: ldr      r2, [sp, #0x1c]
003ce014: ldr      r3, [r3]
003ce018: cmp      r2, r3
003ce01c: bne      #0x3ce030
003ce020: add      sp, sp, #0x24
003ce024: pop      {r4, r5, r6, r7, pc}
003ce028: bl       #0x310440
003ce02c: b        #0x3ce00c
003ce030: bl       #0x30e310
003ce034: subseq   r6, ip, r4, lsl #22
003ce038: andeq    r4, r0, ip, lsr #1
003ce03c: andeq    r0, r0, r4, lsl #17
003ce040: subeq    r7, pc, r8, lsl #7

# _ZN13ObjectManager12DoCharAIInitEv
0034064c: push     {r4, r5, r6, lr}
00340650: mov      r6, r0
00340654: ldr      r4, [r6, #0x2c]!
00340658: cmp      r6, r4
0034065c: beq      #0x34068c
00340660: ldr      r5, [r4, #8]
00340664: subs     r0, r5, #0
00340668: beq      #0x340680
0034066c: ldr      r3, [r5]
00340670: mov      lr, pc
00340674: ldr      pc, [r3, #0x24]
00340678: cmp      r0, #0
0034067c: bne      #0x340690
00340680: ldr      r4, [r4]
00340684: cmp      r6, r4
00340688: bne      #0x340660
0034068c: pop      {r4, r5, r6, pc}
00340690: add      r5, r5, #0x3c8
00340694: mov      r0, r5
00340698: bl       #0x3cfd7c
0034069c: mov      r0, r5
003406a0: bl       #0x3cfde4
003406a4: ldr      r4, [r4]
003406a8: b        #0x340684

# _ZN6CharAI17InitScriptProcessEb
003ce7c0: push     {r4, r5, r6, lr}
003ce7c4: mov      r4, r0
003ce7c8: ldr      r0, [r0, #4]
003ce7cc: mov      r5, r1
003ce7d0: bl       #0x3b3a70
003ce7d4: mov      r0, r4
003ce7d8: bl       #0x3ce044
003ce7dc: mov      r0, r4
003ce7e0: bl       #0x3d8894
003ce7e4: ldr      r3, [r4]
003ce7e8: mov      r0, r4
003ce7ec: mov      lr, pc
003ce7f0: ldr      pc, [r3, #0xc]
003ce7f4: cmp      r5, #0
003ce7f8: beq      #0x3ce80c
003ce7fc: mov      r0, r4
003ce800: ldr      r3, [r4]
003ce804: mov      lr, pc
003ce808: ldr      pc, [r3, #0x10]
003ce80c: pop      {r4, r5, r6, pc}

# _ZN10AISDefault10OnInitPostEv
003dbe7c: bx       lr

# _ZN6CharAI8OnUpdateEv
003d1050: push     {r4, lr}
003d1054: ldr      r3, [r0, #0x1c]
003d1058: sub      sp, sp, #0x10
003d105c: mov      r4, r0
003d1060: cmp      r3, #0
003d1064: beq      #0x3d1078
003d1068: mov      r0, r3
003d106c: ldr      r3, [r3]
003d1070: mov      lr, pc
003d1074: ldr      pc, [r3, #0x18]
003d1078: ldr      r0, [r4, #4]
003d107c: mov      r1, #0
003d1080: add      r0, r0, #0x4f0
003d1084: add      r0, r0, #0xc
003d1088: bl       #0x3c0260
003d108c: cmp      r0, #0
003d1090: beq      #0x3d10c8
003d1094: ldr      r3, [r4, #4]
003d1098: ldr      r2, [r3, #0x408]
003d109c: cmp      r2, #0
003d10a0: beq      #0x3d10f4
003d10a4: ldr      r4, [r3, #0x2d8]
003d10a8: mov      r0, r3
003d10ac: bl       #0x38c600
003d10b0: cmp      r4, #0
003d10b4: beq      #0x3d10c0
003d10b8: mov      r0, r4
003d10bc: bl       #0x4713d0
003d10c0: add      sp, sp, #0x10
003d10c4: pop      {r4, pc}
003d10c8: ldr      r0, [r4, #4]
003d10cc: add      r0, r0, #0x4f0
003d10d0: add      r0, r0, #0xc
003d10d4: bl       #0x3c0230
003d10d8: cmp      r0, #0
003d10dc: ldreq    r3, [r4, #4]
003d10e0: beq      #0x3d10a4
003d10e4: ldr      r3, [r4, #4]
003d10e8: ldr      r2, [r3, #0x408]
003d10ec: cmp      r2, #0
003d10f0: bne      #0x3d10a4
003d10f4: ldr      r2, [r3, #0x418]
003d10f8: cmp      r2, #0
003d10fc: bne      #0x3d10a4
003d1100: ldrb     r2, [r3, #0x2ee]
003d1104: cmp      r2, #0
003d1108: bne      #0x3d10c0
003d110c: mov      r0, r3
003d1110: ldr      r3, [r3]
003d1114: mov      lr, pc
003d1118: ldr      pc, [r3, #0xc4]
003d111c: cmp      r0, #0
003d1120: beq      #0x3d10c0
003d1124: ldr      r0, [r4, #4]
003d1128: bl       #0x38c790
003d112c: ldr      r3, [r4, #4]
003d1130: mov      r0, r3
003d1134: ldr      r3, [r3]
003d1138: mov      lr, pc
003d113c: ldr      pc, [r3, #0x34]
003d1140: cmp      r0, #0
003d1144: bne      #0x3d10c0
003d1148: ldr      r0, [r4, #4]
003d114c: ldrb     r3, [r0, #0x85]
003d1150: cmp      r3, #0
003d1154: bne      #0x3d10c0
003d1158: add      r1, r0, #0x1440
003d115c: mov      r2, #1
003d1160: add      r1, r1, #0x10
003d1164: bl       #0x393db4
003d1168: ldr      r3, [r4, #4]
003d116c: ldr      r2, [r3, #0x2d8]
003d1170: cmp      r2, #0
003d1174: beq      #0x3d10c0
003d1178: ldr      r0, [r2, #8]
003d117c: cmp      r0, #0
003d1180: beq      #0x3d10c0
003d1184: movw     r2, #0x1450
003d1188: ldr      lr, [r3, r2]
003d118c: movw     r2, #0x1454
003d1190: ldr      ip, [r3, r2]
003d1194: movw     r2, #0x1458
003d1198: ldr      r2, [r3, r2]
003d119c: ldr      r3, [r0]
003d11a0: add      r1, sp, #4
003d11a4: ldr      r3, [r3, #0xa4]
003d11a8: str      lr, [sp, #4]
003d11ac: str      ip, [sp, #8]
003d11b0: str      r2, [sp, #0xc]
003d11b4: blx      r3
003d11b8: b        #0x3d10c0

# _ZN6CharAI14StepInitScriptEv
003cb314: push     {r4, lr}
003cb318: mov      r4, r0
003cb31c: ldr      r3, [r0]
003cb320: mov      lr, pc
003cb324: ldr      pc, [r3, #8]
003cb328: ldr      r3, [r4, #0x30]
003cb32c: cmp      r3, #0
003cb330: beq      #0x3cb348
003cb334: ldr      r3, [r4, #0x20]
003cb338: mov      r0, r3
003cb33c: ldr      r3, [r3]
003cb340: mov      lr, pc
003cb344: ldr      pc, [r3, #0xcc]
003cb348: pop      {r4, pc}

# _ZN6CharAI22LoadNInitScriptProcessEb
003cf3a4: push     {r4, lr}
003cf3a8: ldr      r3, [r0, #0x1c]
003cf3ac: sub      sp, sp, #8
003cf3b0: mov      r4, r0
003cf3b4: cmp      r3, #0
003cf3b8: beq      #0x3cf3c8
003cf3bc: mov      r0, #0
003cf3c0: add      sp, sp, #8
003cf3c4: pop      {r4, pc}
003cf3c8: str      r1, [sp, #4]
003cf3cc: bl       #0x3cf1f0
003cf3d0: ldr      r3, [r4, #0x1c]
003cf3d4: ldr      r1, [sp, #4]
003cf3d8: cmp      r3, #0
003cf3dc: beq      #0x3cf3bc
003cf3e0: mov      r0, r4
003cf3e4: bl       #0x3ce7c0
003cf3e8: mov      r0, #1
003cf3ec: b        #0x3cf3c0

# _ZN17CharAISkillScriptC1EP9CharacterPKcj
003cde2c: push     {r4, r5, r6, r7, r8, sl, lr}
003cde30: ldr      r5, [pc, #0x11c]
003cde34: ldr      ip, [pc, #0x11c]
003cde38: mov      r4, r0
003cde3c: add      r5, pc, r5
003cde40: ldr      ip, [r5, ip]
003cde44: add      r7, r0, #0xc
003cde48: mov      sl, r1
003cde4c: add      ip, ip, #8
003cde50: str      ip, [r0]
003cde54: sub      sp, sp, #0xc
003cde58: stmib    r4, {r1, r2}
003cde5c: mov      r0, r7
003cde60: mov      r8, r3
003cde64: mov      r6, r2
003cde68: bl       #0x3192b4
003cde6c: mvn      r3, #0
003cde70: cmp      sl, #0
003cde74: str      r3, [r4, #0x18]
003cde78: str      r8, [r4, #0x14]
003cde7c: beq      #0x3cdeac
003cde80: cmp      r6, #0
003cde84: beq      #0x3cdf00
003cde88: mov      r1, r6
003cde8c: mov      r0, r7
003cde90: bl       #0x39ec10
003cde94: mov      r0, r7
003cde98: mov      r1, r8
003cde9c: bl       #0x3cdd78
003cdea0: mov      r0, r4
003cdea4: add      sp, sp, #0xc
003cdea8: pop      {r4, r5, r6, r7, r8, sl, pc}
003cdeac: ldr      r3, [pc, #0xa8]
003cdeb0: ldr      r3, [r5, r3]
003cdeb4: ldr      r3, [r3]
003cdeb8: cmp      r3, #2
003cdebc: streq    sl, [sl]
003cdec0: beq      #0x3cde80
003cdec4: cmp      r3, #1
003cdec8: bne      #0x3cde80
003cdecc: ldr      r0, [pc, #0x8c]
003cded0: ldr      r1, [pc, #0x8c]
003cded4: ldr      r2, [pc, #0x8c]
003cded8: ldr      r0, [r5, r0]
003cdedc: ldr      r3, [pc, #0x88]
003cdee0: mov      ip, #0x2e
003cdee4: add      r1, pc, r1
003cdee8: add      r2, pc, r2
003cdeec: add      r3, pc, r3
003cdef0: add      r0, r0, #0xa8
003cdef4: str      ip, [sp]
003cdef8: bl       #0x30e004
003cdefc: b        #0x3cde80
003cdf00: ldr      r3, [pc, #0x54]
003cdf04: ldr      r3, [r5, r3]
003cdf08: ldr      r3, [r3]
003cdf0c: cmp      r3, #2
003cdf10: streq    r6, [r6]
003cdf14: beq      #0x3cde88
003cdf18: cmp      r3, #1
003cdf1c: bne      #0x3cde88
003cdf20: ldr      r0, [pc, #0x38]
003cdf24: ldr      r1, [pc, #0x44]
003cdf28: ldr      r2, [pc, #0x44]
003cdf2c: ldr      r0, [r5, r0]
003cdf30: ldr      r3, [pc, #0x40]
003cdf34: mov      ip, #0x2e
003cdf38: add      r1, pc, r1
003cdf3c: add      r2, pc, r2
003cdf40: add      r3, pc, r3
003cdf44: add      r0, r0, #0xa8
003cdf48: str      ip, [sp]
003cdf4c: bl       #0x30e004
003cdf50: b        #0x3cde88
003cdf54: subseq   r6, ip, r4, asr ip
003cdf58: andeq    r0, r0, ip, lsr #23
003cdf5c: andeq    r3, r0, r0, asr #19
003cdf60: andeq    r1, r0, r0, asr #19
003cdf64: strdeq   r0, r1, [pc], #-0x44
003cdf68: subeq    r7, pc, r0, lsl r4
003cdf6c: subeq    r7, pc, r4, lsl r4
003cdf70: subeq    r0, pc, r0, lsr #9
003cdf74: subseq   r3, r1, ip, lsr #3
003cdf78: subeq    r7, pc, r0, asr #7

# _ZN6CharAI21AIUnLoadScriptProcessEb
003cc9dc: push     {r4, r5, r6, r7, r8, lr}
003cc9e0: ldr      r3, [r0, #0x20]
003cc9e4: cmp      r1, #0
003cc9e8: movne    r2, #1
003cc9ec: ldrbeq   r2, [r0, #0x24]
003cc9f0: cmp      r3, #0
003cc9f4: mov      r5, r0
003cc9f8: beq      #0x3cca04
003cc9fc: cmp      r2, #0
003cca00: bne      #0x3cca08
003cca04: pop      {r4, r5, r6, r7, r8, pc}
003cca08: bl       #0x3d8ae0
003cca0c: mov      r0, r5
003cca10: bl       #0x3d8a98
003cca14: ldr      r4, [r5, #0xb4]
003cca18: ldr      r6, [r5, #0xb8]
003cca1c: cmp      r4, r6
003cca20: beq      #0x3cca64
003cca24: mov      r7, #0
003cca28: ldr      r3, [r4]
003cca2c: cmp      r3, #0
003cca30: beq      #0x3cca48
003cca34: mov      r0, r3
003cca38: ldr      r3, [r3]
003cca3c: mov      lr, pc
003cca40: ldr      pc, [r3, #4]
003cca44: str      r7, [r4]
003cca48: add      r4, r4, #4
003cca4c: cmp      r6, r4
003cca50: bne      #0x3cca28
003cca54: ldr      r3, [r5, #0xb4]
003cca58: ldr      r2, [r5, #0xb8]
003cca5c: cmp      r3, r2
003cca60: strne    r3, [r5, #0xb8]
003cca64: ldr      r4, [r5, #0xc0]
003cca68: ldr      r6, [r5, #0xc4]
003cca6c: cmp      r4, r6
003cca70: beq      #0x3ccab4
003cca74: mov      r7, #0
003cca78: ldr      r3, [r4]
003cca7c: cmp      r3, #0
003cca80: beq      #0x3cca98
003cca84: mov      r0, r3
003cca88: ldr      r3, [r3]
003cca8c: mov      lr, pc
003cca90: ldr      pc, [r3, #4]
003cca94: str      r7, [r4]
003cca98: add      r4, r4, #4
003cca9c: cmp      r6, r4
003ccaa0: bne      #0x3cca78
003ccaa4: ldr      r3, [r5, #0xc0]
003ccaa8: ldr      r2, [r5, #0xc4]
003ccaac: cmp      r3, r2
003ccab0: strne    r3, [r5, #0xc4]
003ccab4: ldr      r3, [r5, #0x20]
003ccab8: cmp      r3, #0
003ccabc: beq      #0x3ccad8
003ccac0: mov      r0, r3
003ccac4: ldr      r3, [r3]
003ccac8: mov      lr, pc
003ccacc: ldr      pc, [r3, #4]
003ccad0: mov      r3, #0
003ccad4: str      r3, [r5, #0x20]
003ccad8: mov      r3, #0
003ccadc: str      r3, [r5, #0x30]
003ccae0: str      r3, [r5, #0x20]
003ccae4: str      r3, [r5, #0x1c]
003ccae8: str      r3, [r5, #0x28]
003ccaec: strb     r3, [r5, #0x2c]
003ccaf0: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6CharAI16AI_ScriptCleanUpEv
003cfd7c: push     {r4, lr}
003cfd80: mov      r4, r0
003cfd84: ldr      r0, [r0, #4]
003cfd88: ldr      r1, [r4, #0x10]
003cfd8c: add      r0, r0, #0x3b4
003cfd90: bl       #0x3db2d8
003cfd94: ldr      r0, [r4, #4]
003cfd98: ldr      r1, [r4, #0x14]
003cfd9c: add      r0, r0, #0x3b4
003cfda0: bl       #0x3db2d8
003cfda4: ldr      r2, [r4, #0x1c]
003cfda8: mvn      r3, #0
003cfdac: str      r3, [r4, #0x14]
003cfdb0: cmp      r2, #0
003cfdb4: str      r3, [r4, #0x10]
003cfdb8: beq      #0x3cfde0
003cfdbc: mov      r0, r4
003cfdc0: bl       #0x3d8ae0
003cfdc4: mov      r0, r4
003cfdc8: bl       #0x3d8a98
003cfdcc: ldr      r3, [r4, #0x1c]
003cfdd0: mov      r0, r3
003cfdd4: ldr      r3, [r3]
003cfdd8: mov      lr, pc
003cfddc: ldr      pc, [r3, #0x14]
003cfde0: pop      {r4, pc}

# _ZN6CharAI13AI_ScriptInitEv
003cfde4: push     {r4, r5, r6, r7, lr}
003cfde8: ldr      r3, [r0, #4]
003cfdec: sub      sp, sp, #0xc
003cfdf0: mov      r4, r0
003cfdf4: mov      r0, r3
003cfdf8: ldr      r3, [r3]
003cfdfc: mov      lr, pc
003cfe00: ldr      pc, [r3, #0x34]
003cfe04: ldr      r5, [pc, #0x110]
003cfe08: cmp      r0, #0
003cfe0c: add      r5, pc, r5
003cfe10: bne      #0x3cfed0
003cfe14: ldr      r1, [r4, #0x10]
003cfe18: cmn      r1, #1
003cfe1c: beq      #0x3cfe2c
003cfe20: ldr      r0, [r4, #4]
003cfe24: add      r0, r0, #0x3b4
003cfe28: bl       #0x3db2d8
003cfe2c: ldr      r6, [pc, #0xec]
003cfe30: ldr      r1, [pc, #0xec]
003cfe34: ldr      r2, [pc, #0xec]
003cfe38: ldr      r3, [r5, r6]
003cfe3c: add      r1, pc, r1
003cfe40: add      r2, pc, r2
003cfe44: ldr      r0, [r3, #0x2c]
003cfe48: ldr      r7, [r4, #4]
003cfe4c: bl       #0x4c4bdc
003cfe50: add      r7, r7, #0x3b4
003cfe54: mov      r1, r0
003cfe58: mov      ip, #0
003cfe5c: mov      r0, r7
003cfe60: mvn      r2, #0
003cfe64: mov      r3, #0x33
003cfe68: str      ip, [sp]
003cfe6c: bl       #0x3dbe24
003cfe70: ldr      r1, [r4, #0x14]
003cfe74: str      r0, [r4, #0x10]
003cfe78: cmn      r1, #1
003cfe7c: beq      #0x3cfe8c
003cfe80: ldr      r0, [r4, #4]
003cfe84: add      r0, r0, #0x3b4
003cfe88: bl       #0x3db2d8
003cfe8c: ldr      r3, [r5, r6]
003cfe90: ldr      r1, [pc, #0x94]
003cfe94: ldr      r2, [pc, #0x94]
003cfe98: ldr      r0, [r3, #0x2c]
003cfe9c: add      r1, pc, r1
003cfea0: add      r2, pc, r2
003cfea4: ldr      r5, [r4, #4]
003cfea8: bl       #0x4c4bdc
003cfeac: add      r5, r5, #0x3b4
003cfeb0: mov      r1, r0
003cfeb4: mov      ip, #0
003cfeb8: mov      r0, r5
003cfebc: mvn      r2, #0
003cfec0: mov      r3, #0x34
003cfec4: str      ip, [sp]
003cfec8: bl       #0x3dbe24
003cfecc: str      r0, [r4, #0x14]
003cfed0: ldr      r3, [r4, #0x1c]
003cfed4: cmp      r3, #0
003cfed8: beq      #0x3cff14
003cfedc: mov      r0, r3
003cfee0: ldr      r3, [r3]
003cfee4: mov      lr, pc
003cfee8: ldr      pc, [r3, #8]
003cfeec: ldr      r3, [r4, #0x1c]
003cfef0: mov      r0, r3
003cfef4: ldr      r3, [r3]
003cfef8: mov      lr, pc
003cfefc: ldr      pc, [r3, #0xc]
003cff00: ldr      r3, [r4, #0x1c]
003cff04: mov      r0, r3
003cff08: ldr      r3, [r3]
003cff0c: mov      lr, pc
003cff10: ldr      pc, [r3, #0x10]
003cff14: add      sp, sp, #0xc
003cff18: pop      {r4, r5, r6, r7, pc}
003cff1c: subseq   r4, ip, r4, lsl #25
003cff20: strdeq   r3, r4, [r0], -r4
003cff24: subeq    r1, pc, r4, lsl sb
003cff28: subeq    r5, pc, r8, lsr r6
003cff2c: strheq   r1, [pc], #-0x84
003cff30: subeq    r5, pc, r0, ror #11

# _ZN15AISPlayerIPhoneD0Ev
003de294: ldr      r3, [pc, #0x2c]
003de298: ldr      r2, [pc, #0x2c]
003de29c: push     {r4, lr}
003de2a0: add      r3, pc, r3
003de2a4: ldr      r2, [r3, r2]
003de2a8: mov      r4, r0
003de2ac: add      r2, r2, #8
003de2b0: str      r2, [r0]
003de2b4: bl       #0x3de100
003de2b8: mov      r0, r4
003de2bc: bl       #0x310440
003de2c0: mov      r0, r4
003de2c4: pop      {r4, pc}
003de2c8: ldrsheq  r6, [fp], #-0x70
003de2cc: andeq    r1, r0, ip, asr #15
