
# _ZN7gameswf15sprite_instance7advanceEf
0078240c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00782410: ldrb     r4, [r0, #0xec]
00782414: sub      sp, sp, #0xc4
00782418: mov      r6, r0
0078241c: cmp      r4, #0
00782420: str      r1, [sp, #0xc]
00782424: beq      #0x78273c
00782428: ldrb     r3, [r6, #0x9b]
0078242c: cmp      r3, #0
00782430: bne      #0x782440
00782434: ldrb     r3, [r6, #0xec]
00782438: cmp      r3, #0
0078243c: bne      #0x7826c4
00782440: ldr      r3, [r6, #0xd0]
00782444: mov      r0, r6
00782448: cmp      r3, #0
0078244c: movle    r3, #0
00782450: movgt    r3, #1
00782454: strb     r3, [r6, #0x9d]
00782458: bl       #0x75e3a8
0078245c: ldr      r3, [r6, #0xd0]
00782460: cmp      r3, #0
00782464: ble      #0x782608
00782468: add      r3, sp, #0x10
0078246c: mov      r7, #0
00782470: str      r3, [sp, #8]
00782474: add      r3, sp, #0x90
00782478: add      r5, r6, #0xcc
0078247c: add      r8, sp, #0xa0
00782480: str      r3, [sp, #4]
00782484: mov      r4, r7
00782488: ldr      r0, [sp, #8]
0078248c: mov      r1, #0
00782490: mov      r2, #0x80
00782494: bl       #0x30e460
00782498: ldr      r3, [sp, #8]
0078249c: ldr      fp, [r6, #0xd0]
007824a0: ldr      sl, [sp, #4]
007824a4: str      r3, [sp, #0x90]
007824a8: mov      r3, #0x20
007824ac: cmp      fp, #0x1f
007824b0: str      r3, [sp, #0x98]
007824b4: mov      r3, #1
007824b8: movgt    sl, r8
007824bc: str      r4, [sp, #0x94]
007824c0: strb     r3, [sp, #0x9c]
007824c4: str      r4, [sp, #0xa0]
007824c8: str      r4, [sp, #0xa4]
007824cc: str      r4, [sp, #0xa8]
007824d0: strb     r4, [sp, #0xac]
007824d4: cmp      fp, #0
007824d8: ldr      sb, [sl, #4]
007824dc: beq      #0x7824ec
007824e0: ldr      r3, [sl, #8]
007824e4: cmp      fp, r3
007824e8: bgt      #0x7826cc
007824ec: cmp      fp, sb
007824f0: ble      #0x782510
007824f4: lsl      r3, sb, #2
007824f8: ldr      r2, [sl]
007824fc: add      sb, sb, #1
00782500: cmp      sb, fp
00782504: str      r4, [r2, r3]
00782508: add      r3, r3, #4
0078250c: bne      #0x7824f8
00782510: cmp      fp, #0
00782514: str      fp, [sl, #4]
00782518: ble      #0x782540
0078251c: mov      r3, #0
00782520: ldr      r1, [r5]
00782524: ldr      r2, [sl]
00782528: ldr      r1, [r1, r3, lsl #2]
0078252c: str      r1, [r2, r3, lsl #2]
00782530: ldr      r2, [sl, #4]
00782534: add      r3, r3, #1
00782538: cmp      r3, r2
0078253c: blt      #0x782520
00782540: ldr      r3, [r6, #0xd0]
00782544: cmp      r3, #0
00782548: ble      #0x7826dc
0078254c: ldr      r3, [r6]
00782550: str      r4, [r6, #0xd0]
00782554: mov      r0, r6
00782558: mov      lr, pc
0078255c: ldr      pc, [r3, #0x58]
00782560: mov      r1, sl
00782564: bl       #0x75b810
00782568: cmp      r7, #0xb
0078256c: beq      #0x7825bc
00782570: ldr      r3, [sp, #0xa4]
00782574: cmp      r3, #0
00782578: ble      #0x78271c
0078257c: mov      r0, r8
00782580: mov      r1, r4
00782584: str      r4, [sp, #0xa4]
00782588: bl       #0x77e7f8
0078258c: ldr      r3, [sp, #0x94]
00782590: cmp      r3, #0
00782594: ble      #0x7826fc
00782598: ldr      r0, [sp, #4]
0078259c: mov      r1, r4
007825a0: str      r4, [sp, #0x94]
007825a4: bl       #0x77e7f8
007825a8: ldr      r3, [r6, #0xd0]
007825ac: cmp      r3, #0
007825b0: ble      #0x782608
007825b4: add      r7, r7, #1
007825b8: b        #0x782488
007825bc: ldr      r0, [pc, #0x43c]
007825c0: add      r0, pc, r0
007825c4: bl       #0x7611f0
007825c8: ldr      r3, [sp, #0xa4]
007825cc: cmp      r3, #0
007825d0: ble      #0x78293c
007825d4: mov      r4, #0
007825d8: mov      r0, r8
007825dc: mov      r1, r4
007825e0: str      r4, [sp, #0xa4]
007825e4: bl       #0x77e7f8
007825e8: ldr      r3, [sp, #0x94]
007825ec: cmp      r3, r4
007825f0: ble      #0x78295c
007825f4: mov      r3, #0
007825f8: ldr      r0, [sp, #4]
007825fc: mov      r1, r3
00782600: str      r3, [sp, #0x94]
00782604: bl       #0x77e7f8
00782608: ldrsb    r4, [r6, #0xe6]
0078260c: cmp      r4, #0
00782610: bne      #0x78264c
00782614: ldr      r3, [r6, #0xa0]
00782618: ldrb     r5, [r6, #0x9d]
0078261c: mov      r0, r3
00782620: ldr      r3, [r3]
00782624: mov      lr, pc
00782628: ldr      pc, [r3, #0x38]
0078262c: ldrb     r3, [r6, #0xec]
00782630: cmp      r0, #1
00782634: movle    r0, r5
00782638: orrgt    r0, r5, #1
0078263c: strb     r0, [r6, #0x9d]
00782640: cmp      r3, #0
00782644: ldrh     r7, [r6, #0xe4]
00782648: bne      #0x782774
0078264c: add      r4, r6, #0xa8
00782650: ldrb     r3, [r6, #0xe9]
00782654: cmp      r3, #0
00782658: beq      #0x78269c
0078265c: ldrb     r3, [r6, #0xec]
00782660: cmp      r3, #0
00782664: beq      #0x782694
00782668: ldr      r3, [r6]
0078266c: mov      r2, #0
00782670: mov      r1, #0xc
00782674: ldr      r3, [r3, #0x2c]
00782678: mov      r0, r6
0078267c: strb     r1, [sp, #0xb0]
00782680: str      r2, [sp, #0xb4]
00782684: strb     r2, [sp, #0xb1]
00782688: strh     r2, [sp, #0xb2]
0078268c: add      r1, sp, #0xb0
00782690: blx      r3
00782694: mov      r3, #1
00782698: strb     r3, [r6, #0x9d]
0078269c: mov      r0, r6
007826a0: bl       #0x781eac
007826a4: mov      r0, r4
007826a8: ldr      r1, [sp, #0xc]
007826ac: bl       #0x755908
007826b0: cmp      r0, #0
007826b4: movne    r3, #1
007826b8: strbne   r3, [r6, #0x9d]
007826bc: mov      r3, #1
007826c0: strb     r3, [r6, #0xec]
007826c4: add      sp, sp, #0xc4
007826c8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007826cc: mov      r0, sl
007826d0: add      r1, fp, fp, asr #1
007826d4: bl       #0x77e7f8
007826d8: b        #0x7824ec
007826dc: bge      #0x78254c
007826e0: lsl      r2, r3, #2
007826e4: ldr      r1, [r5]
007826e8: adds     r3, r3, #1
007826ec: str      r4, [r1, r2]
007826f0: add      r2, r2, #4
007826f4: bne      #0x7826e4
007826f8: b        #0x78254c
007826fc: bge      #0x782598
00782700: lsl      r2, r3, #2
00782704: ldr      r1, [sp, #0x90]
00782708: adds     r3, r3, #1
0078270c: str      r4, [r1, r2]
00782710: add      r2, r2, #4
00782714: bne      #0x782704
00782718: b        #0x782598
0078271c: bge      #0x78257c
00782720: lsl      r2, r3, #2
00782724: ldr      r1, [sp, #0xa0]
00782728: adds     r3, r3, #1
0078272c: str      r4, [r1, r2]
00782730: add      r2, r2, #4
00782734: bne      #0x782724
00782738: b        #0x78257c
0078273c: ldr      r3, [r0]
00782740: mov      lr, pc
00782744: ldr      pc, [r3, #0x148]
00782748: ldr      r3, [r6]
0078274c: mov      r2, #0xa
00782750: mov      r0, r6
00782754: ldr      r3, [r3, #0x2c]
00782758: add      r1, sp, #0xb8
0078275c: strb     r2, [sp, #0xb8]
00782760: str      r4, [sp, #0xbc]
00782764: strb     r4, [sp, #0xb9]
00782768: strh     r4, [sp, #0xba]
0078276c: blx      r3
00782770: b        #0x782428
00782774: ldr      r3, [r6, #0xa0]
00782778: add      r5, r7, #1
0078277c: uxth     r5, r5
00782780: strh     r5, [r6, #0xe4]
00782784: mov      r0, r3
00782788: ldr      r3, [r3]
0078278c: mov      lr, pc
00782790: ldr      pc, [r3, #0x38]
00782794: sxth     r5, r5
00782798: cmp      r5, r0
0078279c: strhge   r4, [r6, #0xe4]
007827a0: ldrh     r5, [r6, #0xe4]
007827a4: cmp      r5, r7
007827a8: sxth     r1, r5
007827ac: beq      #0x78264c
007827b0: cmp      r5, #0
007827b4: beq      #0x7827dc
007827b8: add      r4, r6, #0xa8
007827bc: ldr      r3, [r6]
007827c0: mov      r0, r6
007827c4: mov      r2, #0
007827c8: mov      lr, pc
007827cc: ldr      pc, [r3, #0xc8]
007827d0: mov      r3, #1
007827d4: strb     r3, [r6, #0x9d]
007827d8: b        #0x782650
007827dc: ldr      r3, [r6, #0xa0]
007827e0: mov      r0, r3
007827e4: ldr      r3, [r3]
007827e8: mov      lr, pc
007827ec: ldr      pc, [r3, #0x38]
007827f0: cmp      r0, #1
007827f4: ble      #0x782990
007827f8: ldr      r3, [r6, #0xa0]
007827fc: mov      r1, r5
00782800: add      r7, sp, #0x10
00782804: mov      r0, r3
00782808: ldr      r3, [r3]
0078280c: mov      lr, pc
00782810: ldr      pc, [r3, #0x50]
00782814: mov      r1, r5
00782818: mov      r4, r0
0078281c: mov      r2, #0x80
00782820: mov      r0, r7
00782824: bl       #0x30e460
00782828: mov      r3, #0x20
0078282c: str      r3, [sp, #0xa8]
00782830: mov      r3, #1
00782834: str      r7, [sp, #0xa0]
00782838: strb     r3, [sp, #0xac]
0078283c: strb     r5, [sp, #0x9c]
00782840: str      r5, [sp, #0xa4]
00782844: str      r5, [sp, #0x90]
00782848: str      r5, [sp, #0x94]
0078284c: str      r5, [sp, #0x98]
00782850: ldr      r3, [r4, #4]
00782854: cmp      r3, #0x1f
00782858: bgt      #0x78297c
0078285c: cmp      r3, #0
00782860: ble      #0x7829cc
00782864: add      r8, sp, #0xa0
00782868: add      r3, sp, #0x90
0078286c: mov      r5, r8
00782870: str      r3, [sp, #4]
00782874: mov      r7, #0
00782878: ldr      r3, [r4]
0078287c: ldr      r3, [r3, r7, lsl #2]
00782880: add      r7, r7, #1
00782884: mov      r0, r3
00782888: ldr      r3, [r3]
0078288c: mov      lr, pc
00782890: ldr      pc, [r3, #0x1c]
00782894: ldr      r3, [r5, #4]
00782898: ldr      r2, [r5, #8]
0078289c: lsr      sb, r0, #0x10
007828a0: add      sl, r3, #1
007828a4: cmp      sl, r2
007828a8: bgt      #0x782928
007828ac: ldr      r2, [r5]
007828b0: str      sb, [r2, r3, lsl #2]
007828b4: str      sl, [r5, #4]
007828b8: ldr      r3, [r4, #4]
007828bc: cmp      r7, r3
007828c0: blt      #0x782878
007828c4: ldr      r3, [r5, #4]
007828c8: cmp      r3, #0
007828cc: ble      #0x782998
007828d0: add      r4, r6, #0xa8
007828d4: mov      r1, r5
007828d8: mov      r0, r4
007828dc: bl       #0x7563f8
007828e0: ldr      r3, [sp, #0x94]
007828e4: cmp      r3, #0
007828e8: ble      #0x7829a8
007828ec: mov      r5, #0
007828f0: ldr      r0, [sp, #4]
007828f4: mov      r1, r5
007828f8: str      r5, [sp, #0x94]
007828fc: bl       #0x7643c0
00782900: ldr      r3, [sp, #0xa4]
00782904: cmp      r3, r5
00782908: ble      #0x7829e0
0078290c: mov      r3, #0
00782910: mov      r1, r3
00782914: mov      r0, r8
00782918: str      r3, [sp, #0xa4]
0078291c: bl       #0x7643c0
00782920: ldrsh    r1, [r6, #0xe4]
00782924: b        #0x7827bc
00782928: mov      r0, r5
0078292c: add      r1, sl, sl, asr #1
00782930: bl       #0x7643c0
00782934: ldr      r3, [r5, #4]
00782938: b        #0x7828ac
0078293c: bge      #0x7825d4
00782940: lsl      r2, r3, #2
00782944: ldr      r1, [sp, #0xa0]
00782948: adds     r3, r3, #1
0078294c: str      r4, [r1, r2]
00782950: add      r2, r2, #4
00782954: bne      #0x782944
00782958: b        #0x7825d4
0078295c: bge      #0x7825f4
00782960: lsl      r2, r3, #2
00782964: ldr      r1, [sp, #0x90]
00782968: adds     r3, r3, #1
0078296c: str      r4, [r1, r2]
00782970: add      r2, r2, #4
00782974: bne      #0x782964
00782978: b        #0x7825f4
0078297c: add      r3, sp, #0x90
00782980: str      r3, [sp, #4]
00782984: mov      r5, r3
00782988: add      r8, sp, #0xa0
0078298c: b        #0x782874
00782990: ldrsh    r1, [r6, #0xe4]
00782994: b        #0x7827b8
00782998: add      r4, r6, #0xa8
0078299c: mov      r0, r4
007829a0: bl       #0x75647c
007829a4: b        #0x7828e0
007829a8: bge      #0x7828ec
007829ac: lsl      r2, r3, #2
007829b0: mov      r0, #0
007829b4: ldr      r1, [sp, #0x90]
007829b8: adds     r3, r3, #1
007829bc: str      r0, [r1, r2]
007829c0: add      r2, r2, #4
007829c4: bne      #0x7829b4
007829c8: b        #0x7828ec
007829cc: add      r8, sp, #0xa0
007829d0: add      r3, sp, #0x90
007829d4: mov      r5, r8
007829d8: str      r3, [sp, #4]
007829dc: b        #0x7828c4
007829e0: bge      #0x78290c
007829e4: lsl      r2, r3, #2
007829e8: ldr      r1, [sp, #0xa0]
007829ec: adds     r3, r3, #1
007829f0: str      r5, [r1, r2]
007829f4: add      r2, r2, #4
007829f8: bne      #0x7829e8
007829fc: b        #0x78290c
00782a00: andseq   r7, r8, r8, ror r6

# _ZN7gameswf15sprite_instance17notify_set_memberERKNS_10tu_stringiERKNS_8as_valueE
0077fe60: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0077fe64: ldrsb    r6, [r1]
0077fe68: mov      r5, r0
0077fe6c: mov      r4, r1
0077fe70: cmn      r6, #1
0077fe74: addne    r0, r1, #1
0077fe78: ldreq    r0, [r1, #0xc]
0077fe7c: ldr      r1, [pc, #0xb0]
0077fe80: sub      sp, sp, #0x10
0077fe84: add      r1, pc, r1
0077fe88: bl       #0x30e31c
0077fe8c: cmp      r0, #0
0077fe90: beq      #0x77ff10
0077fe94: ldr      r1, [pc, #0x9c]
0077fe98: cmn      r6, #1
0077fe9c: addne    r0, r4, #1
0077fea0: ldreq    r0, [r4, #0xc]
0077fea4: mov      r2, #2
0077fea8: add      r1, pc, r1
0077feac: bl       #0x30ec7c
0077feb0: subs     r6, r0, #0
0077feb4: bne      #0x77ff08
0077feb8: ldr      sl, [pc, #0x7c]
0077febc: add      sb, r4, #1
0077fec0: add      r8, sp, #4
0077fec4: add      sl, pc, sl
0077fec8: mov      r7, r6
0077fecc: ldrsb    r3, [r4]
0077fed0: strb     r7, [sp, #4]
0077fed4: strb     r7, [sp, #5]
0077fed8: cmn      r3, #1
0077fedc: ldr      r1, [sl, r6]
0077fee0: movne    r0, sb
0077fee4: ldreq    r0, [r4, #0xc]
0077fee8: bl       #0x751d10
0077feec: cmp      r0, #0
0077fef0: add      r6, r6, #4
0077fef4: mov      r0, r8
0077fef8: beq      #0x77ff24
0077fefc: bl       #0x797124
0077ff00: cmp      r6, #0x20
0077ff04: bne      #0x77fecc
0077ff08: add      sp, sp, #0x10
0077ff0c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0077ff10: mov      r3, #1
0077ff14: strb     r3, [r5, #0xe9]
0077ff18: mov      r0, r5
0077ff1c: bl       #0x7750e8
0077ff20: b        #0x77ff08
0077ff24: mov      r3, #1
0077ff28: strb     r3, [r5, #0x9c]
0077ff2c: bl       #0x797124
0077ff30: b        #0x77ff08
0077ff34: andseq   sb, r8, r4, asr sp
0077ff38: andseq   lr, r5, r8, lsl #12
0077ff3c: andseq   sl, sp, r8, lsl #21
