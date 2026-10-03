
# luaopen_table
0085b174: ldr      r1, [pc, #0x18]
0085b178: ldr      r2, [pc, #0x18]
0085b17c: push     {r4, lr}
0085b180: add      r1, pc, r1
0085b184: add      r2, pc, r2
0085b188: bl       #0x84d264
0085b18c: mov      r0, #1
0085b190: pop      {r4, pc}
0085b194: andeq    r1, sb, r0, asr fp
0085b198: andeq    pc, pc, r0, asr #23

# lua_gettop
0084b12c: ldr      r3, [r0, #0xc]
0084b130: ldr      r0, [r0, #8]
0084b134: rsb      r0, r3, r0
0084b138: asr      r0, r0, #3
0084b13c: bx       lr

# luaopen_string
0085a0dc: ldr      r1, [pc, #0xb8]
0085a0e0: ldr      r2, [pc, #0xb8]
0085a0e4: push     {r4, lr}
0085a0e8: add      r1, pc, r1
0085a0ec: add      r2, pc, r2
0085a0f0: mov      r4, r0
0085a0f4: bl       #0x84d264
0085a0f8: ldr      r2, [pc, #0xa4]
0085a0fc: mov      r0, r4
0085a100: mvn      r1, #0
0085a104: add      r2, pc, r2
0085a108: bl       #0x84c1ec
0085a10c: ldr      r2, [pc, #0x94]
0085a110: mov      r0, r4
0085a114: mvn      r1, #1
0085a118: add      r2, pc, r2
0085a11c: bl       #0x84c080
0085a120: mov      r0, r4
0085a124: mov      r1, #0
0085a128: mov      r2, #1
0085a12c: bl       #0x84c11c
0085a130: ldr      r1, [pc, #0x74]
0085a134: mov      r2, #0
0085a138: mov      r0, r4
0085a13c: add      r1, pc, r1
0085a140: bl       #0x84b9d4
0085a144: mov      r0, r4
0085a148: mvn      r1, #1
0085a14c: bl       #0x84b234
0085a150: mov      r0, r4
0085a154: mvn      r1, #1
0085a158: bl       #0x84be70
0085a15c: mov      r0, r4
0085a160: mvn      r1, #1
0085a164: bl       #0x84b140
0085a168: mov      r0, r4
0085a16c: mvn      r1, #1
0085a170: bl       #0x84b234
0085a174: ldr      r2, [pc, #0x34]
0085a178: mov      r0, r4
0085a17c: mvn      r1, #1
0085a180: add      r2, pc, r2
0085a184: bl       #0x84c080
0085a188: mov      r0, r4
0085a18c: mvn      r1, #1
0085a190: bl       #0x84b140
0085a194: mov      r0, #1
0085a198: pop      {r4, pc}
0085a19c: strheq   r6, [fp], -r0
0085a1a0: ldrsbeq  r0, [r0], -r8
0085a1a4: ldrdeq   r6, r7, [fp], -r4
0085a1a8: andeq    r6, fp, r8, asr #15
0085a1ac: andeq    r1, r7, ip, asr #13
0085a1b0: muleq    r6, r0, r7

# luaopen_base
0084dbfc: push     {r4, r5, r6, r7, r8, lr}
0084dc00: ldr      r7, [pc, #0x138]
0084dc04: mvn      r5, #0x2700
0084dc08: ldr      r6, [pc, #0x134]
0084dc0c: sub      r5, r5, #0x11
0084dc10: mov      r4, r0
0084dc14: add      r7, pc, r7
0084dc18: mov      r1, r5
0084dc1c: bl       #0x84b234
0084dc20: add      r6, pc, r6
0084dc24: mov      r0, r4
0084dc28: mov      r1, r5
0084dc2c: mov      r2, r7
0084dc30: bl       #0x84c080
0084dc34: mov      r1, r7
0084dc38: mov      r0, r4
0084dc3c: add      r2, r6, #0x10
0084dc40: bl       #0x84d264
0084dc44: ldr      r1, [pc, #0xfc]
0084dc48: mov      r0, r4
0084dc4c: mov      r2, #7
0084dc50: add      r1, pc, r1
0084dc54: bl       #0x84b9d4
0084dc58: ldr      r2, [pc, #0xec]
0084dc5c: mov      r0, r4
0084dc60: mov      r1, r5
0084dc64: add      r2, pc, r2
0084dc68: bl       #0x84c080
0084dc6c: ldr      r1, [pc, #0xdc]
0084dc70: ldr      r2, [pc, #0xdc]
0084dc74: ldr      r3, [pc, #0xdc]
0084dc78: mov      r0, r4
0084dc7c: add      r1, pc, r1
0084dc80: add      r2, pc, r2
0084dc84: add      r3, pc, r3
0084dc88: bl       #0x84dbbc
0084dc8c: ldr      r1, [pc, #0xc8]
0084dc90: ldr      r2, [pc, #0xc8]
0084dc94: ldr      r3, [pc, #0xc8]
0084dc98: mov      r0, r4
0084dc9c: add      r1, pc, r1
0084dca0: add      r3, pc, r3
0084dca4: add      r2, pc, r2
0084dca8: bl       #0x84dbbc
0084dcac: mov      r2, #1
0084dcb0: mov      r0, r4
0084dcb4: mov      r1, #0
0084dcb8: bl       #0x84c11c
0084dcbc: mov      r0, r4
0084dcc0: mvn      r1, #0
0084dcc4: bl       #0x84b234
0084dcc8: mov      r0, r4
0084dccc: mvn      r1, #1
0084dcd0: bl       #0x84be70
0084dcd4: ldr      r1, [pc, #0x8c]
0084dcd8: mov      r0, r4
0084dcdc: mov      r2, #2
0084dce0: add      r1, pc, r1
0084dce4: bl       #0x84b9d4
0084dce8: ldr      r2, [pc, #0x7c]
0084dcec: mov      r0, r4
0084dcf0: mvn      r1, #1
0084dcf4: add      r2, pc, r2
0084dcf8: bl       #0x84c080
0084dcfc: ldr      r1, [pc, #0x6c]
0084dd00: mov      r0, r4
0084dd04: mov      r2, #1
0084dd08: add      r1, pc, r1
0084dd0c: bl       #0x84bcdc
0084dd10: ldr      r2, [pc, #0x5c]
0084dd14: mov      r1, r5
0084dd18: mov      r0, r4
0084dd1c: add      r2, pc, r2
0084dd20: bl       #0x84c080
0084dd24: ldr      r1, [pc, #0x4c]
0084dd28: mov      r0, r4
0084dd2c: add      r2, r6, #0xc8
0084dd30: add      r1, pc, r1
0084dd34: bl       #0x84d264
0084dd38: mov      r0, #2
0084dd3c: pop      {r4, r5, r6, r7, r8, pc}
0084dd40: andeq    r1, ip, ip, lsl ip
0084dd44: andseq   ip, r0, r4, ror sp
0084dd48: andeq    r1, ip, r8, ror #23
0084dd4c: ldrdeq   r1, r2, [ip], -ip
0084dd50: ldrdeq   r1, r2, [ip], -r4
0084dd54: muleq    r0, r8, sb
0084dd58: andeq    r0, r0, r4, lsr sb
0084dd5c: strheq   r1, [ip], -ip
0084dd60: andeq    r0, r0, r8, lsl #3
0084dd64: andeq    r1, r0, r0, lsr r0
0084dd68: andeq    r1, ip, r0, lsl #23
0084dd6c: andeq    r1, ip, r4, ror fp
0084dd70: strheq   r0, [r0], -r8
0084dd74: andeq    r1, ip, r4, asr fp
0084dd78: andeq    r1, ip, r0, asr fp

# _ZN3sfc6script3lua8Instance16registerFunctionEPKcPFiP9lua_StateERKNS1_9ArgumentsE
0031af08: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0031af0c: mov      r5, r3
0031af10: ldr      r3, [r3, #4]
0031af14: mov      r4, r0
0031af18: mov      r6, r1
0031af1c: ldm      r3, {r0, r1}
0031af20: mov      sl, r2
0031af24: rsb      r1, r0, r1
0031af28: asr      r1, r1, #4
0031af2c: add      r2, r1, r1, lsl #3
0031af30: add      r2, r2, r2, lsl #6
0031af34: add      r2, r1, r2, lsl #3
0031af38: add      r2, r2, r2, lsl #15
0031af3c: add      r2, r1, r2, lsl #3
0031af40: rsb      r2, r2, #0
0031af44: cmp      r2, #0
0031af48: beq      #0x31af98
0031af4c: mov      r7, #0
0031af50: mov      r8, r7
0031af54: add      r0, r0, r7
0031af58: ldr      r1, [r4, #4]
0031af5c: bl       #0x31cac4
0031af60: ldr      r1, [r5, #4]
0031af64: add      r8, r8, #1
0031af68: add      r7, r7, #0x70
0031af6c: ldm      r1, {r0, r3}
0031af70: rsb      r3, r0, r3
0031af74: asr      r3, r3, #4
0031af78: add      r2, r3, r3, lsl #3
0031af7c: add      r2, r2, r2, lsl #6
0031af80: add      r2, r3, r2, lsl #3
0031af84: add      r2, r2, r2, lsl #15
0031af88: add      r2, r3, r2, lsl #3
0031af8c: rsb      r2, r2, #0
0031af90: cmp      r8, r2
0031af94: blo      #0x31af54
0031af98: mov      r1, sl
0031af9c: ldr      r0, [r4, #4]
0031afa0: bl       #0x84bcdc
0031afa4: ldr      r0, [r4, #4]
0031afa8: mvn      r1, #0x2700
0031afac: sub      r1, r1, #0x11
0031afb0: mov      r2, r6
0031afb4: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
0031afb8: b        #0x84c080

# luaopen_math
00853e38: ldr      r1, [pc, #0x8c]
00853e3c: ldr      r2, [pc, #0x8c]
00853e40: push     {r4, lr}
00853e44: add      r2, pc, r2
00853e48: add      r1, pc, r1
00853e4c: mov      r4, r0
00853e50: bl       #0x84d264
00853e54: movw     r1, #0xfdb
00853e58: mov      r0, r4
00853e5c: movt     r1, #0x4049
00853e60: bl       #0x84b458
00853e64: ldr      r2, [pc, #0x68]
00853e68: mov      r0, r4
00853e6c: mvn      r1, #1
00853e70: add      r2, pc, r2
00853e74: bl       #0x84c080
00853e78: mov      r1, #0x7f000000
00853e7c: mov      r0, r4
00853e80: add      r1, r1, #0x800000
00853e84: bl       #0x84b458
00853e88: ldr      r2, [pc, #0x48]
00853e8c: mov      r0, r4
00853e90: mvn      r1, #1
00853e94: add      r2, pc, r2
00853e98: bl       #0x84c080
00853e9c: ldr      r2, [pc, #0x38]
00853ea0: mov      r0, r4
00853ea4: mvn      r1, #0
00853ea8: add      r2, pc, r2
00853eac: bl       #0x84c1ec
00853eb0: ldr      r2, [pc, #0x28]
00853eb4: mov      r0, r4
00853eb8: mvn      r1, #1
00853ebc: add      r2, pc, r2
00853ec0: bl       #0x84c080
00853ec4: mov      r0, #1
00853ec8: pop      {r4, pc}
00853ecc: ldrdeq   r5, r6, [fp], -r8
00853ed0: ldrsheq  r6, [r0], -ip
00853ed4: andeq    r6, fp, r0, lsr #12
00853ed8: andeq    ip, fp, r4, lsr #2
00853edc: andeq    ip, fp, r8, lsl r1
00853ee0: andeq    sp, fp, ip, lsl #19
