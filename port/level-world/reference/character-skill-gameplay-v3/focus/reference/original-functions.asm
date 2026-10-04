
# _ZN6CharAI13AI_BeginSpellEb
003d81c0: push     {r4, r5, r6, r7, r8, lr}
003d81c4: mov      r4, r0
003d81c8: mov      r6, r1
003d81cc: ldr      r0, [r0, #4]
003d81d0: mvn      r1, #0
003d81d4: bl       #0x3bb98c
003d81d8: mov      r1, r0
003d81dc: mov      r5, r0
003d81e0: ldr      r0, [r4, #4]
003d81e4: bl       #0x3aeac0
003d81e8: ldr      r7, [r0, #0x1c]
003d81ec: cmp      r7, #1
003d81f0: beq      #0x3d82a4
003d81f4: mov      r0, r4
003d81f8: bl       #0x3d80b4
003d81fc: cmp      r0, #0
003d8200: bne      #0x3d8208
003d8204: pop      {r4, r5, r6, r7, r8, pc}
003d8208: ldr      r7, [r4, #4]
003d820c: mov      r8, #0
003d8210: mvn      r1, #0
003d8214: strb     r8, [r4, #0xd0]
003d8218: strb     r8, [r4, #0xd1]
003d821c: mov      r0, r7
003d8220: bl       #0x3bb98c
003d8224: mov      r1, r0
003d8228: add      r0, r7, #0x4f0
003d822c: mov      r3, r8
003d8230: mov      r2, r8
003d8234: add      r0, r0, #0xc
003d8238: bl       #0x3c6394
003d823c: bl       #0x7fd794
003d8240: ldrb     r3, [r0, #5]
003d8244: cmp      r3, r8
003d8248: beq      #0x3d8290
003d824c: cmp      r6, r8
003d8250: bne      #0x3d8290
003d8254: bl       #0x80b1bc
003d8258: mov      r6, r0
003d825c: ldr      r0, [pc, #0xc0]
003d8260: ldr      r3, [r4, #4]
003d8264: mov      r1, #1
003d8268: add      r0, pc, r0
003d826c: ldrb     r7, [r3, #0x108]
003d8270: bl       #0x80a244
003d8274: mov      r3, #3
003d8278: mov      r1, r0
003d827c: strb     r7, [r0, #0x54]
003d8280: strb     r3, [r0, #0x50]
003d8284: strh     r5, [r0, #0x52]
003d8288: mov      r0, r6
003d828c: bl       #0x80e2a4
003d8290: ldr      r0, [r4, #4]
003d8294: add      r0, r0, #0x4f0
003d8298: add      r0, r0, #0xc
003d829c: pop      {r4, r5, r6, r7, r8, lr}
003d82a0: b        #0x3c0334
003d82a4: mov      r0, r4
003d82a8: bl       #0x3d7d9c
003d82ac: cmp      r0, #0
003d82b0: beq      #0x3d81f4
003d82b4: ldr      r3, [r4, #0xc0]
003d82b8: ldr      r0, [r3, r5, lsl #2]
003d82bc: bl       #0x3da8b8
003d82c0: bl       #0x7fd794
003d82c4: ldrb     r3, [r0, #5]
003d82c8: cmp      r3, #0
003d82cc: beq      #0x3d831c
003d82d0: cmp      r6, #0
003d82d4: bne      #0x3d831c
003d82d8: bl       #0x80b1bc
003d82dc: mov      r6, r0
003d82e0: ldr      r0, [pc, #0x40]
003d82e4: ldr      r3, [r4, #4]
003d82e8: mov      r1, r7
003d82ec: add      r0, pc, r0
003d82f0: ldrb     r4, [r3, #0x108]
003d82f4: bl       #0x80a244
003d82f8: mov      r3, #3
003d82fc: mov      r1, r0
003d8300: strb     r4, [r0, #0x54]
003d8304: strb     r3, [r0, #0x50]
003d8308: strh     r5, [r0, #0x52]
003d830c: mov      r0, r6
003d8310: bl       #0x80e2a4
003d8314: mov      r0, r7
003d8318: pop      {r4, r5, r6, r7, r8, pc}
003d831c: mov      r0, #1
003d8320: b        #0x3d8204
003d8324: subeq    r6, lr, r0, lsr #25
003d8328: subeq    r6, lr, ip, lsl ip

# _ZN6CharAI10_SpellBlurEv
003d8b28: push     {r4, lr}
003d8b2c: mvn      r1, #0
003d8b30: mov      r4, r0
003d8b34: ldr      r0, [r0, #4]
003d8b38: bl       #0x3bb98c
003d8b3c: ldr      r3, [r4, #0xc0]
003d8b40: ldr      r2, [r4, #0xc4]
003d8b44: rsb      r2, r3, r2
003d8b48: cmp      r0, r2, asr #2
003d8b4c: bhs      #0x3d8b78
003d8b50: ldr      r3, [r3, r0, lsl #2]
003d8b54: cmp      r3, #0
003d8b58: beq      #0x3d8b78
003d8b5c: ldr      r0, [r4, #4]
003d8b60: mvn      r1, #0
003d8b64: bl       #0x3bb98c
003d8b68: ldr      r3, [r4, #0xc0]
003d8b6c: ldr      r0, [r3, r0, lsl #2]
003d8b70: pop      {r4, lr}
003d8b74: b        #0x3da6c0
003d8b78: pop      {r4, pc}

# _ZN6CharAI11_SpellEventEv
003d8ba4: push     {r4, lr}
003d8ba8: mvn      r1, #0
003d8bac: mov      r4, r0
003d8bb0: ldr      r0, [r0, #4]
003d8bb4: bl       #0x3bb98c
003d8bb8: ldr      r3, [r4, #0xc0]
003d8bbc: ldr      r2, [r4, #0xc4]
003d8bc0: rsb      r2, r3, r2
003d8bc4: cmp      r0, r2, asr #2
003d8bc8: bhs      #0x3d8bf4
003d8bcc: ldr      r3, [r3, r0, lsl #2]
003d8bd0: cmp      r3, #0
003d8bd4: beq      #0x3d8bf4
003d8bd8: ldr      r0, [r4, #4]
003d8bdc: mvn      r1, #0
003d8be0: bl       #0x3bb98c
003d8be4: ldr      r3, [r4, #0xc0]
003d8be8: ldr      r0, [r3, r0, lsl #2]
003d8bec: pop      {r4, lr}
003d8bf0: b        #0x3da794
003d8bf4: pop      {r4, pc}

# _ZN6CharAI11_SpellFocusEv
003d8038: push     {r4, lr}
003d803c: mvn      r1, #0
003d8040: mov      r4, r0
003d8044: ldr      r0, [r0, #4]
003d8048: bl       #0x3bb98c
003d804c: ldr      r3, [r4, #0xc0]
003d8050: ldr      r2, [r4, #0xc4]
003d8054: rsb      r2, r3, r2
003d8058: cmp      r0, r2, asr #2
003d805c: bhs      #0x3d8088
003d8060: ldr      r3, [r3, r0, lsl #2]
003d8064: cmp      r3, #0
003d8068: beq      #0x3d8088
003d806c: ldr      r0, [r4, #4]
003d8070: mvn      r1, #0
003d8074: bl       #0x3bb98c
003d8078: ldr      r3, [r4, #0xc0]
003d807c: ldr      r0, [r3, r0, lsl #2]
003d8080: pop      {r4, lr}
003d8084: b        #0x3da8b8
003d8088: pop      {r4, pc}

# _ZN6CharAI10_SkillBlurEv
003d8b7c: ldr      r1, [r0, #0xb8]
003d8b80: ldr      r3, [r0, #0xb4]
003d8b84: ldr      r2, [r0, #0xcc]
003d8b88: rsb      r1, r3, r1
003d8b8c: cmp      r2, r1, asr #2
003d8b90: bxhs     lr
003d8b94: ldr      r0, [r3, r2, lsl #2]
003d8b98: cmp      r0, #0
003d8b9c: bxeq     lr
003d8ba0: b        #0x3da6c0

# _ZN6CharAI11_SkillFocusEv
003d808c: ldr      r1, [r0, #0xb8]
003d8090: ldr      r3, [r0, #0xb4]
003d8094: ldr      r2, [r0, #0xcc]
003d8098: rsb      r1, r3, r1
003d809c: cmp      r2, r1, asr #2
003d80a0: bxhs     lr
003d80a4: ldr      r0, [r3, r2, lsl #2]
003d80a8: cmp      r0, #0
003d80ac: bxeq     lr
003d80b0: b        #0x3da8b8

# _ZN6CharAI11_SkillEventEv
003d8bf8: ldr      r1, [r0, #0xb8]
003d8bfc: ldr      r3, [r0, #0xb4]
003d8c00: ldr      r2, [r0, #0xcc]
003d8c04: rsb      r1, r3, r1
003d8c08: cmp      r2, r1, asr #2
003d8c0c: bxhs     lr
003d8c10: ldr      r0, [r3, r2, lsl #2]
003d8c14: cmp      r0, #0
003d8c18: bxeq     lr
003d8c1c: b        #0x3da794
