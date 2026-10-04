
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

# _ZNK7gameswf11ref_counted7add_refEv
00759c64: ldr      r3, [r0, #4]
00759c68: add      r3, r3, #1
00759c6c: str      r3, [r0, #4]
00759c70: bx       lr

# _ZN7gameswf11ref_counted8drop_refEv
0075a240: ldr      r2, [r0, #4]
0075a244: sub      r2, r2, #1
0075a248: cmp      r2, #0
0075a24c: str      r2, [r0, #4]
0075a250: bxne     lr
0075a254: b        #0x75a214
