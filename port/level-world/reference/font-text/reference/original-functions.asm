
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

# _ZN7gameswf26default_bitmap_font_entityC1EPNS_21bitmap_glyph_providerERKNS_9tu_stringE
007c68c4: push     {r4, r5, r6, r7, r8, lr}
007c68c8: ldr      r6, [pc, #0x270]
007c68cc: mov      r4, r0
007c68d0: mov      r7, r2
007c68d4: mov      r5, r1
007c68d8: bl       #0x759c04
007c68dc: ldr      r2, [pc, #0x260]
007c68e0: ldr      r3, [r4, #0x20]
007c68e4: add      r6, pc, r6
007c68e8: ldr      r2, [r6, r2]
007c68ec: mvn      r1, #0
007c68f0: bfi      r3, r1, #0, #0x18
007c68f4: mov      r8, #0
007c68f8: lsr      r1, r3, #0x18
007c68fc: add      r2, r2, #8
007c6900: bfi      r1, r8, #0, #1
007c6904: mov      r0, #1
007c6908: str      r2, [r4]
007c690c: str      r3, [r4, #0x20]
007c6910: str      r5, [r4, #0xc]
007c6914: strb     r1, [r4, #0x23]
007c6918: strb     r0, [r4, #0x10]
007c691c: strb     r8, [r4, #0x11]
007c6920: str      r8, [r4, #0x24]
007c6924: str      r8, [r4, #0x2c]
007c6928: str      r8, [r4, #0x30]
007c692c: str      r8, [r4, #0x34]
007c6930: strb     r8, [r4, #0x38]
007c6934: add      r0, r4, #0x3c
007c6938: add      r5, r4, #0x4c
007c693c: bl       #0x7b628c
007c6940: mov      r0, r5
007c6944: bl       #0x7b628c
007c6948: mov      r1, r7
007c694c: add      r0, r4, #0x10
007c6950: str      r8, [r4, #0x60]
007c6954: str      r8, [r4, #0x5c]
007c6958: bl       #0x752f50
007c695c: mov      r3, #0x3f800000
007c6960: str      r3, [r4, #0x28]
007c6964: ldrsb    r3, [r7]
007c6968: mov      r1, #0
007c696c: mov      r0, #0x28
007c6970: cmn      r3, #1
007c6974: addne    r7, r7, #1
007c6978: ldreq    r7, [r7, #0xc]
007c697c: bl       #0x752ba8
007c6980: ldr      r2, [pc, #0x1c0]
007c6984: mov      r6, r0
007c6988: mov      r1, r7
007c698c: add      r2, pc, r2
007c6990: bl       #0x7b68c8
007c6994: str      r6, [r4, #0x60]
007c6998: ldr      r7, [r6]
007c699c: cmp      r7, #0
007c69a0: beq      #0x7c6b20
007c69a4: mov      r0, r7
007c69a8: mov      lr, pc
007c69ac: ldr      pc, [r6, #0x14]
007c69b0: ldr      r3, [r4, #0x60]
007c69b4: ldr      r0, [r3]
007c69b8: mov      lr, pc
007c69bc: ldr      pc, [r3, #0x18]
007c69c0: ldr      r3, [r4, #0x60]
007c69c4: mov      r6, r0
007c69c8: mov      r0, #0
007c69cc: ldr      r1, [r3]
007c69d0: mov      lr, pc
007c69d4: ldr      pc, [r3, #0x10]
007c69d8: mov      r0, r5
007c69dc: mov      r1, #0x28
007c69e0: bl       #0x75ae5c
007c69e4: mvn      r2, #0
007c69e8: mov      r1, r5
007c69ec: ldr      r0, [r4, #0x60]
007c69f0: bl       #0x7b6a80
007c69f4: ldr      r7, [r4, #0x54]
007c69f8: ldrb     r3, [r7, #0x1d]
007c69fc: ldrb     r1, [r7, #0x1c]
007c6a00: ldrb     r2, [r7, #0x1f]
007c6a04: ldrb     r0, [r7, #0x1e]
007c6a08: lsl      r3, r3, #0x10
007c6a0c: orr      r3, r3, r1, lsl #24
007c6a10: orr      r3, r3, r2
007c6a14: orr      r0, r3, r0, lsl #8
007c6a18: bl       #0x30e2e0
007c6a1c: mov      r1, #0x41000000
007c6a20: add      r1, r1, #0xa00000
007c6a24: mov      r8, r0
007c6a28: bl       #0x30ed6c
007c6a2c: mov      r1, r0
007c6a30: mov      r0, #0x44000000
007c6a34: add      r0, r0, #0x800000
007c6a38: bl       #0x30ec94
007c6a3c: mov      r1, r0
007c6a40: mov      r0, r8
007c6a44: bl       #0x30ed6c
007c6a48: str      r0, [r4, #0x28]
007c6a4c: ldrb     r1, [r7, #0xd]
007c6a50: ldrb     r0, [r7, #0xc]
007c6a54: ldrb     r2, [r7, #0xf]
007c6a58: ldrb     r3, [r7, #0xe]
007c6a5c: lsl      r1, r1, #0x10
007c6a60: orr      r1, r1, r0, lsl #24
007c6a64: orr      r1, r1, r2
007c6a68: orr      r1, r1, r3, lsl #8
007c6a6c: add      r1, r1, #0xb
007c6a70: lsl      r1, r1, #2
007c6a74: mov      r0, r5
007c6a78: bl       #0x75ae5c
007c6a7c: ldr      r3, [r4, #0x60]
007c6a80: mov      r0, #0
007c6a84: ldr      r1, [r3]
007c6a88: mov      lr, pc
007c6a8c: ldr      pc, [r3, #0x10]
007c6a90: mov      r1, r5
007c6a94: ldr      r0, [r4, #0x60]
007c6a98: mvn      r2, #0
007c6a9c: bl       #0x7b6a80
007c6aa0: ldr      r3, [r4, #0xc]
007c6aa4: ldrb     r3, [r3, #8]
007c6aa8: cmp      r3, #0
007c6aac: bne      #0x7c6ab8
007c6ab0: mov      r0, r4
007c6ab4: pop      {r4, r5, r6, r7, r8, pc}
007c6ab8: mov      r1, #0
007c6abc: mov      r0, #0x10
007c6ac0: bl       #0x752ba8
007c6ac4: mov      r5, r0
007c6ac8: bl       #0x7b628c
007c6acc: ldr      r1, [r4, #0x4c]
007c6ad0: mov      r0, r5
007c6ad4: str      r5, [r4, #0x5c]
007c6ad8: rsb      r1, r1, r6
007c6adc: bl       #0x75ae5c
007c6ae0: ldr      r0, [r4, #0x60]
007c6ae4: ldr      r1, [r4, #0x5c]
007c6ae8: mvn      r2, #0
007c6aec: bl       #0x7b6a80
007c6af0: ldr      r5, [r4, #0x60]
007c6af4: cmp      r5, #0
007c6af8: beq      #0x7c6b10
007c6afc: mov      r0, r5
007c6b00: bl       #0x7b69e8
007c6b04: mov      r0, r5
007c6b08: mov      r1, #0
007c6b0c: bl       #0x752b38
007c6b10: mov      r3, #0
007c6b14: str      r3, [r4, #0x60]
007c6b18: mov      r0, r4
007c6b1c: pop      {r4, r5, r6, r7, r8, pc}
007c6b20: mov      r0, r6
007c6b24: bl       #0x7b69e8
007c6b28: mov      r0, r6
007c6b2c: mov      r1, r7
007c6b30: bl       #0x752b38
007c6b34: str      r7, [r4, #0x60]
007c6b38: mov      r0, r4
007c6b3c: pop      {r4, r5, r6, r7, r8, pc}
007c6b40: andseq   lr, ip, ip, lsr #3
007c6b44: andeq    r3, r0, r8, ror #8
007c6b48: andeq    sb, pc, r4, lsl lr

# _ZN7gameswf19edit_text_character11format_textEb
0078efb8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0078efbc: mvn      r2, #0
0078efc0: mvn      r3, #0
0078efc4: mov      r4, r0
0078efc8: strd     r2, r3, [r0, #0xe0]
0078efcc: strd     r2, r3, [r0, #0xd8]
0078efd0: sub      sp, sp, #0x10
0078efd4: add      r0, r0, #0xa4
0078efd8: mov      r6, r1
0078efdc: mov      r1, #0
0078efe0: bl       #0x78bccc
0078efe4: mov      r5, #0
0078efe8: mov      r3, #0
0078efec: mvn      r2, #0
0078eff0: mov      r1, r3
0078eff4: str      r2, [r4, #0x16c]
0078eff8: str      r3, [r4, #0x15c]
0078effc: str      r3, [r4, #0x160]
0078f000: str      r5, [r4, #0x164]
0078f004: str      r5, [r4, #0x168]
0078f008: mov      r0, r4
0078f00c: mov      r2, r3
0078f010: bl       #0x78a370
0078f014: ldr      r1, [r4, #0x178]
0078f018: cmp      r1, r5
0078f01c: beq      #0x78f194
0078f020: cmp      r6, r5
0078f024: beq      #0x78f19c
0078f028: mov      r0, sp
0078f02c: mov      r1, r4
0078f030: str      r5, [sp]
0078f034: str      r5, [sp, #4]
0078f038: str      r5, [sp, #8]
0078f03c: strb     r5, [sp, #0xc]
0078f040: bl       #0x78e354
0078f044: mov      r0, sp
0078f048: mov      r1, r5
0078f04c: bl       #0x78bc0c
0078f050: mov      r0, sp
0078f054: mov      r1, r5
0078f058: mov      r6, sp
0078f05c: bl       #0x78a67c
0078f060: ldr      r3, [r4, #0x15c]
0078f064: mov      r0, r4
0078f068: ldr      r1, [r4, #0x17c]
0078f06c: ldr      r2, [r4, #0x164]
0078f070: bl       #0x78a398
0078f074: ldr      r3, [r4, #0xa0]
0078f078: ldrb     r5, [r3, #0x49]
0078f07c: cmp      r5, #0
0078f080: bne      #0x78f178
0078f084: ldr      r8, [r4, #0xa8]
0078f088: cmp      r8, #1
0078f08c: ble      #0x78f178
0078f090: ldr      r6, [r4, #0xa4]
0078f094: mov      r7, r5
0078f098: mov      sb, #0
0078f09c: add      r3, r6, r5
0078f0a0: ldrb     r2, [r3, #0x1d]
0078f0a4: mov      r1, sb
0078f0a8: add      r7, r7, #1
0078f0ac: cmp      r2, #0
0078f0b0: add      r5, r5, #0x30
0078f0b4: beq      #0x78f0f4
0078f0b8: ldr      sl, [r3, #0x14]
0078f0bc: mov      r0, sl
0078f0c0: bl       #0x30e2f8
0078f0c4: cmp      r0, #0
0078f0c8: beq      #0x78f0f4
0078f0cc: cmp      r7, r8
0078f0d0: beq      #0x78f100
0078f0d4: add      r3, r6, r5
0078f0d8: ldrb     r2, [r3, #0x1d]
0078f0dc: mov      sb, sl
0078f0e0: mov      r1, sb
0078f0e4: cmp      r2, #0
0078f0e8: add      r7, r7, #1
0078f0ec: add      r5, r5, #0x30
0078f0f0: bne      #0x78f0b8
0078f0f4: cmp      r7, r8
0078f0f8: mov      sl, sb
0078f0fc: bne      #0x78f0d4
0078f100: mov      r0, sl
0078f104: mov      r1, #0xbf000000
0078f108: bl       #0x30ed6c
0078f10c: mov      r1, #0xbf000000
0078f110: mov      r5, r0
0078f114: ldr      r0, [r6, #0x18]
0078f118: bl       #0x30ed6c
0078f11c: ldr      r1, [r6, #0x14]
0078f120: bl       #0x30eba4
0078f124: mov      r1, r0
0078f128: mov      r0, r5
0078f12c: bl       #0x30eba4
0078f130: mov      r5, #0
0078f134: mov      sl, r0
0078f138: mov      r7, r5
0078f13c: b        #0x78f144
0078f140: ldr      r6, [r4, #0xa4]
0078f144: add      r6, r6, r5
0078f148: ldrb     r3, [r6, #0x1d]
0078f14c: mov      r1, sl
0078f150: add      r7, r7, #1
0078f154: cmp      r3, #0
0078f158: add      r5, r5, #0x30
0078f15c: beq      #0x78f170
0078f160: ldr      r0, [r6, #0x14]
0078f164: bl       #0x30eba4
0078f168: str      r0, [r6, #0x14]
0078f16c: ldr      r8, [r4, #0xa8]
0078f170: cmp      r7, r8
0078f174: blt      #0x78f140
0078f178: mov      r0, r4
0078f17c: bl       #0x78dde8
0078f180: ldrb     r3, [r0, #0x87]
0078f184: cmp      r3, #0
0078f188: beq      #0x78f194
0078f18c: mov      r0, r4
0078f190: bl       #0x78c2d0
0078f194: add      sp, sp, #0x10
0078f198: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0078f19c: ldr      r3, [r4, #0x170]
0078f1a0: mov      r2, #0xc
0078f1a4: mov      r0, sp
0078f1a8: stmib    sp, {r2, r3}
0078f1ac: str      r6, [sp]
0078f1b0: strb     r6, [sp, #0xc]
0078f1b4: bl       #0x764234
0078f1b8: ldr      r0, [r4, #0x174]
0078f1bc: bl       #0x30e4cc
0078f1c0: mov      r2, sp
0078f1c4: str      r0, [sp, #4]
0078f1c8: mov      r3, r6
0078f1cc: mov      r0, r4
0078f1d0: add      r1, r4, #0x138
0078f1d4: bl       #0x78cb90
0078f1d8: ldr      r0, [sp]
0078f1dc: cmp      r0, #0
0078f1e0: beq      #0x78f060
0078f1e4: bl       #0x75a240
0078f1e8: b        #0x78f060

# _ZN8RenderFX13PreloadGlyphsEPKcS1_ibbPKN7gameswf6filterE
007ab0b8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ab0bc: sub      sp, sp, #0x2c
007ab0c0: ldrb     ip, [sp, #0x54]
007ab0c4: ldrb     r8, [sp, #0x50]
007ab0c8: str      r1, [sp, #0x14]
007ab0cc: str      ip, [sp, #0x10]
007ab0d0: mov      ip, #0
007ab0d4: strb     ip, [sp, #0x24]
007ab0d8: mov      r7, r0
007ab0dc: mov      r6, r2
007ab0e0: mov      r5, r3
007ab0e4: str      ip, [sp, #0x18]
007ab0e8: str      ip, [sp, #0x1c]
007ab0ec: str      ip, [sp, #0x20]
007ab0f0: add      fp, sp, #0x14
007ab0f4: add      r4, sp, #0x18
007ab0f8: b        #0x7ab10c
007ab0fc: ldr      r2, [sp, #0x18]
007ab100: lsl      r3, r3, #1
007ab104: strh     sb, [r2, r3]
007ab108: str      sl, [sp, #0x1c]
007ab10c: mov      r0, fp
007ab110: bl       #0x752494
007ab114: subs     sb, r0, #0
007ab118: beq      #0x7ab144
007ab11c: ldr      r3, [sp, #0x1c]
007ab120: ldr      r2, [sp, #0x20]
007ab124: add      sl, r3, #1
007ab128: cmp      sl, r2
007ab12c: ble      #0x7ab0fc
007ab130: mov      r0, r4
007ab134: add      r1, sl, sl, asr #1
007ab138: bl       #0x779e7c
007ab13c: ldr      r3, [sp, #0x1c]
007ab140: b        #0x7ab0fc
007ab144: ldr      r2, [sp, #0x1c]
007ab148: cmp      r2, #0
007ab14c: ble      #0x7ab1a4
007ab150: ldr      ip, [sp, #0x10]
007ab154: mov      r0, r7
007ab158: ldr      r1, [sp, #0x18]
007ab15c: str      ip, [sp, #8]
007ab160: ldr      ip, [sp, #0x58]
007ab164: mov      r3, r6
007ab168: stm      sp, {r5, r8}
007ab16c: str      ip, [sp, #0xc]
007ab170: bl       #0x7aafac
007ab174: ldr      r2, [sp, #0x1c]
007ab178: mov      sb, r0
007ab17c: cmp      r2, #0
007ab180: ble      #0x7ab1a4
007ab184: mov      r3, #0
007ab188: mov      r0, r4
007ab18c: mov      r1, r3
007ab190: str      r3, [sp, #0x1c]
007ab194: bl       #0x779e7c
007ab198: mov      r0, sb
007ab19c: add      sp, sp, #0x2c
007ab1a0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007ab1a4: cmp      r2, #0
007ab1a8: bge      #0x7ab184
007ab1ac: lsl      r3, r2, #1
007ab1b0: ldr      r1, [sp, #0x18]
007ab1b4: mov      r0, #0
007ab1b8: adds     r2, r2, #1
007ab1bc: strh     r0, [r1, r3]
007ab1c0: add      r3, r3, #2
007ab1c4: bne      #0x7ab1b0
007ab1c8: b        #0x7ab184

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

# _ZN7gameswf18bitmap_font_entity14get_char_imageEtiPNS_4rectEPf
007c59dc: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007c59e0: sub      sp, sp, #0x40
007c59e4: add      sl, r0, #0x24
007c59e8: add      r8, sp, #0x3c
007c59ec: mov      r4, r0
007c59f0: mov      r6, r1
007c59f4: mov      r5, r2
007c59f8: mov      r7, #0
007c59fc: orr      r2, r1, r2, lsl #16
007c5a00: mov      r0, sl
007c5a04: mov      r1, r8
007c5a08: str      r2, [sp, #0x3c]
007c5a0c: mov      sb, r3
007c5a10: str      r7, [sp, #0x38]
007c5a14: bl       #0x7c4418
007c5a18: cmp      r0, #0
007c5a1c: blt      #0x7c5a60
007c5a20: ldr      r3, [r4, #0x24]
007c5a24: add      r0, r3, r0, lsl #4
007c5a28: ldr      r3, [r0, #0x14]
007c5a2c: str      r3, [sp, #0x38]
007c5a30: mov      ip, r3
007c5a34: add      r3, r3, #8
007c5a38: ldm      r3, {r0, r1, r2, r3}
007c5a3c: stm      sb, {r0, r1, r2, r3}
007c5a40: ldr      r3, [sp, #0x60]
007c5a44: ldr      r2, [ip, #4]
007c5a48: str      r2, [r3]
007c5a4c: ldr      r3, [r4, #0xc]
007c5a50: ldr      r3, [r3, #0xc]
007c5a54: ldr      r0, [r3, #0x34]
007c5a58: add      sp, sp, #0x40
007c5a5c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007c5a60: add      r3, sp, #0xc
007c5a64: str      r3, [sp]
007c5a68: mov      r2, r6
007c5a6c: mov      r3, r5
007c5a70: ldr      ip, [r4]
007c5a74: mov      r0, r4
007c5a78: add      r1, sp, #0x20
007c5a7c: mov      lr, pc
007c5a80: ldr      pc, [ip, #8]
007c5a84: cmp      r0, #0
007c5a88: beq      #0x7c5a58
007c5a8c: ldr      r3, [r4, #0xc]
007c5a90: ldr      r5, [r3, #0xc]
007c5a94: cmp      r5, #0
007c5a98: beq      #0x7c5bf8
007c5a9c: mov      r1, r7
007c5aa0: mov      r0, #0x18
007c5aa4: bl       #0x752ba8
007c5aa8: mov      r2, #0
007c5aac: str      r7, [r0]
007c5ab0: str      r2, [r0, #0x14]
007c5ab4: str      r2, [r0, #4]
007c5ab8: str      r2, [r0, #8]
007c5abc: str      r2, [r0, #0xc]
007c5ac0: str      r2, [r0, #0x10]
007c5ac4: ldr      ip, [sp, #0x24]
007c5ac8: ldr      r2, [sp, #0x28]
007c5acc: mov      r3, r0
007c5ad0: add      ip, ip, #1
007c5ad4: add      r2, r2, #1
007c5ad8: add      r1, sp, #0x30
007c5adc: add      r0, sp, #0x34
007c5ae0: str      ip, [sp, #0x34]
007c5ae4: str      r2, [sp, #0x30]
007c5ae8: str      r3, [sp, #0x38]
007c5aec: bl       #0x793560
007c5af0: ldr      r0, [sp, #0x24]
007c5af4: bl       #0x30e964
007c5af8: mov      r5, r0
007c5afc: ldr      r0, [sp, #0x34]
007c5b00: bl       #0x30e964
007c5b04: mov      r1, r0
007c5b08: mov      r0, r5
007c5b0c: bl       #0x30ec94
007c5b10: ldr      r3, [sp, #0x38]
007c5b14: str      r0, [r3, #0xc]
007c5b18: ldr      r0, [sp, #0x28]
007c5b1c: bl       #0x30e964
007c5b20: mov      r5, r0
007c5b24: ldr      r0, [sp, #0x30]
007c5b28: bl       #0x30e964
007c5b2c: mov      r1, r0
007c5b30: mov      r0, r5
007c5b34: bl       #0x30ec94
007c5b38: ldr      r3, [sp, #0x38]
007c5b3c: str      r0, [r3, #0x14]
007c5b40: ldr      r0, [sp, #0xc]
007c5b44: rsb      r0, r0, #0
007c5b48: bl       #0x30e964
007c5b4c: mov      r5, r0
007c5b50: ldr      r0, [sp, #0x14]
007c5b54: bl       #0x30e964
007c5b58: mov      r1, r0
007c5b5c: mov      r0, r5
007c5b60: bl       #0x30ec94
007c5b64: ldr      r3, [sp, #0x38]
007c5b68: str      r0, [r3, #8]
007c5b6c: ldr      r0, [sp, #0x10]
007c5b70: bl       #0x30e964
007c5b74: mov      r5, r0
007c5b78: ldr      r0, [sp, #0x18]
007c5b7c: bl       #0x30e964
007c5b80: mov      r1, r0
007c5b84: mov      r0, r5
007c5b88: bl       #0x30ec94
007c5b8c: ldr      r3, [sp, #0x38]
007c5b90: str      r0, [r3, #0x10]
007c5b94: ldr      r5, [sp, #0x38]
007c5b98: ldr      r1, [r5, #0xc]
007c5b9c: ldr      r0, [r5, #8]
007c5ba0: add      r1, r1, #0x80000000
007c5ba4: bl       #0x30ed6c
007c5ba8: str      r0, [r5, #8]
007c5bac: ldr      r5, [sp, #0x38]
007c5bb0: ldr      r1, [r5, #0x14]
007c5bb4: ldr      r0, [r5, #0x10]
007c5bb8: bl       #0x30ed6c
007c5bbc: str      r0, [r5, #0x10]
007c5bc0: ldr      r0, [sp, #0x1c]
007c5bc4: bl       #0x30e964
007c5bc8: mov      r1, #0x41000000
007c5bcc: add      r1, r1, #0xa00000
007c5bd0: bl       #0x30ed6c
007c5bd4: ldr      r3, [sp, #0x38]
007c5bd8: mov      r1, r8
007c5bdc: add      r2, sp, #0x38
007c5be0: str      r0, [r3, #4]
007c5be4: mov      r0, sl
007c5be8: bl       #0x7c5878
007c5bec: ldr      ip, [sp, #0x38]
007c5bf0: mov      r3, ip
007c5bf4: b        #0x7c5a34
007c5bf8: ldr      r0, [pc, #0xc]
007c5bfc: add      r0, pc, r0
007c5c00: bl       #0x761184
007c5c04: mov      r0, r5
007c5c08: b        #0x7c5a58
007c5c0c: ldrsbeq  r5, [r4], -r4

# _ZN12GameSWFUtils12PreloadGlyphEPKcPN7gameswf9characterEP6MenuFX
0041724c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00417250: subs     r5, r1, #0
00417254: sub      sp, sp, #0x1c
00417258: mov      sb, r0
0041725c: str      r2, [sp, #0x14]
00417260: beq      #0x417378
00417264: ldr      r3, [r5]
00417268: mov      r0, r5
0041726c: mov      r1, #0x20
00417270: mov      lr, pc
00417274: ldr      pc, [r3, #8]
00417278: cmp      r0, #0
0041727c: beq      #0x417378
00417280: cmp      sb, #0
00417284: beq      #0x417384
00417288: ldr      r3, [r5, #0x50]
0041728c: ldr      r2, [r3, #8]
00417290: cmp      r2, #0
00417294: movle    r8, #0
00417298: ble      #0x41731c
0041729c: mov      r6, #0
004172a0: mov      r7, r6
004172a4: mov      r8, r6
004172a8: ldr      r4, [r5, #0x178]
004172ac: ldr      sl, [r3, #4]
004172b0: mov      r1, #0x41000000
004172b4: ldrsb    r3, [r4, #0x30]
004172b8: ldr      r0, [r5, #0x174]
004172bc: add      r1, r1, #0xa00000
004172c0: cmn      r3, #1
004172c4: add      fp, r4, #0x31
004172c8: ldreq    fp, [r4, #0x3c]
004172cc: bl       #0x30ec94
004172d0: bl       #0x30e4cc
004172d4: ldrb     r1, [r4, #0x4d]
004172d8: mov      r3, r0
004172dc: add      sl, sl, r6
004172e0: str      r1, [sp]
004172e4: ldrb     ip, [r4, #0x4c]
004172e8: mov      r2, fp
004172ec: ldr      r0, [sp, #0x14]
004172f0: mov      r1, sb
004172f4: str      ip, [sp, #4]
004172f8: str      sl, [sp, #8]
004172fc: bl       #0x7ab0b8
00417300: ldr      r3, [r5, #0x50]
00417304: add      r7, r7, #1
00417308: add      r8, r8, r0
0041730c: ldr      r2, [r3, #8]
00417310: add      r6, r6, #0x2c
00417314: cmp      r7, r2
00417318: blt      #0x4172a8
0041731c: ldr      r4, [r5, #0x178]
00417320: mov      r1, #0x41000000
00417324: ldr      r0, [r5, #0x174]
00417328: ldrsb    r3, [r4, #0x30]
0041732c: add      r1, r1, #0xa00000
00417330: cmn      r3, #1
00417334: ldreq    r6, [r4, #0x3c]
00417338: addne    r6, r4, #0x31
0041733c: bl       #0x30ec94
00417340: bl       #0x30e4cc
00417344: ldrb     r2, [r4, #0x4d]
00417348: mov      r3, r0
0041734c: mov      r1, sb
00417350: str      r2, [sp]
00417354: ldrb     ip, [r4, #0x4c]
00417358: ldr      r0, [sp, #0x14]
0041735c: mov      r2, r6
00417360: str      ip, [sp, #4]
00417364: mov      ip, #0
00417368: str      ip, [sp, #8]
0041736c: bl       #0x7ab0b8
00417370: add      r0, r0, r8
00417374: b        #0x41737c
00417378: mov      r0, #0
0041737c: add      sp, sp, #0x1c
00417380: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00417384: ldrb     r3, [r5, #0x138]
00417388: cmp      r3, #0xff
0041738c: addne    sb, r5, #0x138
00417390: addne    sb, sb, #1
00417394: ldreq    sb, [r5, #0x144]
00417398: b        #0x417288

# _ZN7gameswf4font4readEPNS_6streamEiPNS_20movie_definition_subE
007cfa30: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007cfa34: cmp      r2, #0xa
007cfa38: mov      r7, r0
007cfa3c: sub      sp, sp, #0x4c
007cfa40: mov      r5, r3
007cfa44: mov      r4, r1
007cfa48: str      r3, [r7, #0x44]
007cfa4c: beq      #0x7cfd90
007cfa50: cmp      r2, #0x30
007cfa54: cmpne    r2, #0x4b
007cfa58: beq      #0x7cfa64
007cfa5c: add      sp, sp, #0x4c
007cfa60: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007cfa64: mov      r1, #1
007cfa68: mov      r0, r4
007cfa6c: bl       #0x7839a4
007cfa70: mov      r1, #1
007cfa74: str      r0, [sp, #0x14]
007cfa78: mov      r0, r4
007cfa7c: bl       #0x7839a4
007cfa80: subs     r0, r0, #0
007cfa84: movne    r0, #1
007cfa88: strb     r0, [r7, #0x4a]
007cfa8c: mov      r1, #1
007cfa90: mov      r0, r4
007cfa94: bl       #0x7839a4
007cfa98: subs     r0, r0, #0
007cfa9c: movne    r0, #1
007cfaa0: strb     r0, [r7, #0x49]
007cfaa4: mov      r1, #1
007cfaa8: mov      r0, r4
007cfaac: bl       #0x7839a4
007cfab0: subs     r0, r0, #0
007cfab4: movne    r0, #1
007cfab8: strb     r0, [r7, #0x4b]
007cfabc: mov      r1, #1
007cfac0: mov      r0, r4
007cfac4: bl       #0x7839a4
007cfac8: mov      r1, #1
007cfacc: mov      r6, r0
007cfad0: mov      r0, r4
007cfad4: bl       #0x7839a4
007cfad8: subs     r0, r0, #0
007cfadc: movne    r0, #1
007cfae0: strb     r0, [r7, #0x4e]
007cfae4: mov      r1, #1
007cfae8: mov      r0, r4
007cfaec: bl       #0x7839a4
007cfaf0: subs     r0, r0, #0
007cfaf4: movne    r0, #1
007cfaf8: mov      r1, #1
007cfafc: strb     r0, [r7, #0x4c]
007cfb00: mov      r0, r4
007cfb04: bl       #0x7839a4
007cfb08: subs     r0, r0, #0
007cfb0c: movne    r0, #1
007cfb10: strb     r0, [r7, #0x4d]
007cfb14: mov      r0, r4
007cfb18: bl       #0x783b28
007cfb1c: add      r1, r7, #0x30
007cfb20: mov      r0, r4
007cfb24: bl       #0x783f50
007cfb28: mov      r0, r4
007cfb2c: bl       #0x783c14
007cfb30: mov      sl, r0
007cfb34: mov      r0, r4
007cfb38: bl       #0x783c7c
007cfb3c: mov      r8, #0
007cfb40: cmp      r6, #0
007cfb44: str      r0, [sp, #8]
007cfb48: str      r8, [sp, #0x28]
007cfb4c: str      r8, [sp, #0x2c]
007cfb50: str      r8, [sp, #0x30]
007cfb54: strb     r8, [sp, #0x34]
007cfb58: bne      #0x7cfcc0
007cfb5c: cmp      sl, r8
007cfb60: addeq    r2, sp, #0x28
007cfb64: streq    r2, [sp, #0x10]
007cfb68: beq      #0x7cfbc4
007cfb6c: add      r2, sp, #0x28
007cfb70: mov      r8, r6
007cfb74: str      r2, [sp, #0x10]
007cfb78: mov      fp, r2
007cfb7c: mov      r0, r4
007cfb80: bl       #0x783c14
007cfb84: ldr      r3, [sp, #0x2c]
007cfb88: ldr      r2, [sp, #0x30]
007cfb8c: mov      sb, r0
007cfb90: add      r6, r3, #1
007cfb94: cmp      r6, r2
007cfb98: add      r8, r8, #1
007cfb9c: ble      #0x7cfbb0
007cfba0: mov      r0, fp
007cfba4: add      r1, r6, r6, asr #1
007cfba8: bl       #0x7643c0
007cfbac: ldr      r3, [sp, #0x2c]
007cfbb0: ldr      r2, [sp, #0x28]
007cfbb4: cmp      sl, r8
007cfbb8: str      sb, [r2, r3, lsl #2]
007cfbbc: str      r6, [sp, #0x2c]
007cfbc0: bgt      #0x7cfb7c
007cfbc4: mov      r0, r4
007cfbc8: bl       #0x783c14
007cfbcc: str      r0, [sp, #0xc]
007cfbd0: add      r0, r7, #0x20
007cfbd4: mov      r1, sl
007cfbd8: bl       #0x7cee98
007cfbdc: ldr      r3, [r5]
007cfbe0: mov      r0, r5
007cfbe4: mov      lr, pc
007cfbe8: ldr      pc, [r3, #0xb8]
007cfbec: subs     r6, r0, #0
007cfbf0: bne      #0x7cff74
007cfbf4: cmp      sl, #0
007cfbf8: beq      #0x7cfd50
007cfbfc: mov      r8, r7
007cfc00: mov      sb, sl
007cfc04: b        #0x7cfc50
007cfc08: mov      r1, #0
007cfc0c: mov      r0, #0x88
007cfc10: bl       #0x752ba8
007cfc14: mov      r1, fp
007cfc18: mov      r7, r0
007cfc1c: bl       #0x77b878
007cfc20: mov      r0, r7
007cfc24: mov      r1, r4
007cfc28: mov      r2, #0x16
007cfc2c: mov      r3, #0
007cfc30: str      r5, [sp]
007cfc34: bl       #0x77af58
007cfc38: ldr      r0, [r8, #0x20]
007cfc3c: mov      r1, r7
007cfc40: add      r0, r0, sl
007cfc44: bl       #0x7ce5e8
007cfc48: cmp      sb, r6
007cfc4c: ble      #0x7cfd4c
007cfc50: ldr      r3, [sp, #0x28]
007cfc54: mov      r0, r4
007cfc58: lsl      sl, r6, #2
007cfc5c: ldr      r1, [r3, r6, lsl #2]
007cfc60: ldr      r3, [sp, #8]
007cfc64: add      r6, r6, #1
007cfc68: add      r1, r3, r1
007cfc6c: bl       #0x783c94
007cfc70: ldr      fp, [r5, #0x1c]
007cfc74: cmp      fp, #0
007cfc78: beq      #0x7cfc08
007cfc7c: ldr      r3, [r5, #0x18]
007cfc80: ldrb     r2, [r3, #4]
007cfc84: cmp      r2, #0
007cfc88: bne      #0x7cfc08
007cfc8c: ldr      r2, [r3]
007cfc90: mov      fp, #0
007cfc94: mov      r0, r3
007cfc98: sub      r2, r2, #1
007cfc9c: cmp      r2, fp
007cfca0: mov      r1, r2
007cfca4: str      r2, [r3]
007cfca8: bne      #0x7cfcb0
007cfcac: bl       #0x752b38
007cfcb0: mov      r1, #0
007cfcb4: str      r1, [r5, #0x18]
007cfcb8: str      r1, [r5, #0x1c]
007cfcbc: b        #0x7cfc08
007cfcc0: cmp      sl, #0
007cfcc4: addeq    r3, sp, #0x28
007cfcc8: streq    r3, [sp, #0x10]
007cfccc: beq      #0x7cfd3c
007cfcd0: add      r1, sp, #0x28
007cfcd4: str      r1, [sp, #0x10]
007cfcd8: mov      fp, r1
007cfcdc: b        #0x7cfcf4
007cfce0: ldr      r2, [sp, #0x28]
007cfce4: cmp      sl, r8
007cfce8: str      sb, [r2, r3, lsl #2]
007cfcec: str      r6, [sp, #0x2c]
007cfcf0: ble      #0x7cfd3c
007cfcf4: mov      r0, r4
007cfcf8: bl       #0x783f1c
007cfcfc: ldr      r3, [sp, #0x2c]
007cfd00: ldr      r2, [sp, #0x30]
007cfd04: mov      sb, r0
007cfd08: add      r6, r3, #1
007cfd0c: cmp      r6, r2
007cfd10: add      r8, r8, #1
007cfd14: ble      #0x7cfce0
007cfd18: mov      r0, fp
007cfd1c: add      r1, r6, r6, asr #1
007cfd20: bl       #0x7643c0
007cfd24: ldr      r3, [sp, #0x2c]
007cfd28: ldr      r2, [sp, #0x28]
007cfd2c: cmp      sl, r8
007cfd30: str      sb, [r2, r3, lsl #2]
007cfd34: str      r6, [sp, #0x2c]
007cfd38: bgt      #0x7cfcf4
007cfd3c: mov      r0, r4
007cfd40: bl       #0x783f1c
007cfd44: str      r0, [sp, #0xc]
007cfd48: b        #0x7cfbd0
007cfd4c: mov      r7, r8
007cfd50: mov      r0, r4
007cfd54: bl       #0x783c7c
007cfd58: ldr      r2, [sp, #0xc]
007cfd5c: ldr      r1, [sp, #8]
007cfd60: add      r3, r2, r1
007cfd64: cmp      r0, r3
007cfd68: beq      #0x7cff98
007cfd6c: ldr      r3, [sp, #0x2c]
007cfd70: cmp      r3, #0
007cfd74: ble      #0x7d0164
007cfd78: mov      r3, #0
007cfd7c: ldr      r0, [sp, #0x10]
007cfd80: mov      r1, r3
007cfd84: str      r3, [sp, #0x2c]
007cfd88: bl       #0x7643c0
007cfd8c: b        #0x7cfa5c
007cfd90: mov      r0, r1
007cfd94: bl       #0x783c7c
007cfd98: mov      r3, #0
007cfd9c: str      r0, [sp, #8]
007cfda0: mov      r0, r4
007cfda4: strb     r3, [sp, #0x44]
007cfda8: str      r3, [sp, #0x38]
007cfdac: str      r3, [sp, #0x3c]
007cfdb0: str      r3, [sp, #0x40]
007cfdb4: bl       #0x783c14
007cfdb8: ldr      r3, [sp, #0x3c]
007cfdbc: ldr      r2, [sp, #0x40]
007cfdc0: mov      r8, r0
007cfdc4: add      r6, r3, #1
007cfdc8: cmp      r6, r2
007cfdcc: addle    r1, sp, #0x38
007cfdd0: strle    r1, [sp, #0xc]
007cfdd4: ble      #0x7cfdf0
007cfdd8: add      r2, sp, #0x38
007cfddc: mov      r0, r2
007cfde0: add      r1, r6, r6, asr #1
007cfde4: str      r2, [sp, #0xc]
007cfde8: bl       #0x7643c0
007cfdec: ldr      r3, [sp, #0x3c]
007cfdf0: ldr      r2, [sp, #0x38]
007cfdf4: str      r8, [r2, r3, lsl #2]
007cfdf8: ldr      r3, [sp, #0x38]
007cfdfc: str      r6, [sp, #0x3c]
007cfe00: ldr      r8, [r3]
007cfe04: asr      r8, r8, #1
007cfe08: cmp      r8, #1
007cfe0c: ble      #0x7cfe60
007cfe10: ldr      fp, [sp, #0xc]
007cfe14: mov      sl, #1
007cfe18: mov      r0, r4
007cfe1c: bl       #0x783c14
007cfe20: ldr      r3, [sp, #0x3c]
007cfe24: ldr      r2, [sp, #0x40]
007cfe28: mov      sb, r0
007cfe2c: add      r6, r3, #1
007cfe30: cmp      r6, r2
007cfe34: add      sl, sl, #1
007cfe38: ble      #0x7cfe4c
007cfe3c: mov      r0, fp
007cfe40: add      r1, r6, r6, asr #1
007cfe44: bl       #0x7643c0
007cfe48: ldr      r3, [sp, #0x3c]
007cfe4c: ldr      r2, [sp, #0x38]
007cfe50: cmp      sl, r8
007cfe54: str      sb, [r2, r3, lsl #2]
007cfe58: str      r6, [sp, #0x3c]
007cfe5c: bne      #0x7cfe18
007cfe60: add      r0, r7, #0x20
007cfe64: mov      r1, r8
007cfe68: bl       #0x7cee98
007cfe6c: ldr      r3, [r5]
007cfe70: mov      r0, r5
007cfe74: mov      lr, pc
007cfe78: ldr      pc, [r3, #0xb8]
007cfe7c: subs     r6, r0, #0
007cfe80: bne      #0x7cff50
007cfe84: cmp      r8, #0
007cfe88: ble      #0x7cff50
007cfe8c: mov      sb, r8
007cfe90: mov      r8, r7
007cfe94: b        #0x7cfee0
007cfe98: mov      r1, #0
007cfe9c: mov      r0, #0x88
007cfea0: bl       #0x752ba8
007cfea4: mov      r1, fp
007cfea8: mov      r7, r0
007cfeac: bl       #0x77b878
007cfeb0: mov      r0, r7
007cfeb4: mov      r1, r4
007cfeb8: mov      r2, #2
007cfebc: mov      r3, #0
007cfec0: str      r5, [sp]
007cfec4: bl       #0x77af58
007cfec8: ldr      r0, [r8, #0x20]
007cfecc: mov      r1, r7
007cfed0: add      r0, r0, sl
007cfed4: bl       #0x7ce5e8
007cfed8: cmp      r6, sb
007cfedc: beq      #0x7cff50
007cfee0: ldr      r3, [sp, #0x38]
007cfee4: mov      r0, r4
007cfee8: lsl      sl, r6, #2
007cfeec: ldr      r1, [r3, r6, lsl #2]
007cfef0: ldr      r3, [sp, #8]
007cfef4: add      r6, r6, #1
007cfef8: add      r1, r3, r1
007cfefc: bl       #0x783c94
007cff00: ldr      fp, [r5, #0x1c]
007cff04: cmp      fp, #0
007cff08: beq      #0x7cfe98
007cff0c: ldr      r3, [r5, #0x18]
007cff10: ldrb     r2, [r3, #4]
007cff14: cmp      r2, #0
007cff18: bne      #0x7cfe98
007cff1c: ldr      r2, [r3]
007cff20: mov      fp, #0
007cff24: mov      r0, r3
007cff28: sub      r2, r2, #1
007cff2c: cmp      r2, fp
007cff30: mov      r1, r2
007cff34: str      r2, [r3]
007cff38: bne      #0x7cff40
007cff3c: bl       #0x752b38
007cff40: mov      r1, #0
007cff44: str      r1, [r5, #0x18]
007cff48: str      r1, [r5, #0x1c]
007cff4c: b        #0x7cfe98
007cff50: ldr      r3, [sp, #0x3c]
007cff54: cmp      r3, #0
007cff58: ble      #0x7d0188
007cff5c: mov      r3, #0
007cff60: ldr      r0, [sp, #0xc]
007cff64: mov      r1, r3
007cff68: str      r3, [sp, #0x3c]
007cff6c: bl       #0x7643c0
007cff70: b        #0x7cfa5c
007cff74: mov      r0, r4
007cff78: bl       #0x783cbc
007cff7c: ldr      r2, [sp, #0xc]
007cff80: ldr      r3, [sp, #8]
007cff84: add      r1, r2, r3
007cff88: cmp      r1, r0
007cff8c: bge      #0x7cfd6c
007cff90: mov      r0, r4
007cff94: bl       #0x783c94
007cff98: mov      r1, r4
007cff9c: mov      r0, r7
007cffa0: bl       #0x7cf7ac
007cffa4: ldr      r1, [sp, #0x14]
007cffa8: cmp      r1, #0
007cffac: bne      #0x7cffe0
007cffb0: ldr      r3, [sp, #0x2c]
007cffb4: cmp      r3, #0
007cffb8: bgt      #0x7cfd78
007cffbc: bge      #0x7cfd78
007cffc0: lsl      r2, r3, #2
007cffc4: mov      r0, #0
007cffc8: ldr      r1, [sp, #0x28]
007cffcc: adds     r3, r3, #1
007cffd0: str      r0, [r1, r2]
007cffd4: add      r2, r2, #4
007cffd8: bne      #0x7cffc8
007cffdc: b        #0x7cfd78
007cffe0: mov      r0, r4
007cffe4: bl       #0x783c48
007cffe8: sxth     r0, r0
007cffec: bl       #0x30e964
007cfff0: str      r0, [r7, #0x54]
007cfff4: mov      r0, r4
007cfff8: bl       #0x783c48
007cfffc: sxth     r0, r0
007d0000: bl       #0x30e964
007d0004: str      r0, [r7, #0x58]
007d0008: mov      r0, r4
007d000c: bl       #0x783c48
007d0010: sxth     r0, r0
007d0014: bl       #0x30e964
007d0018: ldr      r6, [r7, #0x24]
007d001c: str      r0, [r7, #0x5c]
007d0020: add      r8, r7, #0x60
007d0024: cmp      r6, #0
007d0028: ldr      r5, [r7, #0x64]
007d002c: beq      #0x7d003c
007d0030: ldr      r3, [r7, #0x68]
007d0034: cmp      r6, r3
007d0038: bgt      #0x7d01ac
007d003c: cmp      r6, r5
007d0040: ble      #0x7d0064
007d0044: mov      r1, #0
007d0048: lsl      r3, r5, #2
007d004c: ldr      r2, [r8]
007d0050: add      r5, r5, #1
007d0054: cmp      r5, r6
007d0058: str      r1, [r2, r3]
007d005c: add      r3, r3, #4
007d0060: bne      #0x7d004c
007d0064: cmp      r6, #0
007d0068: str      r6, [r7, #0x64]
007d006c: ble      #0x7d0098
007d0070: mov      r5, #0
007d0074: mov      r0, r4
007d0078: ldr      r8, [r7, #0x60]
007d007c: bl       #0x783c48
007d0080: sxth     r0, r0
007d0084: bl       #0x30e964
007d0088: str      r0, [r8, r5, lsl #2]
007d008c: add      r5, r5, #1
007d0090: cmp      r5, r6
007d0094: bne      #0x7d0074
007d0098: ldr      r6, [r7, #0x24]
007d009c: cmp      r6, #0
007d00a0: ble      #0x7d00c4
007d00a4: mov      r5, #0
007d00a8: add      r8, sp, #0x18
007d00ac: add      r5, r5, #1
007d00b0: mov      r0, r8
007d00b4: mov      r1, r4
007d00b8: bl       #0x795fec
007d00bc: cmp      r5, r6
007d00c0: bne      #0x7d00ac
007d00c4: mov      r0, r4
007d00c8: bl       #0x783c14
007d00cc: subs     sb, r0, #0
007d00d0: beq      #0x7cffb0
007d00d4: add      r2, sp, #0x38
007d00d8: add      fp, r7, #0x70
007d00dc: mov      r5, #0
007d00e0: str      r2, [sp, #0xc]
007d00e4: b        #0x7d0138
007d00e8: bl       #0x783c14
007d00ec: mov      r8, r0
007d00f0: mov      r0, r4
007d00f4: bl       #0x783c14
007d00f8: mov      r6, r0
007d00fc: mov      r0, r4
007d0100: bl       #0x783c48
007d0104: ldr      r1, [sp, #0xc]
007d0108: uxth     sl, r0
007d010c: mov      r0, fp
007d0110: strh     r6, [sp, #0x3a]
007d0114: strh     r8, [sp, #0x38]
007d0118: bl       #0x7cecc0
007d011c: mov      r6, r0
007d0120: sxth     r0, sl
007d0124: bl       #0x30e964
007d0128: add      r5, r5, #1
007d012c: cmp      sb, r5
007d0130: str      r0, [r6]
007d0134: ble      #0x7cffb0
007d0138: ldrb     r3, [r7, #0x4e]
007d013c: mov      r0, r4
007d0140: cmp      r3, #0
007d0144: bne      #0x7d00e8
007d0148: mov      r0, r4
007d014c: bl       #0x783b28
007d0150: mov      r8, r0
007d0154: mov      r0, r4
007d0158: bl       #0x783b28
007d015c: mov      r6, r0
007d0160: b        #0x7d00fc
007d0164: bge      #0x7cfd78
007d0168: lsl      r2, r3, #2
007d016c: mov      r0, #0
007d0170: ldr      r1, [sp, #0x28]
007d0174: adds     r3, r3, #1
007d0178: str      r0, [r1, r2]
007d017c: add      r2, r2, #4
007d0180: bne      #0x7d0170
007d0184: b        #0x7cfd78
007d0188: bge      #0x7cff5c
007d018c: lsl      r2, r3, #2
007d0190: mov      r0, #0
007d0194: ldr      r1, [sp, #0x38]
007d0198: adds     r3, r3, #1
007d019c: str      r0, [r1, r2]
007d01a0: add      r2, r2, #4
007d01a4: bne      #0x7d0194
007d01a8: b        #0x7cff5c
007d01ac: mov      r0, r8
007d01b0: add      r1, r6, r6, asr #1
007d01b4: bl       #0x779df0
007d01b8: b        #0x7d003c

# _ZN8RenderFX13PreloadGlyphsEPN7gameswf9characterE
007a95a8: push     {r4, r5, r6, lr}
007a95ac: cmp      r1, #0
007a95b0: ldreq    r3, [r0, #0x3c]
007a95b4: mov      r2, #0
007a95b8: ldreq    r1, [r3, #0x10]
007a95bc: mov      r3, r2
007a95c0: bl       #0x7a8c08
007a95c4: ldr      r3, [r0, #4]
007a95c8: mov      r5, r0
007a95cc: cmp      r3, #0
007a95d0: ble      #0x7a962c
007a95d4: mov      r4, #0
007a95d8: b        #0x7a95ec
007a95dc: ldr      r3, [r5, #4]
007a95e0: add      r4, r4, #1
007a95e4: cmp      r4, r3
007a95e8: bge      #0x7a962c
007a95ec: ldr      r3, [r5]
007a95f0: mov      r1, #0x20
007a95f4: ldr      r3, [r3, r4, lsl #2]
007a95f8: mov      r0, r3
007a95fc: ldr      r3, [r3]
007a9600: mov      lr, pc
007a9604: ldr      pc, [r3, #8]
007a9608: cmp      r0, #0
007a960c: beq      #0x7a95dc
007a9610: ldr      r3, [r5]
007a9614: ldr      r0, [r3, r4, lsl #2]
007a9618: bl       #0x78c2d0
007a961c: ldr      r3, [r5, #4]
007a9620: add      r4, r4, #1
007a9624: cmp      r4, r3
007a9628: blt      #0x7a95ec
007a962c: mov      r0, #1
007a9630: pop      {r4, r5, r6, pc}
