
# _ZNSt6vectorIPN6glitch7collada18ISceneNodeAnimatorENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE18_M_insert_overflowEPS3_RKS3_RKSt11__true_typejb.clone.1
00476a9c: push     {r4, r5, r6, r7, r8, lr}
00476aa0: mov      r4, r0
00476aa4: ldr      r3, [r4]
00476aa8: ldr      r0, [r0, #4]
00476aac: mov      r6, r1
00476ab0: mov      r8, r2
00476ab4: rsb      r3, r3, r0
00476ab8: asr      r3, r3, #2
00476abc: cmp      r3, #1
00476ac0: addhs    r7, r3, r3
00476ac4: addlo    r7, r3, #1
00476ac8: cmn      r7, #0xc0000001
00476acc: bhi      #0x476b18
00476ad0: cmp      r3, r7
00476ad4: lslls    r7, r7, #2
00476ad8: bhi      #0x476b18
00476adc: mov      r1, #0
00476ae0: mov      r0, r7
00476ae4: bl       #0x310568
00476ae8: ldr      r1, [r4]
00476aec: mov      r5, r0
00476af0: subs     r6, r6, r1
00476af4: moveq    r6, r0
00476af8: bne      #0x476b3c
00476afc: ldr      r3, [r8]
00476b00: add      r7, r5, r7
00476b04: str      r3, [r6], #4
00476b08: ldr      r0, [r4]
00476b0c: bl       #0x310450
00476b10: stm      r4, {r5, r6, r7}
00476b14: pop      {r4, r5, r6, r7, r8, pc}
00476b18: mvn      r7, #3
00476b1c: mov      r1, #0
00476b20: mov      r0, r7
00476b24: bl       #0x310568
00476b28: ldr      r1, [r4]
00476b2c: mov      r5, r0
00476b30: subs     r6, r6, r1
00476b34: moveq    r6, r0
00476b38: beq      #0x476afc
00476b3c: mov      r2, r6
00476b40: bl       #0x30df38
00476b44: add      r6, r0, r6
00476b48: b        #0x476afc

# _ZN14AnimApplicator10ResetDeltaEjRKN6glitch4core8vector3dIfEE
003644cc: ldr      r3, [r2]
003644d0: mov      ip, #0
003644d4: str      r3, [r0, #0x18]
003644d8: ldr      r3, [r2, #4]
003644dc: str      r3, [r0, #0x1c]
003644e0: ldr      r3, [r2, #8]
003644e4: str      r1, [r0, #0x14]
003644e8: str      ip, [r0, #0x2c]
003644ec: str      r3, [r0, #0x20]
003644f0: str      ip, [r0, #0x24]
003644f4: str      ip, [r0, #0x28]
003644f8: bx       lr

# _ZN6glitch7collada18ISceneNodeAnimator10updateTimeEj
00667c48: push     {r4, r5, r6, lr}
00667c4c: sub      sp, sp, #8
00667c50: ldr      r3, [r0]
00667c54: mov      r5, r0
00667c58: mov      r6, r1
00667c5c: mov      lr, pc
00667c60: ldr      pc, [r3, #0x44]
00667c64: subs     r4, r0, #0
00667c68: beq      #0x667cac
00667c6c: mov      r1, r6
00667c70: ldm      r4, {r3, r6}
00667c74: mov      lr, pc
00667c78: ldr      pc, [r3]
00667c7c: ldr      ip, [r5, #0x18]
00667c80: ldr      r2, [r4, #4]
00667c84: cmp      ip, #0
00667c88: beq      #0x667cac
00667c8c: ldr      lr, [r4, #0x14]
00667c90: ldr      r3, [r4, #0x10]
00667c94: mov      r0, ip
00667c98: mov      r1, r6
00667c9c: ldr      ip, [ip]
00667ca0: str      lr, [sp]
00667ca4: mov      lr, pc
00667ca8: ldr      pc, [ip, #0x10]
00667cac: add      sp, sp, #8
00667cb0: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada18ISceneNodeAnimator19setCompatibleTargetERKNS0_8SChannelEPv
00669574: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00669578: ldr      r3, [r1, #8]
0066957c: ldr      r5, [pc, #0x1a0]
00669580: sub      sp, sp, #0x14
00669584: str      r2, [sp, #0xc]
00669588: cmp      r3, #0xe
0066958c: add      r5, pc, r5
00669590: mov      r6, r1
00669594: mov      r4, r0
00669598: ldr      r8, [r1, #4]
0066959c: beq      #0x669690
006695a0: ldr      r3, [r0]
006695a4: mov      lr, pc
006695a8: ldr      pc, [r3, #0x70]
006695ac: subs     sl, r0, #0
006695b0: ble      #0x669684
006695b4: ldr      r3, [pc, #0x16c]
006695b8: ldr      sb, [pc, #0x16c]
006695bc: mov      r7, #0
006695c0: add      r3, pc, r3
006695c4: str      r3, [sp, #8]
006695c8: mov      fp, #0xc
006695cc: b        #0x6695dc
006695d0: add      r7, r7, #1
006695d4: cmp      r7, sl
006695d8: beq      #0x669684
006695dc: mov      r1, r7
006695e0: ldr      r3, [r4]
006695e4: mov      r0, r4
006695e8: mov      lr, pc
006695ec: ldr      pc, [r3, #0x6c]
006695f0: mov      r1, r8
006695f4: bl       #0x30e31c
006695f8: cmp      r0, #0
006695fc: bne      #0x6695d0
00669600: mov      r1, r7
00669604: ldr      r3, [r4]
00669608: mov      r0, r4
0066960c: mov      lr, pc
00669610: ldr      pc, [r3, #0x54]
00669614: ldr      r1, [r5, sb]
00669618: ldr      r3, [r6, #8]
0066961c: ldr      r2, [r0, #8]
00669620: ldr      r1, [r1]
00669624: cmp      r3, #0x5b
00669628: mla      r2, fp, r2, r1
0066962c: bls      #0x669648
00669630: ldr      r0, [sp, #8]
00669634: str      r2, [sp, #4]
00669638: str      r3, [sp]
0066963c: bl       #0x708eb0
00669640: ldr      r3, [sp]
00669644: ldr      r2, [sp, #4]
00669648: lsr      r1, r3, #5
0066964c: ldr      r2, [r2, r1, lsl #2]
00669650: and      r3, r3, #0x1f
00669654: mov      r1, #1
00669658: ands     r2, r2, r1, lsl r3
0066965c: beq      #0x6695d0
00669660: mov      r0, r4
00669664: mov      r1, r7
00669668: ldr      r2, [sp, #0xc]
0066966c: ldr      ip, [r4]
00669670: mov      r3, #0
00669674: mov      lr, pc
00669678: ldr      pc, [ip, #0x68]
0066967c: mov      r0, #1
00669680: b        #0x669688
00669684: mov      r0, #0
00669688: add      sp, sp, #0x14
0066968c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00669690: ldr      r3, [r0]
00669694: ldrb     sl, [r1, #0xc]
00669698: mov      lr, pc
0066969c: ldr      pc, [r3, #0x70]
006696a0: subs     r7, r0, #0
006696a4: ble      #0x669684
006696a8: mov      r6, #0
006696ac: b        #0x6696bc
006696b0: add      r6, r6, #1
006696b4: cmp      r6, r7
006696b8: beq      #0x669684
006696bc: mov      r1, r6
006696c0: ldr      r3, [r4]
006696c4: mov      r0, r4
006696c8: mov      lr, pc
006696cc: ldr      pc, [r3, #0x6c]
006696d0: mov      r1, r8
006696d4: bl       #0x30e31c
006696d8: subs     r5, r0, #0
006696dc: bne      #0x6696b0
006696e0: ldr      r3, [r4]
006696e4: mov      r1, r6
006696e8: mov      r0, r4
006696ec: mov      lr, pc
006696f0: ldr      pc, [r3, #0x54]
006696f4: ldrb     r3, [r0, #0xc]
006696f8: cmp      r3, sl
006696fc: bne      #0x6696b0
00669700: mov      r0, r4
00669704: mov      r1, r6
00669708: ldr      r2, [sp, #0xc]
0066970c: mov      r3, r5
00669710: ldr      ip, [r4]
00669714: mov      lr, pc
00669718: ldr      pc, [ip, #0x68]
0066971c: mov      r0, #1
00669720: b        #0x669688
00669724: eorseq   fp, r2, r4, lsl #10
00669728: eoreq    r8, r5, r8, lsl #14
0066972c: andeq    r4, r0, ip, asr #10

# _ZN15AnimatorBlender17BlenderApplicator11AnimateNodeEj
00366888: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036688c: mov      r4, r0
00366890: ldr      r2, [r0, #0xc]
00366894: ldr      r0, [pc, #0x1b4]
00366898: sub      sp, sp, #0x3c
0036689c: mov      r3, #0
003668a0: add      r0, pc, r0
003668a4: cmn      r2, #1
003668a8: str      r3, [r4, #0x2c]
003668ac: str      r0, [sp, #0x10]
003668b0: str      r1, [sp, #0xc]
003668b4: str      r3, [sp, #0x2c]
003668b8: str      r3, [sp, #0x30]
003668bc: str      r3, [sp, #0x34]
003668c0: str      r3, [r4, #0x24]
003668c4: str      r3, [r4, #0x28]
003668c8: beq      #0x366a48
003668cc: ldr      r3, [r4, #0x3c]
003668d0: ldr      r1, [r3, #0x2c]
003668d4: ldr      r2, [r3, #0x28]
003668d8: rsb      r3, r2, r1
003668dc: cmp      r3, #3
003668e0: ble      #0x366a48
003668e4: ldr      r3, [pc, #0x168]
003668e8: ldr      r1, [pc, #0x168]
003668ec: mov      r6, #0
003668f0: str      r3, [sp, #0x18]
003668f4: ldr      r3, [pc, #0x160]
003668f8: str      r1, [sp, #0x14]
003668fc: add      r7, sp, #0x2c
00366900: add      r3, pc, r3
00366904: str      r3, [sp, #0x1c]
00366908: ldr      r3, [pc, #0x150]
0036690c: add      r3, pc, r3
00366910: str      r3, [sp, #0x20]
00366914: ldr      r3, [pc, #0x148]
00366918: add      r3, pc, r3
0036691c: str      r3, [sp, #0x24]
00366920: b        #0x3669b4
00366924: mov      r2, r7
00366928: mov      r0, r5
0036692c: ldr      r1, [sp, #0xc]
00366930: bl       #0x364444
00366934: ldr      sl, [r4, #0x3c]
00366938: ldr      r1, [r5, #0x28]
0036693c: add      r6, r6, #1
00366940: ldr      r3, [sl, #0x34]
00366944: ldr      r8, [r3, r8]
00366948: mov      r0, r8
0036694c: bl       #0x30ed6c
00366950: ldr      r1, [r5, #0x2c]
00366954: mov      fp, r0
00366958: mov      r0, r8
0036695c: bl       #0x30ed6c
00366960: ldr      r1, [r5, #0x24]
00366964: mov      sb, r0
00366968: mov      r0, r8
0036696c: bl       #0x30ed6c
00366970: mov      r1, r0
00366974: ldr      r0, [r4, #0x24]
00366978: bl       #0x30eba4
0036697c: mov      r1, fp
00366980: str      r0, [r4, #0x24]
00366984: ldr      r0, [r4, #0x28]
00366988: bl       #0x30eba4
0036698c: mov      r1, sb
00366990: str      r0, [r4, #0x28]
00366994: ldr      r0, [r4, #0x2c]
00366998: bl       #0x30eba4
0036699c: str      r0, [r4, #0x2c]
003669a0: ldr      r3, [sl, #0x2c]
003669a4: ldr      r2, [sl, #0x28]
003669a8: rsb      r3, r2, r3
003669ac: cmp      r6, r3, asr #2
003669b0: bge      #0x366a48
003669b4: ldr      r5, [r2, r6, lsl #2]
003669b8: lsl      r8, r6, #2
003669bc: ldr      r3, [r5]
003669c0: mov      r0, r5
003669c4: mov      lr, pc
003669c8: ldr      pc, [r3, #0x44]
003669cc: ldr      ip, [r5]
003669d0: ldr      r2, [r0, #4]
003669d4: mov      r3, r7
003669d8: mov      r0, r5
003669dc: ldr      r1, [r4, #0xc]
003669e0: mov      lr, pc
003669e4: ldr      pc, [ip, #0x7c]
003669e8: mov      r0, r5
003669ec: bl       #0x369160
003669f0: subs     r5, r0, #0
003669f4: bne      #0x366924
003669f8: ldr      r0, [sp, #0x10]
003669fc: ldr      ip, [sp, #0x14]
00366a00: ldr      r3, [r0, ip]
00366a04: ldr      r3, [r3]
00366a08: cmp      r3, #2
00366a0c: streq    r5, [r5]
00366a10: beq      #0x366924
00366a14: cmp      r3, #1
00366a18: bne      #0x366924
00366a1c: ldr      r2, [sp, #0x10]
00366a20: ldr      r1, [sp, #0x18]
00366a24: movw     ip, #0x18a
00366a28: ldr      r3, [sp, #0x24]
00366a2c: ldr      r0, [r2, r1]
00366a30: ldr      r1, [sp, #0x1c]
00366a34: ldr      r2, [sp, #0x20]
00366a38: add      r0, r0, #0xa8
00366a3c: str      ip, [sp]
00366a40: bl       #0x30e004
00366a44: b        #0x366924
00366a48: add      sp, sp, #0x3c
00366a4c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch5scene10ISceneNode8setScaleERKNS_4core8vector3dIfEE
005970c4: ldr      r3, [r1]
005970c8: ldr      r2, [r0, #0x11c]
005970cc: str      r3, [r0, #0xc8]
005970d0: ldr      r3, [r1, #4]
005970d4: orr      r2, r2, #2
005970d8: str      r3, [r0, #0xcc]
005970dc: ldr      r3, [r1, #8]
005970e0: str      r2, [r0, #0x11c]
005970e4: str      r3, [r0, #0xd0]
005970e8: bx       lr

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE15getBlendedValueEPvPfiSC_
00620134: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00620138: cmp      r3, #1
0062013c: mov      r4, r3
00620140: mov      r5, r1
00620144: mov      r8, r2
00620148: ldr      sl, [sp, #0x20]
0062014c: beq      #0x620194
00620150: cmp      r3, #0
00620154: moveq    r7, #0
00620158: beq      #0x62018c
0062015c: mov      r7, #0
00620160: mov      r6, #0
00620164: ldr      r1, [r8, r6]
00620168: ldr      r0, [r5, r6]
0062016c: bl       #0x30ed6c
00620170: mov      r1, r0
00620174: mov      r0, r7
00620178: bl       #0x30eba4
0062017c: subs     r4, r4, #1
00620180: mov      r7, r0
00620184: add      r6, r6, #4
00620188: bne      #0x620164
0062018c: str      r7, [sl]
00620190: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00620194: ldr      r3, [r1]
00620198: str      r3, [sl]
0062019c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK6glitch7collada16CColladaDatabase15getDefaultValueEPKNS0_8SChannelEPPv
0061c6bc: str      lr, [sp, #-4]!
0061c6c0: mov      ip, r1
0061c6c4: ldr      lr, [ip, #8]
0061c6c8: sub      sp, sp, #0xc
0061c6cc: ldr      r1, [r1, #4]
0061c6d0: mov      r3, r2
0061c6d4: add      ip, ip, #0xc
0061c6d8: mov      r2, lr
0061c6dc: str      ip, [sp]
0061c6e0: bl       #0x61c2f8
0061c6e4: add      sp, sp, #0xc
0061c6e8: ldm      sp!, {pc}

# _Z13GetApplicatorPN6glitch5scene18ISceneNodeAnimatorE
00369160: push     {r4, lr}
00369164: subs     r4, r0, #0
00369168: bne      #0x369174
0036916c: mov      r0, #0
00369170: pop      {r4, pc}
00369174: ldr      r3, [r4]
00369178: mov      lr, pc
0036917c: ldr      pc, [r3, #0x24]
00369180: sub      r0, r0, #0xb
00369184: cmp      r0, #4
00369188: addls    pc, pc, r0, lsl #2
0036918c: b        #0x36916c
00369190: b        #0x3691a4
00369194: b        #0x3691ac
00369198: b        #0x3691a4
0036919c: b        #0x3691b4
003691a0: b        #0x3691bc
003691a4: add      r0, r4, #0x58
003691a8: pop      {r4, pc}
003691ac: add      r0, r4, #0x88
003691b0: pop      {r4, pc}
003691b4: add      r0, r4, #0xd8
003691b8: pop      {r4, pc}
003691bc: add      r0, r4, #0x84
003691c0: pop      {r4, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEEEE17applyBlendedValueEPvPfiSB_PNS1_15CApplicatorInfoE
0062d72c: mov      r0, r1
0062d730: ldr      ip, [sp, #4]
0062d734: mov      r1, r2
0062d738: mov      r2, r3
0062d73c: ldr      r3, [sp]
0062d740: str      ip, [sp]
0062d744: b        #0x62d634

# _ZN6glitch7collada21CSceneNodeAnimatorSet13getTargetSizeEi
0065f188: push     {r4, lr}
0065f18c: ldr      r3, [r0, #0x24]
0065f190: ldr      r3, [r3, #0x18]
0065f194: ldr      r3, [r3, r1, lsl #2]
0065f198: mov      r0, r3
0065f19c: ldr      r3, [r3]
0065f1a0: mov      lr, pc
0065f1a4: ldr      pc, [r3, #8]
0065f1a8: pop      {r4, pc}

# _ZN6glitch7collada25CSceneNodeAnimatorBlender7compileEPSt6vectorIhNS_4core10SAllocatorIhLNS_6memory13E_MEMORY_HINTE0EEEE
0065ed98: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0065ed9c: sub      sp, sp, #0x2c
0065eda0: ldr      r3, [r0]
0065eda4: str      r1, [sp, #8]
0065eda8: mov      r4, r0
0065edac: mov      lr, pc
0065edb0: ldr      pc, [r3, #0x78]
0065edb4: ldr      r1, [r4, #0x2c]
0065edb8: ldr      r2, [r4, #0x28]
0065edbc: ldr      r3, [r4]
0065edc0: mov      r5, r0
0065edc4: rsb      r2, r2, r1
0065edc8: asr      r2, r2, #2
0065edcc: mov      r0, r4
0065edd0: str      r2, [sp, #0xc]
0065edd4: mov      lr, pc
0065edd8: ldr      pc, [r3, #0x70]
0065eddc: ldr      r1, [sp, #8]
0065ede0: str      r0, [sp, #4]
0065ede4: cmp      r1, #0
0065ede8: beq      #0x65efbc
0065edec: add      r2, sp, #0x28
0065edf0: mov      r5, #0
0065edf4: str      r5, [r2, #-8]!
0065edf8: ldr      r1, [sp, #0xc]
0065edfc: add      r0, r4, #0x34
0065ee00: bl       #0x368b60
0065ee04: ldr      r2, [r4, #0x34]
0065ee08: ldr      r1, [r4, #0x38]
0065ee0c: rsb      r1, r2, r1
0065ee10: asrs     r1, r1, #2
0065ee14: beq      #0x65ee34
0065ee18: mov      r3, #0
0065ee1c: b        #0x65ee24
0065ee20: ldr      r2, [r4, #0x34]
0065ee24: str      r5, [r2, r3, lsl #2]
0065ee28: add      r3, r3, #1
0065ee2c: cmp      r3, r1
0065ee30: bne      #0x65ee20
0065ee34: add      r2, sp, #0x28
0065ee38: mov      r5, #0
0065ee3c: str      r5, [r2, #-0xc]!
0065ee40: add      r0, r4, #0x4c
0065ee44: ldr      r1, [sp, #4]
0065ee48: bl       #0x65ec40
0065ee4c: ldr      r1, [sp, #8]
0065ee50: ldr      r3, [r4, #0x28]
0065ee54: ldm      r1, {r0, r2}
0065ee58: ldr      fp, [r3]
0065ee5c: cmp      r2, r0
0065ee60: beq      #0x65ee70
0065ee64: mov      r1, r5
0065ee68: rsb      r2, r0, r2
0065ee6c: bl       #0x30e460
0065ee70: ldr      r2, [sp, #4]
0065ee74: cmp      r2, #0
0065ee78: ble      #0x65ef68
0065ee7c: mov      sb, #0
0065ee80: str      sb, [sp]
0065ee84: mov      r1, sb
0065ee88: ldr      r3, [r4]
0065ee8c: mov      r0, r4
0065ee90: mov      lr, pc
0065ee94: ldr      pc, [r3, #0x74]
0065ee98: ldr      r3, [sp, #8]
0065ee9c: ldr      r1, [sp]
0065eea0: mov      r5, r0
0065eea4: ldr      r2, [r3]
0065eea8: ldr      r3, [r4, #0x4c]
0065eeac: mov      r0, fp
0065eeb0: add      r2, r2, r1
0065eeb4: str      r2, [r3, sb, lsl #2]
0065eeb8: ldr      r2, [r4, #0x4c]
0065eebc: mov      r1, sb
0065eec0: mov      r3, #0
0065eec4: ldr      r7, [r2, sb, lsl #2]
0065eec8: ldr      ip, [fp]
0065eecc: mov      r2, r7
0065eed0: mov      lr, pc
0065eed4: ldr      pc, [ip, #0x68]
0065eed8: ldr      r3, [fp]
0065eedc: mov      r0, fp
0065eee0: mov      r1, sb
0065eee4: mov      lr, pc
0065eee8: ldr      pc, [r3, #0x54]
0065eeec: ldr      r3, [r4, #0x28]
0065eef0: ldr      r8, [r4, #0x2c]
0065eef4: mov      sl, r0
0065eef8: rsb      r8, r3, r8
0065eefc: asr      r8, r8, #2
0065ef00: cmp      r8, #1
0065ef04: bls      #0x65ef44
0065ef08: add      r7, r7, r5
0065ef0c: mov      r6, #1
0065ef10: b        #0x65ef18
0065ef14: ldr      r3, [r4, #0x28]
0065ef18: ldr      r3, [r3, r6, lsl #2]
0065ef1c: mov      r2, r7
0065ef20: add      r6, r6, #1
0065ef24: mov      r0, r3
0065ef28: mov      r1, sl
0065ef2c: ldr      r3, [r3]
0065ef30: mov      lr, pc
0065ef34: ldr      pc, [r3, #0x60]
0065ef38: cmp      r6, r8
0065ef3c: add      r7, r7, r5
0065ef40: bne      #0x65ef14
0065ef44: ldr      r2, [sp, #4]
0065ef48: add      sb, sb, #1
0065ef4c: cmp      sb, r2
0065ef50: beq      #0x65ef68
0065ef54: ldr      r1, [sp]
0065ef58: ldr      r3, [sp, #0xc]
0065ef5c: mla      r1, r3, r5, r1
0065ef60: str      r1, [sp]
0065ef64: b        #0x65ee84
0065ef68: mov      r5, #0
0065ef6c: add      r2, sp, #0x28
0065ef70: str      r5, [r2, #-0x10]!
0065ef74: add      r0, r4, #0x58
0065ef78: ldr      r1, [sp, #4]
0065ef7c: bl       #0x65ec40
0065ef80: add      r2, sp, #0x28
0065ef84: str      r5, [r2, #-0x14]!
0065ef88: ldr      r1, [sp, #4]
0065ef8c: add      r0, r4, #0x64
0065ef90: bl       #0x65ed54
0065ef94: ldr      r2, [r4, #0x2c]
0065ef98: ldr      r3, [r4, #0x28]
0065ef9c: strb     r5, [r4, #0x24]
0065efa0: rsb      r3, r3, r2
0065efa4: lsrs     r3, r3, #2
0065efa8: beq      #0x65efb4
0065efac: mov      r0, r4
0065efb0: bl       #0x667f18
0065efb4: add      sp, sp, #0x2c
0065efb8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0065efbc: add      r2, sp, #0x28
0065efc0: mov      r3, #0
0065efc4: strb     r3, [r2, #-1]!
0065efc8: add      r3, r4, #0x40
0065efcc: str      r3, [sp, #8]
0065efd0: ldr      r3, [sp, #0xc]
0065efd4: ldr      r0, [sp, #8]
0065efd8: mul      r1, r3, r5
0065efdc: bl       #0x57d94c
0065efe0: b        #0x65edec

# _ZN14AnimApplicator11SetCallbackEPFvPN6glitch5scene19ITimelineControllerEPvES4_
00364400: str      r2, [r0, #0x38]
00364404: str      r1, [r0, #0x34]
00364408: bx       lr

# _ZN6glitch5scene10ISceneNode11setPositionERKNS_4core8vector3dIfEE
0059712c: ldr      r3, [r1]
00597130: ldr      r2, [r0, #0x11c]
00597134: str      r3, [r0, #0xac]
00597138: ldr      r3, [r1, #4]
0059713c: orr      r2, r2, #8
00597140: str      r3, [r0, #0xb0]
00597144: ldr      r3, [r1, #8]
00597148: str      r2, [r0, #0x11c]
0059714c: str      r3, [r0, #0xb4]
00597150: bx       lr

# _ZNK6glitch7collada18SAnimationAccessor8getValueEiPvRib
0066a1a8: push     {r4, r5, r6, r7, r8, lr}
0066a1ac: sub      sp, sp, #8
0066a1b0: mov      r7, r1
0066a1b4: mov      r6, r2
0066a1b8: mov      r5, r3
0066a1bc: mov      r8, r0
0066a1c0: ldrb     r4, [sp, #0x20]
0066a1c4: bl       #0x66a004
0066a1c8: mov      r1, r8
0066a1cc: ldr      ip, [r0]
0066a1d0: mov      r2, r7
0066a1d4: mov      r3, r6
0066a1d8: str      r5, [sp]
0066a1dc: str      r4, [sp, #4]
0066a1e0: mov      lr, pc
0066a1e4: ldr      pc, [ip, #0x68]
0066a1e8: add      sp, sp, #8
0066a1ec: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada35CAnimationSetTransformationTemplate24addTransformationTargetsEPNS0_10CSceneNodeE
006e24e8: push     {r4, r5, r6, lr}
006e24ec: mov      r4, r0
006e24f0: sub      sp, sp, #8
006e24f4: mov      r5, r1
006e24f8: mov      r0, #0x10
006e24fc: mov      r1, #0
006e2500: bl       #0x5341ac
006e2504: mov      r3, #0
006e2508: str      r0, [sp, #4]
006e250c: strb     r3, [r0]
006e2510: ldr      r3, [sp, #4]
006e2514: mov      r2, #1
006e2518: add      r6, r4, #4
006e251c: str      r2, [r3, #4]
006e2520: ldr      r3, [sp, #4]
006e2524: str      r5, [r3, #8]
006e2528: ldr      r1, [r4, #8]
006e252c: ldr      r3, [r4, #0xc]
006e2530: cmp      r1, r3
006e2534: beq      #0x6e261c
006e2538: ldr      r3, [sp, #4]
006e253c: str      r3, [r1]
006e2540: ldr      r3, [r4, #8]
006e2544: add      r3, r3, #4
006e2548: str      r3, [r4, #8]
006e254c: mov      r1, #0
006e2550: mov      r0, #0x10
006e2554: bl       #0x5341ac
006e2558: mov      r3, #0
006e255c: str      r0, [sp, #4]
006e2560: strb     r3, [r0]
006e2564: ldr      r3, [sp, #4]
006e2568: mov      r2, #5
006e256c: str      r2, [r3, #4]
006e2570: ldr      r3, [sp, #4]
006e2574: str      r5, [r3, #8]
006e2578: ldr      r1, [r4, #8]
006e257c: ldr      r3, [r4, #0xc]
006e2580: cmp      r1, r3
006e2584: beq      #0x6e262c
006e2588: ldr      r3, [sp, #4]
006e258c: str      r3, [r1]
006e2590: ldr      r3, [r4, #8]
006e2594: add      r3, r3, #4
006e2598: str      r3, [r4, #8]
006e259c: mov      r1, #0
006e25a0: mov      r0, #0x10
006e25a4: bl       #0x5341ac
006e25a8: mov      r3, #0
006e25ac: str      r0, [sp, #4]
006e25b0: strb     r3, [r0]
006e25b4: ldr      r3, [sp, #4]
006e25b8: mov      r2, #0xa
006e25bc: str      r2, [r3, #4]
006e25c0: ldr      r3, [sp, #4]
006e25c4: str      r5, [r3, #8]
006e25c8: ldr      r1, [r4, #8]
006e25cc: ldr      r3, [r4, #0xc]
006e25d0: cmp      r1, r3
006e25d4: beq      #0x6e263c
006e25d8: ldr      r3, [sp, #4]
006e25dc: str      r3, [r1]
006e25e0: ldr      r3, [r4, #8]
006e25e4: add      r3, r3, #4
006e25e8: str      r3, [r4, #8]
006e25ec: ldr      r6, [r5, #0xf4]!
006e25f0: b        #0x6e260c
006e25f4: cmp      r6, #0
006e25f8: moveq    r1, r6
006e25fc: subne    r1, r6, #4
006e2600: mov      r0, r4
006e2604: bl       #0x6e24e8
006e2608: ldr      r6, [r6]
006e260c: cmp      r5, r6
006e2610: bne      #0x6e25f4
006e2614: add      sp, sp, #8
006e2618: pop      {r4, r5, r6, pc}
006e261c: mov      r0, r6
006e2620: add      r2, sp, #4
006e2624: bl       #0x6e2434
006e2628: b        #0x6e254c
006e262c: mov      r0, r6
006e2630: add      r2, sp, #4
006e2634: bl       #0x6e2434
006e2638: b        #0x6e259c
006e263c: mov      r0, r6
006e2640: add      r2, sp, #4
006e2644: bl       #0x6e2434
006e2648: b        #0x6e25ec

# _ZN11AnimatorSetC1ERKN5boost13intrusive_ptrI12AnimationSetEE
003676b8: push     {r4, r5, r6, lr}
003676bc: ldr      r5, [pc, #0xbc]
003676c0: ldr      r3, [pc, #0xbc]
003676c4: mov      r2, #1
003676c8: add      r5, pc, r5
003676cc: ldr      r3, [r5, r3]
003676d0: str      r2, [r0, #0xa0]
003676d4: sub      sp, sp, #8
003676d8: add      r3, r3, #8
003676dc: str      r3, [r0, #0x9c]
003676e0: ldr      r3, [r1]
003676e4: mov      r6, r1
003676e8: ldr      r1, [pc, #0x98]
003676ec: ldr      r3, [r3, #0x20]
003676f0: mov      r4, r0
003676f4: ldr      r1, [r5, r1]
003676f8: cmp      r3, #0
003676fc: str      r3, [sp, #4]
00367700: ldrne    r2, [r3, #4]
00367704: add      r1, r1, #4
00367708: addne    r2, r2, #1
0036770c: strne    r2, [r3, #4]
00367710: add      r2, sp, #4
00367714: bl       #0x660ce4
00367718: ldr      r0, [sp, #4]
0036771c: cmp      r0, #0
00367720: beq      #0x367728
00367724: bl       #0x31d584
00367728: ldr      r3, [pc, #0x5c]
0036772c: add      r0, r4, #0x58
00367730: mov      r1, r4
00367734: ldr      r3, [r5, r3]
00367738: add      r2, r3, #0xa4
0036773c: add      ip, r3, #0xc
00367740: add      r3, r3, #0xc0
00367744: str      r2, [r4, #4]
00367748: str      r3, [r4, #0x9c]
0036774c: str      ip, [r4]
00367750: bl       #0x364398
00367754: ldr      r3, [r6]
00367758: mov      r0, r4
0036775c: cmp      r3, #0
00367760: str      r3, [r4, #0x94]
00367764: ldrne    r2, [r3, #4]
00367768: addne    r2, r2, #1
0036776c: strne    r2, [r3, #4]
00367770: mov      r3, #0
00367774: str      r3, [r4, #0x98]
00367778: add      sp, sp, #8
0036777c: pop      {r4, r5, r6, pc}
00367780: rsbeq    sp, r2, r8, asr #7
00367784: andeq    r2, r0, r4, asr #22
00367788: ldrdeq   r1, r2, [r0], -r4
0036778c: andeq    r1, r0, ip, asr #20

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE12getValueSizeEv
0060ee94: mov      r0, #4
0060ee98: bx       lr

# _ZN24BlendedAnimSetControllerC1EP13RootSceneNodei
00476ddc: push     {r4, r5, r6, r7, r8, sl, lr}
00476de0: mov      r5, r2
00476de4: sub      sp, sp, #0x14
00476de8: mov      r2, #1
00476dec: ldr      r7, [pc, #0x258]
00476df0: mov      r4, r0
00476df4: bl       #0x474e44
00476df8: ldr      r3, [pc, #0x250]
00476dfc: add      r7, pc, r7
00476e00: ldr      r2, [pc, #0x24c]
00476e04: ldr      r3, [r7, r3]
00476e08: mov      r8, #0
00476e0c: ldr      r6, [r7, r2]
00476e10: add      r3, r3, #8
00476e14: str      r3, [r4]
00476e18: mov      r3, #1
00476e1c: strb     r3, [r4, #0x10]
00476e20: mov      r1, r5
00476e24: str      r5, [r4, #8]
00476e28: mov      r0, r6
00476e2c: str      r8, [r4, #0xc]
00476e30: str      r8, [r4, #0x14]
00476e34: bl       #0x4762c4
00476e38: mov      r5, r0
00476e3c: ldr      r1, [r4, #8]
00476e40: mov      r0, r6
00476e44: bl       #0x4762c4
00476e48: subs     r3, r5, r8
00476e4c: movne    r3, #1
00476e50: subs     sl, r0, r8
00476e54: movne    sl, #1
00476e58: tst      sl, r3
00476e5c: mov      r6, r0
00476e60: bne      #0x476ea0
00476e64: cmp      r3, #0
00476e68: beq      #0x476e7c
00476e6c: ldr      r3, [r5]
00476e70: ldr      r0, [r3, #-0xc]
00476e74: add      r0, r5, r0
00476e78: bl       #0x31d584
00476e7c: cmp      sl, #0
00476e80: beq      #0x476e94
00476e84: ldr      r3, [r6]
00476e88: ldr      r0, [r3, #-0xc]
00476e8c: add      r0, r6, r0
00476e90: bl       #0x31d584
00476e94: mov      r0, r4
00476e98: add      sp, sp, #0x14
00476e9c: pop      {r4, r5, r6, r7, r8, sl, pc}
00476ea0: mov      r0, r5
00476ea4: bl       #0x65f11c
00476ea8: cmp      r0, r8
00476eac: ble      #0x476fd8
00476eb0: mov      r1, #0
00476eb4: mov      r0, #0xd0
00476eb8: bl       #0x5341ac
00476ebc: mov      r7, r0
00476ec0: bl       #0x367018
00476ec4: mov      r3, #1
00476ec8: str      r5, [sp, #0xc]
00476ecc: strb     r3, [r7, #0x24]
00476ed0: ldr      r3, [sp, #0xc]
00476ed4: add      r8, r7, #0x28
00476ed8: ldr      r2, [r3]
00476edc: ldr      r2, [r2, #-0xc]
00476ee0: add      r3, r3, r2
00476ee4: ldr      r2, [r3, #4]
00476ee8: add      r2, r2, #1
00476eec: str      r2, [r3, #4]
00476ef0: ldr      r1, [r7, #0x2c]
00476ef4: ldr      r3, [r7, #0x30]
00476ef8: cmp      r1, r3
00476efc: beq      #0x47702c
00476f00: ldr      r3, [sp, #0xc]
00476f04: str      r3, [r1]
00476f08: ldr      r3, [r7, #0x2c]
00476f0c: add      r3, r3, #4
00476f10: str      r3, [r7, #0x2c]
00476f14: mov      r3, #1
00476f18: str      r6, [sp, #0xc]
00476f1c: strb     r3, [r7, #0x24]
00476f20: ldr      r3, [sp, #0xc]
00476f24: ldr      r2, [r3]
00476f28: ldr      r2, [r2, #-0xc]
00476f2c: add      r3, r3, r2
00476f30: ldr      r2, [r3, #4]
00476f34: add      r2, r2, #1
00476f38: str      r2, [r3, #4]
00476f3c: ldr      r1, [r7, #0x2c]
00476f40: ldr      r3, [r7, #0x30]
00476f44: cmp      r1, r3
00476f48: beq      #0x47703c
00476f4c: ldr      r3, [sp, #0xc]
00476f50: str      r3, [r1]
00476f54: ldr      r3, [r7, #0x2c]
00476f58: add      r3, r3, #4
00476f5c: str      r3, [r7, #0x2c]
00476f60: mov      r0, r7
00476f64: ldr      r3, [r7]
00476f68: mov      r1, #0
00476f6c: mov      lr, pc
00476f70: ldr      pc, [r3, #0x88]
00476f74: ldr      r3, [r7, #0x34]
00476f78: mov      r2, #0x3f800000
00476f7c: mov      r1, r7
00476f80: str      r2, [r3]
00476f84: ldr      r3, [r7, #0x34]
00476f88: mov      r2, #0
00476f8c: str      r2, [r3, #4]
00476f90: ldr      r3, [r4, #4]
00476f94: mov      r0, r3
00476f98: ldr      r3, [r3]
00476f9c: mov      lr, pc
00476fa0: ldr      pc, [r3, #0x6c]
00476fa4: ldr      r3, [r5]
00476fa8: ldr      r0, [r3, #-0xc]
00476fac: add      r0, r5, r0
00476fb0: bl       #0x31d584
00476fb4: ldr      r3, [r6]
00476fb8: ldr      r0, [r3, #-0xc]
00476fbc: add      r0, r6, r0
00476fc0: bl       #0x31d584
00476fc4: ldr      r3, [r7]
00476fc8: ldr      r0, [r3, #-0xc]
00476fcc: add      r0, r7, r0
00476fd0: bl       #0x31d584
00476fd4: b        #0x476e94
00476fd8: ldr      r3, [pc, #0x78]
00476fdc: ldr      r3, [r7, r3]
00476fe0: ldr      r3, [r3]
00476fe4: cmp      r3, #2
00476fe8: streq    r8, [r8]
00476fec: beq      #0x476eb0
00476ff0: cmp      r3, #1
00476ff4: bne      #0x476eb0
00476ff8: ldr      r0, [pc, #0x5c]
00476ffc: ldr      r1, [pc, #0x5c]
00477000: ldr      r2, [pc, #0x5c]
00477004: ldr      r0, [r7, r0]
00477008: ldr      r3, [pc, #0x58]
0047700c: mov      ip, #0x45
00477010: add      r1, pc, r1
00477014: add      r2, pc, r2
00477018: add      r3, pc, r3
0047701c: add      r0, r0, #0xa8
00477020: str      ip, [sp]
00477024: bl       #0x30e004
00477028: b        #0x476eb0
0047702c: mov      r0, r8
00477030: add      r2, sp, #0xc
00477034: bl       #0x476a9c
00477038: b        #0x476f14
0047703c: mov      r0, r8
00477040: add      r2, sp, #0xc
00477044: bl       #0x476a9c
00477048: b        #0x476f60

# _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEE19applyBlendedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
006284a4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006284a8: cmp      r2, #1
006284ac: sub      sp, sp, #0x1c
006284b0: mov      sl, #0
006284b4: mov      r4, r2
006284b8: mov      r5, r1
006284bc: str      r3, [sp, #4]
006284c0: str      sl, [sp, #0x14]
006284c4: beq      #0x628578
006284c8: cmp      r2, #0
006284cc: moveq    fp, sl
006284d0: moveq    sb, sl
006284d4: beq      #0x628550
006284d8: mov      r6, r0
006284dc: mov      r8, #0
006284e0: mov      fp, sl
006284e4: mov      sb, sl
006284e8: ldr      r7, [r5, r8]
006284ec: ldr      r1, [r6]
006284f0: add      r8, r8, #4
006284f4: mov      r0, r7
006284f8: bl       #0x30ed6c
006284fc: mov      r1, r0
00628500: mov      r0, sl
00628504: bl       #0x30eba4
00628508: ldr      r1, [r6, #4]
0062850c: mov      sl, r0
00628510: mov      r0, r7
00628514: bl       #0x30ed6c
00628518: mov      r1, r0
0062851c: mov      r0, fp
00628520: bl       #0x30eba4
00628524: ldr      r1, [r6, #8]
00628528: mov      fp, r0
0062852c: mov      r0, r7
00628530: bl       #0x30ed6c
00628534: mov      r1, r0
00628538: mov      r0, sb
0062853c: bl       #0x30eba4
00628540: subs     r4, r4, #1
00628544: mov      sb, r0
00628548: add      r6, r6, #0xc
0062854c: bne      #0x6284e8
00628550: add      r1, sp, #0x18
00628554: str      sl, [r1, #-0xc]!
00628558: str      fp, [sp, #0x10]
0062855c: str      sb, [r1, #8]
00628560: ldr      r0, [sp, #4]
00628564: ldr      r3, [r0]
00628568: mov      lr, pc
0062856c: ldr      pc, [r3, #0xa4]
00628570: add      sp, sp, #0x1c
00628574: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00628578: mov      r3, r0
0062857c: ldr      ip, [r3], #4
00628580: ldr      r2, [r0, #4]
00628584: add      r1, sp, #0x18
00628588: ldr      r3, [r3, #4]
0062858c: str      ip, [r1, #-0xc]!
00628590: str      r2, [sp, #0x10]
00628594: str      r3, [r1, #8]
00628598: b        #0x628560

# _ZN6glitch7collada15animation_track11CVector3dEx17getBlendedValueExEPvPfiS3_
006e3fb0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006e3fb4: cmp      r2, #2
006e3fb8: sub      sp, sp, #0x1c
006e3fbc: str      r2, [sp, #0x10]
006e3fc0: str      r0, [sp, #0xc]
006e3fc4: mov      r2, r1
006e3fc8: str      r3, [sp, #0x14]
006e3fcc: ble      #0x6e40e0
006e3fd0: ldr      r8, [r0]
006e3fd4: ldr      r7, [r0, #4]
006e3fd8: ldr      sl, [r0, #8]
006e3fdc: ldr      sb, [r1]
006e3fe0: mov      r5, #0xc
006e3fe4: mov      r4, #1
006e3fe8: ldr      r6, [r2, r4, lsl #2]
006e3fec: ldr      r3, [sp, #0xc]
006e3ff0: mov      r0, sb
006e3ff4: mov      r1, r6
006e3ff8: str      r2, [sp]
006e3ffc: add      fp, r3, r5
006e4000: bl       #0x30eba4
006e4004: mov      sb, r0
006e4008: mov      r1, sb
006e400c: mov      r0, r6
006e4010: bl       #0x30ec94
006e4014: ldr      r3, [sp, #0xc]
006e4018: mov      r6, r0
006e401c: mov      r1, r8
006e4020: ldr      r0, [r3, r5]
006e4024: bl       #0x30e3ac
006e4028: mov      r1, r0
006e402c: mov      r0, r6
006e4030: bl       #0x30ed6c
006e4034: mov      r1, r7
006e4038: mov      r3, r0
006e403c: ldr      r0, [fp, #4]
006e4040: str      r3, [sp, #4]
006e4044: bl       #0x30e3ac
006e4048: mov      r1, r0
006e404c: mov      r0, r6
006e4050: bl       #0x30ed6c
006e4054: mov      r1, sl
006e4058: mov      ip, r0
006e405c: ldr      r0, [fp, #8]
006e4060: str      ip, [sp, #8]
006e4064: bl       #0x30e3ac
006e4068: mov      r1, r0
006e406c: mov      r0, r6
006e4070: bl       #0x30ed6c
006e4074: ldr      r3, [sp, #4]
006e4078: mov      r6, r0
006e407c: mov      r0, r8
006e4080: mov      r1, r3
006e4084: bl       #0x30eba4
006e4088: ldr      ip, [sp, #8]
006e408c: mov      r8, r0
006e4090: mov      r0, r7
006e4094: mov      r1, ip
006e4098: bl       #0x30eba4
006e409c: mov      r1, r6
006e40a0: mov      r7, r0
006e40a4: mov      r0, sl
006e40a8: bl       #0x30eba4
006e40ac: ldr      r3, [sp, #0x10]
006e40b0: add      r4, r4, #1
006e40b4: mov      sl, r0
006e40b8: cmp      r4, r3
006e40bc: add      r5, r5, #0xc
006e40c0: ldr      r2, [sp]
006e40c4: bne      #0x6e3fe8
006e40c8: ldr      r2, [sp, #0x14]
006e40cc: str      r0, [r2, #8]
006e40d0: str      r8, [r2]
006e40d4: str      r7, [r2, #4]
006e40d8: add      sp, sp, #0x1c
006e40dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006e40e0: beq      #0x6e4124
006e40e4: ldr      r2, [sp, #0x10]
006e40e8: cmp      r2, #1
006e40ec: bne      #0x6e40d8
006e40f0: ldr      r2, [sp, #0xc]
006e40f4: ldr      r3, [r2]
006e40f8: ldr      r2, [sp, #0x14]
006e40fc: str      r3, [r2]
006e4100: ldr      r2, [sp, #0xc]
006e4104: ldr      r3, [r2, #4]
006e4108: ldr      r2, [sp, #0x14]
006e410c: str      r3, [r2, #4]
006e4110: ldr      r2, [sp, #0xc]
006e4114: ldr      r3, [r2, #8]
006e4118: ldr      r2, [sp, #0x14]
006e411c: str      r3, [r2, #8]
006e4120: b        #0x6e40d8
006e4124: ldr      r4, [r1, #4]
006e4128: ldr      r3, [sp, #0xc]
006e412c: ldr      r1, [r1]
006e4130: mov      r0, r4
006e4134: add      r5, r3, #0xc
006e4138: bl       #0x30eba4
006e413c: mov      r1, r0
006e4140: mov      r0, r4
006e4144: bl       #0x30ec94
006e4148: ldr      r2, [sp, #0xc]
006e414c: mov      r4, r0
006e4150: ldr      r0, [r5, #4]
006e4154: ldr      r7, [r2, #4]
006e4158: ldr      r6, [r2, #8]
006e415c: mov      r1, r7
006e4160: bl       #0x30e3ac
006e4164: mov      r1, r0
006e4168: mov      r0, r4
006e416c: bl       #0x30ed6c
006e4170: mov      r1, r0
006e4174: mov      r0, r7
006e4178: bl       #0x30eba4
006e417c: ldr      r3, [sp, #0xc]
006e4180: mov      r1, r6
006e4184: mov      r7, r0
006e4188: ldr      r0, [r5, #8]
006e418c: ldr      r5, [r3]
006e4190: bl       #0x30e3ac
006e4194: mov      r1, r0
006e4198: mov      r0, r4
006e419c: bl       #0x30ed6c
006e41a0: mov      r1, r0
006e41a4: mov      r0, r6
006e41a8: bl       #0x30eba4
006e41ac: ldr      r2, [sp, #0xc]
006e41b0: mov      r6, r0
006e41b4: mov      r1, r5
006e41b8: ldr      r0, [r2, #0xc]
006e41bc: bl       #0x30e3ac
006e41c0: mov      r1, r0
006e41c4: mov      r0, r4
006e41c8: bl       #0x30ed6c
006e41cc: mov      r1, r0
006e41d0: mov      r0, r5
006e41d4: bl       #0x30eba4
006e41d8: ldr      r3, [sp, #0x14]
006e41dc: str      r6, [r3, #8]
006e41e0: str      r0, [r3]
006e41e4: str      r7, [r3, #4]
006e41e8: b        #0x6e40d8

# _ZN6glitch7collada21CSceneNodeAnimatorSet19setCurrentAnimationEi
0065f8c8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0065f8cc: mov      r4, r0
0065f8d0: mov      r5, r1
0065f8d4: bl       #0x65f0fc
0065f8d8: ldr      r3, [r4, #0x24]
0065f8dc: str      r0, [r4, #0x14]
0065f8e0: mov      r1, r5
0065f8e4: ldr      r2, [r3, #0x3c]
0065f8e8: mov      r0, r3
0065f8ec: str      r5, [r4, #0x50]
0065f8f0: mul      r3, r2, r5
0065f8f4: str      r3, [r4, #0x4c]
0065f8f8: bl       #0x65f0b4
0065f8fc: bl       #0x60e334
0065f900: ldr      r3, [r4]
0065f904: mov      r6, r0
0065f908: mov      r0, r4
0065f90c: mov      lr, pc
0065f910: ldr      pc, [r3, #0x44]
0065f914: cmp      r0, #0
0065f918: beq      #0x65f9fc
0065f91c: ldr      r8, [r6]
0065f920: cmp      r8, #0
0065f924: beq      #0x65f968
0065f928: ldr      r3, [r4]
0065f92c: mov      r0, r4
0065f930: mov      lr, pc
0065f934: ldr      pc, [r3, #0x44]
0065f938: str      r6, [r0, #0x34]
0065f93c: ldr      r2, [r6]
0065f940: cmp      r2, #0
0065f944: moveq    r1, #1
0065f948: streq    r1, [r0, #0x14]
0065f94c: streq    r2, [r0, #0x10]
0065f950: beq      #0x65f9d4
0065f954: ldr      r3, [r0]
0065f958: mov      r1, #0
0065f95c: mov      lr, pc
0065f960: ldr      pc, [r3, #0x10]
0065f964: b        #0x65f9d4
0065f968: ldr      r3, [r4]
0065f96c: mov      r0, r4
0065f970: mov      lr, pc
0065f974: ldr      pc, [r3, #0x44]
0065f978: mov      r7, #1
0065f97c: str      r8, [r0, #0x10]
0065f980: str      r8, [r0, #0x34]
0065f984: str      r7, [r0, #0x14]
0065f988: ldr      r3, [r4]
0065f98c: mov      r0, r4
0065f990: mov      lr, pc
0065f994: ldr      pc, [r3, #0x44]
0065f998: ldr      r3, [r0]
0065f99c: mov      r8, r0
0065f9a0: mov      r1, r5
0065f9a4: mov      r0, r4
0065f9a8: ldr      r6, [r3, #0x50]
0065f9ac: bl       #0x65f104
0065f9b0: mov      r1, r5
0065f9b4: mov      sl, r0
0065f9b8: mov      r0, r4
0065f9bc: bl       #0x65f10c
0065f9c0: mov      r1, sl
0065f9c4: mov      r2, r0
0065f9c8: mov      r3, r7
0065f9cc: mov      r0, r8
0065f9d0: blx      r6
0065f9d4: mov      r1, r5
0065f9d8: ldr      r0, [r4, #0x24]
0065f9dc: bl       #0x65f0b4
0065f9e0: ldr      r3, [r0]
0065f9e4: mov      r0, r4
0065f9e8: ldr      r3, [r3, #0x24]
0065f9ec: ldr      r3, [r3, #0x20]
0065f9f0: ldr      r1, [r3, #0x2c]
0065f9f4: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
0065f9f8: b        #0x60fab8
0065f9fc: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN14AnimApplicator14CalculateDeltaEjRKN6glitch4core8vector3dIfEE
00364444: push     {r4, r5, r6, r7, r8, lr}
00364448: ldr      r3, [r0, #0x14]
0036444c: mov      r4, r0
00364450: mov      r5, r1
00364454: cmp      r3, r1
00364458: mov      r6, r2
0036445c: beq      #0x3644b8
00364460: ldr      r0, [r2, #4]
00364464: ldr      r1, [r4, #0x1c]
00364468: bl       #0x30e3ac
0036446c: ldr      r1, [r4, #0x20]
00364470: mov      r8, r0
00364474: ldr      r0, [r6, #8]
00364478: bl       #0x30e3ac
0036447c: ldr      r1, [r4, #0x18]
00364480: mov      r7, r0
00364484: ldr      r0, [r6]
00364488: bl       #0x30e3ac
0036448c: str      r8, [r4, #0x28]
00364490: str      r0, [r4, #0x24]
00364494: str      r7, [r4, #0x2c]
00364498: ldr      r3, [r6]
0036449c: str      r3, [r4, #0x18]
003644a0: ldr      r3, [r6, #4]
003644a4: str      r3, [r4, #0x1c]
003644a8: ldr      r3, [r6, #8]
003644ac: str      r5, [r4, #0x14]
003644b0: str      r3, [r4, #0x20]
003644b4: pop      {r4, r5, r6, r7, r8, pc}
003644b8: mov      r3, #0
003644bc: str      r3, [r0, #0x2c]
003644c0: str      r3, [r0, #0x24]
003644c4: str      r3, [r0, #0x28]
003644c8: b        #0x364498

# _ZN6glitch7collada13CAnimationSet15CompileInternalEv
006605ac: push     {r4, r5, r6, lr}
006605b0: ldr      r3, [r0, #0x24]
006605b4: ldr      r1, [r0, #0x28]
006605b8: add      r6, r0, #0x40
006605bc: mov      r4, r0
006605c0: rsb      r1, r3, r1
006605c4: sub      sp, sp, #0x10
006605c8: mov      r0, r6
006605cc: asr      r1, r1, #3
006605d0: bl       #0x65fe8c
006605d4: ldr      r3, [r4, #0x24]
006605d8: ldr      r1, [r4, #0x28]
006605dc: add      r2, sp, #0x10
006605e0: mov      r5, #0
006605e4: rsb      r1, r3, r1
006605e8: str      r5, [r2, #-4]!
006605ec: mov      r0, r6
006605f0: asr      r1, r1, #3
006605f4: bl       #0x660568
006605f8: ldr      r3, [r4, #0x24]
006605fc: ldr      r1, [r4, #0x28]
00660600: add      r6, r4, #0x4c
00660604: mov      r0, r6
00660608: rsb      r1, r3, r1
0066060c: asr      r1, r1, #3
00660610: bl       #0x65fe8c
00660614: ldr      r3, [r4, #0x24]
00660618: ldr      r1, [r4, #0x28]
0066061c: add      r2, sp, #0x10
00660620: str      r5, [r2, #-8]!
00660624: rsb      r1, r3, r1
00660628: mov      r0, r6
0066062c: asr      r1, r1, #3
00660630: bl       #0x660568
00660634: ldr      r3, [r4, #0x24]
00660638: ldr      r1, [r4, #0x28]
0066063c: add      r6, r4, #0x58
00660640: mov      r0, r6
00660644: rsb      r1, r3, r1
00660648: asr      r1, r1, #3
0066064c: bl       #0x65fe8c
00660650: ldr      r3, [r4, #0x24]
00660654: ldr      r1, [r4, #0x28]
00660658: add      r2, sp, #0x10
0066065c: str      r5, [r2, #-0xc]!
00660660: rsb      r1, r3, r1
00660664: mov      r0, r6
00660668: asr      r1, r1, #3
0066066c: bl       #0x660568
00660670: ldr      r2, [r4, #0x28]
00660674: ldr      r3, [r4, #0x24]
00660678: rsb      r3, r3, r2
0066067c: lsrs     r3, r3, #3
00660680: beq      #0x660708
00660684: mvn      ip, #0x80000000
00660688: mov      r0, #0x80000000
0066068c: ldr      r3, [r4, #0x40]
00660690: str      ip, [r3, r5, lsl #2]
00660694: ldr      r3, [r4, #0x4c]
00660698: str      r0, [r3, r5, lsl #2]
0066069c: ldr      r2, [r4, #0x24]
006606a0: ldr      r3, [r4, #0x40]
006606a4: ldr      r2, [r2, r5, lsl #3]
006606a8: ldr      r2, [r2, #0x24]
006606ac: ldr      r2, [r2, #0x20]
006606b0: ldr      r2, [r2, #0x1c]
006606b4: str      r2, [r3, r5, lsl #2]
006606b8: ldr      r2, [r4, #0x24]
006606bc: ldr      r3, [r4, #0x4c]
006606c0: ldr      r2, [r2, r5, lsl #3]
006606c4: ldr      r2, [r2, #0x24]
006606c8: ldr      r2, [r2, #0x20]
006606cc: ldr      r2, [r2, #0x20]
006606d0: str      r2, [r3, r5, lsl #2]
006606d4: ldr      r1, [r4, #0x4c]
006606d8: ldr      r2, [r4, #0x40]
006606dc: ldr      r3, [r4, #0x58]
006606e0: ldr      r1, [r1, r5, lsl #2]
006606e4: ldr      r2, [r2, r5, lsl #2]
006606e8: rsb      r2, r2, r1
006606ec: str      r2, [r3, r5, lsl #2]
006606f0: ldr      r2, [r4, #0x28]
006606f4: ldr      r3, [r4, #0x24]
006606f8: add      r5, r5, #1
006606fc: rsb      r3, r3, r2
00660700: cmp      r5, r3, asr #3
00660704: blo      #0x66068c
00660708: add      sp, sp, #0x10
0066070c: pop      {r4, r5, r6, pc}

# _ZN15AnimatorBlender11animateNodeEPN6glitch5scene10ISceneNodeEj
00366cb4: push     {r4, r5, r6, lr}
00366cb8: ldr      r3, [r0, #0x7c]
00366cbc: mov      r4, r0
00366cc0: mov      r5, r2
00366cc4: cmp      r3, #0
00366cc8: ldr      r0, [r0, #0x84]
00366ccc: blt      #0x366d18
00366cd0: rsb      r0, r0, r2
00366cd4: rsb      r0, r0, r3
00366cd8: cmp      r0, #0
00366cdc: str      r0, [r4, #0x7c]
00366ce0: ble      #0x366d6c
00366ce4: bl       #0x30e964
00366ce8: ldr      r1, [r4, #0x80]
00366cec: bl       #0x30ed6c
00366cf0: ldr      r2, [r4, #0x34]
00366cf4: ldr      ip, [r4, #0x74]
00366cf8: mov      r3, r0
00366cfc: mov      r1, r0
00366d00: str      r3, [r2, ip, lsl #2]
00366d04: mov      r0, #0x3f800000
00366d08: bl       #0x30e3ac
00366d0c: ldr      r2, [r4, #0x70]
00366d10: ldr      r3, [r4, #0x34]
00366d14: str      r0, [r3, r2, lsl #2]
00366d18: ldr      r3, [r4]
00366d1c: mov      r0, r4
00366d20: mov      r1, r5
00366d24: add      r6, r4, #0x88
00366d28: mov      lr, pc
00366d2c: ldr      pc, [r3, #0x50]
00366d30: mov      r1, r5
00366d34: mov      r0, r6
00366d38: bl       #0x366888
00366d3c: ldr      r2, [r4, #0x70]
00366d40: ldr      r3, [r4, #0x28]
00366d44: ldr      r3, [r3, r2, lsl #2]
00366d48: mov      r0, r3
00366d4c: ldr      r3, [r3]
00366d50: mov      lr, pc
00366d54: ldr      pc, [r3, #0x44]
00366d58: mov      r1, r0
00366d5c: mov      r0, r6
00366d60: bl       #0x36440c
00366d64: str      r5, [r4, #0x84]
00366d68: pop      {r4, r5, r6, pc}
00366d6c: ldr      r2, [r4, #0x74]
00366d70: ldr      r3, [r4, #0x34]
00366d74: mov      r1, #0
00366d78: str      r1, [r3, r2, lsl #2]
00366d7c: ldr      r2, [r4, #0x70]
00366d80: ldr      r3, [r4, #0x34]
00366d84: mov      r1, #0x3f800000
00366d88: str      r1, [r3, r2, lsl #2]
00366d8c: b        #0x366d18

# _ZN11AnimatorSet12SetCallbacksEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
00367510: push     {r4, r5, r6, r7, r8, lr}
00367514: mov      r4, r0
00367518: ldr      r0, [r0, #0x18]
0036751c: ldr      ip, [sp, #0x18]
00367520: str      r3, [r4, #0x1c]
00367524: cmp      r0, #0
00367528: str      ip, [r4, #0x20]
0036752c: strne    ip, [r0, #0xc]
00367530: strne    r3, [r0, #8]
00367534: ldr      r3, [r4]
00367538: mov      r0, r4
0036753c: mov      r7, r1
00367540: mov      r6, r2
00367544: mov      lr, pc
00367548: ldr      pc, [r3, #0x44]
0036754c: ldr      r5, [pc, #0x2c]
00367550: cmp      r0, #0
00367554: add      r5, pc, r5
00367558: beq      #0x36756c
0036755c: ldr      r3, [pc, #0x20]
00367560: str      r4, [r0, #0xc]
00367564: ldr      r3, [r5, r3]
00367568: str      r3, [r0, #8]
0036756c: add      r0, r4, #0x58
00367570: mov      r1, r7
00367574: mov      r2, r6
00367578: pop      {r4, r5, r6, r7, r8, lr}
0036757c: b        #0x364400
00367580: rsbeq    sp, r2, ip, lsr r5
00367584: strheq   r0, [r0], -r0

# _ZN15AnimatorBlenderC2Ev
003670c8: push     {r4, r5, r6, lr}
003670cc: mov      r6, r1
003670d0: ldr      r5, [pc, #0x78]
003670d4: add      r1, r1, #4
003670d8: mov      r4, r0
003670dc: bl       #0x366f7c
003670e0: ldr      r2, [r6]
003670e4: ldr      r3, [pc, #0x68]
003670e8: add      r5, pc, r5
003670ec: str      r2, [r4]
003670f0: ldr      r3, [r5, r3]
003670f4: ldr      r1, [r2, #-0xc]
003670f8: ldr      r0, [r6, #0x24]
003670fc: add      r2, r3, #0xa0
00367100: mov      r3, #0
00367104: str      r0, [r4, r1]
00367108: str      r2, [r4, #4]
0036710c: mov      r2, #0
00367110: str      r3, [r4, #0x84]
00367114: str      r3, [r4, #0x70]
00367118: str      r3, [r4, #0x74]
0036711c: str      r3, [r4, #0x78]
00367120: str      r3, [r4, #0x7c]
00367124: str      r2, [r4, #0x80]
00367128: add      r0, r4, #0x88
0036712c: mov      r1, r4
00367130: bl       #0x364330
00367134: ldr      r3, [pc, #0x1c]
00367138: str      r4, [r4, #0xc4]
0036713c: mov      r0, r4
00367140: ldr      r3, [r5, r3]
00367144: add      r3, r3, #8
00367148: str      r3, [r4, #0x88]
0036714c: pop      {r4, r5, r6, pc}
00367150: rsbeq    sp, r2, r8, lsr #19
00367154: andeq    r3, r0, r4, lsr #6
00367158: andeq    r4, r0, r0, ror #21

# _ZN15AnimatorBlender17_HandleAnimEndingEPN6glitch5scene19ITimelineControllerE
00366628: push     {r4, r5, r6, lr}
0036662c: ldr      r2, [r0, #0x70]
00366630: ldr      r3, [r0, #0x28]
00366634: mov      r4, r0
00366638: mov      r6, r1
0036663c: ldr      r3, [r3, r2, lsl #2]
00366640: mov      r0, r3
00366644: ldr      r3, [r3]
00366648: mov      lr, pc
0036664c: ldr      pc, [r3, #0x44]
00366650: cmp      r0, r6
00366654: mov      r5, r0
00366658: beq      #0x366660
0036665c: pop      {r4, r5, r6, pc}
00366660: cmp      r0, #0
00366664: beq      #0x3666bc
00366668: mov      r1, #0x44000000
0036666c: add      r1, r1, #0x7a0000
00366670: ldr      r0, [r0, #0x1c]
00366674: bl       #0x30ed6c
00366678: bl       #0x30e4cc
0036667c: mov      r1, #0x44000000
00366680: mov      r6, r0
00366684: add      r1, r1, #0x7a0000
00366688: ldr      r0, [r5, #0x2c]
0036668c: bl       #0x30ed6c
00366690: bl       #0x30e4cc
00366694: ldr      r3, [r5, #4]
00366698: rsb      r0, r3, r0
0036669c: cmp      r0, r6
003666a0: movge    r3, #0
003666a4: movlt    r3, #1
003666a8: cmp      r0, #0
003666ac: movlt    r3, #0
003666b0: cmp      r3, #0
003666b4: rsbne    r3, r0, r6
003666b8: str      r3, [r4, #0x98]
003666bc: mov      r3, #1
003666c0: strb     r3, [r4, #0xb8]
003666c4: pop      {r4, r5, r6, pc}

# _ZN13RootSceneNode7NewAnimEb
0035d624: push     {r4, lr}
0035d628: mov      r4, r0
0035d62c: bl       #0x35d4cc
0035d630: ldrb     r3, [r4, #0x1ec]
0035d634: cmp      r3, #0
0035d638: beq      #0x35d654
0035d63c: ldr      r3, [r4, #0x1f0]
0035d640: cmp      r3, #0
0035d644: beq      #0x35d654
0035d648: ldr      r1, [r4, #0x1fc]
0035d64c: cmp      r1, #0
0035d650: bne      #0x35d658
0035d654: pop      {r4, pc}
0035d658: mov      r0, r4
0035d65c: add      r1, r1, #1
0035d660: bl       #0x35ce6c
0035d664: mov      r0, r4
0035d668: ldr      r3, [r4]
0035d66c: ldr      r1, [r4, #0x1fc]
0035d670: mov      lr, pc
0035d674: ldr      pc, [r3, #0x14]
0035d678: pop      {r4, pc}

# _ZN6glitch7collada21CSceneNodeAnimatorSet17getAnimationValueEiiPv
0065f7b4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0065f7b8: mov      r4, r0
0065f7bc: mov      sb, r2
0065f7c0: ldr      r2, [r4, #0xc]
0065f7c4: sub      sp, sp, #0x34
0065f7c8: mov      r5, r1
0065f7cc: ldr      r0, [r0, #0x24]
0065f7d0: ldr      r1, [r4, #0x50]
0065f7d4: mov      sl, r3
0065f7d8: str      r2, [sp, #0xc]
0065f7dc: bl       #0x65f0b4
0065f7e0: ldr      r6, [r4, #0x4c]
0065f7e4: ldr      r3, [r4, #0x24]
0065f7e8: mov      r1, #0xc
0065f7ec: ldr      r2, [r0]
0065f7f0: add      r6, r5, r6
0065f7f4: mul      r6, r1, r6
0065f7f8: ldr      r7, [r3, #0x30]
0065f7fc: ldr      r2, [r2, #0x24]
0065f800: add      r8, r7, r6
0065f804: ldr      r1, [r8, #4]
0065f808: ldr      r2, [r2, #0x20]
0065f80c: cmp      r1, #0
0065f810: ldr      fp, [r2, #0x14]
0065f814: beq      #0x65f844
0065f818: ldr      r3, [r3, #0x18]
0065f81c: ldr      r3, [r3, r5, lsl #2]
0065f820: mov      r0, r3
0065f824: ldr      r3, [r3]
0065f828: str      r1, [sp, #8]
0065f82c: mov      lr, pc
0065f830: ldr      pc, [r3, #8]
0065f834: ldr      r1, [sp, #8]
0065f838: mov      r2, r0
0065f83c: mov      r0, sl
0065f840: bl       #0x30e868
0065f844: ldr      r3, [r7, r6]
0065f848: cmp      r3, #2
0065f84c: bne      #0x65f8a4
0065f850: mov      r3, #0
0065f854: mov      r1, sb
0065f858: mov      r0, r4
0065f85c: strb     r3, [sp, #0x21]
0065f860: bl       #0x65f364
0065f864: ldr      r3, [r8, #8]
0065f868: str      r0, [sp, #0x28]
0065f86c: ldr      r2, [sp, #0xc]
0065f870: str      r3, [sp, #0x24]
0065f874: add      r3, sp, #0x14
0065f878: str      r3, [sp, #0x2c]
0065f87c: ldr      r3, [r4, #0x40]
0065f880: cmp      fp, #0
0065f884: mov      r1, sb
0065f888: addeq    r3, r3, r5, lsl #2
0065f88c: add      r0, sp, #0x24
0065f890: subs     ip, r2, #1
0065f894: movne    ip, #1
0065f898: mov      r2, sl
0065f89c: str      ip, [sp]
0065f8a0: bl       #0x66a1a8
0065f8a4: add      sp, sp, #0x34
0065f8a8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada25CSceneNodeAnimatorBlender16normalizeWeightsEv
00366594: push     {r4, r5, r6, r7, r8, lr}
00366598: ldr      r5, [r0, #0x34]
0036659c: ldr      r7, [r0, #0x38]
003665a0: mov      r8, r0
003665a4: rsb      r7, r5, r7
003665a8: asrs     r7, r7, #2
003665ac: beq      #0x36660c
003665b0: mov      r6, #0
003665b4: mov      r4, #0
003665b8: mov      r0, r6
003665bc: ldr      r1, [r5, r4, lsl #2]
003665c0: bl       #0x30eba4
003665c4: add      r4, r4, #1
003665c8: cmp      r4, r7
003665cc: mov      r6, r0
003665d0: bne      #0x3665b8
003665d4: mov      r1, #0
003665d8: bl       #0x30df8c
003665dc: cmp      r0, #0
003665e0: bne      #0x366610
003665e4: mov      r4, #0
003665e8: b        #0x3665f0
003665ec: ldr      r5, [r8, #0x34]
003665f0: ldr      r0, [r5, r4, lsl #2]
003665f4: mov      r1, r6
003665f8: bl       #0x30ec94
003665fc: str      r0, [r5, r4, lsl #2]
00366600: add      r4, r4, #1
00366604: cmp      r4, r7
00366608: bne      #0x3665ec
0036660c: pop      {r4, r5, r6, r7, r8, pc}
00366610: cmp      r4, #0
00366614: movne    r3, #0x3f800000
00366618: strne    r3, [r5]
0036661c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada25CSceneNodeAnimatorBlender22computeAnimationValuesEj
0065e48c: push     {r4, r5, r6, r7, r8, lr}
0065e490: ldr      r6, [r0, #0x28]
0065e494: ldr      r7, [r0, #0x2c]
0065e498: sub      sp, sp, #8
0065e49c: mov      r4, r0
0065e4a0: rsb      r3, r6, r7
0065e4a4: lsrs     r3, r3, #2
0065e4a8: mov      r8, r1
0065e4ac: beq      #0x65e510
0065e4b0: mov      r5, #0
0065e4b4: b        #0x65e4c8
0065e4b8: add      r5, r5, #1
0065e4bc: rsb      r3, r6, r7
0065e4c0: cmp      r5, r3, asr #2
0065e4c4: bhs      #0x65e510
0065e4c8: ldr      r3, [r4, #0x34]
0065e4cc: mov      r1, #0
0065e4d0: ldr      r0, [r3, r5, lsl #2]
0065e4d4: bl       #0x30df8c
0065e4d8: cmp      r0, #0
0065e4dc: bne      #0x65e4b8
0065e4e0: ldr      r3, [r6, r5, lsl #2]
0065e4e4: mov      r1, r8
0065e4e8: add      r5, r5, #1
0065e4ec: mov      r0, r3
0065e4f0: ldr      r3, [r3]
0065e4f4: mov      lr, pc
0065e4f8: ldr      pc, [r3, #0x4c]
0065e4fc: ldr      r6, [r4, #0x28]
0065e500: ldr      r7, [r4, #0x2c]
0065e504: rsb      r3, r6, r7
0065e508: cmp      r5, r3, asr #2
0065e50c: blo      #0x65e4c8
0065e510: mov      r0, r4
0065e514: bl       #0x366594
0065e518: ldr      r2, [r4, #0x5c]
0065e51c: ldr      r3, [r4, #0x58]
0065e520: rsb      r3, r3, r2
0065e524: lsrs     r3, r3, #2
0065e528: beq      #0x65e5c0
0065e52c: mov      r5, #0
0065e530: mov      r1, r5
0065e534: ldr      r3, [r4]
0065e538: mov      r0, r4
0065e53c: mov      lr, pc
0065e540: ldr      pc, [r3, #0x80]
0065e544: cmp      r0, #0
0065e548: beq      #0x65e5a8
0065e54c: ldr      r3, [r4, #0x58]
0065e550: mov      r1, r5
0065e554: ldr      r2, [r3, r5, lsl #2]
0065e558: cmp      r2, #0
0065e55c: beq      #0x65e5ac
0065e560: ldr      r3, [r4, #0x28]
0065e564: ldr      r3, [r3]
0065e568: mov      r0, r3
0065e56c: ldr      r3, [r3]
0065e570: mov      lr, pc
0065e574: ldr      pc, [r3, #0x58]
0065e578: ldr      ip, [r4, #0x58]
0065e57c: ldr      r2, [r4, #0x34]
0065e580: ldr      r3, [r4, #0x38]
0065e584: ldr      r1, [r4, #0x4c]
0065e588: ldr      lr, [ip, r5, lsl #2]
0065e58c: rsb      r3, r2, r3
0065e590: ldr      r1, [r1, r5, lsl #2]
0065e594: ldr      ip, [r0]
0065e598: asr      r3, r3, #2
0065e59c: str      lr, [sp]
0065e5a0: mov      lr, pc
0065e5a4: ldr      pc, [ip, #0x10]
0065e5a8: ldr      r3, [r4, #0x58]
0065e5ac: ldr      r2, [r4, #0x5c]
0065e5b0: add      r5, r5, #1
0065e5b4: rsb      r3, r3, r2
0065e5b8: cmp      r5, r3, asr #2
0065e5bc: blo      #0x65e530
0065e5c0: add      sp, sp, #8
0065e5c4: pop      {r4, r5, r6, r7, r8, pc}

# _ZN15AnimatorBlender17BlenderApplicator10ResetDeltaEj
00366a68: push     {r4, r5, r6, r7, r8, sl, lr}
00366a6c: ldr      r3, [r0, #8]
00366a70: ldr      sl, [pc, #0xf0]
00366a74: sub      sp, sp, #0x1c
00366a78: cmp      r3, #0
00366a7c: mov      r5, r0
00366a80: mov      r7, r1
00366a84: add      sl, pc, sl
00366a88: beq      #0x366b0c
00366a8c: ldr      r3, [r0, #0x3c]
00366a90: ldr      r2, [r3, #0x28]
00366a94: ldr      r3, [r3, #0x70]
00366a98: ldr      r4, [r2, r3, lsl #2]
00366a9c: ldr      r3, [r4]
00366aa0: mov      r0, r4
00366aa4: mov      lr, pc
00366aa8: ldr      pc, [r3, #0x44]
00366aac: mov      r8, r0
00366ab0: mov      r0, r4
00366ab4: bl       #0x369160
00366ab8: subs     r6, r0, #0
00366abc: beq      #0x366b14
00366ac0: ldr      r1, [r5, #0xc]
00366ac4: mov      r3, #0
00366ac8: str      r3, [sp, #0x14]
00366acc: cmn      r1, #1
00366ad0: str      r3, [sp, #0xc]
00366ad4: str      r3, [sp, #0x10]
00366ad8: beq      #0x366af4
00366adc: mov      r0, r4
00366ae0: ldr      r2, [r8, #0x10]
00366ae4: ldr      ip, [r4]
00366ae8: add      r3, sp, #0xc
00366aec: mov      lr, pc
00366af0: ldr      pc, [ip, #0x7c]
00366af4: cmp      r6, #0
00366af8: beq      #0x366b0c
00366afc: mov      r0, r6
00366b00: mov      r1, r7
00366b04: add      r2, sp, #0xc
00366b08: bl       #0x3644cc
00366b0c: add      sp, sp, #0x1c
00366b10: pop      {r4, r5, r6, r7, r8, sl, pc}
00366b14: ldr      r3, [pc, #0x50]
00366b18: ldr      r3, [sl, r3]
00366b1c: ldr      r3, [r3]
00366b20: cmp      r3, #2
00366b24: streq    r6, [r6]
00366b28: beq      #0x366ac0
00366b2c: cmp      r3, #1
00366b30: bne      #0x366ac0
00366b34: ldr      r0, [pc, #0x34]
00366b38: ldr      r1, [pc, #0x34]
00366b3c: ldr      r2, [pc, #0x34]
00366b40: ldr      r0, [sl, r0]
00366b44: ldr      r3, [pc, #0x30]
00366b48: movw     ip, #0x167
00366b4c: add      r1, pc, r1
00366b50: add      r2, pc, r2
00366b54: add      r3, pc, r3
00366b58: add      r0, r0, #0xa8
00366b5c: str      ip, [sp]
00366b60: bl       #0x30e004
00366b64: b        #0x366ac0
00366b68: rsbeq    lr, r2, ip
00366b6c: andeq    r3, r0, r0, asr #19
00366b70: andeq    r1, r0, r0, asr #19
00366b74: subseq   r7, r5, ip, lsl #17
00366b78: ldrsbeq  r6, [r7], #-0x78
00366b7c: subseq   sl, r5, r4, lsr #5

# _ZN11AnimatorSet20applyAnimationValuesEj
0036737c: ldr      r3, [r0, #0x98]
00367380: cmp      r3, #0
00367384: ldrne    r3, [r3, #0x20]
00367388: str      r3, [r0, #0x50]
0036738c: b        #0x65f418

# _ZN6glitch5scene18ISceneNodeAnimator15getTimelineCtrlEv
00599870: ldr      r0, [r0, #8]
00599874: bx       lr

# _ZNK6glitch7collada16CColladaDatabase21getBlendableAnimationEPKNS0_8SChannelE
0061c1e0: subs     r2, r1, #0
0061c1e4: beq      #0x61c1f4
0061c1e8: ldrb     r3, [r2, #0xc]
0061c1ec: ldmib    r2, {r1, r2}
0061c1f0: b        #0x61c0c8
0061c1f4: mov      r0, r2
0061c1f8: bx       lr

# _ZN6glitch7collada16CColladaDatabase19getAnimationTrackExEPKNS0_10SAnimationE
00611ae0: ldr      r3, [pc, #0x61c]
00611ae4: push     {r4, r5, r6, lr}
00611ae8: subs     r4, r0, #0
00611aec: add      r3, pc, r3
00611af0: beq      #0x611c80
00611af4: ldr      r2, [r4, #0x10]
00611af8: ldr      r2, [r2, #8]
00611afc: sub      r2, r2, #1
00611b00: cmp      r2, #0x5a
00611b04: addls    pc, pc, r2, lsl #2
00611b08: b        #0x611c80
00611b0c: b        #0x611cc4
00611b10: b        #0x611ce8
00611b14: b        #0x611d0c
00611b18: b        #0x611d30
00611b1c: b        #0x611d54
00611b20: b        #0x611d78
00611b24: b        #0x611d78
00611b28: b        #0x611d78
00611b2c: b        #0x611d78
00611b30: b        #0x611d9c
00611b34: b        #0x611dc0
00611b38: b        #0x611de4
00611b3c: b        #0x611e08
00611b40: b        #0x611e2c
00611b44: b        #0x611c80
00611b48: b        #0x611e44
00611b4c: b        #0x611c80
00611b50: b        #0x611c80
00611b54: b        #0x611c80
00611b58: b        #0x611e4c
00611b5c: b        #0x611c80
00611b60: b        #0x611c80
00611b64: b        #0x611c80
00611b68: b        #0x611c80
00611b6c: b        #0x611c80
00611b70: b        #0x611c80
00611b74: b        #0x611c80
00611b78: b        #0x611e58
00611b7c: b        #0x611e58
00611b80: b        #0x611e58
00611b84: b        #0x611e58
00611b88: b        #0x611e58
00611b8c: b        #0x611e64
00611b90: b        #0x611e58
00611b94: b        #0x611e58
00611b98: b        #0x611e58
00611b9c: b        #0x611e58
00611ba0: b        #0x611e58
00611ba4: b        #0x611e64
00611ba8: b        #0x611e58
00611bac: b        #0x611e58
00611bb0: b        #0x611e58
00611bb4: b        #0x611e58
00611bb8: b        #0x611e58
00611bbc: b        #0x611e58
00611bc0: b        #0x611e58
00611bc4: b        #0x611e58
00611bc8: b        #0x611e58
00611bcc: b        #0x611e58
00611bd0: b        #0x611e58
00611bd4: b        #0x611e58
00611bd8: b        #0x611e58
00611bdc: b        #0x611e58
00611be0: b        #0x611e58
00611be4: b        #0x611e58
00611be8: b        #0x611e58
00611bec: b        #0x611e64
00611bf0: b        #0x611e64
00611bf4: b        #0x611e58
00611bf8: b        #0x611e58
00611bfc: b        #0x611e58
00611c00: b        #0x611e58
00611c04: b        #0x611e58
00611c08: b        #0x611e58
00611c0c: b        #0x611e58
00611c10: b        #0x611e58
00611c14: b        #0x611e64
00611c18: b        #0x611e58
00611c1c: b        #0x611e58
00611c20: b        #0x611e58
00611c24: b        #0x611c80
00611c28: b        #0x611c80
00611c2c: b        #0x611c80
00611c30: b        #0x611c80
00611c34: b        #0x611c80
00611c38: b        #0x611c80
00611c3c: b        #0x611c80
00611c40: b        #0x611c80
00611c44: b        #0x611c80
00611c48: b        #0x611c80
00611c4c: b        #0x611c80
00611c50: b        #0x611c80
00611c54: b        #0x611c80
00611c58: b        #0x611c80
00611c5c: b        #0x611c80
00611c60: b        #0x611c88
00611c64: b        #0x611e38
00611c68: b        #0x611e38
00611c6c: b        #0x611e38
00611c70: b        #0x611e38
00611c74: b        #0x611e38
00611c78: cmp      r3, #2
00611c7c: beq      #0x611f6c
00611c80: mov      r0, #0
00611c84: pop      {r4, r5, r6, pc}
00611c88: ldr      r2, [r4, #8]
00611c8c: ldr      r3, [r2, #0x10]
00611c90: cmp      r3, #1
00611c94: beq      #0x611e70
00611c98: cmp      r3, #6
00611c9c: bne      #0x611c80
00611ca0: ldr      r3, [r2, #0x14]
00611ca4: sub      r3, r3, #1
00611ca8: cmp      r3, #3
00611cac: addls    pc, pc, r3, lsl #2
00611cb0: b        #0x611c80
00611cb4: b        #0x611f9c
00611cb8: b        #0x611f94
00611cbc: b        #0x611f8c
00611cc0: b        #0x611f84
00611cc4: ldr      r3, [r4, #0x1c]
00611cc8: cmp      r3, #0
00611ccc: beq      #0x611ef4
00611cd0: ldr      r3, [r3]
00611cd4: cmp      r3, #1
00611cd8: beq      #0x612034
00611cdc: bhs      #0x611eec
00611ce0: pop      {r4, r5, r6, lr}
00611ce4: b        #0x6103c0
00611ce8: ldr      r3, [r4, #0x1c]
00611cec: cmp      r3, #0
00611cf0: beq      #0x611f54
00611cf4: ldr      r3, [r3]
00611cf8: cmp      r3, #1
00611cfc: beq      #0x612024
00611d00: bhs      #0x611f4c
00611d04: pop      {r4, r5, r6, lr}
00611d08: b        #0x61057c
00611d0c: ldr      r3, [r4, #0x1c]
00611d10: cmp      r3, #0
00611d14: beq      #0x611f04
00611d18: ldr      r3, [r3]
00611d1c: cmp      r3, #1
00611d20: beq      #0x61201c
00611d24: bhs      #0x611efc
00611d28: pop      {r4, r5, r6, lr}
00611d2c: b        #0x610738
00611d30: ldr      r3, [r4, #0x1c]
00611d34: cmp      r3, #0
00611d38: beq      #0x611f64
00611d3c: ldr      r3, [r3]
00611d40: cmp      r3, #1
00611d44: beq      #0x61204c
00611d48: bhs      #0x611f5c
00611d4c: pop      {r4, r5, r6, lr}
00611d50: b        #0x6108f4
00611d54: ldr      r3, [r4, #0x1c]
00611d58: cmp      r3, #0
00611d5c: beq      #0x611f6c
00611d60: ldr      r3, [r3]
00611d64: cmp      r3, #1
00611d68: beq      #0x612064
00611d6c: bhs      #0x611c78
00611d70: pop      {r4, r5, r6, lr}
00611d74: b        #0x610048
00611d78: ldr      r3, [r4, #0x1c]
00611d7c: cmp      r3, #0
00611d80: beq      #0x611f44
00611d84: ldr      r3, [r3]
00611d88: cmp      r3, #1
00611d8c: beq      #0x61205c
00611d90: bhs      #0x611f3c
00611d94: pop      {r4, r5, r6, lr}
00611d98: b        #0x610204
00611d9c: ldr      r3, [r4, #0x1c]
00611da0: cmp      r3, #0
00611da4: beq      #0x611f34
00611da8: ldr      r3, [r3]
00611dac: cmp      r3, #1
00611db0: beq      #0x612054
00611db4: bhs      #0x611f2c
00611db8: pop      {r4, r5, r6, lr}
00611dbc: b        #0x610ab0
00611dc0: ldr      r3, [r4, #0x1c]
00611dc4: cmp      r3, #0
00611dc8: beq      #0x611f24
00611dcc: ldr      r3, [r3]
00611dd0: cmp      r3, #1
00611dd4: beq      #0x61202c
00611dd8: bhs      #0x611f1c
00611ddc: pop      {r4, r5, r6, lr}
00611de0: b        #0x610c6c
00611de4: ldr      r3, [r4, #0x1c]
00611de8: cmp      r3, #0
00611dec: beq      #0x611f14
00611df0: ldr      r3, [r3]
00611df4: cmp      r3, #1
00611df8: beq      #0x612044
00611dfc: bhs      #0x611f0c
00611e00: pop      {r4, r5, r6, lr}
00611e04: b        #0x610e28
00611e08: ldr      r3, [r4, #0x1c]
00611e0c: cmp      r3, #0
00611e10: beq      #0x611f7c
00611e14: ldr      r3, [r3]
00611e18: cmp      r3, #1
00611e1c: beq      #0x61203c
00611e20: bhs      #0x611f74
00611e24: pop      {r4, r5, r6, lr}
00611e28: b        #0x610fe4
00611e2c: ldr      r2, [pc, #0x2d4]
00611e30: ldr      r0, [r3, r2]
00611e34: pop      {r4, r5, r6, pc}
00611e38: ldr      r2, [pc, #0x2cc]
00611e3c: ldr      r0, [r3, r2]
00611e40: pop      {r4, r5, r6, pc}
00611e44: pop      {r4, r5, r6, lr}
00611e48: b        #0x611078
00611e4c: ldr      r2, [pc, #0x2bc]
00611e50: ldr      r0, [r3, r2]
00611e54: pop      {r4, r5, r6, pc}
00611e58: ldr      r2, [pc, #0x2b4]
00611e5c: ldr      r0, [r3, r2]
00611e60: pop      {r4, r5, r6, pc}
00611e64: ldr      r2, [pc, #0x2ac]
00611e68: ldr      r0, [r3, r2]
00611e6c: pop      {r4, r5, r6, pc}
00611e70: ldr      r3, [r2, #0x14]
00611e74: cmp      r3, #3
00611e78: beq      #0x61206c
00611e7c: cmp      r3, #4
00611e80: beq      #0x611ff4
00611e84: cmp      r3, #1
00611e88: bne      #0x611c80
00611e8c: ldr      r3, [r4, #0x18]
00611e90: ldr      r2, [r3, #4]
00611e94: cmp      r2, #1
00611e98: ble      #0x611c80
00611e9c: ldr      r3, [r3]
00611ea0: sub      r3, r3, #1
00611ea4: cmp      r3, #0xe
00611ea8: addls    pc, pc, r3, lsl #2
00611eac: b        #0x611c80
00611eb0: b        #0x612004
00611eb4: b        #0x611ffc
00611eb8: b        #0x611c80
00611ebc: b        #0x612014
00611ec0: b        #0x611c80
00611ec4: b        #0x611c80
00611ec8: b        #0x611c80
00611ecc: b        #0x61200c
00611ed0: b        #0x611c80
00611ed4: b        #0x611c80
00611ed8: b        #0x611c80
00611edc: b        #0x611c80
00611ee0: b        #0x611c80
00611ee4: b        #0x611c80
00611ee8: b        #0x611ff4
00611eec: cmp      r3, #2
00611ef0: bne      #0x611c80
00611ef4: pop      {r4, r5, r6, lr}
00611ef8: b        #0x610298
00611efc: cmp      r3, #2
00611f00: bne      #0x611c80
00611f04: pop      {r4, r5, r6, lr}
00611f08: b        #0x610610
00611f0c: cmp      r3, #2
00611f10: bne      #0x611c80
00611f14: pop      {r4, r5, r6, lr}
00611f18: b        #0x610d00
00611f1c: cmp      r3, #2
00611f20: bne      #0x611c80
00611f24: pop      {r4, r5, r6, lr}
00611f28: b        #0x610b44
00611f2c: cmp      r3, #2
00611f30: bne      #0x611c80
00611f34: pop      {r4, r5, r6, lr}
00611f38: b        #0x610988
00611f3c: cmp      r3, #2
00611f40: bne      #0x611c80
00611f44: pop      {r4, r5, r6, lr}
00611f48: b        #0x6100dc
00611f4c: cmp      r3, #2
00611f50: bne      #0x611c80
00611f54: pop      {r4, r5, r6, lr}
00611f58: b        #0x610454
00611f5c: cmp      r3, #2
00611f60: bne      #0x611c80
00611f64: pop      {r4, r5, r6, lr}
00611f68: b        #0x6107cc
00611f6c: pop      {r4, r5, r6, lr}
00611f70: b        #0x60ff20
00611f74: cmp      r3, #2
00611f78: bne      #0x611c80
00611f7c: pop      {r4, r5, r6, lr}
00611f80: b        #0x610ebc
00611f84: pop      {r4, r5, r6, lr}
00611f88: b        #0x6116d4
00611f8c: pop      {r4, r5, r6, lr}
00611f90: b        #0x611640
00611f94: pop      {r4, r5, r6, lr}
00611f98: b        #0x6115ac
00611f9c: ldr      r5, [pc, #0x178]
00611fa0: add      r5, pc, r5
00611fa4: ldr      r3, [r5, #0xc]
00611fa8: tst      r3, #1
00611fac: beq      #0x612074
00611fb0: ldr      r3, [r4, #0x18]
00611fb4: ldm      r3, {r1, r2}
00611fb8: sub      r3, r1, #1
00611fbc: cmp      r3, #7
00611fc0: movhi    r1, #0
00611fc4: bhi      #0x611fd4
00611fc8: ldr      r1, [pc, #0x150]
00611fcc: add      r1, pc, r1
00611fd0: ldr      r1, [r1, r3, lsl #2]
00611fd4: ldr      r3, [pc, #0x148]
00611fd8: sub      r2, r2, #1
00611fdc: add      r2, r2, r2, lsl #2
00611fe0: add      r2, r2, r1
00611fe4: add      r3, pc, r3
00611fe8: add      r3, r3, r2, lsl #2
00611fec: ldr      r0, [r3, #0x10]
00611ff0: pop      {r4, r5, r6, pc}
00611ff4: pop      {r4, r5, r6, lr}
00611ff8: b        #0x611a4c
00611ffc: pop      {r4, r5, r6, lr}
00612000: b        #0x6117fc
00612004: pop      {r4, r5, r6, lr}
00612008: b        #0x611768
0061200c: pop      {r4, r5, r6, lr}
00612010: b        #0x611924
00612014: pop      {r4, r5, r6, lr}
00612018: b        #0x611890
0061201c: pop      {r4, r5, r6, lr}
00612020: b        #0x6106a4
00612024: pop      {r4, r5, r6, lr}
00612028: b        #0x6104e8
0061202c: pop      {r4, r5, r6, lr}
00612030: b        #0x610bd8
00612034: pop      {r4, r5, r6, lr}
00612038: b        #0x61032c
0061203c: pop      {r4, r5, r6, lr}
00612040: b        #0x610f50
00612044: pop      {r4, r5, r6, lr}
00612048: b        #0x610d94
0061204c: pop      {r4, r5, r6, lr}
00612050: b        #0x610860
00612054: pop      {r4, r5, r6, lr}
00612058: b        #0x610a1c
0061205c: pop      {r4, r5, r6, lr}
00612060: b        #0x610170
00612064: pop      {r4, r5, r6, lr}
00612068: b        #0x60ffb4
0061206c: pop      {r4, r5, r6, lr}
00612070: b        #0x6119b8
00612074: add      r6, r5, #0xc
00612078: mov      r0, r6
0061207c: bl       #0x30e76c
00612080: cmp      r0, #0
00612084: beq      #0x611fb0
00612088: bl       #0x61110c
0061208c: str      r0, [r5, #0x10]
00612090: bl       #0x61110c
00612094: str      r0, [r5, #0x14]
00612098: bl       #0x6115ac
0061209c: str      r0, [r5, #0x24]
006120a0: bl       #0x6111a0
006120a4: str      r0, [r5, #0x28]
006120a8: bl       #0x611234
006120ac: str      r0, [r5, #0x2c]
006120b0: bl       #0x611640
006120b4: str      r0, [r5, #0x38]
006120b8: bl       #0x6112c8
006120bc: str      r0, [r5, #0x3c]
006120c0: bl       #0x6112c8
006120c4: str      r0, [r5, #0x40]
006120c8: bl       #0x6112c8
006120cc: str      r0, [r5, #0x44]
006120d0: bl       #0x6116d4
006120d4: str      r0, [r5, #0x4c]
006120d8: bl       #0x61135c
006120dc: str      r0, [r5, #0x50]
006120e0: bl       #0x6113f0
006120e4: str      r0, [r5, #0x54]
006120e8: bl       #0x611484
006120ec: str      r0, [r5, #0x58]
006120f0: bl       #0x611518
006120f4: str      r0, [r5, #0x5c]
006120f8: mov      r0, r6
006120fc: bl       #0x30ea3c
00612100: b        #0x611fb0
00612104: eorseq   r2, r8, r4, lsr #31
00612108: andeq    r0, r0, r4, lsr #30
0061210c: andeq    r2, r0, r4, lsl #27
00612110: andeq    r4, r0, r0, lsl #1
00612114: strheq   r2, [r0], -r0
00612118: ldrdeq   r4, r5, [r0], -r0
0061211c: eorseq   r4, lr, r4, ror sp
00612120: mlaeq    sp, r8, sp, r2
00612124: eorseq   r4, lr, r0, lsr sp

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEEEE10applyValueEPvSB_PNS1_15CApplicatorInfoE
0062393c: push     {r4, lr}
00623940: mov      r0, r2
00623944: ldr      r3, [r2]
00623948: mov      lr, pc
0062394c: ldr      pc, [r3, #0x94]
00623950: pop      {r4, pc}

# _ZN6glitch7collada25CSceneNodeAnimatorBlender9setTargetEiPvPKNS0_15animation_track15CApplicatorInfoE
0065e244: push     {r4, r5, r6, lr}
0065e248: ldr      ip, [r0, #0x58]
0065e24c: mov      r6, r3
0065e250: mov      r4, r0
0065e254: str      r2, [ip, r1, lsl #2]
0065e258: ldr      r3, [r0, #0x64]
0065e25c: mov      r5, r1
0065e260: ldr      r3, [r3, r1, lsl #2]
0065e264: cmp      r3, #0
0065e268: beq      #0x65e288
0065e26c: mov      r0, r3
0065e270: ldr      r3, [r3]
0065e274: mov      lr, pc
0065e278: ldr      pc, [r3, #4]
0065e27c: ldr      r3, [r4, #0x64]
0065e280: mov      r2, #0
0065e284: str      r2, [r3, r5, lsl #2]
0065e288: cmp      r6, #0
0065e28c: beq      #0x65e2a8
0065e290: mov      r0, r6
0065e294: ldr      r3, [r6]
0065e298: ldr      r4, [r4, #0x64]
0065e29c: mov      lr, pc
0065e2a0: ldr      pc, [r3, #8]
0065e2a4: str      r0, [r4, r5, lsl #2]
0065e2a8: pop      {r4, r5, r6, pc}

# _ZNK6glitch7collada21CSceneNodeAnimatorSet19getCurrentAnimationEv
0065f114: ldr      r0, [r0, #0x50]
0065f118: bx       lr

# _ZN15AnimatorBlender10updateTimeEj
00366d90: push     {r4, r5, r6, r7, r8, lr}
00366d94: ldr      r3, [r0, #0x7c]
00366d98: mov      r5, r0
00366d9c: mov      r7, r1
00366da0: cmp      r3, #0
00366da4: ldr      r0, [r0, #0x84]
00366da8: blt      #0x366df4
00366dac: rsb      r0, r0, r1
00366db0: rsb      r0, r0, r3
00366db4: cmp      r0, #0
00366db8: str      r0, [r5, #0x7c]
00366dbc: ble      #0x366e94
00366dc0: bl       #0x30e964
00366dc4: ldr      r1, [r5, #0x80]
00366dc8: bl       #0x30ed6c
00366dcc: ldr      r2, [r5, #0x34]
00366dd0: ldr      ip, [r5, #0x74]
00366dd4: mov      r3, r0
00366dd8: mov      r1, r0
00366ddc: str      r3, [r2, ip, lsl #2]
00366de0: mov      r0, #0x3f800000
00366de4: bl       #0x30e3ac
00366de8: ldr      r2, [r5, #0x70]
00366dec: ldr      r3, [r5, #0x34]
00366df0: str      r0, [r3, r2, lsl #2]
00366df4: ldr      r6, [r5, #0x2c]
00366df8: ldr      r3, [r5, #0x28]
00366dfc: rsb      r6, r3, r6
00366e00: asrs     r6, r6, #2
00366e04: beq      #0x366e5c
00366e08: mov      r4, #0
00366e0c: b        #0x366e1c
00366e10: add      r4, r4, #1
00366e14: cmp      r4, r6
00366e18: beq      #0x366e5c
00366e1c: ldr      r3, [r5, #0x34]
00366e20: mov      r1, #0
00366e24: ldr      r0, [r3, r4, lsl #2]
00366e28: bl       #0x30df8c
00366e2c: cmp      r0, #0
00366e30: bne      #0x366e10
00366e34: ldr      r3, [r5, #0x28]
00366e38: mov      r1, r7
00366e3c: ldr      r3, [r3, r4, lsl #2]
00366e40: add      r4, r4, #1
00366e44: mov      r0, r3
00366e48: ldr      r3, [r3]
00366e4c: mov      lr, pc
00366e50: ldr      pc, [r3, #0x14]
00366e54: cmp      r4, r6
00366e58: bne      #0x366e1c
00366e5c: mov      r0, r5
00366e60: bl       #0x366594
00366e64: ldr      r2, [r5, #0x70]
00366e68: ldr      r3, [r5, #0x28]
00366e6c: ldr      r3, [r3, r2, lsl #2]
00366e70: mov      r0, r3
00366e74: ldr      r3, [r3]
00366e78: mov      lr, pc
00366e7c: ldr      pc, [r3, #0x44]
00366e80: mov      r1, r0
00366e84: add      r0, r5, #0x88
00366e88: bl       #0x36440c
00366e8c: str      r7, [r5, #0x84]
00366e90: pop      {r4, r5, r6, r7, r8, pc}
00366e94: ldr      r2, [r5, #0x74]
00366e98: ldr      r3, [r5, #0x34]
00366e9c: mov      r1, #0
00366ea0: str      r1, [r3, r2, lsl #2]
00366ea4: ldr      r2, [r5, #0x70]
00366ea8: ldr      r3, [r5, #0x34]
00366eac: mov      r1, #0x3f800000
00366eb0: str      r1, [r3, r2, lsl #2]
00366eb4: b        #0x366df4

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE12getValueSizeEv
0060f0e8: mov      r0, #0x10
0060f0ec: bx       lr

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE10applyValueEPvSA_PNS1_15CApplicatorInfoE
0062084c: push     {r4, lr}
00620850: mov      r0, r2
00620854: ldr      r3, [r2]
00620858: mov      lr, pc
0062085c: ldr      pc, [r3, #0x9c]
00620860: pop      {r4, pc}

# _ZN6glitch7collada25CSceneNodeAnimatorBlender17getAnimationTrackEi
0065e204: push     {r4, lr}
0065e208: ldr      r3, [r0, #0x28]
0065e20c: ldr      r3, [r3]
0065e210: mov      r0, r3
0065e214: ldr      r3, [r3]
0065e218: mov      lr, pc
0065e21c: ldr      pc, [r3, #0x54]
0065e220: pop      {r4, pc}

# _ZN6glitch7collada18CSceneNodeAnimator22computeAnimationValuesEj
0065d80c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0065d810: ldr      r2, [r0, #0x48]
0065d814: ldr      r3, [r0, #0x44]
0065d818: sub      sp, sp, #0x2c
0065d81c: mov      r5, r0
0065d820: rsb      r3, r3, r2
0065d824: lsrs     r3, r3, #4
0065d828: mov      r4, r1
0065d82c: bne      #0x65d83c
0065d830: ldr      r3, [r0, #0x50]
0065d834: cmp      r3, #0
0065d838: beq      #0x65d8fc
0065d83c: mov      r0, r5
0065d840: mov      r1, r4
0065d844: bl       #0x667c48
0065d848: ldr      r3, [r5]
0065d84c: mov      r0, r5
0065d850: mov      lr, pc
0065d854: ldr      pc, [r3, #0x44]
0065d858: cmp      r0, #0
0065d85c: ldrne    r7, [r0, #4]
0065d860: beq      #0x65d904
0065d864: mov      r1, r7
0065d868: mov      r0, r5
0065d86c: ldr      r8, [r5, #0xc]
0065d870: bl       #0x65d768
0065d874: ldr      r1, [r5, #0x44]
0065d878: ldr      r6, [r5, #0x48]
0065d87c: ldrb     r3, [r5, #0x34]
0065d880: subs     r8, r8, #1
0065d884: movne    r8, #1
0065d888: rsb      r6, r1, r6
0065d88c: asrs     r6, r6, #4
0065d890: mov      sl, r0
0065d894: strb     r3, [sp, #0x19]
0065d898: beq      #0x65d8fc
0065d89c: mov      r4, #0
0065d8a0: add      sb, sp, #0xc
0065d8a4: add      fp, sp, #0x1c
0065d8a8: b        #0x65d8b0
0065d8ac: ldr      r1, [r5, #0x44]
0065d8b0: add      r3, r1, r4, lsl #4
0065d8b4: ldr      r2, [r3, #4]
0065d8b8: add      r3, r3, #0xc
0065d8bc: cmp      r2, #0
0065d8c0: beq      #0x65d8f0
0065d8c4: ldrb     ip, [r5, #0x34]
0065d8c8: ldr      r0, [r1, r4, lsl #4]
0065d8cc: str      sl, [sp, #0x20]
0065d8d0: cmp      ip, #0
0065d8d4: str      r0, [sp, #0x1c]
0065d8d8: addne    r3, r1, #0xc
0065d8dc: mov      r0, fp
0065d8e0: mov      r1, r7
0065d8e4: str      sb, [sp, #0x24]
0065d8e8: str      r8, [sp]
0065d8ec: bl       #0x66a1a8
0065d8f0: add      r4, r4, #1
0065d8f4: cmp      r4, r6
0065d8f8: bne      #0x65d8ac
0065d8fc: add      sp, sp, #0x2c
0065d900: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0065d904: mov      r0, r4
0065d908: ldr      r1, [r5, #0x14]
0065d90c: bl       #0x30eb2c
0065d910: ldr      r7, [r5, #0x38]
0065d914: add      r7, r1, r7
0065d918: b        #0x65d864

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEEEE15getBlendedValueEPvPfiSB_
006275fc: cmp      r3, #1
00627600: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00627604: mov      r4, r3
00627608: mov      fp, r2
0062760c: beq      #0x6276b8
00627610: cmp      r3, #0
00627614: moveq    r8, #0
00627618: moveq    sb, r8
0062761c: moveq    sl, r8
00627620: beq      #0x6276a0
00627624: mov      r8, #0
00627628: mov      r5, r1
0062762c: mov      r7, #0
00627630: mov      sb, r8
00627634: mov      sl, r8
00627638: ldr      r6, [fp, r7]
0062763c: ldr      r1, [r5]
00627640: add      r7, r7, #4
00627644: mov      r0, r6
00627648: bl       #0x30ed6c
0062764c: mov      r1, r0
00627650: mov      r0, r8
00627654: bl       #0x30eba4
00627658: ldr      r1, [r5, #4]
0062765c: mov      r8, r0
00627660: mov      r0, r6
00627664: bl       #0x30ed6c
00627668: mov      r1, r0
0062766c: mov      r0, sb
00627670: bl       #0x30eba4
00627674: ldr      r1, [r5, #8]
00627678: mov      sb, r0
0062767c: mov      r0, r6
00627680: bl       #0x30ed6c
00627684: mov      r1, r0
00627688: mov      r0, sl
0062768c: bl       #0x30eba4
00627690: subs     r4, r4, #1
00627694: mov      sl, r0
00627698: add      r5, r5, #0xc
0062769c: bne      #0x627638
006276a0: ldr      r3, [sp, #0x28]
006276a4: str      r8, [r3], #4
006276a8: ldr      r2, [sp, #0x28]
006276ac: str      sb, [r2, #4]
006276b0: str      sl, [r3, #4]
006276b4: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
006276b8: mov      r2, r1
006276bc: ldr      r0, [r2], #4
006276c0: ldr      r3, [sp, #0x28]
006276c4: str      r0, [r3], #4
006276c8: ldr      r1, [r1, #4]
006276cc: ldr      r0, [sp, #0x28]
006276d0: str      r1, [r0, #4]
006276d4: ldr      r2, [r2, #4]
006276d8: str      r2, [r3, #4]
006276dc: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEEEE10applyValueEPvSB_PNS1_15CApplicatorInfoE
00623dd8: push     {r4, lr}
00623ddc: mov      r0, r2
00623de0: ldr      r3, [r2]
00623de4: mov      lr, pc
00623de8: ldr      pc, [r3, #0xa4]
00623dec: pop      {r4, pc}

# _ZN15AnimatorBlender9BlendPostEv
00366740: bx       lr

# _ZN6glitch7collada25CSceneNodeAnimatorBlender19getAnimationTrackExEi
0065e224: push     {r4, lr}
0065e228: ldr      r3, [r0, #0x28]
0065e22c: ldr      r3, [r3]
0065e230: mov      r0, r3
0065e234: ldr      r3, [r3]
0065e238: mov      lr, pc
0065e23c: ldr      pc, [r3, #0x58]
0065e240: pop      {r4, pc}

# _ZNK6glitch5scene18ISceneNodeAnimator15getTimelineCtrlEv
00599868: ldr      r0, [r0, #8]
0059986c: bx       lr

# _ZN6glitch7collada21IAnimationSetTemplate11addChannelsEPSt6vectorIPKNS0_8SChannelENS_4core10SAllocatorIS5_LNS_6memory13E_MEMORY_HINTE0EEEEPS2_IPKNS0_17CAnimationTrackExENS7_ISF_LS9_0EEEE
00667384: push     {r4, r5, r6, r7, r8, lr}
00667388: mov      r5, r0
0066738c: ldr      r3, [r0, #4]
00667390: ldr      r0, [r0, #8]
00667394: rsb      r2, r3, r0
00667398: lsrs     r2, r2, #2
0066739c: beq      #0x667488
006673a0: mov      r4, #0
006673a4: mov      r7, #1
006673a8: b        #0x6673bc
006673ac: add      r4, r4, #1
006673b0: rsb      r2, r3, r0
006673b4: cmp      r4, r2, asr #2
006673b8: bhs      #0x667488
006673bc: ldr      r2, [r3, r4, lsl #2]
006673c0: lsl      r6, r4, #2
006673c4: ldrb     r1, [r2]
006673c8: cmp      r1, #0
006673cc: bne      #0x6673ac
006673d0: mov      r0, #0x10
006673d4: bl       #0x5341ac
006673d8: ldr      r3, [r5, #4]
006673dc: mov      r8, r0
006673e0: ldr      r3, [r3, r4, lsl #2]
006673e4: ldr      r3, [r3, #4]
006673e8: str      r3, [r0, #8]
006673ec: ldr      r3, [r5, #4]
006673f0: ldr      r3, [r3, r4, lsl #2]
006673f4: ldr      r3, [r3, #8]
006673f8: mov      r0, r3
006673fc: ldr      r3, [r3]
00667400: mov      lr, pc
00667404: ldr      pc, [r3, #0x54]
00667408: str      r0, [r8, #4]
0066740c: ldr      r3, [r5, #4]
00667410: ldr      r3, [r3, r4, lsl #2]
00667414: str      r8, [r3, #0xc]
00667418: ldr      r3, [r8, #8]
0066741c: sub      r3, r3, #1
00667420: cmp      r3, #0xc
00667424: addls    pc, pc, r3, lsl #2
00667428: b        #0x667464
0066742c: b        #0x66756c
00667430: b        #0x667550
00667434: b        #0x667534
00667438: b        #0x667518
0066743c: b        #0x6674fc
00667440: b        #0x6674e0
00667444: b        #0x6674e0
00667448: b        #0x6674e0
0066744c: b        #0x6674e0
00667450: b        #0x6674c4
00667454: b        #0x6674a8
00667458: b        #0x66748c
0066745c: b        #0x667460
00667460: bl       #0x610ebc
00667464: ldr      r3, [r5, #4]
00667468: ldr      r3, [r3, r6]
0066746c: strb     r7, [r3]
00667470: ldr      r3, [r5, #4]
00667474: ldr      r0, [r5, #8]
00667478: add      r4, r4, #1
0066747c: rsb      r2, r3, r0
00667480: cmp      r4, r2, asr #2
00667484: blo      #0x6673bc
00667488: pop      {r4, r5, r6, r7, r8, pc}
0066748c: bl       #0x610d00
00667490: ldr      r3, [r5, #4]
00667494: ldr      r3, [r3, r6]
00667498: strb     r7, [r3]
0066749c: ldr      r3, [r5, #4]
006674a0: ldr      r0, [r5, #8]
006674a4: b        #0x667478
006674a8: bl       #0x610b44
006674ac: ldr      r3, [r5, #4]
006674b0: ldr      r3, [r3, r6]
006674b4: strb     r7, [r3]
006674b8: ldr      r3, [r5, #4]
006674bc: ldr      r0, [r5, #8]
006674c0: b        #0x667478
006674c4: bl       #0x610988
006674c8: ldr      r3, [r5, #4]
006674cc: ldr      r3, [r3, r6]
006674d0: strb     r7, [r3]
006674d4: ldr      r3, [r5, #4]
006674d8: ldr      r0, [r5, #8]
006674dc: b        #0x667478
006674e0: bl       #0x6100dc
006674e4: ldr      r3, [r5, #4]
006674e8: ldr      r3, [r3, r6]
006674ec: strb     r7, [r3]
006674f0: ldr      r3, [r5, #4]
006674f4: ldr      r0, [r5, #8]
006674f8: b        #0x667478
006674fc: bl       #0x60ff20
00667500: ldr      r3, [r5, #4]
00667504: ldr      r3, [r3, r6]
00667508: strb     r7, [r3]
0066750c: ldr      r3, [r5, #4]
00667510: ldr      r0, [r5, #8]
00667514: b        #0x667478
00667518: bl       #0x6107cc
0066751c: ldr      r3, [r5, #4]
00667520: ldr      r3, [r3, r6]
00667524: strb     r7, [r3]
00667528: ldr      r3, [r5, #4]
0066752c: ldr      r0, [r5, #8]
00667530: b        #0x667478
00667534: bl       #0x610610
00667538: ldr      r3, [r5, #4]
0066753c: ldr      r3, [r3, r6]
00667540: strb     r7, [r3]
00667544: ldr      r3, [r5, #4]
00667548: ldr      r0, [r5, #8]
0066754c: b        #0x667478
00667550: bl       #0x610454
00667554: ldr      r3, [r5, #4]
00667558: ldr      r3, [r3, r6]
0066755c: strb     r7, [r3]
00667560: ldr      r3, [r5, #4]
00667564: ldr      r0, [r5, #8]
00667568: b        #0x667478
0066756c: bl       #0x610298
00667570: ldr      r3, [r5, #4]
00667574: ldr      r3, [r3, r6]
00667578: strb     r7, [r3]
0066757c: ldr      r3, [r5, #4]
00667580: ldr      r0, [r5, #8]
00667584: b        #0x667478

# _ZN11AnimatorSet22computeAnimationValuesEj
00367368: ldr      r3, [r0, #0x98]
0036736c: cmp      r3, #0
00367370: ldrne    r3, [r3, #0x20]
00367374: str      r3, [r0, #0x50]
00367378: b        #0x65f5dc

# _ZN15AnimatorBlender8SetScaleEf
003666d8: push     {r4, r5, r6, r7, r8, lr}
003666dc: ldr      r3, [r0, #0x28]
003666e0: ldr      r6, [r0, #0x2c]
003666e4: mov      r5, r0
003666e8: mov      r7, r1
003666ec: rsb      r6, r3, r6
003666f0: asrs     r6, r6, #2
003666f4: beq      #0x36673c
003666f8: mov      r4, #0
003666fc: b        #0x366704
00366700: ldr      r3, [r5, #0x28]
00366704: ldr      r3, [r3, r4, lsl #2]
00366708: add      r4, r4, #1
0036670c: mov      r0, r3
00366710: ldr      r3, [r3]
00366714: mov      lr, pc
00366718: ldr      pc, [r3, #0x44]
0036671c: subs     r3, r0, #0
00366720: mov      r1, r7
00366724: beq      #0x366734
00366728: ldr      r3, [r3]
0036672c: mov      lr, pc
00366730: ldr      pc, [r3, #0x48]
00366734: cmp      r4, r6
00366738: bne      #0x366700
0036673c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN14AnimSetManager11GetAnimatorEi
004762c4: push     {r4, r5, r6, lr}
004762c8: sub      sp, sp, #0x10
004762cc: str      r1, [sp, #4]
004762d0: mov      r5, r0
004762d4: bl       #0x475404
004762d8: subs     r4, r0, #0
004762dc: bne      #0x4762ec
004762e0: mov      r0, r4
004762e4: add      sp, sp, #0x10
004762e8: pop      {r4, r5, r6, pc}
004762ec: add      r0, r5, #4
004762f0: add      r1, sp, #4
004762f4: bl       #0x476058
004762f8: ldr      r3, [r0, #0x20]
004762fc: mov      r5, r0
00476300: ldrb     r2, [r3, #0x70]
00476304: cmp      r2, #0
00476308: bne      #0x476384
0047630c: add      r6, sp, #0x10
00476310: str      r5, [r6, #-4]!
00476314: ldr      r3, [r5, #4]
00476318: mov      r1, #0
0047631c: mov      r0, #0xa4
00476320: add      r3, r3, #1
00476324: str      r3, [r5, #4]
00476328: bl       #0x310570
0047632c: mov      r1, r6
00476330: mov      r4, r0
00476334: bl       #0x3676b8
00476338: ldr      r0, [sp, #0xc]
0047633c: cmp      r0, #0
00476340: beq      #0x476348
00476344: bl       #0x31d584
00476348: ldr      r3, [r4]
0047634c: mov      r0, r4
00476350: mov      lr, pc
00476354: ldr      pc, [r3, #0x44]
00476358: mov      r6, r0
0047635c: mov      r0, r5
00476360: bl       #0x3649bc
00476364: cmp      r6, #0
00476368: beq      #0x4762e0
0047636c: mov      r0, r6
00476370: ldr      r3, [r6]
00476374: mov      r1, #0
00476378: mov      lr, pc
0047637c: ldr      pc, [r3, #0x40]
00476380: b        #0x4762e0
00476384: mov      r0, r3
00476388: ldr      r3, [r3]
0047638c: mov      lr, pc
00476390: ldr      pc, [r3, #0x38]
00476394: b        #0x47630c

# _ZNK6glitch7collada35CAnimationSetTransformationTemplate15getDefaultValueEPKNS0_8SChannelEPPv
006e23d0: push     {r4, r5, r6, r7, r8, lr}
006e23d4: ldr      r4, [r0, #4]
006e23d8: ldr      r3, [r0, #8]
006e23dc: mov      r5, r0
006e23e0: mov      r7, r1
006e23e4: cmp      r4, r3
006e23e8: mov      r6, r2
006e23ec: bne      #0x6e2400
006e23f0: b        #0x6e242c
006e23f4: ldr      r3, [r5, #8]
006e23f8: cmp      r4, r3
006e23fc: beq      #0x6e242c
006e2400: ldr      r3, [r4]
006e2404: mov      r1, r7
006e2408: mov      r2, r6
006e240c: ldr      r0, [r3, #8]
006e2410: add      r4, r4, #4
006e2414: add      r0, r0, #0x14c
006e2418: bl       #0x61c6bc
006e241c: cmp      r0, #0
006e2420: beq      #0x6e23f4
006e2424: mov      r0, #1
006e2428: pop      {r4, r5, r6, r7, r8, pc}
006e242c: mov      r0, #0
006e2430: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE17applyBlendedValueEPvPfiSC_PNS1_15CApplicatorInfoE
00620118: mov      r0, r1
0062011c: ldr      ip, [sp, #4]
00620120: mov      r1, r2
00620124: mov      r2, r3
00620128: ldr      r3, [sp]
0062012c: str      ip, [sp]
00620130: b        #0x62008c

# _ZN15AnimatorBlenderC1Ev
00367018: push     {r4, r5, r6, lr}
0036701c: ldr      r5, [pc, #0x90]
00367020: ldr      r3, [pc, #0x90]
00367024: ldr      r1, [pc, #0x90]
00367028: add      r5, pc, r5
0036702c: ldr      r3, [r5, r3]
00367030: ldr      r1, [r5, r1]
00367034: mov      r2, #1
00367038: add      r3, r3, #8
0036703c: str      r2, [r0, #0xcc]
00367040: str      r3, [r0, #0xc8]
00367044: add      r1, r1, #4
00367048: mov      r4, r0
0036704c: bl       #0x366f7c
00367050: ldr      r2, [pc, #0x68]
00367054: mov      r1, #0
00367058: mov      r3, #0
0036705c: ldr      r2, [r5, r2]
00367060: str      r1, [r4, #0x80]
00367064: str      r3, [r4, #0x84]
00367068: add      r1, r2, #0xa0
0036706c: add      r0, r2, #0xc
00367070: add      r2, r2, #0xbc
00367074: stm      r4, {r0, r1}
00367078: str      r3, [r4, #0x70]
0036707c: str      r3, [r4, #0x74]
00367080: str      r3, [r4, #0x78]
00367084: str      r3, [r4, #0x7c]
00367088: str      r2, [r4, #0xc8]
0036708c: add      r0, r4, #0x88
00367090: mov      r1, r4
00367094: bl       #0x364330
00367098: ldr      r3, [pc, #0x24]
0036709c: str      r4, [r4, #0xc4]
003670a0: mov      r0, r4
003670a4: ldr      r3, [r5, r3]
003670a8: add      r3, r3, #8
003670ac: str      r3, [r4, #0x88]
003670b0: pop      {r4, r5, r6, pc}
003670b4: rsbeq    sp, r2, r8, ror #20
003670b8: andeq    r2, r0, r4, asr #22
003670bc: andeq    r1, r0, r0, asr #14
003670c0: andeq    r3, r0, r4, lsr #6
003670c4: andeq    r4, r0, r0, ror #21

# _ZN24BlendedAnimSetControllerC2EP13RootSceneNodei
00476b4c: push     {r4, r5, r6, r7, r8, sl, lr}
00476b50: mov      r5, r2
00476b54: sub      sp, sp, #0x14
00476b58: mov      r2, #1
00476b5c: ldr      r7, [pc, #0x258]
00476b60: mov      r4, r0
00476b64: bl       #0x474e44
00476b68: ldr      r3, [pc, #0x250]
00476b6c: add      r7, pc, r7
00476b70: ldr      r2, [pc, #0x24c]
00476b74: ldr      r3, [r7, r3]
00476b78: mov      r8, #0
00476b7c: ldr      r6, [r7, r2]
00476b80: add      r3, r3, #8
00476b84: str      r3, [r4]
00476b88: mov      r3, #1
00476b8c: strb     r3, [r4, #0x10]
00476b90: mov      r1, r5
00476b94: str      r5, [r4, #8]
00476b98: mov      r0, r6
00476b9c: str      r8, [r4, #0xc]
00476ba0: str      r8, [r4, #0x14]
00476ba4: bl       #0x4762c4
00476ba8: mov      r5, r0
00476bac: ldr      r1, [r4, #8]
00476bb0: mov      r0, r6
00476bb4: bl       #0x4762c4
00476bb8: subs     r3, r5, r8
00476bbc: movne    r3, #1
00476bc0: subs     sl, r0, r8
00476bc4: movne    sl, #1
00476bc8: tst      sl, r3
00476bcc: mov      r6, r0
00476bd0: bne      #0x476c10
00476bd4: cmp      r3, #0
00476bd8: beq      #0x476bec
00476bdc: ldr      r3, [r5]
00476be0: ldr      r0, [r3, #-0xc]
00476be4: add      r0, r5, r0
00476be8: bl       #0x31d584
00476bec: cmp      sl, #0
00476bf0: beq      #0x476c04
00476bf4: ldr      r3, [r6]
00476bf8: ldr      r0, [r3, #-0xc]
00476bfc: add      r0, r6, r0
00476c00: bl       #0x31d584
00476c04: mov      r0, r4
00476c08: add      sp, sp, #0x14
00476c0c: pop      {r4, r5, r6, r7, r8, sl, pc}
00476c10: mov      r0, r5
00476c14: bl       #0x65f11c
00476c18: cmp      r0, r8
00476c1c: ble      #0x476d48
00476c20: mov      r1, #0
00476c24: mov      r0, #0xd0
00476c28: bl       #0x5341ac
00476c2c: mov      r7, r0
00476c30: bl       #0x367018
00476c34: mov      r3, #1
00476c38: str      r5, [sp, #0xc]
00476c3c: strb     r3, [r7, #0x24]
00476c40: ldr      r3, [sp, #0xc]
00476c44: add      r8, r7, #0x28
00476c48: ldr      r2, [r3]
00476c4c: ldr      r2, [r2, #-0xc]
00476c50: add      r3, r3, r2
00476c54: ldr      r2, [r3, #4]
00476c58: add      r2, r2, #1
00476c5c: str      r2, [r3, #4]
00476c60: ldr      r1, [r7, #0x2c]
00476c64: ldr      r3, [r7, #0x30]
00476c68: cmp      r1, r3
00476c6c: beq      #0x476d9c
00476c70: ldr      r3, [sp, #0xc]
00476c74: str      r3, [r1]
00476c78: ldr      r3, [r7, #0x2c]
00476c7c: add      r3, r3, #4
00476c80: str      r3, [r7, #0x2c]
00476c84: mov      r3, #1
00476c88: str      r6, [sp, #0xc]
00476c8c: strb     r3, [r7, #0x24]
00476c90: ldr      r3, [sp, #0xc]
00476c94: ldr      r2, [r3]
00476c98: ldr      r2, [r2, #-0xc]
00476c9c: add      r3, r3, r2
00476ca0: ldr      r2, [r3, #4]
00476ca4: add      r2, r2, #1
00476ca8: str      r2, [r3, #4]
00476cac: ldr      r1, [r7, #0x2c]
00476cb0: ldr      r3, [r7, #0x30]
00476cb4: cmp      r1, r3
00476cb8: beq      #0x476dac
00476cbc: ldr      r3, [sp, #0xc]
00476cc0: str      r3, [r1]
00476cc4: ldr      r3, [r7, #0x2c]
00476cc8: add      r3, r3, #4
00476ccc: str      r3, [r7, #0x2c]
00476cd0: mov      r0, r7
00476cd4: ldr      r3, [r7]
00476cd8: mov      r1, #0
00476cdc: mov      lr, pc
00476ce0: ldr      pc, [r3, #0x88]
00476ce4: ldr      r3, [r7, #0x34]
00476ce8: mov      r2, #0x3f800000
00476cec: mov      r1, r7
00476cf0: str      r2, [r3]
00476cf4: ldr      r3, [r7, #0x34]
00476cf8: mov      r2, #0
00476cfc: str      r2, [r3, #4]
00476d00: ldr      r3, [r4, #4]
00476d04: mov      r0, r3
00476d08: ldr      r3, [r3]
00476d0c: mov      lr, pc
00476d10: ldr      pc, [r3, #0x6c]
00476d14: ldr      r3, [r5]
00476d18: ldr      r0, [r3, #-0xc]
00476d1c: add      r0, r5, r0
00476d20: bl       #0x31d584
00476d24: ldr      r3, [r6]
00476d28: ldr      r0, [r3, #-0xc]
00476d2c: add      r0, r6, r0
00476d30: bl       #0x31d584
00476d34: ldr      r3, [r7]
00476d38: ldr      r0, [r3, #-0xc]
00476d3c: add      r0, r7, r0
00476d40: bl       #0x31d584
00476d44: b        #0x476c04
00476d48: ldr      r3, [pc, #0x78]
00476d4c: ldr      r3, [r7, r3]
00476d50: ldr      r3, [r3]
00476d54: cmp      r3, #2
00476d58: streq    r8, [r8]
00476d5c: beq      #0x476c20
00476d60: cmp      r3, #1
00476d64: bne      #0x476c20
00476d68: ldr      r0, [pc, #0x5c]
00476d6c: ldr      r1, [pc, #0x5c]
00476d70: ldr      r2, [pc, #0x5c]
00476d74: ldr      r0, [r7, r0]
00476d78: ldr      r3, [pc, #0x58]
00476d7c: mov      ip, #0x45
00476d80: add      r1, pc, r1
00476d84: add      r2, pc, r2
00476d88: add      r3, pc, r3
00476d8c: add      r0, r0, #0xa8
00476d90: str      ip, [sp]
00476d94: bl       #0x30e004
00476d98: b        #0x476c20
00476d9c: mov      r0, r8
00476da0: add      r2, sp, #0xc
00476da4: bl       #0x476a9c
00476da8: b        #0x476c84
00476dac: mov      r0, r8
00476db0: add      r2, sp, #0xc
00476db4: bl       #0x476a9c
00476db8: b        #0x476cd0
00476dbc: subseq   sp, r1, r4, lsr #30
00476dc0: andeq    r2, r0, r8, lsr #7
00476dc4: andeq    r4, r0, r8, lsr r8
00476dc8: andeq    r3, r0, r0, asr #19
00476dcc: andeq    r1, r0, r0, asr #19
00476dd0: subeq    r7, r4, r8, asr r6
00476dd4: ldrdeq   r6, r7, [r5], #-0xac
00476dd8: strdeq   r6, r7, [r5], #-0xa8

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEEEE15getBlendedValueEPvPfiSB_
00626b4c: cmp      r3, #1
00626b50: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00626b54: mov      r4, r3
00626b58: mov      fp, r2
00626b5c: beq      #0x626c08
00626b60: cmp      r3, #0
00626b64: moveq    r8, #0
00626b68: moveq    sb, r8
00626b6c: moveq    sl, r8
00626b70: beq      #0x626bf0
00626b74: mov      r8, #0
00626b78: mov      r5, r1
00626b7c: mov      r7, #0
00626b80: mov      sb, r8
00626b84: mov      sl, r8
00626b88: ldr      r6, [fp, r7]
00626b8c: ldr      r1, [r5]
00626b90: add      r7, r7, #4
00626b94: mov      r0, r6
00626b98: bl       #0x30ed6c
00626b9c: mov      r1, r0
00626ba0: mov      r0, r8
00626ba4: bl       #0x30eba4
00626ba8: ldr      r1, [r5, #4]
00626bac: mov      r8, r0
00626bb0: mov      r0, r6
00626bb4: bl       #0x30ed6c
00626bb8: mov      r1, r0
00626bbc: mov      r0, sb
00626bc0: bl       #0x30eba4
00626bc4: ldr      r1, [r5, #8]
00626bc8: mov      sb, r0
00626bcc: mov      r0, r6
00626bd0: bl       #0x30ed6c
00626bd4: mov      r1, r0
00626bd8: mov      r0, sl
00626bdc: bl       #0x30eba4
00626be0: subs     r4, r4, #1
00626be4: mov      sl, r0
00626be8: add      r5, r5, #0xc
00626bec: bne      #0x626b88
00626bf0: ldr      r3, [sp, #0x28]
00626bf4: str      r8, [r3], #4
00626bf8: ldr      r2, [sp, #0x28]
00626bfc: str      sb, [r2, #4]
00626c00: str      sl, [r3, #4]
00626c04: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00626c08: mov      r2, r1
00626c0c: ldr      r0, [r2], #4
00626c10: ldr      r3, [sp, #0x28]
00626c14: str      r0, [r3], #4
00626c18: ldr      r1, [r1, #4]
00626c1c: ldr      r0, [sp, #0x28]
00626c20: str      r1, [r0, #4]
00626c24: ldr      r2, [r2, #4]
00626c28: str      r2, [r3, #4]
00626c2c: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE10applyValueEPvSC_PNS1_15CApplicatorInfoE
00618fe4: mov      ip, #0
00618fe8: mov      r3, r1
00618fec: ldrh     r1, [ip, #8]
00618ff0: mov      r0, r2
00618ff4: mov      r2, ip
00618ff8: b        #0x5c6b8c

# _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
00629a1c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00629a20: cmp      r2, #1
00629a24: sub      sp, sp, #0x1c
00629a28: mov      sl, #0
00629a2c: mov      r4, r2
00629a30: mov      r5, r1
00629a34: str      r3, [sp, #4]
00629a38: str      sl, [sp, #0x14]
00629a3c: beq      #0x629af0
00629a40: cmp      r2, #0
00629a44: moveq    fp, sl
00629a48: moveq    sb, sl
00629a4c: beq      #0x629ac8
00629a50: mov      r6, r0
00629a54: mov      r8, #0
00629a58: mov      fp, sl
00629a5c: mov      sb, sl
00629a60: ldr      r7, [r5, r8]
00629a64: ldr      r1, [r6]
00629a68: add      r8, r8, #4
00629a6c: mov      r0, r7
00629a70: bl       #0x30ed6c
00629a74: mov      r1, r0
00629a78: mov      r0, sl
00629a7c: bl       #0x30eba4
00629a80: ldr      r1, [r6, #4]
00629a84: mov      sl, r0
00629a88: mov      r0, r7
00629a8c: bl       #0x30ed6c
00629a90: mov      r1, r0
00629a94: mov      r0, fp
00629a98: bl       #0x30eba4
00629a9c: ldr      r1, [r6, #8]
00629aa0: mov      fp, r0
00629aa4: mov      r0, r7
00629aa8: bl       #0x30ed6c
00629aac: mov      r1, r0
00629ab0: mov      r0, sb
00629ab4: bl       #0x30eba4
00629ab8: subs     r4, r4, #1
00629abc: mov      sb, r0
00629ac0: add      r6, r6, #0xc
00629ac4: bne      #0x629a60
00629ac8: add      r1, sp, #0x18
00629acc: str      sl, [r1, #-0xc]!
00629ad0: str      fp, [sp, #0x10]
00629ad4: str      sb, [r1, #8]
00629ad8: ldr      r0, [sp, #4]
00629adc: ldr      r3, [r0]
00629ae0: mov      lr, pc
00629ae4: ldr      pc, [r3, #0xa4]
00629ae8: add      sp, sp, #0x1c
00629aec: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00629af0: mov      r3, r0
00629af4: ldr      ip, [r3], #4
00629af8: ldr      r2, [r0, #4]
00629afc: add      r1, sp, #0x18
00629b00: ldr      r3, [r3, #4]
00629b04: str      ip, [r1, #-0xc]!
00629b08: str      r2, [sp, #0x10]
00629b0c: str      r3, [r1, #8]
00629b10: b        #0x629ad8

# _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
0062cfbc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0062cfc0: cmp      r2, #1
0062cfc4: sub      sp, sp, #0x1c
0062cfc8: mov      sl, #0
0062cfcc: mov      r4, r2
0062cfd0: mov      r5, r1
0062cfd4: str      r3, [sp, #4]
0062cfd8: str      sl, [sp, #0x14]
0062cfdc: beq      #0x62d090
0062cfe0: cmp      r2, #0
0062cfe4: moveq    fp, sl
0062cfe8: moveq    sb, sl
0062cfec: beq      #0x62d068
0062cff0: mov      r6, r0
0062cff4: mov      r8, #0
0062cff8: mov      fp, sl
0062cffc: mov      sb, sl
0062d000: ldr      r7, [r5, r8]
0062d004: ldr      r1, [r6]
0062d008: add      r8, r8, #4
0062d00c: mov      r0, r7
0062d010: bl       #0x30ed6c
0062d014: mov      r1, r0
0062d018: mov      r0, sl
0062d01c: bl       #0x30eba4
0062d020: ldr      r1, [r6, #4]
0062d024: mov      sl, r0
0062d028: mov      r0, r7
0062d02c: bl       #0x30ed6c
0062d030: mov      r1, r0
0062d034: mov      r0, fp
0062d038: bl       #0x30eba4
0062d03c: ldr      r1, [r6, #8]
0062d040: mov      fp, r0
0062d044: mov      r0, r7
0062d048: bl       #0x30ed6c
0062d04c: mov      r1, r0
0062d050: mov      r0, sb
0062d054: bl       #0x30eba4
0062d058: subs     r4, r4, #1
0062d05c: mov      sb, r0
0062d060: add      r6, r6, #0xc
0062d064: bne      #0x62d000
0062d068: add      r1, sp, #0x18
0062d06c: str      sl, [r1, #-0xc]!
0062d070: str      fp, [sp, #0x10]
0062d074: str      sb, [r1, #8]
0062d078: ldr      r0, [sp, #4]
0062d07c: ldr      r3, [r0]
0062d080: mov      lr, pc
0062d084: ldr      pc, [r3, #0x94]
0062d088: add      sp, sp, #0x1c
0062d08c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0062d090: mov      r3, r0
0062d094: ldr      ip, [r3], #4
0062d098: ldr      r2, [r0, #4]
0062d09c: add      r1, sp, #0x18
0062d0a0: ldr      r3, [r3, #4]
0062d0a4: str      ip, [r1, #-0xc]!
0062d0a8: str      r2, [sp, #0x10]
0062d0ac: str      r3, [r1, #8]
0062d0b0: b        #0x62d078

# _ZN24BlendedAnimSetController12SetCallbacksEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
004766fc: push     {r4, r5, r6, r7, r8, lr}
00476700: mov      r5, r1
00476704: mov      r1, #0
00476708: mov      r7, r2
0047670c: mov      r6, r3
00476710: ldr      r4, [sp, #0x18]
00476714: bl       #0x4748b8
00476718: cmp      r0, #0
0047671c: beq      #0x476738
00476720: mov      r1, r5
00476724: mov      r2, r7
00476728: mov      r3, r6
0047672c: str      r4, [sp, #0x18]
00476730: pop      {r4, r5, r6, r7, r8, lr}
00476734: b        #0x366eb8
00476738: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEEEE15getBlendedValueEPvPfiSD_
006278a8: cmp      r3, #1
006278ac: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
006278b0: mov      r4, r3
006278b4: mov      fp, r2
006278b8: beq      #0x627964
006278bc: cmp      r3, #0
006278c0: moveq    r8, #0
006278c4: moveq    sb, r8
006278c8: moveq    sl, r8
006278cc: beq      #0x62794c
006278d0: mov      r8, #0
006278d4: mov      r5, r1
006278d8: mov      r7, #0
006278dc: mov      sb, r8
006278e0: mov      sl, r8
006278e4: ldr      r6, [fp, r7]
006278e8: ldr      r1, [r5]
006278ec: add      r7, r7, #4
006278f0: mov      r0, r6
006278f4: bl       #0x30ed6c
006278f8: mov      r1, r0
006278fc: mov      r0, r8
00627900: bl       #0x30eba4
00627904: ldr      r1, [r5, #4]
00627908: mov      r8, r0
0062790c: mov      r0, r6
00627910: bl       #0x30ed6c
00627914: mov      r1, r0
00627918: mov      r0, sb
0062791c: bl       #0x30eba4
00627920: ldr      r1, [r5, #8]
00627924: mov      sb, r0
00627928: mov      r0, r6
0062792c: bl       #0x30ed6c
00627930: mov      r1, r0
00627934: mov      r0, sl
00627938: bl       #0x30eba4
0062793c: subs     r4, r4, #1
00627940: mov      sl, r0
00627944: add      r5, r5, #0xc
00627948: bne      #0x6278e4
0062794c: ldr      r3, [sp, #0x28]
00627950: str      r8, [r3], #4
00627954: ldr      r2, [sp, #0x28]
00627958: str      sb, [r2, #4]
0062795c: str      sl, [r3, #4]
00627960: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00627964: mov      r2, r1
00627968: ldr      r0, [r2], #4
0062796c: ldr      r3, [sp, #0x28]
00627970: str      r0, [r3], #4
00627974: ldr      r1, [r1, #4]
00627978: ldr      r0, [sp, #0x28]
0062797c: str      r1, [r0, #4]
00627980: ldr      r2, [r2, #4]
00627984: str      r2, [r3, #4]
00627988: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada25CSceneNodeAnimatorBlender17getAnimationValueEiiPv
0065e34c: bx       lr

# _ZN15AnimatorBlender7compileEPSt6vectorIhN6glitch4core10SAllocatorIhLNS1_6memory13E_MEMORY_HINTE0EEEE
00366764: b        #0x65ed98

# _ZN6glitch7collada21CSceneNodeAnimatorSet14getTargetsSizeEv
0065f1ac: push     {r4, r5, r6, r7, r8, lr}
0065f1b0: ldr      r3, [r0, #0x24]
0065f1b4: mov      r7, r0
0065f1b8: ldr      r6, [r3, #0x3c]
0065f1bc: cmp      r6, #0
0065f1c0: moveq    r5, r6
0065f1c4: beq      #0x65f200
0065f1c8: mov      r4, #0
0065f1cc: mov      r5, r4
0065f1d0: b        #0x65f1d8
0065f1d4: ldr      r3, [r7, #0x24]
0065f1d8: ldr      r3, [r3, #0x18]
0065f1dc: ldr      r3, [r3, r4, lsl #2]
0065f1e0: add      r4, r4, #1
0065f1e4: mov      r0, r3
0065f1e8: ldr      r3, [r3]
0065f1ec: mov      lr, pc
0065f1f0: ldr      pc, [r3, #8]
0065f1f4: cmp      r4, r6
0065f1f8: add      r5, r5, r0
0065f1fc: bne      #0x65f1d4
0065f200: mov      r0, r5
0065f204: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada21CSceneNodeAnimatorSet9setTargetEiPvPKNS0_15animation_track15CApplicatorInfoE
0065f208: push     {r4, r5, r6, lr}
0065f20c: ldr      ip, [r0, #0x28]
0065f210: mov      r6, r3
0065f214: mov      r4, r0
0065f218: str      r2, [ip, r1, lsl #2]
0065f21c: ldr      r3, [r0, #0x34]
0065f220: mov      r5, r1
0065f224: ldr      r3, [r3, r1, lsl #2]
0065f228: cmp      r3, #0
0065f22c: beq      #0x65f24c
0065f230: mov      r0, r3
0065f234: ldr      r3, [r3]
0065f238: mov      lr, pc
0065f23c: ldr      pc, [r3, #4]
0065f240: ldr      r3, [r4, #0x34]
0065f244: mov      r2, #0
0065f248: str      r2, [r3, r5, lsl #2]
0065f24c: cmp      r6, #0
0065f250: beq      #0x65f26c
0065f254: mov      r0, r6
0065f258: ldr      r3, [r6]
0065f25c: ldr      r4, [r4, #0x34]
0065f260: mov      lr, pc
0065f264: ldr      pc, [r3, #8]
0065f268: str      r0, [r4, r5, lsl #2]
0065f26c: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada15animation_track11CVector3dEx17getBlendedValueExEPvPfiS3_f
006e41ec: push     {r4, r5, r6, r7, r8, lr}
006e41f0: sub      sp, sp, #0x10
006e41f4: ldr      r5, [sp, #0x28]
006e41f8: mov      ip, #0
006e41fc: mov      r4, r3
006e4200: add      r3, sp, #4
006e4204: str      ip, [sp, #0xc]
006e4208: str      ip, [sp, #4]
006e420c: str      ip, [sp, #8]
006e4210: bl       #0x6e3fb0
006e4214: mov      r1, r5
006e4218: mov      r0, #0x3f800000
006e421c: bl       #0x30e3ac
006e4220: ldr      r1, [r4, #4]
006e4224: mov      r6, r0
006e4228: bl       #0x30ed6c
006e422c: ldr      r1, [sp, #8]
006e4230: mov      r7, r0
006e4234: mov      r0, r5
006e4238: bl       #0x30ed6c
006e423c: mov      r1, r0
006e4240: mov      r0, r7
006e4244: bl       #0x30eba4
006e4248: ldr      r1, [r4, #8]
006e424c: mov      r7, r0
006e4250: mov      r0, r6
006e4254: bl       #0x30ed6c
006e4258: ldr      r1, [sp, #0xc]
006e425c: mov      r8, r0
006e4260: mov      r0, r5
006e4264: bl       #0x30ed6c
006e4268: mov      r1, r0
006e426c: mov      r0, r8
006e4270: bl       #0x30eba4
006e4274: ldr      r1, [r4]
006e4278: mov      r8, r0
006e427c: mov      r0, r6
006e4280: bl       #0x30ed6c
006e4284: ldr      r1, [sp, #4]
006e4288: mov      r6, r0
006e428c: mov      r0, r5
006e4290: bl       #0x30ed6c
006e4294: mov      r1, r0
006e4298: mov      r0, r6
006e429c: bl       #0x30eba4
006e42a0: str      r8, [r4, #8]
006e42a4: str      r0, [r4]
006e42a8: str      r7, [r4, #4]
006e42ac: add      sp, sp, #0x10
006e42b0: pop      {r4, r5, r6, r7, r8, pc}

# _ZN14AnimApplicator10SetRefNodeEPN6glitch5scene10ISceneNodeE
0036473c: push     {r4, r5, r6, r7, r8, lr}
00364740: ldr      r3, [r0, #8]
00364744: mov      r5, r0
00364748: mov      r4, r1
0036474c: cmp      r3, #0
00364750: beq      #0x364764
00364754: ldr      r2, [r3]
00364758: ldr      r0, [r2, #-0xc]
0036475c: add      r0, r3, r0
00364760: bl       #0x31d584
00364764: cmp      r4, #0
00364768: str      r4, [r5, #8]
0036476c: beq      #0x364834
00364770: ldr      r3, [r4]
00364774: mvn      r2, #0
00364778: ldr      r3, [r3, #-0xc]
0036477c: add      r4, r4, r3
00364780: ldr      r3, [r4, #4]
00364784: add      r3, r3, #1
00364788: str      r3, [r4, #4]
0036478c: ldr      r3, [r5, #4]
00364790: str      r2, [r5, #0xc]
00364794: mov      r0, r3
00364798: ldr      r3, [r3]
0036479c: mov      lr, pc
003647a0: ldr      pc, [r3, #0x70]
003647a4: subs     r7, r0, #0
003647a8: beq      #0x364834
003647ac: mov      r4, #0
003647b0: b        #0x3647c0
003647b4: add      r4, r4, #1
003647b8: cmp      r7, r4
003647bc: beq      #0x364834
003647c0: ldr      r3, [r5, #4]
003647c4: mov      r1, r4
003647c8: mov      r0, r3
003647cc: ldr      r3, [r3]
003647d0: mov      lr, pc
003647d4: ldr      pc, [r3, #0x54]
003647d8: ldr      r3, [r5, #8]
003647dc: mov      r6, r0
003647e0: mov      r0, r3
003647e4: ldr      r3, [r3]
003647e8: mov      lr, pc
003647ec: ldr      pc, [r3, #0x54]
003647f0: mov      r1, r0
003647f4: ldr      r0, [r6, #4]
003647f8: bl       #0x30e31c
003647fc: cmp      r0, #0
00364800: bne      #0x3647b4
00364804: ldr      r3, [r5, #4]
00364808: mov      r1, r4
0036480c: mov      r0, r3
00364810: ldr      r3, [r3]
00364814: mov      lr, pc
00364818: ldr      pc, [r3, #0x54]
0036481c: ldr      r3, [r0, #8]
00364820: cmp      r3, #1
00364824: streq    r4, [r5, #0xc]
00364828: add      r4, r4, #1
0036482c: cmp      r7, r4
00364830: bne      #0x3647c0
00364834: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada18ISceneNodeAnimator9forceBindEv
00667f18: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00667f1c: ldr      r6, [r0, #0x10]
00667f20: ldr      sl, [pc, #0xeec]
00667f24: sub      sp, sp, #0x24
00667f28: cmp      r6, #0
00667f2c: mov      r4, r0
00667f30: add      sl, pc, sl
00667f34: beq      #0x6681e4
00667f38: ldr      r3, [r0]
00667f3c: mov      lr, pc
00667f40: ldr      pc, [r3, #0x70]
00667f44: subs     r7, r0, #0
00667f48: ble      #0x6681e4
00667f4c: ldr      r3, [pc, #0xec4]
00667f50: ldr      fp, [pc, #0xec4]
00667f54: mov      r5, #0
00667f58: add      r3, pc, r3
00667f5c: str      r3, [sp]
00667f60: ldr      r3, [pc, #0xeb8]
00667f64: add      r3, pc, r3
00667f68: str      r3, [sp, #4]
00667f6c: ldr      r3, [pc, #0xeb0]
00667f70: add      r3, pc, r3
00667f74: str      r3, [sp, #8]
00667f78: ldr      r3, [pc, #0xea8]
00667f7c: add      r3, pc, r3
00667f80: str      r3, [sp, #0xc]
00667f84: mov      r1, r5
00667f88: ldr      r3, [r4]
00667f8c: mov      r0, r4
00667f90: mov      lr, pc
00667f94: ldr      pc, [r3, #0x6c]
00667f98: ldr      r3, [r4]
00667f9c: mov      sb, r0
00667fa0: mov      r1, r5
00667fa4: mov      r0, r4
00667fa8: mov      lr, pc
00667fac: ldr      pc, [r3, #0x54]
00667fb0: ldr      r3, [r0, #8]
00667fb4: sub      r3, r3, #1
00667fb8: cmp      r3, #0x5a
00667fbc: addls    pc, pc, r3, lsl #2
00667fc0: b        #0x6681ec
00667fc4: b        #0x66820c
00667fc8: b        #0x66820c
00667fcc: b        #0x66820c
00667fd0: b        #0x66820c
00667fd4: b        #0x66820c
00667fd8: b        #0x6681ec
00667fdc: b        #0x6681ec
00667fe0: b        #0x6681ec
00667fe4: b        #0x66820c
00667fe8: b        #0x66820c
00667fec: b        #0x66820c
00667ff0: b        #0x66820c
00667ff4: b        #0x66820c
00667ff8: b        #0x668258
00667ffc: b        #0x6682ac
00668000: b        #0x6682cc
00668004: b        #0x6681ec
00668008: b        #0x6681ec
0066800c: b        #0x6681ec
00668010: b        #0x66820c
00668014: b        #0x6681ec
00668018: b        #0x6681ec
0066801c: b        #0x6681ec
00668020: b        #0x6681ec
00668024: b        #0x6681ec
00668028: b        #0x66830c
0066802c: b        #0x6681ec
00668030: b        #0x668354
00668034: b        #0x66839c
00668038: b        #0x6683e4
0066803c: b        #0x66842c
00668040: b        #0x668474
00668044: b        #0x6684bc
00668048: b        #0x668504
0066804c: b        #0x66854c
00668050: b        #0x668594
00668054: b        #0x6685dc
00668058: b        #0x668624
0066805c: b        #0x66866c
00668060: b        #0x6686b4
00668064: b        #0x6686fc
00668068: b        #0x668744
0066806c: b        #0x66878c
00668070: b        #0x6687d4
00668074: b        #0x66881c
00668078: b        #0x668864
0066807c: b        #0x6688ac
00668080: b        #0x6688f4
00668084: b        #0x66893c
00668088: b        #0x668984
0066808c: b        #0x6689cc
00668090: b        #0x668a14
00668094: b        #0x668a5c
00668098: b        #0x668aac
0066809c: b        #0x668af4
006680a0: b        #0x668b3c
006680a4: b        #0x668b8c
006680a8: b        #0x668bd4
006680ac: b        #0x668c1c
006680b0: b        #0x668c64
006680b4: b        #0x668cac
006680b8: b        #0x668cf4
006680bc: b        #0x668d3c
006680c0: b        #0x668d84
006680c4: b        #0x668dcc
006680c8: b        #0x668ecc
006680cc: b        #0x668f1c
006680d0: b        #0x668f64
006680d4: b        #0x668fac
006680d8: b        #0x668ff0
006680dc: b        #0x6681ec
006680e0: b        #0x6681ec
006680e4: b        #0x6681ec
006680e8: b        #0x6681ec
006680ec: b        #0x6681ec
006680f0: b        #0x6681ec
006680f4: b        #0x6681ec
006680f8: b        #0x6681ec
006680fc: b        #0x6681ec
00668100: b        #0x6681ec
00668104: b        #0x6681ec
00668108: b        #0x6681ec
0066810c: b        #0x6681ec
00668110: b        #0x6681ec
00668114: b        #0x6681ec
00668118: b        #0x668130
0066811c: b        #0x668130
00668120: b        #0x668130
00668124: b        #0x668130
00668128: b        #0x668130
0066812c: b        #0x668130
00668130: add      r8, sp, #0x1c
00668134: mov      r2, sb
00668138: mov      r0, r8
0066813c: mov      r1, r6
00668140: mov      r3, #0
00668144: bl       #0x65ca30
00668148: ldr      r2, [sp, #0x1c]
0066814c: cmp      r2, #0
00668150: beq      #0x6694a4
00668154: mov      r1, r5
00668158: ldr      r3, [r4]
0066815c: mov      r0, r4
00668160: mov      lr, pc
00668164: ldr      pc, [r3, #0x54]
00668168: ldr      r3, [pc, #0xcbc]
0066816c: mov      r2, #0
00668170: mvn      r1, #0
00668174: ldr      r3, [sl, r3]
00668178: str      r2, [sp, #0x14]
0066817c: str      r1, [sp, #0x18]
00668180: add      r3, r3, #8
00668184: str      r3, [sp, #0x10]
00668188: ldr      r3, [r0, #0xc]
0066818c: str      r3, [sp, #0x14]
00668190: ldr      r3, [sp, #0x1c]
00668194: ldr      r1, [r0, #0xc]
00668198: ldr      r0, [r3, #4]
0066819c: bl       #0x5d308c
006681a0: str      r0, [sp, #0x18]
006681a4: add      r3, sp, #0x10
006681a8: mov      r0, r4
006681ac: ldr      ip, [r4]
006681b0: mov      r1, r5
006681b4: ldr      r2, [sp, #0x1c]
006681b8: mov      lr, pc
006681bc: ldr      pc, [ip, #0x68]
006681c0: ldr      r3, [pc, #0xc68]
006681c4: mov      r0, r8
006681c8: ldr      r3, [sl, r3]
006681cc: add      r3, r3, #8
006681d0: str      r3, [sp, #0x10]
006681d4: bl       #0x310be8
006681d8: add      r5, r5, #1
006681dc: cmp      r5, r7
006681e0: bne      #0x667f84
006681e4: add      sp, sp, #0x24
006681e8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006681ec: mov      r2, #0
006681f0: ldr      ip, [r4]
006681f4: mov      r0, r4
006681f8: mov      r1, r5
006681fc: mov      r3, r2
00668200: mov      lr, pc
00668204: ldr      pc, [ip, #0x68]
00668208: b        #0x6681d8
0066820c: mov      r1, sb
00668210: mov      r0, r6
00668214: bl       #0x5985e4
00668218: mov      r8, r0
0066821c: ldr      ip, [r4]
00668220: mov      r0, r4
00668224: mov      r1, r5
00668228: mov      r2, r8
0066822c: mov      r3, #0
00668230: mov      lr, pc
00668234: ldr      pc, [ip, #0x68]
00668238: cmp      r8, #0
0066823c: beq      #0x6681d8
00668240: mov      r0, r8
00668244: ldr      r3, [r8]
00668248: mov      r1, r4
0066824c: mov      lr, pc
00668250: ldr      pc, [r3, #0x78]
00668254: b        #0x6681d8
00668258: mov      r1, sb
0066825c: mov      r0, r6
00668260: bl       #0x65b5b0
00668264: subs     r8, r0, #0
00668268: beq      #0x669484
0066826c: mov      r1, r5
00668270: ldr      r3, [r4]
00668274: mov      r0, r4
00668278: mov      lr, pc
0066827c: ldr      pc, [r3, #0x54]
00668280: ldr      r3, [r8, #0x24]
00668284: ldrb     r2, [r0, #0xc]
00668288: ldr      ip, [r4]
0066828c: mov      r0, r4
00668290: add      r2, r3, r2, lsl #3
00668294: add      r2, r2, #0xc
00668298: mov      r1, r5
0066829c: mov      r3, #0
006682a0: mov      lr, pc
006682a4: ldr      pc, [ip, #0x68]
006682a8: b        #0x6681d8
006682ac: ldr      ip, [r4]
006682b0: mov      r0, r4
006682b4: mov      r1, r5
006682b8: mov      r2, r6
006682bc: mov      r3, #0
006682c0: mov      lr, pc
006682c4: ldr      pc, [ip, #0x68]
006682c8: b        #0x6681d8
006682cc: mov      r1, sb
006682d0: mov      r0, r6
006682d4: bl       #0x65b5f4
006682d8: subs     r2, r0, #0
006682dc: ldreq    ip, [r4]
006682e0: moveq    r0, r4
006682e4: moveq    r1, r5
006682e8: moveq    r3, r2
006682ec: ldrne    r2, [r2, #0x134]
006682f0: ldrne    ip, [r4]
006682f4: movne    r0, r4
006682f8: movne    r1, r5
006682fc: movne    r3, #0
00668300: mov      lr, pc
00668304: ldr      pc, [ip, #0x68]
00668308: b        #0x6681d8
0066830c: add      r8, sp, #0x1c
00668310: mov      r3, #0
00668314: mov      r2, sb
00668318: mov      r0, r8
0066831c: mov      r1, r6
00668320: bl       #0x65ca30
00668324: ldr      r2, [sp, #0x1c]
00668328: mov      r0, r4
0066832c: ldr      ip, [r4]
00668330: cmp      r2, #0
00668334: mov      r1, r5
00668338: moveq    r3, r2
0066833c: movne    r3, #0
00668340: mov      lr, pc
00668344: ldr      pc, [ip, #0x68]
00668348: mov      r0, r8
0066834c: bl       #0x310be8
00668350: b        #0x6681d8
00668354: mov      r1, sb
00668358: mov      r0, r6
0066835c: bl       #0x65b4e4
00668360: subs     r2, r0, #0
00668364: beq      #0x669468
00668368: ldr      r1, [pc, #0xac4]
0066836c: ldr      ip, [r4]
00668370: ldr      r3, [r2]
00668374: add      r1, pc, r1
00668378: ldr      r8, [ip, #0x68]
0066837c: mov      lr, pc
00668380: ldr      pc, [r3, #0xfc]
00668384: mov      r1, r5
00668388: mov      r2, r0
0066838c: mov      r3, #0
00668390: mov      r0, r4
00668394: blx      r8
00668398: b        #0x6681d8
0066839c: mov      r1, sb
006683a0: mov      r0, r6
006683a4: bl       #0x65b4e4
006683a8: subs     r2, r0, #0
006683ac: beq      #0x66944c
006683b0: ldr      r1, [pc, #0xa80]
006683b4: ldr      ip, [r4]
006683b8: ldr      r3, [r2]
006683bc: add      r1, pc, r1
006683c0: ldr      r8, [ip, #0x68]
006683c4: mov      lr, pc
006683c8: ldr      pc, [r3, #0xfc]
006683cc: mov      r1, r5
006683d0: mov      r2, r0
006683d4: mov      r3, #0
006683d8: mov      r0, r4
006683dc: blx      r8
006683e0: b        #0x6681d8
006683e4: mov      r1, sb
006683e8: mov      r0, r6
006683ec: bl       #0x65b4e4
006683f0: subs     r2, r0, #0
006683f4: beq      #0x669430
006683f8: ldr      r1, [pc, #0xa3c]
006683fc: ldr      ip, [r4]
00668400: ldr      r3, [r2]
00668404: add      r1, pc, r1
00668408: ldr      r8, [ip, #0x68]
0066840c: mov      lr, pc
00668410: ldr      pc, [r3, #0xfc]
00668414: mov      r1, r5
00668418: mov      r2, r0
0066841c: mov      r3, #0
00668420: mov      r0, r4
00668424: blx      r8
00668428: b        #0x6681d8
0066842c: mov      r1, sb
00668430: mov      r0, r6
00668434: bl       #0x65b4e4
00668438: subs     r2, r0, #0
0066843c: beq      #0x669414
00668440: ldr      r1, [pc, #0x9f8]
00668444: ldr      ip, [r4]
00668448: ldr      r3, [r2]
0066844c: add      r1, pc, r1
00668450: ldr      r8, [ip, #0x68]
00668454: mov      lr, pc
00668458: ldr      pc, [r3, #0xfc]
0066845c: mov      r1, r5
00668460: mov      r2, r0
00668464: mov      r3, #0
00668468: mov      r0, r4
0066846c: blx      r8
00668470: b        #0x6681d8
00668474: mov      r1, sb
00668478: mov      r0, r6
0066847c: bl       #0x65b4e4
00668480: subs     r2, r0, #0
00668484: beq      #0x6693f8
00668488: ldr      r1, [pc, #0x9b4]
0066848c: ldr      ip, [r4]
00668490: ldr      r3, [r2]
00668494: add      r1, pc, r1
00668498: ldr      r8, [ip, #0x68]
0066849c: mov      lr, pc
006684a0: ldr      pc, [r3, #0xfc]
006684a4: mov      r1, r5
006684a8: mov      r2, r0
006684ac: mov      r3, #0
006684b0: mov      r0, r4
006684b4: blx      r8
006684b8: b        #0x6681d8
006684bc: mov      r1, sb
006684c0: mov      r0, r6
006684c4: bl       #0x65b4e4
006684c8: subs     r2, r0, #0
006684cc: beq      #0x6693dc
006684d0: ldr      r1, [pc, #0x970]
006684d4: ldr      ip, [r4]
006684d8: ldr      r3, [r2]
006684dc: add      r1, pc, r1
006684e0: ldr      r8, [ip, #0x68]
006684e4: mov      lr, pc
006684e8: ldr      pc, [r3, #0xfc]
006684ec: mov      r1, r5
006684f0: mov      r2, r0
006684f4: mov      r3, #0
006684f8: mov      r0, r4
006684fc: blx      r8
00668500: b        #0x6681d8
00668504: mov      r1, sb
00668508: mov      r0, r6
0066850c: bl       #0x65b4e4
00668510: subs     r2, r0, #0
00668514: beq      #0x6693c0
00668518: ldr      r1, [pc, #0x92c]
0066851c: ldr      ip, [r4]
00668520: ldr      r3, [r2]
00668524: add      r1, pc, r1
00668528: ldr      r8, [ip, #0x68]
0066852c: mov      lr, pc
00668530: ldr      pc, [r3, #0xfc]
00668534: mov      r1, r5
00668538: mov      r2, r0
0066853c: mov      r3, #0
00668540: mov      r0, r4
00668544: blx      r8
00668548: b        #0x6681d8
0066854c: mov      r1, sb
00668550: mov      r0, r6
00668554: bl       #0x65b4e4
00668558: subs     r2, r0, #0
0066855c: beq      #0x6693a4
00668560: ldr      r1, [pc, #0x8e8]
00668564: ldr      ip, [r4]
00668568: ldr      r3, [r2]
0066856c: add      r1, pc, r1
00668570: ldr      r8, [ip, #0x68]
00668574: mov      lr, pc
00668578: ldr      pc, [r3, #0xfc]
0066857c: mov      r1, r5
00668580: mov      r2, r0
00668584: mov      r3, #0
00668588: mov      r0, r4
0066858c: blx      r8
00668590: b        #0x6681d8
00668594: mov      r1, sb
00668598: mov      r0, r6
0066859c: bl       #0x65b4e4
006685a0: subs     r2, r0, #0
006685a4: beq      #0x669388
006685a8: ldr      r1, [pc, #0x8a4]
006685ac: ldr      ip, [r4]
006685b0: ldr      r3, [r2]
006685b4: add      r1, pc, r1
006685b8: ldr      r8, [ip, #0x68]
006685bc: mov      lr, pc
006685c0: ldr      pc, [r3, #0xfc]
006685c4: mov      r1, r5
006685c8: mov      r2, r0
006685cc: mov      r3, #0
006685d0: mov      r0, r4
006685d4: blx      r8
006685d8: b        #0x6681d8
006685dc: mov      r1, sb
006685e0: mov      r0, r6
006685e4: bl       #0x65b4e4
006685e8: subs     r2, r0, #0
006685ec: beq      #0x66936c
006685f0: ldr      r1, [pc, #0x860]
006685f4: ldr      ip, [r4]
006685f8: ldr      r3, [r2]
006685fc: add      r1, pc, r1
00668600: ldr      r8, [ip, #0x68]
00668604: mov      lr, pc
00668608: ldr      pc, [r3, #0xfc]
0066860c: mov      r1, r5
00668610: mov      r2, r0
00668614: mov      r3, #0
00668618: mov      r0, r4
0066861c: blx      r8
00668620: b        #0x6681d8
00668624: mov      r1, sb
00668628: mov      r0, r6
0066862c: bl       #0x65b4e4
00668630: subs     r2, r0, #0
00668634: beq      #0x669350
00668638: ldr      r1, [pc, #0x81c]
0066863c: ldr      ip, [r4]
00668640: ldr      r3, [r2]
00668644: add      r1, pc, r1
00668648: ldr      r8, [ip, #0x68]
0066864c: mov      lr, pc
00668650: ldr      pc, [r3, #0xfc]
00668654: mov      r1, r5
00668658: mov      r2, r0
0066865c: mov      r3, #0
00668660: mov      r0, r4
00668664: blx      r8
00668668: b        #0x6681d8
0066866c: mov      r1, sb
00668670: mov      r0, r6
00668674: bl       #0x65b4e4
00668678: subs     r2, r0, #0
0066867c: beq      #0x669334
00668680: ldr      r1, [pc, #0x7d8]
00668684: ldr      ip, [r4]
00668688: ldr      r3, [r2]
0066868c: add      r1, pc, r1
00668690: ldr      r8, [ip, #0x68]
00668694: mov      lr, pc
00668698: ldr      pc, [r3, #0xfc]
0066869c: mov      r1, r5
006686a0: mov      r2, r0
006686a4: mov      r3, #0
006686a8: mov      r0, r4
006686ac: blx      r8
006686b0: b        #0x6681d8
006686b4: mov      r1, sb
006686b8: mov      r0, r6
006686bc: bl       #0x65b4e4
006686c0: subs     r2, r0, #0
006686c4: beq      #0x669318
006686c8: ldr      r1, [pc, #0x794]
006686cc: ldr      ip, [r4]
006686d0: ldr      r3, [r2]
006686d4: add      r1, pc, r1
006686d8: ldr      r8, [ip, #0x68]
006686dc: mov      lr, pc
006686e0: ldr      pc, [r3, #0xfc]
006686e4: mov      r1, r5
006686e8: mov      r2, r0
006686ec: mov      r3, #0
006686f0: mov      r0, r4
006686f4: blx      r8
006686f8: b        #0x6681d8
006686fc: mov      r1, sb
00668700: mov      r0, r6
00668704: bl       #0x65b4e4
00668708: subs     r2, r0, #0
0066870c: beq      #0x6692fc
00668710: ldr      r1, [pc, #0x750]
00668714: ldr      ip, [r4]
00668718: ldr      r3, [r2]
0066871c: add      r1, pc, r1
00668720: ldr      r8, [ip, #0x68]
00668724: mov      lr, pc
00668728: ldr      pc, [r3, #0xfc]
0066872c: mov      r1, r5
00668730: mov      r2, r0
00668734: mov      r3, #0
00668738: mov      r0, r4
0066873c: blx      r8
00668740: b        #0x6681d8
00668744: mov      r1, sb
00668748: mov      r0, r6
0066874c: bl       #0x65b4e4
00668750: subs     r2, r0, #0
00668754: beq      #0x6692e0
00668758: ldr      r1, [pc, #0x70c]
0066875c: ldr      ip, [r4]
00668760: ldr      r3, [r2]
00668764: add      r1, pc, r1
00668768: ldr      r8, [ip, #0x68]
0066876c: mov      lr, pc
00668770: ldr      pc, [r3, #0xfc]
00668774: mov      r1, r5
00668778: mov      r2, r0
0066877c: mov      r3, #0
00668780: mov      r0, r4
00668784: blx      r8
00668788: b        #0x6681d8
0066878c: mov      r1, sb
00668790: mov      r0, r6
00668794: bl       #0x65b4e4
00668798: subs     r2, r0, #0
0066879c: beq      #0x6692c4
006687a0: ldr      r1, [pc, #0x6c8]
006687a4: ldr      ip, [r4]
006687a8: ldr      r3, [r2]
006687ac: add      r1, pc, r1
006687b0: ldr      r8, [ip, #0x68]
006687b4: mov      lr, pc
006687b8: ldr      pc, [r3, #0xfc]
006687bc: mov      r1, r5
006687c0: mov      r2, r0
006687c4: mov      r3, #0
006687c8: mov      r0, r4
006687cc: blx      r8
006687d0: b        #0x6681d8
006687d4: mov      r1, sb
006687d8: mov      r0, r6
006687dc: bl       #0x65b4e4
006687e0: subs     r2, r0, #0
006687e4: beq      #0x6692a8
006687e8: ldr      r1, [pc, #0x684]
006687ec: ldr      ip, [r4]
006687f0: ldr      r3, [r2]
006687f4: add      r1, pc, r1
006687f8: ldr      r8, [ip, #0x68]
006687fc: mov      lr, pc
00668800: ldr      pc, [r3, #0xfc]
00668804: mov      r1, r5
00668808: mov      r2, r0
0066880c: mov      r3, #0
00668810: mov      r0, r4
00668814: blx      r8
00668818: b        #0x6681d8
0066881c: mov      r1, sb
00668820: mov      r0, r6
00668824: bl       #0x65b4e4
00668828: subs     r2, r0, #0
0066882c: beq      #0x66928c
00668830: ldr      r1, [pc, #0x640]
00668834: ldr      ip, [r4]
00668838: ldr      r3, [r2]
0066883c: add      r1, pc, r1
00668840: ldr      r8, [ip, #0x68]
00668844: mov      lr, pc
00668848: ldr      pc, [r3, #0xfc]
0066884c: mov      r1, r5
00668850: mov      r2, r0
00668854: mov      r3, #0
00668858: mov      r0, r4
0066885c: blx      r8
00668860: b        #0x6681d8
00668864: mov      r1, sb
00668868: mov      r0, r6
0066886c: bl       #0x65b4e4
00668870: subs     r2, r0, #0
00668874: beq      #0x669270
00668878: ldr      r1, [pc, #0x5fc]
0066887c: ldr      ip, [r4]
00668880: ldr      r3, [r2]
00668884: add      r1, pc, r1
00668888: ldr      r8, [ip, #0x68]
0066888c: mov      lr, pc
00668890: ldr      pc, [r3, #0xfc]
00668894: mov      r1, r5
00668898: mov      r2, r0
0066889c: mov      r3, #0
006688a0: mov      r0, r4
006688a4: blx      r8
006688a8: b        #0x6681d8
006688ac: mov      r1, sb
006688b0: mov      r0, r6
006688b4: bl       #0x65b4e4
006688b8: subs     r2, r0, #0
006688bc: beq      #0x669254
006688c0: ldr      r1, [pc, #0x5b8]
006688c4: ldr      ip, [r4]
006688c8: ldr      r3, [r2]
006688cc: add      r1, pc, r1
006688d0: ldr      r8, [ip, #0x68]
006688d4: mov      lr, pc
006688d8: ldr      pc, [r3, #0xfc]
006688dc: mov      r1, r5
006688e0: mov      r2, r0
006688e4: mov      r3, #0
006688e8: mov      r0, r4
006688ec: blx      r8
006688f0: b        #0x6681d8
006688f4: mov      r1, sb
006688f8: mov      r0, r6
006688fc: bl       #0x65b4e4
00668900: subs     r2, r0, #0
00668904: beq      #0x669238
00668908: ldr      r1, [pc, #0x574]
0066890c: ldr      ip, [r4]
00668910: ldr      r3, [r2]
00668914: add      r1, pc, r1
00668918: ldr      r8, [ip, #0x68]
0066891c: mov      lr, pc
00668920: ldr      pc, [r3, #0xfc]
00668924: mov      r1, r5
00668928: mov      r2, r0
0066892c: mov      r3, #0
00668930: mov      r0, r4
00668934: blx      r8
00668938: b        #0x6681d8
0066893c: mov      r1, sb
00668940: mov      r0, r6
00668944: bl       #0x65b4e4
00668948: subs     r2, r0, #0
0066894c: beq      #0x66921c
00668950: ldr      r1, [pc, #0x530]
00668954: ldr      ip, [r4]
00668958: ldr      r3, [r2]
0066895c: add      r1, pc, r1
00668960: ldr      r8, [ip, #0x68]
00668964: mov      lr, pc
00668968: ldr      pc, [r3, #0xfc]
0066896c: mov      r1, r5
00668970: mov      r2, r0
00668974: mov      r3, #0
00668978: mov      r0, r4
0066897c: blx      r8
00668980: b        #0x6681d8
00668984: mov      r1, sb
00668988: mov      r0, r6
0066898c: bl       #0x65b4e4
00668990: subs     r2, r0, #0
00668994: beq      #0x669200
00668998: ldr      r1, [pc, #0x4ec]
0066899c: ldr      ip, [r4]
006689a0: ldr      r3, [r2]
006689a4: add      r1, pc, r1
006689a8: ldr      r8, [ip, #0x68]
006689ac: mov      lr, pc
006689b0: ldr      pc, [r3, #0xfc]
006689b4: mov      r1, r5
006689b8: mov      r2, r0
006689bc: mov      r3, #0
006689c0: mov      r0, r4
006689c4: blx      r8
006689c8: b        #0x6681d8
006689cc: mov      r1, sb
006689d0: mov      r0, r6
006689d4: bl       #0x65b4e4
006689d8: subs     r2, r0, #0
006689dc: beq      #0x6691e4
006689e0: ldr      r1, [pc, #0x4a8]
006689e4: ldr      ip, [r4]
006689e8: ldr      r3, [r2]
006689ec: add      r1, pc, r1
006689f0: ldr      r8, [ip, #0x68]
006689f4: mov      lr, pc
006689f8: ldr      pc, [r3, #0xfc]
006689fc: mov      r1, r5
00668a00: mov      r2, r0
00668a04: mov      r3, #0
00668a08: mov      r0, r4
00668a0c: blx      r8
00668a10: b        #0x6681d8
00668a14: mov      r1, sb
00668a18: mov      r0, r6
00668a1c: bl       #0x65b4e4
00668a20: subs     r2, r0, #0
00668a24: beq      #0x6691c8
00668a28: ldr      r1, [pc, #0x464]
00668a2c: ldr      ip, [r4]
00668a30: ldr      r3, [r2]
00668a34: add      r1, pc, r1
00668a38: ldr      r8, [ip, #0x68]
00668a3c: mov      lr, pc
00668a40: ldr      pc, [r3, #0xfc]
00668a44: mov      r1, r5
00668a48: mov      r2, r0
00668a4c: mov      r3, #0
00668a50: mov      r0, r4
00668a54: blx      r8
00668a58: b        #0x6681d8
00668a5c: mov      r1, sb
00668a60: mov      r0, r6
00668a64: bl       #0x65b4e4
00668a68: subs     r2, r0, #0
00668a6c: beq      #0x66951c
00668a70: ldrb     r8, [r2, #0x13c]
00668a74: cmp      r8, #0
00668a78: bne      #0x6681d8
00668a7c: ldr      ip, [r4]
00668a80: ldr      r3, [r2]
00668a84: ldr      r1, [sp, #0xc]
00668a88: ldr      sb, [ip, #0x68]
00668a8c: mov      lr, pc
00668a90: ldr      pc, [r3, #0xfc]
00668a94: mov      r1, r5
00668a98: mov      r2, r0
00668a9c: mov      r3, r8
00668aa0: mov      r0, r4
00668aa4: blx      sb
00668aa8: b        #0x6681d8
00668aac: mov      r1, sb
00668ab0: mov      r0, r6
00668ab4: bl       #0x65b4e4
00668ab8: subs     r2, r0, #0
00668abc: beq      #0x669040
00668ac0: ldr      r1, [pc, #0x3d0]
00668ac4: ldr      ip, [r4]
00668ac8: ldr      r3, [r2]
00668acc: add      r1, pc, r1
00668ad0: ldr      r8, [ip, #0x68]
00668ad4: mov      lr, pc
00668ad8: ldr      pc, [r3, #0xfc]
00668adc: mov      r1, r5
00668ae0: mov      r2, r0
00668ae4: mov      r3, #0
00668ae8: mov      r0, r4
00668aec: blx      r8
00668af0: b        #0x6681d8
00668af4: mov      r1, sb
00668af8: mov      r0, r6
00668afc: bl       #0x65b4e4
00668b00: subs     r2, r0, #0
00668b04: beq      #0x6691ac
00668b08: ldr      r1, [pc, #0x38c]
00668b0c: ldr      ip, [r4]
00668b10: ldr      r3, [r2]
00668b14: add      r1, pc, r1
00668b18: ldr      r8, [ip, #0x68]
00668b1c: mov      lr, pc
00668b20: ldr      pc, [r3, #0xfc]
00668b24: mov      r1, r5
00668b28: mov      r2, r0
00668b2c: mov      r3, #0
00668b30: mov      r0, r4
00668b34: blx      r8
00668b38: b        #0x6681d8
00668b3c: mov      r1, sb
00668b40: mov      r0, r6
00668b44: bl       #0x65b4e4
00668b48: subs     r2, r0, #0
00668b4c: beq      #0x669500
00668b50: ldrb     r8, [r2, #0x13d]
00668b54: cmp      r8, #0
00668b58: bne      #0x6681d8
00668b5c: ldr      ip, [r4]
00668b60: ldr      r3, [r2]
00668b64: ldr      r1, [sp, #8]
00668b68: ldr      sb, [ip, #0x68]
00668b6c: mov      lr, pc
00668b70: ldr      pc, [r3, #0xfc]
00668b74: mov      r1, r5
00668b78: mov      r2, r0
00668b7c: mov      r3, r8
00668b80: mov      r0, r4
00668b84: blx      sb
00668b88: b        #0x6681d8
00668b8c: mov      r1, sb
00668b90: mov      r0, r6
00668b94: bl       #0x65b4e4
00668b98: subs     r2, r0, #0
00668b9c: beq      #0x6690b0
00668ba0: ldr      r1, [pc, #0x2f8]
00668ba4: ldr      ip, [r4]
00668ba8: ldr      r3, [r2]
00668bac: add      r1, pc, r1
00668bb0: ldr      r8, [ip, #0x68]
00668bb4: mov      lr, pc
00668bb8: ldr      pc, [r3, #0xfc]
00668bbc: mov      r1, r5
00668bc0: mov      r2, r0
00668bc4: mov      r3, #0
00668bc8: mov      r0, r4
00668bcc: blx      r8
00668bd0: b        #0x6681d8
00668bd4: mov      r1, sb
00668bd8: mov      r0, r6
00668bdc: bl       #0x65b4e4
00668be0: subs     r2, r0, #0
00668be4: beq      #0x6690e8
00668be8: ldr      r1, [pc, #0x2b4]
00668bec: ldr      ip, [r4]
00668bf0: ldr      r3, [r2]
00668bf4: add      r1, pc, r1
00668bf8: ldr      r8, [ip, #0x68]
00668bfc: mov      lr, pc
00668c00: ldr      pc, [r3, #0xfc]
00668c04: mov      r1, r5
00668c08: mov      r2, r0
00668c0c: mov      r3, #0
00668c10: mov      r0, r4
00668c14: blx      r8
00668c18: b        #0x6681d8
00668c1c: mov      r1, sb
00668c20: mov      r0, r6
00668c24: bl       #0x65b4e4
00668c28: subs     r2, r0, #0
00668c2c: beq      #0x6690cc
00668c30: ldr      r1, [pc, #0x270]
00668c34: ldr      ip, [r4]
00668c38: ldr      r3, [r2]
00668c3c: add      r1, pc, r1
00668c40: ldr      r8, [ip, #0x68]
00668c44: mov      lr, pc
00668c48: ldr      pc, [r3, #0xfc]
00668c4c: mov      r1, r5
00668c50: mov      r2, r0
00668c54: mov      r3, #0
00668c58: mov      r0, r4
00668c5c: blx      r8
00668c60: b        #0x6681d8
00668c64: mov      r1, sb
00668c68: mov      r0, r6
00668c6c: bl       #0x65b4e4
00668c70: subs     r2, r0, #0
00668c74: beq      #0x669158
00668c78: ldr      r1, [pc, #0x22c]
00668c7c: ldr      ip, [r4]
00668c80: ldr      r3, [r2]
00668c84: add      r1, pc, r1
00668c88: ldr      r8, [ip, #0x68]
00668c8c: mov      lr, pc
00668c90: ldr      pc, [r3, #0xfc]
00668c94: mov      r1, r5
00668c98: mov      r2, r0
00668c9c: mov      r3, #0
00668ca0: mov      r0, r4
00668ca4: blx      r8
00668ca8: b        #0x6681d8
00668cac: mov      r1, sb
00668cb0: mov      r0, r6
00668cb4: bl       #0x65b4e4
00668cb8: subs     r2, r0, #0
00668cbc: beq      #0x66913c
00668cc0: ldr      r1, [pc, #0x1e8]
00668cc4: ldr      ip, [r4]
00668cc8: ldr      r3, [r2]
00668ccc: add      r1, pc, r1
00668cd0: ldr      r8, [ip, #0x68]
00668cd4: mov      lr, pc
00668cd8: ldr      pc, [r3, #0xfc]
00668cdc: mov      r1, r5
00668ce0: mov      r2, r0
00668ce4: mov      r3, #0
00668ce8: mov      r0, r4
00668cec: blx      r8
00668cf0: b        #0x6681d8
00668cf4: mov      r1, sb
00668cf8: mov      r0, r6
00668cfc: bl       #0x65b4e4
00668d00: subs     r2, r0, #0
00668d04: beq      #0x669120
00668d08: ldr      r1, [pc, #0x1a4]
00668d0c: ldr      ip, [r4]
00668d10: ldr      r3, [r2]
00668d14: add      r1, pc, r1
00668d18: ldr      r8, [ip, #0x68]
00668d1c: mov      lr, pc
00668d20: ldr      pc, [r3, #0xfc]
00668d24: mov      r1, r5
00668d28: mov      r2, r0
00668d2c: mov      r3, #0
00668d30: mov      r0, r4
00668d34: blx      r8
00668d38: b        #0x6681d8
00668d3c: mov      r1, sb
00668d40: mov      r0, r6
00668d44: bl       #0x65b4e4
00668d48: subs     r2, r0, #0
00668d4c: beq      #0x669104
00668d50: ldr      r1, [pc, #0x160]
00668d54: ldr      ip, [r4]
00668d58: ldr      r3, [r2]
00668d5c: add      r1, pc, r1
00668d60: ldr      r8, [ip, #0x68]
00668d64: mov      lr, pc
00668d68: ldr      pc, [r3, #0xfc]
00668d6c: mov      r1, r5
00668d70: mov      r2, r0
00668d74: mov      r3, #0
00668d78: mov      r0, r4
00668d7c: blx      r8
00668d80: b        #0x6681d8
00668d84: mov      r1, sb
00668d88: mov      r0, r6
00668d8c: bl       #0x65b4e4
00668d90: subs     r2, r0, #0
00668d94: beq      #0x669190
00668d98: ldr      r1, [pc, #0x11c]
00668d9c: ldr      ip, [r4]
00668da0: ldr      r3, [r2]
00668da4: add      r1, pc, r1
00668da8: ldr      r8, [ip, #0x68]
00668dac: mov      lr, pc
00668db0: ldr      pc, [r3, #0xfc]
00668db4: mov      r1, r5
00668db8: mov      r2, r0
00668dbc: mov      r3, #0
00668dc0: mov      r0, r4
00668dc4: blx      r8
00668dc8: b        #0x6681d8
00668dcc: mov      r1, sb
00668dd0: mov      r0, r6
00668dd4: bl       #0x65b4e4
00668dd8: subs     r2, r0, #0
00668ddc: beq      #0x669174
00668de0: ldr      r1, [pc, #0xd8]
00668de4: ldr      ip, [r4]
00668de8: ldr      r3, [r2]
00668dec: add      r1, pc, r1
00668df0: ldr      r8, [ip, #0x68]
00668df4: mov      lr, pc
00668df8: ldr      pc, [r3, #0xfc]
00668dfc: mov      r1, r5
00668e00: mov      r2, r0
00668e04: mov      r3, #0
00668e08: mov      r0, r4
00668e0c: blx      r8
00668e10: b        #0x6681d8
00668e14: eorseq   ip, r2, r0, ror #22
00668e18: eoreq    sp, r7, r8, ror r2
00668e1c: mlaeq    r7, r8, r2, ip
00668e20: eoreq    sp, r7, r4, asr #4
00668e24: eoreq    sp, r7, r8, lsl r2
00668e28: eoreq    sp, r7, r4, ror #3
00668e2c: andeq    r0, r0, r0, lsl #20
00668e30: andeq    r1, r0, ip, asr r2
00668e34: eoreq    sp, r7, r4, asr #32
00668e38: eoreq    sp, r7, ip, lsr r0
00668e3c: eoreq    sp, r7, r4
00668e40: eoreq    r0, r7, r4, lsr #31
00668e44: eoreq    ip, r7, ip, lsl lr
00668e48: eoreq    ip, r7, ip, asr #26
00668e4c: eoreq    ip, r7, r4, lsl sp
00668e50: eoreq    ip, r7, r4, asr sp
00668e54: eoreq    ip, r7, ip, lsl sp
00668e58: eoreq    ip, r7, r4, ror #25
00668e5c: eoreq    ip, r7, ip, lsr #25
00668e60: eoreq    ip, r7, ip, ror ip
00668e64: eoreq    ip, r7, r4, asr #24
00668e68: eoreq    ip, r7, ip, lsr #26

# _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEE19applyBlendedValueExEPvPfiS8_PNS1_15CApplicatorInfoE
00620968: push     {r4, r5, lr}
0062096c: sub      sp, sp, #0x14
00620970: mov      ip, #0
00620974: mov      r4, r3
00620978: mov      lr, #0x3f800000
0062097c: mov      r3, sp
00620980: str      ip, [sp, #8]
00620984: str      lr, [sp, #0xc]
00620988: str      ip, [sp]
0062098c: str      ip, [sp, #4]
00620990: bl       #0x6130d4
00620994: mov      r0, r4
00620998: mov      r1, sp
0062099c: ldr      r3, [r4]
006209a0: mov      r5, sp
006209a4: mov      lr, pc
006209a8: ldr      pc, [r3, #0x9c]
006209ac: add      sp, sp, #0x14
006209b0: pop      {r4, r5, pc}

# _ZN6glitch7collada14CEventsManager8onUpdateEiiii
0060ebe0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0060ebe4: cmp      r1, r2
0060ebe8: mov      r6, r1
0060ebec: mov      r5, r2
0060ebf0: mov      sl, r3
0060ebf4: mov      r4, r0
0060ebf8: ldr      sb, [sp, #0x20]
0060ebfc: beq      #0x60ec98
0060ec00: ldr      r3, [r0, #8]
0060ec04: cmp      r3, #0
0060ec08: beq      #0x60ec98
0060ec0c: sub      r1, r1, #1
0060ec10: bl       #0x60e02c
0060ec14: mov      r1, r5
0060ec18: add      r7, r0, #1
0060ec1c: mov      r0, r4
0060ec20: bl       #0x60e02c
0060ec24: ldr      r3, [r4, #0x10]
0060ec28: mov      r8, r0
0060ec2c: cmp      r3, r7
0060ec30: ldr      r3, [r4, #4]
0060ec34: addeq    r7, r7, #1
0060ec38: cmp      r6, r5
0060ec3c: add      r3, r3, #1
0060ec40: str      r3, [r4, #4]
0060ec44: ble      #0x60ec9c
0060ec48: mov      r1, sb
0060ec4c: mov      r0, r4
0060ec50: bl       #0x60e02c
0060ec54: rsb      r3, sl, sb
0060ec58: mov      r2, r0
0060ec5c: add      r3, r3, r5
0060ec60: mov      r1, r7
0060ec64: mov      r0, r4
0060ec68: bl       #0x60ebb4
0060ec6c: sub      r1, sl, #1
0060ec70: mov      r0, r4
0060ec74: bl       #0x60e02c
0060ec78: mov      r3, r5
0060ec7c: add      r1, r0, #1
0060ec80: mov      r2, r8
0060ec84: mov      r0, r4
0060ec88: bl       #0x60ebb4
0060ec8c: mov      r0, r4
0060ec90: bl       #0x31d584
0060ec94: str      r8, [r4, #0x10]
0060ec98: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0060ec9c: mov      r1, r7
0060eca0: mov      r3, r5
0060eca4: mov      r0, r4
0060eca8: mov      r2, r8
0060ecac: bl       #0x60ebb4
0060ecb0: b        #0x60ec8c

# _ZN6glitch7collada15animation_track8CBlenderINS_4core10quaternionELi1ES4_E15getAddedValueExEPvPfiS6_
00613378: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061337c: mov      ip, #0x3f800000
00613380: sub      sp, sp, #0x9c
00613384: mov      r8, #0
00613388: subs     fp, r2, #0
0061338c: mov      sl, r1
00613390: str      r3, [sp, #0x30]
00613394: str      r8, [sp, #0x88]
00613398: str      r8, [sp, #0x8c]
0061339c: str      r8, [sp, #0x90]
006133a0: str      ip, [sp, #0x94]
006133a4: str      r8, [sp, #0x78]
006133a8: str      r8, [sp, #0x7c]
006133ac: str      r8, [sp, #0x80]
006133b0: str      ip, [sp, #0x84]
006133b4: ble      #0x613560
006133b8: mov      r6, #0
006133bc: mov      r4, r0
006133c0: add      r1, sp, #4
006133c4: add      r0, sp, #0x48
006133c8: add      r2, sp, #0x88
006133cc: add      r3, sp, #0x38
006133d0: add      ip, sp, #0x78
006133d4: add      lr, sp, #0x68
006133d8: mov      r7, r6
006133dc: str      r0, [sp, #0x2c]
006133e0: str      r1, [sp, #0x1c]
006133e4: add      sb, sp, #0x58
006133e8: str      r2, [sp, #0x20]
006133ec: str      r3, [sp, #0x34]
006133f0: str      ip, [sp, #0x24]
006133f4: str      lr, [sp, #0x28]
006133f8: b        #0x61347c
006133fc: ldr      ip, [sp, #0x1c]
00613400: mov      r0, #0x3f800000
00613404: str      r8, [sp, #0x58]
00613408: str      r8, [sp, #0x5c]
0061340c: str      r8, [sp, #0x60]
00613410: str      r0, [sp, #0x64]
00613414: ldm      r4, {r0, r1, r2, r3}
00613418: stm      ip, {r0, r1, r2, r3}
0061341c: ldr      lr, [sp, #0x20]
00613420: ldr      ip, [sp, #0x94]
00613424: mov      r0, sb
00613428: ldm      lr, {r1, r2, r3}
0061342c: str      ip, [sp]
00613430: str      r5, [sp, #0x14]
00613434: bl       #0x612d00
00613438: ldr      r0, [sp, #0x28]
0061343c: ldr      r1, [sp, #0x24]
00613440: mov      r2, sb
00613444: bl       #0x60dd34
00613448: ldr      r3, [sp, #0x68]
0061344c: str      r3, [sp, #0x78]
00613450: ldr      r3, [sp, #0x6c]
00613454: str      r3, [sp, #0x7c]
00613458: ldr      r3, [sp, #0x70]
0061345c: str      r3, [sp, #0x80]
00613460: ldr      r3, [sp, #0x74]
00613464: str      r3, [sp, #0x84]
00613468: add      r7, r7, #1
0061346c: cmp      r7, fp
00613470: add      r6, r6, #4
00613474: add      r4, r4, #0x10
00613478: beq      #0x61355c
0061347c: ldr      r5, [sl, r6]
00613480: mov      r1, #0
00613484: mov      r0, r5
00613488: bl       #0x30e2f8
0061348c: cmp      r0, #0
00613490: mov      r1, #0
00613494: mov      r0, r5
00613498: bne      #0x6133fc
0061349c: bl       #0x30e70c
006134a0: cmp      r0, #0
006134a4: beq      #0x613468
006134a8: ldr      r2, [r4, #4]
006134ac: ldr      r1, [r4, #8]
006134b0: ldr      r3, [r4]
006134b4: ldr      r0, [r4, #0xc]
006134b8: ldr      lr, [sp, #0x1c]
006134bc: add      r3, r3, #0x80000000
006134c0: add      r2, r2, #0x80000000
006134c4: add      r1, r1, #0x80000000
006134c8: ldr      ip, [sl, r6]
006134cc: str      r0, [sp, #0x64]
006134d0: str      r1, [sp, #0x60]
006134d4: str      r2, [sp, #0x5c]
006134d8: str      r3, [sp, #0x58]
006134dc: ldm      sb, {r0, r1, r2, r3}
006134e0: stm      lr, {r0, r1, r2, r3}
006134e4: ldr      r0, [sp, #0x20]
006134e8: add      ip, ip, #0x80000000
006134ec: str      r8, [sp, #0x48]
006134f0: ldm      r0, {r1, r2, r3}
006134f4: str      ip, [sp, #0x14]
006134f8: ldr      ip, [sp, #0x94]
006134fc: ldr      r0, [sp, #0x2c]
00613500: str      r8, [sp, #0x4c]
00613504: str      ip, [sp]
00613508: mov      ip, #0x3f800000
0061350c: str      ip, [sp, #0x54]
00613510: str      r8, [sp, #0x50]
00613514: bl       #0x612d00
00613518: ldr      r0, [sp, #0x34]
0061351c: ldr      r1, [sp, #0x24]
00613520: ldr      r2, [sp, #0x2c]
00613524: bl       #0x60dd34
00613528: ldr      r3, [sp, #0x38]
0061352c: add      r7, r7, #1
00613530: cmp      r7, fp
00613534: str      r3, [sp, #0x78]
00613538: ldr      r3, [sp, #0x3c]
0061353c: add      r6, r6, #4
00613540: add      r4, r4, #0x10
00613544: str      r3, [sp, #0x7c]
00613548: ldr      r3, [sp, #0x40]
0061354c: str      r3, [sp, #0x80]
00613550: ldr      r3, [sp, #0x44]
00613554: str      r3, [sp, #0x84]
00613558: bne      #0x61347c
0061355c: ldr      r8, [sp, #0x78]
00613560: ldr      r1, [sp, #0x7c]
00613564: ldr      r3, [sp, #0x80]
00613568: ldr      r2, [sp, #0x84]
0061356c: ldr      r0, [sp, #0x30]
00613570: str      r8, [r0]
00613574: str      r1, [r0, #4]
00613578: str      r2, [r0, #0xc]
0061357c: str      r3, [r0, #8]
00613580: add      sp, sp, #0x9c
00613584: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN14AnimApplicator13CheckCallbackEPN6glitch5scene19ITimelineControllerE
0036440c: push     {r4, lr}
00364410: ldrb     r3, [r0, #0x30]
00364414: mov      r4, r0
00364418: cmp      r3, #0
0036441c: beq      #0x364440
00364420: ldr      r3, [r0, #0x34]
00364424: cmp      r3, #0
00364428: beq      #0x364440
0036442c: mov      r0, r1
00364430: ldr      r1, [r4, #0x38]
00364434: blx      r3
00364438: mov      r3, #0
0036443c: strb     r3, [r4, #0x30]
00364440: pop      {r4, pc}

# _ZN6glitch4core10quaternion5slerpES1_S1_f
00612d00: sub      sp, sp, #0x10
00612d04: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00612d08: sub      sp, sp, #0x1c
00612d0c: add      ip, sp, #0x44
00612d10: stm      ip, {r1, r2, r3}
00612d14: ldr      fp, [sp, #0x54]
00612d18: ldr      sb, [sp, #0x44]
00612d1c: ldr      r3, [sp, #0x58]
00612d20: mov      r1, fp
00612d24: mov      r4, r0
00612d28: mov      r0, sb
00612d2c: str      r3, [sp, #0xc]
00612d30: bl       #0x30ed6c
00612d34: ldr      sl, [sp, #0x48]
00612d38: mov      r5, r0
00612d3c: ldr      r1, [sp, #0xc]
00612d40: mov      r0, sl
00612d44: bl       #0x30ed6c
00612d48: ldr      r3, [sp, #0x5c]
00612d4c: mov      r1, r0
00612d50: mov      r0, r5
00612d54: str      r3, [sp, #8]
00612d58: bl       #0x30eba4
00612d5c: ldr      r8, [sp, #0x4c]
00612d60: mov      r5, r0
00612d64: ldr      r1, [sp, #8]
00612d68: mov      r0, r8
00612d6c: bl       #0x30ed6c
00612d70: ldr      r3, [sp, #0x60]
00612d74: mov      r1, r0
00612d78: mov      r0, r5
00612d7c: str      r3, [sp, #4]
00612d80: bl       #0x30eba4
00612d84: ldr      r7, [sp, #0x50]
00612d88: mov      r5, r0
00612d8c: ldr      r1, [sp, #4]
00612d90: mov      r0, r7
00612d94: bl       #0x30ed6c
00612d98: mov      r1, r0
00612d9c: mov      r0, r5
00612da0: bl       #0x30eba4
00612da4: mov      r1, #0
00612da8: mov      r6, r0
00612dac: bl       #0x30e70c
00612db0: cmp      r0, #0
00612db4: addne    r6, r6, #0x80000000
00612db8: mov      r1, #0x3f800000
00612dbc: mov      r0, r6
00612dc0: addne    sb, sb, #0x80000000
00612dc4: addne    sl, sl, #0x80000000
00612dc8: addne    r8, r8, #0x80000000
00612dcc: addne    r7, r7, #0x80000000
00612dd0: bl       #0x30eba4
00612dd4: movw     r1, #0xcccd
00612dd8: movt     r1, #0x3d4c
00612ddc: bl       #0x30e2f8
00612de0: cmp      r0, #0
00612de4: ldr      r5, [sp, #0x64]
00612de8: beq      #0x612f10
00612dec: mov      r1, r6
00612df0: mov      r0, #0x3f800000
00612df4: bl       #0x30e3ac
00612df8: movw     r1, #0xcccd
00612dfc: movt     r1, #0x3d4c
00612e00: bl       #0x30e4b4
00612e04: cmp      r0, #0
00612e08: beq      #0x61300c
00612e0c: mov      r0, r6
00612e10: bl       #0x30e3dc
00612e14: str      r0, [sp, #0x10]
00612e18: bl       #0x30eb08
00612e1c: mov      r1, r0
00612e20: mov      r0, #0x3f800000
00612e24: bl       #0x30ec94
00612e28: mov      r1, r5
00612e2c: str      r0, [sp, #0x14]
00612e30: mov      r0, #0x3f800000
00612e34: bl       #0x30e3ac
00612e38: mov      r1, r0
00612e3c: ldr      r0, [sp, #0x10]
00612e40: bl       #0x30ed6c
00612e44: bl       #0x30eb08
00612e48: ldr      r1, [sp, #0x14]
00612e4c: bl       #0x30ed6c
00612e50: mov      r1, r5
00612e54: mov      r6, r0
00612e58: ldr      r0, [sp, #0x10]
00612e5c: bl       #0x30ed6c
00612e60: bl       #0x30eb08
00612e64: ldr      r1, [sp, #0x14]
00612e68: bl       #0x30ed6c
00612e6c: mov      r1, sb
00612e70: mov      r5, r0
00612e74: mov      r0, r6
00612e78: bl       #0x30ed6c
00612e7c: mov      r1, fp
00612e80: mov      sb, r0
00612e84: mov      r0, r5
00612e88: bl       #0x30ed6c
00612e8c: mov      r1, r0
00612e90: mov      r0, sb
00612e94: bl       #0x30eba4
00612e98: mov      r1, sl
00612e9c: str      r0, [r4]
00612ea0: mov      r0, r6
00612ea4: bl       #0x30ed6c
00612ea8: ldr      r1, [sp, #0xc]
00612eac: mov      sl, r0
00612eb0: mov      r0, r5
00612eb4: bl       #0x30ed6c
00612eb8: mov      r1, r0
00612ebc: mov      r0, sl
00612ec0: bl       #0x30eba4
00612ec4: mov      r1, r8
00612ec8: str      r0, [r4, #4]
00612ecc: mov      r0, r6
00612ed0: bl       #0x30ed6c
00612ed4: ldr      r1, [sp, #8]
00612ed8: mov      r8, r0
00612edc: mov      r0, r5
00612ee0: bl       #0x30ed6c
00612ee4: mov      r1, r0
00612ee8: mov      r0, r8
00612eec: bl       #0x30eba4
00612ef0: mov      r1, r7
00612ef4: str      r0, [r4, #8]
00612ef8: mov      r0, r6
00612efc: bl       #0x30ed6c
00612f00: ldr      r1, [sp, #4]
00612f04: mov      r6, r0
00612f08: mov      r0, r5
00612f0c: b        #0x612fe4
00612f10: mov      r1, r5
00612f14: mov      r0, #0x3f000000
00612f18: bl       #0x30e3ac
00612f1c: movw     r1, #0xfdb
00612f20: movt     r1, #0x4049
00612f24: bl       #0x30ed6c
00612f28: bl       #0x30eb08
00612f2c: movw     r1, #0xfdb
00612f30: mov      r6, r0
00612f34: movt     r1, #0x4049
00612f38: mov      r0, r5
00612f3c: bl       #0x30ed6c
00612f40: bl       #0x30eb08
00612f44: mov      r1, sb
00612f48: mov      r5, r0
00612f4c: mov      r0, r6
00612f50: bl       #0x30ed6c
00612f54: mov      r1, r5
00612f58: mov      fp, r0
00612f5c: add      r0, sl, #0x80000000
00612f60: bl       #0x30ed6c
00612f64: mov      r1, r0
00612f68: mov      r0, fp
00612f6c: bl       #0x30eba4
00612f70: mov      r1, sl
00612f74: str      r0, [r4]
00612f78: mov      r0, r6
00612f7c: bl       #0x30ed6c
00612f80: mov      r1, sb
00612f84: mov      sl, r0
00612f88: mov      r0, r5
00612f8c: bl       #0x30ed6c
00612f90: mov      r1, r0
00612f94: mov      r0, sl
00612f98: bl       #0x30eba4
00612f9c: mov      r1, r8
00612fa0: str      r0, [r4, #4]
00612fa4: mov      r0, r6
00612fa8: bl       #0x30ed6c
00612fac: mov      r1, r5
00612fb0: mov      sl, r0
00612fb4: add      r0, r7, #0x80000000
00612fb8: bl       #0x30ed6c
00612fbc: mov      r1, r0
00612fc0: mov      r0, sl
00612fc4: bl       #0x30eba4
00612fc8: mov      r1, r7
00612fcc: str      r0, [r4, #8]
00612fd0: mov      r0, r6
00612fd4: bl       #0x30ed6c
00612fd8: mov      r1, r8
00612fdc: mov      r6, r0
00612fe0: mov      r0, r5
00612fe4: bl       #0x30ed6c
00612fe8: mov      r1, r0
00612fec: mov      r0, r6
00612ff0: bl       #0x30eba4
00612ff4: str      r0, [r4, #0xc]
00612ff8: mov      r0, r4
00612ffc: add      sp, sp, #0x1c
00613000: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00613004: add      sp, sp, #0x10
00613008: bx       lr
0061300c: mov      r1, r5
00613010: mov      r0, #0x3f800000
00613014: bl       #0x30e3ac
00613018: mov      r1, sb
0061301c: mov      r6, r0
00613020: bl       #0x30ed6c
00613024: mov      r1, fp
00613028: mov      sb, r0
0061302c: mov      r0, r5
00613030: bl       #0x30ed6c
00613034: mov      r1, r0
00613038: mov      r0, sb
0061303c: bl       #0x30eba4
00613040: mov      r1, sl
00613044: str      r0, [r4]
00613048: mov      r0, r6
0061304c: bl       #0x30ed6c
00613050: ldr      r1, [sp, #0xc]
00613054: mov      sl, r0
00613058: mov      r0, r5
0061305c: bl       #0x30ed6c
00613060: mov      r1, r0
00613064: mov      r0, sl
00613068: bl       #0x30eba4
0061306c: mov      r1, r8
00613070: str      r0, [r4, #4]
00613074: mov      r0, r6
00613078: bl       #0x30ed6c
0061307c: ldr      r1, [sp, #8]
00613080: mov      r8, r0
00613084: mov      r0, r5
00613088: bl       #0x30ed6c
0061308c: mov      r1, r0
00613090: mov      r0, r8
00613094: bl       #0x30eba4
00613098: mov      r1, r7
0061309c: str      r0, [r4, #8]
006130a0: mov      r0, r6
006130a4: bl       #0x30ed6c
006130a8: ldr      r1, [sp, #4]
006130ac: mov      r6, r0
006130b0: mov      r0, r5
006130b4: bl       #0x30ed6c
006130b8: mov      r1, r0
006130bc: mov      r0, r6
006130c0: bl       #0x30eba4
006130c4: str      r0, [r4, #0xc]
006130c8: mov      r0, r4
006130cc: bl       #0x35c8f0
006130d0: b        #0x612ff8

# _ZN6glitch7collada13CAnimationSet12addAnimationEPKNS0_10SAnimationE
006601fc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00660200: ldr      r3, [r0, #0xc]
00660204: ldr      fp, [r0, #0x10]
00660208: ldr      r2, [pc, #0x28c]
0066020c: sub      sp, sp, #0x14
00660210: rsb      fp, r3, fp
00660214: str      r1, [sp, #0xc]
00660218: asrs     fp, fp, #2
0066021c: add      r2, pc, r2
00660220: mov      r5, r0
00660224: ldr      r4, [r1, #0x10]
00660228: beq      #0x660330
0066022c: ldr      r1, [pc, #0x26c]
00660230: mov      r6, #0
00660234: mov      sl, #0xc
00660238: ldr      sb, [r2, r1]
0066023c: ldr      r2, [pc, #0x260]
00660240: mov      r8, #1
00660244: lsl      r7, r6, #2
00660248: add      r2, pc, r2
0066024c: str      r2, [sp, #8]
00660250: ldr      r1, [r3, r6, lsl #2]
00660254: ldr      r3, [r4, #8]
00660258: ldr      r2, [sb]
0066025c: ldr      r1, [r1, #8]
00660260: cmp      r3, #0x5b
00660264: mla      r2, sl, r1, r2
00660268: bhi      #0x660300
0066026c: lsr      r1, r3, #5
00660270: ldr      r2, [r2, r1, lsl #2]
00660274: and      r3, r3, #0x1f
00660278: ands     r2, r2, r8, lsl r3
0066027c: beq      #0x6602d0
00660280: ldr      r3, [r5, #0xc]
00660284: ldr      r1, [r4, #4]
00660288: ldr      r7, [r3, r7]
0066028c: ldr      r0, [r7, #4]
00660290: bl       #0x30e31c
00660294: cmp      r0, #0
00660298: bne      #0x6602d0
0066029c: ldr      r3, [r4, #8]
006602a0: cmp      r3, #0xe
006602a4: beq      #0x66031c
006602a8: cmp      r3, #0x56
006602ac: beq      #0x6602bc
006602b0: mov      r0, r6
006602b4: add      sp, sp, #0x14
006602b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006602bc: ldr      r0, [r7, #0xc]
006602c0: ldr      r1, [r4, #0xc]
006602c4: bl       #0x30e31c
006602c8: cmp      r0, #0
006602cc: beq      #0x6602b0
006602d0: add      r6, r6, #1
006602d4: cmp      r6, fp
006602d8: beq      #0x660330
006602dc: ldr      r3, [r5, #0xc]
006602e0: ldr      r2, [sb]
006602e4: lsl      r7, r6, #2
006602e8: ldr      r1, [r3, r6, lsl #2]
006602ec: ldr      r3, [r4, #8]
006602f0: ldr      r1, [r1, #8]
006602f4: cmp      r3, #0x5b
006602f8: mla      r2, sl, r1, r2
006602fc: bls      #0x66026c
00660300: ldr      r0, [sp, #8]
00660304: str      r2, [sp, #4]
00660308: str      r3, [sp]
0066030c: bl       #0x708eb0
00660310: ldr      r3, [sp]
00660314: ldr      r2, [sp, #4]
00660318: b        #0x66026c
0066031c: ldrb     r2, [r7, #0xc]
00660320: ldrb     r3, [r4, #0xc]
00660324: cmp      r2, r3
00660328: bne      #0x6602d0
0066032c: b        #0x6602b0
00660330: ldr      r0, [sp, #0xc]
00660334: bl       #0x611ae0
00660338: subs     r6, r0, #0
0066033c: mvneq    r0, #0
00660340: beq      #0x6602b4
00660344: ldr      sl, [r5, #0x10]
00660348: ldr      r3, [r5, #0x14]
0066034c: cmp      sl, r3
00660350: beq      #0x66039c
00660354: str      r4, [sl]
00660358: ldr      r3, [r5, #0x10]
0066035c: add      r3, r3, #4
00660360: str      r3, [r5, #0x10]
00660364: ldr      r8, [r5, #0x1c]
00660368: ldr      r3, [r5, #0x20]
0066036c: cmp      r8, r3
00660370: beq      #0x66041c
00660374: str      r6, [r8]
00660378: ldr      r3, [r5, #0x1c]
0066037c: add      r3, r3, #4
00660380: str      r3, [r5, #0x1c]
00660384: ldr      r3, [r5, #0xc]
00660388: ldr      r0, [r5, #0x10]
0066038c: rsb      r0, r3, r0
00660390: asr      r0, r0, #2
00660394: sub      r0, r0, #1
00660398: b        #0x6602b4
0066039c: ldr      r3, [r5, #0xc]
006603a0: rsb      r3, r3, sl
006603a4: asr      r3, r3, #2
006603a8: cmp      r3, #1
006603ac: addhs    r8, r3, r3
006603b0: addlo    r8, r3, #1
006603b4: cmn      r8, #0xc0000001
006603b8: bhi      #0x660414
006603bc: cmp      r3, r8
006603c0: bhi      #0x660414
006603c4: lsl      r8, r8, #2
006603c8: mov      r1, #0
006603cc: mov      r0, r8
006603d0: bl       #0x310568
006603d4: ldr      r1, [r5, #0xc]
006603d8: mov      r7, r0
006603dc: subs     sl, sl, r1
006603e0: moveq    sl, r0
006603e4: beq      #0x6603f4
006603e8: mov      r2, sl
006603ec: bl       #0x30df38
006603f0: add      sl, r0, sl
006603f4: str      r4, [sl], #4
006603f8: ldr      r0, [r5, #0xc]
006603fc: add      r8, r7, r8
00660400: bl       #0x310450
00660404: str      sl, [r5, #0x10]
00660408: str      r8, [r5, #0x14]
0066040c: str      r7, [r5, #0xc]
00660410: b        #0x660364
00660414: mvn      r8, #0xc0000000
00660418: b        #0x6603c4
0066041c: ldr      r3, [r5, #0x18]
00660420: rsb      r3, r3, r8
00660424: asr      r3, r3, #2
00660428: cmp      r3, #1
0066042c: addhs    r7, r3, r3
00660430: addlo    r7, r3, #1
00660434: cmn      r7, #0xc0000001
00660438: bhi      #0x660494
0066043c: cmp      r3, r7
00660440: bhi      #0x660494
00660444: lsl      r7, r7, #2
00660448: mov      r1, #0
0066044c: mov      r0, r7
00660450: bl       #0x310568
00660454: ldr      r1, [r5, #0x18]
00660458: mov      r4, r0
0066045c: subs     r8, r8, r1
00660460: moveq    r8, r0
00660464: beq      #0x660474
00660468: mov      r2, r8
0066046c: bl       #0x30df38
00660470: add      r8, r0, r8
00660474: str      r6, [r8], #4
00660478: ldr      r0, [r5, #0x18]
0066047c: add      r7, r4, r7
00660480: bl       #0x310450
00660484: str      r8, [r5, #0x1c]
00660488: str      r7, [r5, #0x20]
0066048c: str      r4, [r5, #0x18]
00660490: b        #0x660384
00660494: mvn      r7, #0xc0000000
00660498: b        #0x660444
0066049c: eorseq   r4, r3, r4, ror r8
006604a0: andeq    r4, r0, ip, asr #10
006604a4: eoreq    r1, r6, r0, lsl #21

# _ZN11AnimatorSet17getAnimationValueEiiPv
00367390: ldr      ip, [r0, #0x98]
00367394: cmp      ip, #0
00367398: ldrne    ip, [ip, #0x20]
0036739c: str      ip, [r0, #0x50]
003673a0: b        #0x65f7b4

# _ZN6glitch7collada21CSceneNodeAnimatorSet22computeAnimationValuesEj
0065f5dc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0065f5e0: ldr      r3, [r0, #0x24]
0065f5e4: sub      sp, sp, #0x4c
0065f5e8: mov      r5, r0
0065f5ec: ldr      r3, [r3, #0x3c]
0065f5f0: mov      r4, r1
0065f5f4: cmp      r3, #0
0065f5f8: bne      #0x65f608
0065f5fc: ldr      r3, [r0, #0x18]
0065f600: cmp      r3, #0
0065f604: beq      #0x65f798
0065f608: mov      r0, r5
0065f60c: mov      r1, r4
0065f610: bl       #0x667c48
0065f614: ldr      r3, [r5]
0065f618: mov      r0, r5
0065f61c: mov      lr, pc
0065f620: ldr      pc, [r3, #0x44]
0065f624: cmp      r0, #0
0065f628: beq      #0x65f7a0
0065f62c: ldr      r0, [r0, #4]
0065f630: str      r0, [sp, #0x14]
0065f634: ldr      r3, [r5, #0xc]
0065f638: ldr      r1, [r5, #0x50]
0065f63c: ldr      r0, [r5, #0x24]
0065f640: subs     r3, r3, #1
0065f644: movne    r3, #1
0065f648: str      r3, [sp, #0x1c]
0065f64c: bl       #0x65f0b4
0065f650: ldr      r3, [r0]
0065f654: ldr      r1, [sp, #0x14]
0065f658: mov      r0, r5
0065f65c: ldr      r3, [r3, #0x24]
0065f660: ldr      r3, [r3, #0x20]
0065f664: ldr      r3, [r3, #0x14]
0065f668: subs     r3, r3, #0
0065f66c: movne    r3, #1
0065f670: str      r3, [sp, #0x10]
0065f674: bl       #0x65f364
0065f678: ldr      r2, [sp, #0x10]
0065f67c: str      r0, [sp, #0x18]
0065f680: ldr      r3, [r5, #0x24]
0065f684: strb     r2, [sp, #0x39]
0065f688: ldr      r6, [r3, #0x3c]
0065f68c: cmp      r6, #0
0065f690: beq      #0x65f798
0065f694: add      r3, sp, #0x2c
0065f698: add      ip, sp, #0x3c
0065f69c: mov      r4, #0
0065f6a0: str      r3, [sp, #0x20]
0065f6a4: str      ip, [sp, #0x24]
0065f6a8: mov      sb, r6
0065f6ac: b        #0x65f6bc
0065f6b0: add      r4, r4, #1
0065f6b4: cmp      r4, sb
0065f6b8: beq      #0x65f798
0065f6bc: mov      r1, r4
0065f6c0: ldr      r3, [r5]
0065f6c4: mov      r0, r5
0065f6c8: mov      lr, pc
0065f6cc: ldr      pc, [r3, #0x80]
0065f6d0: cmp      r0, #0
0065f6d4: beq      #0x65f6b0
0065f6d8: ldr      r3, [r5, #0x28]
0065f6dc: lsl      fp, r4, #2
0065f6e0: ldr      r6, [r3, r4, lsl #2]
0065f6e4: cmp      r6, #0
0065f6e8: beq      #0x65f6b0
0065f6ec: ldr      r7, [r5, #0x4c]
0065f6f0: ldr      r3, [r5, #0x24]
0065f6f4: mov      r2, #0xc
0065f6f8: add      r7, r4, r7
0065f6fc: mul      r7, r2, r7
0065f700: ldr      r8, [r3, #0x30]
0065f704: add      sl, r8, r7
0065f708: ldr      r1, [sl, #4]
0065f70c: cmp      r1, #0
0065f710: beq      #0x65f740
0065f714: ldr      r3, [r3, #0x18]
0065f718: ldr      r3, [r3, fp]
0065f71c: mov      r0, r3
0065f720: ldr      r3, [r3]
0065f724: str      r1, [sp, #0xc]
0065f728: mov      lr, pc
0065f72c: ldr      pc, [r3, #8]
0065f730: ldr      r1, [sp, #0xc]
0065f734: mov      r2, r0
0065f738: mov      r0, r6
0065f73c: bl       #0x30e868
0065f740: ldr      r3, [r8, r7]
0065f744: cmp      r3, #2
0065f748: bne      #0x65f6b0
0065f74c: ldr      r3, [sl, #8]
0065f750: ldr      r2, [sp, #0x18]
0065f754: ldr      ip, [sp, #0x10]
0065f758: str      r3, [sp, #0x3c]
0065f75c: ldr      r3, [sp, #0x20]
0065f760: str      r2, [sp, #0x40]
0065f764: cmp      ip, #0
0065f768: str      r3, [sp, #0x44]
0065f76c: ldr      r3, [r5, #0x40]
0065f770: ldr      ip, [sp, #0x1c]
0065f774: mov      r2, r6
0065f778: addeq    r3, r3, fp
0065f77c: ldr      r0, [sp, #0x24]
0065f780: ldr      r1, [sp, #0x14]
0065f784: add      r4, r4, #1
0065f788: str      ip, [sp]
0065f78c: bl       #0x66a1a8
0065f790: cmp      r4, sb
0065f794: bne      #0x65f6bc
0065f798: add      sp, sp, #0x4c
0065f79c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0065f7a0: ldr      r1, [r5, #0x14]
0065f7a4: mov      r0, r4
0065f7a8: bl       #0x30eb2c
0065f7ac: str      r1, [sp, #0x14]
0065f7b0: b        #0x65f634

# _ZN11AnimatorSet19setCurrentAnimationEi
00367420: push     {r4, r5, r6, lr}
00367424: mov      r4, r0
00367428: ldr      r0, [r0, #0x94]
0036742c: mov      r5, r1
00367430: bl       #0x364bf4
00367434: ldr      r3, [r0, #0x20]
00367438: cmn      r3, #1
0036743c: beq      #0x367480
00367440: ldr      r2, [r0, #0x24]
00367444: ldr      r3, [r0, #0x2c]
00367448: mov      r1, r5
0036744c: add      r2, r2, #1
00367450: add      r3, r3, #1
00367454: str      r2, [r0, #0x24]
00367458: str      r3, [r0, #0x2c]
0036745c: ldr      r3, [r4, #0x98]
00367460: str      r0, [r4, #0x98]
00367464: mov      r0, r4
00367468: cmp      r3, #0
0036746c: ldrne    r2, [r3, #0x24]
00367470: subne    r2, r2, #1
00367474: strne    r2, [r3, #0x24]
00367478: pop      {r4, r5, r6, lr}
0036747c: b        #0x65f8c8
00367480: pop      {r4, r5, r6, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEEEE15getBlendedValueEPvPfiSD_
00626df8: cmp      r3, #1
00626dfc: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00626e00: mov      r4, r3
00626e04: mov      fp, r2
00626e08: beq      #0x626eb4
00626e0c: cmp      r3, #0
00626e10: moveq    r8, #0
00626e14: moveq    sb, r8
00626e18: moveq    sl, r8
00626e1c: beq      #0x626e9c
00626e20: mov      r8, #0
00626e24: mov      r5, r1
00626e28: mov      r7, #0
00626e2c: mov      sb, r8
00626e30: mov      sl, r8
00626e34: ldr      r6, [fp, r7]
00626e38: ldr      r1, [r5]
00626e3c: add      r7, r7, #4
00626e40: mov      r0, r6
00626e44: bl       #0x30ed6c
00626e48: mov      r1, r0
00626e4c: mov      r0, r8
00626e50: bl       #0x30eba4
00626e54: ldr      r1, [r5, #4]
00626e58: mov      r8, r0
00626e5c: mov      r0, r6
00626e60: bl       #0x30ed6c
00626e64: mov      r1, r0
00626e68: mov      r0, sb
00626e6c: bl       #0x30eba4
00626e70: ldr      r1, [r5, #8]
00626e74: mov      sb, r0
00626e78: mov      r0, r6
00626e7c: bl       #0x30ed6c
00626e80: mov      r1, r0
00626e84: mov      r0, sl
00626e88: bl       #0x30eba4
00626e8c: subs     r4, r4, #1
00626e90: mov      sl, r0
00626e94: add      r5, r5, #0xc
00626e98: bne      #0x626e34
00626e9c: ldr      r3, [sp, #0x28]
00626ea0: str      r8, [r3], #4
00626ea4: ldr      r2, [sp, #0x28]
00626ea8: str      sb, [r2, #4]
00626eac: str      sl, [r3, #4]
00626eb0: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00626eb4: mov      r2, r1
00626eb8: ldr      r0, [r2], #4
00626ebc: ldr      r3, [sp, #0x28]
00626ec0: str      r0, [r3], #4
00626ec4: ldr      r1, [r1, #4]
00626ec8: ldr      r0, [sp, #0x28]
00626ecc: str      r1, [r0, #4]
00626ed0: ldr      r2, [r2, #4]
00626ed4: str      r2, [r3, #4]
00626ed8: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada25CSceneNodeAnimatorBlender20applyAnimationValuesEj
0065e350: push     {r4, r5, r6, r7, lr}
0065e354: ldr      r6, [r0, #0x2c]
0065e358: ldr      r3, [r0, #0x28]
0065e35c: sub      sp, sp, #0xc
0065e360: mov      r4, r0
0065e364: rsb      r6, r3, r6
0065e368: asrs     r6, r6, #2
0065e36c: mov      r7, r1
0065e370: beq      #0x65e3c8
0065e374: mov      r5, #0
0065e378: b        #0x65e388
0065e37c: add      r5, r5, #1
0065e380: cmp      r5, r6
0065e384: beq      #0x65e3c8
0065e388: ldr      r3, [r4, #0x34]
0065e38c: mov      r1, #0
0065e390: ldr      r0, [r3, r5, lsl #2]
0065e394: bl       #0x30df8c
0065e398: cmp      r0, #0
0065e39c: bne      #0x65e37c
0065e3a0: ldr      r3, [r4, #0x28]
0065e3a4: mov      r1, r7
0065e3a8: ldr      r3, [r3, r5, lsl #2]
0065e3ac: add      r5, r5, #1
0065e3b0: mov      r0, r3
0065e3b4: ldr      r3, [r3]
0065e3b8: mov      lr, pc
0065e3bc: ldr      pc, [r3, #0x4c]
0065e3c0: cmp      r5, r6
0065e3c4: bne      #0x65e388
0065e3c8: mov      r0, r4
0065e3cc: bl       #0x366594
0065e3d0: ldr      r2, [r4, #0x5c]
0065e3d4: ldr      r3, [r4, #0x58]
0065e3d8: rsb      r3, r3, r2
0065e3dc: lsrs     r3, r3, #2
0065e3e0: beq      #0x65e484
0065e3e4: mov      r5, #0
0065e3e8: mov      r1, r5
0065e3ec: ldr      r3, [r4]
0065e3f0: mov      r0, r4
0065e3f4: mov      lr, pc
0065e3f8: ldr      pc, [r3, #0x80]
0065e3fc: cmp      r0, #0
0065e400: beq      #0x65e46c
0065e404: ldr      r3, [r4, #0x58]
0065e408: mov      r1, r5
0065e40c: ldr      r2, [r3, r5, lsl #2]
0065e410: cmp      r2, #0
0065e414: beq      #0x65e470
0065e418: ldr      r3, [r4, #0x28]
0065e41c: ldr      r3, [r3]
0065e420: mov      r0, r3
0065e424: ldr      r3, [r3]
0065e428: mov      lr, pc
0065e42c: ldr      pc, [r3, #0x58]
0065e430: ldr      r3, [r4, #0x58]
0065e434: ldr      r2, [r4, #0x4c]
0065e438: ldr      lr, [r4, #0x64]
0065e43c: ldr      r3, [r3, r5, lsl #2]
0065e440: ldr      r1, [r2, r5, lsl #2]
0065e444: ldr      ip, [r0]
0065e448: ldr      r2, [r4, #0x34]
0065e44c: str      r3, [sp]
0065e450: ldr      r3, [r4, #0x38]
0065e454: ldr      lr, [lr, r5, lsl #2]
0065e458: rsb      r3, r2, r3
0065e45c: str      lr, [sp, #4]
0065e460: asr      r3, r3, #2
0065e464: mov      lr, pc
0065e468: ldr      pc, [ip, #0x18]
0065e46c: ldr      r3, [r4, #0x58]
0065e470: ldr      r2, [r4, #0x5c]
0065e474: add      r5, r5, #1
0065e478: rsb      r3, r3, r2
0065e47c: cmp      r5, r3, asr #2
0065e480: blo      #0x65e3e8
0065e484: add      sp, sp, #0xc
0065e488: pop      {r4, r5, r6, r7, pc}

# _ZN15AnimatorBlender5BlendEi
0036679c: push     {r4, r5, r6, lr}
003667a0: ldr      r5, [r0, #0x2c]
003667a4: ldr      r2, [r0, #0x28]
003667a8: ldr      r3, [pc, #0xc0]
003667ac: sub      sp, sp, #8
003667b0: rsb      r5, r2, r5
003667b4: asr      r5, r5, #2
003667b8: cmp      r5, #2
003667bc: mov      r4, r0
003667c0: mov      r6, r1
003667c4: add      r3, pc, r3
003667c8: beq      #0x3667f0
003667cc: ldr      r2, [pc, #0xa0]
003667d0: ldr      r2, [r3, r2]
003667d4: ldr      r2, [r2]
003667d8: cmp      r2, #2
003667dc: moveq    r3, #0
003667e0: streq    r3, [r3]
003667e4: beq      #0x3667f0
003667e8: cmp      r2, #1
003667ec: beq      #0x36683c
003667f0: ldr      r0, [r4, #0x70]
003667f4: mov      r1, r5
003667f8: str      r0, [r4, #0x74]
003667fc: add      r0, r0, #1
00366800: bl       #0x30eb2c
00366804: ldr      r0, [r4, #0x78]
00366808: str      r1, [r4, #0x70]
0036680c: cmp      r0, #0
00366810: str      r0, [r4, #0x7c]
00366814: ble      #0x36682c
00366818: bl       #0x30e964
0036681c: mov      r1, r0
00366820: mov      r0, #0x3f800000
00366824: bl       #0x30ec94
00366828: str      r0, [r4, #0x80]
0036682c: bic      r6, r6, r6, asr #31
00366830: str      r6, [r4, #0x78]
00366834: add      sp, sp, #8
00366838: pop      {r4, r5, r6, pc}
0036683c: ldr      r0, [pc, #0x34]
00366840: ldr      r1, [pc, #0x34]
00366844: ldr      r2, [pc, #0x34]
00366848: ldr      r0, [r3, r0]
0036684c: ldr      r3, [pc, #0x30]
00366850: mov      ip, #0x7a
00366854: add      r1, pc, r1
00366858: add      r2, pc, r2
0036685c: add      r3, pc, r3
00366860: add      r0, r0, #0xa8
00366864: str      ip, [sp]
00366868: bl       #0x30e004
0036686c: b        #0x3667f0
00366870: rsbeq    lr, r2, ip, asr #5
00366874: andeq    r3, r0, r0, asr #19
00366878: andeq    r1, r0, r0, asr #19
0036687c: subseq   r7, r5, r4, lsl #23
00366880: subseq   sl, r5, r8, lsl #11

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE17applyBlendedValueEPvPfiSA_PNS1_15CApplicatorInfoE
006209b4: mov      r0, r1
006209b8: ldr      ip, [sp, #4]
006209bc: mov      r1, r2
006209c0: mov      r2, r3
006209c4: ldr      r3, [sp]
006209c8: str      ip, [sp]
006209cc: b        #0x620968

# _ZN15AnimatorBlender12SetCallbacksEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
00366eb8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00366ebc: mov      r4, r0
00366ec0: ldr      r6, [r4, #0x2c]
00366ec4: ldr      r0, [r0, #0x28]
00366ec8: ldr      r5, [pc, #0xa4]
00366ecc: sub      sp, sp, #0xc
00366ed0: rsb      r6, r0, r6
00366ed4: asrs     r6, r6, #2
00366ed8: add      r5, pc, r5
00366edc: mov      r8, r1
00366ee0: mov      r7, r2
00366ee4: mov      sb, r3
00366ee8: ldr      fp, [sp, #0x30]
00366eec: beq      #0x366f5c
00366ef0: ldr      r2, [pc, #0x80]
00366ef4: mov      sl, #0
00366ef8: str      r2, [sp, #4]
00366efc: b        #0x366f04
00366f00: ldr      r0, [r4, #0x28]
00366f04: ldr      r3, [r0, sl, lsl #2]
00366f08: lsl      r1, sl, #2
00366f0c: add      sl, sl, #1
00366f10: ldr      r2, [r3, #0x18]
00366f14: str      sb, [r3, #0x1c]
00366f18: str      fp, [r3, #0x20]
00366f1c: cmp      r2, #0
00366f20: strne    fp, [r2, #0xc]
00366f24: strne    sb, [r2, #8]
00366f28: ldr      r3, [r4, #0x28]
00366f2c: ldr      r3, [r3, r1]
00366f30: mov      r0, r3
00366f34: ldr      r3, [r3]
00366f38: mov      lr, pc
00366f3c: ldr      pc, [r3, #0x44]
00366f40: cmp      r0, #0
00366f44: ldrne    r2, [sp, #4]
00366f48: strne    r4, [r0, #0xc]
00366f4c: ldrne    r3, [r5, r2]
00366f50: strne    r3, [r0, #8]
00366f54: cmp      sl, r6
00366f58: bne      #0x366f00
00366f5c: add      r0, r4, #0x88
00366f60: mov      r1, r8
00366f64: mov      r2, r7
00366f68: add      sp, sp, #0xc
00366f6c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00366f70: b        #0x364400
00366f74: strhteq  sp, [r2], #-0xb8
00366f78: ldrdeq   r2, r3, [r0], -ip

# _ZN6glitch5scene10ISceneNode11setRotationERKNS_4core10quaternionE
005970f4: ldr      r3, [r1]
005970f8: ldr      r2, [r0, #0x11c]
005970fc: str      r3, [r0, #0xb8]
00597100: ldr      r3, [r1, #4]
00597104: orr      r2, r2, #4
00597108: str      r3, [r0, #0xbc]
0059710c: ldr      r3, [r1, #8]
00597110: str      r3, [r0, #0xc0]
00597114: ldr      r3, [r1, #0xc]
00597118: str      r2, [r0, #0x11c]
0059711c: str      r3, [r0, #0xc4]
00597120: bx       lr

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEEEE17applyBlendedValueEPvPfiSB_PNS1_15CApplicatorInfoE
0062859c: mov      r0, r1
006285a0: ldr      ip, [sp, #4]
006285a4: mov      r1, r2
006285a8: mov      r2, r3
006285ac: ldr      r3, [sp]
006285b0: str      ip, [sp]
006285b4: b        #0x6284a4

# _ZN6glitch7collada15animation_track8CBlenderINS_4core10quaternionELi1ES4_E17getBlendedValueExEPvPfiS6_
006130d4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006130d8: mov      ip, #0
006130dc: sub      sp, sp, #0x3c
006130e0: subs     r6, r2, #0
006130e4: mov      r2, #0x3f800000
006130e8: mov      r8, r0
006130ec: str      r2, [sp, #0x34]
006130f0: mov      r5, r1
006130f4: mov      sb, r3
006130f8: str      ip, [sp, #0x28]
006130fc: str      ip, [sp, #0x2c]
00613100: str      ip, [sp, #0x30]
00613104: ble      #0x61328c
00613108: mov      r1, ip
0061310c: ldr      r0, [r5]
00613110: bl       #0x30df8c
00613114: cmp      r0, #0
00613118: moveq    r3, #0
0061311c: moveq    r7, r5
00613120: moveq    r4, r3
00613124: beq      #0x613224
00613128: mov      r7, r5
0061312c: mov      r4, #0
00613130: b        #0x613144
00613134: ldr      r0, [r7, #4]!
00613138: bl       #0x30df8c
0061313c: cmp      r0, #0
00613140: beq      #0x613220
00613144: add      r4, r4, #1
00613148: cmp      r4, r6
0061314c: mov      r1, #0
00613150: bne      #0x613134
00613154: add      r4, r6, #1
00613158: mov      sl, #0
0061315c: cmp      r6, r4
00613160: ble      #0x6131f8
00613164: add      r3, sp, #4
00613168: add      r5, r5, r4, lsl #2
0061316c: add      r8, r8, r4, lsl #4
00613170: add      fp, sp, #0x28
00613174: str      r3, [sp, #0x24]
00613178: b        #0x61318c
0061317c: cmp      r4, r6
00613180: add      r5, r5, #4
00613184: add      r8, r8, #0x10
00613188: beq      #0x6131f8
0061318c: ldr      r7, [r5]
00613190: mov      r1, #0
00613194: add      r4, r4, #1
00613198: mov      r0, r7
0061319c: bl       #0x30df8c
006131a0: cmp      r0, #0
006131a4: bne      #0x61317c
006131a8: mov      r0, sl
006131ac: mov      r1, r7
006131b0: bl       #0x30eba4
006131b4: ldr      ip, [sp, #0x24]
006131b8: mov      sl, r0
006131bc: ldm      r8, {r0, r1, r2, r3}
006131c0: stm      ip, {r0, r1, r2, r3}
006131c4: mov      r1, sl
006131c8: mov      r0, r7
006131cc: bl       #0x30ec94
006131d0: ldm      fp, {r1, r2, r3}
006131d4: ldr      ip, [sp, #0x34]
006131d8: str      r0, [sp, #0x14]
006131dc: mov      r0, fp
006131e0: str      ip, [sp]
006131e4: bl       #0x612d00
006131e8: cmp      r4, r6
006131ec: add      r5, r5, #4
006131f0: add      r8, r8, #0x10
006131f4: bne      #0x61318c
006131f8: ldr      r1, [sp, #0x2c]
006131fc: ldr      r3, [sp, #0x30]
00613200: ldr      r2, [sp, #0x34]
00613204: ldr      r0, [sp, #0x28]
00613208: str      r1, [sb, #4]
0061320c: str      r2, [sb, #0xc]
00613210: str      r0, [sb]
00613214: str      r3, [sb, #8]
00613218: add      sp, sp, #0x3c
0061321c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00613220: lsl      r3, r4, #4
00613224: ldr      sl, [r7]
00613228: add      r2, r8, r3
0061322c: ldr      r7, [r8, r3]
00613230: ldr      fp, [r2, #0xc]
00613234: ldr      r3, [r2, #4]
00613238: ldr      r2, [r2, #8]
0061323c: mov      r0, sl
00613240: mov      r1, #0x3f800000
00613244: str      r3, [sp, #0x2c]
00613248: str      r2, [sp, #0x30]
0061324c: str      r2, [sp, #0x1c]
00613250: str      r3, [sp, #0x20]
00613254: str      r7, [sp, #0x28]
00613258: str      fp, [sp, #0x34]
0061325c: bl       #0x30df8c
00613260: cmp      r0, #0
00613264: ldr      r2, [sp, #0x1c]
00613268: ldr      r3, [sp, #0x20]
0061326c: beq      #0x613284
00613270: str      fp, [sb, #0xc]
00613274: str      r7, [sb]
00613278: str      r3, [sb, #4]
0061327c: str      r2, [sb, #8]
00613280: b        #0x613218
00613284: add      r4, r4, #1
00613288: b        #0x61315c
0061328c: mov      r4, #1
00613290: b        #0x613158

# _ZN6glitch7collada25CSceneNodeAnimatorBlenderC2Ev
00366f7c: push     {r4, r5, r6, lr}
00366f80: mov      r6, r1
00366f84: ldr      r5, [pc, #0x84]
00366f88: add      r1, r1, #4
00366f8c: mov      r4, r0
00366f90: bl       #0x6698fc
00366f94: ldr      r2, [r6]
00366f98: ldr      r3, [pc, #0x74]
00366f9c: add      r5, pc, r5
00366fa0: str      r2, [r4]
00366fa4: ldr      r3, [r5, r3]
00366fa8: ldr      r1, [r2, #-0xc]
00366fac: ldr      r0, [r6, #0x1c]
00366fb0: add      r2, r3, #0xa0
00366fb4: mov      r3, #0
00366fb8: str      r0, [r4, r1]
00366fbc: str      r2, [r4, #4]
00366fc0: str      r3, [r4, #0x6c]
00366fc4: str      r3, [r4, #0x28]
00366fc8: str      r3, [r4, #0x2c]
00366fcc: str      r3, [r4, #0x30]
00366fd0: str      r3, [r4, #0x34]
00366fd4: str      r3, [r4, #0x38]
00366fd8: str      r3, [r4, #0x3c]
00366fdc: str      r3, [r4, #0x40]
00366fe0: str      r3, [r4, #0x44]
00366fe4: str      r3, [r4, #0x48]
00366fe8: str      r3, [r4, #0x4c]
00366fec: str      r3, [r4, #0x50]
00366ff0: str      r3, [r4, #0x54]
00366ff4: str      r3, [r4, #0x58]
00366ff8: str      r3, [r4, #0x5c]
00366ffc: str      r3, [r4, #0x60]
00367000: str      r3, [r4, #0x64]
00367004: str      r3, [r4, #0x68]
00367008: mov      r0, r4
0036700c: pop      {r4, r5, r6, pc}

# _ZN13RootSceneNode9onAnimateEj
0035d168: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0035d16c: ldr      r7, [pc, #0x338]
0035d170: ldr      sl, [pc, #0x338]
0035d174: ldrb     r3, [r0, #0x209]
0035d178: add      r7, pc, r7
0035d17c: ldr      r2, [r7, sl]
0035d180: sub      sp, sp, #0x3c
0035d184: cmp      r3, #0
0035d188: ldr      r2, [r2]
0035d18c: mov      r5, r0
0035d190: mov      r6, r1
0035d194: str      r2, [sp, #0x34]
0035d198: ldreq    r2, [r0, #0x11c]
0035d19c: beq      #0x35d1b0
0035d1a0: ldr      r2, [r0, #0x11c]
0035d1a4: ands     r4, r2, #1
0035d1a8: movne    r3, #0
0035d1ac: beq      #0x35d310
0035d1b0: tst      r2, #0x400
0035d1b4: beq      #0x35d1e0
0035d1b8: tst      r2, #1
0035d1bc: bne      #0x35d1e0
0035d1c0: ldr      r3, [r7, sl]
0035d1c4: str      r6, [r5, #0x1fc]
0035d1c8: ldr      r2, [sp, #0x34]
0035d1cc: ldr      r3, [r3]
0035d1d0: cmp      r2, r3
0035d1d4: bne      #0x35d4a8
0035d1d8: add      sp, sp, #0x3c
0035d1dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0035d1e0: tst      r2, #0x200
0035d1e4: beq      #0x35d1c0
0035d1e8: ldr      r2, [r5, #0x204]
0035d1ec: cmp      r2, #0
0035d1f0: beq      #0x35d330
0035d1f4: cmp      r3, #0
0035d1f8: bne      #0x35d330
0035d1fc: ldr      r3, [pc, #0x2b0]
0035d200: ldr      r3, [r7, r3]
0035d204: ldrb     r3, [r3, #0x30]
0035d208: cmp      r3, #0
0035d20c: bne      #0x35d330
0035d210: ldr      r4, [r2, #0x130]
0035d214: ldr      r3, [r2, #0x140]
0035d218: ldr      lr, [r2, #0x134]
0035d21c: ldr      r0, [r2, #0x138]
0035d220: ldr      r1, [r2, #0x13c]
0035d224: ldr      ip, [r2, #0x12c]
0035d228: str      r4, [sp, #4]
0035d22c: str      lr, [sp, #8]
0035d230: str      ip, [sp]
0035d234: str      r0, [sp, #0xc]
0035d238: str      r1, [sp, #0x10]
0035d23c: str      r3, [sp, #0x14]
0035d240: ldrb     r3, [r2, #0x2f9]
0035d244: cmp      r3, #0
0035d248: moveq    r4, sp
0035d24c: beq      #0x35d270
0035d250: ldr      r3, [r5]
0035d254: mov      r0, r5
0035d258: mov      lr, pc
0035d25c: ldr      pc, [r3, #0x34]
0035d260: mov      r1, r0
0035d264: mov      r0, sp
0035d268: mov      r4, sp
0035d26c: bl       #0x35c150
0035d270: ldr      r3, [pc, #0x240]
0035d274: ldr      r0, [r7, r3]
0035d278: bl       #0x582138
0035d27c: mov      r1, sp
0035d280: bl       #0x35bec8
0035d284: cmp      r0, #0
0035d288: bne      #0x35d330
0035d28c: ldr      r3, [r5, #0x11c]
0035d290: tst      r3, #0x400
0035d294: beq      #0x35d330
0035d298: ldrb     r3, [r5, #0x20a]
0035d29c: cmp      r3, #0
0035d2a0: movne    fp, #1
0035d2a4: bne      #0x35d334
0035d2a8: ldr      r3, [r5]
0035d2ac: mov      r0, r5
0035d2b0: mov      r1, r6
0035d2b4: mov      lr, pc
0035d2b8: ldr      pc, [r3, #0x18]
0035d2bc: ldrb     r3, [r5, #0x1ec]
0035d2c0: cmp      r3, #0
0035d2c4: bne      #0x35d454
0035d2c8: ldrb     r3, [r5, #0x208]
0035d2cc: cmp      r3, #0
0035d2d0: beq      #0x35d2f0
0035d2d4: ldr      r3, [r5]
0035d2d8: mov      r0, r5
0035d2dc: mov      r1, #1
0035d2e0: mov      lr, pc
0035d2e4: ldr      pc, [r3, #0xb8]
0035d2e8: mov      r3, #0
0035d2ec: strb     r3, [r5, #0x208]
0035d2f0: ldr      r3, [pc, #0x1c4]
0035d2f4: mov      fp, #0
0035d2f8: ldr      r3, [r7, r3]
0035d2fc: ldr      r2, [r3]
0035d300: add      r2, r2, #1
0035d304: str      r2, [r3]
0035d308: strb     fp, [r5, #0x20a]
0035d30c: b        #0x35d1c0
0035d310: mov      r1, #1
0035d314: bl       #0x596ec4
0035d318: ldr      r0, [r5, #0x110]
0035d31c: bl       #0x5890a8
0035d320: ldr      r2, [r5, #0x11c]
0035d324: strb     r4, [r5, #0x209]
0035d328: mov      r3, #1
0035d32c: b        #0x35d1b0
0035d330: mov      fp, #0
0035d334: ldr      r3, [pc, #0x184]
0035d338: ldr      r2, [pc, #0x184]
0035d33c: add      r4, sp, #0x1c
0035d340: ldr      r3, [r7, r3]
0035d344: ldr      sb, [r7, r2]
0035d348: mov      r8, r5
0035d34c: ldr      r2, [r3]
0035d350: mov      r0, sb
0035d354: add      r2, r2, #1
0035d358: str      r2, [r3]
0035d35c: bl       #0x337888
0035d360: ldr      r1, [pc, #0x160]
0035d364: add      r2, sp, #0x18
0035d368: mov      r0, r4
0035d36c: add      r1, pc, r1
0035d370: bl       #0x3140ec
0035d374: mov      r1, r4
0035d378: mov      r0, sb
0035d37c: bl       #0x337a88
0035d380: mov      r0, r4
0035d384: bl       #0x3139ac
0035d388: ldr      r4, [r8, #0xfc]!
0035d38c: b        #0x35d3b0
0035d390: ldr      r3, [r4, #8]
0035d394: mov      r1, r5
0035d398: mov      r2, r6
0035d39c: mov      r0, r3
0035d3a0: ldr      r3, [r3]
0035d3a4: mov      lr, pc
0035d3a8: ldr      pc, [r3, #0x10]
0035d3ac: ldr      r4, [r4]
0035d3b0: cmp      r8, r4
0035d3b4: bne      #0x35d390
0035d3b8: ldrb     r3, [r5, #0x1ec]
0035d3bc: cmp      r3, #0
0035d3c0: bne      #0x35d42c
0035d3c4: ldr      r3, [r5, #0x204]
0035d3c8: cmp      r3, #0
0035d3cc: beq      #0x35d43c
0035d3d0: ldr      r3, [r3, #0x110]
0035d3d4: cmn      r3, #1
0035d3d8: beq      #0x35d43c
0035d3dc: mov      r8, r5
0035d3e0: ldr      r4, [r8, #0xf4]!
0035d3e4: b        #0x35d40c
0035d3e8: cmp      r4, #0
0035d3ec: moveq    r3, r4
0035d3f0: subne    r3, r4, #4
0035d3f4: mov      r0, r3
0035d3f8: mov      r1, r6
0035d3fc: ldr      r3, [r3]
0035d400: mov      lr, pc
0035d404: ldr      pc, [r3, #0x14]
0035d408: ldr      r4, [r4]
0035d40c: cmp      r4, r8
0035d410: bne      #0x35d3e8
0035d414: ldr      r3, [r5, #0x11c]
0035d418: eor      fp, fp, #1
0035d41c: strb     fp, [r5, #0x20a]
0035d420: bic      r3, r3, #0x20
0035d424: str      r3, [r5, #0x11c]
0035d428: b        #0x35d1c0
0035d42c: mov      r0, r5
0035d430: mov      r1, r6
0035d434: bl       #0x35cf48
0035d438: b        #0x35d3c4
0035d43c: ldr      r3, [r5]
0035d440: mov      r0, r5
0035d444: mov      r1, #0
0035d448: mov      lr, pc
0035d44c: ldr      pc, [r3, #0xb8]
0035d450: b        #0x35d3dc
0035d454: mov      r8, r5
0035d458: ldr      r4, [r8, #0xfc]!
0035d45c: b        #0x35d47c
0035d460: ldr      r0, [r4, #8]
0035d464: bl       #0x369160
0035d468: mov      r1, r6
0035d46c: ldr      r3, [r0]
0035d470: mov      lr, pc
0035d474: ldr      pc, [r3, #0x10]
0035d478: ldr      r4, [r4]
0035d47c: cmp      r8, r4
0035d480: bne      #0x35d460
0035d484: mov      r0, r5
0035d488: mov      r1, r6
0035d48c: bl       #0x35cf48
0035d490: ldr      r3, [r5, #0x11c]
0035d494: cmp      r0, #0
0035d498: bic      r3, r3, #0x20
0035d49c: str      r3, [r5, #0x11c]
0035d4a0: bne      #0x35d2d4
0035d4a4: b        #0x35d2c8
0035d4a8: bl       #0x30e310
0035d4ac: rsbeq    r7, r3, r8, lsl sb
0035d4b0: andeq    r4, r0, ip, lsr #1
0035d4b4: andeq    r1, r0, r0, lsr #20
0035d4b8: muleq    r0, ip, r5
0035d4bc: muleq    r0, r4, r7
0035d4c0: andeq    r0, r0, r0, asr #30
0035d4c4: andeq    r0, r0, r4, lsl #17
0035d4c8: subseq   r3, r6, ip, lsl #19

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEEEE12getValueSizeEv
0060eff8: mov      r0, #0xc
0060effc: bx       lr

# _ZN6glitch7collada18ISceneNodeAnimator14setEventsTrackEPKNS0_12SEventsTrackE
0060fab8: push     {r4, r5, r6, lr}
0060fabc: mov      r5, r0
0060fac0: ldr      r0, [r0, #0x18]
0060fac4: ldr      r4, [pc, #0x7c]
0060fac8: mov      r6, r1
0060facc: cmp      r0, #0
0060fad0: add      r4, pc, r4
0060fad4: beq      #0x60fadc
0060fad8: bl       #0x31d584
0060fadc: cmp      r6, #0
0060fae0: beq      #0x60fb40
0060fae4: mov      r0, #0x18
0060fae8: mov      r1, #0
0060faec: bl       #0x5341ac
0060faf0: ldr      r3, [pc, #0x54]
0060faf4: ldr      r2, [pc, #0x54]
0060faf8: str      r6, [r0, #0x14]
0060fafc: ldr      r3, [r4, r3]
0060fb00: ldr      r2, [r4, r2]
0060fb04: add      r3, r3, #8
0060fb08: str      r2, [r0, #8]
0060fb0c: mov      r2, #0
0060fb10: str      r2, [r0, #0xc]
0060fb14: str      r3, [r0]
0060fb18: mov      r2, #1
0060fb1c: mvn      r3, #0
0060fb20: str      r2, [r0, #4]
0060fb24: str      r3, [r0, #0x10]
0060fb28: ldr      r2, [r5, #0x1c]
0060fb2c: ldr      r3, [r5, #0x20]
0060fb30: str      r0, [r5, #0x18]
0060fb34: str      r2, [r0, #8]
0060fb38: str      r3, [r0, #0xc]
0060fb3c: pop      {r4, r5, r6, pc}
0060fb40: str      r6, [r5, #0x18]
0060fb44: pop      {r4, r5, r6, pc}
0060fb48: eorseq   r4, r8, r0, asr #31
0060fb4c: ldrdeq   r4, r5, [r0], -r0
0060fb50: andeq    r4, r0, ip, lsr #10

# _ZN11AnimatorSet19SetCurrentAnimationEi
003674ac: push     {r4, r5, r6, lr}
003674b0: mov      r5, r0
003674b4: ldr      r0, [r0, #0x94]
003674b8: bl       #0x3660f4
003674bc: mov      r4, r0
003674c0: ldr      r0, [r0, #0x20]
003674c4: cmn      r0, #1
003674c8: beq      #0x36750c
003674cc: ldr      r2, [r4, #0x24]
003674d0: ldr      r3, [r4, #0x2c]
003674d4: mov      r0, r5
003674d8: add      r2, r2, #1
003674dc: add      r3, r3, #1
003674e0: str      r2, [r4, #0x24]
003674e4: str      r3, [r4, #0x2c]
003674e8: ldr      r3, [r5, #0x98]
003674ec: str      r4, [r5, #0x98]
003674f0: cmp      r3, #0
003674f4: ldrne    r2, [r3, #0x24]
003674f8: subne    r2, r2, #1
003674fc: strne    r2, [r3, #0x24]
00367500: ldr      r1, [r4, #0x20]
00367504: bl       #0x65f8c8
00367508: ldr      r0, [r4, #0x20]
0036750c: pop      {r4, r5, r6, pc}

# _ZN6glitch5scene10ISceneNode11setPositionEfff
00597154: str      lr, [sp, #-4]!
00597158: sub      sp, sp, #0x14
0059715c: str      r1, [sp, #4]
00597160: str      r3, [sp, #0xc]
00597164: str      r2, [sp, #8]
00597168: ldr      r3, [r0]
0059716c: add      r1, sp, #4
00597170: mov      lr, pc
00597174: ldr      pc, [r3, #0xa4]
00597178: add      sp, sp, #0x14
0059717c: ldm      sp!, {pc}

# _ZNK6glitch7collada16CColladaDatabase15getDefaultValueEPKcNS0_8SChannel4TypeEPPvS6_
0061c2f8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061c2fc: ldr      r4, [pc, #0x3a4]
0061c300: ldr      r5, [pc, #0x3a4]
0061c304: mov      r7, r3
0061c308: add      r4, pc, r4
0061c30c: ldr      ip, [r4, r5]
0061c310: sub      sp, sp, #0x7c
0061c314: mov      r6, r1
0061c318: ldr      r3, [ip]
0061c31c: ldr      r8, [sp, #0xa0]
0061c320: str      r3, [sp, #0x74]
0061c324: cmp      r2, #0x5b
0061c328: addls    pc, pc, r2, lsl #2
0061c32c: b        #0x61c5d4
0061c330: b        #0x61c5e0
0061c334: b        #0x61c660
0061c338: b        #0x61c660
0061c33c: b        #0x61c660
0061c340: b        #0x61c660
0061c344: b        #0x61c670
0061c348: b        #0x61c670
0061c34c: b        #0x61c670
0061c350: b        #0x61c670
0061c354: b        #0x61c670
0061c358: b        #0x61c548
0061c35c: b        #0x61c548
0061c360: b        #0x61c5e8
0061c364: b        #0x61c600
0061c368: b        #0x61c61c
0061c36c: b        #0x61c524
0061c370: b        #0x61c564
0061c374: b        #0x61c524
0061c378: b        #0x61c524
0061c37c: b        #0x61c524
0061c380: b        #0x61c644
0061c384: b        #0x61c524
0061c388: b        #0x61c524
0061c38c: b        #0x61c524
0061c390: b        #0x61c524
0061c394: b        #0x61c524
0061c398: b        #0x61c524
0061c39c: b        #0x61c5d4
0061c3a0: b        #0x61c5d4
0061c3a4: b        #0x61c5d4
0061c3a8: b        #0x61c5d4
0061c3ac: b        #0x61c5d4
0061c3b0: b        #0x61c5d4
0061c3b4: b        #0x61c5d4
0061c3b8: b        #0x61c5d4
0061c3bc: b        #0x61c5d4
0061c3c0: b        #0x61c5d4
0061c3c4: b        #0x61c5d4
0061c3c8: b        #0x61c5d4
0061c3cc: b        #0x61c5d4
0061c3d0: b        #0x61c5d4
0061c3d4: b        #0x61c5d4
0061c3d8: b        #0x61c5d4
0061c3dc: b        #0x61c5d4
0061c3e0: b        #0x61c5d4
0061c3e4: b        #0x61c5d4
0061c3e8: b        #0x61c5d4
0061c3ec: b        #0x61c5d4
0061c3f0: b        #0x61c5d4
0061c3f4: b        #0x61c5d4
0061c3f8: b        #0x61c5d4
0061c3fc: b        #0x61c5d4
0061c400: b        #0x61c5d4
0061c404: b        #0x61c5d4
0061c408: b        #0x61c5d4
0061c40c: b        #0x61c5d4
0061c410: b        #0x61c5d4
0061c414: b        #0x61c5d4
0061c418: b        #0x61c5d4
0061c41c: b        #0x61c5d4
0061c420: b        #0x61c5d4
0061c424: b        #0x61c5d4
0061c428: b        #0x61c5d4
0061c42c: b        #0x61c5d4
0061c430: b        #0x61c5d4
0061c434: b        #0x61c5d4
0061c438: b        #0x61c5d4
0061c43c: b        #0x61c5d4
0061c440: b        #0x61c5d4
0061c444: b        #0x61c5d4
0061c448: b        #0x61c5d4
0061c44c: b        #0x61c524
0061c450: b        #0x61c524
0061c454: b        #0x61c524
0061c458: b        #0x61c524
0061c45c: b        #0x61c524
0061c460: b        #0x61c524
0061c464: b        #0x61c524
0061c468: b        #0x61c524
0061c46c: b        #0x61c524
0061c470: b        #0x61c524
0061c474: b        #0x61c524
0061c478: b        #0x61c524
0061c47c: b        #0x61c524
0061c480: b        #0x61c524
0061c484: b        #0x61c524
0061c488: b        #0x61c580
0061c48c: b        #0x61c524
0061c490: b        #0x61c524
0061c494: b        #0x61c524
0061c498: b        #0x61c524
0061c49c: b        #0x61c524
0061c4a0: ldr      r1, [pc, #0x208]
0061c4a4: add      sb, sp, #0x5c
0061c4a8: add      r8, sp, #0x44
0061c4ac: add      r1, pc, r1
0061c4b0: add      r2, sp, #0x10
0061c4b4: mov      r0, sb
0061c4b8: bl       #0x32603c
0061c4bc: mov      r2, r6
0061c4c0: mov      r0, r8
0061c4c4: mov      r1, sb
0061c4c8: bl       #0x56d8b0
0061c4cc: ldr      r1, [pc, #0x1e0]
0061c4d0: add      r6, sp, #0x2c
0061c4d4: add      r2, sp, #0xc
0061c4d8: add      r1, pc, r1
0061c4dc: add      sl, sp, #0x14
0061c4e0: mov      r0, r6
0061c4e4: bl       #0x32603c
0061c4e8: mov      r2, r6
0061c4ec: mov      r0, sl
0061c4f0: mov      r1, r8
0061c4f4: bl       #0x34e324
0061c4f8: mov      r1, #2
0061c4fc: ldr      r0, [sp, #0x28]
0061c500: bl       #0x60aca0
0061c504: mov      r0, sl
0061c508: bl       #0x60f494
0061c50c: mov      r0, r6
0061c510: bl       #0x60f494
0061c514: mov      r0, r8
0061c518: bl       #0x60f494
0061c51c: mov      r0, sb
0061c520: bl       #0x60f494
0061c524: mov      r0, #0
0061c528: str      r0, [r7]
0061c52c: ldr      r3, [r4, r5]
0061c530: ldr      r2, [sp, #0x74]
0061c534: ldr      r3, [r3]
0061c538: cmp      r2, r3
0061c53c: bne      #0x61c6a4
0061c540: add      sp, sp, #0x7c
0061c544: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0061c548: bl       #0x61c290
0061c54c: cmp      r0, #0
0061c550: addne    r0, r0, #0x28
0061c554: strne    r0, [r7]
0061c558: movne    r0, #1
0061c55c: bne      #0x61c52c
0061c560: b        #0x61c524
0061c564: bl       #0x61b1ec
0061c568: cmp      r0, #0
0061c56c: beq      #0x61c524
0061c570: add      r0, r0, #0xc
0061c574: str      r0, [r7]
0061c578: mov      r0, #1
0061c57c: b        #0x61c52c
0061c580: bl       #0x61ac28
0061c584: subs     sb, r0, #0
0061c588: beq      #0x61c4a0
0061c58c: ldr      fp, [sb, #0x10]
0061c590: cmp      fp, #0
0061c594: ble      #0x61c524
0061c598: ldr      r3, [pc, #0x118]
0061c59c: mov      r6, #0
0061c5a0: mov      sl, r6
0061c5a4: add      r3, pc, r3
0061c5a8: ldr      r2, [sb, #0x14]
0061c5ac: ldr      r1, [r8]
0061c5b0: add      r2, r2, r6
0061c5b4: ldr      r2, [r2, #4]
0061c5b8: cmp      r1, r2
0061c5bc: beq      #0x61c68c
0061c5c0: add      sl, sl, #1
0061c5c4: cmp      sl, fp
0061c5c8: add      r6, r6, #0x18
0061c5cc: bne      #0x61c5a8
0061c5d0: b        #0x61c524
0061c5d4: mov      r3, #0
0061c5d8: str      r3, [r7]
0061c5dc: b        #0x61c524
0061c5e0: bl       #0x61c290
0061c5e4: b        #0x61c524
0061c5e8: bl       #0x61c290
0061c5ec: cmp      r0, #0
0061c5f0: addne    r0, r0, #0x2c
0061c5f4: strne    r0, [r7]
0061c5f8: movne    r0, #1
0061c5fc: b        #0x61c52c
0061c600: bl       #0x61c290
0061c604: cmp      r0, #0
0061c608: addne    r0, r0, #0x30
0061c60c: strne    r0, [r7]
0061c610: movne    r0, #1
0061c614: bne      #0x61c52c
0061c618: b        #0x61c524
0061c61c: bl       #0x61a9a0
0061c620: cmp      r0, #0
0061c624: beq      #0x61c524
0061c628: ldr      r3, [r0, #8]
0061c62c: ldrb     r2, [r8]
0061c630: mov      r0, #1
0061c634: ldr      r3, [r3, #0x1c]
0061c638: add      r3, r3, r2, lsl #2
0061c63c: str      r3, [r7]
0061c640: b        #0x61c52c
0061c644: bl       #0x61c290
0061c648: cmp      r0, #0
0061c64c: addne    r0, r0, #0x34
0061c650: strne    r0, [r7]
0061c654: movne    r0, #1
0061c658: bne      #0x61c52c
0061c65c: b        #0x61c524
0061c660: bl       #0x61c290
0061c664: cmp      r0, #0
0061c668: bne      #0x61c570
0061c66c: b        #0x61c524
0061c670: bl       #0x61c290
0061c674: cmp      r0, #0
0061c678: addne    r0, r0, #0x18
0061c67c: strne    r0, [r7]
0061c680: movne    r0, #1
0061c684: bne      #0x61c52c
0061c688: b        #0x61c524
0061c68c: mov      r0, r3
0061c690: mov      r1, #1
0061c694: str      r3, [sp, #4]
0061c698: bl       #0x60aca0
0061c69c: ldr      r3, [sp, #4]
0061c6a0: b        #0x61c5c0
0061c6a4: bl       #0x30e310
0061c6a8: eorseq   r8, r7, r8, lsl #15
0061c6ac: andeq    r4, r0, ip, lsr #1
0061c6b0: eoreq    r8, ip, r4, lsl #18
0061c6b4: eoreq    r8, ip, r8, ror #17
0061c6b8: eoreq    r8, ip, ip, lsr #16

# _ZN6glitch7collada15animation_track13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEE19applyBlendedValueExEPvPfiSA_PNS1_15CApplicatorInfoE
0062008c: push     {r4, r5, r6, r7, r8, sl, lr}
00620090: cmp      r2, #1
00620094: sub      sp, sp, #0xc
00620098: mov      r4, r2
0062009c: mov      r5, r0
006200a0: mov      r6, r1
006200a4: mov      sl, r3
006200a8: beq      #0x62010c
006200ac: cmp      r2, #0
006200b0: moveq    r8, #0
006200b4: beq      #0x6200e8
006200b8: mov      r8, #0
006200bc: mov      r7, #0
006200c0: ldr      r1, [r6, r7]
006200c4: ldr      r0, [r5, r7]
006200c8: bl       #0x30ed6c
006200cc: mov      r1, r0
006200d0: mov      r0, r8
006200d4: bl       #0x30eba4
006200d8: subs     r4, r4, #1
006200dc: mov      r8, r0
006200e0: add      r7, r7, #4
006200e4: bne      #0x6200c0
006200e8: str      r8, [sp, #4]
006200ec: ldr      r3, [sp, #0x28]
006200f0: mov      r0, sl
006200f4: mov      r2, #0
006200f8: ldrh     r1, [r3, #8]
006200fc: add      r3, sp, #4
00620100: bl       #0x5c6b8c
00620104: add      sp, sp, #0xc
00620108: pop      {r4, r5, r6, r7, r8, sl, pc}
0062010c: ldr      r3, [r0]
00620110: str      r3, [sp, #4]
00620114: b        #0x6200ec

# _ZN6glitch7collada13CAnimationSet7compileEv
00660710: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00660714: mov      r4, r0
00660718: ldr      r0, [r0, #0x64]
0066071c: sub      sp, sp, #0x14
00660720: cmp      r0, #0
00660724: beq      #0x66072c
00660728: bl       #0x667320
0066072c: ldr      r3, [r4, #0x24]
00660730: ldr      r2, [r4, #0x28]
00660734: rsb      r1, r3, r2
00660738: lsrs     r1, r1, #3
0066073c: beq      #0x660800
00660740: mov      r8, #0
00660744: ldr      r1, [r3, r8, lsl #3]
00660748: add      r7, r3, r8, lsl #3
0066074c: ldr      r1, [r1, #0x24]
00660750: ldr      r1, [r1, #0x20]
00660754: ldr      r1, [r1, #0x24]
00660758: cmp      r1, #0
0066075c: ble      #0x6607f0
00660760: mov      r5, #0
00660764: b        #0x660784
00660768: ldr      r3, [r7]
0066076c: add      r5, r5, #1
00660770: ldr      r3, [r3, #0x24]
00660774: ldr      r3, [r3, #0x20]
00660778: ldr      r3, [r3, #0x24]
0066077c: cmp      r5, r3
00660780: bge      #0x6607e8
00660784: mov      r1, r5
00660788: mov      r0, r7
0066078c: bl       #0x60e35c
00660790: ldr      r3, [r4, #0x64]
00660794: mov      r6, r0
00660798: subs     r0, r3, #0
0066079c: beq      #0x6607b8
006607a0: ldr      r1, [r6, #0x10]
006607a4: ldr      r3, [r3]
006607a8: mov      lr, pc
006607ac: ldr      pc, [r3, #8]
006607b0: cmp      r0, #0
006607b4: beq      #0x660768
006607b8: ldr      r3, [r4]
006607bc: mov      r1, r6
006607c0: mov      r0, r4
006607c4: mov      lr, pc
006607c8: ldr      pc, [r3, #0xc]
006607cc: ldr      r3, [r7]
006607d0: add      r5, r5, #1
006607d4: ldr      r3, [r3, #0x24]
006607d8: ldr      r3, [r3, #0x20]
006607dc: ldr      r3, [r3, #0x24]
006607e0: cmp      r5, r3
006607e4: blt      #0x660784
006607e8: ldr      r3, [r4, #0x24]
006607ec: ldr      r2, [r4, #0x28]
006607f0: add      r8, r8, #1
006607f4: rsb      r1, r3, r2
006607f8: cmp      r8, r1, asr #3
006607fc: blo      #0x660744
00660800: ldr      r0, [r4, #0x64]
00660804: cmp      r0, #0
00660808: beq      #0x660820
0066080c: add      r2, r4, #0x18
00660810: add      r1, r4, #0xc
00660814: bl       #0x667384
00660818: ldr      r3, [r4, #0x24]
0066081c: ldr      r2, [r4, #0x28]
00660820: rsb      r1, r3, r2
00660824: lsrs     r1, r1, #3
00660828: beq      #0x660ae0
0066082c: ldr      r0, [r4, #0x10]
00660830: ldr      r1, [r4, #0xc]
00660834: mov      r8, #0
00660838: add      sl, sp, #0xc
0066083c: rsb      r0, r1, r0
00660840: asrs     ip, r0, #2
00660844: add      r6, r3, r8, lsl #3
00660848: beq      #0x66088c
0066084c: mov      r5, #0
00660850: ldr      r1, [r1, r5, lsl #2]
00660854: mov      r0, r6
00660858: bl       #0x61c1e0
0066085c: cmp      r0, #0
00660860: lsl      r7, r5, #2
00660864: beq      #0x660a18
00660868: ldr      r0, [r4, #0x10]
0066086c: ldr      r1, [r4, #0xc]
00660870: add      r5, r5, #1
00660874: rsb      r0, r1, r0
00660878: asr      ip, r0, #2
0066087c: cmp      r5, ip
00660880: blo      #0x660850
00660884: ldr      r3, [r4, #0x24]
00660888: ldr      r2, [r4, #0x28]
0066088c: add      r8, r8, #1
00660890: rsb      lr, r3, r2
00660894: cmp      r8, lr, asr #3
00660898: blo      #0x660840
0066089c: rsb      r3, r3, r2
006608a0: asr      r5, r3, #3
006608a4: mul      r5, r5, ip
006608a8: add      r6, r4, #0x30
006608ac: str      ip, [r4, #0x3c]
006608b0: mov      r0, r6
006608b4: mov      r1, r5
006608b8: bl       #0x62e5e0
006608bc: mov      r8, #0
006608c0: mov      r1, r5
006608c4: mov      r2, sp
006608c8: mov      r0, r6
006608cc: str      r8, [sp]
006608d0: str      r8, [sp, #4]
006608d4: str      r8, [sp, #8]
006608d8: bl       #0x62ec50
006608dc: ldr      r3, [r4, #0x24]
006608e0: ldr      r2, [r4, #0x28]
006608e4: rsb      r1, r3, r2
006608e8: lsrs     r1, r1, #3
006608ec: beq      #0x660a08
006608f0: ldr      r1, [r4, #0xc]
006608f4: ldr      r0, [r4, #0x10]
006608f8: mov      fp, r8
006608fc: mov      sb, #2
00660900: rsb      ip, r1, r0
00660904: lsrs     ip, ip, #2
00660908: add      sl, r3, fp, lsl #3
0066090c: beq      #0x6609f8
00660910: mov      r2, #0xc
00660914: mul      r6, r2, r8
00660918: mov      r5, #0
0066091c: b        #0x660954
00660920: ldr      r3, [r4, #0x30]
00660924: str      sb, [r3, r6]
00660928: ldr      r3, [r4, #0x30]
0066092c: add      r3, r3, r6
00660930: str      r7, [r3, #8]
00660934: ldr      r1, [r4, #0xc]
00660938: ldr      r0, [r4, #0x10]
0066093c: add      r5, r5, #1
00660940: add      r8, r8, #1
00660944: rsb      r3, r1, r0
00660948: cmp      r5, r3, asr #2
0066094c: add      r6, r6, #0xc
00660950: bhs      #0x6609f0
00660954: ldr      r1, [r1, r5, lsl #2]
00660958: mov      r0, sl
0066095c: bl       #0x61c1e0
00660960: ldr      r2, [r4, #0x30]
00660964: ldr      r3, [r4, #0xc]
00660968: mov      r7, r0
0066096c: add      r2, r2, r6
00660970: ldr      r1, [r3, r5, lsl #2]
00660974: mov      r0, sl
00660978: add      r2, r2, #4
0066097c: bl       #0x61c6bc
00660980: cmp      r7, #0
00660984: lsl      r1, r5, #2
00660988: bne      #0x660920
0066098c: ldr      r3, [r4, #0x30]
00660990: mov      r2, #1
00660994: cmp      r0, #0
00660998: str      r2, [r3, r6]
0066099c: bne      #0x660934
006609a0: ldr      r3, [r4, #0x64]
006609a4: cmp      r3, #0
006609a8: beq      #0x660934
006609ac: ldr      r2, [r4, #0x30]
006609b0: ldr      ip, [r4, #0xc]
006609b4: mov      r0, r3
006609b8: add      r2, r2, r6
006609bc: ldr      r1, [ip, r1]
006609c0: ldr      r3, [r3]
006609c4: add      r2, r2, #4
006609c8: mov      lr, pc
006609cc: ldr      pc, [r3, #0xc]
006609d0: ldr      r1, [r4, #0xc]
006609d4: ldr      r0, [r4, #0x10]
006609d8: add      r5, r5, #1
006609dc: add      r8, r8, #1
006609e0: rsb      r3, r1, r0
006609e4: cmp      r5, r3, asr #2
006609e8: add      r6, r6, #0xc
006609ec: blo      #0x660954
006609f0: ldr      r3, [r4, #0x24]
006609f4: ldr      r2, [r4, #0x28]
006609f8: add      fp, fp, #1
006609fc: rsb      ip, r3, r2
00660a00: cmp      fp, ip, asr #3
00660a04: blo      #0x660900
00660a08: mov      r0, r4
00660a0c: bl       #0x6605ac
00660a10: add      sp, sp, #0x14
00660a14: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00660a18: ldr      r3, [r4, #0xc]
00660a1c: mov      r0, r6
00660a20: mov      r2, sl
00660a24: ldr      r1, [r3, r5, lsl #2]
00660a28: bl       #0x61c6bc
00660a2c: cmp      r0, #0
00660a30: bne      #0x660868
00660a34: ldr      r3, [r4, #0x64]
00660a38: cmp      r3, #0
00660a3c: beq      #0x660a64
00660a40: ldr      r2, [r4, #0xc]
00660a44: mov      r0, r3
00660a48: ldr      r3, [r3]
00660a4c: ldr      r1, [r2, r7]
00660a50: mov      r2, sl
00660a54: mov      lr, pc
00660a58: ldr      pc, [r3, #0xc]
00660a5c: cmp      r0, #0
00660a60: bne      #0x660868
00660a64: ldr      r3, [r4, #8]
00660a68: cmp      r3, #0
00660a6c: bne      #0x660868
00660a70: ldr      r0, [r4, #0xc]
00660a74: ldr      r3, [r4, #0x10]
00660a78: add      r0, r0, r7
00660a7c: add      r1, r0, #4
00660a80: cmp      r1, r3
00660a84: beq      #0x660a9c
00660a88: subs     r2, r3, r1
00660a8c: moveq    r1, r3
00660a90: beq      #0x660a9c
00660a94: bl       #0x30df38
00660a98: ldr      r1, [r4, #0x10]
00660a9c: ldr      r0, [r4, #0x18]
00660aa0: ldr      r3, [r4, #0x1c]
00660aa4: sub      r2, r1, #4
00660aa8: add      r0, r0, r7
00660aac: add      r1, r0, #4
00660ab0: cmp      r1, r3
00660ab4: str      r2, [r4, #0x10]
00660ab8: beq      #0x660ad0
00660abc: subs     r2, r3, r1
00660ac0: moveq    r1, r3
00660ac4: beq      #0x660ad0
00660ac8: bl       #0x30df38
00660acc: ldr      r1, [r4, #0x1c]
00660ad0: sub      r1, r1, #4
00660ad4: str      r1, [r4, #0x1c]
00660ad8: sub      r5, r5, #1
00660adc: b        #0x660868
00660ae0: ldr      r1, [r4, #0xc]
00660ae4: ldr      ip, [r4, #0x10]
00660ae8: rsb      ip, r1, ip
00660aec: asr      ip, ip, #2
00660af0: b        #0x66089c

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE15getBlendedValueEPvPfiSA_
00613300: mov      r0, r1
00613304: mov      r1, r2
00613308: mov      r2, r3
0061330c: ldr      r3, [sp]
00613310: b        #0x6130d4

# _ZN24BlendedAnimSetController8PlayClipEjbij
0047680c: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00476810: mov      r4, r1
00476814: ldr      r1, [sp, #0x28]
00476818: mov      sl, r2
0047681c: mov      r7, r0
00476820: bl       #0x4748b8
00476824: subs     r6, r0, #0
00476828: beq      #0x476900
0047682c: ldr      r1, [r7, #0x14]
00476830: bl       #0x36679c
00476834: cmn      r4, #1
00476838: beq      #0x476900
0047683c: ldr      r2, [r6, #0x70]
00476840: ldr      r3, [r6, #0x28]
00476844: ldr      r5, [r3, r2, lsl #2]
00476848: cmp      r5, #0
0047684c: beq      #0x476908
00476850: ldr      r3, [r5]
00476854: mov      r0, r5
00476858: mov      lr, pc
0047685c: ldr      pc, [r3, #0x44]
00476860: mov      r8, r0
00476864: mov      r0, r5
00476868: bl       #0x65f114
0047686c: mov      sb, r0
00476870: mov      r0, r6
00476874: bl       #0x369160
00476878: mov      r1, r4
0047687c: mov      fp, r0
00476880: mov      r0, r5
00476884: bl       #0x3674ac
00476888: cmn      r0, #1
0047688c: mov      r4, r0
00476890: beq      #0x476900
00476894: ldr      r3, [r8, #0x34]
00476898: cmp      r3, #0
0047689c: beq      #0x4768b4
004768a0: mov      r0, r5
004768a4: ldr      r3, [r5]
004768a8: ldr      r1, [r7, #0xc]
004768ac: mov      lr, pc
004768b0: ldr      pc, [r3, #0x30]
004768b4: cmp      sb, r4
004768b8: beq      #0x476920
004768bc: mov      r1, sl
004768c0: mov      r0, r8
004768c4: ldr      r3, [r8]
004768c8: mov      lr, pc
004768cc: ldr      pc, [r3, #0x40]
004768d0: ldr      r3, [r8]
004768d4: mov      r0, r8
004768d8: mov      r1, #0x3f800000
004768dc: mov      lr, pc
004768e0: ldr      pc, [r3, #0x48]
004768e4: ldrb     r1, [r7, #0x10]
004768e8: ldr      r0, [r7, #4]
004768ec: bl       #0x35d624
004768f0: mov      r0, r6
004768f4: bl       #0x366740
004768f8: mov      r0, #1
004768fc: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476900: mov      r0, #0
00476904: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476908: mov      r0, r5
0047690c: bl       #0x65f114
00476910: mov      r0, r6
00476914: bl       #0x369160
00476918: mov      r0, r5
0047691c: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476920: ldr      r3, [r8]
00476924: mov      r0, r8
00476928: mov      lr, pc
0047692c: ldr      pc, [r3, #0x44]
00476930: cmp      r0, #0
00476934: bne      #0x4768bc
00476938: cmp      fp, #0
0047693c: ldr      r3, [r8]
00476940: ldr      r1, [r8, #0x10]
00476944: ldrne    fp, [fp, #0x10]
00476948: ldr      r3, [r3, #0xc]
0047694c: mov      r0, r8
00476950: add      r1, fp, r1
00476954: blx      r3
00476958: b        #0x4768bc

# _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEE19applyBlendedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
0062d634: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0062d638: cmp      r2, #1
0062d63c: sub      sp, sp, #0x1c
0062d640: mov      sl, #0
0062d644: mov      r4, r2
0062d648: mov      r5, r1
0062d64c: str      r3, [sp, #4]
0062d650: str      sl, [sp, #0x14]
0062d654: beq      #0x62d708
0062d658: cmp      r2, #0
0062d65c: moveq    fp, sl
0062d660: moveq    sb, sl
0062d664: beq      #0x62d6e0
0062d668: mov      r6, r0
0062d66c: mov      r8, #0
0062d670: mov      fp, sl
0062d674: mov      sb, sl
0062d678: ldr      r7, [r5, r8]
0062d67c: ldr      r1, [r6]
0062d680: add      r8, r8, #4
0062d684: mov      r0, r7
0062d688: bl       #0x30ed6c
0062d68c: mov      r1, r0
0062d690: mov      r0, sl
0062d694: bl       #0x30eba4
0062d698: ldr      r1, [r6, #4]
0062d69c: mov      sl, r0
0062d6a0: mov      r0, r7
0062d6a4: bl       #0x30ed6c
0062d6a8: mov      r1, r0
0062d6ac: mov      r0, fp
0062d6b0: bl       #0x30eba4
0062d6b4: ldr      r1, [r6, #8]
0062d6b8: mov      fp, r0
0062d6bc: mov      r0, r7
0062d6c0: bl       #0x30ed6c
0062d6c4: mov      r1, r0
0062d6c8: mov      r0, sb
0062d6cc: bl       #0x30eba4
0062d6d0: subs     r4, r4, #1
0062d6d4: mov      sb, r0
0062d6d8: add      r6, r6, #0xc
0062d6dc: bne      #0x62d678
0062d6e0: add      r1, sp, #0x18
0062d6e4: str      sl, [r1, #-0xc]!
0062d6e8: str      fp, [sp, #0x10]
0062d6ec: str      sb, [r1, #8]
0062d6f0: ldr      r0, [sp, #4]
0062d6f4: ldr      r3, [r0]
0062d6f8: mov      lr, pc
0062d6fc: ldr      pc, [r3, #0x94]
0062d700: add      sp, sp, #0x1c
0062d704: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0062d708: mov      r3, r0
0062d70c: ldr      ip, [r3], #4
0062d710: ldr      r2, [r0, #4]
0062d714: add      r1, sp, #0x18
0062d718: ldr      r3, [r3, #4]
0062d71c: str      ip, [r1, #-0xc]!
0062d720: str      r2, [sp, #0x10]
0062d724: str      r3, [r1, #8]
0062d728: b        #0x62d6f0

# _ZN6glitch7collada35CAnimationSetTransformationTemplate24addTransformationTargetsERNS0_5SNodeE
006e26d4: push     {r4, r5, r6, r7, lr}
006e26d8: mov      r4, r0
006e26dc: sub      sp, sp, #0xc
006e26e0: mov      r5, r1
006e26e4: mov      r0, #0x10
006e26e8: mov      r1, #0
006e26ec: bl       #0x5341ac
006e26f0: mov      r3, #0
006e26f4: str      r0, [sp, #4]
006e26f8: strb     r3, [r0]
006e26fc: ldr      r3, [sp, #4]
006e2700: mov      r2, #1
006e2704: add      r6, r4, #4
006e2708: str      r2, [r3, #4]
006e270c: ldr      r1, [r4, #8]
006e2710: ldr      r3, [r4, #0xc]
006e2714: cmp      r1, r3
006e2718: beq      #0x6e2800
006e271c: ldr      r3, [sp, #4]
006e2720: str      r3, [r1]
006e2724: ldr      r3, [r4, #8]
006e2728: add      r3, r3, #4
006e272c: str      r3, [r4, #8]
006e2730: mov      r1, #0
006e2734: mov      r0, #0x10
006e2738: bl       #0x5341ac
006e273c: mov      r3, #0
006e2740: str      r0, [sp, #4]
006e2744: strb     r3, [r0]
006e2748: ldr      r3, [sp, #4]
006e274c: mov      r2, #5
006e2750: str      r2, [r3, #4]
006e2754: ldr      r1, [r4, #8]
006e2758: ldr      r3, [r4, #0xc]
006e275c: cmp      r1, r3
006e2760: beq      #0x6e2810
006e2764: ldr      r3, [sp, #4]
006e2768: str      r3, [r1]
006e276c: ldr      r3, [r4, #8]
006e2770: add      r3, r3, #4
006e2774: str      r3, [r4, #8]
006e2778: mov      r1, #0
006e277c: mov      r0, #0x10
006e2780: bl       #0x5341ac
006e2784: mov      r3, #0
006e2788: str      r0, [sp, #4]
006e278c: strb     r3, [r0]
006e2790: ldr      r3, [sp, #4]
006e2794: mov      r2, #0xa
006e2798: str      r2, [r3, #4]
006e279c: ldr      r1, [r4, #8]
006e27a0: ldr      r3, [r4, #0xc]
006e27a4: cmp      r1, r3
006e27a8: beq      #0x6e2820
006e27ac: ldr      r3, [sp, #4]
006e27b0: str      r3, [r1]
006e27b4: ldr      r3, [r4, #8]
006e27b8: add      r3, r3, #4
006e27bc: str      r3, [r4, #8]
006e27c0: ldr      r3, [r5, #0x38]
006e27c4: cmp      r3, #0
006e27c8: movgt    r6, #0
006e27cc: movgt    r7, r6
006e27d0: ble      #0x6e27f8
006e27d4: ldr      r1, [r5, #0x3c]
006e27d8: mov      r0, r4
006e27dc: add      r7, r7, #1
006e27e0: add      r1, r1, r6
006e27e4: bl       #0x6e26d4
006e27e8: ldr      r3, [r5, #0x38]
006e27ec: add      r6, r6, #0x50
006e27f0: cmp      r7, r3
006e27f4: blt      #0x6e27d4
006e27f8: add      sp, sp, #0xc
006e27fc: pop      {r4, r5, r6, r7, pc}
006e2800: mov      r0, r6
006e2804: add      r2, sp, #4
006e2808: bl       #0x6e2434
006e280c: b        #0x6e2730
006e2810: mov      r0, r6
006e2814: add      r2, sp, #4
006e2818: bl       #0x6e2434
006e281c: b        #0x6e2778
006e2820: mov      r0, r6
006e2824: add      r2, sp, #4
006e2828: bl       #0x6e2434
006e282c: b        #0x6e27c0

# _ZN15AnimatorBlender17BlenderApplicator10SetRefNodeEPN6glitch5scene10ISceneNodeE
00366b80: push     {r4, r5, r6, r7, lr}
00366b84: ldr      r2, [r0, #0x3c]
00366b88: ldr      r3, [pc, #0xe4]
00366b8c: sub      sp, sp, #0xc
00366b90: cmp      r2, #0
00366b94: mov      r4, r0
00366b98: mov      r5, r1
00366b9c: add      r3, pc, r3
00366ba0: beq      #0x366c20
00366ba4: mov      r0, r4
00366ba8: mov      r1, r5
00366bac: bl       #0x36473c
00366bb0: ldr      r3, [r4, #0x3c]
00366bb4: ldr      r7, [r3, #0x2c]
00366bb8: ldr      r3, [r3, #0x28]
00366bbc: rsb      r7, r3, r7
00366bc0: asrs     r7, r7, #2
00366bc4: beq      #0x366c18
00366bc8: mov      r6, #0
00366bcc: b        #0x366bf4
00366bd0: ldr      r3, [r1]
00366bd4: add      r6, r6, #1
00366bd8: mov      r1, r5
00366bdc: mov      lr, pc
00366be0: ldr      pc, [r3, #8]
00366be4: cmp      r6, r7
00366be8: beq      #0x366c18
00366bec: ldr      r3, [r4, #0x3c]
00366bf0: ldr      r3, [r3, #0x28]
00366bf4: ldr      r0, [r3, r6, lsl #2]
00366bf8: bl       #0x369160
00366bfc: subs     r1, r0, #0
00366c00: bne      #0x366bd0
00366c04: mov      r0, r4
00366c08: add      r6, r6, #1
00366c0c: bl       #0x36473c
00366c10: cmp      r6, r7
00366c14: bne      #0x366bec
00366c18: add      sp, sp, #0xc
00366c1c: pop      {r4, r5, r6, r7, pc}
00366c20: ldr      r1, [pc, #0x50]
00366c24: ldr      r1, [r3, r1]
00366c28: ldr      r1, [r1]
00366c2c: cmp      r1, #2
00366c30: streq    r2, [r2]
00366c34: beq      #0x366ba4
00366c38: cmp      r1, #1
00366c3c: bne      #0x366ba4
00366c40: ldr      r0, [pc, #0x34]
00366c44: ldr      r1, [pc, #0x34]
00366c48: ldr      r2, [pc, #0x34]
00366c4c: ldr      r0, [r3, r0]
00366c50: ldr      r3, [pc, #0x30]
00366c54: mov      ip, #0x130
00366c58: add      r1, pc, r1
00366c5c: add      r2, pc, r2
00366c60: add      r3, pc, r3
00366c64: add      r0, r0, #0xa8
00366c68: str      ip, [sp]
00366c6c: bl       #0x30e004
00366c70: b        #0x366ba4

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEEEE12getValueSizeEv
0060ef64: mov      r0, #0xc
0060ef68: bx       lr

# _ZN6glitch7collada21CSceneNodeAnimatorSetC2ERKN5boost13intrusive_ptrINS0_13CAnimationSetEEE
00660ce4: push     {r4, r5, r6, r7, r8, lr}
00660ce8: mov      r5, r1
00660cec: ldr      r4, [pc, #0x90]
00660cf0: add      r1, r1, #4
00660cf4: mov      r6, r0
00660cf8: mov      r7, r2
00660cfc: bl       #0x6698fc
00660d00: ldr      r2, [r5]
00660d04: ldr      r3, [pc, #0x7c]
00660d08: add      r4, pc, r4
00660d0c: str      r2, [r6]
00660d10: ldr      r3, [r4, r3]
00660d14: ldr      r2, [r2, #-0xc]
00660d18: ldr      r1, [r5, #0x1c]
00660d1c: add      r3, r3, #0xa4
00660d20: mov      r0, r6
00660d24: str      r1, [r6, r2]
00660d28: str      r3, [r6, #4]
00660d2c: ldr      r3, [r7]
00660d30: mov      r1, r7
00660d34: cmp      r3, #0
00660d38: str      r3, [r6, #0x24]
00660d3c: ldrne    r2, [r3, #4]
00660d40: addne    r2, r2, #1
00660d44: strne    r2, [r3, #4]
00660d48: mov      r3, #0
00660d4c: str      r3, [r6, #0x54]
00660d50: str      r3, [r6, #0x28]
00660d54: str      r3, [r6, #0x2c]
00660d58: str      r3, [r6, #0x30]
00660d5c: str      r3, [r6, #0x34]
00660d60: str      r3, [r6, #0x38]
00660d64: str      r3, [r6, #0x3c]
00660d68: str      r3, [r6, #0x40]
00660d6c: str      r3, [r6, #0x44]
00660d70: str      r3, [r6, #0x48]
00660d74: str      r3, [r6, #0x4c]
00660d78: bl       #0x660af4
00660d7c: mov      r0, r6
00660d80: pop      {r4, r5, r6, r7, r8, pc}
00660d84: eorseq   r3, r3, r8, lsl #27
00660d88: andeq    r2, r0, r0, lsl #12
