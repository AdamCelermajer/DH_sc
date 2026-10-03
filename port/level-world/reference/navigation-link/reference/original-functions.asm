
# _ZNSt4priv8_Rb_treeIP7PFFloorSt4lessIS2_ES2_NS_9_IdentityIS2_EENS_11_SetTraitsTIS2_EESaIS2_EE9_M_insertEPNS_18_Rb_tree_node_baseERKS2_SC_SC_.clone.7
0051de78: push     {r4, r5, r6, r7, lr}
0051de7c: cmp      r1, r2
0051de80: sub      sp, sp, #0xc
0051de84: mov      r4, r1
0051de88: mov      r5, r0
0051de8c: ldr      r7, [sp, #0x20]
0051de90: beq      #0x51dee0
0051de94: cmp      r7, #0
0051de98: beq      #0x51df48
0051de9c: mov      r0, r4
0051dea0: str      r2, [sp, #4]
0051dea4: str      r3, [sp]
0051dea8: bl       #0x51de58
0051deac: ldr      r3, [sp]
0051deb0: mov      r6, r0
0051deb4: ldr      r1, [r3]
0051deb8: mov      r3, #0
0051debc: str      r3, [r0, #0xc]
0051dec0: str      r1, [r0, #0x10]
0051dec4: str      r3, [r0, #8]
0051dec8: ldr      r2, [sp, #4]
0051decc: str      r0, [r2, #8]
0051ded0: ldr      r3, [r4, #8]
0051ded4: cmp      r2, r3
0051ded8: streq    r0, [r4, #8]
0051dedc: b        #0x51df1c
0051dee0: mov      r0, r1
0051dee4: str      r2, [sp, #4]
0051dee8: str      r3, [sp]
0051deec: bl       #0x51de58
0051def0: ldr      r3, [sp]
0051def4: mov      r6, r0
0051def8: ldr      r1, [r3]
0051defc: mov      r3, #0
0051df00: str      r3, [r0, #0xc]
0051df04: str      r1, [r0, #0x10]
0051df08: str      r3, [r0, #8]
0051df0c: str      r0, [r4, #8]
0051df10: str      r0, [r4, #4]
0051df14: str      r0, [r4, #0xc]
0051df18: ldr      r2, [sp, #4]
0051df1c: mov      r0, r6
0051df20: str      r2, [r6, #4]
0051df24: add      r1, r4, #4
0051df28: bl       #0x313760
0051df2c: ldr      r3, [r4, #0x10]
0051df30: mov      r0, r5
0051df34: add      r3, r3, #1
0051df38: str      r3, [r4, #0x10]
0051df3c: str      r6, [r5]
0051df40: add      sp, sp, #0xc
0051df44: pop      {r4, r5, r6, r7, pc}
0051df48: ldr      r0, [r3]
0051df4c: ldr      r1, [r2, #0x10]
0051df50: cmp      r0, r1
0051df54: blo      #0x51de9c
0051df58: mov      r0, r4
0051df5c: str      r2, [sp, #4]
0051df60: str      r3, [sp]
0051df64: bl       #0x51de58
0051df68: ldr      r3, [sp]
0051df6c: mov      r6, r0
0051df70: ldr      r3, [r3]
0051df74: str      r7, [r0, #0xc]
0051df78: str      r7, [r0, #8]
0051df7c: str      r3, [r0, #0x10]
0051df80: ldr      r2, [sp, #4]
0051df84: str      r0, [r2, #0xc]
0051df88: ldr      r3, [r4, #0xc]
0051df8c: cmp      r2, r3
0051df90: streq    r0, [r4, #0xc]
0051df94: b        #0x51df1c

# _ZNSaINSt4priv13_Rb_tree_nodeISt4pairIK7CompPosP12PFGInnerNodeEEEE8allocateEjPKv.clone.11
0051eab4: str      lr, [sp, #-4]!
0051eab8: sub      sp, sp, #0xc
0051eabc: add      r0, sp, #8
0051eac0: mov      r3, #0x20
0051eac4: str      r3, [r0, #-4]!
0051eac8: bl       #0x708ec0
0051eacc: add      sp, sp, #0xc
0051ead0: ldm      sp!, {pc}

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

# _ZNSt4priv8_Rb_treeIP7PFFloorSt4lessIS2_ES2_NS_9_IdentityIS2_EENS_11_SetTraitsTIS2_EESaIS2_EE13insert_uniqueERKS2_
0051df98: push     {r4, r5, r6, lr}
0051df9c: ldr      ip, [r1, #4]
0051dfa0: sub      sp, sp, #0x10
0051dfa4: mov      r4, r0
0051dfa8: cmp      ip, #0
0051dfac: mov      r3, r2
0051dfb0: moveq    ip, r1
0051dfb4: beq      #0x51e010
0051dfb8: ldr      r6, [r2]
0051dfbc: b        #0x51dfc4
0051dfc0: mov      ip, r2
0051dfc4: ldr      r0, [ip, #0x10]
0051dfc8: mov      r5, #1
0051dfcc: cmp      r0, r6
0051dfd0: ldrhi    r2, [ip, #8]
0051dfd4: ldrls    r2, [ip, #0xc]
0051dfd8: movls    r5, #0
0051dfdc: cmp      r2, #0
0051dfe0: bne      #0x51dfc0
0051dfe4: cmp      r5, #0
0051dfe8: moveq    r5, ip
0051dfec: bne      #0x51e010
0051dff0: cmp      r6, r0
0051dff4: movls    r3, #0
0051dff8: strls    r5, [r4]
0051dffc: strbls   r3, [r4, #4]
0051e000: bhi      #0x51e078
0051e004: mov      r0, r4
0051e008: add      sp, sp, #0x10
0051e00c: pop      {r4, r5, r6, pc}
0051e010: ldr      r2, [r1, #8]
0051e014: cmp      ip, r2
0051e018: beq      #0x51e0f4
0051e01c: ldrb     r2, [ip]
0051e020: cmp      r2, #0
0051e024: bne      #0x51e038
0051e028: ldr      r2, [ip, #4]
0051e02c: ldr      r2, [r2, #4]
0051e030: cmp      ip, r2
0051e034: beq      #0x51e0e0
0051e038: ldr      r0, [ip, #8]
0051e03c: cmp      r0, #0
0051e040: bne      #0x51e04c
0051e044: b        #0x51e0a0
0051e048: mov      r0, r2
0051e04c: ldr      r2, [r0, #0xc]
0051e050: cmp      r2, #0
0051e054: bne      #0x51e048
0051e058: ldr      r6, [r3]
0051e05c: mov      r5, r0
0051e060: ldr      r0, [r0, #0x10]
0051e064: cmp      r6, r0
0051e068: movls    r3, #0
0051e06c: strls    r5, [r4]
0051e070: strbls   r3, [r4, #4]
0051e074: bls      #0x51e004
0051e078: mov      r2, ip
0051e07c: add      r0, sp, #8
0051e080: mov      ip, #0
0051e084: str      ip, [sp]
0051e088: bl       #0x51de78
0051e08c: ldr      r3, [sp, #8]
0051e090: mov      r2, #1
0051e094: strb     r2, [r4, #4]
0051e098: str      r3, [r4]
0051e09c: b        #0x51e004
0051e0a0: ldr      r2, [ip, #4]
0051e0a4: ldr      r0, [r2, #8]
0051e0a8: cmp      ip, r0
0051e0ac: movne    r5, r2
0051e0b0: ldrne    r6, [r3]
0051e0b4: ldrne    r0, [r2, #0x10]
0051e0b8: beq      #0x51e0c4
0051e0bc: b        #0x51dff0
0051e0c0: mov      r2, r5
0051e0c4: ldr      r5, [r2, #4]
0051e0c8: ldr      r0, [r5, #8]
0051e0cc: cmp      r0, r2
0051e0d0: beq      #0x51e0c0
0051e0d4: ldr      r6, [r3]
0051e0d8: ldr      r0, [r5, #0x10]
0051e0dc: b        #0x51dff0
0051e0e0: ldr      r2, [ip, #0xc]
0051e0e4: ldr      r6, [r3]
0051e0e8: mov      r5, r2
0051e0ec: ldr      r0, [r2, #0x10]
0051e0f0: b        #0x51dff0
0051e0f4: mov      r2, ip
0051e0f8: add      r0, sp, #0xc
0051e0fc: str      ip, [sp]
0051e100: bl       #0x51de78
0051e104: ldr      r3, [sp, #0xc]
0051e108: mov      r2, #1
0051e10c: strb     r2, [r4, #4]
0051e110: str      r3, [r4]
0051e114: b        #0x51e004

# _ZN7PFWorld8PostLoadEv
005226cc: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
005226d0: ldr      r3, [r0, #4]
005226d4: mov      fp, r0
005226d8: cmp      r3, #1
005226dc: beq      #0x5226e4
005226e0: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
005226e4: ldr      sl, [r0, #0xc]
005226e8: ldr      r7, [r0, #8]
005226ec: mov      r3, #2
005226f0: str      r3, [r0, #4]
005226f4: rsb      r2, r7, sl
005226f8: lsrs     r3, r2, r3
005226fc: beq      #0x5226e0
00522700: mov      r3, #0
00522704: add      sb, r3, #1
00522708: cmp      sb, r2, asr #2
0052270c: ldr      r8, [r7, r3, lsl #2]
00522710: bhs      #0x522820
00522714: lsl      r5, sb, #2
00522718: mov      r4, sb
0052271c: ldr      r6, [r7, r5]
00522720: mov      r1, #0x42000000
00522724: add      r1, r1, #0x480000
00522728: ldr      r0, [r6, #0x48]
0052272c: bl       #0x30eba4
00522730: mov      r1, r0
00522734: ldr      r0, [r8, #0x3c]
00522738: bl       #0x30e9ac
0052273c: mov      r1, #0x42000000
00522740: cmp      r0, #0
00522744: add      r4, r4, #1
00522748: add      r1, r1, #0x480000
0052274c: beq      #0x522810
00522750: ldr      r0, [r6, #0x3c]
00522754: bl       #0x30e3ac
00522758: mov      r1, r0
0052275c: ldr      r0, [r8, #0x48]
00522760: bl       #0x30e4b4
00522764: mov      r1, #0x42000000
00522768: cmp      r0, #0
0052276c: add      r1, r1, #0x480000
00522770: beq      #0x522810
00522774: ldr      r0, [r6, #0x4c]
00522778: bl       #0x30eba4
0052277c: mov      r1, r0
00522780: ldr      r0, [r8, #0x40]
00522784: bl       #0x30e9ac
00522788: mov      r1, #0x42000000
0052278c: cmp      r0, #0
00522790: add      r1, r1, #0x480000
00522794: beq      #0x522810
00522798: ldr      r0, [r6, #0x40]
0052279c: bl       #0x30e3ac
005227a0: mov      r1, r0
005227a4: ldr      r0, [r8, #0x4c]
005227a8: bl       #0x30e4b4
005227ac: mov      r1, #0x42000000
005227b0: cmp      r0, #0
005227b4: add      r1, r1, #0x480000
005227b8: beq      #0x522810
005227bc: ldr      r0, [r6, #0x50]
005227c0: bl       #0x30eba4
005227c4: mov      r1, r0
005227c8: ldr      r0, [r8, #0x44]
005227cc: bl       #0x30e9ac
005227d0: mov      r1, #0x42000000
005227d4: cmp      r0, #0
005227d8: add      r1, r1, #0x480000
005227dc: beq      #0x522810
005227e0: ldr      r0, [r6, #0x44]
005227e4: bl       #0x30e3ac
005227e8: mov      r1, r0
005227ec: ldr      r0, [r8, #0x50]
005227f0: bl       #0x30e4b4
005227f4: cmp      r0, #0
005227f8: beq      #0x522810
005227fc: mov      r1, r6
00522800: mov      r0, r8
00522804: bl       #0x5219cc
00522808: ldr      sl, [fp, #0xc]
0052280c: ldr      r7, [fp, #8]
00522810: rsb      r3, r7, sl
00522814: cmp      r4, r3, asr #2
00522818: add      r5, r5, #4
0052281c: blo      #0x52271c
00522820: mov      r0, r8
00522824: bl       #0x521408
00522828: ldr      sl, [fp, #0xc]
0052282c: ldr      r7, [fp, #8]
00522830: mov      r3, sb
00522834: rsb      r2, r7, sl
00522838: cmp      sb, r2, asr #2
0052283c: blo      #0x522704
00522840: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6PFRoom5_LinkEPS_
005219cc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005219d0: ldr      r2, [pc, #0x26c]
005219d4: ldr      r3, [pc, #0x26c]
005219d8: sub      sp, sp, #0x3c
005219dc: add      r2, pc, r2
005219e0: mov      sl, r0
005219e4: str      r2, [sp]
005219e8: ldr      r0, [r2, r3]
005219ec: str      r3, [sp, #4]
005219f0: ldr      r3, [sl, #0x30]
005219f4: ldr      r2, [sl, #0x34]
005219f8: ldr      r0, [r0]
005219fc: mov      r8, r1
00521a00: rsb      r1, r3, r2
00521a04: lsrs     r1, r1, #2
00521a08: str      r0, [sp, #0x34]
00521a0c: beq      #0x521bf0
00521a10: ldr      r1, [pc, #0x234]
00521a14: mov      r7, #0
00521a18: str      r1, [sp, #0xc]
00521a1c: ldr      r1, [pc, #0x22c]
00521a20: add      r1, pc, r1
00521a24: str      r1, [sp, #0x10]
00521a28: add      r1, sp, #0x1c
00521a2c: str      r1, [sp, #8]
00521a30: add      r1, sp, #0x18
00521a34: str      r1, [sp, #0x14]
00521a38: ldr      r6, [r3, r7, lsl #2]
00521a3c: ldr      r4, [r6, #0x20]
00521a40: ands     r4, r4, #0x4000000
00521a44: bne      #0x521be0
00521a48: ldr      sb, [r8, #0x30]
00521a4c: ldr      fp, [r8, #0x34]
00521a50: rsb      r1, sb, fp
00521a54: lsrs     r1, r1, #2
00521a58: beq      #0x521be0
00521a5c: ldr      r5, [sb, r4, lsl #2]
00521a60: ldr      r3, [r5, #0x20]
00521a64: tst      r3, #0x4000000
00521a68: bne      #0x521bc8
00521a6c: mov      r1, #0x42000000
00521a70: add      r1, r1, #0x480000
00521a74: ldr      r0, [r5, #0x50]
00521a78: bl       #0x30eba4
00521a7c: mov      r1, r0
00521a80: ldr      r0, [r6, #0x44]
00521a84: bl       #0x30e9ac
00521a88: cmp      r0, #0
00521a8c: beq      #0x521bc8
00521a90: mov      r1, #0x42000000
00521a94: add      r1, r1, #0x480000
00521a98: ldr      r0, [r5, #0x44]
00521a9c: bl       #0x30e3ac
00521aa0: mov      r1, r0
00521aa4: ldr      r0, [r6, #0x50]
00521aa8: bl       #0x30e4b4
00521aac: cmp      r0, #0
00521ab0: beq      #0x521bc8
00521ab4: mov      r1, #0x42000000
00521ab8: add      r1, r1, #0x480000
00521abc: ldr      r0, [r5, #0x54]
00521ac0: bl       #0x30eba4
00521ac4: mov      r1, r0
00521ac8: ldr      r0, [r6, #0x48]
00521acc: bl       #0x30e9ac
00521ad0: cmp      r0, #0
00521ad4: beq      #0x521bc8
00521ad8: mov      r1, #0x42000000
00521adc: add      r1, r1, #0x480000
00521ae0: ldr      r0, [r5, #0x48]
00521ae4: bl       #0x30e3ac
00521ae8: mov      r1, r0
00521aec: ldr      r0, [r6, #0x54]
00521af0: bl       #0x30e4b4
00521af4: cmp      r0, #0
00521af8: beq      #0x521bc8
00521afc: mov      r1, #0x42000000
00521b00: add      r1, r1, #0x480000
00521b04: ldr      r0, [r5, #0x58]
00521b08: bl       #0x30eba4
00521b0c: mov      r1, r0
00521b10: ldr      r0, [r6, #0x4c]
00521b14: bl       #0x30e9ac
00521b18: cmp      r0, #0
00521b1c: beq      #0x521bc8
00521b20: mov      r1, #0x42000000
00521b24: add      r1, r1, #0x480000
00521b28: ldr      r0, [r5, #0x4c]
00521b2c: bl       #0x30e3ac
00521b30: mov      r1, r0
00521b34: ldr      r0, [r6, #0x58]
00521b38: bl       #0x30e4b4
00521b3c: cmp      r0, #0
00521b40: beq      #0x521bc8
00521b44: ldr      r3, [sp]
00521b48: ldr      r2, [sp, #0xc]
00521b4c: ldr      sb, [r3, r2]
00521b50: mov      r0, sb
00521b54: bl       #0x337888
00521b58: ldr      r1, [sp, #0x10]
00521b5c: ldr      r2, [sp, #0x14]
00521b60: ldr      r0, [sp, #8]
00521b64: bl       #0x3140ec
00521b68: mov      r0, sb
00521b6c: ldr      r1, [sp, #8]
00521b70: bl       #0x337a88
00521b74: ldr      r1, [sp, #8]
00521b78: mov      sb, r0
00521b7c: ldr      r0, [sp, #0x30]
00521b80: cmp      r0, r1
00521b84: beq      #0x521ba4
00521b88: cmp      r0, #0
00521b8c: beq      #0x521ba4
00521b90: ldr      r1, [sp, #0x1c]
00521b94: rsb      r1, r0, r1
00521b98: cmp      r1, #0x80
00521b9c: bhi      #0x521c38
00521ba0: bl       #0x708f00
00521ba4: cmp      sb, #0
00521ba8: beq      #0x521c10
00521bac: bl       #0x60b0cc
00521bb0: mov      r0, r6
00521bb4: mov      r1, r5
00521bb8: bl       #0x51fd78
00521bbc: bl       #0x60b0cc
00521bc0: ldr      sb, [r8, #0x30]
00521bc4: ldr      fp, [r8, #0x34]
00521bc8: add      r4, r4, #1
00521bcc: rsb      r3, sb, fp
00521bd0: cmp      r4, r3, asr #2
00521bd4: blo      #0x521a5c
00521bd8: ldr      r3, [sl, #0x30]
00521bdc: ldr      r2, [sl, #0x34]
00521be0: add      r7, r7, #1
00521be4: rsb      r1, r3, r2
00521be8: cmp      r7, r1, asr #2
00521bec: blo      #0x521a38
00521bf0: ldm      sp, {r1, r2}
00521bf4: ldr      r3, [r1, r2]
00521bf8: ldr      r2, [sp, #0x34]
00521bfc: ldr      r3, [r3]
00521c00: cmp      r2, r3
00521c04: bne      #0x521c40
00521c08: add      sp, sp, #0x3c
00521c0c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00521c10: mov      r1, r5
00521c14: mov      r0, r6
00521c18: bl       #0x51fd78
00521c1c: ldr      sb, [r8, #0x30]
00521c20: ldr      fp, [r8, #0x34]
00521c24: add      r4, r4, #1
00521c28: rsb      r3, sb, fp
00521c2c: cmp      r4, r3, asr #2
00521c30: blo      #0x521a5c
00521c34: b        #0x521bd8
00521c38: bl       #0x310440
00521c3c: b        #0x521ba4
00521c40: bl       #0x30e310
00521c44: strheq   r3, [r7], #-4
00521c48: andeq    r4, r0, ip, lsr #1
00521c4c: andeq    r0, r0, r4, lsl #17
00521c50: eorseq   sl, fp, r8, lsr #31

# _ZN7PFFloor9_PostLoadEv
0051bee8: push     {r4, lr}
0051beec: ldr      r1, [r0, #0xa8]
0051bef0: ldr      r2, [r0, #0xac]
0051bef4: sub      sp, sp, #8
0051bef8: mov      r4, r0
0051befc: cmp      r1, r2
0051bf00: beq      #0x51bf10
0051bf04: add      r0, r0, #0xa8
0051bf08: add      r3, sp, #4
0051bf0c: bl       #0x51be10
0051bf10: ldr      r3, [r4, #0xb4]
0051bf14: ldr      r2, [r4, #0xb8]
0051bf18: cmp      r3, r2
0051bf1c: strne    r3, [r4, #0xb8]
0051bf20: add      sp, sp, #8
0051bf24: pop      {r4, pc}

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

# _ZN7PFFloor5_LinkEPS_
0051fd78: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0051fd7c: sub      sp, sp, #0xb4
0051fd80: str      r1, [sp, #0x4c]
0051fd84: ldr      r2, [r1, #0x20]
0051fd88: ldr      r3, [r0, #0x20]
0051fd8c: mov      r4, r0
0051fd90: mov      sb, r1
0051fd94: orr      r3, r2, r3
0051fd98: ands     r3, r3, #0x4000000
0051fd9c: bne      #0x51feb8
0051fda0: ldr      r5, [r0, #0xa8]
0051fda4: ldr      r6, [r0, #0xac]
0051fda8: str      r3, [sp, #0x5c]
0051fdac: str      r3, [sp, #0x60]
0051fdb0: cmp      r5, r6
0051fdb4: str      r3, [sp, #0x64]
0051fdb8: str      r3, [sp, #0x68]
0051fdbc: str      r3, [sp, #0x54]
0051fdc0: str      r3, [sp, #0x58]
0051fdc4: beq      #0x51fec0
0051fdc8: mov      r7, #1
0051fdcc: add      r8, sp, #0x60
0051fdd0: add      sl, sp, #0x84
0051fdd4: ldr      fp, [r5]
0051fdd8: mov      r1, #0x3f800000
0051fddc: mov      r0, fp
0051fde0: bl       #0x30eba4
0051fde4: mov      r1, r0
0051fde8: ldr      r0, [sb, #0x44]
0051fdec: bl       #0x30e9ac
0051fdf0: cmp      r0, #0
0051fdf4: beq      #0x51fe5c
0051fdf8: mov      r1, #0x3f800000
0051fdfc: mov      r0, fp
0051fe00: bl       #0x30e3ac
0051fe04: mov      r1, r0
0051fe08: ldr      r0, [sb, #0x50]
0051fe0c: bl       #0x30e4b4
0051fe10: cmp      r0, #0
0051fe14: beq      #0x51fe5c
0051fe18: ldr      fp, [r5, #4]
0051fe1c: mov      r1, #0x3f800000
0051fe20: mov      r0, fp
0051fe24: bl       #0x30eba4
0051fe28: mov      r1, r0
0051fe2c: ldr      r0, [sb, #0x48]
0051fe30: bl       #0x30e9ac
0051fe34: cmp      r0, #0
0051fe38: beq      #0x51fe5c
0051fe3c: mov      r1, #0x3f800000
0051fe40: mov      r0, fp
0051fe44: bl       #0x30e3ac
0051fe48: mov      r1, r0
0051fe4c: ldr      r0, [sb, #0x54]
0051fe50: bl       #0x30e4b4
0051fe54: cmp      r0, #0
0051fe58: bne      #0x5204b4
0051fe5c: add      r5, r5, #0x38
0051fe60: cmp      r5, r6
0051fe64: beq      #0x51fec0
0051fe68: ldr      sb, [sp, #0x4c]
0051fe6c: b        #0x51fdd4
0051fe70: ldr      r0, [sp, #0x54]
0051fe74: cmp      r0, #0
0051fe78: beq      #0x51fe94
0051fe7c: ldr      r1, [sp, #0x5c]
0051fe80: rsb      r1, r0, r1
0051fe84: bic      r1, r1, #7
0051fe88: cmp      r1, #0x80
0051fe8c: bhi      #0x520550
0051fe90: bl       #0x708f00
0051fe94: ldr      r0, [sp, #0x60]
0051fe98: cmp      r0, #0
0051fe9c: beq      #0x51feb8
0051fea0: ldr      r1, [sp, #0x68]
0051fea4: rsb      r1, r0, r1
0051fea8: bic      r1, r1, #7
0051feac: cmp      r1, #0x80
0051feb0: bhi      #0x520548
0051feb4: bl       #0x708f00
0051feb8: add      sp, sp, #0xb4
0051febc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0051fec0: ldr      r6, [sb, #0xac]
0051fec4: ldr      r5, [sb, #0xa8]
0051fec8: cmp      r5, r6
0051fecc: beq      #0x51fffc
0051fed0: mov      r7, #1
0051fed4: add      r8, sp, #0x54
0051fed8: add      sl, sp, #0x7c
0051fedc: ldr      sb, [r5]
0051fee0: mov      r1, #0x3f800000
0051fee4: mov      r0, sb
0051fee8: bl       #0x30eba4
0051feec: mov      r1, r0
0051fef0: ldr      r0, [r4, #0x44]
0051fef4: bl       #0x30e9ac
0051fef8: cmp      r0, #0
0051fefc: mov      r1, #0x3f800000
0051ff00: mov      r0, sb
0051ff04: beq      #0x51fff0
0051ff08: bl       #0x30e3ac
0051ff0c: mov      r1, r0
0051ff10: ldr      r0, [r4, #0x50]
0051ff14: bl       #0x30e4b4
0051ff18: cmp      r0, #0
0051ff1c: mov      r1, #0x3f800000
0051ff20: beq      #0x51fff0
0051ff24: ldr      sb, [r5, #4]
0051ff28: mov      r0, sb
0051ff2c: bl       #0x30eba4
0051ff30: mov      r1, r0
0051ff34: ldr      r0, [r4, #0x48]
0051ff38: bl       #0x30e9ac
0051ff3c: cmp      r0, #0
0051ff40: mov      r1, #0x3f800000
0051ff44: mov      r0, sb
0051ff48: beq      #0x51fff0
0051ff4c: bl       #0x30e3ac
0051ff50: mov      r1, r0
0051ff54: ldr      r0, [r4, #0x54]
0051ff58: bl       #0x30e4b4
0051ff5c: cmp      r0, #0
0051ff60: mov      r1, #0x3f800000
0051ff64: beq      #0x51fff0
0051ff68: ldr      sb, [r5, #8]
0051ff6c: mov      r0, sb
0051ff70: bl       #0x30eba4
0051ff74: mov      r1, r0
0051ff78: ldr      r0, [r4, #0x4c]
0051ff7c: bl       #0x30e9ac
0051ff80: cmp      r0, #0
0051ff84: mov      r1, #0x3f800000
0051ff88: mov      r0, sb
0051ff8c: beq      #0x51fff0
0051ff90: bl       #0x30e3ac
0051ff94: mov      r1, r0
0051ff98: ldr      r0, [r4, #0x58]
0051ff9c: bl       #0x30e4b4
0051ffa0: cmp      r0, #0
0051ffa4: add      r1, r5, #0xc
0051ffa8: add      r2, r5, #0x18
0051ffac: add      r3, r5, #0x2c
0051ffb0: beq      #0x51fff0
0051ffb4: ldr      r0, [sp, #0x4c]
0051ffb8: str      r7, [sp]
0051ffbc: bl       #0x51fa64
0051ffc0: ldr      r1, [sp, #0x58]
0051ffc4: ldr      r3, [sp, #0x5c]
0051ffc8: str      r0, [sp, #0x80]
0051ffcc: str      r5, [sp, #0x7c]
0051ffd0: cmp      r1, r3
0051ffd4: beq      #0x52056c
0051ffd8: str      r5, [r1]
0051ffdc: ldr      r3, [sp, #0x80]
0051ffe0: str      r3, [r1, #4]
0051ffe4: ldr      r3, [sp, #0x58]
0051ffe8: add      r3, r3, #8
0051ffec: str      r3, [sp, #0x58]
0051fff0: add      r5, r5, #0x38
0051fff4: cmp      r5, r6
0051fff8: bne      #0x51fedc
0051fffc: ldr      r3, [sp, #0x60]
00520000: ldr      r2, [sp, #0x64]
00520004: rsb      r2, r3, r2
00520008: asrs     r2, r2, #3
0052000c: str      r2, [sp, #0x34]
00520010: beq      #0x51fe70
00520014: add      r1, r4, #0xc0
00520018: add      r2, r4, #0x78
0052001c: str      r1, [sp, #0x38]
00520020: mov      r1, #0
00520024: str      r2, [sp, #0x28]
00520028: str      r1, [sp, #0x10]
0052002c: add      r2, sp, #0x90
00520030: add      r1, sp, #0x94
00520034: str      r2, [sp, #0x3c]
00520038: str      r1, [sp, #0x40]
0052003c: add      r2, sp, #0x98
00520040: add      r1, sp, #0x9c
00520044: str      r2, [sp, #0x44]
00520048: str      r1, [sp, #0x48]
0052004c: ldr      r1, [sp, #0x10]
00520050: mov      r0, r4
00520054: add      r2, r3, r1, lsl #3
00520058: ldr      r3, [r3, r1, lsl #3]
0052005c: str      r3, [sp, #0xc]
00520060: ldr      r6, [r2, #4]
00520064: ldr      r2, [r3, #0x24]
00520068: mov      r1, r6
0052006c: bl       #0x51e98c
00520070: ldr      r2, [sp, #0xc]
00520074: mov      r0, r4
00520078: ldr      r1, [r2, #0x24]
0052007c: mov      r2, r6
00520080: bl       #0x51e98c
00520084: ldr      r1, [r4, #0xc4]
00520088: ldr      r3, [r4, #0xc8]
0052008c: str      r0, [sp, #0xac]
00520090: cmp      r1, r3
00520094: beq      #0x5204a4
00520098: str      r0, [r1]
0052009c: ldr      r3, [r4, #0xc4]
005200a0: add      r3, r3, #4
005200a4: str      r3, [r4, #0xc4]
005200a8: ldr      r3, [sp, #0xc]
005200ac: mov      r1, r6
005200b0: mov      r0, r4
005200b4: ldr      r2, [r3, #0x28]
005200b8: bl       #0x51e98c
005200bc: ldr      r2, [sp, #0xc]
005200c0: mov      r0, r4
005200c4: ldr      r1, [r2, #0x28]
005200c8: mov      r2, r6
005200cc: bl       #0x51e98c
005200d0: ldr      r1, [r4, #0xc4]
005200d4: ldr      r3, [r4, #0xc8]
005200d8: str      r0, [sp, #0xa8]
005200dc: cmp      r1, r3
005200e0: beq      #0x520494
005200e4: str      r0, [r1]
005200e8: ldr      r3, [r4, #0xc4]
005200ec: add      r3, r3, #4
005200f0: str      r3, [r4, #0xc4]
005200f4: ldr      r3, [sp, #0x54]
005200f8: ldr      r2, [sp, #0x58]
005200fc: rsb      r2, r3, r2
00520100: asrs     r2, r2, #3
00520104: str      r2, [sp, #0x14]
00520108: beq      #0x520414
0052010c: add      r1, sp, #0x74
00520110: add      r2, sp, #0x4c
00520114: str      r1, [sp, #0x18]
00520118: str      r2, [sp, #0x20]
0052011c: add      r1, sp, #0x6c
00520120: add      r2, sp, #0x8c
00520124: str      r1, [sp, #0x24]
00520128: str      r2, [sp, #0x1c]
0052012c: add      r1, sp, #0xa0
00520130: add      r2, sp, #0xa4
00520134: mov      r7, #0
00520138: str      r1, [sp, #0x2c]
0052013c: str      r2, [sp, #0x30]
00520140: b        #0x520368
00520144: ldr      r1, [r5, #8]
00520148: ldr      r0, [r6, #8]
0052014c: bl       #0x30e3ac
00520150: ldr      r1, [r5, #0xc]
00520154: mov      sb, r0
00520158: ldr      r0, [r6, #0xc]
0052015c: bl       #0x30e3ac
00520160: ldr      r1, [r5, #0x10]
00520164: mov      fp, r0
00520168: ldr      r0, [r6, #0x10]
0052016c: bl       #0x30e3ac
00520170: ldr      r1, [r5, #0x20]
00520174: mov      sl, r0
00520178: ldr      r0, [r6, #0x20]
0052017c: bl       #0x30eba4
00520180: mov      r1, r0
00520184: bl       #0x30ed6c
00520188: mov      r1, sb
0052018c: mov      r3, r0
00520190: mov      r0, sb
00520194: str      r3, [sp, #8]
00520198: bl       #0x30ed6c
0052019c: mov      r1, fp
005201a0: mov      sb, r0
005201a4: mov      r0, fp
005201a8: bl       #0x30ed6c
005201ac: mov      r1, r0
005201b0: mov      r0, sb
005201b4: bl       #0x30eba4
005201b8: mov      r1, sl
005201bc: mov      sb, r0
005201c0: mov      r0, sl
005201c4: bl       #0x30ed6c
005201c8: mov      r1, r0
005201cc: mov      r0, sb
005201d0: bl       #0x30eba4
005201d4: ldr      r3, [sp, #8]
005201d8: mov      r1, r0
005201dc: mov      r0, r3
005201e0: bl       #0x30e2f8
005201e4: cmp      r0, #0
005201e8: beq      #0x52032c
005201ec: mov      r1, #0x42000000
005201f0: bic      r0, sl, #0x80000000
005201f4: add      r1, r1, #0xc80000
005201f8: bl       #0x30e70c
005201fc: cmp      r0, #0
00520200: beq      #0x52032c
00520204: mov      r1, r6
00520208: ldr      r2, [r8, #0x24]
0052020c: mov      r0, r4
00520210: bl       #0x51e98c
00520214: ldr      sl, [sp, #0x4c]
00520218: ldr      r1, [r8, #0x24]
0052021c: mov      r2, r6
00520220: mov      r0, sl
00520224: bl       #0x51e98c
00520228: str      r0, [sp, #0x9c]
0052022c: ldr      r1, [sl, #0xc4]
00520230: ldr      r3, [sl, #0xc8]
00520234: cmp      r1, r3
00520238: beq      #0x520464
0052023c: str      r0, [r1]
00520240: ldr      r3, [sl, #0xc4]
00520244: add      r3, r3, #4
00520248: str      r3, [sl, #0xc4]
0052024c: mov      r1, r6
00520250: ldr      r2, [r8, #0x28]
00520254: mov      r0, r4
00520258: bl       #0x51e98c
0052025c: ldr      sl, [sp, #0x4c]
00520260: ldr      r1, [r8, #0x28]
00520264: mov      r2, r6
00520268: mov      r0, sl
0052026c: bl       #0x51e98c
00520270: str      r0, [sp, #0x98]
00520274: ldr      r1, [sl, #0xc4]
00520278: ldr      r3, [sl, #0xc8]
0052027c: cmp      r1, r3
00520280: beq      #0x520454
00520284: str      r0, [r1]
00520288: ldr      r3, [sl, #0xc4]
0052028c: add      r3, r3, #4
00520290: str      r3, [sl, #0xc4]
00520294: ldr      r3, [sp, #0xc]
00520298: mov      r1, r5
0052029c: ldr      r0, [sp, #0x4c]
005202a0: ldr      r2, [r3, #0x24]
005202a4: bl       #0x51e98c
005202a8: ldr      r2, [sp, #0xc]
005202ac: mov      r0, r4
005202b0: ldr      r1, [r2, #0x24]
005202b4: mov      r2, r5
005202b8: bl       #0x51e98c
005202bc: ldr      r1, [r4, #0xc4]
005202c0: ldr      r3, [r4, #0xc8]
005202c4: str      r0, [sp, #0x94]
005202c8: cmp      r1, r3
005202cc: beq      #0x520444
005202d0: str      r0, [r1]
005202d4: ldr      r3, [r4, #0xc4]
005202d8: add      r3, r3, #4
005202dc: str      r3, [r4, #0xc4]
005202e0: ldr      r3, [sp, #0xc]
005202e4: mov      r1, r5
005202e8: ldr      r0, [sp, #0x4c]
005202ec: ldr      r2, [r3, #0x28]
005202f0: bl       #0x51e98c
005202f4: ldr      r3, [sp, #0xc]
005202f8: mov      r0, r4
005202fc: mov      r2, r5
00520300: ldr      r1, [r3, #0x28]
00520304: bl       #0x51e98c
00520308: ldr      r1, [r4, #0xc4]
0052030c: ldr      r3, [r4, #0xc8]
00520310: str      r0, [sp, #0x90]
00520314: cmp      r1, r3
00520318: beq      #0x520434
0052031c: str      r0, [r1]
00520320: ldr      r3, [r4, #0xc4]
00520324: add      r3, r3, #4
00520328: str      r3, [r4, #0xc4]
0052032c: ldr      r0, [sp, #0x18]
00520330: ldr      r1, [sp, #0x28]
00520334: ldr      r2, [sp, #0x20]
00520338: bl       #0x51df98
0052033c: ldr      r1, [sp, #0x4c]
00520340: ldr      r0, [sp, #0x24]
00520344: ldr      r2, [sp, #0x1c]
00520348: add      r1, r1, #0x78
0052034c: str      r4, [sp, #0x8c]
00520350: bl       #0x51df98
00520354: ldr      r1, [sp, #0x14]
00520358: add      r7, r7, #1
0052035c: cmp      r7, r1
00520360: beq      #0x520414
00520364: ldr      r3, [sp, #0x54]
00520368: ldr      r1, [sp, #0x10]
0052036c: add      r2, r3, r7, lsl #3
00520370: ldr      r8, [r3, r7, lsl #3]
00520374: cmp      r1, #0
00520378: ldr      r5, [r2, #4]
0052037c: bne      #0x520144
00520380: mov      r1, r5
00520384: ldr      r2, [r8, #0x24]
00520388: ldr      r0, [sp, #0x4c]
0052038c: bl       #0x51e98c
00520390: ldr      sl, [sp, #0x4c]
00520394: ldr      r1, [r8, #0x24]
00520398: mov      r2, r5
0052039c: mov      r0, sl
005203a0: bl       #0x51e98c
005203a4: str      r0, [sp, #0xa4]
005203a8: ldr      r1, [sl, #0xc4]
005203ac: ldr      r3, [sl, #0xc8]
005203b0: cmp      r1, r3
005203b4: beq      #0x520484
005203b8: str      r0, [r1]
005203bc: ldr      r3, [sl, #0xc4]
005203c0: add      r3, r3, #4
005203c4: str      r3, [sl, #0xc4]
005203c8: mov      r1, r5
005203cc: ldr      r2, [r8, #0x28]
005203d0: ldr      r0, [sp, #0x4c]
005203d4: bl       #0x51e98c
005203d8: ldr      sl, [sp, #0x4c]
005203dc: ldr      r1, [r8, #0x28]
005203e0: mov      r2, r5
005203e4: mov      r0, sl
005203e8: bl       #0x51e98c
005203ec: str      r0, [sp, #0xa0]
005203f0: ldr      r1, [sl, #0xc4]
005203f4: ldr      r3, [sl, #0xc8]
005203f8: cmp      r1, r3
005203fc: beq      #0x520474
00520400: str      r0, [r1]
00520404: ldr      r3, [sl, #0xc4]
00520408: add      r3, r3, #4
0052040c: str      r3, [sl, #0xc4]
00520410: b        #0x520144
00520414: ldr      r2, [sp, #0x10]
00520418: ldr      r3, [sp, #0x34]
0052041c: add      r2, r2, #1
00520420: cmp      r2, r3
00520424: str      r2, [sp, #0x10]
00520428: beq      #0x51fe70
0052042c: ldr      r3, [sp, #0x60]
00520430: b        #0x52004c
00520434: ldr      r0, [sp, #0x38]
00520438: ldr      r2, [sp, #0x3c]
0052043c: bl       #0x51c834
00520440: b        #0x52032c
00520444: ldr      r0, [sp, #0x38]
00520448: ldr      r2, [sp, #0x40]
0052044c: bl       #0x51c834
00520450: b        #0x5202e0
00520454: add      r0, sl, #0xc0
00520458: ldr      r2, [sp, #0x44]
0052045c: bl       #0x51c834
00520460: b        #0x520294
00520464: add      r0, sl, #0xc0
00520468: ldr      r2, [sp, #0x48]
0052046c: bl       #0x51c834
00520470: b        #0x52024c
00520474: add      r0, sl, #0xc0
00520478: ldr      r2, [sp, #0x2c]
0052047c: bl       #0x51c834
00520480: b        #0x520144
00520484: add      r0, sl, #0xc0
00520488: ldr      r2, [sp, #0x30]
0052048c: bl       #0x51c834
00520490: b        #0x5203c8
00520494: ldr      r0, [sp, #0x38]
00520498: add      r2, sp, #0xa8
0052049c: bl       #0x51c834
005204a0: b        #0x5200f4
005204a4: ldr      r0, [sp, #0x38]
005204a8: add      r2, sp, #0xac
005204ac: bl       #0x51c834
005204b0: b        #0x5200a8
005204b4: ldr      fp, [r5, #8]
005204b8: mov      r1, #0x3f800000
005204bc: mov      r0, fp
005204c0: bl       #0x30eba4
005204c4: mov      r1, r0
005204c8: ldr      r0, [sb, #0x4c]
005204cc: bl       #0x30e9ac
005204d0: cmp      r0, #0
005204d4: beq      #0x51fe5c
005204d8: mov      r1, #0x3f800000
005204dc: mov      r0, fp
005204e0: bl       #0x30e3ac
005204e4: mov      r1, r0
005204e8: ldr      r0, [sb, #0x58]
005204ec: bl       #0x30e4b4
005204f0: cmp      r0, #0
005204f4: beq      #0x51fe5c
005204f8: add      r1, r5, #0xc
005204fc: add      r3, r5, #0x2c
00520500: mov      r0, r4
00520504: add      r2, r5, #0x18
00520508: str      r7, [sp]
0052050c: bl       #0x51fa64
00520510: ldr      r1, [sp, #0x64]
00520514: ldr      r3, [sp, #0x68]
00520518: str      r0, [sp, #0x88]
0052051c: str      r5, [sp, #0x84]
00520520: cmp      r1, r3
00520524: beq      #0x520558
00520528: str      r5, [r1]
0052052c: ldr      r3, [sp, #0x88]
00520530: str      r3, [r1, #4]
00520534: ldr      r3, [sp, #0x64]
00520538: ldr      sb, [sp, #0x4c]
0052053c: add      r3, r3, #8
00520540: str      r3, [sp, #0x64]
00520544: b        #0x51fe5c
00520548: bl       #0x310440
0052054c: b        #0x51feb8
00520550: bl       #0x310440
00520554: b        #0x51fe94
00520558: mov      r0, r8
0052055c: mov      r2, sl
00520560: bl       #0x51c8fc
00520564: ldr      sb, [sp, #0x4c]
00520568: b        #0x51fe5c
0052056c: mov      r0, r8
00520570: mov      r2, sl
00520574: add      r5, r5, #0x38
00520578: bl       #0x51c8fc
0052057c: cmp      r5, r6
00520580: bne      #0x51fedc
00520584: b        #0x51fffc

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

# _ZNSt4priv8_Rb_treeI7CompPosSt4lessIS1_ESt4pairIKS1_P12PFGInnerNodeENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE13insert_uniqueERKS8_
0051eca8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0051ecac: sub      sp, sp, #0x1c
0051ecb0: str      r1, [sp, #0xc]
0051ecb4: ldr      r5, [r1, #4]
0051ecb8: mov      r4, r0
0051ecbc: mov      r6, r2
0051ecc0: cmp      r5, #0
0051ecc4: moveq    ip, r1
0051ecc8: beq      #0x51ee4c
0051eccc: ldr      r8, [r2]
0051ecd0: ldr      r7, [r5, #0x10]
0051ecd4: mov      r0, r8
0051ecd8: mov      fp, r5
0051ecdc: mov      r1, r7
0051ece0: bl       #0x30e3ac
0051ece4: movw     r1, #0xb717
0051ece8: bic      r0, r0, #0x80000000
0051ecec: movt     r1, #0x38d1
0051ecf0: bl       #0x30e70c
0051ecf4: cmp      r0, #0
0051ecf8: beq      #0x51ed60
0051ecfc: ldr      sb, [r6, #4]
0051ed00: ldr      sl, [r5, #0x14]
0051ed04: mov      r0, sb
0051ed08: mov      r1, sl
0051ed0c: bl       #0x30e3ac
0051ed10: movw     r1, #0xb717
0051ed14: bic      r0, r0, #0x80000000
0051ed18: movt     r1, #0x38d1
0051ed1c: bl       #0x30e70c
0051ed20: cmp      r0, #0
0051ed24: beq      #0x51ee28
0051ed28: ldr      r0, [r6, #8]
0051ed2c: ldr      r1, [r5, #0x18]
0051ed30: bl       #0x30e70c
0051ed34: cmp      r0, #0
0051ed38: mov      r2, #0
0051ed3c: bne      #0x51ed78
0051ed40: uxtb     r2, r2
0051ed44: cmp      r2, #0
0051ed48: ldrne    r3, [r5, #8]
0051ed4c: ldreq    r3, [r5, #0xc]
0051ed50: cmp      r3, #0
0051ed54: beq      #0x51ed94
0051ed58: mov      r5, r3
0051ed5c: b        #0x51ecd0
0051ed60: mov      r0, r7
0051ed64: mov      r1, r8
0051ed68: bl       #0x30e2f8
0051ed6c: cmp      r0, #0
0051ed70: mov      r2, #0
0051ed74: beq      #0x51ed40
0051ed78: mov      r2, #1
0051ed7c: uxtb     r2, r2
0051ed80: cmp      r2, #0
0051ed84: ldrne    r3, [r5, #8]
0051ed88: ldreq    r3, [r5, #0xc]
0051ed8c: cmp      r3, #0
0051ed90: bne      #0x51ed58
0051ed94: cmp      r2, #0
0051ed98: moveq    sl, r5
0051ed9c: bne      #0x51ee48
0051eda0: mov      r1, r8
0051eda4: mov      r0, r7
0051eda8: bl       #0x30e3ac
0051edac: movw     r1, #0xb717
0051edb0: bic      r0, r0, #0x80000000
0051edb4: movt     r1, #0x38d1
0051edb8: bl       #0x30e70c
0051edbc: cmp      r0, #0
0051edc0: beq      #0x51eeac
0051edc4: ldr      r8, [fp, #0x14]
0051edc8: ldr      r7, [r6, #4]
0051edcc: mov      r0, r8
0051edd0: mov      r1, r7
0051edd4: bl       #0x30e3ac
0051edd8: movw     r1, #0xb717
0051eddc: bic      r0, r0, #0x80000000
0051ede0: movt     r1, #0x38d1
0051ede4: bl       #0x30e70c
0051ede8: cmp      r0, #0
0051edec: beq      #0x51ef10
0051edf0: ldr      r0, [fp, #0x18]
0051edf4: ldr      r1, [r6, #8]
0051edf8: bl       #0x30e70c
0051edfc: cmp      r0, #0
0051ee00: mov      r3, #0
0051ee04: bne      #0x51eec4
0051ee08: uxtb     r3, r3
0051ee0c: cmp      r3, #0
0051ee10: streq    sl, [r4]
0051ee14: strbeq   r3, [r4, #4]
0051ee18: bne      #0x51eedc
0051ee1c: mov      r0, r4
0051ee20: add      sp, sp, #0x1c
0051ee24: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0051ee28: mov      r0, sb
0051ee2c: mov      r1, sl
0051ee30: bl       #0x30e70c
0051ee34: cmp      r0, #0
0051ee38: mov      r2, #0
0051ee3c: beq      #0x51ed40
0051ee40: mov      r2, #1
0051ee44: b        #0x51ed7c
0051ee48: mov      ip, r5
0051ee4c: ldr      r2, [sp, #0xc]
0051ee50: ldr      r3, [r2, #8]
0051ee54: cmp      ip, r3
0051ee58: beq      #0x51ef80
0051ee5c: ldrb     r3, [ip]
0051ee60: cmp      r3, #0
0051ee64: bne      #0x51ee78
0051ee68: ldr      r3, [ip, #4]
0051ee6c: ldr      r3, [r3, #4]
0051ee70: cmp      ip, r3
0051ee74: beq      #0x51ef68
0051ee78: ldr      fp, [ip, #8]
0051ee7c: cmp      fp, #0
0051ee80: bne      #0x51ee8c
0051ee84: b        #0x51ef30
0051ee88: mov      fp, r3
0051ee8c: ldr      r3, [fp, #0xc]
0051ee90: cmp      r3, #0
0051ee94: bne      #0x51ee88
0051ee98: mov      r5, ip
0051ee9c: ldr      r8, [r6]
0051eea0: ldr      r7, [fp, #0x10]
0051eea4: mov      sl, fp
0051eea8: b        #0x51eda0
0051eeac: mov      r0, r8
0051eeb0: mov      r1, r7
0051eeb4: bl       #0x30e2f8
0051eeb8: cmp      r0, #0
0051eebc: mov      r3, #0
0051eec0: beq      #0x51ee08
0051eec4: mov      r3, #1
0051eec8: uxtb     r3, r3
0051eecc: cmp      r3, #0
0051eed0: streq    sl, [r4]
0051eed4: strbeq   r3, [r4, #4]
0051eed8: beq      #0x51ee1c
0051eedc: mov      ip, #0
0051eee0: mov      r2, r5
0051eee4: mov      r3, r6
0051eee8: ldr      r1, [sp, #0xc]
0051eeec: add      r0, sp, #0x10
0051eef0: str      ip, [sp, #4]
0051eef4: str      ip, [sp]
0051eef8: bl       #0x51ead4
0051eefc: ldr      r3, [sp, #0x10]
0051ef00: mov      r2, #1
0051ef04: strb     r2, [r4, #4]
0051ef08: str      r3, [r4]
0051ef0c: b        #0x51ee1c
0051ef10: mov      r0, r8
0051ef14: mov      r1, r7
0051ef18: bl       #0x30e70c
0051ef1c: cmp      r0, #0
0051ef20: mov      r3, #0
0051ef24: beq      #0x51ee08
0051ef28: mov      r3, #1
0051ef2c: b        #0x51eec8
0051ef30: ldr      r3, [ip, #4]
0051ef34: ldr      r5, [r3, #8]
0051ef38: cmp      r5, ip
0051ef3c: beq      #0x51ef48
0051ef40: b        #0x51efb0
0051ef44: mov      r3, fp
0051ef48: ldr      fp, [r3, #4]
0051ef4c: ldr      r2, [fp, #8]
0051ef50: cmp      r2, r3
0051ef54: beq      #0x51ef44
0051ef58: ldr      r8, [r6]
0051ef5c: ldr      r7, [fp, #0x10]
0051ef60: mov      sl, fp
0051ef64: b        #0x51eda0
0051ef68: ldr      fp, [ip, #0xc]
0051ef6c: mov      r5, ip
0051ef70: ldr      r8, [r6]
0051ef74: ldr      r7, [fp, #0x10]
0051ef78: mov      sl, fp
0051ef7c: b        #0x51eda0
0051ef80: mov      r1, r2
0051ef84: mov      r3, r6
0051ef88: mov      r2, ip
0051ef8c: mov      lr, #0
0051ef90: add      r0, sp, #0x14
0051ef94: stm      sp, {ip, lr}
0051ef98: bl       #0x51ead4
0051ef9c: ldr      r3, [sp, #0x14]
0051efa0: mov      r2, #1
0051efa4: strb     r2, [r4, #4]
0051efa8: str      r3, [r4]
0051efac: b        #0x51ee1c
0051efb0: mov      fp, r3
0051efb4: b        #0x51ee98

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

# _ZN6PFRoom9_PostLoadEv
00521408: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0052140c: sub      sp, sp, #0xc
00521410: str      r0, [sp, #4]
00521414: ldr      sl, [r0, #0x34]
00521418: ldr      r7, [r0, #0x30]
0052141c: rsb      r2, r7, sl
00521420: lsrs     r3, r2, #2
00521424: beq      #0x521538
00521428: mov      r3, r7
0052142c: mov      sb, #1
00521430: mov      fp, #0
00521434: ldr      r8, [r7, fp]
00521438: ldr      r1, [r8, #0x20]
0052143c: tst      r1, #0x4000000
00521440: bne      #0x521520
00521444: cmp      sb, r2, asr #2
00521448: bhs      #0x52150c
0052144c: lsl      r5, sb, #2
00521450: mov      r4, sb
00521454: ldr      r6, [r3, r5]
00521458: add      r4, r4, #1
0052145c: add      r5, r5, #4
00521460: ldr      r3, [r6, #0x20]
00521464: tst      r3, #0x4000000
00521468: bne      #0x5214fc
0052146c: ldr      r1, [r6, #0x50]
00521470: ldr      r0, [r8, #0x44]
00521474: bl       #0x30e9ac
00521478: cmp      r0, #0
0052147c: beq      #0x5214fc
00521480: ldr      r1, [r6, #0x44]
00521484: ldr      r0, [r8, #0x50]
00521488: bl       #0x30e4b4
0052148c: cmp      r0, #0
00521490: beq      #0x5214fc
00521494: ldr      r1, [r6, #0x54]
00521498: ldr      r0, [r8, #0x48]
0052149c: bl       #0x30e9ac
005214a0: cmp      r0, #0
005214a4: beq      #0x5214fc
005214a8: ldr      r1, [r6, #0x48]
005214ac: ldr      r0, [r8, #0x54]
005214b0: bl       #0x30e4b4
005214b4: cmp      r0, #0
005214b8: beq      #0x5214fc
005214bc: ldr      r1, [r6, #0x58]
005214c0: ldr      r0, [r8, #0x4c]
005214c4: bl       #0x30e9ac
005214c8: cmp      r0, #0
005214cc: beq      #0x5214fc
005214d0: ldr      r1, [r6, #0x4c]
005214d4: ldr      r0, [r8, #0x58]
005214d8: bl       #0x30e4b4
005214dc: cmp      r0, #0
005214e0: beq      #0x5214fc
005214e4: mov      r1, r6
005214e8: mov      r0, r8
005214ec: bl       #0x51fd78
005214f0: ldr      r3, [sp, #4]
005214f4: ldr      r7, [r3, #0x30]
005214f8: ldr      sl, [r3, #0x34]
005214fc: rsb      r2, r7, sl
00521500: cmp      r4, r2, asr #2
00521504: mov      r3, r7
00521508: blo      #0x521454
0052150c: mov      r0, r8
00521510: bl       #0x51bee8
00521514: ldr      r3, [sp, #4]
00521518: ldr      sl, [r3, #0x34]
0052151c: ldr      r7, [r3, #0x30]
00521520: rsb      r2, r7, sl
00521524: cmp      sb, r2, asr #2
00521528: add      fp, fp, #4
0052152c: add      sb, sb, #1
00521530: movlo    r3, r7
00521534: blo      #0x521434
00521538: add      sp, sp, #0xc
0052153c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
