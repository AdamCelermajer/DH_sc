# _ZN15VoxSoundManager18PlaySoundPackSoundEiPKcN3vox11FormatTypesEiiNS2_21VoxSourceLoadingFlagsERKN6glitch4core8vector3dIfEEff
0036a7c0 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036a7c4 ldr      r5, [pc, #0x6e4]
0036a7c8 ldr      r3, [pc, #0x6e4]
0036a7cc sub      sp, sp, #0xcc
0036a7d0 add      r5, pc, r5
0036a7d4 ldr      r3, [r5, r3]
0036a7d8 mov      r4, r0
0036a7dc mov      r6, r1
0036a7e0 ldrb     r3, [r3]
0036a7e4 ldr      r7, [sp, #0xfc]
0036a7e8 ldr      sl, [sp, #0x100]
0036a7ec cmp      r3, #0
0036a7f0 ldr      sb, [sp, #0x104]
0036a7f4 bne      #0x36a9c0
0036a7f8 ldr      r3, [r0, #8]
0036a7fc ldr      r3, [r3, r1, lsl #2]
0036a800 cmp      r3, #0
0036a804 beq      #0x36aaf0
0036a808 mov      r1, r3
0036a80c ldr      r0, [r4]
0036a810 bl       #0x8624f8
0036a814 cmp      r0, #0
0036a818 beq      #0x36a9c0
0036a81c ldr      r3, [r4, #8]
0036a820 ldr      r2, [sp, #0xf4]
0036a824 ldr      r0, [r4]
0036a828 ldr      r1, [r3, r6, lsl #2]
0036a82c bl       #0x862618
0036a830 add      ip, sp, #0xc7
0036a834 str      ip, [sp]
0036a838 add      r8, r4, #0x64
0036a83c add      ip, sp, #0xb8
0036a840 mov      r1, r6
0036a844 add      r2, sp, #0xc0
0036a848 add      r3, sp, #0xbc
0036a84c str      ip, [sp, #4]
0036a850 mov      r0, r8
0036a854 add      ip, sp, #0xb4
0036a858 str      ip, [sp, #8]
0036a85c bl       #0x889894
0036a860 ldr      r3, [r4, #8]
0036a864 ldr      r1, [r4]
0036a868 mov      ip, #0
0036a86c ldr      r2, [r3, r6, lsl #2]
0036a870 add      r6, sp, #0x38
0036a874 ldr      r3, [sp, #0xc0]
0036a878 mov      r0, r6
0036a87c str      ip, [sp]
0036a880 bl       #0x862438
0036a884 ldr      r2, [sp, #0xb8]
0036a888 cmp      r2, #0
0036a88c beq      #0x36a9cc
0036a890 mov      r0, sl
0036a894 mov      r1, #0
0036a898 bl       #0x30e4b4
0036a89c cmp      r0, #0
0036a8a0 beq      #0x36a8b8
0036a8a4 mov      r0, sb
0036a8a8 mov      r1, #0
0036a8ac bl       #0x30e4b4
0036a8b0 cmp      r0, #0
0036a8b4 bne      #0x36ab08
0036a8b8 ldr      ip, [r7, #8]
0036a8bc ldr      r2, [r7]
0036a8c0 ldr      r3, [r7, #4]
0036a8c4 ldr      r0, [r4]
0036a8c8 mov      r1, r6
0036a8cc str      ip, [sp]
0036a8d0 bl       #0x861e48
0036a8d4 ldr      r0, [r4, #0x54]
0036a8d8 bl       #0x30e964
0036a8dc ldr      r7, [r4]
0036a8e0 mov      r3, r0
0036a8e4 mov      r1, r6
0036a8e8 mov      r0, r7
0036a8ec mov      r2, #2
0036a8f0 bl       #0x861d88
0036a8f4 ldr      r0, [r4, #0x58]
0036a8f8 bl       #0x30e964
0036a8fc ldr      r7, [r4]
0036a900 mov      r3, r0
0036a904 mov      r1, r6
0036a908 mov      r0, r7
0036a90c mov      r2, #1
0036a910 bl       #0x861d88
0036a914 ldr      r3, [r4, #0x5c]
0036a918 ldr      r0, [r4]
0036a91c mov      r1, r6
0036a920 mov      r2, #3
0036a924 bl       #0x861d88
0036a928 ldr      r3, [sp, #0xb8]
0036a92c cmp      r3, #2
0036a930 beq      #0x36ab50
0036a934 ldr      r7, [pc, #0x57c]
0036a938 ldr      r3, [sp, #0xb4]
0036a93c ldr      r0, [r4]
0036a940 mov      r1, r6
0036a944 add      r7, pc, r7
0036a948 mov      r2, #0
0036a94c bl       #0x861950
0036a950 ldr      r3, [r7]
0036a954 tst      r3, #1
0036a958 beq      #0x36aac0
0036a95c ldr      r7, [pc, #0x558]
0036a960 add      r7, pc, r7
0036a964 ldr      r3, [r7, #8]
0036a968 tst      r3, #1
0036a96c beq      #0x36aa8c
0036a970 ldr      r3, [pc, #0x548]
0036a974 ldr      r2, [sp, #0xbc]
0036a978 add      r3, pc, r3
0036a97c ldr      r1, [r3, #4]
0036a980 cmp      r2, r1
0036a984 beq      #0x36a9e0
0036a988 ldr      r3, [r3, #0xc]
0036a98c cmp      r2, r3
0036a990 beq      #0x36a9e0
0036a994 ldr      r0, [r4]
0036a998 mov      r1, r6
0036a99c bl       #0x862058
0036a9a0 movw     r3, #0xcccd
0036a9a4 ldr      r0, [r4]
0036a9a8 mov      r1, r6
0036a9ac ldrb     r2, [sp, #0xc7]
0036a9b0 movt     r3, #0x3d4c
0036a9b4 bl       #0x8621c0
0036a9b8 mov      r0, r6
0036a9bc bl       #0x8683ac
0036a9c0 mov      r0, #0
0036a9c4 add      sp, sp, #0xcc
0036a9c8 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036a9cc ldr      r0, [r4]
0036a9d0 mov      r1, r6
0036a9d4 mov      r3, #1
0036a9d8 bl       #0x861d60
0036a9dc b        #0x36a914
0036a9e0 ldr      r3, [pc, #0x4dc]
0036a9e4 ldr      r1, [pc, #0x4dc]
0036a9e8 ldr      r0, [r5, r3]
0036a9ec add      r1, pc, r1
0036a9f0 bl       #0x320e44
0036a9f4 mov      r7, r0
0036a9f8 bl       #0x30eda8
0036a9fc mov      r5, r0
0036aa00 mov      r0, r7
0036aa04 bl       #0x30e964
0036aa08 bl       #0x30e8a4
0036aa0c movw     r3, #0x215
0036aa10 movt     r3, #0x214d
0036aa14 smull    r2, r3, r3, r5
0036aa18 asr      r2, r5, #0x1f
0036aa1c rsb      r3, r2, r3, asr #4
0036aa20 mov      r8, r0
0036aa24 mov      r0, #0x7b
0036aa28 mls      r0, r0, r3, r5
0036aa2c mov      sb, r1
0036aa30 bl       #0x30ed30
0036aa34 movw     r2, #0xa9fc
0036aa38 movw     r3, #0x624d
0036aa3c movt     r2, #0xd2f1
0036aa40 movt     r3, #0x3f50
0036aa44 bl       #0x30eab4
0036aa48 movw     r2, #0x1eb8
0036aa4c movw     r3, #0xb851
0036aa50 movt     r2, #0xeb85
0036aa54 movt     r3, #0x3fee
0036aa58 bl       #0x30eb44
0036aa5c mov      r2, r0
0036aa60 mov      r3, r1
0036aa64 mov      r0, r8
0036aa68 mov      r1, sb
0036aa6c bl       #0x30eab4
0036aa70 bl       #0x30e6a0
0036aa74 ldr      r1, [sp, #0xbc]
0036aa78 mov      r2, r0
0036aa7c mov      r0, r4
0036aa80 bl       #0x369da0
0036aa84 ldr      r2, [sp, #0xbc]
0036aa88 b        #0x36a994
0036aa8c add      sl, r7, #8
0036aa90 mov      r0, sl
0036aa94 bl       #0x30e76c
0036aa98 cmp      r0, #0
0036aa9c beq      #0x36a970
0036aaa0 ldr      r1, [pc, #0x424]
0036aaa4 mov      r0, r8
0036aaa8 add      r1, pc, r1
0036aaac bl       #0x88b8b8
0036aab0 str      r0, [r7, #0xc]
0036aab4 mov      r0, sl
0036aab8 bl       #0x30ea3c
0036aabc b        #0x36a970
0036aac0 mov      r0, r7
0036aac4 bl       #0x30e76c
0036aac8 cmp      r0, #0
0036aacc beq      #0x36a95c
0036aad0 ldr      r1, [pc, #0x3f8]
0036aad4 mov      r0, r8
0036aad8 add      r1, pc, r1
0036aadc bl       #0x88b8b8
0036aae0 str      r0, [r7, #4]
0036aae4 mov      r0, r7
0036aae8 bl       #0x30ea3c
0036aaec b        #0x36a95c
0036aaf0 bl       #0x3699fc
0036aaf4 ldr      r3, [r4, #8]
0036aaf8 ldr      r3, [r3, r6, lsl #2]
0036aafc cmp      r3, #0
0036ab00 beq      #0x36a9c0
0036ab04 b        #0x36a808
0036ab08 ldr      ip, [r7, #8]
0036ab0c ldr      r0, [r4]
0036ab10 ldr      r2, [r7]
0036ab14 ldr      r3, [r7, #4]
0036ab18 mov      r1, r6
0036ab1c str      ip, [sp]
0036ab20 bl       #0x861e48
0036ab24 mov      r3, sl
0036ab28 ldr      r0, [r4]
0036ab2c mov      r1, r6
0036ab30 mov      r2, #2
0036ab34 bl       #0x861d88
0036ab38 mov      r3, sb
0036ab3c ldr      r0, [r4]
0036ab40 mov      r1, r6
0036ab44 mov      r2, #1
0036ab48 bl       #0x861d88
0036ab4c b        #0x36a914
0036ab50 ldr      r0, [r4]
0036ab54 mov      r1, r6
0036ab58 mov      r2, #0
0036ab5c mov      r3, #1
0036ab60 bl       #0x861d60
0036ab64 add      ip, sp, #0xa4
0036ab68 ldr      r0, [r4]
0036ab6c str      ip, [sp]
0036ab70 add      ip, sp, #0xa0
0036ab74 add      r1, sp, #0xb0
0036ab78 add      r2, sp, #0xac
0036ab7c add      r3, sp, #0xa8
0036ab80 str      ip, [sp, #4]
0036ab84 add      ip, sp, #0x9c
0036ab88 str      ip, [sp, #8]
0036ab8c bl       #0x861aa8
0036ab90 ldr      r3, [sp, #0xac]
0036ab94 ldr      r2, [sp, #0xa4]
0036ab98 add      r0, sp, #0x78
0036ab9c str      r3, [sp, #0x7c]
0036aba0 ldr      r3, [sp, #0xb0]
0036aba4 str      r2, [sp, #0x1c]
0036aba8 ldr      fp, [sp, #0x9c]
0036abac str      r3, [sp, #0x78]
0036abb0 ldr      r3, [sp, #0xa8]
0036abb4 str      r3, [sp, #0x80]
0036abb8 ldr      r3, [sp, #0xa0]
0036abbc str      r3, [sp, #0x18]
0036abc0 bl       #0x35e8e0
0036abc4 ldr      lr, [sp, #0x18]
0036abc8 ldr      sb, [r0, #8]
0036abcc ldr      sl, [r0, #4]
0036abd0 add      r1, lr, #0x80000000
0036abd4 mov      r3, r0
0036abd8 mov      r0, sb
0036abdc ldr      r7, [r3]
0036abe0 bl       #0x30ed6c
0036abe4 mov      r1, sl
0036abe8 mov      r3, r0
0036abec mov      r0, fp
0036abf0 str      r3, [sp, #0x14]
0036abf4 bl       #0x30ed6c
0036abf8 ldr      r3, [sp, #0x14]
0036abfc mov      r1, r0
0036ac00 mov      r0, r3
0036ac04 bl       #0x30eba4
0036ac08 add      r1, fp, #0x80000000
0036ac0c str      r0, [sp, #0x6c]
0036ac10 mov      r0, r7
0036ac14 bl       #0x30ed6c
0036ac18 mov      r1, sb
0036ac1c mov      fp, r0
0036ac20 ldr      r0, [sp, #0x1c]
0036ac24 bl       #0x30ed6c
0036ac28 mov      r1, r0
0036ac2c mov      r0, fp
0036ac30 bl       #0x30eba4
0036ac34 ldr      r2, [sp, #0x1c]
0036ac38 str      r0, [sp, #0x70]
0036ac3c mov      r0, sl
0036ac40 add      r1, r2, #0x80000000
0036ac44 bl       #0x30ed6c
0036ac48 mov      r1, r7
0036ac4c mov      fp, r0
0036ac50 ldr      r0, [sp, #0x18]
0036ac54 bl       #0x30ed6c
0036ac58 mov      r1, r0
0036ac5c mov      r0, fp
0036ac60 bl       #0x30eba4
0036ac64 str      r0, [sp, #0x74]
0036ac68 add      r0, sp, #0x6c
0036ac6c bl       #0x35e8e0
0036ac70 ldr      lr, [r0, #8]
0036ac74 mov      r3, r0
0036ac78 add      r1, sl, #0x80000000
0036ac7c str      lr, [sp, #0x20]
0036ac80 ldr      r2, [r0, #4]
0036ac84 mov      r0, lr
0036ac88 str      r2, [sp, #0x1c]
0036ac8c ldr      r3, [r3]
0036ac90 str      r3, [sp, #0x18]
0036ac94 bl       #0x30ed6c
0036ac98 ldr      r1, [sp, #0x1c]
0036ac9c mov      fp, r0
0036aca0 mov      r0, sb
0036aca4 bl       #0x30ed6c
0036aca8 mov      r1, r0
0036acac mov      r0, fp
0036acb0 bl       #0x30eba4
0036acb4 add      r1, sb, #0x80000000
0036acb8 str      r0, [sp, #0x60]
0036acbc ldr      r0, [sp, #0x18]
0036acc0 bl       #0x30ed6c
0036acc4 ldr      r1, [sp, #0x20]
0036acc8 mov      fp, r0
0036accc mov      r0, r7
0036acd0 bl       #0x30ed6c
0036acd4 mov      r1, r0
0036acd8 mov      r0, fp
0036acdc bl       #0x30eba4
0036ace0 add      r1, r7, #0x80000000
0036ace4 str      r0, [sp, #0x64]
0036ace8 ldr      r0, [sp, #0x1c]
0036acec bl       #0x30ed6c
0036acf0 ldr      r1, [sp, #0x18]
0036acf4 mov      fp, r0
0036acf8 mov      r0, sl
0036acfc bl       #0x30ed6c
0036ad00 mov      r1, r0
0036ad04 mov      r0, fp
0036ad08 bl       #0x30eba4
0036ad0c str      r0, [sp, #0x68]
0036ad10 add      r0, sp, #0x60
0036ad14 bl       #0x35e8e0
0036ad18 mov      ip, r0
0036ad1c ldr      lr, [ip, #8]
0036ad20 ldr      r0, [r4]
0036ad24 mov      r1, r6
0036ad28 str      lr, [sp, #0x34]
0036ad2c ldr      lr, [ip]
0036ad30 add      r2, sp, #0x98
0036ad34 add      r3, sp, #0x94
0036ad38 str      lr, [sp, #0x2c]
0036ad3c ldr      ip, [ip, #4]
0036ad40 str      ip, [sp, #0x30]
0036ad44 add      ip, sp, #0x90
0036ad48 str      ip, [sp]
0036ad4c bl       #0x861d28
0036ad50 add      r2, sp, #0x88
0036ad54 add      r3, sp, #0x84
0036ad58 ldr      r0, [r4]
0036ad5c add      r1, sp, #0x8c
0036ad60 bl       #0x861b10
0036ad64 ldr      r1, [sp, #0x8c]
0036ad68 ldr      r0, [sp, #0x98]
0036ad6c bl       #0x30e3ac
0036ad70 ldr      r1, [sp, #0x88]
0036ad74 mov      fp, r0
0036ad78 ldr      r0, [sp, #0x94]
0036ad7c bl       #0x30e3ac
0036ad80 ldr      r1, [sp, #0x84]
0036ad84 str      r0, [sp, #0x24]
0036ad88 ldr      r0, [sp, #0x90]
0036ad8c bl       #0x30e3ac
0036ad90 mov      r1, fp
0036ad94 str      r0, [sp, #0x28]
0036ad98 ldr      r0, [sp, #0x18]
0036ad9c bl       #0x30ed6c
0036ada0 ldr      r1, [sp, #0x24]
0036ada4 mov      r3, r0
0036ada8 ldr      r0, [sp, #0x1c]
0036adac str      r3, [sp, #0x14]
0036adb0 bl       #0x30ed6c
0036adb4 ldr      r3, [sp, #0x14]
0036adb8 mov      r1, r0
0036adbc mov      r0, r3
0036adc0 bl       #0x30eba4
0036adc4 ldr      r1, [sp, #0x28]
0036adc8 mov      r3, r0
0036adcc ldr      r0, [sp, #0x20]
0036add0 str      r3, [sp, #0x14]
0036add4 bl       #0x30ed6c
0036add8 ldr      r3, [sp, #0x14]
0036addc mov      r1, r0
0036ade0 mov      r0, r3
0036ade4 bl       #0x30eba4
0036ade8 mov      r1, fp
0036adec mov      r2, r0
0036adf0 ldr      r0, [sp, #0x2c]
0036adf4 str      r2, [sp, #0x10]
0036adf8 bl       #0x30ed6c
0036adfc ldr      r1, [sp, #0x24]
0036ae00 mov      r3, r0
0036ae04 ldr      r0, [sp, #0x30]
0036ae08 str      r3, [sp, #0x14]
0036ae0c bl       #0x30ed6c
0036ae10 ldr      r3, [sp, #0x14]
0036ae14 mov      r1, r0
0036ae18 mov      r0, r3
0036ae1c bl       #0x30eba4
0036ae20 ldr      r1, [sp, #0x28]
0036ae24 mov      r3, r0
0036ae28 ldr      r0, [sp, #0x34]
0036ae2c str      r3, [sp, #0x14]
0036ae30 bl       #0x30ed6c
0036ae34 ldr      r3, [sp, #0x14]
0036ae38 mov      r1, r0
0036ae3c mov      r0, r3
0036ae40 bl       #0x30eba4
0036ae44 mov      r1, fp
0036ae48 mov      r3, r0
0036ae4c mov      r0, r7
0036ae50 str      r3, [sp, #0x14]
0036ae54 bl       #0x30ed6c
0036ae58 ldr      r1, [sp, #0x24]
0036ae5c mov      r7, r0
0036ae60 mov      r0, sl
0036ae64 bl       #0x30ed6c
0036ae68 mov      r1, r0
0036ae6c mov      r0, r7
0036ae70 bl       #0x30eba4
0036ae74 ldr      r1, [sp, #0x28]
0036ae78 mov      r7, r0
0036ae7c mov      r0, sb
0036ae80 bl       #0x30ed6c
0036ae84 mov      r1, r0
0036ae88 mov      r0, r7
0036ae8c bl       #0x30eba4
0036ae90 ldr      r1, [r4]
0036ae94 ldr      r2, [sp, #0x10]
0036ae98 str      r0, [sp]
0036ae9c ldr      r3, [sp, #0x14]
0036aea0 mov      r0, r1
0036aea4 mov      r1, r6
0036aea8 bl       #0x861e48
0036aeac b        #0x36a934
0036aeb0 rsbeq    sl, r2, r0, asr #5
0036aeb4 andeq    r3, r0, r0, lsr fp
0036aeb8 rsbeq    r7, r3, r8, lsl sl
