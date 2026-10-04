
# _ZN10ObjectBaseC2ENS_6GO_IDSE
0033f310: push     {r4, r5, r6, r7, r8, lr}
0033f314: ldr      r6, [pc, #0x198]
0033f318: ldr      r2, [pc, #0x198]
0033f31c: ldr      r3, [pc, #0x198]
0033f320: add      r6, pc, r6
0033f324: ldr      r2, [r6, r2]
0033f328: ldr      r3, [r6, r3]
0033f32c: mov      r4, r0
0033f330: add      r2, r2, #8
0033f334: add      r0, r3, #8
0033f338: add      r3, r4, #8
0033f33c: str      r2, [r4]
0033f340: str      r0, [r4, #4]
0033f344: mov      r7, r1
0033f348: mov      r0, r3
0033f34c: str      r3, [r4, #0x18]
0033f350: str      r3, [r4, #0x1c]
0033f354: mov      r1, #0x10
0033f358: bl       #0x31167c
0033f35c: ldr      r2, [pc, #0x15c]
0033f360: ldr      r1, [r4, #0x18]
0033f364: mov      r5, #0
0033f368: ldr      r2, [r6, r2]
0033f36c: strb     r5, [r1]
0033f370: add      r3, r4, #0x30
0033f374: add      r1, r2, #0x74
0033f378: add      r0, r2, #8
0033f37c: add      r2, r2, #0x68
0033f380: stm      r4, {r0, r2}
0033f384: str      r1, [r4, #0x24]
0033f388: mov      r0, r3
0033f38c: str      r5, [r4, #0x20]
0033f390: strb     r5, [r4, #0x28]
0033f394: strb     r5, [r4, #0x29]
0033f398: str      r5, [r4, #0x2c]
0033f39c: str      r3, [r4, #0x40]
0033f3a0: str      r3, [r4, #0x44]
0033f3a4: mov      r1, #0x10
0033f3a8: bl       #0x31167c
0033f3ac: ldr      r2, [r4, #0x40]
0033f3b0: add      r3, r4, #0x48
0033f3b4: mov      r0, r3
0033f3b8: strb     r5, [r2]
0033f3bc: mov      r1, #0x10
0033f3c0: str      r3, [r4, #0x58]
0033f3c4: str      r3, [r4, #0x5c]
0033f3c8: bl       #0x31167c
0033f3cc: ldr      r2, [r4, #0x58]
0033f3d0: add      r3, r4, #0x68
0033f3d4: mvn      r6, #0
0033f3d8: strb     r5, [r2]
0033f3dc: mov      r1, #0x10
0033f3e0: mov      r0, r3
0033f3e4: strb     r5, [r4, #0x60]
0033f3e8: str      r3, [r4, #0x78]
0033f3ec: str      r3, [r4, #0x7c]
0033f3f0: str      r6, [r4, #0x64]
0033f3f4: bl       #0x31167c
0033f3f8: ldr      r3, [r4, #0x78]
0033f3fc: add      r0, r4, #0x8c
0033f400: strb     r5, [r3]
0033f404: mov      r3, #1
0033f408: strb     r3, [r4, #0x8a]
0033f40c: strb     r5, [r4, #0x81]
0033f410: strb     r5, [r4, #0x84]
0033f414: strb     r5, [r4, #0x85]
0033f418: strb     r5, [r4, #0x86]
0033f41c: strb     r5, [r4, #0x88]
0033f420: strb     r5, [r4, #0x89]
0033f424: bl       #0x33ed7c
0033f428: add      r0, r4, #0xb0
0033f42c: bl       #0x33ed7c
0033f430: add      r3, r4, #0xd4
0033f434: mov      r0, r3
0033f438: str      r3, [r4, #0xe4]
0033f43c: str      r3, [r4, #0xe8]
0033f440: mov      r1, #0x10
0033f444: bl       #0x31167c
0033f448: ldr      r3, [r4, #0xe4]
0033f44c: mov      r1, r5
0033f450: mov      r0, #0xc
0033f454: strb     r5, [r3]
0033f458: mov      r3, #0
0033f45c: str      r3, [r4, #0x114]
0033f460: strb     r5, [r4, #0xf0]
0033f464: strb     r5, [r4, #0xf1]
0033f468: strb     r5, [r4, #0xf8]
0033f46c: str      r5, [r4, #0xfc]
0033f470: str      r5, [r4, #0x100]
0033f474: str      r5, [r4, #0x104]
0033f478: strb     r5, [r4, #0x10c]
0033f47c: strb     r5, [r4, #0x118]
0033f480: strb     r5, [r4, #0x119]
0033f484: str      r5, [r4, #0x11c]
0033f488: str      r7, [r4, #0xf4]
0033f48c: str      r6, [r4, #0x110]
0033f490: str      r6, [r4, #0xec]
0033f494: str      r6, [r4, #0x108]
0033f498: bl       #0x310570
0033f49c: mov      r5, r0
0033f4a0: bl       #0x33f50c
0033f4a4: str      r5, [r4, #0x2c]
0033f4a8: mov      r0, r4
0033f4ac: str      r4, [r5, #4]
0033f4b0: pop      {r4, r5, r6, r7, r8, pc}
0033f4b4: rsbeq    r5, r5, r0, ror r7
0033f4b8: andeq    r1, r0, ip, lsl #1
0033f4bc: ldrdeq   r3, r4, [r0], -ip
0033f4c0: andeq    r3, r0, r4, lsl #23

# _ZN10ObjectBase17DeclarePropertiesEv
0033f014: push     {r4, r5, r6, lr}
0033f018: ldr      r1, [pc, #0x10c]
0033f01c: mov      r4, r0
0033f020: add      r5, r0, #4
0033f024: mov      r0, r5
0033f028: add      r2, r4, #0x84
0033f02c: ldrb     r3, [r4, #0x84]
0033f030: add      r1, pc, r1
0033f034: bl       #0x33e4ac
0033f038: ldr      r1, [pc, #0xf0]
0033f03c: mov      r3, #1
0033f040: mov      r0, r5
0033f044: add      r2, r4, #0x80
0033f048: add      r1, pc, r1
0033f04c: bl       #0x33e4ac
0033f050: ldr      r1, [pc, #0xdc]
0033f054: mov      r0, r5
0033f058: add      r2, r4, #0x30
0033f05c: add      r1, pc, r1
0033f060: bl       #0x33ef7c
0033f064: ldr      r1, [pc, #0xcc]
0033f068: mov      r0, r5
0033f06c: add      r2, r4, #0x48
0033f070: add      r1, pc, r1
0033f074: bl       #0x33ef7c
0033f078: ldr      r1, [pc, #0xbc]
0033f07c: mov      r0, r5
0033f080: add      r2, r4, #0x68
0033f084: add      r1, pc, r1
0033f088: bl       #0x33ef7c
0033f08c: ldr      r1, [pc, #0xac]
0033f090: mov      r0, r5
0033f094: add      r2, r4, #0x83
0033f098: add      r1, pc, r1
0033f09c: mov      r3, #0
0033f0a0: bl       #0x33e4ac
0033f0a4: ldr      r1, [pc, #0x98]
0033f0a8: mov      r3, #0
0033f0ac: mov      r0, r5
0033f0b0: add      r2, r4, #0x87
0033f0b4: add      r1, pc, r1
0033f0b8: bl       #0x33e4ac
0033f0bc: ldr      r1, [pc, #0x84]
0033f0c0: mov      r0, r5
0033f0c4: add      r2, r4, #0x90
0033f0c8: add      r1, pc, r1
0033f0cc: bl       #0x33ef7c
0033f0d0: ldr      r1, [pc, #0x74]
0033f0d4: mov      r0, r5
0033f0d8: add      r2, r4, #0xb4
0033f0dc: add      r1, pc, r1
0033f0e0: bl       #0x33ef7c
0033f0e4: ldr      r1, [pc, #0x64]
0033f0e8: mov      r0, r5
0033f0ec: add      r2, r4, #0xd4
0033f0f0: add      r1, pc, r1
0033f0f4: bl       #0x33ef7c
0033f0f8: ldr      r1, [pc, #0x54]
0033f0fc: mov      r0, r5
0033f100: add      r2, r4, #0xf0
0033f104: add      r1, pc, r1
0033f108: mov      r3, #0
0033f10c: bl       #0x33e4ac
0033f110: ldr      r1, [pc, #0x40]
0033f114: mov      r0, r5
0033f118: add      r2, r4, #0xf1
0033f11c: add      r1, pc, r1
0033f120: mov      r3, #0
0033f124: pop      {r4, r5, r6, lr}
0033f128: b        #0x33e4ac
0033f12c: subseq   r1, r8, r8, lsr r1
0033f130: subseq   r1, r8, r8, lsr #2
0033f134: subseq   r2, sl, ip, lsl #1
0033f138: subseq   r1, r8, r8, lsl #2
0033f13c: subseq   r1, r8, r4, lsl #2
0033f140: subseq   r1, r8, r0, lsl #2
0033f144: ldrsheq  r1, [r8], #-4
0033f148: ldrsheq  r1, [r8], #-0
0033f14c: subseq   r1, r8, ip, ror #1
0033f150: subseq   r1, r8, r8, ror #1
0033f154: subseq   r1, r8, r4, ror #1
0033f158: subseq   r1, r8, r4, ror #1

# _ZNK9Character11GetCharAIIdEv
003a2fec: ldr      r0, [r0, #0xffc]
003a2ff0: ldr      r3, [pc, #0x24]
003a2ff4: cmp      r0, #0
003a2ff8: add      r3, pc, r3
003a2ffc: blt      #0x3a3014
003a3000: ldr      r2, [pc, #0x18]
003a3004: ldr      r3, [r3, r2]
003a3008: ldr      r3, [r3]
003a300c: cmp      r0, r3
003a3010: bxlt     lr
003a3014: mov      r0, #8
003a3018: bx       lr

# _ZNK9Character9GetCharAIEv
003a3024: ldr      r3, [pc, #0x20]
003a3028: ldr      r2, [pc, #0x20]
003a302c: push     {r4, lr}
003a3030: add      r3, pc, r3
003a3034: ldr      r2, [r3, r2]
003a3038: ldr      r4, [r2]
003a303c: bl       #0x3a2fec
003a3040: mov      r3, #0x44
003a3044: mla      r0, r3, r0, r4
003a3048: pop      {r4, pc}
003a304c: subseq   r1, pc, r0, ror #20
003a3050: andeq    r0, r0, r8, asr r7

# _ZNK9Character11GetCharTypeEv
003a3054: push     {r4, lr}
003a3058: bl       #0x3a3024
003a305c: ldr      r0, [r0, #0x38]
003a3060: pop      {r4, pc}

# _ZNK9Character8IsPlayerEv
003a49f0: push     {r4, r5, r6, lr}
003a49f4: mov      r5, r0
003a49f8: bl       #0x3a3054
003a49fc: cmp      r0, #0
003a4a00: beq      #0x3a4a14
003a4a04: cmp      r0, #1
003a4a08: movne    r0, #0
003a4a0c: moveq    r0, #1
003a4a10: pop      {r4, r5, r6, pc}
003a4a14: ldr      r4, [r5, #0x44]
003a4a18: ldr      r1, [pc, #0x18]
003a4a1c: mov      r0, r4
003a4a20: add      r1, pc, r1
003a4a24: bl       #0x30ebd4
003a4a28: cmp      r4, r0
003a4a2c: movne    r0, #0
003a4a30: moveq    r0, #1
003a4a34: pop      {r4, r5, r6, pc}
003a4a38: subseq   lr, r1, r8, asr #14

# _ZN9Character18InitPhysicalObjectEv
003b4088: push     {r4, r5, r6, r7, r8, sl, lr}
003b408c: sub      sp, sp, #0x24
003b4090: mov      r5, r0
003b4094: bl       #0x3a30dc
003b4098: ldr      r4, [pc, #0x320]
003b409c: subs     r6, r0, #0
003b40a0: add      r4, pc, r4
003b40a4: bne      #0x3b41a8
003b40a8: ldrb     r7, [r5, #0x84]
003b40ac: cmp      r7, #0
003b40b0: bne      #0x3b412c
003b40b4: mov      r0, r5
003b40b8: bl       #0x3a310c
003b40bc: subs     r6, r0, #0
003b40c0: beq      #0x3b41f0
003b40c4: ldr      r3, [pc, #0x2f8]
003b40c8: mov      r1, r7
003b40cc: mov      r0, #0x28
003b40d0: ldr      r3, [r4, r3]
003b40d4: ldr      r8, [r3, #0x44]
003b40d8: bl       #0x310570
003b40dc: mov      ip, #0x400
003b40e0: mvn      r3, #3
003b40e4: str      ip, [sp]
003b40e8: mov      r1, r8
003b40ec: movw     ip, #0xd1f
003b40f0: mov      r2, r5
003b40f4: mov      r6, r0
003b40f8: str      ip, [sp, #4]
003b40fc: str      r7, [sp, #8]
003b4100: bl       #0x3b4010
003b4104: ldr      r3, [pc, #0x2bc]
003b4108: ldr      r3, [r4, r3]
003b410c: mov      r0, r5
003b4110: mov      r1, r6
003b4114: add      r3, r3, #8
003b4118: mov      r2, #1
003b411c: str      r3, [r6]
003b4120: add      sp, sp, #0x24
003b4124: pop      {r4, r5, r6, r7, r8, sl, lr}
003b4128: b        #0x394bf8
003b412c: ldr      r3, [pc, #0x290]
003b4130: mov      r1, r6
003b4134: mov      r0, #0x28
003b4138: ldr      r3, [r4, r3]
003b413c: mov      r7, #1
003b4140: ldr      sl, [r3, #0x44]
003b4144: bl       #0x310570
003b4148: mov      ip, #2
003b414c: mov      r1, sl
003b4150: mov      r2, r5
003b4154: mov      r3, r7
003b4158: str      ip, [sp, #0x10]
003b415c: movw     ip, #0xffff
003b4160: mov      r8, r0
003b4164: str      r6, [sp, #0xc]
003b4168: str      ip, [sp, #0x14]
003b416c: str      r6, [sp]
003b4170: str      r6, [sp, #4]
003b4174: str      r6, [sp, #8]
003b4178: str      r7, [sp, #0x18]
003b417c: bl       #0x46f2f0
003b4180: ldr      r3, [pc, #0x244]
003b4184: mov      r0, r5
003b4188: mov      r1, r8
003b418c: ldr      r3, [r4, r3]
003b4190: mov      r2, r7
003b4194: add      r3, r3, #8
003b4198: str      r3, [r8]
003b419c: add      sp, sp, #0x24
003b41a0: pop      {r4, r5, r6, r7, r8, sl, lr}
003b41a4: b        #0x394bf8
003b41a8: ldr      r3, [pc, #0x214]
003b41ac: mov      r1, #0
003b41b0: mov      r0, #0x28
003b41b4: ldr      r3, [r4, r3]
003b41b8: ldr      r7, [r3, #0x44]
003b41bc: bl       #0x310570
003b41c0: mov      ip, #0
003b41c4: mov      r3, ip
003b41c8: mov      lr, #0x100
003b41cc: mov      r1, r7
003b41d0: mov      r2, r5
003b41d4: mov      r6, r0
003b41d8: str      lr, [sp]
003b41dc: str      ip, [sp, #4]
003b41e0: str      ip, [sp, #8]
003b41e4: bl       #0x3b4010
003b41e8: ldr      r3, [pc, #0x1e0]
003b41ec: b        #0x3b4108
003b41f0: ldr      r3, [r5]
003b41f4: mov      r0, r5
003b41f8: mov      lr, pc
003b41fc: ldr      pc, [r3, #0x28]
003b4200: cmp      r0, #0
003b4204: bne      #0x3b4264
003b4208: mov      r0, r5
003b420c: bl       #0x3a307c
003b4210: cmp      r0, #0
003b4214: beq      #0x3b42d0
003b4218: ldr      r3, [pc, #0x1a4]
003b421c: mov      r1, #0
003b4220: mov      r0, #0x28
003b4224: ldr      r3, [r4, r3]
003b4228: ldr      r7, [r3, #0x44]
003b422c: bl       #0x310570
003b4230: mov      ip, #8
003b4234: str      ip, [sp]
003b4238: movw     ip, #0xd3b
003b423c: mvn      r3, #1
003b4240: str      ip, [sp, #4]
003b4244: mov      r1, r7
003b4248: mov      ip, #0
003b424c: mov      r2, r5
003b4250: mov      r6, r0
003b4254: str      ip, [sp, #8]
003b4258: bl       #0x3b4010
003b425c: ldr      r3, [pc, #0x170]
003b4260: b        #0x3b4108
003b4264: ldr      r3, [pc, #0x158]
003b4268: mov      r1, r6
003b426c: mov      r0, #0x28
003b4270: ldr      r3, [r4, r3]
003b4274: mov      r7, #1
003b4278: ldr      r8, [r3, #0x44]
003b427c: bl       #0x310570
003b4280: mov      ip, #4
003b4284: mov      r1, r8
003b4288: mov      r2, r5
003b428c: str      ip, [sp]
003b4290: mvn      r3, #0
003b4294: movw     ip, #0xd7f
003b4298: mov      r6, r0
003b429c: str      ip, [sp, #4]
003b42a0: str      r7, [sp, #8]
003b42a4: bl       #0x3b4010
003b42a8: ldr      r3, [pc, #0x128]
003b42ac: mov      r0, r5
003b42b0: mov      r1, r6
003b42b4: ldr      r3, [r4, r3]
003b42b8: mov      r2, r7
003b42bc: add      r3, r3, #8
003b42c0: str      r3, [r6]
003b42c4: add      sp, sp, #0x24
003b42c8: pop      {r4, r5, r6, r7, r8, sl, lr}
003b42cc: b        #0x394bf8
003b42d0: mov      r0, r5
003b42d4: bl       #0x3a30ac
003b42d8: subs     r6, r0, #0
003b42dc: bne      #0x3b4218
003b42e0: mov      r0, r5
003b42e4: bl       #0x3a3094
003b42e8: subs     r7, r0, #0
003b42ec: beq      #0x3b4354
003b42f0: ldr      r3, [pc, #0xcc]
003b42f4: mov      r1, r6
003b42f8: mov      r0, #0x28
003b42fc: ldr      r3, [r4, r3]
003b4300: ldr      r8, [r3, #0x44]
003b4304: bl       #0x310570
003b4308: mov      r1, r8
003b430c: mov      r3, r6
003b4310: mov      r2, r5
003b4314: mov      ip, #0x100
003b4318: mov      r7, r0
003b431c: str      ip, [sp]
003b4320: str      r6, [sp, #4]
003b4324: str      r6, [sp, #8]
003b4328: bl       #0x3b4010
003b432c: ldr      r3, [pc, #0x9c]
003b4330: mov      r0, r5
003b4334: mov      r1, r7
003b4338: ldr      r3, [r4, r3]
003b433c: mov      r2, #1
003b4340: add      r3, r3, #8
003b4344: str      r3, [r7]
003b4348: add      sp, sp, #0x24
003b434c: pop      {r4, r5, r6, r7, r8, sl, lr}
003b4350: b        #0x394bf8
003b4354: mov      r0, r5
003b4358: bl       #0x3a3064
003b435c: subs     r1, r0, #0
003b4360: beq      #0x3b43ac
003b4364: ldr      r3, [pc, #0x58]
003b4368: mov      r1, r7
003b436c: mov      r0, #0x28
003b4370: ldr      r3, [r4, r3]
003b4374: ldr      r8, [r3, #0x44]
003b4378: bl       #0x310570
003b437c: mov      ip, #0x10
003b4380: mov      r3, #2
003b4384: str      ip, [sp]
003b4388: mov      r1, r8
003b438c: movw     ip, #0xd3f
003b4390: mov      r2, r5
003b4394: mov      r6, r0
003b4398: str      ip, [sp, #4]
003b439c: str      r7, [sp, #8]
003b43a0: bl       #0x3b4010
003b43a4: ldr      r3, [pc, #0x30]
003b43a8: b        #0x3b4108
003b43ac: mov      r0, r5
003b43b0: mov      r2, r1
003b43b4: add      sp, sp, #0x24
003b43b8: pop      {r4, r5, r6, r7, r8, sl, lr}
003b43bc: b        #0x394bf8
003b43c0: ldrsheq  r0, [lr], #-0x90
003b43c4: strdeq   r3, r4, [r0], -r4
003b43c8: andeq    r3, r0, ip, asr pc
003b43cc: andeq    r2, r0, r8, lsl r4
003b43d0: andeq    r0, r0, ip, asr #26
003b43d4: andeq    r3, r0, r4, lsr #4
003b43d8: andeq    r1, r0, ip, asr sp
003b43dc: andeq    r1, r0, r0, lsr #5

# _ZN9Character15SetRelativeAABBERK4aabbIfEb
003a4398: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a439c: ldr      r5, [r1]
003a43a0: cmp      r2, #0
003a43a4: sub      sp, sp, #0xc
003a43a8: str      r5, [r0, #0x144]
003a43ac: ldr      fp, [r1, #4]
003a43b0: mov      r4, r0
003a43b4: str      fp, [r0, #0x148]
003a43b8: ldr      sb, [r1, #8]
003a43bc: str      sb, [r0, #0x14c]
003a43c0: ldr      sl, [r1, #0xc]
003a43c4: str      sl, [r0, #0x150]
003a43c8: ldr      r8, [r1, #0x10]
003a43cc: str      r8, [r0, #0x154]
003a43d0: ldr      r7, [r1, #0x14]
003a43d4: str      r7, [r0, #0x158]
003a43d8: bne      #0x3a445c
003a43dc: movw     r3, #0x1038
003a43e0: ldr      r0, [r0, r3]
003a43e4: str      r2, [sp, #4]
003a43e8: bl       #0x30e964
003a43ec: movw     r1, #0xd70a
003a43f0: movt     r1, #0x3c23
003a43f4: bl       #0x30ed6c
003a43f8: mov      r1, r5
003a43fc: mov      r6, r0
003a4400: bl       #0x30ed6c
003a4404: mov      r1, fp
003a4408: str      r0, [r4, #0x144]
003a440c: mov      r0, r6
003a4410: bl       #0x30ed6c
003a4414: mov      r1, sb
003a4418: str      r0, [r4, #0x148]
003a441c: mov      r0, r6
003a4420: bl       #0x30ed6c
003a4424: mov      r1, sl
003a4428: str      r0, [r4, #0x14c]
003a442c: mov      r0, r6
003a4430: bl       #0x30ed6c
003a4434: mov      r1, r8
003a4438: str      r0, [r4, #0x150]
003a443c: mov      r0, r6
003a4440: bl       #0x30ed6c
003a4444: mov      r1, r7
003a4448: str      r0, [r4, #0x154]
003a444c: mov      r0, r6
003a4450: bl       #0x30ed6c
003a4454: str      r0, [r4, #0x158]
003a4458: ldr      r2, [sp, #4]
003a445c: mov      r0, r4
003a4460: add      r1, r4, #0x144
003a4464: add      sp, sp, #0xc
003a4468: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a446c: b        #0x38b110
