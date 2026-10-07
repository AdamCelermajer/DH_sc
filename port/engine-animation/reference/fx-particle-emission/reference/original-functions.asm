
# _ZN6glitch2ps16PGenerationModelINS0_9SParticleEE19initGenerationModelEv
0064fc50: push     {r4, lr}
0064fc54: ldr      r3, [r0]
0064fc58: mov      r2, #0
0064fc5c: str      r2, [r0, #0x10]
0064fc60: mov      r4, r0
0064fc64: ldr      r0, [r3, #-0xc]
0064fc68: sub      sp, sp, #8
0064fc6c: add      r0, r4, r0
0064fc70: ldr      r1, [r0, #0x24]
0064fc74: ldr      r2, [r0, #0x28]
0064fc78: cmp      r1, r2
0064fc7c: beq      #0x64fc98
0064fc80: add      r3, sp, #4
0064fc84: add      r0, r0, #0x24
0064fc88: bl       #0x64f900
0064fc8c: ldr      r3, [r4]
0064fc90: ldr      r0, [r3, #-0xc]
0064fc94: add      r0, r4, r0
0064fc98: add      r0, r0, #0x24
0064fc9c: ldr      r1, [r4, #8]
0064fca0: bl       #0x64fb44
0064fca4: add      sp, sp, #8
0064fca8: pop      {r4, pc}

# _ZNK6glitch7collada18SAnimationAccessor8getValueEiPvRib
0066a1a8: push     {r4, r5, r6, r7, r8, lr}
0066a1ac: sub      sp, sp, #8
0066a1b0: mov      r7, r1
0066a1b4: mov      r6, r2
0066a1b8: mov      r5, r3
0066a1bc: mov      r8, r0
0066a1c0: ldrb     r4, [sp, #0x20]
0066a1c4: bl       #0x66a004
0066a1c8: mov      r1, r8
0066a1cc: ldr      ip, [r0]
0066a1d0: mov      r2, r7
0066a1d4: mov      r3, r6
0066a1d8: str      r5, [sp]
0066a1dc: str      r4, [sp, #4]
0066a1e0: mov      lr, pc
0066a1e4: ldr      pc, [ip, #0x68]
0066a1e8: add      sp, sp, #8
0066a1ec: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK6glitch7collada15animation_track8CFloatEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
006e32cc: push     {r4, r5, r6, lr}
006e32d0: mov      r0, r1
006e32d4: mov      r1, #0
006e32d8: mov      r5, r3
006e32dc: mov      r4, r2
006e32e0: bl       #0x669e24
006e32e4: ldr      r3, [r0, #4]
006e32e8: ldr      r3, [r3, r4, lsl #2]
006e32ec: str      r3, [r5]
006e32f0: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada24CParticleSystemSceneNode4initEv
0064fd60: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0064fd64: ldr      r4, [r0, #0x158]
0064fd68: ldr      r7, [r0, #0x15c]
0064fd6c: ldr      sb, [pc, #0x444]
0064fd70: sub      sp, sp, #0x64
0064fd74: cmp      r4, r7
0064fd78: mov      r6, r0
0064fd7c: add      sb, pc, sb
0064fd80: beq      #0x64fe2c
0064fd84: ldr      r3, [pc, #0x430]
0064fd88: ldr      r1, [pc, #0x430]
0064fd8c: add      r2, r0, #0x134
0064fd90: add      r3, pc, r3
0064fd94: str      r3, [sp, #0xc]
0064fd98: ldr      r3, [pc, #0x424]
0064fd9c: str      r1, [sp, #8]
0064fda0: str      r2, [sp]
0064fda4: add      r3, pc, r3
0064fda8: str      r3, [sp, #0x10]
0064fdac: ldr      r3, [pc, #0x414]
0064fdb0: mov      fp, sb
0064fdb4: add      r3, pc, r3
0064fdb8: str      r3, [sp, #0x18]
0064fdbc: ldr      r3, [pc, #0x408]
0064fdc0: add      r3, pc, r3
0064fdc4: str      r3, [sp, #0x14]
0064fdc8: ldr      r3, [r4]
0064fdcc: mov      r2, #0
0064fdd0: mov      r1, #6
0064fdd4: ldr      r8, [r3, #0x1c]
0064fdd8: ldr      r0, [r3, #4]
0064fddc: cmp      r8, #0
0064fde0: addne    r8, r8, #4
0064fde4: bl       #0x5cef08
0064fde8: ldr      r3, [r4]
0064fdec: mov      r5, r0
0064fdf0: ldr      r3, [r3, #4]
0064fdf4: ldrh     r2, [r3, #0xe]
0064fdf8: cmp      r2, r0
0064fdfc: ldrhi    sl, [r3, #0x20]
0064fe00: ldr      r3, [r6, #0x134]
0064fe04: movls    sl, #0
0064fe08: addhi    sl, sl, r0, lsl #4
0064fe0c: ldr      r2, [r3, #0x24]
0064fe10: ldr      r2, [r2, #0x20]
0064fe14: ldr      r2, [r2, #4]
0064fe18: cmp      r2, #0
0064fe1c: beq      #0x64ff88
0064fe20: add      r4, r4, #4
0064fe24: cmp      r4, r7
0064fe28: bne      #0x64fdc8
0064fe2c: add      r4, sp, #0x58
0064fe30: mov      r0, r4
0064fe34: mov      r1, r6
0064fe38: mov      r2, #0
0064fe3c: ldr      r3, [r6]
0064fe40: mov      lr, pc
0064fe44: ldr      pc, [r3, #0x84]
0064fe48: ldr      r3, [sp, #0x58]
0064fe4c: mov      r1, #6
0064fe50: mov      r2, #0
0064fe54: ldr      r0, [r3, #4]
0064fe58: bl       #0x5cef08
0064fe5c: str      r0, [r6, #0x174]
0064fe60: mov      r0, r4
0064fe64: bl       #0x310be8
0064fe68: ldr      ip, [r6, #0x178]
0064fe6c: add      r4, sp, #0x54
0064fe70: mov      r0, r4
0064fe74: ldr      r3, [ip]
0064fe78: mov      r1, r6
0064fe7c: mov      r2, #0
0064fe80: ldr      r5, [r3, #-0xc]
0064fe84: ldr      r3, [r6]
0064fe88: add      r5, ip, r5
0064fe8c: mov      lr, pc
0064fe90: ldr      pc, [r3, #0x84]
0064fe94: ldr      r1, [pc, #0x334]
0064fe98: mov      r0, r5
0064fe9c: add      r1, pc, r1
0064fea0: bl       #0x64d0bc
0064fea4: ldr      ip, [r5, #0x34]
0064fea8: add      r1, r5, #0x30
0064feac: mov      lr, r0
0064feb0: cmp      ip, #0
0064feb4: moveq    ip, r1
0064feb8: beq      #0x64fee8
0064febc: mov      r2, r1
0064fec0: b        #0x64fec8
0064fec4: mov      ip, r3
0064fec8: ldr      r3, [ip, #0x10]
0064fecc: cmp      lr, r3
0064fed0: ldrhi    r3, [ip, #0xc]
0064fed4: ldrls    r3, [ip, #8]
0064fed8: movhi    ip, r2
0064fedc: mov      r2, ip
0064fee0: cmp      r3, #0
0064fee4: bne      #0x64fec4
0064fee8: cmp      r1, ip
0064feec: beq      #0x64ff60
0064fef0: ldr      r2, [ip, #0x10]
0064fef4: mov      r3, ip
0064fef8: cmp      lr, r2
0064fefc: blo      #0x64ff60
0064ff00: ldr      r2, [r3, #0x14]
0064ff04: cmp      r2, #0
0064ff08: beq      #0x64ff3c
0064ff0c: ldr      r3, [sp, #0x54]
0064ff10: add      r0, sp, #0x60
0064ff14: str      r3, [sp, #0x48]
0064ff18: cmp      r3, #0
0064ff1c: ldrne    r1, [r3]
0064ff20: addne    r1, r1, #1
0064ff24: strne    r1, [r3]
0064ff28: ldrne    r3, [sp, #0x48]
0064ff2c: ldr      r1, [r2]
0064ff30: str      r1, [r0, #-0x18]!
0064ff34: str      r3, [r2]
0064ff38: bl       #0x310be8
0064ff3c: mov      r0, r4
0064ff40: bl       #0x310be8
0064ff44: ldr      r3, [r6, #0x178]
0064ff48: mov      r0, r3
0064ff4c: ldr      r3, [r3]
0064ff50: mov      lr, pc
0064ff54: ldr      pc, [r3, #0xc]
0064ff58: add      sp, sp, #0x64
0064ff5c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0064ff60: add      r3, sp, #0x30
0064ff64: str      lr, [sp, #0x30]
0064ff68: add      r0, sp, #0x40
0064ff6c: mov      lr, #0
0064ff70: add      r2, sp, #0x44
0064ff74: str      lr, [sp, #0x34]
0064ff78: str      ip, [sp, #0x44]
0064ff7c: bl       #0x63aa74
0064ff80: ldr      r3, [sp, #0x40]
0064ff84: b        #0x64ff00
0064ff88: ldr      r2, [r6, #0x138]
0064ff8c: cmp      r3, #0
0064ff90: str      r3, [sp, #0x20]
0064ff94: str      r2, [sp, #0x24]
0064ff98: beq      #0x64ffb0
0064ff9c: ldr      r2, [r3, #4]
0064ffa0: cmp      r2, #0
0064ffa4: addne    r2, r2, #1
0064ffa8: strne    r2, [r3, #4]
0064ffac: ldrne    r3, [r6, #0x134]
0064ffb0: mov      r1, #0
0064ffb4: str      r1, [sp, #0x28]
0064ffb8: ldr      r3, [r3, #0x24]
0064ffbc: ldr      r0, [r3, #0x20]
0064ffc0: ldr      r3, [r0, #0x34]
0064ffc4: cmp      r3, r1
0064ffc8: addeq    r0, r0, #0x18
0064ffcc: streq    r0, [sp, #0x28]
0064ffd0: bne      #0x650170
0064ffd4: ldr      r1, [sp, #8]
0064ffd8: ldmib    r0, {r3, ip}
0064ffdc: ldr      r2, [fp, r1]
0064ffe0: bic      r3, r3, r3, asr #31
0064ffe4: cmp      r3, ip
0064ffe8: strle    r3, [sp, #0x2c]
0064ffec: strgt    ip, [sp, #0x2c]
0064fff0: add      r1, sp, #0x20
0064fff4: ldr      r0, [r2]
0064fff8: mov      r3, #0
0064fffc: add      r2, sp, #0x5c
00650000: str      r1, [sp, #4]
00650004: str      r3, [sp, #0x5c]
00650008: bl       #0x60c59c
0065000c: ldr      r2, [sp, #0x5c]
00650010: cmp      r2, #0
00650014: beq      #0x6500d0
00650018: ldr      r0, [r2, #0x14]
0065001c: ldr      r3, [r6, #0x178]
00650020: ldr      r1, [sp, #0x14]
00650024: ldr      r0, [r0, #0xc]
00650028: ldr      r2, [r3]
0065002c: str      r0, [sp, #0x1c]
00650030: ldr      sb, [r2, #-0xc]
00650034: add      sb, r3, sb
00650038: mov      r0, sb
0065003c: bl       #0x64d0bc
00650040: ldr      ip, [sb, #0x34]
00650044: add      r1, sb, #0x30
00650048: mov      lr, r0
0065004c: cmp      ip, #0
00650050: moveq    ip, r1
00650054: beq      #0x650084
00650058: mov      r2, r1
0065005c: b        #0x650064
00650060: mov      ip, r3
00650064: ldr      r3, [ip, #0x10]
00650068: cmp      lr, r3
0065006c: ldrhi    r3, [ip, #0xc]
00650070: ldrls    r3, [ip, #8]
00650074: movhi    ip, r2
00650078: mov      r2, ip
0065007c: cmp      r3, #0
00650080: bne      #0x650060
00650084: cmp      r1, ip
00650088: beq      #0x65009c
0065008c: ldr      r2, [ip, #0x10]
00650090: mov      r3, ip
00650094: cmp      lr, r2
00650098: bhs      #0x6500c0
0065009c: add      r3, sp, #0x38
006500a0: str      lr, [sp, #0x38]
006500a4: add      r0, sp, #0x4c
006500a8: mov      lr, #0
006500ac: add      r2, sp, #0x50
006500b0: str      lr, [sp, #0x3c]
006500b4: str      ip, [sp, #0x50]
006500b8: bl       #0x63aa74
006500bc: ldr      r3, [sp, #0x4c]
006500c0: ldr      r3, [r3, #0x14]
006500c4: cmp      r3, #0
006500c8: ldrne    r2, [sp, #0x1c]
006500cc: strne    r2, [r3]
006500d0: movw     r3, #0xffff
006500d4: cmp      r5, r3
006500d8: beq      #0x650180
006500dc: ldr      r3, [sl]
006500e0: mov      r2, #0x56
006500e4: ldr      r0, [sp]
006500e8: cmp      r3, #0
006500ec: addne    r3, r3, #4
006500f0: mov      r1, r8
006500f4: bl       #0x61c91c
006500f8: subs     r2, r0, #0
006500fc: beq      #0x650180
00650100: ldr      r3, [r6, #0x178]
00650104: ldr      r1, [sp, #0xc]
00650108: ldr      r0, [r3]
0065010c: ldr      r0, [r0, #-0xc]
00650110: add      r0, r3, r0
00650114: bl       #0x64fcbc
00650118: mov      r1, r8
0065011c: mov      r2, #0x100
00650120: mov      r3, #0xff
00650124: ldr      r0, [sp]
00650128: bl       #0x61c0c8
0065012c: ldr      r3, [r6, #0x178]
00650130: subs     r1, r0, #0
00650134: movne    r1, #1
00650138: strb     r1, [r6, #0x170]
0065013c: ldr      ip, [r3]
00650140: mov      r2, r0
00650144: ldr      r1, [sp, #0x10]
00650148: ldr      r0, [ip, #-0xc]
0065014c: add      r0, r3, r0
00650150: bl       #0x64fcbc
00650154: ldr      r0, [sp, #0x5c]
00650158: cmp      r0, #0
0065015c: beq      #0x650164
00650160: bl       #0x60bbd4
00650164: ldr      r0, [sp, #4]
00650168: bl       #0x619474
0065016c: b        #0x64fe20
00650170: ldr      r0, [sp]
00650174: bl       #0x60e374
00650178: str      r0, [sp, #0x28]
0065017c: b        #0x64ffd4
00650180: mov      r2, #0x19
00650184: ldr      r0, [sp]
00650188: mov      r1, r8
0065018c: mov      r3, #0xff
00650190: bl       #0x61c0c8
00650194: subs     r2, r0, #0
00650198: bne      #0x650100
0065019c: mov      r2, #0x56
006501a0: ldr      r0, [sp]
006501a4: mov      r1, r8
006501a8: ldr      r3, [sp, #0x18]
006501ac: bl       #0x61c91c
006501b0: mov      r2, r0
006501b4: b        #0x650100
006501b8: eorseq   r4, r4, r4, lsl sp
006501bc: eoreq    r5, sb, r8, lsl #7
006501c0: andeq    r0, r0, r4, ror sb
006501c4: eoreq    r5, sb, ip, lsl #7
006501c8: eoreq    r5, sb, r4, asr #6
006501cc: eoreq    r5, sb, r0, lsl r3
006501d0: eoreq    r5, sb, ip, asr #6

# _ZN6glitch7collada15animation_track8CFloatEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
006e3374: push     {r4, r5, r6, lr}
006e3378: mov      r4, r1
006e337c: mov      r1, #0
006e3380: mov      r5, r2
006e3384: mov      r6, r3
006e3388: bl       #0x669e24
006e338c: ldr      r3, [r0, #4]
006e3390: ldr      r4, [r3, r4, lsl #2]
006e3394: ldr      r0, [r3, r5, lsl #2]
006e3398: mov      r1, r4
006e339c: bl       #0x30e3ac
006e33a0: mov      r1, r0
006e33a4: mov      r0, r6
006e33a8: bl       #0x30ed6c
006e33ac: mov      r1, r0
006e33b0: mov      r0, r4
006e33b4: bl       #0x30eba4
006e33b8: ldr      r3, [sp, #0x10]
006e33bc: str      r0, [r3]
006e33c0: pop      {r4, r5, r6, pc}

# _ZNK6glitch7collada18SAnimationAccessor9getOutputEi
00669e24: ldm      r0, {r2, r3}
00669e28: mov      r0, #0x1c
00669e2c: ldr      r2, [r2, #8]
00669e30: mla      r2, r0, r1, r2
00669e34: ldr      r2, [r2, #0x18]
00669e38: add      r3, r3, r2, lsl #3
00669e3c: add      r0, r3, #4
00669e40: bx       lr

# _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoEiiRiRfi
0066b814: push     {r4, r5, r6, r7, lr}
0066b818: sub      sp, sp, #0x14
0066b81c: mov      r5, r2
0066b820: mov      r4, r3
0066b824: mov      r6, r0
0066b828: mov      r7, r1
0066b82c: bl       #0x669eec
0066b830: ldr      ip, [sp, #0x28]
0066b834: mov      r2, r0
0066b838: mov      r1, r7
0066b83c: str      ip, [sp, #4]
0066b840: ldr      ip, [sp, #0x2c]
0066b844: mov      r0, r6
0066b848: mov      r3, r5
0066b84c: str      r4, [sp]
0066b850: str      ip, [sp, #8]
0066b854: bl       #0x66b65c
0066b858: add      sp, sp, #0x14
0066b85c: pop      {r4, r5, r6, r7, pc}

# _ZN6glitch7collada24IParticleSystemSceneNodeD2Ev
006678b8: push     {r4, r5, r6, lr}
006678bc: ldr      r3, [r1]
006678c0: mov      r4, r0
006678c4: mov      r5, r1
006678c8: str      r3, [r0]
006678cc: ldr      r3, [r3, #-0x1c]
006678d0: ldr      r2, [r1, #0x10]
006678d4: str      r2, [r0, r3]
006678d8: ldr      r3, [r0]
006678dc: ldr      r2, [r1, #0x14]
006678e0: ldr      r3, [r3, #-0xc]
006678e4: str      r2, [r0, r3]
006678e8: ldr      r0, [r0, #0x164]
006678ec: cmp      r0, #0
006678f0: beq      #0x6678f8
006678f4: bl       #0x310450
006678f8: add      r0, r4, #0x158
006678fc: bl       #0x6677d8
00667900: ldr      r0, [r4, #0x140]
00667904: cmp      r0, #0
00667908: beq      #0x667910
0066790c: bl       #0x31d584
00667910: add      r0, r4, #0x134
00667914: bl       #0x619474
00667918: mov      r0, r4
0066791c: add      r1, r5, #4
00667920: bl       #0x598cbc
00667924: mov      r0, r4
00667928: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada21intrusive_ptr_releaseEPNS0_15CAnimationBlockE
0060bbd4: b        #0x60bb1c

# _ZN6glitch2ps16IParticleContextINS0_9SParticleEED2Ev
0064d298: push     {r4, r5, r6, lr}
0064d29c: ldr      r3, [pc, #0x60]
0064d2a0: ldr      r2, [pc, #0x60]
0064d2a4: ldr      r1, [r0, #0x40]
0064d2a8: add      r3, pc, r3
0064d2ac: ldr      r2, [r3, r2]
0064d2b0: cmp      r1, #0
0064d2b4: mov      r4, r0
0064d2b8: add      r2, r2, #8
0064d2bc: str      r2, [r0]
0064d2c0: bne      #0x64d2dc
0064d2c4: ldr      r0, [r4, #0x24]
0064d2c8: cmp      r0, #0
0064d2cc: beq      #0x64d2d4
0064d2d0: bl       #0x310450
0064d2d4: mov      r0, r4
0064d2d8: pop      {r4, r5, r6, pc}
0064d2dc: add      r5, r0, #0x30
0064d2e0: mov      r0, r5
0064d2e4: ldr      r1, [r4, #0x34]
0064d2e8: bl       #0x63a1c4
0064d2ec: mov      r3, #0
0064d2f0: str      r5, [r4, #0x3c]
0064d2f4: str      r3, [r4, #0x40]
0064d2f8: str      r5, [r4, #0x38]
0064d2fc: str      r3, [r4, #0x34]
0064d300: b        #0x64d2c4
0064d304: eorseq   r7, r4, r8, ror #15
0064d308: andeq    r2, r0, r8, lsr #4

# _ZNSt6vectorIN6glitch2ps9SParticleENS0_4core10SAllocatorIS2_LNS0_6memory13E_MEMORY_HINTE0EEEE6resizeEjRKS2_
0065311c: push     {r4, r5, lr}
00653120: ldm      r0, {r4, ip}
00653124: movw     r3, #0x5c29
00653128: movt     r3, #0xc28f
0065312c: rsb      r5, r4, ip
00653130: asr      r5, r5, #2
00653134: mul      r5, r3, r5
00653138: sub      sp, sp, #0xc
0065313c: cmp      r1, r5
00653140: mov      r3, r2
00653144: bhs      #0x65316c
00653148: mov      r3, #0x64
0065314c: mla      r1, r3, r1, r4
00653150: cmp      r1, ip
00653154: beq      #0x653164
00653158: mov      r2, ip
0065315c: add      r3, sp, #4
00653160: bl       #0x64f900
00653164: add      sp, sp, #0xc
00653168: pop      {r4, r5, pc}
0065316c: rsb      r2, r5, r1
00653170: mov      r1, ip
00653174: bl       #0x652d6c
00653178: b        #0x653164

# _ZN6glitch7collada16CColladaDatabaseD1Ev
00619474: push     {r4, r5, r6, lr}
00619478: mov      r4, r0
0061947c: ldr      r0, [r0]
00619480: ldr      r5, [pc, #0x7c]
00619484: cmp      r0, #0
00619488: add      r5, pc, r5
0061948c: beq      #0x6194c8
00619490: ldr      r3, [r0, #4]
00619494: cmp      r3, #0
00619498: beq      #0x6194c8
0061949c: bl       #0x31d584
006194a0: ldr      r3, [pc, #0x60]
006194a4: ldr      r6, [r5, r3]
006194a8: ldr      r3, [r6]
006194ac: ldrb     r3, [r3, #0x28]
006194b0: cmp      r3, #0
006194b4: beq      #0x6194c8
006194b8: ldr      r3, [r4]
006194bc: ldr      r3, [r3, #4]
006194c0: cmp      r3, #1
006194c4: beq      #0x6194d8
006194c8: mov      r3, #0
006194cc: str      r3, [r4]
006194d0: mov      r0, r4
006194d4: pop      {r4, r5, r6, pc}
006194d8: ldr      r3, [pc, #0x2c]
006194dc: mov      r1, r4
006194e0: ldr      r3, [r5, r3]
006194e4: ldr      r0, [r3]
006194e8: bl       #0x60b918
006194ec: ldr      r3, [r4]
006194f0: ldr      r0, [r6]
006194f4: mov      r2, #0
006194f8: ldr      r1, [r3, #0x20]
006194fc: bl       #0x659b60
00619500: b        #0x6194c8
00619504: eorseq   fp, r7, r8, lsl #12
00619508: andeq    r4, r0, r8, asr #8
0061950c: andeq    r0, r0, r4, ror sb

# _ZNK6glitch7collada17CAnimationTrackEx8getValueERKNS0_18SAnimationAccessorEiPvRib
006e2c40: push     {r4, r5, r6, r7, r8, lr}
006e2c44: sub      sp, sp, #0x10
006e2c48: ldr      r4, [sp, #0x28]
006e2c4c: mov      lr, #0
006e2c50: add      ip, sp, #0x10
006e2c54: ldr      r7, [r4]
006e2c58: mov      r6, r1
006e2c5c: str      lr, [ip, #-4]!
006e2c60: mov      r5, r0
006e2c64: mov      r8, r3
006e2c68: mov      r1, lr
006e2c6c: mov      r3, ip
006e2c70: mov      r0, r6
006e2c74: add      ip, sp, #8
006e2c78: str      r7, [sp, #4]
006e2c7c: str      ip, [sp]
006e2c80: ldrb     r7, [sp, #0x2c]
006e2c84: bl       #0x66b814
006e2c88: tst      r0, r7
006e2c8c: bne      #0x6e2cbc
006e2c90: mov      r0, r5
006e2c94: mov      r1, r6
006e2c98: mov      r3, r8
006e2c9c: ldr      ip, [r5]
006e2ca0: ldr      r2, [sp, #0xc]
006e2ca4: mov      lr, pc
006e2ca8: ldr      pc, [ip, #0x28]
006e2cac: ldr      r3, [sp, #0xc]
006e2cb0: str      r3, [r4]
006e2cb4: add      sp, sp, #0x10
006e2cb8: pop      {r4, r5, r6, r7, r8, pc}
006e2cbc: ldr      r3, [sp, #8]
006e2cc0: ldr      r2, [sp, #0xc]
006e2cc4: str      r8, [sp, #4]
006e2cc8: str      r3, [sp]
006e2ccc: mov      r0, r5
006e2cd0: mov      r1, r6
006e2cd4: ldr      ip, [r5]
006e2cd8: add      r3, r2, #1
006e2cdc: mov      lr, pc
006e2ce0: ldr      pc, [ip, #0x20]
006e2ce4: b        #0x6e2cac

# _ZNSt6vectorIN6glitch2ps9SParticleENS0_4core10SAllocatorIS2_LNS0_6memory13E_MEMORY_HINTE0EEEE7reserveEj
0064fb44: push     {r4, r5, r6, lr}
0064fb48: mov      r4, r0
0064fb4c: ldr      r2, [r0]
0064fb50: ldr      r0, [r0, #8]
0064fb54: movw     r3, #0x5c29
0064fb58: movt     r3, #0xc28f
0064fb5c: rsb      r0, r2, r0
0064fb60: asr      r0, r0, #2
0064fb64: mul      r3, r3, r0
0064fb68: sub      sp, sp, #8
0064fb6c: cmp      r1, r3
0064fb70: str      r1, [sp, #4]
0064fb74: bls      #0x64fc14
0064fb78: movw     r3, #0x5c28
0064fb7c: movt     r3, #0x28f
0064fb80: cmp      r1, r3
0064fb84: bhi      #0x64fc1c
0064fb88: ldr      r3, [r4, #4]
0064fb8c: movw     r1, #0x5c29
0064fb90: movt     r1, #0xc28f
0064fb94: rsb      r5, r2, r3
0064fb98: asr      r5, r5, #2
0064fb9c: cmp      r2, #0
0064fba0: mul      r5, r1, r5
0064fba4: beq      #0x64fc30
0064fba8: mov      r0, r4
0064fbac: add      r1, sp, #4
0064fbb0: bl       #0x64fa24
0064fbb4: ldr      r3, [r4]
0064fbb8: mov      r6, r0
0064fbbc: ldr      r0, [r4, #4]
0064fbc0: cmp      r0, r3
0064fbc4: beq      #0x64fbf4
0064fbc8: sub      r2, r0, #0x64
0064fbcc: rsb      r2, r3, r2
0064fbd0: movw     r3, #0x5c29
0064fbd4: lsr      r2, r2, #2
0064fbd8: movt     r3, #0x28f
0064fbdc: mul      r3, r3, r2
0064fbe0: mvn      r2, #0x63
0064fbe4: bic      r3, r3, #0xc0000000
0064fbe8: mul      r3, r2, r3
0064fbec: add      r3, r3, r2
0064fbf0: add      r0, r0, r3
0064fbf4: bl       #0x310450
0064fbf8: ldr      r2, [sp, #4]
0064fbfc: mov      r3, #0x64
0064fc00: mla      r5, r3, r5, r6
0064fc04: mla      r3, r3, r2, r6
0064fc08: str      r5, [r4, #4]
0064fc0c: str      r3, [r4, #8]
0064fc10: str      r6, [r4]
0064fc14: add      sp, sp, #8
0064fc18: pop      {r4, r5, r6, pc}
0064fc1c: ldr      r0, [pc, #0x28]
0064fc20: add      r0, pc, r0
0064fc24: bl       #0x708e40
0064fc28: ldr      r2, [r4]
0064fc2c: b        #0x64fb88
0064fc30: ldr      r3, [sp, #4]
0064fc34: mov      r0, #0x64
0064fc38: mov      r1, r2
0064fc3c: mul      r0, r0, r3
0064fc40: bl       #0x310568
0064fc44: mov      r6, r0
0064fc48: b        #0x64fbf8
0064fc4c: eoreq    lr, r6, r8, asr #16

# _ZNK6glitch7collada18SAnimationAccessor11getAnimatorEv
0066a004: ldr      r3, [r0]
0066a008: ldr      r0, [r3, #0x14]
0066a00c: bx       lr

# _ZNSt6vectorIN6glitch2ps9SParticleENS0_4core10SAllocatorIS2_LNS0_6memory13E_MEMORY_HINTE0EEEE8_M_eraseEPS2_S9_RKSt12__false_type
0064f900: push     {r4, r5, r6}
0064f904: mov      r3, r0
0064f908: ldr      r0, [r0, #4]
0064f90c: movw     ip, #0x5c29
0064f910: movt     ip, #0xc28f
0064f914: rsb      r0, r2, r0
0064f918: asr      r0, r0, #2
0064f91c: mul      ip, ip, r0
0064f920: cmp      ip, #0
0064f924: movle    ip, r1
0064f928: ble      #0x64fa14
0064f92c: mov      r5, ip
0064f930: mov      r4, r1
0064f934: ldr      r0, [r2]
0064f938: subs     r5, r5, #1
0064f93c: str      r0, [r4]
0064f940: ldr      r0, [r2, #4]
0064f944: str      r0, [r4, #4]
0064f948: ldr      r0, [r2, #8]
0064f94c: str      r0, [r4, #8]
0064f950: ldr      r0, [r2, #0xc]
0064f954: str      r0, [r4, #0xc]
0064f958: ldr      r0, [r2, #0x10]
0064f95c: str      r0, [r4, #0x10]
0064f960: ldr      r0, [r2, #0x14]
0064f964: str      r0, [r4, #0x14]
0064f968: ldr      r0, [r2, #0x18]
0064f96c: str      r0, [r4, #0x18]
0064f970: ldr      r0, [r2, #0x1c]
0064f974: str      r0, [r4, #0x1c]
0064f978: ldr      r0, [r2, #0x20]
0064f97c: str      r0, [r4, #0x20]
0064f980: ldr      r0, [r2, #0x24]
0064f984: str      r0, [r4, #0x24]
0064f988: ldr      r0, [r2, #0x28]
0064f98c: str      r0, [r4, #0x28]
0064f990: ldr      r0, [r2, #0x2c]
0064f994: str      r0, [r4, #0x2c]
0064f998: ldr      r0, [r2, #0x30]
0064f99c: str      r0, [r4, #0x30]
0064f9a0: ldr      r0, [r2, #0x34]
0064f9a4: str      r0, [r4, #0x34]
0064f9a8: ldr      r0, [r2, #0x38]
0064f9ac: str      r0, [r4, #0x38]
0064f9b0: ldr      r0, [r2, #0x3c]
0064f9b4: str      r0, [r4, #0x3c]
0064f9b8: ldr      r0, [r2, #0x40]
0064f9bc: str      r0, [r4, #0x40]
0064f9c0: ldr      r0, [r2, #0x44]
0064f9c4: str      r0, [r4, #0x44]
0064f9c8: ldr      r0, [r2, #0x48]
0064f9cc: str      r0, [r4, #0x48]
0064f9d0: ldr      r6, [r2, #0x4c]
0064f9d4: str      r6, [r4, #0x4c]
0064f9d8: ldr      r6, [r2, #0x50]
0064f9dc: str      r6, [r4, #0x50]
0064f9e0: ldr      r6, [r2, #0x54]
0064f9e4: str      r6, [r4, #0x54]
0064f9e8: ldr      r6, [r2, #0x58]
0064f9ec: str      r6, [r4, #0x58]
0064f9f0: ldr      r6, [r2, #0x5c]
0064f9f4: str      r6, [r4, #0x5c]
0064f9f8: ldr      r6, [r2, #0x60]
0064f9fc: add      r2, r2, #0x64
0064fa00: str      r6, [r4, #0x60]
0064fa04: add      r4, r4, #0x64
0064fa08: bne      #0x64f934
0064fa0c: mov      r2, #0x64
0064fa10: mla      ip, r2, ip, r1
0064fa14: str      ip, [r3, #4]
0064fa18: mov      r0, r1
0064fa1c: pop      {r4, r5, r6}
0064fa20: bx       lr

# _ZN6glitch7collada24IParticleSystemSceneNodeC2ERKNS0_16CColladaDatabaseEPNS_3res6vectorINS5_6StringEEEPNS0_14CRootSceneNodeE
00667a98: push     {r4, r5, r6, r7, r8, lr}
00667a9c: sub      sp, sp, #0x30
00667aa0: add      r4, sp, #8
00667aa4: mov      ip, #0
00667aa8: mov      lr, #0x3f800000
00667aac: mov      r6, r2
00667ab0: str      r4, [sp]
00667ab4: mvn      r2, #0
00667ab8: add      r4, sp, #0x18
00667abc: mov      r5, r1
00667ac0: mov      r7, r3
00667ac4: add      r1, r1, #4
00667ac8: add      r3, sp, #0x24
00667acc: str      r4, [sp, #4]
00667ad0: str      ip, [sp, #0x10]
00667ad4: mov      r4, r0
00667ad8: str      lr, [sp, #0x20]
00667adc: str      ip, [sp, #0x24]
00667ae0: str      ip, [sp, #0x28]
00667ae4: str      ip, [sp, #0x2c]
00667ae8: str      ip, [sp, #8]
00667aec: str      ip, [sp, #0xc]
00667af0: str      lr, [sp, #0x14]
00667af4: str      lr, [sp, #0x18]
00667af8: str      lr, [sp, #0x1c]
00667afc: ldr      r8, [sp, #0x48]
00667b00: bl       #0x5990c0
00667b04: ldr      r3, [r6]
00667b08: ldr      r1, [pc, #0xbc]
00667b0c: str      r3, [r4, #0x134]
00667b10: ldr      r2, [r6, #4]
00667b14: cmp      r3, #0
00667b18: add      r1, pc, r1
00667b1c: str      r2, [r4, #0x138]
00667b20: beq      #0x667b34
00667b24: ldr      r2, [r3, #4]
00667b28: cmp      r2, #0
00667b2c: addne    r2, r2, #1
00667b30: strne    r2, [r3, #4]
00667b34: ldr      r2, [pc, #0x94]
00667b38: mov      r3, #0
00667b3c: mov      r0, r8
00667b40: ldr      r2, [r1, r2]
00667b44: mov      r1, r4
00667b48: add      r2, r2, #4
00667b4c: str      r2, [r4, #0x130]
00667b50: ldr      r2, [r5]
00667b54: str      r2, [r4]
00667b58: ldr      ip, [r5, #0x10]
00667b5c: ldr      r2, [r2, #-0x1c]
00667b60: str      ip, [r4, r2]
00667b64: ldr      r2, [r4]
00667b68: ldr      ip, [r5, #0x14]
00667b6c: ldr      r2, [r2, #-0xc]
00667b70: str      ip, [r4, r2]
00667b74: strb     r3, [r4, #0x170]
00667b78: strb     r3, [r4, #0x13c]
00667b7c: strb     r3, [r4, #0x13d]
00667b80: strb     r3, [r4, #0x13e]
00667b84: strb     r3, [r4, #0x13f]
00667b88: str      r3, [r4, #0x140]
00667b8c: str      r3, [r4, #0x148]
00667b90: str      r3, [r4, #0x158]
00667b94: str      r3, [r4, #0x15c]
00667b98: str      r3, [r4, #0x160]
00667b9c: str      r3, [r4, #0x164]
00667ba0: str      r3, [r4, #0x168]
00667ba4: str      r3, [r4, #0x16c]
00667ba8: str      r7, [r4, #0x150]
00667bac: str      r8, [r4, #0x154]
00667bb0: bl       #0x65b210
00667bb4: mov      r0, r4
00667bb8: mov      r1, #2
00667bbc: bl       #0x59719c
00667bc0: mov      r0, r4
00667bc4: add      sp, sp, #0x30
00667bc8: pop      {r4, r5, r6, r7, r8, pc}
00667bcc: eorseq   ip, r2, r8, ror pc
00667bd0: strheq   r1, [r0], -r4

# _ZN6glitch2ps16PGenerationModelINS0_9SParticleEE17generateParticlesEv
0065317c: push     {r4, r5, r6, r7, r8, sl, lr}
00653180: ldr      r6, [r0]
00653184: sub      sp, sp, #0x6c
00653188: mov      r4, r0
0065318c: ldr      r3, [r6, #-0xc]
00653190: add      r3, r0, r3
00653194: ldr      r1, [r3, #0x4c]
00653198: ldr      r0, [r3, #0x48]
0065319c: ldr      r8, [r3, #0x28]
006531a0: ldr      r5, [r3, #0x24]
006531a4: bl       #0x30e3ac
006531a8: ldr      r1, [r4, #4]
006531ac: str      r0, [r4, #0xc]
006531b0: bl       #0x30ed6c
006531b4: ldr      r1, [r4, #0x10]
006531b8: bl       #0x30eba4
006531bc: mov      sl, r0
006531c0: bl       #0x30e4cc
006531c4: mov      r7, r0
006531c8: bl       #0x30e964
006531cc: mov      r1, r0
006531d0: mov      r0, sl
006531d4: bl       #0x30e3ac
006531d8: cmp      r7, #0
006531dc: str      r0, [r4, #0x10]
006531e0: ble      #0x6532ac
006531e4: rsb      r5, r5, r8
006531e8: movw     r3, #0x5c29
006531ec: asr      r5, r5, #2
006531f0: movt     r3, #0xc28f
006531f4: mul      r5, r3, r5
006531f8: ldr      r1, [r4, #8]
006531fc: add      r7, r7, r5
00653200: cmp      r7, r1
00653204: bgt      #0x6532a0
00653208: mov      r1, r7
0065320c: ldr      r0, [r6, #-0xc]
00653210: mov      r3, #0
00653214: mvn      ip, #0
00653218: add      r0, r4, r0
0065321c: mov      lr, #0x3f800000
00653220: add      r0, r0, #0x24
00653224: add      r2, sp, #4
00653228: str      r3, [sp, #0x60]
0065322c: str      r3, [sp, #4]
00653230: str      r3, [sp, #8]
00653234: str      r3, [sp, #0xc]
00653238: str      r3, [sp, #0x10]
0065323c: str      r3, [sp, #0x14]
00653240: str      r3, [sp, #0x18]
00653244: str      r3, [sp, #0x24]
00653248: str      r3, [sp, #0x28]
0065324c: str      r3, [sp, #0x2c]
00653250: str      r3, [sp, #0x34]
00653254: str      r3, [sp, #0x38]
00653258: str      r3, [sp, #0x3c]
0065325c: str      r3, [sp, #0x58]
00653260: str      r3, [sp, #0x5c]
00653264: strb     ip, [sp, #0x1f]
00653268: str      lr, [sp, #0x30]
0065326c: strb     ip, [sp, #0x1c]
00653270: strb     ip, [sp, #0x1d]
00653274: strb     ip, [sp, #0x1e]
00653278: str      lr, [sp, #0x20]
0065327c: bl       #0x65311c
00653280: ldr      r3, [r4]
00653284: mov      r0, #0x64
00653288: ldr      r3, [r3, #-0xc]
0065328c: add      r4, r4, r3
00653290: ldr      r3, [r4, #0x24]
00653294: mla      r0, r0, r5, r3
00653298: add      sp, sp, #0x6c
0065329c: pop      {r4, r5, r6, r7, r8, sl, pc}
006532a0: cmp      r1, #0
006532a4: bne      #0x65320c
006532a8: b        #0x653208
006532ac: ldr      r3, [r6, #-0xc]
006532b0: add      r4, r4, r3
006532b4: ldr      r0, [r4, #0x28]
006532b8: b        #0x653298

# _ZN6glitch7collada26CAnimationStreamingManager17getAnimationBlockERNS0_24SAnimationBlockSearchKeyERN5boost13intrusive_ptrINS0_15CAnimationBlockEEE
0060c59c: ldr      r3, [r2]
0060c5a0: push     {r4, r5, r6, r7, r8, lr}
0060c5a4: cmp      r3, #0
0060c5a8: mov      r4, r2
0060c5ac: mov      r6, r0
0060c5b0: mov      r5, r1
0060c5b4: beq      #0x60c5e4
0060c5b8: mov      r0, r3
0060c5bc: bl       #0x60b350
0060c5c0: subs     r7, r0, #0
0060c5c4: beq      #0x60c5e4
0060c5c8: bl       #0x60c598
0060c5cc: ldr      r0, [r4]
0060c5d0: str      r7, [r4]
0060c5d4: cmp      r0, #0
0060c5d8: beq      #0x60c60c
0060c5dc: pop      {r4, r5, r6, r7, r8, lr}
0060c5e0: b        #0x60bbd4
0060c5e4: mov      r1, r5
0060c5e8: mov      r0, r6
0060c5ec: bl       #0x60c220
0060c5f0: subs     r5, r0, #0
0060c5f4: beq      #0x60c60c
0060c5f8: bl       #0x60c598
0060c5fc: ldr      r0, [r4]
0060c600: str      r5, [r4]
0060c604: cmp      r0, #0
0060c608: bne      #0x60c5dc
0060c60c: pop      {r4, r5, r6, r7, r8, pc}
