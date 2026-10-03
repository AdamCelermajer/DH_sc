
# _ZN12CharAIScriptC2Eb
003d8fb0: push     {r4, r5, r6, lr}
003d8fb4: ldr      r5, [pc, #0x58]
003d8fb8: mov      r4, r0
003d8fbc: mov      r6, r1
003d8fc0: bl       #0x37c674
003d8fc4: ldr      r1, [pc, #0x4c]
003d8fc8: add      r5, pc, r5
003d8fcc: mov      r3, #0
003d8fd0: ldr      r1, [r5, r1]
003d8fd4: mov      r2, r4
003d8fd8: str      r3, [r4, #0x98]
003d8fdc: add      r1, r1, #8
003d8fe0: str      r1, [r4]
003d8fe4: str      r3, [r4, #0xa0]
003d8fe8: cmp      r6, r3
003d8fec: strb     r3, [r2, #0x9c]!
003d8ff0: str      r2, [r4, #0xa8]
003d8ff4: str      r3, [r4, #0xb4]
003d8ff8: str      r2, [r4, #0xa4]
003d8ffc: str      r3, [r4, #0xac]
003d9000: bne      #0x3d900c
003d9004: mov      r0, r4
003d9008: bl       #0x3d8ec8
003d900c: mov      r0, r4
003d9010: pop      {r4, r5, r6, pc}
003d9014: subseq   fp, fp, r8, asr #21
003d9018: muleq    r0, ip, fp

# _ZN3sfc6script3lua8InstanceC1EP9lua_State
0031aa20: ldr      r3, [pc, #0x20]
0031aa24: ldr      ip, [pc, #0x20]
0031aa28: str      r1, [r0, #4]
0031aa2c: add      r3, pc, r3
0031aa30: ldr      ip, [r3, ip]
0031aa34: mov      r1, #0
0031aa38: strb     r1, [r0, #8]
0031aa3c: add      ip, ip, #8
0031aa40: str      ip, [r0]
0031aa44: bx       lr
0031aa48: rsbeq    sl, r7, r4, rrx
0031aa4c: andeq    r4, r0, r8, lsl r0

# _ZN3sfc6script3lua8InstanceC1Ev
0031b268: ldr      r3, [pc, #0x30]
0031b26c: ldr      r2, [pc, #0x30]
0031b270: mov      r1, #1
0031b274: add      r3, pc, r3
0031b278: ldr      r2, [r3, r2]
0031b27c: push     {r4, lr}
0031b280: add      r2, r2, #8
0031b284: strb     r1, [r0, #8]
0031b288: str      r2, [r0]
0031b28c: mov      r4, r0
0031b290: bl       #0x31b224
0031b294: str      r0, [r4, #4]
0031b298: mov      r0, r4
0031b29c: pop      {r4, pc}
0031b2a0: rsbeq    sb, r7, ip, lsl r8
0031b2a4: andeq    r4, r0, r8, lsl r0

# _ZN6CharAI16StepCreateScriptEv
003cf04c: push     {r4, r5, r6, r7, r8, sl, lr}
003cf050: ldr      r4, [pc, #0x17c]
003cf054: ldr      r6, [pc, #0x17c]
003cf058: ldr      r2, [pc, #0x17c]
003cf05c: add      r4, pc, r4
003cf060: ldr      r3, [r4, r6]
003cf064: ldr      r2, [r4, r2]
003cf068: sub      sp, sp, #0x44
003cf06c: ldr      r3, [r3]
003cf070: mov      r5, r0
003cf074: ldr      r0, [r0, #4]
003cf078: ldr      r7, [r2]
003cf07c: str      r3, [sp, #0x3c]
003cf080: bl       #0x3a2fec
003cf084: mov      r3, #0x44
003cf088: mla      r7, r3, r0, r7
003cf08c: ldr      r3, [r7, #0x28]
003cf090: cmp      r3, #0
003cf094: beq      #0x3cf124
003cf098: ldr      r3, [pc, #0x140]
003cf09c: add      r8, sp, #0x24
003cf0a0: ldr      sl, [r4, r3]
003cf0a4: mov      r0, sl
003cf0a8: bl       #0x337888
003cf0ac: ldr      r1, [pc, #0x130]
003cf0b0: add      r2, sp, #8
003cf0b4: mov      r0, r8
003cf0b8: add      r1, pc, r1
003cf0bc: bl       #0x3140ec
003cf0c0: mov      r0, sl
003cf0c4: mov      r1, r8
003cf0c8: bl       #0x337a88
003cf0cc: ldr      r0, [sp, #0x38]
003cf0d0: cmp      r0, r8
003cf0d4: beq      #0x3cf0f4
003cf0d8: cmp      r0, #0
003cf0dc: beq      #0x3cf0f4
003cf0e0: ldr      r1, [sp, #0x24]
003cf0e4: rsb      r1, r0, r1
003cf0e8: cmp      r1, #0x80
003cf0ec: bhi      #0x3cf1c8
003cf0f0: bl       #0x708f00
003cf0f4: ldr      r1, [r7, #0x2c]
003cf0f8: mov      r0, r5
003cf0fc: bl       #0x3ceeb0
003cf100: mov      r3, #1
003cf104: strb     r3, [r5, #0x2c]
003cf108: ldr      r3, [r4, r6]
003cf10c: ldr      r2, [sp, #0x3c]
003cf110: ldr      r3, [r3]
003cf114: cmp      r2, r3
003cf118: bne      #0x3cf1d0
003cf11c: add      sp, sp, #0x44
003cf120: pop      {r4, r5, r6, r7, r8, sl, pc}
003cf124: ldr      r3, [pc, #0xb4]
003cf128: add      r7, sp, #0xc
003cf12c: ldr      r8, [r4, r3]
003cf130: mov      r0, r8
003cf134: bl       #0x337888
003cf138: ldr      r1, [pc, #0xa8]
003cf13c: add      r2, sp, #4
003cf140: mov      r0, r7
003cf144: add      r1, pc, r1
003cf148: bl       #0x3140ec
003cf14c: mov      r0, r8
003cf150: mov      r1, r7
003cf154: bl       #0x337a88
003cf158: ldr      r0, [sp, #0x20]
003cf15c: cmp      r0, r7
003cf160: beq      #0x3cf180
003cf164: cmp      r0, #0
003cf168: beq      #0x3cf180
003cf16c: ldr      r1, [sp, #0xc]
003cf170: rsb      r1, r0, r1
003cf174: cmp      r1, #0x80
003cf178: bhi      #0x3cf1c0
003cf17c: bl       #0x708f00
003cf180: ldr      r3, [r5, #4]
003cf184: ldr      r1, [pc, #0x60]
003cf188: ldr      r0, [r3, #0x5c]
003cf18c: add      r1, pc, r1
003cf190: bl       #0x30e31c
003cf194: cmp      r0, #0
003cf198: beq      #0x3cf1b4
003cf19c: mov      r0, r5
003cf1a0: bl       #0x3cce14
003cf1a4: mov      r3, #0
003cf1a8: str      r3, [r5, #0x30]
003cf1ac: strb     r3, [r5, #0x2c]
003cf1b0: b        #0x3cf108
003cf1b4: mov      r0, r5
003cf1b8: bl       #0x3cd11c
003cf1bc: b        #0x3cf1a4
003cf1c0: bl       #0x310440
003cf1c4: b        #0x3cf180
003cf1c8: bl       #0x310440
003cf1cc: b        #0x3cf0f4
003cf1d0: bl       #0x30e310
003cf1d4: subseq   r5, ip, r4, lsr sl
003cf1d8: andeq    r4, r0, ip, lsr #1
003cf1dc: andeq    r0, r0, r8, asr r7
003cf1e0: andeq    r0, r0, r4, lsl #17
003cf1e4: subeq    r4, pc, r8, asr sp
003cf1e8: subeq    r4, pc, ip, asr #25
003cf1ec: subeq    r1, pc, ip, lsr r2

# _ZN3sfc6script3lua8InstanceC2EP9lua_State
0031a9f0: ldr      r3, [pc, #0x20]
0031a9f4: ldr      ip, [pc, #0x20]
0031a9f8: str      r1, [r0, #4]
0031a9fc: add      r3, pc, r3
0031aa00: ldr      ip, [r3, ip]
0031aa04: mov      r1, #0
0031aa08: strb     r1, [r0, #8]
0031aa0c: add      ip, ip, #8
0031aa10: str      ip, [r0]
0031aa14: bx       lr
0031aa18: mlseq    r7, r4, r0, sl
0031aa1c: andeq    r4, r0, r8, lsl r0

# _ZN12CharAIScriptD1Ev
003d926c: push     {r4, r5, r6, lr}
003d9270: ldr      r3, [pc, #0x54]
003d9274: ldr      r2, [pc, #0x54]
003d9278: ldr      r1, [r0, #0xac]
003d927c: add      r3, pc, r3
003d9280: ldr      r2, [r3, r2]
003d9284: cmp      r1, #0
003d9288: mov      r4, r0
003d928c: add      r2, r2, #8
003d9290: str      r2, [r0]
003d9294: beq      #0x3d92bc
003d9298: add      r5, r0, #0x9c
003d929c: mov      r0, r5
003d92a0: ldr      r1, [r4, #0xa0]
003d92a4: bl       #0x3d91e8
003d92a8: mov      r3, #0
003d92ac: str      r5, [r4, #0xa8]
003d92b0: str      r3, [r4, #0xac]
003d92b4: str      r5, [r4, #0xa4]
003d92b8: str      r3, [r4, #0xa0]
003d92bc: mov      r0, r4
003d92c0: bl       #0x37bec0
003d92c4: mov      r0, r4
003d92c8: pop      {r4, r5, r6, pc}
003d92cc: subseq   fp, fp, r4, lsl r8
003d92d0: muleq    r0, ip, fp

# _ZN12CharAIScript12SetCharacterEP9Character
003d90f8: push     {r4, r5, lr}
003d90fc: ldr      r3, [pc, #0x98]
003d9100: subs     r4, r1, #0
003d9104: sub      sp, sp, #0xc
003d9108: mov      r5, r0
003d910c: add      r3, pc, r3
003d9110: beq      #0x3d9148
003d9114: str      r4, [r5, #0x98]
003d9118: mov      r0, r4
003d911c: add      r1, r5, #0x10
003d9120: ldr      r3, [r4]
003d9124: mov      lr, pc
003d9128: ldr      pc, [r3, #0xc]
003d912c: ldr      r1, [pc, #0x6c]
003d9130: add      r0, r5, #0x68
003d9134: add      r1, pc, r1
003d9138: add      r2, r1, #0x10
003d913c: add      sp, sp, #0xc
003d9140: pop      {r4, r5, lr}
003d9144: b        #0x3109e0
003d9148: ldr      r2, [pc, #0x54]
003d914c: ldr      r2, [r3, r2]
003d9150: ldr      r2, [r2]
003d9154: cmp      r2, #2
003d9158: streq    r4, [r4]
003d915c: beq      #0x3d9114
003d9160: cmp      r2, #1
003d9164: bne      #0x3d9114
003d9168: ldr      r0, [pc, #0x38]
003d916c: ldr      r1, [pc, #0x38]
003d9170: ldr      r2, [pc, #0x38]
003d9174: ldr      r0, [r3, r0]
003d9178: ldr      r3, [pc, #0x34]
003d917c: mov      ip, #0x83
003d9180: add      r1, pc, r1
003d9184: add      r2, pc, r2
003d9188: add      r3, pc, r3
003d918c: add      r0, r0, #0xa8
003d9190: str      ip, [sp]
003d9194: bl       #0x30e004
003d9198: b        #0x3d9114
003d919c: subseq   fp, fp, r4, lsl #19
003d91a0: subeq    sl, lr, ip, lsl r5
003d91a4: andeq    r3, r0, r0, asr #19
003d91a8: andeq    r1, r0, r0, asr #19
003d91ac: subeq    r5, lr, r8, asr r2
003d91b0: subseq   r8, r1, r4, lsl #30
003d91b4: subeq    ip, lr, r0, ror r6

# _ZN6CharAI9SetScriptI15AISPlayerIPhoneEEvv
003ccfe4: push     {r4, r5, r6, r7, lr}
003ccfe8: ldr      r3, [r0, #4]
003ccfec: ldr      r5, [pc, #0x108]
003ccff0: sub      sp, sp, #0xc
003ccff4: cmp      r3, #0
003ccff8: mov      r4, r0
003ccffc: add      r5, pc, r5
003cd000: beq      #0x3cd0a8
003cd004: ldr      r3, [r4, #0x20]
003cd008: cmp      r3, #0
003cd00c: beq      #0x3cd044
003cd010: ldr      r3, [r4]
003cd014: mov      r0, r4
003cd018: mov      lr, pc
003cd01c: ldr      pc, [r3, #0x14]
003cd020: ldr      r3, [r4, #0x20]
003cd024: cmp      r3, #0
003cd028: beq      #0x3cd044
003cd02c: mov      r0, r3
003cd030: ldr      r3, [r3]
003cd034: mov      lr, pc
003cd038: ldr      pc, [r3, #4]
003cd03c: mov      r3, #0
003cd040: str      r3, [r4, #0x20]
003cd044: mov      r1, #0
003cd048: mov      r0, #0xd8
003cd04c: bl       #0x310570
003cd050: mov      r1, #1
003cd054: mov      r6, r0
003cd058: bl       #0x3d8fb0
003cd05c: ldr      r3, [pc, #0x9c]
003cd060: mov      r7, #0
003cd064: mov      r0, r6
003cd068: ldr      r3, [r5, r3]
003cd06c: str      r7, [r6, #0xb8]
003cd070: str      r7, [r6, #0xbc]
003cd074: add      r3, r3, #8
003cd078: str      r7, [r6, #0xc0]
003cd07c: str      r3, [r0], #0xc4
003cd080: bl       #0x3ccf9c
003cd084: ldr      r3, [pc, #0x78]
003cd088: str      r7, [r6, #0xd4]
003cd08c: str      r7, [r6, #0xd0]
003cd090: ldr      r3, [r5, r3]
003cd094: add      r3, r3, #8
003cd098: str      r3, [r6]
003cd09c: str      r6, [r4, #0x20]
003cd0a0: add      sp, sp, #0xc
003cd0a4: pop      {r4, r5, r6, r7, pc}
003cd0a8: ldr      r2, [pc, #0x58]
003cd0ac: ldr      r2, [r5, r2]
003cd0b0: ldr      r2, [r2]
003cd0b4: cmp      r2, #2
003cd0b8: streq    r3, [r3]
003cd0bc: beq      #0x3cd004
003cd0c0: cmp      r2, #1
003cd0c4: bne      #0x3cd004
003cd0c8: ldr      r0, [pc, #0x3c]
003cd0cc: ldr      r1, [pc, #0x3c]
003cd0d0: ldr      r2, [pc, #0x3c]
003cd0d4: ldr      r0, [r5, r0]
003cd0d8: ldr      r3, [pc, #0x38]
003cd0dc: movw     ip, #0x2a1
003cd0e0: add      r1, pc, r1
003cd0e4: add      r2, pc, r2
003cd0e8: add      r3, pc, r3
003cd0ec: add      r0, r0, #0xa8
003cd0f0: str      ip, [sp]
003cd0f4: bl       #0x30e004
003cd0f8: b        #0x3cd004

# _ZN12CharAIScript24CharAIScriptBindFunctionEv
003d8ec8: push     {r4, r5, r6, lr}
003d8ecc: ldr      r4, [pc, #0x44]
003d8ed0: ldr      r3, [pc, #0x44]
003d8ed4: ldr      r1, [pc, #0x44]
003d8ed8: mov      r5, r0
003d8edc: add      r4, pc, r4
003d8ee0: add      r6, r0, #0x10
003d8ee4: ldr      r2, [r4, r3]
003d8ee8: mov      r0, r6
003d8eec: mov      r3, r5
003d8ef0: add      r1, pc, r1
003d8ef4: bl       #0x31a4d4
003d8ef8: ldr      r3, [pc, #0x24]
003d8efc: ldr      r1, [pc, #0x24]
003d8f00: mov      r0, r6
003d8f04: ldr      r2, [r4, r3]
003d8f08: add      r1, pc, r1
003d8f0c: mov      r3, r5
003d8f10: pop      {r4, r5, r6, lr}
003d8f14: b        #0x31a4d4
003d8f18: ldrheq   fp, [fp], #-0xb4
003d8f1c: andeq    r4, r0, ip, asr r6
003d8f20: subeq    ip, lr, r8, ror #17
003d8f24: andeq    r1, r0, r0, ror sb
003d8f28: subeq    ip, lr, r0, ror #17

# _ZN12CharAIScript12BindFunctionEv
003d8f2c: push     {r4, lr}
003d8f30: mov      r4, r0
003d8f34: bl       #0x37b5a0
003d8f38: mov      r0, r4
003d8f3c: pop      {r4, lr}
003d8f40: b        #0x3d8ec8

# _ZN12CharAIScriptC1Eb
003d8f44: push     {r4, r5, r6, lr}
003d8f48: ldr      r5, [pc, #0x58]
003d8f4c: mov      r4, r0
003d8f50: mov      r6, r1
003d8f54: bl       #0x37c674
003d8f58: ldr      r1, [pc, #0x4c]
003d8f5c: add      r5, pc, r5
003d8f60: mov      r3, #0
003d8f64: ldr      r1, [r5, r1]
003d8f68: mov      r2, r4
003d8f6c: str      r3, [r4, #0x98]
003d8f70: add      r1, r1, #8
003d8f74: str      r1, [r4]
003d8f78: str      r3, [r4, #0xa0]
003d8f7c: cmp      r6, r3
003d8f80: strb     r3, [r2, #0x9c]!
003d8f84: str      r2, [r4, #0xa8]
003d8f88: str      r3, [r4, #0xb4]
003d8f8c: str      r2, [r4, #0xa4]
003d8f90: str      r3, [r4, #0xac]
003d8f94: bne      #0x3d8fa0
003d8f98: mov      r0, r4
003d8f9c: bl       #0x3d8ec8
003d8fa0: mov      r0, r4
003d8fa4: pop      {r4, r5, r6, pc}
003d8fa8: subseq   fp, fp, r4, lsr fp
003d8fac: muleq    r0, ip, fp

# _ZN10LuaManagerC1Ev
00379eb0: ldr      r1, [pc, #0x38]
00379eb4: str      r4, [sp, #-4]!
00379eb8: ldr      r4, [pc, #0x34]
00379ebc: add      r1, pc, r1
00379ec0: mov      ip, #0
00379ec4: ldr      r4, [r1, r4]
00379ec8: mov      r2, r0
00379ecc: str      ip, [r0, #8]
00379ed0: add      r4, r4, #8
00379ed4: str      r4, [r0]
00379ed8: strb     ip, [r2, #4]!
00379edc: str      r2, [r0, #0x10]
00379ee0: str      ip, [r0, #0x14]
00379ee4: str      r2, [r0, #0xc]
00379ee8: ldm      sp!, {r4}
00379eec: bx       lr

# _ZN3sfc6script3lua8InstanceD1Ev
0031b180: push     {r4, lr}
0031b184: ldr      r3, [pc, #0x30]
0031b188: ldr      r2, [pc, #0x30]
0031b18c: ldrb     r1, [r0, #8]
0031b190: add      r3, pc, r3
0031b194: ldr      r2, [r3, r2]
0031b198: cmp      r1, #0
0031b19c: mov      r4, r0
0031b1a0: add      r2, r2, #8
0031b1a4: str      r2, [r0]
0031b1a8: beq      #0x31b1b4
0031b1ac: ldr      r0, [r0, #4]
0031b1b0: bl       #0x85797c
0031b1b4: mov      r0, r4
0031b1b8: pop      {r4, pc}
0031b1bc: rsbeq    sb, r7, r0, lsl #18
0031b1c0: andeq    r4, r0, r8, lsl r0

# _ZN9AISPlayerD2Ev
003de100: push     {r4, r5, r6, lr}
003de104: ldr      r5, [pc, #0x5c]
003de108: ldr      r3, [pc, #0x5c]
003de10c: mov      r2, r0
003de110: add      r5, pc, r5
003de114: ldr      r3, [r5, r3]
003de118: mov      r4, r0
003de11c: add      r3, r3, #8
003de120: str      r3, [r2], #0xc4
003de124: ldr      r3, [r0, #0xc4]
003de128: cmp      r3, #0
003de12c: beq      #0x3de148
003de130: ldr      r2, [r2, #8]
003de134: mov      r1, r3
003de138: add      r0, r0, #0xcc
003de13c: rsb      r3, r3, r2
003de140: asr      r2, r3, #2
003de144: bl       #0x3de0e4
003de148: ldr      r3, [pc, #0x20]
003de14c: mov      r0, r4
003de150: ldr      r3, [r5, r3]
003de154: add      r3, r3, #8
003de158: str      r3, [r4]
003de15c: bl       #0x3d92f0
003de160: mov      r0, r4
003de164: pop      {r4, r5, r6, pc}
003de168: subseq   r6, fp, r0, lsl #19
003de16c: andeq    r1, r0, r0, lsl fp
003de170: andeq    r2, r0, ip, lsr #21

# _ZN9LuaScriptC1Eb
0037c584: push     {r4, r5, r6, r7, r8, lr}
0037c588: ldr      r6, [pc, #0xd8]
0037c58c: ldr      r3, [pc, #0xd8]
0037c590: mov      r7, r0
0037c594: add      r6, pc, r6
0037c598: ldr      r3, [r6, r3]
0037c59c: mov      r4, r0
0037c5a0: mov      r8, r1
0037c5a4: add      r3, r3, #8
0037c5a8: str      r3, [r7], #4
0037c5ac: mov      r0, r7
0037c5b0: bl       #0x31b268
0037c5b4: ldr      r2, [pc, #0xb4]
0037c5b8: mov      r5, #0
0037c5bc: mov      r3, r4
0037c5c0: ldr      r2, [r6, r2]
0037c5c4: str      r7, [r4, #0x14]
0037c5c8: str      r5, [r4, #0x18]
0037c5cc: add      r2, r2, #8
0037c5d0: str      r2, [r4, #0x10]
0037c5d4: str      r5, [r4, #0x20]
0037c5d8: mov      r2, r4
0037c5dc: strb     r5, [r3, #0x1c]!
0037c5e0: str      r3, [r4, #0x28]
0037c5e4: str      r3, [r4, #0x24]
0037c5e8: str      r5, [r4, #0x2c]
0037c5ec: mov      r3, r4
0037c5f0: str      r5, [r4, #0x38]
0037c5f4: strb     r5, [r2, #0x34]!
0037c5f8: str      r2, [r4, #0x40]
0037c5fc: str      r2, [r4, #0x3c]
0037c600: add      r0, r4, #0x68
0037c604: str      r5, [r4, #0x44]
0037c608: str      r5, [r4, #0x50]
0037c60c: strb     r5, [r3, #0x4c]!
0037c610: str      r3, [r4, #0x58]
0037c614: str      r3, [r4, #0x54]
0037c618: str      r5, [r4, #0x5c]
0037c61c: strb     r5, [r4, #0x64]
0037c620: str      r0, [r4, #0x78]
0037c624: str      r0, [r4, #0x7c]
0037c628: mov      r1, #0x10
0037c62c: bl       #0x31167c
0037c630: ldr      r2, [r4, #0x78]
0037c634: mov      r3, r4
0037c638: cmp      r8, r5
0037c63c: strb     r5, [r2]
0037c640: str      r5, [r4, #0x84]
0037c644: strb     r5, [r3, #0x80]!
0037c648: str      r3, [r4, #0x8c]
0037c64c: str      r5, [r4, #0x90]
0037c650: str      r3, [r4, #0x88]
0037c654: bne      #0x37c660
0037c658: mov      r0, r4
0037c65c: bl       #0x37b5a0
0037c660: mov      r0, r4
0037c664: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6CharAI16StepSetCharacterEv
003cc26c: ldr      r1, [r0, #4]
003cc270: ldr      r0, [r0, #0x20]
003cc274: b        #0x3d90f8

# _ZN12CharAIScriptD2Ev
003d92f0: push     {r4, r5, r6, lr}
003d92f4: ldr      r3, [pc, #0x54]
003d92f8: ldr      r2, [pc, #0x54]
003d92fc: ldr      r1, [r0, #0xac]
003d9300: add      r3, pc, r3
003d9304: ldr      r2, [r3, r2]
003d9308: cmp      r1, #0
003d930c: mov      r4, r0
003d9310: add      r2, r2, #8
003d9314: str      r2, [r0]
003d9318: beq      #0x3d9340
003d931c: add      r5, r0, #0x9c
003d9320: mov      r0, r5
003d9324: ldr      r1, [r4, #0xa0]
003d9328: bl       #0x3d91e8
003d932c: mov      r3, #0
003d9330: str      r5, [r4, #0xa8]
003d9334: str      r3, [r4, #0xac]
003d9338: str      r5, [r4, #0xa4]
003d933c: str      r3, [r4, #0xa0]
003d9340: mov      r0, r4
003d9344: bl       #0x37bec0
003d9348: mov      r0, r4
003d934c: pop      {r4, r5, r6, pc}

# _ZN10LuaManagerC2Ev
00379e68: ldr      r1, [pc, #0x38]
00379e6c: str      r4, [sp, #-4]!
00379e70: ldr      r4, [pc, #0x34]
00379e74: add      r1, pc, r1
00379e78: mov      ip, #0
00379e7c: ldr      r4, [r1, r4]
00379e80: mov      r2, r0
00379e84: str      ip, [r0, #8]
00379e88: add      r4, r4, #8
00379e8c: str      r4, [r0]
00379e90: strb     ip, [r2, #4]!
00379e94: str      r2, [r0, #0x10]
00379e98: str      ip, [r0, #0x14]
00379e9c: str      r2, [r0, #0xc]
00379ea0: ldm      sp!, {r4}
00379ea4: bx       lr
00379ea8: rsbeq    sl, r1, ip, lsl ip
00379eac: andeq    r3, r0, r8, ror #26

# _ZN9LuaScriptD2Ev
0037bec0: push     {r4, r5, r6, lr}
0037bec4: ldr      r5, [pc, #0x12c]
0037bec8: ldr      r3, [pc, #0x12c]
0037becc: ldr      r2, [r0, #0x90]
0037bed0: add      r5, pc, r5
0037bed4: ldr      r3, [r5, r3]
0037bed8: cmp      r2, #0
0037bedc: mov      r4, r0
0037bee0: add      r3, r3, #8
0037bee4: str      r3, [r0]
0037bee8: bne      #0x37bfc8
0037beec: add      r3, r4, #0x68
0037bef0: ldr      r0, [r3, #0x14]
0037bef4: cmp      r0, r3
0037bef8: beq      #0x37bf18
0037befc: cmp      r0, #0
0037bf00: beq      #0x37bf18
0037bf04: ldr      r1, [r4, #0x68]
0037bf08: rsb      r1, r0, r1
0037bf0c: cmp      r1, #0x80
0037bf10: bhi      #0x37bff0
0037bf14: bl       #0x708f00
0037bf18: ldr      r3, [r4, #0x5c]
0037bf1c: cmp      r3, #0
0037bf20: beq      #0x37bf48
0037bf24: add      r6, r4, #0x4c
0037bf28: mov      r0, r6
0037bf2c: ldr      r1, [r4, #0x50]
0037bf30: bl       #0x37bd7c
0037bf34: mov      r3, #0
0037bf38: str      r6, [r4, #0x58]
0037bf3c: str      r3, [r4, #0x5c]
0037bf40: str      r6, [r4, #0x54]
0037bf44: str      r3, [r4, #0x50]
0037bf48: ldr      r3, [r4, #0x44]
0037bf4c: cmp      r3, #0
0037bf50: beq      #0x37bf78
0037bf54: add      r6, r4, #0x34
0037bf58: mov      r0, r6
0037bf5c: ldr      r1, [r4, #0x38]
0037bf60: bl       #0x37bd7c
0037bf64: mov      r3, #0
0037bf68: str      r6, [r4, #0x40]
0037bf6c: str      r3, [r4, #0x44]
0037bf70: str      r6, [r4, #0x3c]
0037bf74: str      r3, [r4, #0x38]
0037bf78: ldr      r3, [r4, #0x2c]
0037bf7c: cmp      r3, #0
0037bf80: beq      #0x37bfa8
0037bf84: add      r6, r4, #0x1c
0037bf88: mov      r0, r6
0037bf8c: ldr      r1, [r4, #0x20]
0037bf90: bl       #0x37bcc0
0037bf94: mov      r3, #0
0037bf98: str      r6, [r4, #0x28]
0037bf9c: str      r3, [r4, #0x2c]
0037bfa0: str      r6, [r4, #0x24]
0037bfa4: str      r3, [r4, #0x20]
0037bfa8: ldr      r3, [pc, #0x50]
0037bfac: add      r0, r4, #4
0037bfb0: ldr      r3, [r5, r3]
0037bfb4: add      r3, r3, #8
0037bfb8: str      r3, [r4, #0x10]
0037bfbc: bl       #0x31b180
0037bfc0: mov      r0, r4
0037bfc4: pop      {r4, r5, r6, pc}
0037bfc8: add      r6, r0, #0x80
0037bfcc: mov      r0, r6
0037bfd0: ldr      r1, [r4, #0x84]
0037bfd4: bl       #0x37bcf8
0037bfd8: mov      r3, #0
0037bfdc: str      r6, [r4, #0x8c]
0037bfe0: str      r3, [r4, #0x90]
0037bfe4: str      r6, [r4, #0x88]
0037bfe8: str      r3, [r4, #0x84]
0037bfec: b        #0x37beec
0037bff0: bl       #0x310440
0037bff4: b        #0x37bf18
0037bff8: rsbeq    r8, r1, r0, asr #23
0037bffc: andeq    r1, r0, r4, ror r6
0037c000: andeq    r3, r0, r8, asr r6

# _ZN6CharAI14StepLoadCommonEv
003cc218: ldrb     r3, [r0, #0x2c]
003cc21c: cmp      r3, #0
003cc220: bxeq     lr
003cc224: ldr      r1, [pc, #8]
003cc228: ldr      r0, [r0, #0x20]
003cc22c: add      r1, pc, r1
003cc230: b        #0x37b574
003cc234: subeq    r8, pc, ip, ror #31

# _ZN10LuaManager7AddFileEP9LuaScriptPKc
0037b23c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037b240: ldr      r4, [pc, #0x2e0]
0037b244: ldr      r6, [pc, #0x2e0]
0037b248: sub      sp, sp, #0x7c
0037b24c: add      r4, pc, r4
0037b250: ldr      r3, [r4, r6]
0037b254: subs     r7, r1, #0
0037b258: mov      sl, r0
0037b25c: ldr      r3, [r3]
0037b260: mov      r5, r2
0037b264: str      r3, [sp, #0x74]
0037b268: beq      #0x37b318
0037b26c: cmp      r5, #0
0037b270: beq      #0x37b280
0037b274: ldrsb    r3, [r5]
0037b278: cmp      r3, #0
0037b27c: bne      #0x37b2a4
0037b280: mov      r5, #0
0037b284: ldr      r3, [r4, r6]
0037b288: ldr      r2, [sp, #0x74]
0037b28c: mov      r0, r5
0037b290: ldr      r3, [r3]
0037b294: cmp      r2, r3
0037b298: bne      #0x37b524
0037b29c: add      sp, sp, #0x7c
0037b2a0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037b2a4: add      r8, sp, #0x5c
0037b2a8: mov      r0, r8
0037b2ac: add      r1, r7, #0x68
0037b2b0: mov      r2, r5
0037b2b4: bl       #0x3338cc
0037b2b8: ldr      r1, [pc, #0x270]
0037b2bc: mov      r0, r5
0037b2c0: add      r1, pc, r1
0037b2c4: bl       #0x30ebd4
0037b2c8: cmp      r0, #0
0037b2cc: beq      #0x37b3f8
0037b2d0: ldr      r1, [pc, #0x25c]
0037b2d4: mov      r2, #5
0037b2d8: add      r1, pc, r1
0037b2dc: bl       #0x30ec7c
0037b2e0: cmp      r0, #0
0037b2e4: bne      #0x37b36c
0037b2e8: ldr      r3, [sp, #0x70]
0037b2ec: add      r1, sp, #0x78
0037b2f0: add      r5, r7, #0x80
0037b2f4: str      r3, [r1, #-0x5c]!
0037b2f8: mov      r0, r5
0037b2fc: bl       #0x37a190
0037b300: cmp      r5, r0
0037b304: beq      #0x37b380
0037b308: mov      r5, #1
0037b30c: mov      r0, r8
0037b310: bl       #0x3139ac
0037b314: b        #0x37b284
0037b318: ldr      r3, [pc, #0x218]
0037b31c: ldr      r3, [r4, r3]
0037b320: ldr      r3, [r3]
0037b324: cmp      r3, #2
0037b328: streq    r7, [r7]
0037b32c: beq      #0x37b26c
0037b330: cmp      r3, #1
0037b334: bne      #0x37b26c
0037b338: ldr      r0, [pc, #0x1fc]
0037b33c: ldr      r1, [pc, #0x1fc]
0037b340: ldr      r2, [pc, #0x1fc]
0037b344: ldr      r0, [r4, r0]
0037b348: ldr      r3, [pc, #0x1f8]
0037b34c: mov      ip, #0x23
0037b350: add      r1, pc, r1
0037b354: add      r2, pc, r2
0037b358: add      r3, pc, r3
0037b35c: add      r0, r0, #0xa8
0037b360: str      ip, [sp]
0037b364: bl       #0x30e004
0037b368: b        #0x37b26c
0037b36c: ldr      r1, [pc, #0x1d8]
0037b370: mov      r0, r8
0037b374: add      r1, pc, r1
0037b378: bl       #0x379ef8
0037b37c: b        #0x37b2e8
0037b380: ldr      r3, [sp, #0x70]
0037b384: add      r1, sp, #0x78
0037b388: add      sl, sl, #4
0037b38c: str      r3, [r1, #-0x60]!
0037b390: mov      r0, sl
0037b394: bl       #0x37a300
0037b398: cmp      r0, sl
0037b39c: mov      sb, r0
0037b3a0: beq      #0x37b444
0037b3a4: ldr      sl, [r0, #0x28]
0037b3a8: mov      r2, #0
0037b3ac: mov      r3, #0
0037b3b0: ldr      r1, [sl]
0037b3b4: mov      r0, sl
0037b3b8: mov      lr, pc
0037b3bc: ldr      pc, [r1, #0x20]
0037b3c0: cmp      sl, #0
0037b3c4: beq      #0x37b4d0
0037b3c8: add      sb, sp, #0x24
0037b3cc: add      r1, r7, #4
0037b3d0: mov      r2, sl
0037b3d4: mov      r0, sb
0037b3d8: bl       #0x31acf4
0037b3dc: ldr      r3, [sp, #0x28]
0037b3e0: cmp      r3, #0
0037b3e4: beq      #0x37b40c
0037b3e8: mov      r0, sb
0037b3ec: bl       #0x31a68c
0037b3f0: mov      r5, #0
0037b3f4: b        #0x37b30c
0037b3f8: ldr      r1, [pc, #0x150]
0037b3fc: mov      r0, r8
0037b400: add      r1, pc, r1
0037b404: bl       #0x379ef8
0037b408: b        #0x37b2e8
0037b40c: add      r7, sp, #0x44
0037b410: mov      r0, sb
0037b414: bl       #0x31a68c
0037b418: ldr      r1, [sp, #0x70]
0037b41c: add      r2, sp, #0x20
0037b420: mov      r0, r7
0037b424: bl       #0x3140ec
0037b428: add      r0, sp, #8
0037b42c: mov      r1, r5
0037b430: mov      r2, r7
0037b434: bl       #0x37a9e8
0037b438: mov      r0, r7
0037b43c: bl       #0x3139ac
0037b440: b        #0x37b308
0037b444: ldr      r3, [pc, #0x108]
0037b448: mov      r2, #0
0037b44c: ldr      r1, [sp, #0x70]
0037b450: ldr      fp, [r4, r3]
0037b454: mov      r3, r2
0037b458: ldr      r0, [fp, #0x10]
0037b45c: ldr      ip, [r0, #0x34]
0037b460: mov      r0, ip
0037b464: ldr      ip, [ip]
0037b468: mov      lr, pc
0037b46c: ldr      pc, [ip, #0x88]
0037b470: cmp      r0, #0
0037b474: str      r0, [sp, #0x14]
0037b478: moveq    r5, r0
0037b47c: beq      #0x37b30c
0037b480: mov      r1, #0
0037b484: mov      r0, #0x30
0037b488: bl       #0x310570
0037b48c: ldr      r1, [sp, #0x14]
0037b490: mov      sl, r0
0037b494: bl       #0x3172d8
0037b498: ldr      r3, [sp, #0x70]
0037b49c: add      r1, sp, #0x78
0037b4a0: mov      r0, sb
0037b4a4: str      r3, [r1, #-0x68]!
0037b4a8: bl       #0x37b0fc
0037b4ac: str      sl, [r0]
0037b4b0: ldr      r3, [fp, #0x10]
0037b4b4: add      r1, sp, #0x14
0037b4b8: ldr      r3, [r3, #0x34]
0037b4bc: mov      r0, r3
0037b4c0: ldr      r3, [r3]
0037b4c4: mov      lr, pc
0037b4c8: ldr      pc, [r3, #0x78]
0037b4cc: b        #0x37b3c0
0037b4d0: ldr      r3, [pc, #0x60]
0037b4d4: ldr      r3, [r4, r3]
0037b4d8: ldr      r3, [r3]
0037b4dc: cmp      r3, #2
0037b4e0: streq    sl, [sl]
0037b4e4: beq      #0x37b3c8
0037b4e8: cmp      r3, #1
0037b4ec: bne      #0x37b3c8
0037b4f0: ldr      r0, [pc, #0x44]
0037b4f4: ldr      r1, [pc, #0x5c]
0037b4f8: ldr      r2, [pc, #0x5c]
0037b4fc: ldr      r0, [r4, r0]
0037b500: ldr      r3, [pc, #0x58]
0037b504: mov      ip, #0x61
0037b508: add      r1, pc, r1
0037b50c: add      r2, pc, r2
0037b510: add      r3, pc, r3
0037b514: add      r0, r0, #0xa8
0037b518: str      ip, [sp]
0037b51c: bl       #0x30e004
0037b520: b        #0x37b3c8
0037b524: bl       #0x30e310
0037b528: rsbeq    sb, r1, r4, asr #16
0037b52c: andeq    r4, r0, ip, lsr #1
0037b530: subseq   r6, r4, r0, ror r7
0037b534: subseq   r6, r4, r0, ror #14
0037b538: andeq    r3, r0, r0, asr #19
0037b53c: andeq    r1, r0, r0, asr #19
0037b540: subseq   r3, r4, r8, lsl #1
0037b544: subseq   r6, r4, r4, lsl #13
0037b548: subseq   r6, r4, r8, lsl #13
0037b54c: subseq   r6, r7, r4, lsl sp
0037b550: subseq   r6, r4, r8, lsr r6
0037b554: strdeq   r3, r4, [r0], -r4
0037b558: ldrsbeq  r2, [r4], #-0xe0
0037b55c: subseq   r6, r4, r4, lsr r5
0037b560: ldrsbeq  r6, [r4], #-0x40

# _ZN6CharAI14StepInitScriptEv
003cb314: push     {r4, lr}
003cb318: mov      r4, r0
003cb31c: ldr      r3, [r0]
003cb320: mov      lr, pc
003cb324: ldr      pc, [r3, #8]
003cb328: ldr      r3, [r4, #0x30]
003cb32c: cmp      r3, #0
003cb330: beq      #0x3cb348
003cb334: ldr      r3, [r4, #0x20]
003cb338: mov      r0, r3
003cb33c: ldr      r3, [r3]
003cb340: mov      lr, pc
003cb344: ldr      pc, [r3, #0xcc]
003cb348: pop      {r4, pc}

# _ZN3sfc6script3lua8InstanceC2Ev
0031b2a8: ldr      r3, [pc, #0x30]
0031b2ac: ldr      r2, [pc, #0x30]
0031b2b0: mov      r1, #1
0031b2b4: add      r3, pc, r3
0031b2b8: ldr      r2, [r3, r2]
0031b2bc: push     {r4, lr}
0031b2c0: add      r2, r2, #8
0031b2c4: strb     r1, [r0, #8]
0031b2c8: str      r2, [r0]
0031b2cc: mov      r4, r0
0031b2d0: bl       #0x31b224
0031b2d4: str      r0, [r4, #4]
0031b2d8: mov      r0, r4
0031b2dc: pop      {r4, pc}

# _ZN3sfc6script3lua8InstanceD2Ev
0031b1e0: push     {r4, lr}
0031b1e4: ldr      r3, [pc, #0x30]
0031b1e8: ldr      r2, [pc, #0x30]
0031b1ec: ldrb     r1, [r0, #8]
0031b1f0: add      r3, pc, r3
0031b1f4: ldr      r2, [r3, r2]
0031b1f8: cmp      r1, #0
0031b1fc: mov      r4, r0
0031b200: add      r2, r2, #8
0031b204: str      r2, [r0]
0031b208: beq      #0x31b214
0031b20c: ldr      r0, [r0, #4]
0031b210: bl       #0x85797c
0031b214: mov      r0, r4
0031b218: pop      {r4, pc}
0031b21c: rsbeq    sb, r7, r0, lsr #17
0031b220: andeq    r4, r0, r8, lsl r0

# _ZN9LuaScriptD1Ev
0037c004: push     {r4, r5, r6, lr}
0037c008: ldr      r5, [pc, #0x12c]
0037c00c: ldr      r3, [pc, #0x12c]
0037c010: ldr      r2, [r0, #0x90]
0037c014: add      r5, pc, r5
0037c018: ldr      r3, [r5, r3]
0037c01c: cmp      r2, #0
0037c020: mov      r4, r0
0037c024: add      r3, r3, #8
0037c028: str      r3, [r0]
0037c02c: bne      #0x37c10c
0037c030: add      r3, r4, #0x68
0037c034: ldr      r0, [r3, #0x14]
0037c038: cmp      r0, r3
0037c03c: beq      #0x37c05c
0037c040: cmp      r0, #0
0037c044: beq      #0x37c05c
0037c048: ldr      r1, [r4, #0x68]
0037c04c: rsb      r1, r0, r1
0037c050: cmp      r1, #0x80
0037c054: bhi      #0x37c134
0037c058: bl       #0x708f00
0037c05c: ldr      r3, [r4, #0x5c]
0037c060: cmp      r3, #0
0037c064: beq      #0x37c08c
0037c068: add      r6, r4, #0x4c
0037c06c: mov      r0, r6
0037c070: ldr      r1, [r4, #0x50]
0037c074: bl       #0x37bd7c
0037c078: mov      r3, #0
0037c07c: str      r6, [r4, #0x58]
0037c080: str      r3, [r4, #0x5c]
0037c084: str      r6, [r4, #0x54]
0037c088: str      r3, [r4, #0x50]
0037c08c: ldr      r3, [r4, #0x44]
0037c090: cmp      r3, #0
0037c094: beq      #0x37c0bc
0037c098: add      r6, r4, #0x34
0037c09c: mov      r0, r6
0037c0a0: ldr      r1, [r4, #0x38]
0037c0a4: bl       #0x37bd7c
0037c0a8: mov      r3, #0
0037c0ac: str      r6, [r4, #0x40]
0037c0b0: str      r3, [r4, #0x44]
0037c0b4: str      r6, [r4, #0x3c]
0037c0b8: str      r3, [r4, #0x38]
0037c0bc: ldr      r3, [r4, #0x2c]
0037c0c0: cmp      r3, #0
0037c0c4: beq      #0x37c0ec
0037c0c8: add      r6, r4, #0x1c
0037c0cc: mov      r0, r6
0037c0d0: ldr      r1, [r4, #0x20]
0037c0d4: bl       #0x37bcc0
0037c0d8: mov      r3, #0
0037c0dc: str      r6, [r4, #0x28]
0037c0e0: str      r3, [r4, #0x2c]
0037c0e4: str      r6, [r4, #0x24]
0037c0e8: str      r3, [r4, #0x20]
0037c0ec: ldr      r3, [pc, #0x50]
0037c0f0: add      r0, r4, #4
0037c0f4: ldr      r3, [r5, r3]
0037c0f8: add      r3, r3, #8
0037c0fc: str      r3, [r4, #0x10]
0037c100: bl       #0x31b180
0037c104: mov      r0, r4
0037c108: pop      {r4, r5, r6, pc}
0037c10c: add      r6, r0, #0x80
0037c110: mov      r0, r6
0037c114: ldr      r1, [r4, #0x84]
0037c118: bl       #0x37bcf8
0037c11c: mov      r3, #0
0037c120: str      r6, [r4, #0x8c]
0037c124: str      r3, [r4, #0x90]
0037c128: str      r6, [r4, #0x88]
0037c12c: str      r3, [r4, #0x84]
0037c130: b        #0x37c030
0037c134: bl       #0x310440
0037c138: b        #0x37c05c
0037c13c: rsbeq    r8, r1, ip, ror sl
0037c140: andeq    r1, r0, r4, ror r6
0037c144: andeq    r3, r0, r8, asr r6

# _ZN9LuaScript4LoadEPKc
0037b574: ldr      r3, [pc, #0x1c]
0037b578: ldr      r2, [pc, #0x1c]
0037b57c: mov      ip, r0
0037b580: add      r3, pc, r3
0037b584: ldr      r0, [r3, r2]
0037b588: mov      r2, r1
0037b58c: mov      r1, ip
0037b590: ldr      r0, [r0, #0x3c]
0037b594: b        #0x37b23c
0037b598: rsbeq    sb, r1, r0, lsl r5
0037b59c: strdeq   r3, r4, [r0], -r4

# _ZN9LuaScriptC2Eb
0037c674: push     {r4, r5, r6, r7, r8, lr}
0037c678: ldr      r6, [pc, #0xd8]
0037c67c: ldr      r3, [pc, #0xd8]
0037c680: mov      r7, r0
0037c684: add      r6, pc, r6
0037c688: ldr      r3, [r6, r3]
0037c68c: mov      r4, r0
0037c690: mov      r8, r1
0037c694: add      r3, r3, #8
0037c698: str      r3, [r7], #4
0037c69c: mov      r0, r7
0037c6a0: bl       #0x31b268
0037c6a4: ldr      r2, [pc, #0xb4]
0037c6a8: mov      r5, #0
0037c6ac: mov      r3, r4
0037c6b0: ldr      r2, [r6, r2]
0037c6b4: str      r7, [r4, #0x14]
0037c6b8: str      r5, [r4, #0x18]
0037c6bc: add      r2, r2, #8
0037c6c0: str      r2, [r4, #0x10]
0037c6c4: str      r5, [r4, #0x20]
0037c6c8: mov      r2, r4
0037c6cc: strb     r5, [r3, #0x1c]!
0037c6d0: str      r3, [r4, #0x28]
0037c6d4: str      r3, [r4, #0x24]
0037c6d8: str      r5, [r4, #0x2c]
0037c6dc: mov      r3, r4
0037c6e0: str      r5, [r4, #0x38]
0037c6e4: strb     r5, [r2, #0x34]!
0037c6e8: str      r2, [r4, #0x40]
0037c6ec: str      r2, [r4, #0x3c]
0037c6f0: add      r0, r4, #0x68
0037c6f4: str      r5, [r4, #0x44]
0037c6f8: str      r5, [r4, #0x50]
0037c6fc: strb     r5, [r3, #0x4c]!
0037c700: str      r3, [r4, #0x58]
0037c704: str      r3, [r4, #0x54]
0037c708: str      r5, [r4, #0x5c]
0037c70c: strb     r5, [r4, #0x64]
0037c710: str      r0, [r4, #0x78]
0037c714: str      r0, [r4, #0x7c]
0037c718: mov      r1, #0x10
0037c71c: bl       #0x31167c
0037c720: ldr      r2, [r4, #0x78]
0037c724: mov      r3, r4
0037c728: cmp      r8, r5
0037c72c: strb     r5, [r2]
0037c730: str      r5, [r4, #0x84]
0037c734: strb     r5, [r3, #0x80]!
0037c738: str      r3, [r4, #0x8c]
0037c73c: str      r5, [r4, #0x90]
0037c740: str      r3, [r4, #0x88]
0037c744: bne      #0x37c750
0037c748: mov      r0, r4
0037c74c: bl       #0x37b5a0
0037c750: mov      r0, r4
0037c754: pop      {r4, r5, r6, r7, r8, pc}
0037c758: rsbeq    r8, r1, ip, lsl #8
0037c75c: andeq    r1, r0, r4, ror r6
0037c760: andeq    r3, r0, r8, asr r6

# _ZN6CharAI16StepBindFunctionEv
003cc278: ldr      r0, [r0, #0x20]
003cc27c: b        #0x3d8f2c

# _ZN15AISPlayerIPhoneD0Ev
003de294: ldr      r3, [pc, #0x2c]
003de298: ldr      r2, [pc, #0x2c]
003de29c: push     {r4, lr}
003de2a0: add      r3, pc, r3
003de2a4: ldr      r2, [r3, r2]
003de2a8: mov      r4, r0
003de2ac: add      r2, r2, #8
003de2b0: str      r2, [r0]
003de2b4: bl       #0x3de100
003de2b8: mov      r0, r4
003de2bc: bl       #0x310440
003de2c0: mov      r0, r4
003de2c4: pop      {r4, pc}
003de2c8: ldrsheq  r6, [fp], #-0x70
003de2cc: andeq    r1, r0, ip, asr #15

# _ZN15AISPlayerIPhoneD1Ev
003de174: ldr      r3, [pc, #0x24]
003de178: ldr      r2, [pc, #0x24]
003de17c: push     {r4, lr}
003de180: add      r3, pc, r3
003de184: ldr      r2, [r3, r2]
003de188: mov      r4, r0
003de18c: add      r2, r2, #8
003de190: str      r2, [r0]
003de194: bl       #0x3de100
003de198: mov      r0, r4
003de19c: pop      {r4, pc}
003de1a0: subseq   r6, fp, r0, lsl sb
003de1a4: andeq    r1, r0, ip, asr #15
