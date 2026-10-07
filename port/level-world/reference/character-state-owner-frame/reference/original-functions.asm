
# _ZN6CSDead8OnUpdateEiP9CharacterP16CharStateMachine
003c004c: bx       lr

# _ZN6CSIdle8OnUpdateEiP9CharacterP16CharStateMachine
003c0e80: mov      r0, r1
003c0e84: mov      r1, r2
003c0e88: mov      r2, r3
003c0e8c: b        #0x3c0b78

# _ZN6CSMove8OnUpdateEiP9CharacterP16CharStateMachine
003c1184: push     {r4, r5, lr}
003c1188: ldrb     r1, [r2, #0x1b5]
003c118c: sub      sp, sp, #0xc
003c1190: mov      r4, r2
003c1194: cmp      r1, #0
003c1198: mov      r5, r0
003c119c: bne      #0x3c11b8
003c11a0: mov      r0, r4
003c11a4: mov      r1, #0x3f
003c11a8: mov      r2, #0
003c11ac: add      sp, sp, #0xc
003c11b0: pop      {r4, r5, lr}
003c11b4: b        #0x3a4d5c
003c11b8: ldr      r2, [r2, #0x408]
003c11bc: cmp      r2, #0
003c11c0: beq      #0x3c1224
003c11c4: mov      r2, r3
003c11c8: mov      r0, r5
003c11cc: mov      r1, r4
003c11d0: bl       #0x3c0f18
003c11d4: add      r0, r4, #0x560
003c11d8: bl       #0x3de6c4
003c11dc: ldr      r1, [r4, #0x52c]
003c11e0: mov      r5, r0
003c11e4: bl       #0x30e3ac
003c11e8: movw     r1, #0xb717
003c11ec: bic      r0, r0, #0x80000000
003c11f0: movt     r1, #0x38d1
003c11f4: bl       #0x30e70c
003c11f8: cmp      r0, #0
003c11fc: beq      #0x3c1208
003c1200: add      sp, sp, #0xc
003c1204: pop      {r4, r5, pc}
003c1208: add      r0, r4, #0x490
003c120c: add      r0, r0, #0xc
003c1210: mov      r1, r5
003c1214: str      r5, [r4, #0x52c]
003c1218: add      sp, sp, #0xc
003c121c: pop      {r4, r5, lr}
003c1220: b        #0x3c93fc
003c1224: ldr      r2, [r4]
003c1228: mov      r0, r4
003c122c: str      r3, [sp, #4]
003c1230: mov      lr, pc
003c1234: ldr      pc, [r2, #0x28]
003c1238: cmp      r0, #0
003c123c: ldr      r3, [sp, #4]
003c1240: beq      #0x3c1254
003c1244: ldrb     r2, [r4, #0x1b5]
003c1248: cmp      r2, #0
003c124c: beq      #0x3c1200
003c1250: b        #0x3c11c4
003c1254: mov      r0, r4
003c1258: bl       #0x39361c
003c125c: cmp      r0, #0
003c1260: ldr      r3, [sp, #4]
003c1264: beq      #0x3c1244
003c1268: ldr      r2, [r4]
003c126c: mov      r0, r4
003c1270: mov      lr, pc
003c1274: ldr      pc, [r2, #0x54]
003c1278: cmp      r0, #0
003c127c: ldr      r3, [sp, #4]
003c1280: bne      #0x3c1244
003c1284: b        #0x3c11a0

# _ZN16CharStateMachine6UpdateEv
003c628c: push     {r4, r5, r6, lr}
003c6290: mov      r4, r0
003c6294: ldr      r0, [pc, #0xe8]
003c6298: sub      sp, sp, #8
003c629c: ldr      r5, [pc, #0xe4]
003c62a0: add      r0, pc, r0
003c62a4: bl       #0x3136b4
003c62a8: ldr      r3, [pc, #0xdc]
003c62ac: add      r5, pc, r5
003c62b0: ldr      r6, [r4, #0x60]
003c62b4: ldr      r0, [r5, r3]
003c62b8: bl       #0x31f66c
003c62bc: ldr      r3, [r4, #0x2c]
003c62c0: add      r0, r0, r6
003c62c4: str      r0, [r4, #0x60]
003c62c8: tst      r3, #2
003c62cc: bne      #0x3c6314
003c62d0: tst      r3, #4
003c62d4: bne      #0x3c6350
003c62d8: ldr      r3, [r4, #0x20]
003c62dc: cmp      r3, #0
003c62e0: beq      #0x3c6300
003c62e4: ldm      r3, {r1, r2}
003c62e8: mov      r3, r4
003c62ec: mov      r0, r2
003c62f0: ldr      ip, [r2]
003c62f4: ldr      r2, [r4, #4]
003c62f8: mov      lr, pc
003c62fc: ldr      pc, [ip, #0x14]
003c6300: ldr      r0, [pc, #0x88]
003c6304: add      r0, pc, r0
003c6308: add      sp, sp, #8
003c630c: pop      {r4, r5, r6, lr}
003c6310: b        #0x3136b8
003c6314: mov      r0, r4
003c6318: mov      r1, #0
003c631c: bl       #0x3c0378
003c6320: subs     ip, r0, #0
003c6324: bne      #0x3c6344
003c6328: ldr      r2, [r4, #0x24]
003c632c: mov      r3, ip
003c6330: mov      r0, r4
003c6334: ubfx     r2, r2, #0xb, #1
003c6338: mvn      r1, #0
003c633c: str      ip, [sp]
003c6340: bl       #0x3c5ffc
003c6344: ldr      r3, [r4, #0x2c]
003c6348: tst      r3, #4
003c634c: beq      #0x3c62d8
003c6350: mov      r0, r4
003c6354: mov      r1, #0
003c6358: bl       #0x3c034c
003c635c: subs     ip, r0, #0
003c6360: bne      #0x3c62d8
003c6364: ldr      r2, [r4, #0x24]
003c6368: mov      r3, ip
003c636c: mov      r0, r4
003c6370: ubfx     r2, r2, #0xa, #1
003c6374: mvn      r1, #0
003c6378: str      ip, [sp]
003c637c: bl       #0x3c6144
003c6380: b        #0x3c62d8
003c6384: subeq    lr, pc, r0, lsr #25
003c6388: subseq   lr, ip, r4, ror #15
003c638c: strdeq   r3, r4, [r0], -r4
003c6390: subeq    lr, pc, ip, lsr ip

# _ZN8CSAttack8OnUpdateEiP9CharacterP16CharStateMachine
003c14f8: push     {r4, r5, r6, lr}
003c14fc: ldr      r1, [r2, #0x408]
003c1500: mov      r4, r2
003c1504: mov      r0, r2
003c1508: bl       #0x393d48
003c150c: add      r0, r4, #0x560
003c1510: bl       #0x3de74c
003c1514: mov      r1, r0
003c1518: mov      r5, r0
003c151c: ldr      r0, [r4, #0x52c]
003c1520: bl       #0x30e3ac
003c1524: movw     r1, #0xb717
003c1528: bic      r0, r0, #0x80000000
003c152c: movt     r1, #0x38d1
003c1530: bl       #0x30e70c
003c1534: cmp      r0, #0
003c1538: beq      #0x3c1540
003c153c: pop      {r4, r5, r6, pc}
003c1540: add      r0, r4, #0x490
003c1544: add      r0, r0, #0xc
003c1548: mov      r1, r5
003c154c: str      r5, [r4, #0x52c]
003c1550: pop      {r4, r5, r6, lr}
003c1554: b        #0x3c93fc
