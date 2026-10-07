
# _ZNK6glitch7collada16CColladaDatabase14constructSceneEPNS_5video12IVideoDriverEb
0061babc: push     {r4, r5, r6, lr}
0061bac0: mov      r5, r2
0061bac4: mov      r6, r0
0061bac8: bl       #0x61b9e8
0061bacc: subs     r4, r0, #0
0061bad0: beq      #0x61badc
0061bad4: cmp      r5, #0
0061bad8: bne      #0x61bae4
0061badc: mov      r0, r4
0061bae0: pop      {r4, r5, r6, pc}
0061bae4: mov      r0, r6
0061bae8: bl       #0x60fb54
0061baec: subs     r5, r0, #0
0061baf0: beq      #0x61badc
0061baf4: mov      r0, r4
0061baf8: ldr      r3, [r4]
0061bafc: mov      r1, r5
0061bb00: mov      lr, pc
0061bb04: ldr      pc, [r3, #0x6c]
0061bb08: ldr      r3, [r5]
0061bb0c: ldr      r0, [r3, #-0xc]
0061bb10: add      r0, r5, r0
0061bb14: bl       #0x31d584
0061bb18: mov      r0, r4
0061bb1c: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada15CColladaFactory11createSceneERKNS0_16CColladaDatabaseE
00631628: push     {r4, r5, r6, lr}
0063162c: mov      r0, #0x1c4
00631630: mov      r5, r1
00631634: mov      r1, #0
00631638: bl       #0x5341ac
0063163c: mov      r1, r5
00631640: mov      r4, r0
00631644: bl       #0x65b734
00631648: mov      r0, r4
0063164c: pop      {r4, r5, r6, pc}

# _ZNK6glitch7collada16CColladaDatabase20constructModularSkinEPNS0_20SInstanceModularSkinEPNS0_14CRootSceneNodeE
0060e6f0: push     {r4, r5, lr}
0060e6f4: ldr      ip, [r1, #4]
0060e6f8: sub      sp, sp, #0x14
0060e6fc: mov      lr, r1
0060e700: mov      r5, r2
0060e704: mov      r4, r0
0060e708: mov      r1, ip
0060e70c: add      r0, sp, #0xc
0060e710: ldr      ip, [ip]
0060e714: mov      r2, lr
0060e718: str      r3, [sp]
0060e71c: mov      r3, r5
0060e720: mov      lr, pc
0060e724: ldr      pc, [ip, #0x5c]
0060e728: ldr      r0, [sp, #0xc]
0060e72c: cmp      r0, #0
0060e730: str      r0, [r4]
0060e734: ldrne    r3, [r0, #4]
0060e738: addne    r3, r3, #1
0060e73c: strne    r3, [r0, #4]
0060e740: ldrne    r0, [sp, #0xc]
0060e744: cmp      r0, #0
0060e748: beq      #0x60e750
0060e74c: bl       #0x31d584
0060e750: mov      r0, r4
0060e754: add      sp, sp, #0x14
0060e758: pop      {r4, r5, pc}

# _ZN6glitch5scene13CSceneManager23notifyVisibilityChangedEv
005890a8: mov      r3, #1
005890ac: strb     r3, [r0, #0x289]
005890b0: bx       lr

# _ZNK6glitch7collada16CColladaDatabase19constructControllerEPNS_5video12IVideoDriverEPNS0_19SInstanceControllerEPNS0_14CRootSceneNodeEb
0061ace8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061acec: mov      r6, r3
0061acf0: sub      sp, sp, #0x2c
0061acf4: ldr      r3, [r3, #4]
0061acf8: ldr      ip, [sp, #0x50]
0061acfc: ldrb     lr, [sp, #0x54]
0061ad00: mov      r5, r0
0061ad04: add      r3, r3, #1
0061ad08: str      ip, [sp]
0061ad0c: mov      sl, r1
0061ad10: mov      fp, r2
0061ad14: str      lr, [sp, #0x14]
0061ad18: bl       #0x61aa00
0061ad1c: ldr      r3, [r5]
0061ad20: cmp      r3, #0
0061ad24: beq      #0x61aeac
0061ad28: ldr      r2, [r6, #0xc]
0061ad2c: cmp      r2, #0
0061ad30: ble      #0x61ade0
0061ad34: mov      r7, #0
0061ad38: mov      r8, r7
0061ad3c: add      r4, sp, #0x20
0061ad40: add      sb, sp, #0x24
0061ad44: b        #0x61adb0
0061ad48: ldr      r2, [r3, #4]
0061ad4c: add      r2, r2, #1
0061ad50: bl       #0x61ac88
0061ad54: mov      r2, r0
0061ad58: mov      r0, r4
0061ad5c: ldr      r1, [sp, #0x50]
0061ad60: mov      r3, fp
0061ad64: bl       #0x65cafc
0061ad68: ldr      r0, [r5]
0061ad6c: mov      lr, #0
0061ad70: mov      r1, r8
0061ad74: ldr      ip, [r0]
0061ad78: mov      r3, sb
0061ad7c: mov      r2, r4
0061ad80: ldr      ip, [ip, #0x20]
0061ad84: str      lr, [sp, #0x24]
0061ad88: blx      ip
0061ad8c: mov      r0, sb
0061ad90: bl       #0x57a26c
0061ad94: mov      r0, r4
0061ad98: bl       #0x310be8
0061ad9c: ldr      r3, [r6, #0xc]
0061ada0: add      r8, r8, #1
0061ada4: add      r7, r7, #0x3c
0061ada8: cmp      r8, r3
0061adac: bge      #0x61addc
0061adb0: ldr      r3, [r6, #0x10]
0061adb4: mov      r0, sl
0061adb8: ldr      r1, [r3, r7]
0061adbc: add      r3, r3, r7
0061adc0: cmp      r1, #0
0061adc4: bne      #0x61ad48
0061adc8: ldr      r1, [r3, #8]
0061adcc: mov      r0, sl
0061add0: bl       #0x60e400
0061add4: mov      r2, r0
0061add8: b        #0x61ad58
0061addc: ldr      r3, [r5]
0061ade0: mov      r0, r3
0061ade4: mov      r1, fp
0061ade8: ldr      r3, [r3]
0061adec: ldr      r2, [sp, #0x14]
0061adf0: mov      lr, pc
0061adf4: ldr      pc, [r3, #0x40]
0061adf8: ldr      r3, [r6, #0xc]
0061adfc: cmp      r3, #0
0061ae00: ble      #0x61aeac
0061ae04: mov      r8, #0
0061ae08: mov      r7, r8
0061ae0c: add      r4, sp, #0x20
0061ae10: add      sb, sp, #0x1c
0061ae14: mov      fp, r8
0061ae18: ldr      r3, [r5]
0061ae1c: mov      r2, r7
0061ae20: mov      r0, r4
0061ae24: mov      r1, r3
0061ae28: ldr      r3, [r3]
0061ae2c: mov      lr, pc
0061ae30: ldr      pc, [r3, #0x18]
0061ae34: ldr      r2, [sl, #4]
0061ae38: ldr      r3, [r6, #0x10]
0061ae3c: mov      r0, sb
0061ae40: ldr      ip, [r2]
0061ae44: mov      r1, r2
0061ae48: add      r3, r3, r8
0061ae4c: str      r7, [sp, #8]
0061ae50: mov      r2, sl
0061ae54: str      r5, [sp]
0061ae58: str      r4, [sp, #4]
0061ae5c: str      fp, [sp, #0xc]
0061ae60: mov      lr, pc
0061ae64: ldr      pc, [ip, #0x24]
0061ae68: ldr      ip, [r5]
0061ae6c: mov      r1, r7
0061ae70: mov      r3, sb
0061ae74: mov      r2, r4
0061ae78: mov      r0, ip
0061ae7c: ldr      ip, [ip]
0061ae80: mov      lr, pc
0061ae84: ldr      pc, [ip, #0x20]
0061ae88: mov      r0, sb
0061ae8c: bl       #0x57a26c
0061ae90: mov      r0, r4
0061ae94: bl       #0x310be8
0061ae98: ldr      r3, [r6, #0xc]
0061ae9c: add      r7, r7, #1
0061aea0: add      r8, r8, #0x3c
0061aea4: cmp      r7, r3
0061aea8: blt      #0x61ae18
0061aeac: mov      r0, r5
0061aeb0: add      sp, sp, #0x2c
0061aeb4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada15CColladaFactory10createNodeERKNS0_16CColladaDatabaseEPNS0_5SNodeE
00631800: push     {r4, r5, r6, lr}
00631804: mov      r0, #0x160
00631808: mov      r5, r1
0063180c: mov      r1, #0
00631810: mov      r6, r2
00631814: bl       #0x5341ac
00631818: mov      r1, r5
0063181c: mov      r4, r0
00631820: mov      r2, r6
00631824: bl       #0x65d150
00631828: mov      r0, r4
0063182c: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada19CModularSkinnedMesh14setModuleCountEjb
00648e18: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00648e1c: ldr      r8, [r0, #0x24]
00648e20: ldr      r6, [r0, #0x28]
00648e24: sub      sp, sp, #0xc
00648e28: mov      r4, r0
00648e2c: rsb      r6, r8, r6
00648e30: asr      r6, r6, #3
00648e34: cmp      r1, r6
00648e38: mov      r5, r1
00648e3c: mov      r7, r2
00648e40: bhs      #0x648e88
00648e44: lsl      sb, r1, #3
00648e48: mov      sl, r1
00648e4c: mvn      fp, #0
00648e50: b        #0x648e58
00648e54: ldr      r8, [r4, #0x24]
00648e58: add      r8, r8, sb
00648e5c: ldr      r0, [r8, #4]
00648e60: mov      r3, #0
00648e64: add      sl, sl, #1
00648e68: cmp      r0, r3
00648e6c: str      r3, [r8, #4]
00648e70: beq      #0x648e78
00648e74: bl       #0x31d584
00648e78: cmp      sl, r6
00648e7c: str      fp, [r8]
00648e80: add      sb, sb, #8
00648e84: blo      #0x648e54
00648e88: mvn      r3, #0
00648e8c: add      r0, r4, #0x24
00648e90: str      r3, [sp]
00648e94: mov      r1, r5
00648e98: mov      r3, #0
00648e9c: mov      r2, sp
00648ea0: str      r3, [sp, #4]
00648ea4: bl       #0x647724
00648ea8: ldr      r0, [sp, #4]
00648eac: cmp      r0, #0
00648eb0: beq      #0x648eb8
00648eb4: bl       #0x31d584
00648eb8: cmp      r5, r6
00648ebc: bls      #0x648f00
00648ec0: lsl      sb, r6, #3
00648ec4: mov      sl, r6
00648ec8: mvn      fp, #0
00648ecc: ldr      r8, [r4, #0x24]
00648ed0: mov      r3, #0
00648ed4: add      sl, sl, #1
00648ed8: add      r8, r8, sb
00648edc: ldr      r0, [r8, #4]
00648ee0: add      sb, sb, #8
00648ee4: str      r3, [r8, #4]
00648ee8: cmp      r0, r3
00648eec: beq      #0x648ef4
00648ef0: bl       #0x31d584
00648ef4: cmp      r5, sl
00648ef8: str      fp, [r8]
00648efc: bhi      #0x648ecc
00648f00: cmp      r7, #0
00648f04: beq      #0x648f10
00648f08: cmp      r5, r6
00648f0c: blo      #0x648f1c
00648f10: mov      r0, #0
00648f14: add      sp, sp, #0xc
00648f18: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00648f1c: ldr      r1, [r4, #0x14]
00648f20: mov      r0, r4
00648f24: eor      r1, r1, #1
00648f28: and      r1, r1, #1
00648f2c: bl       #0x6483c8
00648f30: b        #0x648f14
