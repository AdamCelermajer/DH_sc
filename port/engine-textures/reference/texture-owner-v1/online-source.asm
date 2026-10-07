_ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE8CTexture8bindImplEb
005b5610: push {r4, r5, r6, r7, r8, sb, sl, lr}
005b5614: ldr r3, [r0, #0x54]
005b5618: ldr r4, [r0, #0x34]
005b561c: ldr r7, [r0, #0x38]
005b5620: ldr r6, [pc, #0x2d8]
005b5624: cmp r3, #0
005b5628: and r7, r7, #3
005b562c: add r3, r4, #0x420
005b5630: mov r5, r0
005b5634: add r7, r3, r7, lsl #5
005b5638: add r6, pc, r6
005b563c: mov r8, r1
005b5640: beq #0x5b571c
005b5644: ldr r3, [r4, #0x268]
005b5648: ldr r2, [r7, r3, lsl #2]
005b564c: cmp r2, r0
005b5650: beq #0x5b56a4
005b5654: ldr r6, [r4, #0x4c]
005b5658: sub r6, r6, #1
005b565c: cmp r3, r6
005b5660: beq #0x5b5674
005b5664: add r0, r6, #0x8400
005b5668: add r0, r0, #0xc0
005b566c: bl #0x30e1e4
005b5670: str r6, [r4, #0x268]
005b5674: ldr r3, [r7, r6, lsl #2]
005b5678: cmp r5, r3
005b567c: beq #0x5b56a4
005b5680: ldr r3, [pc, #0x27c]
005b5684: ldr r2, [r5, #0x38]
005b5688: ldr r1, [r5, #0x54]
005b568c: add r3, pc, r3
005b5690: add r3, r3, #0xa4
005b5694: and r2, r2, #3
005b5698: ldr r0, [r3, r2, lsl #2]
005b569c: bl #0x30e7c0
005b56a0: str r5, [r7, r6, lsl #2]
005b56a4: ldrb r1, [r5, #0x58]
005b56a8: cmp r1, #0
005b56ac: bne #0x5b5828
005b56b0: ldrh r4, [r5, #0x40]
005b56b4: bic r4, r4, #2
005b56b8: lsl r4, r4, #0x13
005b56bc: lsr r4, r4, #0x13
005b56c0: cmp r4, #0
005b56c4: bne #0x5b583c
005b56c8: ldrb r3, [r5, #0x3f]
005b56cc: and r1, r3, #0x10
005b56d0: uxtb r1, r1
005b56d4: cmp r1, #0
005b56d8: beq #0x5b56f0
005b56dc: ldr r3, [r5, #0x54]
005b56e0: cmp r3, #0
005b56e4: bne #0x5b5864
005b56e8: mov r0, r4
005b56ec: pop {r4, r5, r6, r7, r8, sb, sl, pc}
005b56f0: cmp r8, #0
005b56f4: beq #0x5b56e8
005b56f8: ldr r2, [r5, #0x2c]
005b56fc: cmp r2, #0
005b5700: beq #0x5b56e8
005b5704: mov r0, r5
005b5708: ubfx r3, r3, #1, #1
005b570c: mov r2, #1
005b5710: bl #0x5fdf74
005b5714: mov r0, r4
005b5718: pop {r4, r5, r6, r7, r8, sb, sl, pc}
005b571c: ldrb r3, [r0, #0x3f]
005b5720: add r1, r5, #0x54
005b5724: mov r0, #1
005b5728: bic r3, r3, #0x10
005b572c: strb r3, [r5, #0x3f]
005b5730: bl #0x30e934
005b5734: ldr r1, [r5, #0x54]
005b5738: cmp r1, #0
005b573c: beq #0x5b5850
005b5740: ldr sl, [r5, #0x34]
005b5744: ldr r3, [sl, #0x268]
005b5748: ldr r2, [r7, r3, lsl #2]
005b574c: cmp r2, r5
005b5750: beq #0x5b577c
005b5754: ldr r4, [sl, #0x4c]
005b5758: sub r4, r4, #1
005b575c: cmp r3, r4
005b5760: beq #0x5b5774
005b5764: add r0, r4, #0x8400
005b5768: add r0, r0, #0xc0
005b576c: bl #0x30e1e4
005b5770: str r4, [sl, #0x268]
005b5774: str r5, [r7, r4, lsl #2]
005b5778: ldr r1, [r5, #0x54]
005b577c: ldr r3, [pc, #0x184]
005b5780: ldr r2, [r5, #0x38]
005b5784: add r3, pc, r3
005b5788: add r3, r3, #0xa4
005b578c: and r2, r2, #3
005b5790: ldr r0, [r3, r2, lsl #2]
005b5794: bl #0x30e7c0
005b5798: ldrb r3, [r5, #0x3e]
005b579c: ldr r2, [r5, #0x38]
005b57a0: cmp r3, #1
005b57a4: bls #0x5b581c
005b57a8: ldrb r3, [r5, #0x3f]
005b57ac: tst r3, #2
005b57b0: mov r1, r3
005b57b4: bne #0x5b5888
005b57b8: ubfx r6, r2, #0xc, #3
005b57bc: cmp r6, #1
005b57c0: ble #0x5b58ac
005b57c4: orr r3, r3, #8
005b57c8: strb r3, [r5, #0x3f]
005b57cc: mov r0, r5
005b57d0: mov r1, #1
005b57d4: bl #0x5b044c
005b57d8: cmp r6, #2
005b57dc: mov r4, r0
005b57e0: beq #0x5b56c8
005b57e4: ldr r3, [r5, #0x38]
005b57e8: ubfx r2, r3, #0xc, #3
005b57ec: cmp r6, r2
005b57f0: beq #0x5b56c8
005b57f4: ldrb r2, [r5, #0x3e]
005b57f8: cmp r2, #1
005b57fc: bls #0x5b58e8
005b5800: ldrh r2, [r5, #0x40]
005b5804: bic r3, r3, #0x7000
005b5808: orr r6, r3, r6, lsl #12
005b580c: orr r2, r2, #4
005b5810: str r6, [r5, #0x38]
005b5814: strh r2, [r5, #0x40]
005b5818: b #0x5b56c8
005b581c: ldrb r1, [r5, #0x3f]
005b5820: orr r1, r1, #8
005b5824: strb r1, [r5, #0x3f]
005b5828: mov r0, r5
005b582c: mov r1, #1
005b5830: bl #0x5b044c
005b5834: mov r4, r0
005b5838: b #0x5b56c8
005b583c: mov r0, r5
005b5840: bl #0x5b044c
005b5844: ldrb r3, [r5, #0x3f]
005b5848: mov r4, r0
005b584c: b #0x5b56cc
005b5850: ldrb r3, [r5, #0x3f]
005b5854: mov r4, r1
005b5858: orr r3, r3, #0x10
005b585c: strb r3, [r5, #0x3f]
005b5860: b #0x5b56cc
005b5864: ldr r3, [r5]
005b5868: mov r0, r5
005b586c: mov lr, pc
005b5870: ldr pc, [r3, #0x10]
005b5874: ldrb r3, [r5, #0x3f]
005b5878: mov r0, r4
005b587c: orr r3, r3, #0x10
005b5880: strb r3, [r5, #0x3f]
005b5884: pop {r4, r5, r6, r7, r8, sb, sl, pc}
005b5888: ldr ip, [pc, #0x7c]
005b588c: ubfx r0, r2, #4, #6
005b5890: mov lr, #0x28
005b5894: ldr ip, [r6, ip]
005b5898: mul r0, lr, r0
005b589c: ldr r0, [ip, r0]
005b58a0: tst r0, #8
005b58a4: bne #0x5b5820
005b58a8: b #0x5b57b8
005b58ac: cmp r6, #2
005b58b0: beq #0x5b58f4
005b58b4: ldrh r0, [r5, #0x40]
005b58b8: bic r2, r2, #0x7000
005b58bc: orr r1, r2, #0x2000
005b58c0: orr r3, r3, #8
005b58c4: orr r2, r0, #4
005b58c8: str r1, [r5, #0x38]
005b58cc: strh r2, [r5, #0x40]
005b58d0: strb r3, [r5, #0x3f]
005b58d4: mov r0, r5
005b58d8: mov r1, #1
005b58dc: bl #0x5b044c
005b58e0: mov r4, r0
005b58e4: b #0x5b57e4
005b58e8: cmp r6, #1
005b58ec: bgt #0x5b56c8
005b58f0: b #0x5b5800
005b58f4: orr r3, r3, #8
005b58f8: strb r3, [r5, #0x3f]
005b58fc: b #0x5b5828
005b5900: eorseq pc, sp, r8, asr r4
005b5904: eorseq sl, r2, r8, lsr #19
005b5908: ldrhteq sl, [r2], -r0
005b590c: andeq r1, r0, r4, lsr pc

_ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE8CTexture10unbindImplEv
005b28dc: push {r4, r5, r6, r7, r8, lr}
005b28e0: ldr r3, [r0, #0x34]
005b28e4: ldr r7, [r0, #0x38]
005b28e8: mov r5, r0
005b28ec: ldr r6, [r3, #0x4c]
005b28f0: and r7, r7, #3
005b28f4: add r3, r3, #0x420
005b28f8: cmp r6, #0
005b28fc: add r7, r3, r7, lsl #5
005b2900: beq #0x5b2920
005b2904: mov r4, #0
005b2908: ldr r3, [r7, r4, lsl #2]
005b290c: cmp r3, r5
005b2910: beq #0x5b29bc
005b2914: add r4, r4, #1
005b2918: cmp r4, r6
005b291c: bne #0x5b2908
005b2920: mov r0, #1
005b2924: add r1, r5, #0x54
005b2928: bl #0x30e8ec
005b292c: ldrh r0, [r5, #0x40]
005b2930: ldrb r3, [r5, #0x3f]
005b2934: mov r2, #0
005b2938: bic r0, r0, #2
005b293c: and r3, r3, #0xe7
005b2940: orr r0, r0, #0x1fc0
005b2944: orr r0, r0, #0x3c
005b2948: tst r3, #2
005b294c: str r2, [r5, #0x54]
005b2950: strb r3, [r5, #0x3f]
005b2954: strh r0, [r5, #0x40]
005b2958: beq #0x5b29d8
005b295c: ldr r7, [r5, #0x38]
005b2960: ldrb r1, [r5, #0x3e]
005b2964: orr r0, r0, #1
005b2968: and r7, r7, #3
005b296c: cmp r7, #2
005b2970: strh r0, [r5, #0x40]
005b2974: moveq r7, #6
005b2978: movne r7, #1
005b297c: mov r3, r2
005b2980: mov r6, #1
005b2984: ldr ip, [r5, #0x30]
005b2988: add r1, r1, #1
005b298c: lsr r0, r3, #5
005b2990: add r1, ip, r1, lsl #2
005b2994: ldr ip, [r1, r0, lsl #2]
005b2998: and r4, r3, #0x1f
005b299c: add r2, r2, #1
005b29a0: orr ip, ip, r6, lsl r4
005b29a4: str ip, [r1, r0, lsl #2]
005b29a8: ldrb r1, [r5, #0x3e]
005b29ac: cmp r2, r7
005b29b0: add r3, r3, r1
005b29b4: blt #0x5b2984
005b29b8: pop {r4, r5, r6, r7, r8, pc}
005b29bc: ldr r3, [r5, #0x38]
005b29c0: mov r1, r4
005b29c4: ldr r0, [r5, #0x34]
005b29c8: and r3, r3, #3
005b29cc: mov r2, #0
005b29d0: bl #0x5b26f0
005b29d4: b #0x5b2914
005b29d8: ldr ip, [r5, #0x38]
005b29dc: ldrb r2, [r5, #0x3e]
005b29e0: ldr r3, [r5, #0x30]
005b29e4: and ip, ip, #3
005b29e8: cmp ip, #2
005b29ec: moveq ip, #6
005b29f0: movne ip, #1
005b29f4: mul ip, r2, ip
005b29f8: add r1, r2, #1
005b29fc: add r2, ip, #0x1f
005b2a00: lsr r2, r2, #5
005b2a04: add r3, r3, r1, lsl #2
005b2a08: add r2, r3, r2, lsl #2
005b2a0c: orr r0, r0, #1
005b2a10: cmp r3, r2
005b2a14: strh r0, [r5, #0x40]
005b2a18: beq #0x5b2a2c
005b2a1c: mvn r1, #0
005b2a20: str r1, [r3], #4
005b2a24: cmp r2, r3
005b2a28: bne #0x5b2a20
005b2a2c: pop {r4, r5, r6, r7, r8, pc}

_ZN6glitch5video19CCommonGLDriverBase12CTextureBase7mapImplEhNS0_23E_TEXTURE_CUBE_MAP_FACEEh
006ddd44: push {r4, r5, r6, r7, r8, lr}
006ddd48: mov r4, r0
006ddd4c: ldr r0, [r0, #0x2c]
006ddd50: and r1, r1, #1
006ddd54: mov r5, r2
006ddd58: cmp r0, #0
006ddd5c: mov r6, r3
006ddd60: orr r7, r1, #4
006ddd64: beq #0x6dde0c
006ddd68: ldrb r2, [r4, #0x3e]
006ddd6c: ldrh ip, [r4, #0x40]
006ddd70: ldr r0, [r4, #0x30]
006ddd74: mla r1, r2, r5, r3
006ddd78: orr ip, ip, #1
006ddd7c: add r3, r2, #1
006ddd80: strh ip, [r4, #0x40]
006ddd84: add r3, r0, r3, lsl #2
006ddd88: lsr r2, r1, #5
006ddd8c: ldr r0, [r3, r2, lsl #2]
006ddd90: and r1, r1, #0x1f
006ddd94: mov ip, #1
006ddd98: orr r1, r0, ip, lsl r1
006ddd9c: str r1, [r3, r2, lsl #2]
006ddda0: ldr r3, [r4, #0x2c]
006ddda4: cmp r3, #0
006ddda8: beq #0x6dde0c
006dddac: ldrb r2, [r4, #0x3f]
006dddb0: ldr r1, [r4, #0x30]
006dddb4: lsl r7, r7, #5
006dddb8: tst r2, #2
006dddbc: ldrbeq r0, [r4, #0x3e]
006dddc0: ldrne r0, [r1]
006dddc4: ldrne r1, [r1, #4]
006dddc8: ldreq r0, [r1, r0, lsl #2]
006dddcc: ldreq ip, [r1, r6, lsl #2]
006dddd0: rsbne r0, r0, r1
006dddd4: addeq r0, r0, #0x7f
006dddd8: biceq r0, r0, #0x7f
006ddddc: mulne r0, r0, r5
006ddde0: mlaeq r0, r0, r5, ip
006ddde4: cmp r6, #0
006ddde8: cmpeq r5, #0
006dddec: orr r7, r7, #1
006dddf0: orr r5, r5, r6, lsl #3
006dddf4: orreq r2, r2, #0x40
006dddf8: add r0, r3, r0
006dddfc: strb r7, [r4, #0x42]
006dde00: strb r5, [r4, #0x43]
006dde04: strbeq r2, [r4, #0x3f]
006dde08: pop {r4, r5, r6, r7, r8, pc}
006dde0c: ldr r3, [r4, #0x38]
006dde10: ldrb r2, [r4, #0x3f]
006dde14: and r3, r3, #3
006dde18: cmp r3, #2
006dde1c: moveq r3, #5
006dde20: movne r3, #0
006dde24: tst r2, #2
006dde28: ldrne r1, [r4, #0x30]
006dde2c: ldreq r2, [r4, #0x30]
006dde30: ldrbeq r1, [r4, #0x3e]
006dde34: ldrne r2, [r1]
006dde38: ldrne r0, [r1, #4]
006dde3c: ldreq r2, [r2, r1, lsl #2]
006dde40: mov r1, #0
006dde44: rsbne r2, r2, r0
006dde48: add r0, r2, #0x7f
006dde4c: bic r0, r0, #0x7f
006dde50: mla r0, r0, r3, r2
006dde54: bl #0x5341a8
006dde58: ldrb r3, [r4, #0x3f]
006dde5c: mov r1, r0
006dde60: mov r2, #1
006dde64: ubfx r3, r3, #1, #1
006dde68: mov r0, r4
006dde6c: bl #0x5fdf74
006dde70: ldr r0, [pc, #0x18]
006dde74: ldr r1, [pc, #0x18]
006dde78: mov r2, #2
006dde7c: add r0, pc, r0
006dde80: add r1, pc, r1
006dde84: bl #0x60ace8
006dde88: ldr r3, [r4, #0x2c]
006dde8c: b #0x6dddac
006dde90: eoreq lr, r0, r4, lsr #32
006dde94: eoreq lr, r0, r8, lsr r0

_ZNK6glitch5video19CCommonGLDriverBase12CTextureBase9unmapImplEv
006dcbf4: bx lr

_ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE8CTexture19generateMipmapsImplEv
005b280c: push {r4, r5, r6, lr}
005b2810: mov r4, r0
005b2814: ldr r0, [r0, #0x34]
005b2818: ldr r3, [r4, #0x38]
005b281c: mov r2, r4
005b2820: ldr r5, [r0, #0x4c]
005b2824: and r3, r3, #3
005b2828: sub r5, r5, #1
005b282c: mov r1, r5
005b2830: bl #0x5b26f0
005b2834: ldr r6, [r4, #0x34]
005b2838: ldr r3, [r6, #0x268]
005b283c: cmp r5, r3
005b2840: beq #0x5b2854
005b2844: add r0, r5, #0x8400
005b2848: add r0, r0, #0xc0
005b284c: bl #0x30e1e4
005b2850: str r5, [r6, #0x268]
005b2854: ldr r3, [r4, #0x38]
005b2858: ldr r5, [pc, #0x78]
005b285c: ubfx r2, r3, #0xc, #3
005b2860: add r5, pc, r5
005b2864: cmp r2, #1
005b2868: and r3, r3, #3
005b286c: add r2, r5, #0xa4
005b2870: ldr r6, [r2, r3, lsl #2]
005b2874: ble #0x5b28a0
005b2878: mov r0, r6
005b287c: bl #0x30e370
005b2880: ldrb r3, [r4, #0x3f]
005b2884: tst r3, #2
005b2888: bne #0x5b289c
005b288c: ldrh r3, [r4, #0x40]
005b2890: orr r3, r3, #2
005b2894: strh r3, [r4, #0x40]
005b2898: pop {r4, r5, r6, pc}
005b289c: pop {r4, r5, r6, pc}
005b28a0: movw r1, #0x2801
005b28a4: mov r2, #0x2700
005b28a8: mov r0, r6
005b28ac: bl #0x30e910
005b28b0: mov r0, r6
005b28b4: bl #0x30e370
005b28b8: ldr r3, [r4, #0x38]
005b28bc: add r5, r5, #0xb4
005b28c0: mov r0, r6
005b28c4: ubfx r3, r3, #0xc, #3
005b28c8: ldr r2, [r5, r3, lsl #2]
005b28cc: movw r1, #0x2801
005b28d0: bl #0x30e910
005b28d4: b #0x5b2880
005b28d8: ldrsbteq sp, [r2], -r4
