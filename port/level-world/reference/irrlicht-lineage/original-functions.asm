
# _ZN6glitch3gui15CGUIEnvironment7onEventERKNS_6SEventE
00535710: push     {r4, lr}
00535714: ldr      r3, [r0, #0x1c4]
00535718: cmp      r3, #0
0053571c: beq      #0x535760
00535720: ldr      r2, [r1]
00535724: cmp      r2, #1
00535728: beq      #0x535760
0053572c: cmp      r2, #2
00535730: beq      #0x535760
00535734: cmp      r2, #0
00535738: bne      #0x53574c
0053573c: ldr      r2, [r1, #8]
00535740: add      r0, r0, #8
00535744: cmp      r2, r0
00535748: beq      #0x535760
0053574c: mov      r0, r3
00535750: ldr      r3, [r3]
00535754: mov      lr, pc
00535758: ldr      pc, [r3, #8]
0053575c: pop      {r4, pc}
00535760: mov      r0, #0
00535764: pop      {r4, pc}

# _ZN6glitch5scene10ISceneNode9onAnimateEj
00596d6c: push     {r4, r5, r6, r7, r8, lr}
00596d70: ldr      r3, [r0, #0x11c]
00596d74: mov      r6, r0
00596d78: mov      r5, r1
00596d7c: tst      r3, #0x400
00596d80: beq      #0x596d8c
00596d84: tst      r3, #1
00596d88: beq      #0x596e20
00596d8c: tst      r3, #0x200
00596d90: beq      #0x596e20
00596d94: mov      r7, r6
00596d98: ldr      r4, [r7, #0xfc]!
00596d9c: b        #0x596dc0
00596da0: ldr      r3, [r4, #8]
00596da4: mov      r1, r6
00596da8: mov      r2, r5
00596dac: mov      r0, r3
00596db0: ldr      r3, [r3]
00596db4: mov      lr, pc
00596db8: ldr      pc, [r3, #0x10]
00596dbc: ldr      r4, [r4]
00596dc0: cmp      r7, r4
00596dc4: bne      #0x596da0
00596dc8: ldr      r3, [r6]
00596dcc: mov      r0, r6
00596dd0: mov      r1, #0
00596dd4: mov      r7, r6
00596dd8: mov      lr, pc
00596ddc: ldr      pc, [r3, #0xb8]
00596de0: ldr      r4, [r7, #0xf4]!
00596de4: b        #0x596e0c
00596de8: cmp      r4, #0
00596dec: moveq    r3, r4
00596df0: subne    r3, r4, #4
00596df4: mov      r0, r3
00596df8: mov      r1, r5
00596dfc: ldr      r3, [r3]
00596e00: mov      lr, pc
00596e04: ldr      pc, [r3, #0x14]
00596e08: ldr      r4, [r4]
00596e0c: cmp      r7, r4
00596e10: bne      #0x596de8
00596e14: ldr      r3, [r6, #0x11c]
00596e18: bic      r3, r3, #0x20
00596e1c: str      r3, [r6, #0x11c]
00596e20: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch5scene13CSceneManager7drawAllERKSt6vectorIPNS0_10ISceneNodeENS_4core10SAllocatorIS4_LNS_6memory13E_MEMORY_HINTE0EEEE
0058b728: push     {r4, r5, r6, lr}
0058b72c: mov      r4, r0
0058b730: mov      r5, r1
0058b734: ldr      r3, [r0]
0058b738: ldr      r1, [r0, #0x14]
0058b73c: mov      lr, pc
0058b740: ldr      pc, [r3, #0x40]
0058b744: mov      r1, r5
0058b748: mov      r0, r4
0058b74c: bl       #0x58b6c8
0058b750: mov      r0, r4
0058b754: ldr      r3, [r4]
0058b758: mov      lr, pc
0058b75c: ldr      pc, [r3, #0x44]
0058b760: mov      r1, r5
0058b764: mov      r0, r4
0058b768: ldr      r3, [r4]
0058b76c: mov      lr, pc
0058b770: ldr      pc, [r3, #0x2c]
0058b774: mov      r0, r4
0058b778: ldr      r3, [r4]
0058b77c: mov      lr, pc
0058b780: ldr      pc, [r3, #0x4c]
0058b784: mov      r0, r4
0058b788: ldr      r3, [r4]
0058b78c: ldr      r1, [r4, #0x14]
0058b790: mov      lr, pc
0058b794: ldr      pc, [r3, #0x48]
0058b798: mov      r3, #9
0058b79c: add      r0, r4, #0x114
0058b7a0: str      r3, [r4, #0x174]
0058b7a4: pop      {r4, r5, r6, lr}
0058b7a8: b        #0x58a75c

# _ZN6glitch5scene10ISceneNode8addChildEPS1_
00598864: cmp      r1, r0
00598868: cmpne    r1, #0
0059886c: push     {r4, r5, r6, lr}
00598870: mov      r5, r0
00598874: mov      r4, r1
00598878: bne      #0x598880
0059887c: pop      {r4, r5, r6, pc}
00598880: ldr      r3, [r1]
00598884: mov      r0, r1
00598888: ldr      r3, [r3, #-0xc]
0059888c: add      r3, r1, r3
00598890: ldr      r2, [r3, #4]
00598894: add      r2, r2, #1
00598898: str      r2, [r3, #4]
0059889c: ldr      r3, [r1]
005988a0: mov      lr, pc
005988a4: ldr      pc, [r3, #0x68]
005988a8: ldr      r2, [r5, #0xf8]
005988ac: add      r3, r4, #4
005988b0: add      r1, r5, #0xf4
005988b4: str      r2, [r4, #8]
005988b8: str      r3, [r2]
005988bc: str      r3, [r5, #0xf8]
005988c0: str      r1, [r4, #4]
005988c4: ldr      r3, [r5, #0xf0]
005988c8: mov      r0, r4
005988cc: mov      r1, r5
005988d0: add      r3, r3, #1
005988d4: str      r3, [r5, #0xf0]
005988d8: bl       #0x5971e0
005988dc: ldr      r0, [r5, #0x110]
005988e0: cmp      r0, #0
005988e4: beq      #0x5988ec
005988e8: bl       #0x5890b4
005988ec: ldr      r1, [r5, #0x11c]
005988f0: mov      r0, r4
005988f4: ldr      r3, [r4]
005988f8: and      r1, r1, #1
005988fc: mov      lr, pc
00598900: ldr      pc, [r3, #0xec]
00598904: pop      {r4, r5, r6, pc}

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

# _ZN6glitch5scene10ISceneNode19onRegisterSceneNodeEv
00596d08: mov      r0, #1
00596d0c: bx       lr

# _ZN6glitch5scene13CSceneManager7drawAllEPNS0_10ISceneNodeE
0058b7f4: push     {r4, r5, r6, lr}
0058b7f8: mov      r5, r1
0058b7fc: ldr      r3, [r0]
0058b800: ldr      r1, [r0, #0x18]
0058b804: mov      r4, r0
0058b808: mov      lr, pc
0058b80c: ldr      pc, [r3, #0x40]
0058b810: cmp      r5, #0
0058b814: beq      #0x58b874
0058b818: mov      r0, r4
0058b81c: ldr      r3, [r4]
0058b820: mov      lr, pc
0058b824: ldr      pc, [r3, #0x44]
0058b828: mov      r1, r5
0058b82c: mov      r0, r4
0058b830: ldr      r3, [r4]
0058b834: mov      lr, pc
0058b838: ldr      pc, [r3, #0x28]
0058b83c: mov      r0, r4
0058b840: ldr      r3, [r4]
0058b844: mov      lr, pc
0058b848: ldr      pc, [r3, #0x4c]
0058b84c: mov      r0, r4
0058b850: ldr      r3, [r4]
0058b854: ldr      r1, [r4, #0x18]
0058b858: mov      lr, pc
0058b85c: ldr      pc, [r3, #0x48]
0058b860: mov      r3, #9
0058b864: add      r0, r4, #0x114
0058b868: str      r3, [r4, #0x174]
0058b86c: pop      {r4, r5, r6, lr}
0058b870: b        #0x58a75c
0058b874: ldrb     r3, [r4, #0x288]
0058b878: cmp      r3, #0
0058b87c: beq      #0x58b818
0058b880: mov      r0, r4
0058b884: bl       #0x58b7ac
0058b888: b        #0x58b818

# _ZN6glitch5scene13CSceneManager24registerNodeForRenderingEPNS0_10ISceneNodeERKN5boost13intrusive_ptrINS_5video9CMaterialEEEPvNS0_24E_SCENE_NODE_RENDER_PASSEPKNS_4core8vector3dIfEEi
0058f348: push     {r4, r5, r6, r7, r8, sl, lr}
0058f34c: sub      sp, sp, #0xd4
0058f350: ldr      ip, [sp, #0xf0]
0058f354: mov      r4, r0
0058f358: mov      r5, r3
0058f35c: ldr      r8, [sp, #0xf4]
0058f360: ldr      r6, [sp, #0xf8]
0058f364: cmp      ip, #8
0058f368: addls    pc, pc, ip, lsl #2
0058f36c: b        #0x58f43c
0058f370: b        #0x58f468
0058f374: b        #0x58f4d8
0058f378: b        #0x58f528
0058f37c: b        #0x58f55c
0058f380: b        #0x58f610
0058f384: b        #0x58f71c
0058f388: b        #0x58f6b4
0058f38c: b        #0x58f6e8
0058f390: b        #0x58f394
0058f394: ldrb     r3, [r0, #0x28a]
0058f398: cmp      r3, #0
0058f39c: beq      #0x58f750
0058f3a0: ldr      r7, [r2]
0058f3a4: add      r4, r0, #0x78
0058f3a8: cmp      r7, #0
0058f3ac: streq    r1, [sp, #0x50]
0058f3b0: streq    r5, [sp, #0x54]
0058f3b4: streq    r7, [sp, #0x58]
0058f3b8: beq      #0x58f3e0
0058f3bc: ldr      r3, [r7]
0058f3c0: add      r3, r3, #1
0058f3c4: str      r3, [r7]
0058f3c8: str      r1, [sp, #0x50]
0058f3cc: str      r5, [sp, #0x54]
0058f3d0: str      r7, [sp, #0x58]
0058f3d4: ldr      r3, [r7]
0058f3d8: add      r3, r3, #1
0058f3dc: str      r3, [r7]
0058f3e0: cmn      r6, #0x80000001
0058f3e4: strne    r6, [sp, #0x5c]
0058f3e8: beq      #0x58f820
0058f3ec: mov      r0, r4
0058f3f0: add      r1, sp, #0x50
0058f3f4: bl       #0x352058
0058f3f8: ldr      r4, [sp, #0x58]
0058f3fc: cmp      r4, #0
0058f400: beq      #0x58f428
0058f404: ldr      r3, [r4]
0058f408: sub      r3, r3, #1
0058f40c: cmp      r3, #0
0058f410: str      r3, [r4]
0058f414: bne      #0x58f428
0058f418: mov      r0, r4
0058f41c: bl       #0x5cbf78
0058f420: mov      r0, r4
0058f424: bl       #0x30e2b0
0058f428: cmp      r7, #0
0058f42c: beq      #0x58f50c
0058f430: mov      r0, r7
0058f434: bl       #0x589de8
0058f438: b        #0x58f50c
0058f43c: ldr      r3, [pc, #0x540]
0058f440: mov      r0, #0
0058f444: add      r3, pc, r3
0058f448: ldr      r2, [r3]
0058f44c: ldr      r1, [r3, #4]
0058f450: add      r2, r2, #1
0058f454: add      r1, r1, #1
0058f458: str      r1, [r3, #4]
0058f45c: str      r2, [r3]
0058f460: add      sp, sp, #0xd4
0058f464: pop      {r4, r5, r6, r7, r8, sl, pc}
0058f468: ldr      ip, [r0, #0x3c]
0058f46c: ldr      r3, [r0, #0x40]
0058f470: rsb      r6, ip, r3
0058f474: asrs     r6, r6, #3
0058f478: beq      #0x58f4a8
0058f47c: ldr      r2, [ip]
0058f480: cmp      r2, r1
0058f484: movne    r2, #0
0058f488: bne      #0x58f49c
0058f48c: b        #0x58f43c
0058f490: ldr      r0, [ip, r2, lsl #3]
0058f494: cmp      r0, r1
0058f498: beq      #0x58f43c
0058f49c: add      r2, r2, #1
0058f4a0: cmp      r2, r6
0058f4a4: bne      #0x58f490
0058f4a8: ldr      r2, [r4, #0x44]
0058f4ac: str      r5, [sp, #0xa8]
0058f4b0: str      r1, [sp, #0xa4]
0058f4b4: cmp      r3, r2
0058f4b8: beq      #0x58f83c
0058f4bc: str      r1, [r3]
0058f4c0: ldr      r2, [sp, #0xa8]
0058f4c4: str      r2, [r3, #4]
0058f4c8: ldr      r3, [r4, #0x40]
0058f4cc: add      r3, r3, #8
0058f4d0: str      r3, [r4, #0x40]
0058f4d4: b        #0x58f50c
0058f4d8: add      r6, sp, #0x70
0058f4dc: mov      r0, r6
0058f4e0: add      r2, r4, #0xe8
0058f4e4: bl       #0x350cf4
0058f4e8: ldr      ip, [r4, #0x4c]
0058f4ec: ldr      r3, [r4, #0x50]
0058f4f0: cmp      ip, r3
0058f4f4: beq      #0x58f960
0058f4f8: ldm      r6, {r0, r1, r2, r3}
0058f4fc: stm      ip, {r0, r1, r2, r3}
0058f500: ldr      r3, [r4, #0x4c]
0058f504: add      r3, r3, #0x10
0058f508: str      r3, [r4, #0x4c]
0058f50c: ldr      r3, [pc, #0x474]
0058f510: mov      r0, #1
0058f514: add      r3, pc, r3
0058f518: ldr      r2, [r3]
0058f51c: add      r2, r2, r0
0058f520: str      r2, [r3]
0058f524: b        #0x58f460
0058f528: ldr      r3, [r0, #0x70]
0058f52c: ldr      r2, [r0, #0x74]
0058f530: str      r5, [sp, #0xa0]
0058f534: str      r1, [sp, #0x9c]
0058f538: cmp      r3, r2
0058f53c: beq      #0x58f91c
0058f540: str      r1, [r3]
0058f544: ldr      r2, [sp, #0xa0]
0058f548: str      r2, [r3, #4]
0058f54c: ldr      r3, [r0, #0x70]
0058f550: add      r3, r3, #8
0058f554: str      r3, [r0, #0x70]
0058f558: b        #0x58f50c
0058f55c: ldr      r7, [r2]
0058f560: cmp      r7, #0
0058f564: beq      #0x58f87c
0058f568: mov      r0, r7
0058f56c: str      r1, [sp, #0x14]
0058f570: str      r2, [sp, #0x10]
0058f574: bl       #0x5c5d34
0058f578: ldr      r3, [r7, #4]
0058f57c: mov      ip, #0xc
0058f580: ldr      r1, [sp, #0x14]
0058f584: ldr      r3, [r3, #0x18]
0058f588: ldr      r2, [sp, #0x10]
0058f58c: mla      r3, ip, r0, r3
0058f590: ldr      r3, [r3, #8]
0058f594: ldr      r3, [r3, #4]
0058f598: tst      r3, #0x10000
0058f59c: bne      #0x58f7b8
0058f5a0: ldr      r7, [r2]
0058f5a4: add      r4, r4, #0x78
0058f5a8: cmp      r7, #0
0058f5ac: beq      #0x58f880
0058f5b0: ldr      r3, [r7]
0058f5b4: add      r3, r3, #1
0058f5b8: str      r3, [r7]
0058f5bc: str      r1, [sp, #0x40]
0058f5c0: str      r5, [sp, #0x44]
0058f5c4: str      r7, [sp, #0x48]
0058f5c8: ldr      r3, [r7]
0058f5cc: add      r3, r3, #1
0058f5d0: str      r3, [r7]
0058f5d4: cmn      r6, #0x80000001
0058f5d8: strne    r6, [sp, #0x4c]
0058f5dc: beq      #0x58f894
0058f5e0: mov      r0, r4
0058f5e4: add      r1, sp, #0x40
0058f5e8: bl       #0x352058
0058f5ec: ldr      r0, [sp, #0x48]
0058f5f0: cmp      r0, #0
0058f5f4: beq      #0x58f5fc
0058f5f8: bl       #0x589de8
0058f5fc: cmp      r7, #0
0058f600: beq      #0x58f50c
0058f604: mov      r0, r7
0058f608: bl       #0x589de8
0058f60c: b        #0x58f50c
0058f610: ldr      r7, [r2]
0058f614: cmp      r7, #0
0058f618: streq    r1, [sp, #0x60]
0058f61c: streq    r3, [sp, #0x64]
0058f620: streq    r7, [sp, #0x68]
0058f624: beq      #0x58f64c
0058f628: ldr      r3, [r7]
0058f62c: add      r3, r3, #1
0058f630: str      r3, [r7]
0058f634: str      r1, [sp, #0x60]
0058f638: str      r5, [sp, #0x64]
0058f63c: str      r7, [sp, #0x68]
0058f640: ldr      r3, [r7]
0058f644: add      r3, r3, #1
0058f648: str      r3, [r7]
0058f64c: cmn      r6, #0x80000001
0058f650: strne    r6, [sp, #0x6c]
0058f654: beq      #0x58f860
0058f658: ldr      r1, [r4, #0x7c]
0058f65c: ldr      r3, [r4, #0x80]
0058f660: cmp      r1, r3
0058f664: beq      #0x58f940
0058f668: ldr      r3, [sp, #0x60]
0058f66c: str      r3, [r1]
0058f670: ldr      r3, [sp, #0x64]
0058f674: str      r3, [r1, #4]
0058f678: ldr      r3, [sp, #0x68]
0058f67c: str      r3, [r1, #8]
0058f680: cmp      r3, #0
0058f684: ldrne    r2, [r3]
0058f688: addne    r2, r2, #1
0058f68c: strne    r2, [r3]
0058f690: ldr      r3, [sp, #0x6c]
0058f694: str      r3, [r1, #0xc]
0058f698: ldr      r3, [r4, #0x7c]
0058f69c: add      r3, r3, #0x10
0058f6a0: str      r3, [r4, #0x7c]
0058f6a4: ldr      r0, [sp, #0x68]
0058f6a8: cmp      r0, #0
0058f6ac: bne      #0x58f5f8
0058f6b0: b        #0x58f5fc
0058f6b4: ldr      r3, [r0, #0x64]
0058f6b8: ldr      r2, [r0, #0x68]
0058f6bc: str      r5, [sp, #0x90]
0058f6c0: str      r1, [sp, #0x8c]
0058f6c4: cmp      r3, r2
0058f6c8: beq      #0x58f8d4
0058f6cc: str      r1, [r3]
0058f6d0: ldr      r2, [sp, #0x90]
0058f6d4: str      r2, [r3, #4]
0058f6d8: ldr      r3, [r0, #0x64]
0058f6dc: add      r3, r3, #8
0058f6e0: str      r3, [r0, #0x64]
0058f6e4: b        #0x58f50c
0058f6e8: ldr      r3, [r0, #0x34]
0058f6ec: ldr      r2, [r0, #0x38]
0058f6f0: str      r5, [sp, #0x88]
0058f6f4: str      r1, [sp, #0x84]
0058f6f8: cmp      r3, r2
0058f6fc: beq      #0x58f8b0
0058f700: str      r1, [r3]
0058f704: ldr      r2, [sp, #0x88]
0058f708: str      r2, [r3, #4]
0058f70c: ldr      r3, [r0, #0x34]
0058f710: add      r3, r3, #8
0058f714: str      r3, [r0, #0x34]
0058f718: b        #0x58f50c
0058f71c: ldr      r3, [r0, #0x58]
0058f720: ldr      r2, [r0, #0x5c]
0058f724: str      r5, [sp, #0x98]
0058f728: str      r1, [sp, #0x94]
0058f72c: cmp      r3, r2
0058f730: beq      #0x58f8f8
0058f734: str      r1, [r3]
0058f738: ldr      r2, [sp, #0x98]
0058f73c: str      r2, [r3, #4]
0058f740: ldr      r3, [r0, #0x58]
0058f744: add      r3, r3, #8
0058f748: str      r3, [r0, #0x58]
0058f74c: b        #0x58f50c
0058f750: ldr      r3, [r2]
0058f754: add      r7, r0, #0x84
0058f758: add      r2, r0, #0xe8
0058f75c: cmp      r3, #0
0058f760: str      r3, [sp, #0xb0]
0058f764: ldrne    r0, [r3]
0058f768: add      r4, sp, #0x2c
0058f76c: addne    r0, r0, #1
0058f770: strne    r0, [r3]
0058f774: add      r3, sp, #0xb0
0058f778: mov      r0, r4
0058f77c: stm      sp, {r5, r8}
0058f780: str      r6, [sp, #8]
0058f784: bl       #0x354e8c
0058f788: mov      r0, r7
0058f78c: mov      r1, r4
0058f790: bl       #0x356b68
0058f794: ldr      r0, [sp, #0x34]
0058f798: cmp      r0, #0
0058f79c: beq      #0x58f7a4
0058f7a0: bl       #0x589de8
0058f7a4: ldr      r0, [sp, #0xb0]
0058f7a8: cmp      r0, #0
0058f7ac: beq      #0x58f50c
0058f7b0: bl       #0x589de8
0058f7b4: b        #0x58f50c
0058f7b8: ldrb     r3, [r4, #0x28a]
0058f7bc: cmp      r3, #0
0058f7c0: bne      #0x58f5a0
0058f7c4: ldr      r3, [r2]
0058f7c8: add      r7, sp, #0x18
0058f7cc: add      r2, r4, #0xe8
0058f7d0: cmp      r3, #0
0058f7d4: str      r3, [sp, #0xac]
0058f7d8: ldrne    r0, [r3]
0058f7dc: add      sl, r4, #0x84
0058f7e0: add      r4, sp, #0xac
0058f7e4: addne    r0, r0, #1
0058f7e8: strne    r0, [r3]
0058f7ec: mov      r3, r4
0058f7f0: mov      r0, r7
0058f7f4: stm      sp, {r5, r8}
0058f7f8: str      r6, [sp, #8]
0058f7fc: bl       #0x354e8c
0058f800: mov      r0, sl
0058f804: mov      r1, r7
0058f808: bl       #0x356b68
0058f80c: mov      r0, r7
0058f810: bl       #0x58d710
0058f814: mov      r0, r4
0058f818: bl       #0x310be8
0058f81c: b        #0x58f50c
0058f820: ldr      r3, [sp, #0x50]
0058f824: mov      r0, r3
0058f828: ldr      r3, [r3]
0058f82c: mov      lr, pc
0058f830: ldr      pc, [r3, #0xd8]
0058f834: str      r0, [sp, #0x5c]
0058f838: b        #0x58f3ec
0058f83c: mov      ip, #1
0058f840: mov      r1, r3
0058f844: add      r0, r4, #0x3c
0058f848: add      r2, sp, #0xa4
0058f84c: add      r3, sp, #0xc8
0058f850: str      ip, [sp, #4]
0058f854: str      ip, [sp]
0058f858: bl       #0x351afc
0058f85c: b        #0x58f50c
0058f860: ldr      r3, [sp, #0x60]
0058f864: mov      r0, r3
0058f868: ldr      r3, [r3]
0058f86c: mov      lr, pc
0058f870: ldr      pc, [r3, #0xd8]
0058f874: str      r0, [sp, #0x6c]
0058f878: b        #0x58f658
0058f87c: add      r4, r0, #0x78
0058f880: mov      r3, #0
0058f884: str      r1, [sp, #0x40]
0058f888: str      r5, [sp, #0x44]
0058f88c: str      r3, [sp, #0x48]
0058f890: b        #0x58f5d4
0058f894: ldr      r3, [sp, #0x40]
0058f898: mov      r0, r3
0058f89c: ldr      r3, [r3]
0058f8a0: mov      lr, pc
0058f8a4: ldr      pc, [r3, #0xd8]
0058f8a8: str      r0, [sp, #0x4c]
0058f8ac: b        #0x58f5e0
0058f8b0: mov      ip, #1
0058f8b4: mov      r1, r3
0058f8b8: add      r0, r0, #0x30
0058f8bc: add      r2, sp, #0x84
0058f8c0: add      r3, sp, #0xb4
0058f8c4: str      ip, [sp, #4]
0058f8c8: str      ip, [sp]
0058f8cc: bl       #0x351afc
0058f8d0: b        #0x58f50c
0058f8d4: mov      ip, #1
0058f8d8: mov      r1, r3
0058f8dc: add      r0, r0, #0x60
0058f8e0: add      r2, sp, #0x8c
0058f8e4: add      r3, sp, #0xb8
0058f8e8: str      ip, [sp, #4]
0058f8ec: str      ip, [sp]
0058f8f0: bl       #0x351c9c
0058f8f4: b        #0x58f50c
0058f8f8: mov      ip, #1
0058f8fc: mov      r1, r3
0058f900: add      r0, r0, #0x54
0058f904: add      r2, sp, #0x94
0058f908: add      r3, sp, #0xbc
0058f90c: str      ip, [sp, #4]
0058f910: str      ip, [sp]
0058f914: bl       #0x351c9c
0058f918: b        #0x58f50c
0058f91c: mov      ip, #1
0058f920: mov      r1, r3
0058f924: add      r0, r0, #0x6c
0058f928: add      r2, sp, #0x9c
0058f92c: add      r3, sp, #0xc4
0058f930: str      ip, [sp, #4]
0058f934: str      ip, [sp]
0058f938: bl       #0x351afc
0058f93c: b        #0x58f50c
0058f940: mov      ip, #1
0058f944: add      r0, r4, #0x78
0058f948: add      r2, sp, #0x60
0058f94c: add      r3, sp, #0xc0
0058f950: str      ip, [sp, #4]
0058f954: str      ip, [sp]
0058f958: bl       #0x351e84
0058f95c: b        #0x58f6a4
0058f960: mov      r1, ip
0058f964: add      r0, r4, #0x48
0058f968: mov      ip, #1
0058f96c: mov      r2, r6
0058f970: add      r3, sp, #0xcc
0058f974: str      ip, [sp, #4]
0058f978: str      ip, [sp]
0058f97c: bl       #0x351920
0058f980: b        #0x58f50c
0058f984: subeq    r7, r6, ip, lsr #8
0058f988: subeq    r7, r6, ip, asr r3

# _ZNK6glitch5scene13CSceneManager8isCulledEPKNS0_10ISceneNodeE
0058ab28: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0058ab2c: ldrb     r3, [r0, #0x250]
0058ab30: mov      r4, r1
0058ab34: cmp      r3, #0
0058ab38: beq      #0x58ab64
0058ab3c: ldr      r5, [r0, #0xe4]
0058ab40: cmp      r5, #0
0058ab44: beq      #0x58ab64
0058ab48: ldr      r3, [r1, #0x118]
0058ab4c: cmp      r3, #2
0058ab50: beq      #0x58ab6c
0058ab54: cmp      r3, #8
0058ab58: beq      #0x58ac70
0058ab5c: cmp      r3, #1
0058ab60: beq      #0x58aba8
0058ab64: mov      r0, #0
0058ab68: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0058ab6c: ldr      r3, [r5]
0058ab70: mov      r0, r5
0058ab74: mov      lr, pc
0058ab78: ldr      pc, [r3, #0x144]
0058ab7c: ldr      r3, [r4]
0058ab80: mov      r5, r0
0058ab84: mov      r0, r4
0058ab88: mov      lr, pc
0058ab8c: ldr      pc, [r3, #0x34]
0058ab90: mov      r1, r0
0058ab94: mov      r0, r5
0058ab98: bl       #0x35bec8
0058ab9c: eor      r0, r0, #1
0058aba0: uxtb     r0, r0
0058aba4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0058aba8: ldr      r3, [r1]
0058abac: mov      r0, r1
0058abb0: mov      lr, pc
0058abb4: ldr      pc, [r3, #0x34]
0058abb8: ldr      r3, [r5]
0058abbc: mov      r2, r0
0058abc0: mov      r0, r5
0058abc4: ldr      sb, [r2]
0058abc8: ldr      r8, [r2, #0x14]
0058abcc: ldr      r5, [r2, #4]
0058abd0: ldr      r7, [r2, #8]
0058abd4: ldr      sl, [r2, #0xc]
0058abd8: ldr      r6, [r2, #0x10]
0058abdc: mov      lr, pc
0058abe0: ldr      pc, [r3, #0x144]
0058abe4: mov      r4, r0
0058abe8: ldr      r1, [r4, #0x78]
0058abec: mov      r0, sb
0058abf0: bl       #0x30e9ac
0058abf4: cmp      r0, #0
0058abf8: beq      #0x58ac68
0058abfc: mov      r0, r5
0058ac00: ldr      r1, [r4, #0x7c]
0058ac04: bl       #0x30e9ac
0058ac08: cmp      r0, #0
0058ac0c: beq      #0x58ac68
0058ac10: mov      r0, r7
0058ac14: ldr      r1, [r4, #0x80]
0058ac18: bl       #0x30e9ac
0058ac1c: cmp      r0, #0
0058ac20: beq      #0x58ac68
0058ac24: mov      r0, sl
0058ac28: ldr      r1, [r4, #0x6c]
0058ac2c: bl       #0x30e4b4
0058ac30: cmp      r0, #0
0058ac34: beq      #0x58ac68
0058ac38: mov      r0, r6
0058ac3c: ldr      r1, [r4, #0x70]
0058ac40: bl       #0x30e4b4
0058ac44: cmp      r0, #0
0058ac48: beq      #0x58ac68
0058ac4c: mov      r0, r8
0058ac50: ldr      r1, [r4, #0x74]
0058ac54: bl       #0x30e4b4
0058ac58: cmp      r0, #0
0058ac5c: mov      r0, #0
0058ac60: movne    r0, #1
0058ac64: b        #0x58aca0
0058ac68: mov      r0, #1
0058ac6c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0058ac70: ldr      r3, [r5]
0058ac74: mov      r0, r5
0058ac78: mov      lr, pc
0058ac7c: ldr      pc, [r3, #0x144]
0058ac80: ldr      r3, [r4]
0058ac84: mov      r5, r0
0058ac88: mov      r0, r4
0058ac8c: mov      lr, pc
0058ac90: ldr      pc, [r3, #0x34]
0058ac94: mov      r1, r0
0058ac98: mov      r0, r5
0058ac9c: bl       #0x58aa8c
0058aca0: eor      r0, r0, #1
0058aca4: uxtb     r0, r0
0058aca8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN6glitch3gui10CGUIButton7onEventERKNS_6SEventE
006a6eac: push     {r4, r5, lr}
006a6eb0: ldrb     r3, [r0, #0x99]
006a6eb4: sub      sp, sp, #0x34
006a6eb8: mov      r4, r0
006a6ebc: cmp      r3, #0
006a6ec0: mov      r5, r1
006a6ec4: beq      #0x6a6f0c
006a6ec8: ldr      r3, [r1]
006a6ecc: cmp      r3, #1
006a6ed0: beq      #0x6a7034
006a6ed4: cmp      r3, #2
006a6ed8: beq      #0x6a6f54
006a6edc: cmp      r3, #0
006a6ee0: beq      #0x6a6f20
006a6ee4: ldr      r3, [r4, #0x24]
006a6ee8: cmp      r3, #0
006a6eec: beq      #0x6a6f18
006a6ef0: mov      r0, r3
006a6ef4: mov      r1, r5
006a6ef8: ldr      r3, [r3]
006a6efc: mov      lr, pc
006a6f00: ldr      pc, [r3, #8]
006a6f04: add      sp, sp, #0x34
006a6f08: pop      {r4, r5, pc}
006a6f0c: ldr      r3, [r0, #0x24]
006a6f10: cmp      r3, #0
006a6f14: bne      #0x6a6fb4
006a6f18: mov      r0, r3
006a6f1c: b        #0x6a6f04
006a6f20: ldr      r3, [r1, #0x10]
006a6f24: cmp      r3, #0
006a6f28: bne      #0x6a6ee4
006a6f2c: ldr      r3, [r1, #8]
006a6f30: cmp      r3, r0
006a6f34: bne      #0x6a6ee4
006a6f38: ldrb     r1, [r0, #0x159]
006a6f3c: cmp      r1, #0
006a6f40: bne      #0x6a6ee4
006a6f44: ldr      r3, [r0]
006a6f48: mov      lr, pc
006a6f4c: ldr      pc, [r3, #0x9c]
006a6f50: b        #0x6a6ee4
006a6f54: ldrb     r3, [r1, #0x10]
006a6f58: cmp      r3, #0
006a6f5c: beq      #0x6a6f70
006a6f60: ldr      r2, [r1, #0xc]
006a6f64: cmp      r2, #0xd
006a6f68: cmpne    r2, #0x20
006a6f6c: beq      #0x6a7168
006a6f70: ldrb     r2, [r4, #0x158]
006a6f74: cmp      r2, #0
006a6f78: beq      #0x6a6fc8
006a6f7c: ldrb     r1, [r4, #0x159]
006a6f80: cmp      r1, #0
006a6f84: bne      #0x6a6fc8
006a6f88: cmp      r3, #0
006a6f8c: beq      #0x6a6fd8
006a6f90: ldr      r3, [r5, #0xc]
006a6f94: cmp      r3, #0x1b
006a6f98: bne      #0x6a6ee4
006a6f9c: mov      r0, r4
006a6fa0: ldr      r3, [r4]
006a6fa4: mov      lr, pc
006a6fa8: ldr      pc, [r3, #0x9c]
006a6fac: mov      r0, #1
006a6fb0: b        #0x6a6f04
006a6fb4: mov      r0, r3
006a6fb8: ldr      r3, [r3]
006a6fbc: mov      lr, pc
006a6fc0: ldr      pc, [r3, #8]
006a6fc4: b        #0x6a6f04
006a6fc8: cmp      r3, #0
006a6fcc: bne      #0x6a6ee4
006a6fd0: cmp      r2, #0
006a6fd4: beq      #0x6a6ee4
006a6fd8: ldr      r3, [r5, #0xc]
006a6fdc: cmp      r3, #0xd
006a6fe0: cmpne    r3, #0x20
006a6fe4: bne      #0x6a6ee4
006a6fe8: ldrb     r1, [r4, #0x159]
006a6fec: cmp      r1, #0
006a6ff0: beq      #0x6a7210
006a6ff4: ldr      r3, [r4, #0x24]
006a6ff8: cmp      r3, #0
006a6ffc: beq      #0x6a71e0
006a7000: mov      r2, #0
006a7004: mov      r1, #5
006a7008: str      r1, [sp, #0x28]
006a700c: str      r4, [sp, #0x20]
006a7010: str      r2, [sp, #0x24]
006a7014: str      r2, [sp, #0x18]
006a7018: mov      r0, r3
006a701c: add      r1, sp, #0x18
006a7020: ldr      r3, [r3]
006a7024: mov      lr, pc
006a7028: ldr      pc, [r3, #8]
006a702c: mov      r0, #1
006a7030: b        #0x6a6f04
006a7034: ldr      r3, [r1, #0x14]
006a7038: cmp      r3, #0
006a703c: beq      #0x6a70f0
006a7040: cmp      r3, #3
006a7044: bne      #0x6a6ee4
006a7048: ldr      r3, [r1, #8]
006a704c: ldr      r2, [r0, #0x48]
006a7050: ldr      r1, [r1, #0xc]
006a7054: ldrb     r5, [r0, #0x158]
006a7058: cmp      r3, r2
006a705c: blt      #0x6a71d4
006a7060: ldr      r2, [r0, #0x4c]
006a7064: cmp      r1, r2
006a7068: blt      #0x6a71d4
006a706c: ldr      r2, [r0, #0x50]
006a7070: cmp      r3, r2
006a7074: bgt      #0x6a71d4
006a7078: ldr      r3, [r0, #0x54]
006a707c: cmp      r1, r3
006a7080: bgt      #0x6a71d4
006a7084: ldrb     r1, [r0, #0x159]
006a7088: cmp      r1, #0
006a708c: bne      #0x6a71e8
006a7090: ldr      r3, [r0]
006a7094: mov      lr, pc
006a7098: ldr      pc, [r3, #0x9c]
006a709c: ldrb     r3, [r4, #0x159]
006a70a0: cmp      r3, #0
006a70a4: bne      #0x6a71fc
006a70a8: cmp      r5, #0
006a70ac: beq      #0x6a71e0
006a70b0: ldr      r3, [r4, #0x24]
006a70b4: cmp      r3, #0
006a70b8: beq      #0x6a71e0
006a70bc: mov      r2, #0
006a70c0: mov      r1, #5
006a70c4: str      r1, [sp, #0x10]
006a70c8: str      r4, [sp, #8]
006a70cc: str      r2, [sp, #0xc]
006a70d0: str      r2, [sp]
006a70d4: mov      r0, r3
006a70d8: mov      r1, sp
006a70dc: ldr      r3, [r3]
006a70e0: mov      lr, pc
006a70e4: ldr      pc, [r3, #8]
006a70e8: mov      r0, #1
006a70ec: b        #0x6a6f04
006a70f0: ldr      r3, [r0, #0x150]
006a70f4: mov      r1, r0
006a70f8: mov      r0, r3
006a70fc: ldr      r3, [r3]
006a7100: mov      lr, pc
006a7104: ldr      pc, [r3, #0x1c]
006a7108: cmp      r0, #0
006a710c: beq      #0x6a7194
006a7110: ldr      r3, [r5, #8]
006a7114: ldr      r2, [r4, #0x48]
006a7118: ldr      r1, [r5, #0xc]
006a711c: cmp      r3, r2
006a7120: blt      #0x6a7148
006a7124: ldr      r2, [r4, #0x4c]
006a7128: cmp      r1, r2
006a712c: blt      #0x6a7148
006a7130: ldr      r2, [r4, #0x50]
006a7134: cmp      r3, r2
006a7138: bgt      #0x6a7148
006a713c: ldr      r3, [r4, #0x54]
006a7140: cmp      r1, r3
006a7144: ble      #0x6a7194
006a7148: ldr      r3, [r4, #0x150]
006a714c: mov      r1, r4
006a7150: mov      r0, r3
006a7154: ldr      r3, [r3]
006a7158: mov      lr, pc
006a715c: ldr      pc, [r3, #0x18]
006a7160: mov      r0, #0
006a7164: b        #0x6a6f04
006a7168: ldrb     r3, [r0, #0x159]
006a716c: cmp      r3, #0
006a7170: ldrbne   r1, [r0, #0x158]
006a7174: ldreq    r3, [r0]
006a7178: ldrne    r3, [r0]
006a717c: moveq    r1, #1
006a7180: eorne    r1, r1, #1
006a7184: mov      lr, pc
006a7188: ldr      pc, [r3, #0x9c]
006a718c: mov      r0, #1
006a7190: b        #0x6a6f04
006a7194: ldrb     r3, [r4, #0x159]
006a7198: cmp      r3, #0
006a719c: bne      #0x6a71b4
006a71a0: ldr      r3, [r4]
006a71a4: mov      r0, r4
006a71a8: mov      r1, #1
006a71ac: mov      lr, pc
006a71b0: ldr      pc, [r3, #0x9c]
006a71b4: ldr      r3, [r4, #0x150]
006a71b8: mov      r1, r4
006a71bc: mov      r0, r3
006a71c0: ldr      r3, [r3]
006a71c4: mov      lr, pc
006a71c8: ldr      pc, [r3, #0x10]
006a71cc: mov      r0, #1
006a71d0: b        #0x6a6f04
006a71d4: ldrb     r1, [r4, #0x159]
006a71d8: cmp      r1, #0
006a71dc: beq      #0x6a6f9c
006a71e0: mov      r0, #1
006a71e4: b        #0x6a6f04
006a71e8: ldr      r3, [r0]
006a71ec: eor      r1, r5, #1
006a71f0: mov      lr, pc
006a71f4: ldr      pc, [r3, #0x9c]
006a71f8: b        #0x6a709c
006a71fc: ldrb     r3, [r4, #0x158]
006a7200: cmp      r3, r5
006a7204: beq      #0x6a71e0
006a7208: ldr      r3, [r4, #0x24]
006a720c: b        #0x6a70bc
006a7210: ldr      r3, [r4]
006a7214: mov      r0, r4
006a7218: mov      lr, pc
006a721c: ldr      pc, [r3, #0x9c]
006a7220: b        #0x6a6ff4

# _ZN6glitch5scene10ISceneNode11removeChildEPS1_
00597004: ldr      r3, [r1, #0xec]
00597008: push     {r4, lr}
0059700c: cmp      r3, r0
00597010: beq      #0x59701c
00597014: mov      r0, #0
00597018: pop      {r4, pc}
0059701c: ldr      r2, [r1, #4]
00597020: add      ip, r1, #4
00597024: cmp      r2, #0
00597028: ldrne    r0, [r1, #8]
0059702c: strne    r2, [r0]
00597030: strne    r0, [r2, #4]
00597034: ldr      lr, [r3, #0xf0]
00597038: mov      r2, #0
0059703c: sub      r0, ip, #4
00597040: sub      lr, lr, #1
00597044: str      lr, [r3, #0xf0]
00597048: str      r2, [r1, #8]
0059704c: str      r2, [r1, #4]
00597050: str      r2, [r0, #0xec]
00597054: ldr      r3, [ip, #-4]
00597058: ldr      r3, [r3, #-0xc]
0059705c: add      r0, r0, r3
00597060: bl       #0x31d584
00597064: mov      r0, #1
00597068: pop      {r4, pc}

# _ZN6glitch16CAndroidOSDevice12createDriverEv
006a0454: push     {r4, lr}
006a0458: ldr      r3, [r0, #0x5c]
006a045c: mov      r4, r0
006a0460: cmp      r3, #1
006a0464: beq      #0x6a04a4
006a0468: ble      #0x6a04b8
006a046c: cmp      r3, #0x80
006a0470: beq      #0x6a0490
006a0474: cmp      r3, #0x100
006a0478: beq      #0x6a0490
006a047c: ldr      r0, [pc, #0x5c]
006a0480: mov      r1, #3
006a0484: add      r0, pc, r0
006a0488: pop      {r4, lr}
006a048c: b        #0x60aca0
006a0490: ldr      r0, [pc, #0x4c]
006a0494: mov      r1, #3
006a0498: add      r0, pc, r0
006a049c: pop      {r4, lr}
006a04a0: b        #0x60aca0
006a04a4: bl       #0x5b5dac
006a04a8: cmp      r0, #0
006a04ac: str      r0, [r4, #0x10]
006a04b0: beq      #0x6a04cc
006a04b4: pop      {r4, pc}
006a04b8: cmp      r3, #0
006a04bc: bne      #0x6a047c
006a04c0: bl       #0x5b9b38
006a04c4: str      r0, [r4, #0x10]
006a04c8: pop      {r4, pc}
006a04cc: ldr      r0, [pc, #0x14]
006a04d0: mov      r1, #3
006a04d4: add      r0, pc, r0
006a04d8: pop      {r4, lr}
006a04dc: b        #0x60aca0

# _ZN6glitch3gui12CGUICheckBox7onEventERKNS_6SEventE
006a7ed8: push     {r4, r5, r6, lr}
006a7edc: ldrb     r3, [r0, #0x99]
006a7ee0: sub      sp, sp, #0x30
006a7ee4: mov      r4, r0
006a7ee8: cmp      r3, #0
006a7eec: mov      r5, r1
006a7ef0: beq      #0x6a7f10
006a7ef4: ldr      r6, [r1]
006a7ef8: cmp      r6, #1
006a7efc: beq      #0x6a7fec
006a7f00: cmp      r6, #2
006a7f04: beq      #0x6a7f58
006a7f08: cmp      r6, #0
006a7f0c: beq      #0x6a7f3c
006a7f10: ldr      r3, [r4, #0x24]
006a7f14: cmp      r3, #0
006a7f18: moveq    r0, r3
006a7f1c: beq      #0x6a7f34
006a7f20: mov      r0, r3
006a7f24: mov      r1, r5
006a7f28: ldr      r3, [r3]
006a7f2c: mov      lr, pc
006a7f30: ldr      pc, [r3, #8]
006a7f34: add      sp, sp, #0x30
006a7f38: pop      {r4, r5, r6, pc}
006a7f3c: ldr      r3, [r1, #0x10]
006a7f40: cmp      r3, #0
006a7f44: bne      #0x6a7f10
006a7f48: ldr      r2, [r1, #8]
006a7f4c: cmp      r2, r0
006a7f50: strbeq   r3, [r0, #0x158]
006a7f54: b        #0x6a7f10
006a7f58: ldrb     r2, [r1, #0x10]
006a7f5c: cmp      r2, #0
006a7f60: beq      #0x6a7f7c
006a7f64: ldr      r3, [r1, #0xc]
006a7f68: cmp      r3, #0xd
006a7f6c: cmpne    r3, #0x20
006a7f70: moveq    r0, #1
006a7f74: strbeq   r0, [r4, #0x158]
006a7f78: beq      #0x6a7f34
006a7f7c: ldrb     r3, [r4, #0x158]
006a7f80: cmp      r3, #0
006a7f84: beq      #0x6a7f10
006a7f88: cmp      r2, #0
006a7f8c: bne      #0x6a80b0
006a7f90: ldr      r3, [r5, #0xc]
006a7f94: cmp      r3, #0xd
006a7f98: cmpne    r3, #0x20
006a7f9c: bne      #0x6a7f10
006a7fa0: ldr      r3, [r4, #0x24]
006a7fa4: strb     r2, [r4, #0x158]
006a7fa8: cmp      r3, #0
006a7fac: beq      #0x6a80f8
006a7fb0: ldrb     r1, [r4, #0x159]
006a7fb4: str      r2, [sp, #0x24]
006a7fb8: mov      r0, r3
006a7fbc: eor      r1, r1, #1
006a7fc0: strb     r1, [r4, #0x159]
006a7fc4: mov      r1, #7
006a7fc8: str      r1, [sp, #0x28]
006a7fcc: str      r2, [sp, #0x18]
006a7fd0: str      r4, [sp, #0x20]
006a7fd4: ldr      r3, [r3]
006a7fd8: add      r1, sp, #0x18
006a7fdc: mov      lr, pc
006a7fe0: ldr      pc, [r3, #8]
006a7fe4: mov      r0, #1
006a7fe8: b        #0x6a7f34
006a7fec: ldr      r3, [r1, #0x14]
006a7ff0: cmp      r3, #0
006a7ff4: beq      #0x6a80cc
006a7ff8: cmp      r3, #3
006a7ffc: bne      #0x6a7f10
006a8000: ldr      r3, [r0, #0x150]
006a8004: ldrb     r6, [r0, #0x158]
006a8008: mov      r1, r0
006a800c: mov      r0, r3
006a8010: ldr      r3, [r3]
006a8014: mov      lr, pc
006a8018: ldr      pc, [r3, #0x18]
006a801c: mov      r3, #0
006a8020: cmp      r6, #0
006a8024: strb     r3, [r4, #0x158]
006a8028: beq      #0x6a80f8
006a802c: ldr      r3, [r4, #0x24]
006a8030: cmp      r3, #0
006a8034: beq      #0x6a80f8
006a8038: ldr      r2, [r5, #8]
006a803c: ldr      r1, [r4, #0x48]
006a8040: ldr      r0, [r5, #0xc]
006a8044: cmp      r2, r1
006a8048: blt      #0x6a80f8
006a804c: ldr      r1, [r4, #0x4c]
006a8050: cmp      r0, r1
006a8054: blt      #0x6a80f8
006a8058: ldr      r1, [r4, #0x50]
006a805c: cmp      r2, r1
006a8060: bgt      #0x6a80f8
006a8064: ldr      r2, [r4, #0x54]
006a8068: cmp      r0, r2
006a806c: bgt      #0x6a80f8
006a8070: ldrb     r1, [r4, #0x159]
006a8074: mov      r2, #0
006a8078: mov      r0, r3
006a807c: eor      r1, r1, #1
006a8080: strb     r1, [r4, #0x159]
006a8084: mov      r1, #7
006a8088: str      r1, [sp, #0x10]
006a808c: str      r2, [sp, #0xc]
006a8090: str      r2, [sp]
006a8094: str      r4, [sp, #8]
006a8098: ldr      r3, [r3]
006a809c: mov      r1, sp
006a80a0: mov      lr, pc
006a80a4: ldr      pc, [r3, #8]
006a80a8: mov      r0, #1
006a80ac: b        #0x6a7f34
006a80b0: ldr      r3, [r5, #0xc]
006a80b4: cmp      r3, #0x1b
006a80b8: moveq    r3, #0
006a80bc: strbeq   r3, [r4, #0x158]
006a80c0: moveq    r0, #1
006a80c4: bne      #0x6a7f10
006a80c8: b        #0x6a7f34
006a80cc: strb     r6, [r0, #0x158]
006a80d0: bl       #0x60aee4
006a80d4: ldr      r3, [r4, #0x150]
006a80d8: str      r0, [r4, #0x15c]
006a80dc: mov      r1, r4
006a80e0: mov      r0, r3
006a80e4: ldr      r3, [r3]
006a80e8: mov      lr, pc
006a80ec: ldr      pc, [r3, #0x10]
006a80f0: mov      r0, r6
006a80f4: b        #0x6a7f34
006a80f8: mov      r0, #1
006a80fc: b        #0x6a7f34
