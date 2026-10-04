
# _ZN7gameswf15sprite_instance26execute_frame_tags_reverseEi
0077f390: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0077f394: subs     r6, r0, #0
0077f398: mov      r8, r1
0077f39c: beq      #0x77f3a4
0077f3a0: bl       #0x759c64
0077f3a4: ldr      r3, [r6, #0xa0]
0077f3a8: mov      r1, r8
0077f3ac: mov      r0, r3
0077f3b0: ldr      r3, [r3]
0077f3b4: mov      lr, pc
0077f3b8: ldr      pc, [r3, #0x50]
0077f3bc: ldr      sl, [r0, #4]
0077f3c0: mov      r7, r0
0077f3c4: subs     r5, sl, #1
0077f3c8: bmi      #0x77f404
0077f3cc: lsl      r5, r5, #2
0077f3d0: mov      r4, #0
0077f3d4: ldr      r3, [r7]
0077f3d8: add      r4, r4, #1
0077f3dc: mov      r1, r6
0077f3e0: ldr      r3, [r3, r5]
0077f3e4: mov      r2, r8
0077f3e8: sub      r5, r5, #4
0077f3ec: mov      r0, r3
0077f3f0: ldr      r3, [r3]
0077f3f4: mov      lr, pc
0077f3f8: ldr      pc, [r3, #0x10]
0077f3fc: cmp      r4, sl
0077f400: bne      #0x77f3d4
0077f404: cmp      r6, #0
0077f408: beq      #0x77f418
0077f40c: mov      r0, r6
0077f410: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
0077f414: b        #0x75a240
0077f418: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN7gameswf9character19notify_need_advanceEv
007750e8: push     {r4, lr}
007750ec: mov      r4, r0
007750f0: ldr      r3, [r4, #0x40]
007750f4: mov      r1, #1
007750f8: strb     r1, [r4, #0x9d]
007750fc: cmp      r3, #0
00775100: beq      #0x775128
00775104: ldr      r0, [r4, #0x3c]
00775108: ldrb     r2, [r0, #4]
0077510c: cmp      r2, #0
00775110: beq      #0x77512c
00775114: mov      r4, r3
00775118: ldr      r3, [r4, #0x40]
0077511c: strb     r1, [r4, #0x9d]
00775120: cmp      r3, #0
00775124: bne      #0x775104
00775128: pop      {r4, pc}
0077512c: ldr      r1, [r0]
00775130: sub      r1, r1, #1
00775134: cmp      r1, #0
00775138: str      r1, [r0]
0077513c: bne      #0x775144
00775140: bl       #0x752b38
00775144: mov      r3, #0
00775148: str      r3, [r4, #0x40]
0077514c: str      r3, [r4, #0x3c]
00775150: pop      {r4, pc}

# _ZN7gameswf15sprite_instance14set_play_stateENS_9character10play_stateE
0077fe10: push     {r4, r5, r6, lr}
0077fe14: mov      r4, r0
0077fe18: mov      r5, r1
0077fe1c: bl       #0x77cba0
0077fe20: subs     ip, r0, #0
0077fe24: beq      #0x77fe50
0077fe28: ldr      r3, [r4, #0xa0]
0077fe2c: ldr      r1, [r3, #0x20]
0077fe30: cmp      r1, #0
0077fe34: blt      #0x77fe50
0077fe38: ldrsb    r2, [r4, #0xe6]
0077fe3c: ldr      r3, [ip]
0077fe40: rsbs     r2, r2, #1
0077fe44: movlo    r2, #0
0077fe48: mov      lr, pc
0077fe4c: ldr      pc, [r3, #0x38]
0077fe50: mov      r0, r4
0077fe54: strb     r5, [r4, #0xe6]
0077fe58: pop      {r4, r5, r6, lr}
0077fe5c: b        #0x7750e8

# _ZN7gameswf15sprite_instance10goto_frameEi
00781a3c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00781a40: ldr      r3, [r0, #0xa0]
00781a44: mov      r4, r0
00781a48: mov      r7, r1
00781a4c: mov      r0, r3
00781a50: ldr      r3, [r3]
00781a54: mov      lr, pc
00781a58: ldr      pc, [r3, #0x38]
00781a5c: cmp      r0, r7
00781a60: bgt      #0x781a74
00781a64: mov      r3, #1
00781a68: strb     r3, [r4, #0xe6]
00781a6c: mov      r0, #0
00781a70: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00781a74: cmp      r7, #0
00781a78: blt      #0x781a64
00781a7c: ldrsh    r3, [r4, #0xe4]
00781a80: cmp      r3, r7
00781a84: beq      #0x781a64
00781a88: ldr      sl, [r4, #0xc0]
00781a8c: add      r6, r4, #0xcc
00781a90: add      r5, r4, #0xbc
00781a94: cmp      sl, #0
00781a98: ldr      r8, [r4, #0xd0]
00781a9c: bne      #0x781c74
00781aa0: cmp      sl, r8
00781aa4: ble      #0x781ac8
00781aa8: lsl      r3, r8, #2
00781aac: mov      r1, #0
00781ab0: ldr      r2, [r6]
00781ab4: add      r8, r8, #1
00781ab8: cmp      r8, sl
00781abc: str      r1, [r2, r3]
00781ac0: add      r3, r3, #4
00781ac4: bne      #0x781ab0
00781ac8: cmp      sl, #0
00781acc: str      sl, [r4, #0xd0]
00781ad0: ble      #0x781af8
00781ad4: mov      r3, #0
00781ad8: ldr      r1, [r5]
00781adc: ldr      r2, [r6]
00781ae0: ldr      r1, [r1, r3, lsl #2]
00781ae4: str      r1, [r2, r3, lsl #2]
00781ae8: ldr      r2, [r6, #4]
00781aec: add      r3, r3, #1
00781af0: cmp      r3, r2
00781af4: blt      #0x781ad8
00781af8: ldr      r3, [r4, #0xc0]
00781afc: cmp      r3, #0
00781b00: ble      #0x781ccc
00781b04: ldrsh    sb, [r4, #0xe4]
00781b08: mov      r8, #0
00781b0c: str      r8, [r4, #0xc0]
00781b10: cmp      r7, sb
00781b14: blt      #0x781b84
00781b18: ble      #0x781bc8
00781b1c: add      sl, sb, #1
00781b20: cmp      r7, sl
00781b24: ble      #0x781bac
00781b28: mvn      sb, sb
00781b2c: add      sb, sb, r7
00781b30: add      r1, sl, r8
00781b34: ldr      r3, [r4]
00781b38: add      r8, r8, #1
00781b3c: mov      r0, r4
00781b40: mov      r2, #1
00781b44: mov      lr, pc
00781b48: ldr      pc, [r3, #0xc8]
00781b4c: cmp      r8, sb
00781b50: bne      #0x781b30
00781b54: ldr      r3, [r4, #0xc0]
00781b58: cmp      r3, #0
00781b5c: bgt      #0x781bac
00781b60: bge      #0x781bac
00781b64: lsl      r2, r3, #2
00781b68: mov      r0, #0
00781b6c: ldr      r1, [r5]
00781b70: adds     r3, r3, #1
00781b74: str      r0, [r1, r2]
00781b78: add      r2, r2, #4
00781b7c: bne      #0x781b6c
00781b80: b        #0x781bac
00781b84: rsb      sl, r7, sb
00781b88: rsb      r1, r8, sb
00781b8c: mov      r0, r4
00781b90: add      r8, r8, #1
00781b94: bl       #0x77f390
00781b98: cmp      r8, sl
00781b9c: bne      #0x781b88
00781ba0: ldr      r3, [r4, #0xc0]
00781ba4: cmp      r3, #0
00781ba8: ble      #0x781d00
00781bac: mov      r2, #0
00781bb0: str      r2, [r4, #0xc0]
00781bb4: ldr      r3, [r4]
00781bb8: mov      r0, r4
00781bbc: mov      r1, r7
00781bc0: mov      lr, pc
00781bc4: ldr      pc, [r3, #0xc8]
00781bc8: ldr      r8, [r4, #0xc0]
00781bcc: mov      r3, #1
00781bd0: strh     r7, [r4, #0xe4]
00781bd4: cmp      r8, #0
00781bd8: strb     r3, [r4, #0xe6]
00781bdc: ldr      sl, [r4, #0xbc]
00781be0: ble      #0x781c90
00781be4: ldr      sb, [r4, #0xd0]
00781be8: adds     r7, sb, r8
00781bec: beq      #0x781bfc
00781bf0: ldr      r3, [r4, #0xd4]
00781bf4: cmp      r7, r3
00781bf8: bgt      #0x781cf0
00781bfc: cmp      sb, r7
00781c00: lslge    r2, sb, #2
00781c04: bge      #0x781c2c
00781c08: lsl      r2, sb, #2
00781c0c: mov      r3, r2
00781c10: mov      r0, #0
00781c14: ldr      r1, [r6]
00781c18: add      sb, sb, #1
00781c1c: cmp      sb, r7
00781c20: str      r0, [r1, r3]
00781c24: add      r3, r3, #4
00781c28: bne      #0x781c14
00781c2c: str      r7, [r4, #0xd0]
00781c30: mov      r3, #0
00781c34: ldr      r0, [sl, r3, lsl #2]
00781c38: ldr      r1, [r4, #0xcc]
00781c3c: add      r3, r3, #1
00781c40: cmp      r3, r8
00781c44: str      r0, [r1, r2]
00781c48: add      r2, r2, #4
00781c4c: bne      #0x781c34
00781c50: ldr      r8, [r4, #0xc0]
00781c54: cmp      r8, #0
00781c58: ble      #0x781c90
00781c5c: mov      r3, #0
00781c60: mov      r0, r4
00781c64: str      r3, [r4, #0xc0]
00781c68: bl       #0x7750e8
00781c6c: mov      r0, #1
00781c70: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00781c74: ldr      r3, [r4, #0xd4]
00781c78: cmp      sl, r3
00781c7c: ble      #0x781aa0
00781c80: mov      r0, r6
00781c84: add      r1, sl, sl, asr #1
00781c88: bl       #0x77e7f8
00781c8c: b        #0x781aa0
00781c90: cmp      r8, #0
00781c94: bge      #0x781c5c
00781c98: lsl      r3, r8, #2
00781c9c: mov      r1, #0
00781ca0: ldr      r2, [r5]
00781ca4: adds     r8, r8, #1
00781ca8: str      r1, [r2, r3]
00781cac: add      r3, r3, #4
00781cb0: bne      #0x781ca0
00781cb4: mov      r3, #0
00781cb8: mov      r0, r4
00781cbc: str      r3, [r4, #0xc0]
00781cc0: bl       #0x7750e8
00781cc4: mov      r0, #1
00781cc8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00781ccc: bge      #0x781b04
00781cd0: lsl      r2, r3, #2
00781cd4: mov      r0, #0
00781cd8: ldr      r1, [r5]
00781cdc: adds     r3, r3, #1
00781ce0: str      r0, [r1, r2]
00781ce4: add      r2, r2, #4
00781ce8: bne      #0x781cd8
00781cec: b        #0x781b04
00781cf0: mov      r0, r6
00781cf4: add      r1, r7, r7, asr #1
00781cf8: bl       #0x77e7f8
00781cfc: b        #0x781bfc
00781d00: bge      #0x781bac
00781d04: lsl      r2, r3, #2
00781d08: mov      r0, #0
00781d0c: ldr      r1, [r5]
00781d10: adds     r3, r3, #1
00781d14: str      r0, [r1, r2]
00781d18: add      r2, r2, #4
00781d1c: bne      #0x781d0c
00781d20: b        #0x781bac

# _ZN7gameswf5arrayIPNS_13action_bufferEE7reserveEi
0077e7f8: push     {r4, lr}
0077e7fc: ldrb     r3, [r0, #0xc]
0077e800: mov      r4, r0
0077e804: cmp      r3, #0
0077e808: bne      #0x77e84c
0077e80c: cmp      r1, #0
0077e810: ldr      r2, [r0, #8]
0077e814: str      r1, [r0, #8]
0077e818: bne      #0x77e850
0077e81c: ldr      r0, [r0]
0077e820: cmp      r0, #0
0077e824: beq      #0x77e830
0077e828: lsl      r1, r2, #2
0077e82c: bl       #0x752b38
0077e830: mov      r3, #0
0077e834: str      r3, [r4]
0077e838: pop      {r4, pc}
0077e83c: lsl      r0, r1, #2
0077e840: mov      r1, ip
0077e844: bl       #0x752b9c
0077e848: str      r0, [r4]
0077e84c: pop      {r4, pc}
0077e850: ldr      ip, [r0]
0077e854: cmp      ip, #0
0077e858: beq      #0x77e83c
0077e85c: mov      r0, ip
0077e860: lsl      r1, r1, #2
0077e864: lsl      r2, r2, #2
0077e868: bl       #0x752bac
0077e86c: str      r0, [r4]
0077e870: pop      {r4, pc}

# _ZN7gameswf17get_sound_handlerEv
0077cba0: push     {r4, lr}
0077cba4: ldr      r4, [pc, #0x34]
0077cba8: add      r4, pc, r4
0077cbac: ldr      r3, [r4]
0077cbb0: cmp      r3, #0
0077cbb4: bne      #0x77cbc0
0077cbb8: mov      r0, #0
0077cbbc: pop      {r4, pc}
0077cbc0: mov      r0, r3
0077cbc4: ldr      r3, [r3]
0077cbc8: mov      lr, pc
0077cbcc: ldr      pc, [r3, #0x34]
0077cbd0: cmp      r0, #0
0077cbd4: beq      #0x77cbb8
0077cbd8: ldr      r0, [r4]
0077cbdc: pop      {r4, pc}
0077cbe0: eoreq    pc, r7, r4, lsl #28
