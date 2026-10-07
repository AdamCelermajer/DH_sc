
# _ZN7gameswf8as_valueC1ERKS0_S2_
00769540: push     {r4, r5, r6, r7, r8, lr}
00769544: mov      r3, #0
00769548: mov      r4, r0
0076954c: mov      r0, #6
00769550: strb     r3, [r4]
00769554: str      r3, [r4, #4]
00769558: strb     r0, [r4, #1]
0076955c: mov      r6, r1
00769560: mov      r0, #0x14
00769564: mov      r1, r3
00769568: mov      r7, r2
0076956c: bl       #0x752ba8
00769570: mov      r1, r6
00769574: mov      r5, r0
00769578: mov      r2, r7
0076957c: bl       #0x796cf4
00769580: mov      r0, r5
00769584: str      r5, [r4, #8]
00769588: bl       #0x759c64
0076958c: mov      r0, r4
00769590: pop      {r4, r5, r6, r7, r8, pc}

# _ZN7gameswf8as_value10set_stringEPNS_9as_stringE
007971fc: push     {r4, r5, r6, lr}
00797200: mov      r4, r0
00797204: mov      r0, r1
00797208: mov      r5, r1
0079720c: bl       #0x759c64
00797210: mov      r0, r4
00797214: bl       #0x797124
00797218: add      r3, r5, #0xc
0079721c: mov      r2, #3
00797220: str      r5, [r4, #8]
00797224: strb     r2, [r4, #1]
00797228: str      r3, [r4, #4]
0079722c: pop      {r4, r5, r6, pc}

# _ZN7gameswf8as_valueC1EPNS_9as_objectE
007babb0: mov      r3, #0
007babb4: push     {r4, lr}
007babb8: cmp      r1, #0
007babbc: strb     r3, [r0]
007babc0: mov      r3, #5
007babc4: mov      r4, r0
007babc8: strb     r3, [r0, #1]
007babcc: str      r1, [r0, #4]
007babd0: beq      #0x7babdc
007babd4: mov      r0, r1
007babd8: bl       #0x759c64
007babdc: mov      r0, r4
007babe0: pop      {r4, pc}

# _ZN7gameswf8as_value10set_stringEPKc
00797350: push     {r4, r5, r6, r7, r8, lr}
00797354: ldr      r4, [pc, #0xa4]
00797358: ldr      r6, [pc, #0xa4]
0079735c: ldrsb    r2, [r0, #1]
00797360: add      r4, pc, r4
00797364: ldr      r3, [r4, r6]
00797368: sub      sp, sp, #0x18
0079736c: cmp      r2, #4
00797370: ldr      r3, [r3]
00797374: mov      r5, r0
00797378: mov      r7, r1
0079737c: str      r3, [sp, #0x14]
00797380: beq      #0x7973c8
00797384: bl       #0x797124
00797388: mov      r3, #4
0079738c: strb     r3, [r5, #1]
00797390: mov      r1, #0
00797394: mov      r0, #0x14
00797398: bl       #0x752ba8
0079739c: mov      r1, r7
007973a0: mov      r8, r0
007973a4: bl       #0x413a7c
007973a8: str      r8, [r5, #4]
007973ac: ldr      r3, [r4, r6]
007973b0: ldr      r2, [sp, #0x14]
007973b4: ldr      r3, [r3]
007973b8: cmp      r2, r3
007973bc: bne      #0x7973fc
007973c0: add      sp, sp, #0x18
007973c4: pop      {r4, r5, r6, r7, r8, pc}
007973c8: mov      r0, sp
007973cc: bl       #0x413a7c
007973d0: ldr      r0, [r5, #4]
007973d4: mov      r1, sp
007973d8: bl       #0x752f50
007973dc: ldrsb    r3, [sp]
007973e0: mov      r7, sp
007973e4: cmn      r3, #1
007973e8: bne      #0x7973ac
007973ec: ldr      r0, [sp, #0xc]
007973f0: ldr      r1, [sp, #8]
007973f4: bl       #0x752b38
007973f8: b        #0x7973ac
007973fc: bl       #0x30e310
00797400: andseq   sp, pc, r0, lsr r7
00797404: andeq    r4, r0, ip, lsr #1

# _ZN7gameswf8as_value10set_doubleEd
00797488: push     {r4, r6, r7, lr}
0079748c: mov      r6, r2
00797490: sub      sp, sp, #8
00797494: mov      r7, r3
00797498: mov      r4, r0
0079749c: bl       #0x797124
007974a0: strd     r6, r7, [sp]
007974a4: ldr      r3, [sp]
007974a8: ldr      r2, [sp, #4]
007974ac: mov      r1, #2
007974b0: strb     r1, [r4, #1]
007974b4: str      r2, [r4, #8]
007974b8: str      r3, [r4, #4]
007974bc: add      sp, sp, #8
007974c0: pop      {r4, r6, r7, pc}

# _ZNK7gameswf8as_value10to_xstringEv
00796f5c: push     {r4, lr}
00796f60: ldrsb    r2, [r0, #1]
00796f64: cmp      r2, #5
00796f68: beq      #0x796f84
00796f6c: bl       #0x420a84
00796f70: ldrsb    r3, [r0]
00796f74: cmn      r3, #1
00796f78: addne    r0, r0, #1
00796f7c: ldreq    r0, [r0, #0xc]
00796f80: pop      {r4, pc}
00796f84: ldr      r4, [pc, #0x20]
00796f88: ldr      r2, [pc, #0x20]
00796f8c: ldr      r3, [r0, #4]
00796f90: add      r4, pc, r4
00796f94: add      r2, pc, r2
00796f98: mov      r0, r4
00796f9c: mov      r1, #0x10
00796fa0: bl       #0x30e244
00796fa4: mov      r0, r4
00796fa8: pop      {r4, pc}
00796fac: eoreq    r5, sb, r4, ror #21
00796fb0: ldrsheq  r3, [r7], -ip

# _ZN7gameswf8as_value8set_boolEb
00797230: push     {r4, r5, r6, lr}
00797234: mov      r4, r0
00797238: mov      r5, r1
0079723c: bl       #0x797124
00797240: mov      r3, #1
00797244: strb     r5, [r4, #4]
00797248: strb     r3, [r4, #1]
0079724c: pop      {r4, r5, r6, pc}
