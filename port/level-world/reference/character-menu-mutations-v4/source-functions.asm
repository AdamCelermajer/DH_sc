Actual _ZTV14CNetPlayerInfo virtual0x50 -> 0x80f1ec
Actual _ZTVN14CMatchingLocal21ServerBackupNetStructE virtual0x6c -> 0x8002bc
Actual _ZTVN14CMatchingLocal21ServerBackupNetStructE virtual0x74 -> 0x808094
Actual _ZTV14NetStructArrayIN14CMatchingLocal21ServerBackupNetStructELj32EE virtual0x6c -> 0x0
Actual _ZTV14NetStructArrayIN14CMatchingLocal21ServerBackupNetStructELj32EE virtual0x74 -> 0x806344
Actual _ZTV10PlayerInfo virtual0x50 -> 0x80f1ec
Actual _ZTVN14CMatchingLocal17RoomInfoNetStructE virtual0x6c -> 0x8135e4
Actual _ZTVN14CMatchingLocal17RoomInfoNetStructE virtual0x74 -> 0x8137ec
Actual _ZTVN14CMatchingLocal19MemberInfoNetStructE virtual0x6c -> 0x8135e4
Actual _ZTVN14CMatchingLocal19MemberInfoNetStructE virtual0x74 -> 0x8137ec
Actual _ZTV14CMatchingLocal virtual0x6c -> 0x801b30
Actual _ZTV14CMatchingLocal virtual0x74 -> 0x801b48
0x3bc4a8 _ZN9Character7SG_SaveEv
003bc4a8 movw     r3, #0x14e8
003bc4ac ldr      r0, [r0, r3]
003bc4b0 cmp      r0, #0
003bc4b4 bxeq     lr
003bc4b8 b        #0x464b2c
0x7fe4e0 _ZN9CMatching8IsServerEv
007fe4e0 push     {r4, r5, r6, lr}
007fe4e4 ldrb     r3, [r0, #0xc]
007fe4e8 mov      r4, r0
007fe4ec cmp      r3, #0
007fe4f0 bne      #0x7fe4fc
007fe4f4 mov      r0, #0
007fe4f8 pop      {r4, r5, r6, pc}
007fe4fc ldr      r3, [r0]
007fe500 mov      lr, pc
007fe504 ldr      pc, [r3, #0x6c]
007fe508 cmp      r0, #0
007fe50c blt      #0x7fe4f4
007fe510 ldr      r3, [r4]
007fe514 mov      r0, r4
007fe518 mov      lr, pc
007fe51c ldr      pc, [r3, #0x6c]
007fe520 ldr      r3, [r4]
007fe524 mov      r5, r0
007fe528 mov      r0, r4
007fe52c mov      lr, pc
007fe530 ldr      pc, [r3, #0x74]
007fe534 cmp      r5, r0
007fe538 movne    r0, #0
007fe53c moveq    r0, #1
007fe540 pop      {r4, r5, r6, pc}
0x800f8c _ZN9CMatching3GetEv
00800f8c push     {r4, r5, r6, r7, r8, lr}
00800f90 ldr      r4, [pc, #0x100]
00800f94 ldr      r5, [pc, #0x100]
00800f98 add      r4, pc, r4
00800f9c ldr      r3, [r4, r5]
00800fa0 ldr      r0, [r3]
00800fa4 cmp      r0, #0
00800fa8 beq      #0x800fb0
00800fac pop      {r4, r5, r6, r7, r8, pc}
00800fb0 ldr      r6, [pc, #0xe8]
00800fb4 ldr      r3, [r4, r6]
00800fb8 ldr      r1, [r3]
00800fbc cmp      r1, #0
00800fc0 moveq    r2, #1
00800fc4 streq    r2, [r3]
00800fc8 beq      #0x800ff8
00800fcc cmp      r1, #1
00800fd0 beq      #0x800ff8
00800fd4 cmp      r1, #2
00800fd8 beq      #0x801074
00800fdc cmp      r1, #3
00800fe0 beq      #0x801048
00800fe4 cmp      r1, #4
00800fe8 beq      #0x801020
00800fec ldr      r3, [r4, r5]
00800ff0 ldr      r0, [r3]
00800ff4 pop      {r4, r5, r6, r7, r8, pc}
00800ff8 mov      r1, #2
00800ffc movw     r0, #0xa9e8
00801000 bl       #0x310570
00801004 mov      r7, r0
00801008 bl       #0x807d08
0080100c ldr      r2, [r4, r6]
00801010 ldr      r3, [r4, r5]
00801014 ldr      r1, [r2]
00801018 str      r7, [r3]
0080101c b        #0x800fd4
00801020 mov      r1, #2
00801024 movw     r0, #0x6c08
00801028 bl       #0x310570
0080102c mov      r1, #1
00801030 mov      r6, r0
00801034 bl       #0x81e9dc
00801038 ldr      r3, [r4, r5]
0080103c mov      r0, r6
00801040 str      r6, [r3]
00801044 b        #0x800fac
00801048 mov      r1, #2
0080104c movw     r0, #0x6c08
00801050 bl       #0x310570
00801054 mov      r1, #0
00801058 mov      r7, r0
0080105c bl       #0x81e9dc
00801060 ldr      r2, [r4, r6]
00801064 ldr      r3, [r4, r5]
00801068 ldr      r1, [r2]
0080106c str      r7, [r3]
00801070 b        #0x800fe4
00801074 movw     r0, #0xabb8
00801078 bl       #0x310570
0080107c mov      r7, r0
00801080 bl       #0x800ed8
00801084 ldr      r2, [r4, r6]
00801088 ldr      r3, [r4, r5]
0080108c ldr      r1, [r2]
00801090 str      r7, [r3]
00801094 b        #0x800fdc
00801098 ldrsheq  r3, [sb], -r8
0080109c andeq    r4, r0, r8, lsr r3
008010a0 andeq    r4, r0, r8, asr fp
0x80003c _ZN9CMatchingC2Ev
0080003c ldr      r3, [pc, #0x8c]
00800040 ldr      r1, [pc, #0x8c]
00800044 ldr      r2, [pc, #0x8c]
00800048 add      r3, pc, r3
0080004c ldr      r1, [r3, r1]
00800050 ldr      r2, [r3, r2]
00800054 push     {r4, r5, r6, lr}
00800058 add      r1, r1, #8
0080005c add      r2, r2, #8
00800060 mov      r4, #0
00800064 str      r1, [r0]
00800068 str      r2, [r0, #0x18]
0080006c mov      r1, #1
00800070 mov      r2, #0x20
00800074 mov      r6, r0
00800078 strb     r1, [r0, #0x10]
0080007c str      r2, [r0, #0x1c]
00800080 str      r4, [r0, #8]
00800084 strb     r4, [r0, #0xc]
00800088 strb     r4, [r0, #0xd]
0080008c strb     r4, [r0, #0xe]
00800090 strb     r4, [r0, #0xf]
00800094 add      r5, r0, r2
00800098 add      r0, r5, r4
0080009c add      r4, r4, #0x1b0
008000a0 bl       #0x7ffd04
008000a4 cmp      r4, #0x3600
008000a8 bne      #0x800098
008000ac mov      r3, #0
008000b0 movw     r2, #0x3628
008000b4 str      r3, [r6, r2]
008000b8 movw     r2, #0x3620
008000bc str      r3, [r6, r2]
008000c0 movw     r2, #0x3624
008000c4 str      r3, [r6, r2]
008000c8 mov      r0, r6
008000cc pop      {r4, r5, r6, pc}
008000d0 andseq   r4, sb, r8, asr #20
008000d4 andeq    r1, r0, r0, ror #3
008000d8 muleq    r0, r8, r6
0x808094 _ZN14CMatchingLocalD0Ev
00808094 push     {r4, lr}
00808098 mov      r4, r0
0080809c bl       #0x807f80
008080a0 mov      r0, r4
008080a4 bl       #0x310440
008080a8 mov      r0, r4
008080ac pop      {r4, pc}
0x80fc28 _ZN14CNetPlayerInfoC2Ev
0080fc28 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0080fc2c ldr      r5, [pc, #0x440]
0080fc30 ldr      r7, [pc, #0x440]
0080fc34 sub      sp, sp, #0x44
0080fc38 add      r5, pc, r5
0080fc3c ldr      r3, [r5, r7]
0080fc40 mov      r4, r0
0080fc44 ldr      r6, [pc, #0x430]
0080fc48 ldr      r3, [r3]
0080fc4c mov      r8, #0
0080fc50 mov      sb, #0
0080fc54 str      r3, [sp, #0x3c]
0080fc58 bl       #0x8138f4
0080fc5c ldr      r3, [pc, #0x41c]
0080fc60 ldr      r2, [r4, #0x150]
0080fc64 ldr      r0, [r5, r6]
0080fc68 ldr      r3, [r5, r3]
0080fc6c cmp      r2, #0
0080fc70 mov      ip, #0x138
0080fc74 mov      r2, #0
0080fc78 add      r3, r3, #8
0080fc7c strd     r8, sb, [r4, ip]
0080fc80 mvn      r1, #0
0080fc84 str      r3, [r4]
0080fc88 str      r2, [r4, #0x148]
0080fc8c strb     r2, [r4, #0x14c]
0080fc90 add      r0, r0, #8
0080fc94 mov      r3, #0x11
0080fc98 addeq    r2, r4, #0x130
0080fc9c str      r3, [r4, #0x134]
0080fca0 str      r1, [r4, #0x144]
0080fca4 str      r0, [r4, #0x130]
0080fca8 str      r1, [r4, #0x140]
0080fcac streq    r2, [sp, #8]
0080fcb0 beq      #0x80fcc8
0080fcb4 add      r3, r4, #0x130
0080fcb8 str      r3, [sp, #8]
0080fcbc str      r2, [r4, #0x150]
0080fcc0 ldr      r0, [sp, #8]
0080fcc4 bl       #0x814f84
0080fcc8 ldr      r8, [pc, #0x3b4]
0080fccc ldr      r3, [r4, #0x178]
0080fcd0 ldr      r1, [r5, r6]
0080fcd4 ldr      r0, [r5, r8]
0080fcd8 cmp      r3, #0
0080fcdc mov      fp, #0
0080fce0 add      r0, r0, #8
0080fce4 mov      ip, #0x160
0080fce8 mov      sl, #0
0080fcec strd     sl, fp, [r4, ip]
0080fcf0 mvn      r2, #0
0080fcf4 mov      r3, #0
0080fcf8 str      r0, [r4, #0x130]
0080fcfc add      r1, r1, #8
0080fd00 mov      r0, #8
0080fd04 addeq    fp, r4, #0x158
0080fd08 str      r0, [r4, #0x15c]
0080fd0c str      r2, [r4, #0x16c]
0080fd10 str      r1, [r4, #0x158]
0080fd14 str      r2, [r4, #0x168]
0080fd18 str      r3, [r4, #0x170]
0080fd1c strb     r3, [r4, #0x174]
0080fd20 streq    fp, [sp, #0xc]
0080fd24 beq      #0x80fd3c
0080fd28 add      r2, r4, #0x158
0080fd2c str      r2, [sp, #0xc]
0080fd30 str      r3, [r4, #0x178]
0080fd34 ldr      r0, [sp, #0xc]
0080fd38 bl       #0x814f84
0080fd3c ldr      r3, [pc, #0x344]
0080fd40 ldr      r2, [r4, #0x1a0]
0080fd44 ldr      r0, [r5, r6]
0080fd48 ldr      r3, [r5, r3]
0080fd4c cmp      r2, #0
0080fd50 mov      fp, #0
0080fd54 add      r3, r3, #8
0080fd58 mov      ip, #0x188
0080fd5c mov      sl, #0
0080fd60 strd     sl, fp, [r4, ip]
0080fd64 mvn      r1, #0
0080fd68 mov      r2, #0
0080fd6c str      r3, [r4, #0x158]
0080fd70 add      r0, r0, #8
0080fd74 mov      r3, #0x11
0080fd78 addeq    fp, r4, #0x180
0080fd7c str      r3, [r4, #0x184]
0080fd80 str      r1, [r4, #0x194]
0080fd84 str      r0, [r4, #0x180]
0080fd88 str      r1, [r4, #0x190]
0080fd8c str      r2, [r4, #0x198]
0080fd90 strb     r2, [r4, #0x19c]
0080fd94 streq    fp, [sp, #4]
0080fd98 beq      #0x80fdb0
0080fd9c add      r3, r4, #0x180
0080fda0 str      r3, [sp, #4]
0080fda4 str      r2, [r4, #0x1a0]
0080fda8 ldr      r0, [sp, #4]
0080fdac bl       #0x814f84
0080fdb0 ldr      r3, [r4, #0x1c8]
0080fdb4 ldr      r0, [r5, r8]
0080fdb8 ldr      r1, [r5, r6]
0080fdbc cmp      r3, #0
0080fdc0 add      r0, r0, #8
0080fdc4 mov      sl, #0
0080fdc8 mov      fp, #0
0080fdcc mov      ip, #0x1b0
0080fdd0 strd     sl, fp, [r4, ip]
0080fdd4 mvn      r2, #0
0080fdd8 mov      r3, #0
0080fddc str      r0, [r4, #0x180]
0080fde0 add      r1, r1, #8
0080fde4 mov      r0, #0x10
0080fde8 addeq    r8, r4, #0x1a8
0080fdec str      r0, [r4, #0x1ac]
0080fdf0 str      r2, [r4, #0x1bc]
0080fdf4 str      r1, [r4, #0x1a8]
0080fdf8 str      r2, [r4, #0x1b8]
0080fdfc str      r3, [r4, #0x1c0]
0080fe00 strb     r3, [r4, #0x1c4]
0080fe04 streq    r8, [sp, #0x14]
0080fe08 beq      #0x80fe20
0080fe0c add      sb, r4, #0x1a8
0080fe10 str      sb, [sp, #0x14]
0080fe14 str      r3, [r4, #0x1c8]
0080fe18 ldr      r0, [sp, #0x14]
0080fe1c bl       #0x814f84
0080fe20 ldr      r0, [pc, #0x264]
0080fe24 ldr      r8, [pc, #0x264]
0080fe28 ldr      r3, [r4, #0x1f0]
0080fe2c ldr      r0, [r5, r0]
0080fe30 ldr      r1, [r5, r8]
0080fe34 cmp      r3, #0
0080fe38 mov      fp, #0
0080fe3c add      r0, r0, #8
0080fe40 mov      ip, #0x1d8
0080fe44 mov      sl, #0
0080fe48 strd     sl, fp, [r4, ip]
0080fe4c mvn      r2, #0
0080fe50 mov      r3, #0
0080fe54 str      r0, [r4, #0x1a8]
0080fe58 add      r1, r1, #8
0080fe5c mov      r0, #8
0080fe60 addeq    fp, r4, #0x1d0
0080fe64 str      r0, [r4, #0x1d4]
0080fe68 str      r2, [r4, #0x1e4]
0080fe6c str      r1, [r4, #0x1d0]
0080fe70 str      r2, [r4, #0x1e0]
0080fe74 str      r3, [r4, #0x1e8]
0080fe78 strb     r3, [r4, #0x1ec]
0080fe7c streq    fp, [sp, #0x1c]
0080fe80 beq      #0x80fe98
0080fe84 add      r2, r4, #0x1d0
0080fe88 str      r2, [sp, #0x1c]
0080fe8c str      r3, [r4, #0x1f0]
0080fe90 ldr      r0, [sp, #0x1c]
0080fe94 bl       #0x814f84
0080fe98 ldr      r6, [pc, #0x1f4]
0080fe9c ldr      r3, [r4, #0x218]
0080fea0 ldr      r1, [r5, r8]
0080fea4 ldr      r0, [r5, r6]
0080fea8 cmp      r3, #0
0080feac mov      fp, #0
0080feb0 add      r0, r0, #8
0080feb4 mov      ip, #0x200
0080feb8 mov      sl, #0
0080febc strd     sl, fp, [r4, ip]
0080fec0 mvn      r2, #0
0080fec4 mov      r3, #0
0080fec8 str      r0, [r4, #0x1d0]
0080fecc add      r1, r1, #8
0080fed0 mov      r0, #8
0080fed4 addeq    fp, r4, #0x1f8
0080fed8 str      r0, [r4, #0x1fc]
0080fedc str      r2, [r4, #0x20c]
0080fee0 str      r1, [r4, #0x1f8]
0080fee4 str      r2, [r4, #0x208]
0080fee8 str      r3, [r4, #0x210]
0080feec strb     r3, [r4, #0x214]
0080fef0 streq    fp, [sp, #0x18]
0080fef4 beq      #0x80ff0c
0080fef8 add      r2, r4, #0x1f8
0080fefc str      r2, [sp, #0x18]
0080ff00 str      r3, [r4, #0x218]
0080ff04 ldr      r0, [sp, #0x18]
0080ff08 bl       #0x814f84
0080ff0c ldr      r0, [r5, r6]
0080ff10 ldr      r3, [r4, #0x240]
0080ff14 ldr      r1, [r5, r8]
0080ff18 add      r0, r0, #8
0080ff1c mov      sl, #0
0080ff20 mov      fp, #0
0080ff24 mov      ip, #0x228
0080ff28 strd     sl, fp, [r4, ip]
0080ff2c cmp      r3, #0
0080ff30 mvn      r2, #0
0080ff34 mov      r3, #0
0080ff38 add      r1, r1, #8
0080ff3c str      r0, [r4, #0x1f8]
0080ff40 mov      r0, #8
0080ff44 str      r0, [r4, #0x224]
0080ff48 str      r2, [r4, #0x234]
0080ff4c str      r1, [r4, #0x220]
0080ff50 str      r2, [r4, #0x230]
0080ff54 str      r3, [r4, #0x238]
0080ff58 strb     r3, [r4, #0x23c]
0080ff5c addeq    sl, r4, #0x220
0080ff60 beq      #0x80ff74
0080ff64 add      sl, r4, #0x220
0080ff68 str      r3, [r4, #0x240]
0080ff6c mov      r0, sl
0080ff70 bl       #0x814f84
0080ff74 ldr      r3, [r5, r6]
0080ff78 ldr      r1, [pc, #0x118]
0080ff7c add      r6, sp, #0x24
0080ff80 add      r3, r3, #8
0080ff84 add      r1, pc, r1
0080ff88 str      r3, [r4, #0x220]
0080ff8c mov      r2, r1
0080ff90 mov      r0, r6
0080ff94 add      r8, r4, #0x248
0080ff98 str      r6, [sp, #0x34]
0080ff9c str      r6, [sp, #0x38]
0080ffa0 bl       #0x3116e8
0080ffa4 mov      r0, r8
0080ffa8 mov      r1, r6
0080ffac bl       #0x371bec
0080ffb0 ldr      r0, [sp, #0x38]
0080ffb4 cmp      r0, r6
0080ffb8 beq      #0x80ffd8
0080ffbc cmp      r0, #0
0080ffc0 beq      #0x80ffd8
0080ffc4 ldr      r1, [sp, #0x24]
0080ffc8 rsb      r1, r0, r1
0080ffcc cmp      r1, #0x80
0080ffd0 bhi      #0x810068
0080ffd4 bl       #0x8be338
0080ffd8 mov      r3, #0
0080ffdc str      r3, [r4, #0x280]
0080ffe0 ldr      r1, [sp, #8]
0080ffe4 mov      r0, r4
0080ffe8 bl       #0x81324c
0080ffec mov      r0, r4
0080fff0 ldr      r1, [sp, #0xc]
0080fff4 bl       #0x81324c
0080fff8 mov      r0, r4
0080fffc ldr      r1, [sp, #4]
00810000 bl       #0x81324c
00810004 mov      r0, r4
00810008 ldr      r1, [sp, #0x14]
0081000c bl       #0x81324c
00810010 mov      r0, r4
00810014 ldr      r1, [sp, #0x1c]
00810018 bl       #0x81324c
0081001c mov      r0, r4
00810020 ldr      r1, [sp, #0x18]
00810024 bl       #0x81324c
00810028 mov      r0, r4
0081002c mov      r1, sl
00810030 bl       #0x81324c
00810034 mov      r0, r4
00810038 mov      r1, r8
0081003c bl       #0x81324c
00810040 mov      r0, r4
00810044 bl       #0x80f27c
00810048 ldr      r3, [r5, r7]
0081004c ldr      r2, [sp, #0x3c]
00810050 mov      r0, r4
00810054 ldr      r3, [r3]
00810058 cmp      r2, r3
0081005c bne      #0x810070
00810060 add      sp, sp, #0x44
00810064 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00810068 bl       #0x310440
0081006c b        #0x80ffd8
00810070 bl       #0x30e310
00810074 andseq   r4, r8, r8, asr lr
00810078 andeq    r4, r0, ip, lsr #1
0081007c andeq    r2, r0, r4, lsl #19
00810080 muleq    r0, r4, r4
00810084 andeq    r3, r0, ip, lsr #16
00810088 andeq    r1, r0, r0, asr r5
0081008c andeq    r3, r0, ip, lsr r5
00810090 andeq    r4, r0, r8, rrx
00810094 andeq    r1, r0, r4, lsr #32
00810098 andeq    fp, fp, r4, lsl #17
0x80f1a0 _ZN14CNetPlayerInfo9IsLocalToEi
0080f1a0 push     {r4, r5, r6, lr}
0080f1a4 mov      r4, r1
0080f1a8 mov      r5, r0
0080f1ac bl       #0x800f8c
0080f1b0 ldr      r3, [r0]
0080f1b4 mov      lr, pc
0080f1b8 ldr      pc, [r3, #0x74]
0080f1bc cmp      r0, r4
0080f1c0 ldrne    r0, [r5, #0x1a0]
0080f1c4 beq      #0x80f1d8
0080f1c8 cmp      r0, r4
0080f1cc movne    r0, #0
0080f1d0 moveq    r0, #1
0080f1d4 pop      {r4, r5, r6, pc}
0080f1d8 ldr      r0, [r5, #0x1a0]
0080f1dc cmp      r0, #0
0080f1e0 bge      #0x80f1c8
0080f1e4 mov      r0, #1
0080f1e8 pop      {r4, r5, r6, pc}
0x3ff200 _ZN13ItemInventoryC1Ev
003ff200 ldr      r3, [pc, #0x120]
003ff204 ldr      r2, [pc, #0x120]
003ff208 push     {r4, r5, r6, r7, r8, sb, sl, lr}
003ff20c add      r3, pc, r3
003ff210 ldr      r2, [r3, r2]
003ff214 mov      r6, r0
003ff218 mov      r1, #0
003ff21c add      r2, r2, #8
003ff220 str      r2, [r6]
003ff224 mvn      r2, #0x80000000
003ff228 sub      sp, sp, #0x10
003ff22c add      r0, r0, #0x30
003ff230 str      r2, [r6, #0x28]
003ff234 mvn      r2, #0
003ff238 mov      r7, r1
003ff23c strb     r2, [r6, #0x2c]
003ff240 str      r0, [r6, #0x34]
003ff244 str      r1, [r6, #4]
003ff248 str      r1, [r6, #8]
003ff24c str      r1, [r6, #0xc]
003ff250 str      r1, [r6, #0x10]
003ff254 str      r1, [r6, #0x14]
003ff258 str      r1, [r6, #0x18]
003ff25c str      r1, [r6, #0x1c]
003ff260 str      r1, [r6, #0x20]
003ff264 str      r1, [r6, #0x24]
003ff268 strb     r1, [r6, #0x2d]
003ff26c strb     r1, [r6, #0x2e]
003ff270 strb     r1, [r6, #0x2f]
003ff274 str      r0, [r6, #0x30]
003ff278 add      sl, r6, #0x14
003ff27c mov      r8, sp
003ff280 mov      r5, r1
003ff284 add      sb, sp, #0xc
003ff288 mov      r0, sl
003ff28c mov      r1, sp
003ff290 str      r5, [sp]
003ff294 str      r5, [sp, #4]
003ff298 str      r5, [sp, #8]
003ff29c bl       #0x3ff178
003ff2a0 ldr      r0, [sp]
003ff2a4 cmp      r0, #0
003ff2a8 beq      #0x3ff2c4
003ff2ac ldr      r1, [sp, #8]
003ff2b0 rsb      r1, r0, r1
003ff2b4 bic      r1, r1, #3
003ff2b8 cmp      r1, #0x80
003ff2bc bhi      #0x3ff320
003ff2c0 bl       #0x708f00
003ff2c4 mov      r4, #0
003ff2c8 ldr      r0, [r6, #0x14]
003ff2cc str      r5, [sp, #0xc]
003ff2d0 add      r0, r0, r7
003ff2d4 ldmib    r0, {r1, r3}
003ff2d8 cmp      r1, r3
003ff2dc beq      #0x3ff314
003ff2e0 str      r5, [r1]
003ff2e4 ldr      r3, [r0, #4]
003ff2e8 add      r3, r3, #4
003ff2ec str      r3, [r0, #4]
003ff2f0 add      r4, r4, #1
003ff2f4 cmp      r4, #9
003ff2f8 bne      #0x3ff2c8
003ff2fc add      r7, r7, #0xc
003ff300 cmp      r7, #0x18
003ff304 bne      #0x3ff288
003ff308 mov      r0, r6
003ff30c add      sp, sp, #0x10
003ff310 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003ff314 mov      r2, sb
003ff318 bl       #0x3fef4c
003ff31c b        #0x3ff2f0
003ff320 bl       #0x310440
003ff324 b        #0x3ff2c4
003ff328 subseq   r5, sb, r4, lsl #17
003ff32c strdeq   r2, r3, [r0], -ip
0x3fdfd8 _ZN13ItemInventory7SetGoldEi
003fdfd8 push     {r4, r5, r6, lr}
003fdfdc ldr      r4, [pc, #0x154]
003fdfe0 subs     r6, r1, #0
003fdfe4 sub      sp, sp, #8
003fdfe8 mov      r5, r0
003fdfec add      r4, pc, r4
003fdff0 blt      #0x3fe0e0
003fdff4 ldr      r2, [pc, #0x140]
003fdff8 ldr      r1, [r5, #0x28]
003fdffc ldr      r3, [r5, #4]
003fe000 ldr      r2, [r4, r2]
003fe004 cmp      r6, r1
003fe008 strle    r6, [r5, #0x20]
003fe00c strgt    r1, [r5, #0x20]
003fe010 cmp      r3, #0
003fe014 ldr      r6, [r2]
003fe018 beq      #0x3fe034
003fe01c mov      r0, r3
003fe020 ldr      r3, [r3]
003fe024 mov      lr, pc
003fe028 ldr      pc, [r3, #0x28]
003fe02c cmp      r0, #0
003fe030 bne      #0x3fe03c
003fe034 add      sp, sp, #8
003fe038 pop      {r4, r5, r6, pc}
003fe03c ldr      r3, [pc, #0xfc]
003fe040 ldr      r1, [r5, #4]
003fe044 ldr      r3, [r4, r3]
003fe048 ldr      r0, [r3, #0x40]
003fe04c bl       #0x36effc
003fe050 cmp      r0, #0
003fe054 beq      #0x3fe034
003fe058 ldr      r2, [r5, #0x20]
003fe05c movw     r3, #0x270f
003fe060 cmp      r2, r3
003fe064 ble      #0x3fe034
003fe068 ldr      r0, [pc, #0xd4]
003fe06c add      r0, pc, r0
003fe070 bl       #0x3a3f70
003fe074 mov      r1, r0
003fe078 mov      r0, r6
003fe07c bl       #0x3813b8
003fe080 ldr      r2, [r5, #0x20]
003fe084 movw     r3, #0x869f
003fe088 movt     r3, #1
003fe08c cmp      r2, r3
003fe090 ble      #0x3fe034
003fe094 ldr      r0, [pc, #0xac]
003fe098 add      r0, pc, r0
003fe09c bl       #0x3a3f70
003fe0a0 mov      r1, r0
003fe0a4 mov      r0, r6
003fe0a8 bl       #0x3813b8
003fe0ac ldr      r2, [r5, #0x20]
003fe0b0 movw     r3, #0x423f
003fe0b4 movt     r3, #0xf
003fe0b8 cmp      r2, r3
003fe0bc ble      #0x3fe034
003fe0c0 ldr      r0, [pc, #0x84]
003fe0c4 add      r0, pc, r0
003fe0c8 bl       #0x3a3f70
003fe0cc mov      r1, r0
003fe0d0 mov      r0, r6
003fe0d4 add      sp, sp, #8
003fe0d8 pop      {r4, r5, r6, lr}
003fe0dc b        #0x3813b8
003fe0e0 ldr      r3, [pc, #0x68]
003fe0e4 ldr      r3, [r4, r3]
003fe0e8 ldr      r3, [r3]
003fe0ec cmp      r3, #2
003fe0f0 moveq    r3, #0
003fe0f4 streq    r3, [r3]
003fe0f8 beq      #0x3fdff4
003fe0fc cmp      r3, #1
003fe100 bne      #0x3fdff4
003fe104 ldr      r0, [pc, #0x48]
003fe108 ldr      r1, [pc, #0x48]
003fe10c ldr      r2, [pc, #0x48]
003fe110 ldr      r0, [r4, r0]
003fe114 ldr      r3, [pc, #0x44]
003fe118 movw     ip, #0x142
003fe11c add      r1, pc, r1
003fe120 add      r2, pc, r2
003fe124 add      r3, pc, r3
003fe128 add      r0, r0, #0xa8
003fe12c str      ip, [sp]
003fe130 bl       #0x30e004
003fe134 b        #0x3fdff4
003fe138 subseq   r6, sb, r4, lsr #21
003fe13c andeq    r1, r0, r0, ror sp
003fe140 strdeq   r3, r4, [r0], -r4
003fe144 subeq    sb, ip, ip, lsr #7
003fe148 umaaleq  sb, ip, r0, r3
003fe14c subeq    sb, ip, r4, ror r3
003fe150 andeq    r3, r0, r0, asr #19
003fe154 andeq    r1, r0, r0, asr #19
003fe158 strheq   r0, [ip], #-0x2c
003fe15c ldrdeq   r6, r7, [ip], #-0x70
003fe160 subeq    sb, ip, r4, lsl #5
0x8137ec _ZN9NetStruct16GetChangedBitMapEv
008137ec push     {r4, r5, r6, r7, r8, lr}
008137f0 ldr      r3, [r0, #0x104]
008137f4 mov      r5, r0
008137f8 cmp      r3, #0
008137fc movle    r6, #0
00813800 ble      #0x813834
00813804 mov      r4, #0
00813808 mov      r6, r4
0081380c mov      r7, #1
00813810 add      r3, r5, r4, lsl #2
00813814 ldr      r0, [r3, #4]
00813818 bl       #0x814f70
0081381c ldr      r3, [r5, #0x104]
00813820 cmp      r0, #0
00813824 orrne    r6, r6, r7, lsl r4
00813828 add      r4, r4, #1
0081382c cmp      r3, r4
00813830 bgt      #0x813810
00813834 mov      r0, r6
00813838 pop      {r4, r5, r6, r7, r8, pc}
0x36effc _ZN13PlayerManager13IsLocalPlayerEPK9Character
0036effc subs     r3, r1, #0
0036f000 push     {r4, lr}
0036f004 beq      #0x36f020
0036f008 mov      r2, #0
0036f00c bl       #0x36eea8
0036f010 ldr      r3, [r0]
0036f014 mov      lr, pc
0036f018 ldr      pc, [r3, #0x50]
0036f01c pop      {r4, pc}
0036f020 mov      r0, r3
0036f024 pop      {r4, pc}
0x80f27c _ZN14CNetPlayerInfo5ResetEv
0080f27c push     {r4, lr}
0080f280 ldr      r3, [r0, #0x1a0]
0080f284 mov      r4, r0
0080f288 cmn      r3, #1
0080f28c beq      #0x80f2a0
0080f290 mvn      r3, #0
0080f294 str      r3, [r0, #0x1a0]
0080f298 add      r0, r0, #0x180
0080f29c bl       #0x814f84
0080f2a0 ldr      r3, [r4, #0x1c8]
0080f2a4 cmn      r3, #1
0080f2a8 beq      #0x80f2bc
0080f2ac mvn      r3, #0
0080f2b0 str      r3, [r4, #0x1c8]
0080f2b4 add      r0, r4, #0x1a8
0080f2b8 bl       #0x814f84
0080f2bc ldr      r3, [r4, #0x1f0]
0080f2c0 cmp      r3, #0
0080f2c4 beq      #0x80f2dc
0080f2c8 mov      r3, #0
0080f2cc add      r0, r4, #0x1d0
0080f2d0 str      r3, [r4, #0x1f0]
0080f2d4 pop      {r4, lr}
0080f2d8 b        #0x814f84
0080f2dc pop      {r4, pc}
0x8074a4 _ZN14CMatchingLocal5ResetEv
008074a4 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
008074a8 ldr      r4, [pc, #0x490]
008074ac sub      sp, sp, #0x164
008074b0 ldr      r8, [pc, #0x48c]
008074b4 ldrb     r2, [sp, #0x125]
008074b8 add      r4, pc, r4
008074bc ldr      r3, [r4, r8]
008074c0 mov      r5, r0
008074c4 cmp      r2, #1
008074c8 movw     ip, #0x465c
008074cc mov      r2, #0
008074d0 str      r2, [r5, ip]
008074d4 movw     ip, #0x3630
008074d8 strb     r2, [r5, ip]
008074dc add      r0, r3, #8
008074e0 mvn      r1, #0
008074e4 mov      r3, #1
008074e8 mov      r6, #0
008074ec mov      r7, #0
008074f0 add      ip, sp, #0x110
008074f4 strd     r6, r7, [ip]
008074f8 str      r1, [sp, #0x11c]
008074fc strb     r2, [sp, #0x124]
00807500 str      r0, [sp, #0x108]
00807504 str      r3, [sp, #0x10c]
00807508 str      r1, [sp, #0x118]
0080750c str      r2, [sp, #0x120]
00807510 addeq    sl, sp, #0x108
00807514 beq      #0x807528
00807518 add      sl, sp, #0x108
0080751c mov      r0, sl
00807520 strb     r3, [sp, #0x125]
00807524 bl       #0x814f84
00807528 ldr      r7, [pc, #0x418]
0080752c ldr      r1, [pc, #0x418]
00807530 add      r6, r5, #0x4b00
00807534 ldr      r2, [r4, r7]
00807538 str      r1, [sp, #0xc]
0080753c movw     r3, #0x4b50
00807540 add      r2, r2, #8
00807544 ldr      r3, [r5, r3]
00807548 add      r1, sl, #0x1d
0080754c str      r2, [sp, #0x108]
00807550 add      r0, r6, #0x50
00807554 mov      lr, pc
00807558 ldr      pc, [r3, #0x1c]
0080755c ldr      r2, [sp, #0xc]
00807560 ldr      r0, [r4, r8]
00807564 ldrb     r3, [sp, #0x105]
00807568 ldr      ip, [r4, r2]
0080756c mvn      r1, #0
00807570 cmp      r3, #1
00807574 mov      r2, #0
00807578 mov      r8, #0
0080757c add      r0, r0, #8
00807580 add      ip, ip, #8
00807584 mov      r3, #1
00807588 mov      sb, #0
0080758c strd     r8, sb, [sp, #0xf0]
00807590 str      ip, [sp, #0x108]
00807594 str      r1, [sp, #0xfc]
00807598 strb     r2, [sp, #0x104]
0080759c str      r0, [sp, #0xe8]
008075a0 str      r3, [sp, #0xec]
008075a4 str      r1, [sp, #0xf8]
008075a8 str      r2, [sp, #0x100]
008075ac addeq    r8, sp, #0xe8
008075b0 beq      #0x8075c4
008075b4 add      r8, sp, #0xe8
008075b8 mov      r0, r8
008075bc strb     r3, [sp, #0x105]
008075c0 bl       #0x814f84
008075c4 ldr      r3, [r4, r7]
008075c8 ldr      ip, [pc, #0x380]
008075cc add      r1, r8, #0x1d
008075d0 add      r3, r3, #8
008075d4 str      r3, [sp, #0xe8]
008075d8 str      ip, [sp, #0x14]
008075dc movw     r3, #0x4b70
008075e0 ldr      r3, [r5, r3]
008075e4 add      r0, r6, #0x70
008075e8 mov      lr, pc
008075ec ldr      pc, [r3, #0x1c]
008075f0 ldr      r1, [sp, #0xc]
008075f4 ldr      r2, [sp, #0x14]
008075f8 ldr      r3, [sp, #0xe0]
008075fc ldr      r0, [r4, r1]
00807600 ldr      r1, [r4, r2]
00807604 cmp      r3, #0
00807608 add      r0, r0, #8
0080760c mvn      r2, #0
00807610 mov      r3, #0
00807614 add      r1, r1, #8
00807618 str      r0, [sp, #0xe8]
0080761c mov      r8, #0
00807620 mov      r0, #0x20
00807624 mov      sb, #0
00807628 str      r0, [sp, #0xc4]
0080762c strd     r8, sb, [sp, #0xc8]
00807630 str      r2, [sp, #0xd4]
00807634 str      r1, [sp, #0xc0]
00807638 str      r2, [sp, #0xd0]
0080763c str      r3, [sp, #0xd8]
00807640 strb     r3, [sp, #0xdc]
00807644 addeq    r7, sp, #0xc0
00807648 beq      #0x80765c
0080764c add      r7, sp, #0xc0
00807650 mov      r0, r7
00807654 str      r3, [sp, #0xe0]
00807658 bl       #0x814f84
0080765c ldr      r3, [pc, #0x2f0]
00807660 add      r1, r7, #0x20
00807664 add      r0, r6, #0xb8
00807668 str      r3, [sp, #0x10]
0080766c ldr      ip, [sp, #0x10]
00807670 movw     r3, #0x4bb8
00807674 ldr      r3, [r5, r3]
00807678 ldr      r2, [r4, ip]
0080767c movw     sb, #0x7ee4
00807680 mvn      sl, #0
00807684 add      r2, r2, #8
00807688 str      r2, [sp, #0xc0]
0080768c mov      lr, pc
00807690 ldr      pc, [r3, #0x1c]
00807694 ldr      r1, [sp, #0xc]
00807698 ldr      r2, [r5, sb]
0080769c ldr      r3, [r4, r1]
008076a0 cmp      r2, #0
008076a4 mvn      r1, #1
008076a8 add      r3, r3, #8
008076ac movw     r2, #0x363c
008076b0 str      r1, [r5, r2]
008076b4 str      r3, [sp, #0xc0]
008076b8 movw     r3, #0x3638
008076bc str      sl, [r5, r3]
008076c0 ble      #0x80778c
008076c4 ldr      r2, [sp, #0x14]
008076c8 add      fp, sp, #0x98
008076cc add      r7, r5, #0x8000
008076d0 ldr      r3, [r4, r2]
008076d4 mov      r6, #0
008076d8 add      r7, r7, #0x18
008076dc add      r3, r3, #8
008076e0 str      r3, [sp, #4]
008076e4 add      r3, fp, #0x20
008076e8 mov      r8, r6
008076ec str      r3, [sp, #8]
008076f0 ldr      r3, [sp, #0xb8]
008076f4 ldr      ip, [sp, #4]
008076f8 mov      r1, #0x20
008076fc cmn      r3, #3
00807700 mov      r2, #0
00807704 mov      r3, #0
00807708 mov      r0, fp
0080770c str      ip, [sp, #0x98]
00807710 str      r1, [sp, #0x9c]
00807714 strd     r2, r3, [sp, #0xa0]
00807718 str      sl, [sp, #0xa8]
0080771c str      sl, [sp, #0xac]
00807720 str      r8, [sp, #0xb0]
00807724 strb     r8, [sp, #0xb4]
00807728 beq      #0x807738
0080772c mvn      r3, #2
00807730 str      r3, [sp, #0xb8]
00807734 bl       #0x814f84
00807738 ldr      r1, [sp, #0x10]
0080773c mov      ip, #0x158
00807740 mul      r0, ip, r6
00807744 ldr      r3, [r4, r1]
00807748 add      r0, r0, #0x8000
0080774c add      r0, r0, #0x18
00807750 add      r3, r3, #8
00807754 str      r3, [sp, #0x98]
00807758 ldr      r3, [r7], #0x158
0080775c add      r0, r5, r0
00807760 ldr      r1, [sp, #8]
00807764 mov      lr, pc
00807768 ldr      pc, [r3, #0x1c]
0080776c ldr      r2, [sp, #0xc]
00807770 add      r6, r6, #1
00807774 ldr      r3, [r4, r2]
00807778 ldr      r2, [r5, sb]
0080777c add      r3, r3, #8
00807780 cmp      r6, r2
00807784 str      r3, [sp, #0x98]
00807788 blt      #0x8076f0
0080778c add      r7, r5, #0x4600
00807790 add      r3, r7, #0x3c
00807794 mov      r0, r3
00807798 movw     r6, #0x4654
0080779c str      r3, [sp, #0x2c]
008077a0 bl       #0x80e36c
008077a4 ldr      r3, [r5, r6]
008077a8 cmp      r3, #0
008077ac bne      #0x80790c
008077b0 ldr      ip, [sp, #0x14]
008077b4 ldr      r1, [pc, #0x19c]
008077b8 add      r2, sp, #0x70
008077bc ldr      r3, [r4, ip]
008077c0 add      ip, sp, #0x30
008077c4 str      r1, [sp, #0x1c]
008077c8 add      r3, r3, #8
008077cc add      r7, r5, #0x4d00
008077d0 str      r2, [sp, #0x18]
008077d4 add      fp, sp, #0x128
008077d8 str      r3, [sp, #0x20]
008077dc add      r1, sp, #0x144
008077e0 add      r2, r2, #0x20
008077e4 add      r3, ip, #0x20
008077e8 mov      r6, #0
008077ec str      ip, [sp, #0x14]
008077f0 add      r7, r7, #0x38
008077f4 str      r1, [sp, #4]
008077f8 mvn      sb, #0
008077fc str      r2, [sp, #0x24]
00807800 str      fp, [sp, #8]
00807804 str      r3, [sp, #0x28]
00807808 mov      sl, r5
0080780c ldr      r3, [sp, #0x90]
00807810 ldr      ip, [sp, #0x20]
00807814 mov      r2, #0
00807818 cmn      r3, #1
0080781c mov      r3, #0
00807820 mov      r1, #0x20
00807824 strd     r2, r3, [sp, #0x78]
00807828 mov      r3, #0
0080782c ldr      r0, [sp, #0x18]
00807830 str      ip, [sp, #0x70]
00807834 str      r1, [sp, #0x74]
00807838 str      sb, [sp, #0x80]
0080783c str      sb, [sp, #0x84]
00807840 str      r3, [sp, #0x88]
00807844 strb     r3, [sp, #0x8c]
00807848 beq      #0x807854
0080784c str      sb, [sp, #0x90]
00807850 bl       #0x814f84
00807854 ldr      ip, [sp, #0x10]
00807858 mov      r1, #0x198
0080785c mul      r5, r1, r6
00807860 ldr      r3, [r4, ip]
00807864 add      r5, r5, #0x4d00
00807868 add      r0, r5, #0x10
0080786c add      r3, r3, #8
00807870 str      r3, [sp, #0x70]
00807874 ldr      r3, [r7, #-0x28]
00807878 ldr      r1, [sp, #0x24]
0080787c add      r0, sl, r0
00807880 mov      lr, pc
00807884 ldr      pc, [r3, #0x1c]
00807888 ldr      r2, [sp, #0xc]
0080788c ldr      r0, [sp, #4]
00807890 add      r6, r6, #1
00807894 ldr      r8, [r4, r2]
00807898 add      r8, r8, #8
0080789c str      r8, [sp, #0x70]
008078a0 bl       #0x7fc384
008078a4 ldr      ip, [sp, #4]
008078a8 ldm      ip!, {r0, r1, r2, r3}
008078ac stm      fp!, {r0, r1, r2, r3}
008078b0 ldm      ip, {r0, r1, r2}
008078b4 stm      fp, {r0, r1, r2}
008078b8 ldr      r1, [sp, #8]
008078bc ldr      r0, [sp, #0x14]
008078c0 bl       #0x8060d0
008078c4 ldr      ip, [sp, #0x1c]
008078c8 add      r0, r5, #0x38
008078cc add      r0, sl, r0
008078d0 ldr      r3, [r4, ip]
008078d4 ldr      r1, [sp, #0x28]
008078d8 add      r3, r3, #8
008078dc str      r3, [sp, #0x30]
008078e0 ldr      r3, [r7], #0x198
008078e4 mov      lr, pc
008078e8 ldr      pc, [r3, #0x1c]
008078ec cmp      r6, #0x20
008078f0 str      r8, [sp, #0x30]
008078f4 ldr      fp, [sp, #8]
008078f8 bne      #0x80780c
008078fc ldr      r0, [sp, #0x2c]
00807900 bl       #0x80e368
00807904 add      sp, sp, #0x164
00807908 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0080790c add      r7, r7, #0x44
00807910 movw     r8, #0x4648
00807914 mov      r0, r7
00807918 ldr      r1, [r5, r8]
0080791c bl       #0x80251c
00807920 movw     r2, #0x4650
00807924 str      r7, [r5, r2]
00807928 mov      r3, #0
0080792c movw     r2, #0x464c
00807930 str      r3, [r5, r6]
00807934 str      r7, [r5, r2]
00807938 str      r3, [r5, r8]
0080793c b        #0x8077b0
00807940 ldrsbeq  sp, [r8], -r8
00807944 andeq    r3, r0, r8, lsl r0
00807948 andeq    r0, r0, r8, asr #21
0080794c andeq    r1, r0, r8, lsr #1
00807950 andeq    r2, r0, r4, lsl #19
00807954 andeq    r1, r0, r8, asr #1
00807958 strdeq   r2, r3, [r0], -r0
0x8135e4 _ZN9NetStruct5EraseER12NetBitStream
008135e4 mov      r2, r1
008135e8 mvn      r3, #0
008135ec mov      r1, #0
008135f0 b        #0x8134f4
0x801b48 _ZNK14CMatchingLocal17GetServerMemberIdEv
00801b48 movw     r3, #0x363c
00801b4c ldr      r0, [r0, r3]
00801b50 bx       lr
0x801b30 _ZNK14CMatchingLocal11GetMemberIdEv
00801b30 movw     r3, #0x3638
00801b34 ldr      r0, [r0, r3]
00801b38 bx       lr
0x464b2c _ZN14PlayerSavegame7SG_SaveEv
00464b2c push     {r4, r5, r6, lr}
00464b30 ldr      r3, [r0, #8]
00464b34 ldr      r6, [pc, #0x158]
00464b38 sub      sp, sp, #8
00464b3c cmp      r3, #0
00464b40 mov      r4, r0
00464b44 add      r6, pc, r6
00464b48 beq      #0x464b58
00464b4c ldrb     r3, [r0, #0xc]
00464b50 cmp      r3, #0
00464b54 beq      #0x464b60
00464b58 add      sp, sp, #8
00464b5c pop      {r4, r5, r6, pc}
00464b60 bl       #0x7fd794
00464b64 ldrb     r3, [r0, #5]
00464b68 cmp      r3, #0
00464b6c bne      #0x464ba0
00464b70 mov      r3, #1
00464b74 str      r3, [r4, #0x178]
00464b78 bl       #0x7fd794
00464b7c ldrb     r3, [r0, #5]
00464b80 cmp      r3, #0
00464b84 bne      #0x464bd4
00464b88 ldr      r0, [r4, #8]
00464b8c bl       #0x315fb8
00464b90 mov      r0, r4
00464b94 add      sp, sp, #8
00464b98 pop      {r4, r5, r6, lr}
00464b9c b        #0x4684f0
00464ba0 ldr      r3, [pc, #0xf0]
00464ba4 ldr      r5, [r6, r3]
00464ba8 ldr      r0, [r5, #0x40]
00464bac bl       #0x36f074
00464bb0 cmp      r0, #0
00464bb4 beq      #0x464b70
00464bb8 ldr      r3, [r5, #0x40]
00464bbc ldrb     r3, [r3, #0x719]
00464bc0 cmp      r3, #0
00464bc4 bne      #0x464b70
00464bc8 mov      r3, #2
00464bcc str      r3, [r4, #0x178]
00464bd0 b        #0x464b78
00464bd4 ldr      r3, [pc, #0xbc]
00464bd8 ldr      r5, [r6, r3]
00464bdc ldr      r0, [r5, #0x40]
00464be0 bl       #0x36f074
00464be4 cmp      r0, #0
00464be8 bne      #0x464c74
00464bec mov      r1, #1
00464bf0 mov      r0, r4
00464bf4 bl       #0x463654
00464bf8 mov      r5, r0
00464bfc mov      r1, #1
00464c00 mov      r0, r4
00464c04 mov      r2, r5
00464c08 bl       #0x468630
00464c0c ldr      r0, [r4, #8]
00464c10 ldr      r1, [r0, #0x1c]
00464c14 cmp      r1, #0
00464c18 beq      #0x464c88
00464c1c bl       #0x315fb8
00464c20 mov      r0, r4
00464c24 mov      r1, #0
00464c28 mov      r2, r5
00464c2c bl       #0x468630
00464c30 cmp      r5, #0
00464c34 beq      #0x464b90
00464c38 ldr      r3, [pc, #0x5c]
00464c3c mov      r5, #0
00464c40 ldr      r0, [r4, #4]
00464c44 ldr      r3, [r6, r3]
00464c48 mov      r1, r5
00464c4c mov      ip, #1
00464c50 ldr      r3, [r3]
00464c54 mov      r2, r5
00464c58 str      ip, [sp]
00464c5c str      r5, [sp, #4]
00464c60 bl       #0x4626f4
00464c64 mov      r1, r5
00464c68 ldr      r0, [r4, #4]
00464c6c bl       #0x464a68
00464c70 b        #0x464b90
00464c74 ldr      r3, [r5, #0x40]
00464c78 ldrb     r3, [r3, #0x719]
00464c7c cmp      r3, #0
00464c80 beq      #0x464b88
00464c84 b        #0x464bec
00464c88 bl       #0x315ad0
00464c8c ldr      r0, [r4, #8]
00464c90 b        #0x464c1c
00464c94 subseq   pc, r2, ip, asr #30
00464c98 strdeq   r3, r4, [r0], -r4
00464c9c muleq    r0, ip, sl
0x3ec974 _ZN10ItemObject13DropInventoryER13ItemInventoryPK10GameObjectS4_PK9Character
003ec974 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ec978 ldr      r7, [pc, #0x144]
003ec97c subs     r4, r1, #0
003ec980 sub      sp, sp, #0x2c
003ec984 add      r7, pc, r7
003ec988 mov      r6, r0
003ec98c mov      r8, r2
003ec990 mov      sb, r3
003ec994 beq      #0x3eca70
003ec998 ldr      r1, [pc, #0x128]
003ec99c ldr      r2, [pc, #0x128]
003ec9a0 mov      r3, #0
003ec9a4 add      fp, r4, #0x160
003ec9a8 add      r5, sp, #0x1c
003ec9ac str      r3, [sp, #0x24]
003ec9b0 str      r3, [sp, #0x1c]
003ec9b4 str      r3, [sp, #0x20]
003ec9b8 str      r1, [sp, #0x10]
003ec9bc str      r2, [sp, #0x14]
003ec9c0 mov      r0, r6
003ec9c4 bl       #0x3fc608
003ec9c8 cmp      r0, #0
003ec9cc mov      r1, r4
003ec9d0 mov      r2, r8
003ec9d4 mov      r0, r5
003ec9d8 beq      #0x3eca68
003ec9dc bl       #0x3ec668
003ec9e0 ldr      r3, [sp, #0x10]
003ec9e4 mov      r1, r6
003ec9e8 mov      r2, #0
003ec9ec ldr      r0, [r7, r3]
003ec9f0 mov      r3, r4
003ec9f4 str      fp, [sp]
003ec9f8 stmib    sp, {r5, sb}
003ec9fc bl       #0x3eacd0
003eca00 ldr      r3, [r4]
003eca04 mov      sl, r0
003eca08 mov      r0, r4
003eca0c mov      lr, pc
003eca10 ldr      pc, [r3, #0x24]
003eca14 cmp      r0, #0
003eca18 beq      #0x3ec9c0
003eca1c ldr      r1, [sp, #0x14]
003eca20 mov      r2, #0
003eca24 ldr      r3, [r7, r1]
003eca28 mov      r1, r4
003eca2c ldr      r0, [r3, #0x40]
003eca30 bl       #0x36eea8
003eca34 ldr      r3, [r0, #0x678]
003eca38 movw     r1, #0x1388
003eca3c mov      r2, #0x3b8
003eca40 strh     r1, [sl, r2]
003eca44 mov      r2, #0x3c0
003eca48 strh     r3, [sl, r2]
003eca4c mov      r0, r6
003eca50 bl       #0x3fc608
003eca54 cmp      r0, #0
003eca58 mov      r1, r4
003eca5c mov      r2, r8
003eca60 mov      r0, r5
003eca64 bne      #0x3ec9dc
003eca68 add      sp, sp, #0x2c
003eca6c pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003eca70 ldr      r3, [pc, #0x58]
003eca74 ldr      r3, [r7, r3]
003eca78 ldr      r3, [r3]
003eca7c cmp      r3, #2
003eca80 streq    r4, [r4]
003eca84 beq      #0x3ec998
003eca88 cmp      r3, #1
003eca8c bne      #0x3ec998
003eca90 ldr      r0, [pc, #0x3c]
003eca94 ldr      r1, [pc, #0x3c]
003eca98 ldr      r2, [pc, #0x3c]
003eca9c ldr      r0, [r7, r0]
003ecaa0 ldr      r3, [pc, #0x38]
003ecaa4 mov      ip, #0xff
003ecaa8 add      r1, pc, r1
003ecaac add      r2, pc, r2
003ecab0 add      r3, pc, r3
003ecab4 add      r0, r0, #0xa8
003ecab8 str      ip, [sp]
003ecabc bl       #0x30e004
003ecac0 b        #0x3ec998
003ecac4 subseq   r8, sl, ip, lsl #2
003ecac8 andeq    r0, r0, ip, lsr #28
003ecacc strdeq   r3, r4, [r0], -r4
003ecad0 andeq    r3, r0, r0, asr #19
003ecad4 andeq    r1, r0, r0, asr #19
003ecad8 subeq    r1, sp, r0, lsr sb
003ecadc subeq    sb, sp, r4, ror #13
003ecae0 subeq    sb, sp, r0, lsl #15
0x8002bc _ZN9CMatching18UpdateMemberIdListEv
008002bc push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
008002c0 sub      sp, sp, #0xd4
008002c4 ldr      r3, [r0]
008002c8 mov      r7, r0
008002cc mov      lr, pc
008002d0 ldr      pc, [r3, #0x64]
008002d4 ldr      r8, [pc, #0x510]
008002d8 cmp      r0, #0
008002dc add      r8, pc, r8
008002e0 bne      #0x8002ec
008002e4 add      sp, sp, #0xd4
008002e8 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
008002ec mov      r0, r7
008002f0 bl       #0x7fe4e0
008002f4 cmp      r0, #0
008002f8 bne      #0x8003a4
008002fc ldr      r3, [r7, #0x1c]
00800300 cmp      r3, #0
00800304 ble      #0x8002e4
00800308 ldr      sl, [pc, #0x4e0]
0080030c mov      r4, r7
00800310 mov      r5, #0
00800314 add      sb, sp, #0xc8
00800318 b        #0x800338
0080031c cmp      r6, #0
00800320 blt      #0x800380
00800324 str      ip, [r4, #0x1c8]
00800328 ldr      r3, [r7, #0x1c]
0080032c cmp      r5, r3
00800330 add      r4, r4, #0x1b0
00800334 bge      #0x8002e4
00800338 ldr      ip, [r4, #0x170]
0080033c ldr      r6, [r4, #0x1c8]
00800340 add      r5, r5, #1
00800344 cmp      r6, ip
00800348 beq      #0x80032c
0080034c cmp      ip, #0
00800350 bge      #0x80031c
00800354 mov      r1, #0x800000
00800358 add      r1, r1, #7
0080035c ldr      r0, [r8, sl]
00800360 mov      r2, sb
00800364 mov      r3, #4
00800368 str      r6, [sp, #0xc8]
0080036c bl       #0x7fe204
00800370 ldr      r6, [r4, #0x1c8]
00800374 ldr      ip, [r4, #0x170]
00800378 cmp      r6, #0
0080037c bge      #0x800324
00800380 mov      r1, #0x800000
00800384 add      r1, r1, #6
00800388 ldr      r0, [r8, sl]
0080038c mov      r2, sb
00800390 mov      r3, #4
00800394 str      ip, [sp, #0xc8]
00800398 bl       #0x7fe204
0080039c ldr      ip, [r4, #0x170]
008003a0 b        #0x800324
008003a4 ldr      r3, [r7]
008003a8 mov      r0, r7
008003ac mov      lr, pc
008003b0 ldr      pc, [r3, #0x64]
008003b4 cmp      r0, #0
008003b8 beq      #0x8002fc
008003bc add      r0, sp, #0xbc
008003c0 str      r0, [sp, #0x10]
008003c4 bl       #0x7fbd74
008003c8 mov      r2, #0
008003cc mov      r1, r0
008003d0 ldr      r0, [sp, #0x10]
008003d4 bl       #0x7fd284
008003d8 ldr      r3, [r7]
008003dc mov      r0, r7
008003e0 mov      lr, pc
008003e4 ldr      pc, [r3, #0x6c]
008003e8 ldr      r1, [sp, #0xc0]
008003ec ldr      r3, [sp, #0xc4]
008003f0 str      r0, [sp, #0xcc]
008003f4 cmp      r1, r3
008003f8 beq      #0x8007d8
008003fc str      r0, [r1]
00800400 ldr      r2, [sp, #0xc0]
00800404 add      r2, r2, #4
00800408 str      r2, [sp, #0xc0]
0080040c ldr      r3, [sp, #0xbc]
00800410 rsb      r2, r3, r2
00800414 lsrs     r2, r2, #2
00800418 beq      #0x8004a0
0080041c ldr      r1, [pc, #0x3d0]
00800420 ldr      r2, [pc, #0x3d0]
00800424 ldr      r0, [pc, #0x3d0]
00800428 str      r1, [sp, #0x14]
0080042c ldr      r1, [pc, #0x3cc]
00800430 ldr      r6, [pc, #0x3cc]
00800434 str      r2, [sp, #4]
00800438 add      r2, sp, #0x90
0080043c str      r0, [sp, #8]
00800440 str      r1, [sp, #0xc]
00800444 mov      r4, #0
00800448 mov      r5, #0x1b0
0080044c add      sl, sp, #0x68
00800450 str      r2, [sp]
00800454 ldr      r1, [r3, r4, lsl #2]
00800458 mov      r0, r7
0080045c bl       #0x7fe59c
00800460 subs     sb, r0, #0
00800464 lsl      fp, r4, #2
00800468 blt      #0x800488
0080046c ldr      r3, [r7, #0x1c]
00800470 cmp      sb, r3
00800474 bge      #0x800488
00800478 mla      r3, r5, sb, r7
0080047c ldr      r3, [r3, #0x170]
00800480 cmp      r3, #0
00800484 blt      #0x800660
00800488 ldr      r3, [sp, #0xbc]
0080048c ldr      r2, [sp, #0xc0]
00800490 add      r4, r4, #1
00800494 rsb      r2, r3, r2
00800498 cmp      r4, r2, asr #2
0080049c blo      #0x800454
008004a0 ldr      r3, [r7, #0x1c]
008004a4 cmp      r3, #0
008004a8 ble      #0x80050c
008004ac ldr      r2, [pc, #0x340]
008004b0 ldr      r0, [pc, #0x340]
008004b4 ldr      r1, [pc, #0x340]
008004b8 str      r2, [sp, #0xc]
008004bc ldr      r2, [pc, #0x33c]
008004c0 ldr      r6, [pc, #0x33c]
008004c4 add      sl, sp, #0x18
008004c8 stm      sp, {r0, r1, r2}
008004cc mov      r4, r7
008004d0 mov      r5, #0
008004d4 add      fp, sp, #0x40
008004d8 mov      sb, sl
008004dc ldr      r1, [r4, #0x170]
008004e0 cmp      r1, #0
008004e4 blt      #0x8004fc
008004e8 mov      r0, r7
008004ec bl       #0x7fe544
008004f0 cmp      r0, #0
008004f4 beq      #0x800518
008004f8 ldr      r3, [r7, #0x1c]
008004fc add      r5, r5, #1
00800500 cmp      r5, r3
00800504 add      r4, r4, #0x1b0
00800508 blt      #0x8004dc
0080050c ldr      r0, [sp, #0x10]
00800510 bl       #0x43f1a0
00800514 b        #0x8002fc
00800518 bl       #0x7fbd74
0080051c ldr      r1, [r4, #0x170]
00800520 bl       #0x7fc6f4
00800524 subs     ip, r0, #0
00800528 bne      #0x8004f8
0080052c ldr      r3, [sp, #0xc]
00800530 ldr      r1, [sp, #0x60]
00800534 mov      r0, #0x20
00800538 ldr      r2, [r8, r3]
0080053c mvn      r3, #0
00800540 cmp      r1, r3
00800544 add      r2, r2, #8
00800548 str      r0, [sp, #0x44]
0080054c mov      r1, #0
00800550 mov      r0, #0
00800554 strb     ip, [sp, #0x5c]
00800558 str      r2, [sp, #0x40]
0080055c strd     r0, r1, [sp, #0x48]
00800560 str      r3, [sp, #0x50]
00800564 str      r3, [sp, #0x54]
00800568 str      ip, [sp, #0x58]
0080056c beq      #0x80057c
00800570 mov      r0, fp
00800574 str      r3, [sp, #0x60]
00800578 bl       #0x814f84
0080057c ldr      r1, [sp]
00800580 mov      r0, #0x1b0
00800584 mul      r0, r0, r5
00800588 ldr      r3, [r8, r1]
0080058c add      r0, r0, #0x150
00800590 add      r0, r7, r0
00800594 add      r3, r3, #8
00800598 str      r3, [sp, #0x40]
0080059c ldr      r3, [r4, #0x150]
008005a0 add      r1, fp, #0x20
008005a4 mov      lr, pc
008005a8 ldr      pc, [r3, #0x1c]
008005ac ldr      r2, [sp, #4]
008005b0 ldr      r0, [r8, r6]
008005b4 ldr      r3, [sp, #0x38]
008005b8 ldr      r1, [r8, r2]
008005bc add      r0, r0, #8
008005c0 str      r0, [sp, #0x40]
008005c4 add      r1, r1, #8
008005c8 mov      r0, #8
008005cc mvn      r2, #0
008005d0 cmp      r3, #0
008005d4 str      r0, [sp, #0x1c]
008005d8 mov      r3, #0
008005dc str      r1, [sp, #0x18]
008005e0 mov      r0, #0
008005e4 mov      r1, #0
008005e8 str      r2, [sp, #0x2c]
008005ec strd     r0, r1, [sp, #0x20]
008005f0 str      r2, [sp, #0x28]
008005f4 str      r3, [sp, #0x30]
008005f8 strb     r3, [sp, #0x34]
008005fc beq      #0x80060c
00800600 mov      r0, sb
00800604 str      r3, [sp, #0x38]
00800608 bl       #0x814f84
0080060c ldr      r1, [sp, #8]
00800610 mov      sl, #0x1b0
00800614 mul      sl, sl, r5
00800618 ldr      r3, [r8, r1]
0080061c add      r0, sl, #0x178
00800620 add      r0, r7, r0
00800624 add      r3, r3, #8
00800628 str      r3, [sp, #0x18]
0080062c add      r1, sb, #0x20
00800630 ldr      r3, [r4, #0x178]
00800634 mov      lr, pc
00800638 ldr      pc, [r3, #0x1c]
0080063c ldr      r3, [r8, r6]
00800640 add      r0, sl, #0x1a0
00800644 mov      r1, #0
00800648 add      r3, r3, #8
0080064c add      r0, r7, r0
00800650 mov      r2, r1
00800654 str      r3, [sp, #0x18]
00800658 bl       #0x7fefb8
0080065c b        #0x8004f8
00800660 ldr      r3, [sp, #0xbc]
00800664 mov      r0, r7
00800668 ldr      r1, [r3, fp]
0080066c bl       #0x7fe544
00800670 cmp      r0, #0
00800674 beq      #0x8007bc
00800678 ldr      r3, [sp, #0x14]
0080067c ldr      r2, [sp, #0xb0]
00800680 mov      r1, #0
00800684 ldr      r0, [r8, r3]
00800688 ldr      r3, [sp, #0xbc]
0080068c mvn      ip, #0
00800690 add      lr, r0, #8
00800694 ldr      r3, [r3, fp]
00800698 mov      r0, #0
0080069c strd     r0, r1, [sp, #0x98]
008006a0 cmp      r3, r2
008006a4 mov      r1, #0x20
008006a8 mov      r2, #0
008006ac str      ip, [sp, #0xa4]
008006b0 strb     r2, [sp, #0xac]
008006b4 str      lr, [sp, #0x90]
008006b8 str      r1, [sp, #0x94]
008006bc str      ip, [sp, #0xa0]
008006c0 str      r2, [sp, #0xa8]
008006c4 beq      #0x8006d4
008006c8 ldr      r0, [sp]
008006cc str      r3, [sp, #0xb0]
008006d0 bl       #0x814f84
008006d4 ldr      r2, [sp, #4]
008006d8 mul      r0, r5, sb
008006dc ldr      r3, [r8, r2]
008006e0 ldr      r2, [sp]
008006e4 add      r3, r3, #8
008006e8 str      r3, [sp, #0x90]
008006ec add      r3, r7, r0
008006f0 add      r0, r0, #0x150
008006f4 add      r1, r2, #0x20
008006f8 ldr      r3, [r3, #0x150]
008006fc add      r0, r7, r0
00800700 mov      lr, pc
00800704 ldr      pc, [r3, #0x1c]
00800708 ldr      r3, [sp, #8]
0080070c ldr      r0, [r8, r6]
00800710 mvn      r2, #0
00800714 ldr      r1, [r8, r3]
00800718 ldr      r3, [sp, #0x88]
0080071c add      r0, r0, #8
00800720 str      r0, [sp, #0x90]
00800724 mov      r0, #8
00800728 add      ip, r1, #8
0080072c cmp      r3, #0
00800730 str      r0, [sp, #0x6c]
00800734 mov      r3, #0
00800738 mov      r0, #0
0080073c mov      r1, #0
00800740 strd     r0, r1, [sp, #0x70]
00800744 str      r2, [sp, #0x7c]
00800748 str      ip, [sp, #0x68]
0080074c str      r2, [sp, #0x78]
00800750 str      r3, [sp, #0x80]
00800754 strb     r3, [sp, #0x84]
00800758 beq      #0x800768
0080075c mov      r0, sl
00800760 str      r3, [sp, #0x88]
00800764 bl       #0x814f84
00800768 ldr      r1, [sp, #0xc]
0080076c mul      sb, r5, sb
00800770 ldr      r3, [r8, r1]
00800774 add      r0, sb, #0x178
00800778 add      r0, r7, r0
0080077c add      r3, r3, #8
00800780 str      r3, [sp, #0x68]
00800784 add      r3, r7, sb
00800788 add      r1, sl, #0x20
0080078c ldr      r3, [r3, #0x178]
00800790 mov      lr, pc
00800794 ldr      pc, [r3, #0x1c]
00800798 ldr      r3, [r8, r6]
0080079c add      r0, sb, #0x1a0
008007a0 mov      r1, #0
008007a4 add      r3, r3, #8
008007a8 add      r0, r7, r0
008007ac mov      r2, r1
008007b0 str      r3, [sp, #0x68]
008007b4 bl       #0x7fefb8
008007b8 b        #0x800488
008007bc bl       #0x7fbd74
008007c0 ldr      r3, [sp, #0xbc]
008007c4 ldr      r1, [r3, fp]
008007c8 bl       #0x7fc6f4
008007cc cmp      r0, #0
008007d0 beq      #0x800488
008007d4 b        #0x800678
008007d8 add      r2, sp, #0xcc
008007dc ldr      r0, [sp, #0x10]
008007e0 bl       #0x8001f4
008007e4 ldr      r2, [sp, #0xc0]
008007e8 b        #0x80040c
008007ec ldrheq   r4, [sb], -r4
008007f0 andeq    r3, r0, ip, lsr r4
008007f4 andeq    r2, r0, r4, lsl #19
008007f8 andeq    r1, r0, r8, asr #1
008007fc andeq    r4, r0, r8, rrx
00800800 andeq    r1, r0, r4, lsr #32
00800804 andeq    r1, r0, r8, lsr #1
0x807d08 _ZN14CMatchingLocalC1Ev
00807d08 push     {r4, r5, r6, r7, r8, lr}
00807d0c ldr      r7, [pc, #0x124]
00807d10 mov      r6, r0
00807d14 bl       #0x80003c
00807d18 ldr      r3, [pc, #0x11c]
00807d1c add      r7, pc, r7
00807d20 mov      r1, #1
00807d24 ldr      r3, [r7, r3]
00807d28 movw     r2, #0x3634
00807d2c str      r1, [r6, r2]
00807d30 mvn      r1, #0
00807d34 movw     r2, #0x3638
00807d38 str      r1, [r6, r2]
00807d3c add      r3, r3, #8
00807d40 mvn      r1, #1
00807d44 movw     r2, #0x363c
00807d48 str      r1, [r6, r2]
00807d4c add      r5, r6, #0x4600
00807d50 mov      r4, #0
00807d54 str      r3, [r6]
00807d58 movw     r3, #0x3630
00807d5c strb     r4, [r6, r3]
00807d60 add      r0, r5, #0x38
00807d64 bl       #0x80e398
00807d68 add      r0, r5, #0x3c
00807d6c bl       #0x80e398
00807d70 add      r0, r5, #0x40
00807d74 bl       #0x80e398
00807d78 add      r3, r5, #0x44
00807d7c movw     r2, #0x4650
00807d80 str      r3, [r6, r2]
00807d84 movw     r2, #0x4648
00807d88 str      r4, [r6, r2]
00807d8c movw     r2, #0x4644
00807d90 strb     r4, [r6, r2]
00807d94 movw     r2, #0x464c
00807d98 str      r3, [r6, r2]
00807d9c movw     r3, #0x4654
00807da0 str      r4, [r6, r3]
00807da4 movw     r3, #0x465c
00807da8 str      r4, [r6, r3]
00807dac add      r0, r5, #0x60
00807db0 bl       #0x819184
00807db4 add      r0, r6, #0x4900
00807db8 add      r0, r0, #0xf8
00807dbc bl       #0x81923c
00807dc0 add      r0, r6, #0x4a00
00807dc4 add      r0, r0, #0x20
00807dc8 bl       #0x806964
00807dcc add      r5, r6, #0x4b00
00807dd0 add      r5, r5, #0xe0
00807dd4 add      r0, r5, r4
00807dd8 add      r4, r4, #0x198
00807ddc bl       #0x806b98
00807de0 cmp      r4, #0x3300
00807de4 bne      #0x807dd4
00807de8 ldr      r3, [pc, #0x50]
00807dec movw     r2, #0x7ee0
00807df0 add      r5, r6, #0x7e00
00807df4 ldr      r3, [r7, r3]
00807df8 add      r5, r5, #0xe8
00807dfc mov      r4, #0
00807e00 add      r3, r3, #8
00807e04 str      r3, [r6, r2]
00807e08 mov      r2, #0x20
00807e0c movw     r3, #0x7ee4
00807e10 str      r2, [r6, r3]
00807e14 add      r0, r5, r4
00807e18 add      r4, r4, #0x158
00807e1c bl       #0x806cac
00807e20 cmp      r4, #0x2b00
00807e24 bne      #0x807e14
00807e28 mov      r0, r6
00807e2c bl       #0x8074a4
00807e30 mov      r0, r6
00807e34 pop      {r4, r5, r6, r7, r8, pc}
00807e38 andseq   ip, r8, r4, ror sp
00807e3c andeq    r4, r0, ip
00807e40 andeq    r2, r0, r4, asr r6
0x806344 _ZN14CMatchingLocal19MemberInfoNetStructD0Ev
00806344 push     {r4, r5, r6, lr}
00806348 ldr      r3, [pc, #0x68]
0080634c ldr      r2, [pc, #0x68]
00806350 ldr      r1, [pc, #0x68]
00806354 add      r3, pc, r3
00806358 mov      r4, r0
0080635c ldr      r1, [r3, r1]
00806360 ldr      r0, [r0, #0x11c]
00806364 ldr      r2, [r3, r2]
00806368 add      r1, r1, #8
0080636c cmp      r0, #0
00806370 add      r2, r2, #8
00806374 str      r2, [r4, #0x130]
00806378 str      r1, [r4]
0080637c str      r2, [r4, #0x158]
00806380 beq      #0x8063a8
00806384 add      r5, r4, #0x10c
00806388 mov      r0, r5
0080638c ldr      r1, [r4, #0x110]
00806390 bl       #0x370fd0
00806394 mov      r3, #0
00806398 str      r5, [r4, #0x118]
0080639c str      r3, [r4, #0x11c]
008063a0 str      r5, [r4, #0x114]
008063a4 str      r3, [r4, #0x110]
008063a8 mov      r0, r4
008063ac bl       #0x310440
008063b0 mov      r0, r4
008063b4 pop      {r4, r5, r6, pc}
008063b8 andseq   lr, r8, ip, lsr r7
008063bc andeq    r1, r0, r8, lsr #1
008063c0 andeq    r4, r0, r4, asr #7
0x80f1ec _ZN14CNetPlayerInfo7IsLocalEv
0080f1ec push     {r4, lr}
0080f1f0 mov      r4, r0
0080f1f4 bl       #0x800f8c
0080f1f8 bl       #0x7fe4e0
0080f1fc cmp      r0, #0
0080f200 ldreq    r4, [r4, #0x1a0]
0080f204 beq      #0x80f214
0080f208 ldr      r4, [r4, #0x1a0]
0080f20c cmp      r4, #0
0080f210 blt      #0x80f234
0080f214 bl       #0x800f8c
0080f218 ldr      r3, [r0]
0080f21c mov      lr, pc
0080f220 ldr      pc, [r3, #0x6c]
0080f224 cmp      r4, r0
0080f228 movne    r0, #0
0080f22c moveq    r0, #1
0080f230 pop      {r4, pc}
0080f234 mov      r0, #1
0080f238 pop      {r4, pc}
