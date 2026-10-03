
# _ZN6glitch7collada14CEventsManager16dispatchEventsExItLi30EEEviii
0060ea08: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0060ea0c: cmp      r1, r2
0060ea10: sub      sp, sp, #0x14
0060ea14: str      r2, [sp, #4]
0060ea18: mov      r4, r0
0060ea1c: mov      fp, r3
0060ea20: bgt      #0x60eadc
0060ea24: str      r1, [sp]
0060ea28: ldr      r5, [r0, #0x14]
0060ea2c: lsl      r7, r1, #3
0060ea30: lsl      sl, r1, #1
0060ea34: add      sb, sp, #8
0060ea38: ldr      r3, [r5, #0x14]
0060ea3c: ldr      r3, [r3, r7]
0060ea40: cmp      r3, #0
0060ea44: movgt    r6, #0
0060ea48: ble      #0x60eac0
0060ea4c: mov      r0, fp
0060ea50: bl       #0x30e964
0060ea54: ldr      r3, [r5, #0xc]
0060ea58: mov      r8, r0
0060ea5c: ldrh     r0, [r3, sl]
0060ea60: bl       #0x30e964
0060ea64: movw     r1, #0x5555
0060ea68: movt     r1, #0xc205
0060ea6c: bl       #0x30ed6c
0060ea70: mov      r1, r0
0060ea74: mov      r0, r8
0060ea78: bl       #0x30eba4
0060ea7c: bl       #0x30e4cc
0060ea80: str      r0, [sp, #8]
0060ea84: ldr      r3, [r5, #0x14]
0060ea88: ldr      r1, [r4, #0xc]
0060ea8c: mov      r0, sb
0060ea90: add      r3, r3, r7
0060ea94: ldr      r3, [r3, #4]
0060ea98: ldr      r3, [r3, r6, lsl #2]
0060ea9c: add      r6, r6, #1
0060eaa0: str      r3, [sp, #0xc]
0060eaa4: mov      lr, pc
0060eaa8: ldr      pc, [r4, #8]
0060eaac: ldr      r5, [r4, #0x14]
0060eab0: ldr      r3, [r5, #0x14]
0060eab4: ldr      r3, [r3, r7]
0060eab8: cmp      r6, r3
0060eabc: blt      #0x60ea4c
0060eac0: ldm      sp, {r2, r3}
0060eac4: add      r7, r7, #8
0060eac8: add      r2, r2, #1
0060eacc: cmp      r3, r2
0060ead0: str      r2, [sp]
0060ead4: add      sl, sl, #2
0060ead8: bge      #0x60ea38
0060eadc: add      sp, sp, #0x14
0060eae0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada14CEventsManager9findEntryEi
0060e02c: push     {r4, r5, r6, r7, r8, lr}
0060e030: ldr      r5, [r0, #0x14]
0060e034: ldr      r3, [r5]
0060e038: cmp      r3, #3
0060e03c: beq      #0x60e14c
0060e040: cmp      r3, #4
0060e044: beq      #0x60e0dc
0060e048: cmp      r3, #1
0060e04c: beq      #0x60e058
0060e050: mov      r0, #0
0060e054: pop      {r4, r5, r6, r7, r8, pc}
0060e058: ldr      r6, [r5, #8]
0060e05c: cmp      r6, #0
0060e060: ble      #0x60e0cc
0060e064: mov      r0, r1
0060e068: bl       #0x30e964
0060e06c: movw     r1, #0x5555
0060e070: movt     r1, #0x4205
0060e074: bl       #0x30ec94
0060e078: ldr      r5, [r5, #0xc]
0060e07c: mov      r7, r0
0060e080: ldrb     r0, [r5]
0060e084: bl       #0x30e964
0060e088: mov      r1, r0
0060e08c: mov      r0, r7
0060e090: bl       #0x30e70c
0060e094: cmp      r0, #0
0060e098: moveq    r4, #0
0060e09c: beq      #0x60e0c0
0060e0a0: b        #0x60e1cc
0060e0a4: ldrb     r0, [r5, r4]
0060e0a8: bl       #0x30e964
0060e0ac: mov      r1, r0
0060e0b0: mov      r0, r7
0060e0b4: bl       #0x30e70c
0060e0b8: cmp      r0, #0
0060e0bc: bne      #0x60e0d4
0060e0c0: add      r4, r4, #1
0060e0c4: cmp      r4, r6
0060e0c8: bne      #0x60e0a4
0060e0cc: sub      r0, r6, #1
0060e0d0: pop      {r4, r5, r6, r7, r8, pc}
0060e0d4: sub      r0, r4, #1
0060e0d8: pop      {r4, r5, r6, r7, r8, pc}
0060e0dc: ldr      r6, [r5, #8]
0060e0e0: cmp      r6, #0
0060e0e4: ble      #0x60e144
0060e0e8: mov      r0, r1
0060e0ec: bl       #0x30e964
0060e0f0: ldr      r5, [r5, #0xc]
0060e0f4: mov      r7, r0
0060e0f8: ldr      r0, [r5]
0060e0fc: bl       #0x30e964
0060e100: mov      r1, r0
0060e104: mov      r0, r7
0060e108: bl       #0x30e70c
0060e10c: cmp      r0, #0
0060e110: moveq    r4, #0
0060e114: beq      #0x60e138
0060e118: b        #0x60e1cc
0060e11c: ldr      r0, [r5, r4, lsl #2]
0060e120: bl       #0x30e964
0060e124: mov      r1, r0
0060e128: mov      r0, r7
0060e12c: bl       #0x30e70c
0060e130: cmp      r0, #0
0060e134: bne      #0x60e0d4
0060e138: add      r4, r4, #1
0060e13c: cmp      r4, r6
0060e140: bne      #0x60e11c
0060e144: sub      r0, r6, #1
0060e148: pop      {r4, r5, r6, r7, r8, pc}
0060e14c: ldr      r6, [r5, #8]
0060e150: cmp      r6, #0
0060e154: ble      #0x60e144
0060e158: mov      r0, r1
0060e15c: bl       #0x30e964
0060e160: movw     r1, #0x5555
0060e164: movt     r1, #0x4205
0060e168: bl       #0x30ec94
0060e16c: ldr      r5, [r5, #0xc]
0060e170: mov      r7, r0
0060e174: ldrh     r0, [r5]
0060e178: bl       #0x30e964
0060e17c: mov      r1, r0
0060e180: mov      r0, r7
0060e184: bl       #0x30e70c
0060e188: cmp      r0, #0
0060e18c: moveq    r4, #0
0060e190: beq      #0x60e1b4
0060e194: b        #0x60e1cc
0060e198: ldrh     r0, [r5, r3]
0060e19c: bl       #0x30e964
0060e1a0: mov      r1, r0
0060e1a4: mov      r0, r7
0060e1a8: bl       #0x30e70c
0060e1ac: cmp      r0, #0
0060e1b0: bne      #0x60e0d4
0060e1b4: add      r4, r4, #1
0060e1b8: cmp      r4, r6
0060e1bc: lsl      r3, r4, #1
0060e1c0: bne      #0x60e198
0060e1c4: sub      r0, r6, #1
0060e1c8: pop      {r4, r5, r6, r7, r8, pc}
0060e1cc: mvn      r0, #0
0060e1d0: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada14CEventsManager8onUpdateEii
0060ecb4: push     {r4, r5, r6, lr}
0060ecb8: ldr      r3, [r0, #8]
0060ecbc: mov      r4, r0
0060ecc0: mov      r5, r2
0060ecc4: cmp      r3, #0
0060ecc8: beq      #0x60ed0c
0060eccc: ldr      r3, [r0, #4]
0060ecd0: add      r3, r3, #1
0060ecd4: str      r3, [r0, #4]
0060ecd8: bl       #0x60e02c
0060ecdc: mov      r1, r5
0060ece0: mov      r6, r0
0060ece4: mov      r0, r4
0060ece8: bl       #0x60e02c
0060ecec: add      r1, r6, #1
0060ecf0: mov      r2, r0
0060ecf4: mov      r3, r5
0060ecf8: mov      r0, r4
0060ecfc: bl       #0x60ebb4
0060ed00: mov      r0, r4
0060ed04: pop      {r4, r5, r6, lr}
0060ed08: b        #0x31d584
0060ed0c: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada14CEventsManager16dispatchEventsExIhLi30EEEviii
0060eae4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0060eae8: cmp      r1, r2
0060eaec: sub      sp, sp, #0x14
0060eaf0: str      r2, [sp, #4]
0060eaf4: mov      r4, r0
0060eaf8: mov      fp, r3
0060eafc: bgt      #0x60ebac
0060eb00: ldr      r5, [r0, #0x14]
0060eb04: mov      sl, r1
0060eb08: lsl      r7, r1, #3
0060eb0c: add      sb, sp, #8
0060eb10: ldr      r3, [r5, #0x14]
0060eb14: ldr      r3, [r3, r7]
0060eb18: cmp      r3, #0
0060eb1c: movgt    r6, #0
0060eb20: ble      #0x60eb98
0060eb24: mov      r0, fp
0060eb28: bl       #0x30e964
0060eb2c: ldr      r3, [r5, #0xc]
0060eb30: mov      r8, r0
0060eb34: ldrb     r0, [r3, sl]
0060eb38: bl       #0x30e964
0060eb3c: movw     r1, #0x5555
0060eb40: movt     r1, #0xc205
0060eb44: bl       #0x30ed6c
0060eb48: mov      r1, r0
0060eb4c: mov      r0, r8
0060eb50: bl       #0x30eba4
0060eb54: bl       #0x30e4cc
0060eb58: str      r0, [sp, #8]
0060eb5c: ldr      r3, [r5, #0x14]
0060eb60: ldr      r1, [r4, #0xc]
0060eb64: mov      r0, sb
0060eb68: add      r3, r3, r7
0060eb6c: ldr      r3, [r3, #4]
0060eb70: ldr      r3, [r3, r6, lsl #2]
0060eb74: add      r6, r6, #1
0060eb78: str      r3, [sp, #0xc]
0060eb7c: mov      lr, pc
0060eb80: ldr      pc, [r4, #8]
0060eb84: ldr      r5, [r4, #0x14]
0060eb88: ldr      r3, [r5, #0x14]
0060eb8c: ldr      r3, [r3, r7]
0060eb90: cmp      r6, r3
0060eb94: blt      #0x60eb24
0060eb98: ldr      r2, [sp, #4]
0060eb9c: add      sl, sl, #1
0060eba0: add      r7, r7, #8
0060eba4: cmp      r2, sl
0060eba8: bge      #0x60eb10
0060ebac: add      sp, sp, #0x14
0060ebb0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada14CEventsManager14dispatchEventsEiii
0060ebb4: ldr      ip, [r0, #0x14]
0060ebb8: ldr      ip, [ip]
0060ebbc: cmp      ip, #3
0060ebc0: beq      #0x60ebdc
0060ebc4: cmp      ip, #4
0060ebc8: beq      #0x60ebd8
0060ebcc: cmp      ip, #1
0060ebd0: bxne     lr
0060ebd4: b        #0x60eae4
0060ebd8: b        #0x60e934
0060ebdc: b        #0x60ea08

# _ZN6glitch7collada14CEventsManager16dispatchEventsExIiLi1000EEEviii
0060e934: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0060e938: cmp      r1, r2
0060e93c: sub      sp, sp, #0x14
0060e940: str      r2, [sp, #4]
0060e944: mov      r4, r0
0060e948: mov      sb, r3
0060e94c: bgt      #0x60ea00
0060e950: ldr      r5, [r0, #0x14]
0060e954: mov      fp, r1
0060e958: lsl      r7, r1, #3
0060e95c: lsl      r8, r1, #2
0060e960: add      sl, sp, #8
0060e964: ldr      r3, [r5, #0x14]
0060e968: ldr      r3, [r3, r7]
0060e96c: cmp      r3, #0
0060e970: movgt    r6, #0
0060e974: ble      #0x60e9e8
0060e978: mov      r0, sb
0060e97c: bl       #0x30e964
0060e980: ldr      r2, [r5, #0xc]
0060e984: mov      r3, r0
0060e988: ldr      r0, [r2, r8]
0060e98c: str      r3, [sp]
0060e990: bl       #0x30e964
0060e994: ldr      r3, [sp]
0060e998: mov      r1, r0
0060e99c: mov      r0, r3
0060e9a0: bl       #0x30e3ac
0060e9a4: bl       #0x30e4cc
0060e9a8: str      r0, [sp, #8]
0060e9ac: ldr      r3, [r5, #0x14]
0060e9b0: ldr      r1, [r4, #0xc]
0060e9b4: mov      r0, sl
0060e9b8: add      r3, r3, r7
0060e9bc: ldr      r3, [r3, #4]
0060e9c0: ldr      r3, [r3, r6, lsl #2]
0060e9c4: add      r6, r6, #1
0060e9c8: str      r3, [sp, #0xc]
0060e9cc: mov      lr, pc
0060e9d0: ldr      pc, [r4, #8]
0060e9d4: ldr      r5, [r4, #0x14]
0060e9d8: ldr      r3, [r5, #0x14]
0060e9dc: ldr      r3, [r3, r7]
0060e9e0: cmp      r6, r3
0060e9e4: blt      #0x60e978
0060e9e8: ldr      r2, [sp, #4]
0060e9ec: add      fp, fp, #1
0060e9f0: add      r7, r7, #8
0060e9f4: cmp      r2, fp
0060e9f8: add      r8, r8, #4
0060e9fc: bge      #0x60e964
0060ea00: add      sp, sp, #0x14
0060ea04: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada14CEventsManager8onUpdateEiiii
0060ebe0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0060ebe4: cmp      r1, r2
0060ebe8: mov      r6, r1
0060ebec: mov      r5, r2
0060ebf0: mov      sl, r3
0060ebf4: mov      r4, r0
0060ebf8: ldr      sb, [sp, #0x20]
0060ebfc: beq      #0x60ec98
0060ec00: ldr      r3, [r0, #8]
0060ec04: cmp      r3, #0
0060ec08: beq      #0x60ec98
0060ec0c: sub      r1, r1, #1
0060ec10: bl       #0x60e02c
0060ec14: mov      r1, r5
0060ec18: add      r7, r0, #1
0060ec1c: mov      r0, r4
0060ec20: bl       #0x60e02c
0060ec24: ldr      r3, [r4, #0x10]
0060ec28: mov      r8, r0
0060ec2c: cmp      r3, r7
0060ec30: ldr      r3, [r4, #4]
0060ec34: addeq    r7, r7, #1
0060ec38: cmp      r6, r5
0060ec3c: add      r3, r3, #1
0060ec40: str      r3, [r4, #4]
0060ec44: ble      #0x60ec9c
0060ec48: mov      r1, sb
0060ec4c: mov      r0, r4
0060ec50: bl       #0x60e02c
0060ec54: rsb      r3, sl, sb
0060ec58: mov      r2, r0
0060ec5c: add      r3, r3, r5
0060ec60: mov      r1, r7
0060ec64: mov      r0, r4
0060ec68: bl       #0x60ebb4
0060ec6c: sub      r1, sl, #1
0060ec70: mov      r0, r4
0060ec74: bl       #0x60e02c
0060ec78: mov      r3, r5
0060ec7c: add      r1, r0, #1
0060ec80: mov      r2, r8
0060ec84: mov      r0, r4
0060ec88: bl       #0x60ebb4
0060ec8c: mov      r0, r4
0060ec90: bl       #0x31d584
0060ec94: str      r8, [r4, #0x10]
0060ec98: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0060ec9c: mov      r1, r7
0060eca0: mov      r3, r5
0060eca4: mov      r0, r4
0060eca8: mov      r2, r8
0060ecac: bl       #0x60ebb4
0060ecb0: b        #0x60ec8c

# _ZN6glitch7collada18ISceneNodeAnimator14setEventsTrackEPKNS0_12SEventsTrackE
0060fab8: push     {r4, r5, r6, lr}
0060fabc: mov      r5, r0
0060fac0: ldr      r0, [r0, #0x18]
0060fac4: ldr      r4, [pc, #0x7c]
0060fac8: mov      r6, r1
0060facc: cmp      r0, #0
0060fad0: add      r4, pc, r4
0060fad4: beq      #0x60fadc
0060fad8: bl       #0x31d584
0060fadc: cmp      r6, #0
0060fae0: beq      #0x60fb40
0060fae4: mov      r0, #0x18
0060fae8: mov      r1, #0
0060faec: bl       #0x5341ac
0060faf0: ldr      r3, [pc, #0x54]
0060faf4: ldr      r2, [pc, #0x54]
0060faf8: str      r6, [r0, #0x14]
0060fafc: ldr      r3, [r4, r3]
0060fb00: ldr      r2, [r4, r2]
0060fb04: add      r3, r3, #8
0060fb08: str      r2, [r0, #8]
0060fb0c: mov      r2, #0
0060fb10: str      r2, [r0, #0xc]
0060fb14: str      r3, [r0]
0060fb18: mov      r2, #1
0060fb1c: mvn      r3, #0
0060fb20: str      r2, [r0, #4]
0060fb24: str      r3, [r0, #0x10]
0060fb28: ldr      r2, [r5, #0x1c]
0060fb2c: ldr      r3, [r5, #0x20]
0060fb30: str      r0, [r5, #0x18]
0060fb34: str      r2, [r0, #8]
0060fb38: str      r3, [r0, #0xc]
0060fb3c: pop      {r4, r5, r6, pc}
0060fb40: str      r6, [r5, #0x18]
0060fb44: pop      {r4, r5, r6, pc}
0060fb48: eorseq   r4, r8, r0, asr #31
0060fb4c: ldrdeq   r4, r5, [r0], -r0
0060fb50: andeq    r4, r0, ip, lsr #10
