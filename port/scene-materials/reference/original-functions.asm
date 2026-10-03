
# _ZNK6glitch7collada16CColladaDatabase14getVisualSceneEi
0060e54c: ldr      r3, [r0]
0060e550: ldr      r3, [r3, #0x24]
0060e554: ldr      r3, [r3, #0x20]
0060e558: ldr      r2, [r3, #0x98]
0060e55c: cmp      r2, #0
0060e560: ldrgt    r0, [r3, #0x9c]
0060e564: movle    r0, #0
0060e568: addgt    r0, r0, r1, lsl #4
0060e56c: bx       lr

# _ZN6glitch7collada15CColladaFactory32createMaterialVertexAttributeMapERKNS0_16CColladaDatabaseEPNS0_17SInstanceMaterialERKN5boost13intrusive_ptrINS0_5IMeshEEERKNS8_INS_5video9CMaterialEEEjb
00634520: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00634524: mov      r4, r3
00634528: ldr      r3, [r3, #0x34]
0063452c: sub      sp, sp, #0x4c
00634530: str      r0, [sp, #0x18]
00634534: cmp      r3, #0
00634538: str      r3, [sp, #0x44]
0063453c: ldrne    r1, [r3]
00634540: mov      r6, r2
00634544: ldrb     r2, [sp, #0x7c]
00634548: addne    r1, r1, #1
0063454c: strne    r1, [r3]
00634550: ldrne    r3, [r4, #0x34]
00634554: cmp      r3, #0
00634558: beq      #0x634564
0063455c: cmp      r2, #0
00634560: beq      #0x6348ac
00634564: ldr      r3, [sp, #0x74]
00634568: ldr      r3, [r3]
0063456c: ldr      r3, [r3, #4]
00634570: cmp      r3, #0
00634574: str      r3, [sp, #0x40]
00634578: ldrne    r2, [r3]
0063457c: addne    r2, r2, #1
00634580: strne    r2, [r3]
00634584: ldrne    r3, [sp, #0x40]
00634588: ldr      r3, [r3, #4]
0063458c: mov      r0, r3
00634590: ldr      r3, [r3]
00634594: mov      lr, pc
00634598: ldr      pc, [r3, #0x5c]
0063459c: tst      r0, #7
006345a0: addne    r7, r4, #0x1c
006345a4: bne      #0x6345b4
006345a8: tst      r0, #0x18
006345ac: addne    r7, r4, #0x24
006345b0: beq      #0x634934
006345b4: add      r2, sp, #0x40
006345b8: add      r5, sp, #0x3c
006345bc: mov      r1, r2
006345c0: mov      r0, r5
006345c4: str      r2, [sp, #0x1c]
006345c8: bl       #0x5df33c
006345cc: ldr      r2, [sp, #0x3c]
006345d0: cmp      r2, #0
006345d4: str      r2, [sp, #0x28]
006345d8: beq      #0x6345ec
006345dc: ldr      r3, [r2]
006345e0: add      r3, r3, #1
006345e4: str      r3, [r2]
006345e8: ldr      r2, [sp, #0x28]
006345ec: ldr      r3, [sp, #0x44]
006345f0: add      r0, sp, #0x28
006345f4: str      r2, [sp, #0x44]
006345f8: str      r3, [sp, #0x28]
006345fc: bl       #0x57a26c
00634600: mov      r0, r5
00634604: bl       #0x57a26c
00634608: ldr      r3, [r4, #0x34]
0063460c: cmp      r3, #0
00634610: beq      #0x634960
00634614: ldr      r3, [sp, #0x70]
00634618: ldr      r2, [sp, #0x78]
0063461c: add      r0, sp, #0x34
00634620: ldr      r3, [r3]
00634624: mov      r1, r3
00634628: ldr      r3, [r3]
0063462c: mov      lr, pc
00634630: ldr      pc, [r3, #0x14]
00634634: ldr      r0, [sp, #0x34]
00634638: ldr      r3, [r0, #0x14]
0063463c: cmp      r3, #0
00634640: str      r3, [sp, #0x38]
00634644: ldrne    r2, [r3]
00634648: addne    r2, r2, #1
0063464c: strne    r2, [r3]
00634650: ldrne    r0, [sp, #0x34]
00634654: cmp      r0, #0
00634658: beq      #0x634660
0063465c: bl       #0x31d584
00634660: ldr      ip, [r7]
00634664: cmp      ip, #0
00634668: str      ip, [sp, #0x14]
0063466c: addle    r8, sp, #0x38
00634670: ble      #0x634780
00634674: mov      r6, #0
00634678: add      r2, sp, #0x30
0063467c: str      r6, [sp, #0x10]
00634680: add      r8, sp, #0x38
00634684: str      r2, [sp, #0xc]
00634688: ldr      r3, [r7, #4]
0063468c: ldr      r0, [sp, #0x40]
00634690: ldr      r1, [r3, r6]
00634694: bl       #0x5d4714
00634698: cmp      r0, #0xff
0063469c: mov      sl, r0
006346a0: beq      #0x634764
006346a4: ldr      r3, [r7, #4]
006346a8: add      r3, r3, r6
006346ac: ldr      sb, [r3, #4]
006346b0: cmp      sb, #0
006346b4: ble      #0x634764
006346b8: mov      r5, #0
006346bc: mov      r4, r5
006346c0: mov      r1, #0
006346c4: mov      r0, #0x24
006346c8: bl       #0x5341ac
006346cc: mov      r1, r8
006346d0: mov      fp, r0
006346d4: bl       #0x5a0958
006346d8: cmp      fp, #0
006346dc: str      fp, [sp, #0x30]
006346e0: ldrne    r3, [fp]
006346e4: moveq    r0, fp
006346e8: mov      ip, #0
006346ec: addne    r3, r3, #1
006346f0: strne    r3, [fp]
006346f4: ldr      r3, [r7, #4]
006346f8: ldrne    r0, [sp, #0x30]
006346fc: mov      r1, r8
00634700: add      r3, r3, r6
00634704: ldr      r2, [r3, #8]
00634708: add      r2, r2, r5
0063470c: ldmib    r2, {r2, r3}
00634710: str      ip, [sp]
00634714: bl       #0x5a0708
00634718: uxtb     r2, r4
0063471c: ldr      r0, [sp, #0x44]
00634720: ldr      r3, [sp, #0xc]
00634724: mov      r1, sl
00634728: bl       #0x5df814
0063472c: ldr      r3, [sp, #0x30]
00634730: add      r4, r4, #1
00634734: add      r5, r5, #0xc
00634738: cmp      r3, #0
0063473c: mov      r0, r3
00634740: beq      #0x63475c
00634744: ldr      r2, [r3]
00634748: sub      r2, r2, #1
0063474c: cmp      r2, #0
00634750: str      r2, [r3]
00634754: bne      #0x63475c
00634758: bl       #0x30e2b0
0063475c: cmp      r4, sb
00634760: bne      #0x6346c0
00634764: ldr      r2, [sp, #0x10]
00634768: ldr      r3, [sp, #0x14]
0063476c: add      r6, r6, #0xc
00634770: add      r2, r2, #1
00634774: cmp      r2, r3
00634778: str      r2, [sp, #0x10]
0063477c: bne      #0x634688
00634780: ldr      r3, [sp, #0x40]
00634784: mov      r6, #0
00634788: str      r6, [sp, #0x2c]
0063478c: ldrb     r2, [r3, #0x10]
00634790: cmp      r2, r6
00634794: beq      #0x63489c
00634798: movw     sl, #0x4ec5
0063479c: str      r8, [sp, #0xc]
006347a0: movt     sl, #0xc4ec
006347a4: mov      r1, r6
006347a8: mov      sb, r6
006347ac: add      fp, sp, #0x2c
006347b0: mov      r8, r2
006347b4: ldr      r3, [r3, #0x18]
006347b8: add      r3, r3, r6
006347bc: ldrb     r7, [r3, #4]
006347c0: cmp      r7, #0
006347c4: beq      #0x634854
006347c8: mov      r5, #0
006347cc: mov      r4, r5
006347d0: b        #0x6347ec
006347d4: add      r4, r4, #1
006347d8: uxtb     r4, r4
006347dc: cmp      r4, r7
006347e0: add      r5, r5, #0x34
006347e4: beq      #0x634854
006347e8: ldr      r1, [sp, #0x2c]
006347ec: ldr      r0, [sp, #0x44]
006347f0: ldr      r3, [r0, #4]
006347f4: ldr      r2, [r3, #0x18]
006347f8: ldr      r3, [r3, #0x1c]
006347fc: add      r2, r2, r6
00634800: ldr      r2, [r2, #8]
00634804: add      r2, r2, r5
00634808: rsb      r3, r3, r2
0063480c: asr      r3, r3, #2
00634810: mul      r3, sl, r3
00634814: add      r3, r0, r3, lsl #2
00634818: ldr      r3, [r3, #8]
0063481c: cmp      r3, #0
00634820: bne      #0x6347d4
00634824: cmp      r1, #0
00634828: beq      #0x6348dc
0063482c: mov      r2, r4
00634830: add      r4, r4, #1
00634834: mov      r1, sb
00634838: mov      r3, fp
0063483c: uxtb     r4, r4
00634840: bl       #0x5df814
00634844: cmp      r4, r7
00634848: ldr      r1, [sp, #0x2c]
0063484c: add      r5, r5, #0x34
00634850: bne      #0x6347e8
00634854: add      sb, sb, #1
00634858: uxtb     sb, sb
0063485c: cmp      sb, r8
00634860: add      r6, r6, #0xc
00634864: beq      #0x634874
00634868: ldr      r3, [sp, #0x40]
0063486c: ldr      r1, [sp, #0x2c]
00634870: b        #0x6347b4
00634874: cmp      r1, #0
00634878: ldr      r8, [sp, #0xc]
0063487c: beq      #0x63489c
00634880: ldr      r3, [r1]
00634884: sub      r3, r3, #1
00634888: cmp      r3, #0
0063488c: str      r3, [r1]
00634890: bne      #0x63489c
00634894: mov      r0, r1
00634898: bl       #0x30e2b0
0063489c: mov      r0, r8
006348a0: bl       #0x35eb90
006348a4: ldr      r0, [sp, #0x1c]
006348a8: bl       #0x3522b8
006348ac: ldr      r3, [sp, #0x44]
006348b0: ldr      ip, [sp, #0x18]
006348b4: cmp      r3, #0
006348b8: str      r3, [ip]
006348bc: ldrne    r2, [r3]
006348c0: addne    r2, r2, #1
006348c4: strne    r2, [r3]
006348c8: add      r0, sp, #0x44
006348cc: bl       #0x57a26c
006348d0: ldr      r0, [sp, #0x18]
006348d4: add      sp, sp, #0x4c
006348d8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006348dc: mov      r0, #0x24
006348e0: bl       #0x5341ac
006348e4: ldr      r1, [sp, #0xc]
006348e8: str      r0, [sp, #8]
006348ec: bl       #0x5a0958
006348f0: ldr      r3, [sp, #8]
006348f4: cmp      r3, #0
006348f8: ldrne    r2, [r3]
006348fc: addne    r2, r2, #1
00634900: strne    r2, [r3]
00634904: ldr      r0, [sp, #0x2c]
00634908: str      r3, [sp, #0x2c]
0063490c: cmp      r0, #0
00634910: beq      #0x63492c
00634914: ldr      r3, [r0]
00634918: sub      r3, r3, #1
0063491c: cmp      r3, #0
00634920: str      r3, [r0]
00634924: bne      #0x63492c
00634928: bl       #0x30e2b0
0063492c: ldr      r0, [sp, #0x44]
00634930: b        #0x63482c
00634934: tst      r0, #0x60
00634938: addne    r7, r4, #0x14
0063493c: bne      #0x6345b4
00634940: ands     r0, r0, #0x300
00634944: addne    r7, r4, #0x2c
00634948: bne      #0x6345b4
0063494c: ldr      r3, [sp, #0x18]
00634950: str      r0, [r3]
00634954: add      r0, sp, #0x40
00634958: bl       #0x3522b8
0063495c: b        #0x6348c8
00634960: ldr      r2, [sp, #0x44]
00634964: add      r0, sp, #0x48
00634968: str      r2, [sp, #0x24]
0063496c: cmp      r2, #0
00634970: ldrne    r3, [r2]
00634974: addne    r3, r3, #1
00634978: strne    r3, [r2]
0063497c: ldrne    r3, [r4, #0x34]
00634980: ldr      r2, [sp, #0x24]
00634984: str      r3, [r0, #-0x24]!
00634988: str      r2, [r4, #0x34]
0063498c: bl       #0x57a26c
00634990: mov      r0, r6
00634994: mov      r1, r4
00634998: bl       #0x60e28c
0063499c: b        #0x634614

# _ZN6glitch7collada15CColladaFactory14createMaterialERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverEPNS0_9SMaterialEPNS0_14CRootSceneNodeE
006323d0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
006323d4: sub      sp, sp, #0x20
006323d8: ldr      r4, [sp, #0x44]
006323dc: mov      r7, r1
006323e0: mov      r8, r2
006323e4: cmp      r4, #0
006323e8: mov      sl, r3
006323ec: mov      r5, r0
006323f0: ldr      r6, [sp, #0x40]
006323f4: beq      #0x63241c
006323f8: mov      r1, r4
006323fc: ldr      r2, [r6]
00632400: bl       #0x65b538
00632404: ldr      r3, [r5]
00632408: cmp      r3, #0
0063240c: beq      #0x632420
00632410: mov      r0, r5
00632414: add      sp, sp, #0x20
00632418: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0063241c: str      r4, [r0]
00632420: ldr      r2, [r6, #0xc]
00632424: ldr      r1, [r6, #0x18]
00632428: ldr      r3, [r6, #8]
0063242c: add      r2, r2, #1
00632430: stm      sp, {r1, r2, r3, r4}
00632434: add      sb, sp, #0x1c
00632438: mov      r3, sl
0063243c: mov      r1, r7
00632440: ldr      ip, [r7]
00632444: mov      r0, sb
00632448: mov      r2, r8
0063244c: mov      lr, pc
00632450: ldr      pc, [ip, #0x1c]
00632454: ldr      r3, [sp, #0x1c]
00632458: cmp      r3, #0
0063245c: beq      #0x6324b8
00632460: add      r7, sp, #0x18
00632464: mov      r2, sl
00632468: mov      r1, r8
0063246c: mov      r0, r7
00632470: mov      r3, sb
00632474: str      r6, [sp]
00632478: str      r4, [sp, #4]
0063247c: bl       #0x631ce8
00632480: ldr      r3, [sp, #0x18]
00632484: add      r0, sp, #0x20
00632488: str      r3, [sp, #0x14]
0063248c: cmp      r3, #0
00632490: ldrne    r2, [r3]
00632494: addne    r2, r2, #1
00632498: strne    r2, [r3]
0063249c: ldrne    r3, [sp, #0x14]
006324a0: ldr      r2, [r5]
006324a4: str      r3, [r5]
006324a8: str      r2, [r0, #-0xc]!
006324ac: bl       #0x310be8
006324b0: mov      r0, r7
006324b4: bl       #0x310be8
006324b8: mov      r0, sb
006324bc: bl       #0x3522b8
006324c0: b        #0x632410

# _ZNK6glitch5scene10ISceneNode25getRelativeTransformationEv
00598908: push     {r4, r5, r6, lr}
0059890c: ldr      r3, [r0, #0x11c]
00598910: sub      sp, sp, #0x48
00598914: mov      r4, r0
00598918: tst      r3, #0xe
0059891c: addeq    r5, r0, #0x68
00598920: beq      #0x598958
00598924: ands     r2, r3, #6
00598928: bne      #0x598964
0059892c: ldr      ip, [r0, #0xac]
00598930: ldr      r1, [r4, #0xb4]
00598934: ldr      r0, [r0, #0xb0]
00598938: add      r5, r4, #0x68
0059893c: strb     r2, [r4, #0xa8]
00598940: str      ip, [r4, #0x98]
00598944: str      r0, [r4, #0x9c]
00598948: str      r1, [r4, #0xa0]
0059894c: bic      r3, r3, #0xe
00598950: orr      r3, r3, #0x10
00598954: str      r3, [r4, #0x11c]
00598958: mov      r0, r5
0059895c: add      sp, sp, #0x48
00598960: pop      {r4, r5, r6, pc}
00598964: add      r6, sp, #4
00598968: mov      r3, #0
0059896c: add      r5, r0, #0x68
00598970: mov      r1, r6
00598974: add      r0, r0, #0xb8
00598978: strb     r3, [sp, #0x44]
0059897c: bl       #0x5602d0
00598980: mov      r1, r6
00598984: mov      r2, #0x41
00598988: mov      r0, r5
0059898c: bl       #0x30e868
00598990: ldr      r0, [r4, #0xc8]
00598994: mov      r1, #0x3f800000
00598998: bl       #0x30df8c
0059899c: cmp      r0, #0
005989a0: beq      #0x5989b8
005989a4: ldr      r0, [r4, #0xcc]
005989a8: mov      r1, #0x3f800000
005989ac: bl       #0x30df8c
005989b0: cmp      r0, #0
005989b4: bne      #0x5989ec
005989b8: mov      r0, r5
005989bc: add      r1, r4, #0xc8
005989c0: bl       #0x597788
005989c4: ldr      r3, [r4, #0xb4]
005989c8: ldr      r1, [r4, #0xac]
005989cc: ldr      r2, [r4, #0xb0]
005989d0: mov      r0, #0
005989d4: str      r3, [r4, #0xa0]
005989d8: strb     r0, [r4, #0xa8]
005989dc: str      r1, [r4, #0x98]
005989e0: str      r2, [r4, #0x9c]
005989e4: ldr      r3, [r4, #0x11c]
005989e8: b        #0x59894c
005989ec: ldr      r0, [r4, #0xd0]
005989f0: mov      r1, #0x3f800000
005989f4: bl       #0x30df8c
005989f8: cmp      r0, #0
005989fc: bne      #0x5989c4
00598a00: b        #0x5989b8

# _ZN6glitch7collada6CImageC1ERKNS0_16CColladaDatabaseERNS0_6SImageE
0060e1d4: push     {r4, lr}
0060e1d8: ldr      r3, [r1]
0060e1dc: mov      r4, r0
0060e1e0: ldr      r0, [pc, #0x98]
0060e1e4: str      r3, [r4, #0xc]
0060e1e8: ldr      r1, [r1, #4]
0060e1ec: cmp      r3, #0
0060e1f0: add      r0, pc, r0
0060e1f4: str      r1, [r4, #0x10]
0060e1f8: beq      #0x60e20c
0060e1fc: ldr      r1, [r3, #4]
0060e200: cmp      r1, #0
0060e204: addne    r1, r1, #1
0060e208: strne    r1, [r3, #4]
0060e20c: ldr      r1, [pc, #0x70]
0060e210: ldr      r3, [pc, #0x70]
0060e214: str      r2, [r4, #0x18]
0060e218: ldr      r1, [r0, r1]
0060e21c: ldr      r3, [r0, r3]
0060e220: add      r1, r1, #4
0060e224: add      r3, r3, #8
0060e228: str      r1, [r4, #8]
0060e22c: str      r3, [r4]
0060e230: mov      r1, #1
0060e234: mov      r3, #0
0060e238: str      r3, [r4, #0x14]
0060e23c: str      r1, [r4, #4]
0060e240: ldr      r3, [r2]
0060e244: str      r3, [r4, #8]
0060e248: ldr      r3, [r2, #0x10]
0060e24c: cmp      r3, #0
0060e250: streq    r3, [r4, #0x14]
0060e254: beq      #0x60e278
0060e258: ldr      r2, [r3, #4]
0060e25c: add      r2, r2, r1
0060e260: str      r2, [r3, #4]
0060e264: ldr      r0, [r4, #0x14]
0060e268: str      r3, [r4, #0x14]
0060e26c: cmp      r0, #0
0060e270: beq      #0x60e278
0060e274: bl       #0x31d584
0060e278: mov      r0, r4
0060e27c: pop      {r4, pc}
0060e280: eorseq   r6, r8, r0, lsr #17
0060e284: strheq   r1, [r0], -r4
0060e288: andeq    r3, r0, r8, lsl #27

# _ZN6glitch5scene10ISceneNode22updateAbsolutePositionEb
00597c60: push     {r4, r5, r6, lr}
00597c64: ldr      r3, [r0, #0xec]
00597c68: mov      r4, r0
00597c6c: mov      r5, r1
00597c70: cmp      r3, #0
00597c74: beq      #0x597d18
00597c78: ldr      r2, [r3, #0x11c]
00597c7c: tst      r2, #0x20
00597c80: bne      #0x597cd0
00597c84: ldr      r2, [r0, #0x11c]
00597c88: tst      r2, #0x5e
00597c8c: bne      #0x597cd0
00597c90: cmp      r5, #0
00597c94: ldrne    r5, [r4, #0xf4]!
00597c98: bne      #0x597cc4
00597c9c: b        #0x597ccc
00597ca0: cmp      r5, #0
00597ca4: moveq    r3, r5
00597ca8: subne    r3, r5, #4
00597cac: mov      r0, r3
00597cb0: mov      r1, #1
00597cb4: ldr      r3, [r3]
00597cb8: mov      lr, pc
00597cbc: ldr      pc, [r3, #0xb8]
00597cc0: ldr      r5, [r5]
00597cc4: cmp      r4, r5
00597cc8: bne      #0x597ca0
00597ccc: pop      {r4, r5, r6, pc}
00597cd0: mov      r0, r3
00597cd4: ldr      r3, [r3]
00597cd8: mov      lr, pc
00597cdc: ldr      pc, [r3, #0x38]
00597ce0: ldr      r3, [r4]
00597ce4: mov      r6, r0
00597ce8: mov      r0, r4
00597cec: mov      lr, pc
00597cf0: ldr      pc, [r3, #0x40]
00597cf4: add      r2, r4, #0x24
00597cf8: mov      r1, r0
00597cfc: mov      r0, r6
00597d00: bl       #0x597884
00597d04: ldr      r3, [r4, #0x11c]
00597d08: orr      r3, r3, #0x120
00597d0c: bic      r3, r3, #0x50
00597d10: str      r3, [r4, #0x11c]
00597d14: b        #0x597c90
00597d18: ldr      r3, [r0, #0x11c]
00597d1c: tst      r3, #0x5e
00597d20: beq      #0x597c90
00597d24: mov      r6, r0
00597d28: ldr      r3, [r6], #0x24
00597d2c: mov      lr, pc
00597d30: ldr      pc, [r3, #0x40]
00597d34: mov      r2, #0x41
00597d38: mov      r1, r0
00597d3c: mov      r0, r6
00597d40: bl       #0x30e868
00597d44: ldr      r3, [r4, #0x11c]
00597d48: orr      r3, r3, #0x120
00597d4c: bic      r3, r3, #0x50
00597d50: str      r3, [r4, #0x11c]
00597d54: b        #0x597c90

# _ZNK6glitch7collada16CColladaDatabase7getNodeEPKc
0061c290: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061c294: mov      r8, r1
0061c298: mov      r1, #0
0061c29c: mov      r7, r0
0061c2a0: bl       #0x60e54c
0061c2a4: subs     r6, r0, #0
0061c2a8: bne      #0x61c2b4
0061c2ac: mov      r0, #0
0061c2b0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061c2b4: ldr      sl, [r6, #8]
0061c2b8: cmp      sl, #0
0061c2bc: ble      #0x61c2ac
0061c2c0: mov      r4, #0
0061c2c4: mov      r5, r4
0061c2c8: ldr      r2, [r6, #0xc]
0061c2cc: mov      r0, r7
0061c2d0: mov      r1, r8
0061c2d4: add      r2, r2, r4
0061c2d8: bl       #0x61c214
0061c2dc: cmp      r0, #0
0061c2e0: add      r5, r5, #1
0061c2e4: bne      #0x61c2b0
0061c2e8: cmp      r5, sl
0061c2ec: add      r4, r4, #0x50
0061c2f0: bne      #0x61c2c8
0061c2f4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK6glitch7collada16CColladaDatabase14constructSceneEPNS_5video12IVideoDriverE
0061b9e8: push     {r4, r5, r6, r7, r8, lr}
0061b9ec: ldr      r6, [r0]
0061b9f0: mov      r5, r0
0061b9f4: mov      r7, r1
0061b9f8: cmp      r6, #0
0061b9fc: beq      #0x61bab4
0061ba00: ldr      r3, [r0, #4]
0061ba04: mov      r1, r0
0061ba08: mov      r0, r3
0061ba0c: ldr      r3, [r3]
0061ba10: mov      lr, pc
0061ba14: ldr      pc, [r3, #0x54]
0061ba18: ldr      r1, [r5]
0061ba1c: mov      r6, r0
0061ba20: ldr      r3, [r1, #0x24]
0061ba24: ldr      r3, [r3, #0x20]
0061ba28: ldr      r2, [r3, #0xb8]
0061ba2c: cmp      r2, #0
0061ba30: ble      #0x61ba9c
0061ba34: mov      r4, #0
0061ba38: b        #0x61ba50
0061ba3c: ldr      r3, [r1, #0x24]
0061ba40: ldr      r3, [r3, #0x20]
0061ba44: ldr      r2, [r3, #0xb8]
0061ba48: cmp      r4, r2
0061ba4c: bge      #0x61ba9c
0061ba50: ldr      r3, [r3, #0xbc]
0061ba54: ldr      r2, [r3, r4, lsl #3]
0061ba58: add      r3, r3, r4, lsl #3
0061ba5c: add      r4, r4, #1
0061ba60: cmp      r2, #6
0061ba64: bne      #0x61ba3c
0061ba68: ldr      r3, [r3, #4]
0061ba6c: mov      r1, r7
0061ba70: mov      r0, r5
0061ba74: ldr      r2, [r3, #4]
0061ba78: mov      r3, r6
0061ba7c: add      r2, r2, #1
0061ba80: bl       #0x61b9b8
0061ba84: ldr      r1, [r5]
0061ba88: ldr      r3, [r1, #0x24]
0061ba8c: ldr      r3, [r3, #0x20]
0061ba90: ldr      r2, [r3, #0xb8]
0061ba94: cmp      r4, r2
0061ba98: blt      #0x61ba50
0061ba9c: mov      r0, r6
0061baa0: bl       #0x65b3dc
0061baa4: mov      r0, r6
0061baa8: bl       #0x65c874
0061baac: mov      r0, r6
0061bab0: pop      {r4, r5, r6, r7, r8, pc}
0061bab4: mov      r0, r6
0061bab8: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK6glitch7collada16CColladaDatabase13constructNodeEPNS_5video12IVideoDriverEPNS0_5SNodeEPNS0_14CRootSceneNodeE
0061b2f4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061b2f8: subs     r4, r2, #0
0061b2fc: sub      sp, sp, #0x44
0061b300: mov      r8, r0
0061b304: mov      fp, r1
0061b308: mov      sl, r3
0061b30c: moveq    r6, r4
0061b310: beq      #0x61b55c
0061b314: ldr      r3, [r4, #0x4c]
0061b318: cmp      r3, #0
0061b31c: beq      #0x61b874
0061b320: ldr      r3, [r0, #4]
0061b324: mov      r1, r0
0061b328: mov      r0, r3
0061b32c: ldr      r3, [r3]
0061b330: mov      lr, pc
0061b334: ldr      pc, [r3, #0x40]
0061b338: mov      r6, r0
0061b33c: ldr      r1, [r4, #0x40]
0061b340: cmp      r1, #0
0061b344: ble      #0x61b434
0061b348: add      r3, sp, #0x38
0061b34c: add      ip, sp, #0x3c
0061b350: mov      r5, #0
0061b354: str      r3, [sp, #8]
0061b358: str      ip, [sp, #0xc]
0061b35c: mov      r7, r6
0061b360: ldr      r2, [r4, #0x44]
0061b364: lsl      r6, r5, #3
0061b368: ldr      r3, [r2, r5, lsl #3]
0061b36c: add      r2, r2, r6
0061b370: sub      r3, r3, #1
0061b374: cmp      r3, #0xc
0061b378: addls    pc, pc, r3, lsl #2
0061b37c: b        #0x61b424
0061b380: b        #0x61b84c
0061b384: b        #0x61b720
0061b388: b        #0x61b68c
0061b38c: b        #0x61b664
0061b390: b        #0x61b424
0061b394: b        #0x61b424
0061b398: b        #0x61b424
0061b39c: b        #0x61b424
0061b3a0: b        #0x61b804
0061b3a4: b        #0x61b3b4
0061b3a8: b        #0x61b828
0061b3ac: b        #0x61b614
0061b3b0: b        #0x61b568
0061b3b4: ldr      r1, [r2, #4]
0061b3b8: mov      r0, r8
0061b3bc: mov      r2, fp
0061b3c0: mov      r3, sl
0061b3c4: bl       #0x61a608
0061b3c8: subs     sb, r0, #0
0061b3cc: beq      #0x61b600
0061b3d0: ldr      r2, [r4, #0x44]
0061b3d4: ldr      r3, [sb]
0061b3d8: add      r6, r2, r6
0061b3dc: ldr      r2, [r6, #4]
0061b3e0: ldr      r1, [r2, #0x14]
0061b3e4: mov      lr, pc
0061b3e8: ldr      pc, [r3, #0xd4]
0061b3ec: mov      r0, sb
0061b3f0: ldr      r3, [sb]
0061b3f4: mov      lr, pc
0061b3f8: ldr      pc, [r3, #0x104]
0061b3fc: mov      r0, r7
0061b400: ldr      r3, [r7]
0061b404: mov      r1, sb
0061b408: mov      lr, pc
0061b40c: ldr      pc, [r3, #0x5c]
0061b410: ldr      r3, [sb]
0061b414: ldr      r0, [r3, #-0xc]
0061b418: add      r0, sb, r0
0061b41c: bl       #0x31d584
0061b420: ldr      r1, [r4, #0x40]
0061b424: add      r5, r5, #1
0061b428: cmp      r5, r1
0061b42c: blt      #0x61b360
0061b430: mov      r6, r7
0061b434: mov      r0, r6
0061b438: ldr      r1, [r4, #4]
0061b43c: ldr      r3, [r6]
0061b440: mov      lr, pc
0061b444: ldr      pc, [r3, #0x28]
0061b448: ldr      r3, [r6]
0061b44c: ldr      r2, [r4, #0xc]
0061b450: mov      r0, r6
0061b454: ldr      r3, [r3, #0xa4]
0061b458: str      r2, [sp, #0x2c]
0061b45c: ldr      r2, [r4, #0x10]
0061b460: add      r1, sp, #0x2c
0061b464: str      r2, [sp, #0x30]
0061b468: ldr      r2, [r4, #0x14]
0061b46c: str      r2, [sp, #0x34]
0061b470: blx      r3
0061b474: ldr      r3, [r6]
0061b478: ldr      r2, [r4, #0x18]
0061b47c: mov      r0, r6
0061b480: ldr      r3, [r3, #0x9c]
0061b484: str      r2, [sp, #0x10]
0061b488: ldr      r2, [r4, #0x1c]
0061b48c: add      r1, sp, #0x10
0061b490: str      r2, [sp, #0x14]
0061b494: ldr      r2, [r4, #0x20]
0061b498: str      r2, [sp, #0x18]
0061b49c: ldr      r2, [r4, #0x24]
0061b4a0: str      r2, [sp, #0x1c]
0061b4a4: blx      r3
0061b4a8: ldr      r3, [r6]
0061b4ac: ldr      r2, [r4, #0x28]
0061b4b0: mov      r0, r6
0061b4b4: ldr      r3, [r3, #0x94]
0061b4b8: str      r2, [sp, #0x20]
0061b4bc: ldr      r2, [r4, #0x2c]
0061b4c0: add      r1, sp, #0x20
0061b4c4: str      r2, [sp, #0x24]
0061b4c8: ldr      r2, [r4, #0x30]
0061b4cc: str      r2, [sp, #0x28]
0061b4d0: blx      r3
0061b4d4: ldr      r1, [r4, #0x34]
0061b4d8: ldr      r3, [r6]
0061b4dc: mov      r0, r6
0061b4e0: subs     r1, r1, #0
0061b4e4: movne    r1, #1
0061b4e8: mov      lr, pc
0061b4ec: ldr      pc, [r3, #0x48]
0061b4f0: ldr      r3, [r4, #0x38]
0061b4f4: cmp      r3, #0
0061b4f8: ble      #0x61b55c
0061b4fc: mov      r5, #0
0061b500: mov      r7, r5
0061b504: mov      sb, r8
0061b508: ldr      r2, [r4, #0x3c]
0061b50c: mov      r3, sl
0061b510: mov      r1, fp
0061b514: add      r2, r2, r5
0061b518: mov      r0, sb
0061b51c: bl       #0x61b2f4
0061b520: ldr      r3, [r6]
0061b524: mov      r8, r0
0061b528: mov      r1, r0
0061b52c: mov      r0, r6
0061b530: mov      lr, pc
0061b534: ldr      pc, [r3, #0x5c]
0061b538: ldr      r3, [r8]
0061b53c: add      r7, r7, #1
0061b540: add      r5, r5, #0x50
0061b544: ldr      r0, [r3, #-0xc]
0061b548: add      r0, r8, r0
0061b54c: bl       #0x31d584
0061b550: ldr      r3, [r4, #0x38]
0061b554: cmp      r7, r3
0061b558: blt      #0x61b508
0061b55c: mov      r0, r6
0061b560: add      sp, sp, #0x44
0061b564: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0061b568: ldr      r2, [r2, #4]
0061b56c: ldr      r0, [sp, #8]
0061b570: mov      r1, r8
0061b574: mov      r3, sl
0061b578: bl       #0x60e6f0
0061b57c: ldr      r3, [r8, #4]
0061b580: mov      r1, r8
0061b584: ldr      r2, [sp, #8]
0061b588: mov      r0, r3
0061b58c: ldr      ip, [r3]
0061b590: ldr      r3, [r4, #0x48]
0061b594: mov      lr, pc
0061b598: ldr      pc, [ip, #0x50]
0061b59c: subs     sb, r0, #0
0061b5a0: beq      #0x61b5f0
0061b5a4: ldr      r2, [r4, #0x44]
0061b5a8: ldr      r3, [sb]
0061b5ac: add      r6, r2, r6
0061b5b0: ldr      r2, [r6, #4]
0061b5b4: ldr      r1, [r2, #0x14]
0061b5b8: mov      lr, pc
0061b5bc: ldr      pc, [r3, #0xd4]
0061b5c0: mov      r0, sb
0061b5c4: mov      r1, #2
0061b5c8: bl       #0x59719c
0061b5cc: mov      r0, r7
0061b5d0: ldr      r3, [r7]
0061b5d4: mov      r1, sb
0061b5d8: mov      lr, pc
0061b5dc: ldr      pc, [r3, #0x5c]
0061b5e0: ldr      r3, [sb]
0061b5e4: ldr      r0, [r3, #-0xc]
0061b5e8: add      r0, sb, r0
0061b5ec: bl       #0x31d584
0061b5f0: ldr      r0, [sp, #0x38]
0061b5f4: cmp      r0, #0
0061b5f8: beq      #0x61b600
0061b5fc: bl       #0x31d584
0061b600: ldr      r1, [r4, #0x40]
0061b604: add      r5, r5, #1
0061b608: cmp      r5, r1
0061b60c: blt      #0x61b360
0061b610: b        #0x61b430
0061b614: ldr      r1, [r2, #4]
0061b618: mov      r0, r8
0061b61c: mov      r2, sl
0061b620: bl       #0x61a994
0061b624: subs     r6, r0, #0
0061b628: beq      #0x61b600
0061b62c: mov      r1, r6
0061b630: mov      r0, r7
0061b634: ldr      r3, [r7]
0061b638: mov      lr, pc
0061b63c: ldr      pc, [r3, #0x5c]
0061b640: ldr      r3, [r6]
0061b644: add      r5, r5, #1
0061b648: ldr      r0, [r3, #-0xc]
0061b64c: add      r0, r6, r0
0061b650: bl       #0x31d584
0061b654: ldr      r1, [r4, #0x40]
0061b658: cmp      r5, r1
0061b65c: blt      #0x61b360
0061b660: b        #0x61b430
0061b664: ldr      r3, [r2, #4]
0061b668: mov      r0, r8
0061b66c: mov      r2, sl
0061b670: ldr      r1, [r3, #4]
0061b674: add      r1, r1, #1
0061b678: bl       #0x61b24c
0061b67c: subs     r6, r0, #0
0061b680: bne      #0x61b62c
0061b684: ldr      r1, [r4, #0x40]
0061b688: b        #0x61b604
0061b68c: ldr      r3, [r2, #4]
0061b690: ldr      r0, [sp, #0xc]
0061b694: mov      r1, r8
0061b698: mov      r2, fp
0061b69c: str      sl, [sp]
0061b6a0: bl       #0x61aeb8
0061b6a4: ldr      r0, [sp, #0x3c]
0061b6a8: cmp      r0, #0
0061b6ac: str      r0, [sp, #0x38]
0061b6b0: ldrne    r3, [r0, #4]
0061b6b4: addne    r3, r3, #1
0061b6b8: strne    r3, [r0, #4]
0061b6bc: ldrne    r0, [sp, #0x3c]
0061b6c0: cmp      r0, #0
0061b6c4: beq      #0x61b6cc
0061b6c8: bl       #0x31d584
0061b6cc: ldr      r3, [sp, #0x38]
0061b6d0: cmp      r3, #0
0061b6d4: beq      #0x61b600
0061b6d8: ldr      r3, [r8, #4]
0061b6dc: mov      r1, r8
0061b6e0: ldr      r2, [sp, #8]
0061b6e4: mov      r0, r3
0061b6e8: ldr      ip, [r3]
0061b6ec: ldr      r3, [r4, #0x48]
0061b6f0: mov      lr, pc
0061b6f4: ldr      pc, [ip, #0x48]
0061b6f8: subs     sb, r0, #0
0061b6fc: beq      #0x61b7f0
0061b700: ldr      r2, [r4, #0x44]
0061b704: ldr      r3, [sb]
0061b708: add      r6, r2, r6
0061b70c: ldr      r2, [r6, #4]
0061b710: ldr      r1, [r2, #0x14]
0061b714: mov      lr, pc
0061b718: ldr      pc, [r3, #0xd4]
0061b71c: b        #0x61b7cc
0061b720: ldr      r3, [r2, #4]
0061b724: mov      ip, #1
0061b728: ldr      r0, [sp, #8]
0061b72c: mov      r1, r8
0061b730: mov      r2, fp
0061b734: stm      sp, {sl, ip}
0061b738: bl       #0x61ace8
0061b73c: ldr      r3, [sp, #0x38]
0061b740: mov      r0, r3
0061b744: ldr      r3, [r3]
0061b748: mov      lr, pc
0061b74c: ldr      pc, [r3, #0x30]
0061b750: cmp      r0, #2
0061b754: beq      #0x61b894
0061b758: ldr      r3, [sp, #0x38]
0061b75c: mov      r0, r3
0061b760: ldr      r3, [r3]
0061b764: mov      lr, pc
0061b768: ldr      pc, [r3, #0x30]
0061b76c: cmp      r0, #3
0061b770: beq      #0x61b894
0061b774: ldr      r3, [r8, #4]
0061b778: mov      r1, r8
0061b77c: ldr      r2, [sp, #8]
0061b780: mov      r0, r3
0061b784: ldr      ip, [r3]
0061b788: ldr      r3, [r4, #0x48]
0061b78c: mov      lr, pc
0061b790: ldr      pc, [ip, #0x48]
0061b794: mov      sb, r0
0061b798: cmp      sb, #0
0061b79c: beq      #0x61b7f0
0061b7a0: ldr      r2, [r4, #0x44]
0061b7a4: mov      r0, sb
0061b7a8: ldr      r3, [sb]
0061b7ac: add      r6, r2, r6
0061b7b0: ldr      r2, [r6, #4]
0061b7b4: ldr      r1, [r2, #0x14]
0061b7b8: mov      lr, pc
0061b7bc: ldr      pc, [r3, #0xd4]
0061b7c0: mov      r0, sb
0061b7c4: mov      r1, #2
0061b7c8: bl       #0x59719c
0061b7cc: mov      r0, r7
0061b7d0: ldr      r3, [r7]
0061b7d4: mov      r1, sb
0061b7d8: mov      lr, pc
0061b7dc: ldr      pc, [r3, #0x5c]
0061b7e0: ldr      r3, [sb]
0061b7e4: ldr      r0, [r3, #-0xc]
0061b7e8: add      r0, sb, r0
0061b7ec: bl       #0x31d584
0061b7f0: ldr      r0, [sp, #0x38]
0061b7f4: cmp      r0, #0
0061b7f8: bne      #0x61b41c
0061b7fc: ldr      r1, [r4, #0x40]
0061b800: b        #0x61b604
0061b804: ldr      r1, [r2, #4]
0061b808: mov      r0, r8
0061b80c: mov      r2, fp
0061b810: mov      r3, sl
0061b814: bl       #0x61a888
0061b818: subs     sb, r0, #0
0061b81c: bne      #0x61b3d0
0061b820: ldr      r1, [r4, #0x40]
0061b824: b        #0x61b604
0061b828: ldr      r1, [r2, #4]
0061b82c: mov      r0, r8
0061b830: mov      r2, fp
0061b834: mov      r3, sl
0061b838: bl       #0x61a77c
0061b83c: subs     r6, r0, #0
0061b840: bne      #0x61b62c
0061b844: ldr      r1, [r4, #0x40]
0061b848: b        #0x61b604
0061b84c: ldr      r3, [r2, #4]
0061b850: mov      r0, r8
0061b854: mov      r2, sl
0061b858: ldr      r1, [r3, #4]
0061b85c: add      r1, r1, #1
0061b860: bl       #0x61b2d0
0061b864: subs     r6, r0, #0
0061b868: bne      #0x61b62c
0061b86c: ldr      r1, [r4, #0x40]
0061b870: b        #0x61b604
0061b874: ldr      r3, [r0, #4]
0061b878: mov      r1, r0
0061b87c: mov      r0, r3
0061b880: ldr      r3, [r3]
0061b884: mov      lr, pc
0061b888: ldr      pc, [r3, #0x3c]
0061b88c: mov      r6, r0
0061b890: b        #0x61b33c
0061b894: ldr      r3, [r8, #4]
0061b898: mov      r1, r8
0061b89c: ldr      r2, [sp, #8]
0061b8a0: mov      r0, r3
0061b8a4: ldr      ip, [r3]
0061b8a8: ldr      r3, [r4, #0x48]
0061b8ac: mov      lr, pc
0061b8b0: ldr      pc, [ip, #0x4c]
0061b8b4: mov      sb, r0
0061b8b8: b        #0x61b798

# _ZNK6glitch7collada16CColladaDatabase17constructGeometryEPNS_5video12IVideoDriverEPNS0_17SInstanceGeometryEPNS0_14CRootSceneNodeE
0061aeb8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061aebc: mov      sb, r0
0061aec0: mov      r0, #0
0061aec4: str      r0, [sb]
0061aec8: mov      r6, r3
0061aecc: ldr      r3, [r3]
0061aed0: ldr      ip, [r6, #4]
0061aed4: sub      sp, sp, #0x34
0061aed8: cmp      r3, r0
0061aedc: mov      r5, r1
0061aee0: add      ip, ip, #1
0061aee4: str      r2, [sp, #0x10]
0061aee8: beq      #0x61b068
0061aeec: add      r0, sp, #0x2c
0061aef0: str      ip, [sp]
0061aef4: bl       #0x61aae0
0061aef8: ldr      r3, [sp, #0x2c]
0061aefc: cmp      r3, #0
0061af00: ldrne    r2, [r3, #4]
0061af04: addne    r2, r2, #1
0061af08: strne    r2, [r3, #4]
0061af0c: ldr      r0, [sb]
0061af10: str      r3, [sb]
0061af14: cmp      r0, #0
0061af18: beq      #0x61af20
0061af1c: bl       #0x31d584
0061af20: ldr      r0, [sp, #0x2c]
0061af24: cmp      r0, #0
0061af28: beq      #0x61af30
0061af2c: bl       #0x31d584
0061af30: ldr      r3, [sb]
0061af34: cmp      r3, #0
0061af38: beq      #0x61b05c
0061af3c: ldr      r3, [r6, #0xc]
0061af40: cmp      r3, #0
0061af44: ble      #0x61b05c
0061af48: mov      r7, #0
0061af4c: add      r0, sp, #0x1c
0061af50: mov      r4, r7
0061af54: mov      r8, r7
0061af58: add      sl, sp, #0x24
0061af5c: add      fp, sp, #0x20
0061af60: str      r0, [sp, #0x14]
0061af64: mov      r7, r6
0061af68: b        #0x61b030
0061af6c: ldr      r2, [r6, #4]
0061af70: add      r2, r2, #1
0061af74: bl       #0x61ac88
0061af78: mov      r2, r0
0061af7c: mov      r0, sl
0061af80: ldr      r1, [sp, #0x58]
0061af84: ldr      r3, [sp, #0x10]
0061af88: bl       #0x65cafc
0061af8c: ldr      lr, [r5, #4]
0061af90: ldr      ip, [sb]
0061af94: mov      r3, r6
0061af98: mov      r1, lr
0061af9c: ldr      lr, [lr]
0061afa0: cmp      ip, #0
0061afa4: mov      r0, fp
0061afa8: ldr      r6, [lr, #0x24]
0061afac: str      ip, [sp, #0x1c]
0061afb0: ldrne    lr, [ip, #4]
0061afb4: mov      r2, r5
0061afb8: add      r8, r8, #0x3c
0061afbc: addne    lr, lr, #1
0061afc0: strne    lr, [ip, #4]
0061afc4: ldr      ip, [sp, #0x14]
0061afc8: str      r4, [sp, #8]
0061afcc: str      sl, [sp, #4]
0061afd0: str      ip, [sp]
0061afd4: mov      ip, #0
0061afd8: str      ip, [sp, #0xc]
0061afdc: blx      r6
0061afe0: ldr      r0, [sp, #0x1c]
0061afe4: cmp      r0, #0
0061afe8: beq      #0x61aff0
0061afec: bl       #0x31d584
0061aff0: ldr      ip, [sb]
0061aff4: mov      r1, r4
0061aff8: mov      r3, fp
0061affc: mov      r2, sl
0061b000: mov      r0, ip
0061b004: ldr      ip, [ip]
0061b008: mov      lr, pc
0061b00c: ldr      pc, [ip, #0x20]
0061b010: mov      r0, fp
0061b014: bl       #0x57a26c
0061b018: mov      r0, sl
0061b01c: bl       #0x310be8
0061b020: ldr      r3, [r7, #0xc]
0061b024: add      r4, r4, #1
0061b028: cmp      r4, r3
0061b02c: bge      #0x61b05c
0061b030: ldr      r6, [r7, #0x10]
0061b034: mov      r0, r5
0061b038: ldr      r1, [r6, r8]
0061b03c: add      r6, r6, r8
0061b040: cmp      r1, #0
0061b044: bne      #0x61af6c
0061b048: ldr      r1, [r6, #8]
0061b04c: mov      r0, r5
0061b050: bl       #0x60e400
0061b054: mov      r2, r0
0061b058: b        #0x61af7c
0061b05c: mov      r0, sb
0061b060: add      sp, sp, #0x34
0061b064: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0061b068: mov      r3, ip
0061b06c: add      r0, sp, #0x28
0061b070: bl       #0x61aaa8
0061b074: ldr      r3, [sp, #0x28]
0061b078: cmp      r3, #0
0061b07c: ldrne    r2, [r3, #4]
0061b080: addne    r2, r2, #1
0061b084: strne    r2, [r3, #4]
0061b088: ldr      r0, [sb]
0061b08c: str      r3, [sb]
0061b090: cmp      r0, #0
0061b094: beq      #0x61b09c
0061b098: bl       #0x31d584
0061b09c: ldr      r0, [sp, #0x28]
0061b0a0: cmp      r0, #0
0061b0a4: bne      #0x61af2c
0061b0a8: b        #0x61af30

# _ZN6glitch7collada14createMaterialERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS4_17CMaterialRendererEEERNS0_9SMaterialEPNS0_14CRootSceneNodeE
00631ce8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00631cec: mov      r2, #0
00631cf0: sub      sp, sp, #0xa4
00631cf4: str      r0, [sp, #0x1c]
00631cf8: str      r3, [sp, #0x18]
00631cfc: str      r2, [r0]
00631d00: ldr      r0, [sp, #0x18]
00631d04: ldr      r1, [pc, #0x69c]
00631d08: ldr      r8, [sp, #0xc8]
00631d0c: ldr      r3, [r0]
00631d10: add      r1, pc, r1
00631d14: str      r1, [sp, #0x28]
00631d18: cmp      r3, r2
00631d1c: beq      #0x631ea8
00631d20: add      r4, sp, #0x9c
00631d24: mov      r3, r2
00631d28: ldr      r1, [sp, #0x18]
00631d2c: ldr      r2, [r8]
00631d30: mov      r0, r4
00631d34: bl       #0x5cc0a0
00631d38: ldr      r3, [sp, #0x9c]
00631d3c: add      r0, sp, #0xa0
00631d40: str      r3, [sp, #0x98]
00631d44: cmp      r3, #0
00631d48: ldrne    r2, [r3]
00631d4c: addne    r2, r2, #1
00631d50: strne    r2, [r3]
00631d54: ldr      sb, [sp, #0x1c]
00631d58: ldrne    r3, [sp, #0x98]
00631d5c: ldr      r2, [sb]
00631d60: str      r3, [sb]
00631d64: str      r2, [r0, #-8]!
00631d68: bl       #0x310be8
00631d6c: mov      r0, r4
00631d70: bl       #0x310be8
00631d74: ldr      sl, [r8, #0x10]
00631d78: cmp      sl, #0
00631d7c: str      sl, [sp, #0x20]
00631d80: ble      #0x631ea8
00631d84: ldr      r3, [pc, #0x620]
00631d88: ldr      r2, [pc, #0x620]
00631d8c: mov      r4, #0
00631d90: add      r3, pc, r3
00631d94: add      r3, r3, #0x4c
00631d98: str      r3, [sp, #0x3c]
00631d9c: ldr      r3, [pc, #0x610]
00631da0: add      r2, pc, r2
00631da4: str      r2, [sp, #0x2c]
00631da8: add      r3, pc, r3
00631dac: str      r3, [sp, #0x34]
00631db0: ldr      r3, [pc, #0x600]
00631db4: mov      r6, r4
00631db8: add      r3, pc, r3
00631dbc: str      r3, [sp, #0x38]
00631dc0: b        #0x631e38
00631dc4: ldr      r1, [sp, #0x1c]
00631dc8: ldr      r0, [r1]
00631dcc: ldr      r1, [r7, #0x10]
00631dd0: ldr      r3, [r0, #4]
00631dd4: ldrh     r2, [r3, #0xe]
00631dd8: cmp      r5, r2
00631ddc: ldrlo    sb, [r3, #0x20]
00631de0: movhs    sb, #0
00631de4: addlo    sb, sb, r5, lsl #4
00631de8: ldr      sl, [sb, #8]
00631dec: str      sl, [sp, #0xc]
00631df0: ldr      r1, [r1]
00631df4: cmp      sl, r1
00631df8: bls      #0x631eb4
00631dfc: ldr      r2, [r0, #0x1c]
00631e00: ldr      r3, [sb]
00631e04: ldr      r1, [pc, #0x5b0]
00631e08: cmp      r2, #0
00631e0c: addne    r2, r2, #4
00631e10: cmp      r3, #0
00631e14: addne    r3, r3, #4
00631e18: add      r1, pc, r1
00631e1c: mov      r0, #3
00631e20: bl       #0x60b034
00631e24: ldr      r3, [sp, #0x20]
00631e28: add      r6, r6, #1
00631e2c: add      r4, r4, #0x18
00631e30: cmp      r6, r3
00631e34: beq      #0x631ea8
00631e38: ldr      r7, [r8, #0x14]
00631e3c: ldr      ip, [sp, #0x18]
00631e40: mov      r2, #0
00631e44: ldr      r1, [r7, r4]
00631e48: ldr      r0, [ip]
00631e4c: bl       #0x5d308c
00631e50: movw     r3, #0xffff
00631e54: cmp      r0, r3
00631e58: add      r7, r7, r4
00631e5c: mov      r5, r0
00631e60: bne      #0x631dc4
00631e64: ldr      r3, [r7, #8]
00631e68: cmp      r3, #0x14
00631e6c: bne      #0x631e24
00631e70: ldr      r3, [r7, #0x14]
00631e74: ldr      r1, [sp, #0x18]
00631e78: add      r6, r6, #1
00631e7c: add      r4, r4, #0x18
00631e80: ldr      r0, [r1]
00631e84: ldr      r1, [r3, #4]
00631e88: bl       #0x5d4714
00631e8c: cmp      r0, #0xff
00631e90: ldrne    r2, [sp, #0x1c]
00631e94: ldrne    r3, [r2]
00631e98: strbne   r0, [r3, #8]
00631e9c: ldr      r3, [sp, #0x20]
00631ea0: cmp      r6, r3
00631ea4: bne      #0x631e38
00631ea8: ldr      r0, [sp, #0x1c]
00631eac: add      sp, sp, #0xa4
00631eb0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00631eb4: ldrb     fp, [sb, #6]
00631eb8: ldr      r1, [sp, #0x2c]
00631ebc: ldr      ip, [r7, #8]
00631ec0: mov      sl, #1
00631ec4: ldr      r1, [r1, fp, lsl #2]
00631ec8: str      ip, [sp, #0x30]
00631ecc: str      ip, [sp, #0x24]
00631ed0: ands     r1, r1, sl, lsl ip
00631ed4: bne      #0x631f4c
00631ed8: ldr      r3, [r0, #0x1c]
00631edc: cmp      r3, #0
00631ee0: addne    r3, r3, #4
00631ee4: str      r3, [sp, #0x30]
00631ee8: ldr      r5, [sb]
00631eec: cmp      r5, #0
00631ef0: addne    r5, r5, #4
00631ef4: cmp      fp, #0xff
00631ef8: beq      #0x631f84
00631efc: mov      r0, #0
00631f00: bl       #0x5e80b4
00631f04: ldr      r7, [r7, #8]
00631f08: ldr      sl, [r0, fp, lsl #2]
00631f0c: str      r7, [sp, #0x24]
00631f10: ldr      r1, [sp, #0x34]
00631f14: add      r0, sp, #0x40
00631f18: mov      r2, #0x58
00631f1c: bl       #0x30e868
00631f20: ldr      r0, [sp, #0x24]
00631f24: add      ip, sp, #0xa0
00631f28: ldr      r2, [sp, #0x30]
00631f2c: add      r3, ip, r0, lsl #2
00631f30: ldr      ip, [r3, #-0x60]
00631f34: mov      r0, #3
00631f38: mov      r3, r5
00631f3c: ldr      r1, [sp, #0x38]
00631f40: stm      sp, {sl, ip}
00631f44: bl       #0x60b034
00631f48: b        #0x631e24
00631f4c: sub      fp, fp, #9
00631f50: cmp      fp, #9
00631f54: addls    pc, pc, fp, lsl #2
00631f58: b        #0x632024
00631f5c: b        #0x631e24
00631f60: b        #0x631e24
00631f64: b        #0x632080
00631f68: b        #0x63215c
00631f6c: b        #0x6321ec
00631f70: b        #0x63227c
00631f74: b        #0x63230c
00631f78: b        #0x632024
00631f7c: b        #0x632024
00631f80: b        #0x631f90
00631f84: ldr      sl, [pc, #0x434]
00631f88: add      sl, pc, sl
00631f8c: b        #0x631f10
00631f90: ldr      r1, [sp, #0xc]
00631f94: cmp      r1, #0
00631f98: beq      #0x631e24
00631f9c: add      sl, sp, #0x40
00631fa0: mov      fp, #0
00631fa4: str      sl, [sp, #0x24]
00631fa8: mov      sb, fp
00631fac: ldr      sl, [sp, #0xc]
00631fb0: b        #0x631fec
00631fb4: ldr      ip, [sp, #0xcc]
00631fb8: cmp      ip, #0
00631fbc: beq      #0x631fdc
00631fc0: ldr      ip, [sp, #0x24]
00631fc4: ldr      r0, [sp, #0xcc]
00631fc8: ldr      r1, [sp, #0x1c]
00631fcc: mov      r2, r5
00631fd0: mov      r3, sb
00631fd4: str      ip, [sp]
00631fd8: bl       #0x65b0fc
00631fdc: add      sb, sb, #1
00631fe0: cmp      sb, sl
00631fe4: add      fp, fp, #4
00631fe8: beq      #0x631e24
00631fec: ldr      r3, [r7, #0x14]
00631ff0: add      r3, r3, fp
00631ff4: ldr      r3, [r3]
00631ff8: str      r3, [sp, #0x40]
00631ffc: ldr      r2, [r3, #-4]
00632000: cmp      r2, #0
00632004: beq      #0x631e24
00632008: ldrsb    r2, [r3]
0063200c: cmp      r2, #0x23
00632010: bne      #0x631fb4
00632014: ldrsb    r3, [r3, #1]
00632018: cmp      r3, #0
0063201c: beq      #0x631e24
00632020: b        #0x631fb4
00632024: ldr      r1, [sp, #0x28]
00632028: ldr      r3, [pc, #0x394]
0063202c: ldr      sb, [sp, #0x30]
00632030: ldr      sl, [sp, #0x28]
00632034: ldr      r2, [r1, r3]
00632038: ldr      r1, [pc, #0x388]
0063203c: add      r3, sb, #1
00632040: ldr      ip, [sp, #0x28]
00632044: ldr      r1, [sl, r1]
00632048: ldr      sl, [r2, r3, lsl #2]
0063204c: ldr      r2, [pc, #0x378]
00632050: ldrb     r1, [r1, sl]
00632054: ldr      r2, [ip, r2]
00632058: ldr      ip, [sp, #0x3c]
0063205c: ldr      lr, [ip, sb, lsl #2]
00632060: ldrb     ip, [r2, r3]
00632064: ldr      r3, [r7, #0x14]
00632068: mov      r2, lr
0063206c: mul      ip, ip, r1
00632070: mov      r1, r5
00632074: str      ip, [sp]
00632078: bl       #0x5ccf40
0063207c: b        #0x631e24
00632080: add      fp, sp, #0x40
00632084: mov      r0, fp
00632088: bl       #0x631c14
0063208c: ldr      sl, [sp, #0x28]
00632090: ldr      r3, [pc, #0x32c]
00632094: ldr      r2, [r7, #8]
00632098: ldr      ip, [sb, #8]
0063209c: ldr      r1, [sl, r3]
006320a0: ldr      r3, [pc, #0x320]
006320a4: add      r2, r2, #1
006320a8: ldr      r1, [r1, r2, lsl #2]
006320ac: ldr      r0, [sl, r3]
006320b0: ldr      r3, [pc, #0x314]
006320b4: cmp      ip, #0
006320b8: ldrb     r1, [r0, r1]
006320bc: ldr      r3, [sl, r3]
006320c0: ldrb     r3, [r3, r2]
006320c4: mul      r3, r3, r1
006320c8: beq      #0x631e24
006320cc: mov      sb, #0
006320d0: str      r4, [sp, #0x24]
006320d4: str      r6, [sp, #0x30]
006320d8: mov      sl, sb
006320dc: mov      r6, r5
006320e0: mov      r4, r3
006320e4: mov      r5, ip
006320e8: b        #0x6320fc
006320ec: add      sl, sl, #1
006320f0: cmp      sl, r5
006320f4: add      sb, sb, r4
006320f8: beq      #0x63239c
006320fc: ldr      lr, [r7, #0x14]
00632100: mov      ip, #0
00632104: strb     ip, [sp, #0x80]
00632108: add      lr, lr, sb
0063210c: mov      ip, fp
00632110: ldm      lr!, {r0, r1, r2, r3}
00632114: stm      ip!, {r0, r1, r2, r3}
00632118: ldm      lr!, {r0, r1, r2, r3}
0063211c: stm      ip!, {r0, r1, r2, r3}
00632120: ldm      lr!, {r0, r1, r2, r3}
00632124: stm      ip!, {r0, r1, r2, r3}
00632128: ldm      lr, {r0, r1, r2, r3}
0063212c: stm      ip, {r0, r1, r2, r3}
00632130: mov      r0, fp
00632134: bl       #0x5ba19c
00632138: cmp      r0, #0
0063213c: bne      #0x6320ec
00632140: ldr      r3, [sp, #0x1c]
00632144: mov      r2, sl
00632148: mov      r1, r6
0063214c: ldr      r0, [r3]
00632150: mov      r3, fp
00632154: bl       #0x5cb4dc
00632158: b        #0x6320ec
0063215c: cmp      r5, r2
00632160: ldrlo    r3, [r3, #0x20]
00632164: movhs    r3, #0
00632168: ldr      sl, [r7, #0x14]
0063216c: addlo    r3, r3, r5, lsl #4
00632170: ldr      sb, [r3, #8]
00632174: cmp      sb, #0
00632178: beq      #0x631e24
0063217c: str      r4, [sp, #0x24]
00632180: ldr      r4, [sp, #0x1c]
00632184: mov      r7, #0
00632188: add      fp, sp, #0x40
0063218c: ldr      r0, [sl, r7, lsl #2]
00632190: mov      r2, r7
00632194: mov      r1, r5
00632198: ldr      r0, [r0]
0063219c: mov      r3, fp
006321a0: add      r7, r7, #1
006321a4: cmp      r0, #0
006321a8: beq      #0x6321dc
006321ac: ldr      r0, [r0, #0x10]
006321b0: cmp      r0, #0
006321b4: str      r0, [sp, #0x40]
006321b8: ldrne    ip, [r0, #4]
006321bc: addne    ip, ip, #1
006321c0: strne    ip, [r0, #4]
006321c4: ldr      r0, [r4]
006321c8: bl       #0x5cd324
006321cc: ldr      r0, [sp, #0x40]
006321d0: cmp      r0, #0
006321d4: beq      #0x6321dc
006321d8: bl       #0x31d584
006321dc: cmp      r7, sb
006321e0: bne      #0x63218c
006321e4: ldr      r4, [sp, #0x24]
006321e8: b        #0x631e24
006321ec: cmp      r5, r2
006321f0: ldrlo    r3, [r3, #0x20]
006321f4: movhs    r3, #0
006321f8: ldr      sl, [r7, #0x14]
006321fc: addlo    r3, r3, r5, lsl #4
00632200: ldr      sb, [r3, #8]
00632204: cmp      sb, #0
00632208: beq      #0x631e24
0063220c: str      r4, [sp, #0x24]
00632210: ldr      r4, [sp, #0x1c]
00632214: mov      r7, #0
00632218: add      fp, sp, #0x40
0063221c: ldr      r0, [sl, r7, lsl #2]
00632220: mov      r2, r7
00632224: mov      r1, r5
00632228: ldr      r0, [r0]
0063222c: mov      r3, fp
00632230: add      r7, r7, #1
00632234: cmp      r0, #0
00632238: beq      #0x63226c
0063223c: ldr      r0, [r0, #0x10]
00632240: cmp      r0, #0
00632244: str      r0, [sp, #0x40]
00632248: ldrne    ip, [r0, #4]
0063224c: addne    ip, ip, #1
00632250: strne    ip, [r0, #4]
00632254: ldr      r0, [r4]
00632258: bl       #0x5cd324
0063225c: ldr      r0, [sp, #0x40]
00632260: cmp      r0, #0
00632264: beq      #0x63226c
00632268: bl       #0x31d584
0063226c: cmp      r7, sb
00632270: bne      #0x63221c
00632274: ldr      r4, [sp, #0x24]
00632278: b        #0x631e24
0063227c: cmp      r5, r2
00632280: ldrlo    r3, [r3, #0x20]
00632284: movhs    r3, #0
00632288: ldr      sl, [r7, #0x14]
0063228c: addlo    r3, r3, r5, lsl #4
00632290: ldr      sb, [r3, #8]
00632294: cmp      sb, #0
00632298: beq      #0x631e24
0063229c: str      r4, [sp, #0x24]
006322a0: ldr      r4, [sp, #0x1c]
006322a4: mov      r7, #0
006322a8: add      fp, sp, #0x40
006322ac: ldr      r0, [sl, r7, lsl #2]
006322b0: mov      r2, r7
006322b4: mov      r1, r5
006322b8: ldr      r0, [r0]
006322bc: mov      r3, fp
006322c0: add      r7, r7, #1
006322c4: cmp      r0, #0
006322c8: beq      #0x6322fc
006322cc: ldr      r0, [r0, #0x10]
006322d0: cmp      r0, #0
006322d4: str      r0, [sp, #0x40]
006322d8: ldrne    ip, [r0, #4]
006322dc: addne    ip, ip, #1
006322e0: strne    ip, [r0, #4]
006322e4: ldr      r0, [r4]
006322e8: bl       #0x5cd324
006322ec: ldr      r0, [sp, #0x40]
006322f0: cmp      r0, #0
006322f4: beq      #0x6322fc
006322f8: bl       #0x31d584
006322fc: cmp      r7, sb
00632300: bne      #0x6322ac
00632304: ldr      r4, [sp, #0x24]
00632308: b        #0x631e24
0063230c: cmp      r5, r2
00632310: ldrlo    r3, [r3, #0x20]
00632314: movhs    r3, #0
00632318: ldr      sl, [r7, #0x14]
0063231c: addlo    r3, r3, r5, lsl #4
00632320: ldr      sb, [r3, #8]
00632324: cmp      sb, #0
00632328: beq      #0x631e24
0063232c: str      r4, [sp, #0x24]
00632330: ldr      r4, [sp, #0x1c]
00632334: mov      r7, #0
00632338: add      fp, sp, #0x40
0063233c: ldr      r0, [sl, r7, lsl #2]
00632340: mov      r2, r7
00632344: mov      r1, r5
00632348: ldr      r0, [r0]
0063234c: mov      r3, fp
00632350: add      r7, r7, #1
00632354: cmp      r0, #0
00632358: beq      #0x63238c
0063235c: ldr      r0, [r0, #0x10]
00632360: cmp      r0, #0
00632364: str      r0, [sp, #0x40]
00632368: ldrne    ip, [r0, #4]
0063236c: addne    ip, ip, #1
00632370: strne    ip, [r0, #4]
00632374: ldr      r0, [r4]
00632378: bl       #0x5cd324
0063237c: ldr      r0, [sp, #0x40]
00632380: cmp      r0, #0
00632384: beq      #0x63238c
00632388: bl       #0x31d584
0063238c: cmp      r7, sb
00632390: bne      #0x63233c
00632394: ldr      r4, [sp, #0x24]
00632398: b        #0x631e24
0063239c: ldr      r4, [sp, #0x24]
006323a0: ldr      r6, [sp, #0x30]
006323a4: b        #0x631e24
006323a8: eorseq   r2, r6, r0, lsl #27
006323ac: strhteq  r3, [fp], -ip
006323b0: eoreq    r3, fp, ip, lsr #1
006323b4: eorseq   r5, r2, r0, asr #15
006323b8: eoreq    r3, fp, r0, ror #3
006323bc: eoreq    r3, fp, r0, asr r1
