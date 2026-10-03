
# _ZN7PFFloor12_CreateNodesEPN6glitch4core10triangle3dIfEEj
00520588: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0052058c: ldr      r5, [r0, #0x20]
00520590: sub      sp, sp, #0x14c
00520594: mov      r4, r0
00520598: ands     r5, r5, #0x1000000
0052059c: str      r1, [sp, #0x2c]
005205a0: str      r2, [sp, #0x3c]
005205a4: bne      #0x520b08
005205a8: cmp      r2, #0
005205ac: beq      #0x520b08
005205b0: add      r2, r0, #0xc0
005205b4: add      r3, r0, #0xa8
005205b8: add      ip, sp, #0x124
005205bc: str      r2, [sp, #0x44]
005205c0: str      r3, [sp, #0x40]
005205c4: add      r2, sp, #0x118
005205c8: add      r3, sp, #0x130
005205cc: str      ip, [sp, #0x38]
005205d0: add      ip, sp, #0x10c
005205d4: str      r2, [sp, #0x34]
005205d8: str      r3, [sp, #0x1c]
005205dc: add      r2, sp, #0x144
005205e0: add      r3, sp, #0x140
005205e4: str      ip, [sp, #0x30]
005205e8: add      ip, sp, #0x13c
005205ec: str      r2, [sp, #0x54]
005205f0: str      r3, [sp, #0x58]
005205f4: str      ip, [sp, #0x5c]
005205f8: add      r2, sp, #0xd4
005205fc: add      r3, sp, #0x9c
00520600: add      ip, sp, #0x64
00520604: str      r5, [sp, #0x18]
00520608: str      r2, [sp, #0x48]
0052060c: str      r3, [sp, #0x4c]
00520610: str      ip, [sp, #0x50]
00520614: b        #0x520644
00520618: cmp      r6, #0
0052061c: beq      #0x520978
00520620: cmp      r8, #0
00520624: beq      #0x520a38
00520628: ldr      r2, [sp, #0x18]
0052062c: ldr      r3, [sp, #0x3c]
00520630: add      r5, r5, #0x24
00520634: add      r2, r2, #1
00520638: cmp      r2, r3
0052063c: str      r2, [sp, #0x18]
00520640: beq      #0x520b08
00520644: ldr      r2, [sp, #0x2c]
00520648: ldr      sl, [r2, r5]
0052064c: add      r6, r2, r5
00520650: ldr      r0, [r6, #0xc]
00520654: mov      r1, sl
00520658: bl       #0x30e3ac
0052065c: ldr      r8, [r6, #4]
00520660: mov      sb, r0
00520664: ldr      r0, [r6, #0x10]
00520668: mov      r1, r8
0052066c: bl       #0x30e3ac
00520670: str      r0, [sp, #0x20]
00520674: ldr      r7, [r6, #8]
00520678: ldr      r0, [r6, #0x14]
0052067c: mov      r1, r7
00520680: bl       #0x30e3ac
00520684: mov      r1, sl
00520688: mov      fp, r0
0052068c: ldr      r0, [r6, #0x18]
00520690: bl       #0x30e3ac
00520694: str      r0, [sp, #0x24]
00520698: ldr      r0, [r6, #0x1c]
0052069c: mov      r1, r8
005206a0: bl       #0x30e3ac
005206a4: str      r0, [sp, #0x28]
005206a8: ldr      r0, [r6, #0x20]
005206ac: mov      r1, r7
005206b0: bl       #0x30e3ac
005206b4: ldr      r3, [sp, #0x20]
005206b8: str      r0, [sp, #0xc]
005206bc: add      r1, r3, #0x80000000
005206c0: bl       #0x30ed6c
005206c4: ldr      r1, [sp, #0x28]
005206c8: mov      r3, r0
005206cc: mov      r0, fp
005206d0: str      r3, [sp, #0x10]
005206d4: bl       #0x30ed6c
005206d8: ldr      r3, [sp, #0x10]
005206dc: mov      r1, r0
005206e0: mov      r0, r3
005206e4: bl       #0x30eba4
005206e8: add      r1, fp, #0x80000000
005206ec: str      r0, [sp, #0x130]
005206f0: ldr      r0, [sp, #0x24]
005206f4: bl       #0x30ed6c
005206f8: ldr      r2, [sp, #0xc]
005206fc: mov      fp, r0
00520700: mov      r0, sb
00520704: mov      r1, r2
00520708: bl       #0x30ed6c
0052070c: mov      r1, r0
00520710: mov      r0, fp
00520714: bl       #0x30eba4
00520718: add      r1, sb, #0x80000000
0052071c: str      r0, [sp, #0x134]
00520720: ldr      r0, [sp, #0x28]
00520724: bl       #0x30ed6c
00520728: ldr      r1, [sp, #0x24]
0052072c: mov      sb, r0
00520730: ldr      r0, [sp, #0x20]
00520734: bl       #0x30ed6c
00520738: mov      r1, r0
0052073c: mov      r0, sb
00520740: bl       #0x30eba4
00520744: ldr      r1, [r6, #0xc]
00520748: ldr      r2, [r6, #0x10]
0052074c: ldr      r3, [r6, #0x14]
00520750: str      r0, [sp, #0x138]
00520754: str      r1, [sp, #0x118]
00520758: str      r2, [sp, #0x11c]
0052075c: str      r8, [sp, #0x128]
00520760: str      r7, [sp, #0x12c]
00520764: str      r3, [sp, #0x120]
00520768: str      sl, [sp, #0x124]
0052076c: ldr      ip, [r6, #0x20]
00520770: ldr      lr, [r6, #0x18]
00520774: ldr      r6, [r6, #0x1c]
00520778: ldr      r1, [sp, #0x38]
0052077c: ldr      r2, [sp, #0x34]
00520780: ldr      r3, [sp, #0x1c]
00520784: str      ip, [sp, #0x114]
00520788: mov      r0, r4
0052078c: mov      ip, #0
00520790: str      lr, [sp, #0x10c]
00520794: str      r6, [sp, #0x110]
00520798: str      ip, [sp]
0052079c: bl       #0x51fa64
005207a0: mov      ip, #0
005207a4: mov      r7, r0
005207a8: ldr      r1, [sp, #0x38]
005207ac: ldr      r2, [sp, #0x30]
005207b0: ldr      r3, [sp, #0x1c]
005207b4: mov      r0, r4
005207b8: str      ip, [sp]
005207bc: bl       #0x51fa64
005207c0: ldr      r3, [sp, #0x1c]
005207c4: mov      ip, #0
005207c8: mov      r6, r0
005207cc: ldr      r1, [sp, #0x34]
005207d0: ldr      r2, [sp, #0x30]
005207d4: mov      r0, r4
005207d8: str      ip, [sp]
005207dc: bl       #0x51fa64
005207e0: mov      r1, r7
005207e4: mov      r2, r6
005207e8: mov      r8, r0
005207ec: mov      r0, r4
005207f0: bl       #0x51e98c
005207f4: mov      r1, r6
005207f8: mov      r0, r4
005207fc: mov      r2, r7
00520800: bl       #0x51e98c
00520804: ldr      r1, [r4, #0xc4]
00520808: ldr      r3, [r4, #0xc8]
0052080c: str      r0, [sp, #0x144]
00520810: cmp      r1, r3
00520814: beq      #0x520b10
00520818: str      r0, [r1]
0052081c: ldr      r3, [r4, #0xc4]
00520820: add      r3, r3, #4
00520824: str      r3, [r4, #0xc4]
00520828: mov      r1, r7
0052082c: mov      r2, r8
00520830: mov      r0, r4
00520834: bl       #0x51e98c
00520838: mov      r1, r8
0052083c: mov      r0, r4
00520840: mov      r2, r7
00520844: bl       #0x51e98c
00520848: ldr      r1, [r4, #0xc4]
0052084c: ldr      r3, [r4, #0xc8]
00520850: str      r0, [sp, #0x140]
00520854: cmp      r1, r3
00520858: beq      #0x520b30
0052085c: str      r0, [r1]
00520860: ldr      r3, [r4, #0xc4]
00520864: add      r3, r3, #4
00520868: str      r3, [r4, #0xc4]
0052086c: mov      r1, r6
00520870: mov      r2, r8
00520874: mov      r0, r4
00520878: bl       #0x51e98c
0052087c: mov      r1, r8
00520880: mov      r0, r4
00520884: mov      r2, r6
00520888: bl       #0x51e98c
0052088c: ldr      r1, [r4, #0xc4]
00520890: ldr      r3, [r4, #0xc8]
00520894: str      r0, [sp, #0x13c]
00520898: cmp      r1, r3
0052089c: beq      #0x520b20
005208a0: str      r0, [r1]
005208a4: ldr      r3, [r4, #0xc4]
005208a8: add      r3, r3, #4
005208ac: str      r3, [r4, #0xc4]
005208b0: cmp      r7, #0
005208b4: bne      #0x520618
005208b8: ldr      r3, [sp, #0x118]
005208bc: ldr      sl, [sp, #0x124]
005208c0: ldr      sb, [sp, #0x128]
005208c4: mov      r1, r3
005208c8: mov      r0, sl
005208cc: str      r3, [sp, #0x10]
005208d0: bl       #0x30eba4
005208d4: mov      r1, #0x3f000000
005208d8: bl       #0x30ed6c
005208dc: ldr      r2, [sp, #0x11c]
005208e0: str      r0, [sp, #0xd4]
005208e4: mov      r0, sb
005208e8: mov      r1, r2
005208ec: str      r2, [sp, #0xc]
005208f0: bl       #0x30eba4
005208f4: mov      r1, #0x3f000000
005208f8: bl       #0x30ed6c
005208fc: ldr      ip, [sp, #0x120]
00520900: ldr      fp, [sp, #0x12c]
00520904: str      r0, [sp, #0xd8]
00520908: mov      r1, ip
0052090c: mov      r0, fp
00520910: str      ip, [sp, #0x14]
00520914: bl       #0x30eba4
00520918: mov      r1, #0x3f000000
0052091c: bl       #0x30ed6c
00520920: add      r2, sp, #0xc
00520924: ldm      r2, {r2, r3, ip}
00520928: str      r3, [sp, #0xec]
0052092c: ldr      r3, [sp, #0x130]
00520930: str      r0, [sp, #0xdc]
00520934: ldr      r1, [sp, #0x48]
00520938: str      r3, [sp, #0x100]
0052093c: ldr      r3, [sp, #0x134]
00520940: ldr      r0, [sp, #0x40]
00520944: str      sl, [sp, #0xe0]
00520948: str      r3, [sp, #0x104]
0052094c: ldr      r3, [sp, #0x138]
00520950: str      sb, [sp, #0xe4]
00520954: str      fp, [sp, #0xe8]
00520958: str      r2, [sp, #0xf0]
0052095c: str      ip, [sp, #0xf4]
00520960: str      r3, [sp, #0x108]
00520964: str      r6, [sp, #0xf8]
00520968: str      r8, [sp, #0xfc]
0052096c: bl       #0x51c9d0
00520970: cmp      r6, #0
00520974: bne      #0x520620
00520978: ldr      r3, [sp, #0x10c]
0052097c: ldr      sl, [sp, #0x124]
00520980: ldr      sb, [sp, #0x128]
00520984: mov      r1, r3
00520988: mov      r0, sl
0052098c: str      r3, [sp, #0x10]
00520990: bl       #0x30eba4
00520994: mov      r1, #0x3f000000
00520998: bl       #0x30ed6c
0052099c: ldr      r2, [sp, #0x110]
005209a0: str      r0, [sp, #0x9c]
005209a4: mov      r0, sb
005209a8: mov      r1, r2
005209ac: str      r2, [sp, #0xc]
005209b0: bl       #0x30eba4
005209b4: mov      r1, #0x3f000000
005209b8: bl       #0x30ed6c
005209bc: ldr      ip, [sp, #0x114]
005209c0: ldr      fp, [sp, #0x12c]
005209c4: str      r0, [sp, #0xa0]
005209c8: mov      r1, ip
005209cc: mov      r0, fp
005209d0: str      ip, [sp, #0x14]
005209d4: bl       #0x30eba4
005209d8: mov      r1, #0x3f000000
005209dc: bl       #0x30ed6c
005209e0: add      r2, sp, #0xc
005209e4: ldm      r2, {r2, r3, ip}
005209e8: str      r3, [sp, #0xb4]
005209ec: ldr      r3, [sp, #0x130]
005209f0: str      r0, [sp, #0xa4]
005209f4: ldr      r1, [sp, #0x4c]
005209f8: str      r3, [sp, #0xc8]
005209fc: ldr      r3, [sp, #0x134]
00520a00: ldr      r0, [sp, #0x40]
00520a04: str      sl, [sp, #0xa8]
00520a08: str      r3, [sp, #0xcc]
00520a0c: ldr      r3, [sp, #0x138]
00520a10: str      sb, [sp, #0xac]
00520a14: str      fp, [sp, #0xb0]
00520a18: str      r2, [sp, #0xb8]
00520a1c: str      ip, [sp, #0xbc]
00520a20: str      r3, [sp, #0xd0]
00520a24: str      r7, [sp, #0xc0]
00520a28: str      r8, [sp, #0xc4]
00520a2c: bl       #0x51c9d0
00520a30: cmp      r8, #0
00520a34: bne      #0x520628
00520a38: ldr      r8, [sp, #0x118]
00520a3c: ldr      fp, [sp, #0x10c]
00520a40: ldr      sl, [sp, #0x11c]
00520a44: mov      r0, r8
00520a48: mov      r1, fp
00520a4c: bl       #0x30eba4
00520a50: mov      r1, #0x3f000000
00520a54: bl       #0x30ed6c
00520a58: ldr      r3, [sp, #0x110]
00520a5c: str      r0, [sp, #0x64]
00520a60: mov      r0, sl
00520a64: mov      r1, r3
00520a68: str      r3, [sp, #0x10]
00520a6c: bl       #0x30eba4
00520a70: mov      r1, #0x3f000000
00520a74: bl       #0x30ed6c
00520a78: ldr      r2, [sp, #0x114]
00520a7c: ldr      sb, [sp, #0x120]
00520a80: str      r0, [sp, #0x68]
00520a84: mov      r1, r2
00520a88: mov      r0, sb
00520a8c: str      r2, [sp, #0xc]
00520a90: bl       #0x30eba4
00520a94: mov      r1, #0x3f000000
00520a98: bl       #0x30ed6c
00520a9c: ldr      r3, [sp, #0x10]
00520aa0: ldr      r2, [sp, #0xc]
00520aa4: str      r0, [sp, #0x6c]
00520aa8: str      r3, [sp, #0x80]
00520aac: ldr      r3, [sp, #0x130]
00520ab0: ldr      r1, [sp, #0x50]
00520ab4: ldr      r0, [sp, #0x40]
00520ab8: str      r3, [sp, #0x90]
00520abc: ldr      r3, [sp, #0x134]
00520ac0: str      r2, [sp, #0x84]
00520ac4: str      r8, [sp, #0x70]
00520ac8: str      r3, [sp, #0x94]
00520acc: ldr      r3, [sp, #0x138]
00520ad0: str      sl, [sp, #0x74]
00520ad4: str      sb, [sp, #0x78]
00520ad8: str      r3, [sp, #0x98]
00520adc: str      fp, [sp, #0x7c]
00520ae0: str      r7, [sp, #0x88]
00520ae4: str      r6, [sp, #0x8c]
00520ae8: bl       #0x51c9d0
00520aec: ldr      r2, [sp, #0x18]
00520af0: ldr      r3, [sp, #0x3c]
00520af4: add      r5, r5, #0x24
00520af8: add      r2, r2, #1
00520afc: cmp      r2, r3
00520b00: str      r2, [sp, #0x18]
00520b04: bne      #0x520644
00520b08: add      sp, sp, #0x14c
00520b0c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00520b10: ldr      r0, [sp, #0x44]
00520b14: ldr      r2, [sp, #0x54]
00520b18: bl       #0x51c834
00520b1c: b        #0x520828
00520b20: ldr      r0, [sp, #0x44]
00520b24: ldr      r2, [sp, #0x5c]
00520b28: bl       #0x51c834
00520b2c: b        #0x5208b0
00520b30: ldr      r0, [sp, #0x44]
00520b34: ldr      r2, [sp, #0x58]
00520b38: bl       #0x51c834
00520b3c: b        #0x52086c

# _ZN3sfc4math5graph4EdgeI12PFGInnerNodefE9setWeightEf
0051bc60: str      r1, [r0, #0xc]
0051bc64: bx       lr

# _ZN7Point3DIfE9normalizeEv
0034d0b0: push     {r4, r5, r6, r7, lr}
0034d0b4: mov      r4, r0
0034d0b8: ldr      r0, [r0]
0034d0bc: sub      sp, sp, #0xc
0034d0c0: ldr      r7, [r4, #4]
0034d0c4: mov      r1, r0
0034d0c8: bl       #0x30ed6c
0034d0cc: mov      r1, r7
0034d0d0: mov      r5, r0
0034d0d4: mov      r0, r7
0034d0d8: bl       #0x30ed6c
0034d0dc: mov      r1, r0
0034d0e0: mov      r0, r5
0034d0e4: bl       #0x30eba4
0034d0e8: ldr      r6, [r4, #8]
0034d0ec: mov      r5, r0
0034d0f0: mov      r1, r6
0034d0f4: mov      r0, r6
0034d0f8: bl       #0x30ed6c
0034d0fc: mov      r1, r0
0034d100: mov      r0, r5
0034d104: bl       #0x30eba4
0034d108: bl       #0x30e124
0034d10c: add      r1, sp, #8
0034d110: str      r0, [r1, #-4]!
0034d114: mov      r0, r4
0034d118: bl       #0x34d04c
0034d11c: add      sp, sp, #0xc
0034d120: pop      {r4, r5, r6, r7, pc}

# _ZNSt4priv8_Rb_treeI7CompPosSt4lessIS1_ESt4pairIKS1_P12PFGInnerNodeENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE13insert_uniqueENS_17_Rb_tree_iteratorIS8_SC_EERKS8_
0051efb8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0051efbc: ldr      r5, [r2]
0051efc0: ldr      r2, [r1, #8]
0051efc4: sub      sp, sp, #0x3c
0051efc8: mov      r6, r1
0051efcc: cmp      r5, r2
0051efd0: mov      r7, r0
0051efd4: mov      r8, r3
0051efd8: beq      #0x51f378
0051efdc: cmp      r5, r1
0051efe0: beq      #0x51f544
0051efe4: ldrb     r3, [r5]
0051efe8: cmp      r3, #0
0051efec: beq      #0x51f2c4
0051eff0: ldr      r4, [r5, #8]
0051eff4: cmp      r4, #0
0051eff8: bne      #0x51f004
0051effc: b        #0x51f2e4
0051f000: mov      r4, r3
0051f004: ldr      r3, [r4, #0xc]
0051f008: cmp      r3, #0
0051f00c: bne      #0x51f000
0051f010: ldr      sl, [r8]
0051f014: ldr      sb, [r5, #0x10]
0051f018: mov      r0, sl
0051f01c: mov      r1, sb
0051f020: bl       #0x30e3ac
0051f024: movw     r1, #0xb717
0051f028: bic      r0, r0, #0x80000000
0051f02c: movt     r1, #0x38d1
0051f030: bl       #0x30e70c
0051f034: cmp      r0, #0
0051f038: beq      #0x51f150
0051f03c: ldr      r3, [r5, #0x14]
0051f040: ldr      fp, [r8, #4]
0051f044: mov      r1, r3
0051f048: mov      r0, fp
0051f04c: str      r3, [sp, #0xc]
0051f050: bl       #0x30e3ac
0051f054: movw     r1, #0xb717
0051f058: bic      r0, r0, #0x80000000
0051f05c: movt     r1, #0x38d1
0051f060: bl       #0x30e70c
0051f064: cmp      r0, #0
0051f068: ldr      r3, [sp, #0xc]
0051f06c: beq      #0x51f334
0051f070: ldr      r0, [r8, #8]
0051f074: ldr      r1, [r5, #0x18]
0051f078: bl       #0x30e70c
0051f07c: cmp      r0, #0
0051f080: mov      fp, #0
0051f084: bne      #0x51f168
0051f088: uxtb     fp, fp
0051f08c: cmp      fp, #0
0051f090: beq      #0x51f178
0051f094: ldr      r3, [r4, #0x10]
0051f098: mov      r1, sl
0051f09c: mov      r0, r3
0051f0a0: str      r3, [sp, #0xc]
0051f0a4: bl       #0x30e3ac
0051f0a8: movw     r1, #0xb717
0051f0ac: bic      r0, r0, #0x80000000
0051f0b0: movt     r1, #0x38d1
0051f0b4: bl       #0x30e70c
0051f0b8: cmp      r0, #0
0051f0bc: ldr      r3, [sp, #0xc]
0051f0c0: beq      #0x51f314
0051f0c4: ldr      r3, [r4, #0x14]
0051f0c8: ldr      r2, [r8, #4]
0051f0cc: mov      r0, r3
0051f0d0: mov      r1, r2
0051f0d4: str      r2, [sp, #0x10]
0051f0d8: str      r3, [sp, #0xc]
0051f0dc: bl       #0x30e3ac
0051f0e0: movw     r1, #0xb717
0051f0e4: bic      r0, r0, #0x80000000
0051f0e8: movt     r1, #0x38d1
0051f0ec: bl       #0x30e70c
0051f0f0: cmp      r0, #0
0051f0f4: ldr      r2, [sp, #0x10]
0051f0f8: ldr      r3, [sp, #0xc]
0051f0fc: beq      #0x51f62c
0051f100: ldr      r0, [r4, #0x18]
0051f104: ldr      r1, [r8, #8]
0051f108: bl       #0x30e70c
0051f10c: cmp      r0, #0
0051f110: mov      r3, #0
0051f114: bne      #0x51f32c
0051f118: uxtb     r3, r3
0051f11c: cmp      r3, #0
0051f120: beq      #0x51f178
0051f124: ldr      ip, [r4, #0xc]
0051f128: cmp      ip, #0
0051f12c: beq      #0x51f6bc
0051f130: mov      ip, #0
0051f134: mov      r1, r6
0051f138: mov      r2, r5
0051f13c: mov      r3, r8
0051f140: mov      r0, r7
0051f144: stm      sp, {r5, ip}
0051f148: bl       #0x51ead4
0051f14c: b        #0x51f36c
0051f150: mov      r0, sl
0051f154: mov      r1, sb
0051f158: bl       #0x30e70c
0051f15c: cmp      r0, #0
0051f160: mov      fp, #0
0051f164: beq      #0x51f088
0051f168: mov      fp, #1
0051f16c: uxtb     fp, fp
0051f170: cmp      fp, #0
0051f174: bne      #0x51f094
0051f178: ldr      r3, [r5, #0xc]
0051f17c: cmp      r3, #0
0051f180: str      r3, [sp, #0x14]
0051f184: beq      #0x51f684
0051f188: mov      r4, r3
0051f18c: ldr      r3, [r3, #8]
0051f190: cmp      r3, #0
0051f194: bne      #0x51f188
0051f198: cmp      fp, #0
0051f19c: bne      #0x51f354
0051f1a0: mov      r1, sl
0051f1a4: mov      r0, sb
0051f1a8: bl       #0x30e3ac
0051f1ac: movw     r1, #0xb717
0051f1b0: bic      r0, r0, #0x80000000
0051f1b4: movt     r1, #0x38d1
0051f1b8: bl       #0x30e70c
0051f1bc: cmp      r0, #0
0051f1c0: beq      #0x51f608
0051f1c4: ldr      r3, [r8, #4]
0051f1c8: ldr      sb, [r5, #0x14]
0051f1cc: mov      r1, r3
0051f1d0: mov      r0, sb
0051f1d4: str      r3, [sp, #0xc]
0051f1d8: bl       #0x30e3ac
0051f1dc: movw     r1, #0xb717
0051f1e0: bic      r0, r0, #0x80000000
0051f1e4: movt     r1, #0x38d1
0051f1e8: bl       #0x30e70c
0051f1ec: cmp      r0, #0
0051f1f0: ldr      r3, [sp, #0xc]
0051f1f4: beq      #0x51f6f8
0051f1f8: ldr      r0, [r5, #0x18]
0051f1fc: ldr      r1, [r8, #8]
0051f200: bl       #0x30e70c
0051f204: cmp      r0, #0
0051f208: bne      #0x51f61c
0051f20c: uxtb     r3, fp
0051f210: cmp      r3, #0
0051f214: beq      #0x51f624
0051f218: cmp      r6, r4
0051f21c: beq      #0x51f298
0051f220: ldr      sb, [r4, #0x10]
0051f224: mov      r0, sl
0051f228: mov      r1, sb
0051f22c: bl       #0x30e3ac
0051f230: movw     r1, #0xb717
0051f234: bic      r0, r0, #0x80000000
0051f238: movt     r1, #0x38d1
0051f23c: bl       #0x30e70c
0051f240: cmp      r0, #0
0051f244: beq      #0x51f710
0051f248: ldr      sb, [r8, #4]
0051f24c: ldr      sl, [r4, #0x14]
0051f250: mov      r0, sb
0051f254: mov      r1, sl
0051f258: bl       #0x30e3ac
0051f25c: movw     r1, #0xb717
0051f260: bic      r0, r0, #0x80000000
0051f264: movt     r1, #0x38d1
0051f268: bl       #0x30e70c
0051f26c: cmp      r0, #0
0051f270: beq      #0x51f7bc
0051f274: ldr      r0, [r8, #8]
0051f278: ldr      r1, [r4, #0x18]
0051f27c: bl       #0x30e70c
0051f280: cmp      r0, #0
0051f284: mov      r3, #0
0051f288: bne      #0x51f728
0051f28c: uxtb     r3, r3
0051f290: cmp      r3, #0
0051f294: beq      #0x51f354
0051f298: ldr      ip, [sp, #0x14]
0051f29c: cmp      ip, #0
0051f2a0: beq      #0x51f784
0051f2a4: mov      ip, #0
0051f2a8: mov      r1, r6
0051f2ac: mov      r2, r4
0051f2b0: mov      r3, r8
0051f2b4: mov      r0, r7
0051f2b8: stm      sp, {r4, ip}
0051f2bc: bl       #0x51ead4
0051f2c0: b        #0x51f36c
0051f2c4: ldr      r3, [r5, #4]
0051f2c8: ldr      r3, [r3, #4]
0051f2cc: cmp      r5, r3
0051f2d0: ldreq    r4, [r5, #0xc]
0051f2d4: beq      #0x51f010
0051f2d8: ldr      r4, [r5, #8]
0051f2dc: cmp      r4, #0
0051f2e0: bne      #0x51f004
0051f2e4: ldr      r4, [r5, #4]
0051f2e8: ldr      r3, [r4, #8]
0051f2ec: cmp      r5, r3
0051f2f0: beq      #0x51f2fc
0051f2f4: b        #0x51f010
0051f2f8: mov      r4, r3
0051f2fc: ldr      r3, [r4, #4]
0051f300: ldr      r2, [r3, #8]
0051f304: cmp      r2, r4
0051f308: beq      #0x51f2f8
0051f30c: mov      r4, r3
0051f310: b        #0x51f010
0051f314: mov      r1, r3
0051f318: mov      r0, sl
0051f31c: bl       #0x30e2f8
0051f320: cmp      r0, #0
0051f324: mov      r3, #0
0051f328: beq      #0x51f118
0051f32c: mov      r3, #1
0051f330: b        #0x51f118
0051f334: mov      r0, fp
0051f338: mov      r1, r3
0051f33c: bl       #0x30e70c
0051f340: cmp      r0, #0
0051f344: mov      fp, #0
0051f348: beq      #0x51f088
0051f34c: mov      fp, #1
0051f350: b        #0x51f16c
0051f354: mov      r1, r6
0051f358: mov      r2, r8
0051f35c: add      r0, sp, #0x18
0051f360: bl       #0x51eca8
0051f364: ldr      r3, [sp, #0x18]
0051f368: str      r3, [r7]
0051f36c: mov      r0, r7
0051f370: add      sp, sp, #0x3c
0051f374: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0051f378: ldr      r3, [r1, #0x10]
0051f37c: cmp      r3, #0
0051f380: beq      #0x51f76c
0051f384: ldr      sb, [r8]
0051f388: ldr      r4, [r5, #0x10]
0051f38c: mov      r0, sb
0051f390: mov      r1, r4
0051f394: bl       #0x30e3ac
0051f398: movw     r1, #0xb717
0051f39c: bic      r0, r0, #0x80000000
0051f3a0: movt     r1, #0x38d1
0051f3a4: bl       #0x30e70c
0051f3a8: cmp      r0, #0
0051f3ac: beq      #0x51f664
0051f3b0: ldr      fp, [r8, #4]
0051f3b4: ldr      sl, [r5, #0x14]
0051f3b8: mov      r0, fp
0051f3bc: mov      r1, sl
0051f3c0: bl       #0x30e3ac
0051f3c4: movw     r1, #0xb717
0051f3c8: bic      r0, r0, #0x80000000
0051f3cc: movt     r1, #0x38d1
0051f3d0: bl       #0x30e70c
0051f3d4: cmp      r0, #0
0051f3d8: beq      #0x51f730
0051f3dc: ldr      r0, [r8, #8]
0051f3e0: ldr      r1, [r5, #0x18]
0051f3e4: bl       #0x30e70c
0051f3e8: cmp      r0, #0
0051f3ec: mov      sl, #0
0051f3f0: bne      #0x51f67c
0051f3f4: uxtb     sl, sl
0051f3f8: cmp      sl, #0
0051f3fc: bne      #0x51f130
0051f400: mov      r1, sb
0051f404: mov      r0, r4
0051f408: bl       #0x30e3ac
0051f40c: movw     r1, #0xb717
0051f410: bic      r0, r0, #0x80000000
0051f414: movt     r1, #0x38d1
0051f418: bl       #0x30e70c
0051f41c: cmp      r0, #0
0051f420: beq      #0x51f6dc
0051f424: ldr      fp, [r5, #0x14]
0051f428: ldr      r4, [r8, #4]
0051f42c: mov      r0, fp
0051f430: mov      r1, r4
0051f434: bl       #0x30e3ac
0051f438: movw     r1, #0xb717
0051f43c: bic      r0, r0, #0x80000000
0051f440: movt     r1, #0x38d1
0051f444: bl       #0x30e70c
0051f448: cmp      r0, #0
0051f44c: beq      #0x51f7a4
0051f450: ldr      r0, [r5, #0x18]
0051f454: ldr      r1, [r8, #8]
0051f458: bl       #0x30e70c
0051f45c: cmp      r0, #0
0051f460: bne      #0x51f6f0
0051f464: uxtb     r3, sl
0051f468: cmp      r3, #0
0051f46c: beq      #0x51f624
0051f470: ldr      sl, [r5, #0xc]
0051f474: cmp      sl, #0
0051f478: beq      #0x51f810
0051f47c: mov      r4, sl
0051f480: b        #0x51f488
0051f484: mov      r4, r3
0051f488: ldr      r3, [r4, #8]
0051f48c: cmp      r3, #0
0051f490: bne      #0x51f484
0051f494: cmp      r6, r4
0051f498: moveq    r1, r6
0051f49c: moveq    r2, r5
0051f4a0: beq      #0x51f5cc
0051f4a4: ldr      fp, [r4, #0x10]
0051f4a8: mov      r0, sb
0051f4ac: mov      r1, fp
0051f4b0: bl       #0x30e3ac
0051f4b4: movw     r1, #0xb717
0051f4b8: bic      r0, r0, #0x80000000
0051f4bc: movt     r1, #0x38d1
0051f4c0: bl       #0x30e70c
0051f4c4: cmp      r0, #0
0051f4c8: beq      #0x51f74c
0051f4cc: ldr      fp, [r8, #4]
0051f4d0: ldr      sb, [r4, #0x14]
0051f4d4: mov      r0, fp
0051f4d8: mov      r1, sb
0051f4dc: bl       #0x30e3ac
0051f4e0: movw     r1, #0xb717
0051f4e4: bic      r0, r0, #0x80000000
0051f4e8: movt     r1, #0x38d1
0051f4ec: bl       #0x30e70c
0051f4f0: cmp      r0, #0
0051f4f4: beq      #0x51f7d8
0051f4f8: ldr      r0, [r8, #8]
0051f4fc: ldr      r1, [r4, #0x18]
0051f500: bl       #0x30e70c
0051f504: cmp      r0, #0
0051f508: mov      r3, #0
0051f50c: bne      #0x51f764
0051f510: uxtb     r3, r3
0051f514: cmp      r3, #0
0051f518: beq      #0x51f7f4
0051f51c: cmp      sl, #0
0051f520: bne      #0x51f2a4
0051f524: mov      r1, r6
0051f528: mov      r2, r5
0051f52c: mov      r3, r8
0051f530: mov      r0, r7
0051f534: str      sl, [sp]
0051f538: str      r5, [sp, #4]
0051f53c: bl       #0x51ead4
0051f540: b        #0x51f36c
0051f544: ldr      r4, [r5, #0xc]
0051f548: ldr      sl, [r3]
0051f54c: ldr      sb, [r4, #0x10]
0051f550: mov      r1, sl
0051f554: mov      r0, sb
0051f558: bl       #0x30e3ac
0051f55c: movw     r1, #0xb717
0051f560: bic      r0, r0, #0x80000000
0051f564: movt     r1, #0x38d1
0051f568: bl       #0x30e70c
0051f56c: cmp      r0, #0
0051f570: beq      #0x51f5e8
0051f574: ldr      sb, [r4, #0x14]
0051f578: ldr      sl, [r8, #4]
0051f57c: mov      r0, sb
0051f580: mov      r1, sl
0051f584: bl       #0x30e3ac
0051f588: movw     r1, #0xb717
0051f58c: bic      r0, r0, #0x80000000
0051f590: movt     r1, #0x38d1
0051f594: bl       #0x30e70c
0051f598: cmp      r0, #0
0051f59c: beq      #0x51f5e8
0051f5a0: ldr      r0, [r4, #0x18]
0051f5a4: ldr      r1, [r8, #8]
0051f5a8: bl       #0x30e70c
0051f5ac: cmp      r0, #0
0051f5b0: mov      r3, #0
0051f5b4: bne      #0x51f600
0051f5b8: uxtb     r3, r3
0051f5bc: cmp      r3, #0
0051f5c0: beq      #0x51f648
0051f5c4: mov      r1, r6
0051f5c8: mov      r2, r4
0051f5cc: mov      ip, #0
0051f5d0: mov      r3, r8
0051f5d4: mov      r0, r7
0051f5d8: str      ip, [sp]
0051f5dc: str      r5, [sp, #4]
0051f5e0: bl       #0x51ead4
0051f5e4: b        #0x51f36c
0051f5e8: mov      r0, sb
0051f5ec: mov      r1, sl
0051f5f0: bl       #0x30e70c
0051f5f4: cmp      r0, #0
0051f5f8: mov      r3, #0
0051f5fc: beq      #0x51f5b8
0051f600: mov      r3, #1
0051f604: b        #0x51f5b8
0051f608: mov      r1, sb
0051f60c: mov      r0, sl
0051f610: bl       #0x30e2f8
0051f614: cmp      r0, #0
0051f618: beq      #0x51f20c
0051f61c: mov      fp, #1
0051f620: b        #0x51f20c
0051f624: str      r5, [r7]
0051f628: b        #0x51f36c
0051f62c: mov      r0, r3
0051f630: mov      r1, r2
0051f634: bl       #0x30e70c
0051f638: cmp      r0, #0
0051f63c: mov      r3, #0
0051f640: movne    r3, #1
0051f644: b        #0x51f118
0051f648: mov      r1, r6
0051f64c: mov      r2, r8
0051f650: add      r0, sp, #0x20
0051f654: bl       #0x51eca8
0051f658: ldr      r3, [sp, #0x20]
0051f65c: str      r3, [r7]
0051f660: b        #0x51f36c
0051f664: mov      r0, sb
0051f668: mov      r1, r4
0051f66c: bl       #0x30e70c
0051f670: cmp      r0, #0
0051f674: mov      sl, #0
0051f678: beq      #0x51f3f4
0051f67c: mov      sl, #1
0051f680: b        #0x51f3f4
0051f684: ldr      r3, [r5, #4]
0051f688: ldr      r2, [r3, #0xc]
0051f68c: cmp      r5, r2
0051f690: movne    r4, r5
0051f694: bne      #0x51f6ac
0051f698: mov      r4, r3
0051f69c: ldr      r3, [r3, #4]
0051f6a0: ldr      r2, [r3, #0xc]
0051f6a4: cmp      r4, r2
0051f6a8: beq      #0x51f698
0051f6ac: ldr      r2, [r4, #0xc]
0051f6b0: cmp      r3, r2
0051f6b4: movne    r4, r3
0051f6b8: b        #0x51f198
0051f6bc: mov      r1, r6
0051f6c0: mov      r2, r4
0051f6c4: mov      r3, r8
0051f6c8: mov      r0, r7
0051f6cc: str      ip, [sp]
0051f6d0: str      r4, [sp, #4]
0051f6d4: bl       #0x51ead4
0051f6d8: b        #0x51f36c
0051f6dc: mov      r1, r4
0051f6e0: mov      r0, sb
0051f6e4: bl       #0x30e2f8
0051f6e8: cmp      r0, #0
0051f6ec: beq      #0x51f464
0051f6f0: mov      sl, #1
0051f6f4: b        #0x51f464
0051f6f8: mov      r0, sb
0051f6fc: mov      r1, r3
0051f700: bl       #0x30e70c
0051f704: cmp      r0, #0
0051f708: movne    fp, #1
0051f70c: b        #0x51f20c
0051f710: mov      r0, sl
0051f714: mov      r1, sb
0051f718: bl       #0x30e70c
0051f71c: cmp      r0, #0
0051f720: mov      r3, #0
0051f724: beq      #0x51f28c
0051f728: mov      r3, #1
0051f72c: b        #0x51f28c
0051f730: mov      r1, sl
0051f734: mov      r0, fp
0051f738: bl       #0x30e70c
0051f73c: cmp      r0, #0
0051f740: mov      sl, #0
0051f744: movne    sl, #1
0051f748: b        #0x51f3f4
0051f74c: mov      r0, sb
0051f750: mov      r1, fp
0051f754: bl       #0x30e70c
0051f758: cmp      r0, #0
0051f75c: mov      r3, #0
0051f760: beq      #0x51f510
0051f764: mov      r3, #1
0051f768: b        #0x51f510
0051f76c: mov      r2, r8
0051f770: add      r0, sp, #0x30
0051f774: bl       #0x51eca8
0051f778: ldr      r3, [sp, #0x30]
0051f77c: str      r3, [r7]
0051f780: b        #0x51f36c
0051f784: mov      r1, r6
0051f788: mov      r2, r5
0051f78c: mov      r3, r8
0051f790: mov      r0, r7
0051f794: str      ip, [sp]
0051f798: str      r5, [sp, #4]
0051f79c: bl       #0x51ead4
0051f7a0: b        #0x51f36c
0051f7a4: mov      r0, fp
0051f7a8: mov      r1, r4
0051f7ac: bl       #0x30e70c
0051f7b0: cmp      r0, #0
0051f7b4: movne    sl, #1
0051f7b8: b        #0x51f464
0051f7bc: mov      r0, sb
0051f7c0: mov      r1, sl
0051f7c4: bl       #0x30e70c
0051f7c8: cmp      r0, #0
0051f7cc: mov      r3, #0
0051f7d0: movne    r3, #1
0051f7d4: b        #0x51f28c
0051f7d8: mov      r0, fp
0051f7dc: mov      r1, sb
0051f7e0: bl       #0x30e70c
0051f7e4: cmp      r0, #0
0051f7e8: mov      r3, #0
0051f7ec: movne    r3, #1
0051f7f0: b        #0x51f510
0051f7f4: mov      r1, r6
0051f7f8: mov      r2, r8
0051f7fc: add      r0, sp, #0x28
0051f800: bl       #0x51eca8
0051f804: ldr      r3, [sp, #0x28]
0051f808: str      r3, [r7]
0051f80c: b        #0x51f36c
0051f810: ldr      r3, [r5, #4]
0051f814: ldr      r2, [r3, #0xc]
0051f818: cmp      r5, r2
0051f81c: movne    r4, r5
0051f820: bne      #0x51f838
0051f824: mov      r4, r3
0051f828: ldr      r3, [r3, #4]
0051f82c: ldr      r2, [r3, #0xc]
0051f830: cmp      r4, r2
0051f834: beq      #0x51f824
0051f838: ldr      r2, [r4, #0xc]
0051f83c: cmp      r3, r2
0051f840: movne    r4, r3
0051f844: b        #0x51f494

# _ZNSt3mapI7CompPosP12PFGInnerNodeSt4lessIS0_ESaISt4pairIKS0_S2_EEEixIS0_EERS2_RKT_
0051f848: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0051f84c: ldr      r4, [r0, #4]
0051f850: sub      sp, sp, #0x18
0051f854: mov      r8, r0
0051f858: cmp      r4, #0
0051f85c: mov      sl, r1
0051f860: beq      #0x51fa58
0051f864: ldr      r5, [r1]
0051f868: mov      r7, r0
0051f86c: ldr      r6, [r4, #0x10]
0051f870: mov      r1, r5
0051f874: mov      r0, r6
0051f878: bl       #0x30e3ac
0051f87c: movw     r1, #0xb717
0051f880: bic      r0, r0, #0x80000000
0051f884: movt     r1, #0x38d1
0051f888: bl       #0x30e70c
0051f88c: cmp      r0, #0
0051f890: beq      #0x51f900
0051f894: ldr      sb, [r4, #0x14]
0051f898: ldr      r6, [sl, #4]
0051f89c: mov      r0, sb
0051f8a0: mov      r1, r6
0051f8a4: bl       #0x30e3ac
0051f8a8: movw     r1, #0xb717
0051f8ac: bic      r0, r0, #0x80000000
0051f8b0: movt     r1, #0x38d1
0051f8b4: bl       #0x30e70c
0051f8b8: cmp      r0, #0
0051f8bc: beq      #0x51fa00
0051f8c0: ldr      r0, [r4, #0x18]
0051f8c4: ldr      r1, [sl, #8]
0051f8c8: bl       #0x30e70c
0051f8cc: cmp      r0, #0
0051f8d0: mov      r3, #0
0051f8d4: bne      #0x51f918
0051f8d8: uxtb     r3, r3
0051f8dc: cmp      r3, #0
0051f8e0: ldrne    r3, [r4, #0xc]
0051f8e4: ldreq    r3, [r4, #8]
0051f8e8: movne    r4, r7
0051f8ec: cmp      r3, #0
0051f8f0: beq      #0x51f938
0051f8f4: mov      r7, r4
0051f8f8: mov      r4, r3
0051f8fc: b        #0x51f86c
0051f900: mov      r0, r6
0051f904: mov      r1, r5
0051f908: bl       #0x30e70c
0051f90c: cmp      r0, #0
0051f910: mov      r3, #0
0051f914: beq      #0x51f8d8
0051f918: mov      r3, #1
0051f91c: uxtb     r3, r3
0051f920: cmp      r3, #0
0051f924: ldrne    r3, [r4, #0xc]
0051f928: ldreq    r3, [r4, #8]
0051f92c: movne    r4, r7
0051f930: cmp      r3, #0
0051f934: bne      #0x51f8f4
0051f938: cmp      r8, r4
0051f93c: beq      #0x51f9bc
0051f940: ldr      r7, [r4, #0x10]
0051f944: mov      r0, r5
0051f948: mov      r6, r4
0051f94c: mov      r1, r7
0051f950: bl       #0x30e3ac
0051f954: movw     r1, #0xb717
0051f958: bic      r0, r0, #0x80000000
0051f95c: movt     r1, #0x38d1
0051f960: bl       #0x30e70c
0051f964: cmp      r0, #0
0051f968: beq      #0x51fa20
0051f96c: ldr      sb, [sl, #4]
0051f970: ldr      r7, [r4, #0x14]
0051f974: mov      r0, sb
0051f978: mov      r1, r7
0051f97c: bl       #0x30e3ac
0051f980: movw     r1, #0xb717
0051f984: bic      r0, r0, #0x80000000
0051f988: movt     r1, #0x38d1
0051f98c: bl       #0x30e70c
0051f990: cmp      r0, #0
0051f994: beq      #0x51fa3c
0051f998: ldr      r0, [sl, #8]
0051f99c: ldr      r1, [r4, #0x18]
0051f9a0: bl       #0x30e70c
0051f9a4: cmp      r0, #0
0051f9a8: mov      r3, #0
0051f9ac: movne    r3, #1
0051f9b0: uxtb     r3, r3
0051f9b4: cmp      r3, #0
0051f9b8: beq      #0x51f9f4
0051f9bc: ldr      ip, [sl, #8]
0051f9c0: ldr      lr, [sl, #4]
0051f9c4: mov      r1, r8
0051f9c8: str      ip, [sp, #8]
0051f9cc: add      r0, sp, #0x14
0051f9d0: mov      ip, #0
0051f9d4: add      r2, sp, #0x10
0051f9d8: mov      r3, sp
0051f9dc: str      r5, [sp]
0051f9e0: str      lr, [sp, #4]
0051f9e4: str      ip, [sp, #0xc]
0051f9e8: str      r4, [sp, #0x10]
0051f9ec: bl       #0x51efb8
0051f9f0: ldr      r6, [sp, #0x14]
0051f9f4: add      r0, r6, #0x1c
0051f9f8: add      sp, sp, #0x18
0051f9fc: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0051fa00: mov      r0, sb
0051fa04: mov      r1, r6
0051fa08: bl       #0x30e70c
0051fa0c: cmp      r0, #0
0051fa10: mov      r3, #0
0051fa14: beq      #0x51f8d8
0051fa18: mov      r3, #1
0051fa1c: b        #0x51f91c
0051fa20: mov      r1, r7
0051fa24: mov      r0, r5
0051fa28: bl       #0x30e70c
0051fa2c: cmp      r0, #0
0051fa30: mov      r3, #0
0051fa34: movne    r3, #1
0051fa38: b        #0x51f9b0
0051fa3c: mov      r0, sb
0051fa40: mov      r1, r7
0051fa44: bl       #0x30e70c
0051fa48: cmp      r0, #0
0051fa4c: mov      r3, #0
0051fa50: movne    r3, #1
0051fa54: b        #0x51f9b0
0051fa58: ldr      r5, [r1]
0051fa5c: mov      r4, r0
0051fa60: b        #0x51f938

# _ZNSt6vectorIN7PFFloor11InvalidNodeESaIS1_EE9push_backERKS1_
0051c9d0: push     {r4, r5, r6, r7, lr}
0051c9d4: ldmib    r0, {r3, r6}
0051c9d8: sub      sp, sp, #0xc
0051c9dc: mov      r5, r0
0051c9e0: cmp      r3, r6
0051c9e4: mov      r4, r1
0051c9e8: beq      #0x51ca70
0051c9ec: ldr      r2, [r1]
0051c9f0: str      r2, [r3]
0051c9f4: ldr      r2, [r1, #4]
0051c9f8: str      r2, [r3, #4]
0051c9fc: ldr      r2, [r1, #8]
0051ca00: str      r2, [r3, #8]
0051ca04: ldr      r2, [r1, #0xc]
0051ca08: str      r2, [r3, #0xc]
0051ca0c: ldr      r2, [r1, #0x10]
0051ca10: str      r2, [r3, #0x10]
0051ca14: ldr      r2, [r1, #0x14]
0051ca18: str      r2, [r3, #0x14]
0051ca1c: ldr      r2, [r1, #0x18]
0051ca20: str      r2, [r3, #0x18]
0051ca24: ldr      r2, [r1, #0x1c]
0051ca28: str      r2, [r3, #0x1c]
0051ca2c: ldr      r2, [r1, #0x20]
0051ca30: str      r2, [r3, #0x20]
0051ca34: ldr      r2, [r1, #0x24]
0051ca38: str      r2, [r3, #0x24]
0051ca3c: ldr      r2, [r1, #0x28]
0051ca40: str      r2, [r3, #0x28]
0051ca44: ldr      r2, [r1, #0x2c]
0051ca48: str      r2, [r3, #0x2c]
0051ca4c: ldr      r2, [r1, #0x30]
0051ca50: str      r2, [r3, #0x30]
0051ca54: ldr      r2, [r1, #0x34]
0051ca58: str      r2, [r3, #0x34]
0051ca5c: ldr      r3, [r0, #4]
0051ca60: add      r3, r3, #0x38
0051ca64: str      r3, [r0, #4]
0051ca68: add      sp, sp, #0xc
0051ca6c: pop      {r4, r5, r6, r7, pc}
0051ca70: ldr      r2, [r0]
0051ca74: movw     r3, #0x4924
0051ca78: orr      r3, r3, r3, lsl #12
0051ca7c: rsb      r2, r2, r6
0051ca80: asr      r2, r2, #3
0051ca84: add      r1, r2, r2, lsl #3
0051ca88: add      r1, r1, r1, lsl #6
0051ca8c: add      r1, r2, r1, lsl #3
0051ca90: add      r1, r1, r1, lsl #15
0051ca94: add      r2, r2, r1, lsl #3
0051ca98: rsb      r2, r2, #0
0051ca9c: cmp      r2, #1
0051caa0: addhs    r1, r2, r2
0051caa4: addlo    r1, r2, #1
0051caa8: cmp      r1, r3
0051caac: bls      #0x51cc90
0051cab0: movw     r1, #0x4924
0051cab4: orr      r1, r1, r1, lsl #12
0051cab8: add      r2, sp, #8
0051cabc: str      r1, [r2, #-4]!
0051cac0: add      r0, r5, #8
0051cac4: bl       #0x51c6cc
0051cac8: ldr      r3, [r5]
0051cacc: mov      r7, r0
0051cad0: rsb      r6, r3, r6
0051cad4: asr      r6, r6, #3
0051cad8: add      ip, r6, r6, lsl #3
0051cadc: add      ip, ip, ip, lsl #6
0051cae0: add      ip, r6, ip, lsl #3
0051cae4: add      ip, ip, ip, lsl #15
0051cae8: add      ip, r6, ip, lsl #3
0051caec: rsb      ip, ip, #0
0051caf0: cmp      ip, #0
0051caf4: movle    ip, r0
0051caf8: ble      #0x51cb8c
0051cafc: mov      r1, ip
0051cb00: mov      r2, r0
0051cb04: ldr      r0, [r3]
0051cb08: subs     r1, r1, #1
0051cb0c: str      r0, [r2]
0051cb10: ldr      r0, [r3, #4]
0051cb14: str      r0, [r2, #4]
0051cb18: ldr      r0, [r3, #8]
0051cb1c: str      r0, [r2, #8]
0051cb20: ldr      r0, [r3, #0xc]
0051cb24: str      r0, [r2, #0xc]
0051cb28: ldr      r0, [r3, #0x10]
0051cb2c: str      r0, [r2, #0x10]
0051cb30: ldr      r0, [r3, #0x14]
0051cb34: str      r0, [r2, #0x14]
0051cb38: ldr      r0, [r3, #0x18]
0051cb3c: str      r0, [r2, #0x18]
0051cb40: ldr      r0, [r3, #0x1c]
0051cb44: str      r0, [r2, #0x1c]
0051cb48: ldr      r0, [r3, #0x20]
0051cb4c: str      r0, [r2, #0x20]
0051cb50: ldr      r0, [r3, #0x24]
0051cb54: str      r0, [r2, #0x24]
0051cb58: ldr      r0, [r3, #0x28]
0051cb5c: str      r0, [r2, #0x28]
0051cb60: ldr      r0, [r3, #0x2c]
0051cb64: str      r0, [r2, #0x2c]
0051cb68: ldr      r0, [r3, #0x30]
0051cb6c: str      r0, [r2, #0x30]
0051cb70: ldr      r0, [r3, #0x34]
0051cb74: add      r3, r3, #0x38
0051cb78: str      r0, [r2, #0x34]
0051cb7c: add      r2, r2, #0x38
0051cb80: bne      #0x51cb04
0051cb84: mov      r3, #0x38
0051cb88: mla      ip, r3, ip, r7
0051cb8c: ldr      r3, [r4]
0051cb90: add      r6, ip, #0x38
0051cb94: str      r3, [ip]
0051cb98: ldr      r3, [r4, #4]
0051cb9c: str      r3, [ip, #4]
0051cba0: ldr      r3, [r4, #8]
0051cba4: str      r3, [ip, #8]
0051cba8: ldr      r3, [r4, #0xc]
0051cbac: str      r3, [ip, #0xc]
0051cbb0: ldr      r3, [r4, #0x10]
0051cbb4: str      r3, [ip, #0x10]
0051cbb8: ldr      r3, [r4, #0x14]
0051cbbc: str      r3, [ip, #0x14]
0051cbc0: ldr      r3, [r4, #0x18]
0051cbc4: str      r3, [ip, #0x18]
0051cbc8: ldr      r3, [r4, #0x1c]
0051cbcc: str      r3, [ip, #0x1c]
0051cbd0: ldr      r3, [r4, #0x20]
0051cbd4: str      r3, [ip, #0x20]
0051cbd8: ldr      r3, [r4, #0x24]
0051cbdc: str      r3, [ip, #0x24]
0051cbe0: ldr      r3, [r4, #0x28]
0051cbe4: str      r3, [ip, #0x28]
0051cbe8: ldr      r3, [r4, #0x2c]
0051cbec: str      r3, [ip, #0x2c]
0051cbf0: ldr      r3, [r4, #0x30]
0051cbf4: str      r3, [ip, #0x30]
0051cbf8: ldr      r3, [r4, #0x34]
0051cbfc: str      r3, [ip, #0x34]
0051cc00: ldm      r5, {r0, r3}
0051cc04: cmp      r3, r0
0051cc08: beq      #0x51cc38
0051cc0c: sub      r1, r3, #0x38
0051cc10: rsb      r1, r0, r1
0051cc14: movw     r2, #0x6db7
0051cc18: lsr      r1, r1, #3
0051cc1c: movt     r2, #0x16db
0051cc20: mul      r2, r2, r1
0051cc24: mvn      r1, #0x37
0051cc28: bic      r2, r2, #0xe0000000
0051cc2c: mul      r2, r1, r2
0051cc30: add      r2, r2, r1
0051cc34: add      r3, r3, r2
0051cc38: cmp      r3, #0
0051cc3c: ldr      r2, [r5, #8]
0051cc40: beq      #0x51cc78
0051cc44: rsb      r3, r3, r2
0051cc48: asr      r3, r3, #3
0051cc4c: mov      r1, #0x38
0051cc50: add      r2, r3, r3, lsl #3
0051cc54: add      r2, r2, r2, lsl #6
0051cc58: add      r2, r3, r2, lsl #3
0051cc5c: add      r2, r2, r2, lsl #15
0051cc60: add      r3, r3, r2, lsl #3
0051cc64: rsb      r3, r3, #0
0051cc68: mul      r1, r1, r3
0051cc6c: cmp      r1, #0x80
0051cc70: bhi      #0x51cc9c
0051cc74: bl       #0x708f00
0051cc78: ldr      r3, [sp, #4]
0051cc7c: mov      r2, #0x38
0051cc80: str      r7, [r5]
0051cc84: mla      r7, r2, r3, r7
0051cc88: stmib    r5, {r6, r7}
0051cc8c: b        #0x51ca68
0051cc90: cmp      r2, r1
0051cc94: bls      #0x51cab8
0051cc98: b        #0x51cab0
0051cc9c: bl       #0x310440
0051cca0: b        #0x51cc78

# _ZNKSt4priv8_Rb_treeI7CompPosSt4lessIS1_ESt4pairIKS1_P12PFGInnerNodeENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE7_M_findI7Point3DIfEEEPNS_18_Rb_tree_node_baseERKT_
0051bf28: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0051bf2c: ldr      r4, [r0, #4]
0051bf30: mov      sl, r0
0051bf34: cmp      r4, #0
0051bf38: beq      #0x51c048
0051bf3c: ldr      sb, [r1, #8]
0051bf40: ldr      r6, [r1]
0051bf44: ldr      r8, [r1, #4]
0051bf48: mov      r7, r0
0051bf4c: ldr      r5, [r4, #0x10]
0051bf50: mov      r1, r6
0051bf54: mov      r0, r5
0051bf58: bl       #0x30e3ac
0051bf5c: movw     r1, #0xb717
0051bf60: bic      r0, r0, #0x80000000
0051bf64: movt     r1, #0x38d1
0051bf68: bl       #0x30e70c
0051bf6c: cmp      r0, #0
0051bf70: beq      #0x51c054
0051bf74: ldr      r5, [r4, #0x14]
0051bf78: mov      r1, r8
0051bf7c: mov      r0, r5
0051bf80: bl       #0x30e3ac
0051bf84: movw     r1, #0xb717
0051bf88: bic      r0, r0, #0x80000000
0051bf8c: movt     r1, #0x38d1
0051bf90: bl       #0x30e70c
0051bf94: cmp      r0, #0
0051bf98: beq      #0x51c070
0051bf9c: ldr      r0, [r4, #0x18]
0051bfa0: mov      r1, sb
0051bfa4: bl       #0x30e70c
0051bfa8: cmp      r0, #0
0051bfac: mov      r3, #0
0051bfb0: movne    r3, #1
0051bfb4: uxtb     r3, r3
0051bfb8: cmp      r3, #0
0051bfbc: moveq    r7, r4
0051bfc0: ldrne    r4, [r4, #0xc]
0051bfc4: ldreq    r4, [r4, #8]
0051bfc8: cmp      r4, #0
0051bfcc: bne      #0x51bf4c
0051bfd0: cmp      r7, sl
0051bfd4: beq      #0x51c04c
0051bfd8: ldr      r5, [r7, #0x10]
0051bfdc: mov      r0, r6
0051bfe0: mov      r1, r5
0051bfe4: bl       #0x30e3ac
0051bfe8: movw     r1, #0xb717
0051bfec: bic      r0, r0, #0x80000000
0051bff0: movt     r1, #0x38d1
0051bff4: bl       #0x30e70c
0051bff8: cmp      r0, #0
0051bffc: beq      #0x51c08c
0051c000: ldr      r5, [r7, #0x14]
0051c004: mov      r0, r8
0051c008: mov      r1, r5
0051c00c: bl       #0x30e3ac
0051c010: movw     r1, #0xb717
0051c014: bic      r0, r0, #0x80000000
0051c018: movt     r1, #0x38d1
0051c01c: bl       #0x30e70c
0051c020: cmp      r0, #0
0051c024: beq      #0x51c0a4
0051c028: mov      r0, sb
0051c02c: ldr      r1, [r7, #0x18]
0051c030: bl       #0x30e70c
0051c034: cmp      r0, #0
0051c038: movne    r4, #1
0051c03c: uxtb     r4, r4
0051c040: cmp      r4, #0
0051c044: beq      #0x51c04c
0051c048: mov      r7, sl
0051c04c: mov      r0, r7
0051c050: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0051c054: mov      r0, r5
0051c058: mov      r1, r6
0051c05c: bl       #0x30e70c
0051c060: cmp      r0, #0
0051c064: mov      r3, #0
0051c068: movne    r3, #1
0051c06c: b        #0x51bfb4
0051c070: mov      r0, r5
0051c074: mov      r1, r8
0051c078: bl       #0x30e70c
0051c07c: cmp      r0, #0
0051c080: mov      r3, #0
0051c084: movne    r3, #1
0051c088: b        #0x51bfb4
0051c08c: mov      r0, r6
0051c090: mov      r1, r5
0051c094: bl       #0x30e70c
0051c098: cmp      r0, #0
0051c09c: movne    r4, #1
0051c0a0: b        #0x51c03c
0051c0a4: mov      r0, r8
0051c0a8: mov      r1, r5
0051c0ac: bl       #0x30e70c
0051c0b0: cmp      r0, #0
0051c0b4: movne    r4, #1
0051c0b8: b        #0x51c03c

# _ZN7PFFloor11_CreateNodeERK7Point3DIfES3_S3_b
0051fa64: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0051fa68: mov      r5, r0
0051fa6c: sub      sp, sp, #0x54
0051fa70: ldr      r0, [r0, #0x20]
0051fa74: mov      r8, r2
0051fa78: ldrb     r2, [sp, #0x78]
0051fa7c: tst      r0, #0x1000000
0051fa80: mov      r4, r1
0051fa84: mov      fp, r3
0051fa88: str      r2, [sp, #8]
0051fa8c: bne      #0x51fbc4
0051fa90: mov      r3, #0
0051fa94: ldr      r1, [r8, #4]
0051fa98: ldr      r0, [r4, #4]
0051fa9c: str      r3, [sp, #0x40]
0051faa0: str      r3, [sp, #0x38]
0051faa4: str      r3, [sp, #0x3c]
0051faa8: bl       #0x30eba4
0051faac: mov      r1, #0x3f000000
0051fab0: bl       #0x30ed6c
0051fab4: ldr      r1, [r8, #8]
0051fab8: mov      sb, r0
0051fabc: ldr      r0, [r4, #8]
0051fac0: bl       #0x30eba4
0051fac4: mov      r1, #0x3f000000
0051fac8: bl       #0x30ed6c
0051facc: ldr      r1, [r8]
0051fad0: mov      sl, r0
0051fad4: ldr      r0, [r4]
0051fad8: bl       #0x30eba4
0051fadc: mov      r1, #0x3f000000
0051fae0: bl       #0x30ed6c
0051fae4: add      r6, r5, #0x90
0051fae8: add      r7, sp, #0x44
0051faec: str      r0, [sp, #0x44]
0051faf0: mov      r1, r7
0051faf4: mov      r0, r6
0051faf8: str      sb, [sp, #0x48]
0051fafc: str      sl, [sp, #0x4c]
0051fb00: bl       #0x51bc78
0051fb04: cmp      r0, r6
0051fb08: ldrne    r8, [r0, #0x1c]
0051fb0c: beq      #0x51fbcc
0051fb10: mov      r0, r8
0051fb14: add      sp, sp, #0x54
0051fb18: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0051fb1c: ldr      r1, [sp, #0x3c]
0051fb20: ldr      r0, [sp, #0x48]
0051fb24: bl       #0x30eba4
0051fb28: ldr      r1, [sp, #0x40]
0051fb2c: mov      r8, r0
0051fb30: ldr      r0, [sp, #0x4c]
0051fb34: bl       #0x30eba4
0051fb38: ldr      r1, [sp, #0x38]
0051fb3c: mov      fp, r0
0051fb40: ldr      r0, [sp, #0x44]
0051fb44: bl       #0x30eba4
0051fb48: ldr      r2, [sp, #8]
0051fb4c: str      r0, [sp, #0x2c]
0051fb50: add      r1, sp, #0x2c
0051fb54: mov      r0, r5
0051fb58: mov      r3, r2
0051fb5c: str      r8, [sp, #0x30]
0051fb60: str      fp, [sp, #0x34]
0051fb64: bl       #0x51badc
0051fb68: cmp      r0, #0
0051fb6c: beq      #0x51fbc4
0051fb70: ldr      r1, [sp, #0x3c]
0051fb74: ldr      r0, [sp, #0x48]
0051fb78: bl       #0x30e3ac
0051fb7c: ldr      r1, [sp, #0x40]
0051fb80: mov      r8, r0
0051fb84: ldr      r0, [sp, #0x4c]
0051fb88: bl       #0x30e3ac
0051fb8c: ldr      r1, [sp, #0x38]
0051fb90: mov      fp, r0
0051fb94: ldr      r0, [sp, #0x44]
0051fb98: bl       #0x30e3ac
0051fb9c: ldr      r2, [sp, #8]
0051fba0: str      r0, [sp, #0x20]
0051fba4: add      r1, sp, #0x20
0051fba8: mov      r0, r5
0051fbac: mov      r3, r2
0051fbb0: str      r8, [sp, #0x24]
0051fbb4: str      fp, [sp, #0x28]
0051fbb8: bl       #0x51badc
0051fbbc: cmp      r0, #0
0051fbc0: bne      #0x51fcac
0051fbc4: mov      r8, #0
0051fbc8: b        #0x51fb10
0051fbcc: ldr      r1, [r4]
0051fbd0: ldr      r0, [r8]
0051fbd4: bl       #0x30e3ac
0051fbd8: ldr      r1, [r4, #4]
0051fbdc: mov      sl, r0
0051fbe0: ldr      r0, [r8, #4]
0051fbe4: bl       #0x30e3ac
0051fbe8: ldr      r1, [r4, #8]
0051fbec: mov      sb, r0
0051fbf0: ldr      r0, [r8, #8]
0051fbf4: bl       #0x30e3ac
0051fbf8: ldr      r3, [fp, #8]
0051fbfc: ldr      r2, [fp, #4]
0051fc00: mov      r4, r0
0051fc04: mov      r1, r3
0051fc08: mov      r0, sb
0051fc0c: str      r2, [sp, #0xc]
0051fc10: str      r3, [sp, #4]
0051fc14: bl       #0x30ed6c
0051fc18: ldr      r1, [sp, #0xc]
0051fc1c: mov      r8, r0
0051fc20: mov      r0, r4
0051fc24: bl       #0x30ed6c
0051fc28: mov      r1, r0
0051fc2c: mov      r0, r8
0051fc30: bl       #0x30e3ac
0051fc34: ldr      r8, [fp]
0051fc38: str      r0, [sp, #0x38]
0051fc3c: mov      r0, r4
0051fc40: mov      r1, r8
0051fc44: bl       #0x30ed6c
0051fc48: ldr      r3, [sp, #4]
0051fc4c: mov      fp, r0
0051fc50: mov      r0, sl
0051fc54: mov      r1, r3
0051fc58: bl       #0x30ed6c
0051fc5c: mov      r1, r0
0051fc60: mov      r0, fp
0051fc64: bl       #0x30e3ac
0051fc68: ldr      r1, [sp, #0xc]
0051fc6c: str      r0, [sp, #0x3c]
0051fc70: mov      r0, sl
0051fc74: bl       #0x30ed6c
0051fc78: mov      r1, r8
0051fc7c: mov      fp, r0
0051fc80: mov      r0, sb
0051fc84: bl       #0x30ed6c
0051fc88: mov      r1, r0
0051fc8c: mov      r0, fp
0051fc90: bl       #0x30e3ac
0051fc94: str      r0, [sp, #0x40]
0051fc98: add      r0, sp, #0x38
0051fc9c: bl       #0x34d0b0
0051fca0: ldr      r3, [sp, #8]
0051fca4: cmp      r3, #0
0051fca8: beq      #0x51fb1c
0051fcac: ldr      r3, [r5, #0x74]
0051fcb0: ldr      r2, [r3, #0x1c]
0051fcb4: mov      r0, r3
0051fcb8: add      r2, r2, #1
0051fcbc: mov      r1, r2
0051fcc0: str      r2, [r3, #0x1c]
0051fcc4: bl       #0x51dca4
0051fcc8: ldr      r3, [sp, #0x44]
0051fccc: mov      r8, r0
0051fcd0: add      r0, sp, #0x14
0051fcd4: str      r3, [r8, #8]
0051fcd8: ldr      r3, [sp, #0x48]
0051fcdc: str      r3, [r8, #0xc]
0051fce0: ldr      r3, [sp, #0x4c]
0051fce4: str      r3, [r8, #0x10]
0051fce8: str      sl, [sp, #0x14]
0051fcec: str      sb, [sp, #0x18]
0051fcf0: str      r4, [sp, #0x1c]
0051fcf4: bl       #0x34d0b0
0051fcf8: ldr      r2, [r0, #4]
0051fcfc: ldr      r3, [r0, #8]
0051fd00: ldr      r1, [r0]
0051fd04: str      r2, [r8, #0x18]
0051fd08: str      r3, [r8, #0x1c]
0051fd0c: mov      r0, sl
0051fd10: str      r1, [r8, #0x14]
0051fd14: mov      r1, sl
0051fd18: bl       #0x30ed6c
0051fd1c: mov      r1, sb
0051fd20: mov      sl, r0
0051fd24: mov      r0, sb
0051fd28: bl       #0x30ed6c
0051fd2c: mov      r1, r0
0051fd30: mov      r0, sl
0051fd34: bl       #0x30eba4
0051fd38: mov      r1, r4
0051fd3c: mov      sl, r0
0051fd40: mov      r0, r4
0051fd44: bl       #0x30ed6c
0051fd48: mov      r1, r0
0051fd4c: mov      r0, sl
0051fd50: bl       #0x30eba4
0051fd54: bl       #0x30e124
0051fd58: str      r5, [r8, #0x28]
0051fd5c: str      r0, [r8, #0x24]
0051fd60: str      r0, [r8, #0x20]
0051fd64: mov      r1, r7
0051fd68: mov      r0, r6
0051fd6c: bl       #0x51f848
0051fd70: str      r8, [r0]
0051fd74: b        #0x51fb10

# _ZNSt4priv10_Rb_globalIbE10_RebalanceEPNS_18_Rb_tree_node_baseERS3_
00313760: mov      r3, #0
00313764: push     {r4, r5, r6}
00313768: strb     r3, [r0]
0031376c: mov      r5, #1
00313770: ldr      ip, [r1]
00313774: cmp      ip, r0
00313778: beq      #0x31378c
0031377c: ldr      r2, [r0, #4]
00313780: ldrb     r4, [r2]
00313784: cmp      r4, #0
00313788: beq      #0x31379c
0031378c: mov      r3, #1
00313790: strb     r3, [ip]
00313794: pop      {r4, r5, r6}
00313798: bx       lr
0031379c: ldr      r6, [r2, #4]
003137a0: ldr      ip, [r6, #8]
003137a4: cmp      r2, ip
003137a8: beq      #0x313864
003137ac: cmp      ip, #0
003137b0: beq      #0x3137e4
003137b4: ldrb     r4, [ip]
003137b8: cmp      r4, #0
003137bc: bne      #0x3137e4
003137c0: strb     r5, [r2]
003137c4: strb     r5, [ip]
003137c8: ldr      r2, [r0, #4]
003137cc: ldr      r2, [r2, #4]
003137d0: strb     r4, [r2]
003137d4: ldr      r2, [r0, #4]
003137d8: ldr      r2, [r2, #4]
003137dc: mov      r0, r2
003137e0: b        #0x313770
003137e4: ldr      ip, [r2, #8]
003137e8: cmp      ip, r0
003137ec: movne    ip, r2
003137f0: movne    r2, r0
003137f4: beq      #0x3138fc
003137f8: strb     r5, [ip]
003137fc: ldr      r0, [r2, #4]
00313800: ldr      r0, [r0, #4]
00313804: strb     r3, [r0]
00313808: ldr      r0, [r2, #4]
0031380c: ldr      r0, [r0, #4]
00313810: ldr      ip, [r0, #0xc]
00313814: ldr      r4, [ip, #8]
00313818: str      r4, [r0, #0xc]
0031381c: ldr      r4, [ip, #8]
00313820: cmp      r4, #0
00313824: strne    r0, [r4, #4]
00313828: ldr      r4, [r0, #4]
0031382c: str      r4, [ip, #4]
00313830: ldr      r4, [r1]
00313834: cmp      r0, r4
00313838: streq    ip, [r1]
0031383c: beq      #0x313854
00313840: ldr      r4, [r0, #4]
00313844: ldr      r6, [r4, #8]
00313848: cmp      r0, r6
0031384c: streq    ip, [r4, #8]
00313850: strne    ip, [r4, #0xc]
00313854: str      r0, [ip, #8]
00313858: str      ip, [r0, #4]
0031385c: mov      r0, r2
00313860: b        #0x313770
00313864: ldr      ip, [r6, #0xc]
00313868: cmp      ip, #0
0031386c: beq      #0x31387c
00313870: ldrb     r4, [ip]
00313874: cmp      r4, #0
00313878: beq      #0x3137c0
0031387c: ldr      ip, [r2, #0xc]
00313880: cmp      ip, r0
00313884: movne    ip, r2
00313888: movne    r2, r0
0031388c: beq      #0x313948
00313890: strb     r5, [ip]
00313894: ldr      r0, [r2, #4]
00313898: ldr      r0, [r0, #4]
0031389c: strb     r3, [r0]
003138a0: ldr      r0, [r2, #4]
003138a4: ldr      r0, [r0, #4]
003138a8: ldr      ip, [r0, #8]
003138ac: ldr      r4, [ip, #0xc]
003138b0: str      r4, [r0, #8]
003138b4: ldr      r4, [ip, #0xc]
003138b8: cmp      r4, #0
003138bc: strne    r0, [r4, #4]
003138c0: ldr      r4, [r0, #4]
003138c4: str      r4, [ip, #4]
003138c8: ldr      r4, [r1]
003138cc: cmp      r0, r4
003138d0: streq    ip, [r1]
003138d4: beq      #0x3138ec
003138d8: ldr      r4, [r0, #4]
003138dc: ldr      r6, [r4, #0xc]
003138e0: cmp      r0, r6
003138e4: streq    ip, [r4, #0xc]
003138e8: strne    ip, [r4, #8]
003138ec: str      r0, [ip, #0xc]
003138f0: str      ip, [r0, #4]
003138f4: mov      r0, r2
003138f8: b        #0x313770
003138fc: ldr      r4, [r0, #0xc]
00313900: str      r4, [r2, #8]
00313904: ldr      r0, [r0, #0xc]
00313908: cmp      r0, #0
0031390c: strne    r2, [r0, #4]
00313910: ldrne    r6, [r2, #4]
00313914: str      r6, [ip, #4]
00313918: ldr      r0, [r1]
0031391c: cmp      r2, r0
00313920: streq    ip, [r1]
00313924: beq      #0x31393c
00313928: ldr      r0, [r2, #4]
0031392c: ldr      r4, [r0, #0xc]
00313930: cmp      r2, r4
00313934: streq    ip, [r0, #0xc]
00313938: strne    ip, [r0, #8]
0031393c: str      r2, [ip, #0xc]
00313940: str      ip, [r2, #4]
00313944: b        #0x3137f8
00313948: ldr      r4, [r0, #8]
0031394c: str      r4, [r2, #0xc]
00313950: ldr      r0, [r0, #8]
00313954: cmp      r0, #0
00313958: strne    r2, [r0, #4]
0031395c: ldrne    r6, [r2, #4]
00313960: str      r6, [ip, #4]
00313964: ldr      r0, [r1]
00313968: cmp      r2, r0
0031396c: streq    ip, [r1]
00313970: beq      #0x313988
00313974: ldr      r0, [r2, #4]
00313978: ldr      r4, [r0, #8]
0031397c: cmp      r2, r4
00313980: streq    ip, [r0, #8]
00313984: strne    ip, [r0, #0xc]
00313988: str      r2, [ip, #8]
0031398c: str      ip, [r2, #4]
00313990: b        #0x313890

# _ZN7Point3DIfEdVERKf
0034d04c: push     {r4, r5, r6, lr}
0034d050: mov      r4, r0
0034d054: mov      r5, r1
0034d058: ldr      r0, [r0]
0034d05c: ldr      r1, [r1]
0034d060: bl       #0x30ec94
0034d064: str      r0, [r4]
0034d068: ldr      r1, [r5]
0034d06c: ldr      r0, [r4, #4]
0034d070: bl       #0x30ec94
0034d074: str      r0, [r4, #4]
0034d078: ldr      r1, [r5]
0034d07c: ldr      r0, [r4, #8]
0034d080: bl       #0x30ec94
0034d084: str      r0, [r4, #8]
0034d088: mov      r0, r4
0034d08c: pop      {r4, r5, r6, pc}

# _ZN7PFFloor11_CreateEdgeEP12PFGInnerNodeS1_
0051e98c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0051e990: mov      r3, r0
0051e994: ldr      r0, [r0, #0x20]
0051e998: mov      r4, r1
0051e99c: mov      r5, r2
0051e9a0: tst      r0, #0x2000000
0051e9a4: beq      #0x51e9b4
0051e9a8: mov      r6, #0
0051e9ac: mov      r0, r6
0051e9b0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0051e9b4: cmp      r1, #0
0051e9b8: cmpne    r2, #0
0051e9bc: beq      #0x51e9a8
0051e9c0: ldr      r2, [r1]
0051e9c4: mov      r0, r1
0051e9c8: ldr      r7, [r3, #0x74]
0051e9cc: mov      lr, pc
0051e9d0: ldr      pc, [r2]
0051e9d4: ldr      r3, [r5]
0051e9d8: mov      r6, r0
0051e9dc: mov      r0, r5
0051e9e0: mov      lr, pc
0051e9e4: ldr      pc, [r3]
0051e9e8: mov      r1, r6
0051e9ec: mov      r2, r0
0051e9f0: mov      r0, r7
0051e9f4: bl       #0x51e758
0051e9f8: ldr      r1, [r4, #8]
0051e9fc: mov      r6, r0
0051ea00: ldr      r0, [r5, #8]
0051ea04: bl       #0x30e3ac
0051ea08: ldr      r1, [r4, #0xc]
0051ea0c: mov      r7, r0
0051ea10: ldr      r0, [r5, #0xc]
0051ea14: bl       #0x30e3ac
0051ea18: ldr      r1, [r4, #0x10]
0051ea1c: mov      sl, r0
0051ea20: ldr      r0, [r5, #0x10]
0051ea24: bl       #0x30e3ac
0051ea28: mov      r1, r7
0051ea2c: mov      r8, r0
0051ea30: mov      r0, r7
0051ea34: bl       #0x30ed6c
0051ea38: mov      r1, sl
0051ea3c: mov      r7, r0
0051ea40: mov      r0, sl
0051ea44: bl       #0x30ed6c
0051ea48: mov      r1, r0
0051ea4c: mov      r0, r7
0051ea50: bl       #0x30eba4
0051ea54: mov      r1, r8
0051ea58: mov      r7, r0
0051ea5c: mov      r0, r8
0051ea60: bl       #0x30ed6c
0051ea64: mov      r1, r0
0051ea68: mov      r0, r7
0051ea6c: bl       #0x30eba4
0051ea70: bl       #0x30e124
0051ea74: str      r0, [r6, #0x10]
0051ea78: ldr      r5, [r5, #0x20]
0051ea7c: ldr      r4, [r4, #0x20]
0051ea80: mov      r7, r0
0051ea84: mov      r0, r5
0051ea88: mov      r1, r4
0051ea8c: bl       #0x30e70c
0051ea90: cmp      r0, #0
0051ea94: moveq    r5, r4
0051ea98: str      r5, [r6, #0x14]
0051ea9c: mov      r1, r7
0051eaa0: ldr      r3, [r6]
0051eaa4: mov      r0, r6
0051eaa8: mov      lr, pc
0051eaac: ldr      pc, [r3, #0x14]
0051eab0: b        #0x51e9ac

# _ZNSt4priv8_Rb_treeI7CompPosSt4lessIS1_ESt4pairIKS1_P12PFGInnerNodeENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE9_M_insertEPNS_18_Rb_tree_node_baseERKS8_SG_SG_
0051ead4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0051ead8: cmp      r1, r2
0051eadc: mov      r4, r1
0051eae0: mov      r5, r2
0051eae4: mov      r6, r0
0051eae8: mov      r8, r3
0051eaec: ldr      r7, [sp, #0x20]
0051eaf0: beq      #0x51ebcc
0051eaf4: ldr      r3, [sp, #0x24]
0051eaf8: cmp      r3, #0
0051eafc: beq      #0x51eb70
0051eb00: mov      r0, r4
0051eb04: bl       #0x51eab4
0051eb08: ldr      r2, [r8]
0051eb0c: mov      r3, #0
0051eb10: mov      r7, r0
0051eb14: str      r2, [r0, #0x10]
0051eb18: ldr      r2, [r8, #4]
0051eb1c: str      r2, [r0, #0x14]
0051eb20: ldr      r2, [r8, #8]
0051eb24: str      r2, [r0, #0x18]
0051eb28: ldr      r2, [r8, #0xc]
0051eb2c: str      r3, [r0, #0xc]
0051eb30: str      r3, [r0, #8]
0051eb34: str      r2, [r0, #0x1c]
0051eb38: str      r0, [r5, #0xc]
0051eb3c: ldr      r3, [r4, #0xc]
0051eb40: cmp      r5, r3
0051eb44: beq      #0x51ebc4
0051eb48: mov      r0, r7
0051eb4c: str      r5, [r7, #4]
0051eb50: add      r1, r4, #4
0051eb54: bl       #0x313760
0051eb58: ldr      r3, [r4, #0x10]
0051eb5c: mov      r0, r6
0051eb60: add      r3, r3, #1
0051eb64: str      r3, [r4, #0x10]
0051eb68: str      r7, [r6]
0051eb6c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0051eb70: cmp      r7, #0
0051eb74: beq      #0x51ec14
0051eb78: mov      r0, r4
0051eb7c: bl       #0x51eab4
0051eb80: ldr      r2, [r8]
0051eb84: mov      r3, #0
0051eb88: mov      r7, r0
0051eb8c: str      r2, [r0, #0x10]
0051eb90: ldr      r2, [r8, #4]
0051eb94: str      r2, [r0, #0x14]
0051eb98: ldr      r2, [r8, #8]
0051eb9c: str      r2, [r0, #0x18]
0051eba0: ldr      r2, [r8, #0xc]
0051eba4: str      r3, [r0, #0xc]
0051eba8: str      r3, [r0, #8]
0051ebac: str      r2, [r0, #0x1c]
0051ebb0: str      r0, [r5, #8]
0051ebb4: ldr      r3, [r4, #8]
0051ebb8: cmp      r5, r3
0051ebbc: streq    r0, [r4, #8]
0051ebc0: b        #0x51eb48
0051ebc4: str      r7, [r4, #0xc]
0051ebc8: b        #0x51eb48
0051ebcc: mov      r0, r1
0051ebd0: bl       #0x51eab4
0051ebd4: ldr      r2, [r8]
0051ebd8: mov      r3, #0
0051ebdc: mov      r7, r0
0051ebe0: str      r2, [r0, #0x10]
0051ebe4: ldr      r2, [r8, #4]
0051ebe8: str      r2, [r0, #0x14]
0051ebec: ldr      r2, [r8, #8]
0051ebf0: str      r2, [r0, #0x18]
0051ebf4: ldr      r2, [r8, #0xc]
0051ebf8: str      r3, [r0, #0xc]
0051ebfc: str      r3, [r0, #8]
0051ec00: str      r2, [r0, #0x1c]
0051ec04: str      r0, [r4, #8]
0051ec08: str      r0, [r4, #4]
0051ec0c: str      r0, [r4, #0xc]
0051ec10: b        #0x51eb48
0051ec14: ldr      sl, [r2, #0x10]
0051ec18: ldr      sb, [r8]
0051ec1c: mov      r1, sl
0051ec20: mov      r0, sb
0051ec24: bl       #0x30e3ac
0051ec28: movw     r1, #0xb717
0051ec2c: bic      r0, r0, #0x80000000
0051ec30: movt     r1, #0x38d1
0051ec34: bl       #0x30e70c
0051ec38: cmp      r0, #0
0051ec3c: beq      #0x51ec90
0051ec40: ldr      sb, [r8, #4]
0051ec44: ldr      sl, [r5, #0x14]
0051ec48: mov      r0, sb
0051ec4c: mov      r1, sl
0051ec50: bl       #0x30e3ac
0051ec54: movw     r1, #0xb717
0051ec58: bic      r0, r0, #0x80000000
0051ec5c: movt     r1, #0x38d1
0051ec60: bl       #0x30e70c
0051ec64: cmp      r0, #0
0051ec68: beq      #0x51ec90
0051ec6c: ldr      r0, [r8, #8]
0051ec70: ldr      r1, [r5, #0x18]
0051ec74: bl       #0x30e70c
0051ec78: cmp      r0, #0
0051ec7c: movne    r7, #1
0051ec80: uxtb     r3, r7
0051ec84: cmp      r3, #0
0051ec88: beq      #0x51eb00
0051ec8c: b        #0x51eb78
0051ec90: mov      r0, sb
0051ec94: mov      r1, sl
0051ec98: bl       #0x30e70c
0051ec9c: cmp      r0, #0
0051eca0: movne    r7, #1
0051eca4: b        #0x51ec80

# _ZNKSt4priv8_Rb_treeI7CompPosSt4lessIS1_ESt4pairIKS1_P12PFGInnerNodeENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE7_M_findIS1_EEPNS_18_Rb_tree_node_baseERKT_
0051bc78: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0051bc7c: ldr      r4, [r0, #4]
0051bc80: mov      sl, r0
0051bc84: mov      r7, r1
0051bc88: cmp      r4, #0
0051bc8c: beq      #0x51bd9c
0051bc90: ldr      r6, [r1]
0051bc94: mov      r8, r0
0051bc98: ldr      r5, [r4, #0x10]
0051bc9c: mov      r1, r6
0051bca0: mov      r0, r5
0051bca4: bl       #0x30e3ac
0051bca8: movw     r1, #0xb717
0051bcac: bic      r0, r0, #0x80000000
0051bcb0: movt     r1, #0x38d1
0051bcb4: bl       #0x30e70c
0051bcb8: cmp      r0, #0
0051bcbc: beq      #0x51bda8
0051bcc0: ldr      sb, [r4, #0x14]
0051bcc4: ldr      r5, [r7, #4]
0051bcc8: mov      r0, sb
0051bccc: mov      r1, r5
0051bcd0: bl       #0x30e3ac
0051bcd4: movw     r1, #0xb717
0051bcd8: bic      r0, r0, #0x80000000
0051bcdc: movt     r1, #0x38d1
0051bce0: bl       #0x30e70c
0051bce4: cmp      r0, #0
0051bce8: beq      #0x51bdc4
0051bcec: ldr      r0, [r4, #0x18]
0051bcf0: ldr      r1, [r7, #8]
0051bcf4: bl       #0x30e70c
0051bcf8: cmp      r0, #0
0051bcfc: mov      r3, #0
0051bd00: movne    r3, #1
0051bd04: uxtb     r3, r3
0051bd08: cmp      r3, #0
0051bd0c: moveq    r8, r4
0051bd10: ldrne    r4, [r4, #0xc]
0051bd14: ldreq    r4, [r4, #8]
0051bd18: cmp      r4, #0
0051bd1c: bne      #0x51bc98
0051bd20: cmp      r8, sl
0051bd24: beq      #0x51bda0
0051bd28: ldr      r5, [r8, #0x10]
0051bd2c: mov      r0, r6
0051bd30: mov      r1, r5
0051bd34: bl       #0x30e3ac
0051bd38: movw     r1, #0xb717
0051bd3c: bic      r0, r0, #0x80000000
0051bd40: movt     r1, #0x38d1
0051bd44: bl       #0x30e70c
0051bd48: cmp      r0, #0
0051bd4c: beq      #0x51bde0
0051bd50: ldr      r6, [r7, #4]
0051bd54: ldr      r5, [r8, #0x14]
0051bd58: mov      r0, r6
0051bd5c: mov      r1, r5
0051bd60: bl       #0x30e3ac
0051bd64: movw     r1, #0xb717
0051bd68: bic      r0, r0, #0x80000000
0051bd6c: movt     r1, #0x38d1
0051bd70: bl       #0x30e70c
0051bd74: cmp      r0, #0
0051bd78: beq      #0x51bdf8
0051bd7c: ldr      r0, [r7, #8]
0051bd80: ldr      r1, [r8, #0x18]
0051bd84: bl       #0x30e70c
0051bd88: cmp      r0, #0
0051bd8c: movne    r4, #1
0051bd90: uxtb     r4, r4
0051bd94: cmp      r4, #0
0051bd98: beq      #0x51bda0
0051bd9c: mov      r8, sl
0051bda0: mov      r0, r8
0051bda4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0051bda8: mov      r0, r5
0051bdac: mov      r1, r6
0051bdb0: bl       #0x30e70c
0051bdb4: cmp      r0, #0
0051bdb8: mov      r3, #0
0051bdbc: movne    r3, #1
0051bdc0: b        #0x51bd04
0051bdc4: mov      r0, sb
0051bdc8: mov      r1, r5
0051bdcc: bl       #0x30e70c
0051bdd0: cmp      r0, #0
0051bdd4: mov      r3, #0
0051bdd8: movne    r3, #1
0051bddc: b        #0x51bd04
0051bde0: mov      r0, r5
0051bde4: mov      r1, r6
0051bde8: bl       #0x30e2f8
0051bdec: cmp      r0, #0
0051bdf0: movne    r4, #1
0051bdf4: b        #0x51bd90
0051bdf8: mov      r0, r6
0051bdfc: mov      r1, r5
0051be00: bl       #0x30e70c
0051be04: cmp      r0, #0
0051be08: movne    r4, #1
0051be0c: b        #0x51bd90
