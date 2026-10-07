
# _ZN10AnimatedFXC2Ei 004922e0 size148
004922e0: push {r4, r5}
004922e4: ldr r4, [pc, #0x80]
004922e8: ldr r2, [pc, #0x80]
004922ec: str r1, [r0, #8]
004922f0: add r4, pc, r4
004922f4: ldr r2, [r4, r2]
004922f8: mvn r1, #0
004922fc: str r1, [r0, #0x1c]
00492300: mov r1, #0x3f800000
00492304: mov ip, #0
00492308: add r5, r2, #8
0049230c: str r1, [r0, #0x20]
00492310: mov r2, #0
00492314: mov r1, #1
00492318: str r2, [r0, #0x50]
0049231c: str r5, [r0]
00492320: strb r1, [r0, #0x24]
00492324: str ip, [r0, #0x48]
00492328: str r2, [r0, #0xc]
0049232c: str r2, [r0, #0x10]
00492330: str r2, [r0, #0x14]
00492334: strb r2, [r0, #0x18]
00492338: str r2, [r0, #0x28]
0049233c: str r2, [r0, #0x2c]
00492340: strb r2, [r0, #0x30]
00492344: strb r2, [r0, #0x31]
00492348: strb r2, [r0, #0x32]
0049234c: str ip, [r0, #0x34]
00492350: str ip, [r0, #0x38]
00492354: str ip, [r0, #0x3c]
00492358: str ip, [r0, #0x40]
0049235c: str ip, [r0, #0x44]
00492360: strb r2, [r0, #0x4c]
00492364: pop {r4, r5}
00492368: bx lr
0049236c: subseq r2, r0, r0, lsr #15
00492370: andeq r3, r0, ip, lsl r1

# _ZN10AnimatedFXC1Ei 00492374 size148
00492374: push {r4, r5}
00492378: ldr r4, [pc, #0x80]
0049237c: ldr r2, [pc, #0x80]
00492380: str r1, [r0, #8]
00492384: add r4, pc, r4
00492388: ldr r2, [r4, r2]
0049238c: mvn r1, #0
00492390: str r1, [r0, #0x1c]
00492394: mov r1, #0x3f800000
00492398: mov ip, #0
0049239c: add r5, r2, #8
004923a0: str r1, [r0, #0x20]
004923a4: mov r2, #0
004923a8: mov r1, #1
004923ac: str r2, [r0, #0x50]
004923b0: str r5, [r0]
004923b4: strb r1, [r0, #0x24]
004923b8: str ip, [r0, #0x48]
004923bc: str r2, [r0, #0xc]
004923c0: str r2, [r0, #0x10]
004923c4: str r2, [r0, #0x14]
004923c8: strb r2, [r0, #0x18]
004923cc: str r2, [r0, #0x28]
004923d0: str r2, [r0, #0x2c]
004923d4: strb r2, [r0, #0x30]
004923d8: strb r2, [r0, #0x31]
004923dc: strb r2, [r0, #0x32]
004923e0: str ip, [r0, #0x34]
004923e4: str ip, [r0, #0x38]
004923e8: str ip, [r0, #0x3c]
004923ec: str ip, [r0, #0x40]
004923f0: str ip, [r0, #0x44]
004923f4: strb r2, [r0, #0x4c]
004923f8: pop {r4, r5}
004923fc: bx lr
00492400: subseq r2, r0, ip, lsl #14
00492404: andeq r3, r0, ip, lsl r1

# _ZN10AnimatedFXD2Ev 00492408 size84
00492408: push {r4, lr}
0049240c: ldr r3, [pc, #0x40]
00492410: ldr r2, [pc, #0x40]
00492414: ldr r1, [r0, #0x2c]
00492418: add r3, pc, r3
0049241c: ldr r2, [r3, r2]
00492420: cmp r1, #0
00492424: mov r4, r0
00492428: add r2, r2, #8
0049242c: str r2, [r0]
00492430: beq #0x49244c
00492434: ldr r3, [r1]
00492438: mov r0, r1
0049243c: mov lr, pc
00492440: ldr pc, [r3, #4]
00492444: mov r3, #0
00492448: str r3, [r4, #0x2c]
0049244c: mov r0, r4
00492450: pop {r4, pc}
00492454: subseq r2, r0, r8, ror r6
00492458: andeq r3, r0, ip, lsl r1

# _ZN10AnimatedFXD1Ev 0049245c size84
0049245c: push {r4, lr}
00492460: ldr r3, [pc, #0x40]
00492464: ldr r2, [pc, #0x40]
00492468: ldr r1, [r0, #0x2c]
0049246c: add r3, pc, r3
00492470: ldr r2, [r3, r2]
00492474: cmp r1, #0
00492478: mov r4, r0
0049247c: add r2, r2, #8
00492480: str r2, [r0]
00492484: beq #0x4924a0
00492488: ldr r3, [r1]
0049248c: mov r0, r1
00492490: mov lr, pc
00492494: ldr pc, [r3, #4]
00492498: mov r3, #0
0049249c: str r3, [r4, #0x2c]
004924a0: mov r0, r4
004924a4: pop {r4, pc}
004924a8: subseq r2, r0, r4, lsr #12
004924ac: andeq r3, r0, ip, lsl r1

# _ZN10AnimatedFX8SetSpeedEf 004924b0 size48
004924b0: push {r4, lr}
004924b4: ldr r3, [r0, #0x2c]
004924b8: str r1, [r0, #0x20]
004924bc: cmp r3, #0
004924c0: beq #0x4924dc
004924c4: ldr r3, [r3, #0x38]
004924c8: mov r2, #0
004924cc: mov r0, r3
004924d0: ldr r3, [r3]
004924d4: mov lr, pc
004924d8: ldr pc, [r3, #0x28]
004924dc: pop {r4, pc}

# _ZNK10AnimatedFX12HasCompletedEv 004924e0 size112
004924e0: push {r4, lr}
004924e4: ldr r3, [pc, #0x5c]
004924e8: ldr r1, [pc, #0x5c]
004924ec: ldr r2, [r0, #0x2c]
004924f0: add r3, pc, r3
004924f4: ldr r0, [r3, r1]
004924f8: movw r1, #0x6164
004924fc: movt r1, #0x7065
00492500: ldr r0, [r0, #0x10]
00492504: ldr r2, [r2, #8]
00492508: ldr r3, [r0, #0x1c]
0049250c: mov r0, r3
00492510: ldr r3, [r3]
00492514: mov lr, pc
00492518: ldr pc, [r3, #0x1c]
0049251c: subs r3, r0, #0
00492520: beq #0x492540
00492524: ldr r3, [r3]
00492528: mov lr, pc
0049252c: ldr pc, [r3, #0xf8]
00492530: cmp r0, #0
00492534: movgt r0, #0
00492538: movle r0, #1
0049253c: pop {r4, pc}
00492540: mov r0, #1
00492544: pop {r4, pc}
00492548: subseq r2, r0, r0, lsr #11
0049254c: strdeq r3, r4, [r0], -r4

# _ZN10AnimatedFX17GetAnimControllerEv 00492550 size16
00492550: ldr r0, [r0, #0x2c]
00492554: cmp r0, #0
00492558: ldrne r0, [r0, #0x38]
0049255c: bx lr

# _ZN10AnimatedFX11SetEndPointERK7Point3DIfE 00492560 size284
00492560: push {r4, r5, r6, r7, r8, lr}
00492564: mov r4, r1
00492568: movw r1, #0x6164
0049256c: mov r6, r0
00492570: movt r1, #0x7065
00492574: ldr r0, [r0, #0x2c]
00492578: bl #0x4709dc
0049257c: subs r5, r0, #0
00492580: beq #0x492678
00492584: ldr r0, [r6, #0x28]
00492588: cmp r0, #0
0049258c: beq #0x492678
00492590: bl #0x3935dc
00492594: mov r6, r0
00492598: ldr r1, [r0]
0049259c: ldr r0, [r4]
004925a0: bl #0x30e3ac
004925a4: ldr r1, [r6, #4]
004925a8: mov r8, r0
004925ac: ldr r0, [r4, #4]
004925b0: bl #0x30e3ac
004925b4: ldr r1, [r6, #8]
004925b8: mov r7, r0
004925bc: ldr r0, [r4, #8]
004925c0: bl #0x30e3ac
004925c4: mov r1, r8
004925c8: mov r6, r0
004925cc: mov r0, r8
004925d0: bl #0x30ed6c
004925d4: mov r1, r7
004925d8: mov r4, r0
004925dc: mov r0, r7
004925e0: bl #0x30ed6c
004925e4: mov r1, r0
004925e8: mov r0, r4
004925ec: bl #0x30eba4
004925f0: mov r1, r6
004925f4: mov r4, r0
004925f8: mov r0, r6
004925fc: bl #0x30ed6c
00492600: mov r1, r0
00492604: mov r0, r4
00492608: bl #0x30eba4
0049260c: bl #0x30e124
00492610: ldr r4, [r5, #0x178]
00492614: cmp r4, #0
00492618: addne r4, r4, #0x60
0049261c: ldr r3, [r4, #8]
00492620: cmp r3, #0
00492624: bne #0x492678
00492628: movw r1, #0x999a
0049262c: movt r1, #0x3e99
00492630: bl #0x30ed6c
00492634: ldr r3, [r4, #4]
00492638: mov r6, r0
0049263c: mov r0, r5
00492640: str r6, [r3, #0x2c]
00492644: ldr r3, [r5]
00492648: mov lr, pc
0049264c: ldr pc, [r3, #0xa0]
00492650: mov r1, #0xbf000000
00492654: mov r4, r0
00492658: mov r0, r6
0049265c: bl #0x30ed6c
00492660: ldr r3, [r4, #8]
00492664: ldr r2, [r4, #4]
00492668: mov r1, r0
0049266c: mov r0, r5
00492670: pop {r4, r5, r6, r7, r8, lr}
00492674: b #0x597154
00492678: pop {r4, r5, r6, r7, r8, pc}

# _ZN10AnimatedFX11GetAnimatorEv 0049267c size24
0049267c: ldr r0, [r0, #0x2c]
00492680: cmp r0, #0
00492684: bxeq lr
00492688: ldr r0, [r0, #0x38]
0049268c: mov r1, #0
00492690: b #0x4748b8

# _ZN10AnimatedFX10SetLoopingEb 00492694 size80
00492694: push {r4, r5, r6, lr}
00492698: ldr r3, [r0, #0x2c]
0049269c: mov r4, r0
004926a0: mov r5, r1
004926a4: cmp r3, #0
004926a8: beq #0x4926dc
004926ac: bl #0x49267c
004926b0: cmp r0, #0
004926b4: beq #0x4926dc
004926b8: mov r0, r4
004926bc: bl #0x49267c
004926c0: ldr r3, [r0]
004926c4: mov lr, pc
004926c8: ldr pc, [r3, #0x44]
004926cc: mov r1, r5
004926d0: ldr r3, [r0]
004926d4: mov lr, pc
004926d8: ldr pc, [r3, #0x40]
004926dc: strb r5, [r4, #0x18]
004926e0: pop {r4, r5, r6, pc}

# _ZN10AnimatedFX6SetEndEv 004926e4 size96
004926e4: push {r4, r5, r6, lr}
004926e8: ldr r3, [r0, #0x2c]
004926ec: mov r4, r0
004926f0: cmp r3, #0
004926f4: beq #0x492740
004926f8: bl #0x49267c
004926fc: cmp r0, #0
00492700: beq #0x492740
00492704: mov r0, r4
00492708: bl #0x49267c
0049270c: ldr r3, [r0]
00492710: mov lr, pc
00492714: ldr pc, [r3, #0x44]
00492718: ldr r5, [r0, #0x14]
0049271c: mov r0, r4
00492720: bl #0x49267c
00492724: ldr r3, [r0]
00492728: mov lr, pc
0049272c: ldr pc, [r3, #0x44]
00492730: mov r1, r5
00492734: ldr r3, [r0]
00492738: mov lr, pc
0049273c: ldr pc, [r3, #0xc]
00492740: pop {r4, r5, r6, pc}

# _ZN10AnimatedFX8SetStartEv 00492744 size96
00492744: push {r4, r5, r6, lr}
00492748: ldr r3, [r0, #0x2c]
0049274c: mov r4, r0
00492750: cmp r3, #0
00492754: beq #0x4927a0
00492758: bl #0x49267c
0049275c: cmp r0, #0
00492760: beq #0x4927a0
00492764: mov r0, r4
00492768: bl #0x49267c
0049276c: ldr r3, [r0]
00492770: mov lr, pc
00492774: ldr pc, [r3, #0x44]
00492778: ldr r5, [r0, #0x10]
0049277c: mov r0, r4
00492780: bl #0x49267c
00492784: ldr r3, [r0]
00492788: mov lr, pc
0049278c: ldr pc, [r3, #0x44]
00492790: mov r1, r5
00492794: ldr r3, [r0]
00492798: mov lr, pc
0049279c: ldr pc, [r3, #0xc]
004927a0: pop {r4, r5, r6, pc}

# _ZN10AnimatedFX14_HandleLoopEndEv 004927a4 size256
004927a4: push {r4, r5, r6, lr}
004927a8: mov r1, #0
004927ac: mov r4, r0
004927b0: ldr r0, [r0, #0x20]
004927b4: bl #0x30e9ac
004927b8: cmp r0, #0
004927bc: beq #0x4927cc
004927c0: ldr r3, [r4, #4]
004927c4: cmp r3, #0
004927c8: beq #0x49288c
004927cc: ldr r1, [r4, #0x14]
004927d0: cmp r1, #0
004927d4: blt #0x49288c
004927d8: bne #0x492890
004927dc: mov r0, r4
004927e0: bl #0x492694
004927e4: ldr r3, [r4, #0x50]
004927e8: cmp r3, #0
004927ec: movne r2, #1
004927f0: strbne r2, [r3]
004927f4: ldr r3, [r4, #4]
004927f8: cmp r3, #0
004927fc: beq #0x49288c
00492800: ldr r1, [r4, #0x50]
00492804: mov r0, r4
00492808: blx r3
0049280c: mov r3, #0
00492810: str r3, [r4, #4]
00492814: mov r0, r4
00492818: bl #0x49267c
0049281c: ldr r3, [r0]
00492820: mov lr, pc
00492824: ldr pc, [r3, #0x44]
00492828: ldr r3, [r4, #0x2c]
0049282c: mov r0, r4
00492830: ldr r6, [r3, #8]
00492834: bl #0x49267c
00492838: ldr r3, [r0]
0049283c: mov lr, pc
00492840: ldr pc, [r3, #0x44]
00492844: ldr r5, [r0, #0x14]
00492848: mov r0, r4
0049284c: bl #0x49267c
00492850: ldr r3, [r0]
00492854: mov lr, pc
00492858: ldr pc, [r3, #0x44]
0049285c: ldr r3, [r0, #4]
00492860: cmp r5, r3
00492864: beq #0x492870
00492868: mov r0, r4
0049286c: bl #0x4926e4
00492870: mov r0, r4
00492874: bl #0x49267c
00492878: mov r1, r6
0049287c: mov r2, r5
00492880: ldr r3, [r0]
00492884: mov lr, pc
00492888: ldr pc, [r3, #0x10]
0049288c: pop {r4, r5, r6, pc}
00492890: sub r1, r1, #1
00492894: mov r0, r4
00492898: str r1, [r4, #0x14]
0049289c: pop {r4, r5, r6, lr}
004928a0: b #0x492744

# _ZN10AnimatedFX7_CBLoopEPN6glitch5scene19ITimelineControllerEPv 004928a4 size8
004928a4: mov r0, r1
004928a8: b #0x4927a4

# _ZN10AnimatedFX8SetScaleERK7Point3DIfE 004928ac size28
004928ac: ldr r3, [r0, #0x2c]
004928b0: cmp r3, #0
004928b4: bxeq lr
004928b8: mov r2, #0
004928bc: strb r2, [r0, #0x32]
004928c0: mov r0, r3
004928c4: b #0x4727ac

# _ZN10AnimatedFX13ForceMaterialEi 004928c8 size136
004928c8: ldr r3, [pc, #0x68]
004928cc: ldr r2, [pc, #0x68]
004928d0: str lr, [sp, #-4]!
004928d4: add r3, pc, r3
004928d8: ldr r2, [r3, r2]
004928dc: sub sp, sp, #0xc
004928e0: ldr r2, [r2]
004928e4: cmp r2, #2
004928e8: moveq r3, #0
004928ec: streq r3, [r3]
004928f0: beq #0x4928fc
004928f4: cmp r2, #1
004928f8: beq #0x492904
004928fc: add sp, sp, #0xc
00492900: ldm sp!, {pc}
00492904: ldr r0, [pc, #0x34]
00492908: ldr r1, [pc, #0x34]
0049290c: ldr r2, [pc, #0x34]
00492910: ldr r0, [r3, r0]
00492914: ldr r3, [pc, #0x30]
00492918: mov ip, #0x11c
0049291c: add r1, pc, r1
00492920: add r2, pc, r2
00492924: add r3, pc, r3
00492928: add r0, r0, #0xa8
0049292c: str ip, [sp]
00492930: bl #0x30e004
00492934: b #0x4928fc
00492938: ldrheq r2, [r0], #-0x1c
0049293c: andeq r3, r0, r0, asr #19
00492940: andeq r1, r0, r0, asr #19
00492944: strheq fp, [r2], #-0xac
00492948: subeq fp, r2, r8, asr #24
0049294c: subeq r2, r4, r4, asr #13

# _ZN10AnimatedFXD0Ev 00492950 size28
00492950: push {r4, lr}
00492954: mov r4, r0
00492958: bl #0x49245c
0049295c: mov r0, r4
00492960: bl #0x310440
00492964: mov r0, r4
00492968: pop {r4, pc}

# _ZN10AnimatedFX4LoadEPKcS1_fib 0049296c size308
0049296c: push {r4, r5, r6, r7, r8, sb, sl, lr}
00492970: ldr r5, [pc, #0x11c]
00492974: ldr sl, [pc, #0x11c]
00492978: sub sp, sp, #0x48
0049297c: add r5, pc, r5
00492980: ldr ip, [r5, sl]
00492984: mov r4, r0
00492988: add r6, sp, #0x2c
0049298c: ldr ip, [ip]
00492990: str r3, [r0, #0x20]
00492994: ldr r3, [sp, #0x68]
00492998: mov r8, r2
0049299c: str r1, [r0, #0xc]
004929a0: str r3, [r0, #0x14]
004929a4: add r7, sp, #0x14
004929a8: str r2, [r4, #0x10]
004929ac: mov r0, r6
004929b0: add r2, sp, #0x10
004929b4: str ip, [sp, #0x44]
004929b8: ldrb sb, [sp, #0x6c]
004929bc: bl #0x3140ec
004929c0: mov r1, r8
004929c4: add r2, sp, #0xc
004929c8: mov r0, r7
004929cc: bl #0x3140ec
004929d0: mov r1, #0
004929d4: mov r0, #0xac
004929d8: bl #0x310570
004929dc: mov r3, r7
004929e0: mov r1, #0
004929e4: mov r2, r6
004929e8: mov r8, r0
004929ec: bl #0x472a0c
004929f0: mov r0, r7
004929f4: str r8, [r4, #0x2c]
004929f8: bl #0x3139ac
004929fc: mov r0, r6
00492a00: bl #0x3139ac
00492a04: ldr r3, [r4, #0x2c]
00492a08: cmp r3, #0
00492a0c: beq #0x492a64
00492a10: ldr r3, [r3, #0x38]
00492a14: ldr r2, [pc, #0x80]
00492a18: mov r6, #0
00492a1c: ldr ip, [r3]
00492a20: ldr r1, [r5, r2]
00492a24: mov r0, r3
00492a28: mov r2, r4
00492a2c: mov r3, r6
00492a30: str r6, [sp]
00492a34: mov lr, pc
00492a38: ldr pc, [ip, #0x2c]
00492a3c: ldr r3, [r4, #0x2c]
00492a40: mov r2, r6
00492a44: ldr r1, [r4, #0x20]
00492a48: ldr r3, [r3, #0x38]
00492a4c: mov r0, r3
00492a50: ldr r3, [r3]
00492a54: mov lr, pc
00492a58: ldr pc, [r3, #0x28]
00492a5c: cmp sb, r6
00492a60: bne #0x492a80
00492a64: ldr r3, [r5, sl]
00492a68: ldr r2, [sp, #0x44]
00492a6c: ldr r3, [r3]
00492a70: cmp r2, r3
00492a74: bne #0x492a90
00492a78: add sp, sp, #0x48
00492a7c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00492a80: ldr r3, [r4, #0x2c]
00492a84: ldr r0, [r3, #8]
00492a88: bl #0x50e398
00492a8c: b #0x492a64
00492a90: bl #0x30e310
00492a94: subseq r2, r0, r4, lsl r1
00492a98: andeq r4, r0, ip, lsr #1
00492a9c: andeq r2, r0, ip, lsr sp

# _ZN10AnimatedFX11SyncIrrDataEb 00492aa0 size1004
00492aa0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00492aa4: ldr r3, [r0, #0x2c]
00492aa8: ldr r8, [pc, #0x3d0]
00492aac: sub sp, sp, #0x74
00492ab0: cmp r3, #0
00492ab4: mov r4, r0
00492ab8: add r8, pc, r8
00492abc: beq #0x492c00
00492ac0: ldrb r3, [r0, #0x30]
00492ac4: ldr r7, [r0, #0x34]
00492ac8: ldr r6, [r0, #0x38]
00492acc: cmp r3, #0
00492ad0: ldr r5, [r0, #0x3c]
00492ad4: movne sb, #1
00492ad8: beq #0x492c08
00492adc: ldr r2, [r4, #0x50]
00492ae0: cmp r2, #0
00492ae4: beq #0x492af0
00492ae8: cmp r1, #0
00492aec: bne #0x492c18
00492af0: ldr r0, [r4, #0x28]
00492af4: cmp r0, #0
00492af8: beq #0x492bc8
00492afc: bl #0x3935dc
00492b00: mov sl, r0
00492b04: ldr r1, [sl]
00492b08: mov r0, r7
00492b0c: bl #0x30eba4
00492b10: ldr r1, [sl, #4]
00492b14: mov r7, r0
00492b18: mov r0, r6
00492b1c: bl #0x30eba4
00492b20: ldr r1, [sl, #8]
00492b24: mov r6, r0
00492b28: mov r0, r5
00492b2c: bl #0x30eba4
00492b30: cmp sb, #0
00492b34: mov r5, r0
00492b38: beq #0x492bbc
00492b3c: ldr r3, [r4, #0x28]
00492b40: ldr r2, [r3, #0x2d8]
00492b44: cmp r2, #0
00492b48: beq #0x492e30
00492b4c: ldr r3, [r2, #8]
00492b50: mov r0, r3
00492b54: ldr r3, [r3]
00492b58: mov lr, pc
00492b5c: ldr pc, [r3, #0x38]
00492b60: mov r1, r0
00492b64: add r0, sp, #0x64
00492b68: bl #0x432bbc
00492b6c: movw r1, #0xfa35
00492b70: ldr r0, [sp, #0x68]
00492b74: movt r1, #0x3c8e
00492b78: bl #0x30ed6c
00492b7c: movw r1, #0xfa35
00492b80: mov fp, r0
00492b84: movt r1, #0x3c8e
00492b88: ldr r0, [sp, #0x6c]
00492b8c: bl #0x30ed6c
00492b90: movw r1, #0xfa35
00492b94: mov sl, r0
00492b98: movt r1, #0x3c8e
00492b9c: ldr r0, [sp, #0x64]
00492ba0: bl #0x30ed6c
00492ba4: str fp, [r4, #0x44]
00492ba8: str r0, [r4, #0x40]
00492bac: str sl, [r4, #0x48]
00492bb0: ldr r0, [r4, #0x2c]
00492bb4: add r1, r4, #0x40
00492bb8: bl #0x472874
00492bbc: ldrb r3, [r4, #0x32]
00492bc0: cmp r3, #0
00492bc4: bne #0x492c50
00492bc8: ldrb r3, [r4, #0x4c]
00492bcc: cmp r3, #0
00492bd0: bne #0x492cd4
00492bd4: ldr ip, [r4, #0x28]
00492bd8: cmp ip, #0
00492bdc: beq #0x492cf0
00492be0: cmp sb, #0
00492be4: beq #0x492ce4
00492be8: ldr r0, [r4, #0x2c]
00492bec: add r1, sp, #0x1c
00492bf0: str r7, [sp, #0x1c]
00492bf4: str r6, [sp, #0x20]
00492bf8: str r5, [sp, #0x24]
00492bfc: bl #0x470c24
00492c00: add sp, sp, #0x74
00492c04: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00492c08: cmp r1, #0
00492c0c: moveq sb, r1
00492c10: ldrbne sb, [r0, #0x31]
00492c14: b #0x492adc
00492c18: ldrb r3, [r4, #0x31]
00492c1c: cmp r3, #0
00492c20: beq #0x492af0
00492c24: ldr r3, [r2, #0x2c]
00492c28: cmp r3, #0
00492c2c: beq #0x492c40
00492c30: mov r2, r3
00492c34: ldr r3, [r2, #0x2c]
00492c38: cmp r3, #0
00492c3c: bne #0x492c30
00492c40: ldr r3, [r2, #4]
00492c44: cmp r3, #0
00492c48: movgt sb, #0
00492c4c: b #0x492af0
00492c50: ldr r3, [pc, #0x22c]
00492c54: ldr r2, [r4, #0x28]
00492c58: ldr r3, [r8, r3]
00492c5c: mov r0, r2
00492c60: ldr ip, [r3, #8]
00492c64: ldr r1, [r3]
00492c68: ldr r3, [r3, #4]
00492c6c: str ip, [sp, #0x60]
00492c70: str r1, [sp, #0x58]
00492c74: str r3, [sp, #0x5c]
00492c78: ldr r3, [r2]
00492c7c: mov lr, pc
00492c80: ldr pc, [r3, #0xa4]
00492c84: cmn r0, #1
00492c88: beq #0x492e10
00492c8c: ldr r3, [r4, #0x28]
00492c90: add r0, sp, #0x4c
00492c94: mov r1, r3
00492c98: ldr r3, [r3]
00492c9c: mov lr, pc
00492ca0: ldr pc, [r3, #0xa0]
00492ca4: ldr r3, [sp, #0x4c]
00492ca8: str r3, [sp, #0x58]
00492cac: ldr r3, [sp, #0x50]
00492cb0: str r3, [sp, #0x5c]
00492cb4: ldr r3, [sp, #0x54]
00492cb8: str r3, [sp, #0x60]
00492cbc: ldr r0, [r4, #0x2c]
00492cc0: add r1, sp, #0x58
00492cc4: bl #0x4727ac
00492cc8: ldrb r3, [r4, #0x4c]
00492ccc: cmp r3, #0
00492cd0: beq #0x492bd4
00492cd4: ldr r0, [r4, #0x2c]
00492cd8: add r1, r4, #0x40
00492cdc: bl #0x472874
00492ce0: b #0x492be8
00492ce4: ldrb r3, [r4, #0x31]
00492ce8: cmp r3, #0
00492cec: bne #0x492be8
00492cf0: mov sl, #0
00492cf4: cmp ip, #0
00492cf8: str sl, [sp, #0x40]
00492cfc: str sl, [sp, #0x44]
00492d00: str sl, [sp, #0x48]
00492d04: beq #0x492e4c
00492d08: ldr r0, [ip, #0x1ec]
00492d0c: str r0, [sp, #0x40]
00492d10: ldr sb, [ip, #0x1f0]
00492d14: mov r1, r0
00492d18: str sb, [sp, #0x44]
00492d1c: ldr fp, [ip, #0x1f4]
00492d20: str fp, [sp, #0x48]
00492d24: bl #0x30ed6c
00492d28: mov r1, sb
00492d2c: mov r3, r0
00492d30: mov r0, sb
00492d34: str r3, [sp, #0x14]
00492d38: bl #0x30ed6c
00492d3c: ldr r3, [sp, #0x14]
00492d40: mov r1, r0
00492d44: mov r0, r3
00492d48: bl #0x30eba4
00492d4c: mov r1, fp
00492d50: mov sb, r0
00492d54: mov r0, fp
00492d58: bl #0x30ed6c
00492d5c: mov r1, r0
00492d60: mov r0, sb
00492d64: bl #0x30eba4
00492d68: mov r1, sl
00492d6c: bl #0x30df8c
00492d70: cmp r0, #0
00492d74: beq #0x492be8
00492d78: ldr r3, [pc, #0x108]
00492d7c: mov ip, #0
00492d80: mov r2, ip
00492d84: ldr r0, [r8, r3]
00492d88: add r1, sp, #0x28
00492d8c: add r3, sp, #0x40
00492d90: str ip, [sp]
00492d94: str ip, [sp, #4]
00492d98: str ip, [sp, #8]
00492d9c: str r7, [sp, #0x28]
00492da0: str r6, [sp, #0x2c]
00492da4: str r5, [sp, #0x30]
00492da8: bl #0x525508
00492dac: ldr r0, [sp, #0x40]
00492db0: mov r1, r0
00492db4: bl #0x30ed6c
00492db8: mov r8, r0
00492dbc: ldr r0, [sp, #0x44]
00492dc0: mov r1, r0
00492dc4: bl #0x30ed6c
00492dc8: mov r1, r0
00492dcc: mov r0, r8
00492dd0: bl #0x30eba4
00492dd4: mov r8, r0
00492dd8: ldr r0, [sp, #0x48]
00492ddc: mov r1, r0
00492de0: bl #0x30ed6c
00492de4: mov r1, r0
00492de8: mov r0, r8
00492dec: bl #0x30eba4
00492df0: mov r1, sl
00492df4: bl #0x30df8c
00492df8: cmp r0, #0
00492dfc: movne r3, #0x3f800000
00492e00: strne sl, [sp, #0x44]
00492e04: strne sl, [sp, #0x40]
00492e08: strne r3, [sp, #0x48]
00492e0c: b #0x492be8
00492e10: ldr r3, [r4, #0x28]
00492e14: ldr r2, [r3, #0x120]
00492e18: str r2, [sp, #0x58]
00492e1c: ldr r2, [r3, #0x124]
00492e20: str r2, [sp, #0x5c]
00492e24: ldr r3, [r3, #0x128]
00492e28: str r3, [sp, #0x60]
00492e2c: b #0x492cbc
00492e30: ldr r2, [r3, #0x16c]
00492e34: str r2, [r4, #0x40]
00492e38: ldr r2, [r3, #0x170]
00492e3c: str r2, [r4, #0x44]
00492e40: ldr r3, [r3, #0x174]
00492e44: str r3, [r4, #0x48]
00492e48: b #0x492bb0
00492e4c: ldr r3, [pc, #0x34]
00492e50: mov r2, ip
00492e54: add r1, sp, #0x34
00492e58: ldr r0, [r8, r3]
00492e5c: add r3, sp, #0x40
00492e60: str r7, [sp, #0x34]
00492e64: str r6, [sp, #0x38]
00492e68: str r5, [sp, #0x3c]
00492e6c: str ip, [sp]
00492e70: str ip, [sp, #4]
00492e74: str ip, [sp, #8]
00492e78: bl #0x525508
00492e7c: b #0x492be8
00492e80: ldrsbeq r1, [r0], #-0xf8
00492e84: andeq r3, r0, ip, lsr #30
00492e88: andeq r1, r0, r4, lsl #4

# _ZN10AnimatedFX9SetAnimFXEbbbfiPN15VisualFXManager13AnimFXSetDataEPFvPS_S2_E 00492e8c size100
00492e8c: push {r4, r5, r6, lr}
00492e90: mov r4, r0
00492e94: mov r5, r2
00492e98: strb r1, [r0, #0x30]
00492e9c: mov r1, #0
00492ea0: mov r6, r3
00492ea4: bl #0x492aa0
00492ea8: mov r0, r4
00492eac: mov r1, #1
00492eb0: strb r5, [r4, #0x31]
00492eb4: bl #0x492aa0
00492eb8: mov r0, r4
00492ebc: mov r1, #0
00492ec0: strb r6, [r4, #0x32]
00492ec4: bl #0x492aa0
00492ec8: mov r0, r4
00492ecc: ldr r1, [sp, #0x10]
00492ed0: bl #0x4924b0
00492ed4: ldr r3, [sp, #0x18]
00492ed8: str r3, [r4, #0x50]
00492edc: ldr r3, [sp, #0x14]
00492ee0: str r3, [r4, #0x14]
00492ee4: ldr r3, [sp, #0x1c]
00492ee8: str r3, [r4, #4]
00492eec: pop {r4, r5, r6, pc}

# _ZN10AnimatedFX10SetVisibleEb 00492ef0 size76
00492ef0: push {r4, lr}
00492ef4: ldrb r2, [r0, #0x24]
00492ef8: mov r4, r0
00492efc: cmp r2, r1
00492f00: beq #0x492f38
00492f04: ldr r0, [r0, #0x2c]
00492f08: strb r1, [r4, #0x24]
00492f0c: cmp r0, #0
00492f10: beq #0x492f28
00492f14: bl #0x471368
00492f18: ldr r3, [r4, #0x2c]
00492f1c: ldrb r2, [r4, #0x24]
00492f20: ldr r3, [r3, #8]
00492f24: strb r2, [r3, #0x200]
00492f28: mov r0, r4
00492f2c: mov r1, #0
00492f30: pop {r4, lr}
00492f34: b #0x492aa0
00492f38: pop {r4, pc}

# _ZN10AnimatedFX11SetRotationERK7Point3DIfE 00492f3c size44
00492f3c: mov r2, r1
00492f40: ldr r1, [r1]
00492f44: str r1, [r0, #0x40]
00492f48: ldr ip, [r2, #4]
00492f4c: mov r1, #0
00492f50: str ip, [r0, #0x44]
00492f54: ldr r2, [r2, #8]
00492f58: mov ip, #1
00492f5c: strb ip, [r0, #0x4c]
00492f60: str r2, [r0, #0x48]
00492f64: b #0x492aa0

# _ZN10AnimatedFX6UpdateEv 00492f68 size456
00492f68: push {r4, r5, r6, r7, r8, sl, lr}
00492f6c: ldr r5, [pc, #0x1a4]
00492f70: ldr r6, [pc, #0x1a4]
00492f74: ldr r3, [r0, #0x28]
00492f78: add r5, pc, r5
00492f7c: ldr r2, [r5, r6]
00492f80: sub sp, sp, #0x44
00492f84: cmp r3, #0
00492f88: ldr r2, [r2]
00492f8c: mov r4, r0
00492f90: str r2, [sp, #0x3c]
00492f94: beq #0x492fb8
00492f98: mov r0, r3
00492f9c: ldr r3, [r3]
00492fa0: mov lr, pc
00492fa4: ldr pc, [r3, #0x34]
00492fa8: cmp r0, #0
00492fac: beq #0x4930d4
00492fb0: mov r3, #0
00492fb4: str r3, [r4, #0x28]
00492fb8: ldr r7, [r4, #0x1c]
00492fbc: cmp r7, #0
00492fc0: blt #0x492fe4
00492fc4: ldr r3, [pc, #0x154]
00492fc8: ldr r0, [r5, r3]
00492fcc: bl #0x31f66c
00492fd0: rsb r0, r0, r7
00492fd4: cmp r0, #0
00492fd8: movle r3, #0
00492fdc: str r0, [r4, #0x1c]
00492fe0: strle r3, [r4, #0x14]
00492fe4: ldr r3, [r4, #0x28]
00492fe8: cmp r3, #0
00492fec: beq #0x492ffc
00492ff0: ldrb r1, [r3, #0x84]
00492ff4: cmp r1, #0
00492ff8: beq #0x4930c8
00492ffc: ldrb r3, [r4, #0x24]
00493000: cmp r3, #0
00493004: beq #0x493088
00493008: ldr r3, [r4, #0x50]
0049300c: cmp r3, #0
00493010: beq #0x4930ec
00493014: ldr r8, [pc, #0x108]
00493018: add r7, sp, #0x24
0049301c: ldr sl, [r5, r8]
00493020: mov r0, sl
00493024: bl #0x337888
00493028: ldr r1, [pc, #0xf8]
0049302c: add r2, sp, #8
00493030: mov r0, r7
00493034: add r1, pc, r1
00493038: bl #0x3140ec
0049303c: mov r0, sl
00493040: mov r1, r7
00493044: bl #0x337a88
00493048: mov r0, r7
0049304c: bl #0x3139ac
00493050: ldr r8, [r5, r8]
00493054: add r7, sp, #0xc
00493058: mov r0, r8
0049305c: bl #0x337888
00493060: ldr r1, [pc, #0xc4]
00493064: add r2, sp, #4
00493068: mov r0, r7
0049306c: add r1, pc, r1
00493070: bl #0x3140ec
00493074: mov r0, r8
00493078: mov r1, r7
0049307c: bl #0x337a88
00493080: mov r0, r7
00493084: bl #0x3139ac
00493088: mov r0, r4
0049308c: ldr r1, [r4, #0x20]
00493090: bl #0x4924b0
00493094: ldrb r7, [r4, #0x18]
00493098: cmp r7, #0
0049309c: bne #0x4930ac
004930a0: ldrb r3, [r4, #0x24]
004930a4: cmp r3, #0
004930a8: bne #0x4930f4
004930ac: ldr r3, [r5, r6]
004930b0: ldr r2, [sp, #0x3c]
004930b4: ldr r3, [r3]
004930b8: cmp r2, r3
004930bc: bne #0x493114
004930c0: add sp, sp, #0x44
004930c4: pop {r4, r5, r6, r7, r8, sl, pc}
004930c8: mov r0, r4
004930cc: bl #0x492aa0
004930d0: b #0x492ffc
004930d4: ldr r3, [r4, #0x28]
004930d8: ldrb r3, [r3, #0x81]
004930dc: cmp r3, #0
004930e0: movne r3, #0
004930e4: strne r3, [r4, #0x28]
004930e8: b #0x492fb8
004930ec: ldr r8, [pc, #0x30]
004930f0: b #0x493050
004930f4: mov r0, r4
004930f8: bl #0x4924e0
004930fc: cmp r0, #0
00493100: beq #0x4930ac
00493104: mov r0, r4
00493108: mov r1, r7
0049310c: bl #0x492ef0
00493110: b #0x4930ac
00493114: bl #0x30e310
00493118: subseq r1, r0, r8, lsl fp
0049311c: andeq r4, r0, ip, lsr #1
00493120: strdeq r3, r4, [r0], -r4
00493124: andeq r0, r0, r4, lsl #17
00493128: strdeq r1, r2, [r4], #-0xfc
0049312c: subeq r1, r4, r4, asr #31

# _GLOBAL__I_.._.._sources_Game_VisualFX_AnimatedFX.cpp 00493130 size304
00493130: push {r4, r5, r6, lr}
00493134: ldr r4, [pc, #0xf4]
00493138: ldr r2, [pc, #0xf4]
0049313c: ldr r3, [pc, #0xf4]
00493140: add r4, pc, r4
00493144: ldr r1, [r4, r2]
00493148: add r3, pc, r3
0049314c: mov r2, #0x3f000000
00493150: ldr r0, [r1]
00493154: str r2, [r3, #8]
00493158: str r2, [r3]
0049315c: tst r0, #1
00493160: str r2, [r3, #4]
00493164: beq #0x4931fc
00493168: ldr r3, [pc, #0xcc]
0049316c: ldr r3, [r4, r3]
00493170: ldr r2, [r3]
00493174: tst r2, #1
00493178: beq #0x4931c8
0049317c: ldr r3, [pc, #0xbc]
00493180: ldr r3, [r4, r3]
00493184: ldr r2, [r3]
00493188: tst r2, #1
0049318c: beq #0x493194
00493190: pop {r4, r5, r6, pc}
00493194: mov r2, #1
00493198: str r2, [r3]
0049319c: ldr r3, [pc, #0xa0]
004931a0: ldr r5, [r4, r3]
004931a4: mov r0, r5
004931a8: bl #0x522e2c
004931ac: ldr r3, [pc, #0x94]
004931b0: mov r0, r5
004931b4: ldr r1, [r4, r3]
004931b8: ldr r3, [pc, #0x8c]
004931bc: ldr r2, [r4, r3]
004931c0: pop {r4, r5, r6, lr}
004931c4: b #0x30e304
004931c8: mov r2, #1
004931cc: str r2, [r3]
004931d0: ldr r3, [pc, #0x78]
004931d4: ldr r5, [r4, r3]
004931d8: mov r0, r5
004931dc: bl #0x32d79c
004931e0: ldr r3, [pc, #0x6c]
004931e4: mov r0, r5
004931e8: ldr r1, [r4, r3]
004931ec: ldr r3, [pc, #0x58]
004931f0: ldr r2, [r4, r3]
004931f4: bl #0x30e304
004931f8: b #0x49317c
004931fc: mov r3, #1
00493200: str r3, [r1]
00493204: ldr r3, [pc, #0x4c]
00493208: ldr r5, [r4, r3]
0049320c: mov r0, r5
00493210: bl #0x3790a8
00493214: ldr r3, [pc, #0x40]
00493218: mov r0, r5
0049321c: ldr r1, [r4, r3]
00493220: ldr r3, [pc, #0x24]
00493224: ldr r2, [r4, r3]
00493228: bl #0x30e304
0049322c: b #0x493168
00493230: subseq r1, r0, r0, asr sb
00493234: strdeq r0, r1, [r0], -r4
00493238: subseq r3, r1, r0, ror #3
0049323c: andeq r0, r0, ip, lsr #31
00493240: andeq r0, r0, r0, ror r6
00493244: andeq r1, r0, r4, lsl #4
00493248: andeq r1, r0, r0, lsr #4
0049324c: muleq r0, r0, r8
00493250: strdeq r3, r4, [r0], -r4
00493254: andeq r0, r0, r0, asr #17
00493258: andeq r2, r0, r4, lsl r7
0049325c: muleq r0, ip, r5

# _ZN15VisualFXManagerC2Ev 00493260 size100
00493260: ldr r1, [pc, #0x54]
00493264: ldr ip, [pc, #0x54]
00493268: mov r2, #0
0049326c: add r1, pc, r1
00493270: ldr ip, [r1, ip]
00493274: str r4, [sp, #-4]!
00493278: add r4, r0, #8
0049327c: add ip, ip, #8
00493280: str r2, [r0, #0x30]
00493284: str ip, [r0]
00493288: str r4, [r0, #0xc]
0049328c: strb r2, [r0, #4]
00493290: str r4, [r0, #8]
00493294: str r2, [r0, #0x10]
00493298: str r2, [r0, #0x14]
0049329c: str r2, [r0, #0x18]
004932a0: str r2, [r0, #0x1c]
004932a4: str r2, [r0, #0x20]
004932a8: str r2, [r0, #0x24]
004932ac: str r2, [r0, #0x28]
004932b0: str r2, [r0, #0x2c]
004932b4: ldm sp!, {r4}
004932b8: bx lr
004932bc: subseq r1, r0, r4, lsr #16
004932c0: andeq r3, r0, r0, lsr r4

# _ZN15VisualFXManagerC1Ev 004932c4 size100
004932c4: ldr r1, [pc, #0x54]
004932c8: ldr ip, [pc, #0x54]
004932cc: mov r2, #0
004932d0: add r1, pc, r1
004932d4: ldr ip, [r1, ip]
004932d8: str r4, [sp, #-4]!
004932dc: add r4, r0, #8
004932e0: add ip, ip, #8
004932e4: str r2, [r0, #0x30]
004932e8: str ip, [r0]
004932ec: str r4, [r0, #0xc]
004932f0: strb r2, [r0, #4]
004932f4: str r4, [r0, #8]
004932f8: str r2, [r0, #0x10]
004932fc: str r2, [r0, #0x14]
00493300: str r2, [r0, #0x18]
00493304: str r2, [r0, #0x1c]
00493308: str r2, [r0, #0x20]
0049330c: str r2, [r0, #0x24]
00493310: str r2, [r0, #0x28]
00493314: str r2, [r0, #0x2c]
00493318: ldm sp!, {r4}
0049331c: bx lr
00493320: subseq r1, r0, r0, asr #15
00493324: andeq r3, r0, r0, lsr r4

# _ZNK15VisualFXManager22DBG_GetAnimatedFXStatsERjS0_ 00493328 size176
00493328: mov r3, #0
0049332c: str r3, [r2]
00493330: str r3, [r1]
00493334: push {r4, r5, r6, r7, r8}
00493338: ldr ip, [r0, #0x28]
0049333c: ldr r4, [r0, #0x2c]
00493340: rsb r4, ip, r4
00493344: asr r4, r4, #3
00493348: add r7, r4, r4, lsl #2
0049334c: add r7, r7, r7, lsl #4
00493350: add r7, r7, r7, lsl #8
00493354: add r7, r7, r7, lsl #16
00493358: adds r7, r4, r7, lsl #1
0049335c: beq #0x4933d0
00493360: mov r5, r3
00493364: mov r6, r3
00493368: add ip, ip, r3
0049336c: ldmib ip, {r4, ip}
00493370: rsb ip, r4, ip
00493374: add ip, r5, ip, asr #2
00493378: str ip, [r1]
0049337c: ldr r5, [r0, #0x28]
00493380: ldr r8, [r2]
00493384: add r5, r5, r3
00493388: ldr ip, [r5, #0x10]!
0049338c: cmp ip, r5
00493390: moveq r4, #0
00493394: beq #0x4933ac
00493398: mov r4, #0
0049339c: ldr ip, [ip]
004933a0: add r4, r4, #1
004933a4: cmp r5, ip
004933a8: bne #0x49339c
004933ac: add r6, r6, #1
004933b0: add r4, r4, r8
004933b4: cmp r6, r7
004933b8: str r4, [r2]
004933bc: add r3, r3, #0x18
004933c0: beq #0x4933d0
004933c4: ldr r5, [r1]
004933c8: ldr ip, [r0, #0x28]
004933cc: b #0x493368
004933d0: pop {r4, r5, r6, r7, r8}
004933d4: bx lr

# _ZN15VisualFXManager17_PreCacheAnimDictEv 004933d8 size12
004933d8: mov r3, #1
004933dc: strb r3, [r0, #4]
004933e0: bx lr

# _ZN15VisualFXManager13GetAnimFXDataENS_13AnimFXSetInfoEPNS_13AnimFXSetDataE 004933e4 size164
004933e4: str r4, [sp, #-4]!
004933e8: ldr r1, [r2]
004933ec: ldr ip, [r3, #4]
004933f0: mov r4, #0x30
004933f4: ldr r1, [r1, #0x10]
004933f8: mla r1, r4, ip, r1
004933fc: ldrb ip, [r1, #0x11]
00493400: strb ip, [r0]
00493404: ldrb ip, [r1, #0x10]
00493408: strb ip, [r0, #1]
0049340c: ldrb ip, [r1, #0x20]
00493410: strb ip, [r0, #2]
00493414: ldr ip, [r1, #0x24]
00493418: str r3, [r0, #0x10]
0049341c: str ip, [r0, #4]
00493420: ldr ip, [r1, #0x14]
00493424: str ip, [r0, #0xc]
00493428: ldr r2, [r2]
0049342c: ldr r2, [r2, #0x14]
00493430: cmp r2, #1
00493434: beq #0x49347c
00493438: ldr r2, [r1, #0xc]
0049343c: cmn r2, #1
00493440: beq #0x493470
00493444: ldr r3, [r3, #0xc]
00493448: cmn r3, #1
0049344c: beq #0x493470
00493450: cmp r2, #0
00493454: streq r3, [r0, #8]
00493458: beq #0x493468
0049345c: cmp r3, #0
00493460: mulne r2, r2, r3
00493464: str r2, [r0, #8]
00493468: ldm sp!, {r4}
0049346c: bx lr
00493470: mvn r3, #0
00493474: str r3, [r0, #8]
00493478: b #0x493468
0049347c: ldr r3, [r1, #0xc]
00493480: str r3, [r0, #8]
00493484: b #0x493468

# _ZNSt4priv6__findIPP10AnimatedFXS2_EET_S4_S4_RKT0_RKSt26random_access_iterator_tag 00493488 size304
00493488: mov r3, r0
0049348c: rsb r0, r0, r1
00493490: asr ip, r0, #4
00493494: cmp ip, #0
00493498: push {r4, r5}
0049349c: asr r4, r0, #2
004934a0: movle r0, r3
004934a4: ble #0x493530
004934a8: ldr r0, [r3]
004934ac: ldr r4, [r2]
004934b0: cmp r0, r4
004934b4: moveq r0, r3
004934b8: beq #0x49354c
004934bc: ldr r5, [r3, #4]
004934c0: add r0, r3, #4
004934c4: cmp r4, r5
004934c8: beq #0x49354c
004934cc: ldr r5, [r0, #4]!
004934d0: cmp r4, r5
004934d4: beq #0x49354c
004934d8: ldr r5, [r0, #4]!
004934dc: cmp r4, r5
004934e0: bne #0x49351c
004934e4: b #0x49354c
004934e8: ldr r0, [r3, #0x10]
004934ec: cmp r0, r4
004934f0: beq #0x493580
004934f4: ldr r0, [r3, #0x14]
004934f8: cmp r0, r4
004934fc: beq #0x493588
00493500: ldr r0, [r3, #0x18]
00493504: cmp r0, r4
00493508: beq #0x493590
0049350c: add r3, r3, #0x10
00493510: ldr r0, [r3, #0xc]
00493514: cmp r4, r0
00493518: beq #0x493598
0049351c: subs ip, ip, #1
00493520: bne #0x4934e8
00493524: add r0, r3, #0x10
00493528: rsb r4, r0, r1
0049352c: asr r4, r4, #2
00493530: cmp r4, #2
00493534: beq #0x493554
00493538: cmp r4, #3
0049353c: beq #0x4935a0
00493540: cmp r4, #1
00493544: beq #0x493578
00493548: mov r0, r1
0049354c: pop {r4, r5}
00493550: bx lr
00493554: ldr r3, [r2]
00493558: ldr r2, [r0]
0049355c: cmp r2, r3
00493560: beq #0x49354c
00493564: add r0, r0, #4
00493568: ldr r2, [r0]
0049356c: cmp r2, r3
00493570: movne r0, r1
00493574: b #0x49354c
00493578: ldr r3, [r2]
0049357c: b #0x493568
00493580: add r0, r3, #0x10
00493584: b #0x49354c
00493588: add r0, r3, #0x14
0049358c: b #0x49354c
00493590: add r0, r3, #0x18
00493594: b #0x49354c
00493598: add r0, r3, #0xc
0049359c: b #0x49354c
004935a0: ldr r3, [r2]
004935a4: ldr r2, [r0]
004935a8: cmp r2, r3
004935ac: beq #0x49354c
004935b0: add r0, r0, #4
004935b4: b #0x493558

# _ZN15VisualFXManager16GetAnimFXSetDataEiiiPK10GameObjectPNS_13AnimFXSetDataE7Point3DIfES6_ 004935b8 size152
004935b8: push {r4, r5, r6, r7, r8, lr}
004935bc: mov r0, #0x30
004935c0: mov r4, r1
004935c4: mov r1, #0
004935c8: ldr r7, [sp, #0x20]
004935cc: ldr r6, [sp, #0x24]
004935d0: mov r5, r2
004935d4: mov r8, r3
004935d8: bl #0x310570
004935dc: mov r2, #0
004935e0: mov r1, #0
004935e4: str r2, [r0, #0x24]
004935e8: strb r1, [r0]
004935ec: str r8, [r0, #4]
004935f0: str r4, [r0, #8]
004935f4: str r5, [r0, #0xc]
004935f8: ldr r1, [sp, #0x18]
004935fc: str r2, [r0, #0x10]
00493600: str r2, [r0, #0x14]
00493604: str r2, [r0, #0x18]
00493608: str r2, [r0, #0x1c]
0049360c: str r2, [r0, #0x20]
00493610: str r1, [r0, #0x28]
00493614: ldr r2, [r7]
00493618: str r2, [r0, #0x10]
0049361c: ldr r2, [r7, #4]
00493620: str r2, [r0, #0x14]
00493624: ldr r2, [r7, #8]
00493628: str r2, [r0, #0x18]
0049362c: ldr r2, [r6]
00493630: str r2, [r0, #0x1c]
00493634: ldr r2, [r6, #4]
00493638: str r2, [r0, #0x20]
0049363c: ldr r2, [r6, #8]
00493640: ldr r1, [sp, #0x1c]
00493644: str r2, [r0, #0x24]
00493648: str r1, [r0, #0x2c]
0049364c: pop {r4, r5, r6, r7, r8, pc}

# _GLOBAL__I_.._.._sources_Game_VisualFX_VisualFXManager.cpp 00493650 size304
00493650: push {r4, r5, r6, lr}
00493654: ldr r4, [pc, #0xf4]
00493658: ldr r2, [pc, #0xf4]
0049365c: ldr r3, [pc, #0xf4]
00493660: add r4, pc, r4
00493664: ldr r1, [r4, r2]
00493668: add r3, pc, r3
0049366c: mov r2, #0x3f000000
00493670: ldr r0, [r1]
00493674: str r2, [r3, #8]
00493678: str r2, [r3]
0049367c: tst r0, #1
00493680: str r2, [r3, #4]
00493684: beq #0x49371c
00493688: ldr r3, [pc, #0xcc]
0049368c: ldr r3, [r4, r3]
00493690: ldr r2, [r3]
00493694: tst r2, #1
00493698: beq #0x4936e8
0049369c: ldr r3, [pc, #0xbc]
004936a0: ldr r3, [r4, r3]
004936a4: ldr r2, [r3]
004936a8: tst r2, #1
004936ac: beq #0x4936b4
004936b0: pop {r4, r5, r6, pc}
004936b4: mov r2, #1
004936b8: str r2, [r3]
004936bc: ldr r3, [pc, #0xa0]
004936c0: ldr r5, [r4, r3]
004936c4: mov r0, r5
004936c8: bl #0x4932c4
004936cc: ldr r3, [pc, #0x94]
004936d0: mov r0, r5
004936d4: ldr r1, [r4, r3]
004936d8: ldr r3, [pc, #0x8c]
004936dc: ldr r2, [r4, r3]
004936e0: pop {r4, r5, r6, lr}
004936e4: b #0x30e304
004936e8: mov r2, #1
004936ec: str r2, [r3]
004936f0: ldr r3, [pc, #0x78]
004936f4: ldr r5, [r4, r3]
004936f8: mov r0, r5
004936fc: bl #0x32d79c
00493700: ldr r3, [pc, #0x6c]
00493704: mov r0, r5
00493708: ldr r1, [r4, r3]
0049370c: ldr r3, [pc, #0x58]
00493710: ldr r2, [r4, r3]
00493714: bl #0x30e304
00493718: b #0x49369c
0049371c: mov r3, #1
00493720: str r3, [r1]
00493724: ldr r3, [pc, #0x4c]
00493728: ldr r5, [r4, r3]
0049372c: mov r0, r5
00493730: bl #0x3790a8
00493734: ldr r3, [pc, #0x40]
00493738: mov r0, r5
0049373c: ldr r1, [r4, r3]
00493740: ldr r3, [pc, #0x24]
00493744: ldr r2, [r4, r3]
00493748: bl #0x30e304
0049374c: b #0x493688
00493750: subseq r1, r0, r0, lsr r4
00493754: strdeq r0, r1, [r0], -r4
00493758: subseq r2, r1, ip, asr #25
0049375c: andeq r0, r0, ip, lsr #31
00493760: strheq r4, [r0], -r4
00493764: andeq r1, r0, r8, lsl #22
00493768: andeq r4, r0, r0, ror r1
0049376c: muleq r0, r0, r8
00493770: strdeq r3, r4, [r0], -r4
00493774: andeq r0, r0, r0, asr #17
00493778: andeq r2, r0, r4, lsl r7
0049377c: muleq r0, ip, r5

# _ZN6Random9GetRandomEib.clone.1 00493780 size148
00493780: push {r4, lr}
00493784: ldr r4, [pc, #0x7c]
00493788: cmp r0, #0
0049378c: add r4, pc, r4
00493790: beq #0x4937f0
00493794: ldr r2, [pc, #0x70]
00493798: mov r1, r0
0049379c: movw r0, #0xe6ab
004937a0: ldr r2, [r4, r2]
004937a4: movw r3, #0xdb17
004937a8: movt r3, #0x2b52
004937ac: ldr lr, [r2]
004937b0: movw ip, #0xf26b
004937b4: movt ip, #0xda
004937b8: mul r0, r0, lr
004937bc: add r0, r0, #0x2b000
004937c0: add r0, r0, #0x3fc
004937c4: add r0, r0, #1
004937c8: umull lr, r3, r3, r0
004937cc: rsb lr, r3, r0
004937d0: add r3, r3, lr, lsr #1
004937d4: lsr r3, r3, #0x17
004937d8: mls r3, ip, r3, r0
004937dc: mov r0, r3
004937e0: str r3, [r2]
004937e4: bl #0x30eb2c
004937e8: eor r0, r1, r1, asr #31
004937ec: sub r0, r0, r1, asr #31
004937f0: ldr r3, [pc, #0x18]
004937f4: ldr r3, [r4, r3]
004937f8: ldr r2, [r3]
004937fc: add r2, r2, #1
00493800: str r2, [r3]
00493804: pop {r4, pc}
00493808: subseq r1, r0, r4, lsl #6
0049380c: muleq r0, r4, ip
00493810: andeq r1, r0, r8, lsl #1

# _ZNSaINSt4priv10_List_nodeIPN15VisualFXManager13AnimFXSetDataEEEE8allocateEjPKv.clone.13 00493814 size32
00493814: str lr, [sp, #-4]!
00493818: sub sp, sp, #0xc
0049381c: add r0, sp, #8
00493820: mov r3, #0xc
00493824: str r3, [r0, #-4]!
00493828: bl #0x708ec0
0049382c: add sp, sp, #0xc
00493830: ldm sp!, {pc}

# _ZNSaIPN15VisualFXManager10AnimFXStepEE11_M_allocateEjRj 00493834 size112
00493834: push {r4, lr}
00493838: cmn r1, #0xc0000001
0049383c: sub sp, sp, #8
00493840: mov r4, r2
00493844: bhi #0x49388c
00493848: cmp r1, #0
0049384c: moveq r0, r1
00493850: bne #0x49385c
00493854: add sp, sp, #8
00493858: pop {r4, pc}
0049385c: lsl r0, r1, #2
00493860: cmp r0, #0x80
00493864: str r0, [sp, #4]
00493868: bhi #0x493884
0049386c: add r0, sp, #4
00493870: bl #0x708ec0
00493874: ldr r3, [sp, #4]
00493878: lsr r3, r3, #2
0049387c: str r3, [r4]
00493880: b #0x493854
00493884: bl #0x310454
00493888: b #0x493874
0049388c: ldr r0, [pc, #0xc]
00493890: add r0, pc, r0
00493894: bl #0x30e0c4
00493898: mov r0, #1
0049389c: bl #0x30de48
004938a0: subeq sl, r2, r0, ror #23

# _ZNSt6vectorIPN15VisualFXManager10AnimFXStepESaIS2_EEC1ERKS4_ 004938a4 size128
004938a4: push {r4, r5, lr}
004938a8: mov r5, r1
004938ac: ldr r3, [r5]
004938b0: ldr r1, [r1, #4]
004938b4: sub sp, sp, #0xc
004938b8: mov r4, r0
004938bc: rsb r1, r3, r1
004938c0: mov ip, #0
004938c4: asr r1, r1, #2
004938c8: add r2, sp, #8
004938cc: str r1, [r2, #-4]!
004938d0: str ip, [r4]
004938d4: str ip, [r4, #4]
004938d8: str ip, [r0, #8]!
004938dc: bl #0x493834
004938e0: ldr r2, [sp, #4]
004938e4: str r0, [r4]
004938e8: str r0, [r4, #4]
004938ec: add r2, r0, r2, lsl #2
004938f0: str r2, [r4, #8]
004938f4: ldm r5, {r1, r2}
004938f8: mov r3, r0
004938fc: cmp r1, r2
00493900: beq #0x493914
00493904: rsb r5, r1, r2
00493908: mov r2, r5
0049390c: bl #0x30e868
00493910: add r3, r0, r5
00493914: str r3, [r4, #4]
00493918: mov r0, r4
0049391c: add sp, sp, #0xc
00493920: pop {r4, r5, pc}

# _ZN15VisualFXManager13AnimFXSetInfoC1ERKS0_ 00493924 size104
00493924: push {r4, r5, r6, r7, r8, lr}
00493928: mov r6, r1
0049392c: ldr r3, [r1], #4
00493930: mov r5, r0
00493934: add r4, r5, #0x10
00493938: str r3, [r0], #4
0049393c: bl #0x4938a4
00493940: str r4, [r5, #0x10]
00493944: str r4, [r5, #0x14]
00493948: ldr r7, [r6, #0x10]!
0049394c: cmp r7, r6
00493950: beq #0x493984
00493954: mov r0, r4
00493958: bl #0x493814
0049395c: ldr r3, [r7, #8]
00493960: str r3, [r0, #8]
00493964: ldr r3, [r4, #4]
00493968: str r4, [r0]
0049396c: str r3, [r0, #4]
00493970: str r0, [r3]
00493974: str r0, [r4, #4]
00493978: ldr r7, [r7]
0049397c: cmp r6, r7
00493980: bne #0x493954
00493984: mov r0, r5
00493988: pop {r4, r5, r6, r7, r8, pc}

# _ZNSt6vectorIPN15VisualFXManager10AnimFXStepESaIS2_EE20_M_allocate_and_copyIPKS2_EEPS2_RjT_SA_ 0049398c size60
0049398c: push {r4, r5, r6, lr}
00493990: add r0, r0, #8
00493994: mov r4, r2
00493998: mov r2, r1
0049399c: ldr r1, [r1]
004939a0: mov r5, r3
004939a4: bl #0x493834
004939a8: cmp r4, r5
004939ac: mov r6, r0
004939b0: beq #0x4939c0
004939b4: mov r1, r4
004939b8: rsb r2, r4, r5
004939bc: bl #0x30e868
004939c0: mov r0, r6
004939c4: pop {r4, r5, r6, pc}

# _ZNSaIP10AnimatedFXE11_M_allocateEjRj 004939c8 size112
004939c8: push {r4, lr}
004939cc: cmn r1, #0xc0000001
004939d0: sub sp, sp, #8
004939d4: mov r4, r2
004939d8: bhi #0x493a20
004939dc: cmp r1, #0
004939e0: moveq r0, r1
004939e4: bne #0x4939f0
004939e8: add sp, sp, #8
004939ec: pop {r4, pc}
004939f0: lsl r0, r1, #2
004939f4: cmp r0, #0x80
004939f8: str r0, [sp, #4]
004939fc: bhi #0x493a18
00493a00: add r0, sp, #4
00493a04: bl #0x708ec0
00493a08: ldr r3, [sp, #4]
00493a0c: lsr r3, r3, #2
00493a10: str r3, [r4]
00493a14: b #0x4939e8
00493a18: bl #0x310454
00493a1c: b #0x493a08
00493a20: ldr r0, [pc, #0xc]
00493a24: add r0, pc, r0
00493a28: bl #0x30e0c4
00493a2c: mov r0, #1
00493a30: bl #0x30de48
00493a34: subeq sl, r2, ip, asr #20

# _ZNSt6vectorIP10AnimatedFXSaIS1_EE20_M_allocate_and_copyIPKS1_EEPS1_RjT_S9_ 00493a38 size60
00493a38: push {r4, r5, r6, lr}
00493a3c: add r0, r0, #8
00493a40: mov r4, r2
00493a44: mov r2, r1
00493a48: ldr r1, [r1]
00493a4c: mov r5, r3
00493a50: bl #0x4939c8
00493a54: cmp r4, r5
00493a58: mov r6, r0
00493a5c: beq #0x493a6c
00493a60: mov r1, r4
00493a64: rsb r2, r4, r5
00493a68: bl #0x30e868
00493a6c: mov r0, r6
00493a70: pop {r4, r5, r6, pc}

# _ZNSt6vectorIP10AnimatedFXSaIS1_EEC2ERKS3_ 00493a74 size128
00493a74: push {r4, r5, lr}
00493a78: mov r5, r1
00493a7c: ldr r3, [r5]
00493a80: ldr r1, [r1, #4]
00493a84: sub sp, sp, #0xc
00493a88: mov r4, r0
00493a8c: rsb r1, r3, r1
00493a90: mov ip, #0
00493a94: asr r1, r1, #2
00493a98: add r2, sp, #8
00493a9c: str r1, [r2, #-4]!
00493aa0: str ip, [r4]
00493aa4: str ip, [r4, #4]
00493aa8: str ip, [r0, #8]!
00493aac: bl #0x4939c8
00493ab0: ldr r2, [sp, #4]
00493ab4: str r0, [r4]
00493ab8: str r0, [r4, #4]
00493abc: add r2, r0, r2, lsl #2
00493ac0: str r2, [r4, #8]
00493ac4: ldm r5, {r1, r2}
00493ac8: mov r3, r0
00493acc: cmp r1, r2
00493ad0: beq #0x493ae4
00493ad4: rsb r5, r1, r2
00493ad8: mov r2, r5
00493adc: bl #0x30e868
00493ae0: add r3, r0, r5
00493ae4: str r3, [r4, #4]
00493ae8: mov r0, r4
00493aec: add sp, sp, #0xc
00493af0: pop {r4, r5, pc}

# _ZNSaIN15VisualFXManager13AnimFXSetInfoEE11_M_allocateEjRj 00493af4 size136
00493af4: push {r4, lr}
00493af8: movw r3, #0xaaaa
00493afc: orr r3, r3, r3, lsl #12
00493b00: cmp r1, r3
00493b04: sub sp, sp, #8
00493b08: mov r4, r2
00493b0c: bhi #0x493b64
00493b10: cmp r1, #0
00493b14: moveq r0, r1
00493b18: bne #0x493b24
00493b1c: add sp, sp, #8
00493b20: pop {r4, pc}
00493b24: mov r0, #0x18
00493b28: mul r0, r0, r1
00493b2c: cmp r0, #0x80
00493b30: str r0, [sp, #4]
00493b34: bhi #0x493b5c
00493b38: add r0, sp, #4
00493b3c: bl #0x708ec0
00493b40: ldr r2, [sp, #4]
00493b44: movw r3, #0xaaab
00493b48: movt r3, #0xaaaa
00493b4c: umull r1, r3, r3, r2
00493b50: lsr r3, r3, #4
00493b54: str r3, [r4]
00493b58: b #0x493b1c
00493b5c: bl #0x310454
00493b60: b #0x493b40
00493b64: ldr r0, [pc, #0xc]
00493b68: add r0, pc, r0
00493b6c: bl #0x30e0c4
00493b70: mov r0, #1
00493b74: bl #0x30de48
00493b78: subeq sl, r2, r8, lsl #18

# _ZNSaIN15VisualFXManager14AnimatedFXInfoEE11_M_allocateEjRj 00493b7c size136
00493b7c: push {r4, lr}
00493b80: movw r3, #0xaaaa
00493b84: orr r3, r3, r3, lsl #12
00493b88: cmp r1, r3
00493b8c: sub sp, sp, #8
00493b90: mov r4, r2
00493b94: bhi #0x493bec
00493b98: cmp r1, #0
00493b9c: moveq r0, r1
00493ba0: bne #0x493bac
00493ba4: add sp, sp, #8
00493ba8: pop {r4, pc}
00493bac: mov r0, #0x18
00493bb0: mul r0, r0, r1
00493bb4: cmp r0, #0x80
00493bb8: str r0, [sp, #4]
00493bbc: bhi #0x493be4
00493bc0: add r0, sp, #4
00493bc4: bl #0x708ec0
00493bc8: ldr r2, [sp, #4]
00493bcc: movw r3, #0xaaab
00493bd0: movt r3, #0xaaaa
00493bd4: umull r1, r3, r3, r2
00493bd8: lsr r3, r3, #4
00493bdc: str r3, [r4]
00493be0: b #0x493ba4
00493be4: bl #0x310454
00493be8: b #0x493bc8
00493bec: ldr r0, [pc, #0xc]
00493bf0: add r0, pc, r0
00493bf4: bl #0x30e0c4
00493bf8: mov r0, #1
00493bfc: bl #0x30de48
00493c00: subeq sl, r2, r0, lsl #17

# _ZNSt4priv10_List_baseIP10AnimatedFXSaIS2_EE5clearEv 00493c04 size64
00493c04: push {r4, r5, r6, lr}
00493c08: mov r5, r0
00493c0c: ldr r0, [r0]
00493c10: cmp r0, r5
00493c14: bne #0x493c20
00493c18: b #0x493c38
00493c1c: mov r0, r4
00493c20: ldr r4, [r0]
00493c24: mov r1, #0xc
00493c28: bl #0x708f00
00493c2c: cmp r4, r5
00493c30: bne #0x493c1c
00493c34: mov r0, r5
00493c38: str r0, [r5, #4]
00493c3c: str r0, [r5]
00493c40: pop {r4, r5, r6, pc}

# _ZNSt4listIPN15VisualFXManager13AnimFXSetDataESaIS2_EE25_M_splice_insert_dispatchINSt4priv14_List_iteratorIS2_St13_Const_traitsIS2_EEEEEvNS7_IS2_St16_Nonconst_traitsIS2_EEET_SE_RKSt12__false_type 00493c44 size212
00493c44: push {r4, r5, r6, r7, lr}
00493c48: cmp r3, r2
00493c4c: sub sp, sp, #0xc
00493c50: mov r5, r3
00493c54: mov r4, sp
00493c58: mov r7, r1
00493c5c: str sp, [sp]
00493c60: str sp, [sp, #4]
00493c64: moveq r0, sp
00493c68: beq #0x493ca4
00493c6c: mov r6, r2
00493c70: mov r0, r4
00493c74: bl #0x493814
00493c78: ldr r3, [r6, #8]
00493c7c: str r3, [r0, #8]
00493c80: ldr r3, [sp, #4]
00493c84: str r4, [r0]
00493c88: str r3, [r0, #4]
00493c8c: str r0, [r3]
00493c90: str r0, [sp, #4]
00493c94: ldr r6, [r6]
00493c98: cmp r6, r5
00493c9c: bne #0x493c70
00493ca0: ldr r0, [sp]
00493ca4: cmp r0, r4
00493ca8: ldr r3, [r7]
00493cac: beq #0x493d10
00493cb0: cmp r3, r4
00493cb4: beq #0x493cfc
00493cb8: ldr r2, [sp, #4]
00493cbc: str r3, [r2]
00493cc0: ldr r2, [r0, #4]
00493cc4: str r4, [r2]
00493cc8: ldr r2, [r3, #4]
00493ccc: str r0, [r2]
00493cd0: ldr r1, [sp, #4]
00493cd4: ldr r2, [r3, #4]
00493cd8: str r1, [r3, #4]
00493cdc: ldr r3, [r0, #4]
00493ce0: str r3, [sp, #4]
00493ce4: str r2, [r0, #4]
00493ce8: ldr r0, [sp]
00493cec: cmp r0, r4
00493cf0: bne #0x493cfc
00493cf4: b #0x493d10
00493cf8: mov r0, r5
00493cfc: ldr r5, [r0]
00493d00: mov r1, #0xc
00493d04: bl #0x708f00
00493d08: cmp r5, r4
00493d0c: bne #0x493cf8
00493d10: add sp, sp, #0xc
00493d14: pop {r4, r5, r6, r7, pc}

# _ZNSt6vectorIPN15VisualFXManager10AnimFXStepESaIS2_EEaSERKS4_ 00493d18 size312
00493d18: push {r4, r5, r6, r7, lr}
00493d1c: cmp r1, r0
00493d20: sub sp, sp, #0xc
00493d24: mov r6, r1
00493d28: mov r4, r0
00493d2c: beq #0x493d74
00493d30: ldm r1, {r2, r3}
00493d34: ldr r7, [r0]
00493d38: ldr r1, [r0, #8]
00493d3c: rsb ip, r2, r3
00493d40: asr r5, ip, #2
00493d44: rsb r1, r7, r1
00493d48: cmp r5, r1, asr #2
00493d4c: bhi #0x493dac
00493d50: ldr r0, [r0, #4]
00493d54: rsb r1, r7, r0
00493d58: asr r1, r1, #2
00493d5c: cmp r5, r1
00493d60: bhi #0x493d80
00493d64: cmp ip, #0
00493d68: bne #0x493dfc
00493d6c: add r5, r7, r5, lsl #2
00493d70: str r5, [r4, #4]
00493d74: mov r0, r4
00493d78: add sp, sp, #0xc
00493d7c: pop {r4, r5, r6, r7, pc}
00493d80: add r1, r2, r1, lsl #2
00493d84: subs ip, r1, r2
00493d88: bne #0x493e1c
00493d8c: cmp r1, r3
00493d90: beq #0x493d6c
00493d94: rsb r2, r1, r3
00493d98: bl #0x30e868
00493d9c: ldr r7, [r4]
00493da0: add r5, r7, r5, lsl #2
00493da4: str r5, [r4, #4]
00493da8: b #0x493d74
00493dac: add r1, sp, #8
00493db0: str r5, [r1, #-4]!
00493db4: bl #0x49398c
00493db8: mov r7, r0
00493dbc: ldr r0, [r4]
00493dc0: ldr r1, [r4, #8]
00493dc4: cmp r0, #0
00493dc8: beq #0x493de0
00493dcc: rsb r1, r0, r1
00493dd0: bic r1, r1, #3
00493dd4: cmp r1, #0x80
00493dd8: bhi #0x493e48
00493ddc: bl #0x708f00
00493de0: ldr r3, [sp, #4]
00493de4: add r5, r7, r5, lsl #2
00493de8: str r7, [r4]
00493dec: add r3, r7, r3, lsl #2
00493df0: str r3, [r4, #8]
00493df4: str r5, [r4, #4]
00493df8: b #0x493d74
00493dfc: mov r0, r7
00493e00: mov r1, r2
00493e04: mov r2, ip
00493e08: bl #0x30df38
00493e0c: ldr r7, [r4]
00493e10: add r5, r7, r5, lsl #2
00493e14: str r5, [r4, #4]
00493e18: b #0x493d74
00493e1c: mov r1, r2
00493e20: mov r0, r7
00493e24: mov r2, ip
00493e28: bl #0x30df38
00493e2c: ldr r0, [r4, #4]
00493e30: ldr r7, [r4]
00493e34: ldm r6, {r2, r3}
00493e38: rsb r1, r7, r0
00493e3c: bic r1, r1, #3
00493e40: add r1, r2, r1
00493e44: b #0x493d8c
00493e48: bl #0x310440
00493e4c: b #0x493de0

# _ZNSt6vectorIP10AnimatedFXSaIS1_EEaSERKS3_ 00493e50 size312
00493e50: push {r4, r5, r6, r7, lr}
00493e54: cmp r1, r0
00493e58: sub sp, sp, #0xc
00493e5c: mov r6, r1
00493e60: mov r4, r0
00493e64: beq #0x493eac
00493e68: ldm r1, {r2, r3}
00493e6c: ldr r7, [r0]
00493e70: ldr r1, [r0, #8]
00493e74: rsb ip, r2, r3
00493e78: asr r5, ip, #2
00493e7c: rsb r1, r7, r1
00493e80: cmp r5, r1, asr #2
00493e84: bhi #0x493ee4
00493e88: ldr r0, [r0, #4]
00493e8c: rsb r1, r7, r0
00493e90: asr r1, r1, #2
00493e94: cmp r5, r1
00493e98: bhi #0x493eb8
00493e9c: cmp ip, #0
00493ea0: bne #0x493f34
00493ea4: add r5, r7, r5, lsl #2
00493ea8: str r5, [r4, #4]
00493eac: mov r0, r4
00493eb0: add sp, sp, #0xc
00493eb4: pop {r4, r5, r6, r7, pc}
00493eb8: add r1, r2, r1, lsl #2
00493ebc: subs ip, r1, r2
00493ec0: bne #0x493f54
00493ec4: cmp r1, r3
00493ec8: beq #0x493ea4
00493ecc: rsb r2, r1, r3
00493ed0: bl #0x30e868
00493ed4: ldr r7, [r4]
00493ed8: add r5, r7, r5, lsl #2
00493edc: str r5, [r4, #4]
00493ee0: b #0x493eac
00493ee4: add r1, sp, #8
00493ee8: str r5, [r1, #-4]!
00493eec: bl #0x493a38
00493ef0: mov r7, r0
00493ef4: ldr r0, [r4]
00493ef8: ldr r1, [r4, #8]
00493efc: cmp r0, #0
00493f00: beq #0x493f18
00493f04: rsb r1, r0, r1
00493f08: bic r1, r1, #3
00493f0c: cmp r1, #0x80
00493f10: bhi #0x493f80
00493f14: bl #0x708f00
00493f18: ldr r3, [sp, #4]
00493f1c: add r5, r7, r5, lsl #2
00493f20: str r7, [r4]
00493f24: add r3, r7, r3, lsl #2
00493f28: str r3, [r4, #8]
00493f2c: str r5, [r4, #4]
00493f30: b #0x493eac
00493f34: mov r0, r7
00493f38: mov r1, r2
00493f3c: mov r2, ip
00493f40: bl #0x30df38
00493f44: ldr r7, [r4]
00493f48: add r5, r7, r5, lsl #2
00493f4c: str r5, [r4, #4]
00493f50: b #0x493eac
00493f54: mov r1, r2
00493f58: mov r0, r7
00493f5c: mov r2, ip
00493f60: bl #0x30df38
00493f64: ldr r0, [r4, #4]
00493f68: ldr r7, [r4]
00493f6c: ldm r6, {r2, r3}
00493f70: rsb r1, r7, r0
00493f74: bic r1, r1, #3
00493f78: add r1, r2, r1
00493f7c: b #0x493ec4
00493f80: bl #0x310440
00493f84: b #0x493f18

# _ZN15VisualFXManager14AnimatedFXInfoD1Ev 00493f88 size76
00493f88: push {r4, lr}
00493f8c: mov r4, r0
00493f90: add r0, r0, #0x10
00493f94: bl #0x493c04
00493f98: ldr r0, [r4, #4]
00493f9c: add r3, r4, #4
00493fa0: cmp r0, #0
00493fa4: beq #0x493fc0
00493fa8: ldr r1, [r3, #8]
00493fac: rsb r1, r0, r1
00493fb0: bic r1, r1, #3
00493fb4: cmp r1, #0x80
00493fb8: bhi #0x493fc8
00493fbc: bl #0x708f00
00493fc0: mov r0, r4
00493fc4: pop {r4, pc}
00493fc8: bl #0x310440
00493fcc: mov r0, r4
00493fd0: pop {r4, pc}

# _ZNSt6vectorIN15VisualFXManager14AnimatedFXInfoESaIS1_EE19_M_clear_after_moveEv 00493fd4 size132
00493fd4: push {r4, r5, r6, lr}
00493fd8: ldr r4, [r0, #4]
00493fdc: ldr r5, [r0]
00493fe0: mov r6, r0
00493fe4: cmp r4, r5
00493fe8: beq #0x494004
00493fec: sub r4, r4, #0x18
00493ff0: mov r0, r4
00493ff4: bl #0x493f88
00493ff8: cmp r5, r4
00493ffc: bne #0x493fec
00494000: ldr r4, [r6]
00494004: cmp r4, #0
00494008: ldr r3, [r6, #8]
0049400c: beq #0x494054
00494010: rsb r3, r4, r3
00494014: asr r3, r3, #3
00494018: add r1, r3, r3, lsl #2
0049401c: add r1, r1, r1, lsl #4
00494020: add r1, r1, r1, lsl #8
00494024: add r1, r1, r1, lsl #16
00494028: add r3, r3, r1, lsl #1
0049402c: mov r1, #0x18
00494030: mul r1, r1, r3
00494034: cmp r1, #0x80
00494038: bhi #0x494048
0049403c: mov r0, r4
00494040: pop {r4, r5, r6, lr}
00494044: b #0x708f00
00494048: mov r0, r4
0049404c: pop {r4, r5, r6, lr}
00494050: b #0x310440
00494054: pop {r4, r5, r6, pc}

# _ZNSt6vectorIN15VisualFXManager14AnimatedFXInfoESaIS1_EED1Ev 00494058 size128
00494058: push {r4, r5, r6, lr}
0049405c: ldr r5, [r0, #4]
00494060: ldr r6, [r0]
00494064: mov r4, r0
00494068: cmp r5, r6
0049406c: beq #0x494084
00494070: sub r5, r5, #0x18
00494074: mov r0, r5
00494078: bl #0x493f88
0049407c: cmp r6, r5
00494080: bne #0x494070
00494084: ldr r0, [r4]
00494088: cmp r0, #0
0049408c: beq #0x4940c4
00494090: ldr r3, [r4, #8]
00494094: rsb r3, r0, r3
00494098: asr r3, r3, #3
0049409c: add r1, r3, r3, lsl #2
004940a0: add r1, r1, r1, lsl #4
004940a4: add r1, r1, r1, lsl #8
004940a8: add r1, r1, r1, lsl #16
004940ac: add r3, r3, r1, lsl #1
004940b0: mov r1, #0x18
004940b4: mul r1, r1, r3
004940b8: cmp r1, #0x80
004940bc: bhi #0x4940cc
004940c0: bl #0x708f00
004940c4: mov r0, r4
004940c8: pop {r4, r5, r6, pc}
004940cc: bl #0x310440
004940d0: mov r0, r4
004940d4: pop {r4, r5, r6, pc}

# _ZN15VisualFXManager13AnimFXSetInfoD1Ev 004940d8 size124
004940d8: push {r4, r5, r6, lr}
004940dc: mov r6, r0
004940e0: ldr r0, [r0, #0x10]
004940e4: add r5, r6, #0x10
004940e8: cmp r0, r5
004940ec: bne #0x4940f8
004940f0: b #0x494110
004940f4: mov r0, r4
004940f8: ldr r4, [r0]
004940fc: mov r1, #0xc
00494100: bl #0x708f00
00494104: cmp r4, r5
00494108: bne #0x4940f4
0049410c: mov r0, r5
00494110: str r0, [r6, #0x10]
00494114: str r0, [r5, #4]
00494118: ldr r0, [r6, #4]
0049411c: add r3, r6, #4
00494120: cmp r0, #0
00494124: beq #0x494140
00494128: ldr r1, [r3, #8]
0049412c: rsb r1, r0, r1
00494130: bic r1, r1, #3
00494134: cmp r1, #0x80
00494138: bhi #0x494148
0049413c: bl #0x708f00
00494140: mov r0, r6
00494144: pop {r4, r5, r6, pc}
00494148: bl #0x310440
0049414c: mov r0, r6
00494150: pop {r4, r5, r6, pc}

# _ZNSt6vectorIN15VisualFXManager13AnimFXSetInfoESaIS1_EE19_M_clear_after_moveEv 00494154 size132
00494154: push {r4, r5, r6, lr}
00494158: ldr r4, [r0, #4]
0049415c: ldr r5, [r0]
00494160: mov r6, r0
00494164: cmp r4, r5
00494168: beq #0x494184
0049416c: sub r4, r4, #0x18
00494170: mov r0, r4
00494174: bl #0x4940d8
00494178: cmp r5, r4
0049417c: bne #0x49416c
00494180: ldr r4, [r6]
00494184: cmp r4, #0
00494188: ldr r3, [r6, #8]
0049418c: beq #0x4941d4
00494190: rsb r3, r4, r3
00494194: asr r3, r3, #3
00494198: add r1, r3, r3, lsl #2
0049419c: add r1, r1, r1, lsl #4
004941a0: add r1, r1, r1, lsl #8
004941a4: add r1, r1, r1, lsl #16
004941a8: add r3, r3, r1, lsl #1
004941ac: mov r1, #0x18
004941b0: mul r1, r1, r3
004941b4: cmp r1, #0x80
004941b8: bhi #0x4941c8
004941bc: mov r0, r4
004941c0: pop {r4, r5, r6, lr}
004941c4: b #0x708f00
004941c8: mov r0, r4
004941cc: pop {r4, r5, r6, lr}
004941d0: b #0x310440
004941d4: pop {r4, r5, r6, pc}

# _ZNSt6vectorIN15VisualFXManager13AnimFXSetInfoESaIS1_EE9push_backERKS1_ 004941d8 size292
004941d8: push {r4, r5, r6, r7, r8, sb, sl, lr}
004941dc: mov r4, r0
004941e0: ldr r5, [r4, #8]
004941e4: ldr r0, [r0, #4]
004941e8: sub sp, sp, #8
004941ec: mov r6, r1
004941f0: cmp r0, r5
004941f4: beq #0x494210
004941f8: bl #0x493924
004941fc: ldr r3, [r4, #4]
00494200: add r3, r3, #0x18
00494204: str r3, [r4, #4]
00494208: add sp, sp, #8
0049420c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00494210: ldr r2, [r4]
00494214: movw r3, #0xaaaa
00494218: orr r3, r3, r3, lsl #12
0049421c: rsb r2, r2, r5
00494220: asr r2, r2, #3
00494224: add r1, r2, r2, lsl #2
00494228: add r1, r1, r1, lsl #4
0049422c: add r1, r1, r1, lsl #8
00494230: add r1, r1, r1, lsl #16
00494234: add r2, r2, r1, lsl #1
00494238: cmp r2, #1
0049423c: addhs r1, r2, r2
00494240: addlo r1, r2, #1
00494244: cmp r1, r3
00494248: bls #0x4942f0
0049424c: movw r1, #0xaaaa
00494250: orr r1, r1, r1, lsl #12
00494254: add r2, sp, #8
00494258: str r1, [r2, #-4]!
0049425c: add r0, r4, #8
00494260: bl #0x493af4
00494264: ldr sb, [r4]
00494268: mov sl, r0
0049426c: rsb r5, sb, r5
00494270: asr r3, r5, #3
00494274: add r5, r3, r3, lsl #2
00494278: add r5, r5, r5, lsl #4
0049427c: add r5, r5, r5, lsl #8
00494280: add r5, r5, r5, lsl #16
00494284: add r5, r3, r5, lsl #1
00494288: cmp r5, #0
0049428c: movle r5, r0
00494290: ble #0x4942bc
00494294: mov r8, r5
00494298: mov r7, #0
0049429c: add r0, sl, r7
004942a0: add r1, sb, r7
004942a4: bl #0x493924
004942a8: subs r8, r8, #1
004942ac: add r7, r7, #0x18
004942b0: bne #0x49429c
004942b4: mov r3, #0x18
004942b8: mla r5, r3, r5, sl
004942bc: mov r1, r6
004942c0: mov r0, r5
004942c4: bl #0x493924
004942c8: mov r0, r4
004942cc: bl #0x494154
004942d0: ldr r3, [sp, #4]
004942d4: mov r2, #0x18
004942d8: add r5, r5, #0x18
004942dc: mla r3, r2, r3, sl
004942e0: str sl, [r4]
004942e4: str r3, [r4, #8]
004942e8: str r5, [r4, #4]
004942ec: b #0x494208
004942f0: cmp r2, r1
004942f4: bls #0x494254
004942f8: b #0x49424c

# _ZNSt6vectorIN15VisualFXManager13AnimFXSetInfoESaIS1_EED1Ev 004942fc size128
004942fc: push {r4, r5, r6, lr}
00494300: ldr r5, [r0, #4]
00494304: ldr r6, [r0]
00494308: mov r4, r0
0049430c: cmp r5, r6
00494310: beq #0x494328
00494314: sub r5, r5, #0x18
00494318: mov r0, r5
0049431c: bl #0x4940d8
00494320: cmp r6, r5
00494324: bne #0x494314
00494328: ldr r0, [r4]
0049432c: cmp r0, #0
00494330: beq #0x494368
00494334: ldr r3, [r4, #8]
00494338: rsb r3, r0, r3
0049433c: asr r3, r3, #3
00494340: add r1, r3, r3, lsl #2
00494344: add r1, r1, r1, lsl #4
00494348: add r1, r1, r1, lsl #8
0049434c: add r1, r1, r1, lsl #16
00494350: add r3, r3, r1, lsl #1
00494354: mov r1, #0x18
00494358: mul r1, r1, r3
0049435c: cmp r1, #0x80
00494360: bhi #0x494370
00494364: bl #0x708f00
00494368: mov r0, r4
0049436c: pop {r4, r5, r6, pc}
00494370: bl #0x310440
00494374: mov r0, r4
00494378: pop {r4, r5, r6, pc}

# _ZN9VectorSetIiE16push_back_uniqueERKi 0049437c size272
0049437c: push {r4, r5, r6, r7, lr}
00494380: mov r4, r0
00494384: sub sp, sp, #0xc
00494388: mov r5, r1
0049438c: add r3, sp, #4
00494390: ldr r0, [r0]
00494394: ldr r1, [r4, #4]
00494398: mov r2, r5
0049439c: bl #0x369350
004943a0: ldr r3, [r4, #4]
004943a4: mov r6, r0
004943a8: cmp r0, r3
004943ac: beq #0x4943b8
004943b0: add sp, sp, #0xc
004943b4: pop {r4, r5, r6, r7, pc}
004943b8: ldr r3, [r4, #8]
004943bc: cmp r0, r3
004943c0: beq #0x4943dc
004943c4: ldr r3, [r5]
004943c8: str r3, [r0]
004943cc: ldr r3, [r4, #4]
004943d0: add r3, r3, #4
004943d4: str r3, [r4, #4]
004943d8: b #0x4943b0
004943dc: ldr r3, [r4]
004943e0: rsb r3, r3, r0
004943e4: asr r3, r3, #2
004943e8: cmp r3, #1
004943ec: addhs r1, r3, r3
004943f0: addlo r1, r3, #1
004943f4: cmn r1, #0xc0000001
004943f8: bhi #0x49447c
004943fc: cmp r3, r1
00494400: bhi #0x49447c
00494404: add r2, sp, #8
00494408: str r1, [r2, #-8]!
0049440c: add r0, r4, #8
00494410: mov r2, sp
00494414: bl #0x35fd5c
00494418: ldr r1, [r4]
0049441c: mov r7, r0
00494420: subs r6, r6, r1
00494424: moveq r6, r0
00494428: beq #0x494438
0049442c: mov r2, r6
00494430: bl #0x30df38
00494434: add r6, r0, r6
00494438: ldr r3, [r5]
0049443c: str r3, [r6], #4
00494440: ldr r0, [r4]
00494444: ldr r1, [r4, #8]
00494448: cmp r0, #0
0049444c: beq #0x494464
00494450: rsb r1, r0, r1
00494454: bic r1, r1, #3
00494458: cmp r1, #0x80
0049445c: bhi #0x494484
00494460: bl #0x708f00
00494464: ldr r3, [sp]
00494468: str r7, [r4]
0049446c: str r6, [r4, #4]
00494470: add r7, r7, r3, lsl #2
00494474: str r7, [r4, #8]
00494478: b #0x4943b0
0049447c: mvn r1, #0xc0000000
00494480: b #0x494404
00494484: bl #0x310440
00494488: b #0x494464

# _ZNSt6vectorIPN15VisualFXManager10AnimFXStepESaIS2_EE18_M_insert_overflowEPS2_RKS2_RKSt11__true_typejb.clone.2 0049448c size196
0049448c: push {r4, r5, r6, r7, lr}
00494490: mov r4, r0
00494494: ldr r3, [r4]
00494498: ldr r0, [r0, #4]
0049449c: mov r6, r1
004944a0: sub sp, sp, #0xc
004944a4: rsb r3, r3, r0
004944a8: asr r3, r3, #2
004944ac: cmp r3, #1
004944b0: addhs r1, r3, r3
004944b4: addlo r1, r3, #1
004944b8: cmn r1, #0xc0000001
004944bc: mov r7, r2
004944c0: bhi #0x494540
004944c4: cmp r3, r1
004944c8: bhi #0x494540
004944cc: add r2, sp, #8
004944d0: str r1, [r2, #-4]!
004944d4: add r0, r4, #8
004944d8: bl #0x493834
004944dc: ldr r1, [r4]
004944e0: mov r5, r0
004944e4: subs r6, r6, r1
004944e8: moveq r6, r0
004944ec: beq #0x4944fc
004944f0: mov r2, r6
004944f4: bl #0x30df38
004944f8: add r6, r0, r6
004944fc: ldr r3, [r7]
00494500: str r3, [r6], #4
00494504: ldr r0, [r4]
00494508: ldr r1, [r4, #8]
0049450c: cmp r0, #0
00494510: beq #0x494528
00494514: rsb r1, r0, r1
00494518: bic r1, r1, #3
0049451c: cmp r1, #0x80
00494520: bhi #0x494548
00494524: bl #0x708f00
00494528: ldr r3, [sp, #4]
0049452c: stm r4, {r5, r6}
00494530: add r5, r5, r3, lsl #2
00494534: str r5, [r4, #8]
00494538: add sp, sp, #0xc
0049453c: pop {r4, r5, r6, r7, pc}
00494540: mvn r1, #0xc0000000
00494544: b #0x4944cc
00494548: bl #0x310440
0049454c: b #0x494528

# _ZN9VectorSetIP10AnimatedFXE16push_back_uniqueERKS1_ 00494550 size272
00494550: push {r4, r5, r6, r7, lr}
00494554: mov r4, r0
00494558: sub sp, sp, #0xc
0049455c: mov r5, r1
00494560: add r3, sp, #4
00494564: ldr r0, [r0]
00494568: ldr r1, [r4, #4]
0049456c: mov r2, r5
00494570: bl #0x493488
00494574: ldr r3, [r4, #4]
00494578: mov r6, r0
0049457c: cmp r0, r3
00494580: beq #0x49458c
00494584: add sp, sp, #0xc
00494588: pop {r4, r5, r6, r7, pc}
0049458c: ldr r3, [r4, #8]
00494590: cmp r0, r3
00494594: beq #0x4945b0
00494598: ldr r3, [r5]
0049459c: str r3, [r0]
004945a0: ldr r3, [r4, #4]
004945a4: add r3, r3, #4
004945a8: str r3, [r4, #4]
004945ac: b #0x494584
004945b0: ldr r3, [r4]
004945b4: rsb r3, r3, r0
004945b8: asr r3, r3, #2
004945bc: cmp r3, #1
004945c0: addhs r1, r3, r3
004945c4: addlo r1, r3, #1
004945c8: cmn r1, #0xc0000001
004945cc: bhi #0x494650
004945d0: cmp r3, r1
004945d4: bhi #0x494650
004945d8: add r2, sp, #8
004945dc: str r1, [r2, #-8]!
004945e0: add r0, r4, #8
004945e4: mov r2, sp
004945e8: bl #0x4939c8
004945ec: ldr r1, [r4]
004945f0: mov r7, r0
004945f4: subs r6, r6, r1
004945f8: moveq r6, r0
004945fc: beq #0x49460c
00494600: mov r2, r6
00494604: bl #0x30df38
00494608: add r6, r0, r6
0049460c: ldr r3, [r5]
00494610: str r3, [r6], #4
00494614: ldr r0, [r4]
00494618: ldr r1, [r4, #8]
0049461c: cmp r0, #0
00494620: beq #0x494638
00494624: rsb r1, r0, r1
00494628: bic r1, r1, #3
0049462c: cmp r1, #0x80
00494630: bhi #0x494658
00494634: bl #0x708f00
00494638: ldr r3, [sp]
0049463c: str r7, [r4]
00494640: str r6, [r4, #4]
00494644: add r7, r7, r3, lsl #2
00494648: str r7, [r4, #8]
0049464c: b #0x494584
00494650: mvn r1, #0xc0000000
00494654: b #0x4945d8
00494658: bl #0x310440
0049465c: b #0x494638

# _ZN15VisualFXManager21DropAnimatedFXSetByIDEi 00494660 size408
00494660: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00494664: ldr r4, [pc, #0x180]
00494668: subs r6, r1, #0
0049466c: sub sp, sp, #0x6c
00494670: mov r5, r0
00494674: add r4, pc, r4
00494678: blt #0x4947e4
0049467c: ldr r3, [pc, #0x16c]
00494680: ldr r3, [r4, r3]
00494684: ldr r3, [r3]
00494688: cmp r6, r3
0049468c: bge #0x4947e4
00494690: ldr fp, [pc, #0x15c]
00494694: mov sb, #0x18
00494698: ldr r7, [r0, #0x1c]
0049469c: ldr lr, [r4, fp]
004946a0: mul r6, sb, r6
004946a4: ldr r8, [lr, #8]
004946a8: ldr r2, [r7, r6]
004946ac: mov ip, #0
004946b0: mov r3, ip
004946b4: ldr r2, [r2, #8]
004946b8: str r8, [sp, #0x60]
004946bc: ldr r8, [lr]
004946c0: add sl, sp, #0x34
004946c4: str r8, [sp, #0x58]
004946c8: ldr r8, [lr, #4]
004946cc: add lr, r7, r6
004946d0: str lr, [sp, #0x14]
004946d4: ldr lr, [sp, #0x58]
004946d8: str ip, [sp]
004946dc: str ip, [sp, #4]
004946e0: str lr, [sp, #0x4c]
004946e4: ldr lr, [sp, #0x60]
004946e8: str r8, [sp, #0x50]
004946ec: str r8, [sp, #0x5c]
004946f0: str lr, [sp, #0x54]
004946f4: add lr, sp, #0x58
004946f8: str lr, [sp, #8]
004946fc: add lr, sp, #0x4c
00494700: str lr, [sp, #0xc]
00494704: bl #0x4935b8
00494708: ldr r1, [sp, #0x14]
0049470c: mov r8, r0
00494710: mov r0, sl
00494714: bl #0x493924
00494718: mov r1, r5
0049471c: mov r2, sl
00494720: mov r3, r8
00494724: add r0, sp, sb
00494728: bl #0x4933e4
0049472c: mov r0, sl
00494730: bl #0x4940d8
00494734: ldr r3, [r7, r6]
00494738: ldr r2, [r8, #4]
0049473c: mov ip, #0x30
00494740: ldr r1, [r3, #0x10]
00494744: ldr r3, [r5, #0x28]
00494748: mov r0, r8
0049474c: mla r2, ip, r2, r1
00494750: ldr r2, [r2, #4]
00494754: mla sb, sb, r2, r3
00494758: bl #0x310440
0049475c: mov r5, sb
00494760: ldr r3, [r5, #0x10]!
00494764: cmp r3, r5
00494768: beq #0x4947e4
0049476c: mov r2, r3
00494770: ldr r2, [r2]
00494774: cmp r5, r2
00494778: bne #0x494770
0049477c: ldr r3, [r3, #8]
00494780: add r1, sp, #0x68
00494784: add r0, sb, #4
00494788: str r3, [r1, #-4]!
0049478c: bl #0x494550
00494790: mov r0, r5
00494794: bl #0x493c04
00494798: ldr r3, [r4, fp]
0049479c: ldr r0, [sp, #0x64]
004947a0: mov r1, #0
004947a4: ldr ip, [r3]
004947a8: ldr r2, [r3, #4]
004947ac: ldr r3, [r3, #8]
004947b0: str ip, [r0, #0x34]
004947b4: str r2, [r0, #0x38]
004947b8: str r3, [r0, #0x3c]
004947bc: bl #0x492aa0
004947c0: ldr r3, [sp, #0x64]
004947c4: mov r4, #0
004947c8: mov r1, #1
004947cc: mov r0, r3
004947d0: str r4, [r3, #0x28]
004947d4: bl #0x492aa0
004947d8: mov r1, r4
004947dc: ldr r0, [sp, #0x64]
004947e0: bl #0x492ef0
004947e4: add sp, sp, #0x6c
004947e8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004947ec: subseq r0, r0, ip, lsl r4
004947f0: andeq r0, r0, r4, asr #13
004947f4: andeq r3, r0, ip, lsr #30

# _ZNSt4listIPN15VisualFXManager13AnimFXSetDataESaIS2_EEaSERKS4_ 004947f8 size156
004947f8: push {r4, r5, lr}
004947fc: cmp r0, r1
00494800: sub sp, sp, #0x14
00494804: mov r4, r0
00494808: mov r3, r1
0049480c: beq #0x494860
00494810: ldr r0, [r0]
00494814: ldr r2, [r1]
00494818: b #0x494838
0049481c: cmp r3, r2
00494820: beq #0x494870
00494824: ldr ip, [r2, #8]
00494828: ldr r1, [r0]
0049482c: ldr r2, [r2]
00494830: str ip, [r0, #8]
00494834: mov r0, r1
00494838: cmp r4, r0
0049483c: bne #0x49481c
00494840: cmp r3, r2
00494844: beq #0x494860
00494848: add r1, sp, #0x10
0049484c: str r4, [r1, #-8]!
00494850: add ip, sp, #0xc
00494854: mov r0, r4
00494858: str ip, [sp]
0049485c: bl #0x493c44
00494860: mov r0, r4
00494864: add sp, sp, #0x14
00494868: pop {r4, r5, pc}
0049486c: mov r0, r5
00494870: ldr r5, [r0]
00494874: ldr r3, [r0, #4]
00494878: mov r1, #0xc
0049487c: str r5, [r3]
00494880: str r3, [r5, #4]
00494884: bl #0x708f00
00494888: cmp r4, r5
0049488c: bne #0x49486c
00494890: b #0x494860

# _ZNSt4priv6__copyIPN15VisualFXManager13AnimFXSetInfoES3_iEET0_T_S5_S4_RKSt26random_access_iterator_tagPT1_ 00494894 size124
00494894: rsb r3, r0, r1
00494898: asr r3, r3, #3
0049489c: push {r4, r5, r6, r7, r8, sb, sl, lr}
004948a0: add sb, r3, r3, lsl #2
004948a4: mov r4, r0
004948a8: add sb, sb, sb, lsl #4
004948ac: mov sl, r2
004948b0: add sb, sb, sb, lsl #8
004948b4: add sb, sb, sb, lsl #16
004948b8: add sb, r3, sb, lsl #1
004948bc: cmp sb, #0
004948c0: ble #0x494908
004948c4: mov r6, sb
004948c8: mov r5, #0
004948cc: ldr r3, [r4, r5]
004948d0: add r7, r4, r5
004948d4: add r8, sl, r5
004948d8: str r3, [sl, r5]
004948dc: add r1, r7, #4
004948e0: add r0, r8, #4
004948e4: bl #0x493d18
004948e8: add r0, r8, #0x10
004948ec: add r1, r7, #0x10
004948f0: bl #0x4947f8
004948f4: subs r6, r6, #1
004948f8: add r5, r5, #0x18
004948fc: bne #0x4948cc
00494900: mov r3, #0x18
00494904: mla sl, r3, sb, sl
00494908: mov r0, sl
0049490c: pop {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNSt6vectorIN15VisualFXManager13AnimFXSetInfoESaIS1_EE8_M_eraseEPS1_S4_RKSt12__false_type 00494910 size104
00494910: push {r4, r5, r6, r7, r8, lr}
00494914: ldr r3, [r0, #4]
00494918: sub sp, sp, #0x10
0049491c: mov r5, r1
00494920: mov r4, r0
00494924: mov r1, r3
00494928: mov r0, r2
0049492c: mov ip, #0
00494930: mov r2, r5
00494934: add r3, sp, #0xc
00494938: str ip, [sp]
0049493c: bl #0x494894
00494940: ldr r7, [r4, #4]
00494944: mov r8, r0
00494948: cmp r7, r0
0049494c: beq #0x494968
00494950: mov r6, r0
00494954: mov r0, r6
00494958: add r6, r6, #0x18
0049495c: bl #0x4940d8
00494960: cmp r7, r6
00494964: bne #0x494954
00494968: str r8, [r4, #4]
0049496c: mov r0, r5
00494970: add sp, sp, #0x10
00494974: pop {r4, r5, r6, r7, r8, pc}

# _ZN15VisualFXManager14DropAnimatedFXERP10AnimatedFX 00494978 size316
00494978: push {r4, r5, r6, r7, r8, lr}
0049497c: ldr r3, [r1]
00494980: ldr r6, [pc, #0x124]
00494984: mov r5, r1
00494988: cmp r3, #0
0049498c: add r6, pc, r6
00494990: beq #0x4949e4
00494994: ldrb r2, [r0, #4]
00494998: cmp r2, #0
0049499c: beq #0x4949e8
004949a0: ldr r3, [r3, #8]
004949a4: cmp r3, #0
004949a8: blt #0x4949d8
004949ac: ldr r2, [r0, #0x2c]
004949b0: ldr r0, [r0, #0x28]
004949b4: rsb r2, r0, r2
004949b8: asr r2, r2, #3
004949bc: add r1, r2, r2, lsl #2
004949c0: add r1, r1, r1, lsl #4
004949c4: add r1, r1, r1, lsl #8
004949c8: add r1, r1, r1, lsl #16
004949cc: add r2, r2, r1, lsl #1
004949d0: cmp r3, r2
004949d4: blo #0x4949f0
004949d8: mov r3, #0
004949dc: str r3, [r5]
004949e0: pop {r4, r5, r6, r7, r8, pc}
004949e4: pop {r4, r5, r6, r7, r8, pc}
004949e8: str r2, [r1]
004949ec: pop {r4, r5, r6, r7, r8, pc}
004949f0: mov r8, #0x18
004949f4: mla r8, r8, r3, r0
004949f8: mov r7, r8
004949fc: ldr r0, [r7, #0x10]!
00494a00: cmp r7, r0
00494a04: beq #0x494a28
00494a08: ldr r3, [r0, #8]
00494a0c: ldr r2, [r5]
00494a10: ldr r4, [r0]
00494a14: cmp r2, r3
00494a18: beq #0x494a90
00494a1c: mov r0, r4
00494a20: cmp r7, r0
00494a24: bne #0x494a08
00494a28: add r0, r8, #4
00494a2c: mov r1, r5
00494a30: bl #0x494550
00494a34: ldr r3, [r5]
00494a38: mov r4, #0
00494a3c: mov r1, #1
00494a40: mov r0, r3
00494a44: str r4, [r3, #0x28]
00494a48: bl #0x492aa0
00494a4c: ldr r2, [pc, #0x5c]
00494a50: ldr r3, [r5]
00494a54: mov r1, r4
00494a58: ldr r2, [r6, r2]
00494a5c: mov r0, r3
00494a60: ldr lr, [r2]
00494a64: ldr ip, [r2, #4]
00494a68: ldr r2, [r2, #8]
00494a6c: str lr, [r3, #0x34]
00494a70: str ip, [r3, #0x38]
00494a74: str r2, [r3, #0x3c]
00494a78: bl #0x492aa0
00494a7c: ldr r0, [r5]
00494a80: mov r1, r4
00494a84: bl #0x492ef0
00494a88: str r4, [r5]
00494a8c: pop {r4, r5, r6, r7, r8, pc}
00494a90: ldr r3, [r0, #4]
00494a94: mov r1, #0xc
00494a98: str r4, [r3]
00494a9c: str r3, [r4, #4]
00494aa0: bl #0x708f00
00494aa4: mov r0, r4
00494aa8: b #0x494a20
00494aac: subseq r0, r0, r4, lsl #2
00494ab0: andeq r3, r0, ip, lsr #30

# _ZNSaINSt4priv10_List_nodeIP10AnimatedFXEEE8allocateEjPKv.clone.12 00494ab4 size32
00494ab4: str lr, [sp, #-4]!
00494ab8: sub sp, sp, #0xc
00494abc: add r0, sp, #8
00494ac0: mov r3, #0xc
00494ac4: str r3, [r0, #-4]!
00494ac8: bl #0x708ec0
00494acc: add sp, sp, #0xc
00494ad0: ldm sp!, {pc}

# _ZN15VisualFXManager10_GetAnimFXEi 00494ad4 size308
00494ad4: push {r4, r5, r6, r7, r8, sl, lr}
00494ad8: ldr sl, [pc, #0x118]
00494adc: subs r7, r1, #0
00494ae0: sub sp, sp, #0xc
00494ae4: add sl, pc, sl
00494ae8: bge #0x494afc
00494aec: mov r5, #0
00494af0: mov r0, r5
00494af4: add sp, sp, #0xc
00494af8: pop {r4, r5, r6, r7, r8, sl, pc}
00494afc: ldr r3, [pc, #0xf8]
00494b00: ldr r3, [sl, r3]
00494b04: ldr r3, [r3]
00494b08: cmp r7, r3
00494b0c: bge #0x494aec
00494b10: ldr r3, [r0, #0x28]
00494b14: mov r8, #0x18
00494b18: mla r8, r8, r7, r3
00494b1c: ldr r2, [r8, #8]
00494b20: ldr r3, [r8, #4]
00494b24: rsb r3, r3, r2
00494b28: asrs r3, r3, #2
00494b2c: bne #0x494bd0
00494b30: mov r6, r8
00494b34: ldr r4, [r6, #0x10]!
00494b38: cmp r4, r6
00494b3c: beq #0x494b58
00494b40: ldr r4, [r4]
00494b44: add r3, r3, #1
00494b48: cmp r6, r4
00494b4c: bne #0x494b40
00494b50: cmp r3, #5
00494b54: bhi #0x494aec
00494b58: mov r1, #0
00494b5c: mov r0, #0x54
00494b60: bl #0x310570
00494b64: mov r1, r7
00494b68: mov r5, r0
00494b6c: bl #0x492374
00494b70: ldr r3, [pc, #0x88]
00494b74: mov r0, #0xc
00494b78: ldr r2, [pc, #0x84]
00494b7c: ldr r1, [sl, r3]
00494b80: mvn ip, #0
00494b84: mov r3, #0x3f800000
00494b88: ldr r1, [r1]
00494b8c: add r2, pc, r2
00494b90: mla r7, r0, r7, r1
00494b94: mov r0, r5
00494b98: ldr r1, [r7, #8]
00494b9c: str ip, [sp]
00494ba0: mov ip, #1
00494ba4: str ip, [sp, #4]
00494ba8: bl #0x49296c
00494bac: mov r0, r6
00494bb0: bl #0x494ab4
00494bb4: str r5, [r0, #8]
00494bb8: ldr r3, [r8, #0x14]
00494bbc: str r4, [r0]
00494bc0: str r3, [r0, #4]
00494bc4: str r0, [r3]
00494bc8: str r0, [r8, #0x14]
00494bcc: b #0x494af0
00494bd0: ldr r5, [r2, #-4]
00494bd4: mov r1, #0x3f800000
00494bd8: add r4, r8, #0x10
00494bdc: ldr r0, [r5, #0x2c]
00494be0: bl #0x472708
00494be4: ldr r3, [r8, #8]
00494be8: mov r0, r4
00494bec: sub r3, r3, #4
00494bf0: str r3, [r8, #8]
00494bf4: b #0x494bb0
00494bf8: subeq pc, pc, ip, lsr #31
00494bfc: andeq r0, r0, r8, lsl #23
00494c00: strheq r1, [r0], -r8
00494c04: subeq r6, r3, ip, ror ip

# _ZNSt4listIP10AnimatedFXSaIS1_EE25_M_splice_insert_dispatchINSt4priv14_List_iteratorIS1_St13_Const_traitsIS1_EEEEEvNS6_IS1_St16_Nonconst_traitsIS1_EEET_SD_RKSt12__false_type 00494c08 size180
00494c08: push {r4, r5, r6, r7, lr}
00494c0c: cmp r3, r2
00494c10: sub sp, sp, #0xc
00494c14: mov r5, r3
00494c18: mov r4, sp
00494c1c: mov r7, r1
00494c20: str sp, [sp]
00494c24: str sp, [sp, #4]
00494c28: moveq r3, sp
00494c2c: beq #0x494c68
00494c30: mov r6, r2
00494c34: mov r0, r4
00494c38: bl #0x494ab4
00494c3c: ldr r3, [r6, #8]
00494c40: str r3, [r0, #8]
00494c44: ldr r3, [sp, #4]
00494c48: str r4, [r0]
00494c4c: str r3, [r0, #4]
00494c50: str r0, [r3]
00494c54: str r0, [sp, #4]
00494c58: ldr r6, [r6]
00494c5c: cmp r6, r5
00494c60: bne #0x494c34
00494c64: ldr r3, [sp]
00494c68: cmp r3, r4
00494c6c: ldr r2, [r7]
00494c70: beq #0x494cac
00494c74: cmp r2, r4
00494c78: beq #0x494cac
00494c7c: ldr r1, [sp, #4]
00494c80: str r2, [r1]
00494c84: ldr r1, [r3, #4]
00494c88: str r4, [r1]
00494c8c: ldr r1, [r2, #4]
00494c90: str r3, [r1]
00494c94: ldr r0, [sp, #4]
00494c98: ldr r1, [r2, #4]
00494c9c: str r0, [r2, #4]
00494ca0: ldr r2, [r3, #4]
00494ca4: str r2, [sp, #4]
00494ca8: str r1, [r3, #4]
00494cac: mov r0, sp
00494cb0: bl #0x493c04
00494cb4: add sp, sp, #0xc
00494cb8: pop {r4, r5, r6, r7, pc}

# _ZNSt4listIP10AnimatedFXSaIS1_EEaSERKS3_ 00494cbc size156
00494cbc: push {r4, r5, lr}
00494cc0: cmp r0, r1
00494cc4: sub sp, sp, #0x14
00494cc8: mov r4, r0
00494ccc: mov r3, r1
00494cd0: beq #0x494d24
00494cd4: ldr r0, [r0]
00494cd8: ldr r2, [r1]
00494cdc: b #0x494cfc
00494ce0: cmp r3, r2
00494ce4: beq #0x494d34
00494ce8: ldr ip, [r2, #8]
00494cec: ldr r1, [r0]
00494cf0: ldr r2, [r2]
00494cf4: str ip, [r0, #8]
00494cf8: mov r0, r1
00494cfc: cmp r4, r0
00494d00: bne #0x494ce0
00494d04: cmp r3, r2
00494d08: beq #0x494d24
00494d0c: add r1, sp, #0x10
00494d10: str r4, [r1, #-8]!
00494d14: add ip, sp, #0xc
00494d18: mov r0, r4
00494d1c: str ip, [sp]
00494d20: bl #0x494c08
00494d24: mov r0, r4
00494d28: add sp, sp, #0x14
00494d2c: pop {r4, r5, pc}
00494d30: mov r0, r5
00494d34: ldr r5, [r0]
00494d38: ldr r3, [r0, #4]
00494d3c: mov r1, #0xc
00494d40: str r5, [r3]
00494d44: str r3, [r5, #4]
00494d48: bl #0x708f00
00494d4c: cmp r4, r5
00494d50: bne #0x494d30
00494d54: b #0x494d24

# _ZNSt4priv6__copyIPN15VisualFXManager14AnimatedFXInfoES3_iEET0_T_S5_S4_RKSt26random_access_iterator_tagPT1_ 00494d58 size124
00494d58: rsb r3, r0, r1
00494d5c: asr r3, r3, #3
00494d60: push {r4, r5, r6, r7, r8, sb, sl, lr}
00494d64: add sb, r3, r3, lsl #2
00494d68: mov r4, r0
00494d6c: add sb, sb, sb, lsl #4
00494d70: mov sl, r2
00494d74: add sb, sb, sb, lsl #8
00494d78: add sb, sb, sb, lsl #16
00494d7c: add sb, r3, sb, lsl #1
00494d80: cmp sb, #0
00494d84: ble #0x494dcc
00494d88: mov r6, sb
00494d8c: mov r5, #0
00494d90: ldr r3, [r4, r5]
00494d94: add r7, r4, r5
00494d98: add r8, sl, r5
00494d9c: str r3, [sl, r5]
00494da0: add r1, r7, #4
00494da4: add r0, r8, #4
00494da8: bl #0x493e50
00494dac: add r0, r8, #0x10
00494db0: add r1, r7, #0x10
00494db4: bl #0x494cbc
00494db8: subs r6, r6, #1
00494dbc: add r5, r5, #0x18
00494dc0: bne #0x494d90
00494dc4: mov r3, #0x18
00494dc8: mla sl, r3, sb, sl
00494dcc: mov r0, sl
00494dd0: pop {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNSt6vectorIN15VisualFXManager14AnimatedFXInfoESaIS1_EE8_M_eraseEPS1_S4_RKSt12__false_type 00494dd4 size104
00494dd4: push {r4, r5, r6, r7, r8, lr}
00494dd8: ldr r3, [r0, #4]
00494ddc: sub sp, sp, #0x10
00494de0: mov r5, r1
00494de4: mov r4, r0
00494de8: mov r1, r3
00494dec: mov r0, r2
00494df0: mov ip, #0
00494df4: mov r2, r5
00494df8: add r3, sp, #0xc
00494dfc: str ip, [sp]
00494e00: bl #0x494d58
00494e04: ldr r7, [r4, #4]
00494e08: mov r8, r0
00494e0c: cmp r7, r0
00494e10: beq #0x494e2c
00494e14: mov r6, r0
00494e18: mov r0, r6
00494e1c: add r6, r6, #0x18
00494e20: bl #0x493f88
00494e24: cmp r7, r6
00494e28: bne #0x494e18
00494e2c: str r8, [r4, #4]
00494e30: mov r0, r5
00494e34: add sp, sp, #0x10
00494e38: pop {r4, r5, r6, r7, r8, pc}

# _ZN15VisualFXManager14_FlushAnimDictEv 00494e3c size612
00494e3c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00494e40: mov r5, r0
00494e44: sub sp, sp, #0xc
00494e48: mov r4, r0
00494e4c: ldr r6, [r5, #8]!
00494e50: b #0x494e64
00494e54: add r1, r6, #8
00494e58: mov r0, r4
00494e5c: bl #0x494978
00494e60: ldr r6, [r6]
00494e64: cmp r6, r5
00494e68: bne #0x494e54
00494e6c: ldr r8, [r4, #0x28]
00494e70: ldr r3, [r4, #0x2c]
00494e74: rsb r2, r8, r3
00494e78: asr r2, r2, #3
00494e7c: add r1, r2, r2, lsl #2
00494e80: add r1, r1, r1, lsl #4
00494e84: add r1, r1, r1, lsl #8
00494e88: add r1, r1, r1, lsl #16
00494e8c: add r2, r2, r1, lsl #1
00494e90: cmp r2, #0
00494e94: beq #0x494f74
00494e98: mov sb, #0
00494e9c: mov fp, sb
00494ea0: mov sl, sb
00494ea4: add r8, r8, sb
00494ea8: ldr r2, [r8, #8]
00494eac: ldr r7, [r8, #4]
00494eb0: rsb r3, r7, r2
00494eb4: lsrs r3, r3, #2
00494eb8: beq #0x494ef8
00494ebc: mov r6, #0
00494ec0: ldr r3, [r7, r6, lsl #2]
00494ec4: cmp r3, #0
00494ec8: beq #0x494ee8
00494ecc: mov r0, r3
00494ed0: ldr r3, [r3]
00494ed4: mov lr, pc
00494ed8: ldr pc, [r3, #4]
00494edc: str sl, [r7, r6, lsl #2]
00494ee0: ldr r2, [r8, #8]
00494ee4: ldr r7, [r8, #4]
00494ee8: add r6, r6, #1
00494eec: rsb r3, r7, r2
00494ef0: cmp r6, r3, asr #2
00494ef4: blo #0x494ec0
00494ef8: cmp r2, r7
00494efc: strne r7, [r8, #8]
00494f00: ldr r6, [r8, #0x10]!
00494f04: cmp r6, r8
00494f08: beq #0x494f38
00494f0c: ldr r3, [r6, #8]
00494f10: cmp r3, #0
00494f14: beq #0x494f2c
00494f18: mov r0, r3
00494f1c: ldr r3, [r3]
00494f20: mov lr, pc
00494f24: ldr pc, [r3, #4]
00494f28: str sl, [r6, #8]
00494f2c: ldr r6, [r6]
00494f30: cmp r6, r8
00494f34: bne #0x494f0c
00494f38: mov r0, r6
00494f3c: bl #0x493c04
00494f40: ldr r8, [r4, #0x28]
00494f44: ldr r3, [r4, #0x2c]
00494f48: add fp, fp, #1
00494f4c: add sb, sb, #0x18
00494f50: rsb r2, r8, r3
00494f54: asr r2, r2, #3
00494f58: add r1, r2, r2, lsl #2
00494f5c: add r1, r1, r1, lsl #4
00494f60: add r1, r1, r1, lsl #8
00494f64: add r1, r1, r1, lsl #16
00494f68: add r2, r2, r1, lsl #1
00494f6c: cmp fp, r2
00494f70: blo #0x494ea4
00494f74: ldr r1, [r4, #0x1c]
00494f78: ldr r2, [r4, #0x20]
00494f7c: rsb r0, r1, r2
00494f80: asr r0, r0, #3
00494f84: add ip, r0, r0, lsl #2
00494f88: add ip, ip, ip, lsl #4
00494f8c: add ip, ip, ip, lsl #8
00494f90: add ip, ip, ip, lsl #16
00494f94: add r0, r0, ip, lsl #1
00494f98: cmp r0, #0
00494f9c: beq #0x495050
00494fa0: mov sb, #0
00494fa4: mov fp, sb
00494fa8: mov r7, sb
00494fac: add sl, r1, sb
00494fb0: mov r8, sl
00494fb4: ldr r6, [r8, #0x10]!
00494fb8: cmp r6, r8
00494fbc: beq #0x494fe0
00494fc0: ldr r0, [r6, #8]
00494fc4: cmp r0, #0
00494fc8: beq #0x494fd4
00494fcc: bl #0x310440
00494fd0: str r7, [r6, #8]
00494fd4: ldr r6, [r6]
00494fd8: cmp r6, r8
00494fdc: bne #0x494fc0
00494fe0: ldr r6, [sl, #4]
00494fe4: ldr r3, [sl, #8]
00494fe8: cmp r3, r6
00494fec: beq #0x495014
00494ff0: ldr r0, [r6]
00494ff4: cmp r0, #0
00494ff8: beq #0x495008
00494ffc: bl #0x310440
00495000: str r7, [r6]
00495004: ldr r3, [sl, #8]
00495008: add r6, r6, #4
0049500c: cmp r6, r3
00495010: bne #0x494ff0
00495014: ldr r1, [r4, #0x1c]
00495018: ldr r2, [r4, #0x20]
0049501c: add fp, fp, #1
00495020: add sb, sb, #0x18
00495024: rsb r3, r1, r2
00495028: asr r3, r3, #3
0049502c: add r0, r3, r3, lsl #2
00495030: add r0, r0, r0, lsl #4
00495034: add r0, r0, r0, lsl #8
00495038: add r0, r0, r0, lsl #16
0049503c: add r3, r3, r0, lsl #1
00495040: cmp fp, r3
00495044: blo #0x494fac
00495048: ldr r8, [r4, #0x28]
0049504c: ldr r3, [r4, #0x2c]
00495050: cmp r8, r3
00495054: beq #0x495074
00495058: mov r2, r3
0049505c: mov r1, r8
00495060: add r0, r4, #0x28
00495064: add r3, sp, #4
00495068: bl #0x494dd4
0049506c: ldr r1, [r4, #0x1c]
00495070: ldr r2, [r4, #0x20]
00495074: cmp r1, r2
00495078: beq #0x495088
0049507c: add r0, r4, #0x1c
00495080: mov r3, sp
00495084: bl #0x494910
00495088: mov r0, r5
0049508c: bl #0x493c04
00495090: mov r3, #0
00495094: strb r3, [r4, #4]
00495098: add sp, sp, #0xc
0049509c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN15VisualFXManager14FlushLibrariesEv 004950a0 size4
004950a0: b #0x494e3c

# _ZN15VisualFXManagerD1Ev 004950a4 size140
004950a4: ldr r3, [pc, #0x7c]
004950a8: ldr r2, [pc, #0x7c]
004950ac: push {r4, r5, r6, lr}
004950b0: add r3, pc, r3
004950b4: ldr r2, [r3, r2]
004950b8: mov r5, r0
004950bc: mov r4, r0
004950c0: add r2, r2, #8
004950c4: str r2, [r5], #0x28
004950c8: bl #0x4950a0
004950cc: mov r0, r5
004950d0: bl #0x494058
004950d4: add r0, r4, #0x1c
004950d8: bl #0x4942fc
004950dc: ldr r0, [r4, #0x10]
004950e0: add r3, r4, #0x10
004950e4: cmp r0, #0
004950e8: beq #0x495104
004950ec: ldr r1, [r3, #8]
004950f0: rsb r1, r0, r1
004950f4: bic r1, r1, #3
004950f8: cmp r1, #0x80
004950fc: bhi #0x495114
00495100: bl #0x708f00
00495104: add r0, r4, #8
00495108: bl #0x493c04
0049510c: mov r0, r4
00495110: pop {r4, r5, r6, pc}
00495114: bl #0x310440
00495118: add r0, r4, #8
0049511c: bl #0x493c04
00495120: mov r0, r4
00495124: pop {r4, r5, r6, pc}
00495128: subeq pc, pc, r0, ror #19
0049512c: andeq r3, r0, r0, lsr r4

# _ZN15VisualFXManagerD0Ev 00495130 size28
00495130: push {r4, lr}
00495134: mov r4, r0
00495138: bl #0x4950a4
0049513c: mov r0, r4
00495140: bl #0x310440
00495144: mov r0, r4
00495148: pop {r4, pc}

# _ZN15VisualFXManagerD2Ev 0049514c size140
0049514c: ldr r3, [pc, #0x7c]
00495150: ldr r2, [pc, #0x7c]
00495154: push {r4, r5, r6, lr}
00495158: add r3, pc, r3
0049515c: ldr r2, [r3, r2]
00495160: mov r5, r0
00495164: mov r4, r0
00495168: add r2, r2, #8
0049516c: str r2, [r5], #0x28
00495170: bl #0x4950a0
00495174: mov r0, r5
00495178: bl #0x494058
0049517c: add r0, r4, #0x1c
00495180: bl #0x4942fc
00495184: ldr r0, [r4, #0x10]
00495188: add r3, r4, #0x10
0049518c: cmp r0, #0
00495190: beq #0x4951ac
00495194: ldr r1, [r3, #8]
00495198: rsb r1, r0, r1
0049519c: bic r1, r1, #3
004951a0: cmp r1, #0x80
004951a4: bhi #0x4951bc
004951a8: bl #0x708f00
004951ac: add r0, r4, #8
004951b0: bl #0x493c04
004951b4: mov r0, r4
004951b8: pop {r4, r5, r6, pc}
004951bc: bl #0x310440
004951c0: add r0, r4, #8
004951c4: bl #0x493c04
004951c8: mov r0, r4
004951cc: pop {r4, r5, r6, pc}
004951d0: subeq pc, pc, r8, lsr sb
004951d4: andeq r3, r0, r0, lsr r4

# _ZN15VisualFXManager14AnimatedFXInfoC1ERKS0_ 004951d8 size104
004951d8: push {r4, r5, r6, r7, r8, lr}
004951dc: mov r6, r1
004951e0: ldr r3, [r1], #4
004951e4: mov r5, r0
004951e8: add r4, r5, #0x10
004951ec: str r3, [r0], #4
004951f0: bl #0x493a74
004951f4: str r4, [r5, #0x10]
004951f8: str r4, [r5, #0x14]
004951fc: ldr r7, [r6, #0x10]!
00495200: cmp r7, r6
00495204: beq #0x495238
00495208: mov r0, r4
0049520c: bl #0x494ab4
00495210: ldr r3, [r7, #8]
00495214: str r3, [r0, #8]
00495218: ldr r3, [r4, #4]
0049521c: str r4, [r0]
00495220: str r3, [r0, #4]
00495224: str r0, [r3]
00495228: str r0, [r4, #4]
0049522c: ldr r7, [r7]
00495230: cmp r6, r7
00495234: bne #0x495208
00495238: mov r0, r5
0049523c: pop {r4, r5, r6, r7, r8, pc}

# _ZNSt6vectorIN15VisualFXManager14AnimatedFXInfoESaIS1_EE9push_backERKS1_ 00495240 size292
00495240: push {r4, r5, r6, r7, r8, sb, sl, lr}
00495244: mov r4, r0
00495248: ldr r5, [r4, #8]
0049524c: ldr r0, [r0, #4]
00495250: sub sp, sp, #8
00495254: mov r6, r1
00495258: cmp r0, r5
0049525c: beq #0x495278
00495260: bl #0x4951d8
00495264: ldr r3, [r4, #4]
00495268: add r3, r3, #0x18
0049526c: str r3, [r4, #4]
00495270: add sp, sp, #8
00495274: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00495278: ldr r2, [r4]
0049527c: movw r3, #0xaaaa
00495280: orr r3, r3, r3, lsl #12
00495284: rsb r2, r2, r5
00495288: asr r2, r2, #3
0049528c: add r1, r2, r2, lsl #2
00495290: add r1, r1, r1, lsl #4
00495294: add r1, r1, r1, lsl #8
00495298: add r1, r1, r1, lsl #16
0049529c: add r2, r2, r1, lsl #1
004952a0: cmp r2, #1
004952a4: addhs r1, r2, r2
004952a8: addlo r1, r2, #1
004952ac: cmp r1, r3
004952b0: bls #0x495358
004952b4: movw r1, #0xaaaa
004952b8: orr r1, r1, r1, lsl #12
004952bc: add r2, sp, #8
004952c0: str r1, [r2, #-4]!
004952c4: add r0, r4, #8
004952c8: bl #0x493b7c
004952cc: ldr sb, [r4]
004952d0: mov sl, r0
004952d4: rsb r5, sb, r5
004952d8: asr r3, r5, #3
004952dc: add r5, r3, r3, lsl #2
004952e0: add r5, r5, r5, lsl #4
004952e4: add r5, r5, r5, lsl #8
004952e8: add r5, r5, r5, lsl #16
004952ec: add r5, r3, r5, lsl #1
004952f0: cmp r5, #0
004952f4: movle r5, r0
004952f8: ble #0x495324
004952fc: mov r8, r5
00495300: mov r7, #0
00495304: add r0, sl, r7
00495308: add r1, sb, r7
0049530c: bl #0x4951d8
00495310: subs r8, r8, #1
00495314: add r7, r7, #0x18
00495318: bne #0x495304
0049531c: mov r3, #0x18
00495320: mla r5, r3, r5, sl
00495324: mov r1, r6
00495328: mov r0, r5
0049532c: bl #0x4951d8
00495330: mov r0, r4
00495334: bl #0x493fd4
00495338: ldr r3, [sp, #4]
0049533c: mov r2, #0x18
00495340: add r5, r5, #0x18
00495344: mla r3, r2, r3, sl
00495348: str sl, [r4]
0049534c: str r3, [r4, #8]
00495350: str r5, [r4, #4]
00495354: b #0x495270
00495358: cmp r2, r1
0049535c: bls #0x4952bc
00495360: b #0x4952b4

# _ZN15VisualFXManager16_BuildAnimFXDictEv 00495364 size204
00495364: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00495368: ldr sl, [pc, #0xb4]
0049536c: ldr sb, [pc, #0xb4]
00495370: sub sp, sp, #0x24
00495374: add sl, pc, sl
00495378: ldr r3, [sl, sb]
0049537c: ldr r3, [r3]
00495380: cmp r3, #0
00495384: beq #0x49541c
00495388: ldr r2, [pc, #0x9c]
0049538c: mov r6, #0
00495390: add r7, sp, #8
00495394: str r2, [sp, #4]
00495398: add fp, r0, #0x28
0049539c: mov r4, r6
004953a0: mov r5, r6
004953a4: add r8, r7, #0x10
004953a8: b #0x4953f0
004953ac: ldr r2, [sp, #4]
004953b0: ldr r3, [sl, r2]
004953b4: ldr r3, [r3]
004953b8: add r3, r3, r6
004953bc: ldr r3, [r3, #8]
004953c0: str r3, [sp, #8]
004953c4: mov r1, r7
004953c8: mov r0, fp
004953cc: bl #0x495240
004953d0: mov r0, r7
004953d4: bl #0x493f88
004953d8: ldr r3, [sl, sb]
004953dc: add r4, r4, #1
004953e0: add r6, r6, #0xc
004953e4: ldr r3, [r3]
004953e8: cmp r3, r4
004953ec: bls #0x49541c
004953f0: cmp r4, #0
004953f4: str r5, [sp, #0xc]
004953f8: str r5, [sp, #0x10]
004953fc: str r5, [sp, #0x14]
00495400: str r8, [sp, #0x18]
00495404: str r8, [sp, #0x1c]
00495408: blt #0x495414
0049540c: cmp r3, r4
00495410: bgt #0x4953ac
00495414: str r5, [sp, #8]
00495418: b #0x4953c4
0049541c: add sp, sp, #0x24
00495420: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00495424: subeq pc, pc, ip, lsl r7
00495428: andeq r0, r0, r8, lsl #23
0049542c: strheq r1, [r0], -r8

# _ZN15VisualFXManager10GrabAnimFXEiP10GameObject 00495430 size648
00495430: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00495434: ldr r4, [pc, #0x260]
00495438: ldr r5, [pc, #0x260]
0049543c: ldr ip, [pc, #0x260]
00495440: add r4, pc, r4
00495444: ldr r3, [r4, r5]
00495448: ldr sl, [r4, ip]
0049544c: sub sp, sp, #0x84
00495450: ldr r3, [r3]
00495454: mov r8, r0
00495458: mov r0, sl
0049545c: str r3, [sp, #0x7c]
00495460: mov r7, r1
00495464: mov sb, r2
00495468: bl #0x337888
0049546c: ldr r1, [pc, #0x234]
00495470: add r6, sp, #0x64
00495474: add r2, sp, #0x60
00495478: add r1, pc, r1
0049547c: mov r0, r6
00495480: bl #0x3140ec
00495484: mov r0, sl
00495488: mov r1, r6
0049548c: bl #0x337ec8
00495490: mov sl, r0
00495494: ldr r0, [sp, #0x78]
00495498: cmp r0, r6
0049549c: beq #0x4954bc
004954a0: cmp r0, #0
004954a4: beq #0x4954bc
004954a8: ldr r1, [sp, #0x64]
004954ac: rsb r1, r0, r1
004954b0: cmp r1, #0x80
004954b4: bhi #0x4954e8
004954b8: bl #0x708f00
004954bc: cmp sl, #0
004954c0: bne #0x4954f4
004954c4: mov r6, #0
004954c8: ldr r3, [r4, r5]
004954cc: ldr r2, [sp, #0x7c]
004954d0: mov r0, r6
004954d4: ldr r3, [r3]
004954d8: cmp r2, r3
004954dc: bne #0x495698
004954e0: add sp, sp, #0x84
004954e4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004954e8: bl #0x310440
004954ec: cmp sl, #0
004954f0: beq #0x4954c4
004954f4: cmp r7, #0
004954f8: blt #0x4954c4
004954fc: ldr r3, [pc, #0x1a8]
00495500: ldr r3, [r4, r3]
00495504: ldr r3, [r3]
00495508: cmp r7, r3
0049550c: bge #0x4954c4
00495510: mov sl, #0x18
00495514: ldr fp, [r8, #0x1c]
00495518: mul sl, sl, r7
0049551c: mov r0, r8
00495520: ldr r3, [fp, sl]
00495524: add r1, fp, sl
00495528: str r1, [sp, #0x14]
0049552c: ldr r3, [r3, #0x10]
00495530: ldr r1, [r3, #4]
00495534: bl #0x494ad4
00495538: subs r6, r0, #0
0049553c: beq #0x4954c8
00495540: ldr r2, [fp, sl]
00495544: ldr r3, [r2, #0x14]
00495548: cmp r3, #2
0049554c: movne r3, #0
00495550: beq #0x495684
00495554: ldr r1, [pc, #0x154]
00495558: mov ip, #0
0049555c: ldr r2, [r2, #8]
00495560: ldr r0, [r4, r1]
00495564: mov r1, r7
00495568: add r7, sp, #0x1c
0049556c: ldr lr, [r0, #4]
00495570: ldr fp, [r0, #8]
00495574: ldr sl, [r0]
00495578: str ip, [sp, #4]
0049557c: add ip, sp, #0x54
00495580: str ip, [sp, #8]
00495584: mov r0, r8
00495588: add ip, sp, #0x48
0049558c: str lr, [sp, #0x4c]
00495590: str ip, [sp, #0xc]
00495594: str lr, [sp, #0x58]
00495598: str sl, [sp, #0x48]
0049559c: str sl, [sp, #0x54]
004955a0: str fp, [sp, #0x50]
004955a4: str fp, [sp, #0x5c]
004955a8: str sb, [sp]
004955ac: bl #0x4935b8
004955b0: ldr r1, [sp, #0x14]
004955b4: mov sl, r0
004955b8: mov r0, r7
004955bc: bl #0x493924
004955c0: mov r1, r8
004955c4: mov r3, sl
004955c8: mov r2, r7
004955cc: add r0, sp, #0x34
004955d0: bl #0x4933e4
004955d4: ldr r2, [sp, #0x14]
004955d8: mov r0, r7
004955dc: add r7, r2, #0x10
004955e0: bl #0x4940d8
004955e4: mov r0, r7
004955e8: bl #0x493814
004955ec: str sl, [r0, #8]
004955f0: ldr r1, [sp, #0x14]
004955f4: mov r3, r0
004955f8: ldr r2, [r1, #0x14]
004955fc: str r7, [r0]
00495600: mov r0, r6
00495604: str r2, [r3, #4]
00495608: str r3, [r2]
0049560c: str r3, [r1, #0x14]
00495610: str sb, [r6, #0x28]
00495614: mov r1, #1
00495618: bl #0x492aa0
0049561c: mov r0, r6
00495620: mov r1, #1
00495624: bl #0x492aa0
00495628: mov r0, r6
0049562c: mov r1, #1
00495630: bl #0x492694
00495634: mov r0, r6
00495638: bl #0x492744
0049563c: ldr lr, [sp, #0x38]
00495640: ldr r0, [pc, #0x6c]
00495644: ldrb r1, [sp, #0x34]
00495648: str lr, [sp]
0049564c: mvn lr, #0
00495650: ldr ip, [r4, r0]
00495654: str lr, [sp, #4]
00495658: ldr lr, [sp, #0x44]
0049565c: mov r0, r6
00495660: ldrb r2, [sp, #0x35]
00495664: ldrb r3, [sp, #0x36]
00495668: str lr, [sp, #8]
0049566c: str ip, [sp, #0xc]
00495670: bl #0x492e8c
00495674: mov r0, r6
00495678: mov r1, #1
0049567c: bl #0x492ef0
00495680: b #0x4954c8
00495684: ldr r0, [r2, #0xc]
00495688: bl #0x493780
0049568c: ldr r2, [fp, sl]
00495690: mov r3, r0
00495694: b #0x495554
00495698: bl #0x30e310
0049569c: subeq pc, pc, r0, asr r6
004956a0: andeq r4, r0, ip, lsr #1
004956a4: andeq r0, r0, r4, lsl #17

# _ZN15VisualFXManager10PlayAnimFXEiRK7Point3DIfES3_PK10GameObjectPNS_10AnimFXDataE 004956b8 size464
004956b8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004956bc: ldr r4, [pc, #0x1b0]
004956c0: ldr r7, [pc, #0x1b0]
004956c4: ldr lr, [pc, #0x1b0]
004956c8: add r4, pc, r4
004956cc: ldr ip, [r4, r7]
004956d0: ldr r5, [r4, lr]
004956d4: sub sp, sp, #0x3c
004956d8: ldr ip, [ip]
004956dc: str r0, [sp, #0x10]
004956e0: mov r0, r5
004956e4: str r3, [sp, #0x14]
004956e8: str ip, [sp, #0x34]
004956ec: mov sb, r1
004956f0: mov r8, r2
004956f4: ldr fp, [sp, #0x60]
004956f8: ldr r6, [sp, #0x64]
004956fc: bl #0x337888
00495700: ldr r1, [pc, #0x178]
00495704: add sl, sp, #0x1c
00495708: add r2, sp, #0x18
0049570c: add r1, pc, r1
00495710: mov r0, sl
00495714: bl #0x3140ec
00495718: mov r0, r5
0049571c: mov r1, sl
00495720: bl #0x337ec8
00495724: mov r5, r0
00495728: ldr r0, [sp, #0x30]
0049572c: cmp r0, sl
00495730: beq #0x495750
00495734: cmp r0, #0
00495738: beq #0x495750
0049573c: ldr r1, [sp, #0x1c]
00495740: rsb r1, r0, r1
00495744: cmp r1, #0x80
00495748: bhi #0x495834
0049574c: bl #0x708f00
00495750: cmp r5, #0
00495754: bne #0x495774
00495758: ldr r3, [r4, r7]
0049575c: ldr r2, [sp, #0x34]
00495760: ldr r3, [r3]
00495764: cmp r2, r3
00495768: bne #0x495870
0049576c: add sp, sp, #0x3c
00495770: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00495774: ldr r0, [sp, #0x10]
00495778: mov r1, sb
0049577c: bl #0x494ad4
00495780: subs r5, r0, #0
00495784: beq #0x495758
00495788: ldr r2, [r8, #4]
0049578c: ldr r3, [r8, #8]
00495790: ldr r1, [r8]
00495794: str r2, [r5, #0x38]
00495798: str r3, [r5, #0x3c]
0049579c: str r1, [r5, #0x34]
004957a0: mov r1, #0
004957a4: bl #0x492aa0
004957a8: mov r0, r5
004957ac: ldr r1, [sp, #0x14]
004957b0: bl #0x492f3c
004957b4: mov r0, r5
004957b8: mov r1, #1
004957bc: bl #0x492694
004957c0: mov r0, r5
004957c4: bl #0x492744
004957c8: cmp r6, #0
004957cc: beq #0x49583c
004957d0: ldr r0, [pc, #0xac]
004957d4: ldr sl, [r6, #4]
004957d8: ldr ip, [r6, #8]
004957dc: ldr lr, [r6, #0x10]
004957e0: ldr r8, [r4, r0]
004957e4: ldrb r3, [r6, #2]
004957e8: ldrb r1, [r6]
004957ec: ldrb r2, [r6, #1]
004957f0: mov r0, r5
004957f4: str sl, [sp]
004957f8: stmib sp, {ip, lr}
004957fc: str r8, [sp, #0xc]
00495800: bl #0x492e8c
00495804: ldr r3, [r6, #0xc]
00495808: str r3, [r5, #0x1c]
0049580c: cmp fp, #0
00495810: beq #0x495824
00495814: str fp, [r5, #0x28]
00495818: mov r0, r5
0049581c: mov r1, #1
00495820: bl #0x492aa0
00495824: mov r0, r5
00495828: mov r1, #1
0049582c: bl #0x492ef0
00495830: b #0x495758
00495834: bl #0x310440
00495838: b #0x495750
0049583c: ldr r3, [pc, #0x40]
00495840: mov r1, #1
00495844: mov ip, #0x3f800000
00495848: ldr lr, [r4, r3]
0049584c: mov r2, r6
00495850: mov r0, r5
00495854: mov r3, r1
00495858: str ip, [sp]
0049585c: str lr, [sp, #0xc]
00495860: str r6, [sp, #4]
00495864: str r6, [sp, #8]
00495868: bl #0x492e8c
0049586c: b #0x49580c
00495870: bl #0x30e310
00495874: subeq pc, pc, r8, asr #7
00495878: andeq r4, r0, ip, lsr #1
0049587c: andeq r0, r0, r4, lsl #17
00495880: subeq pc, r3, ip, lsr sb
00495884: andeq r4, r0, r8, lsr ip

# _ZN15VisualFXManager13PlayAnimFXSetEiRK7Point3DIfES3_PK10GameObjectPNS_13AnimFXSetDataE 00495888 size512
00495888: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0049588c: ldr ip, [pc, #0x1ec]
00495890: cmp r1, #0
00495894: sub sp, sp, #0x74
00495898: add ip, pc, ip
0049589c: mov r6, r0
004958a0: mov r5, r2
004958a4: mov r4, r3
004958a8: blt #0x495a18
004958ac: ldr r3, [pc, #0x1d0]
004958b0: ldr r3, [ip, r3]
004958b4: ldr r3, [r3]
004958b8: cmp r1, r3
004958bc: bge #0x495a18
004958c0: mov fp, #0x18
004958c4: mul fp, fp, r1
004958c8: ldr sb, [r0, #0x1c]
004958cc: ldr r2, [sb, fp]
004958d0: add r7, sb, fp
004958d4: ldr r3, [r2, #0x14]
004958d8: cmp r3, #2
004958dc: beq #0x495a54
004958e0: mov r0, #0
004958e4: str r0, [sp, #0x18]
004958e8: str r0, [sp, #0x24]
004958ec: mov r3, r0
004958f0: ldr ip, [r5, #4]
004958f4: ldr r2, [r2, #8]
004958f8: add r8, sp, #0x2c
004958fc: str ip, [sp, #0x14]
00495900: ldr r0, [r4, #4]
00495904: ldr lr, [r5, #8]
00495908: ldr sl, [r4]
0049590c: str r0, [sp, #0x1c]
00495910: ldr ip, [r4, #8]
00495914: mov r0, r6
00495918: str ip, [sp, #0x20]
0049591c: ldr ip, [r5]
00495920: str lr, [sp, #0x6c]
00495924: str sl, [sp, #0x58]
00495928: str ip, [sp, #0x64]
0049592c: ldr ip, [sp, #0x14]
00495930: str ip, [sp, #0x68]
00495934: ldr ip, [sp, #0x1c]
00495938: str ip, [sp, #0x5c]
0049593c: ldr ip, [sp, #0x20]
00495940: str ip, [sp, #0x60]
00495944: add ip, sp, #0x64
00495948: str ip, [sp, #8]
0049594c: add ip, sp, #0x58
00495950: str ip, [sp, #0xc]
00495954: ldr ip, [sp, #0x98]
00495958: str ip, [sp]
0049595c: ldr ip, [sp, #0x9c]
00495960: str ip, [sp, #4]
00495964: bl #0x4935b8
00495968: add r3, sp, #0x44
0049596c: mov sl, r0
00495970: mov r1, r7
00495974: mov r0, r8
00495978: str r3, [sp, #0x14]
0049597c: bl #0x493924
00495980: mov r2, r8
00495984: mov r3, sl
00495988: mov r1, r6
0049598c: ldr r0, [sp, #0x14]
00495990: bl #0x4933e4
00495994: mov r0, r8
00495998: add r8, r7, #0x10
0049599c: bl #0x4940d8
004959a0: mov r0, r8
004959a4: bl #0x493814
004959a8: str sl, [r0, #8]
004959ac: ldr r3, [r7, #0x14]
004959b0: str r8, [r0]
004959b4: str r3, [r0, #4]
004959b8: str r0, [r3]
004959bc: str r0, [r7, #0x14]
004959c0: ldr r3, [sb, fp]
004959c4: ldr ip, [sp, #0x18]
004959c8: ldr r2, [r3, #0x10]
004959cc: add r3, r2, ip
004959d0: ldr r3, [r3, #4]
004959d4: cmn r3, #1
004959d8: beq #0x495a20
004959dc: ldr r3, [r7, #4]
004959e0: ldr r0, [sp, #0x24]
004959e4: ldr r3, [r3, r0, lsl #2]
004959e8: ldrb r1, [r3]
004959ec: cmp r1, #0
004959f0: beq #0x495a20
004959f4: ldr ip, [sp, #0x98]
004959f8: ldr r1, [r3, #4]
004959fc: mov r0, r6
00495a00: str ip, [sp]
00495a04: ldr ip, [sp, #0x9c]
00495a08: mov r2, r5
00495a0c: mov r3, r4
00495a10: str ip, [sp, #4]
00495a14: bl #0x495888
00495a18: add sp, sp, #0x74
00495a1c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00495a20: ldr r3, [sl, #4]
00495a24: mov r1, #0x30
00495a28: ldr ip, [sp, #0x98]
00495a2c: mla r3, r1, r3, r2
00495a30: mov r0, r6
00495a34: ldr r1, [r3, #4]
00495a38: str ip, [sp]
00495a3c: ldr ip, [sp, #0x14]
00495a40: mov r2, r5
00495a44: mov r3, r4
00495a48: str ip, [sp, #4]
00495a4c: bl #0x4956b8
00495a50: b #0x495a18
00495a54: ldr r0, [r2, #0xc]
00495a58: str r1, [sp, #0x10]
00495a5c: bl #0x493780
00495a60: mov r2, #0x30
00495a64: mul r2, r2, r0
00495a68: mov r3, r0
00495a6c: str r2, [sp, #0x18]
00495a70: ldr r2, [sb, fp]
00495a74: ldr r1, [sp, #0x10]
00495a78: str r0, [sp, #0x24]
00495a7c: b #0x4958f0

# _ZN15VisualFXManager17PreCacheLibrariesEv 00495a88 size204
00495a88: push {r4, r5, r6, r7, r8, lr}
00495a8c: ldr r4, [pc, #0xb0]
00495a90: ldr r6, [pc, #0xb0]
00495a94: ldr r2, [pc, #0xb0]
00495a98: add r4, pc, r4
00495a9c: ldr r3, [r4, r6]
00495aa0: ldr r7, [r4, r2]
00495aa4: sub sp, sp, #0x20
00495aa8: ldr r3, [r3]
00495aac: mov r8, r0
00495ab0: mov r0, r7
00495ab4: str r3, [sp, #0x1c]
00495ab8: bl #0x337888
00495abc: ldr r1, [pc, #0x8c]
00495ac0: add r5, sp, #4
00495ac4: mov r2, sp
00495ac8: add r1, pc, r1
00495acc: mov r0, r5
00495ad0: bl #0x3140ec
00495ad4: mov r0, r7
00495ad8: mov r1, r5
00495adc: bl #0x337ec8
00495ae0: mov r7, r0
00495ae4: ldr r0, [sp, #0x18]
00495ae8: cmp r0, r5
00495aec: beq #0x495b0c
00495af0: cmp r0, #0
00495af4: beq #0x495b0c
00495af8: ldr r1, [sp, #4]
00495afc: rsb r1, r0, r1
00495b00: cmp r1, #0x80
00495b04: bhi #0x495b38
00495b08: bl #0x708f00
00495b0c: cmp r7, #0
00495b10: beq #0x495b1c
00495b14: mov r0, r8
00495b18: bl #0x4933d8
00495b1c: ldr r3, [r4, r6]
00495b20: ldr r2, [sp, #0x1c]
00495b24: ldr r3, [r3]
00495b28: cmp r2, r3
00495b2c: bne #0x495b40
00495b30: add sp, sp, #0x20
00495b34: pop {r4, r5, r6, r7, r8, pc}
00495b38: bl #0x310440
00495b3c: b #0x495b0c
00495b40: bl #0x30e310
00495b44: strdeq lr, pc, [pc], #-0xf8
00495b48: andeq r4, r0, ip, lsr #1
00495b4c: andeq r0, r0, r4, lsl #17
00495b50: subeq pc, r3, r0, lsl #11

# _ZN15VisualFXManager10PlayAnimFXEiRK7Point3DIfEPK10GameObjectPNS_10AnimFXDataE 00495b54 size448
00495b54: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00495b58: ldr r4, [pc, #0x1a0]
00495b5c: ldr r7, [pc, #0x1a0]
00495b60: ldr lr, [pc, #0x1a0]
00495b64: add r4, pc, r4
00495b68: ldr ip, [r4, r7]
00495b6c: ldr r6, [r4, lr]
00495b70: sub sp, sp, #0x3c
00495b74: ldr ip, [ip]
00495b78: str r0, [sp, #0x14]
00495b7c: mov r0, r6
00495b80: mov fp, r3
00495b84: str ip, [sp, #0x34]
00495b88: mov sb, r1
00495b8c: mov r8, r2
00495b90: ldr r5, [sp, #0x60]
00495b94: bl #0x337888
00495b98: ldr r1, [pc, #0x16c]
00495b9c: add sl, sp, #0x1c
00495ba0: add r2, sp, #0x18
00495ba4: add r1, pc, r1
00495ba8: mov r0, sl
00495bac: bl #0x3140ec
00495bb0: mov r0, r6
00495bb4: mov r1, sl
00495bb8: bl #0x337ec8
00495bbc: mov r6, r0
00495bc0: ldr r0, [sp, #0x30]
00495bc4: cmp r0, sl
00495bc8: beq #0x495be8
00495bcc: cmp r0, #0
00495bd0: beq #0x495be8
00495bd4: ldr r1, [sp, #0x1c]
00495bd8: rsb r1, r0, r1
00495bdc: cmp r1, #0x80
00495be0: bhi #0x495cc0
00495be4: bl #0x708f00
00495be8: cmp r6, #0
00495bec: bne #0x495c0c
00495bf0: ldr r3, [r4, r7]
00495bf4: ldr r2, [sp, #0x34]
00495bf8: ldr r3, [r3]
00495bfc: cmp r2, r3
00495c00: bne #0x495cfc
00495c04: add sp, sp, #0x3c
00495c08: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00495c0c: ldr r0, [sp, #0x14]
00495c10: mov r1, sb
00495c14: bl #0x494ad4
00495c18: subs r6, r0, #0
00495c1c: beq #0x495bf0
00495c20: ldr r2, [r8, #4]
00495c24: ldr r3, [r8, #8]
00495c28: ldr r1, [r8]
00495c2c: str r2, [r6, #0x38]
00495c30: str r3, [r6, #0x3c]
00495c34: str r1, [r6, #0x34]
00495c38: mov r1, #0
00495c3c: bl #0x492aa0
00495c40: mov r0, r6
00495c44: mov r1, #1
00495c48: bl #0x492694
00495c4c: mov r0, r6
00495c50: bl #0x492744
00495c54: cmp r5, #0
00495c58: beq #0x495cc8
00495c5c: ldr r0, [pc, #0xac]
00495c60: ldr sl, [r5, #4]
00495c64: ldr ip, [r5, #8]
00495c68: ldr lr, [r5, #0x10]
00495c6c: ldr r8, [r4, r0]
00495c70: ldrb r3, [r5, #2]
00495c74: ldrb r1, [r5]
00495c78: ldrb r2, [r5, #1]
00495c7c: mov r0, r6
00495c80: str sl, [sp]
00495c84: stmib sp, {ip, lr}
00495c88: str r8, [sp, #0xc]
00495c8c: bl #0x492e8c
00495c90: ldr r3, [r5, #0xc]
00495c94: str r3, [r6, #0x1c]
00495c98: cmp fp, #0
00495c9c: beq #0x495cb0
00495ca0: str fp, [r6, #0x28]
00495ca4: mov r0, r6
00495ca8: mov r1, #1
00495cac: bl #0x492aa0
00495cb0: mov r0, r6
00495cb4: mov r1, #1
00495cb8: bl #0x492ef0
00495cbc: b #0x495bf0
00495cc0: bl #0x310440
00495cc4: b #0x495be8
00495cc8: ldr r3, [pc, #0x40]
00495ccc: mov r1, #1
00495cd0: mov ip, #0x3f800000
00495cd4: ldr lr, [r4, r3]
00495cd8: mov r2, r5
00495cdc: mov r0, r6
00495ce0: mov r3, r1
00495ce4: str ip, [sp]
00495ce8: str lr, [sp, #0xc]
00495cec: str r5, [sp, #4]
00495cf0: str r5, [sp, #8]
00495cf4: bl #0x492e8c
00495cf8: b #0x495c98
00495cfc: bl #0x30e310
00495d00: subeq lr, pc, ip, lsr #30
00495d04: andeq r4, r0, ip, lsr #1
00495d08: andeq r0, r0, r4, lsl #17
00495d0c: subeq pc, r3, r4, lsr #9
00495d10: andeq r4, r0, r8, lsr ip

# _ZN15VisualFXManager13PlayAnimFXSetEiRK7Point3DIfEPK10GameObjectPNS_13AnimFXSetDataE 00495d14 size496
00495d14: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00495d18: ldr r7, [pc, #0x1d8]
00495d1c: cmp r1, #0
00495d20: sub sp, sp, #0x74
00495d24: add r7, pc, r7
00495d28: mov r5, r0
00495d2c: mov r4, r2
00495d30: mov fp, r3
00495d34: blt #0x495e98
00495d38: ldr r3, [pc, #0x1bc]
00495d3c: ldr r3, [r7, r3]
00495d40: ldr r3, [r3]
00495d44: cmp r1, r3
00495d48: bge #0x495e98
00495d4c: mov sb, #0x18
00495d50: mul sb, sb, r1
00495d54: ldr sl, [r0, #0x1c]
00495d58: ldr r2, [sl, sb]
00495d5c: add r6, sl, sb
00495d60: ldr r3, [r2, #0x14]
00495d64: cmp r3, #2
00495d68: beq #0x495ecc
00495d6c: mov r0, #0
00495d70: str r0, [sp, #0x18]
00495d74: str r0, [sp, #0x24]
00495d78: mov r3, r0
00495d7c: ldr r0, [pc, #0x17c]
00495d80: ldr ip, [r4, #4]
00495d84: ldr r2, [r2, #8]
00495d88: ldr r0, [r7, r0]
00495d8c: ldr r7, [r4, #8]
00495d90: ldr lr, [r0, #8]
00495d94: ldr r8, [r0]
00495d98: ldr r0, [r0, #4]
00495d9c: str r7, [sp, #0x14]
00495da0: str lr, [sp, #0x20]
00495da4: str r0, [sp, #0x1c]
00495da8: ldr lr, [r4]
00495dac: str ip, [sp, #0x68]
00495db0: ldr ip, [sp, #0x14]
00495db4: str lr, [sp, #0x64]
00495db8: ldr lr, [sp, #0x1c]
00495dbc: str ip, [sp, #0x6c]
00495dc0: ldr ip, [sp, #0x20]
00495dc4: mov r0, r5
00495dc8: str r8, [sp, #0x58]
00495dcc: str ip, [sp, #0x60]
00495dd0: ldr ip, [sp, #0x98]
00495dd4: str lr, [sp, #0x5c]
00495dd8: stm sp, {fp, ip}
00495ddc: add ip, sp, #0x64
00495de0: str ip, [sp, #8]
00495de4: add ip, sp, #0x58
00495de8: str ip, [sp, #0xc]
00495dec: bl #0x4935b8
00495df0: add r7, sp, #0x2c
00495df4: add lr, sp, #0x44
00495df8: mov r8, r0
00495dfc: mov r1, r6
00495e00: mov r0, r7
00495e04: str lr, [sp, #0x14]
00495e08: bl #0x493924
00495e0c: mov r2, r7
00495e10: mov r3, r8
00495e14: mov r1, r5
00495e18: ldr r0, [sp, #0x14]
00495e1c: bl #0x4933e4
00495e20: mov r0, r7
00495e24: add r7, r6, #0x10
00495e28: bl #0x4940d8
00495e2c: mov r0, r7
00495e30: bl #0x493814
00495e34: str r8, [r0, #8]
00495e38: ldr r3, [r6, #0x14]
00495e3c: str r7, [r0]
00495e40: str r3, [r0, #4]
00495e44: str r0, [r3]
00495e48: str r0, [r6, #0x14]
00495e4c: ldr r3, [sl, sb]
00495e50: ldr r0, [sp, #0x18]
00495e54: ldr r2, [r3, #0x10]
00495e58: add r3, r2, r0
00495e5c: ldr r3, [r3, #4]
00495e60: cmn r3, #1
00495e64: beq #0x495ea0
00495e68: ldr r1, [sp, #0x24]
00495e6c: ldr r3, [r6, #4]
00495e70: ldr r3, [r3, r1, lsl #2]
00495e74: ldrb r1, [r3]
00495e78: cmp r1, #0
00495e7c: beq #0x495ea0
00495e80: ldr r1, [r3, #4]
00495e84: mov r0, r5
00495e88: mov r2, r4
00495e8c: mov r3, fp
00495e90: str r8, [sp]
00495e94: bl #0x495d14
00495e98: add sp, sp, #0x74
00495e9c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00495ea0: ldr r3, [r8, #4]
00495ea4: mov r1, #0x30
00495ea8: ldr r7, [sp, #0x14]
00495eac: mla r3, r1, r3, r2
00495eb0: mov r0, r5
00495eb4: ldr r1, [r3, #4]
00495eb8: mov r2, r4
00495ebc: mov r3, fp
00495ec0: str r7, [sp]
00495ec4: bl #0x495b54
00495ec8: b #0x495e98
00495ecc: ldr r0, [r2, #0xc]
00495ed0: str r1, [sp, #0x10]
00495ed4: bl #0x493780
00495ed8: mov r2, #0x30
00495edc: mul r2, r2, r0
00495ee0: mov r3, r0
00495ee4: str r2, [sp, #0x18]
00495ee8: ldr r2, [sl, sb]
00495eec: ldr r1, [sp, #0x10]
00495ef0: str r0, [sp, #0x24]
00495ef4: b #0x495d7c
00495ef8: subeq lr, pc, ip, ror #26
00495efc: andeq r0, r0, r4, asr #13
00495f00: andeq r3, r0, ip, lsr #30

# _ZN15VisualFXManager13PlayAnimFXSetEiPK10GameObjectPNS_13AnimFXSetDataE 00495f04 size56
00495f04: str lr, [sp, #-4]!
00495f08: ldr ip, [pc, #0x24]
00495f0c: mov lr, r2
00495f10: ldr r2, [pc, #0x20]
00495f14: sub sp, sp, #0xc
00495f18: add ip, pc, ip
00495f1c: str r3, [sp]
00495f20: ldr r2, [ip, r2]
00495f24: mov r3, lr
00495f28: bl #0x495d14
00495f2c: add sp, sp, #0xc
00495f30: ldm sp!, {pc}
00495f34: subeq lr, pc, r8, ror fp
00495f38: andeq r3, r0, ip, lsr #30

# _ZN15VisualFXManager10PlayAnimFXEiPK10GameObjectPNS_10AnimFXDataE 00495f3c size56
00495f3c: str lr, [sp, #-4]!
00495f40: ldr ip, [pc, #0x24]
00495f44: mov lr, r2
00495f48: ldr r2, [pc, #0x20]
00495f4c: sub sp, sp, #0xc
00495f50: add ip, pc, ip
00495f54: str r3, [sp]
00495f58: ldr r2, [ip, r2]
00495f5c: mov r3, lr
00495f60: bl #0x495b54
00495f64: add sp, sp, #0xc
00495f68: ldm sp!, {pc}
00495f6c: subeq lr, pc, r0, asr #22
00495f70: andeq r3, r0, ip, lsr #30

# _ZN15VisualFXManager14PlayAnimFXStepEPNS_13AnimFXSetDataENS_10AnimFXDataE 00495f74 size464
00495f74: sub sp, sp, #8
00495f78: push {r4, r5, r6, r7, r8, lr}
00495f7c: mov r5, r0
00495f80: mov r4, r1
00495f84: ldr r0, [r1, #8]
00495f88: ldr r1, [r5, #0x1c]
00495f8c: mov r6, #0x18
00495f90: sub sp, sp, #8
00495f94: mla r6, r6, r0, r1
00495f98: str r3, [sp, #0x24]
00495f9c: str r2, [sp, #0x20]
00495fa0: ldr r1, [r4, #4]
00495fa4: ldr r2, [r6, #4]
00495fa8: ldr r3, [pc, #0x18c]
00495fac: ldr r2, [r2, r1, lsl #2]
00495fb0: add r3, pc, r3
00495fb4: ldrb r2, [r2]
00495fb8: cmp r2, #0
00495fbc: beq #0x496020
00495fc0: ldr r2, [pc, #0x178]
00495fc4: add r7, r4, #0x1c
00495fc8: mov r0, r7
00495fcc: ldr r8, [r3, r2]
00495fd0: mov r1, r8
00495fd4: bl #0x312b6c
00495fd8: cmp r0, #0
00495fdc: beq #0x4960bc
00495fe0: add r7, r4, #0x10
00495fe4: mov r1, r8
00495fe8: mov r0, r7
00495fec: bl #0x312b6c
00495ff0: cmp r0, #0
00495ff4: bne #0x4960ec
00495ff8: ldr r2, [r6, #4]
00495ffc: ldr r1, [r4, #4]
00496000: ldr r3, [r4, #0x28]
00496004: mov r0, r5
00496008: ldr r1, [r2, r1, lsl #2]
0049600c: mov r2, r7
00496010: ldr r1, [r1, #4]
00496014: str r4, [sp]
00496018: bl #0x495d14
0049601c: b #0x4960ac
00496020: ldr r2, [pc, #0x118]
00496024: add r7, r4, #0x1c
00496028: mov r0, r7
0049602c: ldr r8, [r3, r2]
00496030: mov r1, r8
00496034: bl #0x312b6c
00496038: cmp r0, #0
0049603c: beq #0x49607c
00496040: add r7, r4, #0x10
00496044: mov r1, r8
00496048: mov r0, r7
0049604c: bl #0x312b6c
00496050: cmp r0, #0
00496054: beq #0x496110
00496058: ldr r3, [r6, #4]
0049605c: ldr r1, [r4, #4]
00496060: mov r0, r5
00496064: ldr r2, [r4, #0x28]
00496068: ldr r1, [r3, r1, lsl #2]
0049606c: add r3, sp, #0x20
00496070: ldr r1, [r1, #4]
00496074: bl #0x495f3c
00496078: b #0x4960ac
0049607c: ldr r3, [r6, #4]
00496080: ldr r2, [r4, #4]
00496084: ldr ip, [r4, #0x28]
00496088: mov r0, r5
0049608c: ldr r1, [r3, r2, lsl #2]
00496090: add r2, r4, #0x10
00496094: mov r3, r7
00496098: ldr r1, [r1, #4]
0049609c: str ip, [sp]
004960a0: add ip, sp, #0x20
004960a4: str ip, [sp, #4]
004960a8: bl #0x4956b8
004960ac: add sp, sp, #8
004960b0: pop {r4, r5, r6, r7, r8, lr}
004960b4: add sp, sp, #8
004960b8: bx lr
004960bc: ldr r3, [r6, #4]
004960c0: ldr r2, [r4, #4]
004960c4: ldr ip, [r4, #0x28]
004960c8: mov r0, r5
004960cc: ldr r1, [r3, r2, lsl #2]
004960d0: add r2, r4, #0x10
004960d4: mov r3, r7
004960d8: ldr r1, [r1, #4]
004960dc: str ip, [sp]
004960e0: str r4, [sp, #4]
004960e4: bl #0x495888
004960e8: b #0x4960ac
004960ec: ldr r2, [r6, #4]
004960f0: ldr r1, [r4, #4]
004960f4: mov r0, r5
004960f8: mov r3, r4
004960fc: ldr r1, [r2, r1, lsl #2]
00496100: ldr r2, [r4, #0x28]
00496104: ldr r1, [r1, #4]
00496108: bl #0x495f04
0049610c: b #0x4960ac
00496110: ldr r2, [r6, #4]
00496114: ldr r1, [r4, #4]
00496118: ldr r3, [r4, #0x28]
0049611c: add ip, sp, #0x20
00496120: ldr r1, [r2, r1, lsl #2]
00496124: mov r0, r5
00496128: mov r2, r7
0049612c: ldr r1, [r1, #4]
00496130: str ip, [sp]
00496134: bl #0x495b54
00496138: b #0x4960ac
0049613c: subeq lr, pc, r0, ror #21
00496140: andeq r3, r0, ip, lsr #30

# _ZN15VisualFXManager15_HandleSequenceEP10AnimatedFXPNS_13AnimFXSetDataEb 00496144 size544
00496144: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00496148: sub sp, sp, #0x7c
0049614c: str r1, [sp, #0x14]
00496150: cmp r3, #0
00496154: mov r8, r2
00496158: ldrne r2, [r2, #0x2c]
0049615c: ldreq r7, [r8, #8]
00496160: mov r6, r0
00496164: ldrne r7, [r2, #8]
00496168: mov r2, #0x18
0049616c: mul r7, r2, r7
00496170: ldr r2, [r0, #0x1c]
00496174: ldr r1, [r2, r7]
00496178: add r7, r2, r7
0049617c: ldr r2, [r1, #0x14]
00496180: cmp r2, #1
00496184: beq #0x4961e0
00496188: cmp r3, #0
0049618c: beq #0x4961bc
00496190: ldr r3, [r8, #0x2c]
00496194: ldr r2, [r3, #0x2c]
00496198: cmp r2, #0
0049619c: beq #0x4961b4
004961a0: mov r3, #1
004961a4: strb r3, [r2]
004961a8: ldr r1, [sp, #0x14]
004961ac: ldr r2, [r8, #0x2c]
004961b0: bl #0x496144
004961b4: add sp, sp, #0x7c
004961b8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004961bc: ldr r2, [r8, #0x2c]
004961c0: cmp r2, #0
004961c4: beq #0x4961b4
004961c8: mov r3, #1
004961cc: strb r3, [r2]
004961d0: ldr r1, [sp, #0x14]
004961d4: mov r2, r8
004961d8: bl #0x496144
004961dc: b #0x4961b4
004961e0: add fp, sp, #0x6c
004961e4: add r2, sp, #4
004961e8: add r3, fp, #4
004961ec: str r2, [sp, #0x1c]
004961f0: str r3, [sp, #0x20]
004961f4: add ip, sp, #0x34
004961f8: mov sl, r7
004961fc: ldr r5, [sl, #0x10]!
00496200: add r2, r2, #4
00496204: str ip, [sp, #0x18]
00496208: add r3, r3, #4
0049620c: add ip, sp, #0x4c
00496210: str r8, [sp, #0x2c]
00496214: add sb, sp, #0x64
00496218: str r2, [sp, #0x28]
0049621c: str ip, [sp, #0x24]
00496220: mov r8, r3
00496224: cmp r5, sl
00496228: beq #0x4961b4
0049622c: ldr r4, [r5, #8]
00496230: cmp r4, #0
00496234: beq #0x4962f8
00496238: ldrb r3, [r4]
0049623c: cmp r3, #0
00496240: beq #0x4962f8
00496244: mov r2, #0
00496248: strb r2, [r4]
0049624c: ldr r2, [r7]
00496250: ldr r3, [r4, #4]
00496254: ldr r2, [r2, #0xc]
00496258: add r3, r3, #1
0049625c: cmp r3, r2
00496260: blt #0x496328
00496264: ldr r3, [r4, #0xc]
00496268: cmp r3, #0
0049626c: movle r2, #0
00496270: movgt r2, #1
00496274: cmn r3, #1
00496278: movne r1, #0
0049627c: moveq r1, #1
00496280: orrs r1, r2, r1
00496284: beq #0x496300
00496288: cmp r2, #0
0049628c: subne r3, r3, #1
00496290: mov lr, #0
00496294: strne r3, [r4, #0xc]
00496298: str lr, [r4, #4]
0049629c: mov r1, r7
004962a0: ldr r0, [sp, #0x18]
004962a4: bl #0x493924
004962a8: ldr r2, [sp, #0x18]
004962ac: mov r0, sb
004962b0: mov r1, r6
004962b4: mov r3, r4
004962b8: bl #0x4933e4
004962bc: ldr r0, [sp, #0x18]
004962c0: bl #0x4940d8
004962c4: ldr r2, [sp, #0x20]
004962c8: ldr ip, [fp]
004962cc: ldr lr, [r2]
004962d0: ldm sb, {r2, r3}
004962d4: str ip, [sp]
004962d8: ldr ip, [sp, #0x1c]
004962dc: mov r1, r4
004962e0: mov r0, r6
004962e4: str lr, [ip]
004962e8: ldr lr, [r8]
004962ec: ldr ip, [sp, #0x28]
004962f0: str lr, [ip]
004962f4: bl #0x495f74
004962f8: ldr r5, [r5]
004962fc: b #0x496224
00496300: ldr r3, [r4, #0x2c]
00496304: cmp r3, #0
00496308: beq #0x4962f8
0049630c: mov r0, r6
00496310: ldr r1, [sp, #0x14]
00496314: ldr r2, [sp, #0x2c]
00496318: mov r3, #1
0049631c: bl #0x496144
00496320: ldr r5, [r5]
00496324: b #0x496224
00496328: str r3, [r4, #4]
0049632c: mov r1, r7
00496330: ldr r0, [sp, #0x24]
00496334: bl #0x493924
00496338: mov r3, r4
0049633c: mov r0, sb
00496340: mov r1, r6
00496344: ldr r2, [sp, #0x24]
00496348: bl #0x4933e4
0049634c: ldr r0, [sp, #0x24]
00496350: bl #0x4940d8
00496354: ldr r3, [sp, #0x20]
00496358: ldr ip, [fp]
0049635c: ldr lr, [r3]
00496360: b #0x4962d0

# _ZN15VisualFXManager18_HandleEndOfLoopCBEP10AnimatedFXPNS_13AnimFXSetDataE 00496364 size108
00496364: push {r4, r5, r6}
00496368: ldr r5, [r2, #8]
0049636c: mov r4, #0x18
00496370: ldr ip, [r0, #0x1c]
00496374: mul r5, r4, r5
00496378: ldr r5, [ip, r5]
0049637c: ldr r5, [r5, #0x14]
00496380: cmp r5, #1
00496384: beq #0x4963c4
00496388: ldr r3, [r2, #0x2c]
0049638c: cmp r3, #0
00496390: beq #0x4963b4
00496394: ldr r5, [r3, #8]
00496398: mov r6, #1
0049639c: strb r6, [r3]
004963a0: mul r4, r4, r5
004963a4: ldr r3, [ip, r4]
004963a8: ldr r3, [r3, #0x14]
004963ac: cmp r3, r6
004963b0: beq #0x4963bc
004963b4: pop {r4, r5, r6}
004963b8: bx lr
004963bc: pop {r4, r5, r6}
004963c0: b #0x496144
004963c4: mov r3, #0
004963c8: pop {r4, r5, r6}
004963cc: b #0x496144

# _ZN15VisualFXManager16__Anim_EndOfLoopEP10AnimatedFXPNS_13AnimFXSetDataE 004963d0 size100
004963d0: push {r4, r5, r6, lr}
004963d4: ldr r4, [pc, #0x50]
004963d8: subs r2, r1, #0
004963dc: mov r5, r0
004963e0: add r4, pc, r4
004963e4: beq #0x496424
004963e8: ldr r6, [pc, #0x40]
004963ec: mov r1, r0
004963f0: ldr r0, [r4, r6]
004963f4: bl #0x496364
004963f8: ldr r4, [r4, r6]
004963fc: add r6, r4, #8
00496400: mov r0, r6
00496404: bl #0x494ab4
00496408: str r5, [r0, #8]
0049640c: ldr r3, [r4, #0xc]
00496410: str r6, [r0]
00496414: str r3, [r0, #4]
00496418: str r0, [r3]
0049641c: str r0, [r4, #0xc]
00496420: pop {r4, r5, r6, pc}
00496424: ldr r6, [pc, #4]
00496428: b #0x4963f8
0049642c: strheq lr, [pc], #-0x60
00496430: andeq r1, r0, r8, lsl #22

# _ZN15VisualFXManager16RegisterFXToLoadEi 00496434 size352
00496434: push {r4, r5, r6, r7, r8, sl, lr}
00496438: ldr r4, [pc, #0x13c]
0049643c: ldr r6, [pc, #0x13c]
00496440: ldr r8, [pc, #0x13c]
00496444: add r4, pc, r4
00496448: ldr r3, [r4, r6]
0049644c: ldr r7, [r4, r8]
00496450: sub sp, sp, #0x4c
00496454: ldr r3, [r3]
00496458: mov sl, r0
0049645c: mov r0, r7
00496460: str r3, [sp, #0x44]
00496464: str r1, [sp, #4]
00496468: bl #0x337888
0049646c: ldr r1, [pc, #0x114]
00496470: add r5, sp, #0x2c
00496474: add r2, sp, #0x10
00496478: add r1, pc, r1
0049647c: mov r0, r5
00496480: bl #0x3140ec
00496484: mov r0, r7
00496488: mov r1, r5
0049648c: bl #0x337ec8
00496490: mov r7, r0
00496494: ldr r0, [sp, #0x40]
00496498: cmp r0, r5
0049649c: beq #0x4964bc
004964a0: cmp r0, #0
004964a4: beq #0x4964bc
004964a8: ldr r1, [sp, #0x2c]
004964ac: rsb r1, r0, r1
004964b0: cmp r1, #0x80
004964b4: bhi #0x496500
004964b8: bl #0x708f00
004964bc: cmp r7, #0
004964c0: beq #0x4964e4
004964c4: ldr r3, [sp, #4]
004964c8: cmp r3, #0
004964cc: blt #0x4964e4
004964d0: ldr r2, [pc, #0xb4]
004964d4: ldr r2, [r4, r2]
004964d8: ldr r2, [r2]
004964dc: cmp r3, r2
004964e0: blt #0x496508
004964e4: ldr r3, [r4, r6]
004964e8: ldr r2, [sp, #0x44]
004964ec: ldr r3, [r3]
004964f0: cmp r2, r3
004964f4: bne #0x496578
004964f8: add sp, sp, #0x4c
004964fc: pop {r4, r5, r6, r7, r8, sl, pc}
00496500: bl #0x310440
00496504: b #0x4964bc
00496508: ldr r7, [r4, r8]
0049650c: add r5, sp, #0x14
00496510: mov r0, r7
00496514: bl #0x337888
00496518: ldr r1, [pc, #0x70]
0049651c: add r2, sp, #0xc
00496520: mov r0, r5
00496524: add r1, pc, r1
00496528: bl #0x3140ec
0049652c: mov r0, r7
00496530: mov r1, r5
00496534: bl #0x337a88
00496538: ldr r0, [sp, #0x28]
0049653c: cmp r0, r5
00496540: beq #0x496560
00496544: cmp r0, #0
00496548: beq #0x496560
0049654c: ldr r1, [sp, #0x14]
00496550: rsb r1, r0, r1
00496554: cmp r1, #0x80
00496558: bhi #0x496570
0049655c: bl #0x708f00
00496560: add r0, sl, #0x10
00496564: add r1, sp, #4
00496568: bl #0x49437c
0049656c: b #0x4964e4
00496570: bl #0x310440
00496574: b #0x496560
00496578: bl #0x30e310
0049657c: subeq lr, pc, ip, asr #12
00496580: andeq r4, r0, ip, lsr #1
00496584: andeq r0, r0, r4, lsl #17
00496588: ldrdeq lr, pc, [r3], #-0xb0
0049658c: andeq r0, r0, r8, lsl #23
00496590: subeq lr, r3, r4, lsr fp

# _ZN15VisualFXManager6UpdateEv 00496594 size596
00496594: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00496598: ldr r6, [pc, #0x230]
0049659c: ldr r8, [pc, #0x230]
004965a0: ldr r2, [pc, #0x230]
004965a4: add r6, pc, r6
004965a8: ldr r3, [r6, r8]
004965ac: ldr r7, [r6, r2]
004965b0: sub sp, sp, #0x2c
004965b4: ldr r3, [r3]
004965b8: mov r5, r0
004965bc: mov r0, r7
004965c0: str r3, [sp, #0x24]
004965c4: bl #0x337888
004965c8: ldr r1, [pc, #0x20c]
004965cc: add r4, sp, #0xc
004965d0: add r2, sp, #8
004965d4: add r1, pc, r1
004965d8: mov r0, r4
004965dc: bl #0x3140ec
004965e0: mov r0, r7
004965e4: mov r1, r4
004965e8: bl #0x337ec8
004965ec: mov r7, r0
004965f0: ldr r0, [sp, #0x20]
004965f4: cmp r0, r4
004965f8: beq #0x496618
004965fc: cmp r0, #0
00496600: beq #0x496618
00496604: ldr r1, [sp, #0xc]
00496608: rsb r1, r0, r1
0049660c: cmp r1, #0x80
00496610: bhi #0x4967c4
00496614: bl #0x708f00
00496618: cmp r7, #0
0049661c: bne #0x49663c
00496620: ldr r3, [r6, r8]
00496624: ldr r2, [sp, #0x24]
00496628: ldr r3, [r3]
0049662c: cmp r2, r3
00496630: bne #0x4967cc
00496634: add sp, sp, #0x2c
00496638: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0049663c: ldr r0, [pc, #0x19c]
00496640: add r0, pc, r0
00496644: bl #0x3136b4
00496648: ldr r7, [r5, #0x28]
0049664c: ldr r3, [r5, #0x2c]
00496650: rsb r3, r7, r3
00496654: asr r3, r3, #3
00496658: add r2, r3, r3, lsl #2
0049665c: add r2, r2, r2, lsl #4
00496660: add r2, r2, r2, lsl #8
00496664: add r2, r2, r2, lsl #16
00496668: add r3, r3, r2, lsl #1
0049666c: cmp r3, #0
00496670: beq #0x4966d0
00496674: mov sl, #0
00496678: mov sb, sl
0049667c: add r7, r7, sl
00496680: ldr r4, [r7, #0x10]!
00496684: b #0x496694
00496688: ldr r0, [r4, #8]
0049668c: bl #0x492f68
00496690: ldr r4, [r4]
00496694: cmp r7, r4
00496698: bne #0x496688
0049669c: ldr r7, [r5, #0x28]
004966a0: ldr r3, [r5, #0x2c]
004966a4: add sb, sb, #1
004966a8: add sl, sl, #0x18
004966ac: rsb r3, r7, r3
004966b0: asr r3, r3, #3
004966b4: add r2, r3, r3, lsl #2
004966b8: add r2, r2, r2, lsl #4
004966bc: add r2, r2, r2, lsl #8
004966c0: add r2, r2, r2, lsl #16
004966c4: add r3, r3, r2, lsl #1
004966c8: cmp sb, r3
004966cc: blo #0x49667c
004966d0: mov r7, r5
004966d4: ldr r4, [r7, #8]!
004966d8: cmp r4, r7
004966dc: addne sl, sp, #4
004966e0: beq #0x4967b4
004966e4: cmp r7, r4
004966e8: beq #0x4967b4
004966ec: ldr r3, [r4, #8]
004966f0: mov r0, r3
004966f4: str r3, [sp, #4]
004966f8: bl #0x49267c
004966fc: ldr r3, [r0]
00496700: mov lr, pc
00496704: ldr pc, [r3, #0x44]
00496708: ldr r0, [sp, #4]
0049670c: ldr r3, [r0, #0x2c]
00496710: ldr fp, [r3, #8]
00496714: bl #0x49267c
00496718: ldr r3, [r0]
0049671c: mov lr, pc
00496720: ldr pc, [r3, #0x44]
00496724: ldr sb, [r0, #0x14]
00496728: ldr r0, [sp, #4]
0049672c: bl #0x49267c
00496730: ldr r3, [r0]
00496734: mov lr, pc
00496738: ldr pc, [r3, #0x44]
0049673c: ldr r3, [r0, #4]
00496740: cmp sb, r3
00496744: beq #0x496750
00496748: ldr r0, [sp, #4]
0049674c: bl #0x4926e4
00496750: ldr r0, [sp, #4]
00496754: bl #0x49267c
00496758: mov r2, sb
0049675c: ldr r3, [r0]
00496760: mov r1, fp
00496764: mov lr, pc
00496768: ldr pc, [r3, #0x10]
0049676c: ldr r3, [sp, #4]
00496770: ldrb r3, [r3, #0x24]
00496774: cmp r3, #0
00496778: ldrne sb, [r4]
0049677c: bne #0x4967a8
00496780: ldr sb, [r4]
00496784: ldr r3, [r4, #4]
00496788: mov r0, r4
0049678c: mov r1, #0xc
00496790: str sb, [r3]
00496794: str r3, [sb, #4]
00496798: bl #0x708f00
0049679c: mov r0, r5
004967a0: mov r1, sl
004967a4: bl #0x494978
004967a8: mov r4, sb
004967ac: cmp r7, r4
004967b0: bne #0x4966ec
004967b4: ldr r0, [pc, #0x28]
004967b8: add r0, pc, r0
004967bc: bl #0x3136b8
004967c0: b #0x496620
004967c4: bl #0x310440
004967c8: b #0x496618
004967cc: bl #0x30e310
004967d0: subeq lr, pc, ip, ror #9
004967d4: andeq r4, r0, ip, lsr #1
004967d8: andeq r0, r0, r4, lsl #17
004967dc: subeq lr, r3, r4, ror sl
004967e0: subeq lr, r3, r0, lsr sl
004967e4: strheq lr, [r3], #-0x88

# _ZN15VisualFXManager19RegisterFXSetToLoadEi 004967e8 size364
004967e8: push {r4, r5, r6, r7, r8, sl, lr}
004967ec: ldr r4, [pc, #0x148]
004967f0: ldr r7, [pc, #0x148]
004967f4: ldr r2, [pc, #0x148]
004967f8: add r4, pc, r4
004967fc: ldr r3, [r4, r7]
00496800: ldr r8, [r4, r2]
00496804: sub sp, sp, #0x24
00496808: ldr r3, [r3]
0049680c: mov r5, r0
00496810: mov r0, r8
00496814: str r3, [sp, #0x1c]
00496818: mov sl, r1
0049681c: bl #0x337888
00496820: ldr r1, [pc, #0x120]
00496824: add r6, sp, #4
00496828: mov r2, sp
0049682c: add r1, pc, r1
00496830: mov r0, r6
00496834: bl #0x3140ec
00496838: mov r0, r8
0049683c: mov r1, r6
00496840: bl #0x337ec8
00496844: mov r8, r0
00496848: ldr r0, [sp, #0x18]
0049684c: cmp r0, r6
00496850: beq #0x496870
00496854: cmp r0, #0
00496858: beq #0x496870
0049685c: ldr r1, [sp, #4]
00496860: rsb r1, r0, r1
00496864: cmp r1, #0x80
00496868: bhi #0x496930
0049686c: bl #0x708f00
00496870: cmp r8, #0
00496874: beq #0x496914
00496878: cmp sl, #0
0049687c: blt #0x496914
00496880: ldr r3, [pc, #0xc4]
00496884: ldr r3, [r4, r3]
00496888: ldr r3, [r3]
0049688c: cmp sl, r3
00496890: bge #0x496914
00496894: ldr r3, [pc, #0xb4]
00496898: mov r2, #0x18
0049689c: ldr r3, [r4, r3]
004968a0: ldr r3, [r3]
004968a4: mla sl, r2, sl, r3
004968a8: ldr r3, [sl, #0xc]
004968ac: cmp r3, #0
004968b0: ble #0x496914
004968b4: mov r6, #0
004968b8: mov r8, r6
004968bc: b #0x4968e0
004968c0: ldr r1, [r3, #4]
004968c4: mov r0, r5
004968c8: bl #0x496434
004968cc: ldr r3, [sl, #0xc]
004968d0: add r8, r8, #1
004968d4: add r6, r6, #0x30
004968d8: cmp r3, r8
004968dc: ble #0x496914
004968e0: ldr r3, [sl, #0x10]
004968e4: add r3, r3, r6
004968e8: ldr r2, [r3, #0x1c]
004968ec: cmp r2, #1
004968f0: bne #0x4968c0
004968f4: ldr r1, [r3, #4]
004968f8: mov r0, r5
004968fc: bl #0x4967e8
00496900: ldr r3, [sl, #0xc]
00496904: add r8, r8, #1
00496908: add r6, r6, #0x30
0049690c: cmp r3, r8
00496910: bgt #0x4968e0
00496914: ldr r3, [r4, r7]
00496918: ldr r2, [sp, #0x1c]
0049691c: ldr r3, [r3]
00496920: cmp r2, r3
00496924: bne #0x496938
00496928: add sp, sp, #0x24
0049692c: pop {r4, r5, r6, r7, r8, sl, pc}
00496930: bl #0x310440
00496934: b #0x496870
00496938: bl #0x30e310
0049693c: umaaleq lr, pc, r8, r2
00496940: andeq r4, r0, ip, lsr #1
00496944: andeq r0, r0, r4, lsl #17
00496948: subeq lr, r3, ip, lsl r8
0049694c: andeq r0, r0, r4, asr #13
00496950: andeq r3, r0, r0, ror sb

# _ZN15VisualFXManager16_BuildAnimFXSetsEv 00496954 size592
00496954: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00496958: ldr r1, [pc, #0x238]
0049695c: ldr r2, [r0, #0x1c]
00496960: ldr r3, [r0, #0x20]
00496964: sub sp, sp, #0x4c
00496968: add r1, pc, r1
0049696c: cmp r2, r3
00496970: str r1, [sp, #4]
00496974: mov sl, r0
00496978: beq #0x496984
0049697c: add sp, sp, #0x4c
00496980: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00496984: ldr r2, [pc, #0x210]
00496988: ldr r3, [r1, r2]
0049698c: str r2, [sp, #0x14]
00496990: ldr r3, [r3]
00496994: cmp r3, #0
00496998: beq #0x49697c
0049699c: add r3, sp, #0x2c
004969a0: str r3, [sp, #8]
004969a4: ldr r3, [pc, #0x1f4]
004969a8: ldr r2, [sp, #8]
004969ac: mov sb, #0
004969b0: ldr r3, [r1, r3]
004969b4: add r1, r0, #0x1c
004969b8: str r1, [sp, #0x18]
004969bc: ldr r1, [sp, #8]
004969c0: add r2, r2, #0x10
004969c4: str r2, [sp, #0x10]
004969c8: add r1, r1, #4
004969cc: add r2, sp, #0x44
004969d0: str r3, [sp, #0x1c]
004969d4: mov r7, sb
004969d8: mov r8, #1
004969dc: str r1, [sp, #0x20]
004969e0: str r2, [sp, #0x24]
004969e4: str r0, [sp, #0xc]
004969e8: mov sl, sb
004969ec: ldr r3, [sp, #0x1c]
004969f0: ldr r1, [sp, #0x10]
004969f4: str r7, [sp, #0x30]
004969f8: ldr r5, [r3]
004969fc: str r7, [sp, #0x34]
00496a00: str r7, [sp, #0x38]
00496a04: add r5, r5, sb
00496a08: str r1, [sp, #0x3c]
00496a0c: str r1, [sp, #0x40]
00496a10: str r5, [sp, #0x2c]
00496a14: ldr r1, [r5, #0xc]
00496a18: cmp r1, #0
00496a1c: beq #0x496ab4
00496a20: mov r4, r7
00496a24: mov r6, r7
00496a28: ldr r3, [r5, #0x10]
00496a2c: add r3, r3, r4
00496a30: ldr r2, [r3, #4]
00496a34: cmn r2, #1
00496a38: beq #0x496aa4
00496a3c: ldr fp, [r3, #0x1c]
00496a40: cmp fp, #0
00496a44: bne #0x496b08
00496a48: mov r1, fp
00496a4c: mov r0, #8
00496a50: bl #0x310570
00496a54: str r0, [sp, #0x44]
00496a58: strb fp, [r0]
00496a5c: ldr r3, [r5, #0x10]
00496a60: add r3, r3, r4
00496a64: ldr r2, [r3, #4]
00496a68: ldr r3, [sp, #0x44]
00496a6c: str r2, [r3, #4]
00496a70: ldr r1, [sp, #0x34]
00496a74: ldr r3, [sp, #0x38]
00496a78: cmp r1, r3
00496a7c: beq #0x496b78
00496a80: ldr r3, [sp, #0x44]
00496a84: str r3, [r1]
00496a88: ldr r3, [sp, #0x34]
00496a8c: add r3, r3, #4
00496a90: str r3, [sp, #0x34]
00496a94: ldrb r3, [r5, #4]
00496a98: cmp r3, #0
00496a9c: bne #0x496aec
00496aa0: ldr r1, [r5, #0xc]
00496aa4: add r6, r6, #1
00496aa8: cmp r1, r6
00496aac: add r4, r4, #0x30
00496ab0: bhi #0x496a28
00496ab4: ldr r1, [sp, #8]
00496ab8: ldr r0, [sp, #0x18]
00496abc: bl #0x4941d8
00496ac0: ldr r0, [sp, #8]
00496ac4: bl #0x4940d8
00496ac8: ldr r1, [sp, #4]
00496acc: ldr r2, [sp, #0x14]
00496ad0: add sl, sl, #1
00496ad4: add sb, sb, #0x18
00496ad8: ldr r3, [r1, r2]
00496adc: ldr r3, [r3]
00496ae0: cmp r3, sl
00496ae4: bhi #0x4969ec
00496ae8: b #0x49697c
00496aec: ldr r3, [r5, #0x10]
00496af0: ldr r0, [sp, #0xc]
00496af4: add r3, r3, r4
00496af8: ldr r1, [r3, #4]
00496afc: bl #0x496434
00496b00: ldr r1, [r5, #0xc]
00496b04: b #0x496aa4
00496b08: mov r1, #0
00496b0c: mov r0, #8
00496b10: bl #0x310570
00496b14: str r0, [sp, #0x44]
00496b18: strb r8, [r0]
00496b1c: ldr r3, [r5, #0x10]
00496b20: add r3, r3, r4
00496b24: ldr r2, [r3, #4]
00496b28: ldr r3, [sp, #0x44]
00496b2c: str r2, [r3, #4]
00496b30: ldr r1, [sp, #0x34]
00496b34: ldr r3, [sp, #0x38]
00496b38: cmp r1, r3
00496b3c: beq #0x496b88
00496b40: ldr r3, [sp, #0x44]
00496b44: str r3, [r1]
00496b48: ldr r3, [sp, #0x34]
00496b4c: add r3, r3, #4
00496b50: str r3, [sp, #0x34]
00496b54: ldrb r3, [r5, #4]
00496b58: cmp r3, #0
00496b5c: beq #0x496aa0
00496b60: ldr r3, [r5, #0x10]
00496b64: ldr r0, [sp, #0xc]
00496b68: add r3, r3, r4
00496b6c: ldr r1, [r3, #4]
00496b70: bl #0x4967e8
00496b74: b #0x496aa0
00496b78: ldr r0, [sp, #0x20]
00496b7c: ldr r2, [sp, #0x24]
00496b80: bl #0x49448c
00496b84: b #0x496a94
00496b88: ldr r0, [sp, #0x20]
00496b8c: ldr r2, [sp, #0x24]
00496b90: bl #0x49448c
00496b94: b #0x496b54
00496b98: subeq lr, pc, r8, lsr #2
00496b9c: andeq r0, r0, r4, asr #13
00496ba0: andeq r3, r0, r0, ror sb

# _ZN15VisualFXManager17_BuildAnimLibraryEv 00496ba4 size52
00496ba4: push {r4, lr}
00496ba8: ldr r2, [r0, #0x28]
00496bac: ldr r3, [r0, #0x2c]
00496bb0: mov r4, r0
00496bb4: cmp r2, r3
00496bb8: beq #0x496bc0
00496bbc: pop {r4, pc}
00496bc0: mov r3, #1
00496bc4: strb r3, [r0, #4]
00496bc8: bl #0x495364
00496bcc: mov r0, r4
00496bd0: pop {r4, lr}
00496bd4: b #0x496954

# _ZN15VisualFXManager14BuildLibrariesEv 00496bd8 size204
00496bd8: push {r4, r5, r6, r7, r8, lr}
00496bdc: ldr r4, [pc, #0xb0]
00496be0: ldr r6, [pc, #0xb0]
00496be4: ldr r2, [pc, #0xb0]
00496be8: add r4, pc, r4
00496bec: ldr r3, [r4, r6]
00496bf0: ldr r7, [r4, r2]
00496bf4: sub sp, sp, #0x20
00496bf8: ldr r3, [r3]
00496bfc: mov r8, r0
00496c00: mov r0, r7
00496c04: str r3, [sp, #0x1c]
00496c08: bl #0x337888
00496c0c: ldr r1, [pc, #0x8c]
00496c10: add r5, sp, #4
00496c14: mov r2, sp
00496c18: add r1, pc, r1
00496c1c: mov r0, r5
00496c20: bl #0x3140ec
00496c24: mov r0, r7
00496c28: mov r1, r5
00496c2c: bl #0x337ec8
00496c30: mov r7, r0
00496c34: ldr r0, [sp, #0x18]
00496c38: cmp r0, r5
00496c3c: beq #0x496c5c
00496c40: cmp r0, #0
00496c44: beq #0x496c5c
00496c48: ldr r1, [sp, #4]
00496c4c: rsb r1, r0, r1
00496c50: cmp r1, #0x80
00496c54: bhi #0x496c88
00496c58: bl #0x708f00
00496c5c: cmp r7, #0
00496c60: beq #0x496c6c
00496c64: mov r0, r8
00496c68: bl #0x496ba4
00496c6c: ldr r3, [r4, r6]
00496c70: ldr r2, [sp, #0x1c]
00496c74: ldr r3, [r3]
00496c78: cmp r2, r3
00496c7c: bne #0x496c90
00496c80: add sp, sp, #0x20
00496c84: pop {r4, r5, r6, r7, r8, pc}
00496c88: bl #0x310440
00496c8c: b #0x496c5c
00496c90: bl #0x30e310
00496c94: subeq sp, pc, r8, lsr #29
00496c98: andeq r4, r0, ip, lsr #1
00496c9c: andeq r0, r0, r4, lsl #17
00496ca0: subeq lr, r3, r0, lsr r4

# _GLOBAL__I_.._.._sources_Game_SWFAnim_SWFAnim.cpp 00496ca4 size32
00496ca4: ldr r3, [pc, #0x14]
00496ca8: mov r2, #0x3f000000
00496cac: add r3, pc, r3
00496cb0: str r2, [r3, #8]
00496cb4: str r2, [r3]
00496cb8: str r2, [r3, #4]
00496cbc: bx lr

# _ZN7SWFAnim10SetVisibleEb 00496cc4 size24
00496cc4: push {r4, lr}
00496cc8: add r0, r0, #0xc
00496ccc: mov r4, r1
00496cd0: bl #0x427d50
00496cd4: strb r4, [r0, #0x9b]
00496cd8: pop {r4, pc}

# _ZN7SWFAnim9IsVisibleEv 00496cdc size20
00496cdc: push {r4, lr}
00496ce0: add r0, r0, #0xc
00496ce4: bl #0x427d50
00496ce8: mov r0, #1
00496cec: pop {r4, pc}

# _ZN7SWFAnim13IsAnimPlayingEv 00496cf0 size36
00496cf0: push {r4, lr}
00496cf4: add r0, r0, #0xc
00496cf8: bl #0x427d50
00496cfc: ldr r3, [r0]
00496d00: mov lr, pc
00496d04: ldr pc, [r3, #0x98]
00496d08: subs r0, r0, #1
00496d0c: movne r0, #1
00496d10: pop {r4, pc}

# _ZN7SWFAnim8PlayAnimEPKc 00496d14 size84
00496d14: push {r4, r5, r6, lr}
00496d18: add r4, r0, #0xc
00496d1c: ldr r6, [r0, #8]
00496d20: mov r0, r4
00496d24: mov r5, r1
00496d28: bl #0x427d50
00496d2c: mov r2, r5
00496d30: mov r1, r0
00496d34: mov r3, #0
00496d38: mov r0, r6
00496d3c: bl #0x7aba04
00496d40: cmp r0, #0
00496d44: beq #0x496d64
00496d48: mov r0, r4
00496d4c: bl #0x427d50
00496d50: mov r1, #0
00496d54: ldr r3, [r0]
00496d58: mov lr, pc
00496d5c: ldr pc, [r3, #0x94]
00496d60: mov r0, #1
00496d64: pop {r4, r5, r6, pc}

# _ZN7SWFAnim11SetPositionEii 00496d68 size52
00496d68: push {r4, r5, r6, lr}
00496d6c: mov r3, r0
00496d70: add r0, r0, #0xc
00496d74: mov r5, r1
00496d78: mov r4, r2
00496d7c: ldr r6, [r3, #8]
00496d80: bl #0x427d50
00496d84: mov r2, r5
00496d88: mov r1, r0
00496d8c: mov r3, r4
00496d90: mov r0, r6
00496d94: pop {r4, r5, r6, lr}
00496d98: b #0x7aa3f0

# _ZN7SWFAnim13SetPosition3DERK7Point3DIfE 00496d9c size204
00496d9c: push {r4, r5, r6, r7, r8, sl, lr}
00496da0: ldr lr, [r1]
00496da4: ldr ip, [r1, #4]
00496da8: ldr r2, [r1, #8]
00496dac: sub sp, sp, #0x1c
00496db0: mov r4, r0
00496db4: add r1, sp, #0x10
00496db8: mov r3, #0
00496dbc: add r5, r4, #0xc
00496dc0: add r0, sp, #4
00496dc4: str r2, [sp, #0xc]
00496dc8: str lr, [sp, #4]
00496dcc: str ip, [sp, #8]
00496dd0: str r3, [sp, #0x14]
00496dd4: str r3, [sp, #0x10]
00496dd8: bl #0x50e830
00496ddc: mov r0, r5
00496de0: ldr sl, [sp, #0x10]
00496de4: ldr r7, [sp, #0x14]
00496de8: bl #0x427d50
00496dec: ldr r3, [r0]
00496df0: mov lr, pc
00496df4: ldr pc, [r3, #0x54]
00496df8: bl #0x416538
00496dfc: mov r8, r0
00496e00: mov r0, r5
00496e04: bl #0x427d50
00496e08: ldr r3, [r0]
00496e0c: mov lr, pc
00496e10: ldr pc, [r3, #0x54]
00496e14: bl #0x416578
00496e18: mov r6, r0
00496e1c: mov r0, sl
00496e20: bl #0x30e964
00496e24: mov r1, r0
00496e28: mov r0, r8
00496e2c: bl #0x30ed6c
00496e30: bl #0x30e4cc
00496e34: mov r5, r0
00496e38: mov r0, r7
00496e3c: bl #0x30e964
00496e40: mov r1, r0
00496e44: mov r0, r6
00496e48: bl #0x30ed6c
00496e4c: bl #0x30e4cc
00496e50: mov r1, r5
00496e54: mov r2, r0
00496e58: mov r0, r4
00496e5c: bl #0x496d68
00496e60: add sp, sp, #0x1c
00496e64: pop {r4, r5, r6, r7, r8, sl, pc}

# _ZN7SWFAnimD1Ev 00496e68 size60
00496e68: ldr r3, [pc, #0x2c]
00496e6c: ldr r2, [pc, #0x2c]
00496e70: push {r4, lr}
00496e74: add r3, pc, r3
00496e78: ldr r2, [r3, r2]
00496e7c: mov r4, r0
00496e80: add r2, r2, #8
00496e84: str r2, [r0], #0x3c
00496e88: bl #0x41a9fc
00496e8c: add r0, r4, #0xc
00496e90: bl #0x41a9fc
00496e94: mov r0, r4
00496e98: pop {r4, pc}
00496e9c: subeq sp, pc, ip, lsl ip
00496ea0: ldrdeq r1, r2, [r0], -r0

# _ZN7SWFAnimD0Ev 00496ea4 size28
00496ea4: push {r4, lr}
00496ea8: mov r4, r0
00496eac: bl #0x496e68
00496eb0: mov r0, r4
00496eb4: bl #0x310440
00496eb8: mov r0, r4
00496ebc: pop {r4, pc}

# _ZN7SWFAnimD2Ev 00496ec0 size60
00496ec0: ldr r3, [pc, #0x2c]
00496ec4: ldr r2, [pc, #0x2c]
00496ec8: push {r4, lr}
00496ecc: add r3, pc, r3
00496ed0: ldr r2, [r3, r2]
00496ed4: mov r4, r0
00496ed8: add r2, r2, #8
00496edc: str r2, [r0], #0x3c
00496ee0: bl #0x41a9fc
00496ee4: add r0, r4, #0xc
00496ee8: bl #0x41a9fc
00496eec: mov r0, r4
00496ef0: pop {r4, pc}
00496ef4: subeq sp, pc, r4, asr #23
00496ef8: ldrdeq r1, r2, [r0], -r0

# _ZN7SWFAnim7SetTextEPKc 00496efc size128
00496efc: push {r4, r5, r6, lr}
00496f00: ldr r3, [r0, #0x68]
00496f04: mov r5, r0
00496f08: mov r4, r1
00496f0c: cmp r3, #0
00496f10: beq #0x496f4c
00496f14: ldr r0, [r0, #0x64]
00496f18: ldrb r3, [r0, #4]
00496f1c: cmp r3, #0
00496f20: beq #0x496f50
00496f24: add r0, r5, #0x3c
00496f28: ldr r5, [r5, #8]
00496f2c: bl #0x427d50
00496f30: ldr r2, [pc, #0x40]
00496f34: mov r1, r0
00496f38: mov r3, r4
00496f3c: mov r0, r5
00496f40: add r2, pc, r2
00496f44: pop {r4, r5, r6, lr}
00496f48: b #0x7a947c
00496f4c: pop {r4, r5, r6, pc}
00496f50: ldr r1, [r0]
00496f54: sub r1, r1, #1
00496f58: cmp r1, #0
00496f5c: str r1, [r0]
00496f60: bne #0x496f68
00496f64: bl #0x752b38
00496f68: mov r3, #0
00496f6c: str r3, [r5, #0x68]
00496f70: str r3, [r5, #0x64]
00496f74: pop {r4, r5, r6, pc}
00496f78: strheq r7, [r5], #-0xe0

# _ZN7SWFAnim8BindTextEPKc 00496f7c size120
00496f7c: push {r4, r5, r6, lr}
00496f80: ldr r3, [r0, #0x68]
00496f84: mov r4, r0
00496f88: mov r5, r1
00496f8c: cmp r3, #0
00496f90: beq #0x496fc8
00496f94: ldr r0, [r0, #0x64]
00496f98: ldrb r3, [r0, #4]
00496f9c: cmp r3, #0
00496fa0: beq #0x496fa8
00496fa4: pop {r4, r5, r6, pc}
00496fa8: ldr r1, [r0]
00496fac: sub r1, r1, #1
00496fb0: cmp r1, #0
00496fb4: str r1, [r0]
00496fb8: beq #0x496fec
00496fbc: mov r3, #0
00496fc0: str r3, [r4, #0x68]
00496fc4: str r3, [r4, #0x64]
00496fc8: add r0, r4, #0xc
00496fcc: ldr r6, [r4, #8]
00496fd0: bl #0x427d50
00496fd4: mov r1, r5
00496fd8: mov r3, r0
00496fdc: mov r2, r6
00496fe0: add r0, r4, #0x3c
00496fe4: pop {r4, r5, r6, lr}
00496fe8: b #0x427ca0
00496fec: bl #0x752b38
00496ff0: b #0x496fbc

# _ZN7SWFAnim12SetTextColorEjj 00496ff4 size124
00496ff4: push {r4, r5, r6, lr}
00496ff8: ldr r3, [r0, #0x68]
00496ffc: mov r4, r0
00497000: mov r5, r1
00497004: cmp r3, #0
00497008: mov r6, r2
0049700c: beq #0x497044
00497010: ldr r0, [r0, #0x64]
00497014: ldrb r3, [r0, #4]
00497018: cmp r3, #0
0049701c: beq #0x497048
00497020: add r0, r4, #0x3c
00497024: ldr r4, [r4, #8]
00497028: bl #0x427d50
0049702c: mov r2, r6
00497030: mov r1, r0
00497034: mov r3, r5
00497038: mov r0, r4
0049703c: pop {r4, r5, r6, lr}
00497040: b #0x7a9dd0
00497044: pop {r4, r5, r6, pc}
00497048: ldr r1, [r0]
0049704c: sub r1, r1, #1
00497050: cmp r1, #0
00497054: str r1, [r0]
00497058: bne #0x497060
0049705c: bl #0x752b38
00497060: mov r3, #0
00497064: str r3, [r4, #0x68]
00497068: str r3, [r4, #0x64]
0049706c: pop {r4, r5, r6, pc}
