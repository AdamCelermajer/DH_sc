
# _ZN6glitch5video16CPrimitiveStream10SMapBufferIKtE5resetERKS1_NS0_24E_BUFFER_READ_MAP_ACCESSE.clone.2
005911a0: push     {r4, r5, r6, lr}
005911a4: ldr      r3, [r0, #4]
005911a8: mov      r4, r0
005911ac: mov      r5, r1
005911b0: cmp      r3, #0
005911b4: beq      #0x5911ec
005911b8: ldr      r3, [r0]
005911bc: ldr      r6, [r3]
005911c0: ldrb     r3, [r6, #0x13]
005911c4: and      r2, r3, #0x1f
005911c8: cmp      r2, #1
005911cc: bls      #0x59120c
005911d0: sub      r2, r2, #1
005911d4: bic      r3, r3, #0x1f
005911d8: orr      r3, r2, r3
005911dc: strb     r3, [r6, #0x13]
005911e0: mov      r3, #0
005911e4: str      r3, [r4, #4]
005911e8: str      r3, [r4]
005911ec: str      r5, [r4]
005911f0: ldr      r0, [r5]
005911f4: mov      r1, #1
005911f8: bl       #0x5a1adc
005911fc: ldr      r3, [r5, #4]
00591200: add      r3, r0, r3
00591204: str      r3, [r4, #4]
00591208: pop      {r4, r5, r6, pc}
0059120c: ldrb     r3, [r6, #0x12]
00591210: tst      r3, #0x20
00591214: bne      #0x591224
00591218: mov      r3, #0
0059121c: strb     r3, [r6, #0x13]
00591220: b        #0x5911e0
00591224: ldr      r3, [r6]
00591228: mov      r0, r6
0059122c: mov      lr, pc
00591230: ldr      pc, [r3, #0x18]
00591234: b        #0x591218

# _ZNSt6vectorIN6glitch4core10triangle3dIfEENS1_10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE22_M_insert_overflow_auxEPS3_RKS3_RKSt12__false_typejb.clone.3
00591238: push     {r4, r5, r6, r7, r8, lr}
0059123c: mov      r4, r0
00591240: ldm      r0, {r0, ip}
00591244: mov      r5, r1
00591248: mov      r6, r2
0059124c: rsb      r0, r0, ip
00591250: asr      r0, r0, #2
00591254: movw     r3, #0x71c7
00591258: lsl      r1, r0, #3
0059125c: rsb      r1, r0, r1
00591260: add      r1, r1, r1, lsl #6
00591264: orr      r3, r3, r3, lsl #12
00591268: add      r1, r0, r1, lsl #3
0059126c: lsl      r2, r1, #0xf
00591270: rsb      r1, r1, r2
00591274: add      r0, r0, r1, lsl #3
00591278: cmp      r0, #1
0059127c: addhs    r2, r0, r0
00591280: addlo    r2, r0, #1
00591284: cmp      r2, r3
00591288: bhi      #0x591294
0059128c: cmp      r0, r2
00591290: bls      #0x5913f0
00591294: mvn      r7, #3
00591298: mov      r1, #0
0059129c: mov      r0, r7
005912a0: bl       #0x310568
005912a4: ldr      r3, [r4]
005912a8: mov      r8, r0
005912ac: rsb      r5, r3, r5
005912b0: asr      r2, r5, #2
005912b4: lsl      r1, r2, #3
005912b8: rsb      r1, r2, r1
005912bc: add      r1, r1, r1, lsl #6
005912c0: add      r1, r2, r1, lsl #3
005912c4: lsl      r5, r1, #0xf
005912c8: rsb      r5, r1, r5
005912cc: add      r5, r2, r5, lsl #3
005912d0: cmp      r5, #0
005912d4: movle    r3, r0
005912d8: ble      #0x591344
005912dc: mov      r1, r5
005912e0: mov      r2, r0
005912e4: ldr      r0, [r3]
005912e8: subs     r1, r1, #1
005912ec: str      r0, [r2]
005912f0: ldr      r0, [r3, #4]
005912f4: str      r0, [r2, #4]
005912f8: ldr      r0, [r3, #8]
005912fc: str      r0, [r2, #8]
00591300: ldr      r0, [r3, #0xc]
00591304: str      r0, [r2, #0xc]
00591308: ldr      r0, [r3, #0x10]
0059130c: str      r0, [r2, #0x10]
00591310: ldr      r0, [r3, #0x14]
00591314: str      r0, [r2, #0x14]
00591318: ldr      r0, [r3, #0x18]
0059131c: str      r0, [r2, #0x18]
00591320: ldr      r0, [r3, #0x1c]
00591324: str      r0, [r2, #0x1c]
00591328: ldr      r0, [r3, #0x20]
0059132c: add      r3, r3, #0x24
00591330: str      r0, [r2, #0x20]
00591334: add      r2, r2, #0x24
00591338: bne      #0x5912e4
0059133c: mov      r3, #0x24
00591340: mla      r3, r3, r5, r8
00591344: ldr      r2, [r6]
00591348: add      r5, r3, #0x24
0059134c: str      r2, [r3]
00591350: ldr      r2, [r6, #4]
00591354: str      r2, [r3, #4]
00591358: ldr      r2, [r6, #8]
0059135c: str      r2, [r3, #8]
00591360: ldr      r2, [r6, #0xc]
00591364: str      r2, [r3, #0xc]
00591368: ldr      r2, [r6, #0x10]
0059136c: str      r2, [r3, #0x10]
00591370: ldr      r2, [r6, #0x14]
00591374: str      r2, [r3, #0x14]
00591378: ldr      r2, [r6, #0x18]
0059137c: str      r2, [r3, #0x18]
00591380: ldr      r2, [r6, #0x1c]
00591384: str      r2, [r3, #0x1c]
00591388: ldr      r2, [r6, #0x20]
0059138c: str      r2, [r3, #0x20]
00591390: ldr      r0, [r4, #4]
00591394: ldr      r2, [r4]
00591398: cmp      r0, r2
0059139c: beq      #0x5913dc
005913a0: sub      r3, r0, #0x24
005913a4: rsb      r3, r2, r3
005913a8: lsr      r3, r3, #2
005913ac: lsl      r2, r3, #3
005913b0: rsb      r2, r3, r2
005913b4: add      r2, r2, r2, lsl #6
005913b8: add      r2, r3, r2, lsl #3
005913bc: lsl      r1, r2, #0xf
005913c0: rsb      r2, r2, r1
005913c4: add      r3, r3, r2, lsl #3
005913c8: bic      r3, r3, #0xc0000000
005913cc: mvn      r2, #0x23
005913d0: mul      r3, r2, r3
005913d4: add      r3, r3, r2
005913d8: add      r0, r0, r3
005913dc: add      r7, r8, r7
005913e0: bl       #0x310450
005913e4: stmib    r4, {r5, r7}
005913e8: str      r8, [r4]
005913ec: pop      {r4, r5, r6, r7, r8, pc}
005913f0: mov      r7, #0x24
005913f4: mul      r7, r7, r2
005913f8: b        #0x591298

# _ZN6glitch5scene12_GLOBAL__N_115createTrianglesIfSt6vectorINS_4core10triangle3dIfEENS4_10SAllocatorIS6_LNS_6memory13E_MEMORY_HINTE0EEEEEEvPKtjRKNS_5video13SVertexStreamERT0_
00591d38: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00591d3c: mov      r5, r2
00591d40: ldrh     r2, [r2, #0xc]
00591d44: sub      sp, sp, #0xf4
00591d48: mov      r4, r0
00591d4c: cmp      r2, #3
00591d50: mov      r6, r1
00591d54: mov      r7, r3
00591d58: beq      #0x591fdc
00591d5c: cmp      r2, #4
00591d60: beq      #0x591eb0
00591d64: cmp      r2, #2
00591d68: beq      #0x591d74
00591d6c: add      sp, sp, #0xf4
00591d70: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00591d74: ldr      r0, [r5]
00591d78: mov      r1, #1
00591d7c: bl       #0x5a1adc
00591d80: ldr      r8, [r5, #4]
00591d84: cmp      r4, #0
00591d88: add      r8, r0, r8
00591d8c: beq      #0x592160
00591d90: add      fp, r4, r6, lsl #1
00591d94: cmp      r4, fp
00591d98: beq      #0x591e80
00591d9c: add      r0, sp, #0xa8
00591da0: mov      r6, #0
00591da4: str      r0, [sp, #0xc]
00591da8: str      fp, [sp, #0x10]
00591dac: ldrh     r2, [r5, #0xe]
00591db0: ldrh     r1, [r4, #2]
00591db4: ldrh     r0, [r4, #4]
00591db8: ldrh     r3, [r4]
00591dbc: ldr      fp, [r7, #8]
00591dc0: mul      r0, r0, r2
00591dc4: mul      r3, r2, r3
00591dc8: mul      r2, r2, r1
00591dcc: add      sb, r8, r0
00591dd0: ldr      r1, [r7, #4]
00591dd4: add      sl, r8, r2
00591dd8: add      ip, r8, r3
00591ddc: ldr      r0, [r8, r0]
00591de0: ldr      sb, [sb, #4]
00591de4: ldr      r2, [r8, r2]
00591de8: ldr      sl, [sl, #4]
00591dec: ldr      r3, [r8, r3]
00591df0: ldr      ip, [ip, #4]
00591df4: cmp      r1, fp
00591df8: str      sb, [sp, #0xac]
00591dfc: str      r2, [sp, #0xb4]
00591e00: str      sl, [sp, #0xb8]
00591e04: str      r3, [sp, #0xc0]
00591e08: str      ip, [sp, #0xc4]
00591e0c: str      r6, [sp, #0xb0]
00591e10: str      r6, [sp, #0xbc]
00591e14: str      r0, [sp, #0xa8]
00591e18: str      r6, [sp, #0xc8]
00591e1c: beq      #0x592150
00591e20: str      r0, [r1]
00591e24: ldr      r3, [sp, #0xac]
00591e28: str      r3, [r1, #4]
00591e2c: ldr      r3, [sp, #0xb0]
00591e30: str      r3, [r1, #8]
00591e34: ldr      r3, [sp, #0xb4]
00591e38: str      r3, [r1, #0xc]
00591e3c: ldr      r3, [sp, #0xb8]
00591e40: str      r3, [r1, #0x10]
00591e44: ldr      r3, [sp, #0xbc]
00591e48: str      r3, [r1, #0x14]
00591e4c: ldr      r3, [sp, #0xc0]
00591e50: str      r3, [r1, #0x18]
00591e54: ldr      r3, [sp, #0xc4]
00591e58: str      r3, [r1, #0x1c]
00591e5c: ldr      r3, [sp, #0xc8]
00591e60: str      r3, [r1, #0x20]
00591e64: ldr      r3, [r7, #4]
00591e68: add      r3, r3, #0x24
00591e6c: str      r3, [r7, #4]
00591e70: ldr      ip, [sp, #0x10]
00591e74: add      r4, r4, #6
00591e78: cmp      ip, r4
00591e7c: bne      #0x591dac
00591e80: cmp      r8, #0
00591e84: beq      #0x591d6c
00591e88: ldr      r4, [r5]
00591e8c: ldrb     r3, [r4, #0x13]
00591e90: and      r2, r3, #0x1f
00591e94: cmp      r2, #1
00591e98: bls      #0x592104
00591e9c: sub      r2, r2, #1
00591ea0: bic      r3, r3, #0x1f
00591ea4: orr      r3, r2, r3
00591ea8: strb     r3, [r4, #0x13]
00591eac: b        #0x591d6c
00591eb0: ldr      r0, [r5]
00591eb4: mov      r1, #1
00591eb8: bl       #0x5a1adc
00591ebc: ldr      r8, [r5, #4]
00591ec0: cmp      r4, #0
00591ec4: add      r8, r0, r8
00591ec8: beq      #0x592258
00591ecc: add      r6, r4, r6, lsl #1
00591ed0: cmp      r4, r6
00591ed4: str      r6, [sp, #0xc]
00591ed8: beq      #0x591e80
00591edc: add      sl, sp, #0x18
00591ee0: str      sl, [sp, #0x14]
00591ee4: str      r5, [sp, #0x10]
00591ee8: ldr      fp, [sp, #0x10]
00591eec: ldrh     r6, [r4, #2]
00591ef0: ldrh     ip, [r4, #4]
00591ef4: ldrh     r3, [fp, #0xe]
00591ef8: ldrh     r5, [r4]
00591efc: ldr      r1, [r7, #4]
00591f00: mul      r6, r3, r6
00591f04: mul      ip, ip, r3
00591f08: add      r2, r8, r6
00591f0c: ldr      r6, [r8, r6]
00591f10: add      r0, r8, ip
00591f14: ldr      sb, [r0, #8]
00591f18: ldr      ip, [r8, ip]
00591f1c: mul      r5, r3, r5
00591f20: str      r6, [sp, #4]
00591f24: ldr      r0, [r0, #4]
00591f28: ldr      sl, [r7, #8]
00591f2c: add      r3, r8, r5
00591f30: ldr      r6, [r2, #8]
00591f34: ldr      fp, [r3, #8]
00591f38: ldr      r5, [r8, r5]
00591f3c: ldr      r2, [r2, #4]
00591f40: ldr      r3, [r3, #4]
00591f44: str      r0, [sp, #0x1c]
00591f48: ldr      r0, [sp, #4]
00591f4c: cmp      r1, sl
00591f50: str      sb, [sp, #0x20]
00591f54: str      r0, [sp, #0x24]
00591f58: str      r2, [sp, #0x28]
00591f5c: str      r6, [sp, #0x2c]
00591f60: str      r5, [sp, #0x30]
00591f64: str      r3, [sp, #0x34]
00591f68: str      fp, [sp, #0x38]
00591f6c: str      ip, [sp, #0x18]
00591f70: beq      #0x592140
00591f74: str      ip, [r1]
00591f78: ldr      r3, [sp, #0x1c]
00591f7c: str      r3, [r1, #4]
00591f80: ldr      r3, [sp, #0x20]
00591f84: str      r3, [r1, #8]
00591f88: ldr      r3, [sp, #0x24]
00591f8c: str      r3, [r1, #0xc]
00591f90: ldr      r3, [sp, #0x28]
00591f94: str      r3, [r1, #0x10]
00591f98: ldr      r3, [sp, #0x2c]
00591f9c: str      r3, [r1, #0x14]
00591fa0: ldr      r3, [sp, #0x30]
00591fa4: str      r3, [r1, #0x18]
00591fa8: ldr      r3, [sp, #0x34]
00591fac: str      r3, [r1, #0x1c]
00591fb0: ldr      r3, [sp, #0x38]
00591fb4: str      r3, [r1, #0x20]
00591fb8: ldr      r3, [r7, #4]
00591fbc: add      r3, r3, #0x24
00591fc0: str      r3, [r7, #4]
00591fc4: ldr      r2, [sp, #0xc]
00591fc8: add      r4, r4, #6
00591fcc: cmp      r2, r4
00591fd0: bne      #0x591ee8
00591fd4: ldr      r5, [sp, #0x10]
00591fd8: b        #0x591e80
00591fdc: ldr      r0, [r5]
00591fe0: mov      r1, #1
00591fe4: bl       #0x5a1adc
00591fe8: ldr      r8, [r5, #4]
00591fec: cmp      r4, #0
00591ff0: add      r8, r0, r8
00591ff4: beq      #0x592368
00591ff8: add      r6, r4, r6, lsl #1
00591ffc: cmp      r4, r6
00592000: str      r6, [sp, #0xc]
00592004: beq      #0x591e80
00592008: add      r0, sp, #0x60
0059200c: str      r0, [sp, #0x14]
00592010: str      r5, [sp, #0x10]
00592014: ldr      r2, [sp, #0x10]
00592018: ldrh     r6, [r4, #2]
0059201c: ldrh     ip, [r4, #4]
00592020: ldrh     r3, [r2, #0xe]
00592024: ldrh     r5, [r4]
00592028: ldr      r1, [r7, #4]
0059202c: mul      r6, r3, r6
00592030: mul      ip, ip, r3
00592034: add      r2, r8, r6
00592038: ldr      r6, [r8, r6]
0059203c: add      r0, r8, ip
00592040: ldr      sb, [r0, #8]
00592044: ldr      ip, [r8, ip]
00592048: mul      r5, r3, r5
0059204c: str      r6, [sp, #4]
00592050: ldr      r0, [r0, #4]
00592054: ldr      sl, [r7, #8]
00592058: add      r3, r8, r5
0059205c: ldr      r6, [r2, #8]
00592060: ldr      fp, [r3, #8]
00592064: ldr      r5, [r8, r5]
00592068: ldr      r2, [r2, #4]
0059206c: ldr      r3, [r3, #4]
00592070: str      r0, [sp, #0x64]
00592074: ldr      r0, [sp, #4]
00592078: cmp      r1, sl
0059207c: str      sb, [sp, #0x68]
00592080: str      r0, [sp, #0x6c]
00592084: str      r2, [sp, #0x70]
00592088: str      r6, [sp, #0x74]
0059208c: str      r5, [sp, #0x78]
00592090: str      r3, [sp, #0x7c]
00592094: str      fp, [sp, #0x80]
00592098: str      ip, [sp, #0x60]
0059209c: beq      #0x592130
005920a0: str      ip, [r1]
005920a4: ldr      r3, [sp, #0x64]
005920a8: str      r3, [r1, #4]
005920ac: ldr      r3, [sp, #0x68]
005920b0: str      r3, [r1, #8]
005920b4: ldr      r3, [sp, #0x6c]
005920b8: str      r3, [r1, #0xc]
005920bc: ldr      r3, [sp, #0x70]
005920c0: str      r3, [r1, #0x10]
005920c4: ldr      r3, [sp, #0x74]
005920c8: str      r3, [r1, #0x14]
005920cc: ldr      r3, [sp, #0x78]
005920d0: str      r3, [r1, #0x18]
005920d4: ldr      r3, [sp, #0x7c]
005920d8: str      r3, [r1, #0x1c]
005920dc: ldr      r3, [sp, #0x80]
005920e0: str      r3, [r1, #0x20]
005920e4: ldr      r3, [r7, #4]
005920e8: add      r3, r3, #0x24
005920ec: str      r3, [r7, #4]
005920f0: ldr      r2, [sp, #0xc]
005920f4: add      r4, r4, #6
005920f8: cmp      r2, r4
005920fc: bne      #0x592014
00592100: b        #0x591fd4
00592104: ldrb     r3, [r4, #0x12]
00592108: tst      r3, #0x20
0059210c: bne      #0x59211c
00592110: mov      r3, #0
00592114: strb     r3, [r4, #0x13]
00592118: b        #0x591d6c
0059211c: ldr      r3, [r4]
00592120: mov      r0, r4
00592124: mov      lr, pc
00592128: ldr      pc, [r3, #0x18]
0059212c: b        #0x592110
00592130: mov      r0, r7
00592134: ldr      r2, [sp, #0x14]
00592138: bl       #0x591238
0059213c: b        #0x5920f0
00592140: mov      r0, r7
00592144: ldr      r2, [sp, #0x14]
00592148: bl       #0x591238
0059214c: b        #0x591fc4
00592150: mov      r0, r7
00592154: ldr      r2, [sp, #0xc]
00592158: bl       #0x591238
0059215c: b        #0x591e70
00592160: cmp      r6, #0
00592164: beq      #0x591e80
00592168: ldrh     ip, [r5, #0xe]
0059216c: add      r0, sp, #0xcc
00592170: mov      sl, #0
00592174: str      r0, [sp, #0xc]
00592178: mov      r2, ip
0059217c: mov      fp, r6
00592180: str      r5, [sp, #0x10]
00592184: b        #0x592190
00592188: ldr      ip, [sp, #0x10]
0059218c: ldrh     r2, [ip, #0xe]
00592190: add      r0, r4, #2
00592194: mul      r3, r4, r2
00592198: mul      r0, r2, r0
0059219c: mla      r2, r4, r2, r2
005921a0: ldmib    r7, {r1, r5}
005921a4: add      sb, r8, r0
005921a8: add      r6, r8, r2
005921ac: add      ip, r8, r3
005921b0: ldr      r0, [r8, r0]
005921b4: ldr      sb, [sb, #4]
005921b8: ldr      r2, [r8, r2]
005921bc: ldr      r6, [r6, #4]
005921c0: ldr      r3, [r8, r3]
005921c4: ldr      ip, [ip, #4]
005921c8: cmp      r1, r5
005921cc: str      sb, [sp, #0xd0]
005921d0: str      r2, [sp, #0xd8]
005921d4: str      r6, [sp, #0xdc]
005921d8: str      r3, [sp, #0xe4]
005921dc: str      ip, [sp, #0xe8]
005921e0: str      sl, [sp, #0xd4]
005921e4: str      sl, [sp, #0xe0]
005921e8: str      sl, [sp, #0xec]
005921ec: str      r0, [sp, #0xcc]
005921f0: beq      #0x592478
005921f4: str      r0, [r1]
005921f8: ldr      r3, [sp, #0xd0]
005921fc: str      r3, [r1, #4]
00592200: ldr      r3, [sp, #0xd4]
00592204: str      r3, [r1, #8]
00592208: ldr      r3, [sp, #0xd8]
0059220c: str      r3, [r1, #0xc]
00592210: ldr      r3, [sp, #0xdc]
00592214: str      r3, [r1, #0x10]
00592218: ldr      r3, [sp, #0xe0]
0059221c: str      r3, [r1, #0x14]
00592220: ldr      r3, [sp, #0xe4]
00592224: str      r3, [r1, #0x18]
00592228: ldr      r3, [sp, #0xe8]
0059222c: str      r3, [r1, #0x1c]
00592230: ldr      r3, [sp, #0xec]
00592234: str      r3, [r1, #0x20]
00592238: ldr      r3, [r7, #4]
0059223c: add      r3, r3, #0x24
00592240: str      r3, [r7, #4]
00592244: add      r4, r4, #3
00592248: cmp      fp, r4
0059224c: bhi      #0x592188
00592250: ldr      r5, [sp, #0x10]
00592254: b        #0x591e80
00592258: cmp      r6, #0
0059225c: beq      #0x591e80
00592260: ldrh     sb, [r5, #0xe]
00592264: add      r3, sp, #0x3c
00592268: str      r6, [sp, #0xc]
0059226c: str      r3, [sp, #0x14]
00592270: mov      r6, sb
00592274: str      r5, [sp, #0x10]
00592278: b        #0x592284
0059227c: ldr      r3, [sp, #0x10]
00592280: ldrh     r6, [r3, #0xe]
00592284: add      ip, r4, #2
00592288: mul      r5, r4, r6
0059228c: mul      ip, r6, ip
00592290: mla      r6, r4, r6, r6
00592294: add      r0, r8, ip
00592298: add      r2, r8, r6
0059229c: ldr      r6, [r8, r6]
005922a0: ldr      r1, [r7, #4]
005922a4: ldr      sb, [r0, #8]
005922a8: ldr      ip, [r8, ip]
005922ac: str      r6, [sp, #4]
005922b0: ldr      r0, [r0, #4]
005922b4: ldr      sl, [r7, #8]
005922b8: add      r3, r8, r5
005922bc: ldr      r6, [r2, #8]
005922c0: ldr      fp, [r3, #8]
005922c4: ldr      r5, [r8, r5]
005922c8: ldr      r2, [r2, #4]
005922cc: ldr      r3, [r3, #4]
005922d0: str      r0, [sp, #0x40]
005922d4: ldr      r0, [sp, #4]
005922d8: cmp      r1, sl
005922dc: str      sb, [sp, #0x44]
005922e0: str      r0, [sp, #0x48]
005922e4: str      r2, [sp, #0x4c]
005922e8: str      r6, [sp, #0x50]
005922ec: str      r5, [sp, #0x54]
005922f0: str      r3, [sp, #0x58]
005922f4: str      fp, [sp, #0x5c]
005922f8: str      ip, [sp, #0x3c]
005922fc: beq      #0x592488
00592300: str      ip, [r1]
00592304: ldr      r3, [sp, #0x40]
00592308: str      r3, [r1, #4]
0059230c: ldr      r3, [sp, #0x44]
00592310: str      r3, [r1, #8]
00592314: ldr      r3, [sp, #0x48]
00592318: str      r3, [r1, #0xc]
0059231c: ldr      r3, [sp, #0x4c]
00592320: str      r3, [r1, #0x10]
00592324: ldr      r3, [sp, #0x50]
00592328: str      r3, [r1, #0x14]
0059232c: ldr      r3, [sp, #0x54]
00592330: str      r3, [r1, #0x18]
00592334: ldr      r3, [sp, #0x58]
00592338: str      r3, [r1, #0x1c]
0059233c: ldr      r3, [sp, #0x5c]
00592340: str      r3, [r1, #0x20]
00592344: ldr      r3, [r7, #4]
00592348: add      r3, r3, #0x24
0059234c: str      r3, [r7, #4]
00592350: ldr      r2, [sp, #0xc]
00592354: add      r4, r4, #3
00592358: cmp      r2, r4
0059235c: bhi      #0x59227c
00592360: ldr      r5, [sp, #0x10]
00592364: b        #0x591e80
00592368: cmp      r6, #0
0059236c: beq      #0x591e80
00592370: ldrh     sb, [r5, #0xe]
00592374: add      r3, sp, #0x84
00592378: str      r6, [sp, #0xc]
0059237c: str      r3, [sp, #0x14]
00592380: mov      r6, sb
00592384: str      r5, [sp, #0x10]
00592388: b        #0x592394
0059238c: ldr      r3, [sp, #0x10]
00592390: ldrh     r6, [r3, #0xe]
00592394: add      ip, r4, #2
00592398: mul      r5, r4, r6
0059239c: mul      ip, r6, ip
005923a0: mla      r6, r4, r6, r6
005923a4: add      r0, r8, ip
005923a8: add      r2, r8, r6
005923ac: ldr      r6, [r8, r6]
005923b0: ldr      r1, [r7, #4]
005923b4: ldr      sb, [r0, #8]
005923b8: ldr      ip, [r8, ip]
005923bc: str      r6, [sp, #4]
005923c0: ldr      r0, [r0, #4]
005923c4: ldr      sl, [r7, #8]
005923c8: add      r3, r8, r5
005923cc: ldr      r6, [r2, #8]
005923d0: ldr      fp, [r3, #8]
005923d4: ldr      r5, [r8, r5]
005923d8: ldr      r2, [r2, #4]
005923dc: ldr      r3, [r3, #4]
005923e0: str      r0, [sp, #0x88]
005923e4: ldr      r0, [sp, #4]
005923e8: cmp      r1, sl
005923ec: str      sb, [sp, #0x8c]
005923f0: str      r0, [sp, #0x90]
005923f4: str      r2, [sp, #0x94]
005923f8: str      r6, [sp, #0x98]
005923fc: str      r5, [sp, #0x9c]
00592400: str      r3, [sp, #0xa0]
00592404: str      fp, [sp, #0xa4]
00592408: str      ip, [sp, #0x84]
0059240c: beq      #0x592498
00592410: str      ip, [r1]
00592414: ldr      r3, [sp, #0x88]
00592418: str      r3, [r1, #4]
0059241c: ldr      r3, [sp, #0x8c]
00592420: str      r3, [r1, #8]
00592424: ldr      r3, [sp, #0x90]
00592428: str      r3, [r1, #0xc]
0059242c: ldr      r3, [sp, #0x94]
00592430: str      r3, [r1, #0x10]
00592434: ldr      r3, [sp, #0x98]
00592438: str      r3, [r1, #0x14]
0059243c: ldr      r3, [sp, #0x9c]
00592440: str      r3, [r1, #0x18]
00592444: ldr      r3, [sp, #0xa0]
00592448: str      r3, [r1, #0x1c]
0059244c: ldr      r3, [sp, #0xa4]
00592450: str      r3, [r1, #0x20]
00592454: ldr      r3, [r7, #4]
00592458: add      r3, r3, #0x24
0059245c: str      r3, [r7, #4]
00592460: ldr      r2, [sp, #0xc]
00592464: add      r4, r4, #3
00592468: cmp      r2, r4
0059246c: bhi      #0x59238c
00592470: ldr      r5, [sp, #0x10]
00592474: b        #0x591e80
00592478: mov      r0, r7
0059247c: ldr      r2, [sp, #0xc]
00592480: bl       #0x591238
00592484: b        #0x592244
00592488: mov      r0, r7
0059248c: ldr      r2, [sp, #0x14]
00592490: bl       #0x591238
00592494: b        #0x592350
00592498: mov      r0, r7
0059249c: ldr      r2, [sp, #0x14]
005924a0: bl       #0x591238
005924a4: b        #0x592460

# _ZN6glitch5scene12_GLOBAL__N_115createTrianglesIjSt6vectorINS_4core10triangle3dIfEENS4_10SAllocatorIS6_LNS_6memory13E_MEMORY_HINTE0EEEEEEvPKtjRKNS_5video13SVertexStreamERT0_
005924a8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005924ac: mov      r5, r2
005924b0: ldrh     r2, [r2, #0xc]
005924b4: sub      sp, sp, #0xfc
005924b8: mov      r4, r0
005924bc: cmp      r2, #3
005924c0: mov      r8, r1
005924c4: mov      r6, r3
005924c8: beq      #0x592808
005924cc: cmp      r2, #4
005924d0: beq      #0x592670
005924d4: cmp      r2, #2
005924d8: beq      #0x5924e4
005924dc: add      sp, sp, #0xfc
005924e0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005924e4: ldr      r0, [r5]
005924e8: mov      r1, #1
005924ec: bl       #0x5a1adc
005924f0: ldr      r7, [r5, #4]
005924f4: cmp      r4, #0
005924f8: add      r7, r0, r7
005924fc: beq      #0x592b70
00592500: add      r8, r4, r8, lsl #1
00592504: cmp      r4, r8
00592508: str      r8, [sp, #0xc]
0059250c: beq      #0x592640
00592510: add      r2, sp, #0xb0
00592514: mov      r8, #0
00592518: str      r2, [sp, #0x14]
0059251c: str      r5, [sp, #0x10]
00592520: ldr      r2, [sp, #0x10]
00592524: ldrh     sl, [r4, #4]
00592528: ldrh     r3, [r2, #0xe]
0059252c: mul      sl, sl, r3
00592530: ldr      r0, [r7, sl]
00592534: str      r3, [sp, #8]
00592538: bl       #0x30e2e0
0059253c: add      sl, r7, sl
00592540: mov      r5, r0
00592544: ldr      r0, [sl, #4]
00592548: bl       #0x30e2e0
0059254c: ldrh     sl, [r4, #2]
00592550: ldr      r3, [sp, #8]
00592554: mov      fp, r0
00592558: mul      sl, r3, sl
0059255c: ldr      r0, [r7, sl]
00592560: bl       #0x30e2e0
00592564: add      sl, r7, sl
00592568: mov      sb, r0
0059256c: ldr      r0, [sl, #4]
00592570: bl       #0x30e2e0
00592574: ldrh     r2, [r4]
00592578: ldr      r3, [sp, #8]
0059257c: mov      sl, r0
00592580: mul      r3, r3, r2
00592584: ldr      r0, [r7, r3]
00592588: add      r3, r7, r3
0059258c: str      r3, [sp, #8]
00592590: bl       #0x30e2e0
00592594: ldr      r3, [sp, #8]
00592598: mov      r2, r0
0059259c: ldr      r0, [r3, #4]
005925a0: str      r2, [sp, #4]
005925a4: bl       #0x30e2e0
005925a8: ldmib    r6, {r1, r3}
005925ac: ldr      r2, [sp, #4]
005925b0: str      fp, [sp, #0xb4]
005925b4: cmp      r1, r3
005925b8: str      sb, [sp, #0xbc]
005925bc: str      r0, [sp, #0xcc]
005925c0: str      sl, [sp, #0xc0]
005925c4: str      r2, [sp, #0xc8]
005925c8: str      r8, [sp, #0xb8]
005925cc: str      r5, [sp, #0xb0]
005925d0: str      r8, [sp, #0xc4]
005925d4: str      r8, [sp, #0xd0]
005925d8: beq      #0x5929d8
005925dc: str      r5, [r1]
005925e0: ldr      r3, [sp, #0xb4]
005925e4: str      r3, [r1, #4]
005925e8: ldr      r3, [sp, #0xb8]
005925ec: str      r3, [r1, #8]
005925f0: ldr      r3, [sp, #0xbc]
005925f4: str      r3, [r1, #0xc]
005925f8: ldr      r3, [sp, #0xc0]
005925fc: str      r3, [r1, #0x10]
00592600: ldr      r3, [sp, #0xc4]
00592604: str      r3, [r1, #0x14]
00592608: ldr      r3, [sp, #0xc8]
0059260c: str      r3, [r1, #0x18]
00592610: ldr      r3, [sp, #0xcc]
00592614: str      r3, [r1, #0x1c]
00592618: ldr      r3, [sp, #0xd0]
0059261c: str      r3, [r1, #0x20]
00592620: ldr      r3, [r6, #4]
00592624: add      r3, r3, #0x24
00592628: str      r3, [r6, #4]
0059262c: ldr      r3, [sp, #0xc]
00592630: add      r4, r4, #6
00592634: cmp      r3, r4
00592638: bne      #0x592520
0059263c: ldr      r5, [sp, #0x10]
00592640: cmp      r7, #0
00592644: beq      #0x5924dc
00592648: ldr      r4, [r5]
0059264c: ldrb     r3, [r4, #0x13]
00592650: and      r2, r3, #0x1f
00592654: cmp      r2, #1
00592658: bls      #0x59299c
0059265c: sub      r2, r2, #1
00592660: bic      r3, r3, #0x1f
00592664: orr      r3, r2, r3
00592668: strb     r3, [r4, #0x13]
0059266c: b        #0x5924dc
00592670: ldr      r0, [r5]
00592674: mov      r1, #1
00592678: bl       #0x5a1adc
0059267c: ldr      r7, [r5, #4]
00592680: cmp      r4, #0
00592684: add      r7, r0, r7
00592688: beq      #0x5929f8
0059268c: add      r8, r4, r8, lsl #1
00592690: cmp      r4, r8
00592694: str      r8, [sp, #0x14]
00592698: beq      #0x592640
0059269c: add      r3, sp, #0x20
005926a0: str      r3, [sp, #0x1c]
005926a4: str      r5, [sp, #0x18]
005926a8: ldr      r2, [sp, #0x18]
005926ac: ldrh     r8, [r4, #4]
005926b0: ldrh     r3, [r2, #0xe]
005926b4: mul      r8, r8, r3
005926b8: ldr      r0, [r7, r8]
005926bc: str      r3, [sp, #8]
005926c0: bl       #0x30e2e0
005926c4: add      r8, r7, r8
005926c8: mov      r5, r0
005926cc: ldr      r0, [r8, #4]
005926d0: bl       #0x30e2e0
005926d4: mov      fp, r0
005926d8: ldr      r0, [r8, #8]
005926dc: bl       #0x30e2e0
005926e0: ldrh     r2, [r4, #2]
005926e4: ldr      r3, [sp, #8]
005926e8: mov      sb, r0
005926ec: mul      r2, r3, r2
005926f0: ldr      r0, [r7, r2]
005926f4: add      r2, r7, r2
005926f8: str      r2, [sp, #0xc]
005926fc: bl       #0x30e2e0
00592700: ldr      r2, [sp, #0xc]
00592704: mov      sl, r0
00592708: ldr      r0, [r2, #4]
0059270c: bl       #0x30e2e0
00592710: ldr      r2, [sp, #0xc]
00592714: mov      r8, r0
00592718: ldr      r0, [r2, #8]
0059271c: bl       #0x30e2e0
00592720: ldrh     r1, [r4]
00592724: ldr      r3, [sp, #8]
00592728: mov      r2, r0
0059272c: mul      r3, r3, r1
00592730: ldr      r0, [r7, r3]
00592734: add      r3, r7, r3
00592738: str      r2, [sp, #4]
0059273c: str      r3, [sp, #0xc]
00592740: bl       #0x30e2e0
00592744: ldr      r3, [sp, #0xc]
00592748: mov      ip, r0
0059274c: ldr      r0, [r3, #4]
00592750: str      ip, [sp, #8]
00592754: bl       #0x30e2e0
00592758: ldr      r3, [sp, #0xc]
0059275c: str      r0, [sp, #0x10]
00592760: ldr      r0, [r3, #8]
00592764: bl       #0x30e2e0
00592768: ldmib    r6, {r1, r3}
0059276c: str      r0, [sp, #0x40]
00592770: str      fp, [sp, #0x24]
00592774: str      sb, [sp, #0x28]
00592778: ldmib    sp, {r2, ip}
0059277c: cmp      r1, r3
00592780: str      r2, [sp, #0x34]
00592784: ldr      r2, [sp, #0x10]
00592788: str      sl, [sp, #0x2c]
0059278c: str      r8, [sp, #0x30]
00592790: str      ip, [sp, #0x38]
00592794: str      r2, [sp, #0x3c]
00592798: str      r5, [sp, #0x20]
0059279c: beq      #0x5929e8
005927a0: str      r5, [r1]
005927a4: ldr      r3, [sp, #0x24]
005927a8: str      r3, [r1, #4]
005927ac: ldr      r3, [sp, #0x28]
005927b0: str      r3, [r1, #8]
005927b4: ldr      r3, [sp, #0x2c]
005927b8: str      r3, [r1, #0xc]
005927bc: ldr      r3, [sp, #0x30]
005927c0: str      r3, [r1, #0x10]
005927c4: ldr      r3, [sp, #0x34]
005927c8: str      r3, [r1, #0x14]
005927cc: ldr      r3, [sp, #0x38]
005927d0: str      r3, [r1, #0x18]
005927d4: ldr      r3, [sp, #0x3c]
005927d8: str      r3, [r1, #0x1c]
005927dc: ldr      r3, [sp, #0x40]
005927e0: str      r3, [r1, #0x20]
005927e4: ldr      r3, [r6, #4]
005927e8: add      r3, r3, #0x24
005927ec: str      r3, [r6, #4]
005927f0: ldr      r3, [sp, #0x14]
005927f4: add      r4, r4, #6
005927f8: cmp      r3, r4
005927fc: bne      #0x5926a8
00592800: ldr      r5, [sp, #0x18]
00592804: b        #0x592640
00592808: ldr      r0, [r5]
0059280c: mov      r1, #1
00592810: bl       #0x5a1adc
00592814: ldr      r7, [r5, #4]
00592818: cmp      r4, #0
0059281c: add      r7, r0, r7
00592820: beq      #0x592cb0
00592824: add      r8, r4, r8, lsl #1
00592828: cmp      r4, r8
0059282c: str      r8, [sp, #0x14]
00592830: beq      #0x592640
00592834: add      r3, sp, #0x68
00592838: str      r3, [sp, #0x1c]
0059283c: str      r5, [sp, #0x18]
00592840: ldr      r2, [sp, #0x18]
00592844: ldrh     r8, [r4, #4]
00592848: ldrh     r3, [r2, #0xe]
0059284c: mul      r8, r8, r3
00592850: ldr      r0, [r7, r8]
00592854: str      r3, [sp, #8]
00592858: bl       #0x30e2e0
0059285c: add      r8, r7, r8
00592860: mov      r5, r0
00592864: ldr      r0, [r8, #4]
00592868: bl       #0x30e2e0
0059286c: mov      fp, r0
00592870: ldr      r0, [r8, #8]
00592874: bl       #0x30e2e0
00592878: ldrh     r2, [r4, #2]
0059287c: ldr      r3, [sp, #8]
00592880: mov      sb, r0
00592884: mul      r2, r3, r2
00592888: ldr      r0, [r7, r2]
0059288c: add      r2, r7, r2
00592890: str      r2, [sp, #0xc]
00592894: bl       #0x30e2e0
00592898: ldr      r2, [sp, #0xc]
0059289c: mov      sl, r0
005928a0: ldr      r0, [r2, #4]
005928a4: bl       #0x30e2e0
005928a8: ldr      r2, [sp, #0xc]
005928ac: mov      r8, r0
005928b0: ldr      r0, [r2, #8]
005928b4: bl       #0x30e2e0
005928b8: ldrh     r1, [r4]
005928bc: ldr      r3, [sp, #8]
005928c0: mov      r2, r0
005928c4: mul      r3, r3, r1
005928c8: ldr      r0, [r7, r3]
005928cc: add      r3, r7, r3
005928d0: str      r2, [sp, #4]
005928d4: str      r3, [sp, #0xc]
005928d8: bl       #0x30e2e0
005928dc: ldr      r3, [sp, #0xc]
005928e0: mov      ip, r0
005928e4: ldr      r0, [r3, #4]
005928e8: str      ip, [sp, #8]
005928ec: bl       #0x30e2e0
005928f0: ldr      r3, [sp, #0xc]
005928f4: str      r0, [sp, #0x10]
005928f8: ldr      r0, [r3, #8]
005928fc: bl       #0x30e2e0
00592900: ldmib    r6, {r1, r3}
00592904: str      r0, [sp, #0x88]
00592908: str      fp, [sp, #0x6c]
0059290c: str      sb, [sp, #0x70]
00592910: ldmib    sp, {r2, ip}
00592914: cmp      r1, r3
00592918: str      r2, [sp, #0x7c]
0059291c: ldr      r2, [sp, #0x10]
00592920: str      sl, [sp, #0x74]
00592924: str      r8, [sp, #0x78]
00592928: str      ip, [sp, #0x80]
0059292c: str      r2, [sp, #0x84]
00592930: str      r5, [sp, #0x68]
00592934: beq      #0x5929c8
00592938: str      r5, [r1]
0059293c: ldr      r3, [sp, #0x6c]
00592940: str      r3, [r1, #4]
00592944: ldr      r3, [sp, #0x70]
00592948: str      r3, [r1, #8]
0059294c: ldr      r3, [sp, #0x74]
00592950: str      r3, [r1, #0xc]
00592954: ldr      r3, [sp, #0x78]
00592958: str      r3, [r1, #0x10]
0059295c: ldr      r3, [sp, #0x7c]
00592960: str      r3, [r1, #0x14]
00592964: ldr      r3, [sp, #0x80]
00592968: str      r3, [r1, #0x18]
0059296c: ldr      r3, [sp, #0x84]
00592970: str      r3, [r1, #0x1c]
00592974: ldr      r3, [sp, #0x88]
00592978: str      r3, [r1, #0x20]
0059297c: ldr      r3, [r6, #4]
00592980: add      r3, r3, #0x24
00592984: str      r3, [r6, #4]
00592988: ldr      r3, [sp, #0x14]
0059298c: add      r4, r4, #6
00592990: cmp      r3, r4
00592994: bne      #0x592840
00592998: b        #0x592800
0059299c: ldrb     r3, [r4, #0x12]
005929a0: tst      r3, #0x20
005929a4: bne      #0x5929b4
005929a8: mov      r3, #0
005929ac: strb     r3, [r4, #0x13]
005929b0: b        #0x5924dc
005929b4: ldr      r3, [r4]
005929b8: mov      r0, r4
005929bc: mov      lr, pc
005929c0: ldr      pc, [r3, #0x18]
005929c4: b        #0x5929a8
005929c8: mov      r0, r6
005929cc: ldr      r2, [sp, #0x1c]
005929d0: bl       #0x591238
005929d4: b        #0x592988
005929d8: mov      r0, r6
005929dc: ldr      r2, [sp, #0x14]
005929e0: bl       #0x591238
005929e4: b        #0x59262c
005929e8: mov      r0, r6
005929ec: ldr      r2, [sp, #0x1c]
005929f0: bl       #0x591238
005929f4: b        #0x5927f0
005929f8: cmp      r8, #0
005929fc: beq      #0x592640
00592a00: add      r2, sp, #0x44
00592a04: ldrh     r3, [r5, #0xe]
00592a08: str      r2, [sp, #0x1c]
00592a0c: str      r8, [sp, #0x14]
00592a10: str      r5, [sp, #0x18]
00592a14: b        #0x592a20
00592a18: ldr      r2, [sp, #0x18]
00592a1c: ldrh     r3, [r2, #0xe]
00592a20: add      r8, r4, #2
00592a24: mul      r8, r3, r8
00592a28: ldr      r0, [r7, r8]
00592a2c: str      r3, [sp, #8]
00592a30: bl       #0x30e2e0
00592a34: add      r8, r7, r8
00592a38: mov      r5, r0
00592a3c: ldr      r0, [r8, #4]
00592a40: bl       #0x30e2e0
00592a44: mov      fp, r0
00592a48: ldr      r0, [r8, #8]
00592a4c: bl       #0x30e2e0
00592a50: ldr      r3, [sp, #8]
00592a54: mov      sb, r0
00592a58: mla      r2, r4, r3, r3
00592a5c: ldr      r0, [r7, r2]
00592a60: add      r2, r7, r2
00592a64: str      r2, [sp, #0xc]
00592a68: bl       #0x30e2e0
00592a6c: ldr      r2, [sp, #0xc]
00592a70: mov      sl, r0
00592a74: ldr      r0, [r2, #4]
00592a78: bl       #0x30e2e0
00592a7c: ldr      r2, [sp, #0xc]
00592a80: mov      r8, r0
00592a84: ldr      r0, [r2, #8]
00592a88: bl       #0x30e2e0
00592a8c: ldr      r3, [sp, #8]
00592a90: mov      r2, r0
00592a94: mul      r3, r4, r3
00592a98: ldr      r0, [r7, r3]
00592a9c: add      r3, r7, r3
00592aa0: str      r2, [sp, #4]
00592aa4: str      r3, [sp, #0xc]
00592aa8: bl       #0x30e2e0
00592aac: ldr      r3, [sp, #0xc]
00592ab0: mov      ip, r0
00592ab4: ldr      r0, [r3, #4]
00592ab8: str      ip, [sp, #8]
00592abc: bl       #0x30e2e0
00592ac0: ldr      r3, [sp, #0xc]
00592ac4: str      r0, [sp, #0x10]
00592ac8: ldr      r0, [r3, #8]
00592acc: bl       #0x30e2e0
00592ad0: ldmib    sp, {r2, ip}
00592ad4: ldmib    r6, {r1, r3}
00592ad8: str      r2, [sp, #0x58]
00592adc: str      fp, [sp, #0x48]
00592ae0: str      sb, [sp, #0x4c]
00592ae4: str      r0, [sp, #0x64]
00592ae8: str      sl, [sp, #0x50]
00592aec: str      r8, [sp, #0x54]
00592af0: str      ip, [sp, #0x5c]
00592af4: ldr      r2, [sp, #0x10]
00592af8: cmp      r1, r3
00592afc: str      r5, [sp, #0x44]
00592b00: str      r2, [sp, #0x60]
00592b04: beq      #0x592e28
00592b08: str      r5, [r1]
00592b0c: ldr      r3, [sp, #0x48]
00592b10: str      r3, [r1, #4]
00592b14: ldr      r3, [sp, #0x4c]
00592b18: str      r3, [r1, #8]
00592b1c: ldr      r3, [sp, #0x50]
00592b20: str      r3, [r1, #0xc]
00592b24: ldr      r3, [sp, #0x54]
00592b28: str      r3, [r1, #0x10]
00592b2c: ldr      r3, [sp, #0x58]
00592b30: str      r3, [r1, #0x14]
00592b34: ldr      r3, [sp, #0x5c]
00592b38: str      r3, [r1, #0x18]
00592b3c: ldr      r3, [sp, #0x60]
00592b40: str      r3, [r1, #0x1c]
00592b44: ldr      r3, [sp, #0x64]
00592b48: str      r3, [r1, #0x20]
00592b4c: ldr      r3, [r6, #4]
00592b50: add      r3, r3, #0x24
00592b54: str      r3, [r6, #4]
00592b58: ldr      r3, [sp, #0x14]
00592b5c: add      r4, r4, #3
00592b60: cmp      r3, r4
00592b64: bhi      #0x592a18
00592b68: ldr      r5, [sp, #0x18]
00592b6c: b        #0x592640
00592b70: cmp      r8, #0
00592b74: beq      #0x592640
00592b78: add      r2, sp, #0xd4
00592b7c: ldrh     r3, [r5, #0xe]
00592b80: mov      sl, #0
00592b84: str      r2, [sp, #0x14]
00592b88: str      r8, [sp, #0xc]
00592b8c: str      r5, [sp, #0x10]
00592b90: b        #0x592b9c
00592b94: ldr      r2, [sp, #0x10]
00592b98: ldrh     r3, [r2, #0xe]
00592b9c: add      r8, r4, #2
00592ba0: mul      r8, r3, r8
00592ba4: ldr      r0, [r7, r8]
00592ba8: str      r3, [sp, #8]
00592bac: bl       #0x30e2e0
00592bb0: add      r8, r7, r8
00592bb4: mov      r5, r0
00592bb8: ldr      r0, [r8, #4]
00592bbc: bl       #0x30e2e0
00592bc0: ldr      r3, [sp, #8]
00592bc4: mov      fp, r0
00592bc8: mla      r8, r4, r3, r3
00592bcc: ldr      r0, [r7, r8]
00592bd0: bl       #0x30e2e0
00592bd4: add      r8, r7, r8
00592bd8: mov      sb, r0
00592bdc: ldr      r0, [r8, #4]
00592be0: bl       #0x30e2e0
00592be4: ldr      r3, [sp, #8]
00592be8: mov      r8, r0
00592bec: mul      r3, r4, r3
00592bf0: ldr      r0, [r7, r3]
00592bf4: add      r3, r7, r3
00592bf8: str      r3, [sp, #8]
00592bfc: bl       #0x30e2e0
00592c00: ldr      r3, [sp, #8]
00592c04: mov      r2, r0
00592c08: ldr      r0, [r3, #4]
00592c0c: str      r2, [sp, #4]
00592c10: bl       #0x30e2e0
00592c14: ldmib    r6, {r1, r3}
00592c18: ldr      r2, [sp, #4]
00592c1c: str      fp, [sp, #0xd8]
00592c20: cmp      r1, r3
00592c24: str      sb, [sp, #0xe0]
00592c28: str      r8, [sp, #0xe4]
00592c2c: str      r0, [sp, #0xf0]
00592c30: str      r2, [sp, #0xec]
00592c34: str      sl, [sp, #0xdc]
00592c38: str      sl, [sp, #0xe8]
00592c3c: str      r5, [sp, #0xd4]
00592c40: str      sl, [sp, #0xf4]
00592c44: beq      #0x592e38
00592c48: str      r5, [r1]
00592c4c: ldr      r3, [sp, #0xd8]
00592c50: str      r3, [r1, #4]
00592c54: ldr      r3, [sp, #0xdc]
00592c58: str      r3, [r1, #8]
00592c5c: ldr      r3, [sp, #0xe0]
00592c60: str      r3, [r1, #0xc]
00592c64: ldr      r3, [sp, #0xe4]
00592c68: str      r3, [r1, #0x10]
00592c6c: ldr      r3, [sp, #0xe8]
00592c70: str      r3, [r1, #0x14]
00592c74: ldr      r3, [sp, #0xec]
00592c78: str      r3, [r1, #0x18]
00592c7c: ldr      r3, [sp, #0xf0]
00592c80: str      r3, [r1, #0x1c]
00592c84: ldr      r3, [sp, #0xf4]
00592c88: str      r3, [r1, #0x20]
00592c8c: ldr      r3, [r6, #4]
00592c90: add      r3, r3, #0x24
00592c94: str      r3, [r6, #4]
00592c98: ldr      r3, [sp, #0xc]
00592c9c: add      r4, r4, #3
00592ca0: cmp      r3, r4
00592ca4: bhi      #0x592b94
00592ca8: ldr      r5, [sp, #0x10]
00592cac: b        #0x592640
00592cb0: cmp      r8, #0
00592cb4: beq      #0x592640
00592cb8: add      r2, sp, #0x8c
00592cbc: ldrh     r3, [r5, #0xe]
00592cc0: str      r2, [sp, #0x1c]
00592cc4: str      r8, [sp, #0x14]
00592cc8: str      r5, [sp, #0x18]
00592ccc: b        #0x592cd8
00592cd0: ldr      r2, [sp, #0x18]
00592cd4: ldrh     r3, [r2, #0xe]
00592cd8: add      r8, r4, #2
00592cdc: mul      r8, r3, r8
00592ce0: ldr      r0, [r7, r8]
00592ce4: str      r3, [sp, #8]
00592ce8: bl       #0x30e2e0
00592cec: add      r8, r7, r8
00592cf0: mov      r5, r0
00592cf4: ldr      r0, [r8, #4]
00592cf8: bl       #0x30e2e0
00592cfc: mov      fp, r0
00592d00: ldr      r0, [r8, #8]
00592d04: bl       #0x30e2e0
00592d08: ldr      r3, [sp, #8]
00592d0c: mov      sb, r0
00592d10: mla      r2, r4, r3, r3
00592d14: ldr      r0, [r7, r2]
00592d18: add      r2, r7, r2
00592d1c: str      r2, [sp, #0xc]
00592d20: bl       #0x30e2e0
00592d24: ldr      r2, [sp, #0xc]
00592d28: mov      sl, r0
00592d2c: ldr      r0, [r2, #4]
00592d30: bl       #0x30e2e0
00592d34: ldr      r2, [sp, #0xc]
00592d38: mov      r8, r0
00592d3c: ldr      r0, [r2, #8]
00592d40: bl       #0x30e2e0
00592d44: ldr      r3, [sp, #8]
00592d48: mov      r2, r0
00592d4c: mul      r3, r4, r3
00592d50: ldr      r0, [r7, r3]
00592d54: add      r3, r7, r3
00592d58: str      r2, [sp, #4]
00592d5c: str      r3, [sp, #0xc]
00592d60: bl       #0x30e2e0
00592d64: ldr      r3, [sp, #0xc]
00592d68: mov      ip, r0
00592d6c: ldr      r0, [r3, #4]
00592d70: str      ip, [sp, #8]
00592d74: bl       #0x30e2e0
00592d78: ldr      r3, [sp, #0xc]
00592d7c: str      r0, [sp, #0x10]
00592d80: ldr      r0, [r3, #8]
00592d84: bl       #0x30e2e0
00592d88: ldmib    sp, {r2, ip}
00592d8c: ldmib    r6, {r1, r3}
00592d90: str      r2, [sp, #0xa0]
00592d94: str      fp, [sp, #0x90]
00592d98: str      sb, [sp, #0x94]
00592d9c: str      r0, [sp, #0xac]
00592da0: str      sl, [sp, #0x98]
00592da4: str      r8, [sp, #0x9c]
00592da8: str      ip, [sp, #0xa4]
00592dac: ldr      r2, [sp, #0x10]
00592db0: cmp      r1, r3
00592db4: str      r5, [sp, #0x8c]
00592db8: str      r2, [sp, #0xa8]
00592dbc: beq      #0x592e48
00592dc0: str      r5, [r1]
00592dc4: ldr      r3, [sp, #0x90]
00592dc8: str      r3, [r1, #4]
00592dcc: ldr      r3, [sp, #0x94]
00592dd0: str      r3, [r1, #8]
00592dd4: ldr      r3, [sp, #0x98]
00592dd8: str      r3, [r1, #0xc]
00592ddc: ldr      r3, [sp, #0x9c]
00592de0: str      r3, [r1, #0x10]
00592de4: ldr      r3, [sp, #0xa0]
00592de8: str      r3, [r1, #0x14]
00592dec: ldr      r3, [sp, #0xa4]
00592df0: str      r3, [r1, #0x18]
00592df4: ldr      r3, [sp, #0xa8]
00592df8: str      r3, [r1, #0x1c]
00592dfc: ldr      r3, [sp, #0xac]
00592e00: str      r3, [r1, #0x20]
00592e04: ldr      r3, [r6, #4]
00592e08: add      r3, r3, #0x24
00592e0c: str      r3, [r6, #4]
00592e10: ldr      r3, [sp, #0x14]
00592e14: add      r4, r4, #3
00592e18: cmp      r3, r4
00592e1c: bhi      #0x592cd0
00592e20: ldr      r5, [sp, #0x18]
00592e24: b        #0x592640
00592e28: mov      r0, r6
00592e2c: ldr      r2, [sp, #0x1c]
00592e30: bl       #0x591238
00592e34: b        #0x592b58
00592e38: mov      r0, r6
00592e3c: ldr      r2, [sp, #0x14]
00592e40: bl       #0x591238
00592e44: b        #0x592c98
00592e48: mov      r0, r6
00592e4c: ldr      r2, [sp, #0x1c]
00592e50: bl       #0x591238
00592e54: b        #0x592e10

# _ZN6glitch5scene12_GLOBAL__N_115createTrianglesIaSt6vectorINS_4core10triangle3dIfEENS4_10SAllocatorIS6_LNS_6memory13E_MEMORY_HINTE0EEEEEEvPKtjRKNS_5video13SVertexStreamERT0_
00592e58: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00592e5c: mov      r5, r2
00592e60: ldrh     r2, [r2, #0xc]
00592e64: sub      sp, sp, #0xfc
00592e68: mov      r4, r0
00592e6c: cmp      r2, #3
00592e70: mov      r8, r1
00592e74: mov      r6, r3
00592e78: beq      #0x5931b8
00592e7c: cmp      r2, #4
00592e80: beq      #0x593020
00592e84: cmp      r2, #2
00592e88: beq      #0x592e94
00592e8c: add      sp, sp, #0xfc
00592e90: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00592e94: ldr      r0, [r5]
00592e98: mov      r1, #1
00592e9c: bl       #0x5a1adc
00592ea0: ldr      r7, [r5, #4]
00592ea4: cmp      r4, #0
00592ea8: add      r7, r0, r7
00592eac: beq      #0x593520
00592eb0: add      r8, r4, r8, lsl #1
00592eb4: cmp      r4, r8
00592eb8: str      r8, [sp, #0xc]
00592ebc: beq      #0x592ff0
00592ec0: add      r2, sp, #0xb0
00592ec4: mov      r8, #0
00592ec8: str      r2, [sp, #0x14]
00592ecc: str      r5, [sp, #0x10]
00592ed0: ldr      r2, [sp, #0x10]
00592ed4: ldrh     sl, [r4, #4]
00592ed8: ldrh     r3, [r2, #0xe]
00592edc: mul      sl, sl, r3
00592ee0: ldrsb    r0, [r7, sl]
00592ee4: str      r3, [sp, #8]
00592ee8: bl       #0x30e964
00592eec: add      sl, r7, sl
00592ef0: mov      r5, r0
00592ef4: ldrsb    r0, [sl, #1]
00592ef8: bl       #0x30e964
00592efc: ldrh     sl, [r4, #2]
00592f00: ldr      r3, [sp, #8]
00592f04: mov      fp, r0
00592f08: mul      sl, r3, sl
00592f0c: ldrsb    r0, [r7, sl]
00592f10: bl       #0x30e964
00592f14: add      sl, r7, sl
00592f18: mov      sb, r0
00592f1c: ldrsb    r0, [sl, #1]
00592f20: bl       #0x30e964
00592f24: ldrh     r2, [r4]
00592f28: ldr      r3, [sp, #8]
00592f2c: mov      sl, r0
00592f30: mul      r3, r3, r2
00592f34: ldrsb    r0, [r7, r3]
00592f38: add      r3, r7, r3
00592f3c: str      r3, [sp, #8]
00592f40: bl       #0x30e964
00592f44: ldr      r3, [sp, #8]
00592f48: mov      r2, r0
00592f4c: ldrsb    r0, [r3, #1]
00592f50: str      r2, [sp, #4]
00592f54: bl       #0x30e964
00592f58: ldmib    r6, {r1, r3}
00592f5c: ldr      r2, [sp, #4]
00592f60: str      fp, [sp, #0xb4]
00592f64: cmp      r1, r3
00592f68: str      sb, [sp, #0xbc]
00592f6c: str      r0, [sp, #0xcc]
00592f70: str      sl, [sp, #0xc0]
00592f74: str      r2, [sp, #0xc8]
00592f78: str      r8, [sp, #0xb8]
00592f7c: str      r5, [sp, #0xb0]
00592f80: str      r8, [sp, #0xc4]
00592f84: str      r8, [sp, #0xd0]
00592f88: beq      #0x593388
00592f8c: str      r5, [r1]
00592f90: ldr      r3, [sp, #0xb4]
00592f94: str      r3, [r1, #4]
00592f98: ldr      r3, [sp, #0xb8]
00592f9c: str      r3, [r1, #8]
00592fa0: ldr      r3, [sp, #0xbc]
00592fa4: str      r3, [r1, #0xc]
00592fa8: ldr      r3, [sp, #0xc0]
00592fac: str      r3, [r1, #0x10]
00592fb0: ldr      r3, [sp, #0xc4]
00592fb4: str      r3, [r1, #0x14]
00592fb8: ldr      r3, [sp, #0xc8]
00592fbc: str      r3, [r1, #0x18]
00592fc0: ldr      r3, [sp, #0xcc]
00592fc4: str      r3, [r1, #0x1c]
00592fc8: ldr      r3, [sp, #0xd0]
00592fcc: str      r3, [r1, #0x20]
00592fd0: ldr      r3, [r6, #4]
00592fd4: add      r3, r3, #0x24
00592fd8: str      r3, [r6, #4]
00592fdc: ldr      r3, [sp, #0xc]
00592fe0: add      r4, r4, #6
00592fe4: cmp      r3, r4
00592fe8: bne      #0x592ed0
00592fec: ldr      r5, [sp, #0x10]
00592ff0: cmp      r7, #0
00592ff4: beq      #0x592e8c
00592ff8: ldr      r4, [r5]
00592ffc: ldrb     r3, [r4, #0x13]
00593000: and      r2, r3, #0x1f
00593004: cmp      r2, #1
00593008: bls      #0x59334c
0059300c: sub      r2, r2, #1
00593010: bic      r3, r3, #0x1f
00593014: orr      r3, r2, r3
00593018: strb     r3, [r4, #0x13]
0059301c: b        #0x592e8c
00593020: ldr      r0, [r5]
00593024: mov      r1, #1
00593028: bl       #0x5a1adc
0059302c: ldr      r7, [r5, #4]
00593030: cmp      r4, #0
00593034: add      r7, r0, r7
00593038: beq      #0x5933a8
0059303c: add      r8, r4, r8, lsl #1
00593040: cmp      r4, r8
00593044: str      r8, [sp, #0x14]
00593048: beq      #0x592ff0
0059304c: add      r3, sp, #0x20
00593050: str      r3, [sp, #0x1c]
00593054: str      r5, [sp, #0x18]
00593058: ldr      r2, [sp, #0x18]
0059305c: ldrh     r8, [r4, #4]
00593060: ldrh     r3, [r2, #0xe]
00593064: mul      r8, r8, r3
00593068: ldrsb    r0, [r7, r8]
0059306c: str      r3, [sp, #8]
00593070: bl       #0x30e964
00593074: add      r8, r7, r8
00593078: mov      r5, r0
0059307c: ldrsb    r0, [r8, #1]
00593080: bl       #0x30e964
00593084: mov      fp, r0
00593088: ldrsb    r0, [r8, #2]
0059308c: bl       #0x30e964
00593090: ldrh     r2, [r4, #2]
00593094: ldr      r3, [sp, #8]
00593098: mov      sb, r0
0059309c: mul      r2, r3, r2
005930a0: ldrsb    r0, [r7, r2]
005930a4: add      r2, r7, r2
005930a8: str      r2, [sp, #0xc]
005930ac: bl       #0x30e964
005930b0: ldr      r2, [sp, #0xc]
005930b4: mov      sl, r0
005930b8: ldrsb    r0, [r2, #1]
005930bc: bl       #0x30e964
005930c0: ldr      r2, [sp, #0xc]
005930c4: mov      r8, r0
005930c8: ldrsb    r0, [r2, #2]
005930cc: bl       #0x30e964
005930d0: ldrh     r1, [r4]
005930d4: ldr      r3, [sp, #8]
005930d8: mov      r2, r0
005930dc: mul      r3, r3, r1
005930e0: ldrsb    r0, [r7, r3]
005930e4: add      r3, r7, r3
005930e8: str      r2, [sp, #4]
005930ec: str      r3, [sp, #0xc]
005930f0: bl       #0x30e964
005930f4: ldr      r3, [sp, #0xc]
005930f8: mov      ip, r0
005930fc: ldrsb    r0, [r3, #1]
00593100: str      ip, [sp, #8]
00593104: bl       #0x30e964
00593108: ldr      r3, [sp, #0xc]
0059310c: str      r0, [sp, #0x10]
00593110: ldrsb    r0, [r3, #2]
00593114: bl       #0x30e964
00593118: ldmib    r6, {r1, r3}
0059311c: str      r0, [sp, #0x40]
00593120: str      fp, [sp, #0x24]
00593124: str      sb, [sp, #0x28]
00593128: ldmib    sp, {r2, ip}
0059312c: cmp      r1, r3
00593130: str      r2, [sp, #0x34]
00593134: ldr      r2, [sp, #0x10]
00593138: str      sl, [sp, #0x2c]
0059313c: str      r8, [sp, #0x30]
00593140: str      ip, [sp, #0x38]
00593144: str      r2, [sp, #0x3c]
00593148: str      r5, [sp, #0x20]
0059314c: beq      #0x593398
00593150: str      r5, [r1]
00593154: ldr      r3, [sp, #0x24]
00593158: str      r3, [r1, #4]
0059315c: ldr      r3, [sp, #0x28]
00593160: str      r3, [r1, #8]
00593164: ldr      r3, [sp, #0x2c]
00593168: str      r3, [r1, #0xc]
0059316c: ldr      r3, [sp, #0x30]
00593170: str      r3, [r1, #0x10]
00593174: ldr      r3, [sp, #0x34]
00593178: str      r3, [r1, #0x14]
0059317c: ldr      r3, [sp, #0x38]
00593180: str      r3, [r1, #0x18]
00593184: ldr      r3, [sp, #0x3c]
00593188: str      r3, [r1, #0x1c]
0059318c: ldr      r3, [sp, #0x40]
00593190: str      r3, [r1, #0x20]
00593194: ldr      r3, [r6, #4]
00593198: add      r3, r3, #0x24
0059319c: str      r3, [r6, #4]
005931a0: ldr      r3, [sp, #0x14]
005931a4: add      r4, r4, #6
005931a8: cmp      r3, r4
005931ac: bne      #0x593058
005931b0: ldr      r5, [sp, #0x18]
005931b4: b        #0x592ff0
005931b8: ldr      r0, [r5]
005931bc: mov      r1, #1
005931c0: bl       #0x5a1adc
005931c4: ldr      r7, [r5, #4]
005931c8: cmp      r4, #0
005931cc: add      r7, r0, r7
005931d0: beq      #0x593660
005931d4: add      r8, r4, r8, lsl #1
005931d8: cmp      r4, r8
005931dc: str      r8, [sp, #0x14]
005931e0: beq      #0x592ff0
005931e4: add      r3, sp, #0x68
005931e8: str      r3, [sp, #0x1c]
005931ec: str      r5, [sp, #0x18]
005931f0: ldr      r2, [sp, #0x18]
005931f4: ldrh     r8, [r4, #4]
005931f8: ldrh     r3, [r2, #0xe]
005931fc: mul      r8, r8, r3
00593200: ldrsb    r0, [r7, r8]
00593204: str      r3, [sp, #8]
00593208: bl       #0x30e964
0059320c: add      r8, r7, r8
00593210: mov      r5, r0
00593214: ldrsb    r0, [r8, #1]
00593218: bl       #0x30e964
0059321c: mov      fp, r0
00593220: ldrsb    r0, [r8, #2]
00593224: bl       #0x30e964
00593228: ldrh     r2, [r4, #2]
0059322c: ldr      r3, [sp, #8]
00593230: mov      sb, r0
00593234: mul      r2, r3, r2
00593238: ldrsb    r0, [r7, r2]
0059323c: add      r2, r7, r2
00593240: str      r2, [sp, #0xc]
00593244: bl       #0x30e964
00593248: ldr      r2, [sp, #0xc]
0059324c: mov      sl, r0
00593250: ldrsb    r0, [r2, #1]
00593254: bl       #0x30e964
00593258: ldr      r2, [sp, #0xc]
0059325c: mov      r8, r0
00593260: ldrsb    r0, [r2, #2]
00593264: bl       #0x30e964
00593268: ldrh     r1, [r4]
0059326c: ldr      r3, [sp, #8]
00593270: mov      r2, r0
00593274: mul      r3, r3, r1
00593278: ldrsb    r0, [r7, r3]
0059327c: add      r3, r7, r3
00593280: str      r2, [sp, #4]
00593284: str      r3, [sp, #0xc]
00593288: bl       #0x30e964
0059328c: ldr      r3, [sp, #0xc]
00593290: mov      ip, r0
00593294: ldrsb    r0, [r3, #1]
00593298: str      ip, [sp, #8]
0059329c: bl       #0x30e964
005932a0: ldr      r3, [sp, #0xc]
005932a4: str      r0, [sp, #0x10]
005932a8: ldrsb    r0, [r3, #2]
005932ac: bl       #0x30e964
005932b0: ldmib    r6, {r1, r3}
005932b4: str      r0, [sp, #0x88]
005932b8: str      fp, [sp, #0x6c]
005932bc: str      sb, [sp, #0x70]
005932c0: ldmib    sp, {r2, ip}
005932c4: cmp      r1, r3
005932c8: str      r2, [sp, #0x7c]
005932cc: ldr      r2, [sp, #0x10]
005932d0: str      sl, [sp, #0x74]
005932d4: str      r8, [sp, #0x78]
005932d8: str      ip, [sp, #0x80]
005932dc: str      r2, [sp, #0x84]
005932e0: str      r5, [sp, #0x68]
005932e4: beq      #0x593378
005932e8: str      r5, [r1]
005932ec: ldr      r3, [sp, #0x6c]
005932f0: str      r3, [r1, #4]
005932f4: ldr      r3, [sp, #0x70]
005932f8: str      r3, [r1, #8]
005932fc: ldr      r3, [sp, #0x74]
00593300: str      r3, [r1, #0xc]
00593304: ldr      r3, [sp, #0x78]
00593308: str      r3, [r1, #0x10]
0059330c: ldr      r3, [sp, #0x7c]
00593310: str      r3, [r1, #0x14]
00593314: ldr      r3, [sp, #0x80]
00593318: str      r3, [r1, #0x18]
0059331c: ldr      r3, [sp, #0x84]
00593320: str      r3, [r1, #0x1c]
00593324: ldr      r3, [sp, #0x88]
00593328: str      r3, [r1, #0x20]
0059332c: ldr      r3, [r6, #4]
00593330: add      r3, r3, #0x24
00593334: str      r3, [r6, #4]
00593338: ldr      r3, [sp, #0x14]
0059333c: add      r4, r4, #6
00593340: cmp      r3, r4
00593344: bne      #0x5931f0
00593348: b        #0x5931b0
0059334c: ldrb     r3, [r4, #0x12]
00593350: tst      r3, #0x20
00593354: bne      #0x593364
00593358: mov      r3, #0
0059335c: strb     r3, [r4, #0x13]
00593360: b        #0x592e8c
00593364: ldr      r3, [r4]
00593368: mov      r0, r4
0059336c: mov      lr, pc
00593370: ldr      pc, [r3, #0x18]
00593374: b        #0x593358
00593378: mov      r0, r6
0059337c: ldr      r2, [sp, #0x1c]
00593380: bl       #0x591238
00593384: b        #0x593338
00593388: mov      r0, r6
0059338c: ldr      r2, [sp, #0x14]
00593390: bl       #0x591238
00593394: b        #0x592fdc
00593398: mov      r0, r6
0059339c: ldr      r2, [sp, #0x1c]
005933a0: bl       #0x591238
005933a4: b        #0x5931a0
005933a8: cmp      r8, #0
005933ac: beq      #0x592ff0
005933b0: add      r2, sp, #0x44
005933b4: ldrh     r3, [r5, #0xe]
005933b8: str      r2, [sp, #0x1c]
005933bc: str      r8, [sp, #0x14]
005933c0: str      r5, [sp, #0x18]
005933c4: b        #0x5933d0
005933c8: ldr      r2, [sp, #0x18]
005933cc: ldrh     r3, [r2, #0xe]
005933d0: add      r8, r4, #2
005933d4: mul      r8, r3, r8
005933d8: ldrsb    r0, [r7, r8]
005933dc: str      r3, [sp, #8]
005933e0: bl       #0x30e964
005933e4: add      r8, r7, r8
005933e8: mov      r5, r0
005933ec: ldrsb    r0, [r8, #1]
005933f0: bl       #0x30e964
005933f4: mov      fp, r0
005933f8: ldrsb    r0, [r8, #2]
005933fc: bl       #0x30e964
00593400: ldr      r3, [sp, #8]
00593404: mov      sb, r0
00593408: mla      r2, r4, r3, r3
0059340c: ldrsb    r0, [r7, r2]
00593410: add      r2, r7, r2
00593414: str      r2, [sp, #0xc]
00593418: bl       #0x30e964
0059341c: ldr      r2, [sp, #0xc]
00593420: mov      sl, r0
00593424: ldrsb    r0, [r2, #1]
00593428: bl       #0x30e964
0059342c: ldr      r2, [sp, #0xc]
00593430: mov      r8, r0
00593434: ldrsb    r0, [r2, #2]
00593438: bl       #0x30e964
0059343c: ldr      r3, [sp, #8]
00593440: mov      r2, r0
00593444: mul      r3, r4, r3
00593448: ldrsb    r0, [r7, r3]
0059344c: add      r3, r7, r3
00593450: str      r2, [sp, #4]
00593454: str      r3, [sp, #0xc]
00593458: bl       #0x30e964
0059345c: ldr      r3, [sp, #0xc]
00593460: mov      ip, r0
00593464: ldrsb    r0, [r3, #1]
00593468: str      ip, [sp, #8]
0059346c: bl       #0x30e964
00593470: ldr      r3, [sp, #0xc]
00593474: str      r0, [sp, #0x10]
00593478: ldrsb    r0, [r3, #2]
0059347c: bl       #0x30e964
00593480: ldmib    sp, {r2, ip}
00593484: ldmib    r6, {r1, r3}
00593488: str      r2, [sp, #0x58]
0059348c: str      fp, [sp, #0x48]
00593490: str      sb, [sp, #0x4c]
00593494: str      r0, [sp, #0x64]
00593498: str      sl, [sp, #0x50]
0059349c: str      r8, [sp, #0x54]
005934a0: str      ip, [sp, #0x5c]
005934a4: ldr      r2, [sp, #0x10]
005934a8: cmp      r1, r3
005934ac: str      r5, [sp, #0x44]
005934b0: str      r2, [sp, #0x60]
005934b4: beq      #0x5937d8
005934b8: str      r5, [r1]
005934bc: ldr      r3, [sp, #0x48]
005934c0: str      r3, [r1, #4]
005934c4: ldr      r3, [sp, #0x4c]
005934c8: str      r3, [r1, #8]
005934cc: ldr      r3, [sp, #0x50]
005934d0: str      r3, [r1, #0xc]
005934d4: ldr      r3, [sp, #0x54]
005934d8: str      r3, [r1, #0x10]
005934dc: ldr      r3, [sp, #0x58]
005934e0: str      r3, [r1, #0x14]
005934e4: ldr      r3, [sp, #0x5c]
005934e8: str      r3, [r1, #0x18]
005934ec: ldr      r3, [sp, #0x60]
005934f0: str      r3, [r1, #0x1c]
005934f4: ldr      r3, [sp, #0x64]
005934f8: str      r3, [r1, #0x20]
005934fc: ldr      r3, [r6, #4]
00593500: add      r3, r3, #0x24
00593504: str      r3, [r6, #4]
00593508: ldr      r3, [sp, #0x14]
0059350c: add      r4, r4, #3
00593510: cmp      r3, r4
00593514: bhi      #0x5933c8
00593518: ldr      r5, [sp, #0x18]
0059351c: b        #0x592ff0
00593520: cmp      r8, #0
00593524: beq      #0x592ff0
00593528: add      r2, sp, #0xd4
0059352c: ldrh     r3, [r5, #0xe]
00593530: mov      sl, #0
00593534: str      r2, [sp, #0x14]
00593538: str      r8, [sp, #0xc]
0059353c: str      r5, [sp, #0x10]
00593540: b        #0x59354c
00593544: ldr      r2, [sp, #0x10]
00593548: ldrh     r3, [r2, #0xe]
0059354c: add      r8, r4, #2
00593550: mul      r8, r3, r8
00593554: ldrsb    r0, [r7, r8]
00593558: str      r3, [sp, #8]
0059355c: bl       #0x30e964
00593560: add      r8, r7, r8
00593564: mov      r5, r0
00593568: ldrsb    r0, [r8, #1]
0059356c: bl       #0x30e964
00593570: ldr      r3, [sp, #8]
00593574: mov      fp, r0
00593578: mla      r8, r4, r3, r3
0059357c: ldrsb    r0, [r7, r8]
00593580: bl       #0x30e964
00593584: add      r8, r7, r8
00593588: mov      sb, r0
0059358c: ldrsb    r0, [r8, #1]
00593590: bl       #0x30e964
00593594: ldr      r3, [sp, #8]
00593598: mov      r8, r0
0059359c: mul      r3, r4, r3
005935a0: ldrsb    r0, [r7, r3]
005935a4: add      r3, r7, r3
005935a8: str      r3, [sp, #8]
005935ac: bl       #0x30e964
005935b0: ldr      r3, [sp, #8]
005935b4: mov      r2, r0
005935b8: ldrsb    r0, [r3, #1]
005935bc: str      r2, [sp, #4]
005935c0: bl       #0x30e964
005935c4: ldmib    r6, {r1, r3}
005935c8: ldr      r2, [sp, #4]
005935cc: str      fp, [sp, #0xd8]
005935d0: cmp      r1, r3
005935d4: str      sb, [sp, #0xe0]
005935d8: str      r8, [sp, #0xe4]
005935dc: str      r0, [sp, #0xf0]
005935e0: str      r2, [sp, #0xec]
005935e4: str      sl, [sp, #0xdc]
005935e8: str      sl, [sp, #0xe8]
005935ec: str      r5, [sp, #0xd4]
005935f0: str      sl, [sp, #0xf4]
005935f4: beq      #0x5937e8
005935f8: str      r5, [r1]
005935fc: ldr      r3, [sp, #0xd8]
00593600: str      r3, [r1, #4]
00593604: ldr      r3, [sp, #0xdc]
00593608: str      r3, [r1, #8]
0059360c: ldr      r3, [sp, #0xe0]
00593610: str      r3, [r1, #0xc]
00593614: ldr      r3, [sp, #0xe4]
00593618: str      r3, [r1, #0x10]
0059361c: ldr      r3, [sp, #0xe8]
00593620: str      r3, [r1, #0x14]
00593624: ldr      r3, [sp, #0xec]
00593628: str      r3, [r1, #0x18]
0059362c: ldr      r3, [sp, #0xf0]
00593630: str      r3, [r1, #0x1c]
00593634: ldr      r3, [sp, #0xf4]
00593638: str      r3, [r1, #0x20]
0059363c: ldr      r3, [r6, #4]
00593640: add      r3, r3, #0x24
00593644: str      r3, [r6, #4]
00593648: ldr      r3, [sp, #0xc]
0059364c: add      r4, r4, #3
00593650: cmp      r3, r4
00593654: bhi      #0x593544
00593658: ldr      r5, [sp, #0x10]
0059365c: b        #0x592ff0
00593660: cmp      r8, #0
00593664: beq      #0x592ff0
00593668: add      r2, sp, #0x8c
0059366c: ldrh     r3, [r5, #0xe]
00593670: str      r2, [sp, #0x1c]
00593674: str      r8, [sp, #0x14]
00593678: str      r5, [sp, #0x18]
0059367c: b        #0x593688
00593680: ldr      r2, [sp, #0x18]
00593684: ldrh     r3, [r2, #0xe]
00593688: add      r8, r4, #2
0059368c: mul      r8, r3, r8
00593690: ldrsb    r0, [r7, r8]
00593694: str      r3, [sp, #8]
00593698: bl       #0x30e964
0059369c: add      r8, r7, r8
005936a0: mov      r5, r0
005936a4: ldrsb    r0, [r8, #1]
005936a8: bl       #0x30e964
005936ac: mov      fp, r0
005936b0: ldrsb    r0, [r8, #2]
005936b4: bl       #0x30e964
005936b8: ldr      r3, [sp, #8]
005936bc: mov      sb, r0
005936c0: mla      r2, r4, r3, r3
005936c4: ldrsb    r0, [r7, r2]
005936c8: add      r2, r7, r2
005936cc: str      r2, [sp, #0xc]
005936d0: bl       #0x30e964
005936d4: ldr      r2, [sp, #0xc]
005936d8: mov      sl, r0
005936dc: ldrsb    r0, [r2, #1]
005936e0: bl       #0x30e964
005936e4: ldr      r2, [sp, #0xc]
005936e8: mov      r8, r0
005936ec: ldrsb    r0, [r2, #2]
005936f0: bl       #0x30e964
005936f4: ldr      r3, [sp, #8]
005936f8: mov      r2, r0
005936fc: mul      r3, r4, r3
00593700: ldrsb    r0, [r7, r3]
00593704: add      r3, r7, r3
00593708: str      r2, [sp, #4]
0059370c: str      r3, [sp, #0xc]
00593710: bl       #0x30e964
00593714: ldr      r3, [sp, #0xc]
00593718: mov      ip, r0
0059371c: ldrsb    r0, [r3, #1]
00593720: str      ip, [sp, #8]
00593724: bl       #0x30e964
00593728: ldr      r3, [sp, #0xc]
0059372c: str      r0, [sp, #0x10]
00593730: ldrsb    r0, [r3, #2]
00593734: bl       #0x30e964
00593738: ldmib    sp, {r2, ip}
0059373c: ldmib    r6, {r1, r3}
00593740: str      r2, [sp, #0xa0]
00593744: str      fp, [sp, #0x90]
00593748: str      sb, [sp, #0x94]
0059374c: str      r0, [sp, #0xac]
00593750: str      sl, [sp, #0x98]
00593754: str      r8, [sp, #0x9c]
00593758: str      ip, [sp, #0xa4]
0059375c: ldr      r2, [sp, #0x10]
00593760: cmp      r1, r3
00593764: str      r5, [sp, #0x8c]
00593768: str      r2, [sp, #0xa8]
0059376c: beq      #0x5937f8
00593770: str      r5, [r1]
00593774: ldr      r3, [sp, #0x90]
00593778: str      r3, [r1, #4]
0059377c: ldr      r3, [sp, #0x94]
00593780: str      r3, [r1, #8]
00593784: ldr      r3, [sp, #0x98]
00593788: str      r3, [r1, #0xc]
0059378c: ldr      r3, [sp, #0x9c]
00593790: str      r3, [r1, #0x10]
00593794: ldr      r3, [sp, #0xa0]
00593798: str      r3, [r1, #0x14]
0059379c: ldr      r3, [sp, #0xa4]
005937a0: str      r3, [r1, #0x18]
005937a4: ldr      r3, [sp, #0xa8]
005937a8: str      r3, [r1, #0x1c]
005937ac: ldr      r3, [sp, #0xac]
005937b0: str      r3, [r1, #0x20]
005937b4: ldr      r3, [r6, #4]
005937b8: add      r3, r3, #0x24
005937bc: str      r3, [r6, #4]
005937c0: ldr      r3, [sp, #0x14]
005937c4: add      r4, r4, #3
005937c8: cmp      r3, r4
005937cc: bhi      #0x593680
005937d0: ldr      r5, [sp, #0x18]
005937d4: b        #0x592ff0
005937d8: mov      r0, r6
005937dc: ldr      r2, [sp, #0x1c]
005937e0: bl       #0x591238
005937e4: b        #0x593508
005937e8: mov      r0, r6
005937ec: ldr      r2, [sp, #0x14]
005937f0: bl       #0x591238
005937f4: b        #0x593648
005937f8: mov      r0, r6
005937fc: ldr      r2, [sp, #0x1c]
00593800: bl       #0x591238
00593804: b        #0x5937c0

# _ZN6glitch5scene12_GLOBAL__N_115createTrianglesIhSt6vectorINS_4core10triangle3dIfEENS4_10SAllocatorIS6_LNS_6memory13E_MEMORY_HINTE0EEEEEEvPKtjRKNS_5video13SVertexStreamERT0_
00593808: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0059380c: mov      r5, r2
00593810: ldrh     r2, [r2, #0xc]
00593814: sub      sp, sp, #0xfc
00593818: mov      r4, r0
0059381c: cmp      r2, #3
00593820: mov      r8, r1
00593824: mov      r6, r3
00593828: beq      #0x593b68
0059382c: cmp      r2, #4
00593830: beq      #0x5939d0
00593834: cmp      r2, #2
00593838: beq      #0x593844
0059383c: add      sp, sp, #0xfc
00593840: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00593844: ldr      r0, [r5]
00593848: mov      r1, #1
0059384c: bl       #0x5a1adc
00593850: ldr      r7, [r5, #4]
00593854: cmp      r4, #0
00593858: add      r7, r0, r7
0059385c: beq      #0x593ed0
00593860: add      r8, r4, r8, lsl #1
00593864: cmp      r4, r8
00593868: str      r8, [sp, #0xc]
0059386c: beq      #0x5939a0
00593870: add      r2, sp, #0xb0
00593874: mov      r8, #0
00593878: str      r2, [sp, #0x14]
0059387c: str      r5, [sp, #0x10]
00593880: ldr      r2, [sp, #0x10]
00593884: ldrh     sl, [r4, #4]
00593888: ldrh     r3, [r2, #0xe]
0059388c: mul      sl, sl, r3
00593890: ldrb     r0, [r7, sl]
00593894: str      r3, [sp, #8]
00593898: bl       #0x30e2e0
0059389c: add      sl, r7, sl
005938a0: mov      r5, r0
005938a4: ldrb     r0, [sl, #1]
005938a8: bl       #0x30e2e0
005938ac: ldrh     sl, [r4, #2]
005938b0: ldr      r3, [sp, #8]
005938b4: mov      fp, r0
005938b8: mul      sl, r3, sl
005938bc: ldrb     r0, [r7, sl]
005938c0: bl       #0x30e2e0
005938c4: add      sl, r7, sl
005938c8: mov      sb, r0
005938cc: ldrb     r0, [sl, #1]
005938d0: bl       #0x30e2e0
005938d4: ldrh     r2, [r4]
005938d8: ldr      r3, [sp, #8]
005938dc: mov      sl, r0
005938e0: mul      r3, r3, r2
005938e4: ldrb     r0, [r7, r3]
005938e8: add      r3, r7, r3
005938ec: str      r3, [sp, #8]
005938f0: bl       #0x30e2e0
005938f4: ldr      r3, [sp, #8]
005938f8: mov      r2, r0
005938fc: ldrb     r0, [r3, #1]
00593900: str      r2, [sp, #4]
00593904: bl       #0x30e2e0
00593908: ldmib    r6, {r1, r3}
0059390c: ldr      r2, [sp, #4]
00593910: str      fp, [sp, #0xb4]
00593914: cmp      r1, r3
00593918: str      sb, [sp, #0xbc]
0059391c: str      r0, [sp, #0xcc]
00593920: str      sl, [sp, #0xc0]
00593924: str      r2, [sp, #0xc8]
00593928: str      r8, [sp, #0xb8]
0059392c: str      r5, [sp, #0xb0]
00593930: str      r8, [sp, #0xc4]
00593934: str      r8, [sp, #0xd0]
00593938: beq      #0x593d38
0059393c: str      r5, [r1]
00593940: ldr      r3, [sp, #0xb4]
00593944: str      r3, [r1, #4]
00593948: ldr      r3, [sp, #0xb8]
0059394c: str      r3, [r1, #8]
00593950: ldr      r3, [sp, #0xbc]
00593954: str      r3, [r1, #0xc]
00593958: ldr      r3, [sp, #0xc0]
0059395c: str      r3, [r1, #0x10]
00593960: ldr      r3, [sp, #0xc4]
00593964: str      r3, [r1, #0x14]
00593968: ldr      r3, [sp, #0xc8]
0059396c: str      r3, [r1, #0x18]
00593970: ldr      r3, [sp, #0xcc]
00593974: str      r3, [r1, #0x1c]
00593978: ldr      r3, [sp, #0xd0]
0059397c: str      r3, [r1, #0x20]
00593980: ldr      r3, [r6, #4]
00593984: add      r3, r3, #0x24
00593988: str      r3, [r6, #4]
0059398c: ldr      r3, [sp, #0xc]
00593990: add      r4, r4, #6
00593994: cmp      r3, r4
00593998: bne      #0x593880
0059399c: ldr      r5, [sp, #0x10]
005939a0: cmp      r7, #0
005939a4: beq      #0x59383c
005939a8: ldr      r4, [r5]
005939ac: ldrb     r3, [r4, #0x13]
005939b0: and      r2, r3, #0x1f
005939b4: cmp      r2, #1
005939b8: bls      #0x593cfc
005939bc: sub      r2, r2, #1
005939c0: bic      r3, r3, #0x1f
005939c4: orr      r3, r2, r3
005939c8: strb     r3, [r4, #0x13]
005939cc: b        #0x59383c
005939d0: ldr      r0, [r5]
005939d4: mov      r1, #1
005939d8: bl       #0x5a1adc
005939dc: ldr      r7, [r5, #4]
005939e0: cmp      r4, #0
005939e4: add      r7, r0, r7
005939e8: beq      #0x593d58
005939ec: add      r8, r4, r8, lsl #1
005939f0: cmp      r4, r8
005939f4: str      r8, [sp, #0x14]
005939f8: beq      #0x5939a0
005939fc: add      r3, sp, #0x20
00593a00: str      r3, [sp, #0x1c]
00593a04: str      r5, [sp, #0x18]
00593a08: ldr      r2, [sp, #0x18]
00593a0c: ldrh     r8, [r4, #4]
00593a10: ldrh     r3, [r2, #0xe]
00593a14: mul      r8, r8, r3
00593a18: ldrb     r0, [r7, r8]
00593a1c: str      r3, [sp, #8]
00593a20: bl       #0x30e2e0
00593a24: add      r8, r7, r8
00593a28: mov      r5, r0
00593a2c: ldrb     r0, [r8, #1]
00593a30: bl       #0x30e2e0
00593a34: mov      fp, r0
00593a38: ldrb     r0, [r8, #2]
00593a3c: bl       #0x30e2e0
00593a40: ldrh     r2, [r4, #2]
00593a44: ldr      r3, [sp, #8]
00593a48: mov      sb, r0
00593a4c: mul      r2, r3, r2
00593a50: ldrb     r0, [r7, r2]
00593a54: add      r2, r7, r2
00593a58: str      r2, [sp, #0xc]
00593a5c: bl       #0x30e2e0
00593a60: ldr      r2, [sp, #0xc]
00593a64: mov      sl, r0
00593a68: ldrb     r0, [r2, #1]
00593a6c: bl       #0x30e2e0
00593a70: ldr      r2, [sp, #0xc]
00593a74: mov      r8, r0
00593a78: ldrb     r0, [r2, #2]
00593a7c: bl       #0x30e2e0
00593a80: ldrh     r1, [r4]
00593a84: ldr      r3, [sp, #8]
00593a88: mov      r2, r0
00593a8c: mul      r3, r3, r1
00593a90: ldrb     r0, [r7, r3]
00593a94: add      r3, r7, r3
00593a98: str      r2, [sp, #4]
00593a9c: str      r3, [sp, #0xc]
00593aa0: bl       #0x30e2e0
00593aa4: ldr      r3, [sp, #0xc]
00593aa8: mov      ip, r0
00593aac: ldrb     r0, [r3, #1]
00593ab0: str      ip, [sp, #8]
00593ab4: bl       #0x30e2e0
00593ab8: ldr      r3, [sp, #0xc]
00593abc: str      r0, [sp, #0x10]
00593ac0: ldrb     r0, [r3, #2]
00593ac4: bl       #0x30e2e0
00593ac8: ldmib    r6, {r1, r3}
00593acc: str      r0, [sp, #0x40]
00593ad0: str      fp, [sp, #0x24]
00593ad4: str      sb, [sp, #0x28]
00593ad8: ldmib    sp, {r2, ip}
00593adc: cmp      r1, r3
00593ae0: str      r2, [sp, #0x34]
00593ae4: ldr      r2, [sp, #0x10]
00593ae8: str      sl, [sp, #0x2c]
00593aec: str      r8, [sp, #0x30]
00593af0: str      ip, [sp, #0x38]
00593af4: str      r2, [sp, #0x3c]
00593af8: str      r5, [sp, #0x20]
00593afc: beq      #0x593d48
00593b00: str      r5, [r1]
00593b04: ldr      r3, [sp, #0x24]
00593b08: str      r3, [r1, #4]
00593b0c: ldr      r3, [sp, #0x28]
00593b10: str      r3, [r1, #8]
00593b14: ldr      r3, [sp, #0x2c]
00593b18: str      r3, [r1, #0xc]
00593b1c: ldr      r3, [sp, #0x30]
00593b20: str      r3, [r1, #0x10]
00593b24: ldr      r3, [sp, #0x34]
00593b28: str      r3, [r1, #0x14]
00593b2c: ldr      r3, [sp, #0x38]
00593b30: str      r3, [r1, #0x18]
00593b34: ldr      r3, [sp, #0x3c]
00593b38: str      r3, [r1, #0x1c]
00593b3c: ldr      r3, [sp, #0x40]
00593b40: str      r3, [r1, #0x20]
00593b44: ldr      r3, [r6, #4]
00593b48: add      r3, r3, #0x24
00593b4c: str      r3, [r6, #4]
00593b50: ldr      r3, [sp, #0x14]
00593b54: add      r4, r4, #6
00593b58: cmp      r3, r4
00593b5c: bne      #0x593a08
00593b60: ldr      r5, [sp, #0x18]
00593b64: b        #0x5939a0
00593b68: ldr      r0, [r5]
00593b6c: mov      r1, #1
00593b70: bl       #0x5a1adc
00593b74: ldr      r7, [r5, #4]
00593b78: cmp      r4, #0
00593b7c: add      r7, r0, r7
00593b80: beq      #0x594010
00593b84: add      r8, r4, r8, lsl #1
00593b88: cmp      r4, r8
00593b8c: str      r8, [sp, #0x14]
00593b90: beq      #0x5939a0
00593b94: add      r3, sp, #0x68
00593b98: str      r3, [sp, #0x1c]
00593b9c: str      r5, [sp, #0x18]
00593ba0: ldr      r2, [sp, #0x18]
00593ba4: ldrh     r8, [r4, #4]
00593ba8: ldrh     r3, [r2, #0xe]
00593bac: mul      r8, r8, r3
00593bb0: ldrb     r0, [r7, r8]
00593bb4: str      r3, [sp, #8]
00593bb8: bl       #0x30e2e0
00593bbc: add      r8, r7, r8
00593bc0: mov      r5, r0
00593bc4: ldrb     r0, [r8, #1]
00593bc8: bl       #0x30e2e0
00593bcc: mov      fp, r0
00593bd0: ldrb     r0, [r8, #2]
00593bd4: bl       #0x30e2e0
00593bd8: ldrh     r2, [r4, #2]
00593bdc: ldr      r3, [sp, #8]
00593be0: mov      sb, r0
00593be4: mul      r2, r3, r2
00593be8: ldrb     r0, [r7, r2]
00593bec: add      r2, r7, r2
00593bf0: str      r2, [sp, #0xc]
00593bf4: bl       #0x30e2e0
00593bf8: ldr      r2, [sp, #0xc]
00593bfc: mov      sl, r0
00593c00: ldrb     r0, [r2, #1]
00593c04: bl       #0x30e2e0
00593c08: ldr      r2, [sp, #0xc]
00593c0c: mov      r8, r0
00593c10: ldrb     r0, [r2, #2]
00593c14: bl       #0x30e2e0
00593c18: ldrh     r1, [r4]
00593c1c: ldr      r3, [sp, #8]
00593c20: mov      r2, r0
00593c24: mul      r3, r3, r1
00593c28: ldrb     r0, [r7, r3]
00593c2c: add      r3, r7, r3
00593c30: str      r2, [sp, #4]
00593c34: str      r3, [sp, #0xc]
00593c38: bl       #0x30e2e0
00593c3c: ldr      r3, [sp, #0xc]
00593c40: mov      ip, r0
00593c44: ldrb     r0, [r3, #1]
00593c48: str      ip, [sp, #8]
00593c4c: bl       #0x30e2e0
00593c50: ldr      r3, [sp, #0xc]
00593c54: str      r0, [sp, #0x10]
00593c58: ldrb     r0, [r3, #2]
00593c5c: bl       #0x30e2e0
00593c60: ldmib    r6, {r1, r3}
00593c64: str      r0, [sp, #0x88]
00593c68: str      fp, [sp, #0x6c]
00593c6c: str      sb, [sp, #0x70]
00593c70: ldmib    sp, {r2, ip}
00593c74: cmp      r1, r3
00593c78: str      r2, [sp, #0x7c]
00593c7c: ldr      r2, [sp, #0x10]
00593c80: str      sl, [sp, #0x74]
00593c84: str      r8, [sp, #0x78]
00593c88: str      ip, [sp, #0x80]
00593c8c: str      r2, [sp, #0x84]
00593c90: str      r5, [sp, #0x68]
00593c94: beq      #0x593d28
00593c98: str      r5, [r1]
00593c9c: ldr      r3, [sp, #0x6c]
00593ca0: str      r3, [r1, #4]
00593ca4: ldr      r3, [sp, #0x70]
00593ca8: str      r3, [r1, #8]
00593cac: ldr      r3, [sp, #0x74]
00593cb0: str      r3, [r1, #0xc]
00593cb4: ldr      r3, [sp, #0x78]
00593cb8: str      r3, [r1, #0x10]
00593cbc: ldr      r3, [sp, #0x7c]
00593cc0: str      r3, [r1, #0x14]
00593cc4: ldr      r3, [sp, #0x80]
00593cc8: str      r3, [r1, #0x18]
00593ccc: ldr      r3, [sp, #0x84]
00593cd0: str      r3, [r1, #0x1c]
00593cd4: ldr      r3, [sp, #0x88]
00593cd8: str      r3, [r1, #0x20]
00593cdc: ldr      r3, [r6, #4]
00593ce0: add      r3, r3, #0x24
00593ce4: str      r3, [r6, #4]
00593ce8: ldr      r3, [sp, #0x14]
00593cec: add      r4, r4, #6
00593cf0: cmp      r3, r4
00593cf4: bne      #0x593ba0
00593cf8: b        #0x593b60
00593cfc: ldrb     r3, [r4, #0x12]
00593d00: tst      r3, #0x20
00593d04: bne      #0x593d14
00593d08: mov      r3, #0
00593d0c: strb     r3, [r4, #0x13]
00593d10: b        #0x59383c
00593d14: ldr      r3, [r4]
00593d18: mov      r0, r4
00593d1c: mov      lr, pc
00593d20: ldr      pc, [r3, #0x18]
00593d24: b        #0x593d08
00593d28: mov      r0, r6
00593d2c: ldr      r2, [sp, #0x1c]
00593d30: bl       #0x591238
00593d34: b        #0x593ce8
00593d38: mov      r0, r6
00593d3c: ldr      r2, [sp, #0x14]
00593d40: bl       #0x591238
00593d44: b        #0x59398c
00593d48: mov      r0, r6
00593d4c: ldr      r2, [sp, #0x1c]
00593d50: bl       #0x591238
00593d54: b        #0x593b50
00593d58: cmp      r8, #0
00593d5c: beq      #0x5939a0
00593d60: add      r2, sp, #0x44
00593d64: ldrh     r3, [r5, #0xe]
00593d68: str      r2, [sp, #0x1c]
00593d6c: str      r8, [sp, #0x14]
00593d70: str      r5, [sp, #0x18]
00593d74: b        #0x593d80
00593d78: ldr      r2, [sp, #0x18]
00593d7c: ldrh     r3, [r2, #0xe]
00593d80: add      r8, r4, #2
00593d84: mul      r8, r3, r8
00593d88: ldrb     r0, [r7, r8]
00593d8c: str      r3, [sp, #8]
00593d90: bl       #0x30e2e0
00593d94: add      r8, r7, r8
00593d98: mov      r5, r0
00593d9c: ldrb     r0, [r8, #1]
00593da0: bl       #0x30e2e0
00593da4: mov      fp, r0
00593da8: ldrb     r0, [r8, #2]
00593dac: bl       #0x30e2e0
00593db0: ldr      r3, [sp, #8]
00593db4: mov      sb, r0
00593db8: mla      r2, r4, r3, r3
00593dbc: ldrb     r0, [r7, r2]
00593dc0: add      r2, r7, r2
00593dc4: str      r2, [sp, #0xc]
00593dc8: bl       #0x30e2e0
00593dcc: ldr      r2, [sp, #0xc]
00593dd0: mov      sl, r0
00593dd4: ldrb     r0, [r2, #1]
00593dd8: bl       #0x30e2e0
00593ddc: ldr      r2, [sp, #0xc]
00593de0: mov      r8, r0
00593de4: ldrb     r0, [r2, #2]
00593de8: bl       #0x30e2e0
00593dec: ldr      r3, [sp, #8]
00593df0: mov      r2, r0
00593df4: mul      r3, r4, r3
00593df8: ldrb     r0, [r7, r3]
00593dfc: add      r3, r7, r3
00593e00: str      r2, [sp, #4]
00593e04: str      r3, [sp, #0xc]
00593e08: bl       #0x30e2e0
00593e0c: ldr      r3, [sp, #0xc]
00593e10: mov      ip, r0
00593e14: ldrb     r0, [r3, #1]
00593e18: str      ip, [sp, #8]
00593e1c: bl       #0x30e2e0
00593e20: ldr      r3, [sp, #0xc]
00593e24: str      r0, [sp, #0x10]
00593e28: ldrb     r0, [r3, #2]
00593e2c: bl       #0x30e2e0
00593e30: ldmib    sp, {r2, ip}
00593e34: ldmib    r6, {r1, r3}
00593e38: str      r2, [sp, #0x58]
00593e3c: str      fp, [sp, #0x48]
00593e40: str      sb, [sp, #0x4c]
00593e44: str      r0, [sp, #0x64]
00593e48: str      sl, [sp, #0x50]
00593e4c: str      r8, [sp, #0x54]
00593e50: str      ip, [sp, #0x5c]
00593e54: ldr      r2, [sp, #0x10]
00593e58: cmp      r1, r3
00593e5c: str      r5, [sp, #0x44]
00593e60: str      r2, [sp, #0x60]
00593e64: beq      #0x594188
00593e68: str      r5, [r1]
00593e6c: ldr      r3, [sp, #0x48]
00593e70: str      r3, [r1, #4]
00593e74: ldr      r3, [sp, #0x4c]
00593e78: str      r3, [r1, #8]
00593e7c: ldr      r3, [sp, #0x50]
00593e80: str      r3, [r1, #0xc]
00593e84: ldr      r3, [sp, #0x54]
00593e88: str      r3, [r1, #0x10]
00593e8c: ldr      r3, [sp, #0x58]
00593e90: str      r3, [r1, #0x14]
00593e94: ldr      r3, [sp, #0x5c]
00593e98: str      r3, [r1, #0x18]
00593e9c: ldr      r3, [sp, #0x60]
00593ea0: str      r3, [r1, #0x1c]
00593ea4: ldr      r3, [sp, #0x64]
00593ea8: str      r3, [r1, #0x20]
00593eac: ldr      r3, [r6, #4]
00593eb0: add      r3, r3, #0x24
00593eb4: str      r3, [r6, #4]
00593eb8: ldr      r3, [sp, #0x14]
00593ebc: add      r4, r4, #3
00593ec0: cmp      r3, r4
00593ec4: bhi      #0x593d78
00593ec8: ldr      r5, [sp, #0x18]
00593ecc: b        #0x5939a0
00593ed0: cmp      r8, #0
00593ed4: beq      #0x5939a0
00593ed8: add      r2, sp, #0xd4
00593edc: ldrh     r3, [r5, #0xe]
00593ee0: mov      sl, #0
00593ee4: str      r2, [sp, #0x14]
00593ee8: str      r8, [sp, #0xc]
00593eec: str      r5, [sp, #0x10]
00593ef0: b        #0x593efc
00593ef4: ldr      r2, [sp, #0x10]
00593ef8: ldrh     r3, [r2, #0xe]
00593efc: add      r8, r4, #2
00593f00: mul      r8, r3, r8
00593f04: ldrb     r0, [r7, r8]
00593f08: str      r3, [sp, #8]
00593f0c: bl       #0x30e2e0
00593f10: add      r8, r7, r8
00593f14: mov      r5, r0
00593f18: ldrb     r0, [r8, #1]
00593f1c: bl       #0x30e2e0
00593f20: ldr      r3, [sp, #8]
00593f24: mov      fp, r0
00593f28: mla      r8, r4, r3, r3
00593f2c: ldrb     r0, [r7, r8]
00593f30: bl       #0x30e2e0
00593f34: add      r8, r7, r8
00593f38: mov      sb, r0
00593f3c: ldrb     r0, [r8, #1]
00593f40: bl       #0x30e2e0
00593f44: ldr      r3, [sp, #8]
00593f48: mov      r8, r0
00593f4c: mul      r3, r4, r3
00593f50: ldrb     r0, [r7, r3]
00593f54: add      r3, r7, r3
00593f58: str      r3, [sp, #8]
00593f5c: bl       #0x30e2e0
00593f60: ldr      r3, [sp, #8]
00593f64: mov      r2, r0
00593f68: ldrb     r0, [r3, #1]
00593f6c: str      r2, [sp, #4]
00593f70: bl       #0x30e2e0
00593f74: ldmib    r6, {r1, r3}
00593f78: ldr      r2, [sp, #4]
00593f7c: str      fp, [sp, #0xd8]
00593f80: cmp      r1, r3
00593f84: str      sb, [sp, #0xe0]
00593f88: str      r8, [sp, #0xe4]
00593f8c: str      r0, [sp, #0xf0]
00593f90: str      r2, [sp, #0xec]
00593f94: str      sl, [sp, #0xdc]
00593f98: str      sl, [sp, #0xe8]
00593f9c: str      r5, [sp, #0xd4]
00593fa0: str      sl, [sp, #0xf4]
00593fa4: beq      #0x594198
00593fa8: str      r5, [r1]
00593fac: ldr      r3, [sp, #0xd8]
00593fb0: str      r3, [r1, #4]
00593fb4: ldr      r3, [sp, #0xdc]
00593fb8: str      r3, [r1, #8]
00593fbc: ldr      r3, [sp, #0xe0]
00593fc0: str      r3, [r1, #0xc]
00593fc4: ldr      r3, [sp, #0xe4]
00593fc8: str      r3, [r1, #0x10]
00593fcc: ldr      r3, [sp, #0xe8]
00593fd0: str      r3, [r1, #0x14]
00593fd4: ldr      r3, [sp, #0xec]
00593fd8: str      r3, [r1, #0x18]
00593fdc: ldr      r3, [sp, #0xf0]
00593fe0: str      r3, [r1, #0x1c]
00593fe4: ldr      r3, [sp, #0xf4]
00593fe8: str      r3, [r1, #0x20]
00593fec: ldr      r3, [r6, #4]
00593ff0: add      r3, r3, #0x24
00593ff4: str      r3, [r6, #4]
00593ff8: ldr      r3, [sp, #0xc]
00593ffc: add      r4, r4, #3
00594000: cmp      r3, r4
00594004: bhi      #0x593ef4
00594008: ldr      r5, [sp, #0x10]
0059400c: b        #0x5939a0
00594010: cmp      r8, #0
00594014: beq      #0x5939a0
00594018: add      r2, sp, #0x8c
0059401c: ldrh     r3, [r5, #0xe]
00594020: str      r2, [sp, #0x1c]
00594024: str      r8, [sp, #0x14]
00594028: str      r5, [sp, #0x18]
0059402c: b        #0x594038
00594030: ldr      r2, [sp, #0x18]
00594034: ldrh     r3, [r2, #0xe]
00594038: add      r8, r4, #2
0059403c: mul      r8, r3, r8
00594040: ldrb     r0, [r7, r8]
00594044: str      r3, [sp, #8]
00594048: bl       #0x30e2e0
0059404c: add      r8, r7, r8
00594050: mov      r5, r0
00594054: ldrb     r0, [r8, #1]
00594058: bl       #0x30e2e0
0059405c: mov      fp, r0
00594060: ldrb     r0, [r8, #2]
00594064: bl       #0x30e2e0
00594068: ldr      r3, [sp, #8]
0059406c: mov      sb, r0
00594070: mla      r2, r4, r3, r3
00594074: ldrb     r0, [r7, r2]
00594078: add      r2, r7, r2
0059407c: str      r2, [sp, #0xc]
00594080: bl       #0x30e2e0
00594084: ldr      r2, [sp, #0xc]
00594088: mov      sl, r0
0059408c: ldrb     r0, [r2, #1]
00594090: bl       #0x30e2e0
00594094: ldr      r2, [sp, #0xc]
00594098: mov      r8, r0
0059409c: ldrb     r0, [r2, #2]
005940a0: bl       #0x30e2e0
005940a4: ldr      r3, [sp, #8]
005940a8: mov      r2, r0
005940ac: mul      r3, r4, r3
005940b0: ldrb     r0, [r7, r3]
005940b4: add      r3, r7, r3
005940b8: str      r2, [sp, #4]
005940bc: str      r3, [sp, #0xc]
005940c0: bl       #0x30e2e0
005940c4: ldr      r3, [sp, #0xc]
005940c8: mov      ip, r0
005940cc: ldrb     r0, [r3, #1]
005940d0: str      ip, [sp, #8]
005940d4: bl       #0x30e2e0
005940d8: ldr      r3, [sp, #0xc]
005940dc: str      r0, [sp, #0x10]
005940e0: ldrb     r0, [r3, #2]
005940e4: bl       #0x30e2e0
005940e8: ldmib    sp, {r2, ip}
005940ec: ldmib    r6, {r1, r3}
005940f0: str      r2, [sp, #0xa0]
005940f4: str      fp, [sp, #0x90]
005940f8: str      sb, [sp, #0x94]
005940fc: str      r0, [sp, #0xac]
00594100: str      sl, [sp, #0x98]
00594104: str      r8, [sp, #0x9c]
00594108: str      ip, [sp, #0xa4]
0059410c: ldr      r2, [sp, #0x10]
00594110: cmp      r1, r3
00594114: str      r5, [sp, #0x8c]
00594118: str      r2, [sp, #0xa8]
0059411c: beq      #0x5941a8
00594120: str      r5, [r1]
00594124: ldr      r3, [sp, #0x90]
00594128: str      r3, [r1, #4]
0059412c: ldr      r3, [sp, #0x94]
00594130: str      r3, [r1, #8]
00594134: ldr      r3, [sp, #0x98]
00594138: str      r3, [r1, #0xc]
0059413c: ldr      r3, [sp, #0x9c]
00594140: str      r3, [r1, #0x10]
00594144: ldr      r3, [sp, #0xa0]
00594148: str      r3, [r1, #0x14]
0059414c: ldr      r3, [sp, #0xa4]
00594150: str      r3, [r1, #0x18]
00594154: ldr      r3, [sp, #0xa8]
00594158: str      r3, [r1, #0x1c]
0059415c: ldr      r3, [sp, #0xac]
00594160: str      r3, [r1, #0x20]
00594164: ldr      r3, [r6, #4]
00594168: add      r3, r3, #0x24
0059416c: str      r3, [r6, #4]
00594170: ldr      r3, [sp, #0x14]
00594174: add      r4, r4, #3
00594178: cmp      r3, r4
0059417c: bhi      #0x594030
00594180: ldr      r5, [sp, #0x18]
00594184: b        #0x5939a0
00594188: mov      r0, r6
0059418c: ldr      r2, [sp, #0x1c]
00594190: bl       #0x591238
00594194: b        #0x593eb8
00594198: mov      r0, r6
0059419c: ldr      r2, [sp, #0x14]
005941a0: bl       #0x591238
005941a4: b        #0x593ff8
005941a8: mov      r0, r6
005941ac: ldr      r2, [sp, #0x1c]
005941b0: bl       #0x591238
005941b4: b        #0x594170

# _ZN6glitch5scene12_GLOBAL__N_115createTrianglesIsSt6vectorINS_4core10triangle3dIfEENS4_10SAllocatorIS6_LNS_6memory13E_MEMORY_HINTE0EEEEEEvPKtjRKNS_5video13SVertexStreamERT0_
005941b8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005941bc: mov      r5, r2
005941c0: ldrh     r2, [r2, #0xc]
005941c4: sub      sp, sp, #0xfc
005941c8: mov      r4, r0
005941cc: cmp      r2, #3
005941d0: mov      r8, r1
005941d4: mov      r6, r3
005941d8: beq      #0x594518
005941dc: cmp      r2, #4
005941e0: beq      #0x594380
005941e4: cmp      r2, #2
005941e8: beq      #0x5941f4
005941ec: add      sp, sp, #0xfc
005941f0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005941f4: ldr      r0, [r5]
005941f8: mov      r1, #1
005941fc: bl       #0x5a1adc
00594200: ldr      r7, [r5, #4]
00594204: cmp      r4, #0
00594208: add      r7, r0, r7
0059420c: beq      #0x594880
00594210: add      r8, r4, r8, lsl #1
00594214: cmp      r4, r8
00594218: str      r8, [sp, #0xc]
0059421c: beq      #0x594350
00594220: add      r2, sp, #0xb0
00594224: mov      r8, #0
00594228: str      r2, [sp, #0x14]
0059422c: str      r5, [sp, #0x10]
00594230: ldr      r2, [sp, #0x10]
00594234: ldrh     sl, [r4, #4]
00594238: ldrh     r3, [r2, #0xe]
0059423c: mul      sl, sl, r3
00594240: ldrsh    r0, [r7, sl]
00594244: str      r3, [sp, #8]
00594248: bl       #0x30e964
0059424c: add      sl, r7, sl
00594250: mov      r5, r0
00594254: ldrsh    r0, [sl, #2]
00594258: bl       #0x30e964
0059425c: ldrh     sl, [r4, #2]
00594260: ldr      r3, [sp, #8]
00594264: mov      fp, r0
00594268: mul      sl, r3, sl
0059426c: ldrsh    r0, [r7, sl]
00594270: bl       #0x30e964
00594274: add      sl, r7, sl
00594278: mov      sb, r0
0059427c: ldrsh    r0, [sl, #2]
00594280: bl       #0x30e964
00594284: ldrh     r2, [r4]
00594288: ldr      r3, [sp, #8]
0059428c: mov      sl, r0
00594290: mul      r3, r3, r2
00594294: ldrsh    r0, [r7, r3]
00594298: add      r3, r7, r3
0059429c: str      r3, [sp, #8]
005942a0: bl       #0x30e964
005942a4: ldr      r3, [sp, #8]
005942a8: mov      r2, r0
005942ac: ldrsh    r0, [r3, #2]
005942b0: str      r2, [sp, #4]
005942b4: bl       #0x30e964
005942b8: ldmib    r6, {r1, r3}
005942bc: ldr      r2, [sp, #4]
005942c0: str      fp, [sp, #0xb4]
005942c4: cmp      r1, r3
005942c8: str      sb, [sp, #0xbc]
005942cc: str      r0, [sp, #0xcc]
005942d0: str      sl, [sp, #0xc0]
005942d4: str      r2, [sp, #0xc8]
005942d8: str      r8, [sp, #0xb8]
005942dc: str      r5, [sp, #0xb0]
005942e0: str      r8, [sp, #0xc4]
005942e4: str      r8, [sp, #0xd0]
005942e8: beq      #0x5946e8
005942ec: str      r5, [r1]
005942f0: ldr      r3, [sp, #0xb4]
005942f4: str      r3, [r1, #4]
005942f8: ldr      r3, [sp, #0xb8]
005942fc: str      r3, [r1, #8]
00594300: ldr      r3, [sp, #0xbc]
00594304: str      r3, [r1, #0xc]
00594308: ldr      r3, [sp, #0xc0]
0059430c: str      r3, [r1, #0x10]
00594310: ldr      r3, [sp, #0xc4]
00594314: str      r3, [r1, #0x14]
00594318: ldr      r3, [sp, #0xc8]
0059431c: str      r3, [r1, #0x18]
00594320: ldr      r3, [sp, #0xcc]
00594324: str      r3, [r1, #0x1c]
00594328: ldr      r3, [sp, #0xd0]
0059432c: str      r3, [r1, #0x20]
00594330: ldr      r3, [r6, #4]
00594334: add      r3, r3, #0x24
00594338: str      r3, [r6, #4]
0059433c: ldr      r3, [sp, #0xc]
00594340: add      r4, r4, #6
00594344: cmp      r3, r4
00594348: bne      #0x594230
0059434c: ldr      r5, [sp, #0x10]
00594350: cmp      r7, #0
00594354: beq      #0x5941ec
00594358: ldr      r4, [r5]
0059435c: ldrb     r3, [r4, #0x13]
00594360: and      r2, r3, #0x1f
00594364: cmp      r2, #1
00594368: bls      #0x5946ac
0059436c: sub      r2, r2, #1
00594370: bic      r3, r3, #0x1f
00594374: orr      r3, r2, r3
00594378: strb     r3, [r4, #0x13]
0059437c: b        #0x5941ec
00594380: ldr      r0, [r5]
00594384: mov      r1, #1
00594388: bl       #0x5a1adc
0059438c: ldr      r7, [r5, #4]
00594390: cmp      r4, #0
00594394: add      r7, r0, r7
00594398: beq      #0x594708
0059439c: add      r8, r4, r8, lsl #1
005943a0: cmp      r4, r8
005943a4: str      r8, [sp, #0x14]
005943a8: beq      #0x594350
005943ac: add      r3, sp, #0x20
005943b0: str      r3, [sp, #0x1c]
005943b4: str      r5, [sp, #0x18]
005943b8: ldr      r2, [sp, #0x18]
005943bc: ldrh     r8, [r4, #4]
005943c0: ldrh     r3, [r2, #0xe]
005943c4: mul      r8, r8, r3
005943c8: ldrsh    r0, [r7, r8]
005943cc: str      r3, [sp, #8]
005943d0: bl       #0x30e964
005943d4: add      r8, r7, r8
005943d8: mov      r5, r0
005943dc: ldrsh    r0, [r8, #2]
005943e0: bl       #0x30e964
005943e4: mov      fp, r0
005943e8: ldrsh    r0, [r8, #4]
005943ec: bl       #0x30e964
005943f0: ldrh     r2, [r4, #2]
005943f4: ldr      r3, [sp, #8]
005943f8: mov      sb, r0
005943fc: mul      r2, r3, r2
00594400: ldrsh    r0, [r7, r2]
00594404: add      r2, r7, r2
00594408: str      r2, [sp, #0xc]
0059440c: bl       #0x30e964
00594410: ldr      r2, [sp, #0xc]
00594414: mov      sl, r0
00594418: ldrsh    r0, [r2, #2]
0059441c: bl       #0x30e964
00594420: ldr      r2, [sp, #0xc]
00594424: mov      r8, r0
00594428: ldrsh    r0, [r2, #4]
0059442c: bl       #0x30e964
00594430: ldrh     r1, [r4]
00594434: ldr      r3, [sp, #8]
00594438: mov      r2, r0
0059443c: mul      r3, r3, r1
00594440: ldrsh    r0, [r7, r3]
00594444: add      r3, r7, r3
00594448: str      r2, [sp, #4]
0059444c: str      r3, [sp, #0xc]
00594450: bl       #0x30e964
00594454: ldr      r3, [sp, #0xc]
00594458: mov      ip, r0
0059445c: ldrsh    r0, [r3, #2]
00594460: str      ip, [sp, #8]
00594464: bl       #0x30e964
00594468: ldr      r3, [sp, #0xc]
0059446c: str      r0, [sp, #0x10]
00594470: ldrsh    r0, [r3, #4]
00594474: bl       #0x30e964
00594478: ldmib    r6, {r1, r3}
0059447c: str      r0, [sp, #0x40]
00594480: str      fp, [sp, #0x24]
00594484: str      sb, [sp, #0x28]
00594488: ldmib    sp, {r2, ip}
0059448c: cmp      r1, r3
00594490: str      r2, [sp, #0x34]
00594494: ldr      r2, [sp, #0x10]
00594498: str      sl, [sp, #0x2c]
0059449c: str      r8, [sp, #0x30]
005944a0: str      ip, [sp, #0x38]
005944a4: str      r2, [sp, #0x3c]
005944a8: str      r5, [sp, #0x20]
005944ac: beq      #0x5946f8
005944b0: str      r5, [r1]
005944b4: ldr      r3, [sp, #0x24]
005944b8: str      r3, [r1, #4]
005944bc: ldr      r3, [sp, #0x28]
005944c0: str      r3, [r1, #8]
005944c4: ldr      r3, [sp, #0x2c]
005944c8: str      r3, [r1, #0xc]
005944cc: ldr      r3, [sp, #0x30]
005944d0: str      r3, [r1, #0x10]
005944d4: ldr      r3, [sp, #0x34]
005944d8: str      r3, [r1, #0x14]
005944dc: ldr      r3, [sp, #0x38]
005944e0: str      r3, [r1, #0x18]
005944e4: ldr      r3, [sp, #0x3c]
005944e8: str      r3, [r1, #0x1c]
005944ec: ldr      r3, [sp, #0x40]
005944f0: str      r3, [r1, #0x20]
005944f4: ldr      r3, [r6, #4]
005944f8: add      r3, r3, #0x24
005944fc: str      r3, [r6, #4]
00594500: ldr      r3, [sp, #0x14]
00594504: add      r4, r4, #6
00594508: cmp      r3, r4
0059450c: bne      #0x5943b8
00594510: ldr      r5, [sp, #0x18]
00594514: b        #0x594350
00594518: ldr      r0, [r5]
0059451c: mov      r1, #1
00594520: bl       #0x5a1adc
00594524: ldr      r7, [r5, #4]
00594528: cmp      r4, #0
0059452c: add      r7, r0, r7
00594530: beq      #0x5949c0
00594534: add      r8, r4, r8, lsl #1
00594538: cmp      r4, r8
0059453c: str      r8, [sp, #0x14]
00594540: beq      #0x594350
00594544: add      r3, sp, #0x68
00594548: str      r3, [sp, #0x1c]
0059454c: str      r5, [sp, #0x18]
00594550: ldr      r2, [sp, #0x18]
00594554: ldrh     r8, [r4, #4]
00594558: ldrh     r3, [r2, #0xe]
0059455c: mul      r8, r8, r3
00594560: ldrsh    r0, [r7, r8]
00594564: str      r3, [sp, #8]
00594568: bl       #0x30e964
0059456c: add      r8, r7, r8
00594570: mov      r5, r0
00594574: ldrsh    r0, [r8, #2]
00594578: bl       #0x30e964
0059457c: mov      fp, r0
00594580: ldrsh    r0, [r8, #4]
00594584: bl       #0x30e964
00594588: ldrh     r2, [r4, #2]
0059458c: ldr      r3, [sp, #8]
00594590: mov      sb, r0
00594594: mul      r2, r3, r2
00594598: ldrsh    r0, [r7, r2]
0059459c: add      r2, r7, r2
005945a0: str      r2, [sp, #0xc]
005945a4: bl       #0x30e964
005945a8: ldr      r2, [sp, #0xc]
005945ac: mov      sl, r0
005945b0: ldrsh    r0, [r2, #2]
005945b4: bl       #0x30e964
005945b8: ldr      r2, [sp, #0xc]
005945bc: mov      r8, r0
005945c0: ldrsh    r0, [r2, #4]
005945c4: bl       #0x30e964
005945c8: ldrh     r1, [r4]
005945cc: ldr      r3, [sp, #8]
005945d0: mov      r2, r0
005945d4: mul      r3, r3, r1
005945d8: ldrsh    r0, [r7, r3]
005945dc: add      r3, r7, r3
005945e0: str      r2, [sp, #4]
005945e4: str      r3, [sp, #0xc]
005945e8: bl       #0x30e964
005945ec: ldr      r3, [sp, #0xc]
005945f0: mov      ip, r0
005945f4: ldrsh    r0, [r3, #2]
005945f8: str      ip, [sp, #8]
005945fc: bl       #0x30e964
00594600: ldr      r3, [sp, #0xc]
00594604: str      r0, [sp, #0x10]
00594608: ldrsh    r0, [r3, #4]
0059460c: bl       #0x30e964
00594610: ldmib    r6, {r1, r3}
00594614: str      r0, [sp, #0x88]
00594618: str      fp, [sp, #0x6c]
0059461c: str      sb, [sp, #0x70]
00594620: ldmib    sp, {r2, ip}
00594624: cmp      r1, r3
00594628: str      r2, [sp, #0x7c]
0059462c: ldr      r2, [sp, #0x10]
00594630: str      sl, [sp, #0x74]
00594634: str      r8, [sp, #0x78]
00594638: str      ip, [sp, #0x80]
0059463c: str      r2, [sp, #0x84]
00594640: str      r5, [sp, #0x68]
00594644: beq      #0x5946d8
00594648: str      r5, [r1]
0059464c: ldr      r3, [sp, #0x6c]
00594650: str      r3, [r1, #4]
00594654: ldr      r3, [sp, #0x70]
00594658: str      r3, [r1, #8]
0059465c: ldr      r3, [sp, #0x74]
00594660: str      r3, [r1, #0xc]
00594664: ldr      r3, [sp, #0x78]
00594668: str      r3, [r1, #0x10]
0059466c: ldr      r3, [sp, #0x7c]
00594670: str      r3, [r1, #0x14]
00594674: ldr      r3, [sp, #0x80]
00594678: str      r3, [r1, #0x18]
0059467c: ldr      r3, [sp, #0x84]
00594680: str      r3, [r1, #0x1c]
00594684: ldr      r3, [sp, #0x88]
00594688: str      r3, [r1, #0x20]
0059468c: ldr      r3, [r6, #4]
00594690: add      r3, r3, #0x24
00594694: str      r3, [r6, #4]
00594698: ldr      r3, [sp, #0x14]
0059469c: add      r4, r4, #6
005946a0: cmp      r3, r4
005946a4: bne      #0x594550
005946a8: b        #0x594510
005946ac: ldrb     r3, [r4, #0x12]
005946b0: tst      r3, #0x20
005946b4: bne      #0x5946c4
005946b8: mov      r3, #0
005946bc: strb     r3, [r4, #0x13]
005946c0: b        #0x5941ec
005946c4: ldr      r3, [r4]
005946c8: mov      r0, r4
005946cc: mov      lr, pc
005946d0: ldr      pc, [r3, #0x18]
005946d4: b        #0x5946b8
005946d8: mov      r0, r6
005946dc: ldr      r2, [sp, #0x1c]
005946e0: bl       #0x591238
005946e4: b        #0x594698
005946e8: mov      r0, r6
005946ec: ldr      r2, [sp, #0x14]
005946f0: bl       #0x591238
005946f4: b        #0x59433c
005946f8: mov      r0, r6
005946fc: ldr      r2, [sp, #0x1c]
00594700: bl       #0x591238
00594704: b        #0x594500
00594708: cmp      r8, #0
0059470c: beq      #0x594350
00594710: add      r2, sp, #0x44
00594714: ldrh     r3, [r5, #0xe]
00594718: str      r2, [sp, #0x1c]
0059471c: str      r8, [sp, #0x14]
00594720: str      r5, [sp, #0x18]
00594724: b        #0x594730
00594728: ldr      r2, [sp, #0x18]
0059472c: ldrh     r3, [r2, #0xe]
00594730: add      r8, r4, #2
00594734: mul      r8, r3, r8
00594738: ldrsh    r0, [r7, r8]
0059473c: str      r3, [sp, #8]
00594740: bl       #0x30e964
00594744: add      r8, r7, r8
00594748: mov      r5, r0
0059474c: ldrsh    r0, [r8, #2]
00594750: bl       #0x30e964
00594754: mov      fp, r0
00594758: ldrsh    r0, [r8, #4]
0059475c: bl       #0x30e964
00594760: ldr      r3, [sp, #8]
00594764: mov      sb, r0
00594768: mla      r2, r4, r3, r3
0059476c: ldrsh    r0, [r7, r2]
00594770: add      r2, r7, r2
00594774: str      r2, [sp, #0xc]
00594778: bl       #0x30e964
0059477c: ldr      r2, [sp, #0xc]
00594780: mov      sl, r0
00594784: ldrsh    r0, [r2, #2]
00594788: bl       #0x30e964
0059478c: ldr      r2, [sp, #0xc]
00594790: mov      r8, r0
00594794: ldrsh    r0, [r2, #4]
00594798: bl       #0x30e964
0059479c: ldr      r3, [sp, #8]
005947a0: mov      r2, r0
005947a4: mul      r3, r4, r3
005947a8: ldrsh    r0, [r7, r3]
005947ac: add      r3, r7, r3
005947b0: str      r2, [sp, #4]
005947b4: str      r3, [sp, #0xc]
005947b8: bl       #0x30e964
005947bc: ldr      r3, [sp, #0xc]
005947c0: mov      ip, r0
005947c4: ldrsh    r0, [r3, #2]
005947c8: str      ip, [sp, #8]
005947cc: bl       #0x30e964
005947d0: ldr      r3, [sp, #0xc]
005947d4: str      r0, [sp, #0x10]
005947d8: ldrsh    r0, [r3, #4]
005947dc: bl       #0x30e964
005947e0: ldmib    sp, {r2, ip}
005947e4: ldmib    r6, {r1, r3}
005947e8: str      r2, [sp, #0x58]
005947ec: str      fp, [sp, #0x48]
005947f0: str      sb, [sp, #0x4c]
005947f4: str      r0, [sp, #0x64]
005947f8: str      sl, [sp, #0x50]
005947fc: str      r8, [sp, #0x54]
00594800: str      ip, [sp, #0x5c]
00594804: ldr      r2, [sp, #0x10]
00594808: cmp      r1, r3
0059480c: str      r5, [sp, #0x44]
00594810: str      r2, [sp, #0x60]
00594814: beq      #0x594b38
00594818: str      r5, [r1]
0059481c: ldr      r3, [sp, #0x48]
00594820: str      r3, [r1, #4]
00594824: ldr      r3, [sp, #0x4c]
00594828: str      r3, [r1, #8]
0059482c: ldr      r3, [sp, #0x50]
00594830: str      r3, [r1, #0xc]
00594834: ldr      r3, [sp, #0x54]
00594838: str      r3, [r1, #0x10]
0059483c: ldr      r3, [sp, #0x58]
00594840: str      r3, [r1, #0x14]
00594844: ldr      r3, [sp, #0x5c]
00594848: str      r3, [r1, #0x18]
0059484c: ldr      r3, [sp, #0x60]
00594850: str      r3, [r1, #0x1c]
00594854: ldr      r3, [sp, #0x64]
00594858: str      r3, [r1, #0x20]
0059485c: ldr      r3, [r6, #4]
00594860: add      r3, r3, #0x24
00594864: str      r3, [r6, #4]
00594868: ldr      r3, [sp, #0x14]
0059486c: add      r4, r4, #3
00594870: cmp      r3, r4
00594874: bhi      #0x594728
00594878: ldr      r5, [sp, #0x18]
0059487c: b        #0x594350
00594880: cmp      r8, #0
00594884: beq      #0x594350
00594888: add      r2, sp, #0xd4
0059488c: ldrh     r3, [r5, #0xe]
00594890: mov      sl, #0
00594894: str      r2, [sp, #0x14]
00594898: str      r8, [sp, #0xc]
0059489c: str      r5, [sp, #0x10]
005948a0: b        #0x5948ac
005948a4: ldr      r2, [sp, #0x10]
005948a8: ldrh     r3, [r2, #0xe]
005948ac: add      r8, r4, #2
005948b0: mul      r8, r3, r8
005948b4: ldrsh    r0, [r7, r8]
005948b8: str      r3, [sp, #8]
005948bc: bl       #0x30e964
005948c0: add      r8, r7, r8
005948c4: mov      r5, r0
005948c8: ldrsh    r0, [r8, #2]
005948cc: bl       #0x30e964
005948d0: ldr      r3, [sp, #8]
005948d4: mov      fp, r0
005948d8: mla      r8, r4, r3, r3
005948dc: ldrsh    r0, [r7, r8]
005948e0: bl       #0x30e964
005948e4: add      r8, r7, r8
005948e8: mov      sb, r0
005948ec: ldrsh    r0, [r8, #2]
005948f0: bl       #0x30e964
005948f4: ldr      r3, [sp, #8]
005948f8: mov      r8, r0
005948fc: mul      r3, r4, r3
00594900: ldrsh    r0, [r7, r3]
00594904: add      r3, r7, r3
00594908: str      r3, [sp, #8]
0059490c: bl       #0x30e964
00594910: ldr      r3, [sp, #8]
00594914: mov      r2, r0
00594918: ldrsh    r0, [r3, #2]
0059491c: str      r2, [sp, #4]
00594920: bl       #0x30e964
00594924: ldmib    r6, {r1, r3}
00594928: ldr      r2, [sp, #4]
0059492c: str      fp, [sp, #0xd8]
00594930: cmp      r1, r3
00594934: str      sb, [sp, #0xe0]
00594938: str      r8, [sp, #0xe4]
0059493c: str      r0, [sp, #0xf0]
00594940: str      r2, [sp, #0xec]
00594944: str      sl, [sp, #0xdc]
00594948: str      sl, [sp, #0xe8]
0059494c: str      r5, [sp, #0xd4]
00594950: str      sl, [sp, #0xf4]
00594954: beq      #0x594b48
00594958: str      r5, [r1]
0059495c: ldr      r3, [sp, #0xd8]
00594960: str      r3, [r1, #4]
00594964: ldr      r3, [sp, #0xdc]
00594968: str      r3, [r1, #8]
0059496c: ldr      r3, [sp, #0xe0]
00594970: str      r3, [r1, #0xc]
00594974: ldr      r3, [sp, #0xe4]
00594978: str      r3, [r1, #0x10]
0059497c: ldr      r3, [sp, #0xe8]
00594980: str      r3, [r1, #0x14]
00594984: ldr      r3, [sp, #0xec]
00594988: str      r3, [r1, #0x18]
0059498c: ldr      r3, [sp, #0xf0]
00594990: str      r3, [r1, #0x1c]
00594994: ldr      r3, [sp, #0xf4]
00594998: str      r3, [r1, #0x20]
0059499c: ldr      r3, [r6, #4]
005949a0: add      r3, r3, #0x24
005949a4: str      r3, [r6, #4]
005949a8: ldr      r3, [sp, #0xc]
005949ac: add      r4, r4, #3
005949b0: cmp      r3, r4
005949b4: bhi      #0x5948a4
005949b8: ldr      r5, [sp, #0x10]
005949bc: b        #0x594350
005949c0: cmp      r8, #0
005949c4: beq      #0x594350
005949c8: add      r2, sp, #0x8c
005949cc: ldrh     r3, [r5, #0xe]
005949d0: str      r2, [sp, #0x1c]
005949d4: str      r8, [sp, #0x14]
005949d8: str      r5, [sp, #0x18]
005949dc: b        #0x5949e8
005949e0: ldr      r2, [sp, #0x18]
005949e4: ldrh     r3, [r2, #0xe]
005949e8: add      r8, r4, #2
005949ec: mul      r8, r3, r8
005949f0: ldrsh    r0, [r7, r8]
005949f4: str      r3, [sp, #8]
005949f8: bl       #0x30e964
005949fc: add      r8, r7, r8
00594a00: mov      r5, r0
00594a04: ldrsh    r0, [r8, #2]
00594a08: bl       #0x30e964
00594a0c: mov      fp, r0
00594a10: ldrsh    r0, [r8, #4]
00594a14: bl       #0x30e964
00594a18: ldr      r3, [sp, #8]
00594a1c: mov      sb, r0
00594a20: mla      r2, r4, r3, r3
00594a24: ldrsh    r0, [r7, r2]
00594a28: add      r2, r7, r2
00594a2c: str      r2, [sp, #0xc]
00594a30: bl       #0x30e964
00594a34: ldr      r2, [sp, #0xc]
00594a38: mov      sl, r0
00594a3c: ldrsh    r0, [r2, #2]
00594a40: bl       #0x30e964
00594a44: ldr      r2, [sp, #0xc]
00594a48: mov      r8, r0
00594a4c: ldrsh    r0, [r2, #4]
00594a50: bl       #0x30e964
00594a54: ldr      r3, [sp, #8]
00594a58: mov      r2, r0
00594a5c: mul      r3, r4, r3
00594a60: ldrsh    r0, [r7, r3]
00594a64: add      r3, r7, r3
00594a68: str      r2, [sp, #4]
00594a6c: str      r3, [sp, #0xc]
00594a70: bl       #0x30e964
00594a74: ldr      r3, [sp, #0xc]
00594a78: mov      ip, r0
00594a7c: ldrsh    r0, [r3, #2]
00594a80: str      ip, [sp, #8]
00594a84: bl       #0x30e964
00594a88: ldr      r3, [sp, #0xc]
00594a8c: str      r0, [sp, #0x10]
00594a90: ldrsh    r0, [r3, #4]
00594a94: bl       #0x30e964
00594a98: ldmib    sp, {r2, ip}
00594a9c: ldmib    r6, {r1, r3}
00594aa0: str      r2, [sp, #0xa0]
00594aa4: str      fp, [sp, #0x90]
00594aa8: str      sb, [sp, #0x94]
00594aac: str      r0, [sp, #0xac]
00594ab0: str      sl, [sp, #0x98]
00594ab4: str      r8, [sp, #0x9c]
00594ab8: str      ip, [sp, #0xa4]
00594abc: ldr      r2, [sp, #0x10]
00594ac0: cmp      r1, r3
00594ac4: str      r5, [sp, #0x8c]
00594ac8: str      r2, [sp, #0xa8]
00594acc: beq      #0x594b58
00594ad0: str      r5, [r1]
00594ad4: ldr      r3, [sp, #0x90]
00594ad8: str      r3, [r1, #4]
00594adc: ldr      r3, [sp, #0x94]
00594ae0: str      r3, [r1, #8]
00594ae4: ldr      r3, [sp, #0x98]
00594ae8: str      r3, [r1, #0xc]
00594aec: ldr      r3, [sp, #0x9c]
00594af0: str      r3, [r1, #0x10]
00594af4: ldr      r3, [sp, #0xa0]
00594af8: str      r3, [r1, #0x14]
00594afc: ldr      r3, [sp, #0xa4]
00594b00: str      r3, [r1, #0x18]
00594b04: ldr      r3, [sp, #0xa8]
00594b08: str      r3, [r1, #0x1c]
00594b0c: ldr      r3, [sp, #0xac]
00594b10: str      r3, [r1, #0x20]
00594b14: ldr      r3, [r6, #4]
00594b18: add      r3, r3, #0x24
00594b1c: str      r3, [r6, #4]
00594b20: ldr      r3, [sp, #0x14]
00594b24: add      r4, r4, #3
00594b28: cmp      r3, r4
00594b2c: bhi      #0x5949e0
00594b30: ldr      r5, [sp, #0x18]
00594b34: b        #0x594350
00594b38: mov      r0, r6
00594b3c: ldr      r2, [sp, #0x1c]
00594b40: bl       #0x591238
00594b44: b        #0x594868
00594b48: mov      r0, r6
00594b4c: ldr      r2, [sp, #0x14]
00594b50: bl       #0x591238
00594b54: b        #0x5949a8
00594b58: mov      r0, r6
00594b5c: ldr      r2, [sp, #0x1c]
00594b60: bl       #0x591238
00594b64: b        #0x594b20

# _ZN6glitch5scene12_GLOBAL__N_115createTrianglesItSt6vectorINS_4core10triangle3dIfEENS4_10SAllocatorIS6_LNS_6memory13E_MEMORY_HINTE0EEEEEEvPKtjRKNS_5video13SVertexStreamERT0_
00594b68: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00594b6c: mov      r5, r2
00594b70: ldrh     r2, [r2, #0xc]
00594b74: sub      sp, sp, #0xfc
00594b78: mov      r4, r0
00594b7c: cmp      r2, #3
00594b80: mov      r8, r1
00594b84: mov      r6, r3
00594b88: beq      #0x594ec8
00594b8c: cmp      r2, #4
00594b90: beq      #0x594d30
00594b94: cmp      r2, #2
00594b98: beq      #0x594ba4
00594b9c: add      sp, sp, #0xfc
00594ba0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00594ba4: ldr      r0, [r5]
00594ba8: mov      r1, #1
00594bac: bl       #0x5a1adc
00594bb0: ldr      r7, [r5, #4]
00594bb4: cmp      r4, #0
00594bb8: add      r7, r0, r7
00594bbc: beq      #0x595230
00594bc0: add      r8, r4, r8, lsl #1
00594bc4: cmp      r4, r8
00594bc8: str      r8, [sp, #0xc]
00594bcc: beq      #0x594d00
00594bd0: add      r2, sp, #0xb0
00594bd4: mov      r8, #0
00594bd8: str      r2, [sp, #0x14]
00594bdc: str      r5, [sp, #0x10]
00594be0: ldr      r2, [sp, #0x10]
00594be4: ldrh     sl, [r4, #4]
00594be8: ldrh     r3, [r2, #0xe]
00594bec: mul      sl, sl, r3
00594bf0: ldrh     r0, [r7, sl]
00594bf4: str      r3, [sp, #8]
00594bf8: bl       #0x30e2e0
00594bfc: add      sl, r7, sl
00594c00: mov      r5, r0
00594c04: ldrh     r0, [sl, #2]
00594c08: bl       #0x30e2e0
00594c0c: ldrh     sl, [r4, #2]
00594c10: ldr      r3, [sp, #8]
00594c14: mov      fp, r0
00594c18: mul      sl, r3, sl
00594c1c: ldrh     r0, [r7, sl]
00594c20: bl       #0x30e2e0
00594c24: add      sl, r7, sl
00594c28: mov      sb, r0
00594c2c: ldrh     r0, [sl, #2]
00594c30: bl       #0x30e2e0
00594c34: ldrh     r2, [r4]
00594c38: ldr      r3, [sp, #8]
00594c3c: mov      sl, r0
00594c40: mul      r3, r3, r2
00594c44: ldrh     r0, [r7, r3]
00594c48: add      r3, r7, r3
00594c4c: str      r3, [sp, #8]
00594c50: bl       #0x30e2e0
00594c54: ldr      r3, [sp, #8]
00594c58: mov      r2, r0
00594c5c: ldrh     r0, [r3, #2]
00594c60: str      r2, [sp, #4]
00594c64: bl       #0x30e2e0
00594c68: ldmib    r6, {r1, r3}
00594c6c: ldr      r2, [sp, #4]
00594c70: str      fp, [sp, #0xb4]
00594c74: cmp      r1, r3
00594c78: str      sb, [sp, #0xbc]
00594c7c: str      r0, [sp, #0xcc]
00594c80: str      sl, [sp, #0xc0]
00594c84: str      r2, [sp, #0xc8]
00594c88: str      r8, [sp, #0xb8]
00594c8c: str      r5, [sp, #0xb0]
00594c90: str      r8, [sp, #0xc4]
00594c94: str      r8, [sp, #0xd0]
00594c98: beq      #0x595098
00594c9c: str      r5, [r1]
00594ca0: ldr      r3, [sp, #0xb4]
00594ca4: str      r3, [r1, #4]
00594ca8: ldr      r3, [sp, #0xb8]
00594cac: str      r3, [r1, #8]
00594cb0: ldr      r3, [sp, #0xbc]
00594cb4: str      r3, [r1, #0xc]
00594cb8: ldr      r3, [sp, #0xc0]
00594cbc: str      r3, [r1, #0x10]
00594cc0: ldr      r3, [sp, #0xc4]
00594cc4: str      r3, [r1, #0x14]
00594cc8: ldr      r3, [sp, #0xc8]
00594ccc: str      r3, [r1, #0x18]
00594cd0: ldr      r3, [sp, #0xcc]
00594cd4: str      r3, [r1, #0x1c]
00594cd8: ldr      r3, [sp, #0xd0]
00594cdc: str      r3, [r1, #0x20]
00594ce0: ldr      r3, [r6, #4]
00594ce4: add      r3, r3, #0x24
00594ce8: str      r3, [r6, #4]
00594cec: ldr      r3, [sp, #0xc]
00594cf0: add      r4, r4, #6
00594cf4: cmp      r3, r4
00594cf8: bne      #0x594be0
00594cfc: ldr      r5, [sp, #0x10]
00594d00: cmp      r7, #0
00594d04: beq      #0x594b9c
00594d08: ldr      r4, [r5]
00594d0c: ldrb     r3, [r4, #0x13]
00594d10: and      r2, r3, #0x1f
00594d14: cmp      r2, #1
00594d18: bls      #0x59505c
00594d1c: sub      r2, r2, #1
00594d20: bic      r3, r3, #0x1f
00594d24: orr      r3, r2, r3
00594d28: strb     r3, [r4, #0x13]
00594d2c: b        #0x594b9c
00594d30: ldr      r0, [r5]
00594d34: mov      r1, #1
00594d38: bl       #0x5a1adc
00594d3c: ldr      r7, [r5, #4]
00594d40: cmp      r4, #0
00594d44: add      r7, r0, r7
00594d48: beq      #0x5950b8
00594d4c: add      r8, r4, r8, lsl #1
00594d50: cmp      r4, r8
00594d54: str      r8, [sp, #0x14]
00594d58: beq      #0x594d00
00594d5c: add      r3, sp, #0x20
00594d60: str      r3, [sp, #0x1c]
00594d64: str      r5, [sp, #0x18]
00594d68: ldr      r2, [sp, #0x18]
00594d6c: ldrh     r8, [r4, #4]
00594d70: ldrh     r3, [r2, #0xe]
00594d74: mul      r8, r8, r3
00594d78: ldrh     r0, [r7, r8]
00594d7c: str      r3, [sp, #8]
00594d80: bl       #0x30e2e0
00594d84: add      r8, r7, r8
00594d88: mov      r5, r0
00594d8c: ldrh     r0, [r8, #2]
00594d90: bl       #0x30e2e0
00594d94: mov      fp, r0
00594d98: ldrh     r0, [r8, #4]
00594d9c: bl       #0x30e2e0
00594da0: ldrh     r2, [r4, #2]
00594da4: ldr      r3, [sp, #8]
00594da8: mov      sb, r0
00594dac: mul      r2, r3, r2
00594db0: ldrh     r0, [r7, r2]
00594db4: add      r2, r7, r2
00594db8: str      r2, [sp, #0xc]
00594dbc: bl       #0x30e2e0
00594dc0: ldr      r2, [sp, #0xc]
00594dc4: mov      sl, r0
00594dc8: ldrh     r0, [r2, #2]
00594dcc: bl       #0x30e2e0
00594dd0: ldr      r2, [sp, #0xc]
00594dd4: mov      r8, r0
00594dd8: ldrh     r0, [r2, #4]
00594ddc: bl       #0x30e2e0
00594de0: ldrh     r1, [r4]
00594de4: ldr      r3, [sp, #8]
00594de8: mov      r2, r0
00594dec: mul      r3, r3, r1
00594df0: ldrh     r0, [r7, r3]
00594df4: add      r3, r7, r3
00594df8: str      r2, [sp, #4]
00594dfc: str      r3, [sp, #0xc]
00594e00: bl       #0x30e2e0
00594e04: ldr      r3, [sp, #0xc]
00594e08: mov      ip, r0
00594e0c: ldrh     r0, [r3, #2]
00594e10: str      ip, [sp, #8]
00594e14: bl       #0x30e2e0
00594e18: ldr      r3, [sp, #0xc]
00594e1c: str      r0, [sp, #0x10]
00594e20: ldrh     r0, [r3, #4]
00594e24: bl       #0x30e2e0
00594e28: ldmib    r6, {r1, r3}
00594e2c: str      r0, [sp, #0x40]
00594e30: str      fp, [sp, #0x24]
00594e34: str      sb, [sp, #0x28]
00594e38: ldmib    sp, {r2, ip}
00594e3c: cmp      r1, r3
00594e40: str      r2, [sp, #0x34]
00594e44: ldr      r2, [sp, #0x10]
00594e48: str      sl, [sp, #0x2c]
00594e4c: str      r8, [sp, #0x30]
00594e50: str      ip, [sp, #0x38]
00594e54: str      r2, [sp, #0x3c]
00594e58: str      r5, [sp, #0x20]
00594e5c: beq      #0x5950a8
00594e60: str      r5, [r1]
00594e64: ldr      r3, [sp, #0x24]
00594e68: str      r3, [r1, #4]
00594e6c: ldr      r3, [sp, #0x28]
00594e70: str      r3, [r1, #8]
00594e74: ldr      r3, [sp, #0x2c]
00594e78: str      r3, [r1, #0xc]
00594e7c: ldr      r3, [sp, #0x30]
00594e80: str      r3, [r1, #0x10]
00594e84: ldr      r3, [sp, #0x34]
00594e88: str      r3, [r1, #0x14]
00594e8c: ldr      r3, [sp, #0x38]
00594e90: str      r3, [r1, #0x18]
00594e94: ldr      r3, [sp, #0x3c]
00594e98: str      r3, [r1, #0x1c]
00594e9c: ldr      r3, [sp, #0x40]
00594ea0: str      r3, [r1, #0x20]
00594ea4: ldr      r3, [r6, #4]
00594ea8: add      r3, r3, #0x24
00594eac: str      r3, [r6, #4]
00594eb0: ldr      r3, [sp, #0x14]
00594eb4: add      r4, r4, #6
00594eb8: cmp      r3, r4
00594ebc: bne      #0x594d68
00594ec0: ldr      r5, [sp, #0x18]
00594ec4: b        #0x594d00
00594ec8: ldr      r0, [r5]
00594ecc: mov      r1, #1
00594ed0: bl       #0x5a1adc
00594ed4: ldr      r7, [r5, #4]
00594ed8: cmp      r4, #0
00594edc: add      r7, r0, r7
00594ee0: beq      #0x595370
00594ee4: add      r8, r4, r8, lsl #1
00594ee8: cmp      r4, r8
00594eec: str      r8, [sp, #0x14]
00594ef0: beq      #0x594d00
00594ef4: add      r3, sp, #0x68
00594ef8: str      r3, [sp, #0x1c]
00594efc: str      r5, [sp, #0x18]
00594f00: ldr      r2, [sp, #0x18]
00594f04: ldrh     r8, [r4, #4]
00594f08: ldrh     r3, [r2, #0xe]
00594f0c: mul      r8, r8, r3
00594f10: ldrh     r0, [r7, r8]
00594f14: str      r3, [sp, #8]
00594f18: bl       #0x30e2e0
00594f1c: add      r8, r7, r8
00594f20: mov      r5, r0
00594f24: ldrh     r0, [r8, #2]
00594f28: bl       #0x30e2e0
00594f2c: mov      fp, r0
00594f30: ldrh     r0, [r8, #4]
00594f34: bl       #0x30e2e0
00594f38: ldrh     r2, [r4, #2]
00594f3c: ldr      r3, [sp, #8]
00594f40: mov      sb, r0
00594f44: mul      r2, r3, r2
00594f48: ldrh     r0, [r7, r2]
00594f4c: add      r2, r7, r2
00594f50: str      r2, [sp, #0xc]
00594f54: bl       #0x30e2e0
00594f58: ldr      r2, [sp, #0xc]
00594f5c: mov      sl, r0
00594f60: ldrh     r0, [r2, #2]
00594f64: bl       #0x30e2e0
00594f68: ldr      r2, [sp, #0xc]
00594f6c: mov      r8, r0
00594f70: ldrh     r0, [r2, #4]
00594f74: bl       #0x30e2e0
00594f78: ldrh     r1, [r4]
00594f7c: ldr      r3, [sp, #8]
00594f80: mov      r2, r0
00594f84: mul      r3, r3, r1
00594f88: ldrh     r0, [r7, r3]
00594f8c: add      r3, r7, r3
00594f90: str      r2, [sp, #4]
00594f94: str      r3, [sp, #0xc]
00594f98: bl       #0x30e2e0
00594f9c: ldr      r3, [sp, #0xc]
00594fa0: mov      ip, r0
00594fa4: ldrh     r0, [r3, #2]
00594fa8: str      ip, [sp, #8]
00594fac: bl       #0x30e2e0
00594fb0: ldr      r3, [sp, #0xc]
00594fb4: str      r0, [sp, #0x10]
00594fb8: ldrh     r0, [r3, #4]
00594fbc: bl       #0x30e2e0
00594fc0: ldmib    r6, {r1, r3}
00594fc4: str      r0, [sp, #0x88]
00594fc8: str      fp, [sp, #0x6c]
00594fcc: str      sb, [sp, #0x70]
00594fd0: ldmib    sp, {r2, ip}
00594fd4: cmp      r1, r3
00594fd8: str      r2, [sp, #0x7c]
00594fdc: ldr      r2, [sp, #0x10]
00594fe0: str      sl, [sp, #0x74]
00594fe4: str      r8, [sp, #0x78]
00594fe8: str      ip, [sp, #0x80]
00594fec: str      r2, [sp, #0x84]
00594ff0: str      r5, [sp, #0x68]
00594ff4: beq      #0x595088
00594ff8: str      r5, [r1]
00594ffc: ldr      r3, [sp, #0x6c]
00595000: str      r3, [r1, #4]
00595004: ldr      r3, [sp, #0x70]
00595008: str      r3, [r1, #8]
0059500c: ldr      r3, [sp, #0x74]
00595010: str      r3, [r1, #0xc]
00595014: ldr      r3, [sp, #0x78]
00595018: str      r3, [r1, #0x10]
0059501c: ldr      r3, [sp, #0x7c]
00595020: str      r3, [r1, #0x14]
00595024: ldr      r3, [sp, #0x80]
00595028: str      r3, [r1, #0x18]
0059502c: ldr      r3, [sp, #0x84]
00595030: str      r3, [r1, #0x1c]
00595034: ldr      r3, [sp, #0x88]
00595038: str      r3, [r1, #0x20]
0059503c: ldr      r3, [r6, #4]
00595040: add      r3, r3, #0x24
00595044: str      r3, [r6, #4]
00595048: ldr      r3, [sp, #0x14]
0059504c: add      r4, r4, #6
00595050: cmp      r3, r4
00595054: bne      #0x594f00
00595058: b        #0x594ec0
0059505c: ldrb     r3, [r4, #0x12]
00595060: tst      r3, #0x20
00595064: bne      #0x595074
00595068: mov      r3, #0
0059506c: strb     r3, [r4, #0x13]
00595070: b        #0x594b9c
00595074: ldr      r3, [r4]
00595078: mov      r0, r4
0059507c: mov      lr, pc
00595080: ldr      pc, [r3, #0x18]
00595084: b        #0x595068
00595088: mov      r0, r6
0059508c: ldr      r2, [sp, #0x1c]
00595090: bl       #0x591238
00595094: b        #0x595048
00595098: mov      r0, r6
0059509c: ldr      r2, [sp, #0x14]
005950a0: bl       #0x591238
005950a4: b        #0x594cec
005950a8: mov      r0, r6
005950ac: ldr      r2, [sp, #0x1c]
005950b0: bl       #0x591238
005950b4: b        #0x594eb0
005950b8: cmp      r8, #0
005950bc: beq      #0x594d00
005950c0: add      r2, sp, #0x44
005950c4: ldrh     r3, [r5, #0xe]
005950c8: str      r2, [sp, #0x1c]
005950cc: str      r8, [sp, #0x14]
005950d0: str      r5, [sp, #0x18]
005950d4: b        #0x5950e0
005950d8: ldr      r2, [sp, #0x18]
005950dc: ldrh     r3, [r2, #0xe]
005950e0: add      r8, r4, #2
005950e4: mul      r8, r3, r8
005950e8: ldrh     r0, [r7, r8]
005950ec: str      r3, [sp, #8]
005950f0: bl       #0x30e2e0
005950f4: add      r8, r7, r8
005950f8: mov      r5, r0
005950fc: ldrh     r0, [r8, #2]
00595100: bl       #0x30e2e0
00595104: mov      fp, r0
00595108: ldrh     r0, [r8, #4]
0059510c: bl       #0x30e2e0
00595110: ldr      r3, [sp, #8]
00595114: mov      sb, r0
00595118: mla      r2, r4, r3, r3
0059511c: ldrh     r0, [r7, r2]
00595120: add      r2, r7, r2
00595124: str      r2, [sp, #0xc]
00595128: bl       #0x30e2e0
0059512c: ldr      r2, [sp, #0xc]
00595130: mov      sl, r0
00595134: ldrh     r0, [r2, #2]
00595138: bl       #0x30e2e0
0059513c: ldr      r2, [sp, #0xc]
00595140: mov      r8, r0
00595144: ldrh     r0, [r2, #4]
00595148: bl       #0x30e2e0
0059514c: ldr      r3, [sp, #8]
00595150: mov      r2, r0
00595154: mul      r3, r4, r3
00595158: ldrh     r0, [r7, r3]
0059515c: add      r3, r7, r3
00595160: str      r2, [sp, #4]
00595164: str      r3, [sp, #0xc]
00595168: bl       #0x30e2e0
0059516c: ldr      r3, [sp, #0xc]
00595170: mov      ip, r0
00595174: ldrh     r0, [r3, #2]
00595178: str      ip, [sp, #8]
0059517c: bl       #0x30e2e0
00595180: ldr      r3, [sp, #0xc]
00595184: str      r0, [sp, #0x10]
00595188: ldrh     r0, [r3, #4]
0059518c: bl       #0x30e2e0
00595190: ldmib    sp, {r2, ip}
00595194: ldmib    r6, {r1, r3}
00595198: str      r2, [sp, #0x58]
0059519c: str      fp, [sp, #0x48]
005951a0: str      sb, [sp, #0x4c]
005951a4: str      r0, [sp, #0x64]
005951a8: str      sl, [sp, #0x50]
005951ac: str      r8, [sp, #0x54]
005951b0: str      ip, [sp, #0x5c]
005951b4: ldr      r2, [sp, #0x10]
005951b8: cmp      r1, r3
005951bc: str      r5, [sp, #0x44]
005951c0: str      r2, [sp, #0x60]
005951c4: beq      #0x5954e8
005951c8: str      r5, [r1]
005951cc: ldr      r3, [sp, #0x48]
005951d0: str      r3, [r1, #4]
005951d4: ldr      r3, [sp, #0x4c]
005951d8: str      r3, [r1, #8]
005951dc: ldr      r3, [sp, #0x50]
005951e0: str      r3, [r1, #0xc]
005951e4: ldr      r3, [sp, #0x54]
005951e8: str      r3, [r1, #0x10]
005951ec: ldr      r3, [sp, #0x58]
005951f0: str      r3, [r1, #0x14]
005951f4: ldr      r3, [sp, #0x5c]
005951f8: str      r3, [r1, #0x18]
005951fc: ldr      r3, [sp, #0x60]
00595200: str      r3, [r1, #0x1c]
00595204: ldr      r3, [sp, #0x64]
00595208: str      r3, [r1, #0x20]
0059520c: ldr      r3, [r6, #4]
00595210: add      r3, r3, #0x24
00595214: str      r3, [r6, #4]
00595218: ldr      r3, [sp, #0x14]
0059521c: add      r4, r4, #3
00595220: cmp      r3, r4
00595224: bhi      #0x5950d8
00595228: ldr      r5, [sp, #0x18]
0059522c: b        #0x594d00
00595230: cmp      r8, #0
00595234: beq      #0x594d00
00595238: add      r2, sp, #0xd4
0059523c: ldrh     r3, [r5, #0xe]
00595240: mov      sl, #0
00595244: str      r2, [sp, #0x14]
00595248: str      r8, [sp, #0xc]
0059524c: str      r5, [sp, #0x10]
00595250: b        #0x59525c
00595254: ldr      r2, [sp, #0x10]
00595258: ldrh     r3, [r2, #0xe]
0059525c: add      r8, r4, #2
00595260: mul      r8, r3, r8
00595264: ldrh     r0, [r7, r8]
00595268: str      r3, [sp, #8]
0059526c: bl       #0x30e2e0
00595270: add      r8, r7, r8
00595274: mov      r5, r0
00595278: ldrh     r0, [r8, #2]
0059527c: bl       #0x30e2e0
00595280: ldr      r3, [sp, #8]
00595284: mov      fp, r0
00595288: mla      r8, r4, r3, r3
0059528c: ldrh     r0, [r7, r8]
00595290: bl       #0x30e2e0
00595294: add      r8, r7, r8
00595298: mov      sb, r0
0059529c: ldrh     r0, [r8, #2]
005952a0: bl       #0x30e2e0
005952a4: ldr      r3, [sp, #8]
005952a8: mov      r8, r0
005952ac: mul      r3, r4, r3
005952b0: ldrh     r0, [r7, r3]
005952b4: add      r3, r7, r3
005952b8: str      r3, [sp, #8]
005952bc: bl       #0x30e2e0
005952c0: ldr      r3, [sp, #8]
005952c4: mov      r2, r0
005952c8: ldrh     r0, [r3, #2]
005952cc: str      r2, [sp, #4]
005952d0: bl       #0x30e2e0
005952d4: ldmib    r6, {r1, r3}
005952d8: ldr      r2, [sp, #4]
005952dc: str      fp, [sp, #0xd8]
005952e0: cmp      r1, r3
005952e4: str      sb, [sp, #0xe0]
005952e8: str      r8, [sp, #0xe4]
005952ec: str      r0, [sp, #0xf0]
005952f0: str      r2, [sp, #0xec]
005952f4: str      sl, [sp, #0xdc]
005952f8: str      sl, [sp, #0xe8]
005952fc: str      r5, [sp, #0xd4]
00595300: str      sl, [sp, #0xf4]
00595304: beq      #0x5954f8
00595308: str      r5, [r1]
0059530c: ldr      r3, [sp, #0xd8]
00595310: str      r3, [r1, #4]
00595314: ldr      r3, [sp, #0xdc]
00595318: str      r3, [r1, #8]
0059531c: ldr      r3, [sp, #0xe0]
00595320: str      r3, [r1, #0xc]
00595324: ldr      r3, [sp, #0xe4]
00595328: str      r3, [r1, #0x10]
0059532c: ldr      r3, [sp, #0xe8]
00595330: str      r3, [r1, #0x14]
00595334: ldr      r3, [sp, #0xec]
00595338: str      r3, [r1, #0x18]
0059533c: ldr      r3, [sp, #0xf0]
00595340: str      r3, [r1, #0x1c]
00595344: ldr      r3, [sp, #0xf4]
00595348: str      r3, [r1, #0x20]
0059534c: ldr      r3, [r6, #4]
00595350: add      r3, r3, #0x24
00595354: str      r3, [r6, #4]
00595358: ldr      r3, [sp, #0xc]
0059535c: add      r4, r4, #3
00595360: cmp      r3, r4
00595364: bhi      #0x595254
00595368: ldr      r5, [sp, #0x10]
0059536c: b        #0x594d00
00595370: cmp      r8, #0
00595374: beq      #0x594d00
00595378: add      r2, sp, #0x8c
0059537c: ldrh     r3, [r5, #0xe]
00595380: str      r2, [sp, #0x1c]
00595384: str      r8, [sp, #0x14]
00595388: str      r5, [sp, #0x18]
0059538c: b        #0x595398
00595390: ldr      r2, [sp, #0x18]
00595394: ldrh     r3, [r2, #0xe]
00595398: add      r8, r4, #2
0059539c: mul      r8, r3, r8
005953a0: ldrh     r0, [r7, r8]
005953a4: str      r3, [sp, #8]
005953a8: bl       #0x30e2e0
005953ac: add      r8, r7, r8
005953b0: mov      r5, r0
005953b4: ldrh     r0, [r8, #2]
005953b8: bl       #0x30e2e0
005953bc: mov      fp, r0
005953c0: ldrh     r0, [r8, #4]
005953c4: bl       #0x30e2e0
005953c8: ldr      r3, [sp, #8]
005953cc: mov      sb, r0
005953d0: mla      r2, r4, r3, r3
005953d4: ldrh     r0, [r7, r2]
005953d8: add      r2, r7, r2
005953dc: str      r2, [sp, #0xc]
005953e0: bl       #0x30e2e0
005953e4: ldr      r2, [sp, #0xc]
005953e8: mov      sl, r0
005953ec: ldrh     r0, [r2, #2]
005953f0: bl       #0x30e2e0
005953f4: ldr      r2, [sp, #0xc]
005953f8: mov      r8, r0
005953fc: ldrh     r0, [r2, #4]
00595400: bl       #0x30e2e0
00595404: ldr      r3, [sp, #8]
00595408: mov      r2, r0
0059540c: mul      r3, r4, r3
00595410: ldrh     r0, [r7, r3]
00595414: add      r3, r7, r3
00595418: str      r2, [sp, #4]
0059541c: str      r3, [sp, #0xc]
00595420: bl       #0x30e2e0
00595424: ldr      r3, [sp, #0xc]
00595428: mov      ip, r0
0059542c: ldrh     r0, [r3, #2]
00595430: str      ip, [sp, #8]
00595434: bl       #0x30e2e0
00595438: ldr      r3, [sp, #0xc]
0059543c: str      r0, [sp, #0x10]
00595440: ldrh     r0, [r3, #4]
00595444: bl       #0x30e2e0
00595448: ldmib    sp, {r2, ip}
0059544c: ldmib    r6, {r1, r3}
00595450: str      r2, [sp, #0xa0]
00595454: str      fp, [sp, #0x90]
00595458: str      sb, [sp, #0x94]
0059545c: str      r0, [sp, #0xac]
00595460: str      sl, [sp, #0x98]
00595464: str      r8, [sp, #0x9c]
00595468: str      ip, [sp, #0xa4]
0059546c: ldr      r2, [sp, #0x10]
00595470: cmp      r1, r3
00595474: str      r5, [sp, #0x8c]
00595478: str      r2, [sp, #0xa8]
0059547c: beq      #0x595508
00595480: str      r5, [r1]
00595484: ldr      r3, [sp, #0x90]
00595488: str      r3, [r1, #4]
0059548c: ldr      r3, [sp, #0x94]
00595490: str      r3, [r1, #8]
00595494: ldr      r3, [sp, #0x98]
00595498: str      r3, [r1, #0xc]
0059549c: ldr      r3, [sp, #0x9c]
005954a0: str      r3, [r1, #0x10]
005954a4: ldr      r3, [sp, #0xa0]
005954a8: str      r3, [r1, #0x14]
005954ac: ldr      r3, [sp, #0xa4]
005954b0: str      r3, [r1, #0x18]
005954b4: ldr      r3, [sp, #0xa8]
005954b8: str      r3, [r1, #0x1c]
005954bc: ldr      r3, [sp, #0xac]
005954c0: str      r3, [r1, #0x20]
005954c4: ldr      r3, [r6, #4]
005954c8: add      r3, r3, #0x24
005954cc: str      r3, [r6, #4]
005954d0: ldr      r3, [sp, #0x14]
005954d4: add      r4, r4, #3
005954d8: cmp      r3, r4
005954dc: bhi      #0x595390
005954e0: ldr      r5, [sp, #0x18]
005954e4: b        #0x594d00
005954e8: mov      r0, r6
005954ec: ldr      r2, [sp, #0x1c]
005954f0: bl       #0x591238
005954f4: b        #0x595218
005954f8: mov      r0, r6
005954fc: ldr      r2, [sp, #0x14]
00595500: bl       #0x591238
00595504: b        #0x595358
00595508: mov      r0, r6
0059550c: ldr      r2, [sp, #0x1c]
00595510: bl       #0x591238
00595514: b        #0x5954d0

# _ZN6glitch5scene12_GLOBAL__N_115createTrianglesIiSt6vectorINS_4core10triangle3dIfEENS4_10SAllocatorIS6_LNS_6memory13E_MEMORY_HINTE0EEEEEEvPKtjRKNS_5video13SVertexStreamERT0_
00595518: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0059551c: mov      r5, r2
00595520: ldrh     r2, [r2, #0xc]
00595524: sub      sp, sp, #0xfc
00595528: mov      r4, r0
0059552c: cmp      r2, #3
00595530: mov      r8, r1
00595534: mov      r6, r3
00595538: beq      #0x595878
0059553c: cmp      r2, #4
00595540: beq      #0x5956e0
00595544: cmp      r2, #2
00595548: beq      #0x595554
0059554c: add      sp, sp, #0xfc
00595550: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00595554: ldr      r0, [r5]
00595558: mov      r1, #1
0059555c: bl       #0x5a1adc
00595560: ldr      r7, [r5, #4]
00595564: cmp      r4, #0
00595568: add      r7, r0, r7
0059556c: beq      #0x595be0
00595570: add      r8, r4, r8, lsl #1
00595574: cmp      r4, r8
00595578: str      r8, [sp, #0xc]
0059557c: beq      #0x5956b0
00595580: add      r2, sp, #0xb0
00595584: mov      r8, #0
00595588: str      r2, [sp, #0x14]
0059558c: str      r5, [sp, #0x10]
00595590: ldr      r2, [sp, #0x10]
00595594: ldrh     sl, [r4, #4]
00595598: ldrh     r3, [r2, #0xe]
0059559c: mul      sl, sl, r3
005955a0: ldr      r0, [r7, sl]
005955a4: str      r3, [sp, #8]
005955a8: bl       #0x30e964
005955ac: add      sl, r7, sl
005955b0: mov      r5, r0
005955b4: ldr      r0, [sl, #4]
005955b8: bl       #0x30e964
005955bc: ldrh     sl, [r4, #2]
005955c0: ldr      r3, [sp, #8]
005955c4: mov      fp, r0
005955c8: mul      sl, r3, sl
005955cc: ldr      r0, [r7, sl]
005955d0: bl       #0x30e964
005955d4: add      sl, r7, sl
005955d8: mov      sb, r0
005955dc: ldr      r0, [sl, #4]
005955e0: bl       #0x30e964
005955e4: ldrh     r2, [r4]
005955e8: ldr      r3, [sp, #8]
005955ec: mov      sl, r0
005955f0: mul      r3, r3, r2
005955f4: ldr      r0, [r7, r3]
005955f8: add      r3, r7, r3
005955fc: str      r3, [sp, #8]
00595600: bl       #0x30e964
00595604: ldr      r3, [sp, #8]
00595608: mov      r2, r0
0059560c: ldr      r0, [r3, #4]
00595610: str      r2, [sp, #4]
00595614: bl       #0x30e964
00595618: ldmib    r6, {r1, r3}
0059561c: ldr      r2, [sp, #4]
00595620: str      fp, [sp, #0xb4]
00595624: cmp      r1, r3
00595628: str      sb, [sp, #0xbc]
0059562c: str      r0, [sp, #0xcc]
00595630: str      sl, [sp, #0xc0]
00595634: str      r2, [sp, #0xc8]
00595638: str      r8, [sp, #0xb8]
0059563c: str      r5, [sp, #0xb0]
00595640: str      r8, [sp, #0xc4]
00595644: str      r8, [sp, #0xd0]
00595648: beq      #0x595a48
0059564c: str      r5, [r1]
00595650: ldr      r3, [sp, #0xb4]
00595654: str      r3, [r1, #4]
00595658: ldr      r3, [sp, #0xb8]
0059565c: str      r3, [r1, #8]
00595660: ldr      r3, [sp, #0xbc]
00595664: str      r3, [r1, #0xc]
00595668: ldr      r3, [sp, #0xc0]
0059566c: str      r3, [r1, #0x10]
00595670: ldr      r3, [sp, #0xc4]
00595674: str      r3, [r1, #0x14]
00595678: ldr      r3, [sp, #0xc8]
0059567c: str      r3, [r1, #0x18]
00595680: ldr      r3, [sp, #0xcc]
00595684: str      r3, [r1, #0x1c]
00595688: ldr      r3, [sp, #0xd0]
0059568c: str      r3, [r1, #0x20]
00595690: ldr      r3, [r6, #4]
00595694: add      r3, r3, #0x24
00595698: str      r3, [r6, #4]
0059569c: ldr      r3, [sp, #0xc]
005956a0: add      r4, r4, #6
005956a4: cmp      r3, r4
005956a8: bne      #0x595590
005956ac: ldr      r5, [sp, #0x10]
005956b0: cmp      r7, #0
005956b4: beq      #0x59554c
005956b8: ldr      r4, [r5]
005956bc: ldrb     r3, [r4, #0x13]
005956c0: and      r2, r3, #0x1f
005956c4: cmp      r2, #1
005956c8: bls      #0x595a0c
005956cc: sub      r2, r2, #1
005956d0: bic      r3, r3, #0x1f
005956d4: orr      r3, r2, r3
005956d8: strb     r3, [r4, #0x13]
005956dc: b        #0x59554c
005956e0: ldr      r0, [r5]
005956e4: mov      r1, #1
005956e8: bl       #0x5a1adc
005956ec: ldr      r7, [r5, #4]
005956f0: cmp      r4, #0
005956f4: add      r7, r0, r7
005956f8: beq      #0x595a68
005956fc: add      r8, r4, r8, lsl #1
00595700: cmp      r4, r8
00595704: str      r8, [sp, #0x14]
00595708: beq      #0x5956b0
0059570c: add      r3, sp, #0x20
00595710: str      r3, [sp, #0x1c]
00595714: str      r5, [sp, #0x18]
00595718: ldr      r2, [sp, #0x18]
0059571c: ldrh     r8, [r4, #4]
00595720: ldrh     r3, [r2, #0xe]
00595724: mul      r8, r8, r3
00595728: ldr      r0, [r7, r8]
0059572c: str      r3, [sp, #8]
00595730: bl       #0x30e964
00595734: add      r8, r7, r8
00595738: mov      r5, r0
0059573c: ldr      r0, [r8, #4]
00595740: bl       #0x30e964
00595744: mov      fp, r0
00595748: ldr      r0, [r8, #8]
0059574c: bl       #0x30e964
00595750: ldrh     r2, [r4, #2]
00595754: ldr      r3, [sp, #8]
00595758: mov      sb, r0
0059575c: mul      r2, r3, r2
00595760: ldr      r0, [r7, r2]
00595764: add      r2, r7, r2
00595768: str      r2, [sp, #0xc]
0059576c: bl       #0x30e964
00595770: ldr      r2, [sp, #0xc]
00595774: mov      sl, r0
00595778: ldr      r0, [r2, #4]
0059577c: bl       #0x30e964
00595780: ldr      r2, [sp, #0xc]
00595784: mov      r8, r0
00595788: ldr      r0, [r2, #8]
0059578c: bl       #0x30e964
00595790: ldrh     r1, [r4]
00595794: ldr      r3, [sp, #8]
00595798: mov      r2, r0
0059579c: mul      r3, r3, r1
005957a0: ldr      r0, [r7, r3]
005957a4: add      r3, r7, r3
005957a8: str      r2, [sp, #4]
005957ac: str      r3, [sp, #0xc]
005957b0: bl       #0x30e964
005957b4: ldr      r3, [sp, #0xc]
005957b8: mov      ip, r0
005957bc: ldr      r0, [r3, #4]
005957c0: str      ip, [sp, #8]
005957c4: bl       #0x30e964
005957c8: ldr      r3, [sp, #0xc]
005957cc: str      r0, [sp, #0x10]
005957d0: ldr      r0, [r3, #8]
005957d4: bl       #0x30e964
005957d8: ldmib    r6, {r1, r3}
005957dc: str      r0, [sp, #0x40]
005957e0: str      fp, [sp, #0x24]
005957e4: str      sb, [sp, #0x28]
005957e8: ldmib    sp, {r2, ip}
005957ec: cmp      r1, r3
005957f0: str      r2, [sp, #0x34]
005957f4: ldr      r2, [sp, #0x10]
005957f8: str      sl, [sp, #0x2c]
005957fc: str      r8, [sp, #0x30]
00595800: str      ip, [sp, #0x38]
00595804: str      r2, [sp, #0x3c]
00595808: str      r5, [sp, #0x20]
0059580c: beq      #0x595a58
00595810: str      r5, [r1]
00595814: ldr      r3, [sp, #0x24]
00595818: str      r3, [r1, #4]
0059581c: ldr      r3, [sp, #0x28]
00595820: str      r3, [r1, #8]
00595824: ldr      r3, [sp, #0x2c]
00595828: str      r3, [r1, #0xc]
0059582c: ldr      r3, [sp, #0x30]
00595830: str      r3, [r1, #0x10]
00595834: ldr      r3, [sp, #0x34]
00595838: str      r3, [r1, #0x14]
0059583c: ldr      r3, [sp, #0x38]
00595840: str      r3, [r1, #0x18]
00595844: ldr      r3, [sp, #0x3c]
00595848: str      r3, [r1, #0x1c]
0059584c: ldr      r3, [sp, #0x40]
00595850: str      r3, [r1, #0x20]
00595854: ldr      r3, [r6, #4]
00595858: add      r3, r3, #0x24
0059585c: str      r3, [r6, #4]
00595860: ldr      r3, [sp, #0x14]
00595864: add      r4, r4, #6
00595868: cmp      r3, r4
0059586c: bne      #0x595718
00595870: ldr      r5, [sp, #0x18]
00595874: b        #0x5956b0
00595878: ldr      r0, [r5]
0059587c: mov      r1, #1
00595880: bl       #0x5a1adc
00595884: ldr      r7, [r5, #4]
00595888: cmp      r4, #0
0059588c: add      r7, r0, r7
00595890: beq      #0x595d20
00595894: add      r8, r4, r8, lsl #1
00595898: cmp      r4, r8
0059589c: str      r8, [sp, #0x14]
005958a0: beq      #0x5956b0
005958a4: add      r3, sp, #0x68
005958a8: str      r3, [sp, #0x1c]
005958ac: str      r5, [sp, #0x18]
005958b0: ldr      r2, [sp, #0x18]
005958b4: ldrh     r8, [r4, #4]
005958b8: ldrh     r3, [r2, #0xe]
005958bc: mul      r8, r8, r3
005958c0: ldr      r0, [r7, r8]
005958c4: str      r3, [sp, #8]
005958c8: bl       #0x30e964
005958cc: add      r8, r7, r8
005958d0: mov      r5, r0
005958d4: ldr      r0, [r8, #4]
005958d8: bl       #0x30e964
005958dc: mov      fp, r0
005958e0: ldr      r0, [r8, #8]
005958e4: bl       #0x30e964
005958e8: ldrh     r2, [r4, #2]
005958ec: ldr      r3, [sp, #8]
005958f0: mov      sb, r0
005958f4: mul      r2, r3, r2
005958f8: ldr      r0, [r7, r2]
005958fc: add      r2, r7, r2
00595900: str      r2, [sp, #0xc]
00595904: bl       #0x30e964
00595908: ldr      r2, [sp, #0xc]
0059590c: mov      sl, r0
00595910: ldr      r0, [r2, #4]
00595914: bl       #0x30e964
00595918: ldr      r2, [sp, #0xc]
0059591c: mov      r8, r0
00595920: ldr      r0, [r2, #8]
00595924: bl       #0x30e964
00595928: ldrh     r1, [r4]
0059592c: ldr      r3, [sp, #8]
00595930: mov      r2, r0
00595934: mul      r3, r3, r1
00595938: ldr      r0, [r7, r3]
0059593c: add      r3, r7, r3
00595940: str      r2, [sp, #4]
00595944: str      r3, [sp, #0xc]
00595948: bl       #0x30e964
0059594c: ldr      r3, [sp, #0xc]
00595950: mov      ip, r0
00595954: ldr      r0, [r3, #4]
00595958: str      ip, [sp, #8]
0059595c: bl       #0x30e964
00595960: ldr      r3, [sp, #0xc]
00595964: str      r0, [sp, #0x10]
00595968: ldr      r0, [r3, #8]
0059596c: bl       #0x30e964
00595970: ldmib    r6, {r1, r3}
00595974: str      r0, [sp, #0x88]
00595978: str      fp, [sp, #0x6c]
0059597c: str      sb, [sp, #0x70]
00595980: ldmib    sp, {r2, ip}
00595984: cmp      r1, r3
00595988: str      r2, [sp, #0x7c]
0059598c: ldr      r2, [sp, #0x10]
00595990: str      sl, [sp, #0x74]
00595994: str      r8, [sp, #0x78]
00595998: str      ip, [sp, #0x80]
0059599c: str      r2, [sp, #0x84]
005959a0: str      r5, [sp, #0x68]
005959a4: beq      #0x595a38
005959a8: str      r5, [r1]
005959ac: ldr      r3, [sp, #0x6c]
005959b0: str      r3, [r1, #4]
005959b4: ldr      r3, [sp, #0x70]
005959b8: str      r3, [r1, #8]
005959bc: ldr      r3, [sp, #0x74]
005959c0: str      r3, [r1, #0xc]
005959c4: ldr      r3, [sp, #0x78]
005959c8: str      r3, [r1, #0x10]
005959cc: ldr      r3, [sp, #0x7c]
005959d0: str      r3, [r1, #0x14]
005959d4: ldr      r3, [sp, #0x80]
005959d8: str      r3, [r1, #0x18]
005959dc: ldr      r3, [sp, #0x84]
005959e0: str      r3, [r1, #0x1c]
005959e4: ldr      r3, [sp, #0x88]
005959e8: str      r3, [r1, #0x20]
005959ec: ldr      r3, [r6, #4]
005959f0: add      r3, r3, #0x24
005959f4: str      r3, [r6, #4]
005959f8: ldr      r3, [sp, #0x14]
005959fc: add      r4, r4, #6
00595a00: cmp      r3, r4
00595a04: bne      #0x5958b0
00595a08: b        #0x595870
00595a0c: ldrb     r3, [r4, #0x12]
00595a10: tst      r3, #0x20
00595a14: bne      #0x595a24
00595a18: mov      r3, #0
00595a1c: strb     r3, [r4, #0x13]
00595a20: b        #0x59554c
00595a24: ldr      r3, [r4]
00595a28: mov      r0, r4
00595a2c: mov      lr, pc
00595a30: ldr      pc, [r3, #0x18]
00595a34: b        #0x595a18
00595a38: mov      r0, r6
00595a3c: ldr      r2, [sp, #0x1c]
00595a40: bl       #0x591238
00595a44: b        #0x5959f8
00595a48: mov      r0, r6
00595a4c: ldr      r2, [sp, #0x14]
00595a50: bl       #0x591238
00595a54: b        #0x59569c
00595a58: mov      r0, r6
00595a5c: ldr      r2, [sp, #0x1c]
00595a60: bl       #0x591238
00595a64: b        #0x595860
00595a68: cmp      r8, #0
00595a6c: beq      #0x5956b0
00595a70: add      r2, sp, #0x44
00595a74: ldrh     r3, [r5, #0xe]
00595a78: str      r2, [sp, #0x1c]
00595a7c: str      r8, [sp, #0x14]
00595a80: str      r5, [sp, #0x18]
00595a84: b        #0x595a90
00595a88: ldr      r2, [sp, #0x18]
00595a8c: ldrh     r3, [r2, #0xe]
00595a90: add      r8, r4, #2
00595a94: mul      r8, r3, r8
00595a98: ldr      r0, [r7, r8]
00595a9c: str      r3, [sp, #8]
00595aa0: bl       #0x30e964
00595aa4: add      r8, r7, r8
00595aa8: mov      r5, r0
00595aac: ldr      r0, [r8, #4]
00595ab0: bl       #0x30e964
00595ab4: mov      fp, r0
00595ab8: ldr      r0, [r8, #8]
00595abc: bl       #0x30e964
00595ac0: ldr      r3, [sp, #8]
00595ac4: mov      sb, r0
00595ac8: mla      r2, r4, r3, r3
00595acc: ldr      r0, [r7, r2]
00595ad0: add      r2, r7, r2
00595ad4: str      r2, [sp, #0xc]
00595ad8: bl       #0x30e964
00595adc: ldr      r2, [sp, #0xc]
00595ae0: mov      sl, r0
00595ae4: ldr      r0, [r2, #4]
00595ae8: bl       #0x30e964
00595aec: ldr      r2, [sp, #0xc]
00595af0: mov      r8, r0
00595af4: ldr      r0, [r2, #8]
00595af8: bl       #0x30e964
00595afc: ldr      r3, [sp, #8]
00595b00: mov      r2, r0
00595b04: mul      r3, r4, r3
00595b08: ldr      r0, [r7, r3]
00595b0c: add      r3, r7, r3
00595b10: str      r2, [sp, #4]
00595b14: str      r3, [sp, #0xc]
00595b18: bl       #0x30e964
00595b1c: ldr      r3, [sp, #0xc]
00595b20: mov      ip, r0
00595b24: ldr      r0, [r3, #4]
00595b28: str      ip, [sp, #8]
00595b2c: bl       #0x30e964
00595b30: ldr      r3, [sp, #0xc]
00595b34: str      r0, [sp, #0x10]
00595b38: ldr      r0, [r3, #8]
00595b3c: bl       #0x30e964
00595b40: ldmib    sp, {r2, ip}
00595b44: ldmib    r6, {r1, r3}
00595b48: str      r2, [sp, #0x58]
00595b4c: str      fp, [sp, #0x48]
00595b50: str      sb, [sp, #0x4c]
00595b54: str      r0, [sp, #0x64]
00595b58: str      sl, [sp, #0x50]
00595b5c: str      r8, [sp, #0x54]
00595b60: str      ip, [sp, #0x5c]
00595b64: ldr      r2, [sp, #0x10]
00595b68: cmp      r1, r3
00595b6c: str      r5, [sp, #0x44]
00595b70: str      r2, [sp, #0x60]
00595b74: beq      #0x595e98
00595b78: str      r5, [r1]
00595b7c: ldr      r3, [sp, #0x48]
00595b80: str      r3, [r1, #4]
00595b84: ldr      r3, [sp, #0x4c]
00595b88: str      r3, [r1, #8]
00595b8c: ldr      r3, [sp, #0x50]
00595b90: str      r3, [r1, #0xc]
00595b94: ldr      r3, [sp, #0x54]
00595b98: str      r3, [r1, #0x10]
00595b9c: ldr      r3, [sp, #0x58]
00595ba0: str      r3, [r1, #0x14]
00595ba4: ldr      r3, [sp, #0x5c]
00595ba8: str      r3, [r1, #0x18]
00595bac: ldr      r3, [sp, #0x60]
00595bb0: str      r3, [r1, #0x1c]
00595bb4: ldr      r3, [sp, #0x64]
00595bb8: str      r3, [r1, #0x20]
00595bbc: ldr      r3, [r6, #4]
00595bc0: add      r3, r3, #0x24
00595bc4: str      r3, [r6, #4]
00595bc8: ldr      r3, [sp, #0x14]
00595bcc: add      r4, r4, #3
00595bd0: cmp      r3, r4
00595bd4: bhi      #0x595a88
00595bd8: ldr      r5, [sp, #0x18]
00595bdc: b        #0x5956b0
00595be0: cmp      r8, #0
00595be4: beq      #0x5956b0
00595be8: add      r2, sp, #0xd4
00595bec: ldrh     r3, [r5, #0xe]
00595bf0: mov      sl, #0
00595bf4: str      r2, [sp, #0x14]
00595bf8: str      r8, [sp, #0xc]
00595bfc: str      r5, [sp, #0x10]
00595c00: b        #0x595c0c
00595c04: ldr      r2, [sp, #0x10]
00595c08: ldrh     r3, [r2, #0xe]
00595c0c: add      r8, r4, #2
00595c10: mul      r8, r3, r8
00595c14: ldr      r0, [r7, r8]
00595c18: str      r3, [sp, #8]
00595c1c: bl       #0x30e964
00595c20: add      r8, r7, r8
00595c24: mov      r5, r0
00595c28: ldr      r0, [r8, #4]
00595c2c: bl       #0x30e964
00595c30: ldr      r3, [sp, #8]
00595c34: mov      fp, r0
00595c38: mla      r8, r4, r3, r3
00595c3c: ldr      r0, [r7, r8]
00595c40: bl       #0x30e964
00595c44: add      r8, r7, r8
00595c48: mov      sb, r0
00595c4c: ldr      r0, [r8, #4]
00595c50: bl       #0x30e964
00595c54: ldr      r3, [sp, #8]
00595c58: mov      r8, r0
00595c5c: mul      r3, r4, r3
00595c60: ldr      r0, [r7, r3]
00595c64: add      r3, r7, r3
00595c68: str      r3, [sp, #8]
00595c6c: bl       #0x30e964
00595c70: ldr      r3, [sp, #8]
00595c74: mov      r2, r0
00595c78: ldr      r0, [r3, #4]
00595c7c: str      r2, [sp, #4]
00595c80: bl       #0x30e964
00595c84: ldmib    r6, {r1, r3}
00595c88: ldr      r2, [sp, #4]
00595c8c: str      fp, [sp, #0xd8]
00595c90: cmp      r1, r3
00595c94: str      sb, [sp, #0xe0]
00595c98: str      r8, [sp, #0xe4]
00595c9c: str      r0, [sp, #0xf0]
00595ca0: str      r2, [sp, #0xec]
00595ca4: str      sl, [sp, #0xdc]
00595ca8: str      sl, [sp, #0xe8]
00595cac: str      r5, [sp, #0xd4]
00595cb0: str      sl, [sp, #0xf4]
00595cb4: beq      #0x595ea8
00595cb8: str      r5, [r1]
00595cbc: ldr      r3, [sp, #0xd8]
00595cc0: str      r3, [r1, #4]
00595cc4: ldr      r3, [sp, #0xdc]
00595cc8: str      r3, [r1, #8]
00595ccc: ldr      r3, [sp, #0xe0]
00595cd0: str      r3, [r1, #0xc]
00595cd4: ldr      r3, [sp, #0xe4]
00595cd8: str      r3, [r1, #0x10]
00595cdc: ldr      r3, [sp, #0xe8]
00595ce0: str      r3, [r1, #0x14]
00595ce4: ldr      r3, [sp, #0xec]
00595ce8: str      r3, [r1, #0x18]
00595cec: ldr      r3, [sp, #0xf0]
00595cf0: str      r3, [r1, #0x1c]
00595cf4: ldr      r3, [sp, #0xf4]
00595cf8: str      r3, [r1, #0x20]
00595cfc: ldr      r3, [r6, #4]
00595d00: add      r3, r3, #0x24
00595d04: str      r3, [r6, #4]
00595d08: ldr      r3, [sp, #0xc]
00595d0c: add      r4, r4, #3
00595d10: cmp      r3, r4
00595d14: bhi      #0x595c04
00595d18: ldr      r5, [sp, #0x10]
00595d1c: b        #0x5956b0
00595d20: cmp      r8, #0
00595d24: beq      #0x5956b0
00595d28: add      r2, sp, #0x8c
00595d2c: ldrh     r3, [r5, #0xe]
00595d30: str      r2, [sp, #0x1c]
00595d34: str      r8, [sp, #0x14]
00595d38: str      r5, [sp, #0x18]
00595d3c: b        #0x595d48
00595d40: ldr      r2, [sp, #0x18]
00595d44: ldrh     r3, [r2, #0xe]
00595d48: add      r8, r4, #2
00595d4c: mul      r8, r3, r8
00595d50: ldr      r0, [r7, r8]
00595d54: str      r3, [sp, #8]
00595d58: bl       #0x30e964
00595d5c: add      r8, r7, r8
00595d60: mov      r5, r0
00595d64: ldr      r0, [r8, #4]
00595d68: bl       #0x30e964
00595d6c: mov      fp, r0
00595d70: ldr      r0, [r8, #8]
00595d74: bl       #0x30e964
00595d78: ldr      r3, [sp, #8]
00595d7c: mov      sb, r0
00595d80: mla      r2, r4, r3, r3
00595d84: ldr      r0, [r7, r2]
00595d88: add      r2, r7, r2
00595d8c: str      r2, [sp, #0xc]
00595d90: bl       #0x30e964
00595d94: ldr      r2, [sp, #0xc]
00595d98: mov      sl, r0
00595d9c: ldr      r0, [r2, #4]
00595da0: bl       #0x30e964
00595da4: ldr      r2, [sp, #0xc]
00595da8: mov      r8, r0
00595dac: ldr      r0, [r2, #8]
00595db0: bl       #0x30e964
00595db4: ldr      r3, [sp, #8]
00595db8: mov      r2, r0
00595dbc: mul      r3, r4, r3
00595dc0: ldr      r0, [r7, r3]
00595dc4: add      r3, r7, r3
00595dc8: str      r2, [sp, #4]
00595dcc: str      r3, [sp, #0xc]
00595dd0: bl       #0x30e964
00595dd4: ldr      r3, [sp, #0xc]
00595dd8: mov      ip, r0
00595ddc: ldr      r0, [r3, #4]
00595de0: str      ip, [sp, #8]
00595de4: bl       #0x30e964
00595de8: ldr      r3, [sp, #0xc]
00595dec: str      r0, [sp, #0x10]
00595df0: ldr      r0, [r3, #8]
00595df4: bl       #0x30e964
00595df8: ldmib    sp, {r2, ip}
00595dfc: ldmib    r6, {r1, r3}
00595e00: str      r2, [sp, #0xa0]
00595e04: str      fp, [sp, #0x90]
00595e08: str      sb, [sp, #0x94]
00595e0c: str      r0, [sp, #0xac]
00595e10: str      sl, [sp, #0x98]
00595e14: str      r8, [sp, #0x9c]
00595e18: str      ip, [sp, #0xa4]
00595e1c: ldr      r2, [sp, #0x10]
00595e20: cmp      r1, r3
00595e24: str      r5, [sp, #0x8c]
00595e28: str      r2, [sp, #0xa8]
00595e2c: beq      #0x595eb8
00595e30: str      r5, [r1]
00595e34: ldr      r3, [sp, #0x90]
00595e38: str      r3, [r1, #4]
00595e3c: ldr      r3, [sp, #0x94]
00595e40: str      r3, [r1, #8]
00595e44: ldr      r3, [sp, #0x98]
00595e48: str      r3, [r1, #0xc]
00595e4c: ldr      r3, [sp, #0x9c]
00595e50: str      r3, [r1, #0x10]
00595e54: ldr      r3, [sp, #0xa0]
00595e58: str      r3, [r1, #0x14]
00595e5c: ldr      r3, [sp, #0xa4]
00595e60: str      r3, [r1, #0x18]
00595e64: ldr      r3, [sp, #0xa8]
00595e68: str      r3, [r1, #0x1c]
00595e6c: ldr      r3, [sp, #0xac]
00595e70: str      r3, [r1, #0x20]
00595e74: ldr      r3, [r6, #4]
00595e78: add      r3, r3, #0x24
00595e7c: str      r3, [r6, #4]
00595e80: ldr      r3, [sp, #0x14]
00595e84: add      r4, r4, #3
00595e88: cmp      r3, r4
00595e8c: bhi      #0x595d40
00595e90: ldr      r5, [sp, #0x18]
00595e94: b        #0x5956b0
00595e98: mov      r0, r6
00595e9c: ldr      r2, [sp, #0x1c]
00595ea0: bl       #0x591238
00595ea4: b        #0x595bc8
00595ea8: mov      r0, r6
00595eac: ldr      r2, [sp, #0x14]
00595eb0: bl       #0x591238
00595eb4: b        #0x595d08
00595eb8: mov      r0, r6
00595ebc: ldr      r2, [sp, #0x1c]
00595ec0: bl       #0x591238
00595ec4: b        #0x595e80

# _ZN7PFFloor12_LoadNavMeshEPN6glitch5scene14IMeshSceneNodeE
00520b40: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00520b44: mov      r4, r0
00520b48: sub      sp, sp, #0x38
00520b4c: mov      r0, r1
00520b50: mov      r5, r1
00520b54: bl       #0x597290
00520b58: ldr      r3, [r0]
00520b5c: mov      lr, pc
00520b60: ldr      pc, [r3, #0xac]
00520b64: ldr      sl, [pc, #0x400]
00520b68: add      r8, sp, #8
00520b6c: mov      r1, r0
00520b70: add      r7, sp, #0x38
00520b74: add      sl, pc, sl
00520b78: mov      r0, r8
00520b7c: bl       #0x319158
00520b80: add      sb, r8, #4
00520b84: str      sl, [r7, #-8]!
00520b88: mov      r0, sb
00520b8c: mov      r1, r7
00520b90: bl       #0x51cfb8
00520b94: ldr      r6, [pc, #0x3d4]
00520b98: cmp      sb, r0
00520b9c: add      r6, pc, r6
00520ba0: beq      #0x520bd4
00520ba4: mov      r1, r7
00520ba8: mov      r0, sb
00520bac: str      sl, [sp, #0x30]
00520bb0: bl       #0x51cfb8
00520bb4: ldr      sb, [r0, #0x3c]
00520bb8: add      sl, r4, #0x28
00520bbc: mov      r0, sb
00520bc0: bl       #0x30de54
00520bc4: mov      r1, sb
00520bc8: add      r2, sb, r0
00520bcc: mov      r0, sl
00520bd0: bl       #0x3109e0
00520bd4: ldr      sb, [r4, #0x3c]
00520bd8: ldr      r1, [pc, #0x394]
00520bdc: mov      r0, sb
00520be0: add      r1, pc, r1
00520be4: bl       #0x30ebd4
00520be8: ldr      sl, [r4, #0x24]
00520bec: ldr      r1, [pc, #0x384]
00520bf0: cmp      r0, #0
00520bf4: orrne    sl, sl, #0x1000000
00520bf8: strne    sl, [r4, #0x24]
00520bfc: add      r1, pc, r1
00520c00: mov      r0, sb
00520c04: bl       #0x30ebd4
00520c08: ldr      r1, [pc, #0x36c]
00520c0c: cmp      r0, #0
00520c10: orrne    sl, sl, #0x2000000
00520c14: strne    sl, [r4, #0x24]
00520c18: add      r1, pc, r1
00520c1c: mov      r0, sb
00520c20: bl       #0x30ebd4
00520c24: ldr      r1, [pc, #0x354]
00520c28: cmp      r0, #0
00520c2c: orrne    sl, sl, #1
00520c30: strne    sl, [r4, #0x24]
00520c34: add      r1, pc, r1
00520c38: mov      r0, sb
00520c3c: bl       #0x30ebd4
00520c40: cmp      r0, #0
00520c44: orrne    sl, sl, #2
00520c48: strne    sl, [r4, #0x24]
00520c4c: tst      sl, #0x3000000
00520c50: ldrne    r3, [r4, #0x20]
00520c54: mov      r0, r5
00520c58: orrne    r3, r3, #0x7000000
00520c5c: strne    r3, [r4, #0x20]
00520c60: bl       #0x597290
00520c64: cmp      r0, #0
00520c68: beq      #0x520f18
00520c6c: mov      r0, r5
00520c70: bl       #0x597290
00520c74: cmp      r0, #0
00520c78: beq      #0x520ca0
00520c7c: ldr      r3, [r5]
00520c80: add      r6, sp, #0x24
00520c84: mov      r0, r6
00520c88: mov      r1, r5
00520c8c: ldr      sl, [r3, #0xa4]
00520c90: bl       #0x597180
00520c94: mov      r0, r5
00520c98: mov      r1, r6
00520c9c: blx      sl
00520ca0: mov      r0, r5
00520ca4: bl       #0x50f89c
00520ca8: str      r0, [r4, #0x40]
00520cac: mov      r1, #0
00520cb0: mov      r0, r5
00520cb4: ldr      r3, [r5]
00520cb8: mov      lr, pc
00520cbc: ldr      pc, [r3, #0x48]
00520cc0: mov      r0, r5
00520cc4: ldr      r3, [r5]
00520cc8: mov      lr, pc
00520ccc: ldr      pc, [r3, #0x68]
00520cd0: ldr      r3, [r4, #0x40]
00520cd4: mov      r0, r3
00520cd8: ldr      r3, [r3]
00520cdc: mov      lr, pc
00520ce0: ldr      pc, [r3, #0xa0]
00520ce4: ldr      r1, [r0]
00520ce8: mov      r3, r0
00520cec: ldr      r2, [r4, #0x40]
00520cf0: str      r1, [r4, #0x5c]
00520cf4: ldr      r1, [r0, #4]
00520cf8: mov      r0, r2
00520cfc: str      r1, [r4, #0x60]
00520d00: ldr      r3, [r3, #8]
00520d04: str      r3, [r4, #0x64]
00520d08: ldr      r3, [r2]
00520d0c: mov      lr, pc
00520d10: ldr      pc, [r3, #0x34]
00520d14: ldr      r3, [r0]
00520d18: mov      r1, #0x44000000
00520d1c: add      r1, r1, #0x7a0000
00520d20: str      r3, [r4, #0x44]
00520d24: ldr      r3, [r0, #4]
00520d28: str      r3, [r4, #0x48]
00520d2c: ldr      r5, [r0, #8]
00520d30: str      r5, [r4, #0x4c]
00520d34: ldr      r3, [r0, #0xc]
00520d38: str      r3, [r4, #0x50]
00520d3c: ldr      r3, [r0, #0x10]
00520d40: str      r3, [r4, #0x54]
00520d44: ldr      r0, [r0, #0x14]
00520d48: bl       #0x30eba4
00520d4c: mov      r1, #0x44000000
00520d50: str      r0, [r4, #0x58]
00520d54: add      r1, r1, #0x7a0000
00520d58: mov      r0, r5
00520d5c: bl       #0x30e3ac
00520d60: ldr      r3, [r4, #0x40]
00520d64: str      r0, [r4, #0x4c]
00520d68: add      r0, sp, #0x34
00520d6c: mov      r1, r3
00520d70: ldr      r3, [r3]
00520d74: mov      lr, pc
00520d78: ldr      pc, [r3, #0xf8]
00520d7c: mov      r1, #0
00520d80: mov      r0, #0xb8
00520d84: ldr      r5, [sp, #0x34]
00520d88: bl       #0x5341ac
00520d8c: ldr      r2, [r4, #0x40]
00520d90: mov      ip, #1
00520d94: mov      r1, r5
00520d98: mov      r3, #0xf
00520d9c: mov      r6, r0
00520da0: str      ip, [sp]
00520da4: bl       #0x588654
00520da8: ldr      r0, [sp, #0x34]
00520dac: cmp      r0, #0
00520db0: beq      #0x520db8
00520db4: bl       #0x31d584
00520db8: ldr      r3, [r4, #0x40]
00520dbc: mov      r1, r6
00520dc0: mov      r0, r3
00520dc4: ldr      r3, [r3]
00520dc8: mov      lr, pc
00520dcc: ldr      pc, [r3, #0xb4]
00520dd0: mov      r0, r6
00520dd4: bl       #0x31d584
00520dd8: ldr      r3, [r6]
00520ddc: mov      r0, r6
00520de0: mov      lr, pc
00520de4: ldr      pc, [r3, #0xc]
00520de8: cmp      r0, #0
00520dec: mov      sl, r0
00520df0: str      r0, [sp, #0x30]
00520df4: movle    r5, #0
00520df8: ble      #0x520e7c
00520dfc: mov      r0, #0x24
00520e00: mov      r1, #0
00520e04: mul      r0, r0, sl
00520e08: bl       #0x31056c
00520e0c: mov      r2, #0
00520e10: mov      r5, r0
00520e14: mov      r3, r0
00520e18: mov      r1, #0
00520e1c: b        #0x520e24
00520e20: add      r3, r3, #0x24
00520e24: add      r1, r1, #1
00520e28: cmp      sl, r1
00520e2c: str      r2, [r3]
00520e30: str      r2, [r3, #4]
00520e34: str      r2, [r3, #8]
00520e38: str      r2, [r3, #0xc]
00520e3c: str      r2, [r3, #0x10]
00520e40: str      r2, [r3, #0x14]
00520e44: str      r2, [r3, #0x18]
00520e48: str      r2, [r3, #0x1c]
00520e4c: str      r2, [r3, #0x20]
00520e50: bne      #0x520e20
00520e54: mov      r1, #0
00520e58: ldr      ip, [r6]
00520e5c: mov      r0, r6
00520e60: str      r1, [sp]
00520e64: ldr      r2, [sp, #0x30]
00520e68: mov      r3, r7
00520e6c: mov      r1, r5
00520e70: mov      lr, pc
00520e74: ldr      pc, [ip, #0x10]
00520e78: ldr      sl, [sp, #0x30]
00520e7c: mov      r2, sl
00520e80: mov      r0, r4
00520e84: mov      r1, r5
00520e88: bl       #0x520588
00520e8c: ldr      r3, [sp, #0x30]
00520e90: str      r5, [r4, #0x68]
00520e94: cmp      r3, #0
00520e98: str      r3, [r4, #0x6c]
00520e9c: beq      #0x520f08
00520ea0: mov      r6, #0
00520ea4: mov      r7, r6
00520ea8: b        #0x520eb0
00520eac: ldr      r5, [r4, #0x68]
00520eb0: add      r5, r5, r6
00520eb4: ldr      r0, [r5, #8]
00520eb8: mov      r1, #0x3f800000
00520ebc: bl       #0x30eba4
00520ec0: str      r0, [r5, #8]
00520ec4: ldr      r5, [r4, #0x68]
00520ec8: mov      r1, #0x3f800000
00520ecc: add      r7, r7, #1
00520ed0: add      r5, r5, r6
00520ed4: ldr      r0, [r5, #0x14]
00520ed8: bl       #0x30eba4
00520edc: str      r0, [r5, #0x14]
00520ee0: ldr      r5, [r4, #0x68]
00520ee4: mov      r1, #0x3f800000
00520ee8: add      r5, r5, r6
00520eec: ldr      r0, [r5, #0x20]
00520ef0: bl       #0x30eba4
00520ef4: str      r0, [r5, #0x20]
00520ef8: ldr      r3, [r4, #0x6c]
00520efc: add      r6, r6, #0x24
00520f00: cmp      r3, r7
00520f04: bhi      #0x520eac
00520f08: mov      r0, r8
00520f0c: bl       #0x318178
00520f10: add      sp, sp, #0x38
00520f14: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00520f18: ldr      r3, [pc, #0x64]
00520f1c: ldr      r3, [r6, r3]
00520f20: ldr      r3, [r3]
00520f24: cmp      r3, #2
00520f28: streq    r0, [r0]
00520f2c: beq      #0x520c6c
00520f30: cmp      r3, #1
00520f34: bne      #0x520c6c
00520f38: ldr      r0, [pc, #0x48]
00520f3c: ldr      r1, [pc, #0x48]
00520f40: ldr      r2, [pc, #0x48]
00520f44: ldr      r0, [r6, r0]
00520f48: ldr      r3, [pc, #0x44]
00520f4c: mov      ip, #0x8a
00520f50: add      r1, pc, r1
00520f54: add      r2, pc, r2
00520f58: add      r3, pc, r3
00520f5c: add      r0, r0, #0xa8
00520f60: str      ip, [sp]
00520f64: bl       #0x30e004
00520f68: b        #0x520c6c
00520f6c: eorseq   fp, fp, r4, asr #27
00520f70: strdeq   r3, r4, [r7], #-0xe4
00520f74: eorseq   sl, lr, r0, lsl fp
00520f78: eorseq   fp, fp, ip, asr #26
00520f7c: eorseq   fp, fp, r8, lsr sp
00520f80: eorseq   fp, fp, r4, lsr #26
00520f84: andeq    r3, r0, r0, asr #19
00520f88: andeq    r1, r0, r0, asr #19
00520f8c: eorseq   sp, sb, r8, lsl #9
00520f90: eorseq   fp, fp, ip, lsl #20
00520f94: eorseq   fp, fp, r8, ror sb

# _ZNSt6vectorIN6glitch4core10triangle3dIfEENS1_10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE7reserveEj
00591060: push     {r4, r5, r6, lr}
00591064: mov      r4, r0
00591068: ldr      r2, [r0]
0059106c: ldr      r0, [r0, #8]
00591070: sub      sp, sp, #8
00591074: mov      r3, r1
00591078: rsb      r0, r2, r0
0059107c: asr      r0, r0, #2
00591080: str      r1, [sp, #4]
00591084: lsl      r1, r0, #3
00591088: rsb      r1, r0, r1
0059108c: add      r1, r1, r1, lsl #6
00591090: add      r1, r0, r1, lsl #3
00591094: lsl      ip, r1, #0xf
00591098: rsb      r1, r1, ip
0059109c: add      r0, r0, r1, lsl #3
005910a0: cmp      r3, r0
005910a4: bls      #0x591164
005910a8: movw     r1, #0x71c7
005910ac: orr      r1, r1, r1, lsl #12
005910b0: cmp      r3, r1
005910b4: bhi      #0x59116c
005910b8: ldr      r3, [r4, #4]
005910bc: cmp      r2, #0
005910c0: rsb      r1, r2, r3
005910c4: asr      r1, r1, #2
005910c8: lsl      r0, r1, #3
005910cc: rsb      r0, r1, r0
005910d0: add      r0, r0, r0, lsl #6
005910d4: add      r0, r1, r0, lsl #3
005910d8: lsl      r5, r0, #0xf
005910dc: rsb      r5, r0, r5
005910e0: add      r5, r1, r5, lsl #3
005910e4: beq      #0x591180
005910e8: mov      r0, r4
005910ec: add      r1, sp, #4
005910f0: bl       #0x590fb0
005910f4: ldr      r3, [r4]
005910f8: mov      r6, r0
005910fc: ldr      r0, [r4, #4]
00591100: cmp      r0, r3
00591104: beq      #0x591144
00591108: sub      r2, r0, #0x24
0059110c: rsb      r3, r3, r2
00591110: lsr      r3, r3, #2
00591114: lsl      r2, r3, #3
00591118: rsb      r2, r3, r2
0059111c: add      r2, r2, r2, lsl #6
00591120: add      r2, r3, r2, lsl #3
00591124: lsl      r1, r2, #0xf
00591128: rsb      r2, r2, r1
0059112c: add      r3, r3, r2, lsl #3
00591130: bic      r3, r3, #0xc0000000
00591134: mvn      r2, #0x23
00591138: mul      r3, r2, r3
0059113c: add      r3, r3, r2
00591140: add      r0, r0, r3
00591144: bl       #0x310450
00591148: ldr      r2, [sp, #4]
0059114c: mov      r3, #0x24
00591150: mla      r5, r3, r5, r6
00591154: mla      r3, r3, r2, r6
00591158: str      r5, [r4, #4]
0059115c: str      r3, [r4, #8]
00591160: str      r6, [r4]
00591164: add      sp, sp, #8
00591168: pop      {r4, r5, r6, pc}
0059116c: ldr      r0, [pc, #0x28]
00591170: add      r0, pc, r0
00591174: bl       #0x708e40
00591178: ldr      r2, [r4]
0059117c: b        #0x5910b8
00591180: ldr      r3, [sp, #4]
00591184: mov      r0, #0x24
00591188: mov      r1, r2
0059118c: mul      r0, r0, r3
00591190: bl       #0x310568
00591194: mov      r6, r0
00591198: b        #0x591148
0059119c: ldrshteq sp, [r2], -r8

# _ZNK6glitch4core8CMatrix4IfE14transformBoxExERNS0_8aabbox3dIfEE
00597548: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0059754c: sub      sp, sp, #0x54
00597550: str      r1, [sp, #8]
00597554: str      r0, [sp, #0x14]
00597558: ldr      r8, [r0, #0x30]
0059755c: ldr      r2, [r0, #0x34]
00597560: ldr      r3, [r0, #0x38]
00597564: ldr      r0, [r1]
00597568: ldr      r4, [sp, #8]
0059756c: mov      sb, r8
00597570: str      r0, [sp, #0x1c]
00597574: ldr      ip, [r1, #8]
00597578: ldr      lr, [r1, #4]
0059757c: ldr      r1, [r1, #0xc]
00597580: ldr      r5, [sp, #0x1c]
00597584: str      r1, [sp, #0x18]
00597588: ldr      r0, [r4, #0x10]
0059758c: ldr      r1, [r4, #0x14]
00597590: str      ip, [sp, #0x4c]
00597594: ldr      ip, [sp, #0x18]
00597598: str      r0, [sp, #0x3c]
0059759c: str      r1, [sp, #0x40]
005975a0: str      r2, [sp, #0x30]
005975a4: str      r3, [sp, #0x34]
005975a8: str      r2, [sp, #0x24]
005975ac: str      r3, [sp, #0x28]
005975b0: add      r0, sp, #0x44
005975b4: add      r1, sp, #0x38
005975b8: add      r2, sp, #0x2c
005975bc: add      r3, sp, #0x20
005975c0: str      r5, [sp, #0x44]
005975c4: str      lr, [sp, #0x48]
005975c8: str      ip, [sp, #0x38]
005975cc: str      r8, [sp, #0x20]
005975d0: str      r8, [sp, #0x2c]
005975d4: str      r0, [sp, #0xc]
005975d8: str      r1, [sp, #0x10]
005975dc: mov      r5, #0
005975e0: stm      sp, {r2, r3}
005975e4: ldr      r0, [sp, #0x14]
005975e8: ldr      sl, [sp, #0x18]
005975ec: ldr      r1, [sp, #0x1c]
005975f0: mov      r4, #0
005975f4: add      fp, r0, r5
005975f8: ldr      r7, [fp, r4, lsl #2]
005975fc: mov      r0, r7
00597600: bl       #0x30ed6c
00597604: mov      r1, sl
00597608: mov      r6, r0
0059760c: mov      r0, r7
00597610: bl       #0x30ed6c
00597614: mov      r7, r0
00597618: mov      r1, r7
0059761c: mov      r0, r6
00597620: bl       #0x30e70c
00597624: cmp      r0, #0
00597628: mov      r1, r8
0059762c: mov      r0, r6
00597630: beq      #0x597680
00597634: bl       #0x30eba4
00597638: ldr      ip, [sp]
0059763c: mov      r1, sb
00597640: add      r4, r4, #4
00597644: str      r0, [ip, r5]
00597648: mov      r0, r7
0059764c: bl       #0x30eba4
00597650: ldr      r1, [sp, #4]
00597654: cmp      r4, #0xc
00597658: str      r0, [r1, r5]
0059765c: beq      #0x5976b4
00597660: ldr      ip, [sp, #0xc]
00597664: ldr      r0, [sp, #0x10]
00597668: ldm      sp, {r2, r3}
0059766c: ldr      r1, [ip, r4]
00597670: ldr      sl, [r0, r4]
00597674: ldr      r8, [r2, r5]
00597678: ldr      sb, [r3, r5]
0059767c: b        #0x5975f8
00597680: mov      r1, r8
00597684: mov      r0, r7
00597688: bl       #0x30eba4
0059768c: ldr      r2, [sp]
00597690: mov      r1, sb
00597694: add      r4, r4, #4
00597698: str      r0, [r2, r5]
0059769c: mov      r0, r6
005976a0: bl       #0x30eba4
005976a4: ldr      r3, [sp, #4]
005976a8: cmp      r4, #0xc
005976ac: str      r0, [r3, r5]
005976b0: bne      #0x597660
005976b4: add      r5, r5, #4
005976b8: cmp      r5, #0xc
005976bc: beq      #0x5976d0
005976c0: ldm      sp, {r4, ip}
005976c4: ldr      r8, [r4, r5]
005976c8: ldr      sb, [ip, r5]
005976cc: b        #0x5975e4
005976d0: ldr      ip, [sp, #0x30]
005976d4: ldr      r1, [sp, #0x34]
005976d8: ldr      r2, [sp, #0x20]
005976dc: ldr      r3, [sp, #0x24]
005976e0: ldr      r0, [sp, #0x28]
005976e4: ldr      r4, [sp, #0x2c]
005976e8: ldr      r5, [sp, #8]
005976ec: str      r4, [r5]
005976f0: str      ip, [r5, #4]
005976f4: str      r0, [r5, #0x14]
005976f8: str      r1, [r5, #8]
005976fc: str      r2, [r5, #0xc]
00597700: str      r3, [r5, #0x10]
00597704: add      sp, sp, #0x54
00597708: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK6glitch5scene10ISceneNode25getTransformedBoundingBoxEv
0059770c: push     {r4, r5, r6, lr}
00597710: ldr      r3, [r0, #0x11c]
00597714: mov      r4, r0
00597718: tst      r3, #0x100
0059771c: addeq    r5, r0, #0xd4
00597720: beq      #0x597780
00597724: ldr      r3, [r0]
00597728: mov      lr, pc
0059772c: ldr      pc, [r3, #0x30]
00597730: ldr      r2, [r0]
00597734: mov      r3, r0
00597738: add      r5, r4, #0xd4
0059773c: str      r2, [r4, #0xd4]
00597740: ldr      r2, [r3, #4]
00597744: add      r0, r4, #0x24
00597748: mov      r1, r5
0059774c: str      r2, [r4, #0xd8]
00597750: ldr      r2, [r3, #8]
00597754: str      r2, [r4, #0xdc]
00597758: ldr      r2, [r3, #0xc]
0059775c: str      r2, [r4, #0xe0]
00597760: ldr      r2, [r3, #0x10]
00597764: str      r2, [r4, #0xe4]
00597768: ldr      r3, [r3, #0x14]
0059776c: str      r3, [r4, #0xe8]
00597770: bl       #0x597548
00597774: ldr      r3, [r4, #0x11c]
00597778: bic      r3, r3, #0x100
0059777c: str      r3, [r4, #0x11c]
00597780: mov      r0, r5
00597784: pop      {r4, r5, r6, pc}

# _ZNK6glitch5scene17CTriangleSelector12getTrianglesEPNS_4core10triangle3dIfEEiRiPKNS2_8CMatrix4IfEE
00591464: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00591468: mov      ip, r0
0059146c: ldr      lr, [r0, #0x10]
00591470: ldr      r0, [r0, #0xc]
00591474: sub      sp, sp, #0x74
00591478: ldr      r7, [sp, #0x98]
0059147c: rsb      lr, r0, lr
00591480: asr      lr, lr, #2
00591484: add      r4, sp, #0x2c
00591488: lsl      r5, lr, #3
0059148c: rsb      r5, lr, r5
00591490: add      r5, r5, r5, lsl #6
00591494: str      r1, [sp, #0x18]
00591498: add      r5, lr, r5, lsl #3
0059149c: mov      r0, r4
005914a0: lsl      r6, r5, #0xf
005914a4: rsb      r5, r5, r6
005914a8: add      lr, lr, r5, lsl #3
005914ac: cmp      r2, lr
005914b0: movge    r2, lr
005914b4: str      r2, [sp, #0x1c]
005914b8: mov      r1, #0
005914bc: mov      r2, #0x40
005914c0: str      r3, [sp, #0x24]
005914c4: str      ip, [sp]
005914c8: bl       #0x30e460
005914cc: mov      r3, #0x3f800000
005914d0: mov      r2, #1
005914d4: cmp      r7, #0
005914d8: str      r3, [sp, #0x68]
005914dc: strb     r2, [sp, #0x6c]
005914e0: str      r3, [sp, #0x2c]
005914e4: str      r3, [sp, #0x40]
005914e8: str      r3, [sp, #0x54]
005914ec: ldr      ip, [sp]
005914f0: beq      #0x591508
005914f4: mov      r1, r7
005914f8: mov      r0, r4
005914fc: mov      r2, #0x41
00591500: bl       #0x30e868
00591504: ldr      ip, [sp]
00591508: ldr      r3, [ip, #8]
0059150c: cmp      r3, #0
00591510: beq      #0x591520
00591514: ldrb     r2, [ip, #0x18]
00591518: cmp      r2, #0
0059151c: beq      #0x591978
00591520: ldrb     r5, [sp, #0x6c]
00591524: cmp      r5, #0
00591528: beq      #0x5915c0
0059152c: ldr      r1, [sp, #0x1c]
00591530: cmp      r1, #0
00591534: ble      #0x5915ac
00591538: ldr      r6, [sp, #0x1c]
0059153c: ldr      r4, [sp, #0x18]
00591540: mov      r1, #0
00591544: mov      r0, r1
00591548: ldr      r2, [ip, #0xc]
0059154c: add      r3, r4, r1
00591550: add      r0, r0, #1
00591554: ldr      r5, [r2, r1]
00591558: add      r2, r2, r1
0059155c: cmp      r0, r6
00591560: str      r5, [r4, r1]
00591564: ldr      r5, [r2, #4]
00591568: add      r1, r1, #0x24
0059156c: str      r5, [r3, #4]
00591570: ldr      r5, [r2, #8]
00591574: str      r5, [r3, #8]
00591578: ldr      r5, [r2, #0xc]
0059157c: str      r5, [r3, #0xc]
00591580: ldr      r5, [r2, #0x10]
00591584: str      r5, [r3, #0x10]
00591588: ldr      r5, [r2, #0x14]
0059158c: str      r5, [r3, #0x14]
00591590: ldr      r5, [r2, #0x18]
00591594: str      r5, [r3, #0x18]
00591598: ldr      r5, [r2, #0x1c]
0059159c: str      r5, [r3, #0x1c]
005915a0: ldr      r2, [r2, #0x20]
005915a4: str      r2, [r3, #0x20]
005915a8: bne      #0x591548
005915ac: ldr      r3, [sp, #0x1c]
005915b0: ldr      r1, [sp, #0x24]
005915b4: str      r3, [r1]
005915b8: add      sp, sp, #0x74
005915bc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005915c0: ldr      r2, [sp, #0x1c]
005915c4: cmp      r2, #0
005915c8: ble      #0x5915ac
005915cc: str      ip, [sp, #0x20]
005915d0: mov      ip, r5
005915d4: ldr      r1, [sp, #0x20]
005915d8: ldr      r2, [sp, #0x18]
005915dc: add      ip, ip, #1
005915e0: ldr      r3, [r1, #0xc]
005915e4: ldr      r1, [sp, #0x30]
005915e8: add      r4, r2, r5
005915ec: ldr      fp, [r3, r5]
005915f0: add      r3, r3, r5
005915f4: str      fp, [r2, r5]
005915f8: ldr      sb, [r3, #4]
005915fc: mov      r0, fp
00591600: str      sb, [r4, #4]
00591604: ldr      sl, [r3, #8]
00591608: str      sl, [r4, #8]
0059160c: ldr      r8, [r3, #0xc]
00591610: str      r8, [r4, #0xc]
00591614: ldr      r7, [r3, #0x10]
00591618: str      r7, [r4, #0x10]
0059161c: ldr      r6, [r3, #0x14]
00591620: str      r6, [r4, #0x14]
00591624: ldr      r2, [r3, #0x18]
00591628: str      r2, [sp, #0x14]
0059162c: str      r2, [r4, #0x18]
00591630: ldr      r2, [r3, #0x1c]
00591634: str      r2, [sp, #0x10]
00591638: str      r2, [r4, #0x1c]
0059163c: ldr      r3, [r3, #0x20]
00591640: str      r3, [sp, #0xc]
00591644: str      r3, [r4, #0x20]
00591648: str      ip, [sp]
0059164c: bl       #0x30ed6c
00591650: ldr      r1, [sp, #0x40]
00591654: mov      r3, r0
00591658: mov      r0, sb
0059165c: str      r3, [sp, #8]
00591660: bl       #0x30ed6c
00591664: ldr      r3, [sp, #8]
00591668: mov      r1, r0
0059166c: mov      r0, r3
00591670: bl       #0x30eba4
00591674: ldr      r1, [sp, #0x50]
00591678: mov      r3, r0
0059167c: mov      r0, sl
00591680: str      r3, [sp, #8]
00591684: bl       #0x30ed6c
00591688: ldr      r3, [sp, #8]
0059168c: mov      r1, r0
00591690: mov      r0, r3
00591694: bl       #0x30eba4
00591698: ldr      r1, [sp, #0x60]
0059169c: bl       #0x30eba4
005916a0: ldr      r1, [sp, #0x34]
005916a4: mov      r2, r0
005916a8: mov      r0, fp
005916ac: str      r2, [sp, #4]
005916b0: bl       #0x30ed6c
005916b4: ldr      r1, [sp, #0x44]
005916b8: mov      r3, r0
005916bc: mov      r0, sb
005916c0: str      r3, [sp, #8]
005916c4: bl       #0x30ed6c
005916c8: ldr      r3, [sp, #8]
005916cc: mov      r1, r0
005916d0: mov      r0, r3
005916d4: bl       #0x30eba4
005916d8: ldr      r1, [sp, #0x54]
005916dc: mov      r3, r0
005916e0: mov      r0, sl
005916e4: str      r3, [sp, #8]
005916e8: bl       #0x30ed6c
005916ec: ldr      r3, [sp, #8]
005916f0: mov      r1, r0
005916f4: mov      r0, r3
005916f8: bl       #0x30eba4
005916fc: ldr      r1, [sp, #0x64]
00591700: bl       #0x30eba4
00591704: ldr      r1, [sp, #0x2c]
00591708: mov      r3, r0
0059170c: mov      r0, fp
00591710: str      r3, [sp, #8]
00591714: bl       #0x30ed6c
00591718: ldr      r1, [sp, #0x3c]
0059171c: mov      fp, r0
00591720: mov      r0, sb
00591724: bl       #0x30ed6c
00591728: mov      r1, r0
0059172c: mov      r0, fp
00591730: bl       #0x30eba4
00591734: ldr      r1, [sp, #0x4c]
00591738: mov      sb, r0
0059173c: mov      r0, sl
00591740: bl       #0x30ed6c
00591744: mov      r1, r0
00591748: mov      r0, sb
0059174c: bl       #0x30eba4
00591750: ldr      r1, [sp, #0x5c]
00591754: bl       #0x30eba4
00591758: ldr      r1, [sp, #0x18]
0059175c: str      r0, [r1, r5]
00591760: ldr      r2, [sp, #4]
00591764: mov      r0, r8
00591768: add      r5, r5, #0x24
0059176c: str      r2, [r4, #4]
00591770: ldr      r3, [sp, #8]
00591774: str      r3, [r4, #8]
00591778: ldr      r1, [sp, #0x30]
0059177c: bl       #0x30ed6c
00591780: ldr      r1, [sp, #0x40]
00591784: mov      sl, r0
00591788: mov      r0, r7
0059178c: bl       #0x30ed6c
00591790: mov      r1, r0
00591794: mov      r0, sl
00591798: bl       #0x30eba4
0059179c: ldr      r1, [sp, #0x50]
005917a0: mov      sl, r0
005917a4: mov      r0, r6
005917a8: bl       #0x30ed6c
005917ac: mov      r1, r0
005917b0: mov      r0, sl
005917b4: bl       #0x30eba4
005917b8: ldr      r1, [sp, #0x60]
005917bc: bl       #0x30eba4
005917c0: ldr      r1, [sp, #0x34]
005917c4: mov      sb, r0
005917c8: mov      r0, r8
005917cc: bl       #0x30ed6c
005917d0: ldr      r1, [sp, #0x44]
005917d4: mov      sl, r0
005917d8: mov      r0, r7
005917dc: bl       #0x30ed6c
005917e0: mov      r1, r0
005917e4: mov      r0, sl
005917e8: bl       #0x30eba4
005917ec: ldr      r1, [sp, #0x54]
005917f0: mov      sl, r0
005917f4: mov      r0, r6
005917f8: bl       #0x30ed6c
005917fc: mov      r1, r0
00591800: mov      r0, sl
00591804: bl       #0x30eba4
00591808: ldr      r1, [sp, #0x64]
0059180c: bl       #0x30eba4
00591810: ldr      r1, [sp, #0x2c]
00591814: mov      sl, r0
00591818: mov      r0, r8
0059181c: bl       #0x30ed6c
00591820: ldr      r1, [sp, #0x3c]
00591824: mov      r8, r0
00591828: mov      r0, r7
0059182c: bl       #0x30ed6c
00591830: mov      r1, r0
00591834: mov      r0, r8
00591838: bl       #0x30eba4
0059183c: ldr      r1, [sp, #0x4c]
00591840: mov      r7, r0
00591844: mov      r0, r6
00591848: bl       #0x30ed6c
0059184c: mov      r1, r0
00591850: mov      r0, r7
00591854: bl       #0x30eba4
00591858: ldr      r1, [sp, #0x5c]
0059185c: bl       #0x30eba4
00591860: str      sb, [r4, #0x10]
00591864: str      r0, [r4, #0xc]
00591868: str      sl, [r4, #0x14]
0059186c: ldr      r1, [sp, #0x30]
00591870: ldr      r0, [sp, #0x14]
00591874: bl       #0x30ed6c
00591878: ldr      r1, [sp, #0x40]
0059187c: mov      r6, r0
00591880: ldr      r0, [sp, #0x10]
00591884: bl       #0x30ed6c
00591888: mov      r1, r0
0059188c: mov      r0, r6
00591890: bl       #0x30eba4
00591894: ldr      r1, [sp, #0x50]
00591898: mov      r6, r0
0059189c: ldr      r0, [sp, #0xc]
005918a0: bl       #0x30ed6c
005918a4: mov      r1, r0
005918a8: mov      r0, r6
005918ac: bl       #0x30eba4
005918b0: ldr      r1, [sp, #0x60]
005918b4: bl       #0x30eba4
005918b8: ldr      r1, [sp, #0x34]
005918bc: mov      r6, r0
005918c0: ldr      r0, [sp, #0x14]
005918c4: bl       #0x30ed6c
005918c8: ldr      r1, [sp, #0x44]
005918cc: mov      r7, r0
005918d0: ldr      r0, [sp, #0x10]
005918d4: bl       #0x30ed6c
005918d8: mov      r1, r0
005918dc: mov      r0, r7
005918e0: bl       #0x30eba4
005918e4: ldr      r1, [sp, #0x54]
005918e8: mov      r7, r0
005918ec: ldr      r0, [sp, #0xc]
005918f0: bl       #0x30ed6c
005918f4: mov      r1, r0
005918f8: mov      r0, r7
005918fc: bl       #0x30eba4
00591900: ldr      r1, [sp, #0x64]
00591904: bl       #0x30eba4
00591908: ldr      r1, [sp, #0x2c]
0059190c: mov      r7, r0
00591910: ldr      r0, [sp, #0x14]
00591914: bl       #0x30ed6c
00591918: ldr      r1, [sp, #0x3c]
0059191c: mov      r8, r0
00591920: ldr      r0, [sp, #0x10]
00591924: bl       #0x30ed6c
00591928: mov      r1, r0
0059192c: mov      r0, r8
00591930: bl       #0x30eba4
00591934: ldr      r1, [sp, #0x4c]
00591938: mov      r8, r0
0059193c: ldr      r0, [sp, #0xc]
00591940: bl       #0x30ed6c
00591944: mov      r1, r0
00591948: mov      r0, r8
0059194c: bl       #0x30eba4
00591950: ldr      r1, [sp, #0x5c]
00591954: bl       #0x30eba4
00591958: ldr      ip, [sp]
0059195c: ldr      r2, [sp, #0x1c]
00591960: str      r0, [r4, #0x18]
00591964: str      r7, [r4, #0x20]
00591968: cmp      ip, r2
0059196c: str      r6, [r4, #0x1c]
00591970: bne      #0x5915d4
00591974: b        #0x5915ac
00591978: mov      r0, r3
0059197c: ldr      r3, [r3]
00591980: str      ip, [sp]
00591984: mov      lr, pc
00591988: ldr      pc, [r3, #0x38]
0059198c: mov      r1, r0
00591990: mov      r0, r4
00591994: bl       #0x50f6d8
00591998: ldr      ip, [sp]
0059199c: b        #0x591520

# _ZN6glitch5scene17CTriangleSelectorC2ERKN5boost13intrusive_ptrIKNS0_5IMeshEEEPKNS0_10ISceneNodeEb
00596598: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0059659c: ldr      r8, [pc, #0x6bc]
005965a0: ldr      r6, [pc, #0x6bc]
005965a4: mov      lr, #0xbf000000
005965a8: add      r8, pc, r8
005965ac: ldr      r6, [r8, r6]
005965b0: mov      r4, r0
005965b4: mov      r5, #0x3f800000
005965b8: mov      ip, #0
005965bc: add      lr, lr, #0x800000
005965c0: add      r0, r6, #8
005965c4: mov      sl, #1
005965c8: mov      r6, #0
005965cc: sub      sp, sp, #0x64
005965d0: str      r2, [r4, #8]
005965d4: strb     r3, [r4, #0x18]
005965d8: str      r0, [r4]
005965dc: str      ip, [r4, #0x40]
005965e0: str      lr, [r4, #0x4c]
005965e4: mov      r7, r1
005965e8: str      ip, [r4, #0x1c]
005965ec: str      ip, [r4, #0x20]
005965f0: str      ip, [r4, #0x24]
005965f4: str      ip, [r4, #0x38]
005965f8: str      ip, [r4, #0x3c]
005965fc: str      lr, [r4, #0x44]
00596600: str      lr, [r4, #0x48]
00596604: mov      r1, r6
00596608: mov      r2, #0x40
0059660c: str      sl, [r4, #4]
00596610: str      r6, [r4, #0xc]
00596614: str      r6, [r4, #0x10]
00596618: str      r6, [r4, #0x14]
0059661c: str      r5, [r4, #0x28]
00596620: str      r5, [r4, #0x2c]
00596624: str      r5, [r4, #0x30]
00596628: str      r5, [r4, #0x50]
0059662c: str      r5, [r4, #0x54]
00596630: str      r5, [r4, #0x58]
00596634: strb     r6, [r4, #0x9c]
00596638: add      r0, r4, #0x5c
0059663c: bl       #0x30e460
00596640: str      r5, [r4, #0x98]
00596644: strb     sl, [r4, #0x9c]
00596648: str      r5, [r4, #0x5c]
0059664c: str      r5, [r4, #0x70]
00596650: str      r5, [r4, #0x84]
00596654: ldr      r3, [r7]
00596658: mov      r0, r3
0059665c: ldr      r3, [r3]
00596660: mov      lr, pc
00596664: ldr      pc, [r3, #0x10]
00596668: subs     r8, r0, #0
0059666c: beq      #0x596c50
00596670: mov      r5, r6
00596674: add      sl, sp, #0x5c
00596678: ldr      r3, [r7]
0059667c: mov      r2, r5
00596680: mov      r0, sl
00596684: mov      r1, r3
00596688: ldr      r3, [r3]
0059668c: mov      lr, pc
00596690: ldr      pc, [r3, #0x14]
00596694: ldr      r0, [sp, #0x5c]
00596698: add      r5, r5, #1
0059669c: ldr      r3, [r0, #0x20]
005966a0: add      r6, r6, r3
005966a4: bl       #0x31d584
005966a8: cmp      r5, r8
005966ac: bne      #0x596678
005966b0: movw     r3, #0xaaab
005966b4: movt     r3, #0xaaaa
005966b8: umull    r2, r6, r3, r6
005966bc: add      r3, r4, #0xc
005966c0: lsr      r1, r6, #1
005966c4: mov      r0, r3
005966c8: str      r3, [sp, #8]
005966cc: mov      r5, #0
005966d0: bl       #0x591060
005966d4: add      r2, sp, #0x54
005966d8: add      r3, sp, #0x10
005966dc: add      sl, sp, #0x58
005966e0: str      r2, [sp, #4]
005966e4: mov      r6, r5
005966e8: str      r3, [sp, #0xc]
005966ec: b        #0x5966fc
005966f0: add      r5, r5, #1
005966f4: cmp      r5, r8
005966f8: beq      #0x596808
005966fc: ldr      r3, [r7]
00596700: mov      r0, sl
00596704: mov      r2, r5
00596708: mov      r1, r3
0059670c: ldr      r3, [r3]
00596710: mov      lr, pc
00596714: ldr      pc, [r3, #0x14]
00596718: ldr      sb, [sp, #0x58]
0059671c: cmp      sb, #0
00596720: beq      #0x59672c
00596724: mov      r0, sb
00596728: bl       #0x31d584
0059672c: ldr      fp, [sb, #0x14]
00596730: cmp      fp, #0
00596734: str      fp, [sp, #0x54]
00596738: ldrne    r3, [fp]
0059673c: addne    r3, r3, #1
00596740: strne    r3, [fp]
00596744: ldr      r0, [sp, #4]
00596748: ldrne    fp, [sp, #0x54]
0059674c: bl       #0x35eb90
00596750: str      r6, [sp, #0x10]
00596754: str      r6, [sp, #0x14]
00596758: ldrh     r3, [sb, #0x2e]
0059675c: cmp      r3, #6
00596760: bne      #0x5966f0
00596764: ldr      r3, [sb, #0x18]
00596768: cmp      r3, #0
0059676c: moveq    r0, r6
00596770: beq      #0x596784
00596774: ldr      r0, [sp, #0xc]
00596778: add      r1, sb, #0x18
0059677c: bl       #0x5911a0
00596780: ldr      r0, [sp, #0x14]
00596784: add      r2, fp, #0x14
00596788: ldrh     r3, [r2, #0xa]
0059678c: ldr      r1, [sb, #0x20]
00596790: cmp      r3, #6
00596794: addls    pc, pc, r3, lsl #2
00596798: b        #0x5967c0
0059679c: b        #0x5967b8
005967a0: b        #0x59687c
005967a4: b        #0x596870
005967a8: b        #0x596864
005967ac: b        #0x596858
005967b0: b        #0x59684c
005967b4: b        #0x596840
005967b8: ldr      r3, [sp, #8]
005967bc: bl       #0x592e58
005967c0: ldr      r3, [sp, #0x14]
005967c4: cmp      r3, #0
005967c8: beq      #0x5966f0
005967cc: ldr      r3, [sp, #0x10]
005967d0: ldr      sb, [r3]
005967d4: ldrb     r3, [sb, #0x13]
005967d8: and      r2, r3, #0x1f
005967dc: cmp      r2, #1
005967e0: bls      #0x59682c
005967e4: sub      r2, r2, #1
005967e8: bic      r3, r3, #0x1f
005967ec: orr      r3, r2, r3
005967f0: strb     r3, [sb, #0x13]
005967f4: add      r5, r5, #1
005967f8: cmp      r5, r8
005967fc: str      r6, [sp, #0x10]
00596800: str      r6, [sp, #0x14]
00596804: bne      #0x5966fc
00596808: ldr      r3, [r4, #8]
0059680c: cmp      r3, #0
00596810: beq      #0x596820
00596814: ldrb     r2, [r4, #0x18]
00596818: cmp      r2, #0
0059681c: bne      #0x5968a0
00596820: mov      r0, r4
00596824: add      sp, sp, #0x64
00596828: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0059682c: ldrb     r3, [sb, #0x12]
00596830: tst      r3, #0x20
00596834: bne      #0x596888
00596838: strb     r6, [sb, #0x13]
0059683c: b        #0x5967f4
00596840: ldr      r3, [sp, #8]
00596844: bl       #0x591d38
00596848: b        #0x5967c0
0059684c: ldr      r3, [sp, #8]
00596850: bl       #0x5924a8
00596854: b        #0x5967c0
00596858: ldr      r3, [sp, #8]
0059685c: bl       #0x595518
00596860: b        #0x5967c0
00596864: ldr      r3, [sp, #8]
00596868: bl       #0x594b68
0059686c: b        #0x5967c0
00596870: ldr      r3, [sp, #8]
00596874: bl       #0x5941b8
00596878: b        #0x5967c0
0059687c: ldr      r3, [sp, #8]
00596880: bl       #0x593808
00596884: b        #0x5967c0
00596888: ldr      r3, [sb]
0059688c: mov      r0, sb
00596890: mov      lr, pc
00596894: ldr      pc, [r3, #0x18]
00596898: strb     r6, [sb, #0x13]
0059689c: b        #0x5967f4
005968a0: mov      r0, r3
005968a4: ldr      r3, [r3]
005968a8: mov      lr, pc
005968ac: ldr      pc, [r3, #0x38]
005968b0: mov      r1, r0
005968b4: add      r0, sp, #0x10
005968b8: bl       #0x591444
005968bc: ldr      r7, [r4, #0xc]
005968c0: ldr      r3, [r4, #0x10]
005968c4: rsb      r3, r7, r3
005968c8: asr      r3, r3, #2
005968cc: lsl      r2, r3, #3
005968d0: rsb      r2, r3, r2
005968d4: add      r2, r2, r2, lsl #6
005968d8: add      r2, r3, r2, lsl #3
005968dc: lsl      r1, r2, #0xf
005968e0: rsb      r2, r2, r1
005968e4: add      r2, r3, r2, lsl #3
005968e8: cmp      r2, #0
005968ec: str      r2, [sp, #8]
005968f0: ble      #0x596820
005968f4: mov      r5, #0
005968f8: str      r5, [sp, #4]
005968fc: b        #0x596904
00596900: ldr      r7, [r4, #0xc]
00596904: ldr      sb, [r7, r5]
00596908: ldr      r1, [sp, #0x14]
0059690c: add      r6, r7, r5
00596910: mov      r0, sb
00596914: bl       #0x30ed6c
00596918: ldr      sl, [r6, #4]
0059691c: mov      fp, r0
00596920: ldr      r1, [sp, #0x24]
00596924: mov      r0, sl
00596928: bl       #0x30ed6c
0059692c: mov      r1, r0
00596930: mov      r0, fp
00596934: bl       #0x30eba4
00596938: ldr      r8, [r6, #8]
0059693c: mov      fp, r0
00596940: ldr      r1, [sp, #0x34]
00596944: mov      r0, r8
00596948: bl       #0x30ed6c
0059694c: mov      r1, r0
00596950: mov      r0, fp
00596954: bl       #0x30eba4
00596958: ldr      r1, [sp, #0x44]
0059695c: bl       #0x30eba4
00596960: ldr      r1, [sp, #0x18]
00596964: mov      r3, r0
00596968: mov      r0, sb
0059696c: str      r3, [sp]
00596970: bl       #0x30ed6c
00596974: ldr      r1, [sp, #0x28]
00596978: mov      fp, r0
0059697c: mov      r0, sl
00596980: bl       #0x30ed6c
00596984: mov      r1, r0
00596988: mov      r0, fp
0059698c: bl       #0x30eba4
00596990: ldr      r1, [sp, #0x38]
00596994: mov      fp, r0
00596998: mov      r0, r8
0059699c: bl       #0x30ed6c
005969a0: mov      r1, r0
005969a4: mov      r0, fp
005969a8: bl       #0x30eba4
005969ac: ldr      r1, [sp, #0x48]
005969b0: bl       #0x30eba4
005969b4: ldr      r1, [sp, #0x10]
005969b8: mov      fp, r0
005969bc: mov      r0, sb
005969c0: bl       #0x30ed6c
005969c4: ldr      r1, [sp, #0x20]
005969c8: mov      sb, r0
005969cc: mov      r0, sl
005969d0: bl       #0x30ed6c
005969d4: mov      r1, r0
005969d8: mov      r0, sb
005969dc: bl       #0x30eba4
005969e0: ldr      r1, [sp, #0x30]
005969e4: mov      sl, r0
005969e8: mov      r0, r8
005969ec: bl       #0x30ed6c
005969f0: mov      r1, r0
005969f4: mov      r0, sl
005969f8: bl       #0x30eba4
005969fc: ldr      r1, [sp, #0x40]
00596a00: bl       #0x30eba4
00596a04: str      r0, [r7, r5]
00596a08: str      fp, [r6, #8]
00596a0c: ldr      r3, [sp]
00596a10: str      r3, [r6, #4]
00596a14: ldr      r2, [sp, #4]
00596a18: ldr      r6, [r4, #0xc]
00596a1c: ldr      r1, [sp, #0x14]
00596a20: add      r2, r2, #1
00596a24: str      r2, [sp, #4]
00596a28: add      r6, r6, r5
00596a2c: ldr      sl, [r6, #0xc]
00596a30: ldr      r8, [r6, #0x10]
00596a34: ldr      r7, [r6, #0x14]
00596a38: mov      r0, sl
00596a3c: bl       #0x30ed6c
00596a40: ldr      r1, [sp, #0x24]
00596a44: mov      sb, r0
00596a48: mov      r0, r8
00596a4c: bl       #0x30ed6c
00596a50: mov      r1, r0
00596a54: mov      r0, sb
00596a58: bl       #0x30eba4
00596a5c: ldr      r1, [sp, #0x34]
00596a60: mov      sb, r0
00596a64: mov      r0, r7
00596a68: bl       #0x30ed6c
00596a6c: mov      r1, r0
00596a70: mov      r0, sb
00596a74: bl       #0x30eba4
00596a78: ldr      r1, [sp, #0x44]
00596a7c: bl       #0x30eba4
00596a80: ldr      r1, [sp, #0x18]
00596a84: mov      sb, r0
00596a88: mov      r0, sl
00596a8c: bl       #0x30ed6c
00596a90: ldr      r1, [sp, #0x28]
00596a94: mov      fp, r0
00596a98: mov      r0, r8
00596a9c: bl       #0x30ed6c
00596aa0: mov      r1, r0
00596aa4: mov      r0, fp
00596aa8: bl       #0x30eba4
00596aac: ldr      r1, [sp, #0x38]
00596ab0: mov      fp, r0
00596ab4: mov      r0, r7
00596ab8: bl       #0x30ed6c
00596abc: mov      r1, r0
00596ac0: mov      r0, fp
00596ac4: bl       #0x30eba4
00596ac8: ldr      r1, [sp, #0x48]
00596acc: bl       #0x30eba4
00596ad0: ldr      r1, [sp, #0x10]
00596ad4: mov      fp, r0
00596ad8: mov      r0, sl
00596adc: bl       #0x30ed6c
00596ae0: ldr      r1, [sp, #0x20]
00596ae4: mov      sl, r0
00596ae8: mov      r0, r8
00596aec: bl       #0x30ed6c
00596af0: mov      r1, r0
00596af4: mov      r0, sl
00596af8: bl       #0x30eba4
00596afc: ldr      r1, [sp, #0x30]
00596b00: mov      r8, r0
00596b04: mov      r0, r7
00596b08: bl       #0x30ed6c
00596b0c: mov      r1, r0
00596b10: mov      r0, r8
00596b14: bl       #0x30eba4
00596b18: ldr      r1, [sp, #0x40]
00596b1c: bl       #0x30eba4
00596b20: str      r0, [r6, #0xc]
00596b24: str      fp, [r6, #0x14]
00596b28: str      sb, [r6, #0x10]
00596b2c: ldr      r6, [r4, #0xc]
00596b30: ldr      r1, [sp, #0x14]
00596b34: add      r6, r6, r5
00596b38: ldr      sl, [r6, #0x18]
00596b3c: ldr      r8, [r6, #0x1c]
00596b40: ldr      r7, [r6, #0x20]
00596b44: mov      r0, sl
00596b48: bl       #0x30ed6c
00596b4c: ldr      r1, [sp, #0x24]
00596b50: mov      sb, r0
00596b54: mov      r0, r8
00596b58: bl       #0x30ed6c
00596b5c: mov      r1, r0
00596b60: mov      r0, sb
00596b64: bl       #0x30eba4
00596b68: ldr      r1, [sp, #0x34]
00596b6c: mov      sb, r0
00596b70: mov      r0, r7
00596b74: bl       #0x30ed6c
00596b78: mov      r1, r0
00596b7c: mov      r0, sb
00596b80: bl       #0x30eba4
00596b84: ldr      r1, [sp, #0x44]
00596b88: bl       #0x30eba4
00596b8c: ldr      r1, [sp, #0x18]
00596b90: mov      sb, r0
00596b94: mov      r0, sl
00596b98: bl       #0x30ed6c
00596b9c: ldr      r1, [sp, #0x28]
00596ba0: mov      fp, r0
00596ba4: mov      r0, r8
00596ba8: bl       #0x30ed6c
00596bac: mov      r1, r0
00596bb0: mov      r0, fp
00596bb4: bl       #0x30eba4
00596bb8: ldr      r1, [sp, #0x38]
00596bbc: mov      fp, r0
00596bc0: mov      r0, r7
00596bc4: bl       #0x30ed6c
00596bc8: mov      r1, r0
00596bcc: mov      r0, fp
00596bd0: bl       #0x30eba4
00596bd4: ldr      r1, [sp, #0x48]
00596bd8: bl       #0x30eba4
00596bdc: ldr      r1, [sp, #0x10]
00596be0: mov      fp, r0
00596be4: mov      r0, sl
00596be8: bl       #0x30ed6c
00596bec: ldr      r1, [sp, #0x20]
00596bf0: mov      sl, r0
00596bf4: mov      r0, r8
00596bf8: bl       #0x30ed6c
00596bfc: mov      r1, r0
00596c00: mov      r0, sl
00596c04: bl       #0x30eba4
00596c08: ldr      r1, [sp, #0x30]
00596c0c: mov      r8, r0
00596c10: mov      r0, r7
00596c14: bl       #0x30ed6c
00596c18: mov      r1, r0
00596c1c: mov      r0, r8
00596c20: bl       #0x30eba4
00596c24: ldr      r1, [sp, #0x40]
00596c28: bl       #0x30eba4
00596c2c: ldr      r3, [sp, #4]
00596c30: ldr      r2, [sp, #8]
00596c34: add      r5, r5, #0x24
00596c38: str      r0, [r6, #0x18]
00596c3c: cmp      r3, r2
00596c40: str      fp, [r6, #0x20]
00596c44: str      sb, [r6, #0x1c]
00596c48: bne      #0x596900
00596c4c: b        #0x596820
00596c50: mov      r1, r8
00596c54: add      r0, r4, #0xc
00596c58: bl       #0x591060
00596c5c: b        #0x596808
00596c60: eorseq   lr, pc, r8, ror #9
00596c64: andeq    r0, r0, r0, asr #11

# _ZN5boost13intrusive_ptrIKN6glitch5video14CVertexStreamsEED1Ev
0035eb90: push     {r4, r5, r6, lr}
0035eb94: ldr      r4, [r0]
0035eb98: mov      r5, r0
0035eb9c: cmp      r4, #0
0035eba0: beq      #0x35ebc8
0035eba4: ldr      r3, [r4]
0035eba8: sub      r3, r3, #1
0035ebac: cmp      r3, #0
0035ebb0: str      r3, [r4]
0035ebb4: bne      #0x35ebc8
0035ebb8: mov      r0, r4
0035ebbc: bl       #0x5a0a1c
0035ebc0: mov      r0, r4
0035ebc4: bl       #0x310440
0035ebc8: mov      r0, r5
0035ebcc: pop      {r4, r5, r6, pc}
