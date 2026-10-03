
# _ZN11AnimatorSetC1ERKN5boost13intrusive_ptrI12AnimationSetEE
003676b8: push     {r4, r5, r6, lr}
003676bc: ldr      r5, [pc, #0xbc]
003676c0: ldr      r3, [pc, #0xbc]
003676c4: mov      r2, #1
003676c8: add      r5, pc, r5
003676cc: ldr      r3, [r5, r3]
003676d0: str      r2, [r0, #0xa0]
003676d4: sub      sp, sp, #8
003676d8: add      r3, r3, #8
003676dc: str      r3, [r0, #0x9c]
003676e0: ldr      r3, [r1]
003676e4: mov      r6, r1
003676e8: ldr      r1, [pc, #0x98]
003676ec: ldr      r3, [r3, #0x20]
003676f0: mov      r4, r0
003676f4: ldr      r1, [r5, r1]
003676f8: cmp      r3, #0
003676fc: str      r3, [sp, #4]
00367700: ldrne    r2, [r3, #4]
00367704: add      r1, r1, #4
00367708: addne    r2, r2, #1
0036770c: strne    r2, [r3, #4]
00367710: add      r2, sp, #4
00367714: bl       #0x660ce4
00367718: ldr      r0, [sp, #4]
0036771c: cmp      r0, #0
00367720: beq      #0x367728
00367724: bl       #0x31d584
00367728: ldr      r3, [pc, #0x5c]
0036772c: add      r0, r4, #0x58
00367730: mov      r1, r4
00367734: ldr      r3, [r5, r3]
00367738: add      r2, r3, #0xa4
0036773c: add      ip, r3, #0xc
00367740: add      r3, r3, #0xc0
00367744: str      r2, [r4, #4]
00367748: str      r3, [r4, #0x9c]
0036774c: str      ip, [r4]
00367750: bl       #0x364398
00367754: ldr      r3, [r6]
00367758: mov      r0, r4
0036775c: cmp      r3, #0
00367760: str      r3, [r4, #0x94]
00367764: ldrne    r2, [r3, #4]
00367768: addne    r2, r2, #1
0036776c: strne    r2, [r3, #4]
00367770: mov      r3, #0
00367774: str      r3, [r4, #0x98]
00367778: add      sp, sp, #8
0036777c: pop      {r4, r5, r6, pc}
00367780: rsbeq    sp, r2, r8, asr #7
00367784: andeq    r2, r0, r4, asr #22
00367788: ldrdeq   r1, r2, [r0], -r4
0036778c: andeq    r1, r0, ip, asr #20

# _ZN6glitch5scene18ISceneNodeAnimator15getTimelineCtrlEv
00599870: ldr      r0, [r0, #8]
00599874: bx       lr

# _ZNK6glitch7collada21CSceneNodeAnimatorSet19getCurrentAnimationEv
0065f114: ldr      r0, [r0, #0x50]
0065f118: bx       lr

# _ZNK6glitch5scene18ISceneNodeAnimator15getTimelineCtrlEv
00599868: ldr      r0, [r0, #8]
0059986c: bx       lr

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
