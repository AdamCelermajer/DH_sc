_Z25NativeGetStringFromSymbolRKN7gameswf7fn_callE 0x444ebc
00444ebc push {r4, r5, r6, r7, r8, sl, lr}
00444ec0 ldr r4, [pc, #0x188]
00444ec4 ldr r7, [pc, #0x188]
00444ec8 ldr r3, [r0, #0xc]
00444ecc add r4, pc, r4
00444ed0 ldr r1, [r4, r7]
00444ed4 sub sp, sp, #0x4c
00444ed8 ldr r2, [r0, #0x14]
00444edc ldr r1, [r1]
00444ee0 mov r6, r0
00444ee4 mov r0, #0xc
00444ee8 str r1, [sp, #0x44]
00444eec ldr r3, [r3]
00444ef0 mla r0, r0, r2, r3
00444ef4 bl #0x796fb4 ; _ZNK7gameswf8as_value9to_stringEv
00444ef8 ldr r1, [pc, #0x158]
00444efc mov r5, r0
00444f00 add r1, pc, r1
00444f04 bl #0x30e31c ; 
00444f08 cmp r0, #0
00444f0c beq #0x445018
00444f10 ldr r1, [pc, #0x144]
00444f14 mov r0, r5
00444f18 add r1, pc, r1
00444f1c bl #0x30e31c ; 
00444f20 cmp r0, #0
00444f24 beq #0x445018
00444f28 ldr r3, [r6, #0xc]
00444f2c ldr r2, [r6, #0x14]
00444f30 mov r0, #0xc
00444f34 ldr r3, [r3]
00444f38 add r5, sp, #0x18
00444f3c mla r0, r0, r2, r3
00444f40 bl #0x420a84 ; _ZNK7gameswf8as_value12to_tu_stringEv
00444f44 ldr r3, [pc, #0x114]
00444f48 ldrsb r2, [r0]
00444f4c ldr r3, [r4, r3]
00444f50 cmn r2, #1
00444f54 addne r1, r0, #1
00444f58 ldr r8, [r3, #0x34]
00444f5c ldreq r1, [r0, #0xc]
00444f60 mov r0, r8
00444f64 bl #0x508c74 ; _ZNK13StringManager19getStringFromSymbolEPKc
00444f68 mov r1, #0x10
00444f6c mov sl, r0
00444f70 mov r0, r5
00444f74 str r5, [sp, #0x28]
00444f78 str r5, [sp, #0x2c]
00444f7c bl #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
00444f80 ldr r3, [sp, #0x28]
00444f84 mov r2, #0
00444f88 cmp sl, #0
00444f8c strb r2, [r3]
00444f90 beq #0x44502c
00444f94 mov r2, sl
00444f98 mov r0, r8
00444f9c mov r1, r5
00444fa0 bl #0x508ef4 ; _ZN13StringManager5parseERSsPKcz
00444fa4 mov r1, r5
00444fa8 mov r0, sp
00444fac bl #0x433d00 ; _Z15ParsePlayerNameRKSs
00444fb0 ldr r1, [sp, #0x14]
00444fb4 ldr r2, [sp, #0x10]
00444fb8 mov r0, r5
00444fbc bl #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
00444fc0 mov r0, sp
00444fc4 bl #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
00444fc8 ldr r8, [r6]
00444fcc ldr r1, [sp, #0x2c]
00444fd0 add r6, sp, #0x30
00444fd4 mov r0, r6
00444fd8 bl #0x413a7c ; _ZN7gameswf9tu_stringC1EPKc
00444fdc mov r0, r8
00444fe0 mov r1, r6
00444fe4 bl #0x7972d8 ; _ZN7gameswf8as_value13set_tu_stringERKNS_9tu_stringE
00444fe8 ldrsb r3, [sp, #0x30]
00444fec cmn r3, #1
00444ff0 beq #0x44503c
00444ff4 mov r0, r5
00444ff8 bl #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
00444ffc ldr r3, [r4, r7]
00445000 ldr r2, [sp, #0x44]
00445004 ldr r3, [r3]
00445008 cmp r2, r3
0044500c bne #0x44504c
00445010 add sp, sp, #0x4c
00445014 pop {r4, r5, r6, r7, r8, sl, pc}
00445018 ldr r3, [pc, #0x44]
0044501c mov r2, #1
00445020 ldr r3, [r4, r3]
00445024 strb r2, [r3]
00445028 b #0x444f28 ; 
0044502c ldr r1, [pc, #0x34]
00445030 ldr r8, [r6]
00445034 add r1, pc, r1
00445038 b #0x444fd0 ; 
0044503c ldr r0, [sp, #0x3c]
00445040 ldr r1, [sp, #0x38]
00445044 bl #0x752b38 ; _ZN7gameswf13free_internalEPvj
00445048 b #0x444ff4 ; 
0044504c bl #0x30e310 ; 
00445050 subseq pc, r4, r4, asr #23
00445054 andeq r4, r0, ip, lsr #1
00445058 subeq r7, r8, r0, asr r0
0044505c subeq r7, r8, r0, asr r0
00445060 strdeq r3, r4, [r0], -r4
00445064 andeq r1, r0, r4, lsr #29
00445068 subeq r6, r8, ip, asr #30

_ZNK13StringManager19getStringFromSymbolEPKc 0x508c74
00508c74 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00508c78 ldr r8, [pc, #0x188]
00508c7c ldr sb, [pc, #0x188]
00508c80 ldr fp, [pc, #0x188]
00508c84 add r8, pc, r8
00508c88 ldr r3, [r8, sb]
00508c8c ldr r6, [r8, fp]
00508c90 sub sp, sp, #0x44
00508c94 ldr r3, [r3]
00508c98 mov r4, r0
00508c9c mov r0, r6
00508ca0 str r3, [sp, #0x3c]
00508ca4 mov r7, r1
00508ca8 bl #0x337888 ; _ZN13DebugSwitches4loadEv
00508cac ldr r1, [pc, #0x160]
00508cb0 add r5, sp, #0x24
00508cb4 add r2, sp, #8
00508cb8 add r1, pc, r1
00508cbc mov r0, r5
00508cc0 bl #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00508cc4 mov r1, r5
00508cc8 mov r0, r6
00508ccc bl #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
00508cd0 mov r0, r5
00508cd4 bl #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
00508cd8 mov r0, r7
00508cdc mov r1, #0x5f
00508ce0 bl #0x30ec28 ; 
00508ce4 mov r6, #0
00508ce8 rsb sl, r7, r0
00508cec mov r1, r6
00508cf0 mov r0, r4
00508cf4 bl #0x507568 ; _ZNK13StringManager12getSheetNameEj
00508cf8 mov r2, sl
00508cfc mov r1, r0
00508d00 mov r0, r7
00508d04 bl #0x30ddc4 ; 
00508d08 subs r5, r0, #0
00508d0c bne #0x508d84
00508d10 mov r0, r4
00508d14 bl #0x5075b0 ; _ZNK13StringManager13getSymbolPackEv
00508d18 mov r1, r6
00508d1c mov r3, r0
00508d20 mov r2, r5
00508d24 mov r0, r4
00508d28 bl #0x5088c4 ; _ZNK13StringManager12getStringIdxEiij
00508d2c b #0x508d64 ; 
00508d30 mov r0, r4
00508d34 bl #0x5075b0 ; _ZNK13StringManager13getSymbolPackEv
00508d38 mov r1, r6
00508d3c mov r3, r0
00508d40 mov r2, r5
00508d44 mov r0, r4
00508d48 bl #0x5088c4 ; _ZNK13StringManager12getStringIdxEiij
00508d4c mov r1, r0
00508d50 mov r0, r7
00508d54 bl #0x30e6e8 ; 
00508d58 cmp r0, #0
00508d5c beq #0x508d98
00508d60 add r5, r5, #1
00508d64 mov r0, r4
00508d68 bl #0x5075b0 ; _ZNK13StringManager13getSymbolPackEv
00508d6c mov r1, r6
00508d70 mov r2, r0
00508d74 mov r0, r4
00508d78 bl #0x507550 ; _ZNK13StringManager18getNumberOfStringsEii
00508d7c cmp r5, r0
00508d80 blt #0x508d30
00508d84 add r6, r6, #1
00508d88 cmp r6, #0x25
00508d8c bne #0x508cec
00508d90 mov r6, #0
00508d94 b #0x508de4 ; 
00508d98 mov r1, r6
00508d9c mov r2, r5
00508da0 mov r0, r4
00508da4 bl #0x508c6c ; _ZNK13StringManager12getStringIdxEii
00508da8 ldr r5, [r8, fp]
00508dac mov r6, r0
00508db0 add r4, sp, #0xc
00508db4 mov r0, r5
00508db8 bl #0x337888 ; _ZN13DebugSwitches4loadEv
00508dbc ldr r1, [pc, #0x54]
00508dc0 add r2, sp, #4
00508dc4 mov r0, r4
00508dc8 add r1, pc, r1
00508dcc bl #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00508dd0 mov r0, r5
00508dd4 mov r1, r4
00508dd8 bl #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
00508ddc mov r0, r4
00508de0 bl #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
00508de4 ldr r3, [r8, sb]
00508de8 ldr r2, [sp, #0x3c]
00508dec mov r0, r6
00508df0 ldr r3, [r3]
00508df4 cmp r2, r3
00508df8 bne #0x508e04
00508dfc add sp, sp, #0x44
00508e00 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00508e04 bl #0x30e310 ; 
00508e08 subeq fp, r8, ip, lsl #28
00508e0c andeq r4, r0, ip, lsr #1
00508e10 andeq r0, r0, r4, lsl #17
00508e14 ldrhteq r2, [sp], -r0
00508e18 eorseq r2, sp, r0, lsr #29

_ZN13StringManager16preloadPackSheetEjjb 0x50851c
0050851c ldr ip, [pc, #0x38c]
00508520 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00508524 ldr lr, [pc, #0x388]
00508528 add ip, pc, ip
0050852c mov r4, r3
00508530 ldr r3, [ip, lr]
00508534 sub sp, sp, #0xd4
00508538 str ip, [sp, #0xc]
0050853c ldr r3, [r3]
00508540 str lr, [sp, #0x14]
00508544 mov sb, r0
00508548 str r1, [sp, #0x1c]
0050854c str r2, [sp, #0x20]
00508550 str r3, [sp, #0xcc]
00508554 bl #0x507668 ; _ZNK13StringManager17isPackSheetLoadedEjj
00508558 cmp r0, #0
0050855c beq #0x5085a0
00508560 cmp r4, #0
00508564 bne #0x508590
00508568 mov r0, #1
0050856c ldr r3, [sp, #0xc]
00508570 ldr r2, [sp, #0x14]
00508574 ldr r1, [r3, r2]
00508578 ldr r2, [sp, #0xcc]
0050857c ldr r3, [r1]
00508580 cmp r2, r3
00508584 bne #0x5088ac
00508588 add sp, sp, #0xd4
0050858c pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00508590 mov r0, sb
00508594 ldr r1, [sp, #0x1c]
00508598 ldr r2, [sp, #0x20]
0050859c bl #0x507788 ; _ZN13StringManager15unloadPackSheetEjj
005085a0 ldr r0, [pc, #0x310]
005085a4 add r4, sp, #0x38
005085a8 mov ip, #0x64
005085ac str r0, [sp, #0x24]
005085b0 ldr r1, [sp, #0x1c]
005085b4 mov r0, sb
005085b8 ldr r2, [sp, #0x20]
005085bc mov r3, r4
005085c0 str ip, [sp]
005085c4 bl #0x507bbc ; _ZNK13StringManager16getSheetFilenameEjjPci
005085c8 ldr r1, [sp, #0x24]
005085cc ldr r2, [sp, #0xc]
005085d0 ldr r3, [r2, r1]
005085d4 mov r1, r4
005085d8 ldr r3, [r3, #0x10]
005085dc ldr r3, [r3, #0x34]
005085e0 mov r0, r3
005085e4 ldr r3, [r3]
005085e8 mov lr, pc
005085ec ldr pc, [r3, #0x90]
005085f0 subs r3, r0, #0
005085f4 moveq r0, r3
005085f8 beq #0x50856c
005085fc add r4, sp, #0x32
00508600 mov r1, r4
00508604 str r3, [sp, #0x2c]
00508608 bl #0x5075b8 ; _ZN12StreamReader6readAsItEEvP11IStreamBasePT_
0050860c mov r3, #1
00508610 cmp r3, #0
00508614 str r3, [sp, #0x28]
00508618 bne #0x50865c
0050861c mov r3, r4
00508620 add r4, r4, #1
00508624 ldrb r1, [r3, #1]
00508628 ldrb r2, [r4, #-1]
0050862c cmp r4, r3
00508630 eor r2, r1, r2
00508634 strb r2, [r4, #-1]
00508638 ldrb r1, [r3, #1]
0050863c eor r2, r2, r1
00508640 strb r2, [r3, #1]
00508644 ldrb r1, [r4, #-1]
00508648 sub r3, r3, #1
0050864c eor r2, r2, r1
00508650 strb r2, [r4, #-1]
00508654 add r4, r4, #1
00508658 blo #0x508624
0050865c ldrh r0, [sp, #0x32]
00508660 add r3, sp, #0xb4
00508664 mov r1, #0
00508668 add r0, r0, #1
0050866c lsl r0, r0, #2
00508670 str r3, [sp, #0x10]
00508674 bl #0x31056c ; _Znaj15MemoryHintState
00508678 mov r5, r0
0050867c ldr r0, [sp, #0x10]
00508680 mov r1, #0x10
00508684 mov r4, #0
00508688 str r0, [sp, #0xc4]
0050868c str r0, [sp, #0xc8]
00508690 bl #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
00508694 ldr r3, [sp, #0xc4]
00508698 strb r4, [r3]
0050869c ldrh r3, [sp, #0x32]
005086a0 cmp r3, r4
005086a4 beq #0x5087ac
005086a8 add sl, sp, #0x30
005086ac mov r8, #1
005086b0 add lr, sl, r8
005086b4 str lr, [sp, #0x18]
005086b8 mov r7, r4
005086bc ldr r0, [sp, #0x2c]
005086c0 mov r1, sl
005086c4 bl #0x5075b8 ; _ZN12StreamReader6readAsItEEvP11IStreamBasePT_
005086c8 cmp r8, #0
005086cc str r8, [sp, #0x28]
005086d0 bne #0x508714
005086d4 ldr r3, [sp, #0x18]
005086d8 mov r2, sl
005086dc ldrb r0, [r2, #1]
005086e0 ldrb r1, [r3, #-1]
005086e4 cmp r3, r2
005086e8 eor r1, r0, r1
005086ec strb r1, [r3, #-1]
005086f0 ldrb r0, [r2, #1]
005086f4 eor r1, r1, r0
005086f8 strb r1, [r2, #1]
005086fc ldrb r0, [r3, #-1]
00508700 sub r2, r2, #1
00508704 eor r1, r1, r0
00508708 strb r1, [r3, #-1]
0050870c add r3, r3, #1
00508710 blo #0x5086dc
00508714 ldrh r0, [sp, #0x30]
00508718 mov r1, #0
0050871c lsl r6, r4, #2
00508720 add r0, r0, #1
00508724 bl #0x31056c ; _Znaj15MemoryHintState
00508728 str r0, [r5, r4, lsl #2]
0050872c mov r1, r0
00508730 ldrh r2, [sp, #0x30]
00508734 mov r3, #0
00508738 ldr r0, [sp, #0x2c]
0050873c bl #0x317454 ; _ZN12StreamReader12readStringExEP11IStreamBasePcy
00508740 ldrh r3, [sp, #0x30]
00508744 ldr r2, [r5, r4, lsl #2]
00508748 mov r1, #0x5e
0050874c strb r7, [r2, r3]
00508750 ldr fp, [r5, r4, lsl #2]
00508754 mov r0, fp
00508758 bl #0x30ec28 ; 
0050875c cmp r0, #0
00508760 beq #0x508894
00508764 ldr r3, [sp, #0xc8]
00508768 ldr r2, [sp, #0xc4]
0050876c mov r0, sb
00508770 cmp r3, r2
00508774 strbne r7, [r3]
00508778 ldrne r3, [sp, #0xc8]
0050877c ldr r1, [sp, #0x10]
00508780 strne r3, [sp, #0xc4]
00508784 ldrne fp, [r5, r6]
00508788 mov r2, fp
0050878c bl #0x507ea4 ; _ZN13StringManager11parseColorsERSsPKc
00508790 cmp r0, #0
00508794 bne #0x508850
00508798 ldrh r3, [sp, #0x32]
0050879c add r4, r4, #1
005087a0 uxth r4, r4
005087a4 cmp r3, r4
005087a8 bhi #0x5086bc
005087ac ldr r0, [sp, #0x1c]
005087b0 ldr r1, [sp, #0x20]
005087b4 mov r2, #0x25
005087b8 ldr ip, [sp, #0xc]
005087bc mla r2, r2, r0, r1
005087c0 ldr r1, [pc, #0xf4]
005087c4 mov r0, #0
005087c8 str r0, [r5, r3, lsl #2]
005087cc ldr r4, [ip, r1]
005087d0 add r1, r2, #0x29c
005087d4 add r2, r2, #2
005087d8 str r5, [sb, r2, lsl #2]
005087dc ldrh lr, [sp, #0x32]
005087e0 add r3, sb, r1, lsl #1
005087e4 mov r0, r4
005087e8 strh lr, [r3, #4]
005087ec bl #0x337888 ; _ZN13DebugSwitches4loadEv
005087f0 ldr r1, [pc, #0xc8]
005087f4 add r5, sp, #0x9c
005087f8 add r2, sp, #0x34
005087fc add r1, pc, r1
00508800 mov r0, r5
00508804 bl #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00508808 mov r1, r5
0050880c mov r0, r4
00508810 bl #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
00508814 mov r0, r5
00508818 bl #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0050881c ldr r1, [sp, #0xc]
00508820 ldr r0, [sp, #0x24]
00508824 ldr r3, [r1, r0]
00508828 add r1, sp, #0x2c
0050882c ldr r3, [r3, #0x10]
00508830 ldr r3, [r3, #0x34]
00508834 mov r0, r3
00508838 ldr r3, [r3]
0050883c mov lr, pc
00508840 ldr pc, [r3, #0x78]
00508844 ldr r0, [sp, #0x10]
00508848 bl #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0050884c b #0x508568 ; 
00508850 ldr r0, [r5, r6]
00508854 bl #0x310440 ; _Z10CustomFreePv
00508858 ldr r2, [sp, #0xc4]
0050885c ldr r3, [sp, #0xc8]
00508860 mov r1, #0
00508864 rsb r3, r3, r2
00508868 uxth r3, r3
0050886c add r0, r3, #1
00508870 strh r3, [sp, #0x30]
00508874 bl #0x31056c ; _Znaj15MemoryHintState
00508878 str r0, [r5, r6]
0050887c ldr r1, [sp, #0xc8]
00508880 bl #0x30e520 ; 
00508884 ldr r2, [r5, r6]
00508888 ldrh r3, [sp, #0x30]
0050888c strb r7, [r2, r3]
00508890 b #0x508798 ; 
00508894 mov r0, fp
00508898 mov r1, #0x7c
0050889c bl #0x30ec28 ; 
005088a0 cmp r0, #0
005088a4 bne #0x508764
005088a8 b #0x508798 ; 
005088ac bl #0x30e310 ; 
005088b0 subeq ip, r8, r8, ror #10
005088b4 andeq r4, r0, ip, lsr #1
005088b8 strdeq r3, r4, [r0], -r4
005088bc andeq r0, r0, r4, lsl #17
005088c0 eorseq r3, sp, ip, ror #8

_ZNK13StringManager16getSheetFilenameEjjPci 0x507bbc
00507bbc ldr ip, [pc, #0x3c]
00507bc0 ldr r0, [pc, #0x3c]
00507bc4 str r4, [sp, #-4]!
00507bc8 add ip, pc, ip
00507bcc ldr r0, [ip, r0]
00507bd0 mov ip, #0x14
00507bd4 ldr r4, [r0]
00507bd8 mov r0, r3
00507bdc mov r3, #0xc
00507be0 mla r4, r3, r1, r4
00507be4 ldr r1, [pc, #0x1c]
00507be8 ldr r3, [r4, #8]
00507bec add r1, pc, r1
00507bf0 mla r3, ip, r2, r3
00507bf4 ldr r2, [r3, #8]
00507bf8 ldm sp!, {r4}
00507bfc b #0x30eae4 ; 
00507c00 subeq ip, r8, r8, asr #29
00507c04 muleq r0, r0, pc
00507c08 ldrshteq r3, [sp], -ip
