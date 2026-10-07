Original ELF 36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80

FUNCTION createMaterialRendererForProfileGLES2 006361e8
006361e8 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006361ec ldr r1, [pc, #0x964]
006361f0 sub sp, sp, #0xac
006361f4 ldr ip, [sp, #0xd0]
006361f8 str r1, [sp, #0x68]
006361fc str r0, [sp, #0x64]
00636200 ldr r1, [ip]
00636204 ldr r0, [sp, #0x68]
00636208 str r3, [sp, #0x40]
0063620c cmp r1, ip
00636210 ldreq r1, [sp, #0x64]
00636214 add r0, pc, r0
00636218 moveq r3, #0
0063621c str r0, [sp, #0x68]
00636220 str r2, [sp, #0x3c]
00636224 streq r3, [r1]
00636228 beq #0x636b00
0063622c ldr r2, [sp, #0x3c]
00636230 ldr r2, [r2, #0xdc]
00636234 str r2, [sp, #0x24]
00636238 bl #0x534254
0063623c str r0, [sp, #0x6c]
00636240 mov r0, #1
00636244 bl #0x534268
00636248 ldr ip, [sp, #0xd0]
0063624c ldr r3, [ip]
00636250 cmp ip, r3
00636254 moveq r4, #0
00636258 moveq r0, r4
0063625c beq #0x6362a4
00636260 ldr ip, [sp, #0xd0]
00636264 mov r4, #0
00636268 mov r0, r4
0063626c ldr r2, [r3, #0x10]
00636270 ldr r3, [r3]
00636274 ldr r1, [r2, #0x20]
00636278 ldr r2, [r2, #0x28]
0063627c cmp r4, r1
00636280 movlo r4, r1
00636284 cmp r0, r2
00636288 movlo r0, r2
0063628c cmp ip, r3
00636290 bne #0x63626c
00636294 cmp r0, #0
00636298 beq #0x6362a4
0063629c lsl r0, r0, #2
006362a0 bl #0x5345f4
006362a4 cmp r4, #0
006362a8 str r0, [sp, #0x2c]
006362ac streq r4, [sp, #0x54]
006362b0 beq #0x6362c0
006362b4 lsl r0, r4, #2
006362b8 bl #0x5345f4
006362bc str r0, [sp, #0x54]
006362c0 ldr r0, [sp, #0x24]
006362c4 ldr r1, [sp, #0x40]
006362c8 mov r2, #1
006362cc bl #0x5ddaec
006362d0 cmp r0, #0
006362d4 str r0, [sp, #0x70]
006362d8 beq #0x6366b4
006362dc ldr r0, [sp, #0xd0]
006362e0 ldr r1, [sp, #0xd0]
006362e4 ldr r0, [r0]
006362e8 cmp r1, r0
006362ec str r0, [sp, #0x60]
006362f0 beq #0x6366b4
006362f4 ldr r3, [pc, #0x860]
006362f8 mov r2, #1
006362fc str r2, [sp, #0x34]
00636300 add r3, pc, r3
00636304 str r3, [sp, #0x44]
00636308 ldr r3, [pc, #0x850]
0063630c add r3, pc, r3
00636310 str r3, [sp, #0x74]
00636314 add r3, sp, #0xa0
00636318 str r3, [sp, #0x4c]
0063631c ldr ip, [sp, #0x60]
00636320 ldr ip, [ip, #0x10]
00636324 str ip, [sp, #0x28]
00636328 ldr r0, [ip, #0x20]
0063632c str r0, [sp, #0x38]
00636330 ldr r3, [ip, #0x28]
00636334 sbfx r2, r0, #0, #0x1e
00636338 cmp r3, #0
0063633c movle r3, #0
00636340 movgt r3, #1
00636344 cmp r2, #0
00636348 str r3, [sp, #0x50]
0063634c ble #0x636378
00636350 ldr r0, [sp, #0x54]
00636354 mov r3, #0
00636358 mov r1, r3
0063635c str r1, [r0, r3, lsl #2]
00636360 add r3, r3, #1
00636364 cmp r3, r2
00636368 bne #0x63635c
0063636c ldr r1, [sp, #0x28]
00636370 ldr r1, [r1, #0x20]
00636374 str r1, [sp, #0x38]
00636378 ldr r2, [sp, #0x38]
0063637c cmp r2, #0
00636380 movle r8, #0
00636384 ble #0x63653c
00636388 ldr ip, [sp, #0x50]
0063638c mov r3, #0
00636390 add r0, sp, #0xa4
00636394 eor ip, ip, #1
00636398 str r3, [sp, #0x18]
0063639c str r3, [sp, #0x1c]
006363a0 str r3, [sp, #0x20]
006363a4 mov r8, r3
006363a8 str ip, [sp, #0x48]
006363ac add sl, sp, #0x78
006363b0 add fp, sp, #0x9c
006363b4 str r0, [sp, #0x14]
006363b8 ldr sb, [sp, #0x24]
006363bc b #0x636410
006363c0 ldr r1, [sp, #0x18]
006363c4 ldr r0, [sp, #0x54]
006363c8 add r0, r0, r1
006363cc str r0, [sp, #0x30]
006363d0 ldr r1, [sp, #0x30]
006363d4 ldr r5, [r1]
006363d8 cmp r5, #0
006363dc beq #0x636460
006363e0 ldr r2, [sp, #0x20]
006363e4 ldr ip, [sp, #0x1c]
006363e8 ldr r0, [sp, #0x18]
006363ec ldr r3, [sp, #0x38]
006363f0 add r2, r2, #1
006363f4 add ip, ip, #0xc
006363f8 add r0, r0, #4
006363fc cmp r2, r3
00636400 str r2, [sp, #0x20]
00636404 str ip, [sp, #0x1c]
00636408 str r0, [sp, #0x18]
0063640c beq #0x63653c
00636410 ldr r1, [sp, #0x28]
00636414 ldr r2, [sp, #0x34]
00636418 ldr ip, [sp, #0x1c]
0063641c ldr r3, [r1, #0x24]
00636420 cmp r2, #0
00636424 add r7, r3, ip
00636428 bne #0x6363c0
0063642c ldr r2, [sp, #0x1c]
00636430 mov r0, sb
00636434 ldr r1, [r3, r2]
00636438 bl #0x5dbb48
0063643c ldr ip, [sp, #0x54]
00636440 ldr r3, [sp, #0x18]
00636444 str r0, [ip, r3]
00636448 add r0, ip, r3
0063644c str r0, [sp, #0x30]
00636450 ldr r1, [sp, #0x30]
00636454 ldr r5, [r1]
00636458 cmp r5, #0
0063645c bne #0x6363e0
00636460 mov r0, sb
00636464 ldr r1, [r7]
00636468 mov r2, #1
0063646c bl #0x5dd80c
00636470 cmp r0, #0
00636474 beq #0x6363e0
00636478 ldr r2, [r7, #4]
0063647c cmp r2, #0
00636480 str r2, [sp, #0x10]
00636484 ble #0x636520
00636488 mov r6, r5
0063648c ldr r4, [r7, #8]
00636490 ldr r3, [sp, #0x3c]
00636494 ldr r0, [sp, #0x4c]
00636498 add r4, r4, r5
0063649c mov r2, r4
006364a0 ldr r1, [r3, #0xd8]
006364a4 bl #0x634b30
006364a8 ldr r3, [sp, #0xa0]
006364ac add r1, r4, #0x1c
006364b0 mov r0, sl
006364b4 cmp r3, #0
006364b8 str r3, [sp, #0x9c]
006364bc ldrne r2, [r3, #4]
006364c0 add r6, r6, #1
006364c4 add r5, r5, #0x74
006364c8 addne r2, r2, #1
006364cc strne r2, [r3, #4]
006364d0 bl #0x5d7a10
006364d4 mov r0, sb
006364d8 mov r1, fp
006364dc mov r2, sl
006364e0 ldr r3, [sp, #0x14]
006364e4 bl #0x5dcf2c
006364e8 ldr r0, [sp, #0x9c]
006364ec cmp r0, #0
006364f0 beq #0x6364f8
006364f4 bl #0x31d584
006364f8 ldr r3, [r4, #0x6c]
006364fc ldr r0, [sp, #0xa0]
00636500 cmp r3, #0
00636504 movgt r8, #1
00636508 cmp r0, #0
0063650c beq #0x636514
00636510 bl #0x31d584
00636514 ldr ip, [sp, #0x10]
00636518 cmp r6, ip
0063651c bne #0x63648c
00636520 ldr r1, [sp, #0x48]
00636524 mov r0, sb
00636528 mov r2, #0
0063652c bl #0x5dd664
00636530 ldr r1, [sp, #0x30]
00636534 str r0, [r1]
00636538 b #0x6363e0
0063653c ldr r1, [sp, #0x50]
00636540 cmp r1, #0
00636544 bne #0x636550
00636548 cmp r8, #0
0063654c beq #0x636694
00636550 ldr r2, [sp, #0x28]
00636554 ldr sl, [r2, #0x28]
00636558 sbfx r2, sl, #0, #0x1e
0063655c cmp r2, #0
00636560 ble #0x636588
00636564 ldr r0, [sp, #0x2c]
00636568 mov r3, #0
0063656c mov r1, r3
00636570 str r1, [r0, r3, lsl #2]
00636574 add r3, r3, #1
00636578 cmp r3, r2
0063657c bne #0x636570
00636580 ldr r3, [sp, #0x28]
00636584 ldr sl, [r3, #0x28]
00636588 cmp sl, #0
0063658c ble #0x636834
00636590 mov r4, #0
00636594 mov r6, r4
00636598 mov r8, r4
0063659c ldr sb, [sp, #0x2c]
006365a0 ldr fp, [sp, #0x74]
006365a4 b #0x6365c8
006365a8 ldr r3, [r7]
006365ac add r8, r8, #1
006365b0 add r6, r6, #0x18
006365b4 cmp r3, #0
006365b8 beq #0x63662c
006365bc cmp r8, sl
006365c0 add r4, r4, #4
006365c4 beq #0x636834
006365c8 ldr ip, [sp, #0x28]
006365cc ldr r2, [sp, #0x40]
006365d0 mov r0, #2
006365d4 ldr r3, [ip, #0x2c]
006365d8 mov r1, fp
006365dc add r7, sb, r4
006365e0 add r5, r3, r6
006365e4 ldr ip, [r5, #0xc]
006365e8 cmp ip, #1
006365ec ble #0x6365f8
006365f0 ldr r3, [r3, r6]
006365f4 bl #0x60b034
006365f8 ldr r0, [sp, #0x34]
006365fc cmp r0, #0
00636600 bne #0x6365a8
00636604 ldr r1, [r5]
00636608 ldr r0, [sp, #0x24]
0063660c bl #0x631b58
00636610 add r7, sb, r4
00636614 str r0, [sb, r4]
00636618 ldr r3, [r7]
0063661c add r8, r8, #1
00636620 add r6, r6, #0x18
00636624 cmp r3, #0
00636628 bne #0x6365bc
0063662c ldr r3, [r5, #0x10]
00636630 ldr r2, [r5, #8]
00636634 ldr r0, [sp, #0x24]
00636638 ldr r3, [r3]
0063663c ldr r1, [r5]
00636640 bl #0x631c50
00636644 str r0, [r7]
00636648 b #0x6365bc
0063664c ldr r0, [sp, #0x30]
00636650 ldr r2, [sp, #0x34]
00636654 ldr r1, [sp, #0x48]
00636658 add r0, r0, #1
0063665c add r2, r2, #0x74
00636660 cmp r0, r1
00636664 str r0, [sp, #0x30]
00636668 str r2, [sp, #0x34]
0063666c bne #0x6368ac
00636670 ldr r3, [sp, #0x50]
00636674 ldr r0, [sp, #0x58]
00636678 ldr ip, [sp, #0x5c]
0063667c add r3, r3, #1
00636680 add r0, r0, #0xc
00636684 cmp r3, ip
00636688 str r3, [sp, #0x50]
0063668c str r0, [sp, #0x58]
00636690 bne #0x636854
00636694 ldr r1, [sp, #0x60]
00636698 ldr r3, [sp, #0xd0]
0063669c mov r2, #0
006366a0 ldr r1, [r1]
006366a4 str r2, [sp, #0x34]
006366a8 cmp r3, r1
006366ac str r1, [sp, #0x60]
006366b0 bne #0x63631c
006366b4 ldr r0, [sp, #0x24]
006366b8 bl #0x5dddb4
006366bc ldr ip, [sp, #0x24]
006366c0 ldr r3, [ip, #0x18]
006366c4 ldr r2, [ip, #0x1c]
006366c8 rsb r2, r3, r2
006366cc cmp r0, r2, asr #3
006366d0 addlo r3, r3, r0, lsl #3
006366d4 blo #0x6366e4
006366d8 ldr r3, [pc, #0x484]
006366dc ldr r0, [sp, #0x68]
006366e0 ldr r3, [r0, r3]
006366e4 ldr r3, [r3]
006366e8 cmp r3, #0
006366ec str r3, [sp, #0x98]
006366f0 ldrne r2, [r3]
006366f4 addne r2, r2, #1
006366f8 strne r2, [r3]
006366fc ldr r1, [sp, #0x70]
00636700 cmp r1, #0
00636704 beq #0x636aa4
00636708 ldr fp, [sp, #0x98]
0063670c cmp fp, #0
00636710 beq #0x636b0c
00636714 ldr r2, [sp, #0xd0]
00636718 ldr ip, [sp, #0xd0]
0063671c ldr r2, [r2]
00636720 str r2, [sp, #0x1c]
00636724 ldrh r3, [fp, #0xe]
00636728 cmp ip, r2
0063672c str r3, [sp, #0x14]
00636730 beq #0x636b44
00636734 add r0, sp, #0x98
00636738 mov r6, #0
0063673c str r0, [sp, #0x10]
00636740 b #0x63675c
00636744 ldr r1, [sp, #0x1c]
00636748 ldr r2, [sp, #0xd0]
0063674c ldr r1, [r1]
00636750 cmp r2, r1
00636754 str r1, [sp, #0x1c]
00636758 beq #0x636b30
0063675c ldr r1, [sp, #0x1c]
00636760 ldr r2, [sp, #0x14]
00636764 ldr r1, [r1, #0x10]
00636768 cmp r6, r2
0063676c str r1, [sp, #0x18]
00636770 ldr r5, [r1, #0x28]
00636774 bhs #0x636744
00636778 mov r4, #0
0063677c ldrh r3, [fp, #0xe]
00636780 cmp r3, r6
00636784 ldrhi r3, [fp, #0x20]
00636788 movls r3, #0
0063678c addhi r3, r3, r6, lsl #4
00636790 cmp r4, r5
00636794 bge #0x636744
00636798 ldr sl, [r3]
0063679c ldr r3, [sp, #0x18]
006367a0 mov ip, #0x18
006367a4 mul r7, ip, r4
006367a8 ldr r8, [r3, #0x2c]
006367ac cmp sl, #0
006367b0 add sb, sl, #4
006367b4 movne r1, sb
006367b8 moveq r1, #0
006367bc ldr r0, [r8, r7]
006367c0 bl #0x30e31c
006367c4 cmp r0, #0
006367c8 add r2, r8, r7
006367cc beq #0x636800
006367d0 add r4, r4, #1
006367d4 cmp r4, r5
006367d8 add r7, r7, #0x18
006367dc beq #0x636744
006367e0 cmp sl, #0
006367e4 movne r1, sb
006367e8 moveq r1, #0
006367ec ldr r0, [r8, r7]
006367f0 bl #0x30e31c
006367f4 cmp r0, #0
006367f8 add r2, r8, r7
006367fc bne #0x6367d0
00636800 cmp r5, r4
00636804 ble #0x636744
00636808 mov r1, r6
0063680c ldr r0, [sp, #0x10]
00636810 ldr r3, [sp, #0xd4]
00636814 bl #0x6324c4
00636818 ldr r0, [sp, #0x14]
0063681c add r6, r6, #1
00636820 uxth r6, r6
00636824 cmp r6, r0
00636828 bhs #0x636a9c
0063682c ldr fp, [sp, #0x98]
00636830 b #0x63677c
00636834 ldr r1, [sp, #0x28]
00636838 ldr r1, [r1, #0x20]
0063683c cmp r1, #0
00636840 str r1, [sp, #0x5c]
00636844 ble #0x636694
00636848 mov r2, #0
0063684c str r2, [sp, #0x58]
00636850 str r2, [sp, #0x50]
00636854 ldr r3, [sp, #0x50]
00636858 ldr ip, [sp, #0x54]
0063685c ldr r1, [ip, r3, lsl #2]
00636860 add r0, ip, r3, lsl #2
00636864 str r0, [sp, #0x20]
00636868 cmp r1, #0
0063686c beq #0x636670
00636870 ldr r0, [sp, #0x24]
00636874 bl #0x5d7cf4
00636878 ldr r1, [sp, #0x28]
0063687c ldr r2, [sp, #0x58]
00636880 mov fp, r0
00636884 ldr r3, [r1, #0x24]
00636888 add r3, r3, r2
0063688c str r3, [sp, #0x38]
00636890 ldr r3, [r3, #4]
00636894 cmp r3, #0
00636898 str r3, [sp, #0x48]
0063689c ble #0x636670
006368a0 mov ip, #0
006368a4 str ip, [sp, #0x34]
006368a8 str ip, [sp, #0x30]
006368ac ldr r0, [sp, #0x38]
006368b0 ldr r1, [sp, #0x34]
006368b4 ldr r3, [r0, #8]
006368b8 add r3, r3, r1
006368bc str r3, [sp, #0x14]
006368c0 ldr r2, [r3, #0x6c]
006368c4 cmp r2, #0
006368c8 str r2, [sp, #0x10]
006368cc ble #0x63664c
006368d0 ldr r3, [sp, #0x30]
006368d4 mov ip, #0x34
006368d8 mov r4, #0
006368dc uxtb r3, r3
006368e0 mul ip, ip, r3
006368e4 str r3, [sp, #0x1c]
006368e8 str ip, [sp, #0x18]
006368ec mov sl, r4
006368f0 b #0x636928
006368f4 ldr r0, [sp, #0x2c]
006368f8 ldr r1, [r0, r3, lsl #2]
006368fc ldr r3, [sp, #0x20]
00636900 ldr r0, [sp, #0x24]
00636904 ldr r2, [r3]
00636908 ldr r3, [sp, #0x1c]
0063690c stm sp, {r6, r8}
00636910 bl #0x631bc4
00636914 ldr ip, [sp, #0x10]
00636918 add sl, sl, #1
0063691c add r4, r4, #0xc
00636920 cmp sl, ip
00636924 beq #0x63664c
00636928 ldr r0, [sp, #0x14]
0063692c ldr r1, [sp, #0x18]
00636930 ldr r2, [fp, #8]
00636934 ldr r7, [r0, #0x70]
00636938 mov r3, #0
0063693c add r2, r2, r1
00636940 add r5, r7, r4
00636944 ldr sb, [r2, #0x20]
00636948 ldrb r8, [r5, #5]
0063694c ldr r1, [r7, r4]
00636950 mov r0, sb
00636954 mov r2, r8
00636958 bl #0x5e4b74
0063695c movw r2, #0xffff
00636960 cmp r0, r2
00636964 mov r6, r0
00636968 beq #0x6369d0
0063696c ldrb r3, [r5, #4]
00636970 cmp r3, #1
00636974 beq #0x6369f4
00636978 ldr ip, [sp, #0x28]
0063697c ldr r3, [r5, #8]
00636980 mov r0, #0x18
00636984 ldr r2, [ip, #0x2c]
00636988 mla r2, r0, r3, r2
0063698c ldr r2, [r2, #4]
00636990 cmp r2, #0x11
00636994 bne #0x6368f4
00636998 ldr ip, [sp, #0x2c]
0063699c add r2, r8, #5
006369a0 ldr r2, [sb, r2, lsl #3]
006369a4 ldr r0, [sp, #0x20]
006369a8 ldr r1, [ip, r3, lsl #2]
006369ac ldr ip, [sp, #0x1c]
006369b0 add r2, r2, r6, lsl #4
006369b4 ldr r3, [r0]
006369b8 ldrh r2, [r2, #4]
006369bc ldr r0, [sp, #0x24]
006369c0 stmib sp, {r6, r8}
006369c4 str ip, [sp]
006369c8 bl #0x5da83c
006369cc b #0x636914
006369d0 ldr ip, [sp, #0x38]
006369d4 mov r0, #3
006369d8 ldr r1, [sp, #0x44]
006369dc ldr r3, [ip]
006369e0 ldr ip, [r7, r4]
006369e4 ldr r2, [sp, #0x40]
006369e8 str ip, [sp]
006369ec bl #0x60b034
006369f0 b #0x636914
006369f4 ldr r1, [sp, #0x3c]
006369f8 ldr r0, [r1, #0xe4]
006369fc ldr r1, [r5, #8]
00636a00 bl #0x5bb378
00636a04 movw r2, #0xffff
00636a08 cmp r0, r2
00636a0c beq #0x636a30
00636a10 ldr r1, [sp, #0x20]
00636a14 ldr r3, [sp, #0x1c]
00636a18 ldr r2, [r1]
00636a1c mov r1, r0
00636a20 ldr r0, [sp, #0x24]
00636a24 stm sp, {r6, r8}
00636a28 bl #0x5da6f0
00636a2c b #0x636914
00636a30 add r3, r8, #5
00636a34 ldr r3, [sb, r3, lsl #3]
00636a38 add r3, r3, r6, lsl #4
00636a3c ldrh r2, [r3, #4]
00636a40 cmp r2, #0x12
00636a44 ble #0x636a80
00636a48 cmp r2, #0x1b
00636a4c bgt #0x636a80
00636a50 ldr ip, [sp, #0x3c]
00636a54 ldr r1, [r5, #8]
00636a58 mov r2, #0x12
00636a5c ldr r0, [ip, #0xe4]
00636a60 mov lr, r2
00636a64 ldr ip, [r3, #8]
00636a68 str ip, [sp]
00636a6c ldrb ip, [r3, #7]
00636a70 mov r3, lr
00636a74 str ip, [sp, #4]
00636a78 bl #0x5bc374
00636a7c b #0x636a10
00636a80 cmp r2, #0x12
00636a84 beq #0x636a50
00636a88 ldr ip, [sp, #0x3c]
00636a8c ldr r1, [r5, #8]
00636a90 ldrb lr, [r3, #6]
00636a94 ldr r0, [ip, #0xe4]
00636a98 b #0x636a64
00636a9c ldr fp, [sp, #0x98]
00636aa0 b #0x636744
00636aa4 ldr fp, [sp, #0x98]
00636aa8 cmp fp, #0
00636aac beq #0x636b0c
00636ab0 ldr r0, [sp, #0x64]
00636ab4 add r1, sp, #0x98
00636ab8 str fp, [r0]
00636abc str r1, [sp, #0x10]
00636ac0 ldr r3, [fp]
00636ac4 add r3, r3, #1
00636ac8 str r3, [fp]
00636acc ldr r0, [sp, #0x10]
00636ad0 bl #0x3522b8
00636ad4 ldr r0, [sp, #0x54]
00636ad8 cmp r0, #0
00636adc beq #0x636ae4
00636ae0 bl #0x534688
00636ae4 ldr r1, [sp, #0x2c]
00636ae8 cmp r1, #0
00636aec beq #0x636af8
00636af0 mov r0, r1
00636af4 bl #0x534688
00636af8 ldr r0, [sp, #0x6c]
00636afc bl #0x534268
00636b00 ldr r0, [sp, #0x64]
00636b04 add sp, sp, #0xac
00636b08 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00636b0c ldr r1, [pc, #0x54]
00636b10 ldr r2, [sp, #0x40]
00636b14 mov r0, #3
00636b18 add r1, pc, r1
00636b1c bl #0x60b034
00636b20 add r3, sp, #0xa8
00636b24 str r3, [sp, #0x10]
00636b28 ldr fp, [r3, #-0x10]!
00636b2c str r3, [sp, #0x10]
00636b30 ldr ip, [sp, #0x64]
00636b34 cmp fp, #0
00636b38 str fp, [ip]
00636b3c bne #0x636ac0
00636b40 b #0x636acc
00636b44 ldr r2, [sp, #0x64]
00636b48 add r3, sp, #0x98
00636b4c str fp, [r2]
00636b50 str r3, [sp, #0x10]
00636b54 b #0x636ac0
00636b58 eorseq lr, r5, ip, ror r8
00636b5c eoreq lr, sl, r0, lsl sp
00636b60 eoreq lr, sl, ip, asr #25
00636b64 ldrdeq r3, r4, [r0], -ip
00636b68 eoreq lr, sl, r8, lsl r5

FUNCTION createShaderGLES2 00634b30
00634b30 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00634b34 ldr r5, [pc, #0x120]
00634b38 ldr r7, [pc, #0x120]
00634b3c sub sp, sp, #0x3c
00634b40 add r5, pc, r5
00634b44 ldr r3, [r5, r7]
00634b48 add r4, sp, #0x1c
00634b4c mov r8, r0
00634b50 ldr r3, [r3]
00634b54 mov sb, r1
00634b58 mov r0, r4
00634b5c mov r1, #0x10
00634b60 mov sl, r2
00634b64 str r3, [sp, #0x34]
00634b68 str r4, [sp, #0x2c]
00634b6c str r4, [sp, #0x30]
00634b70 bl #0x3209a8
00634b74 ldr r3, [sp, #0x2c]
00634b78 mov r6, #0
00634b7c strb r6, [r3]
00634b80 ldr fp, [sl, #4]
00634b84 mov r0, fp
00634b88 bl #0x30de54
00634b8c mov r1, fp
00634b90 add r2, fp, r0
00634b94 mov r0, r4
00634b98 bl #0x320a4c
00634b9c ldr fp, [sl, #0xc]
00634ba0 mov r0, fp
00634ba4 bl #0x30de54
00634ba8 mov r1, fp
00634bac add r2, fp, r0
00634bb0 mov r0, r4
00634bb4 bl #0x320a4c
00634bb8 ldr fp, [sl, #0x10]
00634bbc mov r0, fp
00634bc0 bl #0x30de54
00634bc4 mov r1, fp
00634bc8 add r2, fp, r0
00634bcc mov r0, r4
00634bd0 bl #0x320a4c
00634bd4 ldr fp, [sl, #0x18]
00634bd8 mov r0, fp
00634bdc bl #0x30de54
00634be0 mov r1, fp
00634be4 add r2, fp, r0
00634be8 mov r0, r4
00634bec bl #0x320a4c
00634bf0 ldr r3, [sl, #4]
00634bf4 ldr lr, [sl, #0x18]
00634bf8 ldr ip, [sl, #0xc]
00634bfc ldr sl, [sl, #0x10]
00634c00 mov r0, r8
00634c04 mov r1, sb
00634c08 ldr r2, [sp, #0x30]
00634c0c str ip, [sp]
00634c10 stmib sp, {sl, lr}
00634c14 str r6, [sp, #0x10]
00634c18 str r6, [sp, #0xc]
00634c1c bl #0x6e01b4
00634c20 ldr r0, [sp, #0x30]
00634c24 cmp r0, r4
00634c28 beq #0x634c38
00634c2c cmp r0, r6
00634c30 beq #0x634c38
00634c34 bl #0x310450
00634c38 ldr r3, [r5, r7]
00634c3c ldr r2, [sp, #0x34]
00634c40 mov r0, r8
00634c44 ldr r3, [r3]
00634c48 cmp r2, r3
00634c4c bne #0x634c58
00634c50 add sp, sp, #0x3c
00634c54 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00634c58 bl #0x30e310
00634c5c eorseq pc, r5, r0, asr pc
00634c60 andeq r4, r0, ip, lsr #1

FUNCTION constructMaterial 0061cca8
0061cca8 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061ccac ldr r5, [pc, #0x23c]
0061ccb0 ldr sb, [pc, #0x23c]
0061ccb4 sub sp, sp, #0x7c
0061ccb8 cmp r3, #0
0061ccbc add r5, pc, r5
0061ccc0 str r3, [sp, #0x14]
0061ccc4 ldr r3, [r5, sb]
0061ccc8 mov fp, r0
0061cccc ldr r0, [sp, #0xa0]
0061ccd0 ldr r3, [r3]
0061ccd4 mov r7, r1
0061ccd8 mov r6, r2
0061ccdc str r0, [sp, #0x18]
0061cce0 str r3, [sp, #0x74]
0061cce4 beq #0x61cee0
0061cce8 ldr r3, [r2, #0xd4]
0061ccec add sl, sp, #0x2c
0061ccf0 add r8, sp, #0x44
0061ccf4 ldr r4, [r3, #0x34]
0061ccf8 cmp r4, #0
0061ccfc ldrne r3, [r4, #4]
0061cd00 mov r0, r4
0061cd04 addne r3, r3, #1
0061cd08 strne r3, [r4, #4]
0061cd0c ldr r3, [r4]
0061cd10 mov lr, pc
0061cd14 ldr pc, [r3, #0x2c]
0061cd18 add r3, sp, #0x5c
0061cd1c mov r1, r0
0061cd20 add r2, sp, #0x28
0061cd24 mov r0, r3
0061cd28 str r3, [sp, #0x10]
0061cd2c bl #0x32603c
0061cd30 ldr r3, [r4]
0061cd34 ldr r1, [r7]
0061cd38 add r2, sp, #0x24
0061cd3c ldr r3, [r3, #0x38]
0061cd40 cmp r1, #0
0061cd44 ldrne r1, [r1, #0x20]
0061cd48 mov r0, sl
0061cd4c str r3, [sp, #0xc]
0061cd50 bl #0x32603c
0061cd54 mov r0, r8
0061cd58 mov r1, r4
0061cd5c mov r2, sl
0061cd60 ldr r3, [sp, #0xc]
0061cd64 blx r3
0061cd68 ldr r0, [sp, #0x40]
0061cd6c cmp r0, sl
0061cd70 beq #0x61cd80
0061cd74 cmp r0, #0
0061cd78 beq #0x61cd80
0061cd7c bl #0x310450
0061cd80 ldr r1, [sp, #0x58]
0061cd84 ldr r3, [sp, #0x54]
0061cd88 cmp r1, r3
0061cd8c beq #0x61cea8
0061cd90 ldrsb r3, [r3, #-1]
0061cd94 cmp r3, #0x5c
0061cd98 beq #0x61cdbc
0061cd9c cmp r3, #0x2f
0061cda0 beq #0x61cdbc
0061cda4 ldr r1, [pc, #0x14c]
0061cda8 mov r0, r8
0061cdac add r1, pc, r1
0061cdb0 add r2, r1, #1
0061cdb4 bl #0x320a4c
0061cdb8 ldr r1, [sp, #0x58]
0061cdbc mov r2, #1
0061cdc0 mov r3, r2
0061cdc4 ldr ip, [r4]
0061cdc8 mov r0, r4
0061cdcc mov lr, pc
0061cdd0 ldr pc, [ip, #0x1c]
0061cdd4 str r0, [sp, #0x1c]
0061cdd8 ldr r0, [r7, #4]
0061cddc add sl, sp, #0x20
0061cde0 mov r2, r7
0061cde4 ldr ip, [r0]
0061cde8 mov r1, r0
0061cdec ldr r0, [sp, #0x14]
0061cdf0 mov r3, r6
0061cdf4 str r0, [sp]
0061cdf8 ldr r0, [sp, #0x18]
0061cdfc str r0, [sp, #4]
0061ce00 mov r0, sl
0061ce04 mov lr, pc
0061ce08 ldr pc, [ip, #0x20]
0061ce0c ldr r2, [sp, #0x1c]
0061ce10 cmp r2, #0
0061ce14 beq #0x61ce2c
0061ce18 ldr r3, [r4]
0061ce1c mov r0, r4
0061ce20 ldr r1, [sp, #0x58]
0061ce24 mov lr, pc
0061ce28 ldr pc, [r3, #0x28]
0061ce2c ldr r3, [sp, #0x20]
0061ce30 mov r0, sl
0061ce34 cmp r3, #0
0061ce38 str r3, [fp]
0061ce3c ldrne r2, [r3]
0061ce40 addne r2, r2, #1
0061ce44 strne r2, [r3]
0061ce48 bl #0x310be8
0061ce4c ldr r0, [sp, #0x58]
0061ce50 cmp r0, r8
0061ce54 beq #0x61ce64
0061ce58 cmp r0, #0
0061ce5c beq #0x61ce64
0061ce60 bl #0x310450
0061ce64 ldr r0, [sp, #0x70]
0061ce68 ldr r3, [sp, #0x10]
0061ce6c cmp r0, r3
0061ce70 beq #0x61ce80
0061ce74 cmp r0, #0
0061ce78 beq #0x61ce80
0061ce7c bl #0x310450
0061ce80 mov r0, r4
0061ce84 bl #0x31d584
0061ce88 ldr r3, [r5, sb]
0061ce8c ldr r2, [sp, #0x74]
0061ce90 mov r0, fp
0061ce94 ldr r3, [r3]
0061ce98 cmp r2, r3
0061ce9c bne #0x61ceec
0061cea0 add sp, sp, #0x7c
0061cea4 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0061cea8 ldr r0, [r7, #4]
0061ceac add sl, sp, #0x20
0061ceb0 mov r2, r7
0061ceb4 ldr ip, [r0]
0061ceb8 mov r1, r0
0061cebc ldr r0, [sp, #0x14]
0061cec0 mov r3, r6
0061cec4 str r0, [sp]
0061cec8 ldr r0, [sp, #0x18]
0061cecc str r0, [sp, #4]
0061ced0 mov r0, sl
0061ced4 mov lr, pc
0061ced8 ldr pc, [ip, #0x20]
0061cedc b #0x61ce2c
0061cee0 ldr r2, [sp, #0x14]
0061cee4 str r2, [fp]
0061cee8 b #0x61ce88
0061ceec bl #0x30e310
0061cef0 ldrsbteq r7, [r7], -r4
0061cef4 andeq r4, r0, ip, lsr #1
0061cef8 eoreq r3, sl, ip, lsr #29

FUNCTION SRenderStateCtor 005d797c
005d797c push {r4, r5}
005d7980 mov r4, #0x1c0000
005d7984 add r4, r4, #0xf00
005d7988 movw ip, #0x3007
005d798c orr ip, ip, ip, lsl #22
005d7990 str r4, [r0, #8]
005d7994 movw r4, #0x101
005d7998 mov r2, #0
005d799c mov r1, #0x3f800000
005d79a0 mov r5, #0
005d79a4 orr r4, r4, #0xff000000
005d79a8 str ip, [r0, #0xc]
005d79ac mvn ip, #0xff00
005d79b0 str r5, [r0, #0x30]
005d79b4 str r1, [r0, #0x38]
005d79b8 str r2, [r0, #0x10]
005d79bc stm r0, {r4, ip}
005d79c0 strb r2, [r0, #0x14]
005d79c4 strb r2, [r0, #0x15]
005d79c8 strb r2, [r0, #0x16]
005d79cc strb r2, [r0, #0x17]
005d79d0 strb r2, [r0, #0x18]
005d79d4 strb r2, [r0, #0x19]
005d79d8 strb r2, [r0, #0x1a]
005d79dc strb r2, [r0, #0x1b]
005d79e0 str r1, [r0, #0x1c]
005d79e4 str r5, [r0, #0x20]
005d79e8 str r1, [r0, #0x24]
005d79ec str r1, [r0, #0x28]
005d79f0 str r1, [r0, #0x2c]
005d79f4 str r1, [r0, #0x34]
005d79f8 str r2, [r0, #0x3c]
005d79fc str r2, [r0, #0x40]
005d7a00 str r2, [r0, #0x44]
005d7a04 str r2, [r0, #0x48]
005d7a08 pop {r4, r5}
005d7a0c bx lr

FUNCTION RenderpassStateConvert 005d7a10
005d7a10 push {r4, r5}
005d7a14 ldrb ip, [r1, #0x15]
005d7a18 ldrb r2, [r1, #0x14]
005d7a1c ldrb r4, [r1, #0x17]
005d7a20 ldrb r5, [r1, #0x16]
005d7a24 strb r2, [r0, #8]
005d7a28 strb r4, [r0, #0xb]
005d7a2c strb r5, [r0, #0xa]
005d7a30 strb ip, [r0, #9]
005d7a34 mov r3, r0
005d7a38 ldr r0, [r1, #0x28]
005d7a3c mov r2, #0
005d7a40 str r0, [r3, #0xc]
005d7a44 ldr r0, [r1, #0x2c]
005d7a48 str r0, [r3, #0x10]
005d7a4c ldr r0, [r1, #0x38]
005d7a50 str r2, [r3, #4]
005d7a54 str r2, [r3]
005d7a58 str r0, [r3, #0x1c]
005d7a5c ldr r2, [r1, #0xc]
005d7a60 tst r2, #0x80000
005d7a64 movne r2, #0x10000
005d7a68 strne r2, [r3, #4]
005d7a6c ldr ip, [r1, #8]
005d7a70 ubfx ip, ip, #0xc, #3
005d7a74 lsl ip, ip, #0x18
005d7a78 str ip, [r3]
005d7a7c ldrb r0, [r1]
005d7a80 orr ip, ip, r0
005d7a84 str ip, [r3]
005d7a88 ldr r2, [r1, #0xc]
005d7a8c tst r2, #0x100000
005d7a90 ldr r2, [r3, #4]
005d7a94 orrne r2, r2, #0x20000
005d7a98 biceq r2, r2, #0x20000
005d7a9c str r2, [r3, #4]
005d7aa0 ldr r0, [r1, #8]
005d7aa4 bic r2, r2, #0x40000
005d7aa8 and r0, r0, #0xc0000000
005d7aac orr r0, ip, r0
005d7ab0 str r0, [r3]
005d7ab4 ldr ip, [r1, #0xc]
005d7ab8 ubfx ip, ip, #0x15, #1
005d7abc orr r2, r2, ip, lsl #18
005d7ac0 str r2, [r3, #4]
005d7ac4 ldr ip, [r1, #0xc]
005d7ac8 tst ip, #0x400000
005d7acc orrne r2, r2, #0x80000
005d7ad0 biceq r2, r2, #0x80000
005d7ad4 str r2, [r3, #4]
005d7ad8 ldr r2, [r1, #0xc]
005d7adc ubfx r2, r2, #0xc, #3
005d7ae0 orr r0, r0, r2, lsl #27
005d7ae4 str r0, [r3]
005d7ae8 ldr r2, [r1, #0xc]
005d7aec tst r2, #0x800000
005d7af0 ldr r2, [r3, #4]
005d7af4 orrne r2, r2, #0x100000
005d7af8 biceq r2, r2, #0x100000
005d7afc str r2, [r3, #4]
005d7b00 ldr r0, [r1, #0xc]
005d7b04 bic r2, r2, #0x3000
005d7b08 ubfx r0, r0, #0xf, #2
005d7b0c orr r2, r2, r0, lsl #12
005d7b10 str r2, [r3, #4]
005d7b14 ldr r0, [r1, #0xc]
005d7b18 bic r2, r2, #0xc000
005d7b1c ubfx r0, r0, #0x11, #2
005d7b20 orr r2, r2, r0, lsl #14
005d7b24 str r2, [r3, #4]
005d7b28 ldr r0, [r1, #0xc]
005d7b2c tst r0, #0x2000000
005d7b30 orrne ip, r2, #0x200000
005d7b34 biceq ip, r2, #0x200000
005d7b38 str ip, [r3, #4]
005d7b3c ldr r2, [r1, #0xc]
005d7b40 tst r2, #0x4000000
005d7b44 orrne ip, ip, #0x400000
005d7b48 biceq ip, ip, #0x400000
005d7b4c str ip, [r3, #4]
005d7b50 ldr r2, [r1, #0xc]
005d7b54 tst r2, #0x8000000
005d7b58 orrne ip, ip, #0x800000
005d7b5c biceq ip, ip, #0x800000
005d7b60 str ip, [r3, #4]
005d7b64 ldr r2, [r1, #0xc]
005d7b68 tst r2, #0x10000000
005d7b6c orrne ip, ip, #0x1000000
005d7b70 biceq ip, ip, #0x1000000
005d7b74 str ip, [r3, #4]
005d7b78 ldr r2, [r1, #0xc]
005d7b7c tst r2, #0x20000000
005d7b80 orrne ip, ip, #0x2000000
005d7b84 biceq ip, ip, #0x2000000
005d7b88 str ip, [r3, #4]
005d7b8c ldr r2, [r1, #0xc]
005d7b90 tst r2, #0x40000000
005d7b94 orrne ip, ip, #0x4000000
005d7b98 biceq ip, ip, #0x4000000
005d7b9c str ip, [r3, #4]
005d7ba0 ldr r2, [r1, #0x10]
005d7ba4 tst r2, #1
005d7ba8 orrne ip, ip, #0x8000000
005d7bac biceq ip, ip, #0x8000000
005d7bb0 str ip, [r3, #4]
005d7bb4 ldr r0, [r1, #8]
005d7bb8 bic ip, ip, #7
005d7bbc ldr r2, [r3]
005d7bc0 ubfx r0, r0, #0x12, #3
005d7bc4 orr ip, ip, r0
005d7bc8 str ip, [r3, #4]
005d7bcc ldrb r0, [r1, #2]
005d7bd0 bic r2, r2, #0xff00
005d7bd4 bic ip, ip, #0x38
005d7bd8 orr r2, r2, r0, lsl #8
005d7bdc str r2, [r3]
005d7be0 ldrb r4, [r1, #3]
005d7be4 bic r2, r2, #0xff0000
005d7be8 mov r0, r3
005d7bec orr r2, r2, r4, lsl #16
005d7bf0 str r2, [r3]
005d7bf4 ldr r2, [r1, #8]
005d7bf8 ubfx r2, r2, #0x15, #3
005d7bfc orr r2, ip, r2, lsl #3
005d7c00 str r2, [r3, #4]
005d7c04 ldr ip, [r1, #8]
005d7c08 bic r2, r2, #0x1c0
005d7c0c ubfx ip, ip, #0x18, #3
005d7c10 orr r2, r2, ip, lsl #6
005d7c14 str r2, [r3, #4]
005d7c18 ldr ip, [r1, #8]
005d7c1c bic r2, r2, #0xe00
005d7c20 ubfx ip, ip, #0x1b, #3
005d7c24 orr r2, r2, ip, lsl #9
005d7c28 str r2, [r3, #4]
005d7c2c ldr ip, [r1, #0x30]
005d7c30 ldr r2, [r1, #0x34]
005d7c34 str ip, [r3, #0x14]
005d7c38 str r2, [r3, #0x18]
005d7c3c pop {r4, r5}
005d7c40 bx lr

FUNCTION ApplyBlend 005af3f8
005af3f8 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005af3fc ldrb r3, [r0, #0x1c4]
005af400 sub sp, sp, #0xc
005af404 mov r4, r0
005af408 cmp r3, #0
005af40c mov r8, r1
005af410 beq #0x5af558
005af414 ldr r2, [r8]
005af418 ldr r3, [r4, #0x1fc]
005af41c ubfx r5, r2, #0x18, #3
005af420 cmp r5, r3
005af424 beq #0x5af444
005af428 ldr r3, [pc, #0x13c]
005af42c add r3, pc, r3
005af430 add r3, r3, #0x3c
005af434 ldr r0, [r3, r5, lsl #2]
005af438 bl #0x30df14
005af43c str r5, [r4, #0x1fc]
005af440 ldr r2, [r8]
005af444 and r3, r2, #0xf
005af448 mov r5, #0
005af44c ubfx r2, r2, #4, #4
005af450 bfi r5, r3, #0, #8
005af454 ldr r1, [r4, #0x200]
005af458 bfi r5, r2, #8, #8
005af45c bfc r5, #0x10, #0x10
005af460 cmp r5, r1
005af464 beq #0x5af480
005af468 ldr r0, [pc, #0x100]
005af46c add r0, pc, r0
005af470 ldr r1, [r0, r2, lsl #2]
005af474 ldr r0, [r0, r3, lsl #2]
005af478 bl #0x30e418
005af47c str r5, [r4, #0x200]
005af480 ldrb r7, [r8, #8]
005af484 ldrb r6, [r8, #0xb]
005af488 ldrb r5, [r8, #0xa]
005af48c ldrb r3, [r4, #0x204]
005af490 ldrb r2, [r4, #0x207]
005af494 ldrb r8, [r8, #9]
005af498 ldrb r0, [r4, #0x206]
005af49c ldrb r1, [r4, #0x205]
005af4a0 strb r2, [sp, #3]
005af4a4 strb r0, [sp, #2]
005af4a8 strb r1, [sp, #1]
005af4ac strb r3, [sp]
005af4b0 strb r6, [sp, #7]
005af4b4 strb r5, [sp, #6]
005af4b8 strb r8, [sp, #5]
005af4bc strb r7, [sp, #4]
005af4c0 ldr r3, [sp]
005af4c4 ldr r2, [sp, #4]
005af4c8 cmp r2, r3
005af4cc beq #0x5af550
005af4d0 mov r0, r7
005af4d4 bl #0x30e964
005af4d8 movw r1, #0x8081
005af4dc movt r1, #0x3b80
005af4e0 bl #0x30ed6c
005af4e4 mov sb, r0
005af4e8 mov r0, r8
005af4ec bl #0x30e964
005af4f0 movw r1, #0x8081
005af4f4 movt r1, #0x3b80
005af4f8 bl #0x30ed6c
005af4fc mov sl, r0
005af500 mov r0, r5
005af504 bl #0x30e964
005af508 movw r1, #0x8081
005af50c movt r1, #0x3b80
005af510 bl #0x30ed6c
005af514 mov fp, r0
005af518 mov r0, r6
005af51c bl #0x30e964
005af520 movw r1, #0x8081
005af524 movt r1, #0x3b80
005af528 bl #0x30ed6c
005af52c mov r1, sl
005af530 mov r3, r0
005af534 mov r2, fp
005af538 mov r0, sb
005af53c bl #0x30e364
005af540 strb r7, [r4, #0x204]
005af544 strb r6, [r4, #0x207]
005af548 strb r5, [r4, #0x206]
005af54c strb r8, [r4, #0x205]
005af550 add sp, sp, #0xc
005af554 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005af558 movw r0, #0xbe2
005af55c bl #0x30e544
005af560 mov r3, #1
005af564 strb r3, [r4, #0x1c4]
005af568 b #0x5af414
005af56c eorseq r0, r3, r8, lsl #24
005af570 eorseq r0, r3, r8, asr #23

FUNCTION ApplyDepth 005af6fc
005af6fc push {r4, r5, r6, lr}
005af700 ldrb r3, [r0, #0x1c6]
005af704 mov r4, r0
005af708 mov r5, r1
005af70c cmp r3, #0
005af710 beq #0x5af748
005af714 ldr r3, [r5]
005af718 ldr r2, [r4, #0x1e0]
005af71c ubfx r3, r3, #0x1b, #3
005af720 cmp r3, r2
005af724 beq #0x5af744
005af728 ldr r2, [pc, #0x2c]
005af72c str r3, [r4, #0x1e0]
005af730 add r2, pc, r2
005af734 add r2, r2, #0x5c
005af738 ldr r0, [r2, r3, lsl #2]
005af73c pop {r4, r5, r6, lr}
005af740 b #0x30e298
005af744 pop {r4, r5, r6, pc}
005af748 movw r0, #0xb71
005af74c bl #0x30e544
005af750 mov r3, #1
005af754 strb r3, [r4, #0x1c6]
005af758 b #0x5af714
005af75c eorseq r0, r3, r4, lsl #18

FUNCTION ApplyCull 005af654
005af654 push {r4, r5, r6, lr}
005af658 ldrb r3, [r0, #0x1c5]
005af65c mov r4, r0
005af660 mov r5, r1
005af664 cmp r3, #0
005af668 beq #0x5af6a0
005af66c ldr r3, [r5]
005af670 ldr r2, [r4, #0x1d8]
005af674 lsr r3, r3, #0x1e
005af678 cmp r3, r2
005af67c beq #0x5af69c
005af680 ldr r2, [pc, #0x2c]
005af684 str r3, [r4, #0x1d8]
005af688 add r2, pc, r2
005af68c add r2, r2, #0x50
005af690 ldr r0, [r2, r3, lsl #2]
005af694 pop {r4, r5, r6, lr}
005af698 b #0x30e8b0
005af69c pop {r4, r5, r6, pc}
005af6a0 movw r0, #0xb44
005af6a4 bl #0x30e544
005af6a8 mov r3, #1
005af6ac strb r3, [r4, #0x1c5]
005af6b0 b #0x5af66c
005af6b4 eorseq r0, r3, ip, lsr #19

FUNCTION ApplyNonGrouped 005b70cc
005b70cc push {r4, r5, r6, lr}
005b70d0 mov r4, r0
005b70d4 ldr r0, [r1, #4]
005b70d8 ldr r3, [r4, #0x1dc]
005b70dc mov r5, r1
005b70e0 ubfx r6, r0, #0x12, #1
005b70e4 cmp r6, r3
005b70e8 beq #0x5b7118
005b70ec ldrb r3, [r4, #0x4a0]
005b70f0 cmp r3, #0
005b70f4 ldr r3, [pc, #0xd8]
005b70f8 rsbne r2, r6, #1
005b70fc moveq r2, r6
005b7100 add r3, pc, r3
005b7104 add r3, r3, r2, lsl #2
005b7108 ldr r0, [r3, #0x9c]
005b710c bl #0x30de78
005b7110 str r6, [r4, #0x1dc]
005b7114 ldr r0, [r5, #4]
005b7118 ldrb r3, [r4, #0x1c7]
005b711c ubfx r0, r0, #0x14, #1
005b7120 cmp r3, r0
005b7124 beq #0x5b7130
005b7128 strb r0, [r4, #0x1c7]
005b712c bl #0x30e238
005b7130 ldr r6, [r5, #0xc]
005b7134 ldr r1, [r4, #0x218]
005b7138 mov r0, r6
005b713c bl #0x30df8c
005b7140 cmp r0, #0
005b7144 beq #0x5b71c4
005b7148 ldr r6, [r5, #0x10]
005b714c ldr r1, [r4, #0x21c]
005b7150 mov r0, r6
005b7154 bl #0x30df8c
005b7158 cmp r0, #0
005b715c streq r6, [r4, #0x21c]
005b7160 ldr r3, [r5, #4]
005b7164 ldr r1, [r4, #0x1e4]
005b7168 ubfx r2, r3, #0xc, #2
005b716c cmp r2, r1
005b7170 strne r2, [r4, #0x1e4]
005b7174 ldrne r3, [r5, #4]
005b7178 ldr r1, [r4, #0x1e8]
005b717c ubfx r2, r3, #0xe, #2
005b7180 cmp r2, r1
005b7184 strne r2, [r4, #0x1e8]
005b7188 ldrne r3, [r5, #4]
005b718c ldrb r2, [r4, #0x1d0]
005b7190 ubfx r3, r3, #0x18, #1
005b7194 cmp r2, r3
005b7198 beq #0x5b71c0
005b719c cmp r3, #0
005b71a0 strb r3, [r4, #0x1d0]
005b71a4 bne #0x5b71b4
005b71a8 movw r0, #0x809e
005b71ac pop {r4, r5, r6, lr}
005b71b0 b #0x30df68
005b71b4 movw r0, #0x809e
005b71b8 pop {r4, r5, r6, lr}
005b71bc b #0x30e544
005b71c0 pop {r4, r5, r6, pc}
005b71c4 str r6, [r4, #0x218]
005b71c8 mov r0, r6
005b71cc bl #0x30ecdc
005b71d0 b #0x5b7148
005b71d4 eorseq r8, r2, r4, lsr pc

FUNCTION ApplyScissor 005b759c
005b759c push {r4, r5, r6, lr}
005b75a0 ldr r6, [r1]
005b75a4 ldrb r3, [r0, #0x1d3]
005b75a8 sub sp, sp, #0x20
005b75ac ubfx r6, r6, #0x15, #1
005b75b0 cmp r3, r6
005b75b4 mov r5, r1
005b75b8 mov r4, r0
005b75bc beq #0x5b75d4
005b75c0 cmp r6, #0
005b75c4 bne #0x5b76b4
005b75c8 movw r0, #0xc11
005b75cc bl #0x30df68
005b75d0 strb r6, [r4, #0x1d3]
005b75d4 ldr r2, [r4, #0xcc]
005b75d8 ldr r3, [r4, #0xc8]
005b75dc rsb r3, r3, r2
005b75e0 asr r3, r3, #2
005b75e4 cmp r3, #1
005b75e8 ldrls r6, [r4, #0x13c]
005b75ec ldr r3, [r4, #0x23c]
005b75f0 movhi r6, #0
005b75f4 cmp r6, r3
005b75f8 beq #0x5b7670
005b75fc add ip, sp, #0x14
005b7600 str ip, [sp]
005b7604 add ip, sp, #0x10
005b7608 str ip, [sp, #4]
005b760c mov ip, #1
005b7610 add r1, r5, #0x14
005b7614 add r2, sp, #0x1c
005b7618 add r3, sp, #0x18
005b761c str ip, [sp, #8]
005b7620 mov r0, r4
005b7624 mov ip, #0
005b7628 str ip, [sp, #0xc]
005b762c bl #0x6dd884
005b7630 ldr r3, [sp, #0x10]
005b7634 ldr r0, [sp, #0x1c]
005b7638 ldr r1, [sp, #0x18]
005b763c ldr r2, [sp, #0x14]
005b7640 bl #0x30e4f0
005b7644 ldr r3, [r5, #0x14]
005b7648 str r3, [r4, #0x22c]
005b764c ldr r3, [r5, #0x18]
005b7650 str r3, [r4, #0x230]
005b7654 ldr r3, [r5, #0x1c]
005b7658 str r3, [r4, #0x234]
005b765c ldr r3, [r5, #0x20]
005b7660 str r6, [r4, #0x23c]
005b7664 str r3, [r4, #0x238]
005b7668 add sp, sp, #0x20
005b766c pop {r4, r5, r6, pc}
005b7670 ldr r2, [r5, #0x14]
005b7674 ldr r3, [r4, #0x22c]
005b7678 cmp r2, r3
005b767c bne #0x5b75fc
005b7680 ldr r2, [r5, #0x18]
005b7684 ldr r3, [r4, #0x230]
005b7688 cmp r2, r3
005b768c bne #0x5b75fc
005b7690 ldr r2, [r5, #0x1c]
005b7694 ldr r3, [r4, #0x234]
005b7698 cmp r2, r3
005b769c bne #0x5b75fc
005b76a0 ldr r2, [r5, #0x20]
005b76a4 ldr r3, [r4, #0x238]
005b76a8 cmp r2, r3
005b76ac bne #0x5b75fc
005b76b0 b #0x5b7668
005b76b4 movw r0, #0xc11
005b76b8 bl #0x30e544
005b76bc strb r6, [r4, #0x1d3]
005b76c0 b #0x5b75d4

FUNCTION ApplyWholePass 005b71d8
005b71d8 push {r4, r5, r6, lr}
005b71dc ldr r3, [r0, #4]
005b71e0 mov r5, r0
005b71e4 mov r4, r1
005b71e8 ubfx r6, r3, #0x10, #1
005b71ec cmp r6, #0
005b71f0 bne #0x5b7300
005b71f4 ldrb r2, [r1, #0x1c4]
005b71f8 cmp r2, #0
005b71fc bne #0x5b7374
005b7200 ubfx r6, r3, #0x11, #1
005b7204 cmp r6, #0
005b7208 bne #0x5b731c
005b720c ldrb r2, [r4, #0x1c5]
005b7210 cmp r2, #0
005b7214 bne #0x5b734c
005b7218 ubfx r6, r3, #0x13, #1
005b721c cmp r6, #0
005b7220 bne #0x5b7338
005b7224 ldrb r2, [r4, #0x1c6]
005b7228 cmp r2, #0
005b722c bne #0x5b7388
005b7230 tst r3, #0x200000
005b7234 beq #0x5b7288
005b7238 mov r0, r4
005b723c mov r1, r5
005b7240 bl #0x5af7a4
005b7244 ldr r3, [r5, #4]
005b7248 ubfx r6, r3, #0x19, #1
005b724c cmp r6, #0
005b7250 bne #0x5b72c4
005b7254 ldrb r2, [r4, #0x1d1]
005b7258 cmp r2, #0
005b725c bne #0x5b7360
005b7260 ubfx r6, r3, #0x1b, #1
005b7264 cmp r6, #0
005b7268 bne #0x5b72e0
005b726c ldrb r3, [r4, #0x1d4]
005b7270 cmp r3, #0
005b7274 bne #0x5b72f0
005b7278 mov r0, r4
005b727c mov r1, r5
005b7280 pop {r4, r5, r6, lr}
005b7284 b #0x5b70cc
005b7288 tst r3, #0x400000
005b728c bne #0x5b7238
005b7290 ubfx r6, r3, #0x17, #1
005b7294 cmp r6, #0
005b7298 bne #0x5b7238
005b729c ldr r2, [r4, #0x1cc]
005b72a0 cmp r2, #0
005b72a4 beq #0x5b7248
005b72a8 movw r0, #0x8037
005b72ac bl #0x30df68
005b72b0 str r6, [r4, #0x1cc]
005b72b4 ldr r3, [r5, #4]
005b72b8 ubfx r6, r3, #0x19, #1
005b72bc cmp r6, #0
005b72c0 beq #0x5b7254
005b72c4 mov r0, r4
005b72c8 mov r1, r5
005b72cc bl #0x5af88c
005b72d0 ldr r3, [r5, #4]
005b72d4 ubfx r6, r3, #0x1b, #1
005b72d8 cmp r6, #0
005b72dc beq #0x5b726c
005b72e0 mov r0, r4
005b72e4 mov r1, r5
005b72e8 bl #0x5afa78
005b72ec b #0x5b7278
005b72f0 mov r0, #0xb90
005b72f4 bl #0x30df68
005b72f8 strb r6, [r4, #0x1d4]
005b72fc b #0x5b7278
005b7300 mov r0, r1
005b7304 mov r1, r5
005b7308 bl #0x5af3f8
005b730c ldr r3, [r5, #4]
005b7310 ubfx r6, r3, #0x11, #1
005b7314 cmp r6, #0
005b7318 beq #0x5b720c
005b731c mov r0, r4
005b7320 mov r1, r5
005b7324 bl #0x5af654
005b7328 ldr r3, [r5, #4]
005b732c ubfx r6, r3, #0x13, #1
005b7330 cmp r6, #0
005b7334 beq #0x5b7224
005b7338 mov r0, r4
005b733c mov r1, r5
005b7340 bl #0x5af6fc
005b7344 ldr r3, [r5, #4]
005b7348 b #0x5b7230
005b734c movw r0, #0xb44
005b7350 bl #0x30df68
005b7354 strb r6, [r4, #0x1c5]
005b7358 ldr r3, [r5, #4]
005b735c b #0x5b7218
005b7360 movw r0, #0x80a0
005b7364 bl #0x30df68
005b7368 strb r6, [r4, #0x1d1]
005b736c ldr r3, [r5, #4]
005b7370 b #0x5b7260
005b7374 movw r0, #0xbe2
005b7378 bl #0x30df68
005b737c strb r6, [r4, #0x1c4]
005b7380 ldr r3, [r5, #4]
005b7384 b #0x5b7200
005b7388 movw r0, #0xb71
005b738c bl #0x30df68
005b7390 strb r6, [r4, #0x1c6]
005b7394 ldr r3, [r5, #4]
005b7398 b #0x5b7230

FUNCTION ApplyRenderStates 005b73e0
005b73e0 push {r4, r5, r6, r7, r8, sb, sl, lr}
005b73e4 ldr ip, [r3, #0xf0]
005b73e8 ldr r4, [pc, #0xec]
005b73ec mov r5, r1
005b73f0 cmp ip, #0
005b73f4 add r4, pc, r4
005b73f8 mov r7, r2
005b73fc ldr r6, [r0, #4]
005b7400 beq #0x5b7410
005b7404 ldr r2, [ip, #4]
005b7408 cmp r6, r2
005b740c beq #0x5b7464
005b7410 mov r8, #0xc
005b7414 mul r8, r8, r5
005b7418 ldr r2, [r6, #0x18]
005b741c ldr sl, [pc, #0xbc]
005b7420 ldr sb, [pc, #0xbc]
005b7424 add r2, r2, r8
005b7428 ldr r2, [r2, #8]
005b742c mov r0, #0x34
005b7430 mla r0, r0, r7, r2
005b7434 mov r1, r3
005b7438 bl #0x5b71d8
005b743c ldr r3, [r6, #0x18]
005b7440 mov r2, #0
005b7444 add r8, r3, r8
005b7448 ldr r3, [r8, #8]
005b744c strb r2, [r3, #0x30]
005b7450 ldr r2, [r4, sl]
005b7454 ldr r3, [r4, sb]
005b7458 strb r5, [r2]
005b745c strb r7, [r3]
005b7460 pop {r4, r5, r6, r7, r8, sb, sl, pc}
005b7464 mov r8, #0xc
005b7468 mul r8, r8, r1
005b746c ldr r2, [r6, #0x18]
005b7470 add r2, r2, r8
005b7474 ldrb r1, [r2, #4]
005b7478 cmp r1, #1
005b747c bls #0x5b7490
005b7480 ldr r2, [r2, #8]
005b7484 ldr sl, [pc, #0x54]
005b7488 ldr sb, [pc, #0x54]
005b748c b #0x5b742c
005b7490 ldr r2, [r2, #8]
005b7494 ldrb r1, [r2, #0x30]
005b7498 cmp r1, #0
005b749c beq #0x5b74ac
005b74a0 ldr sl, [pc, #0x38]
005b74a4 ldr sb, [pc, #0x38]
005b74a8 b #0x5b742c
005b74ac ldr sb, [pc, #0x30]
005b74b0 ldr r1, [r4, sb]
005b74b4 ldrb r1, [r1]
005b74b8 cmp r1, r7
005b74bc ldrne sl, [pc, #0x1c]
005b74c0 bne #0x5b742c
005b74c4 ldr sl, [pc, #0x14]
005b74c8 ldr r1, [r4, sl]
005b74cc ldrb r1, [r1]
005b74d0 cmp r1, r5
005b74d4 bne #0x5b742c
005b74d8 b #0x5b7450
005b74dc mlaseq sp, ip, r6, sp
005b74e0 andeq r1, r0, r8, lsl #12
005b74e4 andeq r1, r0, r0, ror r5
