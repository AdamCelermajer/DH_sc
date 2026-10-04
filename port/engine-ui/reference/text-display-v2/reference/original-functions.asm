# _ZN7gameswfL21display_glyph_recordsEPKNS_6matrixEPNS_9characterERNS_5arrayINS_17text_glyph_recordEEEPNS_20movie_definition_subEPNS_4rgbaEhhh
0078faa0: ldr      ip, [pc, #0xe60]
0078faa4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0078faa8: mov      r6, r0
0078faac: ldr      r0, [pc, #0xe58]
0078fab0: add      ip, pc, ip
0078fab4: sub      sp, sp, #0x204
0078fab8: ldr      r0, [ip, r0]
0078fabc: mov      r4, r1
0078fac0: str      r2, [sp, #0x44]
0078fac4: ldr      sl, [r0]
0078fac8: ldrb     r1, [sp, #0x230]
0078facc: ldrb     r0, [sp, #0x22c]
0078fad0: ldrb     r2, [sp, #0x234]
0078fad4: cmp      sl, #0
0078fad8: str      ip, [sp, #0x6c]
0078fadc: str      r3, [sp, #0x60]
0078fae0: str      r0, [sp, #0x64]
0078fae4: str      r1, [sp, #0x58]
0078fae8: str      r2, [sp, #0x5c]
0078faec: beq      #0x790424
0078faf0: ldr      r3, [r4, #0x30]
0078faf4: cmp      r3, #0
0078faf8: beq      #0x78fb0c
0078fafc: ldr      r0, [r4, #0x2c]
0078fb00: ldrb     r2, [r0, #4]
0078fb04: cmp      r2, #0
0078fb08: beq      #0x7908e0
0078fb0c: ldr      r5, [r3, #0xac]
0078fb10: add      r3, sp, #0x110
0078fb14: str      r3, [sp, #0x80]
0078fb18: ldr      r2, [r5, #0xc]
0078fb1c: add      ip, sp, #0xa4
0078fb20: ldr      r3, [r5, #0x10]
0078fb24: str      ip, [sp, #0x84]
0078fb28: ldr      r2, [r2, #0x28]
0078fb2c: ldr      lr, [sp, #0x58]
0078fb30: ldr      r1, [sp, #0x5c]
0078fb34: str      r2, [sp, #0x3c]
0078fb38: ldr      r3, [r3, #0xc]
0078fb3c: mov      r7, #0
0078fb40: ldr      r0, [sp, #0x80]
0078fb44: str      r3, [sp, #0x48]
0078fb48: strb     lr, [sp, #0x1f9]
0078fb4c: strb     r1, [sp, #0x1fa]
0078fb50: strb     r7, [sp, #0x1fc]
0078fb54: strb     r7, [sp, #0x1fd]
0078fb58: strb     r7, [sp, #0x1fe]
0078fb5c: strb     r7, [sp, #0x1f8]
0078fb60: bl       #0x784a8c
0078fb64: ldr      r0, [sp, #0x84]
0078fb68: bl       #0x784b20
0078fb6c: ldr      ip, [sp, #0x80]
0078fb70: ldr      lr, [sp, #0x84]
0078fb74: mov      r3, #1
0078fb78: add      r2, sp, #0x1e4
0078fb7c: mov      r1, r3
0078fb80: mov      r0, r2
0078fb84: str      r2, [sp, #0x70]
0078fb88: str      ip, [sp, #0x1e4]
0078fb8c: str      r3, [sp, #0x1ec]
0078fb90: strb     r3, [sp, #0x1f0]
0078fb94: str      lr, [sp, #0x1d4]
0078fb98: str      r3, [sp, #0x1dc]
0078fb9c: strb     r3, [sp, #0x1e0]
0078fba0: str      r7, [sp, #0x1d8]
0078fba4: str      r7, [sp, #0x1e8]
0078fba8: bl       #0x76146c
0078fbac: mov      r0, r4
0078fbb0: bl       #0x753f74
0078fbb4: add      r1, sp, #0x17c
0078fbb8: str      r1, [sp, #0x10]
0078fbbc: ldr      lr, [sp, #0x10]
0078fbc0: mov      ip, r0
0078fbc4: ldm      ip!, {r0, r1, r2, r3}
0078fbc8: stm      lr!, {r0, r1, r2, r3}
0078fbcc: ldm      ip, {r0, r1}
0078fbd0: cmp      r6, r7
0078fbd4: stm      lr, {r0, r1}
0078fbd8: beq      #0x78fbe8
0078fbdc: mov      r1, r6
0078fbe0: ldr      r0, [sp, #0x10]
0078fbe4: bl       #0x4165b8
0078fbe8: add      r1, sp, #0x164
0078fbec: mov      r0, r4
0078fbf0: str      r1, [sp, #0x14]
0078fbf4: bl       #0x753ec0
0078fbf8: str      r0, [sp, #0x54]
0078fbfc: ldr      r3, [r4]
0078fc00: mov      r0, r4
0078fc04: mov      lr, pc
0078fc08: ldr      pc, [r3, #0x78]
0078fc0c: ldr      ip, [sp, #0x10]
0078fc10: ldr      lr, [sp, #0x14]
0078fc14: str      r0, [sp, #0x7c]
0078fc18: ldm      ip!, {r0, r1, r2, r3}
0078fc1c: stm      lr!, {r0, r1, r2, r3}
0078fc20: ldm      ip, {r0, r1}
0078fc24: stm      lr, {r0, r1}
0078fc28: ldr      r7, [sp, #0x164]
0078fc2c: ldr      r6, [sp, #0x168]
0078fc30: ldr      r8, [sp, #0x170]
0078fc34: mov      r1, r7
0078fc38: mov      r0, r7
0078fc3c: bl       #0x30ed6c
0078fc40: mov      r1, r6
0078fc44: mov      r4, r0
0078fc48: mov      r0, r6
0078fc4c: bl       #0x30ed6c
0078fc50: mov      r1, r0
0078fc54: mov      r0, r4
0078fc58: bl       #0x30eba4
0078fc5c: mov      r1, r8
0078fc60: mov      fp, r0
0078fc64: mov      r0, r8
0078fc68: bl       #0x30ed6c
0078fc6c: ldr      sb, [sp, #0x174]
0078fc70: mov      r4, r0
0078fc74: mov      r1, sb
0078fc78: mov      r0, sb
0078fc7c: bl       #0x30ed6c
0078fc80: mov      r1, r0
0078fc84: mov      r0, r4
0078fc88: bl       #0x30eba4
0078fc8c: mov      r4, r0
0078fc90: mov      r1, r4
0078fc94: mov      r0, fp
0078fc98: bl       #0x30e70c
0078fc9c: cmp      r0, #0
0078fca0: moveq    r4, fp
0078fca4: mov      r0, r4
0078fca8: mov      r1, #0
0078fcac: bl       #0x30e70c
0078fcb0: cmp      r0, #0
0078fcb4: beq      #0x78fcc0
0078fcb8: mov      r0, r4
0078fcbc: bl       #0x30e124
0078fcc0: mov      r0, r6
0078fcc4: mov      r1, #0
0078fcc8: bl       #0x30df8c
0078fccc: cmp      r0, #0
0078fcd0: moveq    r4, #1
0078fcd4: beq      #0x78fcf4
0078fcd8: mov      r0, r8
0078fcdc: mov      r1, #0
0078fce0: bl       #0x30df8c
0078fce4: cmp      r0, #0
0078fce8: mov      r4, #0
0078fcec: moveq    r4, #1
0078fcf0: uxtb     r4, r4
0078fcf4: mov      r0, r7
0078fcf8: mov      r1, #0x3f800000
0078fcfc: bl       #0x30df8c
0078fd00: cmp      r0, #0
0078fd04: moveq    r2, #1
0078fd08: bne      #0x7908c0
0078fd0c: ldr      r3, [r5, #0xc]
0078fd10: cmp      r4, #0
0078fd14: movne    r2, #1
0078fd18: str      r2, [sp, #0x1c]
0078fd1c: ldr      r4, [r3, #4]
0078fd20: mov      r1, #0x44000000
0078fd24: add      r1, r1, #0x800000
0078fd28: mov      r0, r4
0078fd2c: bl       #0x30ed6c
0078fd30: str      r0, [sp, #0x68]
0078fd34: ldr      r0, [sp, #0x58]
0078fd38: bl       #0x30e964
0078fd3c: mov      r1, r0
0078fd40: bl       #0x30eba4
0078fd44: mov      r1, r4
0078fd48: bl       #0x30ed6c
0078fd4c: str      r0, [sp, #0x94]
0078fd50: ldr      r0, [sp, #0x5c]
0078fd54: bl       #0x30e964
0078fd58: mov      r1, r0
0078fd5c: bl       #0x30eba4
0078fd60: mov      r1, r4
0078fd64: bl       #0x30ed6c
0078fd68: str      r0, [sp, #0x90]
0078fd6c: ldr      r0, [sp, #0x64]
0078fd70: bl       #0x30e964
0078fd74: mov      r1, r0
0078fd78: bl       #0x30eba4
0078fd7c: mov      r1, r4
0078fd80: bl       #0x30ed6c
0078fd84: str      r0, [sp, #0x8c]
0078fd88: ldr      r0, [sp, #0x44]
0078fd8c: ldr      r3, [r0, #4]
0078fd90: cmp      r3, #0
0078fd94: addle    r1, sp, #0x1d4
0078fd98: strle    r1, [sp, #0x74]
0078fd9c: ble      #0x7903bc
0078fda0: add      r3, sp, #0x1a4
0078fda4: str      r3, [sp, #0x2c]
0078fda8: ldr      r0, [sp, #0x2c]
0078fdac: ldr      r3, [pc, #0xb5c]
0078fdb0: mov      r2, #0
0078fdb4: mov      ip, #0
0078fdb8: add      r3, pc, r3
0078fdbc: add      lr, sp, #0x1d4
0078fdc0: add      r0, r0, #8
0078fdc4: str      r2, [sp, #0x18]
0078fdc8: str      ip, [sp, #0x34]
0078fdcc: str      r3, [sp, #0x78]
0078fdd0: mov      r8, r2
0078fdd4: str      lr, [sp, #0x74]
0078fdd8: str      r0, [sp, #0x4c]
0078fddc: ldr      r1, [sp, #0x44]
0078fde0: ldr      r2, [sp, #0x34]
0078fde4: mov      r5, #0x30
0078fde8: ldr      r3, [r1]
0078fdec: ldr      r1, [sp, #0x60]
0078fdf0: mla      r5, r5, r2, r3
0078fdf4: mov      r0, r5
0078fdf8: bl       #0x78ae50
0078fdfc: ldr      r3, [r5, #4]
0078fe00: cmp      r3, #0
0078fe04: str      r3, [sp, #0x28]
0078fe08: beq      #0x790718
0078fe0c: ldr      r0, [r5, #0x18]
0078fe10: ldr      r1, [sp, #0x68]
0078fe14: bl       #0x30ec94
0078fe18: ldr      ip, [sp, #0x28]
0078fe1c: mov      sb, r0
0078fe20: ldr      r3, [ip, #0x7c]
0078fe24: cmp      r3, #0
0078fe28: beq      #0x78fe3c
0078fe2c: mov      r1, #0x41000000
0078fe30: add      r1, r1, #0xa00000
0078fe34: bl       #0x30ec94
0078fe38: mov      sb, r0
0078fe3c: ldrb     r3, [r5, #0x1c]
0078fe40: cmp      r3, #0
0078fe44: ldrb     r3, [r5, #0x1d]
0078fe48: ldrne    r8, [r5, #0x10]
0078fe4c: cmp      r3, #0
0078fe50: ldrne    lr, [r5, #0x14]
0078fe54: ldr      r3, [sp, #0x1e4]
0078fe58: strne    lr, [sp, #0x18]
0078fe5c: ldrb     r2, [r5, #9]
0078fe60: ldrb     r0, [r5, #0xa]
0078fe64: ldrb     r1, [r5, #0xb]
0078fe68: ldrb     ip, [r5, #8]
0078fe6c: strb     r2, [r3, #9]
0078fe70: strb     r0, [r3, #0xa]
0078fe74: strb     ip, [r3, #8]
0078fe78: strb     r1, [r3, #0xb]
0078fe7c: ldr      r1, [r5, #8]
0078fe80: ldr      r0, [sp, #0x54]
0078fe84: bl       #0x794f8c
0078fe88: ubfx     r3, r0, #0x18, #8
0078fe8c: ubfx     r1, r0, #8, #8
0078fe90: ubfx     r2, r0, #0x10, #8
0078fe94: strb     r0, [sp, #0x98]
0078fe98: strb     r1, [sp, #0x99]
0078fe9c: strb     r2, [sp, #0x9a]
0078fea0: strb     r3, [sp, #0x9b]
0078fea4: ldr      r0, [sp, #0x228]
0078fea8: ldr      r3, [sp, #0x98]
0078feac: cmp      r0, #0
0078feb0: str      r3, [sp, #0x1f4]
0078feb4: beq      #0x78fefc
0078feb8: ldrb     r3, [r0, #3]
0078febc: ldrb     r2, [sp, #0x1f7]
0078fec0: movw     ip, #0x8081
0078fec4: movt     ip, #0x8080
0078fec8: mul      r3, r2, r3
0078fecc: ldr      lr, [sp, #0x228]
0078fed0: smull    r2, ip, ip, r3
0078fed4: mov      r1, r0
0078fed8: ldrb     r1, [r1, #1]
0078fedc: ldrb     r0, [r0]
0078fee0: ldrb     r2, [lr, #2]
0078fee4: add      r3, ip, r3
0078fee8: asr      r3, r3, #7
0078feec: strb     r3, [sp, #0x1f7]
0078fef0: strb     r0, [sp, #0x1f4]
0078fef4: strb     r1, [sp, #0x1f5]
0078fef8: strb     r2, [sp, #0x1f6]
0078fefc: ldr      r3, [r5, #0x24]
0078ff00: cmp      r3, #0
0078ff04: ble      #0x7903a0
0078ff08: ldr      r0, [sp, #0x5c]
0078ff0c: ldr      r1, [sp, #0x58]
0078ff10: mov      r6, #0
0078ff14: ldr      r4, [r5, #0x20]
0078ff18: orr      r0, r0, r1
0078ff1c: mov      r7, r6
0078ff20: str      r0, [sp, #0x50]
0078ff24: ldr      ip, [sp, #0x14]
0078ff28: ldr      lr, [sp, #0x10]
0078ff2c: add      r4, r4, r6
0078ff30: ldm      ip!, {r0, r1, r2, r3}
0078ff34: stm      lr!, {r0, r1, r2, r3}
0078ff38: ldr      r2, [sp, #0x1c]
0078ff3c: ldm      ip, {r0, r1}
0078ff40: cmp      r2, #0
0078ff44: stm      lr, {r0, r1}
0078ff48: bne      #0x79044c
0078ff4c: ldr      r1, [sp, #0x184]
0078ff50: mov      r0, r8
0078ff54: bl       #0x30eba4
0078ff58: mvn      r1, #0x800000
0078ff5c: mov      fp, r0
0078ff60: bl       #0x30e4b4
0078ff64: cmp      r0, #0
0078ff68: beq      #0x790444
0078ff6c: mvn      r1, #0x80000000
0078ff70: mov      r0, fp
0078ff74: sub      r1, r1, #0x800000
0078ff78: bl       #0x30e9ac
0078ff7c: cmp      r0, #0
0078ff80: beq      #0x790444
0078ff84: ldr      r1, [sp, #0x190]
0078ff88: ldr      r0, [sp, #0x18]
0078ff8c: str      fp, [sp, #0x184]
0078ff90: bl       #0x30eba4
0078ff94: mvn      r1, #0x800000
0078ff98: mov      fp, r0
0078ff9c: bl       #0x30e4b4
0078ffa0: cmp      r0, #0
0078ffa4: beq      #0x79043c
0078ffa8: mvn      r1, #0x80000000
0078ffac: mov      r0, fp
0078ffb0: sub      r1, r1, #0x800000
0078ffb4: bl       #0x30e9ac
0078ffb8: cmp      r0, #0
0078ffbc: beq      #0x79043c
0078ffc0: ldr      r1, [sp, #0x17c]
0078ffc4: mov      r0, sb
0078ffc8: str      fp, [sp, #0x190]
0078ffcc: bl       #0x30ed6c
0078ffd0: mvn      r1, #0x800000
0078ffd4: mov      fp, r0
0078ffd8: bl       #0x30e4b4
0078ffdc: cmp      r0, #0
0078ffe0: beq      #0x790434
0078ffe4: mvn      r1, #0x80000000
0078ffe8: mov      r0, fp
0078ffec: sub      r1, r1, #0x800000
0078fff0: bl       #0x30e9ac
0078fff4: cmp      r0, #0
0078fff8: beq      #0x790434
0078fffc: ldr      r1, [sp, #0x18c]
00790000: mov      r0, sb
00790004: str      fp, [sp, #0x17c]
00790008: bl       #0x30ed6c
0079000c: mvn      r1, #0x800000
00790010: mov      fp, r0
00790014: bl       #0x30e4b4
00790018: cmp      r0, #0
0079001c: beq      #0x79042c
00790020: mvn      r1, #0x80000000
00790024: mov      r0, fp
00790028: sub      r1, r1, #0x800000
0079002c: bl       #0x30e9ac
00790030: cmp      r0, #0
00790034: beq      #0x79042c
00790038: str      fp, [sp, #0x18c]
0079003c: ldrsh    r3, [r4, #0x1e]
00790040: cmn      r3, #1
00790044: beq      #0x790524
00790048: ldrb     r3, [r5, #0xc]
0079004c: cmp      r3, #0
00790050: bne      #0x79057c
00790054: ldr      r3, [r4, #4]
00790058: cmp      r3, #0
0079005c: beq      #0x790600
00790060: ldrb     r3, [r4, #0x22]
00790064: cmp      r3, #2
00790068: beq      #0x790644
0079006c: ldr      fp, [r4, #0xc]
00790070: mov      r1, #0
00790074: mov      r0, fp
00790078: bl       #0x30e2f8
0079007c: cmp      r0, #0
00790080: beq      #0x790378
00790084: ldr      r0, [r4, #0x14]
00790088: mov      r1, #0
0079008c: bl       #0x30e2f8
00790090: cmp      r0, #0
00790094: beq      #0x790378
00790098: ldr      r1, [r4, #8]
0079009c: mov      r0, fp
007900a0: bl       #0x30e3ac
007900a4: str      r0, [sp, #0x1c8]
007900a8: ldr      r1, [r4, #0x10]
007900ac: ldr      r0, [r4, #0x14]
007900b0: bl       #0x30e3ac
007900b4: str      r0, [sp, #0x1d0]
007900b8: ldr      r3, [r4, #8]
007900bc: add      r3, r3, #0x80000000
007900c0: str      r3, [sp, #0x1c4]
007900c4: ldr      r3, [r4, #0x10]
007900c8: add      r3, r3, #0x80000000
007900cc: str      r3, [sp, #0x1cc]
007900d0: ldrh     r3, [r4, #0x1c]
007900d4: mov      r0, r3
007900d8: str      r3, [sp, #8]
007900dc: bl       #0x30e964
007900e0: mov      r1, r0
007900e4: mov      r0, #0x44000000
007900e8: add      r0, r0, #0x800000
007900ec: bl       #0x30ec94
007900f0: ldr      r1, [sp, #0x48]
007900f4: ldr      r2, [sp, #0x3c]
007900f8: str      r0, [sp, #0x20]
007900fc: ldr      r3, [sp, #8]
00790100: cmp      r2, #0
00790104: cmpeq    r1, #0
00790108: beq      #0x7906a8
0079010c: ldr      fp, [r4, #4]
00790110: ldr      r2, [r2, #0x34]
00790114: cmp      fp, r2
00790118: beq      #0x790738
0079011c: add      r0, sp, #0x1b4
00790120: ldrh     r1, [r4, #0x20]
00790124: ldr      r2, [r4, #0x18]
00790128: str      r0, [sp, #0x38]
0079012c: ldr      ip, [sp, #0x38]
00790130: ldr      r0, [sp, #0x48]
00790134: str      ip, [sp]
00790138: bl       #0x7c55d8
0079013c: ldr      lr, [sp, #0x1b8]
00790140: ldr      r1, [sp, #0x1b4]
00790144: mov      r0, lr
00790148: str      lr, [sp, #0x24]
0079014c: bl       #0x30e3ac
00790150: str      r0, [sp, #0x40]
00790154: ldr      r1, [sp, #0x40]
00790158: ldr      r0, [sp, #0x20]
0079015c: bl       #0x30ed6c
00790160: ldr      r1, [sp, #0x1bc]
00790164: str      r0, [sp, #0x30]
00790168: ldr      r0, [sp, #0x1c0]
0079016c: bl       #0x30e3ac
00790170: mov      r1, r0
00790174: ldr      r0, [sp, #0x20]
00790178: bl       #0x30ed6c
0079017c: str      r0, [sp, #0x20]
00790180: ldr      r0, [sp, #0x50]
00790184: cmp      r0, #0
00790188: beq      #0x79019c
0079018c: ldr      r1, [sp, #0x3c]
00790190: ldr      r3, [r1, #0x34]
00790194: cmp      fp, r3
00790198: beq      #0x790760
0079019c: ldr      r0, [sp, #0x64]
007901a0: cmp      r0, #0
007901a4: beq      #0x7901b8
007901a8: ldr      ip, [sp, #0x3c]
007901ac: ldr      r3, [ip, #0x34]
007901b0: cmp      fp, r3
007901b4: beq      #0x79091c
007901b8: ldr      r1, [r4, #0xc]
007901bc: mov      r0, #0x3f800000
007901c0: bl       #0x30e3ac
007901c4: ldr      r1, [sp, #0x40]
007901c8: bl       #0x30ed6c
007901cc: mov      r1, r0
007901d0: ldr      r0, [sp, #0x24]
007901d4: bl       #0x30e3ac
007901d8: str      r0, [sp, #0x1b8]
007901dc: ldr      r1, [r4, #0x14]
007901e0: mov      r0, #0x3f800000
007901e4: bl       #0x30e3ac
007901e8: ldr      r1, [sp, #0x1bc]
007901ec: mov      r3, r0
007901f0: ldr      r0, [sp, #0x1c0]
007901f4: str      r3, [sp, #8]
007901f8: bl       #0x30e3ac
007901fc: ldr      r3, [sp, #8]
00790200: mov      r1, r0
00790204: mov      r0, r3
00790208: bl       #0x30ed6c
0079020c: mov      r1, r0
00790210: ldr      r0, [sp, #0x1c0]
00790214: bl       #0x30e3ac
00790218: ldr      r1, [sp, #0x1b4]
0079021c: str      r0, [sp, #0x1c0]
00790220: str      r1, [sp, #0x24]
00790224: ldr      r3, [fp]
00790228: mov      r0, fp
0079022c: mov      lr, pc
00790230: ldr      pc, [r3, #0x24]
00790234: bl       #0x30e964
00790238: mov      r1, r0
0079023c: ldr      r0, [sp, #0x24]
00790240: bl       #0x30ec94
00790244: ldr      r3, [sp, #0x1bc]
00790248: str      r0, [sp, #0x1b4]
0079024c: ldr      r2, [fp]
00790250: mov      r0, fp
00790254: str      r3, [sp, #8]
00790258: mov      lr, pc
0079025c: ldr      pc, [r2, #0x28]
00790260: bl       #0x30e964
00790264: ldr      r3, [sp, #8]
00790268: mov      r1, r0
0079026c: mov      r0, r3
00790270: bl       #0x30ec94
00790274: ldr      r3, [sp, #0x1b8]
00790278: str      r0, [sp, #0x1bc]
0079027c: ldr      r2, [fp]
00790280: mov      r0, fp
00790284: str      r3, [sp, #8]
00790288: mov      lr, pc
0079028c: ldr      pc, [r2, #0x24]
00790290: bl       #0x30e964
00790294: ldr      r3, [sp, #8]
00790298: mov      r1, r0
0079029c: mov      r0, r3
007902a0: bl       #0x30ec94
007902a4: str      r0, [sp, #0x1b8]
007902a8: ldr      r3, [fp]
007902ac: mov      r0, fp
007902b0: ldr      fp, [sp, #0x1c0]
007902b4: mov      lr, pc
007902b8: ldr      pc, [r3, #0x28]
007902bc: bl       #0x30e964
007902c0: mov      r1, r0
007902c4: mov      r0, fp
007902c8: bl       #0x30ec94
007902cc: str      r0, [sp, #0x1c0]
007902d0: ldr      lr, [sp, #0x28]
007902d4: cmp      lr, #0
007902d8: beq      #0x790310
007902dc: ldr      r3, [lr, #0x7c]
007902e0: cmp      r3, #0
007902e4: beq      #0x790310
007902e8: mov      r1, #0x41000000
007902ec: ldr      r0, [sp, #0x30]
007902f0: add      r1, r1, #0xa00000
007902f4: bl       #0x30ed6c
007902f8: mov      r1, #0x41000000
007902fc: str      r0, [sp, #0x30]
00790300: add      r1, r1, #0xa00000
00790304: ldr      r0, [sp, #0x20]
00790308: bl       #0x30ed6c
0079030c: str      r0, [sp, #0x20]
00790310: ldr      r0, [sp, #0x1c4]
00790314: ldr      r1, [sp, #0x30]
00790318: bl       #0x30ed6c
0079031c: ldr      r1, [sp, #0x30]
00790320: str      r0, [sp, #0x1c4]
00790324: ldr      r0, [sp, #0x1c8]
00790328: bl       #0x30ed6c
0079032c: ldr      r1, [sp, #0x20]
00790330: str      r0, [sp, #0x1c8]
00790334: ldr      r0, [sp, #0x1cc]
00790338: bl       #0x30ed6c
0079033c: ldr      r1, [sp, #0x20]
00790340: str      r0, [sp, #0x1cc]
00790344: ldr      r0, [sp, #0x1d0]
00790348: bl       #0x30ed6c
0079034c: ldr      r3, [sp, #0x1f4]
00790350: str      r0, [sp, #0x1d0]
00790354: ldr      r0, [sp, #0x38]
00790358: ldr      r2, [r4, #4]
0079035c: ldr      ip, [sl]
00790360: ldr      r1, [sp, #0x10]
00790364: stm      sp, {r0, r3}
00790368: mov      r0, sl
0079036c: add      r3, sp, #0x1c4
00790370: mov      lr, pc
00790374: ldr      pc, [ip, #0x80]
00790378: ldr      r4, [r5, #0x20]
0079037c: mov      r0, r8
00790380: add      r7, r7, #1
00790384: ldr      r1, [r4, r6]
00790388: bl       #0x30eba4
0079038c: ldr      r3, [r5, #0x24]
00790390: mov      r8, r0
00790394: add      r6, r6, #0x24
00790398: cmp      r7, r3
0079039c: blt      #0x78ff24
007903a0: ldr      r0, [sp, #0x44]
007903a4: ldr      r1, [sp, #0x34]
007903a8: ldr      r3, [r0, #4]
007903ac: add      r1, r1, #1
007903b0: str      r1, [sp, #0x34]
007903b4: cmp      r1, r3
007903b8: blt      #0x78fddc
007903bc: ldr      r0, [sp, #0x74]
007903c0: bl       #0x78b8d0
007903c4: ldr      r0, [sp, #0x74]
007903c8: mov      r1, #0
007903cc: bl       #0x76150c
007903d0: ldr      r0, [sp, #0x70]
007903d4: mov      r1, #0
007903d8: bl       #0x76146c
007903dc: ldr      r0, [sp, #0x70]
007903e0: mov      r1, #0
007903e4: bl       #0x7613e4
007903e8: ldr      ip, [sp, #0x6c]
007903ec: ldr      r3, [pc, #0x520]
007903f0: ldr      r2, [sp, #0x84]
007903f4: ldr      r3, [ip, r3]
007903f8: add      r0, r2, #0xc
007903fc: add      r3, r3, #8
00790400: str      r3, [sp, #0xa4]
00790404: bl       #0x784cac
00790408: ldr      r3, [pc, #0x508]
0079040c: ldr      lr, [sp, #0x6c]
00790410: ldr      r0, [sp, #0x80]
00790414: ldr      r3, [lr, r3]
00790418: add      r3, r3, #8
0079041c: str      r3, [sp, #0xa4]
00790420: bl       #0x784cac
00790424: add      sp, sp, #0x204
00790428: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0079042c: mov      fp, #0
00790430: b        #0x790038
00790434: mov      fp, #0
00790438: b        #0x78fffc
0079043c: mov      fp, #0
00790440: b        #0x78ffc0
00790444: mov      fp, #0
00790448: b        #0x78ff84
0079044c: ldr      r1, [sp, #0x17c]
00790450: mov      r0, r8
00790454: bl       #0x30ed6c
00790458: ldr      r1, [sp, #0x180]
0079045c: mov      fp, r0
00790460: ldr      r0, [sp, #0x18]
00790464: bl       #0x30ed6c
00790468: mov      r1, r0
0079046c: mov      r0, fp
00790470: bl       #0x30eba4
00790474: ldr      r1, [sp, #0x184]
00790478: bl       #0x30eba4
0079047c: mvn      r1, #0x800000
00790480: mov      fp, r0
00790484: bl       #0x30e4b4
00790488: cmp      r0, #0
0079048c: beq      #0x790574
00790490: mvn      r1, #0x80000000
00790494: mov      r0, fp
00790498: sub      r1, r1, #0x800000
0079049c: bl       #0x30e9ac
007904a0: cmp      r0, #0
007904a4: beq      #0x790574
007904a8: ldr      r1, [sp, #0x188]
007904ac: mov      r0, r8
007904b0: str      fp, [sp, #0x184]
007904b4: bl       #0x30ed6c
007904b8: ldr      r1, [sp, #0x18c]
007904bc: mov      fp, r0
007904c0: ldr      r0, [sp, #0x18]
007904c4: bl       #0x30ed6c
007904c8: mov      r1, r0
007904cc: mov      r0, fp
007904d0: bl       #0x30eba4
007904d4: ldr      r1, [sp, #0x190]
007904d8: bl       #0x30eba4
007904dc: mvn      r1, #0x800000
007904e0: mov      fp, r0
007904e4: bl       #0x30e4b4
007904e8: cmp      r0, #0
007904ec: beq      #0x79063c
007904f0: mvn      r1, #0x80000000
007904f4: mov      r0, fp
007904f8: sub      r1, r1, #0x800000
007904fc: bl       #0x30e9ac
00790500: cmp      r0, #0
00790504: beq      #0x79063c
00790508: ldr      r0, [sp, #0x10]
0079050c: mov      r1, sb
00790510: str      fp, [sp, #0x190]
00790514: bl       #0x76128c
00790518: ldrsh    r3, [r4, #0x1e]
0079051c: cmn      r3, #1
00790520: bne      #0x790048
00790524: ldr      r3, [r4, #4]
00790528: cmp      r3, #0
0079052c: bne      #0x790048
00790530: mov      r0, sl
00790534: ldr      r1, [sp, #0x10]
00790538: ldr      r3, [sl]
0079053c: mov      lr, pc
00790540: ldr      pc, [r3, #0x50]
00790544: mov      r0, sl
00790548: ldr      r1, [sp, #0x1f4]
0079054c: ldr      r3, [sl]
00790550: mov      lr, pc
00790554: ldr      pc, [r3, #0x78]
00790558: mov      r0, sl
0079055c: ldr      r1, [sp, #0x78]
00790560: mov      r2, #5
00790564: ldr      r3, [sl]
00790568: mov      lr, pc
0079056c: ldr      pc, [r3, #0x60]
00790570: b        #0x790378
00790574: mov      fp, #0
00790578: b        #0x7904a8
0079057c: mov      r0, sl
00790580: ldr      r1, [sp, #0x10]
00790584: ldr      r3, [sl]
00790588: mov      lr, pc
0079058c: ldr      pc, [r3, #0x50]
00790590: mov      r0, sl
00790594: ldr      r1, [sp, #0x1f4]
00790598: ldr      r3, [sl]
0079059c: mov      lr, pc
007905a0: ldr      pc, [r3, #0x78]
007905a4: ldr      ip, [sp, #0x4c]
007905a8: ldr      lr, [sp, #0x2c]
007905ac: mov      r3, #0x42000000
007905b0: mov      r2, #0
007905b4: add      r3, r3, #0x700000
007905b8: str      r2, [ip]
007905bc: str      r2, [lr]
007905c0: str      r3, [sp, #0x1b0]
007905c4: str      r3, [sp, #0x1a8]
007905c8: ldr      r3, [r5, #0x20]
007905cc: mov      r1, sb
007905d0: ldr      r0, [r3, r6]
007905d4: bl       #0x30ec94
007905d8: str      r0, [sp, #0x1ac]
007905dc: ldr      r3, [sl]
007905e0: mov      r0, sl
007905e4: ldr      r1, [sp, #0x2c]
007905e8: mov      r2, #2
007905ec: mov      lr, pc
007905f0: ldr      pc, [r3, #0x60]
007905f4: ldr      r3, [r4, #4]
007905f8: cmp      r3, #0
007905fc: bne      #0x790060
00790600: ldrsh    r1, [r4, #0x1e]
00790604: cmp      r1, #0
00790608: blt      #0x790378
0079060c: ldr      r0, [sp, #0x28]
00790610: bl       #0x7ce3b8
00790614: cmp      r0, #0
00790618: beq      #0x790378
0079061c: ldr      ip, [sp, #0x70]
00790620: ldr      lr, [sp, #0x74]
00790624: ldr      r1, [sp, #0x10]
00790628: ldr      r2, [sp, #0x54]
0079062c: ldr      r3, [sp, #0x7c]
00790630: stm      sp, {ip, lr}
00790634: bl       #0x77a990
00790638: b        #0x790378
0079063c: mov      fp, #0
00790640: b        #0x790508
00790644: mov      r0, #0
00790648: str      r0, [sp, #0x1a4]
0079064c: ldr      r2, [r4, #0x14]
00790650: mov      r3, #0x3f800000
00790654: ldr      r1, [sp, #0x10]
00790658: add      r2, r2, #0x80000000
0079065c: str      r2, [sp, #0x1ac]
00790660: ldr      r2, [r4, #0xc]
00790664: str      r3, [sp, #0x1d0]
00790668: str      r0, [sp, #0x1b0]
0079066c: str      r0, [sp, #0x1c4]
00790670: str      r0, [sp, #0x1cc]
00790674: str      r3, [sp, #0x1c8]
00790678: str      r2, [sp, #0x1a8]
0079067c: add      r3, sp, #0x1c4
00790680: ldr      r2, [r4, #4]
00790684: ldr      ip, [sl]
00790688: str      r3, [sp]
0079068c: ldr      r3, [sp, #0x1f4]
00790690: mov      r0, sl
00790694: str      r3, [sp, #4]
00790698: ldr      r3, [sp, #0x2c]
0079069c: mov      lr, pc
007906a0: ldr      pc, [ip, #0x80]
007906a4: b        #0x790378
007906a8: mov      r3, #0
007906ac: str      r3, [sp, #0x1b4]
007906b0: str      r3, [sp, #0x1bc]
007906b4: ldr      r3, [r4, #0xc]
007906b8: add      ip, sp, #0x1b4
007906bc: str      ip, [sp, #0x38]
007906c0: str      r3, [sp, #0x1b8]
007906c4: ldr      r3, [r4, #0x14]
007906c8: str      r3, [sp, #0x1c0]
007906cc: ldr      r3, [r4, #4]
007906d0: mov      r0, r3
007906d4: ldr      r3, [r3]
007906d8: mov      lr, pc
007906dc: ldr      pc, [r3, #0x24]
007906e0: bl       #0x30e964
007906e4: ldr      r1, [sp, #0x20]
007906e8: bl       #0x30ed6c
007906ec: str      r0, [sp, #0x30]
007906f0: ldr      r3, [r4, #4]
007906f4: mov      r0, r3
007906f8: ldr      r3, [r3]
007906fc: mov      lr, pc
00790700: ldr      pc, [r3, #0x28]
00790704: bl       #0x30e964
00790708: ldr      r1, [sp, #0x20]
0079070c: bl       #0x30ed6c
00790710: str      r0, [sp, #0x20]
00790714: b        #0x7902d0
00790718: ldrb     r3, [r5, #0x1e]
0079071c: cmp      r3, #0
00790720: bne      #0x7903a0
00790724: ldr      r0, [r5, #0x18]
00790728: ldr      r1, [sp, #0x68]
0079072c: bl       #0x30ec94
00790730: mov      sb, r0
00790734: b        #0x78fe3c
00790738: add      ip, sp, #0x1b4
0079073c: ldrh     r1, [r4, #0x20]
00790740: ldr      r2, [r4, #0x18]
00790744: str      ip, [sp, #0x38]
00790748: ldr      lr, [sp, #0x38]
0079074c: add      ip, sp, #0x1fc
00790750: ldr      r0, [sp, #0x3c]
00790754: stm      sp, {ip, lr}
00790758: bl       #0x7d266c
0079075c: b        #0x79013c
00790760: add      ip, sp, #0x1f8
00790764: ldr      r2, [r4, #0x18]
00790768: ldrh     r3, [r4, #0x1c]
0079076c: ldrh     r1, [r4, #0x20]
00790770: str      ip, [sp]
00790774: ldr      ip, [sp, #0x2c]
00790778: ldr      r0, [sp, #0x3c]
0079077c: str      ip, [sp, #4]
00790780: bl       #0x7d266c
00790784: ldr      r1, [sp, #0x1b4]
00790788: ldr      r0, [sp, #0x1b8]
0079078c: bl       #0x30e3ac
00790790: ldr      r1, [sp, #0x1bc]
00790794: mov      r3, r0
00790798: ldr      r0, [sp, #0x1c0]
0079079c: str      r3, [sp, #8]
007907a0: bl       #0x30e3ac
007907a4: ldr      lr, [sp, #0x1a4]
007907a8: ldr      r3, [sp, #8]
007907ac: mov      ip, r0
007907b0: mov      r1, lr
007907b4: mov      r0, r3
007907b8: str      lr, [sp, #0x24]
007907bc: str      lr, [sp, #0x1b4]
007907c0: str      ip, [sp, #0xc]
007907c4: bl       #0x30eba4
007907c8: ldr      ip, [sp, #0xc]
007907cc: ldr      r2, [sp, #0x1ac]
007907d0: mov      r3, r0
007907d4: mov      r0, ip
007907d8: mov      r1, r2
007907dc: str      r3, [sp, #0x1b8]
007907e0: str      r3, [sp, #8]
007907e4: str      r2, [sp, #0x1bc]
007907e8: str      r2, [sp, #0xc]
007907ec: bl       #0x30eba4
007907f0: ldr      r2, [sp, #0xc]
007907f4: str      r0, [sp, #0x40]
007907f8: mov      r1, r2
007907fc: bl       #0x30e3ac
00790800: ldr      r3, [sp, #8]
00790804: ldr      r2, [sp, #0x40]
00790808: ldr      r1, [sp, #0x24]
0079080c: str      r0, [sp, #0x88]
00790810: mov      r0, r3
00790814: str      r2, [sp, #0x1c0]
00790818: bl       #0x30e3ac
0079081c: mov      r2, r0
00790820: mov      r1, r2
00790824: ldr      r0, [sp, #0x94]
00790828: str      r2, [sp, #0xc]
0079082c: bl       #0x30ec94
00790830: mov      r1, r0
00790834: ldr      r0, [sp, #0x1c8]
00790838: bl       #0x30eba4
0079083c: ldr      r1, [sp, #0x88]
00790840: str      r0, [sp, #0x1c8]
00790844: ldr      r0, [sp, #0x90]
00790848: bl       #0x30ec94
0079084c: mov      r1, r0
00790850: ldr      r0, [sp, #0x1d0]
00790854: bl       #0x30eba4
00790858: str      r0, [sp, #0x1d0]
0079085c: ldr      r1, [r4, #0xc]
00790860: mov      r0, #0x3f800000
00790864: bl       #0x30e3ac
00790868: ldr      r2, [sp, #0xc]
0079086c: mov      r1, r2
00790870: bl       #0x30ed6c
00790874: ldr      r1, [sp, #0x94]
00790878: bl       #0x30e3ac
0079087c: ldr      r3, [sp, #8]
00790880: mov      r1, r0
00790884: mov      r0, r3
00790888: bl       #0x30e3ac
0079088c: str      r0, [sp, #0x1b8]
00790890: ldr      r1, [r4, #0x14]
00790894: mov      r0, #0x3f800000
00790898: bl       #0x30e3ac
0079089c: ldr      r1, [sp, #0x88]
007908a0: bl       #0x30ed6c
007908a4: ldr      r1, [sp, #0x90]
007908a8: bl       #0x30e3ac
007908ac: mov      r1, r0
007908b0: ldr      r0, [sp, #0x40]
007908b4: bl       #0x30e3ac
007908b8: str      r0, [sp, #0x1c0]
007908bc: b        #0x790224
007908c0: mov      r0, sb
007908c4: mov      r1, #0x3f800000
007908c8: bl       #0x30df8c
007908cc: cmp      r0, #0
007908d0: mov      r2, #0
007908d4: moveq    r2, #1
007908d8: uxtb     r2, r2
007908dc: b        #0x78fd0c
007908e0: ldr      r1, [r0]
007908e4: sub      r1, r1, #1
007908e8: cmp      r1, #0
007908ec: str      r1, [r0]
007908f0: bne      #0x7908f8
007908f4: bl       #0x752b38
007908f8: mov      r3, #0
007908fc: str      r3, [r4, #0x2c]
00790900: str      r3, [r4, #0x30]
00790904: b        #0x78fb0c
00790908: eoreq    r4, r0, r0, ror #31
0079090c: strheq   r3, [r0], -r4
00790910: mulseq   r7, r4, r0
00790914: andeq    r2, r0, r0, lsr r5
00790918: andeq    r3, r0, ip, lsl r0
0079091c: ldr      lr, [sp, #0x64]
00790920: mov      r3, #0
00790924: strb     r3, [sp, #0x1a6]
00790928: strb     lr, [sp, #0x1a4]
0079092c: strb     r3, [sp, #0x1a5]
00790930: mov      r0, ip
00790934: add      ip, sp, #0x194
00790938: ldr      r2, [r4, #0x18]
0079093c: ldrh     r3, [r4, #0x1c]
00790940: ldrh     r1, [r4, #0x20]
00790944: str      ip, [sp, #4]
00790948: ldr      ip, [sp, #0x2c]
0079094c: str      ip, [sp]
00790950: bl       #0x7d266c
00790954: ldr      r1, [sp, #0x1b4]
00790958: ldr      r0, [sp, #0x1b8]
0079095c: bl       #0x30e3ac
00790960: ldr      r1, [sp, #0x1bc]
00790964: mov      r3, r0
00790968: ldr      r0, [sp, #0x1c0]
0079096c: str      r3, [sp, #8]
00790970: bl       #0x30e3ac
00790974: ldr      lr, [sp, #0x194]
00790978: ldr      r3, [sp, #8]
0079097c: mov      ip, r0
00790980: mov      r1, lr
00790984: mov      r0, r3
00790988: str      lr, [sp, #0x24]
0079098c: str      lr, [sp, #0x1b4]
00790990: str      ip, [sp, #0xc]
00790994: bl       #0x30eba4
00790998: ldr      ip, [sp, #0xc]
0079099c: ldr      r2, [sp, #0x19c]
007909a0: mov      r3, r0
007909a4: mov      r0, ip
007909a8: mov      r1, r2
007909ac: str      r3, [sp, #0x1b8]
007909b0: str      r3, [sp, #8]
007909b4: str      r2, [sp, #0x1bc]
007909b8: str      r2, [sp, #0xc]
007909bc: bl       #0x30eba4
007909c0: ldr      r2, [sp, #0xc]
007909c4: str      r0, [sp, #0x40]
007909c8: mov      r1, r2
007909cc: bl       #0x30e3ac
007909d0: ldr      r3, [sp, #8]
007909d4: ldr      r2, [sp, #0x40]
007909d8: ldr      r1, [sp, #0x24]
007909dc: str      r0, [sp, #0x88]
007909e0: mov      r0, r3
007909e4: str      r2, [sp, #0x1c0]
007909e8: bl       #0x30e3ac
007909ec: mov      r2, r0
007909f0: mov      r1, r2
007909f4: ldr      r0, [sp, #0x8c]
007909f8: str      r2, [sp, #0xc]
007909fc: bl       #0x30ec94
00790a00: mov      r1, r0
00790a04: ldr      r0, [sp, #0x1c8]
00790a08: bl       #0x30eba4
00790a0c: ldr      r1, [sp, #0x88]
00790a10: str      r0, [sp, #0x1c8]
00790a14: ldr      r0, [sp, #0x8c]
00790a18: bl       #0x30ec94
00790a1c: mov      r1, r0
00790a20: ldr      r0, [sp, #0x1d0]
00790a24: bl       #0x30eba4
00790a28: str      r0, [sp, #0x1d0]
00790a2c: ldr      r1, [r4, #0xc]
00790a30: mov      r0, #0x3f800000
00790a34: bl       #0x30e3ac
00790a38: ldr      r2, [sp, #0xc]
00790a3c: mov      r1, r2
00790a40: bl       #0x30ed6c
00790a44: ldr      r1, [sp, #0x8c]
00790a48: bl       #0x30e3ac
00790a4c: ldr      r3, [sp, #8]
00790a50: mov      r1, r0
00790a54: mov      r0, r3
00790a58: bl       #0x30e3ac
00790a5c: str      r0, [sp, #0x1b8]
00790a60: ldr      r1, [r4, #0x14]
00790a64: mov      r0, #0x3f800000
00790a68: bl       #0x30e3ac
00790a6c: ldr      r1, [sp, #0x88]
00790a70: bl       #0x30ed6c
00790a74: ldr      r1, [sp, #0x8c]
00790a78: b        #0x7908a8

# _ZN7gameswf19edit_text_character7displayEv
007928cc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007928d0: mov      r4, r0
007928d4: ldr      r0, [r0, #0x30]
007928d8: ldr      r5, [pc, #0xc40]
007928dc: sub      sp, sp, #0xd4
007928e0: cmp      r0, #0
007928e4: add      r5, pc, r5
007928e8: beq      #0x7928fc
007928ec: ldr      r3, [r4, #0x2c]
007928f0: ldrb     r2, [r3, #4]
007928f4: cmp      r2, #0
007928f8: beq      #0x792b30
007928fc: bl       #0x76d5b4
00792900: ldrb     r3, [r0, #0x85]
00792904: cmp      r3, #0
00792908: beq      #0x792918
0079290c: ldrb     r3, [r0, #0x86]
00792910: cmp      r3, #0
00792914: beq      #0x792e10
00792918: ldr      r3, [r4, #0xa0]
0079291c: ldrb     r3, [r3, #0x4e]
00792920: cmp      r3, #0
00792924: bne      #0x792bac
00792928: ldr      r6, [pc, #0xbf4]
0079292c: ldr      r7, [r4, #0x30]
00792930: cmp      r7, #0
00792934: beq      #0x792948
00792938: ldr      r0, [r4, #0x2c]
0079293c: ldrb     r3, [r0, #4]
00792940: cmp      r3, #0
00792944: beq      #0x792aec
00792948: ldr      r3, [r7, #0xac]
0079294c: mov      r1, #0x3f800000
00792950: ldr      r3, [r3, #0xc]
00792954: ldr      r0, [r3, #4]
00792958: bl       #0x30df8c
0079295c: cmp      r0, #0
00792960: beq      #0x792acc
00792964: ldr      r3, [r5, r6]
00792968: ldr      r2, [r4, #0xa0]
0079296c: ldr      r3, [r3]
00792970: ldr      r2, [r2, #0x94]
00792974: cmp      r3, #0
00792978: beq      #0x79298c
0079297c: subs     r2, r2, #0
00792980: movne    r2, #1
00792984: strb     r2, [r3, #4]
00792988: ldr      r7, [r4, #0x30]
0079298c: cmp      r7, #0
00792990: beq      #0x7929a4
00792994: ldr      r0, [r4, #0x2c]
00792998: ldrb     r3, [r0, #4]
0079299c: cmp      r3, #0
007929a0: beq      #0x792dc0
007929a4: ldrb     r3, [r7, #0x98]
007929a8: cmp      r3, #0
007929ac: bne      #0x792b68
007929b0: cmp      r7, #0
007929b4: beq      #0x7929c8
007929b8: ldr      r3, [r4, #0x2c]
007929bc: ldrb     r8, [r3, #4]
007929c0: cmp      r8, #0
007929c4: beq      #0x792e24
007929c8: ldrb     r3, [r7, #0x98]
007929cc: cmp      r3, #0
007929d0: beq      #0x7929f8
007929d4: ldr      r3, [r5, r6]
007929d8: ldr      r3, [r3]
007929dc: cmp      r3, #0
007929e0: beq      #0x7929f8
007929e4: mov      r0, r3
007929e8: add      r1, r4, #0xd8
007929ec: ldr      r3, [r3]
007929f0: mov      lr, pc
007929f4: ldr      pc, [r3, #0x4c]
007929f8: ldr      r3, [pc, #0xb28]
007929fc: add      sl, sp, #0xb0
00792a00: mov      r2, #0
00792a04: ldr      r1, [r5, r3]
00792a08: add      r3, sl, #8
00792a0c: str      r2, [r3], #4
00792a10: ldr      r0, [r1]
00792a14: str      r2, [r3], #4
00792a18: str      r2, [r3], #4
00792a1c: mov      r1, #0x3f800000
00792a20: cmp      r0, r2
00792a24: str      r2, [r3]
00792a28: str      r2, [sp, #0xb4]
00792a2c: str      r1, [sp, #0xc0]
00792a30: str      r1, [sp, #0xb0]
00792a34: beq      #0x792e64
00792a38: ldr      r3, [r4, #0xa8]
00792a3c: cmp      r3, #0
00792a40: ble      #0x792a70
00792a44: ldr      r3, [r4, #0xa0]
00792a48: mov      ip, #0
00792a4c: mov      r0, ip
00792a50: ldr      r3, [r3, #0x20]
00792a54: mov      r1, r4
00792a58: add      r2, r4, #0xa4
00792a5c: str      ip, [sp]
00792a60: str      ip, [sp, #4]
00792a64: str      ip, [sp, #8]
00792a68: str      ip, [sp, #0xc]
00792a6c: bl       #0x78faa0
00792a70: ldr      r3, [r4, #0x30]
00792a74: cmp      r3, #0
00792a78: beq      #0x792a8c
00792a7c: ldr      r0, [r4, #0x2c]
00792a80: ldrb     r2, [r0, #4]
00792a84: cmp      r2, #0
00792a88: beq      #0x792e3c
00792a8c: ldrb     r3, [r3, #0x98]
00792a90: cmp      r3, #0
00792a94: bne      #0x792de8
00792a98: ldrb     r3, [r4, #0x14c]
00792a9c: cmp      r3, #0
00792aa0: bne      #0x792b5c
00792aa4: ldr      r3, [r4, #0x54]
00792aa8: cmp      r3, #0
00792aac: beq      #0x792ac4
00792ab0: ldr      r3, [r3, #0x60]
00792ab4: cmp      r3, #0
00792ab8: beq      #0x792ac4
00792abc: mov      r0, r4
00792ac0: bl       #0x753f9c
00792ac4: add      sp, sp, #0xd4
00792ac8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00792acc: ldr      r3, [r5, r6]
00792ad0: ldr      r3, [r3]
00792ad4: cmp      r3, #0
00792ad8: beq      #0x79298c
00792adc: mov      r2, #0
00792ae0: strb     r2, [r3, #4]
00792ae4: ldr      r7, [r4, #0x30]
00792ae8: b        #0x79298c
00792aec: ldr      r1, [r0]
00792af0: sub      r1, r1, #1
00792af4: cmp      r1, #0
00792af8: str      r1, [r0]
00792afc: bne      #0x792b04
00792b00: bl       #0x752b38
00792b04: mov      r7, #0
00792b08: str      r7, [r4, #0x2c]
00792b0c: str      r7, [r4, #0x30]
00792b10: ldr      r3, [r7, #0xac]
00792b14: mov      r1, #0x3f800000
00792b18: ldr      r3, [r3, #0xc]
00792b1c: ldr      r0, [r3, #4]
00792b20: bl       #0x30df8c
00792b24: cmp      r0, #0
00792b28: bne      #0x792964
00792b2c: b        #0x792acc
00792b30: ldr      r1, [r3]
00792b34: sub      r1, r1, #1
00792b38: cmp      r1, #0
00792b3c: str      r1, [r3]
00792b40: bne      #0x792b4c
00792b44: mov      r0, r3
00792b48: bl       #0x752b38
00792b4c: mov      r0, #0
00792b50: str      r0, [r4, #0x2c]
00792b54: str      r0, [r4, #0x30]
00792b58: b        #0x7928fc
00792b5c: mov      r0, r4
00792b60: bl       #0x78b128
00792b64: b        #0x792aa4
00792b68: add      r7, r4, #0xd8
00792b6c: mov      r0, r7
00792b70: mov      r1, r4
00792b74: bl       #0x7738a4
00792b78: cmp      r0, #0
00792b7c: ldreq    r7, [r4, #0x30]
00792b80: beq      #0x7929b0
00792b84: ldr      r3, [r5, r6]
00792b88: ldr      r3, [r3]
00792b8c: cmp      r3, #0
00792b90: beq      #0x792a98
00792b94: mov      r0, r3
00792b98: mov      r1, r7
00792b9c: ldr      r3, [r3]
00792ba0: mov      lr, pc
00792ba4: ldr      pc, [r3, #0x64]
00792ba8: b        #0x792a98
00792bac: mov      r0, r4
00792bb0: bl       #0x753f74
00792bb4: add      sl, sp, #0xb0
00792bb8: ldr      r6, [pc, #0x964]
00792bbc: mov      ip, r0
00792bc0: mov      lr, sl
00792bc4: ldm      ip!, {r0, r1, r2, r3}
00792bc8: stm      lr!, {r0, r1, r2, r3}
00792bcc: ldr      r2, [r5, r6]
00792bd0: ldm      ip, {r0, r1}
00792bd4: ldr      ip, [r2]
00792bd8: stm      lr, {r0, r1}
00792bdc: cmp      ip, #0
00792be0: beq      #0x792bf8
00792be4: mov      r0, ip
00792be8: mov      r1, sl
00792bec: ldr      r3, [ip]
00792bf0: mov      lr, pc
00792bf4: ldr      pc, [r3, #0x50]
00792bf8: ldr      r3, [r4, #0xa0]
00792bfc: mov      r7, #0
00792c00: add      r1, sp, #0x48
00792c04: str      r1, [sp, #0x28]
00792c08: str      r7, [sp, #0x90]
00792c0c: str      r7, [sp, #0x94]
00792c10: str      r7, [sp, #0x98]
00792c14: str      r7, [sp, #0x9c]
00792c18: str      r7, [sp, #0xa0]
00792c1c: str      r7, [sp, #0xa4]
00792c20: str      r7, [sp, #0xa8]
00792c24: str      r7, [sp, #0xac]
00792c28: ldr      sl, [r3, #0x24]
00792c2c: ldr      r8, [r3, #0x2c]
00792c30: mov      r0, r1
00792c34: str      sl, [sp, #0x90]
00792c38: str      r8, [sp, #0x94]
00792c3c: ldr      fp, [r3, #0x28]
00792c40: ldr      ip, [r3, #0x2c]
00792c44: mov      r1, #0
00792c48: str      fp, [sp, #0x98]
00792c4c: str      ip, [sp, #0x9c]
00792c50: ldr      r2, [r3, #0x24]
00792c54: str      r2, [sp, #0x20]
00792c58: ldr      lr, [r3, #0x30]
00792c5c: mov      r2, #0x48
00792c60: str      lr, [sp, #0x24]
00792c64: ldr      lr, [sp, #0x20]
00792c68: str      lr, [sp, #0xa0]
00792c6c: ldr      lr, [sp, #0x24]
00792c70: str      lr, [sp, #0xa4]
00792c74: ldr      lr, [r3, #0x30]
00792c78: str      lr, [sp, #0x1c]
00792c7c: ldr      r3, [r3, #0x28]
00792c80: str      lr, [sp, #0xac]
00792c84: str      ip, [sp, #0x14]
00792c88: str      r3, [sp, #0xa8]
00792c8c: str      r3, [sp, #0x18]
00792c90: bl       #0x30e460
00792c94: ldr      sb, [r5, r6]
00792c98: ldr      r3, [sp, #0x18]
00792c9c: ldr      r1, [sp, #0x1c]
00792ca0: ldr      r2, [sb]
00792ca4: ldr      ip, [sp, #0x14]
00792ca8: ldr      lr, [sp, #0x20]
00792cac: str      r3, [sp, #0x78]
00792cb0: str      r1, [sp, #0x7c]
00792cb4: str      r3, [sp, #0x60]
00792cb8: ldr      r1, [sp, #0x24]
00792cbc: ldr      r3, [sp, #0x1c]
00792cc0: cmp      r2, #0
00792cc4: str      fp, [sp, #0x70]
00792cc8: str      ip, [sp, #0x74]
00792ccc: str      lr, [sp, #0x80]
00792cd0: str      r1, [sp, #0x84]
00792cd4: str      sl, [sp, #0x88]
00792cd8: str      r8, [sp, #0x8c]
00792cdc: str      sl, [sp, #0x48]
00792ce0: str      r8, [sp, #0x4c]
00792ce4: str      fp, [sp, #0x50]
00792ce8: str      ip, [sp, #0x54]
00792cec: str      lr, [sp, #0x58]
00792cf0: str      r1, [sp, #0x5c]
00792cf4: str      r3, [sp, #0x64]
00792cf8: str      sl, [sp, #0x68]
00792cfc: str      r8, [sp, #0x6c]
00792d00: beq      #0x79292c
00792d04: ldr      r3, [r2]
00792d08: mov      r0, r2
00792d0c: mov      r1, #0
00792d10: add      r2, r4, #0x194
00792d14: mov      lr, pc
00792d18: ldr      pc, [r3, #0x6c]
00792d1c: ldr      r3, [sb]
00792d20: cmp      r3, #0
00792d24: beq      #0x79292c
00792d28: mov      r0, r3
00792d2c: ldr      r1, [sp, #0x28]
00792d30: ldr      r3, [r3]
00792d34: mov      r2, #4
00792d38: mov      lr, pc
00792d3c: ldr      pc, [r3, #0x58]
00792d40: ldr      r0, [sb]
00792d44: cmp      r0, #0
00792d48: beq      #0x79292c
00792d4c: ldr      r3, [r0]
00792d50: mov      r2, #0
00792d54: mvn      r1, #0
00792d58: ldr      r3, [r3, #0x78]
00792d5c: strb     r1, [sp, #0xcb]
00792d60: strb     r2, [sp, #0xc8]
00792d64: strb     r2, [sp, #0xca]
00792d68: strb     r2, [sp, #0xc9]
00792d6c: ldr      r1, [sp, #0xc8]
00792d70: blx      r3
00792d74: ldr      r3, [sb]
00792d78: cmp      r3, #0
00792d7c: beq      #0x79292c
00792d80: mov      r0, r3
00792d84: mov      r1, r7
00792d88: ldr      r3, [r3]
00792d8c: mov      lr, pc
00792d90: ldr      pc, [r3, #0x7c]
00792d94: ldr      r3, [sb]
00792d98: cmp      r3, #0
00792d9c: beq      #0x79292c
00792da0: ldr      ip, [sp, #0x28]
00792da4: mov      r0, r3
00792da8: mov      r2, #5
00792dac: add      r1, ip, #0x20
00792db0: ldr      r3, [r3]
00792db4: mov      lr, pc
00792db8: ldr      pc, [r3, #0x60]
00792dbc: b        #0x79292c
00792dc0: ldr      r1, [r0]
00792dc4: sub      r1, r1, #1
00792dc8: cmp      r1, #0
00792dcc: str      r1, [r0]
00792dd0: bne      #0x792dd8
00792dd4: bl       #0x752b38
00792dd8: mov      r7, #0
00792ddc: str      r7, [r4, #0x2c]
00792de0: str      r7, [r4, #0x30]
00792de4: b        #0x7929a4
00792de8: ldr      r3, [r5, r6]
00792dec: ldr      r3, [r3]
00792df0: cmp      r3, #0
00792df4: beq      #0x792a98
00792df8: mov      r0, r3
00792dfc: mov      r1, #0
00792e00: ldr      r3, [r3]
00792e04: mov      lr, pc
00792e08: ldr      pc, [r3, #0x4c]
00792e0c: b        #0x792a98
00792e10: add      r1, sp, #0xd0
00792e14: str      r4, [r1, #-4]!
00792e18: add      r0, r0, #0x98
00792e1c: bl       #0x78adc0
00792e20: b        #0x792ac4
00792e24: add      r0, r4, #0x2c
00792e28: mov      r1, r8
00792e2c: bl       #0x41fe84
00792e30: mov      r7, r8
00792e34: str      r8, [r4, #0x30]
00792e38: b        #0x7929c8
00792e3c: ldr      r1, [r0]
00792e40: sub      r1, r1, #1
00792e44: cmp      r1, #0
00792e48: str      r1, [r0]
00792e4c: bne      #0x792e54
00792e50: bl       #0x752b38
00792e54: mov      r3, #0
00792e58: str      r3, [r4, #0x2c]
00792e5c: str      r3, [r4, #0x30]
00792e60: b        #0x792a8c
00792e64: ldr      r3, [r4, #0x50]
00792e68: ldr      r2, [r3, #8]
00792e6c: cmp      r2, #0
00792e70: bgt      #0x793514
00792e74: mov      r7, r4
00792e78: ldr      r8, [r7, #0x40]
00792e7c: cmp      r8, #0
00792e80: beq      #0x792a38
00792e84: ldr      r0, [r7, #0x3c]
00792e88: ldrb     r3, [r0, #4]
00792e8c: cmp      r3, #0
00792e90: beq      #0x7932a8
00792e94: ldr      r3, [r8, #0x50]
00792e98: mov      r7, r8
00792e9c: ldr      r2, [r3, #8]
00792ea0: cmp      r2, #0
00792ea4: ble      #0x792e78
00792ea8: str      r2, [sp, #0x24]
00792eac: ldr      r1, [sp, #0x24]
00792eb0: subs     r2, r1, #1
00792eb4: bmi      #0x792a38
00792eb8: mov      sb, #0x2c
00792ebc: mul      sb, sb, r2
00792ec0: add      r2, r4, #0xa4
00792ec4: str      r2, [sp, #0x34]
00792ec8: mov      ip, #1
00792ecc: add      r1, sp, #0x90
00792ed0: add      r2, sp, #0xc8
00792ed4: mov      fp, #0
00792ed8: str      ip, [sp, #0x3c]
00792edc: str      r1, [sp, #0x1c]
00792ee0: str      r2, [sp, #0x38]
00792ee4: str      r8, [sp, #0x2c]
00792ee8: str      r5, [sp, #0x40]
00792eec: str      r6, [sp, #0x44]
00792ef0: str      sl, [sp, #0x20]
00792ef4: ldr      r5, [r3, #4]
00792ef8: ldr      r3, [r5, sb]
00792efc: add      r5, r5, sb
00792f00: cmp      r3, #2
00792f04: beq      #0x7932d0
00792f08: cmp      r3, #0
00792f0c: bne      #0x793138
00792f10: ldr      r0, [r5, #0x20]
00792f14: bl       #0x30e4cc
00792f18: mov      r6, r0
00792f1c: ldr      r0, [r5, #0x24]
00792f20: bl       #0x30e4cc
00792f24: str      r0, [sp, #0x28]
00792f28: ldr      r8, [r5, #8]
00792f2c: ldr      r7, [r5, #0xc]
00792f30: mov      r0, r8
00792f34: bl       #0x30e754
00792f38: mov      r1, r0
00792f3c: rsb      r0, r6, #0
00792f40: str      r1, [sp, #0x18]
00792f44: bl       #0x30e964
00792f48: ldr      r1, [sp, #0x18]
00792f4c: mov      sl, r0
00792f50: mov      r0, r7
00792f54: bl       #0x30ed6c
00792f58: mov      r1, r0
00792f5c: mov      r0, sl
00792f60: bl       #0x30eba4
00792f64: str      r0, [sp, #0x30]
00792f68: mov      r0, r8
00792f6c: bl       #0x30eb08
00792f70: ldr      lr, [sp, #0x28]
00792f74: mov      sl, r0
00792f78: rsb      r0, lr, #0
00792f7c: bl       #0x30e964
00792f80: mov      r1, sl
00792f84: mov      r8, r0
00792f88: mov      r0, r7
00792f8c: bl       #0x30ed6c
00792f90: mov      r1, r0
00792f94: mov      r0, r8
00792f98: bl       #0x30eba4
00792f9c: mvn      r1, #0
00792fa0: strb     r1, [sp, #0xc8]
00792fa4: strb     r1, [sp, #0xc9]
00792fa8: strb     r1, [sp, #0xca]
00792fac: strb     r1, [sp, #0xcb]
00792fb0: ldrb     lr, [r5, #4]
00792fb4: ldr      ip, [sp, #0x20]
00792fb8: mov      r7, r0
00792fbc: strb     lr, [sp, #0xca]
00792fc0: ldrb     lr, [r5, #5]
00792fc4: ldm      ip!, {r0, r1, r2, r3}
00792fc8: strb     lr, [sp, #0xc9]
00792fcc: ldrb     lr, [r5, #6]
00792fd0: strb     lr, [sp, #0xc8]
00792fd4: ldrb     lr, [r5, #7]
00792fd8: strb     lr, [sp, #0xcb]
00792fdc: ldr      lr, [sp, #0x1c]
00792fe0: stm      lr!, {r0, r1, r2, r3}
00792fe4: ldm      ip, {r0, r1}
00792fe8: str      r1, [lr, #4]
00792fec: mov      r1, #0x41000000
00792ff0: str      r0, [lr]
00792ff4: add      r1, r1, #0xa00000
00792ff8: ldr      r0, [sp, #0x30]
00792ffc: bl       #0x30ed6c
00793000: mov      r1, #0x41000000
00793004: mov      r8, r0
00793008: add      r1, r1, #0xa00000
0079300c: mov      r0, r7
00793010: bl       #0x30ed6c
00793014: ldr      r1, [sp, #0x90]
00793018: mov      r7, r0
0079301c: mov      r0, r8
00793020: bl       #0x30ed6c
00793024: ldr      r1, [sp, #0x94]
00793028: mov      r5, r0
0079302c: mov      r0, r7
00793030: bl       #0x30ed6c
00793034: mov      r1, r0
00793038: mov      r0, r5
0079303c: bl       #0x30eba4
00793040: ldr      r1, [sp, #0x98]
00793044: bl       #0x30eba4
00793048: mvn      r1, #0x800000
0079304c: mov      r5, r0
00793050: bl       #0x30e4b4
00793054: cmp      r0, #0
00793058: beq      #0x793130
0079305c: mvn      r1, #0x80000000
00793060: mov      r0, r5
00793064: sub      r1, r1, #0x800000
00793068: bl       #0x30e9ac
0079306c: cmp      r0, #0
00793070: beq      #0x793130
00793074: ldr      r1, [sp, #0x9c]
00793078: mov      r0, r8
0079307c: str      r5, [sp, #0x98]
00793080: bl       #0x30ed6c
00793084: ldr      r1, [sp, #0xa0]
00793088: mov      r5, r0
0079308c: mov      r0, r7
00793090: bl       #0x30ed6c
00793094: mov      r1, r0
00793098: mov      r0, r5
0079309c: bl       #0x30eba4
007930a0: ldr      r1, [sp, #0xa4]
007930a4: bl       #0x30eba4
007930a8: mvn      r1, #0x800000
007930ac: mov      r5, r0
007930b0: bl       #0x30e4b4
007930b4: cmp      r0, #0
007930b8: beq      #0x7934d4
007930bc: mvn      r1, #0x80000000
007930c0: mov      r0, r5
007930c4: sub      r1, r1, #0x800000
007930c8: bl       #0x30e9ac
007930cc: cmp      r0, #0
007930d0: beq      #0x7934d4
007930d4: ldr      r1, [sp, #0x28]
007930d8: ldr      r3, [r4, #0xa0]
007930dc: str      r5, [sp, #0xa4]
007930e0: uxtb     ip, r1
007930e4: ldr      r3, [r3, #0x20]
007930e8: str      ip, [sp, #0xc]
007930ec: ldr      ip, [sp, #0x38]
007930f0: uxtb     r6, r6
007930f4: mov      lr, #0
007930f8: ldr      r0, [sp, #0x1c]
007930fc: mov      r1, r4
00793100: ldr      r2, [sp, #0x34]
00793104: str      r6, [sp, #8]
00793108: stm      sp, {ip, lr}
0079310c: bl       #0x78faa0
00793110: ldr      r2, [sp, #0x24]
00793114: add      fp, fp, #1
00793118: sub      sb, sb, #0x2c
0079311c: cmp      fp, r2
00793120: beq      #0x7934f0
00793124: ldr      ip, [sp, #0x2c]
00793128: ldr      r3, [ip, #0x50]
0079312c: b        #0x792ef4
00793130: mov      r5, #0
00793134: b        #0x793074
00793138: cmp      r3, #1
0079313c: bne      #0x793110
00793140: ldr      r0, [r5, #0x20]
00793144: bl       #0x8be2a0
00793148: uxtb     r6, r0
0079314c: ldr      r0, [r5, #0x24]
00793150: bl       #0x8be2a0
00793154: uxtb     r5, r0
00793158: orrs     r1, r5, r6
0079315c: beq      #0x793110
00793160: ldr      ip, [sp, #0x20]
00793164: ldr      lr, [sp, #0x1c]
00793168: ldm      ip!, {r0, r1, r2, r3}
0079316c: stm      lr!, {r0, r1, r2, r3}
00793170: ldm      ip, {r0, r1}
00793174: stm      lr, {r0, r1}
00793178: rsb      r0, r6, #0
0079317c: bl       #0x30e964
00793180: mov      r1, #0x41000000
00793184: add      r1, r1, #0xa00000
00793188: bl       #0x30ed6c
0079318c: mov      r7, r0
00793190: rsb      r0, r5, #0
00793194: bl       #0x30e964
00793198: mov      r1, #0x41000000
0079319c: add      r1, r1, #0xa00000
007931a0: bl       #0x30ed6c
007931a4: ldr      r1, [sp, #0x90]
007931a8: mov      sl, r0
007931ac: mov      r0, r7
007931b0: bl       #0x30ed6c
007931b4: ldr      r1, [sp, #0x94]
007931b8: mov      r8, r0
007931bc: mov      r0, sl
007931c0: bl       #0x30ed6c
007931c4: mov      r1, r0
007931c8: mov      r0, r8
007931cc: bl       #0x30eba4
007931d0: ldr      r1, [sp, #0x98]
007931d4: bl       #0x30eba4
007931d8: mvn      r1, #0x800000
007931dc: mov      r8, r0
007931e0: bl       #0x30e4b4
007931e4: cmp      r0, #0
007931e8: beq      #0x7932a0
007931ec: mvn      r1, #0x80000000
007931f0: mov      r0, r8
007931f4: sub      r1, r1, #0x800000
007931f8: bl       #0x30e9ac
007931fc: cmp      r0, #0
00793200: beq      #0x7932a0
00793204: mov      r0, r7
00793208: ldr      r1, [sp, #0x9c]
0079320c: str      r8, [sp, #0x98]
00793210: bl       #0x30ed6c
00793214: ldr      r1, [sp, #0xa0]
00793218: mov      r7, r0
0079321c: mov      r0, sl
00793220: bl       #0x30ed6c
00793224: mov      r1, r0
00793228: mov      r0, r7
0079322c: bl       #0x30eba4
00793230: ldr      r1, [sp, #0xa4]
00793234: bl       #0x30eba4
00793238: mvn      r1, #0x800000
0079323c: mov      r7, r0
00793240: bl       #0x30e4b4
00793244: cmp      r0, #0
00793248: beq      #0x79350c
0079324c: mvn      r1, #0x80000000
00793250: mov      r0, r7
00793254: sub      r1, r1, #0x800000
00793258: bl       #0x30e9ac
0079325c: cmp      r0, #0
00793260: beq      #0x79350c
00793264: ldr      r3, [r4, #0xa0]
00793268: str      r7, [sp, #0xa4]
0079326c: mov      ip, #0
00793270: ldr      r3, [r3, #0x20]
00793274: mov      r1, r4
00793278: ldr      r0, [sp, #0x1c]
0079327c: ldr      r2, [sp, #0x34]
00793280: str      r6, [sp, #8]
00793284: str      r5, [sp, #0xc]
00793288: str      ip, [sp]
0079328c: str      ip, [sp, #4]
00793290: bl       #0x78faa0
00793294: mov      r1, #0
00793298: str      r1, [sp, #0x3c]
0079329c: b        #0x793110
007932a0: mov      r8, #0
007932a4: b        #0x793204
007932a8: ldr      r1, [r0]
007932ac: sub      r1, r1, #1
007932b0: cmp      r1, #0
007932b4: str      r1, [r0]
007932b8: bne      #0x7932c0
007932bc: bl       #0x752b38
007932c0: mov      r3, #0
007932c4: str      r3, [r7, #0x40]
007932c8: str      r3, [r7, #0x3c]
007932cc: b        #0x792a38
007932d0: ldr      r3, [r5, #0x24]
007932d4: mvn      ip, #0
007932d8: ldr      sl, [r5, #0x20]
007932dc: strb     ip, [sp, #0xc8]
007932e0: strb     ip, [sp, #0xc9]
007932e4: strb     ip, [sp, #0xca]
007932e8: strb     ip, [sp, #0xcb]
007932ec: str      r3, [sp, #0x28]
007932f0: ldrb     r3, [r5, #4]
007932f4: strb     r3, [sp, #0xca]
007932f8: ldrb     r3, [r5, #5]
007932fc: strb     r3, [sp, #0xc9]
00793300: ldrb     r3, [r5, #6]
00793304: strb     r3, [sp, #0xc8]
00793308: ldrb     r7, [r5, #7]
0079330c: strb     r7, [sp, #0xcb]
00793310: ldr      r6, [r5, #0x24]
00793314: ldr      r8, [r5, #0x20]
00793318: ldr      r5, [r5, #0x10]
0079331c: mov      r1, r6
00793320: mov      r0, r8
00793324: bl       #0x30e70c
00793328: cmp      r0, #0
0079332c: mov      r0, r7
00793330: moveq    r6, r8
00793334: bl       #0x30e964
00793338: mov      r7, r0
0079333c: mov      r0, r5
00793340: bl       #0x30e964
00793344: mov      r1, #0x41000000
00793348: add      r1, r1, #0x200000
0079334c: bl       #0x30ec94
00793350: mov      r1, r0
00793354: mov      r0, r7
00793358: bl       #0x30ed6c
0079335c: bl       #0x30e4cc
00793360: cmp      r0, #0xfe
00793364: mvngt    r2, #0
00793368: strbgt   r2, [sp, #0xcb]
0079336c: ble      #0x7934dc
00793370: ldr      ip, [sp, #0x20]
00793374: ldr      lr, [sp, #0x1c]
00793378: ldm      ip!, {r0, r1, r2, r3}
0079337c: stm      lr!, {r0, r1, r2, r3}
00793380: ldm      ip, {r0, r1}
00793384: stm      lr, {r0, r1}
00793388: mov      r0, sl
0079338c: bl       #0x30e4cc
00793390: rsb      r0, r0, #0
00793394: bl       #0x30e964
00793398: mov      r1, #0x41000000
0079339c: add      r1, r1, #0xa00000
007933a0: bl       #0x30ed6c
007933a4: mov      r8, r0
007933a8: ldr      r0, [sp, #0x28]
007933ac: bl       #0x30e4cc
007933b0: rsb      r0, r0, #0
007933b4: bl       #0x30e964
007933b8: mov      r1, #0x41000000
007933bc: add      r1, r1, #0xa00000
007933c0: bl       #0x30ed6c
007933c4: ldr      r1, [sp, #0x90]
007933c8: mov      r7, r0
007933cc: mov      r0, r8
007933d0: bl       #0x30ed6c
007933d4: ldr      r1, [sp, #0x94]
007933d8: mov      r5, r0
007933dc: mov      r0, r7
007933e0: bl       #0x30ed6c
007933e4: mov      r1, r0
007933e8: mov      r0, r5
007933ec: bl       #0x30eba4
007933f0: ldr      r1, [sp, #0x98]
007933f4: bl       #0x30eba4
007933f8: mvn      r1, #0x800000
007933fc: mov      r5, r0
00793400: bl       #0x30e4b4
00793404: cmp      r0, #0
00793408: beq      #0x7934cc
0079340c: mvn      r1, #0x80000000
00793410: mov      r0, r5
00793414: sub      r1, r1, #0x800000
00793418: bl       #0x30e9ac
0079341c: cmp      r0, #0
00793420: beq      #0x7934cc
00793424: ldr      r1, [sp, #0x9c]
00793428: mov      r0, r8
0079342c: str      r5, [sp, #0x98]
00793430: bl       #0x30ed6c
00793434: ldr      r1, [sp, #0xa0]
00793438: mov      r5, r0
0079343c: mov      r0, r7
00793440: bl       #0x30ed6c
00793444: mov      r1, r0
00793448: mov      r0, r5
0079344c: bl       #0x30eba4
00793450: ldr      r1, [sp, #0xa4]
00793454: bl       #0x30eba4
00793458: mvn      r1, #0x800000
0079345c: mov      r5, r0
00793460: bl       #0x30e4b4
00793464: cmp      r0, #0
00793468: beq      #0x793504
0079346c: mvn      r1, #0x80000000
00793470: mov      r0, r5
00793474: sub      r1, r1, #0x800000
00793478: bl       #0x30e9ac
0079347c: cmp      r0, #0
00793480: beq      #0x793504
00793484: ldr      r3, [r4, #0xa0]
00793488: ldr      r1, [sp, #0x38]
0079348c: str      r5, [sp, #0xa4]
00793490: mov      r0, r6
00793494: ldr      r5, [r3, #0x20]
00793498: str      r1, [sp]
0079349c: bl       #0x8be2a0
007934a0: uxtb     ip, r0
007934a4: str      ip, [sp, #4]
007934a8: mov      r3, r5
007934ac: mov      ip, #0
007934b0: mov      r1, r4
007934b4: ldr      r0, [sp, #0x1c]
007934b8: ldr      r2, [sp, #0x34]
007934bc: str      ip, [sp, #8]
007934c0: str      ip, [sp, #0xc]
007934c4: bl       #0x78faa0
007934c8: b        #0x793110
007934cc: mov      r5, #0
007934d0: b        #0x793424
007934d4: mov      r5, #0
007934d8: b        #0x7930d4
007934dc: uxtb     r0, r0
007934e0: cmp      r0, #0
007934e4: strb     r0, [sp, #0xcb]
007934e8: beq      #0x793110
007934ec: b        #0x793370
007934f0: add      r1, sp, #0x3c
007934f4: ldm      r1, {r1, r5, r6}
007934f8: cmp      r1, #0
007934fc: beq      #0x792a70
00793500: b        #0x792a38
00793504: mov      r5, #0
00793508: b        #0x793484
0079350c: mov      r7, #0
00793510: b        #0x793264
00793514: str      r2, [sp, #0x24]
00793518: mov      r8, r4
0079351c: b        #0x792eac
00793520: eoreq    r2, r0, ip, lsr #3
00793524: strheq   r3, [r0], -r4
00793528: strheq   r3, [r0], -r8

# _ZNK7gameswf4font16get_units_per_emEv
007cf41c: push     {r4, lr}
007cf420: ldr      r3, [r0, #0x1c]
007cf424: mov      r4, r0
007cf428: cmp      r3, #0
007cf42c: beq      #0x7cf440
007cf430: ldr      r0, [r0, #0x18]
007cf434: ldrb     r2, [r0, #4]
007cf438: cmp      r2, #0
007cf43c: beq      #0x7cf4c8
007cf440: ldr      r2, [r3, #0xac]
007cf444: ldr      r0, [r2, #0x10]
007cf448: cmp      r0, #0
007cf44c: beq      #0x7cf478
007cf450: add      r1, r4, #0x30
007cf454: ldrb     r2, [r4, #0x4d]
007cf458: ldrb     r3, [r4, #0x4c]
007cf45c: bl       #0x7c6048
007cf460: cmp      r0, #0
007cf464: beq      #0x7cf474
007cf468: mov      r0, #0x44000000
007cf46c: add      r0, r0, #0x800000
007cf470: pop      {r4, pc}
007cf474: ldr      r3, [r4, #0x1c]
007cf478: cmp      r3, #0
007cf47c: beq      #0x7cf490
007cf480: ldr      r0, [r4, #0x18]
007cf484: ldrb     r2, [r0, #4]
007cf488: cmp      r2, #0
007cf48c: beq      #0x7cf4f0
007cf490: ldr      r3, [r3, #0xac]
007cf494: ldr      r0, [r3, #0xc]
007cf498: cmp      r0, #0
007cf49c: beq      #0x7cf518
007cf4a0: ldrb     r3, [r4, #0x4c]
007cf4a4: add      r1, r4, #0x30
007cf4a8: ldrb     r2, [r4, #0x4d]
007cf4ac: bl       #0x7d113c
007cf4b0: cmp      r0, #0
007cf4b4: beq      #0x7cf518
007cf4b8: ldr      r3, [r0, #0x24]
007cf4bc: ldrh     r0, [r3, #0x44]
007cf4c0: bl       #0x30e2e0
007cf4c4: pop      {r4, pc}
007cf4c8: ldr      r1, [r0]
007cf4cc: sub      r1, r1, #1
007cf4d0: cmp      r1, #0
007cf4d4: str      r1, [r0]
007cf4d8: bne      #0x7cf4e0
007cf4dc: bl       #0x752b38
007cf4e0: mov      r3, #0
007cf4e4: str      r3, [r4, #0x18]
007cf4e8: str      r3, [r4, #0x1c]
007cf4ec: b        #0x7cf440
007cf4f0: ldr      r1, [r0]
007cf4f4: sub      r1, r1, #1
007cf4f8: cmp      r1, #0
007cf4fc: str      r1, [r0]
007cf500: bne      #0x7cf508
007cf504: bl       #0x752b38
007cf508: mov      r3, #0
007cf50c: str      r3, [r4, #0x18]
007cf510: str      r3, [r4, #0x1c]
007cf514: b        #0x7cf490
007cf518: mov      r0, #0x3f800000
007cf51c: pop      {r4, pc}

# _ZNK7gameswf4font10get_heightEv
007cf520: push     {r4, lr}
007cf524: ldr      r3, [r0, #0x1c]
007cf528: mov      r4, r0
007cf52c: cmp      r3, #0
007cf530: beq      #0x7cf544
007cf534: ldr      r0, [r0, #0x18]
007cf538: ldrb     r2, [r0, #4]
007cf53c: cmp      r2, #0
007cf540: beq      #0x7cf5dc
007cf544: ldr      r2, [r3, #0xac]
007cf548: ldr      r0, [r2, #0x10]
007cf54c: cmp      r0, #0
007cf550: beq      #0x7cf584
007cf554: ldrb     r3, [r4, #0x4c]
007cf558: add      r1, r4, #0x30
007cf55c: ldrb     r2, [r4, #0x4d]
007cf560: bl       #0x7c6048
007cf564: cmp      r0, #0
007cf568: ldreq    r3, [r4, #0x1c]
007cf56c: beq      #0x7cf584
007cf570: mov      r1, #0x41000000
007cf574: ldr      r0, [r0, #0x28]
007cf578: add      r1, r1, #0xa00000
007cf57c: bl       #0x30ed6c
007cf580: pop      {r4, pc}
007cf584: cmp      r3, #0
007cf588: beq      #0x7cf59c
007cf58c: ldr      r0, [r4, #0x18]
007cf590: ldrb     r2, [r0, #4]
007cf594: cmp      r2, #0
007cf598: beq      #0x7cf604
007cf59c: ldr      r3, [r3, #0xac]
007cf5a0: ldr      r0, [r3, #0xc]
007cf5a4: cmp      r0, #0
007cf5a8: beq      #0x7cf62c
007cf5ac: ldrb     r3, [r4, #0x4c]
007cf5b0: add      r1, r4, #0x30
007cf5b4: ldrb     r2, [r4, #0x4d]
007cf5b8: bl       #0x7d113c
007cf5bc: cmp      r0, #0
007cf5c0: beq      #0x7cf62c
007cf5c4: ldr      r3, [r0, #0x24]
007cf5c8: ldrsh    r2, [r3, #0x48]
007cf5cc: ldrsh    r0, [r3, #0x46]
007cf5d0: rsb      r0, r2, r0
007cf5d4: bl       #0x30e964
007cf5d8: pop      {r4, pc}
007cf5dc: ldr      r1, [r0]
007cf5e0: sub      r1, r1, #1
007cf5e4: cmp      r1, #0
007cf5e8: str      r1, [r0]
007cf5ec: bne      #0x7cf5f4
007cf5f0: bl       #0x752b38
007cf5f4: mov      r3, #0
007cf5f8: str      r3, [r4, #0x18]
007cf5fc: str      r3, [r4, #0x1c]
007cf600: b        #0x7cf544
007cf604: ldr      r1, [r0]
007cf608: sub      r1, r1, #1
007cf60c: cmp      r1, #0
007cf610: str      r1, [r0]
007cf614: bne      #0x7cf61c
007cf618: bl       #0x752b38
007cf61c: mov      r3, #0
007cf620: str      r3, [r4, #0x18]
007cf624: str      r3, [r4, #0x1c]
007cf628: b        #0x7cf59c
007cf62c: mov      r0, #0
007cf630: pop      {r4, pc}

# _ZN7gameswf4font9copy_fromEPS0_
007ce628: push     {r4, r5, r6, lr}
007ce62c: mov      r4, r0
007ce630: mov      r5, r1
007ce634: add      r0, r0, #0x30
007ce638: add      r1, r1, #0x30
007ce63c: bl       #0x752f50
007ce640: ldrb     r3, [r5, #0x49]
007ce644: strb     r3, [r4, #0x49]
007ce648: ldrb     r3, [r5, #0x4a]
007ce64c: strb     r3, [r4, #0x4a]
007ce650: ldrb     r3, [r5, #0x4b]
007ce654: strb     r3, [r4, #0x4b]
007ce658: ldrb     r3, [r5, #0x4c]
007ce65c: strb     r3, [r4, #0x4c]
007ce660: ldrb     r3, [r5, #0x4d]
007ce664: strb     r3, [r4, #0x4d]
007ce668: ldrb     r3, [r5, #0x4e]
007ce66c: strb     r3, [r4, #0x4e]
007ce670: ldr      r3, [r5, #0x54]
007ce674: str      r3, [r4, #0x54]
007ce678: ldr      r3, [r5, #0x58]
007ce67c: str      r3, [r4, #0x58]
007ce680: ldr      r3, [r5, #0x5c]
007ce684: str      r3, [r4, #0x5c]
007ce688: ldrb     r3, [r5, #0x74]
007ce68c: strb     r3, [r4, #0x74]
007ce690: pop      {r4, r5, r6, pc}

# _ZN7gameswf4fontC1EPNS_6playerE
007cf8d8: push     {r4, r5, r6, lr}
007cf8dc: ldr      r5, [pc, #0x98]
007cf8e0: mov      r4, r0
007cf8e4: bl       #0x75ea44
007cf8e8: ldr      r3, [pc, #0x90]
007cf8ec: add      r5, pc, r5
007cf8f0: mov      r6, #0
007cf8f4: ldr      r3, [r5, r3]
007cf8f8: str      r6, [r4, #0x20]
007cf8fc: str      r6, [r4, #0x24]
007cf900: add      r3, r3, #8
007cf904: str      r3, [r4]
007cf908: str      r6, [r4, #0x28]
007cf90c: strb     r6, [r4, #0x2c]
007cf910: add      r0, r4, #0x30
007cf914: bl       #0x7cef34
007cf918: mov      r3, #0
007cf91c: mov      r2, #1
007cf920: strb     r2, [r4, #0x4b]
007cf924: str      r3, [r4, #0x5c]
007cf928: strb     r6, [r4, #0x84]
007cf92c: str      r6, [r4, #0x44]
007cf930: strb     r6, [r4, #0x49]
007cf934: strb     r6, [r4, #0x4a]
007cf938: strb     r6, [r4, #0x4c]
007cf93c: strb     r6, [r4, #0x4d]
007cf940: strb     r6, [r4, #0x4e]
007cf944: str      r6, [r4, #0x50]
007cf948: str      r3, [r4, #0x54]
007cf94c: str      r3, [r4, #0x58]
007cf950: str      r6, [r4, #0x60]
007cf954: str      r6, [r4, #0x64]
007cf958: str      r6, [r4, #0x68]
007cf95c: strb     r6, [r4, #0x6c]
007cf960: str      r6, [r4, #0x70]
007cf964: strb     r6, [r4, #0x74]
007cf968: str      r6, [r4, #0x78]
007cf96c: str      r6, [r4, #0x7c]
007cf970: str      r6, [r4, #0x80]
007cf974: mov      r0, r4
007cf978: pop      {r4, r5, r6, pc}
007cf97c: andseq   r5, ip, r4, lsr #3
007cf980: muleq    r0, ip, pc

# _ZN7gameswf19edit_text_character14preload_glyphsEv
0078c2d0: push     {r4, r5, r6, lr}
0078c2d4: ldr      r3, [r0, #0x50]
0078c2d8: mov      r6, r0
0078c2dc: ldr      r2, [r3, #8]
0078c2e0: cmp      r2, #0
0078c2e4: ble      #0x78c318
0078c2e8: mov      r4, #0
0078c2ec: mov      r5, r4
0078c2f0: ldr      r1, [r3, #4]
0078c2f4: mov      r0, r6
0078c2f8: add      r5, r5, #1
0078c2fc: add      r1, r1, r4
0078c300: bl       #0x78c0ec
0078c304: ldr      r3, [r6, #0x50]
0078c308: add      r4, r4, #0x2c
0078c30c: ldr      r2, [r3, #8]
0078c310: cmp      r5, r2
0078c314: blt      #0x78c2f0
0078c318: mov      r0, r6
0078c31c: mov      r1, #0
0078c320: pop      {r4, r5, r6, lr}
0078c324: b        #0x78c0ec

# _ZN7gameswf19edit_text_character8set_textERKNS_9tu_stringEb
0078f1ec: push     {r4, r5, r6, r7, r8, lr}
0078f1f0: add      r6, r0, #0x138
0078f1f4: cmp      r6, r1
0078f1f8: mov      r4, r0
0078f1fc: mov      r5, r1
0078f200: mov      r7, r2
0078f204: beq      #0x78f234
0078f208: ldrb     r3, [r0, #0x138]
0078f20c: cmp      r3, #0xff
0078f210: ldrsb    r3, [r1]
0078f214: addne    r0, r6, #1
0078f218: ldreq    r0, [r4, #0x144]
0078f21c: cmn      r3, #1
0078f220: addne    r1, r1, #1
0078f224: ldreq    r1, [r5, #0xc]
0078f228: bl       #0x30e31c
0078f22c: cmp      r0, #0
0078f230: bne      #0x78f238
0078f234: pop      {r4, r5, r6, r7, r8, pc}
0078f238: mov      r1, r5
0078f23c: mov      r0, r6
0078f240: bl       #0x752f50
0078f244: ldr      r3, [r4, #0xa0]
0078f248: ldr      r1, [r3, #0x64]
0078f24c: cmp      r1, #0
0078f250: ble      #0x78f278
0078f254: ldrb     r3, [r4, #0x138]
0078f258: sxtb     r3, r3
0078f25c: cmn      r3, #1
0078f260: ldreq    r3, [r4, #0x13c]
0078f264: sub      r3, r3, #1
0078f268: cmp      r1, r3
0078f26c: bge      #0x78f278
0078f270: mov      r0, r6
0078f274: bl       #0x751d14
0078f278: mov      r0, r4
0078f27c: mov      r1, r7
0078f280: pop      {r4, r5, r6, r7, r8, lr}
0078f284: b        #0x78efb8

# _ZN7gameswf19edit_text_character14set_text_valueERKNS_9tu_stringEb
00790ab0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00790ab4: ldr      r4, [pc, #0x1e0]
00790ab8: ldr      r6, [pc, #0x1e0]
00790abc: sub      sp, sp, #0x50
00790ac0: add      r4, pc, r4
00790ac4: ldr      r3, [r4, r6]
00790ac8: mov      r5, r0
00790acc: mov      sl, r1
00790ad0: ldr      r3, [r3]
00790ad4: str      r3, [sp, #0x4c]
00790ad8: bl       #0x78f1ec
00790adc: mov      r0, r5
00790ae0: bl       #0x78a364
00790ae4: ldrsb    r3, [r0]
00790ae8: cmn      r3, #1
00790aec: ldreq    r3, [r0, #4]
00790af0: sub      r3, r3, #1
00790af4: cmp      r3, #0
00790af8: ble      #0x790c1c
00790afc: ldr      r7, [r5, #0x40]
00790b00: cmp      r7, #0
00790b04: beq      #0x790b18
00790b08: ldr      r0, [r5, #0x3c]
00790b0c: ldrb     r3, [r0, #4]
00790b10: cmp      r3, #0
00790b14: beq      #0x790c38
00790b18: ldr      r3, [sp, #0x48]
00790b1c: mvn      r1, #0
00790b20: mov      r2, #0
00790b24: bfi      r3, r1, #0, #0x18
00790b28: lsr      r1, r3, #0x18
00790b2c: bfi      r1, r2, #0, #1
00790b30: mov      ip, #1
00790b34: mov      r0, r5
00790b38: str      r3, [sp, #0x48]
00790b3c: strb     ip, [sp, #0x38]
00790b40: strb     r2, [sp, #0x39]
00790b44: strb     r1, [sp, #0x4b]
00790b48: bl       #0x78a364
00790b4c: add      r8, sp, #0x24
00790b50: mov      r1, r0
00790b54: mov      r0, r8
00790b58: bl       #0x75302c
00790b5c: mov      r0, r5
00790b60: add      r5, sp, #0x38
00790b64: bl       #0x78a364
00790b68: mov      r1, r5
00790b6c: mov      r2, r8
00790b70: bl       #0x7cd130
00790b74: cmp      r0, #0
00790b78: beq      #0x790b98
00790b7c: ldrsb    r3, [sp, #0x38]
00790b80: mov      r0, r7
00790b84: cmn      r3, #1
00790b88: addne    r1, r5, #1
00790b8c: ldreq    r1, [sp, #0x44]
00790b90: bl       #0x76b284
00790b94: mov      r7, r0
00790b98: cmp      r7, #0
00790b9c: beq      #0x790c04
00790ba0: ldr      r3, [r7]
00790ba4: add      sb, sp, #0x10
00790ba8: mov      r1, r8
00790bac: mov      r0, sb
00790bb0: ldr      r8, [r3, #0x1c]
00790bb4: bl       #0x75302c
00790bb8: ldrsb    r3, [sl]
00790bbc: add      r5, sp, #4
00790bc0: mov      r0, r5
00790bc4: cmn      r3, #1
00790bc8: ldreq    r1, [sl, #0xc]
00790bcc: mov      r3, #0
00790bd0: addne    r1, sl, #1
00790bd4: strb     r3, [sp, #5]
00790bd8: strb     r3, [sp, #4]
00790bdc: bl       #0x797350
00790be0: mov      r1, sb
00790be4: mov      r2, r5
00790be8: mov      r0, r7
00790bec: blx      r8
00790bf0: mov      r0, r5
00790bf4: bl       #0x797124
00790bf8: ldrsb    r3, [sp, #0x10]
00790bfc: cmn      r3, #1
00790c00: beq      #0x790c60
00790c04: ldrsb    r3, [sp, #0x24]
00790c08: cmn      r3, #1
00790c0c: beq      #0x790c70
00790c10: ldrsb    r3, [sp, #0x38]
00790c14: cmn      r3, #1
00790c18: beq      #0x790c88
00790c1c: ldr      r3, [r4, r6]
00790c20: ldr      r2, [sp, #0x4c]
00790c24: ldr      r3, [r3]
00790c28: cmp      r2, r3
00790c2c: bne      #0x790c98
00790c30: add      sp, sp, #0x50
00790c34: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00790c38: ldr      r1, [r0]
00790c3c: sub      r1, r1, #1
00790c40: cmp      r1, #0
00790c44: str      r1, [r0]
00790c48: bne      #0x790c50
00790c4c: bl       #0x752b38
00790c50: mov      r7, #0
00790c54: str      r7, [r5, #0x3c]
00790c58: str      r7, [r5, #0x40]
00790c5c: b        #0x790b18
00790c60: ldr      r0, [sp, #0x1c]
00790c64: ldr      r1, [sp, #0x18]
00790c68: bl       #0x752b38
00790c6c: b        #0x790c04
00790c70: ldr      r0, [sp, #0x30]
00790c74: ldr      r1, [sp, #0x2c]
00790c78: bl       #0x752b38
00790c7c: ldrsb    r3, [sp, #0x38]
00790c80: cmn      r3, #1
00790c84: bne      #0x790c1c
00790c88: ldr      r0, [sp, #0x44]
00790c8c: ldr      r1, [sp, #0x40]
00790c90: bl       #0x752b38
00790c94: b        #0x790c1c
00790c98: bl       #0x30e310

# _ZN7gameswf19edit_text_character12reset_formatEPNS_13as_textformatE
0078f288: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0078f28c: ldr      r7, [pc, #0x718]
0078f290: ldr      r8, [pc, #0x718]
0078f294: ldr      r3, [r1]
0078f298: add      r7, pc, r7
0078f29c: ldr      r2, [r7, r8]
0078f2a0: mov      r4, r1
0078f2a4: sub      sp, sp, #0x10c
0078f2a8: ldr      r1, [r2]
0078f2ac: add      sb, sp, #0xf0
0078f2b0: mov      r2, #0
0078f2b4: str      r1, [sp, #0x104]
0078f2b8: ldr      r1, [pc, #0x6f4]
0078f2bc: strb     r2, [sp, #9]
0078f2c0: strb     r2, [sp, #8]
0078f2c4: add      r1, pc, r1
0078f2c8: mov      r6, r0
0078f2cc: add      r5, sp, #8
0078f2d0: mov      r0, sb
0078f2d4: ldr      sl, [r3, #0x20]
0078f2d8: bl       #0x413a7c
0078f2dc: mov      r1, sb
0078f2e0: mov      r0, r4
0078f2e4: mov      r2, r5
0078f2e8: blx      sl
0078f2ec: ldrsb    r3, [sp, #0xf0]
0078f2f0: mov      sl, r0
0078f2f4: cmn      r3, #1
0078f2f8: beq      #0x78f8e8
0078f2fc: cmp      sl, #0
0078f300: bne      #0x78f6b0
0078f304: ldr      r1, [pc, #0x6ac]
0078f308: ldr      r3, [r4]
0078f30c: add      sb, sp, #0xdc
0078f310: add      r1, pc, r1
0078f314: mov      r0, sb
0078f318: ldr      sl, [r3, #0x20]
0078f31c: bl       #0x413a7c
0078f320: mov      r0, r4
0078f324: mov      r1, sb
0078f328: mov      r2, r5
0078f32c: blx      sl
0078f330: ldrsb    r3, [sp, #0xdc]
0078f334: mov      sl, r0
0078f338: cmn      r3, #1
0078f33c: beq      #0x78f928
0078f340: cmp      sl, #0
0078f344: bne      #0x78f6f0
0078f348: ldr      r1, [pc, #0x66c]
0078f34c: ldr      r3, [r4]
0078f350: add      sb, sp, #0xc8
0078f354: add      r1, pc, r1
0078f358: mov      r0, sb
0078f35c: ldr      sl, [r3, #0x20]
0078f360: bl       #0x413a7c
0078f364: mov      r0, r4
0078f368: mov      r1, sb
0078f36c: mov      r2, r5
0078f370: blx      sl
0078f374: ldrsb    r3, [sp, #0xc8]
0078f378: mov      sl, r0
0078f37c: cmn      r3, #1
0078f380: beq      #0x78f8f8
0078f384: cmp      sl, #0
0078f388: bne      #0x78f6d0
0078f38c: ldr      r1, [pc, #0x62c]
0078f390: ldr      r3, [r4]
0078f394: add      sb, sp, #0xb4
0078f398: add      r1, pc, r1
0078f39c: mov      r0, sb
0078f3a0: ldr      sl, [r3, #0x20]
0078f3a4: bl       #0x413a7c
0078f3a8: mov      r0, r4
0078f3ac: mov      r1, sb
0078f3b0: mov      r2, r5
0078f3b4: blx      sl
0078f3b8: ldrsb    r3, [sp, #0xb4]
0078f3bc: mov      sl, r0
0078f3c0: cmn      r3, #1
0078f3c4: beq      #0x78f908
0078f3c8: cmp      sl, #0
0078f3cc: bne      #0x78f89c
0078f3d0: ldr      r1, [pc, #0x5ec]
0078f3d4: ldr      r3, [r4]
0078f3d8: add      sb, sp, #0xa0
0078f3dc: add      r1, pc, r1
0078f3e0: mov      r0, sb
0078f3e4: ldr      sl, [r3, #0x20]
0078f3e8: bl       #0x413a7c
0078f3ec: mov      r0, r4
0078f3f0: mov      r1, sb
0078f3f4: mov      r2, r5
0078f3f8: blx      sl
0078f3fc: ldrsb    r3, [sp, #0xa0]
0078f400: mov      sl, r0
0078f404: cmn      r3, #1
0078f408: beq      #0x78f978
0078f40c: cmp      sl, #0
0078f410: bne      #0x78f87c
0078f414: ldr      r1, [pc, #0x5ac]
0078f418: ldr      r3, [r4]
0078f41c: add      sb, sp, #0x8c
0078f420: add      r1, pc, r1
0078f424: mov      r0, sb
0078f428: ldr      sl, [r3, #0x20]
0078f42c: bl       #0x413a7c
0078f430: mov      r0, r4
0078f434: mov      r1, sb
0078f438: mov      r2, r5
0078f43c: blx      sl
0078f440: ldrsb    r3, [sp, #0x8c]
0078f444: mov      sl, r0
0078f448: cmn      r3, #1
0078f44c: beq      #0x78f988
0078f450: cmp      sl, #0
0078f454: bne      #0x78f850
0078f458: ldr      r1, [pc, #0x56c]
0078f45c: ldr      r3, [r4]
0078f460: add      sb, sp, #0x78
0078f464: add      r1, pc, r1
0078f468: mov      r0, sb
0078f46c: ldr      sl, [r3, #0x20]
0078f470: bl       #0x413a7c
0078f474: mov      r0, r4
0078f478: mov      r1, sb
0078f47c: mov      r2, r5
0078f480: blx      sl
0078f484: ldrsb    r3, [sp, #0x78]
0078f488: mov      sl, r0
0078f48c: cmn      r3, #1
0078f490: beq      #0x78f918
0078f494: cmp      sl, #0
0078f498: bne      #0x78f830
0078f49c: ldr      r1, [pc, #0x52c]
0078f4a0: ldr      r3, [r4]
0078f4a4: add      sb, sp, #0x64
0078f4a8: add      r1, pc, r1
0078f4ac: mov      r0, sb
0078f4b0: ldr      sl, [r3, #0x20]
0078f4b4: bl       #0x413a7c
0078f4b8: mov      r0, r4
0078f4bc: mov      r1, sb
0078f4c0: mov      r2, r5
0078f4c4: blx      sl
0078f4c8: ldrsb    r3, [sp, #0x64]
0078f4cc: mov      sl, r0
0078f4d0: cmn      r3, #1
0078f4d4: beq      #0x78f968
0078f4d8: cmp      sl, #0
0078f4dc: bne      #0x78f794
0078f4e0: ldr      r1, [r6, #0x178]
0078f4e4: add      sb, sp, #0x50
0078f4e8: mov      r0, sb
0078f4ec: add      r1, r1, #0x30
0078f4f0: bl       #0x75302c
0078f4f4: ldr      r1, [pc, #0x4d8]
0078f4f8: ldr      r3, [r4]
0078f4fc: add      fp, sp, #0x3c
0078f500: add      r1, pc, r1
0078f504: mov      r0, fp
0078f508: ldr      sl, [r3, #0x20]
0078f50c: bl       #0x413a7c
0078f510: mov      r0, r4
0078f514: mov      r1, fp
0078f518: mov      r2, r5
0078f51c: blx      sl
0078f520: ldrsb    r3, [sp, #0x3c]
0078f524: mov      sl, r0
0078f528: cmn      r3, #1
0078f52c: beq      #0x78f998
0078f530: cmp      sl, #0
0078f534: bne      #0x78f77c
0078f538: ldr      r3, [r6, #0x178]
0078f53c: ldr      r1, [pc, #0x494]
0078f540: ldr      r2, [r4]
0078f544: ldrb     r3, [r3, #0x4d]
0078f548: add      fp, sp, #0x28
0078f54c: add      r1, pc, r1
0078f550: mov      r0, fp
0078f554: ldr      sl, [r2, #0x20]
0078f558: str      r3, [sp, #4]
0078f55c: bl       #0x413a7c
0078f560: mov      r0, r4
0078f564: mov      r1, fp
0078f568: mov      r2, r5
0078f56c: blx      sl
0078f570: ldrsb    r3, [sp, #0x28]
0078f574: mov      sl, r0
0078f578: cmn      r3, #1
0078f57c: beq      #0x78f948
0078f580: cmp      sl, #0
0078f584: bne      #0x78f76c
0078f588: ldr      r3, [r4]
0078f58c: ldr      r1, [pc, #0x448]
0078f590: ldr      r2, [r6, #0x178]
0078f594: ldr      r3, [r3, #0x20]
0078f598: add      fp, sp, #0x14
0078f59c: add      r1, pc, r1
0078f5a0: mov      r0, fp
0078f5a4: ldrb     sl, [r2, #0x4c]
0078f5a8: str      r3, [sp]
0078f5ac: bl       #0x413a7c
0078f5b0: mov      r0, r4
0078f5b4: ldr      r3, [sp]
0078f5b8: mov      r1, fp
0078f5bc: mov      r2, r5
0078f5c0: blx      r3
0078f5c4: ldrsb    r3, [sp, #0x14]
0078f5c8: mov      r4, r0
0078f5cc: cmn      r3, #1
0078f5d0: beq      #0x78f958
0078f5d4: cmp      r4, #0
0078f5d8: bne      #0x78f710
0078f5dc: ldr      r1, [r6, #0x178]
0078f5e0: ldrb     r3, [r1, #0x4c]
0078f5e4: cmp      r3, sl
0078f5e8: beq      #0x78f72c
0078f5ec: ldr      r3, [r6]
0078f5f0: mov      r0, r6
0078f5f4: mov      r1, sb
0078f5f8: mov      lr, pc
0078f5fc: ldr      pc, [r3, #0x84]
0078f600: subs     r4, r0, #0
0078f604: beq      #0x78f620
0078f608: ldr      r3, [r4]
0078f60c: mov      r1, #0x13
0078f610: mov      lr, pc
0078f614: ldr      pc, [r3, #8]
0078f618: cmp      r0, #0
0078f61c: bne      #0x78f8bc
0078f620: mov      r0, r6
0078f624: bl       #0x780374
0078f628: mov      r1, #0
0078f62c: mov      r4, r0
0078f630: mov      r0, #0x88
0078f634: bl       #0x752ba8
0078f638: mov      r1, r4
0078f63c: mov      fp, r0
0078f640: bl       #0x7cf8d8
0078f644: mov      r1, fp
0078f648: add      r0, r6, #0x178
0078f64c: bl       #0x764234
0078f650: ldr      r3, [r6, #0x178]
0078f654: ldr      r2, [sp, #4]
0078f658: mov      r1, sb
0078f65c: strb     r2, [r3, #0x4d]
0078f660: ldr      r3, [r6, #0x178]
0078f664: strb     sl, [r3, #0x4c]
0078f668: ldr      r0, [r6, #0x178]
0078f66c: add      r0, r0, #0x30
0078f670: bl       #0x752f50
0078f674: mov      r0, r6
0078f678: mov      r1, #0
0078f67c: bl       #0x78efb8
0078f680: ldrsb    r3, [sp, #0x50]
0078f684: cmn      r3, #1
0078f688: beq      #0x78f938
0078f68c: mov      r0, r5
0078f690: bl       #0x797124
0078f694: ldr      r3, [r7, r8]
0078f698: ldr      r2, [sp, #0x104]
0078f69c: ldr      r3, [r3]
0078f6a0: cmp      r2, r3
0078f6a4: bne      #0x78f9a8
0078f6a8: add      sp, sp, #0x10c
0078f6ac: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0078f6b0: mov      r0, r5
0078f6b4: bl       #0x797a54
0078f6b8: bl       #0x30e6a0
0078f6bc: mov      r1, #0x41000000
0078f6c0: add      r1, r1, #0xa00000
0078f6c4: bl       #0x30ed6c
0078f6c8: str      r0, [r6, #0x180]
0078f6cc: b        #0x78f304
0078f6d0: mov      r0, r5
0078f6d4: bl       #0x797a54
0078f6d8: bl       #0x30e6a0
0078f6dc: mov      r1, #0x41000000
0078f6e0: add      r1, r1, #0xa00000
0078f6e4: bl       #0x30ed6c
0078f6e8: str      r0, [r6, #0x184]
0078f6ec: b        #0x78f38c
0078f6f0: mov      r0, r5
0078f6f4: bl       #0x797a54
0078f6f8: bl       #0x30e6a0
0078f6fc: mov      r1, #0x41000000
0078f700: add      r1, r1, #0xa00000
0078f704: bl       #0x30ed6c
0078f708: str      r0, [r6, #0x188]
0078f70c: b        #0x78f348
0078f710: mov      r0, r5
0078f714: bl       #0x797960
0078f718: ldr      r1, [r6, #0x178]
0078f71c: mov      sl, r0
0078f720: ldrb     r3, [r1, #0x4c]
0078f724: cmp      r3, sl
0078f728: bne      #0x78f5ec
0078f72c: ldrb     r3, [r1, #0x4d]
0078f730: ldr      r2, [sp, #4]
0078f734: cmp      r3, r2
0078f738: bne      #0x78f5ec
0078f73c: ldrsb    r3, [sp, #0x50]
0078f740: cmn      r3, #1
0078f744: ldrsb    r3, [r1, #0x30]
0078f748: addne    r0, sb, #1
0078f74c: ldreq    r0, [sp, #0x5c]
0078f750: cmn      r3, #1
0078f754: addne    r1, r1, #0x31
0078f758: ldreq    r1, [r1, #0x3c]
0078f75c: bl       #0x30e31c
0078f760: cmp      r0, #0
0078f764: beq      #0x78f674
0078f768: b        #0x78f5ec
0078f76c: mov      r0, r5
0078f770: bl       #0x797960
0078f774: str      r0, [sp, #4]
0078f778: b        #0x78f588
0078f77c: mov      r0, r5
0078f780: bl       #0x420a84
0078f784: mov      r1, r0
0078f788: mov      r0, sb
0078f78c: bl       #0x752f50
0078f790: b        #0x78f538
0078f794: mov      r0, r5
0078f798: bl       #0x420a84
0078f79c: ldrsb    r3, [r0]
0078f7a0: ldr      r1, [pc, #0x238]
0078f7a4: cmn      r3, #1
0078f7a8: ldreq    r0, [r0, #0xc]
0078f7ac: addne    r0, r0, #1
0078f7b0: add      r1, pc, r1
0078f7b4: bl       #0x30e31c
0078f7b8: cmp      r0, #0
0078f7bc: streq    r0, [r6, #0x17c]
0078f7c0: beq      #0x78f4e0
0078f7c4: mov      r0, r5
0078f7c8: bl       #0x420a84
0078f7cc: ldr      r1, [pc, #0x210]
0078f7d0: add      r1, pc, r1
0078f7d4: bl       #0x78aebc
0078f7d8: cmp      r0, #0
0078f7dc: movne    r3, #2
0078f7e0: strne    r3, [r6, #0x17c]
0078f7e4: bne      #0x78f4e0
0078f7e8: mov      r0, r5
0078f7ec: bl       #0x420a84
0078f7f0: ldr      r1, [pc, #0x1f0]
0078f7f4: add      r1, pc, r1
0078f7f8: bl       #0x78aebc
0078f7fc: cmp      r0, #0
0078f800: movne    r3, #1
0078f804: strne    r3, [r6, #0x17c]
0078f808: bne      #0x78f4e0
0078f80c: mov      r0, r5
0078f810: bl       #0x420a84
0078f814: ldr      r1, [pc, #0x1d0]
0078f818: add      r1, pc, r1
0078f81c: bl       #0x78aebc
0078f820: cmp      r0, #0
0078f824: movne    r3, #3
0078f828: strne    r3, [r6, #0x17c]
0078f82c: b        #0x78f4e0
0078f830: mov      r0, r5
0078f834: bl       #0x797a54
0078f838: bl       #0x30e6a0
0078f83c: mov      r1, #0x41000000
0078f840: add      r1, r1, #0xa00000
0078f844: bl       #0x30ed6c
0078f848: str      r0, [r6, #0x174]
0078f84c: b        #0x78f49c
0078f850: mov      r0, r5
0078f854: bl       #0x797a54
0078f858: bl       #0x30ea24
0078f85c: asr      r3, r0, #8
0078f860: asr      r2, r0, #0x10
0078f864: strb     r3, [r6, #0x171]
0078f868: mvn      r3, #0
0078f86c: strb     r2, [r6, #0x170]
0078f870: strb     r0, [r6, #0x172]
0078f874: strb     r3, [r6, #0x173]
0078f878: b        #0x78f458
0078f87c: mov      r0, r5
0078f880: bl       #0x797a54
0078f884: bl       #0x30e6a0
0078f888: mov      r1, #0x41000000
0078f88c: add      r1, r1, #0xa00000
0078f890: bl       #0x30ed6c
0078f894: str      r0, [r6, #0x190]
0078f898: b        #0x78f414
0078f89c: mov      r0, r5
0078f8a0: bl       #0x797a54
0078f8a4: bl       #0x30e6a0
0078f8a8: mov      r1, #0x41000000
0078f8ac: add      r1, r1, #0xa00000
0078f8b0: bl       #0x30ed6c
0078f8b4: str      r0, [r6, #0x18c]
0078f8b8: b        #0x78f3d0
0078f8bc: mov      r1, #0x13
0078f8c0: ldr      r3, [r4]
0078f8c4: mov      r0, r4
0078f8c8: mov      lr, pc
0078f8cc: ldr      pc, [r3, #8]
0078f8d0: cmp      r0, #0
0078f8d4: movne    r1, r4
0078f8d8: moveq    r1, #0
0078f8dc: add      r0, r6, #0x178
0078f8e0: bl       #0x764234
0078f8e4: b        #0x78f650
0078f8e8: ldr      r0, [sp, #0xfc]
0078f8ec: ldr      r1, [sp, #0xf8]
0078f8f0: bl       #0x752b38
0078f8f4: b        #0x78f2fc
0078f8f8: ldr      r0, [sp, #0xd4]
0078f8fc: ldr      r1, [sp, #0xd0]
0078f900: bl       #0x752b38
0078f904: b        #0x78f384
0078f908: ldr      r0, [sp, #0xc0]
0078f90c: ldr      r1, [sp, #0xbc]
0078f910: bl       #0x752b38
0078f914: b        #0x78f3c8
0078f918: ldr      r0, [sp, #0x84]
0078f91c: ldr      r1, [sp, #0x80]
0078f920: bl       #0x752b38
0078f924: b        #0x78f494
0078f928: ldr      r0, [sp, #0xe8]
0078f92c: ldr      r1, [sp, #0xe4]
0078f930: bl       #0x752b38
0078f934: b        #0x78f340
0078f938: ldr      r0, [sp, #0x5c]
0078f93c: ldr      r1, [sp, #0x58]
0078f940: bl       #0x752b38
0078f944: b        #0x78f68c
0078f948: ldr      r0, [sp, #0x34]
0078f94c: ldr      r1, [sp, #0x30]
0078f950: bl       #0x752b38
0078f954: b        #0x78f580
0078f958: ldr      r0, [sp, #0x20]
0078f95c: ldr      r1, [sp, #0x1c]
0078f960: bl       #0x752b38
0078f964: b        #0x78f5d4
0078f968: ldr      r0, [sp, #0x70]
0078f96c: ldr      r1, [sp, #0x6c]
0078f970: bl       #0x752b38
0078f974: b        #0x78f4d8
0078f978: ldr      r0, [sp, #0xac]
0078f97c: ldr      r1, [sp, #0xa8]
0078f980: bl       #0x752b38
0078f984: b        #0x78f40c
0078f988: ldr      r0, [sp, #0x98]
0078f98c: ldr      r1, [sp, #0x94]
0078f990: bl       #0x752b38
0078f994: b        #0x78f450
0078f998: ldr      r0, [sp, #0x48]
0078f99c: ldr      r1, [sp, #0x44]
0078f9a0: bl       #0x752b38
0078f9a4: b        #0x78f530
0078f9a8: bl       #0x30e310

