
# _ZSt11__push_heapIPN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeEiS7_NS6_6_ECompEEvT_T0_SB_T1_T2_.clone.15
00529398: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0052939c: subs     r5, r1, #0
005293a0: mov      r4, r0
005293a4: mov      sb, r2
005293a8: ble      #0x52942c
005293ac: sub      r7, r5, #1
005293b0: asr      r7, r7, #1
005293b4: mov      sl, #0xc
005293b8: mul      r8, sl, r7
005293bc: ldr      r1, [sb, #8]
005293c0: add      r6, r4, r8
005293c4: ldr      r0, [r6, #8]
005293c8: bl       #0x30e2f8
005293cc: mul      r3, sl, r5
005293d0: cmp      r0, #0
005293d4: add      r2, r6, #4
005293d8: add      r1, r4, r3
005293dc: beq      #0x52942c
005293e0: ldr      ip, [r4, r8]
005293e4: cmp      r7, #0
005293e8: sub      r0, r7, #1
005293ec: str      ip, [r4, r3]
005293f0: ldr      r3, [r6, #4]
005293f4: mov      r5, r7
005293f8: asr      r7, r0, #1
005293fc: str      r3, [r1, #4]
00529400: ldr      r3, [r2, #4]
00529404: str      r3, [r1, #8]
00529408: bne      #0x5293b8
0052940c: mov      r3, sb
00529410: ldr      r1, [r3], #4
00529414: str      r1, [r6]
00529418: ldr      r1, [sb, #4]
0052941c: str      r1, [r6, #4]
00529420: ldr      r3, [r3, #4]
00529424: str      r3, [r2, #4]
00529428: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0052942c: mov      r6, #0xc
00529430: mla      r6, r6, r5, r4
00529434: add      r2, r6, #4
00529438: b        #0x52940c

# _ZNSaINSt4priv13_Rb_tree_nodeISt4pairIKjN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeEEEEE8allocateEjPKv.clone.18
00529734: str      lr, [sp, #-4]!
00529738: sub      sp, sp, #0xc
0052973c: add      r0, sp, #8
00529740: mov      r3, #0x20
00529744: str      r3, [r0, #-4]!
00529748: bl       #0x708ec0
0052974c: add      sp, sp, #0xc
00529750: ldm      sp!, {pc}

# _ZNSaINSt4priv10_List_nodeIPK12PFGInnerEdgeEEE8allocateEjPKv.clone.5
0052aa84: str      lr, [sp, #-4]!
0052aa88: sub      sp, sp, #0xc
0052aa8c: add      r0, sp, #8
0052aa90: mov      r3, #0xc
0052aa94: str      r3, [r0, #-4]!
0052aa98: bl       #0x708ec0
0052aa9c: add      sp, sp, #0xc
0052aaa0: ldm      sp!, {pc}

# _ZNSt4listIPK12PFGInnerEdgeSaIS2_EE6resizeEjRKS2_.clone.3
0052aaa4: push     {r4, r5, r6, lr}
0052aaa8: mov      r5, r0
0052aaac: ldr      r0, [r0]
0052aab0: cmp      r0, r5
0052aab4: beq      #0x52aae0
0052aab8: b        #0x52aac0
0052aabc: mov      r0, r4
0052aac0: ldr      r4, [r0]
0052aac4: ldr      r3, [r0, #4]
0052aac8: mov      r1, #0xc
0052aacc: str      r4, [r3]
0052aad0: str      r3, [r4, #4]
0052aad4: bl       #0x708f00
0052aad8: cmp      r5, r4
0052aadc: bne      #0x52aabc
0052aae0: pop      {r4, r5, r6, pc}

# _ZN7PFWorld17ValidateDirectionER7Point3DIfERKS1_fj
00525944: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00525948: ldr      r7, [r1]
0052594c: ldr      r8, [r1, #4]
00525950: ldr      sl, [r1, #8]
00525954: sub      sp, sp, #0xcc
00525958: mov      ip, #0
0052595c: mov      lr, #0
00525960: mov      r4, r1
00525964: mov      r5, r2
00525968: mov      r1, r2
0052596c: add      r6, sp, #0xc4
00525970: add      r2, sp, #0xb0
00525974: add      r3, sp, #0x1c
00525978: str      r7, [sp, #0xa4]
0052597c: str      ip, [sp, #0x3c]
00525980: str      r8, [sp, #0xa8]
00525984: str      sl, [sp, #0xac]
00525988: str      lr, [sp, #8]
0052598c: str      ip, [sp, #0xb0]
00525990: str      ip, [sp, #0xb4]
00525994: str      ip, [sp, #0xb8]
00525998: str      ip, [sp, #0x1c]
0052599c: str      ip, [sp, #0x20]
005259a0: str      ip, [sp, #0x24]
005259a4: str      ip, [sp, #0x28]
005259a8: str      ip, [sp, #0x2c]
005259ac: str      ip, [sp, #0x30]
005259b0: str      ip, [sp, #0x34]
005259b4: str      ip, [sp, #0x38]
005259b8: str      lr, [sp]
005259bc: str      r6, [sp, #4]
005259c0: mov      r7, r0
005259c4: ldr      sb, [sp, #0xf0]
005259c8: bl       #0x5256d4
005259cc: cmp      r0, #0
005259d0: beq      #0x525c50
005259d4: ldr      r3, [sp, #0xc4]
005259d8: ldr      r3, [r3, #0x24]
005259dc: cmp      r3, #0
005259e0: bne      #0x525c5c
005259e4: add      r2, sp, #0xa4
005259e8: mov      r0, r2
005259ec: str      r2, [sp, #0x14]
005259f0: bl       #0x34d0b0
005259f4: mov      r1, #0x41000000
005259f8: mov      r8, r0
005259fc: add      r1, r1, #0x200000
00525a00: ldr      r0, [r0]
00525a04: bl       #0x30ed6c
00525a08: mov      r1, #0x41000000
00525a0c: str      r0, [r8]
00525a10: add      r1, r1, #0x200000
00525a14: ldr      r0, [r8, #4]
00525a18: bl       #0x30ed6c
00525a1c: mov      r1, #0x41000000
00525a20: str      r0, [r8, #4]
00525a24: add      r1, r1, #0x200000
00525a28: ldr      r0, [r8, #8]
00525a2c: bl       #0x30ed6c
00525a30: str      r0, [r8, #8]
00525a34: ldr      r0, [r5, #4]
00525a38: ldr      r1, [sp, #0xa8]
00525a3c: bl       #0x30eba4
00525a40: ldr      r1, [sp, #0xac]
00525a44: mov      r8, r0
00525a48: ldr      r0, [r5, #8]
00525a4c: bl       #0x30eba4
00525a50: ldr      r1, [sp, #0xa4]
00525a54: mov      sl, r0
00525a58: ldr      r0, [r5]
00525a5c: bl       #0x30eba4
00525a60: mov      ip, #0
00525a64: str      r0, [sp, #0x98]
00525a68: mov      r2, ip
00525a6c: add      r1, sp, #0x98
00525a70: mov      r0, r7
00525a74: mov      r3, ip
00525a78: str      r8, [sp, #0x9c]
00525a7c: str      sl, [sp, #0xa0]
00525a80: str      r6, [sp, #4]
00525a84: str      ip, [sp]
00525a88: str      ip, [sp, #8]
00525a8c: bl       #0x525508
00525a90: cmp      r0, #0
00525a94: beq      #0x525ab4
00525a98: ldr      r3, [sp, #0xc4]
00525a9c: ldr      r3, [r3, #0x24]
00525aa0: cmp      r3, #0
00525aa4: beq      #0x525d58
00525aa8: and      sb, sb, r3
00525aac: cmp      r3, sb
00525ab0: beq      #0x525d58
00525ab4: ldr      r3, [r5, #4]
00525ab8: ldr      r6, [r5]
00525abc: mov      r2, #0
00525ac0: mov      r0, r3
00525ac4: ldr      r1, [sp, #0xa8]
00525ac8: str      r2, [sp, #0xc0]
00525acc: str      r2, [sp, #0xbc]
00525ad0: str      r3, [sp, #0x74]
00525ad4: str      r6, [sp, #0x70]
00525ad8: bl       #0x30eba4
00525adc: ldr      r1, [sp, #0xa4]
00525ae0: str      r0, [sp, #0x7c]
00525ae4: mov      r0, r6
00525ae8: bl       #0x30eba4
00525aec: ldr      r3, [sp, #0x20]
00525af0: ldr      ip, [sp, #0x1c]
00525af4: ldr      lr, [sp, #0x28]
00525af8: str      r3, [sp, #0x54]
00525afc: ldr      r3, [sp, #0x38]
00525b00: ldr      sb, [sp, #0x2c]
00525b04: ldr      fp, [sp, #0x34]
00525b08: str      r3, [sp, #0x4c]
00525b0c: ldr      r3, [sp, #0x20]
00525b10: add      r8, sp, #0x70
00525b14: add      r6, sp, #0x60
00525b18: str      r3, [sp, #0x64]
00525b1c: ldr      r3, [sp, #0x38]
00525b20: add      sl, sp, #0xbc
00525b24: str      r0, [sp, #0x78]
00525b28: mov      r1, r6
00525b2c: mov      r0, r8
00525b30: mov      r2, sl
00525b34: str      ip, [sp, #0x50]
00525b38: str      lr, [sp, #0x40]
00525b3c: str      sb, [sp, #0x44]
00525b40: str      fp, [sp, #0x48]
00525b44: str      ip, [sp, #0x60]
00525b48: str      lr, [sp, #0x68]
00525b4c: str      sb, [sp, #0x6c]
00525b50: str      fp, [sp, #0x58]
00525b54: str      r3, [sp, #0x5c]
00525b58: bl       #0x525310
00525b5c: cmp      r0, #0
00525b60: beq      #0x525d20
00525b64: mov      sl, #0
00525b68: str      sl, [sp, #0xac]
00525b6c: ldr      r1, [r6, #8]
00525b70: ldr      r0, [r6]
00525b74: bl       #0x30e3ac
00525b78: ldr      sb, [r6, #4]
00525b7c: ldr      r6, [r6, #0xc]
00525b80: str      r0, [sp, #0x8c]
00525b84: mov      r0, sb
00525b88: mov      r1, r6
00525b8c: bl       #0x30e3ac
00525b90: add      r8, sp, #0x8c
00525b94: str      r0, [sp, #0x90]
00525b98: ldr      r1, [sp, #0x14]
00525b9c: mov      r0, r8
00525ba0: str      sl, [sp, #0x94]
00525ba4: bl       #0x313058
00525ba8: movw     r1, #0xfdb
00525bac: bic      r0, r0, #0x80000000
00525bb0: movt     r1, #0x3fc9
00525bb4: bl       #0x30e70c
00525bb8: cmp      r0, #0
00525bbc: beq      #0x525c6c
00525bc0: mov      r0, r8
00525bc4: bl       #0x34d0b0
00525bc8: mov      r1, #0x41000000
00525bcc: ldr      r0, [sp, #0x90]
00525bd0: add      r1, r1, #0x200000
00525bd4: bl       #0x30ed6c
00525bd8: ldr      r1, [r5, #4]
00525bdc: bl       #0x30eba4
00525be0: mov      r1, #0x41000000
00525be4: mov      r6, r0
00525be8: add      r1, r1, #0x200000
00525bec: ldr      r0, [sp, #0x94]
00525bf0: bl       #0x30ed6c
00525bf4: ldr      r1, [r5, #8]
00525bf8: bl       #0x30eba4
00525bfc: mov      r1, #0x41000000
00525c00: mov      r8, r0
00525c04: add      r1, r1, #0x200000
00525c08: ldr      r0, [sp, #0x8c]
00525c0c: bl       #0x30ed6c
00525c10: ldr      r1, [r5]
00525c14: bl       #0x30eba4
00525c18: mov      ip, #0
00525c1c: str      r0, [sp, #0x80]
00525c20: mov      r2, ip
00525c24: mov      r0, r7
00525c28: add      r1, sp, #0x80
00525c2c: mov      r3, ip
00525c30: str      r6, [sp, #0x84]
00525c34: str      r8, [sp, #0x88]
00525c38: str      ip, [sp]
00525c3c: str      ip, [sp, #4]
00525c40: str      ip, [sp, #8]
00525c44: bl       #0x525508
00525c48: cmp      r0, #0
00525c4c: bne      #0x525c90
00525c50: mov      r0, #0
00525c54: add      sp, sp, #0xcc
00525c58: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00525c5c: and      r2, sb, r3
00525c60: cmp      r3, r2
00525c64: bne      #0x525c50
00525c68: b        #0x5259e4
00525c6c: add      r1, sp, #0x8c
00525c70: ldm      r1, {r1, r2, r3}
00525c74: add      r2, r2, #0x80000000
00525c78: add      r3, r3, #0x80000000
00525c7c: add      r1, r1, #0x80000000
00525c80: str      r1, [sp, #0x8c]
00525c84: str      r2, [sp, #0x90]
00525c88: str      r3, [sp, #0x94]
00525c8c: b        #0x525bc0
00525c90: ldr      r0, [r4]
00525c94: ldr      r7, [r4, #4]
00525c98: ldr      r6, [r4, #8]
00525c9c: mov      r1, r0
00525ca0: bl       #0x30ed6c
00525ca4: mov      r1, r7
00525ca8: mov      r5, r0
00525cac: mov      r0, r7
00525cb0: bl       #0x30ed6c
00525cb4: mov      r1, r0
00525cb8: mov      r0, r5
00525cbc: bl       #0x30eba4
00525cc0: mov      r1, r6
00525cc4: mov      r5, r0
00525cc8: mov      r0, r6
00525ccc: bl       #0x30ed6c
00525cd0: mov      r1, r0
00525cd4: mov      r0, r5
00525cd8: bl       #0x30eba4
00525cdc: bl       #0x30e124
00525ce0: ldr      r1, [sp, #0x90]
00525ce4: mov      r5, r0
00525ce8: bl       #0x30ed6c
00525cec: ldr      r1, [sp, #0x94]
00525cf0: mov      r6, r0
00525cf4: mov      r0, r5
00525cf8: bl       #0x30ed6c
00525cfc: ldr      r1, [sp, #0x8c]
00525d00: mov      r7, r0
00525d04: mov      r0, r5
00525d08: bl       #0x30ed6c
00525d0c: str      r7, [r4, #8]
00525d10: str      r0, [r4]
00525d14: str      r6, [r4, #4]
00525d18: mov      r0, #1
00525d1c: b        #0x525c54
00525d20: add      r6, sp, #0x50
00525d24: mov      r0, r8
00525d28: mov      r1, r6
00525d2c: mov      r2, sl
00525d30: bl       #0x525310
00525d34: cmp      r0, #0
00525d38: bne      #0x525b64
00525d3c: add      r6, sp, #0x40
00525d40: mov      r0, r8
00525d44: mov      r2, sl
00525d48: mov      r1, r6
00525d4c: bl       #0x525310
00525d50: cmp      r0, #0
00525d54: bne      #0x525b64
00525d58: mov      r0, #1
00525d5c: b        #0x525c54

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

# _ZNK3sfc4math5graph4EdgeI12PFGInnerNodefE9getToNodeEv
0051bc50: ldr      r0, [r0, #8]
0051bc54: bx       lr

# _ZNK12PFGInnerEdge9GetSourceEv
0051b844: push     {r4, lr}
0051b848: ldr      r3, [r0]
0051b84c: mov      lr, pc
0051b850: ldr      pc, [r3, #4]
0051b854: add      r0, r0, #8
0051b858: pop      {r4, pc}

# _ZNK3sfc4math5graph4NodeIjE7isValidEv
0051c0fc: mov      r0, #1
0051c100: bx       lr

# _ZNK3sfc4math5graph11GraphSparseI12PFGInnerEdgeE8getEdgesEjRSt4listIPKS3_SaIS7_EE
0052ac30: push     {r4, r5, r6, r7, r8, lr}
0052ac34: ldr      r4, [r0, #8]
0052ac38: mov      r6, r2
0052ac3c: add      r3, r0, #4
0052ac40: cmp      r4, #0
0052ac44: beq      #0x52ad04
0052ac48: mov      r0, r3
0052ac4c: b        #0x52ac54
0052ac50: mov      r4, r2
0052ac54: ldr      r2, [r4, #0x10]
0052ac58: cmp      r1, r2
0052ac5c: ldrhi    r2, [r4, #0xc]
0052ac60: ldrls    r2, [r4, #8]
0052ac64: movhi    r4, r0
0052ac68: mov      r0, r4
0052ac6c: cmp      r2, #0
0052ac70: bne      #0x52ac50
0052ac74: cmp      r3, r4
0052ac78: beq      #0x52ad44
0052ac7c: ldr      r2, [r4, #0x10]
0052ac80: cmp      r1, r2
0052ac84: blo      #0x52ad04
0052ac88: cmp      r3, r4
0052ac8c: beq      #0x52ad44
0052ac90: ldr      r3, [r4, #0x14]
0052ac94: ldr      r5, [r3, #0x34]
0052ac98: add      r2, r3, #0x2c
0052ac9c: cmp      r2, r5
0052aca0: beq      #0x52acfc
0052aca4: mov      r0, r6
0052aca8: ldr      r7, [r5, #0x14]
0052acac: bl       #0x52aa84
0052acb0: str      r7, [r0, #8]
0052acb4: ldr      r3, [r6, #4]
0052acb8: str      r6, [r0]
0052acbc: str      r3, [r0, #4]
0052acc0: str      r0, [r3]
0052acc4: str      r0, [r6, #4]
0052acc8: ldr      r2, [r5, #0xc]
0052accc: cmp      r2, #0
0052acd0: beq      #0x52ad0c
0052acd4: mov      r5, r2
0052acd8: b        #0x52ace0
0052acdc: mov      r5, r3
0052ace0: ldr      r3, [r5, #8]
0052ace4: cmp      r3, #0
0052ace8: bne      #0x52acdc
0052acec: ldr      r3, [r4, #0x14]
0052acf0: add      r2, r3, #0x2c
0052acf4: cmp      r2, r5
0052acf8: bne      #0x52aca4
0052acfc: ldr      r0, [r3, #0x3c]
0052ad00: pop      {r4, r5, r6, r7, r8, pc}
0052ad04: mov      r4, r3
0052ad08: b        #0x52ac88
0052ad0c: ldr      r3, [r5, #4]
0052ad10: ldr      r1, [r3, #0xc]
0052ad14: cmp      r5, r1
0052ad18: bne      #0x52ad34
0052ad1c: mov      r5, r3
0052ad20: ldr      r3, [r3, #4]
0052ad24: ldr      r2, [r3, #0xc]
0052ad28: cmp      r2, r5
0052ad2c: beq      #0x52ad1c
0052ad30: ldr      r2, [r5, #0xc]
0052ad34: cmp      r3, r2
0052ad38: movne    r5, r3
0052ad3c: ldr      r3, [r4, #0x14]
0052ad40: b        #0x52acf0
0052ad44: mov      r0, #0
0052ad48: pop      {r4, r5, r6, r7, r8, pc}

# _ZN7PFFloor10_GetNodeAtERK7Point3DIfE
0051c0dc: push     {r4, lr}
0051c0e0: add      r4, r0, #0x90
0051c0e4: mov      r0, r4
0051c0e8: bl       #0x51bf28
0051c0ec: cmp      r0, r4
0051c0f0: moveq    r0, #0
0051c0f4: ldrne    r0, [r0, #0x1c]
0051c0f8: pop      {r4, pc}

# _ZSt10__pop_heapIPN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeES7_NS6_6_ECompEiEvT_SA_SA_T0_T1_PT2_
0052943c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00529440: rsb      r1, r0, r1
00529444: mov      r4, r0
00529448: ldr      ip, [r0], #4
0052944c: mov      lr, r2
00529450: asr      r1, r1, #2
00529454: str      ip, [lr], #4
00529458: ldr      ip, [r4, #4]
0052945c: add      fp, r1, r1, lsl #2
00529460: sub      sp, sp, #0x24
00529464: str      ip, [r2, #4]
00529468: ldr      r2, [r0, #4]
0052946c: add      fp, fp, fp, lsl #4
00529470: str      r2, [lr, #4]
00529474: ldr      r2, [r3, #8]
00529478: add      fp, fp, fp, lsl #8
0052947c: str      r2, [sp, #0xc]
00529480: ldr      ip, [r3]
00529484: add      fp, fp, fp, lsl #16
00529488: str      ip, [sp, #4]
0052948c: ldr      r3, [r3, #4]
00529490: add      fp, r1, fp, lsl #1
00529494: cmp      fp, #2
00529498: str      r3, [sp, #8]
0052949c: movle    r5, #0
005294a0: movle    r2, #2
005294a4: ble      #0x529518
005294a8: mov      sb, #0
005294ac: mov      r5, #2
005294b0: mov      r7, #0xc
005294b4: b        #0x5294bc
005294b8: mov      r5, r2
005294bc: sub      r8, r5, #1
005294c0: mla      r6, r7, r5, r4
005294c4: mla      sl, r7, r8, r4
005294c8: ldr      r0, [r6, #8]
005294cc: ldr      r1, [sl, #8]
005294d0: bl       #0x30e2f8
005294d4: cmp      r0, #0
005294d8: movne    r6, sl
005294dc: mov      r3, r6
005294e0: ldr      r2, [r3], #4
005294e4: mul      sb, r7, sb
005294e8: movne    r5, r8
005294ec: str      r2, [r4, sb]
005294f0: ldr      r0, [r6, #4]
005294f4: add      r1, r4, sb
005294f8: add      r2, r5, #1
005294fc: str      r0, [r1, #4]
00529500: ldr      r3, [r3, #4]
00529504: lsl      r2, r2, #1
00529508: cmp      fp, r2
0052950c: mov      sb, r5
00529510: str      r3, [r1, #8]
00529514: bgt      #0x5294b8
00529518: cmp      fp, r2
0052951c: beq      #0x529554
00529520: ldr      ip, [sp, #4]
00529524: mov      r0, r4
00529528: mov      r1, r5
0052952c: str      ip, [sp, #0x14]
00529530: ldr      ip, [sp, #8]
00529534: add      r2, sp, #0x14
00529538: mov      r3, #0
0052953c: str      ip, [sp, #0x18]
00529540: ldr      ip, [sp, #0xc]
00529544: str      ip, [sp, #0x1c]
00529548: bl       #0x529398
0052954c: add      sp, sp, #0x24
00529550: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00529554: mov      r3, #0xc
00529558: sub      fp, fp, #1
0052955c: mul      r2, r3, fp
00529560: mul      r5, r3, r5
00529564: ldr      r1, [r4, r2]
00529568: add      r2, r4, r2
0052956c: add      r3, r4, r5
00529570: str      r1, [r4, r5]
00529574: ldr      r1, [r2, #4]
00529578: mov      r5, fp
0052957c: str      r1, [r3, #4]
00529580: ldr      r2, [r2, #8]
00529584: str      r2, [r3, #8]
00529588: b        #0x529520

# _ZN7PFWorld25_ChangeObstacleParentListERK8PFObjectP7PFFloor
00528484: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00528488: ldr      r3, [r1, #4]
0052848c: sub      sp, sp, #0x7c
00528490: mov      r5, r1
00528494: tst      r3, #4
00528498: str      r2, [sp, #0x14]
0052849c: beq      #0x52859c
005284a0: ldr      r3, [r1, #0x10]
005284a4: cmp      r3, r2
005284a8: beq      #0x52859c
005284ac: add      r6, r0, #0x2c
005284b0: add      r1, r1, #0x10
005284b4: mov      r0, r6
005284b8: bl       #0x527560
005284bc: ldr      r3, [r0]
005284c0: ldr      r7, [r0, #8]
005284c4: ldr      lr, [r0, #4]
005284c8: mov      r4, r0
005284cc: str      r3, [sp, #0xc]
005284d0: ldr      ip, [r4, #0xc]
005284d4: ldr      r8, [r0, #0x1c]
005284d8: ldr      sl, [r0, #0x18]
005284dc: ldr      sb, [r0, #0x14]
005284e0: ldr      fp, [r0, #0x10]
005284e4: str      ip, [sp, #0x44]
005284e8: ldr      ip, [sp, #0xc]
005284ec: add      r3, sp, #0x6c
005284f0: add      r0, sp, #0x58
005284f4: str      ip, [sp, #0x38]
005284f8: add      r1, sp, #0x38
005284fc: add      ip, sp, #0x74
00528500: add      r2, sp, #0x48
00528504: str      ip, [sp]
00528508: str      r7, [sp, #0x40]
0052850c: str      lr, [sp, #0x3c]
00528510: str      r8, [sp, #0x54]
00528514: str      sl, [sp, #0x50]
00528518: str      sb, [sp, #0x4c]
0052851c: str      fp, [sp, #0x48]
00528520: str      r5, [sp, #0x6c]
00528524: bl       #0x5262c4
00528528: ldr      ip, [sp, #0x58]
0052852c: ldr      r3, [r4, #0x10]
00528530: cmp      r3, ip
00528534: beq      #0x52859c
00528538: ldr      lr, [sp, #0x64]
0052853c: add      r2, sp, #0x18
00528540: mov      r1, r4
00528544: str      lr, [sp, #0x24]
00528548: ldr      lr, [sp, #0x60]
0052854c: add      r3, sp, #0x70
00528550: add      r0, sp, #0x28
00528554: str      lr, [sp, #0x20]
00528558: ldr      lr, [sp, #0x5c]
0052855c: str      ip, [sp, #0x18]
00528560: str      lr, [sp, #0x1c]
00528564: bl       #0x52689c
00528568: add      r1, sp, #0x14
0052856c: mov      r0, r6
00528570: bl       #0x527560
00528574: str      r5, [sp, #0x68]
00528578: ldr      r1, [r0, #0x18]
0052857c: ldr      r2, [r0, #0x10]
00528580: sub      r1, r1, #4
00528584: cmp      r2, r1
00528588: beq      #0x5285a4
0052858c: str      r5, [r2]
00528590: ldr      r2, [r0, #0x10]
00528594: add      r2, r2, #4
00528598: str      r2, [r0, #0x10]
0052859c: add      sp, sp, #0x7c
005285a0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005285a4: add      r1, sp, #0x68
005285a8: bl       #0x5280b0
005285ac: b        #0x52859c

# _ZN7PFFloor14GetCollisionAtERK7Point3DIfERS1_RN6glitch4core10triangle3dIfEE
0051b96c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0051b970: ldr      r5, [r1]
0051b974: sub      sp, sp, #0x30
0051b978: mov      r6, r1
0051b97c: mov      r4, r0
0051b980: mov      r1, r5
0051b984: ldr      r0, [r0, #0x44]
0051b988: mov      r7, r2
0051b98c: mov      sl, r3
0051b990: bl       #0x30e9ac
0051b994: ldr      r8, [pc, #0x138]
0051b998: cmp      r0, #0
0051b99c: add      r8, pc, r8
0051b9a0: beq      #0x51b9b8
0051b9a4: mov      r0, r5
0051b9a8: ldr      r1, [r4, #0x50]
0051b9ac: bl       #0x30e9ac
0051b9b0: cmp      r0, #0
0051b9b4: bne      #0x51b9c4
0051b9b8: mov      r0, #0
0051b9bc: add      sp, sp, #0x30
0051b9c0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0051b9c4: ldr      sb, [r6, #4]
0051b9c8: ldr      r0, [r4, #0x48]
0051b9cc: mov      r1, sb
0051b9d0: bl       #0x30e9ac
0051b9d4: cmp      r0, #0
0051b9d8: beq      #0x51b9b8
0051b9dc: mov      r0, sb
0051b9e0: ldr      r1, [r4, #0x54]
0051b9e4: bl       #0x30e9ac
0051b9e8: cmp      r0, #0
0051b9ec: beq      #0x51b9b8
0051b9f0: ldr      r6, [r6, #8]
0051b9f4: ldr      r0, [r4, #0x4c]
0051b9f8: mov      r1, r6
0051b9fc: bl       #0x30e9ac
0051ba00: cmp      r0, #0
0051ba04: beq      #0x51b9b8
0051ba08: mov      r0, r6
0051ba0c: ldr      r1, [r4, #0x58]
0051ba10: bl       #0x30e9ac
0051ba14: cmp      r0, #0
0051ba18: beq      #0x51b9b8
0051ba1c: ldr      r2, [pc, #0xb4]
0051ba20: mov      r1, #0x44000000
0051ba24: mov      r3, #0
0051ba28: ldr      r2, [r8, r2]
0051ba2c: add      r1, r1, #0x7a0000
0051ba30: mov      r0, r6
0051ba34: ldr      r2, [r2, #0x10]
0051ba38: ldr      r8, [r2, #0x1c]
0051ba3c: str      r3, [sp, #0x2c]
0051ba40: str      r3, [sp, #0x24]
0051ba44: str      r3, [sp, #0x28]
0051ba48: str      r5, [sp, #0x18]
0051ba4c: str      r5, [sp, #0xc]
0051ba50: str      sb, [sp, #0x1c]
0051ba54: str      sb, [sp, #0x10]
0051ba58: bl       #0x30eba4
0051ba5c: mov      r1, #0x44000000
0051ba60: add      r1, r1, #0x7a0000
0051ba64: str      r0, [sp, #0x14]
0051ba68: mov      r0, r6
0051ba6c: bl       #0x30e3ac
0051ba70: str      r0, [sp, #0x20]
0051ba74: ldr      r5, [r8, #0x2c]
0051ba78: ldr      r3, [r4, #0x40]
0051ba7c: ldr      r2, [r5]
0051ba80: mov      r0, r3
0051ba84: ldr      r3, [r3]
0051ba88: ldr      r4, [r2, #0xc]
0051ba8c: mov      lr, pc
0051ba90: ldr      pc, [r3, #0xb0]
0051ba94: str      sl, [sp]
0051ba98: mov      r2, r0
0051ba9c: add      r1, sp, #0xc
0051baa0: mov      r0, r5
0051baa4: add      r3, sp, #0x24
0051baa8: blx      r4
0051baac: cmp      r0, #0
0051bab0: beq      #0x51b9b8
0051bab4: ldr      r2, [sp, #0x28]
0051bab8: ldr      r3, [sp, #0x2c]
0051babc: ldr      r1, [sp, #0x24]
0051bac0: mov      r0, #1
0051bac4: str      r2, [r7, #4]
0051bac8: str      r1, [r7]
0051bacc: str      r3, [r7, #8]
0051bad0: b        #0x51b9bc
0051bad4: strdeq   sb, sl, [r7], #-4
0051bad8: strdeq   r3, r4, [r0], -r4

# _ZNK8PFObject9CanPathOnEP7PFFloor
00524230: cmp      r1, #0
00524234: moveq    r0, r1
00524238: bxeq     lr
0052423c: ldr      r3, [r1, #0x24]
00524240: ldr      r0, [r0, #0x14]
00524244: cmp      r3, #0
00524248: moveq    r0, #1
0052424c: bxeq     lr
00524250: and      r0, r3, r0
00524254: cmp      r3, r0
00524258: movne    r0, #0
0052425c: moveq    r0, #1
00524260: bx       lr

# _ZN7PFWorld16GetFloorHeightAtERK7Point3DIfEPfPS1_PP6PFRoomPP7PFFloorb
00525508: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0052550c: ldr      r4, [r1]
00525510: sub      sp, sp, #0xc
00525514: mov      r6, r1
00525518: mov      r5, r0
0052551c: mov      r1, r4
00525520: ldr      r0, [r0, #0x14]
00525524: mov      sl, r2
00525528: mov      r8, r3
0052552c: bl       #0x30e9ac
00525530: cmp      r0, #0
00525534: ldr      fp, [sp, #0x30]
00525538: ldr      sb, [sp, #0x34]
0052553c: ldrb     r7, [sp, #0x38]
00525540: beq      #0x525558
00525544: mov      r0, r4
00525548: ldr      r1, [r5, #0x20]
0052554c: bl       #0x30e9ac
00525550: cmp      r0, #0
00525554: bne      #0x525564
00525558: mov      r0, #0
0052555c: add      sp, sp, #0xc
00525560: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00525564: ldr      r4, [r6, #4]
00525568: ldr      r0, [r5, #0x18]
0052556c: mov      r1, r4
00525570: bl       #0x30e9ac
00525574: cmp      r0, #0
00525578: beq      #0x525558
0052557c: mov      r0, r4
00525580: ldr      r1, [r5, #0x24]
00525584: bl       #0x30e9ac
00525588: cmp      r0, #0
0052558c: beq      #0x525558
00525590: ldr      r4, [r6, #8]
00525594: ldr      r0, [r5, #0x1c]
00525598: mov      r1, r4
0052559c: bl       #0x30e9ac
005255a0: cmp      r0, #0
005255a4: beq      #0x525558
005255a8: mov      r0, r4
005255ac: ldr      r1, [r5, #0x28]
005255b0: bl       #0x30e9ac
005255b4: cmp      r0, #0
005255b8: beq      #0x525558
005255bc: ldr      r3, [r5, #8]
005255c0: ldr      r2, [r5, #0xc]
005255c4: rsb      r2, r3, r2
005255c8: lsrs     r2, r2, #2
005255cc: beq      #0x525558
005255d0: mov      r4, #0
005255d4: b        #0x5255ec
005255d8: ldr      r3, [r5, #8]
005255dc: ldr      r2, [r5, #0xc]
005255e0: rsb      r2, r3, r2
005255e4: cmp      r4, r2, asr #2
005255e8: bhs      #0x525558
005255ec: ldr      r0, [r3, r4, lsl #2]
005255f0: mov      r1, r6
005255f4: mov      r3, r8
005255f8: mov      r2, sl
005255fc: str      sb, [sp]
00525600: str      r7, [sp, #4]
00525604: bl       #0x520f98
00525608: cmp      r0, #0
0052560c: lsl      r3, r4, #2
00525610: add      r4, r4, #1
00525614: beq      #0x5255d8
00525618: cmp      fp, #0
0052561c: ldrne    r2, [r5, #8]
00525620: moveq    r0, #1
00525624: movne    r0, #1
00525628: ldrne    r3, [r2, r3]
0052562c: strne    r3, [fp]
00525630: b        #0x52555c

# _ZN7PFWorld16ValidatePositionER7Point3DIfEP8PFObject
00525d84: push     {r4, r5, r6, r7, r8, sl, lr}
00525d88: subs     r4, r2, #0
00525d8c: sub      sp, sp, #0x2c
00525d90: mov      r8, r0
00525d94: mov      r5, r1
00525d98: beq      #0x525db0
00525d9c: add      r0, r4, #0x18
00525da0: bl       #0x312b6c
00525da4: cmp      r0, #0
00525da8: movne    r0, #1
00525dac: bne      #0x525ed4
00525db0: mov      r3, #0
00525db4: cmp      r4, #0
00525db8: str      r3, [sp, #0x18]
00525dbc: str      r3, [sp, #0x10]
00525dc0: str      r3, [sp, #0x14]
00525dc4: beq      #0x525f38
00525dc8: ldr      r3, [r4, #0xc]
00525dcc: str      r3, [sp, #0x20]
00525dd0: ldr      r0, [r4, #0x10]
00525dd4: cmp      r0, #0
00525dd8: str      r0, [sp, #0x1c]
00525ddc: beq      #0x525edc
00525de0: add      r7, sp, #0x24
00525de4: add      r6, sp, #0x10
00525de8: mov      r1, r5
00525dec: mov      r2, r7
00525df0: mov      r3, r6
00525df4: bl       #0x51badc
00525df8: cmp      r0, #0
00525dfc: bne      #0x525e38
00525e00: ldr      r3, [sp, #0x20]
00525e04: cmp      r3, #0
00525e08: addeq    sl, sp, #0x1c
00525e0c: beq      #0x525f08
00525e10: mov      r0, r3
00525e14: mov      ip, #0
00525e18: add      sl, sp, #0x1c
00525e1c: mov      r1, r5
00525e20: mov      r2, r7
00525e24: mov      r3, r6
00525e28: stm      sp, {sl, ip}
00525e2c: bl       #0x520f98
00525e30: cmp      r0, #0
00525e34: beq      #0x525f08
00525e38: mov      r0, r4
00525e3c: ldr      r1, [sp, #0x1c]
00525e40: bl       #0x524230
00525e44: cmp      r0, #0
00525e48: beq      #0x525ee8
00525e4c: ldrb     r3, [r8, #0x94]
00525e50: cmp      r3, #0
00525e54: bne      #0x525e78
00525e58: ldr      r1, [r5, #8]
00525e5c: ldr      r0, [sp, #0x24]
00525e60: bl       #0x30e3ac
00525e64: bic      r1, r0, #0x80000000
00525e68: ldr      r0, [r8, #0x90]
00525e6c: bl       #0x30e2f8
00525e70: cmp      r0, #0
00525e74: beq      #0x525ee8
00525e78: mov      r0, r8
00525e7c: ldr      r2, [sp, #0x1c]
00525e80: mov      r1, r4
00525e84: bl       #0x528484
00525e88: ldr      r3, [r5]
00525e8c: ldr      r2, [sp, #0x24]
00525e90: mov      r0, #1
00525e94: str      r2, [r5, #8]
00525e98: str      r3, [r4, #0x18]
00525e9c: ldr      r3, [r5, #4]
00525ea0: str      r3, [r4, #0x1c]
00525ea4: ldr      r3, [r5, #8]
00525ea8: str      r3, [r4, #0x20]
00525eac: ldr      r3, [sp, #0x10]
00525eb0: str      r3, [r4, #0x24]
00525eb4: ldr      r3, [sp, #0x14]
00525eb8: str      r3, [r4, #0x28]
00525ebc: ldr      r3, [sp, #0x18]
00525ec0: str      r3, [r4, #0x2c]
00525ec4: ldr      r3, [sp, #0x20]
00525ec8: str      r3, [r4, #0xc]
00525ecc: ldr      r3, [sp, #0x1c]
00525ed0: str      r3, [r4, #0x10]
00525ed4: add      sp, sp, #0x2c
00525ed8: pop      {r4, r5, r6, r7, r8, sl, pc}
00525edc: add      r7, sp, #0x24
00525ee0: add      r6, sp, #0x10
00525ee4: b        #0x525e04
00525ee8: mov      r0, #1
00525eec: ldr      r3, [r4, #0x18]
00525ef0: str      r3, [r5]
00525ef4: ldr      r3, [r4, #0x1c]
00525ef8: str      r3, [r5, #4]
00525efc: ldr      r3, [r4, #0x20]
00525f00: str      r3, [r5, #8]
00525f04: b        #0x525ed4
00525f08: add      ip, sp, #0x20
00525f0c: str      ip, [sp]
00525f10: mov      r2, r7
00525f14: mov      ip, #0
00525f18: mov      r3, r6
00525f1c: mov      r0, r8
00525f20: mov      r1, r5
00525f24: stmib    sp, {sl, ip}
00525f28: bl       #0x525508
00525f2c: cmp      r0, #0
00525f30: beq      #0x525eec
00525f34: b        #0x525e38
00525f38: mov      r3, r4
00525f3c: mov      r0, r8
00525f40: mov      r1, r5
00525f44: add      r2, sp, #0x24
00525f48: str      r4, [sp]
00525f4c: str      r4, [sp, #4]
00525f50: str      r4, [sp, #8]
00525f54: bl       #0x525508
00525f58: cmp      r0, #0
00525f5c: ldrne    r3, [sp, #0x24]
00525f60: strne    r3, [r5, #8]
00525f64: b        #0x525ed4

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

# _ZN6PFRoom16GetFloorHeightAtERK7Point3DIfEPfPS1_PP7PFFloorb
00520f98: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00520f9c: ldr      sl, [r1]
00520fa0: mov      r6, r1
00520fa4: mov      r5, r0
00520fa8: mov      r1, sl
00520fac: ldr      r0, [r0, #0x3c]
00520fb0: mov      r8, r2
00520fb4: mov      r7, r3
00520fb8: bl       #0x30e9ac
00520fbc: cmp      r0, #0
00520fc0: ldr      sb, [sp, #0x20]
00520fc4: ldrb     r4, [sp, #0x24]
00520fc8: beq      #0x5210b0
00520fcc: mov      r0, sl
00520fd0: ldr      r1, [r5, #0x48]
00520fd4: bl       #0x30e9ac
00520fd8: cmp      r0, #0
00520fdc: beq      #0x5210b0
00520fe0: ldr      sl, [r6, #4]
00520fe4: ldr      r0, [r5, #0x40]
00520fe8: mov      r1, sl
00520fec: bl       #0x30e9ac
00520ff0: cmp      r0, #0
00520ff4: beq      #0x5210b0
00520ff8: mov      r0, sl
00520ffc: ldr      r1, [r5, #0x4c]
00521000: bl       #0x30e9ac
00521004: cmp      r0, #0
00521008: beq      #0x5210b0
0052100c: ldr      sl, [r6, #8]
00521010: ldr      r0, [r5, #0x44]
00521014: mov      r1, sl
00521018: bl       #0x30e9ac
0052101c: cmp      r0, #0
00521020: beq      #0x5210b0
00521024: mov      r0, sl
00521028: ldr      r1, [r5, #0x50]
0052102c: bl       #0x30e9ac
00521030: cmp      r0, #0
00521034: beq      #0x5210b0
00521038: cmp      r4, #0
0052103c: beq      #0x5210b8
00521040: ldr      r3, [r5, #0x30]
00521044: ldr      r2, [r5, #0x34]
00521048: rsb      r2, r3, r2
0052104c: lsrs     r2, r2, #2
00521050: movne    r4, #0
00521054: bne      #0x521070
00521058: b        #0x5210b0
0052105c: ldr      r3, [r5, #0x30]
00521060: ldr      r2, [r5, #0x34]
00521064: rsb      r2, r3, r2
00521068: cmp      r4, r2, asr #2
0052106c: bhs      #0x5210b0
00521070: ldr      r0, [r3, r4, lsl #2]
00521074: mov      r1, r6
00521078: mov      r3, r7
0052107c: mov      r2, r8
00521080: bl       #0x51badc
00521084: cmp      r0, #0
00521088: lsl      r3, r4, #2
0052108c: add      r4, r4, #1
00521090: beq      #0x52105c
00521094: cmp      sb, #0
00521098: beq      #0x521134
0052109c: ldr      r2, [r5, #0x30]
005210a0: mov      r0, #1
005210a4: ldr      r3, [r2, r3]
005210a8: str      r3, [sb]
005210ac: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
005210b0: mov      r0, #0
005210b4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
005210b8: ldr      r3, [r5, #0x30]
005210bc: ldr      r1, [r5, #0x34]
005210c0: rsb      r2, r3, r1
005210c4: lsrs     r2, r2, #2
005210c8: bne      #0x5210e0
005210cc: b        #0x5210b0
005210d0: add      r4, r4, #1
005210d4: rsb      r2, r3, r1
005210d8: cmp      r4, r2, asr #2
005210dc: bhs      #0x5210b0
005210e0: ldr      r0, [r3, r4, lsl #2]
005210e4: lsl      sl, r4, #2
005210e8: ldr      r2, [r0, #0x24]
005210ec: tst      r2, #0x3000000
005210f0: bne      #0x5210d0
005210f4: mov      r1, r6
005210f8: mov      r2, r8
005210fc: mov      r3, r7
00521100: bl       #0x51badc
00521104: cmp      r0, #0
00521108: bne      #0x521118
0052110c: ldr      r3, [r5, #0x30]
00521110: ldr      r1, [r5, #0x34]
00521114: b        #0x5210d0
00521118: cmp      sb, #0
0052111c: beq      #0x521134
00521120: ldr      r3, [r5, #0x30]
00521124: mov      r0, #1
00521128: ldr      r3, [r3, sl]
0052112c: str      r3, [sb]
00521130: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00521134: mov      r0, #1
00521138: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNSt4priv8_Rb_treeIjSt4lessIjESt4pairIKjN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeEENS_10_Select1stISD_EENS_11_MapTraitsTISD_EESaISD_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
0052a06c: push     {r4, r5, r6, lr}
0052a070: subs     r4, r1, #0
0052a074: mov      r6, r0
0052a078: beq      #0x52a0a0
0052a07c: ldr      r1, [r4, #0xc]
0052a080: mov      r0, r6
0052a084: bl       #0x52a06c
0052a088: ldr      r5, [r4, #8]
0052a08c: mov      r0, r4
0052a090: mov      r1, #0x20
0052a094: bl       #0x708f00
0052a098: subs     r4, r5, #0
0052a09c: bne      #0x52a07c
0052a0a0: pop      {r4, r5, r6, pc}

# _ZNK3sfc4math5graph4EdgeI12PFGInnerNodefE11getFromNodeEv
0051bc48: ldr      r0, [r0, #4]
0051bc4c: bx       lr

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

# _ZNK24PFInnerTest_PathValidity7isValidEPK12PFGInnerEdge
0052503c: push     {r4, lr}
00525040: mov      r0, r1
00525044: ldr      r3, [r1]
00525048: mov      lr, pc
0052504c: ldr      pc, [r3, #0x18]
00525050: pop      {r4, pc}

# _ZN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicED1Ev
00529f8c: ldr      r3, [pc, #0x24]
00529f90: ldr      r2, [pc, #0x24]
00529f94: push     {r4, lr}
00529f98: add      r3, pc, r3
00529f9c: ldr      r2, [r3, r2]
00529fa0: mov      r4, r0
00529fa4: add      r2, r2, #8
00529fa8: str      r2, [r0], #8
00529fac: bl       #0x529f4c
00529fb0: mov      r0, r4
00529fb4: pop      {r4, pc}
00529fb8: strdeq   sl, fp, [r6], #-0xa8
00529fbc: andeq    r1, r0, ip, lsl r0

# _ZN7PFWorld12_SearchGraphEPK8PFObjectRK7Point3DIfES6_jPSt4listIPK12PFGInnerEdgeSaISA_EE
0052b560: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0052b564: ldr      r4, [pc, #0xdc4]
0052b568: ldr      r5, [pc, #0xdc4]
0052b56c: ldr      lr, [pc, #0xdc4]
0052b570: add      r4, pc, r4
0052b574: ldr      ip, [r4, r5]
0052b578: ldr      r6, [r4, lr]
0052b57c: sub      sp, sp, #0x164
0052b580: ldr      ip, [ip]
0052b584: mov      r8, r0
0052b588: mov      r0, r6
0052b58c: mov      sb, r3
0052b590: str      ip, [sp, #0x15c]
0052b594: str      r1, [sp, #0x20]
0052b598: mov      r7, r2
0052b59c: ldr      fp, [sp, #0x18c]
0052b5a0: bl       #0x337888
0052b5a4: ldr      r1, [pc, #0xd90]
0052b5a8: add      sl, sp, #0x144
0052b5ac: add      r2, sp, #0x140
0052b5b0: add      r1, pc, r1
0052b5b4: mov      r0, sl
0052b5b8: bl       #0x3140ec
0052b5bc: mov      r0, r6
0052b5c0: mov      r1, sl
0052b5c4: bl       #0x337ec8
0052b5c8: mov      r6, r0
0052b5cc: ldr      r0, [sp, #0x158]
0052b5d0: cmp      r0, sl
0052b5d4: beq      #0x52b5f4
0052b5d8: cmp      r0, #0
0052b5dc: beq      #0x52b5f4
0052b5e0: ldr      r1, [sp, #0x144]
0052b5e4: rsb      r1, r0, r1
0052b5e8: cmp      r1, #0x80
0052b5ec: bhi      #0x52bee8
0052b5f0: bl       #0x708f00
0052b5f4: cmp      r6, #0
0052b5f8: beq      #0x52b690
0052b5fc: mov      ip, #0
0052b600: mov      r6, #0
0052b604: add      lr, sp, #0x13c
0052b608: mov      r0, r8
0052b60c: mov      r1, r7
0052b610: add      r2, sp, #0x128
0052b614: add      r3, sp, #0x88
0052b618: str      ip, [sp, #0x124]
0052b61c: str      lr, [sp, #4]
0052b620: str      ip, [sp, #0x88]
0052b624: str      ip, [sp, #0x8c]
0052b628: str      ip, [sp, #0x90]
0052b62c: str      ip, [sp, #0x94]
0052b630: str      ip, [sp, #0x98]
0052b634: str      ip, [sp, #0x9c]
0052b638: str      ip, [sp, #0xa0]
0052b63c: str      ip, [sp, #0xa4]
0052b640: str      ip, [sp, #0xa8]
0052b644: str      ip, [sp, #0x64]
0052b648: str      ip, [sp, #0x68]
0052b64c: str      ip, [sp, #0x6c]
0052b650: str      ip, [sp, #0x70]
0052b654: str      ip, [sp, #0x74]
0052b658: str      ip, [sp, #0x78]
0052b65c: str      ip, [sp, #0x7c]
0052b660: str      ip, [sp, #0x80]
0052b664: str      ip, [sp, #0x84]
0052b668: str      ip, [sp, #0x128]
0052b66c: str      ip, [sp, #0x12c]
0052b670: str      ip, [sp, #0x130]
0052b674: str      ip, [sp, #0x11c]
0052b678: str      ip, [sp, #0x120]
0052b67c: str      r6, [sp]
0052b680: str      r6, [sp, #8]
0052b684: bl       #0x5256d4
0052b688: cmp      r0, r6
0052b68c: bne      #0x52b6b4
0052b690: mov      r6, #0
0052b694: ldr      r3, [r4, r5]
0052b698: ldr      r2, [sp, #0x15c]
0052b69c: mov      r0, r6
0052b6a0: ldr      r3, [r3]
0052b6a4: cmp      r2, r3
0052b6a8: bne      #0x52c32c
0052b6ac: add      sp, sp, #0x164
0052b6b0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0052b6b4: ldr      r3, [sp, #0x13c]
0052b6b8: cmp      r3, r6
0052b6bc: beq      #0x52b690
0052b6c0: add      ip, sp, #0x138
0052b6c4: mov      r0, r8
0052b6c8: mov      r1, sb
0052b6cc: add      r2, sp, #0x11c
0052b6d0: add      r3, sp, #0x64
0052b6d4: str      ip, [sp, #4]
0052b6d8: str      r6, [sp, #8]
0052b6dc: str      r6, [sp]
0052b6e0: bl       #0x5256d4
0052b6e4: cmp      r0, #0
0052b6e8: beq      #0x52b690
0052b6ec: ldr      r3, [sp, #0x138]
0052b6f0: cmp      r3, #0
0052b6f4: beq      #0x52b690
0052b6f8: ldr      r6, [sp, #0x88]
0052b6fc: ldr      r0, [sp, #0x64]
0052b700: mov      r1, r6
0052b704: bl       #0x30df8c
0052b708: cmp      r0, #0
0052b70c: bne      #0x52bef0
0052b710: ldr      ip, [sp, #0x98]
0052b714: ldr      lr, [sp, #0x90]
0052b718: ldr      r3, [sp, #0x94]
0052b71c: ldr      sl, [sp, #0x8c]
0052b720: str      ip, [sp, #0x24]
0052b724: str      lr, [sp, #0x2c]
0052b728: ldr      r2, [sp, #0x9c]
0052b72c: str      r2, [sp, #0x28]
0052b730: mov      r1, r3
0052b734: mov      r0, r6
0052b738: bl       #0x30eba4
0052b73c: mov      r1, #0x3f000000
0052b740: bl       #0x30ed6c
0052b744: mov      r1, sl
0052b748: str      r0, [sp, #0x110]
0052b74c: ldr      r0, [sp, #0x24]
0052b750: bl       #0x30eba4
0052b754: mov      r1, #0x3f000000
0052b758: bl       #0x30ed6c
0052b75c: ldr      r1, [sp, #0x28]
0052b760: str      r0, [sp, #0x114]
0052b764: ldr      r0, [sp, #0x2c]
0052b768: bl       #0x30eba4
0052b76c: mov      r1, #0x3f000000
0052b770: bl       #0x30ed6c
0052b774: add      r1, sp, #0x110
0052b778: str      r0, [sp, #0x118]
0052b77c: ldr      r0, [sp, #0x13c]
0052b780: bl       #0x51c0dc
0052b784: ldr      r1, [sp, #0xa4]
0052b788: mov      r6, r0
0052b78c: ldr      r0, [sp, #0x8c]
0052b790: bl       #0x30eba4
0052b794: mov      r1, #0x3f000000
0052b798: bl       #0x30ed6c
0052b79c: ldr      r1, [sp, #0xa8]
0052b7a0: mov      sl, r0
0052b7a4: ldr      r0, [sp, #0x90]
0052b7a8: bl       #0x30eba4
0052b7ac: mov      r1, #0x3f000000
0052b7b0: bl       #0x30ed6c
0052b7b4: ldr      r1, [sp, #0xa0]
0052b7b8: mov      r3, r0
0052b7bc: ldr      r0, [sp, #0x88]
0052b7c0: str      r3, [sp, #0x14]
0052b7c4: bl       #0x30eba4
0052b7c8: mov      r1, #0x3f000000
0052b7cc: bl       #0x30ed6c
0052b7d0: ldr      r3, [sp, #0x14]
0052b7d4: str      r0, [sp, #0x104]
0052b7d8: add      r1, sp, #0x104
0052b7dc: ldr      r0, [sp, #0x13c]
0052b7e0: str      r3, [sp, #0x10c]
0052b7e4: str      sl, [sp, #0x108]
0052b7e8: bl       #0x51c0dc
0052b7ec: ldr      r1, [sp, #0xa4]
0052b7f0: str      r0, [sp, #0x30]
0052b7f4: ldr      r0, [sp, #0x98]
0052b7f8: bl       #0x30eba4
0052b7fc: mov      r1, #0x3f000000
0052b800: bl       #0x30ed6c
0052b804: ldr      r1, [sp, #0xa8]
0052b808: mov      sl, r0
0052b80c: ldr      r0, [sp, #0x9c]
0052b810: bl       #0x30eba4
0052b814: mov      r1, #0x3f000000
0052b818: bl       #0x30ed6c
0052b81c: ldr      r1, [sp, #0xa0]
0052b820: mov      r3, r0
0052b824: ldr      r0, [sp, #0x94]
0052b828: str      r3, [sp, #0x14]
0052b82c: bl       #0x30eba4
0052b830: mov      r1, #0x3f000000
0052b834: bl       #0x30ed6c
0052b838: ldr      r3, [sp, #0x14]
0052b83c: str      r0, [sp, #0xf8]
0052b840: add      r1, sp, #0xf8
0052b844: ldr      r0, [sp, #0x13c]
0052b848: str      r3, [sp, #0x100]
0052b84c: str      sl, [sp, #0xfc]
0052b850: bl       #0x51c0dc
0052b854: ldr      r1, [sp, #0x74]
0052b858: mov      sl, r0
0052b85c: ldr      r0, [sp, #0x68]
0052b860: bl       #0x30eba4
0052b864: mov      r1, #0x3f000000
0052b868: bl       #0x30ed6c
0052b86c: ldr      r1, [sp, #0x78]
0052b870: mov      r3, r0
0052b874: ldr      r0, [sp, #0x6c]
0052b878: str      r3, [sp, #0x14]
0052b87c: bl       #0x30eba4
0052b880: mov      r1, #0x3f000000
0052b884: bl       #0x30ed6c
0052b888: ldr      r1, [sp, #0x70]
0052b88c: mov      r2, r0
0052b890: ldr      r0, [sp, #0x64]
0052b894: str      r2, [sp, #0x18]
0052b898: bl       #0x30eba4
0052b89c: mov      r1, #0x3f000000
0052b8a0: bl       #0x30ed6c
0052b8a4: ldr      r2, [sp, #0x18]
0052b8a8: ldr      r3, [sp, #0x14]
0052b8ac: str      r0, [sp, #0xec]
0052b8b0: add      r1, sp, #0xec
0052b8b4: ldr      r0, [sp, #0x138]
0052b8b8: str      r2, [sp, #0xf4]
0052b8bc: str      r3, [sp, #0xf0]
0052b8c0: bl       #0x51c0dc
0052b8c4: ldr      r1, [sp, #0x80]
0052b8c8: str      r0, [sp, #0x2c]
0052b8cc: ldr      r0, [sp, #0x68]
0052b8d0: bl       #0x30eba4
0052b8d4: mov      r1, #0x3f000000
0052b8d8: bl       #0x30ed6c
0052b8dc: ldr      r1, [sp, #0x84]
0052b8e0: mov      r3, r0
0052b8e4: ldr      r0, [sp, #0x6c]
0052b8e8: str      r3, [sp, #0x14]
0052b8ec: bl       #0x30eba4
0052b8f0: mov      r1, #0x3f000000
0052b8f4: bl       #0x30ed6c
0052b8f8: ldr      r1, [sp, #0x7c]
0052b8fc: mov      r2, r0
0052b900: ldr      r0, [sp, #0x64]
0052b904: str      r2, [sp, #0x18]
0052b908: bl       #0x30eba4
0052b90c: mov      r1, #0x3f000000
0052b910: bl       #0x30ed6c
0052b914: ldr      r2, [sp, #0x18]
0052b918: ldr      r3, [sp, #0x14]
0052b91c: str      r0, [sp, #0xe0]
0052b920: add      r1, sp, #0xe0
0052b924: ldr      r0, [sp, #0x138]
0052b928: str      r2, [sp, #0xe8]
0052b92c: str      r3, [sp, #0xe4]
0052b930: bl       #0x51c0dc
0052b934: ldr      r1, [sp, #0x80]
0052b938: str      r0, [sp, #0x28]
0052b93c: ldr      r0, [sp, #0x74]
0052b940: bl       #0x30eba4
0052b944: mov      r1, #0x3f000000
0052b948: bl       #0x30ed6c
0052b94c: ldr      r1, [sp, #0x84]
0052b950: mov      r3, r0
0052b954: ldr      r0, [sp, #0x78]
0052b958: str      r3, [sp, #0x14]
0052b95c: bl       #0x30eba4
0052b960: mov      r1, #0x3f000000
0052b964: bl       #0x30ed6c
0052b968: ldr      r1, [sp, #0x7c]
0052b96c: mov      r2, r0
0052b970: ldr      r0, [sp, #0x70]
0052b974: str      r2, [sp, #0x18]
0052b978: bl       #0x30eba4
0052b97c: mov      r1, #0x3f000000
0052b980: bl       #0x30ed6c
0052b984: ldr      r3, [sp, #0x14]
0052b988: ldr      r2, [sp, #0x18]
0052b98c: str      r0, [sp, #0xd4]
0052b990: add      r1, sp, #0xd4
0052b994: ldr      r0, [sp, #0x138]
0052b998: str      r3, [sp, #0xd8]
0052b99c: str      r2, [sp, #0xdc]
0052b9a0: bl       #0x51c0dc
0052b9a4: cmp      r6, #0
0052b9a8: str      r0, [sp, #0x24]
0052b9ac: beq      #0x52bf5c
0052b9b0: ldr      r1, [r7]
0052b9b4: ldr      r0, [r6, #8]
0052b9b8: bl       #0x30e3ac
0052b9bc: ldr      r1, [r7, #4]
0052b9c0: mov      r3, r0
0052b9c4: ldr      r0, [r6, #0xc]
0052b9c8: str      r3, [sp, #0x14]
0052b9cc: bl       #0x30e3ac
0052b9d0: ldr      r1, [r7, #8]
0052b9d4: mov      r2, r0
0052b9d8: ldr      r0, [r6, #0x10]
0052b9dc: str      r2, [sp, #0x18]
0052b9e0: bl       #0x30e3ac
0052b9e4: ldr      r3, [sp, #0x14]
0052b9e8: mov      ip, r0
0052b9ec: str      ip, [sp, #0x1c]
0052b9f0: mov      r1, r3
0052b9f4: mov      r0, r3
0052b9f8: bl       #0x30ed6c
0052b9fc: ldr      r2, [sp, #0x18]
0052ba00: mov      r3, r0
0052ba04: str      r3, [sp, #0x14]
0052ba08: mov      r1, r2
0052ba0c: mov      r0, r2
0052ba10: bl       #0x30ed6c
0052ba14: ldr      r3, [sp, #0x14]
0052ba18: mov      r1, r0
0052ba1c: mov      r0, r3
0052ba20: bl       #0x30eba4
0052ba24: ldr      ip, [sp, #0x1c]
0052ba28: mov      r3, r0
0052ba2c: str      r3, [sp, #0x14]
0052ba30: mov      r1, ip
0052ba34: mov      r0, ip
0052ba38: bl       #0x30ed6c
0052ba3c: ldr      r3, [sp, #0x14]
0052ba40: mov      r1, r0
0052ba44: mov      r0, r3
0052ba48: bl       #0x30eba4
0052ba4c: mvn      r1, #0x80000000
0052ba50: sub      r1, r1, #0x800000
0052ba54: str      r0, [sp, #0x34]
0052ba58: bl       #0x30e70c
0052ba5c: cmp      r0, #0
0052ba60: beq      #0x52bf5c
0052ba64: ldr      r2, [sp, #0x30]
0052ba68: cmp      r2, #0
0052ba6c: beq      #0x52bb38
0052ba70: ldr      r0, [r2, #8]
0052ba74: ldr      r1, [r7]
0052ba78: bl       #0x30e3ac
0052ba7c: ldr      ip, [sp, #0x30]
0052ba80: ldr      r1, [r7, #4]
0052ba84: mov      r3, r0
0052ba88: ldr      r0, [ip, #0xc]
0052ba8c: str      r3, [sp, #0x14]
0052ba90: bl       #0x30e3ac
0052ba94: ldr      lr, [sp, #0x30]
0052ba98: ldr      r1, [r7, #8]
0052ba9c: mov      r2, r0
0052baa0: ldr      r0, [lr, #0x10]
0052baa4: str      r2, [sp, #0x18]
0052baa8: bl       #0x30e3ac
0052baac: ldr      r3, [sp, #0x14]
0052bab0: mov      ip, r0
0052bab4: str      ip, [sp, #0x1c]
0052bab8: mov      r1, r3
0052babc: mov      r0, r3
0052bac0: bl       #0x30ed6c
0052bac4: ldr      r2, [sp, #0x18]
0052bac8: mov      r3, r0
0052bacc: str      r3, [sp, #0x14]
0052bad0: mov      r1, r2
0052bad4: mov      r0, r2
0052bad8: bl       #0x30ed6c
0052badc: ldr      r3, [sp, #0x14]
0052bae0: mov      r1, r0
0052bae4: mov      r0, r3
0052bae8: bl       #0x30eba4
0052baec: ldr      ip, [sp, #0x1c]
0052baf0: mov      r3, r0
0052baf4: str      r3, [sp, #0x14]
0052baf8: mov      r1, ip
0052bafc: mov      r0, ip
0052bb00: bl       #0x30ed6c
0052bb04: ldr      r3, [sp, #0x14]
0052bb08: mov      r1, r0
0052bb0c: mov      r0, r3
0052bb10: bl       #0x30eba4
0052bb14: mov      r3, r0
0052bb18: mov      r1, r3
0052bb1c: ldr      r0, [sp, #0x34]
0052bb20: str      r3, [sp, #0x14]
0052bb24: bl       #0x30e2f8
0052bb28: ldr      r3, [sp, #0x14]
0052bb2c: cmp      r0, #0
0052bb30: ldrne    r6, [sp, #0x30]
0052bb34: strne    r3, [sp, #0x34]
0052bb38: cmp      sl, #0
0052bb3c: beq      #0x52bbf0
0052bb40: ldr      r1, [r7]
0052bb44: ldr      r0, [sl, #8]
0052bb48: bl       #0x30e3ac
0052bb4c: ldr      r1, [r7, #4]
0052bb50: mov      r3, r0
0052bb54: ldr      r0, [sl, #0xc]
0052bb58: str      r3, [sp, #0x14]
0052bb5c: bl       #0x30e3ac
0052bb60: ldr      r1, [r7, #8]
0052bb64: mov      r2, r0
0052bb68: ldr      r0, [sl, #0x10]
0052bb6c: str      r2, [sp, #0x18]
0052bb70: bl       #0x30e3ac
0052bb74: ldr      r3, [sp, #0x14]
0052bb78: mov      ip, r0
0052bb7c: str      ip, [sp, #0x1c]
0052bb80: mov      r1, r3
0052bb84: mov      r0, r3
0052bb88: bl       #0x30ed6c
0052bb8c: ldr      r2, [sp, #0x18]
0052bb90: mov      r3, r0
0052bb94: str      r3, [sp, #0x14]
0052bb98: mov      r1, r2
0052bb9c: mov      r0, r2
0052bba0: bl       #0x30ed6c
0052bba4: ldr      r3, [sp, #0x14]
0052bba8: mov      r1, r0
0052bbac: mov      r0, r3
0052bbb0: bl       #0x30eba4
0052bbb4: ldr      ip, [sp, #0x1c]
0052bbb8: mov      r3, r0
0052bbbc: str      r3, [sp, #0x14]
0052bbc0: mov      r1, ip
0052bbc4: mov      r0, ip
0052bbc8: bl       #0x30ed6c
0052bbcc: ldr      r3, [sp, #0x14]
0052bbd0: mov      r1, r0
0052bbd4: mov      r0, r3
0052bbd8: bl       #0x30eba4
0052bbdc: mov      r1, r0
0052bbe0: ldr      r0, [sp, #0x34]
0052bbe4: bl       #0x30e2f8
0052bbe8: cmp      r0, #0
0052bbec: movne    r6, sl
0052bbf0: ldr      r2, [sp, #0x2c]
0052bbf4: cmp      r2, #0
0052bbf8: beq      #0x52bca0
0052bbfc: ldr      r0, [r2, #8]
0052bc00: ldr      r1, [sb]
0052bc04: bl       #0x30e3ac
0052bc08: ldr      r3, [sp, #0x2c]
0052bc0c: mov      sl, r0
0052bc10: ldr      r1, [sb, #4]
0052bc14: ldr      r0, [r3, #0xc]
0052bc18: bl       #0x30e3ac
0052bc1c: ldr      ip, [sp, #0x2c]
0052bc20: mov      r3, r0
0052bc24: ldr      r1, [sb, #8]
0052bc28: ldr      r0, [ip, #0x10]
0052bc2c: str      r3, [sp, #0x14]
0052bc30: bl       #0x30e3ac
0052bc34: mov      r1, sl
0052bc38: mov      r2, r0
0052bc3c: mov      r0, sl
0052bc40: str      r2, [sp, #0x18]
0052bc44: bl       #0x30ed6c
0052bc48: ldr      r3, [sp, #0x14]
0052bc4c: mov      sl, r0
0052bc50: mov      r1, r3
0052bc54: mov      r0, r3
0052bc58: bl       #0x30ed6c
0052bc5c: mov      r1, r0
0052bc60: mov      r0, sl
0052bc64: bl       #0x30eba4
0052bc68: ldr      r2, [sp, #0x18]
0052bc6c: mov      sl, r0
0052bc70: mov      r1, r2
0052bc74: mov      r0, r2
0052bc78: bl       #0x30ed6c
0052bc7c: mov      r1, r0
0052bc80: mov      r0, sl
0052bc84: bl       #0x30eba4
0052bc88: mvn      r1, #0x80000000
0052bc8c: sub      r1, r1, #0x800000
0052bc90: mov      sl, r0
0052bc94: bl       #0x30e70c
0052bc98: cmp      r0, #0
0052bc9c: bne      #0x52bf70
0052bca0: mvn      r2, #0x80000000
0052bca4: sub      r2, r2, #0x800000
0052bca8: str      r2, [sp, #0x30]
0052bcac: mov      sl, #0
0052bcb0: ldr      r3, [sp, #0x28]
0052bcb4: cmp      r3, #0
0052bcb8: beq      #0x52bd84
0052bcbc: ldr      r1, [sb]
0052bcc0: ldr      r0, [r3, #8]
0052bcc4: bl       #0x30e3ac
0052bcc8: ldr      ip, [sp, #0x28]
0052bccc: ldr      r1, [sb, #4]
0052bcd0: mov      r3, r0
0052bcd4: ldr      r0, [ip, #0xc]
0052bcd8: str      r3, [sp, #0x14]
0052bcdc: bl       #0x30e3ac
0052bce0: ldr      lr, [sp, #0x28]
0052bce4: ldr      r1, [sb, #8]
0052bce8: mov      r2, r0
0052bcec: ldr      r0, [lr, #0x10]
0052bcf0: str      r2, [sp, #0x18]
0052bcf4: bl       #0x30e3ac
0052bcf8: ldr      r3, [sp, #0x14]
0052bcfc: mov      ip, r0
0052bd00: str      ip, [sp, #0x1c]
0052bd04: mov      r1, r3
0052bd08: mov      r0, r3
0052bd0c: bl       #0x30ed6c
0052bd10: ldr      r2, [sp, #0x18]
0052bd14: mov      r3, r0
0052bd18: str      r3, [sp, #0x14]
0052bd1c: mov      r1, r2
0052bd20: mov      r0, r2
0052bd24: bl       #0x30ed6c
0052bd28: ldr      r3, [sp, #0x14]
0052bd2c: mov      r1, r0
0052bd30: mov      r0, r3
0052bd34: bl       #0x30eba4
0052bd38: ldr      ip, [sp, #0x1c]
0052bd3c: mov      r3, r0
0052bd40: str      r3, [sp, #0x14]
0052bd44: mov      r1, ip
0052bd48: mov      r0, ip
0052bd4c: bl       #0x30ed6c
0052bd50: ldr      r3, [sp, #0x14]
0052bd54: mov      r1, r0
0052bd58: mov      r0, r3
0052bd5c: bl       #0x30eba4
0052bd60: mov      r3, r0
0052bd64: mov      r1, r3
0052bd68: ldr      r0, [sp, #0x30]
0052bd6c: str      r3, [sp, #0x14]
0052bd70: bl       #0x30e2f8
0052bd74: ldr      r3, [sp, #0x14]
0052bd78: cmp      r0, #0
0052bd7c: ldrne    sl, [sp, #0x28]
0052bd80: strne    r3, [sp, #0x30]
0052bd84: ldr      r2, [sp, #0x24]
0052bd88: cmp      r2, #0
0052bd8c: beq      #0x52be4c
0052bd90: ldr      r0, [r2, #8]
0052bd94: ldr      r1, [sb]
0052bd98: bl       #0x30e3ac
0052bd9c: ldr      ip, [sp, #0x24]
0052bda0: ldr      r1, [sb, #4]
0052bda4: mov      r3, r0
0052bda8: ldr      r0, [ip, #0xc]
0052bdac: str      r3, [sp, #0x14]
0052bdb0: bl       #0x30e3ac
0052bdb4: ldr      lr, [sp, #0x24]
0052bdb8: ldr      r1, [sb, #8]
0052bdbc: mov      r2, r0
0052bdc0: ldr      r0, [lr, #0x10]
0052bdc4: str      r2, [sp, #0x18]
0052bdc8: bl       #0x30e3ac
0052bdcc: ldr      r3, [sp, #0x14]
0052bdd0: mov      ip, r0
0052bdd4: str      ip, [sp, #0x1c]
0052bdd8: mov      r1, r3
0052bddc: mov      r0, r3
0052bde0: bl       #0x30ed6c
0052bde4: ldr      r2, [sp, #0x18]
0052bde8: mov      r3, r0
0052bdec: str      r3, [sp, #0x14]
0052bdf0: mov      r1, r2
0052bdf4: mov      r0, r2
0052bdf8: bl       #0x30ed6c
0052bdfc: ldr      r3, [sp, #0x14]
0052be00: mov      r1, r0
0052be04: mov      r0, r3
0052be08: bl       #0x30eba4
0052be0c: ldr      ip, [sp, #0x1c]
0052be10: mov      r3, r0
0052be14: str      r3, [sp, #0x14]
0052be18: mov      r1, ip
0052be1c: mov      r0, ip
0052be20: bl       #0x30ed6c
0052be24: ldr      r3, [sp, #0x14]
0052be28: mov      r1, r0
0052be2c: mov      r0, r3
0052be30: bl       #0x30eba4
0052be34: mov      r1, r0
0052be38: ldr      r0, [sp, #0x30]
0052be3c: bl       #0x30e2f8
0052be40: ldr      r2, [sp, #0x24]
0052be44: cmp      r0, #0
0052be48: movne    sl, r2
0052be4c: cmp      r6, #0
0052be50: cmpne    sl, #0
0052be54: beq      #0x52b690
0052be58: cmp      r6, sl
0052be5c: beq      #0x52c250
0052be60: ldr      r1, [r8, #0x78]
0052be64: ldr      r2, [r8, #0x7c]
0052be68: ldr      ip, [sp, #0x188]
0052be6c: str      r6, [sp, #0xc8]
0052be70: rsb      r2, r1, r2
0052be74: asr      r2, r2, #2
0052be78: str      sl, [sp, #0xcc]
0052be7c: add      r3, r2, r2, lsl #2
0052be80: str      ip, [sp, #0xd0]
0052be84: add      r3, r3, r3, lsl #4
0052be88: add      r3, r3, r3, lsl #8
0052be8c: add      r3, r3, r3, lsl #16
0052be90: add      r3, r2, r3, lsl #1
0052be94: subs     r2, r3, #1
0052be98: bmi      #0x52bf7c
0052be9c: mov      r0, #0xc
0052bea0: mul      r3, r0, r3
0052bea4: sub      r3, r3, #0xc
0052bea8: b        #0x52beb8
0052beac: subs     r2, r2, #1
0052beb0: sub      r3, r3, #0xc
0052beb4: bmi      #0x52bf7c
0052beb8: ldr      r0, [r1, r3]
0052bebc: add      ip, r1, r3
0052bec0: cmp      r6, r0
0052bec4: bne      #0x52beac
0052bec8: ldr      r0, [ip, #4]
0052becc: cmp      sl, r0
0052bed0: bne      #0x52beac
0052bed4: ldr      r0, [ip, #8]
0052bed8: ldr      sb, [sp, #0x188]
0052bedc: cmp      sb, r0
0052bee0: bne      #0x52beac
0052bee4: b        #0x52b690
0052bee8: bl       #0x310440
0052beec: b        #0x52b5f4
0052bef0: ldr      sl, [sp, #0x8c]
0052bef4: ldr      r0, [sp, #0x68]
0052bef8: mov      r1, sl
0052befc: bl       #0x30df8c
0052bf00: cmp      r0, #0
0052bf04: beq      #0x52bf3c
0052bf08: ldr      r2, [sp, #0x90]
0052bf0c: ldr      r0, [sp, #0x6c]
0052bf10: mov      r1, r2
0052bf14: str      r2, [sp, #0x2c]
0052bf18: bl       #0x30df8c
0052bf1c: cmp      r0, #0
0052bf20: bne      #0x52c084
0052bf24: ldr      ip, [sp, #0x98]
0052bf28: ldr      lr, [sp, #0x9c]
0052bf2c: ldr      r3, [sp, #0x94]
0052bf30: str      ip, [sp, #0x24]
0052bf34: str      lr, [sp, #0x28]
0052bf38: b        #0x52b730
0052bf3c: ldr      r2, [sp, #0x98]
0052bf40: ldr      ip, [sp, #0x90]
0052bf44: ldr      lr, [sp, #0x9c]
0052bf48: ldr      r3, [sp, #0x94]
0052bf4c: str      r2, [sp, #0x24]
0052bf50: str      ip, [sp, #0x2c]
0052bf54: str      lr, [sp, #0x28]
0052bf58: b        #0x52b730
0052bf5c: mvn      ip, #0x80000000
0052bf60: sub      ip, ip, #0x800000
0052bf64: str      ip, [sp, #0x34]
0052bf68: mov      r6, #0
0052bf6c: b        #0x52ba64
0052bf70: str      sl, [sp, #0x30]
0052bf74: ldr      sl, [sp, #0x2c]
0052bf78: b        #0x52bcb0
0052bf7c: ldr      r2, [pc, #0x3bc]
0052bf80: ldr      r3, [sp, #0x20]
0052bf84: ldr      r1, [r8, #0x48]
0052bf88: ldr      r2, [r4, r2]
0052bf8c: add      r7, sp, #0x3c
0052bf90: cmp      r3, #0
0052bf94: add      r2, r2, #8
0052bf98: add      r3, r7, #8
0052bf9c: str      r2, [sp, #0x3c]
0052bfa0: str      r1, [sp, #0x40]
0052bfa4: str      r3, [sp, #0x4c]
0052bfa8: str      r3, [sp, #0x44]
0052bfac: str      r3, [sp, #0x48]
0052bfb0: beq      #0x52c2a8
0052bfb4: ldr      r3, [r6]
0052bfb8: mov      r0, r6
0052bfbc: mov      lr, pc
0052bfc0: ldr      pc, [r3]
0052bfc4: ldr      r3, [pc, #0x378]
0052bfc8: mov      r1, r0
0052bfcc: add      lr, sp, #0xac
0052bfd0: ldr      r3, [r4, r3]
0052bfd4: ldr      ip, [pc, #0x36c]
0052bfd8: ldr      r2, [r3, #4]
0052bfdc: ldr      r0, [r3, #8]
0052bfe0: ldr      sb, [r3, #0x14]
0052bfe4: ldr      r2, [r2, #-0x18]
0052bfe8: ldr      r6, [r3, #0x10]
0052bfec: str      sb, [sp, #0x34]
0052bff0: str      r0, [lr, r2]
0052bff4: str      sl, [sp, #0xb0]
0052bff8: ldr      r2, [r6, #-0x18]
0052bffc: ldr      sb, [r3, #0xc]
0052c000: ldr      sl, [r3, #0x18]
0052c004: ldr      r3, [sp, #0x34]
0052c008: add      r2, lr, r2
0052c00c: ldr      ip, [r4, ip]
0052c010: str      r3, [r2, #8]
0052c014: ldr      r0, [sb, #-0x18]
0052c018: mov      r2, lr
0052c01c: ldr      r3, [sp, #0x188]
0052c020: add      lr, lr, r0
0052c024: str      sl, [lr, #8]
0052c028: ldr      sb, [sp, #0x20]
0052c02c: add      r6, ip, #0x3c
0052c030: mov      r0, r7
0052c034: add      ip, ip, #0x18
0052c038: str      r6, [sp, #0xb4]
0052c03c: str      sb, [sp, #0xb8]
0052c040: str      ip, [sp, #0xac]
0052c044: str      fp, [sp]
0052c048: bl       #0x52ad4c
0052c04c: mov      r6, r0
0052c050: cmp      r6, #0
0052c054: beq      #0x52c240
0052c058: cmp      fp, #0
0052c05c: beq      #0x52c078
0052c060: cmp      r6, #0
0052c064: bne      #0x52c1c0
0052c068: add      r1, sp, #0x160
0052c06c: str      r6, [r1, #-0x2c]!
0052c070: mov      r0, fp
0052c074: bl       #0x52aaa4
0052c078: mov      r0, r7
0052c07c: bl       #0x529f8c
0052c080: b        #0x52b694
0052c084: ldr      r3, [sp, #0x94]
0052c088: ldr      r0, [sp, #0x70]
0052c08c: mov      r1, r3
0052c090: str      r3, [sp, #0x14]
0052c094: bl       #0x30df8c
0052c098: cmp      r0, #0
0052c09c: ldr      r3, [sp, #0x14]
0052c0a0: bne      #0x52c0b8
0052c0a4: ldr      r2, [sp, #0x98]
0052c0a8: ldr      ip, [sp, #0x9c]
0052c0ac: str      r2, [sp, #0x24]
0052c0b0: str      ip, [sp, #0x28]
0052c0b4: b        #0x52b730
0052c0b8: ldr      lr, [sp, #0x98]
0052c0bc: ldr      r0, [sp, #0x74]
0052c0c0: str      r3, [sp, #0x14]
0052c0c4: mov      r1, lr
0052c0c8: str      lr, [sp, #0x24]
0052c0cc: bl       #0x30df8c
0052c0d0: cmp      r0, #0
0052c0d4: ldr      r3, [sp, #0x14]
0052c0d8: beq      #0x52b728
0052c0dc: ldr      ip, [sp, #0x9c]
0052c0e0: ldr      r0, [sp, #0x78]
0052c0e4: str      r3, [sp, #0x14]
0052c0e8: mov      r1, ip
0052c0ec: str      ip, [sp, #0x28]
0052c0f0: bl       #0x30df8c
0052c0f4: cmp      r0, #0
0052c0f8: ldr      r3, [sp, #0x14]
0052c0fc: beq      #0x52b730
0052c100: ldr      r0, [sp, #0x7c]
0052c104: ldr      r1, [sp, #0xa0]
0052c108: bl       #0x30df8c
0052c10c: cmp      r0, #0
0052c110: ldr      r3, [sp, #0x14]
0052c114: beq      #0x52b730
0052c118: ldr      r0, [sp, #0x80]
0052c11c: ldr      r1, [sp, #0xa4]
0052c120: bl       #0x30df8c
0052c124: cmp      r0, #0
0052c128: ldr      r3, [sp, #0x14]
0052c12c: beq      #0x52b730
0052c130: ldr      r0, [sp, #0x84]
0052c134: ldr      r1, [sp, #0xa8]
0052c138: bl       #0x30df8c
0052c13c: cmp      r0, #0
0052c140: ldr      r3, [sp, #0x14]
0052c144: beq      #0x52b730
0052c148: ldr      r2, [sp, #0x20]
0052c14c: cmp      fp, #0
0052c150: cmpne    r2, #0
0052c154: bne      #0x52c160
0052c158: mov      r6, #1
0052c15c: b        #0x52b694
0052c160: ldr      r3, [r7]
0052c164: mov      r0, fp
0052c168: mov      r6, #1
0052c16c: str      r3, [r2, #0x64]
0052c170: ldr      r3, [r7, #4]
0052c174: str      r3, [r2, #0x68]
0052c178: ldr      r3, [r7, #8]
0052c17c: str      r3, [r2, #0x6c]
0052c180: ldr      r3, [sb]
0052c184: str      r3, [r2, #0x70]
0052c188: ldr      r3, [sb, #4]
0052c18c: str      r3, [r2, #0x74]
0052c190: ldr      r3, [sb, #8]
0052c194: str      r3, [r2, #0x78]
0052c198: bl       #0x52aa84
0052c19c: ldr      sb, [sp, #0x20]
0052c1a0: add      r3, sb, #0x4c
0052c1a4: str      r3, [r0, #8]
0052c1a8: ldr      r3, [fp, #4]
0052c1ac: str      fp, [r0]
0052c1b0: str      r3, [r0, #4]
0052c1b4: str      r0, [r3]
0052c1b8: str      r0, [fp, #4]
0052c1bc: b        #0x52b694
0052c1c0: ldr      r3, [fp]
0052c1c4: cmp      r3, fp
0052c1c8: beq      #0x52c078
0052c1cc: mov      r2, #0
0052c1d0: ldr      r3, [r3]
0052c1d4: add      r2, r2, #1
0052c1d8: cmp      fp, r3
0052c1dc: bne      #0x52c1d0
0052c1e0: cmp      r2, #1
0052c1e4: bls      #0x52c078
0052c1e8: ldr      r3, [fp, #4]
0052c1ec: ldr      r3, [r3, #8]
0052c1f0: mov      r0, r3
0052c1f4: ldr      r3, [r3]
0052c1f8: mov      lr, pc
0052c1fc: ldr      pc, [r3, #4]
0052c200: ldr      ip, [sp, #0x28]
0052c204: ldr      lr, [sp, #0x2c]
0052c208: cmp      lr, r0
0052c20c: cmpne    ip, r0
0052c210: beq      #0x52c220
0052c214: ldr      r2, [sp, #0x24]
0052c218: cmp      r2, r0
0052c21c: bne      #0x52c078
0052c220: ldr      r0, [fp, #4]
0052c224: mov      r1, #0xc
0052c228: ldr      r3, [r0]
0052c22c: ldr      r2, [r0, #4]
0052c230: str      r3, [r2]
0052c234: str      r2, [r3, #4]
0052c238: bl       #0x31bb44
0052c23c: b        #0x52c078
0052c240: add      r0, r8, #0x78
0052c244: add      r1, sp, #0xc8
0052c248: bl       #0x52a504
0052c24c: b        #0x52c058
0052c250: ldr      r3, [sp, #0x20]
0052c254: cmp      fp, #0
0052c258: cmpne    r3, #0
0052c25c: beq      #0x52c158
0052c260: ldr      r3, [r7]
0052c264: ldr      sl, [sp, #0x20]
0052c268: mov      r0, fp
0052c26c: mov      r6, #1
0052c270: str      r3, [sl, #0x64]
0052c274: ldr      r3, [r7, #4]
0052c278: str      r3, [sl, #0x68]
0052c27c: ldr      r3, [r7, #8]
0052c280: str      r3, [sl, #0x6c]
0052c284: ldr      r3, [sb]
0052c288: str      r3, [sl, #0x70]
0052c28c: ldr      r3, [sb, #4]
0052c290: str      r3, [sl, #0x74]
0052c294: ldr      r3, [sb, #8]
0052c298: str      r3, [sl, #0x78]
0052c29c: bl       #0x52aa84
0052c2a0: add      r3, sl, #0x4c
0052c2a4: b        #0x52c1a4
0052c2a8: ldr      r3, [r6]
0052c2ac: mov      r0, r6
0052c2b0: mov      lr, pc
0052c2b4: ldr      pc, [r3]
0052c2b8: ldr      r3, [pc, #0x8c]
0052c2bc: mov      r1, r0
0052c2c0: add      lr, sp, #0xbc
0052c2c4: ldr      r3, [r4, r3]
0052c2c8: ldr      ip, [pc, #0x80]
0052c2cc: ldr      r2, [r3, #4]
0052c2d0: ldr      r0, [r3, #8]
0052c2d4: ldr      sb, [r3, #0xc]
0052c2d8: ldr      r2, [r2, #-0x18]
0052c2dc: ldr      r3, [r3, #0x10]
0052c2e0: ldr      ip, [r4, ip]
0052c2e4: str      r3, [sp, #0x20]
0052c2e8: str      r0, [lr, r2]
0052c2ec: str      sl, [sp, #0xc0]
0052c2f0: ldr      r0, [sb, #-0x18]
0052c2f4: ldr      sl, [sp, #0x20]
0052c2f8: mov      r2, lr
0052c2fc: add      lr, lr, r0
0052c300: add      r6, ip, #0x3c
0052c304: ldr      r3, [sp, #0x188]
0052c308: add      ip, ip, #0x18
0052c30c: str      sl, [lr, #8]
0052c310: mov      r0, r7
0052c314: str      r6, [sp, #0xc4]
0052c318: str      ip, [sp, #0xbc]
0052c31c: str      fp, [sp]
0052c320: bl       #0x52ad4c
0052c324: mov      r6, r0
0052c328: b        #0x52c050
0052c32c: bl       #0x30e310
0052c330: subeq    sb, r6, r0, lsr #10
0052c334: andeq    r4, r0, ip, lsr #1
0052c338: andeq    r0, r0, r4, lsl #17
0052c33c: eorseq   r1, fp, r0, lsl r6
0052c340: andeq    r1, r0, ip, lsl r0
0052c344: andeq    r4, r0, r0, asr #22
0052c348: andeq    r3, r0, r8, ror #7
0052c34c: andeq    r2, r0, r0, lsr sp
0052c350: andeq    r4, r0, r0, lsl r6

# _ZNSt6vectorIN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeESaIS7_EED1Ev
0052a640: push     {r4, lr}
0052a644: mov      r4, r0
0052a648: ldr      r0, [r0]
0052a64c: cmp      r0, #0
0052a650: beq      #0x52a688
0052a654: ldr      r3, [r4, #8]
0052a658: rsb      r3, r0, r3
0052a65c: asr      r3, r3, #2
0052a660: add      r1, r3, r3, lsl #2
0052a664: add      r1, r1, r1, lsl #4
0052a668: add      r1, r1, r1, lsl #8
0052a66c: add      r1, r1, r1, lsl #16
0052a670: add      r3, r3, r1, lsl #1
0052a674: mov      r1, #0xc
0052a678: mul      r1, r1, r3
0052a67c: cmp      r1, #0x80
0052a680: bhi      #0x52a690
0052a684: bl       #0x708f00
0052a688: mov      r0, r4
0052a68c: pop      {r4, pc}
0052a690: bl       #0x310440
0052a694: mov      r0, r4
0052a698: pop      {r4, pc}

# _ZN7PFWorld17ValidateDirectionER7Point3DIfERK8PFObject
00525d60: str      lr, [sp, #-4]!
00525d64: ldr      ip, [r2, #0x14]
00525d68: ldr      r3, [r2, #8]
00525d6c: sub      sp, sp, #0xc
00525d70: add      r2, r2, #0x18
00525d74: str      ip, [sp]
00525d78: bl       #0x525944
00525d7c: add      sp, sp, #0xc
00525d80: ldm      sp!, {pc}

# _ZNK24PFInnerTest_PathValidity7isValidEPK12PFGInnerNode
00525054: ldr      r3, [r1]
00525058: push     {r4, lr}
0052505c: mov      r0, r1
00525060: mov      r4, r1
00525064: mov      lr, pc
00525068: ldr      pc, [r3, #4]
0052506c: cmp      r0, #0
00525070: ldrne    r3, [r4, #0x28]
00525074: ldrne    r0, [r3, #0x20]
00525078: andne    r0, r0, #1
0052507c: pop      {r4, pc}

# _ZNK6glitch4core6line2dIfE13intersectWithERKS2_RNS0_8vector2dIfEE
00525310: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00525314: ldr      r5, [r0]
00525318: sub      sp, sp, #0x14
0052531c: mov      r4, r1
00525320: mov      r6, r0
00525324: mov      r1, r5
00525328: ldr      r0, [r0, #8]
0052532c: str      r2, [sp, #0xc]
00525330: bl       #0x30e3ac
00525334: ldr      sl, [r6, #4]
00525338: mov      sb, r0
0052533c: ldr      r0, [r6, #0xc]
00525340: mov      r1, sl
00525344: bl       #0x30e3ac
00525348: str      r0, [sp, #4]
0052534c: ldr      r7, [r4]
00525350: ldr      r1, [r4, #8]
00525354: mov      r0, r7
00525358: bl       #0x30e3ac
0052535c: ldr      r6, [r4, #4]
00525360: ldr      r1, [r4, #0xc]
00525364: mov      r8, r0
00525368: mov      r0, r6
0052536c: bl       #0x30e3ac
00525370: mov      r4, r0
00525374: mov      r1, r4
00525378: mov      r0, sb
0052537c: bl       #0x30ed6c
00525380: mov      r1, r8
00525384: mov      fp, r0
00525388: ldr      r0, [sp, #4]
0052538c: bl       #0x30ed6c
00525390: mov      r1, r0
00525394: mov      r0, fp
00525398: bl       #0x30e3ac
0052539c: movw     r1, #0x37bd
005253a0: movt     r1, #0xb586
005253a4: mov      fp, r0
005253a8: bl       #0x30e2f8
005253ac: cmp      r0, #0
005253b0: beq      #0x5253cc
005253b4: movw     r1, #0x37bd
005253b8: mov      r0, fp
005253bc: movt     r1, #0x3586
005253c0: bl       #0x30e70c
005253c4: cmp      r0, #0
005253c8: bne      #0x5254fc
005253cc: mov      r1, fp
005253d0: mov      r0, #0x3f800000
005253d4: bl       #0x30ec94
005253d8: mov      r1, r5
005253dc: mov      fp, r0
005253e0: mov      r0, r7
005253e4: bl       #0x30e3ac
005253e8: mov      r1, sl
005253ec: mov      r5, r0
005253f0: mov      r0, r6
005253f4: bl       #0x30e3ac
005253f8: mov      r1, r5
005253fc: mov      sl, r0
00525400: mov      r0, r4
00525404: bl       #0x30ed6c
00525408: mov      r1, sl
0052540c: mov      r3, r0
00525410: mov      r0, r8
00525414: str      r3, [sp]
00525418: bl       #0x30ed6c
0052541c: ldr      r3, [sp]
00525420: mov      r1, r0
00525424: mov      r0, r3
00525428: bl       #0x30e3ac
0052542c: mov      r1, fp
00525430: bl       #0x30ed6c
00525434: mov      r1, #0
00525438: str      r0, [sp, #8]
0052543c: bl       #0x30e70c
00525440: cmp      r0, #0
00525444: bne      #0x5254fc
00525448: ldr      r0, [sp, #8]
0052544c: mov      r1, #0x3f800000
00525450: bl       #0x30e2f8
00525454: cmp      r0, #0
00525458: bne      #0x5254fc
0052545c: mov      r1, sl
00525460: mov      r0, sb
00525464: bl       #0x30ed6c
00525468: mov      r1, r5
0052546c: mov      sl, r0
00525470: ldr      r0, [sp, #4]
00525474: bl       #0x30ed6c
00525478: mov      r1, r0
0052547c: mov      r0, sl
00525480: bl       #0x30e3ac
00525484: mov      r1, fp
00525488: bl       #0x30ed6c
0052548c: mov      r1, #0
00525490: mov      r5, r0
00525494: bl       #0x30e70c
00525498: cmp      r0, #0
0052549c: bne      #0x5254fc
005254a0: mov      r0, r5
005254a4: mov      r1, #0x3f800000
005254a8: bl       #0x30e2f8
005254ac: cmp      r0, #0
005254b0: bne      #0x5254fc
005254b4: mov      r1, r8
005254b8: mov      r0, r5
005254bc: bl       #0x30ed6c
005254c0: mov      r1, r0
005254c4: mov      r0, r7
005254c8: bl       #0x30e3ac
005254cc: ldr      r3, [sp, #0xc]
005254d0: mov      r1, r4
005254d4: str      r0, [r3]
005254d8: mov      r0, r5
005254dc: bl       #0x30ed6c
005254e0: mov      r1, r0
005254e4: mov      r0, r6
005254e8: bl       #0x30e3ac
005254ec: ldr      r3, [sp, #0xc]
005254f0: str      r0, [r3, #4]
005254f4: mov      r0, #1
005254f8: b        #0x525500
005254fc: mov      r0, #0
00525500: add      sp, sp, #0x14
00525504: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

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

# _ZN6PFRoom14GetCollisionAtERK7Point3DIfERS1_RN6glitch4core10triangle3dIfEEPP7PFFloorb
0052113c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00521140: ldr      sl, [r1]
00521144: mov      r6, r1
00521148: mov      r5, r0
0052114c: mov      r1, sl
00521150: ldr      r0, [r0, #0x3c]
00521154: mov      r8, r2
00521158: mov      r7, r3
0052115c: bl       #0x30e9ac
00521160: cmp      r0, #0
00521164: ldr      sb, [sp, #0x20]
00521168: ldrb     r4, [sp, #0x24]
0052116c: beq      #0x521254
00521170: mov      r0, sl
00521174: ldr      r1, [r5, #0x48]
00521178: bl       #0x30e9ac
0052117c: cmp      r0, #0
00521180: beq      #0x521254
00521184: ldr      sl, [r6, #4]
00521188: ldr      r0, [r5, #0x40]
0052118c: mov      r1, sl
00521190: bl       #0x30e9ac
00521194: cmp      r0, #0
00521198: beq      #0x521254
0052119c: mov      r0, sl
005211a0: ldr      r1, [r5, #0x4c]
005211a4: bl       #0x30e9ac
005211a8: cmp      r0, #0
005211ac: beq      #0x521254
005211b0: ldr      sl, [r6, #8]
005211b4: ldr      r0, [r5, #0x44]
005211b8: mov      r1, sl
005211bc: bl       #0x30e9ac
005211c0: cmp      r0, #0
005211c4: beq      #0x521254
005211c8: mov      r0, sl
005211cc: ldr      r1, [r5, #0x50]
005211d0: bl       #0x30e9ac
005211d4: cmp      r0, #0
005211d8: beq      #0x521254
005211dc: cmp      r4, #0
005211e0: beq      #0x52125c
005211e4: ldr      r3, [r5, #0x30]
005211e8: ldr      r2, [r5, #0x34]
005211ec: rsb      r2, r3, r2
005211f0: lsrs     r2, r2, #2
005211f4: movne    r4, #0
005211f8: bne      #0x521214
005211fc: b        #0x521254
00521200: ldr      r3, [r5, #0x30]
00521204: ldr      r2, [r5, #0x34]
00521208: rsb      r2, r3, r2
0052120c: cmp      r4, r2, asr #2
00521210: bhs      #0x521254
00521214: ldr      r0, [r3, r4, lsl #2]
00521218: mov      r1, r6
0052121c: mov      r3, r7
00521220: mov      r2, r8
00521224: bl       #0x51b96c
00521228: cmp      r0, #0
0052122c: lsl      r3, r4, #2
00521230: add      r4, r4, #1
00521234: beq      #0x521200
00521238: cmp      sb, #0
0052123c: beq      #0x5212d8
00521240: ldr      r2, [r5, #0x30]
00521244: mov      r0, #1
00521248: ldr      r3, [r2, r3]
0052124c: str      r3, [sb]
00521250: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00521254: mov      r0, #0
00521258: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0052125c: ldr      r3, [r5, #0x30]
00521260: ldr      r1, [r5, #0x34]
00521264: rsb      r2, r3, r1
00521268: lsrs     r2, r2, #2
0052126c: bne      #0x521284
00521270: b        #0x521254
00521274: add      r4, r4, #1
00521278: rsb      r2, r3, r1
0052127c: cmp      r4, r2, asr #2
00521280: bhs      #0x521254
00521284: ldr      r0, [r3, r4, lsl #2]
00521288: lsl      sl, r4, #2
0052128c: ldr      r2, [r0, #0x24]
00521290: tst      r2, #0x3000000
00521294: bne      #0x521274
00521298: mov      r1, r6
0052129c: mov      r2, r8
005212a0: mov      r3, r7
005212a4: bl       #0x51b96c
005212a8: cmp      r0, #0
005212ac: bne      #0x5212bc
005212b0: ldr      r3, [r5, #0x30]
005212b4: ldr      r1, [r5, #0x34]
005212b8: b        #0x521274
005212bc: cmp      sb, #0
005212c0: beq      #0x5212d8
005212c4: ldr      r3, [r5, #0x30]
005212c8: mov      r0, #1
005212cc: ldr      r3, [r3, sl]
005212d0: str      r3, [sb]
005212d4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
005212d8: mov      r0, #1
005212dc: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNSt4priv8_Rb_treeIjSt4lessIjESt4pairIKjN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeEENS_10_Select1stISD_EENS_11_MapTraitsTISD_EESaISD_EE13insert_uniqueENS_17_Rb_tree_iteratorISD_SH_EERKSD_
00529a30: push     {r4, r5, r6, r7, r8, sl, lr}
00529a34: ldr      r4, [r2]
00529a38: ldr      r2, [r1, #8]
00529a3c: sub      sp, sp, #0x2c
00529a40: mov      r5, r1
00529a44: cmp      r4, r2
00529a48: mov      r7, r0
00529a4c: mov      r6, r3
00529a50: beq      #0x529bc0
00529a54: cmp      r4, r1
00529a58: beq      #0x529c40
00529a5c: ldrb     r3, [r4]
00529a60: cmp      r3, #0
00529a64: beq      #0x529b54
00529a68: ldr      ip, [r4, #8]
00529a6c: cmp      ip, #0
00529a70: bne      #0x529a7c
00529a74: b        #0x529b74
00529a78: mov      ip, r3
00529a7c: ldr      r3, [ip, #0xc]
00529a80: cmp      r3, #0
00529a84: bne      #0x529a78
00529a88: ldr      r2, [r6]
00529a8c: ldr      r0, [r4, #0x10]
00529a90: cmp      r2, r0
00529a94: movhs    r1, #0
00529a98: movlo    r1, #1
00529a9c: cmp      r1, #0
00529aa0: bne      #0x529b14
00529aa4: ldr      r8, [r4, #0xc]
00529aa8: cmp      r8, #0
00529aac: beq      #0x529ca8
00529ab0: mov      ip, r8
00529ab4: b        #0x529abc
00529ab8: mov      ip, r3
00529abc: ldr      r3, [ip, #8]
00529ac0: cmp      r3, #0
00529ac4: bne      #0x529ab8
00529ac8: cmp      r1, #0
00529acc: bne      #0x529ba4
00529ad0: cmp      r2, r0
00529ad4: bls      #0x529c68
00529ad8: cmp      r5, ip
00529adc: beq      #0x529aec
00529ae0: ldr      r3, [ip, #0x10]
00529ae4: cmp      r2, r3
00529ae8: bhs      #0x529ba4
00529aec: cmp      r8, #0
00529af0: bne      #0x529c20
00529af4: mov      r1, r5
00529af8: mov      r2, r4
00529afc: mov      r3, r6
00529b00: mov      r0, r7
00529b04: str      r8, [sp]
00529b08: str      r4, [sp, #4]
00529b0c: bl       #0x529754
00529b10: b        #0x529b48
00529b14: ldr      r3, [ip, #0x10]
00529b18: cmp      r2, r3
00529b1c: bls      #0x529aa4
00529b20: ldr      lr, [ip, #0xc]
00529b24: cmp      lr, #0
00529b28: beq      #0x529c88
00529b2c: mov      ip, #0
00529b30: mov      r1, r5
00529b34: mov      r2, r4
00529b38: mov      r3, r6
00529b3c: mov      r0, r7
00529b40: stm      sp, {r4, ip}
00529b44: bl       #0x529754
00529b48: mov      r0, r7
00529b4c: add      sp, sp, #0x2c
00529b50: pop      {r4, r5, r6, r7, r8, sl, pc}
00529b54: ldr      r3, [r4, #4]
00529b58: ldr      r3, [r3, #4]
00529b5c: cmp      r4, r3
00529b60: ldreq    ip, [r4, #0xc]
00529b64: beq      #0x529a88
00529b68: ldr      ip, [r4, #8]
00529b6c: cmp      ip, #0
00529b70: bne      #0x529a7c
00529b74: ldr      ip, [r4, #4]
00529b78: ldr      r3, [ip, #8]
00529b7c: cmp      r4, r3
00529b80: beq      #0x529b8c
00529b84: b        #0x529a88
00529b88: mov      ip, r3
00529b8c: ldr      r3, [ip, #4]
00529b90: ldr      r2, [r3, #8]
00529b94: cmp      r2, ip
00529b98: beq      #0x529b88
00529b9c: mov      ip, r3
00529ba0: b        #0x529a88
00529ba4: mov      r1, r5
00529ba8: mov      r2, r6
00529bac: add      r0, sp, #8
00529bb0: bl       #0x5298a8
00529bb4: ldr      r3, [sp, #8]
00529bb8: str      r3, [r7]
00529bbc: b        #0x529b48
00529bc0: ldr      r2, [r1, #0x10]
00529bc4: cmp      r2, #0
00529bc8: beq      #0x529d18
00529bcc: ldr      r2, [r3]
00529bd0: ldr      ip, [r4, #0x10]
00529bd4: cmp      r2, ip
00529bd8: blo      #0x529d30
00529bdc: bls      #0x529c68
00529be0: ldr      lr, [r4, #0xc]
00529be4: cmp      lr, #0
00529be8: beq      #0x529ce0
00529bec: mov      ip, lr
00529bf0: b        #0x529bf8
00529bf4: mov      ip, r3
00529bf8: ldr      r3, [ip, #8]
00529bfc: cmp      r3, #0
00529c00: bne      #0x529bf4
00529c04: cmp      r5, ip
00529c08: beq      #0x529d80
00529c0c: ldr      r3, [ip, #0x10]
00529c10: cmp      r2, r3
00529c14: bhs      #0x529d44
00529c18: cmp      lr, #0
00529c1c: beq      #0x529d60
00529c20: mov      lr, #0
00529c24: mov      r1, r5
00529c28: mov      r2, ip
00529c2c: mov      r3, r6
00529c30: mov      r0, r7
00529c34: stm      sp, {ip, lr}
00529c38: bl       #0x529754
00529c3c: b        #0x529b48
00529c40: ldr      r2, [r4, #0xc]
00529c44: ldr      ip, [r3]
00529c48: ldr      lr, [r2, #0x10]
00529c4c: cmp      lr, ip
00529c50: bhs      #0x529c70
00529c54: mov      ip, #0
00529c58: str      ip, [sp]
00529c5c: str      r4, [sp, #4]
00529c60: bl       #0x529754
00529c64: b        #0x529b48
00529c68: str      r4, [r7]
00529c6c: b        #0x529b48
00529c70: mov      r2, r3
00529c74: add      r0, sp, #0x10
00529c78: bl       #0x5298a8
00529c7c: ldr      r3, [sp, #0x10]
00529c80: str      r3, [r7]
00529c84: b        #0x529b48
00529c88: mov      r1, r5
00529c8c: mov      r2, ip
00529c90: mov      r3, r6
00529c94: mov      r0, r7
00529c98: str      lr, [sp]
00529c9c: str      ip, [sp, #4]
00529ca0: bl       #0x529754
00529ca4: b        #0x529b48
00529ca8: ldr      r3, [r4, #4]
00529cac: ldr      ip, [r3, #0xc]
00529cb0: cmp      r4, ip
00529cb4: movne    ip, r4
00529cb8: bne      #0x529cd0
00529cbc: mov      ip, r3
00529cc0: ldr      r3, [r3, #4]
00529cc4: ldr      sl, [r3, #0xc]
00529cc8: cmp      ip, sl
00529ccc: beq      #0x529cbc
00529cd0: ldr      sl, [ip, #0xc]
00529cd4: cmp      r3, sl
00529cd8: movne    ip, r3
00529cdc: b        #0x529ac8
00529ce0: ldr      r3, [r4, #4]
00529ce4: ldr      r1, [r3, #0xc]
00529ce8: cmp      r4, r1
00529cec: movne    ip, r4
00529cf0: bne      #0x529d08
00529cf4: mov      ip, r3
00529cf8: ldr      r3, [r3, #4]
00529cfc: ldr      r1, [r3, #0xc]
00529d00: cmp      r1, ip
00529d04: beq      #0x529cf4
00529d08: ldr      r1, [ip, #0xc]
00529d0c: cmp      r3, r1
00529d10: movne    ip, r3
00529d14: b        #0x529c04
00529d18: mov      r2, r3
00529d1c: add      r0, sp, #0x20
00529d20: bl       #0x5298a8
00529d24: ldr      r3, [sp, #0x20]
00529d28: str      r3, [r7]
00529d2c: b        #0x529b48
00529d30: mov      ip, #0
00529d34: mov      r2, r4
00529d38: stm      sp, {r4, ip}
00529d3c: bl       #0x529754
00529d40: b        #0x529b48
00529d44: mov      r1, r5
00529d48: mov      r2, r6
00529d4c: add      r0, sp, #0x18
00529d50: bl       #0x5298a8
00529d54: ldr      r3, [sp, #0x18]
00529d58: str      r3, [r7]
00529d5c: b        #0x529b48
00529d60: mov      r1, r5
00529d64: mov      r2, r4
00529d68: mov      r3, r6
00529d6c: mov      r0, r7
00529d70: str      lr, [sp]
00529d74: str      r4, [sp, #4]
00529d78: bl       #0x529754
00529d7c: b        #0x529b48
00529d80: mov      ip, #0
00529d84: mov      r1, r5
00529d88: mov      r2, r4
00529d8c: mov      r3, r6
00529d90: mov      r0, r7
00529d94: str      ip, [sp]
00529d98: str      r4, [sp, #4]
00529d9c: bl       #0x529754
00529da0: b        #0x529b48

# _ZNK8PFObject8IsFlyingEv
005241e8: ldr      r0, [r0, #0x14]
005241ec: and      r0, r0, #1
005241f0: bx       lr

# _ZNK3sfc4math5graph4EdgeI12PFGInnerNodefE7isValidEv
0051c10c: mov      r0, #1
0051c110: bx       lr

# _ZSt15__push_heap_auxIPN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeENS6_6_ECompEiS7_EvT_SA_T0_PT1_PT2_
0052958c: push     {r4, lr}
00529590: rsb      r3, r0, r1
00529594: asr      r3, r3, #2
00529598: ldr      r4, [r1, #-0xc]
0052959c: add      ip, r3, r3, lsl #2
005295a0: sub      r2, r1, #0xc
005295a4: add      r1, ip, ip, lsl #4
005295a8: ldr      lr, [r2, #4]
005295ac: add      r1, r1, r1, lsl #8
005295b0: ldr      ip, [r2, #8]
005295b4: add      r1, r1, r1, lsl #16
005295b8: sub      sp, sp, #0x10
005295bc: add      r3, r3, r1, lsl #1
005295c0: sub      r1, r3, #1
005295c4: add      r2, sp, #4
005295c8: mov      r3, #0
005295cc: str      r4, [sp, #4]
005295d0: str      lr, [sp, #8]
005295d4: str      ip, [sp, #0xc]
005295d8: bl       #0x529398
005295dc: add      sp, sp, #0x10
005295e0: pop      {r4, pc}

# _ZNSt4priv8_Rb_treeIjSt4lessIjESt4pairIKjN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeEENS_10_Select1stISD_EENS_11_MapTraitsTISD_EESaISD_EE13insert_uniqueERKSD_
005298a8: push     {r4, r5, r6, lr}
005298ac: ldr      ip, [r1, #4]
005298b0: sub      sp, sp, #0x10
005298b4: mov      r4, r0
005298b8: cmp      ip, #0
005298bc: mov      r3, r2
005298c0: moveq    ip, r1
005298c4: beq      #0x529920
005298c8: ldr      r6, [r2]
005298cc: b        #0x5298d4
005298d0: mov      ip, r2
005298d4: ldr      r0, [ip, #0x10]
005298d8: mov      r5, #1
005298dc: cmp      r0, r6
005298e0: ldrhi    r2, [ip, #8]
005298e4: ldrls    r2, [ip, #0xc]
005298e8: movls    r5, #0
005298ec: cmp      r2, #0
005298f0: bne      #0x5298d0
005298f4: cmp      r5, #0
005298f8: moveq    r5, ip
005298fc: bne      #0x529920
00529900: cmp      r6, r0
00529904: movls    r3, #0
00529908: strls    r5, [r4]
0052990c: strbls   r3, [r4, #4]
00529910: bhi      #0x529988
00529914: mov      r0, r4
00529918: add      sp, sp, #0x10
0052991c: pop      {r4, r5, r6, pc}
00529920: ldr      r2, [r1, #8]
00529924: cmp      ip, r2
00529928: beq      #0x529a08
0052992c: ldrb     r2, [ip]
00529930: cmp      r2, #0
00529934: bne      #0x529948
00529938: ldr      r2, [ip, #4]
0052993c: ldr      r2, [r2, #4]
00529940: cmp      ip, r2
00529944: beq      #0x5299f4
00529948: ldr      r0, [ip, #8]
0052994c: cmp      r0, #0
00529950: bne      #0x52995c
00529954: b        #0x5299b4
00529958: mov      r0, r2
0052995c: ldr      r2, [r0, #0xc]
00529960: cmp      r2, #0
00529964: bne      #0x529958
00529968: ldr      r6, [r3]
0052996c: mov      r5, r0
00529970: ldr      r0, [r0, #0x10]
00529974: cmp      r6, r0
00529978: movls    r3, #0
0052997c: strls    r5, [r4]
00529980: strbls   r3, [r4, #4]
00529984: bls      #0x529914
00529988: mov      r2, ip
0052998c: add      r0, sp, #8
00529990: mov      ip, #0
00529994: str      ip, [sp, #4]
00529998: str      ip, [sp]
0052999c: bl       #0x529754
005299a0: ldr      r3, [sp, #8]
005299a4: mov      r2, #1
005299a8: strb     r2, [r4, #4]
005299ac: str      r3, [r4]
005299b0: b        #0x529914
005299b4: ldr      r2, [ip, #4]
005299b8: ldr      r0, [r2, #8]
005299bc: cmp      ip, r0
005299c0: movne    r5, r2
005299c4: ldrne    r6, [r3]
005299c8: ldrne    r0, [r2, #0x10]
005299cc: beq      #0x5299d8
005299d0: b        #0x529900
005299d4: mov      r2, r5
005299d8: ldr      r5, [r2, #4]
005299dc: ldr      r0, [r5, #8]
005299e0: cmp      r0, r2
005299e4: beq      #0x5299d4
005299e8: ldr      r6, [r3]
005299ec: ldr      r0, [r5, #0x10]
005299f0: b        #0x529900
005299f4: ldr      r2, [ip, #0xc]
005299f8: ldr      r6, [r3]
005299fc: mov      r5, r2
00529a00: ldr      r0, [r2, #0x10]
00529a04: b        #0x529900
00529a08: mov      r2, ip
00529a0c: mov      lr, #0
00529a10: add      r0, sp, #0xc
00529a14: stm      sp, {ip, lr}
00529a18: bl       #0x529754
00529a1c: ldr      r3, [sp, #0xc]
00529a20: mov      r2, #1
00529a24: strb     r2, [r4, #4]
00529a28: str      r3, [r4]
00529a2c: b        #0x529914

# _ZNK20PFInnerTest_PFObject7isValidEPK12PFGInnerNode
0052510c: push     {r4, r5, r6, lr}
00525110: mov      r5, r0
00525114: mov      r4, r1
00525118: bl       #0x525054
0052511c: cmp      r0, #0
00525120: bne      #0x525128
00525124: pop      {r4, r5, r6, pc}
00525128: ldr      r0, [r5, #4]
0052512c: ldr      r1, [r4, #0x28]
00525130: pop      {r4, r5, r6, lr}
00525134: b        #0x524230

# _ZNSaIN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeEE11_M_allocateEjRj
0052a0a4: push     {r4, lr}
0052a0a8: movw     r3, #0x5555
0052a0ac: orr      r3, r3, r3, lsl #14
0052a0b0: cmp      r1, r3
0052a0b4: sub      sp, sp, #8
0052a0b8: mov      r4, r2
0052a0bc: bhi      #0x52a114
0052a0c0: cmp      r1, #0
0052a0c4: moveq    r0, r1
0052a0c8: bne      #0x52a0d4
0052a0cc: add      sp, sp, #8
0052a0d0: pop      {r4, pc}
0052a0d4: mov      r0, #0xc
0052a0d8: mul      r0, r0, r1
0052a0dc: cmp      r0, #0x80
0052a0e0: str      r0, [sp, #4]
0052a0e4: bhi      #0x52a10c
0052a0e8: add      r0, sp, #4
0052a0ec: bl       #0x708ec0
0052a0f0: ldr      r2, [sp, #4]
0052a0f4: movw     r3, #0xaaab
0052a0f8: movt     r3, #0xaaaa
0052a0fc: umull    r1, r3, r3, r2
0052a100: lsr      r3, r3, #3
0052a104: str      r3, [r4]
0052a108: b        #0x52a0cc
0052a10c: bl       #0x310454
0052a110: b        #0x52a0f0
0052a114: ldr      r0, [pc, #0xc]
0052a118: add      r0, pc, r0
0052a11c: bl       #0x30e0c4
0052a120: mov      r0, #1
0052a124: bl       #0x30de48
0052a128: eorseq   r4, sb, r8, asr r3

# _ZNK7Point3DIfE8angleCosERKS0_
00312f40: push     {r4, r5, r6, r7, r8, lr}
00312f44: ldr      r7, [r0]
00312f48: mov      r3, r0
00312f4c: mov      r4, r1
00312f50: ldr      r6, [r0, #4]
00312f54: ldr      r1, [r1]
00312f58: mov      r0, r7
00312f5c: ldr      r5, [r3, #8]
00312f60: bl       #0x30ed6c
00312f64: ldr      r1, [r4, #4]
00312f68: mov      r8, r0
00312f6c: mov      r0, r6
00312f70: bl       #0x30ed6c
00312f74: mov      r1, r0
00312f78: mov      r0, r8
00312f7c: bl       #0x30eba4
00312f80: ldr      r1, [r4, #8]
00312f84: mov      r8, r0
00312f88: mov      r0, r5
00312f8c: bl       #0x30ed6c
00312f90: mov      r1, r0
00312f94: mov      r0, r8
00312f98: bl       #0x30eba4
00312f9c: mov      r1, r7
00312fa0: mov      r8, r0
00312fa4: mov      r0, r7
00312fa8: bl       #0x30ed6c
00312fac: mov      r1, r6
00312fb0: mov      r7, r0
00312fb4: mov      r0, r6
00312fb8: bl       #0x30ed6c
00312fbc: mov      r1, r0
00312fc0: mov      r0, r7
00312fc4: bl       #0x30eba4
00312fc8: mov      r1, r5
00312fcc: mov      r6, r0
00312fd0: mov      r0, r5
00312fd4: bl       #0x30ed6c
00312fd8: mov      r1, r0
00312fdc: mov      r0, r6
00312fe0: bl       #0x30eba4
00312fe4: bl       #0x30e124
00312fe8: mov      r5, r0
00312fec: ldr      r0, [r4]
00312ff0: ldr      r7, [r4, #4]
00312ff4: ldr      r6, [r4, #8]
00312ff8: mov      r1, r0
00312ffc: bl       #0x30ed6c
00313000: mov      r1, r7
00313004: mov      r4, r0
00313008: mov      r0, r7
0031300c: bl       #0x30ed6c
00313010: mov      r1, r0
00313014: mov      r0, r4
00313018: bl       #0x30eba4
0031301c: mov      r1, r6
00313020: mov      r4, r0
00313024: mov      r0, r6
00313028: bl       #0x30ed6c
0031302c: mov      r1, r0
00313030: mov      r0, r4
00313034: bl       #0x30eba4
00313038: bl       #0x30e124
0031303c: mov      r1, r0
00313040: mov      r0, r5
00313044: bl       #0x30ed6c
00313048: mov      r1, r0
0031304c: mov      r0, r8
00313050: bl       #0x30ec94
00313054: pop      {r4, r5, r6, r7, r8, pc}

# _ZNSt14priority_queueIN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeESt6vectorIS7_SaIS7_EENS6_6_ECompEE4pushERKS7_
0052a890: push     {r4, r5, r6, r7, lr}
0052a894: ldmib    r0, {r3, r6}
0052a898: sub      sp, sp, #0x14
0052a89c: mov      r4, r0
0052a8a0: cmp      r3, r6
0052a8a4: mov      r5, r1
0052a8a8: beq      #0x52a8f8
0052a8ac: ldr      r2, [r1]
0052a8b0: str      r2, [r3]
0052a8b4: ldr      r2, [r1, #4]
0052a8b8: str      r2, [r3, #4]
0052a8bc: ldr      r2, [r1, #8]
0052a8c0: str      r2, [r3, #8]
0052a8c4: ldr      r6, [r0, #4]
0052a8c8: ldr      r7, [r0]
0052a8cc: add      r6, r6, #0xc
0052a8d0: str      r6, [r0, #4]
0052a8d4: mov      ip, #0
0052a8d8: mov      r0, r7
0052a8dc: mov      r1, r6
0052a8e0: mov      r3, ip
0052a8e4: mov      r2, #0
0052a8e8: str      ip, [sp]
0052a8ec: bl       #0x52958c
0052a8f0: add      sp, sp, #0x14
0052a8f4: pop      {r4, r5, r6, r7, pc}
0052a8f8: ldr      r2, [r0]
0052a8fc: movw     r3, #0x5555
0052a900: orr      r3, r3, r3, lsl #14
0052a904: rsb      r2, r2, r6
0052a908: asr      r2, r2, #2
0052a90c: add      r1, r2, r2, lsl #2
0052a910: add      r1, r1, r1, lsl #4
0052a914: add      r1, r1, r1, lsl #8
0052a918: add      r1, r1, r1, lsl #16
0052a91c: add      r2, r2, r1, lsl #1
0052a920: cmp      r2, #1
0052a924: addhs    r1, r2, r2
0052a928: addlo    r1, r2, #1
0052a92c: cmp      r1, r3
0052a930: bhi      #0x52aa70
0052a934: cmp      r2, r1
0052a938: bhi      #0x52aa70
0052a93c: add      r2, sp, #0x10
0052a940: str      r1, [r2, #-4]!
0052a944: add      r0, r4, #8
0052a948: bl       #0x52a0a4
0052a94c: ldr      r3, [r4]
0052a950: mov      r7, r0
0052a954: rsb      r6, r3, r6
0052a958: asr      r6, r6, #2
0052a95c: add      r2, r6, r6, lsl #2
0052a960: add      r2, r2, r2, lsl #4
0052a964: add      r2, r2, r2, lsl #8
0052a968: add      r2, r2, r2, lsl #16
0052a96c: add      r6, r6, r2, lsl #1
0052a970: cmp      r6, #0
0052a974: movle    r3, r0
0052a978: ble      #0x52a9b4
0052a97c: mov      r1, r6
0052a980: mov      r2, r0
0052a984: ldr      r0, [r3]
0052a988: subs     r1, r1, #1
0052a98c: str      r0, [r2]
0052a990: ldr      r0, [r3, #4]
0052a994: str      r0, [r2, #4]
0052a998: ldr      r0, [r3, #8]
0052a99c: add      r3, r3, #0xc
0052a9a0: str      r0, [r2, #8]
0052a9a4: add      r2, r2, #0xc
0052a9a8: bne      #0x52a984
0052a9ac: mov      r3, #0xc
0052a9b0: mla      r3, r3, r6, r7
0052a9b4: ldr      r2, [r5]
0052a9b8: add      r6, r3, #0xc
0052a9bc: str      r2, [r3]
0052a9c0: ldr      r2, [r5, #4]
0052a9c4: str      r2, [r3, #4]
0052a9c8: ldr      r2, [r5, #8]
0052a9cc: str      r2, [r3, #8]
0052a9d0: ldm      r4, {r0, r3}
0052a9d4: cmp      r3, r0
0052a9d8: beq      #0x52aa18
0052a9dc: sub      r2, r3, #0xc
0052a9e0: rsb      r2, r0, r2
0052a9e4: lsr      r2, r2, #2
0052a9e8: add      r1, r2, r2, lsl #2
0052a9ec: add      r1, r1, r1, lsl #5
0052a9f0: add      r1, r2, r1, lsl #1
0052a9f4: add      r1, r1, r1, lsl #5
0052a9f8: lsl      ip, r1, #0xf
0052a9fc: rsb      r1, r1, ip
0052aa00: add      r2, r2, r1, lsl #1
0052aa04: bic      r2, r2, #0xc0000000
0052aa08: mvn      r1, #0xb
0052aa0c: mul      r2, r1, r2
0052aa10: add      r2, r2, r1
0052aa14: add      r3, r3, r2
0052aa18: cmp      r3, #0
0052aa1c: ldr      r2, [r4, #8]
0052aa20: beq      #0x52aa54
0052aa24: rsb      r3, r3, r2
0052aa28: asr      r3, r3, #2
0052aa2c: add      r1, r3, r3, lsl #2
0052aa30: add      r1, r1, r1, lsl #4
0052aa34: add      r1, r1, r1, lsl #8
0052aa38: add      r1, r1, r1, lsl #16
0052aa3c: add      r3, r3, r1, lsl #1
0052aa40: mov      r1, #0xc
0052aa44: mul      r1, r1, r3
0052aa48: cmp      r1, #0x80
0052aa4c: bhi      #0x52aa7c
0052aa50: bl       #0x708f00
0052aa54: ldr      r3, [sp, #0xc]
0052aa58: mov      r2, #0xc
0052aa5c: str      r7, [r4]
0052aa60: mla      r3, r2, r3, r7
0052aa64: str      r6, [r4, #4]
0052aa68: str      r3, [r4, #8]
0052aa6c: b        #0x52a8d4
0052aa70: movw     r1, #0x5555
0052aa74: orr      r1, r1, r1, lsl #14
0052aa78: b        #0x52a93c
0052aa7c: bl       #0x310440
0052aa80: b        #0x52aa54

# _ZNK25PFInnerTest_BasicIdentityclEPK12PFGInnerNode
00525028: ldr      r0, [r0, #4]
0052502c: cmp      r0, r1
00525030: movne    r0, #0
00525034: moveq    r0, #1
00525038: bx       lr

# _ZN7PFWorld14GetCollisionAtERK7Point3DIfERS1_RN6glitch4core10triangle3dIfEEPP6PFRoomPP7PFFloorb
005256d4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005256d8: ldr      r4, [r1]
005256dc: sub      sp, sp, #0xc
005256e0: mov      r6, r1
005256e4: mov      r5, r0
005256e8: mov      r1, r4
005256ec: ldr      r0, [r0, #0x14]
005256f0: mov      sl, r2
005256f4: mov      r8, r3
005256f8: bl       #0x30e9ac
005256fc: cmp      r0, #0
00525700: ldr      fp, [sp, #0x30]
00525704: ldr      sb, [sp, #0x34]
00525708: ldrb     r7, [sp, #0x38]
0052570c: beq      #0x525724
00525710: mov      r0, r4
00525714: ldr      r1, [r5, #0x20]
00525718: bl       #0x30e9ac
0052571c: cmp      r0, #0
00525720: bne      #0x525730
00525724: mov      r0, #0
00525728: add      sp, sp, #0xc
0052572c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00525730: ldr      r4, [r6, #4]
00525734: ldr      r0, [r5, #0x18]
00525738: mov      r1, r4
0052573c: bl       #0x30e9ac
00525740: cmp      r0, #0
00525744: beq      #0x525724
00525748: mov      r0, r4
0052574c: ldr      r1, [r5, #0x24]
00525750: bl       #0x30e9ac
00525754: cmp      r0, #0
00525758: beq      #0x525724
0052575c: ldr      r4, [r6, #8]
00525760: ldr      r0, [r5, #0x1c]
00525764: mov      r1, r4
00525768: bl       #0x30e9ac
0052576c: cmp      r0, #0
00525770: beq      #0x525724
00525774: mov      r0, r4
00525778: ldr      r1, [r5, #0x28]
0052577c: bl       #0x30e9ac
00525780: cmp      r0, #0
00525784: beq      #0x525724
00525788: ldr      r3, [r5, #8]
0052578c: ldr      r2, [r5, #0xc]
00525790: rsb      r2, r3, r2
00525794: lsrs     r2, r2, #2
00525798: beq      #0x525724
0052579c: mov      r4, #0
005257a0: b        #0x5257b8
005257a4: ldr      r3, [r5, #8]
005257a8: ldr      r2, [r5, #0xc]
005257ac: rsb      r2, r3, r2
005257b0: cmp      r4, r2, asr #2
005257b4: bhs      #0x525724
005257b8: ldr      r0, [r3, r4, lsl #2]
005257bc: mov      r1, r6
005257c0: mov      r3, r8
005257c4: mov      r2, sl
005257c8: str      sb, [sp]
005257cc: str      r7, [sp, #4]
005257d0: bl       #0x52113c
005257d4: cmp      r0, #0
005257d8: lsl      r3, r4, #2
005257dc: add      r4, r4, #1
005257e0: beq      #0x5257a4
005257e4: cmp      fp, #0
005257e8: ldrne    r2, [r5, #8]
005257ec: moveq    r0, #1
005257f0: movne    r0, #1
005257f4: ldrne    r3, [r2, r3]
005257f8: strne    r3, [fp]
005257fc: b        #0x525728

# _ZNK3sfc4math5graph4EdgeI12PFGInnerNodefE9getWeightEv
0051c104: ldr      r0, [r0, #0xc]
0051c108: bx       lr

# _ZNK7Point3DIfEeqERKS0_
00312b6c: push     {r4, r5, r6, lr}
00312b70: mov      r4, r0
00312b74: mov      r5, r1
00312b78: ldr      r0, [r0]
00312b7c: ldr      r1, [r1]
00312b80: bl       #0x30e3ac
00312b84: movw     r1, #0xb717
00312b88: bic      r0, r0, #0x80000000
00312b8c: movt     r1, #0x38d1
00312b90: bl       #0x30e70c
00312b94: cmp      r0, #0
00312b98: beq      #0x312bf0
00312b9c: ldr      r1, [r5, #4]
00312ba0: ldr      r0, [r4, #4]
00312ba4: bl       #0x30e3ac
00312ba8: movw     r1, #0xb717
00312bac: bic      r0, r0, #0x80000000
00312bb0: movt     r1, #0x38d1
00312bb4: bl       #0x30e70c
00312bb8: cmp      r0, #0
00312bbc: beq      #0x312bf0
00312bc0: ldr      r1, [r5, #8]
00312bc4: ldr      r0, [r4, #8]
00312bc8: bl       #0x30e3ac
00312bcc: movw     r1, #0xb717
00312bd0: bic      r0, r0, #0x80000000
00312bd4: movt     r1, #0x38d1
00312bd8: bl       #0x30e70c
00312bdc: cmp      r0, #0
00312be0: mov      r0, #0
00312be4: movne    r0, #1
00312be8: uxtb     r0, r0
00312bec: pop      {r4, r5, r6, pc}
00312bf0: mov      r0, #0
00312bf4: pop      {r4, r5, r6, pc}

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

# _ZNK7Point3DIfE5angleERKS0_
00313058: push     {r4, lr}
0031305c: bl       #0x312f40
00313060: pop      {r4, lr}
00313064: b        #0x30e3dc

# _ZN7PFFloor16GetFloorHeightAtERK7Point3DIfEPfPS1_
0051badc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0051bae0: sub      sp, sp, #0x34
0051bae4: mov      ip, #0
0051bae8: mov      r5, r2
0051baec: mov      r4, r3
0051baf0: add      r2, sp, #0x24
0051baf4: mov      r3, sp
0051baf8: str      ip, [sp, #0x20]
0051bafc: str      ip, [sp, #0x24]
0051bb00: str      ip, [sp, #0x28]
0051bb04: str      ip, [sp, #0x2c]
0051bb08: str      ip, [sp]
0051bb0c: str      ip, [sp, #4]
0051bb10: str      ip, [sp, #8]
0051bb14: str      ip, [sp, #0xc]
0051bb18: str      ip, [sp, #0x10]
0051bb1c: str      ip, [sp, #0x14]
0051bb20: str      ip, [sp, #0x18]
0051bb24: str      ip, [sp, #0x1c]
0051bb28: bl       #0x51b96c
0051bb2c: cmp      r0, #0
0051bb30: beq      #0x51bc38
0051bb34: cmp      r5, #0
0051bb38: ldrne    r3, [sp, #0x2c]
0051bb3c: strne    r3, [r5]
0051bb40: cmp      r4, #0
0051bb44: beq      #0x51bc34
0051bb48: ldr      r5, [sp]
0051bb4c: ldr      r0, [sp, #0xc]
0051bb50: mov      r1, r5
0051bb54: bl       #0x30e3ac
0051bb58: ldr      r7, [sp, #4]
0051bb5c: mov      r8, r0
0051bb60: ldr      r0, [sp, #0x10]
0051bb64: mov      r1, r7
0051bb68: bl       #0x30e3ac
0051bb6c: ldr      sb, [sp, #8]
0051bb70: mov      r6, r0
0051bb74: ldr      r0, [sp, #0x14]
0051bb78: mov      r1, sb
0051bb7c: bl       #0x30e3ac
0051bb80: mov      r1, r5
0051bb84: mov      sl, r0
0051bb88: ldr      r0, [sp, #0x18]
0051bb8c: bl       #0x30e3ac
0051bb90: mov      r1, r7
0051bb94: mov      r5, r0
0051bb98: ldr      r0, [sp, #0x1c]
0051bb9c: bl       #0x30e3ac
0051bba0: mov      r1, sb
0051bba4: mov      r7, r0
0051bba8: ldr      r0, [sp, #0x20]
0051bbac: bl       #0x30e3ac
0051bbb0: add      r1, r6, #0x80000000
0051bbb4: mov      sb, r0
0051bbb8: bl       #0x30ed6c
0051bbbc: mov      r1, r7
0051bbc0: mov      fp, r0
0051bbc4: mov      r0, sl
0051bbc8: bl       #0x30ed6c
0051bbcc: mov      r1, r0
0051bbd0: mov      r0, fp
0051bbd4: bl       #0x30eba4
0051bbd8: add      r1, sl, #0x80000000
0051bbdc: str      r0, [r4]
0051bbe0: mov      r0, r5
0051bbe4: bl       #0x30ed6c
0051bbe8: mov      r1, sb
0051bbec: mov      sl, r0
0051bbf0: mov      r0, r8
0051bbf4: bl       #0x30ed6c
0051bbf8: mov      r1, r0
0051bbfc: mov      r0, sl
0051bc00: bl       #0x30eba4
0051bc04: add      r1, r8, #0x80000000
0051bc08: str      r0, [r4, #4]
0051bc0c: mov      r0, r7
0051bc10: bl       #0x30ed6c
0051bc14: mov      r1, r5
0051bc18: mov      r7, r0
0051bc1c: mov      r0, r6
0051bc20: bl       #0x30ed6c
0051bc24: mov      r1, r0
0051bc28: mov      r0, r7
0051bc2c: bl       #0x30eba4
0051bc30: str      r0, [r4, #8]
0051bc34: mov      r0, #1
0051bc38: add      sp, sp, #0x34
0051bc3c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK3sfc4math5graph5ITestI12PFGInnerEdge12PFGInnerNodeE7isValidEPKS3_
005250bc: push     {r4, lr}
005250c0: mov      r0, r1
005250c4: ldr      r3, [r1]
005250c8: mov      lr, pc
005250cc: ldr      pc, [r3, #0x18]
005250d0: pop      {r4, pc}

# _ZNK12PFGInnerEdge14GetDestinationEv
0051b85c: push     {r4, lr}
0051b860: ldr      r3, [r0]
0051b864: mov      lr, pc
0051b868: ldr      pc, [r3, #0xc]
0051b86c: add      r0, r0, #8
0051b870: pop      {r4, pc}

# _ZN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE8findNodeEjRKNS1_5ITestI12PFGInnerEdge12PFGInnerNodeEEjPSt4listIPKS7_SaISE_EE
0052ad4c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0052ad50: mov      r5, r0
0052ad54: ldr      r0, [r0, #4]
0052ad58: mov      r6, r2
0052ad5c: sub      sp, sp, #0xd4
0052ad60: ldr      r2, [r0, #8]
0052ad64: ldr      sl, [sp, #0xf8]
0052ad68: str      r3, [sp, #0xc]
0052ad6c: cmp      r2, #0
0052ad70: add      r0, r0, #4
0052ad74: beq      #0x52b510
0052ad78: mov      ip, r0
0052ad7c: b        #0x52ad84
0052ad80: mov      r2, r3
0052ad84: ldr      r3, [r2, #0x10]
0052ad88: cmp      r1, r3
0052ad8c: ldrhi    r3, [r2, #0xc]
0052ad90: ldrls    r3, [r2, #8]
0052ad94: movhi    r2, ip
0052ad98: mov      ip, r2
0052ad9c: cmp      r3, #0
0052ada0: bne      #0x52ad80
0052ada4: cmp      r0, r2
0052ada8: beq      #0x52b53c
0052adac: ldr      r3, [r2, #0x10]
0052adb0: cmp      r1, r3
0052adb4: blo      #0x52b510
0052adb8: cmp      r0, r2
0052adbc: beq      #0x52b53c
0052adc0: ldr      r2, [r2, #0x14]
0052adc4: mov      r8, #0
0052adc8: str      r2, [sp, #0x10]
0052adcc: add      r2, r5, #8
0052add0: str      r2, [sp, #0x1c]
0052add4: strb     r8, [r5, #0x14]
0052add8: ldr      r0, [sp, #0x1c]
0052addc: bl       #0x529f4c
0052ade0: ldr      r3, [sp, #0x10]
0052ade4: str      r8, [r5, #0x18]
0052ade8: str      r8, [r5, #0x1c]
0052adec: cmp      r3, r8
0052adf0: str      r8, [r5, #0x20]
0052adf4: str      r8, [r5, #0x24]
0052adf8: beq      #0x52b3f0
0052adfc: ldr      ip, [sp, #0x1c]
0052ae00: cmp      sl, r8
0052ae04: add      r7, sp, #0xd0
0052ae08: moveq    sl, ip
0052ae0c: strb     r8, [r7, #-0x9c]!
0052ae10: str      sl, [r5, #0x10]
0052ae14: ldr      r2, [sp, #0x10]
0052ae18: add      r4, sp, #0xb0
0052ae1c: str      r8, [sp, #0x44]
0052ae20: str      r4, [sp, #0xb0]
0052ae24: str      r4, [sp, #0xb4]
0052ae28: str      r8, [sp, #0x7c]
0052ae2c: str      r8, [sp, #0x80]
0052ae30: str      r8, [sp, #0x84]
0052ae34: str      r8, [sp, #0x38]
0052ae38: str      r7, [sp, #0x3c]
0052ae3c: str      r7, [sp, #0x40]
0052ae40: ldr      r3, [r2]
0052ae44: mov      r0, r2
0052ae48: mov      lr, pc
0052ae4c: ldr      pc, [r3]
0052ae50: ldr      ip, [sp, #0x38]
0052ae54: mov      lr, r0
0052ae58: cmp      ip, r8
0052ae5c: moveq    ip, r7
0052ae60: beq      #0x52aea4
0052ae64: mov      r2, r7
0052ae68: b        #0x52ae70
0052ae6c: mov      ip, r3
0052ae70: ldr      r3, [ip, #0x10]
0052ae74: cmp      lr, r3
0052ae78: ldrhi    r3, [ip, #0xc]
0052ae7c: ldrls    r3, [ip, #8]
0052ae80: movhi    ip, r2
0052ae84: mov      r2, ip
0052ae88: cmp      r3, #0
0052ae8c: bne      #0x52ae6c
0052ae90: cmp      ip, r7
0052ae94: beq      #0x52aea4
0052ae98: ldr      r3, [ip, #0x10]
0052ae9c: cmp      lr, r3
0052aea0: bhs      #0x52aed8
0052aea4: mov      r8, #0
0052aea8: str      ip, [sp, #0xcc]
0052aeac: add      r0, sp, #0xc8
0052aeb0: mov      ip, #0
0052aeb4: mov      r1, r7
0052aeb8: add      r2, sp, #0xcc
0052aebc: add      r3, sp, #0x6c
0052aec0: str      ip, [sp, #0x70]
0052aec4: str      lr, [sp, #0x6c]
0052aec8: str      r8, [sp, #0x78]
0052aecc: str      r8, [sp, #0x74]
0052aed0: bl       #0x529a30
0052aed4: ldr      ip, [sp, #0xc8]
0052aed8: mov      sl, #0
0052aedc: mov      r3, #0
0052aee0: str      r3, [ip, #0x14]
0052aee4: str      sl, [ip, #0x1c]
0052aee8: str      sl, [ip, #0x18]
0052aeec: ldr      sb, [sp, #0x10]
0052aef0: add      r3, sp, #0x7c
0052aef4: add      ip, sp, #0x98
0052aef8: add      r2, sp, #0x8c
0052aefc: str      r3, [sp, #0x14]
0052af00: add      r8, sp, #0xa4
0052af04: str      ip, [sp, #0x20]
0052af08: str      r2, [sp, #0x18]
0052af0c: ldr      r3, [r6]
0052af10: mov      r0, r6
0052af14: mov      r1, sb
0052af18: mov      lr, pc
0052af1c: ldr      pc, [r3]
0052af20: cmp      r0, #0
0052af24: beq      #0x52b124
0052af28: mov      r0, r6
0052af2c: ldr      r3, [r6]
0052af30: mov      r1, sb
0052af34: mov      lr, pc
0052af38: ldr      pc, [r3]
0052af3c: cmp      r0, #0
0052af40: strb     r0, [r5, #0x14]
0052af44: beq      #0x52b474
0052af48: add      r3, sp, #0xc0
0052af4c: add      ip, sp, #0xc4
0052af50: add      r2, sp, #0x5c
0052af54: str      r3, [sp, #0xc]
0052af58: str      ip, [sp, #0x18]
0052af5c: str      r2, [sp, #0x20]
0052af60: add      r3, sp, #0xb8
0052af64: add      ip, sp, #0xbc
0052af68: add      r2, sp, #0x4c
0052af6c: mov      sl, #0
0052af70: mov      fp, #0
0052af74: str      r3, [sp, #0x24]
0052af78: str      ip, [sp, #0x28]
0052af7c: str      r2, [sp, #0x2c]
0052af80: mov      r6, r5
0052af84: mov      r8, r4
0052af88: ldr      r3, [sb]
0052af8c: mov      r0, sb
0052af90: mov      lr, pc
0052af94: ldr      pc, [r3]
0052af98: ldr      ip, [sp, #0x10]
0052af9c: mov      r4, r0
0052afa0: ldr      r3, [ip]
0052afa4: mov      r0, ip
0052afa8: mov      lr, pc
0052afac: ldr      pc, [r3]
0052afb0: cmp      r4, r0
0052afb4: beq      #0x52b3c4
0052afb8: ldr      r3, [sb]
0052afbc: mov      r0, sb
0052afc0: ldr      r5, [r6, #0x10]
0052afc4: mov      lr, pc
0052afc8: ldr      pc, [r3]
0052afcc: ldr      r4, [sp, #0x38]
0052afd0: mov      ip, r0
0052afd4: cmp      r4, #0
0052afd8: moveq    r4, r7
0052afdc: beq      #0x52b020
0052afe0: mov      r2, r7
0052afe4: b        #0x52afec
0052afe8: mov      r4, r3
0052afec: ldr      r3, [r4, #0x10]
0052aff0: cmp      ip, r3
0052aff4: ldrhi    r3, [r4, #0xc]
0052aff8: ldrls    r3, [r4, #8]
0052affc: movhi    r4, r2
0052b000: mov      r2, r4
0052b004: cmp      r3, #0
0052b008: bne      #0x52afe8
0052b00c: cmp      r4, r7
0052b010: beq      #0x52b020
0052b014: ldr      r3, [r4, #0x10]
0052b018: cmp      ip, r3
0052b01c: bhs      #0x52b04c
0052b020: ldr      r0, [sp, #0xc]
0052b024: mov      r1, r7
0052b028: ldr      r2, [sp, #0x18]
0052b02c: ldr      r3, [sp, #0x20]
0052b030: str      r4, [sp, #0xc4]
0052b034: str      ip, [sp, #0x5c]
0052b038: str      fp, [sp, #0x60]
0052b03c: str      sl, [sp, #0x64]
0052b040: str      sl, [sp, #0x68]
0052b044: bl       #0x529a30
0052b048: ldr      r4, [sp, #0xc0]
0052b04c: mov      r0, r5
0052b050: ldr      r5, [r5]
0052b054: bl       #0x52aa84
0052b058: ldr      r2, [r4, #0x14]
0052b05c: mov      r3, r0
0052b060: mov      r0, sb
0052b064: str      r2, [r3, #8]
0052b068: ldr      r2, [r5, #4]
0052b06c: str      r5, [r3]
0052b070: str      r2, [r3, #4]
0052b074: str      r3, [r2]
0052b078: str      r3, [r5, #4]
0052b07c: ldr      r3, [sb]
0052b080: mov      lr, pc
0052b084: ldr      pc, [r3]
0052b088: ldr      ip, [sp, #0x38]
0052b08c: mov      lr, r0
0052b090: cmp      ip, #0
0052b094: moveq    ip, r7
0052b098: beq      #0x52b0dc
0052b09c: mov      r2, r7
0052b0a0: b        #0x52b0a8
0052b0a4: mov      ip, r3
0052b0a8: ldr      r3, [ip, #0x10]
0052b0ac: cmp      lr, r3
0052b0b0: ldrhi    r3, [ip, #0xc]
0052b0b4: ldrls    r3, [ip, #8]
0052b0b8: movhi    ip, r2
0052b0bc: mov      r2, ip
0052b0c0: cmp      r3, #0
0052b0c4: bne      #0x52b0a4
0052b0c8: cmp      ip, r7
0052b0cc: beq      #0x52b0dc
0052b0d0: ldr      r3, [ip, #0x10]
0052b0d4: cmp      lr, r3
0052b0d8: bhs      #0x52b108
0052b0dc: ldr      r0, [sp, #0x24]
0052b0e0: mov      r1, r7
0052b0e4: ldr      r2, [sp, #0x28]
0052b0e8: ldr      r3, [sp, #0x2c]
0052b0ec: str      ip, [sp, #0xbc]
0052b0f0: str      lr, [sp, #0x4c]
0052b0f4: str      fp, [sp, #0x50]
0052b0f8: str      sl, [sp, #0x54]
0052b0fc: str      sl, [sp, #0x58]
0052b100: bl       #0x529a30
0052b104: ldr      ip, [sp, #0xb8]
0052b108: ldr      r3, [ip, #0x14]
0052b10c: mov      r0, r3
0052b110: ldr      r3, [r3]
0052b114: mov      lr, pc
0052b118: ldr      pc, [r3, #4]
0052b11c: mov      sb, r0
0052b120: b        #0x52af88
0052b124: ldr      r2, [sp, #0xc]
0052b128: cmp      r2, #0
0052b12c: beq      #0x52af28
0052b130: ldr      r3, [r5, #0x18]
0052b134: mov      r0, sb
0052b138: ldr      fp, [r5, #4]
0052b13c: add      r3, r3, #1
0052b140: str      r3, [r5, #0x18]
0052b144: ldr      r3, [sb]
0052b148: mov      lr, pc
0052b14c: ldr      pc, [r3]
0052b150: mov      r2, r4
0052b154: mov      r1, r0
0052b158: mov      r0, fp
0052b15c: bl       #0x52ac30
0052b160: ldr      r2, [sp, #0xb0]
0052b164: cmp      r2, r4
0052b168: mov      r3, r2
0052b16c: beq      #0x52b268
0052b170: ldr      r3, [r3]
0052b174: cmp      r3, r4
0052b178: bne      #0x52b170
0052b17c: ldr      r3, [r5, #0x1c]
0052b180: add      r3, r3, #1
0052b184: str      r3, [r5, #0x1c]
0052b188: ldr      r3, [r2, #8]
0052b18c: ldr      r2, [r6]
0052b190: mov      r0, r3
0052b194: ldr      r3, [r3]
0052b198: ldr      fp, [r2]
0052b19c: mov      lr, pc
0052b1a0: ldr      pc, [r3, #0xc]
0052b1a4: mov      r1, r0
0052b1a8: mov      r0, r6
0052b1ac: blx      fp
0052b1b0: cmp      r0, #0
0052b1b4: beq      #0x52b31c
0052b1b8: ldr      r3, [sp, #0xb0]
0052b1bc: ldr      r2, [r6]
0052b1c0: ldr      r3, [r3, #8]
0052b1c4: ldr      fp, [r2]
0052b1c8: mov      r0, r3
0052b1cc: ldr      r3, [r3]
0052b1d0: mov      lr, pc
0052b1d4: ldr      pc, [r3, #0xc]
0052b1d8: mov      r1, r0
0052b1dc: mov      r0, r6
0052b1e0: blx      fp
0052b1e4: ldr      r2, [r5, #0x20]
0052b1e8: ldr      r3, [sp, #0xb0]
0052b1ec: add      r2, r2, #1
0052b1f0: str      r2, [r5, #0x20]
0052b1f4: ldr      fp, [r3, #8]
0052b1f8: ldr      r3, [fp]
0052b1fc: mov      r0, fp
0052b200: mov      lr, pc
0052b204: ldr      pc, [r3, #0x10]
0052b208: mov      r1, sl
0052b20c: bl       #0x30eba4
0052b210: mov      r1, #0
0052b214: str      fp, [sp, #0xa4]
0052b218: str      r0, [sp, #0xa8]
0052b21c: bl       #0x30eba4
0052b220: mov      r1, r7
0052b224: str      r0, [sp, #0xac]
0052b228: mov      r2, r8
0052b22c: mov      r0, r5
0052b230: bl       #0x529da4
0052b234: cmp      r0, #0
0052b238: bne      #0x52b374
0052b23c: ldr      r0, [sp, #0xb0]
0052b240: mov      r1, #0xc
0052b244: ldr      r3, [r0]
0052b248: ldr      r2, [r0, #4]
0052b24c: str      r3, [r2]
0052b250: str      r2, [r3, #4]
0052b254: bl       #0x708f00
0052b258: ldr      r2, [sp, #0xb0]
0052b25c: cmp      r2, r4
0052b260: mov      r3, r2
0052b264: bne      #0x52b170
0052b268: ldr      r2, [sp, #0xc]
0052b26c: subs     r2, r2, #1
0052b270: str      r2, [sp, #0xc]
0052b274: beq      #0x52af28
0052b278: ldr      r2, [sp, #0x7c]
0052b27c: ldr      r3, [sp, #0x80]
0052b280: rsb      r3, r2, r3
0052b284: asr      r3, r3, #2
0052b288: add      r1, r3, r3, lsl #2
0052b28c: add      r1, r1, r1, lsl #4
0052b290: add      r1, r1, r1, lsl #8
0052b294: add      r1, r1, r1, lsl #16
0052b298: add      r3, r3, r1, lsl #1
0052b29c: cmp      r3, #0
0052b2a0: beq      #0x52af28
0052b2a4: ldr      r3, [r2]
0052b2a8: mov      r0, r3
0052b2ac: ldr      r3, [r3]
0052b2b0: mov      lr, pc
0052b2b4: ldr      pc, [r3, #0xc]
0052b2b8: ldr      r3, [sp, #0x80]
0052b2bc: mov      sb, r0
0052b2c0: ldr      r0, [sp, #0x7c]
0052b2c4: ldr      r2, [r3, #-0xc]
0052b2c8: sub      r3, r3, #0xc
0052b2cc: ldr      sl, [r0, #4]
0052b2d0: str      r2, [sp, #0x8c]
0052b2d4: ldr      r2, [r3, #4]
0052b2d8: mov      r1, r3
0052b2dc: str      r2, [sp, #0x90]
0052b2e0: ldr      ip, [r3, #8]
0052b2e4: mov      r2, r3
0052b2e8: ldr      r3, [sp, #0x18]
0052b2ec: str      ip, [sp, #0x94]
0052b2f0: mov      ip, #0
0052b2f4: strb     ip, [sp]
0052b2f8: mov      ip, #0
0052b2fc: str      ip, [sp, #4]
0052b300: bl       #0x52943c
0052b304: ldr      r3, [sp, #0x80]
0052b308: cmp      sb, #0
0052b30c: sub      r3, r3, #0xc
0052b310: str      r3, [sp, #0x80]
0052b314: bne      #0x52af0c
0052b318: b        #0x52af28
0052b31c: ldr      r2, [sp, #0xb0]
0052b320: ldr      r3, [r6]
0052b324: mov      r0, r6
0052b328: ldr      r1, [r2, #8]
0052b32c: mov      lr, pc
0052b330: ldr      pc, [r3, #4]
0052b334: cmp      r0, #0
0052b338: beq      #0x52b23c
0052b33c: ldr      r3, [sp, #0xb0]
0052b340: ldr      r2, [r6]
0052b344: ldr      r3, [r3, #8]
0052b348: ldr      fp, [r2, #8]
0052b34c: mov      r0, r3
0052b350: ldr      r3, [r3]
0052b354: mov      lr, pc
0052b358: ldr      pc, [r3, #0xc]
0052b35c: mov      r1, r0
0052b360: mov      r0, r6
0052b364: blx      fp
0052b368: cmp      r0, #0
0052b36c: beq      #0x52b23c
0052b370: b        #0x52b1b8
0052b374: ldr      r3, [sp, #0xb0]
0052b378: ldr      r2, [r6]
0052b37c: ldr      r3, [r3, #8]
0052b380: ldr      fp, [r2]
0052b384: mov      r0, r3
0052b388: ldr      r3, [r3]
0052b38c: mov      lr, pc
0052b390: ldr      pc, [r3, #0xc]
0052b394: mov      r1, r0
0052b398: mov      r0, r6
0052b39c: blx      fp
0052b3a0: cmp      r0, #0
0052b3a4: bne      #0x52b3fc
0052b3a8: ldr      r3, [r5, #0x24]
0052b3ac: ldr      r0, [sp, #0x14]
0052b3b0: mov      r1, r8
0052b3b4: add      r3, r3, #1
0052b3b8: str      r3, [r5, #0x24]
0052b3bc: bl       #0x52a890
0052b3c0: b        #0x52b23c
0052b3c4: mov      r5, r6
0052b3c8: mov      r4, r8
0052b3cc: ldr      r3, [sp, #0x44]
0052b3d0: cmp      r3, #0
0052b3d4: bne      #0x52b518
0052b3d8: ldr      r0, [sp, #0x14]
0052b3dc: bl       #0x52a640
0052b3e0: mov      r0, r4
0052b3e4: bl       #0x529f4c
0052b3e8: ldr      r2, [sp, #0x1c]
0052b3ec: str      r2, [r5, #0x10]
0052b3f0: ldrb     r0, [r5, #0x14]
0052b3f4: add      sp, sp, #0xd4
0052b3f8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0052b3fc: ldr      r0, [sp, #0x7c]
0052b400: ldr      r3, [sp, #0x80]
0052b404: cmp      r0, r3
0052b408: beq      #0x52b464
0052b40c: ldr      sl, [sp, #0x20]
0052b410: ldr      r2, [r3, #-0xc]
0052b414: sub      r3, r3, #0xc
0052b418: mov      r1, r3
0052b41c: str      r2, [sp, #0x98]
0052b420: ldr      r2, [r3, #4]
0052b424: str      r2, [sp, #0x9c]
0052b428: ldr      ip, [r3, #8]
0052b42c: mov      r2, r3
0052b430: mov      r3, sl
0052b434: str      ip, [sp, #0xa0]
0052b438: mov      ip, #0
0052b43c: strb     ip, [sp]
0052b440: mov      ip, #0
0052b444: str      ip, [sp, #4]
0052b448: bl       #0x52943c
0052b44c: ldr      r3, [sp, #0x80]
0052b450: ldr      r0, [sp, #0x7c]
0052b454: sub      r3, r3, #0xc
0052b458: cmp      r3, r0
0052b45c: str      r3, [sp, #0x80]
0052b460: bne      #0x52b410
0052b464: ldr      r0, [sp, #0x14]
0052b468: mov      r1, r8
0052b46c: bl       #0x52a890
0052b470: b        #0x52b268
0052b474: ldr      r6, [sp, #0x3c]
0052b478: cmp      r6, r7
0052b47c: beq      #0x52b3cc
0052b480: ldr      r3, [r6, #0x14]
0052b484: cmp      r3, #0
0052b488: beq      #0x52b4b8
0052b48c: ldr      r3, [r5, #0x10]
0052b490: mov      r0, r3
0052b494: ldr      r8, [r3]
0052b498: bl       #0x52aa84
0052b49c: ldr      r3, [r6, #0x14]
0052b4a0: str      r3, [r0, #8]
0052b4a4: ldr      r3, [r8, #4]
0052b4a8: str      r8, [r0]
0052b4ac: str      r3, [r0, #4]
0052b4b0: str      r0, [r3]
0052b4b4: str      r0, [r8, #4]
0052b4b8: ldr      r2, [r6, #0xc]
0052b4bc: cmp      r2, #0
0052b4c0: beq      #0x52b4dc
0052b4c4: mov      r6, r2
0052b4c8: ldr      r3, [r6, #8]
0052b4cc: cmp      r3, #0
0052b4d0: beq      #0x52b478
0052b4d4: mov      r6, r3
0052b4d8: b        #0x52b4c8
0052b4dc: ldr      r3, [r6, #4]
0052b4e0: ldr      r1, [r3, #0xc]
0052b4e4: cmp      r1, r6
0052b4e8: bne      #0x52b504
0052b4ec: mov      r6, r3
0052b4f0: ldr      r3, [r3, #4]
0052b4f4: ldr      r2, [r3, #0xc]
0052b4f8: cmp      r2, r6
0052b4fc: beq      #0x52b4ec
0052b500: ldr      r2, [r6, #0xc]
0052b504: cmp      r3, r2
0052b508: movne    r6, r3
0052b50c: b        #0x52b478
0052b510: mov      r2, r0
0052b514: b        #0x52adb8
0052b518: mov      r0, r7
0052b51c: ldr      r1, [sp, #0x38]
0052b520: bl       #0x52a06c
0052b524: mov      r3, #0
0052b528: str      r7, [sp, #0x40]
0052b52c: str      r3, [sp, #0x44]
0052b530: str      r7, [sp, #0x3c]
0052b534: str      r3, [sp, #0x38]
0052b538: b        #0x52b3d8
0052b53c: mov      r4, #0
0052b540: strb     r4, [r5, #0x14]
0052b544: add      r0, r5, #8
0052b548: bl       #0x529f4c
0052b54c: str      r4, [r5, #0x24]
0052b550: str      r4, [r5, #0x18]
0052b554: str      r4, [r5, #0x1c]
0052b558: str      r4, [r5, #0x20]
0052b55c: b        #0x52b3f0

# _ZN15SearchFailCache3addERNS_5EntryE
0052a504: push     {r4, r5, lr}
0052a508: ldr      r3, [r0, #4]
0052a50c: ldr      ip, [r0]
0052a510: mov      r5, r1
0052a514: sub      sp, sp, #0x1c
0052a518: rsb      r1, ip, r3
0052a51c: asr      r1, r1, #2
0052a520: mov      r4, r0
0052a524: add      r2, r1, r1, lsl #2
0052a528: add      r2, r2, r2, lsl #4
0052a52c: add      r2, r2, r2, lsl #8
0052a530: add      r2, r2, r2, lsl #16
0052a534: add      r2, r1, r2, lsl #1
0052a538: cmp      r2, #0xa
0052a53c: bls      #0x52a564
0052a540: mov      r1, #0
0052a544: cmp      r2, #0
0052a548: str      r1, [sp, #0x10]
0052a54c: str      r1, [sp, #8]
0052a550: str      r1, [sp, #0xc]
0052a554: beq      #0x52a5a4
0052a558: cmp      r3, ip
0052a55c: strne    ip, [r0, #4]
0052a560: movne    r3, ip
0052a564: ldr      r2, [r4, #8]
0052a568: cmp      r2, r3
0052a56c: beq      #0x52a5c0
0052a570: mov      r1, r5
0052a574: ldr      r0, [r1], #4
0052a578: mov      r2, r3
0052a57c: str      r0, [r2], #4
0052a580: ldr      r0, [r5, #4]
0052a584: str      r0, [r3, #4]
0052a588: ldr      r3, [r1, #4]
0052a58c: str      r3, [r2, #4]
0052a590: ldr      r3, [r4, #4]
0052a594: add      r3, r3, #0xc
0052a598: str      r3, [r4, #4]
0052a59c: add      sp, sp, #0x1c
0052a5a0: pop      {r4, r5, pc}
0052a5a4: mov      r1, r3
0052a5a8: add      r3, sp, #8
0052a5ac: bl       #0x52a498
0052a5b0: ldr      r3, [r4, #4]
0052a5b4: ldr      r2, [r4, #8]
0052a5b8: cmp      r2, r3
0052a5bc: bne      #0x52a570
0052a5c0: mov      ip, #1
0052a5c4: mov      r1, r3
0052a5c8: mov      r0, r4
0052a5cc: mov      r2, r5
0052a5d0: add      r3, sp, #0x14
0052a5d4: str      ip, [sp, #4]
0052a5d8: str      ip, [sp]
0052a5dc: bl       #0x52a23c
0052a5e0: b        #0x52a59c

# _ZNSt4priv8_Rb_treeIjSt4lessIjESt4pairIKjN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeEENS_10_Select1stISD_EENS_11_MapTraitsTISD_EESaISD_EE9_M_insertEPNS_18_Rb_tree_node_baseERKSD_SL_SL_
00529754: cmp      r1, r2
00529758: push     {r4, r5, r6, r7, r8, lr}
0052975c: mov      r4, r1
00529760: mov      r7, r2
00529764: mov      r5, r0
00529768: mov      r8, r3
0052976c: beq      #0x52984c
00529770: ldr      r3, [sp, #0x1c]
00529774: cmp      r3, #0
00529778: beq      #0x5297ec
0052977c: mov      r0, r4
00529780: bl       #0x529734
00529784: ldr      r2, [r8]
00529788: mov      r3, #0
0052978c: mov      r6, r0
00529790: str      r2, [r0, #0x10]
00529794: ldr      r2, [r8, #4]
00529798: str      r2, [r0, #0x14]
0052979c: ldr      r2, [r8, #8]
005297a0: str      r2, [r0, #0x18]
005297a4: ldr      r2, [r8, #0xc]
005297a8: str      r3, [r0, #0xc]
005297ac: str      r3, [r0, #8]
005297b0: str      r2, [r0, #0x1c]
005297b4: str      r0, [r7, #0xc]
005297b8: ldr      r3, [r4, #0xc]
005297bc: cmp      r7, r3
005297c0: beq      #0x529844
005297c4: mov      r0, r6
005297c8: str      r7, [r6, #4]
005297cc: add      r1, r4, #4
005297d0: bl       #0x313760
005297d4: ldr      r3, [r4, #0x10]
005297d8: mov      r0, r5
005297dc: add      r3, r3, #1
005297e0: str      r3, [r4, #0x10]
005297e4: str      r6, [r5]
005297e8: pop      {r4, r5, r6, r7, r8, pc}
005297ec: ldr      r3, [sp, #0x18]
005297f0: cmp      r3, #0
005297f4: beq      #0x529894
005297f8: mov      r0, r4
005297fc: bl       #0x529734
00529800: ldr      r2, [r8]
00529804: mov      r3, #0
00529808: mov      r6, r0
0052980c: str      r2, [r0, #0x10]
00529810: ldr      r2, [r8, #4]
00529814: str      r2, [r0, #0x14]
00529818: ldr      r2, [r8, #8]
0052981c: str      r2, [r0, #0x18]
00529820: ldr      r2, [r8, #0xc]
00529824: str      r3, [r0, #0xc]
00529828: str      r3, [r0, #8]
0052982c: str      r2, [r0, #0x1c]
00529830: str      r0, [r7, #8]
00529834: ldr      r3, [r4, #8]
00529838: cmp      r7, r3
0052983c: streq    r0, [r4, #8]
00529840: b        #0x5297c4
00529844: str      r6, [r4, #0xc]
00529848: b        #0x5297c4
0052984c: mov      r0, r1
00529850: bl       #0x529734
00529854: ldr      r2, [r8]
00529858: mov      r3, #0
0052985c: mov      r6, r0
00529860: str      r2, [r0, #0x10]
00529864: ldr      r2, [r8, #4]
00529868: str      r2, [r0, #0x14]
0052986c: ldr      r2, [r8, #8]
00529870: str      r2, [r0, #0x18]
00529874: ldr      r2, [r8, #0xc]
00529878: str      r3, [r0, #0xc]
0052987c: str      r3, [r0, #8]
00529880: str      r2, [r0, #0x1c]
00529884: str      r0, [r4, #8]
00529888: str      r0, [r4, #4]
0052988c: str      r0, [r4, #0xc]
00529890: b        #0x5297c4
00529894: ldr      r2, [r8]
00529898: ldr      r3, [r7, #0x10]
0052989c: cmp      r2, r3
005298a0: bhs      #0x52977c
005298a4: b        #0x5297f8

# _ZN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE9_markNodeERSt3mapIjNS5_7_InEdgeESt4lessIjESaISt4pairIKjS7_EEERS7_
00529da4: push     {r4, r5, r6, lr}
00529da8: ldr      r3, [r2]
00529dac: sub      sp, sp, #0x18
00529db0: mov      r6, r1
00529db4: mov      r0, r3
00529db8: ldr      r3, [r3]
00529dbc: mov      r5, r2
00529dc0: mov      lr, pc
00529dc4: ldr      pc, [r3, #0xc]
00529dc8: ldr      r3, [r0]
00529dcc: mov      r4, r0
00529dd0: mov      lr, pc
00529dd4: ldr      pc, [r3]
00529dd8: ldr      r3, [r6, #4]
00529ddc: cmp      r3, #0
00529de0: beq      #0x529f04
00529de4: mov      r1, r6
00529de8: b        #0x529df0
00529dec: mov      r3, r2
00529df0: ldr      r2, [r3, #0x10]
00529df4: cmp      r0, r2
00529df8: ldrhi    r2, [r3, #0xc]
00529dfc: ldrls    r2, [r3, #8]
00529e00: movhi    r3, r1
00529e04: mov      r1, r3
00529e08: cmp      r2, #0
00529e0c: bne      #0x529dec
00529e10: cmp      r6, r3
00529e14: beq      #0x529e44
00529e18: ldr      r2, [r3, #0x10]
00529e1c: cmp      r0, r2
00529e20: blo      #0x529f04
00529e24: cmp      r6, r3
00529e28: beq      #0x529e44
00529e2c: ldr      r0, [r3, #0x18]
00529e30: ldr      r1, [r5, #4]
00529e34: bl       #0x30e9ac
00529e38: cmp      r0, #0
00529e3c: movne    r0, #0
00529e40: bne      #0x529efc
00529e44: mov      r0, r4
00529e48: ldr      r3, [r4]
00529e4c: mov      lr, pc
00529e50: ldr      pc, [r3]
00529e54: ldr      ip, [r6, #4]
00529e58: mov      r4, r0
00529e5c: cmp      ip, #0
00529e60: movne    r2, r6
00529e64: bne      #0x529e70
00529e68: b        #0x529f0c
00529e6c: mov      ip, r3
00529e70: ldr      r3, [ip, #0x10]
00529e74: cmp      r4, r3
00529e78: ldrhi    r3, [ip, #0xc]
00529e7c: ldrls    r3, [ip, #8]
00529e80: movhi    ip, r2
00529e84: mov      r2, ip
00529e88: cmp      r3, #0
00529e8c: bne      #0x529e6c
00529e90: cmp      r6, ip
00529e94: beq      #0x529ea8
00529e98: ldr      r2, [ip, #0x10]
00529e9c: mov      r3, ip
00529ea0: cmp      r4, r2
00529ea4: bhs      #0x529edc
00529ea8: mov      lr, #0
00529eac: mov      r3, sp
00529eb0: str      r4, [sp]
00529eb4: mov      r1, r6
00529eb8: add      r0, sp, #0x10
00529ebc: add      r2, sp, #0x14
00529ec0: mov      r4, #0
00529ec4: str      r4, [sp, #4]
00529ec8: str      lr, [sp, #0xc]
00529ecc: str      ip, [sp, #0x14]
00529ed0: str      lr, [sp, #8]
00529ed4: bl       #0x529a30
00529ed8: ldr      r3, [sp, #0x10]
00529edc: mov      r2, r5
00529ee0: ldr      r1, [r2], #4
00529ee4: mov      r0, #1
00529ee8: str      r1, [r3, #0x14]
00529eec: ldr      r1, [r5, #4]
00529ef0: str      r1, [r3, #0x18]
00529ef4: ldr      r2, [r2, #4]
00529ef8: str      r2, [r3, #0x1c]
00529efc: add      sp, sp, #0x18
00529f00: pop      {r4, r5, r6, pc}
00529f04: mov      r3, r6
00529f08: b        #0x529e24
00529f0c: mov      ip, r6
00529f10: b        #0x529e90

# _ZNK3sfc4math5graph5ITestI12PFGInnerEdge12PFGInnerNodeE7isValidEPKS4_
005250d4: push     {r4, lr}
005250d8: mov      r0, r1
005250dc: ldr      r3, [r1]
005250e0: mov      lr, pc
005250e4: ldr      pc, [r3, #4]
005250e8: pop      {r4, pc}

# _ZNSt4priv10_List_baseIPK12PFGInnerEdgeSaIS3_EE5clearEv
00529f4c: push     {r4, r5, r6, lr}
00529f50: mov      r5, r0
00529f54: ldr      r0, [r0]
00529f58: cmp      r0, r5
00529f5c: bne      #0x529f68
00529f60: b        #0x529f80
00529f64: mov      r0, r4
00529f68: ldr      r4, [r0]
00529f6c: mov      r1, #0xc
00529f70: bl       #0x708f00
00529f74: cmp      r4, r5
00529f78: bne      #0x529f64
00529f7c: mov      r0, r5
00529f80: str      r0, [r5, #4]
00529f84: str      r0, [r5]
00529f88: pop      {r4, r5, r6, pc}

# _ZNK20PFInnerTest_PFObject7isValidEPK12PFGInnerEdge
00525080: push     {r4, r5, r6, lr}
00525084: mov      r5, r0
00525088: mov      r4, r1
0052508c: bl       #0x52503c
00525090: cmp      r0, #0
00525094: beq      #0x5250b8
00525098: ldr      r3, [r5, #4]
0052509c: ldr      r0, [r4, #0x14]
005250a0: mov      r4, #0
005250a4: ldr      r1, [r3, #8]
005250a8: bl       #0x30e4b4
005250ac: cmp      r0, #0
005250b0: movne    r4, #1
005250b4: uxtb     r0, r4
005250b8: pop      {r4, r5, r6, pc}
