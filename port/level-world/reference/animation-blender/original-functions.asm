
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

# _ZN15AnimatorBlender17_HandleAnimEndingEPN6glitch5scene19ITimelineControllerE
00366628: push     {r4, r5, r6, lr}
0036662c: ldr      r2, [r0, #0x70]
00366630: ldr      r3, [r0, #0x28]
00366634: mov      r4, r0
00366638: mov      r6, r1
0036663c: ldr      r3, [r3, r2, lsl #2]
00366640: mov      r0, r3
00366644: ldr      r3, [r3]
00366648: mov      lr, pc
0036664c: ldr      pc, [r3, #0x44]
00366650: cmp      r0, r6
00366654: mov      r5, r0
00366658: beq      #0x366660
0036665c: pop      {r4, r5, r6, pc}
00366660: cmp      r0, #0
00366664: beq      #0x3666bc
00366668: mov      r1, #0x44000000
0036666c: add      r1, r1, #0x7a0000
00366670: ldr      r0, [r0, #0x1c]
00366674: bl       #0x30ed6c
00366678: bl       #0x30e4cc
0036667c: mov      r1, #0x44000000
00366680: mov      r6, r0
00366684: add      r1, r1, #0x7a0000
00366688: ldr      r0, [r5, #0x2c]
0036668c: bl       #0x30ed6c
00366690: bl       #0x30e4cc
00366694: ldr      r3, [r5, #4]
00366698: rsb      r0, r3, r0
0036669c: cmp      r0, r6
003666a0: movge    r3, #0
003666a4: movlt    r3, #1
003666a8: cmp      r0, #0
003666ac: movlt    r3, #0
003666b0: cmp      r3, #0
003666b4: rsbne    r3, r0, r6
003666b8: str      r3, [r4, #0x98]
003666bc: mov      r3, #1
003666c0: strb     r3, [r4, #0xb8]
003666c4: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada25CSceneNodeAnimatorBlender16normalizeWeightsEv
00366594: push     {r4, r5, r6, r7, r8, lr}
00366598: ldr      r5, [r0, #0x34]
0036659c: ldr      r7, [r0, #0x38]
003665a0: mov      r8, r0
003665a4: rsb      r7, r5, r7
003665a8: asrs     r7, r7, #2
003665ac: beq      #0x36660c
003665b0: mov      r6, #0
003665b4: mov      r4, #0
003665b8: mov      r0, r6
003665bc: ldr      r1, [r5, r4, lsl #2]
003665c0: bl       #0x30eba4
003665c4: add      r4, r4, #1
003665c8: cmp      r4, r7
003665cc: mov      r6, r0
003665d0: bne      #0x3665b8
003665d4: mov      r1, #0
003665d8: bl       #0x30df8c
003665dc: cmp      r0, #0
003665e0: bne      #0x366610
003665e4: mov      r4, #0
003665e8: b        #0x3665f0
003665ec: ldr      r5, [r8, #0x34]
003665f0: ldr      r0, [r5, r4, lsl #2]
003665f4: mov      r1, r6
003665f8: bl       #0x30ec94
003665fc: str      r0, [r5, r4, lsl #2]
00366600: add      r4, r4, #1
00366604: cmp      r4, r7
00366608: bne      #0x3665ec
0036660c: pop      {r4, r5, r6, r7, r8, pc}
00366610: cmp      r4, #0
00366614: movne    r3, #0x3f800000
00366618: strne    r3, [r5]
0036661c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada25CSceneNodeAnimatorBlender22computeAnimationValuesEj
0065e48c: push     {r4, r5, r6, r7, r8, lr}
0065e490: ldr      r6, [r0, #0x28]
0065e494: ldr      r7, [r0, #0x2c]
0065e498: sub      sp, sp, #8
0065e49c: mov      r4, r0
0065e4a0: rsb      r3, r6, r7
0065e4a4: lsrs     r3, r3, #2
0065e4a8: mov      r8, r1
0065e4ac: beq      #0x65e510
0065e4b0: mov      r5, #0
0065e4b4: b        #0x65e4c8
0065e4b8: add      r5, r5, #1
0065e4bc: rsb      r3, r6, r7
0065e4c0: cmp      r5, r3, asr #2
0065e4c4: bhs      #0x65e510
0065e4c8: ldr      r3, [r4, #0x34]
0065e4cc: mov      r1, #0
0065e4d0: ldr      r0, [r3, r5, lsl #2]
0065e4d4: bl       #0x30df8c
0065e4d8: cmp      r0, #0
0065e4dc: bne      #0x65e4b8
0065e4e0: ldr      r3, [r6, r5, lsl #2]
0065e4e4: mov      r1, r8
0065e4e8: add      r5, r5, #1
0065e4ec: mov      r0, r3
0065e4f0: ldr      r3, [r3]
0065e4f4: mov      lr, pc
0065e4f8: ldr      pc, [r3, #0x4c]
0065e4fc: ldr      r6, [r4, #0x28]
0065e500: ldr      r7, [r4, #0x2c]
0065e504: rsb      r3, r6, r7
0065e508: cmp      r5, r3, asr #2
0065e50c: blo      #0x65e4c8
0065e510: mov      r0, r4
0065e514: bl       #0x366594
0065e518: ldr      r2, [r4, #0x5c]
0065e51c: ldr      r3, [r4, #0x58]
0065e520: rsb      r3, r3, r2
0065e524: lsrs     r3, r3, #2
0065e528: beq      #0x65e5c0
0065e52c: mov      r5, #0
0065e530: mov      r1, r5
0065e534: ldr      r3, [r4]
0065e538: mov      r0, r4
0065e53c: mov      lr, pc
0065e540: ldr      pc, [r3, #0x80]
0065e544: cmp      r0, #0
0065e548: beq      #0x65e5a8
0065e54c: ldr      r3, [r4, #0x58]
0065e550: mov      r1, r5
0065e554: ldr      r2, [r3, r5, lsl #2]
0065e558: cmp      r2, #0
0065e55c: beq      #0x65e5ac
0065e560: ldr      r3, [r4, #0x28]
0065e564: ldr      r3, [r3]
0065e568: mov      r0, r3
0065e56c: ldr      r3, [r3]
0065e570: mov      lr, pc
0065e574: ldr      pc, [r3, #0x58]
0065e578: ldr      ip, [r4, #0x58]
0065e57c: ldr      r2, [r4, #0x34]
0065e580: ldr      r3, [r4, #0x38]
0065e584: ldr      r1, [r4, #0x4c]
0065e588: ldr      lr, [ip, r5, lsl #2]
0065e58c: rsb      r3, r2, r3
0065e590: ldr      r1, [r1, r5, lsl #2]
0065e594: ldr      ip, [r0]
0065e598: asr      r3, r3, #2
0065e59c: str      lr, [sp]
0065e5a0: mov      lr, pc
0065e5a4: ldr      pc, [ip, #0x10]
0065e5a8: ldr      r3, [r4, #0x58]
0065e5ac: ldr      r2, [r4, #0x5c]
0065e5b0: add      r5, r5, #1
0065e5b4: rsb      r3, r3, r2
0065e5b8: cmp      r5, r3, asr #2
0065e5bc: blo      #0x65e530
0065e5c0: add      sp, sp, #8
0065e5c4: pop      {r4, r5, r6, r7, r8, pc}

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

# _ZN15AnimatorBlender9BlendPostEv
00366740: bx       lr

# _ZN15AnimatorBlender8SetScaleEf
003666d8: push     {r4, r5, r6, r7, r8, lr}
003666dc: ldr      r3, [r0, #0x28]
003666e0: ldr      r6, [r0, #0x2c]
003666e4: mov      r5, r0
003666e8: mov      r7, r1
003666ec: rsb      r6, r3, r6
003666f0: asrs     r6, r6, #2
003666f4: beq      #0x36673c
003666f8: mov      r4, #0
003666fc: b        #0x366704
00366700: ldr      r3, [r5, #0x28]
00366704: ldr      r3, [r3, r4, lsl #2]
00366708: add      r4, r4, #1
0036670c: mov      r0, r3
00366710: ldr      r3, [r3]
00366714: mov      lr, pc
00366718: ldr      pc, [r3, #0x44]
0036671c: subs     r3, r0, #0
00366720: mov      r1, r7
00366724: beq      #0x366734
00366728: ldr      r3, [r3]
0036672c: mov      lr, pc
00366730: ldr      pc, [r3, #0x48]
00366734: cmp      r4, r6
00366738: bne      #0x366700
0036673c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN15AnimatorBlenderC1Ev
00367018: push     {r4, r5, r6, lr}
0036701c: ldr      r5, [pc, #0x90]
00367020: ldr      r3, [pc, #0x90]
00367024: ldr      r1, [pc, #0x90]
00367028: add      r5, pc, r5
0036702c: ldr      r3, [r5, r3]
00367030: ldr      r1, [r5, r1]
00367034: mov      r2, #1
00367038: add      r3, r3, #8
0036703c: str      r2, [r0, #0xcc]
00367040: str      r3, [r0, #0xc8]
00367044: add      r1, r1, #4
00367048: mov      r4, r0
0036704c: bl       #0x366f7c
00367050: ldr      r2, [pc, #0x68]
00367054: mov      r1, #0
00367058: mov      r3, #0
0036705c: ldr      r2, [r5, r2]
00367060: str      r1, [r4, #0x80]
00367064: str      r3, [r4, #0x84]
00367068: add      r1, r2, #0xa0
0036706c: add      r0, r2, #0xc
00367070: add      r2, r2, #0xbc
00367074: stm      r4, {r0, r1}
00367078: str      r3, [r4, #0x70]
0036707c: str      r3, [r4, #0x74]
00367080: str      r3, [r4, #0x78]
00367084: str      r3, [r4, #0x7c]
00367088: str      r2, [r4, #0xc8]
0036708c: add      r0, r4, #0x88
00367090: mov      r1, r4
00367094: bl       #0x364330
00367098: ldr      r3, [pc, #0x24]
0036709c: str      r4, [r4, #0xc4]
003670a0: mov      r0, r4
003670a4: ldr      r3, [r5, r3]
003670a8: add      r3, r3, #8
003670ac: str      r3, [r4, #0x88]
003670b0: pop      {r4, r5, r6, pc}
003670b4: rsbeq    sp, r2, r8, ror #20
003670b8: andeq    r2, r0, r4, asr #22
003670bc: andeq    r1, r0, r0, asr #14
003670c0: andeq    r3, r0, r4, lsr #6
003670c4: andeq    r4, r0, r0, ror #21

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

# _ZN15AnimatorBlender5BlendEi
0036679c: push     {r4, r5, r6, lr}
003667a0: ldr      r5, [r0, #0x2c]
003667a4: ldr      r2, [r0, #0x28]
003667a8: ldr      r3, [pc, #0xc0]
003667ac: sub      sp, sp, #8
003667b0: rsb      r5, r2, r5
003667b4: asr      r5, r5, #2
003667b8: cmp      r5, #2
003667bc: mov      r4, r0
003667c0: mov      r6, r1
003667c4: add      r3, pc, r3
003667c8: beq      #0x3667f0
003667cc: ldr      r2, [pc, #0xa0]
003667d0: ldr      r2, [r3, r2]
003667d4: ldr      r2, [r2]
003667d8: cmp      r2, #2
003667dc: moveq    r3, #0
003667e0: streq    r3, [r3]
003667e4: beq      #0x3667f0
003667e8: cmp      r2, #1
003667ec: beq      #0x36683c
003667f0: ldr      r0, [r4, #0x70]
003667f4: mov      r1, r5
003667f8: str      r0, [r4, #0x74]
003667fc: add      r0, r0, #1
00366800: bl       #0x30eb2c
00366804: ldr      r0, [r4, #0x78]
00366808: str      r1, [r4, #0x70]
0036680c: cmp      r0, #0
00366810: str      r0, [r4, #0x7c]
00366814: ble      #0x36682c
00366818: bl       #0x30e964
0036681c: mov      r1, r0
00366820: mov      r0, #0x3f800000
00366824: bl       #0x30ec94
00366828: str      r0, [r4, #0x80]
0036682c: bic      r6, r6, r6, asr #31
00366830: str      r6, [r4, #0x78]
00366834: add      sp, sp, #8
00366838: pop      {r4, r5, r6, pc}
0036683c: ldr      r0, [pc, #0x34]
00366840: ldr      r1, [pc, #0x34]
00366844: ldr      r2, [pc, #0x34]
00366848: ldr      r0, [r3, r0]
0036684c: ldr      r3, [pc, #0x30]
00366850: mov      ip, #0x7a
00366854: add      r1, pc, r1
00366858: add      r2, pc, r2
0036685c: add      r3, pc, r3
00366860: add      r0, r0, #0xa8
00366864: str      ip, [sp]
00366868: bl       #0x30e004
0036686c: b        #0x3667f0
00366870: rsbeq    lr, r2, ip, asr #5
00366874: andeq    r3, r0, r0, asr #19
00366878: andeq    r1, r0, r0, asr #19
0036687c: subseq   r7, r5, r4, lsl #23
00366880: subseq   sl, r5, r8, lsl #11

# _ZN6glitch7collada25CSceneNodeAnimatorBlenderC2Ev
00366f7c: push     {r4, r5, r6, lr}
00366f80: mov      r6, r1
00366f84: ldr      r5, [pc, #0x84]
00366f88: add      r1, r1, #4
00366f8c: mov      r4, r0
00366f90: bl       #0x6698fc
00366f94: ldr      r2, [r6]
00366f98: ldr      r3, [pc, #0x74]
00366f9c: add      r5, pc, r5
00366fa0: str      r2, [r4]
00366fa4: ldr      r3, [r5, r3]
00366fa8: ldr      r1, [r2, #-0xc]
00366fac: ldr      r0, [r6, #0x1c]
00366fb0: add      r2, r3, #0xa0
00366fb4: mov      r3, #0
00366fb8: str      r0, [r4, r1]
00366fbc: str      r2, [r4, #4]
00366fc0: str      r3, [r4, #0x6c]
00366fc4: str      r3, [r4, #0x28]
00366fc8: str      r3, [r4, #0x2c]
00366fcc: str      r3, [r4, #0x30]
00366fd0: str      r3, [r4, #0x34]
00366fd4: str      r3, [r4, #0x38]
00366fd8: str      r3, [r4, #0x3c]
00366fdc: str      r3, [r4, #0x40]
00366fe0: str      r3, [r4, #0x44]
00366fe4: str      r3, [r4, #0x48]
00366fe8: str      r3, [r4, #0x4c]
00366fec: str      r3, [r4, #0x50]
00366ff0: str      r3, [r4, #0x54]
00366ff4: str      r3, [r4, #0x58]
00366ff8: str      r3, [r4, #0x5c]
00366ffc: str      r3, [r4, #0x60]
00367000: str      r3, [r4, #0x64]
00367004: str      r3, [r4, #0x68]
00367008: mov      r0, r4
0036700c: pop      {r4, r5, r6, pc}

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

# _ZN15AnimatorBlender17BlenderApplicator10SetRefNodeEPN6glitch5scene10ISceneNodeE
00366b80: push     {r4, r5, r6, r7, lr}
00366b84: ldr      r2, [r0, #0x3c]
00366b88: ldr      r3, [pc, #0xe4]
00366b8c: sub      sp, sp, #0xc
00366b90: cmp      r2, #0
00366b94: mov      r4, r0
00366b98: mov      r5, r1
00366b9c: add      r3, pc, r3
00366ba0: beq      #0x366c20
00366ba4: mov      r0, r4
00366ba8: mov      r1, r5
00366bac: bl       #0x36473c
00366bb0: ldr      r3, [r4, #0x3c]
00366bb4: ldr      r7, [r3, #0x2c]
00366bb8: ldr      r3, [r3, #0x28]
00366bbc: rsb      r7, r3, r7
00366bc0: asrs     r7, r7, #2
00366bc4: beq      #0x366c18
00366bc8: mov      r6, #0
00366bcc: b        #0x366bf4
00366bd0: ldr      r3, [r1]
00366bd4: add      r6, r6, #1
00366bd8: mov      r1, r5
00366bdc: mov      lr, pc
00366be0: ldr      pc, [r3, #8]
00366be4: cmp      r6, r7
00366be8: beq      #0x366c18
00366bec: ldr      r3, [r4, #0x3c]
00366bf0: ldr      r3, [r3, #0x28]
00366bf4: ldr      r0, [r3, r6, lsl #2]
00366bf8: bl       #0x369160
00366bfc: subs     r1, r0, #0
00366c00: bne      #0x366bd0
00366c04: mov      r0, r4
00366c08: add      r6, r6, #1
00366c0c: bl       #0x36473c
00366c10: cmp      r6, r7
00366c14: bne      #0x366bec
00366c18: add      sp, sp, #0xc
00366c1c: pop      {r4, r5, r6, r7, pc}
00366c20: ldr      r1, [pc, #0x50]
00366c24: ldr      r1, [r3, r1]
00366c28: ldr      r1, [r1]
00366c2c: cmp      r1, #2
00366c30: streq    r2, [r2]
00366c34: beq      #0x366ba4
00366c38: cmp      r1, #1
00366c3c: bne      #0x366ba4
00366c40: ldr      r0, [pc, #0x34]
00366c44: ldr      r1, [pc, #0x34]
00366c48: ldr      r2, [pc, #0x34]
00366c4c: ldr      r0, [r3, r0]
00366c50: ldr      r3, [pc, #0x30]
00366c54: mov      ip, #0x130
00366c58: add      r1, pc, r1
00366c5c: add      r2, pc, r2
00366c60: add      r3, pc, r3
00366c64: add      r0, r0, #0xa8
00366c68: str      ip, [sp]
00366c6c: bl       #0x30e004
00366c70: b        #0x366ba4
