
# _ZN6glitch7collada18ISceneNodeAnimator10updateTimeEj
00667c48: push     {r4, r5, r6, lr}
00667c4c: sub      sp, sp, #8
00667c50: ldr      r3, [r0]
00667c54: mov      r5, r0
00667c58: mov      r6, r1
00667c5c: mov      lr, pc
00667c60: ldr      pc, [r3, #0x44]
00667c64: subs     r4, r0, #0
00667c68: beq      #0x667cac
00667c6c: mov      r1, r6
00667c70: ldm      r4, {r3, r6}
00667c74: mov      lr, pc
00667c78: ldr      pc, [r3]
00667c7c: ldr      ip, [r5, #0x18]
00667c80: ldr      r2, [r4, #4]
00667c84: cmp      ip, #0
00667c88: beq      #0x667cac
00667c8c: ldr      lr, [r4, #0x14]
00667c90: ldr      r3, [r4, #0x10]
00667c94: mov      r0, ip
00667c98: mov      r1, r6
00667c9c: ldr      ip, [ip]
00667ca0: str      lr, [sp]
00667ca4: mov      lr, pc
00667ca8: ldr      pc, [ip, #0x10]
00667cac: add      sp, sp, #8
00667cb0: pop      {r4, r5, r6, pc}

# _ZN15AnimatorBlender17BlenderApplicator11AnimateNodeEj
00366888: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036688c: mov      r4, r0
00366890: ldr      r2, [r0, #0xc]
00366894: ldr      r0, [pc, #0x1b4]
00366898: sub      sp, sp, #0x3c
0036689c: mov      r3, #0
003668a0: add      r0, pc, r0
003668a4: cmn      r2, #1
003668a8: str      r3, [r4, #0x2c]
003668ac: str      r0, [sp, #0x10]
003668b0: str      r1, [sp, #0xc]
003668b4: str      r3, [sp, #0x2c]
003668b8: str      r3, [sp, #0x30]
003668bc: str      r3, [sp, #0x34]
003668c0: str      r3, [r4, #0x24]
003668c4: str      r3, [r4, #0x28]
003668c8: beq      #0x366a48
003668cc: ldr      r3, [r4, #0x3c]
003668d0: ldr      r1, [r3, #0x2c]
003668d4: ldr      r2, [r3, #0x28]
003668d8: rsb      r3, r2, r1
003668dc: cmp      r3, #3
003668e0: ble      #0x366a48
003668e4: ldr      r3, [pc, #0x168]
003668e8: ldr      r1, [pc, #0x168]
003668ec: mov      r6, #0
003668f0: str      r3, [sp, #0x18]
003668f4: ldr      r3, [pc, #0x160]
003668f8: str      r1, [sp, #0x14]
003668fc: add      r7, sp, #0x2c
00366900: add      r3, pc, r3
00366904: str      r3, [sp, #0x1c]
00366908: ldr      r3, [pc, #0x150]
0036690c: add      r3, pc, r3
00366910: str      r3, [sp, #0x20]
00366914: ldr      r3, [pc, #0x148]
00366918: add      r3, pc, r3
0036691c: str      r3, [sp, #0x24]
00366920: b        #0x3669b4
00366924: mov      r2, r7
00366928: mov      r0, r5
0036692c: ldr      r1, [sp, #0xc]
00366930: bl       #0x364444
00366934: ldr      sl, [r4, #0x3c]
00366938: ldr      r1, [r5, #0x28]
0036693c: add      r6, r6, #1
00366940: ldr      r3, [sl, #0x34]
00366944: ldr      r8, [r3, r8]
00366948: mov      r0, r8
0036694c: bl       #0x30ed6c
00366950: ldr      r1, [r5, #0x2c]
00366954: mov      fp, r0
00366958: mov      r0, r8
0036695c: bl       #0x30ed6c
00366960: ldr      r1, [r5, #0x24]
00366964: mov      sb, r0
00366968: mov      r0, r8
0036696c: bl       #0x30ed6c
00366970: mov      r1, r0
00366974: ldr      r0, [r4, #0x24]
00366978: bl       #0x30eba4
0036697c: mov      r1, fp
00366980: str      r0, [r4, #0x24]
00366984: ldr      r0, [r4, #0x28]
00366988: bl       #0x30eba4
0036698c: mov      r1, sb
00366990: str      r0, [r4, #0x28]
00366994: ldr      r0, [r4, #0x2c]
00366998: bl       #0x30eba4
0036699c: str      r0, [r4, #0x2c]
003669a0: ldr      r3, [sl, #0x2c]
003669a4: ldr      r2, [sl, #0x28]
003669a8: rsb      r3, r2, r3
003669ac: cmp      r6, r3, asr #2
003669b0: bge      #0x366a48
003669b4: ldr      r5, [r2, r6, lsl #2]
003669b8: lsl      r8, r6, #2
003669bc: ldr      r3, [r5]
003669c0: mov      r0, r5
003669c4: mov      lr, pc
003669c8: ldr      pc, [r3, #0x44]
003669cc: ldr      ip, [r5]
003669d0: ldr      r2, [r0, #4]
003669d4: mov      r3, r7
003669d8: mov      r0, r5
003669dc: ldr      r1, [r4, #0xc]
003669e0: mov      lr, pc
003669e4: ldr      pc, [ip, #0x7c]
003669e8: mov      r0, r5
003669ec: bl       #0x369160
003669f0: subs     r5, r0, #0
003669f4: bne      #0x366924
003669f8: ldr      r0, [sp, #0x10]
003669fc: ldr      ip, [sp, #0x14]
00366a00: ldr      r3, [r0, ip]
00366a04: ldr      r3, [r3]
00366a08: cmp      r3, #2
00366a0c: streq    r5, [r5]
00366a10: beq      #0x366924
00366a14: cmp      r3, #1
00366a18: bne      #0x366924
00366a1c: ldr      r2, [sp, #0x10]
00366a20: ldr      r1, [sp, #0x18]
00366a24: movw     ip, #0x18a
00366a28: ldr      r3, [sp, #0x24]
00366a2c: ldr      r0, [r2, r1]
00366a30: ldr      r1, [sp, #0x1c]
00366a34: ldr      r2, [sp, #0x20]
00366a38: add      r0, r0, #0xa8
00366a3c: str      ip, [sp]
00366a40: bl       #0x30e004
00366a44: b        #0x366924
00366a48: add      sp, sp, #0x3c
00366a4c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN11AnimatorSet10updateTimeEj
003673e4: push     {r4, lr}
003673e8: ldr      r3, [r0, #0x98]
003673ec: mov      r4, r0
003673f0: cmp      r3, #0
003673f4: ldrne    r3, [r3, #0x20]
003673f8: str      r3, [r0, #0x50]
003673fc: bl       #0x667c48
00367400: ldr      r3, [r4]
00367404: mov      r0, r4
00367408: mov      lr, pc
0036740c: ldr      pc, [r3, #0x44]
00367410: mov      r1, r0
00367414: add      r0, r4, #0x58
00367418: pop      {r4, lr}
0036741c: b        #0x36440c

# _ZN13RootSceneNode19_HandleDisplacementEj
0035cf48: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0035cf4c: ldr      r3, [r0, #0x1f0]
0035cf50: sub      sp, sp, #0x4c
0035cf54: mov      r4, r0
0035cf58: mov      r0, r3
0035cf5c: ldr      r3, [r3]
0035cf60: mov      lr, pc
0035cf64: ldr      pc, [r3, #0xa0]
0035cf68: mov      r3, r0
0035cf6c: ldr      r2, [r3]
0035cf70: add      r8, sp, #0x3c
0035cf74: ldr      r5, [r3, #8]
0035cf78: mov      r7, #0
0035cf7c: str      r2, [sp, #4]
0035cf80: mov      r1, r8
0035cf84: mov      r0, r4
0035cf88: ldr      r6, [r3, #4]
0035cf8c: str      r7, [sp, #0x3c]
0035cf90: str      r7, [sp, #0x40]
0035cf94: str      r7, [sp, #0x44]
0035cf98: bl       #0x35cd5c
0035cf9c: ldr      r1, [r4, #0xc8]
0035cfa0: ldr      r0, [sp, #0x3c]
0035cfa4: bl       #0x30ed6c
0035cfa8: str      r0, [sp, #0x3c]
0035cfac: ldr      r1, [r4, #0xcc]
0035cfb0: ldr      r0, [sp, #0x40]
0035cfb4: bl       #0x30ed6c
0035cfb8: mov      r2, r8
0035cfbc: add      r1, r4, #0xb8
0035cfc0: str      r0, [sp, #0x40]
0035cfc4: add      r0, sp, #0x30
0035cfc8: str      r7, [sp, #0x44]
0035cfcc: bl       #0x35bc90
0035cfd0: ldr      r2, [sp, #0x30]
0035cfd4: ldr      r3, [r4]
0035cfd8: mov      r0, r4
0035cfdc: str      r2, [sp, #0x3c]
0035cfe0: ldr      r2, [sp, #0x34]
0035cfe4: str      r2, [sp, #0x40]
0035cfe8: ldr      r2, [sp, #0x38]
0035cfec: str      r2, [sp, #0x44]
0035cff0: ldr      sl, [r3, #0xa4]
0035cff4: mov      lr, pc
0035cff8: ldr      pc, [r3, #0xa0]
0035cffc: ldr      r1, [sp, #0x40]
0035d000: mov      r8, r0
0035d004: ldr      r0, [r0, #4]
0035d008: bl       #0x30eba4
0035d00c: ldr      r1, [sp, #0x44]
0035d010: mov      fp, r0
0035d014: ldr      r0, [r8, #8]
0035d018: bl       #0x30eba4
0035d01c: ldr      r1, [sp, #0x3c]
0035d020: mov      sb, r0
0035d024: ldr      r0, [r8]
0035d028: bl       #0x30eba4
0035d02c: str      fp, [sp, #0x28]
0035d030: str      r0, [sp, #0x24]
0035d034: str      sb, [sp, #0x2c]
0035d038: mov      r0, r4
0035d03c: add      r1, sp, #0x24
0035d040: blx      sl
0035d044: ldr      r0, [r4, #0x1f8]
0035d048: cmp      r0, #0
0035d04c: beq      #0x35d0e8
0035d050: ldr      r1, [sp, #4]
0035d054: ldr      r3, [r0]
0035d058: add      r6, r6, #0x80000000
0035d05c: add      r2, r1, #0x80000000
0035d060: add      r5, r5, #0x80000000
0035d064: ldr      r3, [r3, #0xa4]
0035d068: add      r1, sp, #0x18
0035d06c: str      r2, [sp, #0x18]
0035d070: str      r6, [sp, #0x1c]
0035d074: str      r5, [sp, #0x20]
0035d078: blx      r3
0035d07c: ldr      r3, [r4, #0x1f8]
0035d080: mov      r1, #0
0035d084: mov      r0, r3
0035d088: ldr      r3, [r3]
0035d08c: mov      lr, pc
0035d090: ldr      pc, [r3, #0xb8]
0035d094: ldr      r0, [sp, #0x3c]
0035d098: mov      r1, #0
0035d09c: bl       #0x30df8c
0035d0a0: cmp      r0, #0
0035d0a4: beq      #0x35d0dc
0035d0a8: ldr      r0, [sp, #0x40]
0035d0ac: mov      r1, #0
0035d0b0: bl       #0x30df8c
0035d0b4: cmp      r0, #0
0035d0b8: beq      #0x35d0dc
0035d0bc: ldr      r0, [sp, #0x44]
0035d0c0: mov      r1, #0
0035d0c4: bl       #0x30df8c
0035d0c8: cmp      r0, #0
0035d0cc: mov      r0, #0
0035d0d0: moveq    r0, #1
0035d0d4: uxtb     r0, r0
0035d0d8: b        #0x35d0e0
0035d0dc: mov      r0, #1
0035d0e0: add      sp, sp, #0x4c
0035d0e4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0035d0e8: ldr      r0, [r4, #0x1f0]
0035d0ec: mov      r1, r7
0035d0f0: mov      r2, r7
0035d0f4: mov      r3, r5
0035d0f8: bl       #0x597154
0035d0fc: ldr      r4, [r4, #0x1f4]
0035d100: cmp      r4, #0
0035d104: beq      #0x35d094
0035d108: ldr      r3, [r4]
0035d10c: mov      r0, r4
0035d110: ldr      r8, [r3, #0xa4]
0035d114: mov      lr, pc
0035d118: ldr      pc, [r3, #0xa0]
0035d11c: mov      r1, r6
0035d120: mov      r7, r0
0035d124: ldr      r0, [r0, #4]
0035d128: bl       #0x30e3ac
0035d12c: mov      r1, r5
0035d130: mov      r6, r0
0035d134: ldr      r0, [r7, #8]
0035d138: bl       #0x30e3ac
0035d13c: ldr      r1, [sp, #4]
0035d140: mov      r5, r0
0035d144: ldr      r0, [r7]
0035d148: bl       #0x30e3ac
0035d14c: str      r6, [sp, #0x10]
0035d150: str      r0, [sp, #0xc]
0035d154: str      r5, [sp, #0x14]
0035d158: mov      r0, r4
0035d15c: add      r1, sp, #0xc
0035d160: blx      r8
0035d164: b        #0x35d094

# _ZN12CharAnimator6UpdateEv
003caf3c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003caf40: ldr      r5, [pc, #0x37c]
003caf44: ldr      r6, [pc, #0x37c]
003caf48: mov      r4, r0
003caf4c: add      r5, pc, r5
003caf50: ldr      r3, [r5, r6]
003caf54: ldr      r0, [pc, #0x370]
003caf58: sub      sp, sp, #0x78
003caf5c: ldr      r3, [r3]
003caf60: add      r0, pc, r0
003caf64: str      r3, [sp, #0x74]
003caf68: bl       #0x3136b4
003caf6c: ldr      r3, [r4, #4]
003caf70: ldr      r3, [r3, #0x520]
003caf74: tst      r3, #0x200
003caf78: bne      #0x3cafb8
003caf7c: ldrb     r3, [r4, #0x5c]
003caf80: cmp      r3, #0
003caf84: bne      #0x3cb128
003caf88: ldr      r0, [pc, #0x340]
003caf8c: mov      r3, #0
003caf90: strb     r3, [r4, #0x5c]
003caf94: add      r0, pc, r0
003caf98: bl       #0x3136b8
003caf9c: ldr      r3, [r5, r6]
003cafa0: ldr      r2, [sp, #0x74]
003cafa4: ldr      r3, [r3]
003cafa8: cmp      r2, r3
003cafac: bne      #0x3cb2c0
003cafb0: add      sp, sp, #0x78
003cafb4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003cafb8: ldrb     r3, [r4, #0x5c]
003cafbc: cmp      r3, #0
003cafc0: beq      #0x3cb11c
003cafc4: ldrb     r3, [r4, #0x4a]
003cafc8: mov      r2, #1
003cafcc: strb     r2, [r4, #0x5c]
003cafd0: cmp      r3, #0
003cafd4: movne    r3, #0
003cafd8: strbne   r3, [r4, #0x4a]
003cafdc: strbne   r2, [r4, #0x49]
003cafe0: beq      #0x3cb134
003cafe4: ldr      sl, [r4, #0x2c]
003cafe8: ldr      r3, [pc, #0x2e4]
003cafec: mov      r8, #0xc
003caff0: mla      r8, r8, sl, r4
003caff4: ldr      r3, [r5, r3]
003caff8: ldr      r2, [r8, #8]
003caffc: mov      r7, #0x14
003cb000: ldr      r3, [r3]
003cb004: ldr      r0, [r4, #4]
003cb008: mov      r1, #0x27
003cb00c: mla      r7, r7, r2, r3
003cb010: mov      r2, #0
003cb014: bl       #0x3a4d5c
003cb018: ldr      r3, [r7, #0x10]
003cb01c: cmp      r3, #1
003cb020: bne      #0x3cb144
003cb024: ldr      r3, [r8, #0x10]
003cb028: ldr      r2, [r7, #8]
003cb02c: add      r3, r3, #1
003cb030: cmp      r3, r2
003cb034: beq      #0x3cb144
003cb038: mov      r8, #0xc
003cb03c: mla      r8, r8, sl, r4
003cb040: str      r3, [r8, #0x10]
003cb044: ldr      r2, [r7, #8]
003cb048: cmp      r2, r3
003cb04c: bhi      #0x3cb1e4
003cb050: mov      r2, #0xc
003cb054: mla      r2, r2, sl, r4
003cb058: ldr      r3, [r2, #0xc]
003cb05c: cmp      r3, #0
003cb060: beq      #0x3cb174
003cb064: subgt    r3, r3, #1
003cb068: strgt    r3, [r2, #0xc]
003cb06c: ldr      r3, [pc, #0x264]
003cb070: add      r7, sp, #0x44
003cb074: ldr      r8, [r5, r3]
003cb078: mov      r0, r8
003cb07c: bl       #0x337888
003cb080: ldr      r1, [pc, #0x254]
003cb084: add      r2, sp, #0xc
003cb088: mov      r0, r7
003cb08c: add      r1, pc, r1
003cb090: bl       #0x3140ec
003cb094: mov      r1, r7
003cb098: mov      r0, r8
003cb09c: bl       #0x337a88
003cb0a0: mov      r0, r7
003cb0a4: bl       #0x3139ac
003cb0a8: mov      r2, #0
003cb0ac: ldr      r0, [r4, #4]
003cb0b0: mov      r1, #0x23
003cb0b4: bl       #0x3a4d5c
003cb0b8: ldr      r2, [r4, #0x50]
003cb0bc: mov      r3, #0xc
003cb0c0: mla      r3, r3, sl, r4
003cb0c4: cmn      r2, #1
003cb0c8: ldr      r7, [r3, #0xc]
003cb0cc: beq      #0x3cb2ac
003cb0d0: mov      r3, #0xc
003cb0d4: mla      sl, r3, sl, r4
003cb0d8: cmp      r7, #0
003cb0dc: mvnlt    r3, #0
003cb0e0: strlt    r3, [sl, #0xc]
003cb0e4: strge    r7, [sl, #0xc]
003cb0e8: mov      r3, #0
003cb0ec: strb     r3, [r4, #0x49]
003cb0f0: ldr      r1, [r4, #0x50]
003cb0f4: cmn      r1, #1
003cb0f8: beq      #0x3cb10c
003cb0fc: mov      r0, r4
003cb100: bl       #0x3cacb0
003cb104: mvn      r3, #0
003cb108: str      r3, [r4, #0x50]
003cb10c: ldr      r0, [pc, #0x1cc]
003cb110: add      r0, pc, r0
003cb114: bl       #0x3136b8
003cb118: b        #0x3caf9c
003cb11c: mov      r0, r4
003cb120: bl       #0x3c9168
003cb124: b        #0x3cafc4
003cb128: mov      r0, r4
003cb12c: bl       #0x3c91c8
003cb130: b        #0x3caf88
003cb134: ldrb     r3, [r4, #0x49]
003cb138: cmp      r3, #0
003cb13c: beq      #0x3cb0f0
003cb140: b        #0x3cafe4
003cb144: ldr      r0, [r4, #4]
003cb148: mov      r1, #0x25
003cb14c: mov      r2, #0
003cb150: bl       #0x3a4d5c
003cb154: ldr      r3, [r7, #0x10]
003cb158: cmp      r3, #1
003cb15c: bne      #0x3cb050
003cb160: add      r3, r3, #0xb
003cb164: mla      r3, r3, sl, r4
003cb168: ldr      r3, [r3, #0x10]
003cb16c: add      r3, r3, #1
003cb170: b        #0x3cb038
003cb174: ldr      r3, [r4, #0x2c]
003cb178: cmp      r3, #0
003cb17c: bne      #0x3cb258
003cb180: ldrb     r7, [r4, #0x48]
003cb184: cmp      r7, #0
003cb188: bne      #0x3cb0e8
003cb18c: ldr      r3, [pc, #0x144]
003cb190: add      r8, sp, #0x14
003cb194: ldr      sl, [r5, r3]
003cb198: mov      r0, sl
003cb19c: bl       #0x337888
003cb1a0: ldr      r1, [pc, #0x13c]
003cb1a4: add      r2, sp, #4
003cb1a8: mov      r0, r8
003cb1ac: add      r1, pc, r1
003cb1b0: bl       #0x3140ec
003cb1b4: mov      r1, r8
003cb1b8: mov      r0, sl
003cb1bc: bl       #0x337a88
003cb1c0: mov      r0, r8
003cb1c4: bl       #0x3139ac
003cb1c8: mov      r3, #1
003cb1cc: strb     r3, [r4, #0x48]
003cb1d0: mov      r2, r7
003cb1d4: ldr      r0, [r4, #4]
003cb1d8: mov      r1, #0x22
003cb1dc: bl       #0x3a4d5c
003cb1e0: b        #0x3cb0e8
003cb1e4: ldr      r3, [pc, #0xec]
003cb1e8: add      sl, sp, #0x5c
003cb1ec: ldr      sb, [r5, r3]
003cb1f0: mov      r0, sb
003cb1f4: bl       #0x337888
003cb1f8: ldr      r1, [pc, #0xe8]
003cb1fc: add      r2, sp, #0x10
003cb200: mov      r0, sl
003cb204: add      r1, pc, r1
003cb208: bl       #0x3140ec
003cb20c: mov      r1, sl
003cb210: mov      r0, sb
003cb214: bl       #0x337a88
003cb218: mov      r0, sl
003cb21c: bl       #0x3139ac
003cb220: mov      r1, #0x23
003cb224: ldr      r0, [r4, #4]
003cb228: mov      r2, #0
003cb22c: bl       #0x3a4d5c
003cb230: ldr      r1, [r8, #0x10]
003cb234: ldr      r3, [r7, #8]
003cb238: cmp      r1, r3
003cb23c: bhs      #0x3cb24c
003cb240: mov      r0, r4
003cb244: bl       #0x3ca79c
003cb248: b        #0x3cb0e8
003cb24c: mov      r0, r4
003cb250: bl       #0x3caf3c
003cb254: b        #0x3cb0e8
003cb258: ldr      r3, [pc, #0x78]
003cb25c: add      r7, sp, #0x2c
003cb260: ldr      r8, [r5, r3]
003cb264: mov      r0, r8
003cb268: bl       #0x337888
003cb26c: ldr      r1, [pc, #0x78]
003cb270: add      r2, sp, #8
003cb274: mov      r0, r7
003cb278: add      r1, pc, r1
003cb27c: bl       #0x3140ec
003cb280: mov      r1, r7
003cb284: mov      r0, r8
003cb288: bl       #0x337a88
003cb28c: mov      r0, r7
003cb290: bl       #0x3139ac
003cb294: ldr      r3, [r4, #0x2c]
003cb298: mov      r0, r4
003cb29c: sub      r3, r3, #1
003cb2a0: str      r3, [r4, #0x2c]
003cb2a4: bl       #0x3caf3c
003cb2a8: b        #0x3cb0e8
003cb2ac: ldr      r1, [r3, #8]
003cb2b0: mov      r0, r4
003cb2b4: ldr      r2, [r4, #0x2c]
003cb2b8: bl       #0x3cab38
003cb2bc: b        #0x3cb0d0
003cb2c0: bl       #0x30e310
003cb2c4: subseq   sb, ip, r4, asr #22
003cb2c8: andeq    r4, r0, ip, lsr #1
003cb2cc: subeq    sl, pc, r0, asr #4
003cb2d0: subeq    sl, pc, ip, lsl #4
003cb2d4: andeq    r3, r0, ip, ror ip
003cb2d8: andeq    r0, r0, r4, lsl #17
003cb2dc: subeq    sb, pc, r4, asr #31
003cb2e0: umaaleq  sl, pc, r0, r0
003cb2e4: subeq    sb, pc, r4, lsr #29
003cb2e8: subeq    sb, pc, ip, asr #28
003cb2ec: ldrdeq   sb, sl, [pc], #-0xd8

# _ZN6glitch7collada14CEventsManager16dispatchEventsExItLi30EEEviii
0060ea08: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0060ea0c: cmp      r1, r2
0060ea10: sub      sp, sp, #0x14
0060ea14: str      r2, [sp, #4]
0060ea18: mov      r4, r0
0060ea1c: mov      fp, r3
0060ea20: bgt      #0x60eadc
0060ea24: str      r1, [sp]
0060ea28: ldr      r5, [r0, #0x14]
0060ea2c: lsl      r7, r1, #3
0060ea30: lsl      sl, r1, #1
0060ea34: add      sb, sp, #8
0060ea38: ldr      r3, [r5, #0x14]
0060ea3c: ldr      r3, [r3, r7]
0060ea40: cmp      r3, #0
0060ea44: movgt    r6, #0
0060ea48: ble      #0x60eac0
0060ea4c: mov      r0, fp
0060ea50: bl       #0x30e964
0060ea54: ldr      r3, [r5, #0xc]
0060ea58: mov      r8, r0
0060ea5c: ldrh     r0, [r3, sl]
0060ea60: bl       #0x30e964
0060ea64: movw     r1, #0x5555
0060ea68: movt     r1, #0xc205
0060ea6c: bl       #0x30ed6c
0060ea70: mov      r1, r0
0060ea74: mov      r0, r8
0060ea78: bl       #0x30eba4
0060ea7c: bl       #0x30e4cc
0060ea80: str      r0, [sp, #8]
0060ea84: ldr      r3, [r5, #0x14]
0060ea88: ldr      r1, [r4, #0xc]
0060ea8c: mov      r0, sb
0060ea90: add      r3, r3, r7
0060ea94: ldr      r3, [r3, #4]
0060ea98: ldr      r3, [r3, r6, lsl #2]
0060ea9c: add      r6, r6, #1
0060eaa0: str      r3, [sp, #0xc]
0060eaa4: mov      lr, pc
0060eaa8: ldr      pc, [r4, #8]
0060eaac: ldr      r5, [r4, #0x14]
0060eab0: ldr      r3, [r5, #0x14]
0060eab4: ldr      r3, [r3, r7]
0060eab8: cmp      r6, r3
0060eabc: blt      #0x60ea4c
0060eac0: ldm      sp, {r2, r3}
0060eac4: add      r7, r7, #8
0060eac8: add      r2, r2, #1
0060eacc: cmp      r3, r2
0060ead0: str      r2, [sp]
0060ead4: add      sl, sl, #2
0060ead8: bge      #0x60ea38
0060eadc: add      sp, sp, #0x14
0060eae0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada14CEventsManager8onUpdateEii
0060ecb4: push     {r4, r5, r6, lr}
0060ecb8: ldr      r3, [r0, #8]
0060ecbc: mov      r4, r0
0060ecc0: mov      r5, r2
0060ecc4: cmp      r3, #0
0060ecc8: beq      #0x60ed0c
0060eccc: ldr      r3, [r0, #4]
0060ecd0: add      r3, r3, #1
0060ecd4: str      r3, [r0, #4]
0060ecd8: bl       #0x60e02c
0060ecdc: mov      r1, r5
0060ece0: mov      r6, r0
0060ece4: mov      r0, r4
0060ece8: bl       #0x60e02c
0060ecec: add      r1, r6, #1
0060ecf0: mov      r2, r0
0060ecf4: mov      r3, r5
0060ecf8: mov      r0, r4
0060ecfc: bl       #0x60ebb4
0060ed00: mov      r0, r4
0060ed04: pop      {r4, r5, r6, lr}
0060ed08: b        #0x31d584
0060ed0c: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada14CEventsManager16dispatchEventsExIhLi30EEEviii
0060eae4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0060eae8: cmp      r1, r2
0060eaec: sub      sp, sp, #0x14
0060eaf0: str      r2, [sp, #4]
0060eaf4: mov      r4, r0
0060eaf8: mov      fp, r3
0060eafc: bgt      #0x60ebac
0060eb00: ldr      r5, [r0, #0x14]
0060eb04: mov      sl, r1
0060eb08: lsl      r7, r1, #3
0060eb0c: add      sb, sp, #8
0060eb10: ldr      r3, [r5, #0x14]
0060eb14: ldr      r3, [r3, r7]
0060eb18: cmp      r3, #0
0060eb1c: movgt    r6, #0
0060eb20: ble      #0x60eb98
0060eb24: mov      r0, fp
0060eb28: bl       #0x30e964
0060eb2c: ldr      r3, [r5, #0xc]
0060eb30: mov      r8, r0
0060eb34: ldrb     r0, [r3, sl]
0060eb38: bl       #0x30e964
0060eb3c: movw     r1, #0x5555
0060eb40: movt     r1, #0xc205
0060eb44: bl       #0x30ed6c
0060eb48: mov      r1, r0
0060eb4c: mov      r0, r8
0060eb50: bl       #0x30eba4
0060eb54: bl       #0x30e4cc
0060eb58: str      r0, [sp, #8]
0060eb5c: ldr      r3, [r5, #0x14]
0060eb60: ldr      r1, [r4, #0xc]
0060eb64: mov      r0, sb
0060eb68: add      r3, r3, r7
0060eb6c: ldr      r3, [r3, #4]
0060eb70: ldr      r3, [r3, r6, lsl #2]
0060eb74: add      r6, r6, #1
0060eb78: str      r3, [sp, #0xc]
0060eb7c: mov      lr, pc
0060eb80: ldr      pc, [r4, #8]
0060eb84: ldr      r5, [r4, #0x14]
0060eb88: ldr      r3, [r5, #0x14]
0060eb8c: ldr      r3, [r3, r7]
0060eb90: cmp      r6, r3
0060eb94: blt      #0x60eb24
0060eb98: ldr      r2, [sp, #4]
0060eb9c: add      sl, sl, #1
0060eba0: add      r7, r7, #8
0060eba4: cmp      r2, sl
0060eba8: bge      #0x60eb10
0060ebac: add      sp, sp, #0x14
0060ebb0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN12CharAnimator10__CallbackEPN6glitch5scene19ITimelineControllerEPv
003c90ec: mov      r3, #1
003c90f0: strb     r3, [r1, #0x49]
003c90f4: bx       lr

# _ZN15AnimatorBlender11animateNodeEPN6glitch5scene10ISceneNodeEj
00366cb4: push     {r4, r5, r6, lr}
00366cb8: ldr      r3, [r0, #0x7c]
00366cbc: mov      r4, r0
00366cc0: mov      r5, r2
00366cc4: cmp      r3, #0
00366cc8: ldr      r0, [r0, #0x84]
00366ccc: blt      #0x366d18
00366cd0: rsb      r0, r0, r2
00366cd4: rsb      r0, r0, r3
00366cd8: cmp      r0, #0
00366cdc: str      r0, [r4, #0x7c]
00366ce0: ble      #0x366d6c
00366ce4: bl       #0x30e964
00366ce8: ldr      r1, [r4, #0x80]
00366cec: bl       #0x30ed6c
00366cf0: ldr      r2, [r4, #0x34]
00366cf4: ldr      ip, [r4, #0x74]
00366cf8: mov      r3, r0
00366cfc: mov      r1, r0
00366d00: str      r3, [r2, ip, lsl #2]
00366d04: mov      r0, #0x3f800000
00366d08: bl       #0x30e3ac
00366d0c: ldr      r2, [r4, #0x70]
00366d10: ldr      r3, [r4, #0x34]
00366d14: str      r0, [r3, r2, lsl #2]
00366d18: ldr      r3, [r4]
00366d1c: mov      r0, r4
00366d20: mov      r1, r5
00366d24: add      r6, r4, #0x88
00366d28: mov      lr, pc
00366d2c: ldr      pc, [r3, #0x50]
00366d30: mov      r1, r5
00366d34: mov      r0, r6
00366d38: bl       #0x366888
00366d3c: ldr      r2, [r4, #0x70]
00366d40: ldr      r3, [r4, #0x28]
00366d44: ldr      r3, [r3, r2, lsl #2]
00366d48: mov      r0, r3
00366d4c: ldr      r3, [r3]
00366d50: mov      lr, pc
00366d54: ldr      pc, [r3, #0x44]
00366d58: mov      r1, r0
00366d5c: mov      r0, r6
00366d60: bl       #0x36440c
00366d64: str      r5, [r4, #0x84]
00366d68: pop      {r4, r5, r6, pc}
00366d6c: ldr      r2, [r4, #0x74]
00366d70: ldr      r3, [r4, #0x34]
00366d74: mov      r1, #0
00366d78: str      r1, [r3, r2, lsl #2]
00366d7c: ldr      r2, [r4, #0x70]
00366d80: ldr      r3, [r4, #0x34]
00366d84: mov      r1, #0x3f800000
00366d88: str      r1, [r3, r2, lsl #2]
00366d8c: b        #0x366d18

# _ZN14AnimApplicator10ResetDeltaEj
003644fc: push     {r4, r5, lr}
00364500: ldr      r3, [r0, #8]
00364504: sub      sp, sp, #0x14
00364508: mov      r4, r0
0036450c: cmp      r3, #0
00364510: mov      r5, r1
00364514: beq      #0x364590
00364518: ldr      r1, [r0, #0xc]
0036451c: cmn      r1, #1
00364520: beq      #0x3645bc
00364524: ldr      r3, [r0, #4]
00364528: mov      r2, #0
0036452c: str      r2, [sp, #0xc]
00364530: cmp      r3, #0
00364534: str      r2, [sp, #4]
00364538: str      r2, [sp, #8]
0036453c: beq      #0x3645ac
00364540: mov      r0, r3
00364544: ldr      r3, [r3]
00364548: mov      lr, pc
0036454c: ldr      pc, [r3, #0x44]
00364550: ldr      r3, [r4, #4]
00364554: cmp      r0, #0
00364558: ldr      r1, [r4, #0xc]
0036455c: ldr      r2, [r3]
00364560: ldr      ip, [r2, #0x7c]
00364564: ldrne    r2, [r0, #0x10]
00364568: beq      #0x3645b4
0036456c: mov      r0, r3
00364570: add      r3, sp, #4
00364574: blx      ip
00364578: ldr      r2, [sp, #8]
0036457c: ldr      r3, [sp, #0xc]
00364580: ldr      r1, [sp, #4]
00364584: str      r2, [r4, #0x1c]
00364588: str      r3, [r4, #0x20]
0036458c: str      r1, [r4, #0x18]
00364590: mov      r3, #0
00364594: str      r5, [r4, #0x14]
00364598: str      r3, [r4, #0x2c]
0036459c: str      r3, [r4, #0x24]
003645a0: str      r3, [r4, #0x28]
003645a4: add      sp, sp, #0x14
003645a8: pop      {r4, r5, pc}
003645ac: ldr      r2, [r3]
003645b0: ldr      ip, [r2, #0x7c]
003645b4: mov      r2, r5
003645b8: b        #0x36456c
003645bc: mov      r0, r3
003645c0: ldr      r3, [r3]
003645c4: mov      lr, pc
003645c8: ldr      pc, [r3, #0xa0]
003645cc: ldr      r3, [r0]
003645d0: str      r3, [r4, #0x18]
003645d4: ldr      r3, [r0, #4]
003645d8: str      r3, [r4, #0x1c]
003645dc: ldr      r3, [r0, #8]
003645e0: str      r3, [r4, #0x20]
003645e4: b        #0x364590

# _ZN13RootSceneNode7NewAnimEb
0035d624: push     {r4, lr}
0035d628: mov      r4, r0
0035d62c: bl       #0x35d4cc
0035d630: ldrb     r3, [r4, #0x1ec]
0035d634: cmp      r3, #0
0035d638: beq      #0x35d654
0035d63c: ldr      r3, [r4, #0x1f0]
0035d640: cmp      r3, #0
0035d644: beq      #0x35d654
0035d648: ldr      r1, [r4, #0x1fc]
0035d64c: cmp      r1, #0
0035d650: bne      #0x35d658
0035d654: pop      {r4, pc}
0035d658: mov      r0, r4
0035d65c: add      r1, r1, #1
0035d660: bl       #0x35ce6c
0035d664: mov      r0, r4
0035d668: ldr      r3, [r4]
0035d66c: ldr      r1, [r4, #0x1fc]
0035d670: mov      lr, pc
0035d674: ldr      pc, [r3, #0x14]
0035d678: pop      {r4, pc}

# _ZN12CharAnimator13ANIM_SetSpeedEf
003c93fc: push     {r4, lr}
003c9400: ldr      r2, [r0, #4]
003c9404: str      r1, [r0, #0x40]
003c9408: mov      r3, r0
003c940c: ldr      r2, [r2, #0x2d8]
003c9410: cmp      r2, #0
003c9414: beq      #0x3c9440
003c9418: mov      r0, r1
003c941c: ldr      r1, [r3, #0x34]
003c9420: ldr      r4, [r2, #0x38]
003c9424: bl       #0x30ed6c
003c9428: ldr      r3, [r4]
003c942c: mov      r1, r0
003c9430: mov      r2, #0
003c9434: mov      r0, r4
003c9438: mov      lr, pc
003c943c: ldr      pc, [r3, #0x28]
003c9440: pop      {r4, pc}

# _ZN12CharAnimator9ANIM_SwapEii
003caccc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003cacd0: ldr      r4, [pc, #0x230]
003cacd4: cmn      r1, #1
003cacd8: sub      sp, sp, #0x24
003cacdc: add      r4, pc, r4
003cace0: mov      r5, r1
003cace4: mov      sb, r0
003cace8: beq      #0x3cad24
003cacec: cmn      r2, #1
003cacf0: beq      #0x3cad2c
003cacf4: ldr      sl, [r0, #8]
003cacf8: cmp      sl, r2
003cacfc: beq      #0x3cad30
003cad00: cmp      r1, sl
003cad04: beq      #0x3cad24
003cad08: ldr      r4, [r0, #0x40]
003cad0c: bl       #0x3cacb0
003cad10: mov      r0, sb
003cad14: mov      r1, r4
003cad18: add      sp, sp, #0x24
003cad1c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003cad20: b        #0x3c93fc
003cad24: add      sp, sp, #0x24
003cad28: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003cad2c: ldr      sl, [r0, #8]
003cad30: ldr      r3, [pc, #0x1d4]
003cad34: ldr      ip, [pc, #0x1d4]
003cad38: ldr      fp, [pc, #0x1d4]
003cad3c: add      r3, pc, r3
003cad40: str      r3, [sp, #0x10]
003cad44: ldr      r3, [pc, #0x1cc]
003cad48: mov      r6, sb
003cad4c: mov      r8, #0
003cad50: add      r3, pc, r3
003cad54: str      r3, [sp, #0x14]
003cad58: ldr      r3, [pc, #0x1bc]
003cad5c: mov      r7, r4
003cad60: add      r3, pc, r3
003cad64: str      r3, [sp, #0x18]
003cad68: ldr      r3, [pc, #0x1b0]
003cad6c: add      r3, pc, r3
003cad70: str      r3, [sp, #0x1c]
003cad74: ldr      r3, [r7, ip]
003cad78: mov      r4, #0x14
003cad7c: ldr      r3, [r3]
003cad80: str      r5, [r6, #8]
003cad84: mla      sl, r4, sl, r3
003cad88: mla      r4, r4, r5, r3
003cad8c: ldr      r3, [sl, #4]
003cad90: ldr      r2, [r4, #4]
003cad94: cmp      r2, r3
003cad98: beq      #0x3cadbc
003cad9c: ldr      r3, [r7, fp]
003cada0: ldr      r3, [r3]
003cada4: cmp      r3, #2
003cada8: moveq    r3, #0
003cadac: streq    r3, [r3]
003cadb0: beq      #0x3cadbc
003cadb4: cmp      r3, #1
003cadb8: beq      #0x3cae8c
003cadbc: ldr      r3, [sl, #8]
003cadc0: ldr      r2, [r4, #8]
003cadc4: cmp      r2, r3
003cadc8: beq      #0x3cadec
003cadcc: ldr      r3, [r7, fp]
003cadd0: ldr      r3, [r3]
003cadd4: cmp      r3, #2
003cadd8: moveq    r3, #0
003caddc: streq    r3, [r3]
003cade0: beq      #0x3cadec
003cade4: cmp      r3, #1
003cade8: beq      #0x3cae54
003cadec: ldr      r3, [sb, #0x2c]
003cadf0: cmp      r3, r8
003cadf4: strls    r5, [sb, #0x4c]
003cadf8: bls      #0x3cae3c
003cadfc: ldr      r1, [r6, #0x10]
003cae00: ldr      r2, [r4, #0xc]
003cae04: mov      r5, #0x38
003cae08: mla      r2, r5, r1, r2
003cae0c: ldr      r1, [r2, #0x28]
003cae10: cmp      r1, #1
003cae14: beq      #0x3cae38
003cae18: ldr      r1, [r7, fp]
003cae1c: ldr      r1, [r1]
003cae20: cmp      r1, #2
003cae24: moveq    r1, #0
003cae28: streq    r1, [r1]
003cae2c: beq      #0x3cae38
003cae30: cmp      r1, #1
003cae34: beq      #0x3caebc
003cae38: ldr      r5, [r2, #8]
003cae3c: add      r8, r8, #1
003cae40: cmp      r8, r3
003cae44: add      r6, r6, #0xc
003cae48: bhi      #0x3cad24
003cae4c: ldr      sl, [r6, #8]
003cae50: b        #0x3cad74
003cae54: ldr      r0, [pc, #0xc8]
003cae58: ldr      r2, [pc, #0xc8]
003cae5c: ldr      r3, [pc, #0xc8]
003cae60: ldr      r0, [r7, r0]
003cae64: mov      lr, #0x384
003cae68: add      r2, pc, r2
003cae6c: add      r3, pc, r3
003cae70: add      r0, r0, #0xa8
003cae74: ldr      r1, [sp, #0x1c]
003cae78: str      ip, [sp, #0xc]
003cae7c: str      lr, [sp]
003cae80: bl       #0x30e004
003cae84: ldr      ip, [sp, #0xc]
003cae88: b        #0x3cadec
003cae8c: ldr      r0, [pc, #0x90]
003cae90: movw     lr, #0x383
003cae94: ldr      r1, [sp, #0x10]
003cae98: ldr      r0, [r7, r0]
003cae9c: ldr      r2, [sp, #0x14]
003caea0: ldr      r3, [sp, #0x18]
003caea4: add      r0, r0, #0xa8
003caea8: str      ip, [sp, #0xc]
003caeac: str      lr, [sp]
003caeb0: bl       #0x30e004
003caeb4: ldr      ip, [sp, #0xc]
003caeb8: b        #0x3cadbc
003caebc: ldr      r0, [pc, #0x60]
003caec0: ldr      r1, [pc, #0x68]
003caec4: ldr      r2, [pc, #0x68]
003caec8: ldr      r0, [r7, r0]
003caecc: ldr      r3, [pc, #0x64]
003caed0: add      r1, pc, r1
003caed4: add      r2, pc, r2
003caed8: add      r3, pc, r3
003caedc: movw     lr, #0x38b
003caee0: add      r0, r0, #0xa8
003caee4: str      ip, [sp, #0xc]
003caee8: str      lr, [sp]
003caeec: bl       #0x30e004
003caef0: ldr      r2, [r4, #0xc]
003caef4: ldr      r1, [r6, #0x10]
003caef8: ldr      r3, [sb, #0x2c]
003caefc: ldr      ip, [sp, #0xc]
003caf00: mla      r2, r5, r1, r2
003caf04: b        #0x3cae38
003caf08: ldrheq   sb, [ip], #-0xd4
003caf0c: umaaleq  r3, pc, ip, r6
003caf10: andeq    r3, r0, ip, ror ip
003caf14: andeq    r3, r0, r0, asr #19
003caf18: subeq    sl, pc, r8, lsl r3
003caf1c: subeq    sl, pc, r0, lsr #4
003caf20: subeq    r3, pc, ip, ror #12
003caf24: andeq    r1, r0, r0, asr #19
003caf28: subeq    sl, pc, r8, asr r2
003caf2c: subeq    sl, pc, r4, lsl r1
003caf30: subeq    r3, pc, r8, lsl #10
003caf34: subeq    sl, pc, ip, asr #4
003caf38: subeq    sl, pc, r8, lsr #1

# _ZN14AnimApplicator11AnimateNodeEj
003645e8: push     {r4, r5, r6, r7, lr}
003645ec: ldr      r3, [r0, #8]
003645f0: sub      sp, sp, #0x14
003645f4: mov      r4, r0
003645f8: cmp      r3, #0
003645fc: mov      r5, r1
00364600: beq      #0x3646c0
00364604: ldr      r1, [r0, #0xc]
00364608: mov      r2, #0
0036460c: str      r2, [sp, #0xc]
00364610: cmn      r1, #1
00364614: str      r2, [sp, #4]
00364618: str      r2, [sp, #8]
0036461c: beq      #0x3646f0
00364620: ldr      r3, [r0, #4]
00364624: cmp      r3, #0
00364628: beq      #0x3646e0
0036462c: mov      r0, r3
00364630: ldr      r3, [r3]
00364634: mov      lr, pc
00364638: ldr      pc, [r3, #0x44]
0036463c: ldr      r3, [r4, #4]
00364640: cmp      r0, #0
00364644: ldr      r1, [r4, #0xc]
00364648: ldr      r2, [r3]
0036464c: ldr      ip, [r2, #0x7c]
00364650: ldrne    r2, [r0, #4]
00364654: beq      #0x3646e8
00364658: mov      r0, r3
0036465c: add      r3, sp, #4
00364660: blx      ip
00364664: ldr      r3, [r4, #0x14]
00364668: cmp      r3, r5
0036466c: beq      #0x3646cc
00364670: ldr      r0, [sp, #8]
00364674: ldr      r1, [r4, #0x1c]
00364678: bl       #0x30e3ac
0036467c: ldr      r1, [r4, #0x20]
00364680: mov      r7, r0
00364684: ldr      r0, [sp, #0xc]
00364688: bl       #0x30e3ac
0036468c: ldr      r1, [r4, #0x18]
00364690: mov      r6, r0
00364694: ldr      r0, [sp, #4]
00364698: bl       #0x30e3ac
0036469c: str      r7, [r4, #0x28]
003646a0: str      r0, [r4, #0x24]
003646a4: str      r6, [r4, #0x2c]
003646a8: ldr      r2, [sp, #8]
003646ac: ldr      r3, [sp, #0xc]
003646b0: ldr      r1, [sp, #4]
003646b4: str      r2, [r4, #0x1c]
003646b8: str      r3, [r4, #0x20]
003646bc: str      r1, [r4, #0x18]
003646c0: str      r5, [r4, #0x14]
003646c4: add      sp, sp, #0x14
003646c8: pop      {r4, r5, r6, r7, pc}
003646cc: mov      r3, #0
003646d0: str      r3, [r4, #0x2c]
003646d4: str      r3, [r4, #0x24]
003646d8: str      r3, [r4, #0x28]
003646dc: b        #0x3646a8
003646e0: ldr      r2, [r3]
003646e4: ldr      ip, [r2, #0x7c]
003646e8: mov      r2, r5
003646ec: b        #0x364658
003646f0: mov      r0, r3
003646f4: ldr      r3, [r3]
003646f8: mov      lr, pc
003646fc: ldr      pc, [r3, #0xa0]
00364700: ldr      r3, [r0]
00364704: str      r3, [sp, #4]
00364708: ldr      r3, [r0, #4]
0036470c: str      r3, [sp, #8]
00364710: ldr      r3, [r0, #8]
00364714: str      r3, [sp, #0xc]
00364718: b        #0x364664

# _ZN15AnimatorBlender17BlenderApplicator10ResetDeltaEj
00366a68: push     {r4, r5, r6, r7, r8, sl, lr}
00366a6c: ldr      r3, [r0, #8]
00366a70: ldr      sl, [pc, #0xf0]
00366a74: sub      sp, sp, #0x1c
00366a78: cmp      r3, #0
00366a7c: mov      r5, r0
00366a80: mov      r7, r1
00366a84: add      sl, pc, sl
00366a88: beq      #0x366b0c
00366a8c: ldr      r3, [r0, #0x3c]
00366a90: ldr      r2, [r3, #0x28]
00366a94: ldr      r3, [r3, #0x70]
00366a98: ldr      r4, [r2, r3, lsl #2]
00366a9c: ldr      r3, [r4]
00366aa0: mov      r0, r4
00366aa4: mov      lr, pc
00366aa8: ldr      pc, [r3, #0x44]
00366aac: mov      r8, r0
00366ab0: mov      r0, r4
00366ab4: bl       #0x369160
00366ab8: subs     r6, r0, #0
00366abc: beq      #0x366b14
00366ac0: ldr      r1, [r5, #0xc]
00366ac4: mov      r3, #0
00366ac8: str      r3, [sp, #0x14]
00366acc: cmn      r1, #1
00366ad0: str      r3, [sp, #0xc]
00366ad4: str      r3, [sp, #0x10]
00366ad8: beq      #0x366af4
00366adc: mov      r0, r4
00366ae0: ldr      r2, [r8, #0x10]
00366ae4: ldr      ip, [r4]
00366ae8: add      r3, sp, #0xc
00366aec: mov      lr, pc
00366af0: ldr      pc, [ip, #0x7c]
00366af4: cmp      r6, #0
00366af8: beq      #0x366b0c
00366afc: mov      r0, r6
00366b00: mov      r1, r7
00366b04: add      r2, sp, #0xc
00366b08: bl       #0x3644cc
00366b0c: add      sp, sp, #0x1c
00366b10: pop      {r4, r5, r6, r7, r8, sl, pc}
00366b14: ldr      r3, [pc, #0x50]
00366b18: ldr      r3, [sl, r3]
00366b1c: ldr      r3, [r3]
00366b20: cmp      r3, #2
00366b24: streq    r6, [r6]
00366b28: beq      #0x366ac0
00366b2c: cmp      r3, #1
00366b30: bne      #0x366ac0
00366b34: ldr      r0, [pc, #0x34]
00366b38: ldr      r1, [pc, #0x34]
00366b3c: ldr      r2, [pc, #0x34]
00366b40: ldr      r0, [sl, r0]
00366b44: ldr      r3, [pc, #0x30]
00366b48: movw     ip, #0x167
00366b4c: add      r1, pc, r1
00366b50: add      r2, pc, r2
00366b54: add      r3, pc, r3
00366b58: add      r0, r0, #0xa8
00366b5c: str      ip, [sp]
00366b60: bl       #0x30e004
00366b64: b        #0x366ac0
00366b68: rsbeq    lr, r2, ip
00366b6c: andeq    r3, r0, r0, asr #19
00366b70: andeq    r1, r0, r0, asr #19
00366b74: subseq   r7, r5, ip, lsl #17
00366b78: ldrsbeq  r6, [r7], #-0x78
00366b7c: subseq   sl, r5, r4, lsr #5

# _ZN11AnimatorSet20applyAnimationValuesEj
0036737c: ldr      r3, [r0, #0x98]
00367380: cmp      r3, #0
00367384: ldrne    r3, [r3, #0x20]
00367388: str      r3, [r0, #0x50]
0036738c: b        #0x65f418

# _ZN6glitch7collada21CSceneNodeAnimatorSet20applyAnimationValuesEj
0065f418: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0065f41c: ldr      r3, [r0, #0x24]
0065f420: sub      sp, sp, #0x44
0065f424: mov      r4, r0
0065f428: ldr      r3, [r3, #0x3c]
0065f42c: mov      r5, r1
0065f430: cmp      r3, #0
0065f434: bne      #0x65f444
0065f438: ldr      r3, [r0, #0x18]
0065f43c: cmp      r3, #0
0065f440: beq      #0x65f5c0
0065f444: mov      r0, r4
0065f448: mov      r1, r5
0065f44c: bl       #0x667c48
0065f450: ldr      r3, [r4]
0065f454: mov      r0, r4
0065f458: mov      lr, pc
0065f45c: ldr      pc, [r3, #0x44]
0065f460: cmp      r0, #0
0065f464: beq      #0x65f5c8
0065f468: ldr      r0, [r0, #4]
0065f46c: str      r0, [sp, #0xc]
0065f470: ldr      r3, [r4, #0xc]
0065f474: ldr      r1, [r4, #0x50]
0065f478: ldr      r0, [r4, #0x24]
0065f47c: subs     r3, r3, #1
0065f480: movne    r3, #1
0065f484: str      r3, [sp, #0x14]
0065f488: bl       #0x65f0b4
0065f48c: ldr      r3, [r0]
0065f490: ldr      r1, [sp, #0xc]
0065f494: mov      r0, r4
0065f498: ldr      r3, [r3, #0x24]
0065f49c: ldr      r3, [r3, #0x20]
0065f4a0: ldr      r5, [r3, #0x14]
0065f4a4: bl       #0x65f364
0065f4a8: str      r0, [sp, #0x10]
0065f4ac: ldr      r3, [r4, #0x24]
0065f4b0: subs     r5, r5, #0
0065f4b4: movne    r5, #1
0065f4b8: strb     r5, [sp, #0x31]
0065f4bc: ldr      fp, [r3, #0x3c]
0065f4c0: cmp      fp, #0
0065f4c4: beq      #0x65f5c0
0065f4c8: add      r1, sp, #0x24
0065f4cc: add      r2, sp, #0x34
0065f4d0: mov      r5, #0
0065f4d4: str      r1, [sp, #0x18]
0065f4d8: str      r2, [sp, #0x1c]
0065f4dc: b        #0x65f4ec
0065f4e0: add      r5, r5, #1
0065f4e4: cmp      r5, fp
0065f4e8: beq      #0x65f5c0
0065f4ec: mov      r1, r5
0065f4f0: ldr      r3, [r4]
0065f4f4: mov      r0, r4
0065f4f8: mov      lr, pc
0065f4fc: ldr      pc, [r3, #0x80]
0065f500: cmp      r0, #0
0065f504: beq      #0x65f4e0
0065f508: ldr      r3, [r4, #0x28]
0065f50c: lsl      r8, r5, #2
0065f510: ldr      r6, [r3, r5, lsl #2]
0065f514: cmp      r6, #0
0065f518: mov      r2, r6
0065f51c: beq      #0x65f4e0
0065f520: ldr      sb, [r4, #0x4c]
0065f524: ldr      r3, [r4, #0x24]
0065f528: mov      ip, #0xc
0065f52c: add      sb, r5, sb
0065f530: mul      sb, ip, sb
0065f534: ldr      sl, [r3, #0x30]
0065f538: add      r7, sl, sb
0065f53c: ldr      r1, [r7, #4]
0065f540: cmp      r1, #0
0065f544: beq      #0x65f568
0065f548: ldr      r0, [r3, #0x18]
0065f54c: ldr      r3, [r4, #0x34]
0065f550: ldr      ip, [r0, r8]
0065f554: ldr      r3, [r3, r8]
0065f558: mov      r0, ip
0065f55c: ldr      ip, [ip]
0065f560: mov      lr, pc
0065f564: ldr      pc, [ip, #0x70]
0065f568: ldr      r3, [sl, sb]
0065f56c: cmp      r3, #2
0065f570: bne      #0x65f4e0
0065f574: ldr      r2, [r7, #8]
0065f578: ldr      r3, [r4, #0x34]
0065f57c: ldr      r1, [sp, #0x10]
0065f580: str      r2, [sp, #0x34]
0065f584: ldr      r2, [sp, #0x18]
0065f588: str      r1, [sp, #0x38]
0065f58c: ldr      ip, [sp, #0x14]
0065f590: str      r2, [sp, #0x3c]
0065f594: ldr      r1, [r4, #0x40]
0065f598: ldr      r3, [r3, r8]
0065f59c: mov      r2, r6
0065f5a0: add      r8, r1, r8
0065f5a4: ldr      r0, [sp, #0x1c]
0065f5a8: ldr      r1, [sp, #0xc]
0065f5ac: add      r5, r5, #1
0065f5b0: stm      sp, {r8, ip}
0065f5b4: bl       #0x66a0c0
0065f5b8: cmp      r5, fp
0065f5bc: bne      #0x65f4ec
0065f5c0: add      sp, sp, #0x44
0065f5c4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0065f5c8: ldr      r1, [r4, #0x14]
0065f5cc: mov      r0, r5
0065f5d0: bl       #0x30eb2c
0065f5d4: str      r1, [sp, #0xc]
0065f5d8: b        #0x65f470

# _ZN12CharAnimator8ANIM_SetEi
003cacb0: ldrb     r2, [r0, #0x49]
003cacb4: cmp      r2, #0
003cacb8: strne    r1, [r0, #0x50]
003cacbc: bxne     lr
003cacc0: mov      ip, #0x3f800000
003cacc4: str      ip, [r0, #0x40]
003cacc8: b        #0x3cab38

# _ZN15AnimatorBlender10updateTimeEj
00366d90: push     {r4, r5, r6, r7, r8, lr}
00366d94: ldr      r3, [r0, #0x7c]
00366d98: mov      r5, r0
00366d9c: mov      r7, r1
00366da0: cmp      r3, #0
00366da4: ldr      r0, [r0, #0x84]
00366da8: blt      #0x366df4
00366dac: rsb      r0, r0, r1
00366db0: rsb      r0, r0, r3
00366db4: cmp      r0, #0
00366db8: str      r0, [r5, #0x7c]
00366dbc: ble      #0x366e94
00366dc0: bl       #0x30e964
00366dc4: ldr      r1, [r5, #0x80]
00366dc8: bl       #0x30ed6c
00366dcc: ldr      r2, [r5, #0x34]
00366dd0: ldr      ip, [r5, #0x74]
00366dd4: mov      r3, r0
00366dd8: mov      r1, r0
00366ddc: str      r3, [r2, ip, lsl #2]
00366de0: mov      r0, #0x3f800000
00366de4: bl       #0x30e3ac
00366de8: ldr      r2, [r5, #0x70]
00366dec: ldr      r3, [r5, #0x34]
00366df0: str      r0, [r3, r2, lsl #2]
00366df4: ldr      r6, [r5, #0x2c]
00366df8: ldr      r3, [r5, #0x28]
00366dfc: rsb      r6, r3, r6
00366e00: asrs     r6, r6, #2
00366e04: beq      #0x366e5c
00366e08: mov      r4, #0
00366e0c: b        #0x366e1c
00366e10: add      r4, r4, #1
00366e14: cmp      r4, r6
00366e18: beq      #0x366e5c
00366e1c: ldr      r3, [r5, #0x34]
00366e20: mov      r1, #0
00366e24: ldr      r0, [r3, r4, lsl #2]
00366e28: bl       #0x30df8c
00366e2c: cmp      r0, #0
00366e30: bne      #0x366e10
00366e34: ldr      r3, [r5, #0x28]
00366e38: mov      r1, r7
00366e3c: ldr      r3, [r3, r4, lsl #2]
00366e40: add      r4, r4, #1
00366e44: mov      r0, r3
00366e48: ldr      r3, [r3]
00366e4c: mov      lr, pc
00366e50: ldr      pc, [r3, #0x14]
00366e54: cmp      r4, r6
00366e58: bne      #0x366e1c
00366e5c: mov      r0, r5
00366e60: bl       #0x366594
00366e64: ldr      r2, [r5, #0x70]
00366e68: ldr      r3, [r5, #0x28]
00366e6c: ldr      r3, [r3, r2, lsl #2]
00366e70: mov      r0, r3
00366e74: ldr      r3, [r3]
00366e78: mov      lr, pc
00366e7c: ldr      pc, [r3, #0x44]
00366e80: mov      r1, r0
00366e84: add      r0, r5, #0x88
00366e88: bl       #0x36440c
00366e8c: str      r7, [r5, #0x84]
00366e90: pop      {r4, r5, r6, r7, r8, pc}
00366e94: ldr      r2, [r5, #0x74]
00366e98: ldr      r3, [r5, #0x34]
00366e9c: mov      r1, #0
00366ea0: str      r1, [r3, r2, lsl #2]
00366ea4: ldr      r2, [r5, #0x70]
00366ea8: ldr      r3, [r5, #0x34]
00366eac: mov      r1, #0x3f800000
00366eb0: str      r1, [r3, r2, lsl #2]
00366eb4: b        #0x366df4

# _ZN12CharAnimator15__CallbackEventERKN6glitch7collada15STriggeredEventEPv
003c9984: ldr      r2, [r0]
003c9988: mov      r3, r0
003c998c: ldr      r0, [r1, #4]
003c9990: str      r2, [r1, #0x58]
003c9994: ldr      r2, [r3, #4]
003c9998: mov      r1, #0x28
003c999c: b        #0x3a4d5c

# _ZN6glitch7collada14CEventsManager16dispatchEventsExIiLi1000EEEviii
0060e934: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0060e938: cmp      r1, r2
0060e93c: sub      sp, sp, #0x14
0060e940: str      r2, [sp, #4]
0060e944: mov      r4, r0
0060e948: mov      sb, r3
0060e94c: bgt      #0x60ea00
0060e950: ldr      r5, [r0, #0x14]
0060e954: mov      fp, r1
0060e958: lsl      r7, r1, #3
0060e95c: lsl      r8, r1, #2
0060e960: add      sl, sp, #8
0060e964: ldr      r3, [r5, #0x14]
0060e968: ldr      r3, [r3, r7]
0060e96c: cmp      r3, #0
0060e970: movgt    r6, #0
0060e974: ble      #0x60e9e8
0060e978: mov      r0, sb
0060e97c: bl       #0x30e964
0060e980: ldr      r2, [r5, #0xc]
0060e984: mov      r3, r0
0060e988: ldr      r0, [r2, r8]
0060e98c: str      r3, [sp]
0060e990: bl       #0x30e964
0060e994: ldr      r3, [sp]
0060e998: mov      r1, r0
0060e99c: mov      r0, r3
0060e9a0: bl       #0x30e3ac
0060e9a4: bl       #0x30e4cc
0060e9a8: str      r0, [sp, #8]
0060e9ac: ldr      r3, [r5, #0x14]
0060e9b0: ldr      r1, [r4, #0xc]
0060e9b4: mov      r0, sl
0060e9b8: add      r3, r3, r7
0060e9bc: ldr      r3, [r3, #4]
0060e9c0: ldr      r3, [r3, r6, lsl #2]
0060e9c4: add      r6, r6, #1
0060e9c8: str      r3, [sp, #0xc]
0060e9cc: mov      lr, pc
0060e9d0: ldr      pc, [r4, #8]
0060e9d4: ldr      r5, [r4, #0x14]
0060e9d8: ldr      r3, [r5, #0x14]
0060e9dc: ldr      r3, [r3, r7]
0060e9e0: cmp      r6, r3
0060e9e4: blt      #0x60e978
0060e9e8: ldr      r2, [sp, #4]
0060e9ec: add      fp, fp, #1
0060e9f0: add      r7, r7, #8
0060e9f4: cmp      r2, fp
0060e9f8: add      r8, r8, #4
0060e9fc: bge      #0x60e964
0060ea00: add      sp, sp, #0x14
0060ea04: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada14CEventsManager8onUpdateEiiii
0060ebe0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0060ebe4: cmp      r1, r2
0060ebe8: mov      r6, r1
0060ebec: mov      r5, r2
0060ebf0: mov      sl, r3
0060ebf4: mov      r4, r0
0060ebf8: ldr      sb, [sp, #0x20]
0060ebfc: beq      #0x60ec98
0060ec00: ldr      r3, [r0, #8]
0060ec04: cmp      r3, #0
0060ec08: beq      #0x60ec98
0060ec0c: sub      r1, r1, #1
0060ec10: bl       #0x60e02c
0060ec14: mov      r1, r5
0060ec18: add      r7, r0, #1
0060ec1c: mov      r0, r4
0060ec20: bl       #0x60e02c
0060ec24: ldr      r3, [r4, #0x10]
0060ec28: mov      r8, r0
0060ec2c: cmp      r3, r7
0060ec30: ldr      r3, [r4, #4]
0060ec34: addeq    r7, r7, #1
0060ec38: cmp      r6, r5
0060ec3c: add      r3, r3, #1
0060ec40: str      r3, [r4, #4]
0060ec44: ble      #0x60ec9c
0060ec48: mov      r1, sb
0060ec4c: mov      r0, r4
0060ec50: bl       #0x60e02c
0060ec54: rsb      r3, sl, sb
0060ec58: mov      r2, r0
0060ec5c: add      r3, r3, r5
0060ec60: mov      r1, r7
0060ec64: mov      r0, r4
0060ec68: bl       #0x60ebb4
0060ec6c: sub      r1, sl, #1
0060ec70: mov      r0, r4
0060ec74: bl       #0x60e02c
0060ec78: mov      r3, r5
0060ec7c: add      r1, r0, #1
0060ec80: mov      r2, r8
0060ec84: mov      r0, r4
0060ec88: bl       #0x60ebb4
0060ec8c: mov      r0, r4
0060ec90: bl       #0x31d584
0060ec94: str      r8, [r4, #0x10]
0060ec98: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0060ec9c: mov      r1, r7
0060eca0: mov      r3, r5
0060eca4: mov      r0, r4
0060eca8: mov      r2, r8
0060ecac: bl       #0x60ebb4
0060ecb0: b        #0x60ec8c

# _ZN14AnimApplicator13CheckCallbackEPN6glitch5scene19ITimelineControllerE
0036440c: push     {r4, lr}
00364410: ldrb     r3, [r0, #0x30]
00364414: mov      r4, r0
00364418: cmp      r3, #0
0036441c: beq      #0x364440
00364420: ldr      r3, [r0, #0x34]
00364424: cmp      r3, #0
00364428: beq      #0x364440
0036442c: mov      r0, r1
00364430: ldr      r1, [r4, #0x38]
00364434: blx      r3
00364438: mov      r3, #0
0036443c: strb     r3, [r4, #0x30]
00364440: pop      {r4, pc}

# _ZN11AnimatorSet11animateNodeEPN6glitch5scene10ISceneNodeEj
003673a4: push     {r4, r5, r6, lr}
003673a8: mov      r6, r2
003673ac: add      r5, r0, #0x58
003673b0: mov      r4, r0
003673b4: bl       #0x65f270
003673b8: mov      r1, r6
003673bc: mov      r0, r5
003673c0: bl       #0x3645e8
003673c4: mov      r0, r4
003673c8: ldr      r3, [r4]
003673cc: mov      lr, pc
003673d0: ldr      pc, [r3, #0x44]
003673d4: mov      r1, r0
003673d8: mov      r0, r5
003673dc: pop      {r4, r5, r6, lr}
003673e0: b        #0x36440c

# _ZN11AnimatorSet19setCurrentAnimationEi
00367420: push     {r4, r5, r6, lr}
00367424: mov      r4, r0
00367428: ldr      r0, [r0, #0x94]
0036742c: mov      r5, r1
00367430: bl       #0x364bf4
00367434: ldr      r3, [r0, #0x20]
00367438: cmn      r3, #1
0036743c: beq      #0x367480
00367440: ldr      r2, [r0, #0x24]
00367444: ldr      r3, [r0, #0x2c]
00367448: mov      r1, r5
0036744c: add      r2, r2, #1
00367450: add      r3, r3, #1
00367454: str      r2, [r0, #0x24]
00367458: str      r3, [r0, #0x2c]
0036745c: ldr      r3, [r4, #0x98]
00367460: str      r0, [r4, #0x98]
00367464: mov      r0, r4
00367468: cmp      r3, #0
0036746c: ldrne    r2, [r3, #0x24]
00367470: subne    r2, r2, #1
00367474: strne    r2, [r3, #0x24]
00367478: pop      {r4, r5, r6, lr}
0036747c: b        #0x65f8c8
00367480: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada25CSceneNodeAnimatorBlender20applyAnimationValuesEj
0065e350: push     {r4, r5, r6, r7, lr}
0065e354: ldr      r6, [r0, #0x2c]
0065e358: ldr      r3, [r0, #0x28]
0065e35c: sub      sp, sp, #0xc
0065e360: mov      r4, r0
0065e364: rsb      r6, r3, r6
0065e368: asrs     r6, r6, #2
0065e36c: mov      r7, r1
0065e370: beq      #0x65e3c8
0065e374: mov      r5, #0
0065e378: b        #0x65e388
0065e37c: add      r5, r5, #1
0065e380: cmp      r5, r6
0065e384: beq      #0x65e3c8
0065e388: ldr      r3, [r4, #0x34]
0065e38c: mov      r1, #0
0065e390: ldr      r0, [r3, r5, lsl #2]
0065e394: bl       #0x30df8c
0065e398: cmp      r0, #0
0065e39c: bne      #0x65e37c
0065e3a0: ldr      r3, [r4, #0x28]
0065e3a4: mov      r1, r7
0065e3a8: ldr      r3, [r3, r5, lsl #2]
0065e3ac: add      r5, r5, #1
0065e3b0: mov      r0, r3
0065e3b4: ldr      r3, [r3]
0065e3b8: mov      lr, pc
0065e3bc: ldr      pc, [r3, #0x4c]
0065e3c0: cmp      r5, r6
0065e3c4: bne      #0x65e388
0065e3c8: mov      r0, r4
0065e3cc: bl       #0x366594
0065e3d0: ldr      r2, [r4, #0x5c]
0065e3d4: ldr      r3, [r4, #0x58]
0065e3d8: rsb      r3, r3, r2
0065e3dc: lsrs     r3, r3, #2
0065e3e0: beq      #0x65e484
0065e3e4: mov      r5, #0
0065e3e8: mov      r1, r5
0065e3ec: ldr      r3, [r4]
0065e3f0: mov      r0, r4
0065e3f4: mov      lr, pc
0065e3f8: ldr      pc, [r3, #0x80]
0065e3fc: cmp      r0, #0
0065e400: beq      #0x65e46c
0065e404: ldr      r3, [r4, #0x58]
0065e408: mov      r1, r5
0065e40c: ldr      r2, [r3, r5, lsl #2]
0065e410: cmp      r2, #0
0065e414: beq      #0x65e470
0065e418: ldr      r3, [r4, #0x28]
0065e41c: ldr      r3, [r3]
0065e420: mov      r0, r3
0065e424: ldr      r3, [r3]
0065e428: mov      lr, pc
0065e42c: ldr      pc, [r3, #0x58]
0065e430: ldr      r3, [r4, #0x58]
0065e434: ldr      r2, [r4, #0x4c]
0065e438: ldr      lr, [r4, #0x64]
0065e43c: ldr      r3, [r3, r5, lsl #2]
0065e440: ldr      r1, [r2, r5, lsl #2]
0065e444: ldr      ip, [r0]
0065e448: ldr      r2, [r4, #0x34]
0065e44c: str      r3, [sp]
0065e450: ldr      r3, [r4, #0x38]
0065e454: ldr      lr, [lr, r5, lsl #2]
0065e458: rsb      r3, r2, r3
0065e45c: str      lr, [sp, #4]
0065e460: asr      r3, r3, #2
0065e464: mov      lr, pc
0065e468: ldr      pc, [ip, #0x18]
0065e46c: ldr      r3, [r4, #0x58]
0065e470: ldr      r2, [r4, #0x5c]
0065e474: add      r5, r5, #1
0065e478: rsb      r3, r3, r2
0065e47c: cmp      r5, r3, asr #2
0065e480: blo      #0x65e3e8
0065e484: add      sp, sp, #0xc
0065e488: pop      {r4, r5, r6, r7, pc}

# _ZN6glitch7collada21CSceneNodeAnimatorSet11animateNodeEPNS_5scene10ISceneNodeEj
0065f270: push     {r4, lr}
0065f274: mov      r1, r2
0065f278: ldr      r3, [r0]
0065f27c: mov      lr, pc
0065f280: ldr      pc, [r3, #0x50]
0065f284: pop      {r4, pc}

# _ZN13RootSceneNode9onAnimateEj
0035d168: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0035d16c: ldr      r7, [pc, #0x338]
0035d170: ldr      sl, [pc, #0x338]
0035d174: ldrb     r3, [r0, #0x209]
0035d178: add      r7, pc, r7
0035d17c: ldr      r2, [r7, sl]
0035d180: sub      sp, sp, #0x3c
0035d184: cmp      r3, #0
0035d188: ldr      r2, [r2]
0035d18c: mov      r5, r0
0035d190: mov      r6, r1
0035d194: str      r2, [sp, #0x34]
0035d198: ldreq    r2, [r0, #0x11c]
0035d19c: beq      #0x35d1b0
0035d1a0: ldr      r2, [r0, #0x11c]
0035d1a4: ands     r4, r2, #1
0035d1a8: movne    r3, #0
0035d1ac: beq      #0x35d310
0035d1b0: tst      r2, #0x400
0035d1b4: beq      #0x35d1e0
0035d1b8: tst      r2, #1
0035d1bc: bne      #0x35d1e0
0035d1c0: ldr      r3, [r7, sl]
0035d1c4: str      r6, [r5, #0x1fc]
0035d1c8: ldr      r2, [sp, #0x34]
0035d1cc: ldr      r3, [r3]
0035d1d0: cmp      r2, r3
0035d1d4: bne      #0x35d4a8
0035d1d8: add      sp, sp, #0x3c
0035d1dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0035d1e0: tst      r2, #0x200
0035d1e4: beq      #0x35d1c0
0035d1e8: ldr      r2, [r5, #0x204]
0035d1ec: cmp      r2, #0
0035d1f0: beq      #0x35d330
0035d1f4: cmp      r3, #0
0035d1f8: bne      #0x35d330
0035d1fc: ldr      r3, [pc, #0x2b0]
0035d200: ldr      r3, [r7, r3]
0035d204: ldrb     r3, [r3, #0x30]
0035d208: cmp      r3, #0
0035d20c: bne      #0x35d330
0035d210: ldr      r4, [r2, #0x130]
0035d214: ldr      r3, [r2, #0x140]
0035d218: ldr      lr, [r2, #0x134]
0035d21c: ldr      r0, [r2, #0x138]
0035d220: ldr      r1, [r2, #0x13c]
0035d224: ldr      ip, [r2, #0x12c]
0035d228: str      r4, [sp, #4]
0035d22c: str      lr, [sp, #8]
0035d230: str      ip, [sp]
0035d234: str      r0, [sp, #0xc]
0035d238: str      r1, [sp, #0x10]
0035d23c: str      r3, [sp, #0x14]
0035d240: ldrb     r3, [r2, #0x2f9]
0035d244: cmp      r3, #0
0035d248: moveq    r4, sp
0035d24c: beq      #0x35d270
0035d250: ldr      r3, [r5]
0035d254: mov      r0, r5
0035d258: mov      lr, pc
0035d25c: ldr      pc, [r3, #0x34]
0035d260: mov      r1, r0
0035d264: mov      r0, sp
0035d268: mov      r4, sp
0035d26c: bl       #0x35c150
0035d270: ldr      r3, [pc, #0x240]
0035d274: ldr      r0, [r7, r3]
0035d278: bl       #0x582138
0035d27c: mov      r1, sp
0035d280: bl       #0x35bec8
0035d284: cmp      r0, #0
0035d288: bne      #0x35d330
0035d28c: ldr      r3, [r5, #0x11c]
0035d290: tst      r3, #0x400
0035d294: beq      #0x35d330
0035d298: ldrb     r3, [r5, #0x20a]
0035d29c: cmp      r3, #0
0035d2a0: movne    fp, #1
0035d2a4: bne      #0x35d334
0035d2a8: ldr      r3, [r5]
0035d2ac: mov      r0, r5
0035d2b0: mov      r1, r6
0035d2b4: mov      lr, pc
0035d2b8: ldr      pc, [r3, #0x18]
0035d2bc: ldrb     r3, [r5, #0x1ec]
0035d2c0: cmp      r3, #0
0035d2c4: bne      #0x35d454
0035d2c8: ldrb     r3, [r5, #0x208]
0035d2cc: cmp      r3, #0
0035d2d0: beq      #0x35d2f0
0035d2d4: ldr      r3, [r5]
0035d2d8: mov      r0, r5
0035d2dc: mov      r1, #1
0035d2e0: mov      lr, pc
0035d2e4: ldr      pc, [r3, #0xb8]
0035d2e8: mov      r3, #0
0035d2ec: strb     r3, [r5, #0x208]
0035d2f0: ldr      r3, [pc, #0x1c4]
0035d2f4: mov      fp, #0
0035d2f8: ldr      r3, [r7, r3]
0035d2fc: ldr      r2, [r3]
0035d300: add      r2, r2, #1
0035d304: str      r2, [r3]
0035d308: strb     fp, [r5, #0x20a]
0035d30c: b        #0x35d1c0
0035d310: mov      r1, #1
0035d314: bl       #0x596ec4
0035d318: ldr      r0, [r5, #0x110]
0035d31c: bl       #0x5890a8
0035d320: ldr      r2, [r5, #0x11c]
0035d324: strb     r4, [r5, #0x209]
0035d328: mov      r3, #1
0035d32c: b        #0x35d1b0
0035d330: mov      fp, #0
0035d334: ldr      r3, [pc, #0x184]
0035d338: ldr      r2, [pc, #0x184]
0035d33c: add      r4, sp, #0x1c
0035d340: ldr      r3, [r7, r3]
0035d344: ldr      sb, [r7, r2]
0035d348: mov      r8, r5
0035d34c: ldr      r2, [r3]
0035d350: mov      r0, sb
0035d354: add      r2, r2, #1
0035d358: str      r2, [r3]
0035d35c: bl       #0x337888
0035d360: ldr      r1, [pc, #0x160]
0035d364: add      r2, sp, #0x18
0035d368: mov      r0, r4
0035d36c: add      r1, pc, r1
0035d370: bl       #0x3140ec
0035d374: mov      r1, r4
0035d378: mov      r0, sb
0035d37c: bl       #0x337a88
0035d380: mov      r0, r4
0035d384: bl       #0x3139ac
0035d388: ldr      r4, [r8, #0xfc]!
0035d38c: b        #0x35d3b0
0035d390: ldr      r3, [r4, #8]
0035d394: mov      r1, r5
0035d398: mov      r2, r6
0035d39c: mov      r0, r3
0035d3a0: ldr      r3, [r3]
0035d3a4: mov      lr, pc
0035d3a8: ldr      pc, [r3, #0x10]
0035d3ac: ldr      r4, [r4]
0035d3b0: cmp      r8, r4
0035d3b4: bne      #0x35d390
0035d3b8: ldrb     r3, [r5, #0x1ec]
0035d3bc: cmp      r3, #0
0035d3c0: bne      #0x35d42c
0035d3c4: ldr      r3, [r5, #0x204]
0035d3c8: cmp      r3, #0
0035d3cc: beq      #0x35d43c
0035d3d0: ldr      r3, [r3, #0x110]
0035d3d4: cmn      r3, #1
0035d3d8: beq      #0x35d43c
0035d3dc: mov      r8, r5
0035d3e0: ldr      r4, [r8, #0xf4]!
0035d3e4: b        #0x35d40c
0035d3e8: cmp      r4, #0
0035d3ec: moveq    r3, r4
0035d3f0: subne    r3, r4, #4
0035d3f4: mov      r0, r3
0035d3f8: mov      r1, r6
0035d3fc: ldr      r3, [r3]
0035d400: mov      lr, pc
0035d404: ldr      pc, [r3, #0x14]
0035d408: ldr      r4, [r4]
0035d40c: cmp      r4, r8
0035d410: bne      #0x35d3e8
0035d414: ldr      r3, [r5, #0x11c]
0035d418: eor      fp, fp, #1
0035d41c: strb     fp, [r5, #0x20a]
0035d420: bic      r3, r3, #0x20
0035d424: str      r3, [r5, #0x11c]
0035d428: b        #0x35d1c0
0035d42c: mov      r0, r5
0035d430: mov      r1, r6
0035d434: bl       #0x35cf48
0035d438: b        #0x35d3c4
0035d43c: ldr      r3, [r5]
0035d440: mov      r0, r5
0035d444: mov      r1, #0
0035d448: mov      lr, pc
0035d44c: ldr      pc, [r3, #0xb8]
0035d450: b        #0x35d3dc
0035d454: mov      r8, r5
0035d458: ldr      r4, [r8, #0xfc]!
0035d45c: b        #0x35d47c
0035d460: ldr      r0, [r4, #8]
0035d464: bl       #0x369160
0035d468: mov      r1, r6
0035d46c: ldr      r3, [r0]
0035d470: mov      lr, pc
0035d474: ldr      pc, [r3, #0x10]
0035d478: ldr      r4, [r4]
0035d47c: cmp      r8, r4
0035d480: bne      #0x35d460
0035d484: mov      r0, r5
0035d488: mov      r1, r6
0035d48c: bl       #0x35cf48
0035d490: ldr      r3, [r5, #0x11c]
0035d494: cmp      r0, #0
0035d498: bic      r3, r3, #0x20
0035d49c: str      r3, [r5, #0x11c]
0035d4a0: bne      #0x35d2d4
0035d4a4: b        #0x35d2c8
0035d4a8: bl       #0x30e310
0035d4ac: rsbeq    r7, r3, r8, lsl sb
0035d4b0: andeq    r4, r0, ip, lsr #1
0035d4b4: andeq    r1, r0, r0, lsr #20
0035d4b8: muleq    r0, ip, r5
0035d4bc: muleq    r0, r4, r7
0035d4c0: andeq    r0, r0, r0, asr #30
0035d4c4: andeq    r0, r0, r4, lsl #17
0035d4c8: subseq   r3, r6, ip, lsl #19

# _ZN6glitch7collada18ISceneNodeAnimator14setEventsTrackEPKNS0_12SEventsTrackE
0060fab8: push     {r4, r5, r6, lr}
0060fabc: mov      r5, r0
0060fac0: ldr      r0, [r0, #0x18]
0060fac4: ldr      r4, [pc, #0x7c]
0060fac8: mov      r6, r1
0060facc: cmp      r0, #0
0060fad0: add      r4, pc, r4
0060fad4: beq      #0x60fadc
0060fad8: bl       #0x31d584
0060fadc: cmp      r6, #0
0060fae0: beq      #0x60fb40
0060fae4: mov      r0, #0x18
0060fae8: mov      r1, #0
0060faec: bl       #0x5341ac
0060faf0: ldr      r3, [pc, #0x54]
0060faf4: ldr      r2, [pc, #0x54]
0060faf8: str      r6, [r0, #0x14]
0060fafc: ldr      r3, [r4, r3]
0060fb00: ldr      r2, [r4, r2]
0060fb04: add      r3, r3, #8
0060fb08: str      r2, [r0, #8]
0060fb0c: mov      r2, #0
0060fb10: str      r2, [r0, #0xc]
0060fb14: str      r3, [r0]
0060fb18: mov      r2, #1
0060fb1c: mvn      r3, #0
0060fb20: str      r2, [r0, #4]
0060fb24: str      r3, [r0, #0x10]
0060fb28: ldr      r2, [r5, #0x1c]
0060fb2c: ldr      r3, [r5, #0x20]
0060fb30: str      r0, [r5, #0x18]
0060fb34: str      r2, [r0, #8]
0060fb38: str      r3, [r0, #0xc]
0060fb3c: pop      {r4, r5, r6, pc}
0060fb40: str      r6, [r5, #0x18]
0060fb44: pop      {r4, r5, r6, pc}
0060fb48: eorseq   r4, r8, r0, asr #31
0060fb4c: ldrdeq   r4, r5, [r0], -r0
0060fb50: andeq    r4, r0, ip, lsr #10
