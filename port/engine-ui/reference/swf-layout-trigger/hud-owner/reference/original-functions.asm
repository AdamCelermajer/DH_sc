
# _ZN12GameSWFUtils10MoveBtnPosEPN7gameswf9characterEff
0041684c: push     {r4, r5, r6, r7, r8, lr}
00416850: subs     r5, r0, #0
00416854: sub      sp, sp, #0x48
00416858: mov      r7, r1
0041685c: mov      r8, r2
00416860: moveq    r0, r5
00416864: beq      #0x4169a8
00416868: add      r6, sp, #0x30
0041686c: mov      r2, #0
00416870: add      r3, r6, #8
00416874: str      r2, [r3], #4
00416878: str      r2, [r3], #4
0041687c: str      r2, [r3], #4
00416880: add      r4, r5, #0x3c
00416884: str      r2, [r3]
00416888: mov      r1, #0x3f800000
0041688c: mov      r0, r4
00416890: str      r1, [sp, #0x40]
00416894: str      r2, [sp, #0x34]
00416898: str      r1, [sp, #0x30]
0041689c: bl       #0x386144
004168a0: ldr      r3, [r5, #0x40]
004168a4: cmp      r3, #0
004168a8: beq      #0x4168e0
004168ac: mov      r0, r4
004168b0: bl       #0x386144
004168b4: ldr      r0, [r5, #0x40]
004168b8: bl       #0x753f74
004168bc: add      ip, sp, #0x18
004168c0: mov      lr, r0
004168c4: ldm      lr!, {r0, r1, r2, r3}
004168c8: stm      ip!, {r0, r1, r2, r3}
004168cc: ldm      lr, {r0, r1}
004168d0: stm      ip, {r0, r1}
004168d4: add      r1, sp, #0x18
004168d8: mov      r0, r6
004168dc: bl       #0x795adc
004168e0: mov      r0, r5
004168e4: bl       #0x753f74
004168e8: mov      lr, sp
004168ec: mov      ip, r0
004168f0: ldm      ip!, {r0, r1, r2, r3}
004168f4: stm      lr!, {r0, r1, r2, r3}
004168f8: ldr      r0, [ip]
004168fc: mov      r1, #0x41000000
00416900: add      r1, r1, #0xa00000
00416904: str      r0, [lr]
00416908: mov      r0, r7
0041690c: bl       #0x30ed6c
00416910: mvn      r1, #0x800000
00416914: mov      r7, r0
00416918: bl       #0x30e4b4
0041691c: cmp      r0, #0
00416920: bne      #0x4169cc
00416924: mov      r7, #0
00416928: mov      r1, #0x41000000
0041692c: add      r1, r1, #0xa00000
00416930: mov      r0, r8
00416934: str      r7, [sp, #8]
00416938: bl       #0x30ed6c
0041693c: mvn      r1, #0x800000
00416940: mov      r7, r0
00416944: bl       #0x30e4b4
00416948: cmp      r0, #0
0041694c: bne      #0x4169b0
00416950: mov      r7, #0
00416954: mov      r0, r6
00416958: mov      r1, sp
0041695c: str      r7, [sp, #0x14]
00416960: bl       #0x4165b8
00416964: mov      r1, #0x41000000
00416968: add      r1, r1, #0xa00000
0041696c: ldr      r0, [sp, #0x38]
00416970: bl       #0x30ec94
00416974: bl       #0x30e4cc
00416978: mov      r1, #0x41000000
0041697c: add      r1, r1, #0xa00000
00416980: mov      r4, r0
00416984: ldr      r0, [sp, #0x44]
00416988: bl       #0x30ec94
0041698c: bl       #0x30e4cc
00416990: mov      r1, r5
00416994: mov      r3, r0
00416998: mov      r2, r4
0041699c: mov      r0, #0
004169a0: bl       #0x7aa3f0
004169a4: mov      r0, #1
004169a8: add      sp, sp, #0x48
004169ac: pop      {r4, r5, r6, r7, r8, pc}
004169b0: mvn      r1, #0x80000000
004169b4: mov      r0, r7
004169b8: sub      r1, r1, #0x800000
004169bc: bl       #0x30e9ac
004169c0: cmp      r0, #0
004169c4: bne      #0x416954
004169c8: b        #0x416950
004169cc: mvn      r1, #0x80000000
004169d0: mov      r0, r7
004169d4: sub      r1, r1, #0x800000
004169d8: bl       #0x30e9ac
004169dc: cmp      r0, #0
004169e0: bne      #0x416928
004169e4: b        #0x416924

# _ZN11HUDControls7OnEventERN8RenderFX5EventE
00418d28: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00418d2c: ldr      r8, [pc, #0xddc]
00418d30: ldr      sl, [pc, #0xddc]
00418d34: mov      r4, r1
00418d38: add      r8, pc, r8
00418d3c: ldr      r3, [r8, sl]
00418d40: mov      r1, #0
00418d44: sub      sp, sp, #0x44
00418d48: mov      r5, r0
00418d4c: mov      r2, r1
00418d50: ldr      r0, [r3, #0x40]
00418d54: bl       #0x36e478
00418d58: ldr      r7, [r0, #0x660]
00418d5c: bl       #0x4364f8
00418d60: ldrb     r6, [r5, #0x66c]
00418d64: mov      sb, r0
00418d68: cmp      r6, #0
00418d6c: beq      #0x418d9c
00418d70: ldr      r3, [r4, #8]
00418d74: cmp      r3, #4
00418d78: beq      #0x4190a4
00418d7c: cmp      r3, #5
00418d80: beq      #0x418fa8
00418d84: cmp      r3, #6
00418d88: beq      #0x418f40
00418d8c: mov      r3, #1
00418d90: strb     r3, [r4, #0x24]
00418d94: add      sp, sp, #0x44
00418d98: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00418d9c: add      sl, r5, #0x1c
00418da0: mov      r0, sl
00418da4: ldr      r8, [r4]
00418da8: bl       #0x427d50
00418dac: cmp      r8, r0
00418db0: beq      #0x41918c
00418db4: add      r0, r5, #0x88
00418db8: ldr      r7, [r4]
00418dbc: bl       #0x427d50
00418dc0: cmp      r7, r0
00418dc4: beq      #0x418f10
00418dc8: add      r0, r5, #0xe8
00418dcc: ldr      r6, [r4]
00418dd0: bl       #0x427d50
00418dd4: cmp      r6, r0
00418dd8: beq      #0x418ef8
00418ddc: add      r0, r5, #0x118
00418de0: ldr      r6, [r4]
00418de4: bl       #0x427d50
00418de8: cmp      r6, r0
00418dec: beq      #0x418ef8
00418df0: add      r0, r5, #0x148
00418df4: ldr      r6, [r4]
00418df8: bl       #0x427d50
00418dfc: cmp      r6, r0
00418e00: beq      #0x418ef8
00418e04: add      r0, r5, #0x178
00418e08: ldr      r6, [r4]
00418e0c: bl       #0x427d50
00418e10: cmp      r6, r0
00418e14: beq      #0x418ef8
00418e18: add      r0, r5, #0x1a8
00418e1c: ldr      r6, [r4]
00418e20: bl       #0x427d50
00418e24: cmp      r6, r0
00418e28: beq      #0x418ef8
00418e2c: add      r0, r5, #0x1d8
00418e30: ldr      r6, [r4]
00418e34: bl       #0x427d50
00418e38: cmp      r6, r0
00418e3c: beq      #0x418ef8
00418e40: add      r0, r5, #0x208
00418e44: ldr      r6, [r4]
00418e48: bl       #0x427d50
00418e4c: cmp      r6, r0
00418e50: beq      #0x418ef8
00418e54: add      r0, r5, #0x238
00418e58: ldr      r6, [r4]
00418e5c: bl       #0x427d50
00418e60: cmp      r6, r0
00418e64: beq      #0x418ef8
00418e68: add      r0, r5, #0x268
00418e6c: ldr      r6, [r4]
00418e70: bl       #0x427d50
00418e74: cmp      r6, r0
00418e78: beq      #0x418ef8
00418e7c: add      r0, r5, #0x298
00418e80: ldr      r6, [r4]
00418e84: bl       #0x427d50
00418e88: cmp      r6, r0
00418e8c: beq      #0x418ef8
00418e90: add      r0, r5, #0x2c8
00418e94: ldr      r6, [r4]
00418e98: bl       #0x427d50
00418e9c: cmp      r6, r0
00418ea0: beq      #0x418ef8
00418ea4: add      r0, r5, #0x2f8
00418ea8: ldr      r6, [r4]
00418eac: bl       #0x427d50
00418eb0: cmp      r6, r0
00418eb4: beq      #0x418ef8
00418eb8: add      r0, r5, #0x328
00418ebc: ldr      r6, [r4]
00418ec0: bl       #0x427d50
00418ec4: cmp      r6, r0
00418ec8: beq      #0x418ef8
00418ecc: add      r0, r5, #0x358
00418ed0: ldr      r4, [r4]
00418ed4: bl       #0x427d50
00418ed8: cmp      r4, r0
00418edc: beq      #0x418ef8
00418ee0: cmp      sb, #0
00418ee4: beq      #0x418d94
00418ee8: mov      r0, sb
00418eec: bl       #0x41f3f4
00418ef0: cmp      r0, #0
00418ef4: beq      #0x418d94
00418ef8: mvn      r3, #0
00418efc: mov      r2, #1
00418f00: str      r3, [r5, #0x80]
00418f04: strb     r2, [r5, #0x84]
00418f08: str      r3, [r5, #0x7c]
00418f0c: b        #0x418d94
00418f10: ldr      r3, [r4, #8]
00418f14: cmp      r3, #4
00418f18: moveq    r3, #1
00418f1c: strbeq   r3, [r5, #9]
00418f20: beq      #0x418d8c
00418f24: sub      r3, r3, #6
00418f28: cmp      r3, #1
00418f2c: mvnls    r3, #0
00418f30: strls    r3, [r5, #0x80]
00418f34: strbls   r6, [r5, #9]
00418f38: strls    r3, [r5, #0x7c]
00418f3c: b        #0x418d8c
00418f40: add      r6, r5, #0x5c0
00418f44: add      r6, r6, #8
00418f48: mov      r0, r6
00418f4c: ldr      r7, [r4]
00418f50: bl       #0x427d50
00418f54: cmp      r7, r0
00418f58: beq      #0x4195b4
00418f5c: add      r0, r5, #0x590
00418f60: add      r0, r0, #8
00418f64: ldr      r6, [r4]
00418f68: bl       #0x427d50
00418f6c: cmp      r6, r0
00418f70: bne      #0x418d8c
00418f74: ldr      r1, [pc, #0xb9c]
00418f78: ldr      r0, [r8, sl]
00418f7c: add      r1, pc, r1
00418f80: bl       #0x320e44
00418f84: cmp      r0, #0
00418f88: addeq    r1, r5, #0x670
00418f8c: addne    r1, r5, #0x6c0
00418f90: mov      r0, r5
00418f94: addeq    r1, r1, #4
00418f98: addne    r1, r1, #0xc
00418f9c: mov      r2, #1
00418fa0: bl       #0x4187a0
00418fa4: b        #0x418d8c
00418fa8: add      r0, r5, #0x1c
00418fac: ldr      r6, [r4]
00418fb0: bl       #0x427d50
00418fb4: cmp      r6, r0
00418fb8: beq      #0x4196f4
00418fbc: add      sb, r5, #0x88
00418fc0: mov      r0, sb
00418fc4: ldr      r6, [r4]
00418fc8: bl       #0x427d50
00418fcc: cmp      r6, r0
00418fd0: beq      #0x419564
00418fd4: add      r3, r5, #0x3e8
00418fd8: str      r3, [sp, #0x10]
00418fdc: mov      r0, r3
00418fe0: ldr      r6, [r4]
00418fe4: bl       #0x427d50
00418fe8: cmp      r6, r0
00418fec: beq      #0x41920c
00418ff0: add      r6, r5, #0x410
00418ff4: add      r6, r6, #8
00418ff8: mov      r0, r6
00418ffc: ldr      r7, [r4]
00419000: bl       #0x427d50
00419004: cmp      r7, r0
00419008: addeq    r7, r5, #0x440
0041900c: addeq    r7, r7, #8
00419010: beq      #0x41921c
00419014: add      r7, r5, #0x440
00419018: add      r7, r7, #8
0041901c: mov      r0, r7
00419020: ldr      fp, [r4]
00419024: bl       #0x427d50
00419028: cmp      fp, r0
0041902c: beq      #0x41921c
00419030: add      r6, r5, #0x3b8
00419034: mov      r0, r6
00419038: ldr      r7, [r4]
0041903c: bl       #0x427d50
00419040: cmp      r7, r0
00419044: beq      #0x419980
00419048: add      r6, r5, #0x470
0041904c: add      r6, r6, #8
00419050: mov      r0, r6
00419054: ldr      r7, [r4]
00419058: bl       #0x427d50
0041905c: cmp      r7, r0
00419060: beq      #0x419a18
00419064: add      r6, r5, #0x4a0
00419068: add      r6, r6, #8
0041906c: mov      r0, r6
00419070: ldr      r7, [r4]
00419074: bl       #0x427d50
00419078: cmp      r7, r0
0041907c: beq      #0x419ae8
00419080: add      r6, r5, #0x500
00419084: add      r6, r6, #8
00419088: mov      r0, r6
0041908c: ldr      r7, [r4]
00419090: bl       #0x427d50
00419094: cmp      r7, r0
00419098: beq      #0x4194cc
0041909c: ldr      r3, [r4, #8]
004190a0: b        #0x418d84
004190a4: add      r0, r5, #0x1c
004190a8: ldr      r6, [r4]
004190ac: bl       #0x427d50
004190b0: cmp      r6, r0
004190b4: beq      #0x4191ec
004190b8: add      r0, r5, #0x88
004190bc: ldr      r6, [r4]
004190c0: bl       #0x427d50
004190c4: cmp      r6, r0
004190c8: beq      #0x4191ec
004190cc: add      r0, r5, #0x3b8
004190d0: ldr      r6, [r4]
004190d4: bl       #0x427d50
004190d8: cmp      r6, r0
004190dc: beq      #0x4191ec
004190e0: add      r0, r5, #0x3e8
004190e4: ldr      r6, [r4]
004190e8: bl       #0x427d50
004190ec: cmp      r6, r0
004190f0: beq      #0x4191ec
004190f4: add      r0, r5, #0x410
004190f8: add      r0, r0, #8
004190fc: ldr      r6, [r4]
00419100: bl       #0x427d50
00419104: cmp      r6, r0
00419108: beq      #0x4191ec
0041910c: add      r0, r5, #0x440
00419110: add      r0, r0, #8
00419114: ldr      r6, [r4]
00419118: bl       #0x427d50
0041911c: cmp      r6, r0
00419120: beq      #0x4191ec
00419124: add      r0, r5, #0x470
00419128: add      r0, r0, #8
0041912c: ldr      r6, [r4]
00419130: bl       #0x427d50
00419134: cmp      r6, r0
00419138: beq      #0x4191ec
0041913c: add      r0, r5, #0x4a0
00419140: add      r0, r0, #8
00419144: ldr      r6, [r4]
00419148: bl       #0x427d50
0041914c: cmp      r6, r0
00419150: beq      #0x4191ec
00419154: add      r0, r5, #0x500
00419158: add      r0, r0, #8
0041915c: ldr      r6, [r4]
00419160: bl       #0x427d50
00419164: cmp      r6, r0
00419168: beq      #0x4191ec
0041916c: add      r0, r5, #0x530
00419170: add      r0, r0, #8
00419174: ldr      r6, [r4]
00419178: bl       #0x427d50
0041917c: cmp      r6, r0
00419180: beq      #0x4191ec
00419184: ldr      r3, [r4, #8]
00419188: b        #0x418d7c
0041918c: ldr      r3, [r4, #8]
00419190: cmp      r3, #4
00419194: beq      #0x419580
00419198: cmp      r3, #5
0041919c: beq      #0x419764
004191a0: sub      r3, r3, #6
004191a4: cmp      r3, #1
004191a8: bhi      #0x418d8c
004191ac: str      r6, [r5, #0x14]
004191b0: str      r6, [r5, #0x18]
004191b4: mov      r0, sl
004191b8: ldr      r8, [r5, #0x658]
004191bc: bl       #0x427d50
004191c0: mov      r2, r6
004191c4: mov      r1, r0
004191c8: mov      r3, r6
004191cc: mov      r0, r8
004191d0: bl       #0x7aa3f0
004191d4: cmp      r7, #0
004191d8: strb     r6, [r5, #0xa]
004191dc: beq      #0x418d8c
004191e0: ldr      r0, [r7, #0x378]
004191e4: bl       #0x40559c
004191e8: b        #0x418d8c
004191ec: ldr      r3, [pc, #0x928]
004191f0: ldr      r2, [r4, #0xc]
004191f4: add      r3, pc, r3
004191f8: str      r2, [r3]
004191fc: ldr      r2, [r4, #0x10]
00419200: str      r2, [r3, #4]
00419204: ldr      r3, [r4, #8]
00419208: b        #0x418d7c
0041920c: add      r6, r5, #0x410
00419210: add      r7, r5, #0x440
00419214: add      r6, r6, #8
00419218: add      r7, r7, #8
0041921c: mov      r0, sb
00419220: bl       #0x427d50
00419224: bl       #0x753f74
00419228: ldr      fp, [pc, #0x8f0]
0041922c: ldr      r1, [r0, #0x14]
00419230: ldr      r0, [r4, #0x10]
00419234: add      fp, pc, fp
00419238: bl       #0x30eba4
0041923c: ldr      r1, [fp, #4]
00419240: bl       #0x30e3ac
00419244: mov      r1, #0x41000000
00419248: add      r1, r1, #0xa00000
0041924c: bl       #0x30ec94
00419250: bl       #0x30e4cc
00419254: str      r0, [sp, #0x18]
00419258: mov      r0, sb
0041925c: bl       #0x427d50
00419260: bl       #0x753f74
00419264: ldr      r1, [r0, #8]
00419268: ldr      r0, [r4, #0xc]
0041926c: bl       #0x30eba4
00419270: ldr      r1, [fp]
00419274: bl       #0x30e3ac
00419278: mov      r1, #0x41000000
0041927c: add      r1, r1, #0xa00000
00419280: bl       #0x30ec94
00419284: bl       #0x30e4cc
00419288: ldr      r1, [pc, #0x894]
0041928c: str      r0, [sp, #0x14]
00419290: ldr      r0, [r8, sl]
00419294: add      r1, pc, r1
00419298: bl       #0x320e44
0041929c: cmp      r0, #0
004192a0: bne      #0x419644
004192a4: ldr      fp, [r5, #0x67c]
004192a8: ldr      r1, [r5, #0x68c]
004192ac: mov      r0, fp
004192b0: bl       #0x30e3ac
004192b4: mov      r1, #0x41000000
004192b8: add      r1, r1, #0xa00000
004192bc: bl       #0x30ec94
004192c0: bl       #0x30e4cc
004192c4: str      r0, [sp, #0x2c]
004192c8: ldr      r1, [r5, #0x690]
004192cc: ldr      r0, [r5, #0x680]
004192d0: bl       #0x30e3ac
004192d4: mov      r1, #0x41000000
004192d8: add      r1, r1, #0xa00000
004192dc: bl       #0x30ec94
004192e0: bl       #0x30e4cc
004192e4: str      r0, [sp, #0x28]
004192e8: ldr      r1, [r5, #0x694]
004192ec: mov      r0, fp
004192f0: bl       #0x30e3ac
004192f4: mov      r1, #0x41000000
004192f8: add      r1, r1, #0xa00000
004192fc: bl       #0x30ec94
00419300: bl       #0x30e4cc
00419304: str      r0, [sp, #0x24]
00419308: ldr      r1, [r5, #0x698]
0041930c: ldr      r0, [r5, #0x680]
00419310: bl       #0x30e3ac
00419314: mov      r1, #0x41000000
00419318: add      r1, r1, #0xa00000
0041931c: bl       #0x30ec94
00419320: bl       #0x30e4cc
00419324: str      r0, [sp, #0x20]
00419328: ldr      r1, [r5, #0x69c]
0041932c: mov      r0, fp
00419330: bl       #0x30e3ac
00419334: mov      r1, #0x41000000
00419338: add      r1, r1, #0xa00000
0041933c: bl       #0x30ec94
00419340: bl       #0x30e4cc
00419344: str      r0, [sp, #0x1c]
00419348: ldr      r0, [r5, #0x680]
0041934c: ldr      r1, [r5, #0x6a0]
00419350: bl       #0x30e3ac
00419354: mov      r1, #0x41000000
00419358: add      r1, r1, #0xa00000
0041935c: bl       #0x30ec94
00419360: bl       #0x30e4cc
00419364: str      r0, [sp, #0xc]
00419368: mov      r0, sb
0041936c: bl       #0x427d50
00419370: mov      r3, r0
00419374: mov      r0, sb
00419378: str      r3, [sp, #4]
0041937c: bl       #0x427d50
00419380: bl       #0x753f74
00419384: ldr      fp, [pc, #0x79c]
00419388: ldr      r1, [r0, #8]
0041938c: ldr      r0, [r4, #0xc]
00419390: add      fp, pc, fp
00419394: bl       #0x30eba4
00419398: ldr      r1, [fp]
0041939c: bl       #0x30e3ac
004193a0: mov      r1, #0x41000000
004193a4: add      r1, r1, #0xa00000
004193a8: bl       #0x30ec94
004193ac: bl       #0x30e4cc
004193b0: bl       #0x30e964
004193b4: mov      ip, r0
004193b8: mov      r0, sb
004193bc: str      ip, [sp, #8]
004193c0: bl       #0x427d50
004193c4: bl       #0x753f74
004193c8: ldr      r1, [r0, #0x14]
004193cc: ldr      r0, [r4, #0x10]
004193d0: bl       #0x30eba4
004193d4: ldr      r1, [fp, #4]
004193d8: bl       #0x30e3ac
004193dc: mov      r1, #0x41000000
004193e0: add      r1, r1, #0xa00000
004193e4: bl       #0x30ec94
004193e8: bl       #0x30e4cc
004193ec: bl       #0x30e964
004193f0: ldmib    sp, {r3, ip}
004193f4: mov      r2, r0
004193f8: mov      r1, ip
004193fc: mov      r0, r3
00419400: bl       #0x41684c
00419404: ldr      r0, [sp, #0x10]
00419408: bl       #0x427d50
0041940c: ldr      r2, [sp, #0x14]
00419410: ldr      r3, [sp, #0x2c]
00419414: mov      sb, r0
00419418: rsb      r0, r3, r2
0041941c: bl       #0x30e964
00419420: ldr      r3, [sp, #0x28]
00419424: ldr      r2, [sp, #0x18]
00419428: mov      fp, r0
0041942c: rsb      r0, r3, r2
00419430: bl       #0x30e964
00419434: mov      r1, fp
00419438: mov      r2, r0
0041943c: mov      r0, sb
00419440: bl       #0x41684c
00419444: mov      r0, r6
00419448: bl       #0x427d50
0041944c: ldr      r2, [sp, #0x14]
00419450: ldr      r3, [sp, #0x24]
00419454: mov      sb, r0
00419458: rsb      r0, r3, r2
0041945c: bl       #0x30e964
00419460: ldr      r3, [sp, #0x20]
00419464: ldr      r2, [sp, #0x18]
00419468: mov      r6, r0
0041946c: rsb      r0, r3, r2
00419470: bl       #0x30e964
00419474: mov      r1, r6
00419478: mov      r2, r0
0041947c: mov      r0, sb
00419480: bl       #0x41684c
00419484: mov      r0, r7
00419488: bl       #0x427d50
0041948c: ldr      r2, [sp, #0x14]
00419490: ldr      r3, [sp, #0x1c]
00419494: mov      r6, r0
00419498: rsb      r0, r3, r2
0041949c: bl       #0x30e964
004194a0: ldr      r2, [sp, #0x18]
004194a4: ldr      r3, [sp, #0xc]
004194a8: mov      r7, r0
004194ac: rsb      r0, r3, r2
004194b0: bl       #0x30e964
004194b4: mov      r1, r7
004194b8: mov      r2, r0
004194bc: mov      r0, r6
004194c0: bl       #0x41684c
004194c4: ldr      r3, [r4, #8]
004194c8: b        #0x418d84
004194cc: mov      r0, r6
004194d0: bl       #0x427d50
004194d4: mov      fp, r0
004194d8: mov      r0, r6
004194dc: bl       #0x427d50
004194e0: bl       #0x753f74
004194e4: ldr      r7, [pc, #0x640]
004194e8: ldr      r1, [r0, #8]
004194ec: ldr      r0, [r4, #0xc]
004194f0: add      r7, pc, r7
004194f4: bl       #0x30eba4
004194f8: ldr      r1, [r7]
004194fc: bl       #0x30e3ac
00419500: mov      r1, #0x41000000
00419504: add      r1, r1, #0xa00000
00419508: bl       #0x30ec94
0041950c: bl       #0x30e4cc
00419510: bl       #0x30e964
00419514: mov      sb, r0
00419518: mov      r0, r6
0041951c: bl       #0x427d50
00419520: bl       #0x753f74
00419524: ldr      r1, [r0, #0x14]
00419528: ldr      r0, [r4, #0x10]
0041952c: bl       #0x30eba4
00419530: ldr      r1, [r7, #4]
00419534: bl       #0x30e3ac
00419538: mov      r1, #0x41000000
0041953c: add      r1, r1, #0xa00000
00419540: bl       #0x30ec94
00419544: bl       #0x30e4cc
00419548: bl       #0x30e964
0041954c: mov      r1, sb
00419550: mov      r2, r0
00419554: mov      r0, fp
00419558: bl       #0x41684c
0041955c: ldr      r3, [r4, #8]
00419560: b        #0x418d84
00419564: add      r6, r5, #0x410
00419568: add      r7, r5, #0x440
0041956c: add      r2, r5, #0x3e8
00419570: add      r6, r6, #8
00419574: add      r7, r7, #8
00419578: str      r2, [sp, #0x10]
0041957c: b        #0x41921c
00419580: mov      r1, #0x41000000
00419584: ldr      r0, [r4, #0xc]
00419588: add      r1, r1, #0xa00000
0041958c: bl       #0x30ec94
00419590: bl       #0x30e4cc
00419594: mov      r1, #0x41000000
00419598: str      r0, [r5, #0x14]
0041959c: ldr      r0, [r4, #0x10]
004195a0: add      r1, r1, #0xa00000
004195a4: bl       #0x30ec94
004195a8: bl       #0x30e4cc
004195ac: str      r0, [r5, #0x18]
004195b0: b        #0x418d8c
004195b4: mov      r7, #0
004195b8: add      r0, r5, #0x590
004195bc: strb     r7, [r5, #0x66c]
004195c0: add      r0, r0, #8
004195c4: bl       #0x427d50
004195c8: strb     r7, [r0, #0x9b]
004195cc: mov      r0, r6
004195d0: bl       #0x427d50
004195d4: strb     r7, [r0, #0x9b]
004195d8: mov      r0, r5
004195dc: bl       #0x418360
004195e0: ldr      r1, [pc, #0x548]
004195e4: ldr      r0, [r8, sl]
004195e8: add      r1, pc, r1
004195ec: bl       #0x320e44
004195f0: mov      r3, #1
004195f4: cmp      r0, r7
004195f8: strbeq   r3, [r5, #0x66f]
004195fc: strbne   r3, [r5, #0x670]
00419600: ldr      r0, [r8, sl]
00419604: bl       #0x31f594
00419608: cmp      r0, #0
0041960c: movne    r3, #0
00419610: strbne   r3, [r0, #0x1a8]
00419614: bl       #0x42ca8c
00419618: ldr      r1, [pc, #0x514]
0041961c: add      r1, pc, r1
00419620: bl       #0x42d1f0
00419624: ldr      r1, [pc, #0x50c]
00419628: ldr      r3, [r8, sl]
0041962c: mov      r2, #0
00419630: ldr      r1, [r8, r1]
00419634: str      r0, [r1, #0xc]
00419638: ldr      r0, [r3, #0x18]
0041963c: bl       #0x33a344
00419640: b        #0x418d8c
00419644: ldr      fp, [r5, #0x6d4]
00419648: ldr      r1, [r5, #0x6e4]
0041964c: mov      r0, fp
00419650: bl       #0x30e3ac
00419654: mov      r1, #0x41000000
00419658: add      r1, r1, #0xa00000
0041965c: bl       #0x30ec94
00419660: bl       #0x30e4cc
00419664: str      r0, [sp, #0x2c]
00419668: ldr      r1, [r5, #0x6e8]
0041966c: ldr      r0, [r5, #0x6d8]
00419670: bl       #0x30e3ac
00419674: mov      r1, #0x41000000
00419678: add      r1, r1, #0xa00000
0041967c: bl       #0x30ec94
00419680: bl       #0x30e4cc
00419684: str      r0, [sp, #0x28]
00419688: ldr      r1, [r5, #0x6ec]
0041968c: mov      r0, fp
00419690: bl       #0x30e3ac
00419694: mov      r1, #0x41000000
00419698: add      r1, r1, #0xa00000
0041969c: bl       #0x30ec94
004196a0: bl       #0x30e4cc
004196a4: str      r0, [sp, #0x24]
004196a8: ldr      r1, [r5, #0x6f0]
004196ac: ldr      r0, [r5, #0x6d8]
004196b0: bl       #0x30e3ac
004196b4: mov      r1, #0x41000000
004196b8: add      r1, r1, #0xa00000
004196bc: bl       #0x30ec94
004196c0: bl       #0x30e4cc
004196c4: str      r0, [sp, #0x20]
004196c8: ldr      r1, [r5, #0x6f4]
004196cc: mov      r0, fp
004196d0: bl       #0x30e3ac
004196d4: mov      r1, #0x41000000
004196d8: add      r1, r1, #0xa00000
004196dc: bl       #0x30ec94
004196e0: bl       #0x30e4cc
004196e4: str      r0, [sp, #0x1c]
004196e8: ldr      r0, [r5, #0x6d8]
004196ec: ldr      r1, [r5, #0x6f8]
004196f0: b        #0x419350
004196f4: add      r7, r5, #0x4c
004196f8: mov      r0, r7
004196fc: bl       #0x427d50
00419700: mov      fp, r0
00419704: mov      r0, r7
00419708: bl       #0x427d50
0041970c: bl       #0x753f74
00419710: ldr      r6, [pc, #0x424]
00419714: ldr      r1, [r0, #8]
00419718: ldr      r0, [r4, #0xc]
0041971c: add      r6, pc, r6
00419720: bl       #0x30eba4
00419724: ldr      r1, [r6]
00419728: bl       #0x30e3ac
0041972c: mov      r1, #0x41000000
00419730: add      r1, r1, #0xa00000
00419734: bl       #0x30ec94
00419738: bl       #0x30e4cc
0041973c: bl       #0x30e964
00419740: mov      sb, r0
00419744: mov      r0, r7
00419748: bl       #0x427d50
0041974c: bl       #0x753f74
00419750: ldr      r1, [r0, #0x14]
00419754: ldr      r0, [r4, #0x10]
00419758: bl       #0x30eba4
0041975c: ldr      r1, [r6, #4]
00419760: b        #0x419534
00419764: ldr      r3, [r4]
00419768: ldr      r0, [r4, #0xc]
0041976c: ldr      r6, [r3, #0x4c]
00419770: ldr      r1, [r6, #8]
00419774: bl       #0x30eba4
00419778: mov      r1, #0x41000000
0041977c: add      r1, r1, #0xa00000
00419780: bl       #0x30ec94
00419784: mov      r8, r0
00419788: ldr      r0, [r5, #0x14]
0041978c: bl       #0x30e964
00419790: mov      r1, r0
00419794: mov      r0, r8
00419798: bl       #0x30e3ac
0041979c: bl       #0x30e4cc
004197a0: ldr      r1, [r6, #0x14]
004197a4: mov      r8, r0
004197a8: ldr      r0, [r4, #0x10]
004197ac: bl       #0x30eba4
004197b0: mov      r1, #0x41000000
004197b4: add      r1, r1, #0xa00000
004197b8: bl       #0x30ec94
004197bc: mov      r6, r0
004197c0: ldr      r0, [r5, #0x18]
004197c4: bl       #0x30e964
004197c8: mov      r1, r0
004197cc: mov      r0, r6
004197d0: bl       #0x30e3ac
004197d4: bl       #0x30e4cc
004197d8: mov      fp, r0
004197dc: bl       #0x30e964
004197e0: mov      r6, r0
004197e4: mov      r0, r8
004197e8: bl       #0x30e964
004197ec: mov      r1, r0
004197f0: mov      r0, r6
004197f4: bl       #0x30ddb8
004197f8: ldr      r3, [r5, #0xc]
004197fc: mov      r6, r0
00419800: rsb      sb, r3, #0
00419804: cmp      r8, sb
00419808: blt      #0x419818
0041980c: cmp      r8, r3
00419810: movlt    sb, r8
00419814: movge    sb, r3
00419818: ldr      r3, [r5, #0x10]
0041981c: rsb      r8, r3, #0
00419820: cmp      fp, r8
00419824: blt      #0x419834
00419828: cmp      fp, r3
0041982c: movlt    r8, fp
00419830: movge    r8, r3
00419834: mov      r0, sb
00419838: bl       #0x30e964
0041983c: mov      r1, r0
00419840: bl       #0x30ed6c
00419844: mov      fp, r0
00419848: mul      r0, r8, r8
0041984c: bl       #0x30e964
00419850: mov      r1, r0
00419854: mov      r0, fp
00419858: bl       #0x30eba4
0041985c: bl       #0x30e124
00419860: mov      r3, r0
00419864: ldr      r0, [r5, #0xc]
00419868: str      r3, [sp, #4]
0041986c: bl       #0x30e964
00419870: ldr      r3, [sp, #4]
00419874: mov      fp, r0
00419878: mov      r1, fp
0041987c: mov      r0, r3
00419880: bl       #0x30ec94
00419884: mov      r1, #0x3f800000
00419888: str      r0, [r5, #0x668]
0041988c: bl       #0x30e2f8
00419890: cmp      r0, #0
00419894: beq      #0x4198d8
00419898: mov      r0, r6
0041989c: bl       #0x30eb08
004198a0: mov      r8, r0
004198a4: ldr      r0, [r5, #0x10]
004198a8: bl       #0x30e964
004198ac: mov      r1, r8
004198b0: bl       #0x30ed6c
004198b4: bl       #0x30e4cc
004198b8: mov      r8, r0
004198bc: mov      r0, r6
004198c0: bl       #0x30e754
004198c4: mov      r1, r0
004198c8: mov      r0, fp
004198cc: bl       #0x30ed6c
004198d0: bl       #0x30e4cc
004198d4: mov      sb, r0
004198d8: mov      r0, sl
004198dc: ldr      sl, [r5, #0x658]
004198e0: bl       #0x427d50
004198e4: mov      r2, sb
004198e8: mov      r1, r0
004198ec: mov      r3, r8
004198f0: mov      r0, sl
004198f4: bl       #0x7aa3f0
004198f8: cmp      r7, #0
004198fc: beq      #0x418d8c
00419900: mov      r0, r7
00419904: bl       #0x3ad430
00419908: cmp      r0, #0
0041990c: beq      #0x418d8c
00419910: mov      r3, #0x3f800000
00419914: str      r3, [r5, #0x65c]
00419918: add      r8, r5, #0x650
0041991c: mov      r3, #0xbf000000
00419920: add      r3, r3, #0x800000
00419924: mov      r7, #0
00419928: add      r8, r8, #0xc
0041992c: str      r3, [r5, #0x660]
00419930: str      r7, [r5, #0x664]
00419934: mov      r0, r8
00419938: bl       #0x34d0b0
0041993c: movw     r1, #0x2ee0
00419940: movt     r1, #0xc265
00419944: mov      r0, r6
00419948: bl       #0x30ed6c
0041994c: mov      r1, #0x42000000
00419950: add      r1, r1, #0xb40000
00419954: bl       #0x30eba4
00419958: add      r2, sp, #0x34
0041995c: mov      r1, r0
00419960: mov      r0, r8
00419964: str      r7, [sp, #0x3c]
00419968: str      r7, [sp, #0x34]
0041996c: str      r7, [sp, #0x38]
00419970: bl       #0x418c48
00419974: mov      r3, #1
00419978: strb     r3, [r5, #0xa]
0041997c: b        #0x418d8c
00419980: mov      r0, r6
00419984: bl       #0x427d50
00419988: mov      fp, r0
0041998c: mov      r0, r6
00419990: bl       #0x427d50
00419994: bl       #0x753f74
00419998: ldr      r7, [pc, #0x1a0]
0041999c: ldr      r1, [r0, #8]
004199a0: add      r7, pc, r7
004199a4: ldr      r0, [r4, #0xc]
004199a8: bl       #0x30eba4
004199ac: ldr      r1, [r7]
004199b0: bl       #0x30e3ac
004199b4: mov      r1, #0x41000000
004199b8: add      r1, r1, #0xa00000
004199bc: bl       #0x30ec94
004199c0: bl       #0x30e4cc
004199c4: bl       #0x30e964
004199c8: mov      sb, r0
004199cc: mov      r0, r6
004199d0: bl       #0x427d50
004199d4: bl       #0x753f74
004199d8: ldr      r1, [r0, #0x14]
004199dc: ldr      r0, [r4, #0x10]
004199e0: bl       #0x30eba4
004199e4: ldr      r1, [r7, #4]
004199e8: bl       #0x30e3ac
004199ec: mov      r1, #0x41000000
004199f0: add      r1, r1, #0xa00000
004199f4: bl       #0x30ec94
004199f8: bl       #0x30e4cc
004199fc: bl       #0x30e964
00419a00: mov      r1, sb
00419a04: mov      r2, r0
00419a08: mov      r0, fp
00419a0c: bl       #0x41684c
00419a10: ldr      r3, [r4, #8]
00419a14: b        #0x418d84
00419a18: mov      r0, r6
00419a1c: bl       #0x427d50
00419a20: bl       #0x753f74
00419a24: ldr      r7, [pc, #0x118]
00419a28: ldr      r1, [r0, #0x14]
00419a2c: ldr      r0, [r4, #0x10]
00419a30: add      r7, pc, r7
00419a34: bl       #0x30eba4
00419a38: ldr      r1, [r7, #4]
00419a3c: bl       #0x30e3ac
00419a40: mov      r1, #0x41000000
00419a44: add      r1, r1, #0xa00000
00419a48: bl       #0x30ec94
00419a4c: bl       #0x30e4cc
00419a50: mov      sb, r0
00419a54: mov      r0, r6
00419a58: bl       #0x427d50
00419a5c: bl       #0x753f74
00419a60: ldr      r1, [r0, #8]
00419a64: ldr      r0, [r4, #0xc]
00419a68: bl       #0x30eba4
00419a6c: ldr      r1, [r7]
00419a70: bl       #0x30e3ac
00419a74: mov      r1, #0x41000000
00419a78: add      r1, r1, #0xa00000
00419a7c: bl       #0x30ec94
00419a80: bl       #0x30e4cc
00419a84: mov      r7, r0
00419a88: add      r0, r5, #0x4d0
00419a8c: add      r0, r0, #8
00419a90: bl       #0x427d50
00419a94: mov      fp, r0
00419a98: mov      r0, sb
00419a9c: bl       #0x30e964
00419aa0: mov      sb, r0
00419aa4: sub      r0, r7, #0x2b
00419aa8: bl       #0x30e964
00419aac: mov      r2, sb
00419ab0: mov      r1, r0
00419ab4: mov      r0, fp
00419ab8: bl       #0x41684c
00419abc: mov      r0, r6
00419ac0: bl       #0x427d50
00419ac4: mov      r6, r0
00419ac8: mov      r0, r7
00419acc: bl       #0x30e964
00419ad0: mov      r2, sb
00419ad4: mov      r1, r0
00419ad8: mov      r0, r6
00419adc: bl       #0x41684c
00419ae0: ldr      r3, [r4, #8]
00419ae4: b        #0x418d84
00419ae8: mov      r0, r6
00419aec: bl       #0x427d50
00419af0: mov      fp, r0
00419af4: mov      r0, r6
00419af8: bl       #0x427d50
00419afc: bl       #0x753f74
00419b00: ldr      r7, [pc, #0x40]
00419b04: ldr      r1, [r0, #8]
00419b08: add      r7, pc, r7
00419b0c: b        #0x4199a4
00419b10: subseq   fp, r7, r8, asr sp
00419b14: strdeq   r3, r4, [r0], -r4
00419b18: subeq    r8, sl, ip, ror r6
00419b1c: subseq   sl, r8, r0, lsr #9
00419b20: subseq   sl, r8, r0, ror #8
00419b24: subeq    r8, sl, r4, ror #6
00419b28: subseq   sl, r8, r4, lsl #6
00419b2c: subseq   sl, r8, r4, lsr #3
00419b30: subeq    r8, sl, r0, lsl r0
00419b34: strdeq   r5, r6, [sl], #-0x6c
00419b38: andeq    r2, r0, r4, asr r1
00419b3c: subseq   sb, r8, r8, ror pc
00419b40: ldrsheq  sb, [r8], #-0xc4
00419b44: subseq   sb, r8, r4, ror #24
00419b48: subseq   sb, r8, ip, lsl #23

# _ZN11HUDControlsC1Ev
0041af1c: push     {r4, r5, r6, lr}
0041af20: ldr      r6, [pc, #0x1e8]
0041af24: ldr      r3, [pc, #0x1e8]
0041af28: mov      r5, #0
0041af2c: add      r6, pc, r6
0041af30: ldr      r3, [r6, r3]
0041af34: mov      r4, r0
0041af38: strb     r5, [r0, #8]
0041af3c: add      r2, r3, #0x24
0041af40: add      r3, r3, #8
0041af44: str      r2, [r0, #4]
0041af48: str      r3, [r0]
0041af4c: strb     r5, [r0, #9]
0041af50: strb     r5, [r0, #0xa]
0041af54: str      r5, [r0, #0xc]
0041af58: str      r5, [r0, #0x10]
0041af5c: str      r5, [r0, #0x14]
0041af60: str      r5, [r0, #0x18]
0041af64: add      r0, r0, #0x1c
0041af68: bl       #0x41aeec
0041af6c: add      r0, r4, #0x4c
0041af70: bl       #0x41aeec
0041af74: mvn      r3, #0
0041af78: str      r3, [r4, #0x80]
0041af7c: str      r3, [r4, #0x7c]
0041af80: strb     r5, [r4, #0x84]
0041af84: add      r0, r4, #0x88
0041af88: bl       #0x41aeec
0041af8c: add      r0, r4, #0xb8
0041af90: bl       #0x41aeec
0041af94: add      r0, r4, #0xe8
0041af98: bl       #0x41aeec
0041af9c: add      r0, r4, #0x118
0041afa0: bl       #0x41aeec
0041afa4: add      r0, r4, #0x148
0041afa8: bl       #0x41aeec
0041afac: add      r0, r4, #0x178
0041afb0: bl       #0x41aeec
0041afb4: add      r0, r4, #0x1a8
0041afb8: bl       #0x41aeec
0041afbc: add      r0, r4, #0x1d8
0041afc0: bl       #0x41aeec
0041afc4: add      r0, r4, #0x208
0041afc8: bl       #0x41aeec
0041afcc: add      r0, r4, #0x238
0041afd0: bl       #0x41aeec
0041afd4: add      r0, r4, #0x268
0041afd8: bl       #0x41aeec
0041afdc: add      r0, r4, #0x298
0041afe0: bl       #0x41aeec
0041afe4: add      r0, r4, #0x2c8
0041afe8: bl       #0x41aeec
0041afec: add      r0, r4, #0x2f8
0041aff0: bl       #0x41aeec
0041aff4: add      r0, r4, #0x328
0041aff8: bl       #0x41aeec
0041affc: add      r0, r4, #0x358
0041b000: bl       #0x41aeec
0041b004: add      r0, r4, #0x388
0041b008: bl       #0x41aeec
0041b00c: add      r0, r4, #0x3b8
0041b010: bl       #0x41aeec
0041b014: add      r0, r4, #0x3e8
0041b018: bl       #0x41aeec
0041b01c: add      r0, r4, #0x410
0041b020: add      r0, r0, #8
0041b024: bl       #0x41aeec
0041b028: add      r0, r4, #0x440
0041b02c: add      r0, r0, #8
0041b030: bl       #0x41aeec
0041b034: add      r0, r4, #0x470
0041b038: add      r0, r0, #8
0041b03c: bl       #0x41aeec
0041b040: add      r0, r4, #0x4a0
0041b044: add      r0, r0, #8
0041b048: bl       #0x41aeec
0041b04c: add      r0, r4, #0x4d0
0041b050: add      r0, r0, #8
0041b054: bl       #0x41aeec
0041b058: add      r0, r4, #0x500
0041b05c: add      r0, r0, #8
0041b060: bl       #0x41aeec
0041b064: add      r0, r4, #0x530
0041b068: add      r0, r0, #8
0041b06c: bl       #0x41aeec
0041b070: add      r0, r4, #0x560
0041b074: add      r0, r0, #8
0041b078: bl       #0x41aeec
0041b07c: add      r0, r4, #0x590
0041b080: add      r0, r0, #8
0041b084: bl       #0x41aeec
0041b088: add      r0, r4, #0x5c0
0041b08c: add      r0, r0, #8
0041b090: bl       #0x41aeec
0041b094: add      r0, r4, #0x5f0
0041b098: add      r0, r0, #8
0041b09c: bl       #0x41aeec
0041b0a0: add      r0, r4, #0x620
0041b0a4: add      r0, r0, #8
0041b0a8: bl       #0x41aeec
0041b0ac: ldr      r2, [pc, #0x64]
0041b0b0: mov      r3, #0
0041b0b4: str      r3, [r4, #0x668]
0041b0b8: ldr      r6, [r6, r2]
0041b0bc: str      r3, [r4, #0x65c]
0041b0c0: str      r3, [r4, #0x660]
0041b0c4: str      r3, [r4, #0x664]
0041b0c8: str      r5, [r4, #0x658]
0041b0cc: strb     r5, [r4, #0x66c]
0041b0d0: strb     r5, [r4, #0x66d]
0041b0d4: strb     r5, [r4, #0x66e]
0041b0d8: strb     r5, [r4, #0x66f]
0041b0dc: strb     r5, [r4, #0x670]
0041b0e0: mov      r2, r4
0041b0e4: mov      r3, r5
0041b0e8: mov      r1, #4
0041b0ec: ldr      r0, [r6, #0x14]
0041b0f0: bl       #0x338da0
0041b0f4: ldr      r0, [r6, #0x14]
0041b0f8: mov      r3, r5
0041b0fc: mov      r1, #5
0041b100: mov      r2, r4
0041b104: bl       #0x338da0
0041b108: mov      r0, r4
0041b10c: pop      {r4, r5, r6, pc}
0041b110: subseq   sb, r7, r4, ror #22
0041b114: andeq    r2, r0, r0, asr #14
0041b118: strdeq   r3, r4, [r0], -r4

# _ZN11HUDControls7onEventEPK6IEventPK12EventManager
00418258: ldr      r3, [pc, #0xc0]
0041825c: push     {r4, r5, r6, lr}
00418260: ldr      r2, [pc, #0xbc]
00418264: mov      r4, r1
00418268: ldr      r1, [pc, #0xb8]
0041826c: add      r3, pc, r3
00418270: mov      r5, r0
00418274: add      r1, pc, r1
00418278: ldr      r0, [r3, r2]
0041827c: bl       #0x320e44
00418280: cmp      r0, #0
00418284: bne      #0x418294
00418288: ldrb     r3, [r5, #9]
0041828c: cmp      r3, #0
00418290: beq      #0x41829c
00418294: mov      r0, #0
00418298: pop      {r4, r5, r6, pc}
0041829c: ldrb     r3, [r5, #0x84]
004182a0: cmp      r3, #0
004182a4: bne      #0x418294
004182a8: ldr      r3, [r4]
004182ac: mov      r0, r4
004182b0: mov      lr, pc
004182b4: ldr      pc, [r3, #8]
004182b8: cmp      r0, #4
004182bc: bne      #0x4182f4
004182c0: ldrb     r3, [r4, #0x10]
004182c4: ldrh     r1, [r4, #8]
004182c8: ldrh     r2, [r4, #0xa]
004182cc: cmp      r3, #0
004182d0: mvneq    r3, #0
004182d4: sxthne   r1, r1
004182d8: sxthne   r2, r2
004182dc: strne    r2, [r5, #0x80]
004182e0: strne    r1, [r5, #0x7c]
004182e4: streq    r3, [r5, #0x80]
004182e8: streq    r3, [r5, #0x7c]
004182ec: mov      r0, #0
004182f0: pop      {r4, r5, r6, pc}
004182f4: ldr      r3, [r4]
004182f8: mov      r0, r4
004182fc: mov      lr, pc
00418300: ldr      pc, [r3, #8]
00418304: cmp      r0, #5
00418308: ldrsheq  r2, [r4, #8]
0041830c: ldrsheq  r3, [r4, #0xa]
00418310: mov      r0, #0
00418314: streq    r2, [r5, #0x7c]
00418318: streq    r3, [r5, #0x80]
0041831c: pop      {r4, r5, r6, pc}
00418320: subseq   ip, r7, r4, lsr #16
00418324: strdeq   r3, r4, [r0], -r4
00418328: subeq    r6, sl, ip, asr #19

# _ZN11HUDControls15initCachedCharsEv
00419b4c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00419b50: ldr      r6, [pc, #0xb94]
00419b54: ldr      r2, [pc, #0xb94]
00419b58: sub      sp, sp, #0x54
00419b5c: add      r6, pc, r6
00419b60: ldr      r3, [r6, r2]
00419b64: str      r2, [sp, #8]
00419b68: ldr      r2, [r0, #0x658]
00419b6c: ldr      r3, [r3]
00419b70: mov      r4, r0
00419b74: cmp      r2, #0
00419b78: str      r3, [sp, #0x4c]
00419b7c: beq      #0x41a230
00419b80: ldr      r3, [pc, #0xb6c]
00419b84: ldr      r1, [pc, #0xb6c]
00419b88: add      r5, sp, #0x38
00419b8c: ldr      r0, [r6, r3]
00419b90: add      r1, pc, r1
00419b94: bl       #0x320e44
00419b98: ldr      r1, [pc, #0xb5c]
00419b9c: mov      r2, r0
00419ba0: str      r0, [sp, #0xc]
00419ba4: add      r1, pc, r1
00419ba8: mov      r0, r5
00419bac: bl       #0x30eae4
00419bb0: mov      r1, r5
00419bb4: ldr      r0, [r4, #0x658]
00419bb8: bl       #0x7a9160
00419bbc: add      r5, r4, #0x388
00419bc0: mov      r1, r0
00419bc4: ldr      r2, [r4, #0x658]
00419bc8: mov      r3, #0
00419bcc: mov      r0, r5
00419bd0: bl       #0x427c44
00419bd4: mov      r0, r5
00419bd8: ldr      r7, [r4, #0x658]
00419bdc: bl       #0x427d50
00419be0: ldr      r1, [pc, #0xb18]
00419be4: add      r2, r4, #0x1c
00419be8: str      r2, [sp, #0x28]
00419bec: mov      r3, r0
00419bf0: mov      r2, r7
00419bf4: add      r1, pc, r1
00419bf8: ldr      r0, [sp, #0x28]
00419bfc: bl       #0x427ca0
00419c00: mov      r0, r5
00419c04: ldr      r7, [r4, #0x658]
00419c08: bl       #0x427d50
00419c0c: ldr      r1, [pc, #0xaf0]
00419c10: add      r2, r4, #0x88
00419c14: str      r2, [sp, #0x34]
00419c18: mov      r3, r0
00419c1c: mov      r2, r7
00419c20: add      r1, pc, r1
00419c24: ldr      r0, [sp, #0x34]
00419c28: bl       #0x427ca0
00419c2c: mov      r0, r5
00419c30: ldr      r7, [r4, #0x658]
00419c34: bl       #0x427d50
00419c38: ldr      r1, [pc, #0xac8]
00419c3c: add      r2, r4, #0x4c
00419c40: str      r2, [sp, #0x24]
00419c44: mov      r3, r0
00419c48: mov      r2, r7
00419c4c: add      r1, pc, r1
00419c50: ldr      r0, [sp, #0x24]
00419c54: bl       #0x427ca0
00419c58: ldr      r1, [pc, #0xaac]
00419c5c: ldr      r2, [r4, #0x658]
00419c60: mov      r3, #0
00419c64: add      r1, pc, r1
00419c68: add      r0, r4, #0xb8
00419c6c: bl       #0x427ca0
00419c70: mov      r0, r5
00419c74: ldr      r8, [r4, #0x658]
00419c78: bl       #0x427d50
00419c7c: ldr      r7, [pc, #0xa8c]
00419c80: mov      r3, r0
00419c84: mov      r2, r8
00419c88: add      r7, pc, r7
00419c8c: mov      r1, r7
00419c90: add      r0, r4, #0xe8
00419c94: bl       #0x427ca0
00419c98: mov      r0, r5
00419c9c: ldr      r8, [r4, #0x658]
00419ca0: bl       #0x427d50
00419ca4: ldr      r1, [pc, #0xa68]
00419ca8: add      r2, r4, #0x3b8
00419cac: str      r2, [sp, #0x30]
00419cb0: mov      r3, r0
00419cb4: mov      r2, r8
00419cb8: add      r1, pc, r1
00419cbc: ldr      r0, [sp, #0x30]
00419cc0: bl       #0x427ca0
00419cc4: add      r3, r4, #0x3e8
00419cc8: mov      r0, r5
00419ccc: ldr      sl, [r4, #0x658]
00419cd0: str      r3, [sp, #0x2c]
00419cd4: bl       #0x427d50
00419cd8: ldr      r8, [pc, #0xa38]
00419cdc: mov      r3, r0
00419ce0: mov      r2, sl
00419ce4: add      r8, pc, r8
00419ce8: mov      r1, r8
00419cec: ldr      r0, [sp, #0x2c]
00419cf0: bl       #0x427ca0
00419cf4: add      r2, r4, #0x410
00419cf8: add      r2, r2, #8
00419cfc: mov      r0, r5
00419d00: ldr      sb, [r4, #0x658]
00419d04: str      r2, [sp, #0x20]
00419d08: bl       #0x427d50
00419d0c: ldr      sl, [pc, #0xa08]
00419d10: mov      r2, sb
00419d14: mov      r3, r0
00419d18: add      sl, pc, sl
00419d1c: mov      r1, sl
00419d20: ldr      r0, [sp, #0x20]
00419d24: bl       #0x427ca0
00419d28: add      r3, r4, #0x440
00419d2c: add      r3, r3, #8
00419d30: mov      r0, r5
00419d34: ldr      fp, [r4, #0x658]
00419d38: str      r3, [sp, #0x1c]
00419d3c: bl       #0x427d50
00419d40: ldr      sb, [pc, #0x9d8]
00419d44: mov      r3, r0
00419d48: mov      r2, fp
00419d4c: add      sb, pc, sb
00419d50: mov      r1, sb
00419d54: ldr      r0, [sp, #0x1c]
00419d58: bl       #0x427ca0
00419d5c: mov      r0, r5
00419d60: ldr      fp, [r4, #0x658]
00419d64: bl       #0x427d50
00419d68: add      r2, r4, #0x470
00419d6c: ldr      r1, [pc, #0x9b0]
00419d70: add      r2, r2, #8
00419d74: str      r2, [sp, #0x18]
00419d78: mov      r3, r0
00419d7c: mov      r2, fp
00419d80: add      r1, pc, r1
00419d84: ldr      r0, [sp, #0x18]
00419d88: bl       #0x427ca0
00419d8c: mov      r0, r5
00419d90: ldr      fp, [r4, #0x658]
00419d94: bl       #0x427d50
00419d98: add      r2, r4, #0x4a0
00419d9c: ldr      r1, [pc, #0x984]
00419da0: add      r2, r2, #8
00419da4: str      r2, [sp, #4]
00419da8: mov      r3, r0
00419dac: mov      r2, fp
00419db0: add      r1, pc, r1
00419db4: ldr      r0, [sp, #4]
00419db8: bl       #0x427ca0
00419dbc: mov      r0, r5
00419dc0: ldr      fp, [r4, #0x658]
00419dc4: bl       #0x427d50
00419dc8: add      r2, r4, #0x4d0
00419dcc: ldr      r1, [pc, #0x958]
00419dd0: add      r2, r2, #8
00419dd4: str      r2, [sp, #0x14]
00419dd8: mov      r3, r0
00419ddc: mov      r2, fp
00419de0: add      r1, pc, r1
00419de4: ldr      r0, [sp, #0x14]
00419de8: bl       #0x427ca0
00419dec: add      r3, r4, #0x500
00419df0: add      r3, r3, #8
00419df4: mov      r0, r5
00419df8: ldr      fp, [r4, #0x658]
00419dfc: str      r3, [sp, #0x10]
00419e00: bl       #0x427d50
00419e04: mov      r1, r7
00419e08: mov      r3, r0
00419e0c: mov      r2, fp
00419e10: ldr      r0, [sp, #0x10]
00419e14: bl       #0x427ca0
00419e18: mov      r0, r5
00419e1c: ldr      r7, [r4, #0x658]
00419e20: bl       #0x427d50
00419e24: ldr      r1, [pc, #0x904]
00419e28: mov      r3, r0
00419e2c: add      r0, r4, #0x530
00419e30: mov      r2, r7
00419e34: add      r1, pc, r1
00419e38: add      r0, r0, #8
00419e3c: bl       #0x427ca0
00419e40: mov      r0, r5
00419e44: ldr      r7, [r4, #0x658]
00419e48: bl       #0x427d50
00419e4c: ldr      r1, [pc, #0x8e0]
00419e50: add      fp, r4, #0x560
00419e54: add      fp, fp, #8
00419e58: mov      r3, r0
00419e5c: mov      r2, r7
00419e60: add      r1, pc, r1
00419e64: mov      r0, fp
00419e68: bl       #0x427ca0
00419e6c: ldr      r2, [sp, #0xc]
00419e70: cmp      r2, #1
00419e74: ble      #0x41a2e4
00419e78: mov      r0, r5
00419e7c: ldr      r8, [r4, #0x658]
00419e80: bl       #0x427d50
00419e84: ldr      r7, [pc, #0x8ac]
00419e88: mov      r3, r0
00419e8c: mov      r2, r8
00419e90: add      r7, pc, r7
00419e94: mov      r1, r7
00419e98: add      r0, r4, #0x118
00419e9c: bl       #0x427ca0
00419ea0: mov      r0, r5
00419ea4: ldr      r8, [r4, #0x658]
00419ea8: bl       #0x427d50
00419eac: mov      r2, r8
00419eb0: mov      r3, r0
00419eb4: mov      r1, r7
00419eb8: add      r0, r4, #0x148
00419ebc: bl       #0x427ca0
00419ec0: mov      r0, r5
00419ec4: ldr      r8, [r4, #0x658]
00419ec8: bl       #0x427d50
00419ecc: mov      r1, r7
00419ed0: mov      r3, r0
00419ed4: mov      r2, r8
00419ed8: add      r0, r4, #0x178
00419edc: bl       #0x427ca0
00419ee0: mov      r0, r5
00419ee4: ldr      r7, [r4, #0x658]
00419ee8: bl       #0x427d50
00419eec: ldr      r1, [pc, #0x848]
00419ef0: mov      r3, r0
00419ef4: mov      r2, r7
00419ef8: add      r1, pc, r1
00419efc: add      r0, r4, #0x1a8
00419f00: bl       #0x427ca0
00419f04: mov      r0, r5
00419f08: ldr      r7, [r4, #0x658]
00419f0c: bl       #0x427d50
00419f10: ldr      r1, [pc, #0x828]
00419f14: mov      r3, r0
00419f18: mov      r2, r7
00419f1c: add      r1, pc, r1
00419f20: add      r0, r4, #0x1d8
00419f24: bl       #0x427ca0
00419f28: mov      r0, r5
00419f2c: ldr      r7, [r4, #0x658]
00419f30: bl       #0x427d50
00419f34: ldr      r1, [pc, #0x808]
00419f38: mov      r3, r0
00419f3c: mov      r2, r7
00419f40: add      r1, pc, r1
00419f44: add      r0, r4, #0x208
00419f48: bl       #0x427ca0
00419f4c: mov      r0, r5
00419f50: ldr      r7, [r4, #0x658]
00419f54: bl       #0x427d50
00419f58: ldr      r1, [pc, #0x7e8]
00419f5c: mov      r3, r0
00419f60: mov      r2, r7
00419f64: add      r1, pc, r1
00419f68: add      r0, r4, #0x238
00419f6c: bl       #0x427ca0
00419f70: mov      r0, r5
00419f74: ldr      r7, [r4, #0x658]
00419f78: bl       #0x427d50
00419f7c: ldr      r1, [pc, #0x7c8]
00419f80: mov      r3, r0
00419f84: mov      r2, r7
00419f88: add      r1, pc, r1
00419f8c: add      r0, r4, #0x268
00419f90: bl       #0x427ca0
00419f94: mov      r0, r5
00419f98: ldr      r7, [r4, #0x658]
00419f9c: bl       #0x427d50
00419fa0: ldr      r1, [pc, #0x7a8]
00419fa4: mov      r3, r0
00419fa8: mov      r2, r7
00419fac: add      r1, pc, r1
00419fb0: add      r0, r4, #0x298
00419fb4: bl       #0x427ca0
00419fb8: mov      r0, r5
00419fbc: ldr      r7, [r4, #0x658]
00419fc0: bl       #0x427d50
00419fc4: ldr      r1, [pc, #0x788]
00419fc8: mov      r3, r0
00419fcc: mov      r2, r7
00419fd0: add      r1, pc, r1
00419fd4: add      r0, r4, #0x2c8
00419fd8: bl       #0x427ca0
00419fdc: mov      r0, r5
00419fe0: ldr      r7, [r4, #0x658]
00419fe4: bl       #0x427d50
00419fe8: ldr      r1, [pc, #0x768]
00419fec: mov      r3, r0
00419ff0: mov      r2, r7
00419ff4: add      r1, pc, r1
00419ff8: add      r0, r4, #0x2f8
00419ffc: bl       #0x427ca0
0041a000: mov      r0, r5
0041a004: ldr      r7, [r4, #0x658]
0041a008: bl       #0x427d50
0041a00c: ldr      r1, [pc, #0x748]
0041a010: mov      r3, r0
0041a014: mov      r2, r7
0041a018: add      r1, pc, r1
0041a01c: add      r0, r4, #0x328
0041a020: bl       #0x427ca0
0041a024: ldr      r1, [pc, #0x734]
0041a028: mov      r3, #0
0041a02c: ldr      r2, [r4, #0x658]
0041a030: add      r1, pc, r1
0041a034: add      r0, r4, #0x358
0041a038: bl       #0x427ca0
0041a03c: ldr      r0, [sp, #0x24]
0041a040: ldr      r7, [r4, #0x658]
0041a044: bl       #0x427d50
0041a048: ldr      r1, [pc, #0x714]
0041a04c: mov      r2, r0
0041a050: mov      r0, r7
0041a054: add      r1, pc, r1
0041a058: bl       #0x7a8a84
0041a05c: ldr      r3, [r0]
0041a060: mov      lr, pc
0041a064: ldr      pc, [r3, #0x128]
0041a068: mov      r1, #0x41000000
0041a06c: add      r1, r1, #0xa00000
0041a070: bl       #0x30ec94
0041a074: mov      r1, #0x3f000000
0041a078: bl       #0x30ed6c
0041a07c: bl       #0x30e4cc
0041a080: str      r0, [r4, #0xc]
0041a084: str      r0, [r4, #0x10]
0041a088: ldr      r0, [sp, #0x28]
0041a08c: ldr      r7, [r4, #0x658]
0041a090: bl       #0x427d50
0041a094: mov      r2, #0
0041a098: mov      r3, r2
0041a09c: mov      r1, r0
0041a0a0: mov      r0, r7
0041a0a4: bl       #0x7aa3f0
0041a0a8: ldr      r1, [pc, #0x6b8]
0041a0ac: ldr      r0, [r4, #0x658]
0041a0b0: add      r7, r4, #0x590
0041a0b4: add      r1, pc, r1
0041a0b8: bl       #0x7a9160
0041a0bc: ldr      r2, [r4, #0x658]
0041a0c0: mov      r1, r0
0041a0c4: mov      r3, #0
0041a0c8: mov      r0, r5
0041a0cc: bl       #0x427c44
0041a0d0: mov      r0, r5
0041a0d4: ldr      r8, [r4, #0x658]
0041a0d8: bl       #0x427d50
0041a0dc: ldr      r1, [pc, #0x688]
0041a0e0: add      r7, r7, #8
0041a0e4: mov      r3, r0
0041a0e8: mov      r2, r8
0041a0ec: add      r1, pc, r1
0041a0f0: mov      r0, r7
0041a0f4: bl       #0x427ca0
0041a0f8: mov      r0, r5
0041a0fc: ldr      sl, [r4, #0x658]
0041a100: bl       #0x427d50
0041a104: ldr      r1, [pc, #0x664]
0041a108: add      r8, r4, #0x5c0
0041a10c: add      r8, r8, #8
0041a110: mov      r3, r0
0041a114: mov      r2, sl
0041a118: add      r1, pc, r1
0041a11c: mov      r0, r8
0041a120: bl       #0x427ca0
0041a124: mov      r0, r5
0041a128: bl       #0x427d50
0041a12c: ldr      r1, [pc, #0x640]
0041a130: mov      sb, #1
0041a134: strb     sb, [r0, #0x9b]
0041a138: add      r1, pc, r1
0041a13c: ldr      r0, [r4, #0x658]
0041a140: bl       #0x7a9160
0041a144: mov      r3, #0
0041a148: mov      r1, r0
0041a14c: ldr      r2, [r4, #0x658]
0041a150: mov      r0, r5
0041a154: bl       #0x427c44
0041a158: ldr      r2, [r4, #0x658]
0041a15c: mov      r0, r5
0041a160: add      sl, r4, #0x5f0
0041a164: str      r2, [sp]
0041a168: bl       #0x427d50
0041a16c: ldr      r1, [pc, #0x604]
0041a170: add      sl, sl, #8
0041a174: mov      r3, r0
0041a178: add      r1, pc, r1
0041a17c: ldr      r2, [sp]
0041a180: mov      r0, sl
0041a184: bl       #0x427ca0
0041a188: ldr      r2, [r4, #0x658]
0041a18c: mov      r0, r5
0041a190: add      r5, r4, #0x620
0041a194: str      r2, [sp]
0041a198: bl       #0x427d50
0041a19c: ldr      r1, [pc, #0x5d8]
0041a1a0: add      r5, r5, #8
0041a1a4: mov      r3, r0
0041a1a8: add      r1, pc, r1
0041a1ac: ldr      r2, [sp]
0041a1b0: mov      r0, r5
0041a1b4: bl       #0x427ca0
0041a1b8: ldrb     r3, [r4, #0x66c]
0041a1bc: cmp      r3, #0
0041a1c0: bne      #0x41a250
0041a1c4: mov      r0, r7
0041a1c8: str      r3, [sp]
0041a1cc: bl       #0x427d50
0041a1d0: ldr      r3, [sp]
0041a1d4: strb     r3, [r0, #0x9b]
0041a1d8: mov      r0, r8
0041a1dc: str      r3, [sp]
0041a1e0: bl       #0x427d50
0041a1e4: ldr      r3, [sp]
0041a1e8: strb     r3, [r0, #0x9b]
0041a1ec: ldr      r0, [sp, #4]
0041a1f0: bl       #0x427d50
0041a1f4: strb     sb, [r0, #0x9b]
0041a1f8: mov      r0, fp
0041a1fc: bl       #0x427d50
0041a200: strb     sb, [r0, #0x9b]
0041a204: ldr      r3, [sp, #0xc]
0041a208: cmp      r3, #0
0041a20c: bne      #0x41a2a8
0041a210: ldrb     r3, [r4, #0x66d]
0041a214: cmp      r3, #0
0041a218: beq      #0x41a524
0041a21c: ldrb     r3, [r4, #0x66f]
0041a220: cmp      r3, #0
0041a224: bne      #0x41a50c
0041a228: mov      r3, #1
0041a22c: strb     r3, [r4, #8]
0041a230: ldr      r2, [sp, #8]
0041a234: ldr      r3, [r6, r2]
0041a238: ldr      r2, [sp, #0x4c]
0041a23c: ldr      r3, [r3]
0041a240: cmp      r2, r3
0041a244: bne      #0x41a6e8
0041a248: add      sp, sp, #0x54
0041a24c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0041a250: mov      r0, r7
0041a254: bl       #0x427d50
0041a258: strb     sb, [r0, #0x9b]
0041a25c: mov      r0, r8
0041a260: bl       #0x427d50
0041a264: strb     sb, [r0, #0x9b]
0041a268: ldr      r0, [sp, #4]
0041a26c: bl       #0x427d50
0041a270: mov      r7, #0
0041a274: strb     r7, [r0, #0x9b]
0041a278: mov      r0, fp
0041a27c: bl       #0x427d50
0041a280: strb     r7, [r0, #0x9b]
0041a284: mov      r0, sl
0041a288: bl       #0x427d50
0041a28c: strb     r7, [r0, #0x9b]
0041a290: mov      r0, r5
0041a294: bl       #0x427d50
0041a298: strb     r7, [r0, #0x9b]
0041a29c: ldr      r3, [sp, #0xc]
0041a2a0: cmp      r3, #0
0041a2a4: beq      #0x41a210
0041a2a8: ldr      r2, [sp, #0xc]
0041a2ac: cmp      r2, #1
0041a2b0: bne      #0x41a228
0041a2b4: ldrb     r3, [r4, #0x66e]
0041a2b8: cmp      r3, #0
0041a2bc: beq      #0x41a348
0041a2c0: ldrb     r3, [r4, #0x670]
0041a2c4: cmp      r3, #0
0041a2c8: beq      #0x41a228
0041a2cc: add      r1, r4, #0x770
0041a2d0: add      r1, r1, #0xc
0041a2d4: mov      r0, r4
0041a2d8: mov      r2, #0
0041a2dc: bl       #0x4187a0
0041a2e0: b        #0x41a228
0041a2e4: mov      r0, r5
0041a2e8: ldr      r7, [r4, #0x658]
0041a2ec: bl       #0x427d50
0041a2f0: mov      r1, r8
0041a2f4: mov      r3, r0
0041a2f8: mov      r2, r7
0041a2fc: add      r0, r4, #0x118
0041a300: bl       #0x427ca0
0041a304: mov      r0, r5
0041a308: ldr      r7, [r4, #0x658]
0041a30c: bl       #0x427d50
0041a310: mov      r1, sl
0041a314: mov      r3, r0
0041a318: mov      r2, r7
0041a31c: add      r0, r4, #0x148
0041a320: bl       #0x427ca0
0041a324: mov      r0, r5
0041a328: ldr      r7, [r4, #0x658]
0041a32c: bl       #0x427d50
0041a330: mov      r1, sb
0041a334: mov      r3, r0
0041a338: mov      r2, r7
0041a33c: add      r0, r4, #0x178
0041a340: bl       #0x427ca0
0041a344: b        #0x419ee0
0041a348: ldr      r0, [sp, #0x24]
0041a34c: bl       #0x427d50
0041a350: bl       #0x753f74
0041a354: ldr      r3, [r0, #8]
0041a358: ldr      r0, [sp, #0x24]
0041a35c: str      r3, [r4, #0x6cc]
0041a360: bl       #0x427d50
0041a364: bl       #0x753f74
0041a368: ldr      r3, [r0, #0x14]
0041a36c: ldr      r0, [sp, #0x34]
0041a370: str      r3, [r4, #0x6d0]
0041a374: bl       #0x427d50
0041a378: bl       #0x753f74
0041a37c: ldr      r3, [r0, #8]
0041a380: ldr      r0, [sp, #0x34]
0041a384: str      r3, [r4, #0x6d4]
0041a388: bl       #0x427d50
0041a38c: bl       #0x753f74
0041a390: ldr      r3, [r0, #0x14]
0041a394: ldr      r0, [sp, #0x30]
0041a398: str      r3, [r4, #0x6d8]
0041a39c: bl       #0x427d50
0041a3a0: bl       #0x753f74
0041a3a4: ldr      r3, [r0, #8]
0041a3a8: ldr      r0, [sp, #0x30]
0041a3ac: str      r3, [r4, #0x6dc]
0041a3b0: bl       #0x427d50
0041a3b4: bl       #0x753f74
0041a3b8: ldr      r3, [r0, #0x14]
0041a3bc: ldr      r0, [sp, #0x2c]
0041a3c0: str      r3, [r4, #0x6e0]
0041a3c4: bl       #0x427d50
0041a3c8: bl       #0x753f74
0041a3cc: ldr      r3, [r0, #8]
0041a3d0: ldr      r0, [sp, #0x2c]
0041a3d4: str      r3, [r4, #0x6e4]
0041a3d8: bl       #0x427d50
0041a3dc: bl       #0x753f74
0041a3e0: ldr      r3, [r0, #0x14]
0041a3e4: ldr      r0, [sp, #0x20]
0041a3e8: str      r3, [r4, #0x6e8]
0041a3ec: bl       #0x427d50
0041a3f0: bl       #0x753f74
0041a3f4: ldr      r3, [r0, #8]
0041a3f8: ldr      r0, [sp, #0x20]
0041a3fc: str      r3, [r4, #0x6ec]
0041a400: bl       #0x427d50
0041a404: bl       #0x753f74
0041a408: ldr      r3, [r0, #0x14]
0041a40c: ldr      r0, [sp, #0x1c]
0041a410: str      r3, [r4, #0x6f0]
0041a414: bl       #0x427d50
0041a418: bl       #0x753f74
0041a41c: ldr      r3, [r0, #8]
0041a420: ldr      r0, [sp, #0x1c]
0041a424: str      r3, [r4, #0x6f4]
0041a428: bl       #0x427d50
0041a42c: bl       #0x753f74
0041a430: ldr      r3, [r0, #0x14]
0041a434: ldr      r0, [sp, #0x18]
0041a438: str      r3, [r4, #0x6f8]
0041a43c: bl       #0x427d50
0041a440: bl       #0x753f74
0041a444: ldr      r3, [r0, #8]
0041a448: ldr      r0, [sp, #0x18]
0041a44c: str      r3, [r4, #0x6fc]
0041a450: bl       #0x427d50
0041a454: bl       #0x753f74
0041a458: ldr      r3, [r0, #0x14]
0041a45c: ldr      r0, [sp, #0x14]
0041a460: str      r3, [r4, #0x700]
0041a464: bl       #0x427d50
0041a468: bl       #0x753f74
0041a46c: ldr      r3, [r0, #8]
0041a470: ldr      r0, [sp, #0x14]
0041a474: str      r3, [r4, #0x704]
0041a478: bl       #0x427d50
0041a47c: bl       #0x753f74
0041a480: ldr      r3, [r0, #0x14]
0041a484: ldr      r0, [sp, #4]
0041a488: str      r3, [r4, #0x708]
0041a48c: bl       #0x427d50
0041a490: bl       #0x753f74
0041a494: ldr      r3, [r0, #8]
0041a498: ldr      r0, [sp, #4]
0041a49c: str      r3, [r4, #0x70c]
0041a4a0: bl       #0x427d50
0041a4a4: bl       #0x753f74
0041a4a8: ldr      r3, [r0, #0x14]
0041a4ac: ldr      r0, [sp, #0x10]
0041a4b0: str      r3, [r4, #0x710]
0041a4b4: bl       #0x427d50
0041a4b8: bl       #0x753f74
0041a4bc: ldr      r3, [r0, #8]
0041a4c0: ldr      r0, [sp, #0x10]
0041a4c4: str      r3, [r4, #0x714]
0041a4c8: bl       #0x427d50
0041a4cc: bl       #0x753f74
0041a4d0: ldr      r3, [r0, #0x14]
0041a4d4: mov      r0, fp
0041a4d8: str      r3, [r4, #0x718]
0041a4dc: bl       #0x427d50
0041a4e0: bl       #0x753f74
0041a4e4: ldr      r3, [r0, #8]
0041a4e8: mov      r0, fp
0041a4ec: str      r3, [r4, #0x71c]
0041a4f0: bl       #0x427d50
0041a4f4: bl       #0x753f74
0041a4f8: ldr      r2, [sp, #0xc]
0041a4fc: ldr      r3, [r0, #0x14]
0041a500: strb     r2, [r4, #0x66e]
0041a504: str      r3, [r4, #0x720]
0041a508: b        #0x41a2c0
0041a50c: add      r1, r4, #0x720
0041a510: add      r1, r1, #4
0041a514: mov      r0, r4
0041a518: mov      r2, #0
0041a51c: bl       #0x4187a0
0041a520: b        #0x41a228
0041a524: ldr      r0, [sp, #0x24]
0041a528: bl       #0x427d50
0041a52c: bl       #0x753f74
0041a530: ldr      r3, [r0, #8]
0041a534: ldr      r0, [sp, #0x24]
0041a538: str      r3, [r4, #0x674]
0041a53c: bl       #0x427d50
0041a540: bl       #0x753f74
0041a544: ldr      r3, [r0, #0x14]
0041a548: ldr      r0, [sp, #0x34]
0041a54c: str      r3, [r4, #0x678]
0041a550: bl       #0x427d50
0041a554: bl       #0x753f74
0041a558: ldr      r3, [r0, #8]
0041a55c: ldr      r0, [sp, #0x34]
0041a560: str      r3, [r4, #0x67c]
0041a564: bl       #0x427d50
0041a568: bl       #0x753f74
0041a56c: ldr      r3, [r0, #0x14]
0041a570: ldr      r0, [sp, #0x30]
0041a574: str      r3, [r4, #0x680]
0041a578: bl       #0x427d50
0041a57c: bl       #0x753f74
0041a580: ldr      r3, [r0, #8]
0041a584: ldr      r0, [sp, #0x30]
0041a588: str      r3, [r4, #0x684]
0041a58c: bl       #0x427d50
0041a590: bl       #0x753f74
0041a594: ldr      r3, [r0, #0x14]
0041a598: ldr      r0, [sp, #0x2c]
0041a59c: str      r3, [r4, #0x688]
0041a5a0: bl       #0x427d50
0041a5a4: bl       #0x753f74
0041a5a8: ldr      r3, [r0, #8]
0041a5ac: ldr      r0, [sp, #0x2c]
0041a5b0: str      r3, [r4, #0x68c]
0041a5b4: bl       #0x427d50
0041a5b8: bl       #0x753f74
0041a5bc: ldr      r3, [r0, #0x14]
0041a5c0: ldr      r0, [sp, #0x20]
0041a5c4: str      r3, [r4, #0x690]
0041a5c8: bl       #0x427d50
0041a5cc: bl       #0x753f74
0041a5d0: ldr      r3, [r0, #8]
0041a5d4: ldr      r0, [sp, #0x20]
0041a5d8: str      r3, [r4, #0x694]
0041a5dc: bl       #0x427d50
0041a5e0: bl       #0x753f74
0041a5e4: ldr      r3, [r0, #0x14]
0041a5e8: ldr      r0, [sp, #0x1c]
0041a5ec: str      r3, [r4, #0x698]
0041a5f0: bl       #0x427d50
0041a5f4: bl       #0x753f74
0041a5f8: ldr      r3, [r0, #8]
0041a5fc: ldr      r0, [sp, #0x1c]
0041a600: str      r3, [r4, #0x69c]
0041a604: bl       #0x427d50
0041a608: bl       #0x753f74
0041a60c: ldr      r3, [r0, #0x14]
0041a610: ldr      r0, [sp, #0x18]
0041a614: str      r3, [r4, #0x6a0]
0041a618: bl       #0x427d50
0041a61c: bl       #0x753f74
0041a620: ldr      r3, [r0, #8]
0041a624: ldr      r0, [sp, #0x18]
0041a628: str      r3, [r4, #0x6a4]
0041a62c: bl       #0x427d50
0041a630: bl       #0x753f74
0041a634: ldr      r3, [r0, #0x14]
0041a638: ldr      r0, [sp, #0x14]
0041a63c: str      r3, [r4, #0x6a8]
0041a640: bl       #0x427d50
0041a644: bl       #0x753f74
0041a648: ldr      r3, [r0, #8]
0041a64c: ldr      r0, [sp, #0x14]
0041a650: str      r3, [r4, #0x6ac]
0041a654: bl       #0x427d50
0041a658: bl       #0x753f74
0041a65c: ldr      r3, [r0, #0x14]
0041a660: ldr      r0, [sp, #4]
0041a664: str      r3, [r4, #0x6b0]
0041a668: bl       #0x427d50
0041a66c: bl       #0x753f74
0041a670: ldr      r3, [r0, #8]
0041a674: ldr      r0, [sp, #4]
0041a678: str      r3, [r4, #0x6b4]
0041a67c: bl       #0x427d50
0041a680: bl       #0x753f74
0041a684: ldr      r3, [r0, #0x14]
0041a688: ldr      r0, [sp, #0x10]
0041a68c: str      r3, [r4, #0x6b8]
0041a690: bl       #0x427d50
0041a694: bl       #0x753f74
0041a698: ldr      r3, [r0, #8]
0041a69c: ldr      r0, [sp, #0x10]
0041a6a0: str      r3, [r4, #0x6bc]
0041a6a4: bl       #0x427d50
0041a6a8: bl       #0x753f74
0041a6ac: ldr      r3, [r0, #0x14]
0041a6b0: mov      r0, fp
0041a6b4: str      r3, [r4, #0x6c0]
0041a6b8: bl       #0x427d50
0041a6bc: bl       #0x753f74
0041a6c0: ldr      r3, [r0, #8]
0041a6c4: mov      r0, fp
0041a6c8: str      r3, [r4, #0x6c4]
0041a6cc: bl       #0x427d50
0041a6d0: bl       #0x753f74
0041a6d4: ldr      r3, [r0, #0x14]
0041a6d8: mov      r2, #1
0041a6dc: strb     r2, [r4, #0x66d]
0041a6e0: str      r3, [r4, #0x6c8]
0041a6e4: b        #0x41a21c
0041a6e8: bl       #0x30e310
0041a6ec: subseq   sl, r7, r4, lsr pc
0041a6f0: andeq    r4, r0, ip, lsr #1
0041a6f4: strdeq   r3, r4, [r0], -r4
0041a6f8: subeq    r7, sl, r8, ror #20
0041a6fc: subeq    r7, sl, ip, lsr #9
0041a700: subeq    lr, sl, r4, ror r8
0041a704: subeq    lr, sl, r8, ror r8
0041a708: subeq    lr, sl, ip, ror r8
0041a70c: subeq    lr, sl, ip, lsl #17
0041a710: subeq    lr, sl, r0, ror r8
0041a714: subeq    lr, sl, r8, ror #16
0041a718: subeq    lr, sl, r4, ror #16
0041a71c: subeq    lr, sl, r0, ror #16
0041a720: subeq    lr, sl, ip, asr r8
0041a724: subeq    lr, sl, r8, asr r8
0041a728: subeq    lr, sl, r8, asr #16
0041a72c: subeq    lr, sl, r8, lsr r8
0041a730: subeq    lr, sl, r4, lsl #16
0041a734: subeq    lr, sl, r0, lsl #16
0041a738: strdeq   lr, pc, [sl], #-0x70
0041a73c: subeq    lr, sl, r8, lsr #12
0041a740: strheq   lr, [sl], #-0x6c
0041a744: strheq   lr, [sl], #-0x68
0041a748: ldrdeq   lr, pc, [sl], #-0x64
0041a74c: subeq    lr, sl, r8, lsr #14
0041a750: subeq    lr, sl, r4, lsr r7
0041a754: subeq    lr, sl, r8, asr #14
0041a758: subeq    lr, sl, r4, asr r7
0041a75c: subeq    lr, sl, r8, ror #14
0041a760: subeq    lr, sl, r8, lsl #15
0041a764: subeq    lr, sl, r4, lsl #15
0041a768: subeq    lr, sl, ip, lsr #14
0041a76c: subeq    lr, sl, ip, lsl #14
0041a770: strdeq   lr, pc, [sl], #-0x68
0041a774: strdeq   lr, pc, [sl], #-0x60
0041a778: subeq    lr, sl, r8, asr #13
0041a77c: subeq    lr, sl, r8, lsr #13
