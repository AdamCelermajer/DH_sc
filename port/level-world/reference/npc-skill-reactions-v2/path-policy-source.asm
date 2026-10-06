_ZN7PFWorld8FindPathER8PFObjectRK7Point3DIfEj
0052db48 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0052db4c ldr      r6, [pc, #0x244]
0052db50 ldr      sb, [pc, #0x244]
0052db54 mov      r4, r0
0052db58 add      r6, pc, r6
0052db5c ldr      ip, [r6, sb]
0052db60 ldr      r0, [pc, #0x238]
0052db64 mov      r5, r1
0052db68 ldr      r1, [ip]
0052db6c sub      sp, sp, #0x44
0052db70 add      r0, pc, r0
0052db74 mov      r7, r2
0052db78 mov      fp, r3
0052db7c str      r1, [sp, #0x3c]
0052db80 bl       #0x3136b4 ; _Z20PushProfilingContextPKc
0052db84 mov      r1, r5
0052db88 mov      r0, r4
0052db8c bl       #0x52aae4 ; _ZN7PFWorld8DropPathER8PFObject
0052db90 ldr      r3, [r7]
0052db94 ldr      r2, [pc, #0x208]
0052db98 add      r8, sp, #0x24
0052db9c str      r3, [r5, #0x40]
0052dba0 ldr      r3, [r7, #4]
0052dba4 ldr      sl, [r6, r2]
0052dba8 str      r3, [r5, #0x44]
0052dbac ldr      r3, [r7, #8]
0052dbb0 mov      r0, sl
0052dbb4 str      r3, [r5, #0x48]
0052dbb8 bl       #0x337888 ; _ZN13DebugSwitches4loadEv
0052dbbc ldr      r1, [pc, #0x1e4]
0052dbc0 add      r2, sp, #0x20
0052dbc4 mov      r0, r8
0052dbc8 add      r1, pc, r1
0052dbcc bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
0052dbd0 mov      r0, sl
0052dbd4 mov      r1, r8
0052dbd8 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
0052dbdc mov      sl, r0
0052dbe0 ldr      r0, [sp, #0x38]
0052dbe4 cmp      r0, r8
0052dbe8 beq      #0x52dc08
0052dbec cmp      r0, #0
0052dbf0 beq      #0x52dc08
0052dbf4 ldr      r1, [sp, #0x24]
0052dbf8 rsb      r1, r0, r1
0052dbfc cmp      r1, #0x80
0052dc00 bhi      #0x52dd78
0052dc04 bl       #0x708f00 ; ___ZNSt12__node_alloc13_M_deallocateEPvj_veneer
0052dc08 cmp      sl, #0
0052dc0c beq      #0x52dd04
0052dc10 bl       #0x60b0cc ; _ZN6glitch2os5Timer11getRealTimeEv
0052dc14 mov      r3, r7
0052dc18 mov      r1, r5
0052dc1c add      r2, r5, #0x18
0052dc20 add      ip, r5, #0x38
0052dc24 str      r0, [sp, #0x1c]
0052dc28 mov      r0, r4
0052dc2c stm      sp, {fp, ip}
0052dc30 bl       #0x52b560 ; _ZN7PFWorld12_SearchGraphEPK8PFObjectRK7Point3DIfES6_jPSt4listIPK12PFGInnerEdgeSaISA_EE
0052dc34 mov      r7, r0
0052dc38 bl       #0x60b0cc ; _ZN6glitch2os5Timer11getRealTimeEv
0052dc3c ldr      r2, [r4, #0x64]
0052dc40 ldr      r3, [r4, #0x5c]
0052dc44 ldr      r1, [sp, #0x1c]
0052dc48 sub      r2, r2, #4
0052dc4c cmp      r3, r2
0052dc50 rsb      r1, r1, r0
0052dc54 str      r1, [sp, #0x1c]
0052dc58 beq      #0x52dd80
0052dc5c str      r1, [r3]
0052dc60 ldr      r3, [r4, #0x5c]
0052dc64 add      r8, r4, #0x4c
0052dc68 add      r3, r3, #4
0052dc6c str      r3, [r4, #0x5c]
0052dc70 ldr      r2, [r4, #0x74]
0052dc74 ldr      r3, [sp, #0x1c]
0052dc78 add      ip, sp, #0xc
0052dc7c add      r3, r2, r3
0052dc80 str      r3, [r4, #0x74]
0052dc84 ldm      r8, {r0, r1, r2, r3}
0052dc88 stm      ip, {r0, r1, r2, r3}
0052dc8c mov      r1, ip
0052dc90 add      r0, r4, #0x5c
0052dc94 bl       #0x5224f8 ; _ZNKSt4priv20_Deque_iterator_baseIjE11_M_subtractERKS1_
0052dc98 cmp      r0, #0xa
0052dc9c bls      #0x52dd24
0052dca0 ldr      r3, [r4, #0x4c]
0052dca4 ldr      r0, [r4, #0x54]
0052dca8 ldr      r1, [r4, #0x74]
0052dcac ldr      r2, [r3]
0052dcb0 sub      r0, r0, #4
0052dcb4 cmp      r3, r0
0052dcb8 rsb      r2, r2, r1
0052dcbc addne    r3, r3, #4
0052dcc0 str      r2, [r4, #0x74]
0052dcc4 strne    r3, [r4, #0x4c]
0052dcc8 bne      #0x52dd24
0052dccc ldr      r0, [r4, #0x50]
0052dcd0 cmp      r0, #0
0052dcd4 beq      #0x52dce0
0052dcd8 mov      r1, #0x80
0052dcdc bl       #0x31bb44 ; _ZNSt12__node_alloc10deallocateEPvj
0052dce0 ldr      r3, [r4, #0x58]
0052dce4 add      r2, r3, #4
0052dce8 str      r2, [r4, #0x58]
0052dcec ldr      r3, [r3, #4]
0052dcf0 add      r2, r3, #0x80
0052dcf4 str      r2, [r4, #0x54]
0052dcf8 str      r3, [r4, #0x4c]
0052dcfc str      r3, [r4, #0x50]
0052dd00 b        #0x52dd24
0052dd04 mov      r3, r7
0052dd08 add      ip, r5, #0x38
0052dd0c mov      r0, r4
0052dd10 mov      r1, r5
0052dd14 add      r2, r5, #0x18
0052dd18 stm      sp, {fp, ip}
0052dd1c bl       #0x52b560 ; _ZN7PFWorld12_SearchGraphEPK8PFObjectRK7Point3DIfES6_jPSt4listIPK12PFGInnerEdgeSaISA_EE
0052dd20 mov      r7, r0
0052dd24 cmp      r7, #0
0052dd28 bne      #0x52dd58
0052dd2c ldr      r0, [pc, #0x78]
0052dd30 add      r0, pc, r0
0052dd34 bl       #0x3136b8 ; _Z19PopProfilingContextPKc
0052dd38 ldr      r3, [r6, sb]
0052dd3c ldr      r2, [sp, #0x3c]
0052dd40 mov      r0, r7
0052dd44 ldr      r3, [r3]
0052dd48 cmp      r2, r3
0052dd4c bne      #0x52dd94
0052dd50 add      sp, sp, #0x44
0052dd54 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0052dd58 mov      r0, r4
0052dd5c mov      r1, r5
0052dd60 bl       #0x52d538 ; _ZN7PFWorld11_SmoothPathER8PFObject
0052dd64 mov      r0, r4
0052dd68 mov      r1, r5
0052dd6c bl       #0x52899c ; _ZN7PFWorld16_CalcWaypointVecER8PFObject
0052dd70 mov      r7, #1
0052dd74 b        #0x52dd2c
0052dd78 bl       #0x310440 ; _Z10CustomFreePv
0052dd7c b        #0x52dc08
0052dd80 add      r8, r4, #0x4c
0052dd84 mov      r0, r8
0052dd88 add      r1, sp, #0x1c
0052dd8c bl       #0x52d9b4 ; _ZNSt5dequeIjSaIjEE18_M_push_back_aux_vERKj
0052dd90 b        #0x52dc70
0052dd94 bl       #0x30e310
0052dd98 subeq    r6, r6, r8, lsr pc
0052dd9c andeq    r4, r0, ip, lsr #1
0052dda0 eorseq   pc, sl, r8, rrx
0052dda4 andeq    r0, r0, r4, lsl #17
0052dda8 eorseq   pc, sl, r8, lsr #32
0052ddac eorseq   lr, sl, r8, lsr #29
_ZN7PFWorld12_SearchGraphEPK8PFObjectRK7Point3DIfES6_jPSt4listIPK12PFGInnerEdgeSaISA_EE
0052b560 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0052b564 ldr      r4, [pc, #0xdc4]
0052b568 ldr      r5, [pc, #0xdc4]
0052b56c ldr      lr, [pc, #0xdc4]
0052b570 add      r4, pc, r4
0052b574 ldr      ip, [r4, r5]
0052b578 ldr      r6, [r4, lr]
0052b57c sub      sp, sp, #0x164
0052b580 ldr      ip, [ip]
0052b584 mov      r8, r0
0052b588 mov      r0, r6
0052b58c mov      sb, r3
0052b590 str      ip, [sp, #0x15c]
0052b594 str      r1, [sp, #0x20]
0052b598 mov      r7, r2
0052b59c ldr      fp, [sp, #0x18c]
0052b5a0 bl       #0x337888 ; _ZN13DebugSwitches4loadEv
0052b5a4 ldr      r1, [pc, #0xd90]
0052b5a8 add      sl, sp, #0x144
0052b5ac add      r2, sp, #0x140
0052b5b0 add      r1, pc, r1
0052b5b4 mov      r0, sl
0052b5b8 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
0052b5bc mov      r0, r6
0052b5c0 mov      r1, sl
0052b5c4 bl       #0x337ec8 ; _ZN13DebugSwitches9GetModuleERKSs
0052b5c8 mov      r6, r0
0052b5cc ldr      r0, [sp, #0x158]
0052b5d0 cmp      r0, sl
0052b5d4 beq      #0x52b5f4
0052b5d8 cmp      r0, #0
0052b5dc beq      #0x52b5f4
0052b5e0 ldr      r1, [sp, #0x144]
0052b5e4 rsb      r1, r0, r1
0052b5e8 cmp      r1, #0x80
0052b5ec bhi      #0x52bee8
0052b5f0 bl       #0x708f00 ; ___ZNSt12__node_alloc13_M_deallocateEPvj_veneer
0052b5f4 cmp      r6, #0
0052b5f8 beq      #0x52b690
0052b5fc mov      ip, #0
0052b600 mov      r6, #0
0052b604 add      lr, sp, #0x13c
0052b608 mov      r0, r8
0052b60c mov      r1, r7
0052b610 add      r2, sp, #0x128
0052b614 add      r3, sp, #0x88
0052b618 str      ip, [sp, #0x124]
0052b61c str      lr, [sp, #4]
0052b620 str      ip, [sp, #0x88]
0052b624 str      ip, [sp, #0x8c]
0052b628 str      ip, [sp, #0x90]
0052b62c str      ip, [sp, #0x94]
0052b630 str      ip, [sp, #0x98]
0052b634 str      ip, [sp, #0x9c]
0052b638 str      ip, [sp, #0xa0]
0052b63c str      ip, [sp, #0xa4]
0052b640 str      ip, [sp, #0xa8]
0052b644 str      ip, [sp, #0x64]
0052b648 str      ip, [sp, #0x68]
0052b64c str      ip, [sp, #0x6c]
0052b650 str      ip, [sp, #0x70]
0052b654 str      ip, [sp, #0x74]
0052b658 str      ip, [sp, #0x78]
0052b65c str      ip, [sp, #0x7c]
0052b660 str      ip, [sp, #0x80]
0052b664 str      ip, [sp, #0x84]
0052b668 str      ip, [sp, #0x128]
0052b66c str      ip, [sp, #0x12c]
0052b670 str      ip, [sp, #0x130]
0052b674 str      ip, [sp, #0x11c]
0052b678 str      ip, [sp, #0x120]
0052b67c str      r6, [sp]
0052b680 str      r6, [sp, #8]
0052b684 bl       #0x5256d4 ; _ZN7PFWorld14GetCollisionAtERK7Point3DIfERS1_RN6glitch4core10triangle3dIfEEPP6PFRoomPP7PFFloorb
0052b688 cmp      r0, r6
0052b68c bne      #0x52b6b4
0052b690 mov      r6, #0
0052b694 ldr      r3, [r4, r5]
0052b698 ldr      r2, [sp, #0x15c]
0052b69c mov      r0, r6
0052b6a0 ldr      r3, [r3]
0052b6a4 cmp      r2, r3
0052b6a8 bne      #0x52c32c
0052b6ac add      sp, sp, #0x164
0052b6b0 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0052b6b4 ldr      r3, [sp, #0x13c]
0052b6b8 cmp      r3, r6
0052b6bc beq      #0x52b690
0052b6c0 add      ip, sp, #0x138
0052b6c4 mov      r0, r8
0052b6c8 mov      r1, sb
0052b6cc add      r2, sp, #0x11c
0052b6d0 add      r3, sp, #0x64
0052b6d4 str      ip, [sp, #4]
0052b6d8 str      r6, [sp, #8]
0052b6dc str      r6, [sp]
0052b6e0 bl       #0x5256d4 ; _ZN7PFWorld14GetCollisionAtERK7Point3DIfERS1_RN6glitch4core10triangle3dIfEEPP6PFRoomPP7PFFloorb
0052b6e4 cmp      r0, #0
0052b6e8 beq      #0x52b690
0052b6ec ldr      r3, [sp, #0x138]
0052b6f0 cmp      r3, #0
0052b6f4 beq      #0x52b690
0052b6f8 ldr      r6, [sp, #0x88]
0052b6fc ldr      r0, [sp, #0x64]
0052b700 mov      r1, r6
0052b704 bl       #0x30df8c
0052b708 cmp      r0, #0
0052b70c bne      #0x52bef0
0052b710 ldr      ip, [sp, #0x98]
0052b714 ldr      lr, [sp, #0x90]
0052b718 ldr      r3, [sp, #0x94]
0052b71c ldr      sl, [sp, #0x8c]
0052b720 str      ip, [sp, #0x24]
0052b724 str      lr, [sp, #0x2c]
0052b728 ldr      r2, [sp, #0x9c]
0052b72c str      r2, [sp, #0x28]
0052b730 mov      r1, r3
0052b734 mov      r0, r6
0052b738 bl       #0x30eba4
0052b73c mov      r1, #0x3f000000
0052b740 bl       #0x30ed6c
0052b744 mov      r1, sl
0052b748 str      r0, [sp, #0x110]
0052b74c ldr      r0, [sp, #0x24]
0052b750 bl       #0x30eba4
0052b754 mov      r1, #0x3f000000
0052b758 bl       #0x30ed6c
0052b75c ldr      r1, [sp, #0x28]
0052b760 str      r0, [sp, #0x114]
0052b764 ldr      r0, [sp, #0x2c]
0052b768 bl       #0x30eba4
0052b76c mov      r1, #0x3f000000
0052b770 bl       #0x30ed6c
0052b774 add      r1, sp, #0x110
0052b778 str      r0, [sp, #0x118]
0052b77c ldr      r0, [sp, #0x13c]
0052b780 bl       #0x51c0dc ; _ZN7PFFloor10_GetNodeAtERK7Point3DIfE
0052b784 ldr      r1, [sp, #0xa4]
0052b788 mov      r6, r0
0052b78c ldr      r0, [sp, #0x8c]
0052b790 bl       #0x30eba4
0052b794 mov      r1, #0x3f000000
0052b798 bl       #0x30ed6c
0052b79c ldr      r1, [sp, #0xa8]
0052b7a0 mov      sl, r0
0052b7a4 ldr      r0, [sp, #0x90]
0052b7a8 bl       #0x30eba4
0052b7ac mov      r1, #0x3f000000
0052b7b0 bl       #0x30ed6c
0052b7b4 ldr      r1, [sp, #0xa0]
0052b7b8 mov      r3, r0
0052b7bc ldr      r0, [sp, #0x88]
0052b7c0 str      r3, [sp, #0x14]
0052b7c4 bl       #0x30eba4
0052b7c8 mov      r1, #0x3f000000
0052b7cc bl       #0x30ed6c
0052b7d0 ldr      r3, [sp, #0x14]
0052b7d4 str      r0, [sp, #0x104]
0052b7d8 add      r1, sp, #0x104
0052b7dc ldr      r0, [sp, #0x13c]
0052b7e0 str      r3, [sp, #0x10c]
0052b7e4 str      sl, [sp, #0x108]
0052b7e8 bl       #0x51c0dc ; _ZN7PFFloor10_GetNodeAtERK7Point3DIfE
0052b7ec ldr      r1, [sp, #0xa4]
0052b7f0 str      r0, [sp, #0x30]
0052b7f4 ldr      r0, [sp, #0x98]
0052b7f8 bl       #0x30eba4
0052b7fc mov      r1, #0x3f000000
0052b800 bl       #0x30ed6c
0052b804 ldr      r1, [sp, #0xa8]
0052b808 mov      sl, r0
0052b80c ldr      r0, [sp, #0x9c]
0052b810 bl       #0x30eba4
0052b814 mov      r1, #0x3f000000
0052b818 bl       #0x30ed6c
0052b81c ldr      r1, [sp, #0xa0]
0052b820 mov      r3, r0
0052b824 ldr      r0, [sp, #0x94]
0052b828 str      r3, [sp, #0x14]
0052b82c bl       #0x30eba4
0052b830 mov      r1, #0x3f000000
0052b834 bl       #0x30ed6c
0052b838 ldr      r3, [sp, #0x14]
0052b83c str      r0, [sp, #0xf8]
0052b840 add      r1, sp, #0xf8
0052b844 ldr      r0, [sp, #0x13c]
0052b848 str      r3, [sp, #0x100]
0052b84c str      sl, [sp, #0xfc]
0052b850 bl       #0x51c0dc ; _ZN7PFFloor10_GetNodeAtERK7Point3DIfE
0052b854 ldr      r1, [sp, #0x74]
0052b858 mov      sl, r0
0052b85c ldr      r0, [sp, #0x68]
0052b860 bl       #0x30eba4
0052b864 mov      r1, #0x3f000000
0052b868 bl       #0x30ed6c
0052b86c ldr      r1, [sp, #0x78]
0052b870 mov      r3, r0
0052b874 ldr      r0, [sp, #0x6c]
0052b878 str      r3, [sp, #0x14]
0052b87c bl       #0x30eba4
0052b880 mov      r1, #0x3f000000
0052b884 bl       #0x30ed6c
0052b888 ldr      r1, [sp, #0x70]
0052b88c mov      r2, r0
0052b890 ldr      r0, [sp, #0x64]
0052b894 str      r2, [sp, #0x18]
0052b898 bl       #0x30eba4
0052b89c mov      r1, #0x3f000000
0052b8a0 bl       #0x30ed6c
0052b8a4 ldr      r2, [sp, #0x18]
0052b8a8 ldr      r3, [sp, #0x14]
0052b8ac str      r0, [sp, #0xec]
0052b8b0 add      r1, sp, #0xec
0052b8b4 ldr      r0, [sp, #0x138]
0052b8b8 str      r2, [sp, #0xf4]
0052b8bc str      r3, [sp, #0xf0]
0052b8c0 bl       #0x51c0dc ; _ZN7PFFloor10_GetNodeAtERK7Point3DIfE
0052b8c4 ldr      r1, [sp, #0x80]
0052b8c8 str      r0, [sp, #0x2c]
0052b8cc ldr      r0, [sp, #0x68]
0052b8d0 bl       #0x30eba4
0052b8d4 mov      r1, #0x3f000000
0052b8d8 bl       #0x30ed6c
0052b8dc ldr      r1, [sp, #0x84]
0052b8e0 mov      r3, r0
0052b8e4 ldr      r0, [sp, #0x6c]
0052b8e8 str      r3, [sp, #0x14]
0052b8ec bl       #0x30eba4
0052b8f0 mov      r1, #0x3f000000
0052b8f4 bl       #0x30ed6c
0052b8f8 ldr      r1, [sp, #0x7c]
0052b8fc mov      r2, r0
0052b900 ldr      r0, [sp, #0x64]
0052b904 str      r2, [sp, #0x18]
0052b908 bl       #0x30eba4
0052b90c mov      r1, #0x3f000000
0052b910 bl       #0x30ed6c
0052b914 ldr      r2, [sp, #0x18]
0052b918 ldr      r3, [sp, #0x14]
0052b91c str      r0, [sp, #0xe0]
0052b920 add      r1, sp, #0xe0
0052b924 ldr      r0, [sp, #0x138]
0052b928 str      r2, [sp, #0xe8]
0052b92c str      r3, [sp, #0xe4]
0052b930 bl       #0x51c0dc ; _ZN7PFFloor10_GetNodeAtERK7Point3DIfE
0052b934 ldr      r1, [sp, #0x80]
0052b938 str      r0, [sp, #0x28]
0052b93c ldr      r0, [sp, #0x74]
0052b940 bl       #0x30eba4
0052b944 mov      r1, #0x3f000000
0052b948 bl       #0x30ed6c
0052b94c ldr      r1, [sp, #0x84]
0052b950 mov      r3, r0
0052b954 ldr      r0, [sp, #0x78]
0052b958 str      r3, [sp, #0x14]
0052b95c bl       #0x30eba4
0052b960 mov      r1, #0x3f000000
0052b964 bl       #0x30ed6c
0052b968 ldr      r1, [sp, #0x7c]
0052b96c mov      r2, r0
0052b970 ldr      r0, [sp, #0x70]
0052b974 str      r2, [sp, #0x18]
0052b978 bl       #0x30eba4
0052b97c mov      r1, #0x3f000000
0052b980 bl       #0x30ed6c
0052b984 ldr      r3, [sp, #0x14]
0052b988 ldr      r2, [sp, #0x18]
0052b98c str      r0, [sp, #0xd4]
0052b990 add      r1, sp, #0xd4
0052b994 ldr      r0, [sp, #0x138]
0052b998 str      r3, [sp, #0xd8]
0052b99c str      r2, [sp, #0xdc]
0052b9a0 bl       #0x51c0dc ; _ZN7PFFloor10_GetNodeAtERK7Point3DIfE
0052b9a4 cmp      r6, #0
0052b9a8 str      r0, [sp, #0x24]
0052b9ac beq      #0x52bf5c
0052b9b0 ldr      r1, [r7]
0052b9b4 ldr      r0, [r6, #8]
0052b9b8 bl       #0x30e3ac
0052b9bc ldr      r1, [r7, #4]
0052b9c0 mov      r3, r0
0052b9c4 ldr      r0, [r6, #0xc]
0052b9c8 str      r3, [sp, #0x14]
0052b9cc bl       #0x30e3ac
0052b9d0 ldr      r1, [r7, #8]
0052b9d4 mov      r2, r0
0052b9d8 ldr      r0, [r6, #0x10]
0052b9dc str      r2, [sp, #0x18]
0052b9e0 bl       #0x30e3ac
0052b9e4 ldr      r3, [sp, #0x14]
0052b9e8 mov      ip, r0
0052b9ec str      ip, [sp, #0x1c]
0052b9f0 mov      r1, r3
0052b9f4 mov      r0, r3
0052b9f8 bl       #0x30ed6c
0052b9fc ldr      r2, [sp, #0x18]
0052ba00 mov      r3, r0
0052ba04 str      r3, [sp, #0x14]
0052ba08 mov      r1, r2
0052ba0c mov      r0, r2
0052ba10 bl       #0x30ed6c
0052ba14 ldr      r3, [sp, #0x14]
0052ba18 mov      r1, r0
0052ba1c mov      r0, r3
0052ba20 bl       #0x30eba4
0052ba24 ldr      ip, [sp, #0x1c]
0052ba28 mov      r3, r0
0052ba2c str      r3, [sp, #0x14]
0052ba30 mov      r1, ip
0052ba34 mov      r0, ip
0052ba38 bl       #0x30ed6c
0052ba3c ldr      r3, [sp, #0x14]
0052ba40 mov      r1, r0
0052ba44 mov      r0, r3
0052ba48 bl       #0x30eba4
0052ba4c mvn      r1, #0x80000000
0052ba50 sub      r1, r1, #0x800000
0052ba54 str      r0, [sp, #0x34]
0052ba58 bl       #0x30e70c
0052ba5c cmp      r0, #0
0052ba60 beq      #0x52bf5c
0052ba64 ldr      r2, [sp, #0x30]
0052ba68 cmp      r2, #0
0052ba6c beq      #0x52bb38
0052ba70 ldr      r0, [r2, #8]
0052ba74 ldr      r1, [r7]
0052ba78 bl       #0x30e3ac
0052ba7c ldr      ip, [sp, #0x30]
0052ba80 ldr      r1, [r7, #4]
0052ba84 mov      r3, r0
0052ba88 ldr      r0, [ip, #0xc]
0052ba8c str      r3, [sp, #0x14]
0052ba90 bl       #0x30e3ac
0052ba94 ldr      lr, [sp, #0x30]
0052ba98 ldr      r1, [r7, #8]
0052ba9c mov      r2, r0
0052baa0 ldr      r0, [lr, #0x10]
0052baa4 str      r2, [sp, #0x18]
0052baa8 bl       #0x30e3ac
0052baac ldr      r3, [sp, #0x14]
0052bab0 mov      ip, r0
0052bab4 str      ip, [sp, #0x1c]
0052bab8 mov      r1, r3
0052babc mov      r0, r3
0052bac0 bl       #0x30ed6c
0052bac4 ldr      r2, [sp, #0x18]
0052bac8 mov      r3, r0
0052bacc str      r3, [sp, #0x14]
0052bad0 mov      r1, r2
0052bad4 mov      r0, r2
0052bad8 bl       #0x30ed6c
0052badc ldr      r3, [sp, #0x14]
0052bae0 mov      r1, r0
0052bae4 mov      r0, r3
0052bae8 bl       #0x30eba4
0052baec ldr      ip, [sp, #0x1c]
0052baf0 mov      r3, r0
0052baf4 str      r3, [sp, #0x14]
0052baf8 mov      r1, ip
0052bafc mov      r0, ip
0052bb00 bl       #0x30ed6c
0052bb04 ldr      r3, [sp, #0x14]
0052bb08 mov      r1, r0
0052bb0c mov      r0, r3
0052bb10 bl       #0x30eba4
0052bb14 mov      r3, r0
0052bb18 mov      r1, r3
0052bb1c ldr      r0, [sp, #0x34]
0052bb20 str      r3, [sp, #0x14]
0052bb24 bl       #0x30e2f8
0052bb28 ldr      r3, [sp, #0x14]
0052bb2c cmp      r0, #0
0052bb30 ldrne    r6, [sp, #0x30]
0052bb34 strne    r3, [sp, #0x34]
0052bb38 cmp      sl, #0
0052bb3c beq      #0x52bbf0
0052bb40 ldr      r1, [r7]
0052bb44 ldr      r0, [sl, #8]
0052bb48 bl       #0x30e3ac
0052bb4c ldr      r1, [r7, #4]
0052bb50 mov      r3, r0
0052bb54 ldr      r0, [sl, #0xc]
0052bb58 str      r3, [sp, #0x14]
0052bb5c bl       #0x30e3ac
0052bb60 ldr      r1, [r7, #8]
0052bb64 mov      r2, r0
0052bb68 ldr      r0, [sl, #0x10]
0052bb6c str      r2, [sp, #0x18]
0052bb70 bl       #0x30e3ac
0052bb74 ldr      r3, [sp, #0x14]
0052bb78 mov      ip, r0
0052bb7c str      ip, [sp, #0x1c]
0052bb80 mov      r1, r3
0052bb84 mov      r0, r3
0052bb88 bl       #0x30ed6c
0052bb8c ldr      r2, [sp, #0x18]
0052bb90 mov      r3, r0
0052bb94 str      r3, [sp, #0x14]
0052bb98 mov      r1, r2
0052bb9c mov      r0, r2
0052bba0 bl       #0x30ed6c
0052bba4 ldr      r3, [sp, #0x14]
0052bba8 mov      r1, r0
0052bbac mov      r0, r3
0052bbb0 bl       #0x30eba4
0052bbb4 ldr      ip, [sp, #0x1c]
0052bbb8 mov      r3, r0
0052bbbc str      r3, [sp, #0x14]
0052bbc0 mov      r1, ip
0052bbc4 mov      r0, ip
0052bbc8 bl       #0x30ed6c
0052bbcc ldr      r3, [sp, #0x14]
0052bbd0 mov      r1, r0
0052bbd4 mov      r0, r3
0052bbd8 bl       #0x30eba4
0052bbdc mov      r1, r0
0052bbe0 ldr      r0, [sp, #0x34]
0052bbe4 bl       #0x30e2f8
0052bbe8 cmp      r0, #0
0052bbec movne    r6, sl
0052bbf0 ldr      r2, [sp, #0x2c]
0052bbf4 cmp      r2, #0
0052bbf8 beq      #0x52bca0
0052bbfc ldr      r0, [r2, #8]
0052bc00 ldr      r1, [sb]
0052bc04 bl       #0x30e3ac
0052bc08 ldr      r3, [sp, #0x2c]
0052bc0c mov      sl, r0
0052bc10 ldr      r1, [sb, #4]
0052bc14 ldr      r0, [r3, #0xc]
0052bc18 bl       #0x30e3ac
0052bc1c ldr      ip, [sp, #0x2c]
0052bc20 mov      r3, r0
0052bc24 ldr      r1, [sb, #8]
0052bc28 ldr      r0, [ip, #0x10]
0052bc2c str      r3, [sp, #0x14]
0052bc30 bl       #0x30e3ac
0052bc34 mov      r1, sl
0052bc38 mov      r2, r0
0052bc3c mov      r0, sl
0052bc40 str      r2, [sp, #0x18]
0052bc44 bl       #0x30ed6c
0052bc48 ldr      r3, [sp, #0x14]
0052bc4c mov      sl, r0
0052bc50 mov      r1, r3
0052bc54 mov      r0, r3
0052bc58 bl       #0x30ed6c
0052bc5c mov      r1, r0
0052bc60 mov      r0, sl
0052bc64 bl       #0x30eba4
0052bc68 ldr      r2, [sp, #0x18]
0052bc6c mov      sl, r0
0052bc70 mov      r1, r2
0052bc74 mov      r0, r2
0052bc78 bl       #0x30ed6c
0052bc7c mov      r1, r0
0052bc80 mov      r0, sl
0052bc84 bl       #0x30eba4
0052bc88 mvn      r1, #0x80000000
0052bc8c sub      r1, r1, #0x800000
0052bc90 mov      sl, r0
0052bc94 bl       #0x30e70c
0052bc98 cmp      r0, #0
0052bc9c bne      #0x52bf70
0052bca0 mvn      r2, #0x80000000
0052bca4 sub      r2, r2, #0x800000
0052bca8 str      r2, [sp, #0x30]
0052bcac mov      sl, #0
0052bcb0 ldr      r3, [sp, #0x28]
0052bcb4 cmp      r3, #0
0052bcb8 beq      #0x52bd84
0052bcbc ldr      r1, [sb]
0052bcc0 ldr      r0, [r3, #8]
0052bcc4 bl       #0x30e3ac
0052bcc8 ldr      ip, [sp, #0x28]
0052bccc ldr      r1, [sb, #4]
0052bcd0 mov      r3, r0
0052bcd4 ldr      r0, [ip, #0xc]
0052bcd8 str      r3, [sp, #0x14]
0052bcdc bl       #0x30e3ac
0052bce0 ldr      lr, [sp, #0x28]
0052bce4 ldr      r1, [sb, #8]
0052bce8 mov      r2, r0
0052bcec ldr      r0, [lr, #0x10]
0052bcf0 str      r2, [sp, #0x18]
0052bcf4 bl       #0x30e3ac
0052bcf8 ldr      r3, [sp, #0x14]
0052bcfc mov      ip, r0
0052bd00 str      ip, [sp, #0x1c]
0052bd04 mov      r1, r3
0052bd08 mov      r0, r3
0052bd0c bl       #0x30ed6c
0052bd10 ldr      r2, [sp, #0x18]
0052bd14 mov      r3, r0
0052bd18 str      r3, [sp, #0x14]
0052bd1c mov      r1, r2
0052bd20 mov      r0, r2
0052bd24 bl       #0x30ed6c
0052bd28 ldr      r3, [sp, #0x14]
0052bd2c mov      r1, r0
0052bd30 mov      r0, r3
0052bd34 bl       #0x30eba4
0052bd38 ldr      ip, [sp, #0x1c]
0052bd3c mov      r3, r0
0052bd40 str      r3, [sp, #0x14]
0052bd44 mov      r1, ip
0052bd48 mov      r0, ip
0052bd4c bl       #0x30ed6c
0052bd50 ldr      r3, [sp, #0x14]
0052bd54 mov      r1, r0
0052bd58 mov      r0, r3
0052bd5c bl       #0x30eba4
0052bd60 mov      r3, r0
0052bd64 mov      r1, r3
0052bd68 ldr      r0, [sp, #0x30]
0052bd6c str      r3, [sp, #0x14]
0052bd70 bl       #0x30e2f8
0052bd74 ldr      r3, [sp, #0x14]
0052bd78 cmp      r0, #0
0052bd7c ldrne    sl, [sp, #0x28]
0052bd80 strne    r3, [sp, #0x30]
0052bd84 ldr      r2, [sp, #0x24]
0052bd88 cmp      r2, #0
0052bd8c beq      #0x52be4c
0052bd90 ldr      r0, [r2, #8]
0052bd94 ldr      r1, [sb]
0052bd98 bl       #0x30e3ac
0052bd9c ldr      ip, [sp, #0x24]
0052bda0 ldr      r1, [sb, #4]
0052bda4 mov      r3, r0
0052bda8 ldr      r0, [ip, #0xc]
0052bdac str      r3, [sp, #0x14]
0052bdb0 bl       #0x30e3ac
0052bdb4 ldr      lr, [sp, #0x24]
0052bdb8 ldr      r1, [sb, #8]
0052bdbc mov      r2, r0
0052bdc0 ldr      r0, [lr, #0x10]
0052bdc4 str      r2, [sp, #0x18]
0052bdc8 bl       #0x30e3ac
0052bdcc ldr      r3, [sp, #0x14]
0052bdd0 mov      ip, r0
0052bdd4 str      ip, [sp, #0x1c]
0052bdd8 mov      r1, r3
0052bddc mov      r0, r3
0052bde0 bl       #0x30ed6c
0052bde4 ldr      r2, [sp, #0x18]
0052bde8 mov      r3, r0
0052bdec str      r3, [sp, #0x14]
0052bdf0 mov      r1, r2
0052bdf4 mov      r0, r2
0052bdf8 bl       #0x30ed6c
0052bdfc ldr      r3, [sp, #0x14]
0052be00 mov      r1, r0
0052be04 mov      r0, r3
0052be08 bl       #0x30eba4
0052be0c ldr      ip, [sp, #0x1c]
0052be10 mov      r3, r0
0052be14 str      r3, [sp, #0x14]
0052be18 mov      r1, ip
0052be1c mov      r0, ip
0052be20 bl       #0x30ed6c
0052be24 ldr      r3, [sp, #0x14]
0052be28 mov      r1, r0
0052be2c mov      r0, r3
0052be30 bl       #0x30eba4
0052be34 mov      r1, r0
0052be38 ldr      r0, [sp, #0x30]
0052be3c bl       #0x30e2f8
0052be40 ldr      r2, [sp, #0x24]
0052be44 cmp      r0, #0
0052be48 movne    sl, r2
0052be4c cmp      r6, #0
0052be50 cmpne    sl, #0
0052be54 beq      #0x52b690
0052be58 cmp      r6, sl
0052be5c beq      #0x52c250
0052be60 ldr      r1, [r8, #0x78]
0052be64 ldr      r2, [r8, #0x7c]
0052be68 ldr      ip, [sp, #0x188]
0052be6c str      r6, [sp, #0xc8]
0052be70 rsb      r2, r1, r2
0052be74 asr      r2, r2, #2
0052be78 str      sl, [sp, #0xcc]
0052be7c add      r3, r2, r2, lsl #2
0052be80 str      ip, [sp, #0xd0]
0052be84 add      r3, r3, r3, lsl #4
0052be88 add      r3, r3, r3, lsl #8
0052be8c add      r3, r3, r3, lsl #16
0052be90 add      r3, r2, r3, lsl #1
0052be94 subs     r2, r3, #1
0052be98 bmi      #0x52bf7c
0052be9c mov      r0, #0xc
0052bea0 mul      r3, r0, r3
0052bea4 sub      r3, r3, #0xc
0052bea8 b        #0x52beb8
0052beac subs     r2, r2, #1
0052beb0 sub      r3, r3, #0xc
0052beb4 bmi      #0x52bf7c
0052beb8 ldr      r0, [r1, r3]
0052bebc add      ip, r1, r3
0052bec0 cmp      r6, r0
0052bec4 bne      #0x52beac
0052bec8 ldr      r0, [ip, #4]
0052becc cmp      sl, r0
0052bed0 bne      #0x52beac
0052bed4 ldr      r0, [ip, #8]
0052bed8 ldr      sb, [sp, #0x188]
0052bedc cmp      sb, r0
0052bee0 bne      #0x52beac
0052bee4 b        #0x52b690
0052bee8 bl       #0x310440 ; _Z10CustomFreePv
0052beec b        #0x52b5f4
0052bef0 ldr      sl, [sp, #0x8c]
0052bef4 ldr      r0, [sp, #0x68]
0052bef8 mov      r1, sl
0052befc bl       #0x30df8c
0052bf00 cmp      r0, #0
0052bf04 beq      #0x52bf3c
0052bf08 ldr      r2, [sp, #0x90]
0052bf0c ldr      r0, [sp, #0x6c]
0052bf10 mov      r1, r2
0052bf14 str      r2, [sp, #0x2c]
0052bf18 bl       #0x30df8c
0052bf1c cmp      r0, #0
0052bf20 bne      #0x52c084
0052bf24 ldr      ip, [sp, #0x98]
0052bf28 ldr      lr, [sp, #0x9c]
0052bf2c ldr      r3, [sp, #0x94]
0052bf30 str      ip, [sp, #0x24]
0052bf34 str      lr, [sp, #0x28]
0052bf38 b        #0x52b730
0052bf3c ldr      r2, [sp, #0x98]
0052bf40 ldr      ip, [sp, #0x90]
0052bf44 ldr      lr, [sp, #0x9c]
0052bf48 ldr      r3, [sp, #0x94]
0052bf4c str      r2, [sp, #0x24]
0052bf50 str      ip, [sp, #0x2c]
0052bf54 str      lr, [sp, #0x28]
0052bf58 b        #0x52b730
0052bf5c mvn      ip, #0x80000000
0052bf60 sub      ip, ip, #0x800000
0052bf64 str      ip, [sp, #0x34]
0052bf68 mov      r6, #0
0052bf6c b        #0x52ba64
0052bf70 str      sl, [sp, #0x30]
0052bf74 ldr      sl, [sp, #0x2c]
0052bf78 b        #0x52bcb0
0052bf7c ldr      r2, [pc, #0x3bc]
0052bf80 ldr      r3, [sp, #0x20]
0052bf84 ldr      r1, [r8, #0x48]
0052bf88 ldr      r2, [r4, r2]
0052bf8c add      r7, sp, #0x3c
0052bf90 cmp      r3, #0
0052bf94 add      r2, r2, #8
0052bf98 add      r3, r7, #8
0052bf9c str      r2, [sp, #0x3c]
0052bfa0 str      r1, [sp, #0x40]
0052bfa4 str      r3, [sp, #0x4c]
0052bfa8 str      r3, [sp, #0x44]
0052bfac str      r3, [sp, #0x48]
0052bfb0 beq      #0x52c2a8
0052bfb4 ldr      r3, [r6]
0052bfb8 mov      r0, r6
0052bfbc mov      lr, pc
0052bfc0 ldr      pc, [r3]
0052bfc4 ldr      r3, [pc, #0x378]
0052bfc8 mov      r1, r0
0052bfcc add      lr, sp, #0xac
0052bfd0 ldr      r3, [r4, r3]
0052bfd4 ldr      ip, [pc, #0x36c]
0052bfd8 ldr      r2, [r3, #4]
0052bfdc ldr      r0, [r3, #8]
0052bfe0 ldr      sb, [r3, #0x14]
0052bfe4 ldr      r2, [r2, #-0x18]
0052bfe8 ldr      r6, [r3, #0x10]
0052bfec str      sb, [sp, #0x34]
0052bff0 str      r0, [lr, r2]
0052bff4 str      sl, [sp, #0xb0]
0052bff8 ldr      r2, [r6, #-0x18]
0052bffc ldr      sb, [r3, #0xc]
0052c000 ldr      sl, [r3, #0x18]
0052c004 ldr      r3, [sp, #0x34]
0052c008 add      r2, lr, r2
0052c00c ldr      ip, [r4, ip]
0052c010 str      r3, [r2, #8]
0052c014 ldr      r0, [sb, #-0x18]
0052c018 mov      r2, lr
0052c01c ldr      r3, [sp, #0x188]
0052c020 add      lr, lr, r0
0052c024 str      sl, [lr, #8]
0052c028 ldr      sb, [sp, #0x20]
0052c02c add      r6, ip, #0x3c
0052c030 mov      r0, r7
0052c034 add      ip, ip, #0x18
0052c038 str      r6, [sp, #0xb4]
0052c03c str      sb, [sp, #0xb8]
0052c040 str      ip, [sp, #0xac]
0052c044 str      fp, [sp]
0052c048 bl       #0x52ad4c ; _ZN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE8findNodeEjRKNS1_5ITestI12PFGInnerEdge12PFGInnerNodeEEjPSt4listIPKS7_SaISE_EE
0052c04c mov      r6, r0
0052c050 cmp      r6, #0
0052c054 beq      #0x52c240
0052c058 cmp      fp, #0
0052c05c beq      #0x52c078
0052c060 cmp      r6, #0
0052c064 bne      #0x52c1c0
0052c068 add      r1, sp, #0x160
0052c06c str      r6, [r1, #-0x2c]!
0052c070 mov      r0, fp
0052c074 bl       #0x52aaa4 ; _ZNSt4listIPK12PFGInnerEdgeSaIS2_EE6resizeEjRKS2_.clone.3
0052c078 mov      r0, r7
0052c07c bl       #0x529f8c ; _ZN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicED1Ev
0052c080 b        #0x52b694
0052c084 ldr      r3, [sp, #0x94]
0052c088 ldr      r0, [sp, #0x70]
0052c08c mov      r1, r3
0052c090 str      r3, [sp, #0x14]
0052c094 bl       #0x30df8c
0052c098 cmp      r0, #0
0052c09c ldr      r3, [sp, #0x14]
0052c0a0 bne      #0x52c0b8
0052c0a4 ldr      r2, [sp, #0x98]
0052c0a8 ldr      ip, [sp, #0x9c]
0052c0ac str      r2, [sp, #0x24]
0052c0b0 str      ip, [sp, #0x28]
0052c0b4 b        #0x52b730
0052c0b8 ldr      lr, [sp, #0x98]
0052c0bc ldr      r0, [sp, #0x74]
0052c0c0 str      r3, [sp, #0x14]
0052c0c4 mov      r1, lr
0052c0c8 str      lr, [sp, #0x24]
0052c0cc bl       #0x30df8c
0052c0d0 cmp      r0, #0
0052c0d4 ldr      r3, [sp, #0x14]
0052c0d8 beq      #0x52b728
0052c0dc ldr      ip, [sp, #0x9c]
0052c0e0 ldr      r0, [sp, #0x78]
0052c0e4 str      r3, [sp, #0x14]
0052c0e8 mov      r1, ip
0052c0ec str      ip, [sp, #0x28]
0052c0f0 bl       #0x30df8c
0052c0f4 cmp      r0, #0
0052c0f8 ldr      r3, [sp, #0x14]
0052c0fc beq      #0x52b730
0052c100 ldr      r0, [sp, #0x7c]
0052c104 ldr      r1, [sp, #0xa0]
0052c108 bl       #0x30df8c
0052c10c cmp      r0, #0
0052c110 ldr      r3, [sp, #0x14]
0052c114 beq      #0x52b730
0052c118 ldr      r0, [sp, #0x80]
0052c11c ldr      r1, [sp, #0xa4]
0052c120 bl       #0x30df8c
0052c124 cmp      r0, #0
0052c128 ldr      r3, [sp, #0x14]
0052c12c beq      #0x52b730
0052c130 ldr      r0, [sp, #0x84]
0052c134 ldr      r1, [sp, #0xa8]
0052c138 bl       #0x30df8c
0052c13c cmp      r0, #0
0052c140 ldr      r3, [sp, #0x14]
0052c144 beq      #0x52b730
0052c148 ldr      r2, [sp, #0x20]
0052c14c cmp      fp, #0
0052c150 cmpne    r2, #0
0052c154 bne      #0x52c160
0052c158 mov      r6, #1
0052c15c b        #0x52b694
0052c160 ldr      r3, [r7]
0052c164 mov      r0, fp
0052c168 mov      r6, #1
0052c16c str      r3, [r2, #0x64]
0052c170 ldr      r3, [r7, #4]
0052c174 str      r3, [r2, #0x68]
0052c178 ldr      r3, [r7, #8]
0052c17c str      r3, [r2, #0x6c]
0052c180 ldr      r3, [sb]
0052c184 str      r3, [r2, #0x70]
0052c188 ldr      r3, [sb, #4]
0052c18c str      r3, [r2, #0x74]
0052c190 ldr      r3, [sb, #8]
0052c194 str      r3, [r2, #0x78]
0052c198 bl       #0x52aa84 ; _ZNSaINSt4priv10_List_nodeIPK12PFGInnerEdgeEEE8allocateEjPKv.clone.5
0052c19c ldr      sb, [sp, #0x20]
0052c1a0 add      r3, sb, #0x4c
0052c1a4 str      r3, [r0, #8]
0052c1a8 ldr      r3, [fp, #4]
0052c1ac str      fp, [r0]
0052c1b0 str      r3, [r0, #4]
0052c1b4 str      r0, [r3]
0052c1b8 str      r0, [fp, #4]
0052c1bc b        #0x52b694
0052c1c0 ldr      r3, [fp]
0052c1c4 cmp      r3, fp
0052c1c8 beq      #0x52c078
0052c1cc mov      r2, #0
0052c1d0 ldr      r3, [r3]
0052c1d4 add      r2, r2, #1
0052c1d8 cmp      fp, r3
0052c1dc bne      #0x52c1d0
0052c1e0 cmp      r2, #1
0052c1e4 bls      #0x52c078
0052c1e8 ldr      r3, [fp, #4]
0052c1ec ldr      r3, [r3, #8]
0052c1f0 mov      r0, r3
0052c1f4 ldr      r3, [r3]
0052c1f8 mov      lr, pc
0052c1fc ldr      pc, [r3, #4]
0052c200 ldr      ip, [sp, #0x28]
0052c204 ldr      lr, [sp, #0x2c]
0052c208 cmp      lr, r0
0052c20c cmpne    ip, r0
0052c210 beq      #0x52c220
0052c214 ldr      r2, [sp, #0x24]
0052c218 cmp      r2, r0
0052c21c bne      #0x52c078
0052c220 ldr      r0, [fp, #4]
0052c224 mov      r1, #0xc
0052c228 ldr      r3, [r0]
0052c22c ldr      r2, [r0, #4]
0052c230 str      r3, [r2]
0052c234 str      r2, [r3, #4]
0052c238 bl       #0x31bb44 ; _ZNSt12__node_alloc10deallocateEPvj
0052c23c b        #0x52c078
0052c240 add      r0, r8, #0x78
0052c244 add      r1, sp, #0xc8
0052c248 bl       #0x52a504 ; _ZN15SearchFailCache3addERNS_5EntryE
0052c24c b        #0x52c058
0052c250 ldr      r3, [sp, #0x20]
0052c254 cmp      fp, #0
0052c258 cmpne    r3, #0
0052c25c beq      #0x52c158
0052c260 ldr      r3, [r7]
0052c264 ldr      sl, [sp, #0x20]
0052c268 mov      r0, fp
0052c26c mov      r6, #1
0052c270 str      r3, [sl, #0x64]
0052c274 ldr      r3, [r7, #4]
0052c278 str      r3, [sl, #0x68]
0052c27c ldr      r3, [r7, #8]
0052c280 str      r3, [sl, #0x6c]
0052c284 ldr      r3, [sb]
0052c288 str      r3, [sl, #0x70]
0052c28c ldr      r3, [sb, #4]
0052c290 str      r3, [sl, #0x74]
0052c294 ldr      r3, [sb, #8]
0052c298 str      r3, [sl, #0x78]
0052c29c bl       #0x52aa84 ; _ZNSaINSt4priv10_List_nodeIPK12PFGInnerEdgeEEE8allocateEjPKv.clone.5
0052c2a0 add      r3, sl, #0x4c
0052c2a4 b        #0x52c1a4
0052c2a8 ldr      r3, [r6]
0052c2ac mov      r0, r6
0052c2b0 mov      lr, pc
0052c2b4 ldr      pc, [r3]
0052c2b8 ldr      r3, [pc, #0x8c]
0052c2bc mov      r1, r0
0052c2c0 add      lr, sp, #0xbc
0052c2c4 ldr      r3, [r4, r3]
0052c2c8 ldr      ip, [pc, #0x80]
0052c2cc ldr      r2, [r3, #4]
0052c2d0 ldr      r0, [r3, #8]
0052c2d4 ldr      sb, [r3, #0xc]
0052c2d8 ldr      r2, [r2, #-0x18]
0052c2dc ldr      r3, [r3, #0x10]
0052c2e0 ldr      ip, [r4, ip]
0052c2e4 str      r3, [sp, #0x20]
0052c2e8 str      r0, [lr, r2]
0052c2ec str      sl, [sp, #0xc0]
0052c2f0 ldr      r0, [sb, #-0x18]
0052c2f4 ldr      sl, [sp, #0x20]
0052c2f8 mov      r2, lr
0052c2fc add      lr, lr, r0
0052c300 add      r6, ip, #0x3c
0052c304 ldr      r3, [sp, #0x188]
0052c308 add      ip, ip, #0x18
0052c30c str      sl, [lr, #8]
0052c310 mov      r0, r7
0052c314 str      r6, [sp, #0xc4]
0052c318 str      ip, [sp, #0xbc]
0052c31c str      fp, [sp]
0052c320 bl       #0x52ad4c ; _ZN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE8findNodeEjRKNS1_5ITestI12PFGInnerEdge12PFGInnerNodeEEjPSt4listIPKS7_SaISE_EE
0052c324 mov      r6, r0
0052c328 b        #0x52c050
0052c32c bl       #0x30e310
0052c330 subeq    sb, r6, r0, lsr #10
0052c334 andeq    r4, r0, ip, lsr #1
0052c338 andeq    r0, r0, r4, lsl #17
0052c33c eorseq   r1, fp, r0, lsl r6
0052c340 andeq    r1, r0, ip, lsl r0
0052c344 andeq    r4, r0, r0, asr #22
0052c348 andeq    r3, r0, r8, ror #7
0052c34c andeq    r2, r0, r0, lsr sp
0052c350 andeq    r4, r0, r0, lsl r6
_ZN7PFWorldC1Ev
00522e2c ldr      r2, [pc, #0xc4]
00522e30 ldr      r1, [pc, #0xc4]
00522e34 push     {r4, r5, r6, lr}
00522e38 add      r2, pc, r2
00522e3c ldr      r1, [r2, r1]
00522e40 mov      r4, r0
00522e44 mov      r5, #0
00522e48 mov      r3, #0
00522e4c add      r1, r1, #8
00522e50 str      r3, [r4, #0x28]
00522e54 str      r3, [r4, #0x14]
00522e58 str      r3, [r4, #0x18]
00522e5c str      r3, [r4, #0x1c]
00522e60 str      r3, [r4, #0x20]
00522e64 str      r3, [r4, #0x24]
00522e68 stm      r4, {r1, r5}
00522e6c str      r5, [r4, #8]
00522e70 str      r5, [r4, #0xc]
00522e74 str      r5, [r4, #0x10]
00522e78 str      r5, [r4, #0x30]
00522e7c strb     r5, [r0, #0x2c]!
00522e80 str      r0, [r4, #0x38]
00522e84 str      r0, [r4, #0x34]
00522e88 str      r5, [r4, #0x3c]
00522e8c add      r0, r4, #0x4c
00522e90 str      r5, [r4, #0x44]
00522e94 str      r5, [r4, #0x48]
00522e98 str      r5, [r4, #0x4c]
00522e9c str      r5, [r4, #0x50]
00522ea0 str      r5, [r4, #0x54]
00522ea4 str      r5, [r4, #0x58]
00522ea8 str      r5, [r4, #0x5c]
00522eac str      r5, [r4, #0x60]
00522eb0 str      r5, [r4, #0x64]
00522eb4 str      r5, [r4, #0x68]
00522eb8 str      r5, [r4, #0x6c]
00522ebc str      r5, [r4, #0x70]
00522ec0 bl       #0x522cd4 ; _ZNSt4priv11_Deque_baseIjSaIjEE17_M_initialize_mapEj.clone.2
00522ec4 mov      r3, #0x42000000
00522ec8 add      r3, r3, #0xc80000
00522ecc strb     r5, [r4, #0x94]
00522ed0 str      r5, [r4, #0x74]
00522ed4 str      r3, [r4, #0x90]
00522ed8 str      r5, [r4, #0x78]
00522edc str      r5, [r4, #0x7c]
00522ee0 str      r5, [r4, #0x80]
00522ee4 str      r5, [r4, #0x84]
00522ee8 str      r5, [r4, #0x88]
00522eec str      r5, [r4, #0x8c]
00522ef0 mov      r0, r4
00522ef4 pop      {r4, r5, r6, pc}
00522ef8 subeq    r1, r7, r8, asr ip
00522efc andeq    r2, r0, r4, ror #12
