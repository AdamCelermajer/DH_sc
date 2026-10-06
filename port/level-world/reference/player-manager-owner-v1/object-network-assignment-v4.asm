# _ZN13ObjectManager21AssignObjectNetworkIdEP10ObjectBase
003431c0: push {r4, r5, r6, r7, r8, lr}
003431c4: mov r5, r0
003431c8: mov r4, r1
003431cc: bl #0x7fd794
003431d0: ldrb r3, [r0, #5]
003431d4: ldr r6, [pc, #0x110]
003431d8: cmp r3, #0
003431dc: add r6, pc, r6
003431e0: beq #0x343274
003431e4: ldr r7, [r4, #0x44]
003431e8: ldr r1, [pc, #0x100]
003431ec: mov r0, r7
003431f0: add r1, pc, r1
003431f4: bl #0x30ebd4
003431f8: cmp r0, r7
003431fc: beq #0x3432d0
00343200: ldr r3, [r4]
00343204: mov r0, r4
00343208: mov lr, pc
0034320c: ldr pc, [r3, #0x24]
00343210: cmp r0, #0
00343214: bne #0x343278
00343218: ldr r0, [r5, #0x13c]
0034321c: add r3, r0, #1
00343220: str r3, [r5, #0x13c]
00343224: add r0, r0, #5
00343228: cmp r0, #4
0034322c: str r0, [r4, #0x108]
00343230: bgt #0x3432b4
00343234: add r6, r5, #0x100
00343238: mov r0, r6
0034323c: bl #0x343168
00343240: str r4, [r0, #8]
00343244: ldr r3, [r5, #0x104]
00343248: str r6, [r0]
0034324c: str r3, [r0, #4]
00343250: str r0, [r3]
00343254: str r0, [r5, #0x104]
00343258: ldr r3, [r4, #0x100]
0034325c: cmp r3, #0
00343260: beq #0x343274
00343264: ldr r3, [r5, #0x54]
00343268: add r3, r3, #1
0034326c: str r3, [r5, #0x54]
00343270: pop {r4, r5, r6, r7, r8, pc}
00343274: pop {r4, r5, r6, r7, r8, pc}
00343278: movw r3, #0x14e4
0034327c: ldrb r3, [r4, r3]
00343280: cmp r3, #0
00343284: beq #0x3432a0
00343288: ldr r0, [r5, #0x140]
0034328c: add r3, r0, #1
00343290: add r0, r0, #0x2700
00343294: str r3, [r5, #0x140]
00343298: add r0, r0, #0x11
0034329c: b #0x343228
003432a0: mov r0, r4
003432a4: bl #0x3a30ac
003432a8: cmp r0, #0
003432ac: beq #0x343218
003432b0: b #0x343288
003432b4: ldr r3, [pc, #0x38]
003432b8: mov r1, #1
003432bc: ldr r3, [r6, r3]
003432c0: ldr r3, [r3]
003432c4: str r3, [r4, #0xfc]
003432c8: bl #0x33ff90
003432cc: b #0x343234
003432d0: add r0, r0, #0x10
003432d4: bl #0x30e094
003432d8: ldr r3, [r5, #0x138]
003432dc: add r0, r0, #1
003432e0: add r3, r3, #1
003432e4: str r3, [r5, #0x138]
003432e8: b #0x343228
003432ec: strhteq r1, [r5], #-0x84
003432f0: subseq sp, r7, r8, asr r1
003432f4: andeq r0, r0, r0, lsl fp
