
# _ZN6glitch7collada19CTimelineController6jumpToEi
00666f10: push     {r4, lr}
00666f14: mov      r4, r0
00666f18: str      r1, [r0, #4]
00666f1c: mov      r0, r1
00666f20: bl       #0x30e964
00666f24: mov      r1, #0x44000000
00666f28: add      r1, r1, #0x7a0000
00666f2c: bl       #0x30ec94
00666f30: mov      r3, #0
00666f34: strb     r3, [r4, #0x3d]
00666f38: str      r0, [r4, #0x2c]
00666f3c: strb     r3, [r4, #0x3c]
00666f40: pop      {r4, pc}

# _ZN6glitch7collada19CTimelineController7setClipEi
00666f7c: push     {r4, r5, r6, r7, r8, lr}
00666f80: mov      r3, #0
00666f84: str      r1, [r0, #0x38]
00666f88: strb     r3, [r0, #0x3d]
00666f8c: strb     r3, [r0, #0x3c]
00666f90: ldr      r3, [r0]
00666f94: mov      r4, r0
00666f98: mov      lr, pc
00666f9c: ldr      pc, [r3, #0x2c]
00666fa0: ldr      r3, [r4]
00666fa4: str      r0, [r4, #0x10]
00666fa8: mov      r0, r4
00666fac: mov      lr, pc
00666fb0: ldr      pc, [r3, #0x30]
00666fb4: mov      r7, r0
00666fb8: str      r0, [r4, #0x14]
00666fbc: ldr      r0, [r4, #0x10]
00666fc0: bl       #0x30e964
00666fc4: mov      r1, #0x44000000
00666fc8: add      r1, r1, #0x7a0000
00666fcc: bl       #0x30ec94
00666fd0: ldr      r6, [r4, #0x10]
00666fd4: mov      r5, r0
00666fd8: str      r0, [r4, #0x20]
00666fdc: rsb      r0, r6, r7
00666fe0: bl       #0x30e964
00666fe4: mov      r1, #0x44000000
00666fe8: add      r1, r1, #0x7a0000
00666fec: bl       #0x30ec94
00666ff0: str      r6, [r4, #4]
00666ff4: str      r0, [r4, #0x24]
00666ff8: str      r5, [r4, #0x2c]
00666ffc: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada19CTimelineControllerC1Ev
00666e40: ldr      r1, [pc, #0xa4]
00666e44: ldr      r3, [pc, #0xa4]
00666e48: ldr      r2, [pc, #0xa4]
00666e4c: add      r1, pc, r1
00666e50: push     {r4, r5, r6, r7, r8}
00666e54: ldr      r4, [r1, r3]
00666e58: ldr      r2, [r1, r2]
00666e5c: mov      r5, #1
00666e60: ldr      ip, [r4, #8]
00666e64: add      r2, r2, #8
00666e68: str      r2, [r0, #0x40]
00666e6c: str      ip, [r0]
00666e70: str      r5, [r0, #0x44]
00666e74: ldr      r7, [ip, #-0xc]
00666e78: ldr      r6, [r4, #4]
00666e7c: ldr      r8, [r4, #0xc]
00666e80: ldr      ip, [pc, #0x70]
00666e84: mov      r2, #0
00666e88: str      r8, [r0, r7]
00666e8c: ldr      ip, [r1, ip]
00666e90: str      r6, [r0]
00666e94: str      r2, [r0, #4]
00666e98: ldr      r7, [r6, #-0xc]
00666e9c: ldr      r8, [r4, #0x10]
00666ea0: add      r6, ip, #0x70
00666ea4: add      ip, ip, #0xc
00666ea8: str      r8, [r0, r7]
00666eac: mov      r4, #0
00666eb0: str      ip, [r0]
00666eb4: mov      ip, #0x3f800000
00666eb8: strb     r2, [r0, #0x3d]
00666ebc: str      r6, [r0, #0x40]
00666ec0: strb     r5, [r0, #0x18]
00666ec4: str      r4, [r0, #0x2c]
00666ec8: str      ip, [r0, #0x30]
00666ecc: str      r2, [r0, #8]
00666ed0: str      r4, [r0, #0x20]
00666ed4: str      r4, [r0, #0x24]
00666ed8: str      r2, [r0, #0x34]
00666edc: str      r2, [r0, #0x38]
00666ee0: strb     r2, [r0, #0x3c]
00666ee4: pop      {r4, r5, r6, r7, r8}
00666ee8: bx       lr
00666eec: eorseq   sp, r2, r4, asr #24
00666ef0: andeq    r2, r0, r0, ror #28
00666ef4: andeq    r2, r0, r4, asr #22
00666ef8: andeq    r2, r0, r4, lsl pc

# _ZNK6glitch7collada19CTimelineController17getCurrentClipEndEv
00666d54: push     {r4, lr}
00666d58: ldr      r1, [r0, #0x38]
00666d5c: ldr      r3, [r0]
00666d60: mov      lr, pc
00666d64: ldr      pc, [r3, #0x24]
00666d68: pop      {r4, pc}

# _ZN6glitch7collada19CTimelineController8setRangeEiib
00666c38: push     {r4, r5, lr}
00666c3c: ldr      ip, [r0, #0x34]
00666c40: sub      sp, sp, #0xc
00666c44: mov      r4, r0
00666c48: cmp      ip, #0
00666c4c: mov      r5, r1
00666c50: beq      #0x666c78
00666c54: cmp      r3, #0
00666c58: beq      #0x666c70
00666c5c: mov      r0, r4
00666c60: ldr      r3, [r4]
00666c64: ldr      r1, [r4, #0x10]
00666c68: mov      lr, pc
00666c6c: ldr      pc, [r3, #0xc]
00666c70: add      sp, sp, #0xc
00666c74: pop      {r4, r5, pc}
00666c78: str      r1, [r4, #0x10]
00666c7c: str      r2, [r0, #0x14]
00666c80: mov      r0, r1
00666c84: str      r3, [sp]
00666c88: str      r2, [sp, #4]
00666c8c: bl       #0x30e964
00666c90: mov      r1, #0x44000000
00666c94: add      r1, r1, #0x7a0000
00666c98: bl       #0x30ec94
00666c9c: str      r0, [r4, #0x20]
00666ca0: ldr      r2, [sp, #4]
00666ca4: rsb      r0, r5, r2
00666ca8: bl       #0x30e964
00666cac: mov      r1, #0x44000000
00666cb0: add      r1, r1, #0x7a0000
00666cb4: bl       #0x30ec94
00666cb8: str      r0, [r4, #0x24]
00666cbc: ldr      r3, [sp]
00666cc0: b        #0x666c54

# _ZNK6glitch7collada19CTimelineController19getCurrentClipStartEv
00666d3c: push     {r4, lr}
00666d40: ldr      r1, [r0, #0x38]
00666d44: ldr      r3, [r0]
00666d48: mov      lr, pc
00666d4c: ldr      pc, [r3, #0x20]
00666d50: pop      {r4, pc}

# _ZN6glitch7collada19CTimelineControllerC2Ev
00666db8: push     {r4, r5, r6}
00666dbc: add      r4, r1, #4
00666dc0: ldr      ip, [r4, #4]
00666dc4: mov      r2, #0
00666dc8: str      ip, [r0]
00666dcc: ldr      r5, [ip, #-0xc]
00666dd0: ldr      r6, [r4, #8]
00666dd4: mov      ip, #0
00666dd8: str      r6, [r0, r5]
00666ddc: str      r2, [r0, #4]
00666de0: ldr      r5, [r1, #4]
00666de4: str      r5, [r0]
00666de8: ldr      r4, [r4, #0xc]
00666dec: ldr      r5, [r5, #-0xc]
00666df0: str      r4, [r0, r5]
00666df4: str      r2, [r0, #8]
00666df8: ldr      r4, [r1]
00666dfc: str      r4, [r0]
00666e00: ldr      r1, [r1, #0x14]
00666e04: ldr      r4, [r4, #-0xc]
00666e08: str      r1, [r0, r4]
00666e0c: mov      r1, #1
00666e10: strb     r1, [r0, #0x18]
00666e14: mov      r1, #0x3f800000
00666e18: strb     r2, [r0, #0x3d]
00666e1c: str      ip, [r0, #0x2c]
00666e20: str      r1, [r0, #0x30]
00666e24: str      ip, [r0, #0x20]
00666e28: str      ip, [r0, #0x24]
00666e2c: str      r2, [r0, #0x34]
00666e30: str      r2, [r0, #0x38]
00666e34: strb     r2, [r0, #0x3c]
00666e38: pop      {r4, r5, r6}
00666e3c: bx       lr

# _ZN6glitch7collada19CTimelineController6updateEi
00667104: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00667108: mov      r4, r0
0066710c: mov      r0, r1
00667110: bl       #0x30e964
00667114: mov      r1, #0x44000000
00667118: add      r1, r1, #0x7a0000
0066711c: bl       #0x30ec94
00667120: ldrb     r3, [r4, #0x3d]
00667124: mov      r5, r0
00667128: ldr      r1, [r4, #0x28]
0066712c: cmp      r3, #0
00667130: ldr      r6, [r4, #0x30]
00667134: bne      #0x6671f8
00667138: mov      r3, #1
0066713c: ldr      r0, [r4, #0x2c]
00667140: strb     r3, [r4, #0x3d]
00667144: mov      r1, #0
00667148: bl       #0x30eba4
0066714c: mov      sl, #0
00667150: mov      r6, r0
00667154: str      r5, [r4, #0x28]
00667158: str      r0, [r4, #0x2c]
0066715c: ldr      r0, [r4, #0x14]
00667160: bl       #0x30e964
00667164: mov      r1, #0x44000000
00667168: add      r1, r1, #0x7a0000
0066716c: bl       #0x30ec94
00667170: mov      r5, r0
00667174: str      sl, [r4, #0x1c]
00667178: mov      r0, r6
0066717c: mov      r1, r5
00667180: bl       #0x30e2f8
00667184: cmp      r0, #0
00667188: ldr      r6, [r4, #0x20]
0066718c: mov      r3, #0
00667190: bne      #0x667280
00667194: uxtb     r3, r3
00667198: cmp      r3, #0
0066719c: beq      #0x667290
006671a0: ldrb     r3, [r4, #0x18]
006671a4: cmp      r3, #0
006671a8: beq      #0x6672b0
006671ac: ldr      r7, [r4, #0x24]
006671b0: mov      r1, #0
006671b4: mov      r0, r7
006671b8: bl       #0x30df8c
006671bc: cmp      r0, #0
006671c0: movne    r1, #0
006671c4: beq      #0x6672e4
006671c8: mov      r0, r6
006671cc: bl       #0x30eba4
006671d0: ldr      r3, [r4, #8]
006671d4: mov      r5, r0
006671d8: str      r0, [r4, #0x2c]
006671dc: cmp      r3, #0
006671e0: beq      #0x667294
006671e4: mov      r0, r4
006671e8: ldr      r1, [r4, #0xc]
006671ec: blx      r3
006671f0: ldr      r5, [r4, #0x2c]
006671f4: b        #0x667294
006671f8: bl       #0x30e3ac
006671fc: mov      r1, r6
00667200: bl       #0x30ed6c
00667204: ldr      r1, [r4, #0x2c]
00667208: mov      r8, r0
0066720c: bl       #0x30eba4
00667210: str      r5, [r4, #0x28]
00667214: mov      r7, r0
00667218: str      r0, [r4, #0x2c]
0066721c: mov      r1, #0
00667220: mov      r0, r8
00667224: bl       #0x30e70c
00667228: cmp      r0, #0
0066722c: mov      sl, r8
00667230: mov      r6, r7
00667234: beq      #0x66715c
00667238: ldr      r0, [r4, #0x10]
0066723c: bl       #0x30e964
00667240: mov      r1, #0x44000000
00667244: add      r1, r1, #0x7a0000
00667248: bl       #0x30ec94
0066724c: ldr      r1, [r4, #0x24]
00667250: mov      r5, r0
00667254: ldr      r0, [r4, #0x20]
00667258: bl       #0x30eba4
0066725c: add      r8, r8, #0x80000000
00667260: mov      r6, r0
00667264: str      r8, [r4, #0x1c]
00667268: mov      r0, r7
0066726c: mov      r1, r5
00667270: bl       #0x30e70c
00667274: cmp      r0, #0
00667278: mov      r3, #0
0066727c: beq      #0x667194
00667280: mov      r3, #1
00667284: uxtb     r3, r3
00667288: cmp      r3, #0
0066728c: bne      #0x6671a0
00667290: ldr      r5, [r4, #0x2c]
00667294: mov      r1, #0x44000000
00667298: add      r1, r1, #0x7a0000
0066729c: mov      r0, r5
006672a0: bl       #0x30ed6c
006672a4: bl       #0x30e4cc
006672a8: str      r0, [r4, #4]
006672ac: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
006672b0: ldrb     r3, [r4, #0x3c]
006672b4: str      r5, [r4, #0x2c]
006672b8: cmp      r3, #0
006672bc: bne      #0x667294
006672c0: ldr      r3, [r4, #8]
006672c4: mov      r2, #1
006672c8: strb     r2, [r4, #0x3c]
006672cc: cmp      r3, #0
006672d0: beq      #0x667294
006672d4: mov      r0, r4
006672d8: ldr      r1, [r4, #0xc]
006672dc: blx      r3
006672e0: b        #0x667290
006672e4: mov      r1, r5
006672e8: ldr      r0, [r4, #0x2c]
006672ec: bl       #0x30e3ac
006672f0: mov      r1, r7
006672f4: bl       #0x30e7f0
006672f8: mov      r1, r0
006672fc: b        #0x6671c8

# _ZN6glitch7collada19CTimelineController4initEii
00666f04: str      r2, [r0, #0x14]
00666f08: str      r1, [r0, #0x10]
00666f0c: bx       lr

# _ZNK6glitch7collada19CTimelineController7getLoopEv
00666c30: ldrb     r0, [r0, #0x18]
00666c34: bx       lr

# _ZN6glitch7collada19CTimelineController8setScaleEf
00666c20: str      r1, [r0, #0x30]
00666c24: bx       lr

# _ZN6glitch7collada19CTimelineController7setLoopEb
00666c28: strb     r1, [r0, #0x18]
00666c2c: bx       lr

# _ZNK6glitch7collada19CTimelineController19getCurrentClipIndexEv
00666d9c: ldr      r0, [r0, #0x38]
00666da0: bx       lr

# _ZN14AnimController8PlayClipEjbij
00474b50: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00474b54: mov      r4, r1
00474b58: ldr      r1, [sp, #0x20]
00474b5c: mov      r7, r2
00474b60: mov      r6, r0
00474b64: bl       #0x4748b8
00474b68: subs     sb, r0, #0
00474b6c: beq      #0x474c30
00474b70: ldr      r3, [sb]
00474b74: mov      lr, pc
00474b78: ldr      pc, [r3, #0x44]
00474b7c: mov      r5, r0
00474b80: mov      r0, sb
00474b84: bl       #0x369160
00474b88: cmp      r5, #0
00474b8c: mov      sl, r0
00474b90: beq      #0x474bac
00474b94: ldr      r3, [r5]
00474b98: mov      r0, r5
00474b9c: mov      lr, pc
00474ba0: ldr      pc, [r3, #0x1c]
00474ba4: cmp      r0, r4
00474ba8: bls      #0x474c28
00474bac: ldr      r3, [r5]
00474bb0: mov      r0, r5
00474bb4: mov      lr, pc
00474bb8: ldr      pc, [r3, #0x38]
00474bbc: ldr      r3, [sb]
00474bc0: mov      r8, r0
00474bc4: mov      r1, r4
00474bc8: mov      r0, sb
00474bcc: mov      lr, pc
00474bd0: ldr      pc, [r3, #0x30]
00474bd4: cmp      r8, r4
00474bd8: beq      #0x474c3c
00474bdc: mov      r1, r7
00474be0: mov      r0, r5
00474be4: ldr      r3, [r5]
00474be8: mov      lr, pc
00474bec: ldr      pc, [r3, #0x40]
00474bf0: ldr      r3, [r5]
00474bf4: mov      r0, r5
00474bf8: mov      r1, #0x3f800000
00474bfc: mov      lr, pc
00474c00: ldr      pc, [r3, #0x48]
00474c04: ldr      r0, [r6, #4]
00474c08: mov      r1, #0
00474c0c: bl       #0x35d624
00474c10: ldr      r3, [r6, #4]
00474c14: mov      r0, #1
00474c18: ldr      r2, [r3, #0x11c]
00474c1c: orr      r2, r2, #0x200
00474c20: str      r2, [r3, #0x11c]
00474c24: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00474c28: mov      r0, #0
00474c2c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00474c30: bl       #0x369160
00474c34: mov      r0, sb
00474c38: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00474c3c: ldr      r3, [r5]
00474c40: mov      r0, r5
00474c44: mov      lr, pc
00474c48: ldr      pc, [r3, #0x44]
00474c4c: cmp      r0, #0
00474c50: bne      #0x474bdc
00474c54: cmp      sl, #0
00474c58: ldr      r3, [r5]
00474c5c: ldr      r1, [r5, #0x10]
00474c60: ldrne    sl, [sl, #0x10]
00474c64: ldr      r3, [r3, #0xc]
00474c68: mov      r0, r5
00474c6c: add      r1, sl, r1
00474c70: blx      r3
00474c74: b        #0x474bdc

# _ZN12CharAnimator18CalculateExtraTimeEPN6glitch5scene19ITimelineControllerE
003c90f8: push     {r4, r5, r6, lr}
003c90fc: subs     r4, r1, #0
003c9100: mov      r5, r0
003c9104: beq      #0x3c9164
003c9108: mov      r1, #0x44000000
003c910c: add      r1, r1, #0x7a0000
003c9110: ldr      r0, [r4, #0x1c]
003c9114: bl       #0x30ed6c
003c9118: bl       #0x30e4cc
003c911c: mov      r1, #0x44000000
003c9120: mov      r6, r0
003c9124: add      r1, r1, #0x7a0000
003c9128: ldr      r0, [r4, #0x2c]
003c912c: bl       #0x30ed6c
003c9130: bl       #0x30e4cc
003c9134: ldr      r3, [r4, #4]
003c9138: mov      r2, #0
003c913c: str      r2, [r5, #0x44]
003c9140: rsb      r3, r3, r0
003c9144: cmp      r3, r6
003c9148: movge    r2, #0
003c914c: movlt    r2, #1
003c9150: cmp      r3, #0
003c9154: movlt    r2, #0
003c9158: cmp      r2, #0
003c915c: rsbne    r3, r3, r6
003c9160: strne    r3, [r5, #0x44]
003c9164: pop      {r4, r5, r6, pc}

# _ZN8Animator17_HandleAnimEndingEPN6glitch5scene19ITimelineControllerE
00366224: push     {r4, r5, r6, lr}
00366228: subs     r4, r1, #0
0036622c: mov      r5, r0
00366230: beq      #0x366288
00366234: mov      r1, #0x44000000
00366238: add      r1, r1, #0x7a0000
0036623c: ldr      r0, [r4, #0x1c]
00366240: bl       #0x30ed6c
00366244: bl       #0x30e4cc
00366248: mov      r1, #0x44000000
0036624c: mov      r6, r0
00366250: add      r1, r1, #0x7a0000
00366254: ldr      r0, [r4, #0x2c]
00366258: bl       #0x30ed6c
0036625c: bl       #0x30e4cc
00366260: ldr      r3, [r4, #4]
00366264: rsb      r0, r3, r0
00366268: cmp      r0, r6
0036626c: movge    r3, #0
00366270: movlt    r3, #1
00366274: cmp      r0, #0
00366278: movlt    r3, #0
0036627c: cmp      r3, #0
00366280: rsbne    r3, r0, r6
00366284: str      r3, [r5, #0x68]
00366288: mov      r3, #1
0036628c: strb     r3, [r5, #0x88]
00366290: pop      {r4, r5, r6, pc}

# _ZNK6glitch7collada21CSceneNodeAnimatorSet19getCurrentAnimationEv
0065f114: ldr      r0, [r0, #0x50]
0065f118: bx       lr

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

# _ZN11AnimatorSet19SetCurrentAnimationEi
003674ac: push     {r4, r5, r6, lr}
003674b0: mov      r5, r0
003674b4: ldr      r0, [r0, #0x94]
003674b8: bl       #0x3660f4
003674bc: mov      r4, r0
003674c0: ldr      r0, [r0, #0x20]
003674c4: cmn      r0, #1
003674c8: beq      #0x36750c
003674cc: ldr      r2, [r4, #0x24]
003674d0: ldr      r3, [r4, #0x2c]
003674d4: mov      r0, r5
003674d8: add      r2, r2, #1
003674dc: add      r3, r3, #1
003674e0: str      r2, [r4, #0x24]
003674e4: str      r3, [r4, #0x2c]
003674e8: ldr      r3, [r5, #0x98]
003674ec: str      r4, [r5, #0x98]
003674f0: cmp      r3, #0
003674f4: ldrne    r2, [r3, #0x24]
003674f8: subne    r2, r2, #1
003674fc: strne    r2, [r3, #0x24]
00367500: ldr      r1, [r4, #0x20]
00367504: bl       #0x65f8c8
00367508: ldr      r0, [r4, #0x20]
0036750c: pop      {r4, r5, r6, pc}

# _ZN24BlendedAnimSetController8PlayClipEjbij
0047680c: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00476810: mov      r4, r1
00476814: ldr      r1, [sp, #0x28]
00476818: mov      sl, r2
0047681c: mov      r7, r0
00476820: bl       #0x4748b8
00476824: subs     r6, r0, #0
00476828: beq      #0x476900
0047682c: ldr      r1, [r7, #0x14]
00476830: bl       #0x36679c
00476834: cmn      r4, #1
00476838: beq      #0x476900
0047683c: ldr      r2, [r6, #0x70]
00476840: ldr      r3, [r6, #0x28]
00476844: ldr      r5, [r3, r2, lsl #2]
00476848: cmp      r5, #0
0047684c: beq      #0x476908
00476850: ldr      r3, [r5]
00476854: mov      r0, r5
00476858: mov      lr, pc
0047685c: ldr      pc, [r3, #0x44]
00476860: mov      r8, r0
00476864: mov      r0, r5
00476868: bl       #0x65f114
0047686c: mov      sb, r0
00476870: mov      r0, r6
00476874: bl       #0x369160
00476878: mov      r1, r4
0047687c: mov      fp, r0
00476880: mov      r0, r5
00476884: bl       #0x3674ac
00476888: cmn      r0, #1
0047688c: mov      r4, r0
00476890: beq      #0x476900
00476894: ldr      r3, [r8, #0x34]
00476898: cmp      r3, #0
0047689c: beq      #0x4768b4
004768a0: mov      r0, r5
004768a4: ldr      r3, [r5]
004768a8: ldr      r1, [r7, #0xc]
004768ac: mov      lr, pc
004768b0: ldr      pc, [r3, #0x30]
004768b4: cmp      sb, r4
004768b8: beq      #0x476920
004768bc: mov      r1, sl
004768c0: mov      r0, r8
004768c4: ldr      r3, [r8]
004768c8: mov      lr, pc
004768cc: ldr      pc, [r3, #0x40]
004768d0: ldr      r3, [r8]
004768d4: mov      r0, r8
004768d8: mov      r1, #0x3f800000
004768dc: mov      lr, pc
004768e0: ldr      pc, [r3, #0x48]
004768e4: ldrb     r1, [r7, #0x10]
004768e8: ldr      r0, [r7, #4]
004768ec: bl       #0x35d624
004768f0: mov      r0, r6
004768f4: bl       #0x366740
004768f8: mov      r0, #1
004768fc: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476900: mov      r0, #0
00476904: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476908: mov      r0, r5
0047690c: bl       #0x65f114
00476910: mov      r0, r6
00476914: bl       #0x369160
00476918: mov      r0, r5
0047691c: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476920: ldr      r3, [r8]
00476924: mov      r0, r8
00476928: mov      lr, pc
0047692c: ldr      pc, [r3, #0x44]
00476930: cmp      r0, #0
00476934: bne      #0x4768bc
00476938: cmp      fp, #0
0047693c: ldr      r3, [r8]
00476940: ldr      r1, [r8, #0x10]
00476944: ldrne    fp, [fp, #0x10]
00476948: ldr      r3, [r3, #0xc]
0047694c: mov      r0, r8
00476950: add      r1, fp, r1
00476954: blx      r3
00476958: b        #0x4768bc

# _Z13GetApplicatorPN6glitch5scene18ISceneNodeAnimatorE
00369160: push     {r4, lr}
00369164: subs     r4, r0, #0
00369168: bne      #0x369174
0036916c: mov      r0, #0
00369170: pop      {r4, pc}
00369174: ldr      r3, [r4]
00369178: mov      lr, pc
0036917c: ldr      pc, [r3, #0x24]
00369180: sub      r0, r0, #0xb
00369184: cmp      r0, #4
00369188: addls    pc, pc, r0, lsl #2
0036918c: b        #0x36916c
00369190: b        #0x3691a4
00369194: b        #0x3691ac
00369198: b        #0x3691a4
0036919c: b        #0x3691b4
003691a0: b        #0x3691bc
003691a4: add      r0, r4, #0x58
003691a8: pop      {r4, pc}
003691ac: add      r0, r4, #0x88
003691b0: pop      {r4, pc}
003691b4: add      r0, r4, #0xd8
003691b8: pop      {r4, pc}
003691bc: add      r0, r4, #0x84
003691c0: pop      {r4, pc}

# _ZN12CharAnimator10__CallbackEPN6glitch5scene19ITimelineControllerEPv
003c90ec: mov      r3, #1
003c90f0: strb     r3, [r1, #0x49]
003c90f4: bx       lr

# _ZN6glitch7collada21CSceneNodeAnimatorSet19setCurrentAnimationEi
0065f8c8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0065f8cc: mov      r4, r0
0065f8d0: mov      r5, r1
0065f8d4: bl       #0x65f0fc
0065f8d8: ldr      r3, [r4, #0x24]
0065f8dc: str      r0, [r4, #0x14]
0065f8e0: mov      r1, r5
0065f8e4: ldr      r2, [r3, #0x3c]
0065f8e8: mov      r0, r3
0065f8ec: str      r5, [r4, #0x50]
0065f8f0: mul      r3, r2, r5
0065f8f4: str      r3, [r4, #0x4c]
0065f8f8: bl       #0x65f0b4
0065f8fc: bl       #0x60e334
0065f900: ldr      r3, [r4]
0065f904: mov      r6, r0
0065f908: mov      r0, r4
0065f90c: mov      lr, pc
0065f910: ldr      pc, [r3, #0x44]
0065f914: cmp      r0, #0
0065f918: beq      #0x65f9fc
0065f91c: ldr      r8, [r6]
0065f920: cmp      r8, #0
0065f924: beq      #0x65f968
0065f928: ldr      r3, [r4]
0065f92c: mov      r0, r4
0065f930: mov      lr, pc
0065f934: ldr      pc, [r3, #0x44]
0065f938: str      r6, [r0, #0x34]
0065f93c: ldr      r2, [r6]
0065f940: cmp      r2, #0
0065f944: moveq    r1, #1
0065f948: streq    r1, [r0, #0x14]
0065f94c: streq    r2, [r0, #0x10]
0065f950: beq      #0x65f9d4
0065f954: ldr      r3, [r0]
0065f958: mov      r1, #0
0065f95c: mov      lr, pc
0065f960: ldr      pc, [r3, #0x10]
0065f964: b        #0x65f9d4
0065f968: ldr      r3, [r4]
0065f96c: mov      r0, r4
0065f970: mov      lr, pc
0065f974: ldr      pc, [r3, #0x44]
0065f978: mov      r7, #1
0065f97c: str      r8, [r0, #0x10]
0065f980: str      r8, [r0, #0x34]
0065f984: str      r7, [r0, #0x14]
0065f988: ldr      r3, [r4]
0065f98c: mov      r0, r4
0065f990: mov      lr, pc
0065f994: ldr      pc, [r3, #0x44]
0065f998: ldr      r3, [r0]
0065f99c: mov      r8, r0
0065f9a0: mov      r1, r5
0065f9a4: mov      r0, r4
0065f9a8: ldr      r6, [r3, #0x50]
0065f9ac: bl       #0x65f104
0065f9b0: mov      r1, r5
0065f9b4: mov      sl, r0
0065f9b8: mov      r0, r4
0065f9bc: bl       #0x65f10c
0065f9c0: mov      r1, sl
0065f9c4: mov      r2, r0
0065f9c8: mov      r3, r7
0065f9cc: mov      r0, r8
0065f9d0: blx      r6
0065f9d4: mov      r1, r5
0065f9d8: ldr      r0, [r4, #0x24]
0065f9dc: bl       #0x65f0b4
0065f9e0: ldr      r3, [r0]
0065f9e4: mov      r0, r4
0065f9e8: ldr      r3, [r3, #0x24]
0065f9ec: ldr      r3, [r3, #0x20]
0065f9f0: ldr      r1, [r3, #0x2c]
0065f9f4: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
0065f9f8: b        #0x60fab8
0065f9fc: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

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

# _ZN13RootSceneNode11_ResetDeltaEj
0035ce6c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0035ce70: mov      r5, r0
0035ce74: ldr      r4, [r5, #0xfc]!
0035ce78: ldr      r7, [pc, #0xb0]
0035ce7c: ldr      sl, [pc, #0xb0]
0035ce80: ldr      sb, [pc, #0xb0]
0035ce84: ldr      fp, [pc, #0xb0]
0035ce88: ldr      r3, [pc, #0xb0]
0035ce8c: sub      sp, sp, #0x14
0035ce90: cmp      r5, r4
0035ce94: add      r7, pc, r7
0035ce98: mov      r6, r1
0035ce9c: add      sl, pc, sl
0035cea0: add      sb, pc, sb
0035cea4: add      fp, pc, fp
0035cea8: ldr      r8, [pc, #0x94]
0035ceac: str      r3, [sp, #0xc]
0035ceb0: beq      #0x35cee0
0035ceb4: ldr      r0, [r4, #8]
0035ceb8: bl       #0x369160
0035cebc: subs     r3, r0, #0
0035cec0: mov      r1, r6
0035cec4: beq      #0x35cee8
0035cec8: ldr      r3, [r3]
0035cecc: mov      lr, pc
0035ced0: ldr      pc, [r3, #0xc]
0035ced4: ldr      r4, [r4]
0035ced8: cmp      r5, r4
0035cedc: bne      #0x35ceb4
0035cee0: add      sp, sp, #0x14
0035cee4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0035cee8: ldr      r2, [r7, r8]
0035ceec: ldr      r2, [r2]
0035cef0: cmp      r2, #2
0035cef4: streq    r3, [r3]
0035cef8: beq      #0x35ced4
0035cefc: cmp      r2, #1
0035cf00: bne      #0x35ced4
0035cf04: ldr      ip, [sp, #0xc]
0035cf08: mov      r1, sl
0035cf0c: mov      r2, sb
0035cf10: ldr      r0, [r7, ip]
0035cf14: mov      r3, fp
0035cf18: mov      ip, #0xde
0035cf1c: add      r0, r0, #0xa8
0035cf20: str      ip, [sp]
0035cf24: bl       #0x30e004
0035cf28: ldr      r4, [r4]
0035cf2c: b        #0x35ced8

# _ZN13RootSceneNode19_EnableDisplacementEb
0035d4cc: push     {r4, r5, r6, r7, r8, lr}
0035d4d0: subs     r5, r1, #0
0035d4d4: mov      r4, r0
0035d4d8: beq      #0x35d4e8
0035d4dc: ldr      r6, [r0, #0x1f0]
0035d4e0: cmp      r6, #0
0035d4e4: beq      #0x35d4f0
0035d4e8: strb     r5, [r4, #0x1ec]
0035d4ec: pop      {r4, r5, r6, r7, r8, pc}
0035d4f0: mov      r1, r6
0035d4f4: bl       #0x35ccdc
0035d4f8: mov      r1, #1
0035d4fc: str      r0, [r4, #0x1f0]
0035d500: mov      r0, r4
0035d504: bl       #0x35ccdc
0035d508: ldr      r3, [r4, #0x1f0]
0035d50c: str      r0, [r4, #0x1f4]
0035d510: cmp      r3, #0
0035d514: beq      #0x35d61c
0035d518: cmp      r0, r3
0035d51c: streq    r6, [r4, #0x1f4]
0035d520: beq      #0x35d548
0035d524: cmp      r0, #0
0035d528: beq      #0x35d548
0035d52c: ldr      r3, [r0]
0035d530: ldr      r3, [r3, #-0xc]
0035d534: add      r0, r0, r3
0035d538: ldr      r3, [r0, #4]
0035d53c: add      r3, r3, #1
0035d540: str      r3, [r0, #4]
0035d544: ldr      r3, [r4, #0x1f0]
0035d548: ldr      r2, [r3]
0035d54c: mov      r1, #0
0035d550: mov      r0, #0x150
0035d554: ldr      r2, [r2, #-0xc]
0035d558: mov      r7, r4
0035d55c: add      r3, r3, r2
0035d560: ldr      r2, [r3, #4]
0035d564: add      r2, r2, #1
0035d568: str      r2, [r3, #4]
0035d56c: bl       #0x5341ac
0035d570: mvn      r1, #0
0035d574: mov      r6, r0
0035d578: bl       #0x5839d8
0035d57c: str      r6, [r4, #0x1f8]
0035d580: mov      r3, r6
0035d584: ldr      r6, [r7, #0xf4]!
0035d588: cmp      r6, r7
0035d58c: bne      #0x35d598
0035d590: b        #0x35d5c4
0035d594: ldr      r3, [r4, #0x1f8]
0035d598: cmp      r6, #0
0035d59c: moveq    r1, r6
0035d5a0: subne    r1, r6, #4
0035d5a4: ldr      r6, [r6]
0035d5a8: mov      r0, r3
0035d5ac: ldr      r3, [r3]
0035d5b0: mov      lr, pc
0035d5b4: ldr      pc, [r3, #0x5c]
0035d5b8: cmp      r7, r6
0035d5bc: bne      #0x35d594
0035d5c0: ldr      r3, [r4, #0x1f8]
0035d5c4: mov      r1, r3
0035d5c8: mov      r0, r4
0035d5cc: ldr      r3, [r4]
0035d5d0: mov      r7, r4
0035d5d4: mov      lr, pc
0035d5d8: ldr      pc, [r3, #0x5c]
0035d5dc: ldr      r6, [r7, #0xfc]!
0035d5e0: b        #0x35d608
0035d5e4: ldr      r0, [r6, #8]
0035d5e8: bl       #0x369160
0035d5ec: subs     r3, r0, #0
0035d5f0: beq      #0x35d614
0035d5f4: ldr      r3, [r3]
0035d5f8: ldr      r1, [r4, #0x1f0]
0035d5fc: mov      lr, pc
0035d600: ldr      pc, [r3, #8]
0035d604: ldr      r6, [r6]
0035d608: cmp      r7, r6
0035d60c: bne      #0x35d5e4
0035d610: b        #0x35d4e8
0035d614: mov      r5, r3
0035d618: b        #0x35d4e8
0035d61c: str      r3, [r4, #0x1f4]
0035d620: pop      {r4, r5, r6, r7, r8, pc}

# _ZN15AnimatorBlender9BlendPostEv
00366740: bx       lr
