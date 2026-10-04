
# _ZN7gameswf9tu_string5eraseEii.clone.1
0078b94c: push     {r4, lr}
0078b950: ldrsb    r3, [r0]
0078b954: mov      r4, r0
0078b958: cmn      r3, #1
0078b95c: ldreq    r3, [r4, #0xc]
0078b960: addne    r3, r4, #1
0078b964: addne    r0, r0, r1
0078b968: addeq    r0, r3, r1
0078b96c: add      r1, r1, #1
0078b970: add      r1, r3, r1
0078b974: addne    r0, r0, #1
0078b978: bl       #0x30e520
0078b97c: ldrsb    r1, [r4]
0078b980: mov      r0, r4
0078b984: cmn      r1, #1
0078b988: ldreq    r1, [r4, #4]
0078b98c: sub      r1, r1, #1
0078b990: sub      r1, r1, #1
0078b994: bl       #0x751d14
0078b998: ldr      r3, [r4, #0x10]
0078b99c: mvn      r2, #0
0078b9a0: bfi      r3, r2, #0, #0x18
0078b9a4: str      r3, [r4, #0x10]
0078b9a8: pop      {r4, pc}

# _ZNK7gameswf9as_object10get_playerEv
00780374: push     {r4, lr}
00780378: mov      r4, r0
0078037c: ldr      r0, [r0, #0x30]
00780380: cmp      r0, #0
00780384: beq      #0x780398
00780388: ldr      r3, [r4, #0x2c]
0078038c: ldrb     r2, [r3, #4]
00780390: cmp      r2, #0
00780394: beq      #0x78039c
00780398: pop      {r4, pc}
0078039c: ldr      r1, [r3]
007803a0: sub      r1, r1, #1
007803a4: cmp      r1, #0
007803a8: str      r1, [r3]
007803ac: bne      #0x7803b8
007803b0: mov      r0, r3
007803b4: bl       #0x752b38
007803b8: mov      r0, #0
007803bc: str      r0, [r4, #0x30]
007803c0: str      r0, [r4, #0x2c]
007803c4: pop      {r4, pc}

# _ZN7gameswf9tu_string6insertEic
0078b0c0: push     {r4, r5, r6, r7, r8, lr}
0078b0c4: ldrsb    r8, [r0]
0078b0c8: mov      r4, r0
0078b0cc: mov      r5, r1
0078b0d0: cmn      r8, #1
0078b0d4: ldreq    r8, [r0, #4]
0078b0d8: mov      r7, r2
0078b0dc: sub      r8, r8, #1
0078b0e0: add      r8, r8, #1
0078b0e4: mov      r1, r8
0078b0e8: bl       #0x751d14
0078b0ec: ldrsb    r3, [r4]
0078b0f0: add      r0, r5, #1
0078b0f4: rsb      r2, r5, r8
0078b0f8: cmn      r3, #1
0078b0fc: ldreq    r6, [r4, #0xc]
0078b100: addne    r6, r4, #1
0078b104: add      r0, r6, r0
0078b108: add      r1, r6, r5
0078b10c: bl       #0x30df38
0078b110: strb     r7, [r6, r5]
0078b114: ldr      r3, [r4, #0x10]
0078b118: mvn      r2, #0
0078b11c: bfi      r3, r2, #0, #0x18
0078b120: str      r3, [r4, #0x10]
0078b124: pop      {r4, r5, r6, r7, r8, pc}

# _ZN7gameswf4root17set_active_entityEPNS_9characterE
00774918: push     {r4, r5, r6, r7, lr}
0077491c: ldr      r4, [r0, #0x88]
00774920: sub      sp, sp, #0x14
00774924: mov      r5, r0
00774928: cmp      r4, #0
0077492c: mov      r7, r1
00774930: beq      #0x7749e8
00774934: mov      r0, r4
00774938: bl       #0x759c64
0077493c: ldr      r0, [r5, #0x88]
00774940: cmp      r4, r0
00774944: beq      #0x7749f4
00774948: cmp      r0, #0
0077494c: addeq    r5, r5, #0x88
00774950: beq      #0x77498c
00774954: ldr      r3, [r0]
00774958: mov      r6, #0
0077495c: mov      r2, #0x15
00774960: ldr      r3, [r3, #0x2c]
00774964: add      r1, sp, #8
00774968: strb     r2, [sp, #8]
0077496c: strb     r6, [sp, #9]
00774970: strh     r6, [sp, #0xa]
00774974: str      r6, [sp, #0xc]
00774978: add      r5, r5, #0x88
0077497c: blx      r3
00774980: mov      r1, r6
00774984: mov      r0, r5
00774988: bl       #0x75518c
0077498c: ldr      r3, [r4]
00774990: mov      r2, #0
00774994: mov      r1, #0x14
00774998: ldr      r3, [r3, #0x2c]
0077499c: mov      r0, r4
007749a0: strb     r1, [sp]
007749a4: str      r2, [sp, #4]
007749a8: strb     r2, [sp, #1]
007749ac: strh     r2, [sp, #2]
007749b0: mov      r1, sp
007749b4: blx      r3
007749b8: cmp      r0, #0
007749bc: beq      #0x7749cc
007749c0: mov      r0, r5
007749c4: mov      r1, r4
007749c8: bl       #0x75518c
007749cc: mov      r0, r5
007749d0: mov      r1, r7
007749d4: bl       #0x75518c
007749d8: mov      r0, r4
007749dc: bl       #0x75a240
007749e0: add      sp, sp, #0x14
007749e4: pop      {r4, r5, r6, r7, pc}
007749e8: add      r0, r0, #0x88
007749ec: bl       #0x75518c
007749f0: b        #0x7749e0
007749f4: add      r0, r5, #0x88
007749f8: mov      r1, r7
007749fc: bl       #0x75518c
00774a00: b        #0x7749d8

# _ZN7gameswf9tu_stringC1ERKS0_
0075302c: push     {r4, r5, r6, lr}
00753030: mov      r3, #1
00753034: strb     r3, [r0]
00753038: mov      r3, #0
0075303c: strb     r3, [r0, #1]
00753040: mov      r5, r1
00753044: ldrsb    r1, [r1]
00753048: mov      r4, r0
0075304c: cmn      r1, #1
00753050: ldreq    r1, [r5, #4]
00753054: sub      r1, r1, #1
00753058: bl       #0x751d14
0075305c: ldrsb    r3, [r4]
00753060: cmn      r3, #1
00753064: ldrsb    r3, [r5]
00753068: addne    r0, r4, #1
0075306c: ldreq    r0, [r4, #0xc]
00753070: cmn      r3, #1
00753074: addne    r1, r5, #1
00753078: ldreq    r1, [r5, #0xc]
0075307c: bl       #0x30e520
00753080: ldr      r6, [r5, #0x10]
00753084: mvn      r3, #0xff000000
00753088: bic      r2, r6, #0xff000000
0075308c: cmp      r2, r3
00753090: sbfxne   r2, r6, #0, #0x18
00753094: beq      #0x7530b8
00753098: ldr      r3, [r4, #0x10]
0075309c: mov      r0, r4
007530a0: bfi      r3, r2, #0, #0x18
007530a4: lsr      r2, r3, #0x18
007530a8: bfc      r2, #0, #1
007530ac: str      r3, [r4, #0x10]
007530b0: strb     r2, [r4, #0x13]
007530b4: pop      {r4, r5, r6, pc}
007530b8: ldrsb    r3, [r5]
007530bc: cmn      r3, #1
007530c0: ldreq    r3, [r5, #4]
007530c4: subne    r3, r3, #1
007530c8: addne    ip, r5, #1
007530cc: subeq    r3, r3, #1
007530d0: ldreq    ip, [r5, #0xc]
007530d4: cmp      r3, #0
007530d8: movwle   r2, #0x1505
007530dc: ble      #0x753114
007530e0: add      r3, ip, r3
007530e4: movw     r2, #0x1505
007530e8: ldrb     r1, [r3, #-1]
007530ec: sub      r3, r3, #1
007530f0: add      r2, r2, r2, lsl #5
007530f4: sub      r0, r1, #0x41
007530f8: uxtb     r0, r0
007530fc: cmp      r0, #0x19
00753100: addls    r1, r1, #0x20
00753104: cmp      r3, ip
00753108: eor      r2, r1, r2
0075310c: bne      #0x7530e8
00753110: sbfx     r2, r2, #0, #0x18
00753114: bfi      r6, r2, #0, #0x18
00753118: str      r6, [r5, #0x10]
0075311c: b        #0x753098

# _ZN7gameswf19edit_text_character8get_rootEv
0078dde8: push     {r4, lr}
0078ddec: ldr      r3, [r0, #0x40]
0078ddf0: mov      r4, r0
0078ddf4: cmp      r3, #0
0078ddf8: beq      #0x78de0c
0078ddfc: ldr      r0, [r0, #0x3c]
0078de00: ldrb     r2, [r0, #4]
0078de04: cmp      r2, #0
0078de08: beq      #0x78de20
0078de0c: mov      r0, r3
0078de10: ldr      r3, [r3]
0078de14: mov      lr, pc
0078de18: ldr      pc, [r3, #0x54]
0078de1c: pop      {r4, pc}
0078de20: ldr      r1, [r0]
0078de24: sub      r1, r1, #1
0078de28: cmp      r1, #0
0078de2c: str      r1, [r0]
0078de30: bne      #0x78de38
0078de34: bl       #0x752b38
0078de38: mov      r3, #0
0078de3c: str      r3, [r4, #0x40]
0078de40: str      r3, [r4, #0x3c]
0078de44: mov      r0, r3
0078de48: ldr      r3, [r3]
0078de4c: mov      lr, pc
0078de50: ldr      pc, [r3, #0x54]
0078de54: pop      {r4, pc}
