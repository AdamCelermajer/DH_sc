
# _ZN6glitch7collada18CSceneNodeAnimator20applyAnimationValuesEj
0065d91c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0065d920: ldr      r2, [r0, #0x48]
0065d924: ldr      r3, [r0, #0x44]
0065d928: sub      sp, sp, #0x28
0065d92c: mov      r5, r0
0065d930: rsb      r3, r3, r2
0065d934: lsrs     r3, r3, #4
0065d938: mov      r4, r1
0065d93c: bne      #0x65d94c
0065d940: ldr      r3, [r0, #0x50]
0065d944: cmp      r3, #0
0065d948: beq      #0x65da18
0065d94c: mov      r0, r5
0065d950: mov      r1, r4
0065d954: bl       #0x667c48
0065d958: ldr      r3, [r5]
0065d95c: mov      r0, r5
0065d960: mov      lr, pc
0065d964: ldr      pc, [r3, #0x44]
0065d968: cmp      r0, #0
0065d96c: ldrne    r6, [r0, #4]
0065d970: beq      #0x65da20
0065d974: mov      r1, r6
0065d978: mov      r0, r5
0065d97c: ldr      r7, [r5, #0xc]
0065d980: bl       #0x65d768
0065d984: ldr      r3, [r5, #0x44]
0065d988: mov      r8, r0
0065d98c: ldr      r0, [r5, #0x48]
0065d990: ldrb     r2, [r5, #0x34]
0065d994: subs     r7, r7, #1
0065d998: movne    r7, #1
0065d99c: rsb      r1, r3, r0
0065d9a0: lsrs     r1, r1, #4
0065d9a4: strb     r2, [sp, #0x19]
0065d9a8: beq      #0x65da18
0065d9ac: mov      r4, #0
0065d9b0: add      sl, sp, #0xc
0065d9b4: add      sb, sp, #0x1c
0065d9b8: add      r1, r3, r4, lsl #4
0065d9bc: ldr      r2, [r1, #4]
0065d9c0: add      ip, r3, #0xc
0065d9c4: cmp      r2, #0
0065d9c8: beq      #0x65da08
0065d9cc: ldr      r0, [r3, r4, lsl #4]
0065d9d0: ldrb     r3, [r5, #0x34]
0065d9d4: str      r8, [sp, #0x20]
0065d9d8: str      r0, [sp, #0x1c]
0065d9dc: cmp      r3, #0
0065d9e0: str      sl, [sp, #0x24]
0065d9e4: ldr      r3, [r1, #8]
0065d9e8: addeq    ip, r1, #0xc
0065d9ec: mov      r0, sb
0065d9f0: mov      r1, r6
0065d9f4: str      ip, [sp]
0065d9f8: str      r7, [sp, #4]
0065d9fc: bl       #0x66a0c0
0065da00: ldr      r0, [r5, #0x48]
0065da04: ldr      r3, [r5, #0x44]
0065da08: add      r4, r4, #1
0065da0c: rsb      r2, r3, r0
0065da10: cmp      r4, r2, asr #4
0065da14: blo      #0x65d9b8
0065da18: add      sp, sp, #0x28
0065da1c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0065da20: mov      r0, r4
0065da24: ldr      r1, [r5, #0x14]
0065da28: bl       #0x30eb2c
0065da2c: ldr      r6, [r5, #0x38]
0065da30: add      r6, r1, r6
0065da34: b        #0x65d974

# _ZN6glitch7collada18ISceneNodeAnimator9setTargetEPKcPv
00667dc4: push     {r4, r5, r6, r7, r8, lr}
00667dc8: ldr      r3, [r0]
00667dcc: mov      r4, r0
00667dd0: mov      r5, r1
00667dd4: mov      r8, r2
00667dd8: mov      lr, pc
00667ddc: ldr      pc, [r3, #0x70]
00667de0: subs     r7, r0, #0
00667de4: ble      #0x667e3c
00667de8: mov      r6, #0
00667dec: b        #0x667dfc
00667df0: add      r6, r6, #1
00667df4: cmp      r6, r7
00667df8: beq      #0x667e3c
00667dfc: ldr      r3, [r4]
00667e00: mov      r1, r6
00667e04: mov      r0, r4
00667e08: mov      lr, pc
00667e0c: ldr      pc, [r3, #0x6c]
00667e10: mov      r1, r5
00667e14: bl       #0x30e31c
00667e18: subs     r3, r0, #0
00667e1c: bne      #0x667df0
00667e20: mov      r0, r4
00667e24: mov      r1, r6
00667e28: mov      r2, r8
00667e2c: ldr      ip, [r4]
00667e30: mov      lr, pc
00667e34: ldr      pc, [ip, #0x68]
00667e38: pop      {r4, r5, r6, r7, r8, pc}
00667e3c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada13CAnimationSet12addAnimationEPKNS0_10SAnimationE
006601fc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00660200: ldr      r3, [r0, #0xc]
00660204: ldr      fp, [r0, #0x10]
00660208: ldr      r2, [pc, #0x28c]
0066020c: sub      sp, sp, #0x14
00660210: rsb      fp, r3, fp
00660214: str      r1, [sp, #0xc]
00660218: asrs     fp, fp, #2
0066021c: add      r2, pc, r2
00660220: mov      r5, r0
00660224: ldr      r4, [r1, #0x10]
00660228: beq      #0x660330
0066022c: ldr      r1, [pc, #0x26c]
00660230: mov      r6, #0
00660234: mov      sl, #0xc
00660238: ldr      sb, [r2, r1]
0066023c: ldr      r2, [pc, #0x260]
00660240: mov      r8, #1
00660244: lsl      r7, r6, #2
00660248: add      r2, pc, r2
0066024c: str      r2, [sp, #8]
00660250: ldr      r1, [r3, r6, lsl #2]
00660254: ldr      r3, [r4, #8]
00660258: ldr      r2, [sb]
0066025c: ldr      r1, [r1, #8]
00660260: cmp      r3, #0x5b
00660264: mla      r2, sl, r1, r2
00660268: bhi      #0x660300
0066026c: lsr      r1, r3, #5
00660270: ldr      r2, [r2, r1, lsl #2]
00660274: and      r3, r3, #0x1f
00660278: ands     r2, r2, r8, lsl r3
0066027c: beq      #0x6602d0
00660280: ldr      r3, [r5, #0xc]
00660284: ldr      r1, [r4, #4]
00660288: ldr      r7, [r3, r7]
0066028c: ldr      r0, [r7, #4]
00660290: bl       #0x30e31c
00660294: cmp      r0, #0
00660298: bne      #0x6602d0
0066029c: ldr      r3, [r4, #8]
006602a0: cmp      r3, #0xe
006602a4: beq      #0x66031c
006602a8: cmp      r3, #0x56
006602ac: beq      #0x6602bc
006602b0: mov      r0, r6
006602b4: add      sp, sp, #0x14
006602b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006602bc: ldr      r0, [r7, #0xc]
006602c0: ldr      r1, [r4, #0xc]
006602c4: bl       #0x30e31c
006602c8: cmp      r0, #0
006602cc: beq      #0x6602b0
006602d0: add      r6, r6, #1
006602d4: cmp      r6, fp
006602d8: beq      #0x660330
006602dc: ldr      r3, [r5, #0xc]
006602e0: ldr      r2, [sb]
006602e4: lsl      r7, r6, #2
006602e8: ldr      r1, [r3, r6, lsl #2]
006602ec: ldr      r3, [r4, #8]
006602f0: ldr      r1, [r1, #8]
006602f4: cmp      r3, #0x5b
006602f8: mla      r2, sl, r1, r2
006602fc: bls      #0x66026c
00660300: ldr      r0, [sp, #8]
00660304: str      r2, [sp, #4]
00660308: str      r3, [sp]
0066030c: bl       #0x708eb0
00660310: ldr      r3, [sp]
00660314: ldr      r2, [sp, #4]
00660318: b        #0x66026c
0066031c: ldrb     r2, [r7, #0xc]
00660320: ldrb     r3, [r4, #0xc]
00660324: cmp      r2, r3
00660328: bne      #0x6602d0
0066032c: b        #0x6602b0
00660330: ldr      r0, [sp, #0xc]
00660334: bl       #0x611ae0
00660338: subs     r6, r0, #0
0066033c: mvneq    r0, #0
00660340: beq      #0x6602b4
00660344: ldr      sl, [r5, #0x10]
00660348: ldr      r3, [r5, #0x14]
0066034c: cmp      sl, r3
00660350: beq      #0x66039c
00660354: str      r4, [sl]
00660358: ldr      r3, [r5, #0x10]
0066035c: add      r3, r3, #4
00660360: str      r3, [r5, #0x10]
00660364: ldr      r8, [r5, #0x1c]
00660368: ldr      r3, [r5, #0x20]
0066036c: cmp      r8, r3
00660370: beq      #0x66041c
00660374: str      r6, [r8]
00660378: ldr      r3, [r5, #0x1c]
0066037c: add      r3, r3, #4
00660380: str      r3, [r5, #0x1c]
00660384: ldr      r3, [r5, #0xc]
00660388: ldr      r0, [r5, #0x10]
0066038c: rsb      r0, r3, r0
00660390: asr      r0, r0, #2
00660394: sub      r0, r0, #1
00660398: b        #0x6602b4
0066039c: ldr      r3, [r5, #0xc]
006603a0: rsb      r3, r3, sl
006603a4: asr      r3, r3, #2
006603a8: cmp      r3, #1
006603ac: addhs    r8, r3, r3
006603b0: addlo    r8, r3, #1
006603b4: cmn      r8, #0xc0000001
006603b8: bhi      #0x660414
006603bc: cmp      r3, r8
006603c0: bhi      #0x660414
006603c4: lsl      r8, r8, #2
006603c8: mov      r1, #0
006603cc: mov      r0, r8
006603d0: bl       #0x310568
006603d4: ldr      r1, [r5, #0xc]
006603d8: mov      r7, r0
006603dc: subs     sl, sl, r1
006603e0: moveq    sl, r0
006603e4: beq      #0x6603f4
006603e8: mov      r2, sl
006603ec: bl       #0x30df38
006603f0: add      sl, r0, sl
006603f4: str      r4, [sl], #4
006603f8: ldr      r0, [r5, #0xc]
006603fc: add      r8, r7, r8
00660400: bl       #0x310450
00660404: str      sl, [r5, #0x10]
00660408: str      r8, [r5, #0x14]
0066040c: str      r7, [r5, #0xc]
00660410: b        #0x660364
00660414: mvn      r8, #0xc0000000
00660418: b        #0x6603c4
0066041c: ldr      r3, [r5, #0x18]
00660420: rsb      r3, r3, r8
00660424: asr      r3, r3, #2
00660428: cmp      r3, #1
0066042c: addhs    r7, r3, r3
00660430: addlo    r7, r3, #1
00660434: cmn      r7, #0xc0000001
00660438: bhi      #0x660494
0066043c: cmp      r3, r7
00660440: bhi      #0x660494
00660444: lsl      r7, r7, #2
00660448: mov      r1, #0
0066044c: mov      r0, r7
00660450: bl       #0x310568
00660454: ldr      r1, [r5, #0x18]
00660458: mov      r4, r0
0066045c: subs     r8, r8, r1
00660460: moveq    r8, r0
00660464: beq      #0x660474
00660468: mov      r2, r8
0066046c: bl       #0x30df38
00660470: add      r8, r0, r8
00660474: str      r6, [r8], #4
00660478: ldr      r0, [r5, #0x18]
0066047c: add      r7, r4, r7
00660480: bl       #0x310450
00660484: str      r8, [r5, #0x1c]
00660488: str      r7, [r5, #0x20]
0066048c: str      r4, [r5, #0x18]
00660490: b        #0x660384
00660494: mvn      r7, #0xc0000000
00660498: b        #0x660444
0066049c: eorseq   r4, r3, r4, ror r8
006604a0: andeq    r4, r0, ip, asr #10
006604a4: eoreq    r1, r6, r0, lsl #21

# _ZN6glitch7collada35CAnimationSetTransformationTemplate16isAnimationExistEPKNS0_8SChannelE
006e22cc: push     {r4, r5, r6, r7, r8, lr}
006e22d0: ldr      r3, [r0, #4]
006e22d4: ldr      r2, [r0, #8]
006e22d8: mov      r5, r0
006e22dc: mov      r6, r1
006e22e0: rsb      r2, r3, r2
006e22e4: lsrs     r2, r2, #2
006e22e8: beq      #0x6e2394
006e22ec: mov      r4, #0
006e22f0: mov      r8, #1
006e22f4: b        #0x6e234c
006e22f8: ldr      r3, [r6, #8]
006e22fc: cmp      r3, #0xd
006e2300: bhi      #0x6e237c
006e2304: lsl      r3, r8, r3
006e2308: tst      r3, #0x3c00
006e230c: mov      r0, #1
006e2310: bne      #0x6e23b8
006e2314: tst      r3, #0x3e0
006e2318: bne      #0x6e239c
006e231c: tst      r3, #0x1e
006e2320: beq      #0x6e237c
006e2324: ldr      r3, [r5, #4]
006e2328: ldr      r2, [r3, r7]
006e232c: ldr      r0, [r2, #4]
006e2330: cmp      r0, #1
006e2334: beq      #0x6e23b0
006e2338: ldr      r2, [r5, #8]
006e233c: add      r4, r4, #1
006e2340: rsb      r2, r3, r2
006e2344: cmp      r4, r2, asr #2
006e2348: bhs      #0x6e2394
006e234c: ldr      r3, [r3, r4, lsl #2]
006e2350: lsl      r7, r4, #2
006e2354: ldr      r3, [r3, #8]
006e2358: mov      r0, r3
006e235c: ldr      r3, [r3]
006e2360: mov      lr, pc
006e2364: ldr      pc, [r3, #0x54]
006e2368: mov      r1, r0
006e236c: ldr      r0, [r6, #4]
006e2370: bl       #0x30e31c
006e2374: cmp      r0, #0
006e2378: beq      #0x6e22f8
006e237c: ldr      r3, [r5, #4]
006e2380: ldr      r2, [r5, #8]
006e2384: add      r4, r4, #1
006e2388: rsb      r2, r3, r2
006e238c: cmp      r4, r2, asr #2
006e2390: blo      #0x6e234c
006e2394: mov      r0, #0
006e2398: pop      {r4, r5, r6, r7, r8, pc}
006e239c: ldr      r3, [r5, #4]
006e23a0: ldr      r2, [r3, r7]
006e23a4: ldr      r1, [r2, #4]
006e23a8: cmp      r1, #5
006e23ac: bne      #0x6e2338
006e23b0: strb     r0, [r2]
006e23b4: pop      {r4, r5, r6, r7, r8, pc}
006e23b8: ldr      r3, [r5, #4]
006e23bc: ldr      r2, [r3, r7]
006e23c0: ldr      r1, [r2, #4]
006e23c4: cmp      r1, #0xa
006e23c8: bne      #0x6e2338
006e23cc: b        #0x6e23b0

# _ZN6glitch7collada18CSceneNodeAnimator9setTargetEiPvPKNS0_15animation_track15CApplicatorInfoE
0065d408: push     {r4, r5, r6, lr}
0065d40c: ldr      ip, [r0, #0x44]
0065d410: lsl      r5, r1, #4
0065d414: mov      r6, r3
0065d418: add      ip, ip, r5
0065d41c: str      r2, [ip, #4]
0065d420: ldr      r3, [r0, #0x44]
0065d424: mov      r4, r0
0065d428: add      r3, r3, r5
0065d42c: ldr      r3, [r3, #8]
0065d430: cmp      r3, #0
0065d434: beq      #0x65d458
0065d438: mov      r0, r3
0065d43c: ldr      r3, [r3]
0065d440: mov      lr, pc
0065d444: ldr      pc, [r3, #4]
0065d448: ldr      r3, [r4, #0x44]
0065d44c: mov      r2, #0
0065d450: add      r3, r3, r5
0065d454: str      r2, [r3, #8]
0065d458: cmp      r6, #0
0065d45c: beq      #0x65d47c
0065d460: ldr      r2, [r4, #0x44]
0065d464: mov      r0, r6
0065d468: ldr      r3, [r6]
0065d46c: add      r5, r2, r5
0065d470: mov      lr, pc
0065d474: ldr      pc, [r3, #8]
0065d478: str      r0, [r5, #8]
0065d47c: pop      {r4, r5, r6, pc}
