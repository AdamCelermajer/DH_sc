
# _ZNK6CharAI11AI_IsFriendEPK10GameObject
003d511c: push     {r4, r5, r6, r7, lr}
003d5120: ldr      r4, [pc, #0x280]
003d5124: cmp      r1, #0
003d5128: sub      sp, sp, #0x1c
003d512c: mov      r5, r0
003d5130: add      r4, pc, r4
003d5134: beq      #0x3d5330
003d5138: add      r6, sp, #0xc
003d513c: mov      r0, r6
003d5140: bl       #0x33dd70
003d5144: mov      r0, r6
003d5148: mov      r1, #0
003d514c: bl       #0x33ff8c
003d5150: subs     r6, r0, #0
003d5154: bne      #0x3d5164
003d5158: mov      r0, #0
003d515c: add      sp, sp, #0x1c
003d5160: pop      {r4, r5, r6, r7, pc}
003d5164: ldr      r7, [r6, #0xf4]
003d5168: cmp      r7, #0
003d516c: bne      #0x3d5158
003d5170: bl       #0x3a3180
003d5174: cmp      r0, #0
003d5178: blt      #0x3d52dc
003d517c: mov      r0, r6
003d5180: bl       #0x3a3180
003d5184: ldr      r7, [pc, #0x220]
003d5188: ldr      r3, [r4, r7]
003d518c: ldr      r3, [r3]
003d5190: cmp      r0, r3
003d5194: blt      #0x3d51bc
003d5198: ldr      r3, [pc, #0x210]
003d519c: ldr      r3, [r4, r3]
003d51a0: ldr      r3, [r3]
003d51a4: cmp      r3, #2
003d51a8: moveq    r3, #0
003d51ac: streq    r3, [r3]
003d51b0: beq      #0x3d51bc
003d51b4: cmp      r3, #1
003d51b8: beq      #0x3d5374
003d51bc: ldr      r0, [r5, #4]
003d51c0: bl       #0x3a3180
003d51c4: cmp      r0, #0
003d51c8: blt      #0x3d5284
003d51cc: ldr      r0, [r5, #4]
003d51d0: bl       #0x3a3180
003d51d4: ldr      r3, [r4, r7]
003d51d8: ldr      r3, [r3]
003d51dc: cmp      r0, r3
003d51e0: blt      #0x3d5208
003d51e4: ldr      r3, [pc, #0x1c4]
003d51e8: ldr      r3, [r4, r3]
003d51ec: ldr      r3, [r3]
003d51f0: cmp      r3, #2
003d51f4: moveq    r3, #0
003d51f8: streq    r3, [r3]
003d51fc: beq      #0x3d5208
003d5200: cmp      r3, #1
003d5204: beq      #0x3d5340
003d5208: ldr      r3, [pc, #0x1a4]
003d520c: ldr      r0, [r5, #4]
003d5210: ldr      r3, [r4, r3]
003d5214: ldr      r4, [r3]
003d5218: bl       #0x3a3180
003d521c: mov      r3, #0xc
003d5220: mla      r4, r3, r0, r4
003d5224: mov      r0, r6
003d5228: bl       #0x3a3180
003d522c: ldr      ip, [r4, #4]
003d5230: cmp      ip, #0
003d5234: beq      #0x3d5158
003d5238: ldr      r2, [r4, #8]
003d523c: ldr      r3, [r2, #4]
003d5240: cmp      r0, r3
003d5244: movne    r3, #0
003d5248: bne      #0x3d525c
003d524c: b        #0x3d5270
003d5250: ldr      r1, [r2, #4]
003d5254: cmp      r0, r1
003d5258: beq      #0x3d5270
003d525c: add      r3, r3, #1
003d5260: cmp      r3, ip
003d5264: add      r2, r2, #0xc
003d5268: bne      #0x3d5250
003d526c: b        #0x3d5158
003d5270: ldr      r0, [r2, #8]
003d5274: cmp      r0, #0
003d5278: movle    r0, #0
003d527c: movgt    r0, #1
003d5280: b        #0x3d515c
003d5284: ldr      r3, [pc, #0x124]
003d5288: ldr      r3, [r4, r3]
003d528c: ldr      r3, [r3]
003d5290: cmp      r3, #2
003d5294: moveq    r3, #0
003d5298: streq    r3, [r3]
003d529c: beq      #0x3d51cc
003d52a0: cmp      r3, #1
003d52a4: bne      #0x3d51cc
003d52a8: ldr      r0, [pc, #0x108]
003d52ac: ldr      r1, [pc, #0x108]
003d52b0: ldr      r2, [pc, #0x108]
003d52b4: ldr      r0, [r4, r0]
003d52b8: ldr      r3, [pc, #0x104]
003d52bc: mov      ip, #0xc6
003d52c0: add      r1, pc, r1
003d52c4: add      r2, pc, r2
003d52c8: add      r3, pc, r3
003d52cc: add      r0, r0, #0xa8
003d52d0: str      ip, [sp]
003d52d4: bl       #0x30e004
003d52d8: b        #0x3d51cc
003d52dc: ldr      r3, [pc, #0xcc]
003d52e0: ldr      r3, [r4, r3]
003d52e4: ldr      r3, [r3]
003d52e8: cmp      r3, #2
003d52ec: streq    r7, [r7]
003d52f0: beq      #0x3d517c
003d52f4: cmp      r3, #1
003d52f8: bne      #0x3d517c
003d52fc: ldr      r0, [pc, #0xb4]
003d5300: ldr      r1, [pc, #0xc0]
003d5304: ldr      r2, [pc, #0xc0]
003d5308: ldr      r0, [r4, r0]
003d530c: ldr      r3, [pc, #0xbc]
003d5310: mov      ip, #0xc4
003d5314: add      r1, pc, r1
003d5318: add      r2, pc, r2
003d531c: add      r3, pc, r3
003d5320: add      r0, r0, #0xa8
003d5324: str      ip, [sp]
003d5328: bl       #0x30e004
003d532c: b        #0x3d517c
003d5330: ldr      r1, [r0, #0x40]
003d5334: cmp      r1, #0
003d5338: beq      #0x3d5158
003d533c: b        #0x3d5138
003d5340: ldr      r0, [pc, #0x70]
003d5344: ldr      r1, [pc, #0x88]
003d5348: ldr      r2, [pc, #0x88]
003d534c: ldr      r0, [r4, r0]
003d5350: ldr      r3, [pc, #0x84]
003d5354: mov      ip, #0xc7
003d5358: add      r1, pc, r1
003d535c: add      r2, pc, r2
003d5360: add      r3, pc, r3
003d5364: add      r0, r0, #0xa8
003d5368: str      ip, [sp]
003d536c: bl       #0x30e004
003d5370: b        #0x3d5208
003d5374: ldr      r0, [pc, #0x3c]
003d5378: ldr      r1, [pc, #0x60]
003d537c: ldr      r2, [pc, #0x60]
003d5380: ldr      r0, [r4, r0]
003d5384: ldr      r3, [pc, #0x5c]
003d5388: mov      ip, #0xc5
003d538c: add      r1, pc, r1
003d5390: add      r2, pc, r2
003d5394: add      r3, pc, r3
003d5398: add      r0, r0, #0xa8
003d539c: str      ip, [sp]
003d53a0: bl       #0x30e004
003d53a4: b        #0x3d51bc
003d53a8: subseq   pc, fp, r0, ror #18
003d53ac: andeq    r2, r0, r4, asr #4
003d53b0: andeq    r3, r0, r0, asr #19
003d53b4: andeq    r4, r0, ip, lsr #12
003d53b8: andeq    r1, r0, r0, asr #19
003d53bc: subeq    sb, lr, r8, lsl r1
003d53c0: strheq   r0, [pc], #-0x34
003d53c4: strdeq   r0, r1, [pc], #-0x28
003d53c8: subeq    sb, lr, r4, asr #1
003d53cc: subeq    r0, pc, r0, lsl #6
003d53d0: subeq    r0, pc, r4, lsr #5
003d53d4: subeq    sb, lr, r0, lsl #1
003d53d8: subeq    r0, pc, ip, lsr r3
003d53dc: subeq    r0, pc, r0, ror #4
003d53e0: subeq    sb, lr, ip, asr #32
003d53e4: subeq    r0, pc, r8, lsr #5
003d53e8: subeq    r0, pc, ip, lsr #4

# _ZNK10GameObject13IsInteractiveEPS_
003883b0: mov      r0, #0
003883b4: bx       lr

# _ZNK10GameObject15IsInteractiveExEPS_
0038ad30: push     {r4, r5, r6, lr}
0038ad34: ldr      r3, [r0]
0038ad38: mov      r4, r0
0038ad3c: mov      r5, r1
0038ad40: mov      lr, pc
0038ad44: ldr      pc, [r3, #0x88]
0038ad48: cmp      r0, #0
0038ad4c: bne      #0x38ad54
0038ad50: pop      {r4, r5, r6, pc}
0038ad54: mov      r0, r4
0038ad58: mov      r1, r5
0038ad5c: ldr      r3, [r4]
0038ad60: mov      lr, pc
0038ad64: ldr      pc, [r3, #0x90]
0038ad68: adds     r0, r0, #1
0038ad6c: movne    r0, #1
0038ad70: pop      {r4, r5, r6, pc}

# _ZNK10GameObject20GetInteractionRadiusEv
0038ad7c: push     {r4, r5, r6, lr}
0038ad80: mov      r4, r0
0038ad84: ldr      r1, [r0, #0x144]
0038ad88: ldr      r0, [r0, #0x150]
0038ad8c: bl       #0x30e3ac
0038ad90: ldr      r1, [r4, #0x148]
0038ad94: mov      r5, r0
0038ad98: ldr      r0, [r4, #0x154]
0038ad9c: bl       #0x30e3ac
0038ada0: mov      r4, r0
0038ada4: mov      r1, r4
0038ada8: mov      r0, r5
0038adac: bl       #0x30e2f8
0038adb0: cmp      r0, #0
0038adb4: movne    r5, r4
0038adb8: mov      r0, r5
0038adbc: mov      r1, #0x3f000000
0038adc0: bl       #0x30ed6c
0038adc4: pop      {r4, r5, r6, pc}

# _ZNK10GameObject9IsZonableEv
003883b8: ldrb     r0, [r0, #0x2ed]
003883bc: eor      r0, r0, #1
003883c0: bx       lr

# _ZNK9Character6IsDeadEv
003a2ed4: movw     r3, #0x1449
003a2ed8: ldrb     r0, [r0, r3]
003a2edc: bx       lr

# _ZNK10GameObject18GetInteractionTypeEPS_
0038ad74: mvn      r0, #0
0038ad78: bx       lr

# _ZNK10ObjectBase11IsCharacterEv
0033dcd0: mov      r0, #0
0033dcd4: bx       lr
