
# _ZN8RenderFX7SetTextEPKcS1_b
007a9390: push     {r4, r5, r6, lr}
007a9394: mov      r5, r2
007a9398: mov      r4, r3
007a939c: mov      r6, r0
007a93a0: bl       #0x7a9160
007a93a4: mov      r2, r5
007a93a8: mov      r1, r0
007a93ac: mov      r3, r4
007a93b0: mov      r0, r6
007a93b4: pop      {r4, r5, r6, lr}
007a93b8: b        #0x7a92e0

# _ZN7gameswf21bitmap_glyph_provider15get_font_entityERKNS_9tu_stringEbb
007c6048: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007c604c: ldr      r6, [pc, #0x38c]
007c6050: ldr      ip, [pc, #0x38c]
007c6054: sub      sp, sp, #0x18c
007c6058: add      r6, pc, r6
007c605c: str      ip, [sp, #0xc]
007c6060: ldr      ip, [r6, ip]
007c6064: mov      r8, r2
007c6068: add      r5, sp, #0x170
007c606c: ldr      r2, [ip]
007c6070: str      r0, [sp, #0x10]
007c6074: mov      r0, r5
007c6078: mov      sl, r3
007c607c: str      r2, [sp, #0x184]
007c6080: mov      fp, r1
007c6084: bl       #0x75302c
007c6088: cmp      r8, #0
007c608c: bne      #0x7c6284
007c6090: cmp      sl, #0
007c6094: bne      #0x7c6270
007c6098: ldr      lr, [sp, #0x10]
007c609c: add      r4, sp, #0x15c
007c60a0: mov      r3, #0
007c60a4: add      r7, lr, #4
007c60a8: add      r2, sp, #0x1c
007c60ac: mov      r1, r5
007c60b0: mov      r0, r4
007c60b4: str      r3, [sp, #0x1c]
007c60b8: str      r2, [sp, #0x14]
007c60bc: bl       #0x75302c
007c60c0: mov      r1, r4
007c60c4: mov      r0, r7
007c60c8: ldr      r2, [sp, #0x14]
007c60cc: bl       #0x7c4c44
007c60d0: ldrb     ip, [sp, #0x15c]
007c60d4: mov      sb, r0
007c60d8: sxtb     r3, ip
007c60dc: cmn      r3, #1
007c60e0: beq      #0x7c62a8
007c60e4: cmp      sb, #0
007c60e8: bne      #0x7c61f0
007c60ec: add      r4, sp, #0x20
007c60f0: mov      r1, sb
007c60f4: mov      r2, #0x100
007c60f8: mov      r0, r4
007c60fc: bl       #0x30e460
007c6100: ldrsb    r3, [fp]
007c6104: mov      ip, #0x100
007c6108: mov      r1, r8
007c610c: cmn      r3, #1
007c6110: ldreq    r0, [fp, #0xc]
007c6114: addne    r0, fp, #1
007c6118: mov      r2, sl
007c611c: mov      r3, r4
007c6120: str      ip, [sp]
007c6124: bl       #0x42b38c
007c6128: cmp      r0, #0
007c612c: beq      #0x7c61bc
007c6130: ldr      r1, [pc, #0x2b0]
007c6134: mov      r0, r4
007c6138: add      r1, pc, r1
007c613c: bl       #0x30ebd4
007c6140: cmp      r0, #0
007c6144: beq      #0x7c62b8
007c6148: ldr      lr, [sp, #0x10]
007c614c: ldr      sb, [lr, #4]
007c6150: cmp      sb, #0
007c6154: beq      #0x7c6178
007c6158: ldr      r3, [sb, #4]
007c615c: cmp      r3, #0
007c6160: movlt    sl, #0
007c6164: bge      #0x7c6238
007c6168: cmp      r7, #0
007c616c: beq      #0x7c6178
007c6170: cmp      sb, #0
007c6174: bne      #0x7c62e4
007c6178: ldr      ip, [sp, #0x10]
007c617c: add      r8, sp, #0x134
007c6180: mov      r1, r4
007c6184: ldr      r3, [ip]
007c6188: mov      r0, r8
007c618c: ldr      r4, [r3, #8]
007c6190: bl       #0x413a7c
007c6194: mov      r1, r8
007c6198: ldr      r0, [sp, #0x10]
007c619c: blx      r4
007c61a0: mov      r1, r0
007c61a4: ldr      r0, [sp, #0x14]
007c61a8: bl       #0x7c4c04
007c61ac: ldrb     lr, [sp, #0x134]
007c61b0: sxtb     r3, lr
007c61b4: cmn      r3, #1
007c61b8: beq      #0x7c63cc
007c61bc: add      r4, sp, #0x120
007c61c0: mov      r1, r5
007c61c4: mov      r0, r4
007c61c8: bl       #0x75302c
007c61cc: mov      r1, r4
007c61d0: mov      r0, r7
007c61d4: bl       #0x7c5fe0
007c61d8: ldr      r1, [sp, #0x1c]
007c61dc: bl       #0x7c4c04
007c61e0: ldrb     r2, [sp, #0x120]
007c61e4: sxtb     r3, r2
007c61e8: cmn      r3, #1
007c61ec: beq      #0x7c62d4
007c61f0: ldr      r8, [sp, #0x1c]
007c61f4: mov      r0, r8
007c61f8: cmp      r0, #0
007c61fc: beq      #0x7c6204
007c6200: bl       #0x75a240
007c6204: ldrb     ip, [sp, #0x170]
007c6208: sxtb     r3, ip
007c620c: cmn      r3, #1
007c6210: beq      #0x7c6298
007c6214: ldr      r2, [sp, #0xc]
007c6218: mov      r0, r8
007c621c: ldr      r3, [r6, r2]
007c6220: ldr      r2, [sp, #0x184]
007c6224: ldr      r3, [r3]
007c6228: cmp      r2, r3
007c622c: bne      #0x7c63dc
007c6230: add      sp, sp, #0x18c
007c6234: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007c6238: mov      r2, #8
007c623c: mov      sl, #0
007c6240: ldr      r1, [sb, r2]
007c6244: add      r0, sb, r2
007c6248: cmn      r1, #2
007c624c: beq      #0x7c625c
007c6250: ldr      r1, [r0, #4]
007c6254: cmn      r1, #1
007c6258: bne      #0x7c6168
007c625c: add      sl, sl, #1
007c6260: cmp      sl, r3
007c6264: add      r2, r2, #0x20
007c6268: ble      #0x7c6240
007c626c: b        #0x7c6168
007c6270: ldr      r1, [pc, #0x174]
007c6274: mov      r0, r5
007c6278: add      r1, pc, r1
007c627c: bl       #0x7521cc
007c6280: b        #0x7c6098
007c6284: ldr      r1, [pc, #0x164]
007c6288: mov      r0, r5
007c628c: add      r1, pc, r1
007c6290: bl       #0x7521cc
007c6294: b        #0x7c6090
007c6298: ldr      r0, [sp, #0x17c]
007c629c: ldr      r1, [sp, #0x178]
007c62a0: bl       #0x752b38
007c62a4: b        #0x7c6214
007c62a8: ldr      r0, [sp, #0x168]
007c62ac: ldr      r1, [sp, #0x164]
007c62b0: bl       #0x752b38
007c62b4: b        #0x7c60e4
007c62b8: ldr      r1, [pc, #0x134]
007c62bc: mov      r0, r4
007c62c0: add      r1, pc, r1
007c62c4: bl       #0x30ebd4
007c62c8: cmp      r0, #0
007c62cc: bne      #0x7c6148
007c62d0: b        #0x7c61bc
007c62d4: ldr      r0, [sp, #0x12c]
007c62d8: ldr      r1, [sp, #0x128]
007c62dc: bl       #0x752b38
007c62e0: b        #0x7c61f0
007c62e4: ldr      fp, [sb, #4]
007c62e8: cmp      fp, sl
007c62ec: blt      #0x7c6178
007c62f0: add      r3, sb, sl, lsl #5
007c62f4: ldr      r8, [r3, #0x24]
007c62f8: cmp      r8, #0
007c62fc: beq      #0x7c6388
007c6300: ldrsb    r3, [r8, #0x10]
007c6304: mov      r1, r4
007c6308: cmn      r3, #1
007c630c: addne    r0, r8, #0x11
007c6310: ldreq    r0, [r8, #0x1c]
007c6314: bl       #0x30e31c
007c6318: cmp      r0, #0
007c631c: bne      #0x7c6388
007c6320: add      sl, sp, #0x148
007c6324: mov      r1, r5
007c6328: add      r4, sp, #0x188
007c632c: mov      r0, sl
007c6330: bl       #0x75302c
007c6334: str      r8, [r4, #-0x170]!
007c6338: mov      r0, r8
007c633c: bl       #0x759c64
007c6340: mov      r0, r7
007c6344: mov      r1, sl
007c6348: mov      r2, r4
007c634c: bl       #0x7c5c90
007c6350: ldr      r0, [sp, #0x18]
007c6354: cmp      r0, #0
007c6358: beq      #0x7c6360
007c635c: bl       #0x75a240
007c6360: ldrb     r2, [sp, #0x148]
007c6364: sxtb     r3, r2
007c6368: cmn      r3, #1
007c636c: ldrne    r0, [sp, #0x1c]
007c6370: bne      #0x7c61f8
007c6374: ldr      r0, [sp, #0x154]
007c6378: ldr      r1, [sp, #0x150]
007c637c: bl       #0x752b38
007c6380: ldr      r0, [sp, #0x1c]
007c6384: b        #0x7c61f8
007c6388: add      sl, sl, #1
007c638c: cmp      sl, fp
007c6390: lslle    r3, sl, #5
007c6394: addle    r3, r3, #8
007c6398: bgt      #0x7c62e8
007c639c: ldr      r2, [sb, r3]
007c63a0: add      r1, sb, r3
007c63a4: cmn      r2, #2
007c63a8: beq      #0x7c63b8
007c63ac: ldr      r2, [r1, #4]
007c63b0: cmn      r2, #1
007c63b4: bne      #0x7c62e8
007c63b8: add      sl, sl, #1
007c63bc: cmp      sl, fp
007c63c0: add      r3, r3, #0x20
007c63c4: ble      #0x7c639c
007c63c8: b        #0x7c62e8
007c63cc: ldr      r0, [sp, #0x140]
007c63d0: ldr      r1, [sp, #0x13c]
007c63d4: bl       #0x752b38
007c63d8: b        #0x7c61bc
007c63dc: bl       #0x30e310
007c63e0: andseq   lr, ip, r8, lsr sl
007c63e4: andeq    r4, r0, ip, lsr #1
007c63e8: andseq   r4, r4, r8, asr #31
007c63ec: andseq   r4, r4, r0, lsl #29
007c63f0: andseq   sp, r2, r4, lsr #23
007c63f4: andseq   r4, r4, r8, asr #28

# _ZN7gameswf26default_bitmap_font_entity14get_char_imageEPNS_17bitmap_glyph_dataEtiPNS_20bitmap_glyph_metricsE
007c4698: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007c469c: mov      r4, r0
007c46a0: ldr      r0, [r0, #0x54]
007c46a4: sub      sp, sp, #0x44
007c46a8: str      r3, [sp, #0x38]
007c46ac: ldrb     sl, [r0, #0x25]
007c46b0: ldrb     r3, [r0, #0x24]
007c46b4: mov      r5, r1
007c46b8: ldrb     r1, [r0, #0x27]
007c46bc: ldrb     r6, [r0, #0x26]
007c46c0: lsl      sl, sl, #0x10
007c46c4: ldrb     r7, [r0, #0xd]
007c46c8: orr      sl, sl, r3, lsl #24
007c46cc: ldrb     r8, [r0, #0xc]
007c46d0: orr      r1, sl, r1
007c46d4: ldrb     ip, [r0, #0xf]
007c46d8: orr      r6, r1, r6, lsl #8
007c46dc: ldrb     r1, [r0, #0x10]
007c46e0: ldrb     r3, [r0, #0xe]
007c46e4: lsl      r7, r7, #0x10
007c46e8: orr      r7, r7, r8, lsl #24
007c46ec: str      r1, [sp, #0xc]
007c46f0: orr      ip, r7, ip
007c46f4: orr      ip, ip, r3, lsl #8
007c46f8: ldrb     r3, [r0, #0x12]
007c46fc: ldrb     r8, [r0, #0x11]
007c4700: rsb      r2, r6, r2
007c4704: str      r3, [sp, #0x10]
007c4708: ldrb     r1, [r0, #0x13]
007c470c: cmp      r2, ip
007c4710: movlt    ip, #0
007c4714: movge    ip, #1
007c4718: orrs     ip, ip, r2, lsr #31
007c471c: str      r1, [sp, #0x14]
007c4720: ldrb     r3, [r0, #0x14]
007c4724: str      r3, [sp]
007c4728: ldrb     r1, [r0, #0x16]
007c472c: ldrb     r7, [r0, #0x15]
007c4730: str      r1, [sp, #4]
007c4734: ldrb     r3, [r0, #0x17]
007c4738: str      r3, [sp, #8]
007c473c: ldrb     r1, [r0, #0x18]
007c4740: str      r1, [sp, #0x28]
007c4744: ldrb     r3, [r0, #0x19]
007c4748: str      r3, [sp, #0x2c]
007c474c: ldrb     r1, [r0, #0x1a]
007c4750: str      r1, [sp, #0x30]
007c4754: ldrb     r3, [r0, #0x1b]
007c4758: str      r3, [sp, #0x34]
007c475c: ldrb     r1, [r0, #0x20]
007c4760: str      r1, [sp, #0x18]
007c4764: ldrb     r3, [r0, #0x21]
007c4768: str      r3, [sp, #0x1c]
007c476c: ldrb     r1, [r0, #0x22]
007c4770: str      r1, [sp, #0x20]
007c4774: ldrb     r3, [r0, #0x23]
007c4778: str      r3, [sp, #0x24]
007c477c: bne      #0x7c49bc
007c4780: add      ip, r2, #0xb
007c4784: add      r1, r2, #0xa
007c4788: add      r2, r0, r1, lsl #2
007c478c: add      r3, r0, ip, lsl #2
007c4790: ldrb     sl, [r0, ip, lsl #2]
007c4794: ldrb     fp, [r0, r1, lsl #2]
007c4798: ldrb     sb, [r2, #3]
007c479c: ldrb     ip, [r3, #3]
007c47a0: ldrb     r0, [r2, #1]
007c47a4: ldrb     r1, [r3, #1]
007c47a8: ldrb     r2, [r2, #2]
007c47ac: ldrb     r3, [r3, #2]
007c47b0: orr      sb, sb, fp, lsl #24
007c47b4: orr      ip, ip, sl, lsl #24
007c47b8: orr      sb, sb, r0, lsl #16
007c47bc: orr      ip, ip, r1, lsl #16
007c47c0: orr      sb, sb, r2, lsl #8
007c47c4: orr      ip, ip, r3, lsl #8
007c47c8: subs     r6, ip, sb
007c47cc: beq      #0x7c49bc
007c47d0: ldr      r3, [r4, #0x5c]
007c47d4: cmp      r3, #0
007c47d8: beq      #0x7c4a3c
007c47dc: ldr      r6, [r4, #0x4c]
007c47e0: ldr      r3, [r3, #8]
007c47e4: rsb      r6, r6, sb
007c47e8: add      r6, r3, r6
007c47ec: ldr      r1, [sp, #0xc]
007c47f0: ldr      r3, [sp, #0x14]
007c47f4: ldr      r2, [sp]
007c47f8: lsl      r8, r8, #0x10
007c47fc: orr      r8, r8, r1, lsl #24
007c4800: ldr      r1, [sp, #8]
007c4804: orr      r8, r8, r3
007c4808: lsl      r7, r7, #0x10
007c480c: ldr      r3, [sp, #4]
007c4810: orr      r7, r7, r2, lsl #24
007c4814: orr      r7, r7, r1
007c4818: orr      r7, r7, r3, lsl #8
007c481c: str      r7, [sp]
007c4820: ldrb     r1, [r6]
007c4824: ldr      r2, [sp, #0x10]
007c4828: cmp      r5, #0
007c482c: str      r1, [sp, #0xc]
007c4830: orr      fp, r8, r2, lsl #8
007c4834: ldrb     r2, [r6, #1]
007c4838: str      r2, [sp, #0x10]
007c483c: ldrb     r3, [r6, #2]
007c4840: str      r3, [sp, #4]
007c4844: ldrb     r1, [r6, #3]
007c4848: str      r1, [sp, #8]
007c484c: beq      #0x7c48ec
007c4850: mul      r7, fp, r7
007c4854: ldr      r8, [r4, #0x30]
007c4858: cmp      r7, r8
007c485c: bgt      #0x7c4a08
007c4860: cmp      r7, #0
007c4864: ble      #0x7c48d0
007c4868: mov      r1, #0
007c486c: mov      r3, #4
007c4870: str      fp, [sp, #0x14]
007c4874: str      r5, [sp, #0x3c]
007c4878: ldrb     ip, [r6, r3]
007c487c: mov      r2, #0
007c4880: add      r3, r3, #1
007c4884: and      r5, ip, #0x7f
007c4888: lsl      r0, r1, #2
007c488c: and      ip, ip, #0x80
007c4890: mov      r8, r2
007c4894: cmp      ip, #0
007c4898: beq      #0x7c49c8
007c489c: cmp      r2, #0
007c48a0: beq      #0x7c49c8
007c48a4: ldr      sb, [r4, #0x2c]
007c48a8: add      r2, r2, #1
007c48ac: cmp      r5, r2
007c48b0: str      r8, [sb, r0]
007c48b4: add      r1, r1, #1
007c48b8: add      r0, r0, #4
007c48bc: bge      #0x7c4894
007c48c0: cmp      r7, r1
007c48c4: bgt      #0x7c4878
007c48c8: ldr      fp, [sp, #0x14]
007c48cc: ldr      r5, [sp, #0x3c]
007c48d0: lsl      r3, fp, #2
007c48d4: str      r3, [r5]
007c48d8: ldr      r3, [r4, #0x2c]
007c48dc: ldr      r2, [sp]
007c48e0: str      fp, [r5, #4]
007c48e4: str      r3, [r5, #0xc]
007c48e8: str      r2, [r5, #8]
007c48ec: ldr      r3, [sp, #0x68]
007c48f0: cmp      r3, #0
007c48f4: beq      #0x7c49b4
007c48f8: ldr      lr, [sp, #0x1c]
007c48fc: ldr      r1, [sp, #0x18]
007c4900: ldr      r2, [sp, #0x24]
007c4904: lsl      r3, lr, #0x10
007c4908: ldr      lr, [sp, #0x2c]
007c490c: orr      r3, r3, r1, lsl #24
007c4910: orr      r3, r3, r2
007c4914: lsl      r1, lr, #0x10
007c4918: ldr      lr, [sp, #0x20]
007c491c: ldr      r2, [sp, #0x28]
007c4920: orr      r0, r3, lr, lsl #8
007c4924: ldr      r3, [sp, #8]
007c4928: ldr      lr, [sp, #4]
007c492c: orr      r1, r1, r2, lsl #24
007c4930: ldr      r2, [sp, #0x34]
007c4934: orr      ip, r3, lr, lsl #8
007c4938: ldr      r3, [sp, #0x10]
007c493c: ldr      lr, [sp, #0xc]
007c4940: orr      r1, r1, r2
007c4944: add      r0, r0, #1
007c4948: orr      r2, r3, lr, lsl #8
007c494c: ldr      r3, [sp, #0x30]
007c4950: ldr      lr, [sp, #0x68]
007c4954: add      r0, r0, ip
007c4958: orr      r1, r1, r3, lsl #8
007c495c: stmib    lr, {r1, fp}
007c4960: ldr      r1, [sp]
007c4964: rsb      r0, r2, r0
007c4968: str      r2, [lr]
007c496c: str      r1, [lr, #0xc]
007c4970: bl       #0x30e964
007c4974: mov      r4, r0
007c4978: ldr      r0, [sp, #0x38]
007c497c: bl       #0x30e964
007c4980: mov      r1, #0x41000000
007c4984: add      r1, r1, #0xa00000
007c4988: bl       #0x30ed6c
007c498c: mov      r1, r0
007c4990: mov      r0, #0x44000000
007c4994: add      r0, r0, #0x800000
007c4998: bl       #0x30ec94
007c499c: mov      r1, r0
007c49a0: mov      r0, r4
007c49a4: bl       #0x30ed6c
007c49a8: bl       #0x30e4cc
007c49ac: ldr      r2, [sp, #0x68]
007c49b0: str      r0, [r2, #0x10]
007c49b4: mov      r0, #1
007c49b8: b        #0x7c49c0
007c49bc: mov      r0, #0
007c49c0: add      sp, sp, #0x44
007c49c4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007c49c8: add      r8, r6, r3
007c49cc: ldrb     sb, [r6, r3]
007c49d0: ldrb     sl, [r8, #3]
007c49d4: ldrb     fp, [r8, #1]
007c49d8: ldrb     r8, [r8, #2]
007c49dc: orr      sl, sl, sb, lsl #24
007c49e0: orr      fp, sl, fp, lsl #16
007c49e4: orr      fp, fp, r8, lsl #8
007c49e8: and      sl, fp, #0xff00
007c49ec: lsl      r8, fp, #0x18
007c49f0: orr      r8, r8, fp, lsr #24
007c49f4: orr      sl, r8, sl, lsl #8
007c49f8: and      r8, fp, #0xff0000
007c49fc: orr      r8, sl, r8, lsr #8
007c4a00: add      r3, r3, #4
007c4a04: b        #0x7c48a4
007c4a08: cmp      r7, #0
007c4a0c: add      sl, r4, #0x2c
007c4a10: bne      #0x7c4a78
007c4a14: lsl      r3, r8, #2
007c4a18: mov      r1, #0
007c4a1c: ldr      r2, [sl]
007c4a20: add      r8, r8, #1
007c4a24: cmp      r8, r7
007c4a28: str      r1, [r2, r3]
007c4a2c: add      r3, r3, #4
007c4a30: bne      #0x7c4a1c
007c4a34: str      r7, [r4, #0x30]
007c4a38: b        #0x7c4860
007c4a3c: ldr      r3, [r4, #0x3c]
007c4a40: cmp      r6, r3
007c4a44: addle    sl, r4, #0x3c
007c4a48: bgt      #0x7c4a94
007c4a4c: ldr      r3, [r4, #0x60]
007c4a50: mov      r0, sb
007c4a54: ldr      r1, [r3]
007c4a58: mov      lr, pc
007c4a5c: ldr      pc, [r3, #0x10]
007c4a60: mov      r2, r6
007c4a64: mov      r1, sl
007c4a68: ldr      r0, [r4, #0x60]
007c4a6c: bl       #0x7b6a80
007c4a70: ldr      r6, [r4, #0x44]
007c4a74: b        #0x7c47ec
007c4a78: ldr      r3, [r4, #0x34]
007c4a7c: cmp      r7, r3
007c4a80: ble      #0x7c4a14
007c4a84: mov      r0, sl
007c4a88: add      r1, r7, r7, asr #1
007c4a8c: bl       #0x7b802c
007c4a90: b        #0x7c4a14
007c4a94: add      sl, r4, #0x3c
007c4a98: mov      r0, sl
007c4a9c: mov      r1, r6
007c4aa0: bl       #0x75ae5c
007c4aa4: b        #0x7c4a4c

# _ZN7gameswf29default_bitmap_glyph_provider20get_font_entity_implERKNS_9tu_stringE
007a9634: push     {r4, r5, r6, lr}
007a9638: mov      r6, r0
007a963c: mov      r5, r1
007a9640: mov      r0, #0x64
007a9644: mov      r1, #0
007a9648: bl       #0x752ba8
007a964c: mov      r1, r6
007a9650: mov      r4, r0
007a9654: mov      r2, r5
007a9658: bl       #0x7c68c4
007a965c: mov      r0, r4
007a9660: pop      {r4, r5, r6, pc}

# _ZNK7gameswf4font9get_glyphEPNS_5glyphEti
007d01bc: push     {r4, r5, r6, r7, r8, sl, lr}
007d01c0: mov      r4, r1
007d01c4: mov      r1, #0x44000000
007d01c8: str      r1, [r4]
007d01cc: mvn      r1, #0
007d01d0: strh     r1, [r4, #0x1e]
007d01d4: ldr      r1, [r0, #0x1c]
007d01d8: sub      sp, sp, #0x14
007d01dc: mov      r5, r0
007d01e0: cmp      r1, #0
007d01e4: mov      r6, r2
007d01e8: mov      r7, r3
007d01ec: beq      #0x7d0200
007d01f0: ldr      r0, [r0, #0x18]
007d01f4: ldrb     r3, [r0, #4]
007d01f8: cmp      r3, #0
007d01fc: beq      #0x7d0338
007d0200: ldr      r3, [r1, #0xac]
007d0204: ldr      r0, [r3, #0x10]
007d0208: cmp      r0, #0
007d020c: beq      #0x7d0294
007d0210: mov      r3, #0
007d0214: strb     r3, [r4, #0x22]
007d0218: add      r1, r5, #0x30
007d021c: ldrb     r2, [r5, #0x4d]
007d0220: ldrb     r3, [r5, #0x4c]
007d0224: bl       #0x7c6048
007d0228: cmp      r0, #0
007d022c: str      r0, [r4, #0x18]
007d0230: beq      #0x7d0290
007d0234: add      r3, r4, #8
007d0238: mov      r1, r6
007d023c: mov      r2, r7
007d0240: str      r4, [sp]
007d0244: bl       #0x7c59dc
007d0248: mov      r1, r0
007d024c: add      r0, r4, #4
007d0250: bl       #0x77a740
007d0254: ldr      r3, [r4, #4]
007d0258: cmp      r3, #0
007d025c: beq      #0x7d0290
007d0260: ldr      r3, [r5, #0x7c]
007d0264: cmp      r3, #0
007d0268: moveq    r0, #1
007d026c: beq      #0x7d0288
007d0270: mov      r1, #0x41000000
007d0274: ldr      r0, [r4]
007d0278: add      r1, r1, #0xa00000
007d027c: bl       #0x30ed6c
007d0280: str      r0, [r4]
007d0284: mov      r0, #1
007d0288: add      sp, sp, #0x14
007d028c: pop      {r4, r5, r6, r7, r8, sl, pc}
007d0290: ldr      r1, [r5, #0x1c]
007d0294: cmp      r1, #0
007d0298: beq      #0x7d02ac
007d029c: ldr      r0, [r5, #0x18]
007d02a0: ldrb     r3, [r0, #4]
007d02a4: cmp      r3, #0
007d02a8: beq      #0x7d0360
007d02ac: ldr      r3, [r1, #0xac]
007d02b0: ldr      r8, [r3, #0xc]
007d02b4: cmp      r8, #0
007d02b8: beq      #0x7d0320
007d02bc: mov      r3, #0
007d02c0: strb     r3, [r4, #0x22]
007d02c4: ldrb     lr, [r5, #0x4c]
007d02c8: add      sl, r5, #0x30
007d02cc: ldrb     r3, [r5, #0x4d]
007d02d0: add      ip, r4, #8
007d02d4: mov      r2, sl
007d02d8: mov      r1, r6
007d02dc: mov      r0, r8
007d02e0: str      lr, [sp]
007d02e4: stmib    sp, {r7, ip}
007d02e8: str      r4, [sp, #0xc]
007d02ec: bl       #0x7d1614
007d02f0: mov      r1, r0
007d02f4: add      r0, r4, #4
007d02f8: bl       #0x77a740
007d02fc: ldrb     r3, [r5, #0x4c]
007d0300: mov      r0, r8
007d0304: mov      r1, sl
007d0308: ldrb     r2, [r5, #0x4d]
007d030c: bl       #0x7d113c
007d0310: ldr      r3, [r4, #4]
007d0314: str      r0, [r4, #0x18]
007d0318: cmp      r3, #0
007d031c: bne      #0x7d0260
007d0320: ldr      r3, [r5, #0x50]
007d0324: add      r2, r5, #0x50
007d0328: cmp      r3, #0
007d032c: bne      #0x7d0388
007d0330: mov      r0, #0
007d0334: b        #0x7d0288
007d0338: ldr      r1, [r0]
007d033c: sub      r1, r1, #1
007d0340: cmp      r1, #0
007d0344: str      r1, [r0]
007d0348: bne      #0x7d0350
007d034c: bl       #0x752b38
007d0350: mov      r1, #0
007d0354: str      r1, [r5, #0x18]
007d0358: str      r1, [r5, #0x1c]
007d035c: b        #0x7d0200
007d0360: ldr      r1, [r0]
007d0364: sub      r1, r1, #1
007d0368: cmp      r1, #0
007d036c: str      r1, [r0]
007d0370: bne      #0x7d0378
007d0374: bl       #0x752b38
007d0378: mov      r1, #0
007d037c: str      r1, [r5, #0x18]
007d0380: str      r1, [r5, #0x1c]
007d0384: b        #0x7d02ac
007d0388: ldr      ip, [r3, #4]
007d038c: mov      r0, #0xc
007d0390: and      r1, r6, ip
007d0394: mul      r0, r0, r1
007d0398: add      r0, r0, #8
007d039c: ldr      r7, [r3, r0]
007d03a0: add      r0, r3, r0
007d03a4: cmn      r7, #2
007d03a8: beq      #0x7d0330
007d03ac: ldr      r7, [r0, #4]
007d03b0: cmn      r7, #1
007d03b4: beq      #0x7d03c4
007d03b8: and      ip, ip, r7
007d03bc: cmp      r1, ip
007d03c0: bne      #0x7d0330
007d03c4: mov      r8, #0xc
007d03c8: b        #0x7d03ec
007d03cc: ldr      r1, [r0]
007d03d0: cmn      r1, #1
007d03d4: beq      #0x7d0330
007d03d8: mul      r0, r8, r1
007d03dc: ldr      ip, [r2]
007d03e0: add      r0, r0, #8
007d03e4: add      r0, ip, r0
007d03e8: ldr      r7, [r0, #4]
007d03ec: cmp      r6, r7
007d03f0: bne      #0x7d03cc
007d03f4: ldrh     ip, [r0, #8]
007d03f8: cmp      ip, r6
007d03fc: bne      #0x7d03cc
007d0400: cmp      r1, #0
007d0404: blt      #0x7d0330
007d0408: mov      r2, #0xc
007d040c: mla      r3, r2, r1, r3
007d0410: ldrh     r3, [r3, #0x12]
007d0414: strh     r3, [r4, #0x1e]
007d0418: ldr      r2, [r5, #0x64]
007d041c: sxth     r3, r3
007d0420: cmp      r3, r2
007d0424: bge      #0x7d0260
007d0428: ldr      r2, [r5, #0x60]
007d042c: mov      r0, #1
007d0430: ldr      r3, [r2, r3, lsl #2]
007d0434: str      r3, [r4]
007d0438: b        #0x7d0288

# _ZN21render_handler_glitch11draw_bitmapERKN7gameswf6matrixEPNS0_11bitmap_infoERKNS0_4rectES8_NS0_4rgbaE
007d8df4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007d8df8: sub      sp, sp, #0x54
007d8dfc: ldrb     r6, [sp, #0x7f]
007d8e00: mov      r4, r0
007d8e04: mov      r5, r1
007d8e08: mov      r0, r6
007d8e0c: str      r2, [sp, #0x1c]
007d8e10: mov      sb, r3
007d8e14: bl       #0x30e964
007d8e18: mov      r1, #0
007d8e1c: bl       #0x30df8c
007d8e20: cmp      r0, #0
007d8e24: ldrb     r0, [sp, #0x7c]
007d8e28: ldr      r7, [sp, #0x78]
007d8e2c: ldrb     sl, [sp, #0x7d]
007d8e30: str      r0, [sp, #0x2c]
007d8e34: ldrb     r8, [sp, #0x7e]
007d8e38: bne      #0x7d934c
007d8e3c: ldr      r3, [r5]
007d8e40: ldr      r0, [sb]
007d8e44: mov      r1, r3
007d8e48: str      r3, [sp, #8]
007d8e4c: bl       #0x30ed6c
007d8e50: str      r0, [sp, #0x28]
007d8e54: ldr      r1, [r5, #4]
007d8e58: str      r1, [sp, #0x20]
007d8e5c: ldr      r0, [sb, #8]
007d8e60: bl       #0x30ed6c
007d8e64: ldr      fp, [r5, #8]
007d8e68: mov      r2, r0
007d8e6c: mov      r1, r2
007d8e70: ldr      r0, [sp, #0x28]
007d8e74: str      r2, [sp, #0x10]
007d8e78: bl       #0x30eba4
007d8e7c: mov      r1, fp
007d8e80: bl       #0x30eba4
007d8e84: str      r0, [sp, #0x30]
007d8e88: ldr      ip, [r5, #0xc]
007d8e8c: ldr      r0, [sb]
007d8e90: mov      r1, ip
007d8e94: str      ip, [sp, #0xc]
007d8e98: bl       #0x30ed6c
007d8e9c: str      r0, [sp, #0x38]
007d8ea0: ldr      r0, [r5, #0x10]
007d8ea4: str      r0, [sp, #0x24]
007d8ea8: ldr      r0, [sb, #8]
007d8eac: ldr      r1, [sp, #0x24]
007d8eb0: bl       #0x30ed6c
007d8eb4: str      r0, [sp, #0x3c]
007d8eb8: ldr      r5, [r5, #0x14]
007d8ebc: ldr      r1, [sp, #0x3c]
007d8ec0: ldr      r0, [sp, #0x38]
007d8ec4: bl       #0x30eba4
007d8ec8: mov      r1, r5
007d8ecc: bl       #0x30eba4
007d8ed0: ldr      r3, [sp, #8]
007d8ed4: str      r0, [sp, #0x34]
007d8ed8: ldr      r0, [sb, #4]
007d8edc: mov      r1, r3
007d8ee0: bl       #0x30ed6c
007d8ee4: ldr      r2, [sp, #0x10]
007d8ee8: mov      r1, r0
007d8eec: mov      r0, r2
007d8ef0: bl       #0x30eba4
007d8ef4: mov      r1, r0
007d8ef8: mov      r0, fp
007d8efc: bl       #0x30eba4
007d8f00: ldr      ip, [sp, #0xc]
007d8f04: str      r0, [sp, #0x14]
007d8f08: ldr      r0, [sb, #4]
007d8f0c: mov      r1, ip
007d8f10: bl       #0x30ed6c
007d8f14: mov      r1, r0
007d8f18: ldr      r0, [sp, #0x3c]
007d8f1c: bl       #0x30eba4
007d8f20: mov      r1, r0
007d8f24: mov      r0, r5
007d8f28: bl       #0x30eba4
007d8f2c: str      r0, [sp, #0x18]
007d8f30: ldr      r3, [sb, #0xc]
007d8f34: ldr      r1, [sp, #0x20]
007d8f38: mov      r0, r3
007d8f3c: str      r3, [sp, #8]
007d8f40: bl       #0x30ed6c
007d8f44: mov      r1, r0
007d8f48: ldr      r0, [sp, #0x28]
007d8f4c: bl       #0x30eba4
007d8f50: mov      r1, r0
007d8f54: mov      r0, fp
007d8f58: bl       #0x30eba4
007d8f5c: ldr      r3, [sp, #8]
007d8f60: ldr      r1, [sp, #0x24]
007d8f64: mov      sb, r0
007d8f68: mov      r0, r3
007d8f6c: bl       #0x30ed6c
007d8f70: mov      r1, r0
007d8f74: ldr      r0, [sp, #0x38]
007d8f78: bl       #0x30eba4
007d8f7c: mov      r1, r0
007d8f80: mov      r0, r5
007d8f84: bl       #0x30eba4
007d8f88: mov      r1, sb
007d8f8c: mov      r5, r0
007d8f90: ldr      r0, [sp, #0x14]
007d8f94: bl       #0x30eba4
007d8f98: ldr      r1, [sp, #0x30]
007d8f9c: bl       #0x30e3ac
007d8fa0: mov      r1, r5
007d8fa4: str      r0, [sp, #0x28]
007d8fa8: ldr      r0, [sp, #0x18]
007d8fac: bl       #0x30eba4
007d8fb0: ldr      r1, [sp, #0x34]
007d8fb4: bl       #0x30e3ac
007d8fb8: ldr      r1, [sp, #0x1c]
007d8fbc: str      r0, [sp, #0x24]
007d8fc0: ldr      r3, [r1]
007d8fc4: mov      r0, r1
007d8fc8: mov      lr, pc
007d8fcc: ldr      pc, [r3, #8]
007d8fd0: ldr      r2, [sp, #0x1c]
007d8fd4: ldr      r0, [r2, #0x10]
007d8fd8: cmp      r0, #0
007d8fdc: beq      #0x7d8fe8
007d8fe0: mov      r1, #1
007d8fe4: bl       #0x7d3bb4
007d8fe8: ldr      ip, [sp, #0x1c]
007d8fec: add      r3, r4, #0x1f0
007d8ff0: mov      r0, r3
007d8ff4: add      r1, ip, #0x10
007d8ff8: str      r3, [sp, #0x20]
007d8ffc: bl       #0x7d6a48
007d9000: ldr      r3, [r4, #0x374]
007d9004: ldr      r2, [r4, #0x348]
007d9008: ldr      r0, [sp, #0x30]
007d900c: movw     fp, #0x6667
007d9010: str      r2, [r3, #0x14]
007d9014: str      r0, [r3, #0xc]
007d9018: ldr      r1, [sp, #0x34]
007d901c: movt     fp, #0x6666
007d9020: str      r1, [r3, #0x10]
007d9024: ldr      r3, [r4, #0x374]
007d9028: ldr      r2, [r4, #0x348]
007d902c: add      r3, r3, #0x18
007d9030: str      r2, [r3, #0x14]
007d9034: ldr      r2, [sp, #0x14]
007d9038: str      r2, [r3, #0xc]
007d903c: ldr      ip, [sp, #0x18]
007d9040: str      ip, [r3, #0x10]
007d9044: ldr      r2, [r4, #0x374]
007d9048: ldr      r1, [r4, #0x348]
007d904c: mov      r3, #0
007d9050: add      r2, r2, #0x30
007d9054: str      r5, [r2, #0x10]
007d9058: str      r1, [r2, #0x14]
007d905c: str      sb, [r2, #0xc]
007d9060: ldr      r2, [r4, #0x374]
007d9064: ldr      r1, [r4, #0x348]
007d9068: mov      r5, #0x14
007d906c: add      r2, r2, #0x48
007d9070: str      r1, [r2, #0x14]
007d9074: ldr      r0, [sp, #0x28]
007d9078: str      r0, [r2, #0xc]
007d907c: ldr      r1, [sp, #0x24]
007d9080: str      r1, [r2, #0x10]
007d9084: ldr      r0, [r7]
007d9088: ldr      r1, [r7, #8]
007d908c: ldr      r2, [r4, #0x374]
007d9090: str      r0, [r2]
007d9094: str      r1, [r2, #4]
007d9098: ldr      r1, [r7, #8]
007d909c: ldr      r2, [r4, #0x374]
007d90a0: ldr      r0, [r7, #4]
007d90a4: str      r0, [r2, #0x18]
007d90a8: str      r1, [r2, #0x1c]
007d90ac: ldr      r1, [r7, #0xc]
007d90b0: ldr      r0, [r7]
007d90b4: ldr      r2, [r4, #0x374]
007d90b8: str      r0, [r2, #0x30]
007d90bc: str      r1, [r2, #0x34]
007d90c0: ldr      r0, [r7, #0xc]
007d90c4: ldr      r1, [r7, #4]
007d90c8: ldr      r2, [r4, #0x374]
007d90cc: mov      r7, r3
007d90d0: str      r0, [r2, #0x4c]
007d90d4: str      r1, [r2, #0x48]
007d90d8: str      fp, [sp, #0x14]
007d90dc: mov      fp, r6
007d90e0: ldr      r6, [sp, #0x2c]
007d90e4: ldr      r3, [r4, #0x374]
007d90e8: add      r3, r3, r7
007d90ec: strb     r6, [r3, #8]
007d90f0: strb     fp, [r3, #0xb]
007d90f4: strb     r8, [r3, #0xa]
007d90f8: strb     sl, [r3, #9]
007d90fc: ldrb     r3, [r4, #4]
007d9100: cmp      r3, #0
007d9104: beq      #0x7d9168
007d9108: ldr      sb, [r4, #0x374]
007d910c: add      sb, sb, r7
007d9110: ldr      r0, [sb, #0xc]
007d9114: bl       #0x30e4cc
007d9118: ldr      ip, [sp, #0x14]
007d911c: add      r0, r0, #0xa
007d9120: smull    ip, r3, ip, r0
007d9124: asr      r0, r0, #0x1f
007d9128: rsb      r0, r0, r3, asr #3
007d912c: mul      r0, r5, r0
007d9130: bl       #0x30e964
007d9134: str      r0, [sb, #0xc]
007d9138: ldr      sb, [r4, #0x374]
007d913c: add      sb, sb, r7
007d9140: ldr      r0, [sb, #0x10]
007d9144: bl       #0x30e4cc
007d9148: ldr      r1, [sp, #0x14]
007d914c: add      r0, r0, #0xa
007d9150: smull    r1, r3, r1, r0
007d9154: asr      r0, r0, #0x1f
007d9158: rsb      r0, r0, r3, asr #3
007d915c: mul      r0, r5, r0
007d9160: bl       #0x30e964
007d9164: str      r0, [sb, #0x10]
007d9168: add      r7, r7, #0x18
007d916c: cmp      r7, #0x60
007d9170: bne      #0x7d90e4
007d9174: ldr      r3, [pc, #0x22c]
007d9178: ldr      r1, [r4, #0x378]
007d917c: mov      r2, #4
007d9180: add      r3, pc, r3
007d9184: ldr      ip, [r3, #0x18]
007d9188: ldr      r0, [r3, #0x1c]
007d918c: str      r2, [r1, #8]
007d9190: ldr      lr, [r3, #0x20]
007d9194: add      r5, sp, #0x50
007d9198: ldr      r1, [r4, #0x374]
007d919c: str      ip, [r5, #-0xc]!
007d91a0: add      ip, sp, #0x48
007d91a4: str      r0, [ip], #4
007d91a8: str      lr, [ip]
007d91ac: mov      r6, #6
007d91b0: mov      r0, r4
007d91b4: mov      r3, r5
007d91b8: str      r6, [sp]
007d91bc: str      r6, [sp, #4]
007d91c0: bl       #0x7d860c
007d91c4: cmp      r0, #0
007d91c8: beq      #0x7d9354
007d91cc: ldr      r6, [r4, #0xc]
007d91d0: cmp      r6, #0
007d91d4: beq      #0x7d934c
007d91d8: ldr      r2, [r6, #0x44]
007d91dc: str      r2, [sp, #0x14]
007d91e0: ldr      r7, [r4, #0x374]
007d91e4: adds     r8, r2, #6
007d91e8: ldr      fp, [r6, #0x24]
007d91ec: add      r4, r7, #0xc
007d91f0: beq      #0x7d9200
007d91f4: ldr      r3, [r6, #0x48]
007d91f8: cmp      r8, r3
007d91fc: bgt      #0x7d9378
007d9200: ldr      ip, [sp, #0x14]
007d9204: mov      r2, #0
007d9208: lsl      r3, ip, #1
007d920c: ldr      r0, [r6, #0x40]
007d9210: add      r1, r3, r2
007d9214: add      r2, r2, #2
007d9218: mov      ip, #0
007d921c: cmp      r2, #0xc
007d9220: strh     ip, [r0, r1]
007d9224: bne      #0x7d920c
007d9228: ldr      r0, [r6, #0x40]
007d922c: mov      r1, r5
007d9230: str      r8, [r6, #0x44]
007d9234: add      r0, r0, r3
007d9238: bl       #0x30e868
007d923c: ldr      r5, [r6, #0x24]
007d9240: adds     r5, r5, #4
007d9244: beq      #0x7d9254
007d9248: ldr      r3, [r6, #0x28]
007d924c: cmp      r5, r3
007d9250: bgt      #0x7d9398
007d9254: ldr      sl, [r6, #0x34]
007d9258: str      r5, [r6, #0x24]
007d925c: adds     sl, sl, #4
007d9260: beq      #0x7d9270
007d9264: ldr      r3, [r6, #0x38]
007d9268: cmp      sl, r3
007d926c: bgt      #0x7d9388
007d9270: ldr      r3, [r6, #0x20]
007d9274: ldr      sb, [r6, #0x30]
007d9278: mov      r8, #0xc
007d927c: mla      r8, r8, fp, r3
007d9280: mov      r5, #0
007d9284: str      sl, [r6, #0x34]
007d9288: add      sb, sb, fp, lsl #3
007d928c: mov      r1, r5
007d9290: mov      r2, r5
007d9294: ldr      r3, [r4, r2]
007d9298: add      r0, r4, r2
007d929c: add      r0, r0, #4
007d92a0: str      r3, [r8, r1]
007d92a4: ldr      ip, [r0], #4
007d92a8: add      r3, r8, r1
007d92ac: add      r3, r3, #4
007d92b0: str      ip, [r3], #4
007d92b4: ldr      sl, [r0]
007d92b8: mov      ip, r7
007d92bc: mov      r0, sb
007d92c0: str      sl, [r3]
007d92c4: ldr      r3, [ip, r2]!
007d92c8: add      r2, r2, #0x18
007d92cc: cmp      r2, #0x480
007d92d0: str      r3, [r0, r5]!
007d92d4: ldr      r3, [ip, #4]
007d92d8: add      r1, r1, #0xc
007d92dc: add      r5, r5, #8
007d92e0: str      r3, [r0, #4]
007d92e4: bne      #0x7d9294
007d92e8: ldr      r3, [r6, #0x14]
007d92ec: ldr      r2, [r6, #0x18]
007d92f0: add      r4, r3, #1
007d92f4: cmp      r4, r2
007d92f8: ble      #0x7d930c
007d92fc: add      r0, r6, #0x10
007d9300: add      r1, r4, r4, asr #1
007d9304: bl       #0x78a6f8
007d9308: ldr      r3, [r6, #0x14]
007d930c: mov      r2, #0x18
007d9310: ldr      r1, [r6, #0x10]
007d9314: mul      r2, r2, r3
007d9318: ldr      r0, [sp, #0x2c]
007d931c: add      r3, r1, r2
007d9320: str      r0, [r3, #4]
007d9324: ldr      ip, [sp, #0x1c]
007d9328: str      ip, [r1, r2]
007d932c: mov      r2, #6
007d9330: str      r2, [r3, #0x14]
007d9334: str      fp, [r3, #8]
007d9338: ldr      r0, [sp, #0x14]
007d933c: mov      r2, #4
007d9340: str      r2, [r3, #0xc]
007d9344: str      r0, [r3, #0x10]
007d9348: str      r4, [r6, #0x14]
007d934c: add      sp, sp, #0x54
007d9350: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007d9354: mov      r3, r6
007d9358: ldr      r0, [sp, #0x20]
007d935c: add      r1, r4, #0x378
007d9360: mov      r2, r5
007d9364: bl       #0x7d7094
007d9368: ldr      r6, [r4, #0xc]
007d936c: cmp      r6, #0
007d9370: bne      #0x7d91d8
007d9374: b        #0x7d934c
007d9378: add      r0, r6, #0x40
007d937c: add      r1, r8, r8, asr #1
007d9380: bl       #0x779e7c
007d9384: b        #0x7d9200
007d9388: add      r0, r6, #0x30
007d938c: add      r1, sl, sl, asr #1
007d9390: bl       #0x7d4208
007d9394: b        #0x7d9270
007d9398: add      r0, r6, #0x20
007d939c: add      r1, r5, r5, asr #1
007d93a0: bl       #0x7d4180
007d93a4: b        #0x7d9254
007d93a8: ldrheq   r2, [r3], -ip
