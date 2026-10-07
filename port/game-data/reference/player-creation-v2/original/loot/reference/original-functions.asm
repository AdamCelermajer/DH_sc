
# _ZN13ItemInventory13_AddLootItemsERKSt6vectorIPKN7Structs9LootEntryESaIS4_EERS0_INS_8LootInfoESaIS9_EEb
00402dfc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00402e00: ldr      r3, [pc, #0x4ac]
00402e04: sub      sp, sp, #0xb4
00402e08: ldr      r6, [pc, #0x4a8]
00402e0c: str      r3, [sp, #0x4c]
00402e10: str      r0, [sp, #0x30]
00402e14: ldr      r5, [r0]
00402e18: add      r6, pc, r6
00402e1c: ldr      r3, [r6, r3]
00402e20: str      r5, [sp, #0x2c]
00402e24: ldr      r0, [r0, #4]
00402e28: ldr      r3, [r3]
00402e2c: mov      r4, r1
00402e30: cmp      r5, r0
00402e34: str      r3, [sp, #0xac]
00402e38: str      r2, [sp, #0x34]
00402e3c: beq      #0x403050
00402e40: ldr      r3, [pc, #0x474]
00402e44: ldr      ip, [pc, #0x474]
00402e48: add      r3, pc, r3
00402e4c: str      r3, [sp, #0x40]
00402e50: ldr      r3, [pc, #0x46c]
00402e54: str      ip, [sp, #0x3c]
00402e58: add      r3, pc, r3
00402e5c: str      r3, [sp, #0x44]
00402e60: ldr      r3, [pc, #0x460]
00402e64: add      r3, pc, r3
00402e68: str      r3, [sp, #0x48]
00402e6c: ldr      r0, [sp, #0x2c]
00402e70: ldr      r0, [r0]
00402e74: cmp      r0, #0
00402e78: str      r0, [sp, #0x10]
00402e7c: beq      #0x4031bc
00402e80: ldr      r1, [sp, #0x10]
00402e84: ldr      r3, [r1, #4]
00402e88: cmp      r3, #0
00402e8c: blt      #0x402ea4
00402e90: ldr      r2, [pc, #0x434]
00402e94: ldr      r2, [r6, r2]
00402e98: ldr      r2, [r2]
00402e9c: cmp      r3, r2
00402ea0: blt      #0x402ec8
00402ea4: ldr      r2, [pc, #0x424]
00402ea8: ldr      r2, [r6, r2]
00402eac: ldr      r2, [r2]
00402eb0: cmp      r2, #2
00402eb4: moveq    r2, #0
00402eb8: streq    r2, [r2]
00402ebc: beq      #0x402ec8
00402ec0: cmp      r2, #1
00402ec4: beq      #0x403210
00402ec8: ldr      r5, [sp, #0x3c]
00402ecc: ldr      ip, [sp, #0x34]
00402ed0: ldr      r2, [r6, r5]
00402ed4: cmp      ip, #0
00402ed8: mov      r5, #0xc
00402edc: ldr      r7, [r2]
00402ee0: mla      r7, r5, r3, r7
00402ee4: beq      #0x4030ac
00402ee8: ldr      r3, [r7, #4]
00402eec: cmp      r3, #0
00402ef0: beq      #0x403034
00402ef4: ldr      r0, [pc, #0x3d8]
00402ef8: ldr      r1, [pc, #0x3d8]
00402efc: ldr      r2, [pc, #0x3d8]
00402f00: ldr      r3, [pc, #0x3c8]
00402f04: ldr      r5, [pc, #0x3d4]
00402f08: ldr      ip, [pc, #0x3d4]
00402f0c: mov      r8, #0
00402f10: str      r0, [sp, #0x14]
00402f14: str      r1, [sp, #0x1c]
00402f18: add      r0, sp, #0x78
00402f1c: add      r1, sp, #0x64
00402f20: str      r2, [sp, #0x18]
00402f24: str      r3, [sp, #0x28]
00402f28: str      r5, [sp, #0x38]
00402f2c: str      ip, [sp, #0x24]
00402f30: mov      sb, r8
00402f34: add      fp, sp, #0x94
00402f38: str      r0, [sp, #0x20]
00402f3c: str      r1, [sp, #0xc]
00402f40: mov      sl, r8
00402f44: ldr      r5, [r7, #8]
00402f48: add      r5, r5, sl
00402f4c: ldr      r3, [r5, #4]
00402f50: cmp      r3, #0
00402f54: blt      #0x402f6c
00402f58: ldr      ip, [sp, #0x24]
00402f5c: ldr      r2, [r6, ip]
00402f60: ldr      r2, [r2]
00402f64: cmp      r3, r2
00402f68: blt      #0x402f90
00402f6c: ldr      r0, [sp, #0x28]
00402f70: ldr      r3, [r6, r0]
00402f74: ldr      r3, [r3]
00402f78: cmp      r3, #2
00402f7c: moveq    r3, #0
00402f80: streq    r3, [r3]
00402f84: beq      #0x402f90
00402f88: cmp      r3, #1
00402f8c: beq      #0x403084
00402f90: ldr      r2, [sp, #0x14]
00402f94: ldr      r8, [r6, r2]
00402f98: mov      r0, r8
00402f9c: bl       #0x337888
00402fa0: ldr      r3, [sp, #0x1c]
00402fa4: ldr      r2, [sp, #0x20]
00402fa8: mov      r0, fp
00402fac: add      r1, pc, r3
00402fb0: bl       #0x3140ec
00402fb4: mov      r1, fp
00402fb8: mov      r0, r8
00402fbc: bl       #0x337a88
00402fc0: mov      r0, fp
00402fc4: bl       #0x318254
00402fc8: ldr      ip, [sp, #0x18]
00402fcc: ldr      r3, [r5, #4]
00402fd0: mov      r0, #0xa4
00402fd4: ldr      r2, [r6, ip]
00402fd8: ldr      ip, [r4, #4]
00402fdc: ldr      r1, [r2]
00402fe0: ldr      r2, [r4, #8]
00402fe4: mla      r1, r0, r3, r1
00402fe8: ldrb     r0, [r5, #0xa]
00402fec: cmp      ip, r2
00402ff0: strh     r3, [sp, #0x64]
00402ff4: strb     r0, [sp, #0x6c]
00402ff8: ldr      r0, [sp, #0x10]
00402ffc: str      r1, [sp, #0x70]
00403000: str      r0, [sp, #0x68]
00403004: beq      #0x403070
00403008: ldr      r5, [sp, #0xc]
0040300c: ldm      r5, {r0, r1, r2, r3}
00403010: stm      ip, {r0, r1, r2, r3}
00403014: ldr      r3, [r4, #4]
00403018: add      r3, r3, #0x10
0040301c: str      r3, [r4, #4]
00403020: ldr      r3, [r7, #4]
00403024: add      sb, sb, #1
00403028: add      sl, sl, #0xc
0040302c: cmp      r3, sb
00403030: bhi      #0x402f44
00403034: ldr      r5, [sp, #0x30]
00403038: ldr      ip, [sp, #0x2c]
0040303c: ldr      r3, [r5, #4]
00403040: add      ip, ip, #4
00403044: str      ip, [sp, #0x2c]
00403048: cmp      ip, r3
0040304c: bne      #0x402e6c
00403050: ldr      r0, [sp, #0x4c]
00403054: ldr      r2, [sp, #0xac]
00403058: ldr      r3, [r6, r0]
0040305c: ldr      r3, [r3]
00403060: cmp      r2, r3
00403064: bne      #0x4032b0
00403068: add      sp, sp, #0xb4
0040306c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00403070: mov      r1, ip
00403074: mov      r0, r4
00403078: ldr      r2, [sp, #0xc]
0040307c: bl       #0x40256c
00403080: b        #0x403020
00403084: ldr      r1, [sp, #0x38]
00403088: movw     ip, #0x15e
0040308c: ldr      r2, [sp, #0x44]
00403090: ldr      r0, [r6, r1]
00403094: ldr      r3, [sp, #0x48]
00403098: ldr      r1, [sp, #0x40]
0040309c: add      r0, r0, #0xa8
004030a0: str      ip, [sp]
004030a4: bl       #0x30e004
004030a8: b        #0x402f90
004030ac: mov      r0, r7
004030b0: bl       #0x401dbc
004030b4: ldr      r7, [r7, #8]
004030b8: mla      r7, r5, r0, r7
004030bc: ldr      r3, [r7, #4]
004030c0: cmp      r3, #0
004030c4: blt      #0x4030dc
004030c8: ldr      r2, [pc, #0x214]
004030cc: ldr      r2, [r6, r2]
004030d0: ldr      r2, [r2]
004030d4: cmp      r3, r2
004030d8: blt      #0x403100
004030dc: ldr      r3, [pc, #0x1ec]
004030e0: ldr      r3, [r6, r3]
004030e4: ldr      r3, [r3]
004030e8: cmp      r3, #2
004030ec: moveq    r3, #0
004030f0: streq    r3, [r3]
004030f4: beq      #0x403100
004030f8: cmp      r3, #1
004030fc: beq      #0x40327c
00403100: ldr      ip, [pc, #0x1cc]
00403104: ldr      r0, [pc, #0x1d0]
00403108: add      r5, sp, #0x7c
0040310c: ldr      r8, [r6, ip]
00403110: str      r0, [sp, #0x18]
00403114: mov      r0, r8
00403118: bl       #0x337888
0040311c: ldr      r1, [pc, #0x1c4]
00403120: add      r2, sp, #0x74
00403124: mov      r0, r5
00403128: add      r1, pc, r1
0040312c: bl       #0x3140ec
00403130: mov      r1, r5
00403134: mov      r0, r8
00403138: bl       #0x337a88
0040313c: mov      r0, r5
00403140: bl       #0x318254
00403144: ldr      r1, [sp, #0x18]
00403148: ldr      r3, [r7, #4]
0040314c: ldr      ip, [r4, #4]
00403150: ldr      r2, [r6, r1]
00403154: mov      r0, #0xa4
00403158: ldr      r1, [r2]
0040315c: ldr      r2, [r4, #8]
00403160: mla      r1, r0, r3, r1
00403164: cmp      ip, r2
00403168: ldrb     r0, [r7, #0xa]
0040316c: ldr      r2, [sp, #0x10]
00403170: strh     r3, [sp, #0x54]
00403174: strb     r0, [sp, #0x5c]
00403178: str      r2, [sp, #0x58]
0040317c: str      r1, [sp, #0x60]
00403180: beq      #0x40324c
00403184: add      r3, sp, #0x54
00403188: ldm      r3, {r0, r1, r2, r3}
0040318c: stm      ip, {r0, r1, r2, r3}
00403190: ldr      r3, [r4, #4]
00403194: add      r3, r3, #0x10
00403198: str      r3, [r4, #4]
0040319c: ldr      r5, [sp, #0x30]
004031a0: ldr      ip, [sp, #0x2c]
004031a4: ldr      r3, [r5, #4]
004031a8: add      ip, ip, #4
004031ac: str      ip, [sp, #0x2c]
004031b0: cmp      ip, r3
004031b4: bne      #0x402e6c
004031b8: b        #0x403050
004031bc: ldr      r3, [pc, #0x10c]
004031c0: ldr      r3, [r6, r3]
004031c4: ldr      r3, [r3]
004031c8: cmp      r3, #2
004031cc: streq    r0, [r0]
004031d0: beq      #0x402e80
004031d4: cmp      r3, #1
004031d8: bne      #0x402e80
004031dc: ldr      r0, [pc, #0xfc]
004031e0: ldr      r1, [pc, #0x104]
004031e4: ldr      r2, [pc, #0x104]
004031e8: ldr      r0, [r6, r0]
004031ec: ldr      r3, [pc, #0x100]
004031f0: movw     ip, #0x153
004031f4: add      r1, pc, r1
004031f8: add      r2, pc, r2
004031fc: add      r3, pc, r3
00403200: add      r0, r0, #0xa8
00403204: str      ip, [sp]
00403208: bl       #0x30e004
0040320c: b        #0x402e80
00403210: ldr      r0, [pc, #0xc8]
00403214: ldr      r1, [pc, #0xdc]
00403218: ldr      r2, [pc, #0xdc]
0040321c: ldr      r0, [r6, r0]
00403220: ldr      r3, [pc, #0xd8]
00403224: add      r2, pc, r2
00403228: mov      ip, #0x154
0040322c: add      r3, pc, r3
00403230: add      r1, pc, r1
00403234: add      r0, r0, #0xa8
00403238: str      ip, [sp]
0040323c: bl       #0x30e004
00403240: ldr      r2, [sp, #0x10]
00403244: ldr      r3, [r2, #4]
00403248: b        #0x402ec8
0040324c: mov      r1, ip
00403250: mov      r0, r4
00403254: add      r2, sp, #0x54
00403258: bl       #0x40256c
0040325c: ldr      r5, [sp, #0x30]
00403260: ldr      ip, [sp, #0x2c]
00403264: ldr      r3, [r5, #4]
00403268: add      ip, ip, #4
0040326c: str      ip, [sp, #0x2c]
00403270: cmp      ip, r3
00403274: bne      #0x402e6c
00403278: b        #0x403050
0040327c: ldr      r0, [pc, #0x5c]
00403280: ldr      r1, [pc, #0x7c]
00403284: ldr      r2, [pc, #0x7c]
00403288: ldr      r0, [r6, r0]
0040328c: ldr      r3, [pc, #0x78]
00403290: movw     ip, #0x169
00403294: add      r1, pc, r1
00403298: add      r2, pc, r2
0040329c: add      r3, pc, r3
004032a0: add      r0, r0, #0xa8
004032a4: str      ip, [sp]
004032a8: bl       #0x30e004
004032ac: b        #0x403100
004032b0: bl       #0x30e310
004032b4: andeq    r4, r0, ip, lsr #1
004032b8: subseq   r1, sb, r8, ror ip
004032bc: umaaleq  fp, fp, r0, r5
004032c0: andeq    r1, r0, r8, lsr #13
004032c4: subeq    r4, ip, r0, lsl #19
004032c8: subeq    r4, ip, ip, lsl r7
004032cc: ldrdeq   r2, r3, [r0], -r0
004032d0: andeq    r3, r0, r0, asr #19
004032d4: andeq    r0, r0, r4, lsl #17
004032d8: subeq    r4, ip, ip, ror r8
004032dc: andeq    r2, r0, ip, ror #16
004032e0: andeq    r1, r0, r0, asr #19
004032e4: andeq    r0, r0, r0, ror #26
004032e8: subeq    r4, ip, r0, lsl #14
004032ec: subeq    fp, fp, r4, ror #3
004032f0: subeq    r4, ip, r8, ror r5
004032f4: subeq    r4, ip, r4, lsl #7
004032f8: subeq    fp, fp, r8, lsr #3
004032fc: subeq    r4, ip, ip, asr r5
00403300: subeq    r4, ip, r4, asr r3
00403304: subeq    fp, fp, r4, asr #2
00403308: subeq    r4, ip, r0, asr #10
0040330c: subeq    r4, ip, r4, ror #5

# _ZN7Structs9LootEntry4readEP11IStreamBase
004fec08: push     {r4, r5, lr}
004fec0c: mov      r4, r0
004fec10: sub      sp, sp, #0xc
004fec14: mov      r0, r1
004fec18: mov      r5, r1
004fec1c: add      r1, r4, #4
004fec20: bl       #0x459090
004fec24: mov      r3, #1
004fec28: cmp      r3, #0
004fec2c: str      r3, [sp, #4]
004fec30: bne      #0x4fec74
004fec34: add      r3, r4, #5
004fec38: add      r2, r4, #6
004fec3c: ldrb     r0, [r2, #1]
004fec40: ldrb     r1, [r3, #-1]
004fec44: cmp      r3, r2
004fec48: eor      r1, r0, r1
004fec4c: strb     r1, [r3, #-1]
004fec50: ldrb     r0, [r2, #1]
004fec54: eor      r1, r1, r0
004fec58: strb     r1, [r2, #1]
004fec5c: ldrb     r0, [r3, #-1]
004fec60: sub      r2, r2, #1
004fec64: eor      r1, r1, r0
004fec68: strb     r1, [r3, #-1]
004fec6c: add      r3, r3, #1
004fec70: blo      #0x4fec3c
004fec74: mov      r0, r5
004fec78: add      r1, r4, #8
004fec7c: bl       #0x459090
004fec80: mov      r3, #1
004fec84: cmp      r3, #0
004fec88: str      r3, [sp, #4]
004fec8c: bne      #0x4fecd0
004fec90: add      r3, r4, #9
004fec94: add      r2, r4, #0xa
004fec98: ldrb     r0, [r2, #1]
004fec9c: ldrb     r1, [r3, #-1]
004feca0: cmp      r3, r2
004feca4: eor      r1, r0, r1
004feca8: strb     r1, [r3, #-1]
004fecac: ldrb     r0, [r2, #1]
004fecb0: eor      r1, r1, r0
004fecb4: strb     r1, [r2, #1]
004fecb8: ldrb     r0, [r3, #-1]
004fecbc: sub      r2, r2, #1
004fecc0: eor      r1, r1, r0
004fecc4: strb     r1, [r3, #-1]
004fecc8: add      r3, r3, #1
004feccc: blo      #0x4fec98
004fecd0: mov      r0, r5
004fecd4: add      r1, r4, #0xc
004fecd8: bl       #0x459090
004fecdc: mov      r3, #1
004fece0: cmp      r3, #0
004fece4: str      r3, [sp, #4]
004fece8: bne      #0x4fed2c
004fecec: add      r3, r4, #0xd
004fecf0: add      r2, r4, #0xe
004fecf4: ldrb     r0, [r2, #1]
004fecf8: ldrb     r1, [r3, #-1]
004fecfc: cmp      r3, r2
004fed00: eor      r1, r0, r1
004fed04: strb     r1, [r3, #-1]
004fed08: ldrb     r0, [r2, #1]
004fed0c: eor      r1, r1, r0
004fed10: strb     r1, [r2, #1]
004fed14: ldrb     r0, [r3, #-1]
004fed18: sub      r2, r2, #1
004fed1c: eor      r1, r1, r0
004fed20: strb     r1, [r3, #-1]
004fed24: add      r3, r3, #1
004fed28: blo      #0x4fecf4
004fed2c: mov      r0, r5
004fed30: add      r1, r4, #0x10
004fed34: bl       #0x459090
004fed38: mov      r3, #1
004fed3c: cmp      r3, #0
004fed40: str      r3, [sp, #4]
004fed44: bne      #0x4fed88
004fed48: add      r3, r4, #0x11
004fed4c: add      r2, r4, #0x12
004fed50: ldrb     r0, [r2, #1]
004fed54: ldrb     r1, [r3, #-1]
004fed58: cmp      r3, r2
004fed5c: eor      r1, r0, r1
004fed60: strb     r1, [r3, #-1]
004fed64: ldrb     r0, [r2, #1]
004fed68: eor      r1, r1, r0
004fed6c: strb     r1, [r2, #1]
004fed70: ldrb     r0, [r3, #-1]
004fed74: sub      r2, r2, #1
004fed78: eor      r1, r1, r0
004fed7c: strb     r1, [r3, #-1]
004fed80: add      r3, r3, #1
004fed84: blo      #0x4fed50
004fed88: mov      r0, r5
004fed8c: add      r1, r4, #0x14
004fed90: bl       #0x459090
004fed94: mov      r3, #1
004fed98: cmp      r3, #0
004fed9c: str      r3, [sp, #4]
004feda0: bne      #0x4fede4
004feda4: add      r3, r4, #0x15
004feda8: add      r2, r4, #0x16
004fedac: ldrb     r0, [r2, #1]
004fedb0: ldrb     r1, [r3, #-1]
004fedb4: cmp      r3, r2
004fedb8: eor      r1, r0, r1
004fedbc: strb     r1, [r3, #-1]
004fedc0: ldrb     r0, [r2, #1]
004fedc4: eor      r1, r1, r0
004fedc8: strb     r1, [r2, #1]
004fedcc: ldrb     r0, [r3, #-1]
004fedd0: sub      r2, r2, #1
004fedd4: eor      r1, r1, r0
004fedd8: strb     r1, [r3, #-1]
004feddc: add      r3, r3, #1
004fede0: blo      #0x4fedac
004fede4: mov      r0, r5
004fede8: add      r1, r4, #0x18
004fedec: bl       #0x459090
004fedf0: mov      r3, #1
004fedf4: cmp      r3, #0
004fedf8: str      r3, [sp, #4]
004fedfc: bne      #0x4fee40
004fee00: add      r3, r4, #0x19
004fee04: add      r2, r4, #0x1a
004fee08: ldrb     r0, [r2, #1]
004fee0c: ldrb     r1, [r3, #-1]
004fee10: cmp      r3, r2
004fee14: eor      r1, r0, r1
004fee18: strb     r1, [r3, #-1]
004fee1c: ldrb     r0, [r2, #1]
004fee20: eor      r1, r1, r0
004fee24: strb     r1, [r2, #1]
004fee28: ldrb     r0, [r3, #-1]
004fee2c: sub      r2, r2, #1
004fee30: eor      r1, r1, r0
004fee34: strb     r1, [r3, #-1]
004fee38: add      r3, r3, #1
004fee3c: blo      #0x4fee08
004fee40: mov      r0, r5
004fee44: add      r1, r4, #0x1c
004fee48: bl       #0x459090
004fee4c: mov      r3, #1
004fee50: cmp      r3, #0
004fee54: str      r3, [sp, #4]
004fee58: bne      #0x4fee9c
004fee5c: add      r3, r4, #0x1d
004fee60: add      r2, r4, #0x1e
004fee64: ldrb     r0, [r2, #1]
004fee68: ldrb     r1, [r3, #-1]
004fee6c: cmp      r2, r3
004fee70: eor      r1, r0, r1
004fee74: strb     r1, [r3, #-1]
004fee78: ldrb     r0, [r2, #1]
004fee7c: eor      r1, r1, r0
004fee80: strb     r1, [r2, #1]
004fee84: ldrb     r0, [r3, #-1]
004fee88: sub      r2, r2, #1
004fee8c: eor      r1, r1, r0
004fee90: strb     r1, [r3, #-1]
004fee94: add      r3, r3, #1
004fee98: bhi      #0x4fee64
004fee9c: mov      r0, r5
004feea0: add      r1, r4, #0x20
004feea4: bl       #0x459090
004feea8: mov      r3, #1
004feeac: cmp      r3, #0
004feeb0: str      r3, [sp, #4]
004feeb4: bne      #0x4feef8
004feeb8: add      r3, r4, #0x22
004feebc: add      r4, r4, #0x21
004feec0: ldrb     r1, [r3, #1]
004feec4: ldrb     r2, [r4, #-1]
004feec8: cmp      r3, r4
004feecc: eor      r2, r1, r2
004feed0: strb     r2, [r4, #-1]
004feed4: ldrb     r1, [r3, #1]
004feed8: eor      r2, r2, r1
004feedc: strb     r2, [r3, #1]
004feee0: ldrb     r1, [r4, #-1]
004feee4: sub      r3, r3, #1
004feee8: eor      r2, r2, r1
004feeec: strb     r2, [r4, #-1]
004feef0: add      r4, r4, #1
004feef4: bhi      #0x4feec0
004feef8: add      sp, sp, #0xc
004feefc: pop      {r4, r5, pc}

# _ZN7Structs4Loot4readEP11IStreamBase
004fe794: push     {r4, r5, r6, r7, r8, lr}
004fe798: mov      r4, r0
004fe79c: sub      sp, sp, #8
004fe7a0: mov      r0, r1
004fe7a4: mov      r6, r1
004fe7a8: ldr      r8, [pc, #0x450]
004fe7ac: add      r1, r4, #4
004fe7b0: bl       #0x459090
004fe7b4: mov      r3, #1
004fe7b8: cmp      r3, #0
004fe7bc: str      r3, [sp, #4]
004fe7c0: add      r8, pc, r8
004fe7c4: bne      #0x4fe808
004fe7c8: add      r3, r4, #5
004fe7cc: add      r2, r4, #6
004fe7d0: ldrb     r0, [r2, #1]
004fe7d4: ldrb     r1, [r3, #-1]
004fe7d8: cmp      r3, r2
004fe7dc: eor      r1, r0, r1
004fe7e0: strb     r1, [r3, #-1]
004fe7e4: ldrb     r0, [r2, #1]
004fe7e8: eor      r1, r1, r0
004fe7ec: strb     r1, [r2, #1]
004fe7f0: ldrb     r0, [r3, #-1]
004fe7f4: sub      r2, r2, #1
004fe7f8: eor      r1, r1, r0
004fe7fc: strb     r1, [r3, #-1]
004fe800: add      r3, r3, #1
004fe804: blo      #0x4fe7d0
004fe808: mov      r0, r6
004fe80c: add      r1, r4, #8
004fe810: bl       #0x459090
004fe814: mov      r3, #1
004fe818: cmp      r3, #0
004fe81c: str      r3, [sp, #4]
004fe820: bne      #0x4fe864
004fe824: add      r3, r4, #9
004fe828: add      r2, r4, #0xa
004fe82c: ldrb     r0, [r2, #1]
004fe830: ldrb     r1, [r3, #-1]
004fe834: cmp      r3, r2
004fe838: eor      r1, r0, r1
004fe83c: strb     r1, [r3, #-1]
004fe840: ldrb     r0, [r2, #1]
004fe844: eor      r1, r1, r0
004fe848: strb     r1, [r2, #1]
004fe84c: ldrb     r0, [r3, #-1]
004fe850: sub      r2, r2, #1
004fe854: eor      r1, r1, r0
004fe858: strb     r1, [r3, #-1]
004fe85c: add      r3, r3, #1
004fe860: blo      #0x4fe82c
004fe864: mov      r0, r6
004fe868: add      r1, r4, #0xc
004fe86c: bl       #0x3df1a0
004fe870: mov      r3, #1
004fe874: cmp      r3, #0
004fe878: str      r3, [sp, #4]
004fe87c: bne      #0x4fe8c0
004fe880: add      r3, r4, #0xd
004fe884: add      r2, r4, #0xe
004fe888: ldrb     r0, [r2, #1]
004fe88c: ldrb     r1, [r3, #-1]
004fe890: cmp      r3, r2
004fe894: eor      r1, r0, r1
004fe898: strb     r1, [r3, #-1]
004fe89c: ldrb     r0, [r2, #1]
004fe8a0: eor      r1, r1, r0
004fe8a4: strb     r1, [r2, #1]
004fe8a8: ldrb     r0, [r3, #-1]
004fe8ac: sub      r2, r2, #1
004fe8b0: eor      r1, r1, r0
004fe8b4: strb     r1, [r3, #-1]
004fe8b8: add      r3, r3, #1
004fe8bc: blo      #0x4fe888
004fe8c0: ldr      r3, [r4, #0x10]
004fe8c4: cmp      r3, #0
004fe8c8: beq      #0x4fe910
004fe8cc: ldr      r2, [r3, #-4]
004fe8d0: mov      r0, #0x24
004fe8d4: mla      r0, r0, r2, r3
004fe8d8: cmp      r3, r0
004fe8dc: bne      #0x4fe8e8
004fe8e0: b        #0x4fe908
004fe8e4: mov      r0, r5
004fe8e8: sub      r5, r0, #0x24
004fe8ec: ldr      r3, [r0, #-0x24]
004fe8f0: mov      r0, r5
004fe8f4: mov      lr, pc
004fe8f8: ldr      pc, [r3]
004fe8fc: ldr      r0, [r4, #0x10]
004fe900: cmp      r0, r5
004fe904: bne      #0x4fe8e4
004fe908: sub      r0, r0, #8
004fe90c: bl       #0x310440
004fe910: ldr      r5, [r4, #0xc]
004fe914: mov      r7, #0x24
004fe918: mov      r1, #1
004fe91c: mul      r0, r7, r5
004fe920: add      r0, r0, #8
004fe924: bl       #0x31056c
004fe928: cmp      r5, #0
004fe92c: str      r7, [r0]
004fe930: str      r5, [r0, #4]
004fe934: add      r3, r0, #8
004fe938: beq      #0x4fe960
004fe93c: ldr      r1, [pc, #0x2c0]
004fe940: mov      r2, #0
004fe944: ldr      r1, [r8, r1]
004fe948: add      r1, r1, #8
004fe94c: add      r2, r2, #1
004fe950: cmp      r2, r5
004fe954: str      r1, [r0, #8]
004fe958: add      r0, r0, #0x24
004fe95c: bne      #0x4fe94c
004fe960: ldr      r2, [r4, #0xc]
004fe964: str      r3, [r4, #0x10]
004fe968: cmp      r2, #0
004fe96c: beq      #0x4fe9a8
004fe970: mov      r5, #0
004fe974: mov      r7, r5
004fe978: b        #0x4fe980
004fe97c: ldr      r3, [r4, #0x10]
004fe980: add      r0, r3, r5
004fe984: mov      r1, r6
004fe988: ldr      r3, [r3, r5]
004fe98c: mov      lr, pc
004fe990: ldr      pc, [r3, #0xc]
004fe994: ldr      r3, [r4, #0xc]
004fe998: add      r7, r7, #1
004fe99c: add      r5, r5, #0x24
004fe9a0: cmp      r3, r7
004fe9a4: bhi      #0x4fe97c
004fe9a8: mov      r0, r6
004fe9ac: add      r1, r4, #0x14
004fe9b0: bl       #0x3df1a0
004fe9b4: mov      r3, #1
004fe9b8: cmp      r3, #0
004fe9bc: str      r3, [sp, #4]
004fe9c0: bne      #0x4fea04
004fe9c4: add      r3, r4, #0x15
004fe9c8: add      r2, r4, #0x16
004fe9cc: ldrb     r0, [r2, #1]
004fe9d0: ldrb     r1, [r3, #-1]
004fe9d4: cmp      r3, r2
004fe9d8: eor      r1, r0, r1
004fe9dc: strb     r1, [r3, #-1]
004fe9e0: ldrb     r0, [r2, #1]
004fe9e4: eor      r1, r1, r0
004fe9e8: strb     r1, [r2, #1]
004fe9ec: ldrb     r0, [r3, #-1]
004fe9f0: sub      r2, r2, #1
004fe9f4: eor      r1, r1, r0
004fe9f8: strb     r1, [r3, #-1]
004fe9fc: add      r3, r3, #1
004fea00: blo      #0x4fe9cc
004fea04: ldr      r3, [r4, #0x18]
004fea08: cmp      r3, #0
004fea0c: beq      #0x4fea54
004fea10: ldr      r2, [r3, #-4]
004fea14: mov      r0, #0x24
004fea18: mla      r0, r0, r2, r3
004fea1c: cmp      r3, r0
004fea20: bne      #0x4fea2c
004fea24: b        #0x4fea4c
004fea28: mov      r0, r5
004fea2c: sub      r5, r0, #0x24
004fea30: ldr      r3, [r0, #-0x24]
004fea34: mov      r0, r5
004fea38: mov      lr, pc
004fea3c: ldr      pc, [r3]
004fea40: ldr      r0, [r4, #0x18]
004fea44: cmp      r0, r5
004fea48: bne      #0x4fea28
004fea4c: sub      r0, r0, #8
004fea50: bl       #0x310440
004fea54: ldr      r5, [r4, #0x14]
004fea58: mov      r7, #0x24
004fea5c: mov      r1, #1
004fea60: mul      r0, r7, r5
004fea64: add      r0, r0, #8
004fea68: bl       #0x31056c
004fea6c: cmp      r5, #0
004fea70: str      r7, [r0]
004fea74: str      r5, [r0, #4]
004fea78: add      r3, r0, #8
004fea7c: beq      #0x4feaa4
004fea80: ldr      r1, [pc, #0x17c]
004fea84: mov      r2, #0
004fea88: ldr      r1, [r8, r1]
004fea8c: add      r1, r1, #8
004fea90: add      r2, r2, #1
004fea94: cmp      r2, r5
004fea98: str      r1, [r0, #8]
004fea9c: add      r0, r0, #0x24
004feaa0: bne      #0x4fea90
004feaa4: ldr      r2, [r4, #0x14]
004feaa8: str      r3, [r4, #0x18]
004feaac: cmp      r2, #0
004feab0: beq      #0x4feaec
004feab4: mov      r5, #0
004feab8: mov      r7, r5
004feabc: b        #0x4feac4
004feac0: ldr      r3, [r4, #0x18]
004feac4: add      r0, r3, r5
004feac8: mov      r1, r6
004feacc: ldr      r3, [r3, r5]
004fead0: mov      lr, pc
004fead4: ldr      pc, [r3, #0xc]
004fead8: ldr      r3, [r4, #0x14]
004feadc: add      r7, r7, #1
004feae0: add      r5, r5, #0x24
004feae4: cmp      r3, r7
004feae8: bhi      #0x4feac0
004feaec: mov      r0, r6
004feaf0: add      r1, r4, #0x1c
004feaf4: bl       #0x3df1a0
004feaf8: mov      r3, #1
004feafc: cmp      r3, #0
004feb00: str      r3, [sp, #4]
004feb04: bne      #0x4feb48
004feb08: add      r3, r4, #0x1d
004feb0c: add      r2, r4, #0x1e
004feb10: ldrb     r0, [r2, #1]
004feb14: ldrb     r1, [r3, #-1]
004feb18: cmp      r3, r2
004feb1c: eor      r1, r0, r1
004feb20: strb     r1, [r3, #-1]
004feb24: ldrb     r0, [r2, #1]
004feb28: eor      r1, r1, r0
004feb2c: strb     r1, [r2, #1]
004feb30: ldrb     r0, [r3, #-1]
004feb34: sub      r2, r2, #1
004feb38: eor      r1, r1, r0
004feb3c: strb     r1, [r3, #-1]
004feb40: add      r3, r3, #1
004feb44: blo      #0x4feb10
004feb48: ldr      r0, [r4, #0x20]
004feb4c: cmp      r0, #0
004feb50: beq      #0x4feb58
004feb54: bl       #0x310440
004feb58: ldr      r0, [r4, #0x1c]
004feb5c: mov      r1, #1
004feb60: lsl      r0, r0, #2
004feb64: bl       #0x31056c
004feb68: ldr      r3, [r4, #0x1c]
004feb6c: str      r0, [r4, #0x20]
004feb70: cmp      r3, #0
004feb74: beq      #0x4febf8
004feb78: mov      r5, #0
004feb7c: mov      r8, #1
004feb80: lsl      r7, r5, #2
004feb84: add      r1, r0, r7
004feb88: mov      r0, r6
004feb8c: bl       #0x459090
004feb90: str      r8, [sp, #4]
004feb94: cmp      r8, #0
004feb98: ldr      r3, [r4, #0x20]
004feb9c: bne      #0x4febe4
004feba0: add      r7, r3, r7
004feba4: add      r3, r7, #2
004feba8: add      r7, r7, #1
004febac: ldrb     r1, [r3, #1]
004febb0: ldrb     r2, [r7, #-1]
004febb4: cmp      r7, r3
004febb8: eor      r2, r1, r2
004febbc: strb     r2, [r7, #-1]
004febc0: ldrb     r1, [r3, #1]
004febc4: eor      r2, r2, r1
004febc8: strb     r2, [r3, #1]
004febcc: ldrb     r1, [r7, #-1]
004febd0: sub      r3, r3, #1
004febd4: eor      r2, r2, r1
004febd8: strb     r2, [r7, #-1]
004febdc: add      r7, r7, #1
004febe0: blo      #0x4febac
004febe4: ldr      r3, [r4, #0x1c]
004febe8: add      r5, r5, #1
004febec: cmp      r3, r5
004febf0: ldrhi    r0, [r4, #0x20]
004febf4: bhi      #0x4feb80
004febf8: add      sp, sp, #8
004febfc: pop      {r4, r5, r6, r7, r8, pc}
004fec00: ldrdeq   r6, r7, [sb], #-0x20
004fec04: muleq    r0, r4, sl

# _ZN9Character14INV_UpdateSkinEv
003a999c: ldr      r1, [pc, #0x340]
003a99a0: ldr      r2, [pc, #0x340]
003a99a4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a99a8: add      r1, pc, r1
003a99ac: ldr      r3, [r1, r2]
003a99b0: sub      sp, sp, #0x64
003a99b4: add      sb, sp, #0x44
003a99b8: ldr      r3, [r3]
003a99bc: str      r1, [sp, #8]
003a99c0: mov      r6, r0
003a99c4: mov      r1, #0x10
003a99c8: mov      r0, sb
003a99cc: str      r3, [sp, #0x5c]
003a99d0: str      r2, [sp, #0x14]
003a99d4: str      sb, [sp, #0x54]
003a99d8: str      sb, [sp, #0x58]
003a99dc: bl       #0x31167c
003a99e0: ldr      r3, [sp, #0x54]
003a99e4: mov      r4, #0
003a99e8: strb     r4, [r3]
003a99ec: ldr      r3, [r6, #0x2d8]
003a99f0: cmp      r3, r4
003a99f4: beq      #0x3a9b78
003a99f8: ldr      r3, [pc, #0x2ec]
003a99fc: add      r7, r6, #0x37c
003a9a00: mvn      r5, #0
003a9a04: str      r3, [sp, #0xc]
003a9a08: ldr      r3, [pc, #0x2e0]
003a9a0c: mov      fp, sb
003a9a10: add      r3, pc, r3
003a9a14: str      r3, [sp, #0x18]
003a9a18: ldr      r3, [pc, #0x2d4]
003a9a1c: ldr      r1, [sp, #0x18]
003a9a20: add      r3, pc, r3
003a9a24: str      r3, [sp, #0x1c]
003a9a28: ldr      r2, [sp, #0x1c]
003a9a2c: ldr      r3, [pc, #0x2c4]
003a9a30: add      r1, r1, #7
003a9a34: add      r2, r2, #0xd
003a9a38: add      r3, pc, r3
003a9a3c: str      r3, [sp, #0x10]
003a9a40: str      r1, [sp, #0x20]
003a9a44: str      r2, [sp, #0x24]
003a9a48: b        #0x3a9adc
003a9a4c: mov      r1, r8
003a9a50: ldr      r0, [r6, #0x2d8]
003a9a54: bl       #0x470e5c
003a9a58: ldr      r2, [sp, #0x58]
003a9a5c: mov      r1, r0
003a9a60: mov      sl, r0
003a9a64: ldr      r0, [r6, #0x2d8]
003a9a68: bl       #0x474568
003a9a6c: cmn      r0, #1
003a9a70: mov      r3, r0
003a9a74: beq      #0x3a9c28
003a9a78: ldr      r2, [sp, #8]
003a9a7c: ldr      r1, [sp, #0xc]
003a9a80: add      r8, sp, #0x2c
003a9a84: str      r3, [sp, #4]
003a9a88: ldr      sb, [r2, r1]
003a9a8c: mov      r0, sb
003a9a90: bl       #0x337888
003a9a94: add      r2, sp, #0x28
003a9a98: ldr      r1, [sp, #0x10]
003a9a9c: mov      r0, r8
003a9aa0: bl       #0x3140ec
003a9aa4: mov      r1, r8
003a9aa8: mov      r0, sb
003a9aac: bl       #0x337a88
003a9ab0: mov      r0, r8
003a9ab4: bl       #0x318254
003a9ab8: ldr      r3, [sp, #4]
003a9abc: mov      r1, sl
003a9ac0: ldr      r0, [r6, #0x2d8]
003a9ac4: mov      r2, r3
003a9ac8: bl       #0x470e18
003a9acc: add      r4, r4, #1
003a9ad0: cmp      r4, #9
003a9ad4: add      r5, r5, #1
003a9ad8: beq      #0x3a9b74
003a9adc: mov      r1, r4
003a9ae0: mov      r0, r7
003a9ae4: bl       #0x3ffe3c
003a9ae8: mov      r1, r4
003a9aec: mov      sl, r0
003a9af0: mov      r0, r7
003a9af4: bl       #0x3ffd54
003a9af8: subs     r8, r0, #0
003a9afc: beq      #0x3a9acc
003a9b00: cmp      sl, #0
003a9b04: beq      #0x3a9ba4
003a9b08: mov      r0, sl
003a9b0c: bl       #0x3f9e08
003a9b10: ldr      sl, [r0, #0x50]
003a9b14: mov      r0, sl
003a9b18: bl       #0x30de54
003a9b1c: mov      r1, sl
003a9b20: add      r2, sl, r0
003a9b24: mov      r0, fp
003a9b28: bl       #0x3109e0
003a9b2c: cmp      r5, #1
003a9b30: bhi      #0x3a9a4c
003a9b34: ldr      r8, [sp, #0x58]
003a9b38: ldr      r1, [pc, #0x1bc]
003a9b3c: mov      r0, r8
003a9b40: add      r1, pc, r1
003a9b44: bl       #0x30ebd4
003a9b48: cmp      r0, #0
003a9b4c: movne    r3, #0
003a9b50: beq      #0x3a9c68
003a9b54: mov      r2, r4
003a9b58: mov      r1, r8
003a9b5c: ldr      r0, [r6, #0x2d8]
003a9b60: add      r4, r4, #1
003a9b64: bl       #0x473cd8
003a9b68: cmp      r4, #9
003a9b6c: add      r5, r5, #1
003a9b70: bne      #0x3a9adc
003a9b74: mov      sb, fp
003a9b78: mov      r0, sb
003a9b7c: bl       #0x318254
003a9b80: ldr      r2, [sp, #8]
003a9b84: ldr      r1, [sp, #0x14]
003a9b88: ldr      r3, [r2, r1]
003a9b8c: ldr      r2, [sp, #0x5c]
003a9b90: ldr      r3, [r3]
003a9b94: cmp      r2, r3
003a9b98: bne      #0x3a9ce0
003a9b9c: add      sp, sp, #0x64
003a9ba0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a9ba4: bl       #0x30de54
003a9ba8: mov      r1, r8
003a9bac: add      r2, r8, r0
003a9bb0: mov      r0, fp
003a9bb4: bl       #0x3109e0
003a9bb8: mov      r0, fp
003a9bbc: ldr      r1, [sp, #0x18]
003a9bc0: ldr      r2, [sp, #0x20]
003a9bc4: bl       #0x310804
003a9bc8: cmp      r5, #1
003a9bcc: bls      #0x3a9bf8
003a9bd0: mov      r1, r8
003a9bd4: ldr      r0, [r6, #0x2d8]
003a9bd8: bl       #0x470e5c
003a9bdc: ldr      r2, [sp, #0x58]
003a9be0: mov      r1, r0
003a9be4: mov      sl, r0
003a9be8: ldr      r0, [r6, #0x2d8]
003a9bec: bl       #0x474568
003a9bf0: mov      r3, r0
003a9bf4: b        #0x3a9a78
003a9bf8: ldr      r1, [pc, #0x100]
003a9bfc: ldr      r0, [sp, #0x58]
003a9c00: add      r1, pc, r1
003a9c04: bl       #0x30ebd4
003a9c08: cmp      r0, #0
003a9c0c: moveq    r3, r4
003a9c10: movne    r3, #0
003a9c14: mov      r1, sl
003a9c18: ldr      r0, [r6, #0x2d8]
003a9c1c: mov      r2, r4
003a9c20: bl       #0x473cd8
003a9c24: b        #0x3a9acc
003a9c28: mov      r0, r8
003a9c2c: bl       #0x30de54
003a9c30: mov      r1, r8
003a9c34: add      r2, r8, r0
003a9c38: mov      r0, fp
003a9c3c: bl       #0x3109e0
003a9c40: ldr      r1, [sp, #0x1c]
003a9c44: ldr      r2, [sp, #0x24]
003a9c48: mov      r0, fp
003a9c4c: bl       #0x310804
003a9c50: ldr      r0, [r6, #0x2d8]
003a9c54: mov      r1, sl
003a9c58: ldr      r2, [sp, #0x58]
003a9c5c: bl       #0x474568
003a9c60: mov      r3, r0
003a9c64: b        #0x3a9a78
003a9c68: ldr      r1, [pc, #0x94]
003a9c6c: mov      r0, r8
003a9c70: add      r1, pc, r1
003a9c74: bl       #0x30ebd4
003a9c78: cmp      r0, #0
003a9c7c: beq      #0x3a9c88
003a9c80: mov      r3, #2
003a9c84: b        #0x3a9b54
003a9c88: ldr      r1, [pc, #0x78]
003a9c8c: mov      r0, r8
003a9c90: add      r1, pc, r1
003a9c94: bl       #0x30ebd4
003a9c98: subs     r0, r0, #0
003a9c9c: movne    r0, #1
003a9ca0: cmp      r4, #2
003a9ca4: movne    r0, #0
003a9ca8: cmp      r0, #0
003a9cac: moveq    r3, r4
003a9cb0: beq      #0x3a9b54
003a9cb4: ldr      r1, [pc, #0x50]
003a9cb8: mov      r0, r8
003a9cbc: add      r1, pc, r1
003a9cc0: bl       #0x30ebd4
003a9cc4: cmp      r0, #0
003a9cc8: beq      #0x3a9c80
003a9ccc: mov      r3, #0x4c
003a9cd0: strb     r3, [r0]
003a9cd4: ldr      r8, [sp, #0x58]
003a9cd8: mov      r3, #2
003a9cdc: b        #0x3a9b54
003a9ce0: bl       #0x30e310
003a9ce4: subseq   fp, lr, r8, ror #1
003a9ce8: andeq    r4, r0, ip, lsr #1
003a9cec: andeq    r0, r0, r4, lsl #17
003a9cf0: subseq   sb, r1, r8, lsr fp
003a9cf4: subseq   sb, r1, r8, lsl fp
003a9cf8: subseq   sb, r1, r8, lsl fp
003a9cfc: ldrsbeq  sb, [r1], #-0x98
003a9d00: subseq   sb, r1, r8, lsl sb
003a9d04: ldrheq   sb, [r1], #-0x80

# _ZN13ItemInventory17CalcLootItemValueEP12ItemInstancei
004020b4: push     {r4, r5, r6, r7, r8, lr}
004020b8: mov      r6, r1
004020bc: mov      r5, r0
004020c0: bl       #0x3f9e08
004020c4: ldr      r3, [r0, #0x58]
004020c8: cmp      r3, #0xd
004020cc: beq      #0x40212c
004020d0: ldr      r6, [r0, #0x6c]
004020d4: ldr      r3, [r0, #0x70]
004020d8: mov      r4, #0
004020dc: mul      r6, r6, r3
004020e0: b        #0x402104
004020e4: bl       #0x3f9ec4
004020e8: mov      r1, r4
004020ec: ldr      r7, [r0, #0x20]
004020f0: mov      r0, r5
004020f4: bl       #0x3f9ec4
004020f8: ldr      r3, [r0, #0x18]
004020fc: add      r4, r4, #1
00402100: mla      r6, r3, r7, r6
00402104: mov      r0, r5
00402108: bl       #0x3f9e80
0040210c: cmp      r4, r0
00402110: mov      r1, r4
00402114: mov      r0, r5
00402118: blo      #0x4020e4
0040211c: mov      r0, r5
00402120: mov      r1, r6
00402124: pop      {r4, r5, r6, r7, r8, lr}
00402128: b        #0x3fbc58
0040212c: ldr      r3, [r0, #0x70]
00402130: ldr      r4, [r0, #0x6c]
00402134: add      r0, r3, #1
00402138: rsb      r0, r4, r0
0040213c: bl       #0x401afc
00402140: add      r0, r0, r4
00402144: bl       #0x30e964
00402148: mov      r4, r0
0040214c: asr      r0, r6, #8
00402150: bl       #0x30e964
00402154: mov      r1, #0x42000000
00402158: add      r1, r1, #0xc80000
0040215c: bl       #0x30eba4
00402160: mov      r1, #0x42000000
00402164: add      r1, r1, #0xc80000
00402168: bl       #0x30ec94
0040216c: mov      r1, r0
00402170: mov      r0, r4
00402174: bl       #0x30ed6c
00402178: bl       #0x30e4cc
0040217c: mov      r6, r0
00402180: b        #0x40211c

# _ZN6Arrays9LootTable9readNamesEP11IStreamBase
004afdf4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004afdf8: mov      r7, r0
004afdfc: sub      sp, sp, #0x1c
004afe00: bl       #0x4a5be4
004afe04: mov      r0, r7
004afe08: bl       #0x313a90
004afe0c: ldr      r6, [pc, #0x16c]
004afe10: mov      r3, #1
004afe14: cmp      r3, #0
004afe18: add      r6, pc, r6
004afe1c: str      r0, [sp, #0x14]
004afe20: str      r3, [sp, #0xc]
004afe24: bne      #0x4afe74
004afe28: add      r3, sp, #0x14
004afe2c: add      r2, r3, #2
004afe30: add      r3, r3, #1
004afe34: ldrb     r0, [r2, #1]
004afe38: ldrb     r1, [r3, #-1]
004afe3c: cmp      r2, r3
004afe40: mov      r4, r2
004afe44: eor      r1, r0, r1
004afe48: strb     r1, [r3, #-1]
004afe4c: ldrb     r0, [r2, #1]
004afe50: eor      r1, r1, r0
004afe54: strb     r1, [r2, #1]
004afe58: ldrb     r0, [r3, #-1]
004afe5c: sub      r2, r2, #1
004afe60: eor      r1, r1, r0
004afe64: strb     r1, [r3, #-1]
004afe68: add      r3, r3, #1
004afe6c: bhi      #0x4afe34
004afe70: ldr      r0, [sp, #0x14]
004afe74: ldr      r3, [pc, #0x108]
004afe78: ldr      r3, [r6, r3]
004afe7c: ldr      r3, [r3]
004afe80: cmp      r3, r0
004afe84: beq      #0x4afe90
004afe88: add      sp, sp, #0x1c
004afe8c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004afe90: lsl      r0, r0, #2
004afe94: mov      r1, #1
004afe98: bl       #0x31056c
004afe9c: ldr      sb, [pc, #0xe4]
004afea0: ldr      r2, [sp, #0x14]
004afea4: ldr      r3, [r6, sb]
004afea8: cmp      r2, #0
004afeac: str      r0, [r3]
004afeb0: beq      #0x4afe88
004afeb4: add      sl, sp, #0x10
004afeb8: mov      r8, #1
004afebc: add      r1, sl, r8
004afec0: add      r3, sl, #2
004afec4: mov      r4, #0
004afec8: stm      sp, {r1, r3}
004afecc: mov      r0, r7
004afed0: mov      r1, sl
004afed4: bl       #0x3df1a0
004afed8: cmp      r8, #0
004afedc: str      r8, [sp, #0xc]
004afee0: bne      #0x4aff24
004afee4: ldr      r3, [sp]
004afee8: ldr      r2, [sp, #4]
004afeec: ldrb     r0, [r2, #1]
004afef0: ldrb     r1, [r3, #-1]
004afef4: cmp      r2, r3
004afef8: eor      r1, r0, r1
004afefc: strb     r1, [r3, #-1]
004aff00: ldrb     r0, [r2, #1]
004aff04: eor      r1, r1, r0
004aff08: strb     r1, [r2, #1]
004aff0c: ldrb     r0, [r3, #-1]
004aff10: sub      r2, r2, #1
004aff14: eor      r1, r1, r0
004aff18: strb     r1, [r3, #-1]
004aff1c: add      r3, r3, #1
004aff20: bhi      #0x4afeec
004aff24: ldr      r0, [sp, #0x10]
004aff28: ldr      r5, [r6, sb]
004aff2c: mov      r1, #1
004aff30: add      r0, r0, r1
004aff34: ldr      fp, [r5]
004aff38: bl       #0x31056c
004aff3c: str      r0, [fp, r4, lsl #2]
004aff40: ldr      r3, [r5]
004aff44: ldr      r2, [sp, #0x10]
004aff48: mov      r0, r7
004aff4c: ldr      r1, [r3, r4, lsl #2]
004aff50: mov      r3, #0
004aff54: bl       #0x317454
004aff58: ldr      r3, [r5]
004aff5c: mov      r1, #0
004aff60: ldr      r2, [r3, r4, lsl #2]
004aff64: ldr      r3, [sp, #0x10]
004aff68: add      r4, r4, #1
004aff6c: strb     r1, [r2, r3]
004aff70: ldr      r3, [sp, #0x14]
004aff74: cmp      r3, r4
004aff78: bhi      #0x4afecc
004aff7c: b        #0x4afe88
004aff80: subeq    r4, lr, r8, ror ip
004aff84: andeq    r3, r0, r0, lsr #10
004aff88: andeq    r2, r0, r8, ror lr

# _ZN6Arrays9LootTable4readEP11IStreamBase
004b9e8c: push     {r4, r5, r6, r7, r8, sl, lr}
004b9e90: sub      sp, sp, #0xc
004b9e94: mov      sl, r0
004b9e98: bl       #0x313a90
004b9e9c: ldr      r6, [pc, #0x12c]
004b9ea0: mov      r3, #1
004b9ea4: cmp      r3, #0
004b9ea8: str      r0, [sp, #4]
004b9eac: str      r3, [sp]
004b9eb0: add      r6, pc, r6
004b9eb4: bne      #0x4b9efc
004b9eb8: add      r3, sp, #4
004b9ebc: add      r2, r3, #2
004b9ec0: add      r3, r3, #1
004b9ec4: ldrb     r0, [r2, #1]
004b9ec8: ldrb     r1, [r3, #-1]
004b9ecc: cmp      r2, r3
004b9ed0: eor      r1, r0, r1
004b9ed4: strb     r1, [r3, #-1]
004b9ed8: ldrb     r0, [r2, #1]
004b9edc: eor      r1, r1, r0
004b9ee0: strb     r1, [r2, #1]
004b9ee4: ldrb     r0, [r3, #-1]
004b9ee8: sub      r2, r2, #1
004b9eec: eor      r1, r1, r0
004b9ef0: strb     r1, [r3, #-1]
004b9ef4: add      r3, r3, #1
004b9ef8: bhi      #0x4b9ec4
004b9efc: bl       #0x4a5c80
004b9f00: ldr      r7, [pc, #0xcc]
004b9f04: ldr      r4, [sp, #4]
004b9f08: mov      r5, #0x24
004b9f0c: ldr      r3, [r6, r7]
004b9f10: mul      r0, r5, r4
004b9f14: str      r4, [r3]
004b9f18: add      r0, r0, #8
004b9f1c: mov      r1, #1
004b9f20: bl       #0x31056c
004b9f24: cmp      r4, #0
004b9f28: str      r5, [r0]
004b9f2c: str      r4, [r0, #4]
004b9f30: add      r3, r0, #8
004b9f34: beq      #0x4b9f6c
004b9f38: ldr      r1, [pc, #0x98]
004b9f3c: mov      r2, #0
004b9f40: ldr      ip, [r6, r1]
004b9f44: mov      r1, r2
004b9f48: add      ip, ip, #8
004b9f4c: add      r2, r2, #1
004b9f50: cmp      r2, r4
004b9f54: str      ip, [r0, #8]
004b9f58: str      r1, [r0, #0x18]
004b9f5c: str      r1, [r0, #0x20]
004b9f60: str      r1, [r0, #0x28]
004b9f64: add      r0, r0, #0x24
004b9f68: bne      #0x4b9f4c
004b9f6c: ldr      r2, [r6, r7]
004b9f70: ldr      r8, [pc, #0x64]
004b9f74: ldr      r1, [r2]
004b9f78: ldr      r2, [r6, r8]
004b9f7c: cmp      r1, #0
004b9f80: str      r3, [r2]
004b9f84: beq      #0x4b9fc8
004b9f88: mov      r4, #0
004b9f8c: mov      r5, r4
004b9f90: b        #0x4b9f9c
004b9f94: ldr      r3, [r6, r8]
004b9f98: ldr      r3, [r3]
004b9f9c: add      r0, r3, r4
004b9fa0: mov      r1, sl
004b9fa4: ldr      r3, [r3, r4]
004b9fa8: mov      lr, pc
004b9fac: ldr      pc, [r3, #0xc]
004b9fb0: ldr      r3, [r6, r7]
004b9fb4: add      r5, r5, #1
004b9fb8: add      r4, r4, #0x24
004b9fbc: ldr      r3, [r3]
004b9fc0: cmp      r3, r5
004b9fc4: bhi      #0x4b9f94
004b9fc8: add      sp, sp, #0xc
004b9fcc: pop      {r4, r5, r6, r7, r8, sl, pc}
004b9fd0: subeq    sl, sp, r0, ror #23
004b9fd4: andeq    r3, r0, r0, lsr #10
004b9fd8: andeq    r3, r0, r0, ror r4
004b9fdc: strdeq   r1, r2, [r0], -ip

# _ZN13ItemInventory7AddLootEiiiib
0040407c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00404080: ldr      r6, [pc, #0x5a0]
00404084: ldr      ip, [pc, #0x5a0]
00404088: subs     r5, r1, #0
0040408c: add      r6, pc, r6
00404090: ldr      r1, [r6, ip]
00404094: sub      sp, sp, #0x1d4
00404098: str      r2, [sp, #0xc]
0040409c: ldr      r2, [r1]
004040a0: str      ip, [sp, #0x1c]
004040a4: mov      sb, r0
004040a8: str      r3, [sp, #0x10]
004040ac: str      r2, [sp, #0x1cc]
004040b0: ldrb     fp, [sp, #0x1fc]
004040b4: blt      #0x4040cc
004040b8: ldr      r3, [pc, #0x570]
004040bc: ldr      r3, [r6, r3]
004040c0: ldr      r3, [r3]
004040c4: cmp      r5, r3
004040c8: blt      #0x4040ec
004040cc: ldr      r2, [sp, #0x1c]
004040d0: ldr      r3, [r6, r2]
004040d4: ldr      r2, [sp, #0x1cc]
004040d8: ldr      r3, [r3]
004040dc: cmp      r2, r3
004040e0: bne      #0x404624
004040e4: add      sp, sp, #0x1d4
004040e8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004040ec: ldr      r3, [pc, #0x540]
004040f0: add      r4, sp, #0x1b4
004040f4: ldr      r7, [r6, r3]
004040f8: mov      r0, r7
004040fc: bl       #0x337888
00404100: ldr      r1, [pc, #0x530]
00404104: add      r2, sp, #0x80
00404108: mov      r0, r4
0040410c: add      r1, pc, r1
00404110: bl       #0x3140ec
00404114: mov      r1, r4
00404118: mov      r0, r7
0040411c: bl       #0x337a88
00404120: mov      r8, r0
00404124: mov      r0, r4
00404128: bl       #0x318254
0040412c: cmp      r8, #0
00404130: bne      #0x4040cc
00404134: ldr      sl, [pc, #0x500]
00404138: add      r4, sp, #0x19c
0040413c: mov      r0, r7
00404140: add      sl, pc, sl
00404144: bl       #0x337888
00404148: add      r2, sp, #0x7c
0040414c: mov      r0, r4
00404150: mov      r1, sl
00404154: bl       #0x3140ec
00404158: mov      r1, r4
0040415c: mov      r0, r7
00404160: bl       #0x337a88
00404164: mov      r0, r4
00404168: bl       #0x318254
0040416c: add      r4, sp, #0x184
00404170: mov      r0, r7
00404174: bl       #0x337888
00404178: add      r2, sp, #0x78
0040417c: mov      r1, sl
00404180: mov      r0, r4
00404184: bl       #0x3140ec
00404188: mov      r1, r4
0040418c: mov      r0, r7
00404190: bl       #0x337a88
00404194: add      r1, sp, #0x4c
00404198: mov      r0, r4
0040419c: str      r1, [sp, #0x30]
004041a0: bl       #0x318254
004041a4: mov      r2, r8
004041a8: mov      r0, r5
004041ac: ldr      r1, [sp, #0x30]
004041b0: str      r8, [sp, #0x4c]
004041b4: str      r8, [sp, #0x50]
004041b8: str      r8, [sp, #0x54]
004041bc: bl       #0x4039a0
004041c0: ldr      r3, [sp, #0x4c]
004041c4: ldr      r2, [sp, #0x50]
004041c8: rsb      r3, r3, r2
004041cc: lsrs     r3, r3, #2
004041d0: bne      #0x404240
004041d4: add      r4, sp, #0x16c
004041d8: mov      r0, r7
004041dc: bl       #0x337888
004041e0: add      r2, sp, #0x74
004041e4: mov      r0, r4
004041e8: mov      r1, sl
004041ec: bl       #0x3140ec
004041f0: mov      r1, r4
004041f4: mov      r0, r7
004041f8: bl       #0x337a88
004041fc: mov      r0, r4
00404200: bl       #0x318254
00404204: add      r4, sp, #0x154
00404208: mov      r0, r7
0040420c: bl       #0x337888
00404210: mov      r1, sl
00404214: add      r2, sp, #0x70
00404218: mov      r0, r4
0040421c: bl       #0x3140ec
00404220: mov      r0, r7
00404224: mov      r1, r4
00404228: bl       #0x337a88
0040422c: mov      r0, r4
00404230: bl       #0x318254
00404234: ldr      r0, [sp, #0x30]
00404238: bl       #0x4024bc
0040423c: b        #0x4040cc
00404240: add      r4, sp, #0x13c
00404244: mov      r0, r7
00404248: bl       #0x337888
0040424c: add      r2, sp, #0x6c
00404250: mov      r0, r4
00404254: mov      r1, sl
00404258: bl       #0x3140ec
0040425c: mov      r1, r4
00404260: mov      r0, r7
00404264: bl       #0x337a88
00404268: mov      r0, r4
0040426c: bl       #0x318254
00404270: add      r4, sp, #0x124
00404274: mov      r0, r7
00404278: bl       #0x337888
0040427c: add      r2, sp, #0x68
00404280: mov      r1, sl
00404284: mov      r0, r4
00404288: bl       #0x3140ec
0040428c: mov      r1, r4
00404290: mov      r0, r7
00404294: bl       #0x337a88
00404298: add      r2, sp, #0x40
0040429c: mov      r0, r4
004042a0: str      r2, [sp, #0x34]
004042a4: bl       #0x318254
004042a8: ldrb     r2, [sb, #0x2d]
004042ac: ldr      r0, [sp, #0x30]
004042b0: ldr      r1, [sp, #0x34]
004042b4: str      r8, [sp, #0x48]
004042b8: str      r8, [sp, #0x40]
004042bc: str      r8, [sp, #0x44]
004042c0: bl       #0x402dfc
004042c4: ldr      r3, [sp, #0x40]
004042c8: ldr      r2, [sp, #0x44]
004042cc: rsb      r3, r3, r2
004042d0: lsrs     r3, r3, #4
004042d4: beq      #0x404550
004042d8: add      r4, sp, #0xdc
004042dc: mov      r0, r7
004042e0: bl       #0x337888
004042e4: add      r2, sp, #0x5c
004042e8: mov      r0, r4
004042ec: mov      r1, sl
004042f0: bl       #0x3140ec
004042f4: mov      r1, r4
004042f8: mov      r0, r7
004042fc: bl       #0x337a88
00404300: mov      r0, r4
00404304: bl       #0x318254
00404308: add      r4, sp, #0xc4
0040430c: mov      r0, r7
00404310: bl       #0x337888
00404314: add      r2, sp, #0x58
00404318: mov      r1, sl
0040431c: mov      r0, r4
00404320: bl       #0x3140ec
00404324: mov      r1, r4
00404328: mov      r0, r7
0040432c: bl       #0x337a88
00404330: mov      r0, r4
00404334: bl       #0x318254
00404338: ldr      r4, [sp, #0x40]
0040433c: ldr      r3, [sp, #0x44]
00404340: cmp      r4, r3
00404344: beq      #0x4044dc
00404348: ldr      r2, [pc, #0x2f0]
0040434c: ldr      r1, [pc, #0x2f0]
00404350: ldr      ip, [pc, #0x2f0]
00404354: str      r2, [sp, #0x20]
00404358: ldr      r2, [pc, #0x2ec]
0040435c: str      fp, [sp, #0x18]
00404360: str      ip, [sp, #0x24]
00404364: add      r2, pc, r2
00404368: str      r2, [sp, #0x3c]
0040436c: ldr      r2, [pc, #0x2dc]
00404370: add      r4, r4, #0x10
00404374: str      sb, [sp, #0x14]
00404378: add      r2, pc, r2
0040437c: str      r2, [sp, #0x38]
00404380: mov      r8, r6
00404384: mov      fp, r1
00404388: ldr      r2, [r4, #-4]
0040438c: ldr      r2, [r2, #0x10]
00404390: sub      r2, r2, #2
00404394: cmp      r2, #1
00404398: movhi    sb, #1
0040439c: bhi      #0x4043b4
004043a0: ldr      r2, [r8, fp]
004043a4: ldr      r2, [r2, #0x40]
004043a8: ldr      sb, [r2, #0x6c4]
004043ac: cmp      sb, #0
004043b0: ble      #0x4044cc
004043b4: ldr      r3, [pc, #0x298]
004043b8: mov      r6, #0
004043bc: str      r3, [sp, #0x2c]
004043c0: ldr      sl, [r8, fp]
004043c4: ldrsh    r5, [r4, #-0x10]
004043c8: mov      r0, sl
004043cc: bl       #0x31f594
004043d0: subs     r7, r0, #0
004043d4: beq      #0x4043e4
004043d8: mov      r0, sl
004043dc: bl       #0x31f594
004043e0: ldr      r7, [r0, #0x118]
004043e4: ldr      ip, [sp, #0x18]
004043e8: cmp      ip, #0
004043ec: bne      #0x404418
004043f0: ldr      r1, [sp, #0x24]
004043f4: add      sl, r5, #2
004043f8: ldr      r3, [r8, r1]
004043fc: ldr      r3, [r3]
00404400: cmp      sl, r3
00404404: bhs      #0x404418
00404408: cmp      r7, #1
0040440c: beq      #0x4045bc
00404410: cmp      r7, #2
00404414: beq      #0x4044f0
00404418: mov      sl, r5
0040441c: ldrsh    r3, [r4, #-0x10]
00404420: cmp      r3, sl
00404424: beq      #0x40444c
00404428: ldr      ip, [sp, #0x20]
0040442c: mov      r1, #0xa4
00404430: ldr      r3, [r8, ip]
00404434: ldr      r2, [r3]
00404438: ldrb     r3, [r4, #-8]
0040443c: strh     sl, [r4, #-0x10]
00404440: mla      r2, r1, sl, r2
00404444: strb     r3, [r4, #-8]
00404448: str      r2, [r4, #-4]
0040444c: mov      r1, #0
00404450: mov      r0, #0x6c
00404454: bl       #0x310570
00404458: mov      r2, #1
0040445c: mov      r1, sl
00404460: mov      r5, r0
00404464: bl       #0x3fc26c
00404468: ldrsb    r1, [r4, #-8]
0040446c: mov      r0, r5
00404470: add      r6, r6, #1
00404474: cmn      r1, #2
00404478: moveq    r1, #0x63
0040447c: strbeq   r1, [r4, #-8]
00404480: moveq    r1, #0x63
00404484: bl       #0x3fa0e4
00404488: ldr      r0, [r4, #-0xc]
0040448c: ldr      r2, [sp, #0x10]
00404490: ldr      r3, [sp, #0x1f8]
00404494: mov      r1, r5
00404498: str      r7, [sp]
0040449c: bl       #0x403310
004044a0: mov      r0, r5
004044a4: ldr      r1, [sp, #0xc]
004044a8: bl       #0x4020b4
004044ac: ldr      r0, [sp, #0x14]
004044b0: mov      r1, r5
004044b4: mov      r2, #1
004044b8: mov      r3, #0
004044bc: bl       #0x3ff5d4
004044c0: cmp      r6, sb
004044c4: bne      #0x4043c0
004044c8: ldr      r3, [sp, #0x44]
004044cc: cmp      r3, r4
004044d0: add      r4, r4, #0x10
004044d4: bne      #0x404388
004044d8: mov      r6, r8
004044dc: ldr      r0, [sp, #0x34]
004044e0: bl       #0x40247c
004044e4: ldr      r0, [sp, #0x30]
004044e8: bl       #0x4024bc
004044ec: b        #0x4040cc
004044f0: ldr      r3, [sp, #0x2c]
004044f4: add      r1, sp, #0x84
004044f8: str      r1, [sp, #0x28]
004044fc: ldr      ip, [r8, r3]
00404500: mov      r0, r1
00404504: ldr      r3, [ip]
00404508: ldr      r1, [r3, r5, lsl #2]
0040450c: str      ip, [sp, #8]
00404510: bl       #0x30e520
00404514: ldr      r0, [sp, #0x28]
00404518: bl       #0x30de54
0040451c: ldr      r2, [sp, #0x28]
00404520: ldr      r1, [sp, #0x3c]
00404524: add      r0, r2, r0
00404528: mov      r2, #0xa
0040452c: bl       #0x30e868
00404530: ldr      ip, [sp, #8]
00404534: ldr      r0, [sp, #0x28]
00404538: ldr      r3, [ip]
0040453c: ldr      r1, [r3, sl, lsl #2]
00404540: bl       #0x30e31c
00404544: cmp      r0, #0
00404548: beq      #0x40441c
0040454c: b        #0x404418
00404550: add      r4, sp, #0x10c
00404554: mov      r0, r7
00404558: bl       #0x337888
0040455c: add      r2, sp, #0x64
00404560: mov      r1, sl
00404564: mov      r0, r4
00404568: bl       #0x3140ec
0040456c: mov      r1, r4
00404570: mov      r0, r7
00404574: bl       #0x337a88
00404578: mov      r0, r4
0040457c: bl       #0x318254
00404580: add      r4, sp, #0xf4
00404584: mov      r0, r7
00404588: bl       #0x337888
0040458c: add      r2, sp, #0x60
00404590: mov      r1, sl
00404594: mov      r0, r4
00404598: bl       #0x3140ec
0040459c: mov      r1, r4
004045a0: mov      r0, r7
004045a4: bl       #0x337a88
004045a8: mov      r0, r4
004045ac: bl       #0x318254
004045b0: ldr      r0, [sp, #0x34]
004045b4: bl       #0x40247c
004045b8: b        #0x404234
004045bc: ldr      r2, [sp, #0x2c]
004045c0: add      ip, r5, #1
004045c4: add      sl, sp, #0x84
004045c8: ldr      r3, [r8, r2]
004045cc: str      ip, [sp, #0x28]
004045d0: mov      r0, sl
004045d4: ldr      r2, [r3]
004045d8: ldr      r1, [r2, r5, lsl #2]
004045dc: str      r3, [sp, #8]
004045e0: bl       #0x30e520
004045e4: mov      r0, sl
004045e8: bl       #0x30de54
004045ec: ldr      r1, [sp, #0x38]
004045f0: add      r0, sl, r0
004045f4: mov      r2, #6
004045f8: bl       #0x30e868
004045fc: ldr      r3, [sp, #8]
00404600: ldr      r2, [sp, #0x28]
00404604: mov      r0, sl
00404608: ldr      r3, [r3]
0040460c: ldr      r1, [r3, r2, lsl #2]
00404610: bl       #0x30e31c
00404614: cmp      r0, #0
00404618: ldreq    sl, [sp, #0x28]
0040461c: beq      #0x40441c
00404620: b        #0x404418
00404624: bl       #0x30e310
00404628: subseq   r0, sb, r4, lsl #20
0040462c: andeq    r4, r0, ip, lsr #1
00404630: andeq    r3, r0, r0, lsr #10
00404634: andeq    r0, r0, r4, lsl #17

# _ZN13ItemInventory13_AddLootTableEiRSt6vectorIPKN7Structs9LootEntryESaIS4_EEPS6_
004039a0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004039a4: ldr      fp, [pc, #0x694]
004039a8: ldr      r3, [pc, #0x694]
004039ac: sub      sp, sp, #0x1ac
004039b0: add      fp, pc, fp
004039b4: str      r3, [sp, #0x14]
004039b8: ldr      r3, [fp, r3]
004039bc: subs     sl, r0, #0
004039c0: mov      r4, r1
004039c4: ldr      r3, [r3]
004039c8: mov      r5, r2
004039cc: str      r3, [sp, #0x1a4]
004039d0: blt      #0x403cc0
004039d4: ldr      r3, [pc, #0x66c]
004039d8: ldr      r3, [fp, r3]
004039dc: ldr      r3, [r3]
004039e0: cmp      sl, r3
004039e4: bge      #0x403cc0
004039e8: ldr      r0, [pc, #0x65c]
004039ec: ldr      r8, [pc, #0x65c]
004039f0: add      r7, sp, #0x18c
004039f4: ldr      sb, [fp, r0]
004039f8: add      r8, pc, r8
004039fc: str      r0, [sp, #8]
00403a00: mov      r0, sb
00403a04: bl       #0x337888
00403a08: add      r2, sp, #0x68
00403a0c: mov      r0, r7
00403a10: mov      r1, r8
00403a14: bl       #0x3140ec
00403a18: mov      r1, r7
00403a1c: mov      r0, sb
00403a20: bl       #0x337a88
00403a24: mov      r0, r7
00403a28: bl       #0x318254
00403a2c: add      r6, sp, #0x174
00403a30: mov      r0, sb
00403a34: bl       #0x337888
00403a38: add      r2, sp, #0x64
00403a3c: mov      r0, r6
00403a40: mov      r1, r8
00403a44: bl       #0x3140ec
00403a48: mov      r1, r6
00403a4c: mov      r0, sb
00403a50: bl       #0x337a88
00403a54: mov      r0, r6
00403a58: bl       #0x318254
00403a5c: ldr      r3, [pc, #0x5f0]
00403a60: add      r7, sp, #0x15c
00403a64: mov      r0, sb
00403a68: ldr      r3, [fp, r3]
00403a6c: mov      r6, #0x24
00403a70: ldr      r3, [r3]
00403a74: mla      r6, r6, sl, r3
00403a78: bl       #0x337888
00403a7c: add      r2, sp, #0x60
00403a80: mov      r1, r8
00403a84: mov      r0, r7
00403a88: bl       #0x3140ec
00403a8c: mov      r1, r7
00403a90: mov      r0, sb
00403a94: bl       #0x337a88
00403a98: mov      r0, r7
00403a9c: bl       #0x318254
00403aa0: ldr      r3, [r6, #0x14]
00403aa4: cmp      r3, #0
00403aa8: beq      #0x403b6c
00403aac: mov      r2, r8
00403ab0: mov      r7, #0
00403ab4: add      r3, sp, #0x5c
00403ab8: add      r1, sp, #0x34
00403abc: str      r5, [sp, #0x10]
00403ac0: str      fp, [sp, #0x18]
00403ac4: mov      sl, r7
00403ac8: add      r8, sp, #0x144
00403acc: str      r1, [sp, #0xc]
00403ad0: mov      fp, r3
00403ad4: mov      r5, r2
00403ad8: b        #0x403b00
00403adc: str      r3, [r1]
00403ae0: ldr      r3, [r4, #4]
00403ae4: add      sl, sl, #1
00403ae8: add      r7, r7, #0x24
00403aec: add      r3, r3, #4
00403af0: str      r3, [r4, #4]
00403af4: ldr      r3, [r6, #0x14]
00403af8: cmp      r3, sl
00403afc: bls      #0x403b64
00403b00: mov      r0, sb
00403b04: bl       #0x337888
00403b08: mov      r2, fp
00403b0c: mov      r1, r5
00403b10: mov      r0, r8
00403b14: bl       #0x3140ec
00403b18: mov      r1, r8
00403b1c: mov      r0, sb
00403b20: bl       #0x337a88
00403b24: mov      r0, r8
00403b28: bl       #0x318254
00403b2c: ldr      r3, [r6, #0x18]
00403b30: ldmib    r4, {r1, r2}
00403b34: add      r3, r3, r7
00403b38: str      r3, [sp, #0x34]
00403b3c: cmp      r1, r2
00403b40: bne      #0x403adc
00403b44: mov      r0, r4
00403b48: ldr      r2, [sp, #0xc]
00403b4c: bl       #0x4026dc
00403b50: ldr      r3, [r6, #0x14]
00403b54: add      sl, sl, #1
00403b58: add      r7, r7, #0x24
00403b5c: cmp      r3, sl
00403b60: bhi      #0x403b00
00403b64: ldr      r5, [sp, #0x10]
00403b68: ldr      fp, [sp, #0x18]
00403b6c: ldr      r3, [r6, #0xc]
00403b70: cmp      r3, #0
00403b74: beq      #0x403d18
00403b78: cmp      r5, #0
00403b7c: beq      #0x403f18
00403b80: mov      r7, #0
00403b84: mov      r8, r7
00403b88: add      sl, sp, #0x30
00403b8c: b        #0x403bb4
00403b90: str      r3, [r1]
00403b94: ldr      r3, [r5, #4]
00403b98: add      r8, r8, #1
00403b9c: add      r7, r7, #0x24
00403ba0: add      r3, r3, #4
00403ba4: str      r3, [r5, #4]
00403ba8: ldr      r3, [r6, #0xc]
00403bac: cmp      r3, r8
00403bb0: bls      #0x403bec
00403bb4: ldr      r3, [r6, #0x10]
00403bb8: ldmib    r5, {r1, r2}
00403bbc: add      r3, r3, r7
00403bc0: str      r3, [sp, #0x30]
00403bc4: cmp      r1, r2
00403bc8: bne      #0x403b90
00403bcc: mov      r0, r5
00403bd0: mov      r2, sl
00403bd4: bl       #0x4026dc
00403bd8: ldr      r3, [r6, #0xc]
00403bdc: add      r8, r8, #1
00403be0: add      r7, r7, #0x24
00403be4: cmp      r3, r8
00403be8: bhi      #0x403bb4
00403bec: ldr      r3, [sp, #8]
00403bf0: mov      r7, #0
00403bf4: str      r7, [sp, #0x20]
00403bf8: ldr      sl, [fp, r3]
00403bfc: str      r7, [sp, #0x24]
00403c00: str      r7, [sp, #0x28]
00403c04: mov      r0, sl
00403c08: bl       #0x337888
00403c0c: ldr      r1, [pc, #0x444]
00403c10: add      r8, sp, #0xe4
00403c14: add      r2, sp, #0x4c
00403c18: add      r1, pc, r1
00403c1c: mov      r0, r8
00403c20: bl       #0x3140ec
00403c24: mov      r1, r8
00403c28: mov      r0, sl
00403c2c: bl       #0x337a88
00403c30: mov      r0, r8
00403c34: bl       #0x318254
00403c38: ldr      r3, [r6, #0x1c]
00403c3c: cmp      r3, r7
00403c40: beq      #0x403c74
00403c44: add      r8, sp, #0x20
00403c48: ldr      r3, [r6, #0x20]
00403c4c: cmp      r5, #0
00403c50: moveq    r2, r8
00403c54: movne    r2, r5
00403c58: ldr      r0, [r3, r7, lsl #2]
00403c5c: mov      r1, r4
00403c60: bl       #0x4039a0
00403c64: ldr      r3, [r6, #0x1c]
00403c68: add      r7, r7, #1
00403c6c: cmp      r3, r7
00403c70: bhi      #0x403c48
00403c74: cmp      r5, #0
00403c78: beq      #0x403d60
00403c7c: ldr      r0, [sp, #0x20]
00403c80: cmp      r0, #0
00403c84: beq      #0x403ca0
00403c88: ldr      r1, [sp, #0x28]
00403c8c: rsb      r1, r0, r1
00403c90: bic      r1, r1, #3
00403c94: cmp      r1, #0x80
00403c98: bhi      #0x403d58
00403c9c: bl       #0x708f00
00403ca0: ldr      r1, [sp, #0x14]
00403ca4: ldr      r2, [sp, #0x1a4]
00403ca8: ldr      r3, [fp, r1]
00403cac: ldr      r3, [r3]
00403cb0: cmp      r2, r3
00403cb4: bne      #0x40403c
00403cb8: add      sp, sp, #0x1ac
00403cbc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00403cc0: ldr      r3, [pc, #0x394]
00403cc4: ldr      r3, [fp, r3]
00403cc8: ldr      r3, [r3]
00403ccc: cmp      r3, #2
00403cd0: moveq    r3, #0
00403cd4: streq    r3, [r3]
00403cd8: beq      #0x4039e8
00403cdc: cmp      r3, #1
00403ce0: bne      #0x4039e8
00403ce4: ldr      r0, [pc, #0x374]
00403ce8: ldr      r1, [pc, #0x374]
00403cec: ldr      r2, [pc, #0x374]
00403cf0: ldr      r0, [fp, r0]
00403cf4: ldr      r3, [pc, #0x370]
00403cf8: mov      ip, #0xa0
00403cfc: add      r1, pc, r1
00403d00: add      r2, pc, r2
00403d04: add      r3, pc, r3
00403d08: add      r0, r0, #0xa8
00403d0c: str      ip, [sp]
00403d10: bl       #0x30e004
00403d14: b        #0x4039e8
00403d18: ldr      r2, [sp, #8]
00403d1c: add      r7, sp, #0xfc
00403d20: ldr      r8, [fp, r2]
00403d24: mov      r0, r8
00403d28: bl       #0x337888
00403d2c: ldr      r1, [pc, #0x33c]
00403d30: add      r2, sp, #0x50
00403d34: mov      r0, r7
00403d38: add      r1, pc, r1
00403d3c: bl       #0x3140ec
00403d40: mov      r0, r8
00403d44: mov      r1, r7
00403d48: bl       #0x337a88
00403d4c: mov      r0, r7
00403d50: bl       #0x318254
00403d54: b        #0x403bec
00403d58: bl       #0x310440
00403d5c: b        #0x403ca0
00403d60: ldr      r0, [sp, #0x20]
00403d64: ldr      r3, [sp, #0x24]
00403d68: rsb      r3, r0, r3
00403d6c: lsrs     r3, r3, #2
00403d70: beq      #0x403c80
00403d74: mov      r1, r5
00403d78: ldr      r0, [r6, #8]
00403d7c: bl       #0x401b90
00403d80: str      r0, [sp, #0x10]
00403d84: ldr      r0, [sp, #8]
00403d88: ldr      sb, [pc, #0x2e4]
00403d8c: add      r6, sp, #0xcc
00403d90: ldr      r7, [fp, r0]
00403d94: add      sb, pc, sb
00403d98: mov      r0, r7
00403d9c: bl       #0x337888
00403da0: add      r2, sp, #0x48
00403da4: mov      r0, r6
00403da8: mov      r1, sb
00403dac: bl       #0x3140ec
00403db0: mov      r1, r6
00403db4: mov      r0, r7
00403db8: bl       #0x337a88
00403dbc: mov      r0, r6
00403dc0: bl       #0x318254
00403dc4: add      r6, sp, #0xb4
00403dc8: mov      r0, r7
00403dcc: bl       #0x337888
00403dd0: add      r2, sp, #0x44
00403dd4: mov      r0, r6
00403dd8: mov      r1, sb
00403ddc: bl       #0x3140ec
00403de0: mov      r1, r6
00403de4: mov      r0, r7
00403de8: bl       #0x337a88
00403dec: mov      r0, r6
00403df0: bl       #0x318254
00403df4: add      r6, sp, #0x9c
00403df8: mov      r0, r7
00403dfc: bl       #0x337888
00403e00: add      r2, sp, #0x40
00403e04: mov      r0, r6
00403e08: mov      r1, sb
00403e0c: bl       #0x3140ec
00403e10: mov      r1, r6
00403e14: mov      r0, r7
00403e18: bl       #0x337a88
00403e1c: mov      r0, r6
00403e20: bl       #0x318254
00403e24: add      r6, sp, #0x84
00403e28: mov      r0, r7
00403e2c: bl       #0x337888
00403e30: add      r2, sp, #0x3c
00403e34: mov      r1, sb
00403e38: mov      r0, r6
00403e3c: bl       #0x3140ec
00403e40: mov      r1, r6
00403e44: mov      r0, r7
00403e48: bl       #0x337a88
00403e4c: mov      r0, r6
00403e50: bl       #0x318254
00403e54: ldr      r1, [sp, #0x10]
00403e58: cmp      r1, #0
00403e5c: addeq    sl, sp, #0x20
00403e60: beq      #0x403f08
00403e64: add      r2, sp, #0x38
00403e68: add      sl, sp, #0x20
00403e6c: add      r6, sp, #0x6c
00403e70: str      r2, [sp, #0xc]
00403e74: b        #0x403e9c
00403e78: ldr      r3, [r3, r8, lsl #2]
00403e7c: add      r5, r5, #1
00403e80: str      r3, [r1]
00403e84: ldr      r3, [r4, #4]
00403e88: add      r3, r3, #4
00403e8c: str      r3, [r4, #4]
00403e90: ldr      r0, [sp, #0x10]
00403e94: cmp      r5, r0
00403e98: beq      #0x403f08
00403e9c: mov      r0, sl
00403ea0: bl       #0x4028d0
00403ea4: ldr      r3, [sp, #8]
00403ea8: mov      r8, r0
00403eac: ldr      r7, [fp, r3]
00403eb0: mov      r0, r7
00403eb4: bl       #0x337888
00403eb8: ldr      r2, [sp, #0xc]
00403ebc: mov      r1, sb
00403ec0: mov      r0, r6
00403ec4: bl       #0x3140ec
00403ec8: mov      r1, r6
00403ecc: mov      r0, r7
00403ed0: bl       #0x337a88
00403ed4: mov      r0, r6
00403ed8: bl       #0x318254
00403edc: ldmib    r4, {r1, r2}
00403ee0: ldr      r3, [sp, #0x20]
00403ee4: cmp      r1, r2
00403ee8: add      r2, r3, r8, lsl #2
00403eec: bne      #0x403e78
00403ef0: mov      r0, r4
00403ef4: bl       #0x4026dc
00403ef8: ldr      r0, [sp, #0x10]
00403efc: add      r5, r5, #1
00403f00: cmp      r5, r0
00403f04: bne      #0x403e9c
00403f08: mov      r0, r4
00403f0c: mov      r1, sl
00403f10: bl       #0x402ce4
00403f14: b        #0x403c7c
00403f18: mov      r1, r5
00403f1c: ldr      r0, [r6, #8]
00403f20: bl       #0x401b90
00403f24: ldr      r2, [sp, #8]
00403f28: ldr      r8, [pc, #0x148]
00403f2c: add      r7, sp, #0x12c
00403f30: ldr      sl, [fp, r2]
00403f34: str      r0, [sp, #0xc]
00403f38: add      r8, pc, r8
00403f3c: mov      r0, sl
00403f40: bl       #0x337888
00403f44: add      r2, sp, #0x58
00403f48: mov      r1, r8
00403f4c: mov      r0, r7
00403f50: bl       #0x3140ec
00403f54: mov      r1, r7
00403f58: mov      r0, sl
00403f5c: bl       #0x337a88
00403f60: mov      r0, r7
00403f64: bl       #0x318254
00403f68: ldr      r3, [sp, #0xc]
00403f6c: cmp      r3, #0
00403f70: beq      #0x404028
00403f74: add      r0, sp, #0x54
00403f78: add      r1, sp, #0x2c
00403f7c: mov      sb, r8
00403f80: mov      r7, r5
00403f84: add      r8, sp, #0x114
00403f88: str      r0, [sp, #0x10]
00403f8c: str      r1, [sp, #0x18]
00403f90: str      r5, [sp, #0x1c]
00403f94: b        #0x403fb8
00403f98: str      r2, [r1]
00403f9c: ldr      r3, [r4, #4]
00403fa0: add      r3, r3, #4
00403fa4: str      r3, [r4, #4]
00403fa8: ldr      r1, [sp, #0xc]
00403fac: add      r7, r7, #1
00403fb0: cmp      r7, r1
00403fb4: beq      #0x404024
00403fb8: mov      r0, r6
00403fbc: bl       #0x402a60
00403fc0: ldr      r2, [sp, #8]
00403fc4: mov      sl, r0
00403fc8: ldr      r5, [fp, r2]
00403fcc: mov      r0, r5
00403fd0: bl       #0x337888
00403fd4: ldr      r2, [sp, #0x10]
00403fd8: mov      r1, sb
00403fdc: mov      r0, r8
00403fe0: bl       #0x3140ec
00403fe4: mov      r1, r8
00403fe8: mov      r0, r5
00403fec: bl       #0x337a88
00403ff0: mov      r0, r8
00403ff4: bl       #0x318254
00403ff8: ldr      r2, [r6, #0x10]
00403ffc: ldmib    r4, {r1, r3}
00404000: mov      r0, #0x24
00404004: mla      r2, r0, sl, r2
00404008: cmp      r1, r3
0040400c: str      r2, [sp, #0x2c]
00404010: bne      #0x403f98
00404014: mov      r0, r4
00404018: ldr      r2, [sp, #0x18]
0040401c: bl       #0x4026dc
00404020: b        #0x403fa8
00404024: ldr      r5, [sp, #0x1c]
00404028: mov      r0, r4
0040402c: ldr      r1, [r6, #0x10]
00404030: ldr      r2, [r6, #0xc]
00404034: bl       #0x402d70
00404038: b        #0x403bec
0040403c: bl       #0x30e310
00404040: subseq   r1, sb, r0, ror #1
00404044: andeq    r4, r0, ip, lsr #1
00404048: andeq    r3, r0, r0, lsr #10
0040404c: andeq    r0, r0, r4, lsl #17
00404050: subeq    r3, ip, r0, lsr lr
00404054: strdeq   r1, r2, [r0], -ip
00404058: subeq    r3, ip, r0, lsl ip
0040405c: andeq    r3, r0, r0, asr #19
00404060: andeq    r1, r0, r0, asr #19
00404064: ldrdeq   sl, fp, [fp], #-0x6c
00404068: subeq    r3, ip, r8, asr ip
0040406c: subeq    r3, ip, ip, ror r8
00404070: strdeq   r3, r4, [ip], #-0xa0
00404074: umaaleq  r3, ip, r4, sl
00404078: strdeq   r3, r4, [ip], #-0x80
