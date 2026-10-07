# _ZN10ObjectBase7EnabledEv 0x33de04 48
0033de04: push     {r4, lr}
0033de08: mov      r1, #1
0033de0c: mov      r4, r0
0033de10: ldr      r3, [r0]
0033de14: mov      lr, pc
0033de18: ldr      pc, [r3, #0x40]
0033de1c: mov      r0, r4
0033de20: ldr      r3, [r4]
0033de24: mov      r1, #1
0033de28: mov      lr, pc
0033de2c: ldr      pc, [r3, #0x3c]
0033de30: pop      {r4, pc}
# _ZN11PropertyMap11SetPropertyEPKcS1_ 0x51387c 128
0051387c: push     {r4, lr}
00513880: mov      r3, r1
00513884: sub      sp, sp, #8
00513888: mov      r1, r0
0051388c: mov      r4, r2
00513890: mov      r0, sp
00513894: mov      r2, r3
00513898: bl       #0x513858
0051389c: cmp      r4, #0
005138a0: ldr      r3, [sp]
005138a4: ldr      r1, [sp, #4]
005138a8: beq      #0x5138d8
005138ac: cmp      r1, #0
005138b0: beq      #0x5138d0
005138b4: cmp      r3, #0
005138b8: beq      #0x5138d0
005138bc: mov      r0, r3
005138c0: mov      r2, r4
005138c4: ldr      r3, [r3]
005138c8: mov      lr, pc
005138cc: ldr      pc, [r3, #4]
005138d0: add      sp, sp, #8
005138d4: pop      {r4, pc}
005138d8: cmp      r1, #0
005138dc: beq      #0x5138d0
005138e0: cmp      r3, #0
005138e4: beq      #0x5138d0
005138e8: mov      r0, r3
005138ec: ldr      r3, [r3]
005138f0: mov      lr, pc
005138f4: ldr      pc, [r3, #0xc]
005138f8: b        #0x5138d0
# _ZN10GameObject14UpdatePFObjectEv 0x393ea0 240
00393ea0: push     {r4, r5, r6, r7, lr}
00393ea4: ldr      r3, [r0, #0x1c8]
00393ea8: ldr      r5, [pc, #0xd8]
00393eac: sub      sp, sp, #0xc
00393eb0: cmp      r3, #0
00393eb4: mov      r4, r0
00393eb8: add      r5, pc, r5
00393ebc: beq      #0x393ee8
00393ec0: ldr      r3, [r0]
00393ec4: mov      lr, pc
00393ec8: ldr      pc, [r3, #0xb4]
00393ecc: cmp      r0, #0
00393ed0: bne      #0x393f38
00393ed4: ldr      r0, [r4, #0x2dc]
00393ed8: cmp      r0, #0
00393edc: beq      #0x393ef0
00393ee0: bl       #0x46e750
00393ee4: str      r0, [r4, #0x1d0]
00393ee8: add      sp, sp, #0xc
00393eec: pop      {r4, r5, r6, r7, pc}
00393ef0: ldr      r1, [r4, #0x144]
00393ef4: ldr      r0, [r4, #0x150]
00393ef8: bl       #0x30e3ac
00393efc: ldr      r1, [r4, #0x148]
00393f00: mov      r5, r0
00393f04: ldr      r0, [r4, #0x154]
00393f08: bl       #0x30e3ac
00393f0c: mov      r6, r0
00393f10: mov      r1, r6
00393f14: mov      r0, r5
00393f18: bl       #0x30e70c
00393f1c: cmp      r0, #0
00393f20: movne    r5, r6
00393f24: mov      r0, r5
00393f28: mov      r1, #0x3f000000
00393f2c: bl       #0x30ed6c
00393f30: str      r0, [r4, #0x1d0]
00393f34: b        #0x393ee8
00393f38: ldr      r3, [r4]
00393f3c: mov      r0, r4
00393f40: ldr      r7, [r4, #0x2dc]
00393f44: mov      lr, pc
00393f48: ldr      pc, [r3, #0xb8]
00393f4c: ldr      r3, [r4]
00393f50: mov      r6, r0
00393f54: mov      r0, r4
00393f58: mov      lr, pc
00393f5c: ldr      pc, [r3, #0xbc]
00393f60: ldr      r3, [pc, #0x24]
00393f64: subs     r7, r7, #0
00393f68: movne    r7, #1
00393f6c: str      r0, [sp]
00393f70: mov      r2, r7
00393f74: ldr      r0, [r5, r3]
00393f78: add      r1, r4, #0x1c8
00393f7c: mov      r3, r6
00393f80: bl       #0x528234
00393f84: b        #0x393ed4
# _ZNK9Character8IsFaerieEv 0x3a3094 24
003a3094: push     {r4, lr}
003a3098: bl       #0x3a3054
003a309c: cmp      r0, #3
003a30a0: movne    r0, #0
003a30a4: moveq    r0, #1
003a30a8: pop      {r4, pc}
# _ZN14PhysicalObject12enableFilterEv 0x46ebe4 136
0046ebe4: push     {r4, lr}
0046ebe8: ldrb     r3, [r0, #0x26]
0046ebec: mov      r4, r0
0046ebf0: cmp      r3, #0
0046ebf4: beq      #0x46ec60
0046ebf8: ldr      r3, [r0, #0x18]
0046ebfc: cmp      r3, #0
0046ec00: beq      #0x46ec2c
0046ec04: ldrh     r2, [r0, #0x20]
0046ec08: strh     r2, [r3, #0x22]
0046ec0c: ldrh     r2, [r0, #0x22]
0046ec10: strh     r2, [r3, #0x24]
0046ec14: ldrh     r2, [r0, #0x24]
0046ec18: strh     r2, [r3, #0x26]
0046ec1c: ldr      r3, [r0, #4]
0046ec20: ldr      r1, [r0, #0x18]
0046ec24: ldr      r0, [r3, #0x10]
0046ec28: bl       #0x7e7afc
0046ec2c: ldr      r3, [r4, #0x1c]
0046ec30: cmp      r3, #0
0046ec34: beq      #0x46ec60
0046ec38: ldrh     r2, [r4, #0x20]
0046ec3c: strh     r2, [r3, #0x22]
0046ec40: ldrh     r2, [r4, #0x22]
0046ec44: strh     r2, [r3, #0x24]
0046ec48: ldrh     r2, [r4, #0x24]
0046ec4c: strh     r2, [r3, #0x26]
0046ec50: ldr      r3, [r4, #4]
0046ec54: ldr      r1, [r4, #0x1c]
0046ec58: ldr      r0, [r3, #0x10]
0046ec5c: bl       #0x7e7afc
0046ec60: mov      r3, #0
0046ec64: strb     r3, [r4, #0x26]
0046ec68: pop      {r4, pc}
# _ZN11PropertyMap11AddPropertyIbEEvPKcRT_S3_ 0x33e4ac 140
0033e4ac: push     {r4, r5, r6, r7, r8, sl, lr}
0033e4b0: mov      r6, r0
0033e4b4: sub      sp, sp, #0xc
0033e4b8: mov      r5, r1
0033e4bc: mov      r0, #0x24
0033e4c0: mov      r1, #0
0033e4c4: mov      sl, r3
0033e4c8: mov      r7, r2
0033e4cc: bl       #0x310570
0033e4d0: ldr      r4, [pc, #0x54]
0033e4d4: ldr      r3, [pc, #0x54]
0033e4d8: mov      r8, r0
0033e4dc: add      r4, pc, r4
0033e4e0: ldr      r3, [r4, r3]
0033e4e4: mov      r1, r5
0033e4e8: add      r2, sp, #4
0033e4ec: add      r3, r3, #8
0033e4f0: str      r3, [r0], #8
0033e4f4: bl       #0x3140ec
0033e4f8: ldr      r3, [pc, #0x34]
0033e4fc: rsb      r7, r6, r7
0033e500: str      r7, [r8, #4]
0033e504: ldr      r3, [r4, r3]
0033e508: strb     sl, [r8, #0x20]
0033e50c: mov      r0, r6
0033e510: add      r3, r3, #8
0033e514: str      r3, [r8]
0033e518: mov      r1, r5
0033e51c: mov      r2, r8
0033e520: bl       #0x513ce4
0033e524: add      sp, sp, #0xc
0033e528: pop      {r4, r5, r6, r7, r8, sl, pc}
0033e52c: strhteq  r6, [r5], #-0x54
0033e530: andeq    r2, r0, r0, lsr r3
0033e534: andeq    r3, r0, ip, asr #28
# _ZN8RoomZone16AddInitialObjectEP10GameObject 0x396a90 436
00396a90: push     {r4, r5, r6, r7, r8, sl, lr}
00396a94: ldr      r4, [pc, #0x194]
00396a98: ldr      r7, [pc, #0x194]
00396a9c: sub      sp, sp, #0x44
00396aa0: add      r4, pc, r4
00396aa4: ldr      r3, [r4, r7]
00396aa8: subs     r5, r1, #0
00396aac: mov      r6, r0
00396ab0: ldr      r3, [r3]
00396ab4: str      r3, [sp, #0x3c]
00396ab8: beq      #0x396ad4
00396abc: ldr      r3, [r5]
00396ac0: mov      r0, r5
00396ac4: mov      lr, pc
00396ac8: ldr      pc, [r3, #0xc4]
00396acc: cmp      r0, #0
00396ad0: bne      #0x396af4
00396ad4: mov      r0, #0
00396ad8: ldr      r3, [r4, r7]
00396adc: ldr      r2, [sp, #0x3c]
00396ae0: ldr      r3, [r3]
00396ae4: cmp      r2, r3
00396ae8: bne      #0x396c2c
00396aec: add      sp, sp, #0x44
00396af0: pop      {r4, r5, r6, r7, r8, sl, pc}
00396af4: ldr      r8, [r5, #0x160]
00396af8: ldr      r0, [r6, #0x12c]
00396afc: mov      r1, r8
00396b00: bl       #0x30e9ac
00396b04: cmp      r0, #0
00396b08: beq      #0x396ad4
00396b0c: mov      r0, r8
00396b10: ldr      r1, [r6, #0x138]
00396b14: bl       #0x30e9ac
00396b18: cmp      r0, #0
00396b1c: beq      #0x396ad4
00396b20: ldr      r8, [r5, #0x164]
00396b24: ldr      r0, [r6, #0x130]
00396b28: mov      r1, r8
00396b2c: bl       #0x30e9ac
00396b30: cmp      r0, #0
00396b34: beq      #0x396ad4
00396b38: mov      r0, r8
00396b3c: ldr      r1, [r6, #0x13c]
00396b40: bl       #0x30e9ac
00396b44: cmp      r0, #0
00396b48: beq      #0x396ad4
00396b4c: ldrb     r3, [r5, #0x2ef]
00396b50: cmp      r3, #0
00396b54: beq      #0x396bec
00396b58: ldr      r3, [pc, #0xd8]
00396b5c: add      r8, sp, #0x24
00396b60: ldr      sl, [r4, r3]
00396b64: mov      r0, sl
00396b68: bl       #0x337888
00396b6c: ldr      r1, [pc, #0xc8]
00396b70: add      r2, sp, #8
00396b74: mov      r0, r8
00396b78: add      r1, pc, r1
00396b7c: bl       #0x3140ec
00396b80: mov      r1, r8
00396b84: mov      r0, sl
00396b88: bl       #0x337a88
00396b8c: mov      r0, r8
00396b90: bl       #0x318254
00396b94: ldr      r0, [r5, #0x2f4]
00396b98: cmp      r0, #0
00396b9c: beq      #0x396ba8
00396ba0: mov      r1, r5
00396ba4: bl       #0x3968cc
00396ba8: mov      r8, #1
00396bac: mov      r0, r5
00396bb0: add      sl, r6, #0x394
00396bb4: str      r6, [r5, #0x2f4]
00396bb8: strb     r8, [r5, #0x2ef]
00396bbc: bl       #0x38c710
00396bc0: mov      r0, sl
00396bc4: bl       #0x39670c
00396bc8: str      r5, [r0, #8]
00396bcc: ldr      r2, [r6, #0x398]
00396bd0: mov      r3, r0
00396bd4: str      sl, [r0]
00396bd8: str      r2, [r3, #4]
00396bdc: mov      r0, r8
00396be0: str      r3, [r2]
00396be4: str      r3, [r6, #0x398]
00396be8: b        #0x396ad8
00396bec: ldr      r3, [pc, #0x44]
00396bf0: add      r8, sp, #0xc
00396bf4: ldr      sl, [r4, r3]
00396bf8: mov      r0, sl
00396bfc: bl       #0x337888
00396c00: ldr      r1, [pc, #0x38]
00396c04: add      r2, sp, #4
00396c08: mov      r0, r8
00396c0c: add      r1, pc, r1
00396c10: bl       #0x3140ec
00396c14: mov      r0, sl
00396c18: mov      r1, r8
00396c1c: bl       #0x337a88
00396c20: mov      r0, r8
00396c24: bl       #0x318254
00396c28: b        #0x396ba8
00396c2c: bl       #0x30e310
00396c30: ldrsheq  sp, [pc], #-0xf0
00396c34: andeq    r4, r0, ip, lsr #1
00396c38: andeq    r0, r0, r4, lsl #17
00396c3c: subseq   fp, r2, r8, asr lr
00396c40: subseq   fp, r2, r4, asr #27
# _ZN10GameObject7EnabledEv 0x38ba38 52
0038ba38: push     {r4, lr}
0038ba3c: mov      r4, r0
0038ba40: bl       #0x33de04
0038ba44: ldr      r3, [r4, #0x1cc]
0038ba48: ldr      r0, [r4, #0x2dc]
0038ba4c: orr      r3, r3, #8
0038ba50: cmp      r0, #0
0038ba54: str      r3, [r4, #0x1cc]
0038ba58: beq      #0x38ba60
0038ba5c: bl       #0x46ebe4
0038ba60: mov      r3, #0
0038ba64: strb     r3, [r4, #0x373]
0038ba68: pop      {r4, pc}
# _ZN10GameObject8DisabledEv 0x38ba04 52
0038ba04: push     {r4, lr}
0038ba08: mov      r4, r0
0038ba0c: bl       #0x33de34
0038ba10: ldr      r3, [r4, #0x1cc]
0038ba14: ldr      r0, [r4, #0x2dc]
0038ba18: bic      r3, r3, #8
0038ba1c: cmp      r0, #0
0038ba20: str      r3, [r4, #0x1cc]
0038ba24: beq      #0x38ba2c
0038ba28: bl       #0x46eb70
0038ba2c: mov      r3, #1
0038ba30: strb     r3, [r4, #0x373]
0038ba34: pop      {r4, pc}
# _ZN18SimpleTypePropertyIbE17SetToDefaultValueEPv 0x33de80 16
0033de80: ldrb     r2, [r0, #0x20]
0033de84: ldr      r3, [r0, #4]
0033de88: strb     r2, [r1, r3]
0033de8c: bx       lr
# _ZN10GameObject10ZoneExitedEv 0x38c69c 116
0038c69c: push     {r4, r5, r6, lr}
0038c6a0: ldrb     r3, [r0, #0x2ee]
0038c6a4: mov      r2, #0
0038c6a8: mov      r5, r0
0038c6ac: cmp      r3, r2
0038c6b0: strb     r2, [r0, #0x2f0]
0038c6b4: beq      #0x38c6c8
0038c6b8: ldr      r0, [r0, #0x2d8]
0038c6bc: cmp      r0, r2
0038c6c0: beq      #0x38c6c8
0038c6c4: bl       #0x4713d0
0038c6c8: ldr      r3, [r5]
0038c6cc: mov      r0, r5
0038c6d0: ldr      r4, [r3, #0x3c]
0038c6d4: mov      lr, pc
0038c6d8: ldr      pc, [r3, #0xc4]
0038c6dc: cmp      r0, #0
0038c6e0: beq      #0x38c700
0038c6e4: ldrb     r3, [r5, #0x2ee]
0038c6e8: cmp      r3, #0
0038c6ec: ldrbne   r1, [r5, #0x2f0]
0038c6f0: beq      #0x38c700
0038c6f4: mov      r0, r5
0038c6f8: blx      r4
0038c6fc: pop      {r4, r5, r6, pc}
0038c700: mov      r1, #1
0038c704: mov      r0, r5
0038c708: blx      r4
0038c70c: pop      {r4, r5, r6, pc}
# _ZN10ObjectBase11setUpdatingEb 0x33dcf0 8
0033dcf0: strb     r1, [r0, #0x85]
0033dcf4: bx       lr
# _ZN10ObjectBase10SetVisibleEb 0x33dcf8 16
0033dcf8: cmp      r1, #0
0033dcfc: ldrbne   r1, [r0, #0x8a]
0033dd00: strb     r1, [r0, #0x80]
0033dd04: bx       lr
# _ZN10GameObject10SetVisibleEb 0x38b0f0 32
0038b0f0: cmp      r1, #0
0038b0f4: ldr      r3, [r0, #0x2d8]
0038b0f8: ldrbne   r1, [r0, #0x8a]
0038b0fc: cmp      r3, #0
0038b100: strb     r1, [r0, #0x80]
0038b104: bxeq     lr
0038b108: mov      r0, r3
0038b10c: b        #0x4713d0
# _ZNK9Character9IsMonsterEv 0x3a3064 24
003a3064: push     {r4, lr}
003a3068: bl       #0x3a3054
003a306c: cmp      r0, #4
003a3070: movne    r0, #0
003a3074: moveq    r0, #1
003a3078: pop      {r4, pc}
# _ZNK9Character10IsFollowerEv 0x3a307c 24
003a307c: push     {r4, lr}
003a3080: bl       #0x3a3054
003a3084: cmp      r0, #2
003a3088: movne    r0, #0
003a308c: moveq    r0, #1
003a3090: pop      {r4, pc}
# _ZN10ObjectBase17DeclarePropertiesEv 0x33f014 328
0033f014: push     {r4, r5, r6, lr}
0033f018: ldr      r1, [pc, #0x10c]
0033f01c: mov      r4, r0
0033f020: add      r5, r0, #4
0033f024: mov      r0, r5
0033f028: add      r2, r4, #0x84
0033f02c: ldrb     r3, [r4, #0x84]
0033f030: add      r1, pc, r1
0033f034: bl       #0x33e4ac
0033f038: ldr      r1, [pc, #0xf0]
0033f03c: mov      r3, #1
0033f040: mov      r0, r5
0033f044: add      r2, r4, #0x80
0033f048: add      r1, pc, r1
0033f04c: bl       #0x33e4ac
0033f050: ldr      r1, [pc, #0xdc]
0033f054: mov      r0, r5
0033f058: add      r2, r4, #0x30
0033f05c: add      r1, pc, r1
0033f060: bl       #0x33ef7c
0033f064: ldr      r1, [pc, #0xcc]
0033f068: mov      r0, r5
0033f06c: add      r2, r4, #0x48
0033f070: add      r1, pc, r1
0033f074: bl       #0x33ef7c
0033f078: ldr      r1, [pc, #0xbc]
0033f07c: mov      r0, r5
0033f080: add      r2, r4, #0x68
0033f084: add      r1, pc, r1
0033f088: bl       #0x33ef7c
0033f08c: ldr      r1, [pc, #0xac]
0033f090: mov      r0, r5
0033f094: add      r2, r4, #0x83
0033f098: add      r1, pc, r1
0033f09c: mov      r3, #0
0033f0a0: bl       #0x33e4ac
0033f0a4: ldr      r1, [pc, #0x98]
0033f0a8: mov      r3, #0
0033f0ac: mov      r0, r5
0033f0b0: add      r2, r4, #0x87
0033f0b4: add      r1, pc, r1
0033f0b8: bl       #0x33e4ac
0033f0bc: ldr      r1, [pc, #0x84]
0033f0c0: mov      r0, r5
0033f0c4: add      r2, r4, #0x90
0033f0c8: add      r1, pc, r1
0033f0cc: bl       #0x33ef7c
0033f0d0: ldr      r1, [pc, #0x74]
0033f0d4: mov      r0, r5
0033f0d8: add      r2, r4, #0xb4
0033f0dc: add      r1, pc, r1
0033f0e0: bl       #0x33ef7c
0033f0e4: ldr      r1, [pc, #0x64]
0033f0e8: mov      r0, r5
0033f0ec: add      r2, r4, #0xd4
0033f0f0: add      r1, pc, r1
0033f0f4: bl       #0x33ef7c
0033f0f8: ldr      r1, [pc, #0x54]
0033f0fc: mov      r0, r5
0033f100: add      r2, r4, #0xf0
0033f104: add      r1, pc, r1
0033f108: mov      r3, #0
0033f10c: bl       #0x33e4ac
0033f110: ldr      r1, [pc, #0x40]
0033f114: mov      r0, r5
0033f118: add      r2, r4, #0xf1
0033f11c: add      r1, pc, r1
0033f120: mov      r3, #0
0033f124: pop      {r4, r5, r6, lr}
0033f128: b        #0x33e4ac
0033f12c: subseq   r1, r8, r8, lsr r1
0033f130: subseq   r1, r8, r8, lsr #2
0033f134: subseq   r2, sl, ip, lsl #1
0033f138: subseq   r1, r8, r8, lsl #2
0033f13c: subseq   r1, r8, r4, lsl #2
0033f140: subseq   r1, r8, r0, lsl #2
0033f144: ldrsheq  r1, [r8], #-4
0033f148: ldrsheq  r1, [r8], #-0
0033f14c: subseq   r1, r8, ip, ror #1
0033f150: subseq   r1, r8, r8, ror #1
0033f154: subseq   r1, r8, r4, ror #1
0033f158: subseq   r1, r8, r4, ror #1
# _ZN15LightSetManager21GetLightSetIdFromNameESs 0x40c3cc 64
0040c3cc: push     {r4, r5, r6, lr}
0040c3d0: ldr      r6, [r1, #0x14]
0040c3d4: mov      r5, r0
0040c3d8: mov      r4, #0
0040c3dc: ldr      r1, [r5, #0x18]
0040c3e0: mov      r0, r6
0040c3e4: bl       #0x30e31c
0040c3e8: cmp      r0, #0
0040c3ec: beq      #0x40c404
0040c3f0: add      r4, r4, #1
0040c3f4: cmp      r4, #4
0040c3f8: add      r5, r5, #0x18
0040c3fc: bne      #0x40c3dc
0040c400: mov      r4, #0
0040c404: mov      r0, r4
0040c408: pop      {r4, r5, r6, pc}
# _ZN12VisualObject4SyncEv 0x38ba74 32
0038ba74: push     {r4, lr}
0038ba78: mov      r4, r0
0038ba7c: bl       #0x470cb8
0038ba80: mov      r0, r4
0038ba84: bl       #0x472948
0038ba88: mov      r0, r4
0038ba8c: pop      {r4, lr}
0038ba90: b        #0x472860
# _ZN10GameObject11ZoneEnteredEv 0x38c710 128
0038c710: push     {r4, r5, r6, lr}
0038c714: ldrb     r3, [r0, #0x2ee]
0038c718: mov      r2, #1
0038c71c: mov      r5, r0
0038c720: cmp      r3, #0
0038c724: strb     r2, [r0, #0x2f0]
0038c728: beq      #0x38c748
0038c72c: ldr      r0, [r0, #0x2d8]
0038c730: cmp      r0, #0
0038c734: beq      #0x38c748
0038c738: ldrb     r3, [r5, #0x80]
0038c73c: cmp      r3, #0
0038c740: beq      #0x38c748
0038c744: bl       #0x4713d0
0038c748: ldr      r3, [r5]
0038c74c: mov      r0, r5
0038c750: ldr      r4, [r3, #0x3c]
0038c754: mov      lr, pc
0038c758: ldr      pc, [r3, #0xc4]
0038c75c: cmp      r0, #0
0038c760: beq      #0x38c780
0038c764: ldrb     r3, [r5, #0x2ee]
0038c768: cmp      r3, #0
0038c76c: ldrbne   r1, [r5, #0x2f0]
0038c770: beq      #0x38c780
0038c774: mov      r0, r5
0038c778: blx      r4
0038c77c: pop      {r4, r5, r6, pc}
0038c780: mov      r1, #1
0038c784: mov      r0, r5
0038c788: blx      r4
0038c78c: pop      {r4, r5, r6, pc}
# _ZN9Character9InitFinalEv 0x3b4978 588
003b4978: push     {r4, r5, r6, r7, r8, lr}
003b497c: ldr      r4, [pc, #0x22c]
003b4980: ldr      r6, [pc, #0x22c]
003b4984: movw     r3, #0x1395
003b4988: add      r4, pc, r4
003b498c: ldr      r2, [r4, r6]
003b4990: ldrb     r1, [r0, r3]
003b4994: sub      sp, sp, #0x48
003b4998: ldr      r2, [r2]
003b499c: cmp      r1, #0
003b49a0: mov      r5, r0
003b49a4: str      r2, [sp, #0x44]
003b49a8: beq      #0x3b49c8
003b49ac: ldr      r3, [r4, r6]
003b49b0: ldr      r2, [sp, #0x44]
003b49b4: ldr      r3, [r3]
003b49b8: cmp      r2, r3
003b49bc: bne      #0x3b4bac
003b49c0: add      sp, sp, #0x48
003b49c4: pop      {r4, r5, r6, r7, r8, pc}
003b49c8: mov      r2, #1
003b49cc: strb     r2, [r0, r3]
003b49d0: bl       #0x38bd64
003b49d4: ldr      r3, [r5, #0x274]
003b49d8: cmp      r0, r3
003b49dc: bge      #0x3b49ac
003b49e0: mov      r0, r5
003b49e4: bl       #0x38cd48
003b49e8: mov      r0, r5
003b49ec: bl       #0x3a3094
003b49f0: cmp      r0, #0
003b49f4: beq      #0x3b4b98
003b49f8: ldr      r3, [pc, #0x1b8]
003b49fc: mov      r1, #0
003b4a00: mov      r2, #1
003b4a04: ldr      r3, [r4, r3]
003b4a08: ldr      r0, [r3, #0x40]
003b4a0c: bl       #0x36e478
003b4a10: ldr      r7, [r0, #0x660]
003b4a14: mov      r3, #0
003b4a18: str      r3, [sp, #8]
003b4a1c: cmp      r7, #0
003b4a20: str      r3, [sp]
003b4a24: str      r3, [sp, #4]
003b4a28: beq      #0x3b4a88
003b4a2c: mov      r1, sp
003b4a30: mov      r0, r7
003b4a34: bl       #0x393ae4
003b4a38: mov      r0, r7
003b4a3c: bl       #0x3935dc
003b4a40: ldr      r1, [r0]
003b4a44: mov      r7, r0
003b4a48: ldr      r0, [sp]
003b4a4c: bl       #0x30eba4
003b4a50: str      r0, [sp]
003b4a54: ldr      r1, [r7, #4]
003b4a58: ldr      r0, [sp, #4]
003b4a5c: bl       #0x30eba4
003b4a60: str      r0, [sp, #4]
003b4a64: ldr      r1, [r7, #8]
003b4a68: ldr      r0, [sp, #8]
003b4a6c: bl       #0x30eba4
003b4a70: mov      r1, sp
003b4a74: str      r0, [sp, #8]
003b4a78: mov      r2, #1
003b4a7c: mov      r0, r5
003b4a80: mov      r8, sp
003b4a84: bl       #0x393db4
003b4a88: ldr      r3, [r5, #0x2d8]
003b4a8c: cmp      r3, #0
003b4a90: beq      #0x3b4af8
003b4a94: ldr      r3, [r5]
003b4a98: mov      r0, r5
003b4a9c: mov      lr, pc
003b4aa0: ldr      pc, [r3, #0x28]
003b4aa4: cmp      r0, #0
003b4aa8: beq      #0x3b4b7c
003b4aac: ldr      r3, [pc, #0x104]
003b4ab0: ldr      r1, [pc, #0x104]
003b4ab4: add      r7, sp, #0x2c
003b4ab8: ldr      r3, [r4, r3]
003b4abc: add      r1, pc, r1
003b4ac0: add      r2, sp, #0x10
003b4ac4: ldr      r3, [r3, #0x10]
003b4ac8: mov      r0, r7
003b4acc: ldr      r8, [r3, #0x1c]
003b4ad0: bl       #0x3140ec
003b4ad4: add      r8, r8, #0x294
003b4ad8: mov      r0, r8
003b4adc: mov      r1, r7
003b4ae0: bl       #0x40c3cc
003b4ae4: mov      r8, r0
003b4ae8: mov      r0, r7
003b4aec: bl       #0x318254
003b4af0: ldr      r3, [r5, #0x2d8]
003b4af4: str      r8, [r3, #0x40]
003b4af8: add      r7, r5, #0x3c8
003b4afc: mov      r0, r7
003b4b00: ldr      r3, [r5, #0x3c8]
003b4b04: mov      lr, pc
003b4b08: ldr      pc, [r3, #0x10]
003b4b0c: ldr      r3, [r5]
003b4b10: mov      r0, r5
003b4b14: mov      lr, pc
003b4b18: ldr      pc, [r3, #0x28]
003b4b1c: cmp      r0, #0
003b4b20: beq      #0x3b49ac
003b4b24: ldr      r3, [pc, #0x8c]
003b4b28: mov      r1, r5
003b4b2c: ldr      r3, [r4, r3]
003b4b30: ldr      r0, [r3, #0x40]
003b4b34: bl       #0x36effc
003b4b38: cmp      r0, #0
003b4b3c: beq      #0x3b49ac
003b4b40: mov      r0, r7
003b4b44: add      r7, r5, #0x560
003b4b48: bl       #0x3d8894
003b4b4c: mov      r0, r7
003b4b50: mov      r1, #1
003b4b54: bl       #0x3e0810
003b4b58: mov      r0, r7
003b4b5c: mov      r1, #0xc2
003b4b60: mov      r2, #0
003b4b64: bl       #0x3df6e0
003b4b68: bic      r0, r0, r0, asr #31
003b4b6c: strb     r0, [r5, #0x3a8]
003b4b70: mov      r0, r5
003b4b74: bl       #0x3bc4a8
003b4b78: b        #0x3b49ac
003b4b7c: ldr      r3, [pc, #0x34]
003b4b80: ldr      r1, [pc, #0x38]
003b4b84: add      r7, sp, #0x14
003b4b88: ldr      r3, [r4, r3]
003b4b8c: add      r1, pc, r1
003b4b90: add      r2, sp, #0xc
003b4b94: b        #0x3b4ac4
003b4b98: mov      r0, r5
003b4b9c: bl       #0x3a307c
003b4ba0: cmp      r0, #0
003b4ba4: beq      #0x3b4a88
003b4ba8: b        #0x3b49f8
003b4bac: bl       #0x30e310
003b4bb0: subseq   r0, lr, r8, lsl #2
003b4bb4: andeq    r4, r0, ip, lsr #1
003b4bb8: strdeq   r3, r4, [r0], -r4
003b4bbc: subseq   pc, r0, ip, ror r3
# _ZNK14PhysicalObject9getRadiusEv 0x46e750 24
0046e750: push     {r4, lr}
0046e754: mov      r1, #0x42000000
0046e758: ldr      r0, [r0, #0xc]
0046e75c: add      r1, r1, #0xc80000
0046e760: bl       #0x30ed6c
0046e764: pop      {r4, pc}
# _ZN10GameObjectC1EN10ObjectBase6GO_IDSE 0x38c130 616
0038c130: push     {r4, r5, r6, r7, lr}
0038c134: ldr      r6, [pc, #0x254]
0038c138: sub      sp, sp, #0xc
0038c13c: mov      r4, r0
0038c140: bl       #0x33f310
0038c144: ldr      r2, [pc, #0x248]
0038c148: add      r6, pc, r6
0038c14c: mov      r3, #0
0038c150: ldr      r2, [r6, r2]
0038c154: mov      r5, #0
0038c158: str      r3, [r4, #0x120]
0038c15c: add      r1, r2, #0xe4
0038c160: add      r0, r2, #8
0038c164: add      r2, r2, #0xd8
0038c168: str      r2, [r4, #4]
0038c16c: str      r1, [r4, #0x24]
0038c170: str      r0, [r4]
0038c174: str      r3, [r4, #0x124]
0038c178: str      r3, [r4, #0x128]
0038c17c: str      r3, [r4, #0x12c]
0038c180: str      r3, [r4, #0x130]
0038c184: str      r3, [r4, #0x134]
0038c188: str      r3, [r4, #0x138]
0038c18c: str      r3, [r4, #0x13c]
0038c190: str      r3, [r4, #0x140]
0038c194: str      r3, [r4, #0x144]
0038c198: str      r3, [r4, #0x148]
0038c19c: str      r3, [r4, #0x14c]
0038c1a0: str      r3, [r4, #0x150]
0038c1a4: str      r3, [r4, #0x154]
0038c1a8: str      r3, [r4, #0x158]
0038c1ac: str      r3, [r4, #0x160]
0038c1b0: str      r3, [r4, #0x164]
0038c1b4: str      r3, [r4, #0x168]
0038c1b8: str      r3, [r4, #0x16c]
0038c1bc: str      r3, [r4, #0x170]
0038c1c0: str      r3, [r4, #0x174]
0038c1c4: str      r3, [r4, #0x178]
0038c1c8: str      r3, [r4, #0x184]
0038c1cc: str      r3, [r4, #0x188]
0038c1d0: str      r3, [r4, #0x18c]
0038c1d4: str      r3, [r4, #0x190]
0038c1d8: str      r3, [r4, #0x194]
0038c1dc: strb     r5, [r4, #0x15c]
0038c1e0: str      r5, [r4, #0x180]
0038c1e4: add      r0, r4, #0x1c8
0038c1e8: str      r3, [r4, #0x198]
0038c1ec: str      r3, [r4, #0x1c0]
0038c1f0: str      r3, [r4, #0x19c]
0038c1f4: str      r3, [r4, #0x1a0]
0038c1f8: str      r3, [r4, #0x1a4]
0038c1fc: str      r3, [r4, #0x1a8]
0038c200: str      r3, [r4, #0x1ac]
0038c204: str      r3, [r4, #0x1b0]
0038c208: strb     r5, [r4, #0x1b4]
0038c20c: strb     r5, [r4, #0x1b5]
0038c210: str      r3, [r4, #0x1b8]
0038c214: str      r3, [r4, #0x1bc]
0038c218: strb     r5, [r4, #0x1c4]
0038c21c: bl       #0x524644
0038c220: add      r3, r4, #0x278
0038c224: mvn      r7, #0
0038c228: mov      r2, #0x64
0038c22c: str      r2, [r4, #0x274]
0038c230: mov      r0, r3
0038c234: str      r3, [r4, #0x288]
0038c238: str      r3, [r4, #0x28c]
0038c23c: str      r5, [r4, #0x26c]
0038c240: str      r7, [r4, #0x270]
0038c244: mov      r1, #0x10
0038c248: bl       #0x31167c
0038c24c: ldr      r2, [r4, #0x288]
0038c250: add      r3, r4, #0x290
0038c254: mov      r0, r3
0038c258: strb     r5, [r2]
0038c25c: mov      r1, #0x10
0038c260: str      r3, [r4, #0x2a0]
0038c264: str      r3, [r4, #0x2a4]
0038c268: bl       #0x31167c
0038c26c: ldr      r2, [r4, #0x2a0]
0038c270: add      r3, r4, #0x2a8
0038c274: mov      r0, r3
0038c278: strb     r5, [r2]
0038c27c: mov      r1, #0x10
0038c280: str      r3, [r4, #0x2b8]
0038c284: str      r3, [r4, #0x2bc]
0038c288: bl       #0x31167c
0038c28c: ldr      r2, [r4, #0x2b8]
0038c290: add      r3, r4, #0x2c0
0038c294: mov      r0, r3
0038c298: strb     r5, [r2]
0038c29c: mov      r1, #0x10
0038c2a0: str      r3, [r4, #0x2d0]
0038c2a4: str      r3, [r4, #0x2d4]
0038c2a8: bl       #0x31167c
0038c2ac: ldr      r3, [r4, #0x2d0]
0038c2b0: mov      ip, #1
0038c2b4: add      r6, r4, #0x304
0038c2b8: strb     r5, [r3]
0038c2bc: mov      r2, ip
0038c2c0: strb     ip, [r4, #0x2ee]
0038c2c4: strb     ip, [r4, #0x2fb]
0038c2c8: str      r5, [r4, #0x2d8]
0038c2cc: str      r5, [r4, #0x2dc]
0038c2d0: str      r5, [r4, #0x2e0]
0038c2d4: str      r5, [r4, #0x2e4]
0038c2d8: str      r5, [r4, #0x2e8]
0038c2dc: strb     r5, [r4, #0x2ec]
0038c2e0: strb     r5, [r4, #0x2ed]
0038c2e4: strb     r5, [r4, #0x2ef]
0038c2e8: strb     r5, [r4, #0x2f0]
0038c2ec: str      r5, [r4, #0x2f4]
0038c2f0: strb     r5, [r4, #0x2f8]
0038c2f4: strb     r5, [r4, #0x2f9]
0038c2f8: strb     r5, [r4, #0x2fa]
0038c2fc: strb     r5, [r4, #0x2fc]
0038c300: str      r5, [r4, #0x300]
0038c304: mov      r3, r5
0038c308: mov      r1, r5
0038c30c: mov      r0, r6
0038c310: str      ip, [sp]
0038c314: bl       #0x4a2730
0038c318: add      r3, r4, #0x358
0038c31c: mov      r0, r3
0038c320: str      r3, [r4, #0x368]
0038c324: str      r3, [r4, #0x36c]
0038c328: mov      r1, #0x10
0038c32c: bl       #0x31167c
0038c330: ldr      r1, [r4, #0x368]
0038c334: mov      r2, #0xc2000000
0038c338: mov      r3, #0x42000000
0038c33c: strb     r5, [r1]
0038c340: add      r2, r2, #0xc80000
0038c344: add      r3, r3, #0xc80000
0038c348: mov      r1, #0x370
0038c34c: strh     r7, [r4, r1]
0038c350: mov      r0, r4
0038c354: str      r2, [r4, #0x14c]
0038c358: str      r3, [r4, #0x158]
0038c35c: str      r2, [r4, #0x144]
0038c360: str      r2, [r4, #0x148]
0038c364: str      r3, [r4, #0x150]
0038c368: str      r3, [r4, #0x154]
0038c36c: strb     r5, [r4, #0x373]
0038c370: strb     r5, [r4, #0x372]
0038c374: bl       #0x38aac8
0038c378: mov      r0, r6
0038c37c: mov      r1, r4
0038c380: bl       #0x4a191c
0038c384: mov      r0, r4
0038c388: add      sp, sp, #0xc
0038c38c: pop      {r4, r5, r6, r7, pc}
0038c390: rsbeq    r8, r0, r8, asr #18
0038c394: andeq    r2, r0, r0, ror sp
# _ZN11PropertyMap20LoadOverridesFromXMLEP12TiXmlElement 0x513a00 168
00513a00: push     {r4, r5, r6, r7, r8, lr}
00513a04: subs     r7, r1, #0
00513a08: mov      r6, r0
00513a0c: beq      #0x513a70
00513a10: bl       #0x5134c0
00513a14: ldr      r4, [r0, #8]
00513a18: mov      r5, r0
00513a1c: cmp      r5, r4
00513a20: beq      #0x513a70
00513a24: ldr      r8, [r4, #0x24]
00513a28: mov      r0, r7
00513a2c: mov      r1, r8
00513a30: bl       #0x514c70
00513a34: mov      r1, r8
00513a38: mov      r2, r0
00513a3c: mov      r0, r6
00513a40: bl       #0x51387c
00513a44: ldr      r2, [r4, #0xc]
00513a48: cmp      r2, #0
00513a4c: bne      #0x513a58
00513a50: b        #0x513a74
00513a54: mov      r2, r3
00513a58: ldr      r3, [r2, #8]
00513a5c: cmp      r3, #0
00513a60: bne      #0x513a54
00513a64: mov      r4, r2
00513a68: cmp      r5, r4
00513a6c: bne      #0x513a24
00513a70: pop      {r4, r5, r6, r7, r8, pc}
00513a74: ldr      r3, [r4, #4]
00513a78: ldr      r1, [r3, #0xc]
00513a7c: cmp      r4, r1
00513a80: bne      #0x513a9c
00513a84: mov      r4, r3
00513a88: ldr      r3, [r3, #4]
00513a8c: ldr      r2, [r3, #0xc]
00513a90: cmp      r2, r4
00513a94: beq      #0x513a84
00513a98: ldr      r2, [r4, #0xc]
00513a9c: cmp      r2, r3
00513aa0: movne    r4, r3
00513aa4: b        #0x513a1c
# _ZN9Character7EnabledEv 0x3a598c 4
003a598c: b        #0x38ba38
# _ZN8PFObjectC1Ev 0x524644 268
00524644: ldr      r3, [pc, #0xf4]
00524648: ldr      r1, [pc, #0xf4]
0052464c: push     {r4, r5, r6, r7, r8, lr}
00524650: add      r3, pc, r3
00524654: ldr      lr, [r3, r1]
00524658: mov      r1, #8
0052465c: str      r1, [r0, #4]
00524660: mov      r2, #0
00524664: mov      ip, #0
00524668: mov      r5, #0x3f800000
0052466c: mov      r1, #2
00524670: str      ip, [r0]
00524674: str      ip, [r0, #0xc]
00524678: str      ip, [r0, #0x10]
0052467c: str      r2, [r0, #0x18]
00524680: str      r2, [r0, #0x1c]
00524684: str      r2, [r0, #0x20]
00524688: str      r1, [r0, #0x14]
0052468c: str      r5, [r0, #8]
00524690: ldr      r6, [lr]
00524694: mov      r4, r0
00524698: ldr      r0, [pc, #0xa8]
0052469c: str      r6, [r4, #0x24]
005246a0: ldr      r6, [lr, #4]
005246a4: ldr      r0, [r3, r0]
005246a8: ldr      r1, [pc, #0x9c]
005246ac: str      r6, [r4, #0x28]
005246b0: ldr      r7, [lr, #8]
005246b4: add      r6, r4, #0x38
005246b8: add      lr, r4, #0x8c
005246bc: add      r0, r0, #8
005246c0: add      r1, pc, r1
005246c4: str      r0, [r4, #0x4c]
005246c8: str      r2, [r4, #0x34]
005246cc: str      r2, [r4, #0x40]
005246d0: str      r2, [r4, #0x44]
005246d4: str      r2, [r4, #0x48]
005246d8: str      ip, [r4, #0x50]
005246dc: str      ip, [r4, #0x54]
005246e0: str      r2, [r4, #0x5c]
005246e4: str      r2, [r4, #0x60]
005246e8: str      r2, [r4, #0x64]
005246ec: str      r2, [r4, #0x68]
005246f0: mov      r0, lr
005246f4: str      r7, [r4, #0x2c]
005246f8: str      r6, [r4, #0x3c]
005246fc: str      r5, [r4, #0x58]
00524700: str      r5, [r4, #0x30]
00524704: str      r6, [r4, #0x38]
00524708: add      r1, r1, #1
0052470c: str      r2, [r4, #0x6c]
00524710: str      r2, [r4, #0x70]
00524714: str      ip, [r4, #0x7c]
00524718: str      r2, [r4, #0x88]
0052471c: str      r2, [r4, #0x74]
00524720: str      r2, [r4, #0x78]
00524724: str      r2, [r4, #0x80]
00524728: str      r2, [r4, #0x84]
0052472c: str      lr, [r4, #0x9c]
00524730: str      lr, [r4, #0xa0]
00524734: bl       #0x5244e8
00524738: mov      r0, r4
0052473c: pop      {r4, r5, r6, r7, r8, pc}
00524740: subeq    r0, r7, r0, asr #8
00524744: andeq    r4, r0, r0, asr #6
00524748: andeq    r1, r0, r8, ror r2
0052474c: eorseq   lr, ip, r0, lsr #6
# _ZN10GameObject21CheckSpawnProbabilityEv 0x38bd64 248
0038bd64: push     {r4, r5, lr}
0038bd68: sub      sp, sp, #0x14
0038bd6c: add      r4, sp, #4
0038bd70: mov      r1, r0
0038bd74: mov      r5, r0
0038bd78: mov      r0, r4
0038bd7c: bl       #0x33dd2c
0038bd80: mov      r0, r4
0038bd84: bl       #0x33ff54
0038bd88: ldr      r4, [pc, #0xc4]
0038bd8c: subs     r3, r0, #0
0038bd90: add      r4, pc, r4
0038bd94: beq      #0x38bdbc
0038bd98: ldr      r3, [r3]
0038bd9c: mov      lr, pc
0038bda0: ldr      pc, [r3, #0x28]
0038bda4: cmp      r0, #0
0038bda8: beq      #0x38bdbc
0038bdac: mvn      r0, #1
0038bdb0: str      r0, [r5, #0x270]
0038bdb4: add      sp, sp, #0x14
0038bdb8: pop      {r4, r5, pc}
0038bdbc: ldr      r0, [r5, #0x270]
0038bdc0: cmn      r0, #1
0038bdc4: bne      #0x38bdb4
0038bdc8: bl       #0x7fd794
0038bdcc: ldrb     r3, [r0, #5]
0038bdd0: cmp      r3, #0
0038bdd4: beq      #0x38be44
0038bdd8: ldr      r3, [r5, #0x108]
0038bddc: cmn      r3, #1
0038bde0: beq      #0x38be44
0038bde4: ldr      r0, [r5, #0xfc]
0038bde8: subs     r0, r0, #0
0038bdec: movne    r0, #1
0038bdf0: bl       #0x38bc3c
0038bdf4: str      r0, [r5, #0x270]
0038bdf8: ldr      r3, [r5, #0x274]
0038bdfc: cmp      r0, r3
0038be00: blt      #0x38bdac
0038be04: mov      r1, #0
0038be08: ldr      r3, [r5]
0038be0c: mov      r0, r5
0038be10: mov      lr, pc
0038be14: ldr      pc, [r3, #0x40]
0038be18: mov      r0, r5
0038be1c: bl       #0x33ddb4
0038be20: mov      r3, #0
0038be24: strb     r3, [r5, #0x82]
0038be28: ldr      r3, [pc, #0x28]
0038be2c: mov      r1, r5
0038be30: ldr      r3, [r4, r3]
0038be34: ldr      r0, [r3, #0x38]
0038be38: bl       #0x3432f8
0038be3c: ldr      r0, [r5, #0x270]
0038be40: b        #0x38bdb4
0038be44: mov      r0, #0
0038be48: bl       #0x38bc3c
0038be4c: str      r0, [r5, #0x270]
0038be50: b        #0x38bdf8
0038be54: rsbeq    r8, r0, r0, lsl #26
0038be58: strdeq   r3, r4, [r0], -r4
# _ZN9Character8DisabledEv 0x3a5974 24
003a5974: push     {r4, lr}
003a5978: mov      r4, r0
003a597c: bl       #0x38ba04
003a5980: mov      r0, r4
003a5984: pop      {r4, lr}
003a5988: b        #0x3a40b0
# _ZN11PropertyMap21LoadDefaultPropertiesEv 0x5136ec 284
005136ec: push     {r4, r5, r6, r7, r8, sb, sl, lr}
005136f0: ldr      r4, [pc, #0x108]
005136f4: ldr      r6, [pc, #0x108]
005136f8: sub      sp, sp, #0x20
005136fc: add      r4, pc, r4
00513700: ldr      r3, [r4, r6]
00513704: add      sb, r0, #4
00513708: mov      r8, r0
0051370c: ldr      r3, [r3]
00513710: add      r5, sp, #4
00513714: str      r3, [sp, #0x1c]
00513718: bl       #0x5134c0
0051371c: mov      r1, sb
00513720: mov      sl, r0
00513724: mov      r0, r5
00513728: bl       #0x32b918
0051372c: ldr      r7, [sl, #8]
00513730: cmp      sl, r7
00513734: beq      #0x51378c
00513738: ldr      r3, [r7, #0x28]
0051373c: cmp      r3, #0
00513740: beq      #0x513760
00513744: cmp      r8, #0
00513748: beq      #0x513760
0051374c: mov      r0, r3
00513750: mov      r1, r8
00513754: ldr      r3, [r3]
00513758: mov      lr, pc
0051375c: ldr      pc, [r3, #0xc]
00513760: ldr      r2, [r7, #0xc]
00513764: cmp      r2, #0
00513768: bne      #0x513774
0051376c: b        #0x5137c8
00513770: mov      r2, r3
00513774: ldr      r3, [r2, #8]
00513778: cmp      r3, #0
0051377c: bne      #0x513770
00513780: mov      r7, r2
00513784: cmp      sl, r7
00513788: bne      #0x513738
0051378c: cmp      sb, r5
00513790: beq      #0x5137a4
00513794: mov      r0, sb
00513798: ldr      r1, [sp, #0x18]
0051379c: ldr      r2, [sp, #0x14]
005137a0: bl       #0x3109e0
005137a4: mov      r0, r5
005137a8: bl       #0x318254
005137ac: ldr      r3, [r4, r6]
005137b0: ldr      r2, [sp, #0x1c]
005137b4: ldr      r3, [r3]
005137b8: cmp      r2, r3
005137bc: bne      #0x5137fc
005137c0: add      sp, sp, #0x20
005137c4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
005137c8: ldr      r3, [r7, #4]
005137cc: ldr      r1, [r3, #0xc]
005137d0: cmp      r7, r1
005137d4: bne      #0x5137f0
005137d8: mov      r7, r3
005137dc: ldr      r3, [r3, #4]
005137e0: ldr      r2, [r3, #0xc]
005137e4: cmp      r2, r7
005137e8: beq      #0x5137d8
005137ec: ldr      r2, [r7, #0xc]
005137f0: cmp      r2, r3
005137f4: movne    r7, r3
005137f8: b        #0x513730
005137fc: bl       #0x30e310
00513800: umaaleq  r1, r8, r4, r3
00513804: andeq    r4, r0, ip, lsr #1
# _ZN8RoomZone12RemoveObjectEP10GameObject 0x3968cc 76
003968cc: ldr      ip, [r0, #0x394]!
003968d0: cmp      ip, r0
003968d4: beq      #0x3968f4
003968d8: ldr      r3, [ip, #8]
003968dc: cmp      r3, r1
003968e0: beq      #0x3968f4
003968e4: ldr      ip, [ip]
003968e8: cmp      r0, ip
003968ec: bne      #0x3968d8
003968f0: mov      ip, r0
003968f4: cmp      r0, ip
003968f8: bxeq     lr
003968fc: ldr      r3, [ip]
00396900: ldr      r2, [ip, #4]
00396904: mov      r0, ip
00396908: mov      r1, #0xc
0039690c: str      r3, [r2]
00396910: str      r2, [r3, #4]
00396914: b        #0x708f00
# _ZN10GameObject9InitFinalEv 0x38cd48 408
0038cd48: push     {r4, r5, r6, r7, r8, lr}
0038cd4c: ldr      r5, [pc, #0x178]
0038cd50: ldr      r6, [pc, #0x178]
0038cd54: sub      sp, sp, #0x28
0038cd58: add      r5, pc, r5
0038cd5c: ldr      r3, [r5, r6]
0038cd60: mov      r4, r0
0038cd64: ldr      r3, [r3]
0038cd68: str      r3, [sp, #0x24]
0038cd6c: bl       #0x38bd64
0038cd70: ldr      r3, [r4, #0x274]
0038cd74: cmp      r0, r3
0038cd78: bge      #0x38ce94
0038cd7c: ldrb     r3, [r4, #0x81]
0038cd80: cmp      r3, #0
0038cd84: bne      #0x38ce94
0038cd88: ldrb     r3, [r4, #0x2ed]
0038cd8c: cmp      r3, #0
0038cd90: bne      #0x38ceb0
0038cd94: ldr      r1, [r4, #0x144]
0038cd98: ldr      r0, [r4, #0x150]
0038cd9c: bl       #0x30e3ac
0038cda0: ldr      r1, [r4, #0x148]
0038cda4: mov      r7, r0
0038cda8: ldr      r0, [r4, #0x154]
0038cdac: bl       #0x30e3ac
0038cdb0: mov      r8, r0
0038cdb4: mov      r1, r8
0038cdb8: mov      r0, r7
0038cdbc: bl       #0x30e70c
0038cdc0: cmp      r0, #0
0038cdc4: ldr      r0, [pc, #0x108]
0038cdc8: movne    r7, r8
0038cdcc: ldrb     r2, [r4, #0x84]
0038cdd0: add      r1, r4, #0x1c8
0038cdd4: add      r3, r4, #0x160
0038cdd8: ldr      r0, [r5, r0]
0038cddc: str      r7, [sp]
0038cde0: str      r4, [sp, #4]
0038cde4: bl       #0x526b18
0038cde8: ldr      r7, [r4, #0x44]
0038cdec: mov      r0, r7
0038cdf0: bl       #0x30de54
0038cdf4: mov      r1, r7
0038cdf8: add      r2, r7, r0
0038cdfc: add      r0, r4, #0x254
0038ce00: bl       #0x3109e0
0038ce04: ldr      r0, [r4, #0x2d8]
0038ce08: cmp      r0, #0
0038ce0c: beq      #0x38ce8c
0038ce10: bl       #0x38ba74
0038ce14: ldr      r3, [pc, #0xbc]
0038ce18: add      r7, sp, #0xc
0038ce1c: ldr      r2, [r4, #0x288]
0038ce20: ldr      r3, [r5, r3]
0038ce24: ldr      r1, [r4, #0x28c]
0038ce28: mov      r0, r7
0038ce2c: ldr      r3, [r3, #0x10]
0038ce30: ldr      r8, [r3, #0x1c]
0038ce34: str      r7, [sp, #0x1c]
0038ce38: str      r7, [sp, #0x20]
0038ce3c: add      r8, r8, #0x294
0038ce40: bl       #0x3116e8
0038ce44: mov      r0, r8
0038ce48: mov      r1, r7
0038ce4c: bl       #0x40c3cc
0038ce50: mov      r8, r0
0038ce54: mov      r0, r7
0038ce58: bl       #0x318254
0038ce5c: ldr      r3, [r4, #0x2d8]
0038ce60: str      r8, [r3, #0x40]
0038ce64: ldr      r3, [r4, #0x2d8]
0038ce68: cmp      r3, #0
0038ce6c: beq      #0x38ce8c
0038ce70: ldr      r0, [r3, #8]
0038ce74: cmp      r0, #0
0038ce78: beq      #0x38ce8c
0038ce7c: ldr      r1, [pc, #0x58]
0038ce80: add      r1, pc, r1
0038ce84: bl       #0x5984f4
0038ce88: str      r0, [r4, #0x180]
0038ce8c: mov      r0, r4
0038ce90: bl       #0x393ea0
0038ce94: ldr      r3, [r5, r6]
0038ce98: ldr      r2, [sp, #0x24]
0038ce9c: ldr      r3, [r3]
0038cea0: cmp      r2, r3
0038cea4: bne      #0x38cec8
0038cea8: add      sp, sp, #0x28
0038ceac: pop      {r4, r5, r6, r7, r8, pc}
0038ceb0: ldr      r3, [r4]
0038ceb4: mov      r0, r4
0038ceb8: mov      r1, #1
0038cebc: mov      lr, pc
0038cec0: ldr      pc, [r3, #0x40]
0038cec4: b        #0x38cd94
0038cec8: bl       #0x30e310
0038cecc: rsbeq    r7, r0, r8, lsr sp
0038ced0: andeq    r4, r0, ip, lsr #1
0038ced4: andeq    r1, r0, r4, lsl #4
0038ced8: strdeq   r3, r4, [r0], -r4
0038cedc: ldrsheq  r5, [r3], #-0x58
# _ZN8RoomZone9AddObjectEP10GameObject 0x39672c 116
0039672c: push     {r4, r5, lr}
00396730: mov      r4, r0
00396734: ldr      r3, [r4, #0x394]!
00396738: sub      sp, sp, #0xc
0039673c: mov      r5, r0
00396740: cmp      r3, r4
00396744: beq      #0x396764
00396748: ldr      r2, [r3, #8]
0039674c: cmp      r1, r2
00396750: beq      #0x396764
00396754: ldr      r3, [r3]
00396758: cmp      r4, r3
0039675c: bne      #0x396748
00396760: mov      r3, r4
00396764: cmp      r4, r3
00396768: beq      #0x396774
0039676c: add      sp, sp, #0xc
00396770: pop      {r4, r5, pc}
00396774: mov      r0, r4
00396778: str      r1, [sp, #4]
0039677c: bl       #0x39670c
00396780: ldr      r1, [sp, #4]
00396784: str      r1, [r0, #8]
00396788: ldr      r3, [r5, #0x398]
0039678c: str      r4, [r0]
00396790: str      r3, [r0, #4]
00396794: str      r0, [r3]
00396798: str      r0, [r5, #0x398]
0039679c: b        #0x39676c
# _ZN6glitch5scene10ISceneNode20getSceneNodeFromNameEPKc 0x5984f4 116
005984f4: push     {r4, r5, r6, lr}
005984f8: mov      r5, r1
005984fc: ldr      r3, [r0]
00598500: mov      r4, r0
00598504: mov      lr, pc
00598508: ldr      pc, [r3, #0x24]
0059850c: mov      r1, r5
00598510: bl       #0x30e6e8
00598514: cmp      r0, #0
00598518: bne      #0x598524
0059851c: mov      r0, r4
00598520: pop      {r4, r5, r6, pc}
00598524: mov      r6, r4
00598528: ldr      r4, [r6, #0xf4]!
0059852c: b        #0x598550
00598530: cmp      r4, #0
00598534: moveq    r0, r4
00598538: subne    r0, r4, #4
0059853c: mov      r1, r5
00598540: bl       #0x5984f4
00598544: cmp      r0, #0
00598548: bne      #0x598560
0059854c: ldr      r4, [r4]
00598550: cmp      r6, r4
00598554: bne      #0x598530
00598558: mov      r4, #0
0059855c: b        #0x59851c
00598560: mov      r4, r0
00598564: b        #0x59851c
# _ZN14PhysicalObject13disableFilterEv 0x46eb70 116
0046eb70: push     {r4, lr}
0046eb74: ldrb     r3, [r0, #0x26]
0046eb78: mov      r4, r0
0046eb7c: cmp      r3, #0
0046eb80: bne      #0x46ebd8
0046eb84: ldr      r2, [r0, #0x18]
0046eb88: cmp      r2, #0
0046eb8c: beq      #0x46ebac
0046eb90: strh     r3, [r2, #0x26]
0046eb94: strh     r3, [r2, #0x24]
0046eb98: strh     r3, [r2, #0x22]
0046eb9c: ldr      r3, [r0, #4]
0046eba0: ldr      r1, [r0, #0x18]
0046eba4: ldr      r0, [r3, #0x10]
0046eba8: bl       #0x7e7afc
0046ebac: ldr      r3, [r4, #0x1c]
0046ebb0: cmp      r3, #0
0046ebb4: beq      #0x46ebd8
0046ebb8: mov      r2, #0
0046ebbc: strh     r2, [r3, #0x26]
0046ebc0: strh     r2, [r3, #0x24]
0046ebc4: strh     r2, [r3, #0x22]
0046ebc8: ldr      r3, [r4, #4]
0046ebcc: ldr      r1, [r4, #0x1c]
0046ebd0: ldr      r0, [r3, #0x10]
0046ebd4: bl       #0x7e7afc
0046ebd8: mov      r3, #1
0046ebdc: strb     r3, [r4, #0x26]
0046ebe0: pop      {r4, pc}
# _ZN10ObjectBaseC1ENS_6GO_IDSE 0x33f15c 436
0033f15c: push     {r4, r5, r6, r7, r8, lr}
0033f160: ldr      r6, [pc, #0x198]
0033f164: ldr      r2, [pc, #0x198]
0033f168: ldr      r3, [pc, #0x198]
0033f16c: add      r6, pc, r6
0033f170: ldr      r2, [r6, r2]
0033f174: ldr      r3, [r6, r3]
0033f178: mov      r4, r0
0033f17c: add      r2, r2, #8
0033f180: add      r0, r3, #8
0033f184: add      r3, r4, #8
0033f188: str      r2, [r4]
0033f18c: str      r0, [r4, #4]
0033f190: mov      r7, r1
0033f194: mov      r0, r3
0033f198: str      r3, [r4, #0x18]
0033f19c: str      r3, [r4, #0x1c]
0033f1a0: mov      r1, #0x10
0033f1a4: bl       #0x31167c
0033f1a8: ldr      r2, [pc, #0x15c]
0033f1ac: ldr      r1, [r4, #0x18]
0033f1b0: mov      r5, #0
0033f1b4: ldr      r2, [r6, r2]
0033f1b8: strb     r5, [r1]
0033f1bc: add      r3, r4, #0x30
0033f1c0: add      r1, r2, #0x74
0033f1c4: add      r0, r2, #8
0033f1c8: add      r2, r2, #0x68
0033f1cc: stm      r4, {r0, r2}
0033f1d0: str      r1, [r4, #0x24]
0033f1d4: mov      r0, r3
0033f1d8: str      r5, [r4, #0x20]
0033f1dc: strb     r5, [r4, #0x28]
0033f1e0: strb     r5, [r4, #0x29]
0033f1e4: str      r5, [r4, #0x2c]
0033f1e8: str      r3, [r4, #0x40]
0033f1ec: str      r3, [r4, #0x44]
0033f1f0: mov      r1, #0x10
0033f1f4: bl       #0x31167c
0033f1f8: ldr      r2, [r4, #0x40]
0033f1fc: add      r3, r4, #0x48
0033f200: mov      r0, r3
0033f204: strb     r5, [r2]
0033f208: mov      r1, #0x10
0033f20c: str      r3, [r4, #0x58]
0033f210: str      r3, [r4, #0x5c]
0033f214: bl       #0x31167c
0033f218: ldr      r2, [r4, #0x58]
0033f21c: add      r3, r4, #0x68
0033f220: mvn      r6, #0
0033f224: strb     r5, [r2]
0033f228: mov      r1, #0x10
0033f22c: mov      r0, r3
0033f230: strb     r5, [r4, #0x60]
0033f234: str      r3, [r4, #0x78]
0033f238: str      r3, [r4, #0x7c]
0033f23c: str      r6, [r4, #0x64]
0033f240: bl       #0x31167c
0033f244: ldr      r3, [r4, #0x78]
0033f248: add      r0, r4, #0x8c
0033f24c: strb     r5, [r3]
0033f250: mov      r3, #1
0033f254: strb     r3, [r4, #0x8a]
0033f258: strb     r5, [r4, #0x81]
0033f25c: strb     r5, [r4, #0x84]
0033f260: strb     r5, [r4, #0x85]
0033f264: strb     r5, [r4, #0x86]
0033f268: strb     r5, [r4, #0x88]
0033f26c: strb     r5, [r4, #0x89]
0033f270: bl       #0x33ed7c
0033f274: add      r0, r4, #0xb0
0033f278: bl       #0x33ed7c
0033f27c: add      r3, r4, #0xd4
0033f280: mov      r0, r3
0033f284: str      r3, [r4, #0xe4]
0033f288: str      r3, [r4, #0xe8]
0033f28c: mov      r1, #0x10
0033f290: bl       #0x31167c
0033f294: ldr      r3, [r4, #0xe4]
0033f298: mov      r1, r5
0033f29c: mov      r0, #0xc
0033f2a0: strb     r5, [r3]
0033f2a4: mov      r3, #0
0033f2a8: str      r3, [r4, #0x114]
0033f2ac: strb     r5, [r4, #0xf0]
0033f2b0: strb     r5, [r4, #0xf1]
0033f2b4: strb     r5, [r4, #0xf8]
0033f2b8: str      r5, [r4, #0xfc]
0033f2bc: str      r5, [r4, #0x100]
0033f2c0: str      r5, [r4, #0x104]
0033f2c4: strb     r5, [r4, #0x10c]
0033f2c8: strb     r5, [r4, #0x118]
0033f2cc: strb     r5, [r4, #0x119]
0033f2d0: str      r5, [r4, #0x11c]
0033f2d4: str      r7, [r4, #0xf4]
0033f2d8: str      r6, [r4, #0x110]
0033f2dc: str      r6, [r4, #0xec]
0033f2e0: str      r6, [r4, #0x108]
0033f2e4: bl       #0x310570
0033f2e8: mov      r5, r0
0033f2ec: bl       #0x33f50c
0033f2f0: str      r5, [r4, #0x2c]
0033f2f4: mov      r0, r4
0033f2f8: str      r4, [r5, #4]
0033f2fc: pop      {r4, r5, r6, r7, r8, pc}
0033f300: rsbeq    r5, r5, r4, lsr #18
0033f304: andeq    r1, r0, ip, lsl #1
0033f308: ldrdeq   r3, r4, [r0], -ip
0033f30c: andeq    r3, r0, r4, lsl #23
# _ZN10ObjectBase8DisabledEv 0x33de34 48
0033de34: push     {r4, lr}
0033de38: mov      r1, #0
0033de3c: mov      r4, r0
0033de40: ldr      r3, [r0]
0033de44: mov      lr, pc
0033de48: ldr      pc, [r3, #0x3c]
0033de4c: mov      r0, r4
0033de50: ldr      r3, [r4]
0033de54: mov      r1, #0
0033de58: mov      lr, pc
0033de5c: ldr      pc, [r3, #0x40]
0033de60: pop      {r4, pc}
# _ZN18SimpleTypePropertyIbE10FromStringEPvPKc 0x33e598 24
0033e598: ldr      r3, [r0, #4]
0033e59c: mov      r0, #0
0033e5a0: strb     r0, [r1, r3]
0033e5a4: add      r1, r1, r3
0033e5a8: mov      r0, r2
0033e5ac: b        #0x30f020
# _ZN10ObjectBase9SetEnableEb 0x33ddc8 60
0033ddc8: ldrb     r2, [r0, #0x8a]
0033ddcc: push     {r4, lr}
0033ddd0: cmp      r2, r1
0033ddd4: beq      #0x33ddf0
0033ddd8: cmp      r1, #0
0033dddc: strb     r1, [r0, #0x8a]
0033dde0: bne      #0x33ddf4
0033dde4: ldr      r3, [r0]
0033dde8: mov      lr, pc
0033ddec: ldr      pc, [r3, #0x48]
0033ddf0: pop      {r4, pc}
0033ddf4: ldr      r3, [r0]
0033ddf8: mov      lr, pc
0033ddfc: ldr      pc, [r3, #0x44]
0033de00: pop      {r4, pc}
