
# _ZN17MenuFlash2DCamera12SetLimitClipEPN7gameswf9characterE
0042cf30: push     {r4, r5, lr}
0042cf34: cmp      r1, #0
0042cf38: mov      r4, r0
0042cf3c: sub      sp, sp, #0x14
0042cf40: str      r1, [r4, #8]
0042cf44: beq      #0x42cf60
0042cf48: mov      r0, sp
0042cf4c: bl       #0x416a7c
0042cf50: mov      r5, sp
0042cf54: add      ip, r4, #0xc
0042cf58: ldm      r5, {r0, r1, r2, r3}
0042cf5c: stm      ip, {r0, r1, r2, r3}
0042cf60: add      sp, sp, #0x14
0042cf64: pop      {r4, r5, pc}

# _ZN17MenuFlash2DCamera6UpdateEv
0042cd84: push     {r4, r5, r6, r7, r8, lr}
0042cd88: ldr      r2, [r0, #0x24]
0042cd8c: ldr      r1, [r0, #0x2c]
0042cd90: ldr      r3, [pc, #0x190]
0042cd94: sub      sp, sp, #8
0042cd98: cmp      r2, r1
0042cd9c: mov      r4, r0
0042cda0: add      r3, pc, r3
0042cda4: bge      #0x42cefc
0042cda8: movw     r0, #0x6667
0042cdac: rsb      r1, r2, r1
0042cdb0: movt     r0, #0x6666
0042cdb4: smull    ip, r0, r0, r1
0042cdb8: asr      r1, r1, #0x1f
0042cdbc: rsb      r1, r1, r0, asr #2
0042cdc0: add      r2, r2, #1
0042cdc4: add      r2, r2, r1
0042cdc8: str      r2, [r4, #0x24]
0042cdcc: ldr      r2, [r4, #0x28]
0042cdd0: ldr      r1, [r4, #0x30]
0042cdd4: cmp      r2, r1
0042cdd8: bge      #0x42ced0
0042cddc: movw     r0, #0x6667
0042cde0: rsb      r1, r2, r1
0042cde4: movt     r0, #0x6666
0042cde8: smull    ip, r0, r0, r1
0042cdec: asr      r1, r1, #0x1f
0042cdf0: rsb      r1, r1, r0, asr #2
0042cdf4: add      r2, r2, #1
0042cdf8: add      r2, r2, r1
0042cdfc: str      r2, [r4, #0x28]
0042ce00: ldr      r1, [pc, #0x124]
0042ce04: ldr      r2, [r4, #8]
0042ce08: ldr      r3, [r3, r1]
0042ce0c: cmp      r2, #0
0042ce10: ldr      r3, [r3, #0x10]
0042ce14: ldr      r3, [r3, #0x10]
0042ce18: ldr      r3, [r3, #0xcc]
0042ce1c: ldr      r3, [r3, #-4]
0042ce20: ldr      r5, [r3, #0x10]
0042ce24: ldr      r6, [r3, #0xc]
0042ce28: beq      #0x42ce94
0042ce2c: ldr      r0, [r4, #0xc]
0042ce30: bl       #0x30e4cc
0042ce34: ldr      r8, [r4, #0x24]
0042ce38: add      r3, r0, r8
0042ce3c: cmp      r3, #0
0042ce40: rsbgt    r8, r0, #0
0042ce44: strgt    r8, [r4, #0x24]
0042ce48: ldr      r0, [r4, #0x14]
0042ce4c: bl       #0x30e4cc
0042ce50: ldr      r7, [r4, #0x28]
0042ce54: add      r3, r0, r7
0042ce58: cmp      r3, #0
0042ce5c: rsbgt    r7, r0, #0
0042ce60: strgt    r7, [r4, #0x28]
0042ce64: ldr      r0, [r4, #0x10]
0042ce68: bl       #0x30e4cc
0042ce6c: add      r8, r0, r8
0042ce70: cmp      r6, r8
0042ce74: rsbgt    r0, r0, r6
0042ce78: strgt    r0, [r4, #0x24]
0042ce7c: ldr      r0, [r4, #0x18]
0042ce80: bl       #0x30e4cc
0042ce84: add      r7, r0, r7
0042ce88: cmp      r5, r7
0042ce8c: rsbgt    r0, r0, r5
0042ce90: strgt    r0, [r4, #0x28]
0042ce94: mov      r1, #0
0042ce98: ldr      r0, [r4, #4]
0042ce9c: mov      r2, r1
0042cea0: mov      r3, r6
0042cea4: str      r5, [sp]
0042cea8: bl       #0x7a9bac
0042ceac: ldr      r2, [r4, #0x28]
0042ceb0: ldr      r0, [r4, #4]
0042ceb4: ldr      r1, [r4, #0x24]
0042ceb8: mov      ip, #0
0042cebc: mov      r3, r6
0042cec0: stm      sp, {r5, ip}
0042cec4: bl       #0x7a9b30
0042cec8: add      sp, sp, #8
0042cecc: pop      {r4, r5, r6, r7, r8, pc}
0042ced0: ble      #0x42ce00
0042ced4: movw     r0, #0x6667
0042ced8: rsb      r1, r1, r2
0042cedc: movt     r0, #0x6666
0042cee0: smull    ip, r0, r0, r1
0042cee4: asr      r1, r1, #0x1f
0042cee8: rsb      r1, r1, r0, asr #2
0042ceec: mvn      r1, r1
0042cef0: add      r2, r1, r2
0042cef4: str      r2, [r4, #0x28]
0042cef8: b        #0x42ce00
0042cefc: ble      #0x42cdcc
0042cf00: movw     r0, #0x6667
0042cf04: rsb      r1, r1, r2
0042cf08: movt     r0, #0x6666
0042cf0c: smull    ip, r0, r0, r1
0042cf10: asr      r1, r1, #0x1f
0042cf14: rsb      r1, r1, r0, asr #2
0042cf18: mvn      r1, r1
0042cf1c: add      r2, r1, r2
0042cf20: str      r2, [r4, #0x24]
0042cf24: b        #0x42cdcc
0042cf28: ldrsheq  r7, [r6], #-0xc0
0042cf2c: strdeq   r3, r4, [r0], -r4

# _ZN17MenuFlash2DCameraC1EP6MenuFX
0042ccd0: ldr      r2, [pc, #0x68]
0042ccd4: ldr      ip, [pc, #0x68]
0042ccd8: ldr      r3, [pc, #0x68]
0042ccdc: add      r2, pc, r2
0042cce0: push     {r4, r5}
0042cce4: ldr      ip, [r2, ip]
0042cce8: ldr      r4, [r2, r3]
0042ccec: str      r1, [r0, #4]
0042ccf0: add      r5, ip, #8
0042ccf4: mov      ip, #0
0042ccf8: str      r5, [r0]
0042ccfc: str      ip, [r0, #8]
0042cd00: ldr      r1, [r4]
0042cd04: ldr      r4, [pc, #0x40]
0042cd08: add      r1, r1, r1, lsr #31
0042cd0c: ldr      r2, [r2, r4]
0042cd10: asr      r1, r1, #1
0042cd14: str      r1, [r0, #0x1c]
0042cd18: ldr      r2, [r2]
0042cd1c: str      ip, [r0, #0x30]
0042cd20: str      ip, [r0, #0x24]
0042cd24: add      r2, r2, r2, lsr #31
0042cd28: str      ip, [r0, #0x28]
0042cd2c: asr      r2, r2, #1
0042cd30: str      r2, [r0, #0x20]
0042cd34: str      ip, [r0, #0x2c]
0042cd38: pop      {r4, r5}
0042cd3c: bx       lr
0042cd40: ldrheq   r7, [r6], #-0xd4
0042cd44: andeq    r0, r0, r4, ror #28
0042cd48: andeq    r2, r0, r4, asr #11
0042cd4c: strdeq   r2, r3, [r0], -r8

# _ZN7gameswf4root17logical_to_screenERNS_5pointE
00773f50: ldr      r3, [pc, #0x1c8]
00773f54: ldr      r2, [pc, #0x1c8]
00773f58: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00773f5c: add      r3, pc, r3
00773f60: ldr      r2, [r3, r2]
00773f64: sub      sp, sp, #0xc
00773f68: mov      r4, r0
00773f6c: ldr      r3, [r2]
00773f70: mov      r5, r1
00773f74: mov      r0, r3
00773f78: ldr      r3, [r3]
00773f7c: mov      lr, pc
00773f80: ldr      pc, [r3, #0xac]
00773f84: cmp      r0, #0
00773f88: cmpne    r0, #2
00773f8c: movne    r8, #0
00773f90: moveq    r8, #1
00773f94: bne      #0x7740c4
00773f98: ldr      r6, [r4, #0xc]
00773f9c: ldr      r1, [r6, #0xb4]
00773fa0: ldr      r0, [r6, #0xb8]
00773fa4: bl       #0x30e3ac
00773fa8: ldr      r1, [r6, #0xbc]
00773fac: mov      fp, r0
00773fb0: ldr      r0, [r6, #0xc0]
00773fb4: bl       #0x30e3ac
00773fb8: mov      sl, r0
00773fbc: ldr      r0, [r4, #0x2c]
00773fc0: bl       #0x30e964
00773fc4: mov      r7, r0
00773fc8: ldr      r0, [r4, #0x1c]
00773fcc: bl       #0x30e964
00773fd0: mov      r1, r7
00773fd4: bl       #0x30ec94
00773fd8: str      r0, [sp]
00773fdc: ldr      r0, [r4, #0x30]
00773fe0: bl       #0x30e964
00773fe4: mov      r6, r0
00773fe8: ldr      r0, [r4, #0x20]
00773fec: bl       #0x30e964
00773ff0: mov      r1, r6
00773ff4: bl       #0x30ec94
00773ff8: str      r0, [sp, #4]
00773ffc: ldr      r0, [r4, #0x24]
00774000: bl       #0x30e964
00774004: mov      r1, #0x41000000
00774008: add      r1, r1, #0xa00000
0077400c: bl       #0x30ed6c
00774010: mov      r1, #0x41000000
00774014: mov      sb, r0
00774018: add      r1, r1, #0xa00000
0077401c: mov      r0, fp
00774020: bl       #0x30ec94
00774024: mov      r1, r0
00774028: mov      r0, r7
0077402c: bl       #0x30ec94
00774030: mov      r1, r0
00774034: mov      r0, sb
00774038: bl       #0x30ec94
0077403c: mov      r7, r0
00774040: ldr      r0, [r4, #0x28]
00774044: bl       #0x30e964
00774048: mov      r1, #0x41000000
0077404c: add      r1, r1, #0xa00000
00774050: bl       #0x30ed6c
00774054: mov      r1, #0x41000000
00774058: mov      r4, r0
0077405c: add      r1, r1, #0xa00000
00774060: mov      r0, sl
00774064: bl       #0x30ec94
00774068: mov      r1, r0
0077406c: mov      r0, r6
00774070: bl       #0x30ec94
00774074: mov      r1, r0
00774078: mov      r0, r4
0077407c: bl       #0x30ec94
00774080: cmp      r8, #0
00774084: mov      r4, r0
00774088: bne      #0x7740ec
0077408c: ldr      r0, [sp, #4]
00774090: ldr      r1, [r5]
00774094: bl       #0x30ed6c
00774098: mov      r1, r4
0077409c: bl       #0x30e3ac
007740a0: str      r0, [r5]
007740a4: ldr      r1, [r5, #4]
007740a8: ldr      r0, [sp]
007740ac: bl       #0x30ed6c
007740b0: mov      r1, r7
007740b4: bl       #0x30e3ac
007740b8: str      r0, [r5, #4]
007740bc: add      sp, sp, #0xc
007740c0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007740c4: ldr      r6, [r4, #0xc]
007740c8: ldr      r1, [r6, #0xbc]
007740cc: ldr      r0, [r6, #0xc0]
007740d0: bl       #0x30e3ac
007740d4: ldr      r1, [r6, #0xb4]
007740d8: mov      fp, r0
007740dc: ldr      r0, [r6, #0xb8]
007740e0: bl       #0x30e3ac
007740e4: mov      sl, r0
007740e8: b        #0x773fbc
007740ec: ldr      r0, [sp]
007740f0: ldr      r1, [r5]
007740f4: bl       #0x30ed6c
007740f8: mov      r1, r7
007740fc: bl       #0x30e3ac
00774100: str      r0, [r5]
00774104: ldr      r1, [r5, #4]
00774108: ldr      r0, [sp, #4]
0077410c: bl       #0x30ed6c
00774110: mov      r1, r4
00774114: bl       #0x30e3ac
00774118: str      r0, [r5, #4]
0077411c: b        #0x7740bc
00774120: eoreq    r0, r2, r4, lsr fp
00774124: strheq   r3, [r0], -r4
