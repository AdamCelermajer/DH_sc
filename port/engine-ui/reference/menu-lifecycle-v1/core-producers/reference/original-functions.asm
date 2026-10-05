
# _ZN8RenderFX10SetContextEPN7gameswf9characterE
007a7ee8: str      r1, [r0, #0x40]
007a7eec: bx       lr

# _ZN7gameswf11call_methodEPNS_14as_environmentEPNS_9as_objectEPKcPKNS_8as_valueEi
007bbbfc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007bbc00: ldr      r5, [pc, #0x1e0]
007bbc04: ldr      ip, [pc, #0x1e0]
007bbc08: sub      sp, sp, #0x74
007bbc0c: add      r5, pc, r5
007bbc10: str      ip, [sp, #0x18]
007bbc14: ldr      ip, [r5, ip]
007bbc18: mov      r4, r1
007bbc1c: str      r3, [sp, #0x14]
007bbc20: ldr      ip, [ip]
007bbc24: ldr      r6, [sp, #0x9c]
007bbc28: mov      r7, r0
007bbc2c: str      ip, [sp, #0x6c]
007bbc30: ldr      lr, [r4, #4]
007bbc34: subs     r1, r6, #1
007bbc38: mov      sb, r2
007bbc3c: ldr      r3, [sp, #0x98]
007bbc40: str      lr, [sp, #0x1c]
007bbc44: bmi      #0x7bbc70
007bbc48: mov      sl, #0xc
007bbc4c: mla      sl, sl, r1, r3
007bbc50: mov      r8, #0
007bbc54: mov      r1, sl
007bbc58: add      r8, r8, #1
007bbc5c: mov      r0, r4
007bbc60: bl       #0x769098
007bbc64: cmp      r8, r6
007bbc68: sub      sl, sl, #0xc
007bbc6c: bne      #0x7bbc54
007bbc70: add      fp, sp, #0x58
007bbc74: mov      r6, #0
007bbc78: ldr      r1, [sp, #0x14]
007bbc7c: mov      r0, fp
007bbc80: add      r8, sp, #0x4c
007bbc84: add      sl, sp, #0x24
007bbc88: str      r6, [sp, #0x24]
007bbc8c: str      r6, [sp, #0x28]
007bbc90: str      r6, [sp, #0x2c]
007bbc94: strb     r6, [sp, #0x30]
007bbc98: bl       #0x413a7c
007bbc9c: mov      r3, sl
007bbca0: mov      r2, fp
007bbca4: mov      r0, r8
007bbca8: mov      r1, r4
007bbcac: str      r6, [sp]
007bbcb0: bl       #0x7ce120
007bbcb4: ldrsb    r3, [sp, #0x58]
007bbcb8: cmn      r3, #1
007bbcbc: beq      #0x7bbdd4
007bbcc0: ldr      ip, [r4, #4]
007bbcc4: ldr      r2, [sp, #0x1c]
007bbcc8: mov      r3, #0
007bbccc: strb     r3, [sp, #0x34]
007bbcd0: cmp      sb, #0
007bbcd4: mov      r3, #5
007bbcd8: strb     r3, [sp, #0x35]
007bbcdc: rsb      fp, r2, ip
007bbce0: str      sb, [sp, #0x38]
007bbce4: beq      #0x7bbcf4
007bbce8: mov      r0, sb
007bbcec: bl       #0x759c64
007bbcf0: ldr      ip, [r4, #4]
007bbcf4: sub      ip, ip, #1
007bbcf8: str      ip, [sp, #4]
007bbcfc: ldr      ip, [sp, #0x14]
007bbd00: add      sb, sp, #0x34
007bbd04: add      r6, sp, #0x40
007bbd08: mov      r3, sb
007bbd0c: mov      r1, r8
007bbd10: mov      r2, r4
007bbd14: mov      r0, r6
007bbd18: str      ip, [sp, #8]
007bbd1c: str      fp, [sp]
007bbd20: bl       #0x7ba904
007bbd24: mov      r0, sb
007bbd28: bl       #0x797124
007bbd2c: ldr      r1, [r4, #4]
007bbd30: mov      r0, r4
007bbd34: rsb      r1, fp, r1
007bbd38: bl       #0x77eba4
007bbd3c: ldrsb    r3, [sp, #0x41]
007bbd40: cmp      r3, #0
007bbd44: bne      #0x7bbdbc
007bbd48: ldr      r2, [r7, #0x10]
007bbd4c: mvn      r1, #0
007bbd50: mov      r0, #1
007bbd54: bfi      r2, r1, #0, #0x18
007bbd58: lsr      r1, r2, #0x18
007bbd5c: bfi      r1, r3, #0, #1
007bbd60: str      r2, [r7, #0x10]
007bbd64: strb     r0, [r7]
007bbd68: strb     r1, [r7, #0x13]
007bbd6c: strb     r3, [r7, #1]
007bbd70: mov      r0, r6
007bbd74: bl       #0x797124
007bbd78: mov      r0, r8
007bbd7c: bl       #0x797124
007bbd80: mov      r0, sl
007bbd84: mov      r1, #0
007bbd88: bl       #0x7baaac
007bbd8c: mov      r0, sl
007bbd90: mov      r1, #0
007bbd94: bl       #0x75a314
007bbd98: ldr      r2, [sp, #0x18]
007bbd9c: mov      r0, r7
007bbda0: ldr      r3, [r5, r2]
007bbda4: ldr      r2, [sp, #0x6c]
007bbda8: ldr      r3, [r3]
007bbdac: cmp      r2, r3
007bbdb0: bne      #0x7bbde4
007bbdb4: add      sp, sp, #0x74
007bbdb8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007bbdbc: mov      r0, r6
007bbdc0: bl       #0x420a84
007bbdc4: mov      r1, r0
007bbdc8: mov      r0, r7
007bbdcc: bl       #0x75302c
007bbdd0: b        #0x7bbd70
007bbdd4: ldr      r0, [sp, #0x64]
007bbdd8: ldr      r1, [sp, #0x60]
007bbddc: bl       #0x752b38
007bbde0: b        #0x7bbcc0
007bbde4: bl       #0x30e310
007bbde8: andseq   r8, sp, r4, lsl #29
007bbdec: andeq    r4, r0, ip, lsr #1

# _ZNK7gameswf8weak_ptrINS_9characterEE7get_ptrEv
00438224: push     {r4, lr}
00438228: mov      r4, r0
0043822c: ldr      r0, [r0, #4]
00438230: cmp      r0, #0
00438234: beq      #0x438248
00438238: ldr      r3, [r4]
0043823c: ldrb     r2, [r3, #4]
00438240: cmp      r2, #0
00438244: beq      #0x43824c
00438248: pop      {r4, pc}
0043824c: ldr      r1, [r3]
00438250: sub      r1, r1, #1
00438254: cmp      r1, #0
00438258: str      r1, [r3]
0043825c: bne      #0x438268
00438260: mov      r0, r3
00438264: bl       #0x752b38
00438268: mov      r0, #0
0043826c: str      r0, [r4, #4]
00438270: str      r0, [r4]
00438274: pop      {r4, pc}

# _ZN8RenderFX23SetTextBufferingEnabledEb
007a7cb4: ldr      r3, [r0, #0x3c]
007a7cb8: strb     r1, [r3, #0x85]
007a7cbc: bx       lr

# _ZN8RenderFX17CollectCharactersEPN7gameswf9characterEPKci
007a8acc: push     {r4, r5, r6, r7, r8, lr}
007a8ad0: tst      r3, #1
007a8ad4: ldrbne   r4, [r1, #0x9b]
007a8ad8: mov      r6, r3
007a8adc: mov      r8, r0
007a8ae0: ldr      r3, [r1]
007a8ae4: mov      r0, r1
007a8ae8: mov      r5, r1
007a8aec: mov      r1, #2
007a8af0: moveq    r4, #1
007a8af4: mov      r7, r2
007a8af8: mov      lr, pc
007a8afc: ldr      pc, [r3, #8]
007a8b00: cmp      r0, #0
007a8b04: beq      #0x7a8b10
007a8b08: tst      r6, #2
007a8b0c: bne      #0x7a8bc4
007a8b10: cmp      r4, #0
007a8b14: beq      #0x7a8bc0
007a8b18: cmp      r7, #0
007a8b1c: beq      #0x7a8b44
007a8b20: ldr      r0, [r5, #0x44]
007a8b24: mov      r1, r7
007a8b28: ldrsb    r3, [r0]
007a8b2c: cmn      r3, #1
007a8b30: addne    r0, r0, #1
007a8b34: ldreq    r0, [r0, #0xc]
007a8b38: bl       #0x30ebd4
007a8b3c: cmp      r0, #0
007a8b40: beq      #0x7a8b6c
007a8b44: tst      r6, #4
007a8b48: bne      #0x7a8bd4
007a8b4c: ldr      r3, [r8, #8]
007a8b50: ldr      r2, [r8, #0xc]
007a8b54: add      r4, r3, #1
007a8b58: cmp      r4, r2
007a8b5c: bgt      #0x7a8bf4
007a8b60: ldr      r2, [r8, #4]
007a8b64: str      r5, [r2, r3, lsl #2]
007a8b68: str      r4, [r8, #8]
007a8b6c: ldr      r3, [r5]
007a8b70: mov      r0, r5
007a8b74: mov      r1, #2
007a8b78: mov      lr, pc
007a8b7c: ldr      pc, [r3, #8]
007a8b80: cmp      r0, #0
007a8b84: beq      #0x7a8bc0
007a8b88: ldr      r3, [r5, #0xac]
007a8b8c: cmp      r3, #0
007a8b90: ble      #0x7a8bc0
007a8b94: mov      r4, #0
007a8b98: ldr      r3, [r5, #0xa8]
007a8b9c: mov      r0, r8
007a8ba0: mov      r2, r7
007a8ba4: ldr      r1, [r3, r4, lsl #2]
007a8ba8: mov      r3, r6
007a8bac: bl       #0x7a8acc
007a8bb0: ldr      r3, [r5, #0xac]
007a8bb4: add      r4, r4, #1
007a8bb8: cmp      r4, r3
007a8bbc: blt      #0x7a8b98
007a8bc0: pop      {r4, r5, r6, r7, r8, pc}
007a8bc4: ldrb     r3, [r5, #0xea]
007a8bc8: cmp      r3, #0
007a8bcc: bne      #0x7a8b10
007a8bd0: pop      {r4, r5, r6, r7, r8, pc}
007a8bd4: ldr      r2, [r5, #0x44]
007a8bd8: ldrsb    r3, [r2]
007a8bdc: cmn      r3, #1
007a8be0: ldreq    r3, [r2, #4]
007a8be4: sub      r3, r3, #1
007a8be8: cmp      r3, #0
007a8bec: beq      #0x7a8b6c
007a8bf0: b        #0x7a8b4c
007a8bf4: add      r0, r8, #4
007a8bf8: add      r1, r4, r4, asr #1
007a8bfc: bl       #0x413e14
007a8c00: ldr      r3, [r8, #8]
007a8c04: b        #0x7a8b60

# _ZN8RenderFX14GetSearchIndexEv
007add94: push     {r4, lr}
007add98: add      r4, r0, #0x14
007add9c: mov      r1, r0
007adda0: mov      r0, r4
007adda4: bl       #0x7adb14
007adda8: mov      r0, r4
007addac: pop      {r4, pc}

# _ZN12GameSWFUtils23GetAbsoluteBoundingRectEPN7gameswf9characterE
00416a7c: push     {r4, r5, r6, lr}
00416a80: subs     r5, r1, #0
00416a84: sub      sp, sp, #8
00416a88: mov      r4, r0
00416a8c: beq      #0x416b30
00416a90: add      r0, r5, #0x3c
00416a94: bl       #0x386144
00416a98: ldr      r1, [r5, #0x40]
00416a9c: mov      r0, sp
00416aa0: bl       #0x4169e8
00416aa4: ldr      r6, [sp]
00416aa8: ldr      r3, [r5]
00416aac: mov      r0, r5
00416ab0: mov      r1, r4
00416ab4: ldr      r5, [sp, #4]
00416ab8: mov      lr, pc
00416abc: ldr      pc, [r3, #0x12c]
00416ac0: ldr      r1, [r4]
00416ac4: mov      r0, r6
00416ac8: bl       #0x30eba4
00416acc: mov      r1, #0x41000000
00416ad0: add      r1, r1, #0xa00000
00416ad4: bl       #0x30ec94
00416ad8: ldr      r1, [r4, #4]
00416adc: str      r0, [r4]
00416ae0: mov      r0, r6
00416ae4: bl       #0x30eba4
00416ae8: mov      r1, #0x41000000
00416aec: add      r1, r1, #0xa00000
00416af0: bl       #0x30ec94
00416af4: ldr      r1, [r4, #8]
00416af8: str      r0, [r4, #4]
00416afc: mov      r0, r5
00416b00: bl       #0x30eba4
00416b04: mov      r1, #0x41000000
00416b08: add      r1, r1, #0xa00000
00416b0c: bl       #0x30ec94
00416b10: ldr      r1, [r4, #0xc]
00416b14: str      r0, [r4, #8]
00416b18: mov      r0, r5
00416b1c: bl       #0x30eba4
00416b20: mov      r1, #0x41000000
00416b24: add      r1, r1, #0xa00000
00416b28: bl       #0x30ec94
00416b2c: str      r0, [r4, #0xc]
00416b30: mov      r0, r4
00416b34: add      sp, sp, #8
00416b38: pop      {r4, r5, r6, pc}
