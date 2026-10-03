
# _ZN12v2Controller10Cmd_LookAtEP10GameObject
004052bc: push     {r4, lr}
004052c0: ldrb     r2, [r0, #9]
004052c4: ldr      r3, [pc, #0x44]
004052c8: cmp      r2, #0
004052cc: add      r3, pc, r3
004052d0: bne      #0x4052f8
004052d4: ldr      r2, [pc, #0x38]
004052d8: ldr      r3, [r3, r2]
004052dc: ldrb     r3, [r3]
004052e0: cmp      r3, #0
004052e4: beq      #0x4052ec
004052e8: pop      {r4, pc}
004052ec: ldrb     r3, [r0, #8]
004052f0: cmp      r3, #0
004052f4: bne      #0x4052e8
004052f8: ldr      r3, [r0, #4]
004052fc: mov      r0, r3
00405300: ldr      r3, [r3]
00405304: mov      lr, pc
00405308: ldr      pc, [r3, #0x14]
0040530c: pop      {r4, pc}
00405310: subseq   pc, r8, r4, asr #15
00405314: andeq    r3, r0, r0, asr r6

# _ZNK6CharAI19AI_GetMeleeRadiusSqEv
003d4c9c: push     {r4, lr}
003d4ca0: bl       #0x3d4c34
003d4ca4: mov      r1, r0
003d4ca8: bl       #0x30ed6c
003d4cac: pop      {r4, pc}

# _ZN12v2Controller10Cmd_MoveToEP10GameObject
00405540: push     {r4, lr}
00405544: ldrb     r2, [r0, #9]
00405548: ldr      r3, [pc, #0x44]
0040554c: cmp      r2, #0
00405550: add      r3, pc, r3
00405554: bne      #0x40557c
00405558: ldr      r2, [pc, #0x38]
0040555c: ldr      r3, [r3, r2]
00405560: ldrb     r3, [r3]
00405564: cmp      r3, #0
00405568: beq      #0x405570
0040556c: pop      {r4, pc}
00405570: ldrb     r3, [r0, #8]
00405574: cmp      r3, #0
00405578: bne      #0x40556c
0040557c: ldr      r3, [r0, #4]
00405580: mov      r0, r3
00405584: ldr      r3, [r3]
00405588: mov      lr, pc
0040558c: ldr      pc, [r3, #0x30]
00405590: pop      {r4, pc}
00405594: subseq   pc, r8, r0, asr #10
00405598: andeq    r3, r0, r0, asr r6

# _ZN12CharAnimator17ANIM_SkipNextStepEv
003c9464: ldr      r1, [r0, #0x2c]
003c9468: b        #0x3c9444

# _ZNK6CharAI18AI_IsTargetSeekingEv
003d49d0: ldrb     r3, [r0, #0x4a]
003d49d4: cmp      r3, #0
003d49d8: ldrne    r3, [r0, #4]
003d49dc: moveq    r0, r3
003d49e0: ldrne    r0, [r3, #0x520]
003d49e4: eorne    r0, r0, #0x1000
003d49e8: ubfxne   r0, r0, #0xc, #1
003d49ec: bx       lr

# _ZN6CharAI25_OnAnimStepEnd_SkillSpellEv
003d3d68: push     {r4, r5, r6, lr}
003d3d6c: ldr      r3, [r0, #4]
003d3d70: mov      r4, r0
003d3d74: add      r0, r3, #0x490
003d3d78: add      r0, r0, #0xc
003d3d7c: ldr      r5, [r3, #0x4c8]
003d3d80: bl       #0x3c932c
003d3d84: mov      r6, r0
003d3d88: ldr      r0, [r4, #4]
003d3d8c: add      r0, r0, #0x490
003d3d90: add      r0, r0, #0xc
003d3d94: bl       #0x3c934c
003d3d98: cmp      r6, #0
003d3d9c: cmpeq    r5, #1
003d3da0: bne      #0x3d3db0
003d3da4: ldrb     r3, [r4, #0xd1]
003d3da8: cmp      r3, #0
003d3dac: bne      #0x3d3db8
003d3db0: mov      r0, #1
003d3db4: pop      {r4, r5, r6, pc}
003d3db8: ldr      r0, [r4, #4]
003d3dbc: mov      r1, #1
003d3dc0: add      r0, r0, #0x490
003d3dc4: add      r0, r0, #0xc
003d3dc8: bl       #0x3c948c
003d3dcc: mov      r0, #1
003d3dd0: pop      {r4, r5, r6, pc}

# _ZN6CharAI21_OnAnimStepBegin_MoveEv
003d4120: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003d4124: ldr      r3, [r0, #0x40]
003d4128: mov      r4, r0
003d412c: cmp      r3, #0
003d4130: beq      #0x3d41e4
003d4134: bl       #0x3d49d0
003d4138: cmp      r0, #0
003d413c: beq      #0x3d41e4
003d4140: ldr      r0, [r4, #0x40]
003d4144: bl       #0x3935dc
003d4148: ldr      r6, [r4, #4]
003d414c: mov      r5, r0
003d4150: ldr      r0, [r0]
003d4154: ldr      r1, [r6, #0x1a8]
003d4158: bl       #0x30e3ac
003d415c: ldr      r1, [r6, #0x1ac]
003d4160: mov      sl, r0
003d4164: ldr      r0, [r5, #4]
003d4168: bl       #0x30e3ac
003d416c: ldr      r1, [r6, #0x1b0]
003d4170: mov      r8, r0
003d4174: ldr      r0, [r5, #8]
003d4178: bl       #0x30e3ac
003d417c: mov      r7, r0
003d4180: mov      r0, r4
003d4184: bl       #0x3d4c9c
003d4188: mov      r1, sl
003d418c: mov      r5, r0
003d4190: mov      r0, sl
003d4194: bl       #0x30ed6c
003d4198: mov      r1, r8
003d419c: mov      r6, r0
003d41a0: mov      r0, r8
003d41a4: bl       #0x30ed6c
003d41a8: mov      r1, r0
003d41ac: mov      r0, r6
003d41b0: bl       #0x30eba4
003d41b4: mov      r1, r7
003d41b8: mov      r6, r0
003d41bc: mov      r0, r7
003d41c0: bl       #0x30ed6c
003d41c4: mov      r1, r0
003d41c8: mov      r0, r6
003d41cc: bl       #0x30eba4
003d41d0: mov      r1, r0
003d41d4: mov      r0, r5
003d41d8: bl       #0x30e9ac
003d41dc: cmp      r0, #0
003d41e0: bne      #0x3d41ec
003d41e4: mov      r0, #1
003d41e8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003d41ec: ldr      r3, [r4, #4]
003d41f0: ldr      r1, [r4, #0x40]
003d41f4: ldr      r0, [r3, #0x378]
003d41f8: bl       #0x405540
003d41fc: mov      r0, #1
003d4200: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN12CharAnimator13ANIM_StopLoopEb
003c948c: ldrb     r3, [r0, #0x48]
003c9490: cmp      r3, #0
003c9494: bxne     lr
003c9498: ldr      r2, [r0, #0x2c]
003c949c: cmp      r1, #0
003c94a0: mov      r1, #0xc
003c94a4: mla      r2, r1, r2, r0
003c94a8: str      r3, [r2, #0xc]
003c94ac: movne    r3, #1
003c94b0: strbne   r3, [r0, #0x4a]
003c94b4: bx       lr

# _ZNK12CharAnimator17ANIM_GetStepCountEv
003c934c: ldrb     r2, [r0, #0x48]
003c9350: ldr      r3, [pc, #0x38]
003c9354: cmp      r2, #0
003c9358: add      r3, pc, r3
003c935c: movne    r0, #0
003c9360: bxne     lr
003c9364: ldr      r2, [r0, #0x2c]
003c9368: mov      r1, #0xc
003c936c: mla      r0, r1, r2, r0
003c9370: ldr      r2, [pc, #0x1c]
003c9374: mov      r1, #0x14
003c9378: ldr      r2, [r3, r2]
003c937c: ldr      r3, [r0, #8]
003c9380: ldr      r2, [r2]
003c9384: mla      r3, r1, r3, r2
003c9388: ldr      r0, [r3, #8]
003c938c: bx       lr
003c9390: subseq   fp, ip, r8, lsr r7
003c9394: andeq    r3, r0, ip, ror ip

# _ZN6CharAI14_OnAnimStepEndEv
003d3ff8: push     {r4, lr}
003d3ffc: mov      r4, r0
003d4000: ldr      r0, [r0, #4]
003d4004: add      r0, r0, #0x4f0
003d4008: add      r0, r0, #0xc
003d400c: bl       #0x3c01ac
003d4010: cmp      r0, #5
003d4014: beq      #0x3d4038
003d4018: bge      #0x3d4024
003d401c: mov      r0, #1
003d4020: pop      {r4, pc}
003d4024: cmp      r0, #7
003d4028: bgt      #0x3d401c
003d402c: mov      r0, r4
003d4030: pop      {r4, lr}
003d4034: b        #0x3d3d68
003d4038: mov      r0, r4
003d403c: pop      {r4, lr}
003d4040: b        #0x3d3e44

# _ZN6CharAI27_OnAnimStepBegin_SkillSpellEv
003d3dd4: push     {r4, r5, r6, lr}
003d3dd8: ldr      r3, [r0, #4]
003d3ddc: mov      r4, r0
003d3de0: add      r0, r3, #0x490
003d3de4: add      r0, r0, #0xc
003d3de8: ldr      r5, [r3, #0x4c8]
003d3dec: bl       #0x3c932c
003d3df0: mov      r6, r0
003d3df4: ldr      r0, [r4, #4]
003d3df8: add      r0, r0, #0x490
003d3dfc: add      r0, r0, #0xc
003d3e00: bl       #0x3c934c
003d3e04: cmp      r6, #0
003d3e08: cmpeq    r5, #1
003d3e0c: bne      #0x3d3e24
003d3e10: ldrb     r3, [r4, #0xd1]
003d3e14: mov      r1, #1
003d3e18: strb     r1, [r4, #0xd0]
003d3e1c: cmp      r3, #0
003d3e20: bne      #0x3d3e2c
003d3e24: mov      r0, #1
003d3e28: pop      {r4, r5, r6, pc}
003d3e2c: ldr      r0, [r4, #4]
003d3e30: add      r0, r0, #0x490
003d3e34: add      r0, r0, #0xc
003d3e38: bl       #0x3c948c
003d3e3c: mov      r0, #1
003d3e40: pop      {r4, r5, r6, pc}

# _ZN6CharAI23_OnAnimStepBegin_AttackEv
003d4044: push     {r4, r5, r6, lr}
003d4048: ldr      r3, [r0, #4]
003d404c: mov      r4, r0
003d4050: add      r0, r3, #0x490
003d4054: add      r0, r0, #0xc
003d4058: ldr      r5, [r3, #0x4c8]
003d405c: bl       #0x3c932c
003d4060: mov      r6, r0
003d4064: ldr      r0, [r4, #4]
003d4068: add      r0, r0, #0x490
003d406c: add      r0, r0, #0xc
003d4070: bl       #0x3c934c
003d4074: cmp      r5, #0
003d4078: beq      #0x3d40c8
003d407c: cmp      r5, #1
003d4080: beq      #0x3d408c
003d4084: mov      r0, #1
003d4088: pop      {r4, r5, r6, pc}
003d408c: cmp      r6, #0
003d4090: bne      #0x3d40e4
003d4094: ldr      r3, [r4, #4]
003d4098: strb     r5, [r4, #0x79]
003d409c: ldr      r1, [r3, #0x408]
003d40a0: ldr      r0, [r3, #0x378]
003d40a4: bl       #0x4052bc
003d40a8: mov      r0, r4
003d40ac: strb     r6, [r4, #0x7a]
003d40b0: ldr      r3, [r4]
003d40b4: ldr      r1, [r4, #0x74]
003d40b8: mov      lr, pc
003d40bc: ldr      pc, [r3, #0xa4]
003d40c0: mov      r0, #1
003d40c4: pop      {r4, r5, r6, pc}
003d40c8: ldr      r0, [r4, #4]
003d40cc: str      r6, [r4, #0x74]
003d40d0: mov      r2, r6
003d40d4: mov      r1, #0x1a
003d40d8: bl       #0x3a4d5c
003d40dc: mov      r0, #1
003d40e0: pop      {r4, r5, r6, pc}
003d40e4: ldr      r3, [r4, #4]
003d40e8: sub      r0, r0, #1
003d40ec: cmp      r6, r0
003d40f0: movne    r6, #0
003d40f4: moveq    r6, #1
003d40f8: strb     r6, [r4, #0x79]
003d40fc: ldr      r1, [r3, #0x408]
003d4100: ldr      r0, [r3, #0x378]
003d4104: bl       #0x4052bc
003d4108: cmp      r6, #0
003d410c: mov      r3, #0
003d4110: strb     r3, [r4, #0x7a]
003d4114: mov      r0, #1
003d4118: strbne   r5, [r4, #0x7a]
003d411c: pop      {r4, r5, r6, pc}

# _ZN6CharAI16_OnAnimStepBeginEv
003d4204: push     {r4, lr}
003d4208: mov      r4, r0
003d420c: ldr      r0, [r0, #4]
003d4210: add      r0, r0, #0x4f0
003d4214: add      r0, r0, #0xc
003d4218: bl       #0x3c01ac
003d421c: sub      r0, r0, #4
003d4220: cmp      r0, #3
003d4224: addls    pc, pc, r0, lsl #2
003d4228: b        #0x3d4260
003d422c: b        #0x3d4254
003d4230: b        #0x3d4248
003d4234: b        #0x3d423c
003d4238: b        #0x3d423c
003d423c: mov      r0, r4
003d4240: pop      {r4, lr}
003d4244: b        #0x3d3dd4
003d4248: mov      r0, r4
003d424c: pop      {r4, lr}
003d4250: b        #0x3d4044
003d4254: mov      r0, r4
003d4258: pop      {r4, lr}
003d425c: b        #0x3d4120
003d4260: mov      r0, #1
003d4264: pop      {r4, pc}

# _ZNK9Character14HasComboAttackEv
003a346c: push     {r4, r5, r6, lr}
003a3470: ldr      r4, [pc, #0x68]
003a3474: ldr      r3, [pc, #0x68]
003a3478: add      r4, pc, r4
003a347c: ldr      r3, [r4, r3]
003a3480: ldr      r5, [r3]
003a3484: bl       #0x3a3228
003a3488: mov      r3, #0xa0
003a348c: mla      r5, r3, r0, r5
003a3490: ldr      r3, [r5, #4]
003a3494: cmp      r3, #0
003a3498: blt      #0x3a34d8
003a349c: ldr      r2, [pc, #0x44]
003a34a0: ldr      r2, [r4, r2]
003a34a4: ldr      r2, [r2]
003a34a8: cmp      r3, r2
003a34ac: bge      #0x3a34d8
003a34b0: ldr      r2, [pc, #0x34]
003a34b4: mov      r1, #0x14
003a34b8: ldr      r2, [r4, r2]
003a34bc: ldr      r2, [r2]
003a34c0: mla      r3, r1, r3, r2
003a34c4: ldr      r0, [r3, #0x10]
003a34c8: cmp      r0, #1
003a34cc: movne    r0, #0
003a34d0: moveq    r0, #1
003a34d4: pop      {r4, r5, r6, pc}
003a34d8: mov      r0, #0
003a34dc: pop      {r4, r5, r6, pc}
003a34e0: subseq   r1, pc, r8, lsl r6
003a34e4: andeq    r4, r0, r4, asr #16
003a34e8: andeq    r2, r0, r8, asr #20
003a34ec: andeq    r3, r0, ip, ror ip

# _ZNK12CharAnimator17ANIM_GetStepIndexEv
003c932c: ldrb     r3, [r0, #0x48]
003c9330: cmp      r3, #0
003c9334: ldreq    r3, [r0, #0x2c]
003c9338: moveq    r2, #0xc
003c933c: mvnne    r0, #0
003c9340: mlaeq    r0, r2, r3, r0
003c9344: ldreq    r0, [r0, #0x10]
003c9348: bx       lr

# _ZN6CharAI21_OnAnimStepEnd_AttackEv
003d3e44: push     {r4, r5, r6, r7, r8, lr}
003d3e48: mov      r4, r0
003d3e4c: ldr      r0, [r0, #4]
003d3e50: bl       #0x3a346c
003d3e54: cmp      r0, #0
003d3e58: bne      #0x3d3e64
003d3e5c: mov      r0, #1
003d3e60: pop      {r4, r5, r6, r7, r8, pc}
003d3e64: ldr      r3, [r4, #4]
003d3e68: add      r0, r3, #0x490
003d3e6c: add      r0, r0, #0xc
003d3e70: ldr      r5, [r3, #0x4c8]
003d3e74: bl       #0x3c932c
003d3e78: mov      r7, r0
003d3e7c: ldr      r0, [r4, #4]
003d3e80: add      r0, r0, #0x490
003d3e84: add      r0, r0, #0xc
003d3e88: bl       #0x3c934c
003d3e8c: cmp      r5, #0
003d3e90: mov      r6, r0
003d3e94: bne      #0x3d3ed0
003d3e98: ldrb     r1, [r4, #0x78]
003d3e9c: sub      r3, r0, #1
003d3ea0: cmp      r7, r3
003d3ea4: strb     r5, [r4, #0x78]
003d3ea8: eor      r1, r1, #1
003d3eac: beq      #0x3d3f20
003d3eb0: cmp      r1, #0
003d3eb4: bne      #0x3d3f68
003d3eb8: ldr      r0, [r4, #4]
003d3ebc: mov      r1, #0x1b
003d3ec0: mov      r2, #0
003d3ec4: bl       #0x3a4d5c
003d3ec8: mov      r0, #1
003d3ecc: pop      {r4, r5, r6, r7, r8, pc}
003d3ed0: cmp      r5, #1
003d3ed4: bne      #0x3d3e5c
003d3ed8: ldr      r3, [r4, #0x40]
003d3edc: cmp      r3, #0
003d3ee0: moveq    r0, r5
003d3ee4: beq      #0x3d3fc8
003d3ee8: mov      r0, r3
003d3eec: ldr      r3, [r3]
003d3ef0: mov      lr, pc
003d3ef4: ldr      pc, [r3, #0x34]
003d3ef8: ldr      r3, [r4, #0x40]
003d3efc: cmp      r3, #0
003d3f00: beq      #0x3d3fc8
003d3f04: mov      r2, #0
003d3f08: sub      r6, r6, #2
003d3f0c: cmp      r7, r6
003d3f10: beq      #0x3d3fa4
003d3f14: mov      r3, #0
003d3f18: strb     r3, [r4, #0x78]
003d3f1c: b        #0x3d3e5c
003d3f20: cmp      r1, #0
003d3f24: beq      #0x3d3f54
003d3f28: mov      r0, r4
003d3f2c: bl       #0x3d8d70
003d3f30: ldr      r0, [r4, #4]
003d3f34: mov      r1, #0x1b
003d3f38: mov      r2, #0
003d3f3c: bl       #0x3a4d5c
003d3f40: ldr      r0, [r4, #4]
003d3f44: mov      r1, #0x1c
003d3f48: mov      r2, #0
003d3f4c: bl       #0x3a4d5c
003d3f50: b        #0x3d3e5c
003d3f54: ldr      r0, [r4, #4]
003d3f58: add      r0, r0, #0x490
003d3f5c: add      r0, r0, #0xc
003d3f60: bl       #0x3c9484
003d3f64: b        #0x3d3f30
003d3f68: ldr      r3, [r4, #4]
003d3f6c: mov      r0, r3
003d3f70: ldr      r3, [r3]
003d3f74: mov      lr, pc
003d3f78: ldr      pc, [r3, #0x124]
003d3f7c: cmp      r0, #0
003d3f80: bne      #0x3d3eb8
003d3f84: mov      r0, r4
003d3f88: bl       #0x3d8d70
003d3f8c: ldr      r0, [r4, #4]
003d3f90: mov      r1, r6
003d3f94: add      r0, r0, #0x490
003d3f98: add      r0, r0, #0xc
003d3f9c: bl       #0x3c9484
003d3fa0: b        #0x3d3eb8
003d3fa4: cmp      r0, #0
003d3fa8: bne      #0x3d3fec
003d3fac: cmp      r2, #0
003d3fb0: bne      #0x3d3f14
003d3fb4: ldr      r0, [r4, #4]
003d3fb8: add      r0, r0, #0x490
003d3fbc: add      r0, r0, #0xc
003d3fc0: bl       #0x3c9464
003d3fc4: b        #0x3d3e5c
003d3fc8: ldr      r2, [r4, #4]
003d3fcc: movw     r3, #0x14a8
003d3fd0: ldrsb    r3, [r2, r3]
003d3fd4: cmp      r3, #8
003d3fd8: moveq    r3, #0
003d3fdc: moveq    r2, #1
003d3fe0: beq      #0x3d3f08
003d3fe4: mov      r3, #0
003d3fe8: b        #0x3d3f04
003d3fec: cmp      r3, #0
003d3ff0: bne      #0x3d3f14
003d3ff4: b        #0x3d3fac

# _ZN12CharAnimator12ANIM_SetStepEj
003c9484: ldr      r2, [r0, #0x2c]
003c9488: b        #0x3c946c

# _ZN6CharAI21_ClearNonStickyTargetEv
003d8d70: push     {r4, lr}
003d8d74: ldrb     r1, [r0, #0x4b]
003d8d78: mov      r4, r0
003d8d7c: cmp      r1, #0
003d8d80: beq      #0x3d8d88
003d8d84: pop      {r4, pc}
003d8d88: mov      r2, r1
003d8d8c: bl       #0x3d6890
003d8d90: mov      r0, r4
003d8d94: pop      {r4, lr}
003d8d98: b        #0x3d49c4
