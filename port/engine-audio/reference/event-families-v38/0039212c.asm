# _ZN10GameObject12_PlaySound3DERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0039212c push     {r4, r5, r6, r7, r8, sb, sl, lr}
00392130 ldr      r4, [r0, #4]
00392134 mov      r7, r0
00392138 mov      sl, r2
0039213c ldm      r4, {r0, r3}
00392140 ldr      r6, [pc, #0x264]
00392144 sub      sp, sp, #0x28
00392148 rsb      r3, r0, r3
0039214c asr      r3, r3, #4
00392150 add      r6, pc, r6
00392154 add      r2, r3, r3, lsl #3
00392158 add      r2, r2, r2, lsl #6
0039215c add      r2, r3, r2, lsl #3
00392160 add      r2, r2, r2, lsl #15
00392164 add      r3, r3, r2, lsl #3
00392168 cmp      r3, #0
0039216c bne      #0x392180
00392170 ldr      r0, [pc, #0x238]
00392174 add      r0, pc, r0
00392178 bl       #0x708eb0
0039217c ldr      r0, [r4]
00392180 bl       #0x31c49c
00392184 ldr      r3, [pc, #0x228]
00392188 mov      r5, r0
0039218c ldr      r3, [r6, r3]
00392190 ldr      r4, [r3]
00392194 cmp      r4, #0
00392198 beq      #0x392260
0039219c ldr      r3, [pc, #0x214]
003921a0 mov      r8, #0
003921a4 ldr      r3, [r6, r3]
003921a8 ldr      sb, [r3]
003921ac b        #0x3921bc
003921b0 add      r8, r8, #1
003921b4 cmp      r8, r4
003921b8 beq      #0x392260
003921bc ldr      r1, [sb, r8, lsl #2]
003921c0 mov      r0, r5
003921c4 bl       #0x30e31c
003921c8 cmp      r0, #0
003921cc bne      #0x3921b0
003921d0 ldr      r2, [r7, #4]
003921d4 ldm      r2, {r1, r3}
003921d8 rsb      r3, r1, r3
003921dc asr      r3, r3, #4
003921e0 add      r2, r3, r3, lsl #3
003921e4 add      r2, r2, r2, lsl #6
003921e8 add      r2, r3, r2, lsl #3
003921ec add      r2, r2, r2, lsl #15
003921f0 add      r3, r3, r2, lsl #3
003921f4 rsb      r3, r3, #0
003921f8 cmp      r3, #3
003921fc bls      #0x392268
00392200 ldr      r3, [r1, #0x74]
00392204 cmp      r3, #3
00392208 beq      #0x3922a4
0039220c ldr      lr, [sl, #0x168]
00392210 ldr      r5, [sl, #0x160]
00392214 ldr      r4, [sl, #0x164]
00392218 ldr      r3, [pc, #0x19c]
0039221c mov      ip, #0xbf000000
00392220 add      ip, ip, #0x800000
00392224 ldr      r3, [r6, r3]
00392228 mov      r1, r8
0039222c add      r2, sp, #0x1c
00392230 ldr      r0, [r3]
00392234 str      r5, [sp, #0x1c]
00392238 mov      r3, #0
0039223c str      r4, [sp, #0x20]
00392240 str      lr, [sp, #0x24]
00392244 mov      lr, #1
00392248 str      lr, [sp]
0039224c str      ip, [sp, #8]
00392250 str      ip, [sp, #4]
00392254 bl       #0x36b5d8
00392258 add      sp, sp, #0x28
0039225c pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00392260 mvn      r8, #0
00392264 b        #0x3921d0
00392268 ldr      r3, [pc, #0x14c]
0039226c ldr      r4, [sl, #0x160]
00392270 ldr      r5, [sl, #0x164]
00392274 ldr      lr, [sl, #0x168]
00392278 ldr      r3, [r6, r3]
0039227c mov      ip, #0xbf000000
00392280 add      ip, ip, #0x800000
00392284 ldr      r0, [r3]
00392288 mov      r1, r8
0039228c add      r2, sp, #0x10
00392290 mov      r3, #0
00392294 str      r4, [sp, #0x10]
00392298 str      r5, [sp, #0x14]
0039229c str      lr, [sp, #0x18]
003922a0 b        #0x392244
003922a4 mov      r1, #2
003922a8 mov      r0, r7
003922ac bl       #0x37baf8
003922b0 ldr      r1, [r0, #4]
003922b4 cmp      r1, #3
003922b8 bne      #0x39220c
003922bc mov      r0, r7
003922c0 bl       #0x37baf8
003922c4 ldr      r3, [r0, #4]
003922c8 cmp      r3, #3
003922cc bne      #0x39220c
003922d0 ldr      r4, [r7, #4]
003922d4 movw     r3, #0x6db7
003922d8 movt     r3, #0xb6db
003922dc ldm      r4, {r0, r2}
003922e0 rsb      r2, r0, r2
003922e4 asr      r2, r2, #4
003922e8 mul      r3, r3, r2
003922ec cmp      r3, #1
003922f0 bhi      #0x392304
003922f4 ldr      r0, [pc, #0xc4]
003922f8 add      r0, pc, r0
003922fc bl       #0x708eb0
00392300 ldr      r0, [r4]
00392304 add      r0, r0, #0x70
00392308 bl       #0x31bbf0
0039230c ldr      r4, [r7, #4]
00392310 mov      r5, r0
00392314 ldm      r4, {r0, r3}
00392318 rsb      r3, r0, r3
0039231c asr      r3, r3, #4
00392320 add      r2, r3, r3, lsl #3
00392324 add      r2, r2, r2, lsl #6
00392328 add      r2, r3, r2, lsl #3
0039232c add      r2, r2, r2, lsl #15
00392330 add      r3, r3, r2, lsl #3
00392334 rsb      r3, r3, #0
00392338 cmp      r3, #2
0039233c bhi      #0x392350
00392340 ldr      r0, [pc, #0x7c]
00392344 add      r0, pc, r0
00392348 bl       #0x708eb0
0039234c ldr      r0, [r4]
00392350 add      r0, r0, #0xe0
00392354 bl       #0x31bbf0
00392358 ldr      r7, [r7, #4]
0039235c mov      r4, r0
00392360 ldm      r7, {r0, r3}
00392364 rsb      r3, r0, r3
00392368 asr      r3, r3, #4
0039236c add      r2, r3, r3, lsl #3
00392370 add      r2, r2, r2, lsl #6
00392374 add      r2, r3, r2, lsl #3
00392378 add      r2, r2, r2, lsl #15
0039237c add      r3, r3, r2, lsl #3
00392380 rsb      r3, r3, #0
00392384 cmp      r3, #3
00392388 bhi      #0x39239c
0039238c ldr      r0, [pc, #0x34]
00392390 add      r0, pc, r0
00392394 bl       #0x708eb0
00392398 ldr      r0, [r7]
0039239c add      r0, r0, #0x150
003923a0 bl       #0x31bbf0
003923a4 mov      lr, r0
003923a8 b        #0x392218
003923ac rsbeq    r2, r0, r0, asr #18
003923b0 ldrsheq  ip, [r2], #-0x24
003923b4 andeq    r3, r0, r8, lsr sp
003923b8 andeq    r3, r0, r8, lsr #19
003923bc andeq    r0, r0, r4, lsr #27
003923c0 subseq   ip, r2, r0, ror r1
003923c4 subseq   ip, r2, r4, lsr #2
003923c8 ldrsbeq  ip, [r2], #-8
