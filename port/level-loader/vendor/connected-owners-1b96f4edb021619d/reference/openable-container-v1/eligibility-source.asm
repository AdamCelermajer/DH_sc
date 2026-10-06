_ZNK10GameObject13MeetConditionEv
0038ab60 mov      r0, #1
0038ab64 bx       lr
_ZN10GameObject21CheckSpawnProbabilityEv
0038bd64 push     {r4, r5, lr}
0038bd68 sub      sp, sp, #0x14
0038bd6c add      r4, sp, #4
0038bd70 mov      r1, r0
0038bd74 mov      r5, r0
0038bd78 mov      r0, r4
0038bd7c bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
0038bd80 mov      r0, r4
0038bd84 bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
0038bd88 ldr      r4, [pc, #0xc4]
0038bd8c subs     r3, r0, #0
0038bd90 add      r4, pc, r4
0038bd94 beq      #0x38bdbc
0038bd98 ldr      r3, [r3]
0038bd9c mov      lr, pc
0038bda0 ldr      pc, [r3, #0x28]
0038bda4 cmp      r0, #0
0038bda8 beq      #0x38bdbc
0038bdac mvn      r0, #1
0038bdb0 str      r0, [r5, #0x270]
0038bdb4 add      sp, sp, #0x14
0038bdb8 pop      {r4, r5, pc}
0038bdbc ldr      r0, [r5, #0x270]
0038bdc0 cmn      r0, #1
0038bdc4 bne      #0x38bdb4
0038bdc8 bl       #0x7fd794 ; _Z9GetOnlinev
0038bdcc ldrb     r3, [r0, #5]
0038bdd0 cmp      r3, #0
0038bdd4 beq      #0x38be44
0038bdd8 ldr      r3, [r5, #0x108]
0038bddc cmn      r3, #1
0038bde0 beq      #0x38be44
0038bde4 ldr      r0, [r5, #0xfc]
0038bde8 subs     r0, r0, #0
0038bdec movne    r0, #1
0038bdf0 bl       #0x38bc3c ; _ZN6Random9GetRandomEib.clone.2
0038bdf4 str      r0, [r5, #0x270]
0038bdf8 ldr      r3, [r5, #0x274]
0038bdfc cmp      r0, r3
0038be00 blt      #0x38bdac
0038be04 mov      r1, #0
0038be08 ldr      r3, [r5]
0038be0c mov      r0, r5
0038be10 mov      lr, pc
0038be14 ldr      pc, [r3, #0x40]
0038be18 mov      r0, r5
0038be1c bl       #0x33ddb4 ; _ZN10ObjectBase6DeleteEv
0038be20 mov      r3, #0
0038be24 strb     r3, [r5, #0x82]
0038be28 ldr      r3, [pc, #0x28]
0038be2c mov      r1, r5
0038be30 ldr      r3, [r4, r3]
0038be34 ldr      r0, [r3, #0x38]
0038be38 bl       #0x3432f8 ; _ZN13ObjectManager15MarkForDeletionEP10ObjectBase
0038be3c ldr      r0, [r5, #0x270]
0038be40 b        #0x38bdb4
0038be44 mov      r0, #0
0038be48 bl       #0x38bc3c ; _ZN6Random9GetRandomEib.clone.2
0038be4c str      r0, [r5, #0x270]
0038be50 b        #0x38bdf8
0038be54 rsbeq    r8, r0, r0, lsl #26
0038be58 strdeq   r3, r4, [r0], -r4
