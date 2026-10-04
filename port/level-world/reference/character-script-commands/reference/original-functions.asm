
# _ZN9Character7_AttackERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b9f44: push     {r4, lr}
003b9f48: ldr      r3, [r0, #4]
003b9f4c: ldm      r3, {r0, r1}
003b9f50: rsb      r3, r0, r1
003b9f54: asr      r3, r3, #4
003b9f58: add      r1, r3, r3, lsl #3
003b9f5c: add      r1, r1, r1, lsl #6
003b9f60: add      r1, r3, r1, lsl #3
003b9f64: add      r1, r1, r1, lsl #15
003b9f68: add      r3, r3, r1, lsl #3
003b9f6c: cmp      r3, #0
003b9f70: bne      #0x3b9f8c
003b9f74: ldr      r1, [r2, #0x408]
003b9f78: cmp      r1, #0
003b9f7c: beq      #0x3b9fa0
003b9f80: ldr      r0, [r2, #0x378]
003b9f84: pop      {r4, lr}
003b9f88: b        #0x405b04
003b9f8c: ldr      r3, [r0, #4]
003b9f90: cmp      r3, #2
003b9f94: beq      #0x3b9fa4
003b9f98: cmp      r3, #7
003b9f9c: beq      #0x3b9fa4
003b9fa0: pop      {r4, pc}
003b9fa4: ldr      r4, [r2, #0x378]
003b9fa8: bl       #0x31b5a0
003b9fac: mov      r1, r0
003b9fb0: mov      r0, r4
003b9fb4: pop      {r4, lr}
003b9fb8: b        #0x405b04

# _ZN9Character5_FleeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b91a0: push     {r4, r5, r6, r7, r8, sl, lr}
003b91a4: ldr      r3, [r0, #4]
003b91a8: mov      r4, r2
003b91ac: sub      sp, sp, #0x14
003b91b0: ldm      r3, {r0, r2}
003b91b4: rsb      r3, r0, r2
003b91b8: asr      r3, r3, #4
003b91bc: add      r2, r3, r3, lsl #3
003b91c0: add      r2, r2, r2, lsl #6
003b91c4: add      r2, r3, r2, lsl #3
003b91c8: add      r2, r2, r2, lsl #15
003b91cc: add      r3, r3, r2, lsl #3
003b91d0: cmp      r3, #0
003b91d4: bne      #0x3b91e0
003b91d8: add      sp, sp, #0x14
003b91dc: pop      {r4, r5, r6, r7, r8, sl, pc}
003b91e0: ldr      r3, [r0, #4]
003b91e4: cmp      r3, #2
003b91e8: beq      #0x3b91f4
003b91ec: cmp      r3, #7
003b91f0: bne      #0x3b91d8
003b91f4: bl       #0x31b5a0
003b91f8: ldr      r3, [r4, #0x408]
003b91fc: cmp      r3, #0
003b9200: beq      #0x3b91d8
003b9204: mov      r0, r4
003b9208: bl       #0x3935dc
003b920c: mov      r5, r0
003b9210: ldr      r0, [r4, #0x408]
003b9214: bl       #0x3935dc
003b9218: mov      r6, r0
003b921c: ldr      r1, [r0]
003b9220: ldr      r0, [r5]
003b9224: bl       #0x30e3ac
003b9228: ldr      r1, [r6, #4]
003b922c: mov      r8, r0
003b9230: ldr      r0, [r5, #4]
003b9234: bl       #0x30e3ac
003b9238: ldr      r1, [r6, #8]
003b923c: mov      sl, r0
003b9240: ldr      r0, [r5, #8]
003b9244: bl       #0x30e3ac
003b9248: mov      r5, r0
003b924c: mov      r0, r4
003b9250: ldr      r7, [r4, #0x378]
003b9254: bl       #0x3935dc
003b9258: mov      r4, r0
003b925c: ldr      r1, [r4, #4]
003b9260: mov      r0, sl
003b9264: bl       #0x30eba4
003b9268: ldr      r1, [r4, #8]
003b926c: mov      r6, r0
003b9270: mov      r0, r5
003b9274: bl       #0x30eba4
003b9278: ldr      r1, [r4]
003b927c: mov      r5, r0
003b9280: mov      r0, r8
003b9284: bl       #0x30eba4
003b9288: add      r1, sp, #4
003b928c: str      r0, [sp, #4]
003b9290: mov      r0, r7
003b9294: str      r6, [sp, #8]
003b9298: str      r5, [sp, #0xc]
003b929c: bl       #0x4054e4
003b92a0: b        #0x3b91d8

# _ZN9Character7_HeadToERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003ba71c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ba720: ldr      r6, [r0, #4]
003ba724: mov      r5, r2
003ba728: ldr      r7, [pc, #0x4a0]
003ba72c: ldm      r6, {r1, r3}
003ba730: add      r7, pc, r7
003ba734: sub      sp, sp, #0x2c
003ba738: rsb      r3, r1, r3
003ba73c: asr      r3, r3, #4
003ba740: mov      r4, r0
003ba744: add      r2, r3, r3, lsl #3
003ba748: add      r2, r2, r2, lsl #6
003ba74c: add      r2, r3, r2, lsl #3
003ba750: add      r2, r2, r2, lsl #15
003ba754: add      r3, r3, r2, lsl #3
003ba758: rsb      r3, r3, #0
003ba75c: cmp      r3, #1
003ba760: beq      #0x3ba864
003ba764: cmp      r3, #2
003ba768: bls      #0x3ba85c
003ba76c: cmp      r3, #0
003ba770: bne      #0x3ba784
003ba774: ldr      r0, [pc, #0x458]
003ba778: add      r0, pc, r0
003ba77c: bl       #0x708eb0
003ba780: ldr      r1, [r6]
003ba784: ldr      r3, [r1, #4]
003ba788: cmp      r3, #3
003ba78c: bne      #0x3ba8c8
003ba790: ldr      r2, [r4, #4]
003ba794: ldr      r3, [r2]
003ba798: ldr      r1, [r2, #4]
003ba79c: rsb      r1, r3, r1
003ba7a0: asr      r1, r1, #4
003ba7a4: add      r3, r1, r1, lsl #3
003ba7a8: add      r3, r3, r3, lsl #6
003ba7ac: add      r3, r1, r3, lsl #3
003ba7b0: add      r3, r3, r3, lsl #15
003ba7b4: add      r3, r1, r3, lsl #3
003ba7b8: rsb      r3, r3, #0
003ba7bc: cmp      r3, #1
003ba7c0: beq      #0x3babac
003ba7c4: cmp      r3, #2
003ba7c8: bls      #0x3ba85c
003ba7cc: mov      r8, #0
003ba7d0: str      r8, [sp, #0x1c]
003ba7d4: str      r8, [sp, #0x20]
003ba7d8: str      r8, [sp, #0x24]
003ba7dc: ldr      r3, [r2]
003ba7e0: ldr      r2, [r2, #4]
003ba7e4: rsb      r3, r3, r2
003ba7e8: asr      r3, r3, #4
003ba7ec: add      r2, r3, r3, lsl #3
003ba7f0: add      r2, r2, r2, lsl #6
003ba7f4: add      r2, r3, r2, lsl #3
003ba7f8: add      r2, r2, r2, lsl #15
003ba7fc: add      r3, r3, r2, lsl #3
003ba800: rsb      r3, r3, #0
003ba804: cmp      r3, #3
003ba808: bhi      #0x3ba95c
003ba80c: mov      r1, #0
003ba810: mov      r0, r4
003ba814: bl       #0x37baf8
003ba818: bl       #0x31bbf0
003ba81c: mov      r1, #1
003ba820: mov      r7, r0
003ba824: mov      r0, r4
003ba828: bl       #0x37baf8
003ba82c: bl       #0x31bbf0
003ba830: mov      r1, #2
003ba834: mov      r6, r0
003ba838: mov      r0, r4
003ba83c: bl       #0x37baf8
003ba840: bl       #0x31bbf0
003ba844: str      r7, [sp, #0x1c]
003ba848: str      r6, [sp, #0x20]
003ba84c: str      r0, [sp, #0x24]
003ba850: ldr      r0, [r5, #0x378]
003ba854: add      r1, sp, #0x1c
003ba858: bl       #0x40542c
003ba85c: add      sp, sp, #0x2c
003ba860: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ba864: mov      r1, #0
003ba868: bl       #0x37baf8
003ba86c: ldr      r3, [r0, #4]
003ba870: cmp      r3, #2
003ba874: beq      #0x3ba928
003ba878: mov      r0, r4
003ba87c: mov      r1, #0
003ba880: bl       #0x37baf8
003ba884: ldr      r3, [r0, #4]
003ba888: cmp      r3, #7
003ba88c: bne      #0x3ba85c
003ba890: ldr      r2, [r4, #4]
003ba894: ldm      r2, {r1, r3}
003ba898: mov      r6, r2
003ba89c: rsb      r3, r1, r3
003ba8a0: asr      r3, r3, #4
003ba8a4: add      r0, r3, r3, lsl #3
003ba8a8: add      r0, r0, r0, lsl #6
003ba8ac: add      r0, r3, r0, lsl #3
003ba8b0: add      r0, r0, r0, lsl #15
003ba8b4: add      r3, r3, r0, lsl #3
003ba8b8: rsb      r3, r3, #0
003ba8bc: cmp      r3, #2
003ba8c0: bls      #0x3ba7bc
003ba8c4: b        #0x3ba76c
003ba8c8: mov      r0, r4
003ba8cc: mov      r1, #1
003ba8d0: bl       #0x37baf8
003ba8d4: ldr      r3, [r0, #4]
003ba8d8: cmp      r3, #3
003ba8dc: beq      #0x3ba8f8
003ba8e0: mov      r0, r4
003ba8e4: mov      r1, #2
003ba8e8: bl       #0x37baf8
003ba8ec: ldr      r3, [r0, #4]
003ba8f0: cmp      r3, #3
003ba8f4: bne      #0x3ba85c
003ba8f8: ldr      r2, [r4, #4]
003ba8fc: ldr      r1, [r2, #4]
003ba900: ldr      r3, [r2]
003ba904: rsb      r3, r3, r1
003ba908: asr      r3, r3, #4
003ba90c: add      r1, r3, r3, lsl #3
003ba910: add      r1, r1, r1, lsl #6
003ba914: add      r1, r3, r1, lsl #3
003ba918: add      r1, r1, r1, lsl #15
003ba91c: add      r3, r3, r1, lsl #3
003ba920: rsb      r3, r3, #0
003ba924: b        #0x3ba7bc
003ba928: ldr      r2, [r4, #4]
003ba92c: ldr      r1, [r2]
003ba930: ldr      r0, [r2, #4]
003ba934: mov      r6, r2
003ba938: rsb      r0, r1, r0
003ba93c: asr      r0, r0, #4
003ba940: add      r3, r0, r0, lsl #3
003ba944: add      r3, r3, r3, lsl #6
003ba948: add      r3, r0, r3, lsl #3
003ba94c: add      r3, r3, r3, lsl #15
003ba950: add      r3, r0, r3, lsl #3
003ba954: rsb      r3, r3, #0
003ba958: b        #0x3ba8bc
003ba95c: mov      r0, r4
003ba960: mov      r1, #3
003ba964: bl       #0x37baf8
003ba968: ldr      r6, [r0, #4]
003ba96c: cmp      r6, #1
003ba970: bne      #0x3ba80c
003ba974: mov      r1, #3
003ba978: mov      r0, r4
003ba97c: bl       #0x37baf8
003ba980: bl       #0x31bc80
003ba984: cmp      r0, #0
003ba988: beq      #0x3ba80c
003ba98c: mov      r0, r5
003ba990: add      r1, sp, #0x10
003ba994: str      r8, [sp, #0x18]
003ba998: str      r8, [sp, #0x10]
003ba99c: str      r8, [sp, #0x14]
003ba9a0: bl       #0x393ae4
003ba9a4: ldr      r3, [pc, #0x22c]
003ba9a8: ldr      ip, [r5, #0x160]
003ba9ac: ldr      r2, [r5, #0x164]
003ba9b0: ldr      r7, [r7, r3]
003ba9b4: ldr      r3, [r5, #0x168]
003ba9b8: mov      r1, #0
003ba9bc: mov      r0, r4
003ba9c0: str      r3, [sp, #0x24]
003ba9c4: ldr      r3, [r7, #4]
003ba9c8: str      ip, [sp, #0x1c]
003ba9cc: str      r2, [sp, #0x20]
003ba9d0: str      r3, [sp, #4]
003ba9d4: ldr      r3, [sp, #0x14]
003ba9d8: ldr      fp, [r7, #8]
003ba9dc: ldr      sl, [r7]
003ba9e0: str      r3, [sp, #8]
003ba9e4: ldr      r3, [sp, #0x10]
003ba9e8: ldr      sb, [sp, #0x18]
003ba9ec: str      r3, [sp, #0xc]
003ba9f0: bl       #0x37baf8
003ba9f4: bl       #0x31bbf0
003ba9f8: mov      r1, sb
003ba9fc: mov      r8, r0
003baa00: ldr      r0, [sp, #4]
003baa04: bl       #0x30ed6c
003baa08: ldr      r1, [sp, #8]
003baa0c: mov      r3, r0
003baa10: mov      r0, fp
003baa14: str      r3, [sp]
003baa18: bl       #0x30ed6c
003baa1c: ldr      r3, [sp]
003baa20: mov      r1, r0
003baa24: mov      r0, r3
003baa28: bl       #0x30e3ac
003baa2c: mov      r1, r0
003baa30: mov      r0, r8
003baa34: bl       #0x30ed6c
003baa38: mov      r1, r0
003baa3c: ldr      r0, [sp, #0x1c]
003baa40: bl       #0x30eba4
003baa44: ldr      r1, [sp, #0xc]
003baa48: str      r0, [sp, #0x1c]
003baa4c: mov      r0, fp
003baa50: bl       #0x30ed6c
003baa54: mov      r1, sl
003baa58: mov      fp, r0
003baa5c: mov      r0, sb
003baa60: bl       #0x30ed6c
003baa64: mov      r1, r0
003baa68: mov      r0, fp
003baa6c: bl       #0x30e3ac
003baa70: mov      r1, r0
003baa74: mov      r0, r8
003baa78: bl       #0x30ed6c
003baa7c: mov      r1, r0
003baa80: ldr      r0, [sp, #0x20]
003baa84: bl       #0x30eba4
003baa88: mov      r1, sl
003baa8c: str      r0, [sp, #0x20]
003baa90: ldr      r0, [sp, #8]
003baa94: bl       #0x30ed6c
003baa98: ldr      r1, [sp, #0xc]
003baa9c: mov      sl, r0
003baaa0: ldr      r0, [sp, #4]
003baaa4: bl       #0x30ed6c
003baaa8: mov      r1, r0
003baaac: mov      r0, sl
003baab0: bl       #0x30e3ac
003baab4: mov      r1, r0
003baab8: mov      r0, r8
003baabc: bl       #0x30ed6c
003baac0: mov      r1, r0
003baac4: ldr      r0, [sp, #0x24]
003baac8: bl       #0x30eba4
003baacc: mov      r1, r6
003baad0: str      r0, [sp, #0x24]
003baad4: mov      r0, r4
003baad8: bl       #0x37baf8
003baadc: bl       #0x31bbf0
003baae0: ldr      r1, [sp, #0x14]
003baae4: mov      r6, r0
003baae8: bl       #0x30ed6c
003baaec: ldr      r1, [sp, #0x18]
003baaf0: mov      sl, r0
003baaf4: mov      r0, r6
003baaf8: bl       #0x30ed6c
003baafc: ldr      r1, [sp, #0x10]
003bab00: mov      r8, r0
003bab04: mov      r0, r6
003bab08: bl       #0x30ed6c
003bab0c: mov      r1, r0
003bab10: ldr      r0, [sp, #0x1c]
003bab14: bl       #0x30eba4
003bab18: mov      r1, sl
003bab1c: str      r0, [sp, #0x1c]
003bab20: ldr      r0, [sp, #0x20]
003bab24: bl       #0x30eba4
003bab28: mov      r1, r8
003bab2c: str      r0, [sp, #0x20]
003bab30: ldr      r0, [sp, #0x24]
003bab34: bl       #0x30eba4
003bab38: mov      r1, #2
003bab3c: str      r0, [sp, #0x24]
003bab40: mov      r0, r4
003bab44: bl       #0x37baf8
003bab48: bl       #0x31bbf0
003bab4c: ldr      r1, [r7, #4]
003bab50: mov      r4, r0
003bab54: bl       #0x30ed6c
003bab58: ldr      r1, [r7, #8]
003bab5c: mov      r8, r0
003bab60: mov      r0, r4
003bab64: bl       #0x30ed6c
003bab68: ldr      r1, [r7]
003bab6c: mov      r6, r0
003bab70: mov      r0, r4
003bab74: bl       #0x30ed6c
003bab78: mov      r1, r0
003bab7c: ldr      r0, [sp, #0x1c]
003bab80: bl       #0x30eba4
003bab84: mov      r1, r8
003bab88: str      r0, [sp, #0x1c]
003bab8c: ldr      r0, [sp, #0x20]
003bab90: bl       #0x30eba4
003bab94: mov      r1, r6
003bab98: str      r0, [sp, #0x20]
003bab9c: ldr      r0, [sp, #0x24]
003baba0: bl       #0x30eba4
003baba4: str      r0, [sp, #0x24]
003baba8: b        #0x3ba850
003babac: mov      r1, #0
003babb0: mov      r0, r4
003babb4: ldr      r4, [r5, #0x378]
003babb8: bl       #0x37baf8
003babbc: bl       #0x31b5a0
003babc0: mov      r1, r0
003babc4: mov      r0, r4
003babc8: bl       #0x405540
003babcc: b        #0x3ba85c
003babd0: subseq   sl, sp, r0, ror #6
003babd4: ldrsheq  r3, [r0], #-0xc0
003babd8: andeq    r4, r0, r0, asr #6

# _ZN9Character5_StopERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b56b4: ldr      r0, [r2, #0x378]
003b56b8: b        #0x40559c

# _ZN10GameObject8_HasPathERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0038e98c: ldr      r3, [r2, #0x200]!
0038e990: mov      r0, r1
0038e994: cmp      r3, r2
0038e998: moveq    r1, #0
0038e99c: beq      #0x38e9bc
0038e9a0: mov      ip, #0
0038e9a4: ldr      r3, [r3]
0038e9a8: add      ip, ip, #1
0038e9ac: cmp      r2, r3
0038e9b0: bne      #0x38e9a4
0038e9b4: subs     r1, ip, #0
0038e9b8: movne    r1, #1
0038e9bc: b        #0x37c7e4

# _ZN9Character7_MoveToERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003bada8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003badac: ldr      r6, [r0, #4]
003badb0: mov      r5, r2
003badb4: ldr      r7, [pc, #0x4a0]
003badb8: ldm      r6, {r1, r3}
003badbc: add      r7, pc, r7
003badc0: sub      sp, sp, #0x2c
003badc4: rsb      r3, r1, r3
003badc8: asr      r3, r3, #4
003badcc: mov      r4, r0
003badd0: add      r2, r3, r3, lsl #3
003badd4: add      r2, r2, r2, lsl #6
003badd8: add      r2, r3, r2, lsl #3
003baddc: add      r2, r2, r2, lsl #15
003bade0: add      r3, r3, r2, lsl #3
003bade4: rsb      r3, r3, #0
003bade8: cmp      r3, #1
003badec: beq      #0x3baef0
003badf0: cmp      r3, #2
003badf4: bls      #0x3baee8
003badf8: cmp      r3, #0
003badfc: bne      #0x3bae10
003bae00: ldr      r0, [pc, #0x458]
003bae04: add      r0, pc, r0
003bae08: bl       #0x708eb0
003bae0c: ldr      r1, [r6]
003bae10: ldr      r3, [r1, #4]
003bae14: cmp      r3, #3
003bae18: bne      #0x3baf54
003bae1c: ldr      r2, [r4, #4]
003bae20: ldr      r3, [r2]
003bae24: ldr      r1, [r2, #4]
003bae28: rsb      r1, r3, r1
003bae2c: asr      r1, r1, #4
003bae30: add      r3, r1, r1, lsl #3
003bae34: add      r3, r3, r3, lsl #6
003bae38: add      r3, r1, r3, lsl #3
003bae3c: add      r3, r3, r3, lsl #15
003bae40: add      r3, r1, r3, lsl #3
003bae44: rsb      r3, r3, #0
003bae48: cmp      r3, #1
003bae4c: beq      #0x3bb238
003bae50: cmp      r3, #2
003bae54: bls      #0x3baee8
003bae58: mov      r8, #0
003bae5c: str      r8, [sp, #0x1c]
003bae60: str      r8, [sp, #0x20]
003bae64: str      r8, [sp, #0x24]
003bae68: ldr      r3, [r2]
003bae6c: ldr      r2, [r2, #4]
003bae70: rsb      r3, r3, r2
003bae74: asr      r3, r3, #4
003bae78: add      r2, r3, r3, lsl #3
003bae7c: add      r2, r2, r2, lsl #6
003bae80: add      r2, r3, r2, lsl #3
003bae84: add      r2, r2, r2, lsl #15
003bae88: add      r3, r3, r2, lsl #3
003bae8c: rsb      r3, r3, #0
003bae90: cmp      r3, #3
003bae94: bhi      #0x3bafe8
003bae98: mov      r1, #0
003bae9c: mov      r0, r4
003baea0: bl       #0x37baf8
003baea4: bl       #0x31bbf0
003baea8: mov      r1, #1
003baeac: mov      r7, r0
003baeb0: mov      r0, r4
003baeb4: bl       #0x37baf8
003baeb8: bl       #0x31bbf0
003baebc: mov      r1, #2
003baec0: mov      r6, r0
003baec4: mov      r0, r4
003baec8: bl       #0x37baf8
003baecc: bl       #0x31bbf0
003baed0: str      r7, [sp, #0x1c]
003baed4: str      r6, [sp, #0x20]
003baed8: str      r0, [sp, #0x24]
003baedc: ldr      r0, [r5, #0x378]
003baee0: add      r1, sp, #0x1c
003baee4: bl       #0x4054e4
003baee8: add      sp, sp, #0x2c
003baeec: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003baef0: mov      r1, #0
003baef4: bl       #0x37baf8
003baef8: ldr      r3, [r0, #4]
003baefc: cmp      r3, #2
003baf00: beq      #0x3bafb4
003baf04: mov      r0, r4
003baf08: mov      r1, #0
003baf0c: bl       #0x37baf8
003baf10: ldr      r3, [r0, #4]
003baf14: cmp      r3, #7
003baf18: bne      #0x3baee8
003baf1c: ldr      r2, [r4, #4]
003baf20: ldm      r2, {r1, r3}
003baf24: mov      r6, r2
003baf28: rsb      r3, r1, r3
003baf2c: asr      r3, r3, #4
003baf30: add      r0, r3, r3, lsl #3
003baf34: add      r0, r0, r0, lsl #6
003baf38: add      r0, r3, r0, lsl #3
003baf3c: add      r0, r0, r0, lsl #15
003baf40: add      r3, r3, r0, lsl #3
003baf44: rsb      r3, r3, #0
003baf48: cmp      r3, #2
003baf4c: bls      #0x3bae48
003baf50: b        #0x3badf8
003baf54: mov      r0, r4
003baf58: mov      r1, #1
003baf5c: bl       #0x37baf8
003baf60: ldr      r3, [r0, #4]
003baf64: cmp      r3, #3
003baf68: beq      #0x3baf84
003baf6c: mov      r0, r4
003baf70: mov      r1, #2
003baf74: bl       #0x37baf8
003baf78: ldr      r3, [r0, #4]
003baf7c: cmp      r3, #3
003baf80: bne      #0x3baee8
003baf84: ldr      r2, [r4, #4]
003baf88: ldr      r1, [r2, #4]
003baf8c: ldr      r3, [r2]
003baf90: rsb      r3, r3, r1
003baf94: asr      r3, r3, #4
003baf98: add      r1, r3, r3, lsl #3
003baf9c: add      r1, r1, r1, lsl #6
003bafa0: add      r1, r3, r1, lsl #3
003bafa4: add      r1, r1, r1, lsl #15
003bafa8: add      r3, r3, r1, lsl #3
003bafac: rsb      r3, r3, #0
003bafb0: b        #0x3bae48
003bafb4: ldr      r2, [r4, #4]
003bafb8: ldr      r1, [r2]
003bafbc: ldr      r0, [r2, #4]
003bafc0: mov      r6, r2
003bafc4: rsb      r0, r1, r0
003bafc8: asr      r0, r0, #4
003bafcc: add      r3, r0, r0, lsl #3
003bafd0: add      r3, r3, r3, lsl #6
003bafd4: add      r3, r0, r3, lsl #3
003bafd8: add      r3, r3, r3, lsl #15
003bafdc: add      r3, r0, r3, lsl #3
003bafe0: rsb      r3, r3, #0
003bafe4: b        #0x3baf48
003bafe8: mov      r0, r4
003bafec: mov      r1, #3
003baff0: bl       #0x37baf8
003baff4: ldr      r6, [r0, #4]
003baff8: cmp      r6, #1
003baffc: bne      #0x3bae98
003bb000: mov      r1, #3
003bb004: mov      r0, r4
003bb008: bl       #0x37baf8
003bb00c: bl       #0x31bc80
003bb010: cmp      r0, #0
003bb014: beq      #0x3bae98
003bb018: mov      r0, r5
003bb01c: add      r1, sp, #0x10
003bb020: str      r8, [sp, #0x18]
003bb024: str      r8, [sp, #0x10]
003bb028: str      r8, [sp, #0x14]
003bb02c: bl       #0x393ae4
003bb030: ldr      r3, [pc, #0x22c]
003bb034: ldr      ip, [r5, #0x160]
003bb038: ldr      r2, [r5, #0x164]
003bb03c: ldr      r7, [r7, r3]
003bb040: ldr      r3, [r5, #0x168]
003bb044: mov      r1, #0
003bb048: mov      r0, r4
003bb04c: str      r3, [sp, #0x24]
003bb050: ldr      r3, [r7, #4]
003bb054: str      ip, [sp, #0x1c]
003bb058: str      r2, [sp, #0x20]
003bb05c: str      r3, [sp, #4]
003bb060: ldr      r3, [sp, #0x14]
003bb064: ldr      fp, [r7, #8]
003bb068: ldr      sl, [r7]
003bb06c: str      r3, [sp, #8]
003bb070: ldr      r3, [sp, #0x10]
003bb074: ldr      sb, [sp, #0x18]
003bb078: str      r3, [sp, #0xc]
003bb07c: bl       #0x37baf8
003bb080: bl       #0x31bbf0
003bb084: mov      r1, sb
003bb088: mov      r8, r0
003bb08c: ldr      r0, [sp, #4]
003bb090: bl       #0x30ed6c
003bb094: ldr      r1, [sp, #8]
003bb098: mov      r3, r0
003bb09c: mov      r0, fp
003bb0a0: str      r3, [sp]
003bb0a4: bl       #0x30ed6c
003bb0a8: ldr      r3, [sp]
003bb0ac: mov      r1, r0
003bb0b0: mov      r0, r3
003bb0b4: bl       #0x30e3ac
003bb0b8: mov      r1, r0
003bb0bc: mov      r0, r8
003bb0c0: bl       #0x30ed6c
003bb0c4: mov      r1, r0
003bb0c8: ldr      r0, [sp, #0x1c]
003bb0cc: bl       #0x30eba4
003bb0d0: ldr      r1, [sp, #0xc]
003bb0d4: str      r0, [sp, #0x1c]
003bb0d8: mov      r0, fp
003bb0dc: bl       #0x30ed6c
003bb0e0: mov      r1, sl
003bb0e4: mov      fp, r0
003bb0e8: mov      r0, sb
003bb0ec: bl       #0x30ed6c
003bb0f0: mov      r1, r0
003bb0f4: mov      r0, fp
003bb0f8: bl       #0x30e3ac
003bb0fc: mov      r1, r0
003bb100: mov      r0, r8
003bb104: bl       #0x30ed6c
003bb108: mov      r1, r0
003bb10c: ldr      r0, [sp, #0x20]
003bb110: bl       #0x30eba4
003bb114: mov      r1, sl
003bb118: str      r0, [sp, #0x20]
003bb11c: ldr      r0, [sp, #8]
003bb120: bl       #0x30ed6c
003bb124: ldr      r1, [sp, #0xc]
003bb128: mov      sl, r0
003bb12c: ldr      r0, [sp, #4]
003bb130: bl       #0x30ed6c
003bb134: mov      r1, r0
003bb138: mov      r0, sl
003bb13c: bl       #0x30e3ac
003bb140: mov      r1, r0
003bb144: mov      r0, r8
003bb148: bl       #0x30ed6c
003bb14c: mov      r1, r0
003bb150: ldr      r0, [sp, #0x24]
003bb154: bl       #0x30eba4
003bb158: mov      r1, r6
003bb15c: str      r0, [sp, #0x24]
003bb160: mov      r0, r4
003bb164: bl       #0x37baf8
003bb168: bl       #0x31bbf0
003bb16c: ldr      r1, [sp, #0x14]
003bb170: mov      r6, r0
003bb174: bl       #0x30ed6c
003bb178: ldr      r1, [sp, #0x18]
003bb17c: mov      sl, r0
003bb180: mov      r0, r6
003bb184: bl       #0x30ed6c
003bb188: ldr      r1, [sp, #0x10]
003bb18c: mov      r8, r0
003bb190: mov      r0, r6
003bb194: bl       #0x30ed6c
003bb198: mov      r1, r0
003bb19c: ldr      r0, [sp, #0x1c]
003bb1a0: bl       #0x30eba4
003bb1a4: mov      r1, sl
003bb1a8: str      r0, [sp, #0x1c]
003bb1ac: ldr      r0, [sp, #0x20]
003bb1b0: bl       #0x30eba4
003bb1b4: mov      r1, r8
003bb1b8: str      r0, [sp, #0x20]
003bb1bc: ldr      r0, [sp, #0x24]
003bb1c0: bl       #0x30eba4
003bb1c4: mov      r1, #2
003bb1c8: str      r0, [sp, #0x24]
003bb1cc: mov      r0, r4
003bb1d0: bl       #0x37baf8
003bb1d4: bl       #0x31bbf0
003bb1d8: ldr      r1, [r7, #4]
003bb1dc: mov      r4, r0
003bb1e0: bl       #0x30ed6c
003bb1e4: ldr      r1, [r7, #8]
003bb1e8: mov      r8, r0
003bb1ec: mov      r0, r4
003bb1f0: bl       #0x30ed6c
003bb1f4: ldr      r1, [r7]
003bb1f8: mov      r6, r0
003bb1fc: mov      r0, r4
003bb200: bl       #0x30ed6c
003bb204: mov      r1, r0
003bb208: ldr      r0, [sp, #0x1c]
003bb20c: bl       #0x30eba4
003bb210: mov      r1, r8
003bb214: str      r0, [sp, #0x1c]
003bb218: ldr      r0, [sp, #0x20]
003bb21c: bl       #0x30eba4
003bb220: mov      r1, r6
003bb224: str      r0, [sp, #0x20]
003bb228: ldr      r0, [sp, #0x24]
003bb22c: bl       #0x30eba4
003bb230: str      r0, [sp, #0x24]
003bb234: b        #0x3baedc
003bb238: mov      r1, #0
003bb23c: mov      r0, r4
003bb240: ldr      r4, [r5, #0x378]
003bb244: bl       #0x37baf8
003bb248: bl       #0x31b5a0
003bb24c: mov      r1, r0
003bb250: mov      r0, r4
003bb254: bl       #0x405540
003bb258: b        #0x3baee8
003bb25c: ldrsbeq  sb, [sp], #-0xc4
003bb260: subseq   r3, r0, r4, ror #12
003bb264: andeq    r4, r0, r0, asr #6
