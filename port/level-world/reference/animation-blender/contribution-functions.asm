
# _ZNSt6vectorIPN6glitch7collada18ISceneNodeAnimatorENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE18_M_insert_overflowEPS3_RKS3_RKSt11__true_typejb.clone.1
00476a9c: push     {r4, r5, r6, r7, r8, lr}
00476aa0: mov      r4, r0
00476aa4: ldr      r3, [r4]
00476aa8: ldr      r0, [r0, #4]
00476aac: mov      r6, r1
00476ab0: mov      r8, r2
00476ab4: rsb      r3, r3, r0
00476ab8: asr      r3, r3, #2
00476abc: cmp      r3, #1
00476ac0: addhs    r7, r3, r3
00476ac4: addlo    r7, r3, #1
00476ac8: cmn      r7, #0xc0000001
00476acc: bhi      #0x476b18
00476ad0: cmp      r3, r7
00476ad4: lslls    r7, r7, #2
00476ad8: bhi      #0x476b18
00476adc: mov      r1, #0
00476ae0: mov      r0, r7
00476ae4: bl       #0x310568
00476ae8: ldr      r1, [r4]
00476aec: mov      r5, r0
00476af0: subs     r6, r6, r1
00476af4: moveq    r6, r0
00476af8: bne      #0x476b3c
00476afc: ldr      r3, [r8]
00476b00: add      r7, r5, r7
00476b04: str      r3, [r6], #4
00476b08: ldr      r0, [r4]
00476b0c: bl       #0x310450
00476b10: stm      r4, {r5, r6, r7}
00476b14: pop      {r4, r5, r6, r7, r8, pc}
00476b18: mvn      r7, #3
00476b1c: mov      r1, #0
00476b20: mov      r0, r7
00476b24: bl       #0x310568
00476b28: ldr      r1, [r4]
00476b2c: mov      r5, r0
00476b30: subs     r6, r6, r1
00476b34: moveq    r6, r0
00476b38: beq      #0x476afc
00476b3c: mov      r2, r6
00476b40: bl       #0x30df38
00476b44: add      r6, r0, r6
00476b48: b        #0x476afc

# _ZN6glitch7collada25CSceneNodeAnimatorBlender9setTargetEiPvPKNS0_15animation_track15CApplicatorInfoE
0065e244: push     {r4, r5, r6, lr}
0065e248: ldr      ip, [r0, #0x58]
0065e24c: mov      r6, r3
0065e250: mov      r4, r0
0065e254: str      r2, [ip, r1, lsl #2]
0065e258: ldr      r3, [r0, #0x64]
0065e25c: mov      r5, r1
0065e260: ldr      r3, [r3, r1, lsl #2]
0065e264: cmp      r3, #0
0065e268: beq      #0x65e288
0065e26c: mov      r0, r3
0065e270: ldr      r3, [r3]
0065e274: mov      lr, pc
0065e278: ldr      pc, [r3, #4]
0065e27c: ldr      r3, [r4, #0x64]
0065e280: mov      r2, #0
0065e284: str      r2, [r3, r5, lsl #2]
0065e288: cmp      r6, #0
0065e28c: beq      #0x65e2a8
0065e290: mov      r0, r6
0065e294: ldr      r3, [r6]
0065e298: ldr      r4, [r4, #0x64]
0065e29c: mov      lr, pc
0065e2a0: ldr      pc, [r3, #8]
0065e2a4: str      r0, [r4, r5, lsl #2]
0065e2a8: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada25CSceneNodeAnimatorBlender17getAnimationTrackEi
0065e204: push     {r4, lr}
0065e208: ldr      r3, [r0, #0x28]
0065e20c: ldr      r3, [r3]
0065e210: mov      r0, r3
0065e214: ldr      r3, [r3]
0065e218: mov      lr, pc
0065e21c: ldr      pc, [r3, #0x54]
0065e220: pop      {r4, pc}

# _ZN6glitch7collada25CSceneNodeAnimatorBlender19getAnimationTrackExEi
0065e224: push     {r4, lr}
0065e228: ldr      r3, [r0, #0x28]
0065e22c: ldr      r3, [r3]
0065e230: mov      r0, r3
0065e234: ldr      r3, [r3]
0065e238: mov      lr, pc
0065e23c: ldr      pc, [r3, #0x58]
0065e240: pop      {r4, pc}

# _ZN14AnimSetManager11GetAnimatorEi
004762c4: push     {r4, r5, r6, lr}
004762c8: sub      sp, sp, #0x10
004762cc: str      r1, [sp, #4]
004762d0: mov      r5, r0
004762d4: bl       #0x475404
004762d8: subs     r4, r0, #0
004762dc: bne      #0x4762ec
004762e0: mov      r0, r4
004762e4: add      sp, sp, #0x10
004762e8: pop      {r4, r5, r6, pc}
004762ec: add      r0, r5, #4
004762f0: add      r1, sp, #4
004762f4: bl       #0x476058
004762f8: ldr      r3, [r0, #0x20]
004762fc: mov      r5, r0
00476300: ldrb     r2, [r3, #0x70]
00476304: cmp      r2, #0
00476308: bne      #0x476384
0047630c: add      r6, sp, #0x10
00476310: str      r5, [r6, #-4]!
00476314: ldr      r3, [r5, #4]
00476318: mov      r1, #0
0047631c: mov      r0, #0xa4
00476320: add      r3, r3, #1
00476324: str      r3, [r5, #4]
00476328: bl       #0x310570
0047632c: mov      r1, r6
00476330: mov      r4, r0
00476334: bl       #0x3676b8
00476338: ldr      r0, [sp, #0xc]
0047633c: cmp      r0, #0
00476340: beq      #0x476348
00476344: bl       #0x31d584
00476348: ldr      r3, [r4]
0047634c: mov      r0, r4
00476350: mov      lr, pc
00476354: ldr      pc, [r3, #0x44]
00476358: mov      r6, r0
0047635c: mov      r0, r5
00476360: bl       #0x3649bc
00476364: cmp      r6, #0
00476368: beq      #0x4762e0
0047636c: mov      r0, r6
00476370: ldr      r3, [r6]
00476374: mov      r1, #0
00476378: mov      lr, pc
0047637c: ldr      pc, [r3, #0x40]
00476380: b        #0x4762e0
00476384: mov      r0, r3
00476388: ldr      r3, [r3]
0047638c: mov      lr, pc
00476390: ldr      pc, [r3, #0x38]
00476394: b        #0x47630c

# _ZN6glitch7collada25CSceneNodeAnimatorBlender17getAnimationValueEiiPv
0065e34c: bx       lr

# _ZN6glitch7collada15animation_track8CBlenderINS_4core10quaternionELi1ES4_E17getBlendedValueExEPvPfiS6_
006130d4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006130d8: mov      ip, #0
006130dc: sub      sp, sp, #0x3c
006130e0: subs     r6, r2, #0
006130e4: mov      r2, #0x3f800000
006130e8: mov      r8, r0
006130ec: str      r2, [sp, #0x34]
006130f0: mov      r5, r1
006130f4: mov      sb, r3
006130f8: str      ip, [sp, #0x28]
006130fc: str      ip, [sp, #0x2c]
00613100: str      ip, [sp, #0x30]
00613104: ble      #0x61328c
00613108: mov      r1, ip
0061310c: ldr      r0, [r5]
00613110: bl       #0x30df8c
00613114: cmp      r0, #0
00613118: moveq    r3, #0
0061311c: moveq    r7, r5
00613120: moveq    r4, r3
00613124: beq      #0x613224
00613128: mov      r7, r5
0061312c: mov      r4, #0
00613130: b        #0x613144
00613134: ldr      r0, [r7, #4]!
00613138: bl       #0x30df8c
0061313c: cmp      r0, #0
00613140: beq      #0x613220
00613144: add      r4, r4, #1
00613148: cmp      r4, r6
0061314c: mov      r1, #0
00613150: bne      #0x613134
00613154: add      r4, r6, #1
00613158: mov      sl, #0
0061315c: cmp      r6, r4
00613160: ble      #0x6131f8
00613164: add      r3, sp, #4
00613168: add      r5, r5, r4, lsl #2
0061316c: add      r8, r8, r4, lsl #4
00613170: add      fp, sp, #0x28
00613174: str      r3, [sp, #0x24]
00613178: b        #0x61318c
0061317c: cmp      r4, r6
00613180: add      r5, r5, #4
00613184: add      r8, r8, #0x10
00613188: beq      #0x6131f8
0061318c: ldr      r7, [r5]
00613190: mov      r1, #0
00613194: add      r4, r4, #1
00613198: mov      r0, r7
0061319c: bl       #0x30df8c
006131a0: cmp      r0, #0
006131a4: bne      #0x61317c
006131a8: mov      r0, sl
006131ac: mov      r1, r7
006131b0: bl       #0x30eba4
006131b4: ldr      ip, [sp, #0x24]
006131b8: mov      sl, r0
006131bc: ldm      r8, {r0, r1, r2, r3}
006131c0: stm      ip, {r0, r1, r2, r3}
006131c4: mov      r1, sl
006131c8: mov      r0, r7
006131cc: bl       #0x30ec94
006131d0: ldm      fp, {r1, r2, r3}
006131d4: ldr      ip, [sp, #0x34]
006131d8: str      r0, [sp, #0x14]
006131dc: mov      r0, fp
006131e0: str      ip, [sp]
006131e4: bl       #0x612d00
006131e8: cmp      r4, r6
006131ec: add      r5, r5, #4
006131f0: add      r8, r8, #0x10
006131f4: bne      #0x61318c
006131f8: ldr      r1, [sp, #0x2c]
006131fc: ldr      r3, [sp, #0x30]
00613200: ldr      r2, [sp, #0x34]
00613204: ldr      r0, [sp, #0x28]
00613208: str      r1, [sb, #4]
0061320c: str      r2, [sb, #0xc]
00613210: str      r0, [sb]
00613214: str      r3, [sb, #8]
00613218: add      sp, sp, #0x3c
0061321c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00613220: lsl      r3, r4, #4
00613224: ldr      sl, [r7]
00613228: add      r2, r8, r3
0061322c: ldr      r7, [r8, r3]
00613230: ldr      fp, [r2, #0xc]
00613234: ldr      r3, [r2, #4]
00613238: ldr      r2, [r2, #8]
0061323c: mov      r0, sl
00613240: mov      r1, #0x3f800000
00613244: str      r3, [sp, #0x2c]
00613248: str      r2, [sp, #0x30]
0061324c: str      r2, [sp, #0x1c]
00613250: str      r3, [sp, #0x20]
00613254: str      r7, [sp, #0x28]
00613258: str      fp, [sp, #0x34]
0061325c: bl       #0x30df8c
00613260: cmp      r0, #0
00613264: ldr      r2, [sp, #0x1c]
00613268: ldr      r3, [sp, #0x20]
0061326c: beq      #0x613284
00613270: str      fp, [sb, #0xc]
00613274: str      r7, [sb]
00613278: str      r3, [sb, #4]
0061327c: str      r2, [sb, #8]
00613280: b        #0x613218
00613284: add      r4, r4, #1
00613288: b        #0x61315c
0061328c: mov      r4, #1
00613290: b        #0x613158
