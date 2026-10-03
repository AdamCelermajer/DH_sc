
# _ZNK9LuaScript12_GetFuncNameEPKc
0037c314: push     {r4, r5, r6, lr}
0037c318: mov      r5, r0
0037c31c: mov      r0, r1
0037c320: mov      r4, r1
0037c324: bl       #0x37c164
0037c328: ldr      r3, [r5, #0x38]
0037c32c: add      r5, r5, #0x34
0037c330: cmp      r3, #0
0037c334: beq      #0x37c388
0037c338: mov      r1, r5
0037c33c: b        #0x37c344
0037c340: mov      r3, r2
0037c344: ldr      r2, [r3, #0x10]
0037c348: cmp      r0, r2
0037c34c: ldrhi    r2, [r3, #0xc]
0037c350: ldrls    r2, [r3, #8]
0037c354: movhi    r3, r1
0037c358: mov      r1, r3
0037c35c: cmp      r2, #0
0037c360: bne      #0x37c340
0037c364: cmp      r5, r3
0037c368: beq      #0x37c380
0037c36c: ldr      r2, [r3, #0x10]
0037c370: cmp      r0, r2
0037c374: blo      #0x37c388
0037c378: cmp      r5, r3
0037c37c: ldrne    r4, [r3, #0x28]
0037c380: mov      r0, r4
0037c384: pop      {r4, r5, r6, pc}
0037c388: mov      r3, r5
0037c38c: b        #0x37c378

# _ZN3sfc6script3lua5ValueD1Ev
003193e8: ldr      r3, [pc, #0x5c]
003193ec: ldr      r2, [pc, #0x5c]
003193f0: push     {r4, lr}
003193f4: add      r3, pc, r3
003193f8: ldr      r2, [r3, r2]
003193fc: mov      r4, r0
00319400: add      r2, r2, #8
00319404: str      r2, [r0], #0x24
00319408: bl       #0x3193b0
0031940c: add      r3, r4, #0xc
00319410: ldr      r0, [r3, #0x14]
00319414: cmp      r0, r3
00319418: beq      #0x319438
0031941c: cmp      r0, #0
00319420: beq      #0x319438
00319424: ldr      r1, [r4, #0xc]
00319428: rsb      r1, r0, r1
0031942c: cmp      r1, #0x80
00319430: bhi      #0x319440
00319434: bl       #0x708f00
00319438: mov      r0, r4
0031943c: pop      {r4, pc}
00319440: bl       #0x310440
00319444: mov      r0, r4
00319448: pop      {r4, pc}
0031944c: mlseq    r7, ip, r6, fp
00319450: muleq    r0, r8, r7

# _ZN3sfc6script3lua8Instance5pCallEPKcRKNS1_9ArgumentsERNS1_12ReturnValuesE
0031abe8: push     {r4, r5, r6, lr}
0031abec: mov      r4, r0
0031abf0: mov      r0, r1
0031abf4: mvn      r1, #0x2700
0031abf8: mov      r5, r2
0031abfc: sub      r1, r1, #0x11
0031ac00: mov      r2, r0
0031ac04: ldr      r0, [r4, #4]
0031ac08: mov      r6, r3
0031ac0c: bl       #0x84c1ec
0031ac10: mov      r0, r4
0031ac14: mov      r1, r5
0031ac18: mov      r2, r6
0031ac1c: pop      {r4, r5, r6, lr}
0031ac20: b        #0x31ab4c

# _ZN3sfc6script3lua5Error8setErrorEP9lua_Statei
0031a8ac: cmp      r2, #0
0031a8b0: push     {r4, r5, r6, lr}
0031a8b4: mov      r4, r0
0031a8b8: mov      r5, r1
0031a8bc: str      r2, [r0, #4]
0031a8c0: bne      #0x31a8dc
0031a8c4: ldr      r1, [pc, #0x48]
0031a8c8: add      r0, r0, #8
0031a8cc: add      r1, pc, r1
0031a8d0: mov      r2, r1
0031a8d4: pop      {r4, r5, r6, lr}
0031a8d8: b        #0x3109e0
0031a8dc: mvn      r1, #0
0031a8e0: mov      r2, #0
0031a8e4: mov      r0, r5
0031a8e8: bl       #0x84c384
0031a8ec: mov      r6, r0
0031a8f0: bl       #0x30de54
0031a8f4: mov      r1, r6
0031a8f8: add      r2, r6, r0
0031a8fc: add      r0, r4, #8
0031a900: bl       #0x3109e0
0031a904: mov      r0, r5
0031a908: mvn      r1, #1
0031a90c: pop      {r4, r5, r6, lr}
0031a910: b        #0x84b140
0031a914: subseq   r0, fp, ip, lsr pc
