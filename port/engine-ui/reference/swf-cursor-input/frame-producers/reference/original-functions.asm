
# _ZN7gameswf15sprite_instanceC2EPNS_6playerEPNS_20movie_definition_subEPNS_4rootEPNS_9characterEi
00781074: push     {r4, r5, r6, r7, r8, lr}
00781078: sub      sp, sp, #8
0078107c: mov      r6, r2
00781080: mov      ip, #2
00781084: mov      r7, r3
00781088: ldr      r5, [pc, #0x22c]
0078108c: ldr      r3, [sp, #0x24]
00781090: ldr      r2, [sp, #0x20]
00781094: mov      r4, r0
00781098: str      ip, [sp]
0078109c: bl       #0x754b28
007810a0: ldr      r3, [pc, #0x218]
007810a4: add      r5, pc, r5
007810a8: cmp      r6, #0
007810ac: ldr      r3, [r5, r3]
007810b0: str      r6, [r4, #0xa0]
007810b4: add      r3, r3, #8
007810b8: str      r3, [r4]
007810bc: beq      #0x7810c8
007810c0: mov      r0, r6
007810c4: bl       #0x759c64
007810c8: ldr      r3, [r4, #0xa0]
007810cc: mov      r5, #0
007810d0: mov      r2, #1
007810d4: str      r7, [r4, #0xa4]
007810d8: strb     r2, [r4, #0xea]
007810dc: str      r5, [r4, #0xa8]
007810e0: str      r5, [r4, #0xac]
007810e4: str      r5, [r4, #0xb0]
007810e8: strb     r5, [r4, #0xb4]
007810ec: str      r5, [r4, #0xb8]
007810f0: str      r5, [r4, #0xbc]
007810f4: str      r5, [r4, #0xc0]
007810f8: str      r5, [r4, #0xc4]
007810fc: strb     r5, [r4, #0xc8]
00781100: str      r5, [r4, #0xcc]
00781104: str      r5, [r4, #0xd0]
00781108: str      r5, [r4, #0xd4]
0078110c: strb     r5, [r4, #0xd8]
00781110: str      r5, [r4, #0xdc]
00781114: str      r5, [r4, #0xe0]
00781118: strh     r5, [r4, #0xe4]
0078111c: strb     r5, [r4, #0xe6]
00781120: strb     r5, [r4, #0xe7]
00781124: strb     r2, [r4, #0xe8]
00781128: strb     r5, [r4, #0xe9]
0078112c: strb     r5, [r4, #0xeb]
00781130: strb     r5, [r4, #0xec]
00781134: str      r5, [r4, #0xf0]
00781138: str      r5, [r4, #0xf4]
0078113c: str      r5, [r4, #0xf8]
00781140: str      r5, [r4, #0xfc]
00781144: mov      r0, r3
00781148: ldr      r3, [r3]
0078114c: mov      lr, pc
00781150: ldr      pc, [r3, #0x88]
00781154: cmp      r0, r5
00781158: bne      #0x7811b4
0078115c: ldr      r1, [r4, #0x30]
00781160: cmp      r1, #0
00781164: mov      r3, r1
00781168: beq      #0x78117c
0078116c: ldr      r0, [r4, #0x2c]
00781170: ldrb     r2, [r0, #4]
00781174: cmp      r2, #0
00781178: beq      #0x781270
0078117c: ldr      r3, [r3, #0x30]
00781180: cmp      r1, #0
00781184: str      r3, [r4, #0x34]
00781188: beq      #0x78119c
0078118c: ldr      r0, [r4, #0x2c]
00781190: ldrb     r3, [r0, #4]
00781194: cmp      r3, #0
00781198: beq      #0x781248
0078119c: mov      r0, r4
007811a0: add      r1, r1, #0x88
007811a4: bl       #0x768d08
007811a8: mov      r0, r4
007811ac: add      sp, sp, #8
007811b0: pop      {r4, r5, r6, r7, r8, pc}
007811b4: mov      r1, r5
007811b8: mov      r0, #0x20
007811bc: bl       #0x752ba8
007811c0: strb     r5, [r0, #0x1c]
007811c4: str      r5, [r0]
007811c8: str      r5, [r0, #4]
007811cc: str      r5, [r0, #8]
007811d0: strb     r5, [r0, #0xc]
007811d4: str      r5, [r0, #0x10]
007811d8: str      r5, [r0, #0x14]
007811dc: str      r5, [r0, #0x18]
007811e0: ldr      r3, [r4, #0xa0]
007811e4: str      r0, [r4, #0xdc]
007811e8: mov      r7, r0
007811ec: add      r8, r0, #0x10
007811f0: mov      r0, r3
007811f4: ldr      r3, [r3]
007811f8: mov      lr, pc
007811fc: ldr      pc, [r3, #0x38]
00781200: subs     r6, r0, #0
00781204: ldr      r5, [r7, #0x14]
00781208: bne      #0x7812a0
0078120c: cmp      r6, r5
00781210: ble      #0x78122c
00781214: mov      r2, #0
00781218: ldr      r3, [r8]
0078121c: strb     r2, [r3, r5]
00781220: add      r5, r5, #1
00781224: cmp      r5, r6
00781228: bne      #0x781218
0078122c: str      r6, [r7, #0x14]
00781230: ldr      r3, [r4, #0xdc]
00781234: mov      r1, #0
00781238: ldr      r2, [r3, #0x14]
0078123c: ldr      r0, [r3, #0x10]
00781240: bl       #0x30e460
00781244: b        #0x78115c
00781248: ldr      r1, [r0]
0078124c: sub      r1, r1, #1
00781250: cmp      r1, #0
00781254: str      r1, [r0]
00781258: bne      #0x781260
0078125c: bl       #0x752b38
00781260: mov      r1, #0
00781264: str      r1, [r4, #0x2c]
00781268: str      r1, [r4, #0x30]
0078126c: b        #0x78119c
00781270: ldr      r1, [r0]
00781274: sub      r1, r1, #1
00781278: cmp      r1, #0
0078127c: str      r1, [r0]
00781280: bne      #0x781288
00781284: bl       #0x752b38
00781288: mov      r2, #0
0078128c: mov      r3, r2
00781290: str      r2, [r4, #0x2c]
00781294: str      r2, [r4, #0x30]
00781298: mov      r1, r2
0078129c: b        #0x78117c
007812a0: ldr      r3, [r7, #0x18]
007812a4: cmp      r6, r3
007812a8: ble      #0x78120c
007812ac: mov      r0, r8
007812b0: add      r1, r6, r6, asr #1
007812b4: bl       #0x77e930
007812b8: b        #0x78120c
007812bc: eoreq    r3, r1, ip, ror #19
007812c0: andeq    r4, r0, r4, ror fp

# _ZN7gameswf13character_def28instanciate_registered_classEPNS_9characterE
0075fd18: push     {r4, r5, r6, r7, lr}
0075fd1c: ldr      r3, [r0, #0x14]
0075fd20: sub      sp, sp, #0xb4
0075fd24: mov      r5, r0
0075fd28: cmp      r3, #0
0075fd2c: mov      r6, r1
0075fd30: beq      #0x75ff48
0075fd34: ldr      r0, [r0, #0x10]
0075fd38: ldrb     r3, [r0, #4]
0075fd3c: cmp      r3, #0
0075fd40: beq      #0x75ff24
0075fd44: add      r0, r6, #0x20
0075fd48: mov      r1, r6
0075fd4c: bl       #0x75ec88
0075fd50: ldr      r3, [r5, #0x14]
0075fd54: cmp      r3, #0
0075fd58: beq      #0x75fea8
0075fd5c: ldr      r0, [r5, #0x10]
0075fd60: ldrb     r2, [r0, #4]
0075fd64: cmp      r2, #0
0075fd68: beq      #0x75fe84
0075fd6c: mov      r2, #0
0075fd70: strb     r2, [sp, #0xa4]
0075fd74: mov      r0, r3
0075fd78: mov      r2, #5
0075fd7c: strb     r2, [sp, #0xa5]
0075fd80: str      r3, [sp, #0xa8]
0075fd84: bl       #0x759c64
0075fd88: add      r4, sp, #0xa4
0075fd8c: mov      r1, r4
0075fd90: mov      r0, r6
0075fd94: bl       #0x76b8b0
0075fd98: mov      r0, r4
0075fd9c: bl       #0x797124
0075fda0: ldr      r1, [r5, #0x1c]
0075fda4: cmp      r1, #0
0075fda8: beq      #0x75fdbc
0075fdac: ldr      r0, [r5, #0x18]
0075fdb0: ldrb     r3, [r0, #4]
0075fdb4: cmp      r3, #0
0075fdb8: beq      #0x75fefc
0075fdbc: add      r4, sp, #0x14
0075fdc0: mov      r0, r4
0075fdc4: bl       #0x75eb4c
0075fdc8: ldr      r3, [r5, #0x14]
0075fdcc: cmp      r3, #0
0075fdd0: beq      #0x75fee4
0075fdd4: ldr      r0, [r5, #0x10]
0075fdd8: ldrb     r2, [r0, #4]
0075fddc: cmp      r2, #0
0075fde0: beq      #0x75fec0
0075fde4: mov      r2, #0
0075fde8: strb     r2, [sp, #0x98]
0075fdec: mov      r0, r3
0075fdf0: mov      r2, #5
0075fdf4: strb     r2, [sp, #0x99]
0075fdf8: str      r3, [sp, #0x9c]
0075fdfc: bl       #0x759c64
0075fe00: mov      r3, #0
0075fe04: strb     r3, [sp, #0x8c]
0075fe08: cmp      r6, #0
0075fe0c: mov      r3, #5
0075fe10: strb     r3, [sp, #0x8d]
0075fe14: str      r6, [sp, #0x90]
0075fe18: beq      #0x75fe24
0075fe1c: mov      r0, r6
0075fe20: bl       #0x759c64
0075fe24: ldr      ip, [pc, #0x180]
0075fe28: add      r7, sp, #0x80
0075fe2c: add      r5, sp, #0x98
0075fe30: add      r6, sp, #0x8c
0075fe34: mov      lr, #0
0075fe38: add      ip, pc, ip
0075fe3c: mov      r1, r5
0075fe40: mov      r2, r4
0075fe44: mov      r3, r6
0075fe48: mov      r0, r7
0075fe4c: str      lr, [sp, #4]
0075fe50: str      ip, [sp, #8]
0075fe54: str      lr, [sp]
0075fe58: bl       #0x7ba904
0075fe5c: mov      r0, r7
0075fe60: bl       #0x797124
0075fe64: mov      r0, r6
0075fe68: bl       #0x797124
0075fe6c: mov      r0, r5
0075fe70: bl       #0x797124
0075fe74: mov      r0, r4
0075fe78: bl       #0x75e03c
0075fe7c: add      sp, sp, #0xb4
0075fe80: pop      {r4, r5, r6, r7, pc}
0075fe84: ldr      r1, [r0]
0075fe88: sub      r1, r1, #1
0075fe8c: cmp      r1, #0
0075fe90: str      r1, [r0]
0075fe94: bne      #0x75fe9c
0075fe98: bl       #0x752b38
0075fe9c: mov      r3, #0
0075fea0: str      r3, [r5, #0x14]
0075fea4: str      r3, [r5, #0x10]
0075fea8: mov      r3, #0
0075feac: mov      r2, #5
0075feb0: strb     r2, [sp, #0xa5]
0075feb4: str      r3, [sp, #0xa8]
0075feb8: strb     r3, [sp, #0xa4]
0075febc: b        #0x75fd88
0075fec0: ldr      r1, [r0]
0075fec4: sub      r1, r1, #1
0075fec8: cmp      r1, #0
0075fecc: str      r1, [r0]
0075fed0: bne      #0x75fed8
0075fed4: bl       #0x752b38
0075fed8: mov      r3, #0
0075fedc: str      r3, [r5, #0x14]
0075fee0: str      r3, [r5, #0x10]
0075fee4: mov      r3, #0
0075fee8: mov      r2, #5
0075feec: strb     r2, [sp, #0x99]
0075fef0: str      r3, [sp, #0x9c]
0075fef4: strb     r3, [sp, #0x98]
0075fef8: b        #0x75fe00
0075fefc: ldr      r1, [r0]
0075ff00: sub      r1, r1, #1
0075ff04: cmp      r1, #0
0075ff08: str      r1, [r0]
0075ff0c: bne      #0x75ff14
0075ff10: bl       #0x752b38
0075ff14: mov      r1, #0
0075ff18: str      r1, [r5, #0x18]
0075ff1c: str      r1, [r5, #0x1c]
0075ff20: b        #0x75fdbc
0075ff24: ldr      r1, [r0]
0075ff28: sub      r1, r1, #1
0075ff2c: cmp      r1, #0
0075ff30: str      r1, [r0]
0075ff34: bne      #0x75ff3c
0075ff38: bl       #0x752b38
0075ff3c: mov      r3, #0
0075ff40: str      r3, [r5, #0x14]
0075ff44: str      r3, [r5, #0x10]
0075ff48: ldr      r3, [r5]
0075ff4c: mov      r1, r6
0075ff50: mov      r0, r5
0075ff54: mov      lr, pc
0075ff58: ldr      pc, [r3, #0x28]
0075ff5c: mov      r1, r0
0075ff60: add      r0, r5, #0x10
0075ff64: bl       #0x75ebd0
0075ff68: ldr      r3, [r5, #0x14]
0075ff6c: cmp      r3, #0
0075ff70: beq      #0x75fe7c
0075ff74: ldr      r0, [r5, #0x10]
0075ff78: ldrb     r3, [r0, #4]
0075ff7c: cmp      r3, #0
0075ff80: bne      #0x75fd44
0075ff84: ldr      r1, [r0]
0075ff88: sub      r1, r1, #1
0075ff8c: cmp      r1, #0
0075ff90: str      r1, [r0]
0075ff94: bne      #0x75ff9c
0075ff98: bl       #0x752b38
0075ff9c: mov      r3, #0
0075ffa0: str      r3, [r5, #0x14]
0075ffa4: str      r3, [r5, #0x10]
0075ffa8: b        #0x75fe7c
0075ffac: andseq   sp, r6, r0, lsl #16

# _ZN7gameswf15sprite_instance18execute_frame_tagsEib
0078215c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00782160: subs     r4, r0, #0
00782164: sub      sp, sp, #4
00782168: mov      r5, r1
0078216c: mov      r8, r2
00782170: beq      #0x782178
00782174: bl       #0x759c64
00782178: ldr      r6, [r4, #0xa0]
0078217c: ldr      r3, [r6]
00782180: mov      r0, r6
00782184: mov      lr, pc
00782188: ldr      pc, [r3, #0xbc]
0078218c: cmp      r0, #0
00782190: beq      #0x7821a0
00782194: ldr      r3, [r6, #0x3c]
00782198: cmp      r5, r3
0078219c: bge      #0x782408
007821a0: ldr      r3, [r4, #0xdc]
007821a4: cmp      r3, #0
007821a8: beq      #0x7821bc
007821ac: ldr      r3, [r3, #0x10]
007821b0: ldrb     r6, [r3, r5]
007821b4: cmp      r6, #0
007821b8: beq      #0x78226c
007821bc: ldr      r3, [r4, #0xa0]
007821c0: mov      r1, r5
007821c4: mov      r0, r3
007821c8: ldr      r3, [r3]
007821cc: mov      lr, pc
007821d0: ldr      pc, [r3, #0x50]
007821d4: ldr      r3, [r0, #4]
007821d8: mov      r7, r0
007821dc: cmp      r3, #0
007821e0: ble      #0x782240
007821e4: mov      r6, #0
007821e8: b        #0x78220c
007821ec: mov      r0, r3
007821f0: ldr      r3, [r3]
007821f4: mov      lr, pc
007821f8: ldr      pc, [r3, #0xc]
007821fc: ldr      r3, [r7, #4]
00782200: add      r6, r6, #1
00782204: cmp      r6, r3
00782208: bge      #0x782240
0078220c: ldr      r3, [r7]
00782210: cmp      r8, #0
00782214: mov      r1, r4
00782218: ldr      r3, [r3, r6, lsl #2]
0078221c: bne      #0x7821ec
00782220: mov      r0, r3
00782224: ldr      r3, [r3]
00782228: mov      lr, pc
0078222c: ldr      pc, [r3, #8]
00782230: ldr      r3, [r7, #4]
00782234: add      r6, r6, #1
00782238: cmp      r6, r3
0078223c: blt      #0x78220c
00782240: cmp      r8, #0
00782244: beq      #0x782378
00782248: mov      r1, r5
0078224c: mov      r0, r4
00782250: bl       #0x77e2a8
00782254: cmp      r4, #0
00782258: beq      #0x782370
0078225c: mov      r0, r4
00782260: add      sp, sp, #4
00782264: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00782268: b        #0x75a240
0078226c: ldr      r3, [r4, #0xa0]
00782270: mov      r1, r5
00782274: mov      r0, r3
00782278: ldr      r3, [r3]
0078227c: mov      lr, pc
00782280: ldr      pc, [r3, #0x54]
00782284: subs     r7, r0, #0
00782288: beq      #0x7822d4
0078228c: ldr      r3, [r7, #4]
00782290: cmp      r3, #0
00782294: ble      #0x7822d4
00782298: ldr      r3, [r7]
0078229c: mov      r1, r4
007822a0: ldr      r3, [r3, r6, lsl #2]
007822a4: add      r6, r6, #1
007822a8: mov      r0, r3
007822ac: ldr      r3, [r3]
007822b0: mov      lr, pc
007822b4: ldr      pc, [r3, #8]
007822b8: ldr      r3, [r7, #4]
007822bc: cmp      r6, r3
007822c0: blt      #0x782298
007822c4: ldr      r3, [r4, #0xdc]
007822c8: mov      r2, #1
007822cc: ldr      r3, [r3, #0x10]
007822d0: strb     r2, [r3, r5]
007822d4: ldr      r6, [r4, #0xc0]
007822d8: ldr      r7, [r4, #0xdc]
007822dc: ldr      sb, [r4, #0xbc]
007822e0: cmp      r6, #0
007822e4: ble      #0x7823cc
007822e8: ldr      fp, [r7, #4]
007822ec: adds     sl, fp, r6
007822f0: beq      #0x782300
007822f4: ldr      r3, [r7, #8]
007822f8: cmp      sl, r3
007822fc: bgt      #0x7823f8
00782300: cmp      fp, sl
00782304: lslge    r2, fp, #2
00782308: bge      #0x782330
0078230c: lsl      r2, fp, #2
00782310: mov      r3, r2
00782314: mov      r0, #0
00782318: ldr      r1, [r7]
0078231c: add      fp, fp, #1
00782320: cmp      fp, sl
00782324: str      r0, [r1, r3]
00782328: add      r3, r3, #4
0078232c: bne      #0x782318
00782330: str      sl, [r7, #4]
00782334: mov      r3, #0
00782338: ldr      r0, [sb, r3, lsl #2]
0078233c: ldr      r1, [r7]
00782340: add      r3, r3, #1
00782344: cmp      r3, r6
00782348: str      r0, [r1, r2]
0078234c: add      r2, r2, #4
00782350: bne      #0x782338
00782354: ldr      r6, [r4, #0xc0]
00782358: add      r1, r4, #0xbc
0078235c: cmp      r6, #0
00782360: ble      #0x7823d0
00782364: mov      r3, #0
00782368: str      r3, [r4, #0xc0]
0078236c: b        #0x7821bc
00782370: add      sp, sp, #4
00782374: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00782378: bl       #0x77cba0
0078237c: subs     r6, r0, #0
00782380: beq      #0x782248
00782384: ldr      r3, [r4, #0xa0]
00782388: ldr      r2, [r3, #0x28]
0078238c: cmp      r5, r2
00782390: bne      #0x782248
00782394: ldr      r1, [r3, #0x20]
00782398: cmp      r1, #0
0078239c: blt      #0x782248
007823a0: ldr      r3, [r6]
007823a4: mov      lr, pc
007823a8: ldr      pc, [r3, #0x14]
007823ac: ldr      r3, [r4, #0xa0]
007823b0: mov      r0, r6
007823b4: mov      r2, r8
007823b8: ldr      r1, [r3, #0x20]
007823bc: ldr      r3, [r6]
007823c0: mov      lr, pc
007823c4: ldr      pc, [r3, #0xc]
007823c8: b        #0x782248
007823cc: add      r1, r4, #0xbc
007823d0: cmp      r6, #0
007823d4: bge      #0x782364
007823d8: lsl      r3, r6, #2
007823dc: mov      r0, #0
007823e0: ldr      r2, [r1]
007823e4: adds     r6, r6, #1
007823e8: str      r0, [r2, r3]
007823ec: add      r3, r3, #4
007823f0: bne      #0x7823e0
007823f4: b        #0x782364
007823f8: mov      r0, r7
007823fc: add      r1, sl, sl, asr #1
00782400: bl       #0x77e7f8
00782404: b        #0x782300
00782408: b        #0x782408

# _ZN7gameswf15sprite_instance10do_actionsEv
00781eac: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00781eb0: ldr      r3, [r0, #0xc0]
00781eb4: sub      sp, sp, #0xd8
00781eb8: mov      r4, r0
00781ebc: cmp      r3, #0
00781ec0: ble      #0x782008
00781ec4: mov      r5, #1
00781ec8: strb     r5, [r0, #0x9d]
00781ecc: add      r6, sp, #0x14
00781ed0: bl       #0x759c64
00781ed4: mov      r0, r6
00781ed8: mov      r2, #0x80
00781edc: mov      r1, #0
00781ee0: bl       #0x30e460
00781ee4: ldr      r8, [r4, #0xc0]
00781ee8: mov      r3, #0
00781eec: mov      r2, #0x20
00781ef0: cmp      r8, #0x1f
00781ef4: addgt    sb, sp, #0x94
00781ef8: addle    sl, sp, #0xa4
00781efc: strb     r5, [sp, #0xb0]
00781f00: str      r6, [sp, #0xa4]
00781f04: movgt    r5, sb
00781f08: movle    r5, sl
00781f0c: str      r2, [sp, #0xac]
00781f10: strb     r3, [sp, #0xa0]
00781f14: str      r3, [sp, #0xa8]
00781f18: str      r3, [sp, #0x94]
00781f1c: str      r3, [sp, #0x98]
00781f20: str      r3, [sp, #0x9c]
00781f24: addgt    sl, sp, #0xa4
00781f28: addle    sb, sp, #0x94
00781f2c: cmp      r8, #0
00781f30: add      r7, r4, #0xbc
00781f34: ldr      r6, [r5, #4]
00781f38: bne      #0x7820d8
00781f3c: cmp      r8, r6
00781f40: ble      #0x781f64
00781f44: lsl      r3, r6, #2
00781f48: mov      r1, #0
00781f4c: ldr      r2, [r5]
00781f50: add      r6, r6, #1
00781f54: cmp      r6, r8
00781f58: str      r1, [r2, r3]
00781f5c: add      r3, r3, #4
00781f60: bne      #0x781f4c
00781f64: cmp      r8, #0
00781f68: str      r8, [r5, #4]
00781f6c: ble      #0x781f94
00781f70: mov      r3, #0
00781f74: ldr      r1, [r7]
00781f78: ldr      r2, [r5]
00781f7c: ldr      r1, [r1, r3, lsl #2]
00781f80: str      r1, [r2, r3, lsl #2]
00781f84: ldr      r2, [r5, #4]
00781f88: add      r3, r3, #1
00781f8c: cmp      r3, r2
00781f90: blt      #0x781f74
00781f94: ldr      r3, [r4, #0xc0]
00781f98: cmp      r3, #0
00781f9c: ble      #0x7820f4
00781fa0: mov      r6, #0
00781fa4: ldr      r3, [r4]
00781fa8: str      r6, [r4, #0xc0]
00781fac: mov      r0, r4
00781fb0: mov      lr, pc
00781fb4: ldr      pc, [r3, #0x58]
00781fb8: mov      r1, r5
00781fbc: bl       #0x75b810
00781fc0: ldr      r3, [sp, #0x98]
00781fc4: cmp      r3, r6
00781fc8: ble      #0x782118
00781fcc: mov      r5, #0
00781fd0: mov      r0, sb
00781fd4: mov      r1, r5
00781fd8: str      r5, [sp, #0x98]
00781fdc: bl       #0x77e7f8
00781fe0: ldr      r3, [sp, #0xa8]
00781fe4: cmp      r3, r5
00781fe8: ble      #0x782138
00781fec: mov      r3, #0
00781ff0: mov      r0, sl
00781ff4: mov      r1, r3
00781ff8: str      r3, [sp, #0xa8]
00781ffc: bl       #0x77e7f8
00782000: mov      r0, r4
00782004: bl       #0x75a240
00782008: ldr      r3, [r4, #0xfc]
0078200c: cmp      r3, #0
00782010: beq      #0x7820d0
00782014: mov      r0, r4
00782018: bl       #0x759c64
0078201c: ldr      r0, [r4, #0xfc]
00782020: mov      r3, #0
00782024: strb     r3, [sp, #0xcc]
00782028: cmp      r0, #0
0078202c: mov      r3, #5
00782030: strb     r3, [sp, #0xcd]
00782034: str      r0, [sp, #0xd0]
00782038: beq      #0x782040
0078203c: bl       #0x759c64
00782040: ldr      r3, [r4]
00782044: mov      r0, r4
00782048: mov      lr, pc
0078204c: ldr      pc, [r3, #0x58]
00782050: mov      r5, #0
00782054: mov      r3, #5
00782058: mov      sl, r0
0078205c: mov      r0, r4
00782060: strb     r3, [sp, #0xc1]
00782064: strb     r5, [sp, #0xc0]
00782068: str      r4, [sp, #0xc4]
0078206c: bl       #0x759c64
00782070: ldr      ip, [pc, #0xe0]
00782074: add      r7, sp, #0xb4
00782078: add      r8, sp, #0xcc
0078207c: add      r6, sp, #0xc0
00782080: add      ip, pc, ip
00782084: mov      r2, sl
00782088: mov      r1, r8
0078208c: mov      r3, r6
00782090: mov      r0, r7
00782094: str      ip, [sp, #8]
00782098: str      r5, [sp]
0078209c: str      r5, [sp, #4]
007820a0: bl       #0x7ba904
007820a4: mov      r0, r7
007820a8: bl       #0x797124
007820ac: mov      r0, r6
007820b0: bl       #0x797124
007820b4: mov      r0, r8
007820b8: bl       #0x797124
007820bc: add      r0, r4, #0xfc
007820c0: mov      r1, r5
007820c4: bl       #0x77e21c
007820c8: mov      r0, r4
007820cc: bl       #0x75a240
007820d0: add      sp, sp, #0xd8
007820d4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007820d8: ldr      r3, [r5, #8]
007820dc: cmp      r8, r3
007820e0: ble      #0x781f3c
007820e4: mov      r0, r5
007820e8: add      r1, r8, r8, asr #1
007820ec: bl       #0x77e7f8
007820f0: b        #0x781f3c
007820f4: bge      #0x781fa0
007820f8: lsl      r2, r3, #2
007820fc: mov      r0, #0
00782100: ldr      r1, [r7]
00782104: adds     r3, r3, #1
00782108: str      r0, [r1, r2]
0078210c: add      r2, r2, #4
00782110: bne      #0x782100
00782114: b        #0x781fa0
00782118: bge      #0x781fcc
0078211c: lsl      r2, r3, #2
00782120: ldr      r1, [sp, #0x94]
00782124: adds     r3, r3, #1
00782128: str      r6, [r1, r2]
0078212c: add      r2, r2, #4
00782130: bne      #0x782120
00782134: b        #0x781fcc
00782138: bge      #0x781fec
0078213c: lsl      r2, r3, #2
00782140: ldr      r1, [sp, #0xa4]
00782144: adds     r3, r3, #1
00782148: str      r5, [r1, r2]
0078214c: add      r2, r2, #4
00782150: bne      #0x782140
00782154: b        #0x781fec
00782158: ldrheq   fp, [r4], -r8
