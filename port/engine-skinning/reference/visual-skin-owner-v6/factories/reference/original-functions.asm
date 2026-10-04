
# _ZNK6glitch7collada19CModularSkinnedMesh11getModuleIdEPKc
006474b8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
006474bc: ldr      r3, [r0, #0x1c]
006474c0: mov      r7, r1
006474c4: ldr      sl, [r3]
006474c8: cmp      sl, #0
006474cc: ble      #0x647530
006474d0: ldr      sb, [r3, #4]
006474d4: mov      r8, #0
006474d8: add      r3, sb, r8, lsl #4
006474dc: ldr      r5, [r3, #8]
006474e0: cmp      r5, #0
006474e4: ble      #0x647524
006474e8: ldr      r6, [r3, #0xc]
006474ec: mov      r4, #0
006474f0: b        #0x647500
006474f4: add      r4, r4, #1
006474f8: cmp      r4, r5
006474fc: beq      #0x647524
00647500: add      r3, r6, r4, lsl #3
00647504: ldr      r3, [r3, #4]
00647508: mov      r1, r7
0064750c: ldr      r0, [r3, #4]
00647510: bl       #0x30e31c
00647514: cmp      r0, #0
00647518: bne      #0x6474f4
0064751c: mov      r0, r4
00647520: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00647524: add      r8, r8, #1
00647528: cmp      r8, sl
0064752c: bne      #0x6474d8
00647530: mvn      r4, #0
00647534: b        #0x64751c

# _ZN12VisualObject14SetModularSkinEii
00470e18: push     {r4, lr}
00470e1c: ldr      r0, [r0, #0x2c]
00470e20: ldr      r4, [pc, #0x2c]
00470e24: cmp      r0, #0
00470e28: cmnne    r1, #1
00470e2c: add      r4, pc, r4
00470e30: bne      #0x470e38
00470e34: pop      {r4, pc}
00470e38: bl       #0x649430
00470e3c: ldr      r3, [pc, #0x14]
00470e40: ldr      r3, [r4, r3]
00470e44: ldr      r3, [r3, #0x10]
00470e48: ldr      r0, [r3, #0x1c]
00470e4c: pop      {r4, lr}
00470e50: b        #0x5890a8
00470e54: subseq   r3, r2, r4, ror #24
00470e58: strdeq   r3, r4, [r0], -r4

# _ZNK6glitch7collada19CModularSkinnedMesh18getCurrentModuleIdEi
00646a48: ldr      r3, [r0, #0x24]
00646a4c: ldr      r0, [r3, r1, lsl #3]
00646a50: bx       lr

# _ZNK12VisualObject18GetModularModuleIdEiPKc
00474568: push     {r4, r5, r6, r7, r8, lr}
0047456c: ldr      r4, [pc, #0xd0]
00474570: ldr      r7, [pc, #0xd0]
00474574: ldr      r1, [pc, #0xd0]
00474578: add      r4, pc, r4
0047457c: ldr      r3, [r4, r7]
00474580: sub      sp, sp, #0x20
00474584: add      r5, sp, #4
00474588: ldr      r3, [r3]
0047458c: mov      r6, r2
00474590: add      r1, pc, r1
00474594: mov      r2, sp
00474598: mov      r8, r0
0047459c: mov      r0, r5
004745a0: str      r3, [sp, #0x1c]
004745a4: bl       #0x3140ec
004745a8: mov      r0, r6
004745ac: bl       #0x30de54
004745b0: mov      r1, r6
004745b4: add      r2, r6, r0
004745b8: mov      r0, r5
004745bc: bl       #0x310804
004745c0: ldr      r1, [pc, #0x88]
004745c4: mov      r0, r5
004745c8: add      r1, pc, r1
004745cc: add      r2, r1, #0xa
004745d0: bl       #0x310804
004745d4: ldr      r0, [r8, #0x2c]
004745d8: cmp      r0, #0
004745dc: mvneq    r6, #0
004745e0: beq      #0x4745f0
004745e4: ldr      r1, [sp, #0x18]
004745e8: bl       #0x649464
004745ec: mov      r6, r0
004745f0: ldr      r0, [sp, #0x18]
004745f4: cmp      r0, r5
004745f8: beq      #0x474618
004745fc: cmp      r0, #0
00474600: beq      #0x474618
00474604: ldr      r1, [sp, #4]
00474608: rsb      r1, r0, r1
0047460c: cmp      r1, #0x80
00474610: bhi      #0x474638
00474614: bl       #0x708f00
00474618: ldr      r3, [r4, r7]
0047461c: ldr      r2, [sp, #0x1c]
00474620: mov      r0, r6
00474624: ldr      r3, [r3]
00474628: cmp      r2, r3
0047462c: bne      #0x474640
00474630: add      sp, sp, #0x20
00474634: pop      {r4, r5, r6, r7, r8, pc}
00474638: bl       #0x310440
0047463c: b        #0x474618
00474640: bl       #0x30e310
00474644: subseq   r0, r2, r8, lsl r5
00474648: andeq    r4, r0, ip, lsr #1
0047464c: strdeq   r2, r3, [r5], #-0xc0
00474650: subeq    sb, r5, r0, lsr r1

# _ZN6glitch7collada19CModularSkinnedMeshC2ERKNS0_16CColladaDatabaseEPNS0_20SInstanceModularSkinEPNS0_14CRootSceneNodeEibPNS_5video12IVideoDriverE
0064928c: ldr      ip, [pc, #0x154]
00649290: push     {r4, r5, r6, r7, r8, lr}
00649294: ldr      lr, [pc, #0x150]
00649298: add      ip, pc, ip
0064929c: mov      r4, r0
006492a0: ldr      lr, [ip, lr]
006492a4: mov      r0, #0
006492a8: str      r0, [r4, #4]
006492ac: add      lr, lr, #8
006492b0: str      lr, [r4]
006492b4: ldr      r0, [r1]
006492b8: str      r0, [r4, #0xc]
006492bc: ldr      r1, [r1, #4]
006492c0: cmp      r0, #0
006492c4: str      r1, [r4, #0x10]
006492c8: ldrb     r7, [sp, #0x1c]
006492cc: beq      #0x6492e0
006492d0: ldr      r1, [r0, #4]
006492d4: cmp      r1, #0
006492d8: addne    r1, r1, #1
006492dc: strne    r1, [r0, #4]
006492e0: ldr      r5, [pc, #0x108]
006492e4: ldr      r1, [pc, #0x108]
006492e8: mov      lr, #0xbf000000
006492ec: ldr      r5, [ip, r5]
006492f0: ldr      r1, [ip, r1]
006492f4: add      lr, lr, #0x800000
006492f8: mov      r0, #0x3f800000
006492fc: add      r6, r1, #8
00649300: mov      ip, #1
00649304: mov      r1, #0
00649308: add      r5, r5, #4
0064930c: str      r5, [r4, #8]
00649310: str      r6, [r4]
00649314: str      r3, [r4, #0x20]
00649318: str      lr, [r4, #0x48]
0064931c: str      r0, [r4, #0x54]
00649320: strb     r1, [r4, #0x58]
00649324: str      r1, [r4, #0x14]
00649328: strb     ip, [r4, #0x18]
0064932c: str      r2, [r4, #0x1c]
00649330: str      r1, [r4, #0x24]
00649334: str      r1, [r4, #0x28]
00649338: str      r1, [r4, #0x2c]
0064933c: str      r1, [r4, #0x30]
00649340: str      r1, [r4, #0x34]
00649344: str      r1, [r4, #0x38]
00649348: str      r1, [r4, #0x3c]
0064934c: str      lr, [r4, #0x40]
00649350: str      lr, [r4, #0x44]
00649354: str      r0, [r4, #0x4c]
00649358: str      r0, [r4, #0x50]
0064935c: strb     ip, [r4, #0x59]
00649360: ldr      r3, [r2]
00649364: ldr      r6, [r2, #8]
00649368: ldr      r2, [sp, #0x18]
0064936c: add      r6, r6, r3
00649370: cmp      r2, r1
00649374: ble      #0x6493e0
00649378: mov      r0, r4
0064937c: mov      r1, r6
00649380: mov      r2, #0
00649384: bl       #0x648e18
00649388: cmp      r6, #0
0064938c: beq      #0x6493cc
00649390: mov      r5, #0
00649394: ldr      r3, [r4, #0x1c]
00649398: mov      r0, r4
0064939c: ldr      r3, [r3, #4]
006493a0: add      r3, r3, r5, lsl #4
006493a4: ldr      r1, [r3, #4]
006493a8: bl       #0x6474b8
006493ac: mov      r1, r5
006493b0: mov      r2, r0
006493b4: add      r5, r5, #1
006493b8: mov      r0, r4
006493bc: mov      r3, #0
006493c0: bl       #0x648fd0
006493c4: cmp      r6, r5
006493c8: bne      #0x649394
006493cc: mov      r1, r7
006493d0: mov      r0, r4
006493d4: bl       #0x6483c8
006493d8: mov      r0, r4
006493dc: pop      {r4, r5, r6, r7, r8, pc}
006493e0: streq    ip, [r4, #0x3c]
006493e4: b        #0x649378
006493e8: ldrshteq fp, [r4], -r8
006493ec: andeq    r0, r0, r0, asr #20
006493f0: strheq   r1, [r0], -r4
006493f4: andeq    r3, r0, r8, lsr sl

# _ZNK6glitch7collada19CModularSkinnedMesh13getModuleNameEii
006469e8: ldr      r3, [r0, #0x1c]
006469ec: ldr      r0, [r3]
006469f0: cmp      r1, r0
006469f4: bge      #0x646a0c
006469f8: ldr      r3, [r3, #4]
006469fc: add      r1, r3, r1, lsl #4
00646a00: ldr      r3, [r1, #8]
00646a04: cmp      r2, r3
00646a08: blt      #0x646a14
00646a0c: mov      r0, #0
00646a10: bx       lr
00646a14: ldr      r3, [r1, #0xc]
00646a18: add      r2, r3, r2, lsl #3
00646a1c: ldr      r3, [r2, #4]
00646a20: ldr      r0, [r3, #4]
00646a24: bx       lr

# _ZN6glitch7collada16CColladaDatabase14constructSceneEPNS_5video12IVideoDriverEPKcbPNS0_15CColladaFactoryE
0061bbd4: push     {r4, r5, r6, r7, r8, sl, lr}
0061bbd8: ldr      r4, [pc, #0xa0]
0061bbdc: subs     r7, r3, #0
0061bbe0: sub      sp, sp, #0xc
0061bbe4: add      r4, pc, r4
0061bbe8: mov      r8, r0
0061bbec: mov      sl, r2
0061bbf0: beq      #0x61bc74
0061bbf4: ldr      r5, [pc, #0x88]
0061bbf8: mov      r2, #0
0061bbfc: mov      r3, r2
0061bc00: ldr      r6, [r4, r5]
0061bc04: ldr      r0, [r6]
0061bc08: bl       #0x65ac5c
0061bc0c: cmp      r0, #0
0061bc10: moveq    r8, r0
0061bc14: beq      #0x61bc68
0061bc18: ldr      r3, [r6]
0061bc1c: mov      r2, #0
0061bc20: mov      r1, r8
0061bc24: ldrb     r6, [r3, #0x28]
0061bc28: strb     r2, [r3, #0x28]
0061bc2c: stm      sp, {r0, r7}
0061bc30: ldr      r3, [r0, #4]
0061bc34: mov      r7, sp
0061bc38: cmp      r3, r2
0061bc3c: addne    r3, r3, #1
0061bc40: strne    r3, [r0, #4]
0061bc44: mov      r2, sl
0061bc48: mov      r0, sp
0061bc4c: bl       #0x61babc
0061bc50: mov      r8, r0
0061bc54: mov      r0, sp
0061bc58: bl       #0x619474
0061bc5c: ldr      r3, [r4, r5]
0061bc60: ldr      r3, [r3]
0061bc64: strb     r6, [r3, #0x28]
0061bc68: mov      r0, r8
0061bc6c: add      sp, sp, #0xc
0061bc70: pop      {r4, r5, r6, r7, r8, sl, pc}
0061bc74: ldr      r3, [pc, #0xc]
0061bc78: ldr      r7, [r4, r3]
0061bc7c: b        #0x61bbf4
0061bc80: eorseq   r8, r7, ip, lsr #29
0061bc84: andeq    r4, r0, r8, asr #8
0061bc88: andeq    r4, r0, r0, lsl r7

# _ZNK6glitch7collada19CModularSkinnedMesh22getCategoryModuleCountEi
00646a34: ldr      r3, [r0, #0x1c]
00646a38: ldr      r3, [r3, #4]
00646a3c: add      r3, r3, r1, lsl #4
00646a40: ldr      r0, [r3, #8]
00646a44: bx       lr

# _ZN6glitch7collada19CModularSkinnedMesh17setCategoryModuleEiib
00648fd0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00648fd4: ldr      r5, [r0, #0x24]
00648fd8: mov      r4, r1
00648fdc: ldr      sl, [pc, #0xf8]
00648fe0: ldr      r1, [r5, r1, lsl #3]
00648fe4: sub      sp, sp, #0x10
00648fe8: add      sl, pc, sl
00648fec: cmp      r1, r2
00648ff0: mov      r6, r0
00648ff4: mov      r7, r2
00648ff8: mov      sb, r3
00648ffc: add      r8, r5, r4, lsl #3
00649000: beq      #0x6490bc
00649004: ldr      r0, [r8, #4]
00649008: cmp      r0, #0
0064900c: beq      #0x649024
00649010: mov      r3, #0
00649014: str      r3, [r8, #4]
00649018: bl       #0x31d584
0064901c: mvn      r3, #0
00649020: str      r3, [r5, r4, lsl #3]
00649024: cmn      r7, #1
00649028: beq      #0x6490b4
0064902c: ldr      r2, [pc, #0xac]
00649030: ldr      r3, [r6, #0x1c]
00649034: ldr      ip, [r6, #0x20]
00649038: ldr      r2, [sl, r2]
0064903c: ldr      r3, [r3, #4]
00649040: add      r0, sp, #0xc
00649044: ldr      r2, [r2]
00649048: add      r3, r3, r4, lsl #4
0064904c: ldr      r3, [r3, #0xc]
00649050: ldr      r2, [r2, #0x20]
00649054: add      r1, r6, #0xc
00649058: add      r3, r3, r7, lsl #3
0064905c: ldr      r3, [r3, #4]
00649060: ldr      r2, [r2, #0x10]
00649064: str      ip, [sp]
00649068: mov      ip, #1
0064906c: str      ip, [sp, #4]
00649070: bl       #0x61ace8
00649074: ldr      r3, [sp, #0xc]
00649078: cmp      r3, #0
0064907c: beq      #0x6490b4
00649080: ldr      r2, [r3, #4]
00649084: add      r2, r2, #1
00649088: str      r2, [r3, #4]
0064908c: ldr      r0, [r8, #4]
00649090: str      r3, [r8, #4]
00649094: cmp      r0, #0
00649098: beq      #0x6490a0
0064909c: bl       #0x31d584
006490a0: str      r7, [r5, r4, lsl #3]
006490a4: ldr      r0, [sp, #0xc]
006490a8: cmp      r0, #0
006490ac: beq      #0x6490b4
006490b0: bl       #0x31d584
006490b4: cmp      sb, #0
006490b8: bne      #0x6490c4
006490bc: add      sp, sp, #0x10
006490c0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
006490c4: ldr      r1, [r6, #0x14]
006490c8: mov      r0, r6
006490cc: eor      r1, r1, #1
006490d0: and      r1, r1, #1
006490d4: bl       #0x6483c8
006490d8: b        #0x6490bc
006490dc: eorseq   fp, r4, r8, lsr #21
006490e0: andeq    r4, r0, r8, asr #8

# _ZNK6glitch7collada19CModularSkinnedMesh13getCategoryIdEPKc
00647538: push     {r4, r5, r6, r7, r8, lr}
0064753c: ldr      r3, [r0, #0x1c]
00647540: mov      r6, r1
00647544: ldr      r5, [r3]
00647548: cmp      r5, #0
0064754c: ble      #0x647584
00647550: ldr      r7, [r3, #4]
00647554: mov      r4, #0
00647558: b        #0x647568
0064755c: add      r4, r4, #1
00647560: cmp      r4, r5
00647564: beq      #0x647584
00647568: ldr      r0, [r7, r4, lsl #4]
0064756c: mov      r1, r6
00647570: bl       #0x30e31c
00647574: cmp      r0, #0
00647578: bne      #0x64755c
0064757c: mov      r0, r4
00647580: pop      {r4, r5, r6, r7, r8, pc}
00647584: mvn      r4, #0
00647588: mov      r0, r4
0064758c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN12VisualObject13SetWeaponSkinEPKcii
00473cd8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00473cdc: ldr      r4, [pc, #0x1a0]
00473ce0: ldr      r8, [pc, #0x1a0]
00473ce4: subs     r7, r1, #0
00473ce8: add      r4, pc, r4
00473cec: ldr      r1, [r4, r8]
00473cf0: mov      sb, r2
00473cf4: sub      sp, sp, #0x24
00473cf8: ldr      r2, [r1]
00473cfc: mov      r5, r0
00473d00: mov      fp, r3
00473d04: str      r2, [sp, #0x1c]
00473d08: beq      #0x473e34
00473d0c: ldr      r1, [pc, #0x178]
00473d10: add      r6, sp, #4
00473d14: mov      r2, sp
00473d18: add      r1, pc, r1
00473d1c: mov      r0, r6
00473d20: bl       #0x3140ec
00473d24: mov      r0, r7
00473d28: bl       #0x30de54
00473d2c: mov      r1, r7
00473d30: add      r2, r7, r0
00473d34: mov      r0, r6
00473d38: bl       #0x310804
00473d3c: ldr      r1, [pc, #0x14c]
00473d40: ldr      sl, [pc, #0x14c]
00473d44: mov      r0, r6
00473d48: add      r1, pc, r1
00473d4c: add      r2, r1, #5
00473d50: bl       #0x310804
00473d54: ldr      r3, [r4, sl]
00473d58: ldr      r1, [sp, #0x18]
00473d5c: mov      r2, #1
00473d60: ldr      r0, [r3, #0x10]
00473d64: ldr      r3, [pc, #0x12c]
00473d68: ldr      r0, [r0, #0x10]
00473d6c: ldr      r3, [r4, r3]
00473d70: bl       #0x61bbd4
00473d74: mov      r7, r0
00473d78: ldr      r0, [sp, #0x18]
00473d7c: cmp      r0, r6
00473d80: beq      #0x473da0
00473d84: cmp      r0, #0
00473d88: beq      #0x473da0
00473d8c: ldr      r1, [sp, #4]
00473d90: rsb      r1, r0, r1
00473d94: cmp      r1, #0x80
00473d98: bhi      #0x473e78
00473d9c: bl       #0x708f00
00473da0: cmp      sb, #1
00473da4: beq      #0x473e40
00473da8: ldr      r3, [r5, #0x34]
00473dac: cmp      r3, #0
00473db0: beq      #0x473dd8
00473db4: mov      r0, r3
00473db8: ldr      r3, [r3]
00473dbc: mov      lr, pc
00473dc0: ldr      pc, [r3, #0x68]
00473dc4: ldr      r3, [r5, #0x34]
00473dc8: ldr      r2, [r3]
00473dcc: ldr      r0, [r2, #-0xc]
00473dd0: add      r0, r3, r0
00473dd4: bl       #0x31d584
00473dd8: str      r7, [r5, #0x34]
00473ddc: ldr      r3, [r4, sl]
00473de0: ldr      r2, [pc, #0xb4]
00473de4: ldr      r1, [r5, #8]
00473de8: ldr      r3, [r3, #0x10]
00473dec: add      r2, pc, r2
00473df0: ldr      r2, [r2, fp, lsl #2]
00473df4: ldr      r0, [r3, #0x1c]
00473df8: mov      r3, #0
00473dfc: bl       #0x35a0e4
00473e00: subs     r3, r0, #0
00473e04: beq      #0x473e18
00473e08: ldr      r3, [r3]
00473e0c: mov      r1, r7
00473e10: mov      lr, pc
00473e14: ldr      pc, [r3, #0x5c]
00473e18: ldr      r3, [r4, r8]
00473e1c: ldr      r2, [sp, #0x1c]
00473e20: ldr      r3, [r3]
00473e24: cmp      r2, r3
00473e28: bne      #0x473e80
00473e2c: add      sp, sp, #0x24
00473e30: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00473e34: cmp      sb, #1
00473e38: ldr      sl, [pc, #0x54]
00473e3c: bne      #0x473da8
00473e40: ldr      r3, [r5, #0x30]
00473e44: cmp      r3, #0
00473e48: beq      #0x473e70
00473e4c: mov      r0, r3
00473e50: ldr      r3, [r3]
00473e54: mov      lr, pc
00473e58: ldr      pc, [r3, #0x68]
00473e5c: ldr      r3, [r5, #0x30]
00473e60: ldr      r2, [r3]
00473e64: ldr      r0, [r2, #-0xc]
00473e68: add      r0, r3, r0
00473e6c: bl       #0x31d584
00473e70: str      r7, [r5, #0x30]
00473e74: b        #0x473ddc
00473e78: bl       #0x310440
00473e7c: b        #0x473da0
00473e80: bl       #0x30e310
00473e84: subseq   r0, r2, r8, lsr #27
00473e88: andeq    r4, r0, ip, lsr #1
00473e8c: subeq    sb, r5, r0, lsr #19
00473e90: subeq    r5, r5, r0, lsr #23
00473e94: strdeq   r3, r4, [r0], -r4
00473e98: andeq    r0, r0, ip, lsr #26
00473e9c: subeq    r2, lr, r4, ror #25

# _ZNK12VisualObject20GetModularCategoryIdEPKc
00470e5c: ldr      r0, [r0, #0x2c]
00470e60: cmp      r0, #0
00470e64: beq      #0x470e6c
00470e68: b        #0x6494e0
00470e6c: mvn      r0, #0
00470e70: bx       lr

# _ZN6glitch7collada19CModularSkinnedMesh12updateBufferEb
006483c8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006483cc: ldr      r2, [pc, #0x9c4]
006483d0: ldr      r3, [pc, #0x9c4]
006483d4: sub      sp, sp, #0x13c
006483d8: add      r2, pc, r2
006483dc: str      r2, [sp, #0x38]
006483e0: str      r3, [sp, #0x40]
006483e4: ldr      ip, [r2, r3]
006483e8: ldr      r5, [r0, #0x28]
006483ec: mov      r4, r0
006483f0: ldr      r0, [r0, #0x24]
006483f4: ldr      r3, [r4, #0x30]
006483f8: ldr      r2, [r4, #0x34]
006483fc: ldr      ip, [ip]
00648400: rsb      r0, r0, r5
00648404: asr      r0, r0, #3
00648408: cmp      r3, r2
0064840c: str      ip, [sp, #0x134]
00648410: str      r1, [sp, #0x3c]
00648414: str      r0, [sp, #0x1c]
00648418: add      r5, r4, #0x30
0064841c: beq      #0x648430
00648420: mov      r1, r3
00648424: mov      r0, r5
00648428: add      r3, sp, #0xb0
0064842c: bl       #0x647dfc
00648430: ldr      ip, [sp, #0x1c]
00648434: mov      r3, #0
00648438: str      r3, [sp, #0xac]
0064843c: cmp      ip, #0
00648440: beq      #0x648534
00648444: add      r0, sp, #0x8c
00648448: add      r1, sp, #0xac
0064844c: add      sl, sp, #0xa8
00648450: add      r7, sp, #0x4c
00648454: str      r0, [sp, #0x18]
00648458: str      r1, [sp, #0x20]
0064845c: ldr      r2, [r4, #0x24]
00648460: add      r3, r2, r3, lsl #3
00648464: ldr      r8, [r3, #4]
00648468: cmp      r8, #0
0064846c: beq      #0x64851c
00648470: mov      r0, sl
00648474: mov      r1, r8
00648478: mov      r2, #0
0064847c: ldr      r3, [r8]
00648480: mov      lr, pc
00648484: ldr      pc, [r3, #0x18]
00648488: ldr      r3, [r8]
0064848c: mov      r0, r8
00648490: mov      lr, pc
00648494: ldr      pc, [r3, #0x50]
00648498: cmp      r0, #0
0064849c: beq      #0x648a68
006484a0: ldr      r3, [r4, #0x30]
006484a4: ldr      fp, [r4, #0x34]
006484a8: rsb      fp, r3, fp
006484ac: asrs     fp, fp, #5
006484b0: beq      #0x648a68
006484b4: mov      r6, #0
006484b8: b        #0x6484cc
006484bc: add      r6, r6, #1
006484c0: cmp      r6, fp
006484c4: beq      #0x648a68
006484c8: ldr      r3, [r4, #0x30]
006484cc: lsl      sb, r6, #5
006484d0: add      r3, r3, sb
006484d4: ldr      r0, [r3, #4]
006484d8: ldr      r1, [sp, #0xa8]
006484dc: bl       #0x3537b0
006484e0: cmp      r0, #0
006484e4: beq      #0x6484bc
006484e8: ldr      r3, [r4, #0x30]
006484ec: add      sb, r3, sb
006484f0: ldr      r1, [sb, #0x10]
006484f4: ldr      r3, [sb, #0x14]
006484f8: cmp      r1, r3
006484fc: beq      #0x648b78
00648500: ldr      r3, [sp, #0xac]
00648504: str      r3, [r1]
00648508: ldr      r3, [sb, #0x10]
0064850c: add      r3, r3, #4
00648510: str      r3, [sb, #0x10]
00648514: mov      r0, sl
00648518: bl       #0x310be8
0064851c: ldr      r3, [sp, #0xac]
00648520: ldr      r2, [sp, #0x1c]
00648524: add      r3, r3, #1
00648528: cmp      r3, r2
0064852c: str      r3, [sp, #0xac]
00648530: blo      #0x64845c
00648534: ldr      r3, [r4, #0x34]
00648538: ldr      sl, [r4, #0x30]
0064853c: str      r3, [sp, #0x2c]
00648540: mov      r3, #0
00648544: strb     r3, [r4, #0x58]
00648548: ldr      ip, [sp, #0x2c]
0064854c: cmp      sl, ip
00648550: beq      #0x648a3c
00648554: ldr      r0, [pc, #0x844]
00648558: mvn      r1, #0x20000
0064855c: sub      r1, r1, #1
00648560: add      r2, sp, #0x98
00648564: add      r3, sp, #0x94
00648568: add      ip, sp, #0x90
0064856c: str      r0, [sp, #0x44]
00648570: str      r1, [sp, #0x18]
00648574: str      r2, [sp, #0x1c]
00648578: str      r3, [sp, #0x20]
0064857c: str      ip, [sp, #0x24]
00648580: mov      r8, r4
00648584: b        #0x648598
00648588: ldr      r3, [sp, #0x2c]
0064858c: add      sl, sl, #0x20
00648590: cmp      sl, r3
00648594: beq      #0x648a3c
00648598: ldrb     r3, [sl, #0x1c]
0064859c: cmp      r3, #0
006485a0: beq      #0x648588
006485a4: mov      r4, #1
006485a8: strb     r4, [r8, #0x58]
006485ac: ldr      r3, [sl, #4]
006485b0: mov      r0, r3
006485b4: ldr      r5, [r3, #4]
006485b8: bl       #0x5c5d34
006485bc: ldr      r3, [r5, #0x18]
006485c0: mov      r2, #0xc
006485c4: mla      r3, r2, r0, r3
006485c8: add      r0, sp, #0xa8
006485cc: ldr      r3, [r3, #8]
006485d0: ldr      r3, [r3, #0x20]
006485d4: ldr      r1, [r3, #0x38]
006485d8: bl       #0x5a135c
006485dc: ldr      r3, [sl, #4]
006485e0: mov      lr, #0
006485e4: mov      r2, lr
006485e8: ldr      r1, [r3, #4]
006485ec: add      r0, sp, #0xa4
006485f0: mov      r3, #4
006485f4: ldr      ip, [r1, #4]
006485f8: mov      r1, ip
006485fc: ldr      ip, [ip]
00648600: str      r4, [sp, #8]
00648604: str      lr, [sp]
00648608: str      lr, [sp, #4]
0064860c: mov      lr, pc
00648610: ldr      pc, [ip, #0x78]
00648614: ldr      r3, [sp, #0xa4]
00648618: cmp      r3, #0
0064861c: ldrne    r2, [r3, #4]
00648620: addne    r2, r2, r4
00648624: strne    r2, [r3, #4]
00648628: ldr      r0, [sl, #0x18]
0064862c: str      r3, [sl, #0x18]
00648630: cmp      r0, #0
00648634: beq      #0x64863c
00648638: bl       #0x31d584
0064863c: ldr      r0, [sp, #0xa4]
00648640: cmp      r0, #0
00648644: beq      #0x64864c
00648648: bl       #0x31d584
0064864c: ldr      r0, [sp, #0x3c]
00648650: add      r1, sl, #0x18
00648654: cmp      r0, #0
00648658: ldr      r0, [sp, #0xa8]
0064865c: beq      #0x648b24
00648660: mvn      r2, #0x20000
00648664: sub      r2, r2, #1
00648668: bl       #0x5a15c0
0064866c: str      r0, [sp, #0x28]
00648670: ldr      r0, [sp, #0xa8]
00648674: mov      r3, #0
00648678: mov      ip, #6
0064867c: add      r1, r0, #0x14
00648680: strh     r3, [sp, #0x8a]
00648684: str      r3, [sp, #0x7c]
00648688: str      r3, [sp, #0x80]
0064868c: add      r2, sp, #0x7c
00648690: mov      r3, #3
00648694: str      ip, [sp, #0x84]
00648698: strh     r3, [sp, #0x88]
0064869c: bl       #0x647e78
006486a0: ldr      r0, [sp, #0x7c]
006486a4: cmp      r0, #0
006486a8: beq      #0x6486b0
006486ac: bl       #0x31d584
006486b0: ldr      r2, [sp, #0xa8]
006486b4: ldr      r3, [r2, #4]
006486b8: tst      r3, #0x20000
006486bc: beq      #0x648708
006486c0: ldrb     r1, [r2, #0xc]
006486c4: mov      ip, #6
006486c8: mov      r3, #0
006486cc: add      r1, r2, r1, lsl #4
006486d0: mov      r0, r2
006486d4: str      ip, [sp, #0x74]
006486d8: add      r1, r1, #0x24
006486dc: mov      ip, #3
006486e0: add      r2, sp, #0x6c
006486e4: strh     r3, [sp, #0x7a]
006486e8: str      r3, [sp, #0x6c]
006486ec: str      r3, [sp, #0x70]
006486f0: strh     ip, [sp, #0x78]
006486f4: bl       #0x647e78
006486f8: ldr      r0, [sp, #0x6c]
006486fc: cmp      r0, #0
00648700: beq      #0x648708
00648704: bl       #0x31d584
00648708: ldr      r5, [sl, #0xc]
0064870c: ldr      r6, [sl, #0x10]
00648710: cmp      r5, r6
00648714: moveq    r7, #0
00648718: moveq    sb, r7
0064871c: beq      #0x64879c
00648720: mov      r7, #0
00648724: mov      sb, r7
00648728: add      fp, sp, #0xa0
0064872c: ldr      r2, [r5]
00648730: ldr      r3, [r8, #0x24]
00648734: add      r3, r3, r2, lsl #3
00648738: ldr      r4, [r3, #4]
0064873c: cmp      r4, #0
00648740: beq      #0x648790
00648744: ldr      r3, [r4]
00648748: mov      r0, r4
0064874c: mov      lr, pc
00648750: ldr      pc, [r3, #0x50]
00648754: cmp      r0, #0
00648758: beq      #0x648790
0064875c: mov      r1, r4
00648760: mov      r2, #0
00648764: mov      r0, fp
00648768: ldr      r3, [r4]
0064876c: mov      lr, pc
00648770: ldr      pc, [r3, #0x14]
00648774: ldr      r0, [sp, #0xa0]
00648778: ldr      r3, [r0, #0x20]
0064877c: add      sb, sb, r3
00648780: bl       #0x31d584
00648784: mov      r0, r4
00648788: bl       #0x64834c
0064878c: add      r7, r7, r0
00648790: add      r5, r5, #4
00648794: cmp      r5, r6
00648798: bne      #0x64872c
0064879c: ldr      r3, [sp, #0xa8]
006487a0: str      r7, [r3, #8]
006487a4: ldr      lr, [sl]
006487a8: cmp      lr, #0
006487ac: beq      #0x648c58
006487b0: ldr      r3, [r8, #0x3c]
006487b4: ldr      r0, [sp, #0x28]
006487b8: lsl      r2, sb, #1
006487bc: cmp      r3, #1
006487c0: mul      r1, r0, r7
006487c4: beq      #0x648b48
006487c8: cmp      r3, #2
006487cc: beq      #0x648b88
006487d0: cmp      r3, #0
006487d4: beq      #0x648b48
006487d8: ldr      r3, [sl]
006487dc: str      r3, [sp, #0x28]
006487e0: ldr      r0, [r3, #0x18]
006487e4: mov      r1, #4
006487e8: bl       #0x5a19f0
006487ec: ldr      ip, [sp, #0x28]
006487f0: ldr      r7, [sl, #0xc]
006487f4: ldr      r2, [sl, #0x10]
006487f8: ldr      r3, [ip, #0x1c]
006487fc: cmp      r7, r2
00648800: add      r3, r0, r3
00648804: str      r3, [sp, #0x30]
00648808: beq      #0x6489c8
0064880c: mov      sb, #0
00648810: str      sl, [sp, #0x34]
00648814: mov      r4, r3
00648818: mov      fp, sb
0064881c: mov      sl, r2
00648820: ldr      r2, [r7]
00648824: ldr      r3, [r8, #0x24]
00648828: add      r3, r3, r2, lsl #3
0064882c: ldr      r6, [r3, #4]
00648830: cmp      r6, #0
00648834: beq      #0x6489b8
00648838: ldr      r3, [r6]
0064883c: ldr      r0, [sp, #0x1c]
00648840: mov      r1, r6
00648844: mov      r2, #0
00648848: mov      lr, pc
0064884c: ldr      pc, [r3, #0x14]
00648850: ldr      r5, [sp, #0x98]
00648854: cmp      r5, #0
00648858: beq      #0x648864
0064885c: mov      r0, r5
00648860: bl       #0x31d584
00648864: ldr      r5, [r5, #0x14]
00648868: mov      r2, #0
0064886c: cmp      r5, #0
00648870: ldrne    r3, [r5]
00648874: streq    r5, [sp, #0x94]
00648878: ldreq    r0, [sp, #0xa8]
0064887c: addne    r3, r3, #1
00648880: strne    r3, [r5]
00648884: strne    r5, [sp, #0x94]
00648888: ldrne    r3, [r5]
0064888c: ldrne    r0, [sp, #0xa8]
00648890: addne    r3, r3, #1
00648894: strne    r3, [r5]
00648898: ldr      ip, [sp, #0x18]
0064889c: ldr      r3, [r5, #8]
006488a0: ldr      r1, [sp, #0x20]
006488a4: stm      sp, {fp, ip}
006488a8: bl       #0x5a0fe4
006488ac: ldr      r3, [sp, #0x94]
006488b0: cmp      r3, #0
006488b4: beq      #0x6488e4
006488b8: ldr      r2, [r3]
006488bc: sub      r2, r2, #1
006488c0: cmp      r2, #0
006488c4: str      r2, [r3]
006488c8: bne      #0x6488e4
006488cc: mov      r0, r3
006488d0: str      r3, [sp, #0x14]
006488d4: bl       #0x5a0a1c
006488d8: ldr      r3, [sp, #0x14]
006488dc: mov      r0, r3
006488e0: bl       #0x30e2b0
006488e4: ldr      ip, [r5, #8]
006488e8: mov      r1, r6
006488ec: ldr      r3, [r6]
006488f0: ldr      r0, [sp, #0x24]
006488f4: mov      r2, #0
006488f8: add      fp, fp, ip
006488fc: mov      lr, pc
00648900: ldr      pc, [r3, #0x14]
00648904: ldr      r6, [sp, #0x90]
00648908: cmp      r6, #0
0064890c: beq      #0x648918
00648910: mov      r0, r6
00648914: bl       #0x31d584
00648918: ldr      r0, [r6, #0x18]
0064891c: mov      r1, #1
00648920: bl       #0x5a1adc
00648924: ldr      r2, [r6, #0x1c]
00648928: ldr      r3, [r6, #0x20]
0064892c: add      r0, r0, r2
00648930: add      r3, r0, r3, lsl #1
00648934: cmp      r3, r0
00648938: beq      #0x648970
0064893c: add      r2, r0, #2
00648940: rsb      r3, r2, r3
00648944: bic      ip, r3, #1
00648948: add      ip, ip, #2
0064894c: uxth     r1, sb
00648950: mov      r3, #0
00648954: ldrh     r2, [r0, r3]
00648958: add      r2, r1, r2
0064895c: strh     r2, [r4, r3]
00648960: add      r3, r3, #2
00648964: cmp      r3, ip
00648968: bne      #0x648954
0064896c: add      r4, r4, r3
00648970: cmp      r0, #0
00648974: beq      #0x64899c
00648978: ldr      r6, [r6, #0x18]
0064897c: ldrb     r3, [r6, #0x13]
00648980: and      r2, r3, #0x1f
00648984: cmp      r2, #1
00648988: bls      #0x648b0c
0064898c: sub      r2, r2, #1
00648990: bic      r3, r3, #0x1f
00648994: orr      r3, r2, r3
00648998: strb     r3, [r6, #0x13]
0064899c: ldr      r3, [r5]
006489a0: ldr      r2, [r5, #8]
006489a4: sub      r3, r3, #1
006489a8: cmp      r3, #0
006489ac: add      sb, sb, r2
006489b0: str      r3, [r5]
006489b4: beq      #0x648af8
006489b8: add      r7, r7, #4
006489bc: cmp      r7, sl
006489c0: bne      #0x648820
006489c4: ldr      sl, [sp, #0x34]
006489c8: ldr      r1, [sp, #0x30]
006489cc: cmp      r1, #0
006489d0: beq      #0x6489fc
006489d4: ldr      r2, [sp, #0x28]
006489d8: ldr      r4, [r2, #0x18]
006489dc: ldrb     r3, [r4, #0x13]
006489e0: and      r2, r3, #0x1f
006489e4: cmp      r2, #1
006489e8: bls      #0x648c10
006489ec: sub      r2, r2, #1
006489f0: bic      r3, r3, #0x1f
006489f4: orr      r3, r2, r3
006489f8: strb     r3, [r4, #0x13]
006489fc: ldr      r4, [sp, #0xa8]
00648a00: cmp      r4, #0
00648a04: beq      #0x648588
00648a08: ldr      r3, [r4]
00648a0c: sub      r3, r3, #1
00648a10: cmp      r3, #0
00648a14: str      r3, [r4]
00648a18: bne      #0x648588
00648a1c: mov      r0, r4
00648a20: bl       #0x5a0a1c
00648a24: mov      r0, r4
00648a28: bl       #0x30e2b0
00648a2c: ldr      r3, [sp, #0x2c]
00648a30: add      sl, sl, #0x20
00648a34: cmp      sl, r3
00648a38: bne      #0x648598
00648a3c: mov      r5, #0
00648a40: ldr      r0, [sp, #0x38]
00648a44: ldr      ip, [sp, #0x40]
00648a48: ldr      r2, [sp, #0x134]
00648a4c: ldr      r3, [r0, ip]
00648a50: mov      r0, r5
00648a54: ldr      r3, [r3]
00648a58: cmp      r2, r3
00648a5c: bne      #0x648d8c
00648a60: add      sp, sp, #0x13c
00648a64: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00648a68: mov      r0, r7
00648a6c: bl       #0x646994
00648a70: mov      r1, r7
00648a74: mov      r0, r5
00648a78: bl       #0x647bc8
00648a7c: mov      r0, r7
00648a80: bl       #0x647a68
00648a84: ldr      r0, [r4, #0x34]
00648a88: sub      r6, r0, #0x20
00648a8c: ldr      r1, [r6, #0x10]
00648a90: ldr      r3, [r6, #0x14]
00648a94: cmp      r1, r3
00648a98: beq      #0x648b68
00648a9c: ldr      r3, [sp, #0xac]
00648aa0: str      r3, [r1]
00648aa4: ldr      r3, [r6, #0x10]
00648aa8: add      r3, r3, #4
00648aac: str      r3, [r6, #0x10]
00648ab0: ldr      r2, [sp, #0xa8]
00648ab4: str      r2, [sp, #0x8c]
00648ab8: cmp      r2, #0
00648abc: ldrne    r3, [r2]
00648ac0: addne    r3, r3, #1
00648ac4: strne    r3, [r2]
00648ac8: ldrne    r2, [sp, #0x8c]
00648acc: ldr      r3, [r6, #4]
00648ad0: ldr      r0, [sp, #0x18]
00648ad4: str      r3, [sp, #0x8c]
00648ad8: str      r2, [r6, #4]
00648adc: bl       #0x310be8
00648ae0: mov      r0, r8
00648ae4: ldr      r3, [r8]
00648ae8: mov      lr, pc
00648aec: ldr      pc, [r3, #0x50]
00648af0: strb     r0, [r6, #0x1c]
00648af4: b        #0x648514
00648af8: mov      r0, r5
00648afc: bl       #0x5a0a1c
00648b00: mov      r0, r5
00648b04: bl       #0x30e2b0
00648b08: b        #0x6489b8
00648b0c: ldrb     r3, [r6, #0x12]
00648b10: tst      r3, #0x20
00648b14: bne      #0x648b34
00648b18: mov      r0, #0
00648b1c: strb     r0, [r6, #0x13]
00648b20: b        #0x64899c
00648b24: mvn      r2, #0
00648b28: bl       #0x5a15c0
00648b2c: str      r0, [sp, #0x28]
00648b30: b        #0x648708
00648b34: ldr      r3, [r6]
00648b38: mov      r0, r6
00648b3c: mov      lr, pc
00648b40: ldr      pc, [r3, #0x18]
00648b44: b        #0x648b18
00648b48: mov      r0, sl
00648b4c: bl       #0x647970
00648b50: cmp      r0, #0
00648b54: bne      #0x648d90
00648b58: ldr      r2, [sl]
00648b5c: str      r2, [sp, #0x28]
00648b60: ldr      r0, [r2, #0x18]
00648b64: b        #0x6487e4
00648b68: sub      r0, r0, #0x14
00648b6c: ldr      r2, [sp, #0x20]
00648b70: bl       #0x6478c0
00648b74: b        #0x648ab0
00648b78: add      r0, sb, #0xc
00648b7c: ldr      r2, [sp, #0x20]
00648b80: bl       #0x6478c0
00648b84: b        #0x648514
00648b88: ldr      r3, [sl, #0x18]
00648b8c: ldr      r3, [r3, #0xc]
00648b90: cmp      r1, r3
00648b94: bhi      #0x648c28
00648b98: ldr      r1, [sl]
00648b9c: str      r1, [sp, #0x28]
00648ba0: ldr      r0, [r1, #0x18]
00648ba4: ldr      r3, [r0, #0xc]
00648ba8: cmp      r2, r3
00648bac: bls      #0x6487e4
00648bb0: ldr      r1, [pc, #0x1ec]
00648bb4: add      r4, sp, #0xb4
00648bb8: mov      r0, r4
00648bbc: add      r1, pc, r1
00648bc0: bl       #0x30eae4
00648bc4: ldr      r0, [pc, #0x1dc]
00648bc8: mov      r1, r4
00648bcc: mov      r2, #3
00648bd0: add      r0, pc, r0
00648bd4: bl       #0x60ace8
00648bd8: mov      r5, #2
00648bdc: ldr      r4, [sp, #0xa8]
00648be0: cmp      r4, #0
00648be4: beq      #0x648a40
00648be8: ldr      r3, [r4]
00648bec: sub      r3, r3, #1
00648bf0: cmp      r3, #0
00648bf4: str      r3, [r4]
00648bf8: bne      #0x648a40
00648bfc: mov      r0, r4
00648c00: bl       #0x5a0a1c
00648c04: mov      r0, r4
00648c08: bl       #0x30e2b0
00648c0c: b        #0x648a40
00648c10: ldrb     r3, [r4, #0x12]
00648c14: tst      r3, #0x20
00648c18: bne      #0x648d78
00648c1c: mov      r3, #0
00648c20: strb     r3, [r4, #0x13]
00648c24: b        #0x6489fc
00648c28: mov      r2, r1
00648c2c: ldr      r1, [pc, #0x178]
00648c30: add      r4, sp, #0xb4
00648c34: mov      r0, r4
00648c38: add      r1, pc, r1
00648c3c: bl       #0x30eae4
00648c40: ldr      r0, [pc, #0x168]
00648c44: mov      r1, r4
00648c48: mov      r2, #3
00648c4c: add      r0, pc, r0
00648c50: bl       #0x60ace8
00648c54: b        #0x648bd8
00648c58: ldr      r3, [sl, #4]
00648c5c: mov      r4, #1
00648c60: mov      r2, r4
00648c64: ldr      r1, [r3, #4]
00648c68: add      r0, sp, #0x9c
00648c6c: mov      r3, #4
00648c70: ldr      ip, [r1, #4]
00648c74: mov      r1, ip
00648c78: ldr      ip, [ip]
00648c7c: str      lr, [sp, #4]
00648c80: str      lr, [sp]
00648c84: str      r4, [sp, #8]
00648c88: mov      lr, pc
00648c8c: ldr      pc, [ip, #0x78]
00648c90: ldr      r4, [sp, #0x9c]
00648c94: mov      r1, #0
00648c98: mov      r0, #0x38
00648c9c: cmp      r4, #0
00648ca0: ldrne    r3, [r4, #4]
00648ca4: addne    r3, r3, #1
00648ca8: strne    r3, [r4, #4]
00648cac: bl       #0x5341ac
00648cb0: ldr      r2, [sp, #0x38]
00648cb4: mov      r3, r0
00648cb8: ldr      r0, [sp, #0x44]
00648cbc: mov      ip, #6
00648cc0: ldr      r1, [r2, r0]
00648cc4: mov      r2, #0
00648cc8: str      r2, [r3, #4]
00648ccc: add      r1, r1, #8
00648cd0: str      r1, [r3]
00648cd4: str      r2, [r3, #0x10]
00648cd8: str      r2, [r3, #8]
00648cdc: str      r2, [r3, #0xc]
00648ce0: ldr      r2, [sp, #0xa8]
00648ce4: mov      r0, #1
00648ce8: str      r2, [r3, #0x14]
00648cec: cmp      r2, #0
00648cf0: ldrne    r1, [r2]
00648cf4: addne    r1, r1, #1
00648cf8: strne    r1, [r2]
00648cfc: cmp      r4, #0
00648d00: str      r4, [r3, #0x18]
00648d04: ldrne    r2, [r4, #4]
00648d08: addne    r2, r2, #1
00648d0c: strne    r2, [r4, #4]
00648d10: ldr      r1, [r3, #4]
00648d14: mov      r2, #0
00648d18: str      r2, [r3, #0x30]
00648d1c: add      r1, r1, #1
00648d20: strb     r0, [r3, #0x34]
00648d24: str      r1, [r3, #4]
00648d28: str      r2, [r3, #0x1c]
00648d2c: str      sb, [r3, #0x20]
00648d30: str      r2, [r3, #0x24]
00648d34: str      r7, [r3, #0x28]
00648d38: strh     r0, [r3, #0x2c]
00648d3c: strh     ip, [r3, #0x2e]
00648d40: ldr      r0, [sl]
00648d44: str      r3, [sl]
00648d48: cmp      r0, r2
00648d4c: beq      #0x648d54
00648d50: bl       #0x31d584
00648d54: cmp      r4, #0
00648d58: beq      #0x648d64
00648d5c: mov      r0, r4
00648d60: bl       #0x31d584
00648d64: ldr      r0, [sp, #0x9c]
00648d68: cmp      r0, #0
00648d6c: beq      #0x6487b0
00648d70: bl       #0x31d584
00648d74: b        #0x6487b0
00648d78: ldr      r3, [r4]
00648d7c: mov      r0, r4
00648d80: mov      lr, pc
00648d84: ldr      pc, [r3, #0x18]
00648d88: b        #0x648c1c
00648d8c: bl       #0x30e310
00648d90: mov      r5, r0
00648d94: b        #0x648bdc
00648d98: ldrhteq  ip, [r4], -r8
00648d9c: andeq    r4, r0, ip, lsr #1
00648da0: andeq    r0, r0, r4, asr ip
00648da4: eoreq    ip, sb, r4, lsr #20
00648da8: eoreq    ip, sb, r0, lsl #20
00648dac: eoreq    ip, sb, r8, lsr #19
00648db0: eoreq    ip, sb, r4, lsl #19

# _ZN6glitch7collada19CModularSkinnedMesh4skinEj
00647fcc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00647fd0: ldr      r3, [r0, #0x30]
00647fd4: sub      sp, sp, #0x4c
00647fd8: mov      r7, r0
00647fdc: add      r4, r3, r1, lsl #5
00647fe0: ldrb     r2, [r4, #0x1c]
00647fe4: cmp      r2, #0
00647fe8: bne      #0x64803c
00647fec: ldr      r5, [r4, #0x10]
00647ff0: ldr      r4, [r4, #0xc]
00647ff4: cmp      r4, r5
00647ff8: beq      #0x648034
00647ffc: ldr      r2, [r4]
00648000: ldr      r3, [r7, #0x24]
00648004: mov      r1, #0
00648008: add      r4, r4, #4
0064800c: add      r3, r3, r2, lsl #3
00648010: ldr      r3, [r3, #4]
00648014: cmp      r3, r1
00648018: mov      r0, r3
0064801c: beq      #0x647ff4
00648020: ldr      r3, [r3]
00648024: mov      lr, pc
00648028: ldr      pc, [r3, #0x4c]
0064802c: cmp      r4, r5
00648030: bne      #0x647ffc
00648034: add      sp, sp, #0x4c
00648038: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0064803c: ldr      r3, [r3, r1, lsl #5]
00648040: mov      sb, #0
00648044: ldr      r3, [r3, #0x14]
00648048: ldr      r2, [r3, #4]
0064804c: add      sl, r3, #0x14
00648050: ands     r2, r2, #0x20000
00648054: ldrbne   r2, [r3, #0xc]
00648058: addne    r2, r2, #1
0064805c: uxtbne   r2, r2
00648060: addne    r2, sl, r2, lsl #4
00648064: str      r2, [sp, #0x14]
00648068: ldr      r1, [r3, #0x14]
0064806c: cmp      r1, #0
00648070: str      r1, [sp, #8]
00648074: ldrne    ip, [sp, #8]
00648078: moveq    r0, r1
0064807c: mov      r1, #4
00648080: ldrne    r2, [ip, #4]
00648084: addne    r2, r2, #1
00648088: strne    r2, [ip, #4]
0064808c: ldrne    r0, [r3, #0x14]
00648090: bl       #0x5a19f0
00648094: mov      r3, #0xff
00648098: str      r0, [sp, #0x1c]
0064809c: str      r3, [sp, #0x3c]
006480a0: str      sb, [sp, #0x34]
006480a4: str      sb, [sp, #0x38]
006480a8: strh     sb, [sp, #0x40]
006480ac: strh     sb, [sp, #0x42]
006480b0: ldr      fp, [r4, #0x10]
006480b4: ldr      r5, [r4, #0xc]
006480b8: cmp      r5, fp
006480bc: beq      #0x648290
006480c0: add      r1, sp, #0x44
006480c4: add      r2, sp, #0x24
006480c8: add      r3, sp, #0x34
006480cc: str      r1, [sp, #0x10]
006480d0: str      r2, [sp, #0xc]
006480d4: str      r3, [sp, #0x18]
006480d8: mov      r8, r7
006480dc: b        #0x648220
006480e0: ldr      r3, [r4]
006480e4: ldr      r0, [sp, #0x10]
006480e8: mov      r1, r4
006480ec: mov      r2, #0
006480f0: mov      lr, pc
006480f4: ldr      pc, [r3, #0x14]
006480f8: ldr      r7, [sp, #0x44]
006480fc: cmp      r7, #0
00648100: beq      #0x64810c
00648104: mov      r0, r7
00648108: bl       #0x31d584
0064810c: ldr      r6, [r7, #0x14]
00648110: ldrh     r2, [sl, #0xe]
00648114: ldr      r1, [r7, #0x24]
00648118: ldr      r3, [r6, #0x14]
0064811c: add      r7, r6, #0x14
00648120: mls      r1, r1, r2, sb
00648124: str      r3, [sp, #0x24]
00648128: str      r1, [sp, #4]
0064812c: cmp      r3, #0
00648130: ldrne    r2, [r3, #4]
00648134: mov      r0, r6
00648138: mov      r1, r7
0064813c: addne    r2, r2, #1
00648140: strne    r2, [r3, #4]
00648144: ldr      r3, [r7, #4]
00648148: mov      r2, sl
0064814c: str      r3, [sp, #0x28]
00648150: ldrh     ip, [r7, #0xa]
00648154: ldr      r3, [sp, #4]
00648158: str      ip, [sp, #0x2c]
0064815c: ldrh     ip, [r7, #0xc]
00648160: strh     ip, [sp, #0x30]
00648164: ldrh     ip, [r7, #0xe]
00648168: strh     ip, [sp, #0x32]
0064816c: bl       #0x647ee0
00648170: ldr      r3, [r6, #4]
00648174: tst      r3, #0x20000
00648178: beq      #0x6482d8
0064817c: ldr      r1, [sp, #0x14]
00648180: cmp      r1, #0
00648184: beq      #0x6482d8
00648188: ldrb     r1, [r6, #0xc]
0064818c: ldr      r0, [sp, #0x18]
00648190: add      r1, r1, #1
00648194: uxtb     r1, r1
00648198: add      r1, r7, r1, lsl #4
0064819c: bl       #0x647f54
006481a0: ldrb     r1, [r6, #0xc]
006481a4: ldr      r2, [sp, #0x14]
006481a8: ldr      r3, [sp, #4]
006481ac: add      r1, r1, #1
006481b0: add      r1, r7, r1, lsl #4
006481b4: mov      r0, r6
006481b8: bl       #0x647ee0
006481bc: mov      r0, r4
006481c0: ldr      r3, [r4]
006481c4: mov      r1, #0
006481c8: mov      lr, pc
006481cc: ldr      pc, [r3, #0x4c]
006481d0: ldr      r3, [r6, #8]
006481d4: ldrh     ip, [sl, #0xe]
006481d8: mov      r0, r6
006481dc: mov      r1, r7
006481e0: ldr      r2, [sp, #0xc]
006481e4: mla      sb, r3, ip, sb
006481e8: bl       #0x647e78
006481ec: ldrb     r3, [r6, #0xc]
006481f0: mov      r0, r6
006481f4: ldr      r2, [sp, #0x18]
006481f8: add      r3, r3, #1
006481fc: add      r1, r7, r3, lsl #4
00648200: bl       #0x647e78
00648204: ldr      r0, [sp, #0x24]
00648208: cmp      r0, #0
0064820c: beq      #0x648214
00648210: bl       #0x31d584
00648214: add      r5, r5, #4
00648218: cmp      r5, fp
0064821c: beq      #0x648280
00648220: ldr      r2, [r5]
00648224: ldr      r3, [r8, #0x24]
00648228: add      r3, r3, r2, lsl #3
0064822c: ldr      r4, [r3, #4]
00648230: cmp      r4, #0
00648234: beq      #0x648214
00648238: mov      r1, #0
0064823c: mov      r0, r4
00648240: ldr      r3, [r4]
00648244: mov      lr, pc
00648248: ldr      pc, [r3, #0x48]
0064824c: ldr      r3, [r4]
00648250: mov      r0, r4
00648254: mov      lr, pc
00648258: ldr      pc, [r3, #0x50]
0064825c: subs     r1, r0, #0
00648260: bne      #0x6480e0
00648264: mov      r0, r4
00648268: ldr      r3, [r4]
0064826c: add      r5, r5, #4
00648270: mov      lr, pc
00648274: ldr      pc, [r3, #0x4c]
00648278: cmp      r5, fp
0064827c: bne      #0x648220
00648280: ldr      r0, [sp, #0x34]
00648284: cmp      r0, #0
00648288: beq      #0x648290
0064828c: bl       #0x31d584
00648290: ldr      r2, [sp, #0x1c]
00648294: cmp      r2, #0
00648298: beq      #0x6482c0
0064829c: ldr      ip, [sp, #8]
006482a0: ldrb     r3, [ip, #0x13]
006482a4: and      r2, r3, #0x1f
006482a8: cmp      r2, #1
006482ac: bls      #0x64830c
006482b0: sub      r2, r2, #1
006482b4: bic      r3, r3, #0x1f
006482b8: orr      r3, r2, r3
006482bc: strb     r3, [ip, #0x13]
006482c0: ldr      r3, [sp, #8]
006482c4: cmp      r3, #0
006482c8: beq      #0x648034
006482cc: mov      r0, r3
006482d0: bl       #0x31d584
006482d4: b        #0x648034
006482d8: mov      r0, r4
006482dc: ldr      r3, [r4]
006482e0: mov      r1, #0
006482e4: mov      lr, pc
006482e8: ldr      pc, [r3, #0x4c]
006482ec: ldrh     ip, [sl, #0xe]
006482f0: ldr      r3, [r6, #8]
006482f4: mov      r0, r6
006482f8: mov      r1, r7
006482fc: ldr      r2, [sp, #0xc]
00648300: mla      sb, r3, ip, sb
00648304: bl       #0x647e78
00648308: b        #0x648204
0064830c: ldr      r1, [sp, #8]
00648310: ldrb     r3, [r1, #0x12]
00648314: tst      r3, #0x20
00648318: bne      #0x648338
0064831c: ldr      r2, [sp, #8]
00648320: mov      r3, #0
00648324: strb     r3, [r2, #0x13]
00648328: ldr      r3, [sp, #8]
0064832c: cmp      r3, #0
00648330: bne      #0x6482cc
00648334: b        #0x648034
00648338: ldr      r3, [r1]
0064833c: mov      r0, r1
00648340: mov      lr, pc
00648344: ldr      pc, [r3, #0x18]
00648348: b        #0x64831c

# _ZN6glitch7collada19CModularSkinnedMeshC1ERKNS0_16CColladaDatabaseEPNS0_20SInstanceModularSkinEPNS0_14CRootSceneNodeEibPNS_5video12IVideoDriverE
00649120: ldr      ip, [pc, #0x154]
00649124: push     {r4, r5, r6, r7, r8, lr}
00649128: ldr      lr, [pc, #0x150]
0064912c: add      ip, pc, ip
00649130: mov      r4, r0
00649134: ldr      lr, [ip, lr]
00649138: mov      r0, #0
0064913c: str      r0, [r4, #4]
00649140: add      lr, lr, #8
00649144: str      lr, [r4]
00649148: ldr      r0, [r1]
0064914c: str      r0, [r4, #0xc]
00649150: ldr      r1, [r1, #4]
00649154: cmp      r0, #0
00649158: str      r1, [r4, #0x10]
0064915c: ldrb     r7, [sp, #0x1c]
00649160: beq      #0x649174
00649164: ldr      r1, [r0, #4]
00649168: cmp      r1, #0
0064916c: addne    r1, r1, #1
00649170: strne    r1, [r0, #4]
00649174: ldr      r5, [pc, #0x108]
00649178: ldr      r1, [pc, #0x108]
0064917c: mov      lr, #0xbf000000
00649180: ldr      r5, [ip, r5]
00649184: ldr      r1, [ip, r1]
00649188: add      lr, lr, #0x800000
0064918c: mov      r0, #0x3f800000
00649190: add      r6, r1, #8
00649194: mov      ip, #1
00649198: mov      r1, #0
0064919c: add      r5, r5, #4
006491a0: str      r5, [r4, #8]
006491a4: str      r6, [r4]
006491a8: str      r3, [r4, #0x20]
006491ac: str      lr, [r4, #0x48]
006491b0: str      r0, [r4, #0x54]
006491b4: strb     r1, [r4, #0x58]
006491b8: str      r1, [r4, #0x14]
006491bc: strb     ip, [r4, #0x18]
006491c0: str      r2, [r4, #0x1c]
006491c4: str      r1, [r4, #0x24]
006491c8: str      r1, [r4, #0x28]
006491cc: str      r1, [r4, #0x2c]
006491d0: str      r1, [r4, #0x30]
006491d4: str      r1, [r4, #0x34]
006491d8: str      r1, [r4, #0x38]
006491dc: str      r1, [r4, #0x3c]
006491e0: str      lr, [r4, #0x40]
006491e4: str      lr, [r4, #0x44]
006491e8: str      r0, [r4, #0x4c]
006491ec: str      r0, [r4, #0x50]
006491f0: strb     ip, [r4, #0x59]
006491f4: ldr      r3, [r2]
006491f8: ldr      r6, [r2, #8]
006491fc: ldr      r2, [sp, #0x18]
00649200: add      r6, r6, r3
00649204: cmp      r2, r1
00649208: ble      #0x649274
0064920c: mov      r0, r4
00649210: mov      r1, r6
00649214: mov      r2, #0
00649218: bl       #0x648e18
0064921c: cmp      r6, #0
00649220: beq      #0x649260
00649224: mov      r5, #0
00649228: ldr      r3, [r4, #0x1c]
0064922c: mov      r0, r4
00649230: ldr      r3, [r3, #4]
00649234: add      r3, r3, r5, lsl #4
00649238: ldr      r1, [r3, #4]
0064923c: bl       #0x6474b8
00649240: mov      r1, r5
00649244: mov      r2, r0
00649248: add      r5, r5, #1
0064924c: mov      r0, r4
00649250: mov      r3, #0
00649254: bl       #0x648fd0
00649258: cmp      r6, r5
0064925c: bne      #0x649228
00649260: mov      r1, r7
00649264: mov      r0, r4
00649268: bl       #0x6483c8
0064926c: mov      r0, r4
00649270: pop      {r4, r5, r6, r7, r8, pc}
00649274: streq    ip, [r4, #0x3c]
00649278: b        #0x64920c
0064927c: eorseq   fp, r4, r4, ror #18
00649280: andeq    r0, r0, r0, asr #20
00649284: strheq   r1, [r0], -r4
00649288: andeq    r3, r0, r8, lsr sl

# _ZN12SceneManager12SearchByNameEPN6glitch5scene10ISceneNodeEPKcb
0035a0e4: push     {r4, r5, r6, r7, r8, sl, lr}
0035a0e8: ldr      r4, [pc, #0x90]
0035a0ec: ldr      r6, [pc, #0x90]
0035a0f0: cmp      r2, #0
0035a0f4: cmpne    r1, #0
0035a0f8: add      r4, pc, r4
0035a0fc: ldr      ip, [r4, r6]
0035a100: mov      r5, r1
0035a104: sub      sp, sp, #0x24
0035a108: ldr      ip, [ip]
0035a10c: moveq    r1, #0
0035a110: movne    r1, #1
0035a114: mov      sl, r0
0035a118: mov      r8, r3
0035a11c: str      ip, [sp, #0x1c]
0035a120: moveq    r5, r1
0035a124: beq      #0x35a15c
0035a128: add      r7, sp, #4
0035a12c: mov      r1, r2
0035a130: mov      r0, r7
0035a134: mov      r2, sp
0035a138: bl       #0x3140ec
0035a13c: mov      r1, r5
0035a140: mov      r0, sl
0035a144: mov      r2, r7
0035a148: mov      r3, r8
0035a14c: bl       #0x352b74
0035a150: mov      r5, r0
0035a154: mov      r0, r7
0035a158: bl       #0x318254
0035a15c: ldr      r3, [r4, r6]
0035a160: ldr      r2, [sp, #0x1c]
0035a164: mov      r0, r5
0035a168: ldr      r3, [r3]
0035a16c: cmp      r2, r3
0035a170: bne      #0x35a17c
0035a174: add      sp, sp, #0x24
0035a178: pop      {r4, r5, r6, r7, r8, sl, pc}
0035a17c: bl       #0x30e310
0035a180: mlseq    r3, r8, sb, sl
0035a184: andeq    r4, r0, ip, lsr #1
