
# _ZNK6glitch7collada16CColladaDatabase19constructControllerEPNS_5video12IVideoDriverEPKcPNS0_14CRootSceneNodeE
0061aa00: push     {r4, r5, r6, lr}
0061aa04: mov      r5, r1
0061aa08: sub      sp, sp, #8
0061aa0c: mov      r4, r0
0061aa10: mov      r1, r3
0061aa14: mov      r0, r5
0061aa18: mov      r6, r2
0061aa1c: bl       #0x61a9a0
0061aa20: ldr      ip, [sp, #0x18]
0061aa24: mov      r3, r0
0061aa28: mov      r1, r5
0061aa2c: mov      r0, r4
0061aa30: mov      r2, r6
0061aa34: str      ip, [sp]
0061aa38: bl       #0x60fa24
0061aa3c: mov      r0, r4
0061aa40: add      sp, sp, #8
0061aa44: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada15CColladaFactory17createModularSkinERKNS0_16CColladaDatabaseEPNS0_20SInstanceModularSkinEPNS0_14CRootSceneNodeE
00631560: push     {r4, r5, r6, r7, lr}
00631564: mov      r1, #0
00631568: sub      sp, sp, #0x14
0063156c: mov      r5, r0
00631570: mov      r0, #0x5c
00631574: mov      r6, r2
00631578: mov      r7, r3
0063157c: bl       #0x5341ac
00631580: mvn      ip, #0
00631584: str      ip, [sp]
00631588: mov      ip, #1
0063158c: ldr      r3, [sp, #0x28]
00631590: str      ip, [sp, #4]
00631594: mov      r1, r6
00631598: mov      ip, #0
0063159c: mov      r2, r7
006315a0: mov      r4, r0
006315a4: str      ip, [sp, #8]
006315a8: bl       #0x649120
006315ac: cmp      r4, #0
006315b0: str      r4, [r5]
006315b4: ldrne    r3, [r4, #4]
006315b8: mov      r0, r5
006315bc: addne    r3, r3, #1
006315c0: strne    r3, [r4, #4]
006315c4: add      sp, sp, #0x14
006315c8: pop      {r4, r5, r6, r7, pc}

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

# _ZN6glitch7collada12CSkinnedMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERNS0_11SControllerEPNS0_14CRootSceneNodeE
006664f0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006664f4: ldr      r5, [pc, #0x4e4]
006664f8: ldr      ip, [pc, #0x4e4]
006664fc: mov      r4, r0
00666500: add      r5, pc, r5
00666504: ldr      ip, [r5, ip]
00666508: mov      r0, #0
0066650c: str      r0, [r4, #4]
00666510: add      ip, ip, #8
00666514: str      ip, [r4]
00666518: ldr      r0, [r1]
0066651c: mov      r7, r1
00666520: sub      sp, sp, #0x24
00666524: str      r0, [r4, #0xc]
00666528: ldr      r1, [r1, #4]
0066652c: cmp      r0, #0
00666530: str      r1, [r4, #0x10]
00666534: beq      #0x666548
00666538: ldr      r1, [r0, #4]
0066653c: cmp      r1, #0
00666540: addne    r1, r1, #1
00666544: strne    r1, [r0, #4]
00666548: ldr      ip, [pc, #0x498]
0066654c: ldr      r0, [pc, #0x498]
00666550: mov      r1, #0
00666554: ldr      ip, [r5, ip]
00666558: ldr      r0, [r5, r0]
0066655c: str      r1, [r4, #0x14]
00666560: add      ip, ip, #4
00666564: str      ip, [r4, #8]
00666568: add      r0, r0, #8
0066656c: mov      ip, #1
00666570: strb     ip, [r4, #0x18]
00666574: str      r0, [r4]
00666578: ldr      r6, [r3, #8]
0066657c: mov      ip, #0xbf000000
00666580: add      ip, ip, #0x800000
00666584: mov      r0, #0x3f800000
00666588: add      lr, r4, #0x44
0066658c: str      r6, [r4, #0x1c]
00666590: str      ip, [r4, #0x2c]
00666594: str      r0, [r4, #0x38]
00666598: strb     r1, [r4, #0x20]
0066659c: strb     r1, [r4, #0x22]
006665a0: strb     r1, [r4, #0x23]
006665a4: str      ip, [r4, #0x24]
006665a8: str      ip, [r4, #0x28]
006665ac: str      r0, [r4, #0x30]
006665b0: str      r0, [r4, #0x34]
006665b4: str      r1, [r4, #0x3c]
006665b8: str      r1, [r4, #0x40]
006665bc: str      r1, [r4, #0x44]
006665c0: str      r1, [lr, #4]
006665c4: str      r1, [r4, #0x4c]
006665c8: str      r1, [r4, #0x50]
006665cc: str      r1, [r4, #0x54]
006665d0: str      r1, [r4, #0x58]
006665d4: str      r1, [r4, #0x5c]
006665d8: str      r1, [r4, #0x60]
006665dc: str      r1, [r4, #0x64]
006665e0: str      r1, [r4, #0x68]
006665e4: str      r1, [r4, #0x6c]
006665e8: str      r1, [r4, #0x74]
006665ec: str      r1, [r4, #0x78]
006665f0: str      r1, [r4, #0x7c]
006665f4: str      r1, [r4, #0x80]
006665f8: str      r1, [r4, #0x98]
006665fc: str      r1, [r4, #0x84]
00666600: str      r1, [r4, #0x88]
00666604: str      r1, [r4, #0x8c]
00666608: str      r1, [r4, #0x90]
0066660c: str      r1, [r4, #0x94]
00666610: ldr      r3, [r3, #4]
00666614: mov      r1, r2
00666618: mov      r0, r4
0066661c: ldr      r2, [sp, #0x48]
00666620: str      r3, [r4, #8]
00666624: bl       #0x664af8
00666628: ldr      r3, [r7]
0066662c: ldr      r3, [r3, #0x24]
00666630: ldr      r3, [r3, #0x20]
00666634: ldr      sb, [r3, #4]
00666638: ldr      r6, [r3, #0x64]
0066663c: cmp      r6, #0
00666640: movle    r6, #0
00666644: movgt    r6, #1
00666648: cmp      sb, #0
0066664c: beq      #0x666680
00666650: ldr      r3, [pc, #0x398]
00666654: ldr      r1, [sb, #0x14]
00666658: ldr      r8, [r5, r3]
0066665c: ldr      r3, [r8]
00666660: ldr      r3, [r3, #0x20]
00666664: ldr      r3, [r3, #0x34]
00666668: mov      r0, r3
0066666c: ldr      r3, [r3]
00666670: mov      lr, pc
00666674: ldr      pc, [r3, #0xc]
00666678: subs     sb, r0, #0
0066667c: beq      #0x6669a8
00666680: ldr      r3, [pc, #0x36c]
00666684: cmp      r6, #0
00666688: str      sb, [sp, #0x10]
0066668c: ldr      r3, [r5, r3]
00666690: add      r3, r3, #8
00666694: str      r3, [sp, #0xc]
00666698: bne      #0x6667bc
0066669c: cmp      sb, #0
006666a0: beq      #0x6666ac
006666a4: mov      r0, sb
006666a8: bl       #0x31d584
006666ac: mov      r1, #0
006666b0: mov      r0, #0x38
006666b4: bl       #0x5341ac
006666b8: add      r5, r4, #0x70
006666bc: mov      r3, r6
006666c0: ldr      r1, [r4, #0x1c]
006666c4: mov      r2, r5
006666c8: mov      r7, r0
006666cc: bl       #0x66e714
006666d0: ldr      r3, [r4, #0x3c]
006666d4: str      r7, [r4, #0x3c]
006666d8: cmp      r3, #0
006666dc: beq      #0x6666f0
006666e0: mov      r0, r3
006666e4: ldr      r3, [r3]
006666e8: mov      lr, pc
006666ec: ldr      pc, [r3, #4]
006666f0: mov      r1, #0
006666f4: mov      r0, #0x30
006666f8: bl       #0x5341ac
006666fc: mov      r3, r6
00666700: ldr      r1, [r4, #0x1c]
00666704: mov      r2, r5
00666708: mov      r7, r0
0066670c: bl       #0x66b8dc
00666710: ldr      r3, [r4, #0x40]
00666714: str      r7, [r4, #0x40]
00666718: cmp      r3, #0
0066671c: beq      #0x666730
00666720: mov      r0, r3
00666724: ldr      r3, [r3]
00666728: mov      lr, pc
0066672c: ldr      pc, [r3, #4]
00666730: mov      r1, #0
00666734: mov      r0, #0x30
00666738: bl       #0x5341ac
0066673c: mov      r3, r6
00666740: ldr      r1, [r4, #0x1c]
00666744: mov      r2, r5
00666748: mov      r7, r0
0066674c: bl       #0x66d01c
00666750: ldr      r3, [r4, #0x44]
00666754: str      r7, [r4, #0x44]
00666758: cmp      r3, #0
0066675c: beq      #0x666770
00666760: mov      r0, r3
00666764: ldr      r3, [r3]
00666768: mov      lr, pc
0066676c: ldr      pc, [r3, #4]
00666770: mov      r1, #0
00666774: mov      r0, #0x34
00666778: bl       #0x5341ac
0066677c: mov      r3, r6
00666780: mov      r2, r5
00666784: ldr      r1, [r4, #0x1c]
00666788: mov      r7, r0
0066678c: bl       #0x66f658
00666790: ldr      r3, [r4, #0x48]
00666794: str      r7, [r4, #0x48]
00666798: cmp      r3, #0
0066679c: beq      #0x6667b0
006667a0: mov      r0, r3
006667a4: ldr      r3, [r3]
006667a8: mov      lr, pc
006667ac: ldr      pc, [r3, #4]
006667b0: mov      r0, r4
006667b4: add      sp, sp, #0x24
006667b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006667bc: ldr      r3, [r4, #0x1c]
006667c0: add      sl, sp, #0xc
006667c4: mov      r2, sl
006667c8: ldr      r1, [r3, #0x80]
006667cc: add      r0, sp, #0x1c
006667d0: bl       #0x663bd0
006667d4: ldr      r3, [sp, #0x1c]
006667d8: cmp      r3, #0
006667dc: ldrne    r2, [r3]
006667e0: addne    r2, r2, #1
006667e4: strne    r2, [r3]
006667e8: ldr      r5, [r4, #0x4c]
006667ec: cmp      r5, #0
006667f0: beq      #0x666820
006667f4: ldr      r3, [r5]
006667f8: sub      r3, r3, #1
006667fc: cmp      r3, #0
00666800: str      r3, [r5]
00666804: bne      #0x666820
00666808: ldr      r0, [r5, #0xc]
0066680c: cmp      r0, #0
00666810: beq      #0x666818
00666814: bl       #0x30e0b8
00666818: mov      r3, #0
0066681c: str      r3, [r5, #0xc]
00666820: ldr      r5, [sp, #0x1c]
00666824: cmp      r5, #0
00666828: str      r5, [r4, #0x4c]
0066682c: beq      #0x666864
00666830: ldr      r3, [r5]
00666834: sub      r3, r3, #1
00666838: cmp      r3, #0
0066683c: str      r3, [r5]
00666840: bne      #0x66685c
00666844: ldr      r0, [r5, #0xc]
00666848: cmp      r0, #0
0066684c: beq      #0x666854
00666850: bl       #0x30e0b8
00666854: mov      r3, #0
00666858: str      r3, [r5, #0xc]
0066685c: mov      r3, #0
00666860: str      r3, [sp, #0x1c]
00666864: ldr      r3, [r4, #0x1c]
00666868: add      r2, sp, #0x20
0066686c: add      r0, r4, #0x50
00666870: ldr      r1, [r3, #0x84]
00666874: mov      r3, #0
00666878: str      r3, [r2, #-8]!
0066687c: bl       #0x665f98
00666880: ldr      r5, [sp, #0x18]
00666884: cmp      r5, #0
00666888: beq      #0x6668a8
0066688c: ldr      r3, [r5]
00666890: sub      r3, r3, #1
00666894: cmp      r3, #0
00666898: str      r3, [r5]
0066689c: beq      #0x66698c
006668a0: mov      r3, #0
006668a4: str      r3, [sp, #0x18]
006668a8: ldr      r3, [r4, #0x1c]
006668ac: ldr      r2, [r3, #0x84]
006668b0: cmp      r2, #0
006668b4: ble      #0x66669c
006668b8: mov      r5, #0
006668bc: add      fp, sp, #0x14
006668c0: mov      r8, r5
006668c4: ldr      r3, [r3, #0x88]
006668c8: mov      r2, sl
006668cc: mov      r0, fp
006668d0: add      r3, r3, r5, lsl #3
006668d4: ldr      r1, [r3, #4]
006668d8: ldr      r7, [r4, #0x50]
006668dc: bl       #0x663c3c
006668e0: ldr      r3, [sp, #0x14]
006668e4: lsl      r2, r5, #2
006668e8: cmp      r3, #0
006668ec: ldrne    r1, [r3]
006668f0: addne    r1, r1, #1
006668f4: strne    r1, [r3]
006668f8: ldr      r3, [r7, r2]
006668fc: cmp      r3, #0
00666900: beq      #0x666934
00666904: ldr      r1, [r3]
00666908: sub      r1, r1, #1
0066690c: cmp      r1, #0
00666910: str      r1, [r3]
00666914: bne      #0x666934
00666918: ldr      r0, [r3, #0xc]
0066691c: cmp      r0, #0
00666920: beq      #0x666930
00666924: stm      sp, {r2, r3}
00666928: bl       #0x30e0b8
0066692c: ldm      sp, {r2, r3}
00666930: str      r8, [r3, #0xc]
00666934: ldr      r3, [sp, #0x14]
00666938: str      r3, [r7, r2]
0066693c: ldr      r7, [sp, #0x14]
00666940: cmp      r7, #0
00666944: beq      #0x666974
00666948: ldr      r3, [r7]
0066694c: sub      r3, r3, #1
00666950: cmp      r3, #0
00666954: str      r3, [r7]
00666958: bne      #0x666970
0066695c: ldr      r0, [r7, #0xc]
00666960: cmp      r0, #0
00666964: beq      #0x66696c
00666968: bl       #0x30e0b8
0066696c: str      r8, [r7, #0xc]
00666970: str      r8, [sp, #0x14]
00666974: ldr      r3, [r4, #0x1c]
00666978: add      r5, r5, #1
0066697c: ldr      r2, [r3, #0x84]
00666980: cmp      r5, r2
00666984: blt      #0x6668c4
00666988: b        #0x66669c
0066698c: ldr      r0, [r5, #0xc]
00666990: cmp      r0, #0
00666994: beq      #0x66699c
00666998: bl       #0x30e0b8
0066699c: mov      r3, #0
006669a0: str      r3, [r5, #0xc]
006669a4: b        #0x6668a0
006669a8: ldr      r2, [r7]
006669ac: ldr      r3, [r8]
006669b0: ldr      r2, [r2, #0x24]
006669b4: ldr      r3, [r3, #0x20]
006669b8: ldr      r2, [r2, #0x20]
006669bc: ldr      r3, [r3, #0x34]
006669c0: ldr      r2, [r2, #4]
006669c4: mov      r0, r3
006669c8: ldr      r3, [r3]
006669cc: ldr      r1, [r2, #0x14]
006669d0: mov      lr, pc
006669d4: ldr      pc, [r3, #0xc]
006669d8: mov      sb, r0
006669dc: b        #0x666680
006669e0: mlaseq   r2, r0, r5, lr
006669e4: andeq    r0, r0, r0, asr #20
006669e8: strheq   r1, [r0], -r4
006669ec: andeq    r1, r0, r4, lsl r3
006669f0: andeq    r4, r0, r8, asr #8
006669f4: strdeq   r4, r5, [r0], -ip

# _ZNK6glitch7collada16CColladaDatabase13constructSkinEPNS_5video12IVideoDriverEPNS0_11SControllerEPNS0_14CRootSceneNodeE
0060f924: push     {r4, r5, r6, lr}
0060f928: sub      sp, sp, #0x10
0060f92c: ldr      ip, [r1, #4]
0060f930: ldr      r5, [sp, #0x20]
0060f934: mov      lr, r1
0060f938: mov      r6, r2
0060f93c: mov      r1, ip
0060f940: mov      r4, r0
0060f944: ldr      ip, [ip]
0060f948: mov      r2, lr
0060f94c: str      r3, [sp]
0060f950: add      r0, sp, #0xc
0060f954: mov      r3, r6
0060f958: str      r5, [sp, #4]
0060f95c: mov      lr, pc
0060f960: ldr      pc, [ip, #0x58]
0060f964: mov      r0, r5
0060f968: ldr      r1, [sp, #0xc]
0060f96c: bl       #0x65b434
0060f970: ldr      r0, [sp, #0xc]
0060f974: cmp      r0, #0
0060f978: str      r0, [r4]
0060f97c: ldrne    r3, [r0, #4]
0060f980: addne    r3, r3, #1
0060f984: strne    r3, [r0, #4]
0060f988: ldrne    r0, [sp, #0xc]
0060f98c: cmp      r0, #0
0060f990: beq      #0x60f998
0060f994: bl       #0x31d584
0060f998: mov      r0, r4
0060f99c: add      sp, sp, #0x10
0060f9a0: pop      {r4, r5, r6, pc}

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
